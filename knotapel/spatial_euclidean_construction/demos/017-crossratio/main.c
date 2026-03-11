/*
 * Demo 017: Cross-Ratio and Harmonic Division
 *
 * Cross-ratio of four collinear points (parameterized by t):
 *   (A,B;C,D) = ((tC - tA)(tD - tB)) / ((tC - tB)(tD - tA))
 *
 * Harmonic division: (A,B;C,D) = -1
 *
 * Central theorem:
 *   If X, Y are the intersections of a line through O with the unit sphere,
 *   then (X,Y; P, Inv(P)) = -1 for any P on that line.
 *   P and its inverse divide the chord harmonically.
 *
 * Key results:
 *   - Cross-ratio identity: (A,B;C,D) + (A,C;B,D) = 1
 *   - Harmonic division characterizes inversion
 *   - Polar plane meets line OP at the harmonic conjugate Inv(P)
 *   - Cross-ratio is preserved by inversion (lines through O)
 *
 * Key constructions used:
 *   - Inversion (Book XIII)
 *   - Poles and polars (Book XVI)
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

static Rat rvec3_dot(RVec3 a, RVec3 b)
{
    return rat_add(rat_add(rat_mul(a.x, b.x), rat_mul(a.y, b.y)),
                   rat_mul(a.z, b.z));
}

static Rat rvec3_len2(RVec3 v)
{
    return rvec3_dot(v, v);
}

/* ---- cross-ratio ---- */

/*
 * Cross-ratio of four values on a projective line:
 * (a,b;c,d) = ((c-a)(d-b)) / ((c-b)(d-a))
 */
static Rat cross_ratio(Rat a, Rat b, Rat c, Rat d)
{
    return rat_div(rat_mul(rat_sub(c, a), rat_sub(d, b)),
                   rat_mul(rat_sub(c, b), rat_sub(d, a)));
}

