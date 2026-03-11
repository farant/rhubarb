/*
 * Demo 019: Inversive Distance
 *
 * The inversive product of two spheres S1(C1,r1), S2(C2,r2):
 *   delta(S1,S2) = |C1-C2|^2 - r1^2 - r2^2
 *
 * The squared inversive distance:
 *   d^2(S1,S2) = delta^2 / (4 * r1^2 * r2^2)
 *
 * Classification:
 *   d^2 = 0:  orthogonal (Book XVIII)
 *   d^2 = 1:  tangent (external if delta > 0, internal if delta < 0)
 *   d^2 > 1:  separated (non-intersecting)
 *   d^2 < 1:  intersecting at angle cos^2(theta) = d^2
 *
 * Central theorem: d^2 is preserved by inversion.
 *   Under inversion in unit sphere:
 *     C' = C / (|C|^2 - r^2),  r'^2 = r^2 / (|C|^2 - r^2)^2
 *
 * Key constructions used:
 *   - Inversion (Book XIII)
 *   - Orthogonality (Book XVIII)
 *   - Power of a point (Book XV)
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

/* ---- rational vector ops ---- */

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

static int rvec3_eq(RVec3 a, RVec3 b)
{
    return rat_eq(a.x, b.x) && rat_eq(a.y, b.y) && rat_eq(a.z, b.z);
}

