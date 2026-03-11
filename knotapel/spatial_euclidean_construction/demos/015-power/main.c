/*
 * Demo 015: Power of a Point
 *
 * The power of point P w.r.t. sphere S(C, r):
 *
 *   pow(P) = |P - C|^2 - r^2
 *
 * Sign classification:
 *   pow = 0  -> P on sphere
 *   pow < 0  -> P inside
 *   pow > 0  -> P outside
 *
 * Key results:
 *   - Radical plane of two spheres = locus where powers are equal
 *   - Radical plane of unit sphere and circumsphere = face ABC (x+y+z=1)
 *   - Connection to inversion: pow(P) = -|P|^2 * pow(Inv(P))
 *   - Radical plane of unit sphere and S(A,O) = x = 1/2
 *
 * Key constructions used:
 *   - Spheres (Book III: radical planes)
 *   - Inversion (Book XIII)
 *   - Circumsphere (Book IX)
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

static Rat rat_neg(Rat a)
{
    return rat_make(-a.p, a.q);
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

static void rvec3_print(RVec3 v)
{
    printf("("); rat_print(v.x);
    printf(", "); rat_print(v.y);
    printf(", "); rat_print(v.z);
    printf(")");
}

static RVec3 rvec3_sub(RVec3 a, RVec3 b)
{
    return rvec3_make(rat_sub(a.x, b.x), rat_sub(a.y, b.y),
                      rat_sub(a.z, b.z));
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

/* ---- power of a point ---- */

/*
 * Power of point P w.r.t. sphere with center C, radius^2 = r2.
 * pow(P) = |P - C|^2 - r2
 */
static Rat power(RVec3 p, RVec3 center, Rat r2)
{
    return rat_sub(rvec3_len2(rvec3_sub(p, center)), r2);
}

