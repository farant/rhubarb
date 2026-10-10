/*
 * KNOTAPEL DEMO 122: Cell B's Exact Phase Diagram
 * ================================================================
 *
 * D97 asked why D96's Cell B (the six 45-degree elements of zeta_8,
 * rotations about three orthogonal axes) computes XOR for every set,
 * and answered with an angle sweep: the same three axes at half-angles
 * 10, 15, ..., 90 degrees, capacity at N = 3..6. It reported a plateau
 * from 25 to 75 degrees, "45 is the midpoint", an "isolated resonance"
 * at 35 degrees (N = 5), and "total collapse" at 90.
 *
 * The swept family is exact in a way the sweep does not use. Each
 * element is (cos t, +-sin t d_j); a signed sum is (n0 cos t, sin t
 * sum_j n_j d_j) with SMALL INTEGERS n0, n_j fixed by the mask. So:
 *   - the axis cell does not depend on t: largest |n_j|, ties exactly
 *     when maxima are equal, the rule taking the first in D97's order;
 *   - n0 = 0, n != 0: angle exactly 180 degrees - a tie at m = k/2;
 *   - n = 0: real sum (sector 0 or k-1), or zero;
 *   - otherwise the sector depends on u = tan^2 t only, and changes
 *     exactly at u* = n0^2 tan^2(j pi/24) / r, r = |n|^2, j = 1..11
 *     (every sector boundary of k = 6, 12, 24 is a multiple of pi/24).
 *
 * This demo sorts every breakpoint exactly (surdus: tan^2(j pi/24) =
 * (4 - 4 cos(j pi/12)) / (4 + 4 cos(j pi/12)) in Z[sqrt2, sqrt3]),
 * then evaluates rule, robust and possible on every open interval and
 * at every breakpoint by RANK - u's position among the sorted
 * breakpoints answers every sector question. The result is Cell B's
 * capacity for every half-angle in (0, 90] degrees, with exact
 * endpoints; D97's 21 angles are placed into it (multiples of 15
 * degrees exactly, the others by a certified float gap) and its
 * float sweep is reproduced first.
 *
 * Mode: DEMO122_CELER = the replica and N = 3 only (plants).
 *
 * House library: surdus.h. Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_122_cell_b_phase_diagram/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 */

#include "surdus.h"
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


static int quick = 0;

/* ================================================================
 * D95/D96's group (D121's port of D95's integer code)
 * ================================================================ */

typedef struct { int a; int b; } Zr2;

static Zr2 zr2_make (int a, int b) { Zr2 r; r.a = a; r.b = b; return r; }
static Zr2 zr2_add (Zr2 x, Zr2 y)
{ Zr2 r; r.a = x.a + y.a; r.b = x.b + y.b; return r; }
static Zr2 zr2_sub (Zr2 x, Zr2 y)
{ Zr2 r; r.a = x.a - y.a; r.b = x.b - y.b; return r; }
static Zr2 zr2_neg (Zr2 x) { Zr2 r; r.a = -x.a; r.b = -x.b; return r; }
static Zr2 zr2_mul (Zr2 x, Zr2 y)
{
    Zr2 r;
    r.a = x.a * y.a + 2 * x.b * y.b;
    r.b = x.a * y.b + x.b * y.a;
    return r;
}
static int zr2_eq (Zr2 x, Zr2 y) { return x.a == y.a && x.b == y.b; }
static Zr2 zr2_div2 (Zr2 x) { Zr2 r; r.a = x.a / 2; r.b = x.b / 2; return r; }

typedef struct { Zr2 a, b, c, d; } QZ8;

static QZ8 qz8_make (Zr2 a, Zr2 b, Zr2 c, Zr2 d)
{ QZ8 r; r.a = a; r.b = b; r.c = c; r.d = d; return r; }
static int qz8_eq (const QZ8 *p, const QZ8 *q)
{
    return zr2_eq(p->a, q->a) && zr2_eq(p->b, q->b)
        && zr2_eq(p->c, q->c) && zr2_eq(p->d, q->d);
}
static QZ8 qz8_neg (const QZ8 *q)
{ return qz8_make(zr2_neg(q->a), zr2_neg(q->b), zr2_neg(q->c), zr2_neg(q->d)); }
static QZ8 qz8_conj (const QZ8 *q)
{ return qz8_make(q->a, zr2_neg(q->b), zr2_neg(q->c), zr2_neg(q->d)); }

static QZ8
qz8_mul (
    const QZ8 *p,
    const QZ8 *q)
{
    QZ8 r;
    Zr2 t;

    t = zr2_mul(p->a, q->a);
    t = zr2_sub(t, zr2_mul(p->b, q->b));
    t = zr2_sub(t, zr2_mul(p->c, q->c));
    t = zr2_sub(t, zr2_mul(p->d, q->d));
    r.a = zr2_div2(t);
    t = zr2_mul(p->a, q->b);
    t = zr2_add(t, zr2_mul(p->b, q->a));
    t = zr2_add(t, zr2_mul(p->c, q->d));
    t = zr2_sub(t, zr2_mul(p->d, q->c));
    r.b = zr2_div2(t);
    t = zr2_mul(p->a, q->c);
    t = zr2_sub(t, zr2_mul(p->b, q->d));
    t = zr2_add(t, zr2_mul(p->c, q->a));
    t = zr2_add(t, zr2_mul(p->d, q->b));
    r.c = zr2_div2(t);
    t = zr2_mul(p->a, q->d);
    t = zr2_add(t, zr2_mul(p->b, q->c));
    t = zr2_sub(t, zr2_mul(p->c, q->b));
    t = zr2_add(t, zr2_mul(p->d, q->a));
    r.d = zr2_div2(t);
    return r;
}

#define MAX_GRP 64

static QZ8 g_z8[MAX_GRP];
static int g_z8_size = 0;
static int g_level[MAX_GRP];      /* derived level 0..3 */
static int g_null[MAX_GRP];       /* Re = 0 */
static int g_cell[MAX_GRP];       /* A..E = 0..4 */
static int g_in_g1[MAX_GRP];      /* commutator subgroup */
static int g_single_comm[MAX_GRP];

static int
find_z8 (
    const QZ8 *q)
{
    int i;
    QZ8 nq = qz8_neg(q);

    for (i = 0; i < g_z8_size; i++) {
        if (qz8_eq(q, &g_z8[i]) || qz8_eq(&nq, &g_z8[i])) {
            return i;
        }
    }
    return -1;
}

static void
build_z8_d95 (void)
{
    QZ8 gens[4];
    int prev;
    int i;
    int gi;
    int rd;

    gens[0] = qz8_make(zr2_make(0,1), zr2_make(0,1), zr2_make(0,0),
        zr2_make(0,0));
    gens[1] = qz8_conj(&gens[0]);
    gens[2] = qz8_make(zr2_make(0,1), zr2_make(0,0), zr2_make(0,0),
        zr2_make(0,-1));
    gens[3] = qz8_conj(&gens[2]);
    g_z8[0] = qz8_make(zr2_make(2,0), zr2_make(0,0), zr2_make(0,0),
        zr2_make(0,0));
    g_z8_size = 1;
    for (gi = 0; gi < 4; gi++) {
        if (find_z8(&gens[gi]) < 0 && g_z8_size < MAX_GRP) {
            g_z8[g_z8_size++] = gens[gi];
        }
    }
    rd = 1;
    do {
        prev = g_z8_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                QZ8 prod = qz8_mul(&g_z8[i], &gens[gi]);

                if (find_z8(&prod) < 0 && g_z8_size < MAX_GRP) {
                    g_z8[g_z8_size++] = prod;
                }
            }
        }
        rd++;
    } while (g_z8_size > prev && rd < 20);
}

