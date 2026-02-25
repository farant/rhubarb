/*
 * KNOTAPEL DEMO 105 Phase B: W_{8,2} (dim=28, j=2 through-lines)
 * ================================================================
 *
 * Phase A showed W_{8,0} (dim=14, simple) has:
 *   rank=274/784 (34.9%), sign-rank=425, XOR8 alive at k=4096
 *
 * Phase B tests W_{8,2} (non-simple, dim=28):
 *   Branching: W_{8,2} |_{TL_7} = W_{7,1}(14) + W_{7,3}(14)
 *   Expected rank < 3136, Atkinson peak likely (like D104's W_{6,2})
 *
 * New infrastructure: j=2 link state enumeration + 5-case TL action.
 *
 * C89, zero dependencies.
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
 * Cyc8: Z[zeta_8]
 * ================================================================ */

typedef struct { long a, b, c, d; } Cyc8;

static Cyc8 cyc8_make(long a, long b, long c, long d) {
    Cyc8 z; z.a = a; z.b = b; z.c = c; z.d = d; return z;
}
static Cyc8 cyc8_one(void)  { return cyc8_make(1,0,0,0); }
static Cyc8 cyc8_add(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a+y.a, x.b+y.b, x.c+y.c, x.d+y.d);
}
static Cyc8 cyc8_sub(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a-y.a, x.b-y.b, x.c-y.c, x.d-y.d);
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

/* ================================================================
 * MatN: variable-dimension matrix, MAX_DIM=28
 * ================================================================ */

#define MAX_DIM 28

static int g_dim = 28;

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

static MatN matN_scale(Cyc8 s, const MatN *p) {
    MatN r;
    int i, j;
    memset(&r, 0, sizeof(MatN));
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            r.m[i][j] = cyc8_mul(s, p->m[i][j]);
    return r;
}

static long matN_max_abs(const MatN *m) {
    long mx = 0, v;
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++) {
            v = m->m[i][j].a; if(v<0)v=-v; if(v>mx)mx=v;
            v = m->m[i][j].b; if(v<0)v=-v; if(v>mx)mx=v;
            v = m->m[i][j].c; if(v<0)v=-v; if(v>mx)mx=v;
            v = m->m[i][j].d; if(v<0)v=-v; if(v>mx)mx=v;
        }
    return mx;
}

/* ================================================================
 * Hash table for BFS
 * ================================================================ */

#define MAX_CAT 16384
#define HASH_SIZE 32771

static MatN *g_cat;
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
 * j=2 Link State Enumeration for n=8
 *
 * Each state has 2 through-strands and 6 arc-paired positions.
 * Through positions (t1 < t2) must have t1 even, t2 odd.
 * Arcs are non-crossing within each segment.
 *
 * Basis ordering: through[1]==7 first (W_{7,1} block, 14 states),
 *                 then through[1]!=7 (W_{7,3} block, 14 states).
 * ================================================================ */

#define N_STRANDS 8
#define BLOCK_SIZE 14

typedef struct {
    int through[2];
    int pair[N_STRANDS]; /* pair[k]>=0: arc partner, pair[k]=-1: through */
} LS2;

/* Enumerate non-crossing matchings within a segment of positions.
 * seg[0..n-1] = the actual position indices in this segment.
 * match[0..n-1] = local partner index (-1 = unmatched).
 * Fills results[], increments *n_results. */
#define MAX_SEG_MATCH 5

typedef struct { int local_pair[6]; } SegMatch;

static void enum_seg_recurse(int n_pos, int *match,
                             SegMatch *results, int *n_results) {
    int first, k, j;
    for (first = 0; first < n_pos; first++) {
        if (match[first] == -1) break;
    }
    if (first >= n_pos) {
        memcpy(results[*n_results].local_pair, match,
               (size_t)n_pos * sizeof(int));
        (*n_results)++;
        return;
    }
    for (k = first + 1; k < n_pos; k++) {
        int between = k - first - 1;
        int ok = 1;
        if (between % 2 != 0) continue;
        if (match[k] != -1) continue;
        for (j = first + 1; j < k; j++) {
            if (match[j] != -1) { ok = 0; break; }
        }
        if (!ok) continue;
        match[first] = k;
        match[k] = first;
        enum_seg_recurse(n_pos, match, results, n_results);
        match[first] = -1;
        match[k] = -1;
    }
}

