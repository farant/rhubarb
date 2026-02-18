/*
 * KNOTAPEL DEMO 27: Complex-Valued Reverse DKC
 * ==============================================
 *
 * THESIS: Demo 26 showed trained real NN weights match bracket AMPLITUDES
 * at delta=0 with RMS=0.2641 (1.39x over random). But |<b>(A)| discards
 * phase -- a Born-rule projection C -> R>=0. A complex-valued neural
 * network should match bracket VALUES including phase.
 *
 * KEY INSIGHT: At A = e^{i5pi/4}, bracket values are elements of the
 * cyclotomic ring Z[zeta_8] (sums of 8th roots of unity). Demo 26's
 * integer amplitudes are NORMS of these cyclotomic integers.
 *
 * Part A: Complex XOR -- single complex neuron with split-sigmoid
 * Part B: Complex bracket catalog (full Cx values, not just amplitudes)
 * Part C: Complex decomposition (complex distance matching)
 * Part D: Phase analysis (phase structure of matched values)
 * Part E: Degeneracy test (does phase break 430:1 degeneracy?)
 * Part F: Real vs complex comparison (side-by-side)
 *
 * C89, zero dependencies beyond math.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ================================================================
 * Complex arithmetic (from Demo 26)
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
static double cx_phase(Cx a) { return atan2(a.im, a.re); }
static Cx cx_exp_i(double theta) { return cx_make(cos(theta), sin(theta)); }
static Cx cx_scale(Cx a, double s) { return cx_make(a.re * s, a.im * s); }


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
 * State-sum bracket oracle (from Demo 26)
 * Now returns FULL complex value, not just amplitude
 * ================================================================ */

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

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
 * RNG (simple LCG for reproducibility)
 * ================================================================ */

static unsigned long rng_state = 12345;

static void rng_seed(unsigned long s) { rng_state = s; }

static double rng_uniform(void) {
    rng_state = rng_state * 1103515245UL + 12345UL;
    return (double)((rng_state >> 16) & 0x7FFF) / 32768.0;
}

static double rng_normal(void) {
    double u1 = rng_uniform() + 1e-10;
    double u2 = rng_uniform();
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

/* ================================================================
 * Activation functions
 * ================================================================ */

static double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }
static double sigmoid_deriv(double s) { return s * (1.0 - s); }

/* Split-sigmoid: apply sigmoid independently to Re and Im */
static Cx cx_sigmoid(Cx z) {
    return cx_make(sigmoid(z.re), sigmoid(z.im));
}



/* ================================================================
 * PART A: Complex-Valued XOR Network
 *
 * Architecture: single complex neuron (Nitta 2003)
 *   z = w1*x1 + w2*x2 + b    (w1, w2, b in C; x1, x2 in R)
 *   h = split_sigmoid(z)       (apply sigmoid to Re and Im independently)
 *   p = h.re*(1-h.im) + (1-h.re)*h.im   (smooth XOR of two classifiers)
 *
 * The split-sigmoid creates TWO orthogonal decision boundaries.
 * Re(h) classifies one linear feature, Im(h) classifies another.
 * XOR = exactly one fires. p is the smooth version of this.
 *
 * 6 real parameters: w1.re, w1.im, w2.re, w2.im, b.re, b.im
 * ================================================================ */

typedef struct {
    Cx w1, w2, b;  /* 2 complex weights + 1 complex bias = 6 real params */
} CxNeuron;

static void cx_neuron_init(CxNeuron *net) {
    net->w1 = cx_make(rng_normal() * 1.0, rng_normal() * 1.0);
    net->w2 = cx_make(rng_normal() * 1.0, rng_normal() * 1.0);
    net->b  = cx_make(rng_normal() * 0.5, rng_normal() * 0.5);
}

static Cx cx_neuron_forward(const CxNeuron *net, double x1, double x2) {
    Cx z = cx_add(cx_add(cx_scale(net->w1, x1), cx_scale(net->w2, x2)), net->b);
    return cx_sigmoid(z);
}

/* Smooth XOR: p = h.re*(1-h.im) + (1-h.re)*h.im */
static double cx_neuron_xor_output(const CxNeuron *net, double x1, double x2) {
    Cx h = cx_neuron_forward(net, x1, x2);
    return h.re * (1.0 - h.im) + (1.0 - h.re) * h.im;
}

static int cx_neuron_predict(const CxNeuron *net, double x1, double x2) {
    return cx_neuron_xor_output(net, x1, x2) > 0.5 ? 1 : 0;
}

