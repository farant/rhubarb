/*
 * KNOTAPEL DEMO 106: Topological Entropy of XOR-Computing Braids — Phase A
 * =========================================================================
 *
 * PREDICTION (registered BEFORE computation):
 *   XOR-computing braids should be pseudo-Anosov (spectral radius > 1).
 *   Non-computing braids should be periodic/reducible (spectral radius = 1).
 *
 * Theoretical basis: Fadell-Neuwirth 1962 (braids = trajectories of
 * non-colliding particles). Pseudo-Anosov braids force chaotic mixing
 * (Boyland-Aref-Stremler 2000). Hypothesis: chaotic mixing in configuration
 * space corresponds to computational capacity in DKC.
 *
 * Approach: Parallel BFS with dual representation.
 *   - TL representation (3x3 over Cyc8) for bracket/XOR evaluation
 *   - Reduced Burau representation at t=-1 (3x3 integer) for entropy
 *   - Braid word storage (parent+generator backtracking)
 *
 * Module: W_{4,2} (4 strands, dim 3, non-semisimple, XOR14=70 from D100).
 *
 * Entropy classification:
 *   For a 3x3 integer matrix M with char poly p(x) = x^3 + ax^2 + bx + c:
 *   - |c| = |product of eigenvalues|. If |c| > 1, at least one |lambda| > 1.
 *   - More refined: evaluate p at unit circle boundary points.
 *
 * C89, zero dependencies beyond stdio/stdlib/string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * Exact Cyclotomic Arithmetic -- Z[zeta_8]
 * Basis: {1, zeta_8, zeta_8^2, zeta_8^3} with zeta_8^4 = -1
 * ================================================================ */

typedef struct {
    long a, b, c, d;
} Cyc8;

static Cyc8 cyc8_make(long a, long b, long c, long d) {
    Cyc8 z; z.a = a; z.b = b; z.c = c; z.d = d; return z;
}

static Cyc8 cyc8_zero(void) { return cyc8_make(0, 0, 0, 0); }
static Cyc8 cyc8_one(void)  { return cyc8_make(1, 0, 0, 0); }

static Cyc8 cyc8_add(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a + y.a, x.b + y.b, x.c + y.c, x.d + y.d);
}

static Cyc8 cyc8_neg(Cyc8 x) {
    return cyc8_make(-x.a, -x.b, -x.c, -x.d);
}

static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y) {
    return cyc8_make(
        x.a * y.a - x.b * y.d - x.c * y.c - x.d * y.b,
        x.a * y.b + x.b * y.a - x.c * y.d - x.d * y.c,
        x.a * y.c + x.b * y.b + x.c * y.a - x.d * y.d,
        x.a * y.d + x.b * y.c + x.c * y.b + x.d * y.a
    );
}

static int cyc8_eq(Cyc8 x, Cyc8 y) {
    return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}

static int cyc8_is_zero(Cyc8 x) {
    return x.a == 0 && x.b == 0 && x.c == 0 && x.d == 0;
}

static Cyc8 cyc8_conj(Cyc8 z) {
    return cyc8_make(z.a, -z.d, -z.c, -z.b);
}

static long cyc8_max_abs(Cyc8 z) {
    long m = 0, v;
    v = z.a < 0 ? -z.a : z.a; if (v > m) m = v;
    v = z.b < 0 ? -z.b : z.b; if (v > m) m = v;
    v = z.c < 0 ? -z.c : z.c; if (v > m) m = v;
    v = z.d < 0 ? -z.d : z.d; if (v > m) m = v;
    return m;
}

/* ================================================================
 * 3x3 Matrix over Cyc8 (TL representation for W_{4,2})
 * ================================================================ */

typedef struct {
    Cyc8 m[3][3];
} Mat3;

static Mat3 mat3_zero(void) {
    Mat3 r;
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            r.m[i][j] = cyc8_zero();
    return r;
}

static Mat3 mat3_identity(void) {
    Mat3 r = mat3_zero();
    r.m[0][0] = r.m[1][1] = r.m[2][2] = cyc8_one();
    return r;
}

static Mat3 mat3_mul(const Mat3 *p, const Mat3 *q) {
    Mat3 r;
    int i, j, k;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++) {
            r.m[i][j] = cyc8_zero();
            for (k = 0; k < 3; k++)
                r.m[i][j] = cyc8_add(r.m[i][j],
                                      cyc8_mul(p->m[i][k], q->m[k][j]));
        }
    return r;
}

static Mat3 mat3_add(const Mat3 *p, const Mat3 *q) {
    Mat3 r;
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            r.m[i][j] = cyc8_add(p->m[i][j], q->m[i][j]);
    return r;
}

static Mat3 mat3_neg(const Mat3 *p) {
    Mat3 r;
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            r.m[i][j] = cyc8_neg(p->m[i][j]);
    return r;
}

static Mat3 mat3_scale(Cyc8 s, const Mat3 *p) {
    Mat3 r;
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            r.m[i][j] = cyc8_mul(s, p->m[i][j]);
    return r;
}

static int mat3_eq(const Mat3 *p, const Mat3 *q) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (!cyc8_eq(p->m[i][j], q->m[i][j])) return 0;
    return 1;
}

static long mat3_max_abs(const Mat3 *m) {
    long mx = 0, v;
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++) {
            v = cyc8_max_abs(m->m[i][j]);
            if (v > mx) mx = v;
        }
    return mx;
}

/* ================================================================
 * 3x3 Integer Matrix (Burau representation at t=-1)
 * ================================================================ */

typedef struct {
    long m[3][3];
} IntMat3;

static IntMat3 imat3_identity(void) {
    IntMat3 r;
    memset(&r, 0, sizeof(IntMat3));
    r.m[0][0] = r.m[1][1] = r.m[2][2] = 1;
    return r;
}

static IntMat3 imat3_mul(const IntMat3 *p, const IntMat3 *q) {
    IntMat3 r;
    int i, j, k;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++) {
            r.m[i][j] = 0;
            for (k = 0; k < 3; k++)
                r.m[i][j] += p->m[i][k] * q->m[k][j];
        }
    return r;
}

static int imat3_eq(const IntMat3 *p, const IntMat3 *q) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (p->m[i][j] != q->m[i][j]) return 0;
    return 1;
}

static void imat3_print(const char *label, const IntMat3 *m) {
    int r;
    printf("  %s:\n", label);
    for (r = 0; r < 3; r++)
        printf("    [%3ld %3ld %3ld]\n", m->m[r][0], m->m[r][1], m->m[r][2]);
}

/* ================================================================
 * TL generators for W_{4,2}
 * Copied from D100.
 * e_1 = [[0,1,0],[0,0,0],[0,0,0]]
 * e_2 = [[0,0,0],[1,0,1],[0,0,0]]
 * e_3 = [[0,0,0],[0,0,0],[0,1,0]]
 * ================================================================ */

static Mat3 g_e[3];
static Mat3 g_gen_tl[6]; /* sigma_i, sigma_i_inv for TL */
static Cyc8 g_A, g_A_inv;

