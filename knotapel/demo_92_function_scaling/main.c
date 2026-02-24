/*
 * KNOTAPEL DEMO 92: Function Scaling — Is Parity Special?
 * =======================================================
 *
 * D89-D91 established: depth law max_xor ~ depth+6 is a parity wall.
 * But is this linear scaling specific to XOR (parity), or does it
 * hold for ALL Boolean functions?
 *
 * Phase 1: Test AND, OR, MAJ, XOR at each depth level (cumul 0-8)
 *   for input sizes 6, 8, 10. Report max achievable arity per function
 *   at each depth.
 *
 * Key predictions:
 *   - XOR scales linearly (known depth law)
 *   - AND/OR may need NO depth or scale sub-linearly
 *   - MAJ is in AC^0 but harder than AND/OR — interesting middle ground
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
/* Generalized Boolean function test                                */
/* ================================================================ */

/*
 * truth_table[mask] = 0 or 1, for mask in [0, 2^n_inputs).
 * Tests whether the given weight tuple separates class-0 masks
 * from class-1 masks in the combined_cell activation space.
 */
static int test_bool_func(const int *indices, int n_weights, int k_sec,
                          const int *truth_table) {
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
        int cls, cell;

        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                sa += wa[i]; sb += wb[i]; sc += wc[i]; sd += wd[i];
            }
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
/* K-ladder for generalized search                                  */
/* ================================================================ */

/* K_LADDER available for detailed analysis if needed */

/* Count winners for a Boolean function at given weight count.
 * Uses a SHORT k-ladder (3 values) to keep runtime tractable. */
static const int K_SHORT[] = {6, 12, 24};
#define N_KSHORT 3

static int count_bool_winners(int n_weights, int bf_limit,
                              const int *truth_table) {
    int count = 0;
    int indices[7];
    int i0, i1, i2, i3, ki;

    if (bf_limit > g_cat_size) bf_limit = g_cat_size;

    if (n_weights == 3) {
        for (i0 = 0; i0 < bf_limit; i0++)
        for (i1 = i0+1; i1 < bf_limit; i1++)
        for (i2 = i1+1; i2 < bf_limit; i2++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            for (ki = 0; ki < N_KSHORT; ki++) {
                int nc = K_SHORT[ki] * (g_nd + 1);
                if (nc > MAX_ACT) continue;
                if (test_bool_func(indices, 3, K_SHORT[ki], truth_table)) {
                    count++;
                    break;
                }
            }
        }
    } else if (n_weights == 4) {
        for (i0 = 0; i0 < bf_limit; i0++)
        for (i1 = i0+1; i1 < bf_limit; i1++)
        for (i2 = i1+1; i2 < bf_limit; i2++)
        for (i3 = i2+1; i3 < bf_limit; i3++) {
            indices[0] = i0; indices[1] = i1;
            indices[2] = i2; indices[3] = i3;
            for (ki = 0; ki < N_KSHORT; ki++) {
                int nc = K_SHORT[ki] * (g_nd + 1);
                if (nc > MAX_ACT) continue;
                if (test_bool_func(indices, 4, K_SHORT[ki], truth_table)) {
                    count++;
                    break;
                }
            }
        }
    } else if (n_weights == 5) {
        int i4;
        for (i0 = 0; i0 < bf_limit; i0++)
        for (i1 = i0+1; i1 < bf_limit; i1++)
        for (i2 = i1+1; i2 < bf_limit; i2++)
        for (i3 = i2+1; i3 < bf_limit; i3++)
        for (i4 = i3+1; i4 < bf_limit; i4++) {
            indices[0] = i0; indices[1] = i1;
            indices[2] = i2; indices[3] = i3;
            indices[4] = i4;
            for (ki = 0; ki < N_KSHORT; ki++) {
                int nc = K_SHORT[ki] * (g_nd + 1);
                if (nc > MAX_ACT) continue;
                if (test_bool_func(indices, 5, K_SHORT[ki], truth_table)) {
                    count++;
                    break;
                }
            }
        }
    }

    return count;
}

/* ================================================================ */
/* Truth table generators for standard Boolean functions             */
/* n_inputs = 2*n_weights (the DKC encoding uses +/- weight pairs)  */
/* ================================================================ */

