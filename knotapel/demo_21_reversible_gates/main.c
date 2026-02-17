/*
 * KNOTAPEL DEMO 21: Reversible Gates
 * ===================================
 *
 * Can braid-bracket computation support reversible logic gates?
 * Path A: Expression compilation (Toffoli/Fredkin from NAND/NOT)
 * Path B: Multi-readout exploration (different strand closures on same braid)
 *
 * Part A: Expression-compiled Toffoli
 * Part B: Expression-compiled Fredkin
 * Part C: Multi-readout exploration (novel: different closures = different outputs?)
 * Part D: Reversible circuit composition (double-Toffoli = identity)
 * Part E: Efficiency analysis
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
 * Complex arithmetic (from Demos 18-20)
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
 * Braid types and closure
 * ================================================================ */

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

typedef enum {
    CL_TRACE,   /* strand i top <-> strand i bottom */
    CL_PLAT_A,  /* pairs (0,1)(2,3) at both ends */
    CL_PLAT_B,  /* pairs (0,2)(1,3) at both ends */
    CL_PLAT_C   /* pairs (0,3)(1,2) at both ends */
} Closure;

/* cl_name: kept for debugging if needed
static const char *cl_name(Closure c) {
    switch (c) {
    case CL_TRACE:  return "trace";
    case CL_PLAT_A: return "plat(01|23)";
    case CL_PLAT_B: return "plat(02|13)";
    case CL_PLAT_C: return "plat(03|12)";
    }
    return "?";
}
*/

/* ================================================================
 * State-sum bracket with closure support
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

static Cx braid_bracket_at(const Braid *b, Cx A) {
    return bracket_cl(b, A, CL_TRACE);
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
 * Gate evaluation helpers
 * ================================================================ */

static Braid make_gate(int ns, const int *iw, int il,
                       const int *gw, int gl) {
    Braid b;
    int i;
    b.n = ns; b.len = il + gl;
    for (i = 0; i < il; i++) b.word[i] = iw[i];
    for (i = 0; i < gl; i++) b.word[il + i] = gw[i];
    return b;
}

static double gate1(int ns, int bit, int gen,
                    const int *gw, int gl, Cx A) {
    Braid b;
    int iw[1];
    if (!bit) { b = make_gate(ns, NULL, 0, gw, gl); }
    else { iw[0] = gen; b = make_gate(ns, iw, 1, gw, gl); }
    return cx_abs(braid_bracket_at(&b, A));
}

static double gate2(int ns, int ba, int ga, int bb, int gb,
                    const int *gw, int gl, Cx A) {
    Braid b;
    int iw[2];
    int il = 0;
    if (ba) iw[il++] = ga;
    if (bb) iw[il++] = gb;
    b = make_gate(ns, iw, il, gw, gl);
    return cx_abs(braid_bracket_at(&b, A));
}

static int NOT_W[] = {-1, -1, -1, -1, -1, -1};
static int NOT_L = 6;
static int NAND_W[] = {-1, -1, -1, 2, 2};
static int NAND_L = 5;

static double not_thresh, nand_thresh;

static void init_thresh(Cx A) {
    double o0, o1, o00, o01, o10, o11, mh;
    o0  = gate1(2, 0, 1, NOT_W, NOT_L, A);
    o1  = gate1(2, 1, 1, NOT_W, NOT_L, A);
    o00 = gate2(3, 0, 1, 0, 2, NAND_W, NAND_L, A);
    o01 = gate2(3, 0, 1, 1, 2, NAND_W, NAND_L, A);
    o10 = gate2(3, 1, 1, 0, 2, NAND_W, NAND_L, A);
    o11 = gate2(3, 1, 1, 1, 2, NAND_W, NAND_L, A);
    not_thresh = (o0 + o1) / 2.0;
    mh = o00;
    if (o01 < mh) mh = o01;
    if (o10 < mh) mh = o10;
    nand_thresh = (mh + o11) / 2.0;
}

/* ================================================================
 * Expression tree compiler (from Demo 19/20, with MUX added)
 * ================================================================ */

typedef enum { E_VAR, E_NOT, E_NAND } EType;
typedef struct Ex { EType t; int vid; struct Ex *l, *r; } Ex;

#define POOL_SZ 2048
static Ex pool[POOL_SZ];
static int pidx = 0;

static Ex *ea(void) {
    Ex *e;
    if (pidx >= POOL_SZ) { fprintf(stderr, "pool!\n"); exit(1); }
    e = &pool[pidx++];
    e->l = NULL; e->r = NULL; e->vid = 0;
    return e;
}
static void ereset(void) { pidx = 0; }

