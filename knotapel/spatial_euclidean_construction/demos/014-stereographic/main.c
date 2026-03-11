/*
 * Demo 014: Stereographic Projection
 *
 * Stereographic projection from O onto the face plane x+y+z=1
 * maps the circumsphere to the face plane:
 *
 *   Stereo(Q) = Q / (qx + qy + qz)
 *
 * Central theorem:
 *   The circumsphere equation |Q|^2 = qx + qy + qz makes the
 *   stereographic denominator equal to the inversion denominator.
 *   Therefore Stereo(Q) = Inv(Q) for all Q on the circumsphere.
 *
 * Key results:
 *   - Vertices A, B, C are fixed (on both sphere and plane)
 *   - Antipodal point (1,1,1) maps to centroid of ABC
 *   - Circles through O on circumsphere project to edges of ABC
 *   - The inverse map is also inversion: InvStereo(S) = Inv(S)
 *
 * Key constructions used:
 *   - Inversion (Book XIII)
 *   - Circumsphere (Book IX)
 *   - Face plane (Book VII)
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
    Rat sub_x = rat_make(a.x.p * b.x.q - b.x.p * a.x.q, a.x.q * b.x.q);
    Rat sub_y = rat_make(a.y.p * b.y.q - b.y.p * a.y.q, a.y.q * b.y.q);
    Rat sub_z = rat_make(a.z.p * b.z.q - b.z.p * a.z.q, a.z.q * b.z.q);
    return rvec3_make(sub_x, sub_y, sub_z);
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

/* ---- coordinate sum ---- */

static Rat coord_sum(RVec3 v)
{
    return rat_add(rat_add(v.x, v.y), v.z);
}

/* ---- stereographic projection ---- */

/*
 * Stereographic projection from O onto plane x+y+z=1.
 * For Q on circumsphere with Q != O.
 * Formula: Stereo(Q) = Q / (qx + qy + qz)
 */
static RVec3 stereo(RVec3 q)
{
    Rat s = coord_sum(q);
    return rvec3_make(rat_div(q.x, s), rat_div(q.y, s), rat_div(q.z, s));
}

/*
 * Inversion in unit sphere centered at O: Inv(P) = P / |P|^2
 */
