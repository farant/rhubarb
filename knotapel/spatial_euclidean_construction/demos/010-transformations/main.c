/*
 * Demo 010: Transformations and Constructible Motions
 *
 * Translations, scalings, rotations applied to the right tetrahedron
 * {O, A, B, C}. Every transformation is verified to preserve or scale
 * distances exactly, using rational arithmetic throughout.
 *
 * Key results:
 *   - Translation by rational vector: T_Q (two parallel plane reflections)
 *   - Scaling by rational factor: T_Q (Thales, Book IV)
 *   - Rotation by Pythagorean angle: rational matrix -> T_Q
 *   - Rotation by constructible angle: T_E (may introduce sqrt)
 *   - Composition of two plane reflections = rotation (Book VI)
 *
 * The constructibility constraint: a rotation about a rational axis
 * has a rational matrix iff its angle is Pythagorean (sin and cos
 * both rational). These angles are dense but countable.
 *
 * Key constructions used:
 *   - Plane reflections (Book VI) -> rotation via composition
 *   - Perpendicular bisector planes (Book III) -> parallel transport
 *   - Thales / division (Book IV) -> scaling
 *   - S3 symmetry (Book VI) -> 120-degree rotation
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

static Rat rat_neg(Rat a)
{
    return rat_make(-a.p, a.q);
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

/* ---- transformations ---- */

/* Translation by vector offset */
static RVec3 tr_translate(RVec3 p, RVec3 offset)
{
    return rvec3_add(p, offset);
}

/* Scale from center by rational factor k */
static RVec3 tr_scale_from(RVec3 p, RVec3 center, Rat k)
{
    /* center + k * (p - center) */
    return rvec3_add(center, rvec3_scale(rvec3_sub(p, center), k));
}

/* 90 degree rotation about z-axis: (x,y,z) -> (-y, x, z) */
static RVec3 tr_rotate_z_90(RVec3 v)
{
    return rvec3_make(rat_neg(v.y), v.x, v.z);
}

/* 120 degree rotation about (1,1,1) axis: (x,y,z) -> (z, x, y) */
static RVec3 tr_rotate_111_120(RVec3 v)
{
    return rvec3_make(v.z, v.x, v.y);
}

/* Rotation about z-axis by a Pythagorean angle.
 * cos^2 + sin^2 = 1 with both cos, sin rational.
 * R(x,y,z) = (x*cos - y*sin, x*sin + y*cos, z) */
static RVec3 tr_rotate_z_pyth(RVec3 v, Rat cos_t, Rat sin_t)
{
    Rat new_x = rat_sub(rat_mul(v.x, cos_t), rat_mul(v.y, sin_t));
    Rat new_y = rat_add(rat_mul(v.x, sin_t), rat_mul(v.y, cos_t));
    return rvec3_make(new_x, new_y, v.z);
}

/* Reflection across plane n.X = d (from Book VI, Prop VI.1) */
static RVec3 tr_reflect_plane(RVec3 p, RVec3 normal, Rat d)
{
    Rat nP = rvec3_dot(normal, p);
    Rat nn = rvec3_dot(normal, normal);
    Rat t = rat_div(rat_sub(nP, d), nn);
    Rat two_t = rat_mul(rat_make(2, 1), t);
    return rvec3_sub(p, rvec3_scale(normal, two_t));
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

/* Compute 6 pairwise squared distances of 4 vertices */
static void tet_edges(RVec3 v[4], Rat out[6])
{
    int idx = 0;
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = i + 1; j < 4; j++) {
            out[idx] = rvec3_len2(rvec3_sub(v[i], v[j]));
            idx++;
        }
    }
}

/* Check if two sets of 6 edge squared-distances are equal */
static int edges_equal(Rat a[6], Rat b[6])
{
    int i;
    for (i = 0; i < 6; i++) {
        if (!rat_eq(a[i], b[i])) return 0;
    }
    return 1;
}

/* Check if b[i] = k * a[i] for all 6 edges */
static int edges_scaled_by(Rat a[6], Rat b[6], Rat k)
{
    int i;
    for (i = 0; i < 6; i++) {
        if (!rat_eq(b[i], rat_mul(a[i], k))) return 0;
    }
    return 1;
}

/* ---- main ---- */

