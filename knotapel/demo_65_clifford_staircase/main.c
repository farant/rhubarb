/*
 * KNOTAPEL DEMO 65: Clifford Hierarchy Staircase
 * ================================================
 *
 * Demo 63 proved the parity ceiling for Z[zeta_8] is n=5.
 * Demo 64 revealed the matroid funnel structure with a wall at n=6.
 *
 * This demo asks: does climbing the Clifford hierarchy to Z[zeta_16]
 * break through the wall?
 *
 * Z[zeta_16] has phi(16) = 8 components, 16 ray directions (22.5 deg).
 * Key difference: delta = -sqrt(2) != 0, so ALL braid states contribute
 * to the bracket (not just single-loop states).
 *
 * zeta_16 = e^{i*pi/8}  (T-gate phase: pi/8)
 * A = -zeta_16
 * delta = -A^2 - A^{-2} = -sqrt(2)
 *
 * Part A: Cyc16 arithmetic with verification tests
 * Part B: Build bracket catalog at zeta_16, analyze size and angular structure
 * Part C: Compare Z[zeta_8] and Z[zeta_16] regimes (embedding, overlap)
 *
 * C89, zero dependencies beyond math.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * Complex double type
 * ================================================================ */

typedef struct { double re, im; } Cx;

static Cx cx_make(double re, double im) {
    Cx z; z.re = re; z.im = im; return z;
}
static double cx_abs(Cx a) {
    return sqrt(a.re * a.re + a.im * a.im);
}

/* ================================================================
 * Exact cyclotomic type Z[zeta_16]
 * ================================================================
 * Basis: {1, zeta, zeta^2, ..., zeta^7}
 * Reduction: zeta^8 = -1
 * phi(16) = 8 components
 */

typedef struct { long c[8]; } Cyc16;

static Cyc16 cyc16_zero(void) {
    Cyc16 z;
    int i;
    for (i = 0; i < 8; i++) z.c[i] = 0;
    return z;
}

static Cyc16 cyc16_one(void) {
    Cyc16 z = cyc16_zero();
    z.c[0] = 1;
    return z;
}

static Cyc16 cyc16_make8(long c0, long c1, long c2, long c3,
                          long c4, long c5, long c6, long c7) {
    Cyc16 z;
    z.c[0] = c0; z.c[1] = c1; z.c[2] = c2; z.c[3] = c3;
    z.c[4] = c4; z.c[5] = c5; z.c[6] = c6; z.c[7] = c7;
    return z;
}

static int cyc16_eq(Cyc16 x, Cyc16 y) {
    int i;
    for (i = 0; i < 8; i++)
        if (x.c[i] != y.c[i]) return 0;
    return 1;
}

static Cyc16 cyc16_add(Cyc16 x, Cyc16 y) {
    Cyc16 r;
    int i;
    for (i = 0; i < 8; i++) r.c[i] = x.c[i] + y.c[i];
    return r;
}

static Cyc16 cyc16_neg(Cyc16 x) {
    Cyc16 r;
    int i;
    for (i = 0; i < 8; i++) r.c[i] = -x.c[i];
    return r;
}

/*
 * Multiplication in Z[zeta_16]:
 * zeta^i * zeta^j = zeta^{i+j} if i+j < 8
 *                 = -zeta^{i+j-8} if i+j >= 8
 */
static Cyc16 cyc16_mul(Cyc16 x, Cyc16 y) {
    Cyc16 r;
    int i, j, s;
    for (i = 0; i < 8; i++) r.c[i] = 0;
    for (i = 0; i < 8; i++) {
        if (x.c[i] == 0) continue;
        for (j = 0; j < 8; j++) {
            if (y.c[j] == 0) continue;
            s = i + j;
            if (s < 8) r.c[s]     += x.c[i] * y.c[j];
            else        r.c[s - 8] -= x.c[i] * y.c[j];
        }
    }
    return r;
}

/*
 * Conjugate: conj(zeta^k) = zeta^{-k} = zeta^{16-k} = -zeta^{8-k}
 * conj(a0 + a1*z + ... + a7*z^7) = a0 - a7*z - a6*z^2 - ... - a1*z^7
 */
static Cyc16 cyc16_conj(Cyc16 z) {
    return cyc16_make8(z.c[0], -z.c[7], -z.c[6], -z.c[5],
                       -z.c[4], -z.c[3], -z.c[2], -z.c[1]);
}

static Cyc16 cyc16_pow_int(Cyc16 base, int n) {
    Cyc16 r = cyc16_one();
    if (n == 0) return r;
    if (n < 0) { n = -n; base = cyc16_conj(base); }
    while (n > 0) {
        if (n & 1) r = cyc16_mul(r, base);
        base = cyc16_mul(base, base);
        n >>= 1;
    }
    return r;
}

/*
 * Convert to complex double:
 * zeta_16^k = cos(k*pi/8) + i*sin(k*pi/8)
 */
static Cx cyc16_to_cx(Cyc16 z) {
    double re = 0.0, im = 0.0;
    int k;
    for (k = 0; k < 8; k++) {
        double angle = (double)k * M_PI / 8.0;
        re += (double)z.c[k] * cos(angle);
        im += (double)z.c[k] * sin(angle);
    }
    return cx_make(re, im);
}

/* ================================================================
 * Exact cyclotomic type Z[zeta_8] (for embedding comparison)
 * ================================================================ */

typedef struct { long a, b, c, d; } Cyc8;

static Cyc8 cyc8_make(long a, long b, long c, long d) {
    Cyc8 z; z.a = a; z.b = b; z.c = c; z.d = d; return z;
}
static Cyc8 cyc8_zero(void) { return cyc8_make(0, 0, 0, 0); }
static Cyc8 cyc8_one(void)  { return cyc8_make(1, 0, 0, 0); }
static int cyc8_eq(Cyc8 x, Cyc8 y) {
    return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}
static Cyc8 cyc8_add(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a + y.a, x.b + y.b, x.c + y.c, x.d + y.d);
}
static Cyc8 cyc8_conj(Cyc8 z) {
    return cyc8_make(z.a, -z.d, -z.c, -z.b);
}
static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y) {
    return cyc8_make(
        x.a * y.a - x.b * y.d - x.c * y.c - x.d * y.b,
        x.a * y.b + x.b * y.a - x.c * y.d - x.d * y.c,
        x.a * y.c + x.b * y.b + x.c * y.a - x.d * y.d,
        x.a * y.d + x.b * y.c + x.c * y.b + x.d * y.a);
}
static Cyc8 cyc8_pow_int(Cyc8 base, int n) {
    Cyc8 r = cyc8_one();
    if (n == 0) return r;
    if (n < 0) { n = -n; base = cyc8_conj(base); }
    while (n > 0) {
        if (n & 1) r = cyc8_mul(r, base);
        base = cyc8_mul(base, base);
        n >>= 1;
    }
    return r;
}

/* Embed Cyc8 into Cyc16 via zeta_8 = zeta_16^2 */
static Cyc16 cyc8_to_cyc16(Cyc8 z) {
    /* a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3
     * = a + b*zeta_16^2 + c*zeta_16^4 + d*zeta_16^6 */
    return cyc16_make8(z.a, 0, z.b, 0, z.c, 0, z.d, 0);
}

/* Check if a Cyc16 value lies in Z[zeta_8] subring */
static int cyc16_in_zeta8(Cyc16 z) {
    return z.c[1] == 0 && z.c[3] == 0 && z.c[5] == 0 && z.c[7] == 0;
}

/* Extract Cyc8 from Cyc16 (assumes cyc16_in_zeta8 is true) */
static Cyc8 cyc16_to_cyc8(Cyc16 z) {
    return cyc8_make(z.c[0], z.c[2], z.c[4], z.c[6]);
}

/* ================================================================
 * Braid type + union-find loop counter
 * ================================================================ */

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

#define MAX_UF 4096
static int uf_p[MAX_UF];

static void uf_init(int n) {
    int i;
    for (i = 0; i < n; i++) uf_p[i] = i;
}
static int uf_find(int x) {
    while (uf_p[x] != x) { uf_p[x] = uf_p[uf_p[x]]; x = uf_p[x]; }
    return x;
}
static void uf_union(int x, int y) {
    x = uf_find(x); y = uf_find(y);
    if (x != y) uf_p[x] = y;
}

static int braid_loops(const Braid *b, unsigned s) {
    int N = (b->len + 1) * b->n;
    int l, p, i, loops, sgn, bit, cup;
    uf_init(N);
    for (l = 0; l < b->len; l++) {
        sgn = b->word[l] > 0 ? 1 : -1;
        i = (sgn > 0 ? b->word[l] : -b->word[l]) - 1;
        bit = (int)((s >> (unsigned)l) & 1u);
        cup = (sgn > 0) ? (bit == 0) : (bit == 1);
        if (cup) {
            uf_union(l * b->n + i, l * b->n + i + 1);
            uf_union((l + 1) * b->n + i, (l + 1) * b->n + i + 1);
            for (p = 0; p < b->n; p++)
                if (p != i && p != i + 1)
                    uf_union(l * b->n + p, (l + 1) * b->n + p);
        } else {
            for (p = 0; p < b->n; p++)
                uf_union(l * b->n + p, (l + 1) * b->n + p);
        }
    }
    for (p = 0; p < b->n; p++)
        uf_union(p, b->len * b->n + p);
    loops = 0;
    for (i = 0; i < N; i++)
        if (uf_find(i) == i) loops++;
    return loops;
}

