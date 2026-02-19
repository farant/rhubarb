/*
 * KNOTAPEL DEMO 46: Per-Neuron Gauge Correction
 * ===============================================
 *
 * THESIS: Demo 45 found that amplitude-only RMS often beats gauge-corrected
 * complex RMS (e.g., BUF: amp=0.40 vs cx=0.53). The single global U(1)
 * rotation can't satisfy all 12 complex weights simultaneously because
 * different hidden neurons may have learned at different lattice orientations.
 *
 * Demo 46 replaces the single global gauge angle with per-neuron gauge
 * correction: each hidden neuron k gets its own rotation angle theta_k,
 * rotating only that neuron's 4 weights (3 inputs + 1 bias) independently.
 *
 * For each NPN class:
 *   1. Train identical CxNet3 as Demo 45
 *   2. Build same bracket catalog
 *   3. Run global gauge sweep (1D, 360 angles) -- Demo 45 baseline
 *   4. Run per-neuron gauge sweep (3D coarse 36^3 + refinement)
 *   5. Compare: global-gauge RMS, per-neuron-gauge RMS, amplitude-only RMS
 *
 * Predictions:
 *   P1: Per-neuron RMS < global RMS for all 13 classes (trivially true)
 *   P2: Largest improvement for mid-ranking functions (ranks 4-9)
 *   P3: Per-neuron RMS < amplitude-only RMS for at least top 6 functions
 *   P4: Spearman rho(freq, pn_RMS) more negative than -0.8167
 *   P5: Optimal per-neuron angles show structure (clustering/symmetry)
 *
 * Part A: NPN Classification & Topology Frequency
 * Part B: Complex Network Training
 * Part C: Complex Bracket Catalog
 * Part D: Decomposition -- Global Gauge + Per-Neuron Gauge
 * Part E: Ranking & Correlation Comparison
 * Part F: Summary Table & Angle Structure Analysis
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
 * Complex arithmetic (from Demos 23/27/45)
 * ================================================================ */

typedef struct { double re, im; } Cx;

static Cx cx_make(double re, double im) {
    Cx z; z.re = re; z.im = im; return z;
}
static Cx cx_zero(void) { return cx_make(0.0, 0.0); }
static Cx cx_one(void)  { return cx_make(1.0, 0.0); }

static Cx cx_add(Cx a, Cx b) {
    return cx_make(a.re + b.re, a.im + b.im);
}
static Cx cx_sub(Cx a, Cx b) {
    return cx_make(a.re - b.re, a.im - b.im);
}
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
static double cx_abs(Cx a) {
    return sqrt(a.re * a.re + a.im * a.im);
}
static double cx_phase(Cx a) { return atan2(a.im, a.re); }
static Cx cx_exp_i(double theta) {
    return cx_make(cos(theta), sin(theta));
}
static Cx cx_scale(Cx a, double s) {
    return cx_make(a.re * s, a.im * s);
}
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
 * Braid types and closure (from Demo 23)
 * ================================================================ */

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

typedef enum {
    CL_TRACE,
    CL_PLAT_A,
    CL_PLAT_B,
    CL_PLAT_C
} Closure;

/* ================================================================
 * Union-find
 * ================================================================ */

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

/* ================================================================
 * Bracket with closure support (from Demo 23)
 * ================================================================ */

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
static Cx cx_sigmoid(Cx z) {
    return cx_make(sigmoid(z.re), sigmoid(z.im));
}

/* ================================================================
 * NPN equivalence for 3-input Boolean functions (from Demo 23)
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

/* ================================================================
 * Function name lookup
 * ================================================================ */

static const char *fn_name(int tt) {
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
    case 0x80: return "AND3";
    case 0x88: return "B&C";
    case 0x96: return "XOR3";
    case 0xC0: return "A&B";
    case 0xCA: return "MUX";
    case 0xE8: return "MAJ";
    case 0xFE: return "OR3";
    default: return NULL;
    }
}

/* ================================================================
 * Truth table from braid + closure (from Demo 23)
 * ================================================================ */

static int gen_map[6] = {1, -1, 2, -2, 3, -3};

static void decode_word(int code, int len, int *word) {
    int i;
    for (i = 0; i < len; i++) {
        word[i] = gen_map[code % 6];
        code /= 6;
    }
}

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
 * Complex Bracket Catalog (from Demo 27)
 * ================================================================ */

typedef struct {
    Braid braid;
    Cx bracket;
    double amplitude;
    double phase;
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
                Cx bracket;
                double amp;

                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen)
                        word[i] = g + 1;
                    else
                        word[i] = -(g - max_gen + 1);
                }

                b.n = n;
                b.len = len;
                memcpy(b.word, word, (size_t)len * sizeof(int));

                bracket = bracket_cl(&b, A, CL_TRACE);
                amp = cx_abs(bracket);

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

/* ================================================================
 * NPN Class data structures (extended for per-neuron gauge)
 * ================================================================ */

#define MAX_NPN_CLASSES 80
#define NUM_CX_WEIGHTS 12   /* 3 hidden x 3 inputs + 3 biases */

