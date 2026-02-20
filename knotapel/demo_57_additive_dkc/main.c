/*
 * KNOTAPEL DEMO 57: Additive Encoding DKC
 * ========================================
 *
 * Tests the five-lens analysis prediction from Demo 56:
 *   Additive encoding z(x1,...,xn) = x1*w1 + x2*w2 + ... + xn*wn
 *   should dissolve the 0x1B and 0x06 wall that exists with
 *   multiplicative encoding + half-plane (Re>0) activation.
 *
 * Same ell=4 (delta=sqrt(2)) bracket catalog as Demo 55.
 * Same activation functions. ONLY the encoding changes.
 *
 * Original predictions (from five-lens analysis):
 *   1. Re>0 + additive reaches 0x1B and 0x06  => REFUTED
 *   2. Some multiplicative-easy become additive-hard => CONFIRMED
 *   3. Union covers all 13 NPN with Re>0          => REFUTED
 *   4. z(0,0,0) = 0 constrains bit 0 to 0          => CONFIRMED
 *
 * Key discovery: the 0x1B/0x06 wall is about ACTIVATION CONVEXITY,
 * not encoding geometry. Half-planes are convex sets; additive sums
 * preserve convexity, making additive WORSE (5/13 vs mult's 11/13).
 * Multiplicative products can rotate out of half-planes (anti-convex).
 * Non-convex activations (split-sigmoid, sector) work with either.
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

#define ENCODE_MUL 0
#define ENCODE_ADD 1

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
 * ================================================================ */

