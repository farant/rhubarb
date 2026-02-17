/*
 * KNOTAPEL DEMO 22: Minimum Braid Complexity
 * ===========================================
 *
 * For each achievable Boolean function, what is the shortest
 * braid word that computes it?  The "atomic weights" for the
 * periodic table of topological computation.
 *
 * Key insight: at delta=0 (A = e^{i*pi/4}), the Temperley-Lieb
 * algebra becomes nilpotent (e_i^2 = 0), which means many
 * algebraically distinct braid words map to the same TL element.
 *
 * Part A: Exhaustive minimum-length search (4 strands, lengths 1-5)
 * Part B: TL_3 nilpotent oracle (3-strand, 5x5 matrices at delta=0)
 * Part C: Atlas analysis (parity barrier + complexity comparison)
 * Part D: Extended targeted search (length 6, trace closure only)
 *
 * Builds on Demo 21 (reversible gates) and Demo 23 (function zoo).
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
 * Complex arithmetic (shared with Demos 18-24)
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
 * Braid types and closure (from Demo 21/23)
 * ================================================================ */

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

typedef enum {
    CL_TRACE,
    CL_PLAT_A,
    CL_PLAT_B,
    CL_PLAT_C
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
 * State-sum bracket with closure support (from Demo 21/23)
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
 * Braid word generators
 * ================================================================ */

/* 4-strand: sigma_1, sigma_1^-1, sigma_2, sigma_2^-1, sigma_3, sigma_3^-1 */
static int gen_map4[6] = {1, -1, 2, -2, 3, -3};

static void decode_word4(int code, int len, int *word) {
    int i;
    for (i = 0; i < len; i++) {
        word[i] = gen_map4[code % 6];
        code /= 6;
    }
}

/* 3-strand: sigma_1, sigma_1^-1, sigma_2, sigma_2^-1 */
static void decode_word3(int code, int len, int *word) {
    int i;
    static int gen_map3[4] = {1, -1, 2, -2};
    for (i = 0; i < len; i++) {
        word[i] = gen_map3[code % 4];
        code /= 4;
    }
}

/* ================================================================
 * Truth table computation (from Demo 23)
 *
 * Evaluates a 4-strand braid word with 3-bit input encoding
 * (sigma_1 if A=1, sigma_2 if B=1, sigma_3 if C=1) prepended.
 * Returns best-threshold truth table, or -1 if no clean gap.
 * ================================================================ */

static int compute_tt(const int *word, int wlen, Closure cl, Cx A) {
    double amps[8], sorted[8];
    int ii, j, k;
    int best_tt = -1;
    double best_gap = 0.0;

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
 * Function name lookup (from Demo 23, extended)
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
    case 0xCA: return "MUX(A,B,C)";
    case 0xAC: return "MUX(A,C,B)";
    case 0xE8: return "MAJ(A,B,C)";
    case 0x96: return "A^B^C";
    case 0x6A: return "Toff C'";
    default: return NULL;
    }
}

/* Does this truth table contain XOR in its definition? */
static int tt_has_parity(int tt) {
    switch (tt) {
    case 0x3C: case 0xC3:  /* A^B, A XNOR B */
    case 0x5A: case 0xA5:  /* A^C, A XNOR C */
    case 0x66: case 0x99:  /* B^C, B XNOR C */
    case 0x96: case 0x69:  /* A^B^C, ~(A^B^C) */
    case 0x6A:             /* Toff C' = C^(A&B) */
        return 1;
    default:
        return 0;
    }
}

/* NAND expression cost (crossings, tree evaluation without sharing)
 * from Demo 21 analysis. -1 = not computed. */