typedef struct {
    int canon_tt;
    int orbit_size;
    int topo_freq;
    const char *titulis;

    /* Training */
    int converged;
    int best_epoch;

    /* Complex weights from best network (flattened):
     * weights[0..2]  = neuron 0 inputs (wh[0][0..2])
     * weights[3..5]  = neuron 1 inputs (wh[1][0..2])
     * weights[6..8]  = neuron 2 inputs (wh[2][0..2])
     * weights[9]     = neuron 0 bias   (bh[0])
     * weights[10]    = neuron 1 bias   (bh[1])
     * weights[11]    = neuron 2 bias   (bh[2])
     */
    Cx weights[NUM_CX_WEIGHTS];
    int n_weights;

    /* Global gauge decomposition (Demo 45 baseline) */
    double gauge_rms;
    double best_gauge_angle;
    double amp_only_rms;
    double avg_braid_len;
    int n_distinct_phases;

    /* Per-neuron gauge decomposition (NEW in Demo 46) */
    double pn_gauge_rms;
    double pn_best_angles[3];
    double pn_improvement;     /* gauge_rms - pn_gauge_rms */
} NPNClass;

static NPNClass npn_classes[MAX_NPN_CLASSES];
static int n_npn_classes = 0;

/* ================================================================
 * Complex-valued 3-input neural network
 * (identical to Demo 45)
 * ================================================================ */

#define NUM_HIDDEN 3
#define NUM_TRIALS 10
#define MAX_EPOCHS 50000

typedef struct {
    Cx wh[NUM_HIDDEN][3];
    Cx bh[NUM_HIDDEN];
    double vo[NUM_HIDDEN * 2];
    double bo;
} CxNet3;

static void cxnet3_init(CxNet3 *net) {
    int k, j;
    for (k = 0; k < NUM_HIDDEN; k++) {
        for (j = 0; j < 3; j++)
            net->wh[k][j] = cx_make(rng_normal(), rng_normal());
        net->bh[k] = cx_make(rng_normal() * 0.5, rng_normal() * 0.5);
        net->vo[2 * k] = rng_normal() * 0.5;
        net->vo[2 * k + 1] = rng_normal() * 0.5;
    }
    net->bo = rng_normal() * 0.1;
}

static double cxnet3_forward(const CxNet3 *net, const double x[3],
                             Cx h[NUM_HIDDEN]) {
    int k, j;
    double out_pre = net->bo;
    for (k = 0; k < NUM_HIDDEN; k++) {
        Cx z = net->bh[k];
        for (j = 0; j < 3; j++)
            z = cx_add(z, cx_scale(net->wh[k][j], x[j]));
        h[k] = cx_sigmoid(z);
        out_pre += net->vo[2 * k] * h[k].re
                 + net->vo[2 * k + 1] * h[k].im;
    }
    return sigmoid(out_pre);
}

static int cxnet3_train(CxNet3 *net, int tt, double lr, int max_ep) {
    double inputs[8][3];
    double targets[8];
    int epoch, i, k, j;

    for (i = 0; i < 8; i++) {
        inputs[i][0] = (double)((i >> 2) & 1);
        inputs[i][1] = (double)((i >> 1) & 1);
        inputs[i][2] = (double)(i & 1);
        targets[i] = (double)((tt >> i) & 1);
    }

    for (epoch = 0; epoch < max_ep; epoch++) {
        double total_loss = 0.0;
        int correct = 0;

        for (i = 0; i < 8; i++) {
            Cx h[NUM_HIDDEN];
            double out, err, delta_out;
            double dz_re[NUM_HIDDEN], dz_im[NUM_HIDDEN];

            out = cxnet3_forward(net, inputs[i], h);
            err = out - targets[i];
            total_loss += err * err;
            if ((out > 0.5) == (targets[i] > 0.5)) correct++;

            delta_out = 2.0 * err * sigmoid_deriv(out);

            for (k = 0; k < NUM_HIDDEN; k++) {
                double dL_dhr = delta_out * net->vo[2 * k];
                double dL_dhi = delta_out * net->vo[2 * k + 1];
                dz_re[k] = dL_dhr * sigmoid_deriv(h[k].re);
                dz_im[k] = dL_dhi * sigmoid_deriv(h[k].im);

                net->vo[2 * k]     -= lr * delta_out * h[k].re;
                net->vo[2 * k + 1] -= lr * delta_out * h[k].im;

                for (j = 0; j < 3; j++) {
                    net->wh[k][j].re -= lr * dz_re[k] * inputs[i][j];
                    net->wh[k][j].im -= lr * dz_im[k] * inputs[i][j];
                }
                net->bh[k].re -= lr * dz_re[k];
                net->bh[k].im -= lr * dz_im[k];
            }
            net->bo -= lr * delta_out;
        }

        if (correct == 8 && total_loss < 0.01)
            return epoch + 1;
    }
    return 0;
}

/* ================================================================
 * Spearman rank correlation utilities
 * ================================================================ */

