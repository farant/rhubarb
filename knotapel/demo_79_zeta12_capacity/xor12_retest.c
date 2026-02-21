/*
 * xor12_retest.c — Targeted XOR12 test with expanded catalog
 *
 * Strategy:
 * 1. Build small (256-entry) catalog, find XOR10 winners (reproduce Demo 79)
 * 2. Build large catalog via group closure (up to 4096 entries)
 * 3. Try extending each XOR10 winner with entries from the LARGE catalog
 * 4. Report whether any XOR12 winner is found
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
 * Braid generators
 * ================================================================ */

static Quat q_gen[3];

static void init_su2_generators(double half_angle) {
    double c = cos(half_angle);
    double s = sin(half_angle);
    q_gen[1] = quat_make(c, s, 0, 0);
    q_gen[2] = quat_make(c, 0, 0, -s);
}

/* ================================================================
 * Small catalog (word enumeration, matches Demo 79 exactly)
 * ================================================================ */

#define SMALL_CAT_MAX 256
static Quat small_cat[SMALL_CAT_MAX];
static int small_cat_size = 0;

static int find_in(Quat q, const Quat *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (fabs(arr[i].a-q.a)<1e-10 && fabs(arr[i].b-q.b)<1e-10 &&
            fabs(arr[i].c-q.c)<1e-10 && fabs(arr[i].d-q.d)<1e-10)
            return i;
        if (fabs(arr[i].a+q.a)<1e-10 && fabs(arr[i].b+q.b)<1e-10 &&
            fabs(arr[i].c+q.c)<1e-10 && fabs(arr[i].d+q.d)<1e-10)
            return i;
    }
    return -1;
}

static void build_small_catalog(void) {
    int n, len;
    small_cat_size = 0;
    for (n = 2; n <= 3; n++) {
        for (len = 1; len <= 12; len++) {
            int max_gen = n - 1;
            int total_gens = 2 * max_gen;
            unsigned long total, idx_l;
            int i;
            total = 1;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)total_gens;
                if (total > 300000) break;
            }
            if (total > 300000) continue;
            for (idx_l = 0; idx_l < total; idx_l++) {
                unsigned long tmp = idx_l;
                Quat result = quat_one();
                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    int idx;
                    Quat gen;
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen) idx = g + 1;
                    else             idx = -(g - max_gen + 1);
                    gen = idx > 0 ? q_gen[idx] : quat_conj(q_gen[-idx]);
                    result = quat_mul(result, gen);
                }
                if (find_in(result, small_cat, small_cat_size) < 0 &&
                    small_cat_size < SMALL_CAT_MAX) {
                    small_cat[small_cat_size++] = result;
                }
            }
        }
    }
}

/* ================================================================
 * Large catalog (group closure)
 * ================================================================ */

#define BIG_CAT_MAX 4096
static Quat big_cat[BIG_CAT_MAX];
static int big_cat_size = 0;

static void build_big_catalog(void) {
    int prev_size, i;
    int round = 0;

    big_cat_size = 0;
    big_cat[big_cat_size++] = quat_one();

    /* Seed with generators */
    {
        Quat gens[4];
        int ng = 0;
        gens[ng++] = q_gen[1];
        gens[ng++] = quat_conj(q_gen[1]);
        gens[ng++] = q_gen[2];
        gens[ng++] = quat_conj(q_gen[2]);
        for (i = 0; i < ng; i++) {
            if (find_in(gens[i], big_cat, big_cat_size) < 0 &&
                big_cat_size < BIG_CAT_MAX)
                big_cat[big_cat_size++] = gens[i];
        }
    }

    do {
        prev_size = big_cat_size;
        round++;
        for (i = 0; i < prev_size; i++) {
            Quat gens[4];
            int ng = 0, gi;
            gens[ng++] = q_gen[1];
            gens[ng++] = quat_conj(q_gen[1]);
            gens[ng++] = q_gen[2];
            gens[ng++] = quat_conj(q_gen[2]);
            for (gi = 0; gi < ng; gi++) {
                Quat prod = quat_mul(big_cat[i], gens[gi]);
                if (find_in(prod, big_cat, big_cat_size) < 0) {
                    if (big_cat_size < BIG_CAT_MAX)
                        big_cat[big_cat_size++] = prod;
                    else
                        return;
                }
            }
        }
        printf("  Closure round %d: %d -> %d\n", round, prev_size,
               big_cat_size);
    } while (big_cat_size > prev_size);
}

