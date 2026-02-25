/*
 * raqiya.h -- Algebraic Graph Density Analysis for Z[zeta_8]
 * ==========================================================
 *
 * Single-header C89 library. Analyzes relationships between elements
 * of the cyclotomic ring Z[zeta_8] = Z[e^{2*pi*i/8}].
 *
 * Nine relationship detectors:
 *   1. Root-of-unity orbits (+/- zeta_8^k associates)
 *   2. Sign pattern distance (ternary Hamming)
 *   3. Sum zero-component count (cancellation structure)
 *   4. Norm-squared classes (|z|^2 grouping)
 *   5. Product structure (real/pure-imaginary products)
 *   6. Galois orbit membership (Gal(Q(zeta_8)/Q) action)
 *   7. Axis agreement (same zeta-power partition)
 *   8. Divisibility (same-axis integer divisibility partial order)
 *   9. 2-adic valuation classes (v_2 of integer coefficient)
 *
 * Usage: all functions are static. Include this header in your .c file.
 *
 * Name: Hebrew "raqiya" = firmament (Genesis 1:6-8).
 * The firmament separates waters above from waters below --
 * this tool separates algebraic structure from noise.
 *
 * C89, zero dependencies beyond stdlib.h/string.h.
 */

#ifndef RAQIYA_H
#define RAQIYA_H

#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Cyc8 -- Exact arithmetic in Z[zeta_8]
 *
 * Basis: {1, zeta_8, zeta_8^2, zeta_8^3} with zeta_8^4 = -1.
 * Components: a + b*zeta + c*zeta^2 + d*zeta^3
 * ================================================================ */

typedef struct {
    long a, b, c, d;
} Raq_Cyc8;

static Raq_Cyc8 raq_cyc8_make(long a, long b, long c, long d) {
    Raq_Cyc8 z; z.a = a; z.b = b; z.c = c; z.d = d; return z;
}

static Raq_Cyc8 raq_cyc8_zero(void) { return raq_cyc8_make(0, 0, 0, 0); }
static Raq_Cyc8 raq_cyc8_one(void)  { return raq_cyc8_make(1, 0, 0, 0); }

static Raq_Cyc8 raq_cyc8_add(Raq_Cyc8 x, Raq_Cyc8 y) {
    return raq_cyc8_make(x.a + y.a, x.b + y.b, x.c + y.c, x.d + y.d);
}

static Raq_Cyc8 raq_cyc8_neg(Raq_Cyc8 x) {
    return raq_cyc8_make(-x.a, -x.b, -x.c, -x.d);
}

static Raq_Cyc8 raq_cyc8_sub(Raq_Cyc8 x, Raq_Cyc8 y) {
    return raq_cyc8_add(x, raq_cyc8_neg(y));
}

/* Multiplication in Z[zeta_8] using zeta^4 = -1:
 *   (a+bz+cz^2+dz^3)(e+fz+gz^2+hz^3)
 *   constant: ae - bh - cg - df
 *   z:        af + be - ch - dg
 *   z^2:      ag + bf + ce - dh
 *   z^3:      ah + bg + cf + de
 */
static Raq_Cyc8 raq_cyc8_mul(Raq_Cyc8 x, Raq_Cyc8 y) {
    return raq_cyc8_make(
        x.a * y.a - x.b * y.d - x.c * y.c - x.d * y.b,
        x.a * y.b + x.b * y.a - x.c * y.d - x.d * y.c,
        x.a * y.c + x.b * y.b + x.c * y.a - x.d * y.d,
        x.a * y.d + x.b * y.c + x.c * y.b + x.d * y.a
    );
}

static int raq_cyc8_eq(Raq_Cyc8 x, Raq_Cyc8 y) {
    return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}

static int raq_cyc8_is_zero(Raq_Cyc8 x) {
    return x.a == 0 && x.b == 0 && x.c == 0 && x.d == 0;
}

/* Complex conjugation: zeta_8 -> zeta_8^{-1} = zeta_8^7 = -zeta_8^3
 * conj(a + b*z + c*z^2 + d*z^3) = a + b*z^7 + c*z^6 + d*z^5
 *   = a + b*(-z^3) + c*(-z^2) + d*(-z) = (a, -d, -c, -b)
 */
static Raq_Cyc8 raq_cyc8_conj(Raq_Cyc8 z) {
    return raq_cyc8_make(z.a, -z.d, -z.c, -z.b);
}

/* Norm squared: z * conj(z) = |z|^2.
 * Result lives in Z[sqrt(2)] (maximal real subfield):
 * c = 0 and d = -b, i.e. form (a, b, 0, -b) = a + b*sqrt(2).
 */
static Raq_Cyc8 raq_cyc8_norm_sq(Raq_Cyc8 z) {
    return raq_cyc8_mul(z, raq_cyc8_conj(z));
}

/* Multiply by zeta_8: (a,b,c,d) -> (-d, a, b, c)
 * zeta * (a+bz+cz^2+dz^3) = az+bz^2+cz^3+dz^4 = -d + az + bz^2 + cz^3
 */
static Raq_Cyc8 raq_cyc8_mul_zeta(Raq_Cyc8 z) {
    return raq_cyc8_make(-z.d, z.a, z.b, z.c);
}

/* Galois automorphisms: Gal(Q(zeta_8)/Q) = Z/2 x Z/2
 *
 * sigma_3: zeta -> zeta^3.  (a,b,c,d) -> (a, d, -c, b)
 * sigma_5: zeta -> -zeta.   (a,b,c,d) -> (a, -b, c, -d)
 * sigma_7: zeta -> -zeta^3. (a,b,c,d) -> (a, -d, -c, -b)
 *
 * sigma_7 = sigma_5 o sigma_3.
 * sigma_7 = complex conjugation (the standard embedding).
 */
static Raq_Cyc8 raq_cyc8_galois_3(Raq_Cyc8 z) {
    return raq_cyc8_make(z.a, z.d, -z.c, z.b);
}

static Raq_Cyc8 raq_cyc8_galois_5(Raq_Cyc8 z) {
    return raq_cyc8_make(z.a, -z.b, z.c, -z.d);
}

static Raq_Cyc8 raq_cyc8_galois_7(Raq_Cyc8 z) {
    return raq_cyc8_make(z.a, -z.d, -z.c, -z.b);
}

/* Count zero components (0-4) */
static int raq_cyc8_zero_count(Raq_Cyc8 z) {
    int cnt = 0;
    if (z.a == 0) cnt++;
    if (z.b == 0) cnt++;
    if (z.c == 0) cnt++;
    if (z.d == 0) cnt++;
    return cnt;
}

/* Sign distance (ternary Hamming on sign vectors).
 * sign(x) in {-, 0, +} for each component. Distance = count of mismatches.
 */
static int raq_cyc8_sign_distance(Raq_Cyc8 x, Raq_Cyc8 y) {
    int d = 0;
    long xv[4], yv[4];
    int i;
    xv[0] = x.a; xv[1] = x.b; xv[2] = x.c; xv[3] = x.d;
    yv[0] = y.a; yv[1] = y.b; yv[2] = y.c; yv[3] = y.d;
    for (i = 0; i < 4; i++) {
        int sx = (xv[i] > 0 ? 2 : (xv[i] < 0 ? 0 : 1));
        int sy = (yv[i] > 0 ? 2 : (yv[i] < 0 ? 0 : 1));
        if (sx != sy) d++;
    }
    return d;
}

/* Ternary sign pattern as integer in [0, 80] (base-3 encoding) */
static int raq_cyc8_sign_pattern(Raq_Cyc8 z) {
    long v[4];
    int pat = 0, i;
    v[0] = z.a; v[1] = z.b; v[2] = z.c; v[3] = z.d;
    for (i = 0; i < 4; i++)
        pat = pat * 3 + (v[i] > 0 ? 2 : (v[i] < 0 ? 0 : 1));
    return pat;
}

/* Axis identification for axis-aligned values.
 * Returns: 0=a-axis, 1=b-axis, 2=c-axis, 3=d-axis,
 *          -1=zero, -2=multi-axis (not axis-aligned).
 */
static int raq_cyc8_axis(Raq_Cyc8 z) {
    int nonzero = 0, axis = -1;
    if (z.a != 0) { nonzero++; axis = 0; }
    if (z.b != 0) { nonzero++; axis = 1; }
    if (z.c != 0) { nonzero++; axis = 2; }
    if (z.d != 0) { nonzero++; axis = 3; }
    if (nonzero == 0) return -1;
    if (nonzero > 1)  return -2;
    return axis;
}

/* Integer coefficient for axis-aligned values.
 * Returns the nonzero component value, 0 for zero.
 * Undefined for multi-axis values.
 */
static long raq_cyc8_coeff(Raq_Cyc8 z) {
    if (z.a != 0) return z.a;
    if (z.b != 0) return z.b;
    if (z.c != 0) return z.c;
    if (z.d != 0) return z.d;
    return 0;
}

/* 2-adic valuation of an integer.
 * v_2(n) = max power of 2 dividing n.
 * Returns -1 for n=0 (convention for infinity).
 */
static int raq_int_2adic_val(long n) {
    int v = 0;
    if (n == 0) return -1;
    if (n < 0) n = -n;
    while (n % 2 == 0) { v++; n /= 2; }
    return v;
}

/* 2-adic valuation of a Cyc8 value's integer coefficient.
 * For axis-aligned values: v_2(coeff). For zero: -1.
 * For multi-axis: minimum v_2 across nonzero components.
 */