/* ---- inversion ---- */

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
    Rat neg1, one;

    neg1 = rat_make(-1, 1);
    one = rat_make(1, 1);

    printf("=== Demo 017: Cross-Ratio and Harmonic Division ===\n\n");
    printf("(A,B;C,D) = ((tC-tA)(tD-tB)) / ((tC-tB)(tD-tA))\n");
    printf("Harmonic division: cross-ratio = -1\n\n");

    /* ---- Part 1: Cross-ratio computation ---- */
    printf("--- Part 1: Cross-ratio computation ---\n");
    {
        Rat cr, cr2;

        printf("  Four collinear points yield one projective invariant.\n\n");

        /* Basic example: (0,1;2,3) = (2*2)/(1*3) = 4/3 */
        cr = cross_ratio(rat_make(0,1), rat_make(1,1),
                         rat_make(2,1), rat_make(3,1));
        printf("  (0,1;2,3) = "); rat_print(cr); printf("\n");
        check("(0,1;2,3) = 4/3", rat_eq(cr, rat_make(4, 3)));

        /* Swapping C,D gives reciprocal */
        cr = cross_ratio(rat_make(0,1), rat_make(1,1),
                         rat_make(3,1), rat_make(2,1));
        printf("  (0,1;3,2) = "); rat_print(cr); printf("\n");
        check("(0,1;3,2) = 3/4 (swap C,D -> reciprocal)",
              rat_eq(cr, rat_make(3, 4)));

        /* Harmonic example */
        cr = cross_ratio(rat_make(-1,1), rat_make(1,1),
                         rat_make(2,1), rat_make(1,2));
        printf("  (-1,1;2,1/2) = "); rat_print(cr); printf("\n");
        check("(-1,1;2,1/2) = -1 (harmonic)", rat_eq(cr, neg1));

        /* Cross-ratio identity: (A,B;C,D) + (A,C;B,D) = 1 */
        cr = cross_ratio(rat_make(0,1), rat_make(1,1),
                         rat_make(2,1), rat_make(3,1));
        cr2 = cross_ratio(rat_make(0,1), rat_make(2,1),
                          rat_make(1,1), rat_make(3,1));
        printf("  (0,1;2,3) + (0,2;1,3) = ");
        rat_print(cr); printf(" + "); rat_print(cr2);
        printf(" = "); rat_print(rat_add(cr, cr2)); printf("\n");
        check("Identity: (A,B;C,D) + (A,C;B,D) = 1",
              rat_eq(rat_add(cr, cr2), one));

        /* Harmonic permutation: three distinct values {-1, 2, 1/2} */
        cr = cross_ratio(rat_make(-1,1), rat_make(2,1),
                         rat_make(1,1), rat_make(1,2));
        printf("  (-1,2;1,1/2) = "); rat_print(cr); printf("\n");
        check("Harmonic permutation: (-1,2;1,1/2) = 2",
              rat_eq(cr, rat_make(2, 1)));
    }

    /* ---- Part 2: Harmonic division on x-axis ---- */
    printf("\n--- Part 2: P and Inv(P) divide sphere chord harmonically ---\n");
    {
        Rat xn, xp, cr;
        RVec3 P, invP;

        xn = rat_make(-1, 1);
        xp = rat_make(1, 1);

        printf("  X-axis meets unit sphere at X=(-1,0,0), Y=(1,0,0).\n");
        printf("  Theorem: (X,Y; P, Inv(P)) = -1 for any P on the line.\n\n");

        /* P = (2,0,0) */
        P = rvec3_from_ints(2, 0, 0);
        invP = invert(P);
        cr = cross_ratio(xn, xp, P.x, invP.x);
        printf("  P = "); rvec3_print(P);
        printf(", Inv = "); rvec3_print(invP);
        printf(": CR = "); rat_print(cr); printf("\n");
        check("P=(2,0,0): harmonic", rat_eq(cr, neg1));

        /* P = (3,0,0) */
        P = rvec3_from_ints(3, 0, 0);
        invP = invert(P);
        cr = cross_ratio(xn, xp, P.x, invP.x);
        printf("  P = "); rvec3_print(P);
        printf(", Inv = "); rvec3_print(invP);
        printf(": CR = "); rat_print(cr); printf("\n");
        check("P=(3,0,0): harmonic", rat_eq(cr, neg1));

        /* P = (1/2,0,0) - interior point */
        P = rvec3_make(rat_make(1,2), rat_make(0,1), rat_make(0,1));
        invP = invert(P);
        cr = cross_ratio(xn, xp, P.x, invP.x);
        printf("  P = "); rvec3_print(P);
        printf(", Inv = "); rvec3_print(invP);
        printf(": CR = "); rat_print(cr); printf("\n");
        check("P=(1/2,0,0): harmonic (interior)", rat_eq(cr, neg1));

        /* P = (-2,0,0) - negative side */
        P = rvec3_from_ints(-2, 0, 0);
        invP = invert(P);
        cr = cross_ratio(xn, xp, P.x, invP.x);
        printf("  P = "); rvec3_print(P);
        printf(", Inv = "); rvec3_print(invP);
        printf(": CR = "); rat_print(cr); printf("\n");
        check("P=(-2,0,0): harmonic (negative side)", rat_eq(cr, neg1));
    }

    /* ---- Part 3: Harmonic division on y-axis and z-axis ---- */
    printf("\n--- Part 3: Harmonic division on y-axis and z-axis ---\n");
    {
        Rat sn, sp, cr;
        RVec3 P, invP;

        sn = rat_make(-1, 1);
        sp = rat_make(1, 1);

        printf("  Every axis through O meets the sphere at -1 and +1.\n\n");

        /* y-axis: P = (0,3,0) */
        P = rvec3_from_ints(0, 3, 0);
        invP = invert(P);
        cr = cross_ratio(sn, sp, P.y, invP.y);
        printf("  Y: P = "); rvec3_print(P);
        printf(", Inv = "); rvec3_print(invP);
        printf(": CR = "); rat_print(cr); printf("\n");
        check("Y-axis P=(0,3,0): harmonic", rat_eq(cr, neg1));

        /* z-axis: P = (0,0,2) */
        P = rvec3_from_ints(0, 0, 2);
        invP = invert(P);
        cr = cross_ratio(sn, sp, P.z, invP.z);
        printf("  Z: P = "); rvec3_print(P);
        printf(", Inv = "); rvec3_print(invP);
        printf(": CR = "); rat_print(cr); printf("\n");
        check("Z-axis P=(0,0,2): harmonic", rat_eq(cr, neg1));

        /* z-axis: P = (0,0,4) */
        P = rvec3_from_ints(0, 0, 4);
        invP = invert(P);
        cr = cross_ratio(sn, sp, P.z, invP.z);
        printf("  Z: P = "); rvec3_print(P);
        printf(", Inv = "); rvec3_print(invP);
        printf(": CR = "); rat_print(cr); printf("\n");
        check("Z-axis P=(0,0,4): harmonic", rat_eq(cr, neg1));

        /* y-axis: P = (0,5,0) */
        P = rvec3_from_ints(0, 5, 0);
        invP = invert(P);
        cr = cross_ratio(sn, sp, P.y, invP.y);
        printf("  Y: P = "); rvec3_print(P);
        printf(", Inv = "); rvec3_print(invP);
        printf(": CR = "); rat_print(cr); printf("\n");
        check("Y-axis P=(0,5,0): harmonic", rat_eq(cr, neg1));
    }

    /* ---- Part 4: Polar plane meets line at harmonic conjugate ---- */
    printf("\n--- Part 4: Polar meets line OP at harmonic conjugate ---\n");
    {
        RVec3 P, invP, meet;

        printf("  Polar of P: { X : P.X = 1 }\n");
        printf("  Line OP meets polar at t = 1/|P|^2, giving Inv(P).\n");
        printf("  This is the harmonic conjugate (CR = -1).\n\n");

        /* P = (2,0,0): polar is 2x = 1 -> x = 1/2 */
        P = rvec3_from_ints(2, 0, 0);
        invP = invert(P);
        meet = rvec3_make(rat_make(1,2), rat_make(0,1), rat_make(0,1));
        printf("  P = "); rvec3_print(P);
        printf(": polar 2x=1, meets axis at "); rvec3_print(meet);
        printf("\n");
        check("polar(2,0,0) meets x-axis at Inv(P)",
              rvec3_eq(meet, invP));

        /* P = (3,0,0): polar is 3x = 1 -> x = 1/3 */
        P = rvec3_from_ints(3, 0, 0);
        invP = invert(P);
        meet = rvec3_make(rat_make(1,3), rat_make(0,1), rat_make(0,1));
        printf("  P = "); rvec3_print(P);
        printf(": polar 3x=1, meets axis at "); rvec3_print(meet);
        printf("\n");
        check("polar(3,0,0) meets x-axis at Inv(P)",
              rvec3_eq(meet, invP));

        /* P = (0,2,0): polar is 2y = 1 -> y = 1/2 */
        P = rvec3_from_ints(0, 2, 0);
        invP = invert(P);
        meet = rvec3_make(rat_make(0,1), rat_make(1,2), rat_make(0,1));
        printf("  P = "); rvec3_print(P);
        printf(": polar 2y=1, meets axis at "); rvec3_print(meet);
        printf("\n");
        check("polar(0,2,0) meets y-axis at Inv(P)",
              rvec3_eq(meet, invP));
    }

    /* ---- Part 5: Cross-ratio preserved by inversion ---- */
    printf("\n--- Part 5: Cross-ratio preserved by inversion ---\n");
    {
        Rat cr_before, cr_after;
        Rat t[4], s[4];
        RVec3 P, Q;
        int i;

        printf("  Inversion preserves cross-ratio on lines through O.\n\n");

        /* X-axis: {2, 3, 5, -1} */
        t[0] = rat_make(2, 1);
        t[1] = rat_make(3, 1);
        t[2] = rat_make(5, 1);
        t[3] = rat_make(-1, 1);

        for (i = 0; i < 4; i++) {
            P = rvec3_make(t[i], rat_make(0,1), rat_make(0,1));
            Q = invert(P);
            s[i] = Q.x;
        }

        cr_before = cross_ratio(t[0], t[1], t[2], t[3]);
        cr_after = cross_ratio(s[0], s[1], s[2], s[3]);
        printf("  X-axis {2,3,5,-1}: CR = ");
        rat_print(cr_before); printf("\n");
        printf("  Inv -> {");
        for (i = 0; i < 4; i++) {
            rat_print(s[i]);
            if (i < 3) printf(", ");
        }
        printf("}: CR = "); rat_print(cr_after); printf("\n");
        check("CR preserved: x-axis {2,3,5,-1}",
              rat_eq(cr_before, cr_after));

        /* Y-axis: {1/2, 2, 4, -3} */
        t[0] = rat_make(1, 2);
        t[1] = rat_make(2, 1);
        t[2] = rat_make(4, 1);
        t[3] = rat_make(-3, 1);

        for (i = 0; i < 4; i++) {
            P = rvec3_make(rat_make(0,1), t[i], rat_make(0,1));
            Q = invert(P);
            s[i] = Q.y;
        }

        cr_before = cross_ratio(t[0], t[1], t[2], t[3]);
        cr_after = cross_ratio(s[0], s[1], s[2], s[3]);
        printf("  Y-axis {1/2,2,4,-3}: CR = ");
        rat_print(cr_before); printf("\n");
        printf("  Inv: CR = "); rat_print(cr_after); printf("\n");
        check("CR preserved: y-axis {1/2,2,4,-3}",
              rat_eq(cr_before, cr_after));

        /* Z-axis: {1/3, 2, 3, -2} */
        t[0] = rat_make(1, 3);
        t[1] = rat_make(2, 1);
        t[2] = rat_make(3, 1);
        t[3] = rat_make(-2, 1);

        for (i = 0; i < 4; i++) {
            P = rvec3_make(rat_make(0,1), rat_make(0,1), t[i]);
            Q = invert(P);
            s[i] = Q.z;
        }

        cr_before = cross_ratio(t[0], t[1], t[2], t[3]);
        cr_after = cross_ratio(s[0], s[1], s[2], s[3]);
        printf("  Z-axis {1/3,2,3,-2}: CR = ");
        rat_print(cr_before); printf("\n");
        printf("  Inv: CR = "); rat_print(cr_after); printf("\n");
        check("CR preserved: z-axis {1/3,2,3,-2}",
              rat_eq(cr_before, cr_after));

        /* X-axis example 2: {1/4, 3, 5, -2} */
        t[0] = rat_make(1, 4);
        t[1] = rat_make(3, 1);
        t[2] = rat_make(5, 1);
        t[3] = rat_make(-2, 1);

        for (i = 0; i < 4; i++) {
            P = rvec3_make(t[i], rat_make(0,1), rat_make(0,1));
            Q = invert(P);
            s[i] = Q.x;
        }

        cr_before = cross_ratio(t[0], t[1], t[2], t[3]);
        cr_after = cross_ratio(s[0], s[1], s[2], s[3]);
        printf("  X-axis {1/4,3,5,-2}: CR = ");
        rat_print(cr_before); printf("\n");
        printf("  Inv: CR = "); rat_print(cr_after); printf("\n");
        check("CR preserved: x-axis {1/4,3,5,-2}",
              rat_eq(cr_before, cr_after));
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("Cross-ratio: the fundamental projective invariant.\n");
    printf("  (A,B;C,D) = ((tC-tA)(tD-tB)) / ((tC-tB)(tD-tA))\n\n");
    printf("Harmonic division (CR = -1) characterizes inversion:\n");
    printf("  P and Inv(P) divide every sphere chord harmonically.\n\n");
    printf("Key connections:\n");
    printf("  Polar of P meets line OP at Inv(P) = harmonic conjugate\n");
    printf("  Identity: (A,B;C,D) + (A,C;B,D) = 1\n");
    printf("  Inversion preserves cross-ratio on lines through O\n");
    printf("  Harmonic division is the signature of inversion (Book XIII)\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