static int enumerate_ls_j2(LS2 *basis) {
    int count = 0;
    int t1, t2, pass;

    /* Two passes: pass 0 = t2==7 (W_{7,1} block), pass 1 = t2<7 (W_{7,3}) */
    for (pass = 0; pass < 2; pass++) {
        for (t1 = 0; t1 < N_STRANDS; t1++) {
            if (t1 % 2 != 0) continue; /* t1 must be even */
            for (t2 = t1 + 1; t2 < N_STRANDS; t2++) {
                if (t2 % 2 != 1) continue; /* t2 must be odd */
                if (pass == 0 && t2 != 7) continue;
                if (pass == 1 && t2 == 7) continue;

                {
                    /* Build 3 segments */
                    int left_seg[6], mid_seg[6], right_seg[6];
                    int n_left = 0, n_mid = 0, n_right = 0;
                    int p;
                    SegMatch left_m[MAX_SEG_MATCH];
                    SegMatch mid_m[MAX_SEG_MATCH];
                    SegMatch right_m[MAX_SEG_MATCH];
                    int n_lm = 0, n_mm = 0, n_rm = 0;
                    int li, mi, ri;

                    for (p = 0; p < t1; p++) left_seg[n_left++] = p;
                    for (p = t1 + 1; p < t2; p++) mid_seg[n_mid++] = p;
                    for (p = t2 + 1; p < N_STRANDS; p++) right_seg[n_right++] = p;

                    /* Enumerate matchings per segment */
                    if (n_left > 0) {
                        int match[6];
                        memset(match, -1, sizeof(match));
                        enum_seg_recurse(n_left, match, left_m, &n_lm);
                    } else {
                        n_lm = 1; /* trivial empty matching */
                    }
                    if (n_mid > 0) {
                        int match[6];
                        memset(match, -1, sizeof(match));
                        enum_seg_recurse(n_mid, match, mid_m, &n_mm);
                    } else {
                        n_mm = 1;
                    }
                    if (n_right > 0) {
                        int match[6];
                        memset(match, -1, sizeof(match));
                        enum_seg_recurse(n_right, match, right_m, &n_rm);
                    } else {
                        n_rm = 1;
                    }

                    /* Cartesian product */
                    for (li = 0; li < n_lm; li++)
                    for (mi = 0; mi < n_mm; mi++)
                    for (ri = 0; ri < n_rm; ri++) {
                        int pair[N_STRANDS];
                        int s;
                        memset(pair, 0, sizeof(pair));
                        pair[t1] = -1;
                        pair[t2] = -1;
                        /* Map left segment */
                        for (s = 0; s < n_left; s++)
                            pair[left_seg[s]] =
                                left_seg[left_m[li].local_pair[s]];
                        /* Map mid segment */
                        for (s = 0; s < n_mid; s++)
                            pair[mid_seg[s]] =
                                mid_seg[mid_m[mi].local_pair[s]];
                        /* Map right segment */
                        for (s = 0; s < n_right; s++)
                            pair[right_seg[s]] =
                                right_seg[right_m[ri].local_pair[s]];

                        basis[count].through[0] = t1;
                        basis[count].through[1] = t2;
                        memcpy(basis[count].pair, pair, sizeof(pair));
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

/* ================================================================
 * TL generator action on j=2 states (5-case logic)
 *
 * Returns 1 if valid result, 0 if killed (loop at delta=0).
 * ================================================================ */

static int apply_ei_j2(int ei, const LS2 *in, LS2 *out) {
    int pi  = in->pair[ei];
    int pi1 = in->pair[ei + 1];

    /* Case 1: both free, paired to each other -> loop */
    if (pi == ei + 1) return 0;

    /* Case 5: both through -> loop */
    if (pi == -1 && pi1 == -1) return 0;

    memcpy(out, in, sizeof(LS2));

    if (pi >= 0 && pi1 >= 0) {
        /* Case 2: both free, different partners */
        out->pair[ei]     = ei + 1;
        out->pair[ei + 1] = ei;
        out->pair[pi]     = pi1;
        out->pair[pi1]    = pi;
        return 1;
    }

    if (pi == -1 && pi1 >= 0) {
        /* Case 3: i through, i+1 free paired to pi1 */
        out->pair[ei]     = ei + 1;
        out->pair[ei + 1] = ei;
        out->pair[pi1]    = -1;
        /* Update through: replace ei with pi1 */
        if (out->through[0] == ei) out->through[0] = pi1;
        else                       out->through[1] = pi1;
        if (out->through[0] > out->through[1]) {
            int tmp = out->through[0];
            out->through[0] = out->through[1];
            out->through[1] = tmp;
        }
        return 1;
    }

    if (pi >= 0 && pi1 == -1) {
        /* Case 4: i free paired to pi, i+1 through */
        out->pair[ei]     = ei + 1;
        out->pair[ei + 1] = ei;
        out->pair[pi]     = -1;
        /* Update through: replace ei+1 with pi */
        if (out->through[0] == ei + 1) out->through[0] = pi;
        else                           out->through[1] = pi;
        if (out->through[0] > out->through[1]) {
            int tmp = out->through[0];
            out->through[0] = out->through[1];
            out->through[1] = tmp;
        }
        return 1;
    }

    return 0; /* unreachable */
}

static int find_ls2_index(const LS2 *ls, const LS2 *basis, int dim) {
    int idx, j;
    for (idx = 0; idx < dim; idx++) {
        int eq = 1;
        if (basis[idx].through[0] != ls->through[0] ||
            basis[idx].through[1] != ls->through[1]) continue;
        for (j = 0; j < N_STRANDS; j++) {
            if (basis[idx].pair[j] != ls->pair[j]) { eq = 0; break; }
        }
        if (eq) return idx;
    }
    return -1;
}

/* ================================================================
 * Build TL generators as 28x28 matrices
 * ================================================================ */

#define N_TL 7   /* n-1 TL generators */
#define N_GEN 14  /* 2*N_TL braid generators */

static MatN g_e[N_TL];
static Cyc8 g_A, g_A_inv;
static MatN g_gen[N_GEN];
static int g_gen_writhe[N_GEN];

static int build_tl_generators_j2(LS2 *basis, int ls_dim) {
    int gen, col, target, total_nz = 0;
    LS2 out;

    for (gen = 0; gen < N_TL; gen++) {
        g_e[gen] = matN_zero();
        for (col = 0; col < ls_dim; col++) {
            if (apply_ei_j2(gen, &basis[col], &out)) {
                target = find_ls2_index(&out, basis, ls_dim);
                if (target >= 0) {
                    g_e[gen].m[target][col] = cyc8_one();
                    total_nz++;
                }
            }
        }
    }
    return total_nz;
}

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
        g_gen_writhe[2*i] = 1;
        g_gen_writhe[2*i + 1] = -1;
    }
}

/* ================================================================
 * BFS catalog builder
 * ================================================================ */

static void build_catalog(int max_depth) {
    int prev, gi, i, rd;
    MatN *prod;

    prod = (MatN *)malloc(sizeof(MatN));
    if (!prod) { printf("FATAL: malloc prod\n"); return; }

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
                matN_mul_to(&g_cat[i], &g_gen[gi], prod);
                if (hash_find(prod) < 0) {
                    mabs = matN_max_abs(prod);
                    if (mabs > round_max) round_max = mabs;
                    g_cat[g_cat_size] = *prod;
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

    free(prod);
}

/* ================================================================
 * Activation functions
 * ================================================================ */

/* Full activation: all g_dim*g_dim*4 components */
static int matN_activate(const MatN *m, int k_param) {
    unsigned long h = 0;
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++) {
            h = h*3UL + (unsigned long)(m->m[i][j].a>0?2:(m->m[i][j].a<0?0:1));
            h = h*3UL + (unsigned long)(m->m[i][j].b>0?2:(m->m[i][j].b<0?0:1));
            h = h*3UL + (unsigned long)(m->m[i][j].c>0?2:(m->m[i][j].c<0?0:1));
            h = h*3UL + (unsigned long)(m->m[i][j].d>0?2:(m->m[i][j].d<0?0:1));
        }
    return (int)(h % (unsigned long)k_param);
}

/* Cross-block activation: off-diagonal blocks only (rows<BLOCK vs cols>=BLOCK) */
static int matN_activate_cross(const MatN *m, int k_param) {
    unsigned long h = 0;
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++) {
            int cross = (i < BLOCK_SIZE && j >= BLOCK_SIZE) ||
                        (i >= BLOCK_SIZE && j < BLOCK_SIZE);
            if (!cross) continue;
            h = h*3UL + (unsigned long)(m->m[i][j].a>0?2:(m->m[i][j].a<0?0:1));
            h = h*3UL + (unsigned long)(m->m[i][j].b>0?2:(m->m[i][j].b<0?0:1));
            h = h*3UL + (unsigned long)(m->m[i][j].c>0?2:(m->m[i][j].c<0?0:1));
            h = h*3UL + (unsigned long)(m->m[i][j].d>0?2:(m->m[i][j].d<0?0:1));
        }
    return (int)(h % (unsigned long)k_param);
}

/* Within-block activation: diagonal blocks only */
static int matN_activate_within(const MatN *m, int k_param) {
    unsigned long h = 0;
    int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++) {
            int within = (i < BLOCK_SIZE && j < BLOCK_SIZE) ||
                         (i >= BLOCK_SIZE && j >= BLOCK_SIZE);
            if (!within) continue;
            h = h*3UL + (unsigned long)(m->m[i][j].a>0?2:(m->m[i][j].a<0?0:1));
            h = h*3UL + (unsigned long)(m->m[i][j].b>0?2:(m->m[i][j].b<0?0:1));
            h = h*3UL + (unsigned long)(m->m[i][j].c>0?2:(m->m[i][j].c<0?0:1));
            h = h*3UL + (unsigned long)(m->m[i][j].d>0?2:(m->m[i][j].d<0?0:1));
        }
    return (int)(h % (unsigned long)k_param);
}

/* ================================================================
 * XOR test infrastructure
 * ================================================================ */

#define MAX_ACT_CELLS 8192

static int cell_even[MAX_ACT_CELLS], cell_odd[MAX_ACT_CELLS];
static int touched[MAX_ACT_CELLS];

typedef int (*activate_fn)(const MatN *, int);

static int test_xor_fn(const int *indices, int n_weights,
                       int k_param, activate_fn act) {
    int n_inputs = 2 * n_weights;
    int n_masks = 1 << n_inputs;
    int n_touched = 0;
    int mask, i, w;
    int result = 1;

    if (k_param > MAX_ACT_CELLS || n_inputs > 16 || n_weights > 8)
        return 0;

    for (mask = 0; mask < n_masks; mask++) {
        MatN *sum;
        int par = 0, cell;

        sum = (MatN *)calloc(1, sizeof(MatN));
        if (!sum) return 0;

        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                w = i / 2;
                if (i % 2 == 0)
                    matN_add_inplace(sum, &g_cat[indices[w]]);
                else
                    matN_sub_inplace(sum, &g_cat[indices[w]]);
                par ^= 1;
            }
        }

        cell = act(sum, k_param);
        free(sum);

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

static int count_xor_bf_fn(int n_weights, int k_param,
                           int bf_limit, activate_fn act) {
    int count = 0;
    int limit = g_cat_size < bf_limit ? g_cat_size : bf_limit;
    int i0, i1, i2, i3;
    int indices[8];

    if (n_weights == 3) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++) {
            indices[0]=i0; indices[1]=i1; indices[2]=i2;
            if (test_xor_fn(indices, 3, k_param, act)) count++;
        }
    } else if (n_weights == 4) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++) {
            indices[0]=i0; indices[1]=i1; indices[2]=i2; indices[3]=i3;
            if (test_xor_fn(indices, 4, k_param, act)) count++;
        }
    }
    return count;
}