static Ex *evar(int id)     { Ex *e = ea(); e->t = E_VAR; e->vid = id; return e; }
static Ex *enot(Ex *c)      { Ex *e = ea(); e->t = E_NOT; e->l = c; return e; }
static Ex *enand(Ex *a, Ex *b) { Ex *e = ea(); e->t = E_NAND; e->l = a; e->r = b; return e; }
static Ex *eand(Ex *a, Ex *b)  { return enot(enand(a, b)); }
/* eor: kept for future use
static Ex *eor(Ex *a, Ex *b)   { return enand(enot(a), enot(b)); }
*/
static Ex *exor(Ex *a, Ex *b) {
    Ex *n = enand(a, b);
    return enand(enand(a, n), enand(b, n));
}
/* MUX(sel, a, b) = sel ? a : b = NAND(NAND(sel,a), NAND(NOT(sel),b)) */
static Ex *emux(Ex *s, Ex *a, Ex *b) {
    return enand(enand(s, a), enand(enot(s), b));
}

/* Gate evaluation counters */
static int cnt_not = 0, cnt_nand = 0;
static void creset(void) { cnt_not = 0; cnt_nand = 0; }

static int eeval(const Ex *e, const int *v, Cx A) {
    switch (e->t) {
    case E_VAR:
        return v[e->vid];
    case E_NOT: {
        int in = eeval(e->l, v, A);
        double amp = gate1(2, in, 1, NOT_W, NOT_L, A);
        cnt_not++;
        return amp > not_thresh ? 1 : 0;
    }
    case E_NAND: {
        int x = eeval(e->l, v, A);
        int y = eeval(e->r, v, A);
        double amp = gate2(3, x, 1, y, 2, NAND_W, NAND_L, A);
        cnt_nand++;
        return amp > nand_thresh ? 1 : 0;
    }
    }
    return 0;
}

/* ================================================================
 * PART A: Expression-Compiled Toffoli
 *
 * Toffoli(A,B,C) = (A, B, C XOR (A AND B))
 * ================================================================ */

static void part_a_toffoli(void) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int correct = 0;
    int total_not = 0, total_nand = 0;
    int ii;
    char msg[200];

    printf("\n=== PART A: Expression-Compiled Toffoli ===\n");
    printf("  Toffoli(A,B,C) = (A, B, C XOR (A AND B))\n\n");

    init_thresh(A);

    printf("  %-3s %-3s %-3s | %-3s %-3s %-3s | %-3s %-3s %-3s | %s\n",
           "A", "B", "C", "A'", "B'", "C'", "eA", "eB", "eC", "ok?");
    printf("  --- --- --- | --- --- --- | --- --- --- | ---\n");

    for (ii = 0; ii < 8; ii++) {
        int a = (ii >> 2) & 1, b = (ii >> 1) & 1, c = ii & 1;
        int exp_c = c ^ (a & b);
        int got_c;
        int vars[3];
        Ex *toff;
        int ok;

        vars[0] = a; vars[1] = b; vars[2] = c;

        ereset(); creset();
        toff = exor(evar(2), eand(evar(0), evar(1)));
        got_c = eeval(toff, vars, A);
        total_not += cnt_not;
        total_nand += cnt_nand;

        ok = (got_c == exp_c);
        if (ok) correct++;

        printf("  %-3d %-3d %-3d | %-3d %-3d %-3d | %-3d %-3d %-3d | %s\n",
               a, b, c, a, b, got_c, a, b, exp_c,
               ok ? "OK" : "FAIL");
    }

    printf("\n  Avg gate evals per input: NOT=%d, NAND=%d\n",
           total_not / 8, total_nand / 8);
    printf("  Avg crossings per input: %d*6 + %d*5 = %d\n",
           total_not / 8, total_nand / 8,
           (total_not / 8) * 6 + (total_nand / 8) * 5);

    sprintf(msg, "P1: Toffoli truth table correct (%d/8)", correct);
    check(msg, correct == 8);
}

/* ================================================================
 * PART B: Expression-Compiled Fredkin
 *
 * Fredkin(A,B,C) = (A, MUX(A,C,B), MUX(A,B,C))
 * When A=0: pass through. When A=1: swap B and C.
 * ================================================================ */

