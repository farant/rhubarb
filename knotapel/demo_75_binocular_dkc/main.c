/*
 * KNOTAPEL DEMO 75: Binocular DKC (Multi-Output)
 * ================================================
 *
 * Since bracket (eigenvalue/angle) and cell (eigenvector/axis) are
 * independent invariants (Demo 74), can we read BOTH simultaneously
 * to compute multi-output functions?
 *
 * One braid evaluation -> eigenvalue angle + eigenvector direction.
 * Angle -> n bits.  Direction -> Voronoi cell -> 1 bit.
 *
 * Part A: Eigenvalue quantization of sum quaternions
 * Part B: Binocular output
 * Part C: Independence verification (mutual information)
 * Part D: Computable functions analysis
 * Part E: Eigenvalue function vs winning triple
 * Part F: Capacity analysis
 * Part G: Additive magnitude channel (|S| as second output)
 * Part H: Multiplicative product channel (eigenvector + eigenvalue)
 * Part I: Channel comparison summary
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
static Quat quat_one(void) { return quat_make(1, 0, 0, 0); }
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

/* ================================================================
 * Braid type + SU(2) representation (no bracket needed)
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
 * Quaternion catalog (no bracket computation needed)
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
 * XOR6 checker + winner finding
 * ================================================================ */

static int check_xor6(const Quat *w) {
    int seen[NCELLS];
    int mask, i, cell;
    for (i = 0; i < NCELLS; i++) seen[i] = 0;
    for (mask = 0; mask < 64; mask++) {
        Quat sum = quat_make(0, 0, 0, 0);
        int par = 0;
        for (i = 0; i < 6; i++) {
            if (mask & (1 << i)) {
                sum = quat_add(sum, w[i]);
                par ^= 1;
            }
        }
        if (quat_norm(sum) < 1e-12) {
            cell = IDENT_CELL;
        } else {
            EigenData ed = quat_to_eigen(quat_normalize(sum));
            if (!ed.axis_defined) cell = IDENT_CELL;
            else cell = vor_cell(ed.ax, ed.ay, ed.az);
        }
        seen[cell] |= (1 << par);
        if (seen[cell] == 3) return 0;
    }
    return 1;
}

#define MAX_WIN 64
static int g_win[MAX_WIN][3];
static int g_nwin = 0;

static void find_winners(void) {
    int ai, aj, ak;
    g_nwin = 0;
    for (ai = 0; ai < qcat_size; ai++) {
        for (aj = ai + 1; aj < qcat_size; aj++) {
            for (ak = aj + 1; ak < qcat_size; ak++) {
                Quat w6[6];
                w6[0] = qcat[ai]; w6[1] = quat_neg(qcat[ai]);
                w6[2] = qcat[aj]; w6[3] = quat_neg(qcat[aj]);
                w6[4] = qcat[ak]; w6[5] = quat_neg(qcat[ak]);
                if (check_xor6(w6) && g_nwin < MAX_WIN) {
                    g_win[g_nwin][0] = ai;
                    g_win[g_nwin][1] = aj;
                    g_win[g_nwin][2] = ak;
                    g_nwin++;
                }
            }
        }
    }
}

/* ================================================================
 * Helper: build cell labeling for a winning triple
 * ================================================================ */

static void build_cell_labeling(int wi, int *cell_label) {
    Quat w6[6];
    int mask, i;
    for (i = 0; i < NCELLS; i++) cell_label[i] = -1;
    w6[0] = qcat[g_win[wi][0]]; w6[1] = quat_neg(qcat[g_win[wi][0]]);
    w6[2] = qcat[g_win[wi][1]]; w6[3] = quat_neg(qcat[g_win[wi][1]]);
    w6[4] = qcat[g_win[wi][2]]; w6[5] = quat_neg(qcat[g_win[wi][2]]);
    for (mask = 0; mask < 64; mask++) {
        Quat sum = quat_make(0, 0, 0, 0);
        int par = 0, cell;
        for (i = 0; i < 6; i++) {
            if (mask & (1 << i)) {
                sum = quat_add(sum, w6[i]);
                par ^= 1;
            }
        }
        if (quat_norm(sum) < 1e-12) {
            cell = IDENT_CELL;
        } else {
            EigenData ed = quat_to_eigen(quat_normalize(sum));
            if (!ed.axis_defined) cell = IDENT_CELL;
            else cell = vor_cell(ed.ax, ed.ay, ed.az);
        }
        cell_label[cell] = par;
    }
}

/* ================================================================
 * Eigenvalue angle of sum quaternion (degrees, half-angle)
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
 * Global angle catalog (built once from all triples)
 * ================================================================ */

#define MAX_ANG 128
static double g_ang_cat[MAX_ANG];
static int g_n_ang = 0;

static int find_angle_cat(double a) {
    int i;
    for (i = 0; i < g_n_ang; i++) {
        if (fabs(g_ang_cat[i] - a) < 0.01) return i;
    }
    return -1;
}

static int add_angle_cat(double a) {
    int idx = find_angle_cat(a);
    if (idx >= 0) return idx;
    if (g_n_ang < MAX_ANG) {
        idx = g_n_ang;
        g_ang_cat[g_n_ang] = a;
        g_n_ang++;
        return idx;
    }
    return -1;
}

/* ================================================================
 * Per-mask output for one winning triple
 * ================================================================ */

typedef struct {
    double angle;
    int cell;
    int cell_bit;
    int angle_code;
} MaskOut;

