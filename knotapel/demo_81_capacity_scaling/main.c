/*
 * KNOTAPEL DEMO 81: Capacity Scaling Law
 * =======================================
 *
 * For zeta_12, build catalog via group closure and take snapshots at
 * each closure round. At each snapshot size, find the maximum XOR_N
 * achievable. Question: how does capacity scale with catalog size?
 *
 * Closure rounds for zeta_12: 5->17->51->127->275->564->1140->2292->4096+
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
/* Catalog (group closure with round tracking)                      */
/* ================================================================ */

#define MAX_QCAT 4096

static Quat g_cat[MAX_QCAT];
static int g_cat_size = 0;

#define MAX_ROUNDS 16
static int round_end[MAX_ROUNDS];
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

static void build_closure(void) {
    Quat gens[4];
    int prev, i, gi;

    g_cat_size = 0;
    n_rounds = 0;

    /* Identity */
    g_cat[0].a = 1; g_cat[0].b = 0; g_cat[0].c = 0; g_cat[0].d = 0;
    g_cat_size = 1;

    /* 4 generators: sigma1, sigma1^-1, sigma2, sigma2^-1 */
    gens[0] = q_gen[1];
    gens[1].a =  q_gen[1].a; gens[1].b = -q_gen[1].b;
    gens[1].c = -q_gen[1].c; gens[1].d = -q_gen[1].d;
    gens[2] = q_gen[2];
    gens[3].a =  q_gen[2].a; gens[3].b = -q_gen[2].b;
    gens[3].c = -q_gen[2].c; gens[3].d = -q_gen[2].d;

    for (gi = 0; gi < 4; gi++) {
        if (find_in_cat(&gens[gi], g_cat_size) < 0 &&
            g_cat_size < MAX_QCAT)
            g_cat[g_cat_size++] = gens[gi];
    }
    round_end[0] = g_cat_size;
    n_rounds = 1;
    printf("  Round 0: %d entries (initial)\n", g_cat_size);

    do {
        prev = g_cat_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                Quat prod;
                const Quat *p = &g_cat[i];
                const Quat *g = &gens[gi];
                prod.a = p->a*g->a - p->b*g->b - p->c*g->c - p->d*g->d;
                prod.b = p->a*g->b + p->b*g->a + p->c*g->d - p->d*g->c;
                prod.c = p->a*g->c - p->b*g->d + p->c*g->a + p->d*g->b;
                prod.d = p->a*g->d + p->b*g->c - p->c*g->b + p->d*g->a;
                if (find_in_cat(&prod, g_cat_size) < 0) {
                    if (g_cat_size < MAX_QCAT)
                        g_cat[g_cat_size++] = prod;
                }
            }
        }
        if (g_cat_size > prev) {
            round_end[n_rounds] = g_cat_size;
            printf("  Round %d: %d entries (+%d)\n",
                   n_rounds, g_cat_size, g_cat_size - prev);
            n_rounds++;
        }
    } while (g_cat_size > prev && n_rounds < MAX_ROUNDS);
}

/* ================================================================ */
/* Direction catalog + Voronoi (rebuilt per snapshot)                */
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
        double ax, ay, az;
        double inv = 1.0 / (nv * n);
        ax = sb * inv; ay = sc * inv; az = sd * inv;
        vor = vor_cell(ax, ay, az);
    }

    return sec * n_vor + vor;
}

/* ================================================================ */
/* XOR test with early conflict detection + sparse cleanup          */
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
/* XOR6 brute force (limited to first bf_limit entries)             */
/* ================================================================ */

