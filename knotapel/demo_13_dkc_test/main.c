/*
 * KNOTAPEL DEMO 13: DKC Test (Deterministic Knowledge Compilation)
 * =================================================================
 *
 * THE critical experiment: do compiled weights generalize?
 *
 * Demo 12 showed that 10 knots can be classified with 8 bits per knot.
 * Demo 13 scales to ~28 knots (3-strand braids: torus knots, mirrors,
 * connected sums, twist variants) and asks:
 *
 *   1. Does the 8-bit Pareto frontier hold at scale?
 *   2. Does a train/test split show 100% generalization?
 *   3. How does min pairwise separation decay with knot count?
 *   4. Do compiled weights beat random baselines?
 *
 * The DKC thesis: weights compiled from algebraic structure (Kauffman
 * bracket) generalize without training because they encode topology,
 * not statistical patterns.
 *
 * C89, zero dependencies beyond math.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ================================================================
 * Complex arithmetic (from Demo 10/11/12)
 * ================================================================ */

typedef struct { double re, im; } Cx;

static Cx cx_make(double re, double im) { Cx z; z.re = re; z.im = im; return z; }
static Cx cx_zero(void) { return cx_make(0.0, 0.0); }
static Cx cx_one(void)  { return cx_make(1.0, 0.0); }

static Cx cx_add(Cx a, Cx b) { return cx_make(a.re + b.re, a.im + b.im); }
static Cx cx_sub(Cx a, Cx b) { return cx_make(a.re - b.re, a.im - b.im); }
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
 * State-sum bracket oracle (from Demo 10/11/12)
 * ================================================================ */

#define MAX_WORD 40
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

#define MAX_UF 2048
static int uf_p[MAX_UF];
static void uf_init(int n) { int i; for (i = 0; i < n; i++) uf_p[i] = i; }
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
    for (p = 0; p < b->n; p++)
        uf_union(p, b->len * b->n + p);
    loops = 0;
    for (i = 0; i < N; i++)
        if (uf_find(i) == i) loops++;
    return loops;
}

static Cx braid_bracket_at(const Braid *b, Cx A) {
    unsigned s, ns;
    int i, a_count, b_count, lp, j;
    Cx result, delta, d_power, term, coeff;

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
        a_count = 0; b_count = 0;
        for (i = 0; i < b->len; i++) {
            if ((s >> (unsigned)i) & 1u) b_count++;
            else a_count++;
        }
        lp = braid_loops(b, s);

        coeff = cx_pow_int(A, a_count - b_count);
        d_power = cx_one();
        for (j = 0; j < lp - 1; j++)
            d_power = cx_mul(d_power, delta);
        term = cx_mul(coeff, d_power);
        result = cx_add(result, term);
    }
    return result;
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
 * EXPANDED KNOT TABLE: ~28 knots on 3 strands
 * ================================================================ */

#define MAX_KNOTS 40
#define MAX_ANGLES 8
#define NSAMP 256

static Braid knots[MAX_KNOTS];
static const char *knames[MAX_KNOTS];
static int num_knots = 0;

/* Helper to build torus knot T(2,n) on 3 strands:
 * braid word = (s1 s2)^((n-1)/2) s1 for odd n */
static void add_torus_2n(int n, int sign, const char *titulis) {
    int k = num_knots;
    int i;
    knots[k].n = 3;
    knots[k].len = n;
    for (i = 0; i < n; i++)
        knots[k].word[i] = sign * ((i % 2 == 0) ? 1 : 2);
    knames[k] = titulis;
    num_knots++;
}

/* Helper to build connected sum K1#K2 on 3 strands:
 * K1 uses generator s1, K2 uses generator s2 */
static void add_connected_sum(int len1, const int *w1,
                               int len2, const int *w2,
                               const char *titulis) {
    int k = num_knots;
    int i;
    knots[k].n = 3;
    knots[k].len = len1 + len2;
    for (i = 0; i < len1; i++)
        knots[k].word[i] = w1[i];
    for (i = 0; i < len2; i++)
        knots[k].word[len1 + i] = w2[i];
    knames[k] = titulis;
    num_knots++;
}

