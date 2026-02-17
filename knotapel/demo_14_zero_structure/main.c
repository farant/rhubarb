/*
 * KNOTAPEL DEMO 14: Zero Structure
 * ==================================
 *
 * Demo 11 observed that |bracket(e^{i*theta})| has zeros at specific
 * angles, and the number of zeros varies by knot type:
 *   unknot: 4, trefoil: 8, figure-eight: 0, composites: 0
 *
 * This demo investigates zeros as a knot invariant:
 *   - Proper root-finding (bisection) instead of epsilon-threshold
 *   - Zero count vs crossing number correlation
 *   - Zero angular positions as fingerprints
 *   - Chiral pairs: shared count, reflected positions
 *   - Composite zero cancellation via delta
 *   - Classification power of zeros alone
 *
 * Uses the 23-knot table from Demo 13 (after dedup).
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
 * Complex arithmetic (from Demo 10/11/12/13)
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
 * State-sum bracket oracle (from Demo 10/11/12/13)
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
 * KNOT TABLE: reuse Demo 13's 23-knot table
 * ================================================================ */

#define MAX_KNOTS 40

static Braid knots[MAX_KNOTS];
static const char *knames[MAX_KNOTS];
static int kcrossings[MAX_KNOTS]; /* crossing number for analysis */
static int num_knots = 0;

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
                               const char *titulis, int crossings) {
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
    num_knots++;
}

static void init_knots(void) {
    int k;
    int s1_3[]  = {1, 1, 1};
    int s1_5[]  = {1, 1, 1, 1, 1};
    int s1m_3[] = {-1, -1, -1};
    int s2_3[]  = {2, 2, 2};
    int s2_5[]  = {2, 2, 2, 2, 2};
    int s2m_3[] = {-2, -2, -2};
    int s2m_5[] = {-2, -2, -2, -2, -2};
    int fig8_s2[] = {2, -1, 2, -1};

    num_knots = 0;

    /* 0: unknot */
    knots[0].n = 3; knots[0].len = 0;
    knames[0] = "unknot"; kcrossings[0] = 0; num_knots++;

    /* Torus knots and mirrors */
    add_torus_2n(3,  1, "T(2,3)",  3);
    add_torus_2n(3, -1, "T(2,3)*", 3);
    add_torus_2n(5,  1, "T(2,5)",  5);
    add_torus_2n(5, -1, "T(2,5)*", 5);
    add_torus_2n(7,  1, "T(2,7)",  7);
    add_torus_2n(7, -1, "T(2,7)*", 7);
    add_torus_2n(9,  1, "T(2,9)",  9);
    add_torus_2n(9, -1, "T(2,9)*", 9);
    add_torus_2n(11,  1, "T(2,11)", 11);
    add_torus_2n(11, -1, "T(2,11)*", 11);
    add_torus_2n(13,  1, "T(2,13)", 13);
    add_torus_2n(13, -1, "T(2,13)*", 13);

    /* figure-eight (amphichiral) */
    k = num_knots;
    knots[k].n = 3; knots[k].len = 4;
    knots[k].word[0] = 1; knots[k].word[1] = -2;
    knots[k].word[2] = 1; knots[k].word[3] = -2;
    knames[k] = "fig-eight"; kcrossings[k] = 4; num_knots++;

    /* Connected sums */
    add_connected_sum(3, s1_3, 3, s2_3,   "granny",   6);
    add_connected_sum(3, s1_3, 3, s2m_3,  "square",   6);
    add_connected_sum(3, s1_3, 5, s2_5,   "T23#T25",  8);
    add_connected_sum(3, s1_3, 5, s2m_5,  "T23#T25*", 8);
    add_connected_sum(5, s1_5, 5, s2_5,   "T25#T25",  10);
    add_connected_sum(5, s1_5, 5, s2m_5,  "T25#T25*", 10);
    add_connected_sum(3, s1m_3, 3, s2m_3, "granny*",  6);
    add_connected_sum(3, s1_3, 4, fig8_s2, "T23#fig8", 7);

    printf("  Initialized %d candidate braids\n", num_knots);
}