static void assign_ranks(const double *values, double *ranks, int n) {
    int order[MAX_NPN_CLASSES];
    int i, j;

    for (i = 0; i < n; i++) order[i] = i;

    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (values[order[j]] < values[order[i]]) {
                int tmp = order[i];
                order[i] = order[j];
                order[j] = tmp;
            }

    i = 0;
    while (i < n) {
        int tie_start = i;
        double sum_rank = 0.0;
        while (i < n &&
               fabs(values[order[i]] - values[order[tie_start]]) < 1e-10) {
            sum_rank += (double)(i + 1);
            i++;
        }
        {
            double avg_rank = sum_rank / (double)(i - tie_start);
            int kk;
            for (kk = tie_start; kk < i; kk++)
                ranks[order[kk]] = avg_rank;
        }
    }
}

static double spearman_rho(const double *x, const double *y, int n) {
    double rx[MAX_NPN_CLASSES], ry[MAX_NPN_CLASSES];
    double d2 = 0.0;
    int i;

    if (n < 3) return 0.0;

    assign_ranks(x, rx, n);
    assign_ranks(y, ry, n);

    for (i = 0; i < n; i++) {
        double d = rx[i] - ry[i];
        d2 += d * d;
    }
    return 1.0 - 6.0 * d2 /
           ((double)n * ((double)n * (double)n - 1.0));
}

/* ================================================================
 * Per-neuron gauge: compute RMS for given 3 angles
 *
 * Weight layout in npn_classes[i].weights[]:
 *   Neuron k inputs: weights[k*3 + 0..2]
 *   Neuron k bias:   weights[9 + k]
 * ================================================================ */

static double pn_gauge_rms_for_angles(const Cx *weights, int n_weights,
                                      double t0, double t1, double t2) {
    Cx rot[3];
    double total = 0.0;
    int wi, k;

    (void)n_weights; /* always 12 */

    rot[0] = cx_exp_i(t0);
    rot[1] = cx_exp_i(t1);
    rot[2] = cx_exp_i(t2);

    for (k = 0; k < 3; k++) {
        /* Neuron k's 3 input weights */
        for (wi = 0; wi < 3; wi++) {
            Cx w_rot = cx_mul(weights[k * 3 + wi], rot[k]);
            int idx;
            double dist;
            find_nearest_cx(w_rot, &idx, &dist);
            total += dist * dist;
        }
        /* Neuron k's bias */
        {
            Cx w_rot = cx_mul(weights[9 + k], rot[k]);
            int idx;
            double dist;
            find_nearest_cx(w_rot, &idx, &dist);
            total += dist * dist;
        }
    }

    return sqrt(total / 12.0);
}

/* ================================================================
 * PART A: NPN Classification & Topology Frequency
 * ================================================================ */

static void part_a_classify(void) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int found[256];
    int i, len, code, ci;
    char msg[256];

    printf("\n=== PART A: NPN Classification & Topology Frequency ===\n\n");

    {
        int seen[256];
        memset(seen, 0, sizeof(seen));
        n_npn_classes = 0;

        for (i = 1; i < 255; i++) {
            int c = npn_canon[i];
            if (!seen[c]) {
                seen[c] = 1;
                memset(&npn_classes[n_npn_classes], 0,
                       sizeof(NPNClass));
                npn_classes[n_npn_classes].canon_tt = c;
                npn_classes[n_npn_classes].titulis = fn_name(c);
                npn_classes[n_npn_classes].gauge_rms = 1e30;
                npn_classes[n_npn_classes].pn_gauge_rms = 1e30;
                n_npn_classes++;
            }
        }

        for (i = 1; i < 255; i++) {
            int c = npn_canon[i];
            int j;
            for (j = 0; j < n_npn_classes; j++) {
                if (npn_classes[j].canon_tt == c) {
                    npn_classes[j].orbit_size++;
                    break;
                }
            }
        }
    }

    printf("  Non-trivial NPN classes: %d\n", n_npn_classes);

    printf("  Computing topology frequency (braids len 1-3)...\n");
    memset(found, 0, sizeof(found));

    for (len = 1; len <= 3; len++) {
        int n_words = 1, wi;
        for (wi = 0; wi < len; wi++) n_words *= 6;

        for (code = 0; code < n_words; code++) {
            int word[MAX_WORD];
            decode_word(code, len, word);

            for (ci = 0; ci < 4; ci++) {
                int tt = compute_tt(word, len, (Closure)ci, A);
                if (tt > 0 && tt < 255) found[tt]++;
            }
        }
    }

    for (i = 1; i < 255; i++) {
        if (found[i] > 0) {
            int c = npn_canon[i];
            int j;
            for (j = 0; j < n_npn_classes; j++) {
                if (npn_classes[j].canon_tt == c) {
                    npn_classes[j].topo_freq += found[i];
                    break;
                }
            }
        }
    }

    printf("\n  %-6s %-6s %-8s %s\n",
           "Canon", "Orbit", "TopoFreq", "Name");
    printf("  %-6s %-6s %-8s %s\n",
           "------", "-----", "--------", "----");
    for (i = 0; i < n_npn_classes; i++) {
        const char *nm = npn_classes[i].titulis;
        printf("  0x%02x  %-6d %-8d %s\n",
               npn_classes[i].canon_tt,
               npn_classes[i].orbit_size,
               npn_classes[i].topo_freq,
               nm ? nm : "?");
    }

    {
        int n_reached = 0;
        for (i = 0; i < n_npn_classes; i++)
            if (npn_classes[i].topo_freq > 0) n_reached++;
        sprintf(msg, "topology reaches %d / %d NPN classes",
                n_reached, n_npn_classes);
        check(msg, n_reached > 0);
    }
}