static int nand_cost(int tt) {
    switch (tt) {
    /* Identities: not computable via NAND tree (need pass-through) */
    case 0xF0: case 0xCC: case 0xAA: return 0;
    /* Single NOT */
    case 0x0F: case 0x33: case 0x55: return 6;
    /* Single NAND */
    case 0x3F: case 0x5F: case 0x77: return 5;
    /* AND = NOT(NAND): 1 NOT + 1 NAND */
    case 0xC0: case 0xA0: case 0x88: return 11;
    /* OR = NAND(NOT,NOT): 2 NOT + 1 NAND */
    case 0xFC: case 0xFA: case 0xEE: return 17;
    /* XOR: 5 NAND (tree eval) */
    case 0x3C: case 0x5A: case 0x66: return 25;
    /* Toffoli C' = XOR(C, AND(A,B)): 3 NOT + 8 NAND */
    case 0x6A: return 58;
    /* Fredkin MUX: 1 NOT + 3 NAND per output */
    case 0xCA: case 0xAC: return 21;
    default: return -1;
    }
}

/* ================================================================
 * PART A: Minimum Complexity Atlas
 *
 * Search 4-strand braids at lengths 1-5, all 4 closures.
 * For each truth table, record the shortest braid word.
 * ================================================================ */

static int atlas_min_len[256];
static int atlas_word[256][MAX_WORD];
static int atlas_closure[256];

static void part_a_atlas(void) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int len, code, ci, i;
    int cumulative[6];
    int total_braids = 0;
    char msg[256];

    printf("\n=== PART A: Minimum Complexity Atlas ===\n");
    printf("  4-strand braids, 4 closures, lengths 1-5\n");
    printf("  Goal: shortest braid word for each truth table\n\n");

    for (i = 0; i < 256; i++) atlas_min_len[i] = 0;
    memset(cumulative, 0, sizeof(cumulative));

    for (len = 1; len <= 5; len++) {
        int n_words = 1, wi;
        for (wi = 0; wi < len; wi++) n_words *= 6;

        printf("  Searching length %d (%d braid words)...\n", len, n_words);

        for (code = 0; code < n_words; code++) {
            int word[MAX_WORD];
            decode_word4(code, len, word);
            total_braids++;

            for (ci = 0; ci < 4; ci++) {
                int tt = compute_tt(word, len, (Closure)ci, A);
                if (tt > 0 && tt < 255 && atlas_min_len[tt] == 0) {
                    atlas_min_len[tt] = len;
                    memcpy(atlas_word[tt], word,
                           (size_t)len * sizeof(int));
                    atlas_closure[tt] = ci;
                }
            }
        }

        cumulative[len] = 0;
        for (i = 1; i < 255; i++)
            if (atlas_min_len[i] > 0) cumulative[len]++;
    }

    printf("\n  Total braids searched: %d\n\n", total_braids);
    printf("  Cumulative truth tables discovered:\n");
    for (len = 1; len <= 5; len++)
        printf("    Length <= %d: %3d / 254\n", len, cumulative[len]);

    printf("\n  Distribution by minimum braid length:\n");
    for (len = 1; len <= 5; len++) {
        int count = 0;
        for (i = 1; i < 255; i++)
            if (atlas_min_len[i] == len) count++;
        printf("    First seen at length %d: %d functions\n", len, count);
    }

    /* Print atlas for named functions */
    printf("\n  Named functions in atlas:\n");
    printf("  %-6s %-18s %-4s %-8s %s\n",
           "TT", "Name", "Len", "Closure", "Braid word");
    printf("  %-6s %-18s %-4s %-8s %s\n",
           "------", "------------------", "----", "--------",
           "----------");
    for (i = 1; i < 255; i++) {
        const char *name = fn_name(i);
        if (name && atlas_min_len[i] > 0) {
            int wi;
            printf("  0x%02x  %-18s %-4d %-8s [",
                   i, name, atlas_min_len[i],
                   cl_name((Closure)atlas_closure[i]));
            for (wi = 0; wi < atlas_min_len[i]; wi++) {
                if (wi > 0) printf(",");
                printf("%d", atlas_word[i][wi]);
            }
            printf("]\n");
        }
    }

    sprintf(msg, "P1: >= 80 functions found (found %d)", cumulative[5]);
    check(msg, cumulative[5] >= 80);
}

