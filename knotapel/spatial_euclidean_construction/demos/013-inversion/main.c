/*
 * Demo 013: Inversion in a Sphere
 *
 * Inversion in the unit sphere centered at O:
 *
 *   Inv(P) = P / |P|^2
 *
 * Key properties:
 *   - Points on the unit sphere are fixed
 *   - Interior maps to exterior and vice versa
 *   - Double inversion = identity: Inv(Inv(P)) = P
 *   - |P|^2 * |Inv(P)|^2 = 1
 *   - Planes not through O map to spheres through O
 *   - Spheres through O map to planes not through O
 *
 * Key result:
 *   Plane ABC (x+y+z = 1) inverts to the circumsphere
 *   |X - K|^2 = 3/4 where K = (1/2, 1/2, 1/2).
 *   The circumsphere of {O,A,B,C} IS the image of face ABC.
 *
 * Key constructions used:
 *   - Circumcenter/circumsphere (Book IX)
 *   - Face plane (Book VII)
 *   - Distance formula (Book V)
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
    return rvec3_make(rat_sub(a.x, b.x), rat_sub(a.y, b.y), rat_sub(a.z, b.z));
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

/* ---- inversion ---- */

/* Inversion in unit sphere centered at origin: Inv(P) = P / |P|^2 */
static RVec3 invert(RVec3 p)
{
    Rat len2 = rvec3_len2(p);
    return rvec3_make(rat_div(p.x, len2),
                      rat_div(p.y, len2),
                      rat_div(p.z, len2));
}

/* Check if point lies on sphere |X - center|^2 = r2 */
static int on_sphere(RVec3 x, RVec3 center, Rat r2)
{
    RVec3 diff = rvec3_sub(x, center);
    return rat_eq(rvec3_len2(diff), r2);
}

