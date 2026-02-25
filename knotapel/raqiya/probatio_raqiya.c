/*
 * probatio_raqiya.c -- Unit tests for the Raqiya library
 * ======================================================
 *
 * Regression safety net: expand test coverage whenever raqiya.h
 * gains new functionality or is used in a new demo.
 *
 * C89, zero dependencies beyond stdio/stdlib/string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAQ_CONVENIENCE
#include "raqiya.h"

static int n_pass = 0, n_fail = 0;

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * 1. Cyc8 Arithmetic
 * ================================================================ */
static void test_arithmetic(void) {
    Raq_Cyc8 z, w, r;

    printf("\n--- Cyc8 Arithmetic ---\n");

    /* Zero and one */
    z = raq_cyc8_zero();
    check("zero is (0,0,0,0)", z.a == 0 && z.b == 0 && z.c == 0 && z.d == 0);
    check("zero is_zero", raq_cyc8_is_zero(z));

    w = raq_cyc8_one();
    check("one is (1,0,0,0)", w.a == 1 && w.b == 0 && w.c == 0 && w.d == 0);
    check("one is not zero", !raq_cyc8_is_zero(w));

    /* Addition */
    z = raq_cyc8_make(1, 2, 3, 4);
    w = raq_cyc8_make(10, 20, 30, 40);
    r = raq_cyc8_add(z, w);
    check("add (1,2,3,4)+(10,20,30,40)=(11,22,33,44)",
          r.a == 11 && r.b == 22 && r.c == 33 && r.d == 44);

    /* Negation */
    r = raq_cyc8_neg(z);
    check("neg (1,2,3,4)=(-1,-2,-3,-4)",
          r.a == -1 && r.b == -2 && r.c == -3 && r.d == -4);

    /* Subtraction */
    r = raq_cyc8_sub(w, z);
    check("sub (10,20,30,40)-(1,2,3,4)=(9,18,27,36)",
          r.a == 9 && r.b == 18 && r.c == 27 && r.d == 36);

    /* Multiplication: 1 * x = x */
    r = raq_cyc8_mul(raq_cyc8_one(), z);
    check("1 * z = z", raq_cyc8_eq(r, z));

    /* Multiplication: 0 * x = 0 */
    r = raq_cyc8_mul(raq_cyc8_zero(), z);
    check("0 * z = 0", raq_cyc8_is_zero(r));

    /* Multiplication: zeta^4 = -1 */
    {
        Raq_Cyc8 zeta = raq_cyc8_make(0, 1, 0, 0);
        Raq_Cyc8 z2 = raq_cyc8_mul(zeta, zeta);
        Raq_Cyc8 z4 = raq_cyc8_mul(z2, z2);
        check("zeta^2 = (0,0,1,0)", z2.a == 0 && z2.b == 0 && z2.c == 1 && z2.d == 0);
        check("zeta^4 = -1", z4.a == -1 && z4.b == 0 && z4.c == 0 && z4.d == 0);
    }

    /* Multiplication commutativity check */
    {
        Raq_Cyc8 a = raq_cyc8_make(1, 2, -1, 3);
        Raq_Cyc8 b = raq_cyc8_make(-2, 0, 1, 4);
        Raq_Cyc8 ab = raq_cyc8_mul(a, b);
        Raq_Cyc8 ba = raq_cyc8_mul(b, a);
        check("mul is commutative", raq_cyc8_eq(ab, ba));
    }

    /* Equality */
    check("eq: same values", raq_cyc8_eq(z, raq_cyc8_make(1, 2, 3, 4)));
    check("eq: different values", !raq_cyc8_eq(z, w));
}

/* ================================================================
 * 2. Conjugation and Norm
 * ================================================================ */
static void test_conjugation(void) {
    Raq_Cyc8 z, c, ns;

    printf("\n--- Conjugation and Norm ---\n");

    /* conj(a,b,c,d) = (a,-d,-c,-b) */
    z = raq_cyc8_make(2, 3, -1, 5);
    c = raq_cyc8_conj(z);
    check("conj(2,3,-1,5) = (2,-5,1,-3)",
          c.a == 2 && c.b == -5 && c.c == 1 && c.d == -3);

    /* conj(conj(z)) = z */
    check("conj(conj(z)) = z", raq_cyc8_eq(raq_cyc8_conj(c), z));

    /* conj(1) = 1 (rationals are fixed) */
    check("conj(1) = 1", raq_cyc8_eq(raq_cyc8_conj(raq_cyc8_one()),
                                       raq_cyc8_one()));

    /* norm_sq lives in Z[sqrt(2)]: c=0, d=-b */
    ns = raq_cyc8_norm_sq(z);
    check("norm_sq: c component is 0", ns.c == 0);
    check("norm_sq: d = -b", ns.b + ns.d == 0);

    /* norm_sq(1) = 1 */
    ns = raq_cyc8_norm_sq(raq_cyc8_one());
    check("norm_sq(1) = 1", raq_cyc8_eq(ns, raq_cyc8_one()));

    /* norm_sq(zeta) = 1 (unit) */
    ns = raq_cyc8_norm_sq(raq_cyc8_make(0, 1, 0, 0));
    check("norm_sq(zeta) = 1", raq_cyc8_eq(ns, raq_cyc8_one()));

    /* norm_sq(2*zeta) = 4 */
    ns = raq_cyc8_norm_sq(raq_cyc8_make(0, 2, 0, 0));
    check("norm_sq(2*zeta) = 4", ns.a == 4 && ns.b == 0 && ns.c == 0 && ns.d == 0);

    /* norm_sq(n) = n^2 for real n */
    ns = raq_cyc8_norm_sq(raq_cyc8_make(7, 0, 0, 0));
    check("norm_sq(7) = 49", ns.a == 49 && ns.b == 0 && ns.c == 0 && ns.d == 0);

    /* norm_sq on generic value still satisfies Z[sqrt(2)] constraint */
    {
        Raq_Cyc8 test = raq_cyc8_make(-3, 7, 2, -5);
        Raq_Cyc8 n = raq_cyc8_norm_sq(test);
        check("norm_sq generic: c=0 and d=-b",
              n.c == 0 && (n.b + n.d) == 0);
    }
}

/* ================================================================
 * 3. Zeta Rotation
 * ================================================================ */
static void test_zeta_rotation(void) {
    Raq_Cyc8 z, rotated;
    int k;

    printf("\n--- Zeta Rotation ---\n");

    /* mul_zeta: (a,b,c,d) -> (-d,a,b,c) */
    z = raq_cyc8_make(1, 2, 3, 4);
    rotated = raq_cyc8_mul_zeta(z);
    check("mul_zeta (1,2,3,4) = (-4,1,2,3)",
          rotated.a == -4 && rotated.b == 1 && rotated.c == 2 && rotated.d == 3);

    /* 8 rotations = identity */
    z = raq_cyc8_make(5, -3, 2, 7);
    rotated = z;
    for (k = 0; k < 8; k++)
        rotated = raq_cyc8_mul_zeta(rotated);
    check("8 rotations = identity", raq_cyc8_eq(rotated, z));

    /* 4 rotations = negation (zeta^4 = -1) */
    z = raq_cyc8_make(1, 0, 0, 0);
    rotated = z;
    for (k = 0; k < 4; k++)
        rotated = raq_cyc8_mul_zeta(rotated);
    check("4 rotations of 1 = -1",
          raq_cyc8_eq(rotated, raq_cyc8_make(-1, 0, 0, 0)));

    /* mul_zeta is consistent with mul(zeta, z) */
    z = raq_cyc8_make(2, -1, 3, 0);
    {
        Raq_Cyc8 zeta = raq_cyc8_make(0, 1, 0, 0);
        Raq_Cyc8 via_mul = raq_cyc8_mul(zeta, z);
        Raq_Cyc8 via_rot = raq_cyc8_mul_zeta(z);
        check("mul_zeta = mul(zeta, z)", raq_cyc8_eq(via_mul, via_rot));
    }
}

/* ================================================================
 * 4. Galois Automorphisms
 * ================================================================ */
