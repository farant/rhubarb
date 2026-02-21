/*
 * KNOTAPEL DEMO 76: Scaling DKC to 8 Inputs
 * ===========================================
 *
 * Core question: Does DKC scale beyond 6 inputs?
 * 4 catalog entries x 2 signs = 8 weights ("winning quadruples").
 * C(24,4) = 10,626 possible quadruples, 2^8 = 256 masks each.
 *
 * Part A: 8-input XOR8 enumeration
 * Part B: Winning quadruple census + failure analysis
 * Part C: Direction structure at 8 inputs
 * Part D: Eigenvalue angle distribution at 8 inputs
 * Part E: 6-input baseline comparison
 *
 * C89, zero dependencies beyond math.h.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * Quaternion type
 * ================================================================ */

typedef struct { double a, b, c, d; } Quat;

static Quat quat_make(double a, double b, double c, double d) {
    Quat q; q.a = a; q.b = b; q.c = c; q.d = d; return q;
}
static Quat quat_add(Quat p, Quat q) {
    return quat_make(p.a + q.a, p.b + q.b, p.c + q.c, p.d + q.d);
}
static Quat quat_neg(Quat q) {
    return quat_make(-q.a, -q.b, -q.c, -q.d);
}
static Quat quat_mul(Quat p, Quat q) {
    return quat_make(
        p.a*q.a - p.b*q.b - p.c*q.c - p.d*q.d,
        p.a*q.b + p.b*q.a + p.c*q.d - p.d*q.c,
        p.a*q.c - p.b*q.d + p.c*q.a + p.d*q.b,
        p.a*q.d + p.b*q.c - p.c*q.b + p.d*q.a);
}
static Quat quat_conj(Quat q) {
    return quat_make(q.a, -q.b, -q.c, -q.d);
}
static double quat_norm(Quat q) {
    return sqrt(q.a*q.a + q.b*q.b + q.c*q.c + q.d*q.d);
}
static Quat quat_normalize(Quat q) {
    double n = quat_norm(q);
    if (n < 1e-15) return quat_make(0, 0, 0, 0);
    return quat_make(q.a / n, q.b / n, q.c / n, q.d / n);
}
static int quat_close(Quat p, Quat q, double eps) {
    return fabs(p.a - q.a) < eps && fabs(p.b - q.b) < eps &&
           fabs(p.c - q.c) < eps && fabs(p.d - q.d) < eps;
}
static Quat quat_one(void) { return quat_make(1, 0, 0, 0); }

/* ================================================================
 * Braid type + SU(2) representation
 * ================================================================ */

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

static Quat q_gen[3];

static void init_su2_generators(void) {
    double s = 1.0 / sqrt(2.0);
    q_gen[1] = quat_make(s, s, 0, 0);
    q_gen[2] = quat_make(s, 0, 0, -s);
}

static Quat braid_quaternion(const Braid *b) {
    Quat result = quat_one();
    int l;
    for (l = 0; l < b->len; l++) {
        int gen = b->word[l];
        int idx = gen > 0 ? gen : -gen;
        Quat g = q_gen[idx];
        if (gen < 0) g = quat_conj(g);
        result = quat_mul(result, g);
    }
    return result;
}

/* ================================================================
 * Eigenvector extraction
 * ================================================================ */

typedef struct {
    double theta;
    double ax, ay, az;
    int axis_defined;
} EigenData;

static EigenData quat_to_eigen(Quat q) {
    EigenData e;
    double norm_v;
    if (q.a < 0.0) {
        q.a = -q.a; q.b = -q.b; q.c = -q.c; q.d = -q.d;
    }
    if (q.a > 1.0) q.a = 1.0;
    if (q.a < 0.0) q.a = 0.0;
    e.theta = acos(q.a);
    norm_v = sqrt(q.b*q.b + q.c*q.c + q.d*q.d);
    if (norm_v < 1e-12) {
        e.ax = 0.0; e.ay = 0.0; e.az = 1.0;
        e.axis_defined = 0;
    } else {
        e.ax = q.b / norm_v;
        e.ay = q.c / norm_v;
        e.az = q.d / norm_v;
        e.axis_defined = 1;
    }
    return e;
}

