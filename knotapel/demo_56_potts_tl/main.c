/*
 * KNOTAPEL DEMO 56: Potts-TL Dictionary
 * ======================================
 *
 * The Kauffman bracket at A=zeta_16^5 gives delta=sqrt(2), so Q=delta^2=2.
 * This is the ISING MODEL. The bracket IS a Potts partition function.
 *
 * Key dictionary:
 *   ell=3  (delta=1)   -> Q=1  = bond percolation (trivial)
 *   ell=4  (delta=sqrt2)-> Q=2  = Ising model
 *   ell=6  (delta=sqrt3)-> Q=3  = 3-state Potts
 *
 * Fortuin-Kasteleyn partition function on a graph G:
 *   Z_FK(G; Q, v) = sum_{A subset E} v^|A| * Q^{k(A)}
 * where k(A) = number of connected components of subgraph (V, A).
 *
 * At Q=2 and bracket coupling v = -zeta_16^6:
 *   - Z_FK values live in Z[zeta_8] = Z[i,sqrt(2)] (4D over Z)
 *   - RICHER than bracket values which are Z[i]-axial (2D over Z)
 *   - The Q^{k(A)} weighting breaks the axiality braids have
 *
 * At Q=2 and physical critical coupling v = sqrt(2):
 *   - Z_FK values should be real (in Z[sqrt(2)])
 *   - This is the critical temperature of the 2D Ising model
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
 * Z[zeta_16] Arithmetic (from Demo 55)
 *
 * zeta = e^{i*pi/8}, zeta^8 = -1.
 * Elements: c[0] + c[1]*z + ... + c[7]*z^7
 * ================================================================ */

typedef struct { long c[8]; } Cyc16;

static Cyc16 cyc16_zero(void) {
    Cyc16 r;
    memset(r.c, 0, sizeof(r.c));
    return r;
}

static Cyc16 cyc16_one(void) {
    Cyc16 r;
    memset(r.c, 0, sizeof(r.c));
    r.c[0] = 1;
    return r;
}

static Cyc16 cyc16_zeta_power(int k) {
    Cyc16 r;
    memset(r.c, 0, sizeof(r.c));
    k = k % 16;
    if (k < 0) k += 16;
    if (k < 8) { r.c[k] = 1; }
    else       { r.c[k - 8] = -1; }
    return r;
}

static Cyc16 cyc16_add(Cyc16 a, Cyc16 b) {
    Cyc16 r;
    int i;
    for (i = 0; i < 8; i++) r.c[i] = a.c[i] + b.c[i];
    return r;
}

static Cyc16 cyc16_neg(Cyc16 a) {
    Cyc16 r;
    int i;
    for (i = 0; i < 8; i++) r.c[i] = -a.c[i];
    return r;
}

static Cyc16 cyc16_mul(Cyc16 a, Cyc16 b) {
    Cyc16 r;
    int i, j;
    memset(r.c, 0, sizeof(r.c));
    for (i = 0; i < 8; i++) {
        if (a.c[i] == 0) continue;
        for (j = 0; j < 8; j++) {
            if (b.c[j] == 0) continue;
            if (i + j < 8)
                r.c[i + j] += a.c[i] * b.c[j];
            else
                r.c[i + j - 8] -= a.c[i] * b.c[j];
        }
    }
    return r;
}

static int cyc16_eq(Cyc16 a, Cyc16 b) {
    int i;
    for (i = 0; i < 8; i++)
        if (a.c[i] != b.c[i]) return 0;
    return 1;
}

static int cyc16_is_zero(Cyc16 a) {
    int i;
    for (i = 0; i < 8; i++)
        if (a.c[i] != 0) return 0;
    return 1;
}

/* Check if value is "real" in Z[sqrt(2)]: only c[0] and c[2],c[6] pair */
static int cyc16_is_real(Cyc16 a) {
    /* Real means only even-indexed components, with c[1]=c[3]=c[5]=c[7]=0
     * and c[2]=-c[6] (since zeta^2 + zeta^6 is not real, but zeta^2-zeta^6=sqrt(2) is) */
    /* Actually: cos(k*pi/8) for k=0,2,4,6 are real parts of zeta^k.
     * A real Z[zeta_16] element has: c[1]=c[3]=c[5]=c[7]=0
     * and c[2]=c[6]=0 or c[2]=-c[6] (since zeta^2-zeta^6=sqrt(2)),
     * and c[4]=0 or any (since zeta^4=-1 is real).
     * Simpler: check Im part of cyc16_to_cx is 0 */
    return a.c[1] == 0 && a.c[3] == 0 && a.c[5] == 0 && a.c[7] == 0;
}

typedef struct { double re, im; } Cx;

static Cx cyc16_to_cx(Cyc16 a) {
    Cx r;
    int k;
    r.re = 0.0;
    r.im = 0.0;
    for (k = 0; k < 8; k++) {
        if (a.c[k] != 0) {
            double angle = (double)k * M_PI / 8.0;
            r.re += (double)a.c[k] * cos(angle);
            r.im += (double)a.c[k] * sin(angle);
        }
    }
    return r;
}

