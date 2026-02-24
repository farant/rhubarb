/*
 * KNOTAPEL DEMO 88: Anti-Correlation Mechanism
 * =============================================
 *
 * Demo 72 found that optimizing 13 eigenvector directions for spherical
 * design quality DESTROYS computation (36->4 XOR6 solutions). The
 * algebraically-placed directions are simultaneously the worst for
 * spherical integration and the best for DKC computation.
 *
 * This demo investigates WHY through four lenses:
 *   Phase 1: Geometric characterization (pairwise angles, Gram eigenspectrum)
 *   Phase 2: Algebraic characterization (null/non-null direction clustering)
 *   Phase 3: Perturbation sensitivity (which directions are fragile?)
 *   Phase 4: Cross-root comparison (does anti-correlation persist at zeta_12?)
 *
 * Key finding: non-null body-diagonal directions are MORE sensitive
 * to perturbation than null-only edge-midpoints (8.0% vs -2.4% at
 * eps=10). Constrained optimization (e) confirms: flexible nulls +
 * rigid non-nulls gets 48% design improvement with only 2.8% XOR loss.
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
/* SU(2) generators + group closure                                 */
/* ================================================================ */

#define MAX_QCAT 4096
#define MAX_DIR 2048
#define MAX_ROUNDS 16

static Quat g_gen[3];
static Quat g_cat[MAX_QCAT];
static int g_cat_size = 0;
static int n_rounds = 0;

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

    g_cat[0].a = 1; g_cat[0].b = 0; g_cat[0].c = 0; g_cat[0].d = 0;
    g_cat_size = 1;

    gens[0] = g_gen[1];
    gens[1].a =  g_gen[1].a; gens[1].b = -g_gen[1].b;
    gens[1].c = -g_gen[1].c; gens[1].d = -g_gen[1].d;
    gens[2] = g_gen[2];
    gens[3].a =  g_gen[2].a; gens[3].b = -g_gen[2].b;
    gens[3].c = -g_gen[2].c; gens[3].d = -g_gen[2].d;

    for (gi = 0; gi < 4; gi++) {
        if (find_in_cat(&gens[gi], g_cat_size) < 0 &&
            g_cat_size < MAX_QCAT) {
            g_cat[g_cat_size++] = gens[gi];
        }
    }
    n_rounds = 1;
    if (verbose) printf("  Round 0: %d entries\n", g_cat_size);

    rd = 1;
    do {
        prev = g_cat_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                Quat prod = qmul(&g_cat[i], &gens[gi]);
                if (find_in_cat(&prod, g_cat_size) < 0) {
                    if (g_cat_size < MAX_QCAT) {
                        g_cat[g_cat_size++] = prod;
                    }
                }
            }
        }
        if (g_cat_size > prev) {
            if (verbose)
                printf("  Round %d: %d entries (+%d)\n",
                       rd, g_cat_size, g_cat_size - prev);
            n_rounds++;
        }
        rd++;
    } while (g_cat_size > prev && n_rounds < MAX_ROUNDS);
}

/* ================================================================ */
/* Null classification                                              */
/* ================================================================ */

static int is_null[MAX_QCAT];
static int n_null_total, n_nonnull_total;