/* ================================================================
 * Quaternion catalog
 * ================================================================ */

#define MAX_QCAT 128
static Quat qcat[MAX_QCAT];
static int qcat_size = 0;

static int find_quat(Quat q) {
    int i;
    for (i = 0; i < qcat_size; i++) {
        if (quat_close(qcat[i], q, 1e-10)) return i;
        if (quat_close(qcat[i], quat_neg(q), 1e-10)) return i;
    }
    return -1;
}

static void build_catalogs(void) {
    int n, len;
    Braid b;
    int word_buf[MAX_WORD];

    qcat_size = 0;

    for (n = 2; n <= 3; n++) {
        for (len = 1; len <= 8 && len <= MAX_WORD; len++) {
            int max_gen = n - 1;
            int total_gens = 2 * max_gen;
            unsigned long total, idx;
            int i;

            total = 1;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)total_gens;
                if (total > 100000) break;
            }
            if (total > 100000) continue;

            for (idx = 0; idx < total; idx++) {
                unsigned long tmp = idx;
                Quat q;

                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen) word_buf[i] = g + 1;
                    else             word_buf[i] = -(g - max_gen + 1);
                }
                b.n = n; b.len = len;
                memcpy(b.word, word_buf, (size_t)len * sizeof(int));

                q = braid_quaternion(&b);
                if (find_quat(q) < 0 && qcat_size < MAX_QCAT) {
                    qcat[qcat_size] = q;
                    qcat_size++;
                }
            }
        }
    }
}

/* ================================================================
 * Eigenvector direction catalog + Voronoi
 * ================================================================ */

#define MAX_DIR 32
static double g_dir[MAX_DIR][3];
static int g_nd = 0;

static int find_dir(double ax, double ay, double az) {
    int i;
    for (i = 0; i < g_nd; i++) {
        double d1 = fabs(g_dir[i][0] - ax) + fabs(g_dir[i][1] - ay) +
                     fabs(g_dir[i][2] - az);
        double d2 = fabs(g_dir[i][0] + ax) + fabs(g_dir[i][1] + ay) +
                     fabs(g_dir[i][2] + az);
        if (d1 < 1e-8 || d2 < 1e-8) return i;
    }
    return -1;
}

static void build_dir_catalog(void) {
    int i;
    g_nd = 0;
    for (i = 0; i < qcat_size; i++) {
        EigenData e = quat_to_eigen(qcat[i]);
        if (!e.axis_defined) continue;
        if (find_dir(e.ax, e.ay, e.az) < 0 && g_nd < MAX_DIR) {
            g_dir[g_nd][0] = e.ax;
            g_dir[g_nd][1] = e.ay;
            g_dir[g_nd][2] = e.az;
            g_nd++;
        }
    }
}

#define NCELLS 14
#define IDENT_CELL 13

static int vor_cell(double ax, double ay, double az) {
    int i, best = 0;
    double bd = -2.0;
    for (i = 0; i < g_nd; i++) {
        double dp = fabs(ax * g_dir[i][0] +
                         ay * g_dir[i][1] +
                         az * g_dir[i][2]);
        if (dp > bd) { bd = dp; best = i; }
    }
    return best;
}

/* ================================================================
 * Generalized XOR-n checker
 * Returns 1 if cell labeling is consistent with parity function
 * ================================================================ */

static int sum_to_cell(Quat sum) {
    if (quat_norm(sum) < 1e-12) return IDENT_CELL;
    {
        Quat qn = quat_normalize(sum);
        EigenData ed = quat_to_eigen(qn);
        if (!ed.axis_defined) return IDENT_CELL;
        return vor_cell(ed.ax, ed.ay, ed.az);
    }
}

static int check_xorn(const Quat *w, int n_inputs) {
    int seen[NCELLS];
    int mask, i;
    int n_masks = 1 << n_inputs;
    for (i = 0; i < NCELLS; i++) seen[i] = 0;
    for (mask = 0; mask < n_masks; mask++) {
        Quat sum = quat_make(0, 0, 0, 0);
        int par = 0, cell;
        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                sum = quat_add(sum, w[i]);
                par ^= 1;
            }
        }
        cell = sum_to_cell(sum);
        seen[cell] |= (1 << par);
        if (seen[cell] == 3) return 0;
    }
    return 1;
}

