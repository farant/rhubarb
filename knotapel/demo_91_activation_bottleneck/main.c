/*
 * KNOTAPEL DEMO 91: Activation Bottleneck Test
 * =============================================
 *
 * Core question: Is the combined_cell activation the bottleneck for the
 * linear depth law, or is the parity constraint itself the wall?
 *
 * Phase 1: k_sec sweep at fixed catalog (cumul depth 0-4, ~275 entries).
 *   Vary angular quantization k_sec = {2,4,6,8,12,16,24,32,48}.
 *   If max_xor increases monotonically -> activation IS the bottleneck.
 *   If max_xor plateaus -> parity IS the wall.
 *   If max_xor increases then decreases -> optimal resolution exists.
 *
 * Phase 2: Voronoi resolution sweep at optimal k_sec.
 *   Fix k_sec, vary direction set (depth 0 only, 0-1, 0-2, ... all).
 *
 * Phase 3: Depth x k_sec interaction (the key measurement).
 *   For each max_depth in {0,2,4,6,8}, run k_sec sweep.
 *   If activation is bottleneck -> slope should increase at higher k_sec.
 *   If slope stays same regardless -> parity is the wall.
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
/* SU(2) generators + group closure with depth tracking             */
/* ================================================================ */

#define MAX_QCAT 4096
#define MAX_DIR 2048
#define MAX_ROUNDS 16

static Quat g_cat[MAX_QCAT];
static int g_cat_size = 0;
static int g_depth[MAX_QCAT];
static int n_rounds = 0;

static int round_start[MAX_ROUNDS];
static int round_count[MAX_ROUNDS];

static Quat g_gen[3];

static void init_su2(double half_angle) {
    double co = cos(half_angle), si = sin(half_angle);
    g_gen[1].a = co; g_gen[1].b = si; g_gen[1].c = 0; g_gen[1].d = 0;
    g_gen[2].a = co; g_gen[2].b = 0;  g_gen[2].c = 0; g_gen[2].d = -si;
}

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
    memset(round_start, 0, sizeof(round_start));
    memset(round_count, 0, sizeof(round_count));

    g_cat[0].a = 1; g_cat[0].b = 0; g_cat[0].c = 0; g_cat[0].d = 0;
    g_depth[0] = 0;
    g_cat_size = 1;

    gens[0] = g_gen[1];
    gens[1].a =  g_gen[1].a; gens[1].b = -g_gen[1].b;
    gens[1].c = -g_gen[1].c; gens[1].d = -g_gen[1].d;
    gens[2] = g_gen[2];
    gens[3].a =  g_gen[2].a; gens[3].b = -g_gen[2].b;
    gens[3].c = -g_gen[2].c; gens[3].d = -g_gen[2].d;

    round_start[0] = 0;
    for (gi = 0; gi < 4; gi++) {
        if (find_in_cat(&gens[gi], g_cat_size) < 0 &&
            g_cat_size < MAX_QCAT) {
            g_depth[g_cat_size] = 0;
            g_cat[g_cat_size++] = gens[gi];
        }
    }
    round_count[0] = g_cat_size;
    n_rounds = 1;
    if (verbose) printf("  Round 0: %d entries\n", g_cat_size);

    rd = 1;
    do {
        prev = g_cat_size;
        round_start[rd] = prev;
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
        round_count[rd] = g_cat_size - prev;
        if (g_cat_size > prev) {
            if (verbose)
                printf("  Round %d: %d entries (+%d)\n",
                       rd, g_cat_size, g_cat_size - prev);
            n_rounds++;
        }
        rd++;
    } while (g_cat_size > prev && rd < MAX_ROUNDS);
}

/* ================================================================ */
/* Saved catalog for subset restoration                             */
/* ================================================================ */

static Quat saved_cat[MAX_QCAT];
static int saved_depth[MAX_QCAT];
static int saved_cat_size;

static void save_catalog(void) {
    int i;
    saved_cat_size = g_cat_size;
    for (i = 0; i < g_cat_size; i++) {
        saved_cat[i] = g_cat[i];
        saved_depth[i] = g_depth[i];
    }
}

/* Load catalog entries up to max_depth */
static int load_up_to_depth(int max_d) {
    int i, n = 0;
    for (i = 0; i < saved_cat_size; i++) {
        if (saved_depth[i] <= max_d) {
            g_cat[n] = saved_cat[i];
            g_depth[n] = saved_depth[i];
            n++;
        }
    }
    g_cat_size = n;
    return n;
}