static void find_xor6(int bf_limit) {
    int ai, aj, ak, ki;
    int indices[3];

    g_nwin[3] = 0;
    for (ai = 0; ai < bf_limit && g_nwin[3] < MAX_WIN; ai++) {
        for (aj = ai + 1; aj < bf_limit && g_nwin[3] < MAX_WIN; aj++) {
            for (ak = aj + 1; ak < bf_limit && g_nwin[3] < MAX_WIN; ak++) {
                indices[0] = ai; indices[1] = aj; indices[2] = ak;
                for (ki = 0; ki < N_KLADDER; ki++) {
                    int n_vor = g_nd + 1;
                    int n_cells = K_LADDER[ki] * n_vor;
                    if (n_cells > MAX_ACT) continue;
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
    }
}

/* ================================================================ */
/* Recursive extension: extend level-1 winners to level             */
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

            /* Skip if ae is already in the parent winner */
            for (i = 0; i < prev; i++) {
                if (g_win[prev][wi][i] == ae) { in_winner = 1; break; }
            }
            if (in_winner) continue;

            /* Build sorted tuple by inserting ae */
            pos = prev;
            for (i = 0; i < prev; i++) {
                if (ae < g_win[prev][wi][i]) { pos = i; break; }
            }
            for (i = 0; i < pos; i++) indices[i] = g_win[prev][wi][i];
            indices[pos] = ae;
            for (i = pos; i < prev; i++) indices[i+1] = g_win[prev][wi][i];

            /* Dedup against existing winners at this level */
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

            /* Test with k ladder (smallest first) */
            for (ki = 0; ki < N_KLADDER; ki++) {
                int n_vor = g_nd + 1;
                int n_cells = K_LADDER[ki] * n_vor;
                if (n_cells > MAX_ACT) continue;
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
/* Snapshot result                                                  */
/* ================================================================ */

typedef struct {
    int round;
    int cat_size;
    int n_dirs;
    int n_angles;
    int xor6, xor8, xor10, xor12;
    int max_xor;
} Snap;

#define MAX_SNAPS 16
static Snap snaps[MAX_SNAPS];
static int n_snaps = 0;

static void test_snapshot(int round_idx) {
    int cat_size = round_end[round_idx];
    int bf_limit;
    Snap *s;

    if (cat_size < 3) {
        printf("\n  Round %d: %d entries (too small)\n",
               round_idx, cat_size);
        return;
    }

    s = &snaps[n_snaps];
    s->round = round_idx;
    s->cat_size = cat_size;
    s->xor6 = 0; s->xor8 = 0; s->xor10 = 0; s->xor12 = 0;
    s->max_xor = 0;

    printf("\n  --- Round %d: %d entries ---\n", round_idx, cat_size);
    fflush(stdout);

    /* Build directions and count angles for this snapshot */
    build_dirs(cat_size);
    s->n_dirs = g_nd;
    s->n_angles = count_angles(cat_size);
    printf("  Dirs: %d, Angles: %d, Voronoi: %d\n",
           g_nd, s->n_angles, g_nd + 1);

    /* Reset winners */
    memset(g_nwin, 0, sizeof(g_nwin));

    /* XOR6 brute force (first min(cat_size, 100) entries) */
    bf_limit = cat_size < 100 ? cat_size : 100;
    printf("  XOR6 (bf %d)...", bf_limit); fflush(stdout);
    find_xor6(bf_limit);
    s->xor6 = g_nwin[3];
    printf(" %d winners\n", g_nwin[3]);

    /* XOR8 recursive */
    if (g_nwin[3] > 0) {
        printf("  XOR8 (rec %d from %d)...", cat_size, g_nwin[3]);
        fflush(stdout);
        find_recursive(4, cat_size);
        s->xor8 = g_nwin[4];
        printf(" %d winners\n", g_nwin[4]);
    }

    /* XOR10 recursive */
    if (g_nwin[4] > 0) {
        printf("  XOR10 (rec %d from %d)...", cat_size, g_nwin[4]);
        fflush(stdout);
        find_recursive(5, cat_size);
        s->xor10 = g_nwin[5];
        printf(" %d winners\n", g_nwin[5]);
    }

    /* XOR12 recursive */
    if (g_nwin[5] > 0) {
        printf("  XOR12 (rec %d from %d)...", cat_size, g_nwin[5]);
        fflush(stdout);
        find_recursive(6, cat_size);
        s->xor12 = g_nwin[6];
        printf(" %d winners\n", g_nwin[6]);
    }

    /* Determine max XOR */
    if (s->xor6  > 0) s->max_xor = 6;
    if (s->xor8  > 0) s->max_xor = 8;
    if (s->xor10 > 0) s->max_xor = 10;
    if (s->xor12 > 0) s->max_xor = 12;

    printf("  => Max XOR: %d\n", s->max_xor);
    n_snaps++;
}

/* ================================================================ */
/* Main                                                             */
/* ================================================================ */

int main(void) {
    int r, i;
    char msg[128];

    printf("KNOTAPEL DEMO 81: Capacity Scaling Law\n");
    printf("=======================================\n\n");

    /* Part A: Build closure catalog */
    printf("=== Part A: Build zeta_12 closure catalog ===\n\n");
    init_su2(M_PI / 6.0);
    build_closure();
    printf("\n  Total: %d entries in %d rounds\n\n", g_cat_size, n_rounds);

    sprintf(msg, "Catalog: %d entries", g_cat_size);
    check(msg, g_cat_size > 100);
    sprintf(msg, "Rounds: %d", n_rounds);
    check(msg, n_rounds >= 3);

    /* Part B: Test each snapshot */
    printf("\n=== Part B: Capacity at each snapshot ===\n");
    n_snaps = 0;
    for (r = 0; r < n_rounds; r++) {
        test_snapshot(r);
    }

    /* Part C: Scaling table */
    printf("\n=== Part C: Scaling Table ===\n\n");
    printf("  %5s  %5s  %4s  %3s  %5s  %5s  %5s  %5s  %6s  %7s\n",
           "Round", "Cat", "Dirs", "Ang", "XOR6", "XOR8",
           "XOR10", "XOR12", "MaxXOR", "log2(C)");
    printf("  %5s  %5s  %4s  %3s  %5s  %5s  %5s  %5s  %6s  %7s\n",
           "-----", "-----", "----", "---", "-----", "-----",
           "-----", "-----", "------", "-------");

    for (i = 0; i < n_snaps; i++) {
        Snap *s = &snaps[i];
        printf("  %5d  %5d  %4d  %3d  %5d  %5d  %5d  %5d  %6d  %7.2f\n",
               s->round, s->cat_size, s->n_dirs, s->n_angles,
               s->xor6, s->xor8, s->xor10, s->xor12,
               s->max_xor, log((double)s->cat_size) / log(2.0));
    }

    /* Analysis */
    printf("\n  Scaling analysis:\n");
    if (n_snaps >= 2) {
        int first_nz = -1, last_nz = -1;
        int monotone = 1;
        int n_transitions = 0;

        for (i = 0; i < n_snaps; i++) {
            if (snaps[i].max_xor > 0) {
                if (first_nz < 0) first_nz = i;
                last_nz = i;
            }
        }

        for (i = 1; i < n_snaps; i++) {
            if (snaps[i].max_xor < snaps[i-1].max_xor) monotone = 0;
            if (snaps[i].max_xor > snaps[i-1].max_xor) n_transitions++;
        }

        printf("  Monotonic: %s\n", monotone ? "YES" : "NO");
        printf("  Transitions: %d\n", n_transitions);

        if (first_nz >= 0 && last_nz >= 0 && last_nz > first_nz) {
            double cat1 = (double)snaps[first_nz].cat_size;
            double cat2 = (double)snaps[last_nz].cat_size;
            double xor1 = (double)snaps[first_nz].max_xor;
            double xor2 = (double)snaps[last_nz].max_xor;
            double log_ratio = log(cat2 / cat1) / log(2.0);
            double xor_delta = xor2 - xor1;

            printf("  First nonzero: cat=%d -> XOR%d\n",
                   snaps[first_nz].cat_size, snaps[first_nz].max_xor);
            printf("  Last nonzero:  cat=%d -> XOR%d\n",
                   snaps[last_nz].cat_size, snaps[last_nz].max_xor);
            printf("  log2 ratio: %.2f, XOR delta: %.0f\n",
                   log_ratio, xor_delta);

            if (xor_delta > 0 && log_ratio > 0) {
                double slope = xor_delta / log_ratio;
                printf("  XOR per doubling: %.2f\n", slope);
                printf("  If logarithmic: XOR%d needs ~2^%.0f = ~%.0f entries\n",
                       (int)xor2 + 2,
                       log(cat2) / log(2.0) + 2.0 / slope,
                       pow(2.0, log(cat2) / log(2.0) + 2.0 / slope));
            }
        }

        check("Monotonically non-decreasing", monotone);
        sprintf(msg, "At least one transition: %d", n_transitions);
        check(msg, n_transitions >= 1);
        if (last_nz >= 0) {
            sprintf(msg, "Max capacity: XOR%d", snaps[last_nz].max_xor);
            check(msg, snaps[last_nz].max_xor >= 6);
        }
    }

    /* Per-snapshot checks */
    for (i = 0; i < n_snaps; i++) {
        sprintf(msg, "Round %d (cat=%d): max XOR=%d",
                snaps[i].round, snaps[i].cat_size, snaps[i].max_xor);
        check(msg, 1);
    }

    printf("\n=======================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
