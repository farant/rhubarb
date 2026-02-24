/*
 * KNOTAPEL DEMO 87: Null Indispensability at zeta_12
 * ===================================================
 *
 * Demo 84 proved bracket-null entries are computationally indispensable
 * at zeta_8: 9/24 null entries maintain 6 exclusive S2 directions that
 * no non-null entry reaches. Removing nulls collapses capacity.
 *
 * This demo extends that analysis to zeta_12 (infinite group, 4096
 * entries, 2043 directions) where 121 null entries (3%) occupy 67
 * null-only directions.
 *
 * Core question: are null states indispensable at zeta_12?
 * Answer: directionally YES (67 exclusive axes), computationally NO
 * (XOR capacity fully preserved without nulls). This is a regime
 * transition from zeta_8 where nulls are computationally critical.
 *
 * Phase 1: Catalog + direction partition (null-excl / shared / non-null)
 * Phase 2: Capacity impact (full vs null-removed)
 * Phase 3: Direction-stratified analysis
 * Phase 4: Depth dependence
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

/* ================================================================ */
/* Test infrastructure                                              */
/* ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *titulis, int ok) {
    if (ok) { printf("  PASS: %s\n", titulis); n_pass++; }
    else    { printf("  FAIL: %s\n", titulis); n_fail++; }
}

/* ================================================================ */
/* Quaternion                                                       */
/* ================================================================ */

typedef struct { double a, b, c, d; } Quat;

static Quat qmul(const Quat *p, const Quat *g) {
    Quat r;
    r.a = p->a*g->a - p->b*g->b - p->c*g->c - p->d*g->d;
    r.b = p->a*g->b + p->b*g->a + p->c*g->d - p->d*g->c;
    r.c = p->a*g->c - p->b*g->d + p->c*g->a + p->d*g->b;
    r.d = p->a*g->d + p->b*g->c - p->c*g->b + p->d*g->a;
    return r;
}

/* ================================================================ */
/* SU(2) generators                                                 */
/* ================================================================ */

static Quat q_gen[3];

static void init_su2(double half_angle) {
    double co = cos(half_angle), si = sin(half_angle);
    q_gen[1].a = co; q_gen[1].b = si; q_gen[1].c = 0; q_gen[1].d = 0;
    q_gen[2].a = co; q_gen[2].b = 0;  q_gen[2].c = 0; q_gen[2].d = -si;
}

/* ================================================================ */
/* Catalog with depth tracking                                      */
/* ================================================================ */

#define MAX_QCAT 4096

static Quat g_cat[MAX_QCAT];
static int g_depth[MAX_QCAT];
static int g_cat_size = 0;

#define MAX_ROUNDS 16
static int n_rounds = 0;

static int find_in_cat(const Quat *q, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (fabs(g_cat[i].a - q->a) < 1e-10 &&
            fabs(g_cat[i].b - q->b) < 1e-10 &&
            fabs(g_cat[i].c - q->c) < 1e-10 &&
            fabs(g_cat[i].d - q->d) < 1e-10)
            return i;
        if (fabs(g_cat[i].a + q->a) < 1e-10 &&
            fabs(g_cat[i].b + q->b) < 1e-10 &&
            fabs(g_cat[i].c + q->c) < 1e-10 &&
            fabs(g_cat[i].d + q->d) < 1e-10)
            return i;
    }
    return -1;
}

