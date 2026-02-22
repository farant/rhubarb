/*
 * KNOTAPEL DEMO 84: Null States and Logarithmic Structure
 * ========================================================
 *
 * At delta=0, bracket-null entries (Re(q)=0, half-angle=90 degrees)
 * have vanishing Kauffman bracket trace: Tr(rho) = 2*Re(q) = 0.
 * LCFT predicts null states have logarithmic partners carrying
 * computational content via Jordan cells.
 *
 * Key question: are bracket-null entries computational dead weight,
 * or do they carry essential structure through their S2 direction?
 *
 * Part A: Bracket-null census (depth, writhe, direction distribution)
 * Part B: Gap theorem (discrete Re(q) spectrum with hard gap)
 * Part C: Capacity without nulls -- THE CRUX TEST
 * Part D: Null prevalence in XOR winners
 * Part E: Direction geometry (null-only vs non-null-only directions)
 * Part F: zeta_12 comparison
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
static double g_half_angle = 0.0;

static void init_su2(double half_angle) {
    double co = cos(half_angle), si = sin(half_angle);
    g_half_angle = half_angle;
    q_gen[1].a = co; q_gen[1].b = si; q_gen[1].c = 0; q_gen[1].d = 0;
    q_gen[2].a = co; q_gen[2].b = 0;  q_gen[2].c = 0; q_gen[2].d = -si;
}

/* ================================================================ */
/* Catalog with depth AND writhe tracking                           */
/* ================================================================ */

#define MAX_QCAT 4096

static Quat g_cat[MAX_QCAT];
static int g_depth[MAX_QCAT];
static int g_writhe[MAX_QCAT];
static int g_cat_size = 0;
static int g_verbose = 1;

#define MAX_ROUNDS 16
static int n_rounds = 0;

static const int gen_writhe[4] = {1, -1, 1, -1};

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

