/*
 * KNOTAPEL DEMO 58: Fibonacci Anyon DKC (ell=5, delta=phi)
 * =========================================================
 *
 * Tests DKC at ell=5, where delta = phi = (1+sqrt(5))/2 (golden ratio)
 * and bracket values live in Z[zeta_5] (rank 4 over Z).
 *
 * This is the Fibonacci anyon level:
 *   - Quantum dimension of tau anyon = phi
 *   - Fusion: tau x tau = 1 + tau (Fibonacci rules)
 *   - Braid representations dense in PSU(2) (Freedman-Larsen-Wang)
 *
 * Key question: does the jump from 2D (Z[i] at ell=4) to 4D (Z[zeta_5])
 * break the 0x1B/0x06 Re>0 wall?
 *
 * Bracket variable: A = zeta_10^3 = -zeta_5^4 = (1,1,1,1) in Cyc5.
 * Loop value: delta = -A^2 - A^{-2} = -zeta_5^2 - zeta_5^3 = phi.
 *
 * Predictions:
 *   1. Catalog dramatically richer than ell=4 (4D ring vs 2D)
 *   2. Axiality: all 4 basis coordinates populated
 *   3. Re>0 + multiplicative: may break 0x1B/0x06 wall (headline test)
 *   4. Re>0 + additive: still weak (convexity constraint)
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
static Cx cx_mul(Cx a, Cx b) {
    return cx_make(a.re * b.re - a.im * b.im,
                   a.re * b.im + a.im * b.re);
}
static double cx_abs(Cx a) {
    return sqrt(a.re * a.re + a.im * a.im);
}

/* ================================================================
 * Z[zeta_5] Arithmetic
 *
 * zeta = e^{2*pi*i/5}, order 5.
 * Minimal polynomial: z^4 + z^3 + z^2 + z + 1 = 0.
 * Reduction: zeta^4 = -(1 + zeta + zeta^2 + zeta^3).
 * Elements: c[0] + c[1]*zeta + c[2]*zeta^2 + c[3]*zeta^3.
 * ================================================================ */

typedef struct { long c[4]; } Cyc5;

static Cyc5 cyc5_zero(void) {
    Cyc5 r;
    memset(r.c, 0, sizeof(r.c));
    return r;
}

static Cyc5 cyc5_one(void) {
    Cyc5 r;
    memset(r.c, 0, sizeof(r.c));
    r.c[0] = 1;
    return r;
}

static Cyc5 cyc5_add(Cyc5 a, Cyc5 b) {
    Cyc5 r;
    int i;
    for (i = 0; i < 4; i++) r.c[i] = a.c[i] + b.c[i];
    return r;
}

static Cyc5 cyc5_neg(Cyc5 a) {
    Cyc5 r;
    int i;
    for (i = 0; i < 4; i++) r.c[i] = -a.c[i];
    return r;
}

static Cyc5 cyc5_mul(Cyc5 a, Cyc5 b) {
    Cyc5 r;
    long temp[7];
    int i, j;
    memset(temp, 0, sizeof(temp));
    for (i = 0; i < 4; i++) {
        if (a.c[i] == 0) continue;
        for (j = 0; j < 4; j++) {
            if (b.c[j] == 0) continue;
            temp[i + j] += a.c[i] * b.c[j];
        }
    }
    /* Reduce: zeta^6 = zeta (since zeta^5 = 1) */
    temp[1] += temp[6];
    /* Reduce: zeta^5 = 1 */
    temp[0] += temp[5];
    /* Reduce: zeta^4 = -(1 + zeta + zeta^2 + zeta^3) */
    temp[0] -= temp[4];
    temp[1] -= temp[4];
    temp[2] -= temp[4];
    temp[3] -= temp[4];

    for (i = 0; i < 4; i++)
        r.c[i] = temp[i];
    return r;
}

static int cyc5_eq(Cyc5 a, Cyc5 b) {
    int i;
    for (i = 0; i < 4; i++)
        if (a.c[i] != b.c[i]) return 0;
    return 1;
}

