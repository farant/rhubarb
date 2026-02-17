/*
 * KNOTAPEL DEMO 10: Numerical Evaluation Engine
 * ===============================================
 *
 * Demos 04-05 computed the Kauffman bracket as a symbolic Laurent
 * polynomial in A using the quantum trace formula:
 *   bracket(b) = (-1)^{n+1} * tr(K^{xn} * rho(b)) / [2]_q
 *
 * Each matrix entry was a Poly (~808 bytes). For n=5 (32x32),
 * one matrix was ~800KB. Scaling beyond n=5 is impractical.
 *
 * This demo replaces symbolic polynomials with NUMERICAL evaluation:
 *   - Fix A = e^{i*theta} for a specific theta
 *   - Every Poly becomes a complex number (16 bytes)
 *   - All matrix operations become complex linear algebra
 *   - Same quantum trace formula, evaluated numerically
 *
 * VALIDATION: Evaluate the known symbolic bracket polynomials at the
 * same A value and confirm the numerical pipeline matches.
 *
 * SCALING: 50-100x memory reduction enables n=8 (256x256) easily,
 * n=10 (1024x1024) feasibly, n=12 (4096x4096) with patience.
 *
 * EXPERIMENT: How many sample points on the unit circle suffice to
 * distinguish all knots in a test set? This is the bridge to DKC.
 *
 * C89, zero dependencies beyond math.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ================================================================
 * Complex arithmetic (C89 compatible)
 * ================================================================ */

typedef struct { double re, im; } Cx;

static Cx cx_make(double re, double im) { Cx z; z.re = re; z.im = im; return z; }
static Cx cx_zero(void) { return cx_make(0.0, 0.0); }
static Cx cx_one(void)  { return cx_make(1.0, 0.0); }
static Cx cx_real(double x) { return cx_make(x, 0.0); }

static Cx cx_add(Cx a, Cx b) { return cx_make(a.re + b.re, a.im + b.im); }
static Cx cx_sub(Cx a, Cx b) { return cx_make(a.re - b.re, a.im - b.im); }

static Cx cx_mul(Cx a, Cx b) {
    return cx_make(a.re * b.re - a.im * b.im,
                   a.re * b.im + a.im * b.re);
}

static Cx cx_div(Cx a, Cx b) {
    double d = b.re * b.re + b.im * b.im;
    return cx_make((a.re * b.re + a.im * b.im) / d,
                   (a.im * b.re - a.re * b.im) / d);
}

static Cx cx_neg(Cx a) { return cx_make(-a.re, -a.im); }
static double cx_abs2(Cx a) { return a.re * a.re + a.im * a.im; }
static double cx_abs(Cx a) { return sqrt(cx_abs2(a)); }

/* e^{i*theta} */
static Cx cx_exp_i(double theta) { return cx_make(cos(theta), sin(theta)); }

/* A^n for A on unit circle: (e^{i*theta})^n = e^{i*n*theta} */
static Cx cx_pow_int(Cx a, int n) {
    Cx r = cx_one();
    Cx base;
    int neg;
    if (n == 0) return r;
    neg = (n < 0);
    if (neg) n = -n;
    base = a;
    while (n > 0) {
        if (n & 1) r = cx_mul(r, base);
        base = cx_mul(base, base);
        n >>= 1;
    }
    if (neg) r = cx_div(cx_one(), r);
    return r;
}

/* Approximate equality */
static int cx_near(Cx a, Cx b, double tol) {
    return cx_abs(cx_sub(a, b)) < tol;
}

static void cx_print(Cx z, const char *name) {
    printf("%s = (%.10f, %.10f)  |z|=%.10f\n", name, z.re, z.im, cx_abs(z));
}

/* ================================================================
 * Heap-allocated NxN complex matrix
 * ================================================================ */

typedef struct {
    Cx *data;   /* n*n entries, row-major */
    int n;
} CMat;

static CMat cmat_alloc(int n) {
    CMat m;
    m.n = n;
    m.data = (Cx *)calloc((size_t)(n * n), sizeof(Cx));
    if (!m.data) { fprintf(stderr, "cmat_alloc: out of memory (n=%d)\n", n); exit(1); }
    return m;
}

static void cmat_free(CMat *m) {
    if (m->data) { free(m->data); m->data = NULL; }
    m->n = 0;
}

#define CMAT(m, i, j) ((m)->data[(i) * (m)->n + (j)])

static void cmat_zero(CMat *m) {
    memset(m->data, 0, (size_t)(m->n * m->n) * sizeof(Cx));
}

static void cmat_id(CMat *m) {
    int i, j;
    for (i = 0; i < m->n; i++)
        for (j = 0; j < m->n; j++)
            CMAT(m, i, j) = (i == j) ? cx_one() : cx_zero();
}

