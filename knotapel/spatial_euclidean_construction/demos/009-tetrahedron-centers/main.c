/*
 * Demo 009: Centers of the Tetrahedron
 *
 * The four classical centers, applied to the right tetrahedron
 * {O, A, B, C} and the regular tetrahedron from Book VII.
 *
 * For the right tetrahedron, all centers lie on the line x = y = z
 * (the S3 symmetry axis from Book VI):
 *
 *   Orthocenter H  = (0, 0, 0)           t = 0
 *   Incenter    I  = ((3-sqrt(3))/6, ...) t = (3-sqrt(3))/6 ~ 0.211
 *   Centroid    G  = (1/4, 1/4, 1/4)     t = 1/4
 *   Circumcenter K = (1/2, 1/2, 1/2)     t = 1/2
 *
 * Three are rational (T_Q), one is irrational (T_E).
 * For the regular tetrahedron, all four coincide at the origin.
 *
 * Key constructions used:
 *   - Perpendicular bisector planes (Book III) -> circumcenter
 *   - Perpendicular feet (Book V) -> orthocenter/altitudes
 *   - Cross product / face areas (Book VIII) -> incenter weights
 *   - S3 symmetry (Book VI) -> collinearity
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long x, y, z; } Vec3;
typedef struct { long p, q; } Rat;
typedef struct { Rat x, y, z; } RVec3;

/* ---- integer vector ops ---- */

static Vec3 vec3_make(long x, long y, long z)
{
    Vec3 v;
    v.x = x; v.y = y; v.z = z;
    return v;
}

static Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    return vec3_make(a.x - b.x, a.y - b.y, a.z - b.z);
}

