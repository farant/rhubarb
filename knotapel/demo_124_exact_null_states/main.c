/*
 * KNOTAPEL DEMO 124: D84's Null States, Exactly
 * ================================================================
 *
 * D84 asked whether the bracket-null entries of the zeta_8 catalog
 * (Re(q) = 0, half-angle exactly 90 degrees) are computational dead
 * weight. Its recorded answer (atlas entry, findings): removing the 9
 * nulls drops capacity from XOR8 to XOR6, worse than random removal;
 * the null-only subset reaches XOR6; null-only XOR6 is flat in k_sec
 * and beats non-null at k_sec = 1.
 *
 * D84's own code, rebuilt from its only commit and run on this machine
 * (2026-10-10), prints something else: non-null max_xor = 8 (equal to
 * the full catalog), null-only max_xor = 0, k_sec = 1 null-only 0 =
 * non-null 0. D87's zeta_8 baseline agrees with this run (non-null XOR8
 * = 4), not with D84's atlas.
 *
 * The suspicion: the decisive decisions are exact TIES. A null sum has
 * half-angle exactly 90 degrees, i.e. it sits ON a sector boundary for
 * every even k; in float its real part is closure noise ~1e-17 whose
 * sign and size decide the sector. Voronoi ties (equal |dot| to two
 * axes) are frequent in this cuboctahedral geometry too.
 *
 * This demo:
 *   Part A  D84's float search replicated bit for bit (its numbers on
 *           this machine reproduced first);
 *   Part B  the catalog exactly: zeta_8 in Z[sqrt2] (components (p +
 *           q sqrt2)/2), D84's BFS order, census, gap, directions;
 *   Part C  every cell decision D84's float search makes, against the
 *           exact cell with D84's semantics (floor of the sector, first
 *           maximal axis) and its tie set;
 *   Part D  capacity EXHAUSTIVELY (D84 extends only its first 32
 *           winners): full / non-null / null-only, under the exact
 *           rule, robust (every tie resolution) and possible (some
 *           resolution); D84's greedy search with exact cells beside it;
 *           and each subset with the FULL 13-axis activation table
 *           (D84 rebuilds the table from the subset, so removing nulls
 *           also removes 6 activation axes);
 *   Part E  random controls: D84's own 100 + 100 subsets (same rand()
 *           sequence) and 10,000 + 10,000 more, exactly;
 *   Part F  the k_sec sweep and null prevalence among ALL winners;
 *   Part G  the claims.
 *
 * Exact sector: half-angle h of a sum (a, v): tan^2 h = |v|^2 / a^2 in
 * Q(sqrt2); sector = #{m < k/2 : tan^2 h >= tan^2(m pi/k)} (+ k/2 if
 * a = 0), the boundaries placed as roots of S_k(u) = sum C(k, 2l+1)
 * (-1)^l u^l by POSITION (D123) and compared with radices - k = 10, 16,
 * 20 need nested radicals. Exact axis: max of (v.u)^2 / |u|^2 in
 * Q(sqrt2). Separability is downward closed (a separable weight set's
 * subsets separate), so the level-wise search over all subsets is
 * exhaustive.
 *
 * House library: radices.h. Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_124_exact_null_states/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 */

#include "radices.h"
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

static int n_pass = 0;
static int n_fail = 0;

static void
check (
    const char *msg,
    int         ok)
{
    if (ok) {
        n_pass++;
        printf("  PASS: %s\n", msg);
    } else {
        n_fail++;
        printf("  FAIL: %s\n", msg);
    }
}

static void
die (
    const char *what)
{
    printf("  FATAL: %s\n", what);
    exit(1);
}

static Piscina *piscina;

/* ================================================================
 * Part A machinery: D84's float code, replicated (names f_*)
 * ================================================================ */

typedef struct { double a, b, c, d; } Quat;

static Quat
f_qmul (
    const Quat *p,
    const Quat *g)
{
    Quat r;

    r.a = p->a*g->a - p->b*g->b - p->c*g->c - p->d*g->d;
    r.b = p->a*g->b + p->b*g->a + p->c*g->d - p->d*g->c;
    r.c = p->a*g->c - p->b*g->d + p->c*g->a + p->d*g->b;
    r.d = p->a*g->d + p->b*g->c - p->c*g->b + p->d*g->a;
    return r;
}

#define MAX_QCAT 64
#define MAX_ROUNDS 16

static Quat f_gen[3];
static Quat f_cat[MAX_QCAT];
static int  f_depth[MAX_QCAT];
static int  f_writhe[MAX_QCAT];
static int  f_cat_size = 0;
static int  f_rounds = 0;
static const int gen_writhe[4] = { 1, -1, 1, -1 };

static void
f_init_su2 (
    double half_angle)
{
    double co = cos(half_angle);
    double sn = sin(half_angle);

    f_gen[1].a = co; f_gen[1].b = sn; f_gen[1].c = 0; f_gen[1].d = 0;
    f_gen[2].a = co; f_gen[2].b = 0;  f_gen[2].c = 0; f_gen[2].d = -sn;
}

static int
f_find_in_cat (
    const Quat *q,
    int         n)
{
    int i;

    for (i = 0; i < n; i++) {
        if (fabs(f_cat[i].a - q->a) < 1e-10 && fabs(f_cat[i].b - q->b) < 1e-10
            && fabs(f_cat[i].c - q->c) < 1e-10
            && fabs(f_cat[i].d - q->d) < 1e-10) {
            return i;
        }
        if (fabs(f_cat[i].a + q->a) < 1e-10 && fabs(f_cat[i].b + q->b) < 1e-10
            && fabs(f_cat[i].c + q->c) < 1e-10
            && fabs(f_cat[i].d + q->d) < 1e-10) {
            return i;
        }
    }
    return -1;
}

static void
f_build_closure (void)
{
    Quat gens[4];
    int  prev;
    int  i;
    int  gi;
    int  rd;

    f_cat[0].a = 1; f_cat[0].b = 0; f_cat[0].c = 0; f_cat[0].d = 0;
    f_depth[0] = 0;
    f_writhe[0] = 0;
    f_cat_size = 1;
    gens[0] = f_gen[1];
    gens[1].a = f_gen[1].a; gens[1].b = -f_gen[1].b;
    gens[1].c = -f_gen[1].c; gens[1].d = -f_gen[1].d;
    gens[2] = f_gen[2];
    gens[3].a = f_gen[2].a; gens[3].b = -f_gen[2].b;
    gens[3].c = -f_gen[2].c; gens[3].d = -f_gen[2].d;
    for (gi = 0; gi < 4; gi++) {
        if (f_find_in_cat(&gens[gi], f_cat_size) < 0
            && f_cat_size < MAX_QCAT) {
            f_depth[f_cat_size] = 0;
            f_writhe[f_cat_size] = gen_writhe[gi];
            f_cat[f_cat_size++] = gens[gi];
        }
    }
    f_rounds = 1;
    rd = 1;
    do {
        prev = f_cat_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                Quat prod = f_qmul(&f_cat[i], &gens[gi]);

                if (f_find_in_cat(&prod, f_cat_size) < 0
                    && f_cat_size < MAX_QCAT) {
                    f_depth[f_cat_size] = rd;
                    f_writhe[f_cat_size] = f_writhe[i] + gen_writhe[gi];
                    f_cat[f_cat_size++] = prod;
                }
            }
        }
        if (f_cat_size > prev) f_rounds++;
        rd++;
    } while (f_cat_size > prev && f_rounds < MAX_ROUNDS);
}

/* the working (sub)catalog of the replica, as D84's g_cat */
static Quat g_cat[MAX_QCAT];
static int  g_idx[MAX_QCAT];      /* index into the full catalog */
static int  g_cat_size = 0;

static void
load_subset (
    const int *indices,
    int        count)
{
    int i;

    for (i = 0; i < count; i++) {
        g_cat[i] = f_cat[indices[i]];
        g_idx[i] = indices[i];
    }
    g_cat_size = count;
}

#define MAX_DIR 64

static double g_dir[MAX_DIR][3];
static int    g_nd = 0;

static void exact_table_from_current (void);     /* Part C hook */

static void
f_build_dirs (
    int cat_size)
{
    int i;
    int j;

    g_nd = 0;
    for (i = 0; i < cat_size; i++) {
        double qa = g_cat[i].a;
        double qb = g_cat[i].b;
        double qc = g_cat[i].c;
        double qd = g_cat[i].d;
        double nv;
        double ax;
        double ay;
        double az;
        int    found = 0;

        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        if (nv < 1e-12) continue;
        ax = qb / nv; ay = qc / nv; az = qd / nv;
        for (j = 0; j < g_nd; j++) {
            double d1 = fabs(g_dir[j][0] - ax) + fabs(g_dir[j][1] - ay)
                + fabs(g_dir[j][2] - az);
            double d2 = fabs(g_dir[j][0] + ax) + fabs(g_dir[j][1] + ay)
                + fabs(g_dir[j][2] + az);

            if (d1 < 1e-8 || d2 < 1e-8) { found = 1; break; }
        }
        if (!found && g_nd < MAX_DIR) {
            g_dir[g_nd][0] = ax; g_dir[g_nd][1] = ay; g_dir[g_nd][2] = az;
            g_nd++;
        }
    }
    exact_table_from_current();
}

static int
f_vor_cell (
    double ax,
    double ay,
    double az)
{
    int    i;
    int    best = 0;
    double bd = -2.0;

    for (i = 0; i < g_nd; i++) {
        double dp = fabs(ax*g_dir[i][0] + ay*g_dir[i][1] + az*g_dir[i][2]);

        if (dp > bd) { bd = dp; best = i; }
    }
    return best;
}

static int
f_combined_cell (
    double sa,
    double sb,
    double sc,
    double sd,
    int    k_sec)
{
    double n = sqrt(sa*sa + sb*sb + sc*sc + sd*sd);
    double qa;
    double nv;
    double half_ang;
    double ang;
    int    sec;
    int    vor;
    int    n_vor;

    n_vor = g_nd + 1;
    if (n < 1e-12) return (k_sec - 1) * n_vor + g_nd;
    qa = sa / n;
    if (qa < 0) { qa = -qa; sb = -sb; sc = -sc; sd = -sd; }
    if (qa > 1.0) qa = 1.0;
    nv = sqrt(sb*sb + sc*sc + sd*sd) / n;
    half_ang = acos(qa);
    ang = 2.0 * half_ang * 180.0 / M_PI;
    sec = (int)(ang * (double)k_sec / 360.0);
    if (sec >= k_sec) sec = k_sec - 1;
    if (sec < 0) sec = 0;
    if (nv < 1e-12) {
        vor = g_nd;
    } else {
        double inv = 1.0 / (nv * n);

        vor = f_vor_cell(sb * inv, sc * inv, sd * inv);
    }
    return sec * n_vor + vor;
}