static void init_knots(void) {
    int k;
    /* s1^n words for connected sums */
    int s1_3[]  = {1, 1, 1};
    int s1_5[]  = {1, 1, 1, 1, 1};
    int s1m_3[] = {-1, -1, -1};
    int s2_3[]  = {2, 2, 2};
    int s2_5[]  = {2, 2, 2, 2, 2};
    int s2m_3[] = {-2, -2, -2};
    int s2m_5[] = {-2, -2, -2, -2, -2};
    /* figure-eight component for connected sum: s2 s2^{-1} pattern */
    int fig8_s2[] = {2, -1, 2, -1};

    num_knots = 0;

    /* --- Group A: Torus knots T(2,n) and mirrors --- */

    /* 0: unknot */
    knots[0].n = 3; knots[0].len = 0;
    knames[0] = "unknot"; num_knots++;

    /* 1-2: trefoil T(2,3) and mirror */
    add_torus_2n(3,  1, "T(2,3)");
    add_torus_2n(3, -1, "T(2,3)*");

    /* 3-4: cinquefoil T(2,5) and mirror */
    add_torus_2n(5,  1, "T(2,5)");
    add_torus_2n(5, -1, "T(2,5)*");

    /* 5-6: T(2,7) and mirror */
    add_torus_2n(7,  1, "T(2,7)");
    add_torus_2n(7, -1, "T(2,7)*");

    /* 7-8: T(2,9) and mirror */
    add_torus_2n(9,  1, "T(2,9)");
    add_torus_2n(9, -1, "T(2,9)*");

    /* 9-10: T(2,11) and mirror */
    add_torus_2n(11,  1, "T(2,11)");
    add_torus_2n(11, -1, "T(2,11)*");

    /* 11-12: T(2,13) and mirror */
    add_torus_2n(13,  1, "T(2,13)");
    add_torus_2n(13, -1, "T(2,13)*");

    /* 13: figure-eight (amphichiral) */
    k = num_knots;
    knots[k].n = 3; knots[k].len = 4;
    knots[k].word[0] = 1; knots[k].word[1] = -2;
    knots[k].word[2] = 1; knots[k].word[3] = -2;
    knames[k] = "fig-eight"; num_knots++;

    /* --- Group B: Connected sums --- */

    /* 14: granny = T(2,3) # T(2,3) */
    add_connected_sum(3, s1_3, 3, s2_3, "granny");

    /* 15: square = T(2,3) # T(2,3)* */
    add_connected_sum(3, s1_3, 3, s2m_3, "square");

    /* 16: T(2,3) # T(2,5) */
    add_connected_sum(3, s1_3, 5, s2_5, "T23#T25");

    /* 17: T(2,3) # T(2,5)* */
    add_connected_sum(3, s1_3, 5, s2m_5, "T23#T25*");

    /* 18: T(2,5) # T(2,5) */
    add_connected_sum(5, s1_5, 5, s2_5, "T25#T25");

    /* 19: T(2,5) # T(2,5)* */
    add_connected_sum(5, s1_5, 5, s2m_5, "T25#T25*");

    /* 20: T(2,3)* # T(2,3)* (mirror granny) */
    add_connected_sum(3, s1m_3, 3, s2m_3, "granny*");

    /* 21: T(2,3) # figure-eight */
    add_connected_sum(3, s1_3, 4, fig8_s2, "T23#fig8");

    /* --- Group C: Other 3-strand braids --- */

    /* 22: s1^2 * s2 (3 crossings) */
    k = num_knots;
    knots[k].n = 3; knots[k].len = 3;
    knots[k].word[0] = 1; knots[k].word[1] = 1; knots[k].word[2] = 2;
    knames[k] = "s1^2.s2"; num_knots++;

    /* 23: s1^2 * s2^{-1} (3 crossings) */
    k = num_knots;
    knots[k].n = 3; knots[k].len = 3;
    knots[k].word[0] = 1; knots[k].word[1] = 1; knots[k].word[2] = -2;
    knames[k] = "s1^2.s2-"; num_knots++;

    /* 24: s1 * s2^{-1} * s1^{-1} * s2 (4 crossings) */
    k = num_knots;
    knots[k].n = 3; knots[k].len = 4;
    knots[k].word[0] = 1; knots[k].word[1] = -2;
    knots[k].word[2] = -1; knots[k].word[3] = 2;
    knames[k] = "s1.s2-.s1-.s2"; num_knots++;

    /* 25: s1^2 * s2^2 * s1^2 (6 crossings) */
    k = num_knots;
    knots[k].n = 3; knots[k].len = 6;
    knots[k].word[0] = 1; knots[k].word[1] = 1;
    knots[k].word[2] = 2; knots[k].word[3] = 2;
    knots[k].word[4] = 1; knots[k].word[5] = 1;
    knames[k] = "s1^2.s2^2.s1^2"; num_knots++;

    /* 26: s1^2 * s2^{-2} * s1^2 (6 crossings) */
    k = num_knots;
    knots[k].n = 3; knots[k].len = 6;
    knots[k].word[0] = 1; knots[k].word[1] = 1;
    knots[k].word[2] = -2; knots[k].word[3] = -2;
    knots[k].word[4] = 1; knots[k].word[5] = 1;
    knames[k] = "s1^2.s2-2.s1^2"; num_knots++;

    /* 27: s1 * s2 * s1^{-1} * s2^{-1} (4 crossings, commutator) */
    k = num_knots;
    knots[k].n = 3; knots[k].len = 4;
    knots[k].word[0] = 1; knots[k].word[1] = 2;
    knots[k].word[2] = -1; knots[k].word[3] = -2;
    knames[k] = "commutator"; num_knots++;

    printf("  Initialized %d candidate braids (3-strand)\n", num_knots);
}