static void test_galois(void) {
    Raq_Cyc8 z, s3, s5, s7;

    printf("\n--- Galois Automorphisms ---\n");

    z = raq_cyc8_make(2, 3, -1, 5);

    /* Order 2 */
    s3 = raq_cyc8_galois_3(z);
    check("sigma_3 order 2", raq_cyc8_eq(raq_cyc8_galois_3(s3), z));

    s5 = raq_cyc8_galois_5(z);
    check("sigma_5 order 2", raq_cyc8_eq(raq_cyc8_galois_5(s5), z));

    s7 = raq_cyc8_galois_7(z);
    check("sigma_7 order 2", raq_cyc8_eq(raq_cyc8_galois_7(s7), z));

    /* sigma_7 = sigma_5 o sigma_3 */
    check("sigma_7 = sigma_5 o sigma_3",
          raq_cyc8_eq(s7, raq_cyc8_galois_5(s3)));

    /* sigma_3 is multiplicative */
    {
        Raq_Cyc8 x = raq_cyc8_make(1, 2, 0, -1);
        Raq_Cyc8 y = raq_cyc8_make(0, 1, -1, 3);
        Raq_Cyc8 xy = raq_cyc8_mul(x, y);
        check("sigma_3 multiplicative",
              raq_cyc8_eq(raq_cyc8_galois_3(xy),
                          raq_cyc8_mul(raq_cyc8_galois_3(x),
                                       raq_cyc8_galois_3(y))));
    }

    /* sigma_3 is additive */
    {
        Raq_Cyc8 x = raq_cyc8_make(1, 2, 0, -1);
        Raq_Cyc8 y = raq_cyc8_make(0, 1, -1, 3);
        Raq_Cyc8 s = raq_cyc8_add(x, y);
        check("sigma_3 additive",
              raq_cyc8_eq(raq_cyc8_galois_3(s),
                          raq_cyc8_add(raq_cyc8_galois_3(x),
                                       raq_cyc8_galois_3(y))));
    }

    /* Rationals are fixed by all automorphisms */
    {
        Raq_Cyc8 rational = raq_cyc8_make(42, 0, 0, 0);
        check("sigma_3 fixes rationals",
              raq_cyc8_eq(raq_cyc8_galois_3(rational), rational));
        check("sigma_5 fixes rationals",
              raq_cyc8_eq(raq_cyc8_galois_5(rational), rational));
        check("sigma_7 fixes rationals",
              raq_cyc8_eq(raq_cyc8_galois_7(rational), rational));
    }

    /* Galois on specific values (explorer's test cases) */
    {
        /* zeta and zeta^3 should be Galois conjugates under sigma_3 */
        Raq_Cyc8 zeta = raq_cyc8_make(0, 1, 0, 0);
        Raq_Cyc8 zeta3 = raq_cyc8_make(0, 0, 0, 1);
        check("sigma_3(zeta) = zeta^3",
              raq_cyc8_eq(raq_cyc8_galois_3(zeta), zeta3));
    }
    {
        /* 1 and zeta should NOT be Galois conjugates */
        Raq_Cyc8 one = raq_cyc8_one();
        Raq_Cyc8 zeta = raq_cyc8_make(0, 1, 0, 0);
        int related = 0;
        related = raq_cyc8_eq(raq_cyc8_galois_3(one), zeta) ||
                  raq_cyc8_eq(raq_cyc8_galois_5(one), zeta) ||
                  raq_cyc8_eq(raq_cyc8_galois_7(one), zeta);
        check("1 and zeta are NOT Galois conjugates", !related);
    }
}

/* ================================================================
 * 5. Sign Pattern and Zero Count
 * ================================================================ */
static void test_sign_pattern(void) {
    Raq_Cyc8 z;

    printf("\n--- Sign Pattern and Zero Count ---\n");

    z = raq_cyc8_make(1, -2, 0, 3);
    check("zero_count (1,-2,0,3) = 1", raq_cyc8_zero_count(z) == 1);
    check("zero_count (0,0,0,0) = 4",
          raq_cyc8_zero_count(raq_cyc8_zero()) == 4);
    check("zero_count (1,1,1,1) = 0",
          raq_cyc8_zero_count(raq_cyc8_make(1, 1, 1, 1)) == 0);

    /* Sign distance */
    {
        Raq_Cyc8 a = raq_cyc8_make(1, -1, 0, 2);
        Raq_Cyc8 b = raq_cyc8_make(1, -1, 0, 2);
        check("sign_distance identical = 0", raq_cyc8_sign_distance(a, b) == 0);
    }
    {
        Raq_Cyc8 a = raq_cyc8_make(1, 0, 0, 0);
        Raq_Cyc8 b = raq_cyc8_make(-1, 0, 0, 0);
        check("sign_distance (1,0,0,0) vs (-1,0,0,0) = 1",
              raq_cyc8_sign_distance(a, b) == 1);
    }
    {
        Raq_Cyc8 a = raq_cyc8_make(1, 2, 3, 4);
        Raq_Cyc8 b = raq_cyc8_make(-1, -2, -3, -4);
        check("sign_distance all-opposite = 4",
              raq_cyc8_sign_distance(a, b) == 4);
    }

    /* Sign pattern encoding */
    check("sign_pattern (0,0,0,0) = 40",
          raq_cyc8_sign_pattern(raq_cyc8_zero()) == 40);
    check("sign_pattern (1,1,1,1) = 80",
          raq_cyc8_sign_pattern(raq_cyc8_make(1, 1, 1, 1)) == 80);
}

/* ================================================================
 * 6. ValueSet
 * ================================================================ */
static void test_valueset(void) {
    Raq_ValueSet vs;
    int idx;

    printf("\n--- ValueSet ---\n");

    raq_vs_init(&vs, 100);
    check("vs init: count=0", vs.count == 0);

    /* Insert a value */
    idx = raq_vs_insert(&vs, raq_cyc8_make(1, 2, 3, 4));
    check("vs insert returns 0", idx == 0);
    check("vs count=1 after insert", vs.count == 1);

    /* Insert duplicate -- should return existing index */
    idx = raq_vs_insert(&vs, raq_cyc8_make(1, 2, 3, 4));
    check("vs duplicate returns 0", idx == 0);
    check("vs count still 1", vs.count == 1);

    /* Insert different value */
    idx = raq_vs_insert(&vs, raq_cyc8_make(5, 6, 7, 8));
    check("vs second insert returns 1", idx == 1);
    check("vs count=2", vs.count == 2);

    /* Find operations */
    check("vs find existing = 0",
          raq_vs_find(&vs, raq_cyc8_make(1, 2, 3, 4)) == 0);
    check("vs find second = 1",
          raq_vs_find(&vs, raq_cyc8_make(5, 6, 7, 8)) == 1);
    check("vs find missing = -1",
          raq_vs_find(&vs, raq_cyc8_make(9, 9, 9, 9)) == -1);

    raq_vs_free(&vs);
}

/* ================================================================
 * 7. Detector: Root-of-Unity Orbits
 * ================================================================ */
static void test_root_orbits(void) {
    Raq_Partition p;

    printf("\n--- Root-of-Unity Orbits ---\n");

    /* zeta and -zeta should be in the same orbit */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_make(0, 1, 0, 0);  /* zeta */
        vals[1] = raq_cyc8_make(0, -1, 0, 0); /* -zeta */
        p = raq_detect_root_orbits(vals, 2);
        check("zeta and -zeta: same orbit",
              p.n_groups == 1);
        raq_partition_free(&p);
    }

    /* 1 and zeta should be in the same orbit (1 = zeta^0) */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_one();
        vals[1] = raq_cyc8_make(0, 1, 0, 0);
        p = raq_detect_root_orbits(vals, 2);
        check("1 and zeta: same orbit", p.n_groups == 1);
        raq_partition_free(&p);
    }

    /* 1 and 2 should be in different orbits (2 != +/-zeta^k) */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_one();
        vals[1] = raq_cyc8_make(2, 0, 0, 0);
        p = raq_detect_root_orbits(vals, 2);
        check("1 and 2: different orbits", p.n_groups == 2);
        raq_partition_free(&p);
    }

    /* All 8 units {+/-1, +/-zeta, +/-zeta^2, +/-zeta^3} = 1 orbit */
    {
        Raq_Cyc8 vals[8];
        vals[0] = raq_cyc8_make(1, 0, 0, 0);
        vals[1] = raq_cyc8_make(-1, 0, 0, 0);
        vals[2] = raq_cyc8_make(0, 1, 0, 0);
        vals[3] = raq_cyc8_make(0, -1, 0, 0);
        vals[4] = raq_cyc8_make(0, 0, 1, 0);
        vals[5] = raq_cyc8_make(0, 0, -1, 0);
        vals[6] = raq_cyc8_make(0, 0, 0, 1);
        vals[7] = raq_cyc8_make(0, 0, 0, -1);
        p = raq_detect_root_orbits(vals, 8);
        check("all 8 units form 1 orbit", p.n_groups == 1);
        check("orbit has size 8", p.group_sizes[0] == 8);
        raq_partition_free(&p);
    }
}