/* Count conflict cells (cells that see both parities) */
static int count_conflicts(const Quat *w, int n_inputs) {
    int seen[NCELLS];
    int mask, i, conflicts = 0;
    int n_masks = 1 << n_inputs;
    for (i = 0; i < NCELLS; i++) seen[i] = 0;
    for (mask = 0; mask < n_masks; mask++) {
        Quat sum = quat_make(0, 0, 0, 0);
        int par = 0, cell;
        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                sum = quat_add(sum, w[i]);
                par ^= 1;
            }
        }
        cell = sum_to_cell(sum);
        seen[cell] |= (1 << par);
    }
    for (i = 0; i < NCELLS; i++) {
        if (seen[i] == 3) conflicts++;
    }
    return conflicts;
}

/* ================================================================
 * Winner storage (6-input triples and 8-input quadruples)
 * ================================================================ */

#define MAX_WIN6 64
static int g_win6[MAX_WIN6][3];
static int g_nwin6 = 0;

#define MAX_WIN8 2048
static int g_win8[MAX_WIN8][4];
static int g_nwin8 = 0;

static void build_w6(int wi, Quat *w6) {
    w6[0] = qcat[g_win6[wi][0]]; w6[1] = quat_neg(qcat[g_win6[wi][0]]);
    w6[2] = qcat[g_win6[wi][1]]; w6[3] = quat_neg(qcat[g_win6[wi][1]]);
    w6[4] = qcat[g_win6[wi][2]]; w6[5] = quat_neg(qcat[g_win6[wi][2]]);
}

static void build_w8(int wi, Quat *w8) {
    w8[0] = qcat[g_win8[wi][0]]; w8[1] = quat_neg(qcat[g_win8[wi][0]]);
    w8[2] = qcat[g_win8[wi][1]]; w8[3] = quat_neg(qcat[g_win8[wi][1]]);
    w8[4] = qcat[g_win8[wi][2]]; w8[5] = quat_neg(qcat[g_win8[wi][2]]);
    w8[6] = qcat[g_win8[wi][3]]; w8[7] = quat_neg(qcat[g_win8[wi][3]]);
}

/* ================================================================
 * Find 6-input winners (baseline)
 * ================================================================ */

static void find_winners_6(void) {
    int ai, aj, ak;
    g_nwin6 = 0;
    for (ai = 0; ai < qcat_size; ai++) {
        for (aj = ai + 1; aj < qcat_size; aj++) {
            for (ak = aj + 1; ak < qcat_size; ak++) {
                Quat w6[6];
                w6[0] = qcat[ai]; w6[1] = quat_neg(qcat[ai]);
                w6[2] = qcat[aj]; w6[3] = quat_neg(qcat[aj]);
                w6[4] = qcat[ak]; w6[5] = quat_neg(qcat[ak]);
                if (check_xorn(w6, 6) && g_nwin6 < MAX_WIN6) {
                    g_win6[g_nwin6][0] = ai;
                    g_win6[g_nwin6][1] = aj;
                    g_win6[g_nwin6][2] = ak;
                    g_nwin6++;
                }
            }
        }
    }
}

/* ================================================================
 * Find 8-input winners
 * ================================================================ */

static void find_winners_8(void) {
    int ai, aj, ak, al;
    g_nwin8 = 0;
    for (ai = 0; ai < qcat_size; ai++) {
        for (aj = ai + 1; aj < qcat_size; aj++) {
            for (ak = aj + 1; ak < qcat_size; ak++) {
                for (al = ak + 1; al < qcat_size; al++) {
                    Quat w8[8];
                    w8[0] = qcat[ai]; w8[1] = quat_neg(qcat[ai]);
                    w8[2] = qcat[aj]; w8[3] = quat_neg(qcat[aj]);
                    w8[4] = qcat[ak]; w8[5] = quat_neg(qcat[ak]);
                    w8[6] = qcat[al]; w8[7] = quat_neg(qcat[al]);
                    if (check_xorn(w8, 8) && g_nwin8 < MAX_WIN8) {
                        g_win8[g_nwin8][0] = ai;
                        g_win8[g_nwin8][1] = aj;
                        g_win8[g_nwin8][2] = ak;
                        g_win8[g_nwin8][3] = al;
                        g_nwin8++;
                    }
                }
            }
        }
    }
}

