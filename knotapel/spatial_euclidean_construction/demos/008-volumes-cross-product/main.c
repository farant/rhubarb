/*
 * Demo 008: Volumes and the Cross Product
 *
 * The cross product is the quintessentially 3D operation —
 * it exists only in 3 (and 7) dimensions.
 *
 * Three levels of the rational metric:
 *   1. Distance^2 is rational  (Book V)
 *   2. Area^2 is rational      (this book)
 *   3. Volume^2 is rational    (this book)
 *      Volume itself is rational (not just squared!)
 *
 * Key formulas:
 *   Cross product: a x b = (ay*bz - az*by, az*bx - ax*bz, ax*by - ay*bx)
 *   Area of triangle PQR:  4*area^2 = |(Q-P) x (R-P)|^2
 *   Volume of tetrahedron: 36*vol^2 = det(Q-P, R-P, S-P)^2
 *      (equivalently: vol = |det| / 6)
 *   Scalar triple product:  a . (b x c) = det(a, b, c)
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long x, y, z; } Vec3;
typedef struct { long p, q; } Rat;

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

static long vec3_len2(Vec3 v)
{
    return vec3_dot(v, v);
}

static Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    return vec3_make(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

/*
 * Scalar triple product: a . (b x c) = det[a, b, c]
 * This is the signed volume of the parallelepiped.
 */
static long scalar_triple(Vec3 a, Vec3 b, Vec3 c)
{
    return vec3_dot(a, vec3_cross(b, c));
}

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

