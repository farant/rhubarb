/*
 * KNOTAPEL DEMO 119: Exact Capacity
 * ================================================================
 *
 * Demo 94 compared the binary octahedral catalog (zeta_8, 24 entries)
 * with the binary icosahedral group 2I (random 24-subsets, the first 24
 * by BFS, all 60) by counting weight sets that compute XOR, AND and MAJ
 * through a cell activation: the cell of a weighted sum S = sum +-q_i is
 * (sector of the rotation angle 2 acos(a/|S|), k sectors) x (nearest of
 * the catalog's rotation axes by |cos|), and a set passes if, for some
 * k in {6, 12, 24}, no cell receives both truth values. All in doubles.
 *
 * Demo 118 found that D66's Voronoi counts were decided by EXACT ties.
 * This demo recounts D94's tables exactly:
 *
 *   - every float verdict is reproduced first (D94's own code, renamed
 *     for latina.h, its random streams and subsets) and checked against
 *     D94's printed tables;
 *   - every cell is then certified: when a float value lies within 1e-9
 *     of a sector boundary, of a direction tie or of zero, the decision
 *     is made EXACTLY (Q(sqrt 2) / Q(sqrt 5) via quaternio; sector
 *     boundaries cos(m pi/k) in Q(cos 2 pi/48) / Q(cos 2 pi/240) via
 *     extensio's abelian embedding). Far from every boundary the float
 *     cell is exact: sums of at most 8 unit quaternions carry absolute
 *     error ~1e-14, and a NONZERO sum or vector part has norm >= 7.8e-3
 *     (2I) / 1/32 (zeta_8) - coordinates in Z[sqrt5]/4 (Z[sqrt2]/2), so
 *     |S|^2 = (P + Q sqrt d)/16 (/4) with P^2 - d Q^2 a nonzero integer,
 *     and the Galois conjugate is again a sum of <= 8 unit quaternions
 *     (|S'|^2 <= 64). Normalized values then err by ~1e-12, 2.5 orders of
 *     magnitude inside the margin. The demo records the smallest nonzero
 *     norms it meets and checks them against these bounds;
 *   - exact ties (a sum ON a boundary or equidistant from two axes) give
 *     a set of admissible cells. Verdicts, as in D118:
 *       exact rule  D94's formula evaluated exactly (floor of the exact
 *                   angle; first maximal axis)
 *       robust      passes under every tie resolution
 *       possible    passes under some tie resolution
 *     each OR-ed over k in {6, 12, 24} as D94 does (robust_any = some k
 *     robust - a sufficient condition for robustness).
 *
 * House libraries: quaternio.h, extensio.h (latina.h macros in scope).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_119_exact_capacity/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 */

#include "quaternio.h"
#include "extensio.h"
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

static Piscina *pool;

/* ================================================================
 * D94's arithmetic, verbatim up to names (latina.h: no 'si', no 'D')
 * ================================================================ */

typedef struct { int a; int b; } Zr5;

static Zr5 zr5_make (int a, int b) { Zr5 r; r.a = a; r.b = b; return r; }
static Zr5 zr5_add (Zr5 x, Zr5 y)
{ Zr5 r; r.a = x.a + y.a; r.b = x.b + y.b; return r; }
static Zr5 zr5_sub (Zr5 x, Zr5 y)
{ Zr5 r; r.a = x.a - y.a; r.b = x.b - y.b; return r; }
static Zr5 zr5_neg (Zr5 x) { Zr5 r; r.a = -x.a; r.b = -x.b; return r; }
static Zr5 zr5_mul (Zr5 x, Zr5 y)
{
    Zr5 r;
    r.a = x.a * y.a + 5 * x.b * y.b;
    r.b = x.a * y.b + x.b * y.a;
    return r;
}
static int zr5_eq (Zr5 x, Zr5 y) { return x.a == y.a && x.b == y.b; }
static Zr5 zr5_div4 (Zr5 x) { Zr5 r; r.a = x.a / 4; r.b = x.b / 4; return r; }

typedef struct { Zr5 a, b, c, d; } Q2I;

static Q2I q2i_make (Zr5 a, Zr5 b, Zr5 c, Zr5 d)
{ Q2I r; r.a = a; r.b = b; r.c = c; r.d = d; return r; }
static int q2i_eq (const Q2I *p, const Q2I *q)
{
    return zr5_eq(p->a, q->a) && zr5_eq(p->b, q->b)
        && zr5_eq(p->c, q->c) && zr5_eq(p->d, q->d);
}
static Q2I q2i_neg (const Q2I *q)
{ return q2i_make(zr5_neg(q->a), zr5_neg(q->b), zr5_neg(q->c), zr5_neg(q->d)); }
static Q2I q2i_conj (const Q2I *q)
{ return q2i_make(q->a, zr5_neg(q->b), zr5_neg(q->c), zr5_neg(q->d)); }

static Q2I
q2i_mul (
    const Q2I *p,
    const Q2I *q)
{
    Q2I r;
    Zr5 t;

    t = zr5_mul(p->a, q->a);
    t = zr5_sub(t, zr5_mul(p->b, q->b));
    t = zr5_sub(t, zr5_mul(p->c, q->c));
    t = zr5_sub(t, zr5_mul(p->d, q->d));
    r.a = zr5_div4(t);
    t = zr5_mul(p->a, q->b);
    t = zr5_add(t, zr5_mul(p->b, q->a));
    t = zr5_add(t, zr5_mul(p->c, q->d));
    t = zr5_sub(t, zr5_mul(p->d, q->c));
    r.b = zr5_div4(t);
    t = zr5_mul(p->a, q->c);
    t = zr5_sub(t, zr5_mul(p->b, q->d));
    t = zr5_add(t, zr5_mul(p->c, q->a));
    t = zr5_add(t, zr5_mul(p->d, q->b));
    r.c = zr5_div4(t);
    t = zr5_mul(p->a, q->d);
    t = zr5_add(t, zr5_mul(p->b, q->c));
    t = zr5_sub(t, zr5_mul(p->c, q->b));
    t = zr5_add(t, zr5_mul(p->d, q->a));
    r.d = zr5_div4(t);
    return r;
}

typedef struct { double a, b, c, d; } FQuat;

static FQuat
fq_mul (
    const FQuat *p,
    const FQuat *g)
{
    FQuat r;

    r.a = p->a*g->a - p->b*g->b - p->c*g->c - p->d*g->d;
    r.b = p->a*g->b + p->b*g->a + p->c*g->d - p->d*g->c;
    r.c = p->a*g->c - p->b*g->d + p->c*g->a + p->d*g->b;
    r.d = p->a*g->d + p->b*g->c - p->c*g->b + p->d*g->a;
    return r;
}

static FQuat
q2i_to_float (
    const Q2I *q)
{
    static const double SQRT5 = 2.2360679774997896964;
    FQuat r;

    r.a = ((double)q->a.a + (double)q->a.b * SQRT5) / 4.0;
    r.b = ((double)q->b.a + (double)q->b.b * SQRT5) / 4.0;
    r.c = ((double)q->c.a + (double)q->c.b * SQRT5) / 4.0;
    r.d = ((double)q->d.a + (double)q->d.b * SQRT5) / 4.0;
    return r;
}

/* ================================================================
 * Catalogs: float (D94) and exact, same order
 * ================================================================ */

#define MAX_CAT 64
#define MAX_DIRS 64