static void part_b_fredkin(void) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int correct = 0;
    int total_not = 0, total_nand = 0;
    int ii;
    char msg[200];

    printf("\n=== PART B: Expression-Compiled Fredkin ===\n");
    printf("  Fredkin(A,B,C) = (A, MUX(A,C,B), MUX(A,B,C))\n");
    printf("  MUX(s,a,b) = NAND(NAND(s,a), NAND(NOT(s),b))\n\n");

    init_thresh(A);

    printf("  %-3s %-3s %-3s | %-3s %-3s %-3s | %-3s %-3s %-3s | %s\n",
           "A", "B", "C", "A'", "B'", "C'", "eA", "eB", "eC", "ok?");
    printf("  --- --- --- | --- --- --- | --- --- --- | ---\n");

    for (ii = 0; ii < 8; ii++) {
        int a = (ii >> 2) & 1, b = (ii >> 1) & 1, c = ii & 1;
        int exp_b = a ? c : b;  /* MUX(A,C,B) */
        int exp_c = a ? b : c;  /* MUX(A,B,C) */
        int got_b, got_c;
        int vars[3];
        Ex *fb, *fc;
        int ok;

        vars[0] = a; vars[1] = b; vars[2] = c;

        /* B' = MUX(A, C, B) */
        ereset(); creset();
        fb = emux(evar(0), evar(2), evar(1));
        got_b = eeval(fb, vars, A);
        total_not += cnt_not;
        total_nand += cnt_nand;

        /* C' = MUX(A, B, C) */
        ereset(); creset();
        fc = emux(evar(0), evar(1), evar(2));
        got_c = eeval(fc, vars, A);
        total_not += cnt_not;
        total_nand += cnt_nand;

        ok = (got_b == exp_b && got_c == exp_c);
        if (ok) correct++;

        printf("  %-3d %-3d %-3d | %-3d %-3d %-3d | %-3d %-3d %-3d | %s\n",
               a, b, c, a, got_b, got_c, a, exp_b, exp_c,
               ok ? "OK" : "FAIL");
    }

    printf("\n  Avg gate evals per input (both outputs): NOT=%d, NAND=%d\n",
           total_not / 8, total_nand / 8);
    printf("  Avg crossings per input: %d*6 + %d*5 = %d\n",
           total_not / 8, total_nand / 8,
           (total_not / 8) * 6 + (total_nand / 8) * 5);

    sprintf(msg, "P2: Fredkin truth table correct (%d/8)", correct);
    check(msg, correct == 8);
}

/* ================================================================
 * PART C: Multi-Readout Exploration
 *
 * Novel experiment: can a single 4-strand braid word produce
 * different Boolean functions under different strand closures?
 * ================================================================ */

static int gen_map[6] = {1, -1, 2, -2, 3, -3};

static void decode_word(int code, int len, int *word) {
    int i;
    for (i = 0; i < len; i++) {
        word[i] = gen_map[code % 6];
        code /= 6;
    }
}

