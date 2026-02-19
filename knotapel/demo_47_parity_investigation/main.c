/*
 * KNOTAPEL DEMO 47: Parity Investigation
 * =======================================
 *
 * THESIS: Demo 46 proved that split-sigmoid training produces "classical"
 * networks -- magnitudes align with bracket lattice but phases don't.
 * Is parity's resistance to bracket decomposition an artifact of the
 * split-sigmoid activation (architectural) or a fundamental property
 * of the bracket algebra (algebraic)?
 *
 * Three activations compared:
 *   (0) split-sigmoid: baseline from Demo 46
 *   (1) MVN-continuous: z/|z|, maps to unit circle, preserves phase
 *   (2) MVN-k8: snap to nearest 8th root of unity, lattice-native
 *
 * For all 13 NPN classes of 3-input Boolean functions:
 *   - Train with each activation (3 hidden neurons, 10 trials)
 *   - Build bracket catalog (same as Demo 46)
 *   - Decompose: global gauge, per-neuron gauge, amplitude-only
 *   - Compare phase alignment with Z[zeta_8]
 *
 * Additional: parity deep dive with 3-6 hidden neurons.
 *
 * Predictions:
 *   P1: MVN-cont converges for >= 10/13 NPN classes
 *   P2: MVN-cont pn-gauge RMS < split-sigmoid pn-gauge RMS for parity
 *   P3: MVN-cont avg phase error < split-sigmoid avg phase error
 *   P4: Bottom-tier functions improve more with MVN than top-tier
 *   P5: More hidden neurons (4-6) don't help parity under split-sigmoid
 *   P6: MVN-k8 pn-gauge < MVN-cont pn-gauge for parity (if converges)
 *
 * Part A: NPN Classification & Topology Frequency
 * Part B: Training -- 3 activations x 13 classes
 * Part C: Complex Bracket Catalog
 * Part D: Decomposition Comparison
 * Part E: Parity Deep Dive (variable hidden neurons)
 * Part F: Phase Analysis
 * Part G: Summary & Predictions
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
 * Complex arithmetic (from Demo 46)
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
 * Braid types and closure (from Demo 46)
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
 * Bracket with closure support (from Demo 46)
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

typedef enum {
    ACT_SPLIT_SIG = 0,
    ACT_MVN_CONT  = 1,
    ACT_MVN_K8    = 2
} ActType;

#define NUM_ACT 3

static const char *act_names[NUM_ACT] = {
    "split-sigmoid", "MVN-cont", "MVN-k8"
};

static double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }
static double sigmoid_deriv(double s) { return s * (1.0 - s); }

static Cx cx_sigmoid(Cx z) {
    return cx_make(sigmoid(z.re), sigmoid(z.im));
}

/* MVN continuous: normalize to unit circle */
static Cx cx_mvn_cont(Cx z) {
    double r = cx_abs(z);
    if (r < 1e-10) return cx_make(1.0, 0.0);
    return cx_make(z.re / r, z.im / r);
}

/* MVN discrete k=8: snap to nearest 8th root of unity */
static Cx cx_mvn_k8(Cx z) {
    double theta = atan2(z.im, z.re);
    double sector = M_PI / 4.0;  /* 2*pi/8 */
    double snapped = floor(theta / sector + 0.5) * sector;
    return cx_exp_i(snapped);
}

static Cx cx_activate(Cx z, ActType act) {
    switch (act) {
    case ACT_SPLIT_SIG: return cx_sigmoid(z);
    case ACT_MVN_CONT:  return cx_mvn_cont(z);
    case ACT_MVN_K8:    return cx_mvn_k8(z);
    }
    return cx_sigmoid(z);
}

/* ================================================================
 * NPN equivalence for 3-input Boolean functions (from Demo 46)
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
 * Truth table from braid + closure (from Demo 46)
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
 * Complex Bracket Catalog (from Demo 46)
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
                Cx bval;
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

                bval = bracket_cl(&b, A, CL_TRACE);
                amp = cx_abs(bval);

                if (amp > 0.5 && cx_catalog_size < MAX_CX_CATALOG) {
                    cx_catalog[cx_catalog_size].braid = b;
                    cx_catalog[cx_catalog_size].bracket = bval;
                    cx_catalog[cx_catalog_size].amplitude = amp;
                    cx_catalog[cx_catalog_size].phase =
                        atan2(bval.im, bval.re);
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
 * Neural network with variable hidden neuron count
 * ================================================================ */

#define MAX_HIDDEN 6
#define NUM_TRIALS 10
#define MAX_EPOCHS 50000

typedef struct {
    int nh;
    Cx wh[MAX_HIDDEN][3];
    Cx bh[MAX_HIDDEN];
    double vo[MAX_HIDDEN * 2];
    double bo;
} CxNetVar;