/* Part C instrumentation: every float cell decision against the exact
 * cell of the same sum (set while the replica runs) */
static int  instrument = 0;
static void instrument_mask (const int *indices, int n_weights, int mask,
    int k_sec, int float_cell, double float_re);

#define MAX_ACT 16384

static int cell_even[MAX_ACT];
static int cell_odd[MAX_ACT];
static int touched_cells[MAX_ACT];

static int
f_test_xor (
    const int *indices,
    int        n_weights,
    int        k_sec)
{
    int    n_inputs = 2 * n_weights;
    int    n_masks;
    int    n_vor;
    int    n_cells;
    double wa[14];
    double wb[14];
    double wc[14];
    double wd[14];
    int    n_touched = 0;
    int    mask;
    int    i;
    int    result = 1;

    n_masks = 1 << n_inputs;
    n_vor = g_nd + 1;
    n_cells = k_sec * n_vor;
    if (n_cells > MAX_ACT || n_inputs > 14) return 0;
    for (i = 0; i < n_weights; i++) {
        const Quat *q = &g_cat[indices[i]];

        wa[2*i] = q->a; wb[2*i] = q->b; wc[2*i] = q->c; wd[2*i] = q->d;
        wa[2*i+1] = -q->a; wb[2*i+1] = -q->b;
        wc[2*i+1] = -q->c; wd[2*i+1] = -q->d;
    }
    for (mask = 0; mask < n_masks; mask++) {
        double sa = 0;
        double sb = 0;
        double sc = 0;
        double sd = 0;
        int    par = 0;
        int    cell;

        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                sa += wa[i]; sb += wb[i]; sc += wc[i]; sd += wd[i];
                par ^= 1;
            }
        }
        cell = f_combined_cell(sa, sb, sc, sd, k_sec);
        if (instrument) {
            instrument_mask(indices, n_weights, mask, k_sec, cell, sa);
        }
        if (cell_even[cell] == 0 && cell_odd[cell] == 0) {
            touched_cells[n_touched++] = cell;
        }
        if (par == 0) {
            cell_even[cell]++;
            if (cell_odd[cell] > 0) { result = 0; goto cleanup; }
        } else {
            cell_odd[cell]++;
            if (cell_even[cell] > 0) { result = 0; goto cleanup; }
        }
    }
cleanup:
    for (i = 0; i < n_touched; i++) {
        cell_even[touched_cells[i]] = 0;
        cell_odd[touched_cells[i]] = 0;
    }
    return result;
}

#define MAX_WIN 32
#define MAX_TUPLE 7

static const int K_LADDER[] = { 1, 6, 8, 10, 12, 16, 20, 24 };
#define N_KLADDER 8

typedef int (*TestFn) (const int *indices, int n_weights, int k_sec);

static int g_win[8][MAX_WIN][MAX_TUPLE];
static int g_nwin[8];

static void
greedy_xor6 (
    int    bf_limit,
    TestFn test)
{
    int ai;
    int aj;
    int ak;
    int ki;
    int indices[3];

    g_nwin[3] = 0;
    for (ai = 0; ai < bf_limit && g_nwin[3] < MAX_WIN; ai++)
    for (aj = ai + 1; aj < bf_limit && g_nwin[3] < MAX_WIN; aj++)
    for (ak = aj + 1; ak < bf_limit && g_nwin[3] < MAX_WIN; ak++) {
        indices[0] = ai; indices[1] = aj; indices[2] = ak;
        for (ki = 0; ki < N_KLADDER; ki++) {
            if (K_LADDER[ki] * (g_nd + 1) > MAX_ACT) continue;
            if (test(indices, 3, K_LADDER[ki])) {
                g_win[3][g_nwin[3]][0] = ai;
                g_win[3][g_nwin[3]][1] = aj;
                g_win[3][g_nwin[3]][2] = ak;
                g_nwin[3]++;
                break;
            }
        }
    }
}

static void
greedy_recursive (
    int    level,
    int    cat_limit,
    TestFn test)
{
    int prev = level - 1;
    int wi;
    int ae;
    int ki;
    int i;
    int j;

    g_nwin[level] = 0;
    if (g_nwin[prev] == 0) return;
    for (wi = 0; wi < g_nwin[prev] && g_nwin[level] < MAX_WIN; wi++) {
        for (ae = 0; ae < cat_limit && g_nwin[level] < MAX_WIN; ae++) {
            int in_winner = 0;
            int dup = 0;
            int indices[MAX_TUPLE];
            int pos;

            for (i = 0; i < prev; i++) {
                if (g_win[prev][wi][i] == ae) { in_winner = 1; break; }
            }
            if (in_winner) continue;
            pos = prev;
            for (i = 0; i < prev; i++) {
                if (ae < g_win[prev][wi][i]) { pos = i; break; }
            }
            for (i = 0; i < pos; i++) indices[i] = g_win[prev][wi][i];
            indices[pos] = ae;
            for (i = pos; i < prev; i++) indices[i + 1] = g_win[prev][wi][i];
            for (j = 0; j < g_nwin[level]; j++) {
                int match = 1;

                for (i = 0; i < level; i++) {
                    if (g_win[level][j][i] != indices[i]) {
                        match = 0;
                        break;
                    }
                }
                if (match) { dup = 1; break; }
            }
            if (dup) continue;
            for (ki = 0; ki < N_KLADDER; ki++) {
                if (K_LADDER[ki] * (g_nd + 1) > MAX_ACT) continue;
                if (test(indices, level, K_LADDER[ki])) {
                    memcpy(g_win[level][g_nwin[level]], indices,
                        (size_t)level * sizeof(int));
                    g_nwin[level]++;
                    break;
                }
            }
        }
    }
}

typedef struct {
    int xor6;
    int xor8;
    int xor10;
    int xor12;
    int max_xor;
} CapResult;

/* D84's find_capacity (count_angles dropped: it does not feed back) */
static void
greedy_capacity (
    int        cat_size,
    TestFn     test,
    CapResult *r)
{
    int bf_limit = cat_size < 100 ? cat_size : 100;

    f_build_dirs(cat_size);
    memset(g_nwin, 0, sizeof(g_nwin));
    memset(r, 0, sizeof(*r));
    greedy_xor6(bf_limit, test);
    r->xor6 = g_nwin[3];
    if (g_nwin[3] > 0) greedy_recursive(4, cat_size, test);
    r->xor8 = g_nwin[4];
    if (g_nwin[4] > 0) greedy_recursive(5, cat_size, test);
    r->xor10 = g_nwin[5];
    if (g_nwin[5] > 0) greedy_recursive(6, cat_size, test);
    r->xor12 = g_nwin[6];
    if (r->xor6 > 0) r->max_xor = 6;
    if (r->xor8 > 0) r->max_xor = 8;
    if (r->xor10 > 0) r->max_xor = 10;
    if (r->xor12 > 0) r->max_xor = 12;
}

static int
f_count_xor6_at_k (
    int    cat_size,
    int    k_sec,
    TestFn test)
{
    int ai;
    int aj;
    int ak;
    int count = 0;
    int indices[3];

    if (k_sec * (g_nd + 1) > MAX_ACT) return -1;
    for (ai = 0; ai < cat_size; ai++)
    for (aj = ai + 1; aj < cat_size; aj++)
    for (ak = aj + 1; ak < cat_size; ak++) {
        indices[0] = ai; indices[1] = aj; indices[2] = ak;
        if (test(indices, 3, k_sec)) count++;
    }
    return count;
}

static void
random_subset (
    int *subset,
    int  subset_size,
    int  total_size)
{
    static int perm[MAX_QCAT];
    int        i;
    int        j;
    int        tmp;

    for (i = 0; i < total_size; i++) perm[i] = i;
    for (i = 0; i < subset_size; i++) {
        j = i + (rand() % (total_size - i));
        tmp = perm[i]; perm[i] = perm[j]; perm[j] = tmp;
        subset[i] = perm[i];
    }
}

/* ================================================================
 * Exact arithmetic: Z[sqrt2], quaternions with components (p+q sqrt2)/2
 * ================================================================ */

typedef struct { long p; long q; } Z2;          /* p + q sqrt2 */

static Z2 z2 (long p, long q) { Z2 r; r.p = p; r.q = q; return r; }
static Z2 z2_add (Z2 x, Z2 y) { return z2(x.p + y.p, x.q + y.q); }
static Z2 z2_sub (Z2 x, Z2 y) { return z2(x.p - y.p, x.q - y.q); }
static Z2 z2_neg (Z2 x) { return z2(-x.p, -x.q); }
static int z2_zero (Z2 x) { return x.p == 0 && x.q == 0; }
static int z2_eq (Z2 x, Z2 y) { return x.p == y.p && x.q == y.q; }

static Z2
z2_mul (
    Z2 x,
    Z2 y)
{
    return z2(x.p * y.p + 2 * x.q * y.q, x.p * y.q + x.q * y.p);
}

static Z2
z2_half (
    Z2 x)
{
    if ((x.p & 1) || (x.q & 1)) die("z2_half: odd");
    return z2(x.p / 2, x.q / 2);
}

static int
z2_sign (
    Z2 x)
{
    if (x.p >= 0 && x.q >= 0) return (x.p != 0 || x.q != 0) ? 1 : 0;
    if (x.p <= 0 && x.q <= 0) return -1;
    if (x.p > 0) return x.p * x.p > 2 * x.q * x.q ? 1 : -1;
    return 2 * x.q * x.q > x.p * x.p ? 1 : -1;
}

static double z2_val (Z2 x) { return (double)x.p + (double)x.q * sqrt(2.0); }

typedef struct { Z2 a, b, c, d; } QE;          /* numerators over 2 */

static QE
qe_mul (
    const QE *p,
    const QE *g)
{
    QE r;

    r.a = z2_half(z2_sub(z2_sub(z2_sub(z2_mul(p->a, g->a), z2_mul(p->b, g->b)),
        z2_mul(p->c, g->c)), z2_mul(p->d, g->d)));
    r.b = z2_half(z2_sub(z2_add(z2_add(z2_mul(p->a, g->b), z2_mul(p->b, g->a)),
        z2_mul(p->c, g->d)), z2_mul(p->d, g->c)));
    r.c = z2_half(z2_add(z2_add(z2_sub(z2_mul(p->a, g->c), z2_mul(p->b, g->d)),
        z2_mul(p->c, g->a)), z2_mul(p->d, g->b)));
    r.d = z2_half(z2_add(z2_sub(z2_add(z2_mul(p->a, g->d), z2_mul(p->b, g->c)),
        z2_mul(p->c, g->b)), z2_mul(p->d, g->a)));
    return r;
}

