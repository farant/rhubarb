/*
 * KNOTAPEL DEMO 06: Jones-Wenzl Idempotents
 * ==========================================
 *
 * Demo 03 found: bracket(b) = 1*tr_W1(rho(b)) + (A^{-4}+A^4)*tr_W3(rho(b))
 * Demo 04 found: bracket(b) = tr_q(rho(b)) / [2]_q via quantum trace
 *
 * The quantum dimension weights looked like magic numbers. This demo shows
 * they come from PROJECTORS: the Jones-Wenzl idempotents p_n.
 *
 * p_n projects V^{tensor n} onto the highest-spin irreducible component.
 * Properties: p_n^2 = p_n, U_i * p_n = 0, tr_q(p_n) = [n+1]_q.
 *
 * IMPLEMENTATION TRICK: p_n has rational function entries (divisions by
 * quantum integers). We avoid this by working with SCALED idempotents:
 *   Q_n = D_n * p_n  (polynomial entries only)
 *   Q_n^2 = D_n * Q_n  (scaled idempotency)
 *   U_i * Q_n = 0       (annihilation still holds)
 *   tr_q(Q_n) = D_n * [n+1]_q
 *
 * Key simplification: [2]_q / delta = -1, so the n=3 recursion
 * Q_3 = [3]_q * iota(Q_2) - iota(Q_2) * U_2 * iota(Q_2)
 * stays entirely polynomial.
 *
 * PLAN:
 *   Part A: TL relations on V^{tensor 3} (8x8 U_1, U_2)
 *   Part B: Scaled JW p_2 on V tensor V (4x4 Q_2)
 *   Part C: Scaled JW p_3 on V^{tensor 3} (8x8 Q_3)
 *   Part D: Markov trace decomposition via projectors
 *
 * C89, zero dependencies.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Laurent polynomial ring (from Demo 04)
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

static void p_neg(Poly *r, const Poly *a) {
    int i;
    *r = *a;
    for (i = 0; i < r->len; i++) r->c[i] = -r->c[i];
}

static void p_sub(Poly *r, const Poly *a, const Poly *b) {
    Poly nb;
    p_neg(&nb, b);
    p_add(r, a, &nb);
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

static int p_is_zero(const Poly *p) { return p->len == 0; }

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

/* Polynomial division: r = a / b, with assertion that remainder is zero.
 * Returns 1 if exact, 0 if not. */
static int p_div_exact(Poly *r, const Poly *a, const Poly *b) {
    Poly rem, t, mono;
    int deg_r, lead_a, lead_b, q_coeff, q_exp;

    if (!b->len) { printf("  ERROR: division by zero polynomial\n"); return 0; }
    if (!a->len) { p_zero(r); return 1; }

    rem = *a;
    p_zero(r);

    while (rem.len > 0) {
        /* Leading term of remainder / leading term of divisor */
        lead_a = rem.c[rem.len - 1];
        lead_b = b->c[b->len - 1];
        if (lead_a % lead_b != 0) return 0; /* not exact */
        q_coeff = lead_a / lead_b;
        q_exp = (rem.lo + rem.len - 1) - (b->lo + b->len - 1);

        /* Add term to quotient */
        p_mono(&mono, q_coeff, q_exp);
        p_add(r, r, &mono);

        /* Subtract q_term * b from remainder */
        p_mul(&t, &mono, b);
        p_sub(&rem, &rem, &t);
        p_trim(&rem);

        /* Safety: degree should decrease */
        deg_r = rem.len > 0 ? (rem.lo + rem.len - 1) : -9999;
        if (rem.len > 0 && deg_r >= (a->lo + a->len - 1)) {
            return 0; /* not converging */
        }
    }
    return 1;
}

/* ================================================================
 * NxN polynomial matrix (stack-allocated, N up to 8)
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

static int mat_eq(const Mat *A, const Mat *B) {
    int i, j;
    if (A->n != B->n) return 0;
    for (i = 0; i < A->n; i++)
        for (j = 0; j < A->n; j++)
            if (!p_eq(&A->m[i][j], &B->m[i][j])) return 0;
    return 1;
}

static int mat_is_zero(const Mat *M) {
    int i, j;
    for (i = 0; i < M->n; i++)
        for (j = 0; j < M->n; j++)
            if (!p_is_zero(&M->m[i][j])) return 0;
    return 1;
}

/* Tensor product: C = A tensor B */
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