static void build_closure(int verbose) {
    Quat gens[4];
    int prev, i, gi, rd;

    g_cat_size = 0;
    n_rounds = 0;

    g_cat[0].a = 1; g_cat[0].b = 0; g_cat[0].c = 0; g_cat[0].d = 0;
    g_depth[0] = 0;
    g_cat_size = 1;

    gens[0] = q_gen[1];
    gens[1].a =  q_gen[1].a; gens[1].b = -q_gen[1].b;
    gens[1].c = -q_gen[1].c; gens[1].d = -q_gen[1].d;
    gens[2] = q_gen[2];
    gens[3].a =  q_gen[2].a; gens[3].b = -q_gen[2].b;
    gens[3].c = -q_gen[2].c; gens[3].d = -q_gen[2].d;

    for (gi = 0; gi < 4; gi++) {
        if (find_in_cat(&gens[gi], g_cat_size) < 0 &&
            g_cat_size < MAX_QCAT) {
            g_depth[g_cat_size] = 0;
            g_cat[g_cat_size++] = gens[gi];
        }
    }
    n_rounds = 1;
    if (verbose) printf("  Round 0: %d entries\n", g_cat_size);

    rd = 1;
    do {
        prev = g_cat_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                Quat prod = qmul(&g_cat[i], &gens[gi]);
                if (find_in_cat(&prod, g_cat_size) < 0) {
                    if (g_cat_size < MAX_QCAT) {
                        g_depth[g_cat_size] = rd;
                        g_cat[g_cat_size++] = prod;
                    }
                }
            }
        }
        if (g_cat_size > prev) {
            if (verbose)
                printf("  Round %d: %d entries (+%d)\n",
                       rd, g_cat_size, g_cat_size - prev);
            n_rounds++;
        }
        rd++;
    } while (g_cat_size > prev && n_rounds < MAX_ROUNDS);
}

/* ================================================================ */
/* Saved catalog                                                    */
/* ================================================================ */

static Quat saved_cat[MAX_QCAT];
static int saved_depth[MAX_QCAT];
static int saved_cat_size;

static void save_catalog(void) {
    memcpy(saved_cat, g_cat, sizeof(Quat) * (size_t)g_cat_size);
    memcpy(saved_depth, g_depth, sizeof(int) * (size_t)g_cat_size);
    saved_cat_size = g_cat_size;
}

static void restore_catalog(void) {
    memcpy(g_cat, saved_cat, sizeof(Quat) * (size_t)saved_cat_size);
    memcpy(g_depth, saved_depth, sizeof(int) * (size_t)saved_cat_size);
    g_cat_size = saved_cat_size;
}

/* ================================================================ */
/* Null classification                                              */
/* ================================================================ */

static int is_null[MAX_QCAT];
static int null_idx[MAX_QCAT];
static int nonnull_idx[MAX_QCAT];
static int n_null_total, n_nonnull_total;

static void classify_null(int cat_size) {
    int i;
    int ni = 0, nni = 0;
    for (i = 0; i < cat_size; i++) {
        double qa = g_cat[i].a;
        if (qa < 0) qa = -qa;
        is_null[i] = (qa < 1e-10) ? 1 : 0;
        if (is_null[i]) null_idx[ni++] = i;
        else nonnull_idx[nni++] = i;
    }
    n_null_total = ni;
    n_nonnull_total = nni;
}

/* ================================================================ */
/* Direction catalog                                                */
/* ================================================================ */

#define MAX_DIR 2048

static double g_dir[MAX_DIR][3];
static int g_nd = 0;

static void build_dirs(int cat_size) {
    int i, j;
    g_nd = 0;
    for (i = 0; i < cat_size; i++) {
        double qa = g_cat[i].a, qb = g_cat[i].b;
        double qc = g_cat[i].c, qd = g_cat[i].d;
        double nv, ax, ay, az;
        int found = 0;
        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        if (nv < 1e-12) continue;
        ax = qb/nv; ay = qc/nv; az = qd/nv;
        for (j = 0; j < g_nd; j++) {
            double d1 = fabs(g_dir[j][0]-ax) + fabs(g_dir[j][1]-ay) +
                         fabs(g_dir[j][2]-az);
            double d2 = fabs(g_dir[j][0]+ax) + fabs(g_dir[j][1]+ay) +
                         fabs(g_dir[j][2]+az);
            if (d1 < 1e-8 || d2 < 1e-8) { found = 1; break; }
        }
        if (!found && g_nd < MAX_DIR) {
            g_dir[g_nd][0] = ax; g_dir[g_nd][1] = ay; g_dir[g_nd][2] = az;
            g_nd++;
        }
    }
}

/* ================================================================ */
/* Direction partition: null-only / non-null-only / shared           */
/* ================================================================ */

static int dir_null_cnt[MAX_DIR];
static int dir_nonnull_cnt[MAX_DIR];

typedef struct {
    int total_dirs;
    int null_only;
    int nonnull_only;
    int shared;
} DirPartition;

