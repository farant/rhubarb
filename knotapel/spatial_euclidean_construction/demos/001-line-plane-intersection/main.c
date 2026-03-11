/*
 * Demo 001: Line-Plane Intersection
 * Verifies Proposition II.1 from the Elementa.
 *
 * Tests the formula:
 *   d = Q - P
 *   t = n . (R - P) / (n . d)
 *   X = P + t * d
 *
 * Uses the standard initial configuration {O, A, B, C} and
 * verifies intersections of edges with face planes.
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long x, y, z; } Vec3;

/* rational number p/q in lowest terms */
typedef struct { long p, q; } Rat;

typedef struct { Rat x, y, z; } RVec3;

/* ---- basic integer vector ops ---- */

static Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    Vec3 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

static long vec3_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    Vec3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
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
    r.p = p / g;
    r.q = q / g;
    return r;
}

static void rat_print(Rat r)
{
    if (r.q == 1) {
        printf("%ld", r.p);
    } else {
        printf("%ld/%ld", r.p, r.q);
    }
}

static RVec3 rvec3_make(Rat x, Rat y, Rat z)
{
    RVec3 r;
    r.x = x;
    r.y = y;
    r.z = z;
    return r;
}

static void rvec3_print(RVec3 v)
{
    printf("(");
    rat_print(v.x);
    printf(", ");
    rat_print(v.y);
    printf(", ");
    rat_print(v.z);
    printf(")");
}

/* ---- line-plane intersection ---- */

/*
 * Line through P and Q, plane with normal n through R.
 * Returns 1 if intersection found, 0 if parallel.
 * Result stored in *out.
 *
 * All arithmetic is exact rational: t = n.(R-P) / n.(Q-P)
 * Then X_i = P_i + t * (Q_i - P_i), computed as rationals.
 */
static int line_plane_intersect(
    Vec3 P, Vec3 Q,
    Vec3 n, Vec3 R,
    RVec3 *out)
{
    Vec3 d = vec3_sub(Q, P);
    long denom = vec3_dot(n, d);
    long numer;
    Rat t;
    Rat px, py, pz;

    if (denom == 0) {
        return 0; /* parallel */
    }

    numer = vec3_dot(n, vec3_sub(R, P));
    t = rat_make(numer, denom);

    /* X_i = P_i + t * d_i = (P_i * t.q + t.p * d_i) / t.q */
    px = rat_make(P.x * t.q + t.p * d.x, t.q);
    py = rat_make(P.y * t.q + t.p * d.y, t.q);
    pz = rat_make(P.z * t.q + t.p * d.z, t.q);

    *out = rvec3_make(px, py, pz);
    return 1;
}

/* ---- plane normal from three points ---- */

static Vec3 plane_normal(Vec3 P, Vec3 Q, Vec3 R)
{
    return vec3_cross(vec3_sub(Q, P), vec3_sub(R, P));
}

/* ---- skew test ---- */

static int lines_are_skew(Vec3 P1, Vec3 Q1, Vec3 P2, Vec3 Q2)
{
    Vec3 d1 = vec3_sub(Q1, P1);
    Vec3 d2 = vec3_sub(Q2, P2);
    Vec3 cross = vec3_cross(d1, d2);
    long triple = vec3_dot(vec3_sub(P2, P1), cross);
    return triple != 0;
}

/* ---- test helpers ---- */

static int tests_run = 0;
static int tests_passed = 0;

static int rat_eq(Rat a, Rat b)
{
    return a.p == b.p && a.q == b.q;
}

static int rvec3_eq(RVec3 a, RVec3 b)
{
    return rat_eq(a.x, b.x) && rat_eq(a.y, b.y) && rat_eq(a.z, b.z);
}

static void test_intersection(
    const char *desc,
    Vec3 P, Vec3 Q, Vec3 n, Vec3 R,
    int expect_hit, RVec3 expect_point)
{
    RVec3 result;
    int hit;

    tests_run++;
    hit = line_plane_intersect(P, Q, n, R, &result);

    if (hit != expect_hit) {
        printf("FAIL: %s — expected %s, got %s\n",
               desc,
               expect_hit ? "intersection" : "parallel",
               hit ? "intersection" : "parallel");
        return;
    }

    if (hit && !rvec3_eq(result, expect_point)) {
        printf("FAIL: %s — expected ", desc);
        rvec3_print(expect_point);
        printf(", got ");
        rvec3_print(result);
        printf("\n");
        return;
    }

    tests_passed++;
    printf("PASS: %s", desc);
    if (hit) {
        printf(" = ");
        rvec3_print(result);
    }
    printf("\n");
}