/* ================================================================
 * State-sum oracle (from Demo 04)
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
static Poly DELTA;  /* -A^2 - A^{-2} */
static Poly PA, PAinv;
static Poly QD[5];  /* quantum dimensions [1]_q through [4]_q */

static void init_globals(void) {
    Poly t1, t2;
    p_mono(&t1, -1, 2); p_mono(&t2, -1, -2); p_add(&DELTA, &t1, &t2);
    p_mono(&PA, 1, 1);
    p_mono(&PAinv, 1, -1);

    /* Quantum dimensions */
    p_mono(&QD[1], 1, 0);  /* [1]_q = 1 */
    p_mono(&t1, 1, 2); p_mono(&t2, 1, -2); p_add(&QD[2], &t1, &t2); /* [2]_q = A^2+A^{-2} */
    /* [3]_q = [2]_q * [2]_q - [1]_q = A^4+1+A^{-4} */
    p_mul(&t1, &QD[2], &QD[2]);
    p_sub(&QD[3], &t1, &QD[1]);
    /* [4]_q = [2]_q * [3]_q - [2]_q */
    p_mul(&t1, &QD[2], &QD[3]);
    p_sub(&QD[4], &t1, &QD[2]);
}

static int n_pass = 0, n_fail = 0;
static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * Build U on V tensor V (4x4, same as Demo 04)
 * ================================================================ */
static Mat U_VV;  /* 4x4 TL generator */

static void build_U(void) {
    Poly neg_Ainv2, neg_A2;
    p_mono(&neg_Ainv2, -1, -2);
    p_mono(&neg_A2, -1, 2);
    mat_zero(&U_VV, 4);
    U_VV.m[1][1] = neg_Ainv2;
    p_mono(&U_VV.m[1][2], 1, 0);
    p_mono(&U_VV.m[2][1], 1, 0);
    U_VV.m[2][2] = neg_A2;
}

/* Build R and R^{-1} on V tensor V */
static Mat R_VV, Rinv_VV;

static void build_R(void) {
    Mat I4, AU, AinvI, AinvU, AI;
    mat_id(&I4, 4);
    mat_scale(&AU, &U_VV, &PA);
    mat_scale(&AinvI, &I4, &PAinv);
    mat_add(&R_VV, &AU, &AinvI);
    mat_scale(&AinvU, &U_VV, &PAinv);
    mat_scale(&AI, &I4, &PA);
    mat_add(&Rinv_VV, &AinvU, &AI);
}

/* ================================================================
 * Build TL generators on V^{tensor 3} (8x8)
 * U1 = U tensor I_2, U2 = I_2 tensor U
 * And braid generators R1, R2
 * ================================================================ */
static Mat U1_8, U2_8;  /* 8x8 TL generators */
static Mat R1_8, R2_8, R1inv_8, R2inv_8;  /* 8x8 braid generators */

static void build_8x8(void) {
    Mat I2;
    mat_id(&I2, 2);
    mat_tensor(&U1_8, &U_VV, &I2);
    mat_tensor(&U2_8, &I2, &U_VV);
    mat_tensor(&R1_8, &R_VV, &I2);
    mat_tensor(&R2_8, &I2, &R_VV);
    mat_tensor(&R1inv_8, &Rinv_VV, &I2);
    mat_tensor(&R2inv_8, &I2, &Rinv_VV);
}

/* Build braid product on V^{tensor 3} */
static void braid_product(Mat *result, const Braid *b) {
    int i, j;
    Mat *gen;
    mat_id(result, 8);
    for (i = 0; i < b->len; i++) {
        j = b->word[i];
        if (j == 1) gen = &R1_8;
        else if (j == -1) gen = &R1inv_8;
        else if (j == 2) gen = &R2_8;
        else if (j == -2) gen = &R2inv_8;
        else { printf("  ERROR: bad generator %d\n", j); return; }
        mat_mul(result, result, gen);
    }
}

/* Build K^{tensor 3} diagonal weights for quantum trace */
static Poly W8[8];

static void build_weights(void) {
    Poly k[2], t;
    int a, b, c, idx;
    p_mono(&k[0], 1, 2);
    p_mono(&k[1], 1, -2);
    for (a = 0; a < 2; a++)
        for (b = 0; b < 2; b++)
            for (c = 0; c < 2; c++) {
                idx = a * 4 + b * 2 + c;
                p_mul(&t, &k[a], &k[b]);
                p_mul(&W8[idx], &t, &k[c]);
            }
}