static void cmat_mul(CMat *R, const CMat *A, const CMat *B) {
    CMat T;
    int i, j, k, n;
    n = A->n;
    T = cmat_alloc(n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            Cx s = cx_zero();
            for (k = 0; k < n; k++)
                s = cx_add(s, cx_mul(CMAT(A, i, k), CMAT(B, k, j)));
            CMAT(&T, i, j) = s;
        }
    memcpy(R->data, T.data, (size_t)(n * n) * sizeof(Cx));
    cmat_free(&T);
}

static void cmat_scale(CMat *R, const CMat *M, Cx s) {
    int i, j, n;
    n = M->n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            CMAT(R, i, j) = cx_mul(CMAT(M, i, j), s);
}

static void cmat_add(CMat *R, const CMat *A, const CMat *B) {
    int i, j, n;
    n = A->n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            CMAT(R, i, j) = cx_add(CMAT(A, i, j), CMAT(B, i, j));
}

/* Quantum trace: tr(diag_weights * M) = sum_i w[i] * M[i][i] */
static Cx cmat_qtrace(const CMat *M, const Cx *w) {
    int i;
    Cx s = cx_zero();
    for (i = 0; i < M->n; i++)
        s = cx_add(s, cx_mul(w[i], CMAT(M, i, i)));
    return s;
}

/* Tensor product: C = A x B */
static void cmat_tensor(CMat *C, const CMat *A, const CMat *B) {
    int i1, i2, j1, j2, n1, n2;
    n1 = A->n; n2 = B->n;
    for (i1 = 0; i1 < n1; i1++)
        for (i2 = 0; i2 < n2; i2++)
            for (j1 = 0; j1 < n1; j1++)
                for (j2 = 0; j2 < n2; j2++)
                    CMAT(C, i1*n2+i2, j1*n2+j2) =
                        cx_mul(CMAT(A, i1, j1), CMAT(B, i2, j2));
}

/* Approximate matrix equality */
static int cmat_eq_approx(const CMat *A, const CMat *B, double tol) {
    int i, j;
    if (A->n != B->n) return 0;
    for (i = 0; i < A->n; i++)
        for (j = 0; j < A->n; j++)
            if (!cx_near(CMAT(A, i, j), CMAT(B, i, j), tol))
                return 0;
    return 1;
}

/* ================================================================
 * Braid type and state-sum oracle (symbolic, for validation)
 *
 * The oracle evaluates the Kauffman bracket as a Laurent polynomial,
 * then we evaluate that polynomial at a specific A to get a complex
 * number to compare against the numerical pipeline.
 * ================================================================ */

#define MAX_WORD 40

typedef struct { int word[MAX_WORD]; int len, n; } Braid;

/* State-sum bracket as polynomial, then evaluate at A */
#define MAX_UF 2048
static int uf_p[MAX_UF];
static void uf_init(int n) { int i; for (i = 0; i < n; i++) uf_p[i] = i; }
static int uf_find(int x) {
    while (uf_p[x] != x) { uf_p[x] = uf_p[uf_p[x]]; x = uf_p[x]; }
    return x;
}
static void uf_union(int x, int y) {
    x = uf_find(x); y = uf_find(y); if (x != y) uf_p[x] = y;
}

static int braid_loops(const Braid *b, unsigned s) {
    int N = (b->len + 1) * b->n, l, p, i, loops, sgn, bit, cup;
    uf_init(N);
    for (l = 0; l < b->len; l++) {
        sgn = b->word[l] > 0 ? 1 : -1;
        i = (sgn > 0 ? b->word[l] : -b->word[l]) - 1;
        bit = (int)((s >> l) & 1u);
        cup = (sgn > 0) ? (bit == 0) : (bit == 1);
        if (cup) {
            uf_union(l * b->n + i, l * b->n + i + 1);
            uf_union((l + 1) * b->n + i, (l + 1) * b->n + i + 1);
            for (p = 0; p < b->n; p++)
                if (p != i && p != i + 1)
                    uf_union(l * b->n + p, (l + 1) * b->n + p);
        } else {
            for (p = 0; p < b->n; p++)
                uf_union(l * b->n + p, (l + 1) * b->n + p);
        }
    }
    for (p = 0; p < b->n; p++)
        uf_union(p, b->len * b->n + p);
    loops = 0;
    for (i = 0; i < N; i++)
        if (uf_find(i) == i) loops++;
    return loops;
}

