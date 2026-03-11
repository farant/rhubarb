/*
 * Demo 005: Perpendicular and Angular Constructions
 *
 * Constructions unique to or richer in 3D:
 *
 * 1. Foot of perpendicular from a point to a plane
 *    F = P - ((n.P - d) / (n.n)) * n
 *    Rational in, rational out.
 *
 * 2. Foot of perpendicular from a point to a line
 *    F = Q + ((P-Q).d / (d.d)) * d
 *    Rational in, rational out.
 *
 * 3. Common perpendicular of two skew lines (PURELY 3D)
 *    Given L1(P1,d1), L2(P2,d2):
 *    - perpendicular direction n = d1 x d2
 *    - solve for feet on each line
 *    - distance^2 is rational (distance may involve sqrt)
 *    Applied to the 3 skew edge pairs of our tetrahedron.
 *
 * 4. Dihedral angles between faces of the initial tetrahedron
 *    cos(theta) = (n1.n2) / (|n1| |n2|)
 *    The cosine-squared is rational; the cosine itself may be irrational.
 *
 * 5. Distance from point to plane and point to line
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
    Vec3 r;
    r.x = a.x - b.x; r.y = a.y - b.y; r.z = a.z - b.z;
    return r;
}

static long vec3_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
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

static RVec3 rvec3_from_int(Vec3 v)
{
    RVec3 r;
    r.x = rat_make(v.x, 1);
    r.y = rat_make(v.y, 1);
    r.z = rat_make(v.z, 1);
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

static Rat rvec3_dot(RVec3 a, RVec3 b)
{
    return rat_add(rat_add(rat_mul(a.x, b.x), rat_mul(a.y, b.y)),
                   rat_mul(a.z, b.z));
}

static RVec3 rvec3_sub(RVec3 a, RVec3 b)
{
    RVec3 r;
    r.x = rat_sub(a.x, b.x);
    r.y = rat_sub(a.y, b.y);
    r.z = rat_sub(a.z, b.z);
    return r;
}

static Rat rvec3_len2(RVec3 v)
{
    return rvec3_dot(v, v);
}

/* ---- Perpendicular constructions ---- */

/*
 * Foot of perpendicular from point P to plane n.X = d.
 * F = P - ((n.P - d) / (n.n)) * n
 * All rational when inputs are integer/rational.
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

/*
 * Foot of perpendicular from point P to line through Q with direction d.
 * F = Q + ((P-Q).d / (d.d)) * d
 * All rational.
 */
static RVec3 foot_point_to_line(Vec3 P, Vec3 Q, Vec3 d)
{
    Vec3 PQ = vec3_sub(P, Q);
    long PQd = vec3_dot(PQ, d);
    long dd = vec3_dot(d, d);
    Rat t = rat_make(PQd, dd);
    RVec3 result;

    result.x = rat_add(rat_make(Q.x, 1), rat_mul(t, rat_make(d.x, 1)));
    result.y = rat_add(rat_make(Q.y, 1), rat_mul(t, rat_make(d.y, 1)));
    result.z = rat_add(rat_make(Q.z, 1), rat_mul(t, rat_make(d.z, 1)));
    return result;
}

/*
 * Common perpendicular of two skew lines.
 * L1: P1 + s*d1,  L2: P2 + t*d2
 * Perpendicular direction: n = d1 x d2 (nonzero iff lines not parallel)
 *
 * Solve: (P2 + t*d2) - (P1 + s*d1) = lambda * n
 * This gives 3 equations in 3 unknowns (s, t, lambda).
 *
 * Using the standard formula:
 *   s = det(P2-P1, d2, n) / det(d1, d2, n)  [NOT: this is wrong]
 *
 * Actually, more directly: we need
 *   (P1 + s*d1) to (P2 + t*d2) perpendicular to both d1 and d2.
 *   So: (P2-P1 + t*d2 - s*d1) . d1 = 0
 *       (P2-P1 + t*d2 - s*d1) . d2 = 0
 *
 * Let w = P2 - P1.
 *   w.d1 + t*(d2.d1) - s*(d1.d1) = 0
 *   w.d2 + t*(d2.d2) - s*(d1.d2) = 0
 *
 * 2x2 system for s and t:
 *   s*(d1.d1) - t*(d1.d2) = w.d1
 *   s*(d1.d2) - t*(d2.d2) = w.d2
 *
 * det = (d1.d1)*(d2.d2) - (d1.d2)^2   [= |d1 x d2|^2 by Lagrange identity]
 */