typedef struct {
    int            n;
    FQuat          f[MAX_CAT];       /* D94's float values */
    Quaternio      x[MAX_CAT];       /* exact, small field */
    int            depth[MAX_CAT];
    /* directions (D94 build_dirs over this catalog) */
    int            nd;
    double         dir[MAX_DIRS][3];
    int            dir_src[MAX_DIRS];   /* catalog index that created it */
    /* fields */
    Extensio      *small;          /* Q(sqrt 2) or Q(sqrt 5) */
    Extensio      *big;            /* Q(cos 2 pi/48) or Q(cos 2 pi/240) */
    int            big_n;
} Catalog;

/* ---- 2I: D94 build_2i (exact integers), then float + exact ---- */

static Q2I g_2i[128];
static int g_2i_depth[128];
static int g_2i_size = 0;

static int
find_2i (
    const Q2I *q)
{
    int i;
    Q2I nq = q2i_neg(q);

    for (i = 0; i < g_2i_size; i++) {
        if (q2i_eq(q, &g_2i[i]) || q2i_eq(&nq, &g_2i[i])) {
            return i;
        }
    }
    return -1;
}

static void
build_2i (void)
{
    Q2I gens[4];
    Q2I s;
    Q2I t;
    int prev;
    int i;
    int gi;
    int rd;

    s = q2i_make(zr5_make(2,0), zr5_make(2,0), zr5_make(2,0), zr5_make(2,0));
    t = q2i_make(zr5_make(1,1), zr5_make(-1,1), zr5_make(2,0), zr5_make(0,0));
    gens[0] = s;
    gens[1] = q2i_conj(&s);
    gens[2] = t;
    gens[3] = q2i_conj(&t);
    g_2i[0] = q2i_make(zr5_make(4,0), zr5_make(0,0), zr5_make(0,0),
        zr5_make(0,0));
    g_2i_depth[0] = 0;
    g_2i_size = 1;
    for (gi = 0; gi < 4; gi++) {
        if (find_2i(&gens[gi]) < 0) {
            g_2i_depth[g_2i_size] = 0;
            g_2i[g_2i_size++] = gens[gi];
        }
    }
    rd = 1;
    do {
        prev = g_2i_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                Q2I prod = q2i_mul(&g_2i[i], &gens[gi]);

                if (find_2i(&prod) < 0 && g_2i_size < 128) {
                    g_2i_depth[g_2i_size] = rd;
                    g_2i[g_2i_size++] = prod;
                }
            }
        }
        rd++;
    } while (g_2i_size > prev && rd < 20);
}

/* exact element (x + y sqrt 5)/4 of Q(sqrt 5) as text */
static void
zr5_text (
    Zr5   z,
    char *out)
{
    if (z.b < 0) {
        sprintf(out, "(%d - %d a)/4", z.a, -z.b);
    } else {
        sprintf(out, "(%d + %d a)/4", z.a, z.b);
    }
}

static Quaternio
q2i_exact (
    const Q2I *q,
    Extensio  *k5)
{
    char      buf[256];
    char      pa[64];
    char      pb[64];
    char      pc[64];
    char      pd[64];
    Quaternio r;

    zr5_text(q->a, pa);
    zr5_text(q->b, pb);
    zr5_text(q->c, pc);
    zr5_text(q->d, pd);
    sprintf(buf, "[%s, %s, %s, %s]", pa, pb, pc, pd);
    if (!quaternio_ex_chorda(extensio_anulus(k5), chorda_ex_literis(buf,
            pool), pool, &r)) {
        printf("  FATAL: cannot parse %s\n", buf);
        exit(1);
    }
    return r;
}

/* ---- zeta_8: D94 build_z8_catalog, float and exact in lockstep ---- */

static int
find_float (
    const Catalog *cat,
    const FQuat   *q)
{
    int i;

    for (i = 0; i < cat->n; i++) {
        if (fabs(cat->f[i].a - q->a) < 1e-10
            && fabs(cat->f[i].b - q->b) < 1e-10
            && fabs(cat->f[i].c - q->c) < 1e-10
            && fabs(cat->f[i].d - q->d) < 1e-10) {
            return i;
        }
        if (fabs(cat->f[i].a + q->a) < 1e-10
            && fabs(cat->f[i].b + q->b) < 1e-10
            && fabs(cat->f[i].c + q->c) < 1e-10
            && fabs(cat->f[i].d + q->d) < 1e-10) {
            return i;
        }
    }
    return -1;
}

static int
find_exact (
    const Catalog *cat,
    Quaternio      q)
{
    int       i;
    Quaternio zero;
    Quaternio minus;

    (void)quaternio_nullum(quaternio_anulus(q), pool, &zero);
    (void)quaternio_subtrahe(zero, q, pool, &minus);
    for (i = 0; i < cat->n; i++) {
        if (quaternio_aequalis(cat->x[i], q)
            || quaternio_aequalis(cat->x[i], minus)) {
            return i;
        }
    }
    return -1;
}

static Quaternio
qparse (
    const Anulus *ring,
    const char   *text)
{
    Quaternio q;

    if (!quaternio_ex_chorda(ring, chorda_ex_literis(text, pool), pool,
            &q)) {
        printf("  FATAL: cannot parse %s\n", text);
        exit(1);
    }
    return q;
}

/* returns 0 if the float and exact BFS disagree anywhere */
static int
build_z8 (
    Catalog *cat)
{
    FQuat     fg[4];
    Quaternio xg[4];
    double    half = M_PI / 4.0;
    double    co = cos(half);
    double    sn = sin(half);
    int       prev;
    int       i;
    int       gi;
    int       rd;
    const Anulus *ring = extensio_anulus(cat->small);

    fg[0].a = co; fg[0].b = sn; fg[0].c = 0; fg[0].d = 0;
    fg[1].a = co; fg[1].b = -sn; fg[1].c = 0; fg[1].d = 0;
    fg[2].a = co; fg[2].b = 0; fg[2].c = 0; fg[2].d = -sn;
    fg[3].a = co; fg[3].b = 0; fg[3].c = 0; fg[3].d = sn;
    /* D94 builds gens[1] = (a, -b, -c, -d) of gens[0]: -0.0 parts */
    fg[1].c = -0.0; fg[1].d = -0.0;
    fg[3].b = -0.0; fg[3].c = -0.0;
    xg[0] = qparse(ring, "[a/2, a/2, 0, 0]");
    xg[1] = qparse(ring, "[a/2, -a/2, 0, 0]");
    xg[2] = qparse(ring, "[a/2, 0, 0, -a/2]");
    xg[3] = qparse(ring, "[a/2, 0, 0, a/2]");
    cat->n = 1;
    cat->f[0].a = 1; cat->f[0].b = 0; cat->f[0].c = 0; cat->f[0].d = 0;
    (void)quaternio_unum(ring, pool, &cat->x[0]);
    cat->depth[0] = 0;
    for (gi = 0; gi < 4; gi++) {
        int ff = find_float(cat, &fg[gi]);
        int fx = find_exact(cat, xg[gi]);

        if ((ff < 0) != (fx < 0)) {
            return 0;
        }
        if (ff < 0) {
            cat->depth[cat->n] = 0;
            cat->f[cat->n] = fg[gi];
            cat->x[cat->n] = xg[gi];
            cat->n++;
        }
    }
    rd = 1;
    do {
        prev = cat->n;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                FQuat     fp = fq_mul(&cat->f[i], &fg[gi]);
                Quaternio xp;
                int       ff;
                int       fx;

                (void)quaternio_multiplica(cat->x[i], xg[gi], pool, &xp);
                ff = find_float(cat, &fp);
                fx = find_exact(cat, xp);
                if ((ff < 0) != (fx < 0) || (ff >= 0 && ff != fx)) {
                    return 0;
                }
                if (ff < 0 && cat->n < MAX_CAT) {
                    cat->depth[cat->n] = rd;
                    cat->f[cat->n] = fp;
                    cat->x[cat->n] = xp;
                    cat->n++;
                }
            }
        }
        rd++;
    } while (cat->n > prev && rd < 20);
    return 1;
}

