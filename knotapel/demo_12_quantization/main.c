/*
 * KNOTAPEL DEMO 12: Quantization
 * ================================
 *
 * Demo 11 showed that the bracket waveform at a single optimal angle
 * (theta = 1.805*pi) separates all 10 test knots with min distance 0.907.
 * Each knot's "weight" is one complex double = 128 bits.
 *
 * This demo asks: how aggressively can we quantize those weights?
 *   - Bit-depth sweep: 16, 12, 8, 6, 4, 3, 2, 1 bits per component
 *   - Component analysis: amplitude-only, phase-only, sign-only (BitNet)
 *   - Multi-angle quantization: fewer bits x more angles
 *   - The precision x width tradeoff matrix
 *
 * KEY PREDICTION from Demo 11: granny/square have identical amplitude,
 * so amplitude-only classification MUST fail. Phase carries the topology.
 *
 * BitNet parallel: LLMs work with 1.58-bit weights {-1, 0, +1}.
 * Can knot classification work with similar extreme quantization?
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
 * Complex arithmetic (from Demo 10/11)
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
 * State-sum bracket oracle (from Demo 10/11)
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
#define TOL 1e-9

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * Knot table (from Demo 11, with corrected connected-sum braids)
 * ================================================================ */

#define NUM_KNOTS 10
#define MAX_ANGLES 8

static Braid knots[NUM_KNOTS];
static const char *knames[NUM_KNOTS];

static void init_knots(void) {
    int k = 0;

    knots[k].n = 3; knots[k].len = 0;
    knames[k] = "unknot"; k++;

    knots[k].n = 3; knots[k].len = 3;
    knots[k].word[0] = 1; knots[k].word[1] = 2; knots[k].word[2] = 1;
    knames[k] = "trefoil"; k++;

    knots[k].n = 3; knots[k].len = 3;
    knots[k].word[0] = -1; knots[k].word[1] = -2; knots[k].word[2] = -1;
    knames[k] = "mirror trefoil"; k++;

    knots[k].n = 3; knots[k].len = 4;
    knots[k].word[0] = 1; knots[k].word[1] = -2;
    knots[k].word[2] = 1; knots[k].word[3] = -2;
    knames[k] = "figure-eight"; k++;

    knots[k].n = 3; knots[k].len = 5;
    knots[k].word[0] = 1; knots[k].word[1] = 2;
    knots[k].word[2] = 1; knots[k].word[3] = 2;
    knots[k].word[4] = 1;
    knames[k] = "torus(2,5)"; k++;

    knots[k].n = 3; knots[k].len = 7;
    knots[k].word[0] = 1; knots[k].word[1] = 2;
    knots[k].word[2] = 1; knots[k].word[3] = 2;
    knots[k].word[4] = 1; knots[k].word[5] = 2;
    knots[k].word[6] = 1;
    knames[k] = "torus(2,7)"; k++;

    /* granny = trefoil # trefoil: s1^3 * s2^3 */
    knots[k].n = 3; knots[k].len = 6;
    knots[k].word[0] = 1; knots[k].word[1] = 1; knots[k].word[2] = 1;
    knots[k].word[3] = 2; knots[k].word[4] = 2; knots[k].word[5] = 2;
    knames[k] = "granny"; k++;

    /* square = trefoil # mirror trefoil: s1^3 * s2^{-3} */
    knots[k].n = 3; knots[k].len = 6;
    knots[k].word[0] = 1; knots[k].word[1] = 1; knots[k].word[2] = 1;
    knots[k].word[3] = -2; knots[k].word[4] = -2; knots[k].word[5] = -2;
    knames[k] = "square"; k++;

    knots[k].n = 3; knots[k].len = 5;
    knots[k].word[0] = -1; knots[k].word[1] = -2;
    knots[k].word[2] = -1; knots[k].word[3] = -2;
    knots[k].word[4] = -1;
    knames[k] = "cinquefoil mirror"; k++;

    knots[k].n = 3; knots[k].len = 1;
    knots[k].word[0] = 1;
    knames[k] = "single crossing"; k++;
}

/* ================================================================
 * Greedy angle selection (from Demo 11)
 * ================================================================ */

#define NSAMP 256

static double greedy_angles[MAX_ANGLES];
static int n_greedy = 0;