static RVec3 invert(RVec3 p)
{
    Rat len2 = rvec3_len2(p);
    return rvec3_make(rat_div(p.x, len2),
                      rat_div(p.y, len2),
                      rat_div(p.z, len2));
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
    RVec3 A, B, C, K;
    Rat zero;

    A = rvec3_from_ints(1, 0, 0);
    B = rvec3_from_ints(0, 1, 0);
    C = rvec3_from_ints(0, 0, 1);
    K = rvec3_make(rat_make(1, 2), rat_make(1, 2), rat_make(1, 2));
    zero = rat_make(0, 1);

    printf("=== Demo 014: Stereographic Projection ===\n\n");
    printf("Stereographic projection from O onto plane x+y+z=1:\n");
    printf("  Stereo(Q) = Q / (qx + qy + qz)\n\n");

    /* ---- Part 1: Projection formula ---- */
    printf("--- Part 1: Projection formula ---\n");
    {
        RVec3 q110, q111, s;

        check("Stereo(A) = A (vertex fixed)",
              rvec3_eq(stereo(A), A));
        check("Stereo(B) = B (vertex fixed)",
              rvec3_eq(stereo(B), B));
        check("Stereo(C) = C (vertex fixed)",
              rvec3_eq(stereo(C), C));

        q110 = rvec3_from_ints(1, 1, 0);
        s = stereo(q110);
        printf("  Stereo(1,1,0) = "); rvec3_print(s); printf("\n");
        check("Stereo(1,1,0) = (1/2, 1/2, 0) = midpoint(A,B)",
              rvec3_eq(s, rvec3_make(rat_make(1, 2),
                                     rat_make(1, 2), zero)));

        q111 = rvec3_from_ints(1, 1, 1);
        s = stereo(q111);
        printf("  Stereo(1,1,1) = "); rvec3_print(s); printf("\n");
        check("Stereo(1,1,1) = (1/3, 1/3, 1/3) = centroid(ABC)",
              rvec3_eq(s, rvec3_make(rat_make(1, 3),
                                     rat_make(1, 3),
                                     rat_make(1, 3))));

        printf("\n  A, B, C are fixed: they lie on both circumsphere\n");
        printf("  and face plane. (1,1,1) is the antipodal point of O\n");
        printf("  on the circumsphere; it maps to the center of ABC.\n");
    }

    /* ---- Part 2: The circumsphere identity ---- */
    printf("\n--- Part 2: The circumsphere identity |Q|^2 = qx+qy+qz ---\n");
    {
        RVec3 pts[4];
        const char *names[4];
        int i;

        printf("  The circumsphere |Q-K|^2 = 3/4 expands to:\n");
        printf("  |Q|^2 - (qx+qy+qz) + 3/4 = 3/4\n");
        printf("  Therefore |Q|^2 = qx+qy+qz on the circumsphere.\n\n");

        pts[0] = A;
        names[0] = "A";
        pts[1] = rvec3_from_ints(1, 1, 0);
        names[1] = "(1,1,0)";
        pts[2] = rvec3_from_ints(1, 1, 1);
        names[2] = "(1,1,1)";
        pts[3] = rvec3_make(rat_make(3, 5), rat_make(6, 5), zero);
        names[3] = "(3/5,6/5,0)";

        for (i = 0; i < 4; i++) {
            Rat len2 = rvec3_len2(pts[i]);
            Rat sum = coord_sum(pts[i]);
            char desc[80];
            printf("  %s: |Q|^2 = ", names[i]); rat_print(len2);
            printf(", sum = "); rat_print(sum); printf("\n");
            sprintf(desc, "%s: |Q|^2 = coord sum", names[i]);
            check(desc, rat_eq(len2, sum));
        }

        printf("\n  This identity is WHY Stereo = Inv on the circumsphere:\n");
        printf("  Stereo divides by coord sum, Inv divides by |Q|^2,\n");
        printf("  and they are the same number.\n");
    }

    /* ---- Part 3: Stereo = Inv on circumsphere ---- */
    printf("\n--- Part 3: Stereo = Inv (the central theorem) ---\n");
    {
        RVec3 pts[4];
        const char *names[4];
        int i;

        pts[0] = rvec3_from_ints(1, 1, 0);
        names[0] = "(1,1,0)";
        pts[1] = rvec3_from_ints(1, 0, 1);
        names[1] = "(1,0,1)";
        pts[2] = rvec3_from_ints(0, 1, 1);
        names[2] = "(0,1,1)";
        pts[3] = rvec3_from_ints(1, 1, 1);
        names[3] = "(1,1,1)";

        for (i = 0; i < 4; i++) {
            RVec3 s = stereo(pts[i]);
            RVec3 inv = invert(pts[i]);
            char desc[80];
            printf("  %s: Stereo = ", names[i]); rvec3_print(s);
            printf(", Inv = "); rvec3_print(inv); printf("\n");
            sprintf(desc, "Stereo(%s) = Inv(%s)", names[i], names[i]);
            check(desc, rvec3_eq(s, inv));
        }
    }

    /* ---- Part 4: Inverse map and round-trips ---- */
    printf("\n--- Part 4: Inverse map and round-trips ---\n");
    {
        RVec3 face_pts[2], sphere_pts[2];
        int i;

        printf("  The inverse stereographic map (face -> circumsphere)\n");
        printf("  is also inversion: InvStereo(S) = S/|S|^2 = Inv(S).\n");
        printf("  Round-trips reduce to Inv(Inv(P)) = P.\n\n");

        /* Face plane -> circumsphere -> face plane */
        face_pts[0] = rvec3_make(rat_make(1, 2), rat_make(1, 2), zero);
        face_pts[1] = rvec3_make(rat_make(1, 3), rat_make(1, 3),
                                  rat_make(1, 3));

        for (i = 0; i < 2; i++) {
            RVec3 to_sphere = invert(face_pts[i]);
            RVec3 back = stereo(to_sphere);
            printf("  face "); rvec3_print(face_pts[i]);
            printf(" -> sphere "); rvec3_print(to_sphere);
            printf(" -> face "); rvec3_print(back); printf("\n");
            check("round-trip face -> sphere -> face",
                  rvec3_eq(back, face_pts[i]));
        }

        /* Circumsphere -> face plane -> circumsphere */
        sphere_pts[0] = rvec3_from_ints(1, 1, 0);
        sphere_pts[1] = rvec3_from_ints(1, 1, 1);

        for (i = 0; i < 2; i++) {
            RVec3 to_face = stereo(sphere_pts[i]);
            RVec3 back = invert(to_face);
            printf("  sphere "); rvec3_print(sphere_pts[i]);
            printf(" -> face "); rvec3_print(to_face);
            printf(" -> sphere "); rvec3_print(back); printf("\n");
            check("round-trip sphere -> face -> sphere",
                  rvec3_eq(back, sphere_pts[i]));
        }
    }

    /* ---- Part 5: Circles through O -> edges of ABC ---- */
    printf("\n--- Part 5: Circles through O -> edges of ABC ---\n");
    {
        RVec3 circle_pts[3], proj[3];
        Rat circ_r2 = rat_make(3, 4);
        int i;

        printf("  Coordinate planes cut the circumsphere in circles\n");
        printf("  through O. These circles project to edges of ABC.\n\n");

        /* z=0 circle -> edge AB */
        circle_pts[0] = rvec3_make(rat_make(3, 5), rat_make(6, 5), zero);
        /* y=0 circle -> edge AC */
        circle_pts[1] = rvec3_make(rat_make(3, 5), zero, rat_make(6, 5));
        /* x=0 circle -> edge BC */
        circle_pts[2] = rvec3_make(zero, rat_make(3, 5), rat_make(6, 5));

        for (i = 0; i < 3; i++) {
            RVec3 diff = rvec3_sub(circle_pts[i], K);
            int on_circ = rat_eq(rvec3_len2(diff), circ_r2);
            int on_face;
            int on_edge;
            char desc[80];
            const char *edge_names[3];

            edge_names[0] = "AB (z=0)";
            edge_names[1] = "AC (y=0)";
            edge_names[2] = "BC (x=0)";

            proj[i] = stereo(circle_pts[i]);
            on_face = rat_eq(coord_sum(proj[i]), rat_make(1, 1));

            /* Each coordinate plane circle projects onto the edge
             * where that same coordinate is zero */
            if (i == 0) on_edge = rat_eq(proj[i].z, zero);
            else if (i == 1) on_edge = rat_eq(proj[i].y, zero);
            else on_edge = rat_eq(proj[i].x, zero);

            printf("  "); rvec3_print(circle_pts[i]);
            printf(" -> "); rvec3_print(proj[i]);
            printf("  (edge %s)\n", edge_names[i]);

            sprintf(desc, "circle -> edge %s", edge_names[i]);
            check(desc, on_circ && on_face && on_edge);
        }

        printf("\n  The three coordinate planes each contain O and two\n");
        printf("  vertices, cutting the circumsphere in circles through\n");
        printf("  O. Stereographic projection sends circles through the\n");
        printf("  pole to lines, so these become the three edges.\n");
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("Stereographic projection from O onto face plane x+y+z=1:\n");
    printf("  Stereo(Q) = Q / (qx+qy+qz)\n\n");
    printf("The circumsphere identity |Q|^2 = qx+qy+qz means\n");
    printf("stereographic projection IS inversion (Book XIII),\n");
    printf("restricted to the circumsphere.\n\n");
    printf("This unifies three ideas:\n");
    printf("  - Inversion exchanges plane <-> circumsphere (Book XIII)\n");
    printf("  - Stereo projection maps sphere -> plane (classical)\n");
    printf("  - For OUR circumsphere and face plane, they are the SAME map\n\n");
    printf("The face plane is the circumsphere \"unfolded\" through\n");
    printf("the three fixed points A, B, C. Circles through O\n");
    printf("become the edges of triangle ABC.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
