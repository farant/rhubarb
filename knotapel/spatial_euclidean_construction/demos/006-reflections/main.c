/*
 * Demo 006: Reflections and Isometries
 *
 * Three types of reflection, all rational:
 *
 * 1. Reflection across a plane:  R = 2*F - P  where F = foot (Prop V.1)
 *    Equivalently:  R = P - 2*((n.P - d)/(n.n)) * n
 *
 * 2. Reflection across a line:   R = 2*F - P  where F = foot (Prop V.2)
 *    In 3D this is a 180-degree rotation about the line.
 *
 * 3. Reflection through a point: R = 2*C - P
 *
 * Key results:
 * - All reflections preserve rational points (rational in, rational out)
 * - Reflections are isometries (preserve squared distances)
 * - Two reflections across intersecting planes = rotation by 2*dihedral angle
 * - Every isometry of R^3 is a composition of at most 4 reflections
 *
 * Applied to the right tetrahedron {O, A, B, C}.
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

static long vec3_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
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
    RVec3 r;
    r.x = rat_sub(a.x, b.x);
    r.y = rat_sub(a.y, b.y);
    r.z = rat_sub(a.z, b.z);
    return r;
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

/* ---- Reflection operations ---- */

/*
 * Reflect point P across plane n.X = d.
 * R = P - 2 * ((n.P - d) / (n.n)) * n
 */
static RVec3 reflect_across_plane(RVec3 P, Vec3 normal, long d)
{
    Rat nP = rat_add(rat_add(
        rat_mul(rat_make(normal.x, 1), P.x),
        rat_mul(rat_make(normal.y, 1), P.y)),
        rat_mul(rat_make(normal.z, 1), P.z));
    long nn = vec3_dot(normal, normal);
    Rat two_t = rat_make(2 * (nP.p * nn - d * nP.q * nn),
                         nP.q * nn * nn);
    RVec3 result;

    /* Simplify: 2t = 2*(nP - d)/(n.n) */
    two_t = rat_mul(rat_make(2, 1),
                    rat_make(nP.p * 1 - d * nP.q, nP.q * nn));

    result.x = rat_sub(P.x, rat_mul(two_t, rat_make(normal.x, 1)));
    result.y = rat_sub(P.y, rat_mul(two_t, rat_make(normal.y, 1)));
    result.z = rat_sub(P.z, rat_mul(two_t, rat_make(normal.z, 1)));
    return result;
}

/*
 * Reflect point P across line through Q with direction d.
 * F = Q + ((P-Q).d / (d.d)) * d      (foot of perpendicular)
 * R = 2*F - P
 */
static RVec3 reflect_across_line(RVec3 P, Vec3 Q, Vec3 dir)
{
    RVec3 PQ;
    Rat PQd, dd_r, t;
    RVec3 foot, result;

    PQ.x = rat_sub(P.x, rat_make(Q.x, 1));
    PQ.y = rat_sub(P.y, rat_make(Q.y, 1));
    PQ.z = rat_sub(P.z, rat_make(Q.z, 1));

    PQd = rat_add(rat_add(
        rat_mul(PQ.x, rat_make(dir.x, 1)),
        rat_mul(PQ.y, rat_make(dir.y, 1))),
        rat_mul(PQ.z, rat_make(dir.z, 1)));
    dd_r = rat_make(vec3_dot(dir, dir), 1);
    t = rat_make(PQd.p * dd_r.q, PQd.q * dd_r.p);

    foot.x = rat_add(rat_make(Q.x, 1), rat_mul(t, rat_make(dir.x, 1)));
    foot.y = rat_add(rat_make(Q.y, 1), rat_mul(t, rat_make(dir.y, 1)));
    foot.z = rat_add(rat_make(Q.z, 1), rat_mul(t, rat_make(dir.z, 1)));

    /* R = 2*F - P */
    result.x = rat_sub(rat_mul(rat_make(2, 1), foot.x), P.x);
    result.y = rat_sub(rat_mul(rat_make(2, 1), foot.y), P.y);
    result.z = rat_sub(rat_mul(rat_make(2, 1), foot.z), P.z);
    return result;
}