/* Check if point lies on plane n.X = d */
static int on_plane(RVec3 x, RVec3 normal, Rat d)
{
    return rat_eq(rvec3_dot(normal, x), d);
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
    RVec3 A, B, C, G, K;
    Rat zero, one;
    RVec3 n111;

    A = rvec3_from_ints(1, 0, 0);
    B = rvec3_from_ints(0, 1, 0);
    C = rvec3_from_ints(0, 0, 1);
    G = rvec3_make(rat_make(1, 4), rat_make(1, 4), rat_make(1, 4));
    K = rvec3_make(rat_make(1, 2), rat_make(1, 2), rat_make(1, 2));

    zero = rat_make(0, 1);
    one = rat_make(1, 1);
    n111 = rvec3_from_ints(1, 1, 1);

    printf("=== Demo 013: Inversion in a Sphere ===\n\n");
    printf("Inversion in unit sphere at O: Inv(P) = P / |P|^2\n\n");

    /* ---- Part 1: Computing inversions ---- */
    printf("--- Part 1: Computing inversions ---\n");
    {
        RVec3 M_OA, p111;
        RVec3 inv_G, inv_M, inv_K, inv_111;

        M_OA = rvec3_make(rat_make(1, 2), zero, zero);
        p111 = rvec3_from_ints(1, 1, 1);

        inv_G = invert(G);
        printf("  Inv(G = "); rvec3_print(G);
        printf(") = "); rvec3_print(inv_G); printf("\n");
        check("Inv(1/4, 1/4, 1/4) = (4/3, 4/3, 4/3)",
              rvec3_eq(inv_G, rvec3_make(rat_make(4, 3),
                                         rat_make(4, 3),
                                         rat_make(4, 3))));

        inv_M = invert(M_OA);
        printf("  Inv(M_OA = "); rvec3_print(M_OA);
        printf(") = "); rvec3_print(inv_M); printf("\n");
        check("Inv(1/2, 0, 0) = (2, 0, 0)",
              rvec3_eq(inv_M, rvec3_from_ints(2, 0, 0)));

        inv_K = invert(K);
        printf("  Inv(K = "); rvec3_print(K);
        printf(") = "); rvec3_print(inv_K); printf("\n");
        check("Inv(1/2, 1/2, 1/2) = (2/3, 2/3, 2/3)",
              rvec3_eq(inv_K, rvec3_make(rat_make(2, 3),
                                         rat_make(2, 3),
                                         rat_make(2, 3))));

        inv_111 = invert(p111);
        printf("  Inv("); rvec3_print(p111);
        printf(") = "); rvec3_print(inv_111); printf("\n");
        check("Inv(1, 1, 1) = (1/3, 1/3, 1/3) = centroid(ABC)",
              rvec3_eq(inv_111, rvec3_make(rat_make(1, 3),
                                           rat_make(1, 3),
                                           rat_make(1, 3))));
    }

    /* ---- Part 2: Fixed points on unit sphere ---- */
    printf("\n--- Part 2: Fixed points (vertices on unit sphere) ---\n");
    {
        printf("  Points with |P|^2 = 1 are fixed under inversion.\n\n");

        check("Inv(A) = A  (|A|^2 = 1)", rvec3_eq(invert(A), A));
        check("Inv(B) = B  (|B|^2 = 1)", rvec3_eq(invert(B), B));
        check("Inv(C) = C  (|C|^2 = 1)", rvec3_eq(invert(C), C));

        printf("\n  A, B, C lie on the unit sphere: they are vertices\n");
        printf("  of our right tetrahedron AND fixed points of inversion.\n");
        printf("  O is the center of inversion (not in the domain).\n");
    }

    /* ---- Part 3: Interior/exterior exchange ---- */
    printf("\n--- Part 3: Interior/exterior exchange ---\n");
    {
        RVec3 M_OA, inv_G, inv_M;
        Rat len2_G, len2_invG, len2_M, len2_invM;

        M_OA = rvec3_make(rat_make(1, 2), zero, zero);

        printf("  |P|^2 < 1 -> |Inv(P)|^2 > 1 (inside -> outside)\n");
        printf("  |P|^2 > 1 -> |Inv(P)|^2 < 1 (outside -> inside)\n\n");

        inv_G = invert(G);
        len2_G = rvec3_len2(G);
        len2_invG = rvec3_len2(inv_G);
        printf("  |G|^2 = "); rat_print(len2_G);
        printf(" < 1,  |Inv(G)|^2 = "); rat_print(len2_invG);
        printf(" > 1\n");
        check("G inside (3/16), Inv(G) outside (16/3)",
              rat_eq(len2_G, rat_make(3, 16)) &&
              rat_eq(len2_invG, rat_make(16, 3)));

        inv_M = invert(M_OA);
        len2_M = rvec3_len2(M_OA);
        len2_invM = rvec3_len2(inv_M);
        printf("  |M_OA|^2 = "); rat_print(len2_M);
        printf(" < 1,  |Inv(M_OA)|^2 = "); rat_print(len2_invM);
        printf(" > 1\n");
        check("M_OA inside (1/4), Inv(M_OA) outside (4)",
              rat_eq(len2_M, rat_make(1, 4)) &&
              rat_eq(len2_invM, rat_make(4, 1)));
    }

    /* ---- Part 4: Double inversion = identity ---- */
    printf("\n--- Part 4: Double inversion = identity ---\n");
    {
        RVec3 pts[5];
        const char *names[5];
        int i;

        pts[0] = A;  names[0] = "A";
        pts[1] = G;  names[1] = "G";
        pts[2] = K;  names[2] = "K";
        pts[3] = rvec3_from_ints(1, 1, 1);  names[3] = "(1,1,1)";
        pts[4] = rvec3_make(rat_make(1, 2), zero, zero);
        names[4] = "M_OA";

        for (i = 0; i < 5; i++) {
            RVec3 dbl = invert(invert(pts[i]));
            char desc[80];
            sprintf(desc, "Inv(Inv(%s)) = %s", names[i], names[i]);
            check(desc, rvec3_eq(dbl, pts[i]));
        }
    }

    /* ---- Part 5: Product property ---- */
    printf("\n--- Part 5: |P|^2 * |Inv(P)|^2 = 1 ---\n");
    {
        RVec3 pts[5];
        const char *names[5];
        int i;

        pts[0] = A;  names[0] = "A";
        pts[1] = G;  names[1] = "G";
        pts[2] = K;  names[2] = "K";
        pts[3] = rvec3_from_ints(1, 1, 1);  names[3] = "(1,1,1)";
        pts[4] = rvec3_make(rat_make(2, 3), rat_make(1, 5), zero);
        names[4] = "(2/3,1/5,0)";

        for (i = 0; i < 5; i++) {
            RVec3 inv_p = invert(pts[i]);
            Rat prod = rat_mul(rvec3_len2(pts[i]), rvec3_len2(inv_p));
            char desc[80];
            printf("  |%s|^2 * |Inv(%s)|^2 = ", names[i], names[i]);
            rat_print(prod); printf("\n");
            sprintf(desc, "|%s|^2 * |Inv(%s)|^2 = 1", names[i], names[i]);
            check(desc, rat_eq(prod, one));
        }
    }

    /* ---- Part 6: Plane ABC -> circumsphere ---- */
    printf("\n--- Part 6: Plane ABC inverts to circumsphere ---\n");
    {
        RVec3 plane_pts[4];
        RVec3 inv_p;
        Rat circ_r2 = rat_make(3, 4);
        int i;

        printf("  Plane ABC: x + y + z = 1\n");
        printf("  Circumsphere: |X - K|^2 = 3/4, K = (1/2, 1/2, 1/2)\n\n");

        /* Points on plane ABC */
        plane_pts[0] = A;
        plane_pts[1] = B;
        plane_pts[2] = C;
        plane_pts[3] = rvec3_make(rat_make(1, 3), rat_make(1, 3),
                                  rat_make(1, 3));

        for (i = 0; i < 4; i++) {
            int is_plane, is_circ;
            char desc[80];

            inv_p = invert(plane_pts[i]);
            is_plane = on_plane(plane_pts[i], n111, one);
            is_circ = on_sphere(inv_p, K, circ_r2);

            printf("  "); rvec3_print(plane_pts[i]);
            printf(" on plane: %s", is_plane ? "yes" : "no");
            printf(",  Inv = "); rvec3_print(inv_p);
            printf(" on circumsphere: %s\n", is_circ ? "yes" : "no");

            sprintf(desc, "plane pt %d -> circumsphere", i);
            check(desc, is_plane && is_circ);
        }

        printf("\n  The face plane inverts to the circumsphere!\n");
        printf("  x+y+z=1 becomes x^2+y^2+z^2 = x+y+z,\n");
        printf("  i.e. |X - (1/2,1/2,1/2)|^2 = 3/4.\n");
    }

    /* ---- Part 7: Circumsphere -> plane ABC ---- */
    printf("\n--- Part 7: Circumsphere inverts to plane ABC ---\n");
    {
        RVec3 sph_pts[4];
        RVec3 inv_p;
        Rat circ_r2 = rat_make(3, 4);
        int i;

        printf("  Points on circumsphere (excl. O) -> plane ABC.\n\n");

        /* A, B, C are on BOTH unit sphere and circumsphere */
        sph_pts[0] = A;
        sph_pts[1] = B;
        sph_pts[2] = C;
        /* (1,1,0): |(1,1,0)|^2 = 2 = 1+1+0, on circumsphere */
        sph_pts[3] = rvec3_from_ints(1, 1, 0);

        for (i = 0; i < 4; i++) {
            int is_circ, is_plane;
            char desc[80];

            inv_p = invert(sph_pts[i]);
            is_circ = on_sphere(sph_pts[i], K, circ_r2);
            is_plane = on_plane(inv_p, n111, one);

            printf("  "); rvec3_print(sph_pts[i]);
            printf(" on circumsphere: %s", is_circ ? "yes" : "no");
            printf(",  Inv = "); rvec3_print(inv_p);
            printf(" on plane: %s\n", is_plane ? "yes" : "no");

            sprintf(desc, "circumsphere pt %d -> plane ABC", i);
            check(desc, is_circ && is_plane);
        }

        printf("\n  The correspondence is exact: plane <-> sphere.\n");
        printf("  O is on the circumsphere but maps to infinity,\n");
        printf("  reflecting that the plane extends to infinity.\n");
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("Inversion in the unit sphere Inv(P) = P/|P|^2:\n");
    printf("  - Fixes the unit sphere pointwise (A, B, C fixed)\n");
    printf("  - Exchanges interior and exterior\n");
    printf("  - Is its own inverse: Inv(Inv(P)) = P\n");
    printf("  - Satisfies |P|^2 * |Inv(P)|^2 = 1\n");
    printf("  - Maps plane ABC to the circumsphere of {O,A,B,C}\n\n");
    printf("The central discovery: the face plane x+y+z=1 inverts to\n");
    printf("the circumsphere |X-K|^2 = 3/4. Face geometry and sphere\n");
    printf("geometry are the SAME geometry, seen through inversion.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