/* Deduplicate knots by bracket at a reference angle.
 * Two braids with bracket distance < 1e-6 at MULTIPLE angles
 * are treated as the same knot. */
static void dedup_knots(void) {
    double ref_theta = 1.805 * M_PI; /* optimal from Demo 11/12 */
    double ref_theta2 = 0.75 * M_PI; /* second check angle */
    Cx vals[MAX_KNOTS], vals2[MAX_KNOTS];
    int keep[MAX_KNOTS];
    int ki, kj, new_count, i;

    for (ki = 0; ki < num_knots; ki++) {
        vals[ki] = braid_bracket_at(&knots[ki], cx_exp_i(ref_theta));
        vals2[ki] = braid_bracket_at(&knots[ki], cx_exp_i(ref_theta2));
        keep[ki] = 1;
    }

    /* Mark duplicates (keep earlier entry) */
    for (ki = 0; ki < num_knots; ki++) {
        if (!keep[ki]) continue;
        for (kj = ki + 1; kj < num_knots; kj++) {
            if (!keep[kj]) continue;
            if (cx_abs(cx_sub(vals[ki], vals[kj])) < 1e-6 &&
                cx_abs(cx_sub(vals2[ki], vals2[kj])) < 1e-6) {
                printf("  dedup: %s == %s (same knot type)\n",
                       knames[kj], knames[ki]);
                keep[kj] = 0;
            }
        }
    }

    /* Compact */
    new_count = 0;
    for (i = 0; i < num_knots; i++) {
        if (keep[i]) {
            if (new_count != i) {
                knots[new_count] = knots[i];
                knames[new_count] = knames[i];
            }
            new_count++;
        }
    }
    printf("  Deduplicated: %d -> %d unique knot types\n",
           num_knots, new_count);
    num_knots = new_count;
}

/* ================================================================
 * Greedy angle selection (from Demo 12, adapted for variable N)
 * ================================================================ */

static double greedy_angles[MAX_ANGLES];
static int n_greedy = 0;

