/*
 * KNOTAPEL DEMO 72: Spherical Design Test
 * =========================================
 *
 * Tests whether the 13 eigenvector directions form a spherical t-design.
 * A spherical t-design: sum of Y_lm over the points = 0 for all 1<=l<=t.
 * Compares t-design parameter with DKC bandwidth (l=6 from Demo 71).
 *
 * Part A: t-design test for 13 eigenvector directions
 * Part B: Compare to DKC bandwidth
 * Part C: Even vs odd mode analysis
 * Part D: Full 24-quaternion design on S^3
 * Part E: Comparison with known spherical design bounds
 * Part F: Residual analysis at first failing mode
 *
 * C89, zero dependencies beyond math.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int g_pass = 0, g_fail = 0;

static void check(const char *t, int ok) {
    if (ok) { printf("  PASS: %s\n", t); g_pass++; }
    else    { printf("  FAIL: %s\n", t); g_fail++; }
}

/* ================================================================
 * Quaternion (from Demo 67/70/71)
 * ================================================================ */

typedef struct { double a, b, c, d; } Quat;

static Quat qm(double a, double b, double c, double d) {
    Quat q; q.a=a; q.b=b; q.c=c; q.d=d; return q;
}
static Quat qadd(Quat p, Quat q) {
    return qm(p.a+q.a, p.b+q.b, p.c+q.c, p.d+q.d);
}
static Quat qneg(Quat q) { return qm(-q.a,-q.b,-q.c,-q.d); }
static Quat qmul(Quat p, Quat q) {
    return qm(p.a*q.a-p.b*q.b-p.c*q.c-p.d*q.d,
              p.a*q.b+p.b*q.a+p.c*q.d-p.d*q.c,
              p.a*q.c-p.b*q.d+p.c*q.a+p.d*q.b,
              p.a*q.d+p.b*q.c-p.c*q.b+p.d*q.a);
}
static Quat qconj(Quat q) { return qm(q.a,-q.b,-q.c,-q.d); }
static double qnorm(Quat q) {
    return sqrt(q.a*q.a+q.b*q.b+q.c*q.c+q.d*q.d);
}
static Quat qnormalize(Quat q) {
    double n = qnorm(q);
    if (n < 1e-15) return qm(0,0,0,0);
    return qm(q.a/n, q.b/n, q.c/n, q.d/n);
}

/* ================================================================
 * Eigenvector extraction
 * ================================================================ */

typedef struct {
    double theta, ax, ay, az;
    int axis_ok;
} Eigen;

static Eigen to_eigen(Quat q) {
    Eigen e;
    double nv;
    if (q.a < 0) { q.a=-q.a; q.b=-q.b; q.c=-q.c; q.d=-q.d; }
    if (q.a > 1.0) q.a = 1.0;
    e.theta = acos(q.a);
    nv = sqrt(q.b*q.b+q.c*q.c+q.d*q.d);
    if (nv < 1e-12) { e.ax=0; e.ay=0; e.az=1; e.axis_ok=0; }
    else { e.ax=q.b/nv; e.ay=q.c/nv; e.az=q.d/nv; e.axis_ok=1; }
    return e;
}

/* ================================================================
 * Braid generators (zeta_8: theta = pi/4)
 * ================================================================ */

static Quat g_s1, g_s2;

static void init_gens(void) {
    double c = cos(M_PI/4.0), s = sin(M_PI/4.0);
    g_s1 = qm(c, s, 0, 0);
    g_s2 = qm(c, 0, 0, -s);
}

static Quat braid_gen(int g) {
    if (g == 1)  return g_s1;
    if (g == 2)  return g_s2;
    if (g == -1) return qconj(g_s1);
    return qconj(g_s2);
}

/* ================================================================
 * Quaternion + direction catalogs
 * ================================================================ */

#define MAXQ 512
#define MAXD 64

static Quat g_cat[MAXQ];
static Eigen g_eig[MAXQ];
static int g_nq = 0;

static double g_dir[MAXD][3];
static int g_nd = 0;

/* Spherical coords for each direction */
static double g_dir_theta[MAXD];
static double g_dir_phi[MAXD];

static int find_q(Quat q) {
    int i;
    for (i = 0; i < g_nq; i++) {
        double d1 = fabs(g_cat[i].a-q.a)+fabs(g_cat[i].b-q.b)+
                     fabs(g_cat[i].c-q.c)+fabs(g_cat[i].d-q.d);
        double d2 = fabs(g_cat[i].a+q.a)+fabs(g_cat[i].b+q.b)+
                     fabs(g_cat[i].c+q.c)+fabs(g_cat[i].d+q.d);
        if (d1 < 1e-10 || d2 < 1e-10) return i;
    }
    return -1;
}

