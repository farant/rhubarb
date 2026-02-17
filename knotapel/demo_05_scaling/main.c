/*
 * KNOTAPEL DEMO 05: Scaling the Spin Chain
 * ==========================================
 *
 * Demo 04 proved the quantum trace formula for n=3 strands (8x8 matrices):
 *   bracket(b) = tr(K^{x3} * rho(b)) / [2]_q
 *   where K = diag(A^2, A^{-2}), [2]_q = A^2 + A^{-2} = -delta
 *
 * This demo tests whether the SAME formula works for ANY number of strands:
 *   n=2:  4x4    (1 generator)
 *   n=3:  8x8    (2 generators, Demo 04 already proved this)
 *   n=4: 16x16   (3 generators)
 *   n=5: 32x32   (4 generators)
 *
 * For n strands, R_i = I^{x(i-1)} x R x I^{x(n-i-1)} is 2^n x 2^n.
 * The braid group B_n has generators sigma_1,...,sigma_{n-1} with relations:
 *   R_i R_{i+1} R_i = R_{i+1} R_i R_{i+1}  (adjacent Yang-Baxter)
 *   R_i R_j = R_j R_i  for |i-j| >= 2      (far commutativity)
 *
 * PLAN:
 *   Part A: Quantum dimension formula [n]_q recurrence
 *   Part B: n=2 (warmup: 4x4, one generator, simplest case)
 *   Part C: n=4 (16x16, three generators, Yang-Baxter + far commutativity)
 *   Part D: n=5 (32x32, four generators, Fibonacci TQFT boundary)
 *
 * C89, zero dependencies. Heap-allocated matrices for n >= 4.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Laurent polynomial ring (from Demo 03/04)
 * ================================================================ */
#define MAX_TERMS 200

typedef struct { int c[MAX_TERMS]; int lo, len; } Poly;

static void p_zero(Poly *p) { memset(p, 0, sizeof(Poly)); }

static void p_mono(Poly *p, int coeff, int exp) {
    p_zero(p);
    if (!coeff) return;
    p->lo = exp; p->len = 1; p->c[0] = coeff;
}

static void p_trim(Poly *p) {
    int a = 0, b;
    if (!p->len) return;
    while (a < p->len && !p->c[a]) a++;
    if (a == p->len) { p_zero(p); return; }
    b = p->len - 1;
    while (b > a && !p->c[b]) b--;
    if (a > 0) memmove(p->c, p->c + a, (size_t)(b - a + 1) * sizeof(int));
    p->lo += a; p->len = b - a + 1;
}

static void p_add(Poly *r, const Poly *a, const Poly *b) {
    Poly t; int lo, hi, i;
    if (!a->len) { *r = *b; return; }
    if (!b->len) { *r = *a; return; }
    lo = a->lo < b->lo ? a->lo : b->lo;
    hi = (a->lo + a->len - 1) > (b->lo + b->len - 1)
       ? (a->lo + a->len - 1) : (b->lo + b->len - 1);
    p_zero(&t); t.lo = lo; t.len = hi - lo + 1;
    for (i = 0; i < a->len; i++) t.c[(a->lo + i) - lo] += a->c[i];
    for (i = 0; i < b->len; i++) t.c[(b->lo + i) - lo] += b->c[i];
    p_trim(&t); *r = t;
}

static void p_neg(Poly *r, const Poly *a) {
    int i;
    *r = *a;
    for (i = 0; i < r->len; i++) r->c[i] = -r->c[i];
}

static void p_mul(Poly *r, const Poly *a, const Poly *b) {
    Poly t; int i, j;
    if (!a->len || !b->len) { p_zero(r); return; }
    p_zero(&t); t.lo = a->lo + b->lo; t.len = a->len + b->len - 1;
    for (i = 0; i < a->len; i++)
        for (j = 0; j < b->len; j++)
            t.c[i + j] += a->c[i] * b->c[j];
    p_trim(&t); *r = t;
}

static int p_eq(const Poly *a, const Poly *b) {
    if (a->len != b->len) return 0;
    if (!a->len) return 1;
    if (a->lo != b->lo) return 0;
    return memcmp(a->c, b->c, (size_t)a->len * sizeof(int)) == 0;
}