static QZ8
z8_commutator (
    const QZ8 *a,
    const QZ8 *b)
{
    QZ8 ai = qz8_conj(a);
    QZ8 bi = qz8_conj(b);
    QZ8 ab = qz8_mul(a, b);
    QZ8 abi = qz8_mul(&ab, &ai);

    return qz8_mul(&abi, &bi);
}

static int
close_subgroup (
    int *in_set)
{
    int changed;
    int i;
    int j;
    int count = 0;

    in_set[0] = 1;
    do {
        changed = 0;
        for (i = 0; i < g_z8_size; i++) {
            QZ8 inv;
            int k;

            if (!in_set[i]) continue;
            inv = qz8_conj(&g_z8[i]);
            k = find_z8(&inv);
            if (k >= 0 && !in_set[k]) { in_set[k] = 1; changed = 1; }
            for (j = 0; j < g_z8_size; j++) {
                QZ8 prod;

                if (!in_set[j]) continue;
                prod = qz8_mul(&g_z8[i], &g_z8[j]);
                k = find_z8(&prod);
                if (k >= 0 && !in_set[k]) { in_set[k] = 1; changed = 1; }
            }
        }
    } while (changed);
    for (i = 0; i < g_z8_size; i++) {
        count += in_set[i];
    }
    return count;
}

/* D95 Phase 1-2 / D96 cells; sizes[] = derived series */
static int
derive_structure (
    int *sizes)
{
    int level_set[MAX_GRP];
    int comm_set[MAX_GRP];
    int level;
    int n_levels = 0;
    int i;
    int j;

    memset(g_single_comm, 0, sizeof(g_single_comm));
    for (i = 0; i < g_z8_size; i++) {
        for (j = 0; j < g_z8_size; j++) {
            QZ8 cm = z8_commutator(&g_z8[i], &g_z8[j]);
            int k = find_z8(&cm);

            if (k >= 0) g_single_comm[k] = 1;
        }
    }
    memcpy(g_in_g1, g_single_comm, sizeof(g_in_g1));
    (void)close_subgroup(g_in_g1);
    for (i = 0; i < g_z8_size; i++) {
        level_set[i] = 1;
        g_level[i] = 0;
    }
    sizes[0] = g_z8_size;
    for (level = 1; level < 10; level++) {
        int cur;

        memset(comm_set, 0, sizeof(comm_set));
        for (i = 0; i < g_z8_size; i++) {
            if (!level_set[i]) continue;
            for (j = 0; j < g_z8_size; j++) {
                QZ8 cm;
                int k;

                if (!level_set[j]) continue;
                cm = z8_commutator(&g_z8[i], &g_z8[j]);
                k = find_z8(&cm);
                if (k >= 0) comm_set[k] = 1;
            }
        }
        cur = close_subgroup(comm_set);
        sizes[level] = cur;
        for (i = 0; i < g_z8_size; i++) {
            if (level_set[i] && !comm_set[i]) g_level[i] = level - 1;
        }
        memcpy(level_set, comm_set, sizeof(level_set));
        n_levels = level;
        if (cur <= 1) {
            for (i = 0; i < g_z8_size; i++) {
                if (comm_set[i]) g_level[i] = level;
            }
            break;
        }
    }
    for (i = 0; i < g_z8_size; i++) {
        g_null[i] = g_z8[i].a.a == 0 && g_z8[i].a.b == 0;
        g_cell[i] = g_level[i] == 0 ? (g_null[i] ? 0 : 1)
            : g_level[i] == 1 ? 2 : g_level[i] == 2 ? 3 : 4;
    }
    return n_levels;
}


/* ================================================================
 * D97's float sweep, verbatim up to names (latina.h: no 'si')
 * ================================================================ */

typedef struct { double a, b, c, d; } Quat;

#define MAX_DIR 64
#define MAX_ACT 65536

static double g_dir[MAX_DIR][3];
static int    g_nd = 0;
static Quat   g_cat[128];
static int    g_cat_size = 0;
static int    cell_class0[MAX_ACT];
static int    cell_class1[MAX_ACT];
static int    touched_cells[MAX_ACT];
static Quat   cellb_float[8];
static int    cellb_count = 0;
static double cellb_dirs[3][3];
static int    cellb_n_dirs = 0;

static Quat
qz8_to_quat (
    const QZ8 *q)
{
    static const double SQRT2 = 1.4142135623730950488;
    Quat r;

    r.a = ((double)q->a.a + (double)q->a.b * SQRT2) / 2.0;
    r.b = ((double)q->b.a + (double)q->b.b * SQRT2) / 2.0;
    r.c = ((double)q->c.a + (double)q->c.b * SQRT2) / 2.0;
    r.d = ((double)q->d.a + (double)q->d.b * SQRT2) / 2.0;
    return r;
}

/* D97 extract_cell_b + Phase 1 direction extraction */
static void
d97_cell_b (void)
{
    int i;
    int j;

    cellb_count = 0;
    for (i = 0; i < g_z8_size; i++) {
        if (g_level[i] == 0 && !g_null[i]) {
            cellb_float[cellb_count++] = qz8_to_quat(&g_z8[i]);
        }
    }
    cellb_n_dirs = 0;
    for (i = 0; i < cellb_count; i++) {
        double qa = cellb_float[i].a, qb = cellb_float[i].b;
        double qc = cellb_float[i].c, qd = cellb_float[i].d;
        double nv, ax, ay, az;
        int    found = 0;

        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        if (nv < 1e-12) continue;
        ax = qb/nv; ay = qc/nv; az = qd/nv;
        for (j = 0; j < cellb_n_dirs; j++) {
            double d1 = fabs(cellb_dirs[j][0]-ax) + fabs(cellb_dirs[j][1]-ay)
                + fabs(cellb_dirs[j][2]-az);
            double d2 = fabs(cellb_dirs[j][0]+ax) + fabs(cellb_dirs[j][1]+ay)
                + fabs(cellb_dirs[j][2]+az);

            if (d1 < 1e-8 || d2 < 1e-8) { found = 1; break; }
        }
        if (!found && cellb_n_dirs < 3) {
            cellb_dirs[cellb_n_dirs][0] = ax;
            cellb_dirs[cellb_n_dirs][1] = ay;
            cellb_dirs[cellb_n_dirs][2] = az;
            cellb_n_dirs++;
        }
    }
}

static void
d97_build_dirs (void)
{
    int i;
    int j;

    g_nd = 0;
    for (i = 0; i < g_cat_size; i++) {
        double qa = g_cat[i].a, qb = g_cat[i].b;
        double qc = g_cat[i].c, qd = g_cat[i].d;
        double nv, ax, ay, az;
        int    found = 0;

        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        if (nv < 1e-12) continue;
        ax = qb/nv; ay = qc/nv; az = qd/nv;
        for (j = 0; j < g_nd; j++) {
            double d1 = fabs(g_dir[j][0]-ax) + fabs(g_dir[j][1]-ay)
                + fabs(g_dir[j][2]-az);
            double d2 = fabs(g_dir[j][0]+ax) + fabs(g_dir[j][1]+ay)
                + fabs(g_dir[j][2]+az);

            if (d1 < 1e-8 || d2 < 1e-8) { found = 1; break; }
        }
        if (!found && g_nd < MAX_DIR) {
            g_dir[g_nd][0] = ax; g_dir[g_nd][1] = ay; g_dir[g_nd][2] = az;
            g_nd++;
        }
    }
}