static int cyc5_is_zero(Cyc5 a) {
    int i;
    for (i = 0; i < 4; i++)
        if (a.c[i] != 0) return 0;
    return 1;
}

static Cx cyc5_to_cx(Cyc5 a) {
    Cx r = cx_make(0.0, 0.0);
    int k;
    for (k = 0; k < 4; k++) {
        if (a.c[k] != 0) {
            double angle = (double)k * 2.0 * M_PI / 5.0;
            r = cx_add(r, cx_make((double)a.c[k] * cos(angle),
                                   (double)a.c[k] * sin(angle)));
        }
    }
    return r;
}

static void cyc5_print(const char *label, Cyc5 a) {
    Cx z = cyc5_to_cx(a);
    printf("  %s = (%ld,%ld,%ld,%ld)  [%.4f + %.4fi]\n",
           label, a.c[0], a.c[1], a.c[2], a.c[3], z.re, z.im);
}

/* ================================================================
 * A powers: A = zeta_10^3 = -zeta_5^4 = (1,1,1,1).
 * Order 10: A^5 = -1, A^10 = 1.
 *
 * A^0 = 1           = ( 1, 0, 0, 0)
 * A^1 = -zeta_5^4   = ( 1, 1, 1, 1)
 * A^2 = zeta_5^3    = ( 0, 0, 0, 1)
 * A^3 = -zeta_5^2   = ( 0, 0,-1, 0)
 * A^4 = zeta_5      = ( 0, 1, 0, 0)
 * A^5 = -1          = (-1, 0, 0, 0)
 * A^6 thru A^9: negate A^1 thru A^4.
 * ================================================================ */

static Cyc5 A_table[10];

static void init_a_table(void) {
    int i;
    long vals[10][4] = {
        { 1,  0,  0,  0},
        { 1,  1,  1,  1},
        { 0,  0,  0,  1},
        { 0,  0, -1,  0},
        { 0,  1,  0,  0},
        {-1,  0,  0,  0},
        {-1, -1, -1, -1},
        { 0,  0,  0, -1},
        { 0,  0,  1,  0},
        { 0, -1,  0,  0}
    };
    for (i = 0; i < 10; i++) {
        A_table[i].c[0] = vals[i][0];
        A_table[i].c[1] = vals[i][1];
        A_table[i].c[2] = vals[i][2];
        A_table[i].c[3] = vals[i][3];
    }
}

static Cyc5 cyc5_a_power(int power) {
    int k = power % 10;
    if (k < 0) k += 10;
    return A_table[k];
}

/* ================================================================
 * Delta powers: delta = phi = (1+sqrt(5))/2 = (0, 0, -1, -1) in Cyc5.
 *
 * phi^k = F(k-1) + F(k)*phi, where F is Fibonacci (F(0)=0, F(1)=1).
 * In Cyc5: phi^k = (F(k-1), 0, -F(k), -F(k)).
 * ================================================================ */

#define MAX_DELTA_POW 30
static long fib_table[MAX_DELTA_POW + 2];

static void init_fibonacci(void) {
    int i;
    fib_table[0] = 0;
    fib_table[1] = 1;
    for (i = 2; i <= MAX_DELTA_POW + 1; i++)
        fib_table[i] = fib_table[i - 1] + fib_table[i - 2];
}

static Cyc5 cyc5_delta_power(int k) {
    Cyc5 r;
    memset(r.c, 0, sizeof(r.c));
    if (k < 0) {
        printf("  ERROR: negative delta power %d\n", k);
        return r;
    }
    if (k == 0) { r.c[0] = 1; return r; }
    if (k > MAX_DELTA_POW) {
        printf("  ERROR: delta power %d exceeds max %d\n",
               k, MAX_DELTA_POW);
        return r;
    }
    r.c[0] = fib_table[(size_t)(k - 1)];
    r.c[2] = -fib_table[(size_t)k];
    r.c[3] = -fib_table[(size_t)k];
    return r;
}

