/*
 * KNOTAPEL DEMO 100: 4-Strand DKC on W_{4,2}
 * ============================================
 *
 * TL_4 at delta=0 is NON-SEMISIMPLE (n=4 even).
 * Radical dim=9, Loewy length 3.
 * Two simple modules: L_{4,2} (dim 2), L_{4,4} (dim 1).
 *
 * W_{4,2} is the standard module with 2 through-lines, dim=3.
 * Extension: 0 -> L_{4,4} -> W_{4,2} -> L_{4,2} -> 0
 * Every BFS entry is already non-semisimple.
 *
 * Basis: three half-diagrams for 4 strands with 2 through-lines:
 *   h_0: {1,2} paired, through-lines 3,4
 *   h_1: {2,3} paired, through-lines 1,4
 *   h_2: {3,4} paired, through-lines 1,2
 *
 * TL generators (3x3 over Z, verified by hand):
 *   e_1 = [[0,1,0],[0,0,0],[0,0,0]]
 *   e_2 = [[0,0,0],[1,0,1],[0,0,0]]
 *   e_3 = [[0,0,0],[0,0,0],[0,1,0]]
 *
 * Braid generators: sigma_i = A*I + A^{-1}*e_i  (3x3 over Z[zeta_8])
 * A = -zeta_8 = (0,-1,0,0), A^{-1} = (0,0,0,1)
 *
 * 6 generators: sigma_1, sigma_1_inv, sigma_2, sigma_2_inv,
 *               sigma_3, sigma_3_inv
 *
 * Note: sigma_1 and sigma_3 COMMUTE (non-adjacent strands).
 *
 * C89, zero dependencies beyond stdio/stdlib/string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static Cyc8 cyc8_sub(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a - y.a, x.b - y.b, x.c - y.c, x.d - y.d);
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
 * 3x3 Matrix over Cyc8 (W_{4,2} representation)
 * Matrix: [[m00,m01,m02],[m10,m11,m12],[m20,m21,m22]]
 * Stored as array of 9 Cyc8 values: m[row][col]
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

static int mat3_eq(const Mat3 *p, const Mat3 *q) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (!cyc8_eq(p->m[i][j], q->m[i][j])) return 0;
    return 1;
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

static void mat3_print(const char *name, const Mat3 *m) {
    int i, j;
    printf("  %s:\n", name);
    for (i = 0; i < 3; i++) {
        printf("    [");
        for (j = 0; j < 3; j++) {
            printf("(%ld,%ld,%ld,%ld)",
                   m->m[i][j].a, m->m[i][j].b,
                   m->m[i][j].c, m->m[i][j].d);
            if (j < 2) printf(", ");
        }
        printf("]\n");
    }
}

/* ================================================================
 * Hash table for Mat3 BFS
 * ================================================================ */

#define MAX_CAT 32768
#define HASH_SIZE 65537

static Mat3  g_cat[MAX_CAT];
static int   g_depth[MAX_CAT];
static int   g_writhe[MAX_CAT];
static int   g_cat_size = 0;

static int g_hash_head[HASH_SIZE];
static int g_hash_next[MAX_CAT];

static unsigned long hash_mat3(const Mat3 *m) {
    unsigned long h = 2166136261UL;
    const long *p = &m->m[0][0].a;
    int i;
    for (i = 0; i < 36; i++) {
        h = (h * 1000003UL) ^ (unsigned long)p[i];
    }
    return h;
}

static void hash_init(void) {
    memset(g_hash_head, -1, sizeof(g_hash_head));
}

static int hash_find(const Mat3 *m) {
    int bucket = (int)(hash_mat3(m) % (unsigned long)HASH_SIZE);
    int idx = g_hash_head[bucket];
    while (idx >= 0) {
        if (mat3_eq(&g_cat[idx], m)) return idx;
        idx = g_hash_next[idx];
    }
    return -1;
}

static void hash_insert(int cat_idx) {
    int bucket = (int)(hash_mat3(&g_cat[cat_idx]) % (unsigned long)HASH_SIZE);
    g_hash_next[cat_idx] = g_hash_head[bucket];
    g_hash_head[bucket] = cat_idx;
}

/* ================================================================
 * TL generators on W_{4,2}
 *
 * e_1 = [[0,1,0],[0,0,0],[0,0,0]]
 * e_2 = [[0,0,0],[1,0,1],[0,0,0]]
 * e_3 = [[0,0,0],[0,0,0],[0,1,0]]
 * ================================================================ */

static Mat3 g_e[3]; /* e_1, e_2, e_3 (0-indexed: g_e[0]=e_1) */

static void build_tl_generators(void) {
    int i;
    for (i = 0; i < 3; i++)
        g_e[i] = mat3_zero();

    /* e_1: h_1 -> h_0 */
    g_e[0].m[0][1] = cyc8_one();

    /* e_2: h_0 -> h_1, h_2 -> h_1 */
    g_e[1].m[1][0] = cyc8_one();
    g_e[1].m[1][2] = cyc8_one();

    /* e_3: h_1 -> h_2 */
    g_e[2].m[2][1] = cyc8_one();
}

/* ================================================================
 * Braid generators on W_{4,2}
 *
 * sigma_i = A*I + A^{-1}*e_i
 * sigma_i_inv = A^{-1}*I + A*e_i
 *
 * 6 generators indexed 0-5:
 *   0: sigma_1,     1: sigma_1_inv
 *   2: sigma_2,     3: sigma_2_inv
 *   4: sigma_3,     5: sigma_3_inv
 * ================================================================ */

static Cyc8 g_A, g_A_inv;
static Mat3 g_gen[6];
static const int g_gen_writhe[6] = {1, -1, 1, -1, 1, -1};

static void build_braid_generators(void) {
    int i;
    Mat3 id3 = mat3_identity();
    Mat3 a_id, a_inv_id;

    g_A = cyc8_make(0, -1, 0, 0);
    g_A_inv = cyc8_conj(g_A);  /* (0,0,0,1) */

    a_id = mat3_scale(g_A, &id3);
    a_inv_id = mat3_scale(g_A_inv, &id3);

    for (i = 0; i < 3; i++) {
        Mat3 a_inv_e = mat3_scale(g_A_inv, &g_e[i]);
        Mat3 a_e = mat3_scale(g_A, &g_e[i]);
        /* sigma_i = A*I + A^{-1}*e_i */
        g_gen[2*i] = mat3_add(&a_id, &a_inv_e);
        /* sigma_i_inv = A^{-1}*I + A*e_i */
        g_gen[2*i+1] = mat3_add(&a_inv_id, &a_e);
    }
}

/* ================================================================
 * BFS catalog building (6 generators)
 * ================================================================ */