static void test_skew(
    const char *desc,
    Vec3 P1, Vec3 Q1, Vec3 P2, Vec3 Q2,
    int expect_skew)
{
    int result;

    tests_run++;
    result = lines_are_skew(P1, Q1, P2, Q2);

    if (result != expect_skew) {
        printf("FAIL: %s — expected %s, got %s\n",
               desc,
               expect_skew ? "skew" : "coplanar",
               result ? "skew" : "coplanar");
        return;
    }

    tests_passed++;
    printf("PASS: %s — %s\n", desc, result ? "skew" : "coplanar");
}

/* ---- main ---- */

int main(void)
{
    /* standard initial configuration */
    Vec3 O = {0, 0, 0};
    Vec3 A = {1, 0, 0};
    Vec3 B = {0, 1, 0};
    Vec3 C = {0, 0, 1};

    /* face plane normals */
    Vec3 n_OAB = plane_normal(O, A, B);  /* should be (0,0,1) → z=0 */
    Vec3 n_OAC = plane_normal(O, A, C);  /* should be (0,-1,0) → y=0 */
    Vec3 n_OBC = plane_normal(O, B, C);  /* should be (1,0,0) → x=0 */
    Vec3 n_ABC = plane_normal(A, B, C);  /* should be (1,1,1) → x+y+z=1 */

    RVec3 dummy = {{0,1},{0,1},{0,1}};

    Rat r0 = rat_make(0, 1);
    Rat r1 = rat_make(1, 1);

    RVec3 point_A = rvec3_make(r1, r0, r0);
    RVec3 point_B = rvec3_make(r0, r1, r0);
    RVec3 point_C = rvec3_make(r0, r0, r1);


    printf("=== Demo 001: Line-Plane Intersection ===\n\n");

    printf("--- Face plane normals ---\n");
    printf("Pi(O,A,B) normal: (%ld,%ld,%ld)\n", n_OAB.x, n_OAB.y, n_OAB.z);
    printf("Pi(O,A,C) normal: (%ld,%ld,%ld)\n", n_OAC.x, n_OAC.y, n_OAC.z);
    printf("Pi(O,B,C) normal: (%ld,%ld,%ld)\n", n_OBC.x, n_OBC.y, n_OBC.z);
    printf("Pi(A,B,C) normal: (%ld,%ld,%ld)\n", n_ABC.x, n_ABC.y, n_ABC.z);
    printf("\n");

    printf("--- Prop II.1: edge vs opposite face ---\n");
    /* Each edge of the tetrahedron should intersect the opposite face
       at one of the original vertices (confirming the inertness result) */

    /* L(O,A) vs Pi(B,C,?) — but the opposite face to edge OA is face BCA */
    /* Actually opposite face to edge OA contains B and C but also needs
       a third point from {O,A,B,C}\{O,A} = {B,C} — only 2 points!
       The "opposite face" to an edge in a tetrahedron means the face
       not containing either endpoint. Edge OA → face BCA doesn't exist
       since we need 3 points not on the edge. Actually tetrahedron edge
       has opposite edge, not opposite face. Each VERTEX has an opposite face. */

    /* vertex O → opposite face Pi(A,B,C): line from O through face */
    test_intersection(
        "L(O,A) cap Pi(A,B,C): edge hits face at vertex A",
        O, A, n_ABC, A, 1, point_A);

    test_intersection(
        "L(O,B) cap Pi(A,B,C): edge hits face at vertex B",
        O, B, n_ABC, A, 1, point_B);

    test_intersection(
        "L(O,C) cap Pi(A,B,C): edge hits face at vertex C",
        O, C, n_ABC, A, 1, point_C);

    /* vertex A → opposite face Pi(O,B,C) */
    test_intersection(
        "L(A,B) cap Pi(O,B,C): edge hits face at vertex B",
        A, B, n_OBC, O, 1, point_B);

    test_intersection(
        "L(A,C) cap Pi(O,B,C): edge hits face at vertex C",
        A, C, n_OBC, O, 1, point_C);

    /* edge parallel to face test */
    /* L(A,B) is in Pi(O,A,B) so... actually L(A,B) is IN the plane z=0,
       not parallel. Let's test a genuinely parallel case. */
    /* L(B,C) vs Pi(O,A,B): B=(0,1,0) to C=(0,0,1), direction (0,-1,1).
       Normal of Pi(O,A,B) is (0,0,1). Dot = 1, not parallel. */
    /* Actually for our tetrahedron, no edge is parallel to an opposite face.
       Let's manufacture a parallel test: line from (0,0,1) to (1,0,1) vs z=0 plane */
    {
        Vec3 P1 = {0, 0, 1};
        Vec3 Q1 = {1, 0, 1};
        test_intersection(
            "L((0,0,1),(1,0,1)) cap z=0: parallel, no intersection",
            P1, Q1, n_OAB, O, 0, dummy);
    }

    printf("\n--- Prop II.1: non-trivial intersection ---\n");
    /* line from A=(1,0,0) to C=(0,0,1) vs plane y=0 (Pi(O,A,C))
       direction (-1,0,1), normal (0,-1,0), dot = 0. Parallel!
       Actually L(A,C) lies IN the xz-plane. Let's try something better. */

    /* Line from (0,0,0) to (1,1,1) vs plane x+y+z=1 (Pi(A,B,C))
       d = (1,1,1), n = (1,1,1), n.d = 3
       n.(A-O) = 1+0+0 = 1, t = 1/3
       X = (1/3, 1/3, 1/3) — the centroid! */
    {
        Vec3 P = {0, 0, 0};
        Vec3 Q = {1, 1, 1};
        Rat r13 = rat_make(1, 3);
        RVec3 centroid = rvec3_make(r13, r13, r13);
        test_intersection(
            "L(O,(1,1,1)) cap Pi(A,B,C): centroid (1/3,1/3,1/3)",
            P, Q, n_ABC, A, 1, centroid);
    }

    /* Line from (1,0,0) to (0,1,1) vs plane x=0 (Pi(O,B,C))
       d = (-1,1,1), n = (1,0,0), n.d = -1
       n.(O - A) = (1,0,0).(−1,0,0) = −1, t = −1/−1 = 1
       X = (1,0,0) + 1*(-1,1,1) = (0,1,1) */
    {
        Vec3 P = {1, 0, 0};
        Vec3 Q = {0, 1, 1};
        RVec3 expect = rvec3_make(r0, r1, r1);
        test_intersection(
            "L(A,(0,1,1)) cap Pi(O,B,C): (0,1,1)",
            P, Q, n_OBC, O, 1, expect);
    }

    /* A more interesting one: line from B=(0,1,0) toward (1,0,1)
       vs plane x+y+z=1
       d = (1,-1,1), n = (1,1,1), n.d = 1
       n.(A - B) = (1,1,1).(1,-1,0) = 0, t = 0
       X = B = (0,1,0). Hmm, B is on the plane. */

    /* Line from (0,0,0) toward (2,1,0) vs plane x+y+z=1
       d = (2,1,0), n = (1,1,1), n.d = 3
       n.(A - O) = 1, t = 1/3
       X = (2/3, 1/3, 0) */
    {
        Vec3 P = {0, 0, 0};
        Vec3 Q = {2, 1, 0};
        Rat r23 = rat_make(2, 3);
        Rat r13 = rat_make(1, 3);
        RVec3 expect = rvec3_make(r23, r13, r0);
        test_intersection(
            "L(O,(2,1,0)) cap Pi(A,B,C): (2/3, 1/3, 0)",
            P, Q, n_ABC, A, 1, expect);
    }

    printf("\n--- Prop II.5: opposite edge skew tests ---\n");
    test_skew("L(O,A) vs L(B,C)", O, A, B, C, 1);
    test_skew("L(O,B) vs L(A,C)", O, B, A, C, 1);
    test_skew("L(O,C) vs L(A,B)", O, C, A, B, 1);

    /* adjacent edges share a vertex → coplanar */
    test_skew("L(O,A) vs L(O,B): share vertex → coplanar", O, A, O, B, 0);
    test_skew("L(A,B) vs L(A,C): share vertex → coplanar", A, B, A, C, 0);

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);

    return tests_passed == tests_run ? 0 : 1;
}