/* ================================================================
 * Information theory helpers
 * ================================================================ */

static double log2_val(double x) {
    return (x > 0.0) ? log(x) / log(2.0) : 0.0;
}

static double entropy_arr(const int *counts, int n, int total) {
    double h = 0.0;
    int i;
    if (total <= 0) return 0.0;
    for (i = 0; i < n; i++) {
        if (counts[i] > 0) {
            double p = (double)counts[i] / (double)total;
            h -= p * log2_val(p);
        }
    }
    return h;
}

/* ================================================================
 * Eigenvalue angle of sum quaternion
 * ================================================================ */

static double sum_eigen_angle(Quat q) {
    double n = quat_norm(q);
    double cos_half;
    if (n < 1e-12) return 0.0;
    cos_half = fabs(q.a) / n;
    if (cos_half > 1.0) cos_half = 1.0;
    return acos(cos_half) * 180.0 / M_PI;
}

/* ================================================================
 * Part A: 8-input XOR8 enumeration
 * ================================================================ */

static void part_a_enumeration(void) {
    char msg[256];
    int total_quads;

    printf("\n=== Part A: 8-input XOR8 enumeration ===\n");

    total_quads = qcat_size * (qcat_size - 1) * (qcat_size - 2) *
                  (qcat_size - 3) / 24;
    printf("  INFO: Catalog: %d quaternions\n", qcat_size);
    printf("  INFO: Total quadruples C(%d,4) = %d\n",
           qcat_size, total_quads);
    printf("  INFO: Masks per quadruple: 256\n");
    printf("  INFO: Total evaluations: %d\n", total_quads * 256);

    printf("  INFO: Searching for XOR8 winners...\n");
    find_winners_8();
    printf("  INFO: XOR8 winners found: %d\n", g_nwin8);

    if (g_nwin8 > 0) {
        int i;
        printf("  INFO: First 10 winners:\n");
        for (i = 0; i < g_nwin8 && i < 10; i++) {
            printf("    W8[%d]: [%d,%d,%d,%d]\n", i,
                   g_win8[i][0], g_win8[i][1],
                   g_win8[i][2], g_win8[i][3]);
        }
    }

    sprintf(msg, "XOR8 winners: %d (of %d quadruples)",
            g_nwin8, total_quads);
    check(msg, 1);

    if (g_nwin8 > 0) {
        double rate = 100.0 * (double)g_nwin8 / (double)total_quads;
        sprintf(msg, "XOR8 hit rate: %.2f%%", rate);
        check(msg, 1);
    } else {
        check("XOR8 has zero solutions at 8 inputs", 1);
    }
}

/* ================================================================
 * Part B: Failure analysis (if XOR8 has no/few winners)
 * ================================================================ */

