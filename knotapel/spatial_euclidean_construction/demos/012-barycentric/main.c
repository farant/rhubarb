/*
 * Demo 012: Barycentric Coordinates
 *
 * Every point in space can be expressed as a weighted combination of
 * the four vertices {O, A, B, C}:
 *
 *   P = l0*O + l1*A + l2*B + l3*C,   l0 + l1 + l2 + l3 = 1
 *
 * For our right tetrahedron (O at origin, A,B,C on axes), the
 * conversion is simply:
 *
 *   l1 = x,  l2 = y,  l3 = z,  l0 = 1 - x - y - z
 *
 * Key results:
 *   - Inside iff all four weights are positive
 *   - Face distances are encoded: dist^2(P, face_j) = l_j^2 * h_j^2
 *   - Sub-tetrahedron volumes = |l_j| * total volume
 *   - Affine operations (midpoints, centroids) = averaging weights
 *   - Circumcenter K has l0 = -1/2: it is OUTSIDE the tetrahedron
 *
 * Key constructions used:
 *   - Centroid (Book IV), circumcenter (Book IX)
 *   - Face distances (Book V), volumes (Book VIII)
 *   - S3 symmetry (Book VI) explains equal weights for G
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long p, q; } Rat;
typedef struct { Rat x, y, z; } RVec3;
typedef struct { Rat l0, l1, l2, l3; } Bary4;

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

static RVec3 rvec3_sub(RVec3 a, RVec3 b)
{
    return rvec3_make(rat_sub(a.x, b.x), rat_sub(a.y, b.y), rat_sub(a.z, b.z));
}

static Rat rvec3_dot(RVec3 a, RVec3 b)
{
    return rat_add(rat_add(rat_mul(a.x, b.x), rat_mul(a.y, b.y)),
                   rat_mul(a.z, b.z));
}

/* Determinant of 3x3 matrix with rows a, b, c */
static Rat rvec3_det(RVec3 a, RVec3 b, RVec3 c)
{
    Rat t1 = rat_mul(a.x, rat_sub(rat_mul(b.y, c.z), rat_mul(b.z, c.y)));
    Rat t2 = rat_mul(a.y, rat_sub(rat_mul(b.x, c.z), rat_mul(b.z, c.x)));
    Rat t3 = rat_mul(a.z, rat_sub(rat_mul(b.x, c.y), rat_mul(b.y, c.x)));
    return rat_add(rat_sub(t1, t2), t3);
}

/* ---- barycentric coordinate ops ---- */

static Bary4 bary_make(Rat l0, Rat l1, Rat l2, Rat l3)
{
    Bary4 b;
    b.l0 = l0; b.l1 = l1; b.l2 = l2; b.l3 = l3;
    return b;
}

/*
 * Cartesian -> Barycentric for right tetrahedron {O,A,B,C}.
 * l1 = x, l2 = y, l3 = z, l0 = 1 - x - y - z.
 */
static Bary4 cart_to_bary(RVec3 p)
{
    Rat sum = rat_add(rat_add(p.x, p.y), p.z);
    return bary_make(rat_sub(rat_make(1, 1), sum), p.x, p.y, p.z);
}

/* Barycentric -> Cartesian: P = (l1, l2, l3) since O is origin */
static RVec3 bary_to_cart(Bary4 b)
{
    return rvec3_make(b.l1, b.l2, b.l3);
}

static int bary_eq(Bary4 a, Bary4 b)
{
    return rat_eq(a.l0, b.l0) && rat_eq(a.l1, b.l1) &&
           rat_eq(a.l2, b.l2) && rat_eq(a.l3, b.l3);
}

static void bary_print(Bary4 b)
{
    printf("["); rat_print(b.l0);
    printf(", "); rat_print(b.l1);
    printf(", "); rat_print(b.l2);
    printf(", "); rat_print(b.l3);
    printf("]");
}