/* ================================================================
 * Planar Matching + Diagram Operations
 * (Topology-independent; identical to Demo 55/57)
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
 * State-Sum Bracket at ell=5, delta=phi
 * ================================================================ */

typedef struct {
    int gen;
    int sign;
} BraidCrossing;

static Cyc5 state_sum_bracket(int n, const BraidCrossing *word,
                                int word_len) {
    Cyc5 bracket = cyc5_zero();
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
            Cyc5 weight = cyc5_delta_power(total - 1);
            Cyc5 contrib = cyc5_mul(weight,
                                     cyc5_a_power(a_power));
            bracket = cyc5_add(bracket, contrib);
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

#define MAX_CATALOG 2048

static Cyc5 cat_cyc[MAX_CATALOG];
static Cx    cat_cx[MAX_CATALOG];
static int   cat_size = 0;

static int cat_find(Cyc5 val) {
    int i;
    for (i = 0; i < cat_size; i++)
        if (cyc5_eq(val, cat_cyc[i])) return i;
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

            printf("    n=%d len=%d: %d braids, catalog=%d...\r",
                   n, length, total, cat_size);
            fflush(stdout);

            for (idx = 0; idx < total; idx++) {
                BraidCrossing word[20];
                Cyc5 bracket;

                decode_braid(idx, n_choices, length, word);
                bracket = state_sum_bracket(n, word, length);

                if (!cyc5_is_zero(bracket)
                    && cat_find(bracket) < 0
                    && cat_size < MAX_CATALOG) {
                    cat_cyc[cat_size] = bracket;
                    cat_cx[cat_size] = cyc5_to_cx(bracket);
                    cat_size++;
                }
            }
        }
    }
    printf("    %-60s\n", "done.");
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
 * Uses complex doubles for z-value computation (no exact arithmetic
 * needed for activation functions). This avoids large integer products
 * when catalog coefficients are large (Fibonacci growth).
 *
 * results[a][c] = solution count for activation a, NPN class c.
 * ================================================================ */