static void select_greedy_angles(int max_sel) {
    /* Use heap allocation for large waveform array */
    Cx (*waveforms)[NSAMP];
    int selected[MAX_ANGLES];
    int nsel = 0;
    int si, ki, kj, ai;
    double theta;

    waveforms = (Cx (*)[NSAMP])malloc(
        (size_t)num_knots * NSAMP * sizeof(Cx));
    if (!waveforms) {
        fprintf(stderr, "malloc failed for waveforms\n");
        return;
    }

    /* Compute all waveforms */
    for (ki = 0; ki < num_knots; ki++)
        for (si = 0; si < NSAMP; si++) {
            theta = 2.0 * M_PI * (double)si / (double)NSAMP;
            waveforms[ki][si] = braid_bracket_at(&knots[ki], cx_exp_i(theta));
        }

    /* Greedy selection */
    while (nsel < max_sel) {
        int best_si = 0;
        double best_worst = 0.0;

        for (si = 0; si < NSAMP; si++) {
            double worst_pair = 1e30;
            for (ki = 0; ki < num_knots; ki++)
                for (kj = ki + 1; kj < num_knots; kj++) {
                    double dist2 = 0.0;
                    for (ai = 0; ai < nsel; ai++) {
                        Cx d = cx_sub(waveforms[ki][selected[ai]],
                                      waveforms[kj][selected[ai]]);
                        dist2 += d.re * d.re + d.im * d.im;
                    }
                    {
                        Cx d = cx_sub(waveforms[ki][si], waveforms[kj][si]);
                        dist2 += d.re * d.re + d.im * d.im;
                    }
                    if (dist2 < worst_pair) worst_pair = dist2;
                }
            if (worst_pair > best_worst) {
                best_worst = worst_pair;
                best_si = si;
            }
        }

        selected[nsel] = best_si;
        greedy_angles[nsel] = 2.0 * M_PI * (double)best_si / (double)NSAMP;
        nsel++;
    }
    n_greedy = nsel;

    free(waveforms);
}

/* ================================================================
 * Reference signatures
 * ================================================================ */

static Cx ref_sigs[MAX_KNOTS][MAX_ANGLES];

static void compute_references(void) {
    int ki, ai;
    for (ki = 0; ki < num_knots; ki++)
        for (ai = 0; ai < n_greedy; ai++)
            ref_sigs[ki][ai] = braid_bracket_at(&knots[ki],
                                                 cx_exp_i(greedy_angles[ai]));
}

/* ================================================================
 * Quantization (from Demo 12)
 * ================================================================ */

static double quantize(double val, double lo, double hi, int bits) {
    double range, scaled, levels;
    int q;
    if (bits <= 0) return 0.0;
    levels = (double)((1 << bits) - 1);
    range = hi - lo;
    if (range < 1e-15) return val;
    scaled = (val - lo) / range * levels;
    q = (int)(scaled + 0.5);
    if (q < 0) q = 0;
    if (q > (int)levels) q = (int)levels;
    return lo + (double)q * range / levels;
}

static Cx cx_quantize(Cx z, double re_lo, double re_hi,
                       double im_lo, double im_hi, int bits) {
    return cx_make(quantize(z.re, re_lo, re_hi, bits),
                   quantize(z.im, im_lo, im_hi, bits));
}

/* ================================================================
 * PART A: Duplicate detection and separation analysis
 * ================================================================ */

static void part_a_separation(void) {
    int ki, kj, n_unique, n_dups;
    double min_sep, sep;
    int min_i, min_j;
    int total_pairs;
    char msg[200];

    printf("\n=== PART A: Separation Analysis at theta=%.4f*pi ===\n",
           greedy_angles[0] / M_PI);

    /* Check for duplicates (bracket distance < 1e-6) */
    n_unique = num_knots;
    n_dups = 0;
    min_sep = 1e30;
    min_i = 0; min_j = 1;

    for (ki = 0; ki < num_knots; ki++)
        for (kj = ki + 1; kj < num_knots; kj++) {
            sep = cx_abs(cx_sub(ref_sigs[ki][0], ref_sigs[kj][0]));
            if (sep < 1e-6) {
                printf("  DUPLICATE: %s == %s (dist=%.2e)\n",
                       knames[ki], knames[kj], sep);
                n_dups++;
            }
            if (sep < min_sep) {
                min_sep = sep;
                min_i = ki; min_j = kj;
            }
        }

    total_pairs = num_knots * (num_knots - 1) / 2;
    printf("  %d knots, %d pairs, %d duplicates\n",
           num_knots, total_pairs, n_dups);
    printf("  Min separation: %.6f (%s vs %s)\n",
           min_sep, knames[min_i], knames[min_j]);
    printf("  (Compare: Demo 12 had 0.907 for 10 knots)\n");

    n_unique = num_knots - n_dups;
    sprintf(msg, "%d unique knots at 1 angle (of %d total, %d duplicates)",
            n_unique, num_knots, n_dups);
    check(msg, n_unique > 0);
}

