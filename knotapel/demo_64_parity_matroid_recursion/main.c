/*
 * KNOTAPEL DEMO 64: Parity Matroid Recursion
 * ===========================================
 *
 * The parity scaling law (Demo 63) established:
 *   XOR3: 23,004 solutions at k=15
 *   XOR4: 16,108 solutions at k=15
 *   XOR5:  3,020 solutions at k=15
 *   XOR6:      0 solutions (impossible)
 *
 * This demo asks: is this hierarchy a single recursive matroid
 * object? Matroid theory has two fundamental operations:
 *
 *   Deletion  (fix x_i = 0): Remove w_i, compute on remaining.
 *     XOR_n(0, rest) = XOR_{n-1}(rest)
 *
 *   Contraction (fix x_i = 1): Add bias w_i to all sums.
 *     XOR_n(1, rest) = XNOR_{n-1}(rest)
 *
 * Part A tests deletion closure: every deletion of an XOR_n
 * solution produces a valid XOR_{n-1} solution, for n = 5,4,3.
 *
 * Part B tests contraction: every contraction of an XOR_n
 * solution produces a valid XNOR_{n-1} function.
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
 * Exact cyclotomic type Z[zeta_8]
 * ================================================================ */

typedef struct { long a, b, c, d; } Cyc8;

static Cyc8 cyc8_make(long a, long b, long c, long d) {
    Cyc8 z; z.a = a; z.b = b; z.c = c; z.d = d; return z;
}
static Cyc8 cyc8_zero(void) { return cyc8_make(0, 0, 0, 0); }
static Cyc8 cyc8_one(void)  { return cyc8_make(1, 0, 0, 0); }
static Cyc8 cyc8_add(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a + y.a, x.b + y.b, x.c + y.c, x.d + y.d);
}
static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y) {
    return cyc8_make(
        x.a * y.a - x.b * y.d - x.c * y.c - x.d * y.b,
        x.a * y.b + x.b * y.a - x.c * y.d - x.d * y.c,
        x.a * y.c + x.b * y.b + x.c * y.a - x.d * y.d,
        x.a * y.d + x.b * y.c + x.c * y.b + x.d * y.a);
}
static int cyc8_eq(Cyc8 x, Cyc8 y) {
    return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}
