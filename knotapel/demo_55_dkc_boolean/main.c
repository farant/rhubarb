/*
 * KNOTAPEL DEMO 55: DKC Boolean Search at ell=4 (delta=sqrt(2))
 * ==============================================================
 *
 * Demo 54 built the bracket catalog at delta=sqrt(2):
 *   - 25 distinct values at n=3, 28 at n=4
 *   - 100% Z[i]-axial: every value = (a+bi)*zeta_16^c
 *   - Complete negation pairing in the Gaussian integer catalog
 *
 * This demo searches for Boolean functions computable by DKC neurons
 * at ell=4, using the multiplicative encoding:
 *   z(x1,...,xn) = w1^x1 * w2^x2 * ... * wn^xn
 *
 * 2-input: quartet (1, w2, w1, w1*w2) -> 4-bit truth table (16 functions)
 * 3-input: octet (1, w3, ..., w1*w2*w3) -> 8-bit truth table (NPN classes)
 *
 * Activation functions:
 *   1. Split-sigmoid: sigma(Re)*[1-sigma(Im)] + [1-sigma(Re)]*sigma(Im) > 0.5
 *   2. k-sector MVN: divide circle into k sectors, output = sector mod 2
 *   3. Magnitude threshold: |z| > tau
 *
 * Key question: how do the achievable functions compare to delta=0?
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
static Cx cx_add(Cx a, Cx b) {
    return cx_make(a.re + b.re, a.im + b.im);
}
static double cx_abs(Cx a) {
    return sqrt(a.re * a.re + a.im * a.im);
}

/* ================================================================
 * Z[zeta_16] Arithmetic
 *
 * zeta = e^{i*pi/8}, zeta^8 = -1.
 * Elements: c[0] + c[1]*z + ... + c[7]*z^7
 * Multiplication: convolution mod z^8 = -1.
 *
 * Key values:
 *   A = zeta^5, A^{-1} = -zeta^3
 *   delta = zeta^2 - zeta^6 = sqrt(2)
 * ================================================================ */

typedef struct { long c[8]; } Cyc16;

static Cyc16 cyc16_zero(void) {
    Cyc16 r;
    memset(r.c, 0, sizeof(r.c));
    return r;
}

static Cyc16 cyc16_one(void) {
    Cyc16 r;
    memset(r.c, 0, sizeof(r.c));
    r.c[0] = 1;
    return r;
}

static Cyc16 cyc16_zeta_power(int k) {
    Cyc16 r;
    memset(r.c, 0, sizeof(r.c));
    k = k % 16;
    if (k < 0) k += 16;
    if (k < 8) { r.c[k] = 1; }
    else       { r.c[k - 8] = -1; }
    return r;
}

static Cyc16 cyc16_add(Cyc16 a, Cyc16 b) {
    Cyc16 r;
    int i;
    for (i = 0; i < 8; i++) r.c[i] = a.c[i] + b.c[i];
    return r;
}

static Cyc16 cyc16_neg(Cyc16 a) {
    Cyc16 r;
    int i;
    for (i = 0; i < 8; i++) r.c[i] = -a.c[i];
    return r;
}

static Cyc16 cyc16_mul(Cyc16 a, Cyc16 b) {
    Cyc16 r;
    int i, j;
    memset(r.c, 0, sizeof(r.c));
    for (i = 0; i < 8; i++) {
        if (a.c[i] == 0) continue;
        for (j = 0; j < 8; j++) {
            if (b.c[j] == 0) continue;
            if (i + j < 8)
                r.c[i + j] += a.c[i] * b.c[j];
            else
                r.c[i + j - 8] -= a.c[i] * b.c[j];
        }
    }
    return r;
}

static int cyc16_eq(Cyc16 a, Cyc16 b) {
    int i;
    for (i = 0; i < 8; i++)
        if (a.c[i] != b.c[i]) return 0;
    return 1;
}

static int cyc16_is_zero(Cyc16 a) {
    int i;
    for (i = 0; i < 8; i++)
        if (a.c[i] != 0) return 0;
    return 1;
}