static void cxnetvar_init(CxNetVar *net, int nh) {
    int k, j;
    net->nh = nh;
    for (k = 0; k < nh; k++) {
        for (j = 0; j < 3; j++)
            net->wh[k][j] = cx_make(rng_normal(), rng_normal());
        net->bh[k] = cx_make(rng_normal() * 0.5, rng_normal() * 0.5);
        net->vo[2 * k] = rng_normal() * 0.5;
        net->vo[2 * k + 1] = rng_normal() * 0.5;
    }
    net->bo = rng_normal() * 0.1;
}

static double cxnetvar_forward(const CxNetVar *net, const double x[3],
                                Cx h[MAX_HIDDEN], Cx zpre[MAX_HIDDEN],
                                ActType act) {
    int k, j;
    double out_pre = net->bo;
    for (k = 0; k < net->nh; k++) {
        Cx z = net->bh[k];
        for (j = 0; j < 3; j++)
            z = cx_add(z, cx_scale(net->wh[k][j], x[j]));
        zpre[k] = z;
        h[k] = cx_activate(z, act);
        out_pre += net->vo[2 * k] * h[k].re
                 + net->vo[2 * k + 1] * h[k].im;
    }
    return sigmoid(out_pre);
}

static int cxnetvar_train(CxNetVar *net, int tt, double lr,
                           int max_ep, ActType act) {
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
            Cx h[MAX_HIDDEN], zpre[MAX_HIDDEN];
            double out, err, delta_out;
            double dz_re[MAX_HIDDEN], dz_im[MAX_HIDDEN];

            out = cxnetvar_forward(net, inputs[i], h, zpre, act);
            err = out - targets[i];
            total_loss += err * err;
            if ((out > 0.5) == (targets[i] > 0.5)) correct++;

            delta_out = 2.0 * err * sigmoid_deriv(out);

            for (k = 0; k < net->nh; k++) {
                double dL_dhr = delta_out * net->vo[2 * k];
                double dL_dhi = delta_out * net->vo[2 * k + 1];

                switch (act) {
                case ACT_SPLIT_SIG:
                    dz_re[k] = dL_dhr * sigmoid_deriv(h[k].re);
                    dz_im[k] = dL_dhi * sigmoid_deriv(h[k].im);
                    break;
                case ACT_MVN_CONT:
                case ACT_MVN_K8: {
                    /*
                     * MVN gradient: d(z/|z|)/dz
                     * r = |z|, x = z.re, y = z.im
                     * dh_re/dx = y^2/r^3    dh_re/dy = -xy/r^3
                     * dh_im/dx = -xy/r^3    dh_im/dy = x^2/r^3
                     * (STE for MVN-k8: use continuous gradient)
                     */
                    double r = cx_abs(zpre[k]);
                    double r3 = r * r * r;
                    double xv = zpre[k].re;
                    double yv = zpre[k].im;
                    if (r3 < 1e-15) r3 = 1e-15;
                    dz_re[k] = dL_dhr * (yv * yv / r3)
                             + dL_dhi * (-xv * yv / r3);
                    dz_im[k] = dL_dhr * (-xv * yv / r3)
                             + dL_dhi * (xv * xv / r3);
                    break;
                }
                }

                /* Update output weights */
                net->vo[2 * k]     -= lr * delta_out * h[k].re;
                net->vo[2 * k + 1] -= lr * delta_out * h[k].im;

                /* Update input weights */
                for (j = 0; j < 3; j++) {
                    net->wh[k][j].re -= lr * dz_re[k] * inputs[i][j];
                    net->wh[k][j].im -= lr * dz_im[k] * inputs[i][j];
                }

                /* Update bias */
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
 * Data structures for results
 * ================================================================ */

#define MAX_NPN 80
#define MAX_CX_W (MAX_HIDDEN * 4)  /* inputs + biases */

typedef struct {
    int canon_tt;
    int orbit_size;
    int topo_freq;
    const char *titulis;
} NPNInfo;

typedef struct {
    int converged;
    int best_epoch;
    int nh;
    Cx weights[MAX_CX_W];
    int n_weights;
    double gauge_rms;
    double pn_gauge_rms;
    double amp_only_rms;
    double pn_best_angles[MAX_HIDDEN];
    double avg_phase_err;
} ClassResult;

static NPNInfo npn_info[MAX_NPN];
static int n_npn;

/* main_results[act][class_idx] */
static ClassResult main_results[NUM_ACT][MAX_NPN];

/* Parity deep dive results: [act][nh_index] where nh_index = nh-3 */
#define PARITY_NH_MIN 3
#define PARITY_NH_MAX 6
#define PARITY_NH_COUNT (PARITY_NH_MAX - PARITY_NH_MIN + 1)
static ClassResult parity_results[NUM_ACT][PARITY_NH_COUNT];

/* ================================================================
 * Spearman rank correlation (from Demo 46)
 * ================================================================ */

static void assign_ranks(const double *values, double *ranks, int n) {
    int order[MAX_NPN];
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
    double rx[MAX_NPN], ry[MAX_NPN];
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
 * Per-neuron gauge (generalized for variable n_hidden)
 *
 * Weight layout:
 *   weights[k*3 + wi] = neuron k, input wi  (k=0..nh-1, wi=0..2)
 *   weights[nh*3 + k]  = neuron k bias      (k=0..nh-1)
 * ================================================================ */

static double pn_gauge_rms_var(const Cx *wts, int nh,
                                const double *angles) {
    Cx rot[MAX_HIDDEN];
    double total = 0.0;
    int k, wi;
    int nw = nh * 4;

    for (k = 0; k < nh; k++)
        rot[k] = cx_exp_i(angles[k]);

    for (k = 0; k < nh; k++) {
        for (wi = 0; wi < 3; wi++) {
            Cx w_rot = cx_mul(wts[k * 3 + wi], rot[k]);
            int idx;
            double dist;
            find_nearest_cx(w_rot, &idx, &dist);
            total += dist * dist;
        }
        {
            Cx w_rot = cx_mul(wts[nh * 3 + k], rot[k]);
            int idx;
            double dist;
            find_nearest_cx(w_rot, &idx, &dist);
            total += dist * dist;
        }
    }
    return sqrt(total / (double)nw);
}

/* Full 3D sweep for nh=3 (same grid as Demo 46) */
static void pn_gauge_3d(const Cx *wts, int nh,
                         double *best_rms, double *best_angles) {
    int g0, g1, g2;
    double step = 2.0 * M_PI / 36.0;
    double angles[MAX_HIDDEN];

    (void)nh; /* always 3 */
    *best_rms = 1e30;

    for (g0 = 0; g0 < 36; g0++) {
        angles[0] = (double)g0 * step;
        for (g1 = 0; g1 < 36; g1++) {
            angles[1] = (double)g1 * step;
            for (g2 = 0; g2 < 36; g2++) {
                double rms;
                angles[2] = (double)g2 * step;
                rms = pn_gauge_rms_var(wts, 3, angles);
                if (rms < *best_rms) {
                    *best_rms = rms;
                    best_angles[0] = angles[0];
                    best_angles[1] = angles[1];
                    best_angles[2] = angles[2];
                }
            }
        }
    }

    /* Refinement: 1-degree grid around best */
    {
        double fine = 2.0 * M_PI / 360.0;
        int f0, f1, f2;
        for (f0 = -5; f0 <= 5; f0++) {
            angles[0] = best_angles[0] + (double)f0 * fine;
            for (f1 = -5; f1 <= 5; f1++) {
                angles[1] = best_angles[1] + (double)f1 * fine;
                for (f2 = -5; f2 <= 5; f2++) {
                    double rms;
                    angles[2] = best_angles[2] + (double)f2 * fine;
                    rms = pn_gauge_rms_var(wts, 3, angles);
                    if (rms < *best_rms) {
                        *best_rms = rms;
                        best_angles[0] = angles[0];
                        best_angles[1] = angles[1];
                        best_angles[2] = angles[2];
                    }
                }
            }
        }
    }
}

/* Coordinate descent for nh > 3 */
static void pn_gauge_coord(const Cx *wts, int nh,
                            double *best_rms, double *best_angles) {
    int iter, k, gi;
    double step = 2.0 * M_PI / 72.0; /* 5-degree steps */

    for (k = 0; k < nh; k++) best_angles[k] = 0.0;
    *best_rms = pn_gauge_rms_var(wts, nh, best_angles);

    for (iter = 0; iter < 3; iter++) {
        for (k = 0; k < nh; k++) {
            double saved = best_angles[k];
            for (gi = 0; gi < 72; gi++) {
                double rms;
                best_angles[k] = (double)gi * step;
                rms = pn_gauge_rms_var(wts, nh, best_angles);
                if (rms < *best_rms) {
                    *best_rms = rms;
                    saved = best_angles[k];
                }
            }
            best_angles[k] = saved;
        }
    }
}

static void pn_gauge_sweep(const Cx *wts, int nh,
                            double *best_rms, double *best_angles) {
    if (nh == 3)
        pn_gauge_3d(wts, nh, best_rms, best_angles);
    else
        pn_gauge_coord(wts, nh, best_rms, best_angles);
}

/* ================================================================
 * Phase error: avg angular distance to nearest multiple of pi/4
 * ================================================================ */

static double compute_phase_error(const Cx *wts, int nw) {
    double total_err = 0.0;
    double pi4 = M_PI / 4.0;
    int i;

    for (i = 0; i < nw; i++) {
        double theta = atan2(wts[i].im, wts[i].re);
        double nearest = floor(theta / pi4 + 0.5) * pi4;
        double err = fabs(theta - nearest);
        if (err > M_PI) err = 2.0 * M_PI - err;
        total_err += err;
    }
    return total_err / (double)nw;
}

/* ================================================================
 * Helper: extract weights from network into flat array
 * ================================================================ */

static int extract_weights(const CxNetVar *net, Cx *out) {
    int k, j, n = 0;
    for (k = 0; k < net->nh; k++)
        for (j = 0; j < 3; j++)
            out[n++] = net->wh[k][j];
    for (k = 0; k < net->nh; k++)
        out[n++] = net->bh[k];
    return n;
}

/* ================================================================
 * Helper: global gauge sweep (360 angles)
 * ================================================================ */

static double global_gauge_sweep(const Cx *wts, int nw,
                                  double *best_angle) {
    double best_rms = 1e30;
    int gi;

    for (gi = 0; gi < 360; gi++) {
        double theta = 2.0 * M_PI * (double)gi / 360.0;
        Cx rot = cx_exp_i(theta);
        double total = 0.0;
        int wi;

        for (wi = 0; wi < nw; wi++) {
            Cx w_rot = cx_mul(wts[wi], rot);
            int idx;
            double dist;
            find_nearest_cx(w_rot, &idx, &dist);
            total += dist * dist;
        }
        total = sqrt(total / (double)nw);

        if (total < best_rms) {
            best_rms = total;
            *best_angle = theta;
        }
    }
    return best_rms;
}

/* ================================================================
 * Helper: amplitude-only RMS
 * ================================================================ */

static double amplitude_only_rms(const Cx *wts, int nw) {
    double sum = 0.0;
    int wi;
    for (wi = 0; wi < nw; wi++) {
        double w_amp = cx_abs(wts[wi]);
        double min_d = 1e30;
        int ci;
        for (ci = 0; ci < cx_catalog_size; ci++) {
            double d = fabs(w_amp - cx_catalog[ci].amplitude);
            if (d < min_d) min_d = d;
        }
        sum += min_d * min_d;
    }
    return sqrt(sum / (double)nw);
}

/* ================================================================
 * Helper: full decomposition of a ClassResult
 * ================================================================ */

static void decompose_result(ClassResult *cr) {
    double angle;

    if (!cr->converged || cr->n_weights == 0) return;

    /* Global gauge */
    cr->gauge_rms = global_gauge_sweep(cr->weights, cr->n_weights,
                                        &angle);

    /* Amplitude-only */
    cr->amp_only_rms = amplitude_only_rms(cr->weights, cr->n_weights);

    /* Per-neuron gauge */
    pn_gauge_sweep(cr->weights, cr->nh,
                    &cr->pn_gauge_rms, cr->pn_best_angles);

    /* Phase error */
    cr->avg_phase_err = compute_phase_error(cr->weights,
                                             cr->n_weights);
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
        n_npn = 0;

        for (i = 1; i < 255; i++) {
            int c = npn_canon[i];
            if (!seen[c]) {
                seen[c] = 1;
                memset(&npn_info[n_npn], 0, sizeof(NPNInfo));
                npn_info[n_npn].canon_tt = c;
                npn_info[n_npn].titulis = fn_name(c);
                n_npn++;
            }
        }

        for (i = 1; i < 255; i++) {
            int c = npn_canon[i];
            int j;
            for (j = 0; j < n_npn; j++) {
                if (npn_info[j].canon_tt == c) {
                    npn_info[j].orbit_size++;
                    break;
                }
            }
        }
    }

    printf("  Non-trivial NPN classes: %d\n", n_npn);
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
            for (j = 0; j < n_npn; j++) {
                if (npn_info[j].canon_tt == c) {
                    npn_info[j].topo_freq += found[i];
                    break;
                }
            }
        }
    }

    printf("\n  %-6s %-6s %-8s %s\n",
           "Canon", "Orbit", "TopoFreq", "Name");
    printf("  %-6s %-6s %-8s %s\n",
           "------", "-----", "--------", "----");
    for (i = 0; i < n_npn; i++) {
        const char *nm = npn_info[i].titulis;
        printf("  0x%02x  %-6d %-8d %s\n",
               npn_info[i].canon_tt,
               npn_info[i].orbit_size,
               npn_info[i].topo_freq,
               nm ? nm : "?");
    }

    {
        int n_reached = 0;
        for (i = 0; i < n_npn; i++)
            if (npn_info[i].topo_freq > 0) n_reached++;
        sprintf(msg, "topology reaches %d / %d NPN classes",
                n_reached, n_npn);
        check(msg, n_reached > 0);
    }
}

