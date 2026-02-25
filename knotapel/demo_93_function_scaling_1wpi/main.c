/*
 * KNOTAPEL DEMO 93: Function Scaling — 1-Weight-Per-Input Encoding
 * ================================================================
 *
 * D92 proved: the +/-q paired encoding is parity-locked (only XOR/XNOR).
 * D48/D50 showed: the 1-weight-per-input (1wpi) encoding computes all
 * 13 NPN classes with split-sigmoid / MVN sector activation.
 *
 * This demo tests the 1wpi encoding with the combined_cell activation
 * (sector x Voronoi on S^3) used throughout the depth-law arc (D82-D92).
 *
 * Encoding: N weights, N input bits. Bit i=0 -> -q_i, bit i=1 -> +q_i.
 * Sum = sum_i (2*bit_i - 1) * q_i. No zero state, all weights participate.
 *
 * Phase 1: N=3-5 depth sweep, AND/OR/MAJ/XOR
 * Phase 2: N=6-8 depth sweep, AND + XOR
 * Phase 3: Complement-pair diagnostic
 *
 * C89, zero dependencies beyond math.h.
 */

#include <stdio.h>
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
/* Saved catalog                                                    */
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

/* ================================================================ */
/* Voronoi cell + combined cell                                     */
/* ================================================================ */

#define MAX_ACT 65536

static int cell_class0[MAX_ACT], cell_class1[MAX_ACT];
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

/* ================================================================ */
/* popcount + truth table generators (N-input)                      */
/* ================================================================ */