/* ================================================================
 * Catalog builders
 * ================================================================ */

/* Precomputed delta powers: delta^k for k = 0..MAX_DELTA_POW
 * delta = -sqrt(2) in Z[zeta_16] = (0, 0, -1, 0, 0, 0, 1, 0)
 * delta^2 = 2, delta^3 = 2*delta, delta^4 = 4, etc. */
#define MAX_DELTA_POW 20
static Cyc16 delta_pow[MAX_DELTA_POW + 1];

static void build_delta_powers(void) {
    Cyc16 delta = cyc16_make8(0, 0, -1, 0, 0, 0, 1, 0);
    int k;
    delta_pow[0] = cyc16_one();
    for (k = 1; k <= MAX_DELTA_POW; k++)
        delta_pow[k] = cyc16_mul(delta_pow[k - 1], delta);
}

/* Z[zeta_16] catalog at delta = -sqrt(2) */
#define MAX_CAT16 8192

static Cyc16 cat16_exact[MAX_CAT16];
static Cx    cat16_cx[MAX_CAT16];
static int   cat16_size = 0;

static int find_in_16(Cyc16 val) {
    int i;
    for (i = 0; i < cat16_size; i++)
        if (cyc16_eq(val, cat16_exact[i])) return i;
    return -1;
}

static void build_catalog_16(void) {
    Cyc16 A = cyc16_make8(0, -1, 0, 0, 0, 0, 0, 0);
    int n, len;
    Braid b;
    int word_buf[MAX_WORD];

    cat16_size = 0;
    build_delta_powers();

    for (n = 2; n <= 3; n++) {
        for (len = 1; len <= 8 && len <= MAX_WORD; len++) {
            int max_gen = n - 1;
            int total_gens = 2 * max_gen;
            unsigned long total, idx;
            int i;

            total = 1;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)total_gens;
                if (total > 100000) break;
            }
            if (total > 100000) continue;

            for (idx = 0; idx < total; idx++) {
                unsigned long tmp = idx;
                Cyc16 bracket;
                unsigned s, ns;

                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen) word_buf[i] = g + 1;
                    else             word_buf[i] = -(g - max_gen + 1);
                }
                b.n = n; b.len = len;
                memcpy(b.word, word_buf, (size_t)len * sizeof(int));

                bracket = cyc16_zero();
                ns = 1u << b.len;
                for (s = 0; s < ns; s++) {
                    int a_count = 0, b_count = 0, lp;
                    for (i = 0; i < b.len; i++) {
                        if ((s >> (unsigned)i) & 1u) b_count++;
                        else a_count++;
                    }
                    lp = braid_loops(&b, s);
                    /* All states contribute: bracket += A^{a-b} * delta^{loops-1} */
                    if (lp - 1 <= MAX_DELTA_POW) {
                        Cyc16 term = cyc16_mul(
                            cyc16_pow_int(A, a_count - b_count),
                            delta_pow[lp - 1]);
                        bracket = cyc16_add(bracket, term);
                    }
                }

                if (!cyc16_eq(bracket, cyc16_zero())
                    && find_in_16(bracket) < 0
                    && cat16_size < MAX_CAT16) {
                    cat16_exact[cat16_size] = bracket;
                    cat16_cx[cat16_size] = cyc16_to_cx(bracket);
                    cat16_size++;
                }
            }
        }
    }
}

/* Z[zeta_8] catalog at delta = 0 (single-loop only) */
#define MAX_CAT8 512

static Cyc8 cat8_exact[MAX_CAT8];
static int  cat8_size = 0;

static int find_in_8(Cyc8 val) {
    int i;
    for (i = 0; i < cat8_size; i++)
        if (cyc8_eq(val, cat8_exact[i])) return i;
    return -1;
}

static void build_catalog_8(void) {
    Cyc8 A = cyc8_make(0, -1, 0, 0);
    int n, len;
    Braid b;
    int word_buf[MAX_WORD];

    cat8_size = 0;

    for (n = 2; n <= 3; n++) {
        for (len = 1; len <= 8 && len <= MAX_WORD; len++) {
            int max_gen = n - 1;
            int total_gens = 2 * max_gen;
            unsigned long total, idx;
            int i;

            total = 1;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)total_gens;
                if (total > 100000) break;
            }
            if (total > 100000) continue;

            for (idx = 0; idx < total; idx++) {
                unsigned long tmp = idx;
                Cyc8 bracket;
                unsigned s, ns;

                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen) word_buf[i] = g + 1;
                    else             word_buf[i] = -(g - max_gen + 1);
                }
                b.n = n; b.len = len;
                memcpy(b.word, word_buf, (size_t)len * sizeof(int));

                bracket = cyc8_zero();
                ns = 1u << b.len;
                for (s = 0; s < ns; s++) {
                    int a_count = 0, b_count = 0, lp;
                    for (i = 0; i < b.len; i++) {
                        if ((s >> (unsigned)i) & 1u) b_count++;
                        else a_count++;
                    }
                    lp = braid_loops(&b, s);
                    if (lp == 1) {
                        bracket = cyc8_add(bracket,
                            cyc8_pow_int(A, a_count - b_count));
                    }
                }

                if (!cyc8_eq(bracket, cyc8_zero())
                    && find_in_8(bracket) < 0
                    && cat8_size < MAX_CAT8) {
                    cat8_exact[cat8_size] = bracket;
                    cat8_size++;
                }
            }
        }
    }
}

/* ================================================================
 * Sector classification
 * ================================================================ */

static int sector_classify(Cx z, int k) {
    double angle, sw;
    int sec;
    if (cx_abs(z) < 1e-15) return 0;
    angle = atan2(z.im, z.re);
    if (angle < 0.0) angle += 2.0 * M_PI;
    sw = 2.0 * M_PI / (double)k;
    sec = (int)(angle / sw);
    if (sec >= k) sec = k - 1;
    return sec % 2;
}

/* ================================================================
 * Part A: Cyc16 arithmetic verification
 * ================================================================ */

static void part_a_arithmetic(void) {
    Cyc16 zeta, z8, z16, A, Ainv, delta, dsq, prod, one, neg_one;
    Cx zcx;
    double err;
    char msg[256];

    printf("\n=== Part A: Cyc16 arithmetic verification ===\n");

    zeta = cyc16_make8(0, 1, 0, 0, 0, 0, 0, 0);
    one = cyc16_one();
    neg_one = cyc16_make8(-1, 0, 0, 0, 0, 0, 0, 0);

    /* A1: zeta^8 = -1 (fundamental relation) */
    z8 = cyc16_pow_int(zeta, 8);
    check("zeta^8 = -1", cyc16_eq(z8, neg_one));

    /* A2: zeta^16 = 1 */
    z16 = cyc16_pow_int(zeta, 16);
    check("zeta^16 = 1", cyc16_eq(z16, one));

    /* A3: zeta^4 = i = (0,0,0,0,1,0,0,0) */
    {
        Cyc16 z4 = cyc16_pow_int(zeta, 4);
        Cyc16 expected_i = cyc16_make8(0, 0, 0, 0, 1, 0, 0, 0);
        check("zeta^4 = i", cyc16_eq(z4, expected_i));
    }

    /* A4: |zeta| = 1 */
    zcx = cyc16_to_cx(zeta);
    err = fabs(cx_abs(zcx) - 1.0);
    sprintf(msg, "|zeta_16| = 1 (err=%.2e)", err);
    check(msg, err < 1e-12);

    /* A5: zeta * conj(zeta) = 1 */
    prod = cyc16_mul(zeta, cyc16_conj(zeta));
    check("zeta * conj(zeta) = 1", cyc16_eq(prod, one));

    /* A6: A = -zeta, A * A^{-1} = 1 */
    A = cyc16_make8(0, -1, 0, 0, 0, 0, 0, 0);
    Ainv = cyc16_conj(A);
    prod = cyc16_mul(A, Ainv);
    check("A * A^{-1} = 1", cyc16_eq(prod, one));

    /* A7: delta = -A^2 - A^{-2} = (0,0,-1,0,0,0,1,0) */
    {
        Cyc16 A2 = cyc16_mul(A, A);
        Cyc16 Ainv2 = cyc16_mul(Ainv, Ainv);
        delta = cyc16_add(cyc16_neg(A2), cyc16_neg(Ainv2));
    }
    {
        Cyc16 expected_delta = cyc16_make8(0, 0, -1, 0, 0, 0, 1, 0);
        check("delta = (0,0,-1,0,0,0,1,0)", cyc16_eq(delta, expected_delta));
    }

    /* A8: delta^2 = 2 */
    dsq = cyc16_mul(delta, delta);
    check("delta^2 = 2", cyc16_eq(dsq, cyc16_make8(2, 0, 0, 0, 0, 0, 0, 0)));

    /* A9: delta as complex number = -sqrt(2) */
    {
        Cx dcx = cyc16_to_cx(delta);
        double d_err = fabs(dcx.re + sqrt(2.0)) + fabs(dcx.im);
        sprintf(msg, "delta = -sqrt(2) as complex (err=%.2e)", d_err);
        check(msg, d_err < 1e-12);
    }

    /* A10: zeta_16^2 = basis element c[2] (embedding anchor) */
    {
        Cyc16 z16_sq = cyc16_mul(zeta, zeta);
        Cyc16 expected = cyc16_make8(0, 0, 1, 0, 0, 0, 0, 0);
        check("zeta_16^2 = basis[2]", cyc16_eq(z16_sq, expected));
    }

    /* A11: Cyc8 -> Cyc16 embedding roundtrip */
    {
        Cyc8 v8 = cyc8_make(3, -1, 2, 5);
        Cyc16 v16 = cyc8_to_cyc16(v8);
        Cyc16 expected = cyc16_make8(3, 0, -1, 0, 2, 0, 5, 0);
        int ok = cyc16_eq(v16, expected) && cyc16_in_zeta8(v16);
        check("Cyc8->Cyc16 embedding + subring check", ok);
    }

    /* A12: Multiplication commutative */
    {
        Cyc16 a = cyc16_make8(2, -1, 3, 0, 1, -2, 0, 4);
        Cyc16 b = cyc16_make8(-1, 0, 2, 1, -3, 0, 1, -1);
        Cyc16 ab = cyc16_mul(a, b);
        Cyc16 ba = cyc16_mul(b, a);
        check("Multiplication commutative", cyc16_eq(ab, ba));
    }

    /* A13: Delta power precomputation */
    {
        int ok = 1;
        build_delta_powers();
        if (!cyc16_eq(delta_pow[0], one)) ok = 0;
        if (!cyc16_eq(delta_pow[1], cyc16_make8(0, 0, -1, 0, 0, 0, 1, 0))) ok = 0;
        if (!cyc16_eq(delta_pow[2], cyc16_make8(2, 0, 0, 0, 0, 0, 0, 0))) ok = 0;
        if (!cyc16_eq(delta_pow[3], cyc16_make8(0, 0, -2, 0, 0, 0, 2, 0))) ok = 0;
        if (!cyc16_eq(delta_pow[4], cyc16_make8(4, 0, 0, 0, 0, 0, 0, 0))) ok = 0;
        check("Delta powers d^0..d^4 correct", ok);
    }

    /* A14: Embedded Cyc8 multiplication matches Cyc16 multiplication */
    {
        Cyc8 a8 = cyc8_make(1, -2, 0, 3);
        Cyc8 b8 = cyc8_make(-1, 1, 2, 0);
        Cyc8 ab8 = cyc8_mul(a8, b8);
        Cyc16 a16 = cyc8_to_cyc16(a8);
        Cyc16 b16 = cyc8_to_cyc16(b8);
        Cyc16 ab16 = cyc16_mul(a16, b16);
        Cyc16 expected = cyc8_to_cyc16(ab8);
        check("Cyc8 mul embeds correctly into Cyc16", cyc16_eq(ab16, expected));
    }
}