static Cx cyc16_to_cx(Cyc16 a) {
    Cx r = cx_make(0.0, 0.0);
    int k;
    for (k = 0; k < 8; k++) {
        if (a.c[k] != 0) {
            double angle = (double)k * M_PI / 8.0;
            r = cx_add(r, cx_make((double)a.c[k] * cos(angle),
                                   (double)a.c[k] * sin(angle)));
        }
    }
    return r;
}

/* A^power: A = zeta^5 */
static Cyc16 cyc16_a_power(int power) {
    int zp = (5 * power) % 16;
    if (zp < 0) zp += 16;
    return cyc16_zeta_power(zp);
}

/* delta^k: delta = sqrt(2) = zeta^2 - zeta^6 */
static Cyc16 cyc16_delta_power(int k) {
    Cyc16 r;
    long two_pow;
    int m, i;
    memset(r.c, 0, sizeof(r.c));
    if (k == 0) { r.c[0] = 1; return r; }
    if (k < 0) {
        printf("  ERROR: negative delta power %d\n", k);
        return r;
    }
    m = k / 2;
    two_pow = 1;
    for (i = 0; i < m; i++) two_pow *= 2;
    if (k % 2 == 0) { r.c[0] = two_pow; }
    else             { r.c[2] = two_pow; r.c[6] = -two_pow; }
    return r;
}

/* Z[i]-axiality check */
static int cyc16_is_zi_axial(Cyc16 a) {
    int pairs_active = 0;
    int k;
    for (k = 0; k < 4; k++) {
        if (a.c[k] != 0 || a.c[k + 4] != 0)
            pairs_active++;
    }
    return pairs_active <= 1;
}

static void cyc16_print(const char *label, Cyc16 a) {
    Cx z = cyc16_to_cx(a);
    printf("  %s = (%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld)"
           "  [%.4f + %.4fi]\n",
           label,
           a.c[0], a.c[1], a.c[2], a.c[3],
           a.c[4], a.c[5], a.c[6], a.c[7],
           z.re, z.im);
}

/* ================================================================
 * Planar Matching + Diagram Operations
 * ================================================================ */

#define MAX_N 6
#define MAX_2N (2 * MAX_N)

typedef struct { int match[MAX_2N]; } PlanarMatch;

static PlanarMatch make_identity_diagram(int n) {
    PlanarMatch m;
    int k;
    for (k = 0; k < n; k++) {
        m.match[k] = n + k;
        m.match[n + k] = k;
    }
    return m;
}

static PlanarMatch make_generator_diagram(int n, int gen) {
    PlanarMatch m;
    int k;
    for (k = 0; k < n; k++) {
        m.match[k] = n + k;
        m.match[n + k] = k;
    }
    m.match[gen] = gen + 1;
    m.match[gen + 1] = gen;
    m.match[n + gen] = n + gen + 1;
    m.match[n + gen + 1] = n + gen;
    return m;
}

static int compose_diagrams(int n, const PlanarMatch *d1,
                            const PlanarMatch *d2, PlanarMatch *result) {
    int glue_visited[MAX_N];
    int i, loops;

    memset(result->match, -1, (size_t)(2 * n) * sizeof(int));
    memset(glue_visited, 0, (size_t)n * sizeof(int));
    loops = 0;

    for (i = 0; i < 2 * n; i++) {
        int start = i;
        int in_d1, cur, partner;

        if (result->match[i] >= 0) continue;
        if (i < n) { in_d1 = 1; cur = i; }
        else       { in_d1 = 0; cur = i; }

        for (;;) {
            if (in_d1) {
                partner = d1->match[cur];
                if (partner < n) {
                    result->match[start] = partner;
                    result->match[partner] = start;
                    break;
                }
                glue_visited[partner - n] = 1;
                in_d1 = 0;
                cur = partner - n;
            } else {
                partner = d2->match[cur];
                if (partner >= n) {
                    result->match[start] = partner;
                    result->match[partner] = start;
                    break;
                }
                glue_visited[partner] = 1;
                in_d1 = 1;
                cur = n + partner;
            }
        }
    }

    for (i = 0; i < n; i++) {
        int cur2, p, q;
        if (glue_visited[i]) continue;
        loops++;
        cur2 = i;
        do {
            glue_visited[cur2] = 1;
            p = d2->match[cur2];
            glue_visited[p] = 1;
            q = d1->match[n + p];
            cur2 = q - n;
        } while (cur2 != i);
    }

    return loops;
}

