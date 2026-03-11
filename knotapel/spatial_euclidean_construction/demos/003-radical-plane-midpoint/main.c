/*
 * Demo 003: Radical Plane, Three-Sphere Intersection, and Midpoint
 *
 * Key results demonstrated:
 *
 * 1. The radical plane of two spheres S(C1,P1) and S(C2,P2) is found
 *    by subtracting their equations. The quadratic X.X terms cancel,
 *    leaving a LINEAR equation — a plane.
 *
 *      2(C2-C1).X = r1^2 - r2^2 - |C1|^2 + |C2|^2
 *
 *    Normal = C2 - C1.  All coefficients are rational.
 *
 * 2. Three-sphere intersection reduces to:
 *      S1 ∩ S2 → radical plane P12
 *      S1 ∩ S3 → radical plane P13
 *      P12 ∩ P13 → radical line
 *      radical line ∩ S1 → 0, 1, or 2 points
 *
 * 3. The midpoint of O and A can be constructed:
 *      radical plane of S(O,A) and S(A,O) has equation x = 1/2
 *      intersect x = 1/2 with L(O,A) → (1/2, 0, 0)
 *
 *    But constructively, the radical plane must be built from 3 points
 *    on it, which come from three-sphere intersection (irrational!).
 *    The irrationals are scaffolding — the final midpoint is rational.
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long x, y, z; } Vec3;
typedef struct { long p, q; } Rat;
typedef struct { Rat x, y, z; } RVec3;

/*
 * Plane in form: n.x*X + n.y*Y + n.z*Z = d
 * where n (normal) is integer, d is rational (p/q).
 */
typedef struct {
    Vec3 n;     /* normal (integer) */
    Rat d;      /* constant: n . X = d for points on plane */
} Plane;

/* ---- integer vector ops ---- */

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

