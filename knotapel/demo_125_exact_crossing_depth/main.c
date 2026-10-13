/*
 * KNOTAPEL DEMO 125: D82's Crossing Depth, Exactly
 * ================================================================
 *
 * D82 ("Crossing Depth and Computational Role") recorded:
 *   - Part B: XOR6..XOR12 winners of the full zeta_12 catalog (4096
 *     entries, truncated in round 8) and their depth histograms; "XOR12
 *     is the first level to use depth-8 entries";
 *   - Part D (the crux): at matched sizes, shallow / strided / deep
 *     subsets of 127 and 564 entries; deep 564 alone reaches XOR12
 *     ("algebraic coherence beats vocabulary", "deep entries are 2x more
 *     efficient" than the 1140-entry prefix);
 *   - section 7: with D81's per-round ladder (max XOR 6 6 8 8 10 10 12
 *     12 12), "max_xor ~ depth + 6".
 * D87 recorded that removing the 121 null entries of the same catalog
 * preserves capacity (XOR12, 32 = 32 winners).
 *
 * All of these are outputs of ONE greedy search (D81/D82/D87's
 * find_capacity): triples only from the first 100 entries, each level
 * extends the previous level's first 32 winners, the first k of the
 * ladder {1, 6, 8, 10, 12, 16, 20, 24} that separates, and a stop after
 * 32 winners per level. The search never goes past 6 weights (XOR12).
 * Cells are D82's combined_cell in floating point: sector by half-angle,
 * axis = first of the maximal |dot| over a direction table capped at
 * MAX_DIR 512 (D87: 2048), k skipped when k (dirs + 1) > MAX_ACT.
 *
 * Exhaustive search is out of reach at this size, so this demo audits
 * what D82's SEARCH finds, never "the capacity":
 *   Part A  the catalog exactly (Q(sqrt3), cellae), entry for entry
 *           equal to D82's float catalog; census, nulls, axes;
 *   Part B  D81 / D82 / D87's float search replicated bit for bit (the
 *           recorded tables reproduced first);
 *   Part C  the same search with EXACT cells (cellae): the rule
 *           (D82's semantics: sector floor, first maximal axis) and
 *           robust (every tie resolution), each with D82's caps (512
 *           axes, 16384 cells) and uncapped, over the 9 round prefixes,
 *           the 6 matched subsets and D87's non-null catalog; the
 *           search extended one level (XOR14) - the depth law predicts
 *           XOR13-14 at depths 7-8;
 *   Part D  winner depths (D82 Part B) under each semantics;
 *   Part E  order control: the greedy result depends on entry ORDER
 *           (triples from the first 100, the first 32 winners), so the
 *           three 564 subsets in 10 seeded orders each, and 10 random
 *           564-entry samples drawn from all depths;
 *   Part F  D87: full vs non-null;
 *   Part G  the claims.
 *
 * Independent check: oracle.py (exact Q(sqrt3) in Python, after the
 * cellae reviewer's code) rebuilds the catalog and re-decides every
 * winner of Part C and every XOR12 winner of Part E written to
 * $D125_VICTORES.
 *
 * Every search runs in a child process (this binary, "--opus N"), at
 * most 8 at once, longest first; the parent prints from the collected
 * results in a fixed order, so the output does not depend on timing.
 *
 * House libraries: cellae, extensio, processus, fenestra_tempus. Build
 * and run from the repo root:
 *   ./bin/aedilis knotapel/demo_125_exact_crossing_depth/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * Environment: D125_TEMPORA=1 prints timings (stderr); D125_VICTORES=
 * <path> writes the winners for oracle.py; D125_CELER=1 runs a reduced
 * plan (plants only); D125_SERIAL=1 runs every search in this process
 * (same output); D125_OPERARII=<n> sets the number of children.
 */

#include "cellae.h"
#include "extensio.h"
#include "processus.h"
#include "fenestra_tempus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

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

static int   tempora = 0;
static int   celer = 0;
static FILE *victores = NULL;

static Piscina *piscina;

/* ================================================================
 * Results of one search (float or exact)
 * ================================================================ */

#define MAX_WIN   32
#define MAX_TUPLE 7
#define N_LEVELS  8

typedef struct {
    long   num[N_LEVELS];                  /* winners found per n */
    int    nwin[N_LEVELS];
    int    win[N_LEVELS][MAX_WIN][MAX_TUPLE];  /* subset indices */
    int    wk[N_LEVELS][MAX_WIN];
    int    max_xor;
    int    axes;
    int    trunc;                          /* axis table cut */
    int    angles;                         /* float runs only */
    long   st_filter;
    long   st_exact;
    long   st_tied;
    double secs;
} Run;

static void
run_clear (
    Run *r)
{
    memset(r, 0, sizeof(*r));
}

/* same winners (sets, order, k) at every level? If not: the first
 * level that differs, and whether its sets agree (only k differs) */
static int
runs_same_winners (
    const Run *x,
    const Run *y,
    int        nmax,
    int       *first_level,
    int       *only_k)
{
    int n;
    int w;
    int j;

    *first_level = 0;
    *only_k = 0;
    for (n = 3; n <= nmax; n++) {
        int sets_same = x->nwin[n] == y->nwin[n];
        int k_same = 1;

        for (w = 0; w < x->nwin[n] && sets_same; w++) {
            if (x->wk[n][w] != y->wk[n][w]) k_same = 0;
            for (j = 0; j < n; j++) {
                if (x->win[n][w][j] != y->win[n][w][j]) sets_same = 0;
            }
        }
        if (!sets_same || !k_same) {
            *first_level = n;
            *only_k = sets_same;
            return 0;
        }
    }
    return 1;
}

/* ================================================================
 * Part B machinery: D81/D82/D87's float code, replicated (f_*)
 *
 * Verbatim except: MAX_DIR and MAX_ACT are run-time values (D81/D82:
 * 512 / 16384, D87: 2048 / 65536) and winners go into a Run.
 * ================================================================ */

typedef struct { double a, b, c, d; } Quat;

#define F_QCAT    4096
#define F_ROUNDS  16
#define F_DIR_CAP 2048
#define F_ACT_CAP 65536

static Quat f_gen[3];
static Quat f_full[F_QCAT];
static int  f_full_depth[F_QCAT];
static int  f_full_size = 0;
static int  f_round_end[F_ROUNDS];
static int  f_rounds = 0;

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
        if (fabs(f_full[i].a - q->a) < 1e-10 && fabs(f_full[i].b - q->b) < 1e-10
            && fabs(f_full[i].c - q->c) < 1e-10
            && fabs(f_full[i].d - q->d) < 1e-10) {
            return i;
        }
        if (fabs(f_full[i].a + q->a) < 1e-10 && fabs(f_full[i].b + q->b) < 1e-10
            && fabs(f_full[i].c + q->c) < 1e-10
            && fabs(f_full[i].d + q->d) < 1e-10) {
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

    f_full[0].a = 1; f_full[0].b = 0; f_full[0].c = 0; f_full[0].d = 0;
    f_full_depth[0] = 0;
    f_full_size = 1;
    gens[0] = f_gen[1];
    gens[1].a = f_gen[1].a; gens[1].b = -f_gen[1].b;
    gens[1].c = -f_gen[1].c; gens[1].d = -f_gen[1].d;
    gens[2] = f_gen[2];
    gens[3].a = f_gen[2].a; gens[3].b = -f_gen[2].b;
    gens[3].c = -f_gen[2].c; gens[3].d = -f_gen[2].d;
    for (gi = 0; gi < 4; gi++) {
        if (f_find_in_cat(&gens[gi], f_full_size) < 0
            && f_full_size < F_QCAT) {
            f_full_depth[f_full_size] = 0;
            f_full[f_full_size++] = gens[gi];
        }
    }
    f_round_end[0] = f_full_size;
    f_rounds = 1;
    rd = 1;
    do {
        prev = f_full_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                Quat        prod;
                const Quat *p = &f_full[i];
                const Quat *g = &gens[gi];

                prod.a = p->a*g->a - p->b*g->b - p->c*g->c - p->d*g->d;
                prod.b = p->a*g->b + p->b*g->a + p->c*g->d - p->d*g->c;
                prod.c = p->a*g->c - p->b*g->d + p->c*g->a + p->d*g->b;
                prod.d = p->a*g->d + p->b*g->c - p->c*g->b + p->d*g->a;
                if (f_find_in_cat(&prod, f_full_size) < 0) {
                    if (f_full_size < F_QCAT) {
                        f_full_depth[f_full_size] = rd;
                        f_full[f_full_size++] = prod;
                    }
                }
            }
        }
        if (f_full_size > prev) {
            f_round_end[f_rounds] = f_full_size;
            f_rounds++;
        }
        rd++;
    } while (f_full_size > prev && f_rounds < F_ROUNDS);
}

/* the working (sub)catalog, as D82's g_cat */
static Quat g_cat[F_QCAT];
static int  g_cat_size = 0;

static void
f_load (
    const int *idx,
    int        count)
{
    int i;

    for (i = 0; i < count; i++) {
        g_cat[i] = f_full[idx[i]];
    }
    g_cat_size = count;
}

static int    f_max_dir = 512;
static int    f_max_act = 16384;
static double g_dir[F_DIR_CAP][3];
static int    g_nd = 0;

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
        ax = qb/nv; ay = qc/nv; az = qd/nv;
        for (j = 0; j < g_nd; j++) {
            double d1 = fabs(g_dir[j][0]-ax) + fabs(g_dir[j][1]-ay)
                + fabs(g_dir[j][2]-az);
            double d2 = fabs(g_dir[j][0]+ax) + fabs(g_dir[j][1]+ay)
                + fabs(g_dir[j][2]+az);

            if (d1 < 1e-8 || d2 < 1e-8) { found = 1; break; }
        }
        if (!found && g_nd < f_max_dir) {
            g_dir[g_nd][0] = ax; g_dir[g_nd][1] = ay; g_dir[g_nd][2] = az;
            g_nd++;
        }
    }
}

