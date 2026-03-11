/*
 * Demo 018: Pencils of Spheres
 *
 * A pencil is a one-parameter family of spheres sharing a radical plane.
 * The pencil through the unit sphere and circumsphere:
 *
 *   center(t) = (t/2, t/2, t/2)
 *   r^2(t) = 3t^2/4 + 1 - t
 *
 * t=0: unit sphere (center O, r^2=1)
 * t=1: circumsphere (center K, r^2=3/4)
 *
 * Key results:
 *   - All members share a common circle on radical plane x+y+z=1
 *   - Common circle: center (1/3,1/3,1/3), radius^2 = 2/3
 *   - Power on radical plane is |P|^2 - 1, independent of t
 *   - Pencil centers trace the Euler line x=y=z
 *   - Orthogonality to generators => orthogonality to all members
 *
 * Key constructions used:
 *   - Radical planes (Book III / Book XV)
 *   - Power of a point (Book XV)
 *   - Perpendicular foot (Book V)
 *   - Named points from Book IX
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long p, q; } Rat;
typedef struct { Rat x, y, z; } RVec3;

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

/* ---- pencil of spheres ---- */

/*
 * Pencil through unit sphere (t=0) and circumsphere (t=1).
 * Derived from: (1-t)(|X|^2-1) + t(|X|^2-x-y-z) = 0
 *   => |X-(t/2,t/2,t/2)|^2 = 3t^2/4 + 1 - t
 */
static RVec3 pencil_center(Rat t)
{
    Rat h = rat_div(t, rat_make(2, 1));
    return rvec3_make(h, h, h);
}

static Rat pencil_r2(Rat t)
{
    Rat t2 = rat_mul(t, t);
    Rat term1 = rat_div(rat_mul(rat_make(3, 1), t2), rat_make(4, 1));
    return rat_add(rat_sub(term1, t), rat_make(1, 1));
}

/* ---- sphere operations ---- */

static Rat sphere_power(RVec3 p, RVec3 center, Rat r2)
{
    return rat_sub(rvec3_len2(rvec3_sub(p, center)), r2);
}

static int on_sphere(RVec3 p, RVec3 center, Rat r2)
{
    return rat_eq(sphere_power(p, center, r2), rat_make(0, 1));
}