static void build_tl_generators(void) {
    Mat3 z = mat3_zero();
    int i;

    g_e[0] = z; g_e[0].m[0][1] = cyc8_one();
    g_e[1] = z; g_e[1].m[1][0] = cyc8_one(); g_e[1].m[1][2] = cyc8_one();
    g_e[2] = z; g_e[2].m[2][1] = cyc8_one();

    g_A = cyc8_make(0, -1, 0, 0);
    g_A_inv = cyc8_conj(g_A);

    {
        Mat3 id3 = mat3_identity();
        Mat3 a_id = mat3_scale(g_A, &id3);
        Mat3 a_inv_id = mat3_scale(g_A_inv, &id3);
        for (i = 0; i < 3; i++) {
            Mat3 a_inv_e = mat3_scale(g_A_inv, &g_e[i]);
            Mat3 a_e = mat3_scale(g_A, &g_e[i]);
            g_gen_tl[2*i] = mat3_add(&a_id, &a_inv_e);
            g_gen_tl[2*i+1] = mat3_add(&a_inv_id, &a_e);
        }
    }
}

/* ================================================================
 * Reduced Burau representation at t=-1 for B_4
 *
 * The reduced Burau representation of sigma_i is an (n-1)x(n-1)
 * matrix. For B_4: 3x3 integer matrices at t=-1.
 *
 * Standard convention (reduced Burau at parameter t):
 *   sigma_i acts as identity except in rows/cols (i-1, i) where
 *   the 2x2 block is [[-t, 1], [0, 1]] (1-indexed) for sigma_i,
 *   and [[1, 0], [-t^{-1}, -t^{-1}]] for sigma_i_inv.
 *
 *   Wait - there are multiple conventions. Let me use the one where
 *   at t=-1, sigma_i has a 2x2 block [[1, 1], [0, 1]] at position
 *   (i-1,i-1) to (i,i), embedded in identity.
 *
 * The reduced Burau representation acts on basis f_j = e_j - e_{j+1}
 * (1 <= j <= n-1), where n=4 strands gives 3x3 matrices.
 *
 * sigma_i acts as:
 *   f_{i-1} -> f_{i-1} + t*f_i   (if i > 1)
 *   f_i     -> -t*f_i
 *   f_{i+1} -> f_{i+1} + f_i     (if i < n-1)
 *
 * At t = -1, the 3x3 reduced Burau matrices for B_4 are:
 *
 * sigma_1:     [[1, 0, 0],    sigma_1_inv: [[ 1, 0, 0],
 *               [1, 1, 0],                   [-1, 1, 0],
 *               [0, 0, 1]]                   [ 0, 0, 1]]
 *
 * sigma_2:     [[1,-1, 0],    sigma_2_inv: [[ 1, 1, 0],
 *               [0, 1, 0],                   [ 0, 1, 0],
 *               [0, 1, 1]]                   [ 0,-1, 1]]
 *
 * sigma_3:     [[1, 0, 0],    sigma_3_inv: [[1, 0, 0],
 *               [0, 1,-1],                   [0, 1, 1],
 *               [0, 0, 1]]                   [0, 0, 1]]
 *
 * Verified: all braid group relations hold, sigma*sigma_inv=I,
 * far commutativity s1*s3=s3*s1, all det=1 (SL(3,Z)).
 * The representation is faithful (Bigelow 1999 / Krammer 2000).
 * ================================================================ */

static IntMat3 g_gen_burau[6]; /* sigma_i, sigma_i_inv (i=1,2,3) */
static const int g_gen_writhe_val[6] = {1, -1, 1, -1, 1, -1};

static void build_burau_generators(void) {
    IntMat3 id3 = imat3_identity();
    int i;

    for (i = 0; i < 6; i++)
        g_gen_burau[i] = id3;

    /* sigma_1: [[1,0,0],[1,1,0],[0,0,1]] */
    g_gen_burau[0].m[1][0] = 1;
    /* sigma_1_inv: [[1,0,0],[-1,1,0],[0,0,1]] */
    g_gen_burau[1].m[1][0] = -1;

    /* sigma_2: [[1,-1,0],[0,1,0],[0,1,1]] */
    g_gen_burau[2].m[0][1] = -1;
    g_gen_burau[2].m[2][1] = 1;
    /* sigma_2_inv: [[1,1,0],[0,1,0],[0,-1,1]] */
    g_gen_burau[3].m[0][1] = 1;
    g_gen_burau[3].m[2][1] = -1;

    /* sigma_3: [[1,0,0],[0,1,-1],[0,0,1]] */
    g_gen_burau[4].m[1][2] = -1;
    /* sigma_3_inv: [[1,0,0],[0,1,1],[0,0,1]] */
    g_gen_burau[5].m[1][2] = 1;
}

/* ================================================================
 * Hash table for TL Mat3 BFS (dedup by TL matrix)
 * ================================================================ */

#define MAX_CAT 32768
#define HASH_SIZE 65537

static Mat3    *g_cat_tl;     /* TL representation matrices */
static IntMat3 *g_cat_burau;  /* Burau representation matrices */
static int      g_depth[MAX_CAT];
static int      g_writhe[MAX_CAT];
static int      g_parent[MAX_CAT];     /* parent catalog index (-1 for root) */
static int      g_parent_gen[MAX_CAT]; /* generator index that produced this */
static int      g_cat_size = 0;

static int g_hash_head[HASH_SIZE];
static int g_hash_next[MAX_CAT];

static unsigned long hash_mat3(const Mat3 *m) {
    unsigned long h = 5381;
    const long *p = &m->m[0][0].a;
    int i;
    for (i = 0; i < 36; i++)
        h = h * 31 + (unsigned long)(p[i] + 1000000);
    return h;
}

static void hash_init(void) {
    int i;
    for (i = 0; i < HASH_SIZE; i++)
        g_hash_head[i] = -1;
    for (i = 0; i < MAX_CAT; i++)
        g_hash_next[i] = -1;
}

static int hash_find(const Mat3 *m) {
    int bucket = (int)(hash_mat3(m) % (unsigned long)HASH_SIZE);
    int idx = g_hash_head[bucket];
    while (idx >= 0) {
        if (mat3_eq(&g_cat_tl[idx], m)) return idx;
        idx = g_hash_next[idx];
    }
    return -1;
}

static void hash_insert(int cat_idx) {
    int bucket = (int)(hash_mat3(&g_cat_tl[cat_idx]) % (unsigned long)HASH_SIZE);
    g_hash_next[cat_idx] = g_hash_head[bucket];
    g_hash_head[bucket] = cat_idx;
}

/* ================================================================
 * Dual BFS: TL + Burau simultaneously
 * ================================================================ */

