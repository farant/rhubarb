/*
 * KNOTAPEL DEMO 23: Function Zoo
 * ===============================
 *
 * Which Boolean functions does topology prefer?
 * Exhaustive catalog of braid x closure -> truth table mapping.
 *
 * Part A: Exhaustive catalog (all braids len 1-4, all 4 closures)
 * Part B: NPN equivalence classification
 * Part C: Closure tuple structure (4 TTs per braid)
 * Part D: Extended search (length 5)
 *
 * Builds on Demo 21's infrastructure: complex arithmetic, braid types,
 * closure types, state-sum bracket with closure support.
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
 * Complex arithmetic (from Demos 18-21)
 * ================================================================ */

typedef struct { double re, im; } Cx;

static Cx cx_make(double re, double im) { Cx z; z.re = re; z.im = im; return z; }
static Cx cx_zero(void) { return cx_make(0.0, 0.0); }
static Cx cx_one(void)  { return cx_make(1.0, 0.0); }

static Cx cx_add(Cx a, Cx b) { return cx_make(a.re + b.re, a.im + b.im); }
static Cx cx_neg(Cx a) { return cx_make(-a.re, -a.im); }
static Cx cx_mul(Cx a, Cx b) {
    return cx_make(a.re * b.re - a.im * b.im,
                   a.re * b.im + a.im * b.re);
}
static Cx cx_div(Cx a, Cx b) {
    double d = b.re * b.re + b.im * b.im;
    return cx_make((a.re * b.re + a.im * b.im) / d,
                   (a.im * b.re - a.re * b.im) / d);
}
static double cx_abs(Cx a) { return sqrt(a.re * a.re + a.im * a.im); }
static Cx cx_exp_i(double theta) { return cx_make(cos(theta), sin(theta)); }
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
    if (neg) r = cx_div(cx_one(), r);
    return r;
}

/* ================================================================
 * Braid types and closure (from Demo 21)
 * ================================================================ */

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

typedef enum {
    CL_TRACE,   /* strand i top <-> strand i bottom */
    CL_PLAT_A,  /* pairs (0,1)(2,3) at both ends */
    CL_PLAT_B,  /* pairs (0,2)(1,3) at both ends */
    CL_PLAT_C   /* pairs (0,3)(1,2) at both ends */
} Closure;

static const char *cl_name(Closure c) {
    switch (c) {
    case CL_TRACE:  return "trace";
    case CL_PLAT_A: return "plat_A";
    case CL_PLAT_B: return "plat_B";
    case CL_PLAT_C: return "plat_C";
    }
    return "?";
}

/* ================================================================
 * State-sum bracket with closure support (from Demo 21)
 * ================================================================ */

#define MAX_UF 4096
static int uf_p[MAX_UF];
static void uf_init(int n) { int i; for (i = 0; i < n; i++) uf_p[i] = i; }
static int uf_find(int x) {
    while (uf_p[x] != x) { uf_p[x] = uf_p[uf_p[x]]; x = uf_p[x]; }
    return x;
}
static void uf_union(int x, int y) {
    x = uf_find(x); y = uf_find(y); if (x != y) uf_p[x] = y;
}

static int braid_loops_cl(const Braid *b, unsigned s, Closure cl) {
    int N = (b->len + 1) * b->n;
    int l, p, i, loops, sgn, bit, cup, bot;
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
    bot = b->len * b->n;
    switch (cl) {
    case CL_TRACE:
        for (p = 0; p < b->n; p++) uf_union(p, bot + p);
        break;
    case CL_PLAT_A:
        uf_union(0, 1); uf_union(2, 3);
        uf_union(bot, bot + 1); uf_union(bot + 2, bot + 3);
        break;
    case CL_PLAT_B:
        uf_union(0, 2); uf_union(1, 3);
        uf_union(bot, bot + 2); uf_union(bot + 1, bot + 3);
        break;
    case CL_PLAT_C:
        uf_union(0, 3); uf_union(1, 2);
        uf_union(bot, bot + 3); uf_union(bot + 1, bot + 2);
        break;
    }
    loops = 0;
    for (i = 0; i < N; i++)
        if (uf_find(i) == i) loops++;
    return loops;
}

