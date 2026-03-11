/*
 * Demo 002: Line-Sphere Intersection
 *
 * Verifies the quadratic formula for line-sphere intersection:
 *   Line: X = P + t*d,  d = Q - P
 *   Sphere: |X - C|^2 = r^2 = |W - C|^2
 *
 *   f = P - C
 *   a = d.d,  b_half = f.d,  c = f.f - r^2
 *   disc_quarter = b_half^2 - a*c
 *   t = (-b_half +/- sqrt(disc_quarter)) / a
 *
 * When disc_quarter is a perfect square, solutions are rational.
 * When not, solutions involve sqrt — first appearance of irrationals.
 *
 * Catalogs ALL new points reachable from {O, A, B, C} via
 * line-sphere intersection (lines = edges, spheres from point pairs).
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long x, y, z; } Vec3;
typedef struct { long p, q; } Rat;
typedef struct { Rat x, y, z; } RVec3;

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

static void rat_print(Rat r)
{
    if (r.q == 1)
        printf("%ld", r.p);
    else
        printf("%ld/%ld", r.p, r.q);
}

static int rat_eq(Rat a, Rat b)
{
    return a.p == b.p && a.q == b.q;
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

/* ---- perfect square test ---- */

static long isqrt_long(long n)
{
    long x, x1;
    if (n <= 0) return 0;
    x = n;
    x1 = (x + 1) / 2;
    while (x1 < x) {
        x = x1;
        x1 = (x + n / x) / 2;
    }
    return x;
}

static int is_perfect_square(long n, long *root)
{
    long s;
    if (n < 0) return 0;
    s = isqrt_long(n);
    if (s * s == n) {
        *root = s;
        return 1;
    }
    return 0;
}

/* ---- line-sphere intersection ---- */

typedef struct {
    int count;          /* 0, 1, or 2 intersections */
    long disc_quarter;  /* b_half^2 - a*c (exact integer) */
    int is_rational;    /* disc_quarter is a perfect square */
    RVec3 points[2];   /* valid only when is_rational && count > 0 */
} LineSphereResult;

/*
 * Line through P, Q.  Sphere centered at C passing through W.
 * All coordinates integer.
 */
static LineSphereResult line_sphere_intersect(
    Vec3 P, Vec3 Q,
    Vec3 C, Vec3 W)
{
    LineSphereResult res;
    Vec3 d = vec3_sub(Q, P);
    Vec3 f = vec3_sub(P, C);
    long r2 = vec3_len2(vec3_sub(W, C));

    long a = vec3_dot(d, d);        /* d.d */
    long b_half = vec3_dot(f, d);   /* f.d */
    long c = vec3_dot(f, f) - r2;   /* f.f - r^2 */

    long sqrt_disc;

    res.disc_quarter = b_half * b_half - a * c;
    res.is_rational = 0;
    res.count = 0;

    if (res.disc_quarter < 0) {
        res.count = 0;
        return res;
    }

    if (res.disc_quarter == 0) {
        res.count = 1;
    } else {
        res.count = 2;
    }

    /* check if discriminant is a perfect square */
    if (is_perfect_square(res.disc_quarter, &sqrt_disc)) {
        int i;
        long signs[2];
        signs[0] = -1;
        signs[1] = 1;
        res.is_rational = 1;

        for (i = 0; i < res.count; i++) {
            /* t = (-b_half + sign * sqrt_disc) / a */
            long t_num = -b_half + signs[i] * sqrt_disc;
            long t_den = a;

            /* X_i = P_i + (t_num/t_den) * d_i
             *      = (P_i * t_den + t_num * d_i) / t_den */
            res.points[i].x = rat_make(
                P.x * t_den + t_num * d.x, t_den);
            res.points[i].y = rat_make(
                P.y * t_den + t_num * d.y, t_den);
            res.points[i].z = rat_make(
                P.z * t_den + t_num * d.z, t_den);
        }

        /* for tangent case, duplicate the single point */
        if (res.count == 1) {
            res.points[1] = res.points[0];
        }
    }

    return res;
}

/* ---- test framework ---- */

static int tests_run = 0;
static int tests_passed = 0;

