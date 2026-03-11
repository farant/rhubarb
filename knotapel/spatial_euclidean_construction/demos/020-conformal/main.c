/*
 * Demo 020: Conformal Property of Inversion
 *
 * Inversion in a sphere preserves angles between surfaces.
 * For two spheres S1, S2 meeting at point P at angle theta,
 * their images Inv(S1), Inv(S2) meet at Inv(P) at the same angle.
 *
 * The angle formula at intersection point P:
 *   cos^2(theta) = ((P-C1).(P-C2))^2 / (r1^2 * r2^2)
 *
 * This equals the squared inversive distance d^2 (Book XIX):
 *   d^2 preserved by inversion  <=>  angles preserved by inversion
 *
 * Additionally verified:
 *   - Plane not through O inverts to sphere; angle still preserved
 *   - Plane through O stays fixed; angle still preserved
 *
 * Key constructions used:
 *   - Inversion (Book XIII, XIX)
 *   - Inversive distance (Book XIX)
 *   - Dihedral angle cos^2 (Book V)
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

/* ---- inversion ---- */

/*
 * Invert point in unit sphere: P -> P / |P|^2
 */
static RVec3 invert_point(RVec3 p)
{
    Rat r2 = rvec3_len2(p);
    return rvec3_make(rat_div(p.x, r2), rat_div(p.y, r2),
                      rat_div(p.z, r2));
}

/*
 * Invert sphere in unit sphere.
 * pow = |C|^2 - r^2.  C' = C/pow,  r'^2 = r^2/pow^2.
 */
static Sphere invert_sphere(Sphere s)
{
    Rat pw = rat_sub(rvec3_len2(s.c), s.r2);
    Rat pw2 = rat_mul(pw, pw);
    RVec3 nc = rvec3_make(rat_div(s.c.x, pw),
                           rat_div(s.c.y, pw),
                           rat_div(s.c.z, pw));
    return sphere_make(nc, rat_div(s.r2, pw2));
}

/*
 * Invert plane n.X = d (d != 0, plane not through O) to sphere.
 * Image: center = n/(2d), r^2 = |n|^2 / (4d^2)
 */
static Sphere invert_plane(RVec3 n, Rat d)
{
    Rat two_d = rat_mul(rat_make(2, 1), d);
    Rat four_d2 = rat_mul(rat_make(4, 1), rat_mul(d, d));
    RVec3 c = rvec3_make(rat_div(n.x, two_d), rat_div(n.y, two_d),
                          rat_div(n.z, two_d));
    return sphere_make(c, rat_div(rvec3_len2(n), four_d2));
}

/* ---- angle computation ---- */

/*
 * cos^2 of angle between two spheres at intersection point P.
 *   cos^2(theta) = ((P-C1).(P-C2))^2 / (r1^2 * r2^2)
 */
static Rat cos2_spheres(Sphere s1, Sphere s2, RVec3 p)
{
    RVec3 n1 = rvec3_sub(p, s1.c);
    RVec3 n2 = rvec3_sub(p, s2.c);
    Rat d = rvec3_dot(n1, n2);
    return rat_div(rat_mul(d, d), rat_mul(s1.r2, s2.r2));
}

/*
 * cos^2 of angle between sphere and plane at intersection point P.
 *   cos^2(theta) = ((P-C).n_plane)^2 / (r^2 * |n_plane|^2)
 */
