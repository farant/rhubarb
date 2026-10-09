/*
 * KNOTAPEL DEMO 121: Exact Derived Series
 * ================================================================
 *
 * D95 and D96 split zeta_8 (the 24 elements of 2O mod sign) by its
 * derived series 24 > 12 > 4 > 1 and by bracket-null (Re = 0), and
 * measured the XOR (and, in D95, AND) capacity of the pieces with D94's
 * float activation: COMM vs NON-COMM, the 90-degree "0 + 0 = 26%" split,
 * zeta_8-COMM vs the first 12 of 2I, cells A-E, their pairs and
 * triples. Their group theory is exact integer arithmetic; their
 * capacity counts are floats.
 *
 * D94 and D95 count the same 24-element zeta_8 table and print
 * different numbers (N = 4 XOR: 8010 vs 7974; AND N = 3: 1907 vs 1799):
 * D94's floats come from float BFS, D95's from exact integers, and at
 * exact ties the last bit decides.
 *
 * This demo recounts every D95/D96 capacity number exactly, on D120's
 * engine (integer catalogs, surdus signs, extensio fallback):
 *   - the group facts are re-derived (D95's integer code) and the
 *     derived subgroups checked closed with the house quaternio;
 *   - every subset's float count reproduces D95/D96's printed number
 *     (their own float conversion and axis order);
 *   - exact rule, robust, possible per subset; the claims are judged.
 *
 * Mode: DEMO121_CELER = Part A + D96's small subsets (plants).
 *
 * House libraries: quaternio.h, extensio.h, surdus.h. Build and run
 * from the repo root:
 *   ./bin/aedilis knotapel/demo_121_exact_derived_series/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 */

#include "quaternio.h"
#include "extensio.h"
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
    /* D120 integer copy: coordinate k = (xi[k][0] + xi[k][1] sqrt sd)
     * / scale */
    long           xi[MAX_CAT][4][2];
    int            sd;
    int            scale;
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

/* returns 0 if the float and exact BFS disagree anywhere */
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
    /* D120: integers and surds */
    int         have_int;
    long        ix[4][2];         /* scale * sum, integer pairs */
    int         have_surdi;
    int         s_ok;             /* surds built without refusal */
    Surdus      s_a;              /* scale * a */
    Surdus      s_n;              /* scale^2 * |S|^2 */
    int         s_sign;           /* sign(a) */
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
static long st_fallback = 0;      /* surdus refused -> extensio */
static int  force_fallback = 0;   /* Part A: drive the fallback wiring */
static SurdiSpatium spatium;      /* Q(sqrt2, sqrt3, sqrt5) */
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

/* ================================================================
 * D120: the exact layer on integers and surds
 * ================================================================ */

static int
surdi_bitum (
    int d)
{
    return d == 2 ? 1 : d == 3 ? 2 : 4;
}

/* integer copy of the exact catalog; 0 if a coefficient times 'scale'
 * is not an integer */
static int
catalog_integers (
    Catalog *cat,
    int      sd,
    int      scale)
{
    int i;
    int k;
    int c;

    cat->sd = sd;
    cat->scale = scale;
    for (i = 0; i < cat->n; i++) {
        for (k = 0; k < 4; k++) {
            for (c = 0; c < 2; c++) {
                Fractio f = algebraicus_coefficiens(part_of(cat->x[i], k),
                    (i32)c, pool);
                s64 num = 0;
                s64 den = 1;

                if (!magnus_ad_s64(fractio_numerator(f), &num)
                    || !magnus_ad_s64(fractio_denominator(f), &den)
                    || (num * scale) % den != 0) {
                    return 0;
                }
                cat->xi[i][k][c] = (long)(num * scale / den);
            }
        }
    }
    return 1;
}

static void
need_int (
    const Catalog *cat,
    const int     *idx,
    int            n_w,
    int            mask,
    MaskBase      *b)
{
    int i;
    int k;

    if (b->have_int) {
        return;
    }
    for (k = 0; k < 4; k++) {
        b->ix[k][0] = 0;
        b->ix[k][1] = 0;
    }
    for (i = 0; i < n_w; i++) {
        long s = ((mask >> i) & 1) ? 1 : -1;

        for (k = 0; k < 4; k++) {
            b->ix[k][0] += s * cat->xi[idx[i]][k][0];
            b->ix[k][1] += s * cat->xi[idx[i]][k][1];
        }
    }
    b->have_int = 1;
    st_exact_sum++;
}

static Surdus
surd_pair (
    long p,
    long q,
    int  sd)
{
    Surdus x = surdus_ex_s64((s64)p);

    x.c[surdi_bitum(sd)] = (s64)q;
    return x;
}

static void
need_surdi (
    const Catalog *cat,
    MaskBase      *b)
{
    Surdus t;
    s32    s;
    int    k;

    if (b->have_surdi) {
        return;
    }
    b->have_surdi = 1;
    b->s_ok = 0;
    b->s_a = surd_pair(b->ix[0][0], b->ix[0][1], cat->sd);
    b->s_n = surdus_ex_s64(0);
    for (k = 0; k < 4; k++) {
        Surdus x = surd_pair(b->ix[k][0], b->ix[k][1], cat->sd);

        if (!surdus_multiplica(&spatium, x, x, &t)
            || !surdus_adde(b->s_n, t, &b->s_n)) {
            return;
        }
    }
    if (!surdus_signum(&spatium, b->s_a, &s)) {
        return;
    }
    b->s_sign = (int)s;
    b->s_ok = 1;
}