static int raq_cyc8_2adic_val(Raq_Cyc8 z) {
    int ax = raq_cyc8_axis(z);
    if (ax == -1) return -1;  /* zero */
    return raq_int_2adic_val(raq_cyc8_coeff(z));
}

/* Hash a Cyc8 value */
static unsigned long raq_cyc8_hash(Raq_Cyc8 z) {
    unsigned long h = 5381;
    h = h * 31 + (unsigned long)(z.a + 1000000);
    h = h * 31 + (unsigned long)(z.b + 1000000);
    h = h * 31 + (unsigned long)(z.c + 1000000);
    h = h * 31 + (unsigned long)(z.d + 1000000);
    return h;
}

/* ================================================================
 * Raq_ValueSet -- Hash-based distinct value collection
 * ================================================================ */

typedef struct {
    Raq_Cyc8 *values;
    int count;
    int capacity;
    int *hash_head;
    int *hash_next;
    int hash_size;
} Raq_ValueSet;

static void raq_vs_init(Raq_ValueSet *vs, int capacity) {
    int i;
    /* Choose a prime hash size > 2*capacity */
    int primes[] = {127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749};
    int n_primes = 9;
    vs->hash_size = primes[n_primes - 1];
    for (i = 0; i < n_primes; i++) {
        if (primes[i] > 2 * capacity) {
            vs->hash_size = primes[i];
            break;
        }
    }

    vs->capacity = capacity;
    vs->count = 0;
    vs->values = (Raq_Cyc8 *)calloc((size_t)capacity, sizeof(Raq_Cyc8));
    vs->hash_head = (int *)malloc((size_t)vs->hash_size * sizeof(int));
    vs->hash_next = (int *)malloc((size_t)capacity * sizeof(int));

    for (i = 0; i < vs->hash_size; i++) vs->hash_head[i] = -1;
    for (i = 0; i < capacity; i++) vs->hash_next[i] = -1;
}

static void raq_vs_free(Raq_ValueSet *vs) {
    free(vs->values);
    free(vs->hash_head);
    free(vs->hash_next);
    vs->values = NULL;
    vs->count = 0;
}

static int raq_vs_find(const Raq_ValueSet *vs, Raq_Cyc8 z) {
    int bucket = (int)(raq_cyc8_hash(z) % (unsigned long)vs->hash_size);
    int idx = vs->hash_head[bucket];
    while (idx >= 0) {
        if (raq_cyc8_eq(vs->values[idx], z)) return idx;
        idx = vs->hash_next[idx];
    }
    return -1;
}

/* Returns index of inserted value, or existing index if duplicate.
 * Returns -1 if at capacity.
 */
static int raq_vs_insert(Raq_ValueSet *vs, Raq_Cyc8 z) {
    int bucket, idx;
    idx = raq_vs_find(vs, z);
    if (idx >= 0) return idx;
    if (vs->count >= vs->capacity) return -1;
    bucket = (int)(raq_cyc8_hash(z) % (unsigned long)vs->hash_size);
    idx = vs->count;
    vs->values[idx] = z;
    vs->hash_next[idx] = vs->hash_head[bucket];
    vs->hash_head[bucket] = idx;
    vs->count++;
    return idx;
}

/* ================================================================
 * Raq_Partition -- Group assignment result
 *
 * Used by: root-of-unity orbits, norm-squared classes, Galois orbits.
 * group_id[i] = which group value i belongs to.
 * group_sizes[g] = number of values in group g.
 * ================================================================ */

typedef struct {
    int *group_id;
    int *group_sizes;
    int n_groups;
    int n_values;
} Raq_Partition;

static void raq_partition_free(Raq_Partition *p) {
    free(p->group_id);
    free(p->group_sizes);
    p->group_id = NULL;
    p->group_sizes = NULL;
}

/* Count pairs within the same group: sum of C(size, 2) */
static int raq_partition_related_pairs(const Raq_Partition *p) {
    int count = 0, i;
    for (i = 0; i < p->n_groups; i++)
        count += p->group_sizes[i] * (p->group_sizes[i] - 1) / 2;
    return count;
}

/* Max group size */
static int raq_partition_max_size(const Raq_Partition *p) {
    int mx = 0, i;
    for (i = 0; i < p->n_groups; i++)
        if (p->group_sizes[i] > mx) mx = p->group_sizes[i];
    return mx;
}

/* ================================================================
 * Raq_PairHist -- Pairwise analysis histogram (distance 0-4)
 * ================================================================ */

typedef struct {
    int bins[5];
    int n_pairs;
    long total;
} Raq_PairHist;

/* ================================================================
 * Raq_ProductResult -- Product structure analysis
 * ================================================================ */

typedef struct {
    Raq_PairHist zero_components;
    int n_real;   /* products with b=d=0 (in Z[sqrt(2)]) */
    int n_imag;   /* products with a=c=0 */
} Raq_ProductResult;

/* ================================================================
 * Detector 1: Root-of-Unity Orbits
 *
 * Groups values by the relation y = +/- zeta_8^k * x
 * for k in {0,...,7}. Max orbit size = 16 (8 rotations * 2 signs),
 * but effectively 8 since zeta^4 = -1 makes +z^4 = -1.
 * ================================================================ */

static Raq_Partition raq_detect_root_orbits(const Raq_Cyc8 *values, int n) {
    Raq_Partition p;
    int i, oi, oj, k;

    p.n_values = n;
    p.n_groups = 0;
    p.group_id = (int *)calloc((size_t)n, sizeof(int));
    p.group_sizes = (int *)calloc((size_t)n, sizeof(int));
    for (i = 0; i < n; i++) p.group_id[i] = -1;

    for (oi = 0; oi < n; oi++) {
        Raq_Cyc8 rotated;
        if (p.group_id[oi] >= 0) continue;
        p.group_id[oi] = p.n_groups;

        rotated = values[oi];
        for (k = 0; k < 8; k++) {
            Raq_Cyc8 neg_rot = raq_cyc8_neg(rotated);
            for (oj = oi + 1; oj < n; oj++) {
                if (p.group_id[oj] >= 0) continue;
                if (raq_cyc8_eq(values[oj], rotated) ||
                    raq_cyc8_eq(values[oj], neg_rot))
                    p.group_id[oj] = p.n_groups;
            }
            rotated = raq_cyc8_mul_zeta(rotated);
        }
        p.n_groups++;
    }

    /* Compute sizes */
    for (i = 0; i < n; i++)
        p.group_sizes[p.group_id[i]]++;

    return p;
}

/* ================================================================
 * Detector 2: Sign Pattern Distance
 * ================================================================ */

static Raq_PairHist raq_detect_sign_distances(const Raq_Cyc8 *values, int n) {
    Raq_PairHist h;
    int oi, oj;

    memset(&h, 0, sizeof(h));
    h.n_pairs = n * (n - 1) / 2;

    for (oi = 0; oi < n; oi++)
        for (oj = oi + 1; oj < n; oj++) {
            int d = raq_cyc8_sign_distance(values[oi], values[oj]);
            h.bins[d]++;
            h.total += d;
        }

    return h;
}

/* Count distinct sign patterns in the value set */
static int raq_count_sign_patterns(const Raq_Cyc8 *values, int n) {
    int seen[81]; /* 3^4 = 81 possible patterns */
    int count = 0, i;

    memset(seen, 0, sizeof(seen));
    for (i = 0; i < n; i++) {
        int pat = raq_cyc8_sign_pattern(values[i]);
        seen[pat] = 1;
    }
    for (i = 0; i < 81; i++)
        if (seen[i]) count++;
    return count;
}

/* ================================================================
 * Detector 3: Sum/Difference Zero-Component Count
 * ================================================================ */

static Raq_PairHist raq_detect_sum_zeros(const Raq_Cyc8 *values, int n) {
    Raq_PairHist h;
    int oi, oj;

    memset(&h, 0, sizeof(h));
    h.n_pairs = n * (n - 1) / 2;

    for (oi = 0; oi < n; oi++)
        for (oj = oi + 1; oj < n; oj++) {
            Raq_Cyc8 s = raq_cyc8_add(values[oi], values[oj]);
            int zc = raq_cyc8_zero_count(s);
            h.bins[zc]++;
            h.total += zc;
        }

    return h;
}

static Raq_PairHist raq_detect_diff_zeros(const Raq_Cyc8 *values, int n) {
    Raq_PairHist h;
    int oi, oj;

    memset(&h, 0, sizeof(h));
    h.n_pairs = n * (n - 1) / 2;

    for (oi = 0; oi < n; oi++)
        for (oj = oi + 1; oj < n; oj++) {
            Raq_Cyc8 d = raq_cyc8_sub(values[oi], values[oj]);
            int zc = raq_cyc8_zero_count(d);
            h.bins[zc]++;
            h.total += zc;
        }

    return h;
}

/* Count pairs that sum to zero (negatives of each other) */
static int raq_count_neg_pairs(const Raq_Cyc8 *values, int n) {
    int count = 0, oi, oj;
    for (oi = 0; oi < n; oi++)
        for (oj = oi + 1; oj < n; oj++)
            if (raq_cyc8_is_zero(raq_cyc8_add(values[oi], values[oj])))
                count++;
    return count;
}

/* ================================================================
 * Detector 4: Norm-Squared Classes
 * ================================================================ */

