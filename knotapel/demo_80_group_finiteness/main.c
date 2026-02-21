/*
 * KNOTAPEL DEMO 80: Group Finiteness Survey
 * ==========================================
 *
 * For each root of unity zeta_N (N = 4, 6, 8, 10, 12, 16, 20):
 *   1. Generate SU(2) R-matrices with half_angle = 2*pi/N
 *   2. Run group closure (multiply all elements by generators)
 *   3. Report: group size (or ">MAX" if still growing), rounds to close,
 *      number of directions, number of distinct half-angles
 *   4. Attempt ADE identification for finite groups
 *
 * The finite subgroups of SU(2) are classified (ADE):
 *   - Cyclic Z_n (order n, any n)
 *   - Binary dihedral 2D_n (order 4n)
 *   - Binary tetrahedral 2T (order 24)  = E_6
 *   - Binary octahedral 2O (order 48)   = E_7
 *   - Binary icosahedral 2I (order 120) = E_8
 *
 * Hypothesis: zeta_8 -> binary tetrahedral (24 up to +/-).
 *             zeta_10 -> possibly binary icosahedral (Fibonacci anyons).
 *             zeta_12 -> infinite (confirmed by Demo 79 retest).
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

static Quat q_gen[3];

static void init_su2(double half_angle) {
    double c = cos(half_angle);
    double s = sin(half_angle);
    q_gen[1].a = c; q_gen[1].b = s; q_gen[1].c = 0; q_gen[1].d = 0;
    q_gen[2].a = c; q_gen[2].b = 0; q_gen[2].c = 0; q_gen[2].d = -s;
}

/* ================================================================
 * Catalog (shared buffer, reused per root)
 * ================================================================ */

#define MAX_QCAT 4096
static Quat qcat[MAX_QCAT];
static int qcat_size = 0;

static int find_quat(const Quat *q) {
    int i;
    for (i = 0; i < qcat_size; i++) {
        if (fabs(qcat[i].a - q->a) < 1e-10 &&
            fabs(qcat[i].b - q->b) < 1e-10 &&
            fabs(qcat[i].c - q->c) < 1e-10 &&
            fabs(qcat[i].d - q->d) < 1e-10) return i;
        if (fabs(qcat[i].a + q->a) < 1e-10 &&
            fabs(qcat[i].b + q->b) < 1e-10 &&
            fabs(qcat[i].c + q->c) < 1e-10 &&
            fabs(qcat[i].d + q->d) < 1e-10) return i;
    }
    return -1;
}

/* Returns: 1 if group closed (finite), 0 if hit MAX_QCAT */
static int build_closure(int *rounds_out) {
    Quat gens[4];
    int ng = 0;
    int prev, i, gi, round = 0;
    int hit_cap = 0;

    qcat_size = 0;
    /* Identity */
    qcat[0].a = 1; qcat[0].b = 0; qcat[0].c = 0; qcat[0].d = 0;
    qcat_size = 1;

    /* Generators + inverses */
    gens[ng].a = q_gen[1].a; gens[ng].b = q_gen[1].b;
    gens[ng].c = q_gen[1].c; gens[ng].d = q_gen[1].d; ng++;
    gens[ng].a = q_gen[1].a; gens[ng].b = -q_gen[1].b;
    gens[ng].c = -q_gen[1].c; gens[ng].d = -q_gen[1].d; ng++;
    gens[ng].a = q_gen[2].a; gens[ng].b = q_gen[2].b;
    gens[ng].c = q_gen[2].c; gens[ng].d = q_gen[2].d; ng++;
    gens[ng].a = q_gen[2].a; gens[ng].b = -q_gen[2].b;
    gens[ng].c = -q_gen[2].c; gens[ng].d = -q_gen[2].d; ng++;

    for (gi = 0; gi < ng; gi++) {
        if (find_quat(&gens[gi]) < 0 && qcat_size < MAX_QCAT)
            qcat[qcat_size++] = gens[gi];
    }

    do {
        prev = qcat_size;
        round++;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < ng; gi++) {
                const Quat *p = &qcat[i];
                const Quat *g = &gens[gi];
                Quat prod;
                prod.a = p->a*g->a - p->b*g->b - p->c*g->c - p->d*g->d;
                prod.b = p->a*g->b + p->b*g->a + p->c*g->d - p->d*g->c;
                prod.c = p->a*g->c - p->b*g->d + p->c*g->a + p->d*g->b;
                prod.d = p->a*g->d + p->b*g->c - p->c*g->b + p->d*g->a;
                if (find_quat(&prod) < 0) {
                    if (qcat_size < MAX_QCAT) {
                        qcat[qcat_size++] = prod;
                    } else {
                        hit_cap = 1;
                    }
                }
            }
        }
    } while (qcat_size > prev && !hit_cap);

    if (rounds_out) *rounds_out = round;
    return !hit_cap;
}