static int trace_closure_loops(int n, const PlanarMatch *m) {
    int visited[MAX_2N];
    int i, cur, loops = 0;
    memset(visited, 0, (size_t)(2 * n) * sizeof(int));
    for (i = 0; i < 2 * n; i++) {
        if (visited[i]) continue;
        loops++;
        cur = i;
        do {
            visited[cur] = 1;
            cur = m->match[cur];
            visited[cur] = 1;
            cur = (cur < n) ? cur + n : cur - n;
        } while (cur != i);
    }
    return loops;
}

/* ================================================================
 * State-Sum Bracket at delta=sqrt(2)
 *
 * bracket = sum_s A^{a_power(s)} * delta^{total_loops(s) - 1}
 * ================================================================ */

typedef struct {
    int gen;   /* 0-indexed */
    int sign;  /* +1 or -1 */
} BraidCrossing;

static Cyc16 state_sum_bracket(int n, const BraidCrossing *word,
                                int word_len) {
    Cyc16 bracket = cyc16_zero();
    int num_states, s;

    if (word_len > 20) return bracket;

    num_states = 1 << word_len;

    for (s = 0; s < num_states; s++) {
        PlanarMatch current;
        int total_glue_loops = 0;
        int a_power = 0;
        int j;

        current = make_identity_diagram(n);

        for (j = 0; j < word_len; j++) {
            PlanarMatch resolved, result;
            int glue_loops, bit;

            bit = (s >> j) & 1;

            if (bit == 0) {
                resolved = make_identity_diagram(n);
                a_power += word[j].sign;
            } else {
                resolved = make_generator_diagram(n, word[j].gen);
                a_power -= word[j].sign;
            }

            glue_loops = compose_diagrams(n, &current, &resolved,
                                           &result);
            total_glue_loops += glue_loops;
            current = result;
        }

        {
            int tl = trace_closure_loops(n, &current);
            int total = total_glue_loops + tl;
            Cyc16 weight = cyc16_delta_power(total - 1);
            Cyc16 contrib = cyc16_mul(weight,
                                       cyc16_a_power(a_power));
            bracket = cyc16_add(bracket, contrib);
        }
    }
    return bracket;
}

static void decode_braid(int idx, int n_choices, int length,
                          BraidCrossing *word) {
    int i;
    for (i = 0; i < length; i++) {
        int choice = idx % n_choices;
        word[i].gen = choice / 2;
        word[i].sign = (choice % 2 == 0) ? 1 : -1;
        idx /= n_choices;
    }
}

/* ================================================================
 * Bracket Catalog
 * ================================================================ */

#define MAX_CATALOG 512

static Cyc16 cat_cyc[MAX_CATALOG];
static Cx    cat_cx[MAX_CATALOG];
static int   cat_size = 0;

static int cat_find(Cyc16 val) {
    int i;
    for (i = 0; i < cat_size; i++)
        if (cyc16_eq(val, cat_cyc[i])) return i;
    return -1;
}

static void build_catalog(void) {
    int ci;
    int configs_n[3]       = {2, 3, 4};
    int configs_choices[3]  = {2, 4, 6};
    int configs_maxlen[3]   = {10, 6, 6};

    cat_size = 0;

    for (ci = 0; ci < 3; ci++) {
        int n = configs_n[ci];
        int n_choices = configs_choices[ci];
        int max_len = configs_maxlen[ci];
        int length;

        for (length = 1; length <= max_len; length++) {
            int total = 1;
            int idx, i;

            for (i = 0; i < length; i++) {
                total *= n_choices;
                if (total > 100000) { total = -1; break; }
            }
            if (total < 0) break;

            for (idx = 0; idx < total; idx++) {
                BraidCrossing word[20];
                Cyc16 bracket;

                decode_braid(idx, n_choices, length, word);
                bracket = state_sum_bracket(n, word, length);

                if (!cyc16_is_zero(bracket)
                    && cat_find(bracket) < 0
                    && cat_size < MAX_CATALOG) {
                    cat_cyc[cat_size] = bracket;
                    cat_cx[cat_size] = cyc16_to_cx(bracket);
                    cat_size++;
                }
            }
        }
    }
}