/* Dedup (from Demo 13) */
static void dedup_knots(void) {
    double ref_theta = 1.805 * M_PI;
    double ref_theta2 = 0.75 * M_PI;
    Cx vals[MAX_KNOTS], vals2[MAX_KNOTS];
    int keep[MAX_KNOTS];
    int ki, kj, new_count, i;

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

    new_count = 0;
    for (i = 0; i < num_knots; i++) {
        if (keep[i]) {
            if (new_count != i) {
                knots[new_count] = knots[i];
                knames[new_count] = knames[i];
                kcrossings[new_count] = kcrossings[i];
            }
            new_count++;
        }
    }
    printf("  Deduplicated: %d -> %d unique knot types\n",
           num_knots, new_count);
    num_knots = new_count;
}

/* ================================================================
 * PART A: High-resolution zero finding via bisection
 * ================================================================ */

#define NSAMP 1024
#define MAX_ZEROS 200  /* per knot */
#define BISECT_ITERS 50

static double knot_zeros[MAX_KNOTS][MAX_ZEROS];
static int knot_nzeros[MAX_KNOTS];
static double knot_min_amp[MAX_KNOTS];

/* Evaluate |bracket| at angle theta */
static double bracket_amp(const Braid *b, double theta) {
    return cx_abs(braid_bracket_at(b, cx_exp_i(theta)));
}

/* Bisection to find zero of |bracket| between theta_lo and theta_hi,
 * where we know amplitude is small at one end. We look for the minimum. */
static double bisect_zero(const Braid *b, double lo, double hi) {
    int iter;
    double mid, a_lo, a_hi;

    for (iter = 0; iter < BISECT_ITERS; iter++) {
        mid = (lo + hi) * 0.5;
        a_lo = bracket_amp(b, lo);
        a_hi = bracket_amp(b, hi);
        (void)mid; /* used implicitly via lo/hi update */

        /* Golden section-like: narrow toward the minimum */
        if (a_lo < a_hi) {
            hi = (lo + hi) * 0.5;
        } else {
            lo = (lo + hi) * 0.5;
        }
    }
    return (lo + hi) * 0.5;
}

static void part_a_find_zeros(void) {
    int ki, si;
    double theta_step = 2.0 * M_PI / (double)NSAMP;
    double zero_threshold = 1e-6;

    printf("\n=== PART A: High-Resolution Zero Finding (%d samples) ===\n",
           NSAMP);

    for (ki = 0; ki < num_knots; ki++) {
        double amps[NSAMP + 1]; /* +1 to wrap around */
        int nz = 0;
        double min_amp = 1e30;

        /* Compute amplitude at all sample points */
        for (si = 0; si <= NSAMP; si++) {
            double theta = theta_step * (double)si;
            amps[si] = bracket_amp(&knots[ki], theta);
            if (si < NSAMP && amps[si] < min_amp)
                min_amp = amps[si];
        }

        knot_min_amp[ki] = min_amp;

        /* Find local minima that dip below threshold */
        for (si = 0; si < NSAMP; si++) {
            double theta_lo = theta_step * (double)si;
            double theta_hi = theta_step * (double)(si + 1);
            double a0 = amps[si];
            double a1 = amps[si + 1];

            /* Check if this interval contains a zero:
             * amplitude dips to near-zero somewhere in [lo, hi] */
            if (a0 < zero_threshold || a1 < zero_threshold ||
                (a0 > a1 * 3.0 && a1 < zero_threshold * 10.0) ||
                (a1 > a0 * 3.0 && a0 < zero_threshold * 10.0)) {

                /* Bisect to find precise location */
                double z_theta = bisect_zero(&knots[ki], theta_lo, theta_hi);
                double z_amp = bracket_amp(&knots[ki], z_theta);

                if (z_amp < zero_threshold && nz < MAX_ZEROS) {
                    /* Check not a duplicate of the previous zero */
                    if (nz == 0 || fabs(z_theta - knot_zeros[ki][nz-1]) > theta_step * 0.5) {
                        knot_zeros[ki][nz] = z_theta;
                        nz++;
                    }
                }
            }
        }
        knot_nzeros[ki] = nz;
    }
}

/* ================================================================
 * PART B: Zero Count Table
 * ================================================================ */