/* D94 build_dirs; exact representative = the creating entry's vector
 * part. Returns 0 if float and exact dedup disagree. */
static int
build_dirs (
    Catalog *cat)
{
    int i;
    int j;

    cat->nd = 0;
    for (i = 0; i < cat->n; i++) {
        double qa = cat->f[i].a;
        double qb = cat->f[i].b;
        double qc = cat->f[i].c;
        double qd = cat->f[i].d;
        double nv;
        double ax;
        double ay;
        double az;
        int    found = 0;
        int    found_x = -1;
        int    vec_zero;

        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        vec_zero = quaternio_est_nullum(cat->x[i])
            || (algebraicus_est_nullum(*(const Algebraicus *)
            quaternio_pars(cat->x[i], 1))
            && algebraicus_est_nullum(*(const Algebraicus *)
            quaternio_pars(cat->x[i], 2))
            && algebraicus_est_nullum(*(const Algebraicus *)
            quaternio_pars(cat->x[i], 3)));
        if ((nv < 1e-12) != vec_zero) {
            return 0;
        }
        if (nv < 1e-12) {
            continue;
        }
        ax = qb/nv; ay = qc/nv; az = qd/nv;
        for (j = 0; j < cat->nd; j++) {
            double d1 = fabs(cat->dir[j][0]-ax) + fabs(cat->dir[j][1]-ay)
                + fabs(cat->dir[j][2]-az);
            double d2 = fabs(cat->dir[j][0]+ax) + fabs(cat->dir[j][1]+ay)
                + fabs(cat->dir[j][2]+az);

            if (d1 < 1e-8 || d2 < 1e-8) {
                found = 1;
                break;
            }
        }
        for (j = 0; j < cat->nd; j++) {
            if (quaternio_eadem_axis(cat->x[cat->dir_src[j]], cat->x[i],
                    pool)) {
                found_x = j;
                break;
            }
        }
        if (found != (found_x >= 0)) {
            return 0;
        }
        if (!found && cat->nd < MAX_DIRS) {
            cat->dir[cat->nd][0] = ax;
            cat->dir[cat->nd][1] = ay;
            cat->dir[cat->nd][2] = az;
            cat->dir_src[cat->nd] = i;
            cat->nd++;
        }
    }
    return 1;
}

/* ================================================================
 * D94's float cell (verbatim logic)
 * ================================================================ */

static int
float_vor_cell (
    const Catalog *cat,
    double         ax,
    double         ay,
    double         az)
{
    int    i;
    int    best = 0;
    double bd = -2.0;

    for (i = 0; i < cat->nd; i++) {
        double dp = fabs(ax*cat->dir[i][0] + ay*cat->dir[i][1]
            + az*cat->dir[i][2]);

        if (dp > bd) {
            bd = dp;
            best = i;
        }
    }
    return best;
}

static int
float_phase_cell (
    const Catalog *cat,
    double         sa,
    double         sb,
    double         sc,
    double         sd,
    int            k_sec)
{
    double n2 = sa*sa + sb*sb + sc*sc + sd*sd;
    double nm;
    double qa;
    double rv;
    double half_ang;
    double ang;
    int    sec;
    int    vor;
    int    n_vor = cat->nd + 1;

    if (n2 < 1e-24) {
        return (k_sec - 1) * n_vor + cat->nd;
    }
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
    if (rv / nm < 1e-12) {
        vor = cat->nd;
    } else {
        vor = float_vor_cell(cat, sb / rv, sc / rv, sd / rv);
    }
    return sec * n_vor + vor;
}

/* ================================================================
 * Exact certification
 *
 * Per weight set, per mask: the float sum (D94's order), and lazily
 * the exact sum, its norm, and their images in the big field. The
 * direction tie set does not depend on k and is computed once per
 * mask; the sector is decided per k. Exact work only within MARGIN of
 * a boundary, a direction tie or zero. Everything exact lives under ONE
 * pool mark per weight set.
 * ================================================================ */

#define MARGIN 1e-9
#define MAX_TIES 64
#define MAX_MASKS 256

typedef struct {
    double      sa, sb, sc, sd;
    double      n2;
    double      rv;
    int         zero;             /* exact zero sum */
    int         n_dir;            /* direction tie set (nd = no axis) */
    int         dirs[MAX_DIRS + 1];
    int         dir_rule;
    /* exact, lazily */
    int         have_exact;
    Quaternio   xs;
    int         have_big;
    Algebraicus a_big;            /* real part in the big field */
    Algebraicus a2_big;
    Algebraicus n_big;            /* |S|^2 in the big field */
    int         a_sign;
} MaskBase;

typedef struct {
    int float_cell;
    int exact_cell;               /* D94's formula evaluated exactly */
    int n_ties;
    int ties[MAX_TIES];
} MaskCell;

/* statistics */
static long st_masks = 0;
static long st_exact_sum = 0;
static long st_sector_exact = 0;
static long st_sector_ties = 0;
static long st_dir_exact = 0;
static long st_dir_ties = 0;
static long st_zero = 0;
/* smallest nonzero |S| and |v| met, per small field (0 zeta_8, 1 2I) */
static double st_min_norm[2] = { 1e9, 1e9 };
static double st_min_vec[2] = { 1e9, 1e9 };

/* boundaries b_m = cos(m pi/k) in the big field, with sign and square,
 * and the image of the small field's generator; per big field */
static Algebraicus bnd[3][24];
static Algebraicus bnd_sq[3][24];
static int         bnd_sign[3][24];
static Algebraicus gen_img;
static int         bnd_ready = 0;
static Extensio   *bnd_field = NULL;

static int
k_index (
    int k)
{
    return k == 6 ? 0 : k == 12 ? 1 : 2;
}

static Algebraicus
part_of (
    Quaternio q,
    int       k)
{
    return *(const Algebraicus *)quaternio_pars(q, (i32)k);
}

static int
alg_sign (
    Algebraicus a)
{
    s32 s = 0;

    if (!algebraicus_signum(a, pool, &s)) {
        printf("  FATAL: sign refused\n");
        exit(1);
    }
    return (int)s;
}

static void
prepare_boundaries (
    Catalog *cat)
{
    static const int ks[3] = { 6, 12, 24 };
    int     t;
    int     m;
    Fractio half = fractio_ex_s64(0);

    if (bnd_ready && bnd_field == cat->big) {
        return;
    }
    (void)fractio_ex_s64_s64(1, 2, pool, &half);
    for (t = 0; t < 3; t++) {
        for (m = 1; m < ks[t]; m++) {
            Algebraicus twice;

            /* 2 cos(2 pi j/n) = 2 cos(m pi/k): j = m n/(2k) */
            if (!algebraicus_cosinus(cat->big,
                    (s64)(m * cat->big_n / (2 * ks[t])), pool, &twice)) {
                printf("  FATAL: boundary\n");
                exit(1);
            }
            bnd[t][m] = algebraicus_multiplica(twice,
                algebraicus_ex_fractione(cat->big, half, pool), pool);
            bnd_sq[t][m] = algebraicus_multiplica(bnd[t][m], bnd[t][m],
                pool);
            bnd_sign[t][m] = alg_sign(bnd[t][m]);
        }
    }
    if (!algebraicus_immergere(algebraicus_generator(cat->small, pool),
            cat->big, pool, &gen_img)) {
        printf("  FATAL: generator embedding\n");
        exit(1);
    }
    bnd_ready = 1;
    bnd_field = cat->big;
}