/* ================================================================
 * Part B: Catalog at zeta_16
 * ================================================================ */

static void part_b_catalog(void) {
    int i, n_axial, n_in_z8;
    int dir_count[16];
    int n_distinct_dirs;
    char msg[256];

    printf("\n=== Part B: Z[zeta_16] catalog at delta = -sqrt(2) ===\n");

    build_catalog_16();

    sprintf(msg, "Catalog built: %d values (max %d)", cat16_size, MAX_CAT16);
    check(msg, cat16_size > 0 && cat16_size < MAX_CAT16);

    printf("  INFO: Z[zeta_16] catalog has %d distinct nonzero bracket values\n",
           cat16_size);

    /* Axiality check */
    n_axial = 0;
    for (i = 0; i < cat16_size; i++) {
        int nz = 0, k;
        for (k = 0; k < 8; k++)
            if (cat16_exact[i].c[k] != 0) nz++;
        if (nz == 1) n_axial++;
    }
    printf("  INFO: Axial values (single nonzero component): %d / %d (%.1f%%)\n",
           n_axial, cat16_size, 100.0 * (double)n_axial / (double)cat16_size);
    sprintf(msg, "Axiality: %d/%d axial", n_axial, cat16_size);
    check(msg, 1);

    /* Values in Z[zeta_8] subring */
    n_in_z8 = 0;
    for (i = 0; i < cat16_size; i++) {
        if (cyc16_in_zeta8(cat16_exact[i])) n_in_z8++;
    }
    printf("  INFO: Values in Z[zeta_8] subring: %d / %d (%.1f%%)\n",
           n_in_z8, cat16_size, 100.0 * (double)n_in_z8 / (double)cat16_size);

    /* Angular direction distribution (16 sectors of 22.5 degrees) */
    for (i = 0; i < 16; i++) dir_count[i] = 0;
    for (i = 0; i < cat16_size; i++) {
        double angle = atan2(cat16_cx[i].im, cat16_cx[i].re);
        int dir;
        if (angle < 0.0) angle += 2.0 * M_PI;
        dir = (int)(angle / (M_PI / 8.0));
        if (dir > 15) dir = 15;
        dir_count[dir]++;
    }
    n_distinct_dirs = 0;
    printf("  INFO: Direction distribution (16 sectors of 22.5 deg):\n");
    for (i = 0; i < 16; i++) {
        if (dir_count[i] > 0) {
            n_distinct_dirs++;
            printf("    dir %2d (%5.1f deg): %d values\n",
                   i, (double)i * 22.5, dir_count[i]);
        }
    }
    sprintf(msg, "Distinct angular directions used: %d/16", n_distinct_dirs);
    check(msg, 1);

    /* Component usage */
    {
        int comp_used[8];
        int k, n_comp = 0;
        for (k = 0; k < 8; k++) comp_used[k] = 0;
        for (i = 0; i < cat16_size; i++) {
            int k2;
            for (k2 = 0; k2 < 8; k2++)
                if (cat16_exact[i].c[k2] != 0) comp_used[k2] = 1;
        }
        printf("  INFO: Component usage: ");
        for (k = 0; k < 8; k++) {
            if (comp_used[k]) { printf("c%d ", k); n_comp++; }
        }
        printf("(%d/8 used)\n", n_comp);
    }

    /* Max coefficient magnitude */
    {
        long max_coeff = 0;
        for (i = 0; i < cat16_size; i++) {
            int k;
            for (k = 0; k < 8; k++) {
                long v = cat16_exact[i].c[k];
                if (v < 0) v = -v;
                if (v > max_coeff) max_coeff = v;
            }
        }
        printf("  INFO: Max coefficient magnitude: %ld\n", max_coeff);
    }

    /* Nonzero component count distribution */
    {
        int nz_dist[9];
        int k;
        for (k = 0; k <= 8; k++) nz_dist[k] = 0;
        for (i = 0; i < cat16_size; i++) {
            int nz = 0;
            for (k = 0; k < 8; k++)
                if (cat16_exact[i].c[k] != 0) nz++;
            nz_dist[nz]++;
        }
        printf("  INFO: Nonzero component distribution:\n");
        for (k = 0; k <= 8; k++) {
            if (nz_dist[k] > 0)
                printf("    %d nonzero: %d values\n", k, nz_dist[k]);
        }
    }

    /* First 10 catalog values */
    {
        int lim = cat16_size < 10 ? cat16_size : 10;
        printf("  INFO: First %d catalog values:\n", lim);
        for (i = 0; i < lim; i++) {
            Cx z = cat16_cx[i];
            printf("    [%3d] (%2ld,%2ld,%2ld,%2ld,%2ld,%2ld,%2ld,%2ld) = %.4f%+.4fi\n",
                   i,
                   cat16_exact[i].c[0], cat16_exact[i].c[1],
                   cat16_exact[i].c[2], cat16_exact[i].c[3],
                   cat16_exact[i].c[4], cat16_exact[i].c[5],
                   cat16_exact[i].c[6], cat16_exact[i].c[7],
                   z.re, z.im);
        }
    }
}

/* ================================================================
 * Part C: Regime comparison
 * ================================================================ */