static int
f_count_angles (
    int cat_size)
{
    double angles[512];
    int    n_angles = 0;
    int    i;
    int    j;

    for (i = 0; i < cat_size; i++) {
        double qa = g_cat[i].a;
        double qb = g_cat[i].b;
        double qc = g_cat[i].c;
        double qd = g_cat[i].d;
        double nv;
        double ha;
        int    found = 0;

        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        ha = atan2(nv, qa) * 180.0 / M_PI;
        for (j = 0; j < n_angles; j++) {
            if (fabs(angles[j] - ha) < 0.1) { found = 1; break; }
        }
        if (!found && n_angles < 512) angles[n_angles++] = ha;
    }
    return n_angles;
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

static int cell_even[F_ACT_CAP];
static int cell_odd[F_ACT_CAP];
static int touched_cells[F_ACT_CAP];

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
    if (n_cells > f_max_act || n_inputs > 14) return 0;
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
        if (cell_even[cell] == 0 && cell_odd[cell] == 0)
            touched_cells[n_touched++] = cell;
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

static const int K_LADDER[] = { 1, 6, 8, 10, 12, 16, 20, 24 };
#define N_KLADDER 8

static void
f_find_xor6 (
    int  bf_limit,
    Run *r)
{
    int ai;
    int aj;
    int ak;
    int ki;
    int indices[3];

    r->nwin[3] = 0;
    for (ai = 0; ai < bf_limit && r->nwin[3] < MAX_WIN; ai++)
    for (aj = ai+1; aj < bf_limit && r->nwin[3] < MAX_WIN; aj++)
    for (ak = aj+1; ak < bf_limit && r->nwin[3] < MAX_WIN; ak++) {
        indices[0] = ai; indices[1] = aj; indices[2] = ak;
        for (ki = 0; ki < N_KLADDER; ki++) {
            int nc2 = K_LADDER[ki] * (g_nd + 1);

            if (nc2 > f_max_act) continue;
            if (f_test_xor(indices, 3, K_LADDER[ki])) {
                r->win[3][r->nwin[3]][0] = ai;
                r->win[3][r->nwin[3]][1] = aj;
                r->win[3][r->nwin[3]][2] = ak;
                r->wk[3][r->nwin[3]] = K_LADDER[ki];
                r->nwin[3]++;
                break;
            }
        }
    }
}

static void
f_find_recursive (
    int  level,
    int  cat_limit,
    Run *r)
{
    int prev = level - 1;
    int wi;
    int ae;
    int ki;
    int i;
    int j;

    r->nwin[level] = 0;
    if (r->nwin[prev] == 0) return;
    for (wi = 0; wi < r->nwin[prev] && r->nwin[level] < MAX_WIN; wi++) {
        for (ae = 0; ae < cat_limit && r->nwin[level] < MAX_WIN; ae++) {
            int in_winner = 0;
            int dup = 0;
            int indices[MAX_TUPLE];
            int pos;

            for (i = 0; i < prev; i++) {
                if (r->win[prev][wi][i] == ae) { in_winner = 1; break; }
            }
            if (in_winner) continue;
            pos = prev;
            for (i = 0; i < prev; i++) {
                if (ae < r->win[prev][wi][i]) { pos = i; break; }
            }
            for (i = 0; i < pos; i++) indices[i] = r->win[prev][wi][i];
            indices[pos] = ae;
            for (i = pos; i < prev; i++) indices[i+1] = r->win[prev][wi][i];
            for (j = 0; j < r->nwin[level]; j++) {
                int match = 1;

                for (i = 0; i < level; i++) {
                    if (r->win[level][j][i] != indices[i]) {
                        match = 0; break;
                    }
                }
                if (match) { dup = 1; break; }
            }
            if (dup) continue;
            for (ki = 0; ki < N_KLADDER; ki++) {
                int nc2 = K_LADDER[ki] * (g_nd + 1);

                if (nc2 > f_max_act) continue;
                if (f_test_xor(indices, level, K_LADDER[ki])) {
                    memcpy(r->win[level][r->nwin[level]], indices,
                        (size_t)level * sizeof(int));
                    r->wk[level][r->nwin[level]] = K_LADDER[ki];
                    r->nwin[level]++;
                    break;
                }
            }
        }
    }
}

/* D82's find_capacity on the subset idx[0..n) */
static void
f_capacity (
    const int *idx,
    int        n,
    int        max_dir,
    int        max_act,
    Run       *r)
{
    clock_t t0 = clock();
    int     level;

    run_clear(r);
    f_max_dir = max_dir;
    f_max_act = max_act;
    f_load(idx, n);
    f_build_dirs(n);
    r->axes = g_nd;
    r->angles = f_count_angles(n);
    f_find_xor6(n < 100 ? n : 100, r);
    for (level = 4; level <= 6; level++) {
        if (r->nwin[level - 1] > 0) f_find_recursive(level, n, r);
    }
    for (level = 3; level <= 6; level++) {
        r->num[level] = r->nwin[level];
        if (r->nwin[level] > 0) r->max_xor = 2 * level;
    }
    r->secs = (double)(clock() - t0) / CLOCKS_PER_SEC;
}

/* ================================================================
 * Part A machinery: the exact catalog (cellae over Q(sqrt3))
 * ================================================================ */

static Catalogus *e_full;
static const s32  TORSIONES[4] = { 1, -1, 1, -1 };

static double
e_component (
    Quaternio q,
    int       k)
{
    const Algebraicus *a = (const Algebraicus *)quaternio_pars(q, (i32)k);
    PiscinaNotatio     nota = piscina_notare(piscina);
    chorda             textus = algebraicus_ad_ostendendum(*a, XVIII,
        piscina);
    char               buf[64];
    double             v;

    if (textus.mensura >= (i32)sizeof(buf)) die("component text");
    memcpy(buf, textus.datum, (size_t)textus.mensura);
    buf[textus.mensura] = '\0';
    v = strtod(buf, NULL);
    piscina_reficere(piscina, nota);
    return v;
}

static void
e_build_catalog (void)
{
    const Anulus *q3 = extensio_anulus(extensio_quadratica(III, piscina));
    Quaternio     g[4];
    static const char *const gtext[4] = {
        "[a/2, 1/2, 0, 0]", "[a/2, -1/2, 0, 0]",
        "[a/2, 0, 0, -1/2]", "[a/2, 0, 0, 1/2]"
    };
    int i;

    for (i = 0; i < 4; i++) {
        if (!quaternio_ex_chorda(q3, chorda_ex_literis(gtext[i], piscina),
                piscina, &g[i]))
            die("generator");
    }
    if (!catalogus_clausura(g, TORSIONES, 4, F_QCAT, F_ROUNDS, piscina,
            &e_full))
        die("catalogus_clausura");
}

/* ================================================================
 * Part C machinery: D82's search on exact cells
 * ================================================================ */

#define MODE_RULE   CELLAE_REGULA
#define MODE_ROBUST CELLAE_ROBUSTA

static i32 e_idx[F_QCAT];

static void
e_capacity (
    const int *idx,
    int        n,
    int        mode,
    int        capped,
    int        nmax,
    Run       *r)
{
    Piscina          *p = piscina_generare_dynamicum("d125", 1 << 22);
    Catalogus        *pars = NULL;
    Activatio        *act = NULL;
    Quaestio          q;
    Capacitas         c;
    CellaeStatistica  st;
    clock_t           t0 = clock();
    int               i;
    int               lv;
    int               w;

    run_clear(r);
    if (p == NULL) die("piscina");
    for (i = 0; i < n; i++) e_idx[i] = (i32)idx[i];
    if (!catalogus_pars(e_full, e_idx, (i32)n, p, &pars)) die("pars");
    if (!activatio_ex_catalogo(pars, capped ? 512 : 0, p, &act))
        die("activatio");
    q.scala_k = (const i32 *)NULL;
    {
        static i32 ladder[N_KLADDER];

        for (i = 0; i < N_KLADDER; i++) ladder[i] = (i32)K_LADDER[i];
        q.scala_k = ladder;
    }
    q.numerus_k = N_KLADDER;
    q.modus = (i32)mode;
    q.n_maximum = (i32)nmax;
    q.victores_maximi = MAX_WIN;
    q.limes_triplicum = 100;
    q.cellae_maximae = capped ? 16384 : 0;
    q.limes_servandi = MAX_WIN;
    q.indices = NULL;
    q.numerus_indicum = 0;
    if (!activatio_avida(act, &q, p, &c)) die("activatio_avida");
    r->axes = (int)activatio_axes(act);
    r->trunc = activatio_truncata(act) ? 1 : 0;
    for (lv = 3; lv <= nmax; lv++) {
        r->num[lv] = (long)c.numerus[lv];
        r->nwin[lv] = (int)c.servati[lv];
        for (w = 0; w < r->nwin[lv]; w++) {
            r->wk[lv][w] = (int)c.victores[lv][w].k;
            for (i = 0; i < lv; i++) {
                r->win[lv][w][i] = (int)c.victores[lv][w].pondera[i];
            }
        }
    }
    r->max_xor = (int)c.maximum;
    activatio_statistica(act, &st);
    r->st_filter = (long)st.filtro;
    r->st_exact = (long)st.exacte;
    r->st_tied = (long)st.ligatae;
    r->secs = (double)(clock() - t0) / CLOCKS_PER_SEC;
    piscina_destruere(p);
}

/* ================================================================
 * Configurations (subsets of the full catalog, in order)
 * ================================================================ */

#define N_PREFIX 9
#define MAX_CONF 24

typedef struct {
    char name[32];
    int  n;
    int *idx;
} Conf;

static Conf conf[MAX_CONF];
static int  n_conf = 0;
static int  idx_pool[MAX_CONF * F_QCAT];

static Conf *
conf_new (
    const char *name,
    int         n)
{
    Conf *c;

    if (n_conf >= MAX_CONF) die("too many configurations");
    c = &conf[n_conf];
    sprintf(c->name, "%.31s", name);
    c->n = n;
    c->idx = &idx_pool[n_conf * F_QCAT];
    n_conf++;
    return c;
}

static Conf *
conf_find (
    const char *name)
{
    int i;

    for (i = 0; i < n_conf; i++) {
        if (strcmp(conf[i].name, name) == 0) return &conf[i];
    }
    die(name);
    return NULL;
}

static int
conf_at (
    const char *name)
{
    return (int)(conf_find(name) - conf);
}

/* writes a configuration and its winners for oracle.py */
static void
victores_write (
    const Conf *c,
    const char *mode,
    const char *table,
    const Run  *r,
    int         nmin)
{
    int i;
    int lv;
    int w;

    if (victores == NULL) return;
    fprintf(victores, "C\t%s\t%s\t%s\t%d\t", c->name, mode, table, c->n);
    for (i = 0; i < c->n; i++) {
        fprintf(victores, i ? " %d" : "%d", c->idx[i]);
    }
    fprintf(victores, "\n");
    for (lv = nmin; lv < N_LEVELS; lv++) {
        for (w = 0; w < r->nwin[lv]; w++) {
            fprintf(victores, "W\t%d\t%d\t", lv, r->wk[lv][w]);
            for (i = 0; i < lv; i++) {
                fprintf(victores, i ? " %d" : "%d", r->win[lv][w][i]);
            }
            fprintf(victores, "\n");
        }
    }
    fflush(victores);
}

static void
tempus (
    const char *what,
    const Run  *r)
{
    if (tempora) {
        fprintf(stderr, "  [%s: %.1f s]\n", what, r->secs);
        fflush(stderr);
    }
}

/* ================================================================
 * Pinned results: every search is compared with this run's frozen
 * table (n3..n7 found, max XOR, axes, and a checksum of every winner
 * set and its k; -1 = not compared). The counts alone would not see a
 * search that finds different winners in the same number (a plant
 * did exactly that). A live rerun against changed libraries is then a
 * regression check.
 * ================================================================ */

/* FNV-1a over (level, k, indices) of every winner, 31 bits */
static long
winners_sum (
    const Run *r)
{
    unsigned long h = 2166136261UL;
    int           lv;
    int           w;
    int           j;

    for (lv = 3; lv < N_LEVELS; lv++) {
        for (w = 0; w < r->nwin[lv]; w++) {
            h = ((h ^ (unsigned long)lv) * 16777619UL) & 0xffffffffUL;
            h = ((h ^ (unsigned long)r->wk[lv][w]) * 16777619UL)
                & 0xffffffffUL;
            for (j = 0; j < lv; j++) {
                h = ((h ^ (unsigned long)r->win[lv][w][j]) * 16777619UL)
                    & 0xffffffffUL;
            }
        }
    }
    return (long)(h & 0x7fffffffUL);
}

typedef struct {
    const char *section;
    const char *name;
    long        v[8];
} Pin;

static const Pin PINS[] = {
/* PINS BEGIN */
    { "float", "round 0", { 8, 0, 0, 0, 0, 6, 2, 620468251 } },
    { "float", "round 1", { 32, 0, 0, 0, 0, 6, 6, 770366412 } },
    { "float", "round 2", { 32, 32, 0, 0, 0, 8, 22, 1739579427 } },
    { "float", "round 3", { 32, 32, 0, 0, 0, 8, 46, 1757367607 } },
    { "float", "round 4", { 32, 32, 32, 0, 0, 10, 114, 2142514815 } },
    { "float", "round 5", { 32, 32, 32, 0, 0, 10, 239, 14814249 } },
    { "float", "round 6", { 32, 32, 32, 12, 0, 12, 507, 1411049497 } },
    { "float", "round 7", { 32, 32, 32, 32, 0, 12, 512, 1985064467 } },
    { "float", "round 8", { 32, 32, 32, 32, 0, 12, 512, 1134706529 } },
    { "float", "strided 127", { 32, 32, 26, 0, 0, 10, 125, 1565916778 } },
    { "float", "deep 127", { 32, 32, 32, 0, 0, 10, 119, 2054764010 } },
    { "float", "strided 564", { 32, 32, 32, 0, 0, 10, 512, 58318450 } },
    { "float", "deep 564", { 32, 32, 32, 19, 0, 12, 476, 1049093399 } },
    { "float", "non-null", { 32, 32, 32, 32, 0, 12, 1976, 343290072 } },
    { "float", "full (D87)", { 32, 32, 32, 32, 0, 12, 2043, 222133462 } },
    { "rule capped", "round 0", { 8, 0, 0, 0, 0, 6, 2, 1622783435 } },
    { "rule capped", "round 1", { 32, 3, 0, 0, 0, 8, 6, 802696525 } },
    { "rule capped", "round 2", { 32, 32, 0, 0, 0, 8, 22, 443781740 } },
    { "rule capped", "round 3", { 32, 32, 0, 0, 0, 8, 46, 208478593 } },
    { "rule capped", "round 4", { 32, 32, 32, 0, 0, 10, 114, 1270436941 } },
    { "rule capped", "round 5", { 32, 32, 32, 0, 0, 10, 239, 633995208 } },
    { "rule capped", "round 6", { 32, 32, 32, 14, 0, 12, 507, 1279774261 } },
    { "rule capped", "round 7", { 32, 32, 32, 32, 0, 12, 512, 1700668339 } },
    { "rule capped", "round 8", { 32, 32, 32, 32, 0, 12, 512, 1601966739 } },
    { "rule capped", "strided 127", { 32, 32, 22, 0, 0, 10, 125, 631016714 } },
    { "rule capped", "deep 127", { 32, 32, 32, 0, 0, 10, 119, 274337985 } },
    { "rule capped", "strided 564", { 32, 32, 32, 0, 0, 10, 512, 154738134 } },
    { "rule capped", "deep 564", { 32, 32, 32, 21, 0, 12, 476, 477532080 } },
    { "rule uncapped", "round 0", { 8, 0, 0, 0, 0, 6, 2, 1622783435 } },
    { "rule uncapped", "round 1", { 32, 3, 0, 0, 0, 8, 6, 802696525 } },
    { "rule uncapped", "round 2", { 32, 32, 0, 0, 0, 8, 22, 443781740 } },
    { "rule uncapped", "round 3", { 32, 32, 0, 0, 0, 8, 46, 208478593 } },
    { "rule uncapped", "round 4", { 32, 32, 32, 0, 0, 10, 114, 1270436941 } },
    { "rule uncapped", "round 5", { 32, 32, 32, 0, 0, 10, 239, 633995208 } },
    { "rule uncapped", "round 6", { 32, 32, 32, 14, 0, 12, 507, 1279774261 } },
    { "rule uncapped", "round 7", { 32, 32, 32, 32, 0, 12, 975, 677217894 } },
    { "rule uncapped", "round 8", { 32, 32, 32, 32, 0, 12, 2043, 373716020 } },
    { "rule uncapped", "strided 127", { 32, 32, 22, 0, 0, 10, 125, 631016714 } },
    { "rule uncapped", "deep 127", { 32, 32, 32, 0, 0, 10, 119, 274337985 } },
    { "rule uncapped", "strided 564", { 32, 32, 32, 0, 0, 10, 517, 154738134 } },
    { "rule uncapped", "deep 564", { 32, 32, 32, 21, 0, 12, 476, 477532080 } },
    { "rule uncapped", "non-null", { 32, 32, 32, 32, 0, 12, 1976, 222523667 } },
    { "robust capped", "round 0", { 4, 0, 0, 0, 0, 6, 2, 1401769321 } },
    { "robust capped", "round 1", { 32, 0, 0, 0, 0, 6, 6, 1529715125 } },
    { "robust capped", "round 2", { 32, 32, 0, 0, 0, 8, 22, 318492292 } },
    { "robust capped", "round 3", { 32, 32, 0, 0, 0, 8, 46, 1321165011 } },
    { "robust capped", "round 4", { 32, 32, 32, 0, 0, 10, 114, 430689453 } },
    { "robust capped", "round 5", { 32, 32, 32, 0, 0, 10, 239, 1528925190 } },
    { "robust capped", "round 6", { 32, 32, 32, 8, 0, 12, 507, 607277398 } },
    { "robust capped", "round 7", { 32, 32, 32, 27, 0, 12, 512, 1889350810 } },
    { "robust capped", "round 8", { 32, 32, 32, 29, 0, 12, 512, 665607474 } },
    { "robust capped", "strided 127", { 32, 32, 20, 0, 0, 10, 125, 153723396 } },
    { "robust capped", "deep 127", { 32, 32, 32, 0, 0, 10, 119, 387230247 } },
    { "robust capped", "strided 564", { 32, 32, 32, 0, 0, 10, 512, 1266401867 } },
    { "robust capped", "deep 564", { 32, 32, 32, 13, 0, 12, 476, 123013047 } },
    { "robust uncapped", "round 0", { 4, 0, 0, 0, 0, 6, 2, 1401769321 } },
    { "robust uncapped", "round 1", { 32, 0, 0, 0, 0, 6, 6, 1529715125 } },
    { "robust uncapped", "round 2", { 32, 32, 0, 0, 0, 8, 22, 318492292 } },
    { "robust uncapped", "round 3", { 32, 32, 0, 0, 0, 8, 46, 1321165011 } },
    { "robust uncapped", "round 4", { 32, 32, 32, 0, 0, 10, 114, 430689453 } },
    { "robust uncapped", "round 5", { 32, 32, 32, 0, 0, 10, 239, 1528925190 } },
    { "robust uncapped", "round 6", { 32, 32, 32, 8, 0, 12, 507, 607277398 } },
    { "robust uncapped", "round 7", { 32, 32, 32, 32, 0, 12, 975, 1152929817 } },
    { "robust uncapped", "round 8", { 32, 32, 32, 32, 0, 12, 2043, 1326049101 } },
    { "robust uncapped", "strided 127", { 32, 32, 20, 0, 0, 10, 125, 153723396 } },
    { "robust uncapped", "deep 127", { 32, 32, 32, 0, 0, 10, 119, 387230247 } },
    { "robust uncapped", "strided 564", { 32, 32, 32, 0, 0, 10, 517, 1266401867 } },
    { "robust uncapped", "deep 564", { 32, 32, 32, 13, 0, 12, 476, 123013047 } },
    { "robust uncapped", "non-null", { 32, 32, 32, 32, 0, 12, 1976, 940856804 } },
    { "order", "shallow 564 order 0", { 32, 32, 32, 0, 0, 10, 239, 1724387080 } },
    { "order", "shallow 564 order 1", { 32, 32, 32, 0, 0, 10, 239, 283545754 } },
    { "order", "shallow 564 order 2", { 32, 32, 32, 1, 0, 12, 239, 970072164 } },
    { "order", "shallow 564 order 3", { 32, 32, 32, 0, 0, 10, 239, 1082989126 } },
    { "order", "shallow 564 order 4", { 32, 32, 32, 0, 0, 10, 239, 1342355267 } },
    { "order", "shallow 564 order 5", { 32, 32, 32, 0, 0, 10, 239, 52202942 } },
    { "order", "shallow 564 order 6", { 32, 32, 32, 0, 0, 10, 239, 1443532772 } },
    { "order", "shallow 564 order 7", { 32, 32, 32, 0, 0, 10, 239, 1175442605 } },
    { "order", "shallow 564 order 8", { 32, 32, 32, 0, 0, 10, 239, 34519654 } },
    { "order", "shallow 564 order 9", { 32, 32, 32, 0, 0, 10, 239, 1293526153 } },
    { "order", "strided 564 order 0", { 32, 32, 32, 20, 0, 12, 517, 915548835 } },
    { "order", "strided 564 order 1", { 32, 32, 32, 1, 0, 12, 517, 478278004 } },
    { "order", "strided 564 order 2", { 32, 32, 32, 27, 0, 12, 517, 1958517209 } },
    { "order", "strided 564 order 3", { 32, 32, 32, 4, 0, 12, 517, 1465106782 } },
    { "order", "strided 564 order 4", { 32, 32, 32, 1, 0, 12, 517, 1514666988 } },
    { "order", "strided 564 order 5", { 32, 32, 32, 0, 0, 10, 517, 884828458 } },
    { "order", "strided 564 order 6", { 32, 32, 32, 7, 0, 12, 517, 668461939 } },
    { "order", "strided 564 order 7", { 32, 32, 32, 1, 0, 12, 517, 940301421 } },
    { "order", "strided 564 order 8", { 32, 32, 32, 17, 0, 12, 517, 428104810 } },
    { "order", "strided 564 order 9", { 32, 32, 32, 0, 0, 10, 517, 2054605076 } },
    { "order", "deep 564 order 0", { 32, 32, 32, 3, 0, 12, 476, 1711359092 } },
    { "order", "deep 564 order 1", { 32, 32, 32, 2, 0, 12, 476, 1497274618 } },
    { "order", "deep 564 order 2", { 32, 32, 32, 2, 0, 12, 476, 1616408841 } },
    { "order", "deep 564 order 3", { 32, 32, 32, 1, 0, 12, 476, 393167026 } },
    { "order", "deep 564 order 4", { 32, 32, 32, 7, 0, 12, 476, 1107459129 } },
    { "order", "deep 564 order 5", { 32, 32, 32, 7, 0, 12, 476, 625371618 } },
    { "order", "deep 564 order 6", { 32, 32, 32, 4, 0, 12, 476, 156287023 } },
    { "order", "deep 564 order 7", { 32, 32, 32, 9, 0, 12, 476, 2043798022 } },
    { "order", "deep 564 order 8", { 32, 32, 32, 3, 0, 12, 476, 1658455291 } },
    { "order", "deep 564 order 9", { 32, 32, 32, 8, 0, 12, 476, 1683295614 } },
    { "order", "random 564 sample 0", { 32, 32, 32, 4, 0, 12, 525, 437362931 } },
    { "order", "random 564 sample 1", { 32, 32, 32, 18, 0, 12, 512, 987937163 } },
    { "order", "random 564 sample 2", { 32, 32, 32, 12, 0, 12, 508, 1555140249 } },
    { "order", "random 564 sample 3", { 32, 32, 32, 23, 0, 12, 520, 1788068413 } },
    { "order", "random 564 sample 4", { 32, 32, 32, 20, 0, 12, 525, 1671298087 } },
    { "order", "random 564 sample 5", { 32, 32, 32, 0, 0, 10, 496, 70178588 } },
    { "order", "random 564 sample 6", { 32, 32, 32, 6, 0, 12, 518, 244457256 } },
    { "order", "random 564 sample 7", { 32, 32, 32, 32, 0, 12, 507, 1687805824 } },
    { "order", "random 564 sample 8", { 32, 32, 32, 7, 0, 12, 523, 861346651 } },
    { "order", "random 564 sample 9", { 32, 32, 32, 5, 0, 12, 515, 790471608 } },
    { "order robust", "shallow 564 order 0", { 32, 32, 32, 0, 0, 10, 239, 1744783872 } },
    { "order robust", "shallow 564 order 1", { 32, 32, 32, 0, 0, 10, 239, 1326559143 } },
    { "order robust", "shallow 564 order 2", { 32, 32, 32, 1, 0, 12, 239, 1578285104 } },
    { "order robust", "shallow 564 order 3", { 32, 32, 32, 0, 0, 10, 239, 1249222658 } },
    { "order robust", "shallow 564 order 4", { 32, 32, 32, 0, 0, 10, 239, 1111326942 } },
    { "order robust", "shallow 564 order 5", { 32, 32, 32, 0, 0, 10, 239, 1861868352 } },
    { "order robust", "shallow 564 order 6", { 32, 32, 32, 0, 0, 10, 239, 1312609162 } },
    { "order robust", "shallow 564 order 7", { 32, 32, 32, 0, 0, 10, 239, 2057842101 } },
    { "order robust", "shallow 564 order 8", { 32, 32, 32, 0, 0, 10, 239, 366185903 } },
    { "order robust", "shallow 564 order 9", { 32, 32, 32, 0, 0, 10, 239, 1846215458 } },
    { "order robust", "strided 564 order 0", { 32, 32, 32, 20, 0, 12, 517, 1224366754 } },
    { "order robust", "strided 564 order 1", { 32, 32, 32, 1, 0, 12, 517, 1633315448 } },
    { "order robust", "strided 564 order 2", { 32, 32, 32, 27, 0, 12, 517, 1958517209 } },
    { "order robust", "strided 564 order 3", { 32, 32, 32, 3, 0, 12, 517, 741859757 } },
    { "order robust", "strided 564 order 4", { 32, 32, 32, 2, 0, 12, 517, 952887019 } },
    { "order robust", "strided 564 order 5", { 32, 32, 32, 0, 0, 10, 517, 679117882 } },
    { "order robust", "strided 564 order 6", { 32, 32, 32, 6, 0, 12, 517, 1839610055 } },
    { "order robust", "strided 564 order 7", { 32, 32, 32, 1, 0, 12, 517, 750063431 } },
    { "order robust", "strided 564 order 8", { 32, 32, 32, 16, 0, 12, 517, 1046173647 } },
    { "order robust", "strided 564 order 9", { 32, 32, 32, 0, 0, 10, 517, 2054605076 } },
    { "order robust", "deep 564 order 0", { 32, 32, 32, 0, 0, 10, 476, 833435149 } },
    { "order robust", "deep 564 order 1", { 32, 32, 32, 0, 0, 10, 476, 1546876503 } },
    { "order robust", "deep 564 order 2", { 32, 32, 32, 0, 0, 10, 476, 1198045669 } },
    { "order robust", "deep 564 order 3", { 32, 32, 32, 1, 0, 12, 476, 695588473 } },
    { "order robust", "deep 564 order 4", { 32, 32, 32, 3, 0, 12, 476, 951715697 } },
    { "order robust", "deep 564 order 5", { 32, 32, 32, 2, 0, 12, 476, 315270401 } },
    { "order robust", "deep 564 order 6", { 32, 32, 32, 1, 0, 12, 476, 641504798 } },
    { "order robust", "deep 564 order 7", { 32, 32, 32, 7, 0, 12, 476, 510543549 } },
    { "order robust", "deep 564 order 8", { 32, 32, 32, 2, 0, 12, 476, 984146484 } },
    { "order robust", "deep 564 order 9", { 32, 32, 32, 4, 0, 12, 476, 249685518 } },
    { "order robust", "random 564 sample 0", { 32, 32, 32, 1, 0, 12, 525, 983195755 } },
    { "order robust", "random 564 sample 1", { 32, 32, 32, 11, 0, 12, 512, 1609335378 } },
    { "order robust", "random 564 sample 2", { 32, 32, 32, 12, 0, 12, 508, 1247185189 } },
    { "order robust", "random 564 sample 3", { 32, 32, 32, 22, 0, 12, 520, 765604702 } },
    { "order robust", "random 564 sample 4", { 32, 32, 32, 12, 0, 12, 525, 356465794 } },
    { "order robust", "random 564 sample 5", { 32, 32, 32, 0, 0, 10, 496, 1545870161 } },
    { "order robust", "random 564 sample 6", { 32, 32, 32, 6, 0, 12, 518, 1134683351 } },
    { "order robust", "random 564 sample 7", { 32, 32, 32, 32, 0, 12, 507, 357012084 } },
    { "order robust", "random 564 sample 8", { 32, 32, 32, 5, 0, 12, 523, 1548331935 } },
    { "order robust", "random 564 sample 9", { 32, 32, 32, 1, 0, 12, 515, 598157249 } },
/* PINS END */
    { NULL, NULL, { 0, 0, 0, 0, 0, 0, 0, 0 } }
};

#define N_SECTIONS 8

static const char *pin_sec_name[N_SECTIONS];
static int         pin_sec_rows[N_SECTIONS];
static int         pin_sec_bad[N_SECTIONS];
static int         pin_n_sec = 0;
static int         pins_emit = 0;

static void
pin (
    const char *section,
    const char *name,
    const Run  *r)
{
    long v[8];
    int  s;
    int  i;
    int  found = 0;
    int  bad = 0;

    for (i = 0; i < 5; i++) v[i] = r->num[3 + i];
    v[5] = r->max_xor;
    v[6] = r->axes;
    v[7] = winners_sum(r);
    if (pins_emit) {
        fprintf(stderr, "PIN    { \"%s\", \"%s\", { %ld, %ld, %ld, %ld, %ld,"
            " %ld, %ld, %ld } },\n", section, name, v[0], v[1], v[2], v[3],
            v[4], v[5], v[6], v[7]);
    }
    for (s = 0; s < pin_n_sec; s++) {
        if (strcmp(pin_sec_name[s], section) == 0) break;
    }
    if (s == pin_n_sec) {
        if (pin_n_sec >= N_SECTIONS) die("pin sections");
        pin_sec_name[pin_n_sec++] = section;
    }
    pin_sec_rows[s]++;
    for (i = 0; PINS[i].section != NULL; i++) {
        int j;

        if (strcmp(PINS[i].section, section) != 0
            || strcmp(PINS[i].name, name) != 0)
            continue;
        found = 1;
        for (j = 0; j < 8; j++) {
            if (PINS[i].v[j] >= 0 && PINS[i].v[j] != v[j]) bad = 1;
        }
    }
    if (!found || bad) {
        pin_sec_bad[s]++;
        printf("  PIN %s: %s / %s = %ld %ld %ld %ld %ld max %ld axes %ld"
            " sum %ld\n", found ? "MISMATCH" : "MISSING", section, name, v[0],
            v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
    }
}

static void
pin_check (
    const char *section,
    const char *what)
{
    char msg[256];
    int  s;

    for (s = 0; s < pin_n_sec; s++) {
        if (strcmp(pin_sec_name[s], section) == 0) break;
    }
    if (s == pin_n_sec) return;
    sprintf(msg, "%s: %d searches equal the pinned table", what,
        pin_sec_rows[s]);
    check(msg, pin_sec_bad[s] == 0);
}

/* ================================================================
 * Printing
 * ================================================================ */

static void
print_header (void)
{
    printf("  %-13s | Size | Axes | XOR6 | XOR8 | XOR10 | XOR12 | XOR14 |"
        " Max\n", "Subset");
    printf("  --------------|------|------|------|------|-------|-------|"
        "-------|----\n");
}

static void
print_row (
    const char *name,
    int         size,
    const Run  *r,
    int         nmax)
{
    printf("  %-13s | %4d | %4d%s| %4ld | %4ld | %5ld | %5ld | ", name,
        size, r->axes, r->trunc ? "*" : " ", r->num[3], r->num[4],
        r->num[5], r->num[6]);
    if (nmax >= 7) printf("%5ld |", r->num[7]);
    else printf("   -- |");
    printf(" %3d\n", r->max_xor);
}

/* depth histogram of the winners at one level; returns the entry count */
static int
level_hist (
    const Conf *c,
    const Run  *r,
    int         lv,
    int         hist[F_ROUNDS])
{
    int cnt = 0;
    int w;
    int j;

    memset(hist, 0, sizeof(int) * F_ROUNDS);
    for (w = 0; w < r->nwin[lv]; w++) {
        for (j = 0; j < lv; j++) {
            hist[f_full_depth[c->idx[r->win[lv][w][j]]]]++;
            cnt++;
        }
    }
    return cnt;
}

/* D82 Part B: winner depth histogram per level; returns the first level
 * whose winners use a depth-8 entry (0 none) */
static int
print_depths (
    const Conf *c,
    const Run  *r,
    int         nmax)
{
    int lv;
    int first8 = 0;

    printf("  Level | #Win | MinD | MaxD | MeanD | Depth histogram\n");
    printf("  ------|------|------|------|-------|----------------\n");
    for (lv = 3; lv <= nmax; lv++) {
        int    hist[F_ROUNDS];
        int    min_d = -1;
        int    max_d = 0;
        int    cnt;
        double sum = 0.0;
        int    j;

        if (r->nwin[lv] == 0) {
            printf("  XOR%-2d | %4d |  --  |  --  |  --   | --\n", 2 * lv, 0);
            continue;
        }
        cnt = level_hist(c, r, lv, hist);
        for (j = 0; j < F_ROUNDS; j++) {
            if (hist[j] == 0) continue;
            if (min_d < 0) min_d = j;
            max_d = j;
            sum += (double)(j * hist[j]);
        }
        if (hist[8] > 0 && first8 == 0) first8 = lv;
        printf("  XOR%-2d | %4d | %4d | %4d | %5.2f | ", 2 * lv, r->nwin[lv],
            min_d, max_d, sum / (double)cnt);
        for (j = 0; j < F_ROUNDS; j++) {
            if (hist[j] > 0) printf("d%d:%d ", j, hist[j]);
        }
        printf("\n");
    }
    return first8;
}

/* ================================================================
 * Seeded orders (own LCG, deterministic on every platform)
 * ================================================================ */

static unsigned long lcg_state;

static unsigned long
lcg_next (void)
{
    lcg_state = (lcg_state * 1103515245UL + 12345UL) & 0xffffffffUL;
    return lcg_state;
}

static unsigned long
lcg_below (
    unsigned long n)
{
    unsigned long hi = lcg_next() >> 8;
    unsigned long lo = lcg_next() >> 8;

    return ((hi << 24) | lo) % n;
}

static void
shuffle (
    int *a,
    int  n)
{
    int i;

    for (i = n - 1; i > 0; i--) {
        int j = (int)lcg_below((unsigned long)(i + 1));
        int t = a[i];

        a[i] = a[j];
        a[j] = t;
    }
}

/* n of 0..N-1, ascending (selection sampling) */
static void
sample_sorted (
    int *a,
    int  n,
    int  total)
{
    int i;
    int got = 0;

    for (i = 0; i < total && got < n; i++) {
        if ((int)lcg_below((unsigned long)(total - i)) < n - got) {
            a[got++] = i;
        }
    }
}

/* ================================================================
 * Results of every search, and the jobs that fill them
 *
 * Every search is a JOB. The parent runs each job in a child process
 * (its own binary, "--opus N"), at most D125_OPERARII (8) at once,
 * longest first, and reads the child's Run back as one text line. The
 * printing below reads only these arrays, in a fixed order, so the
 * output does not depend on which child finishes first. D125_SERIAL=1
 * runs the same jobs in this process, in table order.
 * ================================================================ */

#define N_ORDERS 10

static Run f_runs[MAX_CONF];
static Run f_d87_full;                 /* full catalog, D87 caps */
static Run e_runs[MAX_CONF][2][2];     /* [conf][mode][capped] */
static Run o_runs[2][3][N_ORDERS];      /* [mode][subset][order] */
static Run s_runs[2][N_ORDERS];         /* [mode][sample] */

static const char *const order_base[3] = { "round 5", "strided 564",
    "deep 564" };

/* subset s of Part E in seeded order o */
static void
order_indices (
    int  s,
    int  o,
    int *out)
{
    const Conf *b = conf_find(order_base[s]);

    memcpy(out, b->idx, sizeof(int) * (size_t)b->n);
    lcg_state = (unsigned long)(1000 * (s + 1) + o + 1);
    shuffle(out, b->n);
}

/* random 564-entry sample o of Part E, ascending */
static void
sample_indices (
    int  o,
    int *out)
{
    lcg_state = (unsigned long)(77 + o);
    sample_sorted(out, 564, F_QCAT);
}

#define J_FLOAT      0
#define J_FLOAT_D87  1
#define J_EXACT      2
#define J_ORDER      3
#define J_SAMPLE     4
#define MAX_JOBS     160

typedef struct {
    int  kind;
    int  conf;          /* J_FLOAT, J_EXACT */
    int  mode;          /* J_EXACT */
    int  capped;        /* J_EXACT */
    int  nmax;          /* J_EXACT */
    int  s;             /* J_ORDER: subset */
    int  o;             /* J_ORDER, J_SAMPLE: order / sample */
    long cost;          /* estimate, for longest-first */
} Job;

static Job jobs[MAX_JOBS];
static int n_jobs = 0;

static Job *
job_new (
    int  kind,
    long cost)
{
    Job *j;

    if (n_jobs >= MAX_JOBS) die("too many jobs");
    j = &jobs[n_jobs++];
    memset(j, 0, sizeof(*j));
    j->kind = kind;
    j->cost = cost;
    return j;
}

/* the job table: the same in parent and child (configurations first) */
static void
jobs_build (
    int nmax)
{
    int i;
    int m;
    int t;
    int s;
    int o;

    for (i = 0; i < n_conf; i++) {
        Job *j;

        if (celer && conf[i].n > 600) continue;
        j = job_new(J_FLOAT, 3L * conf[i].n);
        j->conf = i;
    }
    if (!celer) job_new(J_FLOAT_D87, 3L * F_QCAT);
    for (m = 0; m < 2; m++) {
        for (t = 1; t >= 0; t--) {
            for (i = 0; i < n_conf; i++) {
                Job *j;
                int  nm = m == MODE_RULE && (i == 7 || i == 8) ? nmax : 6;

                if (celer && conf[i].n > 600) continue;
                if (strcmp(conf[i].name, "non-null") == 0 && t == 1) continue;
                j = job_new(J_EXACT, (long)conf[i].n
                    * (nm == 7 ? (t ? 4 : 12) : 1));
                j->conf = i;
                j->mode = m;
                j->capped = t;
                j->nmax = nm;
            }
        }
    }
    for (m = 0; m < 2; m++) {
        for (s = 0; s < 3; s++) {
            for (o = 0; o < (celer ? 2 : N_ORDERS); o++) {
                Job *j = job_new(J_ORDER, 564L);

                j->mode = m;
                j->s = s;
                j->o = o;
            }
        }
        for (o = 0; o < (celer ? 2 : N_ORDERS); o++) {
            Job *j = job_new(J_SAMPLE, 564L);

            j->mode = m;
            j->o = o;
        }
    }
}

static Run *
job_run (
    const Job *j)
{
    switch (j->kind) {
    case J_FLOAT:     return &f_runs[j->conf];
    case J_FLOAT_D87: return &f_d87_full;
    case J_EXACT:     return &e_runs[j->conf][j->mode][j->capped];
    case J_ORDER:     return &o_runs[j->mode][j->s][j->o];
    default:          return &s_runs[j->mode][j->o];
    }
}

static void
job_execute (
    const Job *j)
{
    static int tmp[F_QCAT];
    Run       *r = job_run(j);

    switch (j->kind) {
    case J_FLOAT: {
        int is_d87 = strcmp(conf[j->conf].name, "non-null") == 0;

        f_capacity(conf[j->conf].idx, conf[j->conf].n, is_d87 ? 2048 : 512,
            is_d87 ? 65536 : 16384, r);
        r->trunc = !is_d87 && r->axes == 512 ? 1 : 0;
        break;
    }
    case J_FLOAT_D87: {
        const Conf *full = conf_find("round 8");

        f_capacity(full->idx, full->n, 2048, 65536, r);
        break;
    }
    case J_EXACT:
        e_capacity(conf[j->conf].idx, conf[j->conf].n, j->mode, j->capped,
            j->nmax, r);
        break;
    case J_ORDER:
        order_indices(j->s, j->o, tmp);
        e_capacity(tmp, conf_find(order_base[j->s])->n, j->mode, 0, 6, r);
        break;
    default:
        sample_indices(j->o, tmp);
        e_capacity(tmp, 564, j->mode, 0, 6, r);
        break;
    }
}

/* a Run as one line of numbers (child -> parent) */
static void
run_print (
    const Run *r)
{
    int lv;
    int w;
    int j;

    printf("RUN");
    for (lv = 0; lv < N_LEVELS; lv++) printf(" %ld %d", r->num[lv], r->nwin[lv]);
    for (lv = 0; lv < N_LEVELS; lv++) {
        for (w = 0; w < r->nwin[lv]; w++) {
            printf(" %d", r->wk[lv][w]);
            for (j = 0; j < lv; j++) printf(" %d", r->win[lv][w][j]);
        }
    }
    printf(" %d %d %d %d %ld %ld %ld %.3f\n", r->max_xor, r->axes, r->trunc,
        r->angles, r->st_filter, r->st_exact, r->st_tied, r->secs);
}

static int
run_parse (
    const char *text,
    Run        *r)
{
    const char *p = text;
    char       *end;
    int         lv;
    int         w;
    int         j;

#define NEXT_L(dst) do { long v_ = strtol(p, &end, 10); \
        if (end == p) return 0; (dst) = v_; p = end; } while (0)
#define NEXT_I(dst) do { long v_ = strtol(p, &end, 10); \
        if (end == p) return 0; (dst) = (int)v_; p = end; } while (0)

    run_clear(r);
    if (strncmp(p, "RUN", 3) != 0) return 0;
    p += 3;
    for (lv = 0; lv < N_LEVELS; lv++) {
        NEXT_L(r->num[lv]);
        NEXT_I(r->nwin[lv]);
        if (r->nwin[lv] < 0 || r->nwin[lv] > MAX_WIN) return 0;
    }
    for (lv = 0; lv < N_LEVELS; lv++) {
        for (w = 0; w < r->nwin[lv]; w++) {
            NEXT_I(r->wk[lv][w]);
            for (j = 0; j < lv; j++) NEXT_I(r->win[lv][w][j]);
        }
    }
    NEXT_I(r->max_xor);
    NEXT_I(r->axes);
    NEXT_I(r->trunc);
    NEXT_I(r->angles);
    NEXT_L(r->st_filter);
    NEXT_L(r->st_exact);
    NEXT_L(r->st_tied);
    r->secs = strtod(p, &end);
    if (end == p) return 0;
#undef NEXT_L
#undef NEXT_I
    return 1;
}