/* Train single complex neuron on XOR using smooth XOR decision */
static int cx_neuron_train_xor(CxNeuron *net, double lr, int max_epochs) {
    double inputs[4][2] = {{0,0}, {0,1}, {1,0}, {1,1}};
    double targets[4]   = {0, 1, 1, 0};
    int epoch, i, correct;

    for (epoch = 0; epoch < max_epochs; epoch++) {
        double total_loss = 0.0;
        correct = 0;

        for (i = 0; i < 4; i++) {
            double x1 = inputs[i][0];
            double x2 = inputs[i][1];
            double target = targets[i];

            /* Forward */
            Cx z = cx_add(cx_add(cx_scale(net->w1, x1),
                                  cx_scale(net->w2, x2)), net->b);
            Cx h = cx_sigmoid(z);

            /* Smooth XOR output */
            double p = h.re * (1.0 - h.im) + (1.0 - h.re) * h.im;
            double err = p - target;
            total_loss += err * err;

            if ((p > 0.5) == (target > 0.5)) correct++;

            /* Backprop:
             * p = h.re - 2*h.re*h.im + h.im
             * dp/d(h.re) = 1 - 2*h.im
             * dp/d(h.im) = 1 - 2*h.re
             * dL/dp = 2*err
             * dL/d(h.re) = 2*err*(1-2*h.im)
             * dL/d(h.im) = 2*err*(1-2*h.re)
             * dL/d(z.re) = dL/d(h.re) * h.re*(1-h.re)
             * dL/d(z.im) = dL/d(h.im) * h.im*(1-h.im)
             */
            {
                double dp_dhr = 1.0 - 2.0 * h.im;
                double dp_dhi = 1.0 - 2.0 * h.re;
                double dL_dhr = 2.0 * err * dp_dhr;
                double dL_dhi = 2.0 * err * dp_dhi;
                double dz_re = dL_dhr * h.re * (1.0 - h.re);
                double dz_im = dL_dhi * h.im * (1.0 - h.im);

                net->w1.re -= lr * dz_re * x1;
                net->w1.im -= lr * dz_im * x1;
                net->w2.re -= lr * dz_re * x2;
                net->w2.im -= lr * dz_im * x2;
                net->b.re  -= lr * dz_re;
                net->b.im  -= lr * dz_im;
            }
        }

        if (correct == 4 && total_loss < 0.01)
            return epoch + 1;
    }
    return 0;
}

#define NUM_CX_TRIALS 20

static void part_a_complex_xor(CxNeuron *best_net, int *best_found) {
    int trial, converged_count = 0;
    int best_epoch = 999999;
    char msg[256];

    printf("\n=== PART A: Complex-Valued XOR (Single Complex Neuron) ===\n");
    printf("  Architecture: 2 complex inputs -> 1 complex neuron (split-sigmoid)\n");
    printf("  Parameters: 6 real (w1_re, w1_im, w2_re, w2_im, b_re, b_im)\n\n");

    *best_found = 0;

    for (trial = 0; trial < NUM_CX_TRIALS; trial++) {
        CxNeuron net;
        int epoch;

        rng_seed((unsigned long)(trial * 7919 + 42));
        cx_neuron_init(&net);
        epoch = cx_neuron_train_xor(&net, 0.5, 50000);

        if (epoch > 0) {
            converged_count++;
            printf("  Trial %2d: CONVERGED at epoch %d\n", trial, epoch);
            printf("    w1=(%.4f,%.4f) w2=(%.4f,%.4f) b=(%.4f,%.4f)\n",
                   net.w1.re, net.w1.im, net.w2.re, net.w2.im,
                   net.b.re, net.b.im);

            /* Verify all 4 inputs */
            {
                int i, ok = 1;
                for (i = 0; i < 4; i++) {
                    double x1 = (i >> 1) & 1 ? 1.0 : 0.0;
                    double x2 = i & 1 ? 1.0 : 0.0;
                    int pred = cx_neuron_predict(&net, x1, x2);
                    int tgt = ((i >> 1) ^ (i & 1));
                    if (pred != tgt) ok = 0;
                }
                if (ok && epoch < best_epoch) {
                    *best_net = net;
                    best_epoch = epoch;
                    *best_found = 1;
                }
            }
        } else {
            printf("  Trial %2d: did not converge\n", trial);
        }
    }

    printf("\n  Summary: %d/%d trials converged\n", converged_count, NUM_CX_TRIALS);

    if (*best_found) {
        printf("  Best network (epoch %d):\n", best_epoch);
        printf("    w1 = %.6f + %.6fi  (|w1|=%.6f, phase=%.4f)\n",
               best_net->w1.re, best_net->w1.im,
               cx_abs(best_net->w1), cx_phase(best_net->w1));
        printf("    w2 = %.6f + %.6fi  (|w2|=%.6f, phase=%.4f)\n",
               best_net->w2.re, best_net->w2.im,
               cx_abs(best_net->w2), cx_phase(best_net->w2));
        printf("    b  = %.6f + %.6fi  (|b|=%.6f, phase=%.4f)\n",
               best_net->b.re, best_net->b.im,
               cx_abs(best_net->b), cx_phase(best_net->b));

        /* Show all outputs */
        {
            int i;
            printf("\n  Verification:\n");
            for (i = 0; i < 4; i++) {
                double x1 = (i >> 1) & 1 ? 1.0 : 0.0;
                double x2 = i & 1 ? 1.0 : 0.0;
                Cx h = cx_neuron_forward(best_net, x1, x2);
                double p = cx_neuron_xor_output(best_net, x1, x2);
                int tgt = ((i >> 1) ^ (i & 1));
                printf("    XOR(%d,%d)=%d  h=(%.4f,%.4f)  p=%.4f  pred=%d\n",
                       (int)x1, (int)x2, tgt, h.re, h.im,
                       p, cx_neuron_predict(best_net, x1, x2));
            }
        }
    }

    sprintf(msg, "complex neuron solves XOR (%d/%d converged)",
            converged_count, NUM_CX_TRIALS);
    check(msg, *best_found);
}

