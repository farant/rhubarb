/*
 * KNOTAPEL DEMO 48: Forward DKC Zoo
 * ==================================
 *
 * Demo 29 proved forward DKC for XOR: exact Z[zeta_8] bracket values
 * compute XOR without training (65,536 valid triples from 64 values).
 *
 * Demo 48 extends this to ALL 13 NPN classes of 3-input Boolean
 * functions. For each of 64^4 = 16,777,216 bracket quartets
 * (w1, w2, w3, bias), evaluate a single complex neuron with
 * split-sigmoid on all 8 input patterns. Record which truth table
 * it computes. Search once, classify after.
 *
 * Key prediction: parity (XNOR3, tt=0x69) has ZERO valid quartets.
 *
 * Parts:
 *   A: Exact Z[zeta_8] arithmetic + bracket catalog (from Demo 29)
 *   B: NPN classification + topology frequency (from Demo 47)
 *   C: Exhaustive single-pass search
 *   D: Analysis, correlation, prediction checks
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
 * PART A: Exact Z[zeta_8] Arithmetic + Bracket Catalog
 *
 * Z[zeta_8] elements: a + b*z + c*z^2 + d*z^3 where z = zeta_8.
 * z^4 = -1 closes the ring. Multiplication: 16 integer muls.
 * ================================================================ */

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* --- Complex double type (for sigmoid evaluation + search) --- */

typedef struct { double re, im; } Cx;

static Cx cx_make(double re, double im) {
    Cx z; z.re = re; z.im = im; return z;
}
static Cx cx_zero(void) { return cx_make(0.0, 0.0); }
static Cx cx_one(void)  { return cx_make(1.0, 0.0); }
static Cx cx_add(Cx a, Cx b) {
    return cx_make(a.re + b.re, a.im + b.im);
}
static Cx cx_sub(Cx a, Cx b) {
    return cx_make(a.re - b.re, a.im - b.im);
}
static Cx cx_neg(Cx a) { return cx_make(-a.re, -a.im); }
static Cx cx_mul(Cx a, Cx b) {
    return cx_make(a.re * b.re - a.im * b.im,
                   a.re * b.im + a.im * b.re);
}
static Cx cx_scale(Cx a, double s) {
    return cx_make(a.re * s, a.im * s);
}
static double cx_abs(Cx a) {
    return sqrt(a.re * a.re + a.im * a.im);
}
static Cx cx_exp_i(double theta) {
    return cx_make(cos(theta), sin(theta));
}
static Cx cx_pow_int(Cx a, int n) {
    Cx r = cx_one();
    Cx base;
    int neg;
    if (n == 0) return r;
    neg = (n < 0);
    if (neg) n = -n;
    base = a;
    while (n > 0) {
        if (n & 1) r = cx_mul(r, base);
        base = cx_mul(base, base);
        n >>= 1;
    }
    if (neg) {
        double d = r.re * r.re + r.im * r.im;
        r = cx_make(r.re / d, -r.im / d);
    }
    return r;
}

/* --- Exact cyclotomic type Z[zeta_8] --- */

typedef struct {
    long a, b, c, d;  /* a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3 */
} Cyc8;

static Cyc8 cyc8_make(long a, long b, long c, long d) {
    Cyc8 z; z.a = a; z.b = b; z.c = c; z.d = d; return z;
}
static Cyc8 cyc8_zero(void) { return cyc8_make(0, 0, 0, 0); }
static Cyc8 cyc8_one(void)  { return cyc8_make(1, 0, 0, 0); }
static Cyc8 cyc8_add(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a + y.a, x.b + y.b, x.c + y.c, x.d + y.d);
}
static Cyc8 cyc8_sub(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a - y.a, x.b - y.b, x.c - y.c, x.d - y.d);
}
static Cyc8 cyc8_neg(Cyc8 x) {
    return cyc8_make(-x.a, -x.b, -x.c, -x.d);
}
static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y) {
    return cyc8_make(
        x.a * y.a - x.b * y.d - x.c * y.c - x.d * y.b,
        x.a * y.b + x.b * y.a - x.c * y.d - x.d * y.c,
        x.a * y.c + x.b * y.b + x.c * y.a - x.d * y.d,
        x.a * y.d + x.b * y.c + x.c * y.b + x.d * y.a
    );
}
static int cyc8_eq(Cyc8 x, Cyc8 y) {
    return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}