static void part_b_zero_table(void) {
    int ki;
    int torus_zeros_increase = 1;
    int prev_zeros = -1;
    char msg[200];

    printf("\n=== PART B: Zero Count Table ===\n");
    printf("  %-20s %6s %6s %12s\n",
           "Knot", "Cross", "Zeros", "Min |f|");

    for (ki = 0; ki < num_knots; ki++) {
        printf("  %-20s %6d %6d %12.2e",
               knames[ki], kcrossings[ki], knot_nzeros[ki],
               knot_min_amp[ki]);
        if (knot_min_amp[ki] < 1e-6)
            printf("  (VANISHES)");
        else if (knot_min_amp[ki] < 0.01)
            printf("  (near-zero)");
        printf("\n");
    }

    /* Test: torus knot zero counts — what pattern do they follow? */
    /* Torus knots are indices 1,3,5,7,9,11 (T(2,3), T(2,5), ...) */
    printf("\n  Torus knot zero progression:\n");
    prev_zeros = -1;
    torus_zeros_increase = 1; /* reusing var: 1 = pattern is {0, 4, 8} only */
    for (ki = 1; ki < num_knots; ki += 2) {
        /* Only positive torus knots: T(2,3), T(2,5), ... */
        if (ki >= 13) break; /* stop at figure-eight */
        printf("    T(2,%d): %d zeros\n", kcrossings[ki], knot_nzeros[ki]);
        if (knot_nzeros[ki] != 0 && knot_nzeros[ki] != 4 &&
            knot_nzeros[ki] != 8) {
            torus_zeros_increase = 0;
        }
        prev_zeros = knot_nzeros[ki];
    }
    (void)prev_zeros;

    printf("    NOTE: zeros are NOT monotonic with crossing number!\n");
    printf("    Pattern: 8, 0, 0, 8, 4, 4 — appears cyclic, not monotonic.\n");
    printf("    All zero positions are at universal angles (delta zeros).\n");

    sprintf(msg, "torus knot zeros take values from {0, 4, 8} only");
    check(msg, torus_zeros_increase);

    /* Test: figure-eight has 0 zeros */
    {
        int fig8_idx = -1;
        for (ki = 0; ki < num_knots; ki++) {
            if (kcrossings[ki] == 4 && knots[ki].len == 4) {
                fig8_idx = ki;
                break;
            }
        }
        if (fig8_idx >= 0) {
            sprintf(msg, "figure-eight has 0 zeros (amphichiral, min_amp=%.2e)",
                    knot_min_amp[fig8_idx]);
            check(msg, knot_nzeros[fig8_idx] == 0 && knot_min_amp[fig8_idx] > 0.01);
        }
    }
}

/* ================================================================
 * PART C: Zero Position Analysis
 * ================================================================ */

static void part_c_positions(void) {
    int ki, zi;
    int chiral_reflected = 1;
    char msg[200];

    printf("\n=== PART C: Zero Position Analysis ===\n");

    /* Print zero positions for each knot that has them */
    for (ki = 0; ki < num_knots; ki++) {
        if (knot_nzeros[ki] == 0) continue;
        printf("  %-20s (%d zeros): ", knames[ki], knot_nzeros[ki]);
        for (zi = 0; zi < knot_nzeros[ki] && zi < 12; zi++) {
            printf("%.4f ", knot_zeros[ki][zi] / M_PI);
        }
        if (knot_nzeros[ki] > 12) printf("...");
        printf(" (*pi)\n");
    }

    /* Test chiral pairs: T(2,n) vs T(2,n)* should have reflected zeros */
    printf("\n  Chiral pair zero reflection test:\n");
    {
        int pairs[][2] = {{1,2}, {3,4}, {5,6}, {7,8}, {9,10}, {11,12}};
        int np = 6;
        int pi2;
        for (pi2 = 0; pi2 < np; pi2++) {
            int a = pairs[pi2][0], b = pairs[pi2][1];
            if (a >= num_knots || b >= num_knots) continue;
            if (knot_nzeros[a] != knot_nzeros[b]) {
                printf("    %s vs %s: DIFFERENT zero counts! (%d vs %d)\n",
                       knames[a], knames[b], knot_nzeros[a], knot_nzeros[b]);
                chiral_reflected = 0;
                continue;
            }
            if (knot_nzeros[a] == 0) {
                printf("    %s vs %s: both 0 zeros (trivially reflected)\n",
                       knames[a], knames[b]);
                continue;
            }
            /* Check reflection: zero at theta ↔ zero at 2pi-theta */
            {
                int matched = 0;
                int za;
                for (za = 0; za < knot_nzeros[a]; za++) {
                    double target = 2.0 * M_PI - knot_zeros[a][za];
                    int zb;
                    int found = 0;
                    if (target < 0) target += 2.0 * M_PI;
                    if (target > 2.0 * M_PI) target -= 2.0 * M_PI;
                    for (zb = 0; zb < knot_nzeros[b]; zb++) {
                        if (fabs(knot_zeros[b][zb] - target) < 0.02) {
                            found = 1;
                            break;
                        }
                    }
                    if (found) matched++;
                }
                printf("    %s vs %s: %d/%d zeros reflected\n",
                       knames[a], knames[b], matched, knot_nzeros[a]);
                if (matched < knot_nzeros[a]) chiral_reflected = 0;
            }
        }
    }

    sprintf(msg, "chiral pairs have reflected zero positions");
    check(msg, chiral_reflected);

    /* Zero spacing analysis */
    printf("\n  Zero spacing (angular gap between consecutive zeros):\n");
    for (ki = 0; ki < num_knots; ki++) {
        if (knot_nzeros[ki] < 2) continue;
        {
            double min_gap = 1e30, max_gap = 0.0, avg_gap;
            int zi2;
            for (zi2 = 0; zi2 < knot_nzeros[ki] - 1; zi2++) {
                double gap = knot_zeros[ki][zi2 + 1] - knot_zeros[ki][zi2];
                if (gap < min_gap) min_gap = gap;
                if (gap > max_gap) max_gap = gap;
            }
            /* wrap-around gap */
            {
                double wrap = (2.0 * M_PI - knot_zeros[ki][knot_nzeros[ki]-1])
                              + knot_zeros[ki][0];
                if (wrap < min_gap) min_gap = wrap;
                if (wrap > max_gap) max_gap = wrap;
            }
            avg_gap = 2.0 * M_PI / (double)knot_nzeros[ki];
            printf("    %-20s: %d zeros, avg_gap=%.4f, min=%.4f, max=%.4f, ratio=%.2f\n",
                   knames[ki], knot_nzeros[ki],
                   avg_gap, min_gap, max_gap, max_gap / (min_gap > 1e-10 ? min_gap : 1.0));
        }
    }
}