/* ================================================================
 * PART B: Complex Bracket Catalog
 *
 * Same enumeration as Demo 26, but store FULL complex bracket values.
 * At delta=0, only single-loop states survive.
 * ================================================================ */

typedef struct {
    Braid braid;
    Cx bracket;       /* full complex bracket value */
    double amplitude;  /* |bracket| for comparison with Demo 26 */
    double phase;      /* arg(bracket) */
} CxCatalogEntry;

#define MAX_CX_CATALOG 8192

static CxCatalogEntry cx_catalog[MAX_CX_CATALOG];
static int cx_catalog_size = 0;

static void build_complex_catalog(Cx A, int max_strands, int max_len) {
    int n, len;
    Braid b;
    int word[MAX_WORD];

    cx_catalog_size = 0;

    for (n = 2; n <= max_strands; n++) {
        for (len = 1; len <= max_len && len <= MAX_WORD; len++) {
            /* Enumerate all braid words of length `len` on `n` strands */
            int max_gen = n - 1;
            int total_gens = 2 * max_gen; /* positive and negative */
            unsigned long total, idx;
            int i;

            /* Total combinations = total_gens^len */
            total = 1;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)total_gens;
                if (total > 100000) break; /* cap enumeration */
            }
            if (total > 100000) continue;

            for (idx = 0; idx < total; idx++) {
                unsigned long tmp = idx;
                Cx bracket;
                double amp;

                /* Decode index to braid word */
                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen)
                        word[i] = g + 1;         /* +1, +2, ... */
                    else
                        word[i] = -(g - max_gen + 1); /* -1, -2, ... */
                }

                b.n = n;
                b.len = len;
                memcpy(b.word, word, (size_t)len * sizeof(int));

                bracket = braid_bracket_at(&b, A);
                amp = cx_abs(bracket);

                /* Only keep braids with non-trivial amplitude */
                if (amp > 0.5 && cx_catalog_size < MAX_CX_CATALOG) {
                    cx_catalog[cx_catalog_size].braid = b;
                    cx_catalog[cx_catalog_size].bracket = bracket;
                    cx_catalog[cx_catalog_size].amplitude = amp;
                    cx_catalog[cx_catalog_size].phase = cx_phase(bracket);
                    cx_catalog_size++;
                }
            }
        }
    }
}