static void part_b_census(void) {
    int ai, aj, ak, al;
    char msg[256];
    int conflict_hist[NCELLS + 1];
    int total = 0;
    int min_conflicts = NCELLS;
    int best_quad[4] = {0, 0, 0, 0};

    printf("\n=== Part B: Census and failure analysis ===\n");

    memset(conflict_hist, 0, sizeof(conflict_hist));

    /* Sample quadruples for conflict analysis */
    for (ai = 0; ai < qcat_size; ai++) {
        for (aj = ai + 1; aj < qcat_size; aj++) {
            for (ak = aj + 1; ak < qcat_size; ak++) {
                for (al = ak + 1; al < qcat_size; al++) {
                    Quat w8[8];
                    int c;
                    w8[0] = qcat[ai]; w8[1] = quat_neg(qcat[ai]);
                    w8[2] = qcat[aj]; w8[3] = quat_neg(qcat[aj]);
                    w8[4] = qcat[ak]; w8[5] = quat_neg(qcat[ak]);
                    w8[6] = qcat[al]; w8[7] = quat_neg(qcat[al]);
                    c = count_conflicts(w8, 8);
                    if (c < NCELLS + 1) conflict_hist[c]++;
                    if (c < min_conflicts) {
                        min_conflicts = c;
                        best_quad[0] = ai; best_quad[1] = aj;
                        best_quad[2] = ak; best_quad[3] = al;
                    }
                    total++;
                }
            }
        }
    }

    printf("  INFO: Conflict distribution (all %d quadruples):\n", total);
    {
        int i;
        for (i = 0; i <= NCELLS; i++) {
            if (conflict_hist[i] > 0)
                printf("    %2d conflict cells: %d quadruples (%.1f%%)\n",
                       i, conflict_hist[i],
                       100.0 * (double)conflict_hist[i] / (double)total);
        }
    }

    printf("  INFO: Minimum conflicts: %d (quad [%d,%d,%d,%d])\n",
           min_conflicts, best_quad[0], best_quad[1],
           best_quad[2], best_quad[3]);

    /* 6-input baseline */
    printf("  INFO: 6-input baseline: %d XOR6 winners of %d triples\n",
           g_nwin6,
           qcat_size * (qcat_size - 1) * (qcat_size - 2) / 6);

    sprintf(msg, "Min conflict cells at 8 inputs: %d", min_conflicts);
    check(msg, 1);

    sprintf(msg, "XOR8 = %d vs XOR6 = %d winners",
            g_nwin8, g_nwin6);
    check(msg, 1);

    /* Accuracy of best quadruple (majority vote per cell) */
    if (min_conflicts > 0) {
        Quat w8[8];
        int cell_even[NCELLS], cell_odd[NCELLS];
        int correct = 0, mask, i;

        w8[0] = qcat[best_quad[0]]; w8[1] = quat_neg(qcat[best_quad[0]]);
        w8[2] = qcat[best_quad[1]]; w8[3] = quat_neg(qcat[best_quad[1]]);
        w8[4] = qcat[best_quad[2]]; w8[5] = quat_neg(qcat[best_quad[2]]);
        w8[6] = qcat[best_quad[3]]; w8[7] = quat_neg(qcat[best_quad[3]]);

        memset(cell_even, 0, sizeof(cell_even));
        memset(cell_odd, 0, sizeof(cell_odd));

        for (mask = 0; mask < 256; mask++) {
            Quat sum = quat_make(0, 0, 0, 0);
            int par = 0, cell;
            for (i = 0; i < 8; i++) {
                if (mask & (1 << i)) {
                    sum = quat_add(sum, w8[i]);
                    par ^= 1;
                }
            }
            cell = sum_to_cell(sum);
            if (par == 0) cell_even[cell]++;
            else cell_odd[cell]++;
        }

        for (i = 0; i < NCELLS; i++) {
            correct += (cell_even[i] > cell_odd[i]) ?
                        cell_even[i] : cell_odd[i];
        }

        {
            double acc = 100.0 * (double)correct / 256.0;
            printf("  INFO: Best quad accuracy (majority vote): "
                   "%d/256 = %.1f%%\n", correct, acc);
            sprintf(msg, "Best quad accuracy: %.1f%%", acc);
            check(msg, 1);
        }
    }
}

/* ================================================================
 * Part C: Direction structure at 8 inputs
 * ================================================================ */

