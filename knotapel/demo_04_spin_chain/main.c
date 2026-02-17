/*
 * KNOTAPEL DEMO 04: Spin Chain (Tensor Product) Representation
 * =============================================================
 *
 * Demo 03 computed the Kauffman bracket via TL standard modules:
 *   bracket(b) = 1*tr_W1(rho(b)) + (A^{-4}+A^4)*tr_W3(rho(b))
 * where W_1 (2D) and W_3 (1D) are computed separately.
 *
 * This demo uses the TENSOR PRODUCT representation:
 *   - Each strand gets a 2D vector space V = C^2
 *   - For n=3 strands: state space = V^{tensor 3} = C^8
 *   - The R-matrix (4x4) acts on V tensor V at each crossing
 *   - sigma_1 -> R tensor I (8x8), sigma_2 -> I tensor R (8x8)
 *
 * The tensor product V^{tensor 3} contains BOTH standard modules
 * via Schur-Weyl duality:
 *   V^{tensor 3} = V_{3/2} tensor W_3  +  V_{1/2} tensor W_1
 *                = (dim 4 x dim 1) + (dim 2 x dim 2) = 8
 *
 * So a single trace over V^{tensor 3} should capture EVERYTHING
 * that Demo 03 needed two separate module computations for.
 *
 * The key question: what trace formula? Ordinary trace, or a
 * quantum trace tr(K^{tensor n} * M) for some diagonal K?
 *
 * PLAN:
 *   Part A: Build U on V tensor V (cup-cap, 4x4), verify U^2 = delta*U
 *   Part B: Build R = A*U + A^{-1}*I, verify R*R^{-1} = I
 *   Part C: Tensor to 8x8, verify Yang-Baxter R1*R2*R1 = R2*R1*R2
 *   Part D: Try trace formulas, find which matches state-sum oracle
 *
 * C89, zero dependencies.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Laurent polynomial ring (from Demo 03)
 * ================================================================ */
#define MAX_TERMS 120

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
 * NxN polynomial matrix (generic, N up to 8)
 * ================================================================ */
#define MAXN 8

typedef struct { Poly m[MAXN][MAXN]; int n; } Mat;

static void mat_zero(Mat *M, int n) {
    int i, j;
    M->n = n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            p_zero(&M->m[i][j]);
}

static void mat_id(Mat *M, int n) {
    int i, j;
    M->n = n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            if (i == j) p_mono(&M->m[i][j], 1, 0);
            else p_zero(&M->m[i][j]);
        }
}