static QE
qe_add (
    QE x,
    QE y)
{
    QE r;

    r.a = z2_add(x.a, y.a); r.b = z2_add(x.b, y.b);
    r.c = z2_add(x.c, y.c); r.d = z2_add(x.d, y.d);
    return r;
}

static QE
qe_neg (
    QE x)
{
    QE r;

    r.a = z2_neg(x.a); r.b = z2_neg(x.b); r.c = z2_neg(x.c); r.d = z2_neg(x.d);
    return r;
}

static int
qe_eq (
    const QE *x,
    const QE *y)
{
    return z2_eq(x->a, y->a) && z2_eq(x->b, y->b) && z2_eq(x->c, y->c)
        && z2_eq(x->d, y->d);
}

static QE
qe_zero (void)
{
    QE r;

    r.a = z2(0, 0); r.b = r.a; r.c = r.a; r.d = r.a;
    return r;
}

/* exact catalog in D84's BFS order */
static QE  e_cat[MAX_QCAT];
static int e_depth[MAX_QCAT];
static int e_writhe[MAX_QCAT];
static int e_null[MAX_QCAT];
static int e_cat_size = 0;
static int e_rounds = 0;

static int
e_find (
    const QE *q,
    int       n)
{
    int i;

    for (i = 0; i < n; i++) {
        QE m = qe_neg(*q);

        if (qe_eq(&e_cat[i], q) || qe_eq(&e_cat[i], &m)) return i;
    }
    return -1;
}

static void
e_build_closure (void)
{
    QE  gens[4];
    int prev;
    int i;
    int gi;
    int rd;

    e_cat[0] = qe_zero();
    e_cat[0].a = z2(2, 0);                         /* 1 */
    e_depth[0] = 0;
    e_writhe[0] = 0;
    e_cat_size = 1;
    gens[0] = qe_zero(); gens[0].a = z2(0, 1); gens[0].b = z2(0, 1);
    gens[1] = gens[0]; gens[1].b = z2(0, -1);                /* conjugate */
    gens[2] = qe_zero(); gens[2].a = z2(0, 1); gens[2].d = z2(0, -1);
    gens[3] = gens[2]; gens[3].d = z2(0, 1);
    for (gi = 0; gi < 4; gi++) {
        if (e_find(&gens[gi], e_cat_size) < 0) {
            e_depth[e_cat_size] = 0;
            e_writhe[e_cat_size] = gen_writhe[gi];
            e_cat[e_cat_size++] = gens[gi];
        }
    }
    e_rounds = 1;
    rd = 1;
    do {
        prev = e_cat_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                QE prod = qe_mul(&e_cat[i], &gens[gi]);

                if (e_find(&prod, e_cat_size) < 0) {
                    if (e_cat_size >= MAX_QCAT) die("catalog overflow");
                    e_depth[e_cat_size] = rd;
                    e_writhe[e_cat_size] = e_writhe[i] + gen_writhe[gi];
                    e_cat[e_cat_size++] = prod;
                }
            }
        }
        if (e_cat_size > prev) e_rounds++;
        rd++;
    } while (e_cat_size > prev && e_rounds < MAX_ROUNDS);
    for (i = 0; i < e_cat_size; i++) e_null[i] = z2_zero(e_cat[i].a);
}

/* ================================================================
 * Exact activation: axis table, sector boundaries, point memo
 * ================================================================ */

typedef struct {
    int nd;
    Z2  x[MAX_DIR];
    Z2  y[MAX_DIR];
    Z2  z[MAX_DIR];
    Z2  n2[MAX_DIR];
} Table;

/* axes in D84's order: first appearance, antipodes merged (parallel
 * = zero cross product) */
static void
table_build (
    Table     *t,
    const int *cat_idx,
    int        n)
{
    int i;
    int j;

    t->nd = 0;
    for (i = 0; i < n; i++) {
        const QE *q = &e_cat[cat_idx[i]];
        int found = 0;

        if (z2_zero(q->b) && z2_zero(q->c) && z2_zero(q->d)) continue;
        for (j = 0; j < t->nd && !found; j++) {
            found = z2_zero(z2_sub(z2_mul(q->c, t->z[j]), z2_mul(q->d, t->y[j])))
                && z2_zero(z2_sub(z2_mul(q->d, t->x[j]), z2_mul(q->b, t->z[j])))
                && z2_zero(z2_sub(z2_mul(q->b, t->y[j]), z2_mul(q->c, t->x[j])));
        }
        if (!found) {
            if (t->nd >= MAX_DIR) die("table overflow");
            t->x[t->nd] = q->b; t->y[t->nd] = q->c; t->z[t->nd] = q->d;
            t->n2[t->nd] = z2_add(z2_add(z2_mul(q->b, q->b), z2_mul(q->c, q->c)),
                z2_mul(q->d, q->d));
            t->nd++;
        }
    }
}

/* the k values used anywhere: the ladder and the sweep */
#define NK 10
static const int KS_ALL[NK] = { 1, 2, 4, 6, 8, 10, 12, 16, 20, 24 };

static int
k_index (
    int k)
{
    int i;

    for (i = 0; i < NK; i++) {
        if (KS_ALL[i] == k) return i;
    }
    die("k_index");
    return -1;
}

/* boundaries m < k/2: tan^2(m pi/k) = root m of S_k (position) */
static RadixRealis bnd[NK][12];
static int         bnd_n[NK];

static Polynomium
tan2_polynomial (
    int N)
{
    Magnus     bin[64];
    Magnus     c[32];
    Polynomium q = polynomium_nullum();
    int        d = (N - 1) / 2;
    int        m;
    int        l;

    bin[0] = magnus_ex_s64(1);
    for (m = 1; m <= N; m++) {
        if (!magnus_divide(magnus_multiplica(bin[m - 1],
            magnus_ex_s64(N - m + 1), piscina), magnus_ex_s64(m), piscina,
            &bin[m], NULL)) {
            die("binomial");
        }
    }
    for (l = 0; l <= d; l++) {
        c[l] = l % 2 ? magnus_nega(bin[2 * l + 1], piscina) : bin[2 * l + 1];
    }
    if (!polynomium_ex_coefficientibus(c, (i32)(d + 1), 0, piscina, &q)) {
        die("polynomium_ex_coefficientibus");
    }
    return q;
}

static void
prepare_boundaries (void)
{
    int ki;

    for (ki = 0; ki < NK; ki++) {
        int          k = KS_ALL[ki];
        int          want = (k - 1) / 2;            /* m < k/2 */
        RadixRealis *r;
        i32          n = 0;
        int          m;

        bnd_n[ki] = want;
        if (want == 0) continue;
        if (!radices_reales(tan2_polynomial(k), piscina, &r, &n)
            || (int)n != want) {
            die("boundary isolation");
        }
        for (m = 1; m <= want; m++) bnd[ki][m] = r[m - 1];
    }
}

/* sector entries, keyed by T = tan^2 h = (tx + ty sqrt2) / td, td > 0 */
typedef struct {
    long        tx;
    long        ty;
    long        td;
    signed char sec[NK];
    signed char tie[NK];
} SectorEntry;

#define SEC_CAP 65536
static SectorEntry sec_tab[SEC_CAP];
static int         sec_n = 0;
static int         sec_hash[2 * SEC_CAP];

static long
gcd_l (
    long a,
    long b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) {
        long t = a % b;

        a = b;
        b = t;
    }
    return a;
}

static unsigned long
mix (
    unsigned long h,
    long          v)
{
    h ^= (unsigned long)v + 0x9e3779b97f4a7c15UL + (h << 6) + (h >> 2);
    return h;
}

static long n_sector_compares = 0;

static void
sector_compute (
    SectorEntry *e)
{
    RadixRealis t;
    int         ki;

    if (e->ty == 0) {
        Fractio f = fractio_ex_s64(0);

        if (!fractio_ex_magnis(magnus_ex_s64(e->tx), magnus_ex_s64(e->td),
            piscina, &f)) {
            die("fractio");
        }
        t = radix_ex_fractione(f, piscina);
    } else {
        Magnus       c[3];
        Polynomium   q = polynomium_nullum();
        RadixRealis *r = NULL;
        i32          n = 0;

        /* (td t - tx)^2 - 2 ty^2: roots (tx +- |ty| sqrt2) / td, td > 0 */
        c[2] = magnus_multiplica(magnus_ex_s64(e->td), magnus_ex_s64(e->td),
            piscina);
        c[1] = magnus_multiplica(magnus_ex_s64(-2 * e->td), magnus_ex_s64(e->tx),
            piscina);
        c[0] = magnus_subtrahe(magnus_multiplica(magnus_ex_s64(e->tx),
            magnus_ex_s64(e->tx), piscina), magnus_multiplica(magnus_ex_s64(
            2 * e->ty), magnus_ex_s64(e->ty), piscina), piscina);
        if (!polynomium_ex_coefficientibus(c, 3, 0, piscina, &q)
            || !radices_reales(q, piscina, &r, &n) || n != 2) {
            die("T isolation");
        }
        t = r[e->ty > 0 ? 1 : 0];
    }
    for (ki = 0; ki < NK; ki++) {
        int m;

        e->sec[ki] = 0;
        e->tie[ki] = 0;
        for (m = 1; m <= bnd_n[ki]; m++) {
            s32 s = 0;

            n_sector_compares++;
            if (!radix_compara(t, bnd[ki][m], piscina, &s)) {
                die("radix_compara");
            }
            if (s < 0) break;
            e->sec[ki] = (signed char)m;
            if (s == 0) e->tie[ki] = 1;
        }
    }
}

static int
sector_lookup (
    long tx,
    long ty,
    long td)
{
    long          g = gcd_l(gcd_l(tx, ty), td);
    unsigned long h = 1469598103934665603UL;
    long          slot;

    if (g > 1) { tx /= g; ty /= g; td /= g; }
    if (td < 0) { tx = -tx; ty = -ty; td = -td; }
    h = mix(mix(mix(h, tx), ty), td);
    slot = (long)(h % (unsigned long)(2 * SEC_CAP));
    while (sec_hash[slot]) {
        SectorEntry *e = &sec_tab[sec_hash[slot] - 1];

        if (e->tx == tx && e->ty == ty && e->td == td) return sec_hash[slot] - 1;
        slot = (slot + 1) % (2 * SEC_CAP);
    }
    if (sec_n >= SEC_CAP) die("sector table full");
    sec_tab[sec_n].tx = tx;
    sec_tab[sec_n].ty = ty;
    sec_tab[sec_n].td = td;
    sector_compute(&sec_tab[sec_n]);
    sec_hash[slot] = ++sec_n;
    return sec_n - 1;
}

