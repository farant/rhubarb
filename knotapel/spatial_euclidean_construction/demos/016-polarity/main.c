/*
 * Demo 016: Poles and Polars
 *
 * The polar plane of P w.r.t. the unit sphere:
 *
 *   polar(P) = { X : P . X = 1 }
 *
 * The pole of plane n.X = d:
 *
 *   pole = n / d
 *
 * Central theorem:
 *   Inv(P) = foot of perpendicular from O to polar(P).
 *   This gives a geometric definition of inversion through polarity.
 *
 * Key results:
 *   - Polar of (1,1,1) = face plane x+y+z=1
 *   - Polar of K = plane x+y+z=2
 *   - Polar of (2,0,0) = plane x=1/2 (radical plane from Book XV)
 *   - Points on the sphere are self-conjugate (tangent planes)
 *   - Reciprocity: P on polar(Q) iff Q on polar(P)
 *
 * Key constructions used:
 *   - Inversion (Book XIII)
 *   - Power of a point (Book XV)
 *   - Perpendicular foot (Book V)
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

static Rat rvec3_dot(RVec3 a, RVec3 b)
{
    return rat_add(rat_add(rat_mul(a.x, b.x), rat_mul(a.y, b.y)),
                   rat_mul(a.z, b.z));
}

static Rat rvec3_len2(RVec3 v)
{
    return rvec3_dot(v, v);
}

/* ---- polarity ---- */

/*
 * Check if Q lies on the polar plane of P (unit sphere):
 * P . Q = 1
 */
static int on_polar(RVec3 p, RVec3 q)
{
    return rat_eq(rvec3_dot(p, q), rat_make(1, 1));
}

/*
 * Pole of plane n.X = d w.r.t. unit sphere:
 * pole = n / d
 */
static RVec3 pole_of_plane(RVec3 n, Rat d)
{
    return rvec3_make(rat_div(n.x, d), rat_div(n.y, d), rat_div(n.z, d));
}

/*
 * Foot of perpendicular from origin to plane n.X = d:
 * F = (d / |n|^2) * n
 */
static RVec3 foot_from_origin(RVec3 n, Rat d)
{
    Rat s = rat_div(d, rvec3_len2(n));
    return rvec3_make(rat_mul(n.x, s), rat_mul(n.y, s), rat_mul(n.z, s));
}