/* ================================================================
 * Activation Functions
 * ================================================================ */

static double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

static int split_sigmoid_classify(Cx z) {
    double sr = sigmoid(z.re);
    double si = sigmoid(z.im);
    double val = sr * (1.0 - si) + (1.0 - sr) * si;
    return val > 0.5 ? 1 : 0;
}

static int sector_classify(Cx z, int k) {
    double angle, sector_width;
    int sector;
    if (cx_abs(z) < 1e-15) return 0;
    angle = atan2(z.im, z.re);
    if (angle < 0.0) angle += 2.0 * M_PI;
    sector_width = 2.0 * M_PI / (double)k;
    sector = (int)(angle / sector_width);
    if (sector >= k) sector = k - 1;
    return sector % 2;
}

static int magnitude_classify(Cx z, double tau) {
    return cx_abs(z) > tau ? 1 : 0;
}

/* Half-plane activations: simplest possible */
static int re_positive_classify(Cx z) {
    return z.re > 0.0 ? 1 : 0;
}

static int im_positive_classify(Cx z) {
    return z.im > 0.0 ? 1 : 0;
}

/* ================================================================
 * 2-Input Boolean Function Names
 *
 * Truth table encoding: bit i = f(x1, x2) where x1 = i>>1, x2 = i&1.
 *   i=0: (0,0), i=1: (0,1), i=2: (1,0), i=3: (1,1)
 * ================================================================ */

static const char *fn2_name(int tt) {
    switch (tt) {
    case 0x0: return "FALSE";
    case 0x1: return "AND";
    case 0x2: return "A>B";
    case 0x3: return "PROJ_A";
    case 0x4: return "B>A";
    case 0x5: return "PROJ_B";
    case 0x6: return "XOR";
    case 0x7: return "OR";
    case 0x8: return "NOR";
    case 0x9: return "XNOR";
    case 0xA: return "NOT_B";
    case 0xB: return "A>=B";
    case 0xC: return "NOT_A";
    case 0xD: return "B>=A";
    case 0xE: return "NAND";
    case 0xF: return "TRUE";
    default:  return "???";
    }
}

/* ================================================================
 * NPN Classification for 3-Input Functions (from Demos 48/50)
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

static const char *fn3_name(int tt) {
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
    default:   return "???";
    }
}

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

    for (i = 1; i < 255; i++) {
        int c = npn_canon[i];
        if (!seen[c]) {
            seen[c] = 1;
            npn_classes[n_npn].canon_tt = c;
            npn_classes[n_npn].orbit_size = 0;
            npn_classes[n_npn].titulis = fn3_name(c);
            n_npn++;
        }
    }

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

/* ================================================================
 * PART A: Build and Verify Catalog
 * ================================================================ */

static void part_a_catalog(void) {
    int i, n_zi = 0;
    char msg[256];

    printf("\n=== PART A: Bracket Catalog (delta=sqrt(2)) ===\n\n");
    printf("  Building from state-sum: n=2 (len 1-10),"
           " n=3 (len 1-6), n=4 (len 1-6)...\n");

    build_catalog();

    printf("  Catalog size: %d distinct nonzero values\n", cat_size);

    for (i = 0; i < cat_size; i++)
        if (cyc16_is_zi_axial(cat_cyc[i])) n_zi++;

    sprintf(msg, "all Z[i]-axial (%d/%d)", n_zi, cat_size);
    check(msg, n_zi == cat_size);

    sprintf(msg, "catalog has >= 20 values (got %d)", cat_size);
    check(msg, cat_size >= 20);

    /* Print first 20 values */
    printf("\n  First 20 catalog values:\n");
    for (i = 0; i < cat_size && i < 20; i++) {
        printf("    [%2d] (%3ld,%3ld,%3ld,%3ld,%3ld,%3ld,%3ld,%3ld)"
               "  |z|=%.4f\n",
               i,
               cat_cyc[i].c[0], cat_cyc[i].c[1],
               cat_cyc[i].c[2], cat_cyc[i].c[3],
               cat_cyc[i].c[4], cat_cyc[i].c[5],
               cat_cyc[i].c[6], cat_cyc[i].c[7],
               cx_abs(cat_cx[i]));
    }
    if (cat_size > 20)
        printf("    ... (%d more)\n", cat_size - 20);

    /* Verify A * A^{-1} = 1 */
    {
        Cyc16 A = cyc16_zeta_power(5);
        Cyc16 A_inv = cyc16_neg(cyc16_zeta_power(3));
        Cyc16 prod = cyc16_mul(A, A_inv);
        check("A * A^{-1} = 1", cyc16_eq(prod, cyc16_one()));
    }

    /* Verify delta^2 = 2 */
    {
        Cyc16 delta = cyc16_add(cyc16_zeta_power(2),
                                 cyc16_neg(cyc16_zeta_power(6)));
        Cyc16 d2 = cyc16_mul(delta, delta);
        Cyc16 two = cyc16_zero();
        two.c[0] = 2;
        check("delta^2 = 2", cyc16_eq(d2, two));
    }

    /* Suppress unused */
    (void)cyc16_print;
}