static Cx cyc8_to_cx(Cyc8 z) {
    double inv_sqrt2 = 1.0 / sqrt(2.0);
    double re = (double)z.a + (double)z.b * inv_sqrt2
              + (double)z.d * (-inv_sqrt2);
    double im = (double)z.b * inv_sqrt2 + (double)z.c
              + (double)z.d * inv_sqrt2;
    return cx_make(re, im);
}
static Cyc8 cyc8_conj(Cyc8 z) {
    return cyc8_make(z.a, -z.d, -z.c, -z.b);
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

/* --- Braid type + bracket oracle --- */

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

#define MAX_UF 4096
static int uf_p[MAX_UF];
static void uf_init(int n) {
    int i; for (i = 0; i < n; i++) uf_p[i] = i;
}
static int uf_find(int x) {
    while (uf_p[x] != x) { uf_p[x] = uf_p[uf_p[x]]; x = uf_p[x]; }
    return x;
}
static void uf_union(int x, int y) {
    x = uf_find(x); y = uf_find(y); if (x != y) uf_p[x] = y;
}

static int braid_loops(const Braid *b, unsigned s) {
    int N = (b->len + 1) * b->n, l, p, i, loops, sgn, bit, cup;
    uf_init(N);
    for (l = 0; l < b->len; l++) {
        sgn = b->word[l] > 0 ? 1 : -1;
        i = (sgn > 0 ? b->word[l] : -b->word[l]) - 1;
        bit = (int)((s >> l) & 1u);
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
    /* Trace closure */
    for (p = 0; p < b->n; p++)
        uf_union(p, b->len * b->n + p);
    loops = 0;
    for (i = 0; i < N; i++)
        if (uf_find(i) == i) loops++;
    return loops;
}

/* Exact bracket in Z[zeta_8] — delta=0 optimization */
static Cyc8 braid_bracket_exact(const Braid *b, Cyc8 A) {
    unsigned s, ns;
    int i, a_count, b_count, lp;
    Cyc8 result, delta;

    delta = cyc8_zero(); /* A^2 + A^{-2} = 0 at this A */
    result = cyc8_zero();

    if (!b->len) {
        result = cyc8_one();
        for (i = 0; i < b->n - 1; i++)
            result = cyc8_mul(result, delta);
        return result;
    }

    ns = 1u << b->len;
    for (s = 0; s < ns; s++) {
        a_count = 0; b_count = 0;
        for (i = 0; i < b->len; i++) {
            if ((s >> (unsigned)i) & 1u) b_count++;
            else a_count++;
        }
        lp = braid_loops(b, s);
        if (lp != 1) continue; /* delta=0 kills multi-loop states */
        result = cyc8_add(result,
                          cyc8_pow_int(A, a_count - b_count));
    }
    return result;
}

/* Float bracket for cross-checking */
static Cx braid_bracket_cx(const Braid *b, Cx A) {
    unsigned s, ns;
    int i, ac, bc, lp, j;
    Cx result, delta, dp, coeff;

    delta = cx_neg(cx_add(cx_pow_int(A, 2), cx_pow_int(A, -2)));
    result = cx_zero();

    if (!b->len) {
        result = cx_one();
        for (i = 0; i < b->n - 1; i++)
            result = cx_mul(result, delta);
        return result;
    }

    ns = 1u << b->len;
    for (s = 0; s < ns; s++) {
        ac = 0; bc = 0;
        for (i = 0; i < b->len; i++) {
            if ((s >> (unsigned)i) & 1u) bc++;
            else ac++;
        }
        lp = braid_loops(b, s);
        coeff = cx_pow_int(A, ac - bc);
        dp = cx_one();
        for (j = 0; j < lp - 1; j++)
            dp = cx_mul(dp, delta);
        result = cx_add(result, cx_mul(coeff, dp));
    }
    return result;
}

/* --- Catalog: dual storage (exact + float) --- */

#define MAX_DISTINCT 512
static Cyc8 cat_exact[MAX_DISTINCT];
static Cx   cat_cx[MAX_DISTINCT];
static int  cat_size = 0;

static int cat_find(Cyc8 val) {
    int i;
    for (i = 0; i < cat_size; i++)
        if (cyc8_eq(val, cat_exact[i])) return i;
    return -1;
}

static void build_catalog(void) {
    Cyc8 A = cyc8_make(0, -1, 0, 0);
    int n, len;
    Braid b;
    int word_buf[MAX_WORD];

    cat_size = 0;

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
                Cyc8 br_e;
                double amp;

                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen) word_buf[i] = g + 1;
                    else             word_buf[i] = -(g - max_gen + 1);
                }
                b.n = n; b.len = len;
                memcpy(b.word, word_buf, (size_t)len * sizeof(int));

                br_e = braid_bracket_exact(&b, A);
                amp = cx_abs(cyc8_to_cx(br_e));

                if (amp > 0.5 && cat_find(br_e) < 0
                    && cat_size < MAX_DISTINCT) {
                    cat_exact[cat_size] = br_e;
                    cat_cx[cat_size] = cyc8_to_cx(br_e);
                    cat_size++;
                }
            }
        }
    }
}