static void part_b_complex_catalog(void) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int i, n_distinct_amp = 0, n_distinct_cx = 0;
    double seen_amps[64];
    int n_seen_amps = 0;
    Cx seen_cx[512];
    int n_seen_cx = 0;
    char msg[256];

    printf("\n=== PART B: Complex Bracket Catalog ===\n");
    printf("  A = e^{i*5pi/4}, delta = 0\n");
    printf("  Enumerating 2-3 strand braids up to length 8...\n\n");

    build_complex_catalog(A, 3, 8);

    printf("  Catalog size: %d braids\n", cx_catalog_size);

    /* Count distinct amplitudes (Demo 26 found 14-17) */
    for (i = 0; i < cx_catalog_size; i++) {
        double amp = cx_catalog[i].amplitude;
        int j, found = 0;
        for (j = 0; j < n_seen_amps; j++) {
            if (fabs(amp - seen_amps[j]) < 0.01) { found = 1; break; }
        }
        if (!found && n_seen_amps < 64) {
            seen_amps[n_seen_amps++] = amp;
        }
    }
    n_distinct_amp = n_seen_amps;

    /* Count distinct complex values (should be MORE than amplitudes) */
    for (i = 0; i < cx_catalog_size; i++) {
        Cx val = cx_catalog[i].bracket;
        int j, found = 0;
        for (j = 0; j < n_seen_cx; j++) {
            if (cx_abs(cx_sub(val, seen_cx[j])) < 0.01) { found = 1; break; }
        }
        if (!found && n_seen_cx < 512) {
            seen_cx[n_seen_cx++] = val;
        }
    }
    n_distinct_cx = n_seen_cx;

    printf("  Distinct amplitudes: %d\n", n_distinct_amp);
    printf("  Distinct complex values: %d\n", n_distinct_cx);
    printf("  Constellation expansion: %.1fx\n",
           n_distinct_amp > 0 ? (double)n_distinct_cx / (double)n_distinct_amp : 0.0);

    /* Show some example complex values */
    printf("\n  Sample complex bracket values:\n");
    for (i = 0; i < n_seen_cx && i < 20; i++) {
        printf("    %2d: %.4f + %.4fi  (|z|=%.4f, phase=%.4f)\n",
               i, seen_cx[i].re, seen_cx[i].im,
               cx_abs(seen_cx[i]), cx_phase(seen_cx[i]));
    }
    if (n_seen_cx > 20)
        printf("    ... (%d more)\n", n_seen_cx - 20);

    /* Check if values are cyclotomic integers (sums of 8th roots) */
    {
        int n_near_lattice = 0;
        /* Z[zeta_8] points have coords that are multiples of 1/sqrt(2) */
        double inv_sqrt2 = 1.0 / sqrt(2.0);
        for (i = 0; i < n_seen_cx; i++) {
            double re_scaled = seen_cx[i].re / inv_sqrt2;
            double im_scaled = seen_cx[i].im / inv_sqrt2;
            double re_frac = fabs(re_scaled - floor(re_scaled + 0.5));
            double im_frac = fabs(im_scaled - floor(im_scaled + 0.5));
            /* Also check if on integer lattice directly */
            double re_int_frac = fabs(seen_cx[i].re - floor(seen_cx[i].re + 0.5));
            double im_int_frac = fabs(seen_cx[i].im - floor(seen_cx[i].im + 0.5));
            if ((re_frac < 0.01 && im_frac < 0.01) ||
                (re_int_frac < 0.01 && im_int_frac < 0.01)) {
                n_near_lattice++;
            }
        }
        printf("\n  Cyclotomic lattice test: %d/%d values near Z[zeta_8] or Z[i]\n",
               n_near_lattice, n_seen_cx);
        sprintf(msg, "bracket values are cyclotomic (%d/%d near lattice)",
                n_near_lattice, n_seen_cx);
        check(msg, n_near_lattice > n_seen_cx / 2);
    }

    sprintf(msg, "complex catalog has more distinct values than amplitude-only (%d > %d)",
            n_distinct_cx, n_distinct_amp);
    check(msg, n_distinct_cx > n_distinct_amp);

    /* Compute average degeneracy for amplitude vs complex */
    {
        double amp_deg = (double)cx_catalog_size / (double)(n_distinct_amp > 0 ? n_distinct_amp : 1);
        double cx_deg = (double)cx_catalog_size / (double)(n_distinct_cx > 0 ? n_distinct_cx : 1);
        printf("\n  Average degeneracy:\n");
        printf("    Amplitude-only: %.1f:1 (%d braids / %d amplitudes)\n",
               amp_deg, cx_catalog_size, n_distinct_amp);
        printf("    Complex values: %.1f:1 (%d braids / %d values)\n",
               cx_deg, cx_catalog_size, n_distinct_cx);
        printf("    Degeneracy reduction: %.1fx\n",
               cx_deg > 0 ? amp_deg / cx_deg : 0.0);
    }
}