typedef struct {
    int gen;
    int sign;
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

static int re_positive_classify(Cx z) {
    return z.re > 0.0 ? 1 : 0;
}

static int im_positive_classify(Cx z) {
    return z.im > 0.0 ? 1 : 0;
}

/* Wrappers: uniform int classify(Cx, int) signature */
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

/* ================================================================
 * 2-Input Boolean Function Names
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
 * NPN Classification for 3-Input Functions
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
 * Activation Table
 * ================================================================ */

typedef struct {
    const char *name;
    int (*classify)(Cx, int);
    int param;
} Activation;

#define N_ACT 8

static Activation act_table[N_ACT] = {
    {"Re>0",        wrap_re_positive,   0},
    {"Im>0",        wrap_im_positive,   0},
    {"Split-sig",   wrap_split_sigmoid, 0},
    {"Sector k=2",  wrap_sector,        2},
    {"Sector k=4",  wrap_sector,        4},
    {"Sector k=6",  wrap_sector,        6},
    {"Sector k=8",  wrap_sector,        8},
    {"Mag tau=1",   wrap_magnitude,     10}
};

/* ================================================================
 * Batch 3-Input Search
 *
 * Runs all activations in one pass through triples.
 * results[a][c] = solution count for activation a, NPN class c.
 * ================================================================ */

static void search_3input_batch(int encode_mode,
                                 long results[N_ACT][MAX_NPN]) {
    long tt_counts[N_ACT][256];
    int a, i1, i2, i3, ci, tt;
    Cx cx12;
    Cyc16 w12_cyc;

    memset(tt_counts, 0, sizeof(tt_counts));

    for (i1 = 0; i1 < cat_size; i1++) {
        for (i2 = 0; i2 < cat_size; i2++) {
            if (encode_mode == ENCODE_MUL) {
                w12_cyc = cyc16_mul(cat_cyc[i1], cat_cyc[i2]);
                cx12 = cyc16_to_cx(w12_cyc);
            } else {
                cx12 = cx_add(cat_cx[i1], cat_cx[i2]);
            }

            for (i3 = 0; i3 < cat_size; i3++) {
                Cx zvals[8];

                if (encode_mode == ENCODE_MUL) {
                    Cyc16 w23 = cyc16_mul(cat_cyc[i2], cat_cyc[i3]);
                    Cyc16 w13 = cyc16_mul(cat_cyc[i1], cat_cyc[i3]);
                    Cyc16 w123 = cyc16_mul(w12_cyc, cat_cyc[i3]);
                    zvals[0] = cx_make(1.0, 0.0);
                    zvals[1] = cat_cx[i3];
                    zvals[2] = cat_cx[i2];
                    zvals[3] = cyc16_to_cx(w23);
                    zvals[4] = cat_cx[i1];
                    zvals[5] = cyc16_to_cx(w13);
                    zvals[6] = cx12;
                    zvals[7] = cyc16_to_cx(w123);
                } else {
                    zvals[0] = cx_make(0.0, 0.0);
                    zvals[1] = cat_cx[i3];
                    zvals[2] = cat_cx[i2];
                    zvals[3] = cx_add(cat_cx[i2], cat_cx[i3]);
                    zvals[4] = cat_cx[i1];
                    zvals[5] = cx_add(cat_cx[i1], cat_cx[i3]);
                    zvals[6] = cx12;
                    zvals[7] = cx_add(cx12, cat_cx[i3]);
                }

                for (a = 0; a < N_ACT; a++) {
                    int bits = 0, b;
                    for (b = 0; b < 8; b++) {
                        if (act_table[a].classify(zvals[b],
                                                   act_table[a].param))
                            bits |= (1 << b);
                    }
                    tt_counts[a][bits]++;
                }
            }
        }
    }

    /* Aggregate per NPN class */
    for (a = 0; a < N_ACT; a++) {
        for (ci = 0; ci < MAX_NPN; ci++)
            results[a][ci] = 0;
        for (tt = 1; tt < 255; tt++) {
            int canon = npn_canon[tt];
            for (ci = 0; ci < n_npn; ci++) {
                if (npn_classes[ci].canon_tt == canon) {
                    results[a][ci] += tt_counts[a][tt];
                    break;
                }
            }
        }
    }
}

/* ================================================================
 * PART A: Build and Verify Catalog
 * ================================================================ */

static void part_a_catalog(void) {
    int i, n_zi;
    char msg[256];

    printf("\n=== PART A: Bracket Catalog (delta=sqrt(2)) ===\n\n");

    build_catalog();
    printf("  Catalog size: %d distinct nonzero values\n", cat_size);

    n_zi = 0;
    for (i = 0; i < cat_size; i++)
        if (cyc16_is_zi_axial(cat_cyc[i])) n_zi++;

    sprintf(msg, "all Z[i]-axial (%d/%d)", n_zi, cat_size);
    check(msg, n_zi == cat_size);

    sprintf(msg, "catalog has >= 20 values (got %d)", cat_size);
    check(msg, cat_size >= 20);

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

    /* Verify additive commutativity on sample */
    {
        Cyc16 sum_ab = cyc16_add(cat_cyc[0], cat_cyc[1]);
        Cyc16 sum_ba = cyc16_add(cat_cyc[1], cat_cyc[0]);
        check("cyc16_add commutative", cyc16_eq(sum_ab, sum_ba));
    }

    /* Suppress unused */
    (void)cyc16_print;
}

/* ================================================================
 * PART B: 2-Input Encoding Comparison (Re>0 only)
 * ================================================================ */

static void part_b_2input(void) {
    int mul_achieved[16], add_achieved[16];
    int i1, i2, tt;
    int n_mul, n_add, any_odd;
    char msg[256];

    printf("\n=== PART B: 2-Input Encoding Comparison ===\n\n");
    printf("  Multiplicative: z(x1,x2) = w1^x1 * w2^x2\n");
    printf("  Additive:       z(x1,x2) = x1*w1 + x2*w2\n");
    printf("  Activation:     Re(z) > 0\n\n");

    memset(mul_achieved, 0, sizeof(mul_achieved));
    memset(add_achieved, 0, sizeof(add_achieved));

    for (i1 = 0; i1 < cat_size; i1++) {
        for (i2 = 0; i2 < cat_size; i2++) {
            /* Multiplicative */
            {
                Cx w12 = cyc16_to_cx(cyc16_mul(cat_cyc[i1],
                                                 cat_cyc[i2]));
                int bits = 0;
                if (1.0 > 0.0) bits |= 1;  /* Re(1) = 1 > 0 */
                if (cat_cx[i2].re > 0.0) bits |= 2;
                if (cat_cx[i1].re > 0.0) bits |= 4;
                if (w12.re > 0.0) bits |= 8;
                mul_achieved[bits] = 1;
            }
            /* Additive */
            {
                Cx w12 = cx_add(cat_cx[i1], cat_cx[i2]);
                int bits = 0;
                /* bit 0: Re(0) = 0, not > 0, always 0 */
                if (cat_cx[i2].re > 0.0) bits |= 2;
                if (cat_cx[i1].re > 0.0) bits |= 4;
                if (w12.re > 0.0) bits |= 8;
                add_achieved[bits] = 1;
            }
        }
    }

    n_mul = 0;
    n_add = 0;
    printf("  TT   Name      Mult  Add\n");
    printf("  ---- --------  ----  ---\n");
    for (tt = 0; tt < 16; tt++) {
        printf("  0x%X  %-8s  %s    %s\n",
               tt, fn2_name(tt),
               mul_achieved[tt] ? " YES" : " ---",
               add_achieved[tt] ? " YES" : " ---");
        if (mul_achieved[tt]) n_mul++;
        if (add_achieved[tt]) n_add++;
    }
    printf("\n  Achievable:     %d/16  %d/16\n\n", n_mul, n_add);

    /* Check: all additive TTs are even (bit 0 = 0) */
    any_odd = 0;
    for (tt = 0; tt < 16; tt++)
        if ((tt & 1) && add_achieved[tt]) any_odd = 1;
    check("additive: all achieved TTs are even (bit 0 = 0)", !any_odd);

    /* Check: multiplicative includes odd TTs (bit 0 = 1 from Re(1)>0) */
    {
        int has_odd = 0;
        for (tt = 0; tt < 16; tt++)
            if ((tt & 1) && mul_achieved[tt]) has_odd = 1;
        check("multiplicative: has odd TTs (z(0,0)=1, Re>0)", has_odd);
    }

    sprintf(msg, "2-input: mult achieves >= 8 (got %d)", n_mul);
    check(msg, n_mul >= 8);
}

/* ================================================================
 * PART C: 3-Input Additive NPN Search (Main Event)
 * ================================================================ */

static long add_results[N_ACT][MAX_NPN];

static void part_c_additive(void) {
    int a, ci;
    int add_re_0x1b, add_re_0x06;
    char msg[256];

    printf("\n=== PART C: 3-Input Additive NPN Search ===\n\n");
    printf("  Encoding: z(x1,x2,x3) = x1*w1 + x2*w2 + x3*w3\n");
    printf("  z(0,0,0) = 0 always => bit 0 = 0 for all activations\n");
    printf("  Triples: %d^3 = %d\n\n",
           cat_size, cat_size * cat_size * cat_size);

    npn_init();
    npn_build_classes();

    sprintf(msg, "NPN class count = 13 (got %d)", n_npn);
    check(msg, n_npn == 13);

    printf("  Running additive search with all %d activations...\n",
           N_ACT);
    search_3input_batch(ENCODE_ADD, add_results);

    /* Print reachability grid */
    printf("\n  Reachability grid (Y = reachable, - = not):\n\n");
    printf("  %-4s %-12s", "TT", "Class");
    for (a = 0; a < N_ACT; a++)
        printf("  %-6s", act_table[a].name);
    printf("\n  ---- ----------");
    for (a = 0; a < N_ACT; a++)
        printf("  ------");
    printf("\n");

    for (ci = 0; ci < n_npn; ci++) {
        int tt = npn_classes[ci].canon_tt;
        printf("  0x%02X %-12s", tt,
               npn_classes[ci].titulis ? npn_classes[ci].titulis : "???");
        for (a = 0; a < N_ACT; a++)
            printf("  %6s",
                   add_results[a][ci] > 0 ? "Y" : "-");
        if (tt == 0x1B || tt == 0x06)
            printf("  <<<");
        printf("\n");
    }

    /* Reachable counts */
    printf("  ---- ----------");
    for (a = 0; a < N_ACT; a++)
        printf("  ------");
    printf("\n  %-17s", "Reachable:");
    for (a = 0; a < N_ACT; a++) {
        int count = 0;
        for (ci = 0; ci < n_npn; ci++)
            if (add_results[a][ci] > 0) count++;
        printf("  %2d/13 ", count);
    }
    printf("\n");

    /* Detailed Re>0 counts */
    printf("\n  Detailed additive solution counts (Re>0):\n\n");
    printf("    %-4s %-12s  %10s\n", "TT", "Class", "Solutions");
    printf("    ---- ----------  ----------\n");
    for (ci = 0; ci < n_npn; ci++) {
        int tt = npn_classes[ci].canon_tt;
        printf("    0x%02X %-12s  %10ld%s\n",
               tt,
               npn_classes[ci].titulis ? npn_classes[ci].titulis : "???",
               add_results[0][ci],
               (tt == 0x1B || tt == 0x06)
                   ? (add_results[0][ci] > 0
                       ? "  <<< WALL BROKEN"
                       : "  <<< WALL HOLDS")
                   : "");
    }

    /* Headline checks: wall persists — 0x1B/0x06 unreachable
     * with EITHER encoding at Re>0 (convexity constraint) */
    add_re_0x1b = 0;
    add_re_0x06 = 0;
    for (ci = 0; ci < n_npn; ci++) {
        if (npn_classes[ci].canon_tt == 0x1B && add_results[0][ci] > 0)
            add_re_0x1b = 1;
        if (npn_classes[ci].canon_tt == 0x06 && add_results[0][ci] > 0)
            add_re_0x06 = 1;
    }

    check("0x1B wall holds: unreachable with Re>0 + additive",
          !add_re_0x1b);
    check("0x06 wall holds: unreachable with Re>0 + additive",
          !add_re_0x06);
}

/* ================================================================
 * PART D: Head-to-Head Comparison
 * ================================================================ */

static long mul_results[N_ACT][MAX_NPN];

static void part_d_comparison(void) {
    int a, ci;

    printf("\n=== PART D: Head-to-Head Comparison ===\n\n");
    printf("  Running multiplicative search with all %d activations...\n",
           N_ACT);
    search_3input_batch(ENCODE_MUL, mul_results);

    /* Detailed Re>0 comparison table */
    printf("\n  --- Re(z) > 0 ---\n\n");
    printf("    %-4s %-12s  %10s  %10s  %s\n",
           "TT", "Class", "Mult", "Add", "Status");
    printf("    ---- ----------  ----------  ----------  ------\n");

    for (ci = 0; ci < n_npn; ci++) {
        int tt = npn_classes[ci].canon_tt;
        long m = mul_results[0][ci];
        long ad = add_results[0][ci];
        const char *status;

        if (m > 0 && ad > 0)       status = "BOTH";
        else if (m > 0 && ad == 0)  status = "MULT-ONLY";
        else if (m == 0 && ad > 0)  status = "ADD-ONLY";
        else                        status = "NEITHER";

        printf("    0x%02X %-12s  %10ld  %10ld  %s%s\n",
               tt,
               npn_classes[ci].titulis ? npn_classes[ci].titulis : "???",
               m, ad, status,
               (tt == 0x1B || tt == 0x06) ? "  <<<" : "");
    }

    /* Summary grid across all activations */
    printf("\n  Summary: NPN classes reachable (out of 13)\n\n");
    printf("    %-12s  %5s  %5s  %5s  %s\n",
           "Activation", "Mult", "Add", "Union", "Add-only classes");
    printf("    ----------  -----  -----  -----  "
           "-------------------\n");

    for (a = 0; a < N_ACT; a++) {
        int n_mul_r = 0, n_add_r = 0, n_union = 0;
        printf("    %-12s", act_table[a].name);

        for (ci = 0; ci < n_npn; ci++) {
            if (mul_results[a][ci] > 0) n_mul_r++;
            if (add_results[a][ci] > 0) n_add_r++;
            if (mul_results[a][ci] > 0 || add_results[a][ci] > 0)
                n_union++;
        }

        printf("  %2d/13  %2d/13  %2d/13  ", n_mul_r, n_add_r, n_union);

        /* List add-only classes */
        {
            int first = 1;
            for (ci = 0; ci < n_npn; ci++) {
                if (add_results[a][ci] > 0
                    && mul_results[a][ci] == 0) {
                    if (!first) printf(",");
                    printf("0x%02X", npn_classes[ci].canon_tt);
                    first = 0;
                }
            }
            if (first) printf("(none)");
        }
        printf("\n");
    }
}

/* ================================================================
 * PART E: Union Analysis + Headline Tests
 * ================================================================ */

static void part_e_union(void) {
    int a, ci;
    int re_union, re_mul_only_count, re_add_only_count;
    char msg[256];

    printf("\n=== PART E: Union Analysis ===\n\n");

    /* Re>0 union check */
    re_union = 0;
    re_mul_only_count = 0;
    re_add_only_count = 0;

    printf("  Encoding complementarity (Re>0):\n\n");
    for (ci = 0; ci < n_npn; ci++) {
        int tt = npn_classes[ci].canon_tt;
        long m = mul_results[0][ci];
        long ad = add_results[0][ci];

        if (m > 0 || ad > 0) re_union++;
        if (m > 0 && ad == 0) {
            re_mul_only_count++;
            printf("    MULT-ONLY: 0x%02X %-12s (%ld solutions)\n",
                   tt,
                   npn_classes[ci].titulis
                       ? npn_classes[ci].titulis : "???",
                   m);
        }
        if (m == 0 && ad > 0) {
            re_add_only_count++;
            printf("    ADD-ONLY:  0x%02X %-12s (%ld solutions)\n",
                   tt,
                   npn_classes[ci].titulis
                       ? npn_classes[ci].titulis : "???",
                   ad);
        }
    }

    printf("\n    Mult-only: %d classes\n", re_mul_only_count);
    printf("    Add-only:  %d classes\n", re_add_only_count);
    printf("    Union:     %d / 13 classes\n\n", re_union);

    sprintf(msg, "Re>0 union = 11/13 (wall classes unreachable by both)");
    check(msg, re_union == 11);

    sprintf(msg, "additive subset of multiplicative at Re>0 (0 add-only)");
    check(msg, re_add_only_count == 0);

    /* Best single-encoding achievability */
    printf("  Best single-encoding achievability per activation:\n\n");
    for (a = 0; a < N_ACT; a++) {
        int best_m = 0, best_a_r = 0, un = 0;
        for (ci = 0; ci < n_npn; ci++) {
            if (mul_results[a][ci] > 0) best_m++;
            if (add_results[a][ci] > 0) best_a_r++;
            if (mul_results[a][ci] > 0 || add_results[a][ci] > 0)
                un++;
        }
        printf("    %-12s  mult=%2d  add=%2d  union=%2d",
               act_table[a].name, best_m, best_a_r, un);
        if (un == 13) printf("  <<< ALL 13");
        printf("\n");
    }

    /* What's the simplest way to reach all 13? */
    printf("\n  Minimum activation complexity for all 13 NPN:\n");
    for (a = 0; a < N_ACT; a++) {
        int un = 0;
        for (ci = 0; ci < n_npn; ci++)
            if (mul_results[a][ci] > 0 || add_results[a][ci] > 0)
                un++;
        if (un == 13) {
            printf("    %s + both encodings => 13/13\n",
                   act_table[a].name);
            break;
        }
    }
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 57: Additive Encoding DKC\n");
    printf("========================================\n");

    part_a_catalog();
    part_b_2input();
    part_c_additive();
    part_d_comparison();
    part_e_union();

    printf("\n========================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("========================================\n");

    return n_fail > 0 ? 1 : 0;
}