static void select_greedy_angles(int max_sel) {
    Cx waveforms[NUM_KNOTS][NSAMP];
    int selected[MAX_ANGLES];
    int nsel = 0;
    int si, ki, kj, ai;
    double theta;

    /* Compute all waveforms */
    for (ki = 0; ki < NUM_KNOTS; ki++)
        for (si = 0; si < NSAMP; si++) {
            theta = 2.0 * M_PI * (double)si / (double)NSAMP;
            waveforms[ki][si] = braid_bracket_at(&knots[ki], cx_exp_i(theta));
        }

    /* Greedy selection — stop early once all pairs are separated */
    while (nsel < max_sel) {
        int best_si = 0;
        double best_worst = 0.0;
        int all_sep;

        for (si = 0; si < NSAMP; si++) {
            double worst_pair = 1e30;
            for (ki = 0; ki < NUM_KNOTS; ki++)
                for (kj = ki + 1; kj < NUM_KNOTS; kj++) {
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

        /* Don't early-stop: we want all angles for the tradeoff matrix.
         * Even after full-precision separation is achieved, extra angles
         * help low-bit quantization by adding independent dimensions. */
        (void)all_sep;
    }
    n_greedy = nsel;
}

/* ================================================================
 * Quantization functions
 * ================================================================ */

/* Quantize a double in [lo, hi] to B bits, then dequantize */
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

/* Quantize a complex number: re and im independently */
static Cx cx_quantize(Cx z, double re_lo, double re_hi,
                       double im_lo, double im_hi, int bits) {
    return cx_make(quantize(z.re, re_lo, re_hi, bits),
                   quantize(z.im, im_lo, im_hi, bits));
}

/* Sign-only: map to {-1, 0, +1} per component (ternary / 1.58-bit) */
static Cx cx_ternary(Cx z, double threshold) {
    double re_t, im_t;
    re_t = (z.re > threshold) ? 1.0 : (z.re < -threshold ? -1.0 : 0.0);
    im_t = (z.im > threshold) ? 1.0 : (z.im < -threshold ? -1.0 : 0.0);
    return cx_make(re_t, im_t);
}

/* Sign-only: quadrant (pure 1-bit per component) */
static Cx cx_sign(Cx z) {
    return cx_make(z.re >= 0.0 ? 1.0 : -1.0,
                   z.im >= 0.0 ? 1.0 : -1.0);
}

/* ================================================================
 * PART A: Compute reference signatures
 * ================================================================ */

static Cx ref_sigs[NUM_KNOTS][MAX_ANGLES];

static void compute_references(void) {
    int ki, ai;

    printf("\n=== PART A: Reference Signatures (%d knots, %d angles) ===\n",
           NUM_KNOTS, n_greedy);

    for (ki = 0; ki < NUM_KNOTS; ki++) {
        printf("  %-18s:", knames[ki]);
        for (ai = 0; ai < n_greedy; ai++) {
            ref_sigs[ki][ai] = braid_bracket_at(&knots[ki],
                                                 cx_exp_i(greedy_angles[ai]));
            printf("  (%.4f, %.4f)", ref_sigs[ki][ai].re, ref_sigs[ki][ai].im);
        }
        printf("\n");
    }
}

/* ================================================================
 * PART B: Bit-depth sweep
 * ================================================================ */

static int test_quantized_separation(int bits, int num_angles) {
    /* Compute quantization ranges from reference signatures */
    double re_lo, re_hi, im_lo, im_hi;
    Cx quantized[NUM_KNOTS][MAX_ANGLES];
    int ki, kj, ai, pairs_ok;

    re_lo = 1e30; re_hi = -1e30;
    im_lo = 1e30; im_hi = -1e30;

    for (ki = 0; ki < NUM_KNOTS; ki++)
        for (ai = 0; ai < num_angles; ai++) {
            if (ref_sigs[ki][ai].re < re_lo) re_lo = ref_sigs[ki][ai].re;
            if (ref_sigs[ki][ai].re > re_hi) re_hi = ref_sigs[ki][ai].re;
            if (ref_sigs[ki][ai].im < im_lo) im_lo = ref_sigs[ki][ai].im;
            if (ref_sigs[ki][ai].im > im_hi) im_hi = ref_sigs[ki][ai].im;
        }

    /* Add small margin to avoid boundary issues */
    {
        double re_margin = (re_hi - re_lo) * 0.01 + 1e-10;
        double im_margin = (im_hi - im_lo) * 0.01 + 1e-10;
        re_lo -= re_margin; re_hi += re_margin;
        im_lo -= im_margin; im_hi += im_margin;
    }

    /* Quantize all signatures */
    for (ki = 0; ki < NUM_KNOTS; ki++)
        for (ai = 0; ai < num_angles; ai++)
            quantized[ki][ai] = cx_quantize(ref_sigs[ki][ai],
                                             re_lo, re_hi, im_lo, im_hi, bits);

    /* Test all pairs */
    pairs_ok = 0;
    for (ki = 0; ki < NUM_KNOTS; ki++)
        for (kj = ki + 1; kj < NUM_KNOTS; kj++) {
            double dist2 = 0.0;
            for (ai = 0; ai < num_angles; ai++) {
                Cx d = cx_sub(quantized[ki][ai], quantized[kj][ai]);
                dist2 += d.re * d.re + d.im * d.im;
            }
            if (dist2 > 1e-20) pairs_ok++;
        }

    return pairs_ok;
}

static void bit_depth_sweep(void) {
    int bits_list[] = {16, 12, 8, 6, 4, 3, 2, 1};
    int n_bits = 8;
    int total_pairs = NUM_KNOTS * (NUM_KNOTS - 1) / 2;
    int bi, pairs_ok;
    char msg[120];
    int min_bits = -1;

    printf("\n=== PART B: Bit-Depth Sweep (1 angle, theta=%.4f*pi) ===\n",
           greedy_angles[0] / M_PI);
    printf("  %-8s %12s %8s\n", "Bits", "Pairs OK", "Total");

    for (bi = 0; bi < n_bits; bi++) {
        pairs_ok = test_quantized_separation(bits_list[bi], 1);
        printf("  %-8d %9d/%-2d %8s\n",
               bits_list[bi], pairs_ok, total_pairs,
               pairs_ok == total_pairs ? "PERFECT" : "LOSS");
        if (pairs_ok == total_pairs && min_bits < 0)
            min_bits = bits_list[bi];
    }

    /* The last perfect bit-depth is the minimum */
    /* Re-scan from low to high to find actual minimum */
    {
        int b;
        for (b = 1; b <= 16; b++) {
            pairs_ok = test_quantized_separation(b, 1);
            if (pairs_ok == total_pairs) {
                min_bits = b;
                break;
            }
        }
    }

    sprintf(msg, "minimum bits for 100%% classification at 1 angle: %d", min_bits);
    check(msg, min_bits > 0);
    printf("  Total weight size: %d knots x 1 angle x %d bits x 2 components = %d bits (%d bytes)\n",
           NUM_KNOTS, min_bits, NUM_KNOTS * min_bits * 2,
           (NUM_KNOTS * min_bits * 2 + 7) / 8);
}

/* ================================================================
 * PART C: Component analysis
 * ================================================================ */

static void component_analysis(void) {
    int ki, kj;
    int amp_ok = 0, phase_ok = 0, sign_ok = 0, ternary_ok = 0;
    int total_pairs = NUM_KNOTS * (NUM_KNOTS - 1) / 2;
    int amp_confused[2] = {-1, -1};
    int sign_confused[2] = {-1, -1};
    char msg[120];

    printf("\n=== PART C: Component Analysis (1 angle) ===\n");

    /* Amplitude-only: classify by |bracket| */
    for (ki = 0; ki < NUM_KNOTS; ki++)
        for (kj = ki + 1; kj < NUM_KNOTS; kj++) {
            double a1 = cx_abs(ref_sigs[ki][0]);
            double a2 = cx_abs(ref_sigs[kj][0]);
            if (fabs(a1 - a2) > 1e-6) {
                amp_ok++;
            } else if (amp_confused[0] < 0) {
                amp_confused[0] = ki;
                amp_confused[1] = kj;
            }
        }

    /* Phase-only: classify by arg(bracket) */
    for (ki = 0; ki < NUM_KNOTS; ki++)
        for (kj = ki + 1; kj < NUM_KNOTS; kj++) {
            double p1 = atan2(ref_sigs[ki][0].im, ref_sigs[ki][0].re);
            double p2 = atan2(ref_sigs[kj][0].im, ref_sigs[kj][0].re);
            double dp = fabs(p1 - p2);
            if (dp > M_PI) dp = 2.0 * M_PI - dp;
            if (dp > 1e-6) phase_ok++;
        }

    /* Sign-only (quadrant): sign(re), sign(im) → 4 bins */
    for (ki = 0; ki < NUM_KNOTS; ki++)
        for (kj = ki + 1; kj < NUM_KNOTS; kj++) {
            Cx s1 = cx_sign(ref_sigs[ki][0]);
            Cx s2 = cx_sign(ref_sigs[kj][0]);
            if (s1.re != s2.re || s1.im != s2.im) {
                sign_ok++;
            } else if (sign_confused[0] < 0) {
                sign_confused[0] = ki;
                sign_confused[1] = kj;
            }
        }

    /* Ternary: {-1, 0, +1} per component, threshold = 0.5 */
    for (ki = 0; ki < NUM_KNOTS; ki++)
        for (kj = ki + 1; kj < NUM_KNOTS; kj++) {
            Cx t1 = cx_ternary(ref_sigs[ki][0], 0.5);
            Cx t2 = cx_ternary(ref_sigs[kj][0], 0.5);
            if (t1.re != t2.re || t1.im != t2.im) ternary_ok++;
        }

    printf("  Amplitude-only: %d/%d pairs separated", amp_ok, total_pairs);
    if (amp_confused[0] >= 0)
        printf(" (first collision: %s vs %s)", knames[amp_confused[0]], knames[amp_confused[1]]);
    printf("\n");

    sprintf(msg, "amplitude-only fails (granny/square predicted collision)");
    check(msg, amp_ok < total_pairs);

    printf("  Phase-only:     %d/%d pairs separated\n", phase_ok, total_pairs);
    sprintf(msg, "neither amplitude nor phase alone suffices (amp=%d, phase=%d, need=%d)",
            amp_ok, phase_ok, total_pairs);
    check(msg, amp_ok < total_pairs && phase_ok < total_pairs);

    printf("  Sign-only:      %d/%d pairs separated", sign_ok, total_pairs);
    if (sign_confused[0] >= 0)
        printf(" (first collision: %s vs %s)", knames[sign_confused[0]], knames[sign_confused[1]]);
    printf("\n");

    printf("  Ternary:        %d/%d pairs separated\n", ternary_ok, total_pairs);
}

/* ================================================================
 * PART D: Precision x Width tradeoff matrix
 * ================================================================ */

static void precision_width_matrix(void) {
    int bits_list[] = {1, 2, 3, 4, 8};
    int angle_list[] = {1, 2, 3, 4, 8};
    int n_bits = 5, n_angles = 5;
    int total_pairs = NUM_KNOTS * (NUM_KNOTS - 1) / 2;
    int bi, ai, pairs_ok;
    int min_total_bits = -1;
    int min_b = 0, min_m = 0;

    printf("\n=== PART D: Precision x Width Tradeoff Matrix ===\n");
    printf("  (cells = pairs separated out of %d, * = 100%%)\n\n", total_pairs);

    printf("  %8s", "Bits\\Ang");
    for (ai = 0; ai < n_angles; ai++)
        printf(" %6dA", angle_list[ai]);
    printf("   total_bits\n");

    for (bi = 0; bi < n_bits; bi++) {
        printf("  %6dB ", bits_list[bi]);
        for (ai = 0; ai < n_angles; ai++) {
            int M = angle_list[ai];
            if (M > n_greedy) {
                printf("  %4s  ", "---");
                continue;
            }
            pairs_ok = test_quantized_separation(bits_list[bi], M);
            if (pairs_ok == total_pairs) {
                printf("  %2d* ", pairs_ok);
                {
                    int total = bits_list[bi] * M * 2;
                    if (min_total_bits < 0 || total < min_total_bits) {
                        min_total_bits = total;
                        min_b = bits_list[bi];
                        min_m = M;
                    }
                }
            } else {
                printf("  %2d  ", pairs_ok);
            }
        }
        printf("    %d*M*2\n", bits_list[bi]);
    }

    printf("\n  Pareto optimum: %d-bit x %d angles = %d total bits per knot\n",
           min_b, min_m, min_total_bits);
    printf("  Weight table size: %d knots x %d bits = %d bits (%d bytes)\n",
           NUM_KNOTS, min_total_bits,
           NUM_KNOTS * min_total_bits,
           (NUM_KNOTS * min_total_bits + 7) / 8);

    {
        char msg[120];
        sprintf(msg, "Pareto optimum found: %d bits per knot", min_total_bits);
        check(msg, min_total_bits > 0 && min_total_bits <= 128);
    }
}

/* ================================================================
 * PART E: The compiled weight table
 * ================================================================ */

static void print_weight_table(void) {
    int ki, ai;
    /* Use the Pareto-optimal quantization found in Part D.
     * For now, print both full precision and the minimum-bits version. */
    int best_bits;
    int total_pairs = NUM_KNOTS * (NUM_KNOTS - 1) / 2;

    /* Find minimum bits at 1 angle */
    for (best_bits = 1; best_bits <= 16; best_bits++) {
        if (test_quantized_separation(best_bits, 1) == total_pairs)
            break;
    }

    printf("\n=== PART E: Compiled Weight Table ===\n");
    printf("  Configuration: %d-bit quantization, 1 angle (theta=%.4f*pi)\n\n",
           best_bits, greedy_angles[0] / M_PI);

    /* Compute quantization ranges */
    {
        double re_lo = 1e30, re_hi = -1e30, im_lo = 1e30, im_hi = -1e30;
        double re_margin, im_margin;

        for (ki = 0; ki < NUM_KNOTS; ki++) {
            if (ref_sigs[ki][0].re < re_lo) re_lo = ref_sigs[ki][0].re;
            if (ref_sigs[ki][0].re > re_hi) re_hi = ref_sigs[ki][0].re;
            if (ref_sigs[ki][0].im < im_lo) im_lo = ref_sigs[ki][0].im;
            if (ref_sigs[ki][0].im > im_hi) im_hi = ref_sigs[ki][0].im;
        }
        re_margin = (re_hi - re_lo) * 0.01 + 1e-10;
        im_margin = (im_hi - im_lo) * 0.01 + 1e-10;
        re_lo -= re_margin; re_hi += re_margin;
        im_lo -= im_margin; im_hi += im_margin;

        printf("  Quantization range: re=[%.4f, %.4f]  im=[%.4f, %.4f]\n\n",
               re_lo, re_hi, im_lo, im_hi);

        printf("  %-18s %12s %12s  ->  %8s %8s\n",
               "Knot", "re (full)", "im (full)", "re (q)", "im (q)");

        for (ki = 0; ki < NUM_KNOTS; ki++) {
            Cx q = cx_quantize(ref_sigs[ki][0], re_lo, re_hi, im_lo, im_hi, best_bits);
            printf("  %-18s %12.6f %12.6f  ->  %8.4f %8.4f\n",
                   knames[ki], ref_sigs[ki][0].re, ref_sigs[ki][0].im,
                   q.re, q.im);
        }

        printf("\n  Total model size: %d knots x %d bits x 2 = %d bits (%d bytes)\n",
               NUM_KNOTS, best_bits, NUM_KNOTS * best_bits * 2,
               (NUM_KNOTS * best_bits * 2 + 7) / 8);

        /* Multi-angle table if helpful */
        if (n_greedy > 1) {
            printf("\n  Multi-angle weight vectors (full precision, %d angles):\n", n_greedy);
            for (ki = 0; ki < NUM_KNOTS; ki++) {
                printf("  %-18s", knames[ki]);
                for (ai = 0; ai < n_greedy && ai < 4; ai++)
                    printf("  (%.3f,%.3f)", ref_sigs[ki][ai].re, ref_sigs[ki][ai].im);
                printf("\n");
            }
        }
    }
}

/* ================================================================
 * PART F: Nearest-neighbor classifier + invariance test
 * ================================================================ */

static int classify_braid(const Braid *b, int num_angles, int bits) {
    int ai, ki, best_ki;
    double best_dist, dist2;
    Cx vals[MAX_ANGLES];
    double re_lo, re_hi, im_lo, im_hi, re_margin, im_margin;

    /* Compute bracket at each angle */
    for (ai = 0; ai < num_angles; ai++)
        vals[ai] = braid_bracket_at(b, cx_exp_i(greedy_angles[ai]));

    /* Compute quantization ranges from reference table */
    re_lo = 1e30; re_hi = -1e30;
    im_lo = 1e30; im_hi = -1e30;
    for (ki = 0; ki < NUM_KNOTS; ki++)
        for (ai = 0; ai < num_angles; ai++) {
            if (ref_sigs[ki][ai].re < re_lo) re_lo = ref_sigs[ki][ai].re;
            if (ref_sigs[ki][ai].re > re_hi) re_hi = ref_sigs[ki][ai].re;
            if (ref_sigs[ki][ai].im < im_lo) im_lo = ref_sigs[ki][ai].im;
            if (ref_sigs[ki][ai].im > im_hi) im_hi = ref_sigs[ki][ai].im;
        }
    re_margin = (re_hi - re_lo) * 0.01 + 1e-10;
    im_margin = (im_hi - im_lo) * 0.01 + 1e-10;
    re_lo -= re_margin; re_hi += re_margin;
    im_lo -= im_margin; im_hi += im_margin;

    /* Quantize input */
    if (bits > 0) {
        for (ai = 0; ai < num_angles; ai++)
            vals[ai] = cx_quantize(vals[ai], re_lo, re_hi, im_lo, im_hi, bits);
    }

    /* Find nearest reference */
    best_ki = 0;
    best_dist = 1e30;
    for (ki = 0; ki < NUM_KNOTS; ki++) {
        dist2 = 0.0;
        for (ai = 0; ai < num_angles; ai++) {
            Cx ref_q = (bits > 0)
                ? cx_quantize(ref_sigs[ki][ai], re_lo, re_hi, im_lo, im_hi, bits)
                : ref_sigs[ki][ai];
            Cx d = cx_sub(vals[ai], ref_q);
            dist2 += d.re * d.re + d.im * d.im;
        }
        if (dist2 < best_dist) {
            best_dist = dist2;
            best_ki = ki;
        }
    }
    return best_ki;
}

static void classifier_test(void) {
    int ki, cls;
    char msg[120];
    int all_correct;
    Braid alt_trefoil, alt_fig8;

    printf("\n=== PART F: Nearest-Neighbor Classifier ===\n");

    /* Test all known braids */
    printf("  Self-classification (full precision, 1 angle):\n");
    all_correct = 1;
    for (ki = 0; ki < NUM_KNOTS; ki++) {
        cls = classify_braid(&knots[ki], 1, 0);
        printf("    %-18s -> %s %s\n", knames[ki], knames[cls],
               cls == ki ? "" : "WRONG!");
        if (cls != ki) all_correct = 0;
    }
    check("all knots self-classify correctly (full precision)", all_correct);

    /* Invariance test: alternate braid for trefoil */
    /* trefoil = s2*s1*s2 (equivalent by braid relation) */
    alt_trefoil.n = 3; alt_trefoil.len = 3;
    alt_trefoil.word[0] = 2; alt_trefoil.word[1] = 1; alt_trefoil.word[2] = 2;
    cls = classify_braid(&alt_trefoil, 1, 0);
    sprintf(msg, "alt trefoil (s2*s1*s2) classifies as trefoil");
    check(msg, cls == 1);

    /* figure-eight alternate: s1*s2^{-1}*s1*s2^{-1} == s2^{-1}*s1*s2^{-1}*s1 */
    alt_fig8.n = 3; alt_fig8.len = 4;
    alt_fig8.word[0] = -2; alt_fig8.word[1] = 1;
    alt_fig8.word[2] = -2; alt_fig8.word[3] = 1;
    cls = classify_braid(&alt_fig8, 1, 0);
    sprintf(msg, "alt figure-eight (s2^-1*s1*s2^-1*s1) classifies as figure-eight");
    check(msg, cls == 3);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 12: Quantization\n");
    printf("================================\n");

    init_knots();

    printf("\n--- Phase 0: Selecting greedy angles ---\n");
    select_greedy_angles(MAX_ANGLES);
    {
        int ai;
        printf("  Selected %d angles:\n", n_greedy);
        for (ai = 0; ai < n_greedy; ai++)
            printf("    theta_%d = %.6f (%.4f*pi)\n",
                   ai, greedy_angles[ai], greedy_angles[ai] / M_PI);
    }

    compute_references();
    bit_depth_sweep();
    component_analysis();
    precision_width_matrix();
    print_weight_table();
    classifier_test();

    printf("\n================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("================================\n");
    return n_fail > 0 ? 1 : 0;
}