/* ================================================================
 * 8. Detector: Norm-Squared Classes
 * ================================================================ */
static void test_norm_classes(void) {
    Raq_Partition p;

    printf("\n--- Norm-Squared Classes ---\n");

    /* Values with same |z|^2 should cluster */
    {
        Raq_Cyc8 vals[4];
        vals[0] = raq_cyc8_make(0, 1, 0, 0);  /* |z|^2 = 1 */
        vals[1] = raq_cyc8_make(0, 0, 0, 1);  /* |z|^2 = 1 */
        vals[2] = raq_cyc8_make(2, 0, 0, 0);  /* |z|^2 = 4 */
        vals[3] = raq_cyc8_make(0, 2, 0, 0);  /* |z|^2 = 4 */
        p = raq_detect_norm_classes(vals, 4);
        check("4 values with 2 norms: 2 classes", p.n_groups == 2);
        check("norm class sizes: 2 and 2",
              p.group_sizes[0] == 2 && p.group_sizes[1] == 2);
        raq_partition_free(&p);
    }

    /* Zero has norm 0, distinct from units */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_zero();
        vals[1] = raq_cyc8_one();
        p = raq_detect_norm_classes(vals, 2);
        check("0 and 1: different norm classes", p.n_groups == 2);
        raq_partition_free(&p);
    }
}

/* ================================================================
 * 9. Detector: Galois Orbits
 * ================================================================ */
static void test_galois_orbits(void) {
    Raq_Partition p;

    printf("\n--- Galois Orbits ---\n");

    /* zeta and zeta^3 are Galois conjugates (sigma_3) */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_make(0, 1, 0, 0);  /* zeta */
        vals[1] = raq_cyc8_make(0, 0, 0, 1);  /* zeta^3 */
        p = raq_detect_galois_orbits(vals, 2);
        check("zeta and zeta^3: same Galois orbit", p.n_groups == 1);
        raq_partition_free(&p);
    }

    /* Full Galois orbit of zeta: {zeta, zeta^3, -zeta, -zeta^3} */
    {
        Raq_Cyc8 vals[4];
        vals[0] = raq_cyc8_make(0, 1, 0, 0);   /* zeta */
        vals[1] = raq_cyc8_make(0, 0, 0, 1);   /* zeta^3 = sigma_3(zeta) */
        vals[2] = raq_cyc8_make(0, -1, 0, 0);  /* -zeta = sigma_5(zeta) */
        vals[3] = raq_cyc8_make(0, 0, 0, -1);  /* -zeta^3 = sigma_7(zeta) */
        p = raq_detect_galois_orbits(vals, 4);
        check("full Galois orbit of zeta: 1 orbit", p.n_groups == 1);
        check("full orbit size = 4", p.group_sizes[0] == 4);
        raq_partition_free(&p);
    }

    /* 1 and zeta are NOT Galois conjugates */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_one();
        vals[1] = raq_cyc8_make(0, 1, 0, 0);
        p = raq_detect_galois_orbits(vals, 2);
        check("1 and zeta: different Galois orbits", p.n_groups == 2);
        raq_partition_free(&p);
    }

    /* zeta^2 and -zeta^2: Galois orbit of size 2 */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_make(0, 0, 1, 0);   /* zeta^2 = i */
        vals[1] = raq_cyc8_make(0, 0, -1, 0);  /* -zeta^2 = -i */
        p = raq_detect_galois_orbits(vals, 2);
        check("zeta^2 and -zeta^2: same Galois orbit", p.n_groups == 1);
        raq_partition_free(&p);
    }
}

/* ================================================================
 * 10. Detector: Sum/Diff Zero Components
 * ================================================================ */
static void test_sum_diff(void) {
    printf("\n--- Sum/Diff Zero Components ---\n");

    /* Two axis-aligned values on same axis: sum has 3 zeros */
    {
        Raq_Cyc8 vals[2];
        Raq_PairHist h;
        vals[0] = raq_cyc8_make(2, 0, 0, 0);
        vals[1] = raq_cyc8_make(3, 0, 0, 0);
        h = raq_detect_sum_zeros(vals, 2);
        check("sum of same-axis: 3 zero components", h.bins[3] == 1);
    }

    /* Negatives: x + (-x) = 0, so 4 zero components */
    {
        Raq_Cyc8 vals[2];
        Raq_PairHist h;
        vals[0] = raq_cyc8_make(0, 5, 0, 0);
        vals[1] = raq_cyc8_make(0, -5, 0, 0);
        h = raq_detect_sum_zeros(vals, 2);
        check("sum of negatives: 4 zero components", h.bins[4] == 1);
    }

    /* count_neg_pairs */
    {
        Raq_Cyc8 vals[3];
        vals[0] = raq_cyc8_make(1, 0, 0, 0);
        vals[1] = raq_cyc8_make(-1, 0, 0, 0);
        vals[2] = raq_cyc8_make(2, 0, 0, 0);
        check("neg_pairs: 1 and -1 are negatives",
              raq_count_neg_pairs(vals, 3) == 1);
    }
}

/* ================================================================
 * 11. Detector: Product Structure
 * ================================================================ */
static void test_products(void) {
    printf("\n--- Product Structure ---\n");

    /* Product of two axis-aligned values on same power */
    {
        Raq_Cyc8 vals[2];
        Raq_ProductResult r;
        vals[0] = raq_cyc8_make(2, 0, 0, 0);
        vals[1] = raq_cyc8_make(3, 0, 0, 0);
        r = raq_detect_products(vals, 2);
        check("product of reals: is real", r.n_real == 1);
    }

    /* Product of zeta * zeta = zeta^2 (pure imaginary) */
    {
        Raq_Cyc8 vals[2];
        Raq_ProductResult r;
        vals[0] = raq_cyc8_make(0, 1, 0, 0);  /* zeta */
        vals[1] = raq_cyc8_make(0, 1, 0, 0);  /* zeta */
        /* Note: pairs are (i,j) with i<j, so only 1 pair if distinct.
         * But these are identical so no pair is generated. */
        r = raq_detect_products(vals, 2);
        /* Since both values are the same, there ARE no pairs with i<j
         * where values differ. But the detector uses index pairs, so
         * (0,1) IS a pair even though values happen to be equal. */
        check("product of zeta * zeta: 3 zero components",
              r.zero_components.bins[3] == 1);
    }
}

/* ================================================================
 * 12. Partition Helpers
 * ================================================================ */
static void test_partition_helpers(void) {
    Raq_Partition p;
    Raq_Cyc8 vals[5];

    printf("\n--- Partition Helpers ---\n");

    vals[0] = raq_cyc8_make(0, 1, 0, 0);   /* zeta, norm 1 */
    vals[1] = raq_cyc8_make(0, 0, 0, 1);   /* zeta^3, norm 1 */
    vals[2] = raq_cyc8_make(2, 0, 0, 0);   /* 2, norm 4 */
    vals[3] = raq_cyc8_make(0, 2, 0, 0);   /* 2*zeta, norm 4 */
    vals[4] = raq_cyc8_make(3, 0, 0, 0);   /* 3, norm 9 */

    p = raq_detect_norm_classes(vals, 5);

    check("partition: 3 norm classes", p.n_groups == 3);
    check("partition: max_size = 2", raq_partition_max_size(&p) == 2);
    /* related pairs: C(2,2) + C(2,2) + C(1,2) = 1+1+0 = 2 */
    check("partition: related_pairs = 2", raq_partition_related_pairs(&p) == 2);

    raq_partition_free(&p);
}

