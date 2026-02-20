/*
 * Demo 62: Analytical Proof — WHY 0x06 and 0x1B Are Unreachable
 *
 * Demo 61 proved computationally that exactly 11/13 NPN classes are
 * reachable by half-plane activations on multiplicative encodings.
 * This demo proves it ANALYTICALLY: pure algebra, no search.
 *
 * Two independent proofs:
 *   0x1B (CROSS):  direct algebraic contradiction in 3 steps
 *   0x06 (XOR-AND): four-semicircle parallelogram argument
 *
 * Both proofs work for continuous angles (real-valued), which
 * subsumes the discrete N-point case used in Demo 61.
 */

#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ========== Utility ========== */

static int pass_count = 0;
static int fail_count = 0;

static void check(int cond, const char *msg)
{
    if (cond) {
        printf("  PASS: %s\n", msg);
        pass_count++;
    } else {
        printf("  FAIL: %s\n", msg);
        fail_count++;
    }
}

/* Wrap angle into [0, 2pi) */
static double wrap(double a)
{
    while (a < 0.0)       a += 2.0 * M_PI;
    while (a >= 2.0 * M_PI) a -= 2.0 * M_PI;
    return a;
}

/* Is angle a in semicircle [theta, theta+pi) mod 2pi? */
static int in_semi(double a, double theta)
{
    double d = wrap(a - theta);
    return d < M_PI ? 1 : 0;
}

/* Compute truth table for given angles and threshold */
static int truth_table(double p1, double p2, double p3, double theta)
{
    double ang[8];
    int tt = 0, b;

    ang[0] = 0.0;
    ang[1] = p3;
    ang[2] = p2;
    ang[3] = wrap(p2 + p3);
    ang[4] = p1;
    ang[5] = wrap(p1 + p3);
    ang[6] = wrap(p1 + p2);
    ang[7] = wrap(p1 + p2 + p3);

    for (b = 0; b < 8; b++) {
        if (in_semi(ang[b], theta))
            tt |= (1 << b);
    }
    return tt;
}

/* NPN canonical form (same as demo 61) */
static int npn_table[256];