static void build_catalog(int max_depth) {
    int rd = 1;

    hash_init();

    g_cat_tl[0] = mat3_identity();
    g_cat_burau[0] = imat3_identity();
    g_depth[0] = 0;
    g_writhe[0] = 0;
    g_parent[0] = -1;
    g_parent_gen[0] = -1;
    hash_insert(0);
    g_cat_size = 1;

    printf("  BFS catalog (dual TL+Burau):\n");
    printf("  Round 0: 1 entry (identity)\n");

    do {
        int i, gi, prev;
        long round_max = 0;
        prev = g_cat_size;
        for (i = 0; i < prev && g_cat_size < MAX_CAT; i++) {
            if (g_depth[i] != rd - 1) continue;
            for (gi = 0; gi < 6 && g_cat_size < MAX_CAT; gi++) {
                Mat3 tl_prod = mat3_mul(&g_cat_tl[i], &g_gen_tl[gi]);
                if (hash_find(&tl_prod) < 0) {
                    long mabs = mat3_max_abs(&tl_prod);
                    if (mabs > round_max) round_max = mabs;

                    g_cat_tl[g_cat_size] = tl_prod;
                    g_cat_burau[g_cat_size] = imat3_mul(&g_cat_burau[i],
                                                         &g_gen_burau[gi]);
                    g_depth[g_cat_size] = rd;
                    g_writhe[g_cat_size] = g_writhe[i] + g_gen_writhe_val[gi];
                    g_parent[g_cat_size] = i;
                    g_parent_gen[g_cat_size] = gi;
                    hash_insert(g_cat_size);
                    g_cat_size++;
                }
            }
        }
        if (g_cat_size > prev) {
            printf("  Round %d: %d entries (+%d), max_abs=%ld\n",
                   rd, g_cat_size, g_cat_size - prev, round_max);
        }
        if (round_max > 100000000000L) {
            printf("  WARNING: entries approaching overflow, stopping BFS\n");
            break;
        }
        rd++;
    } while (g_cat_size > 1 && rd <= max_depth &&
             g_cat_size < MAX_CAT);

    if (g_cat_size >= MAX_CAT) {
        printf("  HIT CAP at %d entries (group is infinite)\n", MAX_CAT);
    } else {
        printf("  Stopped at depth %d with %d entries\n",
               max_depth, g_cat_size);
    }
}

/* ================================================================
 * Characteristic polynomial of 3x3 integer matrix
 * (Faddeev-LeVerrier, exact integer arithmetic)
 *
 * p(x) = x^3 + c2*x^2 + c1*x + c0
 * c2 = -tr(M)
 * c1 = (tr(M)^2 - tr(M^2)) / 2
 * c0 = -det(M)
 *
 * For 3x3 we can use the direct formula:
 *   det = a(ei-fh) - b(di-fg) + c(dh-eg)
 * where M = [[a,b,c],[d,e,f],[g,h,i]]
 * ================================================================ */

static long imat3_trace(const IntMat3 *m) {
    return m->m[0][0] + m->m[1][1] + m->m[2][2];
}

static long imat3_det(const IntMat3 *m) {
    long a = m->m[0][0], b = m->m[0][1], c = m->m[0][2];
    long d = m->m[1][0], e = m->m[1][1], f = m->m[1][2];
    long g = m->m[2][0], h = m->m[2][1], i = m->m[2][2];
    return a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g);
}

static long imat3_trace_of_square(const IntMat3 *m) {
    /* tr(M^2) = sum_i sum_k m[i][k]*m[k][i] */
    long t = 0;
    int i, k;
    for (i = 0; i < 3; i++)
        for (k = 0; k < 3; k++)
            t += m->m[i][k] * m->m[k][i];
    return t;
}

/* Compute char poly coefficients: x^3 + c[2]*x^2 + c[1]*x + c[0] */
static void imat3_char_poly(const IntMat3 *m, long c[3]) {
    long tr = imat3_trace(m);
    long tr2 = imat3_trace_of_square(m);
    c[2] = -tr;
    c[1] = (tr * tr - tr2) / 2;
    c[0] = -imat3_det(m);
}

/* ================================================================
 * Entropy classification
 *
 * For 3x3 integer matrix M with char poly p(x) = x^3 + ax^2 + bx + c:
 *
 * Classification (all integer arithmetic):
 *   1. |det(M)| = |c[0]| = product of |eigenvalues|
 *      If |c[0]| > 1, at least one eigenvalue has |lambda| > 1
 *      => pseudo-Anosov (ENTROPY > 0)
 *
 *   2. If |c[0]| = 1 (unimodular, which it must be for braid reps):
 *      Need more refined test. Check if p has a real root > 1:
 *      Evaluate p(1) and p(-1):
 *        p(1)  = 1 + a + b + c
 *        p(-1) = -1 + a - b + c
 *      If p(1)*p(-1) < 0, there's a real root with |lambda| > 1
 *      (Intermediate Value Theorem).
 *
 *   3. Additional: evaluate at x=2 and x=-2 to bracket roots.
 *      If p changes sign between 1 and 2 (or -1 and -2),
 *      there's a root with |lambda| > 1.
 *
 * For braid group representations:
 *   det(Burau(sigma_i)) = (-t) at parameter t.
 *   At t=-1: det = -(-1) = 1. So det of any braid word = 1^{pos} * 1^{neg} = +1 or -1.
 *   Actually det(sigma_i) at t=-1:
 *     sigma_1 has det=1 (lower triangular, all 1 on diag)
 *     sigma_2 has det=1 (expand: 1*(1*1-0*1) - 1*(0*1-0*0) + 0 = 1)
 *     sigma_3 has det=1
 *   So det of any product = 1. All Burau matrices at t=-1 for B_4 are in SL(3,Z).
 *
 * Therefore |c[0]| = 1 always, and we need the refined IVT test.
 *
 * Actually stronger: since det=1 always, c[0] = -det = -1.
 * The char poly is p(x) = x^3 + ax^2 + bx - 1.
 * Note p(0) = -1 < 0, and p(x) -> +inf as x -> +inf.
 * So there's always a real root > 0.
 * The question is whether it's > 1 or = 1.
 *
 * p(1) = 1 + a + b - 1 = a + b.
 * If a + b > 0: the root is < 1 (p is positive at 1 and negative at 0).
 * If a + b < 0: there's a root > 1 => pseudo-Anosov.
 * If a + b = 0: lambda=1 is a root => periodic/reducible.
 *
 * This is the KEY INTEGER TEST: a + b < 0 => pseudo-Anosov.
 * Where a = c[2] = -tr(M) and b = c[1] = (tr^2 - tr(M^2))/2.
 *
 * Additional: for the other direction, check p(-1):
 * p(-1) = -1 + a - b - 1 = a - b - 2.
 * If a - b - 2 > 0 and the braid is pA, the negative root is > -1.
 * If a - b - 2 < 0: there's a root < -1.
 *
 * Classification:
 *   PERIODIC:       p(1) = 0 (lambda = 1 is a root)
 *   REDUCIBLE:      all |lambda| <= 1, but not periodic
 *   PSEUDO_ANOSOV:  exists |lambda| > 1
 * ================================================================ */