/*
 * Reflect point P through center C.
 * R = 2*C - P
 */
static RVec3 reflect_through_point(RVec3 P, RVec3 C)
{
    RVec3 result;
    result.x = rat_sub(rat_mul(rat_make(2, 1), C.x), P.x);
    result.y = rat_sub(rat_mul(rat_make(2, 1), C.y), P.y);
    result.z = rat_sub(rat_mul(rat_make(2, 1), C.z), P.z);
    return result;
}

/* ---- Squared distance helper ---- */

static Rat dist_sq(RVec3 a, RVec3 b)
{
    return rvec3_len2(rvec3_sub(a, b));
}

/* ---- Point set for orbit computation ---- */

#define MAX_POINTS 4096

static RVec3 orbit[MAX_POINTS];
static int orbit_count = 0;

static int orbit_contains(RVec3 p)
{
    int i;
    for (i = 0; i < orbit_count; i++) {
        if (rvec3_eq(orbit[i], p)) return 1;
    }
    return 0;
}

static int orbit_add(RVec3 p)
{
    if (orbit_contains(p)) return 0;
    if (orbit_count >= MAX_POINTS) return 0;
    orbit[orbit_count++] = p;
    return 1;
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

    RVec3 r;

    printf("=== Demo 006: Reflections and Isometries ===\n\n");

    /* ---- Part 1: Reflection across coordinate planes ---- */
    printf("--- Part 1: Reflection across coordinate planes ---\n");
    {
        /* Reflect A=(1,0,0) across x=0 → (-1,0,0) */
        r = reflect_across_plane(A, vec3_make(1, 0, 0), 0);
        printf("  A across x=0: "); rvec3_print(r); printf("\n");
        check("A across x=0 = (-1,0,0)",
              rvec3_eq(r, rvec3_make(rat_make(-1,1), rat_make(0,1), rat_make(0,1))));

        /* Reflect A across y=0 → A (already on the plane) */
        r = reflect_across_plane(A, vec3_make(0, 1, 0), 0);
        printf("  A across y=0: "); rvec3_print(r); printf("\n");
        check("A across y=0 = A (on plane)", rvec3_eq(r, A));

        /* Reflect B=(0,1,0) across z=0 → B (on the plane) */
        r = reflect_across_plane(B, vec3_make(0, 0, 1), 0);
        printf("  B across z=0: "); rvec3_print(r); printf("\n");
        check("B across z=0 = B (on plane)", rvec3_eq(r, B));

        /* Reflect C=(0,0,1) across z=0 → (0,0,-1) */
        r = reflect_across_plane(C, vec3_make(0, 0, 1), 0);
        printf("  C across z=0: "); rvec3_print(r); printf("\n");
        check("C across z=0 = (0,0,-1)",
              rvec3_eq(r, rvec3_make(rat_make(0,1), rat_make(0,1), rat_make(-1,1))));
    }

    /* ---- Part 2: Reflection across the face plane x+y+z=1 ---- */
    printf("\n--- Part 2: Reflection across face plane x+y+z=1 ---\n");
    {
        RVec3 expect;

        /* Reflect O across x+y+z=1: foot is (1/3,1/3,1/3), R = 2F - O = (2/3,2/3,2/3) */
        r = reflect_across_plane(O, vec3_make(1, 1, 1), 1);
        printf("  O across x+y+z=1: "); rvec3_print(r); printf("\n");
        expect = rvec3_make(rat_make(2,3), rat_make(2,3), rat_make(2,3));
        check("O across x+y+z=1 = (2/3,2/3,2/3)", rvec3_eq(r, expect));

        /* A is ON the plane, so R = A */
        r = reflect_across_plane(A, vec3_make(1, 1, 1), 1);
        printf("  A across x+y+z=1: "); rvec3_print(r); printf("\n");
        check("A across x+y+z=1 = A (on plane)", rvec3_eq(r, A));

        /* Similarly B and C are on the plane */
        r = reflect_across_plane(B, vec3_make(1, 1, 1), 1);
        check("B across x+y+z=1 = B (on plane)", rvec3_eq(r, B));
        r = reflect_across_plane(C, vec3_make(1, 1, 1), 1);
        check("C across x+y+z=1 = C (on plane)", rvec3_eq(r, C));

        /* Reflect (-1,0,0) across x+y+z=1: n.P = -1, d=1, t=(-1-1)/3 = -2/3
         * R = (-1,0,0) - 2*(-2/3)*(1,1,1) = (-1,0,0) + (4/3,4/3,4/3) = (1/3,4/3,4/3) */
        r = reflect_across_plane(
            rvec3_make(rat_make(-1,1), rat_make(0,1), rat_make(0,1)),
            vec3_make(1, 1, 1), 1);
        printf("  (-1,0,0) across x+y+z=1: "); rvec3_print(r); printf("\n");
        expect = rvec3_make(rat_make(1,3), rat_make(4,3), rat_make(4,3));
        check("(-1,0,0) across x+y+z=1 = (1/3,4/3,4/3)", rvec3_eq(r, expect));
    }

    /* ---- Part 3: Reflection across a line (180-degree rotation) ---- */
    printf("\n--- Part 3: Reflection across a line ---\n");
    {
        RVec3 expect;

        /* Reflect B across x-axis L(O,A): foot is O, R = 2O - B = (0,-1,0) */
        r = reflect_across_line(B, O_i, vec3_make(1, 0, 0));
        printf("  B across x-axis: "); rvec3_print(r); printf("\n");
        expect = rvec3_make(rat_make(0,1), rat_make(-1,1), rat_make(0,1));
        check("B across x-axis = (0,-1,0)", rvec3_eq(r, expect));

        /* Reflect C across x-axis: foot is O, R = (0,0,-1) */
        r = reflect_across_line(C, O_i, vec3_make(1, 0, 0));
        printf("  C across x-axis: "); rvec3_print(r); printf("\n");
        expect = rvec3_make(rat_make(0,1), rat_make(0,1), rat_make(-1,1));
        check("C across x-axis = (0,0,-1)", rvec3_eq(r, expect));

        /* Reflect A across x-axis: A is ON the line, R = A */
        r = reflect_across_line(A, O_i, vec3_make(1, 0, 0));
        check("A across x-axis = A (on line)", rvec3_eq(r, A));

        /* Reflect B across L(A,C): direction d = (-1,0,1)
         * foot of B to L(A,C) = (1/2,0,1/2) (from Demo 005)
         * R = 2*(1/2,0,1/2) - (0,1,0) = (1,0,1) - (0,1,0) = (1,-1,1) */
        r = reflect_across_line(B, A_i, vec3_make(-1, 0, 1));
        printf("  B across L(A,C): "); rvec3_print(r); printf("\n");
        expect = rvec3_make(rat_make(1,1), rat_make(-1,1), rat_make(1,1));
        check("B across L(A,C) = (1,-1,1)", rvec3_eq(r, expect));
    }

    /* ---- Part 4: Reflection through a point ---- */
    printf("\n--- Part 4: Reflection through a point ---\n");
    {
        RVec3 expect, mid;

        /* Reflect A through O: R = 2O - A = (-1,0,0) */
        r = reflect_through_point(A, O);
        printf("  A through O: "); rvec3_print(r); printf("\n");
        expect = rvec3_make(rat_make(-1,1), rat_make(0,1), rat_make(0,1));
        check("A through O = (-1,0,0)", rvec3_eq(r, expect));

        /* Reflect O through midpoint of AB = (1/2,1/2,0): R = 2*(1/2,1/2,0) - O = (1,1,0) */
        mid = rvec3_make(rat_make(1,2), rat_make(1,2), rat_make(0,1));
        r = reflect_through_point(O, mid);
        printf("  O through mid(A,B): "); rvec3_print(r); printf("\n");
        expect = rvec3_make(rat_make(1,1), rat_make(1,1), rat_make(0,1));
        check("O through mid(A,B) = (1,1,0)", rvec3_eq(r, expect));

        /* Reflect A through centroid of ABC = (1/3,1/3,1/3) */
        {
            RVec3 centroid = rvec3_make(rat_make(1,3), rat_make(1,3), rat_make(1,3));
            r = reflect_through_point(A, centroid);
            printf("  A through centroid(ABC): "); rvec3_print(r); printf("\n");
            expect = rvec3_make(rat_make(-1,3), rat_make(2,3), rat_make(2,3));
            check("A through centroid(ABC) = (-1/3,2/3,2/3)", rvec3_eq(r, expect));
        }
    }

    /* ---- Part 5: Isometry verification ---- */
    printf("\n--- Part 5: Isometry verification ---\n");
    printf("  Checking that reflections preserve squared distances.\n");
    {
        RVec3 rA, rB, rC, rO;
        Rat d_orig, d_refl;

        /* Reflect all 4 points across x+y+z=1 */
        rO = reflect_across_plane(O, vec3_make(1, 1, 1), 1);
        rA = reflect_across_plane(A, vec3_make(1, 1, 1), 1);
        rB = reflect_across_plane(B, vec3_make(1, 1, 1), 1);
        rC = reflect_across_plane(C, vec3_make(1, 1, 1), 1);

        /* dist_sq(O,A) should equal dist_sq(rO,rA) */
        d_orig = dist_sq(O, A);
        d_refl = dist_sq(rO, rA);
        printf("  dist^2(O,A) = "); rat_print(d_orig);
        printf(",  dist^2(R(O),R(A)) = "); rat_print(d_refl); printf("\n");
        check("reflection preserves dist^2(O,A)", rat_eq(d_orig, d_refl));

        d_orig = dist_sq(O, B);
        d_refl = dist_sq(rO, rB);
        check("reflection preserves dist^2(O,B)", rat_eq(d_orig, d_refl));

        d_orig = dist_sq(A, B);
        d_refl = dist_sq(rA, rB);
        printf("  dist^2(A,B) = "); rat_print(d_orig);
        printf(",  dist^2(R(A),R(B)) = "); rat_print(d_refl); printf("\n");
        check("reflection preserves dist^2(A,B)", rat_eq(d_orig, d_refl));

        d_orig = dist_sq(A, C);
        d_refl = dist_sq(rA, rC);
        check("reflection preserves dist^2(A,C)", rat_eq(d_orig, d_refl));

        d_orig = dist_sq(B, C);
        d_refl = dist_sq(rB, rC);
        check("reflection preserves dist^2(B,C)", rat_eq(d_orig, d_refl));

        d_orig = dist_sq(O, C);
        d_refl = dist_sq(rO, rC);
        check("reflection preserves dist^2(O,C)", rat_eq(d_orig, d_refl));
    }

    /* ---- Part 6: Composition of reflections ---- */
    printf("\n--- Part 6: Composition of reflections ---\n");
    {
        RVec3 r1, r2, expect;

        /* Two reflections across intersecting coordinate planes = rotation
         * Reflect B across z=0, then across y=0.
         * z=0: (0,1,0) → (0,1,0) (B is on z=0)
         * y=0: (0,1,0) → (0,-1,0)
         * This is 180-degree rotation about x-axis. */
        r1 = reflect_across_plane(B, vec3_make(0, 0, 1), 0);
        r2 = reflect_across_plane(r1, vec3_make(0, 1, 0), 0);
        printf("  B: z=0 then y=0: "); rvec3_print(r2); printf("\n");
        expect = rvec3_make(rat_make(0,1), rat_make(-1,1), rat_make(0,1));
        check("two coord reflections = 180-deg rotation", rvec3_eq(r2, expect));

        /* Same composition on C = (0,0,1):
         * z=0: (0,0,1) → (0,0,-1)
         * y=0: (0,0,-1) → (0,0,-1) (on y=0 plane)
         * 180-deg rotation about x-axis: (x, y, z) → (x, -y, -z) */
        r1 = reflect_across_plane(C, vec3_make(0, 0, 1), 0);
        r2 = reflect_across_plane(r1, vec3_make(0, 1, 0), 0);
        printf("  C: z=0 then y=0: "); rvec3_print(r2); printf("\n");
        expect = rvec3_make(rat_make(0,1), rat_make(0,1), rat_make(-1,1));
        check("rotation maps (0,0,1) to (0,0,-1)", rvec3_eq(r2, expect));

        /* Verify this is exactly (x, -y, -z) on A */
        r1 = reflect_across_plane(A, vec3_make(0, 0, 1), 0);
        r2 = reflect_across_plane(r1, vec3_make(0, 1, 0), 0);
        check("rotation fixes A = (1,0,0)", rvec3_eq(r2, A));

        /* Two reflections across parallel planes = translation
         * Reflect across x=0 then x=1:
         * x=0: (0,y,z) → (0,y,z);  (a,y,z) → (-a,y,z)
         * x=1: (-a,y,z) → (2-(-a),y,z) = (2+a,y,z)
         * Net: (a,y,z) → (2+a,y,z) = translation by (2,0,0) */
        r1 = reflect_across_plane(O, vec3_make(1, 0, 0), 0);
        r2 = reflect_across_plane(r1, vec3_make(1, 0, 0), 1);
        printf("  O: x=0 then x=1: "); rvec3_print(r2); printf("\n");
        expect = rvec3_make(rat_make(2,1), rat_make(0,1), rat_make(0,1));
        check("two parallel reflections = translation by 2", rvec3_eq(r2, expect));

        r1 = reflect_across_plane(B, vec3_make(1, 0, 0), 0);
        r2 = reflect_across_plane(r1, vec3_make(1, 0, 0), 1);
        printf("  B: x=0 then x=1: "); rvec3_print(r2); printf("\n");
        expect = rvec3_make(rat_make(2,1), rat_make(1,1), rat_make(0,1));
        check("translation maps (0,1,0) to (2,1,0)", rvec3_eq(r2, expect));
    }

    /* ---- Part 7: Orbit under face-plane reflections ---- */
    printf("\n--- Part 7: Orbit of O under 4 face-plane reflections ---\n");
    {
        /* The 4 face planes of the tetrahedron:
         * Pi_0: x=0  (normal (1,0,0), d=0)  -- face OBC
         * Pi_1: y=0  (normal (0,1,0), d=0)  -- face OAC
         * Pi_2: z=0  (normal (0,0,1), d=0)  -- face OAB
         * Pi_3: x+y+z=1  (normal (1,1,1), d=1)  -- face ABC
         */
        Vec3 normals[4];
        long ds[4];
        int gen, new_this_gen, i, j;
        int gen_start, gen_end;

        normals[0] = vec3_make(1, 0, 0); ds[0] = 0;
        normals[1] = vec3_make(0, 1, 0); ds[1] = 0;
        normals[2] = vec3_make(0, 0, 1); ds[2] = 0;
        normals[3] = vec3_make(1, 1, 1); ds[3] = 1;

        /* Seed with O */
        orbit_count = 0;
        orbit_add(O);

        printf("  Gen 0: 1 point (seed = O)\n");

        for (gen = 1; gen <= 4; gen++) {
            gen_start = 0;
            gen_end = orbit_count;
            new_this_gen = 0;

            for (i = gen_start; i < gen_end; i++) {
                for (j = 0; j < 4; j++) {
                    RVec3 reflected = reflect_across_plane(
                        orbit[i], normals[j], ds[j]);
                    if (orbit_add(reflected)) {
                        new_this_gen++;
                    }
                }
            }
            printf("  Gen %d: %d total (+%d new)\n",
                   gen, orbit_count, new_this_gen);
        }

        printf("  Orbit points:\n");
        for (i = 0; i < orbit_count; i++) {
            printf("    [%2d] ", i);
            rvec3_print(orbit[i]);
            printf("\n");
        }

        check("orbit has at least 4 points after 4 generations",
              orbit_count >= 4);
    }

    /* ---- Part 8: S3 symmetry of the right tetrahedron ---- */
    printf("\n--- Part 8: S3 symmetry (permutation of axes) ---\n");
    {
        /* The 6 permutations of (x,y,z) are generated by
         * transpositions, which are reflections across planes like x=y.
         * The plane x=y has normal (1,-1,0), d=0. */
        RVec3 expect;

        /* Swap x and y: reflect across x=y (normal (1,-1,0), d=0)
         * A = (1,0,0) → ? */
        r = reflect_across_plane(A, vec3_make(1, -1, 0), 0);
        printf("  A across x=y: "); rvec3_print(r); printf("\n");
        check("A across x=y = B", rvec3_eq(r, B));

        r = reflect_across_plane(B, vec3_make(1, -1, 0), 0);
        check("B across x=y = A", rvec3_eq(r, A));

        r = reflect_across_plane(C, vec3_make(1, -1, 0), 0);
        check("C across x=y = C (on plane)", rvec3_eq(r, C));

        /* Swap y and z: reflect across y=z (normal (0,1,-1), d=0) */
        r = reflect_across_plane(B, vec3_make(0, 1, -1), 0);
        printf("  B across y=z: "); rvec3_print(r); printf("\n");
        check("B across y=z = C", rvec3_eq(r, C));

        r = reflect_across_plane(C, vec3_make(0, 1, -1), 0);
        check("C across y=z = B", rvec3_eq(r, B));

        /* 3-cycle: swap x→y via x=y, then y→z via y=z
         * A = (1,0,0) → (0,1,0) → (0,0,1) = C */
        r = reflect_across_plane(A, vec3_make(1, -1, 0), 0);
        r = reflect_across_plane(r, vec3_make(0, 1, -1), 0);
        printf("  A: x=y then y=z: "); rvec3_print(r); printf("\n");
        expect = rvec3_make(rat_make(0,1), rat_make(0,1), rat_make(1,1));
        check("composition maps A→C (cyclic permutation)", rvec3_eq(r, expect));
    }

    /* ---- Part 9: Involution check ---- */
    printf("\n--- Part 9: Double reflection = identity ---\n");
    {
        RVec3 r2;

        /* Reflecting twice across any plane returns to original */
        r = reflect_across_plane(O, vec3_make(1, 1, 1), 1);
        r2 = reflect_across_plane(r, vec3_make(1, 1, 1), 1);
        check("O reflected twice across x+y+z=1 = O", rvec3_eq(r2, O));

        r = reflect_across_plane(A, vec3_make(1, 0, 0), 0);
        r2 = reflect_across_plane(r, vec3_make(1, 0, 0), 0);
        check("A reflected twice across x=0 = A", rvec3_eq(r2, A));

        /* Reflecting twice across a line */
        r = reflect_across_line(B, O_i, vec3_make(1, 0, 0));
        r2 = reflect_across_line(r, O_i, vec3_make(1, 0, 0));
        check("B reflected twice across x-axis = B", rvec3_eq(r2, B));

        /* Reflecting twice through a point */
        {
            RVec3 center = rvec3_make(rat_make(1,3), rat_make(1,3), rat_make(1,3));
            r = reflect_through_point(A, center);
            r2 = reflect_through_point(r, center);
            check("A reflected twice through centroid = A", rvec3_eq(r2, A));
        }
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("All three reflection types produce RATIONAL images.\n");
    printf("Reflections are ISOMETRIES (preserve squared distances).\n");
    printf("Two reflections across intersecting planes = ROTATION.\n");
    printf("Two reflections across parallel planes = TRANSLATION.\n");
    printf("Every reflection is an INVOLUTION (applying twice = identity).\n");
    printf("The S3 symmetry of the right tetrahedron is generated by\n");
    printf("  reflections across the planes x=y and y=z.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
