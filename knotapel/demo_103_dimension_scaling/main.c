/*
 * KNOTAPEL DEMO 103: Pure Dimension Scaling Test
 * ================================================
 *
 * W_{6,0} (simple, dim=5) vs W_{6,2} (simple, dim=9)
 * Both on B_6 (6-strand braid group), both at delta=0, both simple.
 * The ONLY structural difference: dimension (5 vs 9).
 *
 * KEY TEST: Does BFS growth rate follow strand count (~5x) or dimension (~9x)?
 *
 * 7 pre-computation predictions:
 *   P1: W_{6,2} dim=9, simple (radical dim=0)
 *   P2: Growth rate: ~5x (strand law) OR ~9x (dim law)
 *   P3: W_{6,2} XOR6 >> W_{6,0}'s 2449
 *   P4: W_{6,2} XOR ceiling higher (XOR10>1, maybe XOR12>0)
 *   P5: W_{6,2} max_abs >= W_{6,0}'s 16
 *   P6: Both commuting hubs (simplicity)
 *   P7: W_{6,2} Casimir > W_{6,0}'s 48
 *
 * W_{6,0}: 5 half-diagrams, all arcs (0 through-lines)
 *   h0=(1,2)(3,4)(5,6), h1=(1,2)(3,6)(4,5), h2=(1,4)(2,3)(5,6)
 *   h3=(1,6)(2,3)(4,5), h4=(1,6)(2,5)(3,4)
 *
 * W_{6,2}: 9 half-diagrams, 2 arcs + 2 through-lines
 *   h0: arcs(1,2)(3,4) TL{5,6}    h1: arcs(1,4)(2,3) TL{5,6}
 *   h2: arcs(1,2)(4,5) TL{3,6}    h3: arcs(1,2)(5,6) TL{3,4}
 *   h4: arcs(2,3)(4,5) TL{1,6}    h5: arcs(2,5)(3,4) TL{1,6}
 *   h6: arcs(2,3)(5,6) TL{1,4}    h7: arcs(3,4)(5,6) TL{1,2}
 *   h8: arcs(3,6)(4,5) TL{1,2}
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

typedef struct { long a, b, c, d; } Cyc8;

static Cyc8 cyc8_make(long a, long b, long c, long d) {
    Cyc8 z; z.a = a; z.b = b; z.c = c; z.d = d; return z;
}
static Cyc8 cyc8_zero(void) { return cyc8_make(0,0,0,0); }
static Cyc8 cyc8_one(void)  { return cyc8_make(1,0,0,0); }

static Cyc8 cyc8_add(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a+y.a, x.b+y.b, x.c+y.c, x.d+y.d);
}
static Cyc8 cyc8_sub(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a-y.a, x.b-y.b, x.c-y.c, x.d-y.d);
}
static Cyc8 cyc8_neg(Cyc8 x) {
    return cyc8_make(-x.a, -x.b, -x.c, -x.d);
}
static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y) {
    return cyc8_make(
        x.a*y.a - x.b*y.d - x.c*y.c - x.d*y.b,
        x.a*y.b + x.b*y.a - x.c*y.d - x.d*y.c,
        x.a*y.c + x.b*y.b + x.c*y.a - x.d*y.d,
        x.a*y.d + x.b*y.c + x.c*y.b + x.d*y.a);
}
static int cyc8_eq(Cyc8 x, Cyc8 y) {
    return x.a==y.a && x.b==y.b && x.c==y.c && x.d==y.d;
}
static int cyc8_is_zero(Cyc8 x) {
    return x.a==0 && x.b==0 && x.c==0 && x.d==0;
}
static Cyc8 cyc8_conj(Cyc8 z) {
    return cyc8_make(z.a, -z.d, -z.c, -z.b);
}
static long cyc8_max_abs(Cyc8 z) {
    long m = 0, v;
    v = z.a<0?-z.a:z.a; if(v>m) m=v;
    v = z.b<0?-z.b:z.b; if(v>m) m=v;
    v = z.c<0?-z.c:z.c; if(v>m) m=v;
    v = z.d<0?-z.d:z.d; if(v>m) m=v;
    return m;
}
static long cyc8_l1(Cyc8 z) {
    long v = 0;
    v += z.a<0?-z.a:z.a; v += z.b<0?-z.b:z.b;
    v += z.c<0?-z.c:z.c; v += z.d<0?-z.d:z.d;
    return v;
}

/* A^n for n mod 8 (not used in D103 -- both modules simple) */

/* ================================================================
 * Variable-dimension Matrix over Cyc8
 * MAX_DIM=9 (for W_{6,2}), g_dim controls active size
 * ================================================================ */

#define MAX_DIM 9

static int g_dim = 5;

typedef struct { Cyc8 m[MAX_DIM][MAX_DIM]; } MatN;

static MatN matN_zero(void) {
    MatN r;
    memset(&r, 0, sizeof(MatN));
    return r;
}

static MatN matN_identity(void) {
    MatN r;
    int i;
    memset(&r, 0, sizeof(MatN));
    for (i = 0; i < g_dim; i++)
        r.m[i][i] = cyc8_one();
    return r;
}

static MatN matN_mul(const MatN *p, const MatN *q) {
    MatN r;
    int i, j, k;
    memset(&r, 0, sizeof(MatN));
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            for (k = 0; k < g_dim; k++)
                r.m[i][j] = cyc8_add(r.m[i][j],
                    cyc8_mul(p->m[i][k], q->m[k][j]));
    return r;
}

/* Pointer-based multiply for hot path (avoids 2.5KB return by value) */
static void matN_mul_to(const MatN *p, const MatN *q, MatN *out) {
    int i, j, k;
    memset(out, 0, sizeof(MatN));
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            for (k = 0; k < g_dim; k++)
                out->m[i][j] = cyc8_add(out->m[i][j],
                    cyc8_mul(p->m[i][k], q->m[k][j]));
}

static int matN_eq(const MatN *p, const MatN *q) {
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            if (!cyc8_eq(p->m[i][j], q->m[i][j])) return 0;
    return 1;
}

static MatN matN_add(const MatN *p, const MatN *q) {
    MatN r;
    int i, j;
    memset(&r, 0, sizeof(MatN));
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            r.m[i][j] = cyc8_add(p->m[i][j], q->m[i][j]);
    return r;
}

static MatN matN_sub(const MatN *p, const MatN *q) {
    MatN r;
    int i, j;
    memset(&r, 0, sizeof(MatN));
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            r.m[i][j] = cyc8_sub(p->m[i][j], q->m[i][j]);
    return r;
}

static MatN matN_neg(const MatN *p) {
    MatN r;
    int i, j;
    memset(&r, 0, sizeof(MatN));
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            r.m[i][j] = cyc8_neg(p->m[i][j]);
    return r;
}

static MatN matN_scale(Cyc8 s, const MatN *p) {
    MatN r;
    int i, j;
    memset(&r, 0, sizeof(MatN));
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            r.m[i][j] = cyc8_mul(s, p->m[i][j]);
    return r;
}

/* In-place add/sub for XOR test hot path */
static void matN_add_inplace(MatN *dst, const MatN *src) {
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            dst->m[i][j] = cyc8_add(dst->m[i][j], src->m[i][j]);
}

static void matN_sub_inplace(MatN *dst, const MatN *src) {
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            dst->m[i][j] = cyc8_sub(dst->m[i][j], src->m[i][j]);
}

static long matN_max_abs(const MatN *m) {
    long mx = 0, v;
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++) {
            v = cyc8_max_abs(m->m[i][j]);
            if (v > mx) mx = v;
        }
    return mx;
}

static Cyc8 matN_trace(const MatN *m) {
    Cyc8 t = cyc8_zero();
    int i;
    for (i = 0; i < g_dim; i++)
        t = cyc8_add(t, m->m[i][i]);
    return t;
}