/* ================================================================
 * PART B: Training -- 3 activations x 13 classes
 * ================================================================ */

static void part_b_train(void) {
    int act, i, trial;
    char msg[256];

    printf("\n=== PART B: Training (3 activations x %d classes) ===\n",
           n_npn);
    printf("  3 inputs -> 3 Cx hidden -> 1 real output\n");
    printf("  %d trials/class, max %d epochs\n\n", NUM_TRIALS,
           MAX_EPOCHS);

    for (act = 0; act < NUM_ACT; act++) {
        int total_conv = 0;
        printf("  --- %s ---\n", act_names[act]);

        for (i = 0; i < n_npn; i++) {
            int tt = npn_info[i].canon_tt;
            int best_epoch = 0;
            CxNetVar best_net;

            memset(&best_net, 0, sizeof(best_net));
            memset(&main_results[act][i], 0, sizeof(ClassResult));
            main_results[act][i].nh = 3;

            for (trial = 0; trial < NUM_TRIALS; trial++) {
                CxNetVar net;
                int epoch;

                /* Same seed for each (class, trial) across activations */
                rng_seed((unsigned long)(
                    (i * NUM_TRIALS + trial) * 7919 + 42));
                cxnetvar_init(&net, 3);
                epoch = cxnetvar_train(&net, tt, 0.5, MAX_EPOCHS,
                                       (ActType)act);

                if (epoch > 0 &&
                    (best_epoch == 0 || epoch < best_epoch)) {
                    best_net = net;
                    best_epoch = epoch;
                }
            }

            if (best_epoch > 0) {
                main_results[act][i].converged = 1;
                main_results[act][i].best_epoch = best_epoch;
                main_results[act][i].n_weights =
                    extract_weights(&best_net,
                                    main_results[act][i].weights);
                total_conv++;

                printf("    0x%02x %-10s epoch %5d\n",
                       tt,
                       npn_info[i].titulis ?
                           npn_info[i].titulis : "?",
                       best_epoch);
            } else {
                printf("    0x%02x %-10s FAILED\n",
                       tt,
                       npn_info[i].titulis ?
                           npn_info[i].titulis : "?");
            }
        }

        sprintf(msg, "%s: %d / %d converged",
                act_names[act], total_conv, n_npn);
        check(msg, total_conv > n_npn / 2);
    }
}

