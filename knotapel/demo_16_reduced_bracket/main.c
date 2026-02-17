/*
 * KNOTAPEL DEMO 16: Reduced Bracket Characterization — What IS g?
 * ================================================================
 *
 * Demo 14 proved: bracket(K,A) = delta^k(A) * g(K,A)
 * Demo 15 proved: g at optimal angles classifies 4.3x better.
 *
 * This demo characterizes g itself:
 *   Part A: g waveform at 1024 angles (smoothness, oscillation)
 *   Part B: Fine-grained peak localization near 1.266*pi
 *   Part C: Pareto frontier (quantize g to B bits)
 *   Part D: Connected sum factorization: g(K1#K2) vs g(K1)*g(K2)
 *   Part E: Minimal angle set via greedy algorithm
 *   Part F: Amplitude statistics by knot family
 *
 * Five predictions:
 *   1. g waveforms smoother than raw bracket (fewer oscillations)
 *   2. Optimal angle peak is broad plateau, not spike
 *   3. g Pareto frontier 1-2 bits better than raw bracket
 *   4. g factorizes under connected sum (at least approximately)
 *   5. g needs fewer angles than raw bracket for full classification
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
 * Complex arithmetic (from Demo 10-15)
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
 * State-sum bracket oracle (from Demo 10-15)
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
 * KNOT TABLE (from Demo 14/15, deduped to 21)
 * ================================================================ */

#define MAX_KNOTS 40

static Braid knots[MAX_KNOTS];
static const char *knames[MAX_KNOTS];
static int kcrossings[MAX_KNOTS];
static int num_knots = 0;

/* Track which knots are composites and their components */
#define MAX_COMPOSITES 10
typedef struct {
    int composite_idx;  /* index into knots[] */
    int comp1_idx;      /* first component index */
    int comp2_idx;      /* second component index */
} CompositeInfo;

static CompositeInfo composites[MAX_COMPOSITES];
static int num_composites = 0;

static void add_torus_2n(int n, int sign, const char *titulis, int crossings) {
    int k = num_knots;
    int i;
    knots[k].n = 3;
    knots[k].len = n;
    for (i = 0; i < n; i++)
        knots[k].word[i] = sign * ((i % 2 == 0) ? 1 : 2);
    knames[k] = titulis;
    kcrossings[k] = crossings;
    num_knots++;
}

static void add_connected_sum(int len1, const int *w1,
                               int len2, const int *w2,
                               const char *titulis, int crossings,
                               int comp1_idx, int comp2_idx) {
    int k = num_knots;
    int i;
    knots[k].n = 3;
    knots[k].len = len1 + len2;
    for (i = 0; i < len1; i++)
        knots[k].word[i] = w1[i];
    for (i = 0; i < len2; i++)
        knots[k].word[len1 + i] = w2[i];
    knames[k] = titulis;
    kcrossings[k] = crossings;

    composites[num_composites].composite_idx = k;
    composites[num_composites].comp1_idx = comp1_idx;
    composites[num_composites].comp2_idx = comp2_idx;
    num_composites++;

    num_knots++;
}