/* ================================================================
 * Direction catalog + S2 Voronoi (uses big catalog)
 * ================================================================ */

#define MAX_DIR 512
static double g_dir[MAX_DIR][3];
static int g_nd = 0;

static void build_dir_catalog(const Quat *cat, int cat_size) {
    int i, j;
    g_nd = 0;
    for (i = 0; i < cat_size; i++) {
        Quat q = cat[i];
        double norm_v, ax, ay, az;
        int found = 0;
        if (q.a < 0) { q.a = -q.a; q.b = -q.b; q.c = -q.c; q.d = -q.d; }
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
    if (n < 1e-12) return g_nd;
    qa = sum.a / n; qb = sum.b / n; qc = sum.c / n; qd = sum.d / n;
    if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
    if (qa > 1.0) qa = 1.0;
    norm_v = sqrt(qb*qb + qc*qc + qd*qd);
    if (norm_v < 1e-12) return g_nd;
    ax = qb / norm_v; ay = qc / norm_v; az = qd / norm_v;
    return vor_cell(ax, ay, az);
}

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
 * XOR checker
 * ================================================================ */

#define MAX_ACT_CELLS 8192

static int check_xor(const int *cells, const int *parity,
                     int n_masks, int n_cells) {
    static int cell_even[MAX_ACT_CELLS], cell_odd[MAX_ACT_CELLS];
    int i;
    memset(cell_even, 0, (size_t)n_cells * sizeof(int));
    memset(cell_odd, 0, (size_t)n_cells * sizeof(int));
    for (i = 0; i < n_masks; i++) {
        int c = cells[i];
        if (parity[i] == 0) cell_even[c]++;
        else                  cell_odd[c]++;
    }
    for (i = 0; i < n_cells; i++) {
        if (cell_even[i] > 0 && cell_odd[i] > 0) return 0;
    }
    return 1;
}

/* ================================================================
 * Test combined activation with a catalog (indices into cat)
 * ================================================================ */

#define MAX_MASKS 16384

static int test_combined_cat(const Quat *cat, const int *indices,
                             int n_idx, int k_sec) {
    static int par_buf[MAX_MASKS], cell_buf[MAX_MASKS];
    int n_inputs = 2 * n_idx;
    int n_masks, n_cells, n_vor;
    int mask, i;
    Quat weights[14];

    n_masks = 1 << n_inputs;
    n_vor = g_nd + 1;
    n_cells = k_sec * n_vor;

    if (n_masks > MAX_MASKS || n_cells > MAX_ACT_CELLS) return 0;

    for (i = 0; i < n_idx; i++) {
        weights[2*i]     = cat[indices[i]];
        weights[2*i + 1] = quat_neg(cat[indices[i]]);
    }

    for (mask = 0; mask < n_masks; mask++) {
        Quat sum = quat_make(0, 0, 0, 0);
        int par = 0;
        double ang;
        int sec, vor;

        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                sum = quat_add(sum, weights[i]);
                par ^= 1;
            }
        }
        par_buf[mask] = par;

        ang = rotation_angle(sum);
        sec = (int)(ang * (double)k_sec / 360.0);
        if (sec >= k_sec) sec = k_sec - 1;
        if (sec < 0) sec = 0;
        vor = sum_to_s2_cell(sum);
        cell_buf[mask] = sec * n_vor + vor;
    }

    return check_xor(cell_buf, par_buf, n_masks, n_cells);
}

/* ================================================================
 * Winner storage for small catalog (XOR6 -> XOR8 -> XOR10)
 * ================================================================ */

#define MAX_WIN 1024
#define MAX_TUPLE 7

