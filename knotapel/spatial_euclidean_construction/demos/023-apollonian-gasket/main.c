/*
 * Demo 023: Apollonian Gasket
 *
 * Starting from the Descartes quadruple (-1, 2, 3, 6), recursively
 * fill each curvilinear triangle with the "other" tangent circle.
 *
 * The Descartes recursion:
 *   k' = 2*(k_a + k_b + k_c) - k_old       (curvatures stay integer)
 *   W' = 2*(W_a + W_b + W_c) - W_old        (W = 10*k*center stays integer)
 *
 * Key insight: all curvature-weighted centers w = k*center have
 * denominators dividing 10 for this configuration.  Defining
 * W = 10*w makes the entire computation purely integer.
 *
 * Tangency verified via:
 *   (kj*Wi - ki*Wj)^2 = 100*(ki + kj)^2
 *
 * Every quadruple satisfies:
 *   (k1 + k2 + k3 + k4)^2 = 2*(k1^2 + k2^2 + k3^2 + k4^2)
 *
 * Key constructions used:
 *   - Descartes Circle Theorem (Book XXI)
 *   - Complex Descartes theorem for center propagation
 *   - Inversive distance d^2 = 1 for tangency (Book XIX)
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long p, q; } Rat;

typedef struct {
    long k;         /* curvature (integer) */
    long Wx, Wy;    /* 10 * k * center (integer) */
    int depth;      /* generation depth */
} GCircle;

typedef struct {
    int ci[4];      /* indices of 4 mutually tangent circles */
    int newest;     /* position of newest circle (-1 for root) */
    int depth;
} GQuad;

#define MAX_DEPTH 4
#define MAX_CIRC 256
#define MAX_QUAD 256

static GCircle gc[MAX_CIRC];
static int n_gc = 0;

static GQuad gq[MAX_QUAD];
static int n_gq = 0;
static int gq_head = 0;

/* ---- rational (display only) ---- */

static long gcd_long(long a, long b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { long t = b; b = a % b; a = t; }
    return a;
}

static Rat rat_make(long p, long q)
{
    Rat r;
    long g;
    if (q < 0) { p = -p; q = -q; }
    if (p == 0) { r.p = 0; r.q = 1; return r; }
    g = gcd_long(p, q);
    r.p = p / g; r.q = q / g;
    return r;
}

static void rat_print(Rat r)
{
    if (r.q == 1) printf("%ld", r.p);
    else printf("%ld/%ld", r.p, r.q);
}

/* Center of circle i: center = W / (10*k) */
static Rat gc_cx(int i) { return rat_make(gc[i].Wx, 10 * gc[i].k); }
static Rat gc_cy(int i) { return rat_make(gc[i].Wy, 10 * gc[i].k); }

/* ---- gasket operations (all integer) ---- */

static int gc_add(long k, long Wx, long Wy, int depth)
{
    gc[n_gc].k = k;
    gc[n_gc].Wx = Wx;
    gc[n_gc].Wy = Wy;
    gc[n_gc].depth = depth;
    return n_gc++;
}

/* Descartes curvature check: (sum k)^2 = 2*(sum k^2) */
static int descartes_k(int a, int b, int c, int d)
{
    long ka = gc[a].k, kb = gc[b].k, kc = gc[c].k, kd = gc[d].k;
    long s = ka + kb + kc + kd;
    long q = ka*ka + kb*kb + kc*kc + kd*kd;
    return s * s == 2 * q;
}

/*
 * Tangency check via integer weighted-centers.
 * |ci - cj|^2 = ((ki+kj)/(ki*kj))^2
 * With W = 10*k*center:
 *   (kj*Wi - ki*Wj)^2_x + (kj*Wi - ki*Wj)^2_y = 100*(ki+kj)^2
 */
static int tangent_ww(int i, int j)
{
    long ki = gc[i].k, kj = gc[j].k;
    long ux = kj * gc[i].Wx - ki * gc[j].Wx;
    long uy = kj * gc[i].Wy - ki * gc[j].Wy;
    long rhs = (ki + kj) * (ki + kj) * 100;
    return ux * ux + uy * uy == rhs;
}