typedef struct {
    RVec3 foot1;    /* foot on L1 */
    RVec3 foot2;    /* foot on L2 */
    Rat dist_sq;    /* squared distance between feet */
    int valid;      /* 0 if lines are parallel */
} CommonPerpResult;

static CommonPerpResult common_perpendicular(
    Vec3 P1, Vec3 d1,
    Vec3 P2, Vec3 d2)
{
    CommonPerpResult res;
    Vec3 w = vec3_sub(P2, P1);
    long a = vec3_dot(d1, d1);   /* d1.d1 */
    long b = vec3_dot(d1, d2);   /* d1.d2 */
    long c = vec3_dot(d2, d2);   /* d2.d2 */
    long e = vec3_dot(w, d1);    /* w.d1  */
    long f = vec3_dot(w, d2);    /* w.d2  */
    long det = a * c - b * b;
    Rat s, t;

    res.valid = 0;
    if (det == 0) return res;  /* parallel */

    res.valid = 1;

    /* s = (e*c - f*b) / det,  t = (e*b - f*a) / det */
    /* From the 2x2 system:
     *   a*s - b*t = e
     *   b*s - c*t = f
     * Cramer's rule:
     *   s = (e*c - f*b) / (a*c - b*b)  [wait, let me redo]
     *   det = | a  -b | = -a*c + b*b = -(a*c - b*b)
     *         | b  -c |
     * Hmm, let me be more careful.
     *   a*s - b*t = e     ... (1)
     *   b*s - c*t = f     ... (2)
     * From (1): s = (e + b*t) / a
     * Sub into (2): b*(e + b*t)/a - c*t = f
     *   b*e/a + b^2*t/a - c*t = f
     *   t*(b^2/a - c) = f - b*e/a
     *   t*(b^2 - a*c)/a = (a*f - b*e)/a
     *   t = (a*f - b*e) / (b^2 - a*c) = (b*e - a*f) / (a*c - b^2)
     * And s = (e + b*t) / a = (e*(a*c - b^2) + b*(b*e - a*f)) / (a*(a*c - b^2))
     *       = (e*a*c - e*b^2 + b^2*e - a*b*f) / (a*(a*c - b^2))
     *       = (e*a*c - a*b*f) / (a*(a*c - b^2))
     *       = (e*c - b*f) / (a*c - b^2)
     */
    s = rat_make(e * c - b * f, det);
    t = rat_make(b * e - a * f, det);

    /* foot1 = P1 + s * d1 */
    res.foot1.x = rat_add(rat_make(P1.x, 1), rat_mul(s, rat_make(d1.x, 1)));
    res.foot1.y = rat_add(rat_make(P1.y, 1), rat_mul(s, rat_make(d1.y, 1)));
    res.foot1.z = rat_add(rat_make(P1.z, 1), rat_mul(s, rat_make(d1.z, 1)));

    /* foot2 = P2 + t * d2 */
    res.foot2.x = rat_add(rat_make(P2.x, 1), rat_mul(t, rat_make(d2.x, 1)));
    res.foot2.y = rat_add(rat_make(P2.y, 1), rat_mul(t, rat_make(d2.y, 1)));
    res.foot2.z = rat_add(rat_make(P2.z, 1), rat_mul(t, rat_make(d2.z, 1)));

    /* squared distance */
    {
        RVec3 diff = rvec3_sub(res.foot2, res.foot1);
        res.dist_sq = rvec3_len2(diff);
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
        printf("  PASS: %s\n", desc);
    } else {
        printf("  FAIL: %s\n", desc);
    }
}

/* ---- main ---- */