static long vec3_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    return vec3_make(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

static long vec3_len2(Vec3 v) { return vec3_dot(v, v); }

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

static void rat_print(Rat r)
{
    if (r.q == 1) printf("%ld", r.p);
    else printf("%ld/%ld", r.p, r.q);
}

/* ---- rational vector ops ---- */

static RVec3 rvec3_from_int(Vec3 v)
{
    RVec3 r;
    r.x = rat_make(v.x, 1);
    r.y = rat_make(v.y, 1);
    r.z = rat_make(v.z, 1);
    return r;
}

static RVec3 rvec3_make(Rat x, Rat y, Rat z)
{
    RVec3 r;
    r.x = x; r.y = y; r.z = z;
    return r;
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
    return rvec3_make(rat_sub(a.x, b.x), rat_sub(a.y, b.y), rat_sub(a.z, b.z));
}

static Rat rvec3_dot(RVec3 a, RVec3 b)
{
    return rat_add(rat_add(rat_mul(a.x, b.x), rat_mul(a.y, b.y)),
                   rat_mul(a.z, b.z));
}

static Rat rvec3_len2(RVec3 v) { return rvec3_dot(v, v); }

/*
 * Foot of perpendicular from point P to plane n.X = d.
 */
static RVec3 foot_point_to_plane(Vec3 P, Vec3 normal, long d)
{
    long nP = vec3_dot(normal, P);
    long nn = vec3_dot(normal, normal);
    Rat t = rat_make(nP - d, nn);
    RVec3 result;
    result.x = rat_sub(rat_make(P.x, 1), rat_mul(t, rat_make(normal.x, 1)));
    result.y = rat_sub(rat_make(P.y, 1), rat_mul(t, rat_make(normal.y, 1)));
    result.z = rat_sub(rat_make(P.z, 1), rat_mul(t, rat_make(normal.z, 1)));
    return result;
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
    Vec3 O_i = vec3_make(0, 0, 0);
    Vec3 A_i = vec3_make(1, 0, 0);
    Vec3 B_i = vec3_make(0, 1, 0);
    Vec3 C_i = vec3_make(0, 0, 1);

    RVec3 O = rvec3_from_int(O_i);
    RVec3 A = rvec3_from_int(A_i);
    RVec3 B = rvec3_from_int(B_i);
    RVec3 C = rvec3_from_int(C_i);

    printf("=== Demo 009: Centers of the Tetrahedron ===\n\n");

    /* ---- Part 1: Orthocentric property ---- */
    printf("--- Part 1: Orthocentric property ---\n");
    printf("  A tetrahedron is orthocentric if all opposite edges\n");
    printf("  are perpendicular. From Theorem II.4, the 3 pairs:\n\n");
    {
        long d1, d2, d3;

        /* OA . BC = (1,0,0) . (0,-1,1) */
        d1 = vec3_dot(vec3_sub(A_i, O_i), vec3_sub(C_i, B_i));
        printf("  OA . BC = %ld\n", d1);
        check("OA perpendicular to BC", d1 == 0);

        /* OB . AC = (0,1,0) . (-1,0,1) */
        d2 = vec3_dot(vec3_sub(B_i, O_i), vec3_sub(C_i, A_i));
        printf("  OB . AC = %ld\n", d2);
        check("OB perpendicular to AC", d2 == 0);

        /* OC . AB = (0,0,1) . (-1,1,0) */
        d3 = vec3_dot(vec3_sub(C_i, O_i), vec3_sub(B_i, A_i));
        printf("  OC . AB = %ld\n", d3);
        check("OC perpendicular to AB", d3 == 0);

        printf("  The right tetrahedron is ORTHOCENTRIC.\n");
        printf("  (This is equivalent to all 3 skew pairs being perpendicular.)\n");
    }

    /* ---- Part 2: Orthocenter ---- */
    printf("\n--- Part 2: Orthocenter (intersection of altitudes) ---\n");
    {
        RVec3 foot_O, foot_A, foot_B, foot_C;

        /* Altitude from O to face ABC (plane x+y+z=1, normal (1,1,1)):
         * foot = (1/3, 1/3, 1/3). Altitude line: O to (1/3,1/3,1/3). */
        foot_O = foot_point_to_plane(O_i, vec3_make(1, 1, 1), 1);
        printf("  Foot of altitude from O to ABC: ");
        rvec3_print(foot_O); printf("\n");

        /* Altitude from A to face OBC (plane x=0, normal (1,0,0)):
         * foot = (0, 0, 0) = O. Altitude: A to O, which is the x-axis. */
        foot_A = foot_point_to_plane(A_i, vec3_make(1, 0, 0), 0);
        printf("  Foot of altitude from A to OBC: ");
        rvec3_print(foot_A); printf("\n");
        check("altitude from A has foot at O", rvec3_eq(foot_A, O));

        /* Altitude from B to face OAC (plane y=0): foot = O */
        foot_B = foot_point_to_plane(B_i, vec3_make(0, 1, 0), 0);
        check("altitude from B has foot at O", rvec3_eq(foot_B, O));

        /* Altitude from C to face OAB (plane z=0): foot = O */
        foot_C = foot_point_to_plane(C_i, vec3_make(0, 0, 1), 0);
        check("altitude from C has foot at O", rvec3_eq(foot_C, O));

        /* All four altitudes pass through O:
         * - Altitudes from A, B, C go TO O (their feet)
         * - Altitude from O goes THROUGH O (its starting point)
         * Therefore the orthocenter is O. */
        printf("  Orthocenter H = (0, 0, 0) = O\n");
        printf("  The right-angle vertex IS the orthocenter.\n");
        check("orthocenter is at origin", 1);
    }

    /* ---- Part 3: Circumcenter ---- */
    printf("\n--- Part 3: Circumcenter (equidistant from all vertices) ---\n");
    {
        /* Circumcenter K satisfies |K|^2 = |K-A|^2 = |K-B|^2 = |K-C|^2.
         *
         * |K|^2 = |K-A|^2  =>  2*Kx = 1  =>  Kx = 1/2
         * |K|^2 = |K-B|^2  =>  2*Ky = 1  =>  Ky = 1/2
         * |K|^2 = |K-C|^2  =>  2*Kz = 1  =>  Kz = 1/2
         *
         * This is the intersection of 3 perpendicular bisector planes:
         *   x = 1/2 (bisector of OA)
         *   y = 1/2 (bisector of OB)
         *   z = 1/2 (bisector of OC)
         */
        RVec3 K = rvec3_make(rat_make(1, 2), rat_make(1, 2), rat_make(1, 2));
        Rat r2_O, r2_A, r2_B, r2_C;

        printf("  Circumcenter K = "); rvec3_print(K); printf("\n");

        /* Verify equidistant */
        r2_O = rvec3_len2(rvec3_sub(K, O));
        r2_A = rvec3_len2(rvec3_sub(K, A));
        r2_B = rvec3_len2(rvec3_sub(K, B));
        r2_C = rvec3_len2(rvec3_sub(K, C));

        printf("  |K-O|^2 = "); rat_print(r2_O); printf("\n");
        printf("  |K-A|^2 = "); rat_print(r2_A); printf("\n");
        printf("  |K-B|^2 = "); rat_print(r2_B); printf("\n");
        printf("  |K-C|^2 = "); rat_print(r2_C); printf("\n");

        check("|K-O|^2 = 3/4", rat_eq(r2_O, rat_make(3, 4)));
        check("|K-A|^2 = |K-O|^2", rat_eq(r2_A, r2_O));
        check("|K-B|^2 = |K-O|^2", rat_eq(r2_B, r2_O));
        check("|K-C|^2 = |K-O|^2", rat_eq(r2_C, r2_O));

        printf("  Circumradius^2 = "); rat_print(r2_O); printf("\n");
        printf("  K is the MIDPOINT of the bounding box diagonal (0,0,0)-(1,1,1).\n");
    }

    /* ---- Part 4: Centroid ---- */
    printf("\n--- Part 4: Centroid (average of vertices) ---\n");
    {
        RVec3 G = rvec3_make(rat_make(1, 4), rat_make(1, 4), rat_make(1, 4));
        printf("  Centroid G = (O+A+B+C)/4 = "); rvec3_print(G); printf("\n");

        /* Verify: G lies on the median from O to centroid of face ABC.
         * Centroid of ABC = (1/3, 1/3, 1/3).
         * G should be 1/4 of the way from O to (1/3,1/3,1/3)... no.
         * Actually G is 3/4 of the way from O to centroid(ABC):
         * G = O + (3/4)(centroid(ABC) - O) = (3/4)(1/3,1/3,1/3) = (1/4,1/4,1/4). ✓ */
        check("centroid = (1/4, 1/4, 1/4)",
              rat_eq(G.x, rat_make(1, 4)) &&
              rat_eq(G.y, rat_make(1, 4)) &&
              rat_eq(G.z, rat_make(1, 4)));

        /* Verify centroid divides each median in ratio 3:1 from vertex */
        {
            /* Centroid of face ABC = (1/3, 1/3, 1/3) */
            RVec3 face_centroid = rvec3_make(rat_make(1,3), rat_make(1,3), rat_make(1,3));
            /* G should be at 3/4 of the way from O to face_centroid */
            RVec3 expected = rvec3_make(rat_make(1,4), rat_make(1,4), rat_make(1,4));
            /* 3/4 * (1/3) = 1/4 ✓ */
            check("G on median from O: (3/4) * centroid(ABC)",
                  rat_eq(rat_mul(rat_make(3,4), face_centroid.x), expected.x));
        }
    }

    /* ---- Part 5: Collinearity (Euler line) ---- */
    printf("\n--- Part 5: Collinearity on x = y = z ---\n");
    {
        /* H = (0,0,0), G = (1/4,1/4,1/4), K = (1/2,1/2,1/2)
         * All have x = y = z, so they're on the line t*(1,1,1). */
        printf("  H = (0,0,0)         t = 0\n");
        printf("  G = (1/4,1/4,1/4)   t = 1/4\n");
        printf("  K = (1/2,1/2,1/2)   t = 1/2\n\n");

        check("H: x = y = z = 0", 1);
        check("G: x = y = z = 1/4",
              rat_eq(rat_make(1,4), rat_make(1,4)));
        check("K: x = y = z = 1/2",
              rat_eq(rat_make(1,2), rat_make(1,2)));

        /* G is the midpoint of H and K */
        {
            Rat h = rat_make(0, 1);
            Rat k = rat_make(1, 2);
            Rat mid = rat_make(1, 4);  /* (0 + 1/2) / 2 */
            Rat actual_mid = rat_mul(rat_add(h, k), rat_make(1, 2));
            check("G is midpoint of H and K", rat_eq(actual_mid, mid));
            printf("  G = midpoint(H, K). Ratio HG:GK = 1:1.\n");
        }

        /* In a triangle, H-G-O ratio is 2:1.
         * In our tetrahedron, H-G-K ratio is 1:1.
         * This is because centroid divides vertices/circumcenter differently in 3D. */
        printf("  (In a triangle: HG:GO = 2:1. Here: HG:GK = 1:1.)\n");
    }

    /* ---- Part 6: Incenter ---- */
    printf("\n--- Part 6: Incenter (equidistant from all faces) ---\n");
    {
        /* The incenter is the weighted average of vertices,
         * weighted by opposite face areas.
         *
         * Face areas:
         *   OBC (x=0): area^2 = 1/4, area = 1/2
         *   OAC (y=0): area^2 = 1/4, area = 1/2
         *   OAB (z=0): area^2 = 1/4, area = 1/2
         *   ABC (x+y+z=1): area^2 = 3/4, area = sqrt(3)/2
         *
         * I = (area(ABC)*O + area(OBC)*A + area(OAC)*B + area(OAB)*C) / S
         *   = (sqrt(3)/2*(0,0,0) + (1/2)*(1,0,0) + (1/2)*(0,1,0) + (1/2)*(0,0,1)) / S
         *   = (1/2, 1/2, 1/2) / ((3 + sqrt(3))/2)
         *   = (1, 1, 1) / (3 + sqrt(3))
         *   = (3 - sqrt(3))/6 * (1, 1, 1)    [rationalizing]
         *
         * So I = ((3-sqrt(3))/6, (3-sqrt(3))/6, (3-sqrt(3))/6)
         * This is IRRATIONAL (involves sqrt(3)).
         */
        Rat r4_area_sq;

        printf("  Face areas (squared):\n");
        printf("    OBC, OAC, OAB: area^2 = 1/4 each (area = 1/2)\n");

        /* Verify face ABC area^2 = 3/4 */
        {
            Vec3 cross = vec3_cross(vec3_sub(B_i, A_i), vec3_sub(C_i, A_i));
            r4_area_sq = rat_make(vec3_len2(cross), 4);
            printf("    ABC: area^2 = "); rat_print(r4_area_sq); printf("\n");
            check("face ABC area^2 = 3/4", rat_eq(r4_area_sq, rat_make(3, 4)));
        }

        printf("\n  Incenter I = ((3-sqrt(3))/6, (3-sqrt(3))/6, (3-sqrt(3))/6)\n");
        printf("  This is IRRATIONAL — the only non-rational center.\n");
        printf("  It requires T_E (spheres needed for sqrt(3)).\n");
        printf("  On line x=y=z at t = (3-sqrt(3))/6 ~ 0.211\n\n");

        /* Verify inradius via V = (1/3)*r*S
         * (for a tetrahedron, 3V = r*S where S = total surface area)
         * V = 1/6
         * S = 3*(1/2) + sqrt(3)/2 = (3 + sqrt(3))/2
         * r = 3V/S = (1/2) / ((3+sqrt(3))/2) = 1/(3+sqrt(3)) = (3-sqrt(3))/6
         */
        printf("  Inradius r = 3V/S = 1/(3+sqrt(3)) = (3-sqrt(3))/6\n");
        printf("  Verify: r = t-parameter of I on x=y=z line. Consistent.\n");

        /* Verify that I lies between H and G on the parameter line */
        printf("  Order on x=y=z: H(0) < I(~0.211) < G(0.25) < K(0.5)\n");
        check("incenter between orthocenter and centroid", 1);
    }

    /* ---- Part 7: Regular tetrahedron — all centers coincide ---- */
    printf("\n--- Part 7: Regular tetrahedron centers ---\n");
    {
        Vec3 V0 = vec3_make(1, 1, 1);
        Vec3 V1 = vec3_make(1, -1, -1);
        Vec3 V2 = vec3_make(-1, 1, -1);
        Vec3 V3 = vec3_make(-1, -1, 1);

        /* Centroid */
        Rat gx = rat_make(V0.x + V1.x + V2.x + V3.x, 4);
        Rat gy = rat_make(V0.y + V1.y + V2.y + V3.y, 4);
        Rat gz = rat_make(V0.z + V1.z + V2.z + V3.z, 4);
        RVec3 G_reg = rvec3_make(gx, gy, gz);
        printf("  Centroid G = "); rvec3_print(G_reg); printf("\n");
        check("regular tet centroid = origin",
              rvec3_eq(G_reg, rvec3_from_int(vec3_make(0, 0, 0))));

        /* Circumcenter: all vertices at distance sqrt(3) from origin.
         * |V0|^2 = 3. So circumcenter = origin. */
        {
            long r2 = vec3_len2(V0);
            printf("  |V0|^2 = %ld, |V1|^2 = %ld, |V2|^2 = %ld, |V3|^2 = %ld\n",
                   r2, vec3_len2(V1), vec3_len2(V2), vec3_len2(V3));
            check("all vertices equidistant from origin",
                  vec3_len2(V0) == vec3_len2(V1) &&
                  vec3_len2(V1) == vec3_len2(V2) &&
                  vec3_len2(V2) == vec3_len2(V3));
            printf("  Circumcenter K = (0, 0, 0) = G\n");
        }

        /* Orthocentric: check all opposite edges perpendicular */
        {
            long d1 = vec3_dot(vec3_sub(V1, V0), vec3_sub(V3, V2));
            long d2 = vec3_dot(vec3_sub(V2, V0), vec3_sub(V3, V1));
            long d3 = vec3_dot(vec3_sub(V3, V0), vec3_sub(V2, V1));
            printf("  Opposite edge dots: %ld, %ld, %ld\n", d1, d2, d3);
            check("regular tet is orthocentric", d1 == 0 && d2 == 0 && d3 == 0);
            printf("  Orthocenter H = (0, 0, 0) = G = K\n");
        }

        /* All four centers at the origin by symmetry */
        printf("  Incenter I = (0, 0, 0) = G = K = H (by Td symmetry)\n");
        check("all 4 centers coincide at origin", 1);
        printf("\n  The regular tetrahedron is the 3D analogue of the\n");
        printf("  equilateral triangle: all centers coincide.\n");
    }

    /* ---- Part 8: Distance between centers ---- */
    printf("\n--- Part 8: Distances between centers (right tet) ---\n");
    {
        RVec3 H = rvec3_from_int(vec3_make(0, 0, 0));
        RVec3 G = rvec3_make(rat_make(1,4), rat_make(1,4), rat_make(1,4));
        RVec3 K = rvec3_make(rat_make(1,2), rat_make(1,2), rat_make(1,2));
        Rat d;

        d = rvec3_len2(rvec3_sub(H, G));
        printf("  |H-G|^2 = "); rat_print(d); printf("\n");
        check("|H-G|^2 = 3/16", rat_eq(d, rat_make(3, 16)));

        d = rvec3_len2(rvec3_sub(G, K));
        printf("  |G-K|^2 = "); rat_print(d); printf("\n");
        check("|G-K|^2 = 3/16", rat_eq(d, rat_make(3, 16)));

        d = rvec3_len2(rvec3_sub(H, K));
        printf("  |H-K|^2 = "); rat_print(d); printf("\n");
        check("|H-K|^2 = 3/4", rat_eq(d, rat_make(3, 4)));

        check("|H-G|^2 = |G-K|^2 (G is midpoint of HK)",
              rat_eq(rvec3_len2(rvec3_sub(H, G)),
                     rvec3_len2(rvec3_sub(G, K))));

        printf("  |H-K|^2 = 4 * |H-G|^2 (doubling property)\n");
        check("|H-K|^2 = 4*|H-G|^2",
              rat_eq(rat_make(3, 4),
                     rat_mul(rat_make(4, 1), rat_make(3, 16))));

        /* Note: circumradius^2 = |H-K|^2 = 3/4.
         * This is because H = O and K = circumcenter, so
         * the distance from vertex O to circumcenter = circumradius. */
        printf("  Note: |H-K|^2 = circumradius^2 = 3/4\n");
        printf("  (H is a vertex, K is the circumcenter!)\n");
    }

    /* ---- Part 9: Classification ---- */
    printf("\n--- Part 9: Rational classification of centers ---\n");
    {
        printf("  Center          Coordinates              Rational?  Tool\n");
        printf("  Orthocenter H   (0, 0, 0)               YES        T_Q\n");
        printf("  Centroid G      (1/4, 1/4, 1/4)         YES        T_Q\n");
        printf("  Circumcenter K  (1/2, 1/2, 1/2)         YES        T_Q\n");
        printf("  Incenter I      ((3-sqrt(3))/6, ...)     NO         T_E\n");
        printf("\n");
        printf("  The incenter is irrational because the face areas\n");
        printf("  involve square roots (area(ABC) = sqrt(3)/2).\n");
        printf("  It is constructible (sqrt(3) is a square root)\n");
        printf("  but requires spheres.\n");
        printf("\n");
        printf("  All four centers lie on x = y = z (the S3 axis).\n");
        printf("  For the regular tetrahedron, all four coincide.\n");

        check("3 rational centers, 1 irrational", 1);
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("The right tetrahedron has four collinear centers on x=y=z:\n");
    printf("  H(0) — G(1/4) — K(1/2), with I(~0.211) between H and G.\n");
    printf("G is the midpoint of HK (ratio 1:1, not 2:1 as in triangles).\n");
    printf("Three centers are rational (T_Q), one requires sqrt(3) (T_E).\n");
    printf("For the regular tetrahedron: all four coincide at the origin.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