static Raq_Partition raq_detect_norm_classes(const Raq_Cyc8 *values, int n) {
    Raq_Partition p;
    Raq_Cyc8 *norms;
    int i, j;

    p.n_values = n;
    p.n_groups = 0;
    p.group_id = (int *)calloc((size_t)n, sizeof(int));
    p.group_sizes = (int *)calloc((size_t)n, sizeof(int));
    norms = (Raq_Cyc8 *)calloc((size_t)n, sizeof(Raq_Cyc8));

    for (i = 0; i < n; i++) {
        norms[i] = raq_cyc8_norm_sq(values[i]);
        p.group_id[i] = -1;
    }

    for (i = 0; i < n; i++) {
        if (p.group_id[i] >= 0) continue;
        p.group_id[i] = p.n_groups;
        for (j = i + 1; j < n; j++) {
            if (p.group_id[j] >= 0) continue;
            if (raq_cyc8_eq(norms[i], norms[j]))
                p.group_id[j] = p.n_groups;
        }
        p.n_groups++;
    }

    for (i = 0; i < n; i++)
        p.group_sizes[p.group_id[i]]++;

    free(norms);
    return p;
}

/* ================================================================
 * Detector 5: Product Structure
 * ================================================================ */

static Raq_ProductResult raq_detect_products(const Raq_Cyc8 *values, int n) {
    Raq_ProductResult r;
    int oi, oj;

    memset(&r, 0, sizeof(r));
    r.zero_components.n_pairs = n * (n - 1) / 2;

    for (oi = 0; oi < n; oi++)
        for (oj = oi + 1; oj < n; oj++) {
            Raq_Cyc8 p = raq_cyc8_mul(values[oi], values[oj]);
            int zc = raq_cyc8_zero_count(p);
            r.zero_components.bins[zc]++;
            r.zero_components.total += zc;
            if (p.b == 0 && p.d == 0) r.n_real++;
            if (p.a == 0 && p.c == 0) r.n_imag++;
        }

    return r;
}

/* ================================================================
 * Detector 6: Galois Orbit Membership
 * ================================================================ */

static Raq_Partition raq_detect_galois_orbits(const Raq_Cyc8 *values, int n) {
    Raq_Partition p;
    int i, oi, oj;

    p.n_values = n;
    p.n_groups = 0;
    p.group_id = (int *)calloc((size_t)n, sizeof(int));
    p.group_sizes = (int *)calloc((size_t)n, sizeof(int));
    for (i = 0; i < n; i++) p.group_id[i] = -1;

    for (oi = 0; oi < n; oi++) {
        Raq_Cyc8 s3, s5, s7;
        if (p.group_id[oi] >= 0) continue;
        p.group_id[oi] = p.n_groups;

        s3 = raq_cyc8_galois_3(values[oi]);
        s5 = raq_cyc8_galois_5(values[oi]);
        s7 = raq_cyc8_galois_7(values[oi]);

        for (oj = oi + 1; oj < n; oj++) {
            if (p.group_id[oj] >= 0) continue;
            if (raq_cyc8_eq(values[oj], s3) ||
                raq_cyc8_eq(values[oj], s5) ||
                raq_cyc8_eq(values[oj], s7))
                p.group_id[oj] = p.n_groups;
        }
        p.n_groups++;
    }

    for (i = 0; i < n; i++)
        p.group_sizes[p.group_id[i]]++;

    return p;
}

/* ================================================================
 * Detector 7: Axis Agreement
 *
 * Partitions values by which zeta-axis they live on:
 *   Group for axis 0 (Z*1), axis 1 (Z*zeta), axis 2 (Z*zeta^2),
 *   axis 3 (Z*zeta^3), and a group for zero.
 * Multi-axis values (if any) get their own group.
 * ================================================================ */

static Raq_Partition raq_detect_axis_classes(const Raq_Cyc8 *values, int n) {
    Raq_Partition p;
    int i;
    /* Map axis code -> group id. axis 0-3 -> groups 0-3, -1 (zero) -> 4 */
    int axis_to_group[6]; /* indices: 0-3 for axes, 4 for zero, 5 for multi */
    int group_used[6];

    p.n_values = n;
    p.n_groups = 0;
    p.group_id = (int *)calloc((size_t)n, sizeof(int));
    p.group_sizes = (int *)calloc((size_t)n, sizeof(int));

    memset(group_used, 0, sizeof(group_used));
    for (i = 0; i < 6; i++) axis_to_group[i] = -1;

    /* First pass: determine which groups exist */
    for (i = 0; i < n; i++) {
        int ax = raq_cyc8_axis(values[i]);
        int slot;
        if (ax >= 0)      slot = ax;        /* axis 0-3 */
        else if (ax == -1) slot = 4;         /* zero */
        else               slot = 5;         /* multi-axis */
        group_used[slot] = 1;
    }

    /* Assign group ids in order */
    for (i = 0; i < 6; i++) {
        if (group_used[i])
            axis_to_group[i] = p.n_groups++;
    }

    /* Second pass: assign values to groups */
    for (i = 0; i < n; i++) {
        int ax = raq_cyc8_axis(values[i]);
        int slot;
        if (ax >= 0)      slot = ax;
        else if (ax == -1) slot = 4;
        else               slot = 5;
        p.group_id[i] = axis_to_group[slot];
        p.group_sizes[axis_to_group[slot]]++;
    }

    return p;
}

/* ================================================================
 * Detector 8: Divisibility (same-axis integer divisibility)
 *
 * For axis-aligned values x = n*zeta^k and y = m*zeta^j:
 * x | y (on same axis) iff k == j AND n divides m in Z.
 *
 * Note: in the full ring Z[zeta_8], divisibility is cross-axis
 * since zeta is a unit. This tighter same-axis notion captures
 * "combinable AND divisible" -- values that can be added while
 * staying on the same axis, where one divides the other.
 *
 * Special cases: zero is divisible by everything.
 * Units (+/-1, +/-zeta^k) divide everything on their axis.
 * ================================================================ */

typedef struct {
    int n_divides;     /* ordered pairs (i,j), i!=j, where val[i] | val[j] */
    int n_same_axis;   /* unordered pairs on the same axis */
    int n_values;
} Raq_DivisResult;

static Raq_DivisResult raq_detect_divisibility(const Raq_Cyc8 *values, int n) {
    Raq_DivisResult r;
    int i, j;

    r.n_values = n;
    r.n_divides = 0;
    r.n_same_axis = 0;

    for (i = 0; i < n; i++) {
        int ax_i = raq_cyc8_axis(values[i]);
        long c_i = raq_cyc8_coeff(values[i]);

        for (j = i + 1; j < n; j++) {
            int ax_j = raq_cyc8_axis(values[j]);
            long c_j = raq_cyc8_coeff(values[j]);

            /* Both zero: trivially same "axis" */
            if (ax_i == -1 && ax_j == -1) {
                r.n_same_axis++;
                /* 0 | 0 in both directions */
                r.n_divides += 2;
                continue;
            }

            /* One is zero: everything divides zero */
            if (ax_j == -1) {
                /* i divides j (nonzero divides zero) */
                r.n_divides++;
                continue;
            }
            if (ax_i == -1) {
                /* j divides i (nonzero divides zero) */
                r.n_divides++;
                continue;
            }

            /* Skip multi-axis or different axes */
            if (ax_i < 0 || ax_j < 0) continue;
            if (ax_i != ax_j) continue;

            r.n_same_axis++;

            /* Check divisibility both directions */
            if (c_i != 0 && c_j % c_i == 0) r.n_divides++;
            if (c_j != 0 && c_i % c_j == 0) r.n_divides++;
        }
    }

    return r;
}

/* ================================================================
 * Detector 9: 2-Adic Valuation Classes
 *
 * Groups values by v_2 of their integer coefficient.
 * v_2(n) = max power of 2 dividing n.
 * Zero gets a special group (v_2 = infinity, coded as -1).
 *
 * Connects to Boolean computation: 2-adic structure = binary
 * structure. Values with v_2=0 are odd, v_2=1 are "singly even",
 * v_2=2 are "doubly even", etc.
 * ================================================================ */

static Raq_Partition raq_detect_2adic_classes(const Raq_Cyc8 *values, int n) {
    Raq_Partition p;
    int *vals_2adic;
    int i, j;

    p.n_values = n;
    p.n_groups = 0;
    p.group_id = (int *)calloc((size_t)n, sizeof(int));
    p.group_sizes = (int *)calloc((size_t)n, sizeof(int));
    vals_2adic = (int *)calloc((size_t)n, sizeof(int));

    for (i = 0; i < n; i++) {
        vals_2adic[i] = raq_cyc8_2adic_val(values[i]);
        p.group_id[i] = -1;
    }

    for (i = 0; i < n; i++) {
        if (p.group_id[i] >= 0) continue;
        p.group_id[i] = p.n_groups;
        for (j = i + 1; j < n; j++) {
            if (p.group_id[j] >= 0) continue;
            if (vals_2adic[i] == vals_2adic[j])
                p.group_id[j] = p.n_groups;
        }
        p.n_groups++;
    }

    for (i = 0; i < n; i++)
        p.group_sizes[p.group_id[i]]++;

    free(vals_2adic);
    return p;
}

/* ================================================================
 * Full Analysis
 * ================================================================ */

typedef struct {
    int n_values;
    Raq_Partition root_orbits;
    Raq_Partition norm_classes;
    Raq_Partition galois_orbits;
    Raq_Partition axis_classes;
    Raq_Partition twoadic_classes;
    Raq_DivisResult divisibility;
    Raq_PairHist sign_distances;
    Raq_PairHist sum_zeros;
    Raq_PairHist diff_zeros;
    Raq_ProductResult products;
    int n_sign_patterns;
    int n_neg_pairs;
} Raq_Analysis;