/* ================================================================
 * PART B: 2-Input Multiplicative Search
 *
 * For each (w1, w2) from catalog:
 *   (0,0) -> z = 1
 *   (0,1) -> z = w2
 *   (1,0) -> z = w1
 *   (1,1) -> z = w1 * w2  (Cyc16 product -> Cx)
 *
 * Apply activation, get 4-bit truth table.
 * ================================================================ */

static void search_2input(const char *act_name,
                           int (*classify)(Cx, int),
                           int act_param) {
    long tt_counts[16];
    int i1, i2, tt;
    long total = 0;
    int n_achievable = 0;
    Cx one_cx = cx_make(1.0, 0.0);

    memset(tt_counts, 0, sizeof(tt_counts));

    for (i1 = 0; i1 < cat_size; i1++) {
        Cx w1 = cat_cx[i1];
        for (i2 = 0; i2 < cat_size; i2++) {
            Cx w2 = cat_cx[i2];
            Cyc16 prod16 = cyc16_mul(cat_cyc[i1], cat_cyc[i2]);
            Cx w12 = cyc16_to_cx(prod16);
            int bits = 0;

            /* bit 0: (0,0) -> z = 1 */
            if (classify(one_cx, act_param)) bits |= 1;
            /* bit 1: (0,1) -> z = w2 */
            if (classify(w2, act_param)) bits |= 2;
            /* bit 2: (1,0) -> z = w1 */
            if (classify(w1, act_param)) bits |= 4;
            /* bit 3: (1,1) -> z = w1*w2 */
            if (classify(w12, act_param)) bits |= 8;

            tt_counts[bits]++;
            total++;
        }
    }

    printf("  --- %s ---\n", act_name);
    printf("  Pairs: %ld, catalog: %d\n", total, cat_size);
    printf("  %4s  %-8s  %10s\n", "TT", "Name", "Count");
    printf("  ----  --------  ----------\n");
    for (tt = 0; tt < 16; tt++) {
        if (tt_counts[tt] > 0) n_achievable++;
        printf("  0x%X   %-8s  %10ld%s\n",
               tt, fn2_name(tt), tt_counts[tt],
               tt_counts[tt] > 0 ? "" : "  (unreachable)");
    }
    printf("  Achievable: %d / 16 functions\n\n", n_achievable);
}

/* Wrappers for uniform interface: classify(Cx, int_param) -> 0/1 */
static int wrap_split_sigmoid(Cx z, int param) {
    (void)param;
    return split_sigmoid_classify(z);
}

static int wrap_sector(Cx z, int k) {
    return sector_classify(z, k);
}

static int wrap_magnitude(Cx z, int tau_x10) {
    return magnitude_classify(z, (double)tau_x10 / 10.0);
}

static int wrap_re_positive(Cx z, int param) {
    (void)param;
    return re_positive_classify(z);
}

static int wrap_im_positive(Cx z, int param) {
    (void)param;
    return im_positive_classify(z);
}