/* ================================================================
 * PART C: Complex Decomposition
 *
 * Match each complex weight to nearest complex bracket value.
 * Compare complex RMS to Demo 26's amplitude-only RMS.
 * Apply U(1) gauge correction.
 * ================================================================ */

static void find_nearest_cx(Cx target, int *best_idx, double *best_dist) {
    int i;
    *best_idx = -1;
    *best_dist = 1e30;
    for (i = 0; i < cx_catalog_size; i++) {
        double d = cx_abs(cx_sub(target, cx_catalog[i].bracket));
        if (d < *best_dist) {
            *best_dist = d;
            *best_idx = i;
        }
    }
}

static void find_nearest_amp(double target_amp, int *best_idx, double *best_dist) {
    int i;
    *best_idx = -1;
    *best_dist = 1e30;
    for (i = 0; i < cx_catalog_size; i++) {
        double d = fabs(target_amp - cx_catalog[i].amplitude);
        if (d < *best_dist) {
            *best_dist = d;
            *best_idx = i;
        }
    }
}

static void part_c_complex_decomposition(const CxNeuron *net) {
    Cx weights[3];
    const char *names[3] = {"w1", "w2", "b "};
    int i;
    double cx_rms = 0.0, amp_rms = 0.0;
    char msg[256];
    int n_weights = 3;

    /* Also try with U(1) gauge correction */
    double best_gauge_angle = 0.0;
    double best_gauge_rms = 1e30;
    int gi;

    printf("\n=== PART C: Complex Decomposition ===\n");

    if (cx_catalog_size == 0) {
        printf("  No catalog available. Skipping.\n");
        return;
    }

    weights[0] = net->w1;
    weights[1] = net->w2;
    weights[2] = net->b;

    /* Direct complex matching (no gauge correction) */
    printf("  Direct complex matching:\n");
    for (i = 0; i < n_weights; i++) {
        int cx_idx, amp_idx;
        double cx_dist, amp_dist;
        Cx w = weights[i];

        find_nearest_cx(w, &cx_idx, &cx_dist);
        find_nearest_amp(cx_abs(w), &amp_idx, &amp_dist);

        printf("    %s = (%.4f,%.4f)  |w|=%.4f\n",
               names[i], w.re, w.im, cx_abs(w));
        printf("       Cx match: (%.4f,%.4f) dist=%.4f  braid_len=%d\n",
               cx_catalog[cx_idx].bracket.re, cx_catalog[cx_idx].bracket.im,
               cx_dist, cx_catalog[cx_idx].braid.len);
        printf("      Amp match: amp=%.4f dist=%.4f  braid_len=%d\n",
               cx_catalog[amp_idx].amplitude, amp_dist,
               cx_catalog[amp_idx].braid.len);

        cx_rms += cx_dist * cx_dist;
        amp_rms += amp_dist * amp_dist;
    }

    cx_rms = sqrt(cx_rms / (double)n_weights);
    amp_rms = sqrt(amp_rms / (double)n_weights);

    printf("\n  RMS (no gauge correction):\n");
    printf("    Complex RMS:   %.6f\n", cx_rms);
    printf("    Amplitude RMS: %.6f\n", amp_rms);
    printf("    Demo 26 RMS:   0.2641 (reference)\n");

    /* U(1) gauge sweep: rotate all weights by e^{i*theta} */
    printf("\n  U(1) gauge sweep (360 angles):\n");
    for (gi = 0; gi < 360; gi++) {
        double theta = 2.0 * M_PI * (double)gi / 360.0;
        Cx rot = cx_exp_i(theta);
        double total = 0.0;

        for (i = 0; i < n_weights; i++) {
            Cx w_rot = cx_mul(weights[i], rot);
            int idx;
            double dist;
            find_nearest_cx(w_rot, &idx, &dist);
            total += dist * dist;
        }
        total = sqrt(total / (double)n_weights);

        if (total < best_gauge_rms) {
            best_gauge_rms = total;
            best_gauge_angle = theta;
        }
    }

    printf("    Best gauge angle: %.4f rad (%.1f deg)\n",
           best_gauge_angle, best_gauge_angle * 180.0 / M_PI);
    printf("    Gauge-corrected complex RMS: %.6f\n", best_gauge_rms);

    /* Show gauge-corrected matches */
    {
        Cx rot = cx_exp_i(best_gauge_angle);
        printf("\n  Gauge-corrected matches:\n");
        for (i = 0; i < n_weights; i++) {
            Cx w_rot = cx_mul(weights[i], rot);
            int idx;
            double dist;
            find_nearest_cx(w_rot, &idx, &dist);
            printf("    %s_rot = (%.4f,%.4f)  -> bracket (%.4f,%.4f)  dist=%.4f\n",
                   names[i], w_rot.re, w_rot.im,
                   cx_catalog[idx].bracket.re, cx_catalog[idx].bracket.im,
                   dist);
        }
    }

    sprintf(msg, "P1: gauge-corrected complex RMS < 0.1 (got %.4f)", best_gauge_rms);
    check(msg, best_gauge_rms < 0.1);

    sprintf(msg, "complex RMS better than amplitude RMS (%.4f vs %.4f)",
            best_gauge_rms, amp_rms);
    check(msg, best_gauge_rms < amp_rms);
}