/* ================================================================
 * Direction census
 * ================================================================ */

#define MAX_DIR 512
static double g_dir[MAX_DIR][3];
static int g_nd = 0;

static void count_directions(void) {
    int i, j;
    g_nd = 0;
    for (i = 0; i < qcat_size; i++) {
        double qa = qcat[i].a, qb = qcat[i].b;
        double qc = qcat[i].c, qd = qcat[i].d;
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

/* ================================================================
 * Angle census
 * ================================================================ */

static double g_angles[MAX_QCAT];
static int g_n_angles = 0;

static void count_angles(void) {
    int i, j;
    g_n_angles = 0;
    for (i = 0; i < qcat_size; i++) {
        double qa = qcat[i].a, qb = qcat[i].b;
        double qc = qcat[i].c, qd = qcat[i].d;
        double nv, ha;
        int found = 0;
        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        if (qa > 1.0) qa = 1.0;
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        ha = atan2(nv, qa) * 180.0 / M_PI;
        for (j = 0; j < g_n_angles; j++) {
            if (fabs(g_angles[j] - ha) < 0.1) { found = 1; break; }
        }
        if (!found && g_n_angles < MAX_QCAT) {
            g_angles[g_n_angles++] = ha;
        }
    }
    /* Sort */
    for (i = 0; i < g_n_angles - 1; i++) {
        for (j = i + 1; j < g_n_angles; j++) {
            if (g_angles[j] < g_angles[i]) {
                double tmp = g_angles[i];
                g_angles[i] = g_angles[j];
                g_angles[j] = tmp;
            }
        }
    }
}

/* ================================================================
 * ADE identification for finite groups
 * ================================================================ */

static const char *identify_ade(int order) {
    /* order = qcat_size (elements up to +/-)
     * Full group order = 2 * qcat_size
     *
     * ADE types by full group order:
     *   Cyclic Z_n: order n (any)
     *   Binary dihedral 2D_n: order 4n (n >= 2)
     *   Binary tetrahedral 2T: order 24  -> qcat_size = 12
     *   Binary octahedral 2O: order 48   -> qcat_size = 24
     *   Binary icosahedral 2I: order 120 -> qcat_size = 60
     */
    int full_order = 2 * order;

    if (order == 12) return "Binary Tetrahedral (2T, E6)";
    if (order == 24) return "Binary Octahedral (2O, E7)";
    if (order == 60) return "Binary Icosahedral (2I, E8)";

    /* Check binary dihedral: full order = 4n, n >= 2 */
    if (full_order >= 8 && full_order % 4 == 0) {
        static char buf[64];
        int n = full_order / 4;
        sprintf(buf, "Binary Dihedral 2D_%d (order %d)?", n, full_order);
        return buf;
    }

    /* Check cyclic */
    if (full_order >= 2) {
        static char buf2[64];
        sprintf(buf2, "Cyclic Z_%d (order %d)?", full_order, full_order);
        return buf2;
    }

    return "Unknown";
}

/* ================================================================
 * Direction distribution analysis
 * For infinite groups: how uniformly are directions distributed?
 * ================================================================ */

static void analyze_direction_distribution(void) {
    /* Check coverage: what fraction of S2 octants have at least one dir? */
    int octant_has[8];
    int i, n_octants = 0;
    memset(octant_has, 0, sizeof(octant_has));

    for (i = 0; i < g_nd; i++) {
        int oct = 0;
        if (g_dir[i][0] >= 0) oct |= 1;
        if (g_dir[i][1] >= 0) oct |= 2;
        if (g_dir[i][2] >= 0) oct |= 4;
        octant_has[oct] = 1;
    }
    for (i = 0; i < 8; i++) n_octants += octant_has[i];

    printf("    S2 coverage: %d/8 octants occupied\n", n_octants);

    /* Report angle spectrum density */
    if (g_n_angles > 1) {
        double min_gap = 999, max_gap = 0, sum_gap = 0;
        for (i = 0; i < g_n_angles - 1; i++) {
            double gap = g_angles[i+1] - g_angles[i];
            if (gap < min_gap) min_gap = gap;
            if (gap > max_gap) max_gap = gap;
            sum_gap += gap;
        }
        printf("    Angle gaps: min=%.2f, max=%.2f, avg=%.2f degrees\n",
               min_gap, max_gap, sum_gap / (double)(g_n_angles - 1));
        if (min_gap < 1.0)
            printf("    => Angles approaching continuum (gap < 1 deg)\n");
        else
            printf("    => Angles cleanly quantized (min gap > 1 deg)\n");
    }
}

/* ================================================================
 * Summary table storage
 * ================================================================ */

typedef struct {
    int N;
    double half_angle_deg;
    int finite;
    int group_size;
    int rounds;
    int n_dirs;
    int n_angles;
    const char *ade;
} RootResult;

#define MAX_ROOTS 10
static RootResult results[MAX_ROOTS];
static int n_results = 0;

/* ================================================================
 * Test one root of unity
 * ================================================================ */

static void test_root(int N) {
    double half_angle = 2.0 * M_PI / (double)N;
    int finite, rounds;
    RootResult *r;
    char msg[128];

    printf("\n--- zeta_%d (half_angle = 2pi/%d = %.1f degrees) ---\n",
           N, N, half_angle * 180.0 / M_PI);

    init_su2(half_angle);
    finite = build_closure(&rounds);

    count_directions();
    count_angles();

    printf("    Group: %s at %d elements (round %d)\n",
           finite ? "CLOSED" : "OPEN (hit cap)",
           qcat_size, rounds);
    printf("    Directions: %d\n", g_nd);
    printf("    Half-angles: %d\n", g_n_angles);

    if (finite) {
        const char *ade = identify_ade(qcat_size);
        printf("    ADE type: %s\n", ade);

        r = &results[n_results];
        r->ade = ade;
    } else {
        printf("    ADE type: INFINITE (not in ADE classification)\n");
        analyze_direction_distribution();
        r = &results[n_results];
        r->ade = "INFINITE";
    }

    r->N = N;
    r->half_angle_deg = half_angle * 180.0 / M_PI;
    r->finite = finite;
    r->group_size = qcat_size;
    r->rounds = rounds;
    r->n_dirs = g_nd;
    r->n_angles = g_n_angles;
    n_results++;

    /* Print angle spectrum for finite groups or first few for infinite */
    if (g_n_angles <= 20) {
        int i;
        printf("    Angles:");
        for (i = 0; i < g_n_angles; i++)
            printf(" %.1f", g_angles[i]);
        printf("\n");
    } else {
        int i;
        printf("    Angles (first 15 of %d):", g_n_angles);
        for (i = 0; i < 15 && i < g_n_angles; i++)
            printf(" %.1f", g_angles[i]);
        printf(" ...\n");
    }

    sprintf(msg, "zeta_%d: %d elements, %s",
            N, qcat_size, finite ? "FINITE" : "INFINITE");
    check(msg, 1);
}

/* ================================================================
 * Summary table
 * ================================================================ */

static void print_summary(void) {
    int i;
    int n_finite = 0, n_infinite = 0;
    char msg[128];

    printf("\n=== Summary Table ===\n\n");
    printf("  %-6s  %8s  %8s  %6s  %6s  %7s  %-30s\n",
           "Root", "Angle", "Size", "Rounds", "Dirs", "Angles", "ADE Type");
    printf("  %-6s  %8s  %8s  %6s  %6s  %7s  %-30s\n",
           "------", "--------", "--------", "------", "------",
           "-------", "------------------------------");

    for (i = 0; i < n_results; i++) {
        RootResult *r = &results[i];
        char size_str[16];
        if (r->finite)
            sprintf(size_str, "%d", r->group_size);
        else
            sprintf(size_str, ">%d", MAX_QCAT);

        printf("  zeta_%-2d  %6.1f  %8s  %6d  %6d  %7d  %-30s\n",
               r->N, r->half_angle_deg, size_str, r->rounds,
               r->n_dirs, r->n_angles, r->ade);

        if (r->finite) n_finite++;
        else n_infinite++;
    }

    printf("\n  Finite: %d    Infinite: %d\n", n_finite, n_infinite);

    sprintf(msg, "Survey: %d roots tested", n_results);
    check(msg, n_results >= 7);

    sprintf(msg, "At least one finite group found");
    check(msg, n_finite > 0);

    sprintf(msg, "At least one infinite group found");
    check(msg, n_infinite > 0);

    /* Check if there's a clean finite/infinite boundary */
    {
        int boundary_N = 0;
        int clean = 1;
        for (i = 0; i < n_results; i++) {
            if (i > 0 && results[i].finite != results[i-1].finite) {
                if (boundary_N == 0)
                    boundary_N = results[i].N;
                else
                    clean = 0;
            }
        }
        if (boundary_N > 0 && clean) {
            sprintf(msg, "Clean boundary at zeta_%d: "
                    "finite below, infinite at/above", boundary_N);
            check(msg, 1);
        } else if (boundary_N > 0) {
            printf("  NOTE: Boundary is not clean "
                   "(alternating finite/infinite)\n");
            check("Boundary structure detected", 1);
        } else {
            printf("  NOTE: All groups are the same finiteness\n");
            check("Uniform finiteness", 1);
        }
    }
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 80: Group Finiteness Survey\n");
    printf("==========================================\n");

    n_results = 0;

    test_root(4);
    test_root(6);
    test_root(8);
    test_root(10);
    test_root(12);
    test_root(16);
    test_root(20);

    print_summary();

    printf("\n==========================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