/* BFS gasket generation */
static void gasket_generate(void)
{
    while (gq_head < n_gq) {
        GQuad *q = &gq[gq_head];
        int j;

        if (q->depth >= MAX_DEPTH) { gq_head++; continue; }

        for (j = 0; j < 4; j++) {
            long sk, sw_x, sw_y, k_new, wx_new, wy_new;
            int ci_new, m;
            GQuad nq;

            if (j == q->newest) continue;
            if (n_gc >= MAX_CIRC || n_gq >= MAX_QUAD) continue;

            sk = 0; sw_x = 0; sw_y = 0;
            for (m = 0; m < 4; m++) {
                if (m == j) continue;
                sk += gc[q->ci[m]].k;
                sw_x += gc[q->ci[m]].Wx;
                sw_y += gc[q->ci[m]].Wy;
            }

            k_new = 2 * sk - gc[q->ci[j]].k;
            wx_new = 2 * sw_x - gc[q->ci[j]].Wx;
            wy_new = 2 * sw_y - gc[q->ci[j]].Wy;

            ci_new = gc_add(k_new, wx_new, wy_new, q->depth + 1);

            nq.ci[0] = q->ci[0]; nq.ci[1] = q->ci[1];
            nq.ci[2] = q->ci[2]; nq.ci[3] = q->ci[3];
            nq.ci[j] = ci_new;
            nq.newest = j;
            nq.depth = q->depth + 1;
            gq[n_gq++] = nq;
        }
        gq_head++;
    }
}

/* ---- test framework ---- */

static int tests_run = 0;
static int tests_passed = 0;

static void check(const char *desc, int cond)
{
    tests_run++;
    if (cond) {
        tests_passed++;
        printf("  PASS: %s\n", desc);
    } else {
        printf("  FAIL: %s\n", desc);
    }
}

/* ---- main ---- */