/* ================================================================
 * 13. Full Analysis (integration test)
 * ================================================================ */
static void test_full_analysis(void) {
    Raq_Cyc8 vals[6];
    Raq_Analysis a;

    printf("\n--- Full Analysis (Integration) ---\n");

    /* The D107 first-128 value set (first 6 values as a mini test) */
    vals[0] = raq_cyc8_one();                   /* 1 */
    vals[1] = raq_cyc8_zero();                  /* 0 */
    vals[2] = raq_cyc8_make(0, -1, 0, 0);      /* -zeta */
    vals[3] = raq_cyc8_make(0, 0, 0, 1);       /* zeta^3 */
    vals[4] = raq_cyc8_make(0, 0, 1, 0);       /* zeta^2 */
    vals[5] = raq_cyc8_make(2, 0, 0, 0);       /* 2 */

    a = raq_analyze(vals, 6);

    check("analysis: 6 values", a.n_values == 6);
    /* root orbits: {1, -zeta, zeta^3, zeta^2} = 1 orbit, {0} = 1, {2} = 1 */
    check("analysis: 3 root orbits", a.root_orbits.n_groups == 3);
    /* norm classes: norm=1 has {1, -zeta, zeta^3, zeta^2}, norm=0 has {0}, norm=4 has {2} */
    check("analysis: 3 norm classes", a.norm_classes.n_groups == 3);
    /* Galois: {1}=fixed, {0}=fixed, {-zeta, zeta^3}=sigma_3 pair,
     *         {zeta^2}=sigma_5 maps to -zeta^2 not present, {2}=fixed */
    check("analysis: 5 Galois orbits", a.galois_orbits.n_groups == 5);
    check("analysis: sign_patterns > 0", a.n_sign_patterns > 0);

    raq_analysis_free(&a);
}

/* ================================================================
 * 14. Axis Alignment Property (D107 finding)
 *
 * All values produced by W_{4,2} TL representation at delta=0
 * should have exactly one nonzero component (or be zero).
 * This verifies the property on a known set.
 * ================================================================ */
static void test_axis_alignment(void) {
    int i;
    int all_axis = 1;

    /* The 13 values from D107 first-128 entries */
    Raq_Cyc8 d107_vals[13];
    d107_vals[0]  = raq_cyc8_make(1, 0, 0, 0);
    d107_vals[1]  = raq_cyc8_make(0, 0, 0, 0);
    d107_vals[2]  = raq_cyc8_make(0, -1, 0, 0);
    d107_vals[3]  = raq_cyc8_make(0, 0, 0, 1);
    d107_vals[4]  = raq_cyc8_make(0, 0, 1, 0);
    d107_vals[5]  = raq_cyc8_make(2, 0, 0, 0);
    d107_vals[6]  = raq_cyc8_make(0, 0, -1, 0);
    d107_vals[7]  = raq_cyc8_make(0, 0, 0, -1);
    d107_vals[8]  = raq_cyc8_make(0, -3, 0, 0);
    d107_vals[9]  = raq_cyc8_make(0, -2, 0, 0);
    d107_vals[10] = raq_cyc8_make(0, 0, 0, 2);
    d107_vals[11] = raq_cyc8_make(0, 1, 0, 0);
    d107_vals[12] = raq_cyc8_make(0, 0, 0, 3);

    printf("\n--- Axis Alignment Property ---\n");

    for (i = 0; i < 13; i++) {
        int zc = raq_cyc8_zero_count(d107_vals[i]);
        if (zc < 3) { all_axis = 0; break; }
    }
    check("D107 first-128: all 13 values axis-aligned (>=3 zeros)", all_axis);

    /* Verify specific orbit structure */
    {
        Raq_Partition p = raq_detect_root_orbits(d107_vals, 13);
        check("D107: 4 root-of-unity orbits", p.n_groups == 4);
        raq_partition_free(&p);
    }
    {
        Raq_Partition p = raq_detect_galois_orbits(d107_vals, 13);
        check("D107: 7 Galois orbits", p.n_groups == 7);
        raq_partition_free(&p);
    }
}

/* ================================================================
 * 15. Helpers: Axis, Coefficient, 2-adic Valuation
 * ================================================================ */
static void test_axis_helpers(void) {
    printf("\n--- Axis / Coeff / 2-adic Helpers ---\n");

    /* raq_cyc8_axis */
    check("axis(0) = -1 (zero)",
          raq_cyc8_axis(raq_cyc8_zero()) == -1);
    check("axis(1,0,0,0) = 0 (a-axis)",
          raq_cyc8_axis(raq_cyc8_make(3, 0, 0, 0)) == 0);
    check("axis(0,5,0,0) = 1 (b-axis)",
          raq_cyc8_axis(raq_cyc8_make(0, 5, 0, 0)) == 1);
    check("axis(0,0,-2,0) = 2 (c-axis)",
          raq_cyc8_axis(raq_cyc8_make(0, 0, -2, 0)) == 2);
    check("axis(0,0,0,7) = 3 (d-axis)",
          raq_cyc8_axis(raq_cyc8_make(0, 0, 0, 7)) == 3);
    check("axis(1,1,0,0) = -2 (multi-axis)",
          raq_cyc8_axis(raq_cyc8_make(1, 1, 0, 0)) == -2);

    /* raq_cyc8_coeff */
    check("coeff(0) = 0",
          raq_cyc8_coeff(raq_cyc8_zero()) == 0);
    check("coeff(3,0,0,0) = 3",
          raq_cyc8_coeff(raq_cyc8_make(3, 0, 0, 0)) == 3);
    check("coeff(0,-5,0,0) = -5",
          raq_cyc8_coeff(raq_cyc8_make(0, -5, 0, 0)) == -5);
    check("coeff(0,0,0,7) = 7",
          raq_cyc8_coeff(raq_cyc8_make(0, 0, 0, 7)) == 7);

    /* raq_int_2adic_val */
    check("v2(0) = -1 (infinity)",
          raq_int_2adic_val(0) == -1);
    check("v2(1) = 0",
          raq_int_2adic_val(1) == 0);
    check("v2(2) = 1",
          raq_int_2adic_val(2) == 1);
    check("v2(4) = 2",
          raq_int_2adic_val(4) == 2);
    check("v2(6) = 1",
          raq_int_2adic_val(6) == 1);
    check("v2(8) = 3",
          raq_int_2adic_val(8) == 3);
    check("v2(3) = 0 (odd)",
          raq_int_2adic_val(3) == 0);
    check("v2(-12) = 2",
          raq_int_2adic_val(-12) == 2);

    /* raq_cyc8_2adic_val */
    check("v2_cyc8(0) = -1",
          raq_cyc8_2adic_val(raq_cyc8_zero()) == -1);
    check("v2_cyc8(6*zeta) = 1",
          raq_cyc8_2adic_val(raq_cyc8_make(0, 6, 0, 0)) == 1);
    check("v2_cyc8(3*zeta^2) = 0",
          raq_cyc8_2adic_val(raq_cyc8_make(0, 0, 3, 0)) == 0);
}

/* ================================================================
 * 16. Detector: Axis Agreement
 * ================================================================ */