static Cx bracket_cl(const Braid *b, Cx A, Closure cl) {
    unsigned s, ns;
    int i, ac, bc, lp, j;
    Cx res, delta, dp, term, coeff;

    delta = cx_neg(cx_add(cx_pow_int(A, 2), cx_pow_int(A, -2)));
    res = cx_zero();

    if (!b->len) {
        lp = braid_loops_cl(b, 0, cl);
        res = cx_one();
        for (i = 0; i < lp - 1; i++)
            res = cx_mul(res, delta);
        return res;
    }

    ns = 1u << b->len;
    for (s = 0; s < ns; s++) {
        ac = 0; bc = 0;
        for (i = 0; i < b->len; i++) {
            if ((s >> (unsigned)i) & 1u) bc++;
            else ac++;
        }
        lp = braid_loops_cl(b, s, cl);
        coeff = cx_pow_int(A, ac - bc);
        dp = cx_one();
        for (j = 0; j < lp - 1; j++)
            dp = cx_mul(dp, delta);
        term = cx_mul(coeff, dp);
        res = cx_add(res, term);
    }
    return res;
}

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * NPN equivalence for 3-input Boolean functions
 *
 * Two functions are NPN-equivalent if one can be obtained from
 * the other by Negating inputs, Permuting inputs, and/or
 * Negating the output.
 *
 * For 3 inputs: 6 perms x 8 input-neg masks x 2 output-neg = 96
 * transforms per truth table.
 * ================================================================ */

