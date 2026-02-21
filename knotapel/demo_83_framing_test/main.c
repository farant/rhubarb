/*
 * KNOTAPEL DEMO 83: Framing as Computational Content
 * ====================================================
 *
 * The Kauffman bracket depends on writhe (framing).
 * The Jones polynomial normalizes writhe away: V(K) = (-A^3)^{-w} * <K>.
 * Kirby (1978) proved framing determines 3-manifold topology.
 *
 * Question: does removing writhe destroy DKC's computational power?
 *
 * Part A: Writhe census -- histogram, correlation with depth
 * Part B: Jones normalization -- catalog size after de-framing
 * Part C: XOR with Jones-normalized values -- THE KEY TEST
 * Part D: Writhe distribution of bracket-based winners
 * Part E: Writhe-only computation test
 * Part F: zeta_8 control
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

#define MAX_ROUNDS 16
static int n_rounds = 0;

/* Generator writhes: sigma1=+1, sigma1^-1=-1, sigma2=+1, sigma2^-1=-1 */
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

    /* Identity -- depth 0, writhe 0 */
    g_cat[0].a = 1; g_cat[0].b = 0; g_cat[0].c = 0; g_cat[0].d = 0;
    g_depth[0] = 0;
    g_writhe[0] = 0;
    g_cat_size = 1;

    /* 4 generators */
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
    printf("  Round 0: %d entries\n", g_cat_size);

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
            printf("  Round %d: %d entries (+%d)\n",
                   rd, g_cat_size, g_cat_size - prev);
            n_rounds++;
        }
        rd++;
    } while (g_cat_size > prev && n_rounds < MAX_ROUNDS);
}

/* ================================================================ */
/* Jones normalization                                              */
/* ================================================================ */

/*
 * Jones polynomial: V(K) = (-A^3)^{-w} * <K>
 * For zeta_8:  A = e^{i*pi/4}, (-A^3)^{-1} = A, so factor = A^w
 * For zeta_12: A = e^{i*pi/6}, (-A^3)^{-1} = A^3, so factor = A^{3w}
 *
 * General: (-A^3)^{-w} = e^{-iw(3*half_angle + pi)}
 * As SU(2) left-multiplier: (cos(phi), sin(phi), 0, 0) with phi above.
 */

static Quat g_jones_cat[MAX_QCAT];