static Raq_Analysis raq_analyze(const Raq_Cyc8 *values, int n) {
    Raq_Analysis a;
    a.n_values = n;
    a.root_orbits = raq_detect_root_orbits(values, n);
    a.norm_classes = raq_detect_norm_classes(values, n);
    a.galois_orbits = raq_detect_galois_orbits(values, n);
    a.axis_classes = raq_detect_axis_classes(values, n);
    a.twoadic_classes = raq_detect_2adic_classes(values, n);
    a.divisibility = raq_detect_divisibility(values, n);
    a.sign_distances = raq_detect_sign_distances(values, n);
    a.sum_zeros = raq_detect_sum_zeros(values, n);
    a.diff_zeros = raq_detect_diff_zeros(values, n);
    a.products = raq_detect_products(values, n);
    a.n_sign_patterns = raq_count_sign_patterns(values, n);
    a.n_neg_pairs = raq_count_neg_pairs(values, n);
    return a;
}

static void raq_analysis_free(Raq_Analysis *a) {
    raq_partition_free(&a->root_orbits);
    raq_partition_free(&a->norm_classes);
    raq_partition_free(&a->galois_orbits);
    raq_partition_free(&a->axis_classes);
    raq_partition_free(&a->twoadic_classes);
}

/* ================================================================
 * GRAPH ANALYSIS LAYER
 *
 * Edge Generators x Graph Analyzers with automatic cross-integration.
 *
 * Edge generators build graphs from algebraic relationships between
 * Cyc8 values. Graph analyzers detect structural properties.
 * The sweep function runs all combinations, reports positive findings.
 * ================================================================ */

/* --- Edge List --- */

typedef struct {
    int *src;
    int *dst;
    int count;
    int capacity;
    const char *titulis;
    int directus;   /* 0 = undirected, 1 = directed */
} Raq_EdgeList;

static void raq_el_init(Raq_EdgeList *el, int cap,
                         const char *titulis, int dir) {
    if (cap < 1) cap = 1;
    el->capacity = cap;
    el->count = 0;
    el->src = (int *)malloc((size_t)cap * sizeof(int));
    el->dst = (int *)malloc((size_t)cap * sizeof(int));
    el->titulis = titulis;
    el->directus = dir;
}

static void raq_el_free(Raq_EdgeList *el) {
    free(el->src);
    free(el->dst);
    el->src = NULL;
    el->dst = NULL;
    el->count = 0;
}

static void raq_el_add(Raq_EdgeList *el, int s, int d) {
    if (el->count >= el->capacity) {
        int new_cap = el->capacity * 2;
        int *ns = (int *)realloc(el->src, (size_t)new_cap * sizeof(int));
        int *nd = (int *)realloc(el->dst, (size_t)new_cap * sizeof(int));
        if (!ns || !nd) return;
        el->src = ns;
        el->dst = nd;
        el->capacity = new_cap;
    }
    el->src[el->count] = s;
    el->dst[el->count] = d;
    el->count++;
}

/* --- Graph Analysis Result --- */

typedef struct {
    int n_vertices;
    int n_edges;

    /* Connectivity */
    int n_components;
    int *component_sizes;    /* sorted descending, length = n_components */
    int n_isolated;

    /* Bipartite (undirected only) */
    int est_bipartitus;      /* 1/0, -1 if not checked (directed) */
    int bipartitio_a;
    int bipartitio_b;

    /* Structure recognition (undirected only) */
    int est_completus;
    int est_arbor;           /* tree or forest */
    int est_via;             /* path graph */
    int est_cyclus;          /* cycle graph */
    int ambitus;             /* girth (shortest cycle), 0 if acyclic */

    /* Directed only */
    int est_dag;             /* 1/0, -1 if not checked */
    int longitudo_catena;    /* longest chain, -1 if not DAG */

    /* Degree stats (non-isolated vertices) */
    int gradus_min;          /* -1 if no edges */
    int gradus_max;
    int gradus_summa;
    int est_regularis;       /* 1 if all non-isolated vertices have same degree */
    int gradus_regularis;    /* the common degree if regular, -1 otherwise */
} Raq_GraphResult;

static void raq_graph_result_free(Raq_GraphResult *gr) {
    free(gr->component_sizes);
    gr->component_sizes = NULL;
}

/* --- Sort helper for component sizes (descending) --- */

static int raq_cmp_int_desc(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia > ib) return -1;
    if (ia < ib) return 1;
    return 0;
}

/* --- Core Graph Analyzer --- */

static Raq_GraphResult raq_graph_analyze(const Raq_EdgeList *el, int n) {
    Raq_GraphResult gr;
    unsigned char *adj = NULL;
    int *degree = NULL;
    int *bfs_deg = NULL;
    int *color = NULL;
    int *queue = NULL;
    int *comp_tmp = NULL;
    int nc, iso, bip;
    int n_active, n_ntcomp;
    int own_bfs_deg = 0;
    int i, j;

    memset(&gr, 0, sizeof(gr));
    gr.n_vertices = n;
    gr.n_edges = el->count;
    gr.est_bipartitus = -1;
    gr.est_dag = -1;
    gr.longitudo_catena = -1;
    gr.gradus_min = -1;
    gr.gradus_max = -1;
    gr.est_regularis = 0;
    gr.gradus_regularis = -1;

    if (n == 0) {
        gr.component_sizes = (int *)calloc(1, sizeof(int));
        return gr;
    }

    if (el->count == 0) {
        gr.n_components = n;
        gr.n_isolated = n;
        gr.component_sizes = (int *)malloc((size_t)n * sizeof(int));
        for (i = 0; i < n; i++) gr.component_sizes[i] = 1;
        if (!el->directus) {
            gr.est_bipartitus = 1;
            gr.bipartitio_a = n;
            gr.est_completus = (n <= 1) ? 1 : 0;
            gr.est_arbor = 1;
            gr.est_via = (n <= 1) ? 1 : 0;
        } else {
            gr.est_dag = 1;
            gr.longitudo_catena = 0;
        }
        return gr;
    }

    /* Build adjacency matrix */
    adj = (unsigned char *)calloc((size_t)n * (size_t)n, 1);
    for (i = 0; i < el->count; i++) {
        adj[el->src[i] * n + el->dst[i]] = 1;
        if (!el->directus)
            adj[el->dst[i] * n + el->src[i]] = 1;
    }

    /* Out-degree (= undirected degree for undirected graphs) */
    degree = (int *)calloc((size_t)n, sizeof(int));
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (adj[i * n + j]) degree[i]++;

    /* For directed: undirected degree for BFS component detection */
    if (el->directus) {
        bfs_deg = (int *)calloc((size_t)n, sizeof(int));
        own_bfs_deg = 1;
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (adj[i * n + j] || adj[j * n + i]) bfs_deg[i]++;
    } else {
        bfs_deg = degree;
    }

    /* BFS: components + bipartite */
    color = (int *)malloc((size_t)n * sizeof(int));
    queue = (int *)malloc((size_t)n * sizeof(int));
    comp_tmp = (int *)calloc((size_t)n, sizeof(int));
    for (i = 0; i < n; i++) color[i] = -1;
    nc = 0; iso = 0; bip = 1;
    gr.bipartitio_a = 0; gr.bipartitio_b = 0;

    for (i = 0; i < n; i++) {
        int qh, qt, comp_a, comp_b;
        if (color[i] >= 0) continue;

        if (bfs_deg[i] == 0) {
            color[i] = 0;
            comp_tmp[nc] = 1;
            iso++;
            nc++;
            continue;
        }

        color[i] = 0;
        comp_a = 1; comp_b = 0;
        qh = 0; qt = 0;
        queue[qt++] = i;
        while (qh < qt) {
            int u = queue[qh++];
            int v;
            for (v = 0; v < n; v++) {
                int has_edge = adj[u * n + v];
                if (el->directus) has_edge |= adj[v * n + u];
                if (!has_edge) continue;
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    if (color[v] == 0) comp_a++;
                    else comp_b++;
                    queue[qt++] = v;
                } else if (!el->directus && color[v] == color[u]) {
                    bip = 0;
                }
            }
        }
        comp_tmp[nc] = comp_a + comp_b;
        gr.bipartitio_a += comp_a;
        gr.bipartitio_b += comp_b;
        nc++;
    }

    gr.n_components = nc;
    gr.n_isolated = iso;
    gr.component_sizes = (int *)malloc((size_t)nc * sizeof(int));
    memcpy(gr.component_sizes, comp_tmp, (size_t)nc * sizeof(int));
    qsort(gr.component_sizes, (size_t)nc, sizeof(int), raq_cmp_int_desc);

    /* Degree stats (non-isolated, uses out-degree for directed) */
    {
        int dmin = n + 1, dmax = 0, dsum = 0;
        for (i = 0; i < n; i++) {
            if (bfs_deg[i] == 0) continue;
            if (degree[i] < dmin) dmin = degree[i];
            if (degree[i] > dmax) dmax = degree[i];
            dsum += degree[i];
        }
        if (dmax > 0) {
            gr.gradus_min = dmin;
            gr.gradus_max = dmax;
            gr.gradus_summa = dsum;
            gr.est_regularis = (dmin == dmax) ? 1 : 0;
            gr.gradus_regularis = (dmin == dmax) ? dmin : -1;
        }
    }

    n_active = n - iso;
    n_ntcomp = nc - iso;

    if (!el->directus) {
        /* --- Undirected analyses --- */
        gr.est_bipartitus = bip;
        if (!bip) { gr.bipartitio_a = 0; gr.bipartitio_b = 0; }

        gr.est_completus = (n_active >= 2 &&
            el->count == n_active * (n_active - 1) / 2) ? 1 :
            (n_active <= 1) ? 1 : 0;

        gr.est_arbor = (n_active == 0) ? 1 :
            (el->count == n_active - n_ntcomp) ? 1 : 0;

        gr.est_via = (gr.est_arbor && n_ntcomp <= 1 &&
                      (gr.gradus_max <= 2 || gr.gradus_max == -1)) ? 1 : 0;

        gr.est_cyclus = (n_ntcomp == 1 && n_active >= 3 &&
                         gr.gradus_min == 2 && gr.gradus_max == 2) ? 1 : 0;

        /* Girth: BFS from each non-isolated vertex */
        gr.ambitus = 0;
        if (!gr.est_arbor && n_active >= 3) {
            int min_cycle = n + 1;
            int *dist = (int *)malloc((size_t)n * sizeof(int));
            int *parent = (int *)malloc((size_t)n * sizeof(int));

            for (i = 0; i < n; i++) {
                int qi, qt2;
                if (degree[i] == 0) continue;
                for (j = 0; j < n; j++) { dist[j] = -1; parent[j] = -1; }
                dist[i] = 0;
                qi = 0; qt2 = 0;
                queue[qt2++] = i;
                while (qi < qt2) {
                    int u = queue[qi++];
                    int v;
                    for (v = 0; v < n; v++) {
                        if (!adj[u * n + v]) continue;
                        if (dist[v] == -1) {
                            dist[v] = dist[u] + 1;
                            parent[v] = u;
                            queue[qt2++] = v;
                        } else if (v != parent[u]) {
                            int cl = dist[u] + dist[v] + 1;
                            if (cl < min_cycle) min_cycle = cl;
                        }
                    }
                }
                if (min_cycle == 3) break;
            }
            if (min_cycle <= n) gr.ambitus = min_cycle;
            free(dist);
            free(parent);
        }
    } else {
        /* --- Directed analyses: DAG + longest chain --- */
        {
            int *in_deg = (int *)calloc((size_t)n, sizeof(int));
            int *dp = (int *)calloc((size_t)n, sizeof(int));
            int topo_count, qi, qt2, max_chain;

            for (i = 0; i < el->count; i++)
                in_deg[el->dst[i]]++;

            qi = 0; qt2 = 0;
            for (i = 0; i < n; i++)
                if (in_deg[i] == 0) queue[qt2++] = i;

            topo_count = 0;
            while (qi < qt2) {
                int u = queue[qi++];
                topo_count++;
                for (j = 0; j < n; j++) {
                    if (!adj[u * n + j]) continue;
                    if (dp[u] + 1 > dp[j]) dp[j] = dp[u] + 1;
                    in_deg[j]--;
                    if (in_deg[j] == 0) queue[qt2++] = j;
                }
            }

            gr.est_dag = (topo_count == n) ? 1 : 0;
            if (gr.est_dag) {
                max_chain = 0;
                for (i = 0; i < n; i++)
                    if (dp[i] > max_chain) max_chain = dp[i];
                gr.longitudo_catena = max_chain;
            }
            free(in_deg);
            free(dp);
        }
    }

    free(adj);
    free(degree);
    if (own_bfs_deg) free(bfs_deg);
    free(color);
    free(queue);
    free(comp_tmp);
    return gr;
}

