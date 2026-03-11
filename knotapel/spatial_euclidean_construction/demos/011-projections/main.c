/*
 * Demo 011: Projections and Shadows
 *
 * Orthogonal projection of the right tetrahedron {O, A, B, C} onto
 * planes and lines, studied with exact rational arithmetic.
 *
 * Key results:
 *   - Coordinate plane shadows: each collapses one vertex onto O,
 *     yielding the opposite right-angle face
 *   - Face-plane shadow: O projects to centroid (1/3, 1/3, 1/3);
 *     A, B, C are already on the plane; triangle ABC is equilateral
 *   - S3-axis shadow: A, B, C collapse to a single point; the
 *     tetrahedron projects to a line segment of length^2 = 1/3
 *   - De Gua's theorem: area^2(ABC) = sum of area^2 of the three
 *     right-angle faces (the 3D Pythagorean theorem)
 *   - Complementary projections: Proj_line + Proj_plane = Identity
 *   - All projections are idempotent: Proj(Proj(P)) = Proj(P)
 *
 * Key constructions used:
 *   - Perpendicular feet (Book V) = orthogonal projection
 *   - Cross product (Book VIII) for area verification
 *   - S3 symmetry axis (Book VI) as projection target
 *   - Dihedral angles (Book V) govern area scaling
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long x, y, z; } Vec3;
typedef struct { long p, q; } Rat;
typedef struct { Rat x, y, z; } RVec3;

/* ---- integer vector ops (for cross products) ---- */

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

static Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    return vec3_make(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

static long vec3_len2(Vec3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
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

static RVec3 rvec3_add(RVec3 a, RVec3 b)
{
    return rvec3_make(rat_add(a.x, b.x), rat_add(a.y, b.y), rat_add(a.z, b.z));
}

static RVec3 rvec3_sub(RVec3 a, RVec3 b)
{
    return rvec3_make(rat_sub(a.x, b.x), rat_sub(a.y, b.y), rat_sub(a.z, b.z));
}

static RVec3 rvec3_scale(RVec3 v, Rat k)
{
    return rvec3_make(rat_mul(v.x, k), rat_mul(v.y, k), rat_mul(v.z, k));
}

static Rat rvec3_dot(RVec3 a, RVec3 b)
{
    return rat_add(rat_add(rat_mul(a.x, b.x), rat_mul(a.y, b.y)),
                   rat_mul(a.z, b.z));
}

static Rat rvec3_len2(RVec3 v) { return rvec3_dot(v, v); }

/* ---- projection functions ---- */

/*
 * Orthogonal projection of P onto the plane n.X = d.
 * This is the foot of the perpendicular from P to the plane (Prop V.1).
 * Proj(P) = P - ((n.P - d) / (n.n)) * n
 */
static RVec3 proj_plane(RVec3 p, RVec3 normal, Rat d)
{
    Rat nP = rvec3_dot(normal, p);
    Rat nn = rvec3_dot(normal, normal);
    Rat t = rat_div(rat_sub(nP, d), nn);
    return rvec3_sub(p, rvec3_scale(normal, t));
}

/*
 * Orthogonal projection of P onto the line through the origin
 * with direction d.
 * Proj(P) = ((P.d) / (d.d)) * d
 */
static RVec3 proj_line(RVec3 p, RVec3 direction)
{
    Rat pd = rvec3_dot(p, direction);
    Rat dd = rvec3_dot(direction, direction);
    Rat t = rat_div(pd, dd);
    return rvec3_scale(direction, t);
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
    RVec3 O, A, B, C;
    RVec3 n111;
    Rat zero, one, third;

    O = rvec3_from_ints(0, 0, 0);
    A = rvec3_from_ints(1, 0, 0);
    B = rvec3_from_ints(0, 1, 0);
    C = rvec3_from_ints(0, 0, 1);

    n111 = rvec3_from_ints(1, 1, 1);
    zero = rat_make(0, 1);
    one = rat_make(1, 1);
    third = rat_make(1, 3);

    printf("=== Demo 011: Projections and Shadows ===\n\n");

    /* ---- Part 1: Coordinate plane shadows ---- */
    printf("--- Part 1: Coordinate plane shadows ---\n");
    {
        RVec3 nz = rvec3_from_ints(0, 0, 1);
        RVec3 ny = rvec3_from_ints(0, 1, 0);
        RVec3 nx = rvec3_from_ints(1, 0, 0);

        /* Project onto z=0: O stays, C collapses to O */
        {
            RVec3 pO = proj_plane(O, nz, zero);
            RVec3 pA = proj_plane(A, nz, zero);
            RVec3 pB = proj_plane(B, nz, zero);
            RVec3 pC = proj_plane(C, nz, zero);
            Rat d_OA, d_OB, d_AB;

            printf("  Projection onto z = 0:\n");
            printf("    O -> "); rvec3_print(pO);
            printf(",  C -> "); rvec3_print(pC); printf("\n");
            printf("    A -> "); rvec3_print(pA);
            printf(",  B -> "); rvec3_print(pB); printf("\n");

            check("z=0: O and C collapse to same point",
                  rvec3_eq(pO, pC));

            d_OA = rvec3_len2(rvec3_sub(pO, pA));
            d_OB = rvec3_len2(rvec3_sub(pO, pB));
            d_AB = rvec3_len2(rvec3_sub(pA, pB));
            printf("    Shadow edges: d^2(O,A)=");
            rat_print(d_OA);
            printf(", d^2(O,B)=");
            rat_print(d_OB);
            printf(", d^2(A,B)=");
            rat_print(d_AB);
            printf("\n");
            check("shadow is right triangle: edges 1, 1, 2",
                  rat_eq(d_OA, one) && rat_eq(d_OB, one) &&
                  rat_eq(d_AB, rat_make(2, 1)));
        }

        /* Project onto y=0: O and B collapse */
        {
            RVec3 pO = proj_plane(O, ny, zero);
            RVec3 pB = proj_plane(B, ny, zero);
            check("y=0: O and B collapse to same point",
                  rvec3_eq(pO, pB));
        }

        /* Project onto x=0: O and A collapse */
        {
            RVec3 pO = proj_plane(O, nx, zero);
            RVec3 pA = proj_plane(A, nx, zero);
            check("x=0: O and A collapse to same point",
                  rvec3_eq(pO, pA));
        }

        printf("  Each coordinate plane shadow = the opposite right-angle face.\n");
    }

    /* ---- Part 2: Face-plane projection ---- */
    printf("\n--- Part 2: Projection onto face plane x+y+z=1 ---\n");
    {
        RVec3 pO, pA, pB, pC;
        RVec3 centroid_ABC;
        Rat dOA, dOB, dOC, dAB, dAC, dBC;
        Rat sum;

        pO = proj_plane(O, n111, one);
        pA = proj_plane(A, n111, one);
        pB = proj_plane(B, n111, one);
        pC = proj_plane(C, n111, one);

        printf("  O -> "); rvec3_print(pO); printf("\n");
        printf("  A -> "); rvec3_print(pA); printf("\n");
        printf("  B -> "); rvec3_print(pB); printf("\n");
        printf("  C -> "); rvec3_print(pC); printf("\n");

        centroid_ABC = rvec3_make(third, third, third);
        check("O projects to centroid of ABC (1/3,1/3,1/3)",
              rvec3_eq(pO, centroid_ABC));

        check("A, B, C stay fixed (already on plane)",
              rvec3_eq(pA, A) && rvec3_eq(pB, B) && rvec3_eq(pC, C));

        /* Verify Proj(O) is on the plane */
        sum = rat_add(rat_add(pO.x, pO.y), pO.z);
        check("Proj(O) lies on plane: x+y+z = 1",
              rat_eq(sum, one));

        /* Distances from O' to A, B, C */
        dOA = rvec3_len2(rvec3_sub(pO, A));
        dOB = rvec3_len2(rvec3_sub(pO, B));
        dOC = rvec3_len2(rvec3_sub(pO, C));
        printf("  d^2(O',A) = "); rat_print(dOA);
        printf(",  d^2(O',B) = "); rat_print(dOB);
        printf(",  d^2(O',C) = "); rat_print(dOC); printf("\n");
        check("O' equidistant from A,B,C: all d^2 = 2/3",
              rat_eq(dOA, rat_make(2, 3)) &&
              rat_eq(dOB, rat_make(2, 3)) &&
              rat_eq(dOC, rat_make(2, 3)));

        /* Triangle ABC edges */
        dAB = rvec3_len2(rvec3_sub(A, B));
        dAC = rvec3_len2(rvec3_sub(A, C));
        dBC = rvec3_len2(rvec3_sub(B, C));
        check("ABC is equilateral: all edges d^2 = 2",
              rat_eq(dAB, rat_make(2, 1)) &&
              rat_eq(dAC, rat_make(2, 1)) &&
              rat_eq(dBC, rat_make(2, 1)));

        printf("  The shadow is an equilateral triangle with O'\n");
        printf("  at its centroid. The right angle at O is hidden;\n");
        printf("  the S3 symmetry (Book VI) becomes manifest.\n");
    }

    /* ---- Part 3: Projection onto the S3 axis ---- */
    printf("\n--- Part 3: Projection onto line x=y=z ---\n");
    {
        RVec3 pO, pA, pB, pC;
        RVec3 expected;
        Rat seg_len2;

        pO = proj_line(O, n111);
        pA = proj_line(A, n111);
        pB = proj_line(B, n111);
        pC = proj_line(C, n111);

        printf("  O -> "); rvec3_print(pO); printf("\n");
        printf("  A -> "); rvec3_print(pA); printf("\n");
        printf("  B -> "); rvec3_print(pB); printf("\n");
        printf("  C -> "); rvec3_print(pC); printf("\n");

        check("O stays at origin", rvec3_eq(pO, O));

        expected = rvec3_make(third, third, third);
        check("A, B, C all collapse to (1/3, 1/3, 1/3)",
              rvec3_eq(pA, expected) &&
              rvec3_eq(pB, expected) &&
              rvec3_eq(pC, expected));

        seg_len2 = rvec3_len2(rvec3_sub(pA, pO));
        printf("  Segment d^2 = "); rat_print(seg_len2); printf("\n");
        check("shadow segment d^2 = 1/3",
              rat_eq(seg_len2, third));

        printf("  The tetrahedron collapses to a line segment.\n");
        printf("  This is the Euler line from Book IX:\n");
        printf("  H(0) and G(1/4) and K(1/2) all lie on this axis.\n");
    }

    /* ---- Part 4: Idempotency ---- */
    printf("\n--- Part 4: Idempotency (Proj^2 = Proj) ---\n");
    {
        RVec3 p1, p2;

        /* Face plane: Proj(Proj(O)) = Proj(O) */
        p1 = proj_plane(O, n111, one);
        p2 = proj_plane(p1, n111, one);
        check("face plane: Proj(Proj(O)) = Proj(O)", rvec3_eq(p1, p2));

        /* Line: Proj(Proj(A)) = Proj(A) */
        p1 = proj_line(A, n111);
        p2 = proj_line(p1, n111);
        check("line: Proj(Proj(A)) = Proj(A)", rvec3_eq(p1, p2));

        /* Coordinate plane: Proj(Proj(C)) = Proj(C) on z=0 */
        {
            RVec3 nz = rvec3_from_ints(0, 0, 1);
            p1 = proj_plane(C, nz, zero);
            p2 = proj_plane(p1, nz, zero);
            check("z=0 plane: Proj(Proj(C)) = Proj(C)", rvec3_eq(p1, p2));
        }

        printf("  Idempotency is the defining property of projection.\n");
        printf("  Once you're on the target, projecting again changes nothing.\n");
    }

    /* ---- Part 5: Complementary projections ---- */
    printf("\n--- Part 5: Complementary projections ---\n");
    {
        /* Proj_line(P, d) + Proj_plane(P, d, 0) = P
         * where line has direction d and plane has normal d through origin */
        RVec3 verts[3];
        const char *names[3];
        int i;

        verts[0] = A; names[0] = "A";
        verts[1] = B; names[1] = "B";
        verts[2] = C; names[2] = "C";

        printf("  Axis: (1,1,1).  Complementary plane: x+y+z=0.\n");
        printf("  Proj_line(P) + Proj_plane(P) should = P:\n\n");

        for (i = 0; i < 3; i++) {
            RVec3 pl = proj_line(verts[i], n111);
            RVec3 pp = proj_plane(verts[i], n111, zero);
            RVec3 sum = rvec3_add(pl, pp);
            char desc[80];

            printf("    %s: line=", names[i]);
            rvec3_print(pl);
            printf("  plane=");
            rvec3_print(pp);
            printf("  sum=");
            rvec3_print(sum);
            printf("\n");

            sprintf(desc, "Proj_line(%s) + Proj_plane(%s) = %s",
                    names[i], names[i], names[i]);
            check(desc, rvec3_eq(sum, verts[i]));
        }

        printf("\n  The identity decomposes into perpendicular projections.\n");
        printf("  Every vector = parallel component + perpendicular component.\n");
    }

    /* ---- Part 6: Projected areas and De Gua's theorem ---- */
    printf("\n--- Part 6: Projected areas and De Gua's theorem ---\n");
    {
        Vec3 Ai, Bi, Ci;
        Vec3 cross_ABC, cross_shadow;
        Rat area2_ABC, area2_shadow, ratio, cos2;

        Ai = vec3_make(1, 0, 0);
        Bi = vec3_make(0, 1, 0);
        Ci = vec3_make(0, 0, 1);

        /* Face ABC area^2 */
        cross_ABC = vec3_cross(vec3_sub(Bi, Ai), vec3_sub(Ci, Ai));
        area2_ABC = rat_make(vec3_len2(cross_ABC), 4);
        printf("  Face ABC: |AB x AC|^2 = %ld, area^2 = ",
               vec3_len2(cross_ABC));
        rat_print(area2_ABC); printf("\n");
        check("face ABC area^2 = 3/4", rat_eq(area2_ABC, rat_make(3, 4)));

        /* Shadow of ABC on z=0: A->(1,0,0), B->(0,1,0), C->(0,0,0) */
        cross_shadow = vec3_cross(
            vec3_sub(Bi, Ai),  /* (0,1,0)-(1,0,0) = (-1,1,0) */
            vec3_sub(vec3_make(0, 0, 0), Ai));  /* (0,0,0)-(1,0,0) = (-1,0,0) */
        area2_shadow = rat_make(vec3_len2(cross_shadow), 4);
        printf("  Shadow on z=0: area^2 = ");
        rat_print(area2_shadow); printf("\n");
        check("shadow area^2 = 1/4", rat_eq(area2_shadow, rat_make(1, 4)));

        /* Area scaling by cos^2(dihedral angle) */
        ratio = rat_div(area2_shadow, area2_ABC);
        cos2 = rat_make(1, 3);
        printf("  Ratio shadow/original = ");
        rat_print(ratio);
        printf(" = cos^2(dihedral) = ");
        rat_print(cos2); printf("\n");
        check("ratio = cos^2(angle between normals) = 1/3",
              rat_eq(ratio, cos2));

        /* De Gua's theorem: the 3D Pythagorean theorem.
         * For a right-angled tetrahedron (right angle at O):
         * area^2(ABC) = area^2(OAB) + area^2(OAC) + area^2(OBC)
         *
         * Each right-angle face has area = 1/2, so area^2 = 1/4.
         * The shadow of ABC on each coordinate plane IS the
         * opposite right-angle face. */
        {
            Rat face_sum = rat_add(rat_add(rat_make(1, 4), rat_make(1, 4)),
                                   rat_make(1, 4));
            printf("\n  De Gua's theorem (3D Pythagorean):\n");
            printf("    area^2(OAB) + area^2(OAC) + area^2(OBC)\n");
            printf("    = 1/4 + 1/4 + 1/4 = ");
            rat_print(face_sum);
            printf(" = area^2(ABC) = ");
            rat_print(area2_ABC);
            printf("\n");
            check("De Gua: sum of right-face areas^2 = hypotenuse area^2",
                  rat_eq(face_sum, area2_ABC));
        }
    }

    /* ---- Part 7: Orthogonal decomposition ---- */
    printf("\n--- Part 7: Orthogonal decomposition ---\n");
    {
        /* Decompose A = (1,0,0) relative to axis (1,1,1) */
        RVec3 v_par, v_perp;
        Rat len2_par, len2_perp, len2_total, dot_perp;

        v_par = proj_line(A, n111);
        v_perp = proj_plane(A, n111, zero);

        printf("  Decomposing A = (1,0,0) relative to axis (1,1,1):\n");
        printf("    v_parallel = "); rvec3_print(v_par); printf("\n");
        printf("    v_perp     = "); rvec3_print(v_perp); printf("\n");

        len2_par = rvec3_len2(v_par);
        len2_perp = rvec3_len2(v_perp);
        len2_total = rvec3_len2(A);

        printf("    |v_par|^2  = "); rat_print(len2_par); printf("\n");
        printf("    |v_perp|^2 = "); rat_print(len2_perp); printf("\n");

        check("|v_parallel|^2 = 1/3", rat_eq(len2_par, third));
        check("|v_perpendicular|^2 = 2/3", rat_eq(len2_perp, rat_make(2, 3)));

        check("Pythagorean: |v_par|^2 + |v_perp|^2 = |A|^2",
              rat_eq(rat_add(len2_par, len2_perp), len2_total));

        dot_perp = rvec3_dot(v_perp, n111);
        check("perpendicularity: v_perp . (1,1,1) = 0",
              rat_eq(dot_perp, zero));

        printf("\n  The decomposition is the Pythagorean theorem\n");
        printf("  for orthogonal complements: |v|^2 = |v_par|^2 + |v_perp|^2.\n");
        printf("  Projection loses the perpendicular component but preserves\n");
        printf("  the parallel component exactly.\n");
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("Projection is the non-invertible cousin of reflection.\n");
    printf("Where reflection preserves distances (isometry), projection\n");
    printf("contracts them. But it preserves rationality: rational points\n");
    printf("project to rational points, and the algebra is clean.\n\n");
    printf("The right tetrahedron's shadows reveal its hidden symmetries:\n");
    printf("  - Coordinate planes: the three right-angle faces\n");
    printf("  - Face plane: an equilateral triangle with O at centroid\n");
    printf("  - S3 axis: a line segment (the Euler line)\n");
    printf("De Gua's theorem (3D Pythagorean): the hypotenuse face area^2\n");
    printf("equals the sum of the three right-angle face areas^2.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
