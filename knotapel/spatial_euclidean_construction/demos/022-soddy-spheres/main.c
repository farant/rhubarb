/*
 * Demo 022: Soddy Spheres -- The 3D Descartes Theorem
 *
 * For 5 mutually tangent spheres with curvatures k_i = 1/r_i:
 *   (k1 + k2 + k3 + k4 + k5)^2 = 3(k1^2 + k2^2 + k3^2 + k4^2 + k5^2)
 *
 * Starting from the (2,3,6,23) Descartes circle configuration (Book XXI),
 * the 3D formula yields a unique Soddy sphere with k = 17.
 * Starting from (2,3,6,-1), it yields k = 5.
 *
 * The discriminant is zero in both cases -- a theorem: when the base
 * configuration satisfies the 2D Descartes relation (sum k)^2 = 2(sum k^2),
 * the 3D quadratic has a double root, giving a unique Soddy curvature
 * k5 = S4/2.
 *
 * Both Soddy spheres have irrational z-coordinates (z = sqrt(3)*r),
 * but z^2 is rational, so inversive distance d^2 remains exact.
 *
 * Bonus: the two Soddy spheres are themselves mutually tangent, yielding
 * three quintets that all satisfy the 3D relation:
 *   (2,3,6,23,17)  (2,3,6,-1,5)  (2,3,6,17,5)
 *
 * Key constructions used:
 *   - Descartes Circle Theorem (Book XXI)
 *   - Inversive distance d^2 = 1 (Book XIX)
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long p, q; } Rat;
typedef struct { Rat x, y, z; } RVec3;
typedef struct { RVec3 c; Rat r2; } Sphere;

/*
 * Soddy sphere: center (x, y, z) where z may be irrational.
 * We store z^2 (rational) instead of z.
 * r is stored explicitly (rational when curvature is integer).
 */
typedef struct { Rat x, y, z2, r, r2; } Soddy;

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

static Rat rvec3_len2(RVec3 v) { return rvec3_dot(v, v); }

/* ---- sphere ops ---- */

static Sphere sphere_make(RVec3 c, Rat r2)
{
    Sphere s;
    s.c = c; s.r2 = r2;
    return s;
}

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

/* ---- Soddy sphere ops ---- */

static Soddy soddy_make(Rat x, Rat y, Rat z2, Rat r)
{
    Soddy s;
    s.x = x; s.y = y; s.z2 = z2;
    s.r = r; s.r2 = rat_mul(r, r);
    return s;
}

/*
 * Squared distance from Soddy center to a z=0 sphere center.
 * |S - P|^2 = (sx - px)^2 + (sy - py)^2 + sz^2
 */
static Rat soddy_flat_dist2(Soddy s, Sphere p)
{
    Rat dx = rat_sub(s.x, p.c.x);
    Rat dy = rat_sub(s.y, p.c.y);
    return rat_add(rat_add(rat_mul(dx, dx), rat_mul(dy, dy)), s.z2);
}

/*
 * Inversive product between Soddy sphere and z=0 sphere.
 */
static Rat soddy_flat_delta(Soddy s, Sphere p)
{
    return rat_sub(rat_sub(soddy_flat_dist2(s, p), s.r2), p.r2);
}

/*
 * Squared inversive distance between Soddy sphere and z=0 sphere.
 */
static Rat soddy_flat_d2(Soddy s, Sphere p)
{
    Rat delta = soddy_flat_delta(s, p);
    return rat_div(rat_mul(delta, delta),
                   rat_mul(rat_make(4, 1), rat_mul(s.r2, p.r2)));
}

/*
 * Squared distance between two Soddy sphere centers (both z > 0).
 * Since z = sqrt(3)*r for both: za*zb = 3*ra*rb.
 * dist^2 = (xa-xb)^2 + (ya-yb)^2 + z2a + z2b - 6*ra*rb
 */