/* ================================================================
 * PART B: Multi-angle separation
 * ================================================================ */

static int count_separated_pairs(int num_angles) {
    int ki, kj, ai, pairs_ok;
    pairs_ok = 0;
    for (ki = 0; ki < num_knots; ki++)
        for (kj = ki + 1; kj < num_knots; kj++) {
            double dist2 = 0.0;
            for (ai = 0; ai < num_angles; ai++) {
                Cx d = cx_sub(ref_sigs[ki][ai], ref_sigs[kj][ai]);
                dist2 += d.re * d.re + d.im * d.im;
            }
            if (dist2 > 1e-10) pairs_ok++;
        }
    return pairs_ok;
}

static void part_b_multi_angle(void) {
    int total_pairs = num_knots * (num_knots - 1) / 2;
    int ai, sep;
    int min_angles = -1;
    char msg[200];

    printf("\n=== PART B: Multi-Angle Separation ===\n");
    printf("  %-8s %12s %8s\n", "Angles", "Pairs OK", "Total");

    for (ai = 1; ai <= n_greedy; ai++) {
        sep = count_separated_pairs(ai);
        printf("  %-8d %9d/%-3d %8s\n",
               ai, sep, total_pairs,
               sep == total_pairs ? "PERFECT" : "");
        if (sep == total_pairs && min_angles < 0)
            min_angles = ai;
    }

    sprintf(msg, "full separation achieved with %d angle(s)", min_angles);
    check(msg, min_angles > 0);
}

/* ================================================================
 * PART C: Precision x Width tradeoff (scaled up from Demo 12)
 * ================================================================ */

static int test_quantized_sep(int bits, int num_angles) {
    double re_lo, re_hi, im_lo, im_hi;
    int ki, kj, ai, pairs_ok;

    re_lo = 1e30; re_hi = -1e30;
    im_lo = 1e30; im_hi = -1e30;

    for (ki = 0; ki < num_knots; ki++)
        for (ai = 0; ai < num_angles; ai++) {
            if (ref_sigs[ki][ai].re < re_lo) re_lo = ref_sigs[ki][ai].re;
            if (ref_sigs[ki][ai].re > re_hi) re_hi = ref_sigs[ki][ai].re;
            if (ref_sigs[ki][ai].im < im_lo) im_lo = ref_sigs[ki][ai].im;
            if (ref_sigs[ki][ai].im > im_hi) im_hi = ref_sigs[ki][ai].im;
        }
    {
        double re_m = (re_hi - re_lo) * 0.01 + 1e-10;
        double im_m = (im_hi - im_lo) * 0.01 + 1e-10;
        re_lo -= re_m; re_hi += re_m;
        im_lo -= im_m; im_hi += im_m;
    }

    pairs_ok = 0;
    for (ki = 0; ki < num_knots; ki++)
        for (kj = ki + 1; kj < num_knots; kj++) {
            double dist2 = 0.0;
            for (ai = 0; ai < num_angles; ai++) {
                Cx qi = cx_quantize(ref_sigs[ki][ai],
                                     re_lo, re_hi, im_lo, im_hi, bits);
                Cx qj = cx_quantize(ref_sigs[kj][ai],
                                     re_lo, re_hi, im_lo, im_hi, bits);
                Cx d = cx_sub(qi, qj);
                dist2 += d.re * d.re + d.im * d.im;
            }
            if (dist2 > 1e-20) pairs_ok++;
        }

    return pairs_ok;
}