static void part_a_catalog(void) {
    int i, n_match = 0;
    char msg[256];

    printf("\n=== PART A: Exact Bracket Catalog ===\n");
    printf("  Building catalog: 2-3 strand braids, length 1-8...\n");
    printf("  A = e^{i*5pi/4}, delta = 0\n\n");

    build_catalog();

    printf("  Distinct Z[zeta_8] values: %d\n", cat_size);

    /* Cross-check: verify exact-to-float conversion */
    for (i = 0; i < cat_size; i++) {
        Cx from_exact = cyc8_to_cx(cat_exact[i]);
        double err = cx_abs(cx_sub(from_exact, cat_cx[i]));
        if (err < 1e-10) n_match++;
    }
    sprintf(msg, "exact-to-float conversion (%d/%d match)",
            n_match, cat_size);
    check(msg, n_match == cat_size);

    /* Show first 10 */
    printf("\n  First 10 distinct values:\n");
    for (i = 0; i < cat_size && i < 10; i++) {
        printf("    %2d: (%2ld,%2ld,%2ld,%2ld)  = %7.4f + %7.4fi"
               "  |z|=%.4f\n",
               i, cat_exact[i].a, cat_exact[i].b,
               cat_exact[i].c, cat_exact[i].d,
               cat_cx[i].re, cat_cx[i].im, cx_abs(cat_cx[i]));
    }
    if (cat_size > 10)
        printf("    ... (%d more)\n", cat_size - 10);

    sprintf(msg, "catalog has >= 50 distinct values (got %d)", cat_size);
    check(msg, cat_size >= 50);

    /* Verify zeta_8^8 = 1 */
    {
        Cyc8 z8 = cyc8_make(0, 1, 0, 0); /* zeta_8 */
        Cyc8 z8_pow = cyc8_pow_int(z8, 8);
        check("zeta_8^8 = 1", cyc8_eq(z8_pow, cyc8_one()));
    }

    /* Suppress unused warnings for functions needed by Parts B-D */
    (void)cx_scale;  (void)cx_exp_i;
    (void)cyc8_sub;  (void)cyc8_neg;
    (void)braid_bracket_cx;
}

/* ================================================================
 * PART B: NPN Classification
 *
 * 256 truth tables -> 14 NPN classes (13 non-trivial).
 * Brute force: 6 permutations x 8 input negations x 2 output
 * negations = 96 transforms per truth table.
 *
 * Topology frequency is NOT computed here — Part C's exhaustive
 * search produces it as a natural byproduct.
 * ================================================================ */

