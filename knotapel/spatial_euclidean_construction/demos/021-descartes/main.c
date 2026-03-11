/*
 * Demo 021: The Descartes Circle Theorem
 *
 * Given three mutually tangent circles with curvatures k_i = 1/r_i,
 * the Descartes formula determines the fourth tangent circle:
 *
 *   k4 = k1 + k2 + k3 +/- 2*sqrt(k1*k2 + k2*k3 + k3*k1)
 *
 * For the configuration (k1,k2,k3) = (2,3,6):
 *   discriminant = 6+18+12 = 36 = 6^2  (perfect square!)
 *   k4 = 11 +/- 12 = 23 (inner) or -1 (outer)
 *
 * The Descartes relation for all four:
 *   (k1+k2+k3+k4)^2 = 2*(k1^2 + k2^2 + k3^2 + k4^2)
 *
 * All centers and radii are rational (a consequence of the
 * discriminant being a perfect square), so exact verification
 * is possible using inversive distance d^2 = 1 (Book XIX).
 *
 * Circles in z=0 plane, represented as spheres in R^3.
 *
 * Key constructions used:
 *   - Inversive distance and product (Book XIX)
 *   - Conformal property (Book XX)
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long p, q; } Rat;
typedef struct { Rat x, y, z; } RVec3;
typedef struct { RVec3 c; Rat r2; } Sphere;

/* ---- rational arithmetic ---- */

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

static int rat_eq(Rat a, Rat b) { return a.p == b.p && a.q == b.q; }

static Rat rat_add(Rat a, Rat b)
{
    return rat_make(a.p * b.q + b.p * a.q, a.q * b.q);
}

static Rat rat_sub(Rat a, Rat b)
{
    return rat_make(a.p * b.q - b.p * a.q, a.q * b.q);
}

static Rat rat_mul(Rat a, Rat b)
{
    return rat_make(a.p * b.p, a.q * b.q);
}

static Rat rat_div(Rat a, Rat b)
{
    return rat_make(a.p * b.q, a.q * b.p);
}

static void rat_print(Rat r)
{
    if (r.q == 1) printf("%ld", r.p);
    else printf("%ld/%ld", r.p, r.q);
}

/* ---- vector ops ---- */

static RVec3 rvec3_make(Rat x, Rat y, Rat z)
{
    RVec3 r;
    r.x = x; r.y = y; r.z = z;
    return r;
}

static RVec3 rvec3_from_ints(long x, long y, long z)
{
    return rvec3_make(rat_make(x, 1), rat_make(y, 1), rat_make(z, 1));
}

static RVec3 rvec3_sub(RVec3 a, RVec3 b)
{
    return rvec3_make(rat_sub(a.x, b.x), rat_sub(a.y, b.y),
                      rat_sub(a.z, b.z));
}

static Rat rvec3_dot(RVec3 a, RVec3 b)
{
    return rat_add(rat_add(rat_mul(a.x, b.x), rat_mul(a.y, b.y)),
                   rat_mul(a.z, b.z));
}

static Rat rvec3_len2(RVec3 v)
{
    return rvec3_dot(v, v);
}

/* ---- sphere ops ---- */

static Sphere sphere_make(RVec3 c, Rat r2)
{
    Sphere s;
    s.c = c; s.r2 = r2;
    return s;
}

static int on_sphere(RVec3 pt, Sphere s)
{
    return rat_eq(rvec3_len2(rvec3_sub(pt, s.c)), s.r2);
}

/* ---- inversive distance (Book XIX) ---- */

static Rat inv_product(Sphere s1, Sphere s2)
{
    return rat_sub(rat_sub(rvec3_len2(rvec3_sub(s1.c, s2.c)),
                           s1.r2), s2.r2);
}