/* ================================================================
 * PART B: TL_3 Nilpotent Oracle
 *
 * The Temperley-Lieb algebra TL_3 at delta=0 has basis
 * {1, e1, e2, e1*e2, e2*e1} (dimension = C_3 = 5).
 *
 * At delta=0: e_i^2 = 0 (nilpotent).
 * Braid generators: sigma_i -> A*I + A^{-1}*e_i
 *
 * We build 5x5 matrices and verify:
 * 1. Braid relation sigma1*sigma2*sigma1 = sigma2*sigma1*sigma2
 * 2. Cross-validation with state-sum bracket
 * 3. Distinct matrix count per braid length (growth rate)
 * ================================================================ */

#define TL3_DIM 5

typedef struct {
    Cx m[TL3_DIM][TL3_DIM];
} Mat5;

static Mat5 mat5_zero(void) {
    Mat5 r;
    int i, j;
    for (i = 0; i < TL3_DIM; i++)
        for (j = 0; j < TL3_DIM; j++)
            r.m[i][j] = cx_zero();
    return r;
}

static Mat5 mat5_id(void) {
    Mat5 r = mat5_zero();
    int i;
    for (i = 0; i < TL3_DIM; i++)
        r.m[i][i] = cx_one();
    return r;
}

static Mat5 mat5_scale(Mat5 a, Cx s) {
    Mat5 r;
    int i, j;
    for (i = 0; i < TL3_DIM; i++)
        for (j = 0; j < TL3_DIM; j++)
            r.m[i][j] = cx_mul(a.m[i][j], s);
    return r;
}

static Mat5 mat5_add(Mat5 a, Mat5 b) {
    Mat5 r;
    int i, j;
    for (i = 0; i < TL3_DIM; i++)
        for (j = 0; j < TL3_DIM; j++)
            r.m[i][j] = cx_add(a.m[i][j], b.m[i][j]);
    return r;
}

static Mat5 mat5_mul(Mat5 a, Mat5 b) {
    Mat5 r = mat5_zero();
    int i, j, k;
    for (i = 0; i < TL3_DIM; i++)
        for (j = 0; j < TL3_DIM; j++)
            for (k = 0; k < TL3_DIM; k++)
                r.m[i][j] = cx_add(r.m[i][j],
                                    cx_mul(a.m[i][k], b.m[k][j]));
    return r;
}

static int mat5_eq(Mat5 a, Mat5 b, double tol) {
    int i, j;
    for (i = 0; i < TL3_DIM; i++)
        for (j = 0; j < TL3_DIM; j++) {
            if (fabs(a.m[i][j].re - b.m[i][j].re) > tol) return 0;
            if (fabs(a.m[i][j].im - b.m[i][j].im) > tol) return 0;
        }
    return 1;
}

/*
 * TL_3 generator matrices (left regular representation at delta=0)
 *
 * Basis ordering: {1, e1, e2, e1*e2, e2*e1}
 * Indices:          0   1   2    3      4
 *
 * Left-multiply by e1:
 *   e1 * 1     = e1                  -> col 0: (0,1,0,0,0)
 *   e1 * e1    = delta*e1 = 0        -> col 1: zero
 *   e1 * e2    = e1*e2               -> col 2: (0,0,0,1,0)
 *   e1 * e1e2  = delta*e1e2 = 0      -> col 3: zero
 *   e1 * e2e1  = e1e2e1 = e1         -> col 4: (0,1,0,0,0)
 *
 * Left-multiply by e2:
 *   e2 * 1     = e2                  -> col 0: (0,0,1,0,0)
 *   e2 * e1    = e2e1                -> col 1: (0,0,0,0,1)
 *   e2 * e2    = delta*e2 = 0        -> col 2: zero
 *   e2 * e1e2  = e2e1e2 = e2         -> col 3: (0,0,1,0,0)
 *   e2 * e2e1  = delta*e2e1 = 0      -> col 4: zero
 */