/* tr(M^2) = sum_{j,k} M[j][k]*M[k][j] */
static Cyc8 matN_tr_sq(const MatN *m) {
    Cyc8 t = cyc8_zero();
    int j, k;
    for (j = 0; j < g_dim; j++)
        for (k = 0; k < g_dim; k++)
            t = cyc8_add(t, cyc8_mul(m->m[j][k], m->m[k][j]));
    return t;
}

/* C_d(M) = d*tr(M^2) - (tr M)^2 */
static Cyc8 matN_casimir(const MatN *m) {
    Cyc8 tr = matN_trace(m);
    Cyc8 trsq = matN_tr_sq(m);
    Cyc8 dim_c = cyc8_make((long)g_dim, 0, 0, 0);
    return cyc8_sub(cyc8_mul(dim_c, trsq), cyc8_mul(tr, tr));
}

static long matN_frob_l1(const MatN *m) {
    long f = 0;
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            f += cyc8_l1(m->m[i][j]);
    return f;
}

static int matN_nonzero_count(const MatN *m) {
    int cnt = 0, i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            if (!cyc8_is_zero(m->m[i][j])) cnt++;
    return cnt;
}

/* ================================================================
 * Hash table for MatN BFS
 * ================================================================ */

#define MAX_CAT 32768
#define HASH_SIZE 65537

static MatN *g_cat = NULL;
static int   g_depth[MAX_CAT];
static int   g_writhe[MAX_CAT];
static int   g_cat_size = 0;

static int g_hash_head[HASH_SIZE];
static int g_hash_next[MAX_CAT];

static unsigned long hash_matN(const MatN *m) {
    unsigned long h = 2166136261UL;
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++) {
            h = (h * 1000003UL) ^ (unsigned long)m->m[i][j].a;
            h = (h * 1000003UL) ^ (unsigned long)m->m[i][j].b;
            h = (h * 1000003UL) ^ (unsigned long)m->m[i][j].c;
            h = (h * 1000003UL) ^ (unsigned long)m->m[i][j].d;
        }
    return h;
}

static void hash_init(void) {
    memset(g_hash_head, -1, sizeof(g_hash_head));
}

static int hash_find(const MatN *m) {
    int bucket = (int)(hash_matN(m) % (unsigned long)HASH_SIZE);
    int idx = g_hash_head[bucket];
    while (idx >= 0) {
        if (matN_eq(&g_cat[idx], m)) return idx;
        idx = g_hash_next[idx];
    }
    return -1;
}

static void hash_insert(int cat_idx) {
    int bucket = (int)(hash_matN(&g_cat[cat_idx]) % (unsigned long)HASH_SIZE);
    g_hash_next[cat_idx] = g_hash_head[bucket];
    g_hash_head[bucket] = cat_idx;
}

/* ================================================================
 * TL generators
 *
 * W_{6,0}: dim=5, all arcs (0 through-lines)
 * W_{6,2}: dim=9, 2 arcs + 2 through-lines
 * ================================================================ */

#define N_TL 5

static MatN g_e[N_TL];

static void build_tl_a(void) {
    int i;
    for (i = 0; i < N_TL; i++) g_e[i] = matN_zero();

    /* e_1: h2->h0, h4->h0, h3->h1 */
    g_e[0].m[0][2] = cyc8_one();
    g_e[0].m[0][4] = cyc8_one();
    g_e[0].m[1][3] = cyc8_one();

    /* e_2: h0->h2, h1->h3, h4->h3 */
    g_e[1].m[2][0] = cyc8_one();
    g_e[1].m[3][1] = cyc8_one();
    g_e[1].m[3][4] = cyc8_one();

    /* e_3: h1->h0, h2->h0, h3->h4 */
    g_e[2].m[0][1] = cyc8_one();
    g_e[2].m[0][2] = cyc8_one();
    g_e[2].m[4][3] = cyc8_one();

    /* e_4: h0->h1, h2->h3, h4->h3 */
    g_e[3].m[1][0] = cyc8_one();
    g_e[3].m[3][2] = cyc8_one();
    g_e[3].m[3][4] = cyc8_one();

    /* e_5: h1->h0, h4->h0, h3->h2 */
    g_e[4].m[0][1] = cyc8_one();
    g_e[4].m[0][4] = cyc8_one();
    g_e[4].m[2][3] = cyc8_one();
}

static void build_tl_b(void) {
    int i;
    for (i = 0; i < N_TL; i++) g_e[i] = matN_zero();

    /* e_1: h1->h0, h5->h0, h4->h2, h6->h3 */
    g_e[0].m[0][1] = cyc8_one();
    g_e[0].m[0][5] = cyc8_one();
    g_e[0].m[2][4] = cyc8_one();
    g_e[0].m[3][6] = cyc8_one();

    /* e_2: h0->h1, h2->h4, h5->h4, h8->h4, h3->h6, h7->h6 */
    g_e[1].m[1][0] = cyc8_one();
    g_e[1].m[4][2] = cyc8_one();
    g_e[1].m[4][5] = cyc8_one();
    g_e[1].m[4][8] = cyc8_one();
    g_e[1].m[6][3] = cyc8_one();
    g_e[1].m[6][7] = cyc8_one();

    /* e_3: h1->h0, h2->h0, h4->h5, h6->h7, h8->h7 */
    g_e[2].m[0][1] = cyc8_one();
    g_e[2].m[0][2] = cyc8_one();
    g_e[2].m[5][4] = cyc8_one();
    g_e[2].m[7][6] = cyc8_one();
    g_e[2].m[7][8] = cyc8_one();

    /* e_4: h0->h2, h3->h2, h1->h4, h5->h4, h6->h4, h7->h8 */
    g_e[3].m[2][0] = cyc8_one();
    g_e[3].m[2][3] = cyc8_one();
    g_e[3].m[4][1] = cyc8_one();
    g_e[3].m[4][5] = cyc8_one();
    g_e[3].m[4][6] = cyc8_one();
    g_e[3].m[8][7] = cyc8_one();

    /* e_5: h2->h3, h4->h6, h5->h7, h8->h7 */
    g_e[4].m[3][2] = cyc8_one();
    g_e[4].m[6][4] = cyc8_one();
    g_e[4].m[7][5] = cyc8_one();
    g_e[4].m[7][8] = cyc8_one();
}

/* ================================================================
 * Braid generators: sigma_i = A*I + A^{-1}*e_i
 *                   sigma_i_inv = A^{-1}*I + A*e_i
 * 10 generators: sigma_1..5 and inverses
 * ================================================================ */

#define N_GEN 10

static Cyc8 g_A, g_A_inv;
static MatN g_gen[N_GEN];
static const int g_gen_writhe[N_GEN] = {1,-1,1,-1,1,-1,1,-1,1,-1};

static void build_braid_generators(void) {
    int i;
    MatN id_mat = matN_identity();
    MatN a_id = matN_scale(g_A, &id_mat);
    MatN ai_id = matN_scale(g_A_inv, &id_mat);

    for (i = 0; i < N_TL; i++) {
        MatN ai_e = matN_scale(g_A_inv, &g_e[i]);
        MatN a_e  = matN_scale(g_A, &g_e[i]);
        g_gen[2*i]     = matN_add(&a_id, &ai_e);
        g_gen[2*i + 1] = matN_add(&ai_id, &a_e);
    }
}

/* ================================================================
 * BFS catalog builder
 * ================================================================ */