static void test_line_sphere(
    const char *desc,
    Vec3 P, Vec3 Q, Vec3 C, Vec3 W,
    int expect_count,
    int expect_rational,
    RVec3 *expect_p0,   /* NULL if no expected point */
    RVec3 *expect_p1)
{
    LineSphereResult res = line_sphere_intersect(P, Q, C, W);
    int ok = 1;

    tests_run++;

    if (res.count != expect_count) {
        printf("FAIL: %s — expected %d intersections, got %d (disc/4=%ld)\n",
               desc, expect_count, res.count, res.disc_quarter);
        return;
    }

    if (expect_count > 0 && res.is_rational != expect_rational) {
        printf("FAIL: %s — expected %s, disc/4=%ld\n",
               desc,
               expect_rational ? "rational" : "irrational",
               res.disc_quarter);
        return;
    }

    if (expect_rational && expect_p0 != NULL) {
        /* check points (order may vary, check both orderings) */
        if (expect_count == 2 && expect_p1 != NULL) {
            int match_direct = rvec3_eq(res.points[0], *expect_p0)
                            && rvec3_eq(res.points[1], *expect_p1);
            int match_swap   = rvec3_eq(res.points[0], *expect_p1)
                            && rvec3_eq(res.points[1], *expect_p0);
            if (!match_direct && !match_swap) {
                printf("FAIL: %s — points don't match\n", desc);
                printf("  expected: ");
                rvec3_print(*expect_p0);
                printf(" and ");
                rvec3_print(*expect_p1);
                printf("\n  got:      ");
                rvec3_print(res.points[0]);
                printf(" and ");
                rvec3_print(res.points[1]);
                printf("\n");
                ok = 0;
            }
        } else if (expect_count >= 1) {
            if (!rvec3_eq(res.points[0], *expect_p0)) {
                printf("FAIL: %s — point doesn't match\n", desc);
                printf("  expected: ");
                rvec3_print(*expect_p0);
                printf("\n  got:      ");
                rvec3_print(res.points[0]);
                printf("\n");
                ok = 0;
            }
        }
    }

    if (ok) {
        tests_passed++;
        printf("PASS: %s", desc);
        if (res.count > 0) {
            if (res.is_rational) {
                printf(" → ");
                rvec3_print(res.points[0]);
                if (res.count == 2) {
                    printf(", ");
                    rvec3_print(res.points[1]);
                }
            } else {
                printf(" [irrational, disc/4=%ld]", res.disc_quarter);
            }
        } else {
            printf(" [no intersection]");
        }
        printf("\n");
    }

    return;
}

/* ---- main ---- */