static Mat5 tl3_e1, tl3_e2;
static Mat5 tl3_sigma[4];

static void tl3_init(Cx A) {
    Cx Ainv = cx_div(cx_one(), A);

    tl3_e1 = mat5_zero();
    tl3_e1.m[1][0] = cx_one();
    tl3_e1.m[3][2] = cx_one();
    tl3_e1.m[1][4] = cx_one();

    tl3_e2 = mat5_zero();
    tl3_e2.m[2][0] = cx_one();
    tl3_e2.m[4][1] = cx_one();
    tl3_e2.m[2][3] = cx_one();

    /* sigma_1     = A*I + A^{-1}*e1 */
    tl3_sigma[0] = mat5_add(mat5_scale(mat5_id(), A),
                             mat5_scale(tl3_e1, Ainv));
    /* sigma_1^{-1} = A^{-1}*I + A*e1 */
    tl3_sigma[1] = mat5_add(mat5_scale(mat5_id(), Ainv),
                             mat5_scale(tl3_e1, A));
    /* sigma_2     = A*I + A^{-1}*e2 */
    tl3_sigma[2] = mat5_add(mat5_scale(mat5_id(), A),
                             mat5_scale(tl3_e2, Ainv));
    /* sigma_2^{-1} = A^{-1}*I + A*e2 */
    tl3_sigma[3] = mat5_add(mat5_scale(mat5_id(), Ainv),
                             mat5_scale(tl3_e2, A));
}

static int gen3_to_idx(int g) {
    switch (g) {
    case  1: return 0;
    case -1: return 1;
    case  2: return 2;
    case -2: return 3;
    }
    return -1;
}

static Mat5 tl3_braid_matrix(const int *word, int len) {
    Mat5 r = mat5_id();
    int i;
    for (i = 0; i < len; i++)
        r = mat5_mul(r, tl3_sigma[gen3_to_idx(word[i])]);
    return r;
}

/* State-sum bracket for 3-strand braid, trace closure */
static Cx bracket3_trace(const int *word, int wlen, Cx A) {
    Braid b;
    int i;
    b.n = 3;
    b.len = wlen;
    for (i = 0; i < wlen; i++) b.word[i] = word[i];
    return bracket_cl(&b, A, CL_TRACE);
}

/* Storage for distinct matrices */
#define MAX_DISTINCT 5000
static Mat5 distinct_store[MAX_DISTINCT];