int main(void)
{
    Vec3 O = vec3_make(0, 0, 0);
    Vec3 A = vec3_make(1, 0, 0);
    Vec3 B = vec3_make(0, 1, 0);
    Vec3 C = vec3_make(0, 0, 1);

    printf("=== Demo 005: Perpendicular and Angular Constructions ===\n\n");

    /* ---- Part 1: Foot of perpendicular from point to plane ---- */
    printf("--- Part 1: Foot of perpendicular, point to plane ---\n");
    {
        RVec3 foot;
        RVec3 expect;

        /* C = (0,0,1) to plane z=0 (normal (0,0,1), d=0) */
        foot = foot_point_to_plane(C, vec3_make(0, 0, 1), 0);
        printf("  C to z=0: ");
        rvec3_print(foot);
        printf("\n");
        expect = rvec3_from_int(O);
        check("foot of C to z=0 is O", rvec3_eq(foot, expect));

        /* O = (0,0,0) to plane x+y+z=1 (normal (1,1,1), d=1) */
        foot = foot_point_to_plane(O, vec3_make(1, 1, 1), 1);
        printf("  O to x+y+z=1: ");
        rvec3_print(foot);
        printf("\n");
        expect.x = rat_make(1, 3);
        expect.y = rat_make(1, 3);
        expect.z = rat_make(1, 3);
        check("foot of O to x+y+z=1 is (1/3,1/3,1/3)", rvec3_eq(foot, expect));

        /* A = (1,0,0) to plane x+y+z=1 */
        foot = foot_point_to_plane(A, vec3_make(1, 1, 1), 1);
        printf("  A to x+y+z=1: ");
        rvec3_print(foot);
        printf("\n");
        /* n.A = 1, d = 1, t = (1-1)/3 = 0 → foot = A itself */
        expect = rvec3_from_int(A);
        check("A is on plane x+y+z=1 (foot is A)", rvec3_eq(foot, expect));

        /* (2,-1,0) to plane x+y+z=1 */
        {
            Vec3 P = vec3_make(2, -1, 0);
            foot = foot_point_to_plane(P, vec3_make(1, 1, 1), 1);
            printf("  (2,-1,0) to x+y+z=1: ");
            rvec3_print(foot);
            printf("\n");
            /* n.P = 2-1+0 = 1, d=1, t=0 → foot = P itself! */
            check("(2,-1,0) is on plane x+y+z=1", rvec3_eq(foot, rvec3_from_int(P)));
        }
    }

    /* ---- Part 2: Foot of perpendicular from point to line ---- */
    printf("\n--- Part 2: Foot of perpendicular, point to line ---\n");
    {
        RVec3 foot;
        RVec3 expect;
        Rat dist_sq;

        /* C = (0,0,1) to L(O,A) = x-axis, direction (1,0,0) */
        foot = foot_point_to_line(C, O, vec3_make(1, 0, 0));
        printf("  C to L(O,A): ");
        rvec3_print(foot);
        printf("\n");
        expect = rvec3_from_int(O);
        check("foot of C to x-axis is O", rvec3_eq(foot, expect));

        /* distance^2 = |C - O|^2 = 1 */
        dist_sq = rvec3_len2(rvec3_sub(rvec3_from_int(C), foot));
        check("distance^2 from C to x-axis is 1",
              rat_eq(dist_sq, rat_make(1, 1)));

        /* B = (0,1,0) to L(A,C): through (1,0,0) direction (-1,0,1) */
        foot = foot_point_to_line(B, A, vec3_sub(C, A));
        printf("  B to L(A,C): ");
        rvec3_print(foot);
        printf("\n");
        /* d = (-1,0,1), P-Q = B-A = (-1,1,0)
         * (P-Q).d = (-1)(-1)+(1)(0)+(0)(1) = 1
         * d.d = 1+0+1 = 2
         * t = 1/2
         * foot = (1,0,0) + (1/2)(-1,0,1) = (1/2, 0, 1/2) */
        expect.x = rat_make(1, 2);
        expect.y = rat_make(0, 1);
        expect.z = rat_make(1, 2);
        check("foot of B to L(A,C) is (1/2,0,1/2)", rvec3_eq(foot, expect));

        /* That foot is the midpoint of A and C! */
        check("foot is midpoint of A and C",
              rat_eq(expect.x, rat_make(1, 2)) &&
              rat_eq(expect.z, rat_make(1, 2)));

        /* O to L(A,B): through (1,0,0) direction (-1,1,0) */
        foot = foot_point_to_line(O, A, vec3_sub(B, A));
        printf("  O to L(A,B): ");
        rvec3_print(foot);
        printf("\n");
        /* d = (-1,1,0), P-Q = O-A = (-1,0,0)
         * (P-Q).d = 1, d.d = 2, t = 1/2
         * foot = (1,0,0) + (1/2)(-1,1,0) = (1/2, 1/2, 0) */
        expect.x = rat_make(1, 2);
        expect.y = rat_make(1, 2);
        expect.z = rat_make(0, 1);
        check("foot of O to L(A,B) is (1/2,1/2,0)", rvec3_eq(foot, expect));
    }

    /* ---- Part 3: Common perpendicular of skew lines ---- */
    printf("\n--- Part 3: Common perpendicular of skew edges ---\n");
    printf("The 3 skew edge pairs from Theorem II.4:\n\n");
    {
        CommonPerpResult cp;
        RVec3 midpt;
        RVec3 expect_f1, expect_f2;

        /* Pair 1: L(O,A) vs L(B,C) */
        /* L(O,A): P1=O, d1=(1,0,0) */
        /* L(B,C): P2=B, d2=C-B=(0,-1,1) */
        printf("  Pair 1: L(O,A) vs L(B,C)\n");
        cp = common_perpendicular(O, vec3_sub(A, O), B, vec3_sub(C, B));
        check("pair 1: not parallel", cp.valid);
        printf("    foot on L(O,A): ");
        rvec3_print(cp.foot1);
        printf("\n    foot on L(B,C): ");
        rvec3_print(cp.foot2);
        printf("\n    dist^2 = ");
        rat_print(cp.dist_sq);
        printf("\n");

        expect_f1 = rvec3_from_int(O);
        expect_f2.x = rat_make(0, 1);
        expect_f2.y = rat_make(1, 2);
        expect_f2.z = rat_make(1, 2);
        check("foot1 = O = (0,0,0)", rvec3_eq(cp.foot1, expect_f1));
        check("foot2 = (0, 1/2, 1/2)", rvec3_eq(cp.foot2, expect_f2));
        check("dist^2 = 1/2", rat_eq(cp.dist_sq, rat_make(1, 2)));

        /* Midpoint of the common perpendicular segment */
        midpt.x = rat_div(rat_add(cp.foot1.x, cp.foot2.x), rat_make(2, 1));
        midpt.y = rat_div(rat_add(cp.foot1.y, cp.foot2.y), rat_make(2, 1));
        midpt.z = rat_div(rat_add(cp.foot1.z, cp.foot2.z), rat_make(2, 1));
        printf("    midpoint: ");
        rvec3_print(midpt);
        printf("\n\n");

        /* Pair 2: L(O,B) vs L(A,C) */
        printf("  Pair 2: L(O,B) vs L(A,C)\n");
        cp = common_perpendicular(O, vec3_sub(B, O), A, vec3_sub(C, A));
        check("pair 2: not parallel", cp.valid);
        printf("    foot on L(O,B): ");
        rvec3_print(cp.foot1);
        printf("\n    foot on L(A,C): ");
        rvec3_print(cp.foot2);
        printf("\n    dist^2 = ");
        rat_print(cp.dist_sq);
        printf("\n");

        expect_f1 = rvec3_from_int(O);
        expect_f2.x = rat_make(1, 2);
        expect_f2.y = rat_make(0, 1);
        expect_f2.z = rat_make(1, 2);
        check("foot1 = O = (0,0,0)", rvec3_eq(cp.foot1, expect_f1));
        check("foot2 = (1/2, 0, 1/2)", rvec3_eq(cp.foot2, expect_f2));
        check("dist^2 = 1/2", rat_eq(cp.dist_sq, rat_make(1, 2)));

        midpt.x = rat_div(rat_add(cp.foot1.x, cp.foot2.x), rat_make(2, 1));
        midpt.y = rat_div(rat_add(cp.foot1.y, cp.foot2.y), rat_make(2, 1));
        midpt.z = rat_div(rat_add(cp.foot1.z, cp.foot2.z), rat_make(2, 1));
        printf("    midpoint: ");
        rvec3_print(midpt);
        printf("\n\n");

        /* Pair 3: L(O,C) vs L(A,B) */
        printf("  Pair 3: L(O,C) vs L(A,B)\n");
        cp = common_perpendicular(O, vec3_sub(C, O), A, vec3_sub(B, A));
        check("pair 3: not parallel", cp.valid);
        printf("    foot on L(O,C): ");
        rvec3_print(cp.foot1);
        printf("\n    foot on L(A,B): ");
        rvec3_print(cp.foot2);
        printf("\n    dist^2 = ");
        rat_print(cp.dist_sq);
        printf("\n");

        expect_f1 = rvec3_from_int(O);
        expect_f2.x = rat_make(1, 2);
        expect_f2.y = rat_make(1, 2);
        expect_f2.z = rat_make(0, 1);
        check("foot1 = O = (0,0,0)", rvec3_eq(cp.foot1, expect_f1));
        check("foot2 = (1/2, 1/2, 0)", rvec3_eq(cp.foot2, expect_f2));
        check("dist^2 = 1/2", rat_eq(cp.dist_sq, rat_make(1, 2)));

        midpt.x = rat_div(rat_add(cp.foot1.x, cp.foot2.x), rat_make(2, 1));
        midpt.y = rat_div(rat_add(cp.foot1.y, cp.foot2.y), rat_make(2, 1));
        midpt.z = rat_div(rat_add(cp.foot1.z, cp.foot2.z), rat_make(2, 1));
        printf("    midpoint: ");
        rvec3_print(midpt);
        printf("\n");
    }

    /* ---- Part 4: Dihedral angles (as cos^2) ---- */
    printf("\n--- Part 4: Dihedral angles of the tetrahedron ---\n");
    printf("  (Reporting cos^2 since it is always rational)\n\n");
    {
        /* Face normals:
         * OAB (z=0): n = (0,0,1)
         * OAC (y=0): n = (0,1,0)
         * OBC (x=0): n = (1,0,0)
         * ABC (x+y+z=1): n = (1,1,1)
         */
        Vec3 n_OAB = vec3_make(0, 0, 1);
        Vec3 n_OAC = vec3_make(0, 1, 0);
        Vec3 n_OBC = vec3_make(1, 0, 0);
        Vec3 n_ABC = vec3_make(1, 1, 1);

        /* cos^2(theta) = (n1.n2)^2 / (|n1|^2 * |n2|^2) */
        long dot_val, len1_sq, len2_sq;
        Rat cos2;

        /* Coordinate planes: 90 degrees */
        dot_val = vec3_dot(n_OAB, n_OAC);
        printf("  OAB vs OAC (z=0 vs y=0): cos^2 = %ld^2/(%ld*%ld) = ",
               dot_val, vec3_len2(n_OAB), vec3_len2(n_OAC));
        cos2 = rat_make(dot_val * dot_val,
                        vec3_len2(n_OAB) * vec3_len2(n_OAC));
        rat_print(cos2);
        printf("  (90 deg)\n");
        check("coordinate planes are perpendicular", rat_eq(cos2, rat_make(0, 1)));

        /* OAB (z=0) vs ABC (x+y+z=1): along edge AB */
        dot_val = vec3_dot(n_OAB, n_ABC);
        len1_sq = vec3_len2(n_OAB);
        len2_sq = vec3_len2(n_ABC);
        cos2 = rat_make(dot_val * dot_val, len1_sq * len2_sq);
        printf("  OAB vs ABC: cos^2 = %ld^2/(%ld*%ld) = ",
               dot_val, len1_sq, len2_sq);
        rat_print(cos2);
        printf("  (arccos(1/sqrt(3)) ~ 54.74 deg)\n");
        check("OAB vs ABC: cos^2 = 1/3", rat_eq(cos2, rat_make(1, 3)));

        /* OAC (y=0) vs ABC: along edge AC */
        dot_val = vec3_dot(n_OAC, n_ABC);
        cos2 = rat_make(dot_val * dot_val,
                        vec3_len2(n_OAC) * vec3_len2(n_ABC));
        printf("  OAC vs ABC: cos^2 = ");
        rat_print(cos2);
        printf("  (same angle by symmetry)\n");
        check("OAC vs ABC: cos^2 = 1/3", rat_eq(cos2, rat_make(1, 3)));

        /* OBC (x=0) vs ABC: along edge BC */
        dot_val = vec3_dot(n_OBC, n_ABC);
        cos2 = rat_make(dot_val * dot_val,
                        vec3_len2(n_OBC) * vec3_len2(n_ABC));
        printf("  OBC vs ABC: cos^2 = ");
        rat_print(cos2);
        printf("  (same angle by symmetry)\n");
        check("OBC vs ABC: cos^2 = 1/3", rat_eq(cos2, rat_make(1, 3)));
    }

    /* ---- Part 5: Distances ---- */
    printf("\n--- Part 5: Distances (as squared values) ---\n");
    {
        RVec3 foot;
        Rat dist_sq;

        /* O to plane x+y+z=1 */
        foot = foot_point_to_plane(O, vec3_make(1, 1, 1), 1);
        dist_sq = rvec3_len2(rvec3_sub(rvec3_from_int(O), foot));
        printf("  dist^2(O, x+y+z=1) = ");
        rat_print(dist_sq);
        printf("  (dist = 1/sqrt(3))\n");
        check("dist^2(O, x+y+z=1) = 1/3", rat_eq(dist_sq, rat_make(1, 3)));

        /* C to L(O,A) = x-axis */
        foot = foot_point_to_line(C, O, vec3_make(1, 0, 0));
        dist_sq = rvec3_len2(rvec3_sub(rvec3_from_int(C), foot));
        printf("  dist^2(C, x-axis) = ");
        rat_print(dist_sq);
        printf("\n");
        check("dist^2(C, x-axis) = 1", rat_eq(dist_sq, rat_make(1, 1)));

        /* C to L(A,B) */
        foot = foot_point_to_line(C, A, vec3_sub(B, A));
        dist_sq = rvec3_len2(rvec3_sub(rvec3_from_int(C), foot));
        printf("  dist^2(C, L(A,B)) = ");
        rat_print(dist_sq);
        printf("  (foot at ");
        rvec3_print(foot);
        printf(")\n");
        /* d = (-1,1,0), C-A = (-1,0,1), (C-A).d = 1, d.d = 2, t = 1/2
         * foot = A + (1/2)(-1,1,0) = (1/2, 1/2, 0)
         * dist^2 = (0-1/2)^2 + (0-1/2)^2 + 1^2 = 1/4+1/4+1 = 3/2 */
        check("dist^2(C, L(A,B)) = 3/2", rat_eq(dist_sq, rat_make(3, 2)));
    }

    /* ---- Part 6: Symmetry observations ---- */
    printf("\n--- Part 6: Symmetry of the right tetrahedron ---\n");
    printf("  All 3 skew edge pairs have:\n");
    printf("    - One foot at the ORIGIN (the right-angle vertex)\n");
    printf("    - The other foot at the MIDPOINT of the opposite edge\n");
    printf("    - Squared distance = 1/2 (distance = 1/sqrt(2))\n");
    printf("  This is a consequence of the 3-fold symmetry\n");
    printf("  of the right tetrahedron under permutation of axes.\n\n");
    printf("  Dihedral angles:\n");
    printf("    - 3 edges at O: 90 degrees (right angles)\n");
    printf("    - 3 edges of face ABC: arccos(1/sqrt(3)) ~ 54.74 deg\n");
    printf("  The angle arccos(1/sqrt(3)) is the 'magic angle' from NMR.\n");

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("All perpendicular feet are RATIONAL (no square roots).\n");
    printf("Squared distances are RATIONAL.\n");
    printf("Actual distances involve sqrt (the Euclidean metric).\n");
    printf("Dihedral angles: cos^2 is rational, cos may be irrational.\n");
    printf("Key insight: the FEET of perpendiculars are constructible\n");
    printf("  points, even when the DISTANCES are irrational.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