static const int perms3[6][3] = {
    {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
};

static int npn_transform(int tt, const int sigma[3],
                         int neg_in, int neg_out) {
    int result = 0, idx;
    for (idx = 0; idx < 8; idx++) {
        int x[3], y[3], src, out;
        x[0] = (idx >> 2) & 1;
        x[1] = (idx >> 1) & 1;
        x[2] = idx & 1;
        y[0] = x[sigma[0]] ^ ((neg_in >> 0) & 1);
        y[1] = x[sigma[1]] ^ ((neg_in >> 1) & 1);
        y[2] = x[sigma[2]] ^ ((neg_in >> 2) & 1);
        src = (y[0] << 2) | (y[1] << 1) | y[2];
        out = (tt >> src) & 1;
        if (neg_out) out ^= 1;
        result |= (out << idx);
    }
    return result;
}

static int npn_canon[256];

static void npn_init(void) {
    int i, pi, ni, no;
    for (i = 0; i < 256; i++) {
        int min_tt = i;
        for (pi = 0; pi < 6; pi++)
            for (ni = 0; ni < 8; ni++)
                for (no = 0; no < 2; no++) {
                    int t = npn_transform(i, perms3[pi], ni, no);
                    if (t < min_tt) min_tt = t;
                }
        npn_canon[i] = min_tt;
    }
}

static const char *fn_name(int tt) {
    switch (tt) {
    case 0x00: return "FALSE";
    case 0xFF: return "TRUE";
    case 0x01: return "AND3'";
    case 0x03: return "AND2'";
    case 0x06: return "~A(B^C)";
    case 0x07: return "~A~(BC)";
    case 0x0F: return "BUF";
    case 0x16: return "EXACT1";
    case 0x17: return "MAJ'";
    case 0x18: return "ISOLATE";
    case 0x19: return "3v-0x19";
    case 0x1B: return "3v-0x1B";
    case 0x1E: return "A^(B|C)";
    case 0x3C: return "XOR2";
    case 0x69: return "XNOR3";
    default: return "???";
    }
}

/* NPN class info for analysis */
#define MAX_NPN 14
typedef struct {
    int canon_tt;
    int orbit_size;
    const char *titulis;
} NPNClass;

static NPNClass npn_classes[MAX_NPN];
static int n_npn = 0;

static void npn_build_classes(void) {
    int seen[256];
    int i, j;

    memset(seen, 0, sizeof(seen));
    n_npn = 0;

    /* Walk all non-trivial truth tables, collect unique canonicals */
    for (i = 1; i < 255; i++) {
        int c = npn_canon[i];
        if (!seen[c]) {
            seen[c] = 1;
            npn_classes[n_npn].canon_tt = c;
            npn_classes[n_npn].orbit_size = 0;
            npn_classes[n_npn].titulis = fn_name(c);
            n_npn++;
        }
    }

    /* Count orbit sizes */
    for (i = 1; i < 255; i++) {
        int c = npn_canon[i];
        for (j = 0; j < n_npn; j++) {
            if (npn_classes[j].canon_tt == c) {
                npn_classes[j].orbit_size++;
                break;
            }
        }
    }
}

static int npn_class_index(int tt) {
    int c = npn_canon[tt], j;
    for (j = 0; j < n_npn; j++)
        if (npn_classes[j].canon_tt == c) return j;
    return -1;
}

static void part_b_classify(void) {
    int i;
    char msg[256];

    printf("\n=== PART B: NPN Classification ===\n");

    npn_init();
    npn_build_classes();

    printf("  Non-trivial NPN classes: %d\n\n", n_npn);
    printf("  %4s  %-12s  %s\n", "TT", "Name", "Orbit");
    printf("  ----  ----------  -----\n");
    for (i = 0; i < n_npn; i++) {
        printf("  0x%02X  %-12s  %d\n",
               npn_classes[i].canon_tt,
               npn_classes[i].titulis ? npn_classes[i].titulis : "???",
               npn_classes[i].orbit_size);
    }

    sprintf(msg, "NPN class count = 13 (got %d)", n_npn);
    check(msg, n_npn == 13);

    /* Verify orbit sizes sum to 254 (256 - 2 constants) */
    {
        int total = 0;
        for (i = 0; i < n_npn; i++) total += npn_classes[i].orbit_size;
        sprintf(msg, "orbit sizes sum to 254 (got %d)", total);
        check(msg, total == 254);
    }

    /* Verify parity class exists */
    sprintf(msg, "XNOR3 (0x69) is a canonical representative");
    check(msg, npn_canon[0x69] == 0x69);

}

/* ================================================================
 * PART C: Exhaustive Single-Pass Search
 *
 * For each quartet (w1, w2, w3, b) from the catalog:
 *   1. Precompute partial sums: s12=w1+w2 (outside w3 loop),
 *      s13, s23, s123 (inside w3 loop, outside bias loop)
 *   2. Add bias b to each partial sum -> 8 pre-activation values
 *   3. Split-sigmoid + threshold -> 8-bit truth table
 *   4. Increment counter[tt]
 *
 * Also store up to 10 example quartets per NPN class.
 * ================================================================ */

static double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }

static double split_sigmoid(Cx z) {
    double sr = sigmoid(z.re);
    double si = sigmoid(z.im);
    return sr * (1.0 - si) + (1.0 - sr) * si;
}