/* ================================================================
 * PART C: Bracket Catalog
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
                found_it = 1;
                break;
            }
        }
        if (!found_it && n_seen < 512)
            seen[n_seen++] = val;
    }
    n_distinct = n_seen;

    printf("  Distinct complex bracket values: %d\n", n_distinct);

    sprintf(msg, "catalog has >= 50 distinct values (%d)", n_distinct);
    check(msg, n_distinct >= 50);
}

/* ================================================================
 * PART D: Decomposition Comparison
 * ================================================================ */

static double g_random_rms = 0.0;

static void part_d_decompose(void) {
    int act, i;
    char msg[256];

    printf("\n=== PART D: Decomposition Comparison ===\n\n");

    if (cx_catalog_size == 0) {
        printf("  No catalog. Skipping.\n");
        return;
    }

    /* Random baseline */
    {
        int ri;
        double rsum = 0.0;
        rng_seed(99999);
        for (ri = 0; ri < 1000; ri++) {
            double mag = fabs(rng_normal()) * 2.0 + 0.5;
            double angle = rng_uniform() * 2.0 * M_PI;
            Cx pt = cx_scale(cx_exp_i(angle), mag);
            int idx;
            double dist;
            find_nearest_cx(pt, &idx, &dist);
            rsum += dist * dist;
        }
        g_random_rms = sqrt(rsum / 1000.0);
        printf("  Random baseline RMS: %.4f\n\n", g_random_rms);
    }

    for (act = 0; act < NUM_ACT; act++) {
        int n_beat = 0, n_conv = 0;
        printf("  --- %s ---\n", act_names[act]);

        for (i = 0; i < n_npn; i++) {
            ClassResult *cr = &main_results[act][i];
            if (!cr->converged) continue;

            decompose_result(cr);
            n_conv++;
            if (cr->pn_gauge_rms < g_random_rms) n_beat++;

            printf("    0x%02x %-10s gl=%.4f pn=%.4f amp=%.4f "
                   "ph_err=%.4f\n",
                   npn_info[i].canon_tt,
                   npn_info[i].titulis ?
                       npn_info[i].titulis : "?",
                   cr->gauge_rms,
                   cr->pn_gauge_rms,
                   cr->amp_only_rms,
                   cr->avg_phase_err);
        }

        sprintf(msg, "%s: %d / %d beat random (%.4f)",
                act_names[act], n_beat, n_conv, g_random_rms);
        check(msg, n_conv == 0 || n_beat > n_conv / 2);
    }
}