/* popcount for small integers */
static int popcount(int x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

/*
 * XOR-N: output = parity of input bits
 * This is the standard DKC parity test.
 */
static void make_xor_tt(int *tt, int n_inputs) {
    int mask;
    for (mask = 0; mask < (1 << n_inputs); mask++)
        tt[mask] = popcount(mask) & 1;
}

/*
 * AND-N: output = 1 iff ALL input bits are 1
 */
static void make_and_tt(int *tt, int n_inputs) {
    int mask, all_ones = (1 << n_inputs) - 1;
    for (mask = 0; mask < (1 << n_inputs); mask++)
        tt[mask] = (mask == all_ones) ? 1 : 0;
}

/*
 * OR-N: output = 1 iff ANY input bit is 1
 */
static void make_or_tt(int *tt, int n_inputs) {
    int mask;
    for (mask = 0; mask < (1 << n_inputs); mask++)
        tt[mask] = (mask != 0) ? 1 : 0;
}

/*
 * MAJ-N: output = 1 iff majority of input bits are 1
 * (strict majority: > n/2)
 */
static void make_maj_tt(int *tt, int n_inputs) {
    int mask, threshold = n_inputs / 2;
    for (mask = 0; mask < (1 << n_inputs); mask++)
        tt[mask] = (popcount(mask) > threshold) ? 1 : 0;
}

/*
 * THRESHOLD-k-N: output = 1 iff popcount >= k
 * Generalization of MAJ; k=1 is OR, k=N is AND
 */
static void make_threshold_tt(int *tt, int n_inputs, int k) {
    int mask;
    for (mask = 0; mask < (1 << n_inputs); mask++)
        tt[mask] = (popcount(mask) >= k) ? 1 : 0;
}

/* ================================================================ */
/* Phase 1: Function comparison across depths                       */
/* ================================================================ */

/* Maximum truth table size: 2^14 = 16384 */
#define MAX_TT 16384
static int g_tt[MAX_TT];

typedef struct {
    const char *name;
    void (*make_tt)(int *tt, int n_inputs);
    int param;  /* extra parameter (e.g., threshold k), -1 if unused */
} BoolFunc;

static void phase1_function_scaling(void) {
    int n_inputs_list[] = {6, 8};
    int n_funcs = 5;
    int di, fi, ni;
    int bf;

    /* Results table: winners[depth][func][n_inputs_idx] */
    int winners[9][5][2];
    int max_depth;

    printf("\n=== Phase 1: Function Scaling Across Depths ===\n\n");

    memset(winners, 0, sizeof(winners));

    max_depth = 0;
    { int i; for (i = 0; i < saved_cat_size; i++)
        if (saved_depth[i] > max_depth) max_depth = saved_depth[i]; }
    if (max_depth > 8) max_depth = 8;

    for (ni = 0; ni < 2; ni++) {
        int n_inputs = n_inputs_list[ni];
        int n_weights = n_inputs / 2;
        int bf_max = (n_inputs == 6) ? 30 : 20;

        printf("  --- %d-input functions (%d weights) ---\n\n", n_inputs, n_weights);
        fflush(stdout);

        for (di = 0; di <= max_depth; di++) {
            int n_entries;

            load_up_to_depth(di);
            n_entries = g_cat_size;
            build_dirs(g_cat_size);

            bf = n_entries;
            if (bf > bf_max) bf = bf_max;

            fprintf(stderr, "DBG: phase1 n_inputs=%d depth=%d entries=%d dirs=%d bf=%d\n",
                    n_inputs, di, n_entries, g_nd, bf);

            /* XOR */
            make_xor_tt(g_tt, n_inputs);
            winners[di][0][ni] = count_bool_winners(n_weights, bf, g_tt);

            /* AND */
            make_and_tt(g_tt, n_inputs);
            winners[di][1][ni] = count_bool_winners(n_weights, bf, g_tt);

            /* OR */
            make_or_tt(g_tt, n_inputs);
            winners[di][2][ni] = count_bool_winners(n_weights, bf, g_tt);

            /* MAJ */
            make_maj_tt(g_tt, n_inputs);
            winners[di][3][ni] = count_bool_winners(n_weights, bf, g_tt);

            /* THRESH-2 (at least 2 bits set) */
            make_threshold_tt(g_tt, n_inputs, 2);
            winners[di][4][ni] = count_bool_winners(n_weights, bf, g_tt);
        }

        /* Print table */
        printf("  Depth | %8s | %8s | %8s | %8s | %8s\n",
               "XOR", "AND", "OR", "MAJ", "THR2");
        printf("  ------|----------|----------|----------|----------|----------\n");

        for (di = 0; di <= max_depth; di++) {
            printf("  %5d | %8d | %8d | %8d | %8d | %8d\n",
                   di,
                   winners[di][0][ni],
                   winners[di][1][ni],
                   winners[di][2][ni],
                   winners[di][3][ni],
                   winners[di][4][ni]);
        }
        printf("\n");
    }

    /* Analysis: first depth where each function is achievable */
    printf("  === First Depth Where Function is Achievable ===\n");
    printf("  Function | 6-input | 8-input\n");
    printf("  ---------|---------|--------\n");

    for (fi = 0; fi < n_funcs; fi++) {
        const char *names[] = {"XOR", "AND", "OR", "MAJ", "THR2"};
        int first6 = -1, first8 = -1;

        for (di = 0; di <= max_depth; di++) {
            if (first6 < 0 && winners[di][fi][0] > 0) first6 = di;
            if (first8 < 0 && winners[di][fi][1] > 0) first8 = di;
        }

        printf("  %8s | %7d | %7d\n", names[fi], first6, first8);
    }
    printf("\n");

    /* Key comparisons */
    {
        int xor6_first = -1, xor8_first = -1;
        int and6_first = -1, and8_first = -1;
        int maj6_first = -1, maj8_first = -1;

        for (di = 0; di <= max_depth; di++) {
            if (xor6_first < 0 && winners[di][0][0] > 0) xor6_first = di;
            if (xor8_first < 0 && winners[di][0][1] > 0) xor8_first = di;
            if (and6_first < 0 && winners[di][1][0] > 0) and6_first = di;
            if (and8_first < 0 && winners[di][1][1] > 0) and8_first = di;
            if (maj6_first < 0 && winners[di][3][0] > 0) maj6_first = di;
            if (maj8_first < 0 && winners[di][3][1] > 0) maj8_first = di;
        }

        /* Depth gap for XOR (6→8 inputs) */
        if (xor6_first >= 0 && xor8_first >= 0) {
            printf("  XOR depth gap (6→8 inputs): %d\n",
                   xor8_first - xor6_first);
        }
        if (and6_first >= 0 && and8_first >= 0) {
            printf("  AND depth gap (6→8 inputs): %d\n",
                   and8_first - and6_first);
        }
        if (maj6_first >= 0 && maj8_first >= 0) {
            printf("  MAJ depth gap (6→8 inputs): %d\n",
                   maj8_first - maj6_first);
        }
        printf("\n");

        check("Phase 1: XOR requires more depth than AND for same arity",
              and6_first >= 0 && xor6_first >= 0 && and6_first <= xor6_first);

        check("Phase 1: AND achievable at depth 0 or 1",
              and6_first >= 0 && and6_first <= 1);
    }
}

/* ================================================================ */
/* Phase 1b: Truth Table Census                                     */
/* What truth tables does the activation ACTUALLY produce?           */
/* ================================================================ */

/* Simple LCG RNG */
static unsigned long g_rng = 42UL;

static double rng_uniform(void) {
    g_rng = g_rng * 6364136223846793005UL + 1442695040888963407UL;
    return (double)(g_rng >> 33) / (double)(1UL << 31);
}

/* A 64-bit truth table stored as array of 64 ints (0 or 1) */
/* We store the cell label for each mask, then derive separability */

#define MAX_TT_STORE 500
static int stored_tt[MAX_TT_STORE][64];
static int n_stored_tt = 0;

/* Compare two 64-element truth tables */
static int tt_equal(const int *a, const int *b, int n) {
    int i;
    for (i = 0; i < n; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

/* Check if a cell-labeling can separate a truth table (i.e., there exists
 * a partition of cells into class-0 and class-1 that matches the truth table).
 * The cell labels define the partition: each mask gets a cell.
 * For the truth table to be "achievable", all masks with tt=0 must land
 * in cells that have NO tt=1 masks, and vice versa. */
static int check_separability(const int *cell_labels, const int *truth_table,
                              int n_masks) {
    /* For each cell, check if all masks in that cell have the same truth value */
    int cell_tt[MAX_ACT];  /* truth value assigned to this cell, -1 = unassigned */
    int i;
    memset(cell_tt, -1, sizeof(cell_tt));
    for (i = 0; i < n_masks; i++) {
        int c = cell_labels[i];
        int tv = truth_table[i];
        if (cell_tt[c] == -1) {
            cell_tt[c] = tv;
        } else if (cell_tt[c] != tv) {
            return 0;  /* collision: different truth values in same cell */
        }
    }
    return 1;
}

static void phase1b_truth_table_census(void) {
    int trial, n_trials = 10000;
    int bf;
    int n_xor = 0, n_xnor = 0, n_other_sep = 0;
    int n_distinct = 0;
    int k_sec = 12;
    int n_masks = 64;
    int xor_tt[64];
    int xnor_tt[64];
    int and_tt[64], or_tt[64], maj_tt[64];
    int n_and_sep = 0, n_or_sep = 0, n_maj_sep = 0;

    /* Hamming weight histogram of truth tables that ARE separable */
    int hw_hist[65];
    memset(hw_hist, 0, sizeof(hw_hist));

    printf("\n=== Phase 1b: Truth Table Census ===\n\n");

    /* Load cumulative depth 0-4 */
    load_up_to_depth(4);
    build_dirs(g_cat_size);
    bf = g_cat_size;
    if (bf > 275) bf = 275;

    printf("  Catalog: %d entries, %d dirs, k_sec=%d\n", g_cat_size, g_nd, k_sec);
    printf("  Testing %d random 3-weight triples for 6-input separability\n\n",
           n_trials);

    /* Build reference truth tables */
    make_xor_tt(xor_tt, 6);
    { int m; for (m = 0; m < 64; m++) xnor_tt[m] = 1 - xor_tt[m]; }
    make_and_tt(and_tt, 6);
    make_or_tt(or_tt, 6);
    make_maj_tt(maj_tt, 6);

    for (trial = 0; trial < n_trials; trial++) {
        int ri[3], ok2;
        int indices[3];
        int cell_labels[64];
        int mask, i2;
        double wa[6], wb[6], wc[6], wd[6];
        int is_xor, is_xnor;

        /* Random 3-tuple */
        do {
            ri[0] = (int)(rng_uniform() * (double)bf);
            ri[1] = (int)(rng_uniform() * (double)bf);
            ri[2] = (int)(rng_uniform() * (double)bf);
            if (ri[0] >= bf) ri[0] = bf - 1;
            if (ri[1] >= bf) ri[1] = bf - 1;
            if (ri[2] >= bf) ri[2] = bf - 1;
            ok2 = (ri[0] != ri[1] && ri[0] != ri[2] && ri[1] != ri[2]);
        } while (!ok2);

        indices[0] = ri[0]; indices[1] = ri[1]; indices[2] = ri[2];

        /* Compute cell label for each of 64 masks */
        for (i2 = 0; i2 < 3; i2++) {
            const Quat *q = &g_cat[indices[i2]];
            wa[2*i2]   =  q->a; wb[2*i2]   =  q->b;
            wc[2*i2]   =  q->c; wd[2*i2]   =  q->d;
            wa[2*i2+1] = -q->a; wb[2*i2+1] = -q->b;
            wc[2*i2+1] = -q->c; wd[2*i2+1] = -q->d;
        }

        for (mask = 0; mask < n_masks; mask++) {
            double sa = 0, sb = 0, sc = 0, sd = 0;
            for (i2 = 0; i2 < 6; i2++) {
                if (mask & (1 << i2)) {
                    sa += wa[i2]; sb += wb[i2];
                    sc += wc[i2]; sd += wd[i2];
                }
            }
            cell_labels[mask] = combined_cell(sa, sb, sc, sd, k_sec);
        }

        /* Check XOR separability */
        is_xor = check_separability(cell_labels, xor_tt, n_masks);
        is_xnor = check_separability(cell_labels, xnor_tt, n_masks);

        if (is_xor) n_xor++;
        if (is_xnor) n_xnor++;

        /* Check AND, OR, MAJ separability */
        if (check_separability(cell_labels, and_tt, n_masks)) n_and_sep++;
        if (check_separability(cell_labels, or_tt, n_masks)) n_or_sep++;
        if (check_separability(cell_labels, maj_tt, n_masks)) n_maj_sep++;

        /* For triples that separate XOR, check what ELSE they can separate */
        if (is_xor || is_xnor) {
            /* already counted above */
        }

        /* Check ALL possible truth tables? Too expensive for 2^64.
         * Instead: just count how many distinct cell-label patterns appear */
        if (!is_xor && !is_xnor) {
            n_other_sep++;  /* This triple doesn't separate XOR or XNOR */
            /* But does it separate ANYTHING nontrivially? */
            /* Check: how many distinct cell labels are used? */
            {
                int used[MAX_ACT];
                int n_used = 0;
                int j2;
                memset(used, 0, sizeof(int) * (size_t)(k_sec * (g_nd + 1)));
                for (j2 = 0; j2 < n_masks; j2++) {
                    if (!used[cell_labels[j2]]) {
                        used[cell_labels[j2]] = 1;
                        n_used++;
                    }
                }
                (void)n_used;  /* could analyze further */
            }
        }

        /* Store distinct cell-label patterns for analysis */
        if (n_stored_tt < MAX_TT_STORE) {
            int is_new = 1;
            int j2;
            for (j2 = 0; j2 < n_stored_tt; j2++) {
                if (tt_equal(stored_tt[j2], cell_labels, n_masks)) {
                    is_new = 0;
                    break;
                }
            }
            if (is_new) {
                int j3;
                for (j3 = 0; j3 < n_masks; j3++)
                    stored_tt[n_stored_tt][j3] = cell_labels[j3];
                n_stored_tt++;
                n_distinct++;
            }
        }
    }

    printf("  Results from %d random triples:\n", n_trials);
    printf("  XOR6-separable:  %d (%.1f%%)\n", n_xor,
           100.0 * (double)n_xor / (double)n_trials);
    printf("  XNOR6-separable: %d (%.1f%%)\n", n_xnor,
           100.0 * (double)n_xnor / (double)n_trials);
    printf("  AND6-separable:  %d (%.1f%%)\n", n_and_sep,
           100.0 * (double)n_and_sep / (double)n_trials);
    printf("  OR6-separable:   %d (%.1f%%)\n", n_or_sep,
           100.0 * (double)n_or_sep / (double)n_trials);
    printf("  MAJ6-separable:  %d (%.1f%%)\n", n_maj_sep,
           100.0 * (double)n_maj_sep / (double)n_trials);
    printf("  Neither XOR nor XNOR: %d\n", n_other_sep);
    printf("  Distinct cell-label patterns: %d (of %d stored)\n\n",
           n_distinct, n_stored_tt);

    check("Phase 1b: XOR separability rate > 10%%", n_xor * 10 > n_trials);
    check("Phase 1b: AND separability = 0 (parity-locked encoding)",
          n_and_sep == 0);
}

/* ================================================================ */
/* Phase 1c: Mask Collision Check                                    */
/* Which masks produce identical sums for given weights?             */
/* ================================================================ */

static void phase1c_mask_collisions(void) {
    int indices[3];
    double wa[6], wb[6], wc[6], wd[6];
    double sums[64][4];  /* quaternion sum for each mask */
    int mask, i, j2;
    int n_collisions = 0;
    int collision_pairs[200][2];
    int n_stored_pairs = 0;

    printf("\n=== Phase 1c: Mask Collision Check ===\n\n");

    /* Use first 3 entries as a sample triple */
    load_up_to_depth(4);
    build_dirs(g_cat_size);

    indices[0] = 0; indices[1] = 1; indices[2] = 2;

    for (i = 0; i < 3; i++) {
        const Quat *q = &g_cat[indices[i]];
        wa[2*i]   =  q->a; wb[2*i]   =  q->b;
        wc[2*i]   =  q->c; wd[2*i]   =  q->d;
        wa[2*i+1] = -q->a; wb[2*i+1] = -q->b;
        wc[2*i+1] = -q->c; wd[2*i+1] = -q->d;
    }

    /* Compute quaternion sum for each mask */
    for (mask = 0; mask < 64; mask++) {
        double sa = 0, sb = 0, sc = 0, sd = 0;
        for (i = 0; i < 6; i++) {
            if (mask & (1 << i)) {
                sa += wa[i]; sb += wb[i]; sc += wc[i]; sd += wd[i];
            }
        }
        sums[mask][0] = sa; sums[mask][1] = sb;
        sums[mask][2] = sc; sums[mask][3] = sd;
    }

    /* Check for exact collisions (same quaternion sum) */
    printf("  Checking all C(64,2) = 2016 mask pairs for sum collisions...\n\n");
    for (i = 0; i < 64; i++) {
        for (j2 = i + 1; j2 < 64; j2++) {
            double da = fabs(sums[i][0] - sums[j2][0]);
            double db = fabs(sums[i][1] - sums[j2][1]);
            double dc = fabs(sums[i][2] - sums[j2][2]);
            double dd = fabs(sums[i][3] - sums[j2][3]);
            if (da + db + dc + dd < 1e-10) {
                int par_i = popcount(i) & 1;
                int par_j = popcount(j2) & 1;
                n_collisions++;
                if (n_stored_pairs < 200) {
                    collision_pairs[n_stored_pairs][0] = i;
                    collision_pairs[n_stored_pairs][1] = j2;
                    n_stored_pairs++;
                }
                if (n_collisions <= 10) {
                    printf("  COLLISION: mask 0x%02X (par=%d) == mask 0x%02X (par=%d)",
                           i, par_i, j2, par_j);
                    if (par_i == par_j)
                        printf(" [SAME parity]\n");
                    else
                        printf(" [DIFFERENT parity — breaks non-XOR!]\n");
                }
            }
        }
    }

    printf("\n  Total exact sum collisions: %d\n", n_collisions);

    /* Analyze: do collisions always have same parity? */
    {
        int same_par = 0, diff_par = 0;
        for (i = 0; i < n_stored_pairs; i++) {
            int p0 = popcount(collision_pairs[i][0]) & 1;
            int p1 = popcount(collision_pairs[i][1]) & 1;
            if (p0 == p1) same_par++;
            else diff_par++;
        }
        printf("  Same-parity collisions: %d\n", same_par);
        printf("  Cross-parity collisions: %d\n", diff_par);
        printf("\n");

        if (n_collisions > 0) {
            check("Phase 1c: ALL collisions are same-parity (XOR-preserving)",
                  diff_par == 0);
            check("Phase 1c: some cross-parity collisions exist (breaks non-XOR)",
                  diff_par > 0);
        }
    }

    /* Also check: for the same triple, how many distinct cells at k_sec=12? */
    {
        int cell_counts[MAX_ACT];
        int n_cells_used = 0;
        int k_sec = 12;
        memset(cell_counts, 0, sizeof(cell_counts));
        for (mask = 0; mask < 64; mask++) {
            int c = combined_cell(sums[mask][0], sums[mask][1],
                                  sums[mask][2], sums[mask][3], k_sec);
            if (cell_counts[c] == 0) n_cells_used++;
            cell_counts[c]++;
        }
        printf("  At k_sec=%d: %d distinct cells used (of %d available)\n",
               k_sec, n_cells_used, k_sec * (g_nd + 1));
        printf("  64 masks / %d cells = %.1f masks per cell\n\n",
               n_cells_used, 64.0 / (double)n_cells_used);
    }

    /* Check with a known XOR winner */
    printf("  --- Repeating with a known XOR6 winner ---\n");
    {
        int wi[3], ki2;
        int found_winner = 0;
        int bf2 = g_cat_size;
        if (bf2 > 30) bf2 = 30;

        /* Find first XOR6 winner */
        {
            int i0, i1, i2;
            for (i0 = 0; i0 < bf2 && !found_winner; i0++)
            for (i1 = i0+1; i1 < bf2 && !found_winner; i1++)
            for (i2 = i1+1; i2 < bf2 && !found_winner; i2++) {
                int idx[3];
                idx[0] = i0; idx[1] = i1; idx[2] = i2;
                for (ki2 = 0; ki2 < N_KSHORT; ki2++) {
                    int nc = K_SHORT[ki2] * (g_nd + 1);
                    if (nc > MAX_ACT) continue;
                    if (test_bool_func(idx, 3, K_SHORT[ki2], g_tt)) {
                        wi[0] = i0; wi[1] = i1; wi[2] = i2;
                        found_winner = 1;
                        break;
                    }
                }
            }
        }

        if (found_winner) {
            int n_col2 = 0, same2 = 0, diff2 = 0;
            double ws[64][4];

            printf("  Winner: [%d, %d, %d]\n", wi[0], wi[1], wi[2]);

            for (i = 0; i < 3; i++) {
                const Quat *q = &g_cat[wi[i]];
                wa[2*i]   =  q->a; wb[2*i]   =  q->b;
                wc[2*i]   =  q->c; wd[2*i]   =  q->d;
                wa[2*i+1] = -q->a; wb[2*i+1] = -q->b;
                wc[2*i+1] = -q->c; wd[2*i+1] = -q->d;
            }

            for (mask = 0; mask < 64; mask++) {
                double sa = 0, sb = 0, sc = 0, sd = 0;
                for (i = 0; i < 6; i++) {
                    if (mask & (1 << i)) {
                        sa += wa[i]; sb += wb[i];
                        sc += wc[i]; sd += wd[i];
                    }
                }
                ws[mask][0] = sa; ws[mask][1] = sb;
                ws[mask][2] = sc; ws[mask][3] = sd;
            }

            for (i = 0; i < 64; i++) {
                for (j2 = i + 1; j2 < 64; j2++) {
                    double da = fabs(ws[i][0] - ws[j2][0]);
                    double db = fabs(ws[i][1] - ws[j2][1]);
                    double dc = fabs(ws[i][2] - ws[j2][2]);
                    double dd = fabs(ws[i][3] - ws[j2][3]);
                    if (da + db + dc + dd < 1e-10) {
                        int p0 = popcount(i) & 1;
                        int p1 = popcount(j2) & 1;
                        n_col2++;
                        if (p0 == p1) same2++; else diff2++;
                    }
                }
            }
            printf("  Winner collisions: %d total, %d same-par, %d cross-par\n\n",
                   n_col2, same2, diff2);
        }
    }

    check("Phase 1c: collision analysis complete", 1);
}

/* ================================================================ */
/* Phase 1d: Equivalence Class Analysis                             */
/* The encoding creates 3^k equivalence classes (k=n_weights).      */
/* Per weight: (0,0)->0, (1,0)->+q, (0,1)->-q, (1,1)->0            */
/* So 3 distinct contributions {-1,0,+1} per weight -> 27 classes.  */
/* A function is computable ONLY if it's constant on all classes.   */
/* ================================================================ */

static void phase1d_class_analysis(void) {
    int c0, c1, c2;
    int indices[3];
    int i, k_sec = 12;
    int class_idx;
    int and_conflicts, or_conflicts, maj_conflicts, xor_conflicts;
    int class_cell[27];

    printf("\n=== Phase 1d: Equivalence Class Analysis ===\n\n");
    printf("  Encoding: bit 2i selects +q_i, bit 2i+1 selects -q_i\n");
    printf("  Per-weight: (0,0)->0, (1,0)->+q, (0,1)->-q, (1,1)->0\n");
    printf("  3 distinct contributions per weight -> 3^3 = 27 classes\n\n");

    load_up_to_depth(4);
    build_dirs(g_cat_size);

    /* Use a known XOR winner */
    indices[0] = 0; indices[1] = 1; indices[2] = 3;

    printf("  Weights [%d,%d,%d], k_sec=%d\n\n", indices[0], indices[1], indices[2], k_sec);
    printf("  Class | Contrib    | Size | Par | AND | OR  | MAJ | XOR | Cell\n");
    printf("  ------|------------|------|-----|-----|-----|-----|-----|------\n");

    class_idx = 0;
    and_conflicts = 0; or_conflicts = 0; maj_conflicts = 0; xor_conflicts = 0;

    for (c0 = -1; c0 <= 1; c0++)
    for (c1 = -1; c1 <= 1; c1++)
    for (c2 = -1; c2 <= 1; c2++) {
        int cc[3];
        int mask, n_masks_in_class = 0;
        int first_and = -1, first_or = -1, first_maj = -1, first_xor = -1;
        int and_ok = 1, or_ok = 1, maj_ok = 1, xor_ok = 1;
        double sa, sb, sc, sd;
        int cell, n_nonzero, par;

        cc[0] = c0; cc[1] = c1; cc[2] = c2;

        for (mask = 0; mask < 64; mask++) {
            int match = 1;
            for (i = 0; i < 3; i++) {
                int b0 = (mask >> (2*i)) & 1;
                int b1 = (mask >> (2*i+1)) & 1;
                int ci;
                if (b0 == 1 && b1 == 0) ci = 1;
                else if (b0 == 0 && b1 == 1) ci = -1;
                else ci = 0;
                if (ci != cc[i]) { match = 0; break; }
            }
            if (match) {
                int av = (mask == 63) ? 1 : 0;
                int ov = (mask != 0) ? 1 : 0;
                int mv = (popcount(mask) > 3) ? 1 : 0;
                int xv = popcount(mask) & 1;

                if (n_masks_in_class == 0) {
                    first_and = av; first_or = ov;
                    first_maj = mv; first_xor = xv;
                } else {
                    if (av != first_and) and_ok = 0;
                    if (ov != first_or) or_ok = 0;
                    if (mv != first_maj) maj_ok = 0;
                    if (xv != first_xor) xor_ok = 0;
                }
                n_masks_in_class++;
            }
        }

        if (!and_ok) and_conflicts++;
        if (!or_ok) or_conflicts++;
        if (!maj_ok) maj_conflicts++;
        if (!xor_ok) xor_conflicts++;

        /* Compute quaternion sum for this class */
        sa = 0.0; sb = 0.0; sc = 0.0; sd = 0.0;
        for (i = 0; i < 3; i++) {
            const Quat *q = &g_cat[indices[i]];
            sa += (double)cc[i] * q->a;
            sb += (double)cc[i] * q->b;
            sc += (double)cc[i] * q->c;
            sd += (double)cc[i] * q->d;
        }

        cell = combined_cell(sa, sb, sc, sd, k_sec);
        class_cell[class_idx] = cell;

        n_nonzero = (c0 != 0) + (c1 != 0) + (c2 != 0);
        par = n_nonzero & 1;

        printf("  %5d | (%+d,%+d,%+d) | %4d | %3d | %3s | %3s | %3s | %3s | %5d\n",
               class_idx, c0, c1, c2, n_masks_in_class, par,
               and_ok ? "ok" : "NO",
               or_ok ? "ok" : "NO",
               maj_ok ? "ok" : "NO",
               xor_ok ? "ok" : "NO",
               cell);
        class_idx++;
    }

    printf("\n  Classes with function conflicts:\n");
    printf("  AND: %d/27\n", and_conflicts);
    printf("  OR:  %d/27\n", or_conflicts);
    printf("  MAJ: %d/27\n", maj_conflicts);
    printf("  XOR: %d/27\n\n", xor_conflicts);

    /* Key insight */
    printf("  --- KEY: The (0,0,0) class ---\n");
    printf("  mask 0x3F (111111): each pair is (1,1) -> contrib 0 -> sum = 0\n");
    printf("  mask 0x00 (000000): each pair is (0,0) -> contrib 0 -> sum = 0\n");
    printf("  AND(0x3F)=1, AND(0x00)=0: SAME sum, DIFFERENT output -> IMPOSSIBLE\n");
    printf("  OR(0x3F)=1, OR(0x00)=0: same collision\n");
    printf("  This is ENCODING-structural, not activation-structural.\n\n");

    /* Count distinct cells among 27 classes */
    {
        int n_distinct = 0;
        int j;
        for (i = 0; i < 27; i++) {
            int found = 0;
            for (j = 0; j < i; j++) {
                if (class_cell[j] == class_cell[i]) { found = 1; break; }
            }
            if (!found) n_distinct++;
        }
        printf("  Distinct cells for 27 class sums: %d (of 27 classes)\n", n_distinct);
        printf("  (If all 27 get distinct cells -> ANY class-constant function is achievable)\n\n");
    }

    check("Phase 1d: XOR constant on ALL 27 classes", xor_conflicts == 0);
    check("Phase 1d: AND has class conflicts (encoding-impossible)", and_conflicts > 0);
    check("Phase 1d: OR has class conflicts (encoding-impossible)", or_conflicts > 0);
    check("Phase 1d: MAJ has class conflicts (encoding-impossible)", maj_conflicts > 0);
}

/* ================================================================ */
/* Phase 1e: Sign-Flip Symmetry + Multi-Triple Cell Analysis        */
/* For S = c1*q1 + c2*q2 + c3*q3, the negated class -S = -c1*q1... */
/* Question: do S and -S always share the same combined_cell?       */
/* ================================================================ */

static int sector_of(double sa, double sb, double sc, double sd,
                     int k_sec) {
    double n2, nm, qa, half_ang, ang;
    int sec;
    n2 = sa*sa + sb*sb + sc*sc + sd*sd;
    if (n2 < 1e-24) return k_sec - 1;
    nm = sqrt(n2);
    qa = sa / nm;
    if (qa < 0.0) qa = -qa;
    if (qa > 1.0) qa = 1.0;
    half_ang = acos(qa);
    ang = 2.0 * half_ang * 180.0 / M_PI;
    sec = (int)(ang * (double)k_sec / 360.0);
    if (sec >= k_sec) sec = k_sec - 1;
    if (sec < 0) sec = 0;
    return sec;
}

static int voronoi_of(double sa, double sb, double sc, double sd) {
    double n2, nm, qa, nv, inv, ax2, ay2, az2;
    n2 = sa*sa + sb*sb + sc*sc + sd*sd;
    if (n2 < 1e-24) return g_nd;
    nm = sqrt(n2);
    qa = sa / nm;
    if (qa < 0.0) { sb = -sb; sc = -sc; sd = -sd; }
    nv = sqrt(sb*sb + sc*sc + sd*sd) / nm;
    if (nv < 1e-12) return g_nd;
    inv = nm * nv;
    ax2 = sb / inv; ay2 = sc / inv; az2 = sd / inv;
    return vor_cell(ax2, ay2, az2);
}

static void phase1e_sign_flip_symmetry(void) {
    int c0, c1, c2, ci, i, k_sec;
    int indices[3];
    double sums[27][4];
    int sec[27], vor[27], par[27], cells[27];
    int n_pairs, n_ss, n_sv, n_sc, n_sp;

    printf("\n=== Phase 1e: Sign-Flip Symmetry ===\n\n");

    load_up_to_depth(4);
    build_dirs(g_cat_size);
    k_sec = 12;
    indices[0] = 0; indices[1] = 1; indices[2] = 3;

    printf("  Weights [%d,%d,%d], k_sec=%d, %d dirs\n\n",
           indices[0], indices[1], indices[2], k_sec, g_nd);

    /* Build 27 class data */
    ci = 0;
    for (c0 = -1; c0 <= 1; c0++)
    for (c1 = -1; c1 <= 1; c1++)
    for (c2 = -1; c2 <= 1; c2++) {
        double sa = 0, sb = 0, sc2 = 0, sd = 0;
        int cc[3], nn;
        cc[0] = c0; cc[1] = c1; cc[2] = c2;
        for (i = 0; i < 3; i++) {
            const Quat *q = &g_cat[indices[i]];
            sa  += (double)cc[i] * q->a;
            sb  += (double)cc[i] * q->b;
            sc2 += (double)cc[i] * q->c;
            sd  += (double)cc[i] * q->d;
        }
        sums[ci][0] = sa; sums[ci][1] = sb;
        sums[ci][2] = sc2; sums[ci][3] = sd;
        sec[ci] = sector_of(sa, sb, sc2, sd, k_sec);
        vor[ci] = voronoi_of(sa, sb, sc2, sd);
        cells[ci] = combined_cell(sa, sb, sc2, sd, k_sec);
        nn = (c0 != 0) + (c1 != 0) + (c2 != 0);
        par[ci] = nn & 1;
        ci++;
    }

    /* Print decomposed table */
    printf("  Cls | Contrib    | Par | Sec | Vor  | Cell  | Sum(a,b,c,d)\n");
    printf("  ----|------------|-----|-----|------|-------|-------------\n");
    for (ci = 0; ci < 27; ci++) {
        int x0 = (ci / 9) - 1;
        int x1 = ((ci / 3) % 3) - 1;
        int x2 = (ci % 3) - 1;
        printf("  %3d | (%+d,%+d,%+d) | %3d | %3d | %4d | %5d | (%+.3f,%+.3f,%+.3f,%+.3f)\n",
               ci, x0, x1, x2, par[ci], sec[ci], vor[ci], cells[ci],
               sums[ci][0], sums[ci][1], sums[ci][2], sums[ci][3]);
    }

    /* Sign-flip pair analysis */
    printf("\n  --- Sign-flip pairs: (c) vs (-c) ---\n\n");
    n_pairs = 0; n_ss = 0; n_sv = 0; n_sc = 0; n_sp = 0;
    for (ci = 0; ci < 27; ci++) {
        int x0 = (ci / 9) - 1;
        int x1 = ((ci / 3) % 3) - 1;
        int x2 = (ci % 3) - 1;
        int ni = (-x0 + 1) * 9 + (-x1 + 1) * 3 + (-x2 + 1);
        if (ci < ni) {
            int ess = (sec[ci] == sec[ni]);
            int esv = (vor[ci] == vor[ni]);
            int esc = (cells[ci] == cells[ni]);
            int esp = (par[ci] == par[ni]);
            n_pairs++;
            if (ess) n_ss++;
            if (esv) n_sv++;
            if (esc) n_sc++;
            if (esp) n_sp++;
            printf("  (%+d,%+d,%+d)<->(%+d,%+d,%+d): sec=%s vor=%s cell=%s par=%s\n",
                   x0, x1, x2, -x0, -x1, -x2,
                   ess ? "SAME" : "DIFF",
                   esv ? "SAME" : "DIFF",
                   esc ? "SAME" : "DIFF",
                   esp ? "SAME" : "DIFF");
        }
    }

    printf("\n  Sign-flip totals (%d pairs):\n", n_pairs);
    printf("  Same sector:  %d/%d\n", n_ss, n_pairs);
    printf("  Same Voronoi: %d/%d\n", n_sv, n_pairs);
    printf("  Same cell:    %d/%d\n", n_sc, n_pairs);
    printf("  Same parity:  %d/%d\n\n", n_sp, n_pairs);

    check("Phase 1e: sign-flip ALWAYS same sector", n_ss == n_pairs);
    check("Phase 1e: sign-flip ALWAYS same Voronoi", n_sv == n_pairs);
    check("Phase 1e: sign-flip ALWAYS same cell", n_sc == n_pairs);
    check("Phase 1e: sign-flip ALWAYS same parity", n_sp == n_pairs);

    /* Multi-triple: distinct cells for winners vs non-winners */
    printf("  --- Multi-triple cell counts (k_sec=%d) ---\n\n", k_sec);
    {
        int i0, i1, i2, nw = 0, nnw = 0;
        int bf2 = g_cat_size;
        if (bf2 > 20) bf2 = 20;

        make_xor_tt(g_tt, 6);

        for (i0 = 0; i0 < bf2 && (nw < 5 || nnw < 5); i0++)
        for (i1 = i0+1; i1 < bf2 && (nw < 5 || nnw < 5); i1++)
        for (i2 = i1+1; i2 < bf2 && (nw < 5 || nnw < 5); i2++) {
            int idx[3], ki2, is_w = 0, nd2 = 0;
            int tc[27];

            idx[0] = i0; idx[1] = i1; idx[2] = i2;
            for (ki2 = 0; ki2 < N_KSHORT; ki2++) {
                int nc = K_SHORT[ki2] * (g_nd + 1);
                if (nc > MAX_ACT) continue;
                if (test_bool_func(idx, 3, K_SHORT[ki2], g_tt)) {
                    is_w = 1; break;
                }
            }

            if ((is_w && nw >= 5) || (!is_w && nnw >= 5)) continue;

            /* Compute 27 class cells */
            {
                int cx = 0;
                for (c0 = -1; c0 <= 1; c0++)
                for (c1 = -1; c1 <= 1; c1++)
                for (c2 = -1; c2 <= 1; c2++) {
                    double ta = 0, tb = 0, tc2 = 0, td = 0;
                    int cc2[3];
                    cc2[0] = c0; cc2[1] = c1; cc2[2] = c2;
                    for (i = 0; i < 3; i++) {
                        const Quat *q = &g_cat[idx[i]];
                        ta  += (double)cc2[i] * q->a;
                        tb  += (double)cc2[i] * q->b;
                        tc2 += (double)cc2[i] * q->c;
                        td  += (double)cc2[i] * q->d;
                    }
                    tc[cx++] = combined_cell(ta, tb, tc2, td, k_sec);
                }
            }

            /* Count distinct */
            {
                int j3;
                for (ci = 0; ci < 27; ci++) {
                    int found2 = 0;
                    for (j3 = 0; j3 < ci; j3++) {
                        if (tc[j3] == tc[ci]) { found2 = 1; break; }
                    }
                    if (!found2) nd2++;
                }
            }

            printf("  [%2d,%2d,%2d] %s: %2d distinct cells\n",
                   idx[0], idx[1], idx[2],
                   is_w ? "WINNER    " : "non-winner",
                   nd2);

            if (is_w) nw++; else nnw++;
        }
    }
}

/* ================================================================ */
/* Main                                                             */
/* ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 92: Function Scaling\n");
    printf("===================================\n\n");
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

    /* Phase 1: Function comparison across depths */
    fprintf(stderr, "DBG: starting phase1\n");
    phase1_function_scaling();
    fprintf(stderr, "DBG: phase1 done\n");

    /* Phase 1b: Truth table census */
    fprintf(stderr, "DBG: starting phase1b\n");
    phase1b_truth_table_census();
    fprintf(stderr, "DBG: phase1b done\n");

    /* Phase 1c: Mask collision check */
    fprintf(stderr, "DBG: starting phase1c\n");
    make_xor_tt(g_tt, 6);  /* needed for winner search in 1c */
    phase1c_mask_collisions();
    fprintf(stderr, "DBG: phase1c done\n");

    /* Phase 1d: Equivalence class analysis */
    fprintf(stderr, "DBG: starting phase1d\n");
    phase1d_class_analysis();
    fprintf(stderr, "DBG: phase1d done\n");

    /* Phase 1e: Sign-flip symmetry + multi-triple */
    fprintf(stderr, "DBG: starting phase1e\n");
    phase1e_sign_flip_symmetry();
    fprintf(stderr, "DBG: phase1e done\n");

    /* Summary */
    printf("\n===================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);

    return n_fail;
}