/* ================================================================
 * PART D: Phase Analysis
 *
 * Analyze the phase structure of bracket values in the catalog.
 * Do phases cluster at specific angles? Is phase related to writhe?
 * ================================================================ */

static void part_d_phase_analysis(void) {
    int i;
    int phase_bins[8]; /* 8 sectors of 45 degrees each = 8th roots */
    int max_bin = 0, min_bin = 999999;
    char msg[256];

    printf("\n=== PART D: Phase Analysis ===\n");

    if (cx_catalog_size == 0) {
        printf("  No catalog available. Skipping.\n");
        return;
    }

    /* Bin phases into 8 sectors (matching 8th roots of unity) */
    memset(phase_bins, 0, sizeof(phase_bins));
    for (i = 0; i < cx_catalog_size; i++) {
        double ph = cx_catalog[i].phase;
        int bin;
        /* Normalize to [0, 2pi) */
        while (ph < 0) ph += 2.0 * M_PI;
        while (ph >= 2.0 * M_PI) ph -= 2.0 * M_PI;
        bin = (int)(ph / (M_PI / 4.0));
        if (bin >= 8) bin = 7;
        phase_bins[bin]++;
    }

    printf("  Phase distribution (8 sectors of 45 degrees):\n");
    for (i = 0; i < 8; i++) {
        double angle = (double)i * 45.0;
        printf("    [%5.1f - %5.1f): %d braids\n",
               angle, angle + 45.0, phase_bins[i]);
        if (phase_bins[i] > max_bin) max_bin = phase_bins[i];
        if (phase_bins[i] < min_bin) min_bin = phase_bins[i];
    }

    printf("\n  Max bin: %d, Min bin: %d, Ratio: %.2f\n",
           max_bin, min_bin,
           min_bin > 0 ? (double)max_bin / (double)min_bin : 99.9);

    /* Phase-writhe correlation */
    printf("\n  Phase vs writhe (first 20 braids):\n");
    for (i = 0; i < cx_catalog_size && i < 20; i++) {
        int writhe = 0, j;
        for (j = 0; j < cx_catalog[i].braid.len; j++)
            writhe += cx_catalog[i].braid.word[j] > 0 ? 1 : -1;
        printf("    braid_len=%d writhe=%+d  bracket=(%.3f,%.3f) phase=%.4f\n",
               cx_catalog[i].braid.len, writhe,
               cx_catalog[i].bracket.re, cx_catalog[i].bracket.im,
               cx_catalog[i].phase);
    }

    sprintf(msg, "phases are structured (not uniform, ratio > 1.5 or < 0.67)");
    check(msg, max_bin > 0 && min_bin > 0 &&
               ((double)max_bin / (double)min_bin > 1.5 ||
                (double)max_bin / (double)min_bin < 0.67));
}

/* ================================================================
 * PART E: Degeneracy Test
 *
 * How many braids match each weight when using complex values
 * vs amplitude only?
 * ================================================================ */