/* Evaluate state-sum bracket directly at a specific A (complex) */
static Cx braid_bracket_at(const Braid *b, Cx A) {
    unsigned s, ns;
    int i, a_count, b_count, lp, j;
    Cx result, delta, d_power, term, coeff;

    /* delta = -A^2 - A^{-2} */
    delta = cx_neg(cx_add(cx_pow_int(A, 2), cx_pow_int(A, -2)));

    result = cx_zero();
    if (!b->len) {
        /* Identity braid: bracket = delta^{n-1} */
        result = cx_one();
        for (i = 0; i < b->n - 1; i++)
            result = cx_mul(result, delta);
        return result;
    }

    ns = 1u << b->len;
    for (s = 0; s < ns; s++) {
        a_count = 0; b_count = 0;
        for (i = 0; i < b->len; i++) {
            if ((s >> (unsigned)i) & 1u) b_count++;
            else a_count++;
        }
        lp = braid_loops(b, s);

        /* A^{a-b} * delta^{loops-1} */
        coeff = cx_pow_int(A, a_count - b_count);
        d_power = cx_one();
        for (j = 0; j < lp - 1; j++)
            d_power = cx_mul(d_power, delta);
        term = cx_mul(coeff, d_power);
        result = cx_add(result, term);
    }
    return result;
}

/* ================================================================
 * Numerical R-matrix construction
 * ================================================================ */

/* Build 4x4 R and R^{-1} for a given A */
static void build_R4(CMat *R, CMat *Rinv, Cx A) {
    CMat U, I4, AU, AinvI, AinvU, AI;
    Cx Ainv, neg_A2, neg_Ainv2;

    Ainv = cx_div(cx_one(), A);
    neg_A2 = cx_neg(cx_pow_int(A, 2));
    neg_Ainv2 = cx_neg(cx_pow_int(A, -2));

    U = cmat_alloc(4); I4 = cmat_alloc(4);
    AU = cmat_alloc(4); AinvI = cmat_alloc(4);
    AinvU = cmat_alloc(4); AI = cmat_alloc(4);

    /* U: TL cup-cap on V x V */
    cmat_zero(&U);
    CMAT(&U, 1, 1) = neg_Ainv2;    /* -A^{-2} */
    CMAT(&U, 1, 2) = cx_one();     /* 1 */
    CMAT(&U, 2, 1) = cx_one();     /* 1 */
    CMAT(&U, 2, 2) = neg_A2;       /* -A^2 */

    /* R = A*U + A^{-1}*I */
    cmat_id(&I4);
    cmat_scale(&AU, &U, A);
    cmat_scale(&AinvI, &I4, Ainv);
    cmat_add(R, &AU, &AinvI);

    /* R^{-1} = A^{-1}*U + A*I */
    cmat_scale(&AinvU, &U, Ainv);
    cmat_scale(&AI, &I4, A);
    cmat_add(Rinv, &AinvU, &AI);

    cmat_free(&U); cmat_free(&I4);
    cmat_free(&AU); cmat_free(&AinvI);
    cmat_free(&AinvU); cmat_free(&AI);
}

/* Build R_i = I^{x(i-1)} x R x I^{x(n-i-1)} for n strands */
static void build_Ri(CMat *result, const CMat *R4, int i, int n) {
    int left_dim, right_dim;
    CMat Ileft, Iright, temp;

    left_dim = 1 << (i - 1);
    right_dim = 1 << (n - i - 1);

    if (left_dim == 1) {
        if (right_dim == 1) {
            memcpy(result->data, R4->data, (size_t)(4 * 4) * sizeof(Cx));
            return;
        }
        Iright = cmat_alloc(right_dim);
        cmat_id(&Iright);
        cmat_tensor(result, R4, &Iright);
        cmat_free(&Iright);
    } else if (right_dim == 1) {
        Ileft = cmat_alloc(left_dim);
        cmat_id(&Ileft);
        cmat_tensor(result, &Ileft, R4);
        cmat_free(&Ileft);
    } else {
        Ileft = cmat_alloc(left_dim);
        cmat_id(&Ileft);
        temp = cmat_alloc(left_dim * 4);
        cmat_tensor(&temp, &Ileft, R4);

        Iright = cmat_alloc(right_dim);
        cmat_id(&Iright);
        cmat_tensor(result, &temp, &Iright);

        cmat_free(&Ileft); cmat_free(&temp); cmat_free(&Iright);
    }
}

/* Build K^{xn} diagonal weights for quantum trace */
static void build_Kn(Cx *w, int n, Cx A) {
    int dim = 1 << n;
    int idx, bit, s;
    Cx k[2], t;
    k[0] = cx_pow_int(A, 2);    /* A^2 */
    k[1] = cx_pow_int(A, -2);   /* A^{-2} */

    for (idx = 0; idx < dim; idx++) {
        w[idx] = cx_one();
        for (bit = n - 1; bit >= 0; bit--) {
            s = (idx >> bit) & 1;
            t = cx_mul(w[idx], k[s]);
            w[idx] = t;
        }
    }
}