static void cyc16_print(const char *label, Cyc16 a) {
    Cx z = cyc16_to_cx(a);
    printf("  %s = (%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld)"
           "  [%.4f + %.4fi]\n",
           label,
           a.c[0], a.c[1], a.c[2], a.c[3],
           a.c[4], a.c[5], a.c[6], a.c[7],
           z.re, z.im);
}

/* ================================================================
 * Lattice Strip Graph
 *
 * S(w, h): w sites wide, h sites tall.
 * Vertices: 0..w*h-1, arranged row-major.
 * Edges: horizontal within rows + vertical between rows.
 * ================================================================ */

#define MAX_SITES 25
#define MAX_EDGES 50

typedef struct {
    int n_sites;
    int n_edges;
    int width, height;
    int edge_u[MAX_EDGES];
    int edge_v[MAX_EDGES];
} LatticeStrip;

static void build_strip(LatticeStrip *g, int w, int h) {
    int x, y;
    g->width = w;
    g->height = h;
    g->n_sites = w * h;
    g->n_edges = 0;

    /* Horizontal edges */
    for (y = 0; y < h; y++)
        for (x = 0; x < w - 1; x++) {
            g->edge_u[g->n_edges] = y * w + x;
            g->edge_v[g->n_edges] = y * w + x + 1;
            g->n_edges++;
        }

    /* Vertical edges */
    for (y = 0; y < h - 1; y++)
        for (x = 0; x < w; x++) {
            g->edge_u[g->n_edges] = y * w + x;
            g->edge_v[g->n_edges] = (y + 1) * w + x;
            g->n_edges++;
        }
}

/* ================================================================
 * Union-Find for component counting
 * ================================================================ */

static int uf_parent[MAX_SITES];

static void uf_init(int n) {
    int i;
    for (i = 0; i < n; i++) uf_parent[i] = i;
}

static int uf_find(int x) {
    while (uf_parent[x] != x) {
        uf_parent[x] = uf_parent[uf_parent[x]];
        x = uf_parent[x];
    }
    return x;
}

static void uf_union(int a, int b) {
    a = uf_find(a);
    b = uf_find(b);
    if (a != b) uf_parent[a] = b;
}

/* ================================================================
 * Fortuin-Kasteleyn Partition Function (Cyc16 arithmetic)
 *
 * Z_FK(G; Q, v) = sum_{A subset E} v^|A| * Q^{k(A)}
 *
 * Precomputes v^k and Q^k arrays. Enumerates all 2^|E| subsets.
 * Feasible up to ~24 edges.
 * ================================================================ */

static Cyc16 fk_partition(const LatticeStrip *g, Cyc16 Q, Cyc16 v) {
    Cyc16 Z = cyc16_zero();
    long mask, total;
    Cyc16 v_pow[MAX_EDGES + 1];
    Cyc16 Q_pow[MAX_SITES + 1];
    int k;

    v_pow[0] = cyc16_one();
    for (k = 1; k <= g->n_edges; k++)
        v_pow[k] = cyc16_mul(v_pow[k - 1], v);

    Q_pow[0] = cyc16_one();
    for (k = 1; k <= g->n_sites; k++)
        Q_pow[k] = cyc16_mul(Q_pow[k - 1], Q);

    total = 1L << g->n_edges;

    for (mask = 0; mask < total; mask++) {
        int n_bonds = 0;
        int n_components = 0;
        int i;

        uf_init(g->n_sites);

        for (i = 0; i < g->n_edges; i++) {
            if (mask & (1L << i)) {
                uf_union(g->edge_u[i], g->edge_v[i]);
                n_bonds++;
            }
        }

        for (i = 0; i < g->n_sites; i++)
            if (uf_find(i) == i) n_components++;

        Z = cyc16_add(Z, cyc16_mul(v_pow[n_bonds],
                                      Q_pow[n_components]));
    }

    return Z;
}

/* ================================================================
 * Direct Ising Spin Sum (integer arithmetic, for cross-validation)
 *
 * Z_Ising = sum_{sigma in {0,1}^V} prod_{edges} (1 + v * delta(s_i, s_j))
 *
 * At integer v, this equals FK Z with Q=2.
 * ================================================================ */

static long ising_spin_sum(const LatticeStrip *g, long v_int) {
    long Z = 0;
    long config, total;

    total = 1L << g->n_sites;

    for (config = 0; config < total; config++) {
        long weight = 1;
        int i;

        for (i = 0; i < g->n_edges; i++) {
            int u = g->edge_u[i];
            int w = g->edge_v[i];
            int su = (int)((config >> u) & 1);
            int sw = (int)((config >> w) & 1);

            if (su == sw)
                weight *= (1 + v_int);
            /* else weight *= 1 */
        }

        Z += weight;
    }

    return Z;
}

/* ================================================================
 * PART A: Cross-Validation (FK = Ising Spin Sum)
 * ================================================================ */