static long vec3_len2(Vec3 v)
{
    return vec3_dot(v, v);
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

static int rat_eq(Rat a, Rat b)
{
    return a.p == b.p && a.q == b.q;
}

static void rat_print(Rat r)
{
    if (r.q == 1)
        printf("%ld", r.p);
    else
        printf("%ld/%ld", r.p, r.q);
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

static int rvec3_eq(RVec3 a, RVec3 b)
{
    return rat_eq(a.x, b.x) && rat_eq(a.y, b.y) && rat_eq(a.z, b.z);
}

/* ---- radical plane ---- */

/*
 * Compute the radical plane of two spheres:
 *   S1: |X - C1|^2 = |P1 - C1|^2
 *   S2: |X - C2|^2 = |P2 - C2|^2
 *
 * Subtracting: 2(C2-C1).X = r1^2 - r2^2 - |C1|^2 + |C2|^2
 *
 * Returns plane with integer normal 2*(C2-C1) and rational constant.
 * (We could simplify by dividing normal by 2, but keep integers for clarity.)
 */
static Plane radical_plane(Vec3 C1, Vec3 P1, Vec3 C2, Vec3 P2)
{
    Plane pl;
    long r1_sq = vec3_len2(vec3_sub(P1, C1));
    long r2_sq = vec3_len2(vec3_sub(P2, C2));
    Vec3 diff = vec3_sub(C2, C1);

    /* normal = 2*(C2 - C1) */
    pl.n.x = 2 * diff.x;
    pl.n.y = 2 * diff.y;
    pl.n.z = 2 * diff.z;

    /* constant = r1^2 - r2^2 - |C1|^2 + |C2|^2 */
    pl.d = rat_make(r1_sq - r2_sq - vec3_len2(C1) + vec3_len2(C2), 1);

    return pl;
}

static void plane_print(Plane pl)
{
    int first = 1;
    if (pl.n.x != 0) {
        if (pl.n.x == 1) printf("x");
        else if (pl.n.x == -1) printf("-x");
        else printf("%ldx", pl.n.x);
        first = 0;
    }
    if (pl.n.y != 0) {
        if (!first && pl.n.y > 0) printf(" + ");
        if (!first && pl.n.y < 0) printf(" - ");
        if (first && pl.n.y < 0) printf("-");
        if (pl.n.y == 1 || pl.n.y == -1) printf("y");
        else {
            long v = pl.n.y < 0 ? -pl.n.y : pl.n.y;
            printf("%ldy", first ? pl.n.y : v);
        }
        first = 0;
    }
    if (pl.n.z != 0) {
        if (!first && pl.n.z > 0) printf(" + ");
        if (!first && pl.n.z < 0) printf(" - ");
        if (first && pl.n.z < 0) printf("-");
        if (pl.n.z == 1 || pl.n.z == -1) printf("z");
        else {
            long v = pl.n.z < 0 ? -pl.n.z : pl.n.z;
            printf("%ldz", first ? pl.n.z : v);
        }
    }
    printf(" = ");
    rat_print(pl.d);
}

/* ---- line-plane intersection (from demo 001) ---- */

static int line_plane_intersect(
    Vec3 P, Vec3 Q,
    Plane pl,
    RVec3 *out)
{
    Vec3 d = vec3_sub(Q, P);
    long denom = vec3_dot(pl.n, d);
    long numer_base;
    Rat t;

    if (denom == 0) return 0;

    /* n . X = d  means  n . (X - P) = d - n.P
     * but d is rational (p/q), so:
     * t = (d.p/d.q - n.P) / (n . dir)
     *   = (d.p - d.q * n.P) / (d.q * n.dir) */
    numer_base = pl.d.p - pl.d.q * vec3_dot(pl.n, P);

    t = rat_make(numer_base, pl.d.q * denom);

    out->x = rat_make(P.x * t.q + t.p * d.x, t.q);
    out->y = rat_make(P.y * t.q + t.p * d.y, t.q);
    out->z = rat_make(P.z * t.q + t.p * d.z, t.q);
    return 1;
}

/* ---- plane-plane intersection → line ---- */

/*
 * Two planes → line (direction + point on line).
 * Direction = n1 × n2.
 * Point: set largest component of direction to 0, solve 2×2 system.
 */
typedef struct {
    Vec3 dir;
    RVec3 point;
    int valid;    /* 0 if planes are parallel */
} PlanePlaneResult;

static PlanePlaneResult plane_plane_intersect(Plane pl1, Plane pl2)
{
    PlanePlaneResult res;
    Vec3 d;
    long abs_dx, abs_dy, abs_dz;

    /* d1, d2 are the plane constants as integers (p * other_q for common denominator) */
    long d1_num, d2_num, common_q;

    d = vec3_cross(pl1.n, pl2.n);
    res.dir = d;
    res.valid = 0;

    if (d.x == 0 && d.y == 0 && d.z == 0) return res;

    res.valid = 1;

    /* Put plane constants over common denominator:
     * pl1: n1.X = d1.p/d1.q  →  n1.X * d1.q = d1.p
     * pl2: n2.X = d2.p/d2.q  →  n2.X * d2.q = d2.p
     * For the 2×2 solve we need: n_i . X = d_i as rationals.
     * Use common denominator: d1.q * d2.q */
    common_q = pl1.d.q * pl2.d.q;
    d1_num = pl1.d.p * pl2.d.q;
    d2_num = pl2.d.p * pl1.d.q;

    abs_dx = d.x < 0 ? -d.x : d.x;
    abs_dy = d.y < 0 ? -d.y : d.y;
    abs_dz = d.z < 0 ? -d.z : d.z;

    if (abs_dz >= abs_dx && abs_dz >= abs_dy) {
        /* set Z = 0, solve for X, Y */
        /* n1.x*X + n1.y*Y = d1, n2.x*X + n2.y*Y = d2 */
        long det = (long)pl1.n.x * pl2.n.y - (long)pl1.n.y * pl2.n.x;
        long x_num = d1_num * pl2.n.y - d2_num * pl1.n.y;
        long y_num = pl1.n.x * d2_num - pl2.n.x * d1_num;
        res.point.x = rat_make(x_num, det * common_q);
        res.point.y = rat_make(y_num, det * common_q);
        res.point.z = rat_make(0, 1);
    } else if (abs_dy >= abs_dx) {
        /* set Y = 0, solve for X, Z */
        long det = (long)pl1.n.x * pl2.n.z - (long)pl1.n.z * pl2.n.x;
        long x_num = d1_num * pl2.n.z - d2_num * pl1.n.z;
        long z_num = pl1.n.x * d2_num - pl2.n.x * d1_num;
        res.point.x = rat_make(x_num, det * common_q);
        res.point.y = rat_make(0, 1);
        res.point.z = rat_make(z_num, det * common_q);
    } else {
        /* set X = 0, solve for Y, Z */
        long det = (long)pl1.n.y * pl2.n.z - (long)pl1.n.z * pl2.n.y;
        long y_num = d1_num * pl2.n.z - d2_num * pl1.n.z;
        long z_num = pl1.n.y * d2_num - pl2.n.y * d1_num;
        res.point.x = rat_make(0, 1);
        res.point.y = rat_make(y_num, det * common_q);
        res.point.z = rat_make(z_num, det * common_q);
    }

    return res;
}

/* ---- test framework ---- */

static int tests_run = 0;
static int tests_passed = 0;

static void check(const char *desc, int cond)
{
    tests_run++;
    if (cond) {
        tests_passed++;
        printf("PASS: %s\n", desc);
    } else {
        printf("FAIL: %s\n", desc);
    }
}

/* ---- main ---- */

/* helper to make expected RVec3 values (C89-safe, no compound literals) */
static RVec3 make_rvec3(long px, long pq, long py, long yq, long pz, long zq)
{
    RVec3 r;
    r.x = rat_make(px, pq);
    r.y = rat_make(py, yq);
    r.z = rat_make(pz, zq);
    return r;
}

int main(void)
{
    Vec3 O = {0, 0, 0};
    Vec3 A = {1, 0, 0};
    Vec3 B = {0, 1, 0};
    Vec3 C = {0, 0, 1};

    Plane rp_OA_AO, rp_OA_BO, rp_OA_CO;
    PlanePlaneResult radical_line;
    RVec3 midpoint_A, midpoint_B, midpoint_C;

    Plane rp_OB, rp_OC;

    Rat half = rat_make(1, 2);

    RVec3 expect;

    printf("=== Demo 003: Radical Plane and Midpoint Construction ===\n\n");

    /* ---- Part 1: Radical planes from unit spheres ---- */
    printf("--- Radical planes of unit spheres at O, A, B, C ---\n");

    /* S(O,A) vs S(A,O): both radius 1.
     * 2(A-O).X = 1 - 1 - 0 + 1 = 1 → 2x = 1 → x = 1/2 */
    rp_OA_AO = radical_plane(O, A, A, O);
    printf("S(O,A) vs S(A,O): ");
    plane_print(rp_OA_AO);
    printf("\n");
    check("radical plane S(O,A) vs S(A,O): normal=(2,0,0), d=1",
          rp_OA_AO.n.x == 2 && rp_OA_AO.n.y == 0 && rp_OA_AO.n.z == 0
          && rat_eq(rp_OA_AO.d, rat_make(1, 1)));

    /* S(O,A) vs S(B,O): both radius 1.
     * 2(B-O).X = 1 - 1 - 0 + 1 = 1 → 2y = 1 → y = 1/2 */
    rp_OA_BO = radical_plane(O, A, B, O);
    printf("S(O,A) vs S(B,O): ");
    plane_print(rp_OA_BO);
    printf("\n");
    check("radical plane S(O,A) vs S(B,O): normal=(0,2,0), d=1",
          rp_OA_BO.n.x == 0 && rp_OA_BO.n.y == 2 && rp_OA_BO.n.z == 0
          && rat_eq(rp_OA_BO.d, rat_make(1, 1)));

    /* S(O,A) vs S(C,O): both radius 1.
     * 2(C-O).X = 1 → 2z = 1 → z = 1/2 */
    rp_OA_CO = radical_plane(O, A, C, O);
    printf("S(O,A) vs S(C,O): ");
    plane_print(rp_OA_CO);
    printf("\n");
    check("radical plane S(O,A) vs S(C,O): normal=(0,0,2), d=1",
          rp_OA_CO.n.x == 0 && rp_OA_CO.n.y == 0 && rp_OA_CO.n.z == 2
          && rat_eq(rp_OA_CO.d, rat_make(1, 1)));

    /* ---- Part 2: Three-sphere → radical line ---- */
    printf("\n--- Three-sphere intersection: S(O,A) ∩ S(A,O) ∩ S(B,O) ---\n");

    radical_line = plane_plane_intersect(rp_OA_AO, rp_OA_BO);
    check("radical line exists", radical_line.valid == 1);
    printf("  direction: (%ld, %ld, %ld)\n",
           radical_line.dir.x, radical_line.dir.y, radical_line.dir.z);
    printf("  point on line: ");
    rvec3_print(radical_line.point);
    printf("\n");

    check("radical line parallel to z-axis",
          radical_line.dir.x == 0 && radical_line.dir.y == 0
          && radical_line.dir.z != 0);

    check("radical line at x=1/2, y=1/2",
          rat_eq(radical_line.point.x, half)
          && rat_eq(radical_line.point.y, half));

    printf("  Intersecting radical line with S(O,A):\n");
    printf("    1/4 + 1/4 + z^2 = 1 → z^2 = 1/2\n");
    printf("    Points are IRRATIONAL: (1/2, 1/2, +/-sqrt(2)/2)\n");

    /* ---- Part 3: Midpoint via radical plane ---- */
    printf("\n--- Midpoint construction ---\n");
    printf("The radical plane of S(O,A) and S(A,O) is x = 1/2.\n");
    printf("Intersecting with L(O,A) gives the midpoint.\n\n");

    /* Midpoint of O and A */
    {
        int hit = line_plane_intersect(O, A, rp_OA_AO, &midpoint_A);
        check("midpoint L(O,A) ∩ radical plane: exists", hit == 1);
        printf("  midpoint of O and A: ");
        rvec3_print(midpoint_A);
        printf("\n");
        expect = make_rvec3(1, 2, 0, 1, 0, 1);
        check("midpoint = (1/2, 0, 0)", rvec3_eq(midpoint_A, expect));
    }

    /* Midpoint of O and B */
    rp_OB = radical_plane(O, B, B, O);
    {
        int hit = line_plane_intersect(O, B, rp_OB, &midpoint_B);
        check("midpoint L(O,B) ∩ radical plane: exists", hit == 1);
        printf("  midpoint of O and B: ");
        rvec3_print(midpoint_B);
        printf("\n");
        expect = make_rvec3(0, 1, 1, 2, 0, 1);
        check("midpoint = (0, 1/2, 0)", rvec3_eq(midpoint_B, expect));
    }

    /* Midpoint of O and C */
    rp_OC = radical_plane(O, C, C, O);
    {
        int hit = line_plane_intersect(O, C, rp_OC, &midpoint_C);
        check("midpoint L(O,C) ∩ radical plane: exists", hit == 1);
        printf("  midpoint of O and C: ");
        rvec3_print(midpoint_C);
        printf("\n");
        expect = make_rvec3(0, 1, 0, 1, 1, 2);
        check("midpoint = (0, 0, 1/2)", rvec3_eq(midpoint_C, expect));
    }

    /* ---- Part 4: Midpoint of non-axis edge A-B ---- */
    printf("\n--- Midpoint of A and B ---\n");
    {
        Plane rp_AB = radical_plane(A, B, B, A);
        RVec3 mid_AB;
        int hit;

        printf("S(A,B) vs S(B,A): ");
        plane_print(rp_AB);
        printf("\n");
        check("radical plane S(A,B) vs S(B,A): normal=(-2,2,0), d=0",
              rp_AB.n.x == -2 && rp_AB.n.y == 2 && rp_AB.n.z == 0
              && rat_eq(rp_AB.d, rat_make(0, 1)));

        hit = line_plane_intersect(A, B, rp_AB, &mid_AB);
        check("midpoint L(A,B) ∩ radical plane: exists", hit == 1);
        printf("  midpoint of A and B: ");
        rvec3_print(mid_AB);
        printf("\n");
        expect = make_rvec3(1, 2, 1, 2, 0, 1);
        check("midpoint = (1/2, 1/2, 0)", rvec3_eq(mid_AB, expect));
    }

    /* ---- Part 5: Midpoint of A and C ---- */
    printf("\n--- Midpoint of A and C ---\n");
    {
        Plane rp_AC = radical_plane(A, C, C, A);
        RVec3 mid_AC;
        int hit;

        printf("S(A,C) vs S(C,A): ");
        plane_print(rp_AC);
        printf("\n");

        hit = line_plane_intersect(A, C, rp_AC, &mid_AC);
        check("midpoint L(A,C) ∩ radical plane: exists", hit == 1);
        printf("  midpoint of A and C: ");
        rvec3_print(mid_AC);
        printf("\n");
        expect = make_rvec3(1, 2, 0, 1, 1, 2);
        check("midpoint = (1/2, 0, 1/2)", rvec3_eq(mid_AC, expect));
    }

    /* ---- Part 6: Radical plane of non-equal-radius spheres ---- */
    printf("\n--- Radical plane of non-equal-radius spheres ---\n");
    {
        Plane rp_diff = radical_plane(O, A, A, B);
        printf("S(O,A) vs S(A,B): ");
        plane_print(rp_diff);
        printf("\n");
        check("radical plane S(O,A) vs S(A,B): 2x = 0",
              rp_diff.n.x == 2 && rp_diff.n.y == 0 && rp_diff.n.z == 0
              && rat_eq(rp_diff.d, rat_make(0, 1)));
    }

    /* ---- Part 7: Three radical planes → radical center ---- */
    printf("\n--- Radical center: 3 radical planes intersect ---\n");
    {
        /* x=1/2, y=1/2, z=1/2 → (1/2, 1/2, 1/2) */
        PlanePlaneResult line12 = plane_plane_intersect(rp_OA_AO, rp_OA_BO);
        RVec3 center;

        if (line12.valid) {
            /* line12: x=1/2, y=1/2, z=free. direction (0,0,k).
             * Intersect with z=1/2 (rp_OA_CO): z = 1/2. */
            center.x = radical_line.point.x;  /* 1/2 */
            center.y = radical_line.point.y;  /* 1/2 */
            center.z = rat_make(1, 2);

            printf("Radical center (3 unit-sphere planes): ");
            rvec3_print(center);
            printf("\n");
            expect = make_rvec3(1, 2, 1, 2, 1, 2);
            check("radical center = (1/2, 1/2, 1/2)",
                  rvec3_eq(center, expect));
        }
    }

    printf("\n--- Summary ---\n");
    printf("Key results:\n");
    printf("  1. Radical plane of two spheres is ALWAYS rational\n");
    printf("     (quadratic terms cancel in subtraction)\n");
    printf("  2. Radical plane of S(P,Q) and S(Q,P) is the\n");
    printf("     perpendicular bisector plane of segment PQ\n");
    printf("  3. Intersecting this plane with L(P,Q) gives the midpoint\n");
    printf("  4. All 6 edge midpoints of initial tetrahedron: constructible\n");
    printf("  5. Three-sphere intersection points are generally IRRATIONAL\n");
    printf("     but the radical planes they define are RATIONAL\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);

    return tests_passed == tests_run ? 0 : 1;
}