static void mat_mul(Mat *R, const Mat *A, const Mat *B) {
    Mat T; int i, j, k, n; Poly t;
    n = A->n;
    mat_zero(&T, n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            for (k = 0; k < n; k++) {
                p_mul(&t, &A->m[i][k], &B->m[k][j]);
                p_add(&T.m[i][j], &T.m[i][j], &t);
            }
    *R = T;
}

static void mat_scale(Mat *R, const Mat *M, const Poly *s) {
    int i, j, n;
    n = M->n; R->n = n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            p_mul(&R->m[i][j], &M->m[i][j], s);
}

static void mat_add(Mat *R, const Mat *A, const Mat *B) {
    int i, j, n;
    n = A->n; R->n = n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            p_add(&R->m[i][j], &A->m[i][j], &B->m[i][j]);
}

static void mat_trace(Poly *tr, const Mat *M) {
    int i;
    p_zero(tr);
    for (i = 0; i < M->n; i++)
        p_add(tr, tr, &M->m[i][i]);
}

static int mat_eq(const Mat *A, const Mat *B) {
    int i, j;
    if (A->n != B->n) return 0;
    for (i = 0; i < A->n; i++)
        for (j = 0; j < A->n; j++)
            if (!p_eq(&A->m[i][j], &B->m[i][j])) return 0;
    return 1;
}

/* Tensor product: C = A tensor B
 * If A is n1 x n1 and B is n2 x n2, C is (n1*n2) x (n1*n2)
 * C[i1*n2+i2][j1*n2+j2] = A[i1][j1] * B[i2][j2] */
static void mat_tensor(Mat *C, const Mat *A, const Mat *B) {
    int i1, i2, j1, j2, n1, n2;
    n1 = A->n; n2 = B->n;
    C->n = n1 * n2;
    for (i1 = 0; i1 < n1; i1++)
        for (i2 = 0; i2 < n2; i2++)
            for (j1 = 0; j1 < n1; j1++)
                for (j2 = 0; j2 < n2; j2++)
                    p_mul(&C->m[i1*n2+i2][j1*n2+j2],
                          &A->m[i1][j1], &B->m[i2][j2]);
}

/* Quantum trace: tr(diag_weights * M) = sum_i w[i] * M[i][i] */
static void mat_qtrace(Poly *tr, const Mat *M, const Poly *w) {
    int i; Poly t;
    p_zero(tr);
    for (i = 0; i < M->n; i++) {
        p_mul(&t, &w[i], &M->m[i][i]);
        p_add(tr, tr, &t);
    }
}

static void mat_print(const Mat *M, const char *name) {
    int i, j; char buf[64];
    printf("%s (%dx%d):\n", name, M->n, M->n);
    for (i = 0; i < M->n; i++)
        for (j = 0; j < M->n; j++) {
            sprintf(buf, "  [%d,%d]", i, j);
            if (M->m[i][j].len > 0)
                p_print(&M->m[i][j], buf);
        }
}

/* ================================================================
 * State-sum oracle (from Demo 03)
 * ================================================================ */
#define MAX_UF 256
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
 * Globals
 * ================================================================ */
static Poly DELTA; /* -A^2 - A^{-2} */
static Poly PA, PAinv; /* A and A^{-1} as polynomials */

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

/* ================================================================
 * Build TL cup-cap U on V tensor V
 *
 * The quantum cup-cap for U_q(sl_2) with q = A^2:
 *   Cup: |Omega> = |01> - q|10> = |01> - A^2|10>
 *   Cap: <Omega| = -q^{-1}<01| + <10| = -A^{-2}<01| + <10|
 *
 *   <Omega|Omega> = -(q+q^{-1}) = -(A^2+A^{-2}) = delta
 *
 *   U = |Omega><Omega| acts as:
 *   U(|00>) = 0
 *   U(|01>) = -A^{-2} * |Omega> = -A^{-2}|01> + A^{-2}*A^2|10> = -A^{-2}|01> + |10>
 *   U(|10>) = 1 * |Omega> = |01> - A^2|10>
 *   U(|11>) = 0
 *
 * Matrix in basis {|00>, |01>, |10>, |11>}:
 *   U = [[0,     0,     0,   0],
 *        [0,  -A^{-2},  1,   0],
 *        [0,     1,   -A^2,  0],
 *        [0,     0,     0,   0]]
 *
 * Verify: tr(U) = -A^{-2} + (-A^2) = delta. CHECK.
 * Verify: U^2 = delta*U. CHECK (derived from <Omega|Omega> = delta).
 * ================================================================ */

static Mat U_VV;   /* 4x4 TL generator on V tensor V */
static Mat R_VV;   /* 4x4 R-matrix = A*U + A^{-1}*I */
static Mat Rinv_VV; /* 4x4 inverse */
static Mat R1, R2; /* 8x8: R tensor I, I tensor R */
static Mat R1inv, R2inv; /* 8x8 inverses */

static void build_all(void) {
    Mat I2, I4, AU, AinvI, AinvU, AI;
    Poly neg_Ainv2, neg_A2;

    /* U on V tensor V */
    p_mono(&neg_Ainv2, -1, -2);
    p_mono(&neg_A2, -1, 2);
    mat_zero(&U_VV, 4);
    U_VV.m[1][1] = neg_Ainv2;  /* -A^{-2} */
    p_mono(&U_VV.m[1][2], 1, 0); /* 1 */
    p_mono(&U_VV.m[2][1], 1, 0); /* 1 */
    U_VV.m[2][2] = neg_A2;     /* -A^2 */

    /* R = A*U + A^{-1}*I_4 */
    mat_id(&I4, 4);
    mat_scale(&AU, &U_VV, &PA);       /* A*U */
    mat_scale(&AinvI, &I4, &PAinv);   /* A^{-1}*I */
    mat_add(&R_VV, &AU, &AinvI);

    /* R^{-1} = A^{-1}*U + A*I_4 */
    mat_scale(&AinvU, &U_VV, &PAinv); /* A^{-1}*U */
    mat_scale(&AI, &I4, &PA);         /* A*I */
    mat_add(&Rinv_VV, &AinvU, &AI);

    /* 8x8: R1 = R tensor I_2, R2 = I_2 tensor R */
    mat_id(&I2, 2);
    mat_tensor(&R1, &R_VV, &I2);
    mat_tensor(&R2, &I2, &R_VV);

    /* 8x8 inverses */
    mat_tensor(&R1inv, &Rinv_VV, &I2);
    mat_tensor(&R2inv, &I2, &Rinv_VV);
}

/* ================================================================
 * Build braid product on V^{tensor 3}
 * word[i] = +j means sigma_j (1-indexed)
 * word[i] = -j means sigma_j^{-1}
 * For 3-strand: j=1 -> R1, j=2 -> R2
 * ================================================================ */
static void braid_product(Mat *result, const Braid *b) {
    int i, j;
    Mat *gen;
    mat_id(result, 8);
    for (i = 0; i < b->len; i++) {
        j = b->word[i];
        if (j == 1) gen = &R1;
        else if (j == -1) gen = &R1inv;
        else if (j == 2) gen = &R2;
        else if (j == -2) gen = &R2inv;
        else { printf("  ERROR: bad generator %d\n", j); return; }
        mat_mul(result, result, gen);
    }
}

/* ================================================================
 * TESTS
 * ================================================================ */

static void test_part_a(void) {
    Mat U2, dU;

    printf("\n=== PART A: TL Generator on V tensor V ===\n");
    mat_print(&U_VV, "U");

    /* U^2 = delta * U */
    mat_mul(&U2, &U_VV, &U_VV);
    mat_scale(&dU, &U_VV, &DELTA);
    check("U^2 = delta * U", mat_eq(&U2, &dU));

    /* tr(U) = delta */
    {
        Poly tr;
        mat_trace(&tr, &U_VV);
        check("tr(U) = delta", p_eq(&tr, &DELTA));
    }
}

static void test_part_b(void) {
    Mat prod, I4;

    printf("\n=== PART B: R-matrix on V tensor V ===\n");
    mat_print(&R_VV, "R = A*U + A^{-1}*I");

    mat_id(&I4, 4);
    mat_mul(&prod, &R_VV, &Rinv_VV);
    check("R * R^{-1} = I (4x4)", mat_eq(&prod, &I4));
}

static void test_part_c(void) {
    Mat t, lhs, rhs, prod, I8;

    printf("\n=== PART C: Yang-Baxter in 8D ===\n");
    printf("  R1 = R tensor I, R2 = I tensor R\n");
    printf("  Checking R1*R2*R1 = R2*R1*R2...\n");

    /* R1 * R2 * R1 */
    mat_mul(&t, &R1, &R2);
    mat_mul(&lhs, &t, &R1);

    /* R2 * R1 * R2 */
    mat_mul(&t, &R2, &R1);
    mat_mul(&rhs, &t, &R2);

    check("R1*R2*R1 = R2*R1*R2 (Yang-Baxter, 8x8)", mat_eq(&lhs, &rhs));

    /* Also check inverses */
    mat_mul(&t, &R1inv, &R2inv);
    mat_mul(&lhs, &t, &R1inv);
    mat_mul(&t, &R2inv, &R1inv);
    mat_mul(&rhs, &t, &R2inv);
    check("R1inv*R2inv*R1inv = R2inv*R1inv*R2inv", mat_eq(&lhs, &rhs));

    /* And R1*R1inv = I */
    mat_id(&I8, 8);
    mat_mul(&prod, &R1, &R1inv);
    check("R1 * R1inv = I (8x8)", mat_eq(&prod, &I8));
    mat_mul(&prod, &R2, &R2inv);
    check("R2 * R2inv = I (8x8)", mat_eq(&prod, &I8));
}

static void test_part_d(void) {
    /*
     * DISCOVERED FORMULA:
     *   bracket(beta) = tr_q(rho(beta)) / (A^2 + A^{-2})
     *
     * where tr_q = tr(K^{tensor 3} * M) with K = diag(A^2, A^{-2}).
     *
     * Equivalently: tr_q(rho(beta)) = (A^2+A^{-2}) * bracket(beta) = -delta * bracket
     *
     * This works because V^{tensor 3} decomposes via Schur-Weyl duality:
     *   V^{tensor 3} = V_{3/2} tensor W_3 + V_{1/2} tensor W_1
     * and the quantum trace with K = diag(q, q^{-1}), q = A^2, weights
     * each component by its quantum dimension:
     *   qdim(V_{1/2}) = [2]_q = A^2+A^{-2} = -delta
     *   qdim(V_{3/2}) = [4]_q = A^6+A^2+A^{-2}+A^{-6}
     *
     * Dividing by -delta gives:
     *   bracket = 1*tr_{W_1} + (A^{-4}+A^4)*tr_{W_3}
     * which is exactly the formula from Demo 03!
     *
     * We test by checking: tr_q(rho(beta)) = -delta * oracle(beta)
     * This avoids polynomial division.
     */

    Braid braids[5];
    const char *bnames[5];
    Poly oracles[5];
    Mat products[5];
    Poly w[8]; /* K^{tensor 3} diagonal weights */
    Poly neg_delta;
    int bi, a, b, c, idx;

    printf("\n=== PART D: Bracket via Quantum Trace ===\n");
    printf("  K = diag(A^2, A^{-2})  (quantum group element q^H)\n");
    printf("  bracket = tr(K^{x3} * rho) / (A^2 + A^{-2})\n");
    printf("  Testing: tr_q(rho) = -delta * oracle\n\n");

    /* Build K^{tensor 3} diagonal: K = diag(A^2, A^{-2}) */
    {
        Poly k[2];
        p_mono(&k[0], 1, 2);   /* A^2 */
        p_mono(&k[1], 1, -2);  /* A^{-2} */
        for (a = 0; a < 2; a++)
            for (b = 0; b < 2; b++)
                for (c = 0; c < 2; c++) {
                    Poly t;
                    idx = a * 4 + b * 2 + c;
                    p_mul(&t, &k[a], &k[b]);
                    p_mul(&w[idx], &t, &k[c]);
                }
    }

    /* -delta = A^2 + A^{-2} */
    { Poly t1, t2; p_mono(&t1,1,2); p_mono(&t2,1,-2); p_add(&neg_delta,&t1,&t2); }

    /* Set up test braids */
    braids[0].n = 3; braids[0].len = 0;
    bnames[0] = "identity (3-unlink)";

    braids[1].n = 3; braids[1].len = 3;
    braids[1].word[0] = 1; braids[1].word[1] = 2; braids[1].word[2] = 1;
    bnames[1] = "trefoil";

    braids[2].n = 3; braids[2].len = 4;
    braids[2].word[0] = 1; braids[2].word[1] = -2;
    braids[2].word[2] = 1; braids[2].word[3] = -2;
    bnames[2] = "figure-eight";

    braids[3].n = 3; braids[3].len = 3;
    braids[3].word[0] = -1; braids[3].word[1] = -2; braids[3].word[2] = -1;
    bnames[3] = "mirror trefoil";

    braids[4].n = 3; braids[4].len = 1;
    braids[4].word[0] = 1;
    bnames[4] = "single crossing";

    /* Compute oracle values and 8x8 products */
    for (bi = 0; bi < 5; bi++) {
        braid_bracket(&oracles[bi], &braids[bi]);
        braid_product(&products[bi], &braids[bi]);
    }

    /* Test each braid: tr_q(rho(beta)) should equal -delta * oracle */
    for (bi = 0; bi < 5; bi++) {
        Poly qtrace, expected;
        char msg[80];
        mat_qtrace(&qtrace, &products[bi], w);
        p_mul(&expected, &neg_delta, &oracles[bi]);

        printf("  %s:\n", bnames[bi]);
        p_print(&oracles[bi], "    oracle");
        p_print(&qtrace, "    tr_q  ");
        p_print(&expected, "    -d*orc");

        sprintf(msg, "%s: tr_q = -delta * oracle", bnames[bi]);
        check(msg, p_eq(&qtrace, &expected));
    }

    /* Print the quantum dimensions for documentation */
    printf("\n  Schur-Weyl decomposition V^{x3} = V_{3/2} x W_3 + V_{1/2} x W_1:\n");
    printf("    qdim(V_{1/2}) = [2]_q = A^2+A^{-2} = -delta\n");
    printf("    qdim(V_{3/2}) = [4]_q = A^6+A^2+A^{-2}+A^{-6}\n");
    printf("    bracket = tr_q / (-delta)\n");
    printf("            = 1*tr_{W_1} + (A^{-4}+A^4)*tr_{W_3}  [matches Demo 03]\n");
}


int main(void) {
    printf("KNOTAPEL DEMO 04: Spin Chain (Tensor Product)\n");
    printf("==============================================\n");

    init_globals();
    p_print(&DELTA, "delta");

    build_all();

    test_part_a();
    test_part_b();
    test_part_c();
    test_part_d();

    printf("\n==============================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("==============================================\n");
    return n_fail > 0 ? 1 : 0;
}