static void part_c_comparison(void) {
    int i, n_z8_found;
    char msg[256];

    printf("\n=== Part C: Z[zeta_8] vs Z[zeta_16] regime comparison ===\n");

    build_catalog_8();
    printf("  INFO: Z[zeta_8] catalog: %d values (delta=0, single-loop)\n", cat8_size);
    printf("  INFO: Z[zeta_16] catalog: %d values (delta=-sqrt(2), all loops)\n",
           cat16_size);

    sprintf(msg, "Z[zeta_8] catalog size: %d (expected 100)", cat8_size);
    check(msg, cat8_size == 100);

    /* Which Z[zeta_8] catalog values appear in Z[zeta_16] catalog? */
    n_z8_found = 0;
    for (i = 0; i < cat8_size; i++) {
        Cyc16 embedded = cyc8_to_cyc16(cat8_exact[i]);
        if (find_in_16(embedded) >= 0) n_z8_found++;
    }
    printf("  INFO: Z[zeta_8] values found in Z[zeta_16] catalog: %d / %d\n",
           n_z8_found, cat8_size);
    sprintf(msg, "Z[zeta_8] -> Z[zeta_16] overlap: %d/%d",
            n_z8_found, cat8_size);
    check(msg, 1);

    /* Z[zeta_16] values in Z[zeta_8] subring */
    {
        int n_in_z8 = 0, n_match = 0, n_new = 0;
        for (i = 0; i < cat16_size; i++) {
            if (cyc16_in_zeta8(cat16_exact[i])) {
                Cyc8 as_z8 = cyc16_to_cyc8(cat16_exact[i]);
                n_in_z8++;
                if (find_in_8(as_z8) >= 0) n_match++;
                else n_new++;
            }
        }
        printf("  INFO: Z[zeta_16] values in Z[zeta_8] subring: %d\n", n_in_z8);
        printf("  INFO:   matching Z[zeta_8] catalog: %d, new: %d\n",
               n_match, n_new);
    }

    /* Sector analysis at key k values */
    {
        int k_values[] = {6, 8, 15, 31};
        int nk = 4;
        int ki;
        printf("  INFO: Class-1 values by sector count:\n");
        printf("  INFO:   %6s  %12s  %12s\n", "k", "Z[zeta_8]", "Z[zeta_16]");
        for (ki = 0; ki < nk; ki++) {
            int k = k_values[ki];
            int n1_z8 = 0, n1_z16 = 0;
            for (i = 0; i < cat8_size; i++) {
                Cx z = cyc16_to_cx(cyc8_to_cyc16(cat8_exact[i]));
                if (sector_classify(z, k) == 1) n1_z8++;
            }
            for (i = 0; i < cat16_size; i++) {
                if (sector_classify(cat16_cx[i], k) == 1) n1_z16++;
            }
            printf("  INFO:   k=%4d  %5d/%-5d  %5d/%-5d\n",
                   k, n1_z8, cat8_size, n1_z16, cat16_size);
        }
    }

    /* Pigeonhole analysis for Z[zeta_16] */
    {
        int dir_count[16];
        int n_dirs = 0;
        for (i = 0; i < 16; i++) dir_count[i] = 0;
        for (i = 0; i < cat16_size; i++) {
            double angle = atan2(cat16_cx[i].im, cat16_cx[i].re);
            int dir;
            if (angle < 0.0) angle += 2.0 * M_PI;
            dir = (int)(angle / (M_PI / 8.0));
            if (dir > 15) dir = 15;
            dir_count[dir]++;
        }
        for (i = 0; i < 16; i++)
            if (dir_count[i] > 0) n_dirs++;
        printf("  INFO: Z[zeta_16] uses %d/16 angular directions\n", n_dirs);
        printf("  INFO: Pigeonhole bound for parity: n <= %d (need distinct dirs)\n",
               n_dirs);
        printf("  INFO: Z[zeta_8] pigeonhole was n <= 7 (8 dirs, minus dir 0)\n");
        printf("  INFO: Z[zeta_8] actual ceiling was n = 5 (gap of 2)\n");
        printf("  INFO: If gap = 2 pattern holds: Z[zeta_16] ceiling ~ n = %d\n",
               n_dirs - 2);
    }

    /* Universal k comparison */
    {
        int n1_k15_z16 = 0, n1_k31_z16 = 0;
        for (i = 0; i < cat16_size; i++) {
            if (sector_classify(cat16_cx[i], 15) == 1) n1_k15_z16++;
            if (sector_classify(cat16_cx[i], 31) == 1) n1_k31_z16++;
        }
        printf("  INFO: Z[zeta_16] class-1 at k=15 (Z[zeta_8] universal): %d/%d\n",
               n1_k15_z16, cat16_size);
        printf("  INFO: Z[zeta_16] class-1 at k=31 (Z[zeta_16] universal 2*16-1): %d/%d\n",
               n1_k31_z16, cat16_size);
        sprintf(msg, "k=31 class-1 catalog values: %d", n1_k31_z16);
        check(msg, 1);
    }
}

/* ================================================================
 * Part D: NPN tier probe at Z[zeta_16]
 * ================================================================
 * Recursive XOR search with subset-sum pruning.
 * ps16[mask] holds the sum of chosen weights for each subset mask.
 * At depth d, we've chosen w_idx[0..d-1]. We try each class-1
 * value for w_idx[d], compute all new subset sums involving position d,
 * check each against XOR parity, and prune on first failure.
 */

static Cyc16 ps16[64]; /* partial subset sums, up to 6 inputs */
static int xor16_count;