static int g_win[8][MAX_WIN][MAX_TUPLE];
static int g_win_k[8][MAX_WIN];
static int g_nwin[8];

static const int K_LADDER[] = {1, 6, 8, 10, 12, 16, 20, 24};
#define N_KLADDER 8

/* ================================================================
 * Reproduce XOR6/8/10 from small catalog (needed for extension)
 * ================================================================ */

static void find_xor6(void) {
    int ai, aj, ak, ki;
    int indices[3];
    g_nwin[3] = 0;
    for (ai = 0; ai < small_cat_size; ai++)
      for (aj = ai + 1; aj < small_cat_size; aj++)
        for (ak = aj + 1; ak < small_cat_size; ak++) {
          indices[0] = ai; indices[1] = aj; indices[2] = ak;
          for (ki = 0; ki < N_KLADDER; ki++) {
            if (test_combined_cat(small_cat, indices, 3, K_LADDER[ki])) {
              if (g_nwin[3] < MAX_WIN) {
                g_win[3][g_nwin[3]][0] = ai;
                g_win[3][g_nwin[3]][1] = aj;
                g_win[3][g_nwin[3]][2] = ak;
                g_win_k[3][g_nwin[3]] = K_LADDER[ki];
                g_nwin[3]++;
              }
              break;
            }
          }
        }
}

static void find_recursive_small(int level) {
    int prev = level - 1;
    int wi, ae, ki, i, j;
    g_nwin[level] = 0;
    if (g_nwin[prev] == 0) return;

    for (wi = 0; wi < g_nwin[prev]; wi++) {
        for (ae = 0; ae < small_cat_size; ae++) {
            int in_winner = 0;
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
            for (i = pos; i < prev; i++) indices[i + 1] = g_win[prev][wi][i];

            {
                int dup = 0;
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
            }

            for (ki = 0; ki < N_KLADDER; ki++) {
                if (test_combined_cat(small_cat, indices, level,
                                      K_LADDER[ki])) {
                    if (g_nwin[level] < MAX_WIN) {
                        memcpy(g_win[level][g_nwin[level]],
                               indices, (size_t)level * sizeof(int));
                        g_win_k[level][g_nwin[level]] = K_LADDER[ki];
                        g_nwin[level]++;
                    }
                    break;
                }
            }
        }
    }
}

/* ================================================================
 * Main: reproduce XOR10 winners, then extend with big catalog
 * ================================================================ */