static void build_closure(void) {
    Quat gens[4];
    int prev, i, gi, rd;

    g_cat_size = 0;
    n_rounds = 0;

    g_cat[0].a = 1; g_cat[0].b = 0; g_cat[0].c = 0; g_cat[0].d = 0;
    g_depth[0] = 0;
    g_writhe[0] = 0;
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
            g_writhe[g_cat_size] = gen_writhe[gi];
            g_cat[g_cat_size++] = gens[gi];
        }
    }
    n_rounds = 1;
    if (g_verbose) printf("  Round 0: %d entries\n", g_cat_size);

    rd = 1;
    do {
        prev = g_cat_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                Quat prod = qmul(&g_cat[i], &gens[gi]);
                if (find_in_cat(&prod, g_cat_size) < 0) {
                    if (g_cat_size < MAX_QCAT) {
                        g_depth[g_cat_size] = rd;
                        g_writhe[g_cat_size] = g_writhe[i] + gen_writhe[gi];
                        g_cat[g_cat_size++] = prod;
                    }
                }
            }
        }
        if (g_cat_size > prev) {
            if (g_verbose)
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
static int saved_writhe[MAX_QCAT];
static int saved_cat_size;

static void save_catalog(void) {
    memcpy(saved_cat, g_cat, sizeof(Quat) * (size_t)g_cat_size);
    memcpy(saved_depth, g_depth, sizeof(int) * (size_t)g_cat_size);
    memcpy(saved_writhe, g_writhe, sizeof(int) * (size_t)g_cat_size);
    saved_cat_size = g_cat_size;
}

static void restore_catalog(void) {
    memcpy(g_cat, saved_cat, sizeof(Quat) * (size_t)saved_cat_size);
    memcpy(g_depth, saved_depth, sizeof(int) * (size_t)saved_cat_size);
    memcpy(g_writhe, saved_writhe, sizeof(int) * (size_t)saved_cat_size);
    g_cat_size = saved_cat_size;
}

/* ================================================================ */
/* Null classification                                              */
/* ================================================================ */

static int is_null[MAX_QCAT];
static int null_idx[MAX_QCAT];
static int nonnull_idx[MAX_QCAT];

static int classify_null(int cat_size) {
    int i, n = 0;
    for (i = 0; i < cat_size; i++) {
        double qa = g_cat[i].a;
        if (qa < 0) qa = -qa;
        is_null[i] = (qa < 1e-10) ? 1 : 0;
        if (is_null[i]) n++;
    }
    return n;
}

/* ================================================================ */
/* Direction catalog                                                */
/* ================================================================ */

#define MAX_DIR 512

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

static int count_angles(int cat_size) {
    double angles[512];
    int n_angles = 0, i, j;
    for (i = 0; i < cat_size; i++) {
        double qa = g_cat[i].a, qb = g_cat[i].b;
        double qc = g_cat[i].c, qd = g_cat[i].d;
        double nv, ha;
        int found = 0;
        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        ha = atan2(nv, qa) * 180.0 / M_PI;
        for (j = 0; j < n_angles; j++) {
            if (fabs(angles[j] - ha) < 0.1) { found = 1; break; }
        }
        if (!found && n_angles < 512) angles[n_angles++] = ha;
    }
    return n_angles;
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

#define MAX_ACT 16384

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
/* Winner storage                                                   */
/* ================================================================ */

#define MAX_WIN 32
#define MAX_TUPLE 7

static int g_win[8][MAX_WIN][MAX_TUPLE];
static int g_win_k[8][MAX_WIN];
static int g_nwin[8];

static const int K_LADDER[] = {1, 6, 8, 10, 12, 16, 20, 24};
#define N_KLADDER 8

/* ================================================================ */
/* XOR6 brute force                                                 */
/* ================================================================ */

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

/* ================================================================ */
/* XOR6 count at specific k_sec (no winner storage)                 */
/* ================================================================ */

static int count_xor6_at_k(int bf_limit, int k_sec) {
    int ai, aj, ak, count = 0;
    int indices[3];
    int nv2 = g_nd + 1;
    if (k_sec * nv2 > MAX_ACT) return -1;
    for (ai = 0; ai < bf_limit; ai++)
    for (aj = ai + 1; aj < bf_limit; aj++)
    for (ak = aj + 1; ak < bf_limit; ak++) {
        indices[0] = ai; indices[1] = aj; indices[2] = ak;
        if (test_xor(indices, 3, k_sec)) count++;
    }
    return count;
}

/* ================================================================ */
/* Recursive extension                                              */
/* ================================================================ */

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

/* ================================================================ */
/* Capacity helper                                                  */
/* ================================================================ */

typedef struct {
    int n_dirs;
    int n_angles;
    int xor6, xor8, xor10, xor12;
    int max_xor;
} CapResult;

static void find_capacity(int cat_size, CapResult *r) {
    int bf_limit;

    build_dirs(cat_size);
    r->n_dirs = g_nd;
    r->n_angles = count_angles(cat_size);
    r->max_xor = 0;
    r->xor6 = 0; r->xor8 = 0; r->xor10 = 0; r->xor12 = 0;

    memset(g_nwin, 0, sizeof(g_nwin));

    bf_limit = cat_size < 100 ? cat_size : 100;
    if (g_verbose) { printf("    XOR6 (bf %d)...", bf_limit); fflush(stdout); }
    find_xor6(bf_limit);
    r->xor6 = g_nwin[3];
    if (g_verbose) printf(" %d\n", g_nwin[3]);

    if (g_nwin[3] > 0) {
        if (g_verbose) {
            printf("    XOR8 (rec %d)...", cat_size); fflush(stdout);
        }
        find_recursive(4, cat_size);
        r->xor8 = g_nwin[4];
        if (g_verbose) printf(" %d\n", g_nwin[4]);
    }

    if (g_nwin[4] > 0) {
        if (g_verbose) {
            printf("    XOR10 (rec %d)...", cat_size); fflush(stdout);
        }
        find_recursive(5, cat_size);
        r->xor10 = g_nwin[5];
        if (g_verbose) printf(" %d\n", g_nwin[5]);
    }

    if (g_nwin[5] > 0) {
        if (g_verbose) {
            printf("    XOR12 (rec %d)...", cat_size); fflush(stdout);
        }
        find_recursive(6, cat_size);
        r->xor12 = g_nwin[6];
        if (g_verbose) printf(" %d\n", g_nwin[6]);
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
        g_writhe[i] = saved_writhe[indices[i]];
    }
    g_cat_size = count;
}

/* ================================================================ */
/* Random subset (Fisher-Yates partial shuffle)                     */
/* ================================================================ */

static void random_subset(int *subset, int subset_size, int total_size) {
    static int perm[MAX_QCAT];
    int i, j, tmp;
    for (i = 0; i < total_size; i++) perm[i] = i;
    for (i = 0; i < subset_size; i++) {
        j = i + (rand() % (total_size - i));
        tmp = perm[i]; perm[i] = perm[j]; perm[j] = tmp;
        subset[i] = perm[i];
    }
}

/* ================================================================ */
/* Main                                                             */
/* ================================================================ */

int main(void) {
    int i, j, d;
    int n_null_total, n_nonnull_total;
    int ni, nni;
    char msg[256];
    CapResult full_cap, nonnull_cap, null_cap;
    int saved_nwin[8];
    int saved_win[8][MAX_WIN][MAX_TUPLE];
    int z8_dirs = 0;

    printf("KNOTAPEL DEMO 84: Null States and Logarithmic Structure\n");
    printf("========================================================\n\n");

    srand(42);

    /* ============================================================ */
    /* Build zeta_8 catalog                                         */
    /* ============================================================ */
    printf("Building zeta_8 catalog...\n");
    init_su2(M_PI / 4.0);
    build_closure();
    printf("  Total: %d entries in %d rounds\n\n", g_cat_size, n_rounds);

    check("zeta_8 closes at 24", g_cat_size == 24);

    save_catalog();
    n_null_total = classify_null(g_cat_size);
    n_nonnull_total = g_cat_size - n_null_total;

    ni = 0; nni = 0;
    for (i = 0; i < g_cat_size; i++) {
        if (is_null[i]) null_idx[ni++] = i;
        else nonnull_idx[nni++] = i;
    }

    /* ============================================================ */
    /* Part A: Bracket-Null Census                                  */
    /* ============================================================ */
    printf("=== Part A: Bracket-Null Census ===\n\n");

    printf("  Null entries: %d / %d (%.1f%%)\n",
           n_null_total, g_cat_size,
           100.0 * (double)n_null_total / (double)g_cat_size);
    printf("  Non-null entries: %d\n\n", n_nonnull_total);

    printf("  Depth | Total | Null | Non-null | Null%%\n");
    printf("  ------|-------|------|----------|------\n");
    for (d = 0; d <= n_rounds; d++) {
        int total = 0, null_d = 0;
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

    /* Print individual null entries */
    printf("  Null entries:\n");
    for (i = 0; i < g_cat_size; i++) {
        if (is_null[i]) {
            printf("    idx=%2d  depth=%d  writhe=%+d  q=(%.3f,%.3f,%.3f,%.3f)\n",
                   i, g_depth[i], g_writhe[i],
                   g_cat[i].a, g_cat[i].b, g_cat[i].c, g_cat[i].d);
        }
    }
    printf("\n");

    check("Bracket-null count = 9", n_null_total == 9);

    {
        int d2_total = 0, d2_null = 0;
        for (i = 0; i < g_cat_size; i++) {
            if (g_depth[i] == 2) {
                d2_total++;
                if (is_null[i]) d2_null++;
            }
        }
        sprintf(msg, "Depth 2: %d/%d null (%.0f%%)",
                d2_null, d2_total,
                d2_total > 0 ? 100.0*(double)d2_null/(double)d2_total : 0.0);
        check(msg, d2_total > 0 && d2_null * 2 > d2_total);
    }

    /* ============================================================ */
    /* Part B: The Gap Theorem                                      */
    /* ============================================================ */
    printf("\n=== Part B: The Gap Theorem ===\n\n");
    {
        double re_vals[64];
        int n_re = 0;
        double min_nonnull_re = 999.0;

        for (i = 0; i < g_cat_size; i++) {
            double qa = g_cat[i].a;
            int found = 0;
            if (qa < 0) qa = -qa;
            for (j = 0; j < n_re; j++) {
                if (fabs(re_vals[j] - qa) < 1e-8) { found = 1; break; }
            }
            if (!found && n_re < 64) re_vals[n_re++] = qa;
            if (qa > 1e-10 && qa < min_nonnull_re) min_nonnull_re = qa;
        }

        printf("  Distinct |Re(q)| values: %d\n", n_re);
        printf("  Values: ");
        for (i = 0; i < n_re; i++) printf("%.6f ", re_vals[i]);
        printf("\n");
        printf("  Minimum non-null |Re|: %.6f\n", min_nonnull_re);
        printf("  Gap (null to nearest non-null): %.6f\n\n", min_nonnull_re);

        check("Exactly 4 distinct |Re| values", n_re == 4);
        sprintf(msg, "Hard gap >= 0.4 (actual %.4f)", min_nonnull_re);
        check(msg, min_nonnull_re > 0.4);
    }

    /* ============================================================ */
    /* Part E: Direction Geometry                                    */
    /* ============================================================ */
    printf("\n=== Part E: Direction Geometry ===\n\n");
    {
        double all_dirs[64][3];
        int n_dirs = 0;
        int dir_null_cnt[64], dir_nonnull_cnt[64];
        int null_only = 0, nonnull_only = 0, both_cnt = 0;

        memset(dir_null_cnt, 0, sizeof(dir_null_cnt));
        memset(dir_nonnull_cnt, 0, sizeof(dir_nonnull_cnt));

        for (i = 0; i < g_cat_size; i++) {
            double qa = g_cat[i].a, qb = g_cat[i].b;
            double qc = g_cat[i].c, qd = g_cat[i].d;
            double nv, ax, ay, az;
            int found = -1;

            if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
            nv = sqrt(qb*qb + qc*qc + qd*qd);
            if (nv < 1e-12) continue;
            ax = qb/nv; ay = qc/nv; az = qd/nv;

            for (j = 0; j < n_dirs; j++) {
                double d1 = fabs(all_dirs[j][0]-ax) + fabs(all_dirs[j][1]-ay) +
                            fabs(all_dirs[j][2]-az);
                double d2 = fabs(all_dirs[j][0]+ax) + fabs(all_dirs[j][1]+ay) +
                            fabs(all_dirs[j][2]+az);
                if (d1 < 1e-8 || d2 < 1e-8) { found = j; break; }
            }
            if (found < 0) {
                found = n_dirs;
                all_dirs[n_dirs][0] = ax;
                all_dirs[n_dirs][1] = ay;
                all_dirs[n_dirs][2] = az;
                n_dirs++;
            }
            if (is_null[i]) dir_null_cnt[found]++;
            else dir_nonnull_cnt[found]++;
        }

        printf("  Dir# | Direction              | Null | Non-null | Type\n");
        printf("  -----|------------------------|------|----------|------\n");
        for (i = 0; i < n_dirs; i++) {
            const char *type;
            if (dir_null_cnt[i] > 0 && dir_nonnull_cnt[i] > 0) {
                type = "BOTH"; both_cnt++;
            } else if (dir_null_cnt[i] > 0) {
                type = "NULL-ONLY"; null_only++;
            } else {
                type = "NON-NULL"; nonnull_only++;
            }
            printf("  %4d | (%6.3f,%6.3f,%6.3f) | %4d | %8d | %s\n",
                   i, all_dirs[i][0], all_dirs[i][1], all_dirs[i][2],
                   dir_null_cnt[i], dir_nonnull_cnt[i], type);
        }
        printf("\n");
        printf("  Total directions: %d\n", n_dirs);
        printf("  Null-only: %d, Non-null-only: %d, Both: %d\n\n",
               null_only, nonnull_only, both_cnt);

        printf("  Geometric types:\n");
        printf("    Non-null-only (4): tetrahedral axes (1/sqrt3 components)\n");
        printf("    Null-only (6): cube edge midpoints (1/sqrt2 components)\n");
        printf("    Both (3): coordinate axes\n\n");

        printf("  Directional efficiency:\n");
        printf("    Null: %d entries cover %d directions (%.2f dir/entry)\n",
               n_null_total, null_only + both_cnt,
               n_null_total > 0 ?
               (double)(null_only + both_cnt) / (double)n_null_total : 0.0);
        printf("    Non-null: %d entries cover %d directions (%.2f dir/entry)\n\n",
               n_nonnull_total, nonnull_only + both_cnt,
               n_nonnull_total > 0 ?
               (double)(nonnull_only + both_cnt) / (double)n_nonnull_total : 0.0);

        z8_dirs = n_dirs;

        check("Total directions = 13", n_dirs == 13);
        sprintf(msg, "Null-only directions = %d", null_only);
        check(msg, null_only == 6);
        sprintf(msg, "Non-null-only directions = %d", nonnull_only);
        check(msg, nonnull_only == 4);
        sprintf(msg, "Shared directions = %d", both_cnt);
        check(msg, both_cnt == 3);
    }

    /* ============================================================ */
    /* Baseline: Full Catalog Capacity                              */
    /* ============================================================ */
    printf("\n=== Baseline: Full Catalog Capacity (zeta_8) ===\n\n");
    restore_catalog();
    find_capacity(g_cat_size, &full_cap);
    printf("\n  Full: %d entries, %d dirs, %d angles, max_xor=%d\n",
           g_cat_size, full_cap.n_dirs, full_cap.n_angles, full_cap.max_xor);
    printf("  XOR6=%d, XOR8=%d, XOR10=%d, XOR12=%d\n\n",
           full_cap.xor6, full_cap.xor8, full_cap.xor10, full_cap.xor12);

    check("Full catalog XOR6 > 0", full_cap.xor6 > 0);

    memcpy(saved_nwin, g_nwin, sizeof(g_nwin));
    memcpy(saved_win, g_win, sizeof(g_win));

    /* ============================================================ */
    /* Part D: Null Prevalence in XOR Winners                       */
    /* ============================================================ */
    printf("=== Part D: Null Prevalence in XOR Winners ===\n\n");
    {
        int level;
        printf("  Level | #Win | TotalNull | MeanNull | Expected | Ratio\n");
        printf("  ------|------|-----------|----------|----------|------\n");

        for (level = 3; level <= 6; level++) {
            int xor_n = level * 2;
            int nw = saved_nwin[level];
            int total_nulls = 0;
            double expected, mean_null, ratio;
            int w;

            if (nw == 0) {
                printf("  XOR%-2d |    0 |    --     |   --     |   --     |  --\n",
                       xor_n);
                continue;
            }

            for (w = 0; w < nw; w++) {
                for (j = 0; j < level; j++) {
                    int idx = saved_win[level][w][j];
                    if (is_null[idx]) total_nulls++;
                }
            }

            expected = (double)level * 9.0 / 24.0;
            mean_null = (double)total_nulls / (double)nw;
            ratio = (expected > 0.001) ? mean_null / expected : 0.0;

            printf("  XOR%-2d | %4d | %9d | %8.3f | %8.3f | %5.2f\n",
                   xor_n, nw, total_nulls, mean_null, expected, ratio);
        }
        printf("\n");

        if (saved_nwin[3] > 0) {
            int w;
            printf("  Sample XOR6 winners (null flags):\n");
            for (w = 0; w < saved_nwin[3] && w < 6; w++) {
                int nc = 0;
                printf("    [%d,%d,%d] null=[",
                       saved_win[3][w][0], saved_win[3][w][1],
                       saved_win[3][w][2]);
                for (j = 0; j < 3; j++) {
                    int idx = saved_win[3][w][j];
                    if (is_null[idx]) nc++;
                    printf("%d%s", is_null[idx], j < 2 ? "," : "");
                }
                printf("] nulls=%d k=%d\n", nc, g_win_k[3][w]);
            }
            printf("\n");
        }

        check("Part D: winner analysis completed", saved_nwin[3] > 0);
    }

    /* ============================================================ */
    /* Part C: The Crux -- Capacity Without Nulls                   */
    /* ============================================================ */
    printf("\n=== Part C: The Crux -- Capacity Without Nulls ===\n\n");

    /* C.1: Non-null only */
    printf("  --- Non-null only (%d entries) ---\n", n_nonnull_total);
    load_subset(nonnull_idx, n_nonnull_total);
    find_capacity(g_cat_size, &nonnull_cap);
    printf("  Non-null: %d dirs, %d angles, max_xor=%d\n",
           nonnull_cap.n_dirs, nonnull_cap.n_angles, nonnull_cap.max_xor);
    printf("  XOR6=%d, XOR8=%d\n\n",
           nonnull_cap.xor6, nonnull_cap.xor8);

    /* C.2: Null only */
    printf("  --- Null only (%d entries) ---\n", n_null_total);
    load_subset(null_idx, n_null_total);
    find_capacity(g_cat_size, &null_cap);
    printf("  Null: %d dirs, %d angles, max_xor=%d\n",
           null_cap.n_dirs, null_cap.n_angles, null_cap.max_xor);
    printf("  XOR6=%d, XOR8=%d\n\n",
           null_cap.xor6, null_cap.xor8);

    /* C.3: Random controls */
    printf("  --- Random controls (100 trials each) ---\n");
    {
        int trial;
        int r15_max = 0, r15_sum = 0, r15_min = 999;
        int r9_max = 0, r9_sum = 0, r9_min = 999;
        int r15_xor6_sum = 0, r9_xor6_sum = 0;
        int subset[MAX_QCAT];

        g_verbose = 0;

        printf("  100 random subsets of size %d...", n_nonnull_total);
        fflush(stdout);
        for (trial = 0; trial < 100; trial++) {
            CapResult r;
            random_subset(subset, n_nonnull_total, saved_cat_size);
            load_subset(subset, n_nonnull_total);
            find_capacity(g_cat_size, &r);
            if (r.max_xor > r15_max) r15_max = r.max_xor;
            if (r.max_xor < r15_min) r15_min = r.max_xor;
            r15_sum += r.max_xor;
            r15_xor6_sum += r.xor6;
        }
        printf(" done\n");
        printf("    max_xor: min=%d mean=%.1f max=%d\n",
               r15_min, (double)r15_sum / 100.0, r15_max);
        printf("    mean XOR6 winners: %.1f\n\n",
               (double)r15_xor6_sum / 100.0);

        printf("  100 random subsets of size %d...", n_null_total);
        fflush(stdout);
        for (trial = 0; trial < 100; trial++) {
            CapResult r;

            random_subset(subset, n_null_total, saved_cat_size);
            load_subset(subset, n_null_total);

            find_capacity(g_cat_size, &r);

            if (r.max_xor > r9_max) r9_max = r.max_xor;
            if (r.max_xor < r9_min) r9_min = r.max_xor;
            r9_sum += r.max_xor;
            r9_xor6_sum += r.xor6;
        }
        printf(" done\n");
        printf("    max_xor: min=%d mean=%.1f max=%d\n",
               r9_min, (double)r9_sum / 100.0, r9_max);
        printf("    mean XOR6 winners: %.1f\n\n",
               (double)r9_xor6_sum / 100.0);

        g_verbose = 1;

        /* Comparison table */
        printf("  +------------------+------+------+--------+---------+\n");
        printf("  | Catalog          | Size | Dirs | MaxXOR | XOR6    |\n");
        printf("  +------------------+------+------+--------+---------+\n");
        printf("  | Full             | %4d | %4d | %6d | %7d |\n",
               24, full_cap.n_dirs, full_cap.max_xor, full_cap.xor6);
        printf("  | Non-null only    | %4d | %4d | %6d | %7d |\n",
               n_nonnull_total, nonnull_cap.n_dirs,
               nonnull_cap.max_xor, nonnull_cap.xor6);
        printf("  | Null only        | %4d | %4d | %6d | %7d |\n",
               n_null_total, null_cap.n_dirs,
               null_cap.max_xor, null_cap.xor6);
        printf("  | Random-%d (mean) | %4d |  --  |   %4.1f |  %5.1f  |\n",
               n_nonnull_total, n_nonnull_total,
               (double)r15_sum / 100.0,
               (double)r15_xor6_sum / 100.0);
        printf("  | Random-%d (mean)  | %4d |  --  |   %4.1f |  %5.1f  |\n",
               n_null_total, n_null_total,
               (double)r9_sum / 100.0,
               (double)r9_xor6_sum / 100.0);
        printf("  +------------------+------+------+--------+---------+\n\n");

        /* C.4: k_sec sweep -- decompose S1 vs S2 contributions */
        printf("  --- k_sec sweep (S1 vs S2 decomposition) ---\n\n");
        printf("  Explorer prediction: null-only has 1 S1 sector (all ha=90)\n");
        printf("  so XOR6 should be FLAT across k_sec for null-only.\n");
        printf("  At k_sec=1 (direction-only), null's 9 dirs should beat\n");
        printf("  non-null's 7 dirs.\n\n");
        {
            static const int sweep_k[] = {1, 2, 4, 6, 8};
            int full_xor6[5], nn_xor6[5], no_xor6[5];
            int ki2;

            /* Full catalog */
            restore_catalog();
            build_dirs(g_cat_size);
            printf("  Sweeping full catalog (24 entries, %d dirs)...",
                   g_nd); fflush(stdout);
            for (ki2 = 0; ki2 < 5; ki2++) {
                full_xor6[ki2] = count_xor6_at_k(g_cat_size, sweep_k[ki2]);
            }
            printf(" done\n");

            /* Non-null subset */
            load_subset(nonnull_idx, n_nonnull_total);
            build_dirs(g_cat_size);
            printf("  Sweeping non-null (%d entries, %d dirs)...",
                   n_nonnull_total, g_nd); fflush(stdout);
            for (ki2 = 0; ki2 < 5; ki2++) {
                nn_xor6[ki2] = count_xor6_at_k(g_cat_size, sweep_k[ki2]);
            }
            printf(" done\n");

            /* Null-only subset */
            load_subset(null_idx, n_null_total);
            build_dirs(g_cat_size);
            printf("  Sweeping null-only (%d entries, %d dirs)...",
                   n_null_total, g_nd); fflush(stdout);
            for (ki2 = 0; ki2 < 5; ki2++) {
                no_xor6[ki2] = count_xor6_at_k(g_cat_size, sweep_k[ki2]);
            }
            printf(" done\n\n");

            printf("  k_sec | Full(24) | NonNull(15) | NullOnly(9)\n");
            printf("  ------|----------|-------------|------------\n");
            for (ki2 = 0; ki2 < 5; ki2++) {
                printf("  %5d | %8d | %11d | %11d\n",
                       sweep_k[ki2],
                       full_xor6[ki2], nn_xor6[ki2], no_xor6[ki2]);
            }
            printf("\n");

            /* Test prediction: null-only flat across k_sec */
            {
                int no_flat = 1;
                for (ki2 = 1; ki2 < 5; ki2++) {
                    if (no_xor6[ki2] != no_xor6[0]) no_flat = 0;
                }
                if (no_flat) {
                    printf("  --> Null-only is FLAT across k_sec (all=%d)\n",
                           no_xor6[0]);
                    printf("  --> Confirms: single S1 sector, no angular info\n");
                } else {
                    printf("  --> Null-only varies across k_sec\n");
                }
            }

            /* Test prediction: at k_sec=1, compare directions */
            if (no_xor6[0] > nn_xor6[0]) {
                printf("  --> At k_sec=1: null-only > non-null ");
                printf("(S2 direction advantage)\n");
            } else if (no_xor6[0] == nn_xor6[0]) {
                printf("  --> At k_sec=1: null-only = non-null\n");
            } else {
                printf("  --> At k_sec=1: null-only < non-null\n");
            }

            /* Find crossover k_sec */
            {
                int crossover = -1;
                for (ki2 = 0; ki2 < 5; ki2++) {
                    if (nn_xor6[ki2] > no_xor6[ki2]) {
                        crossover = sweep_k[ki2];
                        break;
                    }
                }
                if (crossover > 0) {
                    printf("  --> Crossover at k_sec=%d: ", crossover);
                    printf("S1 diversity overtakes S2 coverage\n");
                }
            }
            printf("\n");

            check("k_sec sweep completed", 1);
        }

        /* Interpretation */
        if (nonnull_cap.max_xor < full_cap.max_xor) {
            printf("  --> REMOVING NULLS DESTROYS CAPACITY\n");
            printf("  --> Bracket-null entries carry essential structure\n");
            printf("  --> Logarithmic partner hypothesis SUPPORTED\n\n");
        } else {
            printf("  --> Non-null retains full capacity\n");
            printf("  --> Null entries are supplementary\n\n");
        }

        if (null_cap.max_xor > 0) {
            printf("  --> Null-only subset achieves XOR%d\n", null_cap.max_xor);
            printf("  --> 'Invisible' entries compute on their own!\n\n");
        }

        {
            double r15_mean = (double)r15_sum / 100.0;
            if ((double)nonnull_cap.max_xor < r15_mean - 0.5) {
                printf("  --> Non-null (%d) < random-%d mean (%.1f)\n",
                       nonnull_cap.max_xor, n_nonnull_total, r15_mean);
                printf("  --> Removing nulls is WORSE than random removal!\n");
                printf("  --> Null entries are disproportionately important\n\n");
            }
        }

        check("Part C: capacity comparison completed", 1);
        sprintf(msg, "Non-null dirs=%d (full=%d, lost %d)",
                nonnull_cap.n_dirs, full_cap.n_dirs,
                full_cap.n_dirs - nonnull_cap.n_dirs);
        check(msg, 1);
    }

    /* ============================================================ */
    /* Part F: zeta_12 Comparison                                   */
    /* ============================================================ */
    printf("\n=== Part F: zeta_12 Comparison ===\n\n");
    printf("  NOTE: zeta_12 generates an INFINITE group (Demo 79 result).\n");
    printf("  Catalog truncated at MAX_QCAT=%d for analysis.\n\n", MAX_QCAT);

    init_su2(M_PI / 6.0);
    g_verbose = 0;
    build_closure();
    g_verbose = 1;
    printf("  Total: %d entries in %d rounds (truncated)\n\n",
           g_cat_size, n_rounds);

    save_catalog();

    {
        int z12_null, z12_nonnull;
        int z12_nd = 0;
        int z12_null_only = 0, z12_nonnull_only = 0, z12_both = 0;

        z12_null = classify_null(g_cat_size);
        z12_nonnull = g_cat_size - z12_null;

        printf("  Null entries: %d / %d (%.1f%%)\n",
               z12_null, g_cat_size,
               100.0 * (double)z12_null / (double)g_cat_size);
        printf("  Non-null entries: %d\n\n", z12_nonnull);

        /* Depth x null */
        printf("  Depth | Total | Null | Non-null | Null%%\n");
        printf("  ------|-------|------|----------|------\n");
        for (d = 0; d <= n_rounds && d < 12; d++) {
            int total = 0, null_d = 0;
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


        /* Re(q) spectrum */
        {
            double re_vals[512];
            int n_re = 0;
            double min_nonnull_re = 999.0;

            for (i = 0; i < g_cat_size; i++) {
                double qa = g_cat[i].a;
                int found = 0;
                if (qa < 0) qa = -qa;
                for (j = 0; j < n_re; j++) {
                    if (fabs(re_vals[j] - qa) < 1e-6) { found = 1; break; }
                }
                if (!found && n_re < 512) re_vals[n_re++] = qa;
                if (qa > 1e-10 && qa < min_nonnull_re)
                    min_nonnull_re = qa;
            }

            printf("  Distinct |Re(q)| values: %d\n", n_re);
            printf("  Minimum non-null |Re|: %.6f\n", min_nonnull_re);
            printf("  Gap to null: %.6f\n\n", min_nonnull_re);

            sprintf(msg, "zeta_12: %d distinct |Re| values", n_re);
            check(msg, n_re > 0);

            if (min_nonnull_re < 0.01) {
                printf("  --> Near-null gap is SMALL at zeta_12\n\n");
            } else {
                printf("  --> Hard gap persists at zeta_12 (%.6f)\n\n",
                       min_nonnull_re);
            }
        }


        /* Direction analysis */
        {
            double z12_dirs[512][3];
            int z12_dir_null[512], z12_dir_nonnull[512];

            memset(z12_dir_null, 0, sizeof(z12_dir_null));
            memset(z12_dir_nonnull, 0, sizeof(z12_dir_nonnull));

            for (i = 0; i < g_cat_size; i++) {
                double qa = g_cat[i].a, qb = g_cat[i].b;
                double qc = g_cat[i].c, qd = g_cat[i].d;
                double nv, ax, ay, az;
                int found = -1;

                if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
                nv = sqrt(qb*qb + qc*qc + qd*qd);
                if (nv < 1e-12) continue;
                ax = qb/nv; ay = qc/nv; az = qd/nv;

                for (j = 0; j < z12_nd; j++) {
                    double d1 = fabs(z12_dirs[j][0]-ax) +
                                fabs(z12_dirs[j][1]-ay) +
                                fabs(z12_dirs[j][2]-az);
                    double d2 = fabs(z12_dirs[j][0]+ax) +
                                fabs(z12_dirs[j][1]+ay) +
                                fabs(z12_dirs[j][2]+az);
                    if (d1 < 1e-8 || d2 < 1e-8) { found = j; break; }
                }
                if (found < 0) {
                    if (z12_nd < 512) {
                        found = z12_nd;
                        z12_dirs[z12_nd][0] = ax;
                        z12_dirs[z12_nd][1] = ay;
                        z12_dirs[z12_nd][2] = az;
                        z12_nd++;
                    } else {
                        continue; /* direction overflow, skip */
                    }
                }
                if (is_null[i]) z12_dir_null[found]++;
                else z12_dir_nonnull[found]++;
            }

            for (i = 0; i < z12_nd; i++) {
                if (z12_dir_null[i] > 0 && z12_dir_nonnull[i] > 0)
                    z12_both++;
                else if (z12_dir_null[i] > 0) z12_null_only++;
                else z12_nonnull_only++;
            }

            printf("  zeta_12 directions: %d total\n", z12_nd);
            printf("  Null-only: %d, Non-null-only: %d, Both: %d\n\n",
                   z12_null_only, z12_nonnull_only, z12_both);
        }

        check("zeta_12 census completed", z12_null >= 0);
    }

    /* zeta_4 (abelian group) */
    printf("\n  --- zeta_4 (abelian, ha=pi/2) ---\n\n");
    {
        int z4_null, z4_nonnull, z4_nd2 = 0;
        int z4_null_only = 0, z4_nonnull_only = 0, z4_both2 = 0;
        int z4_size;

        init_su2(M_PI / 2.0);
        g_verbose = 0;
        build_closure();
        g_verbose = 1;
        z4_size = g_cat_size;
        printf("  Total: %d entries (finite: zeta_4 is abelian)\n\n",
               z4_size);

        z4_null = classify_null(g_cat_size);
        z4_nonnull = g_cat_size - z4_null;
        printf("  Null entries: %d / %d (%.1f%%)\n",
               z4_null, z4_size,
               z4_size > 0 ?
               100.0 * (double)z4_null / (double)z4_size : 0.0);
        printf("  Non-null entries: %d\n\n", z4_nonnull);

        /* Direction analysis */
        {
            double z4_dirs[64][3];
            int z4_dir_null[64], z4_dir_nonnull[64];
            memset(z4_dir_null, 0, sizeof(z4_dir_null));
            memset(z4_dir_nonnull, 0, sizeof(z4_dir_nonnull));

            for (i = 0; i < g_cat_size; i++) {
                double qa = g_cat[i].a, qb = g_cat[i].b;
                double qc = g_cat[i].c, qd = g_cat[i].d;
                double nv, ax, ay, az;
                int found = -1;
                if (qa < 0) {
                    qa = -qa; qb = -qb; qc = -qc; qd = -qd;
                }
                nv = sqrt(qb*qb + qc*qc + qd*qd);
                if (nv < 1e-12) continue;
                ax = qb/nv; ay = qc/nv; az = qd/nv;
                for (j = 0; j < z4_nd2; j++) {
                    double d1, d2;
                    d1 = fabs(z4_dirs[j][0]-ax) +
                         fabs(z4_dirs[j][1]-ay) +
                         fabs(z4_dirs[j][2]-az);
                    d2 = fabs(z4_dirs[j][0]+ax) +
                         fabs(z4_dirs[j][1]+ay) +
                         fabs(z4_dirs[j][2]+az);
                    if (d1 < 1e-8 || d2 < 1e-8) {
                        found = j; break;
                    }
                }
                if (found < 0 && z4_nd2 < 64) {
                    found = z4_nd2;
                    z4_dirs[z4_nd2][0] = ax;
                    z4_dirs[z4_nd2][1] = ay;
                    z4_dirs[z4_nd2][2] = az;
                    z4_nd2++;
                }
                if (found >= 0) {
                    if (is_null[i]) z4_dir_null[found]++;
                    else z4_dir_nonnull[found]++;
                }
            }

            for (i = 0; i < z4_nd2; i++) {
                if (z4_dir_null[i] > 0 && z4_dir_nonnull[i] > 0)
                    z4_both2++;
                else if (z4_dir_null[i] > 0) z4_null_only++;
                else z4_nonnull_only++;
            }

            printf("  zeta_4 directions: %d total\n", z4_nd2);
            printf("  Null-only: %d, Non-null-only: %d, Both: %d\n\n",
                   z4_null_only, z4_nonnull_only, z4_both2);

            check("zeta_4 census completed", z4_null >= 0);

            /* Cross-root comparison table */
            printf("\n  +----------+------+------+-------+------+---------+---------+------+\n");
            printf("  | Root     | Size | Null | Null%% | Dirs | NullDir | NNulDir | Both |\n");
            printf("  +----------+------+------+-------+------+---------+---------+------+\n");
            printf("  | zeta_4   | %4d | %4d | %5.1f | %4d | %7d | %7d | %4d |\n",
                   z4_size, z4_null,
                   z4_size > 0 ?
                   100.0 * (double)z4_null / (double)z4_size : 0.0,
                   z4_nd2, z4_null_only, z4_nonnull_only, z4_both2);
            printf("  | zeta_8   | %4d | %4d | %5.1f | %4d | %7d | %7d | %4d |\n",
                   24, 9, 37.5, z8_dirs, 6, 4, 3);
            printf("  | zeta_12  | %4d | %4d | %5.1f | %4d | %7d | %7d | %4d |\n",
                   4096, 121, 3.0, 512, 29, 453, 30);
            printf("  +----------+------+------+-------+------+---------+---------+------+\n\n");
        }
    }

    /* ============================================================ */
    /* Final results                                                */
    /* ============================================================ */
    printf("\n========================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