static int popcount_small(int x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

static void search_xor16_r(int *w_idx, int depth, int n, int k,
                            const int *class1, int n_class1) {
    int ci;
    int ms = 1 << depth;
    int me = 1 << (depth + 1);

    for (ci = 0; ci < n_class1; ci++) {
        int idx = class1[ci];
        Cyc16 wv = cat16_exact[idx];
        int ok = 1;
        int mask;

        w_idx[depth] = idx;

        for (mask = ms; mask < me; mask++) {
            Cyc16 ns = cyc16_add(ps16[mask ^ ms], wv);
            Cx scx = cyc16_to_cx(ns);
            int cls = sector_classify(scx, k);
            if (cls != (popcount_small(mask) % 2)) {
                ok = 0;
                break;
            }
            ps16[mask] = ns;
        }

        if (ok) {
            if (depth + 1 == n) {
                xor16_count++;
            } else {
                search_xor16_r(w_idx, depth + 1, n, k, class1, n_class1);
            }
        }
    }
}

static int count_xor16(int n, int k) {
    int class1[MAX_CAT16];
    int n_c1 = 0;
    int w_idx[8];
    int i;

    for (i = 0; i < cat16_size; i++) {
        if (sector_classify(cat16_cx[i], k) == 1)
            class1[n_c1++] = i;
    }

    ps16[0] = cyc16_zero();
    xor16_count = 0;
    search_xor16_r(w_idx, 0, n, k, class1, n_c1);
    return xor16_count;
}

static void part_d_tier_probe(void) {
    int k, count;
    char msg[256];

    printf("\n=== Part D: NPN tier probe at Z[zeta_16] ===\n");

    /* XOR3 sweep across k=2..31 */
    printf("  INFO: XOR3 solutions by sector count:\n");
    {
        int any_xor3 = 0;
        for (k = 2; k <= 31; k++) {
            count = count_xor16(3, k);
            if (count > 0) {
                printf("    k=%2d: %6d solutions\n", k, count);
                any_xor3 = 1;
            }
        }
        check("XOR3 achievable at Z[zeta_16]", any_xor3);
    }

    /* XOR4 sweep across k=2..31 */
    printf("  INFO: XOR4 solutions by sector count:\n");
    {
        int any_xor4 = 0;
        for (k = 2; k <= 31; k++) {
            count = count_xor16(4, k);
            if (count > 0) {
                printf("    k=%2d: %6d solutions\n", k, count);
                any_xor4 = 1;
            }
        }
        check("XOR4 achievable at Z[zeta_16]", any_xor4);
    }

    /* XOR5 sweep across k=2..31 */
    printf("  INFO: XOR5 solutions by sector count:\n");
    {
        int any_xor5 = 0;
        for (k = 2; k <= 31; k++) {
            printf("    k=%2d: ", k);
            fflush(stdout);
            count = count_xor16(5, k);
            printf("%6d solutions\n", count);
            if (count > 0) any_xor5 = 1;
        }
        sprintf(msg, "XOR5 achievable at Z[zeta_16]: %s",
                any_xor5 ? "YES" : "NO");
        check(msg, 1);
    }
}

/* ================================================================
 * Part E: XOR6 search (the big question)
 * ================================================================ */

static void part_e_xor6(void) {
    int count;
    char msg[256];

    printf("\n=== Part E: XOR6 search at Z[zeta_16] ===\n");
    printf("  INFO: Does climbing to Z[zeta_16] break the n=5 ceiling?\n");

    /* XOR6 at k=31 (universal k for Z[zeta_16]) */
    printf("  INFO: XOR6 at k=31 (universal k = 2*16-1)...");
    fflush(stdout);
    count = count_xor16(6, 31);
    printf(" %d solutions\n", count);

    if (count > 0) {
        sprintf(msg, "*** XOR6 ACHIEVABLE at Z[zeta_16] k=31: %d solutions ***",
                count);
        check(msg, 1);

        /* If XOR6 works, try XOR7 */
        printf("  INFO: XOR7 at k=31...");
        fflush(stdout);
        count = count_xor16(7, 31);
        printf(" %d solutions\n", count);
        sprintf(msg, "XOR7 at k=31: %d solutions", count);
        check(msg, 1);
    } else {
        check("XOR6 NOT achievable at Z[zeta_16] k=31", 1);

        /* Try other promising k values */
        printf("  INFO: Trying other k values for XOR6...\n");
        {
            int kvals[] = {12, 15, 16, 20, 24, 30};
            int nk = 6;
            int ki;
            int any = 0;
            for (ki = 0; ki < nk; ki++) {
                printf("    k=%2d: ", kvals[ki]);
                fflush(stdout);
                count = count_xor16(6, kvals[ki]);
                printf("%d solutions\n", count);
                if (count > 0) any = 1;
            }
            if (any) {
                check("XOR6 achievable at some k", 1);
            } else {
                check("XOR6 NOT achievable at any tested k", 1);
            }
        }
    }
}

/* ================================================================
 * Part F: Catalog convergence check
 * ================================================================
 * Does extending to 4 strands or longer braids add new values?
 * The 36-value catalog from 2-3 strands len<=8 might be incomplete.
 */

static void part_f_convergence(void) {
    Cyc16 A = cyc16_make8(0, -1, 0, 0, 0, 0, 0, 0);
    int n, len;
    Braid b;
    int word_buf[MAX_WORD];
    int new_from_4strand = 0;
    char msg[256];

    printf("\n=== Part F: Catalog convergence check ===\n");
    printf("  INFO: Current catalog from 2-3 strands len<=8: %d values\n",
           cat16_size);

    /* Report how many values come from each (n_strands, length) */
    printf("  INFO: Catalog growth by (strands, length):\n");
    {
        int running = 0;
        int prev_cat = 0;
        Cyc16 tmp_cat[MAX_CAT16];
        int tmp_size = 0;

        for (n = 2; n <= 3; n++) {
            for (len = 1; len <= 8; len++) {
                int max_gen = n - 1;
                int total_gens = 2 * max_gen;
                unsigned long total, idx;
                int new_this = 0, i;

                total = 1;
                for (i = 0; i < len; i++) {
                    total *= (unsigned long)total_gens;
                    if (total > 100000) break;
                }
                if (total > 100000) continue;

                for (idx = 0; idx < total; idx++) {
                    unsigned long tmp = idx;
                    Cyc16 bracket;
                    unsigned s, ns;

                    for (i = 0; i < len; i++) {
                        int g = (int)(tmp % (unsigned long)total_gens);
                        tmp /= (unsigned long)total_gens;
                        if (g < max_gen) word_buf[i] = g + 1;
                        else             word_buf[i] = -(g - max_gen + 1);
                    }
                    b.n = n; b.len = len;
                    memcpy(b.word, word_buf, (size_t)len * sizeof(int));

                    bracket = cyc16_zero();
                    ns = 1u << b.len;
                    for (s = 0; s < ns; s++) {
                        int a_count = 0, b_count = 0, lp;
                        for (i = 0; i < b.len; i++) {
                            if ((s >> (unsigned)i) & 1u) b_count++;
                            else a_count++;
                        }
                        lp = braid_loops(&b, s);
                        if (lp - 1 <= MAX_DELTA_POW) {
                            Cyc16 term = cyc16_mul(
                                cyc16_pow_int(A, a_count - b_count),
                                delta_pow[lp - 1]);
                            bracket = cyc16_add(bracket, term);
                        }
                    }

                    if (!cyc16_eq(bracket, cyc16_zero())) {
                        int found = 0, j;
                        for (j = 0; j < tmp_size; j++) {
                            if (cyc16_eq(bracket, tmp_cat[j])) {
                                found = 1;
                                break;
                            }
                        }
                        if (!found && tmp_size < MAX_CAT16) {
                            tmp_cat[tmp_size++] = bracket;
                            new_this++;
                        }
                    }
                }
                if (new_this > 0) {
                    printf("    n=%d len=%d: +%d new (total %d)\n",
                           n, len, new_this, tmp_size);
                }
                running = tmp_size;
            }
        }
        (void)prev_cat;
        (void)running;
    }

    /* Now extend to 4-strand braids */
    printf("  INFO: Extending to 4-strand braids...\n");
    {
        int max_gen = 3;
        int total_gens = 6;

        n = 4;
        for (len = 1; len <= 8; len++) {
            unsigned long total, idx;
            int new_this = 0, i;

            total = 1;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)total_gens;
                if (total > 100000) break;
            }
            if (total > 100000) {
                printf("    n=4 len=%d: skipped (%lu > 100000)\n",
                       len, total);
                continue;
            }

            for (idx = 0; idx < total; idx++) {
                unsigned long tmp = idx;
                Cyc16 bracket;
                unsigned s, ns;

                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen) word_buf[i] = g + 1;
                    else             word_buf[i] = -(g - max_gen + 1);
                }
                b.n = n; b.len = len;
                memcpy(b.word, word_buf, (size_t)len * sizeof(int));

                bracket = cyc16_zero();
                ns = 1u << b.len;
                for (s = 0; s < ns; s++) {
                    int a_count = 0, b_count = 0, lp;
                    for (i = 0; i < b.len; i++) {
                        if ((s >> (unsigned)i) & 1u) b_count++;
                        else a_count++;
                    }
                    lp = braid_loops(&b, s);
                    if (lp - 1 <= MAX_DELTA_POW) {
                        Cyc16 term = cyc16_mul(
                            cyc16_pow_int(A, a_count - b_count),
                            delta_pow[lp - 1]);
                        bracket = cyc16_add(bracket, term);
                    }
                }

                if (!cyc16_eq(bracket, cyc16_zero())
                    && find_in_16(bracket) < 0
                    && cat16_size < MAX_CAT16) {
                    cat16_exact[cat16_size] = bracket;
                    cat16_cx[cat16_size] = cyc16_to_cx(bracket);
                    cat16_size++;
                    new_this++;
                }
            }
            if (new_this > 0) {
                printf("    n=4 len=%d: +%d new (total %d)\n",
                       len, new_this, cat16_size);
                new_from_4strand += new_this;
            } else {
                printf("    n=4 len=%d: 0 new\n", len);
            }
        }
    }

    printf("  INFO: After 4-strand extension: %d total values (+%d new)\n",
           cat16_size, new_from_4strand);
    sprintf(msg, "4-strand braids added %d new values", new_from_4strand);
    check(msg, 1);

    if (new_from_4strand > 0) {
        int k;

        /* Re-check XOR5 with extended catalog */
        printf("  INFO: Re-checking XOR5 with %d-value catalog:\n", cat16_size);
        {
            int any = 0;
            for (k = 2; k <= 31; k++) {
                int c = count_xor16(5, k);
                if (c > 0) {
                    printf("    k=%2d: %d solutions\n", k, c);
                    any = 1;
                }
            }
            sprintf(msg, "XOR5 with extended catalog: %s",
                    any ? "achievable" : "not achievable");
            check(msg, 1);
        }

        /* Full XOR6 sweep with extended catalog */
        printf("  INFO: XOR6 sweep k=2..31 with %d-value catalog:\n",
               cat16_size);
        {
            int any = 0;
            for (k = 2; k <= 31; k++) {
                int c;
                printf("    k=%2d: ", k);
                fflush(stdout);
                c = count_xor16(6, k);
                printf("%d solutions\n", c);
                if (c > 0) any = 1;
            }
            if (any) {
                sprintf(msg, "*** XOR6 ACHIEVABLE with extended catalog! ***");
            } else {
                sprintf(msg, "XOR6 NOT achievable at any k (extended catalog)");
            }
            check(msg, 1);
        }
    }
}

/* ================================================================
 * Part G: Generalized Activation Check
 * ================================================================
 * Can ANY binary sector labeling compute XOR6?
 * Method: extend known XOR5 weight sets by one weight. For each
 * 6-tuple, check if there EXISTS a 2-coloring of k sectors such
 * that all 64 inputs are correctly classified. This is STRICTLY
 * WEAKER than requiring odd=1/even=0, so if this fails, the wall
 * is deeper than labeling choice.
 */

/* Part G catalog: Z[zeta_8] values embedded in Cyc16 */
static Cyc16 pg_cat[MAX_CAT8];
static Cx    pg_cx[MAX_CAT8];
static int   pg_size = 0;

/* Part G partial sums */
static Cyc16 pg_ps[64];

/* Part G stored XOR5 solutions */
#define MAX_XOR5_STORE 4000
static int pg_xor5[MAX_XOR5_STORE][5];
static int pg_n_xor5 = 0;

/* Part G deduplicated unordered sets */
static int pg_uniq[200][5];
static int pg_n_uniq = 0;

static void pg_search_r(int *widx, int depth, int k,
                         const int *c1, int nc1) {
    int ci, ms, me, mask;
    ms = 1 << depth;
    me = 1 << (depth + 1);
    for (ci = 0; ci < nc1; ci++) {
        int ok = 1;
        widx[depth] = c1[ci];
        for (mask = ms; mask < me; mask++) {
            Cyc16 ns = cyc16_add(pg_ps[mask ^ ms], pg_cat[c1[ci]]);
            Cx scx = cyc16_to_cx(ns);
            int cls = sector_classify(scx, k);
            if (cls != (popcount_small(mask) % 2)) { ok = 0; break; }
            pg_ps[mask] = ns;
        }
        if (ok) {
            if (depth == 4) {
                if (pg_n_xor5 < MAX_XOR5_STORE)
                    memcpy(pg_xor5[pg_n_xor5++], widx, 5 * sizeof(int));
            } else {
                pg_search_r(widx, depth + 1, k, c1, nc1);
            }
        }
        if (pg_n_xor5 >= MAX_XOR5_STORE) return;
    }
}

/*
 * Check if 6 weights can compute XOR6 with ANY binary sector labeling.
 * For each of 64 inputs, compute additive sum, determine sector.
 * If any sector contains inputs with both parity 0 and parity 1,
 * no labeling can work. Returns 1 if a consistent labeling exists.
 */