static const char *argv0 = NULL;

/* every job in a child, at most 'workers' at once, longest first */
static void
jobs_run_parallel (
    int workers)
{
    static int        order[MAX_JOBS];
    static Processus *slot_p[64];
    static Piscina   *slot_pool[64];
    static int        slot_job[64];
    static char       arg_n[MAX_JOBS][16];
    int               next = 0;
    int               running = 0;
    int               i;
    int               k;

    if (workers > 64) workers = 64;
    for (i = 0; i < n_jobs; i++) order[i] = i;
    for (i = 1; i < n_jobs; i++) {          /* stable: cost descending */
        int x = order[i];

        for (k = i; k > 0 && jobs[order[k - 1]].cost < jobs[x].cost; k--) {
            order[k] = order[k - 1];
        }
        order[k] = x;
    }
    for (i = 0; i < workers; i++) slot_p[i] = NULL;
    while (next < n_jobs || running > 0) {
        int reaped = 0;

        for (i = 0; i < workers; i++) {
            if (slot_p[i] == NULL && next < n_jobs) {
                const char *args[4];
                int         jid = order[next++];

                sprintf(arg_n[jid], "%d", jid);
                args[0] = argv0;
                args[1] = "--opus";
                args[2] = arg_n[jid];
                args[3] = NULL;
                slot_pool[i] = piscina_generare_dynamicum("d125-opus", 1 << 16);
                if (slot_pool[i] == NULL) die("piscina (opus)");
                slot_p[i] = processus_incipere((const char *const *)args, 0,
                    slot_pool[i]);
                if (slot_p[i] == NULL) die("processus_incipere");
                slot_job[i] = jid;
                running++;
            }
        }
        for (i = 0; i < workers; i++) {
            ProcessusResultus res;
            char             *text;

            if (slot_p[i] == NULL
                || processus_pulsare(slot_p[i]) != PROCESSUS_PARATUS)
                continue;
            res = processus_metere(slot_p[i]);
            if (!res.successus || res.codex_exitus != 0) {
                printf("  FATAL: job %d: %s, exit %d, signal %d\n",
                    slot_job[i], processus_error_nomen(res.error),
                    (int)res.codex_exitus, (int)res.signum);
                fwrite(res.erratum.datum, 1, (size_t)res.erratum.mensura,
                    stdout);
                exit(1);
            }
            text = (char *)malloc((size_t)res.effusio.mensura + 1);
            if (text == NULL) die("malloc");
            memcpy(text, res.effusio.datum, (size_t)res.effusio.mensura);
            text[res.effusio.mensura] = '\0';
            if (!run_parse(text, job_run(&jobs[slot_job[i]]))) {
                printf("  FATAL: job %d: unreadable result\n", slot_job[i]);
                exit(1);
            }
            free(text);
            piscina_destruere(slot_pool[i]);
            slot_p[i] = NULL;
            running--;
            reaped++;
        }
        if (reaped == 0) fenestra_dormire(20000);
    }
}


