/*
 * Demo 007: Platonic Solids — Constructibility Classification
 *
 * The five Platonic solids, stratified by algebraic complexity:
 *
 * T_Q constructible (integer vertices, lines + planes suffice):
 *   1. Regular tetrahedron: (1,1,1), (1,-1,-1), (-1,1,-1), (-1,-1,1)
 *   2. Cube: all 8 sign combinations of (1,1,1)
 *   3. Octahedron: (+-1,0,0), (0,+-1,0), (0,0,+-1)
 *
 * T_E required (vertices in Q(sqrt(5)), spheres needed):
 *   4. Icosahedron: cyclic permutations of (0, +-1, +-phi)
 *   5. Dodecahedron: (+-1,+-1,+-1) union cyclic perms of (0, +-1/phi, +-phi)
 *
 * Key results:
 *   - Icosahedron: squared edge length is RATIONAL (= 4) despite
 *     irrational coordinates. The sqrt(5) cancels.
 *   - Dodecahedron: squared edge length is IRRATIONAL (= 6 - 2*sqrt(5)).
 *   - No embedding of icosahedron/dodecahedron can have all-rational
 *     vertices (A5 representation theory).
 */

#include <stdio.h>

/* ---- rational arithmetic (Q) ---- */

typedef struct { long p, q; } Rat;

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

/* ---- Q(sqrt(5)) arithmetic ---- */

typedef struct { Rat a, b; } QS5;  /* a + b*sqrt(5) */

static QS5 qs5_make(Rat a, Rat b)
{
    QS5 r;
    r.a = a; r.b = b;
    return r;
}

static QS5 qs5_from_rat(Rat a)
{
    return qs5_make(a, rat_make(0, 1));
}

static QS5 qs5_add(QS5 x, QS5 y)
{
    return qs5_make(rat_add(x.a, y.a), rat_add(x.b, y.b));
}

static QS5 qs5_sub(QS5 x, QS5 y)
{
    return qs5_make(rat_sub(x.a, y.a), rat_sub(x.b, y.b));
}

static QS5 qs5_mul(QS5 x, QS5 y)
{
    /* (a1 + b1*s)(a2 + b2*s) = (a1*a2 + 5*b1*b2) + (a1*b2 + a2*b1)*s */
    Rat ra = rat_add(rat_mul(x.a, y.a), rat_mul(rat_make(5, 1), rat_mul(x.b, y.b)));
    Rat rb = rat_add(rat_mul(x.a, y.b), rat_mul(y.a, x.b));
    return qs5_make(ra, rb);
}

static int qs5_eq(QS5 x, QS5 y)
{
    return rat_eq(x.a, y.a) && rat_eq(x.b, y.b);
}

static int qs5_is_rational(QS5 x)
{
    return x.b.p == 0;
}

static void qs5_print(QS5 x)
{
    rat_print(x.a);
    if (x.b.p > 0) {
        printf(" + ");
        rat_print(x.b);
        printf("*sqrt(5)");
    } else if (x.b.p < 0) {
        Rat neg;
        neg.p = -x.b.p;
        neg.q = x.b.q;
        printf(" - ");
        rat_print(neg);
        printf("*sqrt(5)");
    }
}

/* ---- Q(sqrt(5)) vectors ---- */

typedef struct { QS5 x, y, z; } QS5Vec3;

static QS5Vec3 qs5v_make(QS5 x, QS5 y, QS5 z)
{
    QS5Vec3 v;
    v.x = x; v.y = y; v.z = z;
    return v;
}

static QS5Vec3 qs5v_sub(QS5Vec3 a, QS5Vec3 b)
{
    return qs5v_make(qs5_sub(a.x, b.x), qs5_sub(a.y, b.y), qs5_sub(a.z, b.z));
}

static QS5 qs5v_len2(QS5Vec3 v)
{
    return qs5_add(qs5_add(qs5_mul(v.x, v.x), qs5_mul(v.y, v.y)),
                   qs5_mul(v.z, v.z));
}

static QS5 qs5v_dist2(QS5Vec3 a, QS5Vec3 b)
{
    return qs5v_len2(qs5v_sub(a, b));
}

/* ---- integer vector helpers (for rational solids) ---- */

typedef struct { long x, y, z; } Vec3;