/* --- Edge Generators --- */

/* Generate undirected edges from a partition (connect all within-group pairs) */
static Raq_EdgeList raq_edges_from_partition(const Raq_Partition *p,
                                              const char *titulis) {
    Raq_EdgeList el;
    int max_e = raq_partition_related_pairs(p);
    int i, j;
    raq_el_init(&el, max_e > 0 ? max_e : 1, titulis, 0);
    for (i = 0; i < p->n_values; i++)
        for (j = i + 1; j < p->n_values; j++)
            if (p->group_id[i] == p->group_id[j])
                raq_el_add(&el, i, j);
    return el;
}

#ifdef RAQ_CONVENIENCE
static Raq_EdgeList raq_edges_same_axis(const Raq_Cyc8 *values, int n) {
    Raq_Partition p = raq_detect_axis_classes(values, n);
    Raq_EdgeList el = raq_edges_from_partition(&p, "same_axis");
    raq_partition_free(&p);
    return el;
}

static Raq_EdgeList raq_edges_same_root_orbit(const Raq_Cyc8 *values, int n) {
    Raq_Partition p = raq_detect_root_orbits(values, n);
    Raq_EdgeList el = raq_edges_from_partition(&p, "same_root_orbit");
    raq_partition_free(&p);
    return el;
}

static Raq_EdgeList raq_edges_same_galois_orbit(const Raq_Cyc8 *values, int n) {
    Raq_Partition p = raq_detect_galois_orbits(values, n);
    Raq_EdgeList el = raq_edges_from_partition(&p, "same_galois");
    raq_partition_free(&p);
    return el;
}

static Raq_EdgeList raq_edges_same_v2(const Raq_Cyc8 *values, int n) {
    Raq_Partition p = raq_detect_2adic_classes(values, n);
    Raq_EdgeList el = raq_edges_from_partition(&p, "same_v2");
    raq_partition_free(&p);
    return el;
}
#endif /* RAQ_CONVENIENCE */

/* Divisibility: directed edge i->j when val[i] | val[j] (same axis) */
static Raq_EdgeList raq_edges_divisibility(const Raq_Cyc8 *values, int n) {
    Raq_EdgeList el;
    int i, j;
    raq_el_init(&el, n > 1 ? n * 10 : 1, "divisibility", 1);
    for (i = 0; i < n; i++) {
        int ax_i = raq_cyc8_axis(values[i]);
        long c_i;
        if (ax_i == -2) continue;
        c_i = raq_cyc8_coeff(values[i]);
        for (j = 0; j < n; j++) {
            int ax_j;
            long c_j;
            if (i == j) continue;
            ax_j = raq_cyc8_axis(values[j]);
            if (ax_j == -2) continue;
            c_j = raq_cyc8_coeff(values[j]);
            if (ax_j == -1 && ax_i != -1) {
                raq_el_add(&el, i, j);
                continue;
            }
            if (ax_i == -1) continue;
            if (ax_i != ax_j) continue;
            if (c_i != 0 && c_j % c_i == 0)
                raq_el_add(&el, i, j);
        }
    }
    return el;
}

/* Positive divisibility: directed edge i->j when |coeff(i)| | |coeff(j)|
 * on the same axis. Uses absolute value to avoid sign-cycles. Genuine DAG. */
static Raq_EdgeList raq_edges_positive_divisibility(const Raq_Cyc8 *values,
                                                     int n) {
    Raq_EdgeList el;
    int i, j;
    raq_el_init(&el, n > 1 ? n * 10 : 1, "pos_divisibility", 1);
    for (i = 0; i < n; i++) {
        int ax_i = raq_cyc8_axis(values[i]);
        long c_i, ac_i;
        if (ax_i == -2) continue;  /* multi-axis, skip */
        c_i = raq_cyc8_coeff(values[i]);
        ac_i = c_i < 0 ? -c_i : c_i;
        for (j = 0; j < n; j++) {
            int ax_j;
            long c_j, ac_j;
            if (i == j) continue;
            ax_j = raq_cyc8_axis(values[j]);
            if (ax_j == -2) continue;
            c_j = raq_cyc8_coeff(values[j]);
            ac_j = c_j < 0 ? -c_j : c_j;
            /* zero is divisible by everything */
            if (ax_j == -1 && ax_i != -1) {
                raq_el_add(&el, i, j);
                continue;
            }
            if (ax_i == -1) continue;  /* 0 doesn't divide nonzero */
            if (ax_i != ax_j) continue;  /* different axes */
            /* |c_i| divides |c_j|, no self-edges, no sign-cycles */
            if (ac_i != 0 && ac_j % ac_i == 0 && ac_j > ac_i)
                raq_el_add(&el, i, j);
        }
    }
    return el;
}

/* Product closure: undirected edge i-j when val[i]*val[j] is in the value set */
static Raq_EdgeList raq_edges_product_closure(const Raq_Cyc8 *values, int n,
                                                const Raq_ValueSet *vs) {
    Raq_EdgeList el;
    int i, j;
    raq_el_init(&el, n > 1 ? n * 4 : 1, "product_closure", 0);
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++) {
            Raq_Cyc8 prod = raq_cyc8_mul(values[i], values[j]);
            if (raq_vs_find(vs, prod) >= 0)
                raq_el_add(&el, i, j);
        }
    return el;
}

/* Galois action edges: directed edge i->j when sigma_k(val[i]) = val[j].
 * Each sigma is a permutation, so resulting graph is a union of disjoint cycles
 * (if value set is closed under sigma_k) or partial permutation with isolated vertices. */
static Raq_EdgeList raq_edges_galois_sigma3(const Raq_Cyc8 *values, int n,
                                              const Raq_ValueSet *vs) {
    Raq_EdgeList el;
    int i;
    raq_el_init(&el, n, "galois_s3", 1);
    for (i = 0; i < n; i++) {
        Raq_Cyc8 img = raq_cyc8_galois_3(values[i]);
        int j = raq_vs_find(vs, img);
        if (j >= 0 && j != i) raq_el_add(&el, i, j);
    }
    return el;
}