static Rat bary_sum(Bary4 b)
{
    return rat_add(rat_add(b.l0, b.l1), rat_add(b.l2, b.l3));
}

/* Midpoint = componentwise average of barycentric coords */
static Bary4 bary_avg2(Bary4 a, Bary4 b)
{
    Rat h = rat_make(1, 2);
    return bary_make(
        rat_mul(rat_add(a.l0, b.l0), h),
        rat_mul(rat_add(a.l1, b.l1), h),
        rat_mul(rat_add(a.l2, b.l2), h),
        rat_mul(rat_add(a.l3, b.l3), h));
}

/* Face centroid = average of 3 barycentric coords */
static Bary4 bary_avg3(Bary4 a, Bary4 b, Bary4 c)
{
    Rat t = rat_make(1, 3);
    return bary_make(
        rat_mul(rat_add(rat_add(a.l0, b.l0), c.l0), t),
        rat_mul(rat_add(rat_add(a.l1, b.l1), c.l1), t),
        rat_mul(rat_add(rat_add(a.l2, b.l2), c.l2), t),
        rat_mul(rat_add(rat_add(a.l3, b.l3), c.l3), t));
}

/* Tetrahedron centroid = average of 4 barycentric coords */
static Bary4 bary_avg4(Bary4 a, Bary4 b, Bary4 c, Bary4 d)
{
    Rat q = rat_make(1, 4);
    return bary_make(
        rat_mul(rat_add(rat_add(a.l0, b.l0), rat_add(c.l0, d.l0)), q),
        rat_mul(rat_add(rat_add(a.l1, b.l1), rat_add(c.l1, d.l1)), q),
        rat_mul(rat_add(rat_add(a.l2, b.l2), rat_add(c.l2, d.l2)), q),
        rat_mul(rat_add(rat_add(a.l3, b.l3), rat_add(c.l3, d.l3)), q));
}

/* ---- geometry helpers ---- */

/* Squared distance from P to plane n.X = d */
static Rat dist2_to_plane(RVec3 p, RVec3 normal, Rat d)
{
    Rat nP = rvec3_dot(normal, p);
    Rat nn = rvec3_dot(normal, normal);
    Rat diff = rat_sub(nP, d);
    return rat_div(rat_mul(diff, diff), nn);
}