static int check_gen_xor6(const Cx *w, int k) {
    int seen[1025]; /* k sectors + 1 for zero-sum pseudo-sector */
    int mask, i, sec;
    double angle;

    if (k > 1024) return 0;
    for (i = 0; i <= k; i++) seen[i] = 0;

    for (mask = 0; mask < 64; mask++) {
        Cx sum = cx_make(0.0, 0.0);
        int par = 0;
        for (i = 0; i < 6; i++) {
            if (mask & (1 << i)) {
                sum.re += w[i].re;
                sum.im += w[i].im;
                par ^= 1;
            }
        }
        if (cx_abs(sum) < 1e-12) {
            sec = k; /* zero-sum: special pseudo-sector */
        } else {
            angle = atan2(sum.im, sum.re);
            if (angle < 0.0) angle += 2.0 * M_PI;
            sec = (int)(angle / (2.0 * M_PI / (double)k));
            if (sec >= k) sec = k - 1;
        }
        seen[sec] |= (1 << par);
        if (seen[sec] == 3) return 0; /* both parities in same sector */
    }
    return 1;
}

static void part_g_generalized(void) {
    int c1[MAX_CAT8];
    int nc1 = 0;
    int widx[6];
    int i, j, k, ki;
    char msg[256];
    int k_test[] = {6, 8, 15, 31, 63, 127, 255, 500};
    int nk = 8;
    int min_k_found = 0;

    printf("\n=== Part G: Generalized Activation Check ===\n");
    printf("  INFO: Can ANY sector labeling compute XOR6?\n");
    printf("  INFO: Method: extend XOR5 sets by 1 weight, check consistency\n");

    /* Load Z[zeta_8] catalog embedded in Cyc16 */
    pg_size = cat8_size;
    for (i = 0; i < cat8_size; i++) {
        pg_cat[i] = cyc8_to_cyc16(cat8_exact[i]);
        pg_cx[i] = cyc16_to_cx(pg_cat[i]);
    }

    /* Class-1 at k=15 */
    for (i = 0; i < pg_size; i++)
        if (sector_classify(pg_cx[i], 15) == 1) c1[nc1++] = i;
    printf("  INFO: Z[zeta_8]: %d values, %d class-1 at k=15\n", pg_size, nc1);

    /* Find and store XOR5 at k=15 */
    pg_ps[0] = cyc16_zero();
    pg_n_xor5 = 0;
    pg_search_r(widx, 0, 15, c1, nc1);
    printf("  INFO: XOR5 ordered solutions: %d (stored %d)\n",
           pg_n_xor5, pg_n_xor5 < MAX_XOR5_STORE ? pg_n_xor5 : MAX_XOR5_STORE);

    /* Deduplicate to unordered sets */
    pg_n_uniq = 0;
    for (i = 0; i < pg_n_xor5 && i < MAX_XOR5_STORE; i++) {
        int sorted[5], dup = 0;
        memcpy(sorted, pg_xor5[i], 5 * sizeof(int));
        /* Insertion sort */
        for (j = 1; j < 5; j++) {
            int key = sorted[j], jj = j - 1;
            while (jj >= 0 && sorted[jj] > key) {
                sorted[jj + 1] = sorted[jj];
                jj--;
            }
            sorted[jj + 1] = key;
        }
        for (j = 0; j < pg_n_uniq; j++)
            if (memcmp(sorted, pg_uniq[j], 5 * sizeof(int)) == 0)
                { dup = 1; break; }
        if (!dup && pg_n_uniq < 200)
            memcpy(pg_uniq[pg_n_uniq++], sorted, 5 * sizeof(int));
    }
    printf("  INFO: Distinct unordered XOR5 sets: %d\n", pg_n_uniq);
    sprintf(msg, "Found %d unordered XOR5 sets at Z[zeta_8] k=15", pg_n_uniq);
    check(msg, pg_n_uniq > 0);

    /* Test generalized XOR6 at specific k values */
    printf("  INFO: Testing %d sets x %d extensions = %d checks per k\n",
           pg_n_uniq, pg_size, pg_n_uniq * pg_size);

    for (ki = 0; ki < nk; ki++) {
        int kv = k_test[ki];
        int n_pass = 0;
        for (i = 0; i < pg_n_uniq; i++) {
            for (j = 0; j < pg_size; j++) {
                Cx w6[6];
                int wi;
                for (wi = 0; wi < 5; wi++)
                    w6[wi] = pg_cx[pg_uniq[i][wi]];
                w6[5] = pg_cx[j];
                if (check_gen_xor6(w6, kv)) n_pass++;
            }
        }
        printf("  INFO: k=%3d: %d/%d pass generalized XOR6\n",
               kv, n_pass, pg_n_uniq * pg_size);
    }

    /* Find minimum k where generalized XOR6 first becomes possible */
    printf("  INFO: Scanning for minimum k (generalized XOR6)...\n");
    fflush(stdout);
    for (k = 2; k <= 1000; k++) {
        int found = 0;
        for (i = 0; i < pg_n_uniq && !found; i++) {
            for (j = 0; j < pg_size && !found; j++) {
                Cx w6[6];
                int wi;
                for (wi = 0; wi < 5; wi++)
                    w6[wi] = pg_cx[pg_uniq[i][wi]];
                w6[5] = pg_cx[j];
                if (check_gen_xor6(w6, k)) found = 1;
            }
        }
        if (found) { min_k_found = k; break; }
    }

    if (min_k_found > 0) {
        /* Count how many pass at the minimum k */
        int n_pass_min = 0;
        for (i = 0; i < pg_n_uniq; i++) {
            for (j = 0; j < pg_size; j++) {
                Cx w6[6];
                int wi;
                for (wi = 0; wi < 5; wi++)
                    w6[wi] = pg_cx[pg_uniq[i][wi]];
                w6[5] = pg_cx[j];
                if (check_gen_xor6(w6, min_k_found)) n_pass_min++;
            }
        }
        printf("  INFO: *** Minimum k for generalized XOR6: %d (%d/%d pass) ***\n",
               min_k_found, n_pass_min, pg_n_uniq * pg_size);
        sprintf(msg, "Generalized XOR6 first achievable at k=%d", min_k_found);
    } else {
        printf("  INFO: Generalized XOR6 NOT achievable at any k <= 1000\n");
        sprintf(msg, "Generalized XOR6 NOT achievable (k<=1000, %d sets x %d ext)",
                pg_n_uniq, pg_size);
    }
    check(msg, 1);

    (void)widx;
}

/* ================================================================
 * Part H: Anatomy of the 3 Passing XOR6 Tuples at k=24
 * ================================================================
 * Identify which tuples pass, characterize their sector labeling,
 * measure margins, and check structure.
 */

/* Detailed version: returns the labeling and diagnostics */
static int check_gen_xor6_detail(const Cx *w, int k,
                                  int *label_out, int *used_out,
                                  int *n_label1, int *n_used,
                                  double *min_margin) {
    int seen[1025];
    int desired[1025]; /* which parity is in this sector (-1=none, 0, 1) */
    int mask, i, sec;
    double angle;
    int n1 = 0, nu = 0;

    if (k > 1024) return 0;
    for (i = 0; i <= k; i++) { seen[i] = 0; desired[i] = -1; }
    if (label_out) for (i = 0; i <= k; i++) label_out[i] = -1;
    if (used_out) for (i = 0; i <= k; i++) used_out[i] = 0;
    *min_margin = 999.0;

    for (mask = 0; mask < 64; mask++) {
        Cx sum = cx_make(0.0, 0.0);
        int par = 0;
        double sector_center, dist;
        for (i = 0; i < 6; i++) {
            if (mask & (1 << i)) {
                sum.re += w[i].re;
                sum.im += w[i].im;
                par ^= 1;
            }
        }
        if (cx_abs(sum) < 1e-12) {
            sec = k;
        } else {
            double sw = 2.0 * M_PI / (double)k;
            angle = atan2(sum.im, sum.re);
            if (angle < 0.0) angle += 2.0 * M_PI;
            sec = (int)(angle / sw);
            if (sec >= k) sec = k - 1;
            /* Margin: distance to nearest sector boundary */
            sector_center = ((double)sec + 0.5) * sw;
            dist = fabs(angle - sector_center);
            if (dist > sw / 2.0) dist = sw - dist;
            dist = (sw / 2.0) - dist; /* distance to boundary */
            if (dist < *min_margin) *min_margin = dist;
        }
        seen[sec] |= (1 << par);
        if (seen[sec] == 3) return 0;
        desired[sec] = par;
        if (used_out) used_out[sec] = 1;
    }

    /* Build labeling */
    for (i = 0; i <= k; i++) {
        if (desired[i] >= 0) {
            if (label_out) label_out[i] = desired[i];
            if (desired[i] == 1) n1++;
            nu++;
        }
    }
    *n_label1 = n1;
    *n_used = nu;
    return 1;
}

/* Stored passing tuples for Part I (XOR7) */
#define MAX_PASS6 10
static int ph_pass_set[MAX_PASS6];  /* index into pg_uniq */
static int ph_pass_ext[MAX_PASS6];  /* index into pg catalog */
static int ph_n_pass = 0;

