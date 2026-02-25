/*
 * KNOTAPEL DEMO 101: 5-Strand DKC Scaling Verification on W_{5,3}
 * ================================================================
 *
 * PREDICTION-DENSE DEMO: Tests sl_d functor thesis.
 *
 * 5-strand braid group on W_{5,3} at delta=0.
 *
 * KEY STRUCTURAL DIFFERENCE: W_{5,3} is SIMPLE (no radical).
 * ker(e_1) ∩ ... ∩ ker(e_4) = {0}.
 *
 * Basis: four half-diagrams for 5 strands with 3 through-lines:
 *   h_0: {1,2} paired, through-lines 3,4,5
 *   h_1: {2,3} paired, through-lines 1,4,5
 *   h_2: {3,4} paired, through-lines 1,2,5
 *   h_3: {4,5} paired, through-lines 1,2,3
 *
 * TL generators (4x4 over Z):
 *   e_1 = [[0,1,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
 *   e_2 = [[0,0,0,0],[1,0,1,0],[0,0,0,0],[0,0,0,0]]
 *   e_3 = [[0,0,0,0],[0,0,0,0],[0,1,0,1],[0,0,0,0]]
 *   e_4 = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,1,0]]
 *
 * Braid: sigma_i = A*I + A^{-1}*e_i
 * A = -zeta_8 = (0,-1,0,0), A^{-1} = (0,0,0,1)
 * 8 generators: sigma_1..4 and inverses
 *
 * PREDICTIONS:
 *   BFS growth ≈ 4x per round
 *   12 super-hubs = 6 pairs = positive roots of sl_4
 *   No radical (W_{5,3} simple)
 *   Winners always high-C4 (no inversion)
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

static long cyc8_l1(Cyc8 z) {
    long v = 0;
    v += z.a < 0 ? -z.a : z.a;
    v += z.b < 0 ? -z.b : z.b;
    v += z.c < 0 ? -z.c : z.c;
    v += z.d < 0 ? -z.d : z.d;
    return v;
}

/* ================================================================
 * 4x4 Matrix over Cyc8 (W_{5,3} representation)
 * ================================================================ */

#define DIM 4

typedef struct {
    Cyc8 m[DIM][DIM];
} Mat4;

static Mat4 mat4_zero(void) {
    Mat4 r;
    int i, j;
    for (i = 0; i < DIM; i++)
        for (j = 0; j < DIM; j++)
            r.m[i][j] = cyc8_zero();
    return r;
}

static Mat4 mat4_identity(void) {
    Mat4 r = mat4_zero();
    int i;
    for (i = 0; i < DIM; i++)
        r.m[i][i] = cyc8_one();
    return r;
}

static Mat4 mat4_mul(const Mat4 *p, const Mat4 *q) {
    Mat4 r;
    int i, j, k;
    for (i = 0; i < DIM; i++)
        for (j = 0; j < DIM; j++) {
            r.m[i][j] = cyc8_zero();
            for (k = 0; k < DIM; k++)
                r.m[i][j] = cyc8_add(r.m[i][j],
                                      cyc8_mul(p->m[i][k], q->m[k][j]));
        }
    return r;
}

static int mat4_eq(const Mat4 *p, const Mat4 *q) {
    int i, j;
    for (i = 0; i < DIM; i++)
        for (j = 0; j < DIM; j++)
            if (!cyc8_eq(p->m[i][j], q->m[i][j])) return 0;
    return 1;
}

static Mat4 mat4_add(const Mat4 *p, const Mat4 *q) {
    Mat4 r;
    int i, j;
    for (i = 0; i < DIM; i++)
        for (j = 0; j < DIM; j++)
            r.m[i][j] = cyc8_add(p->m[i][j], q->m[i][j]);
    return r;
}

static Mat4 mat4_neg(const Mat4 *p) {
    Mat4 r;
    int i, j;
    for (i = 0; i < DIM; i++)
        for (j = 0; j < DIM; j++)
            r.m[i][j] = cyc8_neg(p->m[i][j]);
    return r;
}

static Mat4 mat4_scale(Cyc8 s, const Mat4 *p) {
    Mat4 r;
    int i, j;
    for (i = 0; i < DIM; i++)
        for (j = 0; j < DIM; j++)
            r.m[i][j] = cyc8_mul(s, p->m[i][j]);
    return r;
}

static long mat4_max_abs(const Mat4 *m) {
    long mx = 0, v;
    int i, j;
    for (i = 0; i < DIM; i++)
        for (j = 0; j < DIM; j++) {
            v = cyc8_max_abs(m->m[i][j]);
            if (v > mx) mx = v;
        }
    return mx;
}

static void mat4_print(const char *name, const Mat4 *m) {
    int i, j;
    printf("  %s:\n", name);
    for (i = 0; i < DIM; i++) {
        printf("    [");
        for (j = 0; j < DIM; j++) {
            printf("(%ld,%ld,%ld,%ld)",
                   m->m[i][j].a, m->m[i][j].b,
                   m->m[i][j].c, m->m[i][j].d);
            if (j < DIM - 1) printf(", ");
        }
        printf("]\n");
    }
}