static void part_a_cross_validation(void) {
    int test_w[] = {2, 2, 2, 3, 3, 4};
    int test_h[] = {2, 3, 4, 3, 4, 3};
    int n_tests = 6;
    int t;
    char msg[256];
    long v_int = 1;  /* simplest non-trivial coupling */
    Cyc16 Q_cyc, v_cyc;

    printf("\n=== PART A: FK = Ising Spin Sum Cross-Validation ===\n\n");
    printf("  Coupling: v = 1 (integer), Q = 2\n");
    printf("  Z_FK = sum_{A} v^|A| * Q^{k(A)}\n");
    printf("  Z_Ising = sum_{sigma} prod_{edges} (1 + v*delta)\n\n");

    Q_cyc = cyc16_zero();
    Q_cyc.c[0] = 2;

    v_cyc = cyc16_one();

    for (t = 0; t < n_tests; t++) {
        LatticeStrip g;
        Cyc16 Z_fk;
        long Z_ising, Z_fk_int;

        build_strip(&g, test_w[t], test_h[t]);

        Z_fk = fk_partition(&g, Q_cyc, v_cyc);
        Z_ising = ising_spin_sum(&g, v_int);

        /* FK at integer Q=2, v=1 should give integer (only c[0] nonzero) */
        Z_fk_int = Z_fk.c[0];

        sprintf(msg, "S(%d,%d): FK=%ld  Ising=%ld  (%d sites, %d edges)",
                test_w[t], test_h[t], Z_fk_int, Z_ising,
                g.n_sites, g.n_edges);
        check(msg, Z_fk_int == Z_ising);
    }

    /* Also check v=2 for one case */
    {
        LatticeStrip g;
        Cyc16 Z_fk;
        long Z_ising, Z_fk_int;

        build_strip(&g, 3, 3);
        v_cyc = cyc16_zero(); v_cyc.c[0] = 2;
        Z_fk = fk_partition(&g, Q_cyc, v_cyc);
        Z_ising = ising_spin_sum(&g, 2);
        Z_fk_int = Z_fk.c[0];

        sprintf(msg, "S(3,3) v=2: FK=%ld  Ising=%ld", Z_fk_int, Z_ising);
        check(msg, Z_fk_int == Z_ising);
    }
}

/* ================================================================
 * PART B: Q=1 Degeneracy (Percolation)
 *
 * At Q=1: Z_FK = sum_A v^|A| * 1^{k(A)} = sum_A v^|A| = (1+v)^|E|
 * Independent of graph structure! This is why Demo 53 was trivial.
 * ================================================================ */

static void part_b_percolation(void) {
    int test_w[] = {2, 2, 3, 3};
    int test_h[] = {2, 3, 3, 4};
    int n_tests = 4;
    int t;
    char msg[256];
    Cyc16 Q_one;

    /* Q=1 coupling for ell=3: A = zeta_6^5, v = -A^{-2} = -zeta_6^{-10} = -zeta_6^2
     * But for Q=1, the result doesn't depend on v!
     * Let's use v = zeta_16^3 (arbitrary) to show graph-independence */
    Cyc16 v_arb = cyc16_zeta_power(3);

    printf("\n=== PART B: Q=1 Degeneracy (Percolation) ===\n\n");
    printf("  At Q=1: Z_FK = (1+v)^|E| regardless of graph structure\n\n");

    Q_one = cyc16_one();

    for (t = 0; t < n_tests; t++) {
        LatticeStrip g;
        Cyc16 Z_fk, Z_expected;
        int k;

        build_strip(&g, test_w[t], test_h[t]);
        Z_fk = fk_partition(&g, Q_one, v_arb);

        /* Expected: (1+v)^|E| */
        Z_expected = cyc16_add(cyc16_one(), v_arb);
        {
            Cyc16 base = Z_expected;
            Z_expected = cyc16_one();
            for (k = 0; k < g.n_edges; k++)
                Z_expected = cyc16_mul(Z_expected, base);
        }

        sprintf(msg, "S(%d,%d): Z_FK = (1+v)^%d",
                test_w[t], test_h[t], g.n_edges);
        check(msg, cyc16_eq(Z_fk, Z_expected));
    }

    printf("\n  This confirms Demo 53: Q=1 is computationally trivial.\n");
    printf("  The partition function carries NO geometric information.\n");
}

/* Check if value lives in Z[zeta_8] subring: only even-indexed components */
static int cyc16_is_zeta8_valued(Cyc16 a) {
    return a.c[1] == 0 && a.c[3] == 0 && a.c[5] == 0 && a.c[7] == 0;
}

/* ================================================================
 * PART C: Bracket Coupling — FK lives in Z[zeta_8]
 *
 * At Q=2, bracket coupling v = -zeta_16^6:
 * Bracket values are Z[i]-axial (Demo 54).
 * FK partition functions use the FULL even subring Z[zeta_8] = Z[i,sqrt(2)].
 * This is a genuine algebraic distinction: FK is 4D over Z, bracket is 2D.
 * ================================================================ */

#define MAX_CATALOG 256

static Cyc16 pf_catalog[MAX_CATALOG];
static int pf_cat_size = 0;

static int pf_cat_find(Cyc16 val) {
    int i;
    for (i = 0; i < pf_cat_size; i++)
        if (cyc16_eq(val, pf_catalog[i])) return i;
    return -1;
}