static void part_c_multi_readout(void) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int tt_toffoli_c = 0, tt_fredkin_b = 0, tt_fredkin_c = 0;
    int tt_and = 0, tt_or = 0, tt_xor_ab = 0;
    int len, code, total_braids = 0;
    int multi_count = 0;
    int toff_hits = 0, fred_b_hits = 0, fred_c_hits = 0;
    int max_distinct = 0;
    int best_word[MAX_WORD];
    int best_len = 0;
    int found[256];
    char msg[200];
    int i;

    printf("\n=== PART C: Multi-Readout Exploration ===\n");
    printf("  Searching 4-strand braids (len 1-4), 4 closure types\n");
    printf("  Question: do different closures give different Boolean functions?\n\n");

    /* Compute target truth tables */
    memset(found, 0, sizeof(found));
    for (i = 0; i < 8; i++) {
        int a = (i >> 2) & 1, b = (i >> 1) & 1, c = i & 1;
        if (c ^ (a & b))   tt_toffoli_c |= (1 << i);
        if (a ? c : b)     tt_fredkin_b |= (1 << i);
        if (a ? b : c)     tt_fredkin_c |= (1 << i);
        if (a & b)         tt_and       |= (1 << i);
        if (a | b)         tt_or        |= (1 << i);
        if (a ^ b)         tt_xor_ab    |= (1 << i);
    }

    printf("  Target truth tables:\n");
    printf("    Toffoli C': 0x%02x, Fredkin B': 0x%02x, Fredkin C': 0x%02x\n",
           tt_toffoli_c, tt_fredkin_b, tt_fredkin_c);
    printf("    AND(A,B): 0x%02x, OR(A,B): 0x%02x, XOR(A,B): 0x%02x\n\n",
           tt_and, tt_or, tt_xor_ab);

    for (len = 1; len <= 4; len++) {
        int n_words = 1;
        int wi;
        for (wi = 0; wi < len; wi++) n_words *= 6;

        for (code = 0; code < n_words; code++) {
            int word[MAX_WORD];
            int closure_tts[4];
            int n_distinct, ci;

            decode_word(code, len, word);
            total_braids++;

            for (ci = 0; ci < 4; ci++) {
                Closure cl = (Closure)ci;
                double amps[8];
                double sorted[8];
                int sorted_idx[8];
                int best_tt = -1;
                double best_gap = 0.0;
                int ii, j, k;

                /* Evaluate all 8 inputs */
                for (ii = 0; ii < 8; ii++) {
                    int a = (ii >> 2) & 1, b = (ii >> 1) & 1, c = ii & 1;
                    int iw[3];
                    int il = 0;
                    Braid br;

                    if (a) iw[il++] = 1;
                    if (b) iw[il++] = 2;
                    if (c) iw[il++] = 3;

                    br = make_gate(4, iw, il, word, len);
                    amps[ii] = cx_abs(bracket_cl(&br, A, cl));
                }

                /* Sort amplitudes (insertion sort) */
                for (ii = 0; ii < 8; ii++) {
                    sorted[ii] = amps[ii];
                    sorted_idx[ii] = ii;
                }
                for (ii = 1; ii < 8; ii++) {
                    double ka = sorted[ii];
                    int ki = sorted_idx[ii];
                    j = ii - 1;
                    while (j >= 0 && sorted[j] > ka) {
                        sorted[j + 1] = sorted[j];
                        sorted_idx[j + 1] = sorted_idx[j];
                        j--;
                    }
                    sorted[j + 1] = ka;
                    sorted_idx[j + 1] = ki;
                }

                /* Try each threshold gap */
                for (k = 0; k < 7; k++) {
                    double gap = sorted[k + 1] - sorted[k];
                    if (gap > 0.001) {
                        double thresh = (sorted[k] + sorted[k + 1]) / 2.0;
                        int tt = 0;
                        int m;
                        for (m = 0; m < 8; m++) {
                            if (amps[m] > thresh)
                                tt |= (1 << m);
                        }
                        if (tt != 0 && tt != 255 && gap > best_gap) {
                            best_gap = gap;
                            best_tt = tt;
                        }
                    }
                }
                closure_tts[ci] = best_tt;
                if (best_tt > 0 && best_tt < 255)
                    found[best_tt]++;
            }

            /* Count distinct non-trivial truth tables */
            {
                int distinct[4];
                int nd = 0;
                int ci2;
                for (ci = 0; ci < 4; ci++) {
                    int is_new = 1;
                    if (closure_tts[ci] < 0) continue;
                    for (ci2 = 0; ci2 < nd; ci2++) {
                        if (distinct[ci2] == closure_tts[ci]) {
                            is_new = 0;
                            break;
                        }
                    }
                    if (is_new && nd < 4) distinct[nd++] = closure_tts[ci];
                }
                n_distinct = nd;
            }

            if (n_distinct > 1) multi_count++;
            if (n_distinct > max_distinct) {
                max_distinct = n_distinct;
                memcpy(best_word, word, (size_t)len * sizeof(int));
                best_len = len;
            }

            /* Check target hits */
            for (ci = 0; ci < 4; ci++) {
                if (closure_tts[ci] == tt_toffoli_c) toff_hits++;
                if (closure_tts[ci] == tt_fredkin_b) fred_b_hits++;
                if (closure_tts[ci] == tt_fredkin_c) fred_c_hits++;
            }
        }
    }

    printf("  Searched %d braid words (lengths 1-4)\n", total_braids);
    printf("  Multi-readout braids (>1 distinct TT): %d (%.1f%%)\n",
           multi_count,
           total_braids > 0 ? 100.0 * (double)multi_count / (double)total_braids : 0.0);
    printf("  Max distinct truth tables from one braid: %d\n", max_distinct);

    if (best_len > 0) {
        printf("  Best multi-readout example: [");
        for (i = 0; i < best_len; i++) {
            if (i > 0) printf(",");
            printf("%d", best_word[i]);
        }
        printf("]\n");
    }

    printf("\n  Target function hits (any closure):\n");
    printf("    Toffoli C' (0x%02x): %d\n", tt_toffoli_c, toff_hits);
    printf("    Fredkin B' (0x%02x): %d\n", tt_fredkin_b, fred_b_hits);
    printf("    Fredkin C' (0x%02x): %d\n", tt_fredkin_c, fred_c_hits);

    /* Top 10 most common functions */
    printf("\n  Top 10 most common Boolean functions found:\n");
    {
        int top_i[10];
        int top_c[10];
        int fi, ti;
        memset(top_i, 0, sizeof(top_i));
        memset(top_c, 0, sizeof(top_c));

        for (fi = 1; fi < 255; fi++) {
            if (found[fi] > top_c[9]) {
                top_c[9] = found[fi];
                top_i[9] = fi;
                for (ti = 8; ti >= 0; ti--) {
                    if (top_c[ti + 1] > top_c[ti]) {
                        int tc = top_c[ti]; int ti2 = top_i[ti];
                        top_c[ti] = top_c[ti + 1]; top_i[ti] = top_i[ti + 1];
                        top_c[ti + 1] = tc; top_i[ti + 1] = ti2;
                    }
                }
            }
        }

        for (ti = 0; ti < 10 && top_c[ti] > 0; ti++) {
            int tt = top_i[ti];
            const char *lab = "";
            int bi;
            if (tt == tt_toffoli_c)      lab = " <-- Toffoli C'";
            else if (tt == tt_fredkin_b)  lab = " <-- Fredkin B'";
            else if (tt == tt_fredkin_c)  lab = " <-- Fredkin C'";
            else if (tt == tt_and)        lab = " <-- AND(A,B)";
            else if (tt == tt_or)         lab = " <-- OR(A,B)";
            else if (tt == tt_xor_ab)     lab = " <-- XOR(A,B)";
            printf("    0x%02x = ", tt);
            for (bi = 7; bi >= 0; bi--)
                printf("%d", (tt >> bi) & 1);
            printf("  count=%d%s\n", top_c[ti], lab);
        }
    }

    sprintf(msg, "P3: multi-readout possible (>1 distinct TT from same braid)");
    check(msg, multi_count > 0);
}