/* ================================================================
 * PART E: Parity Deep Dive (variable hidden neurons)
 * ================================================================ */

static void part_e_parity(void) {
    int act, nhi, trial;
    int parity_tt = 0x69; /* XNOR3 */

    printf("\n=== PART E: Parity Deep Dive (nh = 3-6) ===\n\n");

    if (cx_catalog_size == 0) {
        printf("  No catalog. Skipping.\n");
        return;
    }

    for (act = 0; act < NUM_ACT; act++) {
        printf("  --- %s ---\n", act_names[act]);

        for (nhi = 0; nhi < PARITY_NH_COUNT; nhi++) {
            int nh = PARITY_NH_MIN + nhi;
            int best_epoch = 0;
            CxNetVar best_net;
            ClassResult *cr = &parity_results[act][nhi];

            memset(cr, 0, sizeof(ClassResult));
            memset(&best_net, 0, sizeof(best_net));
            cr->nh = nh;

            for (trial = 0; trial < NUM_TRIALS; trial++) {
                CxNetVar net;
                int epoch;

                rng_seed((unsigned long)(
                    (100 + nhi) * NUM_TRIALS * 7919
                    + trial * 31 + 77));
                cxnetvar_init(&net, nh);
                epoch = cxnetvar_train(&net, parity_tt, 0.5,
                                       MAX_EPOCHS, (ActType)act);

                if (epoch > 0 &&
                    (best_epoch == 0 || epoch < best_epoch)) {
                    best_net = net;
                    best_epoch = epoch;
                }
            }

            if (best_epoch > 0) {
                cr->converged = 1;
                cr->best_epoch = best_epoch;
                cr->n_weights = extract_weights(&best_net,
                                                 cr->weights);
                decompose_result(cr);

                printf("    nh=%d  epoch=%5d  gl=%.4f  pn=%.4f  "
                       "amp=%.4f  ph=%.4f\n",
                       nh, best_epoch,
                       cr->gauge_rms,
                       cr->pn_gauge_rms,
                       cr->amp_only_rms,
                       cr->avg_phase_err);
            } else {
                printf("    nh=%d  FAILED\n", nh);
            }
        }
    }
}