static void build_catalog(int max_depth) {
    int prev, gi, i, rd;
    MatN prod;

    g_cat_size = 0;
    hash_init();

    g_cat[0] = matN_identity();
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
                long mabs;
                matN_mul_to(&g_cat[i], &g_gen[gi], &prod);
                if (hash_find(&prod) < 0) {
                    mabs = matN_max_abs(&prod);
                    if (mabs > round_max) round_max = mabs;
                    g_cat[g_cat_size] = prod;
                    g_depth[g_cat_size] = rd;
                    g_writhe[g_cat_size] = g_writhe[i] + g_gen_writhe[gi];
                    hash_insert(g_cat_size);
                    g_cat_size++;
                }
            }
        }
        if (g_cat_size > prev)
            printf("  Round %d: %d entries (+%d), max_abs=%ld\n",
                   rd, g_cat_size, g_cat_size - prev, round_max);
        if (round_max > 100000000000L) {
            printf("  WARNING: approaching overflow\n");
            break;
        }
        rd++;
    } while (g_cat_size > prev && g_cat_size < MAX_CAT && rd <= max_depth);

    if (g_cat_size == prev)
        printf("  GROUP CLOSED at %d entries\n", g_cat_size);
    else if (g_cat_size >= MAX_CAT)
        printf("  HIT CAP at %d entries (group is infinite)\n", MAX_CAT);
}

/* ================================================================
 * Radical dimension via Gaussian elimination
 * Stacks N_TL*g_dim rows x g_dim columns.
 * For g_dim=5: 25x5, for g_dim=9: 45x9.
 * ================================================================ */

static int compute_radical_dim(void) {
    long stk[45][9]; /* N_TL * MAX_DIM rows, MAX_DIM cols */
    int total_rows = N_TL * g_dim;
    int row, col, prow, r, c, rank;

    memset(stk, 0, sizeof(stk));

    /* Fill stacked matrix */
    row = 0;
    for (r = 0; r < N_TL; r++)
        for (c = 0; c < g_dim; c++) {
            int j;
            for (j = 0; j < g_dim; j++)
                stk[row][j] = g_e[r].m[c][j].a;
            row++;
        }

    /* Gaussian elimination */
    rank = 0;
    for (col = 0; col < g_dim; col++) {
        prow = -1;
        for (r = rank; r < total_rows; r++) {
            if (stk[r][col] != 0) { prow = r; break; }
        }
        if (prow < 0) continue;
        if (prow != rank) {
            for (c = 0; c < g_dim; c++) {
                long tmp = stk[rank][c];
                stk[rank][c] = stk[prow][c];
                stk[prow][c] = tmp;
            }
        }
        for (r = rank + 1; r < total_rows; r++) {
            if (stk[r][col] != 0) {
                long fn = stk[r][col], fd = stk[rank][col];
                for (c = 0; c < g_dim; c++)
                    stk[r][c] = stk[r][c] * fd - stk[rank][c] * fn;
            }
        }
        rank++;
    }
    return g_dim - rank;
}

/* ================================================================
 * Activation: sign hash of g_dim*g_dim*4 integer components
 * 3-valued: pos=2, zero=1, neg=0.  Polynomial hash mod k_param.
 * ================================================================ */

static int matN_activate(const MatN *m, int k_param) {
    unsigned long h = 0;
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++) {
            h = h * 3UL + (unsigned long)(m->m[i][j].a > 0 ? 2 : (m->m[i][j].a < 0 ? 0 : 1));
            h = h * 3UL + (unsigned long)(m->m[i][j].b > 0 ? 2 : (m->m[i][j].b < 0 ? 0 : 1));
            h = h * 3UL + (unsigned long)(m->m[i][j].c > 0 ? 2 : (m->m[i][j].c < 0 ? 0 : 1));
            h = h * 3UL + (unsigned long)(m->m[i][j].d > 0 ? 2 : (m->m[i][j].d < 0 ? 0 : 1));
        }
    return (int)(h % (unsigned long)k_param);
}

/* Subset activation: hash only first sub_dim x sub_dim block (100 comp for sub_dim=5) */
static int g_sub_dim = 5;

static int matN_activate_sub(const MatN *m, int k_param) {
    unsigned long h = 0;
    int i, j;
    for (i = 0; i < g_sub_dim; i++)
        for (j = 0; j < g_sub_dim; j++) {
            h = h * 3UL + (unsigned long)(m->m[i][j].a > 0 ? 2 : (m->m[i][j].a < 0 ? 0 : 1));
            h = h * 3UL + (unsigned long)(m->m[i][j].b > 0 ? 2 : (m->m[i][j].b < 0 ? 0 : 1));
            h = h * 3UL + (unsigned long)(m->m[i][j].c > 0 ? 2 : (m->m[i][j].c < 0 ? 0 : 1));
            h = h * 3UL + (unsigned long)(m->m[i][j].d > 0 ? 2 : (m->m[i][j].d < 0 ? 0 : 1));
        }
    return (int)(h % (unsigned long)k_param);
}

/* Strided activation: hash every stride-th component across full dim */
static int g_act_stride = 3;

static int matN_activate_stride(const MatN *m, int k_param) {
    unsigned long h = 0;
    int idx = 0;
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++) {
            if (idx % g_act_stride == 0) {
                h = h * 3UL + (unsigned long)(m->m[i][j].a > 0 ? 2 : (m->m[i][j].a < 0 ? 0 : 1));
                h = h * 3UL + (unsigned long)(m->m[i][j].b > 0 ? 2 : (m->m[i][j].b < 0 ? 0 : 1));
                h = h * 3UL + (unsigned long)(m->m[i][j].c > 0 ? 2 : (m->m[i][j].c < 0 ? 0 : 1));
                h = h * 3UL + (unsigned long)(m->m[i][j].d > 0 ? 2 : (m->m[i][j].d < 0 ? 0 : 1));
            }
            idx++;
        }
    return (int)(h % (unsigned long)k_param);
}

/* Function pointer for activation (switchable for subset tests) */
typedef int (*activate_fn)(const MatN *, int);
static activate_fn g_activate = matN_activate;

/* ================================================================
 * XOR test infrastructure
 * ================================================================ */

#define MAX_ACT_CELLS 8192

static int cell_even[MAX_ACT_CELLS], cell_odd[MAX_ACT_CELLS];
static int touched[MAX_ACT_CELLS];

static int test_xor(const int *indices, int n_weights, int k_param) {
    int n_inputs = 2 * n_weights;
    int n_masks = 1 << n_inputs;
    int n_touched = 0;
    int mask, i, w;
    int result = 1;

    if (k_param > MAX_ACT_CELLS || n_inputs > 16 || n_weights > 8)
        return 0;

    for (mask = 0; mask < n_masks; mask++) {
        MatN sum;
        int par = 0, cell;
        memset(&sum, 0, sizeof(MatN));

        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                w = i / 2;
                if (i % 2 == 0)
                    matN_add_inplace(&sum, &g_cat[indices[w]]);
                else
                    matN_sub_inplace(&sum, &g_cat[indices[w]]);
                par ^= 1;
            }
        }

        cell = g_activate(&sum, k_param);

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
    int i0, i1, i2, i3, i4, i5;
    int indices[8];

    if (n_weights == 3) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++) {
            indices[0]=i0; indices[1]=i1; indices[2]=i2;
            if (test_xor(indices, 3, k_param)) count++;
        }
    } else if (n_weights == 4) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++) {
            indices[0]=i0; indices[1]=i1; indices[2]=i2; indices[3]=i3;
            if (test_xor(indices, 4, k_param)) count++;
        }
    } else if (n_weights == 5) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++)
        for (i4 = i3+1; i4 < limit; i4++) {
            indices[0]=i0; indices[1]=i1; indices[2]=i2;
            indices[3]=i3; indices[4]=i4;
            if (test_xor(indices, 5, k_param)) count++;
        }
    } else if (n_weights == 6) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++)
        for (i4 = i3+1; i4 < limit; i4++)
        for (i5 = i4+1; i5 < limit; i5++) {
            indices[0]=i0; indices[1]=i1; indices[2]=i2;
            indices[3]=i3; indices[4]=i4; indices[5]=i5;
            if (test_xor(indices, 6, k_param)) count++;
        }
    }
    return count;
}

/* ================================================================
 * Saved data from Module A
 * ================================================================ */

#define BF_SAVE 30