static int
d97_vor_cell (
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
d97_phase_cell (
    double sa,
    double sb,
    double sc,
    double sd,
    int    k_sec)
{
    double n2 = sa*sa + sb*sb + sc*sc + sd*sd;
    double nm, qa, half_ang, ang, rv;
    int    sec, vor, n_vor;

    n_vor = g_nd + 1;
    if (n2 < 1e-24) return (k_sec - 1) * n_vor + g_nd;
    nm = sqrt(n2);
    qa = sa / nm;
    if (qa > 1.0) qa = 1.0;
    if (qa < -1.0) qa = -1.0;
    half_ang = acos(qa);
    ang = 2.0 * half_ang * 180.0 / M_PI;
    sec = (int)(ang * (double)k_sec / 360.0);
    if (sec >= k_sec) sec = k_sec - 1;
    if (sec < 0) sec = 0;
    rv = sqrt(sb*sb + sc*sc + sd*sd);
    if (rv / nm < 1e-12) { vor = g_nd; }
    else { vor = d97_vor_cell(sb / rv, sc / rv, sd / rv); }
    return sec * n_vor + vor;
}

static int
popcount (
    int x)
{
    int c = 0;

    while (x) { c += x & 1; x >>= 1; }
    return c;
}

static int
d97_test_xor (
    const int *indices,
    int        n_weights,
    int        k_sec)
{
    int n_masks = 1 << n_weights;
    int n_touched = 0;
    int mask;
    int i;
    int result = 1;

    for (mask = 0; mask < n_masks && result; mask++) {
        double sa = 0, sb = 0, sc = 0, sd = 0;
        int    cls;
        int    cell;

        for (i = 0; i < n_weights; i++) {
            const Quat *q = &g_cat[indices[i]];
            double sign = ((mask >> i) & 1) ? 1.0 : -1.0;

            sa += sign * q->a; sb += sign * q->b;
            sc += sign * q->c; sd += sign * q->d;
        }
        cell = d97_phase_cell(sa, sb, sc, sd, k_sec);
        cls = popcount(mask) & 1;
        if (cell_class0[cell] == 0 && cell_class1[cell] == 0) {
            touched_cells[n_touched++] = cell;
        }
        if (cls == 0) {
            cell_class0[cell]++;
            if (cell_class1[cell] > 0) result = 0;
        } else {
            cell_class1[cell]++;
            if (cell_class0[cell] > 0) result = 0;
        }
    }
    for (i = 0; i < n_touched; i++) {
        cell_class0[touched_cells[i]] = 0;
        cell_class1[touched_cells[i]] = 0;
    }
    return result;
}

static int
next_combo (
    int *combo,
    int  n,
    int  bf)
{
    int i = n - 1;

    while (i >= 0) {
        combo[i]++;
        if (combo[i] <= bf - n + i) {
            int j;

            for (j = i + 1; j < n; j++) combo[j] = combo[j - 1] + 1;
            return 1;
        }
        i--;
    }
    return 0;
}

static const int KS[3] = { 6, 12, 24 };

/* D97 Phase 3 at one half-angle (degrees): XOR count for N */
static int
d97_sweep_count (
    double degrees,
    int    n_w)
{
    double ha = degrees * M_PI / 180.0;
    double co = cos(ha);
    double sn = sin(ha);
    int    combo[8];
    int    count = 0;
    int    di;
    int    i;

    g_cat_size = 0;
    for (di = 0; di < cellb_n_dirs; di++) {
        Quat q1, q2;

        q1.a = co;
        q1.b = sn * cellb_dirs[di][0];
        q1.c = sn * cellb_dirs[di][1];
        q1.d = sn * cellb_dirs[di][2];
        q2.a = co;
        q2.b = -sn * cellb_dirs[di][0];
        q2.c = -sn * cellb_dirs[di][1];
        q2.d = -sn * cellb_dirs[di][2];
        g_cat[g_cat_size++] = q1;
        g_cat[g_cat_size++] = q2;
    }
    d97_build_dirs();
    for (i = 0; i < n_w; i++) combo[i] = i;
    do {
        int ki;

        for (ki = 0; ki < 3; ki++) {
            if (d97_test_xor(combo, n_w, KS[ki])) { count++; break; }
        }
    } while (next_combo(combo, n_w, g_cat_size));
    return count;
}

/* ================================================================
 * Verdicts (D119-D121's engine: rule, robust, possible by DPLL)
 * ================================================================ */

#define MAX_TIES 64
#define MAX_MASKS 256
#define MAX_CELLS 65536

typedef struct {
    int float_cell;
    int exact_cell;
    int n_ties;
    int ties[MAX_TIES];
} MaskCell;

static unsigned char seen0[MAX_CELLS];
static unsigned char seen1[MAX_CELLS];

typedef struct {
    int v_float;
    int v_rule;
    int v_robust;
    int v_possible;
    int undecided;
    int tied;
    int clash;
} Verdict;

static int
labels_pass (
    const MaskCell *mc,
    int             n_masks,
    const int      *tt,
    int             use_float)
{
    int touched[MAX_MASKS];
    int nt = 0;
    int m;
    int ok = 1;

    for (m = 0; m < n_masks && ok; m++) {
        int cell = use_float ? mc[m].float_cell : mc[m].exact_cell;

        if (!seen0[cell] && !seen1[cell]) {
            touched[nt++] = cell;
        }
        if (tt[m]) {
            seen1[cell] = 1;
            ok = !seen0[cell];
        } else {
            seen0[cell] = 1;
            ok = !seen1[cell];
        }
    }
    for (m = 0; m < nt; m++) {
        seen0[touched[m]] = 0;
        seen1[touched[m]] = 0;
    }
    return ok;
}

static int
robust_pass (
    const MaskCell *mc,
    int             n_masks,
    const int      *tt)
{
    static int touched[MAX_MASKS * MAX_TIES];
    int nt = 0;
    int m;
    int k;
    int ok = 1;

    for (m = 0; m < n_masks && ok; m++) {
        for (k = 0; k < mc[m].n_ties && ok; k++) {
            int cell = mc[m].ties[k];

            if (!seen0[cell] && !seen1[cell]) {
                touched[nt++] = cell;
            }
            if (tt[m]) {
                seen1[cell] = 1;
                ok = !seen0[cell];
            } else {
                seen0[cell] = 1;
                ok = !seen1[cell];
            }
        }
    }
    for (m = 0; m < nt; m++) {
        seen0[touched[m]] = 0;
        seen1[touched[m]] = 0;
    }
    return ok;
}

/* some resolution passes <=> SAT: variable x_c = label of cell c; a
 * mask with truth value 1 needs (OR of x_c over its tied cells), one
 * with value 0 needs (OR of NOT x_c). DPLL with unit propagation;
 * decision budget -> undecided (counted, never guessed). */
#define MAX_VARS (MAX_MASKS * MAX_TIES)

static int  sat_var_of[MAX_CELLS];      /* cell -> var + 1 (0 = none) */
static int  sat_cells[MAX_VARS];
static int  sat_nv;
static int  sat_val[MAX_VARS];          /* -1 unassigned, 0, 1 */
static int  sat_trail[MAX_VARS];
static int  sat_nt;
static long sat_decisions;
static const MaskCell *sat_mc;
static const int      *sat_tt;
static int             sat_n;

/* returns 0 on conflict */
static int
sat_propagate (void)
{
    int changed = 1;

    while (changed) {
        int m;

        changed = 0;
        for (m = 0; m < sat_n; m++) {
            int want = sat_tt[m] ? 1 : 0;
            int free_var = -1;
            int n_free = 0;
            int sat = 0;
            int j;

            for (j = 0; j < sat_mc[m].n_ties && !sat; j++) {
                int v = sat_var_of[sat_mc[m].ties[j]] - 1;

                if (sat_val[v] == want) {
                    sat = 1;
                } else if (sat_val[v] < 0) {
                    if (free_var != v) {
                        n_free++;
                    }
                    free_var = v;
                }
            }
            if (sat) {
                continue;
            }
            if (n_free == 0) {
                return 0;
            }
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

    if (!sat_propagate()) {
        goto fail;
    }
    /* first unsatisfied clause: branch on its first free variable */
    for (m = 0; m < sat_n && pick < 0; m++) {
        int want = sat_tt[m] ? 1 : 0;
        int sat = 0;
        int j;
        int cand = -1;

        for (j = 0; j < sat_mc[m].n_ties; j++) {
            int v = sat_var_of[sat_mc[m].ties[j]] - 1;

            if (sat_val[v] == want) {
                sat = 1;
                break;
            }
            if (sat_val[v] < 0 && cand < 0) {
                cand = v;
            }
        }
        if (!sat) {
            pick = cand;
            pick_val = want;
        }
    }
    if (pick < 0) {
        return 1;                    /* every clause satisfied */
    }
    if (++sat_decisions > 200000L) {
        r = -1;
        goto out;
    }
    sat_val[pick] = pick_val;
    sat_trail[sat_nt++] = pick;
    r = sat_solve();
    if (r != 0) {
        goto out;
    }
    sat_val[pick] = 1 - pick_val;
    r = sat_solve();
    if (r != 0) {
        goto out;
    }
fail:
    r = 0;
out:
    if (r != 1) {
        while (sat_nt > mark) {
            sat_val[sat_trail[--sat_nt]] = -1;
        }
    }
    return r;
}

static int
possible_pass (
    const MaskCell *mc,
    int             n_masks,
    const int      *tt,
    int            *undecided)
{
    int m;
    int j;
    int r;

    sat_nv = 0;
    for (m = 0; m < n_masks; m++) {
        for (j = 0; j < mc[m].n_ties; j++) {
            int cell = mc[m].ties[j];

            if (sat_var_of[cell] == 0) {
                sat_cells[sat_nv] = cell;
                sat_val[sat_nv] = -1;
                sat_var_of[cell] = ++sat_nv;
            }
        }
    }
    sat_mc = mc;
    sat_tt = tt;
    sat_n = n_masks;
    sat_nt = 0;
    sat_decisions = 0;
    r = sat_solve();
    for (j = 0; j < sat_nv; j++) {
        sat_var_of[sat_cells[j]] = 0;
    }
    if (r < 0) {
        (*undecided)++;
        return 0;
    }
    return r;
}


/* ================================================================
 * The exact family: entry e = (cos t, eps_e sin t d_{dir_e}),
 * dir_e = e/2, eps_e = +1 (even e) or -1 (odd e) - D97's sweep order;
 * axis j of the activation = d_j (D97's build_dirs over the sweep)
 * ================================================================ */

#define N_AXES 3
#define N_VOR  4                   /* 3 axes + 'no axis' */

static SurdiSpatium spatium;

/* 4 cos(j pi/12) in Z[sqrt2, sqrt3] (D120): bit 1 sqrt2, 2 sqrt3, 3 sqrt6 */
static Surdus
cos4_surdus (
    int j)
{
    Surdus r = surdus_ex_s64(0);
    int    neg = 0;
    int    b;

    j %= 24;
    if (j < 0) j += 24;
    if (j > 12) j = 24 - j;
    if (j > 6) { j = 12 - j; neg = 1; }
    switch (j) {
    case 0: r.c[0] = 4; break;
    case 1: r.c[3] = 1; r.c[1] = 1; break;
    case 2: r.c[2] = 2; break;
    case 3: r.c[1] = 2; break;
    case 4: r.c[0] = 2; break;
    case 5: r.c[3] = 1; r.c[1] = -1; break;
    default: break;
    }
    if (neg) {
        for (b = 0; b < 8; b++) r.c[b] = -r.c[b];
    }
    return r;
}

/* tan^2(jj pi/24) = (4 - 4C) / (4 + 4C), C = cos(jj pi/12) */
static Surdus
tan2_num (
    int jj)
{
    Surdus r = cos4_surdus(jj);
    int    b;

    for (b = 0; b < 8; b++) r.c[b] = -r.c[b];
    r.c[0] += 4;
    return r;
}

static Surdus
tan2_den (
    int jj)
{
    Surdus r = cos4_surdus(jj);

    r.c[0] += 4;
    return r;
}

static int
surd_sign_or_die (
    Surdus x)
{
    s32 sg = 0;

    if (!surdus_signum(&spatium, x, &sg)) {
        printf("  FATAL: surdus refused\n");
        exit(1);
    }
    return (int)sg;
}

/* sign(a_sq * Ta / ra - b_sq * Tb / rb) for breakpoint values n0^2 T(jj)/r */
static int
cmp_values (
    long a_sq, long ra, int ja,
    long b_sq, long rb, int jb)
{
    Surdus lhs;
    Surdus rhs;
    Surdus d;

    if (!surdus_multiplica(&spatium, tan2_num(ja), tan2_den(jb), &lhs)
        || !surdus_scala(lhs, (s64)(a_sq * rb), &lhs)
        || !surdus_multiplica(&spatium, tan2_num(jb), tan2_den(ja), &rhs)
        || !surdus_scala(rhs, (s64)(b_sq * ra), &rhs)
        || !surdus_subtrahe(lhs, rhs, &d)) {
        printf("  FATAL: surdus refused\n");
        exit(1);
    }
    return surd_sign_or_die(d);
}

/* ---------- breakpoints ---------- */

#define MAX_BP 4096
#define MAX_N0 7
#define MAX_R  64

typedef struct {
    int    n0;                     /* |n0| */
    long   n0sq;
    long   r;
    int    jj;
    double val;                    /* float value, for reading only */
} Breakpoint;

static Breakpoint bp[MAX_BP];
static int        n_bp = 0;
static int        n_rank = 0;      /* distinct values */
static int        bp_rank[MAX_BP];
static int        rank_of[MAX_N0][MAX_R][12];
static double     rank_val[MAX_BP];
static int        pair_seen[MAX_N0][MAX_R];

static void
mask_numbers (
    const int *idx,
    int        n_w,
    int        mask,
    int       *n0,
    int        nv[N_AXES])
{
    int i;

    *n0 = 0;
    nv[0] = nv[1] = nv[2] = 0;
    for (i = 0; i < n_w; i++) {
        int sg = ((mask >> i) & 1) ? 1 : -1;
        int e = idx[i];

        *n0 += sg;
        nv[e / 2] += sg * ((e % 2 == 0) ? 1 : -1);
    }
}

static void
collect_breakpoints (void)
{
    int n_w;
    int i;
    int j;

    memset(pair_seen, 0, sizeof(pair_seen));
    for (n_w = 3; n_w <= 6; n_w++) {
        int combo[8];

        for (i = 0; i < n_w; i++) combo[i] = i;
        do {
            int mask;

            for (mask = 0; mask < (1 << n_w); mask++) {
                int n0;
                int nv[N_AXES];
                int r;

                mask_numbers(combo, n_w, mask, &n0, nv);
                r = nv[0]*nv[0] + nv[1]*nv[1] + nv[2]*nv[2];
                if (n0 != 0 && r > 0) {
                    pair_seen[n0 < 0 ? -n0 : n0][r] = 1;
                }
            }
        } while (next_combo(combo, n_w, 6));
    }
    n_bp = 0;
    for (i = 1; i < MAX_N0; i++) {
        for (j = 1; j < MAX_R; j++) {
            int jj;

            if (!pair_seen[i][j]) continue;
            for (jj = 1; jj <= 11; jj++) {
                double t = tan((double)jj * M_PI / 24.0);

                bp[n_bp].n0 = i;
                bp[n_bp].n0sq = (long)i * i;
                bp[n_bp].r = j;
                bp[n_bp].jj = jj;
                bp[n_bp].val = (double)(i * i) * t * t / (double)j;
                n_bp++;
            }
        }
    }
}

static void
sort_breakpoints (void)
{
    int i;
    int j;

    /* insertion sort, exact comparator */
    for (i = 1; i < n_bp; i++) {
        Breakpoint key = bp[i];

        j = i - 1;
        while (j >= 0 && cmp_values(bp[j].n0sq, bp[j].r, bp[j].jj,
                key.n0sq, key.r, key.jj) > 0) {
            bp[j + 1] = bp[j];
            j--;
        }
        bp[j + 1] = key;
    }
    n_rank = 0;
    for (i = 0; i < n_bp; i++) {
        if (i > 0 && cmp_values(bp[i - 1].n0sq, bp[i - 1].r, bp[i - 1].jj,
                bp[i].n0sq, bp[i].r, bp[i].jj) == 0) {
            bp_rank[i] = bp_rank[i - 1];
        } else {
            bp_rank[i] = n_rank;
            rank_val[n_rank] = bp[i].val;
            n_rank++;
        }
        rank_of[bp[i].n0][bp[i].r][bp[i].jj] = bp_rank[i];
    }
}

/* ---------- positions: u = tan^2 t ---------- */

typedef struct {
    int kind;                      /* 0 interval, 1 at a breakpoint, 2 t = 90 */
    int rank;                      /* interval: below rank 'rank'; point: = */
} Position;

/* sign(u - value(n0, r, jj)) */
static int
u_cmp (
    Position p,
    int      n0abs,
    int      r,
    int      jj)
{
    int rk = rank_of[n0abs][r][jj];

    if (p.kind == 1) {
        return (p.rank > rk) - (p.rank < rk);
    }
    return rk < p.rank ? 1 : -1;   /* interval 'rank': above ranks < rank */
}

/* exact cell of one mask at position p, for k; tie set */
static void
exact_cell (
    Position  p,
    int       n0,
    const int nv[N_AXES],
    int       k,
    MaskCell *out)
{
    int r = nv[0]*nv[0] + nv[1]*nv[1] + nv[2]*nv[2];
    int a0 = n0 < 0 ? -n0 : n0;
    int sec = 0;
    int tie_m = -1;
    int axes[N_AXES];
    int n_axes = 0;
    int axis_rule = N_AXES;
    int a;
    int m;

    if (p.kind == 2) {
        a0 = 0;                    /* cos t = 0: real part vanishes */
        n0 = 0;
    }
    if (r == 0) {
        int cell;

        if (n0 == 0) {
            cell = (k - 1) * N_VOR + N_AXES;      /* zero sum */
        } else {
            cell = (n0 > 0 ? 0 : k - 1) * N_VOR + N_AXES;
        }
        out->float_cell = out->exact_cell = cell;
        out->n_ties = 1;
        out->ties[0] = cell;
        return;
    }
    if (n0 == 0) {
        sec = k / 2;
        tie_m = k / 2;
    } else if (n0 > 0) {
        for (m = 1; m < k / 2; m++) {
            int sg = u_cmp(p, a0, r, m * 24 / k);

            if (sg >= 0) sec++;
            if (sg == 0) tie_m = m;
        }
    } else {
        sec = k / 2;
        for (m = k / 2 + 1; m < k; m++) {
            int sg = u_cmp(p, a0, r, (k - m) * 24 / k);

            if (sg <= 0) sec++;
            if (sg == 0) tie_m = m;
        }
    }
    {
        int mx = 0;

        for (a = 0; a < N_AXES; a++) {
            int v = nv[a] < 0 ? -nv[a] : nv[a];

            if (v > mx) mx = v;
        }
        for (a = 0; a < N_AXES; a++) {
            int v = nv[a] < 0 ? -nv[a] : nv[a];

            if (v == mx) {
                axes[n_axes++] = a;
                if (a < axis_rule) axis_rule = a;
            }
        }
    }
    out->exact_cell = sec * N_VOR + axis_rule;
    out->float_cell = out->exact_cell;
    out->n_ties = 0;
    for (a = 0; a < n_axes; a++) {
        if (tie_m >= 0) {
            out->ties[out->n_ties++] = (tie_m - 1) * N_VOR + axes[a];
        }
        out->ties[out->n_ties++] = sec * N_VOR + axes[a];
    }
}

typedef struct {
    int rule;
    int robust;
    int possible;
    int tied;
} SetVerdict;

static MaskCell g_mc[MAX_MASKS];

static SetVerdict
judge (
    Position   p,
    const int *idx,
    int        n_w)
{
    SetVerdict v;
    int        tt[MAX_MASKS];
    int        n0s[MAX_MASKS];
    int        nvs[MAX_MASKS][N_AXES];
    int        n_masks = 1 << n_w;
    int        clash = 0;
    int        m;
    int        l;
    int        t;

    memset(&v, 0, sizeof(v));
    for (m = 0; m < n_masks; m++) {
        tt[m] = popcount(m) & 1;
        mask_numbers(idx, n_w, m, &n0s[m], nvs[m]);
    }
    /* same exact point, different truth value: no tie rule separates */
    for (m = 0; m < n_masks && !clash; m++) {
        for (l = m + 1; l < n_masks && !clash; l++) {
            if (tt[m] != tt[l]
                && (p.kind == 2 || n0s[m] == n0s[l])
                && nvs[m][0] == nvs[l][0] && nvs[m][1] == nvs[l][1]
                && nvs[m][2] == nvs[l][2]) {
                clash = 1;
            }
        }
    }
    for (t = 0; t < 3; t++) {
        int undecided = 0;
        int rb;

        for (m = 0; m < n_masks; m++) {
            exact_cell(p, n0s[m], nvs[m], KS[t], &g_mc[m]);
            if (g_mc[m].n_ties > 1) v.tied = 1;
        }
        v.rule |= labels_pass(g_mc, n_masks, tt, 0);
        rb = robust_pass(g_mc, n_masks, tt);
        v.robust |= rb;
        if (!v.possible) {
            v.possible = rb || (!clash
                && possible_pass(g_mc, n_masks, tt, &undecided));
        }
        if (undecided) {
            printf("  FATAL: SAT budget\n");
            exit(1);
        }
    }
    return v;
}

/* one FIXED tie rule (review M1): at a tie take the lower (0) or upper
 * (1) sector, the first (0) or last (1) maximal axis; XOR sets passing
 * at position p for N, OR over k as D97 */
static int
fixed_rule_count (
    Position p,
    int      n_w,
    int      upper,
    int      last)
{
    int combo[8];
    int count = 0;
    int i;

    for (i = 0; i < n_w; i++) combo[i] = i;
    do {
        int tt[MAX_MASKS];
        int n_masks = 1 << n_w;
        int pass = 0;
        int t;
        int m;

        for (m = 0; m < n_masks; m++) tt[m] = popcount(m) & 1;
        for (t = 0; t < 3 && !pass; t++) {
            for (m = 0; m < n_masks; m++) {
                int n0;
                int nv[N_AXES];
                int lo_sec = 99;
                int hi_sec = -1;
                int lo_ax = 99;
                int hi_ax = -1;
                int j;

                mask_numbers(combo, n_w, m, &n0, nv);
                exact_cell(p, n0, nv, KS[t], &g_mc[m]);
                for (j = 0; j < g_mc[m].n_ties; j++) {
                    int sec = g_mc[m].ties[j] / N_VOR;
                    int ax = g_mc[m].ties[j] % N_VOR;

                    if (sec < lo_sec) lo_sec = sec;
                    if (sec > hi_sec) hi_sec = sec;
                    if (ax < lo_ax) lo_ax = ax;
                    if (ax > hi_ax) hi_ax = ax;
                }
                g_mc[m].exact_cell = (upper ? hi_sec : lo_sec) * N_VOR
                    + (last ? hi_ax : lo_ax);
            }
            pass = labels_pass(g_mc, n_masks, tt, 0);
        }
        count += pass;
    } while (next_combo(combo, n_w, 6));
    return count;
}

/* capacity signature at a position: per N = 3..6, counts of rule,
 * robust, possible, tied */
typedef struct {
    int c[4][4];
} Signature;

static Signature
signature (
    Position p,
    int      n_hi)
{
    Signature sg;
    int       n_w;

    memset(&sg, 0, sizeof(sg));
    for (n_w = 3; n_w <= n_hi; n_w++) {
        int combo[8];
        int i;

        for (i = 0; i < n_w; i++) combo[i] = i;
        do {
            SetVerdict v = judge(p, combo, n_w);

            sg.c[n_w - 3][0] += v.rule;
            sg.c[n_w - 3][1] += v.robust;
            sg.c[n_w - 3][2] += v.possible;
            sg.c[n_w - 3][3] += v.tied;
        } while (next_combo(combo, n_w, 6));
    }
    return sg;
}

/* rank of the value n0^2 tan^2(jj pi/24) / r among the breakpoints
 * (exact comparison; -1 if no breakpoint has that value) */
static int
rank_of_value (
    long n0sq,
    long r,
    int  jj)
{
    int i;

    for (i = 0; i < n_bp; i++) {
        if (cmp_values(n0sq, r, jj, bp[i].n0sq, bp[i].r, bp[i].jj) == 0) {
            return bp_rank[i];
        }
    }
    return -1;
}

/* text 'arctan(n0 tan(j pi/24) / sqrt r)' for a rank, smallest n0 */
static void
closed_form (
    int   rank,
    char *out)
{
    int i;
    int best = -1;

    for (i = 0; i < n_bp; i++) {
        if (bp_rank[i] == rank && (best < 0 || bp[i].n0 < bp[best].n0
                || (bp[i].n0 == bp[best].n0 && bp[i].r < bp[best].r))) {
            best = i;
        }
    }
    {
        /* reduce n0^2/r: (2, 8) -> (1, 2) when the numerator stays square */
        long a = bp[best].n0sq;
        long b = bp[best].r;
        long g = a;
        long h = b;
        long n0 = bp[best].n0;
        long q;

        long qb;

        while (h) { long t = g % h; g = h; h = t; }
        a /= g;
        b /= g;
        /* coefficient sqrt(a/b), a/b in lowest terms */
        for (q = 1; q * q < a; q++) { }
        for (qb = 1; qb * qb < b; qb++) { }
        (void)n0;
        if (q * q == a && qb * qb == b) {
            if (qb == 1) {
                sprintf(out, "arctan(%ld tan(%d pi/24))", q, bp[best].jj);
            } else {
                sprintf(out, "arctan(%ld/%ld tan(%d pi/24))", q, qb,
                    bp[best].jj);
            }
        } else if (b == 1) {
            sprintf(out, "arctan(sqrt(%ld) tan(%d pi/24))", a, bp[best].jj);
        } else {
            sprintf(out, "arctan(sqrt(%ld/%ld) tan(%d pi/24))", a, b,
                bp[best].jj);
        }
    }
}

static double
degrees_of (
    double u)
{
    return atan(sqrt(u)) * 180.0 / M_PI;
}

/* ---------- placing an angle ---------- */

/* exact for multiples of 15 degrees: u = tan^2(jj pi/24), jj = 2..10 */
static Position
place_exact (
    int jj)
{
    Position p;
    int      lo = 0;
    int      i;

    p.kind = 0;
    p.rank = 0;
    for (i = 0; i < n_bp; i++) {
        int sg = cmp_values(1, 1, jj, bp[i].n0sq, bp[i].r, bp[i].jj);

        if (sg == 0) {
            p.kind = 1;
            p.rank = bp_rank[i];
            return p;
        }
        if (sg > 0 && bp_rank[i] + 1 > lo) lo = bp_rank[i] + 1;
    }
    p.rank = lo;
    return p;
}

/* exact for a rational u = pn/pd: sign(pn r (4 + 4C) - pd n0^2 (4 - 4C)) */
static Position
place_rational (
    long pn,
    long pd)
{
    Position p;
    int      lo = 0;
    int      i;

    p.kind = 0;
    p.rank = 0;
    for (i = 0; i < n_bp; i++) {
        Surdus lhs;
        Surdus rhs;
        Surdus d;
        int    sg;

        if (!surdus_scala(tan2_den(bp[i].jj), (s64)(pn * bp[i].r), &lhs)
            || !surdus_scala(tan2_num(bp[i].jj), (s64)(pd * bp[i].n0sq),
            &rhs) || !surdus_subtrahe(lhs, rhs, &d)) {
            printf("  FATAL: surdus refused\n");
            exit(1);
        }
        sg = surd_sign_or_die(d);
        if (sg == 0) {
            p.kind = 1;
            p.rank = bp_rank[i];
            return p;
        }
        if (sg > 0 && bp_rank[i] + 1 > lo) lo = bp_rank[i] + 1;
    }
    p.rank = lo;
    return p;
}

/* by float, certified: refuses (gap < 1e-9 relative) */
static int
place_float (
    double    degrees,
    Position *p)
{
    double u = tan(degrees * M_PI / 180.0);
    int    i;
    int    lo = 0;

    u *= u;
    for (i = 0; i < n_rank; i++) {
        if (fabs(u - rank_val[i]) < 1e-9 * u) {
            return 0;
        }
        if (u > rank_val[i]) lo = i + 1;
    }
    p->kind = 0;
    p->rank = lo;
    return 1;
}

/* ================================================================
 * Main
 * ================================================================ */

/* D97's sweep as printed (knotapel/demo_97_cell_b_perfect, run of the
 * unmodified source 2026-10-10): XOR counts N = 3..6 of 20, 15, 6, 1 */
#define N_ANG 21
static const double D97_ANG[N_ANG] = { 10, 15, 20, 25, 30, 35, 40, 42, 44,
    45, 46, 48, 50, 55, 60, 65, 70, 75, 80, 85, 90 };
static const int D97_X[N_ANG][4] = {
    { 20, 3, 0, 1 }, { 20, 15, 0, 1 }, { 20, 15, 0, 1 }, { 20, 15, 6, 1 },
    { 20, 15, 6, 1 }, { 20, 15, 0, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 1 },
    { 20, 15, 6, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 1 },
    { 20, 15, 6, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 1 },
    { 20, 15, 6, 1 }, { 20, 15, 6, 1 }, { 20, 15, 6, 0 }, { 20, 0, 0, 0 },
    { 0, 0, 0, 0 } };

static Signature sig_int[MAX_BP + 1];
static Signature sig_pt[MAX_BP];
static Signature sig_90;
static int       d97_equals_rule = 1;   /* Part D */

static const Signature *
sig_at (
    Position p)
{
    return p.kind == 2 ? &sig_90 : p.kind == 1 ? &sig_pt[p.rank]
        : &sig_int[p.rank];
}

static int
same_col (
    const Signature *a,
    const Signature *b,
    int              col)
{
    int n;

    for (n = 0; n < 4; n++) {
        if (a->c[n][col] != b->c[n][col]) return 0;
    }
    return 1;
}

/* one diagram: merge consecutive positions with equal counts in 'col' */
static void
print_diagram (
    const char *title,
    int         col)
{
    /* positions in order: I0, P0, I1, P1, ..., I_R, then 90 */
    int n_pos = 2 * n_rank + 2;
    int start = 0;
    int i;

    printf("\n  --- %s (XOR sets passing, N = 3/4/5/6 of 20/15/6/1) ---\n",
        title);
    for (i = 1; i <= n_pos; i++) {
        const Signature *cur;
        const Signature *first;

        first = start == n_pos - 1 ? &sig_90 : (start % 2 == 0)
            ? &sig_int[start / 2] : &sig_pt[start / 2];
        if (i < n_pos) {
            cur = i == n_pos - 1 ? &sig_90 : (i % 2 == 0)
                ? &sig_int[i / 2] : &sig_pt[i / 2];
            if (same_col(first, cur, col)) continue;
        }
        {
            int    e = i - 1;
            double lo;
            double hi;
            char   lb;
            char   rb;

            if (start == n_pos - 1) {
                lo = 90.0; lb = '[';
            } else if (start % 2 == 0) {
                lo = start == 0 ? 0.0 : degrees_of(rank_val[start / 2 - 1]);
                lb = '(';
            } else {
                lo = degrees_of(rank_val[start / 2]); lb = '[';
            }
            if (e == n_pos - 1) {
                hi = 90.0; rb = ']';
            } else if (e % 2 == 0) {
                hi = e / 2 == n_rank ? 90.0 : degrees_of(rank_val[e / 2]);
                rb = ')';
            } else {
                hi = degrees_of(rank_val[e / 2]); rb = ']';
            }
            {
                char cf[96];

                strcpy(cf, "90");
                if (e < n_pos - 1 && (e % 2 == 1 || e / 2 < n_rank)) {
                    closed_form(e / 2, cf);       /* the region's upper end */
                }
                if (start == e && start % 2 == 1) {
                    printf("    = %9.5f deg        | %2d %2d %d %d   %s\n", lo,
                        first->c[0][col], first->c[1][col],
                        first->c[2][col], first->c[3][col], cf);
                } else {
                    printf("    %c%9.5f, %9.5f%c  | %2d %2d %d %d   to %s\n",
                        lb, lo, hi, rb, first->c[0][col], first->c[1][col],
                        first->c[2][col], first->c[3][col], cf);
                }
            }
        }
        start = i;
    }
}

int
main (void)
{
    int  sizes[10];
    int  i;
    char msg[256];

    quick = getenv("DEMO122_CELER") != NULL;
    printf("KNOTAPEL DEMO 122: Cell B's Exact Phase Diagram\n");
    printf("===============================================\n");
    {
        s32 primi[3];

        primi[0] = 2;
        primi[1] = 3;
        primi[2] = 5;
        check("surd field Q(sqrt2, sqrt3, sqrt5)",
            surdi_spatium(primi, 3, &spatium));
    }

    /* ---------- Part A: Cell B and D97's sweep ---------- */
    printf("\n=== Part A: Cell B (D96/D97) and D97's float sweep ===\n");
    build_z8_d95();
    (void)derive_structure(sizes);
    d97_cell_b();
    {
        int ok = cellb_count == 6 && cellb_n_dirs == 3;

        /* exact: a = sqrt2/2 (Zr2 (0,1)), one nonzero vector coordinate */
        for (i = 0; i < g_z8_size; i++) {
            const QZ8 *q = &g_z8[i];
            int nz;

            if (g_level[i] != 0 || g_null[i]) continue;
            nz = (q->b.a || q->b.b) + (q->c.a || q->c.b) + (q->d.a || q->d.b);
            ok &= q->a.a == 0 && (q->a.b == 1 || q->a.b == -1) && nz == 1;
        }
        check("Cell B = 6 elements (+-45 degrees about 3 orthogonal axes: "
            "a = sqrt2/2, one nonzero vector coordinate)", ok);
    }
    {
        int ok = 1;
        int ai;
        int n_hi = quick ? 3 : 6;

        printf("    angle | D97 float replica (N = 3/4/5/6)\n");
        for (ai = 0; ai < N_ANG; ai++) {
            int c[4];
            int ni;

            for (ni = 0; ni < 4; ni++) {
                c[ni] = 3 + ni <= n_hi ? d97_sweep_count(D97_ANG[ai], 3 + ni)
                    : D97_X[ai][ni];
                ok &= c[ni] == D97_X[ai][ni];
            }
            printf("    %5.1f | %2d %2d %d %d\n", D97_ANG[ai], c[0], c[1], c[2],
                c[3]);
        }
        check("D97's sweep reproduced at all 21 angles", ok);
    }

    /* ---------- Part B: breakpoints ---------- */
    printf("\n=== Part B: breakpoints u* = n0^2 tan^2(j pi/24) / r ===\n");
    collect_breakpoints();
    sort_breakpoints();
    {
        int ok = 1;

        for (i = 1; i < n_bp; i++) {
            ok &= bp[i - 1].val <= bp[i].val * (1.0 + 1e-12);
        }
        /* the rank invariant: equal values share one rank, distinct values
         * strictly increase with rank (each neighbour pair, exactly) */
        for (i = 1; i < n_bp; i++) {
            int c = cmp_values(bp[i - 1].n0sq, bp[i - 1].r, bp[i - 1].jj,
                bp[i].n0sq, bp[i].r, bp[i].jj);

            ok &= c <= 0 && ((c == 0) == (bp_rank[i] == bp_rank[i - 1]))
                && bp_rank[i] - bp_rank[i - 1] == (c == 0 ? 0 : 1);
        }
        sprintf(msg, "%d breakpoints, %d distinct values, exactly sorted "
            "(equal values share a rank; float values agree)", n_bp, n_rank);
        check(msg, ok && n_rank > 0);
    }

    /* ---------- Part C: the phase diagram ---------- */
    printf("\n=== Part C: Cell B's capacity at every half-angle ===\n");
    {
        Position p;
        int      n_hi = quick ? 3 : 6;

        for (i = 0; i <= n_rank; i++) {
            p.kind = 0;
            p.rank = i;
            sig_int[i] = signature(p, n_hi);
            if (i < n_rank) {
                p.kind = 1;
                sig_pt[i] = signature(p, n_hi);
            }
        }
        p.kind = 2;
        p.rank = 0;
        sig_90 = signature(p, n_hi);
    }
    print_diagram("exact rule", 0);
    print_diagram("robust (every tie resolution)", 1);
    print_diagram("possible (some tie resolution)", 2);

    /* ---------- Part D: D97's angles, exactly ---------- */
    printf("\n=== Part D: D97's 21 angles placed exactly ===\n");
    printf("    angle | D97 float   | exact rule  | robust      | possible\n");
    {
        int ok = 1;
        int ai;

        for (ai = 0; ai < N_ANG; ai++) {
            double   ang = D97_ANG[ai];
            Position p;
            const Signature *sg;
            int      jj = (int)(ang / 7.5 + 0.5);

            if (ang >= 90.0) {
                p.kind = 2;
                p.rank = 0;
            } else if (fabs(ang - 7.5 * (double)jj) < 1e-9 && jj % 2 == 0) {
                p = place_exact(jj);
            } else if (!place_float(ang, &p)) {
                printf("    %5.1f | too close to a breakpoint\n", ang);
                ok = 0;
                continue;
            }
            sg = sig_at(p);
            {
                int n;

                for (n = 0; n < 4; n++) {
                    d97_equals_rule &= sg->c[n][0] == D97_X[ai][n];
                }
            }
            printf("    %5.1f | %2d %2d %d %d | %2d %2d %d %d | %2d %2d %d %d | "
                "%2d %2d %d %d%s\n", ang, D97_X[ai][0], D97_X[ai][1],
                D97_X[ai][2], D97_X[ai][3], sg->c[0][0], sg->c[1][0],
                sg->c[2][0], sg->c[3][0], sg->c[0][1], sg->c[1][1],
                sg->c[2][1], sg->c[3][1], sg->c[0][2], sg->c[1][2],
                sg->c[2][2], sg->c[3][2], p.kind == 1 ? "  (ON a breakpoint)"
                : p.kind == 2 ? "  (90: every sum a tie)" : "");
        }
        check("every D97 angle placed (multiples of 15 exactly, others by a "
            "certified float gap)", ok);
    }

    if (!quick) {
        Position p;
        int      ok;
        int      r_lo = -1;
        int      r_d1 = -1;
        int      r_d2 = -1;
        int      r_hi = -1;
        int      full;

        printf("\n=== Part E: the claims, exactly ===\n");
        /* ranks of the four boundaries, located by their closed forms */
        r_lo = rank_of_value(9, 1, 1);
        r_d1 = rank_of_value(25, 1, 1);
        r_d2 = rank_of_value(9, 1, 2);
        r_hi = rank_of_value(1, 2, 11);
        /* robust 100% at every N exactly on the open intervals strictly
         * inside (lo, d1) and (d2, hi), nowhere else */
        ok = r_lo >= 0 && r_d1 > r_lo && r_d2 > r_d1 && r_hi > r_d2;
        for (i = 0; i <= n_rank && ok; i++) {
            const Signature *sg = &sig_int[i];
            int inside = (i > r_lo && i <= r_d1) || (i > r_d2 && i <= r_hi);

            full = sg->c[0][1] == 20 && sg->c[1][1] == 15 && sg->c[2][1] == 6
                && sg->c[3][1] == 1;
            ok = full == inside;
        }
        for (i = 0; i < n_rank && ok; i++) {
            const Signature *sg = &sig_pt[i];
            int inside = (i > r_lo && i < r_d1) || (i > r_d2 && i < r_hi);

            full = sg->c[0][1] == 20 && sg->c[1][1] == 15 && sg->c[2][1] == 6
                && sg->c[3][1] == 1;
            ok = full == inside;
        }
        check("robust 100% at every N on exactly TWO open plateaus: "
            "(arctan(3 tan pi/24), arctan(5 tan pi/24)) = (21.552, 33.355) "
            "and (arctan(3 tan pi/12), arctan(tan(11pi/24)/sqrt2)) = (38.794, "
            "79.453) degrees - not 25..75", ok);
        ok = 1;
        for (i = r_d1 + 1; i <= r_d2; i++) {
            ok &= sig_int[i].c[2][1] == 0 && sig_int[i].c[2][2] == 0;
        }
        check("D97's '35 degree resonance' is a 5.44-degree BAND: on every "
            "interval of [33.355, 38.794] N = 5 fails robustly AND possibly",
            ok && r_d2 - r_d1 >= 1);
        p = place_exact(6);
        check("45 degrees is ON a breakpoint (u = 1) and robustly 100% - not "
            "the plateau's midpoint (59.12)", p.kind == 1
            && sig_at(p)->c[0][1] == 20 && sig_at(p)->c[1][1] == 15
            && sig_at(p)->c[2][1] == 6 && sig_at(p)->c[3][1] == 1);
        {
            /* at 90 every non-zero sum is tied between k/2 - 1 and k/2, so
             * 'possible' may put each vector on the side of its own truth
             * value: it passes ANY function constant on vectors - vacuous
             * there. The collapse is real under every FIXED tie rule. */
            int upper;
            int last;
            int n;

            p.kind = 2;
            p.rank = 0;
            ok = sig_90.c[0][1] == 0 && sig_90.c[1][1] == 0
                && sig_90.c[2][1] == 0 && sig_90.c[3][1] == 0
                && sig_90.c[0][2] == 20 && sig_90.c[1][2] == 15
                && sig_90.c[2][2] == 6 && sig_90.c[3][2] == 1;
            for (upper = 0; upper < 2; upper++) {
                for (last = 0; last < 2; last++) {
                    for (n = 3; n <= 6; n++) {
                        ok &= fixed_rule_count(p, n, upper, last) == 0;
                    }
                }
            }
            check("90 degrees: D97's 'total collapse' HOLDS - 0 at every N "
                "under all four fixed tie rules (lower/upper sector x "
                "first/last axis) and robustly; 'possible' 100% is vacuous "
                "there (every vector tied two ways)", ok);
        }
        ok = 1;
        for (i = 0; i <= n_rank; i++) {
            int n;

            for (n = 0; n < 4; n++) {
                ok &= sig_int[i].c[n][0] == sig_int[i].c[n][1];
            }
        }
        check("on every open interval the exact rule equals robust: the "
            "rule's tie choices matter only AT breakpoints", ok);
        check("D97's float sweep equals the exact rule at all 21 angles",
            d97_equals_rule);
        {
            /* oracle.py (cos^2 comparison in Q(sqrt2, sqrt3), no tan, no
             * breakpoints), run 2026-10-10: rule, robust, possible per
             * N = 3..6 at u = tan^2 t - constants, not a live call */
            static const long U[10][2] = { { 1, 100 }, { 1, 10 }, { 4, 25 },
                { 1, 3 }, { 49, 100 }, { 1, 1 }, { 3, 1 }, { 25, 1 },
                { 30, 1 }, { 400, 1 } };
            static const int OR[11][4][3] = {
                { { 8, 8, 8 }, { 3, 3, 15 }, { 0, 0, 0 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 0, 0, 0 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 0, 0, 0 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 0, 0, 0 } },
                { { 12, 12, 20 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
                { { 0, 0, 20 }, { 0, 0, 15 }, { 0, 0, 6 }, { 0, 0, 1 } } };
            int pt;
            int n;
            int c;

            ok = 1;
            for (pt = 0; pt < 11; pt++) {
                const Signature *sg;

                if (pt < 10) {
                    p = place_rational(U[pt][0], U[pt][1]);
                } else {
                    p.kind = 2;
                    p.rank = 0;
                }
                sg = sig_at(p);
                for (n = 0; n < 4; n++) {
                    for (c = 0; c < 3; c++) {
                        ok &= sg->c[n][c] == OR[pt][n][c];
                    }
                }
            }
            check("oracle.py's rule / robust / possible at 11 points (u = "
                "1/100 .. 400 and 90 degrees, incl. breakpoints 30, 45, 60 "
                "degrees) == the diagram", ok);
        }
        {
            /* oracle.py at the region boundaries themselves, u = A
             * tan^2(jj pi/24) / B exactly (where rule, robust, possible
             * part ways), same run */
            static const long BPT[7][3] = { { 1, 1, 1 }, { 9, 1, 1 },
                { 25, 1, 1 }, { 9, 1, 2 }, { 1, 2, 11 }, { 1, 1, 11 },
                { 8, 1, 1 } };
            static const int ORB[7][4][3] = {
                { { 8, 8, 20 }, { 3, 3, 15 }, { 0, 0, 0 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 0, 0, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 0, 0, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 0, 0, 6 }, { 1, 1, 1 } },
                { { 20, 20, 20 }, { 15, 15, 15 }, { 6, 6, 6 }, { 1, 0, 1 } },
                { { 20, 20, 20 }, { 3, 0, 3 }, { 6, 6, 6 }, { 0, 0, 0 } },
                { { 20, 20, 20 }, { 3, 3, 15 }, { 0, 0, 0 }, { 1, 1, 1 } } };
            int pt;
            int n;
            int c;

            ok = 1;
            for (pt = 0; pt < 7; pt++) {
                int rk = rank_of_value(BPT[pt][0], BPT[pt][1],
                    (int)BPT[pt][2]);

                ok &= rk >= 0;
                if (rk < 0) continue;
                for (n = 0; n < 4; n++) {
                    for (c = 0; c < 3; c++) {
                        ok &= sig_pt[rk].c[n][c] == ORB[pt][n][c];
                    }
                }
            }
            check("oracle.py AT 7 region boundaries (7.5, 20.42, 21.55, "
                "33.36, 38.79, 79.45, 82.5 degrees; rule, robust and possible "
                "part ways there) == the diagram", ok);
        }
    }

    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