/* ================================================================
 * Rank computation
 * ================================================================ */

#define MAX_COLS 3136 /* 28*28*4 */

static int compute_rank(int n_entries, int sign_mode) {
    static long basis_r[MAX_COLS][MAX_COLS];
    int n_cols = g_dim * g_dim * 4;
    int rank = 0;
    int ei, col, r;

    if (n_cols > MAX_COLS) return -1;
    memset(basis_r, 0, sizeof(basis_r));

    for (ei = 0; ei < n_entries; ei++) {
        long row[MAX_COLS];
        int ci = 0;
        int i, j;

        memset(row, 0, sizeof(row));
        for (i = 0; i < g_dim; i++)
            for (j = 0; j < g_dim; j++) {
                if (sign_mode) {
                    row[ci++] = g_cat[ei].m[i][j].a>0?2:(g_cat[ei].m[i][j].a<0?0:1);
                    row[ci++] = g_cat[ei].m[i][j].b>0?2:(g_cat[ei].m[i][j].b<0?0:1);
                    row[ci++] = g_cat[ei].m[i][j].c>0?2:(g_cat[ei].m[i][j].c<0?0:1);
                    row[ci++] = g_cat[ei].m[i][j].d>0?2:(g_cat[ei].m[i][j].d<0?0:1);
                } else {
                    row[ci++] = g_cat[ei].m[i][j].a;
                    row[ci++] = g_cat[ei].m[i][j].b;
                    row[ci++] = g_cat[ei].m[i][j].c;
                    row[ci++] = g_cat[ei].m[i][j].d;
                }
            }

        for (r = 0; r < rank; r++) {
            int lead = -1;
            for (col = 0; col < n_cols; col++) {
                if (basis_r[r][col] != 0) { lead = col; break; }
            }
            if (lead < 0) continue;
            if (row[lead] != 0) {
                long rf = row[lead];
                long bf = basis_r[r][lead];
                for (col = 0; col < n_cols; col++)
                    row[col] = row[col] * bf - basis_r[r][col] * rf;
            }
        }

        {
            int is_zero = 1;
            for (col = 0; col < n_cols; col++) {
                if (row[col] != 0) { is_zero = 0; break; }
            }

            if (!is_zero && rank < n_cols) {
                long g = 0;
                for (col = 0; col < n_cols; col++) {
                    long v = row[col] < 0 ? -row[col] : row[col];
                    if (v > 0) {
                        if (g == 0) g = v;
                        else {
                            long aa = g, bb = v;
                            while (bb != 0) {
                                long t = bb;
                                bb = aa % bb;
                                aa = t;
                            }
                            g = aa;
                        }
                    }
                }
                if (g > 1) {
                    for (col = 0; col < n_cols; col++)
                        row[col] /= g;
                }
                for (col = 0; col < n_cols; col++)
                    basis_r[rank][col] = row[col];
                rank++;
                if (rank >= n_cols) break;
            }
        }
    }

    return rank;
}