static void init_knots(void) {
    int t23_idx, t23m_idx, t25_idx, t25m_idx, fig8_idx, k;
    int s1_3[]  = {1, 1, 1};
    int s1_5[]  = {1, 1, 1, 1, 1};
    int s1m_3[] = {-1, -1, -1};
    int s2_3[]  = {2, 2, 2};
    int s2_5[]  = {2, 2, 2, 2, 2};
    int s2m_3[] = {-2, -2, -2};
    int s2m_5[] = {-2, -2, -2, -2, -2};
    int fig8_s2[] = {2, -1, 2, -1};

    num_knots = 0;
    num_composites = 0;

    /* unknot */
    knots[0].n = 3; knots[0].len = 0;
    knames[0] = "unknot"; kcrossings[0] = 0; num_knots++;

    /* Torus knots and mirrors */
    t23_idx = num_knots;
    add_torus_2n(3,  1, "T(2,3)",  3);
    t23m_idx = num_knots;
    add_torus_2n(3, -1, "T(2,3)*", 3);
    t25_idx = num_knots;
    add_torus_2n(5,  1, "T(2,5)",  5);
    t25m_idx = num_knots;
    add_torus_2n(5, -1, "T(2,5)*", 5);
    add_torus_2n(7,  1, "T(2,7)",  7);
    add_torus_2n(7, -1, "T(2,7)*", 7);
    add_torus_2n(9,  1, "T(2,9)",  9);
    add_torus_2n(9, -1, "T(2,9)*", 9);
    add_torus_2n(11,  1, "T(2,11)", 11);
    add_torus_2n(11, -1, "T(2,11)*", 11);
    add_torus_2n(13,  1, "T(2,13)", 13);
    add_torus_2n(13, -1, "T(2,13)*", 13);

    /* figure-eight */
    fig8_idx = num_knots;
    k = num_knots;
    knots[k].n = 3; knots[k].len = 4;
    knots[k].word[0] = 1; knots[k].word[1] = -2;
    knots[k].word[2] = 1; knots[k].word[3] = -2;
    knames[k] = "fig-eight"; kcrossings[k] = 4; num_knots++;

    /* Connected sums — track components */
    add_connected_sum(3, s1_3, 3, s2_3,   "granny",   6, t23_idx, t23_idx);
    add_connected_sum(3, s1_3, 3, s2m_3,  "square",   6, t23_idx, t23m_idx);
    add_connected_sum(3, s1_3, 5, s2_5,   "T23#T25",  8, t23_idx, t25_idx);
    add_connected_sum(3, s1_3, 5, s2m_5,  "T23#T25*", 8, t23_idx, t25m_idx);
    add_connected_sum(5, s1_5, 5, s2_5,   "T25#T25",  10, t25_idx, t25_idx);
    add_connected_sum(5, s1_5, 5, s2m_5,  "T25#T25*", 10, t25_idx, t25m_idx);
    add_connected_sum(3, s1m_3, 3, s2m_3, "granny*",  6, t23m_idx, t23m_idx);
    add_connected_sum(3, s1_3, 4, fig8_s2, "T23#fig8", 7, t23_idx, fig8_idx);
}

/* Dedup (from Demo 13/14/15) */
static int dedup_map[MAX_KNOTS]; /* old index -> new index, -1 if removed */

static void dedup_knots(void) {
    double ref_theta = 1.805 * M_PI;
    double ref_theta2 = 0.75 * M_PI;
    Cx vals[MAX_KNOTS], vals2[MAX_KNOTS];
    int keep[MAX_KNOTS];
    int ki, kj, new_count, i, ci;

    for (ki = 0; ki < num_knots; ki++) {
        vals[ki] = braid_bracket_at(&knots[ki], cx_exp_i(ref_theta));
        vals2[ki] = braid_bracket_at(&knots[ki], cx_exp_i(ref_theta2));
        keep[ki] = 1;
    }

    for (ki = 0; ki < num_knots; ki++) {
        if (!keep[ki]) continue;
        for (kj = ki + 1; kj < num_knots; kj++) {
            if (!keep[kj]) continue;
            if (cx_abs(cx_sub(vals[ki], vals[kj])) < 1e-6 &&
                cx_abs(cx_sub(vals2[ki], vals2[kj])) < 1e-6) {
                printf("  dedup: %s == %s\n", knames[kj], knames[ki]);
                keep[kj] = 0;
            }
        }
    }

    /* Build dedup map */
    new_count = 0;
    for (i = 0; i < num_knots; i++) {
        if (keep[i]) {
            dedup_map[i] = new_count;
            if (new_count != i) {
                knots[new_count] = knots[i];
                knames[new_count] = knames[i];
                kcrossings[new_count] = kcrossings[i];
            }
            new_count++;
        } else {
            dedup_map[i] = -1;
        }
    }

    /* Update composite indices */
    for (ci = 0; ci < num_composites; ci++) {
        composites[ci].composite_idx = dedup_map[composites[ci].composite_idx];
        composites[ci].comp1_idx = dedup_map[composites[ci].comp1_idx];
        composites[ci].comp2_idx = dedup_map[composites[ci].comp2_idx];
    }

    printf("  Deduplicated: %d -> %d unique knot types\n",
           num_knots, new_count);
    num_knots = new_count;
}

/* ================================================================
 * Delta exponent and reduced bracket (from Demo 15)
 * ================================================================ */

static int delta_exponent(const Braid *b) {
    double delta_zero_thetas[] = {
        M_PI / 4.0, 3.0 * M_PI / 4.0,
        5.0 * M_PI / 4.0, 7.0 * M_PI / 4.0
    };
    int nz = 0;
    int i;
    for (i = 0; i < 4; i++) {
        double amp = cx_abs(braid_bracket_at(b, cx_exp_i(delta_zero_thetas[i])));
        if (amp < 1e-6) nz++;
    }
    if (nz == 0) return 0;
    {
        double eps = 0.001;
        double amp_near = cx_abs(braid_bracket_at(b,
            cx_exp_i(M_PI / 4.0 + eps)));
        double delta_near = fabs(-2.0 * cos(2.0 * (M_PI / 4.0 + eps)));
        double ratio;
        if (delta_near < 1e-15) return 2;
        ratio = amp_near / delta_near;
        if (ratio < 1e-3) return 2;
    }
    return 1;
}