static void part_h_anatomy(void) {
    int i, j, wi;
    char msg[256];

    printf("\n=== Part H: Anatomy of Passing XOR6 Tuples at k=24 ===\n");

    ph_n_pass = 0;
    for (i = 0; i < pg_n_uniq; i++) {
        for (j = 0; j < pg_size; j++) {
            Cx w6[6];
            int label[1025], used[1025], n1, nu;
            double margin;

            for (wi = 0; wi < 5; wi++)
                w6[wi] = pg_cx[pg_uniq[i][wi]];
            w6[5] = pg_cx[j];

            if (check_gen_xor6_detail(w6, 24, label, used, &n1, &nu, &margin)) {
                int s;
                printf("  INFO: === Passing tuple %d ===\n", ph_n_pass + 1);
                printf("  INFO: XOR5 parent set #%d: cat indices [", i);
                for (wi = 0; wi < 5; wi++)
                    printf("%d%s", pg_uniq[i][wi], wi < 4 ? "," : "");
                printf("]\n");
                printf("  INFO: Extension value: cat index %d\n", j);

                /* Print the 6 weights */
                printf("  INFO: Weights (complex):\n");
                for (wi = 0; wi < 6; wi++) {
                    printf("    w[%d] = %+.6f %+.6fi\n",
                           wi, w6[wi].re, w6[wi].im);
                }

                /* Sector labeling */
                printf("  INFO: Sectors labeled 1: %d/%d used, %d/%d total\n",
                       n1, nu, n1, 24);
                printf("  INFO: Sectors used: %d/24\n", nu);
                printf("  INFO: Sector labeling (1=class1, 0=class0, .=unused):\n    ");
                for (s = 0; s < 24; s++) {
                    if (!used[s]) printf(".");
                    else printf("%d", label[s]);
                }
                printf("\n");

                /* Margin */
                printf("  INFO: Min margin to sector boundary: %.6f rad (%.2f deg)\n",
                       margin, margin * 180.0 / M_PI);

                /* How many inputs per sector? */
                {
                    int sec_count[25];
                    int mask, s2;
                    for (s2 = 0; s2 <= 24; s2++) sec_count[s2] = 0;
                    for (mask = 0; mask < 64; mask++) {
                        Cx sum = cx_make(0.0, 0.0);
                        int ii;
                        double sw, ang;
                        for (ii = 0; ii < 6; ii++) {
                            if (mask & (1 << ii)) {
                                sum.re += w6[ii].re;
                                sum.im += w6[ii].im;
                            }
                        }
                        if (cx_abs(sum) < 1e-12) {
                            sec_count[24]++;
                        } else {
                            sw = 2.0 * M_PI / 24.0;
                            ang = atan2(sum.im, sum.re);
                            if (ang < 0.0) ang += 2.0 * M_PI;
                            s2 = (int)(ang / sw);
                            if (s2 >= 24) s2 = 23;
                            sec_count[s2]++;
                        }
                    }
                    printf("  INFO: Inputs per sector: ");
                    for (s2 = 0; s2 < 24; s2++) {
                        if (sec_count[s2] > 0) printf("[%d]=%d ", s2, sec_count[s2]);
                    }
                    if (sec_count[24] > 0) printf("[zero]=%d", sec_count[24]);
                    printf("\n");
                }

                /* Check if labeling matches standard odd/even */
                {
                    int std_match = 1;
                    int s3;
                    for (s3 = 0; s3 < 24; s3++) {
                        if (used[s3] && label[s3] != s3 % 2) {
                            std_match = 0;
                            break;
                        }
                    }
                    printf("  INFO: Matches standard odd/even: %s\n",
                           std_match ? "YES" : "NO");
                }

                if (ph_n_pass < MAX_PASS6) {
                    ph_pass_set[ph_n_pass] = i;
                    ph_pass_ext[ph_n_pass] = j;
                    ph_n_pass++;
                }
            }
        }
    }

    sprintf(msg, "Identified %d passing XOR6 tuples at k=24", ph_n_pass);
    check(msg, ph_n_pass == 3);

    /* Check: are these 3 from the same XOR5 parent or different? */
    {
        int n_distinct_parents = 0;
        int parent_seen[200];
        memset(parent_seen, 0, sizeof(parent_seen));
        for (i = 0; i < ph_n_pass; i++) {
            if (!parent_seen[ph_pass_set[i]]) {
                parent_seen[ph_pass_set[i]] = 1;
                n_distinct_parents++;
            }
        }
        printf("  INFO: Distinct XOR5 parent sets among passers: %d\n",
               n_distinct_parents);
    }

    /* ROBUSTNESS CHECK: Zero margin suggests boundary artifact.
     * All weights are on Z[zeta_8] lattice (45 degree multiples).
     * At k=24 (15 deg sectors), 45 deg angles land exactly on boundaries.
     * Test: do the same tuples pass at k=25 (14.4 deg sectors)?
     * At k=25, lattice angles don't align with boundaries. */
    printf("  INFO:\n");
    printf("  INFO: === Boundary Robustness Check ===\n");
    printf("  INFO: Zero margin detected — checking for boundary artifact\n");
    {
        int kk, ki2;
        int k_check[] = {23, 25, 26, 27, 28, 29, 30, 32, 48};
        int nkc = 9;
        int any_robust = 0;
        printf("  INFO: Do the 3 tuples pass at nearby k values?\n");
        for (ki2 = 0; ki2 < nkc; ki2++) {
            int n_pass_k = 0;
            kk = k_check[ki2];
            for (i = 0; i < ph_n_pass; i++) {
                Cx w6[6];
                for (wi = 0; wi < 5; wi++)
                    w6[wi] = pg_cx[pg_uniq[ph_pass_set[i]][wi]];
                w6[5] = pg_cx[ph_pass_ext[i]];
                if (check_gen_xor6(w6, kk)) n_pass_k++;
            }
            printf("    k=%2d: %d/%d tuples pass\n", kk, n_pass_k, ph_n_pass);
            if (n_pass_k > 0) any_robust = 1;
        }

        /* Also: does ANY extension (not just the 3) pass at k=25..31? */
        printf("  INFO: Full 2700-extension sweep at k=25..32:\n");
        for (kk = 25; kk <= 32; kk++) {
            int n_pass_full = 0;
            for (i = 0; i < pg_n_uniq; i++) {
                for (j = 0; j < pg_size; j++) {
                    Cx w6[6];
                    for (wi = 0; wi < 5; wi++)
                        w6[wi] = pg_cx[pg_uniq[i][wi]];
                    w6[5] = pg_cx[j];
                    if (check_gen_xor6(w6, kk)) n_pass_full++;
                }
            }
            printf("    k=%2d: %d/2700 pass\n", kk, n_pass_full);
            if (n_pass_full > 0) any_robust = 1;
        }

        if (any_robust) {
            sprintf(msg, "Boundary robustness: XOR6 survives at non-boundary k");
            check(msg, 1);
        } else {
            sprintf(msg, "*** WARNING: XOR6 at k=24 may be a boundary artifact ***");
            check(msg, 1);
        }
    }
}

/* ================================================================
 * Part I: Generalized XOR7 Search
 * ================================================================
 * Extend the passing XOR6 tuples by one more weight.
 * Does the ceiling keep climbing with custom labeling?
 */

static int check_gen_xor7(const Cx *w, int k) {
    int seen[1025];
    int mask, i, sec;
    double angle;

    if (k > 1024) return 0;
    for (i = 0; i <= k; i++) seen[i] = 0;

    for (mask = 0; mask < 128; mask++) {
        Cx sum = cx_make(0.0, 0.0);
        int par = 0;
        for (i = 0; i < 7; i++) {
            if (mask & (1 << i)) {
                sum.re += w[i].re;
                sum.im += w[i].im;
                par ^= 1;
            }
        }
        if (cx_abs(sum) < 1e-12) {
            sec = k;
        } else {
            angle = atan2(sum.im, sum.re);
            if (angle < 0.0) angle += 2.0 * M_PI;
            sec = (int)(angle / (2.0 * M_PI / (double)k));
            if (sec >= k) sec = k - 1;
        }
        seen[sec] |= (1 << par);
        if (seen[sec] == 3) return 0;
    }
    return 1;
}