/* point memo per table: exact sum -> kind, sector entry, axis rule and
 * ties */
#define KIND_ZERO 0
#define KIND_REAL 1
#define KIND_NULL 2
#define KIND_GEN  3

typedef struct {
    QE  s;
    int kind;
    int sec_entry;
    int vor_rule;
    int vt_off;
    int n_vt;
} PointInfo;

#define PT_CAP 131072
static PointInfo pts[PT_CAP];
static int       pt_n = 0;
static int       pt_hash[2 * PT_CAP];
static int       pt_slot[PT_CAP];        /* hash slot of each point */
static int       vt_pool[PT_CAP * 4];
static int       vt_n = 0;
static const Table *pt_table = NULL;

/* clears only the slots in use (a full memset per random subset cost
 * more than the search) */
static void
memo_reset (
    const Table *t)
{
    int i;

    for (i = 0; i < pt_n; i++) pt_hash[pt_slot[i]] = 0;
    pt_n = 0;
    vt_n = 0;
    pt_table = t;
}

static unsigned long
qe_hash (
    const QE *s)
{
    unsigned long h = 1469598103934665603UL;

    h = mix(mix(h, s->a.p), s->a.q);
    h = mix(mix(h, s->b.p), s->b.q);
    h = mix(mix(h, s->c.p), s->c.q);
    h = mix(mix(h, s->d.p), s->d.q);
    return h;
}

static void
point_compute (
    PointInfo *pi)
{
    const Table *t = pt_table;
    const QE    *s = &pi->s;
    Z2           v2;
    int          j;

    v2 = z2_add(z2_add(z2_mul(s->b, s->b), z2_mul(s->c, s->c)),
        z2_mul(s->d, s->d));
    pi->sec_entry = -1;
    pi->vor_rule = t->nd;
    pi->vt_off = vt_n;
    pi->n_vt = 0;
    if (z2_zero(v2)) {
        pi->kind = z2_zero(s->a) ? KIND_ZERO : KIND_REAL;
        return;
    }
    if (z2_zero(s->a)) {
        pi->kind = KIND_NULL;
    } else {
        Z2 a2 = z2_mul(s->a, s->a);

        pi->kind = KIND_GEN;
        /* T = v2 / a2 = (P + Q r2)(R - S r2) / (R^2 - 2 S^2) */
        pi->sec_entry = sector_lookup(v2.p * a2.p - 2 * v2.q * a2.q,
            v2.q * a2.p - v2.p * a2.q, a2.p * a2.p - 2 * a2.q * a2.q);
    }
    /* axis: max (v.u_j)^2 / |u_j|^2; rule = first maximum */
    {
        Z2  best_num = z2(-1, 0);
        Z2  best_den = z2(1, 0);
        int best = -1;

        for (j = 0; j < t->nd; j++) {
            Z2  dot = z2_add(z2_add(z2_mul(s->b, t->x[j]),
                z2_mul(s->c, t->y[j])), z2_mul(s->d, t->z[j]));
            Z2  num = z2_mul(dot, dot);
            int c = best < 0 ? 1 : z2_sign(z2_sub(z2_mul(num, best_den),
                z2_mul(best_num, t->n2[j])));

            if (c > 0) {
                best = j;
                best_num = num;
                best_den = t->n2[j];
                vt_n = pi->vt_off;
                pi->n_vt = 0;
            }
            if (c >= 0) {
                if (vt_n >= PT_CAP * 4) die("tie pool full");
                vt_pool[vt_n++] = j;
                pi->n_vt++;
            }
        }
        pi->vor_rule = best;
    }
}

static int
point_id (
    const QE *s)
{
    long slot = (long)(qe_hash(s) % (unsigned long)(2 * PT_CAP));

    while (pt_hash[slot]) {
        if (qe_eq(&pts[pt_hash[slot] - 1].s, s)) return pt_hash[slot] - 1;
        slot = (slot + 1) % (2 * PT_CAP);
    }
    if (pt_n >= PT_CAP) die("point memo full");
    pts[pt_n].s = *s;
    point_compute(&pts[pt_n]);
    pt_slot[pt_n] = (int)slot;
    pt_hash[slot] = ++pt_n;
    return pt_n - 1;
}

/* cells of a point at k: rule cell, and all tied cells */
#define MAX_PTIES (2 * MAX_DIR)

static int
point_cells (
    const PointInfo *pi,
    int              k,
    int              ki,
    int             *ties,
    int             *n_ties)
{
    int nd = pt_table->nd;
    int n_vor = nd + 1;
    int secs[2];
    int n_sec = 1;
    int i;
    int j;

    *n_ties = 0;
    if (pi->kind == KIND_ZERO) {
        ties[(*n_ties)++] = (k - 1) * n_vor + nd;
        return ties[0];
    }
    if (pi->kind == KIND_REAL) {
        ties[(*n_ties)++] = nd;                  /* sector 0, no axis */
        return ties[0];
    }
    if (pi->kind == KIND_NULL) {
        secs[0] = k / 2;                         /* 180 degrees, floor */
        if (k % 2 == 0) { secs[1] = k / 2 - 1; n_sec = 2; }
    } else {
        const SectorEntry *e = &sec_tab[pi->sec_entry];

        secs[0] = e->sec[ki];
        if (e->tie[ki]) { secs[1] = e->sec[ki] - 1; n_sec = 2; }
    }
    for (i = 0; i < n_sec; i++) {
        for (j = 0; j < pi->n_vt; j++) {
            ties[(*n_ties)++] = secs[i] * n_vor + vt_pool[pi->vt_off + j];
        }
    }
    return secs[0] * n_vor + pi->vor_rule;
}

/* one FIXED tie rule (D122's four): at a sector tie the upper (the
 * exact floor) or the lower sector; at an axis tie the first or the
 * last maximal axis. (upper, first) is the exact rule. */
static int
point_cell_fixed (
    const PointInfo *pi,
    int              k,
    int              ki,
    int              upper,
    int              last)
{
    int nd = pt_table->nd;
    int n_vor = nd + 1;
    int sec;

    if (pi->kind == KIND_ZERO) return (k - 1) * n_vor + nd;
    if (pi->kind == KIND_REAL) return nd;
    if (pi->kind == KIND_NULL) {
        sec = k / 2;
        if (k % 2 == 0 && !upper) sec = k / 2 - 1;
    } else {
        const SectorEntry *e = &sec_tab[pi->sec_entry];

        sec = e->sec[ki];
        if (e->tie[ki] && !upper) sec--;
    }
    return sec * n_vor + vt_pool[pi->vt_off + (last ? pi->n_vt - 1 : 0)];
}

/* ================================================================
 * Separability of a weight set: rule / robust / possible
 * ================================================================ */

#define MODE_RULE     0
#define MODE_ROBUST   1
#define MODE_POSSIBLE 2
#define MODE_FIXED    3          /* + 2 * upper + last: 3..6 */
#define MAX_VEC 2187                             /* 3^7 */
#define MAX_CELLS 4096

static unsigned char seen0[MAX_CELLS];
static unsigned char seen1[MAX_CELLS];

static int sv_pid[MAX_VEC];
static int sv_par[MAX_VEC];

/* SAT over distinct points (D121's DPLL, clauses = points) */
static int  sat_var_of[MAX_CELLS];
static int  sat_cells[MAX_CELLS];
static int  sat_nv;
static int  sat_val[MAX_CELLS];
static int  sat_trail[MAX_CELLS];
static int  sat_nt;
static long sat_decisions;
static int  cl_n;
static int  cl_want[MAX_VEC];
static int  cl_off[MAX_VEC];
static int  cl_len[MAX_VEC];
static int  cl_lit[MAX_VEC * MAX_PTIES];
static long n_undecided = 0;

static int
sat_propagate (void)
{
    int changed = 1;

    while (changed) {
        int m;

        changed = 0;
        for (m = 0; m < cl_n; m++) {
            int want = cl_want[m];
            int free_var = -1;
            int n_free = 0;
            int sat = 0;
            int j;

            for (j = 0; j < cl_len[m] && !sat; j++) {
                int v = cl_lit[cl_off[m] + j];

                if (sat_val[v] == want) {
                    sat = 1;
                } else if (sat_val[v] < 0) {
                    if (free_var != v) n_free++;
                    free_var = v;
                }
            }
            if (sat) continue;
            if (n_free == 0) return 0;
            if (n_free == 1) {
                sat_val[free_var] = want;
                sat_trail[sat_nt++] = free_var;
                changed = 1;
            }
        }
    }
    return 1;
}

static int
sat_solve (void)
{
    int mark = sat_nt;
    int m;
    int pick = -1;
    int pick_val = 0;
    int r;

    if (!sat_propagate()) goto fail;
    for (m = 0; m < cl_n && pick < 0; m++) {
        int sat = 0;
        int cand = -1;
        int j;

        for (j = 0; j < cl_len[m]; j++) {
            int v = cl_lit[cl_off[m] + j];

            if (sat_val[v] == cl_want[m]) { sat = 1; break; }
            if (sat_val[v] < 0 && cand < 0) cand = v;
        }
        if (!sat) {
            pick = cand;
            pick_val = cl_want[m];
        }
    }
    if (pick < 0) return 1;
    if (++sat_decisions > 200000L) { r = -1; goto out; }
    sat_val[pick] = pick_val;
    sat_trail[sat_nt++] = pick;
    r = sat_solve();
    if (r != 0) goto out;
    sat_val[pick] = 1 - pick_val;
    r = sat_solve();
    if (r != 0) goto out;
fail:
    r = 0;
out:
    if (r != 1) {
        while (sat_nt > mark) sat_val[sat_trail[--sat_nt]] = -1;
    }
    return r;
}

static int pid_stamp[PT_CAP];
static int pid_par[PT_CAP];
static int stamp_now = 0;