static DirPartition compute_dir_partition(int cat_size) {
    DirPartition p;
    double dirs[MAX_DIR][3];
    int nd = 0;
    int i, j;

    memset(dir_null_cnt, 0, sizeof(dir_null_cnt));
    memset(dir_nonnull_cnt, 0, sizeof(dir_nonnull_cnt));

    for (i = 0; i < cat_size; i++) {
        double qa = g_cat[i].a, qb = g_cat[i].b;
        double qc = g_cat[i].c, qd = g_cat[i].d;
        double nv, ax, ay, az;
        int found = -1;

        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        if (nv < 1e-12) continue;
        ax = qb/nv; ay = qc/nv; az = qd/nv;

        for (j = 0; j < nd; j++) {
            double d1 = fabs(dirs[j][0]-ax) + fabs(dirs[j][1]-ay) +
                         fabs(dirs[j][2]-az);
            double d2 = fabs(dirs[j][0]+ax) + fabs(dirs[j][1]+ay) +
                         fabs(dirs[j][2]+az);
            if (d1 < 1e-8 || d2 < 1e-8) { found = j; break; }
        }
        if (found < 0 && nd < MAX_DIR) {
            found = nd;
            dirs[nd][0] = ax; dirs[nd][1] = ay; dirs[nd][2] = az;
            nd++;
        }
        if (found >= 0) {
            if (is_null[i]) dir_null_cnt[found]++;
            else dir_nonnull_cnt[found]++;
        }
    }

    p.total_dirs = nd;
    p.null_only = 0;
    p.nonnull_only = 0;
    p.shared = 0;
    for (i = 0; i < nd; i++) {
        if (dir_null_cnt[i] > 0 && dir_nonnull_cnt[i] > 0) p.shared++;
        else if (dir_null_cnt[i] > 0) p.null_only++;
        else p.nonnull_only++;
    }
    return p;
}

/* ================================================================ */
/* Voronoi + combined cell                                          */
/* ================================================================ */

static int vor_cell(double ax, double ay, double az) {
    int i, best = 0;
    double bd = -2.0;
    for (i = 0; i < g_nd; i++) {
        double dp = fabs(ax*g_dir[i][0] + ay*g_dir[i][1] + az*g_dir[i][2]);
        if (dp > bd) { bd = dp; best = i; }
    }
    return best;
}

static int combined_cell(double sa, double sb, double sc, double sd,
                         int k_sec) {
    double n = sqrt(sa*sa + sb*sb + sc*sc + sd*sd);
    double qa, nv, half_ang, ang;
    int sec, vor, n_vor;

    n_vor = g_nd + 1;
    if (n < 1e-12) return (k_sec - 1) * n_vor + g_nd;

    qa = sa / n;
    if (qa < 0) { qa = -qa; sb = -sb; sc = -sc; sd = -sd; }
    if (qa > 1.0) qa = 1.0;

    nv = sqrt(sb*sb + sc*sc + sd*sd) / n;
    half_ang = acos(qa);
    ang = 2.0 * half_ang * 180.0 / M_PI;
    sec = (int)(ang * (double)k_sec / 360.0);
    if (sec >= k_sec) sec = k_sec - 1;
    if (sec < 0) sec = 0;

    if (nv < 1e-12) {
        vor = g_nd;
    } else {
        double ax2, ay2, az2, inv;
        inv = 1.0 / (nv * n);
        ax2 = sb * inv; ay2 = sc * inv; az2 = sd * inv;
        vor = vor_cell(ax2, ay2, az2);
    }

    return sec * n_vor + vor;
}

/* ================================================================ */
/* XOR test with early conflict detection                           */
/* ================================================================ */

#define MAX_ACT 65536

static int cell_even[MAX_ACT], cell_odd[MAX_ACT];
static int touched_cells[MAX_ACT];