static void part_e_degeneracy(const CxNeuron *net) {
    Cx weights[3];
    const char *names[3] = {"w1", "w2", "b "};
    int i;
    double total_amp_deg = 0.0, total_cx_deg = 0.0;
    char msg[256];
    int n_weights = 3;

    printf("\n=== PART E: Degeneracy Test ===\n");

    if (cx_catalog_size == 0) {
        printf("  No catalog available. Skipping.\n");
        return;
    }

    weights[0] = net->w1;
    weights[1] = net->w2;
    weights[2] = net->b;

    for (i = 0; i < n_weights; i++) {
        double w_amp = cx_abs(weights[i]);
        int amp_matches = 0, cx_matches = 0;
        int j;
        double tolerance = 0.5; /* generous matching radius */

        /* Count amplitude matches */
        for (j = 0; j < cx_catalog_size; j++) {
            if (fabs(cx_catalog[j].amplitude - w_amp) < tolerance)
                amp_matches++;
        }

        /* Count complex matches */
        for (j = 0; j < cx_catalog_size; j++) {
            if (cx_abs(cx_sub(cx_catalog[j].bracket, weights[i])) < tolerance)
                cx_matches++;
        }

        printf("  %s  |w|=%.4f:\n", names[i], w_amp);
        printf("    Amplitude matches (tol=%.1f): %d\n", tolerance, amp_matches);
        printf("    Complex matches   (tol=%.1f): %d\n", tolerance, cx_matches);
        printf("    Degeneracy reduction: %.1fx\n",
               cx_matches > 0 ? (double)amp_matches / (double)cx_matches : 99.9);

        total_amp_deg += (double)amp_matches;
        total_cx_deg += (double)cx_matches;
    }

    printf("\n  Average degeneracy:\n");
    printf("    Amplitude: %.1f matches per weight\n", total_amp_deg / (double)n_weights);
    printf("    Complex:   %.1f matches per weight\n", total_cx_deg / (double)n_weights);

    sprintf(msg, "P2: complex matching reduces degeneracy (%.1f vs %.1f)",
            total_cx_deg / (double)n_weights, total_amp_deg / (double)n_weights);
    check(msg, total_cx_deg < total_amp_deg);
}

/* ================================================================
 * PART F: Real vs Complex Comparison
 *
 * Train a real 2-2-1 network (from Demo 26) and a complex neuron
 * on the same XOR task. Compare decomposition quality.
 * ================================================================ */

/* Real XOR network from Demo 26 (2-2-1 architecture) */
typedef struct {
    double wh[2][2]; /* hidden weights */
    double bh[2];    /* hidden biases */
    double wo[2];    /* output weights */
    double bo;       /* output bias */
} RealXorNet;

static void real_net_init(RealXorNet *net) {
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++)
            net->wh[i][j] = rng_normal() * 0.5;
        net->bh[i] = rng_normal() * 0.1;
        net->wo[i] = rng_normal() * 0.5;
    }
    net->bo = rng_normal() * 0.1;
}

static double real_net_forward(const RealXorNet *net,
                                double x1, double x2, double h[2]) {
    int i;
    double out;
    for (i = 0; i < 2; i++) {
        h[i] = sigmoid(net->wh[i][0] * x1 + net->wh[i][1] * x2 + net->bh[i]);
    }
    out = sigmoid(net->wo[0] * h[0] + net->wo[1] * h[1] + net->bo);
    return out;
}

static int real_net_train_xor(RealXorNet *net, double lr, int max_epochs) {
    double inputs[4][2] = {{0,0}, {0,1}, {1,0}, {1,1}};
    double targets[4]   = {0, 1, 1, 0};
    int epoch, i;

    for (epoch = 0; epoch < max_epochs; epoch++) {
        double total_loss = 0.0;
        int correct = 0;

        for (i = 0; i < 4; i++) {
            double x1 = inputs[i][0], x2 = inputs[i][1];
            double h[2], out, err, d_out;
            double d_h[2];
            int j;

            out = real_net_forward(net, x1, x2, h);
            err = out - targets[i];
            total_loss += err * err;
            if ((out > 0.5) == (targets[i] > 0.5)) correct++;

            /* Backprop */
            d_out = err * sigmoid_deriv(out);
            for (j = 0; j < 2; j++) {
                d_h[j] = d_out * net->wo[j] * sigmoid_deriv(h[j]);
                net->wo[j] -= lr * d_out * h[j];
            }
            net->bo -= lr * d_out;
            for (j = 0; j < 2; j++) {
                net->wh[j][0] -= lr * d_h[j] * x1;
                net->wh[j][1] -= lr * d_h[j] * x2;
                net->bh[j]    -= lr * d_h[j];
            }
        }

        if (correct == 4 && total_loss < 0.01)
            return epoch + 1;
    }
    return 0;
}