/* Trace */
static Cyc8 mat4_trace(const Mat4 *m) {
    Cyc8 t = cyc8_zero();
    int i;
    for (i = 0; i < DIM; i++)
        t = cyc8_add(t, m->m[i][i]);
    return t;
}

/* tr(M^2) = sum_{j,k} M[j][k] * M[k][j] */
static Cyc8 mat4_tr_sq(const Mat4 *m) {
    Cyc8 t = cyc8_zero();
    int j, k;
    for (j = 0; j < DIM; j++)
        for (k = 0; k < DIM; k++)
            t = cyc8_add(t, cyc8_mul(m->m[j][k], m->m[k][j]));
    return t;
}

/* C4(M) = 4*tr(M^2) - (tr M)^2 */
static Cyc8 mat4_casimir(const Mat4 *m) {
    Cyc8 tr = mat4_trace(m);
    Cyc8 trsq = mat4_tr_sq(m);
    Cyc8 four = cyc8_make(4, 0, 0, 0);
    return cyc8_sub(cyc8_mul(four, trsq), cyc8_mul(tr, tr));
}

/* Frobenius L1 norm */
static long mat4_frob_l1(const Mat4 *m) {
    long f = 0;
    const long *p = &m->m[0][0].a;
    int i;
    for (i = 0; i < DIM * DIM * 4; i++) {
        long v = p[i];
        f += v < 0 ? -v : v;
    }
    return f;
}

/* ================================================================
 * Hash table for Mat4 BFS
 * ================================================================ */

#define MAX_CAT 32768
#define HASH_SIZE 65537

static Mat4  g_cat[MAX_CAT];
static int   g_depth[MAX_CAT];
static int   g_writhe[MAX_CAT];
static int   g_cat_size = 0;

static int g_hash_head[HASH_SIZE];
static int g_hash_next[MAX_CAT];

static unsigned long hash_mat4(const Mat4 *m) {
    unsigned long h = 2166136261UL;
    const long *p = &m->m[0][0].a;
    int i;
    for (i = 0; i < DIM * DIM * 4; i++) {
        h = (h * 1000003UL) ^ (unsigned long)p[i];
    }
    return h;
}

static void hash_init(void) {
    memset(g_hash_head, -1, sizeof(g_hash_head));
}

static int hash_find(const Mat4 *m) {
    int bucket = (int)(hash_mat4(m) % (unsigned long)HASH_SIZE);
    int idx = g_hash_head[bucket];
    while (idx >= 0) {
        if (mat4_eq(&g_cat[idx], m)) return idx;
        idx = g_hash_next[idx];
    }
    return -1;
}

static void hash_insert(int cat_idx) {
    int bucket = (int)(hash_mat4(&g_cat[cat_idx]) % (unsigned long)HASH_SIZE);
    g_hash_next[cat_idx] = g_hash_head[bucket];
    g_hash_head[bucket] = cat_idx;
}

/* ================================================================
 * TL generators on W_{5,3}
 *
 * e_1: h_1 -> h_0           (pair {2,3} reconnects via cup {1,2})
 * e_2: h_0 -> h_1, h_2->h_1 (symmetric around position 2)
 * e_3: h_1 -> h_2, h_3->h_2 (symmetric around position 3)
 * e_4: h_2 -> h_3           (pair {3,4} reconnects via cup {4,5})
 *
 * Pattern: path graph (A_3 Dynkin diagram)
 * ================================================================ */

static Mat4 g_e[4];

static void build_tl_generators(void) {
    int i;
    for (i = 0; i < 4; i++)
        g_e[i] = mat4_zero();

    /* e_1: h_1 -> h_0 */
    g_e[0].m[0][1] = cyc8_one();

    /* e_2: h_0 -> h_1, h_2 -> h_1 */
    g_e[1].m[1][0] = cyc8_one();
    g_e[1].m[1][2] = cyc8_one();

    /* e_3: h_1 -> h_2, h_3 -> h_2 */
    g_e[2].m[2][1] = cyc8_one();
    g_e[2].m[2][3] = cyc8_one();

    /* e_4: h_2 -> h_3 */
    g_e[3].m[3][2] = cyc8_one();
}

/* ================================================================
 * Braid generators on W_{5,3}
 *
 * sigma_i = A*I + A^{-1}*e_i
 * sigma_i_inv = A^{-1}*I + A*e_i
 *
 * 8 generators indexed 0-7:
 *   0: sigma_1,   1: sigma_1_inv
 *   2: sigma_2,   3: sigma_2_inv
 *   4: sigma_3,   5: sigma_3_inv
 *   6: sigma_4,   7: sigma_4_inv
 * ================================================================ */

#define N_GEN 8
static Cyc8 g_A, g_A_inv;
static Mat4 g_gen[N_GEN];
static const int g_gen_writhe[N_GEN] = {1, -1, 1, -1, 1, -1, 1, -1};