static void build_catalog(int max_depth) {
    int prev, gi, i, rd;

    g_cat_size = 0;
    hash_init();

    g_cat[0] = mat3_identity();
    g_depth[0] = 0;
    g_writhe[0] = 0;
    hash_insert(0);
    g_cat_size = 1;

    printf("  Round 0: 1 entry\n");

    rd = 1;
    do {
        long round_max = 0;
        prev = g_cat_size;
        for (i = 0; i < prev && g_cat_size < MAX_CAT; i++) {
            if (g_depth[i] != rd - 1) continue; /* only expand frontier */
            for (gi = 0; gi < 6 && g_cat_size < MAX_CAT; gi++) {
                Mat3 prod = mat3_mul(&g_cat[i], &g_gen[gi]);
                if (hash_find(&prod) < 0) {
                    long mabs = mat3_max_abs(&prod);
                    if (mabs > round_max) round_max = mabs;
                    g_cat[g_cat_size] = prod;
                    g_depth[g_cat_size] = rd;
                    g_writhe[g_cat_size] = g_writhe[i] + g_gen_writhe[gi];
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
    } while (g_cat_size > prev && g_cat_size < MAX_CAT && rd <= max_depth);

    if (g_cat_size == prev) {
        printf("  GROUP CLOSED at %d entries in %d rounds\n",
               g_cat_size, rd - 1);
    } else if (g_cat_size >= MAX_CAT) {
        printf("  HIT CAP at %d entries (group is infinite)\n", MAX_CAT);
    } else {
        printf("  Stopped at depth %d with %d entries\n",
               max_depth, g_cat_size);
    }
}

/* ================================================================
 * Activation: 3x3_hash
 * Sign of all 36 integer components (9 entries x 4 Cyc8 components).
 * 3-valued: pos=2, zero=1, neg=0. Hash mod k_param.
 * ================================================================ */

#define ACT_3x3_HASH  0

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
 * XOR test for 3x3 matrices
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
        pos[w] = g_cat[indices[w]];
        neg[w] = mat3_neg(&g_cat[indices[w]]);
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

/* Brute-force XOR search */
static int count_xor_bf(int n_weights, int k_param, int bf_limit) {
    int count = 0;
    int limit = g_cat_size < bf_limit ? g_cat_size : bf_limit;
    int i0, i1, i2, i3, i4, i5, i6;
    int indices[8];

    if (n_weights == 3) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            if (test_xor_mat3(indices, 3, k_param))
                count++;
        }
    } else if (n_weights == 4) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++) {
            indices[0] = i0; indices[1] = i1;
            indices[2] = i2; indices[3] = i3;
            if (test_xor_mat3(indices, 4, k_param))
                count++;
        }
    } else if (n_weights == 5) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++)
        for (i4 = i3+1; i4 < limit; i4++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            indices[3] = i3; indices[4] = i4;
            if (test_xor_mat3(indices, 5, k_param))
                count++;
        }
    } else if (n_weights == 6) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++)
        for (i4 = i3+1; i4 < limit; i4++)
        for (i5 = i4+1; i5 < limit; i5++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            indices[3] = i3; indices[4] = i4; indices[5] = i5;
            if (test_xor_mat3(indices, 6, k_param))
                count++;
        }
    } else if (n_weights == 7) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++)
        for (i4 = i3+1; i4 < limit; i4++)
        for (i5 = i4+1; i5 < limit; i5++)
        for (i6 = i5+1; i6 < limit; i6++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            indices[3] = i3; indices[4] = i4; indices[5] = i5;
            indices[6] = i6;
            if (test_xor_mat3(indices, 7, k_param))
                count++;
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

    printf("KNOTAPEL DEMO 100: 4-Strand DKC on W_{4,2}\n");
    printf("============================================\n");

    /* ============================================================
     * Phase 0: Build and Verify Representation
     * ============================================================ */
    printf("\n=== Phase 0: Representation Verification ===\n\n");

    build_tl_generators();

    printf("  A = (0,-1,0,0)\n");
    printf("  A_inv = (0,0,0,1)\n");

    g_A = cyc8_make(0, -1, 0, 0);
    g_A_inv = cyc8_conj(g_A);

    {
        Cyc8 prod = cyc8_mul(g_A, g_A_inv);
        check("A * A_inv = 1", cyc8_eq(prod, cyc8_one()));
    }
    {
        Cyc8 a2 = cyc8_mul(g_A, g_A);
        Cyc8 ai2 = cyc8_mul(g_A_inv, g_A_inv);
        Cyc8 sum = cyc8_add(a2, ai2);
        check("A^2 + A^{-2} = 0 (delta=0)", cyc8_is_zero(sum));
    }

    /* Verify TL relations */
    printf("\n  TL generators (3x3 integer matrices):\n");
    mat3_print("e_1", &g_e[0]);
    mat3_print("e_2", &g_e[1]);
    mat3_print("e_3", &g_e[2]);

    /* e_i^2 = delta * e_i = 0 */
    {
        int ei;
        for (ei = 0; ei < 3; ei++) {
            Mat3 sq = mat3_mul(&g_e[ei], &g_e[ei]);
            Mat3 z = mat3_zero();
            sprintf(msg, "e_%d^2 = 0 (delta=0)", ei + 1);
            check(msg, mat3_eq(&sq, &z));
        }
    }

    /* e_i * e_{i+1} * e_i = e_i (Jones-Wenzl) */
    {
        Mat3 t;
        t = mat3_mul(&g_e[0], &g_e[1]);
        t = mat3_mul(&t, &g_e[0]);
        check("e_1 * e_2 * e_1 = e_1", mat3_eq(&t, &g_e[0]));

        t = mat3_mul(&g_e[1], &g_e[0]);
        t = mat3_mul(&t, &g_e[1]);
        check("e_2 * e_1 * e_2 = e_2", mat3_eq(&t, &g_e[1]));

        t = mat3_mul(&g_e[1], &g_e[2]);
        t = mat3_mul(&t, &g_e[1]);
        check("e_2 * e_3 * e_2 = e_2", mat3_eq(&t, &g_e[1]));

        t = mat3_mul(&g_e[2], &g_e[1]);
        t = mat3_mul(&t, &g_e[2]);
        check("e_3 * e_2 * e_3 = e_3", mat3_eq(&t, &g_e[2]));
    }

    /* Far commutativity: e_1 * e_3 = e_3 * e_1 */
    {
        Mat3 e1e3 = mat3_mul(&g_e[0], &g_e[2]);
        Mat3 e3e1 = mat3_mul(&g_e[2], &g_e[0]);
        check("e_1 * e_3 = e_3 * e_1 (far commutativity)", mat3_eq(&e1e3, &e3e1));
    }

    /* Build braid generators */
    build_braid_generators();

    printf("\n  Braid generators:\n");
    mat3_print("sigma_1", &g_gen[0]);
    mat3_print("sigma_2", &g_gen[2]);
    mat3_print("sigma_3", &g_gen[4]);

    /* Verify sigma_i * sigma_i_inv = I */
    {
        int si;
        Mat3 id3 = mat3_identity();
        for (si = 0; si < 3; si++) {
            Mat3 prod = mat3_mul(&g_gen[2*si], &g_gen[2*si+1]);
            sprintf(msg, "sigma_%d * sigma_%d_inv = I", si+1, si+1);
            check(msg, mat3_eq(&prod, &id3));
        }
    }

    /* Braid relations: sigma_i * sigma_{i+1} * sigma_i = sigma_{i+1} * sigma_i * sigma_{i+1} */
    {
        Mat3 lhs, rhs;

        lhs = mat3_mul(&g_gen[0], &g_gen[2]); /* s1 * s2 */
        lhs = mat3_mul(&lhs, &g_gen[0]);       /* * s1 */
        rhs = mat3_mul(&g_gen[2], &g_gen[0]); /* s2 * s1 */
        rhs = mat3_mul(&rhs, &g_gen[2]);       /* * s2 */
        check("Braid: s1*s2*s1 = s2*s1*s2", mat3_eq(&lhs, &rhs));

        lhs = mat3_mul(&g_gen[2], &g_gen[4]); /* s2 * s3 */
        lhs = mat3_mul(&lhs, &g_gen[2]);       /* * s2 */
        rhs = mat3_mul(&g_gen[4], &g_gen[2]); /* s3 * s2 */
        rhs = mat3_mul(&rhs, &g_gen[4]);       /* * s3 */
        check("Braid: s2*s3*s2 = s3*s2*s3", mat3_eq(&lhs, &rhs));
    }

    /* Far commutativity: sigma_1 * sigma_3 = sigma_3 * sigma_1 */
    {
        Mat3 s1s3 = mat3_mul(&g_gen[0], &g_gen[4]);
        Mat3 s3s1 = mat3_mul(&g_gen[4], &g_gen[0]);
        check("Far: s1*s3 = s3*s1", mat3_eq(&s1s3, &s3s1));
    }

    /* Hecke relations: sigma_i - sigma_i_inv = (A - A_inv)(I - e_i) */
    {
        int si;
        Cyc8 a_minus_ainv = cyc8_sub(g_A, g_A_inv);
        Mat3 id3 = mat3_identity();

        for (si = 0; si < 3; si++) {
            Mat3 neg_sinv = mat3_neg(&g_gen[2*si+1]);
            Mat3 lhs_m = mat3_add(&g_gen[2*si], &neg_sinv);
            Mat3 neg_ei = mat3_neg(&g_e[si]);
            Mat3 i_minus_e = mat3_add(&id3, &neg_ei);
            Mat3 rhs_m = mat3_scale(a_minus_ainv, &i_minus_e);
            sprintf(msg, "Hecke: s%d - s%d_inv = (A-A_inv)(I-e%d)", si+1, si+1, si+1);
            check(msg, mat3_eq(&lhs_m, &rhs_m));
        }
    }

    /* ============================================================
     * Phase 1: BFS Catalog
     * ============================================================ */
    printf("\n=== Phase 1: BFS Catalog (cap %d) ===\n\n", MAX_CAT);

    build_catalog(12);

    sprintf(msg, "Catalog has %d entries", g_cat_size);
    check(msg, g_cat_size > 100);

    /* ============================================================
     * Phase 2: Catalog Statistics
     * ============================================================ */
    printf("\n=== Phase 2: Catalog Statistics ===\n\n");

    {
        int max_d = 0;
        long max_mag = 0;
        int depth_count[20];
        int zero_count = 0;

        memset(depth_count, 0, sizeof(depth_count));

        for (i = 0; i < g_cat_size; i++) {
            long m = mat3_max_abs(&g_cat[i]);
            if (m > max_mag) max_mag = m;
            if (g_depth[i] > max_d && g_depth[i] < 20) max_d = g_depth[i];
            if (g_depth[i] < 20) depth_count[g_depth[i]]++;

            /* Count entries with any zero component in diagonal */
            if (cyc8_is_zero(g_cat[i].m[0][0]) ||
                cyc8_is_zero(g_cat[i].m[1][1]) ||
                cyc8_is_zero(g_cat[i].m[2][2]))
                zero_count++;
        }

        printf("  Max depth: %d\n", max_d);
        printf("  Max entry magnitude: %ld\n", max_mag);
        printf("  Entries with zero diagonal element: %d (%.1f%%)\n",
               zero_count, 100.0 * zero_count / g_cat_size);

        printf("  Depth distribution:\n");
        {
            int d;
            for (d = 0; d <= max_d; d++) {
                if (depth_count[d] > 0)
                    printf("    d=%d: %d entries\n", d, depth_count[d]);
            }
        }
    }

    /* Print first 5 entries */
    printf("\n  First 5 catalog entries:\n");
    for (i = 0; i < 5 && i < g_cat_size; i++) {
        printf("    [%d] d=%d w=%d\n", i, g_depth[i], g_writhe[i]);
        {
            int r, c;
            for (r = 0; r < 3; r++) {
                printf("      [");
                for (c = 0; c < 3; c++) {
                    printf("(%ld,%ld,%ld,%ld)",
                           g_cat[i].m[r][c].a, g_cat[i].m[r][c].b,
                           g_cat[i].m[r][c].c, g_cat[i].m[r][c].d);
                    if (c < 2) printf(", ");
                }
                printf("]\n");
            }
        }
    }

    /* Growth rate comparison */
    printf("\n  Growth rate (new entries per round):\n");
    {
        int d, prev_count = 1;
        int depth_count2[20];
        memset(depth_count2, 0, sizeof(depth_count2));
        for (i = 0; i < g_cat_size; i++)
            if (g_depth[i] < 20) depth_count2[g_depth[i]]++;

        for (d = 1; d < 20 && depth_count2[d] > 0; d++) {
            printf("    d=%d: +%d (ratio: %.2f)\n",
                   d, depth_count2[d],
                   prev_count > 0 ? (double)depth_count2[d] / prev_count : 0.0);
            prev_count = depth_count2[d];
        }
    }

    /* Check if sigma_1 has infinite order (Jordan block test) */
    printf("\n  sigma_1^n growth test:\n");
    {
        Mat3 s1_pow = mat3_identity();
        int n;
        for (n = 1; n <= 8; n++) {
            s1_pow = mat3_mul(&s1_pow, &g_gen[0]);
            printf("    n=%d: max_abs=%ld\n", n, mat3_max_abs(&s1_pow));
        }
        {
            Mat3 id3 = mat3_identity();
            int is_identity = mat3_eq(&s1_pow, &id3);
            printf("  sigma_1^8 %s identity\n", is_identity ? "=" : "!=");
            check("sigma_1 order test completed", 1);
        }
    }

    /* ============================================================
     * Phase 3: XOR Capacity Scan
     * ============================================================ */
    printf("\n=== Phase 3: XOR Capacity Scan ===\n\n");

    {
        static const int k_vals[] = {64, 128, 256, 512};
        int ki;
        int best_xor6 = 0;
        int best_k = 0;

        printf("  %-8s  %10s  %10s  %10s  %10s\n",
               "k_cells", "XOR6", "XOR8", "XOR10", "XOR12");
        printf("  %-8s  %10s  %10s  %10s  %10s\n",
               "--------", "----------", "----------",
               "----------", "----------");

        for (ki = 0; ki < 4; ki++) {
            int k = k_vals[ki];
            int x6, x8, x10, x12;
            int bf = g_cat_size < 30 ? g_cat_size : 30;

            x6  = count_xor_bf(3, k, bf);
            x8  = count_xor_bf(4, k, bf);
            x10 = count_xor_bf(5, k, bf);
            x12 = count_xor_bf(6, k, bf);

            printf("  %-8d  %10d  %10d  %10d  %10d\n",
                   k, x6, x8, x10, x12);

            if (x6 > best_xor6) {
                best_xor6 = x6;
                best_k = k;
            }
        }

        sprintf(msg, "Best XOR6: %d (k=%d)", best_xor6, best_k);
        check(msg, 1);

        /* Deep entry scan */
        {
            int deep_idx[1024];
            int nd = 0;
            Mat3 saved[40];
            int sd[40], sw[40];
            int saved_sz = g_cat_size;
            int bf_deep, step, j;

            for (i = 0; i < g_cat_size && nd < 1024; i++) {
                if (g_depth[i] >= 4)
                    deep_idx[nd++] = i;
            }
            printf("\n  Deep entries (d>=4): %d\n", nd);

            bf_deep = nd < 30 ? nd : 30;
            if (bf_deep >= 7) {
                step = nd / bf_deep;
                if (step < 1) step = 1;

                for (j = 0; j < bf_deep && j < 40; j++) {
                    saved[j] = g_cat[j];
                    sd[j] = g_depth[j];
                    sw[j] = g_writhe[j];
                }
                for (j = 0; j < bf_deep; j++) {
                    int src = deep_idx[j * step];
                    g_cat[j] = g_cat[src];
                    g_depth[j] = g_depth[src];
                    g_writhe[j] = g_writhe[src];
                }
                g_cat_size = bf_deep;

                printf("  Deep bf=%d:\n", bf_deep);
                {
                    int x6d, x8d, x10d, x12d, x14d;
                    x6d  = count_xor_bf(3, 128, bf_deep);
                    x8d  = count_xor_bf(4, 128, bf_deep);
                    x10d = count_xor_bf(5, 128, bf_deep);
                    x12d = count_xor_bf(6, 128, bf_deep);
                    x14d = count_xor_bf(7, 128, bf_deep);
                    printf("    XOR6=%d  XOR8=%d  XOR10=%d  XOR12=%d  XOR14=%d\n",
                           x6d, x8d, x10d, x12d, x14d);
                    sprintf(msg, "Deep XOR6=%d XOR8=%d XOR10=%d XOR12=%d XOR14=%d",
                            x6d, x8d, x10d, x12d, x14d);
                    check(msg, 1);
                }

                /* Restore */
                for (j = 0; j < bf_deep && j < 40; j++) {
                    g_cat[j] = saved[j];
                    g_depth[j] = sd[j];
                    g_writhe[j] = sw[j];
                }
                g_cat_size = saved_sz;
            }
        }
    }

    /* ============================================================
     * Phase 4: Radical Analysis
     *
     * L_{4,4} is the 1-dim simple at top of Loewy filtration.
     * Radical direction r = (1, 0, -1): unique vector killed by
     * all TL generators e_i on W_{4,2}.
     *
     * Step 1: Verify r
     * Step 2: Per-entry radical content, rMr, Casimir
     * Step 3-4: XOR14 winner vs control (radical + Casimir)
     * Step 5: Casimir by XOR level
     * ============================================================ */
    printf("\n=== Phase 4: Radical Analysis ===\n\n");

    {
        static long rad_content[MAX_CAT];
        static long cas3[MAX_CAT];
        static long rMr_l1_arr[MAX_CAT];

        /* Step 1: Verify radical direction r = (1, 0, -1) */
        printf("  Step 1: Radical direction verification\n");
        printf("  r = (1, 0, -1)\n");
        {
            Cyc8 r_vec[3];
            int ei;
            r_vec[0] = cyc8_one();
            r_vec[1] = cyc8_zero();
            r_vec[2] = cyc8_make(-1, 0, 0, 0);

            for (ei = 0; ei < 3; ei++) {
                Cyc8 result[3];
                int ri, ok = 1;
                for (ri = 0; ri < 3; ri++) {
                    int ci;
                    result[ri] = cyc8_zero();
                    for (ci = 0; ci < 3; ci++)
                        result[ri] = cyc8_add(result[ri],
                            cyc8_mul(g_e[ei].m[ri][ci], r_vec[ci]));
                    if (!cyc8_is_zero(result[ri])) ok = 0;
                }
                sprintf(msg, "e_%d * r = 0", ei + 1);
                check(msg, ok);
            }
        }

        /* Step 2: Per-entry radical content and Casimir */
        printf("\n  Step 2: Per-entry radical content and Casimir\n");
        {
            long sum_rad = 0, sum_rMr = 0, sum_cas = 0;

            for (i = 0; i < g_cat_size; i++) {
                long rc = 0;
                int ri;
                Cyc8 rmr, tr_m, tr_m2, three_val, c3_val;
                long l1;
                int j2, k2;

                /* radical_content = ||M * r|| where r = (1,0,-1) */
                /* M*r = col0 - col2, then sum L1 of each row */
                for (ri = 0; ri < 3; ri++) {
                    Cyc8 v = cyc8_sub(g_cat[i].m[ri][0], g_cat[i].m[ri][2]);
                    long va = v.a < 0 ? -v.a : v.a;
                    long vb = v.b < 0 ? -v.b : v.b;
                    long vc = v.c < 0 ? -v.c : v.c;
                    long vd = v.d < 0 ? -v.d : v.d;
                    rc += va + vb + vc + vd;
                }
                rad_content[i] = rc;

                /* rMr = r^T M r = M[0][0] - M[0][2] - M[2][0] + M[2][2] */
                rmr = cyc8_sub(
                    cyc8_sub(g_cat[i].m[0][0], g_cat[i].m[0][2]),
                    cyc8_sub(g_cat[i].m[2][0], g_cat[i].m[2][2]));
                l1 = (rmr.a < 0 ? -rmr.a : rmr.a)
                   + (rmr.b < 0 ? -rmr.b : rmr.b)
                   + (rmr.c < 0 ? -rmr.c : rmr.c)
                   + (rmr.d < 0 ? -rmr.d : rmr.d);
                rMr_l1_arr[i] = l1;

                /* Casimir: C3(M) = 3*tr(M^2) - (tr M)^2 */
                tr_m = cyc8_add(
                    cyc8_add(g_cat[i].m[0][0], g_cat[i].m[1][1]),
                    g_cat[i].m[2][2]);
                tr_m2 = cyc8_zero();
                for (j2 = 0; j2 < 3; j2++)
                    for (k2 = 0; k2 < 3; k2++)
                        tr_m2 = cyc8_add(tr_m2,
                            cyc8_mul(g_cat[i].m[j2][k2],
                                     g_cat[i].m[k2][j2]));
                three_val = cyc8_make(3, 0, 0, 0);
                c3_val = cyc8_sub(
                    cyc8_mul(three_val, tr_m2),
                    cyc8_mul(tr_m, tr_m));
                l1 = (c3_val.a < 0 ? -c3_val.a : c3_val.a)
                   + (c3_val.b < 0 ? -c3_val.b : c3_val.b)
                   + (c3_val.c < 0 ? -c3_val.c : c3_val.c)
                   + (c3_val.d < 0 ? -c3_val.d : c3_val.d);
                cas3[i] = l1;

                sum_rad += rad_content[i];
                sum_rMr += rMr_l1_arr[i];
                sum_cas += cas3[i];
            }

            printf("  Catalog: %d entries\n", g_cat_size);
            printf("  Mean radical_content: %ld\n",
                   sum_rad / (long)g_cat_size);
            printf("  Mean |rMr|: %ld\n",
                   sum_rMr / (long)g_cat_size);
            printf("  Mean |C3|: %ld\n",
                   sum_cas / (long)g_cat_size);

            /* By depth */
            printf("\n  %-6s  %12s  %12s  %12s  %6s\n",
                   "depth", "mean_rad", "mean_rMr", "mean_C3", "count");
            {
                int d;
                for (d = 0; d <= 8; d++) {
                    long sr = 0, srm = 0, sc = 0;
                    int cnt = 0;
                    int ii;
                    for (ii = 0; ii < g_cat_size; ii++) {
                        if (g_depth[ii] == d) {
                            sr += rad_content[ii];
                            srm += rMr_l1_arr[ii];
                            sc += cas3[ii];
                            cnt++;
                        }
                    }
                    if (cnt > 0)
                        printf("  %-6d  %12ld  %12ld  %12ld  %6d\n",
                               d, sr / (long)cnt, srm / (long)cnt,
                               sc / (long)cnt, cnt);
                }
            }

            check("Per-entry radical and Casimir computed", 1);
        }

        /* Steps 3-5: Deep sub-catalog analysis */
        printf("\n  Steps 3-5: Deep sub-catalog (d>=4, bf=30)\n");
        {
            int deep_idx[1024];
            int nd = 0;
            Mat3 saved_cat[40];
            int saved_d[40], saved_w[40];
            long saved_rad[40], saved_cas[40], saved_rMr[40];
            int saved_sz;
            int bf, step_sz, j;

            for (i = 0; i < g_cat_size && nd < 1024; i++)
                if (g_depth[i] >= 4)
                    deep_idx[nd++] = i;

            printf("  Deep entries (d>=4): %d\n", nd);
            saved_sz = g_cat_size;
            bf = nd < 30 ? nd : 30;

            if (bf < 7) {
                printf("  Not enough deep entries\n");
            } else {
                step_sz = nd / bf;
                if (step_sz < 1) step_sz = 1;

                /* Save first bf entries */
                for (j = 0; j < bf; j++) {
                    saved_cat[j] = g_cat[j];
                    saved_d[j] = g_depth[j];
                    saved_w[j] = g_writhe[j];
                    saved_rad[j] = rad_content[j];
                    saved_cas[j] = cas3[j];
                    saved_rMr[j] = rMr_l1_arr[j];
                }

                /* Replace with deep entries */
                for (j = 0; j < bf; j++) {
                    int src = deep_idx[j * step_sz];
                    g_cat[j] = g_cat[src];
                    g_depth[j] = g_depth[src];
                    g_writhe[j] = g_writhe[src];
                    rad_content[j] = rad_content[src];
                    cas3[j] = cas3[src];
                    rMr_l1_arr[j] = rMr_l1_arr[src];
                }
                g_cat_size = bf;

                /* Step 3-4: XOR14 winner vs control */
                printf("\n  Step 3-4: XOR14 winner vs control\n");
                {
                    int n_win = 0, n_ctrl = 0;
                    long win_rad = 0, ctrl_rad = 0;
                    long win_cas = 0, ctrl_cas = 0;
                    long win_rMr = 0, ctrl_rMr = 0;
                    int idx[8];
                    int a0, a1, a2, a3, a4, a5, a6;
                    int ctrl_lim = 1000;

                    for (a0 = 0; a0 < bf; a0++)
                    for (a1 = a0+1; a1 < bf; a1++)
                    for (a2 = a1+1; a2 < bf; a2++)
                    for (a3 = a2+1; a3 < bf; a3++)
                    for (a4 = a3+1; a4 < bf; a4++)
                    for (a5 = a4+1; a5 < bf; a5++)
                    for (a6 = a5+1; a6 < bf; a6++) {
                        long tr, tc, trm;
                        idx[0]=a0; idx[1]=a1; idx[2]=a2;
                        idx[3]=a3; idx[4]=a4; idx[5]=a5;
                        idx[6]=a6;

                        tr = rad_content[a0] + rad_content[a1]
                           + rad_content[a2] + rad_content[a3]
                           + rad_content[a4] + rad_content[a5]
                           + rad_content[a6];
                        tc = cas3[a0] + cas3[a1] + cas3[a2]
                           + cas3[a3] + cas3[a4] + cas3[a5]
                           + cas3[a6];
                        trm = rMr_l1_arr[a0] + rMr_l1_arr[a1]
                            + rMr_l1_arr[a2] + rMr_l1_arr[a3]
                            + rMr_l1_arr[a4] + rMr_l1_arr[a5]
                            + rMr_l1_arr[a6];

                        if (test_xor_mat3(idx, 7, 128)) {
                            n_win++;
                            win_rad += tr;
                            win_cas += tc;
                            win_rMr += trm;
                        } else if (n_ctrl < ctrl_lim) {
                            n_ctrl++;
                            ctrl_rad += tr;
                            ctrl_cas += tc;
                            ctrl_rMr += trm;
                        }
                    }

                    printf("  Winners: %d  Control: %d\n",
                           n_win, n_ctrl);

                    if (n_win > 0 && n_ctrl > 0) {
                        printf("\n  %-12s  %12s  %12s\n",
                               "", "Winners", "Control");
                        printf("  %-12s  %12ld  %12ld\n",
                               "mean_rad",
                               win_rad / (long)n_win,
                               ctrl_rad / (long)n_ctrl);
                        printf("  %-12s  %12ld  %12ld\n",
                               "mean_rMr",
                               win_rMr / (long)n_win,
                               ctrl_rMr / (long)n_ctrl);
                        printf("  %-12s  %12ld  %12ld\n",
                               "mean_C3",
                               win_cas / (long)n_win,
                               ctrl_cas / (long)n_ctrl);
                    }

                    sprintf(msg, "XOR14 winners: %d", n_win);
                    check(msg, n_win > 0);
                }

                /* Step 5: Casimir by XOR level */
                printf("\n  Step 5: Casimir by XOR level\n");
                printf("  %-8s  %8s  %12s  %12s  %8s\n",
                       "XOR", "winners", "win_C3", "ctrl_C3", "ratio");

                /* XOR6 (nw=3) */
                {
                    int a0, a1, a2;
                    int nw2 = 0, nc2 = 0;
                    long wc = 0, cc = 0;
                    for (a0 = 0; a0 < bf; a0++)
                    for (a1 = a0+1; a1 < bf; a1++)
                    for (a2 = a1+1; a2 < bf; a2++) {
                        int idx[8];
                        long tc;
                        idx[0]=a0; idx[1]=a1; idx[2]=a2;
                        tc = cas3[a0]+cas3[a1]+cas3[a2];
                        if (test_xor_mat3(idx, 3, 128)) {
                            nw2++; wc += tc;
                        } else if (nc2 < 500) {
                            nc2++; cc += tc;
                        }
                    }
                    if (nw2 > 0 && nc2 > 0)
                        printf("  XOR6     %8d  %12ld  %12ld  %7.2fx\n",
                               nw2, wc/(long)nw2, cc/(long)nc2,
                               cc > 0 ? (double)(wc/(long)nw2)
                                       / (double)(cc/(long)nc2) : 0.0);
                    else
                        printf("  XOR6     %8d       N/A          N/A       N/A\n",
                               nw2);
                }

                /* XOR8 (nw=4) */
                {
                    int a0, a1, a2, a3;
                    int nw2 = 0, nc2 = 0;
                    long wc = 0, cc = 0;
                    for (a0 = 0; a0 < bf; a0++)
                    for (a1 = a0+1; a1 < bf; a1++)
                    for (a2 = a1+1; a2 < bf; a2++)
                    for (a3 = a2+1; a3 < bf; a3++) {
                        int idx[8];
                        long tc;
                        idx[0]=a0; idx[1]=a1; idx[2]=a2; idx[3]=a3;
                        tc = cas3[a0]+cas3[a1]+cas3[a2]+cas3[a3];
                        if (test_xor_mat3(idx, 4, 128)) {
                            nw2++; wc += tc;
                        } else if (nc2 < 500) {
                            nc2++; cc += tc;
                        }
                    }
                    if (nw2 > 0 && nc2 > 0)
                        printf("  XOR8     %8d  %12ld  %12ld  %7.2fx\n",
                               nw2, wc/(long)nw2, cc/(long)nc2,
                               cc > 0 ? (double)(wc/(long)nw2)
                                       / (double)(cc/(long)nc2) : 0.0);
                    else
                        printf("  XOR8     %8d       N/A          N/A       N/A\n",
                               nw2);
                }

                /* XOR10 (nw=5) */
                {
                    int a0, a1, a2, a3, a4;
                    int nw2 = 0, nc2 = 0;
                    long wc = 0, cc = 0;
                    for (a0 = 0; a0 < bf; a0++)
                    for (a1 = a0+1; a1 < bf; a1++)
                    for (a2 = a1+1; a2 < bf; a2++)
                    for (a3 = a2+1; a3 < bf; a3++)
                    for (a4 = a3+1; a4 < bf; a4++) {
                        int idx[8];
                        long tc;
                        idx[0]=a0; idx[1]=a1; idx[2]=a2;
                        idx[3]=a3; idx[4]=a4;
                        tc = cas3[a0]+cas3[a1]+cas3[a2]
                           + cas3[a3]+cas3[a4];
                        if (test_xor_mat3(idx, 5, 128)) {
                            nw2++; wc += tc;
                        } else if (nc2 < 500) {
                            nc2++; cc += tc;
                        }
                    }
                    if (nw2 > 0 && nc2 > 0)
                        printf("  XOR10    %8d  %12ld  %12ld  %7.2fx\n",
                               nw2, wc/(long)nw2, cc/(long)nc2,
                               cc > 0 ? (double)(wc/(long)nw2)
                                       / (double)(cc/(long)nc2) : 0.0);
                    else
                        printf("  XOR10    %8d       N/A          N/A       N/A\n",
                               nw2);
                }

                /* XOR12 (nw=6) */
                {
                    int a0, a1, a2, a3, a4, a5;
                    int nw2 = 0, nc2 = 0;
                    long wc = 0, cc = 0;
                    for (a0 = 0; a0 < bf; a0++)
                    for (a1 = a0+1; a1 < bf; a1++)
                    for (a2 = a1+1; a2 < bf; a2++)
                    for (a3 = a2+1; a3 < bf; a3++)
                    for (a4 = a3+1; a4 < bf; a4++)
                    for (a5 = a4+1; a5 < bf; a5++) {
                        int idx[8];
                        long tc;
                        idx[0]=a0; idx[1]=a1; idx[2]=a2;
                        idx[3]=a3; idx[4]=a4; idx[5]=a5;
                        tc = cas3[a0]+cas3[a1]+cas3[a2]
                           + cas3[a3]+cas3[a4]+cas3[a5];
                        if (test_xor_mat3(idx, 6, 128)) {
                            nw2++; wc += tc;
                        } else if (nc2 < 500) {
                            nc2++; cc += tc;
                        }
                    }
                    if (nw2 > 0 && nc2 > 0)
                        printf("  XOR12    %8d  %12ld  %12ld  %7.2fx\n",
                               nw2, wc/(long)nw2, cc/(long)nc2,
                               cc > 0 ? (double)(wc/(long)nw2)
                                       / (double)(cc/(long)nc2) : 0.0);
                    else
                        printf("  XOR12    %8d       N/A          N/A       N/A\n",
                               nw2);
                }

                check("Casimir-by-XOR-level sweep completed", 1);

                /* ============================================
                 * Phase 5: Winner Anatomy
                 * ============================================ */
                printf("\n=== Phase 5: Winner Anatomy ===\n\n");

                /* Step 1: Entry frequency + co-occurrence */
                printf("  Step 1: Entry frequency in XOR14 winners\n");
                {
                    static int entry_freq[40];
                    static int cooccur[40][40];
                    int n_win5 = 0;
                    int hub_idx[10];
                    int n_hubs = 0;
                    int a0, a1, a2, a3, a4, a5, a6;
                    int idx5[8];
                    int pp, qq;

                    memset(entry_freq, 0, (size_t)bf * sizeof(int));
                    memset(cooccur, 0, sizeof(cooccur));

                    for (a0 = 0; a0 < bf; a0++)
                    for (a1 = a0+1; a1 < bf; a1++)
                    for (a2 = a1+1; a2 < bf; a2++)
                    for (a3 = a2+1; a3 < bf; a3++)
                    for (a4 = a3+1; a4 < bf; a4++)
                    for (a5 = a4+1; a5 < bf; a5++)
                    for (a6 = a5+1; a6 < bf; a6++) {
                        idx5[0]=a0; idx5[1]=a1; idx5[2]=a2;
                        idx5[3]=a3; idx5[4]=a4; idx5[5]=a5;
                        idx5[6]=a6;
                        if (test_xor_mat3(idx5, 7, 128)) {
                            n_win5++;
                            entry_freq[a0]++;
                            entry_freq[a1]++;
                            entry_freq[a2]++;
                            entry_freq[a3]++;
                            entry_freq[a4]++;
                            entry_freq[a5]++;
                            entry_freq[a6]++;
                            for (pp = 0; pp < 7; pp++)
                                for (qq = pp+1; qq < 7; qq++)
                                    cooccur[idx5[pp]][idx5[qq]]++;
                        }
                    }

                    printf("  Total XOR14 winners: %d\n", n_win5);

                    /* Frequency histogram */
                    printf("\n  Frequency histogram:\n");
                    {
                        int freq_hist[80];
                        int max_freq = 0;
                        int fi;
                        memset(freq_hist, 0, sizeof(freq_hist));
                        for (fi = 0; fi < bf; fi++) {
                            if (entry_freq[fi] > max_freq)
                                max_freq = entry_freq[fi];
                            if (entry_freq[fi] < 80)
                                freq_hist[entry_freq[fi]]++;
                        }
                        printf("  %-6s  %6s\n", "freq", "entries");
                        for (fi = 0; fi <= max_freq && fi < 80; fi++) {
                            if (freq_hist[fi] > 0)
                                printf("  %-6d  %6d\n",
                                       fi, freq_hist[fi]);
                        }
                    }

                    /* Identify super-hubs (>50% of winners) */
                    {
                        int threshold = n_win5 / 2;
                        int fi;
                        printf("\n  Super-hubs (freq > %d):\n",
                               threshold);
                        for (fi = 0; fi < bf && n_hubs < 10; fi++) {
                            if (entry_freq[fi] > threshold) {
                                hub_idx[n_hubs] = fi;
                                printf("    Hub %d: entry %d, "
                                       "freq=%d (%.1f%%), d=%d, w=%d\n",
                                       n_hubs, fi, entry_freq[fi],
                                       100.0 * (double)entry_freq[fi]
                                             / (double)n_win5,
                                       g_depth[fi], g_writhe[fi]);
                                n_hubs++;
                            }
                        }
                    }

                    sprintf(msg, "Super-hubs found: %d", n_hubs);
                    check(msg, n_hubs > 0);

                    /* Step 2: Super-hub properties */
                    if (n_hubs >= 1) {
                        int hi;
                        printf("\n  Step 2: Super-hub properties\n");
                        for (hi = 0; hi < n_hubs; hi++) {
                            int hidx = hub_idx[hi];
                            Cyc8 tr_h, tr_h2, c3_h;
                            long c3_l1, frob;
                            int j3, k3;
                            const long *fp;

                            printf("\n    Hub %d (entry %d, "
                                   "d=%d, w=%d):\n",
                                   hi, hidx,
                                   g_depth[hidx], g_writhe[hidx]);
                            mat3_print("matrix", &g_cat[hidx]);

                            /* Trace */
                            tr_h = cyc8_add(
                                cyc8_add(g_cat[hidx].m[0][0],
                                         g_cat[hidx].m[1][1]),
                                g_cat[hidx].m[2][2]);
                            printf("    trace = (%ld,%ld,%ld,%ld)\n",
                                   tr_h.a, tr_h.b,
                                   tr_h.c, tr_h.d);

                            /* C3 */
                            tr_h2 = cyc8_zero();
                            for (j3 = 0; j3 < 3; j3++)
                                for (k3 = 0; k3 < 3; k3++)
                                    tr_h2 = cyc8_add(tr_h2,
                                        cyc8_mul(g_cat[hidx].m[j3][k3],
                                                 g_cat[hidx].m[k3][j3]));
                            c3_h = cyc8_sub(
                                cyc8_mul(cyc8_make(3,0,0,0), tr_h2),
                                cyc8_mul(tr_h, tr_h));
                            c3_l1 = (c3_h.a<0?-c3_h.a:c3_h.a)
                                  + (c3_h.b<0?-c3_h.b:c3_h.b)
                                  + (c3_h.c<0?-c3_h.c:c3_h.c)
                                  + (c3_h.d<0?-c3_h.d:c3_h.d);
                            printf("    C3 = (%ld,%ld,%ld,%ld), "
                                   "|C3|_L1=%ld\n",
                                   c3_h.a, c3_h.b,
                                   c3_h.c, c3_h.d, c3_l1);

                            /* Radical content */
                            printf("    radical_content = %ld\n",
                                   rad_content[hidx]);

                            /* Frobenius L1 norm */
                            frob = 0;
                            fp = &g_cat[hidx].m[0][0].a;
                            for (j3 = 0; j3 < 36; j3++) {
                                long v = fp[j3];
                                frob += v < 0 ? -v : v;
                            }
                            printf("    Frobenius L1 = %ld\n", frob);
                        }

                        /* Compare hub traces if 2+ hubs */
                        if (n_hubs >= 2) {
                            Cyc8 tr_a = cyc8_add(
                                cyc8_add(g_cat[hub_idx[0]].m[0][0],
                                         g_cat[hub_idx[0]].m[1][1]),
                                g_cat[hub_idx[0]].m[2][2]);
                            Cyc8 tr_b = cyc8_add(
                                cyc8_add(g_cat[hub_idx[1]].m[0][0],
                                         g_cat[hub_idx[1]].m[1][1]),
                                g_cat[hub_idx[1]].m[2][2]);
                            Cyc8 tr_sum = cyc8_add(tr_a, tr_b);
                            printf("\n    Hub comparison:\n");
                            printf("    tr(h0)+tr(h1) = "
                                   "(%ld,%ld,%ld,%ld)\n",
                                   tr_sum.a, tr_sum.b,
                                   tr_sum.c, tr_sum.d);
                            printf("    writhe: h0=%d, h1=%d\n",
                                   g_writhe[hub_idx[0]],
                                   g_writhe[hub_idx[1]]);
                        }

                        check("Super-hub properties reported", 1);
                    }

                    /* Step 3: Hub commutator */
                    if (n_hubs >= 2) {
                        Mat3 ab, ba, neg_ba, comm;
                        Cyc8 tr_c, tr_c2, c3_c;
                        long c3_l1, frob_l1;
                        int j3, k3;
                        const long *fp2;

                        printf("\n  Step 3: Hub commutator "
                               "[hub_0, hub_1]\n");

                        ab = mat3_mul(&g_cat[hub_idx[0]],
                                      &g_cat[hub_idx[1]]);
                        ba = mat3_mul(&g_cat[hub_idx[1]],
                                      &g_cat[hub_idx[0]]);
                        neg_ba = mat3_neg(&ba);
                        comm = mat3_add(&ab, &neg_ba);

                        mat3_print("commutator", &comm);

                        /* Trace */
                        tr_c = cyc8_add(
                            cyc8_add(comm.m[0][0], comm.m[1][1]),
                            comm.m[2][2]);
                        printf("    trace = (%ld,%ld,%ld,%ld)\n",
                               tr_c.a, tr_c.b, tr_c.c, tr_c.d);
                        check("Commutator traceless",
                              cyc8_is_zero(tr_c));

                        /* C3 of commutator */
                        tr_c2 = cyc8_zero();
                        for (j3 = 0; j3 < 3; j3++)
                            for (k3 = 0; k3 < 3; k3++)
                                tr_c2 = cyc8_add(tr_c2,
                                    cyc8_mul(comm.m[j3][k3],
                                             comm.m[k3][j3]));
                        c3_c = cyc8_sub(
                            cyc8_mul(cyc8_make(3,0,0,0), tr_c2),
                            cyc8_mul(tr_c, tr_c));
                        c3_l1 = (c3_c.a<0?-c3_c.a:c3_c.a)
                              + (c3_c.b<0?-c3_c.b:c3_c.b)
                              + (c3_c.c<0?-c3_c.c:c3_c.c)
                              + (c3_c.d<0?-c3_c.d:c3_c.d);
                        printf("    C3 = (%ld,%ld,%ld,%ld), "
                               "|C3|_L1=%ld\n",
                               c3_c.a, c3_c.b,
                               c3_c.c, c3_c.d, c3_l1);

                        /* Frobenius L1 */
                        frob_l1 = 0;
                        fp2 = &comm.m[0][0].a;
                        for (j3 = 0; j3 < 36; j3++) {
                            long v = fp2[j3];
                            frob_l1 += v < 0 ? -v : v;
                        }
                        printf("    Frobenius L1 = %ld\n", frob_l1);

                        /* Radical content of commutator */
                        {
                            long comm_rad = 0;
                            int cr;
                            for (cr = 0; cr < 3; cr++) {
                                Cyc8 v = cyc8_sub(comm.m[cr][0],
                                                   comm.m[cr][2]);
                                long va = v.a<0 ? -v.a : v.a;
                                long vb = v.b<0 ? -v.b : v.b;
                                long vc = v.c<0 ? -v.c : v.c;
                                long vd = v.d<0 ? -v.d : v.d;
                                comm_rad += va + vb + vc + vd;
                            }
                            printf("    radical_content = %ld\n",
                                   comm_rad);
                        }

                        check("Hub commutator computed", 1);
                    }

                    /* Step 3b: All pair + cross-pair commutators */
                    if (n_hubs >= 6) {
                        /* Within-pair: [h2,h5], [h3,h4]
                         * Cross-pair: [h0,h3], [h0,h2], [h3,h2]
                         */
                        static const int comm_pairs[][2] = {
                            {2, 5}, {3, 4},
                            {0, 3}, {0, 2}, {3, 2}
                        };
                        static const char *comm_names[] = {
                            "[h2,h5] zeta8^3 pair",
                            "[h3,h4] zeta8 pair",
                            "[h0,h3] top x mid",
                            "[h0,h2] top x low",
                            "[h3,h2] mid x low"
                        };
                        int ci;

                        printf("\n  Step 3b: All pair and "
                               "cross-pair commutators\n");
                        printf("  %-24s  %20s  %8s  %8s  %8s\n",
                               "pair", "trace", "|C3|",
                               "rad", "Frob");

                        for (ci = 0; ci < 5; ci++) {
                            int ha = hub_idx[comm_pairs[ci][0]];
                            int hb = hub_idx[comm_pairs[ci][1]];
                            Mat3 ab2, ba2, neg_ba2, comm2;
                            Cyc8 tr2, tr2_sq, c3_2;
                            long c3_l1_2, frob2, comm_rad2;
                            int j4, k4;
                            const long *fp3;

                            ab2 = mat3_mul(&g_cat[ha], &g_cat[hb]);
                            ba2 = mat3_mul(&g_cat[hb], &g_cat[ha]);
                            neg_ba2 = mat3_neg(&ba2);
                            comm2 = mat3_add(&ab2, &neg_ba2);

                            /* Trace */
                            tr2 = cyc8_add(
                                cyc8_add(comm2.m[0][0],
                                         comm2.m[1][1]),
                                comm2.m[2][2]);

                            /* C3 */
                            tr2_sq = cyc8_zero();
                            for (j4 = 0; j4 < 3; j4++)
                                for (k4 = 0; k4 < 3; k4++)
                                    tr2_sq = cyc8_add(tr2_sq,
                                        cyc8_mul(comm2.m[j4][k4],
                                                 comm2.m[k4][j4]));
                            c3_2 = cyc8_sub(
                                cyc8_mul(cyc8_make(3,0,0,0),
                                         tr2_sq),
                                cyc8_mul(tr2, tr2));
                            c3_l1_2 =
                                (c3_2.a<0?-c3_2.a:c3_2.a)
                              + (c3_2.b<0?-c3_2.b:c3_2.b)
                              + (c3_2.c<0?-c3_2.c:c3_2.c)
                              + (c3_2.d<0?-c3_2.d:c3_2.d);

                            /* Frobenius L1 */
                            frob2 = 0;
                            fp3 = &comm2.m[0][0].a;
                            for (j4 = 0; j4 < 36; j4++) {
                                long v = fp3[j4];
                                frob2 += v < 0 ? -v : v;
                            }

                            /* Radical content */
                            comm_rad2 = 0;
                            for (j4 = 0; j4 < 3; j4++) {
                                Cyc8 rv = cyc8_sub(
                                    comm2.m[j4][0],
                                    comm2.m[j4][2]);
                                long va = rv.a<0?-rv.a:rv.a;
                                long vb = rv.b<0?-rv.b:rv.b;
                                long vc = rv.c<0?-rv.c:rv.c;
                                long vd = rv.d<0?-rv.d:rv.d;
                                comm_rad2 += va+vb+vc+vd;
                            }

                            printf("  %-24s  (%3ld,%3ld,%3ld,%3ld)"
                                   "  %8ld  %8ld  %8ld\n",
                                   comm_names[ci],
                                   tr2.a, tr2.b, tr2.c, tr2.d,
                                   c3_l1_2, comm_rad2, frob2);
                        }

                        /* Also print the original [h0,h1] for
                         * comparison in same format */
                        {
                            Mat3 ab3, ba3, neg3, cm3;
                            Cyc8 t3, tsq3, c33;
                            long cl3, fr3, cr3;
                            int j5, k5;
                            const long *fp4;

                            ab3 = mat3_mul(&g_cat[hub_idx[0]],
                                           &g_cat[hub_idx[1]]);
                            ba3 = mat3_mul(&g_cat[hub_idx[1]],
                                           &g_cat[hub_idx[0]]);
                            neg3 = mat3_neg(&ba3);
                            cm3 = mat3_add(&ab3, &neg3);

                            t3 = cyc8_add(
                                cyc8_add(cm3.m[0][0], cm3.m[1][1]),
                                cm3.m[2][2]);
                            tsq3 = cyc8_zero();
                            for (j5=0;j5<3;j5++)
                                for (k5=0;k5<3;k5++)
                                    tsq3 = cyc8_add(tsq3,
                                        cyc8_mul(cm3.m[j5][k5],
                                                 cm3.m[k5][j5]));
                            c33 = cyc8_sub(
                                cyc8_mul(cyc8_make(3,0,0,0), tsq3),
                                cyc8_mul(t3, t3));
                            cl3 = (c33.a<0?-c33.a:c33.a)
                                + (c33.b<0?-c33.b:c33.b)
                                + (c33.c<0?-c33.c:c33.c)
                                + (c33.d<0?-c33.d:c33.d);
                            fr3 = 0;
                            fp4 = &cm3.m[0][0].a;
                            for (j5=0;j5<36;j5++) {
                                long v = fp4[j5];
                                fr3 += v < 0 ? -v : v;
                            }
                            cr3 = 0;
                            for (j5=0;j5<3;j5++) {
                                Cyc8 rv = cyc8_sub(
                                    cm3.m[j5][0], cm3.m[j5][2]);
                                long va = rv.a<0?-rv.a:rv.a;
                                long vb = rv.b<0?-rv.b:rv.b;
                                long vc = rv.c<0?-rv.c:rv.c;
                                long vd = rv.d<0?-rv.d:rv.d;
                                cr3 += va+vb+vc+vd;
                            }
                            printf("  %-24s  (%3ld,%3ld,%3ld,%3ld)"
                                   "  %8ld  %8ld  %8ld\n",
                                   "[h0,h1] zeta8^2 pair",
                                   t3.a, t3.b, t3.c, t3.d,
                                   cl3, cr3, fr3);
                        }

                        check("All commutators computed", 1);
                    }

                    /* Step 4: Topology */
                    printf("\n  Step 4: Topology "
                           "(co-occurrence graph)\n");
                    {
                        int max_co = 0;
                        int n_edges = 0;
                        int n_hub_edges = 0;
                        int n_sat_edges = 0;
                        int degree[40];
                        int fi, fj;

                        memset(degree, 0, sizeof(degree));

                        for (fi = 0; fi < bf; fi++)
                            for (fj = fi+1; fj < bf; fj++) {
                                if (cooccur[fi][fj] > max_co)
                                    max_co = cooccur[fi][fj];
                                if (cooccur[fi][fj] > 0) {
                                    n_edges++;
                                    degree[fi]++;
                                    degree[fj]++;
                                }
                            }

                        printf("  Total edges: %d\n", n_edges);
                        printf("  Max co-occurrence: %d\n", max_co);

                        /* Print nodes with freq > 0 */
                        printf("\n  %-6s  %6s  %6s  %6s  %8s\n",
                               "entry", "freq", "degree", "depth",
                               "C3");
                        {
                            int fi2;
                            for (fi2 = 0; fi2 < bf; fi2++) {
                                if (entry_freq[fi2] > 0)
                                    printf("  %-6d  %6d  %6d  %6d"
                                           "  %8ld\n",
                                           fi2, entry_freq[fi2],
                                           degree[fi2], g_depth[fi2],
                                           cas3[fi2]);
                            }
                        }

                        /* Star-graph check */
                        if (n_hubs >= 2) {
                            for (fi = 0; fi < bf; fi++) {
                                int fi_hub = 0;
                                int hi;
                                for (hi = 0; hi < n_hubs; hi++)
                                    if (fi == hub_idx[hi])
                                        fi_hub = 1;
                                for (fj = fi+1; fj < bf; fj++) {
                                    int fj_hub = 0;
                                    for (hi = 0; hi < n_hubs; hi++)
                                        if (fj == hub_idx[hi])
                                            fj_hub = 1;
                                    if (cooccur[fi][fj] > 0) {
                                        if (fi_hub || fj_hub)
                                            n_hub_edges++;
                                        else
                                            n_sat_edges++;
                                    }
                                }
                            }

                            printf("\n  Hub-involved edges: %d\n",
                                   n_hub_edges);
                            printf("  Satellite-only edges: %d\n",
                                   n_sat_edges);
                            {
                                int is_star = (n_sat_edges == 0);
                                sprintf(msg, "Star topology: %s",
                                        is_star ? "YES" : "NO");
                                check(msg, 1);
                            }
                        }

                        check("Topology analysis completed", 1);
                    }
                }

                /* Restore catalog */
                for (j = 0; j < bf; j++) {
                    g_cat[j] = saved_cat[j];
                    g_depth[j] = saved_d[j];
                    g_writhe[j] = saved_w[j];
                    rad_content[j] = saved_rad[j];
                    cas3[j] = saved_cas[j];
                    rMr_l1_arr[j] = saved_rMr[j];
                }
                g_cat_size = saved_sz;
            }
        }
    }

    /* ============================================================
     * Summary
     * ============================================================ */
    printf("\n============================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