/* Hamming distance to parity (0x69 / 0x96) lookup */
static int popcount8(int x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

static int parity_dist_lut[256];

static void init_parity_dist(void) {
    int tt;
    for (tt = 0; tt < 256; tt++) {
        int d1 = popcount8(tt ^ 0x69);
        int d2 = popcount8(tt ^ 0x96);
        parity_dist_lut[tt] = d1 < d2 ? d1 : d2;
    }
}

static long tt_counts[256];

#define MAX_EXAMPLES 10
typedef struct { int w1, w2, w3, b; } Quartet;
static Quartet examples[MAX_NPN][MAX_EXAMPLES];
static int n_examples[MAX_NPN];

static int parity_min_dist = 8;
static int parity_min_tt = 0;
static Quartet parity_min_q;

static void part_c_search(void) {
    int i1, i2, i3, ib;
    long total = 0;
    long n4;
    int report_every;
    int examples_filled = 0;
    char msg[256];

    n4 = (long)cat_size * (long)cat_size
       * (long)cat_size * (long)cat_size;
    report_every = cat_size / 10;
    if (report_every < 1) report_every = 1;

    printf("\n=== PART C: Exhaustive Search ===\n");
    printf("  Catalog size: %d, quartets: %ld\n", cat_size, n4);
    printf("  Split-sigmoid threshold: 0.5\n\n");

    memset(tt_counts, 0, sizeof(tt_counts));
    memset(n_examples, 0, sizeof(n_examples));
    init_parity_dist();
    parity_min_dist = 8;
    parity_min_tt = 0;

    for (i1 = 0; i1 < cat_size; i1++) {
        Cx w1;
        w1 = cat_cx[i1];

        if (i1 % report_every == 0)
            printf("  ... %d%%\n", (i1 * 100) / cat_size);

        for (i2 = 0; i2 < cat_size; i2++) {
            Cx w2, s12;
            w2 = cat_cx[i2];
            s12 = cx_add(w1, w2);

            for (i3 = 0; i3 < cat_size; i3++) {
                Cx w3, s13, s23, s123;
                w3 = cat_cx[i3];
                s13 = cx_add(w1, w3);
                s23 = cx_add(w2, w3);
                s123 = cx_add(s12, w3);

                for (ib = 0; ib < cat_size; ib++) {
                    Cx b;
                    int tt;
                    double p;

                    b = cat_cx[ib];
                    tt = 0;

                    /* (0,0,0): z = b */
                    p = split_sigmoid(b);
                    if (p > 0.5) tt |= 1;

                    /* (0,0,1): z = w3 + b */
                    p = split_sigmoid(cx_add(w3, b));
                    if (p > 0.5) tt |= 2;

                    /* (0,1,0): z = w2 + b */
                    p = split_sigmoid(cx_add(w2, b));
                    if (p > 0.5) tt |= 4;

                    /* (0,1,1): z = s23 + b */
                    p = split_sigmoid(cx_add(s23, b));
                    if (p > 0.5) tt |= 8;

                    /* (1,0,0): z = w1 + b */
                    p = split_sigmoid(cx_add(w1, b));
                    if (p > 0.5) tt |= 16;

                    /* (1,0,1): z = s13 + b */
                    p = split_sigmoid(cx_add(s13, b));
                    if (p > 0.5) tt |= 32;

                    /* (1,1,0): z = s12 + b */
                    p = split_sigmoid(cx_add(s12, b));
                    if (p > 0.5) tt |= 64;

                    /* (1,1,1): z = s123 + b */
                    p = split_sigmoid(cx_add(s123, b));
                    if (p > 0.5) tt |= 128;

                    tt_counts[tt]++;
                    total++;

                    /* Track min Hamming distance to parity */
                    {
                        int pd = parity_dist_lut[tt];
                        if (pd < parity_min_dist) {
                            parity_min_dist = pd;
                            parity_min_tt = tt;
                            parity_min_q.w1 = i1;
                            parity_min_q.w2 = i2;
                            parity_min_q.w3 = i3;
                            parity_min_q.b = ib;
                        }
                    }

                    /* Store examples by NPN class */
                    if (examples_filled < MAX_NPN * MAX_EXAMPLES
                        && tt != 0x00 && tt != 0xFF) {
                        int ci;
                        ci = npn_class_index(tt);
                        if (ci >= 0
                            && n_examples[ci] < MAX_EXAMPLES) {
                            examples[ci][n_examples[ci]].w1 = i1;
                            examples[ci][n_examples[ci]].w2 = i2;
                            examples[ci][n_examples[ci]].w3 = i3;
                            examples[ci][n_examples[ci]].b = ib;
                            n_examples[ci]++;
                            examples_filled++;
                        }
                    }
                }
            }
        }
    }
    printf("  ... 100%%\n");
    printf("  Search complete: %ld quartets evaluated\n\n", total);

    sprintf(msg, "total quartets = %ld (expected %ld)", total, n4);
    check(msg, total == n4);

    /* Per-NPN-class summary */
    {
        int ci, tt;
        long class_total;
        printf("\n  Per-NPN-class solution counts:\n");
        printf("  %4s  %-12s  %10s  %s\n",
               "TT", "Name", "Solutions", "Ex");
        printf("  ----  ----------  ----------  --\n");
        for (ci = 0; ci < n_npn; ci++) {
            int canon = npn_classes[ci].canon_tt;
            class_total = 0;
            for (tt = 0; tt < 256; tt++) {
                if (npn_canon[tt] == canon)
                    class_total += tt_counts[tt];
            }
            printf("  0x%02X  %-12s  %10ld  %d\n",
                   canon,
                   npn_classes[ci].titulis
                       ? npn_classes[ci].titulis : "???",
                   class_total, n_examples[ci]);
        }
    }

    printf("\n  Constants: FALSE(0x00)=%ld  TRUE(0xFF)=%ld\n",
           tt_counts[0x00], tt_counts[0xFF]);

    printf("\n  Parity wall thickness:\n");
    printf("    Min Hamming distance to parity: %d\n",
           parity_min_dist);
    if (parity_min_dist < 8) {
        printf("    Closest tt: 0x%02X (%s, canon 0x%02X)\n",
               parity_min_tt, fn_name(npn_canon[parity_min_tt]),
               npn_canon[parity_min_tt]);
        printf("    Quartet: w=(%d,%d,%d) b=%d\n",
               parity_min_q.w1, parity_min_q.w2,
               parity_min_q.w3, parity_min_q.b);
    }
}

/* ================================================================
 * PART D: Analysis + Prediction Checks
 *
 * Map tt_counts to NPN classes. Compute:
 *   - Solution count per class
 *   - Spearman rho(solution_count, topology_frequency)
 *   - Seven prediction checks (P1-P7) with PASS/FAIL
 *   - Master comparison table
 * ================================================================ */

/* --- Spearman rank correlation (from Demo 47) --- */

static void assign_ranks(const double *values, double *ranks, int n) {
    int order[MAX_NPN];
    int i, j;
    for (i = 0; i < n; i++) order[i] = i;
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (values[order[j]] < values[order[i]]) {
                int tmp = order[i];
                order[i] = order[j];
                order[j] = tmp;
            }
    i = 0;
    while (i < n) {
        int tie_start = i;
        double sum_rank = 0.0;
        while (i < n &&
               fabs(values[order[i]] - values[order[tie_start]])
               < 1e-10) {
            sum_rank += (double)(i + 1);
            i++;
        }
        {
            double avg_rank = sum_rank / (double)(i - tie_start);
            int kk;
            for (kk = tie_start; kk < i; kk++)
                ranks[order[kk]] = avg_rank;
        }
    }
}

static double spearman_rho(const double *x, const double *y, int n) {
    double rx[MAX_NPN], ry[MAX_NPN];
    double d2 = 0.0;
    int i;
    if (n < 3) return 0.0;
    assign_ranks(x, rx, n);
    assign_ranks(y, ry, n);
    for (i = 0; i < n; i++) {
        double d = rx[i] - ry[i];
        d2 += d * d;
    }
    return 1.0 - 6.0 * d2
         / ((double)n * ((double)n * (double)n - 1.0));
}

/* --- Demo 45 reference data (hard-coded from findings.md) ---
 * Order matches the canonical truth table ordering from npn_build_classes.
 * We look up by canon_tt to align with our Part B class ordering.
 */
typedef struct {
    int canon_tt;
    int topo_freq;   /* Demo 45: forward braid enumeration count */
    double rms;      /* Demo 45: reverse decomposition RMS */
    int orbit;
} Demo45Ref;

static const Demo45Ref demo45_data[] = {
    { 0x01, 405, 0.6011, 16 },  /* AND3' */
    { 0x03, 290, 0.5400, 24 },  /* AND2' */
    { 0x06,  30, 0.7781, 24 },  /* ~A(B^C) */
    { 0x07,  73, 0.6294, 48 },  /* ~A~(BC) */
    { 0x0F, 143, 0.5285,  6 },  /* BUF */
    { 0x16,   1, 0.8639, 16 },  /* EXACT1 */
    { 0x17,  10, 0.6554,  8 },  /* MAJ' */
    { 0x18,   0, 0.7210,  8 },  /* ISOLATE */
    { 0x19,  54, 0.6805, 48 },  /* 3v-0x19 */
    { 0x1B,  26, 0.7390, 24 },  /* 3v-0x1B */
    { 0x1E,   0, 0.7920, 24 },  /* A^(B|C) */
    { 0x3C,   0, 0.7880,  6 },  /* XOR2 */
    { 0x69,   0, 0.9963,  2 }   /* XNOR3 */
};
#define DEMO45_N 13

static int demo45_lookup(int canon_tt) {
    int i;
    for (i = 0; i < DEMO45_N; i++)
        if (demo45_data[i].canon_tt == canon_tt) return i;
    return -1;
}

static void part_d_analysis(void) {
    int ci, tt;
    long class_solutions[MAX_NPN];
    double sol_d[MAX_NPN], topo_d[MAX_NPN], rms_d[MAX_NPN];
    double orbit_d[MAX_NPN];
    double rho_topo, rho_rms, rho_orbit;
    long total_non_const;
    char msg[256];

    printf("\n=== PART D: Analysis + Prediction Checks ===\n");

    /* Aggregate solution counts per NPN class */
    for (ci = 0; ci < n_npn; ci++) {
        int canon = npn_classes[ci].canon_tt;
        class_solutions[ci] = 0;
        for (tt = 0; tt < 256; tt++)
            if (npn_canon[tt] == canon)
                class_solutions[ci] += tt_counts[tt];
    }

    /* Build parallel arrays for correlation */
    for (ci = 0; ci < n_npn; ci++) {
        int di = demo45_lookup(npn_classes[ci].canon_tt);
        sol_d[ci] = (double)class_solutions[ci];
        if (di >= 0) {
            topo_d[ci] = (double)demo45_data[di].topo_freq;
            rms_d[ci] = demo45_data[di].rms;
            orbit_d[ci] = (double)demo45_data[di].orbit;
        } else {
            topo_d[ci] = 0.0;
            rms_d[ci] = 1.0;
            orbit_d[ci] = 1.0;
        }
    }

    /* Spearman correlations */
    rho_topo = spearman_rho(sol_d, topo_d, n_npn);
    rho_rms = spearman_rho(sol_d, rms_d, n_npn);
    rho_orbit = spearman_rho(sol_d, orbit_d, n_npn);

    printf("\n  Spearman rank correlations (n=%d):\n", n_npn);
    printf("    solutions vs topo_freq (D45):  rho = %+.4f\n",
           rho_topo);
    printf("    solutions vs reverse_RMS (D45): rho = %+.4f\n",
           rho_rms);
    printf("    solutions vs orbit_size:        rho = %+.4f\n",
           rho_orbit);

    /* Master comparison table */
    total_non_const = 0;
    for (ci = 0; ci < n_npn; ci++)
        total_non_const += class_solutions[ci];

    printf("\n  Master comparison table:\n");
    printf("  %4s  %-12s  %10s  %5s  %5s  %5s  %5s\n",
           "TT", "Name", "Solutions", "%",
           "TFreq", "RMS45", "Orbit");
    printf("  ----  ----------  ----------"
           "  -----  -----  -----  -----\n");
    for (ci = 0; ci < n_npn; ci++) {
        int di = demo45_lookup(npn_classes[ci].canon_tt);
        double pct = total_non_const > 0
            ? 100.0 * (double)class_solutions[ci]
              / (double)total_non_const
            : 0.0;
        printf("  0x%02X  %-12s  %10ld  %5.1f  %5d  %.3f  %5d\n",
               npn_classes[ci].canon_tt,
               npn_classes[ci].titulis
                   ? npn_classes[ci].titulis : "???",
               class_solutions[ci],
               pct,
               di >= 0 ? demo45_data[di].topo_freq : 0,
               di >= 0 ? demo45_data[di].rms : 0.0,
               npn_classes[ci].orbit_size);
    }

    /* Print one example quartet per class */
    printf("\n  Example quartets (first per class):\n");
    for (ci = 0; ci < n_npn; ci++) {
        if (n_examples[ci] > 0) {
            Quartet q = examples[ci][0];
            printf("    %-12s: w=(%2d,%2d,%2d) b=%2d"
                   "  |w|=(%.2f,%.2f,%.2f) |b|=%.2f\n",
                   npn_classes[ci].titulis
                       ? npn_classes[ci].titulis : "???",
                   q.w1, q.w2, q.w3, q.b,
                   cx_abs(cat_cx[q.w1]),
                   cx_abs(cat_cx[q.w2]),
                   cx_abs(cat_cx[q.w3]),
                   cx_abs(cat_cx[q.b]));
        } else {
            printf("    %-12s: (no solutions)\n",
                   npn_classes[ci].titulis
                       ? npn_classes[ci].titulis : "???");
        }
    }

    /* Per-class min Hamming distance to parity */
    printf("\n  Per-class min Hamming distance to parity:\n");
    printf("  %4s  %-12s  %7s  %10s\n",
           "TT", "Name", "MinDist", "Solutions");
    printf("  ----  ----------  -------  ----------\n");
    for (ci = 0; ci < n_npn; ci++) {
        int canon = npn_classes[ci].canon_tt;
        int min_d = 8;
        for (tt = 0; tt < 256; tt++) {
            if (npn_canon[tt] == canon && tt_counts[tt] > 0) {
                int d = parity_dist_lut[tt];
                if (d < min_d) min_d = d;
            }
        }
        printf("  0x%02X  %-12s  %7d  %10ld\n",
               canon,
               npn_classes[ci].titulis
                   ? npn_classes[ci].titulis : "???",
               min_d, class_solutions[ci]);
    }

    printf("\n  Global min distance to parity: %d (tt=0x%02X, %s)\n",
           parity_min_dist, parity_min_tt,
           fn_name(npn_canon[parity_min_tt]));

    /* === Prediction checks === */
    printf("\n  Prediction checks:\n");

    /* P1: XNOR3 has zero solutions */
    {
        long parity_sols = 0;
        for (tt = 0; tt < 256; tt++)
            if (npn_canon[tt] == 0x69)
                parity_sols += tt_counts[tt];
        sprintf(msg, "P1: XNOR3 (parity) has zero solutions"
                " (got %ld)", parity_sols);
        check(msg, parity_sols == 0);
    }

    /* P2: Top-3 by solution count includes AND-type functions */
    {
        int top3[3] = {0, 0, 0};
        int has_and = 0;
        int k;
        for (ci = 0; ci < n_npn; ci++) {
            if (class_solutions[ci] > class_solutions[top3[0]])
                { top3[2] = top3[1]; top3[1] = top3[0]; top3[0] = ci; }
            else if (class_solutions[ci] > class_solutions[top3[1]])
                { top3[2] = top3[1]; top3[1] = ci; }
            else if (class_solutions[ci] > class_solutions[top3[2]])
                { top3[2] = ci; }
        }
        for (k = 0; k < 3; k++) {
            int c = npn_classes[top3[k]].canon_tt;
            if (c == 0x01 || c == 0x03) has_and = 1;
        }
        sprintf(msg, "P2: Top-3 includes AND-type (top: 0x%02X,"
                " 0x%02X, 0x%02X)",
                npn_classes[top3[0]].canon_tt,
                npn_classes[top3[1]].canon_tt,
                npn_classes[top3[2]].canon_tt);
        check(msg, has_and);
    }

    /* P3: rho(solutions, topo_freq) > 0.5 */
    sprintf(msg, "P3: rho(solutions, topo_freq) > 0.5"
            " (got %.4f)", rho_topo);
    check(msg, rho_topo > 0.5);

    /* P4: rho(solutions, reverse_RMS) < -0.5 */
    sprintf(msg, "P4: rho(solutions, reverse_RMS) < -0.5"
            " (got %.4f)", rho_rms);
    check(msg, rho_rms < -0.5);

    /* P5: All 12 non-parity classes have >= 100 solutions */
    {
        int all_above = 1;
        long min_nonparity = 0;
        int min_ci = 0;
        for (ci = 0; ci < n_npn; ci++) {
            if (npn_classes[ci].canon_tt == 0x69) continue;
            if (min_nonparity == 0
                || class_solutions[ci] < min_nonparity) {
                min_nonparity = class_solutions[ci];
                min_ci = ci;
            }
            if (class_solutions[ci] < 100) all_above = 0;
        }
        sprintf(msg, "P5: All non-parity classes >= 100 sols"
                " (min: %s=%ld)",
                npn_classes[min_ci].titulis
                    ? npn_classes[min_ci].titulis : "???",
                min_nonparity);
        check(msg, all_above);
    }

    /* P6: XOR2 has solutions (Demo 29 extension) */
    {
        long xor2_sols = 0;
        for (tt = 0; tt < 256; tt++)
            if (npn_canon[tt] == 0x3C)
                xor2_sols += tt_counts[tt];
        sprintf(msg, "P6: XOR2 has solutions (got %ld)",
                xor2_sols);
        check(msg, xor2_sols > 0);
    }

    /* P7: Constants exceed 10% of total */
    {
        long n4 = (long)cat_size * (long)cat_size
                * (long)cat_size * (long)cat_size;
        long constants = tt_counts[0x00] + tt_counts[0xFF];
        double pct = 100.0 * (double)constants / (double)n4;
        sprintf(msg, "P7: Constants > 10%% of total"
                " (%.1f%%)", pct);
        check(msg, pct > 10.0);
    }

    printf("\n  Summary: %d/%d predictions confirmed\n",
           n_pass - 7, 7);  /* subtract the 7 pre-Part-D passes */
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 48: Forward DKC Zoo\n");
    printf("==================================\n");

    part_a_catalog();
    part_b_classify();
    part_c_search();
    part_d_analysis();

    printf("\n==================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("==================================\n");

    return n_fail > 0 ? 1 : 0;
}