/* Inversion in unit sphere: Inv(P) = P / |P|^2 */
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
    RVec3 O, A, B, C, G, K;
    Rat zero, one;
    Rat unit_r2, circ_r2;

    O = rvec3_from_ints(0, 0, 0);
    A = rvec3_from_ints(1, 0, 0);
    B = rvec3_from_ints(0, 1, 0);
    C = rvec3_from_ints(0, 0, 1);
    G = rvec3_make(rat_make(1, 4), rat_make(1, 4), rat_make(1, 4));
    K = rvec3_make(rat_make(1, 2), rat_make(1, 2), rat_make(1, 2));

    zero = rat_make(0, 1);
    one = rat_make(1, 1);
    unit_r2 = one;
    circ_r2 = rat_make(3, 4);

    (void)B; (void)C;

    printf("=== Demo 015: Power of a Point ===\n\n");
    printf("pow(P, S(C,r)) = |P - C|^2 - r^2\n\n");

    /* ---- Part 1: Power w.r.t. unit sphere ---- */
    printf("--- Part 1: Power w.r.t. unit sphere (center O, r^2=1) ---\n");
    {
        RVec3 M_OA, p111;
        Rat pw;

        printf("  pow = 0: on sphere. pow < 0: inside. pow > 0: outside.\n\n");

        pw = power(A, O, unit_r2);
        printf("  pow(A = "); rvec3_print(A);
        printf(") = "); rat_print(pw); printf("\n");
        check("pow(A) = 0 (on unit sphere)", rat_eq(pw, zero));

        pw = power(G, O, unit_r2);
        printf("  pow(G) = "); rat_print(pw); printf("\n");
        check("pow(G) = -13/16 (inside)", rat_eq(pw, rat_make(-13, 16)));

        pw = power(K, O, unit_r2);
        printf("  pow(K) = "); rat_print(pw); printf("\n");
        check("pow(K) = -1/4 (inside)", rat_eq(pw, rat_make(-1, 4)));

        p111 = rvec3_from_ints(1, 1, 1);
        pw = power(p111, O, unit_r2);
        printf("  pow(1,1,1) = "); rat_print(pw); printf("\n");
        check("pow(1,1,1) = 2 (outside)", rat_eq(pw, rat_make(2, 1)));

        M_OA = rvec3_make(rat_make(1, 2), zero, zero);
        pw = power(M_OA, O, unit_r2);
        printf("  pow(M_OA) = "); rat_print(pw); printf("\n");
        check("pow(M_OA) = -3/4 (inside)", rat_eq(pw, rat_make(-3, 4)));
    }

    /* ---- Part 2: Power w.r.t. circumsphere ---- */
    printf("\n--- Part 2: Power w.r.t. circumsphere (center K, r^2=3/4) ---\n");
    {
        RVec3 p111, p200;
        Rat pw;

        pw = power(O, K, circ_r2);
        printf("  pow(O) = "); rat_print(pw); printf("\n");
        check("pow(O) = 0 (on circumsphere)", rat_eq(pw, zero));

        pw = power(A, K, circ_r2);
        printf("  pow(A) = "); rat_print(pw); printf("\n");
        check("pow(A) = 0 (on circumsphere)", rat_eq(pw, zero));

        pw = power(G, K, circ_r2);
        printf("  pow(G) = "); rat_print(pw); printf("\n");
        check("pow(G) = -9/16 (inside)", rat_eq(pw, rat_make(-9, 16)));

        p111 = rvec3_from_ints(1, 1, 1);
        pw = power(p111, K, circ_r2);
        printf("  pow(1,1,1) = "); rat_print(pw); printf("\n");
        check("pow(1,1,1) = 0 (on circumsphere!)",
              rat_eq(pw, zero));

        p200 = rvec3_from_ints(2, 0, 0);
        pw = power(p200, K, circ_r2);
        printf("  pow(2,0,0) = "); rat_print(pw); printf("\n");
        check("pow(2,0,0) = 2 (outside)", rat_eq(pw, rat_make(2, 1)));

        printf("\n  (1,1,1) on circumsphere: the circumsphere of {O,A,B,C}\n");
        printf("  is also the circumsphere of the unit cube [0,1]^3.\n");
    }

    /* ---- Part 3: Radical plane = face ABC ---- */
    printf("\n--- Part 3: Radical plane of unit sphere & circumsphere ---\n");
    {
        RVec3 cABC, midAB;
        Rat pw_unit, pw_circ;

        printf("  pow_unit(P) = pow_circum(P)  iff  x + y + z = 1.\n");
        printf("  The radical plane is the face ABC.\n\n");

        /* A: on both spheres, both powers = 0 */
        pw_unit = power(A, O, unit_r2);
        pw_circ = power(A, K, circ_r2);
        printf("  A: pow_unit = "); rat_print(pw_unit);
        printf(", pow_circum = "); rat_print(pw_circ); printf("\n");
        check("A: both powers = 0 (on radical plane)",
              rat_eq(pw_unit, pw_circ));

        /* centroid(ABC) = (1/3,1/3,1/3): on plane x+y+z=1 */
        cABC = rvec3_make(rat_make(1, 3), rat_make(1, 3), rat_make(1, 3));
        pw_unit = power(cABC, O, unit_r2);
        pw_circ = power(cABC, K, circ_r2);
        printf("  centroid(ABC): pow_unit = "); rat_print(pw_unit);
        printf(", pow_circum = "); rat_print(pw_circ); printf("\n");
        check("centroid(ABC): both = -2/3 (on radical plane)",
              rat_eq(pw_unit, pw_circ) &&
              rat_eq(pw_unit, rat_make(-2, 3)));

        /* midpoint(A,B) = (1/2,1/2,0): on plane x+y+z=1 */
        midAB = rvec3_make(rat_make(1, 2), rat_make(1, 2), zero);
        pw_unit = power(midAB, O, unit_r2);
        pw_circ = power(midAB, K, circ_r2);
        printf("  midpoint(A,B): pow_unit = "); rat_print(pw_unit);
        printf(", pow_circum = "); rat_print(pw_circ); printf("\n");
        check("midpoint(A,B): both = -1/2 (on radical plane)",
              rat_eq(pw_unit, pw_circ) &&
              rat_eq(pw_unit, rat_make(-1, 2)));

        /* G NOT on radical plane: powers differ */
        pw_unit = power(G, O, unit_r2);
        pw_circ = power(G, K, circ_r2);
        printf("  G: pow_unit = "); rat_print(pw_unit);
        printf(", pow_circum = "); rat_print(pw_circ);
        printf("  (NOT equal)\n");
        check("G not on radical plane: -13/16 != -9/16",
              !rat_eq(pw_unit, pw_circ));
    }

    /* ---- Part 4: Power and inversion ---- */
    printf("\n--- Part 4: pow(P) = -|P|^2 * pow(Inv(P)) ---\n");
    {
        RVec3 pts[3];
        const char *names[3];
        int i;

        printf("  Inversion flips the sign and scales by |P|^2.\n\n");

        pts[0] = G;  names[0] = "G";
        pts[1] = K;  names[1] = "K";
        pts[2] = rvec3_from_ints(1, 1, 1);  names[2] = "(1,1,1)";

        for (i = 0; i < 3; i++) {
            Rat pw_P = power(pts[i], O, unit_r2);
            Rat pw_inv = power(invert(pts[i]), O, unit_r2);
            Rat len2 = rvec3_len2(pts[i]);
            Rat rhs = rat_mul(rat_neg(len2), pw_inv);
            char desc[80];

            printf("  %s: pow = ", names[i]); rat_print(pw_P);
            printf(", -|P|^2 * pow(Inv) = "); rat_print(rhs);
            printf("\n");
            sprintf(desc, "%s: pow(P) = -|P|^2 * pow(Inv(P))", names[i]);
            check(desc, rat_eq(pw_P, rhs));
        }

        printf("\n  This identity means inversion flips inside/outside\n");
        printf("  relative to the unit sphere, scaled by |P|^2.\n");
    }

    /* ---- Part 5: Radical plane of unit sphere & S(A,O) ---- */
    printf("\n--- Part 5: Radical plane of unit sphere & S(A,O) ---\n");
    {
        RVec3 M_OA, p_half_1_0;
        Rat pw_unit, pw_SA;
        Rat sa_r2 = one;

        printf("  S(A,O): center A = (1,0,0), r^2 = 1.\n");
        printf("  Radical plane: x = 1/2 (perpendicular bisector of OA).\n\n");

        /* M_OA = (1/2, 0, 0): on plane x=1/2 */
        M_OA = rvec3_make(rat_make(1, 2), zero, zero);
        pw_unit = power(M_OA, O, unit_r2);
        pw_SA = power(M_OA, A, sa_r2);
        printf("  M_OA: pow_unit = "); rat_print(pw_unit);
        printf(", pow_SA = "); rat_print(pw_SA); printf("\n");
        check("M_OA: both = -3/4 (on radical plane x=1/2)",
              rat_eq(pw_unit, pw_SA) &&
              rat_eq(pw_unit, rat_make(-3, 4)));

        /* (1/2, 1, 0): on plane x=1/2 */
        p_half_1_0 = rvec3_make(rat_make(1, 2), one, zero);
        pw_unit = power(p_half_1_0, O, unit_r2);
        pw_SA = power(p_half_1_0, A, sa_r2);
        printf("  (1/2,1,0): pow_unit = "); rat_print(pw_unit);
        printf(", pow_SA = "); rat_print(pw_SA); printf("\n");
        check("(1/2,1,0): both = 1/4 (on radical plane x=1/2)",
              rat_eq(pw_unit, pw_SA) &&
              rat_eq(pw_unit, rat_make(1, 4)));

        /* A = (1,0,0): NOT on plane x=1/2, powers differ */
        pw_unit = power(A, O, unit_r2);
        pw_SA = power(A, A, sa_r2);
        printf("  A: pow_unit = "); rat_print(pw_unit);
        printf(", pow_SA = "); rat_print(pw_SA);
        printf("  (NOT equal)\n");
        check("A not on radical plane: 0 != -1",
              !rat_eq(pw_unit, pw_SA));
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("Power of a point unifies the sphere theory:\n\n");
    printf("  Book III:  radical plane = locus of equal power\n");
    printf("  Book IX:   circumsphere = zero-power locus of all vertices\n");
    printf("  Book XIII: inversion flips power (pow = -|P|^2 * pow(Inv))\n\n");
    printf("The radical plane of unit sphere and circumsphere is\n");
    printf("the face plane x+y+z=1. This is WHY the face plane\n");
    printf("kept appearing in the inversion and stereographic\n");
    printf("projection theorems: it is the natural boundary where\n");
    printf("the two spheres have equal influence.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