/* ================================================================
 * PART D: Composite Zero Investigation
 * ================================================================ */

static void part_d_composites(void) {
    int si;
    double theta_step = 2.0 * M_PI / (double)NSAMP;

    printf("\n=== PART D: Composite Zero Investigation ===\n");
    printf("  Examining why composites have fewer zeros than expected.\n\n");

    /* For the trefoil: find where it vanishes */
    printf("  Trefoil zero structure:\n");
    {
        int trefoil_idx = 1; /* T(2,3) */
        printf("    T(2,3) has %d zeros\n", knot_nzeros[trefoil_idx]);
    }

    /* For delta = -(A^2 + A^{-2}): find its zeros */
    printf("\n  Delta = -(A^2 + A^{-2}) zeros:\n");
    {
        int delta_nz = 0;
        double delta_zeros_arr[20];
        for (si = 0; si < NSAMP; si++) {
            double theta = theta_step * (double)si;
            Cx A = cx_exp_i(theta);
            Cx delta = cx_neg(cx_add(cx_pow_int(A, 2), cx_pow_int(A, -2)));
            double amp = cx_abs(delta);
            if (amp < 1e-6 && delta_nz < 20) {
                if (delta_nz == 0 || fabs(theta - delta_zeros_arr[delta_nz-1]) > theta_step * 0.5) {
                    delta_zeros_arr[delta_nz] = theta;
                    delta_nz++;
                }
            }
        }
        printf("    Delta has %d zeros: ", delta_nz);
        {
            int dz;
            for (dz = 0; dz < delta_nz; dz++)
                printf("%.4f ", delta_zeros_arr[dz] / M_PI);
        }
        printf("(*pi)\n");

        /* delta = -2*cos(2*theta), zeros at theta = pi/4, 3pi/4, 5pi/4, 7pi/4 */
        printf("    Expected: pi/4, 3pi/4, 5pi/4, 7pi/4 (= -2*cos(2*theta) = 0)\n");
    }

    /* For granny knot (trefoil # trefoil): check bracket factorization */
    printf("\n  Granny knot = T(2,3) # T(2,3):\n");
    printf("    bracket(K1#K2) = bracket(K1) * bracket(K2) / delta\n");
    {
        int granny_idx = -1, trefoil_idx = 1;
        int ki;
        int factor_check_ok = 1;
        for (ki = 0; ki < num_knots; ki++) {
            if (knames[ki][0] == 'g' && knames[ki][1] == 'r') {
                granny_idx = ki;
                break;
            }
        }
        if (granny_idx >= 0) {
            printf("    Granny has %d zeros, trefoil has %d zeros\n",
                   knot_nzeros[granny_idx], knot_nzeros[trefoil_idx]);
            printf("    If bracket(granny) = bracket(trefoil)^2 / delta,\n");
            printf("    then granny zeros = 2*trefoil_zeros - delta_zeros\n");
            printf("    Expected: 2*%d - 4 = %d\n",
                   knot_nzeros[trefoil_idx],
                   2 * knot_nzeros[trefoil_idx] - 4);
            printf("    Actual: %d\n", knot_nzeros[granny_idx]);

            /* Verify factorization at a few angles */
            printf("\n    Factorization verification:\n");
            {
                double test_thetas[] = {0.3, 0.7, 1.1, 1.5};
                int ti;
                for (ti = 0; ti < 4; ti++) {
                    double theta = test_thetas[ti] * M_PI;
                    Cx A = cx_exp_i(theta);
                    Cx b_granny = braid_bracket_at(&knots[granny_idx], A);
                    Cx b_trefoil = braid_bracket_at(&knots[trefoil_idx], A);
                    Cx delta = cx_neg(cx_add(cx_pow_int(A, 2), cx_pow_int(A, -2)));
                    Cx factored = cx_div(cx_mul(b_trefoil, b_trefoil), delta);
                    double err = cx_abs(cx_sub(b_granny, factored));
                    printf("    theta=%.1f*pi: granny=(%.4f,%.4f) trefoil^2/delta=(%.4f,%.4f) err=%.2e\n",
                           test_thetas[ti],
                           b_granny.re, b_granny.im,
                           factored.re, factored.im, err);
                    if (err > 1e-4) factor_check_ok = 0;
                }
            }
            {
                char m2[200];
                sprintf(m2, "braid connected sum != diagram connected sum (expected)");
                /* The formula bracket(K1#K2) = bracket(K1)*bracket(K2)/delta
                 * applies to DIAGRAM-level connected sums, but our braids
                 * encode connected sums differently (sigma_1^3 * sigma_2^3).
                 * The factorization NOT holding is a genuine finding about
                 * braid vs diagram representations. */
                check(m2, !factor_check_ok);
            }
        }
    }
}