static void part_f_comparison(const CxNeuron *cx_net, int cx_found) {
    RealXorNet real_net;
    int real_found = 0;
    int trial;
    double real_amp_rms = 1e30;
    double cx_gauge_rms = 1e30;
    char msg[256];

    printf("\n=== PART F: Real vs Complex Comparison ===\n");

    if (cx_catalog_size == 0 || !cx_found) {
        printf("  Missing catalog or complex network. Skipping.\n");
        return;
    }

    /* Train real network */
    for (trial = 0; trial < 20; trial++) {
        RealXorNet rnet;
        rng_seed((unsigned long)(trial * 6271 + 99));
        real_net_init(&rnet);
        if (real_net_train_xor(&rnet, 1.0, 50000)) {
            real_net = rnet;
            real_found = 1;
            break;
        }
    }

    if (!real_found) {
        printf("  Could not train real network. Skipping comparison.\n");
        return;
    }

    /* Real network decomposition (amplitude only) */
    {
        double real_weights[9];
        int n_rw = 0;
        double rms = 0.0;
        int i;

        real_weights[n_rw++] = real_net.wh[0][0];
        real_weights[n_rw++] = real_net.wh[0][1];
        real_weights[n_rw++] = real_net.wh[1][0];
        real_weights[n_rw++] = real_net.wh[1][1];
        real_weights[n_rw++] = real_net.bh[0];
        real_weights[n_rw++] = real_net.bh[1];
        real_weights[n_rw++] = real_net.wo[0];
        real_weights[n_rw++] = real_net.wo[1];
        real_weights[n_rw++] = real_net.bo;

        for (i = 0; i < n_rw; i++) {
            int idx;
            double dist;
            find_nearest_amp(fabs(real_weights[i]), &idx, &dist);
            rms += dist * dist;
        }
        real_amp_rms = sqrt(rms / (double)n_rw);
        printf("  Real 2-2-1 network (9 params):\n");
        printf("    Amplitude RMS: %.6f\n", real_amp_rms);
    }

    /* Complex network decomposition (with gauge) */
    {
        Cx weights[3];
        int gi, i;
        double best_rms = 1e30;
        double best_angle = 0.0;

        weights[0] = cx_net->w1;
        weights[1] = cx_net->w2;
        weights[2] = cx_net->b;

        for (gi = 0; gi < 360; gi++) {
            double theta = 2.0 * M_PI * (double)gi / 360.0;
            Cx rot = cx_exp_i(theta);
            double total = 0.0;

            for (i = 0; i < 3; i++) {
                Cx w_rot = cx_mul(weights[i], rot);
                int idx;
                double dist;
                find_nearest_cx(w_rot, &idx, &dist);
                total += dist * dist;
            }
            total = sqrt(total / 3.0);
            if (total < best_rms) {
                best_rms = total;
                best_angle = theta;
            }
        }
        cx_gauge_rms = best_rms;
        printf("\n  Complex neuron (6 params):\n");
        printf("    Gauge-corrected complex RMS: %.6f\n", cx_gauge_rms);
        printf("    Gauge angle: %.4f rad (%.1f deg)\n",
               best_angle, best_angle * 180.0 / M_PI);
    }

    printf("\n  COMPARISON:\n");
    printf("    Real 2-2-1 (9 params, amp-only):  RMS = %.6f\n", real_amp_rms);
    printf("    Complex neuron (6 params, gauge):  RMS = %.6f\n", cx_gauge_rms);
    printf("    Demo 26 reference:                 RMS = 0.2641\n");
    printf("    Winner: %s\n",
           cx_gauge_rms < real_amp_rms ? "COMPLEX" : "REAL");

    sprintf(msg, "P5: complex (6 params) beats real (9 params): %.4f vs %.4f",
            cx_gauge_rms, real_amp_rms);
    check(msg, cx_gauge_rms < real_amp_rms);

    sprintf(msg, "complex RMS improves on Demo 26 reference (%.4f vs 0.2641)",
            cx_gauge_rms);
    check(msg, cx_gauge_rms < 0.2641);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    CxNeuron best_cx_net;
    int cx_found = 0;

    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 27: Complex-Valued Reverse DKC\n");
    printf("==============================================\n");

    part_a_complex_xor(&best_cx_net, &cx_found);
    part_b_complex_catalog();
    if (cx_found) {
        part_c_complex_decomposition(&best_cx_net);
        part_d_phase_analysis();
        part_e_degeneracy(&best_cx_net);
        part_f_comparison(&best_cx_net, cx_found);
    } else {
        printf("\n  Skipping Parts C-F: no converged complex network.\n");
    }

    printf("\n==============================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("==============================================\n");
    return n_fail > 0 ? 1 : 0;
}