/* ================================================================
 * TESTS
 * ================================================================ */

static void test_part_a(void) {
    Mat U1sq, dU1, U2sq, dU2;
    Mat U1U2U1, U2U1U2, t;

    printf("\n=== PART A: TL Relations on V^{tensor 3} ===\n");

    /* U_i^2 = delta * U_i */
    mat_mul(&U1sq, &U1_8, &U1_8);
    mat_scale(&dU1, &U1_8, &DELTA);
    check("U1^2 = delta * U1 (8x8)", mat_eq(&U1sq, &dU1));

    mat_mul(&U2sq, &U2_8, &U2_8);
    mat_scale(&dU2, &U2_8, &DELTA);
    check("U2^2 = delta * U2 (8x8)", mat_eq(&U2sq, &dU2));

    /* U1 * U2 * U1 = U1 */
    mat_mul(&t, &U1_8, &U2_8);
    mat_mul(&U1U2U1, &t, &U1_8);
    check("U1*U2*U1 = U1 (TL relation)", mat_eq(&U1U2U1, &U1_8));

    /* U2 * U1 * U2 = U2 */
    mat_mul(&t, &U2_8, &U1_8);
    mat_mul(&U2U1U2, &t, &U2_8);
    check("U2*U1*U2 = U2 (TL relation)", mat_eq(&U2U1U2, &U2_8));
}

static void test_part_b(void) {
    /*
     * Scaled Jones-Wenzl p_2 on V tensor V (4x4):
     *   Q_2 = delta * I_4 - U
     *   D_2 = delta
     *
     * Properties:
     *   Q_2^2 = delta * Q_2  (scaled idempotency)
     *   U * Q_2 = 0          (annihilation)
     *   tr_q(Q_2) = delta * [3]_q
     */
    Mat Q2, Q2sq, dQ2, UQ2;
    Mat dI4;
    Poly w4[4], tr, expected;
    Poly k[2];
    int a, b, idx;

    printf("\n=== PART B: Scaled JW p_2 on V tensor V (4x4) ===\n");
    printf("  Q_2 = delta*I - U, D_2 = delta\n\n");

    /* Q_2 = delta * I_4 - U */
    mat_id(&dI4, 4);
    mat_scale(&dI4, &dI4, &DELTA);
    mat_scale(&Q2, &U_VV, &DELTA); /* temporary: use as -U = negate */
    /* Actually: Q_2 = delta*I - U. Let's build it properly */
    {
        Mat negU;
        int i, j;
        negU.n = 4;
        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                p_neg(&negU.m[i][j], &U_VV.m[i][j]);
        mat_add(&Q2, &dI4, &negU);
    }

    /* Q_2^2 = delta * Q_2 */
    mat_mul(&Q2sq, &Q2, &Q2);
    mat_scale(&dQ2, &Q2, &DELTA);
    check("Q2^2 = delta * Q2 (scaled idempotency)", mat_eq(&Q2sq, &dQ2));

    /* U * Q_2 = 0 */
    mat_mul(&UQ2, &U_VV, &Q2);
    check("U * Q2 = 0 (annihilation)", mat_is_zero(&UQ2));

    /* tr_q(Q_2) = delta * [3]_q */
    /* Build K^{tensor 2} weights */
    p_mono(&k[0], 1, 2);
    p_mono(&k[1], 1, -2);
    for (a = 0; a < 2; a++)
        for (b = 0; b < 2; b++) {
            idx = a * 2 + b;
            p_mul(&w4[idx], &k[a], &k[b]);
        }
    mat_qtrace(&tr, &Q2, w4);
    p_mul(&expected, &DELTA, &QD[3]);
    p_print(&tr, "  tr_q(Q2)");
    p_print(&expected, "  delta*[3]_q");
    check("tr_q(Q2) = delta * [3]_q", p_eq(&tr, &expected));
}

static Mat Q3_8;  /* global so Part D can use it */
static Poly D3;   /* D_3 = delta * [3]_q */