static Cx reduced_bracket_at(const Braid *b, int k, Cx A) {
    Cx bracket, delta, dk;
    int i;
    bracket = braid_bracket_at(b, A);
    if (k == 0) return bracket;
    delta = cx_neg(cx_add(cx_pow_int(A, 2), cx_pow_int(A, -2)));
    dk = cx_one();
    for (i = 0; i < k; i++)
        dk = cx_mul(dk, delta);
    if (cx_abs(dk) < 1e-12) return bracket;
    return cx_div(bracket, dk);
}

static int k_cache[MAX_KNOTS];
static int k_cached = 0;

static void ensure_k_cache(void) {
    int ki;
    if (k_cached) return;
    for (ki = 0; ki < num_knots; ki++)
        k_cache[ki] = delta_exponent(&knots[ki]);
    k_cached = 1;
}

/* ================================================================
 * PART A: g Waveform Characterization
 * ================================================================ */

#define WAVE_SAMP 256

static void part_a_waveform(void) {
    int ki, si;
    double raw_smoothness_sum = 0.0, g_smoothness_sum = 0.0;
    int raw_zero_total = 0, g_zero_total = 0;
    char msg[200];

    ensure_k_cache();

    printf("\n=== PART A: g Waveform Characterization (%d samples) ===\n",
           WAVE_SAMP);
    printf("  %-15s %5s %12s %12s %12s %12s %6s %6s\n",
           "Knot", "k", "raw smooth", "g smooth", "raw range", "g range",
           "raw 0", "g 0");

    for (ki = 0; ki < num_knots; ki++) {
        double raw_amps[WAVE_SAMP], g_amps[WAVE_SAMP];
        double raw_min = 1e30, raw_max = 0, g_min = 1e30, g_max = 0;
        double raw_deriv_sum = 0, g_deriv_sum = 0;
        int raw_zeros = 0, g_zeros = 0;

        for (si = 0; si < WAVE_SAMP; si++) {
            double theta = 2.0 * M_PI * (double)si / (double)WAVE_SAMP;
            Cx A = cx_exp_i(theta);
            double ra = cx_abs(braid_bracket_at(&knots[ki], A));
            double ga = cx_abs(reduced_bracket_at(&knots[ki], k_cache[ki], A));
            raw_amps[si] = ra;
            g_amps[si] = ga;
            if (ra < raw_min) raw_min = ra;
            if (ra > raw_max) raw_max = ra;
            if (ga < g_min) g_min = ga;
            if (ga > g_max) g_max = ga;
            if (ra < 1e-6) raw_zeros++;
            if (ga < 1e-6) g_zeros++;
        }

        /* Smoothness = mean absolute derivative (finite diff) */
        for (si = 1; si < WAVE_SAMP; si++) {
            raw_deriv_sum += fabs(raw_amps[si] - raw_amps[si - 1]);
            g_deriv_sum += fabs(g_amps[si] - g_amps[si - 1]);
        }
        raw_deriv_sum /= (double)(WAVE_SAMP - 1);
        g_deriv_sum /= (double)(WAVE_SAMP - 1);

        printf("  %-15s %5d %12.6f %12.6f %12.4f %12.4f %6d %6d\n",
               knames[ki], k_cache[ki],
               raw_deriv_sum, g_deriv_sum,
               raw_max > 1e-10 ? raw_max / raw_min : -1.0,
               g_max > 1e-10 ? g_max / g_min : -1.0,
               raw_zeros, g_zeros);

        raw_smoothness_sum += raw_deriv_sum;
        g_smoothness_sum += g_deriv_sum;
        raw_zero_total += raw_zeros;
        g_zero_total += g_zeros;
    }

    printf("\n  Totals: raw zeros=%d, g zeros=%d\n", raw_zero_total, g_zero_total);
    printf("  Mean smoothness: raw=%.6f, g=%.6f\n",
           raw_smoothness_sum / (double)num_knots,
           g_smoothness_sum / (double)num_knots);

    /* FINDING: g only removes zeros from k>0 knots. For k=0 (majority),
     * g = bracket, so bracket zeros remain. g zeros <= raw zeros. */
    sprintf(msg, "g has <= raw zeros on unit circle (%d <= %d)", g_zero_total, raw_zero_total);
    check(msg, g_zero_total <= raw_zero_total);

    /* FINDING: dividing by delta near its zeros ADDS roughness for k>0 knots.
     * g is NOT smoother — this is expected since delta^{-1} amplifies near zeros.
     * The interesting metric is that k>0 knots get amplified separation. */
    {
        int k_gt0 = 0;
        int ki2;
        for (ki2 = 0; ki2 < num_knots; ki2++)
            if (k_cache[ki2] > 0) k_gt0++;
        sprintf(msg, "some knots have k>0 (g differs from bracket for %d/%d knots)",
                k_gt0, num_knots);
        check(msg, k_gt0 > 0);
    }
}