/* small-field element c0 + c1 alpha into the big field via the cached
 * image of alpha (degree-2 small fields) */
static Algebraicus
embed (
    Algebraicus a,
    Catalog    *cat)
{
    Fractio c0 = algebraicus_coefficiens(a, 0, pool);
    Fractio c1 = algebraicus_coefficiens(a, 1, pool);

    return algebraicus_adde(algebraicus_ex_fractione(cat->big, c0, pool),
        algebraicus_multiplica(algebraicus_ex_fractione(cat->big, c1, pool),
        gen_img, pool), pool);
}

static void
need_exact (
    const Catalog *cat,
    const int     *idx,
    int            n_w,
    int            mask,
    MaskBase      *b)
{
    int i;

    if (b->have_exact) {
        return;
    }
    (void)quaternio_nullum(quaternio_anulus(cat->x[0]), pool, &b->xs);
    for (i = 0; i < n_w; i++) {
        Quaternio t;

        if ((mask >> i) & 1) {
            (void)quaternio_adde(b->xs, cat->x[idx[i]], pool, &t);
        } else {
            (void)quaternio_subtrahe(b->xs, cat->x[idx[i]], pool, &t);
        }
        b->xs = t;
    }
    b->have_exact = 1;
    st_exact_sum++;
}

static void
need_big (
    Catalog  *cat,
    MaskBase *b)
{
    Algebraicus nn;
    int         k;

    if (b->have_big) {
        return;
    }
    nn = algebraicus_multiplica(part_of(b->xs, 0), part_of(b->xs, 0), pool);
    for (k = 1; k < 4; k++) {
        nn = algebraicus_adde(nn, algebraicus_multiplica(part_of(b->xs, k),
            part_of(b->xs, k), pool), pool);
    }
    b->a_big = embed(part_of(b->xs, 0), cat);
    b->a2_big = algebraicus_multiplica(b->a_big, b->a_big, pool);
    b->n_big = embed(nn, cat);
    b->a_sign = alg_sign(part_of(b->xs, 0));
    b->have_big = 1;
}

/* sign(a/|S| - b_m) exactly */
static int
compare_boundary (
    MaskBase *b,
    int       t,
    int       m)
{
    int sb = bnd_sign[t][m];

    if (sb == 0) {
        return b->a_sign;
    }
    if (b->a_sign == 0) {
        return -sb;
    }
    if (b->a_sign != sb) {
        return b->a_sign;
    }
    return b->a_sign * alg_sign(algebraicus_subtrahe(b->a2_big,
        algebraicus_multiplica(bnd_sq[t][m], b->n_big, pool), pool));
}

static Algebraicus
vec_dot (
    Quaternio p,
    Quaternio q)
{
    Algebraicus s = algebraicus_multiplica(part_of(p, 1), part_of(q, 1),
        pool);

    s = algebraicus_adde(s, algebraicus_multiplica(part_of(p, 2),
        part_of(q, 2), pool), pool);
    return algebraicus_adde(s, algebraicus_multiplica(part_of(p, 3),
        part_of(q, 3), pool), pool);
}

/* sign of (v.u_j)^2 |u_l|^2 - (v.u_l)^2 |u_j|^2: is axis j nearer? */
static int
dir_compare (
    const Catalog *cat,
    Quaternio      v,
    int            j,
    int            l)
{
    Quaternio   uj = cat->x[cat->dir_src[j]];
    Quaternio   ul = cat->x[cat->dir_src[l]];
    Algebraicus dj = vec_dot(v, uj);
    Algebraicus dl = vec_dot(v, ul);
    Algebraicus lhs = algebraicus_multiplica(algebraicus_multiplica(dj, dj,
        pool), vec_dot(ul, ul), pool);
    Algebraicus rhs = algebraicus_multiplica(algebraicus_multiplica(dl, dl,
        pool), vec_dot(uj, uj), pool);

    return alg_sign(algebraicus_subtrahe(lhs, rhs, pool));
}

/* float sums, exact zero test, direction tie set (once per mask) */
static void
mask_base (
    const Catalog *cat,
    const int     *idx,
    int            n_w,
    int            mask,
    MaskBase      *b)
{
    int i;

    st_masks++;
    memset(b, 0, sizeof(*b));
    for (i = 0; i < n_w; i++) {
        const FQuat *q = &cat->f[idx[i]];
        double sign = ((mask >> i) & 1) ? 1.0 : -1.0;

        b->sa += sign * q->a;
        b->sb += sign * q->b;
        b->sc += sign * q->c;
        b->sd += sign * q->d;
    }
    b->n2 = b->sa*b->sa + b->sb*b->sb + b->sc*b->sc + b->sd*b->sd;
    b->rv = sqrt(b->sb*b->sb + b->sc*b->sc + b->sd*b->sd);
    if (b->n2 < 1e-6) {
        need_exact(cat, idx, n_w, mask, b);
        if (quaternio_est_nullum(b->xs)) {
            st_zero++;
            b->zero = 1;
            return;
        }
    }
    if (sqrt(b->n2) < st_min_norm[cat->big_n == 48 ? 0 : 1]) {
        st_min_norm[cat->big_n == 48 ? 0 : 1] = sqrt(b->n2);
    }
    /* vector part exactly zero? */
    if (b->rv * b->rv < 1e-6 * b->n2 + 1e-12) {
        need_exact(cat, idx, n_w, mask, b);
        if (algebraicus_est_nullum(part_of(b->xs, 1))
            && algebraicus_est_nullum(part_of(b->xs, 2))
            && algebraicus_est_nullum(part_of(b->xs, 3))) {
            b->n_dir = 1;
            b->dirs[0] = cat->nd;
            b->dir_rule = cat->nd;
            return;
        }
    }
    if (b->rv < st_min_vec[cat->big_n == 48 ? 0 : 1]) {
        st_min_vec[cat->big_n == 48 ? 0 : 1] = b->rv;
    }
    {
        double best = -2.0;
        double dps[MAX_DIRS];
        int    cand[MAX_DIRS];
        int    n_cand = 0;
        int    tiny = b->rv * b->rv < 1e-6 * b->n2 + 1e-12;
        int    j;

        for (i = 0; i < cat->nd; i++) {
            dps[i] = fabs((b->sb * cat->dir[i][0] + b->sc * cat->dir[i][1]
                + b->sd * cat->dir[i][2]) / (b->rv > 0 ? b->rv : 1.0));
            if (dps[i] > best) {
                best = dps[i];
            }
        }
        for (i = 0; i < cat->nd; i++) {
            if (tiny || dps[i] > best - MARGIN) {
                cand[n_cand++] = i;
            }
        }
        if (n_cand == 1) {
            b->n_dir = 1;
            b->dirs[0] = cand[0];
            b->dir_rule = cand[0];
            return;
        }
        need_exact(cat, idx, n_w, mask, b);
        st_dir_exact++;
        b->dirs[0] = cand[0];
        b->n_dir = 1;
        for (j = 1; j < n_cand; j++) {
            int cmpv = dir_compare(cat, b->xs, cand[j], b->dirs[0]);

            if (cmpv > 0) {
                b->dirs[0] = cand[j];
                b->n_dir = 1;
            } else if (cmpv == 0) {
                b->dirs[b->n_dir++] = cand[j];
            }
        }
        b->dir_rule = b->dirs[0];
        for (j = 1; j < b->n_dir; j++) {
            if (b->dirs[j] < b->dir_rule) {
                b->dir_rule = b->dirs[j];
            }
        }
        if (b->n_dir > 1) {
            st_dir_ties++;
        }
    }
}