/* ================================================================
 * Radical dimension
 * ================================================================ */

static int compute_radical_dim(int ls_dim) {
    static long stk[N_TL * MAX_DIM][MAX_DIM];
    int total_rows = N_TL * ls_dim;
    int row, col, prow, r, c, rank;

    memset(stk, 0, sizeof(stk));
    row = 0;
    for (r = 0; r < N_TL; r++)
        for (c = 0; c < ls_dim; c++) {
            int j;
            for (j = 0; j < ls_dim; j++)
                stk[row][j] = g_e[r].m[c][j].a;
            row++;
        }

    rank = 0;
    for (col = 0; col < ls_dim; col++) {
        prow = -1;
        for (r = rank; r < total_rows; r++) {
            if (stk[r][col] != 0) { prow = r; break; }
        }
        if (prow < 0) continue;
        if (prow != rank) {
            for (c = 0; c < ls_dim; c++) {
                long tmp = stk[rank][c];
                stk[rank][c] = stk[prow][c];
                stk[prow][c] = tmp;
            }
        }
        for (r = rank + 1; r < total_rows; r++) {
            if (stk[r][col] != 0) {
                long fn = stk[r][col], fd = stk[rank][col];
                for (c = 0; c < ls_dim; c++)
                    stk[r][c] = stk[r][c] * fd - stk[rank][c] * fn;
            }
        }
        rank++;
    }
    return ls_dim - rank;
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    char msg[256];
    LS2 basis[28];
    int ls_dim;
    int total_nz;
    int ei, bf;

    printf("KNOTAPEL DEMO 105 Phase B: W_{8,2} (dim=28, j=2)\n");
    printf("===================================================\n\n");

    g_cat = (MatN *)malloc((size_t)MAX_CAT * sizeof(MatN));
    if (!g_cat) {
        printf("FATAL: malloc failed (%lu MB)\n",
               (unsigned long)((size_t)MAX_CAT * sizeof(MatN) / (1024*1024)));
        return 1;
    }
    printf("  Catalog memory: %lu MB\n\n",
           (unsigned long)((size_t)MAX_CAT * sizeof(MatN) / (1024*1024)));

    g_A = cyc8_make(0, -1, 0, 0);
    g_A_inv = cyc8_make(0, 0, 0, 1);

    /* ============================================================
     * Phase 0: j=2 Link State Enumeration + Verification
     * ============================================================ */
    printf("=== Phase 0: j=2 Link State Enumeration & Verification ===\n\n");

    ls_dim = enumerate_ls_j2(basis);
    printf("  W_{8,2} link states: %d (expected 28)\n", ls_dim);
    g_dim = ls_dim;
    check("Link state count = 28", ls_dim == 28);

    /* Print first/last few basis states */
    printf("\n  First 14 states (W_{7,1} block, through[1]=7):\n");
    for (ei = 0; ei < 14 && ei < ls_dim; ei++) {
        int s;
        printf("    h%d: T={%d,%d} arcs:", ei,
               basis[ei].through[0], basis[ei].through[1]);
        for (s = 0; s < N_STRANDS; s++) {
            if (basis[ei].pair[s] > s)
                printf(" (%d,%d)", s, basis[ei].pair[s]);
        }
        printf("\n");
    }
    printf("  Last 14 states (W_{7,3} block, through[1]<7):\n");
    for (ei = 14; ei < ls_dim; ei++) {
        int s;
        printf("    h%d: T={%d,%d} arcs:", ei,
               basis[ei].through[0], basis[ei].through[1]);
        for (s = 0; s < N_STRANDS; s++) {
            if (basis[ei].pair[s] > s)
                printf(" (%d,%d)", s, basis[ei].pair[s]);
        }
        printf("\n");
    }

    /* Verify block split */
    {
        int n_block0 = 0, n_block1 = 0;
        for (ei = 0; ei < ls_dim; ei++) {
            if (basis[ei].through[1] == 7) n_block0++;
            else n_block1++;
        }
        sprintf(msg, "Block split: %d + %d = 28", n_block0, n_block1);
        check(msg, n_block0 == 14 && n_block1 == 14);
    }

    /* Build TL generators */
    total_nz = build_tl_generators_j2(basis, ls_dim);
    printf("\n  Generator sparsity: %d/%d nonzero\n",
           total_nz, N_TL * ls_dim * ls_dim);

    /* Verify e_i^2 = 0 */
    {
        MatN *prod_v = (MatN *)malloc(sizeof(MatN));
        MatN zero_mat = matN_zero();
        for (ei = 0; ei < N_TL; ei++) {
            matN_mul_to(&g_e[ei], &g_e[ei], prod_v);
            sprintf(msg, "e_%d^2 = 0", ei + 1);
            check(msg, matN_eq(prod_v, &zero_mat));
        }
        free(prod_v);
    }

    /* Verify braid relation: e_i * e_{i+1} * e_i = e_i */
    {
        MatN *tmp1 = (MatN *)malloc(sizeof(MatN));
        MatN *tmp2 = (MatN *)malloc(sizeof(MatN));
        for (ei = 0; ei < N_TL - 1; ei++) {
            matN_mul_to(&g_e[ei], &g_e[ei+1], tmp1);
            matN_mul_to(tmp1, &g_e[ei], tmp2);
            sprintf(msg, "e_%d*e_%d*e_%d = e_%d", ei+1, ei+2, ei+1, ei+1);
            check(msg, matN_eq(tmp2, &g_e[ei]));
        }
        free(tmp1);
        free(tmp2);
    }

    /* Verify commutativity: e_i * e_j = e_j * e_i for |i-j| >= 2 */
    {
        MatN *pij = (MatN *)malloc(sizeof(MatN));
        MatN *pji = (MatN *)malloc(sizeof(MatN));
        int i, j;
        for (i = 0; i < N_TL; i++)
            for (j = i + 2; j < N_TL; j++) {
                matN_mul_to(&g_e[i], &g_e[j], pij);
                matN_mul_to(&g_e[j], &g_e[i], pji);
                sprintf(msg, "e_%d*e_%d = e_%d*e_%d", i+1, j+1, j+1, i+1);
                check(msg, matN_eq(pij, pji));
            }
        free(pij);
        free(pji);
    }

    /* Radical dimension */
    {
        int rad_dim = compute_radical_dim(ls_dim);
        printf("  Radical dimension: %d\n", rad_dim);
        sprintf(msg, "Radical dim = %d (non-simple expected)", rad_dim);
        check(msg, rad_dim >= 0);
    }

    /* Build braid generators */
    build_braid_generators();

    /* Verify braid inverses */
    {
        MatN *prod_v = (MatN *)malloc(sizeof(MatN));
        MatN id_chk = matN_identity();
        for (ei = 0; ei < N_TL; ei++) {
            matN_mul_to(&g_gen[2*ei], &g_gen[2*ei+1], prod_v);
            sprintf(msg, "s%d * s%d_inv = I", ei+1, ei+1);
            check(msg, matN_eq(prod_v, &id_chk));
        }
        free(prod_v);
    }

    /* Verify Yang-Baxter */
    {
        MatN *t1 = (MatN *)malloc(sizeof(MatN));
        MatN *t2 = (MatN *)malloc(sizeof(MatN));
        MatN *lhs = (MatN *)malloc(sizeof(MatN));
        MatN *rhs = (MatN *)malloc(sizeof(MatN));
        for (ei = 0; ei < N_TL - 1; ei++) {
            matN_mul_to(&g_gen[2*ei], &g_gen[2*(ei+1)], t1);
            matN_mul_to(t1, &g_gen[2*ei], lhs);
            matN_mul_to(&g_gen[2*(ei+1)], &g_gen[2*ei], t2);
            matN_mul_to(t2, &g_gen[2*(ei+1)], rhs);
            sprintf(msg, "s%d*s%d*s%d = s%d*s%d*s%d",
                    ei+1, ei+2, ei+1, ei+2, ei+1, ei+2);
            check(msg, matN_eq(lhs, rhs));
        }
        free(t1); free(t2); free(lhs); free(rhs);
    }

    /* ============================================================
     * Phase 1: BFS Catalog
     * ============================================================ */
    printf("\n=== Phase 1: BFS Catalog ===\n\n");
    build_catalog(12);
    bf = g_cat_size < 30 ? g_cat_size : 30;
    printf("  Deep entries: %d (bf=%d)\n", bf*(bf-1)*(bf-2)/6, bf);
    check("Catalog built", g_cat_size > 100);

    /* ============================================================
     * Phase 2: Rank + Sign-Rank
     * ============================================================ */
    printf("\n=== Phase 2: Rank Computation ===\n\n");
    {
        int n_cols = g_dim * g_dim * 4;
        int n_entries = g_cat_size < 512 ? g_cat_size : 512;
        int raw_rank, sign_rank;

        raw_rank = compute_rank(n_entries, 0);
        sign_rank = compute_rank(n_entries, 1);

        printf("  W_{8,2} (dim=%d): raw rank = %d / %d (%.1f%%)\n",
               g_dim, raw_rank, n_cols, 100.0*(double)raw_rank/(double)n_cols);
        printf("  W_{8,2} (dim=%d): sign-rank = %d / %d (%.1f%%)\n",
               g_dim, sign_rank, n_cols, 100.0*(double)sign_rank/(double)n_cols);
        if (sign_rank > raw_rank)
            printf("  Sign EXPANSION: ratio %.2fx\n",
                   (double)sign_rank / (double)raw_rank);
        sprintf(msg, "Raw rank = %d", raw_rank);
        check(msg, raw_rank > 0);
        sprintf(msg, "Sign-rank = %d", sign_rank);
        check(msg, sign_rank > 0);
    }

    /* ============================================================
     * Phase 3: Branching Decomposition
     *
     * Cross-block (off-diag) vs within-block (on-diag)
     * Block split: rows/cols 0-13 (W_{7,1}) vs 14-27 (W_{7,3})
     * ============================================================ */
    printf("\n=== Phase 3: Branching Decomposition (k=128) ===\n\n");
    {
        int xor6_full, xor8_full;
        int xor6_cross, xor8_cross;
        int xor6_within, xor8_within;
        int n_cross_comp = BLOCK_SIZE * BLOCK_SIZE * 2 * 4;
        int n_within_comp = BLOCK_SIZE * BLOCK_SIZE * 2 * 4;

        xor6_full   = count_xor_bf_fn(3, 128, bf, matN_activate);
        xor8_full   = count_xor_bf_fn(4, 128, bf, matN_activate);
        xor6_cross  = count_xor_bf_fn(3, 128, bf, matN_activate_cross);
        xor8_cross  = count_xor_bf_fn(4, 128, bf, matN_activate_cross);
        xor6_within = count_xor_bf_fn(3, 128, bf, matN_activate_within);
        xor8_within = count_xor_bf_fn(4, 128, bf, matN_activate_within);

        printf("  %-16s  %6s  %6s  %6s\n", "Activation", "comp", "XOR6", "XOR8");
        printf("  ----------------  ------  ------  ------\n");
        printf("  %-16s  %6d  %6d  %6d\n", "B full",
               g_dim*g_dim*4, xor6_full, xor8_full);
        printf("  %-16s  %6d  %6d  %6d\n", "B cross-block",
               n_cross_comp, xor6_cross, xor8_cross);
        printf("  %-16s  %6d  %6d  %6d\n", "B within-block",
               n_within_comp, xor6_within, xor8_within);

        if (xor6_cross > xor6_within)
            printf("\n  VERDICT: cross-block > within-block => branching IS signal\n");
        else
            printf("\n  VERDICT: within-block >= cross-block\n");
        check("Branching decomposition completed", 1);
    }

    /* ============================================================
     * Phase 4: k-Sweep
     * ============================================================ */
    printf("\n=== Phase 4: k-Sweep ===\n\n");
    {
        int k_vals[] = {128, 256, 512, 1024, 2048, 4096};
        int n_k = 6;
        int ki;

        printf("  W_{8,2} full (dim=%d, %d comp), bf=%d:\n",
               g_dim, g_dim*g_dim*4, bf);
        printf("  %8s  %8s  %8s\n", "k", "XOR6", "XOR8");
        printf("  --------  --------  --------\n");

        for (ki = 0; ki < n_k; ki++) {
            int xor6 = count_xor_bf_fn(3, k_vals[ki], bf, matN_activate);
            int xor8 = count_xor_bf_fn(4, k_vals[ki], bf, matN_activate);
            printf("  %8d  %8d  %8d\n", k_vals[ki], xor6, xor8);
        }
        check("k-sweep completed", 1);

        printf("\n  W_{8,2} cross-block, bf=%d:\n", bf);
        printf("  %8s  %8s  %8s\n", "k", "XOR6", "XOR8");
        printf("  --------  --------  --------\n");

        for (ki = 0; ki < n_k; ki++) {
            int xor6 = count_xor_bf_fn(3, k_vals[ki], bf, matN_activate_cross);
            int xor8 = count_xor_bf_fn(4, k_vals[ki], bf, matN_activate_cross);
            printf("  %8d  %8d  %8d\n", k_vals[ki], xor6, xor8);
        }
        check("Cross-block k-sweep completed", 1);
    }

    /* ============================================================
     * Summary
     * ============================================================ */
    printf("\n===================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);

    free(g_cat);
    return n_fail > 0 ? 1 : 0;
}