/* ================================================================
 * PART D: Reversible Circuit Composition
 *
 * Double-Toffoli should return the original C:
 *   Toffoli(Toffoli(A,B,C)) = (A, B, C)
 * Double-Fredkin should return original B,C:
 *   Fredkin(Fredkin(A,B,C)) = (A, B, C)
 * ================================================================ */

static void part_d_composition(void) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int toff_ok = 0, fred_ok = 0;
    int ii;
    char msg[200];

    printf("\n=== PART D: Reversible Circuit Composition ===\n\n");

    init_thresh(A);

    /* Double-Toffoli */
    printf("  Double-Toffoli: Toffoli(Toffoli(A,B,C)) = (A,B,C)?\n");
    for (ii = 0; ii < 8; ii++) {
        int a = (ii >> 2) & 1, b = (ii >> 1) & 1, c = ii & 1;
        int vars[3], vars2[3];
        int cp, cpp;
        Ex *t;

        vars[0] = a; vars[1] = b; vars[2] = c;
        ereset();
        t = exor(evar(2), eand(evar(0), evar(1)));
        cp = eeval(t, vars, A);

        vars2[0] = a; vars2[1] = b; vars2[2] = cp;
        ereset();
        t = exor(evar(2), eand(evar(0), evar(1)));
        cpp = eeval(t, vars2, A);

        printf("    (%d,%d,%d) -> (%d,%d,%d) -> (%d,%d,%d)  %s\n",
               a, b, c, a, b, cp, a, b, cpp,
               cpp == c ? "OK" : "FAIL");
        if (cpp == c) toff_ok++;
    }

    sprintf(msg, "P4a: double-Toffoli = identity (%d/8)", toff_ok);
    check(msg, toff_ok == 8);

    /* Double-Fredkin */
    printf("\n  Double-Fredkin: Fredkin(Fredkin(A,B,C)) = (A,B,C)?\n");
    for (ii = 0; ii < 8; ii++) {
        int a = (ii >> 2) & 1, b = (ii >> 1) & 1, c = ii & 1;
        int vars[3], vars2[3];
        int bp, cp, bpp, cpp;
        Ex *fb, *fc;

        vars[0] = a; vars[1] = b; vars[2] = c;
        ereset();
        fb = emux(evar(0), evar(2), evar(1));
        bp = eeval(fb, vars, A);
        ereset();
        fc = emux(evar(0), evar(1), evar(2));
        cp = eeval(fc, vars, A);

        vars2[0] = a; vars2[1] = bp; vars2[2] = cp;
        ereset();
        fb = emux(evar(0), evar(2), evar(1));
        bpp = eeval(fb, vars2, A);
        ereset();
        fc = emux(evar(0), evar(1), evar(2));
        cpp = eeval(fc, vars2, A);

        printf("    (%d,%d,%d) -> (%d,%d,%d) -> (%d,%d,%d)  %s\n",
               a, b, c, a, bp, cp, a, bpp, cpp,
               (bpp == b && cpp == c) ? "OK" : "FAIL");
        if (bpp == b && cpp == c) fred_ok++;
    }

    sprintf(msg, "P4b: double-Fredkin = identity (%d/8)", fred_ok);
    check(msg, fred_ok == 8);
}