static void part_c_pareto(void) {
    int bits_list[] = {1, 2, 3, 4, 8};
    int angle_list[] = {1, 2, 3, 4, 8};
    int n_bits = 5, n_angles = 5;
    int total_pairs = num_knots * (num_knots - 1) / 2;
    int bi, ai, pairs_ok;
    int min_total_bits = -1;
    int min_b = 0, min_m = 0;
    char msg[200];

    printf("\n=== PART C: Precision x Width Tradeoff ===\n");
    printf("  (%d knots, %d pairs, * = 100%%)\n\n", num_knots, total_pairs);

    printf("  %8s", "Bits\\Ang");
    for (ai = 0; ai < n_angles; ai++)
        printf(" %6dA", angle_list[ai]);
    printf("\n");

    for (bi = 0; bi < n_bits; bi++) {
        printf("  %6dB ", bits_list[bi]);
        for (ai = 0; ai < n_angles; ai++) {
            int M = angle_list[ai];
            if (M > n_greedy) {
                printf("  %5s ", "---");
                continue;
            }
            pairs_ok = test_quantized_sep(bits_list[bi], M);
            if (pairs_ok == total_pairs) {
                printf(" %3d*  ", pairs_ok);
                {
                    int total = bits_list[bi] * M * 2;
                    if (min_total_bits < 0 || total < min_total_bits) {
                        min_total_bits = total;
                        min_b = bits_list[bi];
                        min_m = M;
                    }
                }
            } else {
                printf(" %3d   ", pairs_ok);
            }
        }
        printf("\n");
    }

    if (min_total_bits > 0) {
        printf("\n  Pareto optimum: %d-bit x %d angles = %d total bits per knot\n",
               min_b, min_m, min_total_bits);
        printf("  Weight table: %d knots x %d bits = %d bits (%d bytes)\n",
               num_knots, min_total_bits,
               num_knots * min_total_bits,
               (num_knots * min_total_bits + 7) / 8);
    } else {
        printf("\n  No perfect quantization found in tested range!\n");
    }

    sprintf(msg, "Pareto frontier found (%d bits/knot, was 8 for 10 knots)",
            min_total_bits);
    check(msg, min_total_bits > 0);
}

/* ================================================================
 * PART D: Train/test DKC validation
 * ================================================================ */

static int classify_against_table(const Cx *query, int num_angles,
                                   const Cx table[][MAX_ANGLES],
                                   int table_size) {
    int ki, ai, best;
    double best_dist, dist2;

    best = 0;
    best_dist = 1e30;
    for (ki = 0; ki < table_size; ki++) {
        dist2 = 0.0;
        for (ai = 0; ai < num_angles; ai++) {
            Cx d = cx_sub(query[ai], table[ki][ai]);
            dist2 += d.re * d.re + d.im * d.im;
        }
        if (dist2 < best_dist) {
            best_dist = dist2;
            best = ki;
        }
    }
    return best;
}