static double cosb[25][25];

static void
prepare_cos_table (void)
{
    int kk;
    int m;

    for (kk = 1; kk <= 24; kk++) {
        for (m = 0; m <= kk; m++) {
            cosb[kk][m] = cos((double)m * M_PI / (double)kk);
        }
    }
}

/* the mask's cell for one k: float (D94) and exact tie set */
static void
mask_cell (
    Catalog       *cat,
    const int     *idx,
    int            n_w,
    int            mask,
    MaskBase      *b,
    int            k_sec,
    MaskCell      *out)
{
    int    n_vor = cat->nd + 1;
    int    sec_set[2];
    int    n_sec = 0;
    int    sec_rule = 0;
    int    t = k_index(k_sec);
    int    a;
    int    c2;

    out->float_cell = float_phase_cell(cat, b->sa, b->sb, b->sc, b->sd,
        k_sec);
    if (b->zero) {
        out->exact_cell = (k_sec - 1) * n_vor + cat->nd;
        out->n_ties = 1;
        out->ties[0] = out->exact_cell;
        return;
    }
    {
        double c = b->sa / sqrt(b->n2);
        int    near = -1;
        int    m;

        if (c > 1.0) c = 1.0;
        if (c < -1.0) c = -1.0;
        for (m = 1; m < k_sec; m++) {
            if (fabs(c - cosb[k_sec][m]) < MARGIN) {
                near = m;
            }
        }
        if (near < 0 && b->n2 >= 1e-6) {
            int s = (int)(acos(c) * (double)k_sec / M_PI);

            if (s >= k_sec) s = k_sec - 1;
            if (s < 0) s = 0;
            sec_set[n_sec++] = s;
            sec_rule = s;
        } else if (b->n2 >= 1e-6) {
            /* one boundary within MARGIN; boundaries are >= 0.0255 apart */
            int cmpv;

            need_exact(cat, idx, n_w, mask, b);
            need_big(cat, b);
            st_sector_exact++;
            cmpv = compare_boundary(b, t, near);
            if (cmpv > 0) {
                sec_set[n_sec++] = near - 1;
                sec_rule = near - 1;
            } else if (cmpv < 0) {
                sec_set[n_sec++] = near;
                sec_rule = near;
            } else {
                st_sector_ties++;
                sec_set[n_sec++] = near - 1;
                sec_set[n_sec++] = near;
                sec_rule = near;
            }
        } else {
            /* tiny nonzero sum: every boundary exactly */
            int lo = 0;

            need_exact(cat, idx, n_w, mask, b);
            need_big(cat, b);
            st_sector_exact++;
            sec_rule = 0;
            for (m = 1; m < k_sec; m++) {
                int cmpv = compare_boundary(b, t, m);

                if (cmpv <= 0) {
                    sec_rule = m;
                }
                if (cmpv == 0) {
                    lo = m;
                }
            }
            if (lo > 0) {
                st_sector_ties++;
                sec_set[n_sec++] = lo - 1;
                sec_set[n_sec++] = lo;
            } else {
                sec_set[n_sec++] = sec_rule;
            }
        }
    }
    out->exact_cell = sec_rule * n_vor + b->dir_rule;
    out->n_ties = 0;
    for (a = 0; a < n_sec; a++) {
        for (c2 = 0; c2 < b->n_dir && out->n_ties < MAX_TIES; c2++) {
            out->ties[out->n_ties++] = sec_set[a] * n_vor + b->dirs[c2];
        }
    }
}

/* ================================================================
 * Verdicts for one weight set
 * ================================================================ */

#define MAX_CELLS 65536

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

/* a tie rule is a function of the POINT: two masks with the same exact
 * sum and different truth values fail under every rule. Candidates by
 * float sums, confirmed exactly. */