static Cx cyc8_to_cx(Cyc8 z) {
    double s = 1.0 / sqrt(2.0);
    return cx_make((double)z.a + (double)z.b * s + (double)z.d * (-s),
                   (double)z.b * s + (double)z.c + (double)z.d * s);
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

/* ================================================================
 * Braid type + union-find loop counter (from Demo 50)
 * ================================================================ */

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
 * Bracket catalog builder (standard, from Demo 50)
 * ================================================================ */

#define MAX_DISTINCT 512

static Cyc8 cat_exact[MAX_DISTINCT];
static Cx   cat_cx[MAX_DISTINCT];
static int  cat_octant[MAX_DISTINCT];
static int  cat_size = 0;

static int find_in(const Cyc8 *arr, int size, Cyc8 val) {
    int i;
    for (i = 0; i < size; i++)
        if (cyc8_eq(val, arr[i])) return i;
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
                Cyc8 bracket;
                unsigned s, ns;
                int a_count, b_count, lp;

                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen) word_buf[i] = g + 1;
                    else             word_buf[i] = -(g - max_gen + 1);
                }
                b.n = n; b.len = len;
                memcpy(b.word, word_buf, (size_t)len * sizeof(int));

                bracket = cyc8_zero();
                if (b.len > 0) {
                    ns = 1u << b.len;
                    for (s = 0; s < ns; s++) {
                        a_count = 0; b_count = 0;
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
                }

                if (!cyc8_eq(bracket, cyc8_zero())
                    && find_in(cat_exact, cat_size, bracket) < 0
                    && cat_size < MAX_DISTINCT) {
                    cat_exact[cat_size] = bracket;
                    cat_cx[cat_size] = cyc8_to_cx(bracket);
                    cat_size++;
                }
            }
        }
    }

    /* Compute octants */
    {
        int i;
        for (i = 0; i < cat_size; i++) {
            double angle = atan2(cat_cx[i].im, cat_cx[i].re);
            int oct;
            if (angle < 0.0) angle += 2.0 * M_PI;
            oct = (int)(angle / (M_PI / 4.0));
            if (oct > 7) oct = 7;
            cat_octant[i] = oct;
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
 * Class-1 catalog index collection
 * ================================================================ */

static int class1_idx[MAX_DISTINCT];
static int n_class1 = 0;

static void collect_class1(int k) {
    int i;
    n_class1 = 0;
    for (i = 0; i < cat_size; i++)
        if (sector_classify(cat_cx[i], k))
            class1_idx[n_class1++] = i;
}

/* ================================================================
 * Utility: popcount
 * ================================================================ */

static int popcount(int x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

/* ================================================================
 * Truth table computation
 *
 * compute_tt: standard (no bias), f(0,...,0) = 0 by convention
 * compute_tt_biased: with bias added to every sum
 * xor_tt / xnor_tt: expected truth tables for XOR_n / XNOR_n
 * ================================================================ */

/* General n-input truth table (n <= 4 for int safety) */
static int compute_tt(const Cx *w, int n, int k) {
    int tt = 0, idx, total = 1 << n;
    for (idx = 1; idx < total; idx++) {
        Cx z = cx_make(0.0, 0.0);
        int bit;
        for (bit = 0; bit < n; bit++) {
            if ((idx >> (n - 1 - bit)) & 1)
                z = cx_add(z, w[bit]);
        }
        if (sector_classify(z, k)) tt |= (1 << idx);
    }
    return tt;
}

/* Expected XOR_n truth table: bit i set iff popcount(i) is odd */
static int xor_tt(int n) {
    int tt = 0, idx, total = 1 << n;
    for (idx = 1; idx < total; idx++) {
        if (popcount(idx) % 2 == 1) tt |= (1 << idx);
    }
    return tt;
}

/* Expected XNOR_n truth table: bit i set iff popcount(i) is even */
static int xnor_tt(int n) {
    int tt = 0, idx, total = 1 << n;
    for (idx = 0; idx < total; idx++) {
        if (popcount(idx) % 2 == 0) tt |= (1 << idx);
    }
    return tt;
}

/* ================================================================
 * Solution storage
 *
 * Store all XOR_n solutions at k=15 for n = 5, 4, 3.
 * Stored as catalog indices.
 * ================================================================ */

#define MAX_SOL5 4096
#define MAX_SOL4 20000
#define MAX_SOL3 30000
#define MAX_SOL2 100000

static int sol5[MAX_SOL5][5];
static int n_sol5 = 0;

static int sol4[MAX_SOL4][4];
static int n_sol4 = 0;

static int sol3[MAX_SOL3][3];
static int n_sol3 = 0;

static int sol2[MAX_SOL2][2];
static int n_sol2 = 0;

/* ================================================================
 * Recursive XOR solution enumerator
 *
 * Finds all weight tuples (w_0, ..., w_{n-1}) from the class-1
 * catalog such that for every nonempty subset S:
 *   sector_classify(sum_S, k) = |S| mod 2
 *
 * This is the XOR_n constraint: odd subsets -> class 1,
 * even subsets -> class 0.
 * ================================================================ */

static Cx   sr_partial[64];   /* partial sums by subset mask */
static int  sr_indices[8];    /* current weight indices */
static int  sr_n;             /* target n (for progress) */

/* Storage target (set before calling search) */
static int *sr_target;
static int  sr_width;
static int  sr_max;
static int *sr_count;

static void search_xor(int depth, int n, int k) {
    int n_prev, e, j, ok, ie;
    Cx we, ns;
    int sz, req;

    if (depth == n) {
        if (*sr_count < sr_max) {
            int *row = sr_target + (*sr_count) * sr_width;
            for (j = 0; j < n; j++)
                row[j] = sr_indices[j];
            (*sr_count)++;
        }
        return;
    }

    n_prev = 1 << depth;

    for (e = 0; e < n_class1; e++) {
        ie = class1_idx[e];
        we = cat_cx[ie];
        ok = 1;

        /* Progress at depth 0 for large searches */
        if (depth == 0 && sr_n >= 5) {
            printf("\r    %d/%d", e + 1, n_class1);
            fflush(stdout);
        }

        /* Check all 2^depth new subsets formed by adding this weight */
        for (j = 0; j < n_prev; j++) {
            ns = cx_add(sr_partial[j], we);
            sz = popcount(j) + 1;
            req = sz % 2;
            if (sector_classify(ns, k) != req) {
                ok = 0;
                break;
            }
        }

        if (ok) {
            for (j = 0; j < n_prev; j++)
                sr_partial[n_prev + j] =
                    cx_add(sr_partial[j], we);
            sr_indices[depth] = ie;
            search_xor(depth + 1, n, k);
        }
    }
}

static void enumerate_xor(int n, int k,
                           int *target, int width,
                           int max_sol, int *count) {
    *count = 0;
    sr_target = target;
    sr_width = width;
    sr_max = max_sol;
    sr_count = count;
    sr_n = n;
    sr_partial[0] = cx_make(0.0, 0.0);
    search_xor(0, n, k);
}

/* ================================================================
 * Deletion / Contraction test helpers
 * ================================================================ */

/* Test all deletions of n-weight solutions.
 * For each solution, delete each of the n weights and check if
 * the resulting (n-1)-weight tuple computes the expected truth table. */
static void test_deletions(const int *sols, int n_sols, int n,
                           int k, int expected_tt,
                           const char *label,
                           int *total_out, int *valid_out) {
    int s, d, j, p;
    int total = 0, valid = 0;

    for (s = 0; s < n_sols; s++) {
        const int *sol = sols + s * n;
        for (d = 0; d < n; d++) {
            Cx w[7];
            int tt;
            p = 0;
            for (j = 0; j < n; j++) {
                if (j == d) continue;
                w[p++] = cat_cx[sol[j]];
            }
            tt = compute_tt(w, n - 1, k);
            total++;
            if (tt == expected_tt) valid++;
        }
    }
    printf("    %d solutions * %d deletions = %d checks\n",
           n_sols, n, total);
    printf("    Valid %s: %d/%d (%.1f%%)\n",
           label, valid, total,
           total > 0 ? 100.0 * (double)valid / (double)total : 0.0);
    *total_out = total;
    *valid_out = valid;
}

/* Test all contractions using EXACT Z[zeta_8] arithmetic for sums.
 * This avoids floating-point association-order issues: integer sums
 * are computed first, converted to Cx only for sector_classify. */
static void test_contractions_exact(const int *sols, int n_sols, int n,
                                    int k, int expected_tt,
                                    const char *label,
                                    int *total_out, int *valid_out,
                                    int *n_bad_sols) {
    int s, d, j, p;
    int total = 0, valid = 0;
    int bad_sols = 0;

    for (s = 0; s < n_sols; s++) {
        const int *sol = sols + s * n;
        int sol_ok = 1;
        for (d = 0; d < n; d++) {
            int w_idx[7];
            int bias_idx;
            int tt = 0, idx, tt_total = 1 << (n - 1);
            p = 0;
            bias_idx = sol[d];
            for (j = 0; j < n; j++) {
                if (j == d) continue;
                w_idx[p++] = sol[j];
            }
            /* Compute biased TT using exact integer sums */
            for (idx = 0; idx < tt_total; idx++) {
                Cyc8 ez = cat_exact[bias_idx];
                Cx z;
                int bit;
                for (bit = 0; bit < n - 1; bit++) {
                    if ((idx >> (n - 2 - bit)) & 1)
                        ez = cyc8_add(ez, cat_exact[w_idx[bit]]);
                }
                z = cyc8_to_cx(ez);
                if (sector_classify(z, k)) tt |= (1 << idx);
            }
            total++;
            if (tt == expected_tt) valid++;
            else sol_ok = 0;
        }
        if (!sol_ok) bad_sols++;
    }
    printf("    %d solutions * %d contractions = %d checks\n",
           n_sols, n, total);
    printf("    Valid %s (exact arith): %d/%d (%.1f%%)\n",
           label, valid, total,
           total > 0 ? 100.0 * (double)valid / (double)total : 0.0);
    if (valid < total) {
        printf("    %d solutions have at least one failure\n", bad_sols);
    }
    *total_out = total;
    *valid_out = valid;
    *n_bad_sols = bad_sols;
}

/* ================================================================
 * PART A: Deletion Closure
 *
 * For each level of the parity hierarchy, delete each weight
 * and verify the resulting function is the next-lower XOR.
 *
 * XOR5(0, x1..x4) = XOR4(x1..x4)
 * XOR4(0, x1..x3) = XOR3(x1..x3)
 * XOR3(0, x1..x2) = XOR2(x1..x2)
 * ================================================================ */

static void part_a_deletion(void) {
    int K = 15;
    int total, valid;
    char msg[256];

    printf("\n=== PART A: Deletion Closure ===\n\n");

    /* Show expected truth tables */
    printf("  Expected truth tables:\n");
    printf("    XOR4 = 0x%04X\n", xor_tt(4));
    printf("    XOR3 = 0x%02X\n", xor_tt(3));
    printf("    XOR2 = 0x%X\n\n", xor_tt(2));

    /* Enumerate all XOR solutions at k=15 */
    printf("  Enumerating XOR5 at k=%d...", K);
    fflush(stdout);
    enumerate_xor(5, K, &sol5[0][0], 5, MAX_SOL5, &n_sol5);
    printf("\r  XOR5 at k=%d: %d solutions                \n", K, n_sol5);
    if (n_sol5 > 0) {
        Cyc8 v0 = cat_exact[sol5[0][0]];
        Cyc8 v1 = cat_exact[sol5[0][1]];
        printf("    first: (%ld,%ld,%ld,%ld) (%ld,%ld,%ld,%ld) ... "
               "oct{%d,%d,%d,%d,%d}\n",
               v0.a, v0.b, v0.c, v0.d,
               v1.a, v1.b, v1.c, v1.d,
               cat_octant[sol5[0][0]], cat_octant[sol5[0][1]],
               cat_octant[sol5[0][2]], cat_octant[sol5[0][3]],
               cat_octant[sol5[0][4]]);
    }

    printf("  Enumerating XOR4 at k=%d...", K);
    fflush(stdout);
    enumerate_xor(4, K, &sol4[0][0], 4, MAX_SOL4, &n_sol4);
    printf("\r  XOR4 at k=%d: %d solutions                \n", K, n_sol4);

    printf("  Enumerating XOR3 at k=%d...", K);
    fflush(stdout);
    enumerate_xor(3, K, &sol3[0][0], 3, MAX_SOL3, &n_sol3);
    printf("\r  XOR3 at k=%d: %d solutions                \n", K, n_sol3);

    /* Verify counts match Demo 63 */
    sprintf(msg, "XOR5 count = 3020 (matches Demo 63)");
    check(msg, n_sol5 == 3020);
    sprintf(msg, "XOR4 count = 16108 (matches Demo 63)");
    check(msg, n_sol4 == 16108);
    sprintf(msg, "XOR3 count = 23004 (matches Demo 63)");
    check(msg, n_sol3 == 23004);

    /* --- XOR5 -> XOR4 --- */
    printf("\n  --- XOR5 -> XOR4 deletions ---\n");
    test_deletions(&sol5[0][0], n_sol5, 5, K, xor_tt(4),
                   "XOR4", &total, &valid);
    sprintf(msg, "deletion closure: XOR5 -> XOR4 (%d/%d)",
            valid, total);
    check(msg, valid == total && total > 0);

    /* --- XOR4 -> XOR3 --- */
    printf("\n  --- XOR4 -> XOR3 deletions ---\n");
    test_deletions(&sol4[0][0], n_sol4, 4, K, xor_tt(3),
                   "XOR3", &total, &valid);
    sprintf(msg, "deletion closure: XOR4 -> XOR3 (%d/%d)",
            valid, total);
    check(msg, valid == total && total > 0);

    /* --- XOR3 -> XOR2 --- */
    printf("\n  --- XOR3 -> XOR2 deletions ---\n");
    test_deletions(&sol3[0][0], n_sol3, 3, K, xor_tt(2),
                   "XOR2", &total, &valid);
    sprintf(msg, "deletion closure: XOR3 -> XOR2 (%d/%d)",
            valid, total);
    check(msg, valid == total && total > 0);
}

/* ================================================================
 * PART B: Contraction
 *
 * Contraction = fix x_i = 1 = add w_i as bias to all sums.
 * XOR_n(1, rest) = XNOR_{n-1}(rest) since fixing one input
 * to 1 flips the parity of the remaining function.
 *
 * Expected biased truth tables:
 *   XNOR4 = 0x9669  (complement of XOR4 = 0x6996, including bit 0)
 *   XNOR3 = 0x69    (complement of XOR3 = 0x96, including bit 0)
 *   XNOR2 = 0x9     (complement of XOR2 = 0x6, including bit 0)
 * ================================================================ */

static void part_b_contraction(void) {
    int K = 15;
    int total, valid, bad_sols;
    char msg[256];

    printf("\n=== PART B: Contraction (exact Z[zeta_8] arithmetic) ===\n\n");

    /* Show expected truth tables */
    printf("  Expected biased truth tables:\n");
    printf("    XNOR4 = 0x%04X\n", xnor_tt(4));
    printf("    XNOR3 = 0x%02X\n", xnor_tt(3));
    printf("    XNOR2 = 0x%X\n\n", xnor_tt(2));

    /* --- XOR5 -> XNOR4 --- */
    printf("  --- XOR5 -> XNOR4 contractions ---\n");
    test_contractions_exact(&sol5[0][0], n_sol5, 5, K, xnor_tt(4),
                            "XNOR4", &total, &valid, &bad_sols);
    sprintf(msg, "contraction: XOR5 -> XNOR4 (%d/%d)",
            valid, total);
    check(msg, valid == total && total > 0);

    /* --- XOR4 -> XNOR3 --- */
    printf("\n  --- XOR4 -> XNOR3 contractions ---\n");
    test_contractions_exact(&sol4[0][0], n_sol4, 4, K, xnor_tt(3),
                            "XNOR3", &total, &valid, &bad_sols);
    sprintf(msg, "contraction: XOR4 -> XNOR3 (%d/%d)",
            valid, total);
    check(msg, valid == total && total > 0);

    /* --- XOR3 -> XNOR2 --- */
    printf("\n  --- XOR3 -> XNOR2 contractions ---\n");
    test_contractions_exact(&sol3[0][0], n_sol3, 3, K, xnor_tt(2),
                            "XNOR2", &total, &valid, &bad_sols);
    sprintf(msg, "contraction: XOR3 -> XNOR2 (%d/%d)",
            valid, total);
    check(msg, valid == total && total > 0);
}

/* ================================================================
 * Canonical key utilities for Parts C & D
 *
 * A "canonical key" uniquely identifies an unordered weight SET.
 * We sort the catalog indices and encode as a base-100 number:
 *   key = i0 + i1*100 + i2*100^2 + ...
 * Since catalog has 100 values and indices < 100, this is injective.
 * ================================================================ */

static int cmp_int(const void *a, const void *b) {
    int ia = *(const int *)a, ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

static long make_key(const int *indices, int n) {
    int sorted[8];
    long key;
    int i;
    memcpy(sorted, indices, (size_t)n * sizeof(int));
    qsort(sorted, (size_t)n, sizeof(int), cmp_int);
    key = 0;
    for (i = n - 1; i >= 0; i--)
        key = key * 100 + (long)sorted[i];
    return key;
}

static int cmp_long(const void *a, const void *b) {
    long la = *(const long *)a, lb = *(const long *)b;
    return (la > lb) - (la < lb);
}

/* Dedup a sorted long array in-place, return new count */
static int dedup_longs(long *arr, int n) {
    int w, r;
    if (n <= 1) return n;
    w = 1;
    for (r = 1; r < n; r++) {
        if (arr[r] != arr[r - 1])
            arr[w++] = arr[r];
    }
    return w;
}

/* Binary search for key in sorted array */
static int find_key(const long *arr, int n, long key) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == key) return mid;
        if (arr[mid] < key) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

/* ================================================================
 * Canonical key storage
 * ================================================================ */

static long ck5[MAX_SOL5];
static int  nck5 = 0;

static long ck4[MAX_SOL4];
static int  nck4 = 0;

static long ck3[MAX_SOL3];
static int  nck3 = 0;

static long ck2[MAX_SOL2];
static int  nck2 = 0;

/* Build canonical key set from solution array.
 * Returns number of DISTINCT keys (after sort+dedup). */
static int build_keys(const int *sols, int n_sols, int width,
                      long *keys) {
    int i;
    for (i = 0; i < n_sols; i++)
        keys[i] = make_key(sols + i * width, width);
    qsort(keys, (size_t)n_sols, sizeof(long), cmp_long);
    return dedup_longs(keys, n_sols);
}

/* ================================================================
 * PART C: Coverage (Upward Extension)
 *
 * For each level, how many lower-level DISTINCT weight sets
 * are "hit" by deleting one input from the level above?
 *
 * Coverage(5->4): for each XOR5 solution, delete each weight,
 *   compute canonical key of resulting 4-tuple, check if it's in
 *   the XOR4 canonical key set.
 *
 * Also: XOR4->XOR3, XOR3->XOR2.
 *
 * "Orphans" = lower-level keys that are NOT hit by any deletion
 * from above.
 * ================================================================ */

static void part_c_coverage(void) {
    int K = 15;
    char msg[256];
    int *hit;
    int n_hit, n_orphan;

    printf("\n=== PART C: Coverage (Upward Extension) ===\n\n");

    /* Enumerate XOR2 */
    printf("  Enumerating XOR2 at k=%d...\n", K);
    enumerate_xor(2, K, &sol2[0][0], 2, MAX_SOL2, &n_sol2);
    printf("  XOR2 at k=%d: %d solutions\n", K, n_sol2);

    /* Build canonical key sets */
    printf("  Building canonical key sets...\n");
    nck5 = build_keys(&sol5[0][0], n_sol5, 5, ck5);
    nck4 = build_keys(&sol4[0][0], n_sol4, 4, ck4);
    nck3 = build_keys(&sol3[0][0], n_sol3, 3, ck3);
    nck2 = build_keys(&sol2[0][0], n_sol2, 2, ck2);

    printf("  Distinct weight sets: XOR5=%d  XOR4=%d  XOR3=%d  XOR2=%d\n\n",
           nck5, nck4, nck3, nck2);

    /* --- Coverage: XOR5 -> XOR4 --- */
    {
        int s, d, j, p;
        printf("  --- XOR5 -> XOR4 coverage ---\n");
        hit = (int *)calloc((size_t)nck4, sizeof(int));
        for (s = 0; s < n_sol5; s++) {
            const int *sol = &sol5[s][0];
            for (d = 0; d < 5; d++) {
                int sub[4];
                long key;
                int idx;
                p = 0;
                for (j = 0; j < 5; j++) {
                    if (j == d) continue;
                    sub[p++] = sol[j];
                }
                key = make_key(sub, 4);
                idx = find_key(ck4, nck4, key);
                if (idx >= 0) hit[idx] = 1;
            }
        }
        n_hit = 0;
        for (j = 0; j < nck4; j++) n_hit += hit[j];
        n_orphan = nck4 - n_hit;
        printf("    XOR4 distinct sets covered by XOR5 deletions: %d/%d (%.1f%%)\n",
               n_hit, nck4,
               nck4 > 0 ? 100.0 * (double)n_hit / (double)nck4 : 0.0);
        printf("    XOR4 orphans (not reachable from XOR5): %d\n", n_orphan);
        sprintf(msg, "XOR5->XOR4 coverage: %d/%d sets covered", n_hit, nck4);
        check(msg, 1);  /* informational */
        free(hit);
    }

    /* --- Coverage: XOR4 -> XOR3 --- */
    {
        int s, d, j, p;
        printf("\n  --- XOR4 -> XOR3 coverage ---\n");
        hit = (int *)calloc((size_t)nck3, sizeof(int));
        for (s = 0; s < n_sol4; s++) {
            const int *sol = &sol4[s][0];
            for (d = 0; d < 4; d++) {
                int sub[3];
                long key;
                int idx;
                p = 0;
                for (j = 0; j < 4; j++) {
                    if (j == d) continue;
                    sub[p++] = sol[j];
                }
                key = make_key(sub, 3);
                idx = find_key(ck3, nck3, key);
                if (idx >= 0) hit[idx] = 1;
            }
        }
        n_hit = 0;
        for (j = 0; j < nck3; j++) n_hit += hit[j];
        n_orphan = nck3 - n_hit;
        printf("    XOR3 distinct sets covered by XOR4 deletions: %d/%d (%.1f%%)\n",
               n_hit, nck3,
               nck3 > 0 ? 100.0 * (double)n_hit / (double)nck3 : 0.0);
        printf("    XOR3 orphans (not reachable from XOR4): %d\n", n_orphan);
        sprintf(msg, "XOR4->XOR3 coverage: %d/%d sets covered", n_hit, nck3);
        check(msg, 1);  /* informational */
        free(hit);
    }

    /* --- Coverage: XOR3 -> XOR2 --- */
    {
        int s, d, j, p;
        printf("\n  --- XOR3 -> XOR2 coverage ---\n");
        hit = (int *)calloc((size_t)nck2, sizeof(int));
        for (s = 0; s < n_sol3; s++) {
            const int *sol = &sol3[s][0];
            for (d = 0; d < 3; d++) {
                int sub[2];
                long key;
                int idx;
                p = 0;
                for (j = 0; j < 3; j++) {
                    if (j == d) continue;
                    sub[p++] = sol[j];
                }
                key = make_key(sub, 2);
                idx = find_key(ck2, nck2, key);
                if (idx >= 0) hit[idx] = 1;
            }
        }
        n_hit = 0;
        for (j = 0; j < nck2; j++) n_hit += hit[j];
        n_orphan = nck2 - n_hit;
        printf("    XOR2 distinct sets covered by XOR3 deletions: %d/%d (%.1f%%)\n",
               n_hit, nck2,
               nck2 > 0 ? 100.0 * (double)n_hit / (double)nck2 : 0.0);
        printf("    XOR2 orphans (not reachable from XOR3): %d\n", n_orphan);
        sprintf(msg, "XOR3->XOR2 coverage: %d/%d sets covered", n_hit, nck2);
        check(msg, 1);  /* informational */
        free(hit);
    }
}

/* ================================================================
 * PART D: Parent-Child Multiplicity
 *
 * For the XOR5 -> XOR4 mapping: for each XOR4 weight set that IS
 * covered, how many distinct XOR5 parents does it have?
 *
 * A "parent" is a distinct XOR5 weight set that, when one weight
 * is deleted, yields this XOR4 set.
 *
 * Report: min, max, mean, distribution histogram.
 * Also analyze the XOR4->XOR3 level.
 * ================================================================ */

typedef struct { long child; long parent; } DelPair;

static int cmp_delpair(const void *a, const void *b) {
    const DelPair *pa = (const DelPair *)a;
    const DelPair *pb = (const DelPair *)b;
    if (pa->child < pb->child) return -1;
    if (pa->child > pb->child) return 1;
    if (pa->parent < pb->parent) return -1;
    if (pa->parent > pb->parent) return 1;
    return 0;
}

static void report_multiplicity(const char *label,
                                DelPair *pairs, int n_pairs,
                                const long *child_keys, int n_child_keys) {
    int i, n_distinct, run_start;
    int *parent_counts;
    int n_covered, max_parents, min_parents;
    double sum_parents;
    int histo[64];

    if (n_pairs == 0) {
        printf("    No parent-child pairs to analyze.\n");
        return;
    }

    /* Sort by (child, parent) */
    qsort(pairs, (size_t)n_pairs, sizeof(DelPair), cmp_delpair);

    /* Dedup: remove duplicate (child, parent) pairs */
    n_distinct = 1;
    for (i = 1; i < n_pairs; i++) {
        if (pairs[i].child != pairs[i - 1].child ||
            pairs[i].parent != pairs[i - 1].parent) {
            pairs[n_distinct++] = pairs[i];
        }
    }
    printf("    %s: %d raw pairs -> %d distinct (child, parent)\n",
           label, n_pairs, n_distinct);

    /* Count parents per child */
    parent_counts = (int *)calloc((size_t)n_child_keys, sizeof(int));
    run_start = 0;
    for (i = 0; i <= n_distinct; i++) {
        if (i == n_distinct || (i > 0 && pairs[i].child != pairs[i - 1].child)) {
            long child_key = pairs[run_start].child;
            int count = i - run_start;
            int idx = find_key(child_keys, n_child_keys, child_key);
            if (idx >= 0) parent_counts[idx] = count;
            run_start = i;
        }
    }

    /* Statistics */
    n_covered = 0;
    max_parents = 0;
    min_parents = n_distinct + 1;
    sum_parents = 0.0;
    memset(histo, 0, sizeof(histo));

    for (i = 0; i < n_child_keys; i++) {
        int c = parent_counts[i];
        if (c > 0) {
            n_covered++;
            sum_parents += (double)c;
            if (c > max_parents) max_parents = c;
            if (c < min_parents) min_parents = c;
            if (c < 64) histo[c]++;
            else histo[63]++;
        }
    }

    printf("    Covered children: %d/%d\n", n_covered, n_child_keys);
    if (n_covered > 0) {
        printf("    Parent count: min=%d, max=%d, mean=%.2f\n",
               min_parents, max_parents, sum_parents / (double)n_covered);
        printf("    Distribution:\n");
        for (i = 0; i < 64; i++) {
            if (histo[i] > 0) {
                if (i < 63)
                    printf("      %d parents: %d children\n", i, histo[i]);
                else
                    printf("      63+ parents: %d children\n", histo[i]);
            }
        }
    }

    free(parent_counts);
}

static void part_d_multiplicity(void) {
    char msg[256];

    printf("\n=== PART D: Parent-Child Multiplicity ===\n\n");

    /* --- XOR5 -> XOR4 --- */
    {
        DelPair *pairs;
        int n_pairs = 0;
        int s, d, j, p;

        printf("  --- XOR5 -> XOR4 multiplicity ---\n");
        pairs = (DelPair *)malloc(
            (size_t)n_sol5 * 5 * sizeof(DelPair));

        for (s = 0; s < n_sol5; s++) {
            const int *sol = &sol5[s][0];
            long parent_key = make_key(sol, 5);
            for (d = 0; d < 5; d++) {
                int sub[4];
                long child_key;
                p = 0;
                for (j = 0; j < 5; j++) {
                    if (j == d) continue;
                    sub[p++] = sol[j];
                }
                child_key = make_key(sub, 4);
                if (find_key(ck4, nck4, child_key) >= 0) {
                    pairs[n_pairs].child = child_key;
                    pairs[n_pairs].parent = parent_key;
                    n_pairs++;
                }
            }
        }

        report_multiplicity("XOR5->XOR4", pairs, n_pairs, ck4, nck4);
        sprintf(msg, "XOR5->XOR4 multiplicity analysis complete");
        check(msg, 1);
        free(pairs);
    }

    /* --- XOR4 -> XOR3 --- */
    {
        DelPair *pairs;
        int n_pairs = 0;
        int s, d, j, p;

        printf("\n  --- XOR4 -> XOR3 multiplicity ---\n");
        pairs = (DelPair *)malloc(
            (size_t)n_sol4 * 4 * sizeof(DelPair));

        for (s = 0; s < n_sol4; s++) {
            const int *sol = &sol4[s][0];
            long parent_key = make_key(sol, 4);
            for (d = 0; d < 4; d++) {
                int sub[3];
                long child_key;
                p = 0;
                for (j = 0; j < 4; j++) {
                    if (j == d) continue;
                    sub[p++] = sol[j];
                }
                child_key = make_key(sub, 3);
                if (find_key(ck3, nck3, child_key) >= 0) {
                    pairs[n_pairs].child = child_key;
                    pairs[n_pairs].parent = parent_key;
                    n_pairs++;
                }
            }
        }

        report_multiplicity("XOR4->XOR3", pairs, n_pairs, ck3, nck3);
        sprintf(msg, "XOR4->XOR3 multiplicity analysis complete");
        check(msg, 1);
        free(pairs);
    }

    /* --- XOR3 -> XOR2 --- */
    {
        DelPair *pairs;
        int n_pairs = 0;
        int s, d, j, p;

        printf("\n  --- XOR3 -> XOR2 multiplicity ---\n");
        pairs = (DelPair *)malloc(
            (size_t)n_sol3 * 3 * sizeof(DelPair));

        for (s = 0; s < n_sol3; s++) {
            const int *sol = &sol3[s][0];
            long parent_key = make_key(sol, 3);
            for (d = 0; d < 3; d++) {
                int sub[2];
                long child_key;
                p = 0;
                for (j = 0; j < 3; j++) {
                    if (j == d) continue;
                    sub[p++] = sol[j];
                }
                child_key = make_key(sub, 2);
                if (find_key(ck2, nck2, child_key) >= 0) {
                    pairs[n_pairs].child = child_key;
                    pairs[n_pairs].parent = parent_key;
                    n_pairs++;
                }
            }
        }

        report_multiplicity("XOR3->XOR2", pairs, n_pairs, ck2, nck2);
        sprintf(msg, "XOR3->XOR2 multiplicity analysis complete");
        check(msg, 1);
        free(pairs);
    }
}

/* ================================================================
 * Part E helpers
 * ================================================================ */

/* Decode canonical key back to sorted catalog indices */
static void decode_key(long key, int *indices, int n) {
    int i;
    for (i = 0; i < n; i++) {
        indices[i] = (int)(key % 100);
        key /= 100;
    }
}

/* 6-input truth table using exact Z[zeta_8] arithmetic.
 * Returns unsigned long (64-bit on macOS LP64). */
static unsigned long compute_tt6_exact(const int *w_idx, int k) {
    unsigned long tt = 0;
    int idx;
    for (idx = 1; idx < 64; idx++) {
        Cyc8 ez = cyc8_zero();
        int bit;
        for (bit = 0; bit < 6; bit++) {
            if ((idx >> (5 - bit)) & 1)
                ez = cyc8_add(ez, cat_exact[w_idx[bit]]);
        }
        if (sector_classify(cyc8_to_cx(ez), k))
            tt |= (1UL << (unsigned)idx);
    }
    return tt;
}

/* Expected XOR6 truth table */
static unsigned long xor_tt6(void) {
    unsigned long tt = 0;
    int idx;
    for (idx = 1; idx < 64; idx++) {
        if (popcount(idx) % 2 == 1)
            tt |= (1UL << (unsigned)idx);
    }
    return tt;
}

static int popcount_ul(unsigned long x) {
    int c = 0;
    while (x) { c += (int)(x & 1UL); x >>= 1; }
    return c;
}

/* ================================================================
 * PART E: The Wall Under a Microscope
 *
 * E1: Extension attempt census (27 XOR5 sets x 83 class-1 values)
 * E2: Error pattern analysis for near-misses
 * E3: Orphan vocabulary (extensible vs orphan XOR4 sets)
 * ================================================================ */

static void part_e_wall_microscope(void) {
    int K = 15;
    char msg[256];
    unsigned long target_tt;
    int i, j;

    printf("\n=== PART E: The Wall Under a Microscope ===\n\n");

    /* --- Print the 27 distinct XOR5 weight sets --- */
    {
        int used[MAX_DISTINCT];
        int n_unique = 0;

        memset(used, 0, sizeof(used));
        printf("  The %d distinct XOR5 weight sets:\n", nck5);
        for (i = 0; i < nck5; i++) {
            int indices[5];
            decode_key(ck5[i], indices, 5);
            printf("    set %2d: {%2d,%2d,%2d,%2d,%2d}"
                   "  oct{%d,%d,%d,%d,%d}\n",
                   i,
                   indices[0], indices[1], indices[2],
                   indices[3], indices[4],
                   cat_octant[indices[0]], cat_octant[indices[1]],
                   cat_octant[indices[2]], cat_octant[indices[3]],
                   cat_octant[indices[4]]);
            for (j = 0; j < 5; j++) used[indices[j]] = 1;
        }
        for (i = 0; i < cat_size; i++) n_unique += used[i];
        printf("\n  Unique catalog values in XOR5: %d / %d class-1\n\n",
               n_unique, n_class1);
        sprintf(msg, "XOR5 uses %d of %d class-1 catalog values",
                n_unique, n_class1);
        check(msg, 1);
    }

    /* --- E1: Extension Attempt Census --- */
    target_tt = xor_tt6();
    printf("  Expected XOR6 truth table: 0x%lX\n", target_tt);
    printf("  XOR6 has %d ones out of 63 non-zero inputs\n\n",
           popcount_ul(target_tt));

    {
        int hamming_histo[64];
        int min_hamming = 63;
        int n_attempts = 0;
        int near_threshold = 8;
        int n_near = 0;
        int s, e;

        /* Near-miss storage (max 2500 = 27*83 + spare) */
        int nm_set[2500], nm_ext[2500], nm_ham[2500];
        unsigned long nm_errors[2500], nm_tt[2500];

        memset(hamming_histo, 0, sizeof(hamming_histo));

        for (s = 0; s < nck5; s++) {
            int base[5];
            decode_key(ck5[s], base, 5);
            for (e = 0; e < n_class1; e++) {
                int w6[6];
                unsigned long tt, errors;
                int ham;
                int ext_idx = class1_idx[e];

                memcpy(w6, base, 5 * sizeof(int));
                w6[5] = ext_idx;

                tt = compute_tt6_exact(w6, K);
                errors = tt ^ target_tt;
                ham = popcount_ul(errors);

                if (ham < 64) hamming_histo[ham]++;
                if (ham < min_hamming) min_hamming = ham;
                n_attempts++;

                if (ham <= near_threshold && n_near < 2500) {
                    nm_set[n_near] = s;
                    nm_ext[n_near] = ext_idx;
                    nm_ham[n_near] = ham;
                    nm_errors[n_near] = errors;
                    nm_tt[n_near] = tt;
                    n_near++;
                }
            }
        }

        printf("  --- E1: Extension Census ---\n");
        printf("    %d attempts (%d sets x %d class-1 values)\n",
               n_attempts, nck5, n_class1);
        printf("    Minimum Hamming distance from XOR6: %d\n",
               min_hamming);
        printf("    Hamming distance distribution:\n");
        for (i = 0; i < 64; i++) {
            if (hamming_histo[i] > 0)
                printf("      d=%2d: %d attempts\n",
                       i, hamming_histo[i]);
        }

        {
            int n_near4 = 0;
            for (i = 0; i <= 4; i++) n_near4 += hamming_histo[i];
            printf("    Near misses (Hamming <= 4): %d\n", n_near4);
            sprintf(msg, "Minimum Hamming distance to XOR6 = %d",
                    min_hamming);
            check(msg, 1);
            sprintf(msg, "Near misses (Hamming <= 4): %d", n_near4);
            check(msg, 1);
        }

        /* --- E2: Error Pattern Analysis --- */
        printf("\n  --- E2: Near-Miss Error Patterns"
               " (Hamming <= %d) ---\n", near_threshold);

        if (n_near == 0) {
            printf("    No near misses found.\n");
        } else {
            unsigned long distinct_tt[2500];
            int n_distinct_tt = 0;
            int error_weight_histo[7];

            memset(error_weight_histo, 0, sizeof(error_weight_histo));

            printf("    %d near-miss attempts:\n", n_near);
            for (i = 0; i < n_near && i < 20; i++) {
                printf("      set %2d + val %2d (oct %d):"
                       " Hamming=%d  errors=0x%lX\n",
                       nm_set[i], nm_ext[i],
                       cat_octant[nm_ext[i]],
                       nm_ham[i], nm_errors[i]);
            }
            if (n_near > 20)
                printf("      ... (%d more)\n", n_near - 20);

            /* Count distinct near-miss TTs */
            for (i = 0; i < n_near; i++) {
                int found = 0;
                for (j = 0; j < n_distinct_tt; j++) {
                    if (nm_tt[i] == distinct_tt[j]) {
                        found = 1; break;
                    }
                }
                if (!found && n_distinct_tt < 2500)
                    distinct_tt[n_distinct_tt++] = nm_tt[i];
            }
            printf("    Distinct near-miss truth tables: %d\n",
                   n_distinct_tt);

            /* Errors by input Hamming weight */
            for (i = 0; i < n_near; i++) {
                unsigned long err = nm_errors[i];
                int idx;
                for (idx = 1; idx < 64; idx++) {
                    if ((err >> (unsigned)idx) & 1UL) {
                        int w = popcount(idx);
                        if (w <= 6) error_weight_histo[w]++;
                    }
                }
            }
            printf("    Errors by input Hamming weight:\n");
            for (i = 0; i <= 6; i++) {
                if (error_weight_histo[i] > 0)
                    printf("      weight %d: %d errors"
                           " across all near-misses\n",
                           i, error_weight_histo[i]);
            }

            /* Show wrong inputs for closest misses */
            printf("    Closest misses (Hamming = %d):\n",
                   min_hamming);
            for (i = 0; i < n_near; i++) {
                if (nm_ham[i] == min_hamming) {
                    unsigned long err = nm_errors[i];
                    int idx, first;
                    printf("      set %2d + val %2d:"
                           " wrong inputs = {",
                           nm_set[i], nm_ext[i]);
                    first = 1;
                    for (idx = 1; idx < 64; idx++) {
                        if ((err >> (unsigned)idx) & 1UL) {
                            if (!first) printf(",");
                            printf("%d(w%d)", idx,
                                   popcount(idx));
                            first = 0;
                        }
                    }
                    printf("}\n");
                }
            }
        }
    }

    /* --- E3: Orphan Vocabulary --- */
    {
        int *hit4;
        int ext_used[MAX_DISTINCT], orph_used[MAX_DISTINCT];
        int n_ext_vals = 0, n_orph_vals = 0;
        int n_only_ext = 0, n_only_orph = 0, n_both = 0;
        int s, d, p;

        printf("\n  --- E3: Orphan Vocabulary ---\n");

        /* Rebuild hit array for XOR4 keys */
        hit4 = (int *)calloc((size_t)nck4, sizeof(int));
        for (s = 0; s < n_sol5; s++) {
            const int *sol = &sol5[s][0];
            for (d = 0; d < 5; d++) {
                int sub[4];
                long key;
                int idx;
                p = 0;
                for (j = 0; j < 5; j++) {
                    if (j == d) continue;
                    sub[p++] = sol[j];
                }
                key = make_key(sub, 4);
                idx = find_key(ck4, nck4, key);
                if (idx >= 0) hit4[idx] = 1;
            }
        }

        memset(ext_used, 0, sizeof(ext_used));
        memset(orph_used, 0, sizeof(orph_used));

        for (i = 0; i < nck4; i++) {
            int indices[4];
            decode_key(ck4[i], indices, 4);
            if (hit4[i]) {
                for (j = 0; j < 4; j++) ext_used[indices[j]] = 1;
            } else {
                for (j = 0; j < 4; j++) orph_used[indices[j]] = 1;
            }
        }

        for (i = 0; i < cat_size; i++) {
            n_ext_vals += ext_used[i];
            n_orph_vals += orph_used[i];
            if (ext_used[i] && !orph_used[i]) n_only_ext++;
            if (!ext_used[i] && orph_used[i]) n_only_orph++;
            if (ext_used[i] && orph_used[i]) n_both++;
        }

        printf("    Extensible XOR4 sets (129): use %d"
               " distinct catalog values\n", n_ext_vals);
        printf("    Orphan XOR4 sets (544): use %d"
               " distinct catalog values\n", n_orph_vals);
        printf("    Values only in extensible: %d\n", n_only_ext);
        printf("    Values only in orphans: %d\n", n_only_orph);
        printf("    Values in both: %d\n", n_both);

        if (n_only_ext > 0) {
            printf("    Extensible-only values:");
            for (i = 0; i < cat_size; i++)
                if (ext_used[i] && !orph_used[i])
                    printf(" %d(oct%d)", i, cat_octant[i]);
            printf("\n");
        }
        if (n_only_orph > 0) {
            printf("    Orphan-only values:");
            for (i = 0; i < cat_size; i++)
                if (!ext_used[i] && orph_used[i])
                    printf(" %d(oct%d)", i, cat_octant[i]);
            printf("\n");
        }

        sprintf(msg,
                "Extensible XOR4 use %d values, orphans use %d",
                n_ext_vals, n_orph_vals);
        check(msg, 1);

        free(hit4);
    }
}

/* ================================================================
 * PART E4: Broken Pair Anatomy
 *
 * For each d=4 near-miss, which weight-2 input is wrong?
 * Does the broken pair always involve the extension weight?
 * Are all d=4 cases self-extensions (6th = duplicate of one of 5)?
 * ================================================================ */

static void part_e4_broken_pairs(void) {
    int K = 15;
    unsigned long target_tt = xor_tt6();
    char msg[256];
    int s, e, i;

    int n_d4 = 0;
    int bp_set[64], bp_ext[64], bp_a[64], bp_b[64];
    int all_involve_ext = 1;
    int n_self_extend = 0;

    /* d=5..8: weight-2 error counts */
    int w2_by_d[9][16];

    printf("\n  --- E4: Broken Pair Anatomy ---\n\n");
    memset(w2_by_d, 0, sizeof(w2_by_d));

    for (s = 0; s < nck5; s++) {
        int base[5];
        decode_key(ck5[s], base, 5);
        for (e = 0; e < n_class1; e++) {
            int w6[6];
            unsigned long tt, errors;
            int ham, ext_idx = class1_idx[e];

            memcpy(w6, base, 5 * sizeof(int));
            w6[5] = ext_idx;

            tt = compute_tt6_exact(w6, K);
            errors = tt ^ target_tt;
            ham = popcount_ul(errors);

            if (ham == 4 && n_d4 < 64) {
                int idx, found = 0;
                int is_self = 0;
                for (i = 0; i < 5; i++) {
                    if (base[i] == ext_idx) {
                        is_self = 1; break;
                    }
                }
                if (is_self) n_self_extend++;

                for (idx = 1; idx < 64 && !found; idx++) {
                    if (((errors >> (unsigned)idx) & 1UL)
                        && popcount(idx) == 2) {
                        int bit, wa = -1, wb = -1;
                        for (bit = 0; bit < 6; bit++) {
                            if ((idx >> (5 - bit)) & 1) {
                                if (wa < 0) wa = bit;
                                else wb = bit;
                            }
                        }
                        bp_set[n_d4] = s;
                        bp_ext[n_d4] = ext_idx;
                        bp_a[n_d4] = wa;
                        bp_b[n_d4] = wb;
                        if (wb != 5) all_involve_ext = 0;
                        n_d4++;
                        found = 1;
                    }
                }
            }

            if (ham >= 5 && ham <= 8) {
                int n_w2 = 0, idx;
                for (idx = 1; idx < 64; idx++) {
                    if (((errors >> (unsigned)idx) & 1UL)
                        && popcount(idx) == 2)
                        n_w2++;
                }
                if (n_w2 < 16) w2_by_d[ham][n_w2]++;
            }
        }
    }

    /* Report d=4 broken pairs */
    printf("  d=4 near-misses: %d cases\n\n", n_d4);
    printf("  Broken pairs (weight positions):\n");
    for (i = 0; i < n_d4; i++) {
        printf("    set %2d + val %2d: pair (%d,%d)%s\n",
               bp_set[i], bp_ext[i], bp_a[i], bp_b[i],
               bp_b[i] == 5 ? " [ext]" : "");
    }

    /* Distinct pairs + clustering */
    {
        int pair_seen[6][6], n_distinct = 0;
        int set_count[30], n_active = 0;
        memset(pair_seen, 0, sizeof(pair_seen));
        memset(set_count, 0, sizeof(set_count));
        for (i = 0; i < n_d4; i++) {
            if (!pair_seen[bp_a[i]][bp_b[i]]) {
                pair_seen[bp_a[i]][bp_b[i]] = 1;
                n_distinct++;
            }
            if (bp_set[i] < 30) set_count[bp_set[i]]++;
        }
        printf("\n  Distinct broken pairs: %d / 15 possible\n",
               n_distinct);
        printf("  All involve extension (pos 5): %s\n",
               all_involve_ext ? "YES" : "no");
        printf("  Self-extensions (6th = dup): %d/%d\n",
               n_self_extend, n_d4);

        printf("\n  Cases per XOR5 set:\n");
        for (i = 0; i < nck5; i++) {
            if (set_count[i] > 0) {
                n_active++;
                printf("    set %2d: %d cases\n",
                       i, set_count[i]);
            }
        }
        printf("  Active sets: %d / %d\n", n_active, nck5);

        sprintf(msg,
                "All d=4 broken pairs involve extension (pos 5)");
        check(msg, all_involve_ext);
        sprintf(msg, "d=4 self-extensions: %d/%d", n_self_extend, n_d4);
        check(msg, 1);
    }

    /* d=5..8 weight-2 error counts */
    {
        int d;
        printf("\n  Weight-2 errors by Hamming distance:\n");
        for (d = 5; d <= 8; d++) {
            printf("    d=%d: ", d);
            for (i = 0; i < 16; i++) {
                if (w2_by_d[d][i] > 0)
                    printf("%d_broken:%d  ", i, w2_by_d[d][i]);
            }
            printf("\n");
        }
    }
}

/* ================================================================
 * PART E5: Pairwise Sum Census
 *
 * For a representative d=4 near-miss, compute all C(6,2)=15
 * pairwise sums as Cyc8 values, show their k=15 sectors,
 * and identify which sum(s) are in the wrong class.
 * ================================================================ */

static void part_e5_pairwise_census(void) {
    int K = 15;
    unsigned long target_tt = xor_tt6();
    char msg[256];
    int i, j;
    int w6[6];
    int base[5];
    int n_wrong = 0, n_correct = 0;

    printf("\n  --- E5: Pairwise Sum Census ---\n\n");

    /* Find first d=4 case */
    {
        int s, e, found = 0;
        for (s = 0; s < nck5 && !found; s++) {
            decode_key(ck5[s], base, 5);
            for (e = 0; e < n_class1 && !found; e++) {
                int ext_idx = class1_idx[e];
                unsigned long tt, errors;
                int ham;
                memcpy(w6, base, 5 * sizeof(int));
                w6[5] = ext_idx;
                tt = compute_tt6_exact(w6, K);
                errors = tt ^ target_tt;
                ham = popcount_ul(errors);
                if (ham == 4) found = 1;
            }
        }
    }

    printf("  Representative d=4 case:\n  Weights: {");
    for (i = 0; i < 6; i++) {
        if (i > 0) printf(", ");
        printf("%d(oct%d)", w6[i], cat_octant[w6[i]]);
    }
    printf("}\n\n");

    printf("  Exact Cyc8 values:\n");
    for (i = 0; i < 6; i++) {
        Cyc8 v = cat_exact[w6[i]];
        printf("    w[%d] = cat[%2d] = (%2ld,%2ld,%2ld,%2ld)\n",
               i, w6[i], v.a, v.b, v.c, v.d);
    }

    /* All 15 pairwise sums */
    printf("\n  All 15 pairwise sums at k=%d:\n", K);
    printf("  %3s %3s  %14s  %6s  %5s  %s\n",
           "i", "j", "(a,b,c,d)", "sec", "cls", "");

    for (i = 0; i < 6; i++) {
        for (j = i + 1; j < 6; j++) {
            Cyc8 sum = cyc8_add(cat_exact[w6[i]],
                                cat_exact[w6[j]]);
            Cx sc = cyc8_to_cx(sum);
            double angle, sw;
            int sec, cls;
            double mag = cx_abs(sc);

            if (mag < 1e-15) {
                sec = 0; cls = 0;
            } else {
                angle = atan2(sc.im, sc.re);
                if (angle < 0.0) angle += 2.0 * M_PI;
                sw = 2.0 * M_PI / (double)K;
                sec = (int)(angle / sw);
                if (sec >= K) sec = K - 1;
                cls = sec % 2;
            }

            /* XOR6 expects class 0 for pairs (even weight) */
            printf("  w[%d]+w[%d]  (%3ld,%3ld,%3ld,%3ld)  sec=%2d"
                   "  cls=%d  %s\n",
                   i, j, sum.a, sum.b, sum.c, sum.d,
                   sec, cls,
                   cls == 0 ? "OK" : "*** WRONG ***");
            if (cls == 0) n_correct++;
            else n_wrong++;
        }
    }

    printf("\n  Correct pairs: %d/15\n", n_correct);
    printf("  Wrong pairs: %d/15\n", n_wrong);

    /* Detail on wrong pair(s) */
    if (n_wrong > 0) {
        printf("\n  Broken pair detail:\n");
        for (i = 0; i < 6; i++) {
            for (j = i + 1; j < 6; j++) {
                Cyc8 sum = cyc8_add(cat_exact[w6[i]],
                                    cat_exact[w6[j]]);
                Cx sc = cyc8_to_cx(sum);
                double angle, sw, deg, lo, hi;
                int sec;
                double mag = cx_abs(sc);
                if (mag < 1e-15) continue;
                angle = atan2(sc.im, sc.re);
                if (angle < 0.0) angle += 2.0 * M_PI;
                sw = 2.0 * M_PI / (double)K;
                sec = (int)(angle / sw);
                if (sec >= K) sec = K - 1;
                if (sec % 2 == 1) {
                    deg = angle * 180.0 / M_PI;
                    lo = (double)sec * 360.0 / (double)K;
                    hi = (double)(sec + 1) * 360.0 / (double)K;
                    printf("    w[%d]+w[%d] = (%ld,%ld,%ld,%ld)\n",
                           i, j,
                           sum.a, sum.b, sum.c, sum.d);
                    printf("    angle = %.2f deg, sector %d"
                           " [%.1f, %.1f) deg\n",
                           deg, sec, lo, hi);
                    printf("    w[%d] = cat[%d], w[%d] = cat[%d]"
                           " (same value = %s)\n",
                           i, w6[i], j, w6[j],
                           w6[i] == w6[j] ? "YES" : "no");
                }
            }
        }
    }

    sprintf(msg, "Exactly %d wrong pairwise sum(s) in representative",
            n_wrong);
    check(msg, n_wrong > 0);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 64: Parity Matroid Recursion\n");
    printf("===========================================\n");

    printf("\n  Building bracket catalog...\n");
    build_catalog();
    printf("  Standard catalog: %d distinct values\n", cat_size);

    printf("  Collecting class-1 values at k=15...\n");
    collect_class1(15);
    printf("  Class-1 values: %d\n", n_class1);

    part_a_deletion();
    part_b_contraction();
    part_c_coverage();
    part_d_multiplicity();
    part_e_wall_microscope();
    part_e4_broken_pairs();
    part_e5_pairwise_census();

    printf("\n===========================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("===========================================\n");

    return n_fail > 0 ? 1 : 0;
}