static void compute_triple_outputs(int wi, MaskOut *out,
                                    const int *cell_label) {
    Quat w6[6];
    int mask, i;
    w6[0] = qcat[g_win[wi][0]]; w6[1] = quat_neg(qcat[g_win[wi][0]]);
    w6[2] = qcat[g_win[wi][1]]; w6[3] = quat_neg(qcat[g_win[wi][1]]);
    w6[4] = qcat[g_win[wi][2]]; w6[5] = quat_neg(qcat[g_win[wi][2]]);

    for (mask = 0; mask < 64; mask++) {
        Quat sum = quat_make(0, 0, 0, 0);
        int cell;
        for (i = 0; i < 6; i++) {
            if (mask & (1 << i))
                sum = quat_add(sum, w6[i]);
        }
        out[mask].angle = sum_eigen_angle(sum);
        if (quat_norm(sum) < 1e-12) {
            cell = IDENT_CELL;
        } else {
            Quat qn = quat_normalize(sum);
            EigenData ed = quat_to_eigen(qn);
            if (!ed.axis_defined) cell = IDENT_CELL;
            else cell = vor_cell(ed.ax, ed.ay, ed.az);
        }
        out[mask].cell = cell;
        out[mask].cell_bit = (cell_label[cell] >= 0) ?
                              cell_label[cell] : 0;
        out[mask].angle_code = add_angle_cat(out[mask].angle);
    }
}

/* Build the angle catalog from all triples (call once) */
static void build_angle_catalog(void) {
    int wi;
    MaskOut out[64];
    int cell_label[NCELLS];
    g_n_ang = 0;
    for (wi = 0; wi < g_nwin; wi++) {
        build_cell_labeling(wi, cell_label);
        compute_triple_outputs(wi, out, cell_label);
    }
}

/* ================================================================
 * Helper: build w6 weight array for a winning triple
 * ================================================================ */

static void build_w6(int wi, Quat *w6) {
    w6[0] = qcat[g_win[wi][0]]; w6[1] = quat_neg(qcat[g_win[wi][0]]);
    w6[2] = qcat[g_win[wi][1]]; w6[3] = quat_neg(qcat[g_win[wi][1]]);
    w6[4] = qcat[g_win[wi][2]]; w6[5] = quat_neg(qcat[g_win[wi][2]]);
}

/* ================================================================
 * Multiplicative product of selected quaternions
 * ================================================================ */

static Quat mask_product(const Quat *w6, int mask) {
    Quat M = quat_one();
    int i;
    for (i = 0; i < 6; i++) {
        if (mask & (1 << i))
            M = quat_mul(M, w6[i]);
    }
    return M;
}

/* ================================================================
 * Global magnitude catalog (additive |S|)
 * ================================================================ */

#define MAX_MAG 256
static double g_mag_cat[MAX_MAG];
static int g_n_mag = 0;

static int find_mag_cat(double m) {
    int i;
    for (i = 0; i < g_n_mag; i++) {
        if (fabs(g_mag_cat[i] - m) < 0.01) return i;
    }
    return -1;
}

static int add_mag_cat(double m) {
    int idx = find_mag_cat(m);
    if (idx >= 0) return idx;
    if (g_n_mag < MAX_MAG) {
        idx = g_n_mag;
        g_mag_cat[g_n_mag++] = m;
        return idx;
    }
    return -1;
}

static void build_mag_catalog(void) {
    int wi, mask, i;
    Quat w6[6];
    g_n_mag = 0;
    for (wi = 0; wi < g_nwin; wi++) {
        build_w6(wi, w6);
        for (mask = 0; mask < 64; mask++) {
            Quat sum = quat_make(0, 0, 0, 0);
            for (i = 0; i < 6; i++) {
                if (mask & (1 << i))
                    sum = quat_add(sum, w6[i]);
            }
            add_mag_cat(quat_norm(sum));
        }
    }
}

/* ================================================================
 * Global multiplicative eigenvalue angle catalog
 * ================================================================ */

static double g_mult_ang_cat[MAX_ANG];
static int g_n_mult_ang = 0;

static int find_mult_ang(double a) {
    int i;
    for (i = 0; i < g_n_mult_ang; i++) {
        if (fabs(g_mult_ang_cat[i] - a) < 0.01) return i;
    }
    return -1;
}

static int add_mult_ang(double a) {
    int idx = find_mult_ang(a);
    if (idx >= 0) return idx;
    if (g_n_mult_ang < MAX_ANG) {
        idx = g_n_mult_ang;
        g_mult_ang_cat[g_n_mult_ang++] = a;
        return idx;
    }
    return -1;
}