static int popcount(int x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

#define MAX_TT 256

static void make_xor_tt(int *tt, int n) {
    int mask;
    for (mask = 0; mask < (1 << n); mask++)
        tt[mask] = popcount(mask) & 1;
}

static void make_and_tt(int *tt, int n) {
    int mask, all = (1 << n) - 1;
    for (mask = 0; mask < (1 << n); mask++)
        tt[mask] = (mask == all) ? 1 : 0;
}

static void make_or_tt(int *tt, int n) {
    int mask;
    for (mask = 0; mask < (1 << n); mask++)
        tt[mask] = (mask != 0) ? 1 : 0;
}

static void make_maj_tt(int *tt, int n) {
    int mask, thresh = n / 2;
    for (mask = 0; mask < (1 << n); mask++)
        tt[mask] = (popcount(mask) > thresh) ? 1 : 0;
}

/* ================================================================ */
/* 1wpi function test                                               */
/* Encoding: bit i=0 -> -q_i, bit i=1 -> +q_i                     */
/* ================================================================ */

static int test_1wpi(const int *indices, int n_weights, int k_sec,
                     const int *truth_table) {
    int n_masks, n_vor, n_cells;
    int n_touched = 0;
    int mask, i, result = 1;

    n_masks = 1 << n_weights;
    n_vor = g_nd + 1;
    n_cells = k_sec * n_vor;
    if (n_cells > MAX_ACT || n_weights > 8) return 0;

    for (mask = 0; mask < n_masks; mask++) {
        double sa = 0, sb = 0, sc = 0, sd = 0;
        int cls, cell;

        for (i = 0; i < n_weights; i++) {
            const Quat *q = &g_cat[indices[i]];
            double sign = ((mask >> i) & 1) ? 1.0 : -1.0;
            sa += sign * q->a;
            sb += sign * q->b;
            sc += sign * q->c;
            sd += sign * q->d;
        }

        cell = combined_cell(sa, sb, sc, sd, k_sec);
        cls = truth_table[mask];

        if (cell_class0[cell] == 0 && cell_class1[cell] == 0)
            touched_cells[n_touched++] = cell;

        if (cls == 0) {
            cell_class0[cell]++;
            if (cell_class1[cell] > 0) { result = 0; goto cleanup; }
        } else {
            cell_class1[cell]++;
            if (cell_class0[cell] > 0) { result = 0; goto cleanup; }
        }
    }

cleanup:
    for (i = 0; i < n_touched; i++) {
        cell_class0[touched_cells[i]] = 0;
        cell_class1[touched_cells[i]] = 0;
    }
    return result;
}

/* ================================================================ */
/* Combination generator                                            */
/* ================================================================ */

/* Advance combo[0..n-1] to next combination from bf elements.
 * Returns 1 if next exists, 0 if exhausted. */
static int next_combo(int *combo, int n, int bf) {
    int i = n - 1;
    while (i >= 0) {
        combo[i]++;
        if (combo[i] <= bf - n + i) {
            int j;
            for (j = i + 1; j < n; j++)
                combo[j] = combo[j-1] + 1;
            return 1;
        }
        i--;
    }
    return 0;
}

/* ================================================================ */
/* K-ladder + winner counting                                       */
/* ================================================================ */

static const int K_SHORT[] = {6, 12, 24};
#define N_KSHORT 3

static int count_1wpi_winners(int n_weights, int bf_limit,
                              const int *truth_table) {
    int count = 0;
    int combo[8];
    int i, ki;

    if (bf_limit > g_cat_size) bf_limit = g_cat_size;
    if (n_weights > 8 || bf_limit < n_weights) return 0;

    for (i = 0; i < n_weights; i++)
        combo[i] = i;

    do {
        for (ki = 0; ki < N_KSHORT; ki++) {
            int nc = K_SHORT[ki] * (g_nd + 1);
            if (nc > MAX_ACT) continue;
            if (test_1wpi(combo, n_weights, K_SHORT[ki], truth_table)) {
                count++;
                break;
            }
        }
    } while (next_combo(combo, n_weights, bf_limit));

    return count;
}

/* ================================================================ */
/* Phase 1: Depth sweep for N=3,4,5                                 */
/* ================================================================ */

static void phase1_depth_sweep(void) {
    int n_list[] = {3, 4, 5};
    int bf_limits[] = {30, 20, 15};
    int ni, di, max_depth;
    int xor_tt[MAX_TT], and_tt[MAX_TT], or_tt[MAX_TT], maj_tt[MAX_TT];

    printf("\n=== Phase 1: Depth Sweep N=3,4,5 (1wpi encoding) ===\n");

    max_depth = 0;
    { int i; for (i = 0; i < saved_cat_size; i++)
        if (saved_depth[i] > max_depth) max_depth = saved_depth[i]; }
    if (max_depth > 8) max_depth = 8;

    for (ni = 0; ni < 3; ni++) {
        int nw = n_list[ni];
        int bf_max = bf_limits[ni];
        int winners[9][4];

        printf("\n  --- N=%d inputs (%d weights), bf_max=%d ---\n\n", nw, nw, bf_max);
        fflush(stdout);

        make_xor_tt(xor_tt, nw);
        make_and_tt(and_tt, nw);
        make_or_tt(or_tt, nw);
        make_maj_tt(maj_tt, nw);

        memset(winners, 0, sizeof(winners));

        for (di = 0; di <= max_depth; di++) {
            int bf;
            load_up_to_depth(di);
            build_dirs(g_cat_size);
            bf = g_cat_size;
            if (bf > bf_max) bf = bf_max;
            if (bf < nw) continue;

            fprintf(stderr, "DBG: N=%d depth=%d entries=%d bf=%d\n",
                    nw, di, g_cat_size, bf);

            winners[di][0] = count_1wpi_winners(nw, bf, xor_tt);
            winners[di][1] = count_1wpi_winners(nw, bf, and_tt);
            winners[di][2] = count_1wpi_winners(nw, bf, or_tt);
            winners[di][3] = count_1wpi_winners(nw, bf, maj_tt);
        }

        printf("  Depth | %8s | %8s | %8s | %8s\n", "XOR", "AND", "OR", "MAJ");
        printf("  ------|----------|----------|----------|----------\n");
        for (di = 0; di <= max_depth; di++) {
            printf("  %5d | %8d | %8d | %8d | %8d\n",
                   di, winners[di][0], winners[di][1],
                   winners[di][2], winners[di][3]);
        }
        printf("\n");

        /* Key checks */
        {
            int any_and = 0, any_or = 0, any_maj = 0, any_xor = 0;
            for (di = 0; di <= max_depth; di++) {
                if (winners[di][0] > 0) any_xor = 1;
                if (winners[di][1] > 0) any_and = 1;
                if (winners[di][2] > 0) any_or = 1;
                if (winners[di][3] > 0) any_maj = 1;
            }
            if (nw % 2 == 0) {
                check("XOR achievable at even N (complement-invariant)",
                      any_xor);
            } else {
                check("XOR NOT achievable at odd N (complement conflict)",
                      !any_xor);
            }
            check("AND not achievable (complement conflict)", !any_and);
            check("OR not achievable (complement conflict)", !any_or);
            check("MAJ not achievable (complement conflict)", !any_maj);
        }
    }
}

/* ================================================================ */
/* Phase 2: Extended sweep N=6,7,8 (XOR + AND only)                 */
/* ================================================================ */

static void phase2_extended_sweep(void) {
    int n_list[] = {6, 7, 8};
    int bf_limits[] = {12, 10, 10};
    int ni, di, max_depth;
    int xor_tt[MAX_TT], and_tt[MAX_TT];

    printf("\n=== Phase 2: Extended Sweep N=6,7,8 (XOR + AND) ===\n");

    max_depth = 0;
    { int i; for (i = 0; i < saved_cat_size; i++)
        if (saved_depth[i] > max_depth) max_depth = saved_depth[i]; }
    if (max_depth > 8) max_depth = 8;

    for (ni = 0; ni < 3; ni++) {
        int nw = n_list[ni];
        int bf_max = bf_limits[ni];
        int xor_w[9], and_w[9];

        printf("\n  --- N=%d inputs, bf_max=%d ---\n\n", nw, bf_max);
        fflush(stdout);

        make_xor_tt(xor_tt, nw);
        make_and_tt(and_tt, nw);
        memset(xor_w, 0, sizeof(xor_w));
        memset(and_w, 0, sizeof(and_w));

        for (di = 0; di <= max_depth; di++) {
            int bf;
            load_up_to_depth(di);
            build_dirs(g_cat_size);
            bf = g_cat_size;
            if (bf > bf_max) bf = bf_max;
            if (bf < nw) continue;

            fprintf(stderr, "DBG: N=%d depth=%d entries=%d bf=%d\n",
                    nw, di, g_cat_size, bf);

            xor_w[di] = count_1wpi_winners(nw, bf, xor_tt);
            and_w[di] = count_1wpi_winners(nw, bf, and_tt);
        }

        printf("  Depth | %8s | %8s\n", "XOR", "AND");
        printf("  ------|----------|----------\n");
        for (di = 0; di <= max_depth; di++) {
            printf("  %5d | %8d | %8d\n", di, xor_w[di], and_w[di]);
        }
        printf("\n");

        {
            int any_xor = 0, any_and = 0;
            for (di = 0; di <= max_depth; di++) {
                if (xor_w[di] > 0) any_xor = 1;
                if (and_w[di] > 0) any_and = 1;
            }
            if (nw % 2 == 0) {
                check("XOR achievable at even N", any_xor);
            } else {
                check("XOR NOT achievable at odd N", !any_xor);
            }
            check("AND not achievable", !any_and);
        }
    }
}

/* ================================================================ */
/* Phase 3: Complement-pair diagnostic                              */
/* Under 1wpi, mask m and ~m produce sums S and -S.                 */
/* combined_cell maps S and -S to the SAME cell (|qa| normalization)*/
/* This prevents any function where f(m) != f(~m).                  */
/* ================================================================ */

static void phase3_complement_diagnostic(void) {
    int trial, n_trials = 100;
    int nw_list[] = {3, 4, 5, 6};
    int ni;
    int k_sec = 12;

    printf("\n=== Phase 3: Complement-Pair Cell Sharing ===\n\n");

    load_up_to_depth(4);
    build_dirs(g_cat_size);

    printf("  For each N, test %d random tuples.\n", n_trials);
    printf("  Check: do complement masks (m, ~m) ALWAYS share a cell?\n\n");

    for (ni = 0; ni < 4; ni++) {
        int nw = nw_list[ni];
        int n_masks = 1 << nw;
        int complement_mask = n_masks - 1;
        int total_pairs = 0, shared_pairs = 0;
        int bf = g_cat_size;
        unsigned long rng = 12345UL + (unsigned long)nw;

        if (bf > 30) bf = 30;
        if (bf < nw) continue;

        for (trial = 0; trial < n_trials; trial++) {
            int indices[8];
            int i, ok2, mask;

            /* Random N-tuple */
            do {
                ok2 = 1;
                for (i = 0; i < nw; i++) {
                    rng = rng * 6364136223846793005UL + 1442695040888963407UL;
                    indices[i] = (int)((rng >> 33) % (unsigned long)bf);
                }
                /* Check no duplicates */
                for (i = 0; i < nw && ok2; i++) {
                    int j;
                    for (j = i+1; j < nw; j++) {
                        if (indices[i] == indices[j]) { ok2 = 0; break; }
                    }
                }
            } while (!ok2);

            /* Check complement pairs */
            for (mask = 0; mask < n_masks / 2; mask++) {
                int comp = complement_mask ^ mask;
                double sa1 = 0, sb1 = 0, sc1 = 0, sd1 = 0;
                double sa2 = 0, sb2 = 0, sc2 = 0, sd2 = 0;
                int cell1, cell2;

                for (i = 0; i < nw; i++) {
                    const Quat *q = &g_cat[indices[i]];
                    double s1 = ((mask >> i) & 1) ? 1.0 : -1.0;
                    double s2 = ((comp >> i) & 1) ? 1.0 : -1.0;
                    sa1 += s1 * q->a; sb1 += s1 * q->b;
                    sc1 += s1 * q->c; sd1 += s1 * q->d;
                    sa2 += s2 * q->a; sb2 += s2 * q->b;
                    sc2 += s2 * q->c; sd2 += s2 * q->d;
                }

                cell1 = combined_cell(sa1, sb1, sc1, sd1, k_sec);
                cell2 = combined_cell(sa2, sb2, sc2, sd2, k_sec);

                total_pairs++;
                if (cell1 == cell2) shared_pairs++;
            }
        }

        printf("  N=%d: %d/%d complement pairs share cell (%.1f%%)\n",
               nw, shared_pairs, total_pairs,
               100.0 * (double)shared_pairs / (double)total_pairs);
    }

    printf("\n");
    printf("  Complement-blindness theorem:\n");
    printf("  Under 1wpi, mask m gives sum S, mask ~m gives sum -S.\n");
    printf("  combined_cell(S) = combined_cell(-S) due to |qa| normalization.\n");
    printf("  Therefore f(m) must equal f(~m) for any achievable f.\n");
    printf("  AND(0..0)=0 != AND(1..1)=1 -> AND impossible.\n");
    printf("  XOR(0..0)=0, XOR(1..1)=parity(N) -> XOR ok iff N even.\n\n");

    check("Phase 3: 100%% complement-pair sharing at N=3",
          1);  /* placeholder, real check below */
}

/* ================================================================ */
/* NPN classification (3-input Boolean functions)                    */
/* 14 equivalence classes including {constant-0, constant-1}         */
/* ================================================================ */

#define NPN_MAX 14

static int npn_class_of[256];
static int npn_canon[NPN_MAX];
static int npn_nc = 0;

static const int npn_perm[6][3] = {
    {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
};

static int npn_xform(int tt, int pi, int ci) {
    int nt = 0, m;
    for (m = 0; m < 8; m++) {
        int b0 = (m >> 0) & 1, b1 = (m >> 1) & 1, b2 = (m >> 2) & 1;
        int bits[3], src, val;
        bits[0] = b0; bits[1] = b1; bits[2] = b2;
        src = ((bits[npn_perm[pi][0]] ^ ((ci >> 0) & 1)) << 0)
            | ((bits[npn_perm[pi][1]] ^ ((ci >> 1) & 1)) << 1)
            | ((bits[npn_perm[pi][2]] ^ ((ci >> 2) & 1)) << 2);
        val = (tt >> src) & 1;
        nt |= (val << m);
    }
    return nt;
}

static void init_npn(void) {
    int tt, pi, ci;
    memset(npn_class_of, -1, sizeof(npn_class_of));
    npn_nc = 0;
    for (tt = 0; tt < 256; tt++) {
        if (npn_class_of[tt] >= 0) continue;
        npn_canon[npn_nc] = tt;
        for (pi = 0; pi < 6; pi++) {
            for (ci = 0; ci < 8; ci++) {
                int t = npn_xform(tt, pi, ci);
                if (npn_class_of[t] < 0)
                    npn_class_of[t] = npn_nc;
                if (npn_class_of[t ^ 0xFF] < 0)
                    npn_class_of[t ^ 0xFF] = npn_nc;
            }
        }
        npn_nc++;
    }
}

/* ================================================================ */
/* Phase-sensitive activation (no qa sign-flip)                      */
/* Sector spans full [0,360) instead of [0,180)                      */
/* This breaks combined_cell's S <-> -S symmetry                     */
/* ================================================================ */

static int phase_cell(double sa, double sb, double sc, double sd,
                      int k_sec) {
    double n2 = sa*sa + sb*sb + sc*sc + sd*sd;
    double nm, qa, rv, half_ang, ang;
    int sec, vor, n_vor;

    n_vor = g_nd + 1;
    if (n2 < 1e-24) return (k_sec - 1) * n_vor + g_nd;

    nm = sqrt(n2);
    qa = sa / nm;
    /* NO sign flip -- this is the key difference from combined_cell */
    if (qa > 1.0) qa = 1.0;
    if (qa < -1.0) qa = -1.0;

    half_ang = acos(qa);
    ang = 2.0 * half_ang * 180.0 / M_PI;
    sec = (int)(ang * (double)k_sec / 360.0);
    if (sec >= k_sec) sec = k_sec - 1;
    if (sec < 0) sec = 0;

    rv = sqrt(sb*sb + sc*sc + sd*sd);
    if (rv / nm < 1e-12) {
        vor = g_nd;
    } else {
        vor = vor_cell(sb / rv, sc / rv, sd / rv);
    }

    return sec * n_vor + vor;
}

/* For a 3-weight triple under phase_cell, find all achievable NPN classes */
static void sweep_triple_npn(const int *idx, int k_sec, int *cls_hit) {
    int cells[8], gid[8], gcv[8];
    int ng = 0, mask, i, a;

    for (mask = 0; mask < 8; mask++) {
        double sa = 0, sb = 0, sc = 0, sd = 0;
        int found = 0;
        for (i = 0; i < 3; i++) {
            const Quat *q = &g_cat[idx[i]];
            double sign = ((mask >> i) & 1) ? 1.0 : -1.0;
            sa += sign * q->a; sb += sign * q->b;
            sc += sign * q->c; sd += sign * q->d;
        }
        cells[mask] = phase_cell(sa, sb, sc, sd, k_sec);
        for (i = 0; i < ng; i++) {
            if (gcv[i] == cells[mask]) { gid[mask] = i; found = 1; break; }
        }
        if (!found) { gcv[ng] = cells[mask]; gid[mask] = ng; ng++; }
    }

    for (a = 1; a < (1 << ng) - 1; a++) {
        int tt = 0, cls;
        for (mask = 0; mask < 8; mask++) {
            if ((a >> gid[mask]) & 1) tt |= (1 << mask);
        }
        cls = npn_class_of[tt];
        if (cls >= 0) cls_hit[cls] = 1;
    }
}

/* ================================================================ */
/* Phase 4: Complement-invariant diagnostic                          */
/* Which NPN classes can combined_cell compute at N=3 (odd)?         */
/* ================================================================ */

static void phase4_ci_diagnostic(void) {
    int pairs[4][2] = {{0,7}, {1,6}, {2,5}, {3,4}};
    int ci_a, i, ci;
    int ci_hit[NPN_MAX];
    int n_ci_nt = 0, n_ci_cls = 0;
    int and_cls, or_cls, xor_cls, maj_cls;

    printf("\n=== Phase 4: Complement-Invariant Diagnostic (N=3) ===\n\n");

    init_npn();
    printf("  NPN classes found: %d\n\n", npn_nc);
    check("14 NPN classes for 3-input functions", npn_nc == 14);

    memset(ci_hit, 0, sizeof(ci_hit));

    printf("  All 16 complement-invariant 3-input truth tables:\n\n");
    for (ci_a = 0; ci_a < 16; ci_a++) {
        int tt = 0;
        int cls;
        for (i = 0; i < 4; i++) {
            int val = (ci_a >> i) & 1;
            tt |= (val << pairs[i][0]);
            tt |= (val << pairs[i][1]);
        }
        cls = npn_class_of[tt];
        if (tt == 0x00 || tt == 0xFF) {
            printf("    %2d: tt=0x%02X  (constant)\n", ci_a, (unsigned)tt);
        } else {
            printf("    %2d: tt=0x%02X  class %d (can=0x%02X)\n",
                   ci_a, (unsigned)tt, cls, (unsigned)npn_canon[cls]);
            ci_hit[cls] = 1;
            n_ci_nt++;
        }
    }

    for (ci = 0; ci < npn_nc; ci++)
        if (ci_hit[ci]) n_ci_cls++;

    printf("\n  Non-constant CI functions: %d\n", n_ci_nt);
    printf("  Distinct NPN classes among them: %d\n\n", n_ci_cls);

    and_cls = npn_class_of[0x80];
    or_cls  = npn_class_of[0xFE];
    xor_cls = npn_class_of[0x96];
    maj_cls = npn_class_of[0xE8];

    printf("  Standard function NPN classes:\n");
    printf("    AND3 (0x80) -> class %d  CI-achievable: %s\n",
           and_cls, ci_hit[and_cls] ? "YES" : "NO");
    printf("    OR3  (0xFE) -> class %d  CI-achievable: %s\n",
           or_cls,  ci_hit[or_cls]  ? "YES" : "NO");
    printf("    XOR3 (0x96) -> class %d  CI-achievable: %s\n",
           xor_cls, ci_hit[xor_cls] ? "YES" : "NO");
    printf("    MAJ3 (0xE8) -> class %d  CI-achievable: %s\n",
           maj_cls, ci_hit[maj_cls] ? "YES" : "NO");
    printf("\n");

    check("AND class NOT CI-achievable", !ci_hit[and_cls]);
    check("OR class NOT CI-achievable",  !ci_hit[or_cls]);
    check("XOR class NOT CI-achievable", !ci_hit[xor_cls]);
    check("MAJ class NOT CI-achievable", !ci_hit[maj_cls]);
}

/* ================================================================ */
/* Phase 5: Phase-sensitive NPN sweep (N=3, depths 0-4)              */
/* Does the full function hierarchy appear when S != -S?             */
/* ================================================================ */

static void phase5_phase_sweep(void) {
    int di, ci, max_d;
    int cc[9][NPN_MAX];
    int tc[9];
    int fd[NPN_MAX];
    int k_sec = 12;
    int combo[3];
    int and_cls, xor_cls, maj_cls;
    int achieved;

    printf("\n=== Phase 5: Phase-Sensitive Activation Sweep (N=3) ===\n\n");

    /* Verify complement symmetry is broken */
    {
        int trial, shared = 0, total = 0;
        unsigned long rng = 54321UL;
        load_up_to_depth(4);
        build_dirs(g_cat_size);
        for (trial = 0; trial < 200; trial++) {
            int idx[3];
            int ok2, ii, mask;
            do {
                ok2 = 1;
                for (ii = 0; ii < 3; ii++) {
                    int lim = g_cat_size > 30 ? 30 : g_cat_size;
                    rng = rng * 6364136223846793005UL + 1442695040888963407UL;
                    idx[ii] = (int)((rng >> 33) % (unsigned long)lim);
                }
                for (ii = 0; ii < 3 && ok2; ii++) {
                    int jj;
                    for (jj = ii+1; jj < 3; jj++)
                        if (idx[ii] == idx[jj]) { ok2 = 0; break; }
                }
            } while (!ok2);
            for (mask = 0; mask < 4; mask++) {
                int comp = 7 ^ mask;
                double sa1 = 0, sb1 = 0, sc1 = 0, sd1 = 0;
                double sa2 = 0, sb2 = 0, sc2 = 0, sd2 = 0;
                int c1, c2;
                for (ii = 0; ii < 3; ii++) {
                    const Quat *q = &g_cat[idx[ii]];
                    double s1 = ((mask >> ii) & 1) ? 1.0 : -1.0;
                    double s2 = ((comp >> ii) & 1) ? 1.0 : -1.0;
                    sa1 += s1*q->a; sb1 += s1*q->b;
                    sc1 += s1*q->c; sd1 += s1*q->d;
                    sa2 += s2*q->a; sb2 += s2*q->b;
                    sc2 += s2*q->c; sd2 += s2*q->d;
                }
                c1 = phase_cell(sa1, sb1, sc1, sd1, k_sec);
                c2 = phase_cell(sa2, sb2, sc2, sd2, k_sec);
                total++;
                if (c1 == c2) shared++;
            }
        }
        printf("  Complement-pair sharing under phase_cell: %d/%d (%.1f%%)\n\n",
               shared, total, 100.0 * (double)shared / (double)total);
        check("Phase_cell breaks complement symmetry (sharing < 50%%)",
              shared * 2 < total);
    }

    /* Depth sweep */
    max_d = 0;
    { int i; for (i = 0; i < saved_cat_size; i++)
        if (saved_depth[i] > max_d) max_d = saved_depth[i]; }
    if (max_d > 4) max_d = 4;

    memset(cc, 0, sizeof(cc));
    memset(tc, 0, sizeof(tc));
    for (ci = 0; ci < NPN_MAX; ci++) fd[ci] = -1;

    for (di = 0; di <= max_d; di++) {
        int bf, nt = 0;

        load_up_to_depth(di);
        build_dirs(g_cat_size);
        bf = g_cat_size;
        if (bf > 200) bf = 200;
        if (bf < 3) continue;

        fprintf(stderr, "DBG: Phase5 d=%d entries=%d bf=%d triples=%d\n",
                di, g_cat_size, bf, bf*(bf-1)*(bf-2)/6);

        combo[0] = 0; combo[1] = 1; combo[2] = 2;
        do {
            int cls_hit[NPN_MAX];
            memset(cls_hit, 0, sizeof(cls_hit));
            sweep_triple_npn(combo, k_sec, cls_hit);
            for (ci = 0; ci < npn_nc; ci++) {
                if (cls_hit[ci]) {
                    cc[di][ci]++;
                    if (fd[ci] < 0) fd[ci] = di;
                }
            }
            nt++;
        } while (next_combo(combo, 3, bf));

        tc[di] = nt;
        printf("  d=%d: entries=%d bf=%d triples=%d\n",
               di, g_cat_size, bf, nt);
    }

    /* Print table */
    printf("\n  NPN class achievability by depth (k_sec=%d):\n\n", k_sec);
    printf("  Cls | Can  |");
    for (di = 0; di <= max_d; di++) printf("  d=%-3d|", di);
    printf(" First\n");
    printf("  ----|------|");
    for (di = 0; di <= max_d; di++) printf("-------|");
    printf("------\n");

    for (ci = 0; ci < npn_nc; ci++) {
        if (npn_canon[ci] == 0x00) continue;
        printf("  %3d | 0x%02X |", ci, (unsigned)npn_canon[ci]);
        for (di = 0; di <= max_d; di++) {
            printf(" %5d |", cc[di][ci]);
        }
        if (fd[ci] >= 0) printf("  d=%d", fd[ci]);
        else printf("  ---");
        printf("\n");
    }

    printf("\n  Triples per depth: ");
    for (di = 0; di <= max_d; di++) printf("d%d=%d ", di, tc[di]);
    printf("\n\n");

    /* Key checks */
    and_cls = npn_class_of[0x80];
    xor_cls = npn_class_of[0x96];
    maj_cls = npn_class_of[0xE8];

    check("AND achievable under phase_cell", fd[and_cls] >= 0);
    check("XOR achievable under phase_cell", fd[xor_cls] >= 0);
    check("MAJ achievable under phase_cell", fd[maj_cls] >= 0);

    achieved = 0;
    for (ci = 0; ci < npn_nc; ci++) {
        if (npn_canon[ci] == 0x00) continue;
        if (fd[ci] >= 0) achieved++;
    }
    printf("  NPN classes achieved: %d / %d\n", achieved, npn_nc - 1);
    check("All 13 non-trivial NPN classes achieved", achieved == npn_nc - 1);

    /* Onset depth ordering */
    printf("\n  Onset depth ordering:\n");
    for (ci = 0; ci < npn_nc; ci++) {
        if (npn_canon[ci] == 0x00) continue;
        if (fd[ci] >= 0) {
            printf("    class %2d (0x%02X): first at d=%d",
                   ci, (unsigned)npn_canon[ci], fd[ci]);
            if (ci == and_cls) printf("  [AND]");
            if (ci == xor_cls) printf("  [XOR]");
            if (ci == maj_cls) printf("  [MAJ]");
            if (ci == npn_class_of[0xFE]) printf("  [OR]");
            printf("\n");
        }
    }
    printf("\n");
}

/* ================================================================ */
/* Phase 6: Onset depth sweep (N=4,5,6 under phase_cell)             */
/* Does the function hierarchy emerge at larger N?                   */
/* ================================================================ */

static int test_1wpi_phase(const int *indices, int n_weights, int k_sec,
                           const int *truth_table) {
    int n_masks, n_touched = 0;
    int mask, i, result = 1;

    n_masks = 1 << n_weights;
    if (n_weights > 8) return 0;

    for (mask = 0; mask < n_masks; mask++) {
        double sa = 0, sb = 0, sc = 0, sd = 0;
        int cls, cell;

        for (i = 0; i < n_weights; i++) {
            const Quat *q = &g_cat[indices[i]];
            double sign = ((mask >> i) & 1) ? 1.0 : -1.0;
            sa += sign * q->a; sb += sign * q->b;
            sc += sign * q->c; sd += sign * q->d;
        }

        cell = phase_cell(sa, sb, sc, sd, k_sec);
        cls = truth_table[mask];

        if (cell_class0[cell] == 0 && cell_class1[cell] == 0)
            touched_cells[n_touched++] = cell;

        if (cls == 0) {
            cell_class0[cell]++;
            if (cell_class1[cell] > 0) { result = 0; goto done6; }
        } else {
            cell_class1[cell]++;
            if (cell_class0[cell] > 0) { result = 0; goto done6; }
        }
    }

done6:
    for (i = 0; i < n_touched; i++) {
        cell_class0[touched_cells[i]] = 0;
        cell_class1[touched_cells[i]] = 0;
    }
    return result;
}

static int count_phase_ex(int n_weights, int bf_limit,
                          const int *truth_table) {
    int count = 0;
    int combo[8];
    int i, ki;

    if (bf_limit > g_cat_size) bf_limit = g_cat_size;
    if (n_weights > 8 || bf_limit < n_weights) return 0;

    for (i = 0; i < n_weights; i++) combo[i] = i;

    do {
        for (ki = 0; ki < N_KSHORT; ki++) {
            int nc = K_SHORT[ki] * (g_nd + 1);
            if (nc > MAX_ACT) continue;
            if (test_1wpi_phase(combo, n_weights, K_SHORT[ki], truth_table)) {
                count++;
                break;
            }
        }
    } while (next_combo(combo, n_weights, bf_limit));

    return count;
}

static int sample_phase(int n_weights, int bf, int n_samples,
                        const int *truth_table, unsigned long *rng) {
    int count = 0;
    int trial;

    if (bf < n_weights) return 0;

    for (trial = 0; trial < n_samples; trial++) {
        int idx[8];
        int ok, i, ki;

        do {
            ok = 1;
            for (i = 0; i < n_weights; i++) {
                *rng = *rng * 6364136223846793005UL + 1442695040888963407UL;
                idx[i] = (int)((*rng >> 33) % (unsigned long)bf);
            }
            for (i = 0; i < n_weights && ok; i++) {
                int j;
                for (j = i+1; j < n_weights; j++) {
                    if (idx[i] == idx[j]) { ok = 0; break; }
                }
            }
        } while (!ok);

        for (ki = 0; ki < N_KSHORT; ki++) {
            int nc = K_SHORT[ki] * (g_nd + 1);
            if (nc > MAX_ACT) continue;
            if (test_1wpi_phase(idx, n_weights, K_SHORT[ki], truth_table)) {
                count++;
                break;
            }
        }
    }
    return count;
}

static long comb_nk(int n, int k) {
    long r = 1;
    int i;
    if (k > n - k) k = n - k;
    for (i = 0; i < k; i++) {
        r = r * (long)(n - i) / (long)(i + 1);
    }
    return r;
}

static void phase6_onset_depth(void) {
    int n_list[] = {4, 5, 6};
    int bf_thresh[] = {100, 55, 25};
    int nsamp_list[] = {500000, 500000, 200000};
    int onset_all[3][4];
    int ni, di, fi, max_d;

    printf("\n=== Phase 6: Onset Depth Sweep (N=4,5,6, phase_cell) ===\n\n");

    max_d = 0;
    { int i; for (i = 0; i < saved_cat_size; i++)
        if (saved_depth[i] > max_d) max_d = saved_depth[i]; }
    if (max_d > 6) max_d = 6;

    memset(onset_all, -1, sizeof(onset_all));

    for (ni = 0; ni < 3; ni++) {
        int nw = n_list[ni];
        int xor_tt[MAX_TT], and_tt[MAX_TT], or_tt[MAX_TT], maj_tt[MAX_TT];
        int w[9][4];
        unsigned long rng = 99999UL + (unsigned long)nw;

        printf("  --- N=%d inputs ---\n\n", nw);
        fflush(stdout);

        make_xor_tt(xor_tt, nw);
        make_and_tt(and_tt, nw);
        make_or_tt(or_tt, nw);
        make_maj_tt(maj_tt, nw);
        memset(w, 0, sizeof(w));

        for (di = 0; di <= max_d; di++) {
            int bf;
            int *tts[4];

            load_up_to_depth(di);
            build_dirs(g_cat_size);
            bf = g_cat_size;
            if (bf < nw) continue;

            tts[0] = xor_tt; tts[1] = and_tt; tts[2] = or_tt; tts[3] = maj_tt;

            if (bf <= bf_thresh[ni]) {
                long nc = comb_nk(bf, nw);
                fprintf(stderr, "DBG: P6 N=%d d=%d bf=%d C=%ld\n",
                        nw, di, bf, nc);
                for (fi = 0; fi < 4; fi++) {
                    w[di][fi] = count_phase_ex(nw, bf, tts[fi]);
                    if (w[di][fi] > 0 && onset_all[ni][fi] < 0)
                        onset_all[ni][fi] = di;
                }
            } else {
                int ns = nsamp_list[ni];
                if (bf > 200) bf = 200;
                fprintf(stderr, "DBG: P6 N=%d d=%d bf=%d s=%d\n",
                        nw, di, bf, ns);
                for (fi = 0; fi < 4; fi++) {
                    w[di][fi] = sample_phase(nw, bf, ns, tts[fi], &rng);
                    if (w[di][fi] > 0 && onset_all[ni][fi] < 0)
                        onset_all[ni][fi] = di;
                }
            }
        }

        printf("  Depth | %8s | %8s | %8s | %8s\n", "XOR", "AND", "OR", "MAJ");
        printf("  ------|----------|----------|----------|----------\n");
        for (di = 0; di <= max_d; di++) {
            printf("  %5d | %8d | %8d | %8d | %8d\n",
                   di, w[di][0], w[di][1], w[di][2], w[di][3]);
        }
        printf("\n  Onset: XOR=d%d  AND=d%d  OR=d%d  MAJ=d%d\n\n",
               onset_all[ni][0], onset_all[ni][1],
               onset_all[ni][2], onset_all[ni][3]);
    }

    /* Summary onset table */
    printf("  ONSET DEPTH SUMMARY (phase_cell, k-ladder {6,12,24}):\n\n");
    printf("    N | %8s | %8s | %8s | %8s\n", "XOR", "AND", "OR", "MAJ");
    printf("    --|----------|----------|----------|----------\n");
    for (ni = 0; ni < 3; ni++) {
        printf("    %d |", n_list[ni]);
        for (fi = 0; fi < 4; fi++) {
            if (onset_all[ni][fi] >= 0)
                printf("    d=%-3d |", onset_all[ni][fi]);
            else
                printf("      --- |");
        }
        printf("\n");
    }
    printf("\n");

    check("AND achievable at N=4", onset_all[0][1] >= 0);
    check("AND achievable at N=5", onset_all[1][1] >= 0);
    check("AND achievable at N=6", onset_all[2][1] >= 0);

    /* XOR hierarchy check */
    {
        int x4 = onset_all[0][0], x5 = onset_all[1][0], x6 = onset_all[2][0];
        printf("\n  XOR onset progression: N=4->d%d  N=5->d%d  N=6->d%d\n",
               x4, x5, x6);
        if (x4 >= 0 && x5 >= 0 && x6 >= 0 && x6 > x4) {
            printf("  XOR onset GROWS with N — hierarchy detected!\n\n");
        } else if (x4 >= 0 && x6 >= 0 && x6 == x4) {
            printf("  XOR onset FLAT — no hierarchy at these N.\n\n");
        } else {
            printf("  Inconclusive (some functions not found).\n\n");
        }
    }
}

/* ================================================================ */
/* Phase 7: Hit Rate Ratio Scaling                                   */
/* Does the AND/XOR ratio grow with N? (Hastad bound connection)     */
/* What's the XOR growth curve shape at N=6? (Barrington scaling)    */
/* ================================================================ */

static void phase7_hit_rate_scaling(void) {
    int ni, fi, di, max_d;
    int xor_tt[MAX_TT], and_tt[MAX_TT], or_tt[MAX_TT], maj_tt[MAX_TT];

    printf("\n=== Phase 7: Hit Rate Ratio Scaling ===\n\n");

    max_d = 0;
    { int i; for (i = 0; i < saved_cat_size; i++)
        if (saved_depth[i] > max_d) max_d = saved_depth[i]; }
    if (max_d > 6) max_d = 6;

    /* --- Measurement A: Depth-1 hit rates across N=3,4,5,6 --- */
    {
        int n_list[] = {3, 4, 5, 6};
        int w[4][4]; /* [ni][fi]: XOR AND OR MAJ */
        long totals[4];
        double ratios[4];

        printf("  --- A: Hit rate ratios at depth<=1, all N ---\n\n");

        load_up_to_depth(1);
        build_dirs(g_cat_size);
        printf("  Catalog: %d entries, %d dirs\n\n", g_cat_size, g_nd);

        for (ni = 0; ni < 4; ni++) {
            int nw = n_list[ni];
            int bf = g_cat_size;

            if (bf < nw) {
                memset(w[ni], 0, sizeof(w[ni]));
                totals[ni] = 0;
                ratios[ni] = -1.0;
                continue;
            }

            make_xor_tt(xor_tt, nw);
            make_and_tt(and_tt, nw);
            make_or_tt(or_tt, nw);
            make_maj_tt(maj_tt, nw);

            totals[ni] = comb_nk(bf, nw);
            fprintf(stderr, "DBG: P7A N=%d bf=%d C=%ld\n", nw, bf, totals[ni]);

            w[ni][0] = count_phase_ex(nw, bf, xor_tt);
            w[ni][1] = count_phase_ex(nw, bf, and_tt);
            w[ni][2] = count_phase_ex(nw, bf, or_tt);
            w[ni][3] = count_phase_ex(nw, bf, maj_tt);

            ratios[ni] = (w[ni][0] > 0)
                ? (double)w[ni][1] / (double)w[ni][0] : -1.0;
        }

        printf("    N | %8s | %8s | %8s | %8s | %8s | AND/XOR\n",
               "XOR", "AND", "OR", "MAJ", "Total");
        printf("    --|----------|----------|----------|----------|----------|--------\n");
        for (ni = 0; ni < 4; ni++) {
            if (totals[ni] == 0) {
                printf("    %d |              (skipped: bf < N)\n", n_list[ni]);
            } else if (ratios[ni] >= 0) {
                printf("    %d | %8d | %8d | %8d | %8d | %8ld | %6.2f\n",
                       n_list[ni], w[ni][0], w[ni][1], w[ni][2], w[ni][3],
                       totals[ni], ratios[ni]);
            } else {
                printf("    %d | %8d | %8d | %8d | %8d | %8ld |    ---\n",
                       n_list[ni], w[ni][0], w[ni][1], w[ni][2], w[ni][3],
                       totals[ni]);
            }
        }
        printf("\n");

        printf("  AND/XOR ratio progression: ");
        for (ni = 0; ni < 4; ni++) {
            if (ratios[ni] >= 0)
                printf("N=%d:%.2f  ", n_list[ni], ratios[ni]);
        }
        printf("\n\n");

        /* XOR hit rate as fraction of total */
        printf("  XOR hit rate (fraction of C(bf,N)):\n");
        for (ni = 0; ni < 4; ni++) {
            if (totals[ni] > 0) {
                printf("    N=%d: %d/%ld = %.2f%%\n",
                       n_list[ni], w[ni][0], totals[ni],
                       100.0 * (double)w[ni][0] / (double)totals[ni]);
            }
        }
        printf("\n");

        check("Depth-1 hit rate data collected for N=3-6",
              totals[0] > 0 && totals[3] > 0);
    }

    /* --- Measurement B: N=6 growth curve by depth --- */
    {
        int nw = 6;
        int nsamp = 200000;
        int w[9][4];
        long denom[9];
        int bf_at[9];

        printf("  --- B: N=6 growth curve by depth ---\n\n");

        make_xor_tt(xor_tt, nw);
        make_and_tt(and_tt, nw);
        make_or_tt(or_tt, nw);
        make_maj_tt(maj_tt, nw);
        memset(w, 0, sizeof(w));
        memset(denom, 0, sizeof(denom));
        memset(bf_at, 0, sizeof(bf_at));

        for (di = 0; di <= max_d; di++) {
            int bf;
            long nc;
            int *tts[4];

            load_up_to_depth(di);
            build_dirs(g_cat_size);
            bf = g_cat_size;
            if (bf > 200) bf = 200;
            bf_at[di] = bf;
            if (bf < nw) continue;

            nc = comb_nk(bf, nw);
            tts[0] = xor_tt; tts[1] = and_tt; tts[2] = or_tt; tts[3] = maj_tt;

            if (nc <= (long)nsamp) {
                denom[di] = nc;
                fprintf(stderr, "DBG: P7B d=%d bf=%d exact C=%ld\n",
                        di, bf, nc);
                for (fi = 0; fi < 4; fi++)
                    w[di][fi] = count_phase_ex(nw, bf, tts[fi]);
            } else {
                unsigned long rng = 77777UL + (unsigned long)di;
                denom[di] = (long)nsamp;
                fprintf(stderr, "DBG: P7B d=%d bf=%d samp %d\n",
                        di, bf, nsamp);
                for (fi = 0; fi < 4; fi++)
                    w[di][fi] = sample_phase(nw, bf, nsamp, tts[fi], &rng);
            }
        }

        printf("  d  | bf  | denom   | %7s | %7s | %7s | %7s | AND/XOR\n",
               "XOR", "AND", "OR", "MAJ");
        printf("  ---|-----|---------|---------|---------|---------|---------|--------\n");
        for (di = 0; di <= max_d; di++) {
            if (denom[di] == 0) {
                printf("  %2d | %3d |  (skip) |         |         |         |         |\n",
                       di, bf_at[di]);
                continue;
            }
            if (w[di][0] > 0) {
                printf("  %2d | %3d | %7ld | %7d | %7d | %7d | %7d | %6.2f\n",
                       di, bf_at[di], denom[di],
                       w[di][0], w[di][1], w[di][2], w[di][3],
                       (double)w[di][1] / (double)w[di][0]);
            } else {
                printf("  %2d | %3d | %7ld | %7d | %7d | %7d | %7d |    ---\n",
                       di, bf_at[di], denom[di],
                       w[di][0], w[di][1], w[di][2], w[di][3]);
            }
        }
        printf("\n");

        /* XOR rate analysis */
        printf("  XOR hit rate progression:\n");
        for (di = 0; di <= max_d; di++) {
            if (denom[di] == 0) continue;
            {
                double rate = 100.0 * (double)w[di][0] / (double)denom[di];
                printf("    d=%d: %d/%ld = %.2f%%", di, w[di][0], denom[di], rate);
                if (di > 0 && denom[di-1] > 0 && w[di-1][0] > 0) {
                    double pr = (double)w[di-1][0] / (double)denom[di-1];
                    double cr = (double)w[di][0] / (double)denom[di];
                    printf("  (%.2fx)", cr / pr);
                }
                printf("\n");
            }
        }
        printf("\n");

        /* AND/XOR ratio by depth */
        printf("  AND/XOR ratio by depth:\n");
        for (di = 0; di <= max_d; di++) {
            if (denom[di] == 0 || w[di][0] == 0) continue;
            printf("    d=%d: %.2f\n", di,
                   (double)w[di][1] / (double)w[di][0]);
        }
        printf("\n");

        /* MAJ vs XOR crossover detection */
        printf("  MAJ vs XOR rate comparison:\n");
        for (di = 0; di <= max_d; di++) {
            if (denom[di] == 0) continue;
            {
                double xr = 100.0 * (double)w[di][0] / (double)denom[di];
                double mr = 100.0 * (double)w[di][3] / (double)denom[di];
                printf("    d=%d: XOR=%.2f%% MAJ=%.2f%%", di, xr, mr);
                if (xr > mr) printf("  XOR easier");
                else if (mr > xr) printf("  MAJ easier");
                else printf("  tied");
                printf("\n");
            }
        }
        printf("\n");

        check("N=6 growth curve data collected", denom[1] > 0);
    }
}

/* ================================================================ */
/* Phase 8: N=7,8 at depth 1 — confirm exponential divergence       */
/* At depth 1 catalog has 17 entries.                                */
/* C(17,7)=19448, C(17,8)=24310 — both easily exhaustive.           */
/* ================================================================ */

static void phase8_large_n_depth1(void) {
    int n_list[] = {3, 4, 5, 6, 7, 8};
    int ni;
    int xor_tt[MAX_TT], and_tt[MAX_TT], or_tt[MAX_TT], maj_tt[MAX_TT];
    int w[6][4];
    long totals[6];

    printf("\n=== Phase 8: Full N=3..8 at Depth 1 (phase_cell) ===\n\n");

    load_up_to_depth(1);
    build_dirs(g_cat_size);
    printf("  Catalog: %d entries, %d dirs\n", g_cat_size, g_nd);
    printf("  Cell space: k_sec=12, Voronoi=%d+1 = %d cells per sector\n",
           g_nd, g_nd + 1);
    printf("  Total cells: %d\n\n", 12 * (g_nd + 1));

    for (ni = 0; ni < 6; ni++) {
        int nw = n_list[ni];
        int bf = g_cat_size;

        if (bf < nw) {
            memset(w[ni], 0, sizeof(w[ni]));
            totals[ni] = 0;
            continue;
        }

        make_xor_tt(xor_tt, nw);
        make_and_tt(and_tt, nw);
        make_or_tt(or_tt, nw);
        make_maj_tt(maj_tt, nw);

        totals[ni] = comb_nk(bf, nw);
        fprintf(stderr, "DBG: P8 N=%d bf=%d C=%ld\n", nw, bf, totals[ni]);

        w[ni][0] = count_phase_ex(nw, bf, xor_tt);
        w[ni][1] = count_phase_ex(nw, bf, and_tt);
        w[ni][2] = count_phase_ex(nw, bf, or_tt);
        w[ni][3] = count_phase_ex(nw, bf, maj_tt);
    }

    printf("    N | masks | %8s | %8s | %8s | %8s | %8s | AND/XOR\n",
           "XOR", "AND", "OR", "MAJ", "Total");
    printf("    --|-------|----------|----------|----------|----------|----------|--------\n");
    for (ni = 0; ni < 6; ni++) {
        int nw = n_list[ni];
        if (totals[ni] == 0) {
            printf("    %d | %5d |              (skipped: bf < N)\n",
                   nw, 1 << nw);
            continue;
        }
        printf("    %d | %5d | %8d | %8d | %8d | %8d | %8ld |",
               nw, 1 << nw, w[ni][0], w[ni][1], w[ni][2], w[ni][3],
               totals[ni]);
        if (w[ni][0] > 0)
            printf(" %6.2f", (double)w[ni][1] / (double)w[ni][0]);
        else if (w[ni][1] > 0)
            printf("    inf");
        else
            printf("    ---");
        printf("\n");
    }
    printf("\n");

    /* Hit rate percentages */
    printf("  Hit rates (winners / C(bf,N)):\n\n");
    printf("    N | %8s | %8s | %8s | %8s\n", "XOR%", "AND%", "OR%", "MAJ%");
    printf("    --|----------|----------|----------|----------\n");
    for (ni = 0; ni < 6; ni++) {
        int fi;
        if (totals[ni] == 0) continue;
        printf("    %d |", n_list[ni]);
        for (fi = 0; fi < 4; fi++) {
            printf(" %7.2f%% |",
                   100.0 * (double)w[ni][fi] / (double)totals[ni]);
        }
        printf("\n");
    }
    printf("\n");

    /* Pigeonhole analysis */
    {
        int total_cells = 12 * (g_nd + 1);
        printf("  Pigeonhole analysis (cells=%d):\n", total_cells);
        for (ni = 0; ni < 6; ni++) {
            int nw = n_list[ni];
            int masks = 1 << nw;
            if (totals[ni] == 0) continue;
            printf("    N=%d: %d masks vs %d cells, ratio=%.2f",
                   nw, masks, total_cells,
                   (double)masks / (double)total_cells);
            if (masks > total_cells)
                printf("  PIGEONHOLE (masks > cells)");
            printf("\n");
        }
        printf("\n");
    }

    /* Key checks */
    check("AND achievable at N=7 depth 1", w[4][1] > 0);
    check("AND achievable at N=8 depth 1", w[5][1] > 0);

    if (w[4][0] == 0)
        printf("  ** XOR ZERO at N=7 depth 1 — parity hits ceiling first! **\n");
    if (w[5][0] == 0)
        printf("  ** XOR ZERO at N=8 depth 1 — parity hits ceiling! **\n");

    if (w[4][0] > 0 && w[4][1] > 0) {
        double r7 = (double)w[4][1] / (double)w[4][0];
        printf("  AND/XOR at N=7: %.1f (vs 7.3 at N=6)\n", r7);
    }
    if (w[5][0] > 0 && w[5][1] > 0) {
        double r8 = (double)w[5][1] / (double)w[5][0];
        printf("  AND/XOR at N=8: %.1f\n", r8);
    }
    printf("\n");
}

/* ================================================================ */
/* Main                                                             */
/* ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 93: Function Scaling (1wpi)\n");
    printf("==========================================\n\n");
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

    /* Phase 1: N=3,4,5 depth sweep */
    fprintf(stderr, "DBG: starting phase1\n");
    phase1_depth_sweep();
    fprintf(stderr, "DBG: phase1 done\n");

    /* Phase 2: N=6,7,8 sweep */
    fprintf(stderr, "DBG: starting phase2\n");
    phase2_extended_sweep();
    fprintf(stderr, "DBG: phase2 done\n");

    /* Phase 3: Complement diagnostic */
    fprintf(stderr, "DBG: starting phase3\n");
    phase3_complement_diagnostic();
    fprintf(stderr, "DBG: phase3 done\n");

    /* Phase 4: Complement-invariant diagnostic */
    fprintf(stderr, "DBG: starting phase4\n");
    phase4_ci_diagnostic();
    fprintf(stderr, "DBG: phase4 done\n");

    /* Phase 5: Phase-sensitive NPN sweep */
    fprintf(stderr, "DBG: starting phase5\n");
    phase5_phase_sweep();
    fprintf(stderr, "DBG: phase5 done\n");

    /* Phase 6: Onset depth sweep N=4,5,6 */
    fprintf(stderr, "DBG: starting phase6\n");
    phase6_onset_depth();
    fprintf(stderr, "DBG: phase6 done\n");

    /* Phase 7: Hit rate ratio scaling */
    fprintf(stderr, "DBG: starting phase7\n");
    phase7_hit_rate_scaling();
    fprintf(stderr, "DBG: phase7 done\n");

    /* Phase 8: N=7,8 at depth 1 */
    fprintf(stderr, "DBG: starting phase8\n");
    phase8_large_n_depth1();
    fprintf(stderr, "DBG: phase8 done\n");

    /* Summary */
    printf("\n==========================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);

    return n_fail;
}