#define CLASS_IDENTITY    0  /* The identity matrix */
#define CLASS_PERIODIC    1  /* All eigenvalues on unit circle, finite order */
#define CLASS_PSEUDO_ANOSOV 2  /* Exists |lambda| > 1 */

/*
 * Classification for SL(3,Z) Burau matrices (det = 1 always).
 *
 * Char poly: p(x) = x^3 + c[2]*x^2 + c[1]*x - 1.
 * Product of eigenvalues = 1.
 *
 * KEY THEOREM (for SL(3,Z)):
 *
 * Case 1: p(1) != 0. Since p(0) = -1 < 0 and p(+inf) = +inf,
 *   there's always a positive real root.
 *   - If p(1) < 0: the positive root is > 1 => spectral radius > 1.
 *   - If p(1) > 0: the positive root is in (0,1). Since the product
 *     of all three eigenvalues = 1 and one root has |r| < 1, the
 *     remaining pair (complex conjugates) must have |lambda| > 1.
 *   Either way: PSEUDO-ANOSOV.
 *
 * Case 2: p(1) = 0. Lambda=1 is a root.
 *   Factor: p(x) = (x-1)(x^2 + e*x + 1) where e = c[2] + 1 = 1 - tr(M).
 *   The quadratic x^2 + e*x + 1 has roots on the unit circle
 *   iff |e| <= 2 (they're exp(+-i*theta) with 2*cos(theta) = -e).
 *   - |e| <= 2 (i.e., -1 <= tr <= 3): all eigenvalues on unit circle.
 *     NOT pseudo-Anosov.
 *   - |e| > 2 (i.e., tr < -1 or tr > 3): quadratic has two real roots
 *     with product 1 (one > 1, one < 1). PSEUDO-ANOSOV.
 */
static int classify_entropy(const IntMat3 *m, long cpoly[3]) {
    long p1, tr, e;
    IntMat3 id3 = imat3_identity();

    imat3_char_poly(m, cpoly);

    /* Identity check */
    if (imat3_eq(m, &id3))
        return CLASS_IDENTITY;

    /* p(1) = 1 + c[2] + c[1] + c[0] = 1 + c[2] + c[1] - 1 = c[2] + c[1] */
    p1 = cpoly[2] + cpoly[1];

    if (p1 != 0)
        return CLASS_PSEUDO_ANOSOV;

    /* p(1) = 0: check if remaining quadratic has roots on unit circle */
    tr = imat3_trace(m);
    e = 1 - tr;  /* coefficient of x in factored quadratic */

    if (e > 2 || e < -2)
        return CLASS_PSEUDO_ANOSOV;

    return CLASS_PERIODIC;
}

/* ================================================================
 * Spectral radius computation (double precision)
 *
 * For a 3x3 integer matrix in SL(3,Z), the char poly is
 * p(x) = x^3 + a*x^2 + b*x - 1 (since det=1, c[0]=-1).
 *
 * Strategy:
 *   1. Find the positive real root r by bisection (always exists).
 *   2. If r > 1: spectral radius = r.
 *   3. If r < 1: the other two eigenvalues are a complex conjugate
 *      pair with modulus rho = 1/sqrt(r). Spectral radius = rho.
 *   4. If r = 1: check quadratic factor.
 * ================================================================ */

static double spectral_radius(const IntMat3 *m) {
    long cp[3];
    double a, b;
    double lo, hi, mid, val;
    double r;
    int iter;

    imat3_char_poly(m, cp);
    a = (double)cp[2];
    b = (double)cp[1];
    /* p(x) = x^3 + a*x^2 + b*x - 1 */

    /* Find the positive real root by bisection.
     * p(0) = -1 < 0. As x -> +inf, p(x) -> +inf.
     * Find upper bound where p > 0. */
    hi = 2.0;
    while (hi * hi * hi + a * hi * hi + b * hi - 1.0 < 0.0) {
        hi *= 2.0;
        if (hi > 1e15) return hi; /* overflow safety */
    }
    lo = 0.0;

    /* Bisection: find root in (lo, hi) */
    for (iter = 0; iter < 100; iter++) {
        mid = (lo + hi) * 0.5;
        val = mid * mid * mid + a * mid * mid + b * mid - 1.0;
        if (val < 0.0) lo = mid;
        else hi = mid;
    }
    r = (lo + hi) * 0.5;

    if (r > 1.0 + 1e-12)
        return r;

    /* r <= 1: check if complex pair has larger modulus */
    if (r < 1.0 - 1e-12)
        return 1.0 / sqrt(r);

    /* r ≈ 1: check the quadratic x^2 + e*x + 1 where e = a+1 */
    {
        double e = a + 1.0;
        double disc = e * e - 4.0;
        if (disc > 1e-12) {
            /* Two real roots with product 1 */
            double root1 = (-e + sqrt(disc)) / 2.0;
            double root2 = (-e - sqrt(disc)) / 2.0;
            double ar1 = fabs(root1);
            double ar2 = fabs(root2);
            return ar1 > ar2 ? ar1 : ar2;
        }
    }

    return 1.0; /* All eigenvalues on unit circle */
}

/* ================================================================
 * Activation: 3x3_hash (from D100)
 * Sign of all 36 integer components (9 entries x 4 Cyc8 components).
 * 3-valued: pos=2, zero=1, neg=0. Hash mod k_param.
 * ================================================================ */

static int mat3_activate(const Mat3 *m, int k_param) {
    int h = 0;
    const long *p = &m->m[0][0].a;
    int i;
    for (i = 0; i < 36; i++) {
        h = h * 3 + (p[i] > 0 ? 2 : (p[i] < 0 ? 0 : 1));
    }
    if (h < 0) h = -h;
    return h % k_param;
}

/* ================================================================
 * XOR test for 3x3 matrices (from D100)
 * 1wpi encoding: N weights -> 2N inputs.
 * ================================================================ */

#define MAX_ACT_CELLS 8192

static int cell_even[MAX_ACT_CELLS], cell_odd[MAX_ACT_CELLS];
static int touched[MAX_ACT_CELLS];

static int test_xor_mat3(const int *indices, int n_weights, int k_param) {
    int n_inputs = 2 * n_weights;
    int n_masks = 1 << n_inputs;
    int n_cells = k_param;
    int n_touched = 0;
    int mask, i, w;
    int result = 1;
    Mat3 pos[8], neg[8];

    if (n_cells > MAX_ACT_CELLS || n_inputs > 16 || n_weights > 8)
        return 0;

    for (w = 0; w < n_weights; w++) {
        pos[w] = g_cat_tl[indices[w]];
        neg[w] = mat3_neg(&g_cat_tl[indices[w]]);
    }

    for (mask = 0; mask < n_masks; mask++) {
        Mat3 sum = mat3_zero();
        int par = 0, cell;

        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                w = i / 2;
                if (i % 2 == 0)
                    sum = mat3_add(&sum, &pos[w]);
                else
                    sum = mat3_add(&sum, &neg[w]);
                par ^= 1;
            }
        }

        cell = mat3_activate(&sum, k_param);

        if (cell_even[cell] == 0 && cell_odd[cell] == 0)
            touched[n_touched++] = cell;

        if (par == 0) {
            cell_even[cell]++;
            if (cell_odd[cell] > 0) { result = 0; goto cleanup; }
        } else {
            cell_odd[cell]++;
            if (cell_even[cell] > 0) { result = 0; goto cleanup; }
        }
    }