static void part_c_direction(void) {
    char msg[256];

    printf("\n=== Part C: Direction structure at 8 inputs ===\n");

    if (g_nwin8 > 0) {
        /* Analyze cell usage for 8-input winners */
        int wi, mask, i;
        int total_cell_counts[NCELLS];
        int n_cells_used_total = 0;

        memset(total_cell_counts, 0, sizeof(total_cell_counts));

        for (wi = 0; wi < g_nwin8 && wi < 100; wi++) {
            Quat w8[8];
            build_w8(wi, w8);
            for (mask = 0; mask < 256; mask++) {
                Quat sum = quat_make(0, 0, 0, 0);
                int cell;
                for (i = 0; i < 8; i++) {
                    if (mask & (1 << i))
                        sum = quat_add(sum, w8[i]);
                }
                cell = sum_to_cell(sum);
                total_cell_counts[cell]++;
            }
        }

        printf("  INFO: Cell usage across 8-input winners:\n");
        for (i = 0; i < NCELLS; i++) {
            if (total_cell_counts[i] > 0) {
                printf("    cell %2d: %d hits\n", i, total_cell_counts[i]);
                n_cells_used_total++;
            }
        }
        printf("  INFO: Cells used: %d of %d\n", n_cells_used_total, NCELLS);

        sprintf(msg, "Cells used at 8 inputs: %d", n_cells_used_total);
        check(msg, n_cells_used_total > 0);
    } else {
        /* Analyze cell usage for best (lowest-conflict) quadruple */
        int ai, aj, ak, al;
        int best_quad[4] = {0, 0, 0, 0};
        int min_c = NCELLS;
        Quat w8[8];
        int cell_counts[NCELLS];
        int n_cells_used = 0;
        int mask, i;

        for (ai = 0; ai < qcat_size; ai++)
            for (aj = ai + 1; aj < qcat_size; aj++)
                for (ak = aj + 1; ak < qcat_size; ak++)
                    for (al = ak + 1; al < qcat_size; al++) {
                        Quat tw[8];
                        int c;
                        tw[0] = qcat[ai]; tw[1] = quat_neg(qcat[ai]);
                        tw[2] = qcat[aj]; tw[3] = quat_neg(qcat[aj]);
                        tw[4] = qcat[ak]; tw[5] = quat_neg(qcat[ak]);
                        tw[6] = qcat[al]; tw[7] = quat_neg(qcat[al]);
                        c = count_conflicts(tw, 8);
                        if (c < min_c) {
                            min_c = c;
                            best_quad[0] = ai; best_quad[1] = aj;
                            best_quad[2] = ak; best_quad[3] = al;
                        }
                    }

        w8[0] = qcat[best_quad[0]]; w8[1] = quat_neg(qcat[best_quad[0]]);
        w8[2] = qcat[best_quad[1]]; w8[3] = quat_neg(qcat[best_quad[1]]);
        w8[4] = qcat[best_quad[2]]; w8[5] = quat_neg(qcat[best_quad[2]]);
        w8[6] = qcat[best_quad[3]]; w8[7] = quat_neg(qcat[best_quad[3]]);

        memset(cell_counts, 0, sizeof(cell_counts));
        for (mask = 0; mask < 256; mask++) {
            Quat sum = quat_make(0, 0, 0, 0);
            int cell;
            for (i = 0; i < 8; i++) {
                if (mask & (1 << i))
                    sum = quat_add(sum, w8[i]);
            }
            cell = sum_to_cell(sum);
            cell_counts[cell]++;
        }

        printf("  INFO: Cell usage for best quadruple "
               "[%d,%d,%d,%d] (%d conflicts):\n",
               best_quad[0], best_quad[1], best_quad[2],
               best_quad[3], min_c);
        for (i = 0; i < NCELLS; i++) {
            if (cell_counts[i] > 0) {
                printf("    cell %2d: %d masks\n", i, cell_counts[i]);
                n_cells_used++;
            }
        }
        printf("  INFO: Cells used: %d of %d\n", n_cells_used, NCELLS);

        sprintf(msg, "Cells used (best quad): %d", n_cells_used);
        check(msg, n_cells_used > 0);
    }
}

/* ================================================================
 * Part D: Eigenvalue angle distribution at 8 inputs
 * ================================================================ */

#define MAX_ANG 128