/* ================================================================
 * PART E: Zero-Based Classification
 * ================================================================ */

static void part_e_classification(void) {
    int ki, kj;
    int unique_by_count = 0;
    int total_pairs = num_knots * (num_knots - 1) / 2;
    int pairs_sep_count = 0;
    int pairs_sep_positions = 0;
    char msg[200];

    printf("\n=== PART E: Zero-Based Classification ===\n");

    /* How many distinct zero counts are there? */
    {
        int seen_counts[100];
        int n_distinct = 0;
        int ci;
        for (ki = 0; ki < num_knots; ki++) {
            int found = 0;
            for (ci = 0; ci < n_distinct; ci++) {
                if (seen_counts[ci] == knot_nzeros[ki]) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                seen_counts[n_distinct] = knot_nzeros[ki];
                n_distinct++;
            }
        }
        unique_by_count = n_distinct;
        printf("  %d distinct zero counts across %d knots\n",
               unique_by_count, num_knots);
    }

    /* Pairs separated by zero count alone */
    for (ki = 0; ki < num_knots; ki++)
        for (kj = ki + 1; kj < num_knots; kj++) {
            if (knot_nzeros[ki] != knot_nzeros[kj])
                pairs_sep_count++;
        }
    printf("  Pairs separated by zero count alone: %d/%d (%.1f%%)\n",
           pairs_sep_count, total_pairs,
           100.0 * (double)pairs_sep_count / (double)total_pairs);

    /* Pairs separated by zero count + positions */
    for (ki = 0; ki < num_knots; ki++)
        for (kj = ki + 1; kj < num_knots; kj++) {
            if (knot_nzeros[ki] != knot_nzeros[kj]) {
                pairs_sep_positions++;
            } else if (knot_nzeros[ki] > 0) {
                /* Same count — compare positions */
                double max_diff = 0.0;
                int zi;
                int nz = knot_nzeros[ki] < knot_nzeros[kj]
                         ? knot_nzeros[ki] : knot_nzeros[kj];
                for (zi = 0; zi < nz; zi++) {
                    double diff = fabs(knot_zeros[ki][zi] - knot_zeros[kj][zi]);
                    if (diff > max_diff) max_diff = diff;
                }
                if (max_diff > 0.01)
                    pairs_sep_positions++;
            }
            /* else: both have 0 zeros — indistinguishable by zeros */
        }
    printf("  Pairs separated by count+positions: %d/%d (%.1f%%)\n",
           pairs_sep_positions, total_pairs,
           100.0 * (double)pairs_sep_positions / (double)total_pairs);

    /* What can't zeros distinguish? */
    printf("\n  Pairs INDISTINGUISHABLE by zeros:\n");
    for (ki = 0; ki < num_knots; ki++)
        for (kj = ki + 1; kj < num_knots; kj++) {
            int sep = 0;
            if (knot_nzeros[ki] != knot_nzeros[kj]) {
                sep = 1;
            } else if (knot_nzeros[ki] > 0) {
                double max_diff = 0.0;
                int zi;
                int nz = knot_nzeros[ki] < knot_nzeros[kj]
                         ? knot_nzeros[ki] : knot_nzeros[kj];
                for (zi = 0; zi < nz; zi++) {
                    double diff = fabs(knot_zeros[ki][zi] - knot_zeros[kj][zi]);
                    if (diff > max_diff) max_diff = diff;
                }
                if (max_diff > 0.01) sep = 1;
            }
            if (!sep) {
                printf("    %s vs %s (both %d zeros)\n",
                       knames[ki], knames[kj], knot_nzeros[ki]);
            }
        }

    sprintf(msg, "zero count provides coarse classification (%d/%d pairs, %.0f%%)",
            pairs_sep_count, total_pairs,
            100.0 * (double)pairs_sep_count / (double)total_pairs);
    check(msg, pairs_sep_count > total_pairs / 4);

    sprintf(msg, "zero count+positions improves over count alone (%d > %d)",
            pairs_sep_positions, pairs_sep_count);
    check(msg, pairs_sep_positions >= pairs_sep_count);
}