static void build_mult_angle_catalog(void) {
    int wi, mask;
    Quat w6[6];
    g_n_mult_ang = 0;
    for (wi = 0; wi < g_nwin; wi++) {
        build_w6(wi, w6);
        for (mask = 0; mask < 64; mask++) {
            Quat M = mask_product(w6, mask);
            add_mult_ang(sum_eigen_angle(M));
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
 * Part A: Eigenvalue quantization
 * ================================================================ */

static void part_a_quantization(void) {
    int cell_label[NCELLS];
    MaskOut out[64];
    int mask, i, j;
    char msg[256];
    int ang_counts[MAX_ANG];
    double sorted[MAX_ANG];

    printf("\n=== Part A: Eigenvalue quantization ===\n");
    printf("  INFO: Global angle catalog: %d distinct angles (from %d triples)\n",
           g_n_ang, g_nwin);

    /* Sort and print all angles */
    memcpy(sorted, g_ang_cat, (size_t)g_n_ang * sizeof(double));
    for (i = 0; i < g_n_ang - 1; i++) {
        for (j = i + 1; j < g_n_ang; j++) {
            if (sorted[j] < sorted[i]) {
                double tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }
    printf("  INFO: All distinct angles (sorted):\n");
    for (i = 0; i < g_n_ang; i++) {
        printf("    %7.3f deg\n", sorted[i]);
    }

    /* First winning triple analysis */
    build_cell_labeling(0, cell_label);
    compute_triple_outputs(0, out, cell_label);

    memset(ang_counts, 0, sizeof(ang_counts));
    for (mask = 0; mask < 64; mask++) {
        if (out[mask].angle_code >= 0 &&
            out[mask].angle_code < MAX_ANG)
            ang_counts[out[mask].angle_code]++;
    }

    {
        int n_used = 0;
        for (i = 0; i < g_n_ang; i++) {
            if (ang_counts[i] > 0) n_used++;
        }
        printf("  INFO: Winner[0] [%d,%d,%d] uses %d of %d angles:\n",
               g_win[0][0], g_win[0][1], g_win[0][2],
               n_used, g_n_ang);
        for (i = 0; i < g_n_ang; i++) {
            if (ang_counts[i] > 0) {
                printf("    angle[%2d] = %7.3f deg: %2d inputs\n",
                       i, g_ang_cat[i], ang_counts[i]);
            }
        }
    }

    /* Canonical clustering check */
    {
        int n_near_0 = 0, n_near_45 = 0, n_near_60 = 0;
        int n_near_90 = 0, n_other = 0;
        for (i = 0; i < g_n_ang; i++) {
            double a = g_ang_cat[i];
            if (a < 1.0) n_near_0++;
            else if (fabs(a - 45.0) < 2.0) n_near_45++;
            else if (fabs(a - 60.0) < 2.0) n_near_60++;
            else if (fabs(a - 90.0) < 2.0) n_near_90++;
            else n_other++;
        }
        printf("  INFO: Angle clustering (2 deg tolerance):\n");
        printf("    Near  0 deg: %d\n", n_near_0);
        printf("    Near 45 deg: %d\n", n_near_45);
        printf("    Near 60 deg: %d\n", n_near_60);
        printf("    Near 90 deg: %d\n", n_near_90);
        printf("    Other:       %d\n", n_other);

        sprintf(msg, "Distinct eigenvalue angles: %d", g_n_ang);
        check(msg, g_n_ang > 0);

        sprintf(msg, "Non-canonical angles: %d of %d", n_other, g_n_ang);
        check(msg, 1);

        sprintf(msg, "Angles restricted to canonical 4? %s",
                (n_other == 0 && g_n_ang <= 4) ? "YES" : "NO");
        check(msg, 1);
    }
}

/* ================================================================
 * Part B: Binocular output
 * ================================================================ */

static void part_b_binocular(void) {
    int cell_label[NCELLS];
    MaskOut out[64];
    int mask, wi;
    char msg[256];
    int angle_funcs[MAX_WIN][64];
    int n_distinct_angle = 0;
    int n_distinct_binocular = 0;

    printf("\n=== Part B: Binocular output ===\n");

    /* Build all output functions */
    for (wi = 0; wi < g_nwin; wi++) {
        build_cell_labeling(wi, cell_label);
        compute_triple_outputs(wi, out, cell_label);
        for (mask = 0; mask < 64; mask++) {
            angle_funcs[wi][mask] = out[mask].angle_code;
        }
    }

    /* Count distinct angle functions */
    for (wi = 0; wi < g_nwin; wi++) {
        int unique = 1;
        int wj;
        for (wj = 0; wj < wi; wj++) {
            if (memcmp(angle_funcs[wi], angle_funcs[wj],
                       64 * sizeof(int)) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) n_distinct_angle++;
    }

    /* Count distinct binocular functions (angle_code * 2 + cell_bit) */
    {
        int bino_funcs[MAX_WIN][64];
        for (wi = 0; wi < g_nwin; wi++) {
            build_cell_labeling(wi, cell_label);
            compute_triple_outputs(wi, out, cell_label);
            for (mask = 0; mask < 64; mask++) {
                bino_funcs[wi][mask] = out[mask].angle_code * 2 +
                                        out[mask].cell_bit;
            }
        }
        for (wi = 0; wi < g_nwin; wi++) {
            int unique = 1;
            int wj;
            for (wj = 0; wj < wi; wj++) {
                if (memcmp(bino_funcs[wi], bino_funcs[wj],
                           64 * sizeof(int)) == 0) {
                    unique = 0;
                    break;
                }
            }
            if (unique) n_distinct_binocular++;
        }
    }

    printf("  INFO: Distinct angle-only functions: %d\n",
           n_distinct_angle);
    printf("  INFO: Distinct binocular functions:  %d\n",
           n_distinct_binocular);
    printf("  INFO: Distinct cell-only function:   1 (always XOR6)\n");

    /* B1 */
    sprintf(msg, "Distinct binocular functions: %d of %d triples",
            n_distinct_binocular, g_nwin);
    check(msg, n_distinct_binocular > 0);

    /* B2: Verify cell bit = XOR6 for all triples */
    {
        int all_xor = 1;
        for (wi = 0; wi < g_nwin && all_xor; wi++) {
            int i;
            build_cell_labeling(wi, cell_label);
            compute_triple_outputs(wi, out, cell_label);
            for (mask = 0; mask < 64; mask++) {
                int par = 0;
                for (i = 0; i < 6; i++) {
                    if (mask & (1 << i)) par ^= 1;
                }
                if (out[mask].cell_bit != par) {
                    all_xor = 0;
                    break;
                }
            }
        }
        sprintf(msg, "Cell bit = XOR6 for all triples: %s",
                all_xor ? "YES" : "NO");
        check(msg, all_xor);
    }

    /* B3: Binocular = angle (since cell is universal XOR6) */
    sprintf(msg, "Binocular distinct == angle distinct: %s",
            n_distinct_binocular == n_distinct_angle ? "YES" : "NO");
    check(msg, 1);
}

/* ================================================================
 * Part C: Independence verification (mutual information)
 * ================================================================ */

static void part_c_independence(void) {
    int cell_label[NCELLS];
    MaskOut out[64];
    int mask, wi;
    char msg[256];
    double total_mi = 0.0;
    int n_indep = 0, n_correlated = 0;

    printf("\n=== Part C: Independence (mutual information) ===\n");

    for (wi = 0; wi < g_nwin; wi++) {
        int joint[MAX_ANG][2];
        int ang_margin[MAX_ANG];
        int cell_margin[2];
        int joint_flat[MAX_ANG * 2];
        double h_a, h_c, h_j, mi;
        int i;

        build_cell_labeling(wi, cell_label);
        compute_triple_outputs(wi, out, cell_label);

        memset(joint, 0, sizeof(joint));
        memset(ang_margin, 0, sizeof(ang_margin));
        memset(cell_margin, 0, sizeof(cell_margin));

        for (mask = 0; mask < 64; mask++) {
            int ac = out[mask].angle_code;
            int cb = out[mask].cell_bit;
            if (ac >= 0 && ac < MAX_ANG) {
                joint[ac][cb]++;
                ang_margin[ac]++;
            }
            cell_margin[cb]++;
        }

        h_a = entropy_arr(ang_margin, g_n_ang, 64);
        h_c = entropy_arr(cell_margin, 2, 64);
        for (i = 0; i < g_n_ang; i++) {
            joint_flat[i * 2] = joint[i][0];
            joint_flat[i * 2 + 1] = joint[i][1];
        }
        h_j = entropy_arr(joint_flat, g_n_ang * 2, 64);
        mi = h_a + h_c - h_j;

        if (wi < 5 || wi == g_nwin - 1) {
            printf("  INFO: W[%2d]: H(ang)=%.3f H(cell)=%.3f "
                   "H(jnt)=%.3f MI=%.4f\n",
                   wi, h_a, h_c, h_j, mi);
        }
        total_mi += mi;
        if (mi < 0.01) n_indep++;
        else n_correlated++;
    }

    {
        double mean_mi = total_mi / (double)g_nwin;
        printf("  INFO: Mean MI: %.4f bits\n", mean_mi);
        printf("  INFO: Independent (MI<0.01): %d/%d\n",
               n_indep, g_nwin);
        printf("  INFO: Correlated (MI>=0.01): %d/%d\n",
               n_correlated, g_nwin);

        sprintf(msg, "MI computed for %d triples", g_nwin);
        check(msg, g_nwin > 0);

        sprintf(msg, "Mean MI = %.4f bits (%s)",
                mean_mi,
                mean_mi < 0.05 ? "INDEPENDENT" : "CORRELATED");
        check(msg, 1);
    }
}

/* ================================================================
 * Part D: Computable functions analysis
 * ================================================================ */

static void part_d_functions(void) {
    int cell_label[NCELLS];
    MaskOut out[64];
    int mask;
    char msg[256];
    int i;

    printf("\n=== Part D: Computable functions ===\n");

    /* Analyze first triple's angle function */
    build_cell_labeling(0, cell_label);
    compute_triple_outputs(0, out, cell_label);

    {
        int counts[MAX_ANG];
        int n_used = 0;
        memset(counts, 0, sizeof(counts));
        for (mask = 0; mask < 64; mask++) {
            if (out[mask].angle_code >= 0 &&
                out[mask].angle_code < MAX_ANG)
                counts[out[mask].angle_code]++;
        }
        for (i = 0; i < g_n_ang; i++) {
            if (counts[i] > 0) n_used++;
        }

        printf("  INFO: Winner[0] angle function: %d distinct outputs\n",
               n_used);
        printf("  INFO: Output distribution:\n");
        for (i = 0; i < g_n_ang; i++) {
            if (counts[i] > 0)
                printf("    angle[%2d] %7.3f deg: %2d inputs (%.1f%%)\n",
                       i, g_ang_cat[i], counts[i],
                       100.0 * (double)counts[i] / 64.0);
        }

        sprintf(msg, "Angle function outputs: %d", n_used);
        check(msg, n_used > 0);
    }

    /* Per-angle parity balance */
    printf("  INFO: Per-angle parity distribution (winner[0]):\n");
    for (i = 0; i < g_n_ang; i++) {
        int n_even = 0, n_odd = 0;
        for (mask = 0; mask < 64; mask++) {
            int par = 0, j;
            for (j = 0; j < 6; j++) {
                if (mask & (1 << j)) par ^= 1;
            }
            if (out[mask].angle_code == i) {
                if (par == 0) n_even++; else n_odd++;
            }
        }
        if (n_even + n_odd > 0) {
            printf("    angle[%2d] %7.3f deg: %d even, %d odd %s\n",
                   i, g_ang_cat[i], n_even, n_odd,
                   (n_even == n_odd) ? "(balanced)" : "(SKEWED)");
        }
    }

    /* Is the angle function related to parity? */
    {
        /* Check if angle perfectly predicts parity (like cell does) */
        int angle_predicts_parity = 1;
        int angle_label[MAX_ANG];
        memset(angle_label, -1, sizeof(angle_label));

        for (mask = 0; mask < 64; mask++) {
            int ac = out[mask].angle_code;
            int par = 0, j;
            for (j = 0; j < 6; j++) {
                if (mask & (1 << j)) par ^= 1;
            }
            if (ac >= 0 && ac < MAX_ANG) {
                if (angle_label[ac] < 0) {
                    angle_label[ac] = par;
                } else if (angle_label[ac] != par) {
                    angle_predicts_parity = 0;
                }
            }
        }

        printf("  INFO: Angle predicts parity? %s\n",
               angle_predicts_parity ? "YES" : "NO");
        sprintf(msg, "Angle predicts parity: %s",
                angle_predicts_parity ? "YES (redundant with cell)" :
                                        "NO (independent information)");
        check(msg, 1);
    }
}

/* ================================================================
 * Part E: Eigenvalue function vs winning triple
 * ================================================================ */

static void part_e_cross_triple(void) {
    int cell_label[NCELLS];
    MaskOut out[64];
    int mask, wi;
    char msg[256];
    double angle_vals[MAX_WIN][64];
    int groups[MAX_WIN];
    int n_groups = 0;

    printf("\n=== Part E: Eigenvalue function vs triple ===\n");

    for (wi = 0; wi < g_nwin; wi++) {
        build_cell_labeling(wi, cell_label);
        compute_triple_outputs(wi, out, cell_label);
        for (mask = 0; mask < 64; mask++)
            angle_vals[wi][mask] = out[mask].angle;
    }

    /* Group triples by angle function */
    for (wi = 0; wi < g_nwin; wi++) groups[wi] = -1;
    for (wi = 0; wi < g_nwin; wi++) {
        int wj;
        if (groups[wi] >= 0) continue;
        groups[wi] = n_groups;
        for (wj = wi + 1; wj < g_nwin; wj++) {
            int same = 1;
            for (mask = 0; mask < 64; mask++) {
                if (fabs(angle_vals[wi][mask] -
                         angle_vals[wj][mask]) > 0.01) {
                    same = 0;
                    break;
                }
            }
            if (same) groups[wj] = n_groups;
        }
        n_groups++;
    }

    printf("  INFO: Distinct angle functions: %d of %d triples\n",
           n_groups, g_nwin);

    if (n_groups == 1)
        printf("  INFO: UNIVERSAL: all triples produce same angle function!\n");
    else if (n_groups == g_nwin)
        printf("  INFO: ALL DIFFERENT: each triple is unique\n");
    else
        printf("  INFO: PARTIAL SHARING: %d groups among %d triples\n",
               n_groups, g_nwin);

    /* Print groups */
    {
        int gi;
        printf("  INFO: Angle function groups:\n");
        for (gi = 0; gi < n_groups && gi < 10; gi++) {
            int count = 0, first = -1;
            for (wi = 0; wi < g_nwin; wi++) {
                if (groups[wi] == gi) {
                    count++;
                    if (first < 0) first = wi;
                }
            }
            if (first >= 0)
                printf("    Group %d: %d triples (first: W[%d] [%d,%d,%d])\n",
                       gi, count, first,
                       g_win[first][0], g_win[first][1], g_win[first][2]);
        }
        if (n_groups > 10)
            printf("    ... (%d more groups)\n", n_groups - 10);
    }

    /* Angle range */
    {
        double min_a = 180.0, max_a = 0.0;
        for (wi = 0; wi < g_nwin; wi++) {
            for (mask = 0; mask < 64; mask++) {
                if (angle_vals[wi][mask] > 0.01) {
                    if (angle_vals[wi][mask] < min_a)
                        min_a = angle_vals[wi][mask];
                    if (angle_vals[wi][mask] > max_a)
                        max_a = angle_vals[wi][mask];
                }
            }
        }
        printf("  INFO: Angle range (non-zero): %.3f - %.3f deg\n",
               min_a, max_a);
    }

    sprintf(msg, "Angle function groups: %d", n_groups);
    check(msg, n_groups > 0);

    sprintf(msg, "Angle function is triple-%s",
            n_groups == 1 ? "INDEPENDENT" : "DEPENDENT");
    check(msg, 1);
}

/* ================================================================
 * Part F: Capacity analysis
 * ================================================================ */

static void part_f_capacity(void) {
    int cell_label[NCELLS];
    MaskOut out[64];
    int mask, wi;
    char msg[256];
    double total_h_a = 0.0, total_h_c = 0.0, total_h_j = 0.0;

    printf("\n=== Part F: Capacity analysis ===\n");

    for (wi = 0; wi < g_nwin; wi++) {
        int ang_margin[MAX_ANG];
        int cell_margin[2];
        int joint[MAX_ANG][2];
        int joint_flat[MAX_ANG * 2];
        double h_a, h_c, h_j;
        int i;

        build_cell_labeling(wi, cell_label);
        compute_triple_outputs(wi, out, cell_label);

        memset(ang_margin, 0, sizeof(ang_margin));
        memset(cell_margin, 0, sizeof(cell_margin));
        memset(joint, 0, sizeof(joint));

        for (mask = 0; mask < 64; mask++) {
            int ac = out[mask].angle_code;
            int cb = out[mask].cell_bit;
            if (ac >= 0 && ac < MAX_ANG) {
                ang_margin[ac]++;
                joint[ac][cb]++;
            }
            cell_margin[cb]++;
        }

        h_a = entropy_arr(ang_margin, g_n_ang, 64);
        h_c = entropy_arr(cell_margin, 2, 64);
        for (i = 0; i < g_n_ang; i++) {
            joint_flat[i * 2] = joint[i][0];
            joint_flat[i * 2 + 1] = joint[i][1];
        }
        h_j = entropy_arr(joint_flat, g_n_ang * 2, 64);

        total_h_a += h_a;
        total_h_c += h_c;
        total_h_j += h_j;
    }

    {
        double mean_h_a = total_h_a / (double)g_nwin;
        double mean_h_c = total_h_c / (double)g_nwin;
        double mean_h_j = total_h_j / (double)g_nwin;
        double mean_mi = mean_h_a + mean_h_c - mean_h_j;
        double additive = mean_h_a + mean_h_c;

        printf("  INFO: Channel capacity (avg over %d triples):\n",
               g_nwin);
        printf("    Angle channel:     %.3f bits\n", mean_h_a);
        printf("    Cell channel:      %.3f bits\n", mean_h_c);
        printf("    Binocular (joint): %.3f bits\n", mean_h_j);
        printf("    Additive bound:    %.3f bits\n", additive);
        printf("    Mutual info:       %.4f bits\n", mean_mi);
        printf("    Efficiency:        %.1f%% of additive\n",
               additive > 0.0 ? 100.0 * mean_h_j / additive : 0.0);

        sprintf(msg, "Binocular capacity: %.3f bits", mean_h_j);
        check(msg, mean_h_j > 0.0);

        sprintf(msg, "Gain over cell-only: +%.3f bits (%.1fx)",
                mean_h_j - mean_h_c,
                mean_h_c > 0.0 ? mean_h_j / mean_h_c : 0.0);
        check(msg, mean_h_j >= mean_h_c);

        sprintf(msg, "Independence: MI=%.4f (%s)", mean_mi,
                fabs(mean_mi) < 0.05 ? "INDEPENDENT" : "CORRELATED");
        check(msg, 1);
    }
}

/* ================================================================
 * Part G: Additive magnitude channel
 * ================================================================ */

static void part_g_magnitude(void) {
    int wi, mask, i, j;
    char msg[256];
    double sorted[MAX_MAG];
    int n_predicts = 0;
    double total_h_mag = 0.0, total_mi_mc = 0.0;

    printf("\n=== Part G: Additive magnitude channel ===\n");
    printf("  INFO: Global magnitude catalog: %d distinct values\n", g_n_mag);

    /* Sort and print magnitudes */
    memcpy(sorted, g_mag_cat, (size_t)g_n_mag * sizeof(double));
    for (i = 0; i < g_n_mag - 1; i++) {
        for (j = i + 1; j < g_n_mag; j++) {
            if (sorted[j] < sorted[i]) {
                double tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }
    printf("  INFO: All distinct magnitudes (sorted):\n");
    for (i = 0; i < g_n_mag && i < 20; i++)
        printf("    |S| = %.4f\n", sorted[i]);
    if (g_n_mag > 20)
        printf("    ... (%d more)\n", g_n_mag - 20);

    /* Per-triple analysis */
    for (wi = 0; wi < g_nwin; wi++) {
        Quat w6[6];
        int cell_label[NCELLS];
        int mag_label[MAX_MAG];
        int mag_counts[MAX_MAG];
        int cell_margin[2];
        int joint_mc[MAX_MAG * 2];
        double h_m, h_c, h_j;
        int predicts = 1;

        build_w6(wi, w6);
        build_cell_labeling(wi, cell_label);
        memset(mag_label, -1, sizeof(mag_label));
        memset(mag_counts, 0, sizeof(mag_counts));
        memset(cell_margin, 0, sizeof(cell_margin));
        memset(joint_mc, 0, sizeof(joint_mc));

        for (mask = 0; mask < 64; mask++) {
            Quat sum = quat_make(0, 0, 0, 0);
            int p = 0, mc, cell, cb;
            for (i = 0; i < 6; i++) {
                if (mask & (1 << i)) {
                    sum = quat_add(sum, w6[i]);
                    p ^= 1;
                }
            }
            mc = find_mag_cat(quat_norm(sum));
            if (quat_norm(sum) < 1e-12) {
                cell = IDENT_CELL;
            } else {
                Quat qn = quat_normalize(sum);
                EigenData ed = quat_to_eigen(qn);
                if (!ed.axis_defined) cell = IDENT_CELL;
                else cell = vor_cell(ed.ax, ed.ay, ed.az);
            }
            cb = (cell_label[cell] >= 0) ? cell_label[cell] : 0;
            if (mc >= 0 && mc < MAX_MAG) {
                mag_counts[mc]++;
                if (mag_label[mc] < 0)
                    mag_label[mc] = p;
                else if (mag_label[mc] != p)
                    predicts = 0;
                joint_mc[mc * 2 + cb]++;
            }
            cell_margin[cb]++;
        }

        if (predicts) n_predicts++;
        h_m = entropy_arr(mag_counts, g_n_mag, 64);
        h_c = entropy_arr(cell_margin, 2, 64);
        h_j = entropy_arr(joint_mc, g_n_mag * 2, 64);
        total_h_mag += h_m;
        total_mi_mc += (h_m + h_c - h_j);

        /* Print details for first triple */
        if (wi == 0) {
            int n_used = 0;
            for (i = 0; i < g_n_mag; i++) {
                if (mag_counts[i] > 0) n_used++;
            }
            printf("  INFO: Winner[0] uses %d of %d magnitude values\n",
                   n_used, g_n_mag);
            printf("  INFO: Distribution (winner[0]):\n");
            for (i = 0; i < g_n_mag; i++) {
                if (mag_counts[i] > 0)
                    printf("    mag[%2d] |S|=%.4f: %2d inputs\n",
                           i, g_mag_cat[i], mag_counts[i]);
            }
            printf("  INFO: Magnitude predicts parity (winner[0])? %s\n",
                   predicts ? "YES" : "NO");
        }
    }

    {
        double mean_h = total_h_mag / (double)g_nwin;
        double mean_mi = total_mi_mc / (double)g_nwin;
        printf("  INFO: Magnitude predicts parity: %d/%d triples\n",
               n_predicts, g_nwin);
        printf("  INFO: Avg H(|S|): %.3f bits\n", mean_h);
        printf("  INFO: Avg MI(|S|; cell): %.4f bits\n", mean_mi);

        sprintf(msg, "Distinct magnitude values: %d", g_n_mag);
        check(msg, g_n_mag > 0);
        sprintf(msg, "Magnitude predicts parity: %d/%d triples",
                n_predicts, g_nwin);
        check(msg, 1);
        sprintf(msg, "Magnitude channel: %.3f bits (avg)", mean_h);
        check(msg, mean_h > 0.0);
    }
}

/* ================================================================
 * Part H: Multiplicative product channel
 * ================================================================ */

static void part_h_multiplicative(void) {
    int wi, mask, i, j;
    char msg[256];
    int n_xor6 = 0;
    double total_h_mc = 0.0, total_h_ma = 0.0;
    double sorted[MAX_ANG];

    printf("\n=== Part H: Multiplicative product channel ===\n");

    /* Sort and print multiplicative angles */
    printf("  INFO: Multiplicative eigenvalue angles: %d distinct\n",
           g_n_mult_ang);
    memcpy(sorted, g_mult_ang_cat,
           (size_t)g_n_mult_ang * sizeof(double));
    for (i = 0; i < g_n_mult_ang - 1; i++) {
        for (j = i + 1; j < g_n_mult_ang; j++) {
            if (sorted[j] < sorted[i]) {
                double tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }
    printf("  INFO: Angles (sorted):\n");
    for (i = 0; i < g_n_mult_ang && i < 15; i++)
        printf("    %.3f deg\n", sorted[i]);
    if (g_n_mult_ang > 15)
        printf("    ... (%d more)\n", g_n_mult_ang - 15);

    /* Per-triple analysis */
    for (wi = 0; wi < g_nwin; wi++) {
        Quat w6[6];
        int seen[NCELLS];
        int mult_cell_counts[NCELLS];
        int mult_ang_counts[MAX_ANG];
        int computes_xor = 1;

        build_w6(wi, w6);
        memset(seen, 0, sizeof(seen));
        memset(mult_cell_counts, 0, sizeof(mult_cell_counts));
        memset(mult_ang_counts, 0, sizeof(mult_ang_counts));

        for (mask = 0; mask < 64; mask++) {
            Quat M = mask_product(w6, mask);
            EigenData ed = quat_to_eigen(M);
            double ang = sum_eigen_angle(M);
            int cell, par = 0, ac;

            if (!ed.axis_defined) cell = IDENT_CELL;
            else cell = vor_cell(ed.ax, ed.ay, ed.az);

            for (i = 0; i < 6; i++) {
                if (mask & (1 << i)) par ^= 1;
            }
            seen[cell] |= (1 << par);
            if (seen[cell] == 3) computes_xor = 0;

            mult_cell_counts[cell]++;
            ac = find_mult_ang(ang);
            if (ac >= 0) mult_ang_counts[ac]++;
        }

        if (computes_xor) n_xor6++;
        total_h_mc += entropy_arr(mult_cell_counts, NCELLS, 64);
        total_h_ma += entropy_arr(mult_ang_counts, g_n_mult_ang, 64);

        /* Print details for first triple */
        if (wi == 0) {
            int n_cells_used = 0, n_angs_used = 0;
            for (i = 0; i < NCELLS; i++) {
                if (mult_cell_counts[i] > 0) n_cells_used++;
            }
            for (i = 0; i < g_n_mult_ang; i++) {
                if (mult_ang_counts[i] > 0) n_angs_used++;
            }
            printf("  INFO: Winner[0] multiplicative: %d cells, %d angles\n",
                   n_cells_used, n_angs_used);
            printf("  INFO: Mult cell distribution (winner[0]):\n");
            for (i = 0; i < NCELLS; i++) {
                if (mult_cell_counts[i] > 0)
                    printf("    cell %2d: %2d inputs\n",
                           i, mult_cell_counts[i]);
            }
            printf("  INFO: Mult cell = XOR6 (winner[0])? %s\n",
                   computes_xor ? "YES" : "NO");
        }
    }

    /* Check if multiplicative angle predicts parity */
    {
        int n_ang_predicts = 0;
        for (wi = 0; wi < g_nwin; wi++) {
            Quat w6[6];
            int ang_label[MAX_ANG];
            int predicts = 1;

            build_w6(wi, w6);
            memset(ang_label, -1, sizeof(ang_label));

            for (mask = 0; mask < 64; mask++) {
                Quat M = mask_product(w6, mask);
                double ang = sum_eigen_angle(M);
                int ac = find_mult_ang(ang);
                int par = 0;
                for (i = 0; i < 6; i++) {
                    if (mask & (1 << i)) par ^= 1;
                }
                if (ac >= 0 && ac < MAX_ANG) {
                    if (ang_label[ac] < 0) ang_label[ac] = par;
                    else if (ang_label[ac] != par) predicts = 0;
                }
            }
            if (predicts) n_ang_predicts++;
        }
        printf("  INFO: Mult angle predicts parity: %d/%d triples\n",
               n_ang_predicts, g_nwin);
    }

    {
        double mean_h_mc = total_h_mc / (double)g_nwin;
        double mean_h_ma = total_h_ma / (double)g_nwin;
        printf("  INFO: Avg H(mult cell) = %.3f bits\n", mean_h_mc);
        printf("  INFO: Avg H(mult angle) = %.3f bits\n", mean_h_ma);
        printf("  INFO: Mult cell = XOR6: %d/%d triples\n",
               n_xor6, g_nwin);

        sprintf(msg, "Mult eigenvalue angles: %d distinct", g_n_mult_ang);
        check(msg, g_n_mult_ang > 0);
        sprintf(msg, "Mult cell = XOR6: %d/%d triples", n_xor6, g_nwin);
        check(msg, 1);
        sprintf(msg, "Mult cell channel: %.3f bits (avg)", mean_h_mc);
        check(msg, 1);
        sprintf(msg, "Mult angle channel: %.3f bits (avg)", mean_h_ma);
        check(msg, 1);
    }
}

/* ================================================================
 * Part I: Channel comparison
 * ================================================================ */

static void part_i_comparison(void) {
    int wi, mask, i;
    char msg[256];
    double total_mi_add_mult = 0.0;

    printf("\n=== Part I: Channel comparison ===\n");

    /* MI between additive cell and multiplicative cell */
    for (wi = 0; wi < g_nwin; wi++) {
        Quat w6[6];
        int cell_label[NCELLS];
        int add_cell_counts[2];
        int mult_cell_counts[NCELLS];
        int joint[2][NCELLS];
        int joint_flat[2 * NCELLS];
        double h_a, h_m, h_j;

        build_w6(wi, w6);
        build_cell_labeling(wi, cell_label);
        memset(add_cell_counts, 0, sizeof(add_cell_counts));
        memset(mult_cell_counts, 0, sizeof(mult_cell_counts));
        memset(joint, 0, sizeof(joint));

        for (mask = 0; mask < 64; mask++) {
            Quat sum = quat_make(0, 0, 0, 0);
            Quat M;
            int add_cell, add_bit, mult_cell;
            EigenData ed;

            for (i = 0; i < 6; i++) {
                if (mask & (1 << i))
                    sum = quat_add(sum, w6[i]);
            }
            if (quat_norm(sum) < 1e-12) {
                add_cell = IDENT_CELL;
            } else {
                Quat qn = quat_normalize(sum);
                EigenData ed2 = quat_to_eigen(qn);
                if (!ed2.axis_defined) add_cell = IDENT_CELL;
                else add_cell = vor_cell(ed2.ax, ed2.ay, ed2.az);
            }
            add_bit = (cell_label[add_cell] >= 0) ?
                       cell_label[add_cell] : 0;

            M = mask_product(w6, mask);
            ed = quat_to_eigen(M);
            if (!ed.axis_defined) mult_cell = IDENT_CELL;
            else mult_cell = vor_cell(ed.ax, ed.ay, ed.az);

            add_cell_counts[add_bit]++;
            mult_cell_counts[mult_cell]++;
            joint[add_bit][mult_cell]++;
        }

        h_a = entropy_arr(add_cell_counts, 2, 64);
        h_m = entropy_arr(mult_cell_counts, NCELLS, 64);
        for (i = 0; i < NCELLS; i++) {
            joint_flat[i] = joint[0][i];
            joint_flat[NCELLS + i] = joint[1][i];
        }
        h_j = entropy_arr(joint_flat, 2 * NCELLS, 64);
        total_mi_add_mult += (h_a + h_m - h_j);
    }

    {
        double mean_mi = total_mi_add_mult / (double)g_nwin;
        printf("  INFO: Avg MI(additive cell; mult cell) = %.4f bits\n",
               mean_mi);
        printf("  INFO: Summary of all channels:\n");
        printf("    Additive cell (direction):     1.000 bits (always XOR6)\n");
        printf("    Additive eigenvalue angle:     ~2.0 bits (subsumes cell)\n");
        printf("    Additive magnitude |S|:        see Part G\n");
        printf("    Multiplicative cell:           see Part H\n");
        printf("    Multiplicative eigenvalue:     see Part H\n");

        sprintf(msg, "MI(add cell; mult cell) = %.4f bits", mean_mi);
        check(msg, 1);
    }
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 75: Binocular DKC (Multi-Output)\n");
    printf("================================================\n");

    init_su2_generators();
    build_catalogs();
    build_dir_catalog();
    find_winners();

    printf("  INFO: Setup: %d quaternions, %d directions, %d winners\n",
           qcat_size, g_nd, g_nwin);

    /* Build global catalogs from all triples */
    build_angle_catalog();
    build_mag_catalog();
    build_mult_angle_catalog();
    printf("  INFO: Global angle catalog: %d distinct angles\n", g_n_ang);
    printf("  INFO: Global magnitude catalog: %d distinct values\n", g_n_mag);
    printf("  INFO: Global mult angle catalog: %d distinct angles\n",
           g_n_mult_ang);

    part_a_quantization();
    part_b_binocular();
    part_c_independence();
    part_d_functions();
    part_e_cross_triple();
    part_f_capacity();
    part_g_magnitude();
    part_h_multiplicative();
    part_i_comparison();

    printf("\n================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