static void part_i_xor7(void) {
    int i, j, wi, ki;
    char msg[256];
    int k_test[] = {24, 48, 63, 127, 255, 500};
    int nk = 6;
    int any_xor7 = 0;
    int min_k7 = 0;

    printf("\n=== Part I: Generalized XOR7 Search ===\n");
    printf("  INFO: Extending %d passing XOR6 tuples by 1 weight\n", ph_n_pass);
    printf("  INFO: %d extensions per tuple, checking 128 inputs each\n", pg_size);

    if (ph_n_pass == 0) {
        check("XOR7 search: no XOR6 tuples to extend", 1);
        return;
    }

    /* Test at specific k values */
    for (ki = 0; ki < nk; ki++) {
        int kv = k_test[ki];
        int n_pass = 0;
        int n_tested = 0;

        for (i = 0; i < ph_n_pass; i++) {
            for (j = 0; j < pg_size; j++) {
                Cx w7[7];
                for (wi = 0; wi < 5; wi++)
                    w7[wi] = pg_cx[pg_uniq[ph_pass_set[i]][wi]];
                w7[5] = pg_cx[ph_pass_ext[i]];
                w7[6] = pg_cx[j];
                if (check_gen_xor7(w7, kv)) {
                    n_pass++;
                    if (!any_xor7) {
                        any_xor7 = 1;
                        printf("  INFO: *** FIRST XOR7 found at k=%d! ***\n", kv);
                    }
                }
                n_tested++;
            }
        }
        printf("  INFO: k=%3d: %d/%d pass generalized XOR7\n",
               kv, n_pass, n_tested);
    }

    /* Find minimum k for XOR7 */
    if (!any_xor7) {
        printf("  INFO: Scanning for minimum k (generalized XOR7)...\n");
        fflush(stdout);
        for (min_k7 = 2; min_k7 <= 1000; min_k7++) {
            int found = 0;
            for (i = 0; i < ph_n_pass && !found; i++) {
                for (j = 0; j < pg_size && !found; j++) {
                    Cx w7[7];
                    for (wi = 0; wi < 5; wi++)
                        w7[wi] = pg_cx[pg_uniq[ph_pass_set[i]][wi]];
                    w7[5] = pg_cx[ph_pass_ext[i]];
                    w7[6] = pg_cx[j];
                    if (check_gen_xor7(w7, min_k7)) found = 1;
                }
            }
            if (found) break;
        }
        if (min_k7 <= 1000) {
            printf("  INFO: *** Minimum k for generalized XOR7: %d ***\n", min_k7);
            any_xor7 = 1;
        }
    }

    if (any_xor7) {
        sprintf(msg, "Generalized XOR7 achievable");
    } else {
        sprintf(msg, "Generalized XOR7 NOT achievable (k<=1000, %d tuples x %d ext)",
                ph_n_pass, pg_size);
    }
    check(msg, 1);
}

/* ================================================================
 * Part J: Follow-up Analysis
 * ================================================================
 * J1: Margin check at robust k values (k=26,28,30,32,48)
 * J2: Even k sweep for generalized XOR6
 * J3: Broader XOR7 search (27 sets x 5050 pairs)
 * J4: Anatomy of XOR7 winner (antipodal structure)
 */

static void part_j_followup(void) {
    int i, wi;
    char msg[256];
    Cx j4_w[7];
    int j4_k = 0;
    int j4_ok = 0;

    printf("\n=== Part J: Follow-up Analysis ===\n");

    /* J1: Margins at k=26,28,30,32,48 for the 3 original tuples */
    printf("  INFO: J1 — Margin at robust (non-boundary) k values\n");
    {
        int k_arr[] = {26, 28, 30, 32, 48};
        int nk = 5, ki;
        double best = 0.0;
        for (ki = 0; ki < nk; ki++) {
            int kv = k_arr[ki];
            printf("    k=%2d: ", kv);
            for (i = 0; i < ph_n_pass; i++) {
                Cx w6[6];
                int lbl[1025], usd[1025], n1, nu;
                double mrg;
                for (wi = 0; wi < 5; wi++)
                    w6[wi] = pg_cx[pg_uniq[ph_pass_set[i]][wi]];
                w6[5] = pg_cx[ph_pass_ext[i]];
                if (check_gen_xor6_detail(w6, kv, lbl, usd, &n1, &nu, &mrg)) {
                    printf("T%d:%.4f ", i + 1, mrg);
                    if (mrg > best) best = mrg;
                } else {
                    printf("T%d:FAIL ", i + 1);
                }
            }
            printf("\n");
        }
        printf("  INFO: Best margin across robust k: %.6f rad (%.2f deg)\n",
               best, best * 180.0 / M_PI);
        sprintf(msg, "J1: Best non-boundary margin = %.4f rad (%.1f deg)",
                best, best * 180.0 / M_PI);
        check(msg, 1);
    }

    /* J2: Even k sweep k=2,4,...,64 */
    printf("  INFO: J2 — Even k sweep (generalized XOR6)\n");
    {
        int k, j, first_k = 0;
        for (k = 2; k <= 64; k += 2) {
            int np = 0;
            for (i = 0; i < pg_n_uniq; i++) {
                for (j = 0; j < pg_size; j++) {
                    Cx w6[6];
                    for (wi = 0; wi < 5; wi++)
                        w6[wi] = pg_cx[pg_uniq[i][wi]];
                    w6[5] = pg_cx[j];
                    if (check_gen_xor6(w6, k)) np++;
                }
            }
            if (np > 0) {
                printf("    k=%2d: %d/2700\n", k, np);
                if (first_k == 0) first_k = k;
            }
        }
        sprintf(msg, "J2: Min even k for gen XOR6 = %d", first_k);
        check(msg, first_k > 0);
    }

    /* J3: Broader XOR7 at milestones */
    printf("  INFO: J3 — Broader XOR7 (27 sets x C(100,2)=5050 pairs)\n");
    {
        int k_arr[] = {48, 63, 80, 100, 120, 127};
        int nk = 6, ki;
        for (ki = 0; ki < nk; ki++) {
            int kv = k_arr[ki];
            int np = 0, nt = 0;
            int j1, j2;
            printf("    k=%3d: ", kv); fflush(stdout);
            for (i = 0; i < pg_n_uniq; i++) {
                for (j1 = 0; j1 < pg_size; j1++) {
                    for (j2 = j1; j2 < pg_size; j2++) {
                        Cx w7[7];
                        for (wi = 0; wi < 5; wi++)
                            w7[wi] = pg_cx[pg_uniq[i][wi]];
                        w7[5] = pg_cx[j1];
                        w7[6] = pg_cx[j2];
                        if (check_gen_xor7(w7, kv)) {
                            np++;
                            if (!j4_ok) {
                                memcpy(j4_w, w7, 7 * sizeof(Cx));
                                j4_k = kv;
                                j4_ok = 1;
                            }
                        }
                        nt++;
                    }
                }
            }
            printf("%d/%d pass\n", np, nt);
        }
        if (j4_ok) {
            sprintf(msg, "J3: Broader XOR7 first at k=%d", j4_k);
        } else {
            sprintf(msg, "J3: No XOR7 in broader search");
        }
        check(msg, 1);
    }

    /* J3b: Min k scan with broader search */
    if (j4_ok) {
        int orig_k = j4_k;
        if (orig_k > 24) {
            int k;
            printf("  INFO: J3b — Scanning k=24..%d for broader min\n",
                   orig_k - 1);
            fflush(stdout);
            for (k = 24; k < orig_k; k++) {
                int found = 0;
                int j1, j2;
                for (i = 0; i < pg_n_uniq && !found; i++) {
                    for (j1 = 0; j1 < pg_size && !found; j1++) {
                        for (j2 = j1; j2 < pg_size && !found; j2++) {
                            Cx w7[7];
                            for (wi = 0; wi < 5; wi++)
                                w7[wi] = pg_cx[pg_uniq[i][wi]];
                            w7[5] = pg_cx[j1];
                            w7[6] = pg_cx[j2];
                            if (check_gen_xor7(w7, k)) {
                                found = 1;
                                memcpy(j4_w, w7, 7 * sizeof(Cx));
                                j4_k = k;
                            }
                        }
                    }
                }
                if (found) break;
                if (k % 20 == 0) {
                    printf("    ...k=%d\n", k);
                    fflush(stdout);
                }
            }
        }
        printf("  INFO: Broader XOR7 min k = %d\n", j4_k);
        sprintf(msg, "J3b: Broader XOR7 min k=%d", j4_k);
        check(msg, 1);

        /* J4: Anatomy of XOR7 winner */
        {
            int ai, aj, nap = 0, lc = 0;
            int paired[7];
            printf("  INFO: J4 — XOR7 winner anatomy (k=%d)\n", j4_k);
            printf("  INFO: Weights:\n");
            for (wi = 0; wi < 7; wi++)
                printf("    w[%d] = %+.6f %+.6fi\n",
                       wi, j4_w[wi].re, j4_w[wi].im);
            memset(paired, 0, sizeof(paired));
            for (ai = 0; ai < 7; ai++) {
                for (aj = ai + 1; aj < 7; aj++) {
                    double d = fabs(j4_w[ai].re + j4_w[aj].re) +
                               fabs(j4_w[ai].im + j4_w[aj].im);
                    if (d < 1e-9) {
                        printf("    w[%d]+w[%d]=0 (antipodal)\n", ai, aj);
                        nap++;
                        paired[ai] = 1;
                        paired[aj] = 1;
                    }
                }
            }
            for (ai = 0; ai < 7; ai++) {
                if (!paired[ai]) {
                    printf("    w[%d] unpaired = %+.6f %+.6fi\n",
                           ai, j4_w[ai].re, j4_w[ai].im);
                    lc++;
                }
            }
            printf("  INFO: Structure: %d antipodal pairs + %d unpaired\n",
                   nap, lc);
            sprintf(msg, "J4: %d antipodal pairs + %d unpaired", nap, lc);
            check(msg, 1);
        }
    } else {
        check("J3b: No XOR7 found", 1);
        check("J4: Skipped (no winner)", 1);
    }
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 65: Clifford Hierarchy Staircase\n");
    printf("================================================\n");
    printf("Z[zeta_16] at delta = -sqrt(2): climbing the Clifford ladder\n");

    part_a_arithmetic();
    part_b_catalog();
    part_c_comparison();
    part_d_tier_probe();
    part_e_xor6();
    part_f_convergence();
    part_g_generalized();
    part_h_anatomy();
    part_i_xor7();
    part_j_followup();

    printf("\n================================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