static int test_xor(const int *indices, int n_weights, int k_sec) {
    int n_inputs = 2 * n_weights;
    int n_masks, n_vor, n_cells;
    double wa[14], wb[14], wc[14], wd[14];
    int n_touched = 0;
    int mask, i, result = 1;

    n_masks = 1 << n_inputs;
    n_vor = g_nd + 1;
    n_cells = k_sec * n_vor;

    if (n_cells > MAX_ACT || n_inputs > 14) return 0;

    for (i = 0; i < n_weights; i++) {
        const Quat *q = &g_cat[indices[i]];
        wa[2*i]   =  q->a; wb[2*i]   =  q->b;
        wc[2*i]   =  q->c; wd[2*i]   =  q->d;
        wa[2*i+1] = -q->a; wb[2*i+1] = -q->b;
        wc[2*i+1] = -q->c; wd[2*i+1] = -q->d;
    }

    for (mask = 0; mask < n_masks; mask++) {
        double sa = 0, sb = 0, sc = 0, sd = 0;
        int par = 0, cell;

        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                sa += wa[i]; sb += wb[i]; sc += wc[i]; sd += wd[i];
                par ^= 1;
            }
        }

        cell = combined_cell(sa, sb, sc, sd, k_sec);

        if (cell_even[cell] == 0 && cell_odd[cell] == 0)
            touched_cells[n_touched++] = cell;

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
        cell_even[touched_cells[i]] = 0;
        cell_odd[touched_cells[i]] = 0;
    }
    return result;
}

/* ================================================================ */
/* Winner storage + capacity pipeline                               */
/* ================================================================ */

#define MAX_WIN 32
#define MAX_TUPLE 7

static int g_win[8][MAX_WIN][MAX_TUPLE];
static int g_win_k[8][MAX_WIN];
static int g_nwin[8];

static const int K_LADDER[] = {1, 6, 8, 10, 12, 16, 20, 24};
#define N_KLADDER 8

static void find_xor6(int bf_limit) {
    int ai, aj, ak, ki;
    int indices[3];

    g_nwin[3] = 0;
    for (ai = 0; ai < bf_limit && g_nwin[3] < MAX_WIN; ai++)
    for (aj = ai+1; aj < bf_limit && g_nwin[3] < MAX_WIN; aj++)
    for (ak = aj+1; ak < bf_limit && g_nwin[3] < MAX_WIN; ak++) {
        indices[0] = ai; indices[1] = aj; indices[2] = ak;
        for (ki = 0; ki < N_KLADDER; ki++) {
            int nv2 = g_nd + 1;
            int nc2 = K_LADDER[ki] * nv2;
            if (nc2 > MAX_ACT) continue;
            if (test_xor(indices, 3, K_LADDER[ki])) {
                g_win[3][g_nwin[3]][0] = ai;
                g_win[3][g_nwin[3]][1] = aj;
                g_win[3][g_nwin[3]][2] = ak;
                g_win_k[3][g_nwin[3]] = K_LADDER[ki];
                g_nwin[3]++;
                break;
            }
        }
    }
}

static void find_recursive(int level, int cat_limit) {
    int prev = level - 1;
    int wi, ae, ki, i, j;

    g_nwin[level] = 0;
    if (g_nwin[prev] == 0) return;

    for (wi = 0; wi < g_nwin[prev] && g_nwin[level] < MAX_WIN; wi++) {
        for (ae = 0; ae < cat_limit && g_nwin[level] < MAX_WIN; ae++) {
            int in_winner = 0, dup = 0;
            int indices[MAX_TUPLE];
            int pos;

            for (i = 0; i < prev; i++) {
                if (g_win[prev][wi][i] == ae) { in_winner = 1; break; }
            }
            if (in_winner) continue;

            pos = prev;
            for (i = 0; i < prev; i++) {
                if (ae < g_win[prev][wi][i]) { pos = i; break; }
            }
            for (i = 0; i < pos; i++) indices[i] = g_win[prev][wi][i];
            indices[pos] = ae;
            for (i = pos; i < prev; i++) indices[i+1] = g_win[prev][wi][i];

            for (j = 0; j < g_nwin[level]; j++) {
                int match = 1;
                for (i = 0; i < level; i++) {
                    if (g_win[level][j][i] != indices[i]) {
                        match = 0; break;
                    }
                }
                if (match) { dup = 1; break; }
            }
            if (dup) continue;

            for (ki = 0; ki < N_KLADDER; ki++) {
                int nv2 = g_nd + 1;
                int nc2 = K_LADDER[ki] * nv2;
                if (nc2 > MAX_ACT) continue;
                if (test_xor(indices, level, K_LADDER[ki])) {
                    memcpy(g_win[level][g_nwin[level]],
                           indices, (size_t)level * sizeof(int));
                    g_win_k[level][g_nwin[level]] = K_LADDER[ki];
                    g_nwin[level]++;
                    break;
                }
            }
        }
    }
}