static void p_print(const Poly *p, const char *name) {
    int i, e, first;
    printf("%s = ", name);
    if (!p->len) { printf("0\n"); return; }
    first = 1;
    for (i = 0; i < p->len; i++) {
        if (!p->c[i]) continue;
        e = p->lo + i;
        if (!first && p->c[i] > 0) printf(" + ");
        if (!first && p->c[i] < 0) printf(" - ");
        if (first && p->c[i] < 0) printf("-");
        first = 0;
        if (abs(p->c[i]) != 1 || e == 0) printf("%d", abs(p->c[i]));
        if (e == 1) printf("A");
        else if (e == -1) printf("A^-1");
        else if (e) printf("A^%d", e);
    }
    printf("\n");
}

/* ================================================================
 * Heap-allocated NxN polynomial matrix
 *
 * For n=5 strands, matrices are 32x32 = 1024 Poly entries.
 * Each Poly is ~808 bytes, so one matrix is ~800KB.
 * Stack allocation would overflow, so we use malloc.
 * ================================================================ */

typedef struct {
    Poly *data;  /* n*n Poly entries, row-major */
    int n;
} Mat;

static Mat mat_alloc(int n) {
    Mat m;
    m.n = n;
    m.data = (Poly *)calloc((size_t)(n * n), sizeof(Poly));
    if (!m.data) { fprintf(stderr, "mat_alloc: out of memory (n=%d)\n", n); exit(1); }
    return m;
}

static void mat_free(Mat *m) {
    if (m->data) { free(m->data); m->data = NULL; }
    m->n = 0;
}

#define MAT(m, i, j) ((m)->data[(i) * (m)->n + (j)])

static void mat_zero(Mat *m) {
    int i, n2;
    n2 = m->n * m->n;
    for (i = 0; i < n2; i++) p_zero(&m->data[i]);
}

static void mat_id(Mat *m) {
    int i, j;
    for (i = 0; i < m->n; i++)
        for (j = 0; j < m->n; j++) {
            if (i == j) p_mono(&MAT(m, i, j), 1, 0);
            else p_zero(&MAT(m, i, j));
        }
}