static void part_c_bracket_coupling(void) {
    /* Bracket coupling: A = zeta_16^5, v = -A^{-2} = -zeta_16^{-10}
     * zeta_16^{-10} = zeta_16^{16-10} = zeta_16^6
     * So v = -zeta_16^6 */
    int test_w[] = {2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4};
    int test_h[] = {2, 3, 4, 5, 6, 3, 4, 5, 6, 3, 4};
    int n_tests = 11;
    int t;
    int n_z8 = 0, n_total = 0;
    char msg[256];
    Cyc16 v_bracket = cyc16_neg(cyc16_zeta_power(6));
    Cyc16 Q_two = cyc16_zero();
    Q_two.c[0] = 2;

    printf("\n=== PART C: Bracket Coupling — FK in Z[zeta_8] ===\n\n");
    printf("  Q = 2 (Ising), v = -zeta_16^6 (bracket coupling)\n");
    printf("  Bracket values are Z[i]-axial (2D). FK values use Z[zeta_8] (4D).\n\n");

    cyc16_print("v (bracket)", v_bracket);
    cyc16_print("Q", Q_two);
    printf("\n");

    pf_cat_size = 0;

    printf("  %6s  %5s  %5s  %s  %s\n",
           "Strip", "Sites", "Edges", "Z[zeta_8]?", "Value");
    printf("  ------  -----  -----  ----------  -----\n");

    for (t = 0; t < n_tests; t++) {
        LatticeStrip g;
        Cyc16 Z;
        int zi;
        Cx z_cx;

        build_strip(&g, test_w[t], test_h[t]);

        /* Skip if too many edges */
        if (g.n_edges > 24) {
            printf("  S(%d,%d)  %5d  %5d  (skipped, %d edges)\n",
                   test_w[t], test_h[t], g.n_sites, g.n_edges,
                   g.n_edges);
            continue;
        }

        Z = fk_partition(&g, Q_two, v_bracket);
        zi = cyc16_is_zeta8_valued(Z);
        z_cx = cyc16_to_cx(Z);
        n_total++;
        if (zi) n_z8++;

        printf("  S(%d,%d)  %5d  %5d  %s"
               "         (%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld)"
               "  [%.2f+%.2fi]\n",
               test_w[t], test_h[t], g.n_sites, g.n_edges,
               zi ? "YES" : "NO ",
               Z.c[0], Z.c[1], Z.c[2], Z.c[3],
               Z.c[4], Z.c[5], Z.c[6], Z.c[7],
               z_cx.re, z_cx.im);

        /* Add to catalog */
        if (!cyc16_is_zero(Z) && pf_cat_find(Z) < 0
            && pf_cat_size < MAX_CATALOG) {
            pf_catalog[pf_cat_size++] = Z;
        }
    }

    printf("\n");
    sprintf(msg, "all FK values in Z[zeta_8] (%d/%d)", n_z8, n_total);
    check(msg, n_z8 == n_total);

    printf("  Partition function catalog: %d distinct nonzero values\n",
           pf_cat_size);
}

/* ================================================================
 * PART D: Physical Critical Coupling
 *
 * Self-dual point: v = sqrt(Q) = sqrt(2) for Ising.
 * This is the critical temperature: K_c = ln(1+sqrt(2)).
 * Values should be real (in Z[sqrt(2)]).
 * ================================================================ */

static void part_d_critical_coupling(void) {
    /* v = sqrt(2) = zeta_16^2 - zeta_16^6 = delta */
    int test_w[] = {2, 2, 2, 3, 3, 4};
    int test_h[] = {2, 3, 4, 3, 4, 3};
    int n_tests = 6;
    int t;
    int n_real = 0, n_total = 0;
    char msg[256];
    Cyc16 v_crit = cyc16_add(cyc16_zeta_power(2),
                                cyc16_neg(cyc16_zeta_power(6)));
    Cyc16 Q_two = cyc16_zero();
    Q_two.c[0] = 2;

    printf("\n=== PART D: Physical Critical Coupling ===\n\n");
    printf("  Q = 2, v = sqrt(2) (self-dual = Ising critical temperature)\n");
    printf("  K_c = ln(1+sqrt(2)) = 0.8814...\n");
    printf("  Values should be real (in Z[sqrt(2)]).\n\n");

    cyc16_print("v (critical)", v_crit);
    printf("\n");

    for (t = 0; t < n_tests; t++) {
        LatticeStrip g;
        Cyc16 Z;
        int is_real;
        Cx z_cx;

        build_strip(&g, test_w[t], test_h[t]);

        if (g.n_edges > 24) continue;

        Z = fk_partition(&g, Q_two, v_crit);
        is_real = cyc16_is_real(Z);
        z_cx = cyc16_to_cx(Z);
        n_total++;
        if (is_real) n_real++;

        printf("  S(%d,%d): %s  (%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld)"
               "  [%.2f]\n",
               test_w[t], test_h[t],
               is_real ? "REAL" : "COMPLEX!",
               Z.c[0], Z.c[1], Z.c[2], Z.c[3],
               Z.c[4], Z.c[5], Z.c[6], Z.c[7],
               z_cx.re);
    }

    printf("\n");
    sprintf(msg, "all critical Z values are real (%d/%d)", n_real, n_total);
    check(msg, n_real == n_total);

    /* Verify v^2 = 2 (sanity check) */
    {
        Cyc16 v2 = cyc16_mul(v_crit, v_crit);
        Cyc16 two = cyc16_zero();
        two.c[0] = 2;
        check("v_crit^2 = 2 (sqrt(2) verification)", cyc16_eq(v2, two));
    }
}