/* ================================================================
 * PART F: Phase Analysis
 * ================================================================ */

static void part_f_phase(void) {
    int act, i;
    double avg_phase[NUM_ACT];
    int n_conv[NUM_ACT];

    printf("\n=== PART F: Phase Analysis ===\n\n");

    /* Per-activation average phase error */
    for (act = 0; act < NUM_ACT; act++) {
        avg_phase[act] = 0.0;
        n_conv[act] = 0;
    }

    printf("  %-6s %-10s  ", "Canon", "Name");
    for (act = 0; act < NUM_ACT; act++)
        printf("%-14s  ", act_names[act]);
    printf("\n");

    printf("  %-6s %-10s  ", "------", "----");
    for (act = 0; act < NUM_ACT; act++)
        printf("%-14s  ", "--------------");
    printf("\n");

    for (i = 0; i < n_npn; i++) {
        printf("  0x%02x  %-10s  ",
               npn_info[i].canon_tt,
               npn_info[i].titulis ?
                   npn_info[i].titulis : "?");

        for (act = 0; act < NUM_ACT; act++) {
            ClassResult *cr = &main_results[act][i];
            if (cr->converged) {
                printf("%.4f          ", cr->avg_phase_err);
                avg_phase[act] += cr->avg_phase_err;
                n_conv[act]++;
            } else {
                printf("--              ");
            }
        }
        printf("\n");
    }

    printf("\n  Averages:\n");
    for (act = 0; act < NUM_ACT; act++) {
        if (n_conv[act] > 0)
            avg_phase[act] /= (double)n_conv[act];
        printf("    %-14s: %.4f rad (n=%d)\n",
               act_names[act], avg_phase[act], n_conv[act]);
    }
}

/* ================================================================
 * PART G: Summary & Predictions
 * ================================================================ */