typedef struct {
    int n_dirs;
    int xor6, xor8, xor10, xor12;
    int max_xor;
} CapResult;

static void find_capacity(int cat_size, int bf_limit, CapResult *r) {
    build_dirs(cat_size);
    r->n_dirs = g_nd;
    r->max_xor = 0;
    r->xor6 = 0; r->xor8 = 0; r->xor10 = 0; r->xor12 = 0;

    memset(g_nwin, 0, sizeof(g_nwin));

    printf("    XOR6 (bf %d)...", bf_limit); fflush(stdout);
    find_xor6(bf_limit);
    r->xor6 = g_nwin[3];
    printf(" %d\n", g_nwin[3]);

    if (g_nwin[3] > 0) {
        printf("    XOR8 (rec %d)...", cat_size); fflush(stdout);
        find_recursive(4, cat_size);
        r->xor8 = g_nwin[4];
        printf(" %d\n", g_nwin[4]);
    }

    if (g_nwin[4] > 0) {
        printf("    XOR10 (rec %d)...", cat_size); fflush(stdout);
        find_recursive(5, cat_size);
        r->xor10 = g_nwin[5];
        printf(" %d\n", g_nwin[5]);
    }

    if (g_nwin[5] > 0) {
        printf("    XOR12 (rec %d)...", cat_size); fflush(stdout);
        find_recursive(6, cat_size);
        r->xor12 = g_nwin[6];
        printf(" %d\n", g_nwin[6]);
    }

    if (r->xor6  > 0) r->max_xor = 6;
    if (r->xor8  > 0) r->max_xor = 8;
    if (r->xor10 > 0) r->max_xor = 10;
    if (r->xor12 > 0) r->max_xor = 12;
}

/* ================================================================ */
/* Sub-catalog loader                                               */
/* ================================================================ */

static void load_subset(const int *indices, int count) {
    int i;
    for (i = 0; i < count; i++) {
        g_cat[i] = saved_cat[indices[i]];
        g_depth[i] = saved_depth[indices[i]];
    }
    g_cat_size = count;
}

/* ================================================================ */
/* Phase 1: Catalog + Direction Partition                           */
/* ================================================================ */

static void phase1_catalog(double half_angle, const char *root_name,
                           int expected_size, int expected_dirs) {
    DirPartition p;
    char msg[256];
    int d;

    printf("\n=== Phase 1: %s Catalog ===\n\n", root_name);

    init_su2(half_angle);
    build_closure(1);
    printf("  Total: %d entries in %d rounds\n\n", g_cat_size, n_rounds);

    if (expected_size > 0) {
        sprintf(msg, "%s catalog size = %d", root_name, expected_size);
        check(msg, g_cat_size == expected_size);
    }

    save_catalog();
    classify_null(g_cat_size);

    printf("  Null entries: %d / %d (%.1f%%)\n",
           n_null_total, g_cat_size,
           100.0 * (double)n_null_total / (double)g_cat_size);
    printf("  Non-null entries: %d\n\n", n_nonnull_total);

    /* Depth breakdown */
    printf("  Depth | Total | Null | Non-null | Null%%\n");
    printf("  ------|-------|------|----------|------\n");
    for (d = 0; d <= n_rounds && d < 12; d++) {
        int total = 0, null_d = 0, i;
        for (i = 0; i < g_cat_size; i++) {
            if (g_depth[i] == d) {
                total++;
                if (is_null[i]) null_d++;
            }
        }
        if (total > 0) {
            printf("  %5d | %5d | %4d | %8d | %5.1f%%\n",
                   d, total, null_d, total - null_d,
                   100.0 * (double)null_d / (double)total);
        }
    }
    printf("\n");

    /* Direction partition */
    p = compute_dir_partition(g_cat_size);

    printf("  Directions: %d total\n", p.total_dirs);
    printf("  Null-only: %d, Non-null-only: %d, Shared: %d\n",
           p.null_only, p.nonnull_only, p.shared);
    printf("  Partition sum: %d (should = %d)\n\n",
           p.null_only + p.nonnull_only + p.shared, p.total_dirs);

    sprintf(msg, "%s direction partition sums correctly", root_name);
    check(msg, p.null_only + p.nonnull_only + p.shared == p.total_dirs);

    if (expected_dirs > 0) {
        sprintf(msg, "%s total directions = %d", root_name, expected_dirs);
        check(msg, p.total_dirs == expected_dirs);
    }

    sprintf(msg, "%s has null-only directions", root_name);
    check(msg, p.null_only > 0);
}