static void mat_mul(Mat *R, const Mat *A, const Mat *B) {
    Mat T; int i, j, k, n; Poly t;
    n = A->n;
    T = mat_alloc(n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            for (k = 0; k < n; k++) {
                p_mul(&t, &MAT(A, i, k), &MAT(B, k, j));
                p_add(&MAT(&T, i, j), &MAT(&T, i, j), &t);
            }
    /* Copy result into R (which may alias A or B) */
    memcpy(R->data, T.data, (size_t)(n * n) * sizeof(Poly));
    mat_free(&T);
}

static void mat_scale(Mat *R, const Mat *M, const Poly *s) {
    int i, j, n;
    n = M->n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            p_mul(&MAT(R, i, j), &MAT(M, i, j), s);
}

static void mat_add(Mat *R, const Mat *A, const Mat *B) {
    int i, j, n;
    n = A->n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            p_add(&MAT(R, i, j), &MAT(A, i, j), &MAT(B, i, j));
}

static int mat_eq(const Mat *A, const Mat *B) {
    int i, j;
    if (A->n != B->n) return 0;
    for (i = 0; i < A->n; i++)
        for (j = 0; j < A->n; j++)
            if (!p_eq(&MAT(A, i, j), &MAT(B, i, j))) return 0;
    return 1;
}

/* Tensor product: C = A x B
 * C[i1*n2+i2][j1*n2+j2] = A[i1][j1] * B[i2][j2] */
static void mat_tensor(Mat *C, const Mat *A, const Mat *B) {
    int i1, i2, j1, j2, n1, n2;
    n1 = A->n; n2 = B->n;
    /* C must already be allocated with size n1*n2 */
    for (i1 = 0; i1 < n1; i1++)
        for (i2 = 0; i2 < n2; i2++)
            for (j1 = 0; j1 < n1; j1++)
                for (j2 = 0; j2 < n2; j2++)
                    p_mul(&MAT(C, i1*n2+i2, j1*n2+j2),
                          &MAT(A, i1, j1), &MAT(B, i2, j2));
}

/* Quantum trace: tr(diag_weights * M) = sum_i w[i] * M[i][i] */
static void mat_qtrace(Poly *tr, const Mat *M, const Poly *w) {
    int i; Poly t;
    p_zero(tr);
    for (i = 0; i < M->n; i++) {
        p_mul(&t, &w[i], &MAT(M, i, i));
        p_add(tr, tr, &t);
    }
}

/* ================================================================
 * State-sum oracle (from Demo 03/04)
 * ================================================================ */
#define MAX_UF 512
static int uf_p[MAX_UF];
static void uf_init(int n) { int i; for (i = 0; i < n; i++) uf_p[i] = i; }
static int uf_find(int x) {
    while (uf_p[x] != x) { uf_p[x] = uf_p[uf_p[x]]; x = uf_p[x]; }
    return x;
}
static void uf_union(int x, int y) {
    x = uf_find(x); y = uf_find(y); if (x != y) uf_p[x] = y;
}

typedef struct { int word[20]; int len, n; } Braid;

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

static void braid_bracket(Poly *r, const Braid *b) {
    unsigned s, ns; int i, a, bc, lp, j;
    Poly d, dp, t, c, t1, t2;
    p_zero(r);
    p_mono(&t1, -1, 2); p_mono(&t2, -1, -2); p_add(&d, &t1, &t2);
    if (!b->len) {
        p_mono(r, 1, 0);
        for (i = 0; i < b->n - 1; i++) p_mul(r, r, &d);
        return;
    }
    ns = 1u << b->len;
    for (s = 0; s < ns; s++) {
        a = 0; bc = 0;
        for (i = 0; i < b->len; i++) {
            if ((s >> i) & 1u) bc++; else a++;
        }
        lp = braid_loops(b, s);
        p_mono(&t, 1, a - bc);
        p_mono(&dp, 1, 0);
        for (j = 0; j < lp - 1; j++) p_mul(&dp, &dp, &d);
        p_mul(&c, &t, &dp);
        p_add(r, r, &c);
    }
    p_trim(r);
}

/* ================================================================
 * Core construction: R-matrix and generators for n strands
 * ================================================================ */

static Poly DELTA, PA, PAinv;

static void init_globals(void) {
    Poly t1, t2;
    p_mono(&t1, -1, 2); p_mono(&t2, -1, -2); p_add(&DELTA, &t1, &t2);
    p_mono(&PA, 1, 1);
    p_mono(&PAinv, 1, -1);
}

static int n_pass = 0, n_fail = 0;
static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* Build the 4x4 R-matrix on V x V (same as Demo 04) */
static void build_R4(Mat *R, Mat *Rinv) {
    Mat U, I4, AU, AinvI, AinvU, AI;
    Poly neg_Ainv2, neg_A2;

    U = mat_alloc(4); I4 = mat_alloc(4);
    AU = mat_alloc(4); AinvI = mat_alloc(4);
    AinvU = mat_alloc(4); AI = mat_alloc(4);

    p_mono(&neg_Ainv2, -1, -2);
    p_mono(&neg_A2, -1, 2);
    mat_zero(&U);
    MAT(&U, 1, 1) = neg_Ainv2;
    p_mono(&MAT(&U, 1, 2), 1, 0);
    p_mono(&MAT(&U, 2, 1), 1, 0);
    MAT(&U, 2, 2) = neg_A2;

    mat_id(&I4);
    mat_scale(&AU, &U, &PA);
    mat_scale(&AinvI, &I4, &PAinv);
    mat_add(R, &AU, &AinvI);

    mat_scale(&AinvU, &U, &PAinv);
    mat_scale(&AI, &I4, &PA);
    mat_add(Rinv, &AinvU, &AI);

    mat_free(&U); mat_free(&I4);
    mat_free(&AU); mat_free(&AinvI);
    mat_free(&AinvU); mat_free(&AI);
}

/* Build R_i for n strands: R_i = I^{x(i-1)} x R x I^{x(n-i-1)}
 * where i is 1-indexed (generator sigma_i acts on strands i, i+1)
 * Result is 2^n x 2^n */
static void build_Ri(Mat *result, const Mat *R4, int i, int n) {
    int dim = 1 << n; /* 2^n */
    int left_dim, right_dim;
    Mat Ileft, Iright, temp;

    /* i is 1-indexed: left has i-1 identity factors, right has n-i-1 */
    left_dim = 1 << (i - 1);   /* 2^(i-1) */
    right_dim = 1 << (n - i - 1); /* 2^(n-i-1) */

    /* Build I_left x R4 first */
    if (left_dim == 1) {
        /* No left identity needed */
        if (right_dim == 1) {
            /* R_i = R4 directly (n=2 case) */
            memcpy(result->data, R4->data, (size_t)(4 * 4) * sizeof(Poly));
            return;
        }
        /* R4 x I_right */
        Iright = mat_alloc(right_dim);
        mat_id(&Iright);
        mat_tensor(result, R4, &Iright);
        mat_free(&Iright);
    } else if (right_dim == 1) {
        /* I_left x R4 */
        Ileft = mat_alloc(left_dim);
        mat_id(&Ileft);
        mat_tensor(result, &Ileft, R4);
        mat_free(&Ileft);
    } else {
        /* I_left x R4 first, then tensor with I_right */
        Ileft = mat_alloc(left_dim);
        mat_id(&Ileft);
        temp = mat_alloc(left_dim * 4);
        mat_tensor(&temp, &Ileft, R4);

        Iright = mat_alloc(right_dim);
        mat_id(&Iright);
        mat_tensor(result, &temp, &Iright);

        mat_free(&Ileft); mat_free(&temp); mat_free(&Iright);
    }
    (void)dim;
}

/* Build K^{xn} diagonal weights for quantum trace */
static void build_Kn(Poly *w, int n) {
    int dim = 1 << n;
    int idx, bit, s;
    Poly k[2], t;
    p_mono(&k[0], 1, 2);   /* A^2 */
    p_mono(&k[1], 1, -2);  /* A^{-2} */

    for (idx = 0; idx < dim; idx++) {
        p_mono(&w[idx], 1, 0); /* start with 1 */
        for (bit = n - 1; bit >= 0; bit--) {
            s = (idx >> bit) & 1;
            p_mul(&t, &w[idx], &k[s]);
            w[idx] = t;
        }
    }
}

/* Compute braid product on 2^n x 2^n space */
static void braid_product_n(Mat *result, const Braid *b,
                            Mat *Ri, Mat *Ri_inv) {
    int i, j, gen;
    mat_id(result);
    for (i = 0; i < b->len; i++) {
        j = b->word[i];
        gen = j > 0 ? j : -j; /* 1-indexed generator */
        if (j > 0) {
            mat_mul(result, result, &Ri[gen - 1]);
        } else {
            mat_mul(result, result, &Ri_inv[gen - 1]);
        }
    }
}

/* ================================================================
 * TESTS
 * ================================================================ */

static void test_part_a(void) {
    /* Quantum dimension recurrence:
     * [1]_q = 1
     * [2]_q = A^2 + A^{-2}
     * [n+1]_q = (A^2 + A^{-2}) * [n]_q - [n-1]_q
     *
     * Also verify explicit formula:
     * [n]_q = (A^{2n} - A^{-2n}) / (A^2 - A^{-2})
     * which means [n]_q * (A^2 - A^{-2}) = A^{2n} - A^{-2n}
     */
    Poly qd[7]; /* [0]_q through [6]_q */
    Poly t1, t2, denom, lhs, rhs;
    int n;
    char msg[80];

    printf("\n=== PART A: Quantum Dimension Recurrence ===\n");

    /* [0]_q = 0 */
    p_zero(&qd[0]);
    /* [1]_q = 1 */
    p_mono(&qd[1], 1, 0);
    /* [2]_q = A^2 + A^{-2} = -delta */
    p_mono(&t1, 1, 2); p_mono(&t2, 1, -2); p_add(&qd[2], &t1, &t2);

    /* Recurrence: [n+1]_q = [2]_q * [n]_q - [n-1]_q */
    for (n = 2; n <= 5; n++) {
        p_mul(&t1, &qd[2], &qd[n]);
        p_neg(&t2, &qd[n - 1]);
        p_add(&qd[n + 1], &t1, &t2);
    }

    /* Print them */
    for (n = 1; n <= 6; n++) {
        sprintf(msg, "[%d]_q", n);
        p_print(&qd[n], msg);
    }

    /* Verify: [n]_q * (A^2 - A^{-2}) = A^{2n} - A^{-2n} */
    p_mono(&t1, 1, 2); p_mono(&t2, -1, -2); p_add(&denom, &t1, &t2);

    for (n = 1; n <= 6; n++) {
        p_mul(&lhs, &qd[n], &denom);
        p_mono(&t1, 1, 2 * n); p_mono(&t2, -1, -2 * n);
        p_add(&rhs, &t1, &t2);
        sprintf(msg, "[%d]_q * (A^2-A^{-2}) = A^%d - A^{-%d}", n, 2*n, 2*n);
        check(msg, p_eq(&lhs, &rhs));
    }
}

static void test_n_strands(int n) {
    int dim = 1 << n;
    int ngen = n - 1;
    int i;
    Mat R4, R4inv;
    Mat *Ri, *Ri_inv;
    Poly *w;
    Poly neg_delta, oracle, qtrace, expected;
    Poly t1, t2;
    char msg[120];
    Braid braids[4];
    const char *bnames[4];
    int nbr, bi;
    Mat prod;

    printf("\n=== n=%d strands (%dx%d, %d generators) ===\n", n, dim, dim, ngen);

    /* Build base R4 */
    R4 = mat_alloc(4);
    R4inv = mat_alloc(4);
    build_R4(&R4, &R4inv);

    /* Build all generators */
    Ri = (Mat *)malloc((size_t)ngen * sizeof(Mat));
    Ri_inv = (Mat *)malloc((size_t)ngen * sizeof(Mat));
    for (i = 0; i < ngen; i++) {
        Ri[i] = mat_alloc(dim);
        Ri_inv[i] = mat_alloc(dim);
        build_Ri(&Ri[i], &R4, i + 1, n);
        build_Ri(&Ri_inv[i], &R4inv, i + 1, n);
    }

    /* === Yang-Baxter: R_i R_{i+1} R_i = R_{i+1} R_i R_{i+1} === */
    {
        Mat t, lhs, rhs;
        lhs = mat_alloc(dim);
        rhs = mat_alloc(dim);
        t = mat_alloc(dim);
        for (i = 0; i < ngen - 1; i++) {
            mat_mul(&t, &Ri[i], &Ri[i + 1]);
            mat_mul(&lhs, &t, &Ri[i]);
            mat_mul(&t, &Ri[i + 1], &Ri[i]);
            mat_mul(&rhs, &t, &Ri[i + 1]);
            sprintf(msg, "Yang-Baxter: R%d*R%d*R%d = R%d*R%d*R%d (%dx%d)",
                    i+1, i+2, i+1, i+2, i+1, i+2, dim, dim);
            check(msg, mat_eq(&lhs, &rhs));
        }
        mat_free(&lhs); mat_free(&rhs); mat_free(&t);
    }

    /* === Far commutativity: R_i R_j = R_j R_i for |i-j| >= 2 === */
    if (ngen >= 3) {
        Mat ab, ba;
        int j;
        ab = mat_alloc(dim);
        ba = mat_alloc(dim);
        for (i = 0; i < ngen; i++)
            for (j = i + 2; j < ngen; j++) {
                mat_mul(&ab, &Ri[i], &Ri[j]);
                mat_mul(&ba, &Ri[j], &Ri[i]);
                sprintf(msg, "Far commutativity: R%d*R%d = R%d*R%d (%dx%d)",
                        i+1, j+1, j+1, i+1, dim, dim);
                check(msg, mat_eq(&ab, &ba));
            }
        mat_free(&ab); mat_free(&ba);
    }

    /* === R_i * R_i^{-1} = I === */
    {
        Mat prod_check, Id;
        Id = mat_alloc(dim);
        prod_check = mat_alloc(dim);
        mat_id(&Id);
        for (i = 0; i < ngen; i++) {
            mat_mul(&prod_check, &Ri[i], &Ri_inv[i]);
            sprintf(msg, "R%d * R%d_inv = I (%dx%d)", i+1, i+1, dim, dim);
            check(msg, mat_eq(&prod_check, &Id));
        }
        mat_free(&prod_check); mat_free(&Id);
    }

    /* === Quantum trace tests === */
    w = (Poly *)calloc((size_t)dim, sizeof(Poly));
    build_Kn(w, n);

    /* -delta = A^2 + A^{-2} */
    p_mono(&t1, 1, 2); p_mono(&t2, 1, -2); p_add(&neg_delta, &t1, &t2);

    prod = mat_alloc(dim);

    /* Set up test braids for this n */
    nbr = 0;

    /* Identity braid (n-unlink) */
    braids[nbr].n = n; braids[nbr].len = 0;
    bnames[nbr] = "identity";
    nbr++;

    /* Single crossing sigma_1 */
    braids[nbr].n = n; braids[nbr].len = 1;
    braids[nbr].word[0] = 1;
    bnames[nbr] = "sigma_1";
    nbr++;

    if (n >= 3) {
        /* sigma_1 sigma_2 (uses two generators) */
        braids[nbr].n = n; braids[nbr].len = 2;
        braids[nbr].word[0] = 1; braids[nbr].word[1] = 2;
        bnames[nbr] = "sigma_1*sigma_2";
        nbr++;
    }

    if (n >= 4) {
        /* sigma_1 sigma_3 (far generators, tests commutativity) */
        braids[nbr].n = n; braids[nbr].len = 2;
        braids[nbr].word[0] = 1; braids[nbr].word[1] = 3;
        bnames[nbr] = "sigma_1*sigma_3";
        nbr++;
    }

    /*
     * DISCOVERED: The sign depends on strand count.
     *   tr_q = (-1)^n * delta * oracle
     *
     * Equivalently: bracket = (-1)^{n+1} * tr_q / [2]_q
     *
     * For odd n (3, 5): tr_q = -delta * oracle  (what Demo 04 found)
     * For even n (2, 4): tr_q = +delta * oracle
     *
     * This comes from tr_q(I) = (-delta)^n while oracle(identity) = delta^{n-1}.
     * The ratio picks up a factor of (-1)^n.
     */
    {
        if (n % 2 == 0) {
            /* even n: tr_q = delta * oracle, so expected = -(-delta) * oracle */
            p_print(&neg_delta, "    [2]_q ");
            printf("  Sign factor: (-1)^%d = +1, so tr_q = delta * oracle\n", n);
        } else {
            printf("  Sign factor: (-1)^%d = -1, so tr_q = -delta * oracle\n", n);
        }
    }
    printf("\n  Quantum trace tests (bracket = (-1)^{n+1} * tr_q / [2]_q):\n");
    for (bi = 0; bi < nbr; bi++) {
        Poly sign_delta;
        braid_bracket(&oracle, &braids[bi]);
        braid_product_n(&prod, &braids[bi], Ri, Ri_inv);
        mat_qtrace(&qtrace, &prod, w);

        /* expected = (-1)^n * delta * oracle */
        if (n % 2 == 0) {
            /* +delta * oracle: delta = -neg_delta */
            p_neg(&sign_delta, &neg_delta);
            p_mul(&expected, &sign_delta, &oracle);
        } else {
            /* -delta * oracle = neg_delta * oracle */
            p_mul(&expected, &neg_delta, &oracle);
        }

        printf("  %s:\n", bnames[bi]);
        p_print(&oracle, "    oracle");
        p_print(&qtrace, "    tr_q  ");

        sprintf(msg, "n=%d %s: tr_q = (-1)^n * delta * oracle", n, bnames[bi]);
        check(msg, p_eq(&qtrace, &expected));
    }

    /* Cleanup */
    mat_free(&prod);
    free(w);
    for (i = 0; i < ngen; i++) {
        mat_free(&Ri[i]);
        mat_free(&Ri_inv[i]);
    }
    free(Ri); free(Ri_inv);
    mat_free(&R4); mat_free(&R4inv);
}

int main(void) {
    printf("KNOTAPEL DEMO 05: Scaling the Spin Chain\n");
    printf("=========================================\n");

    init_globals();
    p_print(&DELTA, "delta");

    test_part_a();

    printf("\n--- Testing n=2 (4x4) ---\n");
    test_n_strands(2);

    printf("\n--- Testing n=3 (8x8, reproducing Demo 04) ---\n");
    test_n_strands(3);

    printf("\n--- Testing n=4 (16x16) ---\n");
    test_n_strands(4);

    printf("\n--- Testing n=5 (32x32) ---\n");
    test_n_strands(5);

    printf("\n=========================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("=========================================\n");
    return n_fail > 0 ? 1 : 0;
}