int main(void) {
    int wi, ae, ki, i;
    int n_xor12 = 0;
    int n_tested = 0;
    printf("=== XOR12 Retest with Expanded Catalog ===\n\n");

    /* Step 1: Build small catalog + reproduce XOR10 winners */
    printf("--- Step 1: Reproduce XOR10 winners (small catalog) ---\n");
    init_su2_generators(M_PI / 6.0);
    build_small_catalog();
    printf("  Small catalog: %d quaternions\n", small_cat_size);

    /* Build direction catalog from small catalog for XOR6/8/10 */
    build_dir_catalog(small_cat, small_cat_size);
    printf("  Small directions: %d, Voronoi cells: %d\n", g_nd, g_nd + 1);

    memset(g_nwin, 0, sizeof(g_nwin));
    printf("  Finding XOR6...\n");
    find_xor6();
    printf("  XOR6: %d winners\n", g_nwin[3]);

    printf("  Finding XOR8...\n");
    find_recursive_small(4);
    printf("  XOR8: %d winners\n", g_nwin[4]);

    printf("  Finding XOR10...\n");
    find_recursive_small(5);
    printf("  XOR10: %d winners\n\n", g_nwin[5]);

    if (g_nwin[5] == 0) {
        printf("  No XOR10 winners to extend. Aborting.\n");
        return 1;
    }

    /* Step 2: Build big catalog */
    printf("--- Step 2: Build large catalog (group closure) ---\n");
    build_big_catalog();
    printf("  Big catalog: %d quaternions\n\n", big_cat_size);

    /* Step 3: Rebuild direction catalog from big catalog */
    printf("--- Step 3: Rebuild directions from big catalog ---\n");
    build_dir_catalog(big_cat, big_cat_size);
    printf("  Big directions: %d, Voronoi cells: %d\n\n", g_nd, g_nd + 1);

    /* Step 4: Try extending each XOR10 winner with big catalog entries */
    printf("--- Step 4: Extend XOR10 winners to XOR12 ---\n");
    printf("  Testing %d XOR10 winners x %d catalog entries...\n",
           g_nwin[5], big_cat_size);

    for (wi = 0; wi < g_nwin[5]; wi++) {
        /* Map small catalog indices to big catalog */
        Quat winner_quats[5];
        for (i = 0; i < 5; i++)
            winner_quats[i] = small_cat[g_win[5][wi][i]];

        for (ae = 0; ae < big_cat_size; ae++) {
            /* Check not duplicate of any winner member */
            int dup = 0;
            int indices[6];
            for (i = 0; i < 5; i++) {
                Quat diff;
                diff.a = fabs(big_cat[ae].a - winner_quats[i].a);
                diff.b = fabs(big_cat[ae].b - winner_quats[i].b);
                diff.c = fabs(big_cat[ae].c - winner_quats[i].c);
                diff.d = fabs(big_cat[ae].d - winner_quats[i].d);
                if (diff.a < 1e-10 && diff.b < 1e-10 &&
                    diff.c < 1e-10 && diff.d < 1e-10) { dup = 1; break; }
                diff.a = fabs(big_cat[ae].a + winner_quats[i].a);
                diff.b = fabs(big_cat[ae].b + winner_quats[i].b);
                diff.c = fabs(big_cat[ae].c + winner_quats[i].c);
                diff.d = fabs(big_cat[ae].d + winner_quats[i].d);
                if (diff.a < 1e-10 && diff.b < 1e-10 &&
                    diff.c < 1e-10 && diff.d < 1e-10) { dup = 1; break; }
            }
            if (dup) continue;

            /* Build 6-tuple: 5 winner quats + big_cat[ae] */
            /* Use big_cat indices directly */
            for (i = 0; i < 5; i++) {
                int found = find_in(winner_quats[i], big_cat, big_cat_size);
                indices[i] = found;
            }
            indices[5] = ae;

            /* Verify all found */
            {
                int ok = 1;
                for (i = 0; i < 6; i++) {
                    if (indices[i] < 0) { ok = 0; break; }
                }
                if (!ok) continue;
            }

            n_tested++;

            /* Test with k ladder */
            for (ki = 0; ki < N_KLADDER; ki++) {
                int n_vor = g_nd + 1;
                int n_cells = K_LADDER[ki] * n_vor;
                if (n_cells > MAX_ACT_CELLS) continue;
                if (test_combined_cat(big_cat, indices, 6, K_LADDER[ki])) {
                    n_xor12++;
                    printf("  FOUND XOR12 #%d: winner[%d] + big[%d] "
                           "at k=%d (%d cells)\n",
                           n_xor12, wi, ae, K_LADDER[ki], n_cells);
                    if (n_xor12 >= 20) goto done;
                    break;
                }
            }
        }

        if (wi % 20 == 0)
            printf("  ... processed %d/%d XOR10 winners, "
                   "%d XOR12 found so far, %d tested\n",
                   wi, g_nwin[5], n_xor12, n_tested);
    }

done:
    printf("\n=== Results ===\n");
    printf("  XOR10 winners tested: %d\n", g_nwin[5]);
    printf("  Unique candidates tested: %d\n", n_tested);
    printf("  XOR12 winners found: %d\n", n_xor12);

    if (n_xor12 > 0)
        printf("\n  >>> XOR12 IS SOLVABLE with expanded catalog! <<<\n"
               "  >>> The Demo 79 wall was an ARTIFACT of truncation. <<<\n");
    else
        printf("\n  >>> XOR12 still unsolvable from these XOR10 parents. <<<\n"
               "  >>> But this is a lower bound — more XOR10 winners\n"
               "  >>> from the full catalog might extend to XOR12. <<<\n");

    return 0;
}