static void rvec3_print(RVec3 v)
{
    printf("("); rat_print(v.x);
    printf(", "); rat_print(v.y);
    printf(", "); rat_print(v.z);
    printf(")");
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

/* ---- sphere operations ---- */

static Sphere sphere_make(RVec3 c, Rat r2)
{
    Sphere s;
    s.c = c; s.r2 = r2;
    return s;
}

/*
 * Inversive product: delta = |C1-C2|^2 - r1^2 - r2^2
 */
static Rat inv_product(Sphere s1, Sphere s2)
{
    return rat_sub(rat_sub(rvec3_len2(rvec3_sub(s1.c, s2.c)),
                           s1.r2), s2.r2);
}

/*
 * Squared inversive distance: d^2 = delta^2 / (4 * r1^2 * r2^2)
 */
static Rat inv_dist2(Sphere s1, Sphere s2)
{
    Rat d = inv_product(s1, s2);
    return rat_div(rat_mul(d, d),
                   rat_mul(rat_make(4, 1), rat_mul(s1.r2, s2.r2)));
}

/*
 * Inversion of a sphere in the unit sphere.
 * Requires pow(O) = |C|^2 - r^2 != 0 (sphere doesn't pass through O).
 *   C' = C / pow,  r'^2 = r^2 / pow^2
 */
static Sphere invert_sphere(Sphere s)
{
    Rat p = rat_sub(rvec3_len2(s.c), s.r2);
    Rat p2 = rat_mul(p, p);
    RVec3 nc = rvec3_make(rat_div(s.c.x, p),
                           rat_div(s.c.y, p),
                           rat_div(s.c.z, p));
    return sphere_make(nc, rat_div(s.r2, p2));
}

/*
 * Check if point lies on sphere.
 */
static int on_sphere(RVec3 pt, Sphere s)
{
    return rat_eq(rvec3_len2(rvec3_sub(pt, s.c)), s.r2);
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
    Sphere U, SA, SB, SC, SD;
    Sphere iSA, iSB, iSC, iSD;
    RVec3 tang_pt;

    /* Unit sphere */
    U = sphere_make(rvec3_from_ints(0, 0, 0), rat_make(1, 1));

    /* SA: center (2,0,0), r^2=1 — externally tangent to U at (1,0,0) */
    SA = sphere_make(rvec3_from_ints(2, 0, 0), rat_make(1, 1));

    /* SB: center (0,3,0), r^2=1 — externally tangent to U at (0,1,0) */
    SB = sphere_make(rvec3_from_ints(0, 3, 0), rat_make(1, 1));

    /* SC: center (3,0,0), r^2=4 — externally tangent to U, internally to SA */
    SC = sphere_make(rvec3_from_ints(3, 0, 0), rat_make(4, 1));

    /* SD: center (2,0,-1), r^2=4 — orthogonal to U (from Book XVIII) */
    SD = sphere_make(rvec3_from_ints(2, 0, -1), rat_make(4, 1));

    /* Inversions */
    iSA = invert_sphere(SA);
    iSB = invert_sphere(SB);
    iSC = invert_sphere(SC);
    iSD = invert_sphere(SD);

    tang_pt = rvec3_from_ints(1, 0, 0);

    printf("=== Demo 019: Inversive Distance ===\n\n");
    printf("delta(S1,S2) = |C1-C2|^2 - r1^2 - r2^2\n");
    printf("d^2(S1,S2) = delta^2 / (4 * r1^2 * r2^2)\n\n");

    /* ---- Part 1: Inversive product ---- */
    printf("--- Part 1: Inversive product delta ---\n");
    {
        Rat d;

        printf("  delta > 0: external side.  delta = 0: orthogonal.\n");
        printf("  delta < 0: overlapping side.\n\n");

        d = inv_product(U, SA);
        printf("  delta(U, SA) = "); rat_print(d); printf("\n");
        check("delta(U, SA) = 2 (ext. tangent)", rat_eq(d, rat_make(2, 1)));

        d = inv_product(U, SC);
        printf("  delta(U, SC) = "); rat_print(d); printf("\n");
        check("delta(U, SC) = 4 (ext. tangent)", rat_eq(d, rat_make(4, 1)));

        d = inv_product(SA, SC);
        printf("  delta(SA, SC) = "); rat_print(d); printf("\n");
        check("delta(SA, SC) = -4 (int. tangent)",
              rat_eq(d, rat_make(-4, 1)));

        d = inv_product(U, SD);
        printf("  delta(U, SD) = "); rat_print(d); printf("\n");
        check("delta(U, SD) = 0 (orthogonal)", rat_eq(d, rat_make(0, 1)));

        d = inv_product(SA, SB);
        printf("  delta(SA, SB) = "); rat_print(d); printf("\n");
        check("delta(SA, SB) = 11 (well separated)",
              rat_eq(d, rat_make(11, 1)));
    }

    /* ---- Part 2: Squared inversive distance ---- */
    printf("\n--- Part 2: Squared inversive distance d^2 ---\n");
    {
        Rat d2;

        printf("  d^2 = 0: orthogonal.  d^2 = 1: tangent.  d^2 > 1: separated.\n\n");

        d2 = inv_dist2(U, SA);
        printf("  d^2(U, SA) = "); rat_print(d2); printf("\n");
        check("d^2(U, SA) = 1 (tangent)", rat_eq(d2, rat_make(1, 1)));

        d2 = inv_dist2(SA, SC);
        printf("  d^2(SA, SC) = "); rat_print(d2); printf("\n");
        check("d^2(SA, SC) = 1 (tangent)", rat_eq(d2, rat_make(1, 1)));

        d2 = inv_dist2(U, SD);
        printf("  d^2(U, SD) = "); rat_print(d2); printf("\n");
        check("d^2(U, SD) = 0 (orthogonal)", rat_eq(d2, rat_make(0, 1)));

        d2 = inv_dist2(SA, SB);
        printf("  d^2(SA, SB) = "); rat_print(d2); printf("\n");
        check("d^2(SA, SB) = 121/4 (separated)",
              rat_eq(d2, rat_make(121, 4)));
    }

    /* ---- Part 3: Sphere inversion ---- */
    printf("\n--- Part 3: Inversion of spheres in unit sphere ---\n");
    {
        printf("  C' = C / (|C|^2 - r^2),  r'^2 = r^2 / (|C|^2 - r^2)^2\n\n");

        printf("  Inv(SA): center "); rvec3_print(iSA.c);
        printf(", r^2 = "); rat_print(iSA.r2); printf("\n");
        check("Inv(SA) = ((2/3,0,0), 1/9)",
              rvec3_eq(iSA.c, rvec3_make(rat_make(2,3),
                  rat_make(0,1), rat_make(0,1)))
              && rat_eq(iSA.r2, rat_make(1, 9)));

        printf("  Inv(SB): center "); rvec3_print(iSB.c);
        printf(", r^2 = "); rat_print(iSB.r2); printf("\n");
        check("Inv(SB) = ((0,3/8,0), 1/64)",
              rvec3_eq(iSB.c, rvec3_make(rat_make(0,1),
                  rat_make(3,8), rat_make(0,1)))
              && rat_eq(iSB.r2, rat_make(1, 64)));

        printf("  Inv(SC): center "); rvec3_print(iSC.c);
        printf(", r^2 = "); rat_print(iSC.r2); printf("\n");
        check("Inv(SC) = ((3/5,0,0), 4/25)",
              rvec3_eq(iSC.c, rvec3_make(rat_make(3,5),
                  rat_make(0,1), rat_make(0,1)))
              && rat_eq(iSC.r2, rat_make(4, 25)));

        printf("  Inv(SD): center "); rvec3_print(iSD.c);
        printf(", r^2 = "); rat_print(iSD.r2); printf("\n");
        check("Inv(SD) = SD (orthogonal to U, hence fixed)",
              rvec3_eq(iSD.c, SD.c) && rat_eq(iSD.r2, SD.r2));
    }

    /* ---- Part 4: d^2 preserved by inversion ---- */
    printf("\n--- Part 4: d^2 preserved by inversion ---\n");
    {
        Rat d2_before, d2_after;

        printf("  d^2(Inv(S1), Inv(S2)) = d^2(S1, S2) always.\n\n");

        /* U is fixed by inversion, so (U, Inv(SA)) vs (U, SA) */
        d2_before = inv_dist2(U, SA);
        d2_after = inv_dist2(U, iSA);
        printf("  d^2(U, SA) = "); rat_print(d2_before);
        printf(",  d^2(U, Inv(SA)) = "); rat_print(d2_after); printf("\n");
        check("d^2 preserved: (U, SA) -> (U, Inv(SA))",
              rat_eq(d2_before, d2_after));

        d2_before = inv_dist2(SA, SC);
        d2_after = inv_dist2(iSA, iSC);
        printf("  d^2(SA, SC) = "); rat_print(d2_before);
        printf(",  d^2(Inv(SA), Inv(SC)) = "); rat_print(d2_after);
        printf("\n");
        check("d^2 preserved: (SA, SC)",
              rat_eq(d2_before, d2_after));

        d2_before = inv_dist2(SA, SB);
        d2_after = inv_dist2(iSA, iSB);
        printf("  d^2(SA, SB) = "); rat_print(d2_before);
        printf(",  d^2(Inv(SA), Inv(SB)) = "); rat_print(d2_after);
        printf("\n");
        check("d^2 preserved: (SA, SB)",
              rat_eq(d2_before, d2_after));

        d2_before = inv_dist2(U, SC);
        d2_after = inv_dist2(U, iSC);
        printf("  d^2(U, SC) = "); rat_print(d2_before);
        printf(",  d^2(U, Inv(SC)) = "); rat_print(d2_after); printf("\n");
        check("d^2 preserved: (U, SC)",
              rat_eq(d2_before, d2_after));

        /* SA and SD: Inv(SD) = SD */
        d2_before = inv_dist2(SA, SD);
        d2_after = inv_dist2(iSA, iSD);
        printf("  d^2(SA, SD) = "); rat_print(d2_before);
        printf(",  d^2(Inv(SA), SD) = "); rat_print(d2_after); printf("\n");
        check("d^2 preserved: (SA, SD)", rat_eq(d2_before, d2_after));
    }

    /* ---- Part 5: Tangent chain ---- */
    printf("\n--- Part 5: Tangent chain U, SA, SC ---\n");
    {
        Rat d2;

        printf("  U, SA, SC are pairwise tangent (d^2 = 1).\n");
        printf("  All three meet at the point (1,0,0).\n\n");

        d2 = inv_dist2(U, SC);
        printf("  d^2(U, SC) = "); rat_print(d2); printf("\n");
        check("d^2(U, SC) = 1 (completing tangent triple)",
              rat_eq(d2, rat_make(1, 1)));

        check("(1,0,0) on U, SA, and SC",
              on_sphere(tang_pt, U) && on_sphere(tang_pt, SA)
              && on_sphere(tang_pt, SC));
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("Inversive distance: the projective invariant of sphere pairs.\n\n");
    printf("  delta = |C1-C2|^2 - r1^2 - r2^2  (inversive product)\n");
    printf("  d^2 = delta^2 / (4*r1^2*r2^2)     (squared inv. distance)\n\n");
    printf("Classification:\n");
    printf("  d^2 = 0: orthogonal (Book XVIII)\n");
    printf("  d^2 = 1: tangent\n");
    printf("  d^2 > 1: separated\n");
    printf("  d^2 < 1: intersecting (cos^2(angle) = d^2)\n\n");
    printf("Central result: d^2 is preserved by inversion.\n");
    printf("  Orthogonal spheres (d^2=0) are fixed by inversion.\n");
    printf("  Tangent spheres (d^2=1) remain tangent after inversion.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