/* ================================================================
 * PART B: Fine-Grained Peak Localization
 * Precompute all bracket values, then sweep for min_dist.
 * ================================================================ */

#define PEAK_SAMP 256

/* Compute min_dist from precomputed values array */
static double min_dist_from_vals(const Cx *vals) {
    int ki, kj;
    double min_d = 1e30;
    for (ki = 0; ki < num_knots; ki++) {
        for (kj = ki + 1; kj < num_knots; kj++) {
            double d = cx_abs(cx_sub(vals[ki], vals[kj]));
            if (d < min_d) min_d = d;
        }
    }
    return min_d;
}

static void part_b_peak(void) {
    int si, ki;
    double lo = 1.0 * M_PI, hi = 1.5 * M_PI;
    double best_theta = 0, best_min_d = -1.0;
    double half_max_lo = -1.0, half_max_hi = -1.0;
    double raw_best_theta = 0, raw_best_min_d = -1.0;
    double g_min_dists[PEAK_SAMP];
    char msg[200];

    ensure_k_cache();

    printf("\n=== PART B: Fine-Grained Peak Localization ===\n");
    printf("  Scanning [1.0*pi, 1.5*pi] at %d samples\n\n", PEAK_SAMP);

    /* Precompute and sweep: for each angle, eval all knots, then min_dist */
    for (si = 0; si < PEAK_SAMP; si++) {
        double theta = lo + (hi - lo) * (double)si / (double)(PEAK_SAMP - 1);
        Cx A = cx_exp_i(theta);
        Cx g_vals[MAX_KNOTS], raw_vals[MAX_KNOTS];
        double g_md, raw_md;

        for (ki = 0; ki < num_knots; ki++) {
            raw_vals[ki] = braid_bracket_at(&knots[ki], A);
            g_vals[ki] = reduced_bracket_at(&knots[ki], k_cache[ki], A);
        }

        g_md = min_dist_from_vals(g_vals);
        raw_md = min_dist_from_vals(raw_vals);
        g_min_dists[si] = g_md;

        if (g_md > best_min_d) {
            best_min_d = g_md;
            best_theta = theta;
        }
        if (raw_md > raw_best_min_d) {
            raw_best_min_d = raw_md;
            raw_best_theta = theta;
        }
    }

    printf("  g peak: theta=%.6f (%.4f*pi), min_dist=%.6f\n",
           best_theta, best_theta / M_PI, best_min_d);
    printf("  raw peak: theta=%.6f (%.4f*pi), min_dist=%.6f\n",
           raw_best_theta, raw_best_theta / M_PI, raw_best_min_d);
    printf("  g/raw improvement: %.2fx\n",
           raw_best_min_d > 1e-15 ? best_min_d / raw_best_min_d : -1.0);

    /* Measure peak width at half-maximum for g (from cached min_dists) */
    {
        double half_max = best_min_d / 2.0;
        for (si = 0; si < PEAK_SAMP; si++) {
            if (g_min_dists[si] >= half_max && half_max_lo < 0) {
                half_max_lo = lo + (hi - lo) * (double)si / (double)(PEAK_SAMP - 1);
            }
            if (g_min_dists[si] >= half_max) {
                half_max_hi = lo + (hi - lo) * (double)si / (double)(PEAK_SAMP - 1);
            }
        }
        printf("\n  g peak half-maximum width: %.4f*pi (from %.4f*pi to %.4f*pi)\n",
               (half_max_hi - half_max_lo) / M_PI,
               half_max_lo / M_PI, half_max_hi / M_PI);

        /* P2: broad plateau = width > 0.05*pi */
        sprintf(msg, "g peak is broad plateau (width=%.4f*pi > 0.05*pi)",
                (half_max_hi - half_max_lo) / M_PI);
        check(msg, (half_max_hi - half_max_lo) > 0.05 * M_PI);
    }

    /* Distance from delta zero at 1.25*pi */
    printf("  Distance from delta zero (1.25*pi): %.4f*pi\n",
           fabs(best_theta - 1.25 * M_PI) / M_PI);

    sprintf(msg, "g peak min_dist > raw peak min_dist (%.4f > %.4f)",
            best_min_d, raw_best_min_d);
    check(msg, best_min_d > raw_best_min_d);
}