static void jones_normalize_catalog(int cat_size) {
    double jones_phase_per_w;
    int i;

    /* Phase per unit writhe: -(3*half_angle + pi) */
    jones_phase_per_w = -(3.0 * g_half_angle + M_PI);

    for (i = 0; i < cat_size; i++) {
        double phi = (double)g_writhe[i] * jones_phase_per_w;
        Quat norm_q;
        norm_q.a = cos(phi);
        norm_q.b = sin(phi);
        norm_q.c = 0.0;
        norm_q.d = 0.0;
        g_jones_cat[i] = qmul(&norm_q, &g_cat[i]);
    }
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
/* Direction catalog + angles                                       */
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

static int count_distinct_quats(const Quat *cats, int n) {
    int i, j, count = 0;
    for (i = 0; i < n; i++) {
        int dup = 0;
        for (j = 0; j < i; j++) {
            if ((fabs(cats[i].a - cats[j].a) < 1e-10 &&
                 fabs(cats[i].b - cats[j].b) < 1e-10 &&
                 fabs(cats[i].c - cats[j].c) < 1e-10 &&
                 fabs(cats[i].d - cats[j].d) < 1e-10) ||
                (fabs(cats[i].a + cats[j].a) < 1e-10 &&
                 fabs(cats[i].b + cats[j].b) < 1e-10 &&
                 fabs(cats[i].c + cats[j].c) < 1e-10 &&
                 fabs(cats[i].d + cats[j].d) < 1e-10)) {
                dup = 1; break;
            }
        }
        if (!dup) count++;
    }
    return count;
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
/* Writhe-only XOR test                                             */
/* ================================================================ */

#define WRITHE_OFFSET 64  /* offset so writhe sums are non-negative */
#define WRITHE_CELLS 129  /* -64 to +64 */

static int w_cell_even[WRITHE_CELLS], w_cell_odd[WRITHE_CELLS];

static int test_xor_writhe(const int *indices, int n_weights) {
    int n_inputs = 2 * n_weights;
    int n_masks;
    int wvals[14];
    int mask, i, result = 1;

    n_masks = 1 << n_inputs;

    for (i = 0; i < n_weights; i++) {
        wvals[2*i]   =  g_writhe[indices[i]];
        wvals[2*i+1] = -g_writhe[indices[i]];
    }

    memset(w_cell_even, 0, sizeof(w_cell_even));
    memset(w_cell_odd, 0, sizeof(w_cell_odd));

    for (mask = 0; mask < n_masks; mask++) {
        int wsum = 0, par = 0, cell;

        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                wsum += wvals[i];
                par ^= 1;
            }
        }

        cell = wsum + WRITHE_OFFSET;
        if (cell < 0 || cell >= WRITHE_CELLS) { result = 0; break; }

        if (par == 0) {
            w_cell_even[cell]++;
            if (w_cell_odd[cell] > 0) { result = 0; break; }
        } else {
            w_cell_odd[cell]++;
            if (w_cell_even[cell] > 0) { result = 0; break; }
        }
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
/* Main                                                             */
/* ================================================================ */

int main(void) {
    int i, j, w;
    char msg[256];

    printf("KNOTAPEL DEMO 83: Framing as Computational Content\n");
    printf("====================================================\n\n");

    /* ============================================================ */
    /* Part A: Writhe Census (zeta_12)                              */
    /* ============================================================ */
    printf("=== Part A: Writhe Census (zeta_12) ===\n\n");
    init_su2(M_PI / 6.0);
    build_closure();
    printf("\n  Total: %d entries in %d rounds\n\n", g_cat_size, n_rounds);

    {
        int w_min = 9999, w_max = -9999;
        int whist[32]; /* writhes from -15 to +15 mapped to 0..30 */
        double depth_writhe_sum = 0.0;
        double depth_sum = 0.0, w_sum = 0.0;
        double depth_sq_sum = 0.0, w_sq_sum = 0.0;
        double corr;
        int n_nonzero_writhe = 0;

        memset(whist, 0, sizeof(whist));

        for (i = 0; i < g_cat_size; i++) {
            int wr = g_writhe[i];
            if (wr < w_min) w_min = wr;
            if (wr > w_max) w_max = wr;
            if (wr >= -15 && wr <= 15) whist[wr + 15]++;
            if (wr != 0) n_nonzero_writhe++;

            depth_writhe_sum += (double)g_depth[i] * (double)wr;
            depth_sum += (double)g_depth[i];
            w_sum += (double)wr;
            depth_sq_sum += (double)g_depth[i] * (double)g_depth[i];
            w_sq_sum += (double)wr * (double)wr;
        }

        printf("  Writhe range: [%d, %d]\n", w_min, w_max);
        printf("  Mean writhe: %.3f\n", w_sum / (double)g_cat_size);
        printf("  Entries with writhe != 0: %d / %d (%.1f%%)\n\n",
               n_nonzero_writhe, g_cat_size,
               100.0 * (double)n_nonzero_writhe / (double)g_cat_size);

        printf("  Writhe | Count\n");
        printf("  -------|------\n");
        for (i = 0; i < 31; i++) {
            if (whist[i] > 0)
                printf("  %5d  | %5d\n", i - 15, whist[i]);
        }
        printf("\n");

        /* Depth-writhe correlation */
        {
            double n_d = (double)g_cat_size;
            double cov = depth_writhe_sum/n_d - (depth_sum/n_d)*(w_sum/n_d);
            double sd = sqrt(depth_sq_sum/n_d - (depth_sum/n_d)*(depth_sum/n_d));
            double sw = sqrt(w_sq_sum/n_d - (w_sum/n_d)*(w_sum/n_d));
            corr = (sd > 1e-12 && sw > 1e-12) ? cov / (sd * sw) : 0.0;
        }
        printf("  Depth-writhe correlation: %.4f\n\n", corr);

        /* Depth vs |writhe| table */
        printf("  Depth | MeanW | Mean|W| | MaxW | MinW\n");
        printf("  ------|-------|---------|------|-----\n");
        {
            int d;
            for (d = 0; d < n_rounds; d++) {
                int cnt = 0, dmin = 9999, dmax = -9999;
                double dw_sum = 0.0, daw_sum = 0.0;
                for (i = 0; i < g_cat_size; i++) {
                    if (g_depth[i] == d) {
                        int wr = g_writhe[i];
                        dw_sum += (double)wr;
                        daw_sum += fabs((double)wr);
                        if (wr < dmin) dmin = wr;
                        if (wr > dmax) dmax = wr;
                        cnt++;
                    }
                }
                if (cnt > 0) {
                    printf("  %5d | %5.2f | %7.2f | %4d | %4d\n",
                           d, dw_sum/(double)cnt, daw_sum/(double)cnt,
                           dmax, dmin);
                }
            }
        }
        printf("\n");

        check("Writhe tracked for all entries", w_min <= 0 && w_max >= 0);
        sprintf(msg, "Writhe range [%d, %d]", w_min, w_max);
        check(msg, w_max > 0);
    }

    save_catalog();

    /* ============================================================ */
    /* Bracket capacity (save winners for Part D)                   */
    /* ============================================================ */
    printf("\n=== Bracket Capacity (baseline) ===\n\n");
    {
        CapResult bracket_cap;
        find_capacity(g_cat_size, &bracket_cap);
        printf("\n  Bracket: dirs=%d, ang=%d, max_xor=%d\n",
               bracket_cap.n_dirs, bracket_cap.n_angles, bracket_cap.max_xor);
        printf("  XOR6=%d, XOR8=%d, XOR10=%d, XOR12=%d\n\n",
               bracket_cap.xor6, bracket_cap.xor8,
               bracket_cap.xor10, bracket_cap.xor12);
        check("Bracket baseline XOR6 > 0", bracket_cap.xor6 > 0);
        check("Bracket baseline XOR12 > 0", bracket_cap.xor12 > 0);
    }

    /* Save bracket winners for Part D analysis */
    {
        int saved_nwin[8];
        int saved_win[8][MAX_WIN][MAX_TUPLE];
        int level;

        memcpy(saved_nwin, g_nwin, sizeof(g_nwin));
        memcpy(saved_win, g_win, sizeof(g_win));

        /* ============================================================ */
        /* Part D: Writhe Distribution of Bracket Winners               */
        /* ============================================================ */
        printf("=== Part D: Writhe Distribution of Winners ===\n\n");

        printf("  Level | #Win | MeanW | Mean|W| | MinW | MaxW |"
               " Writhe histogram\n");
        printf("  ------|------|-------|---------|------|------|"
               "-----------------\n");

        for (level = 3; level <= 6; level++) {
            int nw = saved_nwin[level];
            int xor_n = level * 2;
            int total_entries = 0;
            double sum_w = 0.0, sum_aw = 0.0;
            int min_w = 9999, max_w = -9999;
            int whist2[32];
            int n_mixed = 0; /* winners with both + and - writhe */

            if (nw == 0) {
                printf("  XOR%-2d | %4d |  --   |   --    |  --  |  -- "
                       " | --\n", xor_n, 0);
                continue;
            }

            memset(whist2, 0, sizeof(whist2));

            for (w = 0; w < nw; w++) {
                int has_pos = 0, has_neg = 0;
                for (j = 0; j < level; j++) {
                    int idx = saved_win[level][w][j];
                    int wr = g_writhe[idx];
                    sum_w += (double)wr;
                    sum_aw += fabs((double)wr);
                    if (wr < min_w) min_w = wr;
                    if (wr > max_w) max_w = wr;
                    total_entries++;
                    if (wr >= -15 && wr <= 15) whist2[wr + 15]++;
                    if (wr > 0) has_pos = 1;
                    if (wr < 0) has_neg = 1;
                }
                if (has_pos && has_neg) n_mixed++;
            }

            printf("  XOR%-2d | %4d | %5.2f | %7.2f | %4d | %4d | ",
                   xor_n, nw,
                   sum_w / (double)total_entries,
                   sum_aw / (double)total_entries,
                   min_w, max_w);
            for (i = 0; i < 31; i++) {
                if (whist2[i] > 0) printf("w%d:%d ", i-15, whist2[i]);
            }
            printf("\n");

            printf("         Mixed-writhe winners: %d/%d (%.0f%%)\n",
                   n_mixed, nw, 100.0 * (double)n_mixed / (double)nw);
        }
        printf("\n");

        /* Sample XOR6 winners with writhes */
        printf("  Sample XOR6 winners:\n");
        for (w = 0; w < saved_nwin[3] && w < 4; w++) {
            printf("    [%d,%d,%d] writhes=[%d,%d,%d] depths=[%d,%d,%d]\n",
                   saved_win[3][w][0], saved_win[3][w][1], saved_win[3][w][2],
                   g_writhe[saved_win[3][w][0]],
                   g_writhe[saved_win[3][w][1]],
                   g_writhe[saved_win[3][w][2]],
                   g_depth[saved_win[3][w][0]],
                   g_depth[saved_win[3][w][1]],
                   g_depth[saved_win[3][w][2]]);
        }

        if (saved_nwin[6] > 0) {
            printf("  Sample XOR12 winners:\n");
            for (w = 0; w < saved_nwin[6] && w < 4; w++) {
                printf("    [");
                for (j = 0; j < 6; j++)
                    printf("%d%s", saved_win[6][w][j], j<5?",":"");
                printf("] writhes=[");
                for (j = 0; j < 6; j++)
                    printf("%d%s", g_writhe[saved_win[6][w][j]], j<5?",":"");
                printf("] depths=[");
                for (j = 0; j < 6; j++)
                    printf("%d%s", g_depth[saved_win[6][w][j]], j<5?",":"");
                printf("]\n");
            }
        }
        printf("\n");

        check("Part D: writhe data collected", saved_nwin[3] > 0);
    }

    /* ============================================================ */
    /* Part B: Jones Normalization Analysis                         */
    /* ============================================================ */
    printf("\n=== Part B: Jones Normalization ===\n\n");
    restore_catalog();
    {
        int bracket_distinct, jones_distinct;
        int jones_dirs, jones_angles;

        jones_normalize_catalog(g_cat_size);

        bracket_distinct = count_distinct_quats(g_cat, g_cat_size);
        jones_distinct = count_distinct_quats(g_jones_cat, g_cat_size);

        printf("  Bracket catalog: %d entries, %d distinct quaternions\n",
               g_cat_size, bracket_distinct);
        printf("  Jones catalog:   %d entries, %d distinct quaternions\n",
               g_cat_size, jones_distinct);

        /* Count Jones dirs and angles by temporarily swapping catalog */
        memcpy(g_cat, g_jones_cat, sizeof(Quat) * (size_t)g_cat_size);
        build_dirs(g_cat_size);
        jones_dirs = g_nd;
        jones_angles = count_angles(g_cat_size);

        restore_catalog();
        build_dirs(g_cat_size);

        printf("\n  Bracket: %d dirs, %d angles\n", g_nd, count_angles(g_cat_size));
        printf("  Jones:   %d dirs, %d angles\n\n", jones_dirs, jones_angles);

        sprintf(msg, "Jones has %d distinct (bracket has %d)",
                jones_distinct, bracket_distinct);
        check(msg, 1);

        if (jones_distinct < bracket_distinct) {
            printf("  --> Jones normalization COLLAPSES %d quaternions\n",
                   bracket_distinct - jones_distinct);
            printf("  --> %.1f%% information loss\n\n",
                   100.0 * (double)(bracket_distinct - jones_distinct) /
                   (double)bracket_distinct);
        } else if (jones_distinct == bracket_distinct) {
            printf("  --> Jones normalization preserves all quaternions\n");
            printf("  --> But may change directions/angles\n\n");
        } else {
            printf("  --> Jones normalization INCREASES distinct count!\n\n");
        }
    }

    /* ============================================================ */
    /* Part C: XOR with Jones-Normalized Values -- THE KEY TEST     */
    /* ============================================================ */
    printf("=== Part C: XOR with Jones-Normalized Values ===\n\n");
    restore_catalog();
    jones_normalize_catalog(g_cat_size);
    {
        CapResult jones_cap;

        /* Replace bracket catalog with Jones-normalized */
        memcpy(g_cat, g_jones_cat, sizeof(Quat) * (size_t)g_cat_size);

        printf("  Running XOR capacity on Jones-normalized catalog...\n");
        find_capacity(g_cat_size, &jones_cap);

        printf("\n  Jones:   dirs=%d, ang=%d, max_xor=%d\n",
               jones_cap.n_dirs, jones_cap.n_angles, jones_cap.max_xor);
        printf("  XOR6=%d, XOR8=%d, XOR10=%d, XOR12=%d\n\n",
               jones_cap.xor6, jones_cap.xor8,
               jones_cap.xor10, jones_cap.xor12);

        printf("  +-----------+----------+--------+\n");
        printf("  | Level     | Bracket  | Jones  |\n");
        printf("  +-----------+----------+--------+\n");
        printf("  | XOR6      | %5d    | %5d  |\n",
               32, jones_cap.xor6);
        printf("  | XOR8      | %5d    | %5d  |\n",
               32, jones_cap.xor8);
        printf("  | XOR10     | %5d    | %5d  |\n",
               32, jones_cap.xor10);
        printf("  | XOR12     | %5d    | %5d  |\n",
               32, jones_cap.xor12);
        printf("  +-----------+----------+--------+\n\n");

        check("Jones XOR6 tested", 1);
        if (jones_cap.max_xor < 12) {
            check("Jones loses capacity vs bracket", 1);
            printf("  --> JONES NORMALIZATION DESTROYS COMPUTATIONAL POWER\n");
            printf("  --> Framing IS the computational content!\n\n");
        } else {
            check("Jones retains full capacity", 1);
            printf("  --> Framing is supplementary, not essential\n\n");
        }
    }

    /* ============================================================ */
    /* Part E: Writhe-Only Computation                              */
    /* ============================================================ */
    printf("=== Part E: Writhe-Only Computation ===\n\n");
    restore_catalog();
    {
        int ai, aj, ak;
        int bf_limit;
        int writhe_xor6_wins = 0;
        int indices[3];

        bf_limit = g_cat_size < 100 ? g_cat_size : 100;

        printf("  Testing if writhe alone can separate XOR6...\n");
        printf("  (brute force %d entries)...", bf_limit);
        fflush(stdout);

        for (ai = 0; ai < bf_limit && writhe_xor6_wins < MAX_WIN; ai++)
        for (aj = ai+1; aj < bf_limit && writhe_xor6_wins < MAX_WIN; aj++)
        for (ak = aj+1; ak < bf_limit && writhe_xor6_wins < MAX_WIN; ak++) {
            indices[0] = ai; indices[1] = aj; indices[2] = ak;
            if (test_xor_writhe(indices, 3)) {
                writhe_xor6_wins++;
            }
        }
        printf(" %d winners\n\n", writhe_xor6_wins);

        if (writhe_xor6_wins == 0) {
            printf("  --> Writhe alone CANNOT separate XOR\n");
            printf("  --> Writhe needs quaternion geometry to compute\n\n");
        } else {
            printf("  --> Writhe alone CAN separate XOR6 (%d winners)\n\n",
                   writhe_xor6_wins);
        }

        check("Writhe-only test completed", 1);
    }

    /* ============================================================ */
    /* Part F: zeta_8 Control                                       */
    /* ============================================================ */
    printf("=== Part F: zeta_8 Control ===\n\n");
    init_su2(M_PI / 4.0);
    g_cat_size = 0;
    n_rounds = 0;
    memset(g_depth, 0, sizeof(g_depth));
    memset(g_writhe, 0, sizeof(g_writhe));
    build_closure();
    printf("\n  zeta_8: %d entries, %d rounds\n\n", g_cat_size, n_rounds);

    check("zeta_8 closes at 24", g_cat_size == 24);

    /* Writhe census for zeta_8 */
    {
        int w_min = 9999, w_max = -9999;
        printf("  Writhe distribution:\n");
        for (i = 0; i < g_cat_size; i++) {
            if (g_writhe[i] < w_min) w_min = g_writhe[i];
            if (g_writhe[i] > w_max) w_max = g_writhe[i];
        }
        printf("  Range: [%d, %d]\n", w_min, w_max);
        {
            int wr;
            for (wr = w_min; wr <= w_max; wr++) {
                int cnt = 0;
                for (i = 0; i < g_cat_size; i++)
                    if (g_writhe[i] == wr) cnt++;
                if (cnt > 0)
                    printf("    w=%2d: %d entries\n", wr, cnt);
            }
        }
        printf("\n");
    }

    /* Bracket capacity at zeta_8 */
    {
        CapResult r8_bracket, r8_jones;

        printf("  Bracket capacity:\n");
        find_capacity(g_cat_size, &r8_bracket);
        printf("  XOR6=%d, XOR8=%d, max=%d\n\n",
               r8_bracket.xor6, r8_bracket.xor8, r8_bracket.max_xor);

        check("zeta_8 bracket XOR6 > 0", r8_bracket.xor6 > 0);

        /* Jones-normalize and test */
        jones_normalize_catalog(g_cat_size);
        save_catalog();
        memcpy(g_cat, g_jones_cat, sizeof(Quat) * (size_t)g_cat_size);

        printf("  Jones-normalized capacity:\n");
        find_capacity(g_cat_size, &r8_jones);
        printf("  XOR6=%d, XOR8=%d, max=%d\n\n",
               r8_jones.xor6, r8_jones.xor8, r8_jones.max_xor);

        printf("  zeta_8 comparison:\n");
        printf("  +-----------+----------+--------+\n");
        printf("  | Level     | Bracket  | Jones  |\n");
        printf("  +-----------+----------+--------+\n");
        printf("  | XOR6      | %5d    | %5d  |\n",
               r8_bracket.xor6, r8_jones.xor6);
        printf("  | XOR8      | %5d    | %5d  |\n",
               r8_bracket.xor8, r8_jones.xor8);
        printf("  +-----------+----------+--------+\n\n");

        if (r8_jones.max_xor < r8_bracket.max_xor) {
            check("zeta_8: Jones loses capacity", 1);
        } else {
            check("zeta_8: Jones retains capacity", 1);
        }
    }

    /* ============================================================ */
    /* Final results                                                */
    /* ============================================================ */
    printf("\n====================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