/* ================================================================ */
/* Phase 2: Capacity Impact                                        */
/* ================================================================ */

static void phase2_capacity(const char *root_name, int bf_limit) {
    CapResult full_cap, nonnull_cap;
    char msg[256];

    printf("\n=== Phase 2: %s Capacity Impact ===\n\n", root_name);

    /* Full catalog */
    printf("  --- Full catalog (%d entries) ---\n", saved_cat_size);
    restore_catalog();
    find_capacity(g_cat_size, bf_limit, &full_cap);
    printf("  Full: %d dirs, max_xor=%d\n",
           full_cap.n_dirs, full_cap.max_xor);
    printf("  XOR6=%d, XOR8=%d, XOR10=%d, XOR12=%d\n\n",
           full_cap.xor6, full_cap.xor8, full_cap.xor10, full_cap.xor12);

    /* Non-null only */
    printf("  --- Non-null only (%d entries) ---\n", n_nonnull_total);
    load_subset(nonnull_idx, n_nonnull_total);
    find_capacity(g_cat_size, bf_limit < n_nonnull_total ? bf_limit : n_nonnull_total,
                  &nonnull_cap);
    printf("  Non-null: %d dirs, max_xor=%d\n",
           nonnull_cap.n_dirs, nonnull_cap.max_xor);
    printf("  XOR6=%d, XOR8=%d, XOR10=%d, XOR12=%d\n\n",
           nonnull_cap.xor6, nonnull_cap.xor8, nonnull_cap.xor10, nonnull_cap.xor12);

    /* Comparison table */
    printf("  +------------------+------+------+--------+------+------+------+------+\n");
    printf("  | Catalog          | Size | Dirs | MaxXOR | XOR6 | XOR8 |XOR10 |XOR12 |\n");
    printf("  +------------------+------+------+--------+------+------+------+------+\n");
    printf("  | Full             | %4d | %4d | %6d | %4d | %4d | %4d | %4d |\n",
           saved_cat_size, full_cap.n_dirs, full_cap.max_xor,
           full_cap.xor6, full_cap.xor8, full_cap.xor10, full_cap.xor12);
    printf("  | Non-null only    | %4d | %4d | %6d | %4d | %4d | %4d | %4d |\n",
           n_nonnull_total, nonnull_cap.n_dirs, nonnull_cap.max_xor,
           nonnull_cap.xor6, nonnull_cap.xor8, nonnull_cap.xor10, nonnull_cap.xor12);
    printf("  +------------------+------+------+--------+------+------+------+------+\n\n");

    /* Direction loss */
    printf("  Directions lost: %d (%.1f%%)\n",
           full_cap.n_dirs - nonnull_cap.n_dirs,
           100.0 * (double)(full_cap.n_dirs - nonnull_cap.n_dirs)
                 / (double)full_cap.n_dirs);

    /* Capacity loss */
    if (full_cap.xor6 > 0 && nonnull_cap.xor6 < full_cap.xor6) {
        printf("  XOR6 winners lost: %d -> %d (%.1f%% loss)\n",
               full_cap.xor6, nonnull_cap.xor6,
               100.0 * (double)(full_cap.xor6 - nonnull_cap.xor6)
                     / (double)full_cap.xor6);
    }

    printf("\n");

    sprintf(msg, "%s full catalog XOR6 > 0", root_name);
    check(msg, full_cap.xor6 > 0);

    sprintf(msg, "%s non-null capacity <= full capacity", root_name);
    check(msg, nonnull_cap.max_xor <= full_cap.max_xor);

    if (nonnull_cap.max_xor < full_cap.max_xor) {
        printf("  --> REMOVING NULLS DESTROYS CAPACITY at %s\n", root_name);
        printf("  --> Null entries are INDISPENSABLE\n\n");
    } else if (nonnull_cap.xor6 < full_cap.xor6) {
        printf("  --> Max XOR level preserved but FEWER winners at %s\n",
               root_name);
        printf("  --> Null entries provide computational diversity\n\n");
    } else {
        printf("  --> Capacity fully preserved without nulls at %s\n",
               root_name);
    }
}

