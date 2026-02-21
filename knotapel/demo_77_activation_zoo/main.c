/*
 * KNOTAPEL DEMO 77: Activation Zoo for 8-Input DKC
 * =================================================
 *
 * Core question: Can a different activation function enable XOR8
 * at zeta_8, or is the limitation fundamental to the root of unity?
 *
 * Demo 76 showed: 0/10626 quadruples compute XOR8 with S2 Voronoi
 * activation (14 cells). This demo tests 14 activation functions:
 *
 * Part A: Sector activation (S1, k=4,6,8,10,12,16)
 * Part B: Polar activation (S2 direction x magnitude bins, 2/3/6)
 * Part C: S3 Voronoi (24-cell, 24 cells)
 * Part D: Combined S1 x S2 (sector x Voronoi, k=4,6,8)
 * Part E: Head-to-head comparison table
 * Part F: Winner analysis (quadruple enumeration, XOR6 subsets, geometry)
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
    return quat_make(p.a+q.a, p.b+q.b, p.c+q.c, p.d+q.d);
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
 * Quaternion catalog (24 entries from binary icosahedral group)
 * ================================================================ */

#define MAX_QCAT 128
static Quat qcat[MAX_QCAT];
static int qcat_size = 0;

static int find_quat(Quat q) {
    int i;
    for (i = 0; i < qcat_size; i++) {
        if (fabs(qcat[i].a-q.a)<1e-10 && fabs(qcat[i].b-q.b)<1e-10 &&
            fabs(qcat[i].c-q.c)<1e-10 && fabs(qcat[i].d-q.d)<1e-10)
            return i;
        if (fabs(qcat[i].a+q.a)<1e-10 && fabs(qcat[i].b+q.b)<1e-10 &&
            fabs(qcat[i].c+q.c)<1e-10 && fabs(qcat[i].d+q.d)<1e-10)
            return i;
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
            unsigned long total, idx_l;
            int i;
            total = 1;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)total_gens;
                if (total > 100000) break;
            }
            if (total > 100000) continue;
            for (idx_l = 0; idx_l < total; idx_l++) {
                unsigned long tmp = idx_l;
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
 * Eigenvector direction catalog + S2 Voronoi (14 cells)
 * ================================================================ */

#define MAX_DIR 32
static double g_dir[MAX_DIR][3];
static int g_nd = 0;

#define NCELLS 14
#define IDENT_CELL 13

static void build_dir_catalog(void) {
    int i;
    g_nd = 0;
    for (i = 0; i < qcat_size; i++) {
        Quat q = qcat[i];
        double norm_v, ax, ay, az;
        int found = 0, j;
        if (q.a < 0) { q.a = -q.a; q.b = -q.b; q.c = -q.c; q.d = -q.d; }
        if (q.a > 1.0) q.a = 1.0;
        norm_v = sqrt(q.b*q.b + q.c*q.c + q.d*q.d);
        if (norm_v < 1e-12) continue;
        ax = q.b / norm_v; ay = q.c / norm_v; az = q.d / norm_v;
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

static int vor_cell(double ax, double ay, double az) {
    int i, best = 0;
    double bd = -2.0;
    for (i = 0; i < g_nd; i++) {
        double dp = fabs(ax * g_dir[i][0] + ay * g_dir[i][1] +
                         az * g_dir[i][2]);
        if (dp > bd) { bd = dp; best = i; }
    }
    return best;
}

static int sum_to_s2_cell(Quat sum) {
    double n = quat_norm(sum);
    double norm_v, ax, ay, az;
    double qa, qb, qc, qd;
    if (n < 1e-12) return IDENT_CELL;
    qa = sum.a / n; qb = sum.b / n; qc = sum.c / n; qd = sum.d / n;
    if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
    if (qa > 1.0) qa = 1.0;
    norm_v = sqrt(qb*qb + qc*qc + qd*qd);
    if (norm_v < 1e-12) return IDENT_CELL;
    ax = qb / norm_v; ay = qc / norm_v; az = qd / norm_v;
    return vor_cell(ax, ay, az);
}

/* ================================================================
 * 24-cell vertices + S3 Voronoi (24 cells, no antipodal ID)
 * ================================================================ */

#define N_24CELL 24
static Quat g_24cell[N_24CELL];

static void build_24cell(void) {
    int idx = 0;
    int s1, s2, s3, s4;
    /* 8 axial vertices */
    g_24cell[idx++] = quat_make( 1, 0, 0, 0);
    g_24cell[idx++] = quat_make(-1, 0, 0, 0);
    g_24cell[idx++] = quat_make( 0, 1, 0, 0);
    g_24cell[idx++] = quat_make( 0,-1, 0, 0);
    g_24cell[idx++] = quat_make( 0, 0, 1, 0);
    g_24cell[idx++] = quat_make( 0, 0,-1, 0);
    g_24cell[idx++] = quat_make( 0, 0, 0, 1);
    g_24cell[idx++] = quat_make( 0, 0, 0,-1);
    /* 16 corners: all (+-1/2, +-1/2, +-1/2, +-1/2) */
    for (s1 = -1; s1 <= 1; s1 += 2)
        for (s2 = -1; s2 <= 1; s2 += 2)
            for (s3 = -1; s3 <= 1; s3 += 2)
                for (s4 = -1; s4 <= 1; s4 += 2)
                    g_24cell[idx++] = quat_make(
                        0.5 * (double)s1, 0.5 * (double)s2,
                        0.5 * (double)s3, 0.5 * (double)s4);
}

/* Nearest 24-cell vertex on S3 (no antipodal identification) */
static int sum_to_s3_cell(Quat sum) {
    double n = quat_norm(sum);
    int i, best = 0;
    double bd = -2.0, inv;
    double qa, qb, qc, qd;
    if (n < 1e-12) return 0;
    inv = 1.0 / n;
    qa = sum.a * inv; qb = sum.b * inv;
    qc = sum.c * inv; qd = sum.d * inv;
    for (i = 0; i < N_24CELL; i++) {
        double dp = qa * g_24cell[i].a + qb * g_24cell[i].b +
                    qc * g_24cell[i].c + qd * g_24cell[i].d;
        if (dp > bd) { bd = dp; best = i; }
    }
    return best;
}

/* ================================================================
 * Rotation angle (0 to 360 degrees, full SU(2))
 * No antipodal ID: angle uses raw sign of real part.
 * ================================================================ */

static double rotation_angle(Quat sum) {
    double n = quat_norm(sum);
    double cos_half, half_ang;
    if (n < 1e-12) return 0.0;
    cos_half = sum.a / n;
    if (cos_half > 1.0) cos_half = 1.0;
    if (cos_half < -1.0) cos_half = -1.0;
    half_ang = acos(cos_half);
    return 2.0 * half_ang * 180.0 / M_PI;
}

/* ================================================================
 * Generalized XOR checker
 * Returns 1 if every cell sees only one parity, 0 otherwise.
 * *acc_out = majority-vote correct count (out of n_masks).
 * ================================================================ */

#define MAX_ACT_CELLS 256

static int check_xor_gen(const int *cells, const int *parity,
                          int n_masks, int n_cells, int *acc_out) {
    int cell_even[MAX_ACT_CELLS], cell_odd[MAX_ACT_CELLS];
    int i, correct = 0, is_winner = 1;
    memset(cell_even, 0, (size_t)n_cells * sizeof(int));
    memset(cell_odd, 0, (size_t)n_cells * sizeof(int));
    for (i = 0; i < n_masks; i++) {
        int c = cells[i];
        if (parity[i] == 0) cell_even[c]++;
        else                 cell_odd[c]++;
    }
    for (i = 0; i < n_cells; i++) {
        if (cell_even[i] > 0 && cell_odd[i] > 0) is_winner = 0;
        correct += (cell_even[i] > cell_odd[i]) ? cell_even[i] : cell_odd[i];
    }
    if (acc_out) *acc_out = correct;
    return is_winner;
}

/* ================================================================
 * Activation result tracking
 * ================================================================ */

#define ACT_VOR    0    /* S2 Voronoi baseline (14 cells) */
#define ACT_S4     1    /* Sector k=4 */
#define ACT_S6     2    /* Sector k=6 */
#define ACT_S8     3    /* Sector k=8 */
#define ACT_S10    4    /* Sector k=10 */
#define ACT_S12    5    /* Sector k=12 */
#define ACT_S16    6    /* Sector k=16 */
#define ACT_P2     7    /* Polar 14x2 (28 cells) */
#define ACT_P3     8    /* Polar 14x3 (42 cells) */
#define ACT_P6     9    /* Polar 14x6 (84 cells) */
#define ACT_S3     10   /* S3 24-cell (24 cells) */
#define ACT_C4     11   /* Combined 4x14 (56 cells) */
#define ACT_C6     12   /* Combined 6x14 (84 cells) */
#define ACT_C8     13   /* Combined 8x14 (112 cells) */
#define N_ACT      14

static const char *g_act_name[N_ACT] = {
    "Voronoi S2",
    "Sector k=4",
    "Sector k=6",
    "Sector k=8",
    "Sector k=10",
    "Sector k=12",
    "Sector k=16",
    "Polar 14x2",
    "Polar 14x3",
    "Polar 14x6",
    "S3 24-cell",
    "Sec4 x Vor",
    "Sec6 x Vor",
    "Sec8 x Vor"
};

static const int g_act_cells[N_ACT] = {
    14, 4, 6, 8, 10, 12, 16, 28, 42, 84, 24, 56, 84, 112
};

static int g_act_winners[N_ACT];
static int g_act_best_acc[N_ACT];
static int g_act_best_quad[N_ACT][4];
static int g_total_quads;

/* ACT_C8 winner storage */
#define MAX_C8_WIN 64
static int g_c8_winners[MAX_C8_WIN][4];
static int g_n_c8_win = 0;

/* XOR6 winner storage (for subset analysis) */
#define MAX_WIN6 64
static int g_win6[MAX_WIN6][3];
static int g_nwin6 = 0;

/* Shared k-value arrays */
static const int g_sector_ks[6] = {4, 6, 8, 10, 12, 16};
static const int g_polar_bins[3] = {2, 3, 6};
static const int g_combined_ks[3] = {4, 6, 8};

/* ================================================================
 * Main sweep: compute features + test all activations
 * One pass over all C(24,4) = 10626 quadruples.
 * ================================================================ */

static void sweep_all(void) {
    int ai, aj, ak, al, a;

    for (a = 0; a < N_ACT; a++) {
        g_act_winners[a] = 0;
        g_act_best_acc[a] = 0;
        memset(g_act_best_quad[a], 0, sizeof(g_act_best_quad[a]));
    }
    g_total_quads = 0;

    for (ai = 0; ai < qcat_size; ai++) {
        for (aj = ai + 1; aj < qcat_size; aj++) {
            for (ak = aj + 1; ak < qcat_size; ak++) {
                for (al = ak + 1; al < qcat_size; al++) {
                    Quat w8[8];
                    int mask, i;
                    int parity[256];
                    double ang[256], mag[256];
                    int vor[256], s3v[256];
                    int cells[256];
                    int acc;

                    w8[0] = qcat[ai]; w8[1] = quat_neg(qcat[ai]);
                    w8[2] = qcat[aj]; w8[3] = quat_neg(qcat[aj]);
                    w8[4] = qcat[ak]; w8[5] = quat_neg(qcat[ak]);
                    w8[6] = qcat[al]; w8[7] = quat_neg(qcat[al]);

                    /* Extract features for all 256 masks */
                    for (mask = 0; mask < 256; mask++) {
                        Quat sum = quat_make(0, 0, 0, 0);
                        int par = 0;
                        for (i = 0; i < 8; i++) {
                            if (mask & (1 << i)) {
                                sum = quat_add(sum, w8[i]);
                                par ^= 1;
                            }
                        }
                        parity[mask] = par;
                        ang[mask] = rotation_angle(sum);
                        mag[mask] = quat_norm(sum);
                        vor[mask] = sum_to_s2_cell(sum);
                        s3v[mask] = sum_to_s3_cell(sum);
                    }

                    /* --- ACT_VOR: S2 Voronoi baseline --- */
                    if (check_xor_gen(vor, parity, 256, NCELLS, &acc))
                        g_act_winners[ACT_VOR]++;
                    if (acc > g_act_best_acc[ACT_VOR]) {
                        g_act_best_acc[ACT_VOR] = acc;
                        g_act_best_quad[ACT_VOR][0] = ai;
                        g_act_best_quad[ACT_VOR][1] = aj;
                        g_act_best_quad[ACT_VOR][2] = ak;
                        g_act_best_quad[ACT_VOR][3] = al;
                    }

                    /* --- Sector activations: k=4,6,8,10,12,16 --- */
                    {
                        int ki;
                        for (ki = 0; ki < 6; ki++) {
                            int k = g_sector_ks[ki];
                            int act = ACT_S4 + ki;
                            for (mask = 0; mask < 256; mask++) {
                                int sec = (int)(ang[mask] *
                                          (double)k / 360.0);
                                if (sec >= k) sec = k - 1;
                                if (sec < 0) sec = 0;
                                cells[mask] = sec;
                            }
                            if (check_xor_gen(cells, parity, 256, k, &acc))
                                g_act_winners[act]++;
                            if (acc > g_act_best_acc[act]) {
                                g_act_best_acc[act] = acc;
                                g_act_best_quad[act][0] = ai;
                                g_act_best_quad[act][1] = aj;
                                g_act_best_quad[act][2] = ak;
                                g_act_best_quad[act][3] = al;
                            }
                        }
                    }

                    /* --- Polar: Voronoi x magnitude bins --- */
                    {
                        int bi;
                        for (bi = 0; bi < 3; bi++) {
                            int nb = g_polar_bins[bi];
                            int act = ACT_P2 + bi;
                            int nc = NCELLS * nb;
                            for (mask = 0; mask < 256; mask++) {
                                int mb = (int)(mag[mask] *
                                          (double)nb / 4.0);
                                if (mb >= nb) mb = nb - 1;
                                if (mb < 0) mb = 0;
                                cells[mask] = vor[mask] * nb + mb;
                            }
                            if (check_xor_gen(cells, parity, 256, nc, &acc))
                                g_act_winners[act]++;
                            if (acc > g_act_best_acc[act]) {
                                g_act_best_acc[act] = acc;
                                g_act_best_quad[act][0] = ai;
                                g_act_best_quad[act][1] = aj;
                                g_act_best_quad[act][2] = ak;
                                g_act_best_quad[act][3] = al;
                            }
                        }
                    }

                    /* --- S3 24-cell Voronoi --- */
                    if (check_xor_gen(s3v, parity, 256, N_24CELL, &acc))
                        g_act_winners[ACT_S3]++;
                    if (acc > g_act_best_acc[ACT_S3]) {
                        g_act_best_acc[ACT_S3] = acc;
                        g_act_best_quad[ACT_S3][0] = ai;
                        g_act_best_quad[ACT_S3][1] = aj;
                        g_act_best_quad[ACT_S3][2] = ak;
                        g_act_best_quad[ACT_S3][3] = al;
                    }

                    /* --- Combined: sector x Voronoi --- */
                    {
                        int ki;
                        for (ki = 0; ki < 3; ki++) {
                            int k = g_combined_ks[ki];
                            int act = ACT_C4 + ki;
                            int nc = k * NCELLS;
                            for (mask = 0; mask < 256; mask++) {
                                int sec = (int)(ang[mask] *
                                          (double)k / 360.0);
                                if (sec >= k) sec = k - 1;
                                if (sec < 0) sec = 0;
                                cells[mask] = sec * NCELLS + vor[mask];
                            }
                            if (check_xor_gen(cells, parity, 256, nc, &acc)){
                                g_act_winners[act]++;
                                if (act == ACT_C8 &&
                                    g_n_c8_win < MAX_C8_WIN) {
                                    g_c8_winners[g_n_c8_win][0] = ai;
                                    g_c8_winners[g_n_c8_win][1] = aj;
                                    g_c8_winners[g_n_c8_win][2] = ak;
                                    g_c8_winners[g_n_c8_win][3] = al;
                                    g_n_c8_win++;
                                }
                            }
                            if (acc > g_act_best_acc[act]) {
                                g_act_best_acc[act] = acc;
                                g_act_best_quad[act][0] = ai;
                                g_act_best_quad[act][1] = aj;
                                g_act_best_quad[act][2] = ak;
                                g_act_best_quad[act][3] = al;
                            }
                        }
                    }

                    g_total_quads++;
                }
            }
        }
    }
}

/* ================================================================
 * Part A: Sector activation results
 * ================================================================ */

static void part_a_sector(void) {
    char msg[256];
    int ki, any_winner = 0, best_ki = 0;

    printf("\n=== Part A: Sector activation (Aizenberg MVN style) ===\n");
    printf("  INFO: Rotation angle [0, 360) quantized into k sectors\n");
    printf("  INFO: Tested %d quadruples per sector size\n\n", g_total_quads);

    printf("  %-8s  %5s  %8s  %10s\n",
           "k", "Cells", "Winners", "Best Acc");
    printf("  %-8s  %5s  %8s  %10s\n",
           "---", "-----", "-------", "--------");

    for (ki = 0; ki < 6; ki++) {
        int act = ACT_S4 + ki;
        double acc = 100.0 * (double)g_act_best_acc[act] / 256.0;
        printf("  k=%-6d  %5d  %8d  %8.1f%%\n",
               g_sector_ks[ki], g_act_cells[act],
               g_act_winners[act], acc);
        if (g_act_winners[act] > 0) any_winner = 1;
        if (g_act_best_acc[ACT_S4 + ki] >
            g_act_best_acc[ACT_S4 + best_ki])
            best_ki = ki;
    }

    if (any_winner) {
        for (ki = 0; ki < 6; ki++) {
            int act = ACT_S4 + ki;
            if (g_act_winners[act] > 0) {
                sprintf(msg, "Sector k=%d: %d XOR8 winners!",
                        g_sector_ks[ki], g_act_winners[act]);
                check(msg, 1);
            }
        }
    } else {
        check("No sector activation computes XOR8", 1);
        sprintf(msg, "Best sector acc: %.1f%% at k=%d",
                100.0 * (double)g_act_best_acc[ACT_S4 + best_ki] / 256.0,
                g_sector_ks[best_ki]);
        check(msg, 1);
    }
}

/* ================================================================
 * Part B: Polar activation results
 * ================================================================ */

static void part_b_polar(void) {
    char msg[256];
    int bi, any_winner = 0, best_bi = 0;

    printf("\n=== Part B: Polar activation (Voronoi x magnitude) ===\n");
    printf("  INFO: Equal-width magnitude bins over [0, 4]\n");
    printf("  INFO: Total cells = 14 directions x N mag bins\n\n");

    printf("  %-10s  %5s  %8s  %10s\n",
           "Mag bins", "Cells", "Winners", "Best Acc");
    printf("  %-10s  %5s  %8s  %10s\n",
           "--------", "-----", "-------", "--------");

    for (bi = 0; bi < 3; bi++) {
        int act = ACT_P2 + bi;
        double acc = 100.0 * (double)g_act_best_acc[act] / 256.0;
        printf("  %-10d  %5d  %8d  %8.1f%%\n",
               g_polar_bins[bi], g_act_cells[act],
               g_act_winners[act], acc);
        if (g_act_winners[act] > 0) any_winner = 1;
        if (g_act_best_acc[ACT_P2 + bi] >
            g_act_best_acc[ACT_P2 + best_bi])
            best_bi = bi;
    }

    if (any_winner) {
        for (bi = 0; bi < 3; bi++) {
            int act = ACT_P2 + bi;
            if (g_act_winners[act] > 0) {
                sprintf(msg, "Polar 14x%d: %d XOR8 winners!",
                        g_polar_bins[bi], g_act_winners[act]);
                check(msg, 1);
            }
        }
    } else {
        check("No polar activation computes XOR8", 1);
        sprintf(msg, "Best polar acc: %.1f%% at 14x%d",
                100.0 * (double)g_act_best_acc[ACT_P2 + best_bi] / 256.0,
                g_polar_bins[best_bi]);
        check(msg, 1);
    }
}

/* ================================================================
 * Part C: S3 Voronoi results
 * ================================================================ */

static void part_c_s3(void) {
    char msg[256];
    double acc = 100.0 * (double)g_act_best_acc[ACT_S3] / 256.0;

    printf("\n=== Part C: S3 Voronoi (24-cell, 24 cells) ===\n");
    printf("  INFO: Full quaternion Voronoi, no antipodal ID\n");
    printf("  INFO: 256 masks / 24 cells = 10.7 masks/cell\n\n");

    printf("  Winners: %d / %d quadruples\n",
           g_act_winners[ACT_S3], g_total_quads);
    printf("  Best accuracy: %.1f%% [%d,%d,%d,%d]\n", acc,
           g_act_best_quad[ACT_S3][0], g_act_best_quad[ACT_S3][1],
           g_act_best_quad[ACT_S3][2], g_act_best_quad[ACT_S3][3]);

    if (g_act_winners[ACT_S3] > 0) {
        sprintf(msg, "S3 24-cell: %d XOR8 winners!",
                g_act_winners[ACT_S3]);
        check(msg, 1);
    } else {
        check("S3 24-cell does not compute XOR8", 1);
        sprintf(msg, "S3 best accuracy: %.1f%%", acc);
        check(msg, 1);
    }
}

/* ================================================================
 * Part D: Combined S1 x S2 results
 * ================================================================ */

static void part_d_combined(void) {
    char msg[256];
    int ki, any_winner = 0, best_ki = 0;

    printf("\n=== Part D: Combined sector x Voronoi ===\n");
    printf("  INFO: Sector(angle) x Voronoi(direction)\n");
    printf("  INFO: Total cells = k sectors x 14 directions\n\n");

    printf("  %-10s  %5s  %8s  %10s\n",
           "Sectors", "Cells", "Winners", "Best Acc");
    printf("  %-10s  %5s  %8s  %10s\n",
           "-------", "-----", "-------", "--------");

    for (ki = 0; ki < 3; ki++) {
        int act = ACT_C4 + ki;
        double acc = 100.0 * (double)g_act_best_acc[act] / 256.0;
        printf("  k=%-8d  %5d  %8d  %8.1f%%\n",
               g_combined_ks[ki], g_act_cells[act],
               g_act_winners[act], acc);
        if (g_act_winners[act] > 0) any_winner = 1;
        if (g_act_best_acc[ACT_C4 + ki] >
            g_act_best_acc[ACT_C4 + best_ki])
            best_ki = ki;
    }

    if (any_winner) {
        for (ki = 0; ki < 3; ki++) {
            int act = ACT_C4 + ki;
            if (g_act_winners[act] > 0) {
                sprintf(msg, "Combined %dx14: %d XOR8 winners!",
                        g_combined_ks[ki], g_act_winners[act]);
                check(msg, 1);
            }
        }
    } else {
        check("No combined activation computes XOR8", 1);
        sprintf(msg, "Best combined acc: %.1f%% at %dx14",
                100.0 * (double)g_act_best_acc[ACT_C4 + best_ki] / 256.0,
                g_combined_ks[best_ki]);
        check(msg, 1);
    }
}

/* ================================================================
 * Part E: Head-to-head comparison
 * ================================================================ */

static void part_e_comparison(void) {
    char msg[256];
    int a, any_winner = 0, best_a = 0;

    printf("\n=== Part E: Head-to-head comparison ===\n\n");

    printf("  %-16s  %5s  %10s  %7s  %8s\n",
           "Activation", "Cells", "Masks/cell", "Winners", "Best Acc");
    printf("  %-16s  %5s  %10s  %7s  %8s\n",
           "----------------", "-----", "----------",
           "-------", "--------");

    for (a = 0; a < N_ACT; a++) {
        double mpc = 256.0 / (double)g_act_cells[a];
        double acc = 100.0 * (double)g_act_best_acc[a] / 256.0;
        printf("  %-16s  %5d  %10.1f  %7d  %6.1f%%\n",
               g_act_name[a], g_act_cells[a], mpc,
               g_act_winners[a], acc);
        if (g_act_winners[a] > 0) any_winner = 1;
        if (g_act_best_acc[a] > g_act_best_acc[best_a]) best_a = a;
    }

    printf("\n  INFO: 6-input baseline: 36 XOR6 winners "
           "at 14 Voronoi cells\n");
    printf("  INFO: Total quadruples tested: %d\n\n", g_total_quads);

    /* Sanity: correct quad count */
    {
        int expected = qcat_size * (qcat_size - 1) * (qcat_size - 2) *
                       (qcat_size - 3) / 24;
        sprintf(msg, "Swept all %d quadruples", g_total_quads);
        check(msg, g_total_quads == expected);
    }

    if (any_winner) {
        int tw = 0;
        for (a = 0; a < N_ACT; a++) tw += g_act_winners[a];
        sprintf(msg, "At least one activation computes XOR8 (%d total)", tw);
        check(msg, 1);
    } else {
        check("NO activation computes XOR8 at zeta_8", 1);
    }

    sprintf(msg, "Best overall: %s at %.1f%%",
            g_act_name[best_a],
            100.0 * (double)g_act_best_acc[best_a] / 256.0);
    check(msg, 1);

    /* Characterize best accuracy relative to chance */
    {
        double best_pct = 100.0 * (double)g_act_best_acc[best_a] / 256.0;
        if (best_pct > 55.0) {
            sprintf(msg, "Best accuracy %.1f%% exceeds 55%%", best_pct);
            check(msg, 1);
        } else {
            sprintf(msg, "Best accuracy %.1f%% near random chance (50%%)",
                    best_pct);
            check(msg, 1);
        }
    }
}

/* ================================================================
 * XOR6 winner finder (for subset analysis)
 * ================================================================ */

static void find_winners_6(void) {
    int ai, aj, ak;
    g_nwin6 = 0;
    for (ai = 0; ai < qcat_size; ai++) {
        for (aj = ai + 1; aj < qcat_size; aj++) {
            for (ak = aj + 1; ak < qcat_size; ak++) {
                Quat w6[6];
                int mask, i;
                int par6[64], vor6[64];
                w6[0] = qcat[ai]; w6[1] = quat_neg(qcat[ai]);
                w6[2] = qcat[aj]; w6[3] = quat_neg(qcat[aj]);
                w6[4] = qcat[ak]; w6[5] = quat_neg(qcat[ak]);
                for (mask = 0; mask < 64; mask++) {
                    Quat sum = quat_make(0, 0, 0, 0);
                    int par = 0;
                    for (i = 0; i < 6; i++) {
                        if (mask & (1 << i)) {
                            sum = quat_add(sum, w6[i]);
                            par ^= 1;
                        }
                    }
                    par6[mask] = par;
                    vor6[mask] = sum_to_s2_cell(sum);
                }
                if (check_xor_gen(vor6, par6, 64, NCELLS, NULL)) {
                    if (g_nwin6 < MAX_WIN6) {
                        g_win6[g_nwin6][0] = ai;
                        g_win6[g_nwin6][1] = aj;
                        g_win6[g_nwin6][2] = ak;
                        g_nwin6++;
                    }
                }
            }
        }
    }
}

static int is_xor6_winner(int a, int b, int c) {
    int i;
    for (i = 0; i < g_nwin6; i++) {
        if (g_win6[i][0] == a && g_win6[i][1] == b &&
            g_win6[i][2] == c) return 1;
    }
    return 0;
}

/* ================================================================
 * Part F: Winner analysis
 * ================================================================ */

static void part_f_winners(void) {
    char msg[256];
    int wi, i;
    int unique_indices[96];
    int n_unique = 0;
    int total_subsets = 0, matching_subsets = 0;

    printf("\n=== Part F: Winner analysis (%d winners) ===\n\n",
           g_n_c8_win);

    for (wi = 0; wi < g_n_c8_win; wi++) {
        int *q = g_c8_winners[wi];
        int sub[4][3];
        int n_sub_match = 0;

        printf("  Winner %d: [%d, %d, %d, %d]\n", wi, q[0], q[1], q[2], q[3]);

        /* Print quaternion values and eigenvector directions */
        for (i = 0; i < 4; i++) {
            Quat qv = qcat[q[i]];
            double norm_v, ax, ay, az;
            int cell;
            Quat qn = qv;
            if (qn.a < 0) {
                qn.a = -qn.a; qn.b = -qn.b;
                qn.c = -qn.c; qn.d = -qn.d;
            }
            norm_v = sqrt(qn.b*qn.b + qn.c*qn.c + qn.d*qn.d);
            if (norm_v > 1e-12) {
                ax = qn.b / norm_v; ay = qn.c / norm_v;
                az = qn.d / norm_v;
                cell = vor_cell(ax, ay, az);
            } else {
                ax = 0; ay = 0; az = 1; cell = IDENT_CELL;
            }
            printf("    q[%d] = (%.4f, %.4f, %.4f, %.4f)  "
                   "dir=(%.3f, %.3f, %.3f) cell=%d\n",
                   q[i], qv.a, qv.b, qv.c, qv.d,
                   ax, ay, az, cell);

            /* Track unique indices */
            {
                int found = 0, j;
                for (j = 0; j < n_unique; j++) {
                    if (unique_indices[j] == q[i]) { found = 1; break; }
                }
                if (!found && n_unique < 96)
                    unique_indices[n_unique++] = q[i];
            }
        }

        /* Check C(4,3) = 4 triples as XOR6 subsets */
        sub[0][0] = q[0]; sub[0][1] = q[1]; sub[0][2] = q[2];
        sub[1][0] = q[0]; sub[1][1] = q[1]; sub[1][2] = q[3];
        sub[2][0] = q[0]; sub[2][1] = q[2]; sub[2][2] = q[3];
        sub[3][0] = q[1]; sub[3][1] = q[2]; sub[3][2] = q[3];

        for (i = 0; i < 4; i++) {
            int hit = is_xor6_winner(sub[i][0], sub[i][1], sub[i][2]);
            if (hit) {
                printf("    XOR6 subset: [%d,%d,%d] = YES\n",
                       sub[i][0], sub[i][1], sub[i][2]);
                n_sub_match++;
            }
            total_subsets++;
        }
        if (n_sub_match == 0)
            printf("    XOR6 subsets: none\n");
        matching_subsets += n_sub_match;
        printf("\n");
    }

    /* Summary statistics */
    printf("  INFO: Unique catalog indices across winners: %d / %d\n",
           n_unique, qcat_size);
    printf("  INFO: XOR6 triple subsets: %d / %d\n",
           matching_subsets, total_subsets);

    /* Cell usage analysis */
    {
        int cell_hist[NCELLS];
        memset(cell_hist, 0, sizeof(cell_hist));
        for (wi = 0; wi < g_n_c8_win; wi++) {
            for (i = 0; i < 4; i++) {
                Quat qv = qcat[g_c8_winners[wi][i]];
                double norm_v, ax, ay, az;
                int cell;
                if (qv.a < 0) {
                    qv.a = -qv.a; qv.b = -qv.b;
                    qv.c = -qv.c; qv.d = -qv.d;
                }
                norm_v = sqrt(qv.b*qv.b + qv.c*qv.c + qv.d*qv.d);
                if (norm_v > 1e-12) {
                    ax = qv.b / norm_v; ay = qv.c / norm_v;
                    az = qv.d / norm_v;
                    cell = vor_cell(ax, ay, az);
                } else {
                    cell = IDENT_CELL;
                }
                cell_hist[cell]++;
            }
        }
        printf("  INFO: Voronoi cell distribution of winner quaternions:\n");
        for (i = 0; i < NCELLS; i++) {
            if (cell_hist[i] > 0)
                printf("    cell %2d: %d entries\n", i, cell_hist[i]);
        }
    }

    /* Eigenvalue angle diversity */
    {
        double angles[96];
        int n_ang = 0;
        for (wi = 0; wi < g_n_c8_win; wi++) {
            for (i = 0; i < 4; i++) {
                Quat qv = qcat[g_c8_winners[wi][i]];
                double theta, norm_v;
                int found = 0, j;
                if (qv.a < 0) {
                    qv.a = -qv.a; qv.b = -qv.b;
                    qv.c = -qv.c; qv.d = -qv.d;
                }
                if (qv.a > 1.0) qv.a = 1.0;
                norm_v = sqrt(qv.b*qv.b + qv.c*qv.c + qv.d*qv.d);
                theta = atan2(norm_v, qv.a) * 180.0 / M_PI;
                for (j = 0; j < n_ang; j++) {
                    if (fabs(angles[j] - theta) < 0.01) {
                        found = 1; break;
                    }
                }
                if (!found && n_ang < 96)
                    angles[n_ang++] = theta;
            }
        }
        printf("  INFO: Distinct eigenvalue half-angles: %d\n", n_ang);
        for (i = 0; i < n_ang; i++)
            printf("    %.3f deg\n", angles[i]);
    }

    sprintf(msg, "XOR8 winners enumerated: %d", g_n_c8_win);
    check(msg, g_n_c8_win > 0);

    sprintf(msg, "XOR6 subsets found: %d of %d triples",
            matching_subsets, total_subsets);
    check(msg, 1);

    sprintf(msg, "Unique quaternions in winners: %d of %d",
            n_unique, qcat_size);
    check(msg, 1);
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 77: Activation Zoo for 8-Input DKC\n");
    printf("=================================================\n");

    init_su2_generators();
    build_catalogs();
    build_dir_catalog();
    build_24cell();

    printf("  INFO: Setup: %d quaternions, %d S2 directions, "
           "%d S3 vertices\n", qcat_size, g_nd, N_24CELL);
    printf("  INFO: Quadruples: C(%d,4) = %d\n",
           qcat_size,
           qcat_size * (qcat_size - 1) * (qcat_size - 2) *
           (qcat_size - 3) / 24);
    printf("  INFO: Testing %d activation functions...\n", N_ACT);

    printf("  INFO: Finding XOR6 winners (baseline)...\n");
    find_winners_6();
    printf("  INFO: XOR6 winners: %d\n", g_nwin6);

    sweep_all();

    part_a_sector();
    part_b_polar();
    part_c_s3();
    part_d_combined();
    part_e_comparison();
    part_f_winners();

    printf("\n=================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