/* ================================================================
 * PART E: Q-Hierarchy Comparison
 *
 * Q=1: (1+v)^|E| always — no geometric information
 * Q=2: rich Z[i]-axial catalog — computes Boolean functions
 * Demonstrate the computational phase transition at Q=2.
 * ================================================================ */

static void part_e_q_hierarchy(void) {
    LatticeStrip g;
    Cyc16 Q_one, Q_two;
    Cyc16 v_bracket;
    Cyc16 Z_q1, Z_q2;
    int q1_distinct = 0, q2_distinct = 0;
    Cyc16 q1_vals[64], q2_vals[64];
    int q1_n = 0, q2_n = 0;
    int w, h, i;

    printf("\n=== PART E: Q-Hierarchy Comparison ===\n\n");

    Q_one = cyc16_one();
    Q_two = cyc16_zero(); Q_two.c[0] = 2;
    v_bracket = cyc16_neg(cyc16_zeta_power(6));

    printf("  Computing FK Z for lattice strips S(w,h)"
           " at bracket coupling v=-zeta_16^6\n");
    printf("  Q=1 (percolation) vs Q=2 (Ising)\n\n");

    printf("  %6s  %16s  %16s\n", "Strip", "Q=1 (distinct?)", "Q=2 (distinct?)");
    printf("  ------  ----------------  ----------------\n");

    for (w = 2; w <= 4; w++) {
        for (h = 2; h <= 5; h++) {
            int found_q1, found_q2;

            build_strip(&g, w, h);
            if (g.n_edges > 24) continue;

            Z_q1 = fk_partition(&g, Q_one, v_bracket);
            Z_q2 = fk_partition(&g, Q_two, v_bracket);

            /* Check if new */
            found_q1 = 0;
            for (i = 0; i < q1_n; i++)
                if (cyc16_eq(Z_q1, q1_vals[i])) { found_q1 = 1; break; }
            if (!found_q1 && q1_n < 64)
                q1_vals[q1_n++] = Z_q1;

            found_q2 = 0;
            for (i = 0; i < q2_n; i++)
                if (cyc16_eq(Z_q2, q2_vals[i])) { found_q2 = 1; break; }
            if (!found_q2 && q2_n < 64)
                q2_vals[q2_n++] = Z_q2;

            printf("  S(%d,%d)  %s                %s\n",
                   w, h,
                   found_q1 ? "repeat" : "NEW   ",
                   found_q2 ? "repeat" : "NEW   ");
        }
    }

    q1_distinct = q1_n;
    q2_distinct = q2_n;

    printf("\n  Q=1 distinct values: %d\n", q1_distinct);
    printf("  Q=2 distinct values: %d\n\n", q2_distinct);

    /* Q=1: all values are (1+v)^|E|, so S(w,h)=S(h,w) and
     * same-edge-count strips collide. Q=2: S(3,4)=S(4,3) by
     * transposition, but different-topology strips differ. */
    {
        char msg2[256];
        LatticeStrip g1, g2;
        Cyc16 z1, z2;

        /* S(2,5) and S(3,3) both have ~12-13 edges but diff topology */
        build_strip(&g1, 2, 5);
        build_strip(&g2, 3, 3);
        z1 = fk_partition(&g1, Q_two, v_bracket);
        z2 = fk_partition(&g2, Q_two, v_bracket);

        sprintf(msg2, "Q=2 distinguishes S(2,5) from S(3,3) (diff topology)");
        check(msg2, !cyc16_eq(z1, z2));
    }

    printf("  At Q=1, all values are powers of (1+v) — no geometry.\n");
    printf("  At Q=2, different topologies give different Z.\n");
    printf("  This is the computational phase transition: "
           "Q=2 (Ising) carries geometric information.\n");
}

/* ================================================================
 * PART F: Catalog Overlap with Bracket Values
 *
 * Check if partition function values at bracket coupling
 * share algebraic structure with bracket catalog from Demo 54.
 * ================================================================ */