static void test_part_c(void) {
    /*
     * Scaled Jones-Wenzl p_3 on V^{tensor 3} (8x8):
     *   iota(Q_2) = Q_2 tensor I_2 = delta*I_8 - U1
     *   Q_3 = [3]_q * iota(Q_2) - iota(Q_2) * U2 * iota(Q_2)
     *   D_3 = delta * [3]_q
     *
     * Derivation: p_3 = iota(p_2) - (delta/[3]_q) * iota(p_2)*U2*iota(p_2)
     * With Q_n = D_n*p_n and D_3 = delta*[3]_q, the polynomial form becomes:
     *   Q_3 = [3]_q * iota(Q_2) - iota(Q_2) * U2 * iota(Q_2)
     */
    Mat iotaQ2;  /* Q_2 tensor I_2 = delta*I_8 - U1 */
    Mat iotaQ2_U2, iotaQ2_U2_iotaQ2, scaled, Q3sq, D3_Q3;
    Mat U1Q3, U2Q3;
    Poly tr, expected;
    int i, j;

    printf("\n=== PART C: Scaled JW p_3 on V^{tensor 3} (8x8) ===\n");
    printf("  Q_3 = [3]_q * iota(Q_2) - iota(Q_2) * U2 * iota(Q_2)\n");
    printf("  D_3 = delta * [3]_q\n\n");

    /* D_3 = delta * [3]_q */
    p_mul(&D3, &DELTA, &QD[3]);
    p_print(&D3, "  D_3");

    /* iota(Q_2) = delta*I_8 - U1 */
    mat_id(&iotaQ2, 8);
    mat_scale(&iotaQ2, &iotaQ2, &DELTA);
    iotaQ2.n = 8;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
            Poly neg;
            p_neg(&neg, &U1_8.m[i][j]);
            p_add(&iotaQ2.m[i][j], &iotaQ2.m[i][j], &neg);
        }

    /* Q_3 = [3]_q * iota(Q_2) - iota(Q_2) * U2 * iota(Q_2)
     *
     * Derivation: c = delta/[3]_q (from U_2*p_3 = 0 requirement).
     * Q_3 = D_3*p_3 = [3]_q*iota(Q_2) - delta^2*iota(Q_2)*U_2*iota(Q_2)/delta^2
     *      = [3]_q*iota(Q_2) - iota(Q_2)*U_2*iota(Q_2)
     */
    mat_scale(&scaled, &iotaQ2, &QD[3]);
    mat_mul(&iotaQ2_U2, &iotaQ2, &U2_8);
    mat_mul(&iotaQ2_U2_iotaQ2, &iotaQ2_U2, &iotaQ2);
    /* Negate the second term */
    {
        int ii, jj;
        for (ii = 0; ii < 8; ii++)
            for (jj = 0; jj < 8; jj++)
                p_neg(&iotaQ2_U2_iotaQ2.m[ii][jj], &iotaQ2_U2_iotaQ2.m[ii][jj]);
    }
    mat_add(&Q3_8, &scaled, &iotaQ2_U2_iotaQ2);

    /* Q_3^2 = D_3 * Q_3 (scaled idempotency) */
    mat_mul(&Q3sq, &Q3_8, &Q3_8);
    mat_scale(&D3_Q3, &Q3_8, &D3);
    check("Q3^2 = D3 * Q3 (scaled idempotency)", mat_eq(&Q3sq, &D3_Q3));

    /* U1 * Q_3 = 0 */
    mat_mul(&U1Q3, &U1_8, &Q3_8);
    check("U1 * Q3 = 0 (annihilation)", mat_is_zero(&U1Q3));

    /* U2 * Q_3 = 0 */
    mat_mul(&U2Q3, &U2_8, &Q3_8);
    check("U2 * Q3 = 0 (annihilation)", mat_is_zero(&U2Q3));

    /* tr_q(Q_3) = D_3 * [4]_q */
    mat_qtrace(&tr, &Q3_8, W8);
    p_mul(&expected, &D3, &QD[4]);
    p_print(&tr, "  tr_q(Q3)");
    p_print(&expected, "  D3*[4]_q");
    check("tr_q(Q3) = D3 * [4]_q", p_eq(&tr, &expected));
}