static Raq_EdgeList raq_edges_galois_sigma5(const Raq_Cyc8 *values, int n,
                                              const Raq_ValueSet *vs) {
    Raq_EdgeList el;
    int i;
    raq_el_init(&el, n, "galois_s5", 1);
    for (i = 0; i < n; i++) {
        Raq_Cyc8 img = raq_cyc8_galois_5(values[i]);
        int j = raq_vs_find(vs, img);
        if (j >= 0 && j != i) raq_el_add(&el, i, j);
    }
    return el;
}

static Raq_EdgeList raq_edges_galois_sigma7(const Raq_Cyc8 *values, int n,
                                              const Raq_ValueSet *vs) {
    Raq_EdgeList el;
    int i;
    raq_el_init(&el, n, "galois_s7", 1);
    for (i = 0; i < n; i++) {
        Raq_Cyc8 img = raq_cyc8_galois_7(values[i]);
        int j = raq_vs_find(vs, img);
        if (j >= 0 && j != i) raq_el_add(&el, i, j);
    }
    return el;
}

/* Additive closure: undirected edge i-j when val[i]+val[j] is in the set.
 * Only connects same-axis values (cross-axis sums produce multi-axis values
 * which can't be in an axis-aligned set). */
static Raq_EdgeList raq_edges_additive_closure(const Raq_Cyc8 *values, int n,
                                                 const Raq_ValueSet *vs) {
    Raq_EdgeList el;
    int i, j;
    raq_el_init(&el, n > 1 ? n * 4 : 1, "additive_closure", 0);
    for (i = 0; i < n; i++) {
        int ax_i = raq_cyc8_axis(values[i]);
        for (j = i + 1; j < n; j++) {
            Raq_Cyc8 sum;
            int ax_j = raq_cyc8_axis(values[j]);
            /* Cross-axis sums produce non-axis-aligned values */
            if (ax_i >= 0 && ax_j >= 0 && ax_i != ax_j) continue;
            sum = raq_cyc8_add(values[i], values[j]);
            if (raq_vs_find(vs, sum) >= 0)
                raq_el_add(&el, i, j);
        }
    }
    return el;
}

/* Power edges: directed edge i->j when val[i]^2 = val[j] (squaring map).
 * Each vertex has at most one outgoing edge. */
static Raq_EdgeList raq_edges_power2(const Raq_Cyc8 *values, int n,
                                       const Raq_ValueSet *vs) {
    Raq_EdgeList el;
    int i;
    raq_el_init(&el, n, "power2", 1);
    for (i = 0; i < n; i++) {
        Raq_Cyc8 sq = raq_cyc8_mul(values[i], values[i]);
        int j = raq_vs_find(vs, sq);
        if (j >= 0 && j != i) raq_el_add(&el, i, j);
    }
    return el;
}

/* GCD edges: undirected edge i-j when gcd(|coeff(i)|, |coeff(j)|) > 1,
 * same axis only. Captures shared prime factor structure. */
static Raq_EdgeList raq_edges_gcd(const Raq_Cyc8 *values, int n) {
    Raq_EdgeList el;
    int i, j;
    raq_el_init(&el, n > 1 ? n * 4 : 1, "gcd", 0);
    for (i = 0; i < n; i++) {
        int ax_i = raq_cyc8_axis(values[i]);
        long c_i, ac_i;
        if (ax_i < 0) continue;  /* skip zero and multi-axis */
        c_i = raq_cyc8_coeff(values[i]);
        ac_i = c_i < 0 ? -c_i : c_i;
        if (ac_i <= 1) continue;  /* gcd(1,x)=1 always, skip */
        for (j = i + 1; j < n; j++) {
            int ax_j = raq_cyc8_axis(values[j]);
            long c_j, ac_j, a, b, t;
            if (ax_j < 0) continue;
            if (ax_i != ax_j) continue;
            c_j = raq_cyc8_coeff(values[j]);
            ac_j = c_j < 0 ? -c_j : c_j;
            if (ac_j <= 1) continue;
            /* Euclidean GCD */
            a = ac_i; b = ac_j;
            while (b != 0) { t = b; b = a % b; a = t; }
            if (a > 1) raq_el_add(&el, i, j);
        }
    }
    return el;
}

/* --- Cross-Integration --- */

/* Restrict edge list to vertices in partition group g, re-indexing to 0..size-1 */
static Raq_EdgeList raq_el_restrict(const Raq_EdgeList *el,
                                     const Raq_Partition *p,
                                     int group,
                                     int *n_verts_out) {
    Raq_EdgeList res;
    int *remap;
    int new_n = 0;
    int i;
    remap = (int *)malloc((size_t)p->n_values * sizeof(int));
    for (i = 0; i < p->n_values; i++) {
        if (p->group_id[i] == group)
            remap[i] = new_n++;
        else
            remap[i] = -1;
    }
    raq_el_init(&res, el->count > 0 ? el->count : 1,
                el->titulis, el->directus);
    for (i = 0; i < el->count; i++) {
        if (remap[el->src[i]] >= 0 && remap[el->dst[i]] >= 0)
            raq_el_add(&res, remap[el->src[i]], remap[el->dst[i]]);
    }
    *n_verts_out = new_n;
    free(remap);
    return res;
}

/* Intersect two edge lists (same vertex set, size n) */
static Raq_EdgeList raq_el_intersect(const Raq_EdgeList *a,
                                      const Raq_EdgeList *b,
                                      int n) {
    Raq_EdgeList res;
    unsigned char *adj_b;
    int i;
    adj_b = (unsigned char *)calloc((size_t)n * (size_t)n, 1);
    for (i = 0; i < b->count; i++) {
        adj_b[b->src[i] * n + b->dst[i]] = 1;
        if (!b->directus)
            adj_b[b->dst[i] * n + b->src[i]] = 1;
    }
    raq_el_init(&res, a->count > 0 ? a->count : 1, "intersect",
                a->directus || b->directus);
    for (i = 0; i < a->count; i++) {
        if (adj_b[a->src[i] * n + a->dst[i]])
            raq_el_add(&res, a->src[i], a->dst[i]);
    }
    free(adj_b);
    return res;
}

/* Quotient graph: collapse partition classes to super-vertices.
 * Edge (g1, g2) in quotient if any edge (u, v) exists where
 * u in group g1 and v in group g2. No self-loops. */
static Raq_EdgeList raq_el_quotient(const Raq_EdgeList *el,
                                      const Raq_Partition *p,
                                      int *n_groups_out) {
    Raq_EdgeList res;
    int ng = p->n_groups;
    unsigned char *seen;
    int i;
    *n_groups_out = ng;
    seen = (unsigned char *)calloc((size_t)ng * (size_t)ng, 1);
    raq_el_init(&res, ng * ng > 0 ? ng * ng : 1,
                "quotient", el->directus);
    for (i = 0; i < el->count; i++) {
        int gs = p->group_id[el->src[i]];
        int gd = p->group_id[el->dst[i]];
        if (gs == gd) continue;  /* no self-loops */
        if (!el->directus && gs > gd) { int t = gs; gs = gd; gd = t; }
        if (!seen[gs * ng + gd]) {
            seen[gs * ng + gd] = 1;
            raq_el_add(&res, gs, gd);
        }
    }
    free(seen);
    return res;
}

/* Check degree sequence isomorphism between two subgraphs.
 * Returns 1 if they have the same sorted degree sequence. */
static int raq_degree_seq_match(const Raq_EdgeList *a, int na,
                                  const Raq_EdgeList *b, int nb) {
    int *da, *db;
    int i, match;
    if (na != nb || a->count != b->count) return 0;
    if (na == 0) return 1;
    da = (int *)calloc((size_t)na, sizeof(int));
    db = (int *)calloc((size_t)nb, sizeof(int));
    for (i = 0; i < a->count; i++) {
        da[a->src[i]]++;
        if (!a->directus) da[a->dst[i]]++;
    }
    for (i = 0; i < b->count; i++) {
        db[b->src[i]]++;
        if (!b->directus) db[b->dst[i]]++;
    }
    qsort(da, (size_t)na, sizeof(int), raq_cmp_int_desc);
    qsort(db, (size_t)nb, sizeof(int), raq_cmp_int_desc);
    match = 1;
    for (i = 0; i < na; i++) {
        if (da[i] != db[i]) { match = 0; break; }
    }
    free(da); free(db);
    return match;
}

/* ================================================================
 * Print Helpers (require stdio.h -- include before raqiya.h)
 *
 * Only compiled if RAQ_PRINT is defined.
 * ================================================================ */

#ifdef RAQ_PRINT
#include <stdio.h>

static void raq_print_cyc8(Raq_Cyc8 v) {
    printf("(%ld,%ld,%ld,%ld)", v.a, v.b, v.c, v.d);
}

/* Print value with algebraic identification if it is n*zeta^k */
static void raq_print_cyc8_named(Raq_Cyc8 v) {
    raq_print_cyc8(v);
    if (v.b == 0 && v.c == 0 && v.d == 0)
        printf(" = %ld", v.a);
    else if (v.a == 0 && v.c == 0 && v.d == 0)
        printf(" = %ld*z", v.b);
    else if (v.a == 0 && v.b == 0 && v.d == 0)
        printf(" = %ld*z^2", v.c);
    else if (v.a == 0 && v.b == 0 && v.c == 0)
        printf(" = %ld*z^3", v.d);
}