static MatN g_save_cat[BF_SAVE];
static int  g_save_depth[BF_SAVE], g_save_writhe[BF_SAVE];
static long g_save_cas[BF_SAVE];
static int  g_save_bf;
static int  g_save_dim;

static int g_save_cat_size;
static int g_save_depth_counts[20];
static long g_save_max_abs[20];
static int g_save_xor[4]; /* XOR6..XOR12 at k=128, bf=30 deep */

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    char msg[256];
    int i;

    g_A = cyc8_make(0, -1, 0, 0);
    g_A_inv = cyc8_conj(g_A); /* (0,0,0,1) */

    /* Allocate catalog */
    g_cat = (MatN *)malloc((size_t)MAX_CAT * sizeof(MatN));
    if (!g_cat) {
        printf("FAIL: Cannot allocate catalog (%lu bytes)\n",
               (unsigned long)((size_t)MAX_CAT * sizeof(MatN)));
        return 1;
    }

    printf("KNOTAPEL DEMO 103: Pure Dimension Scaling Test\n");
    printf("W_{6,0} (simple, dim=5) vs W_{6,2} (simple, dim=9)\n");
    printf("=============================================\n");

    /* ============================================================
     * Phase 0: Generator Verification
     * ============================================================ */
    printf("\n=== Phase 0: Generator Verification ===\n");

    /* --- Module A: W_{6,0} --- */
    printf("\n--- Module A: W_{6,0} (simple, dim=5) ---\n\n");
    {
        int ei, rad_dim;

        g_dim = 5;
        build_tl_a();

        printf("  A = (0,-1,0,0), A_inv = (0,0,0,1)\n");
        {
            Cyc8 prod = cyc8_mul(g_A, g_A_inv);
            check("A * A_inv = 1", cyc8_eq(prod, cyc8_one()));
        }
        {
            Cyc8 a2 = cyc8_mul(g_A, g_A);
            Cyc8 ai2 = cyc8_mul(g_A_inv, g_A_inv);
            check("A^2 + A^{-2} = 0 (delta=0)",
                  cyc8_is_zero(cyc8_add(a2, ai2)));
        }

        /* e_i^2 = 0 */
        for (ei = 0; ei < N_TL; ei++) {
            MatN sq = matN_mul(&g_e[ei], &g_e[ei]);
            MatN z = matN_zero();
            sprintf(msg, "A: e_%d^2 = 0", ei+1);
            check(msg, matN_eq(&sq, &z));
        }

        /* Jones-Wenzl: e_i * e_{i+1} * e_i = e_i */
        for (ei = 0; ei < N_TL - 1; ei++) {
            MatN t = matN_mul(&g_e[ei], &g_e[ei+1]);
            t = matN_mul(&t, &g_e[ei]);
            sprintf(msg, "A: e_%d*e_%d*e_%d = e_%d", ei+1,ei+2,ei+1,ei+1);
            check(msg, matN_eq(&t, &g_e[ei]));
            t = matN_mul(&g_e[ei+1], &g_e[ei]);
            t = matN_mul(&t, &g_e[ei+1]);
            sprintf(msg, "A: e_%d*e_%d*e_%d = e_%d", ei+2,ei+1,ei+2,ei+2);
            check(msg, matN_eq(&t, &g_e[ei+1]));
        }

        /* Far commutativity: |i-j| >= 2 */
        {
            int pairs[][2] = {{0,2},{0,3},{0,4},{1,3},{1,4},{2,4}};
            int pi;
            for (pi = 0; pi < 6; pi++) {
                int a = pairs[pi][0], b = pairs[pi][1];
                MatN ab = matN_mul(&g_e[a], &g_e[b]);
                MatN ba = matN_mul(&g_e[b], &g_e[a]);
                sprintf(msg, "A: e_%d*e_%d = e_%d*e_%d", a+1,b+1,b+1,a+1);
                check(msg, matN_eq(&ab, &ba));
            }
        }

        /* Radical dimension */
        rad_dim = compute_radical_dim();
        printf("  Radical dimension: %d\n", rad_dim);
        check("P1a: W_{6,0} radical dim = 0 (simple)", rad_dim == 0);

        /* Sparsity */
        {
            int total_nz = 0;
            for (ei = 0; ei < N_TL; ei++)
                total_nz += matN_nonzero_count(&g_e[ei]);
            printf("  Generator sparsity A: %d/%d nonzero\n",
                   total_nz, N_TL * g_dim * g_dim);
        }

        /* Braid generators and relations */
        build_braid_generators();

        {
            int si;
            MatN id_mat = matN_identity();
            for (si = 0; si < N_TL; si++) {
                MatN bprod = matN_mul(&g_gen[2*si], &g_gen[2*si+1]);
                sprintf(msg, "A: s%d * s%d_inv = I", si+1, si+1);
                check(msg, matN_eq(&bprod, &id_mat));
            }
        }
        {
            int si;
            for (si = 0; si < N_TL - 1; si++) {
                MatN lhs = matN_mul(&g_gen[2*si], &g_gen[2*(si+1)]);
                MatN rhs;
                lhs = matN_mul(&lhs, &g_gen[2*si]);
                rhs = matN_mul(&g_gen[2*(si+1)], &g_gen[2*si]);
                rhs = matN_mul(&rhs, &g_gen[2*(si+1)]);
                sprintf(msg, "A: s%d*s%d*s%d = s%d*s%d*s%d",
                        si+1,si+2,si+1,si+2,si+1,si+2);
                check(msg, matN_eq(&lhs, &rhs));
            }
        }
        /* Hecke */
        {
            int si;
            Cyc8 a_m_ai = cyc8_sub(g_A, g_A_inv);
            MatN id_mat = matN_identity();
            for (si = 0; si < N_TL; si++) {
                MatN ns = matN_neg(&g_gen[2*si+1]);
                MatN lhs = matN_add(&g_gen[2*si], &ns);
                MatN ne = matN_neg(&g_e[si]);
                MatN ime = matN_add(&id_mat, &ne);
                MatN rhs = matN_scale(a_m_ai, &ime);
                sprintf(msg, "A: Hecke s%d", si+1);
                check(msg, matN_eq(&lhs, &rhs));
            }
        }
    }

    /* --- Module B: W_{6,2} --- */
    printf("\n--- Module B: W_{6,2} (simple, dim=9) ---\n\n");
    {
        int ei, rad_dim;

        g_dim = 9;
        build_tl_b();

        /* e_i^2 = 0 */
        for (ei = 0; ei < N_TL; ei++) {
            MatN sq = matN_mul(&g_e[ei], &g_e[ei]);
            MatN z = matN_zero();
            sprintf(msg, "B: e_%d^2 = 0", ei+1);
            check(msg, matN_eq(&sq, &z));
        }

        /* Jones-Wenzl */
        for (ei = 0; ei < N_TL - 1; ei++) {
            MatN t = matN_mul(&g_e[ei], &g_e[ei+1]);
            t = matN_mul(&t, &g_e[ei]);
            sprintf(msg, "B: e_%d*e_%d*e_%d = e_%d", ei+1,ei+2,ei+1,ei+1);
            check(msg, matN_eq(&t, &g_e[ei]));
            t = matN_mul(&g_e[ei+1], &g_e[ei]);
            t = matN_mul(&t, &g_e[ei+1]);
            sprintf(msg, "B: e_%d*e_%d*e_%d = e_%d", ei+2,ei+1,ei+2,ei+2);
            check(msg, matN_eq(&t, &g_e[ei+1]));
        }

        /* Far commutativity */
        {
            int pairs[][2] = {{0,2},{0,3},{0,4},{1,3},{1,4},{2,4}};
            int pi;
            for (pi = 0; pi < 6; pi++) {
                int a = pairs[pi][0], b = pairs[pi][1];
                MatN ab = matN_mul(&g_e[a], &g_e[b]);
                MatN ba = matN_mul(&g_e[b], &g_e[a]);
                sprintf(msg, "B: e_%d*e_%d = e_%d*e_%d", a+1,b+1,b+1,a+1);
                check(msg, matN_eq(&ab, &ba));
            }
        }

        /* Radical dimension (P1: must be 0 for simple) */
        rad_dim = compute_radical_dim();
        printf("  Radical dimension: %d\n", rad_dim);
        check("P1: W_{6,2} radical dim = 0 (simple)", rad_dim == 0);

        /* Sparsity */
        {
            int total_nz = 0;
            for (ei = 0; ei < N_TL; ei++)
                total_nz += matN_nonzero_count(&g_e[ei]);
            printf("  Generator sparsity B: %d/%d nonzero\n",
                   total_nz, N_TL * g_dim * g_dim);
        }

        /* Braid generators and relations */
        build_braid_generators();

        {
            int si;
            MatN id_mat = matN_identity();
            for (si = 0; si < N_TL; si++) {
                MatN bprod = matN_mul(&g_gen[2*si], &g_gen[2*si+1]);
                sprintf(msg, "B: s%d * s%d_inv = I", si+1, si+1);
                check(msg, matN_eq(&bprod, &id_mat));
            }
        }
        {
            int si;
            for (si = 0; si < N_TL - 1; si++) {
                MatN lhs = matN_mul(&g_gen[2*si], &g_gen[2*(si+1)]);
                MatN rhs;
                lhs = matN_mul(&lhs, &g_gen[2*si]);
                rhs = matN_mul(&g_gen[2*(si+1)], &g_gen[2*si]);
                rhs = matN_mul(&rhs, &g_gen[2*(si+1)]);
                sprintf(msg, "B: s%d*s%d*s%d = s%d*s%d*s%d",
                        si+1,si+2,si+1,si+2,si+1,si+2);
                check(msg, matN_eq(&lhs, &rhs));
            }
        }
        /* Hecke */
        {
            int si;
            Cyc8 a_m_ai = cyc8_sub(g_A, g_A_inv);
            MatN id_mat = matN_identity();
            for (si = 0; si < N_TL; si++) {
                MatN ns = matN_neg(&g_gen[2*si+1]);
                MatN lhs = matN_add(&g_gen[2*si], &ns);
                MatN ne = matN_neg(&g_e[si]);
                MatN ime = matN_add(&id_mat, &ne);
                MatN rhs = matN_scale(a_m_ai, &ime);
                sprintf(msg, "B: Hecke s%d", si+1);
                check(msg, matN_eq(&lhs, &rhs));
            }
        }
    }

    /* ============================================================
     * Phase 1: BFS Catalog -- Module A (W_{6,0})
     * ============================================================ */
    printf("\n=== Phase 1: BFS Catalog -- Module A: W_{6,0} (dim=5) ===\n\n");
    {
        int d;

        g_dim = 5;
        build_tl_a();
        build_braid_generators();
        build_catalog(12);

        g_save_cat_size = g_cat_size;
        g_save_dim = g_dim;
        sprintf(msg, "A: Catalog has %d entries", g_cat_size);
        check(msg, g_cat_size > 100);

        /* Save depth distribution and max_abs by depth */
        memset(g_save_depth_counts, 0, sizeof(g_save_depth_counts));
        memset(g_save_max_abs, 0, sizeof(g_save_max_abs));
        for (i = 0; i < g_cat_size; i++) {
            if (g_depth[i] < 20) {
                long v;
                g_save_depth_counts[g_depth[i]]++;
                v = matN_max_abs(&g_cat[i]);
                if (v > g_save_max_abs[g_depth[i]])
                    g_save_max_abs[g_depth[i]] = v;
            }
        }

        printf("\n  Depth distribution:\n");
        for (d = 0; d < 20 && g_save_depth_counts[d] > 0; d++)
            printf("    d=%d: %d entries\n", d, g_save_depth_counts[d]);

        printf("  Growth rate:\n");
        {
            int prev_c = 1;
            for (d = 1; d < 20 && g_save_depth_counts[d] > 0; d++) {
                printf("    d=%d: +%d (ratio: %.2f)\n",
                       d, g_save_depth_counts[d],
                       prev_c > 0 ? (double)g_save_depth_counts[d]
                                  / (double)prev_c : 0.0);
                prev_c = g_save_depth_counts[d];
            }
        }

        /* Save deep subcatalog for later comparison */
        {
            int deep_idx[1024];
            int nd = 0, bf, step, j;

            for (i = 0; i < g_cat_size && nd < 1024; i++)
                if (g_depth[i] >= 4) deep_idx[nd++] = i;

            printf("\n  Deep entries (d>=4): %d\n", nd);
            bf = nd < BF_SAVE ? nd : BF_SAVE;
            g_save_bf = bf;

            step = nd / bf;
            if (step < 1) step = 1;

            for (j = 0; j < bf; j++) {
                int src = deep_idx[j * step];
                g_save_cat[j] = g_cat[src];
                g_save_depth[j] = g_depth[src];
                g_save_writhe[j] = g_writhe[src];
            }

            /* XOR scan on deep subcatalog */
            {
                MatN orig_cat[BF_SAVE];
                int orig_d[BF_SAVE], orig_w[BF_SAVE];
                int orig_sz = g_cat_size;

                for (j = 0; j < bf; j++) {
                    orig_cat[j] = g_cat[j];
                    orig_d[j] = g_depth[j];
                    orig_w[j] = g_writhe[j];
                }
                for (j = 0; j < bf; j++) {
                    g_cat[j] = g_save_cat[j];
                    g_depth[j] = g_save_depth[j];
                    g_writhe[j] = g_save_writhe[j];
                }
                g_cat_size = bf;

                g_save_xor[0] = count_xor_bf(3, 128, bf);
                g_save_xor[1] = count_xor_bf(4, 128, bf);
                g_save_xor[2] = count_xor_bf(5, 128, bf);
                g_save_xor[3] = count_xor_bf(6, 128, bf);

                printf("  A deep bf=%d: XOR6=%d XOR8=%d XOR10=%d XOR12=%d\n",
                       bf, g_save_xor[0], g_save_xor[1],
                       g_save_xor[2], g_save_xor[3]);
                check("A: XOR scan completed", 1);

                /* Casimir for saved entries */
                for (j = 0; j < bf; j++) {
                    Cyc8 cas = matN_casimir(&g_save_cat[j]);
                    g_save_cas[j] = cyc8_l1(cas);
                }

                /* Restore */
                for (j = 0; j < bf; j++) {
                    g_cat[j] = orig_cat[j];
                    g_depth[j] = orig_d[j];
                    g_writhe[j] = orig_w[j];
                }
                g_cat_size = orig_sz;
            }
        }
    }

    /* ============================================================
     * Phase 2: BFS Catalog -- Module B (W_{6,2})
     * ============================================================ */
    printf("\n=== Phase 2: BFS Catalog -- Module B: W_{6,2} (dim=9) ===\n\n");
    {
        int d;
        int depth_counts_b[20];
        long max_abs_b[20];

        g_dim = 9;
        build_tl_b();
        build_braid_generators();
        build_catalog(12);

        sprintf(msg, "B: Catalog has %d entries", g_cat_size);
        check(msg, g_cat_size > 100);

        memset(depth_counts_b, 0, sizeof(depth_counts_b));
        memset(max_abs_b, 0, sizeof(max_abs_b));
        for (i = 0; i < g_cat_size; i++) {
            if (g_depth[i] < 20) {
                long v;
                depth_counts_b[g_depth[i]]++;
                v = matN_max_abs(&g_cat[i]);
                if (v > max_abs_b[g_depth[i]])
                    max_abs_b[g_depth[i]] = v;
            }
        }

        printf("\n  Depth distribution:\n");
        for (d = 0; d < 20 && depth_counts_b[d] > 0; d++)
            printf("    d=%d: %d entries\n", d, depth_counts_b[d]);

        printf("  Growth rate:\n");
        {
            int prev_c = 1;
            for (d = 1; d < 20 && depth_counts_b[d] > 0; d++) {
                printf("    d=%d: +%d (ratio: %.2f)\n",
                       d, depth_counts_b[d],
                       prev_c > 0 ? (double)depth_counts_b[d]
                                  / (double)prev_c : 0.0);
                prev_c = depth_counts_b[d];
            }
        }

        /* P2: Growth comparison (THE KEY TABLE) */
        printf("\n  Growth comparison (P2: strand law ~5x vs dim law ~9x):\n");
        printf("  %-6s  %10s  %10s  %10s  %10s\n",
               "depth", "A (W60)", "B (W62)", "A ratio", "B ratio");
        {
            int prev_a = 1, prev_b = 1;
            for (d = 0; d < 20; d++) {
                if (g_save_depth_counts[d] > 0 || depth_counts_b[d] > 0) {
                    printf("  d=%-4d  %10d  %10d",
                           d, g_save_depth_counts[d], depth_counts_b[d]);
                    if (d > 0 && prev_a > 0)
                        printf("  %9.2fx",
                               (double)g_save_depth_counts[d]/(double)prev_a);
                    else
                        printf("  %10s", "-");
                    if (d > 0 && prev_b > 0)
                        printf("  %9.2fx",
                               (double)depth_counts_b[d]/(double)prev_b);
                    else
                        printf("  %10s", "-");
                    printf("\n");
                    prev_a = g_save_depth_counts[d];
                    prev_b = depth_counts_b[d];
                }
            }
        }

        /* P5: Max abs comparison */
        printf("\n  Max coefficient magnitude by depth (P5):\n");
        printf("  %-6s  %10s  %10s\n", "depth", "A (W60)", "B (W62)");
        for (d = 0; d < 20; d++) {
            if (g_save_max_abs[d] > 0 || max_abs_b[d] > 0)
                printf("  d=%-4d  %10ld  %10ld\n",
                       d, g_save_max_abs[d], max_abs_b[d]);
        }
    }

    /* ============================================================
     * Phase 3: XOR + Casimir + Hub Analysis for B
     * (B catalog is live, g_dim=9)
     * ============================================================ */
    printf("\n=== Phase 3: XOR Comparison ===\n\n");
    {
        int deep_idx[1024];
        int nd = 0, bf, step, j;
        int xor_b[4];
        MatN orig_cat[BF_SAVE];
        int orig_d[BF_SAVE], orig_w[BF_SAVE];
        int orig_sz;
        static long cas_b[BF_SAVE];

        for (i = 0; i < g_cat_size && nd < 1024; i++)
            if (g_depth[i] >= 4) deep_idx[nd++] = i;

        bf = nd < BF_SAVE ? nd : BF_SAVE;
        step = nd / bf;
        if (step < 1) step = 1;

        /* Save and replace */
        orig_sz = g_cat_size;
        for (j = 0; j < bf; j++) {
            orig_cat[j] = g_cat[j];
            orig_d[j] = g_depth[j];
            orig_w[j] = g_writhe[j];
        }
        for (j = 0; j < bf; j++) {
            int src = deep_idx[j * step];
            g_cat[j] = g_cat[src];
            g_depth[j] = g_depth[src];
            g_writhe[j] = g_writhe[src];
        }
        g_cat_size = bf;

        printf("  B deep bf=%d (g_dim=%d):\n", bf, g_dim);
        xor_b[0] = count_xor_bf(3, 128, bf);
        printf("    XOR6=%d\n", xor_b[0]);
        xor_b[1] = count_xor_bf(4, 128, bf);
        printf("    XOR8=%d\n", xor_b[1]);
        xor_b[2] = count_xor_bf(5, 128, bf);
        printf("    XOR10=%d\n", xor_b[2]);
        xor_b[3] = count_xor_bf(6, 128, bf);
        printf("    XOR12=%d\n", xor_b[3]);

        printf("\n  XOR Capacity Comparison (k=128, deep bf=30):\n");
        printf("  %-8s  %10s  %10s\n", "XOR", "A (W60,d5)", "B (W62,d9)");
        printf("  %-8s  %10s  %10s\n", "--------", "----------", "----------");
        printf("  XOR6     %10d  %10d\n", g_save_xor[0], xor_b[0]);
        printf("  XOR8     %10d  %10d\n", g_save_xor[1], xor_b[1]);
        printf("  XOR10    %10d  %10d\n", g_save_xor[2], xor_b[2]);
        printf("  XOR12    %10d  %10d\n", g_save_xor[3], xor_b[3]);
        check("P3/P4: XOR comparison completed", 1);

        /* ============================================================
         * Phase 4: Casimir Analysis
         * ============================================================ */
        printf("\n=== Phase 4: Casimir Analysis ===\n\n");

        for (j = 0; j < bf; j++) {
            Cyc8 cas = matN_casimir(&g_cat[j]);
            cas_b[j] = cyc8_l1(cas);
        }

        /* Mean Casimir comparison */
        {
            long sum_a = 0, sum_b = 0;
            int lim = g_save_bf < bf ? g_save_bf : bf;
            for (j = 0; j < lim; j++) {
                sum_a += g_save_cas[j];
                sum_b += cas_b[j];
            }
            printf("  Mean |C_d| (bf=%d): A(C5)=%ld  B(C9)=%ld\n",
                   lim, sum_a / (long)lim, sum_b / (long)lim);
        }

        /* Casimir by XOR level */
        printf("\n  Casimir by XOR level:\n");
        printf("  %-8s  %8s  %12s  %12s  %8s\n",
               "XOR", "winners", "win_C", "ctrl_C", "ratio");

        /* XOR6 winners vs control for B */
        {
            int a0, a1, a2;
            int nw = 0, nc = 0;
            long wc = 0, cc = 0;
            int idx[8];
            for (a0 = 0; a0 < bf; a0++)
            for (a1 = a0+1; a1 < bf; a1++)
            for (a2 = a1+1; a2 < bf; a2++) {
                long tc;
                idx[0]=a0; idx[1]=a1; idx[2]=a2;
                tc = cas_b[a0]+cas_b[a1]+cas_b[a2];
                if (test_xor(idx, 3, 128)) {
                    nw++; wc += tc;
                } else if (nc < 500) {
                    nc++; cc += tc;
                }
            }
            if (nw > 0 && nc > 0)
                printf("  XOR6     %8d  %12ld  %12ld  %7.2fx\n",
                       nw, wc/(long)nw, cc/(long)nc,
                       cc>0?(double)(wc/(long)nw)/(double)(cc/(long)nc):0.0);
            else
                printf("  XOR6     %8d  (insufficient data)\n", nw);
        }

        check("P7: Casimir-by-XOR analysis completed", 1);

        /* ============================================================
         * Phase 5: Hub Anatomy (W_{6,2})
         * ============================================================ */
        printf("\n=== Phase 5: Hub Anatomy (W_{6,2}) ===\n\n");

        /* Entry frequency in XOR6 winners */
        printf("  Step 1: Entry frequency in XOR6 winners\n");
        {
            static int entry_freq[BF_SAVE];
            int n_win6 = 0;
            int hub_idx[20];
            int n_hubs = 0;
            int a0, a1, a2;
            int idx3[8];

            memset(entry_freq, 0, (size_t)bf * sizeof(int));

            for (a0 = 0; a0 < bf; a0++)
            for (a1 = a0+1; a1 < bf; a1++)
            for (a2 = a1+1; a2 < bf; a2++) {
                idx3[0]=a0; idx3[1]=a1; idx3[2]=a2;
                if (test_xor(idx3, 3, 128)) {
                    n_win6++;
                    entry_freq[a0]++;
                    entry_freq[a1]++;
                    entry_freq[a2]++;
                }
            }

            printf("  Total XOR6 winners: %d\n\n", n_win6);

            /* Frequency histogram */
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
                for (fi = 0; fi <= max_freq && fi < 200; fi++)
                    if (freq_hist[fi] > 0)
                        printf("  %-6d  %6d\n", fi, freq_hist[fi]);
            }

            /* Super-hubs (>33% of winners) */
            {
                int threshold = n_win6 > 0 ? n_win6 / 3 : 1;
                int fi;
                printf("\n  Super-hubs (freq > %d):\n", threshold);
                for (fi = 0; fi < bf && n_hubs < 20; fi++) {
                    if (entry_freq[fi] > threshold) {
                        hub_idx[n_hubs] = fi;
                        printf("    Hub %d: entry %d, freq=%d "
                               "(%.1f%%), d=%d, w=%d\n",
                               n_hubs, fi, entry_freq[fi],
                               n_win6 > 0 ?
                               100.0*(double)entry_freq[fi]
                                    /(double)n_win6 : 0.0,
                               g_depth[fi], g_writhe[fi]);
                        n_hubs++;
                    }
                }
            }

            sprintf(msg, "B: XOR6 super-hubs found: %d", n_hubs);
            check(msg, n_hubs > 0);

            /* Hub properties */
            if (n_hubs >= 1) {
                int hi;
                printf("\n  Step 2: Hub properties (B)\n");
                for (hi = 0; hi < n_hubs && hi < 6; hi++) {
                    int hidx = hub_idx[hi];
                    Cyc8 tr_h = matN_trace(&g_cat[hidx]);
                    Cyc8 c_h = matN_casimir(&g_cat[hidx]);
                    printf("    Hub %d (e%d, d=%d, w=%d): "
                           "tr=(%ld,%ld,%ld,%ld) |C|=%ld frob=%ld\n",
                           hi, hidx, g_depth[hidx], g_writhe[hidx],
                           tr_h.a, tr_h.b, tr_h.c, tr_h.d,
                           cyc8_l1(c_h), matN_frob_l1(&g_cat[hidx]));
                }
            }

            /* Hub commutators (P6: simple -> commuting?) */
            if (n_hubs >= 2) {
                int hi, hj;
                int n_comm = 0, n_zero_comm = 0;
                printf("\n  Step 3: Hub commutators (P6: simple->commuting?)\n");
                printf("  %-4s %-4s  %24s  %8s  %8s\n",
                       "i", "j", "trace", "|C|", "frob_L1");
                for (hi = 0; hi < n_hubs && hi < 6; hi++) {
                    for (hj = hi+1; hj < n_hubs && hj < 6; hj++) {
                        MatN ab, ba, cm;
                        Cyc8 tr_cm;
                        long f_l1;
                        ab = matN_mul(&g_cat[hub_idx[hi]],
                                      &g_cat[hub_idx[hj]]);
                        ba = matN_mul(&g_cat[hub_idx[hj]],
                                      &g_cat[hub_idx[hi]]);
                        cm = matN_sub(&ab, &ba);
                        tr_cm = matN_trace(&cm);
                        f_l1 = matN_frob_l1(&cm);
                        n_comm++;
                        if (f_l1 == 0) n_zero_comm++;
                        printf("  %-4d %-4d  "
                               "(%3ld,%3ld,%3ld,%3ld)  "
                               "%8ld  %8ld\n",
                               hi, hj,
                               tr_cm.a, tr_cm.b, tr_cm.c, tr_cm.d,
                               cyc8_l1(matN_casimir(&cm)), f_l1);
                    }
                }
                printf("  Commuting pairs: %d/%d\n",
                       n_zero_comm, n_comm);
                check("B: Hub commutator analysis completed", 1);
            }
        }

        /* Restore B catalog */
        for (j = 0; j < bf; j++) {
            g_cat[j] = orig_cat[j];
            g_depth[j] = orig_d[j];
            g_writhe[j] = orig_w[j];
        }
        g_cat_size = orig_sz;
    }

    /* ============================================================
     * Phase 6: A Hub Analysis (using saved entries)
     * ============================================================ */
    printf("\n=== Phase 6: A Hub Analysis (W_{6,0}) ===\n\n");
    {
        int bf_a = g_save_bf;
        MatN hold_cat[BF_SAVE];
        int hold_d[BF_SAVE], hold_w[BF_SAVE];
        int hold_sz = g_cat_size;
        int j;

        /* Switch to A dimension and replace catalog */
        g_dim = g_save_dim; /* 5 */

        for (j = 0; j < bf_a; j++) {
            hold_cat[j] = g_cat[j];
            hold_d[j] = g_depth[j];
            hold_w[j] = g_writhe[j];
        }
        for (j = 0; j < bf_a; j++) {
            g_cat[j] = g_save_cat[j];
            g_depth[j] = g_save_depth[j];
            g_writhe[j] = g_save_writhe[j];
        }
        g_cat_size = bf_a;

        /* XOR6 hub analysis for A */
        {
            static int ef[BF_SAVE];
            int n6 = 0;
            int hub_a[20];
            int nh = 0;
            int a0, a1, a2;
            int idx3[8];

            memset(ef, 0, (size_t)bf_a * sizeof(int));

            for (a0 = 0; a0 < bf_a; a0++)
            for (a1 = a0+1; a1 < bf_a; a1++)
            for (a2 = a1+1; a2 < bf_a; a2++) {
                idx3[0]=a0; idx3[1]=a1; idx3[2]=a2;
                if (test_xor(idx3, 3, 128)) {
                    n6++;
                    ef[a0]++;
                    ef[a1]++;
                    ef[a2]++;
                }
            }

            printf("  Total A XOR6 winners: %d\n", n6);

            /* Frequency distribution */
            {
                int freq_hist[200];
                int max_freq = 0, fi;
                memset(freq_hist, 0, sizeof(freq_hist));
                for (fi = 0; fi < bf_a; fi++) {
                    if (ef[fi] > max_freq) max_freq = ef[fi];
                    if (ef[fi] < 200) freq_hist[ef[fi]]++;
                }
                printf("  %-6s  %6s\n", "freq", "entries");
                for (fi = 0; fi <= max_freq && fi < 200; fi++)
                    if (freq_hist[fi] > 0)
                        printf("  %-6d  %6d\n", fi, freq_hist[fi]);
            }

            /* Super-hubs (>33%) */
            {
                int threshold = n6 > 0 ? n6 / 3 : 1;
                int fi;
                printf("\n  Super-hubs (freq > %d):\n", threshold);
                for (fi = 0; fi < bf_a && nh < 20; fi++) {
                    if (ef[fi] > threshold) {
                        hub_a[nh] = fi;
                        printf("    Hub %d: entry %d, freq=%d "
                            "(%.1f%%), d=%d, w=%d\n",
                            nh, fi, ef[fi],
                            n6 > 0 ? 100.0 * (double)ef[fi]
                                    / (double)n6 : 0.0,
                            g_depth[fi], g_writhe[fi]);
                        nh++;
                    }
                }
            }

            sprintf(msg, "A: XOR6 super-hubs: %d", nh);
            check(msg, nh > 0);

            /* Hub properties */
            if (nh >= 1) {
                int hi;
                printf("\n  Hub properties (A):\n");
                for (hi = 0; hi < nh && hi < 6; hi++) {
                    int hidx = hub_a[hi];
                    Cyc8 tr_h = matN_trace(&g_cat[hidx]);
                    Cyc8 c_h = matN_casimir(&g_cat[hidx]);
                    printf("    Hub %d (e%d, d=%d, w=%d): "
                           "tr=(%ld,%ld,%ld,%ld) |C|=%ld frob=%ld\n",
                           hi, hidx, g_depth[hidx], g_writhe[hidx],
                           tr_h.a, tr_h.b, tr_h.c, tr_h.d,
                           cyc8_l1(c_h), matN_frob_l1(&g_cat[hidx]));
                }
            }

            /* Hub commutators for A */
            if (nh >= 2) {
                int hi, hj;
                int n_comm = 0, n_zero_comm = 0;
                printf("\n  Hub commutators (P6: simple->commuting?)\n");
                printf("  %-4s %-4s  %24s  %8s  %8s\n",
                       "i", "j", "trace", "|C|", "frob_L1");
                for (hi = 0; hi < nh && hi < 6; hi++) {
                    for (hj = hi+1; hj < nh && hj < 6; hj++) {
                        MatN ab, ba, cm;
                        Cyc8 tr_cm;
                        long f_l1;
                        ab = matN_mul(&g_cat[hub_a[hi]],
                                      &g_cat[hub_a[hj]]);
                        ba = matN_mul(&g_cat[hub_a[hj]],
                                      &g_cat[hub_a[hi]]);
                        cm = matN_sub(&ab, &ba);
                        tr_cm = matN_trace(&cm);
                        f_l1 = matN_frob_l1(&cm);
                        n_comm++;
                        if (f_l1 == 0) n_zero_comm++;
                        printf("  %-4d %-4d  "
                            "(%3ld,%3ld,%3ld,%3ld)  "
                            "%8ld  %8ld\n",
                            hi, hj,
                            tr_cm.a, tr_cm.b, tr_cm.c, tr_cm.d,
                            cyc8_l1(matN_casimir(&cm)), f_l1);
                    }
                }
                printf("  Commuting pairs: %d/%d\n",
                       n_zero_comm, n_comm);
                check("A: Hub commutator analysis completed", 1);
            }
        }

        /* Restore */
        for (j = 0; j < bf_a; j++) {
            g_cat[j] = hold_cat[j];
            g_depth[j] = hold_d[j];
            g_writhe[j] = hold_w[j];
        }
        g_cat_size = hold_sz;
    }

    /* ============================================================
     * Phase 7: Subset-Hash Activation Test
     * ============================================================
     * Explorer hypothesis: dim-9 XOR inversion may be curse of
     * dimensionality in activation (324 vs 100 sign components).
     * Test: hash only a SUBSET of dim-9 components, compare to
     * dim-5 standard activation.
     *
     * Four activation variants on B (dim=9):
     *   full:    all 9x9x4 = 324 sign components (standard)
     *   sub5:    first 5x5 subblock = 100 components (matches dim-5)
     *   stride3: every 3rd of 81 entries x4 = ~108 components
     *   sub7:    first 7x7 subblock = 196 components (intermediate)
     * ============================================================ */
    printf("\n=== Phase 7: Subset-Hash Activation Test ===\n\n");
    {
        int deep_idx7[1024];
        int nd7 = 0, bf7, step7, j;
        int xfull[4], xs5[4], xs3[4], xs7[4];

        /* B catalog should still be live (Phase 6 restored it) */
        g_dim = 9;

        for (i = 0; i < g_cat_size && nd7 < 1024; i++)
            if (g_depth[i] >= 4) deep_idx7[nd7++] = i;

        bf7 = nd7 < BF_SAVE ? nd7 : BF_SAVE;
        step7 = nd7 / bf7;
        if (step7 < 1) step7 = 1;

        /* Save catalog head, replace with deep entries */
        {
            MatN hold7[BF_SAVE];
            int hold7_d[BF_SAVE], hold7_w[BF_SAVE];
            int hold7_sz = g_cat_size;

            for (j = 0; j < bf7; j++) {
                hold7[j] = g_cat[j];
                hold7_d[j] = g_depth[j];
                hold7_w[j] = g_writhe[j];
            }
            for (j = 0; j < bf7; j++) {
                int src = deep_idx7[j * step7];
                g_cat[j] = g_cat[src];
                g_depth[j] = g_depth[src];
                g_writhe[j] = g_writhe[src];
            }
            g_cat_size = bf7;

            /* (1) B full (standard, 324 comp) */
            g_activate = matN_activate;
            xfull[0] = count_xor_bf(3, 128, bf7);
            xfull[1] = count_xor_bf(4, 128, bf7);
            xfull[2] = count_xor_bf(5, 128, bf7);
            xfull[3] = count_xor_bf(6, 128, bf7);

            /* (2) B sub5 (first 5x5, 100 comp) */
            g_sub_dim = 5;
            g_activate = matN_activate_sub;
            xs5[0] = count_xor_bf(3, 128, bf7);
            xs5[1] = count_xor_bf(4, 128, bf7);
            xs5[2] = count_xor_bf(5, 128, bf7);
            xs5[3] = count_xor_bf(6, 128, bf7);

            /* (3) B stride3 (every 3rd, ~108 comp) */
            g_act_stride = 3;
            g_activate = matN_activate_stride;
            xs3[0] = count_xor_bf(3, 128, bf7);
            xs3[1] = count_xor_bf(4, 128, bf7);
            xs3[2] = count_xor_bf(5, 128, bf7);
            xs3[3] = count_xor_bf(6, 128, bf7);

            /* (4) B sub7 (first 7x7, 196 comp) */
            g_sub_dim = 7;
            g_activate = matN_activate_sub;
            xs7[0] = count_xor_bf(3, 128, bf7);
            xs7[1] = count_xor_bf(4, 128, bf7);
            xs7[2] = count_xor_bf(5, 128, bf7);
            xs7[3] = count_xor_bf(6, 128, bf7);

            g_activate = matN_activate; /* restore */

            /* Comparison table */
            printf("  Subset-Hash Comparison (k=128, deep bf=%d):\n", bf7);
            printf("  %-10s  %8s  %8s  %8s  %8s  %8s\n",
                   "Activation", "comp", "XOR6", "XOR8", "XOR10", "XOR12");
            printf("  %-10s  %8s  %8s  %8s  %8s  %8s\n",
                   "----------", "--------", "--------", "--------",
                   "--------", "--------");
            printf("  %-10s  %8d  %8d  %8d  %8d  %8d\n",
                   "A full", 100, g_save_xor[0], g_save_xor[1],
                   g_save_xor[2], g_save_xor[3]);
            printf("  %-10s  %8d  %8d  %8d  %8d  %8d\n",
                   "B full", 324, xfull[0], xfull[1],
                   xfull[2], xfull[3]);
            printf("  %-10s  %8d  %8d  %8d  %8d  %8d\n",
                   "B sub5", 100, xs5[0], xs5[1], xs5[2], xs5[3]);
            printf("  %-10s  %8s  %8d  %8d  %8d  %8d\n",
                   "B stride3", "~108", xs3[0], xs3[1],
                   xs3[2], xs3[3]);
            printf("  %-10s  %8d  %8d  %8d  %8d  %8d\n",
                   "B sub7", 196, xs7[0], xs7[1], xs7[2], xs7[3]);

            check("P7: Subset-hash activation test completed", 1);

            /* Key diagnostic: does sub5 beat A's full? */
            if (xs5[0] > g_save_xor[0])
                printf("\n  VERDICT: B@sub5 > A@full => "
                       "dim-9 algebra is richer, "
                       "activation granularity was the bottleneck\n");
            else if (xs5[0] < g_save_xor[0])
                printf("\n  VERDICT: B@sub5 < A@full => "
                       "dim-5 algebra has more XOR capacity "
                       "(not just activation artifact)\n");
            else
                printf("\n  VERDICT: B@sub5 == A@full => "
                       "tied at matched component count\n");

            /* Restore catalog */
            for (j = 0; j < bf7; j++) {
                g_cat[j] = hold7[j];
                g_depth[j] = hold7_d[j];
                g_writhe[j] = hold7_w[j];
            }
            g_cat_size = hold7_sz;
        }
    }

    /* ============================================================
     * Summary
     * ============================================================ */
    printf("\n=============================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);

    free(g_cat);
    return n_fail > 0 ? 1 : 0;
}