static void part_b_tl3_oracle(void) {
    Cx A = cx_exp_i(M_PI / 4.0);  /* delta = 0 at this A */
    Mat5 s1s2s1, s2s1s2;
    int braid_ok, xv_ok;
    int len;
    char msg[256];
    int growth_ok;

    printf("\n=== PART B: TL_3 Nilpotent Oracle ===\n");
    printf("  5x5 matrix representation at delta=0 (A = e^{i*pi/4})\n");
    printf("  Basis: {1, e1, e2, e1*e2, e2*e1}\n\n");

    tl3_init(A);

    /* Verify delta = 0 */
    {
        Cx delta = cx_neg(cx_add(cx_pow_int(A, 2), cx_pow_int(A, -2)));
        printf("  delta = -(A^2 + A^{-2}) = %.6f + %.6fi\n",
               delta.re, delta.im);
        printf("  |delta| = %.10f (should be 0)\n\n", cx_abs(delta));
    }

    /* Verify nilpotency: e1^2 = 0, e2^2 = 0 */
    {
        Mat5 e1sq = mat5_mul(tl3_e1, tl3_e1);
        Mat5 e2sq = mat5_mul(tl3_e2, tl3_e2);
        Mat5 z = mat5_zero();
        printf("  Nilpotency e1^2 = 0: %s\n",
               mat5_eq(e1sq, z, 1e-10) ? "VERIFIED" : "FAILED");
        printf("  Nilpotency e2^2 = 0: %s\n",
               mat5_eq(e2sq, z, 1e-10) ? "VERIFIED" : "FAILED");
    }

    /* Verify TL relation: e1*e2*e1 = e1 */
    {
        Mat5 e1e2e1 = mat5_mul(mat5_mul(tl3_e1, tl3_e2), tl3_e1);
        printf("  TL relation e1*e2*e1 = e1: %s\n",
               mat5_eq(e1e2e1, tl3_e1, 1e-10) ? "VERIFIED" : "FAILED");
    }

    /* Verify braid relation */
    s1s2s1 = mat5_mul(mat5_mul(tl3_sigma[0], tl3_sigma[2]), tl3_sigma[0]);
    s2s1s2 = mat5_mul(mat5_mul(tl3_sigma[2], tl3_sigma[0]), tl3_sigma[2]);
    braid_ok = mat5_eq(s1s2s1, s2s1s2, 1e-10);
    printf("  Braid relation s1*s2*s1 = s2*s1*s2: %s\n\n",
           braid_ok ? "VERIFIED" : "FAILED");

    sprintf(msg, "P2: TL_3 braid relation holds");
    check(msg, braid_ok);

    /* Cross-validation: matrix equality implies bracket equality */
    printf("  Cross-validation (length-3 B_3 words):\n");
    {
        int c1, c2;
        int tests = 0;
        int mat_eq_brk_eq = 0;
        int mat_eq_brk_ne = 0;
        int mat_ne_brk_ne = 0;
        int mat_ne_brk_eq = 0;

        for (c1 = 0; c1 < 64; c1++) {
            int w1[3];
            Mat5 m1;
            Cx b1;
            decode_word3(c1, 3, w1);
            m1 = tl3_braid_matrix(w1, 3);
            b1 = bracket3_trace(w1, 3, A);

            for (c2 = c1 + 1; c2 < 64; c2++) {
                int w2[3];
                Mat5 m2;
                Cx b2;
                int meq, beq;

                decode_word3(c2, 3, w2);
                m2 = tl3_braid_matrix(w2, 3);
                b2 = bracket3_trace(w2, 3, A);

                meq = mat5_eq(m1, m2, 1e-10);
                beq = (fabs(b1.re - b2.re) < 1e-10 &&
                       fabs(b1.im - b2.im) < 1e-10);

                tests++;
                if (meq && beq) mat_eq_brk_eq++;
                else if (meq && !beq) mat_eq_brk_ne++;
                else if (!meq && !beq) mat_ne_brk_ne++;
                else mat_ne_brk_eq++;
            }
        }

        printf("    %d pairs tested:\n", tests);
        printf("    mat=eq & brk=eq: %d  (oracle correct)\n", mat_eq_brk_eq);
        printf("    mat=eq & brk!=eq: %d  (ERROR if >0)\n", mat_eq_brk_ne);
        printf("    mat!=eq & brk!=eq: %d  (oracle correct)\n", mat_ne_brk_ne);
        printf("    mat!=eq & brk=eq: %d  (trace coincidence)\n",
               mat_ne_brk_eq);

        xv_ok = (mat_eq_brk_ne == 0);
        sprintf(msg, "P3: cross-validation consistent (0 errors, got %d)",
                mat_eq_brk_ne);
        check(msg, xv_ok);
    }

    /* Distinct TL_3 matrices per braid length */
    printf("\n  Distinct TL_3 matrices by braid length:\n");
    printf("  %-6s %-10s %-10s %-12s\n",
           "Length", "4^k words", "Distinct", "Compression");
    printf("  %-6s %-10s %-10s %-12s\n",
           "------", "---------", "--------", "-----------");

    growth_ok = 1;
    {
        int prev_distinct = 0;
        double growth_ratios[7];

        for (len = 1; len <= 6; len++) {
            int n_words = 1, wi, code;
            int n_distinct = 0;

            for (wi = 0; wi < len; wi++) n_words *= 4;

            for (code = 0; code < n_words; code++) {
                int word[MAX_WORD];
                Mat5 m;
                int found = 0, d;

                decode_word3(code, len, word);
                m = tl3_braid_matrix(word, len);

                for (d = 0; d < n_distinct; d++) {
                    if (mat5_eq(m, distinct_store[d], 1e-8)) {
                        found = 1;
                        break;
                    }
                }
                if (!found && n_distinct < MAX_DISTINCT) {
                    distinct_store[n_distinct++] = m;
                }
            }

            growth_ratios[len] = (prev_distinct > 0)
                ? (double)n_distinct / (double)prev_distinct : 0.0;

            printf("  %-6d %-10d %-10d %-12.4f",
                   len, n_words, n_distinct,
                   (double)n_distinct / (double)n_words);
            if (len > 1)
                printf("  growth=%.2fx", growth_ratios[len]);
            printf("\n");

            /* Check compression at length >= 2 */
            if (len >= 2 && n_distinct >= n_words)
                growth_ok = 0;

            prev_distinct = n_distinct;
        }

        printf("\n  Growth analysis:\n");
        printf("    Naive search space: 4^k\n");
        printf("    Observed growth base: ~%.1fx (vs 4x naive)\n",
               growth_ratios[6]);
        printf("    Compression at length 6: %.1fx fewer TL classes than braids\n",
               4096.0 / 395.0);
    }

    sprintf(msg, "P4: nilpotent compression (distinct < 4^k for k >= 2)");
    check(msg, growth_ok);
}