static Rat inv_dist2(Sphere s1, Sphere s2)
{
    Rat d = inv_product(s1, s2);
    return rat_div(rat_mul(d, d),
                   rat_mul(rat_make(4, 1), rat_mul(s1.r2, s2.r2)));
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
    /*
     * Descartes configuration (k = 2, 3, 6).
     * Circles in the z=0 plane, represented as spheres.
     *
     * S1: center (0,0,0),         r = 1/2,  k = 2
     * S2: center (5/6, 0, 0),     r = 1/3,  k = 3
     * S3: center (8/15, 2/5, 0),  r = 1/6,  k = 6
     *
     * Inner Apollonius circle:
     * S_in: center (117/230, 22/115, 0), r = 1/23, k = 23
     *
     * Outer Apollonius circle (encloses all):
     * S_out: center (3/10, -2/5, 0), r = 1, k = -1
     */
    Sphere S1, S2, S3, S_in, S_out;
    RVec3 T12, T14;
    Rat k1, k2, k3, k_in, k_out;
    Rat disc, k_sum, lhs, rhs;
    Rat d2, delta, expected;

    S1 = sphere_make(rvec3_from_ints(0, 0, 0), rat_make(1, 4));
    S2 = sphere_make(
        rvec3_make(rat_make(5, 6), rat_make(0, 1), rat_make(0, 1)),
        rat_make(1, 9));
    S3 = sphere_make(
        rvec3_make(rat_make(8, 15), rat_make(2, 5), rat_make(0, 1)),
        rat_make(1, 36));
    S_in = sphere_make(
        rvec3_make(rat_make(117, 230), rat_make(22, 115),
                   rat_make(0, 1)),
        rat_make(1, 529));
    S_out = sphere_make(
        rvec3_make(rat_make(3, 10), rat_make(-2, 5), rat_make(0, 1)),
        rat_make(1, 1));

    k1 = rat_make(2, 1);
    k2 = rat_make(3, 1);
    k3 = rat_make(6, 1);
    k_in = rat_make(23, 1);
    k_out = rat_make(-1, 1);

    T12 = rvec3_make(rat_make(1, 2), rat_make(0, 1), rat_make(0, 1));
    T14 = rvec3_make(rat_make(117, 250), rat_make(22, 125),
                     rat_make(0, 1));

    printf("=== Demo 021: The Descartes Circle Theorem ===\n\n");
    printf("Curvature k = 1/r.  Descartes: (sum k)^2 = 2(sum k^2).\n");
    printf("Configuration: k = (2, 3, 6) => k4 = 23 or -1.\n\n");

    /* ---- Part 1: Generating triple ---- */
    printf("--- Part 1: Generating triple (k = 2, 3, 6) ---\n");
    {
        printf("  S1: (0,0,0) r=1/2   S2: (5/6,0,0) r=1/3\n");
        printf("  S3: (8/15,2/5,0) r=1/6\n\n");

        d2 = inv_dist2(S1, S2);
        printf("  d^2(S1,S2) = "); rat_print(d2); printf("\n");
        check("d^2(S1,S2) = 1 (tangent)", rat_eq(d2, rat_make(1, 1)));

        d2 = inv_dist2(S1, S3);
        printf("  d^2(S1,S3) = "); rat_print(d2); printf("\n");
        check("d^2(S1,S3) = 1 (tangent)", rat_eq(d2, rat_make(1, 1)));

        d2 = inv_dist2(S2, S3);
        printf("  d^2(S2,S3) = "); rat_print(d2); printf("\n");
        check("d^2(S2,S3) = 1 (tangent)", rat_eq(d2, rat_make(1, 1)));

        check("T12 = (1/2,0,0) on S1 and S2",
              on_sphere(T12, S1) && on_sphere(T12, S2));
    }

    /* ---- Part 2: Descartes formula ---- */
    printf("\n--- Part 2: Descartes Circle Theorem ---\n");
    {
        printf("  k4 = k1+k2+k3 +/- 2*sqrt(k1k2+k2k3+k3k1)\n\n");

        disc = rat_add(rat_add(rat_mul(k1, k2), rat_mul(k2, k3)),
                       rat_mul(k3, k1));
        printf("  discriminant = "); rat_print(disc); printf("\n");
        check("discriminant = 36 = 6^2 (perfect square)",
              rat_eq(disc, rat_make(36, 1)));

        k_sum = rat_add(rat_add(k1, k2), k3);
        printf("  k1+k2+k3 = "); rat_print(k_sum);
        printf(",  2*sqrt(36) = 12\n");
        check("k4 = 11+12 = 23 (inner), 11-12 = -1 (outer)",
              rat_eq(rat_add(k_sum, rat_make(12, 1)), k_in)
              && rat_eq(rat_sub(k_sum, rat_make(12, 1)), k_out));

        /* (2+3+6+23)^2 = 2(4+9+36+529) */
        lhs = rat_mul(rat_add(k_sum, k_in), rat_add(k_sum, k_in));
        rhs = rat_mul(rat_make(2, 1),
              rat_add(rat_add(rat_mul(k1, k1), rat_mul(k2, k2)),
                      rat_add(rat_mul(k3, k3), rat_mul(k_in, k_in))));
        printf("  Inner: (34)^2 = "); rat_print(lhs);
        printf(",  2*(4+9+36+529) = "); rat_print(rhs); printf("\n");
        check("Descartes (2,3,6,23): 1156 = 1156", rat_eq(lhs, rhs));

        /* (2+3+6-1)^2 = 2(4+9+36+1) */
        lhs = rat_mul(rat_add(k_sum, k_out), rat_add(k_sum, k_out));
        rhs = rat_mul(rat_make(2, 1),
              rat_add(rat_add(rat_mul(k1, k1), rat_mul(k2, k2)),
                      rat_add(rat_mul(k3, k3),
                              rat_mul(k_out, k_out))));
        printf("  Outer: (10)^2 = "); rat_print(lhs);
        printf(",  2*(4+9+36+1) = "); rat_print(rhs); printf("\n");
        check("Descartes (2,3,6,-1): 100 = 100", rat_eq(lhs, rhs));
    }

    /* ---- Part 3: Inner Apollonius circle ---- */
    printf("\n--- Part 3: Inner Apollonius circle (k = 23) ---\n");
    {
        printf("  S_in: center (117/230, 22/115, 0), r = 1/23\n\n");

        d2 = inv_dist2(S1, S_in);
        printf("  d^2(S1,S_in) = "); rat_print(d2); printf("\n");
        check("d^2(S1,S_in) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = inv_dist2(S2, S_in);
        printf("  d^2(S2,S_in) = "); rat_print(d2); printf("\n");
        check("d^2(S2,S_in) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = inv_dist2(S3, S_in);
        printf("  d^2(S3,S_in) = "); rat_print(d2); printf("\n");
        check("d^2(S3,S_in) = 1", rat_eq(d2, rat_make(1, 1)));

        check("T14 = (117/250,22/125,0) on S1 and S_in",
              on_sphere(T14, S1) && on_sphere(T14, S_in));
    }

    /* ---- Part 4: Outer Apollonius circle ---- */
    printf("\n--- Part 4: Outer Apollonius circle (k = -1) ---\n");
    {
        printf("  S_out: center (3/10, -2/5, 0), r = 1\n");
        printf("  Encloses all three generating circles.\n\n");

        d2 = inv_dist2(S1, S_out);
        printf("  d^2(S1,S_out) = "); rat_print(d2); printf("\n");
        check("d^2(S1,S_out) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = inv_dist2(S2, S_out);
        printf("  d^2(S2,S_out) = "); rat_print(d2); printf("\n");
        check("d^2(S2,S_out) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = inv_dist2(S3, S_out);
        printf("  d^2(S3,S_out) = "); rat_print(d2); printf("\n");
        check("d^2(S3,S_out) = 1", rat_eq(d2, rat_make(1, 1)));

        delta = inv_product(S1, S_out);
        printf("  delta(S1,S_out) = "); rat_print(delta);
        printf(" < 0 (S1 inside S_out)\n");
        check("delta(S1,S_out) = -1 (internal tangency)",
              rat_eq(delta, rat_make(-1, 1)));
    }

    /* ---- Part 5: External vs internal tangency ---- */
    printf("\n--- Part 5: External vs internal tangency ---\n");
    {
        printf("  External: delta = +2*r1*r2.\n");
        printf("  Internal: delta = -2*r1*r2.\n");
        printf("  Both give d^2 = 1.\n\n");

        delta = inv_product(S1, S2);
        expected = rat_make(1, 3);  /* 2*(1/2)*(1/3) */
        printf("  delta(S1,S2) = "); rat_print(delta);
        printf(" = +2r1r2 = "); rat_print(expected); printf("\n");
        check("delta(S1,S2) = 1/3 (external)",
              rat_eq(delta, expected));

        delta = inv_product(S1, S_out);
        expected = rat_make(-1, 1);  /* -2*(1/2)*1 */
        printf("  delta(S1,S_out) = "); rat_print(delta);
        printf(" = -2r1*r_out = "); rat_print(expected); printf("\n");
        check("delta(S1,S_out) = -1 (internal)",
              rat_eq(delta, expected));

        delta = inv_product(S_in, S3);
        expected = rat_make(1, 69);  /* 2*(1/23)*(1/6) */
        printf("  delta(S_in,S3) = "); rat_print(delta);
        printf(" = +2*r_in*r3 = "); rat_print(expected); printf("\n");
        check("delta(S_in,S3) = 1/69 (external)",
              rat_eq(delta, expected));

        d2 = inv_dist2(S_in, S_out);
        printf("  d^2(S_in,S_out) = "); rat_print(d2);
        printf(" (not tangent)\n");
        check("d^2(S_in,S_out) = 49 (solutions are separated)",
              rat_eq(d2, rat_make(49, 1)));
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("The Descartes Circle Theorem: (sum k)^2 = 2(sum k^2).\n\n");
    printf("Given mutually tangent circles (k = 2, 3, 6):\n");
    printf("  disc = k1k2+k2k3+k3k1 = 36 = 6^2 (rational solutions)\n");
    printf("  k4 = 23 (inner, fits in gaps)\n");
    printf("  k4 = -1 (outer, encloses all)\n\n");
    printf("Verified: d^2 = 1 for all 9 tangent pairs.\n");
    printf("  delta > 0: external tangency\n");
    printf("  delta < 0: internal tangency (enclosure)\n");
    printf("  d^2 = 1 regardless of sign.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