/* Orthogonality: |C1-C2|^2 = r1^2 + r2^2 */
static int orthogonal(RVec3 c1, Rat r2_1, RVec3 c2, Rat r2_2)
{
    return rat_eq(rvec3_len2(rvec3_sub(c1, c2)), rat_add(r2_1, r2_2));
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
    RVec3 O, A, B, C, K, G;
    RVec3 face_cen;

    O = rvec3_from_ints(0, 0, 0);
    A = rvec3_from_ints(1, 0, 0);
    B = rvec3_from_ints(0, 1, 0);
    C = rvec3_from_ints(0, 0, 1);
    K = rvec3_make(rat_make(1,2), rat_make(1,2), rat_make(1,2));
    G = rvec3_make(rat_make(1,4), rat_make(1,4), rat_make(1,4));
    face_cen = rvec3_make(rat_make(1,3), rat_make(1,3), rat_make(1,3));

    printf("=== Demo 018: Pencils of Spheres ===\n\n");
    printf("Pencil through unit sphere and circumsphere:\n");
    printf("  center(t) = (t/2, t/2, t/2)\n");
    printf("  r^2(t) = 3t^2/4 + 1 - t\n\n");

    /* ---- Part 1: Pencil members ---- */
    printf("--- Part 1: Pencil members ---\n");
    {
        RVec3 cen;
        Rat r2;

        printf("  t=0: unit sphere.  t=1: circumsphere.\n\n");

        /* t=0: unit sphere */
        cen = pencil_center(rat_make(0, 1));
        r2 = pencil_r2(rat_make(0, 1));
        printf("  t=0: center = "); rvec3_print(cen);
        printf(", r^2 = "); rat_print(r2); printf("\n");
        check("t=0: unit sphere (center O, r^2=1)",
              rvec3_eq(cen, O) && rat_eq(r2, rat_make(1, 1)));

        /* t=1: circumsphere */
        cen = pencil_center(rat_make(1, 1));
        r2 = pencil_r2(rat_make(1, 1));
        printf("  t=1: center = "); rvec3_print(cen);
        printf(", r^2 = "); rat_print(r2); printf("\n");
        check("t=1: circumsphere (center K, r^2=3/4)",
              rvec3_eq(cen, K) && rat_eq(r2, rat_make(3, 4)));

        /* t=1/2: centered at G */
        cen = pencil_center(rat_make(1, 2));
        r2 = pencil_r2(rat_make(1, 2));
        printf("  t=1/2: center = "); rvec3_print(cen);
        printf(", r^2 = "); rat_print(r2); printf("\n");
        check("t=1/2: center = G, r^2 = 11/16",
              rvec3_eq(cen, G) && rat_eq(r2, rat_make(11, 16)));

        /* t=2: centered at (1,1,1) */
        cen = pencil_center(rat_make(2, 1));
        r2 = pencil_r2(rat_make(2, 1));
        printf("  t=2: center = "); rvec3_print(cen);
        printf(", r^2 = "); rat_print(r2); printf("\n");
        check("t=2: center = (1,1,1), r^2 = 2",
              rvec3_eq(cen, rvec3_from_ints(1, 1, 1))
              && rat_eq(r2, rat_make(2, 1)));
    }

    /* ---- Part 2: Common circle ---- */
    printf("\n--- Part 2: Common circle (all members through A, B, C) ---\n");
    {
        RVec3 cen;
        Rat r2;

        printf("  A, B, C lie on both generators and on x+y+z=1.\n");
        printf("  They lie on EVERY pencil member.\n\n");

        /* A on t=1/2 */
        cen = pencil_center(rat_make(1, 2));
        r2 = pencil_r2(rat_make(1, 2));
        check("A on pencil(t=1/2)", on_sphere(A, cen, r2));

        /* B on t=2 */
        cen = pencil_center(rat_make(2, 1));
        r2 = pencil_r2(rat_make(2, 1));
        check("B on pencil(t=2)", on_sphere(B, cen, r2));

        /* C on t=-1 */
        cen = pencil_center(rat_make(-1, 1));
        r2 = pencil_r2(rat_make(-1, 1));
        check("C on pencil(t=-1)", on_sphere(C, cen, r2));

        /* A on t=2/3 */
        cen = pencil_center(rat_make(2, 3));
        r2 = pencil_r2(rat_make(2, 3));
        check("A on pencil(t=2/3)", on_sphere(A, cen, r2));
    }

    /* ---- Part 3: Power constant on radical plane ---- */
    printf("\n--- Part 3: Power constant on radical plane x+y+z=1 ---\n");
    {
        Rat pow1, pow2;
        RVec3 cen, mid_AB;
        Rat r2;

        mid_AB = rvec3_make(rat_make(1,2), rat_make(1,2), rat_make(0,1));

        printf("  For P on x+y+z=1: pow(P, t) = |P|^2 - 1, any t.\n\n");

        /* centroid(ABC): |P|^2 = 1/3, power = -2/3 */
        cen = pencil_center(rat_make(1, 2));
        r2 = pencil_r2(rat_make(1, 2));
        pow1 = sphere_power(face_cen, cen, r2);

        cen = pencil_center(rat_make(2, 1));
        r2 = pencil_r2(rat_make(2, 1));
        pow2 = sphere_power(face_cen, cen, r2);

        printf("  centroid_ABC: pow(t=1/2) = ");
        rat_print(pow1);
        printf(", pow(t=2) = ");
        rat_print(pow2); printf("\n");
        check("pow(centroid_ABC, t=1/2) = -2/3",
              rat_eq(pow1, rat_make(-2, 3)));
        check("pow(centroid_ABC, t=2) = -2/3",
              rat_eq(pow2, rat_make(-2, 3)));

        /* mid(A,B): |P|^2 = 1/2, power = -1/2 */
        cen = pencil_center(rat_make(0, 1));
        r2 = pencil_r2(rat_make(0, 1));
        pow1 = sphere_power(mid_AB, cen, r2);

        cen = pencil_center(rat_make(2, 1));
        r2 = pencil_r2(rat_make(2, 1));
        pow2 = sphere_power(mid_AB, cen, r2);

        printf("  mid(A,B):     pow(t=0) = ");
        rat_print(pow1);
        printf(", pow(t=2) = ");
        rat_print(pow2); printf("\n");
        check("pow(mid_AB, t=0) = -1/2",
              rat_eq(pow1, rat_make(-1, 2)));
        check("pow(mid_AB, t=2) = -1/2",
              rat_eq(pow2, rat_make(-1, 2)));
    }

    /* ---- Part 4: Common circle geometry ---- */
    printf("\n--- Part 4: Common circle geometry ---\n");
    {
        Rat d2_unit, d2_circ;
        Rat cr2_unit, cr2_circ;
        RVec3 pc;
        Rat pr2;

        printf("  Common circle = intersection of all members with x+y+z=1.\n");
        printf("  Center = foot from sphere center to radical plane.\n\n");

        /* From unit sphere: circle r^2 = 1 - |face_cen|^2 = 1 - 1/3 = 2/3 */
        d2_unit = rvec3_len2(face_cen);
        cr2_unit = rat_sub(rat_make(1, 1), d2_unit);
        printf("  Unit sphere: 1 - d^2(O, face_cen) = 1 - ");
        rat_print(d2_unit);
        printf(" = "); rat_print(cr2_unit); printf("\n");
        check("Circle r^2 from unit sphere = 2/3",
              rat_eq(cr2_unit, rat_make(2, 3)));

        /* From circumsphere: circle r^2 = 3/4 - |K - face_cen|^2 */
        d2_circ = rvec3_len2(rvec3_sub(K, face_cen));
        cr2_circ = rat_sub(rat_make(3, 4), d2_circ);
        printf("  Circumsphere: 3/4 - d^2(K, face_cen) = 3/4 - ");
        rat_print(d2_circ);
        printf(" = "); rat_print(cr2_circ); printf("\n");
        check("Circle r^2 from circumsphere = 2/3 (same!)",
              rat_eq(cr2_circ, rat_make(2, 3)));

        /* Pencil at t=2/3: center = circle center, r^2 = circle r^2 */
        pc = pencil_center(rat_make(2, 3));
        pr2 = pencil_r2(rat_make(2, 3));
        printf("  Pencil t=2/3: center = "); rvec3_print(pc);
        printf(", r^2 = "); rat_print(pr2); printf("\n");
        check("t=2/3: center = circle center, r^2 = 2/3",
              rvec3_eq(pc, face_cen) && rat_eq(pr2, rat_make(2, 3)));
    }

    /* ---- Part 5: Orthogonality propagates ---- */
    printf("\n--- Part 5: Orthogonality propagates through pencil ---\n");
    {
        RVec3 s3_c, s4_c;
        Rat s3_r2, s4_r2;
        RVec3 pc;
        Rat pr2;

        s3_c = rvec3_from_ints(2, 0, -1);
        s3_r2 = rat_make(4, 1);
        s4_c = rvec3_from_ints(1, 1, -1);
        s4_r2 = rat_make(2, 1);

        printf("  If S is orthogonal to both generators (|C1-C2|^2 = r1^2+r2^2),\n");
        printf("  then S is orthogonal to every pencil member.\n");
        printf("  (Centers of orthogonal spheres lie on the radical plane.)\n\n");

        /* S3 orthogonal to unit sphere */
        printf("  S3: center "); rvec3_print(s3_c);
        printf(", r^2 = "); rat_print(s3_r2); printf("\n");
        check("S3 orth. to unit sphere",
              orthogonal(s3_c, s3_r2, O, rat_make(1, 1)));

        /* S3 orthogonal to circumsphere */
        check("S3 orth. to circumsphere",
              orthogonal(s3_c, s3_r2, K, rat_make(3, 4)));

        /* S3 orthogonal to t=1/2 */
        pc = pencil_center(rat_make(1, 2));
        pr2 = pencil_r2(rat_make(1, 2));
        check("S3 orth. to pencil(t=1/2)",
              orthogonal(s3_c, s3_r2, pc, pr2));

        /* S3 orthogonal to t=2 */
        pc = pencil_center(rat_make(2, 1));
        pr2 = pencil_r2(rat_make(2, 1));
        check("S3 orth. to pencil(t=2)",
              orthogonal(s3_c, s3_r2, pc, pr2));

        /* S4 orthogonal to t=1/2 */
        printf("  S4: center "); rvec3_print(s4_c);
        printf(", r^2 = "); rat_print(s4_r2); printf("\n");
        pc = pencil_center(rat_make(1, 2));
        pr2 = pencil_r2(rat_make(1, 2));
        check("S4 orth. to pencil(t=1/2)",
              orthogonal(s4_c, s4_r2, pc, pr2));
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("A pencil of spheres: one-parameter family, one radical plane.\n\n");
    printf("The unit sphere / circumsphere pencil:\n");
    printf("  center(t) = (t/2, t/2, t/2)  (the Euler line!)\n");
    printf("  r^2(t) = 3t^2/4 + 1 - t      (always positive)\n");
    printf("  Radical plane: x+y+z=1        (the face plane, again)\n\n");
    printf("Key results:\n");
    printf("  All members share the common circle on x+y+z=1\n");
    printf("  Circle: center (1/3,1/3,1/3), radius^2 = 2/3\n");
    printf("  Power on radical plane = |P|^2 - 1 (constant across pencil)\n");
    printf("  Orthogonality to generators => orthogonality to all members\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