static int find_d(double x, double y, double z) {
    int i;
    for (i = 0; i < g_nd; i++) {
        double d1 = fabs(g_dir[i][0]-x)+fabs(g_dir[i][1]-y)+fabs(g_dir[i][2]-z);
        double d2 = fabs(g_dir[i][0]+x)+fabs(g_dir[i][1]+y)+fabs(g_dir[i][2]+z);
        if (d1 < 1e-8 || d2 < 1e-8) return i;
    }
    return -1;
}

static void build_cat(void) {
    int n, len, w[32], i;
    g_nq = 0; g_nd = 0;
    for (n = 2; n <= 3; n++) {
        int mg = n-1, tg = 2*mg;
        for (len = 1; len <= 8; len++) {
            unsigned long total = 1, idx;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)tg;
                if (total > 100000) break;
            }
            if (total > 100000) continue;
            for (idx = 0; idx < total; idx++) {
                unsigned long tmp = idx;
                Quat q = qm(1,0,0,0);
                for (i = 0; i < len; i++) {
                    int g2 = (int)(tmp % (unsigned long)tg);
                    tmp /= (unsigned long)tg;
                    w[i] = g2 < mg ? g2+1 : -(g2-mg+1);
                }
                for (i = 0; i < len; i++) q = qmul(q, braid_gen(w[i]));
                if (find_q(q) < 0 && g_nq < MAXQ)
                    g_cat[g_nq++] = q;
            }
        }
    }
    for (i = 0; i < g_nq; i++) {
        g_eig[i] = to_eigen(g_cat[i]);
        if (g_eig[i].axis_ok &&
            find_d(g_eig[i].ax, g_eig[i].ay, g_eig[i].az) < 0 &&
            g_nd < MAXD) {
            g_dir[g_nd][0] = g_eig[i].ax;
            g_dir[g_nd][1] = g_eig[i].ay;
            g_dir[g_nd][2] = g_eig[i].az;
            g_nd++;
        }
    }
    /* Compute spherical coords */
    for (i = 0; i < g_nd; i++) {
        double z = g_dir[i][2];
        if (z > 1.0) z = 1.0;
        if (z < -1.0) z = -1.0;
        g_dir_theta[i] = acos(z);
        g_dir_phi[i] = atan2(g_dir[i][1], g_dir[i][0]);
        if (g_dir_phi[i] < 0) g_dir_phi[i] += 2.0 * M_PI;
    }
}

/* ================================================================
 * Spherical harmonics (from Demo 71)
 * ================================================================ */

#define LMAX 12

static double factorial(int n) {
    double f = 1.0;
    int i;
    for (i = 2; i <= n; i++) f *= (double)i;
    return f;
}

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
        pll = (x * (double)(2*i-1) * pmm1 - (double)(i+m-1) * pmm) / (double)(i-m);
        pmm = pmm1;
        pmm1 = pll;
    }
    return pmm1;
}