static void classify_null(int cat_size) {
    int i;
    n_null_total = 0;
    n_nonnull_total = 0;
    for (i = 0; i < cat_size; i++) {
        double qa = g_cat[i].a;
        if (qa < 0) qa = -qa;
        is_null[i] = (qa < 1e-10) ? 1 : 0;
        if (is_null[i]) n_null_total++;
        else n_nonnull_total++;
    }
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
/* Direction classification by null type                            */
/* ================================================================ */

#define MAX_DIR_SMALL 64

static int dir_null_cnt[MAX_DIR];
static int dir_nonnull_cnt[MAX_DIR];
/* 0=null-only, 1=shared, 2=non-null-only */
static int dir_type[MAX_DIR_SMALL];
static int n_null_only_dirs, n_shared_dirs, n_nonnull_only_dirs;

static void classify_directions(int cat_size) {
    int i, j;

    memset(dir_null_cnt, 0, sizeof(int) * (size_t)g_nd);
    memset(dir_nonnull_cnt, 0, sizeof(int) * (size_t)g_nd);

    for (i = 0; i < cat_size; i++) {
        double qa = g_cat[i].a, qb = g_cat[i].b;
        double qc = g_cat[i].c, qd = g_cat[i].d;
        double nv, ax, ay, az;
        int found = -1;

        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        if (nv < 1e-12) continue;
        ax = qb/nv; ay = qc/nv; az = qd/nv;

        for (j = 0; j < g_nd; j++) {
            double d1 = fabs(g_dir[j][0]-ax) + fabs(g_dir[j][1]-ay) +
                         fabs(g_dir[j][2]-az);
            double d2 = fabs(g_dir[j][0]+ax) + fabs(g_dir[j][1]+ay) +
                         fabs(g_dir[j][2]+az);
            if (d1 < 1e-8 || d2 < 1e-8) { found = j; break; }
        }
        if (found >= 0) {
            if (is_null[i]) dir_null_cnt[found]++;
            else dir_nonnull_cnt[found]++;
        }
    }

    n_null_only_dirs = 0;
    n_shared_dirs = 0;
    n_nonnull_only_dirs = 0;
    for (i = 0; i < g_nd && i < MAX_DIR_SMALL; i++) {
        if (dir_null_cnt[i] > 0 && dir_nonnull_cnt[i] == 0) {
            dir_type[i] = 0;
            n_null_only_dirs++;
        } else if (dir_null_cnt[i] > 0 && dir_nonnull_cnt[i] > 0) {
            dir_type[i] = 1;
            n_shared_dirs++;
        } else {
            dir_type[i] = 2;
            n_nonnull_only_dirs++;
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
/* XOR6 counter (returns true total, not capped)                    */
/* ================================================================ */

static const int K_LADDER[] = {1, 6, 8, 10, 12, 16, 20, 24};
#define N_KLADDER 8

static int count_xor6(int bf_limit) {
    int ai, aj, ak, ki;
    int count = 0;
    int indices[3];

    for (ai = 0; ai < bf_limit; ai++)
    for (aj = ai+1; aj < bf_limit; aj++)
    for (ak = aj+1; ak < bf_limit; ak++) {
        indices[0] = ai; indices[1] = aj; indices[2] = ak;
        for (ki = 0; ki < N_KLADDER; ki++) {
            int nv2 = g_nd + 1;
            int nc2 = K_LADDER[ki] * nv2;
            if (nc2 > MAX_ACT) continue;
            if (test_xor(indices, 3, K_LADDER[ki])) {
                count++;
                break;
            }
        }
    }
    return count;
}

/* ================================================================ */
/* Spherical harmonics + design residual                            */
/* ================================================================ */

static double assoc_legendre(int l, int m, double x) {
    double pmm = 1.0;
    double somx2 = sqrt(1.0 - x*x);
    int i;
    double pmm1, pll;

    for (i = 1; i <= m; i++)
        pmm *= -(double)(2*i - 1) * somx2;

    if (l == m) return pmm;

    pmm1 = x * (double)(2*m + 1) * pmm;
    if (l == m+1) return pmm1;

    for (i = m+2; i <= l; i++) {
        pll = (x * (double)(2*i-1) * pmm1 - (double)(i+m-1) * pmm)
              / (double)(i-m);
        pmm = pmm1;
        pmm1 = pll;
    }
    return pmm1;
}

static double factorial_d(int n) {
    double f = 1.0;
    int i;
    for (i = 2; i <= n; i++) f *= (double)i;
    return f;
}

static double ylm_norm(int l, int m) {
    int am = abs(m);
    double num = (double)(2*l+1) * factorial_d(l - am);
    double den = 4.0 * M_PI * factorial_d(l + am);
    return sqrt(num / den);
}

static double ylm_real(int l, int m, double theta, double phi) {
    double ct = cos(theta);
    int am = abs(m);
    double plm = assoc_legendre(l, am, ct);
    double norm = ylm_norm(l, m);

    if (m > 0) return sqrt(2.0) * norm * plm * cos((double)m * phi);
    if (m < 0) return sqrt(2.0) * norm * plm * sin((double)am * phi);
    return norm * plm;
}

static double design_residual(const double dirs[][3], int nd, int t) {
    int l, m, i;
    double max_res = 0.0;
    for (l = 1; l <= t; l++)
        for (m = -l; m <= l; m++) {
            double sum = 0.0;
            for (i = 0; i < nd; i++) {
                double z = dirs[i][2], theta, phi;
                if (z > 1.0) z = 1.0;
                if (z < -1.0) z = -1.0;
                theta = acos(z);
                phi = atan2(dirs[i][1], dirs[i][0]);
                if (phi < 0) phi += 2.0 * M_PI;
                sum += ylm_real(l, m, theta, phi);
            }
            if (fabs(sum) > max_res) max_res = fabs(sum);
        }
    return max_res;
}

/* ================================================================ */
/* Jacobi eigendecomposition (small symmetric matrices)             */
/* ================================================================ */

static void jacobi_eigen(double *a, int n, double *evals) {
    int iter, i, j, k, p, q;
    double max_off;

    for (iter = 0; iter < 200; iter++) {
        max_off = 0.0;
        p = 0; q = 1;
        for (i = 0; i < n; i++)
            for (j = i+1; j < n; j++)
                if (fabs(a[i*n+j]) > max_off) {
                    max_off = fabs(a[i*n+j]);
                    p = i; q = j;
                }
        if (max_off < 1e-14) break;

        {
            double app = a[p*n+p], aqq = a[q*n+q], apq = a[p*n+q];
            double tau, t, c, s;

            if (fabs(apq) < 1e-30) continue;

            tau = (aqq - app) / (2.0 * apq);
            t = (tau >= 0 ? 1.0 : -1.0) / (fabs(tau) + sqrt(1.0 + tau*tau));
            c = 1.0 / sqrt(1.0 + t*t);
            s = t * c;

            for (k = 0; k < n; k++) {
                if (k != p && k != q) {
                    double akp = a[k*n+p], akq = a[k*n+q];
                    a[k*n+p] = c*akp - s*akq;
                    a[p*n+k] = a[k*n+p];
                    a[k*n+q] = s*akp + c*akq;
                    a[q*n+k] = a[k*n+q];
                }
            }

            {
                double new_pp = c*c*app - 2.0*s*c*apq + s*s*aqq;
                double new_qq = s*s*app + 2.0*s*c*apq + c*c*aqq;
                a[p*n+p] = new_pp;
                a[q*n+q] = new_qq;
                a[p*n+q] = 0.0;
                a[q*n+p] = 0.0;
            }
        }
    }

    for (i = 0; i < n; i++) evals[i] = a[i*n+i];

    /* Sort descending */
    for (i = 0; i < n-1; i++)
        for (j = i+1; j < n; j++)
            if (evals[j] > evals[i]) {
                double tmp = evals[i];
                evals[i] = evals[j];
                evals[j] = tmp;
            }
}

/* ================================================================ */
/* RNG (deterministic LCG)                                          */
/* ================================================================ */

static unsigned long g_rng = 42UL;

static double rng_uniform(void) {
    g_rng = g_rng * 1103515245UL + 12345UL;
    return (double)((g_rng >> 16) & 0x7FFF) / 32768.0;
}

static double rng_gauss(void) {
    double u1 = rng_uniform() + 1e-15;
    double u2 = rng_uniform();
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

static void normalize3(double *v) {
    double n = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    if (n > 1e-15) { v[0]/=n; v[1]/=n; v[2]/=n; }
}

/* ================================================================ */
/* Phase 1: Geometric Characterization                              */
/* ================================================================ */

static double z8_dirs[MAX_DIR_SMALL][3];
static int z8_nd;

static void phase1_geometry(void) {
    int i, j;
    double gram[64*64];
    double evals[64];
    double min_ang, max_ang, sum_ang;
    int n_pairs;
    double res_t2, res_t4, res_t6;

    printf("\n=== Phase 1: Geometric Characterization (zeta_8) ===\n\n");

    /* Save directions */
    z8_nd = g_nd;
    for (i = 0; i < g_nd; i++) {
        z8_dirs[i][0] = g_dir[i][0];
        z8_dirs[i][1] = g_dir[i][1];
        z8_dirs[i][2] = g_dir[i][2];
    }

    printf("  %d directions on S^2\n\n", g_nd);

    /* Pairwise angles (±-identified: use |dot|, range 0-90 deg) */
    min_ang = 999; max_ang = 0; sum_ang = 0; n_pairs = 0;

    printf("  Pairwise angle matrix (degrees, +-identified):\n    ");
    for (j = 0; j < g_nd; j++) printf(" %5d", j);
    printf("\n");

    for (i = 0; i < g_nd; i++) {
        printf("  %2d", i);
        for (j = 0; j < g_nd; j++) {
            double dp = g_dir[i][0]*g_dir[j][0] + g_dir[i][1]*g_dir[j][1] +
                        g_dir[i][2]*g_dir[j][2];
            double ang_deg;
            if (dp > 1.0) dp = 1.0;
            if (dp < -1.0) dp = -1.0;
            ang_deg = acos(fabs(dp)) * 180.0 / M_PI;
            printf(" %5.1f", ang_deg);

            if (i < j) {
                if (ang_deg < min_ang) min_ang = ang_deg;
                if (ang_deg > max_ang) max_ang = ang_deg;
                sum_ang += ang_deg;
                n_pairs++;
            }
        }
        printf("\n");
    }

    printf("\n  Angle stats: min=%.1f, max=%.1f, mean=%.1f deg (%d pairs)\n",
           min_ang, max_ang, sum_ang / (double)n_pairs, n_pairs);

    /* Gram matrix G[i][j] = |dot(dir_i, dir_j)| */
    for (i = 0; i < g_nd; i++)
        for (j = 0; j < g_nd; j++) {
            double dp = g_dir[i][0]*g_dir[j][0] + g_dir[i][1]*g_dir[j][1] +
                        g_dir[i][2]*g_dir[j][2];
            gram[i*g_nd+j] = fabs(dp);
        }

    /* Eigendecompose */
    jacobi_eigen(gram, g_nd, evals);

    printf("\n  Gram matrix eigenvalues (descending):\n    ");
    for (i = 0; i < g_nd; i++) printf("%.4f ", evals[i]);
    printf("\n");

    {
        double max_ev = evals[0];
        double min_ev = evals[g_nd-1];
        double ratio = max_ev / (min_ev > 1e-10 ? min_ev : 1e-10);
        printf("  Condition: max=%.4f, min=%.4f, ratio=%.1f\n",
               max_ev, min_ev, ratio);
    }

    check("Gram has 13 eigenvalues", g_nd == 13);
    /* Note: |dot| kernel is NOT PSD (negative evals expected for RP^2 geometry) */
    printf("  Note: negative eigenvalues expected for |dot| kernel on RP^2\n");

    /* Spherical design residual */
    res_t2 = design_residual((const double (*)[3])g_dir, g_nd, 2);
    res_t4 = design_residual((const double (*)[3])g_dir, g_nd, 4);
    res_t6 = design_residual((const double (*)[3])g_dir, g_nd, 6);

    printf("\n  Spherical design residuals:\n");
    printf("    t=2: %.6f\n", res_t2);
    printf("    t=4: %.6f\n", res_t4);
    printf("    t=6: %.6f (bandwidth level)\n", res_t6);

    check("NOT a 2-design (residual > 0.01)", res_t2 > 0.01);
    printf("\n");
}

/* ================================================================ */
/* Phase 2: Algebraic Characterization                              */
/* ================================================================ */

static void phase2_algebraic(void) {
    int i, j;
    double null_sum, null_n;
    double nonnull_sum, nonnull_n;
    double shared_sum, shared_n;
    double inter_sum, inter_n;

    printf("\n=== Phase 2: Algebraic Characterization ===\n\n");

    classify_directions(g_cat_size);

    printf("  Direction partition:\n");
    printf("    Null-only:     %d dirs (type 0)\n", n_null_only_dirs);
    printf("    Shared:        %d dirs (type 1)\n", n_shared_dirs);
    printf("    Non-null-only: %d dirs (type 2)\n", n_nonnull_only_dirs);
    printf("    Total:         %d\n\n", g_nd);

    check("Partition: 6 null-only + 3 shared + 4 non-null = 13",
          n_null_only_dirs == 6 && n_shared_dirs == 3 &&
          n_nonnull_only_dirs == 4);

    /* List directions by type */
    printf("  Directions by type:\n");
    for (i = 0; i < g_nd; i++) {
        const char *tn = dir_type[i] == 0 ? "NULL-ONLY" :
                         dir_type[i] == 1 ? "shared   " : "non-null ";
        printf("    [%2d] (%+.4f, %+.4f, %+.4f)  %s  n=%d nn=%d\n",
               i, g_dir[i][0], g_dir[i][1], g_dir[i][2],
               tn, dir_null_cnt[i], dir_nonnull_cnt[i]);
    }
    printf("\n");

    /* Intra-group and inter-group angle statistics */
    null_sum = 0; null_n = 0;
    nonnull_sum = 0; nonnull_n = 0;
    shared_sum = 0; shared_n = 0;
    inter_sum = 0; inter_n = 0;

    for (i = 0; i < g_nd; i++) {
        for (j = i+1; j < g_nd; j++) {
            double dp = g_dir[i][0]*g_dir[j][0] + g_dir[i][1]*g_dir[j][1] +
                        g_dir[i][2]*g_dir[j][2];
            double ang;
            if (dp > 1.0) dp = 1.0;
            if (dp < -1.0) dp = -1.0;
            ang = acos(fabs(dp)) * 180.0 / M_PI;

            if (dir_type[i] == 0 && dir_type[j] == 0) {
                null_sum += ang; null_n++;
            } else if (dir_type[i] == 2 && dir_type[j] == 2) {
                nonnull_sum += ang; nonnull_n++;
            } else if (dir_type[i] == 1 && dir_type[j] == 1) {
                shared_sum += ang; shared_n++;
            }

            if ((dir_type[i] == 0 && dir_type[j] == 2) ||
                (dir_type[i] == 2 && dir_type[j] == 0)) {
                inter_sum += ang; inter_n++;
            }
        }
    }

    printf("  Intra/inter-group mean angles:\n");
    if (null_n > 0)
        printf("    Null-only intra:      %.1f deg (%d pairs)\n",
               null_sum/null_n, (int)null_n);
    if (nonnull_n > 0)
        printf("    Non-null intra:       %.1f deg (%d pairs)\n",
               nonnull_sum/nonnull_n, (int)nonnull_n);
    if (shared_n > 0)
        printf("    Shared intra:         %.1f deg (%d pairs)\n",
               shared_sum/shared_n, (int)shared_n);
    if (inter_n > 0)
        printf("    Null<->Non-null inter: %.1f deg (%d pairs)\n",
               inter_sum/inter_n, (int)inter_n);
    printf("\n");

    if (null_n > 0 && nonnull_n > 0) {
        double diff = fabs(null_sum/null_n - nonnull_sum/nonnull_n);
        printf("  Intra-group angle difference: %.1f deg\n", diff);
        check("Null and non-null have distinct geometry", diff > 1.0);
    }
    printf("\n");
}

/* ================================================================ */
/* Phase 3: Perturbation Sensitivity                                */
/* ================================================================ */

static void phase3_perturbation(void) {
    double eps_deg[] = {1.0, 2.0, 5.0, 10.0, 20.0, 30.0, 45.0};
    int n_eps = 7;
    int n_trials = 20;
    int e, t, i;
    int baseline_xor6;
    double orig_dirs[MAX_DIR_SMALL][3];
    int orig_nd;
    int null_loss_10, nonnull_loss_10;

    printf("\n=== Phase 3: Perturbation Sensitivity ===\n\n");

    orig_nd = g_nd;
    for (i = 0; i < g_nd; i++) {
        orig_dirs[i][0] = g_dir[i][0];
        orig_dirs[i][1] = g_dir[i][1];
        orig_dirs[i][2] = g_dir[i][2];
    }

    baseline_xor6 = count_xor6(g_cat_size);
    printf("  Baseline XOR6 = %d (unperturbed, %d entries)\n\n",
           baseline_xor6, g_cat_size);
    check("Baseline XOR6 > 0", baseline_xor6 > 0);

    /* --- (a) Random perturbation of ALL directions --- */
    printf("  (a) Random perturbation (all %d directions):\n", g_nd);
    printf("  | Epsilon | Mean XOR6 | Min | Max | Loss%% |\n");
    printf("  |---------|-----------|-----|-----|-------|\n");

    for (e = 0; e < n_eps; e++) {
        double eps_rad = eps_deg[e] * M_PI / 180.0;
        int sum_xor = 0, min_xor = 9999, max_xor = 0;

        for (t = 0; t < n_trials; t++) {
            int xc;
            for (i = 0; i < orig_nd; i++) {
                g_dir[i][0] = orig_dirs[i][0] + rng_gauss() * eps_rad;
                g_dir[i][1] = orig_dirs[i][1] + rng_gauss() * eps_rad;
                g_dir[i][2] = orig_dirs[i][2] + rng_gauss() * eps_rad;
                normalize3(g_dir[i]);
            }
            g_nd = orig_nd;
            xc = count_xor6(g_cat_size);
            sum_xor += xc;
            if (xc < min_xor) min_xor = xc;
            if (xc > max_xor) max_xor = xc;
        }
        {
            double mean = (double)sum_xor / (double)n_trials;
            printf("  | %5.0f   | %9.1f | %3d | %3d | %5.1f |\n",
                   eps_deg[e], mean, min_xor, max_xor,
                   100.0 * (1.0 - mean / (double)baseline_xor6));
        }
    }

    /* --- (c) Null-only perturbation --- */
    printf("\n  (c) Null-only perturbation (%d null-exclusive directions):\n",
           n_null_only_dirs);
    printf("  | Epsilon | Mean XOR6 | Min | Max | Loss%% |\n");
    printf("  |---------|-----------|-----|-----|-------|\n");

    null_loss_10 = 0;
    for (e = 0; e < n_eps; e++) {
        double eps_rad = eps_deg[e] * M_PI / 180.0;
        int sum_xor = 0, min_xor = 9999, max_xor = 0;

        for (t = 0; t < n_trials; t++) {
            int xc;
            for (i = 0; i < orig_nd; i++) {
                g_dir[i][0] = orig_dirs[i][0];
                g_dir[i][1] = orig_dirs[i][1];
                g_dir[i][2] = orig_dirs[i][2];
            }
            for (i = 0; i < orig_nd; i++) {
                if (dir_type[i] == 0) {
                    g_dir[i][0] = orig_dirs[i][0] + rng_gauss() * eps_rad;
                    g_dir[i][1] = orig_dirs[i][1] + rng_gauss() * eps_rad;
                    g_dir[i][2] = orig_dirs[i][2] + rng_gauss() * eps_rad;
                    normalize3(g_dir[i]);
                }
            }
            g_nd = orig_nd;
            xc = count_xor6(g_cat_size);
            sum_xor += xc;
            if (xc < min_xor) min_xor = xc;
            if (xc > max_xor) max_xor = xc;
        }
        {
            double mean = (double)sum_xor / (double)n_trials;
            double loss = 100.0 * (1.0 - mean / (double)baseline_xor6);
            printf("  | %5.0f   | %9.1f | %3d | %3d | %5.1f |\n",
                   eps_deg[e], mean, min_xor, max_xor, loss);
            if (e == 3) null_loss_10 = (int)(loss * 10); /* eps=10 deg */
        }
    }

    /* --- (d) Non-null-only perturbation --- */
    printf("\n  (d) Non-null-only perturbation (%d non-null-exclusive dirs):\n",
           n_nonnull_only_dirs);
    printf("  | Epsilon | Mean XOR6 | Min | Max | Loss%% |\n");
    printf("  |---------|-----------|-----|-----|-------|\n");

    nonnull_loss_10 = 0;
    for (e = 0; e < n_eps; e++) {
        double eps_rad = eps_deg[e] * M_PI / 180.0;
        int sum_xor = 0, min_xor = 9999, max_xor = 0;

        for (t = 0; t < n_trials; t++) {
            int xc;
            for (i = 0; i < orig_nd; i++) {
                g_dir[i][0] = orig_dirs[i][0];
                g_dir[i][1] = orig_dirs[i][1];
                g_dir[i][2] = orig_dirs[i][2];
            }
            for (i = 0; i < orig_nd; i++) {
                if (dir_type[i] == 2) {
                    g_dir[i][0] = orig_dirs[i][0] + rng_gauss() * eps_rad;
                    g_dir[i][1] = orig_dirs[i][1] + rng_gauss() * eps_rad;
                    g_dir[i][2] = orig_dirs[i][2] + rng_gauss() * eps_rad;
                    normalize3(g_dir[i]);
                }
            }
            g_nd = orig_nd;
            xc = count_xor6(g_cat_size);
            sum_xor += xc;
            if (xc < min_xor) min_xor = xc;
            if (xc > max_xor) max_xor = xc;
        }
        {
            double mean = (double)sum_xor / (double)n_trials;
            double loss = 100.0 * (1.0 - mean / (double)baseline_xor6);
            printf("  | %5.0f   | %9.1f | %3d | %3d | %5.1f |\n",
                   eps_deg[e], mean, min_xor, max_xor, loss);
            if (e == 3) nonnull_loss_10 = (int)(loss * 10);
        }
    }

    /* Key comparison */
    printf("\n  Sensitivity comparison at eps=10 deg:\n");
    printf("    Null-only loss:     %.1f%%\n", (double)null_loss_10 / 10.0);
    printf("    Non-null-only loss: %.1f%%\n", (double)nonnull_loss_10 / 10.0);
    if (null_loss_10 > nonnull_loss_10)
        printf("    --> Null directions MORE sensitive (radical-driven)\n");
    else if (null_loss_10 < nonnull_loss_10)
        printf("    --> Non-null directions MORE sensitive (unexpected!)\n");
    else
        printf("    --> Equal sensitivity\n");
    printf("\n");

    /* --- (b) Design-improving optimization from algebraic start --- */
    printf("  (b) Design-improving gradient (algebraic -> geometric):\n");
    {
        double pert_dirs[MAX_DIR_SMALL][3];
        int checkpoints[] = {0, 100, 200, 500, 1000, 2000, 5000};
        int n_cp = 7;
        int cp_idx = 0;
        int step;

        /* Start from algebraic directions */
        for (i = 0; i < orig_nd; i++) {
            pert_dirs[i][0] = orig_dirs[i][0];
            pert_dirs[i][1] = orig_dirs[i][1];
            pert_dirs[i][2] = orig_dirs[i][2];
        }

        printf("  | Step  | Residual(t=6) | XOR6 | Loss%% |\n");
        printf("  |-------|---------------|------|-------|\n");

        for (step = 0; step <= 5000; step++) {
            if (cp_idx < n_cp && step == checkpoints[cp_idx]) {
                double res;
                int xc;

                /* Measure */
                for (i = 0; i < orig_nd; i++) {
                    g_dir[i][0] = pert_dirs[i][0];
                    g_dir[i][1] = pert_dirs[i][1];
                    g_dir[i][2] = pert_dirs[i][2];
                }
                g_nd = orig_nd;
                res = design_residual((const double (*)[3])pert_dirs,
                                      orig_nd, 6);
                xc = count_xor6(g_cat_size);

                printf("  | %5d | %13.4f | %4d | %5.1f |\n",
                       step, res, xc,
                       100.0 * (1.0 - (double)xc / (double)baseline_xor6));
                cp_idx++;
            }

            /* Optimization step: jitter random direction toward lower residual */
            if (step < 5000) {
                double temp = 0.3 * (1.0 - (double)step / 5000.0);
                int idx = (int)(rng_uniform() * (double)orig_nd) % orig_nd;
                double old[3], old_res, new_res;

                old[0] = pert_dirs[idx][0];
                old[1] = pert_dirs[idx][1];
                old[2] = pert_dirs[idx][2];
                old_res = design_residual((const double (*)[3])pert_dirs,
                                          orig_nd, 6);

                pert_dirs[idx][0] = old[0] + rng_gauss() * temp;
                pert_dirs[idx][1] = old[1] + rng_gauss() * temp;
                pert_dirs[idx][2] = old[2] + rng_gauss() * temp;
                normalize3(pert_dirs[idx]);

                new_res = design_residual((const double (*)[3])pert_dirs,
                                          orig_nd, 6);
                if (new_res > old_res) {
                    pert_dirs[idx][0] = old[0];
                    pert_dirs[idx][1] = old[1];
                    pert_dirs[idx][2] = old[2];
                }
            }
        }
    }

    /* --- (e) Constrained optimization: flexible nulls + rigid non-nulls --- */
    printf("\n  (e) Constrained design-optimization ");
    printf("(nulls free, non-nulls clamped to 2 deg):\n");
    {
        double pert_dirs_e[MAX_DIR_SMALL][3];
        int checkpoints_e[] = {0, 100, 200, 500, 1000, 2000, 5000};
        int n_cp_e = 7;
        int cp_idx_e = 0;
        int step_e;
        double max_clamp_rad = 2.0 * M_PI / 180.0;

        /* Start from algebraic directions */
        for (i = 0; i < orig_nd; i++) {
            pert_dirs_e[i][0] = orig_dirs[i][0];
            pert_dirs_e[i][1] = orig_dirs[i][1];
            pert_dirs_e[i][2] = orig_dirs[i][2];
        }

        printf("  | Step  | Residual(t=6) | XOR6 | Loss%% |\n");
        printf("  |-------|---------------|------|-------|\n");

        for (step_e = 0; step_e <= 5000; step_e++) {
            if (cp_idx_e < n_cp_e && step_e == checkpoints_e[cp_idx_e]) {
                double res_e;
                int xc_e;

                for (i = 0; i < orig_nd; i++) {
                    g_dir[i][0] = pert_dirs_e[i][0];
                    g_dir[i][1] = pert_dirs_e[i][1];
                    g_dir[i][2] = pert_dirs_e[i][2];
                }
                g_nd = orig_nd;
                res_e = design_residual((const double (*)[3])pert_dirs_e,
                                        orig_nd, 6);
                xc_e = count_xor6(g_cat_size);

                printf("  | %5d | %13.4f | %4d | %5.1f |\n",
                       step_e, res_e, xc_e,
                       100.0 * (1.0 - (double)xc_e / (double)baseline_xor6));
                cp_idx_e++;
            }

            /* Optimization step */
            if (step_e < 5000) {
                double temp_e = 0.3 * (1.0 - (double)step_e / 5000.0);
                int idx_e = (int)(rng_uniform() * (double)orig_nd) % orig_nd;
                double old_e[3], old_res_e, new_res_e;

                old_e[0] = pert_dirs_e[idx_e][0];
                old_e[1] = pert_dirs_e[idx_e][1];
                old_e[2] = pert_dirs_e[idx_e][2];
                old_res_e = design_residual((const double (*)[3])pert_dirs_e,
                                            orig_nd, 6);

                pert_dirs_e[idx_e][0] = old_e[0] + rng_gauss() * temp_e;
                pert_dirs_e[idx_e][1] = old_e[1] + rng_gauss() * temp_e;
                pert_dirs_e[idx_e][2] = old_e[2] + rng_gauss() * temp_e;
                normalize3(pert_dirs_e[idx_e]);

                /* Clamp non-null and shared dirs to max_clamp_rad */
                if (dir_type[idx_e] != 0) {
                    double dot_e = pert_dirs_e[idx_e][0] * orig_dirs[idx_e][0]
                                 + pert_dirs_e[idx_e][1] * orig_dirs[idx_e][1]
                                 + pert_dirs_e[idx_e][2] * orig_dirs[idx_e][2];
                    double ang_e;
                    if (dot_e > 1.0) dot_e = 1.0;
                    if (dot_e < -1.0) dot_e = -1.0;
                    /* RP^2: use |dot| */
                    ang_e = acos(fabs(dot_e));
                    if (ang_e > max_clamp_rad) {
                        /* Revert — clamp exceeded */
                        pert_dirs_e[idx_e][0] = old_e[0];
                        pert_dirs_e[idx_e][1] = old_e[1];
                        pert_dirs_e[idx_e][2] = old_e[2];
                    }
                }

                new_res_e = design_residual((const double (*)[3])pert_dirs_e,
                                            orig_nd, 6);
                if (new_res_e > old_res_e) {
                    pert_dirs_e[idx_e][0] = old_e[0];
                    pert_dirs_e[idx_e][1] = old_e[1];
                    pert_dirs_e[idx_e][2] = old_e[2];
                }
            }
        }
    }

    /* Restore original directions */
    for (i = 0; i < orig_nd; i++) {
        g_dir[i][0] = orig_dirs[i][0];
        g_dir[i][1] = orig_dirs[i][1];
        g_dir[i][2] = orig_dirs[i][2];
    }
    g_nd = orig_nd;

    printf("\n");
}

/* ================================================================ */
/* Phase 4: Cross-Root Comparison at zeta_12                        */
/* ================================================================ */

static void phase4_cross_root(void) {
    double z8_res, z12_res;
    double z12_min_ang, z12_max_ang, z12_sum_ang;
    int z12_n_pairs;
    int i, s;

    printf("\n=== Phase 4: Cross-Root Comparison ===\n\n");

    /* zeta_8 design residual */
    z8_res = design_residual((const double (*)[3])z8_dirs, z8_nd, 6);
    printf("  zeta_8:  %d directions, residual(t=6) = %.4f\n", z8_nd, z8_res);

    /* Build zeta_12 */
    printf("  Building zeta_12 catalog...\n");
    init_su2(M_PI / 6.0);
    build_closure(0);
    build_dirs(g_cat_size);
    printf("  zeta_12: %d entries, %d directions\n", g_cat_size, g_nd);

    z12_res = design_residual((const double (*)[3])g_dir, g_nd, 6);
    printf("  zeta_12: residual(t=6) = %.4f\n\n", z12_res);

    /* zeta_12 pairwise angle statistics (sample) */
    z12_min_ang = 999; z12_max_ang = 0; z12_sum_ang = 0; z12_n_pairs = 0;

    for (s = 0; s < 100000; s++) {
        int ii = (int)(rng_uniform() * (double)g_nd) % g_nd;
        int jj = (int)(rng_uniform() * (double)g_nd) % g_nd;
        double dp, ang;
        if (ii == jj) continue;
        dp = g_dir[ii][0]*g_dir[jj][0] + g_dir[ii][1]*g_dir[jj][1] +
             g_dir[ii][2]*g_dir[jj][2];
        if (dp > 1.0) dp = 1.0;
        if (dp < -1.0) dp = -1.0;
        ang = acos(fabs(dp)) * 180.0 / M_PI;
        if (ang < z12_min_ang) z12_min_ang = ang;
        if (ang > z12_max_ang) z12_max_ang = ang;
        z12_sum_ang += ang;
        z12_n_pairs++;
    }

    printf("  Pairwise angles (sampled %d pairs):\n", z12_n_pairs);
    printf("    zeta_12: min=%.2f, max=%.2f, mean=%.2f deg\n\n",
           z12_min_ang, z12_max_ang,
           z12_n_pairs > 0 ? z12_sum_ang/(double)z12_n_pairs : 0.0);

    {
        double z8_norm = z8_res / (double)z8_nd;
        double z12_norm = z12_res / (double)g_nd;
        double z12_mean = z12_n_pairs > 0 ? z12_sum_ang/(double)z12_n_pairs : 0.0;

        printf("  +----------+------+----------+----------+----------+\n");
        printf("  | Root     | Dirs | Res(t=6) | Res/N    | MeanAngl |\n");
        printf("  +----------+------+----------+----------+----------+\n");
        printf("  | zeta_8   | %4d | %8.4f | %8.4f | %5.1f deg|\n",
               z8_nd, z8_res, z8_norm, 63.1);
        printf("  | zeta_12  | %4d | %8.4f | %8.4f | %5.1f deg|\n",
               g_nd, z12_res, z12_norm, z12_mean);
        printf("  +----------+------+----------+----------+----------+\n\n");

        check("zeta_12 has more directions", g_nd > z8_nd);

        if (z12_norm < z8_norm) {
            printf("  Per-direction residual: zeta_12 LOWER (%.4f vs %.4f)\n",
                   z12_norm, z8_norm);
            printf("  More directions = better per-direction integration.\n");
            printf("  Connects to Demo 87: nulls dispensable because direction\n");
            printf("  set is rich enough that no subset is critical.\n");
        } else {
            printf("  Per-direction residual: zeta_12 HIGHER (%.4f vs %.4f)\n",
                   z12_norm, z8_norm);
            printf("  Anti-correlation persists at larger groups.\n");
        }
    }

    /* Restore ζ₈ for any further use */
    for (i = 0; i < z8_nd; i++) {
        g_dir[i][0] = z8_dirs[i][0];
        g_dir[i][1] = z8_dirs[i][1];
        g_dir[i][2] = z8_dirs[i][2];
    }
    g_nd = z8_nd;

    printf("\n");
}

/* ================================================================ */
/* Main                                                             */
/* ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 88: Anti-Correlation Mechanism\n");
    printf("=============================================\n");

    /* Build zeta_8 catalog */
    printf("\n  Building zeta_8 catalog...\n");
    init_su2(M_PI / 4.0);
    build_closure(1);
    classify_null(g_cat_size);
    build_dirs(g_cat_size);
    printf("  %d entries, %d directions, %d null\n",
           g_cat_size, g_nd, n_null_total);

    check("zeta_8 catalog = 24", g_cat_size == 24);
    check("zeta_8 directions = 13", g_nd == 13);

    phase1_geometry();
    phase2_algebraic();
    phase3_perturbation();
    phase4_cross_root();

    printf("\n=============================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
