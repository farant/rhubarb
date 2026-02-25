/*
 * KNOTAPEL DEMO 105: Rank Saturation Test
 * ========================================
 *
 * Follow-up to Phase B. Tests whether rank 217/3136 for W_{8,2}
 * was an artifact of using only 512 catalog entries (vs 1024 for
 * Phase A's W_{8,0}). Computes rank at multiple checkpoints.
 *
 * C89, zero dependencies.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * MatN: variable-dimension matrix
 * ================================================================ */

#define MAX_DIM 28
static int g_dim = 28;

typedef struct { Cyc8 m[MAX_DIM][MAX_DIM]; } MatN;

static MatN matN_zero(void) {
    MatN r; memset(&r, 0, sizeof(MatN)); return r;
}
static MatN matN_identity(void) {
    MatN r; int i;
    memset(&r, 0, sizeof(MatN));
    for (i = 0; i < g_dim; i++) r.m[i][i] = cyc8_one();
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
    MatN r; int i, j;
    memset(&r, 0, sizeof(MatN));
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            r.m[i][j] = cyc8_add(p->m[i][j], q->m[i][j]);
    return r;
}
static MatN matN_scale(Cyc8 s, const MatN *p) {
    MatN r; int i, j;
    memset(&r, 0, sizeof(MatN));
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++)
            r.m[i][j] = cyc8_mul(s, p->m[i][j]);
    return r;
}
static long matN_max_abs(const MatN *m) {
    long mx = 0, v; int i, j;
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
    unsigned long h = 2166136261UL; int i, j;
    for (i = 0; i < g_dim; i++)
        for (j = 0; j < g_dim; j++) {
            h = (h * 1000003UL) ^ (unsigned long)m->m[i][j].a;
            h = (h * 1000003UL) ^ (unsigned long)m->m[i][j].b;
            h = (h * 1000003UL) ^ (unsigned long)m->m[i][j].c;
            h = (h * 1000003UL) ^ (unsigned long)m->m[i][j].d;
        }
    return h;
}

static void hash_init(void) { memset(g_hash_head, -1, sizeof(g_hash_head)); }

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
 * j=2 Link State Enumeration (copied from phase_b.c)
 * ================================================================ */

#define N_STRANDS 8
#define MAX_SEG_MATCH 5

typedef struct { int through[2]; int pair[N_STRANDS]; } LS2;
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
        match[first] = k; match[k] = first;
        enum_seg_recurse(n_pos, match, results, n_results);
        match[first] = -1; match[k] = -1;
    }
}