static int
separable (
    const int *w,                /* indices into e_cat */
    int        n,
    int        k,
    int        mode)
{
    int ki = k_index(k);
    int n_vec = 1;
    int v;
    int i;
    int ok = 1;
    int touched[MAX_VEC * MAX_PTIES];
    int nt = 0;

    for (i = 0; i < n; i++) n_vec *= 3;
    if (k * (pt_table->nd + 1) > MAX_CELLS) die("cells");
    /* sums of the 3^n coefficient vectors; parity = #nonzero mod 2 */
    for (v = 0; v < n_vec; v++) {
        QE  s = qe_zero();
        int x = v;
        int par = 0;

        for (i = 0; i < n; i++) {
            int digit = x % 3;

            x /= 3;
            if (digit == 1) {
                s = qe_add(s, e_cat[w[i]]);
                par ^= 1;
            } else if (digit == 2) {
                s = qe_add(s, qe_neg(e_cat[w[i]]));
                par ^= 1;
            }
        }
        sv_pid[v] = point_id(&s);
        sv_par[v] = par;
    }
    if (mode != MODE_POSSIBLE) {
        for (v = 0; v < n_vec && ok; v++) {
            int ties[MAX_PTIES];
            int nt2 = 0;
            int rule = point_cells(&pts[sv_pid[v]], k, ki, ties, &nt2);
            int j;

            if (mode == MODE_RULE) {
                ties[0] = rule;
                nt2 = 1;
            } else if (mode >= MODE_FIXED) {
                ties[0] = point_cell_fixed(&pts[sv_pid[v]], k, ki,
                    (mode - MODE_FIXED) / 2, (mode - MODE_FIXED) % 2);
                nt2 = 1;
            }
            for (j = 0; j < nt2 && ok; j++) {
                int cell = ties[j];

                if (!seen0[cell] && !seen1[cell]) touched[nt++] = cell;
                if (sv_par[v]) {
                    seen1[cell] = 1;
                    ok = !seen0[cell];
                } else {
                    seen0[cell] = 1;
                    ok = !seen1[cell];
                }
            }
        }
        for (i = 0; i < nt; i++) {
            seen0[touched[i]] = 0;
            seen1[touched[i]] = 0;
        }
        return ok;
    }
    /* possible: a resolution is a function of the POINT; a point with
     * both parities fails under every rule */
    stamp_now++;
    cl_n = 0;
    sat_nv = 0;
    for (v = 0; v < n_vec; v++) {
        int pid = sv_pid[v];

        if (pid_stamp[pid] != stamp_now) {
            int ties[MAX_PTIES];
            int nt2 = 0;
            int j;

            pid_stamp[pid] = stamp_now;
            pid_par[pid] = sv_par[v];
            (void)point_cells(&pts[pid], k, ki, ties, &nt2);
            cl_want[cl_n] = sv_par[v];
            cl_off[cl_n] = cl_n == 0 ? 0 : cl_off[cl_n - 1] + cl_len[cl_n - 1];
            cl_len[cl_n] = nt2;
            for (j = 0; j < nt2; j++) {
                int cell = ties[j];

                if (sat_var_of[cell] == 0) {
                    sat_cells[sat_nv] = cell;
                    sat_val[sat_nv] = -1;
                    sat_var_of[cell] = ++sat_nv;
                }
                cl_lit[cl_off[cl_n] + j] = sat_var_of[cell] - 1;
            }
            cl_n++;
        } else if (pid_par[pid] != sv_par[v]) {
            ok = 0;
        }
    }
    if (ok) {
        int r;

        sat_nt = 0;
        sat_decisions = 0;
        r = sat_solve();
        if (r < 0) {
            n_undecided++;
            r = 0;
        }
        ok = r;
    }
    for (i = 0; i < sat_nv; i++) sat_var_of[sat_cells[i]] = 0;
    return ok;
}

/* ================================================================
 * Exhaustive capacity: level-wise over all weight sets (downward
 * closed per k), union over D84's k ladder
 * ================================================================ */

typedef struct {
    unsigned int *key;
    long          cap;
    long          n;
} USet;

static void
uset_init (
    USet *s,
    long  cap)
{
    s->cap = cap;
    s->n = 0;
    s->key = (unsigned int *)calloc((size_t)cap, sizeof(unsigned int));
    if (!s->key) die("calloc");
}

static void uset_free (USet *s) { free(s->key); s->key = NULL; }

static int
uset_has (
    const USet  *s,
    unsigned int x)
{
    long slot = (long)((x * 2654435761u) % (unsigned int)s->cap);

    while (s->key[slot]) {
        if (s->key[slot] == x) return 1;
        slot = (slot + 1) % s->cap;
    }
    return 0;
}

static int
uset_add (
    USet        *s,
    unsigned int x)
{
    long slot = (long)((x * 2654435761u) % (unsigned int)s->cap);

    while (s->key[slot]) {
        if (s->key[slot] == x) return 0;
        slot = (slot + 1) % s->cap;
    }
    if (2 * (s->n + 1) > s->cap) die("uset full");
    s->key[slot] = x;
    s->n++;
    return 1;
}

#define MAX_LEVEL 7

typedef struct {
    long count[MAX_LEVEL + 1];   /* weight sets passing at SOME ladder k */
    int  max_xor;                /* 2 x largest level >= 3 (D84: from XOR6) */
    long null_members[MAX_LEVEL + 1];
} Capacity;

static unsigned int *lvl_list[MAX_LEVEL + 1];
static long          lvl_n[MAX_LEVEL + 1];

/* sub: catalog indices of the subset (bit i = sub[i]); table set by
 * caller via memo_reset */
static void
exhaustive_capacity (
    const int *sub,
    int        n_sub,
    int        mode,
    const int *ks,
    int        n_ks,
    Capacity  *cap)
{
    USet union_lv[MAX_LEVEL + 1];
    int  kk;
    int  lv;

    memset(cap, 0, sizeof(*cap));
    for (lv = 1; lv <= MAX_LEVEL; lv++) {
        long choose = 1;
        int  t;

        for (t = 0; t < lv; t++) choose = choose * (n_sub - t) / (t + 1);
        uset_init(&union_lv[lv], 2 * choose + 16);
    }
    for (kk = 0; kk < n_ks; kk++) {
        int  k = ks[kk];
        USet cur;
        int  i;

        if (k * (pt_table->nd + 1) > MAX_ACT) continue;
        /* level 1 */
        lvl_n[1] = 0;
        lvl_list[1] = (unsigned int *)malloc(sizeof(unsigned int)
            * (size_t)n_sub);
        for (i = 0; i < n_sub; i++) {
            int w[1];

            w[0] = sub[i];
            if (separable(w, 1, k, mode)) {
                lvl_list[1][lvl_n[1]++] = 1u << i;
                (void)uset_add(&union_lv[1], 1u << i);
            }
        }
        for (lv = 2; lv <= MAX_LEVEL; lv++) {
            long cap_n = lvl_n[lv - 1] * (long)n_sub + 1;
            long p;

            uset_init(&cur, 2 * (lvl_n[lv - 1] > 0 ? lvl_n[lv - 1] : 1) + 16);
            for (p = 0; p < lvl_n[lv - 1]; p++) {
                (void)uset_add(&cur, lvl_list[lv - 1][p]);
            }
            lvl_list[lv] = (unsigned int *)malloc(sizeof(unsigned int)
                * (size_t)cap_n);
            lvl_n[lv] = 0;
            for (p = 0; p < lvl_n[lv - 1]; p++) {
                unsigned int base = lvl_list[lv - 1][p];
                int          hi = 31;
                int          j;

                while (hi >= 0 && !(base & (1u << hi))) hi--;
                for (j = hi + 1; j < n_sub; j++) {
                    unsigned int cand = base | (1u << j);
                    int          all = 1;
                    int          b;

                    for (b = 0; b < n_sub && all; b++) {
                        if ((base & (1u << b))
                            && !uset_has(&cur, cand & ~(1u << b))) {
                            all = 0;
                        }
                    }
                    if (all) {
                        int w[MAX_LEVEL];
                        int nw = 0;

                        for (b = 0; b < n_sub; b++) {
                            if (cand & (1u << b)) w[nw++] = sub[b];
                        }
                        if (separable(w, lv, k, mode)) {
                            lvl_list[lv][lvl_n[lv]++] = cand;
                            (void)uset_add(&union_lv[lv], cand);
                        }
                    }
                }
            }
            uset_free(&cur);
        }
        for (lv = 1; lv <= MAX_LEVEL; lv++) free(lvl_list[lv]);
    }
    for (lv = 1; lv <= MAX_LEVEL; lv++) {
        long slot;

        cap->count[lv] = union_lv[lv].n;
        if (lv >= 3 && cap->count[lv] > 0) cap->max_xor = 2 * lv;
        for (slot = 0; slot < union_lv[lv].cap; slot++) {
            unsigned int m = union_lv[lv].key[slot];
            int          b;

            if (!m) continue;
            for (b = 0; b < n_sub; b++) {
                if ((m & (1u << b)) && e_null[sub[b]]) cap->null_members[lv]++;
            }
        }
        uset_free(&union_lv[lv]);
    }
}

/* ================================================================
 * Part C: the replica's decisions against the exact cells
 * ================================================================ */

static Table cur_table;                  /* the replica's current table */
static long  ins_total = 0;
static long  ins_agree = 0;
static long  ins_tie_other = 0;          /* float != rule, float in ties */
static long  ins_bad = 0;                /* float outside the tie set */
static long  ins_null_sums = 0;
static long  ins_null_low = 0;           /* null sum, float sector k/2-1 */
static long  ins_null_on = 0;            /* null sum, float sector k/2 */
static long  ins_null_zero_low = 0;      /* ... low with float Re exactly 0 */
static long  ins_axis_ties = 0;
static long  ins_sector_ties = 0;

static void
exact_table_from_current (void)
{
    table_build(&cur_table, g_idx, g_cat_size);
    if (cur_table.nd != g_nd) die("float and exact axis counts differ");
    memo_reset(&cur_table);
}

static void
instrument_mask (
    const int *indices,
    int        n_weights,
    int        mask,
    int        k_sec,
    int        float_cell,
    double     float_re)
{
    QE  s = qe_zero();
    int i;
    int ties[MAX_PTIES];
    int nt = 0;
    int rule;
    int pid;
    int in_ties = 0;

    for (i = 0; i < n_weights; i++) {
        int plus = (mask >> (2 * i)) & 1;
        int minus = (mask >> (2 * i + 1)) & 1;

        if (plus && !minus) s = qe_add(s, e_cat[g_idx[indices[i]]]);
        if (minus && !plus) s = qe_add(s, qe_neg(e_cat[g_idx[indices[i]]]));
    }
    pid = point_id(&s);
    rule = point_cells(&pts[pid], k_sec, k_index(k_sec), ties, &nt);
    for (i = 0; i < nt; i++) {
        if (ties[i] == float_cell) in_ties = 1;
    }
    ins_total++;
    if (float_cell == rule) {
        ins_agree++;
    } else if (in_ties) {
        ins_tie_other++;
    } else {
        ins_bad++;
    }
    if (pts[pid].kind == KIND_NULL && k_sec % 2 == 0) {
        /* even k: the exact sector k/2 is a boundary, k/2 - 1 the other
         * side (odd k has no tie at 180 degrees) */
        int fsec = float_cell / (g_nd + 1);

        ins_null_sums++;
        if (fsec == k_sec / 2 - 1) ins_null_low++;
        if (fsec == k_sec / 2) ins_null_on++;
        if (fsec == k_sec / 2 - 1 && float_re == 0.0) ins_null_zero_low++;
    }
    if (pts[pid].n_vt > 1) ins_axis_ties++;
    if (pts[pid].kind == KIND_GEN && sec_tab[pts[pid].sec_entry].tie[
        k_index(k_sec)]) {
        ins_sector_ties++;
    }
}