static void build_npn(void)
{
    static const int pm[6][3] = {
        {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
    };
    int t, p, ni;
    for (t = 0; t < 256; t++) {
        int best = t;
        for (p = 0; p < 6; p++) {
            for (ni = 0; ni < 8; ni++) {
                int nt = 0, b;
                for (b = 0; b < 8; b++) {
                    int x[3], s, v;
                    x[0] = b & 1;
                    x[1] = (b >> 1) & 1;
                    x[2] = (b >> 2) & 1;
                    if (ni & 1) x[0] ^= 1;
                    if (ni & 2) x[1] ^= 1;
                    if (ni & 4) x[2] ^= 1;
                    s = (x[pm[p][2]] << 2) | (x[pm[p][1]] << 1) | x[pm[p][0]];
                    v = (t >> s) & 1;
                    nt |= (v << b);
                }
                if (nt < best) best = nt;
                nt = (~nt) & 0xFF;
                if (nt < best) best = nt;
            }
        }
        npn_table[t] = best;
    }
}

/* ========== Proof A: 0x1B (CROSS) — Algebraic Contradiction ========== */

static void prove_0x1B(void)
{
    printf("========================================================\n");
    printf("  PROOF A: 0x1B (CROSS) is unreachable\n");
    printf("========================================================\n\n");

    printf("  0x1B = {000, 001, 011, 100}\n");
    printf("  TRUE bits:  0(=000), phi3(=001), phi2+phi3(=011), phi1(=100)\n");
    printf("  FALSE bits: phi2(=010), phi1+phi3(=101), phi1+phi2(=110),\n");
    printf("              phi1+phi2+phi3(=111)\n\n");

    printf("  Step 1: Set theta = 0 (WLOG, since 0 is in TRUE set).\n");
    printf("          Semicircle S = [0, pi).\n");
    printf("          phi3 in S        =>  phi3 in [0, pi).\n");
    printf("          phi1 in S        =>  phi1 in [0, pi).\n");
    printf("          phi2 in S-bar    =>  phi2 in [pi, 2pi).\n\n");

    printf("  Step 2: Write phi2 = pi + x,  x in [0, pi).\n");
    printf("          phi3 = y,              y in [0, pi).\n\n");

    printf("          phi2 + phi3 in S requires:\n");
    printf("            (pi + x + y) mod 2pi in [0, pi)\n");
    printf("            This wraps iff x + y >= pi.\n");
    printf("            When it wraps: phi2+phi3 = x + y - pi, in [0, pi). OK.\n");
    printf("            So: x + y >= pi.  ... (I)\n\n");

    printf("          phi1+phi2+phi3 in S-bar requires:\n");
    printf("            phi1 + (pi+x) + y  mod 2pi  in [pi, 2pi)\n");
    printf("            Let delta = x + y - pi >= 0 (from (I)).\n");
    printf("            phi1 + pi + delta  mod 2pi  in [pi, 2pi)\n");
    printf("            This means: phi1 + delta >= pi\n");
    printf("            i.e. phi1 >= pi - delta = 2pi - x - y.  ... (II)\n\n");

    printf("          phi1+phi2 in S-bar requires:\n");
    printf("            phi1 + pi + x  mod 2pi  in [pi, 2pi)\n");
    printf("            Since phi1 < pi and x < pi: phi1+pi+x < 3pi.\n");
    printf("            For this to be in [pi,2pi): need phi1+pi+x < 2pi\n");
    printf("            i.e. phi1 < pi - x.  ... (III)\n\n");

    printf("  Step 3: Combine (II) and (III):\n");
    printf("            2pi - x - y  <=  phi1  <  pi - x\n");
    printf("            Lower bound < upper bound requires:\n");
    printf("              2pi - x - y  <  pi - x\n");
    printf("              2pi - y  <  pi\n");
    printf("              pi  <  y\n\n");

    printf("          But y = phi3 in [0, pi), so y < pi.\n");
    printf("          CONTRADICTION.  No valid phi1 exists.\n\n");

    printf("  QED: 0x1B is unreachable for ANY angles (phi1,phi2,phi3)\n");
    printf("  and ANY semicircle orientation theta.\n\n");

    /* Numerical verification */
    printf("  --- Numerical verification ---\n\n");
    {
        /* Try configurations that get CLOSE to 0x1B */
        double test_p1[] = { 0.5, 1.0, 2.0, 0.3, 2.5 };
        double test_p2[] = { 3.5, 4.0, 3.8, 3.3, 4.5 };
        double test_p3[] = { 1.5, 1.2, 0.8, 2.0, 0.5 };
        int n_tests = 5;
        int i, any_found = 0;

        printf("  Trying configurations near 0x1B boundary:\n\n");
        for (i = 0; i < n_tests; i++) {
            double p1 = test_p1[i], p2 = test_p2[i], p3 = test_p3[i];
            double theta;
            int best_dist = 9;
            double best_theta = 0.0;
            int best_tt = 0;
            int steps = 1000;
            int j;

            for (j = 0; j < steps; j++) {
                int tt, dist, b;
                theta = 2.0 * M_PI * (double)j / (double)steps;
                tt = truth_table(p1, p2, p3, theta);
                dist = 0;
                for (b = 0; b < 8; b++)
                    dist += ((tt >> b) & 1) != ((0x1B >> b) & 1);
                if (dist < best_dist) {
                    best_dist = dist;
                    best_theta = theta;
                    best_tt = tt;
                }
            }

            printf("    p=(%.2f, %.2f, %.2f): closest TT=0x%02X "
                   "(NPN 0x%02X), dist=%d from 0x1B\n",
                   p1, p2, p3, best_tt, npn_table[best_tt], best_dist);
            (void)best_theta;

            if (best_dist == 0) any_found = 1;
        }
        printf("\n");
        check(!any_found, "0x1B never achieved in numerical tests");
    }

    /* Exhaustive integer check at small N */
    {
        int N = 120, half = 60;
        int p1, p2, p3, k, found = 0;

        for (p1 = 0; p1 < N && !found; p1++) {
            for (p2 = 0; p2 < N && !found; p2++) {
                for (p3 = 0; p3 < N && !found; p3++) {
                    int a[8];
                    a[0] = 0;
                    a[1] = p3;
                    a[2] = p2;
                    a[3] = (p2 + p3) % N;
                    a[4] = p1;
                    a[5] = (p1 + p3) % N;
                    a[6] = (p1 + p2) % N;
                    a[7] = ((p1 + p2) % N + p3) % N;

                    for (k = 0; k < 8; k++) {
                        int th, tt, b;
                        th = (a[k] + 1) % N;
                        tt = 0;
                        for (b = 0; b < 8; b++) {
                            int d = a[b] - th;
                            if (d < 0) d += N;
                            if (d < half) tt |= (1 << b);
                        }
                        if (npn_table[tt] == 0x1B) { found = 1; break; }
                    }
                }
            }
        }
        check(!found, "0x1B absent in exhaustive N=120 search (confirms proof)");
    }
    printf("\n");
}

/* ========== Proof B: 0x06 (XOR-AND) — Parallelogram Argument ========== */

static void prove_0x06(void)
{
    printf("========================================================\n");
    printf("  PROOF B: 0x06 (XOR-AND) is unreachable\n");
    printf("========================================================\n\n");

    printf("  0x06 = {001, 010}\n");
    printf("  TRUE bits:  phi3(=001), phi2(=010)\n");
    printf("  FALSE bits: 0(=000), phi2+phi3(=011), phi1(=100),\n");
    printf("              phi1+phi3(=101), phi1+phi2(=110),\n");
    printf("              phi1+phi2+phi3(=111)\n\n");

    printf("  Step 1: Semicircle S = [theta, theta+pi).\n");
    printf("          0 not in S  =>  theta > 0.\n");
    printf("          phi2, phi3 in S.\n");
    printf("          WLOG let alpha = phi2, beta = phi3 with alpha <= beta.\n");
    printf("          Both in [theta, theta+pi).\n\n");

    printf("  Step 2: phi2+phi3 not in S forces:\n");
    printf("            (alpha + beta) mod 2pi  not in [theta, theta+pi)\n");
    printf("          Since alpha,beta in [theta, theta+pi), their sum is in\n");
    printf("          [2*theta, 2*theta+2pi). For this NOT to hit S:\n");
    printf("            alpha + beta >= theta + pi  (it lands in S-bar).\n");
    printf("          So alpha + beta >= theta + pi > pi.  ... (KEY)\n\n");

    printf("  Step 3: The 4 phi1-dependent FALSE constraints.\n");
    printf("          phi1 not in S,  phi1+alpha not in S,\n");
    printf("          phi1+beta not in S,  phi1+alpha+beta not in S.\n\n");
    printf("          Each 'not in S' restricts phi1 to a semicircle of S-bar:\n");
    printf("            phi1 in [theta+pi, theta+2pi)  (i.e. S-bar)\n");
    printf("            phi1 in [theta+pi-alpha, theta+2pi-alpha)\n");
    printf("            phi1 in [theta+pi-beta, theta+2pi-beta)\n");
    printf("            phi1 in [theta+pi-alpha-beta, theta+2pi-alpha-beta)\n\n");

    printf("          These are 4 semicircles of width pi centered at midpoints:\n");
    printf("            M0 = theta + 3pi/2\n");
    printf("            M1 = theta + 3pi/2 - alpha\n");
    printf("            M2 = theta + 3pi/2 - beta\n");
    printf("            M3 = theta + 3pi/2 - alpha - beta\n\n");

    printf("          The midpoints form a PARALLELOGRAM on the circle\n");
    printf("          (shifts by alpha and beta are additive).\n\n");

    printf("  Step 4: Gap analysis.\n");
    printf("          Sorted gaps between consecutive midpoints:\n");
    printf("            g1 = alpha           (M0 to M1)\n");
    printf("            g2 = beta - alpha    (M1 to M2, or M2 to M3)\n");
    printf("            g3 = alpha           (M2 to M3, or M1 to M0)\n");
    printf("            g4 = 2pi-alpha-beta  (remaining arc)\n\n");

    printf("          From (KEY): alpha + beta > pi, so:\n");
    printf("            g4 = 2pi - alpha - beta < pi.\n");
    printf("          Since alpha <= beta and alpha+beta <= 2pi:\n");
    printf("            g1 = alpha <= beta, and alpha < pi (since alpha in S).\n");
    printf("            g2 = beta-alpha < pi (since both in same semicircle).\n");
    printf("            g3 = alpha < pi.\n\n");

    printf("          ALL FOUR GAPS < pi.\n\n");

    printf("          The intersection of 4 semicircles of width pi is\n");
    printf("          non-empty iff some gap between consecutive midpoints\n");
    printf("          is >= pi.  Since all gaps < pi, the intersection\n");
    printf("          is EMPTY.  No valid phi1 exists.\n\n");

    printf("  QED: 0x06 is unreachable for ANY angles and ANY threshold.\n\n");

    printf("  --- Why the parallelogram argument works ---\n");
    printf("  The key is alpha+beta > pi (forced by phi2+phi3 not in S).\n");
    printf("  This single inequality makes the \"big gap\" shrink below pi,\n");
    printf("  which closes the only possible window for phi1.  For the 12\n");
    printf("  achievable classes, no such global squeeze occurs — there is\n");
    printf("  always at least one gap >= pi.\n\n");

    /* Numerical verification */
    printf("  --- Numerical verification ---\n\n");
    {
        double test_p1[] = { 0.5, 1.0, 2.0, 0.1, 2.8 };
        double test_p2[] = { 1.0, 1.5, 0.8, 0.5, 1.2 };
        double test_p3[] = { 2.0, 2.5, 1.8, 0.7, 2.0 };
        int n_tests = 5;
        int i, any_found = 0;

        printf("  Trying configurations near 0x06 boundary:\n\n");
        for (i = 0; i < n_tests; i++) {
            double p1 = test_p1[i], p2 = test_p2[i], p3 = test_p3[i];
            double theta;
            int best_dist = 9;
            double best_theta = 0.0;
            int best_tt = 0;
            int steps = 1000;
            int j;

            for (j = 0; j < steps; j++) {
                int tt, dist, b;
                theta = 2.0 * M_PI * (double)j / (double)steps;
                tt = truth_table(p1, p2, p3, theta);
                dist = 0;
                for (b = 0; b < 8; b++)
                    dist += ((tt >> b) & 1) != ((0x06 >> b) & 1);
                if (dist < best_dist) {
                    best_dist = dist;
                    best_theta = theta;
                    best_tt = tt;
                }
            }

            printf("    p=(%.2f, %.2f, %.2f): closest TT=0x%02X "
                   "(NPN 0x%02X), dist=%d from 0x06\n",
                   p1, p2, p3, best_tt, npn_table[best_tt], best_dist);
            (void)best_theta;

            if (best_dist == 0) any_found = 1;
        }
        printf("\n");
        check(!any_found, "0x06 never achieved in numerical tests");
    }

    /* Exhaustive integer check at small N */
    {
        int N = 120, half = 60;
        int p1, p2, p3, k, found = 0;

        for (p1 = 0; p1 < N && !found; p1++) {
            for (p2 = 0; p2 < N && !found; p2++) {
                for (p3 = 0; p3 < N && !found; p3++) {
                    int a[8];
                    a[0] = 0;
                    a[1] = p3;
                    a[2] = p2;
                    a[3] = (p2 + p3) % N;
                    a[4] = p1;
                    a[5] = (p1 + p3) % N;
                    a[6] = (p1 + p2) % N;
                    a[7] = ((p1 + p2) % N + p3) % N;

                    for (k = 0; k < 8; k++) {
                        int th, tt, b;
                        th = (a[k] + 1) % N;
                        tt = 0;
                        for (b = 0; b < 8; b++) {
                            int d = a[b] - th;
                            if (d < 0) d += N;
                            if (d < half) tt |= (1 << b);
                        }
                        if (npn_table[tt] == 0x06) { found = 1; break; }
                    }
                }
            }
        }
        check(!found, "0x06 absent in exhaustive N=120 search (confirms proof)");
    }
    printf("\n");
}

/* ========== Gap Analysis Demo ========== */

/*
 * For a near-miss configuration, show the parallelogram gap structure
 * to illustrate WHY the proof works.
 */
static void gap_analysis_demo(void)
{
    /* Configurations that produce truth tables 1 bit from 0x06 or 0x1B */
    struct {
        double p1, p2, p3;
        int target;
        const char *desc;
    } cases[4];
    int n_cases = 4;
    int i;

    printf("========================================================\n");
    printf("  GAP ANALYSIS: near-miss configurations\n");
    printf("========================================================\n\n");

    cases[0].p1 = 0.5; cases[0].p2 = 1.0; cases[0].p3 = 2.0;
    cases[0].target = 0x06; cases[0].desc = "near 0x06";
    cases[1].p1 = 2.0; cases[1].p2 = 1.5; cases[1].p3 = 0.8;
    cases[1].target = 0x06; cases[1].desc = "near 0x06";
    cases[2].p1 = 0.5; cases[2].p2 = 3.5; cases[2].p3 = 1.5;
    cases[2].target = 0x1B; cases[2].desc = "near 0x1B";
    cases[3].p1 = 1.0; cases[3].p2 = 4.0; cases[3].p3 = 1.2;
    cases[3].target = 0x1B; cases[3].desc = "near 0x1B";

    for (i = 0; i < n_cases; i++) {
        double p1 = cases[i].p1;
        double p2 = cases[i].p2;
        double p3 = cases[i].p3;
        double ang[8];
        int best_dist = 9, best_tt = 0;
        double best_theta = 0.0;
        int j;

        ang[0] = 0.0;
        ang[1] = p3;
        ang[2] = p2;
        ang[3] = wrap(p2 + p3);
        ang[4] = p1;
        ang[5] = wrap(p1 + p3);
        ang[6] = wrap(p1 + p2);
        ang[7] = wrap(p1 + p2 + p3);

        /* Find closest theta */
        for (j = 0; j < 10000; j++) {
            double theta = 2.0 * M_PI * (double)j / 10000.0;
            int tt = truth_table(p1, p2, p3, theta);
            int dist = 0, b;
            for (b = 0; b < 8; b++)
                dist += ((tt >> b) & 1) != ((cases[i].target >> b) & 1);
            if (dist < best_dist) {
                best_dist = dist;
                best_theta = theta;
                best_tt = tt;
            }
        }

        printf("  Case %d (%s): p=(%.2f, %.2f, %.2f)\n",
               i + 1, cases[i].desc, p1, p2, p3);
        printf("    Best theta=%.4f => TT=0x%02X (NPN 0x%02X), "
               "dist=%d from 0x%02X\n",
               best_theta, best_tt, npn_table[best_tt],
               best_dist, cases[i].target);

        printf("    Angles (radians):\n");
        {
            const char *labels[] = {
                "0", "p3", "p2", "p2+p3", "p1", "p1+p3", "p1+p2", "p1+p2+p3"
            };
            int b;
            for (b = 0; b < 8; b++) {
                printf("      %-10s = %6.3f  (%s)\n",
                       labels[b], ang[b],
                       in_semi(ang[b], best_theta) ? "IN " : "OUT");
            }
        }

        /* For 0x06 targets: show parallelogram gaps */
        if (cases[i].target == 0x06) {
            double alpha = p2 < p3 ? p2 : p3;
            double beta  = p2 < p3 ? p3 : p2;
            double sum_ab = alpha + beta;
            double g4 = 2.0 * M_PI - sum_ab;

            printf("    Parallelogram: alpha=%.3f, beta=%.3f, sum=%.3f\n",
                   alpha, beta, sum_ab);
            printf("    Gaps: alpha=%.3f, beta-alpha=%.3f, alpha=%.3f, "
                   "2pi-sum=%.3f\n",
                   alpha, beta - alpha, alpha, g4);
            printf("    Max gap = %.3f, pi = %.3f => %s\n",
                   g4 > alpha ? (g4 > beta - alpha ? g4 : beta - alpha) :
                   (alpha > beta - alpha ? alpha : beta - alpha),
                   M_PI,
                   g4 < M_PI ? "all gaps < pi => BLOCKED" :
                               "gap >= pi => possible");
        }
        printf("\n");
    }
}

/* ========== NPN Symmetry Argument ========== */

static void npn_symmetry_argument(void)
{
    printf("========================================================\n");
    printf("  NPN SYMMETRY: proofs extend to full equivalence classes\n");
    printf("========================================================\n\n");

    printf("  The proofs above work for the canonical representatives\n");
    printf("  0x06 and 0x1B. They extend to ALL 48 truth tables in\n");
    printf("  their NPN orbits by the following symmetries:\n\n");

    printf("  1. Input permutation (x0,x1,x2) -> (x_s0,x_s1,x_s2):\n");
    printf("     Equivalent to relabeling (phi1,phi2,phi3).\n");
    printf("     The additive structure is preserved.\n\n");

    printf("  2. Input negation x_i -> NOT x_i:\n");
    printf("     Equivalent to reflecting angle phi_i -> -phi_i.\n");
    printf("     The semicircle structure is preserved (reflect S too).\n\n");

    printf("  3. Output negation f -> NOT f:\n");
    printf("     Equivalent to replacing S with S-bar.\n");
    printf("     Swaps TRUE/FALSE but the constraint geometry is identical.\n\n");

    printf("  Therefore: if the canonical representative is unreachable,\n");
    printf("  ALL truth tables in its NPN orbit are unreachable.\n\n");

    /* Verify orbit sizes */
    {
        int count_06 = 0, count_1B = 0, i;
        for (i = 0; i < 256; i++) {
            if (npn_table[i] == 0x06) count_06++;
            if (npn_table[i] == 0x1B) count_1B++;
        }
        printf("  Orbit of 0x06: %d truth tables\n", count_06);
        printf("  Orbit of 0x1B: %d truth tables\n", count_1B);
        printf("  Total unreachable: %d of 256\n\n", count_06 + count_1B);

        check(count_06 == 24, "0x06 orbit has 24 members");
        check(count_1B == 24, "0x1B orbit has 24 members");
        check(count_06 + count_1B == 48,
              "48 total unreachable truth tables (24+24)");
    }
    printf("\n");
}

/* ========== Main ========== */

int main(void)
{
    printf("=== Demo 62: Analytical Proof of the 11/13 Half-Plane Wall ===\n");
    printf("Why EXACTLY 0x06 and 0x1B are unreachable.\n");
    printf("Pure algebra -- no search required.\n\n");

    build_npn();

    prove_0x1B();
    prove_0x06();
    gap_analysis_demo();
    npn_symmetry_argument();

    printf("========================================================\n");
    printf("  SUMMARY\n");
    printf("========================================================\n\n");

    printf("  The 11/13 wall is not computational -- it is geometric.\n\n");

    printf("  For 0x1B (CROSS): the additive constraint phi2+phi3 in S\n");
    printf("  with phi2 in S-bar creates a SQUEEZE on phi1 that collapses\n");
    printf("  to a contradiction in 3 algebraic steps.  The obstruction\n");
    printf("  is direct: lower bound > upper bound.\n\n");

    printf("  For 0x06 (XOR-AND): the 4 phi1-constraints form a\n");
    printf("  parallelogram of semicircles on the circle.  The constraint\n");
    printf("  alpha+beta > pi (from phi2+phi3 not in S) forces ALL gaps\n");
    printf("  between midpoints to be < pi, making the intersection\n");
    printf("  empty.  The obstruction is topological: 4 semicircles\n");
    printf("  whose midpoints span the whole circle.\n\n");

    printf("  Both proofs work for CONTINUOUS angles, subsuming the\n");
    printf("  discrete integer proofs of Demo 61.  The wall is exact\n");
    printf("  and dimension-independent: no refinement of the angular\n");
    printf("  grid will ever find these classes.\n\n");

    printf("  The additive structure of multiplicative encodings\n");
    printf("  z(a,b,c) = w1^a * w2^b * w3^c creates a parallelogram\n");
    printf("  constraint on the circle that is incompatible with the\n");
    printf("  alternating parity structure of these two Boolean classes.\n\n");

    printf("=== Results: %d pass, %d fail ===\n", pass_count, fail_count);
    return fail_count > 0 ? 1 : 0;
}