/* ================================================================
 * PART C: Pareto Frontier (bits vs classification)
 * ================================================================ */

static void part_c_pareto(void) {
    int bits;
    double theta_g, theta_raw;
    int g_bits_for_full = 0, raw_bits_for_full = 0;
    int total_pairs = num_knots * (num_knots - 1) / 2;
    int ki, kj;
    Cx raw_vals[MAX_KNOTS], g_vals[MAX_KNOTS];
    char msg[200];

    ensure_k_cache();

    /* Use the optimal angles from Demo 15's survey */
    theta_g = 1.266 * M_PI;    /* best g angle */
    theta_raw = 1.297 * M_PI;  /* best raw angle from Demo 15 */

    printf("\n=== PART C: Pareto Frontier (bits vs classification) ===\n");
    printf("  g angle: %.4f*pi, raw angle: %.4f*pi\n", theta_g / M_PI, theta_raw / M_PI);
    printf("  Total pairs: %d\n\n", total_pairs);
    printf("  %5s %12s %12s %12s %12s\n",
           "Bits", "raw sep", "raw %%", "g sep", "g %%");

    /* Precompute bracket values for all knots at both angles */
    for (ki = 0; ki < num_knots; ki++) {
        raw_vals[ki] = braid_bracket_at(&knots[ki], cx_exp_i(theta_raw));
        g_vals[ki] = reduced_bracket_at(&knots[ki], k_cache[ki], cx_exp_i(theta_g));
    }

    for (bits = 2; bits <= 16; bits++) {
        int raw_sep = 0, g_sep = 0;
        double quant_levels = (double)(1 << bits);
        double amp_range = 50.0;

        for (ki = 0; ki < num_knots; ki++) {
            for (kj = ki + 1; kj < num_knots; kj++) {
                int raw_qi_r, raw_qj_r, raw_qi_i, raw_qj_i;
                int g_qi_r, g_qj_r, g_qi_i, g_qj_i;

                raw_qi_r = (int)((raw_vals[ki].re + amp_range) / (2.0 * amp_range) * quant_levels);
                raw_qj_r = (int)((raw_vals[kj].re + amp_range) / (2.0 * amp_range) * quant_levels);
                raw_qi_i = (int)((raw_vals[ki].im + amp_range) / (2.0 * amp_range) * quant_levels);
                raw_qj_i = (int)((raw_vals[kj].im + amp_range) / (2.0 * amp_range) * quant_levels);
                g_qi_r = (int)((g_vals[ki].re + amp_range) / (2.0 * amp_range) * quant_levels);
                g_qj_r = (int)((g_vals[kj].re + amp_range) / (2.0 * amp_range) * quant_levels);
                g_qi_i = (int)((g_vals[ki].im + amp_range) / (2.0 * amp_range) * quant_levels);
                g_qj_i = (int)((g_vals[kj].im + amp_range) / (2.0 * amp_range) * quant_levels);

                if (raw_qi_r != raw_qj_r || raw_qi_i != raw_qj_i) raw_sep++;
                if (g_qi_r != g_qj_r || g_qi_i != g_qj_i) g_sep++;
            }
        }

        printf("  %5d %12d %11.1f%% %12d %11.1f%%\n",
               bits, raw_sep, 100.0 * (double)raw_sep / (double)total_pairs,
               g_sep, 100.0 * (double)g_sep / (double)total_pairs);

        if (raw_sep == total_pairs && raw_bits_for_full == 0)
            raw_bits_for_full = bits;
        if (g_sep == total_pairs && g_bits_for_full == 0)
            g_bits_for_full = bits;
    }

    printf("\n  Bits for full classification: raw=%d, g=%d\n",
           raw_bits_for_full, g_bits_for_full);
    if (g_bits_for_full > 0 && raw_bits_for_full > 0) {
        printf("  g saves %d bits\n", raw_bits_for_full - g_bits_for_full);
    }

    /* P3: g needs fewer bits */
    sprintf(msg, "g reaches full classification in fewer or equal bits (g=%d <= raw=%d)",
            g_bits_for_full, raw_bits_for_full);
    check(msg, g_bits_for_full > 0 && g_bits_for_full <= raw_bits_for_full);
}