/* exact rule cells inside D84's greedy search (same TestFn shape) */
static int
exact_rule_test (
    const int *indices,
    int        n_weights,
    int        k_sec)
{
    int w[MAX_TUPLE];
    int i;

    for (i = 0; i < n_weights; i++) w[i] = g_idx[indices[i]];
    return separable(w, n_weights, k_sec, MODE_RULE);
}

/* ================================================================
 * Small PRNG for the extended random sample (splitmix64)
 * ================================================================ */

static unsigned long sm_state = 124UL;

static unsigned long
sm_next (void)
{
    unsigned long z;

    sm_state += 0x9e3779b97f4a7c15UL;
    z = sm_state;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9UL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebUL;
    return z ^ (z >> 31);
}

static void
sm_subset (
    int *subset,
    int  size,
    int  total)
{
    int perm[MAX_QCAT];
    int i;

    for (i = 0; i < total; i++) perm[i] = i;
    for (i = 0; i < size; i++) {
        int j = i + (int)(sm_next() % (unsigned long)(total - i));
        int t = perm[i];

        perm[i] = perm[j];
        perm[j] = t;
    }
    for (i = 0; i < size; i++) subset[i] = perm[i];
    /* catalog order, as D84's subsets after load (the table order) */
    for (i = 1; i < size; i++) {
        int x = subset[i];
        int j = i - 1;

        while (j >= 0 && subset[j] > x) { subset[j + 1] = subset[j]; j--; }
        subset[j + 1] = x;
    }
}

/* ================================================================
 * Main
 * ================================================================ */

static const char *MODE_NAME[3] = { "rule", "robust", "possible" };

static void
print_capacity (
    const char     *title,
    const Capacity *c)
{
    printf("    %-34s max XOR%-2d | sets passing, n = 3..7: %ld %ld %ld %ld "
        "%ld\n", title, c->max_xor, c->count[3], c->count[4], c->count[5],
        c->count[6], c->count[7]);
}