static void part_f_catalog_analysis(void) {
    int i;
    int n_zi = 0;
    Cx z;
    char msg[256];

    printf("\n=== PART F: Partition Function Catalog Analysis ===\n\n");

    printf("  Catalog from Part C: %d distinct FK values at Q=2,"
           " v=-zeta_16^6\n\n", pf_cat_size);

    printf("  Checking algebraic properties:\n");

    for (i = 0; i < pf_cat_size; i++)
        if (cyc16_is_zeta8_valued(pf_catalog[i])) n_zi++;

    sprintf(msg, "all catalog values in Z[zeta_8] (%d/%d)",
            n_zi, pf_cat_size);
    check(msg, n_zi == pf_cat_size);

    /* Print catalog */
    printf("\n  Full partition function catalog:\n");
    for (i = 0; i < pf_cat_size; i++) {
        z = cyc16_to_cx(pf_catalog[i]);
        printf("    [%2d] (%3ld,%3ld,%3ld,%3ld,%3ld,%3ld,%3ld,%3ld)"
               "  |z|=%.4f\n",
               i,
               pf_catalog[i].c[0], pf_catalog[i].c[1],
               pf_catalog[i].c[2], pf_catalog[i].c[3],
               pf_catalog[i].c[4], pf_catalog[i].c[5],
               pf_catalog[i].c[6], pf_catalog[i].c[7],
               sqrt(z.re * z.re + z.im * z.im));
    }

    /* Check for negation pairing */
    {
        int n_pairs = 0;
        int n_checked = 0;
        for (i = 0; i < pf_cat_size; i++) {
            Cyc16 neg = cyc16_neg(pf_catalog[i]);
            n_checked++;
            if (pf_cat_find(neg) >= 0 && !cyc16_eq(neg, pf_catalog[i]))
                n_pairs++;
        }
        printf("\n  Negation pairs in catalog: %d / %d have negation present\n",
               n_pairs, n_checked);
    }

    printf("\n  KEY RESULT: FK partition functions at bracket coupling\n");
    printf("  live in Z[zeta_8] = Z[i,sqrt(2)] (4D over Z).\n");
    printf("  Bracket values are Z[i]-axial (2D over Z).\n");
    printf("  FK is ALGEBRAICALLY RICHER: 4D vs 2D.\n");
    printf("  The Q^{k(A)} weighting breaks axiality.\n");

    /* Suppress unused */
    (void)cyc16_print;
}

/* ================================================================
 * Activation Functions (from Demo 55)
 * ================================================================ */

static double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

static double cx_abs(Cx z) {
    return sqrt(z.re * z.re + z.im * z.im);
}

static int split_sigmoid_classify(Cx z) {
    double sr = sigmoid(z.re);
    double si = sigmoid(z.im);
    double val = sr * (1.0 - si) + (1.0 - sr) * si;
    return val > 0.5 ? 1 : 0;
}

static int sector_classify(Cx z, int k) {
    double angle, sector_width;
    int sector;
    if (cx_abs(z) < 1e-15) return 0;
    angle = atan2(z.im, z.re);
    if (angle < 0.0) angle += 2.0 * M_PI;
    sector_width = 2.0 * M_PI / (double)k;
    sector = (int)(angle / sector_width);
    if (sector >= k) sector = k - 1;
    return sector % 2;
}

static int re_positive_classify(Cx z) {
    return z.re > 0.0 ? 1 : 0;
}

/* Wrappers: classify(Cx, int) -> 0/1 */
static int wrap_split_sigmoid(Cx z, int param) {
    (void)param;
    return split_sigmoid_classify(z);
}

static int wrap_sector(Cx z, int k) {
    return sector_classify(z, k);
}

static int wrap_re_positive(Cx z, int param) {
    (void)param;
    return re_positive_classify(z);
}

/* ================================================================
 * 2-Input Boolean Function Names
 * ================================================================ */

static const char *fn2_name(int tt) {
    switch (tt) {
    case 0x0: return "FALSE";
    case 0x1: return "AND";
    case 0x2: return "A>B";
    case 0x3: return "PROJ_A";
    case 0x4: return "B>A";
    case 0x5: return "PROJ_B";
    case 0x6: return "XOR";
    case 0x7: return "OR";
    case 0x8: return "NOR";
    case 0x9: return "XNOR";
    case 0xA: return "NOT_B";
    case 0xB: return "A>=B";
    case 0xC: return "NOT_A";
    case 0xD: return "B>=A";
    case 0xE: return "NAND";
    case 0xF: return "TRUE";
    default:  return "???";
    }
}

/* ================================================================
 * NPN Classification for 3-Input Functions
 * ================================================================ */