static void part_g_summary(void) {
    int act, i;
    int parity_idx = -1;
    char msg[256];

    printf("\n=== PART G: Summary & Predictions ===\n\n");

    /* Find parity class index */
    for (i = 0; i < n_npn; i++) {
        if (npn_info[i].canon_tt == 0x69) {
            parity_idx = i;
            break;
        }
    }

    /* Master comparison table */
    printf("  Master Table (per-neuron gauge RMS):\n\n");
    printf("  %-6s %-10s %-4s  ", "Canon", "Name", "Freq");
    for (act = 0; act < NUM_ACT; act++)
        printf("%-14s  ", act_names[act]);
    printf("\n");
    printf("  %-6s %-10s %-4s  ", "------", "----", "----");
    for (act = 0; act < NUM_ACT; act++)
        printf("%-14s  ", "--------------");
    printf("\n");

    for (i = 0; i < n_npn; i++) {
        printf("  0x%02x  %-10s %-4d  ",
               npn_info[i].canon_tt,
               npn_info[i].titulis ?
                   npn_info[i].titulis : "?",
               npn_info[i].topo_freq);

        for (act = 0; act < NUM_ACT; act++) {
            ClassResult *cr = &main_results[act][i];
            if (cr->converged)
                printf("%.4f          ", cr->pn_gauge_rms);
            else
                printf("--              ");
        }
        printf("\n");
    }

    /* Averages */
    printf("\n  Averages (converged classes):\n");
    printf("  %-14s  %7s  %7s  %7s  %7s\n",
           "Activation", "gl-RMS", "pn-RMS", "amp-RMS", "ph-err");
    printf("  %-14s  %7s  %7s  %7s  %7s\n",
           "--------------", "-------", "-------", "-------", "-------");

    for (act = 0; act < NUM_ACT; act++) {
        double sum_gl = 0, sum_pn = 0, sum_amp = 0, sum_ph = 0;
        int nc = 0;
        for (i = 0; i < n_npn; i++) {
            ClassResult *cr = &main_results[act][i];
            if (cr->converged) {
                sum_gl  += cr->gauge_rms;
                sum_pn  += cr->pn_gauge_rms;
                sum_amp += cr->amp_only_rms;
                sum_ph  += cr->avg_phase_err;
                nc++;
            }
        }
        if (nc > 0) {
            printf("  %-14s  %7.4f  %7.4f  %7.4f  %7.4f  (n=%d)\n",
                   act_names[act],
                   sum_gl / (double)nc,
                   sum_pn / (double)nc,
                   sum_amp / (double)nc,
                   sum_ph / (double)nc,
                   nc);
        }
    }

    /* Parity deep dive summary */
    printf("\n  Parity Deep Dive (XNOR3, per-neuron gauge RMS):\n");
    printf("  %-14s  ", "Activation");
    {
        int nhi;
        for (nhi = 0; nhi < PARITY_NH_COUNT; nhi++)
            printf("nh=%-4d  ", PARITY_NH_MIN + nhi);
    }
    printf("\n  %-14s  ", "--------------");
    {
        int nhi;
        for (nhi = 0; nhi < PARITY_NH_COUNT; nhi++)
            printf("-------  ");
    }
    printf("\n");

    for (act = 0; act < NUM_ACT; act++) {
        int nhi;
        printf("  %-14s  ", act_names[act]);
        for (nhi = 0; nhi < PARITY_NH_COUNT; nhi++) {
            ClassResult *cr = &parity_results[act][nhi];
            if (cr->converged)
                printf("%.4f   ", cr->pn_gauge_rms);
            else
                printf("--       ");
        }
        printf("\n");
    }

    /* Spearman correlation per activation */
    printf("\n  Spearman rho(pn-RMS, freq) per activation:\n");
    for (act = 0; act < NUM_ACT; act++) {
        double rms_arr[MAX_NPN], freq_arr[MAX_NPN];
        int nc = 0;
        double rho;
        for (i = 0; i < n_npn; i++) {
            if (main_results[act][i].converged &&
                npn_info[i].topo_freq > 0) {
                rms_arr[nc] = main_results[act][i].pn_gauge_rms;
                freq_arr[nc] = (double)npn_info[i].topo_freq;
                nc++;
            }
        }
        rho = spearman_rho(rms_arr, freq_arr, nc);
        printf("    %-14s: rho = %.4f (n=%d)\n",
               act_names[act], rho, nc);
    }

    /* ============================================================
     * PREDICTION CHECKS
     * ============================================================ */

    printf("\n  --- Prediction Checks ---\n\n");

    /* P1: MVN-cont converges for >= 10/13 */
    {
        int nc = 0;
        for (i = 0; i < n_npn; i++)
            if (main_results[ACT_MVN_CONT][i].converged) nc++;
        sprintf(msg, "P1: MVN-cont converges for >= 10/13 (%d/13)", nc);
        check(msg, nc >= 10);
    }

    /* P2: MVN-cont pn-gauge < split-sig pn-gauge for parity */
    if (parity_idx >= 0 &&
        main_results[ACT_SPLIT_SIG][parity_idx].converged &&
        main_results[ACT_MVN_CONT][parity_idx].converged) {
        double ss = main_results[ACT_SPLIT_SIG][parity_idx].pn_gauge_rms;
        double mc = main_results[ACT_MVN_CONT][parity_idx].pn_gauge_rms;
        sprintf(msg, "P2: MVN-cont parity pn-RMS (%.4f) < "
                "split-sig (%.4f)", mc, ss);
        check(msg, mc < ss);
    } else {
        sprintf(msg, "P2: skipped (parity not converged for both)");
        check(msg, 0);
    }

    /* P3: MVN-cont avg phase error < split-sig avg phase error */
    {
        double ss_ph = 0, mc_ph = 0;
        int ss_n = 0, mc_n = 0;
        for (i = 0; i < n_npn; i++) {
            if (main_results[ACT_SPLIT_SIG][i].converged) {
                ss_ph += main_results[ACT_SPLIT_SIG][i].avg_phase_err;
                ss_n++;
            }
            if (main_results[ACT_MVN_CONT][i].converged) {
                mc_ph += main_results[ACT_MVN_CONT][i].avg_phase_err;
                mc_n++;
            }
        }
        if (ss_n > 0) ss_ph /= (double)ss_n;
        if (mc_n > 0) mc_ph /= (double)mc_n;
        sprintf(msg, "P3: MVN-cont avg phase err (%.4f) < "
                "split-sig (%.4f)", mc_ph, ss_ph);
        check(msg, mc_n > 0 && ss_n > 0 && mc_ph < ss_ph);
    }

    /* P4: Bottom-tier improvement with MVN > top-tier
     * (compare how much MVN-cont improves over split-sig
     *  for bottom vs top classes by Demo 45/46 ranking) */
    {
        double top_imp = 0, bot_imp = 0;
        int ti = 0, bi = 0;

        /* Use topology frequency to define tiers:
         * top = topo_freq in top 3, bottom = topo_freq == 0 */
        for (i = 0; i < n_npn; i++) {
            double ss_rms, mc_rms, imp;
            if (!main_results[ACT_SPLIT_SIG][i].converged ||
                !main_results[ACT_MVN_CONT][i].converged)
                continue;

            ss_rms = main_results[ACT_SPLIT_SIG][i].pn_gauge_rms;
            mc_rms = main_results[ACT_MVN_CONT][i].pn_gauge_rms;
            imp = ss_rms - mc_rms; /* positive = MVN improved */

            if (npn_info[i].topo_freq >= 100) {
                top_imp += imp; ti++;
            } else if (npn_info[i].topo_freq == 0) {
                bot_imp += imp; bi++;
            }
        }
        if (ti > 0) top_imp /= (double)ti;
        if (bi > 0) bot_imp /= (double)bi;
        sprintf(msg, "P4: bottom-tier MVN improvement (%.4f) > "
                "top-tier (%.4f)", bot_imp, top_imp);
        check(msg, ti > 0 && bi > 0 && bot_imp > top_imp);
    }

    /* P5: More hidden neurons don't help parity under split-sig */
    {
        int any_big_improve = 0;
        int nhi;
        double base_rms = 0.0;

        if (parity_results[ACT_SPLIT_SIG][0].converged)
            base_rms = parity_results[ACT_SPLIT_SIG][0].pn_gauge_rms;

        for (nhi = 1; nhi < PARITY_NH_COUNT; nhi++) {
            ClassResult *cr = &parity_results[ACT_SPLIT_SIG][nhi];
            if (cr->converged && base_rms > 0.01) {
                double improvement = base_rms - cr->pn_gauge_rms;
                /* "significant" = more than 20% relative improvement */
                if (improvement > 0.2 * base_rms)
                    any_big_improve = 1;
            }
        }
        sprintf(msg, "P5: more neurons don't significantly help "
                "parity under split-sig");
        check(msg, !any_big_improve);
    }

    /* P6: MVN-k8 pn-gauge < MVN-cont pn-gauge for parity */
    if (parity_idx >= 0 &&
        main_results[ACT_MVN_CONT][parity_idx].converged &&
        main_results[ACT_MVN_K8][parity_idx].converged) {
        double mc = main_results[ACT_MVN_CONT][parity_idx].pn_gauge_rms;
        double mk = main_results[ACT_MVN_K8][parity_idx].pn_gauge_rms;
        sprintf(msg, "P6: MVN-k8 parity pn-RMS (%.4f) < "
                "MVN-cont (%.4f)", mk, mc);
        check(msg, mk < mc);
    } else {
        sprintf(msg, "P6: skipped (parity not converged for MVN-k8)");
        check(msg, 0);
    }

    /* Random baseline reminder */
    printf("\n  Random baseline RMS: %.4f\n", g_random_rms);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 47: Parity Investigation\n");
    printf("=======================================\n");

    npn_init();

    part_a_classify();
    part_b_train();
    part_c_catalog();
    part_d_decompose();
    part_e_parity();
    part_f_phase();
    part_g_summary();

    printf("\n=======================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("=======================================\n");

    return n_fail > 0 ? 1 : 0;
}