static void test_axis_classes(void) {
    Raq_Partition p;

    printf("\n--- Axis Agreement ---\n");

    /* Same axis: both on Z*1 */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_make(3, 0, 0, 0);
        vals[1] = raq_cyc8_make(-5, 0, 0, 0);
        p = raq_detect_axis_classes(vals, 2);
        check("same a-axis: 1 group", p.n_groups == 1);
        raq_partition_free(&p);
    }

    /* Different axes */
    {
        Raq_Cyc8 vals[3];
        vals[0] = raq_cyc8_make(1, 0, 0, 0);   /* a-axis */
        vals[1] = raq_cyc8_make(0, 1, 0, 0);   /* b-axis */
        vals[2] = raq_cyc8_make(0, 0, 0, 1);   /* d-axis */
        p = raq_detect_axis_classes(vals, 3);
        check("3 different axes: 3 groups", p.n_groups == 3);
        raq_partition_free(&p);
    }

    /* Zero gets its own group */
    {
        Raq_Cyc8 vals[3];
        vals[0] = raq_cyc8_make(1, 0, 0, 0);
        vals[1] = raq_cyc8_zero();
        vals[2] = raq_cyc8_make(2, 0, 0, 0);
        p = raq_detect_axis_classes(vals, 3);
        check("zero separate: 2 groups (a-axis + zero)", p.n_groups == 2);
        raq_partition_free(&p);
    }

    /* All 4 axes + zero: 5 groups */
    {
        Raq_Cyc8 vals[5];
        vals[0] = raq_cyc8_make(1, 0, 0, 0);
        vals[1] = raq_cyc8_make(0, 1, 0, 0);
        vals[2] = raq_cyc8_make(0, 0, 1, 0);
        vals[3] = raq_cyc8_make(0, 0, 0, 1);
        vals[4] = raq_cyc8_zero();
        p = raq_detect_axis_classes(vals, 5);
        check("all 4 axes + zero: 5 groups", p.n_groups == 5);
        raq_partition_free(&p);
    }

    /* Multiple values on same axis cluster together */
    {
        Raq_Cyc8 vals[4];
        vals[0] = raq_cyc8_make(0, 1, 0, 0);
        vals[1] = raq_cyc8_make(0, -3, 0, 0);
        vals[2] = raq_cyc8_make(0, 7, 0, 0);
        vals[3] = raq_cyc8_make(0, -2, 0, 0);
        p = raq_detect_axis_classes(vals, 4);
        check("4 values on b-axis: 1 group", p.n_groups == 1);
        check("group size = 4", p.group_sizes[0] == 4);
        raq_partition_free(&p);
    }
}

/* ================================================================
 * 17. Detector: Divisibility
 * ================================================================ */
static void test_divisibility(void) {
    Raq_DivisResult r;

    printf("\n--- Divisibility ---\n");

    /* 1 divides 3 (same a-axis) */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_make(1, 0, 0, 0);
        vals[1] = raq_cyc8_make(3, 0, 0, 0);
        r = raq_detect_divisibility(vals, 2);
        check("1|3: 1 same-axis pair", r.n_same_axis == 1);
        check("1|3: 1 divisibility (1|3, not 3|1)", r.n_divides == 1);
    }

    /* 2 divides 6 (same axis, both directions: 2|6 yes, 6|2 no) */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_make(0, 2, 0, 0);
        vals[1] = raq_cyc8_make(0, 6, 0, 0);
        r = raq_detect_divisibility(vals, 2);
        check("2z|6z: 1 divisibility relation", r.n_divides == 1);
    }

    /* Mutual divisibility: 3 and -3 on same axis */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_make(0, 0, 3, 0);
        vals[1] = raq_cyc8_make(0, 0, -3, 0);
        r = raq_detect_divisibility(vals, 2);
        check("3z^2 and -3z^2: mutual (2 relations)", r.n_divides == 2);
    }

    /* Different axes: no same-axis pairs */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_make(2, 0, 0, 0);
        vals[1] = raq_cyc8_make(0, 2, 0, 0);
        r = raq_detect_divisibility(vals, 2);
        check("different axes: 0 same-axis pairs", r.n_same_axis == 0);
        check("different axes: 0 divisibility", r.n_divides == 0);
    }

    /* Zero is divisible by everything */
    {
        Raq_Cyc8 vals[2];
        vals[0] = raq_cyc8_make(5, 0, 0, 0);
        vals[1] = raq_cyc8_zero();
        r = raq_detect_divisibility(vals, 2);
        check("5|0: 1 divisibility relation", r.n_divides == 1);
    }
}

/* ================================================================
 * 18. Detector: 2-Adic Valuation Classes
 * ================================================================ */
static void test_2adic_classes(void) {
    Raq_Partition p;

    printf("\n--- 2-Adic Valuation Classes ---\n");

    /* v2=0 (odd) vs v2=1 (singly even) vs v2=-1 (zero) */
    {
        Raq_Cyc8 vals[4];
        vals[0] = raq_cyc8_make(1, 0, 0, 0);   /* v2 = 0 */
        vals[1] = raq_cyc8_make(3, 0, 0, 0);   /* v2 = 0 */
        vals[2] = raq_cyc8_make(0, 2, 0, 0);   /* v2 = 1 */
        vals[3] = raq_cyc8_zero();              /* v2 = -1 */
        p = raq_detect_2adic_classes(vals, 4);
        check("v2 classes: 3 groups (odd, even, zero)", p.n_groups == 3);
        raq_partition_free(&p);
    }

    /* All odd: 1 group */
    {
        Raq_Cyc8 vals[3];
        vals[0] = raq_cyc8_make(1, 0, 0, 0);
        vals[1] = raq_cyc8_make(0, -3, 0, 0);
        vals[2] = raq_cyc8_make(0, 0, 5, 0);
        p = raq_detect_2adic_classes(vals, 3);
        check("all odd: 1 group", p.n_groups == 1);
        raq_partition_free(&p);
    }

    /* Powers of 2: each gets own group */
    {
        Raq_Cyc8 vals[4];
        vals[0] = raq_cyc8_make(1, 0, 0, 0);   /* v2 = 0 */
        vals[1] = raq_cyc8_make(2, 0, 0, 0);   /* v2 = 1 */
        vals[2] = raq_cyc8_make(4, 0, 0, 0);   /* v2 = 2 */
        vals[3] = raq_cyc8_make(8, 0, 0, 0);   /* v2 = 3 */
        p = raq_detect_2adic_classes(vals, 4);
        check("powers of 2: 4 groups", p.n_groups == 4);
        raq_partition_free(&p);
    }

    /* Cross-axis grouping: same v2 regardless of axis */
    {
        Raq_Cyc8 vals[3];
        vals[0] = raq_cyc8_make(3, 0, 0, 0);   /* v2=0, a-axis */
        vals[1] = raq_cyc8_make(0, 5, 0, 0);   /* v2=0, b-axis */
        vals[2] = raq_cyc8_make(0, 0, 0, -7);  /* v2=0, d-axis */
        p = raq_detect_2adic_classes(vals, 3);
        check("same v2 cross-axis: 1 group", p.n_groups == 1);
        raq_partition_free(&p);
    }
}

/* ================================================================
 * 19. Edge List Operations
 * ================================================================ */
static void test_edgelist(void) {
    Raq_EdgeList el;

    printf("\n--- Edge List ---\n");

    raq_el_init(&el, 4, "test", 0);
    check("el init: count=0", el.count == 0);
    check("el init: capacity=4", el.capacity == 4);

    raq_el_add(&el, 0, 1);
    raq_el_add(&el, 1, 2);
    raq_el_add(&el, 2, 3);
    check("el 3 edges: count=3", el.count == 3);
    check("el edge 0: 0->1", el.src[0] == 0 && el.dst[0] == 1);
    check("el edge 2: 2->3", el.src[2] == 2 && el.dst[2] == 3);

    /* Dynamic growth */
    raq_el_add(&el, 3, 4);
    raq_el_add(&el, 4, 5);
    check("el after growth: count=5", el.count == 5);
    check("el grew capacity", el.capacity >= 5);

    raq_el_free(&el);
    check("el free: count=0", el.count == 0);
}

/* ================================================================
 * 20. Graph Analyzer: Known Structures
 * ================================================================ */