static void raq_print_partition(const char *titulis,
                                 const Raq_Partition *p,
                                 const Raq_Cyc8 *values) {
    int max_sz = raq_partition_max_size(p);
    int related = raq_partition_related_pairs(p);
    int total_pairs = p->n_values * (p->n_values - 1) / 2;
    int sz, gi, i;

    printf("  %d values form %d %s\n", p->n_values, p->n_groups, titulis);
    if (total_pairs > 0)
        printf("  Related pairs: %d / %d (%.1f%%)\n",
               related, total_pairs,
               100.0 * (double)related / (double)total_pairs);
    printf("  Max group size: %d\n", max_sz);

    printf("  Size histogram:\n");
    for (sz = 1; sz <= max_sz; sz++) {
        int cnt = 0;
        for (gi = 0; gi < p->n_groups; gi++)
            if (p->group_sizes[gi] == sz) cnt++;
        if (cnt > 0)
            printf("    Size %2d: %d groups\n", sz, cnt);
    }

    /* Print each group with members */
    printf("  Groups:\n");
    for (gi = 0; gi < p->n_groups; gi++) {
        printf("    [%d] (n=%d):", gi, p->group_sizes[gi]);
        for (i = 0; i < p->n_values; i++) {
            if (p->group_id[i] == gi) {
                printf(" ");
                raq_print_cyc8_named(values[i]);
            }
        }
        printf("\n");
    }
}

static void raq_print_pair_hist(const char *titulis, const Raq_PairHist *h) {
    int i;
    printf("  %s histogram:\n", titulis);
    for (i = 0; i <= 4; i++) {
        if (h->bins[i] > 0 || i <= 2)
            printf("    %d: %d pairs (%.1f%%)\n",
                   i, h->bins[i],
                   h->n_pairs > 0
                       ? 100.0 * (double)h->bins[i] / (double)h->n_pairs
                       : 0.0);
    }
    if (h->n_pairs > 0)
        printf("  Mean: %.3f\n",
               (double)h->total / (double)h->n_pairs);
}

static void raq_print_divis(const Raq_DivisResult *d) {
    printf("  %d values, %d same-axis unordered pairs\n",
           d->n_values, d->n_same_axis);
    printf("  Divisibility relations (ordered): %d\n", d->n_divides);
    if (d->n_same_axis > 0)
        printf("  Density: %.1f%% of same-axis pairs have at least "
               "one direction\n",
               100.0 * (double)d->n_divides / (double)(d->n_same_axis * 2));
}

static void raq_print_analysis(const Raq_Analysis *a,
                                const Raq_Cyc8 *values) {
    printf("\n--- Root-of-Unity Orbits ---\n");
    raq_print_partition("root-of-unity orbits", &a->root_orbits, values);

    printf("\n--- Norm-Squared Classes ---\n");
    raq_print_partition("norm-sq classes", &a->norm_classes, values);

    printf("\n--- Galois Orbits ---\n");
    raq_print_partition("Galois orbits", &a->galois_orbits, values);

    printf("\n--- Axis Agreement ---\n");
    raq_print_partition("axis classes", &a->axis_classes, values);

    printf("\n--- Divisibility (Same-Axis) ---\n");
    raq_print_divis(&a->divisibility);

    printf("\n--- 2-Adic Valuation Classes ---\n");
    raq_print_partition("2-adic classes", &a->twoadic_classes, values);

    printf("\n--- Sign Pattern Distance ---\n");
    raq_print_pair_hist("Sign distance", &a->sign_distances);
    printf("  Distinct sign patterns: %d / 81\n", a->n_sign_patterns);

    printf("\n--- Sum Zero-Components ---\n");
    raq_print_pair_hist("Sum x+y zeros", &a->sum_zeros);
    printf("  Negative pairs (x+y=0): %d\n", a->n_neg_pairs);

    printf("\n--- Difference Zero-Components ---\n");
    raq_print_pair_hist("Diff x-y zeros", &a->diff_zeros);

    printf("\n--- Product Structure ---\n");
    raq_print_pair_hist("Product zeros", &a->products.zero_components);
    if (a->products.zero_components.n_pairs > 0) {
        printf("  Real products (b=d=0): %d (%.1f%%)\n",
               a->products.n_real,
               100.0 * (double)a->products.n_real
                   / (double)a->products.zero_components.n_pairs);
        printf("  Pure-imag products (a=c=0): %d (%.1f%%)\n",
               a->products.n_imag,
               100.0 * (double)a->products.n_imag
                   / (double)a->products.zero_components.n_pairs);
    }
}

/* ================================================================
 * Graph Analysis: Print and Sweep
 * ================================================================ */

static void raq_fprint_graph_result(FILE *f, const char *label,
                                     const Raq_GraphResult *gr) {
    int nc_nt;
    if (gr->n_edges == 0 && gr->n_vertices <= 1) return;

    nc_nt = gr->n_components - gr->n_isolated;
    fprintf(f, "[%s] %dv %de", label, gr->n_vertices, gr->n_edges);
    if (nc_nt > 0) {
        fprintf(f, ", %d comp", nc_nt);
        if (gr->n_isolated > 0) fprintf(f, " (+%d iso)", gr->n_isolated);
    }
    fprintf(f, "\n");

    if (gr->est_bipartitus == 1)
        fprintf(f, "  bipartite: %d+%d\n", gr->bipartitio_a, gr->bipartitio_b);
    if (gr->est_completus)
        fprintf(f, "  complete: K_%d\n", gr->n_vertices - gr->n_isolated);
    if (gr->est_arbor) {
        if (gr->est_via)
            fprintf(f, "  path: P_%d\n", gr->n_vertices - gr->n_isolated);
        else
            fprintf(f, "  tree/forest\n");
    }
    if (gr->est_cyclus)
        fprintf(f, "  cycle: C_%d\n", gr->n_vertices - gr->n_isolated);
    if (gr->ambitus > 0)
        fprintf(f, "  girth: %d\n", gr->ambitus);
    if (gr->est_dag == 1)
        fprintf(f, "  DAG: longest chain %d\n", gr->longitudo_catena);
    if (gr->est_regularis)
        fprintf(f, "  regular: %d-regular\n", gr->gradus_regularis);
    if (gr->gradus_min >= 0)
        fprintf(f, "  degree: min=%d max=%d sum=%d\n",
                gr->gradus_min, gr->gradus_max, gr->gradus_summa);
}