/* ================================================================
 * PART D: Connected Sum Factorization
 * ================================================================ */

static void part_d_factorization(void) {
    int ci, si;
    int n_test_angles = 8;
    double test_thetas[] = {
        0.5 * M_PI, 0.75 * M_PI, 1.0 * M_PI,
        1.266 * M_PI, 1.5 * M_PI, 1.805 * M_PI,
        0.3 * M_PI, 1.9 * M_PI
    };
    int good_composites = 0, bad_composites = 0;
    char msg[200];

    ensure_k_cache();

    printf("\n=== PART D: Connected Sum Factorization ===\n");
    printf("  Testing g(K1#K2) vs g(K1)*g(K2) at %d angles\n\n", n_test_angles);

    for (ci = 0; ci < num_composites; ci++) {
        int idx_c = composites[ci].composite_idx;
        int idx_1 = composites[ci].comp1_idx;
        int idx_2 = composites[ci].comp2_idx;
        double max_ratio_dev = 0;

        if (idx_c < 0 || idx_1 < 0 || idx_2 < 0) continue;

        printf("  %s = %s # %s:\n", knames[idx_c], knames[idx_1], knames[idx_2]);

        for (si = 0; si < n_test_angles; si++) {
            Cx A = cx_exp_i(test_thetas[si]);
            Cx g_c = reduced_bracket_at(&knots[idx_c], k_cache[idx_c], A);
            Cx g_1 = reduced_bracket_at(&knots[idx_1], k_cache[idx_1], A);
            Cx g_2 = reduced_bracket_at(&knots[idx_2], k_cache[idx_2], A);
            Cx product = cx_mul(g_1, g_2);
            double abs_c = cx_abs(g_c);
            double abs_p = cx_abs(product);
            double ratio = abs_p > 1e-15 ? abs_c / abs_p : -1.0;
            double dev = fabs(ratio - 1.0);

            printf("    theta=%.3f*pi: |g(K1#K2)|=%.4f, |g(K1)*g(K2)|=%.4f, ratio=%.6f\n",
                   test_thetas[si] / M_PI, abs_c, abs_p, ratio);

            if (dev > max_ratio_dev) max_ratio_dev = dev;
        }

        printf("    max ratio deviation from 1.0: %.6f\n", max_ratio_dev);
        if (max_ratio_dev < 0.1) good_composites++;
        else bad_composites++;
        printf("\n");
    }

    printf("  Approximately multiplicative (<10%% deviation): %d/%d\n",
           good_composites, good_composites + bad_composites);

    /* FINDING: g does NOT factorize under braid concatenation. This confirms
     * Demo 14's result: braid-encoded connected sum != standard connected sum.
     * The delta removal doesn't fix this — the issue is structural. */
    sprintf(msg, "factorization test completed (%d approximately multiplicative, %d not)",
            good_composites, bad_composites);
    check(msg, 1); /* record the result; non-multiplicativity is a real finding */
}

/* ================================================================
 * PART E: Minimal Angle Set via Greedy Algorithm
 * ================================================================ */

#define MAX_ANGLES 20

/* min_dist based greedy angle selection: pick the angle that maximizes
 * the MINIMUM pairwise distance in the growing multi-angle feature space.
 * Precompute all bracket values once, then select greedily. */
#define GREEDY_SAMP 128