/* ================================================================ */
/* Phase 3: Direction-stratified analysis                           */
/* ================================================================ */

static void phase3_direction_analysis(const char *root_name) {
    int i;
    int null_only_entries = 0, shared_entries = 0, nonnull_only_entries = 0;

    printf("\n=== Phase 3: %s Direction-Stratified Analysis ===\n\n",
           root_name);

    /* Count entries in each direction category */
    /* Uses dir_null_cnt/dir_nonnull_cnt from last compute_dir_partition */
    restore_catalog();
    {
        DirPartition p = compute_dir_partition(g_cat_size);

        for (i = 0; i < p.total_dirs; i++) {
            if (dir_null_cnt[i] > 0 && dir_nonnull_cnt[i] == 0)
                null_only_entries += dir_null_cnt[i];
            else if (dir_null_cnt[i] == 0 && dir_nonnull_cnt[i] > 0)
                nonnull_only_entries += dir_nonnull_cnt[i];
            else {
                shared_entries += dir_null_cnt[i] + dir_nonnull_cnt[i];
            }
        }

        printf("  Entry distribution by direction type:\n");
        printf("    Null-only directions: %d dirs, %d entries\n",
               p.null_only, null_only_entries);
        printf("    Shared directions: %d dirs, %d entries\n",
               p.shared, shared_entries);
        printf("    Non-null-only directions: %d dirs, %d entries\n\n",
               p.nonnull_only, nonnull_only_entries);

        if (p.null_only > 0) {
            printf("  Null-only direction density: %.1f entries/dir\n",
                   (double)null_only_entries / (double)p.null_only);
        }
        if (p.shared > 0) {
            printf("  Shared direction density: %.1f entries/dir\n",
                   (double)shared_entries / (double)p.shared);
        }
        if (p.nonnull_only > 0) {
            printf("  Non-null-only direction density: %.1f entries/dir\n",
                   (double)nonnull_only_entries / (double)p.nonnull_only);
        }
        printf("\n");

        /* Half-angle distribution for null entries */
        printf("  Half-angle distribution (null entries):\n");
        {
            double ha_vals[64];
            int ha_counts[64];
            int n_ha = 0;
            int j;

            for (i = 0; i < g_cat_size; i++) {
                double qa, nv, ha;
                int found = 0;
                if (!is_null[i]) continue;

                qa = g_cat[i].a;
                if (qa < 0) qa = -qa;
                nv = sqrt(g_cat[i].b*g_cat[i].b + g_cat[i].c*g_cat[i].c +
                          g_cat[i].d*g_cat[i].d);
                ha = atan2(nv, qa) * 180.0 / M_PI;

                for (j = 0; j < n_ha; j++) {
                    if (fabs(ha_vals[j] - ha) < 0.1) {
                        ha_counts[j]++;
                        found = 1;
                        break;
                    }
                }
                if (!found && n_ha < 64) {
                    ha_vals[n_ha] = ha;
                    ha_counts[n_ha] = 1;
                    n_ha++;
                }
            }

            printf("    Distinct half-angles: %d\n", n_ha);
            for (i = 0; i < n_ha && i < 20; i++) {
                printf("    ha=%.2f deg: %d entries\n",
                       ha_vals[i], ha_counts[i]);
            }
            if (n_ha > 20) printf("    ... (%d more)\n", n_ha - 20);
        }
        printf("\n");
    }
}