static void part_b_2input(void) {
    printf("\n=== PART B: 2-Input Multiplicative Search ===\n\n");
    printf("  Encoding: z(x1,x2) = w1^x1 * w2^x2\n");
    printf("  Quartet: (1, w2, w1, w1*w2)\n\n");

    search_2input("Re(z) > 0", wrap_re_positive, 0);
    search_2input("Im(z) > 0", wrap_im_positive, 0);
    search_2input("Split-sigmoid (>0.5)", wrap_split_sigmoid, 0);
    search_2input("Sector k=2", wrap_sector, 2);
    search_2input("Sector k=4", wrap_sector, 4);
    search_2input("Sector k=6", wrap_sector, 6);
    search_2input("Sector k=8", wrap_sector, 8);
    search_2input("Magnitude tau=0.5", wrap_magnitude, 5);
    search_2input("Magnitude tau=1.0", wrap_magnitude, 10);
    search_2input("Magnitude tau=1.5", wrap_magnitude, 15);

    /* Summary check: XOR achievable somewhere */
    {
        int found_xor = 0;
        int i1, i2;
        Cx one_cx = cx_make(1.0, 0.0);

        /* Check sector k=4 for XOR */
        for (i1 = 0; i1 < cat_size && !found_xor; i1++) {
            Cx w1 = cat_cx[i1];
            for (i2 = 0; i2 < cat_size && !found_xor; i2++) {
                Cx w2 = cat_cx[i2];
                Cyc16 prod16 = cyc16_mul(cat_cyc[i1], cat_cyc[i2]);
                Cx w12 = cyc16_to_cx(prod16);
                int bits = 0;

                if (sector_classify(one_cx, 4)) bits |= 1;
                if (sector_classify(w2, 4)) bits |= 2;
                if (sector_classify(w1, 4)) bits |= 4;
                if (sector_classify(w12, 4)) bits |= 8;

                if (bits == 0x6 || bits == 0x9) found_xor = 1;
            }
        }

        check("XOR or XNOR achievable (2-input, some activation)",
              found_xor);
    }
}

/* ================================================================
 * PART C: 3-Input Multiplicative Search
 *
 * For each (w1, w2, w3) from catalog:
 *   (0,0,0) -> z = 1
 *   (0,0,1) -> z = w3
 *   (0,1,0) -> z = w2
 *   (0,1,1) -> z = w2*w3
 *   (1,0,0) -> z = w1
 *   (1,0,1) -> z = w1*w3
 *   (1,1,0) -> z = w1*w2
 *   (1,1,1) -> z = w1*w2*w3
 *
 * NPN classification -> 13 non-trivial classes.
 * ================================================================ */