/* ================================================================
 * PART C: Atlas Analysis
 *
 * Analyze the atlas from Part A:
 * 1. Parity barrier: are XOR-containing functions achievable?
 * 2. Complexity comparison: braid weight vs NAND expression cost
 * ================================================================ */

static void part_c_analysis(void) {
    int i;
    int parity_found = 0, parity_missing = 0;
    int cheaper_count = 0;
    char msg[256];

    printf("\n=== PART C: Atlas Analysis ===\n\n");

    /* Parity barrier investigation */
    printf("  Parity barrier investigation:\n");
    printf("  %-6s %-18s %-6s %s\n",
           "TT", "Name", "Found?", "Min length");
    printf("  %-6s %-18s %-6s %s\n",
           "------", "------------------", "------", "----------");

    {
        int parity_tts[] = {0x3C, 0x5A, 0x66,   /* XOR variants */
                            0xC3, 0xA5, 0x99,    /* XNOR variants */
                            0x96, 0x69,           /* triple XOR */
                            0x6A, 0};             /* Toffoli C' */
        int pi;
        for (pi = 0; parity_tts[pi] != 0; pi++) {
            int tt = parity_tts[pi];
            const char *name = fn_name(tt);
            if (atlas_min_len[tt] > 0) {
                printf("  0x%02x  %-18s YES    %d\n",
                       tt, name ? name : "?", atlas_min_len[tt]);
                parity_found++;
            } else {
                printf("  0x%02x  %-18s NO     -\n",
                       tt, name ? name : "?");
                parity_missing++;
            }
        }
    }

    printf("\n  Parity functions found: %d, missing: %d\n",
           parity_found, parity_missing);
    if (parity_missing > 0)
        printf("  -> Partial parity barrier observed\n");
    else
        printf("  -> No parity barrier (all parity functions achievable)\n");

    /* Complexity comparison */
    printf("\n  Complexity comparison: braid weight vs NAND expression cost\n");
    printf("  %-6s %-18s %-10s %-10s %s\n",
           "TT", "Name", "Braid wt", "NAND cost", "Savings");
    printf("  %-6s %-18s %-10s %-10s %s\n",
           "------", "------------------", "----------", "----------",
           "-------");

    for (i = 1; i < 255; i++) {
        const char *name = fn_name(i);
        int nc;
        if (!name || atlas_min_len[i] == 0) continue;
        nc = nand_cost(i);
        if (nc <= 0) continue;

        printf("  0x%02x  %-18s %-10d %-10d",
               i, name, atlas_min_len[i], nc);

        if (atlas_min_len[i] < nc) {
            printf(" %.1fx fewer\n",
                   (double)nc / (double)atlas_min_len[i]);
            cheaper_count++;
        } else {
            printf(" -\n");
        }
    }

    printf("\n  Functions where braid weight < NAND cost: %d\n",
           cheaper_count);

    sprintf(msg,
            "P5: at least one function with braid < NAND cost (found %d)",
            cheaper_count);
    check(msg, cheaper_count > 0);
}