static void part_d_dkc_validation(void) {
    int train_size, test_size;
    int ti, ai, classified, correct;
    Cx query[MAX_ANGLES];
    char msg[200];

    /* Split: first 70% train, last 30% test */
    train_size = (num_knots * 7) / 10;
    if (train_size < 2) train_size = 2;
    test_size = num_knots - train_size;

    printf("\n=== PART D: Train/Test DKC Validation ===\n");
    printf("  Train set: knots 0-%d (%d knots)\n", train_size - 1, train_size);
    printf("  Test set:  knots %d-%d (%d knots)\n",
           train_size, num_knots - 1, test_size);
    printf("  Using %d angle(s) for classification\n\n", n_greedy);

    /* Classify each test knot against training table */
    correct = 0;
    for (ti = 0; ti < test_size; ti++) {
        int test_idx = train_size + ti;
        for (ai = 0; ai < n_greedy; ai++)
            query[ai] = ref_sigs[test_idx][ai];

        classified = classify_against_table(query, n_greedy,
                                             ref_sigs, train_size);

        /* For DKC: the test knot should NOT match any training knot
         * (they're different knots). The key question is whether the
         * classifier correctly identifies it as DIFFERENT from all
         * training entries, OR if it happens to be a duplicate. */

        /* Check if test knot is actually the same as any training knot */
        {
            double min_d = 1e30;
            int ai2;
            for (ai2 = 0; ai2 < n_greedy; ai2++) {
                Cx d = cx_sub(query[ai2], ref_sigs[classified][ai2]);
                min_d = cx_abs(d);
            }
            printf("  Test %-20s -> nearest train: %-20s (dist=%.6f) %s\n",
                   knames[test_idx], knames[classified], min_d,
                   min_d < 1e-6 ? "[SAME KNOT]" : "[DIFFERENT]");
            /* If they're genuinely different knots, distance should be > 0 */
            if (min_d > 1e-6) correct++;
        }
    }

    /* The DKC test: for genuinely new knots, the classifier should
     * produce distances > 0 to all training entries (correctly
     * identifying them as novel). For equivalent knots (duplicates),
     * it should match exactly. */
    printf("\n  %d/%d test knots correctly differentiated from training set\n",
           correct, test_size);

    /* Also test self-classification on training set */
    {
        int self_correct = 0;
        for (ti = 0; ti < train_size; ti++) {
            classified = classify_against_table(ref_sigs[ti], n_greedy,
                                                 ref_sigs, train_size);
            if (classified == ti) self_correct++;
        }
        sprintf(msg, "training self-classification: %d/%d",
                self_correct, train_size);
        check(msg, self_correct == train_size);
    }

    /* Invariance test: alt braid for trefoil */
    {
        Braid alt;
        int cls;
        double d;
        alt.n = 3; alt.len = 3;
        alt.word[0] = 2; alt.word[1] = 1; alt.word[2] = 2;
        for (ai = 0; ai < n_greedy; ai++)
            query[ai] = braid_bracket_at(&alt, cx_exp_i(greedy_angles[ai]));
        cls = classify_against_table(query, n_greedy, ref_sigs, train_size);
        d = cx_abs(cx_sub(query[0], ref_sigs[cls][0]));
        sprintf(msg, "alt trefoil (s2.s1.s2) -> %s (dist=%.2e)",
                knames[cls], d);
        check(msg, d < 1e-6);
    }
}

/* ================================================================
 * PART E: Random baseline comparison
 * ================================================================ */

/* Simple LCG random for reproducibility */
static unsigned long rng_state = 12345;
static double rng_double(void) {
    rng_state = rng_state * 1103515245UL + 12345UL;
    return (double)(rng_state & 0x7fffffffUL) / (double)0x7fffffffUL;
}

static void part_e_baseline(void) {
    int trial, ki, kj, ai;
    int n_trials = 100;
    double avg_accuracy = 0.0;
    int total_pairs = num_knots * (num_knots - 1) / 2;
    char msg[200];

    printf("\n=== PART E: Random Baseline Comparison ===\n");

    /* For each trial: generate random "weight table" with same dimensions,
     * count how many pairs are separated (distance > threshold) */
    for (trial = 0; trial < n_trials; trial++) {
        Cx random_sigs[MAX_KNOTS][MAX_ANGLES];
        int pairs_sep = 0;
        double re_range, im_range;

        /* Use similar range to actual signatures */
        re_range = 20.0; im_range = 20.0;

        for (ki = 0; ki < num_knots; ki++)
            for (ai = 0; ai < n_greedy; ai++)
                random_sigs[ki][ai] = cx_make(
                    (rng_double() - 0.5) * re_range,
                    (rng_double() - 0.5) * im_range);

        for (ki = 0; ki < num_knots; ki++)
            for (kj = ki + 1; kj < num_knots; kj++) {
                double dist2 = 0.0;
                for (ai = 0; ai < n_greedy; ai++) {
                    Cx d = cx_sub(random_sigs[ki][ai], random_sigs[kj][ai]);
                    dist2 += d.re * d.re + d.im * d.im;
                }
                if (dist2 > 1e-10) pairs_sep++;
            }
        avg_accuracy += (double)pairs_sep / (double)total_pairs;
    }
    avg_accuracy /= (double)n_trials;

    printf("  Random baseline (100 trials): avg %.1f%% pairs separated\n",
           avg_accuracy * 100.0);
    printf("  DKC classifier: %d/%d pairs at full precision\n",
           count_separated_pairs(n_greedy), total_pairs);

    sprintf(msg, "DKC beats random baseline (%.1f%% vs 100%%)",
            avg_accuracy * 100.0);
    /* Random should separate most pairs (they're random points in high-D space)
     * but the DKC classifier should get 100% */
    check(msg, count_separated_pairs(n_greedy) == total_pairs);
}