static void search_3input(const char *act_name,
                           int (*classify)(Cx, int),
                           int act_param) {
    long tt_counts[256];
    long class_solutions[MAX_NPN];
    int i1, i2, i3, ci, tt;
    long total = 0;
    int n_reachable = 0;
    Cx one_cx = cx_make(1.0, 0.0);

    memset(tt_counts, 0, sizeof(tt_counts));

    for (i1 = 0; i1 < cat_size; i1++) {
        Cx cx1 = cat_cx[i1];
        for (i2 = 0; i2 < cat_size; i2++) {
            Cx cx2 = cat_cx[i2];
            Cyc16 w12_cyc = cyc16_mul(cat_cyc[i1], cat_cyc[i2]);
            Cx cx12 = cyc16_to_cx(w12_cyc);

            for (i3 = 0; i3 < cat_size; i3++) {
                Cx cx3 = cat_cx[i3];
                Cyc16 w23_cyc = cyc16_mul(cat_cyc[i2], cat_cyc[i3]);
                Cyc16 w13_cyc = cyc16_mul(cat_cyc[i1], cat_cyc[i3]);
                Cyc16 w123_cyc = cyc16_mul(w12_cyc, cat_cyc[i3]);
                Cx cx23 = cyc16_to_cx(w23_cyc);
                Cx cx13 = cyc16_to_cx(w13_cyc);
                Cx cx123 = cyc16_to_cx(w123_cyc);
                int bits = 0;

                /* bit 0: (0,0,0) -> 1 */
                if (classify(one_cx, act_param)) bits |= 1;
                /* bit 1: (0,0,1) -> w3 */
                if (classify(cx3, act_param)) bits |= 2;
                /* bit 2: (0,1,0) -> w2 */
                if (classify(cx2, act_param)) bits |= 4;
                /* bit 3: (0,1,1) -> w2*w3 */
                if (classify(cx23, act_param)) bits |= 8;
                /* bit 4: (1,0,0) -> w1 */
                if (classify(cx1, act_param)) bits |= 16;
                /* bit 5: (1,0,1) -> w1*w3 */
                if (classify(cx13, act_param)) bits |= 32;
                /* bit 6: (1,1,0) -> w1*w2 */
                if (classify(cx12, act_param)) bits |= 64;
                /* bit 7: (1,1,1) -> w1*w2*w3 */
                if (classify(cx123, act_param)) bits |= 128;

                tt_counts[bits]++;
                total++;
            }
        }
    }

    /* Aggregate per NPN class */
    for (ci = 0; ci < n_npn; ci++) {
        int canon = npn_classes[ci].canon_tt;
        class_solutions[ci] = 0;
        for (tt = 0; tt < 256; tt++)
            if (npn_canon[tt] == canon)
                class_solutions[ci] += tt_counts[tt];
    }

    printf("  --- %s ---\n", act_name);
    printf("  Triples: %ld, catalog: %d\n", total, cat_size);
    printf("  %4s  %-12s  %10s\n", "TT", "Name", "Solutions");
    printf("  ----  ----------  ----------\n");
    for (ci = 0; ci < n_npn; ci++) {
        if (class_solutions[ci] > 0) n_reachable++;
        printf("  0x%02X  %-12s  %10ld%s\n",
               npn_classes[ci].canon_tt,
               npn_classes[ci].titulis
                   ? npn_classes[ci].titulis : "???",
               class_solutions[ci],
               npn_classes[ci].canon_tt == 0x69
                   && class_solutions[ci] > 0
                   ? "  <<< PARITY" : "");
    }
    printf("  Constants: FALSE=%ld  TRUE=%ld\n",
           tt_counts[0x00], tt_counts[0xFF]);
    printf("  Reachable NPN classes: %d / %d\n\n",
           n_reachable, n_npn);
}

static void part_c_3input(void) {
    char msg[256];

    printf("\n=== PART C: 3-Input Multiplicative Search ===\n\n");
    printf("  Encoding: z(x1,x2,x3) = w1^x1 * w2^x2 * w3^x3\n");
    printf("  NPN classification: 13 non-trivial classes\n\n");

    npn_init();
    npn_build_classes();

    sprintf(msg, "NPN class count = 13 (got %d)", n_npn);
    check(msg, n_npn == 13);

    search_3input("Re(z) > 0", wrap_re_positive, 0);
    search_3input("Im(z) > 0", wrap_im_positive, 0);
    search_3input("Split-sigmoid (>0.5)", wrap_split_sigmoid, 0);
    search_3input("Sector k=2", wrap_sector, 2);
    search_3input("Sector k=4", wrap_sector, 4);
    search_3input("Sector k=6", wrap_sector, 6);
    search_3input("Sector k=8", wrap_sector, 8);
    search_3input("Magnitude tau=1.0", wrap_magnitude, 10);
}

/* ================================================================
 * PART D: Comparison and Analysis
 * ================================================================ */