/* ================================================================
 * PART B: Complex Network Training
 * ================================================================ */

static void part_b_train_all(void) {
    int i, trial;
    int total_converged = 0;
    char msg[256];

    printf("\n=== PART B: Complex Network Training ===\n");
    printf("  Architecture: 3 inputs -> %d Cx hidden (split-sigmoid)"
           " -> 1 real output\n", NUM_HIDDEN);
    printf("  31 real parameters, %d trials/class, "
           "max %d epochs\n\n", NUM_TRIALS, MAX_EPOCHS);

    for (i = 0; i < n_npn_classes; i++) {
        int tt = npn_classes[i].canon_tt;
        int best_epoch = 0;
        CxNet3 best_net;
        int k, j;

        memset(&best_net, 0, sizeof(best_net));

        for (trial = 0; trial < NUM_TRIALS; trial++) {
            CxNet3 net;
            int epoch;

            rng_seed((unsigned long)(
                (i * NUM_TRIALS + trial) * 7919 + 42));
            cxnet3_init(&net);
            epoch = cxnet3_train(&net, tt, 0.5, MAX_EPOCHS);

            if (epoch > 0 &&
                (best_epoch == 0 || epoch < best_epoch)) {
                best_net = net;
                best_epoch = epoch;
            }
        }

        if (best_epoch > 0) {
            npn_classes[i].converged = 1;
            npn_classes[i].best_epoch = best_epoch;
            total_converged++;

            npn_classes[i].n_weights = 0;
            for (k = 0; k < NUM_HIDDEN; k++)
                for (j = 0; j < 3; j++)
                    npn_classes[i].weights[
                        npn_classes[i].n_weights++] = best_net.wh[k][j];
            for (k = 0; k < NUM_HIDDEN; k++)
                npn_classes[i].weights[
                    npn_classes[i].n_weights++] = best_net.bh[k];

            printf("  0x%02x %-10s converged epoch %5d\n",
                   tt,
                   npn_classes[i].titulis ?
                       npn_classes[i].titulis : "?",
                   best_epoch);
        } else {
            printf("  0x%02x %-10s DID NOT CONVERGE\n",
                   tt,
                   npn_classes[i].titulis ?
                       npn_classes[i].titulis : "?");
        }
    }

    sprintf(msg, "%d / %d NPN classes converged",
            total_converged, n_npn_classes);
    check(msg, total_converged > n_npn_classes / 2);
}

/* ================================================================
 * PART C: Complex Bracket Catalog
 * ================================================================ */

static void part_c_catalog(void) {
    Cx A = cx_exp_i(5.0 * M_PI / 4.0);
    int n_distinct = 0;
    Cx seen[512];
    int n_seen = 0;
    int i;
    char msg[256];

    printf("\n=== PART C: Complex Bracket Catalog ===\n");
    printf("  A = e^{i*5pi/4}, delta = 0\n");
    printf("  Enumerating 2-3 strand braids up to length 8...\n\n");

    build_complex_catalog(A, 3, 8);

    printf("  Catalog size: %d braids\n", cx_catalog_size);

    for (i = 0; i < cx_catalog_size; i++) {
        Cx val = cx_catalog[i].bracket;
        int j, found_it = 0;
        for (j = 0; j < n_seen; j++) {
            if (cx_abs(cx_sub(val, seen[j])) < 0.01) {
                found_it = 1; break;
            }
        }
        if (!found_it && n_seen < 512) {
            seen[n_seen++] = val;
        }
    }
    n_distinct = n_seen;

    printf("  Distinct complex bracket values: %d\n", n_distinct);

    {
        double max_min_dist = 0.0;
        int trial;
        rng_seed(54321);
        for (trial = 0; trial < 1000; trial++) {
            double mag = rng_uniform() * 5.0 + 0.5;
            double angle = rng_uniform() * 2.0 * M_PI;
            Cx pt = cx_scale(cx_exp_i(angle), mag);
            int idx;
            double dist;
            find_nearest_cx(pt, &idx, &dist);
            if (dist > max_min_dist) max_min_dist = dist;
        }
        printf("  Covering radius (est): %.4f\n", max_min_dist);
    }

    sprintf(msg, "catalog has >= 50 distinct bracket values (%d)",
            n_distinct);
    check(msg, n_distinct >= 50);
}

/* ================================================================
 * PART D: Decomposition -- Global Gauge + Per-Neuron Gauge
 * ================================================================ */

static double g_random_rms = 0.0;