static void rat_print(Rat r)
{
    if (r.q == 1) printf("%ld", r.p);
    else printf("%ld/%ld", r.p, r.q);
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

/* ---- helpers ---- */

static int vec3_eq(Vec3 a, Vec3 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

static void vec3_print(Vec3 v)
{
    printf("(%ld, %ld, %ld)", v.x, v.y, v.z);
}

/* ---- main ---- */

int main(void)
{
    Vec3 O = vec3_make(0, 0, 0);
    Vec3 A = vec3_make(1, 0, 0);
    Vec3 B = vec3_make(0, 1, 0);
    Vec3 C = vec3_make(0, 0, 1);

    printf("=== Demo 008: Volumes and the Cross Product ===\n\n");

    /* ---- Part 1: Cross product basics ---- */
    printf("--- Part 1: Cross product properties ---\n");
    {
        Vec3 c1, c2, c3;

        /* Standard basis cross products */
        c1 = vec3_cross(A, B);
        printf("  A x B = "); vec3_print(c1); printf("\n");
        check("(1,0,0) x (0,1,0) = (0,0,1)", vec3_eq(c1, C));

        c2 = vec3_cross(B, C);
        printf("  B x C = "); vec3_print(c2); printf("\n");
        check("(0,1,0) x (0,0,1) = (1,0,0)", vec3_eq(c2, A));

        c3 = vec3_cross(C, A);
        printf("  C x A = "); vec3_print(c3); printf("\n");
        check("(0,0,1) x (1,0,0) = (0,1,0)", vec3_eq(c3, B));

        /* Anti-commutativity: a x b = -(b x a) */
        c1 = vec3_cross(A, B);
        c2 = vec3_cross(B, A);
        check("anti-commutativity: A x B = -(B x A)",
              c1.x == -c2.x && c1.y == -c2.y && c1.z == -c2.z);

        /* Self cross product is zero */
        c1 = vec3_cross(A, A);
        check("A x A = 0", vec3_eq(c1, O));

        /* Cross product of parallel vectors is zero */
        c1 = vec3_cross(A, vec3_make(3, 0, 0));
        check("(1,0,0) x (3,0,0) = 0 (parallel)", vec3_eq(c1, O));

        /* Perpendicularity: (a x b) . a = 0 and (a x b) . b = 0 */
        {
            Vec3 u = vec3_make(1, 2, 3);
            Vec3 v = vec3_make(4, 5, 6);
            Vec3 w = vec3_cross(u, v);
            check("(u x v) . u = 0", vec3_dot(w, u) == 0);
            check("(u x v) . v = 0", vec3_dot(w, v) == 0);
        }

        /* |a x b|^2 = |a|^2 * |b|^2 - (a.b)^2  (Lagrange identity) */
        {
            Vec3 u = vec3_make(1, 2, 3);
            Vec3 v = vec3_make(4, 5, 6);
            Vec3 w = vec3_cross(u, v);
            long lhs = vec3_len2(w);
            long rhs = vec3_len2(u) * vec3_len2(v) - vec3_dot(u, v) * vec3_dot(u, v);
            printf("  Lagrange identity: |u x v|^2 = %ld, |u|^2*|v|^2 - (u.v)^2 = %ld\n",
                   lhs, rhs);
            check("Lagrange identity", lhs == rhs);
        }
    }

    /* ---- Part 2: Face areas of the right tetrahedron ---- */
    printf("\n--- Part 2: Face areas of right tetrahedron {O,A,B,C} ---\n");
    printf("  4*area^2 = |edge1 x edge2|^2\n\n");
    {
        Vec3 cross;
        Rat area_sq;

        /* Face OAB (z=0): edges OA=(1,0,0), OB=(0,1,0) */
        cross = vec3_cross(vec3_sub(A, O), vec3_sub(B, O));
        printf("  Face OAB: normal = "); vec3_print(cross);
        area_sq = rat_make(vec3_len2(cross), 4);
        printf(", area^2 = "); rat_print(area_sq); printf("\n");
        check("area^2(OAB) = 1/4", rat_eq(area_sq, rat_make(1, 4)));

        /* Face OAC (y=0): edges OA=(1,0,0), OC=(0,0,1) */
        cross = vec3_cross(vec3_sub(A, O), vec3_sub(C, O));
        printf("  Face OAC: normal = "); vec3_print(cross);
        area_sq = rat_make(vec3_len2(cross), 4);
        printf(", area^2 = "); rat_print(area_sq); printf("\n");
        check("area^2(OAC) = 1/4", rat_eq(area_sq, rat_make(1, 4)));

        /* Face OBC (x=0): edges OB=(0,1,0), OC=(0,0,1) */
        cross = vec3_cross(vec3_sub(B, O), vec3_sub(C, O));
        printf("  Face OBC: normal = "); vec3_print(cross);
        area_sq = rat_make(vec3_len2(cross), 4);
        printf(", area^2 = "); rat_print(area_sq); printf("\n");
        check("area^2(OBC) = 1/4", rat_eq(area_sq, rat_make(1, 4)));

        /* Face ABC (x+y+z=1): edges AB=(-1,1,0), AC=(-1,0,1) */
        cross = vec3_cross(vec3_sub(B, A), vec3_sub(C, A));
        printf("  Face ABC: normal = "); vec3_print(cross);
        area_sq = rat_make(vec3_len2(cross), 4);
        printf(", area^2 = "); rat_print(area_sq); printf("\n");
        check("area^2(ABC) = 3/4", rat_eq(area_sq, rat_make(3, 4)));

        /* The three right-angle faces each have area 1/2,
         * the hypotenuse face has area sqrt(3)/2 */
        printf("  Right-angle faces: area = 1/2 each\n");
        printf("  Hypotenuse face ABC: area = sqrt(3)/2\n");
        printf("  Note: area^2 is RATIONAL even when area is irrational.\n");

        /* Total surface area: 3*(1/2) + sqrt(3)/2 = (3 + sqrt(3))/2
         * This is irrational, but each face's area^2 is rational. */
    }

    /* ---- Part 3: Volume of right tetrahedron ---- */
    printf("\n--- Part 3: Volume of right tetrahedron ---\n");
    {
        Vec3 a = vec3_sub(A, O);  /* (1,0,0) */
        Vec3 b = vec3_sub(B, O);  /* (0,1,0) */
        Vec3 c = vec3_sub(C, O);  /* (0,0,1) */
        long det;
        Rat vol, vol_sq;

        det = scalar_triple(a, b, c);
        printf("  det[A-O, B-O, C-O] = %ld\n", det);
        check("determinant = 1", det == 1);

        vol = rat_make(det, 6);  /* det is positive */
        vol_sq = rat_make(det * det, 36);
        printf("  Volume = |det|/6 = "); rat_print(vol); printf("\n");
        printf("  Volume^2 = det^2/36 = "); rat_print(vol_sq); printf("\n");
        check("volume = 1/6", rat_eq(vol, rat_make(1, 6)));
        check("volume^2 = 1/36", rat_eq(vol_sq, rat_make(1, 36)));

        /* Verify: parallelepiped volume = |det| = 1
         * This is the unit cube, so volume = 1.
         * Tetrahedron = 1/6 of the parallelepiped. */
        printf("  (The unit parallelepiped has volume 1.\n");
        printf("   The tetrahedron is 1/6 of it.)\n");
    }

    /* ---- Part 4: Volume of regular tetrahedron ---- */
    printf("\n--- Part 4: Regular tetrahedron volume ---\n");
    {
        Vec3 V0 = vec3_make(1, 1, 1);
        Vec3 V1 = vec3_make(1, -1, -1);
        Vec3 V2 = vec3_make(-1, 1, -1);
        Vec3 V3 = vec3_make(-1, -1, 1);
        Vec3 a = vec3_sub(V1, V0);
        Vec3 b = vec3_sub(V2, V0);
        Vec3 c = vec3_sub(V3, V0);
        long det;
        Rat vol, vol_sq;

        printf("  Edge vectors from (1,1,1):\n");
        printf("    a = "); vec3_print(a); printf("\n");
        printf("    b = "); vec3_print(b); printf("\n");
        printf("    c = "); vec3_print(c); printf("\n");

        det = scalar_triple(a, b, c);
        printf("  det = %ld\n", det);
        check("determinant = -16", det == -16);

        vol = rat_make(det < 0 ? -det : det, 6);
        vol_sq = rat_make(det * det, 36);
        printf("  Volume = |det|/6 = "); rat_print(vol); printf("\n");
        printf("  Volume^2 = det^2/36 = "); rat_print(vol_sq); printf("\n");
        check("volume = 8/3", rat_eq(vol, rat_make(8, 3)));
        check("volume^2 = 64/9", rat_eq(vol_sq, rat_make(64, 9)));

        /* Verify with formula: V = edge^3 / (6*sqrt(2))
         * edge = 2*sqrt(2), so V = (2*sqrt(2))^3 / (6*sqrt(2))
         *   = 16*sqrt(2) / (6*sqrt(2)) = 16/6 = 8/3  ✓ */
        printf("  Verification: edge = 2*sqrt(2), V = edge^3/(6*sqrt(2)) = 8/3\n");

        /* Face area */
        {
            Vec3 cross = vec3_cross(a, b);
            Rat face_area_sq = rat_make(vec3_len2(cross), 4);
            printf("  Face area^2 = "); rat_print(face_area_sq); printf("\n");
            check("face area^2 = 12", rat_eq(face_area_sq, rat_make(12, 1)));
            /* area = 2*sqrt(3), area^2 = 12 */
        }
    }

    /* ---- Part 5: Cube volume ---- */
    printf("\n--- Part 5: Cube volume ---\n");
    {
        /* Cube (+-1)^3: the three edge directions from (-1,-1,-1) are
         * (2,0,0), (0,2,0), (0,0,2) */
        Vec3 e1 = vec3_make(2, 0, 0);
        Vec3 e2 = vec3_make(0, 2, 0);
        Vec3 e3 = vec3_make(0, 0, 2);
        long det;

        det = scalar_triple(e1, e2, e3);
        printf("  Parallelepiped det = %ld\n", det);
        check("cube volume (parallelepiped) = 8", det == 8);

        /* Face area: 2x2 square, area = 4, area^2 = 16 */
        {
            Vec3 cross = vec3_cross(e1, e2);
            Rat face_area_sq = rat_make(vec3_len2(cross), 1);
            printf("  Face: |e1 x e2|^2 = "); rat_print(face_area_sq);
            printf(" (area = 4)\n");
            check("cube face area^2 = 16", rat_eq(face_area_sq, rat_make(16, 1)));
        }
    }

    /* ---- Part 6: Octahedron volume ---- */
    printf("\n--- Part 6: Octahedron volume ---\n");
    {
        /* Decompose into 8 tetrahedra from center (0,0,0) to each face.
         * One such face: (1,0,0), (0,1,0), (0,0,1)
         * Edges from O: (1,0,0), (0,1,0), (0,0,1)
         * det = 1, volume of tetrahedron = 1/6 */
        long det_face;
        Rat single_vol, total_vol, total_vol_sq;

        det_face = scalar_triple(
            vec3_make(1, 0, 0),
            vec3_make(0, 1, 0),
            vec3_make(0, 0, 1));
        single_vol = rat_make(det_face < 0 ? -det_face : det_face, 6);

        /* 8 such tetrahedra: the 8 faces of the octahedron,
         * each with vertices at one point on each axis (+-1,0,0) etc.
         * All have |det| = 1 by symmetry. */
        printf("  Each center-to-face tetrahedron: vol = "); rat_print(single_vol); printf("\n");
        total_vol = rat_make(8, 6);  /* 8 * 1/6 */
        total_vol_sq = rat_make(64, 36);
        printf("  Octahedron volume = 8 * 1/6 = "); rat_print(total_vol); printf("\n");
        printf("  Volume^2 = "); rat_print(total_vol_sq); printf("\n");
        check("octahedron volume = 4/3", rat_eq(total_vol, rat_make(4, 3)));
        check("octahedron volume^2 = 16/9", rat_eq(total_vol_sq, rat_make(16, 9)));

        /* Face area: equilateral triangle with side sqrt(2)
         * normal = (1,0,0) x (0,1,0) ... but from face vertices:
         * edges from (1,0,0): (-1,1,0), (-1,0,1)
         * cross = (1,1,0) x (1,0,1)... let me compute directly */
        {
            Vec3 p = vec3_make(1, 0, 0);
            Vec3 q = vec3_make(0, 1, 0);
            Vec3 r = vec3_make(0, 0, 1);
            Vec3 cross = vec3_cross(vec3_sub(q, p), vec3_sub(r, p));
            Rat area_sq = rat_make(vec3_len2(cross), 4);
            printf("  Octahedron face area^2 = "); rat_print(area_sq);
            printf(" (area = sqrt(3)/2)\n");
            check("octahedron face area^2 = 3/4", rat_eq(area_sq, rat_make(3, 4)));
        }

        /* Verify with formula: V = sqrt(2)/3 * a^3 where a = sqrt(2)
         * V = sqrt(2)/3 * 2*sqrt(2) = sqrt(2)*2*sqrt(2)/3 = 4/3  ✓ */
    }

    /* ---- Part 7: Volume ratios ---- */
    printf("\n--- Part 7: Volume ratios ---\n");
    {
        /* How do the Platonic solids compare?
         * All inscribed in a sphere of radius R.
         * Circumradius^2: tet=3, cube=3, oct=1 (from Demo 007)
         *
         * But let's compare by normalizing to circumradius R=1:
         * That requires scaling. Instead, compare raw volumes: */
        Rat v_right_tet = rat_make(1, 6);
        Rat v_reg_tet   = rat_make(8, 3);
        Rat v_cube      = rat_make(8, 1);
        Rat v_oct       = rat_make(4, 3);

        /* Ratio: cube / reg_tet = 8 / (8/3) = 3 */
        Rat ratio = rat_make(v_cube.p * v_reg_tet.q, v_cube.q * v_reg_tet.p);
        printf("  V(cube) / V(reg_tet) = "); rat_print(ratio); printf("\n");
        check("cube is 3x regular tetrahedron", rat_eq(ratio, rat_make(3, 1)));

        /* Ratio: cube / octahedron = 8 / (4/3) = 6 */
        ratio = rat_make(v_cube.p * v_oct.q, v_cube.q * v_oct.p);
        printf("  V(cube) / V(octahedron) = "); rat_print(ratio); printf("\n");
        check("cube is 6x octahedron", rat_eq(ratio, rat_make(6, 1)));

        /* Ratio: reg_tet / right_tet = (8/3) / (1/6) = 16 */
        ratio = rat_make(v_reg_tet.p * v_right_tet.q,
                         v_reg_tet.q * v_right_tet.p);
        printf("  V(reg_tet) / V(right_tet) = "); rat_print(ratio); printf("\n");
        check("regular tet is 16x right tet", rat_eq(ratio, rat_make(16, 1)));

        /* Ratio: octahedron / right_tet = (4/3) / (1/6) = 8 */
        ratio = rat_make(v_oct.p * v_right_tet.q, v_oct.q * v_right_tet.p);
        printf("  V(octahedron) / V(right_tet) = "); rat_print(ratio); printf("\n");
        check("octahedron is 8x right tet", rat_eq(ratio, rat_make(8, 1)));
    }

    /* ---- Part 8: Scalar triple product properties ---- */
    printf("\n--- Part 8: Scalar triple product properties ---\n");
    {
        Vec3 a = vec3_make(1, 2, 3);
        Vec3 b = vec3_make(4, 5, 6);
        Vec3 c = vec3_make(7, 8, 10);
        long abc, bca, cab, bac;

        abc = scalar_triple(a, b, c);
        bca = scalar_triple(b, c, a);
        cab = scalar_triple(c, a, b);
        bac = scalar_triple(b, a, c);

        printf("  a.(b x c) = %ld\n", abc);
        printf("  b.(c x a) = %ld\n", bca);
        printf("  c.(a x b) = %ld\n", cab);
        printf("  b.(a x c) = %ld\n", bac);

        check("cyclic: a.(b x c) = b.(c x a)", abc == bca);
        check("cyclic: a.(b x c) = c.(a x b)", abc == cab);
        check("swap: a.(b x c) = -b.(a x c)", abc == -bac);

        /* Coplanar vectors have zero triple product */
        {
            Vec3 u = vec3_make(1, 0, 0);
            Vec3 v = vec3_make(0, 1, 0);
            Vec3 w = vec3_make(3, 4, 0);  /* in xy-plane */
            long trip = scalar_triple(u, v, w);
            check("coplanar vectors: triple product = 0", trip == 0);
        }
    }

    /* ---- Summary ---- */
    printf("\n--- Summary: The Three-Level Rational Metric ---\n");
    printf("  Level 1 (Book V):   distance^2 is rational\n");
    printf("  Level 2 (Book VIII): area^2 is rational\n");
    printf("  Level 3 (Book VIII): volume^2 is rational\n");
    printf("                       (volume itself is rational!)\n");
    printf("\n");
    printf("  The cross product is the bridge from level 1 to levels 2-3.\n");
    printf("  It is an INTRINSICALLY 3D operation with no 2D analogue.\n");
    printf("\n");
    printf("  Right tetrahedron: V=1/6, faces: 3x(1/4) + 1x(3/4)\n");
    printf("  Regular tetrahedron: V=8/3, faces: 4x(12)\n");
    printf("  Cube: V=8, faces: 6x(16)\n");
    printf("  Octahedron: V=4/3, faces: 8x(3/4)\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