int main(void)
{
    int i, j;
    int desc_ok, tang_ok, desc_n, tang_n;
    long dist_k[MAX_CIRC];
    int dist_cnt[MAX_CIRC];
    int n_dist;
    int res6[6];
    int depth_count[MAX_DEPTH + 1];
    long depth_max[MAX_DEPTH + 1];

    printf("=== Demo 023: Apollonian Gasket ===\n\n");
    printf("From Descartes quadruple (-1, 2, 3, 6), fill every\n");
    printf("curvilinear triangle with the other tangent circle.\n\n");

    /* ---- Seed base configuration ---- */
    /*
     * k=-1: center (3/10, -2/5),  W = 10*(-1)*(3/10, -2/5) = (-3, 4)
     * k=2:  center (0, 0),        W = 10*2*(0, 0)           = (0, 0)
     * k=3:  center (5/6, 0),      W = 10*3*(5/6, 0)         = (25, 0)
     * k=6:  center (8/15, 2/5),   W = 10*6*(8/15, 2/5)      = (32, 24)
     */
    gc_add(-1, -3, 4, 0);
    gc_add(2, 0, 0, 0);
    gc_add(3, 25, 0, 0);
    gc_add(6, 32, 24, 0);

    gq[0].ci[0] = 0; gq[0].ci[1] = 1;
    gq[0].ci[2] = 2; gq[0].ci[3] = 3;
    gq[0].newest = -1; gq[0].depth = 0;
    n_gq = 1;

    /* ---- Part 1: Descartes recursion ---- */
    printf("--- Part 1: The Descartes recursion ---\n");
    printf("  k' = 2*(k_a + k_b + k_c) - k_old\n\n");
    {
        long k;

        check("Descartes (-1,2,3,6): 100 = 100",
              descartes_k(0, 1, 2, 3));

        k = 2 * (2 + 3 + 6) - (-1);
        printf("  Replace k=-1: k' = 2*(2+3+6)-(-1) = %ld\n", k);
        check("Other tangent to (2,3,6) = 23", k == 23);

        k = 2 * (-1 + 3 + 6) - 2;
        printf("  Replace k=2:  k' = 2*(-1+3+6)-2   = %ld\n", k);
        check("Other tangent to (-1,3,6) = 14", k == 14);

        k = 2 * (-1 + 2 + 6) - 3;
        printf("  Replace k=3:  k' = 2*(-1+2+6)-3   = %ld\n", k);
        check("Other tangent to (-1,2,6) = 11", k == 11);

        k = 2 * (-1 + 2 + 3) - 6;
        printf("  Replace k=6:  k' = 2*(-1+2+3)-6   = %ld\n", k);
        check("Other tangent to (-1,2,3) = 2", k == 2);
    }

    /* ---- Part 2: Integer center propagation ---- */
    printf("\n--- Part 2: Center computation (complex Descartes) ---\n");
    printf("  W = 10 * k * center (integer).\n");
    printf("  W' = 2*(W_a + W_b + W_c) - W_old.\n\n");
    {
        long swx, swy;
        long lhs_r, lhs_i, rhs_r, rhs_i;
        Rat cx, cy;

        /* k=23: W = 2*(0+25+32)-(-3), 2*(0+0+24)-4 = (117, 44) */
        swx = 2 * (0 + 25 + 32) - (-3);
        swy = 2 * (0 + 0 + 24) - 4;
        cx = rat_make(swx, 10 * 23);
        cy = rat_make(swy, 10 * 23);
        printf("  k=23: W=(%ld,%ld)  center=(", swx, swy);
        rat_print(cx); printf(", "); rat_print(cy); printf(")\n");
        check("k=23 center = (117/230, 22/115)",
              cx.p == 117 && cx.q == 230 && cy.p == 22 && cy.q == 115);

        /* k=14: W = 2*(-3+25+32)-0, 2*(4+0+24)-0 = (108, 56) */
        swx = 2 * (-3 + 25 + 32) - 0;
        swy = 2 * (4 + 0 + 24) - 0;
        cx = rat_make(swx, 10 * 14);
        cy = rat_make(swy, 10 * 14);
        printf("  k=14: W=(%ld,%ld)  center=(", swx, swy);
        rat_print(cx); printf(", "); rat_print(cy); printf(")\n");
        check("k=14 center = (27/35, 2/5)",
              cx.p == 27 && cx.q == 35 && cy.p == 2 && cy.q == 5);

        /*
         * Complex Descartes: (sum W)^2 = 2*(sum Wi^2)
         * Complex squaring: (a+bi)^2 = (a^2 - b^2) + (2ab)i
         */
        swx = -3 + 0 + 25 + 32;  /* 54 */
        swy = 4 + 0 + 0 + 24;    /* 28 */
        lhs_r = swx * swx - swy * swy;  /* 2916 - 784 = 2132 */
        lhs_i = 2 * swx * swy;          /* 3024 */

        rhs_r = 0; rhs_i = 0;
        for (i = 0; i < 4; i++) {
            rhs_r += gc[i].Wx * gc[i].Wx - gc[i].Wy * gc[i].Wy;
            rhs_i += 2 * gc[i].Wx * gc[i].Wy;
        }
        rhs_r *= 2;
        rhs_i *= 2;

        printf("  (sum W)^2   = %ld + %ldi\n", lhs_r, lhs_i);
        printf("  2*sum(Wi^2) = %ld + %ldi\n", rhs_r, rhs_i);
        check("Complex Descartes for root quadruple",
              lhs_r == rhs_r && lhs_i == rhs_i);
    }

    /* ---- Part 3: Generate + first generation detail ---- */
    printf("\n--- Part 3: First generation tangencies ---\n");
    gasket_generate();
    printf("  Generated %d circles, %d quadruples (depth %d).\n\n",
           n_gc, n_gq, MAX_DEPTH);

    /* Verify first-gen curvatures match */
    check("Gen 1: k = 23, 14, 11, 2",
          gc[4].k == 23 && gc[5].k == 14 &&
          gc[6].k == 11 && gc[7].k == 2);

    /* Display first generation */
    printf("\n  Depth 1 circles:\n");
    for (i = 4; i < 8 && i < n_gc; i++) {
        Rat cx_r, cy_r;
        cx_r = gc_cx(i); cy_r = gc_cy(i);
        printf("    k=%-3ld  W=(%ld,%ld)  center=(", gc[i].k,
               gc[i].Wx, gc[i].Wy);
        rat_print(cx_r); printf(", "); rat_print(cy_r);
        printf(")  r=1/%ld\n", gc[i].k);
    }
    printf("\n");

    /* Tangency: k=23 with its triple (2, 3, 6) */
    check("k=23 tangent to k=2", tangent_ww(4, 1));
    check("k=23 tangent to k=3", tangent_ww(4, 2));
    check("k=23 tangent to k=6", tangent_ww(4, 3));

    /* Tangency: k=14 with its triple (-1, 3, 6) */
    check("k=14 tangent to k=-1", tangent_ww(5, 0));
    check("k=14 tangent to k=3", tangent_ww(5, 2));
    check("k=14 tangent to k=6", tangent_ww(5, 3));

    /* Tangency: k=11 with its triple (-1, 2, 6) */
    check("k=11 tangent to k=-1", tangent_ww(6, 0));
    check("k=11 tangent to k=2", tangent_ww(6, 1));
    check("k=11 tangent to k=6", tangent_ww(6, 3));

    /* Tangency: k=2' with its triple (-1, 2, 3) */
    check("k=2' tangent to k=-1", tangent_ww(7, 0));
    check("k=2' tangent to k=2", tangent_ww(7, 1));
    check("k=2' tangent to k=3", tangent_ww(7, 2));

    /* Non-tangency: k=23 and k=-1 are NOT adjacent */
    check("k=23 and k=-1 not tangent", !tangent_ww(4, 0));

    printf("\n");
    check("Descartes (23,2,3,6)", descartes_k(4, 1, 2, 3));
    check("Descartes (-1,14,3,6)", descartes_k(0, 5, 2, 3));
    check("Descartes (-1,2,11,6)", descartes_k(0, 1, 6, 3));
    check("Descartes (-1,2,3,2')", descartes_k(0, 1, 2, 7));

    /* ---- Part 4: Batch verification ---- */
    printf("\n--- Part 4: Full gasket verification (depth %d) ---\n",
           MAX_DEPTH);
    {
        desc_ok = 0; desc_n = 0;
        tang_ok = 0; tang_n = 0;

        for (i = 0; i < n_gq; i++) {
            int a, b;
            desc_n++;
            if (descartes_k(gq[i].ci[0], gq[i].ci[1],
                            gq[i].ci[2], gq[i].ci[3]))
                desc_ok++;
            for (a = 0; a < 4; a++) {
                for (b = a + 1; b < 4; b++) {
                    tang_n++;
                    if (tangent_ww(gq[i].ci[a], gq[i].ci[b]))
                        tang_ok++;
                }
            }
        }

        printf("  Descartes check: %d/%d quadruples\n", desc_ok, desc_n);
        check("All quadruples satisfy Descartes", desc_ok == desc_n);

        printf("  Tangency check: %d/%d pairs\n", tang_ok, tang_n);
        check("All adjacent pairs tangent (d^2=1)", tang_ok == tang_n);
    }

    /* ---- Part 5: Curvature catalog ---- */
    printf("\n--- Part 5: Curvature catalog ---\n");
    {
        /* Collect distinct curvatures */
        n_dist = 0;
        for (i = 0; i < n_gc; i++) {
            long k = gc[i].k;
            int found = 0;
            for (j = 0; j < n_dist; j++) {
                if (dist_k[j] == k) { dist_cnt[j]++; found = 1; break; }
            }
            if (!found) {
                dist_k[n_dist] = k;
                dist_cnt[n_dist] = 1;
                n_dist++;
            }
        }

        /* Sort by curvature */
        for (i = 0; i < n_dist - 1; i++) {
            for (j = i + 1; j < n_dist; j++) {
                if (dist_k[j] < dist_k[i]) {
                    long tk = dist_k[i];
                    int tc = dist_cnt[i];
                    dist_k[i] = dist_k[j];
                    dist_cnt[i] = dist_cnt[j];
                    dist_k[j] = tk;
                    dist_cnt[j] = tc;
                }
            }
        }

        printf("  %d circles, %d distinct curvatures.\n\n  ", n_gc, n_dist);
        for (i = 0; i < n_dist; i++) {
            if (i > 0) printf(", ");
            if (i > 0 && i % 10 == 0) printf("\n  ");
            printf("%ld", dist_k[i]);
            if (dist_cnt[i] > 1) printf("(x%d)", dist_cnt[i]);
        }
        printf("\n\n");

        /* Per-depth statistics */
        for (i = 0; i <= MAX_DEPTH; i++) {
            depth_count[i] = 0;
            depth_max[i] = 0;
        }
        for (i = 0; i < n_gc; i++) {
            int d = gc[i].depth;
            depth_count[d]++;
            if (gc[i].k > depth_max[d]) depth_max[d] = gc[i].k;
        }
        printf("  Per-depth statistics:\n");
        for (i = 0; i <= MAX_DEPTH; i++) {
            printf("    depth %d: %3d circles, max k = %ld\n",
                   i, depth_count[i], depth_max[i]);
        }
        printf("\n");

        /* Residues mod 6 */
        for (i = 0; i < 6; i++) res6[i] = 0;
        for (i = 0; i < n_dist; i++) {
            if (dist_k[i] > 0) {
                int r = (int)(dist_k[i] % 6);
                res6[r] = 1;
            }
        }
        printf("  Positive curvatures mod 6: {");
        {
            int first = 1;
            for (i = 0; i < 6; i++) {
                if (res6[i]) {
                    if (!first) printf(", ");
                    printf("%d", i);
                    first = 0;
                }
            }
        }
        printf("}\n");
        check("Residues mod 6 = {0, 2, 3, 5}",
              res6[0] && !res6[1] && res6[2] &&
              res6[3] && !res6[4] && res6[5]);

        check("Total circles = 164", n_gc == 164);
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("Apollonian gasket from (-1, 2, 3, 6) to depth %d.\n\n",
           MAX_DEPTH);
    printf("Recursion (all integer arithmetic):\n");
    printf("  k' = 2*(k_a+k_b+k_c) - k_old\n");
    printf("  W' = 2*(W_a+W_b+W_c) - W_old    (W = 10*k*center)\n\n");
    printf("Generated %d circles (%d distinct curvatures).\n", n_gc, n_dist);
    printf("All %d quadruples satisfy (sum k)^2 = 2(sum k^2).\n", n_gq);
    printf("All %d adjacent pairs verified tangent.\n", tang_n);
    printf("Curvatures mod 6 in {0, 2, 3, 5}: residues 1, 4 never appear.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