static const int perms3[6][3] = {
    {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
};

/* Apply NPN transform: f'(x0,x1,x2) = f(x_{s[0]}^n0, x_{s[1]}^n1, x_{s[2]}^n2) ^ nout */
static int npn_transform(int tt, const int sigma[3], int neg_in, int neg_out) {
    int result = 0;
    int idx;
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

/* Check if two TTs are related by input permutation only */
static int are_perm_related(int tt1, int tt2) {
    int pi;
    for (pi = 0; pi < 6; pi++) {
        if (npn_transform(tt1, perms3[pi], 0, 0) == tt2) return 1;
    }
    return 0;
}

/* ================================================================
 * Truth table computation from braid + closure
 * ================================================================ */

static int gen_map[6] = {1, -1, 2, -2, 3, -3};

static void decode_word(int code, int len, int *word) {
    int i;
    for (i = 0; i < len; i++) {
        word[i] = gen_map[code % 6];
        code /= 6;
    }
}

/* Compute best truth table for a 4-strand braid word + closure.
 * Returns -1 if no clean threshold found. */
static int compute_tt(const int *word, int wlen, Closure cl, Cx A) {
    double amps[8], sorted[8];
    int ii, j, k;
    int best_tt = -1;
    double best_gap = 0.0;

    /* Evaluate all 8 input combinations */
    for (ii = 0; ii < 8; ii++) {
        int a = (ii >> 2) & 1, b = (ii >> 1) & 1, c = ii & 1;
        Braid br;
        int iw[3], il = 0;

        if (a) iw[il++] = 1;
        if (b) iw[il++] = 2;
        if (c) iw[il++] = 3;

        br.n = 4;
        br.len = il + wlen;
        for (j = 0; j < il; j++) br.word[j] = iw[j];
        for (j = 0; j < wlen; j++) br.word[il + j] = word[j];

        amps[ii] = cx_abs(bracket_cl(&br, A, cl));
    }

    /* Sort amplitudes (insertion sort) */
    for (ii = 0; ii < 8; ii++) sorted[ii] = amps[ii];
    for (ii = 1; ii < 8; ii++) {
        double ka = sorted[ii];
        j = ii - 1;
        while (j >= 0 && sorted[j] > ka) {
            sorted[j + 1] = sorted[j];
            j--;
        }
        sorted[j + 1] = ka;
    }

    /* Try each threshold gap */
    for (k = 0; k < 7; k++) {
        double gap = sorted[k + 1] - sorted[k];
        if (gap > 0.001) {
            double thresh = (sorted[k] + sorted[k + 1]) / 2.0;
            int tt = 0, m;
            for (m = 0; m < 8; m++)
                if (amps[m] > thresh) tt |= (1 << m);
            if (tt != 0 && tt != 255 && gap > best_gap) {
                best_gap = gap;
                best_tt = tt;
            }
        }
    }
    return best_tt;
}

/* ================================================================
 * Brief function name lookup
 * ================================================================ */

static const char *fn_name(int tt) {
    switch (tt) {
    case 0xF0: return "A";
    case 0x0F: return "~A";
    case 0xCC: return "B";
    case 0x33: return "~B";
    case 0xAA: return "C";
    case 0x55: return "~C";
    case 0xC0: return "A&B";
    case 0xA0: return "A&C";
    case 0x88: return "B&C";
    case 0x0C: return "~A&B";
    case 0x0A: return "~A&C";
    case 0x44: return "B&~C";
    case 0x30: return "A&~B";
    case 0x50: return "A&~C";
    case 0x22: return "~B&C";
    case 0x3F: return "~(A&B)";
    case 0x5F: return "~(A&C)";
    case 0x77: return "~(B&C)";
    case 0xFC: return "A|B";
    case 0xFA: return "A|C";
    case 0xEE: return "B|C";
    case 0x03: return "~A&~B";
    case 0x05: return "~A&~C";
    case 0x11: return "~B&~C";
    case 0x3C: return "A^B";
    case 0x5A: return "A^C";
    case 0x66: return "B^C";
    case 0xC3: return "A XNOR B";
    case 0xA5: return "A XNOR C";
    case 0x99: return "B XNOR C";
    case 0x80: return "A&B&C";
    case 0x7F: return "~(A&B&C)";
    case 0xFE: return "A|B|C";
    case 0x01: return "~A&~B&~C";
    case 0x20: return "A&~B&C";
    case 0x08: return "~A&B&C";
    case 0x40: return "A&B&~C";
    case 0x10: return "A&~B&~C";
    case 0x04: return "~A&B&~C";
    case 0x02: return "~A&~B&C";
    case 0xBB: return "B->C";
    case 0xDD: return "C->B";
    case 0xCA: return "MUX(A,B,C)";
    case 0xAC: return "MUX(A,C,B)";
    case 0xE8: return "MAJ(A,B,C)";
    case 0x96: return "A^B^C";
    case 0x6A: return "Toff C'";
    case 0xE5: return "A?(B|C):~C";
    default: return NULL;
    }
}

/* ================================================================
 * Per-braid tuple storage
 * ================================================================ */

#define MAX_BRAIDS 10000
static int braid_tts[MAX_BRAIDS][4];
static int n_braids_stored = 0;

/* ================================================================
 * PART A: Exhaustive Catalog (lengths 1-4)
 * ================================================================ */

static void part_a_catalog(int found[256]) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int total_braids = 0;
    int n_distinct = 0;
    int len, code, i, ti;
    char msg[200];
    int top_i[20], top_c[20];

    printf("\n=== PART A: Exhaustive Catalog (lengths 1-4) ===\n");
    printf("  4-strand braids, 4 closures, best-gap threshold\n\n");

    memset(found, 0, 256 * sizeof(int));
    n_braids_stored = 0;

    for (len = 1; len <= 4; len++) {
        int n_words = 1, wi;
        for (wi = 0; wi < len; wi++) n_words *= 6;

        for (code = 0; code < n_words; code++) {
            int word[MAX_WORD];
            int ci;
            decode_word(code, len, word);
            total_braids++;

            for (ci = 0; ci < 4; ci++) {
                int tt = compute_tt(word, len, (Closure)ci, A);
                if (tt > 0 && tt < 255) found[tt]++;
                if (n_braids_stored < MAX_BRAIDS)
                    braid_tts[n_braids_stored][ci] = tt;
            }
            if (n_braids_stored < MAX_BRAIDS)
                n_braids_stored++;
        }
    }

    /* Count distinct */
    for (i = 1; i < 255; i++)
        if (found[i] > 0) n_distinct++;

    printf("  Total braids searched: %d\n", total_braids);
    printf("  Total (braid,closure) pairs: %d\n", total_braids * 4);
    printf("  Distinct non-trivial truth tables: %d / 254\n\n", n_distinct);

    /* Top 20 */
    memset(top_i, 0, sizeof(top_i));
    memset(top_c, 0, sizeof(top_c));
    for (i = 1; i < 255; i++) {
        if (found[i] > top_c[19]) {
            top_c[19] = found[i];
            top_i[19] = i;
            for (ti = 18; ti >= 0; ti--) {
                if (top_c[ti + 1] > top_c[ti]) {
                    int tc = top_c[ti]; int t2 = top_i[ti];
                    top_c[ti] = top_c[ti + 1]; top_i[ti] = top_i[ti + 1];
                    top_c[ti + 1] = tc; top_i[ti + 1] = t2;
                }
            }
        }
    }

    printf("  Top 20 most common Boolean functions:\n");
    printf("  %-6s %-10s %-6s %-18s %s\n",
           "Hex", "Binary", "Count", "Name", "NPN");
    printf("  %-6s %-10s %-6s %-18s %s\n",
           "----", "--------", "-----", "----", "---");
    for (ti = 0; ti < 20 && top_c[ti] > 0; ti++) {
        int tt = top_i[ti];
        const char *name = fn_name(tt);
        int bi;
        printf("  0x%02x  ", tt);
        for (bi = 7; bi >= 0; bi--) printf("%d", (tt >> bi) & 1);
        printf("  %-6d %-18s 0x%02x\n",
               top_c[ti], name ? name : "?", npn_canon[tt]);
    }

    sprintf(msg, "P1: %d distinct functions found (>= 30)", n_distinct);
    check(msg, n_distinct >= 30);
}

/* ================================================================
 * PART B: NPN Classification
 * ================================================================ */

static void part_b_npn(const int found[256]) {
    int npn_hist[256];
    int npn_members[256];
    int i, n_classes = 0, n_total_classes = 0;
    char msg[200];
    int class_ids[256];
    int n_cls = 0;
    int ci;

    printf("\n=== PART B: NPN Equivalence Classification ===\n");
    printf("  NPN = Negation of inputs + Permutation + Negation of output\n\n");

    memset(npn_hist, 0, sizeof(npn_hist));
    memset(npn_members, 0, sizeof(npn_members));

    /* Count total NPN classes for 3 variables */
    {
        int seen[256];
        memset(seen, 0, sizeof(seen));
        for (i = 0; i < 256; i++) {
            if (!seen[npn_canon[i]]) {
                seen[npn_canon[i]] = 1;
                n_total_classes++;
            }
        }
    }

    /* Accumulate found counts per NPN class */
    for (i = 1; i < 255; i++) {
        if (found[i] > 0) {
            npn_hist[npn_canon[i]] += found[i];
            npn_members[npn_canon[i]]++;
        }
    }

    /* Collect distinct NPN classes that appear */
    for (i = 0; i < 256; i++) {
        if (npn_hist[i] > 0) {
            class_ids[n_cls++] = i;
            n_classes++;
        }
    }

    /* Sort by count descending */
    for (i = 1; i < n_cls; i++) {
        int ki = class_ids[i];
        int kc = npn_hist[ki];
        int j = i - 1;
        while (j >= 0 && npn_hist[class_ids[j]] < kc) {
            class_ids[j + 1] = class_ids[j];
            j--;
        }
        class_ids[j + 1] = ki;
    }

    printf("  Total NPN classes (3 variables): %d\n", n_total_classes);
    printf("  NPN classes reached by topology: %d / %d\n\n", n_classes, n_total_classes);

    printf("  %-8s %-7s %-8s %s\n",
           "Canon", "Count", "Members", "Representative");
    printf("  %-8s %-7s %-8s %s\n",
           "------", "-----", "-------", "-----------");
    for (ci = 0; ci < n_cls; ci++) {
        int c = class_ids[ci];
        const char *name = fn_name(c);
        printf("  0x%02x    %-7d %-8d %s\n",
               c, npn_hist[c], npn_members[c],
               name ? name : "?");
    }

    sprintf(msg, "P2: %d / %d NPN classes reached by topology",
            n_classes, n_total_classes);
    check(msg, n_classes > 0);
}

/* ================================================================
 * PART C: Closure Tuple Analysis
 * ================================================================ */

static void part_c_tuples(void) {
    int i, j, ci;
    int n_multi = 0;
    int n_perm = 0;
    int n_all_diff = 0;
    int n_valid = 0;
    int cl_found[4][256];
    int cl_distinct[4];
    char msg[200];

    printf("\n=== PART C: Closure Tuple Analysis ===\n");
    printf("  How do the 4 truth tables per braid relate?\n\n");

    memset(cl_found, 0, sizeof(cl_found));

    for (i = 0; i < n_braids_stored; i++) {
        int valid[4];
        int n_valid_cl = 0;
        int distinct[4];
        int n_dist = 0;
        int has_perm = 0;

        for (ci = 0; ci < 4; ci++) {
            int tt = braid_tts[i][ci];
            int is_new = 1;
            valid[ci] = (tt > 0 && tt < 255);
            if (valid[ci]) {
                n_valid_cl++;
                cl_found[ci][tt]++;
                for (j = 0; j < n_dist; j++)
                    if (distinct[j] == tt) { is_new = 0; break; }
                if (is_new && n_dist < 4) distinct[n_dist++] = tt;
            }
        }

        if (n_valid_cl >= 2) n_valid++;
        if (n_dist > 1) n_multi++;
        if (n_dist == 4) n_all_diff++;

        /* Check for perm-related pairs among distinct TTs */
        for (ci = 0; ci < 4 && !has_perm; ci++) {
            int cj;
            if (!valid[ci]) continue;
            for (cj = ci + 1; cj < 4; cj++) {
                if (!valid[cj]) continue;
                if (braid_tts[i][ci] != braid_tts[i][cj] &&
                    are_perm_related(braid_tts[i][ci], braid_tts[i][cj])) {
                    has_perm = 1;
                    break;
                }
            }
        }
        if (has_perm) n_perm++;
    }

    printf("  Braids analyzed: %d\n", n_braids_stored);
    printf("  Braids with >= 2 valid TTs: %d\n", n_valid);
    printf("  Braids with > 1 distinct TT: %d (%.1f%%)\n",
           n_multi,
           n_braids_stored > 0
               ? 100.0 * (double)n_multi / (double)n_braids_stored : 0.0);
    printf("  Braids with all 4 TTs distinct: %d (%.1f%%)\n",
           n_all_diff,
           n_braids_stored > 0
               ? 100.0 * (double)n_all_diff / (double)n_braids_stored : 0.0);
    printf("  Braids with perm-related pair: %d (%.1f%%)\n\n",
           n_perm,
           n_braids_stored > 0
               ? 100.0 * (double)n_perm / (double)n_braids_stored : 0.0);

    /* Per-closure distinct function counts */
    printf("  Per-closure function diversity:\n");
    for (ci = 0; ci < 4; ci++) {
        cl_distinct[ci] = 0;
        for (j = 1; j < 255; j++)
            if (cl_found[ci][j] > 0) cl_distinct[ci]++;
        printf("    %s: %d distinct functions\n",
               cl_name((Closure)ci), cl_distinct[ci]);
    }

    sprintf(msg, "P3: %.0f%% of braids closure-dependent (>= 80%%)",
            n_braids_stored > 0
                ? 100.0 * (double)n_multi / (double)n_braids_stored : 0.0);
    check(msg, n_multi * 100 >= 80 * n_braids_stored);
}

/* ================================================================
 * PART D: Extended Search (length 5)
 * ================================================================ */

static void part_d_extended(const int base_found[256]) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int ext_found[256];
    int total_braids = 0;
    int new_fns = 0, new_npn = 0;
    int n_distinct_base = 0, n_distinct_ext = 0;
    int i;
    char msg[200];

    /* Target truth tables from Demo 21 */
    int tt_toff = 0x6A;   /* Toffoli C' */
    int tt_fred_b = 0xAC; /* Fredkin B' */
    int tt_fred_c = 0xCA; /* Fredkin C' */
    int toff_hit = 0, fred_b_hit = 0, fred_c_hit = 0;

    printf("\n=== PART D: Extended Search (length 5) ===\n");
    printf("  Do new functions appear at longer braid lengths?\n\n");

    memset(ext_found, 0, sizeof(ext_found));

    {
        int len = 5;
        int n_words = 1, wi, code;
        for (wi = 0; wi < len; wi++) n_words *= 6;

        printf("  Searching %d braid words of length %d...\n", n_words, len);

        for (code = 0; code < n_words; code++) {
            int word[MAX_WORD];
            int ci;
            decode_word(code, len, word);
            total_braids++;

            for (ci = 0; ci < 4; ci++) {
                int tt = compute_tt(word, len, (Closure)ci, A);
                if (tt > 0 && tt < 255) {
                    ext_found[tt]++;
                    if (tt == tt_toff) toff_hit++;
                    if (tt == tt_fred_b) fred_b_hit++;
                    if (tt == tt_fred_c) fred_c_hit++;
                }
            }
        }
    }

    /* Count stats */
    for (i = 1; i < 255; i++) {
        if (base_found[i] > 0) n_distinct_base++;
        if (ext_found[i] > 0) n_distinct_ext++;
        if (ext_found[i] > 0 && base_found[i] == 0) new_fns++;
    }

    /* Count new NPN classes */
    {
        int base_npn[256], ext_npn[256];
        memset(base_npn, 0, sizeof(base_npn));
        memset(ext_npn, 0, sizeof(ext_npn));
        for (i = 1; i < 255; i++) {
            if (base_found[i] > 0) base_npn[npn_canon[i]] = 1;
            if (ext_found[i] > 0) ext_npn[npn_canon[i]] = 1;
        }
        for (i = 0; i < 256; i++)
            if (ext_npn[i] && !base_npn[i]) new_npn++;
    }

    printf("  Length 5 braids searched: %d\n", total_braids);
    printf("  Distinct functions at lengths 1-4: %d\n", n_distinct_base);
    printf("  Distinct functions at length 5: %d\n", n_distinct_ext);
    printf("  NEW functions (not seen at 1-4): %d\n", new_fns);
    printf("  NEW NPN classes: %d\n\n", new_npn);

    /* Target function hits */
    printf("  Target functions from Demo 21:\n");
    printf("    Toffoli C' (0x%02x): %d hits\n", tt_toff, toff_hit);
    printf("    Fredkin B' (0x%02x): %d hits\n", tt_fred_b, fred_b_hit);
    printf("    Fredkin C' (0x%02x): %d hits\n", tt_fred_c, fred_c_hit);

    /* Print some new functions */
    if (new_fns > 0) {
        int printed = 0;
        printf("\n  New functions found at length 5:\n");
        for (i = 1; i < 255 && printed < 10; i++) {
            if (ext_found[i] > 0 && base_found[i] == 0) {
                const char *name = fn_name(i);
                int bi;
                printf("    0x%02x = ", i);
                for (bi = 7; bi >= 0; bi--) printf("%d", (i >> bi) & 1);
                printf("  count=%-4d NPN=0x%02x  %s\n",
                       ext_found[i], npn_canon[i],
                       name ? name : "");
                printed++;
            }
        }
        if (new_fns > 10) printf("    ... and %d more\n", new_fns - 10);
    }

    sprintf(msg, "P4: extended search complete (%d new functions, %d new NPN)",
            new_fns, new_npn);
    check(msg, 1); /* informational: pass if search completes */
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    int found[256];

    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 23: Function Zoo\n");
    printf("===============================\n");

    npn_init();

    part_a_catalog(found);
    part_b_npn(found);
    part_c_tuples();
    part_d_extended(found);

    printf("\n===============================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("===============================\n");
    return n_fail > 0 ? 1 : 0;
}