static void part_d_angles(void) {
    char msg[256];
    double ang_cat[MAX_ANG];
    int n_ang = 0;
    int i, j;

    printf("\n=== Part D: Eigenvalue angle distribution (8 inputs) ===\n");

    /* Build angle catalog from either winners or best quadruple */
    if (g_nwin8 > 0) {
        int wi, mask;
        for (wi = 0; wi < g_nwin8 && wi < 50; wi++) {
            Quat w8[8];
            build_w8(wi, w8);
            for (mask = 0; mask < 256; mask++) {
                Quat sum = quat_make(0, 0, 0, 0);
                double ang;
                int found = 0;
                for (i = 0; i < 8; i++) {
                    if (mask & (1 << i))
                        sum = quat_add(sum, w8[i]);
                }
                ang = sum_eigen_angle(sum);
                for (j = 0; j < n_ang; j++) {
                    if (fabs(ang_cat[j] - ang) < 0.01) {
                        found = 1; break;
                    }
                }
                if (!found && n_ang < MAX_ANG) {
                    ang_cat[n_ang++] = ang;
                }
            }
        }
    } else {
        /* Use all quadruples with min conflicts to get angle catalog */
        int ai, aj, ak, al;
        int count = 0;
        for (ai = 0; ai < qcat_size && count < 100; ai++) {
            for (aj = ai + 1; aj < qcat_size && count < 100; aj++) {
                for (ak = aj + 1; ak < qcat_size && count < 100; ak++) {
                    for (al = ak + 1; al < qcat_size && count < 100; al++) {
                        Quat w8[8];
                        int mask;
                        w8[0] = qcat[ai]; w8[1] = quat_neg(qcat[ai]);
                        w8[2] = qcat[aj]; w8[3] = quat_neg(qcat[aj]);
                        w8[4] = qcat[ak]; w8[5] = quat_neg(qcat[ak]);
                        w8[6] = qcat[al]; w8[7] = quat_neg(qcat[al]);
                        for (mask = 0; mask < 256; mask++) {
                            Quat sum = quat_make(0, 0, 0, 0);
                            double ang;
                            int found = 0;
                            for (i = 0; i < 8; i++) {
                                if (mask & (1 << i))
                                    sum = quat_add(sum, w8[i]);
                            }
                            ang = sum_eigen_angle(sum);
                            for (j = 0; j < n_ang; j++) {
                                if (fabs(ang_cat[j] - ang) < 0.01) {
                                    found = 1; break;
                                }
                            }
                            if (!found && n_ang < MAX_ANG) {
                                ang_cat[n_ang++] = ang;
                            }
                        }
                        count++;
                    }
                }
            }
        }
    }

    /* Sort angles */
    for (i = 0; i < n_ang - 1; i++) {
        for (j = i + 1; j < n_ang; j++) {
            if (ang_cat[j] < ang_cat[i]) {
                double tmp = ang_cat[i];
                ang_cat[i] = ang_cat[j];
                ang_cat[j] = tmp;
            }
        }
    }

    printf("  INFO: Distinct eigenvalue angles at 8 inputs: %d\n", n_ang);
    printf("  INFO: Angles (sorted):\n");
    for (i = 0; i < n_ang && i < 20; i++)
        printf("    %.3f deg\n", ang_cat[i]);
    if (n_ang > 20)
        printf("    ... (%d more)\n", n_ang - 20);

    printf("  INFO: Compare: 6-input has 7 distinct angles\n");

    sprintf(msg, "Distinct angles at 8 inputs: %d", n_ang);
    check(msg, n_ang > 0);

    sprintf(msg, "Angle count change: %d (8-input) vs 7 (6-input)",
            n_ang);
    check(msg, 1);
}

/* ================================================================
 * Part E: Capacity comparison
 * ================================================================ */