/* Compute braid product on 2^n x 2^n space */
static void braid_product_n(CMat *result, const Braid *b,
                            CMat *Ri, CMat *Ri_inv) {
    int i, j, gen;
    cmat_id(result);
    for (i = 0; i < b->len; i++) {
        j = b->word[i];
        gen = j > 0 ? j : -j;
        if (j > 0) {
            cmat_mul(result, result, &Ri[gen - 1]);
        } else {
            cmat_mul(result, result, &Ri_inv[gen - 1]);
        }
    }
}

/* Compute numerical bracket:
 * bracket(A) = (-1)^{n+1} * tr_q(K^{xn} * rho(b)) / [2]_q */
static Cx numerical_bracket(const Braid *b, Cx A,
                            CMat *Ri, CMat *Ri_inv, Cx *Kw) {
    int dim = 1 << b->n;
    CMat prod;
    Cx qtrace, neg_delta, bracket, sign;

    prod = cmat_alloc(dim);
    braid_product_n(&prod, b, Ri, Ri_inv);
    qtrace = cmat_qtrace(&prod, Kw);
    cmat_free(&prod);

    /* [2]_q = A^2 + A^{-2} = -delta */
    neg_delta = cx_add(cx_pow_int(A, 2), cx_pow_int(A, -2));

    /* bracket = (-1)^{n+1} * qtrace / [2]_q */
    sign = ((b->n + 1) % 2 == 0) ? cx_one() : cx_real(-1.0);
    bracket = cx_mul(sign, cx_div(qtrace, neg_delta));
    return bracket;
}

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;
#define TOL 1e-9

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * PART A: Complex arithmetic validation
 * ================================================================ */

static void test_part_a(void) {
    Cx A, A2, Ainv2, sum, R_11;

    printf("\n=== PART A: Complex Arithmetic Validation ===\n");

    /* Test at A = e^{i*pi/5} */
    A = cx_exp_i(M_PI / 5.0);
    printf("  A = e^{i*pi/5}\n");
    cx_print(A, "  A");

    /* |A| = 1 */
    check("|A| = 1", fabs(cx_abs(A) - 1.0) < TOL);

    /* A * A^{-1} = 1 */
    check("A * A^{-1} = 1", cx_near(cx_mul(A, cx_div(cx_one(), A)), cx_one(), TOL));

    /* A^2 + A^{-2} should be real for these special angles */
    A2 = cx_pow_int(A, 2);
    Ainv2 = cx_pow_int(A, -2);
    sum = cx_add(A2, Ainv2);
    printf("  A^2 + A^{-2} = (%.10f, %.10f)\n", sum.re, sum.im);
    /* For A = e^{i*pi/5}: A^2 = e^{i*2pi/5}, A^{-2} = e^{-i*2pi/5}
     * sum = 2*cos(2pi/5) = 2*(sqrt(5)-1)/4 = (sqrt(5)-1)/2 ~ 0.618 */
    check("A^2 + A^{-2} = 2*cos(2pi/5)", fabs(sum.re - 2.0*cos(2.0*M_PI/5.0)) < TOL && fabs(sum.im) < TOL);

    /* R-matrix entry test: -A^{-2} */
    R_11 = cx_neg(Ainv2);
    printf("  -A^{-2}: (%.10f, %.10f)\n", R_11.re, R_11.im);

    /* cx_pow_int path test */
    check("A^0 = 1", cx_near(cx_pow_int(A, 0), cx_one(), TOL));
    check("A^1 = A", cx_near(cx_pow_int(A, 1), A, TOL));
    check("A^5 = e^{i*pi} = -1", cx_near(cx_pow_int(A, 5), cx_real(-1.0), TOL));
    check("A^10 = 1", cx_near(cx_pow_int(A, 10), cx_one(), TOL));
}

/* ================================================================
 * PART B: Numerical R-matrix and Yang-Baxter
 * ================================================================ */