/*
 * Inversion in unit sphere: Inv(P) = P / |P|^2
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
    RVec3 A, B, C, G, K;
    Rat one;

    A = rvec3_from_ints(1, 0, 0);
    B = rvec3_from_ints(0, 1, 0);
    C = rvec3_from_ints(0, 0, 1);
    G = rvec3_make(rat_make(1, 4), rat_make(1, 4), rat_make(1, 4));
    K = rvec3_make(rat_make(1, 2), rat_make(1, 2), rat_make(1, 2));
    one = rat_make(1, 1);

    printf("=== Demo 016: Poles and Polars ===\n\n");
    printf("Polar of P (unit sphere): { X : P . X = 1 }\n");
    printf("Pole of plane n.X = d:    P = n / d\n\n");

    /* ---- Part 1: Polar planes ---- */
    printf("--- Part 1: Polar planes of named points ---\n");
    {
        RVec3 p111, p200;

        printf("  Point on sphere -> polar = tangent plane.\n");
        printf("  Point inside    -> polar is far from O.\n");
        printf("  Point outside   -> polar cuts the sphere.\n\n");

        /* Vertices: self-conjugate (tangent planes) */
        printf("  polar(A) = {X : x = 1}  (tangent at A)\n");
        check("A on polar(A): A.A = 1", on_polar(A, A));

        printf("  polar(B) = {X : y = 1}  (tangent at B)\n");
        check("B on polar(B): B.B = 1", on_polar(B, B));

        /* (1,1,1): polar = face plane */
        p111 = rvec3_from_ints(1, 1, 1);
        printf("  polar(1,1,1) = {X : x+y+z = 1} = face ABC!\n");
        check("A on polar(1,1,1)", on_polar(p111, A));
        check("B on polar(1,1,1)", on_polar(p111, B));
        check("C on polar(1,1,1)", on_polar(p111, C));

        /* (2,0,0): polar = x=1/2 = radical plane from Book XV */
        p200 = rvec3_from_ints(2, 0, 0);
        {
            RVec3 M_OA = rvec3_make(rat_make(1, 2),
                                    rat_make(0, 1), rat_make(0, 1));
            printf("  polar(2,0,0) = {X : x = 1/2}\n");
            check("M_OA on polar(2,0,0)", on_polar(p200, M_OA));
        }
    }

    /* ---- Part 2: Poles of planes ---- */
    printf("\n--- Part 2: Poles of planes ---\n");
    {
        RVec3 n111, nx;
        RVec3 pole;

        printf("  Pole of n.X = d is P = n/d.\n\n");

        /* Face plane x+y+z=1 -> pole is (1,1,1) */
        n111 = rvec3_from_ints(1, 1, 1);
        pole = pole_of_plane(n111, one);
        printf("  pole(x+y+z=1) = "); rvec3_print(pole); printf("\n");
        check("pole(x+y+z=1) = (1,1,1)",
              rvec3_eq(pole, rvec3_from_ints(1, 1, 1)));

        /* Tangent plane x=1 -> pole is A */
        nx = rvec3_from_ints(1, 0, 0);
        pole = pole_of_plane(nx, one);
        printf("  pole(x=1) = "); rvec3_print(pole); printf("\n");
        check("pole(x=1) = A", rvec3_eq(pole, A));

        /* x+y+z=2 -> pole is K */
        pole = pole_of_plane(n111, rat_make(2, 1));
        printf("  pole(x+y+z=2) = "); rvec3_print(pole); printf("\n");
        check("pole(x+y+z=2) = K", rvec3_eq(pole, K));

        /* x+y+z=4 -> pole is G */
        pole = pole_of_plane(n111, rat_make(4, 1));
        printf("  pole(x+y+z=4) = "); rvec3_print(pole); printf("\n");
        check("pole(x+y+z=4) = G", rvec3_eq(pole, G));
    }

    /* ---- Part 3: Reciprocity ---- */
    printf("\n--- Part 3: Reciprocity ---\n");
    {
        RVec3 p111, p200;
        Rat dot;

        printf("  P on polar(Q) iff Q on polar(P).\n");
        printf("  (Because P.Q = Q.P by commutativity.)\n\n");

        p111 = rvec3_from_ints(1, 1, 1);
        check("A on polar(1,1,1) AND (1,1,1) on polar(A)",
              on_polar(p111, A) && on_polar(A, p111));

        p200 = rvec3_from_ints(2, 0, 0);
        check("K on polar(2,0,0) AND (2,0,0) on polar(K)",
              on_polar(p200, K) && on_polar(K, p200));

        /* Non-conjugate pair: G and (1,1,1) */
        dot = rvec3_dot(G, p111);
        printf("  G . (1,1,1) = "); rat_print(dot);
        printf(" != 1\n");
        check("G and (1,1,1) NOT conjugate (dot = 3/4)",
              !on_polar(G, p111) && rat_eq(dot, rat_make(3, 4)));
    }

    /* ---- Part 4: Inversion = foot of perpendicular to polar ---- */
    printf("\n--- Part 4: Inv(P) = foot of perp from O to polar(P) ---\n");
    {
        RVec3 pts[4];
        const char *names[4];
        int i;

        printf("  Polar of P has normal P and constant 1.\n");
        printf("  Foot from O to this plane = P/|P|^2 = Inv(P).\n\n");

        pts[0] = G;  names[0] = "G";
        pts[1] = K;  names[1] = "K";
        pts[2] = rvec3_from_ints(1, 1, 1);  names[2] = "(1,1,1)";
        pts[3] = rvec3_from_ints(2, 0, 0);  names[3] = "(2,0,0)";

        for (i = 0; i < 4; i++) {
            RVec3 inv = invert(pts[i]);
            RVec3 foot = foot_from_origin(pts[i], one);
            char desc[80];

            printf("  %s: Inv = ", names[i]); rvec3_print(inv);
            printf(", foot = "); rvec3_print(foot); printf("\n");

            sprintf(desc, "%s: Inv(P) = foot to polar(P)", names[i]);
            check(desc, rvec3_eq(inv, foot));
        }

        printf("\n  Inversion is the geometric operation: project the\n");
        printf("  center of the sphere perpendicularly onto the polar\n");
        printf("  plane of the point.\n");
    }

    /* ---- Part 5: Conjugate pairs ---- */
    printf("\n--- Part 5: Conjugate pairs (P.Q = 1) ---\n");
    {
        RVec3 p200, p400;

        printf("  P and Q are conjugate iff P.Q = 1.\n");
        printf("  Self-conjugate (P.P = 1) iff P is on the sphere.\n\n");

        /* Self-conjugate: A on sphere */
        check("A self-conjugate (A.A = 1, on sphere)", on_polar(A, A));

        /* Conjugate pair: K and (2,0,0) */
        p200 = rvec3_from_ints(2, 0, 0);
        printf("  K . (2,0,0) = "); rat_print(rvec3_dot(K, p200));
        printf("\n");
        check("K and (2,0,0) conjugate", on_polar(K, p200));

        /* Conjugate pair: G and (4,0,0) */
        p400 = rvec3_from_ints(4, 0, 0);
        printf("  G . (4,0,0) = "); rat_print(rvec3_dot(G, p400));
        printf("\n");
        check("G and (4,0,0) conjugate", on_polar(G, p400));
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("Polarity w.r.t. the unit sphere:\n");
    printf("  - Each point P has a polar plane { X : P.X = 1 }\n");
    printf("  - Each plane n.X = d has a pole P = n/d\n");
    printf("  - Reciprocity: P on polar(Q) iff Q on polar(P)\n\n");
    printf("The central theorem: Inv(P) = foot of perpendicular\n");
    printf("from O to polar(P). Inversion (Book XIII) IS the\n");
    printf("orthogonal projection onto the polar plane.\n\n");
    printf("Key connections:\n");
    printf("  polar(1,1,1) = face plane x+y+z=1  (Books XIII-XV)\n");
    printf("  polar(2,0,0) = plane x=1/2          (Book XV radical)\n");
    printf("  polar(A) = tangent at A              (A on sphere)\n");
    printf("  pole(face plane) = (1,1,1)           (on circumsphere)\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