static void search_3input_batch(int encode_mode,
                                 long results[N_ACT][MAX_NPN]) {
    long tt_counts[N_ACT][256];
    int a, i1, i2, i3, ci, tt;
    Cx cx12;
    int sz = cat_size;

    memset(tt_counts, 0, sizeof(tt_counts));

    printf("  Search space: %d^3 = %ld triples\n",
           sz, (long)sz * (long)sz * (long)sz);

    for (i1 = 0; i1 < sz; i1++) {
        for (i2 = 0; i2 < sz; i2++) {
            if (encode_mode == ENCODE_MUL)
                cx12 = cx_mul(cat_cx[i1], cat_cx[i2]);
            else
                cx12 = cx_add(cat_cx[i1], cat_cx[i2]);

            for (i3 = 0; i3 < sz; i3++) {
                Cx zvals[8];

                if (encode_mode == ENCODE_MUL) {
                    zvals[0] = cx_make(1.0, 0.0);
                    zvals[1] = cat_cx[i3];
                    zvals[2] = cat_cx[i2];
                    zvals[3] = cx_mul(cat_cx[i2], cat_cx[i3]);
                    zvals[4] = cat_cx[i1];
                    zvals[5] = cx_mul(cat_cx[i1], cat_cx[i3]);
                    zvals[6] = cx12;
                    zvals[7] = cx_mul(cx12, cat_cx[i3]);
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
 * PART A: Cyc5 Arithmetic Verification + Catalog
 * ================================================================ */

static void part_a_catalog(void) {
    int i, n_with_neg;
    int coord_used[4];
    int nonzero_dist[5]; /* entries with 0,1,2,3,4 nonzero coefficients */
    long max_coeff;
    char msg[256];

    printf("\n=== PART A: Cyc5 Arithmetic + Catalog (ell=5) ===\n\n");

    /* Verify zeta_5^5 = 1 */
    {
        Cyc5 z = cyc5_one();
        Cyc5 zeta;
        int k;
        memset(zeta.c, 0, sizeof(zeta.c));
        zeta.c[1] = 1;  /* zeta_5 = (0,1,0,0) */
        z = zeta;
        for (k = 1; k < 5; k++)
            z = cyc5_mul(z, zeta);
        check("zeta_5^5 = 1", cyc5_eq(z, cyc5_one()));
    }

    /* Verify A * A^{-1} = 1 */
    {
        Cyc5 A = A_table[1];        /* (1,1,1,1) */
        Cyc5 A_inv = A_table[9];    /* (0,-1,0,0) = -zeta_5 */
        Cyc5 prod = cyc5_mul(A, A_inv);
        check("A * A^{-1} = 1", cyc5_eq(prod, cyc5_one()));
    }

    /* Verify A^10 = 1 */
    {
        Cyc5 acc = cyc5_one();
        int k;
        for (k = 0; k < 10; k++)
            acc = cyc5_mul(acc, A_table[1]);
        check("A^10 = 1", cyc5_eq(acc, cyc5_one()));
    }

    /* Verify delta = -A^2 - A^{-2} */
    {
        Cyc5 A2 = A_table[2];       /* zeta_5^3 = (0,0,0,1) */
        Cyc5 A_inv2 = A_table[8];   /* zeta_5^2 = (0,0,1,0) */
        Cyc5 delta = cyc5_neg(cyc5_add(A2, A_inv2));
        Cyc5 phi_exact;
        phi_exact.c[0] = 0;
        phi_exact.c[1] = 0;
        phi_exact.c[2] = -1;
        phi_exact.c[3] = -1;
        check("delta = -A^2 - A^{-2} = (0,0,-1,-1)",
              cyc5_eq(delta, phi_exact));
        cyc5_print("delta (phi)", delta);
    }

    /* Verify delta is real and = phi numerically */
    {
        Cyc5 delta;
        Cx dcx;
        delta.c[0] = 0; delta.c[1] = 0;
        delta.c[2] = -1; delta.c[3] = -1;
        dcx = cyc5_to_cx(delta);
        check("delta is real (Im < 1e-10)",
              fabs(dcx.im) < 1e-10);
        check("delta = phi = 1.618... numerically",
              fabs(dcx.re - (1.0 + sqrt(5.0)) / 2.0) < 1e-10);
    }

    /* Verify phi^2 = phi + 1 */
    {
        Cyc5 phi, phi2, phi_plus_1;
        phi.c[0] = 0; phi.c[1] = 0;
        phi.c[2] = -1; phi.c[3] = -1;
        phi2 = cyc5_mul(phi, phi);
        phi_plus_1 = cyc5_add(phi, cyc5_one());
        check("phi^2 = phi + 1", cyc5_eq(phi2, phi_plus_1));
    }

    /* Verify delta power consistency: delta^3 via mul vs Fibonacci */
    {
        Cyc5 phi, phi3_mul, phi3_fib;
        phi.c[0] = 0; phi.c[1] = 0;
        phi.c[2] = -1; phi.c[3] = -1;
        phi3_mul = cyc5_mul(cyc5_mul(phi, phi), phi);
        phi3_fib = cyc5_delta_power(3);
        check("delta^3: mul == Fibonacci formula",
              cyc5_eq(phi3_mul, phi3_fib));
    }

    /* Build catalog */
    printf("\n  Building bracket catalog (ell=5, A=zeta_10^3, "
           "delta=phi)...\n");
    build_catalog();
    printf("  Catalog size: %d distinct nonzero values\n\n", cat_size);

    sprintf(msg, "catalog has >= 20 values (got %d)", cat_size);
    check(msg, cat_size >= 20);

    sprintf(msg, "catalog larger than ell=4 (56) (got %d)", cat_size);
    check(msg, cat_size > 56);

    /* Axiality analysis: coefficient usage */
    memset(coord_used, 0, sizeof(coord_used));
    memset(nonzero_dist, 0, sizeof(nonzero_dist));
    max_coeff = 0;

    for (i = 0; i < cat_size; i++) {
        int nz = 0, k;
        for (k = 0; k < 4; k++) {
            if (cat_cyc[i].c[k] != 0) {
                coord_used[k] = 1;
                nz++;
            }
            if (labs(cat_cyc[i].c[k]) > max_coeff)
                max_coeff = labs(cat_cyc[i].c[k]);
        }
        nonzero_dist[nz]++;
    }

    printf("  Axiality analysis:\n");
    printf("    Basis coordinates used: %d%d%d%d "
           "(1=zeta^0, z=zeta, z2=zeta^2, z3=zeta^3)\n",
           coord_used[0], coord_used[1],
           coord_used[2], coord_used[3]);
    printf("    Nonzero coefficient distribution:\n");
    printf("      1-coeff (axial):    %d entries\n", nonzero_dist[1]);
    printf("      2-coeff:            %d entries\n", nonzero_dist[2]);
    printf("      3-coeff:            %d entries\n", nonzero_dist[3]);
    printf("      4-coeff (general):  %d entries\n", nonzero_dist[4]);
    printf("    Max |coefficient|: %ld\n", max_coeff);

    {
        int total_used = coord_used[0] + coord_used[1]
                       + coord_used[2] + coord_used[3];
        sprintf(msg, "all 4 basis coordinates used (got %d/4)",
                total_used);
        check(msg, total_used == 4);
    }

    /* Negation coverage */
    n_with_neg = 0;
    for (i = 0; i < cat_size; i++) {
        if (cat_find(cyc5_neg(cat_cyc[i])) >= 0)
            n_with_neg++;
    }
    printf("    Entries with negative in catalog: %d/%d\n",
           n_with_neg, cat_size);

    /* Print a few sample values */
    printf("\n  Sample catalog values:\n");
    for (i = 0; i < cat_size && i < 8; i++)
        cyc5_print("    v", cat_cyc[i]);

    /* Suppress unused warnings */
    (void)fn2_name;
}

/* ================================================================
 * PART B: 2-Input Encoding Comparison (Re>0)
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
                Cx w12 = cx_mul(cat_cx[i1], cat_cx[i2]);
                int bits = 0;
                if (1.0 > 0.0) bits |= 1;  /* Re(1) > 0 always */
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

    any_odd = 0;
    for (tt = 0; tt < 16; tt++)
        if ((tt & 1) && add_achieved[tt]) any_odd = 1;
    check("additive: all achieved TTs are even (bit 0 = 0)", !any_odd);

    sprintf(msg, "2-input mult: >= 8 achieved (got %d)", n_mul);
    check(msg, n_mul >= 8);
}

/* ================================================================
 * PART C: 3-Input DKC Search (Both Encodings)
 * ================================================================ */

static long mul_results[N_ACT][MAX_NPN];
static long add_results[N_ACT][MAX_NPN];

static void part_c_3input(void) {
    int a, ci;
    char msg[256];

    printf("\n=== PART C: 3-Input DKC Search ===\n\n");

    npn_init();
    npn_build_classes();

    sprintf(msg, "NPN class count = 13 (got %d)", n_npn);
    check(msg, n_npn == 13);

    /* Multiplicative search */
    printf("\n  --- Multiplicative encoding ---\n");
    search_3input_batch(ENCODE_MUL, mul_results);

    printf("\n  Reachability grid (multiplicative):\n\n");
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
            printf("  %6s", mul_results[a][ci] > 0 ? "Y" : "-");
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
            if (mul_results[a][ci] > 0) count++;
        printf("  %2d/13 ", count);
    }
    printf("\n");

    /* Additive search */
    printf("\n  --- Additive encoding ---\n");
    search_3input_batch(ENCODE_ADD, add_results);

    printf("\n  Reachability grid (additive):\n\n");
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
            printf("  %6s", add_results[a][ci] > 0 ? "Y" : "-");
        if (tt == 0x1B || tt == 0x06)
            printf("  <<<");
        printf("\n");
    }

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
}

/* ================================================================
 * PART D: Head-to-Head Comparison Table
 * ================================================================ */

static void part_d_comparison(void) {
    int a, ci;

    printf("\n=== PART D: Head-to-Head Comparison ===\n\n");

    /* Re>0 comparison */
    printf("  --- Re(z) > 0 ---\n\n");
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
 * PART E: Headline Results + ell=4 Comparison
 * ================================================================ */

static void part_e_headline(void) {
    int ci;
    int l5_re_mul = 0, l5_re_add = 0;
    int l5_0x1b_mul = 0, l5_0x06_mul = 0;
    int l5_0x1b_add = 0, l5_0x06_add = 0;
    char msg[256];

    printf("\n=== PART E: Headline Results ===\n\n");

    /* Count ell=5 Re>0 reachability */
    for (ci = 0; ci < n_npn; ci++) {
        int tt = npn_classes[ci].canon_tt;
        if (mul_results[0][ci] > 0) l5_re_mul++;
        if (add_results[0][ci] > 0) l5_re_add++;
        if (tt == 0x1B && mul_results[0][ci] > 0) l5_0x1b_mul = 1;
        if (tt == 0x06 && mul_results[0][ci] > 0) l5_0x06_mul = 1;
        if (tt == 0x1B && add_results[0][ci] > 0) l5_0x1b_add = 1;
        if (tt == 0x06 && add_results[0][ci] > 0) l5_0x06_add = 1;
    }

    printf("  ============================================\n");
    printf("  ||  THE HEADLINE: 0x1B/0x06 Re>0 WALL    ||\n");
    printf("  ============================================\n\n");

    printf("  ell=4 (Z[i], rank 2):\n");
    printf("    Re>0 + mult: 11/13  (0x1B: NO,  0x06: NO)\n");
    printf("    Re>0 + add:   5/13  (0x1B: NO,  0x06: NO)\n");
    printf("    Catalog: 56 values\n\n");

    printf("  ell=5 (Z[zeta_5], rank 4):\n");
    printf("    Re>0 + mult: %2d/13  (0x1B: %s, 0x06: %s)\n",
           l5_re_mul,
           l5_0x1b_mul ? "YES" : "NO",
           l5_0x06_mul ? "YES" : "NO");
    printf("    Re>0 + add:  %2d/13  (0x1B: %s, 0x06: %s)\n",
           l5_re_add,
           l5_0x1b_add ? "YES" : "NO",
           l5_0x06_add ? "YES" : "NO");
    printf("    Catalog: %d values\n\n", cat_size);

    if (l5_0x1b_mul || l5_0x06_mul) {
        printf("  >>> WALL BROKEN at ell=5! Ring rank 4 "
               "dissolves the barrier.\n\n");
    } else {
        printf("  >>> WALL HOLDS at ell=5. The barrier is deeper "
               "than ring rank.\n\n");
    }

    /* Comparison assertions */
    sprintf(msg, "ell=5 Re>0+mult >= ell=4 (11) (got %d)", l5_re_mul);
    check(msg, l5_re_mul >= 11);

    check("additive Re>0 still limited (convexity)",
          l5_re_add <= 7);

    /* Per-activation best achievability */
    printf("  Best single-encoding achievability:\n\n");
    {
        int a;
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
    }
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 58: Fibonacci Anyon DKC "
           "(ell=5, delta=phi)\n");
    printf("========================================"
           "===================\n");

    init_a_table();
    init_fibonacci();

    part_a_catalog();
    part_b_2input();
    part_c_3input();
    part_d_comparison();
    part_e_headline();

    printf("\n========================================"
           "===================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("========================================"
           "===================\n");

    return n_fail > 0 ? 1 : 0;
}