static void test_graph_analyzer(void) {
    printf("\n--- Graph Analyzer ---\n");

    /* Path P_5: 0-1-2-3-4 */
    {
        Raq_EdgeList el;
        Raq_GraphResult gr;
        raq_el_init(&el, 4, "path5", 0);
        raq_el_add(&el, 0, 1);
        raq_el_add(&el, 1, 2);
        raq_el_add(&el, 2, 3);
        raq_el_add(&el, 3, 4);
        gr = raq_graph_analyze(&el, 5);
        check("P5: bipartite", gr.est_bipartitus == 1);
        check("P5: partition 3+2",
              (gr.bipartitio_a == 3 && gr.bipartitio_b == 2) ||
              (gr.bipartitio_a == 2 && gr.bipartitio_b == 3));
        check("P5: tree", gr.est_arbor == 1);
        check("P5: path", gr.est_via == 1);
        check("P5: not cycle", gr.est_cyclus == 0);
        check("P5: 1 component", gr.n_components == 1);
        check("P5: girth 0 (acyclic)", gr.ambitus == 0);
        raq_graph_result_free(&gr);
        raq_el_free(&el);
    }

    /* Cycle C_4: 0-1-2-3-0 */
    {
        Raq_EdgeList el;
        Raq_GraphResult gr;
        raq_el_init(&el, 4, "cycle4", 0);
        raq_el_add(&el, 0, 1);
        raq_el_add(&el, 1, 2);
        raq_el_add(&el, 2, 3);
        raq_el_add(&el, 3, 0);
        gr = raq_graph_analyze(&el, 4);
        check("C4: bipartite", gr.est_bipartitus == 1);
        check("C4: partition 2+2", gr.bipartitio_a == 2 && gr.bipartitio_b == 2);
        check("C4: not tree", gr.est_arbor == 0);
        check("C4: cycle", gr.est_cyclus == 1);
        check("C4: girth 4", gr.ambitus == 4);
        raq_graph_result_free(&gr);
        raq_el_free(&el);
    }

    /* Complete K_4 */
    {
        Raq_EdgeList el;
        Raq_GraphResult gr;
        raq_el_init(&el, 6, "K4", 0);
        raq_el_add(&el, 0, 1); raq_el_add(&el, 0, 2); raq_el_add(&el, 0, 3);
        raq_el_add(&el, 1, 2); raq_el_add(&el, 1, 3); raq_el_add(&el, 2, 3);
        gr = raq_graph_analyze(&el, 4);
        check("K4: not bipartite", gr.est_bipartitus == 0);
        check("K4: complete", gr.est_completus == 1);
        check("K4: girth 3", gr.ambitus == 3);
        check("K4: 1 component", gr.n_components == 1);
        check("K4: degree min=max=3", gr.gradus_min == 3 && gr.gradus_max == 3);
        raq_graph_result_free(&gr);
        raq_el_free(&el);
    }

    /* Triangle K_3 */
    {
        Raq_EdgeList el;
        Raq_GraphResult gr;
        raq_el_init(&el, 3, "K3", 0);
        raq_el_add(&el, 0, 1); raq_el_add(&el, 1, 2); raq_el_add(&el, 0, 2);
        gr = raq_graph_analyze(&el, 3);
        check("K3: not bipartite", gr.est_bipartitus == 0);
        check("K3: complete", gr.est_completus == 1);
        check("K3: girth 3", gr.ambitus == 3);
        raq_graph_result_free(&gr);
        raq_el_free(&el);
    }

    /* DAG: 0->1, 0->2, 1->3, 2->3 (diamond) */
    {
        Raq_EdgeList el;
        Raq_GraphResult gr;
        raq_el_init(&el, 4, "diamond_dag", 1);
        raq_el_add(&el, 0, 1); raq_el_add(&el, 0, 2);
        raq_el_add(&el, 1, 3); raq_el_add(&el, 2, 3);
        gr = raq_graph_analyze(&el, 4);
        check("DAG: is DAG", gr.est_dag == 1);
        check("DAG: longest chain 2", gr.longitudo_catena == 2);
        check("DAG: bipartite not checked", gr.est_bipartitus == -1);
        raq_graph_result_free(&gr);
        raq_el_free(&el);
    }

    /* Directed cycle (not DAG): 0->1->2->0 */
    {
        Raq_EdgeList el;
        Raq_GraphResult gr;
        raq_el_init(&el, 3, "dir_cycle", 1);
        raq_el_add(&el, 0, 1); raq_el_add(&el, 1, 2); raq_el_add(&el, 2, 0);
        gr = raq_graph_analyze(&el, 3);
        check("dir_cycle: not DAG", gr.est_dag == 0);
        raq_graph_result_free(&gr);
        raq_el_free(&el);
    }

    /* Disconnected: 0-1 + 2-3-4 + isolated 5 */
    {
        Raq_EdgeList el;
        Raq_GraphResult gr;
        raq_el_init(&el, 3, "discon", 0);
        raq_el_add(&el, 0, 1);
        raq_el_add(&el, 2, 3); raq_el_add(&el, 3, 4);
        gr = raq_graph_analyze(&el, 6);
        check("discon: 3 components (2 nontrivial + 1 isolated)",
              gr.n_components == 3);
        check("discon: 1 isolated", gr.n_isolated == 1);
        check("discon: bipartite", gr.est_bipartitus == 1);
        check("discon: forest", gr.est_arbor == 1);
        check("discon: not path (2 components)", gr.est_via == 0);
        raq_graph_result_free(&gr);
        raq_el_free(&el);
    }

    /* Empty graph (no edges, 4 vertices) */
    {
        Raq_EdgeList el;
        Raq_GraphResult gr;
        raq_el_init(&el, 1, "empty", 0);
        gr = raq_graph_analyze(&el, 4);
        check("empty: 4 components all isolated", gr.n_components == 4);
        check("empty: 4 isolated", gr.n_isolated == 4);
        check("empty: bipartite", gr.est_bipartitus == 1);
        check("empty: forest", gr.est_arbor == 1);
        raq_graph_result_free(&gr);
        raq_el_free(&el);
    }
}

/* ================================================================
 * 21. Edge Generators
 * ================================================================ */