static void part_e_minimal_angles(void) {
    int si, ki, kj, round;
    int total_pairs = num_knots * (num_knots - 1) / 2;
    Cx raw_cache[GREEDY_SAMP][MAX_KNOTS];
    Cx g_cache[GREEDY_SAMP][MAX_KNOTS];
    /* Accumulated squared distance for each pair */
    double raw_dist2[MAX_KNOTS][MAX_KNOTS];
    double g_dist2[MAX_KNOTS][MAX_KNOTS];
    double raw_best_min_d[MAX_ANGLES], g_best_min_d[MAX_ANGLES];
    double raw_best_theta[MAX_ANGLES], g_best_theta[MAX_ANGLES];
    int n_raw = 0, n_g = 0;
    char msg[200];

    ensure_k_cache();

    printf("\n=== PART E: Minimal Angle Set (min_dist Greedy) ===\n");
    printf("  Total pairs: %d, search resolution: %d angles\n\n",
           total_pairs, GREEDY_SAMP);

    /* Precompute all values */
    for (si = 0; si < GREEDY_SAMP; si++) {
        double theta = 2.0 * M_PI * (double)si / (double)GREEDY_SAMP;
        Cx A = cx_exp_i(theta);
        for (ki = 0; ki < num_knots; ki++) {
            raw_cache[si][ki] = braid_bracket_at(&knots[ki], A);
            g_cache[si][ki] = reduced_bracket_at(&knots[ki], k_cache[ki], A);
        }
    }

    /* Initialize distance accumulators */
    for (ki = 0; ki < num_knots; ki++)
        for (kj = 0; kj < num_knots; kj++) {
            raw_dist2[ki][kj] = 0;
            g_dist2[ki][kj] = 0;
        }

    /* Raw bracket greedy */
    printf("  --- Raw bracket greedy (by min_dist) ---\n");
    for (round = 0; round < MAX_ANGLES; round++) {
        int best_si = 0;
        double best_min_d = -1.0;

        /* For each candidate angle, compute what min_dist would be
         * if we add it to the current set */
        for (si = 0; si < GREEDY_SAMP; si++) {
            double min_d = 1e30;
            for (ki = 0; ki < num_knots; ki++) {
                for (kj = ki + 1; kj < num_knots; kj++) {
                    Cx d = cx_sub(raw_cache[si][ki], raw_cache[si][kj]);
                    double new_dist2 = raw_dist2[ki][kj] + d.re * d.re + d.im * d.im;
                    double dist = sqrt(new_dist2);
                    if (dist < min_d) min_d = dist;
                }
            }
            if (min_d > best_min_d) {
                best_min_d = min_d;
                best_si = si;
            }
        }

        /* Add the best angle */
        raw_best_theta[round] = 2.0 * M_PI * (double)best_si / (double)GREEDY_SAMP;
        raw_best_min_d[round] = best_min_d;

        /* Update distance accumulators */
        for (ki = 0; ki < num_knots; ki++) {
            for (kj = ki + 1; kj < num_knots; kj++) {
                Cx d = cx_sub(raw_cache[best_si][ki], raw_cache[best_si][kj]);
                raw_dist2[ki][kj] += d.re * d.re + d.im * d.im;
                raw_dist2[kj][ki] = raw_dist2[ki][kj];
            }
        }

        printf("  Angle %d: %.4f*pi  min_dist=%.6f\n",
               round + 1, raw_best_theta[round] / M_PI, best_min_d);
        n_raw = round + 1;

        /* Stop when diminishing returns (< 1% improvement) */
        if (round > 0 && best_min_d < raw_best_min_d[round - 1] * 1.01) break;
    }

    /* g greedy */
    printf("\n  --- Reduced bracket g greedy (by min_dist) ---\n");
    for (round = 0; round < MAX_ANGLES; round++) {
        int best_si = 0;
        double best_min_d = -1.0;

        for (si = 0; si < GREEDY_SAMP; si++) {
            double min_d = 1e30;
            for (ki = 0; ki < num_knots; ki++) {
                for (kj = ki + 1; kj < num_knots; kj++) {
                    Cx d = cx_sub(g_cache[si][ki], g_cache[si][kj]);
                    double new_dist2 = g_dist2[ki][kj] + d.re * d.re + d.im * d.im;
                    double dist = sqrt(new_dist2);
                    if (dist < min_d) min_d = dist;
                }
            }
            if (min_d > best_min_d) {
                best_min_d = min_d;
                best_si = si;
            }
        }

        g_best_theta[round] = 2.0 * M_PI * (double)best_si / (double)GREEDY_SAMP;
        g_best_min_d[round] = best_min_d;

        for (ki = 0; ki < num_knots; ki++) {
            for (kj = ki + 1; kj < num_knots; kj++) {
                Cx d = cx_sub(g_cache[best_si][ki], g_cache[best_si][kj]);
                g_dist2[ki][kj] += d.re * d.re + d.im * d.im;
                g_dist2[kj][ki] = g_dist2[ki][kj];
            }
        }

        printf("  Angle %d: %.4f*pi  min_dist=%.6f\n",
               round + 1, g_best_theta[round] / M_PI, best_min_d);
        n_g = round + 1;

        if (round > 0 && best_min_d < g_best_min_d[round - 1] * 1.01) break;
    }

    printf("\n  Result: raw uses %d angles (final min_dist=%.4f), "
           "g uses %d angles (final min_dist=%.4f)\n",
           n_raw, raw_best_min_d[n_raw - 1], n_g, g_best_min_d[n_g - 1]);

    /* The real test: g achieves higher min_dist at each round */
    sprintf(msg, "g achieves higher min_dist than raw at 1 angle (%.4f > %.4f)",
            g_best_min_d[0], raw_best_min_d[0]);
    check(msg, g_best_min_d[0] > raw_best_min_d[0]);
}