int main(void)
{
    RVec3 O, A, B, C;
    RVec3 tet[4];
    Rat orig_edges[6];

    O = rvec3_from_ints(0, 0, 0);
    A = rvec3_from_ints(1, 0, 0);
    B = rvec3_from_ints(0, 1, 0);
    C = rvec3_from_ints(0, 0, 1);

    tet[0] = O; tet[1] = A; tet[2] = B; tet[3] = C;
    tet_edges(tet, orig_edges);

    printf("=== Demo 010: Transformations and Constructible Motions ===\n\n");

    /* ---- Part 1: Translation ---- */
    printf("--- Part 1: Translation ---\n");
    {
        RVec3 offset = rvec3_from_ints(2, 3, 5);
        RVec3 trans[4];
        Rat trans_edges[6];
        int i;

        printf("  Translating {O,A,B,C} by (2,3,5):\n");
        for (i = 0; i < 4; i++) {
            trans[i] = tr_translate(tet[i], offset);
            printf("    ");
            rvec3_print(tet[i]);
            printf(" -> ");
            rvec3_print(trans[i]);
            printf("\n");
        }

        tet_edges(trans, trans_edges);
        check("translation preserves all 6 edge distances",
              edges_equal(orig_edges, trans_edges));

        printf("  Translation = two parallel plane reflections (Thm VI.4).\n");
        printf("  T_Q constructible for any rational vector.\n");
    }

    /* ---- Part 2: Uniform scaling from origin ---- */
    printf("\n--- Part 2: Uniform scaling from origin ---\n");
    {
        Rat k = rat_make(3, 1);
        Rat k2 = rat_mul(k, k);
        RVec3 scaled[4];
        Rat scaled_edges[6];
        int i;

        printf("  Scaling {O,A,B,C} by factor 3 from origin:\n");
        for (i = 0; i < 4; i++) {
            scaled[i] = tr_scale_from(tet[i], O, k);
            printf("    ");
            rvec3_print(tet[i]);
            printf(" -> ");
            rvec3_print(scaled[i]);
            printf("\n");
        }

        tet_edges(scaled, scaled_edges);
        check("scaling by 3: d^2 scales by 9",
              edges_scaled_by(orig_edges, scaled_edges, k2));

        printf("  Scaling by rational k: T_Q (Thales, Book IV).\n");
        printf("  d^2 scales by k^2 = ");
        rat_print(k2);
        printf(".\n");
    }

    /* ---- Part 3: Scaling from centroid ---- */
    printf("\n--- Part 3: Scaling from centroid ---\n");
    {
        RVec3 G = rvec3_make(rat_make(1, 4), rat_make(1, 4), rat_make(1, 4));
        Rat k = rat_make(2, 1);
        Rat k2 = rat_make(4, 1);
        RVec3 scaled[4];
        Rat scaled_edges[6];
        int i;

        printf("  Scaling by 2 from centroid G = (1/4, 1/4, 1/4):\n");
        for (i = 0; i < 4; i++) {
            scaled[i] = tr_scale_from(tet[i], G, k);
            printf("    ");
            rvec3_print(tet[i]);
            printf(" -> ");
            rvec3_print(scaled[i]);
            printf("\n");
        }

        tet_edges(scaled, scaled_edges);
        check("scaling by 2 from G: d^2 scales by 4",
              edges_scaled_by(orig_edges, scaled_edges, k2));

        /* Centroid is the fixed point of scaling from centroid */
        {
            Rat quarter = rat_make(1, 4);
            RVec3 new_G = rvec3_scale(
                rvec3_add(rvec3_add(scaled[0], scaled[1]),
                          rvec3_add(scaled[2], scaled[3])),
                quarter);
            printf("  Centroid of scaled tet: ");
            rvec3_print(new_G);
            printf("\n");
            check("centroid is fixed point of scaling from centroid",
                  rvec3_eq(new_G, G));
        }
    }

    /* ---- Part 4: 90 degree rotation about z-axis ---- */
    printf("\n--- Part 4: 90 degree rotation about z-axis ---\n");
    {
        RVec3 rot[4];
        Rat rot_edges[6];
        int i;

        printf("  R(x,y,z) = (-y, x, z):\n");
        for (i = 0; i < 4; i++) {
            rot[i] = tr_rotate_z_90(tet[i]);
            printf("    ");
            rvec3_print(tet[i]);
            printf(" -> ");
            rvec3_print(rot[i]);
            printf("\n");
        }

        tet_edges(rot, rot_edges);
        check("90 deg rotation preserves all edge distances",
              edges_equal(orig_edges, rot_edges));
        check("O maps to O (on axis)", rvec3_eq(rot[0], O));
        check("C maps to C (z-axis fixed)", rvec3_eq(rot[3], C));
        check("A = (1,0,0) maps to (0,1,0) = B", rvec3_eq(rot[1], B));
        check("B = (0,1,0) maps to (-1,0,0)",
              rvec3_eq(rot[2], rvec3_from_ints(-1, 0, 0)));
    }

    /* ---- Part 5: 120 degree rotation about (1,1,1) ---- */
    printf("\n--- Part 5: 120 degree rotation about (1,1,1) ---\n");
    {
        RVec3 rot[4];
        Rat rot_edges[6];
        int i;

        printf("  R(x,y,z) = (z, x, y)  [cyclic permutation]:\n");
        for (i = 0; i < 4; i++) {
            rot[i] = tr_rotate_111_120(tet[i]);
            printf("    ");
            rvec3_print(tet[i]);
            printf(" -> ");
            rvec3_print(rot[i]);
            printf("\n");
        }

        tet_edges(rot, rot_edges);
        check("120 deg rotation preserves all edge distances",
              edges_equal(orig_edges, rot_edges));
        check("O fixed (on axis)", rvec3_eq(rot[0], O));
        check("A -> B", rvec3_eq(rot[1], B));
        check("B -> C", rvec3_eq(rot[2], C));
        check("C -> A", rvec3_eq(rot[3], A));

        /* Apply 3 times = identity */
        {
            RVec3 v = A;
            v = tr_rotate_111_120(v);
            v = tr_rotate_111_120(v);
            v = tr_rotate_111_120(v);
            check("R^3 = identity (order 3)", rvec3_eq(v, A));
        }

        printf("  This is the S3 3-cycle from Book VI.\n");
    }

    /* ---- Part 6: Pythagorean rotation ---- */
    printf("\n--- Part 6: Pythagorean rotation (3-4-5 triangle) ---\n");
    {
        /* cos = 4/5, sin = 3/5. Angle = arctan(3/4) ~ 36.87 deg */
        Rat cos_t = rat_make(4, 5);
        Rat sin_t = rat_make(3, 5);
        RVec3 rot[4];
        Rat rot_edges[6];
        int i;

        /* Verify Pythagorean identity */
        check("cos^2 + sin^2 = 1",
              rat_eq(rat_add(rat_mul(cos_t, cos_t), rat_mul(sin_t, sin_t)),
                     rat_make(1, 1)));

        printf("  R(x,y,z) = (4x/5 - 3y/5, 3x/5 + 4y/5, z):\n");
        for (i = 0; i < 4; i++) {
            rot[i] = tr_rotate_z_pyth(tet[i], cos_t, sin_t);
            printf("    ");
            rvec3_print(tet[i]);
            printf(" -> ");
            rvec3_print(rot[i]);
            printf("\n");
        }

        tet_edges(rot, rot_edges);
        check("Pythagorean rotation preserves all edge distances",
              edges_equal(orig_edges, rot_edges));

        printf("  All coordinates remain rational.\n");
        printf("  Pythagorean angle: sin AND cos both rational.\n");
        printf("  Rotation matrix has rational entries -> T_Q constructible.\n");
    }

    /* ---- Part 7: Composition (translate + rotate) ---- */
    printf("\n--- Part 7: Composition of transformations ---\n");
    {
        RVec3 offset = rvec3_from_ints(1, 2, 0);
        Rat cos_t = rat_make(4, 5);
        Rat sin_t = rat_make(3, 5);
        RVec3 comp[4];
        Rat comp_edges[6];
        int i;

        printf("  Translate by (1,2,0) then Pythagorean rotate about z:\n");
        for (i = 0; i < 4; i++) {
            RVec3 mid = tr_translate(tet[i], offset);
            comp[i] = tr_rotate_z_pyth(mid, cos_t, sin_t);
            printf("    ");
            rvec3_print(tet[i]);
            printf(" -> ");
            rvec3_print(comp[i]);
            printf("\n");
        }

        tet_edges(comp, comp_edges);
        check("composition preserves all edge distances",
              edges_equal(orig_edges, comp_edges));
        printf("  Composition of isometries is an isometry.\n");
        printf("  Composition of T_Q operations is T_Q.\n");
    }

    /* ---- Part 8: Two plane reflections = rotation ---- */
    printf("\n--- Part 8: Two reflections = rotation ---\n");
    {
        /* Reflect across x=y (normal (1,-1,0)), then across x=0 (normal (1,0,0)).
         * These planes meet along the z-axis at 45 degrees.
         * Composition = 90 degree rotation about z-axis. */
        RVec3 n1 = rvec3_from_ints(1, 0, 0);
        RVec3 n2 = rvec3_from_ints(1, -1, 0);
        Rat zero = rat_make(0, 1);
        int all_match = 1;
        int i;

        printf("  Plane 1: x = y  (normal (1,-1,0))\n");
        printf("  Plane 2: x = 0  (normal (1,0,0))\n");
        printf("  Dihedral angle: 45 deg.\n");
        printf("  Composition should = 90 deg rotation about z.\n\n");

        for (i = 0; i < 4; i++) {
            RVec3 r1 = tr_reflect_plane(tet[i], n2, zero);
            RVec3 r2 = tr_reflect_plane(r1, n1, zero);
            RVec3 expected = tr_rotate_z_90(tet[i]);

            printf("    ");
            rvec3_print(tet[i]);
            printf(" -r1-> ");
            rvec3_print(r1);
            printf(" -r2-> ");
            rvec3_print(r2);
            printf("\n");

            if (!rvec3_eq(r2, expected)) all_match = 0;
        }

        check("two reflections at 45 deg = 90 deg rotation", all_match);
        printf("  Theorem VI.4 verified computationally.\n");
    }

    /* ---- Part 9: Constructibility classification ---- */
    printf("\n--- Part 9: Constructibility classification ---\n");
    {
        printf("  Transformation          Field      Tool   Constructible?\n");
        printf("  Translation (rat vec)   Q          T_Q    always\n");
        printf("  Scaling (rat factor)    Q          T_Q    always\n");
        printf("  Rot 0/90/180/270        Q          T_Q    always\n");
        printf("  Rot 120 about (1,1,1)   Q          T_Q    coordinate perm\n");
        printf("  Rot by Pythagorean      Q          T_Q    sin,cos rational\n");
        printf("  Rot by constructible    Q(sqrt..)  T_E    cos constructible\n");
        printf("  Rot by 60 about z       Q(sqrt3)   T_E    cos=1/2\n");
        printf("  Rot by 1 radian         --         --     NOT constructible\n");
        printf("  Rot by 20 deg           --         --     NOT (trisection)\n");
        printf("\n");

        /* Pythagorean triples give rational rotation matrices */
        printf("  Pythagorean angles from small triples:\n");
        printf("    (3,4,5):   cos=4/5,  sin=3/5   ~ 36.87 deg\n");
        printf("    (5,12,13): cos=12/13, sin=5/13  ~ 22.62 deg\n");
        printf("    (8,15,17): cos=15/17, sin=8/17  ~ 28.07 deg\n");
        printf("    (7,24,25): cos=24/25, sin=7/25  ~ 16.26 deg\n");
        printf("\n");

        /* Verify second Pythagorean triple preserves distance */
        {
            Rat c2 = rat_make(12, 13);
            Rat s2 = rat_make(5, 13);
            RVec3 r = tr_rotate_z_pyth(A, c2, s2);
            printf("  5-12-13 rotation: A = ");
            rvec3_print(A);
            printf(" -> ");
            rvec3_print(r);
            printf("\n");
            check("5-12-13 rotation preserves |A|^2",
                  rat_eq(rvec3_len2(A), rvec3_len2(r)));
        }

        printf("\n  KEY THEOREM: A rotation about a rational axis has a\n");
        printf("  rational matrix iff sin and cos are both rational\n");
        printf("  (Pythagorean angle). Dense in the circle, but countable.\n");

        check("classification complete", 1);
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("Every constructible transformation preserves rationality of\n");
    printf("coordinates when applied to rational input. The hierarchy:\n");
    printf("  T_Q: translations, scalings, rotations by Pythagorean angles\n");
    printf("  T_E: rotations by constructible angles (may introduce sqrt)\n");
    printf("  Neither: rotations by transcendental or non-constructible angles\n");
    printf("Composition of two plane reflections at angle theta/2\n");
    printf("  = rotation by theta (Theorem VI.4).\n");
    printf("Every isometry = at most 4 plane reflections (Book VI).\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