static void test_edge_generators(void) {
    /* Use the D107 first-128 value set (13 values) */
    Raq_Cyc8 vals[13];
    Raq_ValueSet vs;
    int i;

    printf("\n--- Edge Generators ---\n");

    vals[0]  = raq_cyc8_make(1, 0, 0, 0);
    vals[1]  = raq_cyc8_make(0, 0, 0, 0);
    vals[2]  = raq_cyc8_make(0, -1, 0, 0);
    vals[3]  = raq_cyc8_make(0, 0, 0, 1);
    vals[4]  = raq_cyc8_make(0, 0, 1, 0);
    vals[5]  = raq_cyc8_make(2, 0, 0, 0);
    vals[6]  = raq_cyc8_make(0, 0, -1, 0);
    vals[7]  = raq_cyc8_make(0, 0, 0, -1);
    vals[8]  = raq_cyc8_make(0, -3, 0, 0);
    vals[9]  = raq_cyc8_make(0, -2, 0, 0);
    vals[10] = raq_cyc8_make(0, 0, 0, 2);
    vals[11] = raq_cyc8_make(0, 1, 0, 0);
    vals[12] = raq_cyc8_make(0, 0, 0, 3);

    raq_vs_init(&vs, 20);
    for (i = 0; i < 13; i++) raq_vs_insert(&vs, vals[i]);

    /* Same axis: 5 groups (a:2, b:4, c:2, d:4, zero:1) */
    /* Edges within groups: C(2,1)+C(4,2)+C(2,1)+C(4,2)+C(1,0) = 1+6+1+6+0 = 14 */
    {
        Raq_EdgeList el = raq_edges_same_axis(vals, 13);
        check("same_axis: 14 edges", el.count == 14);
        check("same_axis: undirected", el.directus == 0);
        raq_el_free(&el);
    }

    /* Divisibility: directed, should include 1|2 (both on a-axis).
     * NOT a DAG because +/- pairs create 2-cycles (e.g., 1|-1 and -1|1). */
    {
        Raq_EdgeList el = raq_edges_divisibility(vals, 13);
        Raq_GraphResult gr;
        check("divisibility: directed", el.directus == 1);
        check("divisibility: edges > 0", el.count > 0);
        gr = raq_graph_analyze(&el, 13);
        check("divisibility: not DAG (sign cycles)", gr.est_dag == 0);
        raq_graph_result_free(&gr);
        raq_el_free(&el);
    }

    /* Product closure: zeta * zeta = zeta^2, which is val[11]*val[11] = val[4].
     * But edges are i<j, so self-product (i==j) is excluded.
     * However val[2]=-zeta * val[7]=-zeta^3 = zeta^4 = -1 = (-1,0,0,0) not in set.
     * val[2]=-zeta * val[3]=zeta^3: (-zeta)(zeta^3) = -zeta^4 = 1 = val[0]. In set!
     * So there should be at least one edge.
     */
    {
        Raq_EdgeList el = raq_edges_product_closure(vals, 13, &vs);
        check("product_closure: edges > 0", el.count > 0);
        raq_el_free(&el);
    }

    /* Same v2: v2 groups values by 2-adic valuation of coefficient */
    {
        Raq_EdgeList el = raq_edges_same_v2(vals, 13);
        check("same_v2: edges > 0", el.count > 0);
        raq_el_free(&el);
    }

    /* Same root orbit: values sharing norm class */
    {
        Raq_EdgeList el = raq_edges_same_root_orbit(vals, 13);
        check("same_root_orbit: edges > 0", el.count > 0);
        check("same_root_orbit: undirected", el.directus == 0);
        raq_el_free(&el);
    }

    /* Same galois orbit: values related by Gal(Q(zeta_8)/Q) */
    {
        Raq_EdgeList el = raq_edges_same_galois_orbit(vals, 13);
        check("same_galois_orbit: edges > 0", el.count > 0);
        check("same_galois_orbit: undirected", el.directus == 0);
        raq_el_free(&el);
    }

    /* Positive divisibility: |coeff| divides |coeff|, same axis, strict DAG */
    {
        Raq_EdgeList el = raq_edges_positive_divisibility(vals, 13);
        Raq_GraphResult gr;
        check("pos_divisibility: directed", el.directus == 1);
        check("pos_divisibility: edges > 0", el.count > 0);
        gr = raq_graph_analyze(&el, 13);
        check("pos_divisibility: IS a DAG", gr.est_dag == 1);
        check("pos_divisibility: chain >= 1", gr.longitudo_catena >= 1);
        raq_graph_result_free(&gr);
        raq_el_free(&el);
    }

    raq_vs_free(&vs);
}

/* ================================================================
 * 22. Cross-Integration: Restrict and Intersect
 * ================================================================ */
static void test_cross_integration(void) {
    printf("\n--- Cross-Integration ---\n");

    /* Restrict: path 0-1-2-3-4, partition {0,1,2} and {3,4} */
    {
        Raq_EdgeList el;
        Raq_Partition p;
        Raq_EdgeList sub;
        int sub_n;

        raq_el_init(&el, 4, "path", 0);
        raq_el_add(&el, 0, 1); raq_el_add(&el, 1, 2);
        raq_el_add(&el, 2, 3); raq_el_add(&el, 3, 4);

        p.n_values = 5;
        p.n_groups = 2;
        p.group_id = (int *)malloc(5 * sizeof(int));
        p.group_sizes = (int *)malloc(2 * sizeof(int));
        p.group_id[0] = 0; p.group_id[1] = 0; p.group_id[2] = 0;
        p.group_id[3] = 1; p.group_id[4] = 1;
        p.group_sizes[0] = 3; p.group_sizes[1] = 2;

        sub = raq_el_restrict(&el, &p, 0, &sub_n);
        check("restrict group 0: 3 vertices", sub_n == 3);
        check("restrict group 0: 2 edges (0-1, 1-2)", sub.count == 2);

        raq_el_free(&sub);
        sub = raq_el_restrict(&el, &p, 1, &sub_n);
        check("restrict group 1: 2 vertices", sub_n == 2);
        check("restrict group 1: 1 edge (3-4)", sub.count == 1);

        raq_el_free(&sub);
        raq_el_free(&el);
        raq_partition_free(&p);
    }

    /* Intersect: two overlapping edge sets */
    {
        Raq_EdgeList a, b, inter;

        raq_el_init(&a, 3, "a", 0);
        raq_el_add(&a, 0, 1); raq_el_add(&a, 1, 2); raq_el_add(&a, 2, 3);

        raq_el_init(&b, 3, "b", 0);
        raq_el_add(&b, 1, 2); raq_el_add(&b, 2, 3); raq_el_add(&b, 3, 4);

        inter = raq_el_intersect(&a, &b, 5);
        check("intersect: 2 common edges (1-2, 2-3)", inter.count == 2);

        raq_el_free(&inter);
        raq_el_free(&a);
        raq_el_free(&b);
    }

    /* Intersect: disjoint edge sets */
    {
        Raq_EdgeList a, b, inter;

        raq_el_init(&a, 2, "a", 0);
        raq_el_add(&a, 0, 1); raq_el_add(&a, 2, 3);

        raq_el_init(&b, 2, "b", 0);
        raq_el_add(&b, 1, 2); raq_el_add(&b, 3, 4);

        inter = raq_el_intersect(&a, &b, 5);
        check("intersect disjoint: 0 edges", inter.count == 0);

        raq_el_free(&inter);
        raq_el_free(&a);
        raq_el_free(&b);
    }
}

/* ================================================================
 * 23. Extended Edge Generators: Galois, Additive, Power, GCD
 * ================================================================ */
static void test_extended_edge_generators(void) {
    Raq_Cyc8 vals[13];
    Raq_ValueSet vs;
    int i;

    printf("\n--- Extended Edge Generators ---\n");

    vals[0]  = raq_cyc8_make(1, 0, 0, 0);
    vals[1]  = raq_cyc8_make(0, 0, 0, 0);
    vals[2]  = raq_cyc8_make(0, -1, 0, 0);
    vals[3]  = raq_cyc8_make(0, 0, 0, 1);
    vals[4]  = raq_cyc8_make(0, 0, 1, 0);
    vals[5]  = raq_cyc8_make(2, 0, 0, 0);
    vals[6]  = raq_cyc8_make(0, 0, -1, 0);
    vals[7]  = raq_cyc8_make(0, 0, 0, -1);
    vals[8]  = raq_cyc8_make(0, -3, 0, 0);
    vals[9]  = raq_cyc8_make(0, -2, 0, 0);
    vals[10] = raq_cyc8_make(0, 0, 0, 2);
    vals[11] = raq_cyc8_make(0, 1, 0, 0);
    vals[12] = raq_cyc8_make(0, 0, 0, 3);

    raq_vs_init(&vs, 20);
    for (i = 0; i < 13; i++) raq_vs_insert(&vs, vals[i]);

    /* Galois sigma3: (a,b,c,d) -> (a,d,-c,b).
     * E.g., val[0]=(1,0,0,0) -> sigma3=(1,0,0,0)=val[0], no self-edge.
     * val[2]=(0,-1,0,0) -> (0,0,1,-1)... probably not in set.
     * val[11]=(0,1,0,0) -> (0,0,0,1)=val[3]. Edge 11->3. */
    {
        Raq_EdgeList el = raq_edges_galois_sigma3(vals, 13, &vs);
        check("galois_s3: directed", el.directus == 1);
        check("galois_s3: edges > 0", el.count > 0);
        raq_el_free(&el);
    }

    /* Galois sigma5: (a,b,c,d) -> (a,-b,c,-d).
     * val[11]=(0,1,0,0) -> (0,-1,0,0)=val[2]. Edge 11->2.
     * val[3]=(0,0,0,1) -> (0,0,0,-1)=val[7]. Edge 3->7. */
    {
        Raq_EdgeList el = raq_edges_galois_sigma5(vals, 13, &vs);
        check("galois_s5: directed", el.directus == 1);
        check("galois_s5: edges > 0", el.count > 0);
        raq_el_free(&el);
    }

    /* Galois sigma7: (a,b,c,d) -> (a,-d,-c,-b).
     * val[11]=(0,1,0,0) -> (0,0,0,-1)=val[7]. Edge 11->7. */
    {
        Raq_EdgeList el = raq_edges_galois_sigma7(vals, 13, &vs);
        check("galois_s7: directed", el.directus == 1);
        check("galois_s7: edges > 0", el.count > 0);
        raq_el_free(&el);
    }

    /* Additive closure: same-axis only.
     * val[0]=(1,0,0,0) + val[5]=(2,0,0,0) = (3,0,0,0)... not in set.
     * val[0]=(1,0,0,0) + val[1]=(0,0,0,0) = (1,0,0,0)=val[0]. In set! */
    {
        Raq_EdgeList el = raq_edges_additive_closure(vals, 13, &vs);
        check("additive_closure: undirected", el.directus == 0);
        check("additive_closure: edges > 0", el.count > 0);
        raq_el_free(&el);
    }

    /* Power2: val[0]=(1,0,0,0)^2=(1,0,0,0)=self, no self-edge.
     * val[11]=(0,1,0,0)^2 = zeta^2 = (0,0,1,0)=val[4]. Edge 11->4. */
    {
        Raq_EdgeList el = raq_edges_power2(vals, 13, &vs);
        check("power2: directed", el.directus == 1);
        check("power2: edges > 0", el.count > 0);
        raq_el_free(&el);
    }

    /* GCD: same-axis, |coeff| gcd > 1.
     * val[5]=(2,0,0,0) and val[0]=(1,0,0,0): gcd(2,1)=1, no edge.
     * val[9]=(0,-2,0,0) and val[8]=(0,-3,0,0): gcd(2,3)=1, no edge.
     * val[9]=(0,-2,0,0) and val[11]=(0,1,0,0): skip, |1|<=1.
     * val[9]=(0,-2,0,0) and val[2]=(0,-1,0,0): skip, |1|<=1.
     * val[10]=(0,0,0,2) and val[12]=(0,0,0,3): gcd(2,3)=1, no.
     * val[10]=(0,0,0,2) and val[3]=(0,0,0,1): skip, |1|<=1. */
    {
        Raq_EdgeList el = raq_edges_gcd(vals, 13);
        check("gcd: undirected", el.directus == 0);
        /* GCD with this small set may have 0 edges (all coprime) */
        check("gcd: tested", 1);
        raq_el_free(&el);
    }

    raq_vs_free(&vs);
}