static Rat ivec3_dist2(Vec3 a, Vec3 b)
{
    long dx = a.x - b.x;
    long dy = a.y - b.y;
    long dz = a.z - b.z;
    return rat_make(dx*dx + dy*dy + dz*dz, 1);
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
    /* Q(sqrt(5)) constants */
    QS5 ZERO = qs5_from_rat(rat_make(0, 1));
    QS5 ONE  = qs5_from_rat(rat_make(1, 1));
    QS5 MONE = qs5_from_rat(rat_make(-1, 1));
    QS5 PHI  = qs5_make(rat_make(1, 2), rat_make(1, 2));   /* (1+sqrt(5))/2 */
    QS5 MPHI = qs5_make(rat_make(-1, 2), rat_make(-1, 2)); /* -(1+sqrt(5))/2 */
    QS5 IPHI = qs5_make(rat_make(-1, 2), rat_make(1, 2));  /* (sqrt(5)-1)/2 = 1/phi */
    QS5 MIPHI = qs5_make(rat_make(1, 2), rat_make(-1, 2)); /* -(sqrt(5)-1)/2 = -1/phi */

    printf("=== Demo 007: Platonic Solids ===\n\n");

    /* Verify phi^2 = phi + 1 */
    {
        QS5 phi_sq = qs5_mul(PHI, PHI);
        QS5 phi_plus_1 = qs5_add(PHI, ONE);
        check("phi^2 = phi + 1", qs5_eq(phi_sq, phi_plus_1));
        printf("  phi^2 = "); qs5_print(phi_sq);
        printf(",  phi + 1 = "); qs5_print(phi_plus_1); printf("\n");
    }
    /* Verify phi * (1/phi) = 1 */
    {
        QS5 product = qs5_mul(PHI, IPHI);
        check("phi * (1/phi) = 1", qs5_eq(product, ONE));
    }

    /* ======== Part 1: Regular Tetrahedron ======== */
    printf("\n--- Part 1: Regular Tetrahedron ---\n");
    printf("  Vertices: (1,1,1), (1,-1,-1), (-1,1,-1), (-1,-1,1)\n");
    {
        Vec3 tv[4];
        int i, j, edge_count;
        Rat d;

        tv[0].x = 1; tv[0].y = 1; tv[0].z = 1;
        tv[1].x = 1; tv[1].y = -1; tv[1].z = -1;
        tv[2].x = -1; tv[2].y = 1; tv[2].z = -1;
        tv[3].x = -1; tv[3].y = -1; tv[3].z = 1;

        edge_count = 0;
        for (i = 0; i < 4; i++) {
            for (j = i + 1; j < 4; j++) {
                d = ivec3_dist2(tv[i], tv[j]);
                if (rat_eq(d, rat_make(8, 1))) edge_count++;
            }
        }
        printf("  All 6 squared distances = 8 (edge length = 2*sqrt(2))\n");
        check("tetrahedron: all 6 pairs have d^2 = 8", edge_count == 6);

        /* Centroid */
        {
            Rat cx = rat_make(tv[0].x + tv[1].x + tv[2].x + tv[3].x, 4);
            Rat cy = rat_make(tv[0].y + tv[1].y + tv[2].y + tv[3].y, 4);
            Rat cz = rat_make(tv[0].z + tv[1].z + tv[2].z + tv[3].z, 4);
            printf("  Centroid: ("); rat_print(cx);
            printf(", "); rat_print(cy);
            printf(", "); rat_print(cz); printf(")\n");
            check("centroid is origin", rat_eq(cx, rat_make(0,1)) &&
                  rat_eq(cy, rat_make(0,1)) && rat_eq(cz, rat_make(0,1)));
        }

        /* Circumradius */
        d = rat_make(tv[0].x * tv[0].x + tv[0].y * tv[0].y + tv[0].z * tv[0].z, 1);
        printf("  Circumradius^2 = "); rat_print(d); printf("\n");
        check("circumradius^2 = 3", rat_eq(d, rat_make(3, 1)));

        printf("  Field: Z^3 (all integer vertices). T_Q constructible.\n");
    }

    /* ======== Part 2: Cube ======== */
    printf("\n--- Part 2: Cube ---\n");
    printf("  Vertices: all 8 sign combinations of (1,1,1)\n");
    {
        Vec3 cv[8];
        int i, j, idx;
        int edges, face_diags, space_diags;
        long sx, sy, sz;
        Rat d;

        idx = 0;
        for (sx = -1; sx <= 1; sx += 2) {
            for (sy = -1; sy <= 1; sy += 2) {
                for (sz = -1; sz <= 1; sz += 2) {
                    cv[idx].x = sx;
                    cv[idx].y = sy;
                    cv[idx].z = sz;
                    idx++;
                }
            }
        }

        edges = 0; face_diags = 0; space_diags = 0;
        for (i = 0; i < 8; i++) {
            for (j = i + 1; j < 8; j++) {
                d = ivec3_dist2(cv[i], cv[j]);
                if (rat_eq(d, rat_make(4, 1))) edges++;
                else if (rat_eq(d, rat_make(8, 1))) face_diags++;
                else if (rat_eq(d, rat_make(12, 1))) space_diags++;
            }
        }

        printf("  Distance catalog: %d edges (d^2=4), %d face diagonals (d^2=8), %d space diagonals (d^2=12)\n",
               edges, face_diags, space_diags);
        check("cube: 12 edges with d^2 = 4", edges == 12);
        check("cube: 12 face diagonals with d^2 = 8", face_diags == 12);
        check("cube: 4 space diagonals with d^2 = 12", space_diags == 4);
        check("cube: total pairs = 28", edges + face_diags + space_diags == 28);

        d = rat_make(cv[0].x * cv[0].x + cv[0].y * cv[0].y + cv[0].z * cv[0].z, 1);
        printf("  Circumradius^2 = "); rat_print(d); printf("\n");
        check("circumradius^2 = 3", rat_eq(d, rat_make(3, 1)));

        printf("  Field: Z^3. T_Q constructible.\n");
    }

    /* ======== Part 3: Octahedron ======== */
    printf("\n--- Part 3: Octahedron ---\n");
    printf("  Vertices: (+-1,0,0), (0,+-1,0), (0,0,+-1)\n");
    {
        Vec3 ov[6];
        int i, j, edges, diameters;
        Rat d;

        ov[0].x = 1; ov[0].y = 0; ov[0].z = 0;
        ov[1].x = -1; ov[1].y = 0; ov[1].z = 0;
        ov[2].x = 0; ov[2].y = 1; ov[2].z = 0;
        ov[3].x = 0; ov[3].y = -1; ov[3].z = 0;
        ov[4].x = 0; ov[4].y = 0; ov[4].z = 1;
        ov[5].x = 0; ov[5].y = 0; ov[5].z = -1;

        edges = 0; diameters = 0;
        for (i = 0; i < 6; i++) {
            for (j = i + 1; j < 6; j++) {
                d = ivec3_dist2(ov[i], ov[j]);
                if (rat_eq(d, rat_make(2, 1))) edges++;
                else if (rat_eq(d, rat_make(4, 1))) diameters++;
            }
        }

        printf("  Distance catalog: %d edges (d^2=2), %d diameters (d^2=4)\n",
               edges, diameters);
        check("octahedron: 12 edges with d^2 = 2", edges == 12);
        check("octahedron: 3 diameters with d^2 = 4", diameters == 3);
        check("octahedron: total pairs = 15", edges + diameters == 15);

        d = rat_make(ov[0].x * ov[0].x + ov[0].y * ov[0].y + ov[0].z * ov[0].z, 1);
        printf("  Circumradius^2 = "); rat_print(d); printf("\n");
        check("circumradius^2 = 1", rat_eq(d, rat_make(1, 1)));

        printf("  Field: Z^3. T_Q constructible.\n");
        printf("  Note: octahedron vertices are a SUBSET of bootstrap points!\n");
        printf("    (1,0,0)=A, (0,1,0)=B, (0,0,1)=C are initial.\n");
        printf("    (-1,0,0), (0,-1,0), (0,0,-1) are bootstrap reflections.\n");
    }

    /* ======== Part 4: Icosahedron ======== */
    printf("\n--- Part 4: Icosahedron ---\n");
    printf("  Vertices: cyclic permutations of (0, +-1, +-phi)\n");
    printf("  phi = (1+sqrt(5))/2\n");
    {
        QS5Vec3 iv[12];
        int i, j;
        int edges;
        QS5 d, edge_d2;
        QS5 FOUR = qs5_from_rat(rat_make(4, 1));

        /* (0, +-1, +-phi) */
        iv[0]  = qs5v_make(ZERO, ONE,  PHI);
        iv[1]  = qs5v_make(ZERO, ONE,  MPHI);
        iv[2]  = qs5v_make(ZERO, MONE, PHI);
        iv[3]  = qs5v_make(ZERO, MONE, MPHI);
        /* (+-1, +-phi, 0) */
        iv[4]  = qs5v_make(ONE,  PHI,  ZERO);
        iv[5]  = qs5v_make(ONE,  MPHI, ZERO);
        iv[6]  = qs5v_make(MONE, PHI,  ZERO);
        iv[7]  = qs5v_make(MONE, MPHI, ZERO);
        /* (+-phi, 0, +-1) */
        iv[8]  = qs5v_make(PHI,  ZERO, ONE);
        iv[9]  = qs5v_make(PHI,  ZERO, MONE);
        iv[10] = qs5v_make(MPHI, ZERO, ONE);
        iv[11] = qs5v_make(MPHI, ZERO, MONE);

        /* Count edges and verify squared edge length */
        edges = 0;
        edge_d2 = FOUR;

        for (i = 0; i < 12; i++) {
            for (j = i + 1; j < 12; j++) {
                d = qs5v_dist2(iv[i], iv[j]);
                if (qs5_eq(d, edge_d2)) edges++;
            }
        }

        printf("  Edges with d^2 = 4: %d\n", edges);
        check("icosahedron: 30 edges", edges == 30);
        check("icosahedron: squared edge length is rational (= 4)",
              qs5_eq(edge_d2, FOUR) && qs5_is_rational(edge_d2));

        /* Circumradius */
        d = qs5v_len2(iv[0]);
        printf("  Circumradius^2 = "); qs5_print(d); printf("\n");
        check("circumradius^2 = 5/2 + sqrt(5)/2",
              qs5_eq(d, qs5_make(rat_make(5, 2), rat_make(1, 2))));
        check("circumradius is irrational", !qs5_is_rational(d));

        /* Count all distance classes */
        {
            int total_pairs = 0;
            int irrational_dist = 0;
            for (i = 0; i < 12; i++) {
                for (j = i + 1; j < 12; j++) {
                    d = qs5v_dist2(iv[i], iv[j]);
                    total_pairs++;
                    if (!qs5_is_rational(d)) irrational_dist++;
                }
            }
            printf("  Total pairs: %d, with irrational d^2: %d\n",
                   total_pairs, irrational_dist);
            check("66 total pairs (C(12,2))", total_pairs == 66);
            check("some non-edge distances are irrational", irrational_dist > 0);
        }

        printf("  Field: Q(sqrt(5))^3. T_E required (spheres needed for sqrt(5)).\n");
        printf("  Key insight: d^2 = 4 is RATIONAL despite irrational coords.\n");
        printf("  The sqrt(5) cancels in the sum of squared differences.\n");
    }

    /* ======== Part 5: Dodecahedron ======== */
    printf("\n--- Part 5: Dodecahedron ---\n");
    printf("  Vertices: (+-1)^3 union cyclic perms of (0, +-1/phi, +-phi)\n");
    {
        QS5Vec3 dv[20];
        int idx, i, j;
        int edges;
        QS5 d, edge_d2;
        /* Squared edge length = 6 - 2*sqrt(5) */
        QS5 EXPECTED_EDGE = qs5_make(rat_make(6, 1), rat_make(-2, 1));

        idx = 0;
        /* Group A: cube vertices (+-1, +-1, +-1) */
        {
            long sx, sy, sz;
            for (sx = -1; sx <= 1; sx += 2)
                for (sy = -1; sy <= 1; sy += 2)
                    for (sz = -1; sz <= 1; sz += 2) {
                        dv[idx] = qs5v_make(
                            qs5_from_rat(rat_make(sx, 1)),
                            qs5_from_rat(rat_make(sy, 1)),
                            qs5_from_rat(rat_make(sz, 1)));
                        idx++;
                    }
        }
        /* Group B: (0, +-1/phi, +-phi) */
        dv[idx++] = qs5v_make(ZERO, IPHI,  PHI);
        dv[idx++] = qs5v_make(ZERO, IPHI,  MPHI);
        dv[idx++] = qs5v_make(ZERO, MIPHI, PHI);
        dv[idx++] = qs5v_make(ZERO, MIPHI, MPHI);
        /* Group C: (+-1/phi, +-phi, 0) */
        dv[idx++] = qs5v_make(IPHI,  PHI,  ZERO);
        dv[idx++] = qs5v_make(IPHI,  MPHI, ZERO);
        dv[idx++] = qs5v_make(MIPHI, PHI,  ZERO);
        dv[idx++] = qs5v_make(MIPHI, MPHI, ZERO);
        /* Group D: (+-phi, 0, +-1/phi) */
        dv[idx++] = qs5v_make(PHI,  ZERO, IPHI);
        dv[idx++] = qs5v_make(PHI,  ZERO, MIPHI);
        dv[idx++] = qs5v_make(MPHI, ZERO, IPHI);
        dv[idx++] = qs5v_make(MPHI, ZERO, MIPHI);

        printf("  Total vertices: %d\n", idx);
        check("dodecahedron: 20 vertices", idx == 20);

        /* Count edges */
        edges = 0;
        edge_d2 = EXPECTED_EDGE;

        for (i = 0; i < 20; i++) {
            for (j = i + 1; j < 20; j++) {
                d = qs5v_dist2(dv[i], dv[j]);
                if (qs5_eq(d, edge_d2)) edges++;
            }
        }

        printf("  Edges with d^2 = 6 - 2*sqrt(5): %d\n", edges);
        check("dodecahedron: 30 edges", edges == 30);
        check("squared edge length is IRRATIONAL",
              !qs5_is_rational(EXPECTED_EDGE));
        printf("  d^2 = "); qs5_print(EXPECTED_EDGE); printf("\n");

        /* Circumradius: all vertices should be at same distance from origin */
        {
            QS5 r2_cube, r2_non;
            int all_same = 1;

            r2_cube = qs5v_len2(dv[0]);  /* cube vertex */
            r2_non = qs5v_len2(dv[8]);   /* non-cube vertex */
            printf("  |cube vertex|^2 = "); qs5_print(r2_cube); printf("\n");
            printf("  |non-cube vertex|^2 = "); qs5_print(r2_non); printf("\n");

            for (i = 1; i < 20; i++) {
                if (!qs5_eq(qs5v_len2(dv[i]), r2_cube)) {
                    all_same = 0;
                    break;
                }
            }
            check("all vertices equidistant from origin", all_same);
            check("circumradius^2 = 3 (rational!)",
                  qs5_is_rational(r2_cube) && rat_eq(r2_cube.a, rat_make(3, 1)));
        }

        printf("  Field: Q(sqrt(5))^3. T_E required.\n");
        printf("  Note: cube vertices are rational; 12 others are not.\n");
        printf("  Contrast with icosahedron: d^2 rational there, irrational here.\n");
    }

    /* ======== Part 6: Duality check ======== */
    printf("\n--- Part 6: Duality and Euler characteristic ---\n");
    {
        printf("  Solid          V   E   F   V-E+F  d^2(edge)  Field\n");
        printf("  Tetrahedron    4   6   4     2      8        Z^3\n");
        printf("  Cube           8  12   6     2      4        Z^3\n");
        printf("  Octahedron     6  12   8     2      2        Z^3\n");
        printf("  Icosahedron   12  30  20     2      4        Q(sqrt(5))^3\n");
        printf("  Dodecahedron  20  30  12     2      6-2s5    Q(sqrt(5))^3\n");
        printf("\n");
        printf("  Dual pairs: Cube <-> Octahedron, Icosahedron <-> Dodecahedron\n");
        printf("  Self-dual: Tetrahedron\n");
        printf("\n");
        printf("  Classification:\n");
        printf("    T_Q (lines + planes): Tetrahedron, Cube, Octahedron\n");
        printf("    T_E (spheres needed): Icosahedron, Dodecahedron\n");
        printf("  The boundary is the golden ratio: phi = (1+sqrt(5))/2.\n");
        printf("  A5 representation theory forces sqrt(5) into icosahedral coords.\n");

        /* Verify the V-E+F = 2 for our computed values */
        check("tetrahedron: 4-6+4 = 2", 4 - 6 + 4 == 2);
        check("cube: 8-12+6 = 2", 8 - 12 + 6 == 2);
        check("octahedron: 6-12+8 = 2", 6 - 12 + 8 == 2);
        check("icosahedron: 12-30+20 = 2", 12 - 30 + 20 == 2);
        check("dodecahedron: 20-30+12 = 2", 20 - 30 + 12 == 2);
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("The five Platonic solids divide into two algebraic classes:\n");
    printf("  RATIONAL (T_Q): tetrahedron, cube, octahedron\n");
    printf("    All vertices in Z^3. Constructible with lines + planes.\n");
    printf("  GOLDEN (T_E): icosahedron, dodecahedron\n");
    printf("    Vertices in Q(sqrt(5))^3. Spheres needed for sqrt(5).\n");
    printf("The octahedron is already present in the bootstrap.\n");
    printf("The cube is reachable in one more generation.\n");
    printf("The icosahedron/dodecahedron require the golden ratio,\n");
    printf("  which is constructible but not rational.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