/* 4 cos(j pi/12) in Z[sqrt2, sqrt3] (bit 1 sqrt2, bit 2 sqrt3, 3 sqrt6) */
static Surdus
cos4_surdus (
    int j)
{
    Surdus r = surdus_ex_s64(0);
    int    neg = 0;
    int    s;

    j %= 24;
    if (j < 0) j += 24;
    if (j > 12) j = 24 - j;
    if (j > 6) {
        j = 12 - j;
        neg = 1;
    }
    switch (j) {
    case 0: r.c[0] = 4; break;
    case 1: r.c[3] = 1; r.c[1] = 1; break;     /* sqrt6 + sqrt2 */
    case 2: r.c[2] = 2; break;                 /* 2 sqrt3 */
    case 3: r.c[1] = 2; break;                 /* 2 sqrt2 */
    case 4: r.c[0] = 2; break;
    case 5: r.c[3] = 1; r.c[1] = -1; break;    /* sqrt6 - sqrt2 */
    default: break;                            /* 6: 0 */
    }
    if (neg) {
        for (s = 0; s < 8; s++) {
            r.c[s] = -r.c[s];
        }
    }
    return r;
}

/* sign(a/|S| - cos(m pi/k)) by surds; *ok = 0 on refusal */
static int
sector_surdus (
    const Catalog *cat,
    MaskBase      *b,
    int            k_sec,
    int            m,
    int           *ok)
{
    int    sb = (k_sec > 2 * m) - (k_sec < 2 * m);
    Surdus cc;
    Surdus lhs;
    Surdus rhs;
    Surdus d;
    s32    r;

    *ok = 0;
    need_surdi(cat, b);
    if (!b->s_ok) {
        return 0;
    }
    *ok = 1;
    if (sb == 0) {
        return b->s_sign;
    }
    if (b->s_sign == 0) {
        return -sb;
    }
    if (b->s_sign != sb) {
        return b->s_sign;
    }
    /* 8 cos^2(m pi/k) = 4 + 4 cos(j pi/12), j = 24 m/k */
    cc = cos4_surdus(24 * m / k_sec);
    cc.c[0] += 4;
    if (!surdus_multiplica(&spatium, b->s_a, b->s_a, &lhs)
        || !surdus_scala(lhs, 8, &lhs)
        || !surdus_multiplica(&spatium, cc, b->s_n, &rhs)
        || !surdus_subtrahe(lhs, rhs, &d)
        || !surdus_signum(&spatium, d, &r)) {
        *ok = 0;
        return 0;
    }
    return b->s_sign * (int)r;
}

/* surdus first; extensio (D119) when surdus refuses or force_fallback */
static int
sector_sign (
    Catalog   *cat,
    const int *idx,
    int        n_w,
    int        mask,
    MaskBase  *b,
    int        k_sec,
    int        m)
{
    int ok = 0;
    int r = force_fallback ? 0 : sector_surdus(cat, b, k_sec, m, &ok);

    if (ok) {
        return r;
    }
    st_fallback++;
    need_exact(cat, idx, n_w, mask, b);
    need_big(cat, b);
    return compare_boundary(b, k_index(k_sec), m);
}

/* vector-part dot product of integer pairs over Z[sqrt d] */
static void
vec_dot_int (
    const long v[4][2],
    const long u[4][2],
    int        sd,
    long      *p,
    long      *q)
{
    int k;

    *p = 0;
    *q = 0;
    for (k = 1; k < 4; k++) {
        *p += v[k][0] * u[k][0] + (long)sd * v[k][1] * u[k][1];
        *q += v[k][0] * u[k][1] + v[k][1] * u[k][0];
    }
}

/* sign of (v.u_j)^2 |u_l|^2 - (v.u_l)^2 |u_j|^2 by surds */
static int
dir_surdus (
    const Catalog *cat,
    MaskBase      *b,
    int            j,
    int            l,
    int           *ok)
{
    const long (*uj)[2] = cat->xi[cat->dir_src[j]];
    const long (*ul)[2] = cat->xi[cat->dir_src[l]];
    long   p;
    long   q;
    Surdus dj;
    Surdus dl;
    Surdus nj;
    Surdus nl;
    Surdus lhs;
    Surdus rhs;
    Surdus d;
    s32    r;

    *ok = 0;
    vec_dot_int((const long (*)[2])b->ix, uj, cat->sd, &p, &q);
    dj = surd_pair(p, q, cat->sd);
    vec_dot_int((const long (*)[2])b->ix, ul, cat->sd, &p, &q);
    dl = surd_pair(p, q, cat->sd);
    vec_dot_int(uj, uj, cat->sd, &p, &q);
    nj = surd_pair(p, q, cat->sd);
    vec_dot_int(ul, ul, cat->sd, &p, &q);
    nl = surd_pair(p, q, cat->sd);
    if (!surdus_multiplica(&spatium, dj, dj, &lhs)
        || !surdus_multiplica(&spatium, lhs, nl, &lhs)
        || !surdus_multiplica(&spatium, dl, dl, &rhs)
        || !surdus_multiplica(&spatium, rhs, nj, &rhs)
        || !surdus_subtrahe(lhs, rhs, &d)
        || !surdus_signum(&spatium, d, &r)) {
        return 0;
    }
    *ok = 1;
    return (int)r;
}