int main(void)
{
    /* Standard initial configuration */
    Vec3 O = {0, 0, 0};
    Vec3 A = {1, 0, 0};
    Vec3 B = {0, 1, 0};
    Vec3 C = {0, 0, 1};

    printf("=== Demo 002: Line-Sphere Intersection ===\n\n");

    /* ---- Axis lines vs unit spheres ---- */
    printf("--- L(O,A) = x-axis vs various spheres ---\n");
    {
        /* S(O,A): center O, passes through A. r=1.
         * Line (t,0,0): t^2 = 1 → t = ±1 → A=(1,0,0), (-1,0,0) */
        RVec3 pA = {{1,1},{0,1},{0,1}};
        RVec3 pNeg = {{-1,1},{0,1},{0,1}};
        test_line_sphere("L(O,A) ∩ S(O,A): t²=1",
            O, A, O, A, 2, 1, &pNeg, &pA);
    }
    {
        /* S(A,O): center A, passes through O. r=1.
         * Line (t,0,0): (t-1)^2 = 1 → t=0,2 → O, (2,0,0) */
        RVec3 pO = {{0,1},{0,1},{0,1}};
        RVec3 p2 = {{2,1},{0,1},{0,1}};
        test_line_sphere("L(O,A) ∩ S(A,O): (t-1)²=1",
            O, A, A, O, 2, 1, &pO, &p2);
    }
    {
        /* S(B,O): center B=(0,1,0), passes through O. r=1.
         * Line (t,0,0): t^2 + 1 = 1 → t^2 = 0. Tangent at O. */
        RVec3 pO = {{0,1},{0,1},{0,1}};
        test_line_sphere("L(O,A) ∩ S(B,O): t²+1=1, tangent",
            O, A, B, O, 1, 1, &pO, NULL);
    }
    {
        /* S(B,A): center B=(0,1,0), passes through A=(1,0,0). r²=2.
         * Line (t,0,0): t^2 + 1 = 2 → t=±1 → A, (-1,0,0) */
        RVec3 pA = {{1,1},{0,1},{0,1}};
        RVec3 pNeg = {{-1,1},{0,1},{0,1}};
        test_line_sphere("L(O,A) ∩ S(B,A): t²+1=2",
            O, A, B, A, 2, 1, &pNeg, &pA);
    }
    {
        /* S(A,B): center A=(1,0,0), passes through B=(0,1,0). r²=2.
         * Line (t,0,0): (t-1)² = 2 → t = 1±√2. IRRATIONAL! */
        test_line_sphere("L(O,A) ∩ S(A,B): (t-1)²=2, irrational",
            O, A, A, B, 2, 0, NULL, NULL);
    }
    {
        /* S(C,O): center C=(0,0,1), passes through O. r=1.
         * Line (t,0,0): t² + 1 = 1 → tangent at O */
        RVec3 pO = {{0,1},{0,1},{0,1}};
        test_line_sphere("L(O,A) ∩ S(C,O): tangent at O",
            O, A, C, O, 1, 1, &pO, NULL);
    }

    printf("\n--- L(A,B) vs various spheres ---\n");
    {
        /* S(O,A)=S(O,B): unit sphere at O.
         * L(A,B) = (1-t, t, 0): (1-t)²+t² = 1 → 2t²-2t = 0 → t=0,1
         * t=0: A, t=1: B. Already known. */
        RVec3 pA = {{1,1},{0,1},{0,1}};
        RVec3 pB = {{0,1},{1,1},{0,1}};
        test_line_sphere("L(A,B) ∩ S(O,A): A and B (known)",
            A, B, O, A, 2, 1, &pA, &pB);
    }
    {
        /* S(A,O): center A, r=1.
         * L(A,B): P=A, d=B-A=(-1,1,0). f=A-A=(0,0,0).
         * a=2, b_half=0, c=0-1=-1. disc/4=0+2=2. Not perfect square! */
        test_line_sphere("L(A,B) ∩ S(A,O): disc/4=2, irrational",
            A, B, A, O, 2, 0, NULL, NULL);
    }
    {
        /* S(A,B): center A, passes through B. r²=2.
         * f=A-A=(0,0,0). a=2, b_half=0, c=0-2=-2.
         * disc/4=0+4=4=2². Rational!
         * t = (0 ± 2)/2 = ±1. t=-1: A+(-1)(-1,1,0)=(2,-1,0). NEW!
         * t=1: A+1*(-1,1,0)=(0,1,0)=B. */
        RVec3 pB = {{0,1},{1,1},{0,1}};
        RVec3 pNew = {{2,1},{-1,1},{0,1}};
        test_line_sphere("L(A,B) ∩ S(A,B): B and (2,-1,0) NEW",
            A, B, A, B, 2, 1, &pB, &pNew);
    }
    {
        /* S(B,A): center B, passes through A. r²=2.
         * P=A, d=(-1,1,0), C=B=(0,1,0). f=A-B=(1,-1,0).
         * a=2, b_half=f.d=-1-1=-2, c=f.f-r²=2-2=0.
         * disc/4=4-0=4=2². t=(-(-2)±2)/2=(2±2)/2.
         * t=0: A. t=2: A+2*(-1,1,0)=(-1,2,0). NEW! */
        RVec3 pA = {{1,1},{0,1},{0,1}};
        RVec3 pNew = {{-1,1},{2,1},{0,1}};
        test_line_sphere("L(A,B) ∩ S(B,A): A and (-1,2,0) NEW",
            A, B, B, A, 2, 1, &pA, &pNew);
    }
    {
        /* S(C,O): center C=(0,0,1), passes through O. r=1.
         * L(A,B)=(1-t,t,0). f=A-C=(1,0,-1).
         * a=2, b_half=f.d=(1)(-1)+(0)(1)+(−1)(0)=-1
         * c=f.f-1=2-1=1. disc/4=1-2=-1. No intersection! */
        test_line_sphere("L(A,B) ∩ S(C,O): no intersection",
            A, B, C, O, 0, 0, NULL, NULL);
    }

    printf("\n--- L(B,C) vs selected spheres ---\n");
    {
        /* S(O,A): unit sphere at O.
         * L(B,C) = (0, 1-t, t): 0+(1-t)²+t² = 1 → 2t²-2t=0 → t=0,1
         * B and C. Known. */
        RVec3 pB = {{0,1},{1,1},{0,1}};
        RVec3 pC = {{0,1},{0,1},{1,1}};
        test_line_sphere("L(B,C) ∩ S(O,A): B and C (known)",
            B, C, O, A, 2, 1, &pB, &pC);
    }
    {
        /* S(B,C): center B, passes through C. r²=|C-B|²=2.
         * L(B,C): P=B, d=C-B=(0,-1,1). f=B-B=(0,0,0).
         * a=2, b_half=0, c=-2. disc/4=4=2².
         * t=(0±2)/2=±1. t=1: C. t=-1: B+(−1)(0,−1,1)=(0,2,−1). NEW! */
        RVec3 pC = {{0,1},{0,1},{1,1}};
        RVec3 pNew = {{0,1},{2,1},{-1,1}};
        test_line_sphere("L(B,C) ∩ S(B,C): C and (0,2,-1) NEW",
            B, C, B, C, 2, 1, &pC, &pNew);
    }

    printf("\n--- L(O,C) vs S(A,O): new point on z-axis ---\n");
    {
        /* S(A,O): center A=(1,0,0), passes through O. r=1.
         * L(O,C)=(0,0,t). f=O-A=(-1,0,0).
         * a=1, b_half=0, c=1-1=0. disc/4=0. Tangent!
         * t=0: O. */
        RVec3 pO = {{0,1},{0,1},{0,1}};
        test_line_sphere("L(O,C) ∩ S(A,O): tangent at O",
            O, C, A, O, 1, 1, &pO, NULL);
    }

    printf("\n--- L(O,C) vs S(A,C): irrational on z-axis ---\n");
    {
        /* S(A,C): center A=(1,0,0), passes through C=(0,0,1). r²=2.
         * L(O,C)=(0,0,t). f=O-A=(-1,0,0).
         * a=1, b_half=0, c=1-2=-1. disc/4=1.
         * t=(0±1)/1=±1. (0,0,1)=C, (0,0,-1). */
        RVec3 pC = {{0,1},{0,1},{1,1}};
        RVec3 pNew = {{0,1},{0,1},{-1,1}};
        test_line_sphere("L(O,C) ∩ S(A,C): C and (0,0,-1) NEW",
            O, C, A, C, 2, 1, &pC, &pNew);
    }

    /* ---- Summary of new rational points from initial config ---- */
    printf("\n=== NEW RATIONAL POINTS from {O,A,B,C} via line-sphere ===\n");
    printf("From axis lines (extending through origin):\n");
    printf("  (-1, 0, 0)  via L(O,A) ∩ S(O,A) or S(B,A)\n");
    printf("  ( 2, 0, 0)  via L(O,A) ∩ S(A,O)\n");
    printf("  ( 0,-1, 0)  via L(O,B) ∩ S(O,B) [by symmetry]\n");
    printf("  ( 0, 2, 0)  via L(O,B) ∩ S(B,O) [by symmetry]\n");
    printf("  ( 0, 0,-1)  via L(O,C) ∩ S(O,C) or S(A,C)\n");
    printf("  ( 0, 0, 2)  via L(O,C) ∩ S(C,O) [by symmetry]\n");
    printf("\nFrom non-axis edges (reflections through vertices):\n");
    printf("  ( 2,-1, 0)  via L(A,B) ∩ S(A,B)\n");
    printf("  (-1, 2, 0)  via L(A,B) ∩ S(B,A)\n");
    printf("  ( 2, 0,-1)  via L(A,C) ∩ S(A,C) [by symmetry]\n");
    printf("  (-1, 0, 2)  via L(A,C) ∩ S(C,A) [by symmetry]\n");
    printf("  ( 0, 2,-1)  via L(B,C) ∩ S(B,C)\n");
    printf("  ( 0,-1, 2)  via L(B,C) ∩ S(C,B) [by symmetry]\n");
    printf("\nTotal: 12 new rational points (+ 4 original = 16 points)\n");
    printf("All are INTEGER points — no fractions yet!\n");

    printf("\nIrrational cases encountered:\n");
    printf("  L(O,A) ∩ S(A,B): disc/4 = 2 (not a perfect square)\n");
    printf("  L(A,B) ∩ S(A,O): disc/4 = 2 (not a perfect square)\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);

    return tests_passed == tests_run ? 0 : 1;
}