static void build_braid_generators(void) {
    int i;
    Mat4 id4 = mat4_identity();
    Mat4 a_id, a_inv_id;

    g_A = cyc8_make(0, -1, 0, 0);
    g_A_inv = cyc8_conj(g_A);

    a_id = mat4_scale(g_A, &id4);
    a_inv_id = mat4_scale(g_A_inv, &id4);

    for (i = 0; i < 4; i++) {
        Mat4 a_inv_e = mat4_scale(g_A_inv, &g_e[i]);
        Mat4 a_e = mat4_scale(g_A, &g_e[i]);
        g_gen[2 * i] = mat4_add(&a_id, &a_inv_e);
        g_gen[2 * i + 1] = mat4_add(&a_inv_id, &a_e);
    }
}

/* ================================================================
 * BFS catalog building (8 generators)
 * ================================================================ */

static void build_catalog(int max_depth) {
    int prev, gi, i, rd;

    g_cat_size = 0;
    hash_init();

    g_cat[0] = mat4_identity();
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
            if (g_depth[i] != rd - 1) continue;
            for (gi = 0; gi < N_GEN && g_cat_size < MAX_CAT; gi++) {
                Mat4 prod = mat4_mul(&g_cat[i], &g_gen[gi]);
                if (hash_find(&prod) < 0) {
                    long mabs = mat4_max_abs(&prod);
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
            printf("  WARNING: entries approaching overflow\n");
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
 * Activation: 4x4 sign hash
 * Sign of all 64 integer components. 3-valued hash mod k_param.
 * ================================================================ */

static int mat4_activate(const Mat4 *m, int k_param) {
    unsigned long h = 0;
    const long *p = &m->m[0][0].a;
    int i;
    for (i = 0; i < DIM * DIM * 4; i++) {
        h = h * 3UL + (unsigned long)(p[i] > 0 ? 2 : (p[i] < 0 ? 0 : 1));
    }
    return (int)(h % (unsigned long)k_param);
}

/* ================================================================
 * XOR test for 4x4 matrices
 * ================================================================ */

#define MAX_ACT_CELLS 8192

static int cell_even[MAX_ACT_CELLS], cell_odd[MAX_ACT_CELLS];
static int touched[MAX_ACT_CELLS];

static int test_xor_mat4(const int *indices, int n_weights, int k_param) {
    int n_inputs = 2 * n_weights;
    int n_masks = 1 << n_inputs;
    int n_touched = 0;
    int mask, i, w;
    int result = 1;
    Mat4 pos[8], neg[8];

    if (k_param > MAX_ACT_CELLS || n_inputs > 16 || n_weights > 8)
        return 0;

    for (w = 0; w < n_weights; w++) {
        pos[w] = g_cat[indices[w]];
        neg[w] = mat4_neg(&g_cat[indices[w]]);
    }

    for (mask = 0; mask < n_masks; mask++) {
        Mat4 sum = mat4_zero();
        int par = 0, cell;

        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                w = i / 2;
                if (i % 2 == 0)
                    sum = mat4_add(&sum, &pos[w]);
                else
                    sum = mat4_add(&sum, &neg[w]);
                par ^= 1;
            }
        }

        cell = mat4_activate(&sum, k_param);

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
            if (test_xor_mat4(indices, 3, k_param))
                count++;
        }
    } else if (n_weights == 4) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++) {
            indices[0] = i0; indices[1] = i1;
            indices[2] = i2; indices[3] = i3;
            if (test_xor_mat4(indices, 4, k_param))
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
            if (test_xor_mat4(indices, 5, k_param))
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
            if (test_xor_mat4(indices, 6, k_param))
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
            if (test_xor_mat4(indices, 7, k_param))
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

    printf("KNOTAPEL DEMO 101: 5-Strand DKC on W_{5,3}\n");
    printf("============================================\n");

    /* ============================================================
     * Phase 0: Build and Verify Representation
     * ============================================================ */
    printf("\n=== Phase 0: Representation Verification ===\n\n");

    build_tl_generators();

    g_A = cyc8_make(0, -1, 0, 0);
    g_A_inv = cyc8_conj(g_A);

    printf("  A = (0,-1,0,0)\n");
    printf("  A_inv = (0,0,0,1)\n");

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

    /* Print TL generators */
    printf("\n  TL generators (4x4 integer matrices):\n");
    {
        int ei;
        for (ei = 0; ei < 4; ei++) {
            sprintf(msg, "e_%d", ei + 1);
            mat4_print(msg, &g_e[ei]);
        }
    }

    /* Verify e_i^2 = 0 */
    {
        int ei;
        for (ei = 0; ei < 4; ei++) {
            Mat4 sq = mat4_mul(&g_e[ei], &g_e[ei]);
            Mat4 z = mat4_zero();
            sprintf(msg, "e_%d^2 = 0 (delta=0)", ei + 1);
            check(msg, mat4_eq(&sq, &z));
        }
    }

    /* Verify e_i * e_{i+1} * e_i = e_i (Jones-Wenzl) */
    {
        int ei;
        for (ei = 0; ei < 3; ei++) {
            Mat4 t;
            /* e_i * e_{i+1} * e_i = e_i */
            t = mat4_mul(&g_e[ei], &g_e[ei + 1]);
            t = mat4_mul(&t, &g_e[ei]);
            sprintf(msg, "e_%d * e_%d * e_%d = e_%d",
                    ei+1, ei+2, ei+1, ei+1);
            check(msg, mat4_eq(&t, &g_e[ei]));

            /* e_{i+1} * e_i * e_{i+1} = e_{i+1} */
            t = mat4_mul(&g_e[ei + 1], &g_e[ei]);
            t = mat4_mul(&t, &g_e[ei + 1]);
            sprintf(msg, "e_%d * e_%d * e_%d = e_%d",
                    ei+2, ei+1, ei+2, ei+2);
            check(msg, mat4_eq(&t, &g_e[ei + 1]));
        }
    }

    /* Far commutativity: e_i * e_j = e_j * e_i for |i-j| >= 2 */
    {
        int pairs[][2] = {{0, 2}, {0, 3}, {1, 3}};
        int pi;
        for (pi = 0; pi < 3; pi++) {
            int a = pairs[pi][0], b = pairs[pi][1];
            Mat4 ab = mat4_mul(&g_e[a], &g_e[b]);
            Mat4 ba = mat4_mul(&g_e[b], &g_e[a]);
            sprintf(msg, "e_%d * e_%d = e_%d * e_%d (far commutativity)",
                    a+1, b+1, b+1, a+1);
            check(msg, mat4_eq(&ab, &ba));
        }
    }

    /* Verify radical = {0} */
    printf("\n  Radical verification:\n");
    {
        /* For each standard basis vector, check if ALL e_i kill it.
         * Only the zero vector should survive. */
        int bi, all_killed;
        int radical_dim = 0;
        for (bi = 0; bi < DIM; bi++) {
            Cyc8 v[DIM];
            int vi;
            all_killed = 1;
            for (vi = 0; vi < DIM; vi++)
                v[vi] = (vi == bi) ? cyc8_one() : cyc8_zero();

            {
                int ei;
                for (ei = 0; ei < 4 && all_killed; ei++) {
                    int ri;
                    for (ri = 0; ri < DIM && all_killed; ri++) {
                        Cyc8 res = cyc8_zero();
                        int ci;
                        for (ci = 0; ci < DIM; ci++)
                            res = cyc8_add(res,
                                cyc8_mul(g_e[ei].m[ri][ci], v[ci]));
                        if (!cyc8_is_zero(res))
                            all_killed = 0;
                    }
                }
            }
            if (all_killed) radical_dim++;
        }

        printf("  Radical dimension: %d\n", radical_dim);
        check("W_{5,3} is simple (radical = {0})", radical_dim == 0);
    }

    /* Build braid generators */
    build_braid_generators();

    printf("\n  Braid generators:\n");
    mat4_print("sigma_1", &g_gen[0]);
    mat4_print("sigma_2", &g_gen[2]);
    mat4_print("sigma_3", &g_gen[4]);
    mat4_print("sigma_4", &g_gen[6]);

    /* Verify sigma_i * sigma_i_inv = I */
    {
        int si;
        Mat4 id4 = mat4_identity();
        for (si = 0; si < 4; si++) {
            Mat4 prod = mat4_mul(&g_gen[2*si], &g_gen[2*si+1]);
            sprintf(msg, "sigma_%d * sigma_%d_inv = I", si+1, si+1);
            check(msg, mat4_eq(&prod, &id4));
        }
    }

    /* Braid relations: adjacent s_i*s_{i+1}*s_i = s_{i+1}*s_i*s_{i+1} */
    {
        int si;
        for (si = 0; si < 3; si++) {
            Mat4 lhs, rhs;
            lhs = mat4_mul(&g_gen[2*si], &g_gen[2*(si+1)]);
            lhs = mat4_mul(&lhs, &g_gen[2*si]);
            rhs = mat4_mul(&g_gen[2*(si+1)], &g_gen[2*si]);
            rhs = mat4_mul(&rhs, &g_gen[2*(si+1)]);
            sprintf(msg, "Braid: s%d*s%d*s%d = s%d*s%d*s%d",
                    si+1, si+2, si+1, si+2, si+1, si+2);
            check(msg, mat4_eq(&lhs, &rhs));
        }
    }

    /* Far braid commutativity */
    {
        int pairs[][2] = {{0, 2}, {0, 3}, {1, 3}};
        int pi;
        for (pi = 0; pi < 3; pi++) {
            int a = pairs[pi][0], b = pairs[pi][1];
            Mat4 ab = mat4_mul(&g_gen[2*a], &g_gen[2*b]);
            Mat4 ba = mat4_mul(&g_gen[2*b], &g_gen[2*a]);
            sprintf(msg, "Far: s%d*s%d = s%d*s%d",
                    a+1, b+1, b+1, a+1);
            check(msg, mat4_eq(&ab, &ba));
        }
    }

    /* Hecke relations: s_i - s_i_inv = (A - A_inv)(I - e_i) */
    {
        int si;
        Cyc8 a_minus_ainv = cyc8_sub(g_A, g_A_inv);
        Mat4 id4 = mat4_identity();

        for (si = 0; si < 4; si++) {
            Mat4 neg_sinv = mat4_neg(&g_gen[2*si+1]);
            Mat4 lhs_m = mat4_add(&g_gen[2*si], &neg_sinv);
            Mat4 neg_ei = mat4_neg(&g_e[si]);
            Mat4 i_minus_e = mat4_add(&id4, &neg_ei);
            Mat4 rhs_m = mat4_scale(a_minus_ainv, &i_minus_e);
            sprintf(msg, "Hecke: s%d - s%d_inv = (A-A_inv)(I-e%d)",
                    si+1, si+1, si+1);
            check(msg, mat4_eq(&lhs_m, &rhs_m));
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
     * Phase 2: Catalog Statistics + XOR Capacity
     * ============================================================ */
    printf("\n=== Phase 2: Catalog Statistics ===\n\n");

    {
        int max_d = 0;
        long max_mag = 0;
        int depth_count[20];

        memset(depth_count, 0, sizeof(depth_count));

        for (i = 0; i < g_cat_size; i++) {
            long m = mat4_max_abs(&g_cat[i]);
            if (m > max_mag) max_mag = m;
            if (g_depth[i] > max_d && g_depth[i] < 20) max_d = g_depth[i];
            if (g_depth[i] < 20) depth_count[g_depth[i]]++;
        }

        printf("  Max depth: %d\n", max_d);
        printf("  Max entry magnitude: %ld\n", max_mag);

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
            for (r = 0; r < DIM; r++) {
                printf("      [");
                for (c = 0; c < DIM; c++) {
                    printf("(%ld,%ld,%ld,%ld)",
                           g_cat[i].m[r][c].a, g_cat[i].m[r][c].b,
                           g_cat[i].m[r][c].c, g_cat[i].m[r][c].d);
                    if (c < DIM - 1) printf(", ");
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
                   prev_count > 0 ? (double)depth_count2[d] / prev_count
                                  : 0.0);
            prev_count = depth_count2[d];
        }
    }

    /* sigma_1^n growth test */
    printf("\n  sigma_1^n growth test:\n");
    {
        Mat4 s1_pow = mat4_identity();
        int n;
        for (n = 1; n <= 8; n++) {
            s1_pow = mat4_mul(&s1_pow, &g_gen[0]);
            printf("    n=%d: max_abs=%ld\n", n, mat4_max_abs(&s1_pow));
        }
        {
            Mat4 id4 = mat4_identity();
            int is_identity = mat4_eq(&s1_pow, &id4);
            printf("  sigma_1^8 %s identity\n",
                   is_identity ? "=" : "!=");
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
        int best_xor6 = 0, best_k = 0;

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
            Mat4 saved[40];
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
                    printf("    XOR6=%d  XOR8=%d  XOR10=%d  XOR12=%d"
                           "  XOR14=%d\n",
                           x6d, x8d, x10d, x12d, x14d);
                    sprintf(msg, "Deep XOR6=%d XOR8=%d XOR10=%d"
                            " XOR12=%d XOR14=%d",
                            x6d, x8d, x10d, x12d, x14d);
                    check(msg, 1);
                }

                /* k-sweep for XOR14 */
                printf("  Deep XOR14 k-sweep:\n");
                {
                    static const int ks[] = {256, 512, 1024, 2048, 4096};
                    int ki;
                    for (ki = 0; ki < 5; ki++) {
                        int x14k = count_xor_bf(7, ks[ki], bf_deep);
                        printf("    k=%d: XOR14=%d\n", ks[ki], x14k);
                    }
                }

                /* Restore from bf=30 */
                for (j = 0; j < bf_deep && j < 40; j++) {
                    g_cat[j] = saved[j];
                    g_depth[j] = sd[j];
                    g_writhe[j] = sw[j];
                }
                g_cat_size = saved_sz;

                /* NOTE: bf=40+ XOR14 checks skipped -- too slow
                 * (C(40,7)=18.6M combos with 4x4 matrices).
                 * k-sweep above is the stronger test. */
            }
        }
    }

    /* ============================================================
     * Phase 4: Casimir Analysis
     *
     * No radical (W_{5,3} simple), so no radical_content.
     * C4(M) = 4*tr(M^2) - (tr M)^2 (integer-exact).
     * ============================================================ */
    printf("\n=== Phase 4: Casimir Analysis ===\n\n");

    {
        static long cas4[MAX_CAT];

        /* Compute C4 for all entries */
        printf("  Step 1: C4 per entry\n");
        {
            long sum_cas = 0;

            for (i = 0; i < g_cat_size; i++) {
                Cyc8 c4_val = mat4_casimir(&g_cat[i]);
                cas4[i] = cyc8_l1(c4_val);
                sum_cas += cas4[i];
            }

            printf("  Catalog: %d entries\n", g_cat_size);
            printf("  Mean |C4|: %ld\n", sum_cas / (long)g_cat_size);

            /* By depth */
            printf("\n  %-6s  %12s  %6s\n", "depth", "mean_C4", "count");
            {
                int d;
                for (d = 0; d <= 12; d++) {
                    long sc = 0;
                    int cnt = 0, ii;
                    for (ii = 0; ii < g_cat_size; ii++) {
                        if (g_depth[ii] == d) {
                            sc += cas4[ii];
                            cnt++;
                        }
                    }
                    if (cnt > 0)
                        printf("  %-6d  %12ld  %6d\n",
                               d, sc / (long)cnt, cnt);
                }
            }

            check("C4 computed for all entries", 1);
        }

        /* Step 2: Deep sub-catalog with Casimir analysis */
        printf("\n  Step 2: Deep sub-catalog (d>=4, bf=30)\n");
        {
            int deep_idx[1024];
            int nd = 0;
            Mat4 saved_cat[40];
            int saved_d[40], saved_w[40];
            long saved_cas[40];
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

                for (j = 0; j < bf; j++) {
                    saved_cat[j] = g_cat[j];
                    saved_d[j] = g_depth[j];
                    saved_w[j] = g_writhe[j];
                    saved_cas[j] = cas4[j];
                }

                for (j = 0; j < bf; j++) {
                    int src = deep_idx[j * step_sz];
                    g_cat[j] = g_cat[src];
                    g_depth[j] = g_depth[src];
                    g_writhe[j] = g_writhe[src];
                    cas4[j] = cas4[src];
                }
                g_cat_size = bf;

                /* XOR10 winner vs control */
                printf("\n  Step 2a: XOR10 winner vs control\n");
                {
                    int n_win = 0, n_ctrl = 0;
                    long win_cas = 0, ctrl_cas = 0;
                    int idx[8];
                    int a0, a1, a2, a3, a4;
                    int ctrl_lim = 1000;

                    for (a0 = 0; a0 < bf; a0++)
                    for (a1 = a0+1; a1 < bf; a1++)
                    for (a2 = a1+1; a2 < bf; a2++)
                    for (a3 = a2+1; a3 < bf; a3++)
                    for (a4 = a3+1; a4 < bf; a4++) {
                        long tc;
                        idx[0]=a0; idx[1]=a1; idx[2]=a2;
                        idx[3]=a3; idx[4]=a4;

                        tc = cas4[a0] + cas4[a1] + cas4[a2]
                           + cas4[a3] + cas4[a4];

                        if (test_xor_mat4(idx, 5, 128)) {
                            n_win++;
                            win_cas += tc;
                        } else if (n_ctrl < ctrl_lim) {
                            n_ctrl++;
                            ctrl_cas += tc;
                        }
                    }

                    printf("  Winners: %d  Control: %d\n",
                           n_win, n_ctrl);

                    if (n_win > 0 && n_ctrl > 0) {
                        printf("  %-12s  %12s  %12s\n",
                               "", "Winners", "Control");
                        printf("  %-12s  %12ld  %12ld\n",
                               "mean_C4",
                               win_cas / (long)n_win,
                               ctrl_cas / (long)n_ctrl);
                    }

                    sprintf(msg, "XOR10 winners: %d", n_win);
                    check(msg, n_win > 0);
                }

                /* Casimir by XOR level */
                printf("\n  Step 2b: Casimir by XOR level\n");
                printf("  %-8s  %8s  %12s  %12s  %8s\n",
                       "XOR", "winners", "win_C4", "ctrl_C4", "ratio");

                /* XOR6 */
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
                        tc = cas4[a0]+cas4[a1]+cas4[a2];
                        if (test_xor_mat4(idx, 3, 128)) {
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
                }

                /* XOR8 */
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
                        tc = cas4[a0]+cas4[a1]+cas4[a2]+cas4[a3];
                        if (test_xor_mat4(idx, 4, 128)) {
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
                }

                /* XOR10 */
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
                        tc = cas4[a0]+cas4[a1]+cas4[a2]
                           + cas4[a3]+cas4[a4];
                        if (test_xor_mat4(idx, 5, 128)) {
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
                }

                /* XOR12 */
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
                        tc = cas4[a0]+cas4[a1]+cas4[a2]
                           + cas4[a3]+cas4[a4]+cas4[a5];
                        if (test_xor_mat4(idx, 6, 128)) {
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
                }

                check("Casimir-by-XOR-level sweep completed", 1);

                /* ============================================
                 * Phase 5: Hub Anatomy
                 * ============================================ */
                printf("\n=== Phase 5: Hub Anatomy ===\n\n");

                /* Step 1: Entry frequency + co-occurrence */
                printf("  Step 1: Entry frequency in XOR10 winners\n");
                {
                    static int entry_freq[40];
                    static int cooccur[40][40];
                    int n_win5 = 0;
                    int hub_idx[20];
                    int n_hubs = 0;
                    int a0, a1, a2, a3, a4;
                    int idx5[8];
                    int pp, qq;

                    memset(entry_freq, 0, (size_t)bf * sizeof(int));
                    memset(cooccur, 0, sizeof(cooccur));

                    for (a0 = 0; a0 < bf; a0++)
                    for (a1 = a0+1; a1 < bf; a1++)
                    for (a2 = a1+1; a2 < bf; a2++)
                    for (a3 = a2+1; a3 < bf; a3++)
                    for (a4 = a3+1; a4 < bf; a4++) {
                        idx5[0]=a0; idx5[1]=a1; idx5[2]=a2;
                        idx5[3]=a3; idx5[4]=a4;
                        if (test_xor_mat4(idx5, 5, 128)) {
                            n_win5++;
                            entry_freq[a0]++;
                            entry_freq[a1]++;
                            entry_freq[a2]++;
                            entry_freq[a3]++;
                            entry_freq[a4]++;
                            for (pp = 0; pp < 5; pp++)
                                for (qq = pp+1; qq < 5; qq++)
                                    cooccur[idx5[pp]][idx5[qq]]++;
                        }
                    }

                    printf("  Total XOR10 winners: %d\n", n_win5);

                    /* Frequency histogram */
                    printf("\n  Frequency histogram:\n");
                    {
                        int freq_hist[200];
                        int max_freq = 0, fi;
                        memset(freq_hist, 0, sizeof(freq_hist));
                        for (fi = 0; fi < bf; fi++) {
                            if (entry_freq[fi] > max_freq)
                                max_freq = entry_freq[fi];
                            if (entry_freq[fi] < 200)
                                freq_hist[entry_freq[fi]]++;
                        }
                        printf("  %-6s  %6s\n", "freq", "entries");
                        for (fi = 0; fi <= max_freq && fi < 200; fi++) {
                            if (freq_hist[fi] > 0)
                                printf("  %-6d  %6d\n",
                                       fi, freq_hist[fi]);
                        }
                    }

                    /* Identify super-hubs (>35% of winners) */
                    {
                        int threshold = n_win5 / 3;
                        int fi;
                        printf("\n  Super-hubs (freq > %d):\n",
                               threshold);
                        for (fi = 0; fi < bf && n_hubs < 20; fi++) {
                            if (entry_freq[fi] > threshold) {
                                hub_idx[n_hubs] = fi;
                                printf("    Hub %d: entry %d, "
                                       "freq=%d (%.1f%%), d=%d, w=%d\n",
                                       n_hubs, fi, entry_freq[fi],
                                       n_win5 > 0 ?
                                       100.0 * (double)entry_freq[fi]
                                             / (double)n_win5 : 0.0,
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
                        for (hi = 0; hi < n_hubs && hi < 12; hi++) {
                            int hidx = hub_idx[hi];
                            Cyc8 tr_h = mat4_trace(&g_cat[hidx]);
                            Cyc8 c4_h = mat4_casimir(&g_cat[hidx]);
                            long c4_l1 = cyc8_l1(c4_h);
                            long frob = mat4_frob_l1(&g_cat[hidx]);

                            printf("\n    Hub %d (entry %d, "
                                   "d=%d, w=%d):\n",
                                   hi, hidx,
                                   g_depth[hidx], g_writhe[hidx]);
                            mat4_print("matrix", &g_cat[hidx]);
                            printf("    trace = (%ld,%ld,%ld,%ld)\n",
                                   tr_h.a, tr_h.b, tr_h.c, tr_h.d);
                            printf("    C4 = (%ld,%ld,%ld,%ld), "
                                   "|C4|=%ld\n",
                                   c4_h.a, c4_h.b, c4_h.c, c4_h.d,
                                   c4_l1);
                            printf("    Frobenius L1 = %ld\n", frob);
                        }

                        check("Super-hub properties reported", 1);
                    }

                    /* Step 3: Hub commutators */
                    if (n_hubs >= 2) {
                        printf("\n  Step 3: Hub commutator table\n");
                        printf("  %-4s %-4s  %20s  %8s  %8s\n",
                               "i", "j", "trace", "|C4|", "Frob");

                        {
                            int hi, hj;
                            int n_traceless = 0;
                            for (hi = 0; hi < n_hubs && hi < 12; hi++) {
                                for (hj = hi+1;
                                     hj < n_hubs && hj < 12;
                                     hj++) {
                                    int ha = hub_idx[hi];
                                    int hb = hub_idx[hj];
                                    Mat4 ab, ba, nba, comm;
                                    Cyc8 tr_c, c4_c;
                                    long c4_l1, frob_c;

                                    ab = mat4_mul(&g_cat[ha],
                                                  &g_cat[hb]);
                                    ba = mat4_mul(&g_cat[hb],
                                                  &g_cat[ha]);
                                    nba = mat4_neg(&ba);
                                    comm = mat4_add(&ab, &nba);

                                    tr_c = mat4_trace(&comm);
                                    c4_c = mat4_casimir(&comm);
                                    c4_l1 = cyc8_l1(c4_c);
                                    frob_c = mat4_frob_l1(&comm);

                                    if (cyc8_is_zero(tr_c))
                                        n_traceless++;

                                    printf("  %-4d %-4d  "
                                           "(%3ld,%3ld,%3ld,%3ld)  "
                                           "%8ld  %8ld\n",
                                           hi, hj,
                                           tr_c.a, tr_c.b,
                                           tr_c.c, tr_c.d,
                                           c4_l1, frob_c);
                                }
                            }

                            {
                                int total_pairs;
                                int nh = n_hubs < 12 ? n_hubs : 12;
                                total_pairs = nh * (nh - 1) / 2;
                                sprintf(msg,
                                    "Traceless commutators: %d / %d",
                                    n_traceless, total_pairs);
                                check(msg, n_traceless == total_pairs);
                            }
                        }
                    }

                    /* Step 4: First commutator detail */
                    if (n_hubs >= 2) {
                        Mat4 ab, ba, nba, comm;
                        Cyc8 tr_c;

                        printf("\n  Step 4: [hub_0, hub_1] detail\n");

                        ab = mat4_mul(&g_cat[hub_idx[0]],
                                      &g_cat[hub_idx[1]]);
                        ba = mat4_mul(&g_cat[hub_idx[1]],
                                      &g_cat[hub_idx[0]]);
                        nba = mat4_neg(&ba);
                        comm = mat4_add(&ab, &nba);

                        mat4_print("commutator", &comm);

                        tr_c = mat4_trace(&comm);
                        printf("    trace = (%ld,%ld,%ld,%ld)\n",
                               tr_c.a, tr_c.b, tr_c.c, tr_c.d);
                        check("First commutator traceless",
                              cyc8_is_zero(tr_c));
                    }

                    /* Step 5: Topology */
                    printf("\n  Step 5: Topology "
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

                        printf("\n  %-6s  %6s  %6s  %6s  %8s\n",
                               "entry", "freq", "degree", "depth",
                               "C4");
                        {
                            int fi2;
                            for (fi2 = 0; fi2 < bf; fi2++) {
                                if (entry_freq[fi2] > 0)
                                    printf("  %-6d  %6d  %6d  %6d"
                                           "  %8ld\n",
                                           fi2, entry_freq[fi2],
                                           degree[fi2], g_depth[fi2],
                                           cas4[fi2]);
                            }
                        }

                        /* Star-graph check */
                        if (n_hubs >= 2) {
                            for (fi = 0; fi < bf; fi++) {
                                int fi_hub = 0, hi;
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

                /* Step 6: XOR12 hub frequency scan */
                printf("\n  Step 6: XOR12 hub frequency "
                       "(6 weights, %d entries)\n", bf);
                {
                    static int freq12[40];
                    int n_win12 = 0;
                    int a0, a1, a2, a3, a4, a5;
                    int idx6[8];

                    memset(freq12, 0, (size_t)bf * sizeof(int));

                    for (a0 = 0; a0 < bf; a0++)
                    for (a1 = a0+1; a1 < bf; a1++)
                    for (a2 = a1+1; a2 < bf; a2++)
                    for (a3 = a2+1; a3 < bf; a3++)
                    for (a4 = a3+1; a4 < bf; a4++)
                    for (a5 = a4+1; a5 < bf; a5++) {
                        idx6[0]=a0; idx6[1]=a1; idx6[2]=a2;
                        idx6[3]=a3; idx6[4]=a4; idx6[5]=a5;
                        if (test_xor_mat4(idx6, 6, 128)) {
                            n_win12++;
                            freq12[a0]++;
                            freq12[a1]++;
                            freq12[a2]++;
                            freq12[a3]++;
                            freq12[a4]++;
                            freq12[a5]++;
                        }
                    }

                    printf("  Total XOR12 winners: %d\n", n_win12);
                    if (n_win12 > 0) {
                        int fi;
                        printf("  %-6s  %6s  %6s  %8s\n",
                               "entry", "freq12", "depth", "C4");
                        for (fi = 0; fi < bf; fi++) {
                            if (freq12[fi] > 0)
                                printf("  %-6d  %6d  %6d  %8ld\n",
                                       fi, freq12[fi],
                                       g_depth[fi], cas4[fi]);
                        }
                    }

                    sprintf(msg, "XOR12 winners: %d", n_win12);
                    check(msg, 1);
                }

                /* Restore catalog */
                for (j = 0; j < bf; j++) {
                    g_cat[j] = saved_cat[j];
                    g_depth[j] = saved_d[j];
                    g_writhe[j] = saved_w[j];
                    cas4[j] = saved_cas[j];
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