/* ================================================================ */
/* Phase 4: Depth dependence                                       */
/* ================================================================ */

static void phase4_depth(const char *root_name) {
    int d, i;

    printf("\n=== Phase 4: %s Depth Dependence ===\n\n", root_name);

    restore_catalog();
    classify_null(g_cat_size);

    printf("  Depth | Total | Null | NullFrac | CumNull | CumTotal | CumFrac\n");
    printf("  ------|-------|------|----------|---------|----------|--------\n");
    {
        int cum_null = 0, cum_total = 0;
        for (d = 0; d <= n_rounds && d < 12; d++) {
            int total = 0, null_d = 0;
            for (i = 0; i < g_cat_size; i++) {
                if (g_depth[i] == d) {
                    total++;
                    if (is_null[i]) null_d++;
                }
            }
            if (total > 0) {
                cum_null += null_d;
                cum_total += total;
                printf("  %5d | %5d | %4d | %7.1f%% | %7d | %8d | %6.1f%%\n",
                       d, total, null_d,
                       100.0 * (double)null_d / (double)total,
                       cum_null, cum_total,
                       100.0 * (double)cum_null / (double)cum_total);
            }
        }
    }
    printf("\n");
}

/* ================================================================ */
/* Main                                                             */
/* ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 87: Null Indispensability at zeta_12\n");
    printf("===================================================\n");

    /* ============================================================ */
    /* Control: zeta_8 (should match D84: 6/4/3 partition)          */
    /* ============================================================ */
    phase1_catalog(M_PI / 4.0, "zeta_8", 24, 13);
    phase2_capacity("zeta_8", 24);

    {
        DirPartition p8;
        restore_catalog();
        classify_null(g_cat_size);
        p8 = compute_dir_partition(g_cat_size);
        check("zeta_8 null-only dirs = 6", p8.null_only == 6);
        check("zeta_8 non-null-only dirs = 4", p8.nonnull_only == 4);
        check("zeta_8 shared dirs = 3", p8.shared == 3);
    }

    /* ============================================================ */
    /* Main event: zeta_12                                          */
    /* ============================================================ */
    phase1_catalog(M_PI / 6.0, "zeta_12", 4096, 0);
    phase2_capacity("zeta_12", 100);
    phase3_direction_analysis("zeta_12");
    phase4_depth("zeta_12");

    /* ============================================================ */
    /* Cross-root comparison                                        */
    /* ============================================================ */
    printf("\n=== Cross-Root Comparison ===\n\n");
    {
        DirPartition p8, p12;
        int z8_null, z8_size, z12_null, z12_size;

        init_su2(M_PI / 4.0);
        build_closure(0);
        z8_size = g_cat_size;
        classify_null(g_cat_size);
        z8_null = n_null_total;
        p8 = compute_dir_partition(g_cat_size);

        init_su2(M_PI / 6.0);
        build_closure(0);
        z12_size = g_cat_size;
        classify_null(g_cat_size);
        z12_null = n_null_total;
        p12 = compute_dir_partition(g_cat_size);

        printf("  +----------+------+------+-------+------+---------+---------+--------+\n");
        printf("  | Root     | Size | Null | Null%% | Dirs | NullDir | NNulDir | Shared |\n");
        printf("  +----------+------+------+-------+------+---------+---------+--------+\n");
        printf("  | zeta_8   | %4d | %4d | %5.1f | %4d | %7d | %7d | %6d |\n",
               z8_size, z8_null,
               100.0 * (double)z8_null / (double)z8_size,
               p8.total_dirs, p8.null_only, p8.nonnull_only, p8.shared);
        printf("  | zeta_12  | %4d | %4d | %5.1f | %4d | %7d | %7d | %6d |\n",
               z12_size, z12_null,
               100.0 * (double)z12_null / (double)z12_size,
               p12.total_dirs, p12.null_only, p12.nonnull_only, p12.shared);
        printf("  +----------+------+------+-------+------+---------+---------+--------+\n\n");
    }

    /* ============================================================ */
    /* Final results                                                */
    /* ============================================================ */
    printf("\n===================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