static int
dir_sign (
    const Catalog *cat,
    const int     *idx,
    int            n_w,
    int            mask,
    MaskBase      *b,
    int            j,
    int            l)
{
    int ok = 0;
    int r = force_fallback ? 0 : dir_surdus(cat, b, j, l, &ok);

    if (ok) {
        return r;
    }
    st_fallback++;
    need_exact(cat, idx, n_w, mask, b);
    return dir_compare(cat, b->xs, j, l);
}

static int
ix_zero (
    const MaskBase *b,
    int             from)
{
    int k;

    for (k = from; k < 4; k++) {
        if (b->ix[k][0] != 0 || b->ix[k][1] != 0) {
            return 0;
        }
    }
    return 1;
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
        need_int(cat, idx, n_w, mask, b);
        if (ix_zero(b, 0)) {
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
        need_int(cat, idx, n_w, mask, b);
        if (ix_zero(b, 1)) {
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
        need_int(cat, idx, n_w, mask, b);
        st_dir_exact++;
        b->dirs[0] = cand[0];
        b->n_dir = 1;
        for (j = 1; j < n_cand; j++) {
            int cmpv = dir_sign(cat, idx, n_w, mask, b, cand[j],
                b->dirs[0]);

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

            need_int(cat, idx, n_w, mask, b);
            st_sector_exact++;
            cmpv = sector_sign(cat, idx, n_w, mask, b, k_sec, near);
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

            need_int(cat, idx, n_w, mask, b);
            st_sector_exact++;
            sec_rule = 0;
            for (m = 1; m < k_sec; m++) {
                int cmpv = sector_sign(cat, idx, n_w, mask, b, k_sec, m);

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
                need_int(cat, idx, n_w, m, &base[m]);
                need_int(cat, idx, n_w, l, &base[l]);
                if (memcmp(base[m].ix, base[l].ix, sizeof(base[m].ix)) == 0) {
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

/* one D94 row: exhaustive if C(n, N) <= 200000, else 'samples' from a
 * seeded stream shared by XOR, AND, MAJ in that order */
/* ================================================================
 * Reporting
 * ================================================================ */


/* ================================================================
 * Main
 * ================================================================ */

/* ================================================================
 * D95/D96's group: zeta_8 by exact BFS over Z[sqrt2]/2 (D95 verbatim
 * up to names), floats by D95's conversion
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

static FQuat
qz8_to_float (
    const QZ8 *q)
{
    static const double SQRT2 = 1.4142135623730950488;
    FQuat r;

    r.a = ((double)q->a.a + (double)q->a.b * SQRT2) / 2.0;
    r.b = ((double)q->b.a + (double)q->b.b * SQRT2) / 2.0;
    r.c = ((double)q->c.a + (double)q->c.b * SQRT2) / 2.0;
    r.d = ((double)q->d.a + (double)q->d.b * SQRT2) / 2.0;
    return r;
}

static void
zr2_text (
    Zr2   z,
    char *out)
{
    if (z.b < 0) {
        sprintf(out, "(%d - %d a)/2", z.a, -z.b);
    } else {
        sprintf(out, "(%d + %d a)/2", z.a, z.b);
    }
}

static Quaternio
qz8_exact (
    const QZ8 *q,
    Extensio  *k2)
{
    char      buf[256];
    char      pa[64];
    char      pb[64];
    char      pc[64];
    char      pd[64];
    Quaternio r;

    zr2_text(q->a, pa);
    zr2_text(q->b, pb);
    zr2_text(q->c, pc);
    zr2_text(q->d, pd);
    sprintf(buf, "[%s, %s, %s, %s]", pa, pb, pc, pd);
    if (!quaternio_ex_chorda(extensio_anulus(k2), chorda_ex_literis(buf,
            pool), pool, &r)) {
        printf("  FATAL: cannot parse %s\n", buf);
        exit(1);
    }
    return r;
}

static Extensio *k_sqrt2;
static Extensio *k_cos48;

/* subset of zeta_8 in BFS order (D95 load_z8_subset / D96 load_cells) */
static int
z8_catalog (
    Catalog   *cat,
    const int *mask)
{
    int i;

    cat->n = 0;
    cat->small = k_sqrt2;
    cat->big = k_cos48;
    cat->big_n = 48;
    for (i = 0; i < g_z8_size; i++) {
        if (mask[i]) {
            cat->f[cat->n] = qz8_to_float(&g_z8[i]);
            cat->x[cat->n] = qz8_exact(&g_z8[i], k_sqrt2);
            cat->depth[cat->n] = 0;
            cat->n++;
        }
    }
    return build_dirs(cat) && catalog_integers(cat, 2, 2);
}

/* ================================================================
 * Tables: every subset exhaustive, XOR (and AND), N = 3 .. n_hi
 * ================================================================ */

#define N_ROWS 5                  /* N = 3..7 */

typedef struct {
    const char *title;
    int         n;
    int         nd;
    int         n_hi;
    int         with_and;
    Tally       t[N_ROWS][2];
} Table;

static int quick = 0;

static void
run_subset (
    Table   *tb,
    Catalog *cat)
{
    int tt[MAX_MASKS];
    int ni;
    int fn;

    tb->n = cat->n;
    tb->nd = cat->nd;
    memset(tb->t, 0, sizeof(tb->t));
    prepare_boundaries(cat);
    printf("\n  --- %s: %d entries, %d directions ---\n", tb->title, cat->n,
        cat->nd);
    printf("    N fn  |  sets   |  float | exact rule |  robust | possible "
        "| tied sets\n");
    for (ni = 0; ni < N_ROWS && 3 + ni <= tb->n_hi; ni++) {
        int n_w = 3 + ni;

        if (cat->n < n_w) {
            continue;
        }
        for (fn = 0; fn < (tb->with_and ? 2 : 1); fn++) {
            const Tally *t = &tb->t[ni][fn];

            make_tt(tt, n_w, fn);
            count_exhaustive(cat, n_w, tt, &tb->t[ni][fn]);
            printf("    %d %s | %7ld | %6ld | %10ld | %7ld | %8ld | %ld%s\n",
                n_w, fn == 0 ? "XOR" : "AND", t->sets, t->n_float, t->n_rule,
                t->n_robust, t->n_possible, t->n_tied,
                t->n_undecided ? " (undecided!)" : "");
        }
        fflush(stdout);
    }
}

/* the float counts D95/D96 printed for this subset (XOR, AND per N =
 * 3..7; -1 = not printed) and its direction count */
static int
float_matches (
    const Table *tb,
    int          nd_printed,
    const long   xor_printed[N_ROWS],
    const long   and_printed[N_ROWS])
{
    int ni;
    int ok = tb->nd == nd_printed;

    for (ni = 0; ni < N_ROWS; ni++) {
        if (xor_printed[ni] >= 0) {
            ok &= tb->t[ni][0].n_float == xor_printed[ni];
        }
        if (and_printed != NULL && and_printed[ni] >= 0) {
            ok &= tb->t[ni][1].n_float == and_printed[ni];
        }
        ok &= tb->t[ni][0].n_undecided == 0 && tb->t[ni][1].n_undecided == 0;
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

/* Part A: surdus vs extensio on random decisions, both through the
 * dispatch (force_fallback 0 and 1), so the fallback wiring itself is
 * exercised. The statistics are saved and restored: this cross-check is
 * not part of D119's counts. */
static int
cross_validate (
    Catalog       *cat,
    int            trials,
    unsigned long  seed,
    long          *n_dec)
{
    static const int ks[3] = { 6, 12, 24 };
    long          sv[8];
    double        mn[2];
    double        mv[2];
    unsigned long rng = seed;
    int           t;
    int           ok = 1;

    sv[0] = st_masks; sv[1] = st_exact_sum; sv[2] = st_sector_exact;
    sv[3] = st_sector_ties; sv[4] = st_dir_exact; sv[5] = st_dir_ties;
    sv[6] = st_zero; sv[7] = st_fallback;
    mn[0] = st_min_norm[0]; mn[1] = st_min_norm[1];
    mv[0] = st_min_vec[0]; mv[1] = st_min_vec[1];
    prepare_boundaries(cat);
    for (t = 0; t < trials && ok; t++) {
        PiscinaNotatio nota = piscina_notare(pool);
        MaskBase b;
        int      idx[6];
        int      mask;
        int      i;
        int      kk;
        int      m;
        int      j;

        for (i = 0; i < 6; i++) {
            int dup;
            int q;

            do {
                rng = rng * 6364136223846793005UL + 1442695040888963407UL;
                idx[i] = (int)((rng >> 33) % (unsigned long)cat->n);
                dup = 0;
                for (q = 0; q < i; q++) {
                    if (idx[q] == idx[i]) dup = 1;
                }
            } while (dup);
        }
        rng = rng * 6364136223846793005UL + 1442695040888963407UL;
        mask = (int)((rng >> 33) % 64UL);
        memset(&b, 0, sizeof(b));
        need_int(cat, idx, 6, mask, &b);
        if (!ix_zero(&b, 0)) {
            for (kk = 0; kk < 3; kk++) {
                for (m = 1; m < ks[kk]; m++) {
                    int r1;
                    int r2;

                    force_fallback = 0;
                    r1 = sector_sign(cat, idx, 6, mask, &b, ks[kk], m);
                    force_fallback = 1;
                    r2 = sector_sign(cat, idx, 6, mask, &b, ks[kk], m);
                    force_fallback = 0;
                    ok &= r1 == r2;
                    (*n_dec)++;
                }
            }
        }
        if (!ix_zero(&b, 1)) {
            for (j = 1; j < cat->nd && j < 8; j++) {
                int r1;
                int r2;

                force_fallback = 0;
                r1 = dir_sign(cat, idx, 6, mask, &b, j, 0);
                force_fallback = 1;
                r2 = dir_sign(cat, idx, 6, mask, &b, j, 0);
                force_fallback = 0;
                ok &= r1 == r2;
                (*n_dec)++;
            }
        }
        piscina_reficere(pool, nota);
    }
    st_masks = sv[0]; st_exact_sum = sv[1]; st_sector_exact = sv[2];
    st_sector_ties = sv[3]; st_dir_exact = sv[4]; st_dir_ties = sv[5];
    st_zero = sv[6]; st_fallback = sv[7];
    st_min_norm[0] = mn[0]; st_min_norm[1] = mn[1];
    st_min_vec[0] = mv[0]; st_min_vec[1] = mv[1];
    return ok;
}


/* ================================================================
 * Main
 * ================================================================ */

static Catalog full2i;
static Catalog cat_tmp;

/* index of q (or -q) among the catalog's exact entries; -1 */
static int
qfind (
    const Catalog *cat,
    Quaternio      q)
{
    Quaternio zero;
    Quaternio minus;
    int       i;

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

/* subgroup closed under the house quaternio product (indices into the
 * full zeta_8 catalog = BFS order) */
static int
closed_by_quaternio (
    const Catalog *all,
    const int     *in_set)
{
    PiscinaNotatio nota = piscina_notare(pool);
    int i;
    int j;
    int ok = 1;

    for (i = 0; i < all->n && ok; i++) {
        for (j = 0; j < all->n && ok; j++) {
            Quaternio p;
            int       k;

            if (!in_set[i] || !in_set[j]) continue;
            (void)quaternio_multiplica(all->x[i], all->x[j], pool, &p);
            k = qfind(all, p);
            ok = k >= 0 && in_set[k];
        }
    }
    piscina_reficere(pool, nota);
    return ok;
}

/* mask of the cells in 'cells' (bit c = cell c, A..E = 0..4) */
static void
mask_cells (
    int *mask,
    int  cells)
{
    int i;

    for (i = 0; i < g_z8_size; i++) {
        mask[i] = (cells >> g_cell[i]) & 1;
    }
}

static int
subset_table (
    Table      *tb,
    const char *title,
    const int  *mask,
    int         n_hi,
    int         with_and)
{
    tb->title = title;
    tb->n_hi = n_hi;
    tb->with_and = with_and;
    if (!z8_catalog(&cat_tmp, mask)) {
        printf("  FATAL: catalog %s\n", title);
        return 0;
    }
    run_subset(tb, &cat_tmp);
    return 1;
}

/* D95's printed float counts (knotapel/demo_95_commutator_depth, run of
 * the unmodified source 2026-10-09); -1 = not printed */
static const long D95_COMM_X[N_ROWS] = { 107, 218, 120, 58, 8 };
static const long D95_COMM_A[N_ROWS] = { 204, 446, 671, 732, 593 };
static const long D95_NONC_X[N_ROWS] = { 94, 255, 108, 52, 0 };
static const long D95_NONC_A[N_ROWS] = { 170, 365, 558, 558, 411 };
static const long D95_ALL_X[N_ROWS] = { 1456, 7974, 17325, 12975, 628 };
static const long D95_ALL_A[N_ROWS] = { 1799, 9492, 37493, 111253, 249270 };
static const long D95_Q8N_X[N_ROWS] = { 0, -1, -1, -1, -1 };
static const long D95_OUTN_X[N_ROWS] = { 0, 0, 0, 0, -1 };
static const long D95_ALLN_X[N_ROWS] = { 0, 33, 0, 0, -1 };
static const long D95_2I_X[N_ROWS] = { 167, 357, 258, 55, 0 };
static const long D95_2I_A[N_ROWS] = { 204, 452, 699, 712, 486 };

/* D96's (knotapel/demo_96_tl_group_cross, same run): cells, pairs,
 * triples (+E); XOR N = 3..6 */
static const char *D96_TITLE[15] = {
    "A (D0-null)", "B (D0-nonnull)", "C (D1, 2T)", "D (D2-null, Q8)",
    "A+B", "A+C", "A+D", "B+C", "B+D", "C+D",
    "A+B+C+E (skip Q8)", "A+B+D+E", "A+C+D+E", "B+C+D+E", "E (identity)" };
static const int D96_CELLS[15] = { 1, 2, 4, 8, 3, 5, 9, 6, 10, 12,
    23, 27, 29, 30, 16 };
static const int D96_DIRS[15] = { 6, 3, 4, 3, 9, 10, 9, 7, 3, 7, 13, 9, 13,
    7, 0 };
static const long D96_X[15][N_ROWS] = {
    { 0, 0, 0, 0, -1 }, { 20, 15, 6, 1, -1 }, { 56, 70, 24, 16, -1 },
    { 0, -1, -1, -1, -1 }, { 94, 255, 108, 52, -1 }, { 168, 750, 300, 40, -1 },
    { 0, 33, 0, 0, -1 }, { 352, 941, 1465, 931, -1 }, { 20, 15, 6, 1, -1 },
    { 80, 162, 96, 34, -1 }, { 985, 4639, 9178, 7043, -1 },
    { 288, 920, 976, 318, -1 }, { 452, 2007, 1977, 614, -1 },
    { 613, 2180, 2744, 1369, -1 }, { -1, -1, -1, -1, -1 } };

static Table t95[7];               /* COMM, NONC, ALL, Q8N, OUTN, ALLN, 2I */
static Table t96[15];

/* a/b > c/d for counts (b, d > 0) */
static int
rate_gt (
    long a,
    long b,
    long c,
    long d)
{
    return a * d > c * b;
}

static long
robust_of (
    const Table *tb,
    int          ni)
{
    return tb->t[ni][0].n_robust;
}

static long
sets_of (
    const Table *tb,
    int          ni)
{
    return tb->t[ni][0].sets;
}

/* every row of a subset: every set tied */
static int
all_tied (
    const Table *tb)
{
    int ni;
    int ok = 1;

    for (ni = 0; ni < N_ROWS; ni++) {
        if (tb->t[ni][0].sets > 0) {
            ok &= tb->t[ni][0].n_tied == tb->t[ni][0].sets;
        }
    }
    return ok;
}

/* D119's exact zeta_8 counts (N = 3..6: rule, robust, possible; XOR then
 * AND) - the same 24-element table, counted from D94's floats */
static const long D119_Z8[4][2][3] = {
    { { 1456, 1456, 1938 }, { 1799, 1799, 1970 } },
    { { 7908, 7696, 8678 }, { 9486, 9402, 10046 } },
    { { 16479, 15497, 24714 }, { 37309, 36707, 39072 } },
    { { 12268, 10070, 22979 }, { 110495, 107663, 116323 } } };

static void
judge_claims (void)
{
    int  ni;
    int  ok;
    int  ti;

    printf("\n=== Part D: the claims, exactly ===\n");

    /* the two float implementations disagree; exact counts do not */
    ok = 1;
    for (ni = 0; ni < 4; ni++) {
        int fn;

        for (fn = 0; fn < 2; fn++) {
            const Tally *t = &t95[2].t[ni][fn];

            ok &= t->n_rule == D119_Z8[ni][fn][0]
                && t->n_robust == D119_Z8[ni][fn][1]
                && t->n_possible == D119_Z8[ni][fn][2];
        }
    }
    check("ALL(24), N = 3..6: exact rule / robust / possible == D119's, "
        "although D95's floats (7974 at N = 4 XOR) differ from D94's (8010)",
        ok && t95[2].t[1][0].n_float != 8010);

    /* D96: Cell B perfect */
    ok = 1;
    for (ni = 0; ni < 4; ni++) {
        ok &= robust_of(&t96[1], ni) == sets_of(&t96[1], ni);
    }
    check("D96 'Cell B is 100% at every N' holds ROBUSTLY (every set tied, "
        "every resolution passes)", ok && all_tied(&t96[1]));

    /* D96: B+C wins at every N, robust */
    ok = 1;
    for (ni = 0; ni < 4; ni++) {
        for (ti = 4; ti <= 9; ti++) {
            if (ti != 7) {
                ok &= rate_gt(robust_of(&t96[7], ni), sets_of(&t96[7], ni),
                    robust_of(&t96[ti], ni), sets_of(&t96[ti], ni));
            }
        }
    }
    check("D96 'B+C wins at every N' holds ROBUSTLY (every pair, N = 3..6)",
        ok);

    /* D96: A+B+C (+E) beats ALL, under rule, robust and possible */
    ok = 1;
    for (ni = 0; ni < 4; ni++) {
        const Tally *abc = &t96[10].t[ni][0];
        const Tally *all = &t95[2].t[ni][0];

        ok &= rate_gt(abc->n_rule, abc->sets, all->n_rule, all->sets)
            && rate_gt(abc->n_robust, abc->sets, all->n_robust, all->sets)
            && rate_gt(abc->n_possible, abc->sets, all->n_possible,
            all->sets);
    }
    check("D96 'A+B+C beats ALL at every N (Q8 is noise)' holds under rule, "
        "robust AND possible", ok);

    /* D96: removing C hurts most at N = 3..5, B at N = 6 (robust) */
    ok = 1;
    for (ni = 0; ni < 4; ni++) {
        /* lowest robust rate among triples = the missing cell that hurts
         * most: 11 = skip C, 12 = skip B */
        int want = ni < 3 ? 11 : 12;

        for (ti = 10; ti <= 13; ti++) {
            if (ti != want) {
                ok &= rate_gt(robust_of(&t96[ti], ni), sets_of(&t96[ti], ni),
                    robust_of(&t96[want], ni), sets_of(&t96[want], ni));
            }
        }
    }
    check("D96 'C most important at N = 3..5, B at N = 6' holds ROBUSTLY", ok);

    /* D95: COMM survives N = 7, NON-COMM dies */
    check("D95 'COMM survives N = 7 (8), NON-COMM dies (0)' holds under "
        "every resolution (COMM robust 8 = possible 8; NON-COMM possible 0)",
        t95[0].t[4][0].n_robust == 8 && t95[0].t[4][0].n_possible == 8
        && t95[1].t[4][0].n_possible == 0);

    /* D95: 2I first 12 beats zeta_8-COMM at N = 3..5, not at 6, 7 */
    ok = 1;
    for (ni = 0; ni < 3; ni++) {
        ok &= robust_of(&t95[6], ni) > robust_of(&t95[0], ni);
    }
    ok &= robust_of(&t95[6], 3) < robust_of(&t95[0], 3)
        && robust_of(&t95[6], 4) < robust_of(&t95[0], 4);
    check("D95 '2I first 12 beats zeta_8-COMM at N = 3..5, crossover at "
        "N = 6..7' holds ROBUSTLY", ok);

    /* D95: the 90-degree split is decided by ties */
    check("every set of every null subset is TIED (Re = 0 exactly: angle "
        "180 degrees, a sector boundary for k = 6, 12, 24)",
        all_tied(&t95[3]) && all_tied(&t95[4]) && all_tied(&t95[5]));
    check("D95 '0 + 0 = 26%' (all-null N = 4, float 33): exact rule 27, "
        "robust 15 (11.9%), possible 102",
        t95[5].t[1][0].n_float == 33 && t95[5].t[1][0].n_rule == 27
        && t95[5].t[1][0].n_robust == 15 && t95[5].t[1][0].n_possible == 102);
    check("D95 'zero elsewhere' and 'each half is dead' hold only robustly: "
        "possible all-null N = 3, 5, 6 = 80, 72, 25; outer-null N = 3 = 16; "
        "Q8-null 1 of 1",
        t95[5].t[0][0].n_robust == 0 && t95[5].t[0][0].n_possible == 80
        && t95[5].t[2][0].n_possible == 72 && t95[5].t[3][0].n_possible == 25
        && t95[4].t[0][0].n_robust == 0 && t95[4].t[0][0].n_possible == 16
        && t95[3].t[0][0].n_robust == 0 && t95[3].t[0][0].n_possible == 1);

    /* D96: cell C at N = 6 is tie-decided */
    check("D96 cell C at N = 6 (float 16 of 28): robust 4, possible 28 - "
        "decided by ties", t96[2].t[3][0].n_float == 16
        && t96[2].t[3][0].n_robust == 4 && t96[2].t[3][0].n_possible == 28);
}

int
main (void)
{
    int  sizes[10];
    int  n_levels;
    int  i;
    int  mask[MAX_GRP];
    char msg[256];

    pool = piscina_generare_dynamicum("demo_121", 1 << 22);
    quick = getenv("DEMO121_CELER") != NULL;
    prepare_cos_table();
    printf("KNOTAPEL DEMO 121: Exact Derived Series\n");
    printf("=======================================\n");
    {
        int primi[3];

        primi[0] = 2;
        primi[1] = 3;
        primi[2] = 5;
        check("surd field Q(sqrt2, sqrt3, sqrt5)",
            surdi_spatium(primi, 3, &spatium));
    }

    /* ---------- Part A: the group, exactly ---------- */
    printf("\n=== Part A: zeta_8's derived series and cells (D95, D96) ===\n");
    k_sqrt2 = extensio_quadratica(2, pool);
    k_cos48 = extensio_cosinus(48, pool);
    build_z8_d95();
    sprintf(msg, "zeta_8: 24 elements mod sign (got %d)", g_z8_size);
    check(msg, g_z8_size == 24);
    n_levels = derive_structure(sizes);
    {
        int n_single = 0;
        int n_g1 = 0;
        int lv[4] = { 0, 0, 0, 0 };
        int cl[5] = { 0, 0, 0, 0, 0 };
        int n_null = 0;

        for (i = 0; i < g_z8_size; i++) {
            n_single += g_single_comm[i];
            n_g1 += g_in_g1[i];
            lv[g_level[i]]++;
            cl[g_cell[i]]++;
            n_null += g_null[i];
        }
        printf("  derived series: %d > %d > %d > %d; levels %d/%d/%d/%d; "
            "cells A-E %d/%d/%d/%d/%d; null %d\n", sizes[0], sizes[1],
            sizes[2], sizes[3], lv[0], lv[1], lv[2], lv[3], cl[0], cl[1],
            cl[2], cl[3], cl[4], n_null);
        check("D95: 12 single commutators, G1 = 12; series 24 > 12 > 4 > 1, "
            "levels 12/8/3/1",
            n_single == 12 && n_g1 == 12 && n_levels == 3 && sizes[1] == 12
            && sizes[2] == 4 && sizes[3] == 1 && lv[0] == 12 && lv[1] == 8
            && lv[2] == 3 && lv[3] == 1);
        check("D96: cells A/B/C/D/E = 6/6/8/3/1, null = 9 (A + D)",
            cl[0] == 6 && cl[1] == 6 && cl[2] == 8 && cl[3] == 3
            && cl[4] == 1 && n_null == 9);
    }
    {
        Catalog *all = &cat_tmp;
        int      g2[MAX_GRP];

        for (i = 0; i < g_z8_size; i++) mask[i] = 1;
        check("zeta_8 catalog: float and exact axes agree, integers exact",
            z8_catalog(all, mask));
        for (i = 0; i < g_z8_size; i++) g2[i] = g_level[i] >= 2;
        check("G1 and G2 closed under the house quaternio product "
            "(independent of D95's integer code)",
            closed_by_quaternio(all, g_in_g1) && closed_by_quaternio(all, g2));
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
    {
        int n_comm = 0;
        int seen[128];
        int j;

        memset(seen, 0, sizeof(seen));
        for (i = 0; i < g_2i_size; i++) {
            for (j = 0; j < g_2i_size; j++) {
                Q2I ai = q2i_conj(&g_2i[i]);
                Q2I bj = q2i_conj(&g_2i[j]);
                Q2I ab = q2i_mul(&g_2i[i], &g_2i[j]);
                Q2I t1 = q2i_mul(&ab, &ai);
                Q2I cm = q2i_mul(&t1, &bj);
                int k = find_2i(&cm);

                if (k >= 0) seen[k] = 1;
            }
        }
        for (i = 0; i < g_2i_size; i++) n_comm += seen[i];
        sprintf(msg, "2I perfect: all %d of 60 are single commutators",
            n_comm);
        check(msg, g_2i_size == 60 && n_comm == 60);
    }
    {
        Catalog *z = &cat_tmp;
        long     n_dec = 0;
        int      ok;

        for (i = 0; i < g_z8_size; i++) mask[i] = 1;
        (void)z8_catalog(z, mask);
        ok = cross_validate(z, 60, 2026UL, &n_dec);
        full2i.n = 12;
        ok = ok && build_dirs(&full2i) && catalog_integers(&full2i, 5, 4)
            && cross_validate(&full2i, 20, 2027UL, &n_dec);
        sprintf(msg, "surdus == extensio on %ld random sector/axis "
            "decisions (fallback forced)", n_dec);
        check(msg, ok && n_dec > 1000);
    }

    /* ---------- Part B: D95's tables ---------- */
    if (!quick) {
        int nn[MAX_GRP];

        printf("\n=== Part B: D95 - COMM, NON-COMM, ALL; 90-degree split; "
            "2I first 12 ===\n");
        for (i = 0; i < g_z8_size; i++) nn[i] = !g_in_g1[i];
        (void)subset_table(&t95[0], "COMM (G1)", g_in_g1, 7, 1);
        (void)subset_table(&t95[1], "NON-COMM (G0 \\ G1)", nn, 7, 1);
        for (i = 0; i < g_z8_size; i++) mask[i] = 1;
        (void)subset_table(&t95[2], "ALL", mask, 7, 1);
        for (i = 0; i < g_z8_size; i++) mask[i] = g_null[i] && g_level[i] >= 2;
        (void)subset_table(&t95[3], "Q8-null", mask, 6, 0);
        for (i = 0; i < g_z8_size; i++) mask[i] = g_null[i] && g_level[i] == 0;
        (void)subset_table(&t95[4], "outermost-null", mask, 6, 0);
        for (i = 0; i < g_z8_size; i++) mask[i] = g_null[i];
        (void)subset_table(&t95[5], "all-null", mask, 6, 0);
        t95[6].title = "2I first 12";
        t95[6].n_hi = 7;
        t95[6].with_and = 1;
        full2i.n = 12;
        run_subset(&t95[6], &full2i);
        check("D95's float counts reproduced: COMM, NON-COMM, ALL (XOR and "
            "AND, N = 3..7; axes 7, 9, 13)",
            float_matches(&t95[0], 7, D95_COMM_X, D95_COMM_A)
            && float_matches(&t95[1], 9, D95_NONC_X, D95_NONC_A)
            && float_matches(&t95[2], 13, D95_ALL_X, D95_ALL_A));
        check("D95's float counts reproduced: 90-degree split (axes 3, 6, 9) "
            "and 2I first 12 (axes 6)",
            float_matches(&t95[3], 3, D95_Q8N_X, NULL)
            && float_matches(&t95[4], 6, D95_OUTN_X, NULL)
            && float_matches(&t95[5], 9, D95_ALLN_X, NULL)
            && float_matches(&t95[6], 6, D95_2I_X, D95_2I_A));
    }

    /* ---------- Part C: D96's tables ---------- */
    printf("\n=== Part C: D96 - cells, pairs, triples ===\n");
    {
        int ok = 1;
        int ti;
        int n_tab = quick ? 10 : 15;

        for (ti = 0; ti < n_tab; ti++) {
            mask_cells(mask, D96_CELLS[ti]);
            ok &= subset_table(&t96[ti], D96_TITLE[ti], mask, 6, 0)
                && float_matches(&t96[ti], D96_DIRS[ti], D96_X[ti], NULL);
        }
        sprintf(msg, "D96's float counts and axis counts reproduced (%d "
            "subsets)", n_tab);
        check(msg, ok);
    }

    if (quick) {
        sprintf(msg, "surdus never refused (fallbacks %ld)", st_fallback);
        check(msg, st_fallback == 0);
        printf("\n  (DEMO121_CELER: Part A + D96 cells and pairs)\n");
        printf("\n%d passed, %d failed\n", n_pass, n_fail);
        piscina_destruere(pool);
        return n_fail == 0 ? 0 : 1;
    }

    judge_claims();

    /* ---------- statistics ---------- */
    printf("\n=== Certification statistics ===\n");
    printf("  masks evaluated: %ld\n", st_masks);
    printf("  exact sums built: %ld; exact zero sums: %ld\n", st_exact_sum,
        st_zero);
    printf("  sector decided exactly: %ld (sums ON a boundary: %ld)\n",
        st_sector_exact, st_sector_ties);
    printf("  direction decided exactly: %ld (exact ties: %ld)\n",
        st_dir_exact, st_dir_ties);
    printf("  surdus refusals (extensio fallback): %ld\n", st_fallback);
    norm_bound_check(1);
    sprintf(msg, "surdus never refused (fallbacks %ld)", st_fallback);
    check(msg, st_fallback == 0);

    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pool);
    return n_fail == 0 ? 0 : 1;
}