static void part_d_analysis(void) {
    int ci;
    int k;
    long k_parity[9];
    long k_reachable[9];
    char msg[256];
    Cx one_cx = cx_make(1.0, 0.0);

    printf("\n=== PART D: Analysis ===\n\n");

    /* k-sector sweep for parity: for each k, count parity triples */
    memset(k_parity, 0, sizeof(k_parity));
    memset(k_reachable, 0, sizeof(k_reachable));

    for (k = 2; k <= 8; k++) {
        int i1, i2, i3;
        int class_seen[MAX_NPN];
        memset(class_seen, 0, sizeof(class_seen));

        for (i1 = 0; i1 < cat_size; i1++) {
            Cx cx1 = cat_cx[i1];
            for (i2 = 0; i2 < cat_size; i2++) {
                Cx cx2 = cat_cx[i2];
                Cyc16 w12_cyc = cyc16_mul(cat_cyc[i1], cat_cyc[i2]);
                Cx cx12 = cyc16_to_cx(w12_cyc);

                for (i3 = 0; i3 < cat_size; i3++) {
                    Cx cx3 = cat_cx[i3];
                    Cyc16 w23_cyc, w13_cyc, w123_cyc;
                    Cx cx23, cx13, cx123;
                    int bits = 0;
                    int canon;

                    w23_cyc = cyc16_mul(cat_cyc[i2], cat_cyc[i3]);
                    w13_cyc = cyc16_mul(cat_cyc[i1], cat_cyc[i3]);
                    w123_cyc = cyc16_mul(w12_cyc, cat_cyc[i3]);
                    cx23 = cyc16_to_cx(w23_cyc);
                    cx13 = cyc16_to_cx(w13_cyc);
                    cx123 = cyc16_to_cx(w123_cyc);

                    if (sector_classify(one_cx, k)) bits |= 1;
                    if (sector_classify(cx3, k)) bits |= 2;
                    if (sector_classify(cx2, k)) bits |= 4;
                    if (sector_classify(cx23, k)) bits |= 8;
                    if (sector_classify(cx1, k)) bits |= 16;
                    if (sector_classify(cx13, k)) bits |= 32;
                    if (sector_classify(cx12, k)) bits |= 64;
                    if (sector_classify(cx123, k)) bits |= 128;

                    canon = npn_canon[bits];
                    if (canon == 0x69) k_parity[k]++;
                    if (bits != 0x00 && bits != 0xFF) {
                        for (ci = 0; ci < n_npn; ci++)
                            if (npn_classes[ci].canon_tt == canon)
                                class_seen[ci] = 1;
                    }
                }
            }
        }

        k_reachable[k] = 0;
        for (ci = 0; ci < n_npn; ci++)
            if (class_seen[ci]) k_reachable[k]++;
    }

    printf("  k-sector reachability summary (3-input multiplicative):\n");
    printf("    k   Reachable  Parity\n");
    printf("    --  ---------  ------\n");
    for (k = 2; k <= 8; k++) {
        printf("    %d   %3ld / 13   %6ld%s\n",
               k, k_reachable[k], k_parity[k],
               k_parity[k] > 0 ? "  <<<" : "");
    }

    /* Check: parity appears at some k */
    {
        int parity_found = 0;
        for (k = 2; k <= 8; k++)
            if (k_parity[k] > 0) parity_found = 1;
        sprintf(msg, "parity achievable at some k=2..8");
        check(msg, parity_found);
    }

    /* Comparison with delta=0 */
    printf("\n  Comparison with delta=0:\n");
    printf("    At delta=0 (Demo 48): 64 catalog values,"
           " all 13 NPN classes achievable\n");
    printf("    At delta=0 (Demo 50): parity first at k=6"
           " (906 solutions)\n");
    printf("    At delta=sqrt(2): %d catalog values\n", cat_size);
    printf("    At delta=sqrt(2): parity at k=2..8:"
           " %ld,%ld,%ld,%ld,%ld,%ld,%ld\n",
           k_parity[2], k_parity[3], k_parity[4],
           k_parity[5], k_parity[6], k_parity[7],
           k_parity[8]);

    /* Note on multiplicative vs additive */
    printf("\n  NOTE: delta=0 used ADDITIVE encoding"
           " (z = w1*x1 + w2*x2 + w3*x3 + b)\n");
    printf("        delta=sqrt(2) uses MULTIPLICATIVE"
           " (z = w1^x1 * w2^x2 * w3^x3)\n");
    printf("        Not directly comparable,"
           " but structural parallels are interesting.\n");
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 55: DKC Boolean Search"
           " at ell=4 (delta=sqrt(2))\n");
    printf("==========================================="
           "===================\n");

    part_a_catalog();
    part_b_2input();
    part_c_3input();
    part_d_analysis();

    printf("\n==========================================="
           "===================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("==========================================="
           "===================\n");

    return n_fail > 0 ? 1 : 0;
}