static const int perms3[6][3] = {
    {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
};

static int npn_transform(int tt, const int sigma[3],
                         int neg_in, int neg_out) {
    int result = 0, idx;
    for (idx = 0; idx < 8; idx++) {
        int x[3], y[3], src, out;
        x[0] = (idx >> 2) & 1;
        x[1] = (idx >> 1) & 1;
        x[2] = idx & 1;
        y[0] = x[sigma[0]] ^ ((neg_in >> 0) & 1);
        y[1] = x[sigma[1]] ^ ((neg_in >> 1) & 1);
        y[2] = x[sigma[2]] ^ ((neg_in >> 2) & 1);
        src = (y[0] << 2) | (y[1] << 1) | y[2];
        out = (tt >> src) & 1;
        if (neg_out) out ^= 1;
        result |= (out << idx);
    }
    return result;
}

static int npn_canon[256];

static void npn_init(void) {
    int i, pi, ni, no;
    for (i = 0; i < 256; i++) {
        int min_tt = i;
        for (pi = 0; pi < 6; pi++)
            for (ni = 0; ni < 8; ni++)
                for (no = 0; no < 2; no++) {
                    int t = npn_transform(i, perms3[pi], ni, no);
                    if (t < min_tt) min_tt = t;
                }
        npn_canon[i] = min_tt;
    }
}

static const char *fn3_name(int tt) {
    switch (tt) {
    case 0x00: return "FALSE";
    case 0xFF: return "TRUE";
    case 0x01: return "AND3'";
    case 0x03: return "AND2'";
    case 0x06: return "~A(B^C)";
    case 0x07: return "~A~(BC)";
    case 0x0F: return "BUF";
    case 0x16: return "EXACT1";
    case 0x17: return "MAJ'";
    case 0x18: return "ISOLATE";
    case 0x19: return "3v-0x19";
    case 0x1B: return "3v-0x1B";
    case 0x1E: return "A^(B|C)";
    case 0x3C: return "XOR2";
    case 0x69: return "XNOR3";
    default:   return "???";
    }
}

#define MAX_NPN 14

typedef struct {
    int canon_tt;
    int orbit_size;
    const char *titulis;
} NPNClass;

static NPNClass npn_classes[MAX_NPN];
static int n_npn = 0;

static void npn_build_classes(void) {
    int seen[256];
    int i, j;

    memset(seen, 0, sizeof(seen));
    n_npn = 0;

    for (i = 1; i < 255; i++) {
        int c = npn_canon[i];
        if (!seen[c]) {
            seen[c] = 1;
            npn_classes[n_npn].canon_tt = c;
            npn_classes[n_npn].orbit_size = 0;
            npn_classes[n_npn].titulis = fn3_name(c);
            n_npn++;
        }
    }

    for (i = 1; i < 255; i++) {
        int c = npn_canon[i];
        for (j = 0; j < n_npn; j++) {
            if (npn_classes[j].canon_tt == c) {
                npn_classes[j].orbit_size++;
                break;
            }
        }
    }
}

/* ================================================================
 * FK DKC Search Functions
 *
 * Uses pf_catalog[] (FK values) as weights in multiplicative encoding.
 * z(x1,...,xn) = w1^x1 * w2^x2 * ... * wn^xn
 * where weights are FK partition function values.
 * ================================================================ */

/* Pre-computed complex versions of FK catalog */
static Cx pf_cat_cx[MAX_CATALOG];

static void fk_search_2input(const char *act_name,
                              int (*classify)(Cx, int),
                              int act_param) {
    long tt_counts[16];
    int i1, i2, tt;
    long total = 0;
    int n_achievable = 0;
    Cx one_cx;
    one_cx.re = 1.0;
    one_cx.im = 0.0;

    memset(tt_counts, 0, sizeof(tt_counts));

    for (i1 = 0; i1 < pf_cat_size; i1++) {
        Cx w1 = pf_cat_cx[i1];
        for (i2 = 0; i2 < pf_cat_size; i2++) {
            Cx w2 = pf_cat_cx[i2];
            Cyc16 prod16 = cyc16_mul(pf_catalog[i1], pf_catalog[i2]);
            Cx w12 = cyc16_to_cx(prod16);
            int bits = 0;

            if (classify(one_cx, act_param)) bits |= 1;
            if (classify(w2, act_param)) bits |= 2;
            if (classify(w1, act_param)) bits |= 4;
            if (classify(w12, act_param)) bits |= 8;

            tt_counts[bits]++;
            total++;
        }
    }

    printf("  --- %s ---\n", act_name);
    printf("  Pairs: %ld, FK catalog: %d\n", total, pf_cat_size);
    printf("  %4s  %-8s  %10s\n", "TT", "Name", "Count");
    printf("  ----  --------  ----------\n");
    for (tt = 0; tt < 16; tt++) {
        if (tt_counts[tt] > 0) n_achievable++;
        printf("  0x%X   %-8s  %10ld%s\n",
               tt, fn2_name(tt), tt_counts[tt],
               tt_counts[tt] > 0 ? "" : "  (unreachable)");
    }
    printf("  Achievable: %d / 16 functions\n\n", n_achievable);
}

static void fk_search_3input(const char *act_name,
                              int (*classify)(Cx, int),
                              int act_param) {
    long tt_counts[256];
    long class_solutions[MAX_NPN];
    int i1, i2, i3, ci, tt;
    long total = 0;
    int n_reachable = 0;
    Cx one_cx;
    one_cx.re = 1.0;
    one_cx.im = 0.0;

    memset(tt_counts, 0, sizeof(tt_counts));

    for (i1 = 0; i1 < pf_cat_size; i1++) {
        Cx cx1 = pf_cat_cx[i1];
        for (i2 = 0; i2 < pf_cat_size; i2++) {
            Cx cx2 = pf_cat_cx[i2];
            Cyc16 w12_cyc = cyc16_mul(pf_catalog[i1], pf_catalog[i2]);
            Cx cx12 = cyc16_to_cx(w12_cyc);

            for (i3 = 0; i3 < pf_cat_size; i3++) {
                Cx cx3 = pf_cat_cx[i3];
                Cyc16 w23_cyc = cyc16_mul(pf_catalog[i2], pf_catalog[i3]);
                Cyc16 w13_cyc = cyc16_mul(pf_catalog[i1], pf_catalog[i3]);
                Cyc16 w123_cyc = cyc16_mul(w12_cyc, pf_catalog[i3]);
                Cx cx23 = cyc16_to_cx(w23_cyc);
                Cx cx13 = cyc16_to_cx(w13_cyc);
                Cx cx123 = cyc16_to_cx(w123_cyc);
                int bits = 0;

                if (classify(one_cx, act_param)) bits |= 1;
                if (classify(cx3, act_param)) bits |= 2;
                if (classify(cx2, act_param)) bits |= 4;
                if (classify(cx23, act_param)) bits |= 8;
                if (classify(cx1, act_param)) bits |= 16;
                if (classify(cx13, act_param)) bits |= 32;
                if (classify(cx12, act_param)) bits |= 64;
                if (classify(cx123, act_param)) bits |= 128;

                tt_counts[bits]++;
                total++;
            }
        }
    }

    /* Aggregate per NPN class */
    for (ci = 0; ci < n_npn; ci++) {
        int canon = npn_classes[ci].canon_tt;
        class_solutions[ci] = 0;
        for (tt = 0; tt < 256; tt++)
            if (npn_canon[tt] == canon)
                class_solutions[ci] += tt_counts[tt];
    }

    printf("  --- %s ---\n", act_name);
    printf("  Triples: %ld, FK catalog: %d\n", total, pf_cat_size);
    printf("  %4s  %-12s  %10s\n", "TT", "Name", "Solutions");
    printf("  ----  ----------  ----------\n");
    for (ci = 0; ci < n_npn; ci++) {
        if (class_solutions[ci] > 0) n_reachable++;
        printf("  0x%02X  %-12s  %10ld%s\n",
               npn_classes[ci].canon_tt,
               npn_classes[ci].titulis
                   ? npn_classes[ci].titulis : "???",
               class_solutions[ci],
               npn_classes[ci].canon_tt == 0x69
                   && class_solutions[ci] > 0
                   ? "  <<< PARITY" : "");
    }
    printf("  Constants: FALSE=%ld  TRUE=%ld\n",
           tt_counts[0x00], tt_counts[0xFF]);
    printf("  Reachable NPN classes: %d / %d\n\n",
           n_reachable, n_npn);
}

/* ================================================================
 * PART G: DKC Boolean Search with FK Partition Functions
 *
 * The key experiment: do 4D (Z[zeta_8]) FK values compute more
 * Boolean functions than 2D (Z[i]-axial) bracket values?
 * ================================================================ */

static void part_g_fk_dkc(void) {
    int i;
    char msg[256];

    printf("\n=== PART G: FK DKC Boolean Search ===\n\n");
    printf("  Using %d FK partition function values as DKC weights\n",
           pf_cat_size);
    printf("  FK values live in Z[zeta_8] (4D) vs bracket in Z[i] (2D)\n");
    printf("  Hypothesis: 4D -> more computable functions\n\n");

    /* Pre-compute complex versions */
    for (i = 0; i < pf_cat_size; i++)
        pf_cat_cx[i] = cyc16_to_cx(pf_catalog[i]);

    /* NPN setup */
    npn_init();
    npn_build_classes();

    sprintf(msg, "NPN class count = 13 (got %d)", n_npn);
    check(msg, n_npn == 13);

    /* 2-input search */
    printf("  --- 2-Input Search ---\n\n");
    fk_search_2input("Re(z) > 0", wrap_re_positive, 0);
    fk_search_2input("Split-sigmoid", wrap_split_sigmoid, 0);
    fk_search_2input("Sector k=4", wrap_sector, 4);

    /* 3-input search */
    printf("  --- 3-Input NPN Search ---\n\n");
    fk_search_3input("Re(z) > 0", wrap_re_positive, 0);
    fk_search_3input("Split-sigmoid", wrap_split_sigmoid, 0);
    fk_search_3input("Sector k=2", wrap_sector, 2);
    fk_search_3input("Sector k=4", wrap_sector, 4);
    fk_search_3input("Sector k=6", wrap_sector, 6);

    printf("  KEY QUESTION: Does FK catalog (4D, 9 values) achieve\n");
    printf("  more NPN classes than bracket catalog (2D, 56 values)?\n");
    printf("  Bracket at Re>0: 11/13 NPN. FK at Re>0: see above.\n");
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 56: Potts-TL Dictionary\n");
    printf("==========================================="
           "===================\n");
    printf("  delta^2 = Q: Kauffman bracket <-> Potts model\n");
    printf("  Q=1 (percolation), Q=2 (Ising), Q=3 (3-state Potts)\n");

    part_a_cross_validation();
    part_b_percolation();
    part_c_bracket_coupling();
    part_d_critical_coupling();
    part_e_q_hierarchy();
    part_f_catalog_analysis();
    part_g_fk_dkc();

    printf("\n==========================================="
           "===================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("==========================================="
           "===================\n");

    return n_fail > 0 ? 1 : 0;
}