cleanup:
    for (i = 0; i < n_touched; i++) {
        cell_even[touched[i]] = 0;
        cell_odd[touched[i]] = 0;
    }
    return result;
}

/* ================================================================
 * Boolean function test with +/- encoding
 *
 * For N weights and a target Boolean function f: {0,1}^N -> {0,1}
 * given as a truth table (bit p = f(b0,b1,...) where p = sum bi*2^i):
 *
 * For each N-bit mask m:
 *   sum = sum_w (bit_w ? +w : -w)
 *   cell = activate(sum, k)
 *   target = (truth_table >> m) & 1
 *
 * Succeeds if f=0 and f=1 inputs always land in different cells.
 * ================================================================ */

static int test_bool_pm_mat3(const int *indices, int n_weights,
                              int k_param, unsigned char truth_table) {
    int n_masks = 1 << n_weights;
    int mask, w;
    int result = 1;
    int n_touched_local = 0;
    Mat3 pos_w[8], neg_w[8];

    if (k_param > MAX_ACT_CELLS || n_weights > 8) return 0;

    for (w = 0; w < n_weights; w++) {
        pos_w[w] = g_cat_tl[indices[w]];
        neg_w[w] = mat3_neg(&g_cat_tl[indices[w]]);
    }

    for (mask = 0; mask < n_masks; mask++) {
        Mat3 sum = mat3_zero();
        int target, cell;

        for (w = 0; w < n_weights; w++) {
            if (mask & (1 << w))
                sum = mat3_add(&sum, &pos_w[w]);
            else
                sum = mat3_add(&sum, &neg_w[w]);
        }

        cell = mat3_activate(&sum, k_param);
        target = (truth_table >> mask) & 1;

        if (cell_even[cell] == 0 && cell_odd[cell] == 0)
            touched[n_touched_local++] = cell;

        if (target == 0) {
            cell_even[cell]++;
            if (cell_odd[cell] > 0) { result = 0; goto cleanup_pm; }
        } else {
            cell_odd[cell]++;
            if (cell_even[cell] > 0) { result = 0; goto cleanup_pm; }
        }
    }

cleanup_pm:
    {
        int ci;
        for (ci = 0; ci < n_touched_local; ci++) {
            cell_even[touched[ci]] = 0;
            cell_odd[touched[ci]] = 0;
        }
    }
    return result;
}

/* ================================================================
 * Per-entry XOR participation score
 *
 * For each catalog entry, count how many XOR-succeeding triples
 * it participates in. This gives a per-entry "XOR score."
 * ================================================================ */

static int *g_xor_score;  /* per-entry count of XOR triples */