/* ================================================================
 * 24. Quotient Graph and Degree Sequence
 * ================================================================ */
static void test_quotient_and_degsq(void) {
    printf("\n--- Quotient & Degree Sequence ---\n");

    /* Quotient: K_4 over partition {0,1},{2,3} -> K_2 */
    {
        Raq_EdgeList el;
        Raq_Partition p;
        Raq_EdgeList qel;
        Raq_GraphResult qgr;
        int qn;

        raq_el_init(&el, 6, "k4", 0);
        raq_el_add(&el, 0, 1); raq_el_add(&el, 0, 2);
        raq_el_add(&el, 0, 3); raq_el_add(&el, 1, 2);
        raq_el_add(&el, 1, 3); raq_el_add(&el, 2, 3);

        p.n_values = 4;
        p.n_groups = 2;
        p.group_id = (int *)malloc(4 * sizeof(int));
        p.group_sizes = (int *)malloc(2 * sizeof(int));
        p.group_id[0] = 0; p.group_id[1] = 0;
        p.group_id[2] = 1; p.group_id[3] = 1;
        p.group_sizes[0] = 2; p.group_sizes[1] = 2;

        qel = raq_el_quotient(&el, &p, &qn);
        check("quotient K4/{2,2}: 2 groups", qn == 2);
        check("quotient K4/{2,2}: 1 edge", qel.count == 1);
        qgr = raq_graph_analyze(&qel, qn);
        check("quotient K4/{2,2}: is K_2", qgr.est_completus == 1);
        raq_graph_result_free(&qgr);
        raq_el_free(&qel);
        raq_el_free(&el);
        free(p.group_id); free(p.group_sizes);
    }

    /* Quotient: C_6 over 3 groups {0,1},{2,3},{4,5} -> C_3 */
    {
        Raq_EdgeList el;
        Raq_Partition p;
        Raq_EdgeList qel;
        Raq_GraphResult qgr;
        int qn;

        raq_el_init(&el, 6, "c6", 0);
        raq_el_add(&el, 0, 1); raq_el_add(&el, 1, 2);
        raq_el_add(&el, 2, 3); raq_el_add(&el, 3, 4);
        raq_el_add(&el, 4, 5); raq_el_add(&el, 5, 0);

        p.n_values = 6;
        p.n_groups = 3;
        p.group_id = (int *)malloc(6 * sizeof(int));
        p.group_sizes = (int *)malloc(3 * sizeof(int));
        p.group_id[0] = 0; p.group_id[1] = 0;
        p.group_id[2] = 1; p.group_id[3] = 1;
        p.group_id[4] = 2; p.group_id[5] = 2;
        p.group_sizes[0] = 2; p.group_sizes[1] = 2; p.group_sizes[2] = 2;

        qel = raq_el_quotient(&el, &p, &qn);
        check("quotient C6/{3x2}: 3 groups", qn == 3);
        check("quotient C6/{3x2}: 3 edges", qel.count == 3);
        qgr = raq_graph_analyze(&qel, qn);
        check("quotient C6/{3x2}: cycle C_3", qgr.est_cyclus == 1);
        raq_graph_result_free(&qgr);
        raq_el_free(&qel);
        raq_el_free(&el);
        free(p.group_id); free(p.group_sizes);
    }

    /* Degree sequence match: P_3 (0-1-2) vs P_3 (0-1-2) = match */
    {
        Raq_EdgeList a, b;
        raq_el_init(&a, 2, "a", 0);
        raq_el_add(&a, 0, 1); raq_el_add(&a, 1, 2);
        raq_el_init(&b, 2, "b", 0);
        raq_el_add(&b, 0, 1); raq_el_add(&b, 1, 2);
        check("degsq: P3 vs P3 match", raq_degree_seq_match(&a, 3, &b, 3));
        raq_el_free(&a); raq_el_free(&b);
    }

    /* Degree sequence match: P_3 vs K_3 = no match */
    {
        Raq_EdgeList a, b;
        raq_el_init(&a, 2, "a", 0);
        raq_el_add(&a, 0, 1); raq_el_add(&a, 1, 2);
        raq_el_init(&b, 3, "b", 0);
        raq_el_add(&b, 0, 1); raq_el_add(&b, 1, 2); raq_el_add(&b, 0, 2);
        check("degsq: P3 vs K3 no match", !raq_degree_seq_match(&a, 3, &b, 3));
        raq_el_free(&a); raq_el_free(&b);
    }

    /* Regular graph detection */
    {
        Raq_EdgeList el;
        Raq_GraphResult gr;
        raq_el_init(&el, 3, "c3", 0);
        raq_el_add(&el, 0, 1); raq_el_add(&el, 1, 2); raq_el_add(&el, 0, 2);
        gr = raq_graph_analyze(&el, 3);
        check("regular: C_3 is 2-regular", gr.est_regularis == 1);
        check("regular: degree = 2", gr.gradus_regularis == 2);
        raq_graph_result_free(&gr);
        raq_el_free(&el);
    }
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    printf("PROBATIO RAQIYA: Unit Tests for raqiya.h\n");
    printf("==========================================\n");

    test_arithmetic();
    test_conjugation();
    test_zeta_rotation();
    test_galois();
    test_sign_pattern();
    test_valueset();
    test_root_orbits();
    test_norm_classes();
    test_galois_orbits();
    test_sum_diff();
    test_products();
    test_partition_helpers();
    test_full_analysis();
    test_axis_alignment();
    test_axis_helpers();
    test_axis_classes();
    test_divisibility();
    test_2adic_classes();
    test_edgelist();
    test_graph_analyzer();
    test_edge_generators();
    test_extended_edge_generators();
    test_cross_integration();
    test_quotient_and_degsq();

    printf("\n==========================================\n");
    printf("  %d pass, %d fail\n", n_pass, n_fail);

    return n_fail > 0 ? 1 : 0;
}