static void raq_graph_sweep(const Raq_Cyc8 *values, int count, FILE *f) {
    Raq_Partition p_axis, p_root, p_galois, p_norm, p_v2;
    const Raq_Partition *parts[5];
    const char *part_names[5];
    Raq_EdgeList edges[13];
    const char *edge_names[13];
    Raq_ValueSet vs;
    int ne = 13, np = 5;
    int i, j, g;

    fprintf(f, "\n===== GRAPH ANALYSIS SWEEP (%d values) =====\n\n", count);
    if (count < 2) {
        fprintf(f, "(too few values)\n");
        return;
    }

    /* Build partitions */
    p_axis   = raq_detect_axis_classes(values, count);
    p_root   = raq_detect_root_orbits(values, count);
    p_galois = raq_detect_galois_orbits(values, count);
    p_norm   = raq_detect_norm_classes(values, count);
    p_v2     = raq_detect_2adic_classes(values, count);

    parts[0] = &p_axis;   part_names[0] = "axis";
    parts[1] = &p_root;   part_names[1] = "root";
    parts[2] = &p_galois; part_names[2] = "galois";
    parts[3] = &p_norm;   part_names[3] = "norm";
    parts[4] = &p_v2;     part_names[4] = "v2";

    /* Partition equivalence check */
    {
        int root_eq_norm = 1, root_eq_v2 = 1;
        if (p_root.n_groups == p_norm.n_groups &&
            p_root.n_groups == p_v2.n_groups) {
            for (i = 0; i < count; i++) {
                if (p_root.group_id[i] != p_norm.group_id[i])
                    root_eq_norm = 0;
                if (p_root.group_id[i] != p_v2.group_id[i])
                    root_eq_v2 = 0;
            }
        } else {
            root_eq_norm = 0;
            root_eq_v2 = 0;
        }
        fprintf(f, "--- Partition Equivalences ---\n");
        fprintf(f, "  root (%d groups) %s norm (%d groups)\n",
                p_root.n_groups, root_eq_norm ? "==" : "!=",
                p_norm.n_groups);
        fprintf(f, "  root (%d groups) %s v2 (%d groups)\n",
                p_root.n_groups, root_eq_v2 ? "==" : "!=",
                p_v2.n_groups);
        if (root_eq_norm && root_eq_v2)
            fprintf(f, "  THEOREM: root = norm = v2 "
                    "(axis-aligned => |coeff| determines all three)\n");
        fprintf(f, "\n");
    }

    /* Build value set for product closure lookup */
    raq_vs_init(&vs, count + 1);
    for (i = 0; i < count; i++) raq_vs_insert(&vs, values[i]);

    /* Generate edge types */
    edges[0]  = raq_edges_from_partition(&p_axis, "same_axis");
    edges[1]  = raq_edges_from_partition(&p_root, "same_root");
    edges[2]  = raq_edges_from_partition(&p_galois, "same_galois");
    edges[3]  = raq_edges_from_partition(&p_v2, "same_v2");
    edges[4]  = raq_edges_divisibility(values, count);
    edges[5]  = raq_edges_product_closure(values, count, &vs);
    edges[6]  = raq_edges_positive_divisibility(values, count);
    edges[7]  = raq_edges_galois_sigma3(values, count, &vs);
    edges[8]  = raq_edges_galois_sigma5(values, count, &vs);
    edges[9]  = raq_edges_galois_sigma7(values, count, &vs);
    edges[10] = raq_edges_additive_closure(values, count, &vs);
    edges[11] = raq_edges_power2(values, count, &vs);
    edges[12] = raq_edges_gcd(values, count);
    edge_names[0]  = "same_axis";
    edge_names[1]  = "same_root";
    edge_names[2]  = "same_galois";
    edge_names[3]  = "same_v2";
    edge_names[4]  = "divisibility";
    edge_names[5]  = "product_closure";
    edge_names[6]  = "pos_divisibility";
    edge_names[7]  = "galois_s3";
    edge_names[8]  = "galois_s5";
    edge_names[9]  = "galois_s7";
    edge_names[10] = "additive_closure";
    edge_names[11] = "power2";
    edge_names[12] = "gcd";

    /* Phase 1: Base edge types */
    fprintf(f, "--- Base Edge Types ---\n\n");
    for (i = 0; i < ne; i++) {
        Raq_GraphResult gr = raq_graph_analyze(&edges[i], count);
        raq_fprint_graph_result(f, edge_names[i], &gr);
        fprintf(f, "\n");
        raq_graph_result_free(&gr);
    }

    /* Phase 2: Cross-restriction (edge type x partition) */
    fprintf(f, "--- Restricted Analyses ---\n\n");
    for (i = 0; i < ne; i++) {
        for (j = 0; j < np; j++) {
            /* Skip self-restrictions (trivially complete within each class) */
            if (i == 0 && j == 0) continue;  /* same_axis | axis */
            if (i == 1 && j == 1) continue;  /* same_root | root */
            if (i == 2 && j == 2) continue;  /* same_galois | galois */
            if (i == 3 && j == 4) continue;  /* same_v2 | v2 */

            for (g = 0; g < parts[j]->n_groups; g++) {
                int sub_n;
                Raq_EdgeList sub_el;
                Raq_GraphResult sub_gr;
                char label[128];

                if (parts[j]->group_sizes[g] < 3) continue;
                sub_el = raq_el_restrict(&edges[i], parts[j], g, &sub_n);
                if (sub_el.count == 0) { raq_el_free(&sub_el); continue; }
                sub_gr = raq_graph_analyze(&sub_el, sub_n);

                if (sub_gr.est_bipartitus == 1 || sub_gr.est_arbor ||
                    sub_gr.est_completus || sub_gr.est_cyclus ||
                    sub_gr.est_dag == 1 || sub_gr.est_regularis) {
                    sprintf(label, "%s | %s#%d (n=%d)",
                            edge_names[i], part_names[j], g,
                            parts[j]->group_sizes[g]);
                    raq_fprint_graph_result(f, label, &sub_gr);
                }
                raq_graph_result_free(&sub_gr);
                raq_el_free(&sub_el);
            }
        }
    }

    /* Phase 3: Edge type intersections */
    fprintf(f, "\n--- Edge Type Intersections ---\n\n");
    for (i = 0; i < ne; i++) {
        for (j = i + 1; j < ne; j++) {
            Raq_EdgeList inter;
            Raq_GraphResult igr;
            char label[128];

            inter = raq_el_intersect(&edges[i], &edges[j], count);
            if (inter.count == 0) { raq_el_free(&inter); continue; }
            igr = raq_graph_analyze(&inter, count);
            sprintf(label, "%s ^ %s", edge_names[i], edge_names[j]);
            raq_fprint_graph_result(f, label, &igr);
            raq_graph_result_free(&igr);
            raq_el_free(&inter);
        }
    }

    /* Phase 4: Quotient graphs (edge type collapsed over partition) */
    fprintf(f, "\n--- Quotient Graphs ---\n\n");
    for (i = 0; i < ne; i++) {
        if (edges[i].count == 0) continue;
        for (j = 0; j < np; j++) {
            int qn;
            Raq_EdgeList qel;
            Raq_GraphResult qgr;
            char label[128];

            if (parts[j]->n_groups < 3) continue;
            qel = raq_el_quotient(&edges[i], parts[j], &qn);
            if (qel.count == 0) { raq_el_free(&qel); continue; }
            qgr = raq_graph_analyze(&qel, qn);
            sprintf(label, "%s / %s (%d groups)",
                    edge_names[i], part_names[j], qn);
            /* Print all quotients — they're small so always interesting */
            raq_fprint_graph_result(f, label, &qgr);
            raq_graph_result_free(&qgr);
            raq_el_free(&qel);
        }
    }

    /* Phase 5: Subgraph isomorphism across partition classes */
    fprintf(f, "\n--- Subgraph Isomorphism (degree-seq match) ---\n\n");
    for (i = 0; i < ne; i++) {
        if (edges[i].count == 0) continue;
        for (j = 0; j < np; j++) {
            int g1, g2, n_iso_classes = 0;
            /* Check all pairs of groups of size >= 3 */
            for (g1 = 0; g1 < parts[j]->n_groups; g1++) {
                int sub_n1;
                Raq_EdgeList sub1;
                if (parts[j]->group_sizes[g1] < 3) continue;
                sub1 = raq_el_restrict(&edges[i], parts[j], g1, &sub_n1);
                for (g2 = g1 + 1; g2 < parts[j]->n_groups; g2++) {
                    int sub_n2;
                    Raq_EdgeList sub2;
                    if (parts[j]->group_sizes[g2] < 3) continue;
                    sub2 = raq_el_restrict(&edges[i], parts[j], g2, &sub_n2);
                    if (raq_degree_seq_match(&sub1, sub_n1, &sub2, sub_n2)) {
                        if (n_iso_classes == 0)
                            fprintf(f, "  [%s | %s] iso pairs: ",
                                    edge_names[i], part_names[j]);
                        fprintf(f, "(%d,%d) ", g1, g2);
                        n_iso_classes++;
                    }
                    raq_el_free(&sub2);
                }
                raq_el_free(&sub1);
            }
            if (n_iso_classes > 0) fprintf(f, "[%d pairs]\n", n_iso_classes);
        }
    }

    /* Phase 6: Diagnostics — universal vertex, longest DAG chain */
    fprintf(f, "\n--- Diagnostics ---\n\n");
    for (i = 0; i < ne; i++) {
        if (edges[i].count == 0) continue;
        /* Check for universal vertex (degree = n-1) in undirected graphs */
        if (!edges[i].directus) {
            int *deg = (int *)calloc((size_t)count, sizeof(int));
            int k;
            for (k = 0; k < edges[i].count; k++) {
                deg[edges[i].src[k]]++;
                deg[edges[i].dst[k]]++;
            }
            for (k = 0; k < count; k++) {
                if (deg[k] == count - 1) {
                    fprintf(f, "  [%s] universal vertex: idx=%d = (%ld,%ld,%ld,%ld)\n",
                            edge_names[i], k,
                            (long)values[k].a, (long)values[k].b,
                            (long)values[k].c, (long)values[k].d);
                }
            }
            free(deg);
        }
        /* Print longest chain for DAGs */
        if (edges[i].directus) {
            Raq_GraphResult gr = raq_graph_analyze(&edges[i], count);
            if (gr.est_dag == 1 && gr.longitudo_catena > 0) {
                /* Reconstruct chain via BFS-topo */
                int *in_deg = (int *)calloc((size_t)count, sizeof(int));
                int *dist = (int *)calloc((size_t)count, sizeof(int));
                int *pred = (int *)malloc((size_t)count * sizeof(int));
                int *queue = (int *)malloc((size_t)count * sizeof(int));
                int qh = 0, qt = 0, k, best_v = 0;
                for (k = 0; k < count; k++) { dist[k] = 0; pred[k] = -1; }
                for (k = 0; k < edges[i].count; k++)
                    in_deg[edges[i].dst[k]]++;
                for (k = 0; k < count; k++)
                    if (in_deg[k] == 0) queue[qt++] = k;
                while (qh < qt) {
                    int u = queue[qh++];
                    for (k = 0; k < edges[i].count; k++) {
                        if (edges[i].src[k] == u) {
                            int v = edges[i].dst[k];
                            if (dist[u] + 1 > dist[v]) {
                                dist[v] = dist[u] + 1;
                                pred[v] = u;
                            }
                            in_deg[v]--;
                            if (in_deg[v] == 0) queue[qt++] = v;
                        }
                    }
                }
                for (k = 0; k < count; k++)
                    if (dist[k] > dist[best_v]) best_v = k;
                if (dist[best_v] > 0) {
                    int chain[64];
                    int clen = 0, cv = best_v;
                    while (cv != -1 && clen < 64) {
                        chain[clen++] = cv;
                        cv = pred[cv];
                    }
                    fprintf(f, "  [%s] longest chain (len %d): ",
                            edge_names[i], dist[best_v]);
                    for (k = clen - 1; k >= 0; k--) {
                        fprintf(f, "(%ld,%ld,%ld,%ld)",
                                (long)values[chain[k]].a,
                                (long)values[chain[k]].b,
                                (long)values[chain[k]].c,
                                (long)values[chain[k]].d);
                        if (k > 0) fprintf(f, " -> ");
                    }
                    fprintf(f, "\n");
                }
                free(in_deg); free(dist); free(pred); free(queue);
            }
            raq_graph_result_free(&gr);
        }
    }

    fprintf(f, "\n===== END SWEEP =====\n");

    for (i = 0; i < ne; i++) raq_el_free(&edges[i]);
    raq_vs_free(&vs);
    raq_partition_free(&p_axis);
    raq_partition_free(&p_root);
    raq_partition_free(&p_galois);
    raq_partition_free(&p_norm);
    raq_partition_free(&p_v2);
}

#endif /* RAQ_PRINT */

#endif /* RAQIYA_H */