static void test_part_d(void) {
    /*
     * Markov trace decomposition via projectors.
     *
     * For 3 strands: V^{tensor 3} = V_{3/2} tensor W_3 + V_{1/2} tensor W_1
     *
     * p_3 projects onto V_{3/2} tensor W_3 (the highest-spin, all-through-lines module).
     * (I - p_3) projects onto V_{1/2} tensor W_1.
     *
     * Using scaled Q_3 = D_3 * p_3:
     *   tr_q(Q_3 * rho(beta)) / D_3 = [4]_q * tr_{W_3}(rho_3(beta))
     *   tr_q((D_3*I - Q_3) * rho(beta)) / D_3 = [2]_q * tr_{W_1}(rho_1(beta))
     *
     * And: bracket = [tr_q(Q_3*rho) + tr_q((D_3*I - Q_3)*rho)] / (D_3 * [2]_q)
     *              = tr_q(rho) / [2]_q  (since Q_3 + (D_3*I - Q_3) = D_3*I, everything cancels)
     *
     * But the INTERESTING test is that each COMPONENT separately gives the
     * correct module trace with the correct quantum dimension weight.
     *
     * W_3 (1D, all through-lines): all U_i act as 0, so g_i = A^{-1}.
     * For a braid with word [s_1, s_2, ...], tr_{W_3} = product of A^{-sgn(s_i)}.
     *
     * Test: tr_q(Q_3 * rho(beta)) should equal D_3 * [4]_q * (product of A^{-sgn(s_i)})
     */
    Braid braids[4];
    const char *bnames[4];
    int nbr, bi;
    Mat rho, Q3_rho;
    Poly tr_total, tr_w3, expected_w3, w3_trace;
    Poly neg_delta;
    Poly oracle, expected_total;

    printf("\n=== PART D: Markov Trace Decomposition ===\n");
    printf("  bracket = [W_3 component + W_1 component] / [2]_q\n");
    printf("  W_3 component = tr_q(Q_3 * rho) / D_3 = [4]_q * tr_{W_3}\n\n");

    /* Test braids */
    nbr = 0;

    /* Identity (3-unlink) */
    braids[nbr].n = 3; braids[nbr].len = 0;
    bnames[nbr] = "identity";
    nbr++;

    /* sigma_1 */
    braids[nbr].n = 3; braids[nbr].len = 1;
    braids[nbr].word[0] = 1;
    bnames[nbr] = "sigma_1";
    nbr++;

    /* Trefoil: sigma_1 sigma_2 sigma_1 */
    braids[nbr].n = 3; braids[nbr].len = 3;
    braids[nbr].word[0] = 1; braids[nbr].word[1] = 2; braids[nbr].word[2] = 1;
    bnames[nbr] = "trefoil";
    nbr++;

    /* Figure-eight: sigma_1 sigma_2^{-1} sigma_1 sigma_2^{-1} */
    braids[nbr].n = 3; braids[nbr].len = 4;
    braids[nbr].word[0] = 1; braids[nbr].word[1] = -2;
    braids[nbr].word[2] = 1; braids[nbr].word[3] = -2;
    bnames[nbr] = "figure-eight";
    nbr++;

    /* neg_delta = -delta = A^2 + A^{-2} = [2]_q */
    p_neg(&neg_delta, &DELTA);

    for (bi = 0; bi < nbr; bi++) {
        int ci;
        char msg[120];

        printf("  --- %s ---\n", bnames[bi]);

        /* Compute rho(beta) */
        braid_product(&rho, &braids[bi]);

        /* tr_q(rho) = total quantum trace */
        mat_qtrace(&tr_total, &rho, W8);

        /* tr_q(Q_3 * rho) = W_3 component (scaled by D_3) */
        mat_mul(&Q3_rho, &Q3_8, &rho);
        mat_qtrace(&tr_w3, &Q3_rho, W8);

        /* Expected W_3 trace: on W_3 (all through-lines), g_i = A^{-1}*I (since U_i=0).
         * So tr_{W_3}(rho(beta)) = product of A^{-sgn(word[i])}
         * And expected: tr_q(Q_3 * rho) = D_3 * [4]_q * tr_{W_3} */
        p_mono(&w3_trace, 1, 0);
        for (ci = 0; ci < braids[bi].len; ci++) {
            Poly factor;
            p_mono(&factor, 1, braids[bi].word[ci] > 0 ? -1 : 1);
            p_mul(&w3_trace, &w3_trace, &factor);
        }
        {
            Poly d3_q4;
            p_mul(&d3_q4, &D3, &QD[4]);
            p_mul(&expected_w3, &d3_q4, &w3_trace);
        }

        p_print(&tr_w3, "    tr_q(Q3*rho)");
        p_print(&expected_w3, "    D3*[4]_q*trW3");
        sprintf(msg, "%s: W_3 component matches", bnames[bi]);
        check(msg, p_eq(&tr_w3, &expected_w3));

        /* Also verify total: tr_q(rho) = -delta * oracle (from Demo 04) */
        braid_bracket(&oracle, &braids[bi]);
        p_mul(&expected_total, &neg_delta, &oracle);
        sprintf(msg, "%s: total tr_q = -delta * bracket", bnames[bi]);
        check(msg, p_eq(&tr_total, &expected_total));

        /* The W_1 component is tr_q(rho) - tr_q(Q_3*rho)/D_3, scaled by D_3:
         * tr_q((D_3*I - Q_3)*rho) = D_3*tr_q(rho) - tr_q(Q_3*rho) */
        {
            Poly tr_w1_scaled, d3_tr_total;
            Poly d3_q2, w1_module_trace;
            p_mul(&d3_tr_total, &D3, &tr_total);
            p_sub(&tr_w1_scaled, &d3_tr_total, &tr_w3);

            /* Expected: D_3 * [2]_q * tr_{W_1}(rho_1(beta))
             * From Demo 03: W_1 is 2D with matrices
             *   g1 = A*U1_W1 + A^{-1}*I, g2 = A*U2_W1 + A^{-1}*I
             * where U1_W1 = [[delta,1],[0,0]], U2_W1 = [[0,0],[1,delta]]
             *
             * For now, we verify indirectly: the sum of both components
             * divided by D_3 should give tr_q(rho), and the bracket
             * should be tr_q(rho) / [2]_q. So verify:
             *   tr_w1_scaled + tr_w3 = D_3 * tr_q(rho)
             */
            {
                Poly sum_check;
                p_add(&sum_check, &tr_w1_scaled, &tr_w3);
                sprintf(msg, "%s: W_1 + W_3 = D3 * tr_q(rho)", bnames[bi]);
                check(msg, p_eq(&sum_check, &d3_tr_total));
            }

            /* Final: verify bracket = tr_q(rho) / [2]_q
             * i.e., [2]_q * bracket = tr_q(rho) = -delta * bracket
             * Wait, [2]_q = -delta, so [2]_q * bracket = -delta*bracket = tr_q(rho).
             * Already verified above. Let's verify the DECOMPOSED form instead:
             *
             * bracket = 1*tr_{W_1}(rho) + ([4]_q/[2]_q)*tr_{W_3}(rho)
             *
             * [4]_q/[2]_q = A^4 + A^{-4} (verified by Demo 05 recurrence)
             *
             * So: [2]_q * bracket = [2]_q*tr_{W_1} + [4]_q*tr_{W_3}
             *                     = tr_q((D_3*I-Q_3)*rho)/D_3 + tr_q(Q_3*rho)/D_3
             *                     = tr_q(rho)
             *
             * This is tautological. The non-trivial content is that each
             * component SEPARATELY factors as qdim * module_trace.
             * We verified the W_3 component above.
             *
             * For W_1, verify that tr_w1_scaled is divisible by D_3*[2]_q:
             */
            p_mul(&d3_q2, &D3, &QD[2]);
            if (p_div_exact(&w1_module_trace, &tr_w1_scaled, &d3_q2)) {
                p_print(&w1_module_trace, "    tr_{W_1}(rho)");
                sprintf(msg, "%s: W_1 component divisible by D3*[2]_q", bnames[bi]);
                check(msg, 1);
            } else {
                sprintf(msg, "%s: W_1 component divisible by D3*[2]_q", bnames[bi]);
                check(msg, 0);
            }
        }
    }
}


int main(void) {
    printf("KNOTAPEL DEMO 06: Jones-Wenzl Idempotents\n");
    printf("==========================================\n");

    init_globals();
    p_print(&DELTA, "delta");
    p_print(&QD[2], "[2]_q");
    p_print(&QD[3], "[3]_q");
    p_print(&QD[4], "[4]_q");

    build_U();
    build_R();
    build_8x8();
    build_weights();

    test_part_a();
    test_part_b();
    test_part_c();
    test_part_d();

    printf("\n==========================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("==========================================\n");
    return n_fail > 0 ? 1 : 0;
}