/* Signed volume of tetrahedron PQRS = det(Q-P, R-P, S-P) / 6 */
static Rat tet_signed_vol(RVec3 P, RVec3 Q, RVec3 R, RVec3 S)
{
    Rat det = rvec3_det(rvec3_sub(Q, P), rvec3_sub(R, P), rvec3_sub(S, P));
    return rat_mul(det, rat_make(1, 6));
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
    Bary4 bO, bA, bB, bC;
    Rat zero, one;

    O = rvec3_from_ints(0, 0, 0);
    A = rvec3_from_ints(1, 0, 0);
    B = rvec3_from_ints(0, 1, 0);
    C = rvec3_from_ints(0, 0, 1);
    G = rvec3_make(rat_make(1, 4), rat_make(1, 4), rat_make(1, 4));
    K = rvec3_make(rat_make(1, 2), rat_make(1, 2), rat_make(1, 2));

    zero = rat_make(0, 1);
    one = rat_make(1, 1);

    /* Vertex barycentric coords */
    bO = bary_make(one, zero, zero, zero);
    bA = bary_make(zero, one, zero, zero);
    bB = bary_make(zero, zero, one, zero);
    bC = bary_make(zero, zero, zero, one);

    printf("=== Demo 012: Barycentric Coordinates ===\n\n");

    /* ---- Part 1: Cartesian <-> Barycentric conversion ---- */
    printf("--- Part 1: Coordinate conversion ---\n");
    {
        Bary4 b;

        printf("  For right tet {O,A,B,C}: l1=x, l2=y, l3=z, l0=1-x-y-z\n\n");

        b = cart_to_bary(O);
        printf("  O = "); rvec3_print(O);
        printf(" -> "); bary_print(b); printf("\n");
        check("O -> bary (1, 0, 0, 0)", bary_eq(b, bO));

        b = cart_to_bary(A);
        printf("  A = "); rvec3_print(A);
        printf(" -> "); bary_print(b); printf("\n");
        check("A -> bary (0, 1, 0, 0)", bary_eq(b, bA));

        b = cart_to_bary(G);
        printf("  G = "); rvec3_print(G);
        printf(" -> "); bary_print(b); printf("\n");
        check("G -> bary (1/4, 1/4, 1/4, 1/4) with sum = 1",
              bary_eq(b, bary_make(rat_make(1, 4), rat_make(1, 4),
                                   rat_make(1, 4), rat_make(1, 4))) &&
              rat_eq(bary_sum(b), one));

        /* Round-trip: K -> bary -> cart -> verify = K */
        {
            RVec3 roundtrip = bary_to_cart(cart_to_bary(K));
            printf("  K round-trip: "); rvec3_print(K);
            printf(" -> bary -> "); rvec3_print(roundtrip); printf("\n");
            check("K round-trip: cart -> bary -> cart = K",
                  rvec3_eq(roundtrip, K));
        }
    }

    /* ---- Part 2: Special points ---- */
    printf("\n--- Part 2: Special points in barycentric form ---\n");
    {
        Bary4 bK, bMidAB, bCentABC;

        bK = cart_to_bary(K);
        printf("  Circumcenter K = "); rvec3_print(K);
        printf(" -> "); bary_print(bK); printf("\n");
        check("K = (-1/2, 1/2, 1/2, 1/2)",
              bary_eq(bK, bary_make(rat_make(-1, 2), rat_make(1, 2),
                                    rat_make(1, 2), rat_make(1, 2))));

        bCentABC = cart_to_bary(rvec3_make(
            rat_make(1, 3), rat_make(1, 3), rat_make(1, 3)));
        printf("  centroid(ABC) -> "); bary_print(bCentABC); printf("\n");
        check("centroid(ABC) = (0, 1/3, 1/3, 1/3)",
              bary_eq(bCentABC, bary_make(zero, rat_make(1, 3),
                                          rat_make(1, 3), rat_make(1, 3))));

        bMidAB = cart_to_bary(rvec3_make(
            rat_make(1, 2), rat_make(1, 2), zero));
        printf("  midpoint(A,B) -> "); bary_print(bMidAB); printf("\n");
        check("midpoint(A,B) = (0, 1/2, 1/2, 0)",
              bary_eq(bMidAB, bary_make(zero, rat_make(1, 2),
                                        rat_make(1, 2), zero)));
    }

    /* ---- Part 3: Inside/outside classification ---- */
    printf("\n--- Part 3: Inside/outside classification ---\n");
    {
        Bary4 b;

        printf("  Inside: all l_j > 0.  Boundary: all l_j >= 0, some = 0.\n");
        printf("  Outside: any l_j < 0.\n\n");

        /* G: all positive -> inside */
        b = cart_to_bary(G);
        printf("  G "); bary_print(b);
        check("  G: INSIDE (all l > 0)",
              b.l0.p > 0 && b.l1.p > 0 && b.l2.p > 0 && b.l3.p > 0);

        /* K: l0 < 0 -> outside */
        b = cart_to_bary(K);
        printf("  K "); bary_print(b);
        check("  K: OUTSIDE (l0 = -1/2 < 0)", b.l0.p < 0);

        /* O: vertex (3 zeros) */
        b = cart_to_bary(O);
        printf("  O "); bary_print(b);
        check("  O: VERTEX (3 zeros)",
              b.l0.p > 0 && b.l1.p == 0 && b.l2.p == 0 && b.l3.p == 0);

        /* centroid(ABC): face (l0 = 0) */
        b = cart_to_bary(rvec3_make(
            rat_make(1, 3), rat_make(1, 3), rat_make(1, 3)));
        printf("  centroid(ABC) "); bary_print(b);
        check("  centroid(ABC): FACE (l0 = 0, rest > 0)",
              b.l0.p == 0 && b.l1.p > 0 && b.l2.p > 0 && b.l3.p > 0);

        /* (2,0,0): outside */
        b = cart_to_bary(rvec3_from_ints(2, 0, 0));
        printf("  (2,0,0) "); bary_print(b);
        check("  (2,0,0): OUTSIDE (l0 = -1 < 0)", b.l0.p < 0);

        printf("\n  Boundary sub-types: vertex (3 zeros), edge (2 zeros),\n");
        printf("  face (1 zero). The number of zeros = codimension.\n");
    }

    /* ---- Part 4: Affine combinations ---- */
    printf("\n--- Part 4: Affine combinations = averaging weights ---\n");
    {
        Bary4 mid_AB, cent_OAB, centroid_G;
        Bary4 expected;

        /* Midpoint of A and B */
        mid_AB = bary_avg2(bA, bB);
        expected = bary_make(zero, rat_make(1, 2), rat_make(1, 2), zero);
        printf("  avg(bary(A), bary(B)) = "); bary_print(mid_AB);
        printf("\n  = midpoint(A,B) at "); rvec3_print(bary_to_cart(mid_AB));
        printf("\n");
        check("avg2(A,B) = midpoint (1/2, 1/2, 0)", bary_eq(mid_AB, expected));

        /* Centroid of face OAB */
        cent_OAB = bary_avg3(bO, bA, bB);
        expected = bary_make(rat_make(1, 3), rat_make(1, 3),
                             rat_make(1, 3), zero);
        printf("  avg(bary(O), bary(A), bary(B)) = "); bary_print(cent_OAB);
        printf("\n  = centroid(OAB) at "); rvec3_print(bary_to_cart(cent_OAB));
        printf("\n");
        check("avg3(O,A,B) = centroid of face OAB",
              bary_eq(cent_OAB, expected));

        /* G = average of all 4 vertices */
        centroid_G = bary_avg4(bO, bA, bB, bC);
        printf("  avg(bary(O,A,B,C)) = "); bary_print(centroid_G);
        printf("\n  = G at "); rvec3_print(bary_to_cart(centroid_G));
        printf("\n");
        check("avg4(O,A,B,C) = centroid G = (1/4,1/4,1/4,1/4)",
              bary_eq(centroid_G, cart_to_bary(G)));

        printf("\n  Affine operations on barycentric coords are just\n");
        printf("  weighted averages. No coordinate conversion needed.\n");
    }

    /* ---- Part 5: Face distances from barycentric coords ---- */
    printf("\n--- Part 5: Face distances encoded in weights ---\n");
    {
        RVec3 nx = rvec3_from_ints(1, 0, 0);
        RVec3 n111 = rvec3_from_ints(1, 1, 1);
        Rat face_centroid_d;

        /* dist^2(G, face OBC) = x^2 = l1^2 = (1/4)^2 = 1/16 */
        {
            Rat d2 = dist2_to_plane(G, nx, zero);
            Rat l1_sq = rat_mul(rat_make(1, 4), rat_make(1, 4));
            printf("  dist^2(G, face OBC) = "); rat_print(d2);
            printf(" = l1^2 = "); rat_print(l1_sq); printf("\n");
            check("dist^2(G, OBC) = l1^2 = 1/16", rat_eq(d2, l1_sq));
        }

        /* dist^2(G, face ABC) = (1-x-y-z)^2/3 = l0^2/3 = 1/48 */
        {
            Rat d2 = dist2_to_plane(G, n111, one);
            Rat l0_sq_over_3 = rat_div(
                rat_mul(rat_make(1, 4), rat_make(1, 4)),
                rat_make(3, 1));
            printf("  dist^2(G, face ABC) = "); rat_print(d2);
            printf(" = l0^2/3 = "); rat_print(l0_sq_over_3); printf("\n");
            check("dist^2(G, ABC) = l0^2/3 = 1/48",
                  rat_eq(d2, l0_sq_over_3));
        }

        /* centroid(ABC) has l0 = 0, so dist to face ABC = 0 */
        {
            RVec3 cABC = rvec3_make(
                rat_make(1, 3), rat_make(1, 3), rat_make(1, 3));
            face_centroid_d = dist2_to_plane(cABC, n111, one);
            printf("  dist^2(centroid(ABC), face ABC) = ");
            rat_print(face_centroid_d); printf("\n");
            check("centroid(ABC) on face ABC: dist^2 = 0",
                  rat_eq(face_centroid_d, zero));
        }

        printf("\n  The barycentric weight l_j encodes the signed distance\n");
        printf("  from P to face j. The face equation IS the weight.\n");
    }

    /* ---- Part 6: Volume ratios ---- */
    printf("\n--- Part 6: Volume ratios from barycentric weights ---\n");
    {
        Rat vol_total, vol_G_ABC, vol_K_ABC;
        Rat sixth = rat_make(1, 6);

        /* Total volume */
        vol_total = tet_signed_vol(O, A, B, C);
        printf("  V(O,A,B,C) = "); rat_print(vol_total); printf("\n");
        check("V(O,A,B,C) = 1/6", rat_eq(vol_total, sixth));

        /* Sub-tet from G: V(G,A,B,C) = l0 * V_total = (1/4)(1/6) = 1/24 */
        vol_G_ABC = tet_signed_vol(G, A, B, C);
        printf("  V(G,A,B,C) = "); rat_print(vol_G_ABC); printf("\n");
        check("V(G,A,B,C) = 1/24 = (1/4) * V_total",
              rat_eq(vol_G_ABC, rat_make(1, 24)));

        /* Circumcenter is outside: signed volume is negative */
        vol_K_ABC = tet_signed_vol(K, A, B, C);
        printf("  V(K,A,B,C) = "); rat_print(vol_K_ABC);
        printf("  (NEGATIVE = K is past face ABC)\n");
        check("V(K,A,B,C) = -1/12 = (-1/2) * V_total",
              rat_eq(vol_K_ABC, rat_make(-1, 12)));

        /* Four sub-tets of G sum to total */
        {
            Rat v0 = tet_signed_vol(G, A, B, C);
            Rat v1 = tet_signed_vol(O, G, B, C);
            Rat v2 = tet_signed_vol(O, A, G, C);
            Rat v3 = tet_signed_vol(O, A, B, G);
            Rat sum = rat_add(rat_add(v0, v1), rat_add(v2, v3));

            printf("  Sub-volumes from G: ");
            rat_print(v0); printf(" + ");
            rat_print(v1); printf(" + ");
            rat_print(v2); printf(" + ");
            rat_print(v3); printf(" = ");
            rat_print(sum); printf("\n");
            check("4 sub-volumes sum to V_total = 1/6",
                  rat_eq(sum, sixth));
        }
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("Barycentric coordinates are the natural coordinate system\n");
    printf("for the tetrahedron. They encode:\n");
    printf("  - Position: P = l0*O + l1*A + l2*B + l3*C\n");
    printf("  - Containment: inside iff all l_j > 0\n");
    printf("  - Face distances: dist to face_j proportional to l_j\n");
    printf("  - Volume ratios: V(P,face_j) / V_total = |l_j|\n");
    printf("  - Affine operations: midpoints and centroids by averaging\n\n");
    printf("The circumcenter K = (-1/2, 1/2, 1/2, 1/2) has negative l0,\n");
    printf("revealing it lies outside the tetrahedron, past face ABC.\n");
    printf("The centroid G = (1/4, 1/4, 1/4, 1/4) has equal weights,\n");
    printf("the unique point equidistant (in the volume-ratio sense)\n");
    printf("from all four faces.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