static void part_d_decompose(void) {
    int i, gi, wi;
    double random_rms_sum = 0.0;
    int n_random = 1000;
    char msg[256];

    printf("\n=== PART D: Decomposition (Global + Per-Neuron Gauge) ===\n\n");

    if (cx_catalog_size == 0) {
        printf("  No catalog. Skipping.\n");
        return;
    }

    /* Random baseline */
    rng_seed(99999);
    for (i = 0; i < n_random; i++) {
        double mag = fabs(rng_normal()) * 2.0 + 0.5;
        double angle = rng_uniform() * 2.0 * M_PI;
        Cx pt = cx_scale(cx_exp_i(angle), mag);
        int idx;
        double dist;
        find_nearest_cx(pt, &idx, &dist);
        random_rms_sum += dist * dist;
    }
    g_random_rms = sqrt(random_rms_sum / (double)n_random);
    printf("  Random baseline RMS: %.4f\n\n", g_random_rms);

    /* Decompose each converged class */
    for (i = 0; i < n_npn_classes; i++) {
        double best_rms = 1e30;
        double best_angle = 0.0;
        double amp_rms_sum = 0.0;

        if (!npn_classes[i].converged) continue;

        /* --- Amplitude-only RMS (same as Demo 45) --- */
        for (wi = 0; wi < npn_classes[i].n_weights; wi++) {
            double w_amp = cx_abs(npn_classes[i].weights[wi]);
            double min_d = 1e30;
            int ci;
            for (ci = 0; ci < cx_catalog_size; ci++) {
                double d = fabs(w_amp - cx_catalog[ci].amplitude);
                if (d < min_d) min_d = d;
            }
            amp_rms_sum += min_d * min_d;
        }
        npn_classes[i].amp_only_rms = sqrt(
            amp_rms_sum / (double)npn_classes[i].n_weights);

        /* --- Global gauge sweep: 360 angles (Demo 45 baseline) --- */
        for (gi = 0; gi < 360; gi++) {
            double theta = 2.0 * M_PI * (double)gi / 360.0;
            Cx rot = cx_exp_i(theta);
            double total = 0.0;

            for (wi = 0; wi < npn_classes[i].n_weights; wi++) {
                Cx w_rot = cx_mul(npn_classes[i].weights[wi], rot);
                int idx;
                double dist;
                find_nearest_cx(w_rot, &idx, &dist);
                total += dist * dist;
            }
            total = sqrt(total / (double)npn_classes[i].n_weights);

            if (total < best_rms) {
                best_rms = total;
                best_angle = theta;
            }
        }

        npn_classes[i].gauge_rms = best_rms;
        npn_classes[i].best_gauge_angle = best_angle;

        /* Stats at best angle */
        {
            Cx rot = cx_exp_i(best_angle);
            double braid_sum = 0.0;
            double seen_ph[12];
            int n_sp = 0;

            for (wi = 0; wi < npn_classes[i].n_weights; wi++) {
                Cx w_rot = cx_mul(npn_classes[i].weights[wi], rot);
                int idx;
                double dist, ph;
                int si, ph_found;

                find_nearest_cx(w_rot, &idx, &dist);
                braid_sum += (double)cx_catalog[idx].braid.len;

                ph = cx_catalog[idx].phase;
                ph_found = 0;
                for (si = 0; si < n_sp; si++) {
                    if (fabs(ph - seen_ph[si]) < 0.1) {
                        ph_found = 1; break;
                    }
                }
                if (!ph_found && n_sp < 12)
                    seen_ph[n_sp++] = ph;
            }

            npn_classes[i].avg_braid_len =
                braid_sum / (double)npn_classes[i].n_weights;
            npn_classes[i].n_distinct_phases = n_sp;
        }

        /* --- Per-neuron gauge: coarse 3D grid (10-degree resolution) --- */
        {
            double pn_best = 1e30;
            double pn_angles[3] = {0.0, 0.0, 0.0};
            int g0, g1, g2;
            double step = 2.0 * M_PI / 36.0; /* 10 degrees */

            /* Coarse sweep: 36^3 = 46656 combinations */
            for (g0 = 0; g0 < 36; g0++) {
                double t0 = (double)g0 * step;
                for (g1 = 0; g1 < 36; g1++) {
                    double t1 = (double)g1 * step;
                    for (g2 = 0; g2 < 36; g2++) {
                        double t2 = (double)g2 * step;
                        double rms = pn_gauge_rms_for_angles(
                            npn_classes[i].weights,
                            npn_classes[i].n_weights,
                            t0, t1, t2);
                        if (rms < pn_best) {
                            pn_best = rms;
                            pn_angles[0] = t0;
                            pn_angles[1] = t1;
                            pn_angles[2] = t2;
                        }
                    }
                }
            }

            /* Refinement: 1-degree grid around best coarse angles */
            {
                double fine_step = 2.0 * M_PI / 360.0; /* 1 degree */
                int f0, f1, f2;
                for (f0 = -5; f0 <= 5; f0++) {
                    double t0 = pn_angles[0] + (double)f0 * fine_step;
                    for (f1 = -5; f1 <= 5; f1++) {
                        double t1 = pn_angles[1] + (double)f1 * fine_step;
                        for (f2 = -5; f2 <= 5; f2++) {
                            double t2 = pn_angles[2]
                                        + (double)f2 * fine_step;
                            double rms = pn_gauge_rms_for_angles(
                                npn_classes[i].weights,
                                npn_classes[i].n_weights,
                                t0, t1, t2);
                            if (rms < pn_best) {
                                pn_best = rms;
                                pn_angles[0] = t0;
                                pn_angles[1] = t1;
                                pn_angles[2] = t2;
                            }
                        }
                    }
                }
            }

            npn_classes[i].pn_gauge_rms = pn_best;
            npn_classes[i].pn_best_angles[0] = pn_angles[0];
            npn_classes[i].pn_best_angles[1] = pn_angles[1];
            npn_classes[i].pn_best_angles[2] = pn_angles[2];
            npn_classes[i].pn_improvement = best_rms - pn_best;
        }

        printf("  0x%02x %-10s global=%.4f pn=%.4f amp=%.4f "
               "improv=%.4f angles=(%.0f,%.0f,%.0f)\n",
               npn_classes[i].canon_tt,
               npn_classes[i].titulis ?
                   npn_classes[i].titulis : "?",
               best_rms, npn_classes[i].pn_gauge_rms,
               npn_classes[i].amp_only_rms,
               npn_classes[i].pn_improvement,
               npn_classes[i].pn_best_angles[0] * 180.0 / M_PI,
               npn_classes[i].pn_best_angles[1] * 180.0 / M_PI,
               npn_classes[i].pn_best_angles[2] * 180.0 / M_PI);
    }

    /* Count beating random */
    {
        int n_beat_g = 0, n_beat_pn = 0, n_conv = 0;
        for (i = 0; i < n_npn_classes; i++) {
            if (npn_classes[i].converged) {
                n_conv++;
                if (npn_classes[i].gauge_rms < g_random_rms)
                    n_beat_g++;
                if (npn_classes[i].pn_gauge_rms < g_random_rms)
                    n_beat_pn++;
            }
        }
        sprintf(msg, "global: %d / %d beat random (%.4f)",
                n_beat_g, n_conv, g_random_rms);
        check(msg, n_beat_g > n_conv / 2);
        sprintf(msg, "per-neuron: %d / %d beat random (%.4f)",
                n_beat_pn, n_conv, g_random_rms);
        check(msg, n_beat_pn >= n_beat_g);
    }
}