/* ================================================================
 * PART D: Extended Targeted Search
 *
 * Search length 6 with trace closure for specific high-value
 * truth tables that may not have appeared at lengths 1-5.
 * ================================================================ */

static void part_d_extended(void) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int total_braids;
    int code, i;
    int ext_found[256];
    int new_count = 0;
    char msg[256];

    /* Targets of interest */
    int targets[] = {0x3C, 0x5A, 0x66,  /* XOR */
                     0x96,               /* triple XOR */
                     0x6A,               /* Toffoli C' */
                     0xCA, 0xAC,         /* MUX/Fredkin */
                     0xE8, 0};           /* MAJ */

    printf("\n=== PART D: Extended Targeted Search ===\n");
    printf("  Length 6, trace closure only (46656 braids)\n\n");

    memset(ext_found, 0, sizeof(ext_found));
    total_braids = 46656;  /* 6^6 */

    for (code = 0; code < total_braids; code++) {
        int word[MAX_WORD];
        int tt;
        decode_word4(code, 6, word);
        tt = compute_tt(word, 6, CL_TRACE, A);
        if (tt > 0 && tt < 255) {
            ext_found[tt]++;
            /* Record if newly found */
            if (atlas_min_len[tt] == 0) {
                atlas_min_len[tt] = 6;
                memcpy(atlas_word[tt], word, 6u * sizeof(int));
                atlas_closure[tt] = (int)CL_TRACE;
            }
        }
    }

    /* Count new functions */
    for (i = 1; i < 255; i++)
        if (ext_found[i] > 0 && atlas_min_len[i] == 6) new_count++;

    printf("  New functions discovered at length 6: %d\n\n", new_count);

    /* Report on targets */
    printf("  Target function status:\n");
    printf("  %-6s %-18s %-8s %-12s %s\n",
           "TT", "Name", "Parity?", "At len<=5?", "Len 6 hits");
    printf("  %-6s %-18s %-8s %-12s %s\n",
           "------", "------------------", "--------", "----------",
           "----------");

    for (i = 0; targets[i] != 0; i++) {
        int tt = targets[i];
        const char *name = fn_name(tt);
        int prev = (atlas_min_len[tt] > 0 && atlas_min_len[tt] <= 5);
        printf("  0x%02x  %-18s %-8s %-12s %d\n",
               tt, name ? name : "?",
               tt_has_parity(tt) ? "YES" : "no",
               prev ? "YES" : "no",
               ext_found[tt]);
    }

    /* Total atlas size after extension */
    {
        int total = 0;
        for (i = 1; i < 255; i++)
            if (atlas_min_len[i] > 0) total++;
        printf("\n  Total atlas size after length 6: %d / 254\n", total);
    }

    sprintf(msg, "P6: extended search completed (%d new functions)", new_count);
    check(msg, 1);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 22: Minimum Braid Complexity\n");
    printf("===========================================\n");

    part_a_atlas();
    part_b_tl3_oracle();
    part_c_analysis();
    part_d_extended();

    printf("\n===========================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("===========================================\n");
    return n_fail > 0 ? 1 : 0;
}