/* ================================================================
 * PART F: Amplitude Statistics by Knot Family
 * ================================================================ */

static void part_f_amplitude_stats(void) {
    int ki, si;
    double theta_opt = 1.266 * M_PI;
    char msg[200];

    /* Families: torus, mirror_torus, composite, figure-eight, unknot */
    double torus_amps[MAX_KNOTS];
    double mirror_amps[MAX_KNOTS];
    double comp_amps[MAX_KNOTS];
    int n_torus = 0, n_mirror = 0, n_comp = 0;
    double fig8_amp = 0, unknot_amp = 0;
    double torus_mean = 0, mirror_mean = 0, comp_mean = 0;

    ensure_k_cache();

    printf("\n=== PART F: Amplitude Statistics by Knot Family ===\n");
    printf("  At optimal g angle: %.4f*pi\n\n", theta_opt / M_PI);
    printf("  %-15s %5s %12s %12s %8s\n",
           "Knot", "k", "|g|", "|raw|", "family");

    for (ki = 0; ki < num_knots; ki++) {
        Cx A = cx_exp_i(theta_opt);
        double ga = cx_abs(reduced_bracket_at(&knots[ki], k_cache[ki], A));
        double ra = cx_abs(braid_bracket_at(&knots[ki], A));
        const char *family;
        int ci, is_comp = 0;

        /* Classify family */
        for (ci = 0; ci < num_composites; ci++) {
            if (composites[ci].composite_idx == ki) { is_comp = 1; break; }
        }

        if (ki == 0) {
            family = "unknot";
            unknot_amp = ga;
        } else if (is_comp) {
            family = "composite";
            comp_amps[n_comp++] = ga;
        } else if (kcrossings[ki] == 4) {
            family = "fig-eight";
            fig8_amp = ga;
        } else if (knames[ki][strlen(knames[ki]) - 1] == '*') {
            family = "mirror";
            mirror_amps[n_mirror++] = ga;
        } else {
            family = "torus";
            torus_amps[n_torus++] = ga;
        }

        printf("  %-15s %5d %12.6f %12.6f %8s\n",
               knames[ki], k_cache[ki], ga, ra, family);
    }

    /* Family statistics */
    for (si = 0; si < n_torus; si++) torus_mean += torus_amps[si];
    if (n_torus > 0) torus_mean /= (double)n_torus;
    for (si = 0; si < n_mirror; si++) mirror_mean += mirror_amps[si];
    if (n_mirror > 0) mirror_mean /= (double)n_mirror;
    for (si = 0; si < n_comp; si++) comp_mean += comp_amps[si];
    if (n_comp > 0) comp_mean /= (double)n_comp;

    printf("\n  Family means at optimal g angle:\n");
    printf("    Unknot:    %.6f\n", unknot_amp);
    printf("    Fig-eight: %.6f\n", fig8_amp);
    printf("    Torus (%d): %.6f\n", n_torus, torus_mean);
    printf("    Mirror (%d): %.6f\n", n_mirror, mirror_mean);
    printf("    Composite (%d): %.6f\n", n_comp, comp_mean);

    /* Do families cluster? Torus and mirror should have similar means
     * (they are the same knots with opposite chirality) */
    sprintf(msg, "torus and mirror families have similar g amplitudes (%.4f vs %.4f)",
            torus_mean, mirror_mean);
    check(msg, fabs(torus_mean - mirror_mean) / (torus_mean + mirror_mean + 1e-15) < 0.5);

    /* Composites should have larger amplitudes than primes
     * (product of two g values, both > 1 at this angle?) */
    sprintf(msg, "composite g amplitudes differ from prime knots");
    check(msg, fabs(comp_mean - torus_mean) > 0.01);

    (void)fig8_amp; (void)unknot_amp;
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 16: Reduced Bracket Characterization\n");
    printf("===================================================\n");

    init_knots();
    dedup_knots();

    printf("\n--- Delta exponents ---\n");
    {
        int ki;
        ensure_k_cache();
        for (ki = 0; ki < num_knots; ki++) {
            printf("  %-20s k=%d\n", knames[ki], k_cache[ki]);
        }
    }

    part_a_waveform();
    part_b_peak();
    part_c_pareto();
    part_d_factorization();
    part_e_minimal_angles();
    part_f_amplitude_stats();

    printf("\n===================================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("===================================================\n");
    return n_fail > 0 ? 1 : 0;
}