static Rat cos2_sphere_plane(Sphere s, RVec3 n_pl, RVec3 p)
{
    RVec3 ns = rvec3_sub(p, s.c);
    Rat d = rvec3_dot(ns, n_pl);
    return rat_div(rat_mul(d, d), rat_mul(s.r2, rvec3_len2(n_pl)));
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
    /* Three spheres through P = (2,0,0), all with pow(O) = 8 */
    RVec3 P, Pp;
    Sphere S1, S2, S3;
    Sphere iS1, iS2, iS3;

    /* Planes */
    RVec3 n_pi1, n_pi2, n_y0;
    Rat d_pi1, d_pi2;
    Sphere Sp1, Sp2;

    Rat c2, c2p;

    P = rvec3_from_ints(2, 0, 0);

    S1 = sphere_make(rvec3_from_ints(3, 0, 0), rat_make(1, 1));
    S2 = sphere_make(rvec3_from_ints(3, 1, 0), rat_make(2, 1));
    S3 = sphere_make(rvec3_from_ints(3, 1, 1), rat_make(3, 1));

    iS1 = invert_sphere(S1);
    iS2 = invert_sphere(S2);
    iS3 = invert_sphere(S3);

    Pp = invert_point(P);

    /* Planes through P */
    n_pi1 = rvec3_from_ints(1, 1, 0);     /* x + y = 2 */
    d_pi1 = rat_make(2, 1);
    n_pi2 = rvec3_from_ints(1, 1, 1);     /* x + y + z = 2 */
    d_pi2 = rat_make(2, 1);
    n_y0  = rvec3_from_ints(0, 1, 0);     /* y = 0 (through O, fixed) */

    Sp1 = invert_plane(n_pi1, d_pi1);
    Sp2 = invert_plane(n_pi2, d_pi2);

    printf("=== Demo 020: Conformal Property of Inversion ===\n\n");
    printf("Inversion preserves angles between surfaces.\n");
    printf("cos^2(theta) = ((P-C1).(P-C2))^2 / (r1^2 * r2^2)\n\n");

    /* ---- Part 1: Three spheres meet at P ---- */
    printf("--- Part 1: Three spheres meet at P = (2,0,0) ---\n");
    {
        printf("  S1 = ((3,0,0), r^2=1)\n");
        printf("  S2 = ((3,1,0), r^2=2)\n");
        printf("  S3 = ((3,1,1), r^2=3)\n\n");

        check("P on S1, S2, and S3",
              on_sphere(P, S1) && on_sphere(P, S2) && on_sphere(P, S3));

        c2 = cos2_spheres(S1, S2, P);
        printf("  cos^2(S1, S2) = "); rat_print(c2); printf("\n");
        check("cos^2(S1, S2) = 1/2", rat_eq(c2, rat_make(1, 2)));

        c2 = cos2_spheres(S1, S3, P);
        printf("  cos^2(S1, S3) = "); rat_print(c2); printf("\n");
        check("cos^2(S1, S3) = 1/3", rat_eq(c2, rat_make(1, 3)));

        c2 = cos2_spheres(S2, S3, P);
        printf("  cos^2(S2, S3) = "); rat_print(c2); printf("\n");
        check("cos^2(S2, S3) = 2/3", rat_eq(c2, rat_make(2, 3)));
    }

    /* ---- Part 2: Inversion carries P to P' ---- */
    printf("\n--- Part 2: Inversion carries P to P' ---\n");
    {
        printf("  Inv(P) = "); rvec3_print(Pp); printf("\n");
        check("Inv(P) = (1/2, 0, 0)",
              rvec3_eq(Pp, rvec3_make(rat_make(1,2),
                  rat_make(0,1), rat_make(0,1))));

        printf("  Inv(S1): center "); rvec3_print(iS1.c);
        printf(", r^2 = "); rat_print(iS1.r2); printf("\n");
        check("P' on Inv(S1)", on_sphere(Pp, iS1));

        printf("  Inv(S2): center "); rvec3_print(iS2.c);
        printf(", r^2 = "); rat_print(iS2.r2); printf("\n");
        check("P' on Inv(S2)", on_sphere(Pp, iS2));

        printf("  Inv(S3): center "); rvec3_print(iS3.c);
        printf(", r^2 = "); rat_print(iS3.r2); printf("\n");
        check("P' on Inv(S3)", on_sphere(Pp, iS3));
    }

    /* ---- Part 3: Sphere-sphere angles preserved ---- */
    printf("\n--- Part 3: Sphere-sphere angles preserved ---\n");
    {
        printf("  cos^2 at P vs cos^2 at P' for image spheres:\n\n");

        c2 = cos2_spheres(S1, S2, P);
        c2p = cos2_spheres(iS1, iS2, Pp);
        printf("  cos^2(S1,S2) = "); rat_print(c2);
        printf("  ->  cos^2(S1',S2') = "); rat_print(c2p); printf("\n");
        check("angle(S1,S2) preserved: 1/2", rat_eq(c2, c2p));

        c2 = cos2_spheres(S1, S3, P);
        c2p = cos2_spheres(iS1, iS3, Pp);
        printf("  cos^2(S1,S3) = "); rat_print(c2);
        printf("  ->  cos^2(S1',S3') = "); rat_print(c2p); printf("\n");
        check("angle(S1,S3) preserved: 1/3", rat_eq(c2, c2p));

        c2 = cos2_spheres(S2, S3, P);
        c2p = cos2_spheres(iS2, iS3, Pp);
        printf("  cos^2(S2,S3) = "); rat_print(c2);
        printf("  ->  cos^2(S2',S3') = "); rat_print(c2p); printf("\n");
        check("angle(S2,S3) preserved: 2/3", rat_eq(c2, c2p));
    }

    /* ---- Part 4: Plane-sphere angles preserved ---- */
    printf("\n--- Part 4: Plane becomes sphere, angle preserved ---\n");
    {
        printf("  Plane not through O inverts to sphere.\n\n");

        /* Plane x+y=2 */
        c2 = cos2_sphere_plane(S1, n_pi1, P);
        printf("  cos^2(S1, plane x+y=2) at P = "); rat_print(c2);
        printf("\n");
        check("cos^2(S1, plane x+y=2) = 1/2",
              rat_eq(c2, rat_make(1, 2)));

        printf("  Image of x+y=2: sphere center ");
        rvec3_print(Sp1.c);
        printf(", r^2 = "); rat_print(Sp1.r2); printf("\n");
        check("P' on image of plane x+y=2", on_sphere(Pp, Sp1));

        c2p = cos2_spheres(iS1, Sp1, Pp);
        printf("  cos^2(S1', image) at P' = "); rat_print(c2p);
        printf("\n");
        check("angle(S1, plane x+y=2) preserved: 1/2",
              rat_eq(c2, c2p));

        /* Plane x+y+z=2 */
        c2 = cos2_sphere_plane(S1, n_pi2, P);
        printf("  cos^2(S1, plane x+y+z=2) at P = "); rat_print(c2);
        printf("\n");
        check("cos^2(S1, plane x+y+z=2) = 1/3",
              rat_eq(c2, rat_make(1, 3)));

        c2p = cos2_spheres(iS1, Sp2, Pp);
        printf("  cos^2(S1', image) at P' = "); rat_print(c2p);
        printf("\n");
        check("angle(S1, plane x+y+z=2) preserved: 1/3",
              rat_eq(c2, c2p));
    }

    /* ---- Part 5: Unification: cos^2(theta) = d^2 ---- */
    printf("\n--- Part 5: Unification: cos^2(theta) = d^2 ---\n");
    {
        Rat d2;

        printf("  For intersecting spheres: angle = inversive distance.\n");
        printf("  d^2 preserved (Thm XIX.3) <==> conformal property.\n\n");

        d2 = inv_dist2(S1, S2);
        c2 = cos2_spheres(S1, S2, P);
        printf("  d^2(S1,S2) = "); rat_print(d2);
        printf(",  cos^2 = "); rat_print(c2); printf("\n");
        check("d^2 = cos^2 for S1,S2 (= 1/2)", rat_eq(d2, c2));

        d2 = inv_dist2(S1, S3);
        c2 = cos2_spheres(S1, S3, P);
        printf("  d^2(S1,S3) = "); rat_print(d2);
        printf(",  cos^2 = "); rat_print(c2); printf("\n");
        check("d^2 = cos^2 for S1,S3 (= 1/3)", rat_eq(d2, c2));

        d2 = inv_dist2(S2, S3);
        c2 = cos2_spheres(S2, S3, P);
        printf("  d^2(S2,S3) = "); rat_print(d2);
        printf(",  cos^2 = "); rat_print(c2); printf("\n");
        check("d^2 = cos^2 for S2,S3 (= 2/3)", rat_eq(d2, c2));

        /* Plane through O (y=0): fixed by inversion */
        c2 = cos2_sphere_plane(S3, n_y0, P);
        c2p = cos2_sphere_plane(iS3, n_y0, Pp);
        printf("  cos^2(S3, y=0) at P = "); rat_print(c2);
        printf("  ->  cos^2(S3', y=0) at P' = "); rat_print(c2p);
        printf("\n");
        check("plane through O (y=0): angle preserved (1/3)",
              rat_eq(c2, c2p));
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("Inversion is conformal: it preserves angles between surfaces.\n\n");
    printf("Three demonstrations:\n");
    printf("  1. Sphere-sphere: cos^2 at P = cos^2 at Inv(P)\n");
    printf("  2. Sphere-plane: plane -> sphere, angle preserved\n");
    printf("  3. cos^2(theta) = d^2 (Book XIX): same invariant\n\n");
    printf("The conformal property explains WHY inversive distance,\n");
    printf("cross-ratio, and tangency are all preserved by inversion.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