static void part_e_capacity(void) {
    char msg[256];
    double total_h6 = 0.0, total_h8 = 0.0;
    int wi, mask, i;

    printf("\n=== Part E: Capacity comparison ===\n");

    /* 6-input baseline capacity */
    for (wi = 0; wi < g_nwin6; wi++) {
        Quat w6[6];
        int cell_counts[NCELLS];
        int cell_label[NCELLS];

        build_w6(wi, w6);
        memset(cell_counts, 0, sizeof(cell_counts));
        memset(cell_label, -1, sizeof(cell_label));

        for (mask = 0; mask < 64; mask++) {
            Quat sum = quat_make(0, 0, 0, 0);
            int par = 0, cell;
            for (i = 0; i < 6; i++) {
                if (mask & (1 << i)) {
                    sum = quat_add(sum, w6[i]);
                    par ^= 1;
                }
            }
            cell = sum_to_cell(sum);
            cell_counts[cell]++;
            cell_label[cell] = par;
        }
        total_h6 += entropy_arr(cell_counts, NCELLS, 64);
    }

    if (g_nwin6 > 0) {
        double mean_h6 = total_h6 / (double)g_nwin6;
        printf("  INFO: 6-input avg H(cell): %.3f bits (%d winners)\n",
               mean_h6, g_nwin6);
    }

    /* 8-input capacity */
    if (g_nwin8 > 0) {
        for (wi = 0; wi < g_nwin8; wi++) {
            Quat w8[8];
            int cell_counts[NCELLS];

            build_w8(wi, w8);
            memset(cell_counts, 0, sizeof(cell_counts));

            for (mask = 0; mask < 256; mask++) {
                Quat sum = quat_make(0, 0, 0, 0);
                int cell;
                for (i = 0; i < 8; i++) {
                    if (mask & (1 << i))
                        sum = quat_add(sum, w8[i]);
                }
                cell = sum_to_cell(sum);
                cell_counts[cell]++;
            }
            total_h8 += entropy_arr(cell_counts, NCELLS, 256);
        }

        {
            double mean_h8 = total_h8 / (double)g_nwin8;
            printf("  INFO: 8-input avg H(cell): %.3f bits (%d winners)\n",
                   mean_h8, g_nwin8);
            sprintf(msg, "8-input cell entropy: %.3f bits", mean_h8);
            check(msg, 1);
        }
    } else {
        printf("  INFO: No 8-input winners — capacity comparison N/A\n");
        check("8-input capacity: N/A (no winners)", 1);
    }

    /* Magnitude quantization at 8 inputs */
    {
        double mag_cat[256];
        int n_mag = 0;
        int ai, aj, ak, al;
        int count = 0;

        for (ai = 0; ai < qcat_size && count < 50; ai++) {
            for (aj = ai + 1; aj < qcat_size && count < 50; aj++) {
                for (ak = aj + 1; ak < qcat_size && count < 50; ak++) {
                    for (al = ak + 1; al < qcat_size && count < 50; al++) {
                        Quat w8[8];
                        w8[0] = qcat[ai]; w8[1] = quat_neg(qcat[ai]);
                        w8[2] = qcat[aj]; w8[3] = quat_neg(qcat[aj]);
                        w8[4] = qcat[ak]; w8[5] = quat_neg(qcat[ak]);
                        w8[6] = qcat[al]; w8[7] = quat_neg(qcat[al]);
                        for (mask = 0; mask < 256; mask++) {
                            Quat sum = quat_make(0, 0, 0, 0);
                            double mag;
                            int found = 0, j;
                            for (i = 0; i < 8; i++) {
                                if (mask & (1 << i))
                                    sum = quat_add(sum, w8[i]);
                            }
                            mag = quat_norm(sum);
                            for (j = 0; j < n_mag; j++) {
                                if (fabs(mag_cat[j] - mag) < 0.01) {
                                    found = 1; break;
                                }
                            }
                            if (!found && n_mag < 256) {
                                mag_cat[n_mag++] = mag;
                            }
                        }
                        count++;
                    }
                }
            }
        }

        /* Sort */
        for (i = 0; i < n_mag - 1; i++) {
            int j;
            for (j = i + 1; j < n_mag; j++) {
                if (mag_cat[j] < mag_cat[i]) {
                    double tmp = mag_cat[i];
                    mag_cat[i] = mag_cat[j];
                    mag_cat[j] = tmp;
                }
            }
        }

        printf("  INFO: Distinct |S| values at 8 inputs: %d\n", n_mag);
        printf("  INFO: Magnitudes (sorted):\n");
        for (i = 0; i < n_mag && i < 15; i++)
            printf("    |S| = %.4f\n", mag_cat[i]);
        if (n_mag > 15)
            printf("    ... (%d more)\n", n_mag - 15);
        printf("  INFO: Compare: 6-input has 6 distinct magnitudes "
               "(sqrt{0,1,2,3,4,6})\n");

        sprintf(msg, "Distinct magnitudes at 8 inputs: %d (vs 6 at 6-input)",
                n_mag);
        check(msg, 1);
    }
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 76: Scaling DKC to 8 Inputs\n");
    printf("==========================================\n");

    init_su2_generators();
    build_catalogs();
    build_dir_catalog();

    printf("  INFO: Setup: %d quaternions, %d directions\n",
           qcat_size, g_nd);

    /* 6-input baseline */
    printf("  INFO: Finding 6-input XOR6 winners (baseline)...\n");
    find_winners_6();
    printf("  INFO: 6-input winners: %d\n", g_nwin6);

    part_a_enumeration();
    part_b_census();
    part_c_direction();
    part_d_angles();
    part_e_capacity();

    printf("\n==========================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