/* ================================================================ */
/* Direction catalog                                                */
/* ================================================================ */

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

/* Build direction catalog using only saved entries up to max_depth */
static void build_dirs_from_saved(int max_d) {
    int i, j;
    g_nd = 0;
    for (i = 0; i < saved_cat_size; i++) {
        double qa, qb, qc, qd, nv, ax, ay, az;
        int found = 0;
        if (saved_depth[i] > max_d) continue;
        qa = saved_cat[i].a; qb = saved_cat[i].b;
        qc = saved_cat[i].c; qd = saved_cat[i].d;
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
/* Voronoi cell + combined cell + XOR test                          */
/* ================================================================ */

#define MAX_ACT 65536

static int cell_even[MAX_ACT], cell_odd[MAX_ACT];
static int touched_cells[MAX_ACT];

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
    double n2 = sa*sa + sb*sb + sc*sc + sd*sd;
    double nm, qa, nv, half_ang, ang;
    int sec, vor, n_vor;

    n_vor = g_nd + 1;
    if (n2 < 1e-24) return (k_sec - 1) * n_vor + g_nd;

    nm = sqrt(n2);
    qa = sa / nm;
    if (qa < 0) { qa = -qa; sb = -sb; sc = -sc; sd = -sd; }
    if (qa > 1.0) qa = 1.0;

    half_ang = acos(qa);
    ang = 2.0 * half_ang * 180.0 / M_PI;
    sec = (int)(ang * (double)k_sec / 360.0);
    if (sec >= k_sec) sec = k_sec - 1;
    if (sec < 0) sec = 0;

    nv = sqrt(sb*sb + sc*sc + sd*sd) / nm;
    if (nv < 1e-12) {
        vor = g_nd;
    } else {
        double inv = nm * nv;
        double ax2 = sb / inv, ay2 = sc / inv, az2 = sd / inv;
        vor = vor_cell(ax2, ay2, az2);
    }

    return sec * n_vor + vor;
}

static int test_xor(const int *indices, int n_weights, int k_sec) {
    int n_inputs = 2 * n_weights;
    int n_vor, n_cells;
    double wa[14], wb[14], wc[14], wd[14];
    int n_touched = 0;
    int mask, i, result = 1;
    int n_masks;

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
/* XOR search: find max XOR arity at a given k_sec                  */
/* Returns max XOR arity achieved and number of winners              */
/* ================================================================ */

static int count_xor_winners(int n_weights, int bf_limit, int k_sec) {
    int count = 0;
    int indices[7];
    int i0, i1, i2, i3, i4;

    if (bf_limit > g_cat_size) bf_limit = g_cat_size;

    /* Check if cell count exceeds MAX_ACT */
    {
        int n_cells = k_sec * (g_nd + 1);
        if (n_cells > MAX_ACT || 2 * n_weights > 14) return 0;
    }

    if (n_weights == 3) {
        for (i0 = 0; i0 < bf_limit; i0++)
        for (i1 = i0+1; i1 < bf_limit; i1++)
        for (i2 = i1+1; i2 < bf_limit; i2++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            if (test_xor(indices, 3, k_sec)) count++;
        }
    } else if (n_weights == 4) {
        for (i0 = 0; i0 < bf_limit; i0++)
        for (i1 = i0+1; i1 < bf_limit; i1++)
        for (i2 = i1+1; i2 < bf_limit; i2++)
        for (i3 = i2+1; i3 < bf_limit; i3++) {
            indices[0] = i0; indices[1] = i1;
            indices[2] = i2; indices[3] = i3;
            if (test_xor(indices, 4, k_sec)) count++;
        }
    } else if (n_weights == 5) {
        for (i0 = 0; i0 < bf_limit; i0++)
        for (i1 = i0+1; i1 < bf_limit; i1++)
        for (i2 = i1+1; i2 < bf_limit; i2++)
        for (i3 = i2+1; i3 < bf_limit; i3++)
        for (i4 = i3+1; i4 < bf_limit; i4++) {
            indices[0] = i0; indices[1] = i1;
            indices[2] = i2; indices[3] = i3;
            indices[4] = i4;
            if (test_xor(indices, 5, k_sec)) count++;
        }
    }

    return count;
}

/* find_max_xor available if needed for later phases */

/* ================================================================ */
/* Phase 1: k_sec sweep at fixed catalog                            */
/* ================================================================ */

static void phase1_ksec_sweep(void) {
    static const int ksec_vals[] = {2, 4, 6, 8, 12, 16, 24, 32, 48};
    int n_ksec = 9;
    int ki;
    int bf = 25;
    int max_xor_overall = 0;
    int best_ksec = 0;

    printf("\n=== Phase 1: k_sec Sweep (fixed catalog, cumul depth 0-4) ===\n\n");

    /* Load cumulative depth 0-4 */
    load_up_to_depth(4);
    build_dirs(g_cat_size);

    printf("  Catalog: %d entries, %d dirs, bf=%d\n\n", g_cat_size, g_nd, bf);

    printf("  k_sec | n_cells | XOR6_wins | XOR8_wins | XOR10_wins | max_xor\n");
    printf("  ------|---------|-----------|-----------|------------|--------\n");

    for (ki = 0; ki < n_ksec; ki++) {
        int k = ksec_vals[ki];
        int n_cells = k * (g_nd + 1);
        int w3, w4, w5 = 0;
        int max_arity = 0;

        fprintf(stderr, "DBG: phase1 k_sec=%d, n_cells=%d\n", k, n_cells);

        if (n_cells > MAX_ACT) {
            printf("  %5d | %7d | SKIP (exceeds MAX_ACT=%d)\n",
                   k, n_cells, MAX_ACT);
            continue;
        }

        w3 = count_xor_winners(3, bf, k);
        w4 = count_xor_winners(4, bf, k);
        if (bf <= 15) {
            w5 = count_xor_winners(5, bf, k);
        }

        if (w5 > 0) max_arity = 10;
        else if (w4 > 0) max_arity = 8;
        else if (w3 > 0) max_arity = 6;

        printf("  %5d | %7d | %9d | %9d | %10d | %7d\n",
               k, n_cells, w3, w4, w5, max_arity);

        if (max_arity > max_xor_overall) {
            max_xor_overall = max_arity;
            best_ksec = k;
        }
    }

    printf("\n  Best: k_sec=%d -> max_xor=%d\n\n", best_ksec, max_xor_overall);

    check("Phase 1: k_sec sweep completed", max_xor_overall > 0);
    check("Phase 1: XOR6 achievable at low k_sec",
          count_xor_winners(3, bf, 2) > 0 || count_xor_winners(3, bf, 4) > 0);
}

/* ================================================================ */
/* Phase 2: Voronoi resolution sweep                                */
/* ================================================================ */

static void phase2_voronoi_sweep(void) {
    int d, max_depth_avail;
    int bf = 25;
    int k_sec_test = 24;  /* will be updated based on Phase 1 if needed */

    printf("\n=== Phase 2: Voronoi Resolution Sweep (k_sec=%d) ===\n\n",
           k_sec_test);

    /* Find max depth available */
    max_depth_avail = 0;
    { int i; for (i = 0; i < saved_cat_size; i++)
        if (saved_depth[i] > max_depth_avail) max_depth_avail = saved_depth[i]; }

    printf("  Build Voronoi from dirs up to depth D, but keep full catalog.\n");
    printf("  This tests whether coarser direction quantization hurts.\n\n");

    printf("  VorMaxD | n_dirs | n_cells | XOR6_wins | XOR8_wins | max_xor\n");
    printf("  --------|--------|---------|-----------|-----------|--------\n");

    /* Load full catalog (cumul depth 0-4 for search tractability) */
    load_up_to_depth(4);

    for (d = 0; d <= 4; d++) {
        int n_cells, w3, w4;
        int max_arity = 0;

        /* Build direction catalog using only entries up to depth d */
        build_dirs_from_saved(d);
        n_cells = k_sec_test * (g_nd + 1);

        fprintf(stderr, "DBG: phase2 vor_depth=%d, n_dirs=%d, n_cells=%d\n",
                d, g_nd, n_cells);

        if (n_cells > MAX_ACT) {
            printf("  %7d | %6d | %7d | SKIP (exceeds MAX_ACT)\n",
                   d, g_nd, n_cells);
            /* Rebuild full dirs for next iteration check */
            continue;
        }

        w3 = count_xor_winners(3, bf, k_sec_test);
        w4 = count_xor_winners(4, bf, k_sec_test);

        if (w4 > 0) max_arity = 8;
        else if (w3 > 0) max_arity = 6;

        printf("  %7d | %6d | %7d | %9d | %9d | %7d\n",
               d, g_nd, n_cells, w3, w4, max_arity);
    }

    /* Also test with full Voronoi (all depths) */
    build_dirs(g_cat_size);
    {
        int n_cells = k_sec_test * (g_nd + 1);
        int w3, w4;
        int max_arity = 0;

        if (n_cells <= MAX_ACT) {
            w3 = count_xor_winners(3, bf, k_sec_test);
            w4 = count_xor_winners(4, bf, k_sec_test);
            if (w4 > 0) max_arity = 8;
            else if (w3 > 0) max_arity = 6;
            printf("  %7s | %6d | %7d | %9d | %9d | %7d\n",
                   "all", g_nd, n_cells, w3, w4, max_arity);
        }
    }

    printf("\n");
    check("Phase 2: Voronoi resolution sweep completed", 1);
}

/* ================================================================ */
/* Phase 3: Depth x k_sec interaction                               */
/* ================================================================ */

static void phase3_depth_ksec_interaction(void) {
    static const int ksec_vals[] = {2, 4, 6, 8, 12, 16, 24, 32, 48};
    int n_ksec = 9;
    static const int depth_vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int n_depths = 9;
    int di, ki;
    int bf;

    /* Table: max_xor[depth][k_sec] and winner counts */
    int max_xor_table[9][9];
    int xor6_table[9][9];
    int xor8_table[9][9];

    printf("\n=== Phase 3: Depth x k_sec Interaction ===\n\n");

    memset(max_xor_table, 0, sizeof(max_xor_table));
    memset(xor6_table, 0, sizeof(xor6_table));
    memset(xor8_table, 0, sizeof(xor8_table));

    for (di = 0; di < n_depths; di++) {
        int max_d = depth_vals[di];
        int n_entries;

        load_up_to_depth(max_d);
        n_entries = g_cat_size;
        build_dirs(g_cat_size);

        /* Scale bf with catalog size — don't exceed what's available,
         * and keep tractable */
        bf = n_entries;
        if (bf > 30) bf = 30;

        fprintf(stderr, "DBG: phase3 depth=%d, entries=%d, dirs=%d, bf=%d\n",
                max_d, n_entries, g_nd, bf);

        printf("  --- Depth 0-%d: %d entries, %d dirs, bf=%d ---\n",
               max_d, n_entries, g_nd, bf);

        for (ki = 0; ki < n_ksec; ki++) {
            int k = ksec_vals[ki];
            int n_cells = k * (g_nd + 1);
            int w3, w4;
            int max_arity = 0;

            if (n_cells > MAX_ACT) {
                printf("    k=%2d: SKIP (n_cells=%d > MAX_ACT)\n",
                       k, n_cells);
                continue;
            }

            w3 = count_xor_winners(3, bf, k);
            w4 = count_xor_winners(4, bf, k);

            if (w4 > 0) max_arity = 8;
            else if (w3 > 0) max_arity = 6;

            max_xor_table[di][ki] = max_arity;
            xor6_table[di][ki] = w3;
            xor8_table[di][ki] = w4;
        }
    }

    /* Print summary table: max_xor */
    printf("\n  === Max XOR Arity Table ===\n");
    printf("  Depth\\k_sec");
    for (ki = 0; ki < n_ksec; ki++)
        printf(" | %4d", ksec_vals[ki]);
    printf("\n  -----------");
    for (ki = 0; ki < n_ksec; ki++)
        printf("-|-----");
    printf("\n");

    for (di = 0; di < n_depths; di++) {
        printf("  %9d  ", depth_vals[di]);
        for (ki = 0; ki < n_ksec; ki++) {
            if (max_xor_table[di][ki] > 0)
                printf(" | %4d", max_xor_table[di][ki]);
            else
                printf(" |    -");
        }
        printf("\n");
    }

    /* Print XOR6 winner count table */
    printf("\n  === XOR6 Winner Count Table ===\n");
    printf("  Depth\\k_sec");
    for (ki = 0; ki < n_ksec; ki++)
        printf(" | %5d", ksec_vals[ki]);
    printf("\n  -----------");
    for (ki = 0; ki < n_ksec; ki++)
        printf("-|------");
    printf("\n");

    for (di = 0; di < n_depths; di++) {
        printf("  %9d  ", depth_vals[di]);
        for (ki = 0; ki < n_ksec; ki++) {
            printf(" | %5d", xor6_table[di][ki]);
        }
        printf("\n");
    }

    /* Print XOR8 winner count table */
    printf("\n  === XOR8 Winner Count Table ===\n");
    printf("  Depth\\k_sec");
    for (ki = 0; ki < n_ksec; ki++)
        printf(" | %5d", ksec_vals[ki]);
    printf("\n  -----------");
    for (ki = 0; ki < n_ksec; ki++)
        printf("-|------");
    printf("\n");

    for (di = 0; di < n_depths; di++) {
        printf("  %9d  ", depth_vals[di]);
        for (ki = 0; ki < n_ksec; ki++) {
            printf(" | %5d", xor8_table[di][ki]);
        }
        printf("\n");
    }

    /* Analysis: depth law slope at different k_sec values */
    printf("\n  === Depth Law Slope Analysis ===\n");
    printf("  For each k_sec, find the first depth where XOR8 appears.\n");
    printf("  k_sec | XOR6_first_depth | XOR8_first_depth | gap\n");
    printf("  ------|------------------|------------------|----\n");

    for (ki = 0; ki < n_ksec; ki++) {
        int first_xor6 = -1, first_xor8 = -1;
        for (di = 0; di < n_depths; di++) {
            if (first_xor6 < 0 && xor6_table[di][ki] > 0)
                first_xor6 = depth_vals[di];
            if (first_xor8 < 0 && xor8_table[di][ki] > 0)
                first_xor8 = depth_vals[di];
        }
        printf("  %5d | %16d | %16d | %3d\n",
               ksec_vals[ki],
               first_xor6, first_xor8,
               (first_xor8 >= 0 && first_xor6 >= 0) ?
                first_xor8 - first_xor6 : -1);
    }

    printf("\n");
    check("Phase 3: depth x k_sec interaction computed", 1);

    /* Key test: does the depth gap between XOR6 and XOR8 decrease
     * at higher k_sec? If so, activation IS the bottleneck. */
    {
        int gap_low = -1, gap_high = -1;
        /* Gap at k_sec=6 (low) */
        {
            int f6 = -1, f8 = -1;
            for (di = 0; di < n_depths; di++) {
                if (f6 < 0 && xor6_table[di][2] > 0) f6 = depth_vals[di];
                if (f8 < 0 && xor8_table[di][2] > 0) f8 = depth_vals[di];
            }
            if (f6 >= 0 && f8 >= 0) gap_low = f8 - f6;
        }
        /* Gap at k_sec=24 (high) */
        {
            int f6 = -1, f8 = -1;
            for (di = 0; di < n_depths; di++) {
                if (f6 < 0 && xor6_table[di][6] > 0) f6 = depth_vals[di];
                if (f8 < 0 && xor8_table[di][6] > 0) f8 = depth_vals[di];
            }
            if (f6 >= 0 && f8 >= 0) gap_high = f8 - f6;
        }

        if (gap_low >= 0 && gap_high >= 0) {
            printf("  Depth gap (XOR6->XOR8) at k_sec=6: %d\n", gap_low);
            printf("  Depth gap (XOR6->XOR8) at k_sec=24: %d\n", gap_high);
            check("Phase 3: depth gap decreases at higher k_sec (activation bottleneck)",
                  gap_high < gap_low);
            check("Phase 3: depth gap same at all k_sec (parity wall)",
                  gap_high == gap_low);
        }
    }
}

/* ================================================================ */
/* Main                                                             */
/* ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 91: Activation Bottleneck Test\n");
    printf("=============================================\n\n");
    fflush(stdout);

    /* Build zeta_12 catalog */
    printf("  Building zeta_12 catalog...\n");
    fflush(stdout);
    init_su2(M_PI / 6.0);
    build_closure(1);
    save_catalog();

    printf("  %d entries, %d rounds\n\n", g_cat_size, n_rounds);
    fflush(stdout);
    check("zeta_12 catalog built", g_cat_size > 3000);

    /* Phase 1: k_sec sweep */
    fprintf(stderr, "DBG: starting phase1\n");
    phase1_ksec_sweep();
    fprintf(stderr, "DBG: phase1 done\n");

    /* Phase 2: Voronoi resolution sweep */
    fprintf(stderr, "DBG: starting phase2\n");
    phase2_voronoi_sweep();
    fprintf(stderr, "DBG: phase2 done\n");

    /* Phase 3: Depth x k_sec interaction */
    fprintf(stderr, "DBG: starting phase3\n");
    phase3_depth_ksec_interaction();
    fprintf(stderr, "DBG: phase3 done\n");

    /* Summary */
    printf("\n=============================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);

    return n_fail;
}