static int
vector_clash (
    const Catalog *cat,
    const int     *idx,
    int            n_w,
    MaskBase      *base,
    int            n_masks,
    const int     *tt)
{
    int m;
    int l;

    for (m = 0; m < n_masks; m++) {
        for (l = m + 1; l < n_masks; l++) {
            if (tt[m] == tt[l]) {
                continue;
            }
            if (fabs(base[m].sa - base[l].sa) < MARGIN
                && fabs(base[m].sb - base[l].sb) < MARGIN
                && fabs(base[m].sc - base[l].sc) < MARGIN
                && fabs(base[m].sd - base[l].sd) < MARGIN) {
                need_exact(cat, idx, n_w, m, &base[m]);
                need_exact(cat, idx, n_w, l, &base[l]);
                if (quaternio_aequalis(base[m].xs, base[l].xs)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

static MaskBase g_base[MAX_MASKS];
static MaskCell g_mc[MAX_MASKS];

/* one weight set, one truth table: D94's any-k rule for each verdict */
static Verdict
judge_set (
    Catalog   *cat,
    const int *idx,
    int        n_w,
    const int *tt)
{
    static const int ks[3] = { 6, 12, 24 };
    PiscinaNotatio mark = piscina_notare(pool);
    Verdict v;
    int     t;
    int     m;
    int     n_masks = 1 << n_w;
    int     clash_checked = 0;

    memset(&v, 0, sizeof(v));
    for (m = 0; m < n_masks; m++) {
        mask_base(cat, idx, n_w, m, &g_base[m]);
    }
    for (t = 0; t < 3; t++) {
        int f_ok;
        int r_ok;
        int rb_ok;
        int p_ok;

        for (m = 0; m < n_masks; m++) {
            mask_cell(cat, idx, n_w, m, &g_base[m], ks[t], &g_mc[m]);
            if (g_mc[m].n_ties > 1) {
                v.tied = 1;
            }
        }
        f_ok = labels_pass(g_mc, n_masks, tt, 1);
        r_ok = labels_pass(g_mc, n_masks, tt, 0);
        rb_ok = robust_pass(g_mc, n_masks, tt);
        p_ok = rb_ok;
        if (!rb_ok && !v.v_possible) {
            if (!clash_checked) {
                v.clash = vector_clash(cat, idx, n_w, g_base, n_masks, tt);
                clash_checked = 1;
            }
            p_ok = !v.clash && possible_pass(g_mc, n_masks, tt,
                &v.undecided);
        }
        v.v_float |= f_ok;
        v.v_rule |= r_ok;
        v.v_robust |= rb_ok;
        v.v_possible |= p_ok;
    }
    piscina_reficere(pool, mark);
    return v;
}

/* ================================================================
 * Sets: D94's combination order and random streams
 * ================================================================ */

typedef struct {
    long sets;
    long n_float;
    long n_rule;
    long n_robust;
    long n_possible;
    long n_undecided;
    long n_tied;
    long rule_vs_float;
    long float_not_possible;
} Tally;

static void
tally_add (
    Tally  *t,
    Verdict v)
{
    t->sets++;
    t->n_float += v.v_float;
    t->n_rule += v.v_rule;
    t->n_robust += v.v_robust;
    t->n_possible += v.v_possible;
    t->n_undecided += v.undecided > 0;
    t->n_tied += v.tied;
    t->rule_vs_float += v.v_rule != v.v_float;
    t->float_not_possible += v.v_float && !v.v_possible;
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

            for (j = i + 1; j < n; j++) {
                combo[j] = combo[j - 1] + 1;
            }
            return 1;
        }
        i--;
    }
    return 0;
}

static long
comb_nk (
    int n,
    int k)
{
    long r = 1;
    int  i;

    if (k > n - k) k = n - k;
    for (i = 0; i < k; i++) {
        r = r * (long)(n - i) / (long)(i + 1);
    }
    return r;
}

static void
make_tt (
    int *tt,
    int  n,
    int  fn)
{
    int mask;
    int all = (1 << n) - 1;

    for (mask = 0; mask < (1 << n); mask++) {
        int pc = 0;
        int x = mask;

        while (x) { pc += x & 1; x >>= 1; }
        tt[mask] = fn == 0 ? (pc & 1) : fn == 1 ? (mask == all)
            : (pc > n / 2);
    }
}

static void
count_exhaustive (
    Catalog   *cat,
    int        n_w,
    const int *tt,
    Tally     *t)
{
    int combo[8];
    int i;

    for (i = 0; i < n_w; i++) combo[i] = i;
    do {
        tally_add(t, judge_set(cat, combo, n_w, tt));
    } while (next_combo(combo, n_w, cat->n));
}

static void
count_sampled (
    Catalog       *cat,
    int            n_w,
    int            n_samples,
    const int     *tt,
    unsigned long *rng,
    Tally         *t)
{
    int trial;

    for (trial = 0; trial < n_samples; trial++) {
        int idx[8];
        int ok;
        int i;

        do {
            ok = 1;
            for (i = 0; i < n_w; i++) {
                *rng = *rng * 6364136223846793005UL + 1442695040888963407UL;
                idx[i] = (int)((*rng >> 33) % (unsigned long)cat->n);
            }
            for (i = 0; i < n_w && ok; i++) {
                int j;

                for (j = i + 1; j < n_w; j++) {
                    if (idx[i] == idx[j]) { ok = 0; break; }
                }
            }
        } while (!ok);
        tally_add(t, judge_set(cat, idx, n_w, tt));
    }
}

/* one D94 row: exhaustive if C(n, N) <= 200000, else 'samples' from a
 * seeded stream shared by XOR, AND, MAJ in that order */
static void
count_row (
    Catalog      *cat,
    int           n_w,
    int           samples,
    unsigned long seed,
    Tally        *out3)
{
    int tt[MAX_MASKS];
    int fn;

    if (comb_nk(cat->n, n_w) <= 200000) {
        for (fn = 0; fn < 3; fn++) {
            make_tt(tt, n_w, fn);
            count_exhaustive(cat, n_w, tt, &out3[fn]);
        }
    } else {
        unsigned long rng = seed;

        for (fn = 0; fn < 3; fn++) {
            make_tt(tt, n_w, fn);
            count_sampled(cat, n_w, samples, tt, &rng, &out3[fn]);
        }
    }
}

/* ================================================================
 * Reporting
 * ================================================================ */

static const char *FN_NAME[3] = { "XOR", "AND", "MAJ" };

static void
print_row_header (void)
{
    printf("    N fn  |  sets   | D94-float | exact rule |  robust | "
        "possible | tied sets\n");
}

static void
print_row (
    int          n_w,
    const Tally *t3)
{
    int fn;

    for (fn = 0; fn < 3; fn++) {
        const Tally *t = &t3[fn];

        printf("    %d %s | %7ld | %9ld | %10ld | %7ld | %8ld | %ld%s\n",
            n_w, FN_NAME[fn], t->sets, t->n_float, t->n_rule, t->n_robust,
            t->n_possible, t->n_tied, t->n_undecided ? " (undecided!)" : "");
    }
}

/* ================================================================
 * Main
 * ================================================================ */

/* D94's printed tables (knotapel/demo_94_binary_icosahedral, run
 * 2026-10-08): Phase 2 zeta_8, Phase 2b, Phase 3 - raw counts */
static const long D94_Z8[6][3] = {
    { 1480, 1907, 1494 }, { 8010, 9723, 7156 }, { 17201, 37835, 18368 },
    { 12983, 111290, 10031 }, { 197, 72003, 1085 }, { 1, 57449, 22 } };
static const long D94_2B[6][3] = {
    { 1580, 1930, 1596 }, { 9114, 10055, 8089 }, { 20805, 38688, 24333 },
    { 21679, 112556, 21237 }, { 165, 70931, 7058 }, { 1, 52555, 215 } };
static const long D94_P3[6][3] = {
    { 25595, 31969, 25591 }, { 182660, 189469, 148996 },
    { 119959, 187479, 122681 }, { 74250, 179438, 52393 },
    { 4760, 163243, 10112 }, { 2, 137653, 10 } };

static Catalog z8;
static Catalog full2i;
static Catalog sub2i;

static int quick = 0;     /* DEMO119_CELER: zeta_8 N <= 4 only (plants) */

static int
run_table (
    const char  *title,
    Catalog     *cat,
    unsigned long seed_base,
    int          samples,
    const long (*printed)[3],
    Tally (*out)[3])
{
    int ni;
    int ok = 1;
    int n_rows = quick ? 2 : 6;

    prepare_boundaries(cat);   /* in the pool, before any per-mask mark */
    printf("\n  --- %s: %d entries, %d directions ---\n", title, cat->n,
        cat->nd);
    print_row_header();
    for (ni = 0; ni < n_rows; ni++) {
        int n_w = 3 + ni;
        int fn;

        memset(out[ni], 0, sizeof(out[ni]));
        count_row(cat, n_w, samples, seed_base + (unsigned long)n_w, out[ni]);
        print_row(n_w, out[ni]);
        fflush(stdout);
        if (printed != NULL) {
            for (fn = 0; fn < 3; fn++) {
                if (out[ni][fn].n_float != printed[ni][fn]) {
                    ok = 0;
                }
            }
        }
    }
    return ok;
}

/* the filter's premise: every NONZERO sum and vector part met is at
 * least the algebraic bound (zeta_8 1/32, 2I 1/128; see the header),
 * and something was recorded at all */
static void
norm_bound_check (
    int with_2i)
{
    static const double bound[2] = { 1.0 / 32.0, 1.0 / 128.0 };
    char msg[200];
    int  f;

    for (f = 0; f <= with_2i; f++) {
        printf("  smallest nonzero |S| %.4g, |v| %.4g (%s, bound %.4g)\n",
            st_min_norm[f], st_min_vec[f], f == 0 ? "zeta_8" : "2I",
            bound[f]);
        sprintf(msg, "%s: nonzero |S|, |v| >= %s (normalized float error "
            "~1e-12 << MARGIN)", f == 0 ? "zeta_8" : "2I",
            f == 0 ? "1/32" : "1/128");
        check(msg, st_min_norm[f] >= bound[f] - 1e-12
            && st_min_vec[f] >= bound[f] - 1e-12
            && st_min_norm[f] <= 8.0 && st_min_vec[f] <= 8.0);
    }
}

int
main (void)
{
    static Tally tz8[6][3];
    static Tally t2b[6][3];
    static Tally tp3[6][3];
    static Tally ttrial[6][3];
    static double trial_float[6][3];
    static double trial_rule[6][3];
    static double trial_robust[6][3];
    static double trial_possible[6][3];
    int  i;
    char msg[256];

    pool = piscina_generare_dynamicum("demo_119", 1 << 22);
    quick = getenv("DEMO119_CELER") != NULL;
    prepare_cos_table();
    printf("KNOTAPEL DEMO 119: Exact Capacity\n");
    printf("=================================\n");

    /* ---------- Part A: catalogs ---------- */
    printf("\n=== Part A: catalogs, float (D94) and exact ===\n");
    z8.small = extensio_quadratica(2, pool);
    z8.big_n = 48;
    z8.big = extensio_cosinus(48, pool);
    check("zeta_8 catalog: float BFS and exact BFS agree entry by entry",
        build_z8(&z8));
    sprintf(msg, "zeta_8: 24 entries (got %d)", z8.n);
    check(msg, z8.n == 24);
    check("zeta_8 directions: float and exact dedup agree", build_dirs(&z8));
    sprintf(msg, "zeta_8: 13 directions (got %d)", z8.nd);
    check(msg, z8.nd == 13);
    {
        double worst = 0.0;
        double r2 = sqrt(2.0);

        for (i = 0; i < z8.n; i++) {
            int    k;

            for (k = 0; k < 4; k++) {
                Fractio f0 = algebraicus_coefficiens(part_of(z8.x[i], k), 0,
                    pool);
                Fractio f1 = algebraicus_coefficiens(part_of(z8.x[i], k), 1,
                    pool);
                s64 n0 = 0, d0 = 1, n1 = 0, d1 = 1;
                double ex;
                double fv = k == 0 ? z8.f[i].a : k == 1 ? z8.f[i].b
                    : k == 2 ? z8.f[i].c : z8.f[i].d;

                (void)magnus_ad_s64(fractio_numerator(f0), &n0);
                (void)magnus_ad_s64(fractio_denominator(f0), &d0);
                (void)magnus_ad_s64(fractio_numerator(f1), &n1);
                (void)magnus_ad_s64(fractio_denominator(f1), &d1);
                ex = (double)n0 / (double)d0 + (double)n1 / (double)d1 * r2;
                if (fabs(ex - fv) > worst) worst = fabs(ex - fv);
            }
        }
        sprintf(msg, "zeta_8 exact == float values (max |diff| %.1e)", worst);
        check(msg, worst < 1e-12);
    }

    /* sector unit checks: sigma_1 = (sqrt2/2, sqrt2/2, 0, 0) has c =
     * cos(pi/4) = cos(3 pi/12) EXACTLY - a k = 12 boundary: exact sector
     * 3 (floor of the exact angle), tie set {2, 3}; at k = 6 not a
     * boundary: sector 1, no tie. -sigma_1^-1 has c = -sqrt2/2 < +sqrt2/2
     * (opposite signs: the comparison's sign branch) */
    {
        int      one[1];
        MaskBase b;
        MaskCell mc;
        int      n_vor = z8.nd + 1;
        int      ok12;
        int      ok6;

        prepare_boundaries(&z8);
        one[0] = 1;     /* catalog entry 1 = sigma_1 */
        mask_base(&z8, one, 1, 1, &b);
        mask_cell(&z8, one, 1, 1, &b, 12, &mc);
        ok12 = mc.exact_cell / n_vor == 3 && mc.n_ties == 2
            && mc.ties[0] / n_vor == 2 && mc.ties[1] / n_vor == 3;
        mask_cell(&z8, one, 1, 1, &b, 6, &mc);
        ok6 = mc.exact_cell / n_vor == 1 && mc.n_ties == 1;
        check("sigma_1 on the k = 12 boundary m = 3: exact sector 3, tie "
            "{2, 3}; k = 6: sector 1, no tie", ok12 && ok6);
        mask_base(&z8, one, 1, 0, &b);     /* mask 0: -sigma_1 */
        need_exact(&z8, one, 1, 0, &b);
        need_big(&z8, &b);
        check("-sigma_1 (c = -sqrt2/2) compares BELOW the +sqrt2/2 "
            "boundary; sigma_1 ON it", compare_boundary(&b, 1, 3) < 0
            && (mask_base(&z8, one, 1, 1, &b), need_exact(&z8, one, 1, 1,
            &b), need_big(&z8, &b), compare_boundary(&b, 1, 3) == 0));
    }

    full2i.small = extensio_quadratica(5, pool);
    full2i.big_n = 240;
    full2i.big = extensio_cosinus(240, pool);
    build_2i();
    full2i.n = g_2i_size;
    for (i = 0; i < g_2i_size; i++) {
        full2i.f[i] = q2i_to_float(&g_2i[i]);
        full2i.x[i] = q2i_exact(&g_2i[i], full2i.small);
        full2i.depth[i] = g_2i_depth[i];
    }
    sprintf(msg, "2I: 60 entries mod sign (got %d)", full2i.n);
    check(msg, full2i.n == 60);
    check("2I directions: float and exact dedup agree", build_dirs(&full2i));
    sprintf(msg, "2I: 31 directions (got %d) - D94 printed 31", full2i.nd);
    check(msg, full2i.nd == 31);

    /* ---------- Part B: zeta_8 (D94 Phase 2) ---------- */
    printf("\n=== Part B: zeta_8, 24 entries (D94 Phase 2) ===\n");
    check("zeta_8 table: float replica == D94's printed counts",
        run_table("zeta_8", &z8, 77777UL, 100000, D94_Z8, tz8));
    {
        /* exact counts confirmed by oracle.py (independent exact
         * arithmetic in Q(sqrt2, sqrt3), no float filter, per-vector
         * 'possible'), run 2026-10-08 - constants, not a live call */
        static const long ORACLE[2][3][3] = {
            { { 1456, 1456, 1938 }, { 1799, 1799, 1970 },
              { 1462, 1462, 1968 } },
            { { 7908, 7696, 8678 }, { 9486, 9402, 10046 },
              { 6797, 6611, 8666 } } };
        int ok = 1;
        int ni;
        int fn;

        for (ni = 0; ni < 2; ni++) {
            for (fn = 0; fn < 3; fn++) {
                const Tally *t = &tz8[ni][fn];

                ok &= t->n_rule == ORACLE[ni][fn][0]
                    && t->n_robust == ORACLE[ni][fn][1]
                    && t->n_possible == ORACLE[ni][fn][2]
                    && t->n_undecided == 0;
            }
        }
        check("zeta_8 N = 3, 4: exact rule / robust / possible == "
            "oracle.py's counts (all 18), none undecided", ok);
    }
    if (quick) {
        printf("\n  (DEMO119_CELER: zeta_8 N <= 4 only)\n");
        norm_bound_check(0);
        printf("\n%d passed, %d failed\n", n_pass, n_fail);
        piscina_destruere(pool);
        return n_fail == 0 ? 0 : 1;
    }

    /* ---------- Part C: 2I first 24 by BFS (D94 Phase 2b) ---------- */
    printf("\n=== Part C: 2I, first 24 by BFS (D94 Phase 2b) ===\n");
    sub2i = full2i;
    sub2i.n = 24;
    check("2b directions agree", build_dirs(&sub2i));
    sprintf(msg, "2b: 12 directions (got %d) - D94 printed 12", sub2i.nd);
    check(msg, sub2i.nd == 12);
    check("2b table: float replica == D94's printed counts",
        run_table("2I first 24", &sub2i, 88888UL, 100000, D94_2B, t2b));

    /* ---------- Part D: 2I all 60 (D94 Phase 3) ---------- */
    printf("\n=== Part D: 2I, all 60 (D94 Phase 3) ===\n");
    check("Phase 3 table: float replica == D94's printed counts",
        run_table("2I all 60", &full2i, 55555UL, 200000, D94_P3, tp3));

    /* ---------- Part E: 2I random 24-subsets (D94 Phase 2) ---------- */
    printf("\n=== Part E: 2I random 24-subsets, 10 trials (D94 Phase 2) ===\n");
    memset(trial_float, 0, sizeof(trial_float));
    memset(trial_rule, 0, sizeof(trial_rule));
    memset(trial_robust, 0, sizeof(trial_robust));
    memset(trial_possible, 0, sizeof(trial_possible));
    {
        unsigned long rng = 42UL;
        int           trial;

        for (trial = 0; trial < 10; trial++) {
            int perm[64];
            int ni;

            for (i = 0; i < full2i.n; i++) perm[i] = i;
            for (i = full2i.n - 1; i > 0; i--) {
                int j2;
                int tmp;

                rng = rng * 6364136223846793005UL + 1442695040888963407UL;
                j2 = (int)((rng >> 33) % (unsigned long)(i + 1));
                tmp = perm[i]; perm[i] = perm[j2]; perm[j2] = tmp;
            }
            sub2i = full2i;
            sub2i.n = 24;
            for (i = 0; i < 24; i++) {
                sub2i.f[i] = full2i.f[perm[i]];
                sub2i.x[i] = full2i.x[perm[i]];
                sub2i.depth[i] = full2i.depth[perm[i]];
            }
            if (!build_dirs(&sub2i)) {
                check("trial directions agree", 0);
            }
            prepare_boundaries(&sub2i);
            for (ni = 0; ni < 6; ni++) {
                int   n_w = 3 + ni;
                long  cn2 = comb_nk(24, n_w);
                int   fn;
                Tally t3[3];

                memset(t3, 0, sizeof(t3));
                count_row(&sub2i, n_w, 100000,
                    99999UL + (unsigned long)(trial * 7 + n_w), t3);
                for (fn = 0; fn < 3; fn++) {
                    double scale = cn2 <= 200000 ? 1.0
                        : (double)cn2 / 100000.0;

                    trial_float[ni][fn] += (double)t3[fn].n_float * scale;
                    trial_rule[ni][fn] += (double)t3[fn].n_rule * scale;
                    trial_robust[ni][fn] += (double)t3[fn].n_robust * scale;
                    trial_possible[ni][fn] += (double)t3[fn].n_possible
                        * scale;
                    ttrial[ni][fn].n_tied += t3[fn].n_tied;
                    ttrial[ni][fn].n_undecided += t3[fn].n_undecided;
                    ttrial[ni][fn].rule_vs_float += t3[fn].rule_vs_float;
                }
            }
            printf("    trial %d/10 done\n", trial + 1);
            fflush(stdout);
        }
    }
    printf("    N fn  | D94-float mean | exact rule | robust | possible\n");
    for (i = 0; i < 6; i++) {
        int fn;

        for (fn = 0; fn < 3; fn++) {
            printf("    %d %s | %14.0f | %10.0f | %6.0f | %8.0f\n", 3 + i,
                FN_NAME[fn], trial_float[i][fn] / 10.0,
                trial_rule[i][fn] / 10.0, trial_robust[i][fn] / 10.0,
                trial_possible[i][fn] / 10.0);
        }
    }
    {
        /* D94 printed 2I means for XOR: 1514 9475 21990 23137 672 1 */
        static const long d94_xor_mean[6] = { 1514, 9475, 21990, 23137, 672,
            1 };
        int ok = 1;

        for (i = 0; i < 6; i++) {
            char buf[32];
            char want[32];

            sprintf(buf, "%.0f", trial_float[i][0] / 10.0);
            sprintf(want, "%ld", d94_xor_mean[i]);
            ok &= strcmp(buf, want) == 0;
        }
        check("2I trials: float replica XOR means == D94's printed means", ok);
    }
    {
        long undecided = 0;
        int  fn;

        for (i = 0; i < 6; i++) {
            for (fn = 0; fn < 3; fn++) {
                undecided += ttrial[i][fn].n_undecided;
            }
        }
        sprintf(msg, "2I trials: no set undecided (got %ld)", undecided);
        check(msg, undecided == 0);
    }

    /* ---------- Part F: the comparison on ONE scale ---------- */
    /* Part E scales each sampled count to the population (x C(24,N) /
     * 100000, as D94 does). Part B's zeta_8 rows at N = 7, 8 are RAW
     * counts out of 100000 samples, and D94 compared the two directly
     * ("N=7 XOR: z8=197 2I_mean=672 2I WINS"). Here zeta_8 gets the same
     * scale; N <= 6 is exhaustive on both sides (scale 1). */
    printf("\n=== Part F: 2I random-24 mean vs zeta_8, one scale ===\n");
    printf("    N fn  | float z8 / 2I (ratio)    | exact rule             "
        "| robust                 | possible\n");
    {
        double ratio[6][3][4];
        int    xor_ahead = 1;
        int    n7_even = 1;
        int    and_z8_ahead = 1;

        for (i = 0; i < 6; i++) {
            long   cn = comb_nk(24, 3 + i);
            double scale = cn <= 200000 ? 1.0 : (double)cn / 100000.0;
            int    fn;

            for (fn = 0; fn < 3; fn++) {
                const Tally *t = &tz8[i][fn];
                double z[4];
                double w[4];
                int    col;

                z[0] = (double)t->n_float * scale;
                z[1] = (double)t->n_rule * scale;
                z[2] = (double)t->n_robust * scale;
                z[3] = (double)t->n_possible * scale;
                w[0] = trial_float[i][fn] / 10.0;
                w[1] = trial_rule[i][fn] / 10.0;
                w[2] = trial_robust[i][fn] / 10.0;
                w[3] = trial_possible[i][fn] / 10.0;
                printf("    %d %s |", 3 + i, FN_NAME[fn]);
                for (col = 0; col < 4; col++) {
                    ratio[i][fn][col] = z[col] > 0.0 ? w[col] / z[col] : 0.0;
                    printf(" %7.0f / %6.0f (%4.2f) |", z[col], w[col],
                        ratio[i][fn][col]);
                }
                printf("\n");
            }
        }
        for (i = 0; i < 4; i++) {
            int col;

            for (col = 0; col < 4; col++) {
                xor_ahead &= ratio[i][0][col] > 1.0;
                if (i == 0) {
                    n7_even &= ratio[4][0][col] > 0.9
                        && ratio[4][0][col] < 1.15;
                }
            }
        }
        and_z8_ahead = ratio[4][1][0] < 1.0 && ratio[5][1][0] < 1.0;
        check("XOR, N = 3..6: the 2I mean exceeds zeta_8 under float, rule, "
            "robust and possible", xor_ahead);
        check("N = 7 XOR on one scale: 2I / zeta_8 within [0.9, 1.15] under "
            "all four (D94: 3.4x)", n7_even);
        check("N = 7, 8 AND on one scale: zeta_8 ahead in floats (D94: 2I "
            "'massively wins')", and_z8_ahead);
    }

    /* ---------- statistics ---------- */
    printf("\n=== Certification statistics ===\n");
    printf("  masks evaluated: %ld\n", st_masks);
    printf("  exact sums built: %ld; exact zero sums: %ld\n", st_exact_sum,
        st_zero);
    printf("  sector decided exactly: %ld (sums ON a boundary: %ld)\n",
        st_sector_exact, st_sector_ties);
    printf("  direction decided exactly: %ld (exact ties: %ld)\n",
        st_dir_exact, st_dir_ties);
    norm_bound_check(1);

    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pool);
    return n_fail == 0 ? 0 : 1;
}