int
main (
    int    argc,
    char **argv)
{
    static const int D82_CENSUS[9] = { 5, 12, 34, 76, 148, 289, 576, 1152,
        1804 };
    static const int D81_MAX[9] = { 6, 6, 8, 8, 10, 10, 12, 12, 12 };
    static const char *const mode_name[2] = { "rule", "robust" };
    static const char *const table_name[2] = { "uncapped", "capped" };
    static const char *const sec_name[2][2] = {
        { "rule uncapped", "rule capped" },
        { "robust uncapped", "robust capped" }
    };
    char msg[256];
    int  i;
    int  j;
    int  m;
    int  t;
    int  nmax;
    int  n_null = 0;
    int  depth_hist[F_ROUNDS];

    celer = getenv("D125_CELER") != NULL;
    piscina = piscina_generare_dynamicum("d125", 1 << 24);
    if (piscina == NULL) die("piscina");
    nmax = 7;
    argv0 = argv[0];

    /* setup, the same in parent and child: catalogs, subsets, jobs */
    f_init_su2(M_PI / 6.0);
    f_build_closure();
    e_build_catalog();
    for (i = 0; i < F_QCAT; i++) {
        if (catalogus_nullum(e_full, (i32)i)) n_null++;
    }
    for (i = 0; i < N_PREFIX; i++) {
        Conf *c;

        sprintf(msg, "round %d", i);
        c = conf_new(msg, f_round_end[i]);
        for (j = 0; j < c->n; j++) c->idx[j] = j;
    }
    {
        static const int sizes[2] = { 127, 564 };

        for (m = 0; m < 2; m++) {
            Conf *c;
            int   n = sizes[m];

            sprintf(msg, "strided %d", n);
            c = conf_new(msg, n);
            for (j = 0; j < n; j++) {
                c->idx[j] = (int)((long)j * (long)F_QCAT / (long)n);
            }
            sprintf(msg, "deep %d", n);
            c = conf_new(msg, n);
            for (j = 0; j < n; j++) c->idx[j] = F_QCAT - n + j;
        }
    }
    {
        Conf *c = conf_new("non-null", F_QCAT - n_null);
        int   k = 0;

        for (i = 0; i < F_QCAT; i++) {
            if (!catalogus_nullum(e_full, (i32)i)) c->idx[k++] = i;
        }
    }
    jobs_build(nmax);

    /* child: one job, its Run on stdout */
    if (argc == 3 && strcmp(argv[1], "--opus") == 0) {
        int jid = atoi(argv[2]);

        if (jid < 0 || jid >= n_jobs) die("--opus: no such job");
        job_execute(&jobs[jid]);
        run_print(job_run(&jobs[jid]));
        return 0;
    }

    tempora = getenv("D125_TEMPORA") != NULL;
    pins_emit = getenv("D125_PINS") != NULL;
    if (getenv("D125_VICTORES") != NULL) {
        victores = fopen(getenv("D125_VICTORES"), "w");
        if (victores == NULL) die("D125_VICTORES");
    }
    {
        s64 t0 = fenestra_tempus_ms();

        if (getenv("D125_SERIAL") != NULL) {
            for (i = 0; i < n_jobs; i++) job_execute(&jobs[i]);
        } else {
            const char *w = getenv("D125_OPERARII");

            jobs_run_parallel(w != NULL && atoi(w) > 0 ? atoi(w) : 8);
        }
        if (tempora) {
            fprintf(stderr, "  [%d jobs: %.1f s wall]\n", n_jobs,
                (double)(fenestra_tempus_ms() - t0) / 1000.0);
        }
    }

    printf("KNOTAPEL DEMO 125: D82's Crossing Depth, Exactly\n");
    printf("================================================================\n");

    /* ============================================================
     * Part A: the catalog
     * ============================================================ */
    printf("\n=== Part A: the zeta_12 catalog, float and exact ===\n\n");
    printf("  float: %d entries in %d rounds; exact: %d entries in %d"
        " rounds, truncated %s\n", f_full_size, f_rounds,
        (int)catalogus_numerus(e_full), (int)catalogus_gradus(e_full),
        catalogus_truncatus(e_full) ? "yes" : "no");
    check("float and exact catalogs have 4096 entries",
        f_full_size == F_QCAT && (int)catalogus_numerus(e_full) == F_QCAT);
    check("exact catalog is marked truncated (round 8 cut by MAX_QCAT)",
        catalogus_truncatus(e_full) ? 1 : 0);
    {
        int same = 1;
        int first_bad = -1;

        for (i = 0; i < F_QCAT && same; i++) {
            Quaternio q = catalogus_elementum(e_full, (i32)i);

            if (fabs(e_component(q, 0) - f_full[i].a) > 1e-12
                || fabs(e_component(q, 1) - f_full[i].b) > 1e-12
                || fabs(e_component(q, 2) - f_full[i].c) > 1e-12
                || fabs(e_component(q, 3) - f_full[i].d) > 1e-12
                || (int)catalogus_profunditas(e_full, (i32)i)
                    != f_full_depth[i]) {
                same = 0;
                first_bad = i;
            }
        }
        sprintf(msg, "exact catalog = D82's float catalog entry for entry"
            " (values within 1e-12, same order, same depths; first"
            " difference %d)", first_bad);
        check(msg, same);
    }
    memset(depth_hist, 0, sizeof(depth_hist));
    for (i = 0; i < F_QCAT; i++) {
        depth_hist[catalogus_profunditas(e_full, (i32)i)]++;
    }
    printf("\n  Depth | Count | Cumulative (= D81 round prefix)\n");
    printf("  ------|-------|-----------\n");
    {
        int cum = 0;
        int ok = 1;

        for (i = 0; i < 9; i++) {
            cum += depth_hist[i];
            printf("  %5d | %5d | %5d\n", i, depth_hist[i], cum);
            ok = ok && depth_hist[i] == D82_CENSUS[i]
                && cum == f_round_end[i];
        }
        check("depth census = D82 Part A (5 12 34 76 148 289 576 1152"
            " 1804), prefixes = D81's rounds", ok);
    }
    {
        int f_null = 0;

        for (i = 0; i < F_QCAT; i++) {
            double a = f_full[i].a;

            if ((a < 0 ? -a : a) < 1e-10) f_null++;
        }
        printf("\n  null entries (Re q = 0): exact %d, float (D87's"
            " |a| < 1e-10) %d\n", n_null, f_null);
        check("exact nulls = D87's float nulls (121)",
            n_null == f_null && n_null == 121);
    }

    /* ============================================================
     * Part B: D81 / D82 / D87 float, replicated
     * ============================================================ */
    printf("\n=== Part B: the float search (D81/D82/D87), replicated ===\n\n");
    printf("  D81/D82 caps: MAX_DIR 512, MAX_ACT 16384; D87: 2048, 65536."
        " '*' = axis table cut.\n\n");
    print_header();
    for (i = 0; i < n_conf; i++) {
        if (celer && conf[i].n > 600) continue;
        print_row(conf[i].name, conf[i].n, &f_runs[i], 6);
        tempus(conf[i].name, &f_runs[i]);
        pin("float", conf[i].name, &f_runs[i]);
    }
    if (!celer) {
        Run  *d87_full = &f_d87_full;
        Conf *full = conf_find("round 8");

        print_row("full (D87)", full->n, d87_full, 6);
        tempus("full (D87)", d87_full);
        pin("float", "full (D87)", d87_full);
        {
            int ok = 1;

            for (i = 0; i < N_PREFIX; i++) {
                ok = ok && f_runs[i].max_xor == D81_MAX[i];
            }
            check("D81's max-XOR ladder reproduced: 6 6 8 8 10 10 12 12 12",
                ok);
        }
        /* D82 Part D, as recorded */
        {
            const Run *s127 = &f_runs[3];
            const Run *t127 = &f_runs[conf_at("strided 127")];
            const Run *d127 = &f_runs[conf_at("deep 127")];
            const Run *s564 = &f_runs[5];
            const Run *t564 = &f_runs[conf_at("strided 564")];
            const Run *d564 = &f_runs[conf_at("deep 564")];

            check("D82 Part D reproduced: shallow/strided/deep 127 ="
                " 32 32 0 0 / 32 32 26 0 / 32 32 32 0",
                s127->num[5] == 0 && t127->num[5] == 26
                && t127->num[6] == 0 && d127->num[5] == 32
                && d127->num[6] == 0);
            check("D82 Part D reproduced: shallow/strided/deep 564 XOR12"
                " = 0 / 0 / 19",
                s564->num[6] == 0 && t564->num[6] == 0
                && d564->num[6] == 19);
            check("D82 Part D vocabulary reproduced: dirs 46 125 119 239"
                " 512 476, angles 10 37 19 16 43 19",
                s127->axes == 46 && t127->axes == 125 && d127->axes == 119
                && s564->axes == 239 && t564->axes == 512
                && d564->axes == 476 && s127->angles == 10
                && t127->angles == 37 && d127->angles == 19
                && s564->angles == 16 && t564->angles == 43
                && d564->angles == 19);
        }
        check("D87 full catalog (2048/65536): 2043 directions, XOR12",
            d87_full->axes == 2043 && d87_full->max_xor == 12);
        check("D87 non-null (2048/65536): XOR12",
            f_runs[conf_at("non-null")].max_xor == 12);
    }

    pin_check("float", "Part B float replica");

    /* ============================================================
     * Part C: the same search on exact cells
     * ============================================================ */
    printf("\n=== Part C: D82's search on EXACT cells (cellae) ===\n\n");
    printf("  capped = D82's semantics (first 512 axes, k skipped when"
        " k (axes + 1) > 16384);\n");
    printf("  uncapped = every axis, every k. Rule mode at rounds 7 and 8"
        " is extended to XOR14\n  (the depth law predicts XOR13-14 there);"
        " '--' = not searched.\n");
    for (m = 0; m < 2; m++) {
        for (t = 1; t >= 0; t--) {
            printf("\n  --- exact %s, %s ---\n\n", mode_name[m],
                table_name[t]);
            print_header();
            for (i = 0; i < n_conf; i++) {
                Run *r = &e_runs[i][m][t];
                int  nm = m == MODE_RULE && (i == 7 || i == 8) ? nmax : 6;

                if (celer && conf[i].n > 600) continue;
                if (strcmp(conf[i].name, "non-null") == 0 && t == 1) continue;
                print_row(conf[i].name, conf[i].n, r, nm);
                sprintf(msg, "%s %s %s", conf[i].name, mode_name[m],
                    table_name[t]);
                tempus(msg, r);
                victores_write(&conf[i], mode_name[m], table_name[t], r, 3);
                pin(sec_name[m][t], conf[i].name, r);
            }
            sprintf(msg, "Part C exact %s, %s", mode_name[m], table_name[t]);
            pin_check(sec_name[m][t], msg);
        }
    }
    printf("\n  Cell decisions (certified float filter / exact / exact"
        " with a tie):\n\n");
    printf("  %-13s | %-6s | %-8s | %10s | %9s | %9s\n", "Subset", "Mode",
        "Table", "filter", "exact", "tied");
    for (i = 0; i < n_conf; i++) {
        for (m = 0; m < 2; m++) {
            for (t = 1; t >= 0; t--) {
                const Run *r = &e_runs[i][m][t];

                if (r->axes == 0) continue;
                printf("  %-13s | %-6s | %-8s | %10ld | %9ld | %9ld\n",
                    conf[i].name, mode_name[m], table_name[t], r->st_filter,
                    r->st_exact, r->st_tied);
            }
        }
    }
    printf("\n  Float vs exact rule (same caps): identical winner lists?\n\n");
    for (i = 0; i < n_conf; i++) {
        int lvl;
        int only_k;
        int same;
        int tt = strcmp(conf[i].name, "non-null") == 0 ? 0 : 1;

        if (celer && conf[i].n > 600) continue;
        same = runs_same_winners(&f_runs[i], &e_runs[i][MODE_RULE][tt], 6,
            &lvl, &only_k);
        if (same) {
            printf("  %-13s same (every winner, every k)\n", conf[i].name);
        } else {
            printf("  %-13s first difference at XOR%d (%s)\n", conf[i].name,
                2 * lvl, only_k ? "same sets, a different first k"
                : "different sets");
        }
    }

    /* ============================================================
     * Part D: winner depths (D82 Part B)
     * ============================================================ */
    printf("\n=== Part D: winner depths, full catalog (D82 Part B) ===\n");
    if (!celer) {
        static const int REC[4][9] = {
            { 66, 10, 20, 0, 0, 0, 0, 0, 0 },
            { 96, 2, 16, 14, 0, 0, 0, 0, 0 },
            { 96, 32, 0, 10, 11, 11, 0, 0, 0 },
            { 96, 32, 0, 2, 19, 15, 9, 15, 4 }
        };
        const Conf *full = &conf[8];
        int         ok = 1;
        int         lv;
        int         first8;

        printf("\n  float (D82's run):\n\n");
        first8 = print_depths(full, &f_runs[8], 6);
        for (lv = 3; lv <= 6; lv++) {
            int hist[F_ROUNDS];

            level_hist(full, &f_runs[8], lv, hist);
            for (j = 0; j < 9; j++) ok = ok && hist[j] == REC[lv - 3][j];
        }
        check("D82 Part B depth histograms reproduced (float)", ok);
        check("float: XOR12 is the first level with a depth-8 entry"
            " (as recorded)", first8 == 6);
        for (m = 0; m < 2; m++) {
            for (t = 1; t >= 0; t--) {
                printf("\n  exact %s, %s:\n\n", mode_name[m], table_name[t]);
                first8 = print_depths(full, &e_runs[8][m][t],
                    m == MODE_RULE ? nmax : 6);
                if (first8 > 0) {
                    printf("  first level with a depth-8 entry: XOR%d\n",
                        2 * first8);
                } else {
                    printf("  no winner uses a depth-8 entry\n");
                }
            }
        }
    }

    /* ============================================================
     * Part E: order control
     * ============================================================ */
    printf("\n=== Part E: order control (exact rule and robust, uncapped,"
        " to XOR12) ===\n\n");
    printf("  D82's greedy takes triples from the first 100 entries and"
        " extends the first\n  32 winners of each level, so its result"
        " depends on entry order. Each 564\n  subset in %d seeded orders;"
        " then %d random 564-entry samples (ascending\n  catalog order)"
        " from all depths.\n", celer ? 2 : N_ORDERS, celer ? 2 : N_ORDERS);
    printf("  A shuffle also reorders the axis table, and the rule breaks"
        " axis ties by\n  first maximum: under the rule, search order and"
        " cell semantics change\n  together. Robust mode takes no tie"
        " choice, so its rows isolate the order.\n");
    {
        static const char *const label[3] = { "shallow 564", "strided 564",
            "deep 564" };
        static const char *const order_sec[2] = { "order", "order robust" };
        static int tmp[F_QCAT];
        int        n_ord = celer ? 2 : N_ORDERS;
        int        s;
        int        o;

        for (m = 0; m < 2; m++) {
            printf("\n  --- %s ---\n\n", mode_name[m]);
            printf("  %-13s | max XOR per order          | XOR12 winners"
                " per order\n", "Subset");
            for (s = 0; s < 3; s++) {
                const Conf *b = conf_find(order_base[s]);
                int         reach = 0;

                for (o = 0; o < n_ord; o++) {
                    Conf tc;
                    Run *r = &o_runs[m][s][o];

                    order_indices(s, o, tmp);
                    sprintf(tc.name, "%s order %d", label[s], o);
                    tc.n = b->n;
                    tc.idx = tmp;
                    victores_write(&tc, mode_name[m], "uncapped", r, 6);
                    tempus(tc.name, r);
                    pin(order_sec[m], tc.name, r);
                    if (r->max_xor == 12) reach++;
                }
                printf("  %-13s | ", label[s]);
                for (o = 0; o < n_ord; o++) {
                    printf("%d ", o_runs[m][s][o].max_xor);
                }
                printf("| ");
                for (o = 0; o < n_ord; o++) {
                    printf("%ld ", o_runs[m][s][o].num[6]);
                }
                printf("(%d/%d reach XOR12)\n", reach, n_ord);
            }
            printf("\n  random 564 | max XOR | XOR12 | mean depth | depth 6-8"
                " | depth-8 entries\n");
            for (o = 0; o < n_ord; o++) {
                Conf   tc;
                Run   *r = &s_runs[m][o];
                double md = 0.0;
                int    d8 = 0;
                int    d68 = 0;

                sample_indices(o, tmp);
                for (j = 0; j < 564; j++) {
                    md += (double)f_full_depth[tmp[j]];
                    if (f_full_depth[tmp[j]] == 8) d8++;
                    if (f_full_depth[tmp[j]] >= 6) d68++;
                }
                sprintf(tc.name, "random 564 sample %d", o);
                tc.n = 564;
                tc.idx = tmp;
                victores_write(&tc, mode_name[m], "uncapped", r, 6);
                tempus(tc.name, r);
                pin(order_sec[m], tc.name, r);
                printf("  sample %-3d | %7d | %5ld | %10.2f | %8.0f%% | %d\n",
                    o, r->max_xor, r->num[6], md / 564.0,
                    100.0 * (double)d68 / 564.0, d8);
            }
        }
    }

    pin_check("order", "Part E order control (rule)");
    pin_check("order robust", "Part E order control (robust)");

    /* ============================================================
     * Part F: D87, full vs non-null
     * ============================================================ */
    printf("\n=== Part F: D87, full vs non-null (uncapped; D87's own caps"
        " 2048 / 65536 do not bind) ===\n\n");
    if (!celer) {
        int nn = conf_at("non-null");

        print_header();
        print_row("full (float)", conf[8].n, &f_d87_full, 6);
        print_row("non-null (fl)", conf[nn].n, &f_runs[nn], 6);
        for (m = 0; m < 2; m++) {
            sprintf(msg, "full (%s)", m == 0 ? "r" : "rb");
            print_row(msg, conf[8].n, &e_runs[8][m][0],
                m == MODE_RULE ? nmax : 6);
            sprintf(msg, "non-null (%s)", m == 0 ? "r" : "rb");
            print_row(msg, conf[nn].n, &e_runs[nn][m][0], 6);
        }
    }

    /* ============================================================
     * Part G: the claims
     * ============================================================ */
    printf("\n=== Part G: the claims, per semantics ===\n\n");
    if (!celer) {
        static const char *const sem_name[5] = { "float (D82)",
            "rule capped", "rule uncapped", "robust capped",
            "robust uncap." };
        int sem;

        printf("  %-14s | max XOR by round (D81 ladder) | XOR14 r7 r8 |"
            " depth+6 holds at\n", "Semantics");
        for (sem = 0; sem < 5; sem++) {
            int r;

            printf("  %-14s | ", sem_name[sem]);
            for (r = 0; r < N_PREFIX; r++) {
                const Run *x = sem == 0 ? &f_runs[r]
                    : &e_runs[r][(sem - 1) / 2][(sem % 2)];

                printf("%2d ", x->max_xor);
            }
            if (sem == 0 || sem >= 3) {
                printf("| not run    | ");
            } else {
                printf("| %5ld %4ld | ",
                    e_runs[7][(sem - 1) / 2][sem % 2].num[7],
                    e_runs[8][(sem - 1) / 2][sem % 2].num[7]);
            }
            for (r = 0; r < N_PREFIX; r++) {
                const Run *x = sem == 0 ? &f_runs[r]
                    : &e_runs[r][(sem - 1) / 2][(sem % 2)];

                if (x->max_xor == r + 6) printf("%d ", r);
            }
            printf("\n");
        }
        printf("\n  %-14s | sh127 st127 dp127 | sh564 st564 dp564 | first"
            " prefix with XOR12 | non-null = full\n", "Semantics");
        for (sem = 0; sem < 5; sem++) {
            static const char *const cn[6] = { "round 3", "strided 127",
                "deep 127", "round 5", "strided 564", "deep 564" };
            int first12 = -1;
            int r;

            printf("  %-14s | ", sem_name[sem]);
            for (r = 0; r < 6; r++) {
                int        ci = conf_at(cn[r]);
                const Run *x = sem == 0 ? &f_runs[ci]
                    : &e_runs[ci][(sem - 1) / 2][(sem % 2)];

                printf("%5d ", x->max_xor);
                if (r == 2) printf("| ");
            }
            for (r = 0; r < N_PREFIX && first12 < 0; r++) {
                const Run *x = sem == 0 ? &f_runs[r]
                    : &e_runs[r][(sem - 1) / 2][(sem % 2)];

                if (x->max_xor >= 12) first12 = r;
            }
            printf("| round %d (%4d entries)     | ", first12,
                first12 >= 0 ? f_round_end[first12] : 0);
            if (sem == 0 || sem % 2 == 0) {
                int        nn = conf_at("non-null");
                const Run *a = sem == 0 ? &f_d87_full : &e_runs[8][(sem - 1) / 2][0];
                const Run *b = sem == 0 ? &f_runs[nn] : &e_runs[nn][(sem - 1) / 2][0];

                printf("%s (XOR%d vs XOR%d)\n",
                    a->max_xor == b->max_xor ? "yes" : "NO", b->max_xor,
                    a->max_xor);
            } else {
                printf("--\n");
            }
        }
    }

    if (!celer) {
        int sem;
        int ok;
        int reach[4];
        int s;
        int o;
        int lv;

        printf("\n  Claims checked (D82's search; never 'the capacity'):\n\n");
        ok = 1;
        for (t = 0; t < 2; t++) {
            for (i = 7; i <= 8; i++) {
                ok = ok && e_runs[i][MODE_RULE][t].num[6] > 0
                    && e_runs[i][MODE_RULE][t].num[7] == 0;
            }
        }
        check("depth law, rounds 7-8: D82's search, extended one level"
            " under the RULE only (from its first 32 XOR12 winners), finds"
            " XOR12 winners but no XOR14 (512 axes and all axes); robust"
            " not extended", ok);
        ok = 1;
        for (sem = 0; sem < 5; sem++) {
            for (i = 7; i <= 8; i++) {
                const Run *x = sem == 0 ? &f_runs[i]
                    : &e_runs[i][(sem - 1) / 2][sem % 2];

                ok = ok && x->max_xor == 12;
            }
        }
        check("depth law: D82's search finds max XOR 12 at rounds 6, 7 and"
            " 8 under every semantics (searched to XOR12; XOR14 under the"
            " rule only); depth + 6 would be 13, 14 at rounds 7, 8", ok
            && f_runs[6].max_xor == 12);
        check("round 1 reaches XOR8 under the exact rule (3 winners) but not"
            " robustly: decided by exact ties",
            e_runs[1][MODE_RULE][1].num[4] == 3
            && e_runs[1][MODE_RULE][0].num[4] == 3
            && e_runs[1][MODE_ROBUST][1].num[4] == 0
            && e_runs[1][MODE_ROBUST][0].num[4] == 0);
        ok = 1;
        for (sem = 0; sem < 5; sem++) {
            const Run *s127 = sem == 0 ? &f_runs[3]
                : &e_runs[3][(sem - 1) / 2][sem % 2];
            const Run *d127 = sem == 0 ? &f_runs[conf_at("deep 127")]
                : &e_runs[conf_at("deep 127")][(sem - 1) / 2][sem % 2];
            const Run *s564 = sem == 0 ? &f_runs[5]
                : &e_runs[5][(sem - 1) / 2][sem % 2];
            const Run *t564 = sem == 0 ? &f_runs[conf_at("strided 564")]
                : &e_runs[conf_at("strided 564")][(sem - 1) / 2][sem % 2];
            const Run *d564 = sem == 0 ? &f_runs[conf_at("deep 564")]
                : &e_runs[conf_at("deep 564")][(sem - 1) / 2][sem % 2];

            ok = ok && s127->max_xor == 8 && d127->max_xor == 10
                && s564->max_xor == 10 && t564->max_xor == 10
                && d564->max_xor == 12;
        }
        check("D82 Part D in D82's own order holds under every semantics:"
            " deep 127 / 564 reach XOR10 / XOR12, shallow XOR8 / XOR10,"
            " strided 564 XOR10", ok);
        for (m = 0; m < 2; m++) {
            static const int EXPECT[2][4] = {
                { 1, 8, 10, 9 },          /* rule */
                { 1, 8, 7, 9 }            /* robust */
            };

            for (s = 0; s < 3; s++) {
                reach[s] = 0;
                for (o = 0; o < N_ORDERS; o++) {
                    if (o_runs[m][s][o].max_xor == 12) reach[s]++;
                }
            }
            reach[3] = 0;
            for (o = 0; o < N_ORDERS; o++) {
                if (s_runs[m][o].max_xor == 12) reach[3]++;
            }
            sprintf(msg, "order control (%s): XOR12 reached by deep 564 in"
                " %d/10 orders, strided 564 in %d/10, shallow 564 in %d/10,"
                " random 564 samples (mostly rounds 6-8) in %d/10",
                mode_name[m], reach[2], reach[1], reach[0], reach[3]);
            check(msg, reach[0] == EXPECT[m][0] && reach[1] == EXPECT[m][1]
                && reach[2] == EXPECT[m][2] && reach[3] == EXPECT[m][3]);
        }
        ok = 1;
        for (m = 0; m < 2; m++) {
            int hist[F_ROUNDS];

            level_hist(&conf[8], &e_runs[8][m][0], 6, hist);
            ok = ok && e_runs[8][m][0].nwin[6] > 0 && hist[8] == 0;
            level_hist(&conf[8], &e_runs[8][m][1], 6, hist);
            ok = ok && hist[8] > 0;
        }
        for (lv = 3; lv <= 5; lv++) {
            int hist[F_ROUNDS];

            level_hist(&conf[8], &e_runs[8][MODE_RULE][1], lv, hist);
            ok = ok && hist[8] == 0;
        }
        check("'XOR12 is the first level to use depth-8 entries' holds with"
            " the 512-axis cap and fails without it (no XOR12 winner uses"
            " depth 8)", ok);
        {
            int nn = conf_at("non-null");

            check("D87: non-null keeps XOR12 under float, rule and robust;"
                " 32 = 32 is the search's stop at 32 winners",
                f_runs[nn].max_xor == 12 && f_d87_full.max_xor == 12
                && e_runs[nn][MODE_RULE][0].max_xor == 12
                && e_runs[nn][MODE_ROBUST][0].max_xor == 12
                && e_runs[8][MODE_RULE][0].max_xor == 12
                && e_runs[8][MODE_ROBUST][0].max_xor == 12
                && e_runs[nn][MODE_ROBUST][0].num[6] == MAX_WIN
                && f_runs[nn].num[6] == MAX_WIN);
        }
    }

    if (victores != NULL) fclose(victores);
    printf("\n================================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