static void test_part_b(Cx A) {
    CMat R4, R4inv, I4, prod;
    CMat R1, R2, R1inv, R2inv;
    CMat t, lhs, rhs;
    CMat I8;

    printf("\n=== PART B: Numerical R-matrix (8x8 Yang-Baxter) ===\n");
    cx_print(A, "  A");

    R4 = cmat_alloc(4); R4inv = cmat_alloc(4);
    build_R4(&R4, &R4inv, A);

    /* R * R^{-1} = I */
    I4 = cmat_alloc(4); prod = cmat_alloc(4);
    cmat_id(&I4);
    cmat_mul(&prod, &R4, &R4inv);
    check("R4 * R4inv = I (4x4)", cmat_eq_approx(&prod, &I4, TOL));
    cmat_free(&I4); cmat_free(&prod);

    /* Build 8x8 generators for n=3 */
    R1 = cmat_alloc(8); R2 = cmat_alloc(8);
    R1inv = cmat_alloc(8); R2inv = cmat_alloc(8);
    {
        CMat I2;
        I2 = cmat_alloc(2); cmat_id(&I2);
        cmat_tensor(&R1, &R4, &I2);
        cmat_tensor(&R2, &I2, &R4);
        cmat_tensor(&R1inv, &R4inv, &I2);
        cmat_tensor(&R2inv, &I2, &R4inv);
        cmat_free(&I2);
    }

    /* Yang-Baxter: R1*R2*R1 = R2*R1*R2 */
    t = cmat_alloc(8); lhs = cmat_alloc(8); rhs = cmat_alloc(8);
    cmat_mul(&t, &R1, &R2);
    cmat_mul(&lhs, &t, &R1);
    cmat_mul(&t, &R2, &R1);
    cmat_mul(&rhs, &t, &R2);
    check("R1*R2*R1 = R2*R1*R2 (Yang-Baxter, 8x8)", cmat_eq_approx(&lhs, &rhs, TOL));

    /* R1*R1inv = I */
    I8 = cmat_alloc(8);
    cmat_id(&I8);
    cmat_mul(&t, &R1, &R1inv);
    check("R1 * R1inv = I (8x8)", cmat_eq_approx(&t, &I8, TOL));
    cmat_mul(&t, &R2, &R2inv);
    check("R2 * R2inv = I (8x8)", cmat_eq_approx(&t, &I8, TOL));

    cmat_free(&I8); cmat_free(&t); cmat_free(&lhs); cmat_free(&rhs);
    cmat_free(&R1); cmat_free(&R2); cmat_free(&R1inv); cmat_free(&R2inv);
    cmat_free(&R4); cmat_free(&R4inv);
}

/* ================================================================
 * PART C: Bracket validation against state-sum oracle
 * ================================================================ */

static void test_part_c(Cx A) {
    int dim, ngen, i;
    CMat R4, R4inv;
    CMat *Ri, *Ri_inv;
    Cx *Kw;
    Braid braids[5];
    const char *bnames[5];
    int nbr, bi;
    char msg[120];
    int n = 3;

    printf("\n=== PART C: Bracket vs State-Sum Oracle (n=3) ===\n");
    cx_print(A, "  A");

    dim = 1 << n;
    ngen = n - 1;

    R4 = cmat_alloc(4); R4inv = cmat_alloc(4);
    build_R4(&R4, &R4inv, A);

    Ri = (CMat *)malloc((size_t)ngen * sizeof(CMat));
    Ri_inv = (CMat *)malloc((size_t)ngen * sizeof(CMat));
    for (i = 0; i < ngen; i++) {
        Ri[i] = cmat_alloc(dim);
        Ri_inv[i] = cmat_alloc(dim);
        build_Ri(&Ri[i], &R4, i + 1, n);
        build_Ri(&Ri_inv[i], &R4inv, i + 1, n);
    }

    Kw = (Cx *)calloc((size_t)dim, sizeof(Cx));
    build_Kn(Kw, n, A);

    /* Test braids */
    nbr = 0;

    braids[nbr].n = 3; braids[nbr].len = 0;
    bnames[nbr] = "identity (3-unlink)";
    nbr++;

    braids[nbr].n = 3; braids[nbr].len = 3;
    braids[nbr].word[0] = 1; braids[nbr].word[1] = 2; braids[nbr].word[2] = 1;
    bnames[nbr] = "trefoil";
    nbr++;

    braids[nbr].n = 3; braids[nbr].len = 4;
    braids[nbr].word[0] = 1; braids[nbr].word[1] = -2;
    braids[nbr].word[2] = 1; braids[nbr].word[3] = -2;
    bnames[nbr] = "figure-eight";
    nbr++;

    braids[nbr].n = 3; braids[nbr].len = 3;
    braids[nbr].word[0] = -1; braids[nbr].word[1] = -2; braids[nbr].word[2] = -1;
    bnames[nbr] = "mirror trefoil";
    nbr++;

    braids[nbr].n = 3; braids[nbr].len = 1;
    braids[nbr].word[0] = 1;
    bnames[nbr] = "single crossing";
    nbr++;

    for (bi = 0; bi < nbr; bi++) {
        Cx oracle_val, numerical_val;

        oracle_val = braid_bracket_at(&braids[bi], A);
        numerical_val = numerical_bracket(&braids[bi], A, Ri, Ri_inv, Kw);

        printf("  %s:\n", bnames[bi]);
        cx_print(oracle_val, "    oracle   ");
        cx_print(numerical_val, "    numerical");
        printf("    diff = %.2e\n", cx_abs(cx_sub(oracle_val, numerical_val)));

        sprintf(msg, "%s: numerical matches oracle", bnames[bi]);
        check(msg, cx_near(oracle_val, numerical_val, TOL));
    }

    free(Kw);
    for (i = 0; i < ngen; i++) {
        cmat_free(&Ri[i]); cmat_free(&Ri_inv[i]);
    }
    free(Ri); free(Ri_inv);
    cmat_free(&R4); cmat_free(&R4inv);
}