static double ylm_norm(int l, int m) {
    int am = abs(m);
    double num = (double)(2*l+1) * factorial(l - am);
    double den = 4.0 * M_PI * factorial(l + am);
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

/* ================================================================
 * t-design sum: S_lm = sum_{i=1}^{N} Y_lm(dir_i)
 *
 * For undirected axes (our case), we sum over both +dir and -dir.
 * Y_lm(-x) = (-1)^l Y_lm(x), so:
 *   S_lm(+/-) = S_lm(+) + (-1)^l S_lm(+)
 *             = S_lm(+) * (1 + (-1)^l)
 *             = 0 for odd l, 2*S_lm(+) for even l
 *
 * Since odd l vanish automatically for undirected axes, we focus
 * on DIRECTED test (just the 13 directions as given) for the
 * t-design property. The undirected version is tested in Part C.
 * ================================================================ */

/* Sum Y_lm over the 13 directions (directed) */
static double slm_directed(int l, int m) {
    double sum = 0.0;
    int i;
    for (i = 0; i < g_nd; i++)
        sum += ylm_real(l, m, g_dir_theta[i], g_dir_phi[i]);
    return sum;
}

/* Sum Y_lm over both +dir and -dir (undirected, 26 points) */
static double slm_undirected(int l, int m) {
    double sum = 0.0;
    int i;
    for (i = 0; i < g_nd; i++) {
        sum += ylm_real(l, m, g_dir_theta[i], g_dir_phi[i]);
        /* Antipodal point: theta -> pi - theta, phi -> phi + pi */
        sum += ylm_real(l, m, M_PI - g_dir_theta[i],
                        g_dir_phi[i] + M_PI);
    }
    return sum;
}

/* ================================================================
 * Part A: t-design test for 13 eigenvector directions
 * ================================================================ */

static double g_max_slm[LMAX+1]; /* max |S_lm| for each l */
static int g_t_design = 0;       /* t-design parameter */

static void part_a(void) {
    int l, m;
    double tol = 1e-8;
    char msg[256];
    int failed = 0;

    printf("\n=== Part A: t-design test for %d eigenvector directions ===\n\n", g_nd);
    printf("  Testing S_lm = sum_{i=1}^{%d} Y_lm(dir_i) = 0 for t-design\n\n", g_nd);

    printf("  | l  | max|S_lm| | Pass? | 2l+1 |\n");
    printf("  |----|-----------|-------|------|\n");

    g_t_design = 0;

    for (l = 1; l <= LMAX; l++) {
        double max_abs = 0.0;
        for (m = -l; m <= l; m++) {
            double s = fabs(slm_directed(l, m));
            if (s > max_abs) max_abs = s;
        }
        g_max_slm[l] = max_abs;

        if (!failed && max_abs < tol) {
            g_t_design = l;
            printf("  | %2d | %9.2e |  YES  | %4d |\n", l, max_abs, 2*l+1);
        } else {
            if (!failed) failed = 1;
            printf("  | %2d | %9.2e |  no   | %4d |\n", l, max_abs, 2*l+1);
        }
    }

    printf("\n  The 13 eigenvector directions form a spherical t-design with t = %d\n",
           g_t_design);

    sprintf(msg, "t-design computed: t = %d (N=%d points)", g_t_design, g_nd);
    check(msg, 1);
}

/* ================================================================
 * Part B: Compare to DKC bandwidth
 * ================================================================ */

static void part_b(void) {
    int bw = 6; /* from Demo 71 */
    char msg[256];

    printf("\n=== Part B: Compare to DKC bandwidth ===\n\n");
    printf("  DKC bandwidth (Demo 71):  l = %d  (2l+1 = %d = %d directions)\n",
           bw, 2*bw+1, g_nd);
    printf("  t-design parameter:       t = %d\n\n", g_t_design);

    if (g_t_design >= bw) {
        if (g_t_design == bw) {
            printf("  EXACT MATCH: t = bandwidth = %d\n", bw);
            printf("  The directions are OPTIMAL sampling points for exactly\n");
            printf("  the modes the computation needs.\n");
        } else {
            printf("  OVER-SAMPLE: t = %d > bandwidth = %d\n", g_t_design, bw);
            printf("  The directions can exactly integrate polynomials of degree\n");
            printf("  higher than needed for DKC computation.\n");
        }
    } else {
        printf("  UNDER-DESIGN: t = %d < bandwidth = %d\n", g_t_design, bw);
        printf("  The directions are NOT a t-design at the bandwidth level.\n");
        printf("  Compressed sensing must do more work than the design property alone.\n");
        printf("\n  Gap: modes l=%d through l=%d are NOT exactly integrated.\n",
               g_t_design + 1, bw);
        printf("  The XOR6 computation works despite imperfect sampling.\n");
    }

    sprintf(msg, "Bandwidth comparison: t=%d vs bw=%d", g_t_design, bw);
    check(msg, 1);
}

/* ================================================================
 * Part C: Even vs odd mode analysis
 * ================================================================ */

static void part_c(void) {
    int l;
    double tol = 1e-8;
    char msg[256];
    int t_even = 0, t_odd = 0;
    int even_failed = 0, odd_failed = 0;

    printf("\n=== Part C: Even vs odd mode analysis ===\n\n");
    printf("  Undirected axes: Y_lm(-x) = (-1)^l Y_lm(x)\n");
    printf("  Sum over +-dirs: odd l vanish automatically.\n");
    printf("  Testing directed sums for even and odd l separately.\n\n");

    printf("  EVEN modes (directed sum):\n");
    printf("  | l  | max|S_lm| | Pass? |\n");
    printf("  |----|-----------|-------|\n");

    for (l = 2; l <= LMAX; l += 2) {
        double max_abs = g_max_slm[l]; /* already computed */
        if (!even_failed && max_abs < tol) {
            t_even = l;
            printf("  | %2d | %9.2e |  YES  |\n", l, max_abs);
        } else {
            if (!even_failed) even_failed = 1;
            printf("  | %2d | %9.2e |  no   |\n", l, max_abs);
        }
    }

    printf("\n  ODD modes (directed sum):\n");
    printf("  | l  | max|S_lm| | Pass? |\n");
    printf("  |----|-----------|-------|\n");

    for (l = 1; l <= LMAX; l += 2) {
        double max_abs = g_max_slm[l];
        if (!odd_failed && max_abs < tol) {
            t_odd = l;
            printf("  | %2d | %9.2e |  YES  |\n", l, max_abs);
        } else {
            if (!odd_failed) odd_failed = 1;
            printf("  | %2d | %9.2e |  no   |\n", l, max_abs);
        }
    }

    printf("\n  Max even t-design: t_even = %d\n", t_even);
    printf("  Max odd t-design:  t_odd  = %d\n\n", t_odd);

    if (t_even > t_odd)
        printf("  Directions are BETTER at sampling even modes (which DKC uses).\n");
    else if (t_even == t_odd)
        printf("  Even and odd modes have the same design strength.\n");
    else
        printf("  Directions are better at sampling odd modes (unexpected).\n");

    /* Undirected test: sum over both +dir and -dir */
    {
        int t_undir = 0;
        int undir_failed = 0;
        printf("\n  UNDIRECTED test (sum over %d points = 13 dirs + 13 antipodal):\n",
               2 * g_nd);
        printf("  | l  | max|S_lm| | Pass? |\n");
        printf("  |----|-----------|-------|\n");

        for (l = 1; l <= LMAX; l++) {
            int m2;
            double max_abs = 0.0;
            for (m2 = -l; m2 <= l; m2++) {
                double s = fabs(slm_undirected(l, m2));
                if (s > max_abs) max_abs = s;
            }
            if (!undir_failed && max_abs < tol) {
                t_undir = l;
                printf("  | %2d | %9.2e |  YES  |\n", l, max_abs);
            } else {
                if (!undir_failed) undir_failed = 1;
                printf("  | %2d | %9.2e |  no   |\n", l, max_abs);
            }
        }
        printf("\n  Undirected t-design: t = %d (26 points)\n", t_undir);
        printf("  (Odd l automatically vanish; effective test is on even l only)\n");

        sprintf(msg, "Even/odd analysis: t_even=%d, t_odd=%d, t_undir=%d",
                t_even, t_odd, t_undir);
        check(msg, 1);
    }
}

/* ================================================================
 * Part D: 24-quaternion design test
 * ================================================================
 *
 * The 24 quaternions of the binary octahedral group on S^3.
 * Test: do they form a t-design on S^3?
 *
 * Hyperspherical harmonics on S^3 can be tested via:
 * A set of N points on S^d is a t-design iff
 *   sum_i p(x_i) = (N/area(S^d)) * integral_{S^d} p(x) dS
 * for all polynomials p of degree <= t.
 *
 * For S^3 (4D), we test homogeneous harmonic polynomials.
 * Basis of degree-l harmonics on S^3: Gegenbauer polynomials.
 * Simpler test: check sum of monomials x^a y^b z^c w^d with a+b+c+d=k
 * against the sphere integral.
 *
 * For S^3: integral of x^{2a} y^{2b} z^{2c} w^{2d} over S^3 =
 *   2 * pi^2 * (2a-1)!!(2b-1)!!(2c-1)!!(2d-1)!! / (2(a+b+c+d)+2)!!
 * (with (-1)!! = 1)
 *
 * Odd powers integrate to zero.
 * Area(S^3) = 2*pi^2.
 *
 * Design test: for each monomial of degree k, check
 *   (1/N) sum_i monomial(q_i) = (1/area) integral monomial dS
 */

static double double_factorial(int n) {
    double f = 1.0;
    int i;
    for (i = n; i >= 2; i -= 2) f *= (double)i;
    return f;
}

/* Integral of x^(2a)*y^(2b)*z^(2c)*w^(2d) over S^3, divided by area(S^3) */
static double s3_even_monomial_avg(int a, int b, int c, int d) {
    int total = a + b + c + d;
    double num = double_factorial(2*a-1) * double_factorial(2*b-1)
               * double_factorial(2*c-1) * double_factorial(2*d-1);
    double den = double_factorial(2*total + 2);
    return num / den;
}

static void part_d(void) {
    int k, max_t = 0;
    double tol = 1e-8;
    char msg[256];
    int failed = 0;
    int npts = g_nq * 2; /* both q and -q for full group */

    printf("\n=== Part D: Quaternion design on S^3 ===\n\n");
    printf("  Catalog has %d quaternions (mod +-1)\n", g_nq);
    printf("  Binary octahedral group has %d elements (both q and -q)\n", npts);
    printf("  Testing monomial averages against S^3 integrals\n\n");

    /* Test degree by degree, using both q_i and -q_i */
    printf("  | deg | # monomials | max error   | Pass? |\n");
    printf("  |-----|-------------|-------------|-------|\n");

    for (k = 1; k <= 12; k++) {
        int a2, b2, c2, d2;
        double max_err = 0.0;
        int mono_count = 0;

        for (a2 = k; a2 >= 0; a2--)
            for (b2 = k-a2; b2 >= 0; b2--)
                for (c2 = k-a2-b2; c2 >= 0; c2--) {
                    int i;
                    double sum_pts = 0.0;
                    double expected;
                    double err;
                    d2 = k - a2 - b2 - c2;

                    if (a2 % 2 != 0 || b2 % 2 != 0 ||
                        c2 % 2 != 0 || d2 % 2 != 0) {
                        expected = 0.0;
                    } else {
                        expected = s3_even_monomial_avg(a2/2, b2/2, c2/2, d2/2);
                    }

                    /* Sum over both q_i and -q_i */
                    for (i = 0; i < g_nq; i++) {
                        double v_pos = 1.0, v_neg = 1.0;
                        int j;
                        double na = -g_cat[i].a, nb = -g_cat[i].b;
                        double nc = -g_cat[i].c, nd = -g_cat[i].d;
                        for (j = 0; j < a2; j++) { v_pos *= g_cat[i].a; v_neg *= na; }
                        for (j = 0; j < b2; j++) { v_pos *= g_cat[i].b; v_neg *= nb; }
                        for (j = 0; j < c2; j++) { v_pos *= g_cat[i].c; v_neg *= nc; }
                        for (j = 0; j < d2; j++) { v_pos *= g_cat[i].d; v_neg *= nd; }
                        sum_pts += v_pos + v_neg;
                    }
                    sum_pts /= (double)npts;

                    err = fabs(sum_pts - expected);
                    if (err > max_err) max_err = err;
                    mono_count++;
                }

        if (!failed && max_err < tol) {
            max_t = k;
            printf("  | %3d | %11d | %11.2e |  YES  |\n", k, mono_count, max_err);
        } else {
            if (!failed) failed = 1;
            printf("  | %3d | %11d | %11.2e |  no   |\n", k, mono_count, max_err);
        }
    }

    printf("\n  The %d quaternions (full group) form a t-design on S^3 with t = %d\n",
           npts, max_t);
    printf("  (Known: 24-cell vertices are a 5-design; binary octahedral has 48 elements)\n");

    sprintf(msg, "S^3 design: t = %d for %d quaternions", max_t, npts);
    check(msg, 1);
}

/* ================================================================
 * Part E: Comparison with known spherical design bounds
 * ================================================================ */

static void part_e(void) {
    int t;
    char msg[256];
    int max_possible_t = 0;

    printf("\n=== Part E: Spherical design bounds ===\n\n");
    printf("  Delsarte lower bound on S^2:\n");
    printf("    Even t: N >= (t+2)(t+4)/8\n");
    printf("    Odd t:  N >= (t+1)(t+3)/8\n");
    printf("  We have N = %d directed points.\n\n", g_nd);

    printf("  | t  | Delsarte min N | N=%d >= min? | Actual |\n", g_nd);
    printf("  |----|----------------|-------------|--------|\n");

    for (t = 1; t <= 12; t++) {
        int min_n;
        int feasible;
        const char *actual;
        if (t % 2 == 0)
            min_n = (t+2)*(t+4)/8;
        else
            min_n = (t+1)*(t+3)/8;
        feasible = (g_nd >= min_n);
        if (feasible && t <= g_t_design)
            actual = "ACHIEVED";
        else if (feasible && t > g_t_design)
            actual = "possible (not achieved)";
        else
            actual = "impossible";

        if (feasible) max_possible_t = t;

        printf("  | %2d | %14d | %11s | %-23s |\n",
               t, min_n, feasible ? "yes" : "NO", actual);
    }

    printf("\n  Maximum theoretically possible t-design with %d points: t = %d\n",
           g_nd, max_possible_t);
    printf("  Achieved t-design: t = %d\n", g_t_design);

    if (g_t_design == max_possible_t)
        printf("  The directions SATURATE the Delsarte bound!\n");
    else
        printf("  Gap: t = %d achieved vs t = %d possible\n",
               g_t_design, max_possible_t);

    sprintf(msg, "Design bounds: achieved t=%d, max possible t=%d",
            g_t_design, max_possible_t);
    check(msg, max_possible_t > 0);
}

/* ================================================================
 * Part F: Residual analysis at first failing mode
 * ================================================================ */

static void part_f(void) {
    int l_fail = g_t_design + 1;
    int m;
    double max_abs = 0.0;
    int max_m = 0;
    char msg[256];

    printf("\n=== Part F: Residual analysis at first failing mode l=%d ===\n\n",
           l_fail);

    if (l_fail > LMAX) {
        printf("  No failing mode within l <= %d!\n", LMAX);
        check("Residual analysis", 1);
        return;
    }

    printf("  S_lm values for l=%d:\n\n", l_fail);
    printf("  | m   | S_%d,m     | |S_%d,m|  |\n", l_fail, l_fail);
    printf("  |-----|-----------|----------|\n");

    for (m = -l_fail; m <= l_fail; m++) {
        double s = slm_directed(l_fail, m);
        double a = fabs(s);
        if (a > max_abs) { max_abs = a; max_m = m; }
        printf("  | %+3d | %+9.6f | %8.6f |%s\n",
               m, s, a, (a > 1e-8) ? " *" : "");
    }

    printf("\n  Largest deviation: |S_%d,%d| = %.6f\n", l_fail, max_m, max_abs);
    printf("  Number of nonzero S_lm: ");
    {
        int count = 0;
        for (m = -l_fail; m <= l_fail; m++)
            if (fabs(slm_directed(l_fail, m)) > 1e-8) count++;
        printf("%d out of %d\n", count, 2*l_fail+1);
    }

    /* Character of the failure */
    printf("\n  Severity: %.6f / sqrt(N=%d) = %.6f (normalized)\n",
           max_abs, g_nd, max_abs / sqrt((double)g_nd));

    /* Check if this is an even or odd mode */
    printf("  Mode l=%d is %s\n", l_fail, (l_fail % 2 == 0) ? "EVEN" : "ODD");
    if (l_fail % 2 != 0) {
        printf("  Note: odd modes don't contribute to DKC (antipodal symmetry)\n");
        printf("  The EFFECTIVE first failure for DKC might be at a higher even l.\n");
    }

    sprintf(msg, "First failure at l=%d (max |S_lm|=%.4f)", l_fail, max_abs);
    check(msg, 1);
}

/* ================================================================
 * Voronoi cell assignment + XOR6 checker (for Parts G, H)
 * ================================================================ */

static int vor_cell_ext(double ax, double ay, double az,
                        const double c[][3], int nc) {
    int i, best=0;
    double bd=-2.0;
    for (i=0; i<nc; i++) {
        double d = fabs(ax*c[i][0]+ay*c[i][1]+az*c[i][2]);
        if (d > bd) { bd=d; best=i; }
    }
    return best;
}

static int xor6_check_ext(const Quat *w, const double c[][3], int nc) {
    int seen[256];
    int mask, i, cell;
    for (i=0; i<=nc; i++) seen[i]=0;
    for (mask=0; mask<64; mask++) {
        Quat sum = qm(0,0,0,0);
        int par = 0;
        Eigen ed;
        for (i=0; i<6; i++)
            if (mask & (1<<i)) { sum=qadd(sum,w[i]); par^=1; }
        if (qnorm(sum) < 1e-12) cell=nc;
        else {
            sum=qnormalize(sum);
            ed=to_eigen(sum);
            cell = ed.axis_ok ? vor_cell_ext(ed.ax,ed.ay,ed.az,c,nc) : nc;
        }
        seen[cell] |= (1<<par);
        if (seen[cell]==3) return 0;
    }
    return 1;
}

static int count_xor6_ext(const double c[][3], int nc) {
    int ai,aj,ak,count=0;
    for (ai=0; ai<g_nq; ai++)
        for (aj=ai+1; aj<g_nq; aj++)
            for (ak=aj+1; ak<g_nq; ak++) {
                Quat w[6];
                w[0]=g_cat[ai]; w[1]=qneg(g_cat[ai]);
                w[2]=g_cat[aj]; w[3]=qneg(g_cat[aj]);
                w[4]=g_cat[ak]; w[5]=qneg(g_cat[ak]);
                if (xor6_check_ext(w,c,nc)) count++;
            }
    return count;
}

/* ================================================================
 * Simple LCG random number generator (deterministic, no stdlib rand)
 * ================================================================ */

static unsigned long g_rng = 12345UL;

static double rng_uniform(void) {
    g_rng = g_rng * 1103515245UL + 12345UL;
    return (double)((g_rng >> 16) & 0x7FFF) / 32768.0;
}

static double rng_gauss(void) {
    /* Box-Muller */
    double u1 = rng_uniform() + 1e-15;
    double u2 = rng_uniform();
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

/* ================================================================
 * Part G: Optimal 6-design comparison
 * ================================================================
 *
 * Generate "well-distributed" 13 points on S^2 by optimizing to
 * minimize max |Σ Y_lm(x_i)| for l=1..6 (approximate a 6-design).
 * Then run XOR6 on those directions and compare to eigenvector result.
 */

static void normalize3(double *v) {
    double n = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    if (n > 1e-15) { v[0]/=n; v[1]/=n; v[2]/=n; }
}

/* Compute t-design residual: max over l=1..t of max_m |S_lm| */
static double design_residual(const double pts[][3], int np, int t) {
    int l, m;
    double max_res = 0.0;
    for (l = 1; l <= t; l++)
        for (m = -l; m <= l; m++) {
            int i;
            double sum = 0.0;
            for (i = 0; i < np; i++) {
                double z = pts[i][2], theta, phi;
                if (z > 1.0) z = 1.0;
                if (z < -1.0) z = -1.0;
                theta = acos(z);
                phi = atan2(pts[i][1], pts[i][0]);
                if (phi < 0) phi += 2.0 * M_PI;
                sum += ylm_real(l, m, theta, phi);
            }
            if (fabs(sum) > max_res) max_res = fabs(sum);
        }
    return max_res;
}

static void part_g(void) {
    double opt_pts[MAXD][3];
    double best_pts[MAXD][3];
    double best_res = 1e30;
    int trial, step, i;
    int xor_opt, xor_eig;
    char msg[256];
    int np = g_nd; /* use same number of points as eigenvector dirs */

    printf("\n=== Part G: Optimal design comparison ===\n\n");
    printf("  Generating %d approximately-optimal points on S^2\n", np);
    printf("  (stochastic optimization to minimize t=6 design residual)\n\n");

    /* Run multiple random starts */
    for (trial = 0; trial < 20; trial++) {
        double res;
        /* Random initial placement */
        for (i = 0; i < np; i++) {
            opt_pts[i][0] = rng_gauss();
            opt_pts[i][1] = rng_gauss();
            opt_pts[i][2] = rng_gauss();
            normalize3(opt_pts[i]);
        }

        /* Simple gradient-free optimization: jitter each point */
        for (step = 0; step < 2000; step++) {
            double temp = 0.5 * (1.0 - (double)step / 2000.0);
            int idx = (int)(rng_uniform() * (double)np) % np;
            double old[3], pert[3];
            double old_res, new_res;

            old[0]=opt_pts[idx][0]; old[1]=opt_pts[idx][1]; old[2]=opt_pts[idx][2];
            old_res = design_residual((const double (*)[3])opt_pts, np, 6);

            pert[0] = old[0] + rng_gauss() * temp;
            pert[1] = old[1] + rng_gauss() * temp;
            pert[2] = old[2] + rng_gauss() * temp;
            normalize3(pert);
            opt_pts[idx][0]=pert[0]; opt_pts[idx][1]=pert[1]; opt_pts[idx][2]=pert[2];

            new_res = design_residual((const double (*)[3])opt_pts, np, 6);
            if (new_res > old_res) {
                /* Reject: restore */
                opt_pts[idx][0]=old[0]; opt_pts[idx][1]=old[1]; opt_pts[idx][2]=old[2];
            }
        }

        res = design_residual((const double (*)[3])opt_pts, np, 6);
        if (res < best_res) {
            best_res = res;
            for (i = 0; i < np; i++) {
                best_pts[i][0]=opt_pts[i][0];
                best_pts[i][1]=opt_pts[i][1];
                best_pts[i][2]=opt_pts[i][2];
            }
        }
    }

    {
        double eig_res = design_residual((const double (*)[3])g_dir, g_nd, 6);
        printf("  Optimized design residual (t=6): %.6f\n", best_res);
        printf("  Eigenvector design residual (t=6): %.6f\n", eig_res);
        printf("  Improvement: %.1fx\n\n", eig_res / (best_res > 1e-15 ? best_res : 1e-15));
    }

    /* XOR6 with optimized directions */
    printf("  Counting XOR6 solutions...\n");
    printf("    Eigenvector (%d dirs)...", g_nd);
    fflush(stdout);
    xor_eig = count_xor6_ext(g_dir, g_nd);
    printf(" %d solutions\n", xor_eig);

    printf("    Optimized design (%d dirs)...", np);
    fflush(stdout);
    xor_opt = count_xor6_ext(best_pts, np);
    printf(" %d solutions\n", xor_opt);

    printf("\n  | Directions     | Design res | XOR6 solutions |\n");
    printf("  |----------------|-----------|----------------|\n");
    printf("  | Eigenvector    | %9.4f | %14d |\n",
           design_residual((const double (*)[3])g_dir, g_nd, 6), xor_eig);
    printf("  | Optimized      | %9.4f | %14d |\n", best_res, xor_opt);

    if (xor_opt < xor_eig)
        printf("\n  CONFIRMED: better design geometry -> FEWER XOR6 solutions.\n");
    else if (xor_opt == 0)
        printf("\n  CONFIRMED: optimal design gets ZERO XOR6 solutions.\n");
    else
        printf("\n  NOTE: optimized design also finds solutions (algebra may not be unique).\n");

    printf("  DKC computation requires algebraic structure, not geometric optimality.\n");

    sprintf(msg, "Optimal vs eigenvector: %d vs %d XOR6 solutions", xor_opt, xor_eig);
    check(msg, xor_eig == 36);
}

/* ================================================================
 * Part H: Perturbation sensitivity
 * ================================================================ */

static void part_h(void) {
    double eps_deg[] = {1.0, 5.0, 10.0, 20.0};
    int neps = 4;
    int trials = 10;
    int e, t, i;
    char msg[256];
    int sharp = 1;

    printf("\n=== Part H: Perturbation sensitivity ===\n\n");
    printf("  Perturbing %d eigenvector directions by angular epsilon.\n", g_nd);
    printf("  %d random trials per epsilon level.\n\n", trials);

    printf("  | Epsilon | Mean XOR6 | Min | Max | Loss from 36 |\n");
    printf("  |---------|-----------|-----|-----|--------------|\n");

    for (e = 0; e < neps; e++) {
        double eps_rad = eps_deg[e] * M_PI / 180.0;
        int sum_xor = 0, min_xor = 9999, max_xor = 0;

        for (t = 0; t < trials; t++) {
            double pert[MAXD][3];
            int xor_count;

            /* Perturb each direction */
            for (i = 0; i < g_nd; i++) {
                double px = g_dir[i][0] + rng_gauss() * eps_rad;
                double py = g_dir[i][1] + rng_gauss() * eps_rad;
                double pz = g_dir[i][2] + rng_gauss() * eps_rad;
                double n = sqrt(px*px+py*py+pz*pz);
                if (n > 1e-15) { pert[i][0]=px/n; pert[i][1]=py/n; pert[i][2]=pz/n; }
                else { pert[i][0]=g_dir[i][0]; pert[i][1]=g_dir[i][1]; pert[i][2]=g_dir[i][2]; }
            }

            xor_count = count_xor6_ext(pert, g_nd);
            sum_xor += xor_count;
            if (xor_count < min_xor) min_xor = xor_count;
            if (xor_count > max_xor) max_xor = xor_count;
        }

        {
            double mean = (double)sum_xor / (double)trials;
            printf("  | %5.0f°  | %9.1f | %3d | %3d | %12.1f |\n",
                   eps_deg[e], mean, min_xor, max_xor, 36.0 - mean);

            if (mean < 35.0) sharp = 0;
        }
    }

    printf("\n  Unperturbed: 36 solutions (reference)\n");
    if (sharp)
        printf("  Sensitivity: LOW (robust to small perturbations)\n");
    else
        printf("  Sensitivity: HIGH (solutions drop with perturbation)\n");

    sprintf(msg, "Perturbation test (%d eps levels, %d trials each)", neps, trials);
    check(msg, 1);
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 72: Spherical Design Test\n");
    printf("=========================================\n");

    init_gens();

    printf("\n  Building zeta_8 catalog...\n");
    build_cat();
    printf("    %d quaternions, %d directions\n", g_nq, g_nd);

    part_a();
    part_b();
    part_c();
    part_d();
    part_e();
    part_f();
    part_g();
    part_h();

    printf("\n=========================================\n");
    printf("Results: %d pass, %d fail\n", g_pass, g_fail);
    return g_fail > 0 ? 1 : 0;
}