/* ================================================================
 * PART E: Ranking & Correlation Comparison
 * ================================================================ */

static void part_e_correlate(void) {
    int conv_idx[MAX_NPN_CLASSES];
    int n_conv = 0;
    int i;
    double rms_arr[MAX_NPN_CLASSES];
    double pn_rms_arr[MAX_NPN_CLASSES];
    double freq_arr[MAX_NPN_CLASSES];
    double rho_global, rho_pn;
    char msg[256];

    int sorted[MAX_NPN_CLASSES];

    printf("\n=== PART E: Ranking & Correlation Comparison ===\n\n");

    for (i = 0; i < n_npn_classes; i++) {
        if (npn_classes[i].converged && npn_classes[i].topo_freq > 0) {
            rms_arr[n_conv] = npn_classes[i].gauge_rms;
            pn_rms_arr[n_conv] = npn_classes[i].pn_gauge_rms;
            freq_arr[n_conv] = (double)npn_classes[i].topo_freq;
            conv_idx[n_conv] = i;
            n_conv++;
        }
    }

    if (n_conv < 3) {
        printf("  Too few classes (%d). Skipping.\n", n_conv);
        return;
    }

    printf("  Classes analyzed: %d (converged + topology hits)\n\n",
           n_conv);

    /* Spearman correlations: global vs per-neuron */
    rho_global = spearman_rho(rms_arr, freq_arr, n_conv);
    rho_pn = spearman_rho(pn_rms_arr, freq_arr, n_conv);

    /* Sort by per-neuron RMS ascending for display */
    for (i = 0; i < n_conv; i++) sorted[i] = i;
    for (i = 0; i < n_conv - 1; i++) {
        int j;
        for (j = i + 1; j < n_conv; j++) {
            if (pn_rms_arr[sorted[j]] < pn_rms_arr[sorted[i]]) {
                int tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }

    printf("  Ranked by per-neuron gauge RMS (ascending = best):\n");
    printf("  %-4s %-6s %-10s %-8s %-8s %-8s %-8s\n",
           "Rank", "Canon", "Name", "PnRMS", "GlRMS", "AmpRMS",
           "TopoFreq");
    printf("  %-4s %-6s %-10s %-8s %-8s %-8s %-8s\n",
           "----", "------", "----", "------", "------", "------",
           "--------");
    for (i = 0; i < n_conv; i++) {
        int si = sorted[i];
        int ci = conv_idx[si];
        printf("  %3d  0x%02x  %-10s %.4f   %.4f   %.4f   %-8d\n",
               i + 1,
               npn_classes[ci].canon_tt,
               npn_classes[ci].titulis ?
                   npn_classes[ci].titulis : "?",
               npn_classes[ci].pn_gauge_rms,
               npn_classes[ci].gauge_rms,
               npn_classes[ci].amp_only_rms,
               npn_classes[ci].topo_freq);
    }

    /* P4: Compare Spearman rho */
    printf("\n  Spearman rho(global_RMS, freq) = %.4f\n", rho_global);
    printf("  Spearman rho(pn_RMS, freq)     = %.4f\n", rho_pn);
    sprintf(msg, "P4: pn rho (%.4f) more negative than global (%.4f)",
            rho_pn, rho_global);
    check(msg, rho_pn < rho_global);

    /* P1: Per-neuron RMS <= global RMS for all classes */
    {
        int all_better = 1;
        for (i = 0; i < n_conv; i++) {
            int ci = conv_idx[i];
            if (npn_classes[ci].pn_gauge_rms >
                npn_classes[ci].gauge_rms + 0.001) {
                all_better = 0;
                break;
            }
        }
        sprintf(msg, "P1: pn_RMS <= global_RMS for all %d classes",
                n_conv);
        check(msg, all_better);
    }

    /* P2: Largest improvement for mid-ranking (ranks 4-9 by global) */
    {
        int g_sorted[MAX_NPN_CLASSES];
        double top_improv = 0.0, mid_improv = 0.0, bot_improv = 0.0;
        int n_top = 0, n_mid = 0, n_bot = 0;

        for (i = 0; i < n_conv; i++) g_sorted[i] = i;
        for (i = 0; i < n_conv - 1; i++) {
            int j;
            for (j = i + 1; j < n_conv; j++) {
                if (rms_arr[g_sorted[j]] < rms_arr[g_sorted[i]]) {
                    int tmp = g_sorted[i];
                    g_sorted[i] = g_sorted[j];
                    g_sorted[j] = tmp;
                }
            }
        }

        for (i = 0; i < n_conv; i++) {
            int ci = conv_idx[g_sorted[i]];
            double imp = npn_classes[ci].pn_improvement;
            if (i < 3) {
                top_improv += imp; n_top++;
            } else if (i >= n_conv - 3) {
                bot_improv += imp; n_bot++;
            } else {
                mid_improv += imp; n_mid++;
            }
        }
        if (n_top > 0) top_improv /= (double)n_top;
        if (n_mid > 0) mid_improv /= (double)n_mid;
        if (n_bot > 0) bot_improv /= (double)n_bot;

        printf("\n  P2 - Improvement by tier (global rank):\n");
        printf("    Top 3:     avg improvement = %.4f\n", top_improv);
        printf("    Middle:    avg improvement = %.4f (n=%d)\n",
               mid_improv, n_mid);
        printf("    Bottom 3:  avg improvement = %.4f\n", bot_improv);
        sprintf(msg,
                "P2: mid improvement (%.4f) > top (%.4f) and bot (%.4f)",
                mid_improv, top_improv, bot_improv);
        check(msg, mid_improv > top_improv && mid_improv > bot_improv);
    }

    /* P3: Per-neuron RMS < amplitude-only RMS for top 6 */
    {
        int n_pn_beats_amp = 0;
        for (i = 0; i < n_conv && i < 6; i++) {
            int ci = conv_idx[sorted[i]];
            if (npn_classes[ci].pn_gauge_rms <
                npn_classes[ci].amp_only_rms)
                n_pn_beats_amp++;
        }
        printf("\n  P3 - Per-neuron beats amplitude for top 6: "
               "%d / %d\n",
               n_pn_beats_amp, n_conv < 6 ? n_conv : 6);
        sprintf(msg, "P3: pn < amp for %d / %d top functions",
                n_pn_beats_amp, n_conv < 6 ? n_conv : 6);
        check(msg, n_pn_beats_amp >= 6);
    }
}

/* ================================================================
 * PART F: Summary Table & Angle Structure Analysis
 * ================================================================ */

static void part_f_summary(void) {
    int order[MAX_NPN_CLASSES];
    int n_conv = 0;
    int i;

    printf("\n=== PART F: Summary Table & Angle Structure ===\n\n");

    for (i = 0; i < n_npn_classes; i++) {
        if (npn_classes[i].converged) {
            order[n_conv] = i;
            n_conv++;
        }
    }

    /* Sort by per-neuron gauge RMS ascending */
    for (i = 0; i < n_conv - 1; i++) {
        int j;
        for (j = i + 1; j < n_conv; j++) {
            if (npn_classes[order[j]].pn_gauge_rms <
                npn_classes[order[i]].pn_gauge_rms) {
                int tmp = order[i];
                order[i] = order[j];
                order[j] = tmp;
            }
        }
    }

    /* Master table */
    printf("  %-4s %-6s %-10s %-7s %-7s %-7s %-6s %-6s"
           "  angles(deg)\n",
           "Rank", "Canon", "Name", "PnRMS", "GlRMS", "AmpRMS",
           "Freq", "Impr");
    printf("  %-4s %-6s %-10s %-7s %-7s %-7s %-6s %-6s"
           "  -----------\n",
           "----", "------", "----", "-------", "-------", "-------",
           "------", "------");

    for (i = 0; i < n_conv; i++) {
        int ci = order[i];
        printf("  %3d  0x%02x  %-10s %.4f  %.4f  %.4f  %-6d %.4f"
               "  (%.0f, %.0f, %.0f)\n",
               i + 1,
               npn_classes[ci].canon_tt,
               npn_classes[ci].titulis ?
                   npn_classes[ci].titulis : "?",
               npn_classes[ci].pn_gauge_rms,
               npn_classes[ci].gauge_rms,
               npn_classes[ci].amp_only_rms,
               npn_classes[ci].topo_freq,
               npn_classes[ci].pn_improvement,
               npn_classes[ci].pn_best_angles[0] * 180.0 / M_PI,
               npn_classes[ci].pn_best_angles[1] * 180.0 / M_PI,
               npn_classes[ci].pn_best_angles[2] * 180.0 / M_PI);
    }

    /* P5: Angle structure analysis */
    printf("\n  Per-Neuron Angle Structure:\n");
    {
        double pi4 = M_PI / 4.0;
        int n_clustered = 0;    /* 2+ neurons at similar angles */
        int n_pi4_aligned = 0;  /* angles near pi/4 multiples */
        int total_angles = 0;

        for (i = 0; i < n_conv; i++) {
            int ci = order[i];
            double a0 = npn_classes[ci].pn_best_angles[0];
            double a1 = npn_classes[ci].pn_best_angles[1];
            double a2 = npn_classes[ci].pn_best_angles[2];
            double d01, d02, d12;
            int k;

            /* Check if any pair of angles is within 20 degrees */
            d01 = fabs(a0 - a1);
            if (d01 > M_PI) d01 = 2.0 * M_PI - d01;
            d02 = fabs(a0 - a2);
            if (d02 > M_PI) d02 = 2.0 * M_PI - d02;
            d12 = fabs(a1 - a2);
            if (d12 > M_PI) d12 = 2.0 * M_PI - d12;

            if (d01 < 20.0 * M_PI / 180.0 ||
                d02 < 20.0 * M_PI / 180.0 ||
                d12 < 20.0 * M_PI / 180.0)
                n_clustered++;

            /* Check each angle for pi/4 alignment */
            for (k = 0; k < 3; k++) {
                double ang = npn_classes[ci].pn_best_angles[k];
                double nearest = floor(ang / pi4 + 0.5) * pi4;
                double dev = fabs(ang - nearest);
                if (dev < pi4 * 0.15) n_pi4_aligned++;
                total_angles++;
            }

            printf("    0x%02x %-10s (%.0f, %.0f, %.0f) "
                   "spread=(%.0f,%.0f,%.0f)\n",
                   npn_classes[ci].canon_tt,
                   npn_classes[ci].titulis ?
                       npn_classes[ci].titulis : "?",
                   a0 * 180.0 / M_PI,
                   a1 * 180.0 / M_PI,
                   a2 * 180.0 / M_PI,
                   d01 * 180.0 / M_PI,
                   d02 * 180.0 / M_PI,
                   d12 * 180.0 / M_PI);
        }

        printf("\n  P5 - Angle clustering: %d / %d classes have "
               "2+ neurons within 20deg\n", n_clustered, n_conv);
        printf("       Pi/4 alignment: %d / %d angles near "
               "multiples of pi/4\n", n_pi4_aligned, total_angles);
        {
            char p5msg[256];
            sprintf(p5msg, "P5: %d/%d classes show angle clustering",
                    n_clustered, n_conv);
            check(p5msg, n_clustered > n_conv / 3);
        }
    }

    /* Overall comparison */
    {
        double avg_global = 0.0, avg_pn = 0.0, avg_amp = 0.0;
        for (i = 0; i < n_conv; i++) {
            int ci = order[i];
            avg_global += npn_classes[ci].gauge_rms;
            avg_pn += npn_classes[ci].pn_gauge_rms;
            avg_amp += npn_classes[ci].amp_only_rms;
        }
        if (n_conv > 0) {
            avg_global /= (double)n_conv;
            avg_pn /= (double)n_conv;
            avg_amp /= (double)n_conv;
        }
        printf("\n  Overall: %d converged\n", n_conv);
        printf("    avg global gauge RMS:     %.4f\n", avg_global);
        printf("    avg per-neuron gauge RMS: %.4f\n", avg_pn);
        printf("    avg amplitude-only RMS:   %.4f\n", avg_amp);
        printf("    random baseline:          %.4f\n", g_random_rms);
    }
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 46: Per-Neuron Gauge Correction\n");
    printf("===============================================\n");

    npn_init();

    part_a_classify();
    part_b_train_all();
    part_c_catalog();
    part_d_decompose();
    part_e_correlate();
    part_f_summary();

    printf("\n===============================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("===============================================\n");
    return n_fail > 0 ? 1 : 0;
}