/* ================================================================
 * PART D: Scaling test (n=2..8)
 * ================================================================ */

static void test_n_strands(int n, Cx A) {
    int dim = 1 << n;
    int ngen = n - 1;
    int i;
    CMat R4, R4inv;
    CMat *Ri, *Ri_inv;
    Cx *Kw;
    char msg[120];
    Braid braids[4];
    const char *bnames[4];
    int nbr, bi;

    printf("\n  --- n=%d (%dx%d, %d generators) ---\n", n, dim, dim, ngen);

    R4 = cmat_alloc(4); R4inv = cmat_alloc(4);
    build_R4(&R4, &R4inv, A);

    Ri = (CMat *)malloc((size_t)ngen * sizeof(CMat));
    Ri_inv = (CMat *)malloc((size_t)ngen * sizeof(CMat));
    for (i = 0; i < ngen; i++) {
        Ri[i] = cmat_alloc(dim);
        Ri_inv[i] = cmat_alloc(dim);
        build_Ri(&Ri[i], &R4, i + 1, n);
        build_Ri(&Ri_inv[i], &R4inv, i + 1, n);
    }

    /* Yang-Baxter check */
    {
        CMat t, lhs, rhs;
        t = cmat_alloc(dim); lhs = cmat_alloc(dim); rhs = cmat_alloc(dim);
        for (i = 0; i < ngen - 1; i++) {
            cmat_mul(&t, &Ri[i], &Ri[i + 1]);
            cmat_mul(&lhs, &t, &Ri[i]);
            cmat_mul(&t, &Ri[i + 1], &Ri[i]);
            cmat_mul(&rhs, &t, &Ri[i + 1]);
            sprintf(msg, "YB: R%d*R%d*R%d = R%d*R%d*R%d (%dx%d)",
                    i+1, i+2, i+1, i+2, i+1, i+2, dim, dim);
            check(msg, cmat_eq_approx(&lhs, &rhs, TOL));
        }
        cmat_free(&t); cmat_free(&lhs); cmat_free(&rhs);
    }

    /* Far commutativity for n >= 4 */
    if (ngen >= 3) {
        CMat ab, ba;
        int j;
        ab = cmat_alloc(dim); ba = cmat_alloc(dim);
        for (i = 0; i < ngen; i++)
            for (j = i + 2; j < ngen; j++) {
                cmat_mul(&ab, &Ri[i], &Ri[j]);
                cmat_mul(&ba, &Ri[j], &Ri[i]);
                sprintf(msg, "Far: R%d*R%d = R%d*R%d (%dx%d)",
                        i+1, j+1, j+1, i+1, dim, dim);
                check(msg, cmat_eq_approx(&ab, &ba, TOL));
            }
        cmat_free(&ab); cmat_free(&ba);
    }

    /* Quantum trace vs oracle */
    Kw = (Cx *)calloc((size_t)dim, sizeof(Cx));
    build_Kn(Kw, n, A);

    nbr = 0;
    braids[nbr].n = n; braids[nbr].len = 0;
    bnames[nbr] = "identity";
    nbr++;

    braids[nbr].n = n; braids[nbr].len = 1;
    braids[nbr].word[0] = 1;
    bnames[nbr] = "sigma_1";
    nbr++;

    if (n >= 3) {
        braids[nbr].n = n; braids[nbr].len = 2;
        braids[nbr].word[0] = 1; braids[nbr].word[1] = 2;
        bnames[nbr] = "sigma_1*sigma_2";
        nbr++;
    }

    if (n >= 4) {
        braids[nbr].n = n; braids[nbr].len = 2;
        braids[nbr].word[0] = 1; braids[nbr].word[1] = 3;
        bnames[nbr] = "sigma_1*sigma_3";
        nbr++;
    }

    for (bi = 0; bi < nbr; bi++) {
        Cx oracle_val, numerical_val;
        oracle_val = braid_bracket_at(&braids[bi], A);
        numerical_val = numerical_bracket(&braids[bi], A, Ri, Ri_inv, Kw);

        sprintf(msg, "n=%d %s: bracket matches oracle", n, bnames[bi]);
        check(msg, cx_near(oracle_val, numerical_val, TOL));
    }

    free(Kw);
    for (i = 0; i < ngen; i++) {
        cmat_free(&Ri[i]); cmat_free(&Ri_inv[i]);
    }
    free(Ri); free(Ri_inv);
    cmat_free(&R4); cmat_free(&R4inv);
}