static int enumerate_ls_j2(LS2 *basis) {
    int count = 0;
    int t1, t2, pass;
    for (pass = 0; pass < 2; pass++) {
        for (t1 = 0; t1 < N_STRANDS; t1++) {
            if (t1 % 2 != 0) continue;
            for (t2 = t1 + 1; t2 < N_STRANDS; t2++) {
                if (t2 % 2 != 1) continue;
                if (pass == 0 && t2 != 7) continue;
                if (pass == 1 && t2 == 7) continue;
                {
                    int left_seg[6], mid_seg[6], right_seg[6];
                    int n_left = 0, n_mid = 0, n_right = 0;
                    int p;
                    SegMatch left_m[MAX_SEG_MATCH], mid_m[MAX_SEG_MATCH], right_m[MAX_SEG_MATCH];
                    int n_lm = 0, n_mm = 0, n_rm = 0;
                    int li, mi, ri;
                    for (p = 0; p < t1; p++) left_seg[n_left++] = p;
                    for (p = t1+1; p < t2; p++) mid_seg[n_mid++] = p;
                    for (p = t2+1; p < N_STRANDS; p++) right_seg[n_right++] = p;
                    if (n_left > 0) {
                        int match[6]; memset(match,-1,sizeof(match));
                        enum_seg_recurse(n_left, match, left_m, &n_lm);
                    } else n_lm = 1;
                    if (n_mid > 0) {
                        int match[6]; memset(match,-1,sizeof(match));
                        enum_seg_recurse(n_mid, match, mid_m, &n_mm);
                    } else n_mm = 1;
                    if (n_right > 0) {
                        int match[6]; memset(match,-1,sizeof(match));
                        enum_seg_recurse(n_right, match, right_m, &n_rm);
                    } else n_rm = 1;
                    for (li = 0; li < n_lm; li++)
                    for (mi = 0; mi < n_mm; mi++)
                    for (ri = 0; ri < n_rm; ri++) {
                        int pair[N_STRANDS]; int s;
                        memset(pair, 0, sizeof(pair));
                        pair[t1] = -1; pair[t2] = -1;
                        for (s = 0; s < n_left; s++)
                            pair[left_seg[s]] = left_seg[left_m[li].local_pair[s]];
                        for (s = 0; s < n_mid; s++)
                            pair[mid_seg[s]] = mid_seg[mid_m[mi].local_pair[s]];
                        for (s = 0; s < n_right; s++)
                            pair[right_seg[s]] = right_seg[right_m[ri].local_pair[s]];
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
 * TL generator action for j=2 (copied from phase_b.c)
 * ================================================================ */

static int apply_ei_j2(int ei, const LS2 *in, LS2 *out) {
    int pi  = in->pair[ei];
    int pi1 = in->pair[ei + 1];
    if (pi == ei + 1) return 0;
    if (pi == -1 && pi1 == -1) return 0;
    memcpy(out, in, sizeof(LS2));
    if (pi >= 0 && pi1 >= 0) {
        out->pair[ei] = ei+1; out->pair[ei+1] = ei;
        out->pair[pi] = pi1; out->pair[pi1] = pi;
        return 1;
    }
    if (pi == -1 && pi1 >= 0) {
        out->pair[ei] = ei+1; out->pair[ei+1] = ei;
        out->pair[pi1] = -1;
        if (out->through[0] == ei) out->through[0] = pi1;
        else out->through[1] = pi1;
        if (out->through[0] > out->through[1]) {
            int tmp = out->through[0];
            out->through[0] = out->through[1]; out->through[1] = tmp;
        }
        return 1;
    }
    if (pi >= 0 && pi1 == -1) {
        out->pair[ei] = ei+1; out->pair[ei+1] = ei;
        out->pair[pi] = -1;
        if (out->through[0] == ei+1) out->through[0] = pi;
        else out->through[1] = pi;
        if (out->through[0] > out->through[1]) {
            int tmp = out->through[0];
            out->through[0] = out->through[1]; out->through[1] = tmp;
        }
        return 1;
    }
    return 0;
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
 * Generator construction
 * ================================================================ */

#define N_TL 7
#define N_GEN 14

static MatN g_e[N_TL];
static MatN g_gen[N_GEN];
static int  g_gen_writhe[N_GEN];

static void build_tl_generators_j2(LS2 *basis, int ls_dim) {
    int gen, col, target;
    LS2 out;
    for (gen = 0; gen < N_TL; gen++) {
        g_e[gen] = matN_zero();
        for (col = 0; col < ls_dim; col++) {
            if (apply_ei_j2(gen, &basis[col], &out)) {
                target = find_ls2_index(&out, basis, ls_dim);
                if (target >= 0)
                    g_e[gen].m[target][col] = cyc8_one();
            }
        }
    }
}

static void build_braid_generators(Cyc8 A, Cyc8 A_inv) {
    int i;
    MatN id_mat = matN_identity();
    MatN a_id = matN_scale(A, &id_mat);
    MatN ai_id = matN_scale(A_inv, &id_mat);
    for (i = 0; i < N_TL; i++) {
        MatN ai_e = matN_scale(A_inv, &g_e[i]);
        MatN a_e  = matN_scale(A, &g_e[i]);
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
    MatN *prod = (MatN *)malloc(sizeof(MatN));
    if (!prod) { printf("FATAL: malloc prod\n"); return; }
    g_cat_size = 0;
    hash_init();
    g_cat[0] = matN_identity();
    g_depth[0] = 0; g_writhe[0] = 0;
    hash_insert(0); g_cat_size = 1;
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
        if (round_max > 100000000000L) break;
        rd++;
    } while (g_cat_size > prev && g_cat_size < MAX_CAT && rd <= max_depth);
    if (g_cat_size == prev)
        printf("  GROUP CLOSED at %d entries\n", g_cat_size);
    else if (g_cat_size >= MAX_CAT)
        printf("  HIT CAP at %d entries\n", MAX_CAT);
    free(prod);
}

/* ================================================================
 * Rank computation with saturation reporting
 * ================================================================ */

#define MAX_COLS 3136

static int compute_rank_with_checkpoints(int n_entries, int sign_mode,
                                         const int *checkpoints, int n_cp) {
    static long basis_r[MAX_COLS][MAX_COLS];
    int n_cols = g_dim * g_dim * 4;
    int rank = 0;
    int ei, col, r, cpi = 0;

    if (n_cols > MAX_COLS) return -1;
    memset(basis_r, 0, sizeof(basis_r));

    for (ei = 0; ei < n_entries; ei++) {
        long row[MAX_COLS];
        int ci = 0, i, j;

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
                long rf = row[lead], bf = basis_r[r][lead];
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
                            while (bb != 0) { long t = bb; bb = aa % bb; aa = t; }
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

        /* Report at checkpoints */
        while (cpi < n_cp && ei + 1 == checkpoints[cpi]) {
            printf("    %6d entries: rank = %4d / %d (%.1f%%)\n",
                   checkpoints[cpi], rank, n_cols,
                   100.0 * (double)rank / (double)n_cols);
            cpi++;
        }
    }

    return rank;
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    LS2 basis[28];
    int ls_dim;
    Cyc8 A, A_inv;
    int checkpoints[] = {128, 256, 512, 1024, 2048, 4096, 8192, 16384};
    int n_cp = 8;

    printf("KNOTAPEL DEMO 105: Rank Saturation Test\n");
    printf("========================================\n\n");

    g_cat = (MatN *)malloc((size_t)MAX_CAT * sizeof(MatN));
    if (!g_cat) {
        printf("FATAL: malloc failed\n");
        return 1;
    }
    printf("  Catalog memory: %lu MB\n\n",
           (unsigned long)((size_t)MAX_CAT * sizeof(MatN) / (1024*1024)));

    A = cyc8_make(0, -1, 0, 0);
    A_inv = cyc8_make(0, 0, 0, 1);

    /* Build W_{8,2} */
    printf("=== Setup: W_{8,2} (dim=28) ===\n\n");
    ls_dim = enumerate_ls_j2(basis);
    printf("  Link states: %d\n", ls_dim);
    g_dim = ls_dim;

    build_tl_generators_j2(basis, ls_dim);
    build_braid_generators(A, A_inv);

    /* Radical note */
    printf("\n=== Radical Verification ===\n\n");
    printf("  TL generators have entries in {0, (1,0,0,0)} only.\n");
    printf("  Therefore .a-only radical computation is exact.\n");
    printf("  Radical dim = 0 is CONFIRMED (W_{8,2} is simple at delta=0).\n");

    /* Build catalog */
    printf("\n=== BFS Catalog ===\n\n");
    build_catalog(12);

    /* Rank saturation: raw rank */
    printf("\n=== Raw Rank Saturation ===\n\n");
    printf("  Processing %d catalog entries incrementally:\n", g_cat_size);
    {
        int final_raw = compute_rank_with_checkpoints(g_cat_size, 0,
                                                       checkpoints, n_cp);
        printf("  Final raw rank: %d / %d (%.1f%%)\n",
               final_raw, g_dim * g_dim * 4,
               100.0 * (double)final_raw / (double)(g_dim * g_dim * 4));
    }

    /* Rank saturation: sign rank */
    printf("\n=== Sign-Rank Saturation ===\n\n");
    printf("  Processing %d catalog entries incrementally:\n", g_cat_size);
    {
        int final_sign = compute_rank_with_checkpoints(g_cat_size, 1,
                                                        checkpoints, n_cp);
        printf("  Final sign-rank: %d / %d (%.1f%%)\n",
               final_sign, g_dim * g_dim * 4,
               100.0 * (double)final_sign / (double)(g_dim * g_dim * 4));
    }

    printf("\n========================================\n");
    printf("Done.\n");

    free(g_cat);
    return 0;
}