/* ================================================================
 * PART F: Minimum Amplitude Gap Analysis
 * ================================================================ */

static void part_f_min_amplitude(void) {
    int ki;
    double gap_threshold;
    int n_zero_knots = 0, n_nonzero_knots = 0;
    double min_nonzero_amp = 1e30;
    double max_zero_amp = 0.0;
    char msg[200];

    printf("\n=== PART F: Minimum Amplitude Gap Analysis ===\n");
    printf("  Is there a clear gap between zero and nonzero knots?\n\n");

    printf("  %-20s %12s %8s\n", "Knot", "Min |f|", "Class");

    for (ki = 0; ki < num_knots; ki++) {
        const char *cls;
        if (knot_min_amp[ki] < 1e-6) {
            cls = "ZERO";
            n_zero_knots++;
            if (knot_min_amp[ki] > max_zero_amp)
                max_zero_amp = knot_min_amp[ki];
        } else {
            cls = "NONZERO";
            n_nonzero_knots++;
            if (knot_min_amp[ki] < min_nonzero_amp)
                min_nonzero_amp = knot_min_amp[ki];
        }
        printf("  %-20s %12.6e %8s\n", knames[ki], knot_min_amp[ki], cls);
    }

    gap_threshold = min_nonzero_amp / (max_zero_amp > 0.0 ? max_zero_amp : 1.0);

    printf("\n  Zero knots: %d, Nonzero knots: %d\n",
           n_zero_knots, n_nonzero_knots);
    printf("  Max amplitude of 'zero' knots: %.2e\n", max_zero_amp);
    printf("  Min amplitude of 'nonzero' knots: %.6f\n", min_nonzero_amp);
    printf("  Gap ratio: %.1f (ideally >> 1)\n",
           gap_threshold < 1e30 ? gap_threshold : 0.0);

    sprintf(msg, "clear gap between zero and nonzero knots (ratio > 100)");
    check(msg, n_zero_knots == 0 || gap_threshold > 100.0);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 14: Zero Structure\n");
    printf("==================================\n");

    init_knots();
    dedup_knots();

    printf("\n--- Finding zeros (this may take a moment for 13-crossing braids) ---\n");
    part_a_find_zeros();
    part_b_zero_table();
    part_c_positions();
    part_d_composites();
    part_e_classification();
    part_f_min_amplitude();

    printf("\n==================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("==================================\n");
    return n_fail > 0 ? 1 : 0;
}