static void test_part_d(void) {
    Cx A;
    int n;

    printf("\n=== PART D: Scaling Test ===\n");
    A = cx_exp_i(M_PI / 7.0);
    cx_print(A, "  A = e^{i*pi/7}");

    for (n = 2; n <= 8; n++) {
        test_n_strands(n, A);
    }
}

/* ================================================================
 * PART E: Multi-angle validation
 * Confirm numerical bracket gives same result at different angles
 * for known knots (trefoil, figure-eight)
 * ================================================================ */

static void test_part_e(void) {
    double thetas[5];
    const char *tnames[5];
    Braid trefoil, fig8;
    int ti;
    char msg[120];

    printf("\n=== PART E: Multi-Angle Validation ===\n");

    thetas[0] = M_PI / 5.0;  tnames[0] = "pi/5";
    thetas[1] = M_PI / 7.0;  tnames[1] = "pi/7";
    thetas[2] = M_PI / 10.0; tnames[2] = "pi/10";
    thetas[3] = M_PI / 3.0;  tnames[3] = "pi/3";
    thetas[4] = 0.7;          tnames[4] = "0.7";

    trefoil.n = 3; trefoil.len = 3;
    trefoil.word[0] = 1; trefoil.word[1] = 2; trefoil.word[2] = 1;

    fig8.n = 3; fig8.len = 4;
    fig8.word[0] = 1; fig8.word[1] = -2;
    fig8.word[2] = 1; fig8.word[3] = -2;

    for (ti = 0; ti < 5; ti++) {
        Cx A = cx_exp_i(thetas[ti]);
        int dim = 1 << 3;
        int ngen = 2, i;
        CMat R4, R4inv;
        CMat *Ri, *Ri_inv;
        Cx *Kw;
        Cx oracle_t, num_t, oracle_f, num_f;

        R4 = cmat_alloc(4); R4inv = cmat_alloc(4);
        build_R4(&R4, &R4inv, A);

        Ri = (CMat *)malloc((size_t)ngen * sizeof(CMat));
        Ri_inv = (CMat *)malloc((size_t)ngen * sizeof(CMat));
        for (i = 0; i < ngen; i++) {
            Ri[i] = cmat_alloc(dim);
            Ri_inv[i] = cmat_alloc(dim);
            build_Ri(&Ri[i], &R4, i + 1, 3);
            build_Ri(&Ri_inv[i], &R4inv, i + 1, 3);
        }

        Kw = (Cx *)calloc((size_t)dim, sizeof(Cx));
        build_Kn(Kw, 3, A);

        oracle_t = braid_bracket_at(&trefoil, A);
        num_t = numerical_bracket(&trefoil, A, Ri, Ri_inv, Kw);

        oracle_f = braid_bracket_at(&fig8, A);
        num_f = numerical_bracket(&fig8, A, Ri, Ri_inv, Kw);

        sprintf(msg, "trefoil at theta=%s", tnames[ti]);
        check(msg, cx_near(oracle_t, num_t, TOL));

        sprintf(msg, "figure-eight at theta=%s", tnames[ti]);
        check(msg, cx_near(oracle_f, num_f, TOL));

        free(Kw);
        for (i = 0; i < ngen; i++) {
            cmat_free(&Ri[i]); cmat_free(&Ri_inv[i]);
        }
        free(Ri); free(Ri_inv);
        cmat_free(&R4); cmat_free(&R4inv);
    }
}

/* ================================================================
 * PART F: Sample-point classification experiment
 * Given M sample angles, compute bracket vectors and check
 * whether they distinguish test knots.
 * ================================================================ */