/* ================================================================
 * PART E: Efficiency Analysis
 * ================================================================ */

static void part_e_efficiency(void) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int ii;
    int tn = 0, tna = 0;     /* Toffoli NOT/NAND totals */
    int fn = 0, fna = 0;     /* Fredkin NOT/NAND totals */
    int toff_cross, fred_cross;
    char msg[200];

    printf("\n=== PART E: Efficiency Analysis ===\n\n");

    init_thresh(A);

    /* Toffoli: count gate evals for nontrivial output */
    for (ii = 0; ii < 8; ii++) {
        int vars[3];
        Ex *e;
        vars[0] = (ii >> 2) & 1;
        vars[1] = (ii >> 1) & 1;
        vars[2] = ii & 1;
        ereset(); creset();
        e = exor(evar(2), eand(evar(0), evar(1)));
        (void)eeval(e, vars, A);
        tn += cnt_not;
        tna += cnt_nand;
    }

    /* Fredkin: count gate evals for both nontrivial outputs */
    for (ii = 0; ii < 8; ii++) {
        int vars[3];
        Ex *e;
        vars[0] = (ii >> 2) & 1;
        vars[1] = (ii >> 1) & 1;
        vars[2] = ii & 1;

        ereset(); creset();
        e = emux(evar(0), evar(2), evar(1));
        (void)eeval(e, vars, A);
        fn += cnt_not;
        fna += cnt_nand;

        ereset(); creset();
        e = emux(evar(0), evar(1), evar(2));
        (void)eeval(e, vars, A);
        fn += cnt_not;
        fna += cnt_nand;
    }

    toff_cross = (tn / 8) * 6 + (tna / 8) * 5;
    fred_cross = (fn / 8) * 6 + (fna / 8) * 5;

    printf("  Toffoli (nontrivial output C' = XOR(C, AND(A,B))):\n");
    printf("    NOT evals per input:  %d  (%d crossings)\n",
           tn / 8, (tn / 8) * 6);
    printf("    NAND evals per input: %d  (%d crossings)\n",
           tna / 8, (tna / 8) * 5);
    printf("    Total crossings/input: %d\n", toff_cross);
    printf("    (Note: shared sub-expressions are re-evaluated without memoization)\n");

    printf("\n  Fredkin (both nontrivial outputs):\n");
    printf("    NOT evals per input:  %d  (%d crossings)\n",
           fn / 8, (fn / 8) * 6);
    printf("    NAND evals per input: %d  (%d crossings)\n",
           fna / 8, (fna / 8) * 5);
    printf("    Total crossings/input: %d\n", fred_cross);

    printf("\n  Comparison with literature:\n");
    printf("    A5 coloured-braid Toffoli (Bonesteel et al.): ~132 crossings\n");
    printf("    Our expression-compiled Toffoli:                %d crossings\n",
           toff_cross);
    if (toff_cross > 0 && toff_cross < 132) {
        printf("    Ratio: %.1fx fewer crossings\n",
               132.0 / (double)toff_cross);
    }

    printf("\n  Architectural comparison:\n");
    printf("    A5: single braid -> unitary matrix -> quantum gate\n");
    printf("    Ours: %d separate bracket evals -> classical bits -> classical gate\n",
           tn / 8 + tna / 8);
    printf("    Not apples-to-apples, but total topological complexity is lower.\n");

    sprintf(msg, "P5: Toffoli < 132 crossings (%d)", toff_cross);
    check(msg, toff_cross < 132);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 21: Reversible Gates\n");
    printf("===================================\n");

    part_a_toffoli();
    part_b_fredkin();
    part_c_multi_readout();
    part_d_composition();
    part_e_efficiency();

    printf("\n===================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("===================================\n");
    return n_fail > 0 ? 1 : 0;
}