int
main (void)
{
    int       i;
    int       n_null = 0;
    int       null_idx[MAX_QCAT];
    int       nonnull_idx[MAX_QCAT];
    int       all_idx[MAX_QCAT];
    int       n_nn = 0;
    char      msg[320];
    CapResult f_full;
    CapResult f_nn;
    CapResult f_no;
    int       rnd15[100][15];
    int       rnd9[100][9];
    int       f_r15[100];
    int       f_r9[100];
    int       sweep_k[5] = { 1, 2, 4, 6, 8 };
    int       f_sweep[3][5];
    Table     full_table;
    Capacity  cap_full[3];
    Capacity  cap_nn[3];
    Capacity  cap_no[3];
    int       fixed_max[4][3];
    Capacity  cap13_nn[2];
    Capacity  cap13_no[2];
    int       ex[3][3][5];
    double    rand_mean15_rule = 0.0;
    double    null_ratio[MAX_LEVEL + 1];

    piscina = piscina_generare_dynamicum("demo_124", 1 << 22);
    if (piscina == NULL) die("piscina");
    printf("KNOTAPEL DEMO 124: D84's Null States, Exactly\n");
    printf("=============================================\n");

    /* ---------- Part A: D84's float run, replicated ---------- */
    printf("\n=== Part A: D84's float search, replicated ===\n");
    f_init_su2(M_PI / 4.0);
    f_build_closure();
    e_build_closure();
    for (i = 0; i < f_cat_size; i++) {
        all_idx[i] = i;
        if (fabs(f_cat[i].a) < 1e-10) null_idx[n_null++] = i;
        else nonnull_idx[n_nn++] = i;
    }
    prepare_boundaries();
    instrument = 1;
    load_subset(all_idx, f_cat_size);
    greedy_capacity(f_cat_size, f_test_xor, &f_full);
    load_subset(nonnull_idx, n_nn);
    greedy_capacity(n_nn, f_test_xor, &f_nn);
    load_subset(null_idx, n_null);
    greedy_capacity(n_null, f_test_xor, &f_no);
    srand(42);
    {
        int t;
        int s15 = 0;
        int s9 = 0;
        int x15 = 0;
        int x9 = 0;

        for (t = 0; t < 100; t++) {
            CapResult r;

            random_subset(rnd15[t], 15, f_cat_size);
            load_subset(rnd15[t], 15);
            greedy_capacity(15, f_test_xor, &r);
            f_r15[t] = r.max_xor;
            s15 += r.max_xor;
            x15 += r.xor6;
        }
        for (t = 0; t < 100; t++) {
            CapResult r;

            random_subset(rnd9[t], 9, f_cat_size);
            load_subset(rnd9[t], 9);
            greedy_capacity(9, f_test_xor, &r);
            f_r9[t] = r.max_xor;
            s9 += r.max_xor;
            x9 += r.xor6;
        }
        printf("    full: max_xor %d (XOR6 %d, XOR8 %d, XOR10 %d)\n",
            f_full.max_xor, f_full.xor6, f_full.xor8, f_full.xor10);
        printf("    non-null (%d): max_xor %d (XOR6 %d, XOR8 %d)\n", n_nn,
            f_nn.max_xor, f_nn.xor6, f_nn.xor8);
        printf("    null-only (%d): max_xor %d (XOR6 %d)\n", n_null,
            f_no.max_xor, f_no.xor6);
        printf("    random-15 mean max_xor %.1f (XOR6 mean %.1f); random-9 "
            "mean %.1f (XOR6 mean %.1f)\n", (double)s15 / 100.0,
            (double)x15 / 100.0, (double)s9 / 100.0, (double)x9 / 100.0);
        /* D84 rebuilt from its source (384e44e3), run 2026-10-10 on this
         * machine - its printed values */
        {
            char m15[64];
            int  lo15 = 99;
            int  hi15 = 0;
            int  lo9 = 99;
            int  hi9 = 0;

            for (t = 0; t < 100; t++) {
                if (f_r15[t] < lo15) lo15 = f_r15[t];
                if (f_r15[t] > hi15) hi15 = f_r15[t];
                if (f_r9[t] < lo9) lo9 = f_r9[t];
                if (f_r9[t] > hi9) hi9 = f_r9[t];
            }
            /* D84 prints means to one decimal, min and max */
            sprintf(m15, "%.1f %.1f %.1f %.1f %d %d %d %d", (double)s15 / 100.0,
                (double)x15 / 100.0, (double)s9 / 100.0, (double)x9 / 100.0,
                lo15, hi15, lo9, hi9);
            printf("    (sums %d %d %d %d; %s)\n", s15, x15, s9, x9, m15);
            check("replica == D84 as run here: full 8 (32/32/0), non-null 8 "
                "(32/4), null-only 0; random-15 mean 7.7 (XOR6 32.0), min 6 "
                "max 8; random-9 mean 6.7 (XOR6 30.6), min 6 max 8",
                f_full.max_xor == 8 && f_full.xor6 == 32 && f_full.xor8 == 32
                && f_full.xor10 == 0 && f_nn.max_xor == 8 && f_nn.xor6 == 32
                && f_nn.xor8 == 4 && f_no.max_xor == 0
                && strcmp(m15, "7.7 32.0 6.7 30.6 6 8 6 8") == 0);
        }
    }
    {
        int  ki;
        int  ok;
        int *subs[3];
        int  sizes[3];
        int  sj;

        subs[0] = all_idx; subs[1] = nonnull_idx; subs[2] = null_idx;
        sizes[0] = f_cat_size; sizes[1] = n_nn; sizes[2] = n_null;
        for (sj = 0; sj < 3; sj++) {
            load_subset(subs[sj], sizes[sj]);
            f_build_dirs(sizes[sj]);
            for (ki = 0; ki < 5; ki++) {
                f_sweep[sj][ki] = f_count_xor6_at_k(sizes[sj], sweep_k[ki],
                    f_test_xor);
            }
        }
        ok = f_sweep[0][0] == 36 && f_sweep[0][1] == 137 && f_sweep[0][2] == 185
            && f_sweep[0][3] == 370 && f_sweep[0][4] == 459
            && f_sweep[1][0] == 0 && f_sweep[1][1] == 12 && f_sweep[1][2] == 22
            && f_sweep[1][3] == 46 && f_sweep[1][4] == 66;
        for (ki = 0; ki < 5; ki++) ok &= f_sweep[2][ki] == 0;
        printf("    k_sec sweep XOR6 (full / non-null / null-only):");
        for (ki = 0; ki < 5; ki++) {
            printf("  k=%d %d/%d/%d", sweep_k[ki], f_sweep[0][ki],
                f_sweep[1][ki], f_sweep[2][ki]);
        }
        printf("\n");
        check("replica k_sec sweep == D84 as run here (full 36..459, non-null "
            "0..66, null-only 0 at every k)", ok);
    }
    instrument = 0;

    /* ---------- Part B: the catalog, exactly ---------- */
    printf("\n=== Part B: the zeta_8 catalog in Z[sqrt2] ===\n");
    {
        int ok = e_cat_size == 24 && f_cat_size == 24;
        int nn = 0;
        int re_kinds[4] = { 0, 0, 0, 0 };
        int depth_null[4] = { 0, 0, 0, 0 };
        int depth_all[4] = { 0, 0, 0, 0 };

        for (i = 0; i < e_cat_size && ok; i++) {
            ok &= fabs(z2_val(e_cat[i].a) / 2 - f_cat[i].a) < 1e-12
                && fabs(z2_val(e_cat[i].b) / 2 - f_cat[i].b) < 1e-12
                && fabs(z2_val(e_cat[i].c) / 2 - f_cat[i].c) < 1e-12
                && fabs(z2_val(e_cat[i].d) / 2 - f_cat[i].d) < 1e-12
                && e_depth[i] == f_depth[i] && e_writhe[i] == f_writhe[i];
        }
        check("exact closure = D84's float catalog entry by entry (same BFS "
            "order, depths, writhes; 24 entries)", ok);
        for (i = 0; i < e_cat_size; i++) {
            long p = e_cat[i].a.p < 0 ? -e_cat[i].a.p : e_cat[i].a.p;
            long q = e_cat[i].a.q < 0 ? -e_cat[i].a.q : e_cat[i].a.q;

            nn += e_null[i];
            if (p == 0 && q == 0) re_kinds[0]++;
            else if (p == 1 && q == 0) re_kinds[1]++;
            else if (p == 0 && q == 1) re_kinds[2]++;
            else if (p == 2 && q == 0) re_kinds[3]++;
            if (e_depth[i] < 4) {
                depth_all[e_depth[i]]++;
                depth_null[e_depth[i]] += e_null[i];
            }
        }
        printf("    null: %d; |Re| = 0 / 1/2 / sqrt2/2 / 1: %d %d %d %d\n", nn,
            re_kinds[0], re_kinds[1], re_kinds[2], re_kinds[3]);
        printf("    depth 0..3: %d/%d %d/%d %d/%d %d/%d null\n", depth_null[0],
            depth_all[0], depth_null[1], depth_all[1], depth_null[2],
            depth_all[2], depth_null[3], depth_all[3]);
        check("census and gap exactly: 9 null; |Re| takes exactly 0, 1/2, "
            "sqrt2/2, 1 (9, 8, 6, 1 entries; gap 1/2); null by depth 0/5, "
            "2/10, 6/8, 1/1 (D84: 0%, 20%, 75%, 100%)",
            nn == 9 && re_kinds[0] == 9 && re_kinds[1] == 8
            && re_kinds[2] == 6 && re_kinds[3] == 1 && depth_null[0] == 0
            && depth_all[0] == 5 && depth_null[1] == 2 && depth_all[1] == 10
            && depth_null[2] == 6 && depth_all[2] == 8 && depth_null[3] == 1
            && depth_all[3] == 1);
        table_build(&full_table, all_idx, e_cat_size);
        {
            Table tn;
            Table to;
            int   both = 0;
            int   only_null = 0;
            int   only_nn = 0;
            int   j;

            table_build(&tn, nonnull_idx, n_nn);
            table_build(&to, null_idx, n_null);
            for (j = 0; j < full_table.nd; j++) {
                int in_n = 0;
                int in_o = 0;
                int t;

                for (t = 0; t < tn.nd; t++) {
                    in_n |= z2_eq(tn.x[t], full_table.x[j])
                        && z2_eq(tn.y[t], full_table.y[j])
                        && z2_eq(tn.z[t], full_table.z[j]);
                    in_n |= z2_eq(tn.x[t], z2_neg(full_table.x[j]))
                        && z2_eq(tn.y[t], z2_neg(full_table.y[j]))
                        && z2_eq(tn.z[t], z2_neg(full_table.z[j]));
                }
                for (t = 0; t < to.nd; t++) {
                    /* parallel test */
                    in_o |= z2_zero(z2_sub(z2_mul(to.y[t], full_table.z[j]),
                        z2_mul(to.z[t], full_table.y[j])))
                        && z2_zero(z2_sub(z2_mul(to.z[t], full_table.x[j]),
                        z2_mul(to.x[t], full_table.z[j])))
                        && z2_zero(z2_sub(z2_mul(to.x[t], full_table.y[j]),
                        z2_mul(to.y[t], full_table.x[j])));
                }
                if (!in_n) {
                    for (t = 0; t < tn.nd; t++) {
                        in_n |= z2_zero(z2_sub(z2_mul(tn.y[t], full_table.z[j]),
                            z2_mul(tn.z[t], full_table.y[j])))
                            && z2_zero(z2_sub(z2_mul(tn.z[t], full_table.x[j]),
                            z2_mul(tn.x[t], full_table.z[j])))
                            && z2_zero(z2_sub(z2_mul(tn.x[t], full_table.y[j]),
                            z2_mul(tn.y[t], full_table.x[j])));
                    }
                }
                if (in_n && in_o) both++;
                else if (in_o) only_null++;
                else only_nn++;
            }
            printf("    axes: %d total; null-only %d, non-null-only %d, both "
                "%d (non-null table %d, null table %d)\n", full_table.nd,
                only_null, only_nn, both, tn.nd, to.nd);
            check("axes exactly: 13 = 6 null-only + 4 non-null-only + 3 shared",
                full_table.nd == 13 && only_null == 6 && only_nn == 4
                && both == 3 && tn.nd == 7 && to.nd == 9);
        }
    }

    /* ---------- Part C: the float decisions against the exact cells */
    printf("\n=== Part C: every float cell decision of Part A, exactly ===\n");
    printf("    decisions %ld: agree %ld, differ inside the tie set %ld, "
        "differ OUTSIDE it %ld\n", ins_total, ins_agree, ins_tie_other,
        ins_bad);
    printf("    null sums at even k (exactly 90 deg, ON the k/2 boundary) %ld: "
        "float sector k/2-1 %ld, k/2 %ld (low with float Re exactly 0: %ld)\n",
        ins_null_sums, ins_null_low, ins_null_on, ins_null_zero_low);
    printf("    axis ties %ld, sector-boundary ties (non-null) %ld; exact "
        "sector compares %ld\n", ins_axis_ties, ins_sector_ties,
        n_sector_compares);
    check("every float decision equals the exact rule cell or lies in its "
        "exact tie set (no float error outside ties)", ins_bad == 0
        && ins_total > 0);
    sprintf(msg, "the float run's disagreements are ALL at exact ties: %ld of "
        "%ld decisions", ins_tie_other, ins_total);
    check(msg, ins_tie_other > 0);

    /* ---------- Part D: capacity exhaustively ---------- */
    printf("\n=== Part D: capacity over ALL weight sets (k ladder) ===\n");
    {
        int        mode;
        Table      tn;
        Table      to;
        CapResult  g_full;
        CapResult  g_nn;
        CapResult  g_no;

        table_build(&tn, nonnull_idx, n_nn);
        table_build(&to, null_idx, n_null);
        for (mode = 0; mode < 3; mode++) {
            memo_reset(&full_table);
            exhaustive_capacity(all_idx, e_cat_size, mode, K_LADDER,
                N_KLADDER, &cap_full[mode]);
            memo_reset(&tn);
            exhaustive_capacity(nonnull_idx, n_nn, mode, K_LADDER, N_KLADDER,
                &cap_nn[mode]);
            memo_reset(&to);
            exhaustive_capacity(null_idx, n_null, mode, K_LADDER, N_KLADDER,
                &cap_no[mode]);
            printf("  %s:\n", MODE_NAME[mode]);
            print_capacity("full (24, 13 axes)", &cap_full[mode]);
            print_capacity("non-null (15, own 7 axes)", &cap_nn[mode]);
            print_capacity("null-only (9, own 9 axes)", &cap_no[mode]);
        }
        /* D84's greedy search with exact rule cells */
        load_subset(all_idx, f_cat_size);
        greedy_capacity(f_cat_size, exact_rule_test, &g_full);
        load_subset(nonnull_idx, n_nn);
        greedy_capacity(n_nn, exact_rule_test, &g_nn);
        load_subset(null_idx, n_null);
        greedy_capacity(n_null, exact_rule_test, &g_no);
        printf("  D84's greedy search, exact rule cells: full %d, non-null %d, "
            "null-only %d\n", g_full.max_xor, g_nn.max_xor, g_no.max_xor);
        /* confound: every subset with the FULL 13-axis table */
        printf("  with the full 13-axis activation table:\n");
        for (mode = 0; mode < 3; mode++) {
            Capacity a;
            Capacity b;

            memo_reset(&full_table);
            exhaustive_capacity(nonnull_idx, n_nn, mode, K_LADDER, N_KLADDER,
                &a);
            exhaustive_capacity(null_idx, n_null, mode, K_LADDER, N_KLADDER,
                &b);
            if (mode < 2) {
                cap13_nn[mode] = a;
                cap13_no[mode] = b;
            }
            sprintf(msg, "non-null, 13 axes (%s)", MODE_NAME[mode]);
            print_capacity(msg, &a);
            sprintf(msg, "null-only, 13 axes (%s)", MODE_NAME[mode]);
            print_capacity(msg, &b);
        }
        /* four FIXED tie rules: does any consistent rule give the
         * atlas's numbers (full 8, non-null 6, null-only 6)? */
        printf("  four fixed tie rules (sector lower/upper x axis "
            "first/last):\n");
        {
            int fr;

            for (fr = 0; fr < 4; fr++) {
                Capacity a;
                Capacity b;
                Capacity c;

                memo_reset(&full_table);
                exhaustive_capacity(all_idx, e_cat_size, MODE_FIXED + fr,
                    K_LADDER, N_KLADDER, &a);
                memo_reset(&tn);
                exhaustive_capacity(nonnull_idx, n_nn, MODE_FIXED + fr,
                    K_LADDER, N_KLADDER, &b);
                memo_reset(&to);
                exhaustive_capacity(null_idx, n_null, MODE_FIXED + fr,
                    K_LADDER, N_KLADDER, &c);
                fixed_max[fr][0] = a.max_xor;
                fixed_max[fr][1] = b.max_xor;
                fixed_max[fr][2] = c.max_xor;
                printf("    sector %s, axis %s: full XOR%d, non-null XOR%d, "
                    "null-only XOR%d\n", fr / 2 ? "upper" : "lower",
                    fr % 2 ? "last" : "first", a.max_xor, b.max_xor,
                    c.max_xor);
            }
        }
        printf("  SAT budget exhausted (counted as fail): %ld\n", n_undecided);
        check("exact rule and robust: removing the 9 nulls leaves max XOR8 "
            "(full 8 = non-null 8) - capacity is NOT destroyed; null-only "
            "computes nothing (0)",
            cap_full[MODE_RULE].max_xor == 8 && cap_nn[MODE_RULE].max_xor == 8
            && cap_no[MODE_RULE].max_xor == 0
            && cap_full[MODE_ROBUST].max_xor == 8
            && cap_nn[MODE_ROBUST].max_xor == 8
            && cap_no[MODE_ROBUST].max_xor == 0);
        check("D84's greedy search with exact rule cells gives the same "
            "maxima (8, 8, 0): the search is not what differs",
            g_full.max_xor == 8 && g_nn.max_xor == 8 && g_no.max_xor == 0);
    }

    /* ---------- Part E: random controls ---------- */
    printf("\n=== Part E: random controls, exactly ===\n");
    {
        int  hist[2][2][16];             /* [size 15/9][rule/robust][xor] */
        int  ehist[2][2][16];
        int  t;
        int  sz;
        long sum[2][2];
        long esum[2][2];
        int  n_ext = 10000;

        memset(hist, 0, sizeof(hist));
        memset(ehist, 0, sizeof(ehist));
        memset(sum, 0, sizeof(sum));
        memset(esum, 0, sizeof(esum));
        for (sz = 0; sz < 2; sz++) {
            for (t = 0; t < 100; t++) {
                const int *sub = sz == 0 ? rnd15[t] : rnd9[t];
                int        n_sub = sz == 0 ? 15 : 9;
                Table      tt;
                int        mode;

                table_build(&tt, sub, n_sub);
                for (mode = 0; mode < 2; mode++) {
                    Capacity c;

                    memo_reset(&tt);
                    exhaustive_capacity(sub, n_sub, mode, K_LADDER, N_KLADDER,
                        &c);
                    hist[sz][mode][c.max_xor]++;
                    sum[sz][mode] += c.max_xor;
                }
            }
            for (t = 0; t < n_ext; t++) {
                int   sub[15];
                int   n_sub = sz == 0 ? 15 : 9;
                Table tt;
                int   mode;

                sm_subset(sub, n_sub, e_cat_size);
                table_build(&tt, sub, n_sub);
                for (mode = 0; mode < 2; mode++) {
                    Capacity c;

                    memo_reset(&tt);
                    exhaustive_capacity(sub, n_sub, mode, K_LADDER, N_KLADDER,
                        &c);
                    ehist[sz][mode][c.max_xor]++;
                    esum[sz][mode] += c.max_xor;
                }
            }
        }
        for (sz = 0; sz < 2; sz++) {
            int mode;

            for (mode = 0; mode < 2; mode++) {
                printf("    random-%d %-6s D84's 100: mean %.2f (max XOR 0/6/8/"
                    "10: %d %d %d %d) | 10,000 more: mean %.3f (%d %d %d %d)\n",
                    sz == 0 ? 15 : 9, MODE_NAME[mode],
                    (double)sum[sz][mode] / 100.0, hist[sz][mode][0],
                    hist[sz][mode][6], hist[sz][mode][8], hist[sz][mode][10],
                    (double)esum[sz][mode] / (double)n_ext,
                    ehist[sz][mode][0], ehist[sz][mode][6], ehist[sz][mode][8],
                    ehist[sz][mode][10]);
            }
        }
        {
            double p8 = (double)ehist[0][MODE_RULE][8] / (double)n_ext;

            sprintf(msg, "non-null (exact rule XOR8) is NOT worse than random "
                "15-subsets: %.1f%% of 10,000 reach XOR8, none more; the "
                "non-null subset sits at the top of the distribution",
                100.0 * p8);
            check(msg, cap_nn[MODE_RULE].max_xor >= 8
                && ehist[0][MODE_RULE][10] == 0 && ehist[0][MODE_RULE][12] == 0
                && ehist[0][MODE_RULE][14] == 0);
        }
        rand_mean15_rule = (double)esum[0][MODE_RULE] / (double)n_ext;
    }

    /* ---------- Part F: the k_sec sweep, null prevalence ---------- */
    printf("\n=== Part F: k_sec sweep (XOR6 sets) and nulls among ALL "
        "winners ===\n");
    {
        int          sj;
        int          ki;
        int          mode;
        const int   *subs[3];
        int          sizes[3];
        const char  *names[3];

        subs[0] = all_idx; subs[1] = nonnull_idx; subs[2] = null_idx;
        sizes[0] = e_cat_size; sizes[1] = n_nn; sizes[2] = n_null;
        names[0] = "full"; names[1] = "non-null"; names[2] = "null-only";
        for (sj = 0; sj < 3; sj++) {
            Table tt;

            table_build(&tt, subs[sj], sizes[sj]);
            memo_reset(&tt);
            for (mode = 0; mode < 3; mode++) {
                for (ki = 0; ki < 5; ki++) {
                    int a;
                    int b;
                    int c;
                    int n = 0;

                    for (a = 0; a < sizes[sj]; a++)
                    for (b = a + 1; b < sizes[sj]; b++)
                    for (c = b + 1; c < sizes[sj]; c++) {
                        int w[3];

                        w[0] = subs[sj][a]; w[1] = subs[sj][b];
                        w[2] = subs[sj][c];
                        n += separable(w, 3, sweep_k[ki], mode);
                    }
                    ex[sj][mode][ki] = n;
                }
            }
            printf("    %-9s k = 1/2/4/6/8 | float %d %d %d %d %d | rule %d %d %d "
                "%d %d | robust %d %d %d %d %d | possible %d %d %d %d %d\n",
                names[sj], f_sweep[sj][0], f_sweep[sj][1], f_sweep[sj][2],
                f_sweep[sj][3], f_sweep[sj][4], ex[sj][0][0], ex[sj][0][1],
                ex[sj][0][2], ex[sj][0][3], ex[sj][0][4], ex[sj][1][0],
                ex[sj][1][1], ex[sj][1][2], ex[sj][1][3], ex[sj][1][4],
                ex[sj][2][0], ex[sj][2][1], ex[sj][2][2], ex[sj][2][3],
                ex[sj][2][4]);
        }
        {
            int flat0 = 1;

            for (ki = 0; ki < 5; ki++) {
                flat0 &= ex[2][MODE_RULE][ki] == 0
                    && ex[2][MODE_ROBUST][ki] == 0;
            }
            check("null-only XOR6 under the exact rule and robustly: 0 at every "
                "k ('flat' only because it is empty); at k = 1 null-only does "
                "not beat non-null", flat0 && ex[2][MODE_RULE][0]
                <= ex[1][MODE_RULE][0]);
        }
        {
            int lv;

            printf("    nulls among ALL exact-rule winners (full catalog): ");
            for (lv = 3; lv <= 4; lv++) {
                long   cnt = cap_full[MODE_RULE].count[lv];
                double mean = cnt > 0 ? (double)cap_full[MODE_RULE].null_members[
                    lv] / (double)cnt : 0.0;

                printf("XOR%d %ld sets, mean nulls %.3f (random %.3f, ratio "
                    "%.2f)  ", 2 * lv, cnt, mean, (double)lv * 9.0 / 24.0,
                    mean / ((double)lv * 9.0 / 24.0));
                null_ratio[lv] = mean / ((double)lv * 9.0 / 24.0);
            }
            printf("\n");
        }
    }

    /* ---------- Part G: the claims ---------- */
    printf("\n=== Part G: D84's recorded claims, exactly ===\n");
    {
        int fr;
        int any = -1;

        for (fr = 0; fr < 4; fr++) {
            if (fixed_max[fr][0] == 8 && fixed_max[fr][1] == 6
                && fixed_max[fr][2] == 6) {
                any = fr;
            }
        }
        printf("    'removing nulls drops XOR8 to XOR6': exact rule %d -> %d, "
            "robust %d -> %d\n", cap_full[MODE_RULE].max_xor,
            cap_nn[MODE_RULE].max_xor, cap_full[MODE_ROBUST].max_xor,
            cap_nn[MODE_ROBUST].max_xor);
        printf("    'null-only reaches XOR6': exact rule %d, robust %d (with the "
            "full 13-axis table: see Part D)\n", cap_no[MODE_RULE].max_xor,
            cap_no[MODE_ROBUST].max_xor);
        printf("    'worse than random (mean 7.8)': non-null %d; random-15 exact "
            "rule mean %.3f\n", cap_nn[MODE_RULE].max_xor, rand_mean15_rule);
        printf("    'nulls specifically required for XOR8 (ratio 0.79)': ratio "
            "among all XOR8 winners %.2f\n", null_ratio[4]);
        printf("    a fixed tie rule giving the atlas's (8, 6, 6): %s\n",
            any >= 0 ? "yes" : "none of the four");
        check("no consistent tie rule reproduces the atlas: all four fixed "
            "rules give (full, non-null, null-only) = (8, 8, 0)", any < 0
            && fixed_max[0][1] == 8 && fixed_max[1][1] == 8
            && fixed_max[2][1] == 8 && fixed_max[3][1] == 8
            && fixed_max[0][2] == 0 && fixed_max[1][2] == 0
            && fixed_max[2][2] == 0 && fixed_max[3][2] == 0);
    }
    {
        /* oracle.py (Fractions over Q(sqrt2), own BFS, Decimal boundaries
         * where tan^2(m pi/k) is not in Q(sqrt2); no radices), run
         * 2026-10-10 - constants, not a live call. Sets passing per level
         * n = 3, 4 (5..7 are 0 in every row) */
        static const long OR_CAP[2][3][2] = {
            { { 1132, 402 }, { 304, 106 }, { 0, 0 } },
            { { 992, 218 }, { 280, 36 }, { 0, 0 } } };
        static const long OR_13[2][2][2] = {
            { { 328, 184 }, { 4, 0 } }, { { 328, 88 }, { 4, 0 } } };
        static const int OR_SWEEP[2][3][5] = {
            { { 36, 168, 180, 324, 484 }, { 0, 12, 14, 70, 46 },
              { 0, 0, 0, 0, 0 } },
            { { 36, 36, 36, 68, 176 }, { 0, 0, 0, 12, 44 },
              { 0, 0, 0, 0, 0 } } };
        int ok = 1;
        int mode;
        int j;

        for (mode = 0; mode < 2; mode++) {
            const Capacity *cs[3];
            int             t;

            cs[0] = &cap_full[mode]; cs[1] = &cap_nn[mode];
            cs[2] = &cap_no[mode];
            for (t = 0; t < 3; t++) {
                ok &= cs[t]->count[3] == OR_CAP[mode][t][0]
                    && cs[t]->count[4] == OR_CAP[mode][t][1];
                for (j = 5; j <= MAX_LEVEL; j++) ok &= cs[t]->count[j] == 0;
                for (j = 0; j < 5; j++) {
                    ok &= ex[t][mode][j] == OR_SWEEP[mode][t][j];
                }
            }
            ok &= cap13_nn[mode].count[3] == OR_13[mode][0][0]
                && cap13_nn[mode].count[4] == OR_13[mode][0][1]
                && cap13_no[mode].count[3] == OR_13[mode][1][0]
                && cap13_no[mode].count[4] == OR_13[mode][1][1];
        }
        ok &= cap_full[MODE_RULE].null_members[3] == 1056
            && cap_full[MODE_RULE].null_members[4] == 300;
        check("oracle.py == the demo: rule and robust set counts per level "
            "(full, non-null, null-only, and with the 13-axis table), the "
            "k_sec sweep, null members of all winners, the four fixed rules",
            ok);
    }

    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