static void test_part_f(void) {
    /* Test knots as 3-strand braids */
    Braid knots[6];
    const char *knames[6];
    int nk = 0;

    /* Sample angles */
    double angles[8];
    int na, ki, kj, ai;
    int distinguishes;
    char msg[120];

    printf("\n=== PART F: Sample-Point Classification ===\n");
    printf("  How many evaluation points to distinguish knots?\n\n");

    /* unknot (identity braid) */
    knots[nk].n = 3; knots[nk].len = 0;
    knames[nk] = "unknot";
    nk++;

    /* trefoil */
    knots[nk].n = 3; knots[nk].len = 3;
    knots[nk].word[0] = 1; knots[nk].word[1] = 2; knots[nk].word[2] = 1;
    knames[nk] = "trefoil";
    nk++;

    /* mirror trefoil */
    knots[nk].n = 3; knots[nk].len = 3;
    knots[nk].word[0] = -1; knots[nk].word[1] = -2; knots[nk].word[2] = -1;
    knames[nk] = "mirror trefoil";
    nk++;

    /* figure-eight */
    knots[nk].n = 3; knots[nk].len = 4;
    knots[nk].word[0] = 1; knots[nk].word[1] = -2;
    knots[nk].word[2] = 1; knots[nk].word[3] = -2;
    knames[nk] = "figure-eight";
    nk++;

    /* (2,5) torus knot: sigma_1^5 on 2 strands but let's use 3-strand */
    /* 5_1 knot: sigma_1*sigma_2*sigma_1*sigma_2*sigma_1 */
    knots[nk].n = 3; knots[nk].len = 5;
    knots[nk].word[0] = 1; knots[nk].word[1] = 2;
    knots[nk].word[2] = 1; knots[nk].word[3] = 2;
    knots[nk].word[4] = 1;
    knames[nk] = "torus(2,5)";
    nk++;

    /* Granny knot: trefoil#trefoil (6 crossings, not prime but good test) */
    knots[nk].n = 3; knots[nk].len = 6;
    knots[nk].word[0] = 1; knots[nk].word[1] = 2; knots[nk].word[2] = 1;
    knots[nk].word[3] = 1; knots[nk].word[4] = 2; knots[nk].word[5] = 1;
    knames[nk] = "granny (trefoil#trefoil)";
    nk++;

    /* Pick 8 sample angles */
    angles[0] = M_PI / 5.0;
    angles[1] = M_PI / 7.0;
    angles[2] = M_PI / 10.0;
    angles[3] = M_PI / 3.0;
    angles[4] = 0.7;
    angles[5] = 1.1;
    angles[6] = 2.0;
    angles[7] = M_PI / 13.0;

    /* For each number of sample points 1..8, check if all pairs distinguished */
    for (na = 1; na <= 8; na++) {
        Cx vectors[6][8]; /* knot x angle */

        /* Compute bracket at first na angles for each knot */
        for (ki = 0; ki < nk; ki++) {
            for (ai = 0; ai < na; ai++) {
                vectors[ki][ai] = braid_bracket_at(&knots[ki], cx_exp_i(angles[ai]));
            }
        }

        /* Check all pairs */
        distinguishes = 1;
        for (ki = 0; ki < nk && distinguishes; ki++) {
            for (kj = ki + 1; kj < nk && distinguishes; kj++) {
                int same = 1;
                for (ai = 0; ai < na; ai++) {
                    if (!cx_near(vectors[ki][ai], vectors[kj][ai], TOL)) {
                        same = 0;
                        break;
                    }
                }
                if (same) {
                    printf("  %d angles: %s == %s (indistinguishable)\n",
                           na, knames[ki], knames[kj]);
                    distinguishes = 0;
                }
            }
        }

        sprintf(msg, "%d sample point(s): all %d knots distinguished", na, nk);
        if (distinguishes) {
            check(msg, 1);
            printf("  --> %d sample points suffice for %d-knot table!\n", na, nk);
            break;
        } else {
            check(msg, 0);
        }
    }

    /* Print the bracket vectors for the winning number of points */
    printf("\n  Bracket vectors (first 3 angles):\n");
    for (ki = 0; ki < nk; ki++) {
        Cx v0, v1, v2;
        v0 = braid_bracket_at(&knots[ki], cx_exp_i(angles[0]));
        v1 = braid_bracket_at(&knots[ki], cx_exp_i(angles[1]));
        v2 = braid_bracket_at(&knots[ki], cx_exp_i(angles[2]));
        printf("  %s:\n", knames[ki]);
        printf("    [0] = (%.6f, %.6f)\n", v0.re, v0.im);
        printf("    [1] = (%.6f, %.6f)\n", v1.re, v1.im);
        printf("    [2] = (%.6f, %.6f)\n", v2.re, v2.im);
    }
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    Cx A;

    printf("KNOTAPEL DEMO 10: Numerical Evaluation Engine\n");
    printf("===============================================\n");

    test_part_a();

    A = cx_exp_i(M_PI / 7.0);
    test_part_b(A);
    test_part_c(A);
    test_part_d();
    test_part_e();
    test_part_f();

    printf("\n===============================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("===============================================\n");
    return n_fail > 0 ? 1 : 0;
}