static Rat soddy_pair_dist2(Soddy a, Soddy b)
{
    Rat dx = rat_sub(a.x, b.x);
    Rat dy = rat_sub(a.y, b.y);
    Rat xy = rat_add(rat_mul(dx, dx), rat_mul(dy, dy));
    Rat zz = rat_sub(rat_add(a.z2, b.z2),
                     rat_mul(rat_make(6, 1), rat_mul(a.r, b.r)));
    return rat_add(xy, zz);
}

static Rat soddy_pair_delta(Soddy a, Soddy b)
{
    return rat_sub(rat_sub(soddy_pair_dist2(a, b), a.r2), b.r2);
}

static Rat soddy_pair_d2(Soddy a, Soddy b)
{
    Rat delta = soddy_pair_delta(a, b);
    return rat_div(rat_mul(delta, delta),
                   rat_mul(rat_make(4, 1), rat_mul(a.r2, b.r2)));
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
    /* Demo 021 circles in z=0 */
    Sphere S1, S2, S3, S_in, S_out;
    Rat k1, k2, k3, k_in, k_out, k_A, k_B;
    Soddy SA, SB;
    Rat S4, Q4, disc, d2, delta;
    Rat sum5, sumsq5, lhs, rhs;

    /* Base circles from Demo 021 */
    S1 = sphere_make(rvec3_from_ints(0, 0, 0), rat_make(1, 4));
    S2 = sphere_make(
        rvec3_make(rat_make(5, 6), rat_make(0, 1), rat_make(0, 1)),
        rat_make(1, 9));
    S3 = sphere_make(
        rvec3_make(rat_make(8, 15), rat_make(2, 5), rat_make(0, 1)),
        rat_make(1, 36));
    S_in = sphere_make(
        rvec3_make(rat_make(117, 230), rat_make(22, 115), rat_make(0, 1)),
        rat_make(1, 529));
    S_out = sphere_make(
        rvec3_make(rat_make(3, 10), rat_make(-2, 5), rat_make(0, 1)),
        rat_make(1, 1));

    k1 = rat_make(2, 1);
    k2 = rat_make(3, 1);
    k3 = rat_make(6, 1);
    k_in = rat_make(23, 1);
    k_out = rat_make(-1, 1);

    /* Soddy sphere A: k=17, r=1/17, center (87/170, 1/5, sqrt(3)/17) */
    k_A = rat_make(17, 1);
    SA = soddy_make(rat_make(87, 170), rat_make(1, 5),
                    rat_make(3, 289), rat_make(1, 17));

    /* Soddy sphere B: k=5, r=1/5, center (27/50, 7/25, sqrt(3)/5) */
    k_B = rat_make(5, 1);
    SB = soddy_make(rat_make(27, 50), rat_make(7, 25),
                    rat_make(3, 25), rat_make(1, 5));

    printf("=== Demo 022: Soddy Spheres -- 3D Descartes Theorem ===\n\n");
    printf("For 5 mutually tangent spheres in R^3:\n");
    printf("  (sum k)^2 = 3(sum k^2)\n\n");

    /* ---- Part 1: Base tangencies ---- */
    printf("--- Part 1: Base configuration (from Demo 021) ---\n");
    {
        d2 = inv_dist2(S1, S2);
        printf("  d^2(S1, S2) = "); rat_print(d2); printf("\n");
        check("d^2(S1, S2) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = inv_dist2(S2, S3);
        printf("  d^2(S2, S3) = "); rat_print(d2); printf("\n");
        check("d^2(S2, S3) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = inv_dist2(S1, S_in);
        printf("  d^2(S1, S_in) = "); rat_print(d2); printf("\n");
        check("d^2(S1, S_in) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = inv_dist2(S1, S_out);
        printf("  d^2(S1, S_out) = "); rat_print(d2); printf("\n");
        check("d^2(S1, S_out) = 1", rat_eq(d2, rat_make(1, 1)));
    }

    /* ---- Part 2: 3D Descartes formula ---- */
    printf("\n--- Part 2: 3D Descartes formula ---\n");
    {
        printf("  Quadratic: 2*k5^2 - 2*S4*k5 + (3*Q4 - S4^2) = 0\n");
        printf("  disc = 12*S4^2 - 24*Q4\n\n");
        printf("  Theorem: 2D Descartes (S4^2 = 2*Q4) => disc = 0.\n");
        printf("  Coplanar base => unique Soddy curvature k5 = S4/2.\n\n");

        /* Inner: (2,3,6,23) */
        S4 = rat_add(rat_add(k1, k2), rat_add(k3, k_in));
        Q4 = rat_add(rat_add(rat_mul(k1, k1), rat_mul(k2, k2)),
                     rat_add(rat_mul(k3, k3), rat_mul(k_in, k_in)));
        disc = rat_sub(rat_mul(rat_make(12, 1), rat_mul(S4, S4)),
                       rat_mul(rat_make(24, 1), Q4));

        printf("  Inner (2,3,6,23): S4 = "); rat_print(S4);
        printf(", Q4 = "); rat_print(Q4); printf("\n");

        lhs = rat_mul(S4, S4);
        rhs = rat_mul(rat_make(2, 1), Q4);
        printf("  2D check: %ld^2 = ", S4.p); rat_print(lhs);
        printf(", 2*%ld = ", Q4.p); rat_print(rhs); printf("\n");
        check("2D Descartes: 34^2 = 2*578", rat_eq(lhs, rhs));

        printf("  disc = "); rat_print(disc); printf("\n");
        check("disc = 0", rat_eq(disc, rat_make(0, 1)));
        printf("  k5 = S4/2 = "); rat_print(rat_div(S4, rat_make(2, 1)));
        printf("\n");
        check("k5 = 17", rat_eq(rat_div(S4, rat_make(2, 1)), k_A));

        printf("\n");

        /* Outer: (2,3,6,-1) */
        S4 = rat_add(rat_add(k1, k2), rat_add(k3, k_out));
        Q4 = rat_add(rat_add(rat_mul(k1, k1), rat_mul(k2, k2)),
                     rat_add(rat_mul(k3, k3), rat_mul(k_out, k_out)));
        disc = rat_sub(rat_mul(rat_make(12, 1), rat_mul(S4, S4)),
                       rat_mul(rat_make(24, 1), Q4));

        printf("  Outer (2,3,6,-1): S4 = "); rat_print(S4);
        printf(", Q4 = "); rat_print(Q4); printf("\n");

        lhs = rat_mul(S4, S4);
        rhs = rat_mul(rat_make(2, 1), Q4);
        check("2D Descartes: 10^2 = 2*50", rat_eq(lhs, rhs));

        printf("  disc = "); rat_print(disc); printf("\n");
        check("disc = 0", rat_eq(disc, rat_make(0, 1)));
        printf("  k5 = S4/2 = "); rat_print(rat_div(S4, rat_make(2, 1)));
        printf("\n");
        check("k5 = 5", rat_eq(rat_div(S4, rat_make(2, 1)), k_B));
    }

    /* ---- Part 3: Soddy sphere centers ---- */
    printf("\n--- Part 3: Soddy sphere centers ---\n");
    {
        printf("  SA (k=17): center (87/170, 1/5, sqrt(3)/17)\n");
        printf("             r = 1/17,  z^2 = 3/289 = 3*r^2\n\n");

        printf("  SB (k=5):  center (27/50, 7/25, sqrt(3)/5)\n");
        printf("             r = 1/5,   z^2 = 3/25  = 3*r^2\n\n");

        printf("  z = sqrt(3)*r: irrational, but z^2 rational.\n");
        printf("  All distance computations remain exact.\n\n");

        /* Verify SA center distances */
        d2 = soddy_flat_dist2(SA, S1);
        printf("  |SA-C1|^2 = "); rat_print(d2);
        printf(" = (1/2+1/17)^2\n");
        check("|SA-C1|^2 = 361/1156", rat_eq(d2, rat_make(361, 1156)));

        d2 = soddy_flat_dist2(SA, S2);
        printf("  |SA-C2|^2 = "); rat_print(d2); printf("\n");
        check("|SA-C2|^2 = 400/2601", rat_eq(d2, rat_make(400, 2601)));

        d2 = soddy_flat_dist2(SA, S3);
        printf("  |SA-C3|^2 = "); rat_print(d2); printf("\n");
        check("|SA-C3|^2 = 529/10404", rat_eq(d2, rat_make(529, 10404)));

        d2 = soddy_flat_dist2(SA, S_in);
        printf("  |SA-C_in|^2 = "); rat_print(d2); printf("\n");
        check("|SA-C_in|^2 = 1600/152881",
              rat_eq(d2, rat_make(1600, 152881)));

        printf("\n");

        /* Verify SB center distances */
        d2 = soddy_flat_dist2(SB, S1);
        printf("  |SB-C1|^2 = "); rat_print(d2); printf("\n");
        check("|SB-C1|^2 = 49/100", rat_eq(d2, rat_make(49, 100)));

        d2 = soddy_flat_dist2(SB, S2);
        printf("  |SB-C2|^2 = "); rat_print(d2); printf("\n");
        check("|SB-C2|^2 = 64/225", rat_eq(d2, rat_make(64, 225)));

        d2 = soddy_flat_dist2(SB, S3);
        printf("  |SB-C3|^2 = "); rat_print(d2); printf("\n");
        check("|SB-C3|^2 = 121/900", rat_eq(d2, rat_make(121, 900)));

        d2 = soddy_flat_dist2(SB, S_out);
        printf("  |SB-C_out|^2 = "); rat_print(d2); printf("\n");
        check("|SB-C_out|^2 = 16/25 = (1-1/5)^2",
              rat_eq(d2, rat_make(16, 25)));
    }

    /* ---- Part 4: Inner Soddy tangencies ---- */
    printf("\n--- Part 4: SA (k=17) tangencies ---\n");
    {
        printf("  All external tangency (SA fits in the gap).\n\n");

        d2 = soddy_flat_d2(SA, S1);
        printf("  d^2(SA, S1) = "); rat_print(d2); printf("\n");
        check("d^2(SA, S1) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = soddy_flat_d2(SA, S2);
        printf("  d^2(SA, S2) = "); rat_print(d2); printf("\n");
        check("d^2(SA, S2) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = soddy_flat_d2(SA, S3);
        printf("  d^2(SA, S3) = "); rat_print(d2); printf("\n");
        check("d^2(SA, S3) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = soddy_flat_d2(SA, S_in);
        printf("  d^2(SA, S_in) = "); rat_print(d2); printf("\n");
        check("d^2(SA, S_in) = 1", rat_eq(d2, rat_make(1, 1)));

        delta = soddy_flat_delta(SA, S1);
        printf("  delta(SA, S1) = "); rat_print(delta);
        printf(" > 0 (external)\n");
        check("delta(SA, S1) > 0", delta.p > 0);
    }

    /* ---- Part 5: Outer Soddy tangencies ---- */
    printf("\n--- Part 5: SB (k=5) tangencies ---\n");
    {
        printf("  External to S1,S2,S3. Internal to S_out.\n\n");

        d2 = soddy_flat_d2(SB, S1);
        printf("  d^2(SB, S1) = "); rat_print(d2); printf("\n");
        check("d^2(SB, S1) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = soddy_flat_d2(SB, S2);
        printf("  d^2(SB, S2) = "); rat_print(d2); printf("\n");
        check("d^2(SB, S2) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = soddy_flat_d2(SB, S3);
        printf("  d^2(SB, S3) = "); rat_print(d2); printf("\n");
        check("d^2(SB, S3) = 1", rat_eq(d2, rat_make(1, 1)));

        d2 = soddy_flat_d2(SB, S_out);
        printf("  d^2(SB, S_out) = "); rat_print(d2); printf("\n");
        check("d^2(SB, S_out) = 1", rat_eq(d2, rat_make(1, 1)));

        delta = soddy_flat_delta(SB, S_out);
        printf("  delta(SB, S_out) = "); rat_print(delta);
        printf(" < 0 (internal)\n");
        check("delta(SB, S_out) < 0", delta.p < 0);
    }

    /* ---- Part 6: Three quintets ---- */
    printf("\n--- Part 6: Three quintets ---\n");
    {
        /* SA-SB mutual tangency */
        printf("  The two Soddy spheres are themselves tangent!\n\n");

        d2 = soddy_pair_dist2(SA, SB);
        printf("  |SA-SB|^2 = "); rat_print(d2);
        printf(" = (1/17+1/5)^2 = (22/85)^2\n");
        check("|SA-SB|^2 = 484/7225", rat_eq(d2, rat_make(484, 7225)));

        d2 = soddy_pair_d2(SA, SB);
        printf("  d^2(SA, SB) = "); rat_print(d2); printf("\n");
        check("d^2(SA, SB) = 1", rat_eq(d2, rat_make(1, 1)));

        delta = soddy_pair_delta(SA, SB);
        printf("  delta(SA, SB) = "); rat_print(delta);
        printf(" > 0 (external)\n");
        check("delta(SA, SB) > 0", delta.p > 0);

        printf("\n  (sum k)^2 = 3(sum k^2) for each quintet:\n\n");

        /* (2,3,6,23,17) */
        sum5 = rat_add(rat_add(rat_add(k1, k2), rat_add(k3, k_in)), k_A);
        sumsq5 = rat_add(
            rat_add(rat_add(rat_mul(k1,k1), rat_mul(k2,k2)),
                    rat_add(rat_mul(k3,k3), rat_mul(k_in,k_in))),
            rat_mul(k_A, k_A));
        lhs = rat_mul(sum5, sum5);
        rhs = rat_mul(rat_make(3, 1), sumsq5);
        printf("  (2+3+6+23+17)^2 = "); rat_print(lhs);
        printf(",  3*867 = "); rat_print(rhs); printf("\n");
        check("(2,3,6,23,17): 2601 = 2601", rat_eq(lhs, rhs));

        /* (2,3,6,-1,5) */
        sum5 = rat_add(rat_add(rat_add(k1, k2), rat_add(k3, k_out)), k_B);
        sumsq5 = rat_add(
            rat_add(rat_add(rat_mul(k1,k1), rat_mul(k2,k2)),
                    rat_add(rat_mul(k3,k3), rat_mul(k_out,k_out))),
            rat_mul(k_B, k_B));
        lhs = rat_mul(sum5, sum5);
        rhs = rat_mul(rat_make(3, 1), sumsq5);
        printf("  (2+3+6-1+5)^2  = "); rat_print(lhs);
        printf(",  3*75  = "); rat_print(rhs); printf("\n");
        check("(2,3,6,-1,5): 225 = 225", rat_eq(lhs, rhs));

        /* (2,3,6,17,5) -- the bonus quintet */
        sum5 = rat_add(rat_add(rat_add(k1, k2), rat_add(k3, k_A)), k_B);
        sumsq5 = rat_add(
            rat_add(rat_add(rat_mul(k1,k1), rat_mul(k2,k2)),
                    rat_add(rat_mul(k3,k3), rat_mul(k_A,k_A))),
            rat_mul(k_B, k_B));
        lhs = rat_mul(sum5, sum5);
        rhs = rat_mul(rat_make(3, 1), sumsq5);
        printf("  (2+3+6+17+5)^2 = "); rat_print(lhs);
        printf(",  3*363 = "); rat_print(rhs); printf("\n");
        check("(2,3,6,17,5): 1089 = 1089", rat_eq(lhs, rhs));
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("3D Descartes: (sum k)^2 = 3(sum k^2) for 5 tangent spheres.\n\n");
    printf("From coplanar circles, disc = 0 => unique Soddy curvature:\n");
    printf("  (2,3,6,23) => k = 17,  (2,3,6,-1) => k = 5\n\n");
    printf("Centers: z = sqrt(3)*r (irrational), z^2 = 3r^2 (rational).\n");
    printf("All tangencies verified: d^2 = 1.\n");
    printf("SA and SB are mutually tangent.\n");
    printf("Three quintets satisfy 3D Descartes:\n");
    printf("  (2,3,6,23,17)  (2,3,6,-1,5)  (2,3,6,17,5)\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