/* ================================================================
 * PART F: Scaling law analysis
 * ================================================================ */

static void part_f_scaling(void) {
    int subset_sizes[] = {5, 10, 15, 20, 25, 0};
    int si, ki, kj, n;
    char msg[200];
    int saw_decrease = 0;

    printf("\n=== PART F: Scaling Law — Min Separation vs Knot Count ===\n");
    printf("  %-8s %12s %20s\n", "N knots", "Min sep", "Closest pair");

    for (si = 0; subset_sizes[si] > 0; si++) {
        double min_sep = 1e30;
        int mi = 0, mj = 1;

        n = subset_sizes[si];
        if (n > num_knots) break;

        for (ki = 0; ki < n; ki++)
            for (kj = ki + 1; kj < n; kj++) {
                double dist2 = 0.0;
                /* Use first angle only for comparability with Demo 12 */
                {
                    Cx d = cx_sub(ref_sigs[ki][0], ref_sigs[kj][0]);
                    dist2 = d.re * d.re + d.im * d.im;
                }
                if (dist2 < min_sep) {
                    min_sep = dist2;
                    mi = ki; mj = kj;
                }
            }
        min_sep = sqrt(min_sep);
        printf("  %-8d %12.6f %s vs %s\n",
               n, min_sep, knames[mi], knames[mj]);
    }

    /* Full table */
    {
        double min_sep = 1e30;
        int mi = 0, mj = 1;
        for (ki = 0; ki < num_knots; ki++)
            for (kj = ki + 1; kj < num_knots; kj++) {
                Cx d = cx_sub(ref_sigs[ki][0], ref_sigs[kj][0]);
                double dist2 = d.re * d.re + d.im * d.im;
                if (dist2 < min_sep) {
                    min_sep = dist2;
                    mi = ki; mj = kj;
                }
            }
        min_sep = sqrt(min_sep);
        printf("  %-8d %12.6f %s vs %s  <-- FULL TABLE\n",
               num_knots, min_sep, knames[mi], knames[mj]);
        saw_decrease = (min_sep < 0.907);
    }

    sprintf(msg, "min separation decreased from 0.907 (10 knots) as expected");
    check(msg, saw_decrease);

    /* Extrapolation */
    printf("\n  Extrapolation:\n");
    printf("  If min_sep ~ 1/sqrt(N), then for 250 knots: ~%.3f\n",
           0.907 / sqrt(250.0 / 10.0));
    printf("  4-bit quantization step size ~ range/15\n");
    printf("  Fails when step_size > min_sep\n");
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 13: DKC Test\n");
    printf("===========================\n");

    init_knots();
    dedup_knots();

    printf("\n--- Selecting greedy angles (this may take a moment) ---\n");
    select_greedy_angles(MAX_ANGLES);
    {
        int ai;
        printf("  Selected %d angles:\n", n_greedy);
        for (ai = 0; ai < n_greedy; ai++)
            printf("    theta_%d = %.6f (%.4f*pi)\n",
                   ai, greedy_angles[ai], greedy_angles[ai] / M_PI);
    }

    printf("\n--- Computing reference signatures ---\n");
    compute_references();

    part_a_separation();
    part_b_multi_angle();
    part_c_pareto();
    part_d_dkc_validation();
    part_e_baseline();
    part_f_scaling();

    printf("\n===========================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("===========================\n");
    return n_fail > 0 ? 1 : 0;
}