static int count_xor_and_score(int n_weights, int k_param, int bf_limit) {
    int count = 0;
    int limit = g_cat_size < bf_limit ? g_cat_size : bf_limit;
    int i0, i1, i2;
    int indices[3];

    /* Only support triples for now */
    if (n_weights != 3) return 0;

    for (i0 = 0; i0 < limit; i0++)
    for (i1 = i0+1; i1 < limit; i1++)
    for (i2 = i1+1; i2 < limit; i2++) {
        indices[0] = i0; indices[1] = i1; indices[2] = i2;
        if (test_xor_mat3(indices, 3, k_param)) {
            count++;
            g_xor_score[i0]++;
            g_xor_score[i1]++;
            g_xor_score[i2]++;
        }
    }
    return count;
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    char msg[256];
    int i;
    int class_counts[3];
    int xor_by_class[3];
    int entries_by_class[3];
    int total_xor;

    printf("KNOTAPEL DEMO 106: Topological Entropy of XOR-Computing Braids\n");
    printf("================================================================\n");

    /* Allocate heap arrays */
    g_cat_tl = (Mat3 *)calloc((size_t)MAX_CAT, sizeof(Mat3));
    g_cat_burau = (IntMat3 *)calloc((size_t)MAX_CAT, sizeof(IntMat3));
    g_xor_score = (int *)calloc((size_t)MAX_CAT, sizeof(int));

    if (!g_cat_tl || !g_cat_burau || !g_xor_score) {
        printf("  FAIL: memory allocation\n");
        return 1;
    }

    /* ============================================================
     * Phase 0: Build and Verify Representations
     * ============================================================ */
    printf("\n=== Phase 0: Representation Verification ===\n\n");

    build_tl_generators();
    build_burau_generators();

    /* Verify A * A_inv = 1 */
    {
        Cyc8 prod = cyc8_mul(g_A, g_A_inv);
        check("A * A_inv = 1", cyc8_eq(prod, cyc8_one()));
    }

    /* Verify delta = 0 */
    {
        Cyc8 a2 = cyc8_mul(g_A, g_A);
        Cyc8 ai2 = cyc8_mul(g_A_inv, g_A_inv);
        Cyc8 sum = cyc8_add(a2, ai2);
        check("A^2 + A^{-2} = 0 (delta=0)", cyc8_is_zero(sum));
    }

    /* Verify TL relations: e_i^2 = 0 */
    {
        int ei;
        for (ei = 0; ei < 3; ei++) {
            Mat3 sq = mat3_mul(&g_e[ei], &g_e[ei]);
            Mat3 z = mat3_zero();
            sprintf(msg, "e_%d^2 = 0 (delta=0)", ei + 1);
            check(msg, mat3_eq(&sq, &z));
        }
    }

    /* Verify TL braid relations */
    {
        Mat3 t;
        t = mat3_mul(&g_e[0], &g_e[1]);
        t = mat3_mul(&t, &g_e[0]);
        check("e_1*e_2*e_1 = e_1", mat3_eq(&t, &g_e[0]));

        t = mat3_mul(&g_e[1], &g_e[2]);
        t = mat3_mul(&t, &g_e[1]);
        check("e_2*e_3*e_2 = e_2", mat3_eq(&t, &g_e[1]));
    }

    /* Verify TL sigma_i * sigma_i_inv = I */
    {
        int si;
        Mat3 id3 = mat3_identity();
        for (si = 0; si < 3; si++) {
            Mat3 prod = mat3_mul(&g_gen_tl[2*si], &g_gen_tl[2*si+1]);
            sprintf(msg, "TL: sigma_%d * sigma_%d_inv = I", si+1, si+1);
            check(msg, mat3_eq(&prod, &id3));
        }
    }

    /* Verify Burau sigma_i * sigma_i_inv = I */
    {
        int si;
        IntMat3 id3 = imat3_identity();
        for (si = 0; si < 3; si++) {
            IntMat3 prod = imat3_mul(&g_gen_burau[2*si], &g_gen_burau[2*si+1]);
            sprintf(msg, "Burau: sigma_%d * sigma_%d_inv = I", si+1, si+1);
            check(msg, imat3_eq(&prod, &id3));
        }
    }

    /* Verify Burau braid relations */
    {
        IntMat3 lhs, rhs;

        lhs = imat3_mul(&g_gen_burau[0], &g_gen_burau[2]);
        lhs = imat3_mul(&lhs, &g_gen_burau[0]);
        rhs = imat3_mul(&g_gen_burau[2], &g_gen_burau[0]);
        rhs = imat3_mul(&rhs, &g_gen_burau[2]);
        check("Burau braid: s1*s2*s1 = s2*s1*s2", imat3_eq(&lhs, &rhs));

        lhs = imat3_mul(&g_gen_burau[2], &g_gen_burau[4]);
        lhs = imat3_mul(&lhs, &g_gen_burau[2]);
        rhs = imat3_mul(&g_gen_burau[4], &g_gen_burau[2]);
        rhs = imat3_mul(&rhs, &g_gen_burau[4]);
        check("Burau braid: s2*s3*s2 = s3*s2*s3", imat3_eq(&lhs, &rhs));
    }

    /* Verify Burau far commutativity */
    {
        IntMat3 s1s3 = imat3_mul(&g_gen_burau[0], &g_gen_burau[4]);
        IntMat3 s3s1 = imat3_mul(&g_gen_burau[4], &g_gen_burau[0]);
        check("Burau far: s1*s3 = s3*s1", imat3_eq(&s1s3, &s3s1));
    }

    /* Verify all Burau generators have det=1 */
    {
        int gi;
        int all_det_one = 1;
        for (gi = 0; gi < 6; gi++) {
            long d = imat3_det(&g_gen_burau[gi]);
            if (d != 1) { all_det_one = 0; break; }
        }
        check("All Burau generators have det=1", all_det_one);
    }

    /* Print Burau generators */
    printf("\n  Burau generators at t=-1 (3x3 integer):\n");
    imat3_print("sigma_1", &g_gen_burau[0]);
    imat3_print("sigma_1_inv", &g_gen_burau[1]);
    imat3_print("sigma_2", &g_gen_burau[2]);
    imat3_print("sigma_2_inv", &g_gen_burau[3]);
    imat3_print("sigma_3", &g_gen_burau[4]);
    imat3_print("sigma_3_inv", &g_gen_burau[5]);

    /* ============================================================
     * Phase 1: Dual BFS Catalog
     * ============================================================ */
    printf("\n=== Phase 1: Dual BFS Catalog (cap %d) ===\n\n", MAX_CAT);

    build_catalog(12);

    sprintf(msg, "Catalog has %d entries", g_cat_size);
    check(msg, g_cat_size > 100);

    /* Verify all Burau entries have det=1 */
    {
        int bad = 0;
        for (i = 0; i < g_cat_size; i++) {
            long d = imat3_det(&g_cat_burau[i]);
            if (d != 1) bad++;
        }
        sprintf(msg, "All %d Burau entries have det=1", g_cat_size);
        check(msg, bad == 0);
    }

    /* ============================================================
     * Phase 2: Entropy Classification
     * ============================================================ */
    printf("\n=== Phase 2: Entropy Classification ===\n\n");

    memset(class_counts, 0, sizeof(class_counts));

    for (i = 0; i < g_cat_size; i++) {
        long cpoly[3];
        int cls = classify_entropy(&g_cat_burau[i], cpoly);
        if (cls >= 0 && cls < 3) class_counts[cls]++;
    }

    printf("  Classification of %d catalog entries:\n", g_cat_size);
    printf("    Identity:       %6d (%5.1f%%)\n",
           class_counts[CLASS_IDENTITY],
           100.0 * class_counts[CLASS_IDENTITY] / g_cat_size);
    printf("    Periodic:       %6d (%5.1f%%)\n",
           class_counts[CLASS_PERIODIC],
           100.0 * class_counts[CLASS_PERIODIC] / g_cat_size);
    printf("    Pseudo-Anosov:  %6d (%5.1f%%)\n",
           class_counts[CLASS_PSEUDO_ANOSOV],
           100.0 * class_counts[CLASS_PSEUDO_ANOSOV] / g_cat_size);

    /* Distribution by depth */
    printf("\n  Classification by BFS depth:\n");
    printf("    %5s  %6s  %6s  %6s  %6s\n",
           "depth", "total", "ident", "per", "pA");
    {
        int d;
        for (d = 0; d <= 12; d++) {
            int dc[3] = {0,0,0};
            int total = 0;
            for (i = 0; i < g_cat_size; i++) {
                if (g_depth[i] == d) {
                    long cpoly[3];
                    int cls = classify_entropy(&g_cat_burau[i], cpoly);
                    if (cls >= 0 && cls < 3) dc[cls]++;
                    total++;
                }
            }
            if (total > 0)
                printf("    %5d  %6d  %6d  %6d  %6d\n",
                       d, total, dc[0], dc[1], dc[2]);
        }
    }

    /* Print char poly stats for pA entries */
    printf("\n  Char poly statistics for pseudo-Anosov entries:\n");
    {
        long min_tr = 999999, max_tr = -999999;
        long min_det = 999999, max_det = -999999;
        int n_pa = 0;

        for (i = 0; i < g_cat_size; i++) {
            long cpoly[3];
            int cls = classify_entropy(&g_cat_burau[i], cpoly);
            if (cls == CLASS_PSEUDO_ANOSOV) {
                long tr = imat3_trace(&g_cat_burau[i]);
                long det = imat3_det(&g_cat_burau[i]);
                if (tr < min_tr) min_tr = tr;
                if (tr > max_tr) max_tr = tr;
                if (det < min_det) min_det = det;
                if (det > max_det) max_det = det;
                n_pa++;
                if (n_pa <= 5) {
                    printf("    [%d] depth=%d tr=%ld det=%ld cpoly=[%ld,%ld,%ld]\n",
                           i, g_depth[i], tr, det,
                           cpoly[0], cpoly[1], cpoly[2]);
                }
            }
        }
        if (n_pa > 5) printf("    ... (%d more)\n", n_pa - 5);
        if (n_pa > 0)
            printf("    Trace range: [%ld, %ld], Det range: [%ld, %ld]\n",
                   min_tr, max_tr, min_det, max_det);
    }

    /* ============================================================
     * Phase 3: XOR Capacity + Entropy Correlation
     * ============================================================ */
    printf("\n=== Phase 3: XOR Capacity + Entropy Correlation ===\n\n");

    /* Compute XOR triples and per-entry scores */
    {
        int k_param = 128;
        int bf_limit = 128; /* Use first 128 entries for tractable search */

        printf("  XOR6 search: k=%d, limit=%d entries\n", k_param, bf_limit);
        total_xor = count_xor_and_score(3, k_param, bf_limit);
        sprintf(msg, "XOR6 triples found: %d", total_xor);
        check(msg, 1); /* Report regardless */
        printf("  Total XOR6 triples: %d\n", total_xor);
    }

    /* Contingency table: entropy class vs XOR participation */
    printf("\n  Contingency table (first 128 entries):\n");
    memset(xor_by_class, 0, sizeof(xor_by_class));
    memset(entries_by_class, 0, sizeof(entries_by_class));

    {
        int limit = g_cat_size < 128 ? g_cat_size : 128;
        for (i = 0; i < limit; i++) {
            long cpoly[3];
            int cls = classify_entropy(&g_cat_burau[i], cpoly);
            if (cls >= 0 && cls < 3) {
                entries_by_class[cls]++;
                if (g_xor_score[i] > 0) xor_by_class[cls]++;
            }
        }
    }

    printf("    %-16s  %6s  %6s  %6s  %8s\n",
           "Class", "Total", "XOR>0", "XOR=0", "XOR rate");
    {
        const char *names[3] = {"Identity", "Periodic", "Pseudo-Anosov"};
        int c;
        for (c = 0; c < 3; c++) {
            if (entries_by_class[c] > 0) {
                printf("    %-16s  %6d  %6d  %6d  %7.1f%%\n",
                       names[c], entries_by_class[c], xor_by_class[c],
                       entries_by_class[c] - xor_by_class[c],
                       100.0 * xor_by_class[c] / entries_by_class[c]);
            }
        }
    }

    /* XOR score distribution by class */
    printf("\n  XOR score distribution by entropy class (first 128 entries):\n");
    {
        int limit = g_cat_size < 128 ? g_cat_size : 128;
        const char *names[3] = {"Identity", "Periodic", "Pseudo-Anosov"};
        int c;
        for (c = 0; c < 3; c++) {
            long total_score = 0;
            int count = 0;
            int max_score = 0;
            for (i = 0; i < limit; i++) {
                long cpoly[3];
                int cls = classify_entropy(&g_cat_burau[i], cpoly);
                if (cls == c) {
                    total_score += g_xor_score[i];
                    if (g_xor_score[i] > max_score) max_score = g_xor_score[i];
                    count++;
                }
            }
            if (count > 0) {
                printf("    %-16s: n=%d, total_score=%ld, mean=%.1f, max=%d\n",
                       names[c], count, total_score,
                       (double)total_score / count, max_score);
            }
        }
    }

    /* ============================================================
     * Phase 4: Higher k sweep (still on first 128)
     * ============================================================ */
    printf("\n=== Phase 4: k-Sweep (first 128 entries) ===\n\n");
    {
        int k_vals[] = {64, 128, 256, 512, 1024};
        int nk = 5;
        int ki;

        printf("    %6s  %8s\n", "k", "XOR6");
        for (ki = 0; ki < nk; ki++) {
            int k = k_vals[ki];
            int xor_count;

            /* Reset scores */
            memset(g_xor_score, 0, (size_t)MAX_CAT * sizeof(int));
            xor_count = count_xor_and_score(3, k, 128);
            printf("    %6d  %8d\n", k, xor_count);
        }
    }

    /* ============================================================
     * Phase 5: Entropy Per Crossing Analysis
     * ============================================================ */
    printf("\n=== Phase 5: Entropy Per Crossing ===\n\n");
    {
        /* log(phi) where phi = golden ratio = (1+sqrt(5))/2 */
        double log_phi = log((1.0 + sqrt(5.0)) / 2.0);
        int limit = g_cat_size < 128 ? g_cat_size : 128;
        double total_epc_xor = 0.0, total_epc_noxor = 0.0;
        int n_pa_xor = 0, n_pa_noxor = 0;
        double max_epc = 0.0;
        int max_epc_idx = -1;

        printf("  Reference: log(phi) = %.6f (optimal mixing entropy per crossing)\n\n",
               log_phi);

        /* Reset XOR scores for this analysis */
        memset(g_xor_score, 0, (size_t)MAX_CAT * sizeof(int));
        total_xor = count_xor_and_score(3, 128, limit);

        printf("  Entropy per crossing for pseudo-Anosov entries (first %d):\n", limit);
        printf("    %5s  %5s  %10s  %10s  %6s\n",
               "idx", "depth", "spec_rad", "entropy/d", "xor_sc");

        for (i = 0; i < limit; i++) {
            long cpoly[3];
            int cls = classify_entropy(&g_cat_burau[i], cpoly);
            if (cls == CLASS_PSEUDO_ANOSOV && g_depth[i] > 0) {
                double sr = spectral_radius(&g_cat_burau[i]);
                double ent = log(sr);
                double epc = ent / (double)g_depth[i];

                if (epc > max_epc) { max_epc = epc; max_epc_idx = i; }

                if (g_xor_score[i] > 0) {
                    total_epc_xor += epc;
                    n_pa_xor++;
                } else {
                    total_epc_noxor += epc;
                    n_pa_noxor++;
                }

                /* Print first 10 */
                if (n_pa_xor + n_pa_noxor <= 10)
                    printf("    %5d  %5d  %10.6f  %10.6f  %6d\n",
                           i, g_depth[i], sr, epc, g_xor_score[i]);
            }
        }

        if (n_pa_xor + n_pa_noxor > 10)
            printf("    ... (%d more pA entries)\n",
                   n_pa_xor + n_pa_noxor - 10);

        printf("\n  Summary:\n");
        if (n_pa_xor > 0)
            printf("    pA entries with XOR>0: n=%d, mean entropy/crossing=%.6f\n",
                   n_pa_xor, total_epc_xor / n_pa_xor);
        if (n_pa_noxor > 0)
            printf("    pA entries with XOR=0: n=%d, mean entropy/crossing=%.6f\n",
                   n_pa_noxor, total_epc_noxor / n_pa_noxor);
        if (max_epc_idx >= 0)
            printf("    Max entropy/crossing: %.6f at entry %d (depth %d)\n",
                   max_epc, max_epc_idx, g_depth[max_epc_idx]);
        printf("    log(phi) = %.6f for comparison\n", log_phi);

        /* Histogram of entropy per crossing */
        printf("\n  Entropy per crossing histogram (pA entries, first %d):\n", limit);
        {
            int bins[10];
            int bi;
            double bin_width = log_phi / 5.0; /* 10 bins from 0 to 2*log(phi) */
            memset(bins, 0, sizeof(bins));
            for (i = 0; i < limit; i++) {
                long cpoly2[3];
                int cls2 = classify_entropy(&g_cat_burau[i], cpoly2);
                if (cls2 == CLASS_PSEUDO_ANOSOV && g_depth[i] > 0) {
                    double sr2 = spectral_radius(&g_cat_burau[i]);
                    double epc2 = log(sr2) / (double)g_depth[i];
                    bi = (int)(epc2 / bin_width);
                    if (bi < 0) bi = 0;
                    if (bi > 9) bi = 9;
                    bins[bi]++;
                }
            }
            for (bi = 0; bi < 10; bi++) {
                printf("    [%.3f, %.3f): %d\n",
                       (double)bi * bin_width, ((double)bi + 1.0) * bin_width,
                       bins[bi]);
            }
        }
    }

    /* ============================================================
     * Phase 6: Boolean Function Sweep (Entropy x Function)
     *
     * Test whether entropy class correlates with ANY Boolean function.
     * +/- encoding: for each weight, either add +w or -w.
     * 3 weights -> 8 input patterns.
     * ============================================================ */
    printf("\n=== Phase 6: Boolean Function Sweep ===\n\n");
    {
        /* Truth tables for 3-input Boolean functions.
         * Bit p of truth table = f(b0,b1,b2) where p = b0 + 2*b1 + 4*b2. */
        static const unsigned char func_tt[] = {
            0x80,  /* AND3:  b0 & b1 & b2                       */
            0xFE,  /* OR3:   b0 | b1 | b2                       */
            0x96,  /* XOR3:  b0 ^ b1 ^ b2  (parity)             */
            0xE8,  /* MAJ3:  majority(b0,b1,b2)                  */
            0x7F,  /* NAND3: ~(b0 & b1 & b2)                    */
            0x01,  /* NOR3:  ~(b0 | b1 | b2)                    */
            0x88,  /* AND2:  b0 & b1 (ignore b2)                */
            0x66   /* XOR2:  b0 ^ b1 (ignore b2)                */
        };
        static const char *func_names[] = {
            "AND3 ", "OR3  ", "XOR3 ", "MAJ3 ",
            "NAND3", "NOR3 ", "AND2 ", "XOR2 "
        };
        int n_funcs = 8;
        int fi;
        int bf_limit = g_cat_size < 128 ? g_cat_size : 128;
        int k_sweep = 128;
        int *entry_class;
        int n_per_local = 0, n_pa_local = 0;
        long c_per_3, c_pa_3, c_total_3;

        entry_class = (int *)calloc((size_t)bf_limit, sizeof(int));

        for (i = 0; i < bf_limit; i++) {
            long cpoly[3];
            entry_class[i] = classify_entropy(&g_cat_burau[i], cpoly);
            if (entry_class[i] <= CLASS_PERIODIC) n_per_local++;
            if (entry_class[i] == CLASS_PSEUDO_ANOSOV) n_pa_local++;
        }

        c_per_3 = (long)n_per_local * (n_per_local - 1) * (n_per_local - 2) / 6;
        c_pa_3 = (long)n_pa_local * (n_pa_local - 1) * (n_pa_local - 2) / 6;
        c_total_3 = (long)bf_limit * (bf_limit - 1) * (bf_limit - 2) / 6;

        printf("  +/- encoding: 3 weights -> 8 patterns, k=%d\n", k_sweep);
        printf("  First %d entries: periodic=%d, pA=%d\n",
               bf_limit, n_per_local, n_pa_local);
        printf("  Triples: C(%d,3)=%ld total, C(%d,3)=%ld per-only, "
               "C(%d,3)=%ld pA-only\n\n",
               bf_limit, c_total_3,
               n_per_local, c_per_3,
               n_pa_local, c_pa_3);

        printf("  %-6s  %7s  %7s  %7s  %7s  %8s  %8s\n",
               "Func", "Total", "PerOnly", "pAOnly", "Mixed",
               "PerRate", "pARate");

        for (fi = 0; fi < n_funcs; fi++) {
            int ftotal = 0, per_only = 0, pa_only = 0, mixed_ct = 0;
            int i0, i1, i2;

            for (i0 = 0; i0 < bf_limit; i0++)
            for (i1 = i0 + 1; i1 < bf_limit; i1++)
            for (i2 = i1 + 1; i2 < bf_limit; i2++) {
                int indices[3];
                int c0, c1, c2, all_per, all_pa;

                indices[0] = i0; indices[1] = i1; indices[2] = i2;
                if (!test_bool_pm_mat3(indices, 3, k_sweep, func_tt[fi]))
                    continue;

                c0 = entry_class[i0];
                c1 = entry_class[i1];
                c2 = entry_class[i2];
                all_per = (c0 <= CLASS_PERIODIC) && (c1 <= CLASS_PERIODIC)
                       && (c2 <= CLASS_PERIODIC);
                all_pa = (c0 == CLASS_PSEUDO_ANOSOV)
                      && (c1 == CLASS_PSEUDO_ANOSOV)
                      && (c2 == CLASS_PSEUDO_ANOSOV);

                ftotal++;
                if (all_per) per_only++;
                else if (all_pa) pa_only++;
                else mixed_ct++;
            }

            printf("  %-6s  %7d  %7d  %7d  %7d  %7.1f%%  %7.1f%%\n",
                   func_names[fi], ftotal, per_only, pa_only, mixed_ct,
                   c_per_3 > 0 ? 100.0 * (double)per_only / (double)c_per_3 : 0.0,
                   c_pa_3 > 0 ? 100.0 * (double)pa_only / (double)c_pa_3 : 0.0);
        }

        /* Null hypothesis: if entropy is irrelevant, success rates should
         * be roughly proportional to triple counts */
        printf("\n  Null hypothesis baseline (if entropy irrelevant):\n");
        printf("    Per-only fraction of all triples: %.1f%%\n",
               100.0 * (double)c_per_3 / (double)c_total_3);
        printf("    pA-only fraction of all triples:  %.1f%%\n",
               100.0 * (double)c_pa_3 / (double)c_total_3);

        free(entry_class);
    }

    /* ============================================================
     * Summary
     * ============================================================ */
    printf("\n=== Summary ===\n\n");
    printf("  %d pass, %d fail\n\n", n_pass, n_fail);

    {
        int n_pa = class_counts[CLASS_PSEUDO_ANOSOV];
        int n_non_pa = class_counts[CLASS_PERIODIC];
        printf("  PREDICTION TEST:\n");
        printf("    Hypothesis: XOR-computing braids are pseudo-Anosov\n");
        printf("    Total entries: %d\n", g_cat_size);
        printf("    Pseudo-Anosov: %d (%.1f%%)\n",
               n_pa, 100.0 * n_pa / g_cat_size);
        printf("    Periodic (excl identity): %d (%.1f%%)\n",
               n_non_pa, 100.0 * n_non_pa / g_cat_size);
        printf("    XOR-participating in pA: %d / %d\n",
               xor_by_class[CLASS_PSEUDO_ANOSOV],
               entries_by_class[CLASS_PSEUDO_ANOSOV]);
        printf("    XOR-participating in periodic: %d / %d\n",
               xor_by_class[CLASS_PERIODIC],
               entries_by_class[CLASS_PERIODIC]);
    }

    free(g_cat_tl);
    free(g_cat_burau);
    free(g_xor_score);

    return n_fail > 0 ? 1 : 0;
}
