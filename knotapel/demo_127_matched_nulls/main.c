/*
 * KNOTAPEL DEMO 127: Group Catalogs against Matched Nulls (audit P1)
 * ================================================================
 *
 * Under ONE fixed exact activation, do the group catalogs compute better
 * or worse than knot-free point sets, and what carries a difference: the
 * axis table, coincident sums, or the geometry? (claims.md KC 47, 48,
 * 19.7; ledger ...E72XG3). Plan: plan.md (approved 2026-10-14).
 *
 * Fixed activation: D94's phase cell (cellae, CELLAE_CELLA_D94), ladder
 * k in {6, 12, 24}, the SIGNED encoding S = sum +-q_i (coincidences are
 * relations with entries in {-1, 0, 1}), XOR / AND / MAJ, rule / robust
 * / possible, with tied, concursus and undecided reported.
 *
 *   Part A  construction and checks: the catalogs (2O = D119's zeta_8,
 *           2I-D94 = D119's first 24 of 2I, 2I-B = the first 24 of 2I
 *           closed from a BRAID-CORRECT pair of order-10 generators, 2T
 *           = the 12 rational points of 2O); the tables (T13 = 2O's 13
 *           axes as integer directions, R13 = 13 group-free random
 *           integer directions; it has one near-duplicate pair, about
 *           7.2 degrees, against T13's 35.3); the nulls (H6, H5: D126's
 *           height-H sphere subsets, 20 seeds - "height" is the lcm,
 *           and each set holds a few points of height 1-3, some in 2T;
 *           G: D126's high-height rational points, generic but NOT Haar,
 *           set heights of 48-65 digits; AX: 2O's axes with random
 *           angles; AN: 2O's angles on random axes; LIN: A 2O, A in
 *           GL4(Q), which changes norms as well as directions); the
 *           controls 2I-R (20 random 24-subsets of 2I: entry order) and
 *           random sign draws of 2O and 2I-D94; the D119 anchor (own
 *           tables reproduce D119's exact rows, N = 3-4).
 *   Part B  relation profiles (reticulum, as D126): per set the height,
 *           the norm-3 relations and the odd-norm {-1, 0, 1} relations
 *           (the only coincidences that can matter for XOR) within
 *           D126's radius (8 for N = 24, 10 for N = 12).
 *   Part C  passing subsets per N (not D94's "capacity", the largest N
 *           with a pass), N = 3-7 exhaustive, every set under its own
 *           table, T13 and R13; anchored to D119 (N = 3-6) and, for R13,
 *           to the review's independent oracle. Only XOR is a property of
 *           a point set: AND and MAJ depend on each weight's sign.
 *
 * Representatives: the sign of a weight matters for AND and MAJ under
 * the signed encoding (flipping q_i relabels the masks: AND becomes AND
 * with input i negated); XOR is invariant (every label flips). cellae's
 * closure keeps the FIRST element BFS produced from each {q, -q}, so
 * D119's catalogs have mixed signs. Here every set ALSO has the rule
 * "first nonzero part positive" (canonical): the comparison uses one
 * rule for all sets, and the catalogs in D119's BFS signs are kept as
 * the anchor. The nulls are canonical; AX, AN and LIN are built from
 * canonical 2O.
 *
 * Rational nulls are built in Q(sqrt 2) (Q embeds in every field); their
 * cells depend only on the geometry, so they are compared with the
 * Q(sqrt 5) catalogs as they are. T13 and R13 are rational, so the same
 * table is written into each field.
 *
 * House libraries: cellae, quaternio, extensio, reticulum, hence
 * latina.h (Roman numerals and Latin keywords are macros here).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_127_matched_nulls/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 */

#include "cellae.h"
#include "quaternio.h"
#include "extensio.h"
#include "reticulum.h"
#include "matrix.h"
#include "processus.h"
#include "fenestra_tempus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
static int      in_worker = 0;   /* a "--opus" child: build, run, print */

static void
fatal (
    const char *msg)
{
    printf("  FATAL: %s\n", msg);
    exit(1);
}

/* ================================================================
 * Exact helpers
 * ================================================================ */

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

static Algebraicus
part_of (
    Quaternio q,
    int       k)
{
    return *(const Algebraicus *)quaternio_pars(q, (i32)k);
}

static int
sign_of (
    Algebraicus a)
{
    s32 s = 0;

    if (!algebraicus_signum(a, pool, &s)) {
        fatal("sign refused");
    }
    return (int)s;
}

static Quaternio
negate (
    Quaternio q)
{
    Quaternio zero;
    Quaternio minus;

    (void)quaternio_nullum(quaternio_anulus(q), pool, &zero);
    (void)quaternio_subtrahe(zero, q, pool, &minus);
    return minus;
}

/* canonical representative of {q, -q}: first nonzero part positive */
static Quaternio
canonical (
    Quaternio q)
{
    int k;

    for (k = 0; k < 4; k++) {
        int s = sign_of(part_of(q, k));

        if (s != 0) {
            return s > 0 ? q : negate(q);
        }
    }
    return q;
}

/* independent of canonical(): the first nonzero part is positive */
static int
is_canonical (
    Quaternio q)
{
    int k;

    for (k = 0; k < 4; k++) {
        s32 s = 0;

        if (!algebraicus_signum(part_of(q, k), pool, &s)) {
            fatal("sign refused");
        }
        if (s != 0) {
            return s > 0;
        }
    }
    return 0;
}

static int
is_unit (
    Quaternio q)
{
    Algebraicus n;
    Algebraicus one;

    (void)quaternio_norma(q, pool, &n);
    one = algebraicus_ex_fractione(algebraicus_corpus(n),
        fractio_ex_s64(1), pool);
    return algebraicus_aequalis(n, one);
}

static Quaternio
mul (
    Quaternio p,
    Quaternio q)
{
    Quaternio x;

    if (!quaternio_multiplica(p, q, pool, &x)) {
        fatal("multiplication refused");
    }
    return x;
}

/* smallest e > 0 with q^e = 1, 0 if none up to 'limit' */
static int
order_of (
    Quaternio q,
    int       limit)
{
    Quaternio one;
    Quaternio x = q;
    int       e;

    (void)quaternio_unum(quaternio_anulus(q), pool, &one);
    for (e = 1; e <= limit; e++) {
        if (quaternio_aequalis(x, one)) {
            return e;
        }
        x = mul(x, q);
    }
    return 0;
}

/* ================================================================
 * Point sets
 * ================================================================ */

#define MAX_POINTS 64

typedef struct {
    char          name[64];
    const Anulus *ring;
    int           n;
    Quaternio     p[MAX_POINTS];
} PointSet;

static int
contains (
    const PointSet *s,
    Quaternio       q)
{
    int k;

    for (k = 0; k < s->n; k++) {
        if (quaternio_aequalis(s->p[k], q)) {
            return 1;
        }
    }
    return 0;
}

static int
contains_up_to_sign (
    const PointSet *s,
    Quaternio       q)
{
    return contains(s, q) || contains(s, negate(q));
}

/* S u -S closed under multiplication? */
static int
closed_with_signs (
    const PointSet *s)
{
    int a;
    int b;

    for (a = 0; a < s->n; a++) {
        for (b = 0; b < s->n; b++) {
            PiscinaNotatio mark = piscina_notare(pool);
            int            in = contains_up_to_sign(s, mul(s->p[a],
                s->p[b]));

            piscina_reficere(pool, mark);
            if (!in) {
                return 0;
            }
        }
    }
    return 1;
}

static void
from_catalog (
    const Catalogus *cat,
    const char      *name,
    PointSet        *s)
{
    int k;

    snprintf(s->name, sizeof(s->name), "%s", name);
    s->n = (int)catalogus_numerus(cat);
    if (s->n > MAX_POINTS) {
        fatal("catalog too large for a point set");
    }
    for (k = 0; k < s->n; k++) {
        s->p[k] = catalogus_elementum(cat, (i32)k);
    }
    s->ring = quaternio_anulus(s->p[0]);
}

/* the same points with the canonical representative of each {q, -q};
 * returns how many changed sign */
static int
canonical_set (
    const PointSet *s,
    PointSet       *out)
{
    int k;
    int flipped = 0;

    *out = *s;
    snprintf(out->name, sizeof(out->name), "%s", s->name);
    for (k = 0; k < s->n; k++) {
        out->p[k] = canonical(s->p[k]);
        flipped += !quaternio_aequalis(out->p[k], s->p[k]);
    }
    return flipped;
}

static Catalogus *
as_catalog (
    const PointSet *s)
{
    Catalogus *cat = NULL;

    if (!catalogus_ex_punctis(s->p, (i32)s->n, pool, &cat)) {
        return NULL;
    }
    return cat;
}

/* lcm of the denominators of every rational coefficient: the height */
static Magnus
height_of (
    const PointSet *s)
{
    Magnus h = magnus_ex_s64(1);
    int    j;
    int    k;
    int    e;

    for (j = 0; j < s->n; j++) {
        for (k = 0; k < 4; k++) {
            for (e = 0; e < 2; e++) {
                Magnus d = fractio_denominator(algebraicus_coefficiens(
                    part_of(s->p[j], k), (i32)e, pool));
                Magnus g = magnus_divisor_communis(h, d, pool);
                Magnus q;

                (void)magnus_divide(d, g, pool, &q, NULL);
                h = magnus_multiplica(h, q, pool);
            }
        }
    }
    return h;
}

/* decimal text of a Magnus, shortened to its digit count if long */
static void
magnus_text (
    Magnus a,
    char  *out,
    int    size)
{
    chorda t = magnus_ad_chordam(a, pool);

    if (t.mensura <= 16) {
        snprintf(out, (size_t)size, "%.*s", (int)t.mensura,
            (const char *)t.datum);
    } else {
        snprintf(out, (size_t)size, "(%d digits)", (int)t.mensura);
    }
}

/* ================================================================
 * Directions (axis tables) as integer vectors
 * ================================================================ */

#define TABLE_SIZE 13

typedef struct {
    long v[TABLE_SIZE][3];
} Table;

static int
parallel (
    const long *u,
    const long *w)
{
    return u[1] * w[2] - u[2] * w[1] == 0 && u[2] * w[0] - u[0] * w[2] == 0
        && u[0] * w[1] - u[1] * w[0] == 0;
}

static long
dot3 (
    const long *u,
    const long *w)
{
    return u[0] * w[0] + u[1] * w[1] + u[2] * w[2];
}

/* largest cos^2 between two axes of a table, exactly, as num / den */
static void
max_cos2 (
    const Table *t,
    long        *num,
    long        *den)
{
    int a;
    int b;

    *num = 0;
    *den = 1;
    for (a = 0; a < TABLE_SIZE; a++) {
        for (b = a + 1; b < TABLE_SIZE; b++) {
            long d = dot3(t->v[a], t->v[b]);
            long n = d * d;
            long m = dot3(t->v[a], t->v[a]) * dot3(t->v[b], t->v[b]);

            if (n * *den > *num * m) {
                *num = n;
                *den = m;
            }
        }
    }
}

/* the table's axes as pure quaternions (0, v) in 'ring' */
static Quaternio *
table_axes (
    const Table  *t,
    const Anulus *ring)
{
    Quaternio *axes = (Quaternio *)piscina_allocare(pool,
        TABLE_SIZE * sizeof(Quaternio));
    int        a;
    char       text[128];

    for (a = 0; a < TABLE_SIZE; a++) {
        sprintf(text, "[0, %ld, %ld, %ld]", t->v[a][0], t->v[a][1],
            t->v[a][2]);
        axes[a] = qparse(ring, text);
    }
    return axes;
}

/* the direction of a nonzero vector part as an integer vector, if it is
 * a rational multiple of one (else 0) */
static int
rational_direction (
    Quaternio q,
    long     *out)
{
    Algebraicus first;
    int         k;
    int         f = 0;

    for (k = 1; k < 4 && f == 0; k++) {
        if (!algebraicus_est_nullum(part_of(q, k))) {
            f = k;
        }
    }
    if (f == 0) {
        return 0;
    }
    first = part_of(q, f);
    {
        Fractio r[3];
        Magnus  lcm = magnus_ex_s64(1);

        for (k = 1; k < 4; k++) {
            Algebraicus x;

            if (!algebraicus_divide(part_of(q, k), first, pool, &x)
                || !algebraicus_est_rationalis(x, pool, &r[k - 1])) {
                return 0;
            }
        }
        for (k = 0; k < 3; k++) {
            Magnus d = fractio_denominator(r[k]);
            Magnus g = magnus_divisor_communis(lcm, d, pool);
            Magnus quot;

            (void)magnus_divide(d, g, pool, &quot, NULL);
            lcm = magnus_multiplica(lcm, quot, pool);
        }
        for (k = 0; k < 3; k++) {
            Magnus q3;
            s64    v = 0;

            (void)magnus_divide(magnus_multiplica(fractio_numerator(r[k]),
                lcm, pool), fractio_denominator(r[k]), pool, &q3, NULL);
            if (!magnus_ad_s64(q3, &v)) {
                return 0;
            }
            out[k] = (long)v;
        }
    }
    return 1;
}

/* ================================================================
 * Seeded draws (D126's LCG)
 * ================================================================ */

static long rng_state;

static long
rng (
    long modulus)
{
    rng_state = (rng_state * 1103515245L + 12345L) & 0x7fffffffL;
    return rng_state % modulus;
}

/* ================================================================
 * Catalogs
 * ================================================================ */

static const s32 TWISTS_D82[4] = { 1, -1, 1, -1 };
static const s32 TWISTS_NONE[4] = { 0, 0, 0, 0 };

static Extensio  *q2;
static Extensio  *q5;
static Catalogus *cat_2o;        /* D119's zeta_8: 24 */
static Catalogus *cat_2i_full;   /* D94's 2I mod sign: 60 */
static Catalogus *cat_2i_d94;    /* D119: first 24 */
static Catalogus *cat_2i_b;      /* braid-correct pair: first 24 */
static Catalogus *cat_2t;        /* the 12 rational points of 2O */
static PointSet   set_2o;        /* D119's BFS signs */
static PointSet   set_2i_d94;
static PointSet   set_2i_b;
static PointSet   set_2t;
static PointSet   canon_2o;      /* first nonzero part positive */
static PointSet   canon_2i_d94;
static PointSet   canon_2i_b;
static PointSet   canon_2t;
static Table      t13;
static Table      r13;

/* D119/D120 zeta_8 (credo _zeta_octo_catalogus): D66's generators
 * sigma_1 -> (1 + i)/sqrt 2, sigma_2 -> (1 - k)/sqrt 2 and inverses,
 * twists (+1, -1, +1, -1) */
static Catalogus *
zeta8_catalog (void)
{
    const Anulus *ring = extensio_anulus(q2);
    Quaternio     g[4];
    Catalogus    *cat = NULL;

    g[0] = qparse(ring, "[a/2, a/2, 0, 0]");
    g[1] = qparse(ring, "[a/2, -a/2, 0, 0]");
    g[2] = qparse(ring, "[a/2, 0, 0, -a/2]");
    g[3] = qparse(ring, "[a/2, 0, 0, a/2]");
    if (!catalogus_clausura(g, TWISTS_D82, 4, 0, 16, pool, &cat)) {
        fatal("zeta_8 closure");
    }
    return cat;
}

/* closure of (x, x*, y, y*) in BFS order, no twists (D94 build_2i) */
static Catalogus *
close_pair (
    Quaternio x,
    Quaternio y)
{
    Quaternio  g[4];
    Catalogus *cat = NULL;

    g[0] = x;
    (void)quaternio_conjugatum(x, pool, &g[1]);
    g[2] = y;
    (void)quaternio_conjugatum(y, pool, &g[3]);
    if (!catalogus_clausura(g, TWISTS_NONE, 4, 0, 0, pool, &cat)) {
        return NULL;
    }
    return cat;
}

static Catalogus *
first_k (
    const Catalogus *cat,
    int              k)
{
    i32        idx[64];
    int        i;
    Catalogus *part = NULL;

    for (i = 0; i < k; i++) {
        idx[i] = (i32)i;
    }
    if (!catalogus_pars(cat, idx, (i32)k, pool, &part)) {
        fatal("catalogus_pars");
    }
    return part;
}

static int
braid_relation (
    Quaternio a,
    Quaternio b)
{
    return quaternio_aequalis(mul(mul(a, b), a), mul(mul(b, a), b));
}

/* ================================================================
 * Nulls
 * ================================================================ */

#define SEEDS 5        /* G, AX, AN, LIN, and the N = 12 nulls */
#define SEEDS_H 20     /* H6 and H5 at N = 24 (review M3: 5 were too few) */
#define RANDOM_2I 20   /* random 24-subsets of 2I (review M2: entry order) */
#define SIGN_DRAWS 5   /* random sign choices of 2O and 2I-D94 (review M4) */

static int
seeds_of (
    int kind)
{
    return kind < 2 ? SEEDS_H : SEEDS;
}
#define MAX_SPHERE 4096

static long sphere_v[MAX_SPHERE][4];

/* D126 Part F: v in Z^4 with |v|^2 = h^2, first nonzero entry positive,
 * lexicographic over [-h, h]^4 */
static int
sphere_points (
    long h)
{
    long a;
    long b;
    long c;
    long d;
    int  n = 0;

    for (a = -h; a <= h; a++) {
        for (b = -h; b <= h; b++) {
            for (c = -h; c <= h; c++) {
                for (d = -h; d <= h; d++) {
                    long first = a != 0 ? a : b != 0 ? b : c != 0 ? c : d;

                    if (a * a + b * b + c * c + d * d != h * h
                        || first < 0) {
                        continue;
                    }
                    if (n >= MAX_SPHERE) {
                        fatal("sphere too large");
                    }
                    sphere_v[n][0] = a;
                    sphere_v[n][1] = b;
                    sphere_v[n][2] = c;
                    sphere_v[n][3] = d;
                    n++;
                }
            }
        }
    }
    return n;
}

/* D126 Part F's subsets: seed formula, sphere order and LCG draws
 * identical, so these sets ARE D126's (and its oracle pins apply) */
static void
null_height (
    long      h,
    int       n,
    int       seed,
    PointSet *s)
{
    int  idx[MAX_SPHERE];
    int  m = sphere_points(h);
    int  i;
    char text[256];

    rng_state = 1000L * h + n + seed;
    for (i = 0; i < m; i++) {
        idx[i] = i;
    }
    snprintf(s->name, sizeof(s->name), "H%ld N%d seed %d", h, n, seed);
    s->ring = extensio_anulus(q2);
    s->n = n;
    for (i = 0; i < n; i++) {
        int k = i + (int)rng(m - i);
        int t = idx[i];

        idx[i] = idx[k];
        idx[k] = t;
        sprintf(text, "[%ld/%ld, %ld/%ld, %ld/%ld, %ld/%ld]",
            sphere_v[idx[i]][0], h, sphere_v[idx[i]][1], h,
            sphere_v[idx[i]][2], h, sphere_v[idx[i]][3], h);
        s->p[i] = qparse(s->ring, text);
    }
}

/* D126 Part D (b): inverse stereographic projection of a random rational
 * point (n_k/m_k), |n| <= 9, 1 <= m <= 9 */
static Quaternio
rational_unit (
    const Anulus *ring)
{
    long n[3];
    long m[3];
    long big_m;
    long u[3];
    long s;
    long den;
    int  k;
    char text[256];

    for (k = 0; k < 3; k++) {
        n[k] = rng(19) - 9;
        m[k] = 1 + rng(9);
    }
    big_m = m[0] * m[1] * m[2];
    s = 0;
    for (k = 0; k < 3; k++) {
        u[k] = n[k] * (big_m / m[k]);
        s += u[k] * u[k];
    }
    den = s + big_m * big_m;
    sprintf(text, "[%ld/%ld, %ld/%ld, %ld/%ld, %ld/%ld]",
        s - big_m * big_m, den, 2 * u[0] * big_m, den, 2 * u[1] * big_m,
        den, 2 * u[2] * big_m, den);
    return qparse(ring, text);
}

/* add q (made canonical) unless it repeats up to sign; 1 if added */
static int
add_point (
    PointSet *s,
    Quaternio q)
{
    q = canonical(q);
    if (contains(s, q)) {
        return 0;
    }
    s->p[s->n++] = q;
    return 1;
}

/* G: D126 Part D's rational nulls for 2O (kind b, seed + 100), draw for
 * draw */
static void
null_generic (
    int       seed,
    PointSet *s)
{
    rng_state = seed + 100;
    snprintf(s->name, sizeof(s->name), "G seed %d", seed);
    s->ring = extensio_anulus(q2);
    s->n = 0;
    while (s->n < canon_2o.n) {
        (void)add_point(s, rational_unit(s->ring));
    }
}

static long axis_of_2o[MAX_POINTS][3];   /* 0 0 0: the identity */
static int  has_axis_2o[MAX_POINTS];

/* AX: each 2O point replaced by a random rational angle on its own axis
 * u: (c, t u) with c = (m^2 - U n^2)/(m^2 + U n^2), t = 2 n m/(m^2 + U
 * n^2), U = |u|^2, so c^2 + U t^2 = 1. The identity stays. */
static void
null_axes (
    int       seed,
    PointSet *s)
{
    int  j;
    char text[256];

    rng_state = 127100L + seed;
    snprintf(s->name, sizeof(s->name), "AX seed %d", seed);
    s->ring = canon_2o.ring;
    s->n = 0;
    for (j = 0; j < canon_2o.n; j++) {
        if (!has_axis_2o[j]) {
            (void)add_point(s, canon_2o.p[j]);
            continue;
        }
        for (;;) {
            long n = rng(19) - 9;
            long m = 1 + rng(9);
            long u2 = dot3(axis_of_2o[j], axis_of_2o[j]);
            long den = m * m + u2 * n * n;

            if (n == 0) {
                continue;
            }
            sprintf(text, "[%ld/%ld, %ld/%ld, %ld/%ld, %ld/%ld]",
                m * m - u2 * n * n, den, 2 * n * m * axis_of_2o[j][0], den,
                2 * n * m * axis_of_2o[j][1], den,
                2 * n * m * axis_of_2o[j][2], den);
            if (add_point(s, qparse(s->ring, text))) {
                break;
            }
        }
    }
}

/* largest m with m^2 | x */
static long
square_part (
    long x)
{
    long m;
    long best = 1;

    for (m = 1; m * m <= x; m++) {
        if (x % (m * m) == 0) {
            best = m;
        }
    }
    return best;
}

/* AN: each 2O point (c, v) replaced by (c, s w/|w|), s = |v|, w a random
 * integer vector in [-9, 9]^3 with s/|w| in Q(sqrt 2) and w parallel to
 * no T13 axis. Real parts of 2O: 1, 0, 1/2, sqrt2/2, so s^2 = 1, 3/4,
 * 1/2 and |w|^2 must be m^2 or 2 m^2 (s^2 = 1, 1/2) or 3 m^2 or 6 m^2
 * (s^2 = 3/4). The identity stays. */
static void
null_angles (
    int       seed,
    PointSet *s)
{
    int  j;
    char text[256];

    rng_state = 127200L + seed;
    snprintf(s->name, sizeof(s->name), "AN seed %d", seed);
    s->ring = canon_2o.ring;
    s->n = 0;
    for (j = 0; j < canon_2o.n; j++) {
        Algebraicus c = part_of(canon_2o.p[j], 0);
        const char *c_text;
        int         kind;   /* 0: s^2 = 1, 1: s^2 = 3/4, 2: s^2 = 1/2 */

        if (!has_axis_2o[j]) {
            (void)add_point(s, canon_2o.p[j]);
            continue;
        }
        if (algebraicus_est_nullum(c)) {
            kind = 0;
            c_text = "0";
        } else if (algebraicus_aequalis(c, part_of(qparse(s->ring,
                       "[1/2, 0, 0, 0]"), 0))) {
            kind = 1;
            c_text = "1/2";
        } else if (algebraicus_aequalis(c, part_of(qparse(s->ring,
                       "[a/2, 0, 0, 0]"), 0))) {
            kind = 2;
            c_text = "a/2";
        } else {
            fatal("unexpected real part in 2O");
        }
        for (;;) {
            long        w[3];
            long        w2;
            long        m;
            long        k;
            char        scalar[64];
            int         t;
            int         clash = 0;
            Quaternio   v;
            Quaternio   wq;
            Algebraicus sigma;

            for (k = 0; k < 3; k++) {
                w[k] = rng(19) - 9;
            }
            w2 = dot3(w, w);
            if (w2 == 0) {
                continue;
            }
            for (t = 0; t < TABLE_SIZE; t++) {
                clash = clash || parallel(w, t13.v[t]);
            }
            if (clash) {
                continue;
            }
            m = square_part(w2);
            /* sigma = s / |w| */
            if (kind != 1 && w2 == m * m) {
                sprintf(scalar, kind == 0 ? "1/%ld" : "a/%ld",
                    kind == 0 ? m : 2 * m);
            } else if (kind != 1 && w2 == 2 * m * m) {
                sprintf(scalar, kind == 0 ? "a/%ld" : "1/%ld", 2 * m);
            } else if (kind == 1 && w2 == 3 * m * m) {
                sprintf(scalar, "1/%ld", 2 * m);
            } else if (kind == 1 && w2 == 6 * m * m) {
                sprintf(scalar, "a/%ld", 4 * m);
            } else {
                continue;
            }
            sprintf(text, "[%s, 0, 0, 0]", scalar);
            sigma = part_of(qparse(s->ring, text), 0);
            sprintf(text, "[0, %ld, %ld, %ld]", w[0], w[1], w[2]);
            wq = qparse(s->ring, text);
            (void)quaternio_scalari(wq, &sigma, pool, &v);
            sprintf(text, "[%s, 0, 0, 0]", c_text);
            (void)quaternio_adde(v, qparse(s->ring, text), pool, &v);
            if (add_point(s, v)) {
                break;
            }
        }
    }
}

static long
det4 (
    long a[4][4])
{
    long d = 0;
    int  col;

    /* Laplace on the first row (small integers: exact in long) */
    for (col = 0; col < 4; col++) {
        long minor = 0;
        int  r;
        int  c;
        int  ri = 0;
        long b[3][3];

        for (r = 1; r < 4; r++) {
            int ci = 0;

            for (c = 0; c < 4; c++) {
                if (c != col) {
                    b[ri][ci++] = a[r][c];
                }
            }
            ri++;
        }
        minor = b[0][0] * (b[1][1] * b[2][2] - b[1][2] * b[2][1])
            - b[0][1] * (b[1][0] * b[2][2] - b[1][2] * b[2][0])
            + b[0][2] * (b[1][0] * b[2][1] - b[1][1] * b[2][0]);
        d += (col % 2 == 0 ? 1 : -1) * a[0][col] * minor;
    }
    return d;
}

/* LIN: A p for every 2O point, A in GL4(Q), entries in [-3, 3] */
static void
null_linear (
    int       seed,
    PointSet *s,
    long     *det_out)
{
    long a[4][4];
    int  r;
    int  c;
    int  j;

    rng_state = 127300L + seed;
    do {
        for (r = 0; r < 4; r++) {
            for (c = 0; c < 4; c++) {
                a[r][c] = rng(7) - 3;
            }
        }
    } while (det4(a) == 0);
    *det_out = det4(a);
    snprintf(s->name, sizeof(s->name), "LIN seed %d", seed);
    s->ring = canon_2o.ring;
    s->n = 0;
    for (j = 0; j < canon_2o.n; j++) {
        Algebraicus x[4];

        for (r = 0; r < 4; r++) {
            x[r] = algebraicus_ex_fractione(algebraicus_corpus(part_of(
                canon_2o.p[j], 0)), fractio_ex_s64(0), pool);
            for (c = 0; c < 4; c++) {
                Algebraicus f = algebraicus_ex_fractione(algebraicus_corpus(
                    part_of(canon_2o.p[j], c)), fractio_ex_s64(a[r][c]),
                    pool);

                x[r] = algebraicus_adde(x[r], algebraicus_multiplica(f,
                    part_of(canon_2o.p[j], c), pool), pool);
            }
        }
        {
            Quaternio q;

            if (!quaternio_ex_partibus(s->ring, &x[0], &x[1], &x[2],
                    &x[3], pool, &q)) {
                fatal("LIN point");
            }
            if (!add_point(s, q)) {
                fatal("LIN: A is invertible, so no repeat is possible");
            }
        }
    }
}

/* 2I-R: 24 random points of 2I (canonical), in increasing catalog order
 * (review M2: does a "2I" result belong to 2I or to one prefix?) */
static void
random_2i (
    int       seed,
    PointSet *s)
{
    int idx[64];
    int m = (int)catalogus_numerus(cat_2i_full);
    int i;
    int k;

    rng_state = 127400L + seed;
    for (i = 0; i < m; i++) {
        idx[i] = i;
    }
    for (i = 0; i < 24; i++) {
        int t;

        k = i + (int)rng(m - i);
        t = idx[i];
        idx[i] = idx[k];
        idx[k] = t;
    }
    for (i = 1; i < 24; i++) {
        int x = idx[i];

        for (k = i; k > 0 && idx[k - 1] > x; k--) {
            idx[k] = idx[k - 1];
        }
        idx[k] = x;
    }
    snprintf(s->name, sizeof(s->name), "2I-R seed %d", seed);
    s->ring = quaternio_anulus(catalogus_elementum(cat_2i_full, 0));
    s->n = 24;
    for (i = 0; i < 24; i++) {
        s->p[i] = canonical(catalogus_elementum(cat_2i_full, (i32)idx[i]));
    }
}

/* the same points, each negated with probability 1/2 (review M4: AND
 * and MAJ depend on these signs); returns the number flipped */
static int
signed_copy (
    const PointSet *base,
    long            seed0,
    int             seed,
    const char     *label,
    PointSet       *s)
{
    int k;
    int flipped = 0;

    *s = *base;
    snprintf(s->name, sizeof(s->name), "%s sgn %d", label, seed);
    rng_state = seed0 + seed;
    for (k = 0; k < base->n; k++) {
        /* a high bit: the LCG's lowest bit only alternates */
        (void)rng(2);
        if (((rng_state >> 16) & 1) == 1) {
            s->p[k] = negate(base->p[k]);
            flipped++;
        }
    }
    return flipped;
}

/* R13: 13 random integer directions in [-9, 9]^3, pairwise non-parallel,
 * none parallel to a T13 axis; fixed seed */
static void
group_free_table (void)
{
    int n = 0;

    rng_state = 127013L;
    while (n < TABLE_SIZE) {
        long w[3];
        int  k;
        int  clash = 0;

        for (k = 0; k < 3; k++) {
            w[k] = rng(19) - 9;
        }
        if (dot3(w, w) == 0) {
            continue;
        }
        for (k = 0; k < n; k++) {
            clash = clash || parallel(w, r13.v[k]);
        }
        for (k = 0; k < TABLE_SIZE; k++) {
            clash = clash || parallel(w, t13.v[k]);
        }
        if (!clash) {
            r13.v[n][0] = w[0];
            r13.v[n][1] = w[1];
            r13.v[n][2] = w[2];
            n++;
        }
    }
}

/* ================================================================
 * Capacity rows (shared with Part C)
 * ================================================================ */

typedef struct {
    long sets;
    long rule;
    long robust;
    long possible;
    long tied;
    long clash;       /* concursus */
    long undecided;
} Row;

static const i8 *
truth_table (
    int function,
    int n)
{
    return function == 0 ? cellae_paritas((i32)n, pool)
        : function == 1 ? cellae_et((i32)n, pool)
        : cellae_maior((i32)n, pool);
}

static const char *FUNCTION_NAME[3] = { "XOR", "AND", "MAJ" };
static const i32   LADDER[3] = { 6, 12, 24 };

/* every n-subset of [0, points) in lexicographic order */
static int
next_subset (
    i32 *w,
    int  n,
    int  points)
{
    int j = n;
    int l;

    while (j > 0 && (int)w[j - 1] == points - n + j - 1) {
        j--;
    }
    if (j == 0) {
        return 0;
    }
    w[j - 1]++;
    for (l = j; l < n; l++) {
        w[l] = w[l - 1] + 1;
    }
    return 1;
}

static void
run_rows (
    Activatio *act,
    int        points,
    int        n,
    int        function,
    Row       *r)
{
    const i8 *tv = truth_table(function, n);
    i32       w[8];
    int       j;

    memset(r, 0, sizeof(*r));
    for (j = 0; j < n; j++) {
        w[j] = (i32)j;
    }
    do {
        Iudicium x;

        if (!activatio_iudica(act, w, (i32)n, LADDER, 3, tv, &x)) {
            fatal("activatio_iudica refused");
        }
        r->sets++;
        r->rule += x.regula ? 1 : 0;
        r->robust += x.robusta ? 1 : 0;
        r->possible += x.possibilis ? 1 : 0;
        r->tied += x.ligata ? 1 : 0;
        r->clash += x.concursus ? 1 : 0;
        r->undecided += x.indecisa ? 1 : 0;
    } while (next_subset(w, n, points));
}

/* ================================================================
 * Relation profiles (as D126)
 * ================================================================ */

#define MAX_R 16
#define STORE_LIMIT 200000L
#define NODE_BUDGET 40000000L

typedef struct {
    int  ambient_rank;
    int  rank;
    int  rows_checked;
    int  radius;
    int  complete;
    int  classified;
    long count;
    long by_norm[MAX_R + 1];
    long pm1_by_norm[MAX_R + 1];
    long odd_pm1;             /* entries in {-1, 0, 1}, odd norm */
    long pm1;
} Profile;

static Matrix
coordinates (
    const PointSet *s)
{
    Matrix a;
    int    j;
    int    k;
    int    e;

    if (!matrix_nulla(&ANULUS_RATIONALIUM, 8, (i32)s->n, pool, &a)) {
        fatal("matrix");
    }
    for (j = 0; j < s->n; j++) {
        for (k = 0; k < 4; k++) {
            for (e = 0; e < 2; e++) {
                Fractio f = algebraicus_coefficiens(part_of(s->p[j], k),
                    (i32)e, pool);

                matrix_pone(&a, (i32)(2 * k + e), (i32)j, &f);
            }
        }
    }
    return a;
}

static int
is_relation (
    const PointSet *s,
    Matrix          r,
    int             row)
{
    Quaternio sum;
    int       j;

    (void)quaternio_nullum(s->ring, pool, &sum);
    for (j = 0; j < s->n; j++) {
        const Magnus *x = (const Magnus *)matrix_elementum(r, (i32)row,
            (i32)j);
        Quaternio     t;
        Algebraicus   c;

        if (magnus_signum(*x) == 0) {
            continue;
        }
        c = algebraicus_ex_fractione(algebraicus_corpus(part_of(s->p[j],
            0)), fractio_ex_magno(*x), pool);
        (void)quaternio_scalari(s->p[j], &c, pool, &t);
        (void)quaternio_adde(sum, t, pool, &sum);
    }
    return quaternio_est_nullum(sum);
}

static void
profile (
    const PointSet *s,
    int             radius,
    Profile        *pr)
{
    Matrix              a = coordinates(s);
    Matrix              basis;
    ReticulumQuaestio   q;
    ReticulumEnumeratio e;
    i32                 g = 0;
    int                 i;
    int                 j;

    memset(pr, 0, sizeof(*pr));
    pr->radius = radius;
    if (!matrix_gradus(a, pool, &g)) {
        fatal("rank");
    }
    pr->ambient_rank = (int)g;
    if (!reticulum_relationes(a, NULL, pool, &basis)) {
        fatal("reticulum_relationes refused");
    }
    pr->rank = (int)matrix_lineae(basis);
    for (i = 0; i < pr->rank; i++) {
        pr->rows_checked += is_relation(s, basis, i);
    }
    q.radius_quadratus = magnus_ex_s64(radius);
    q.limes_servandi = (i32)STORE_LIMIT;
    q.limes_nodorum = (i64)NODE_BUDGET;
    if (!reticulum_enumera(basis, &q, pool, &e)) {
        fatal("reticulum_enumera refused");
    }
    pr->complete = (int)e.completa;
    pr->count = (long)e.numerus;
    pr->classified = e.servati == (i32)e.numerus;
    for (i = 0; i < (int)e.servati; i++) {
        s64 n = 0;
        int pm1 = 1;

        (void)magnus_ad_s64(e.normae[i], &n);
        pr->by_norm[n]++;
        for (j = 0; j < s->n; j++) {
            s64 v = 0;

            (void)magnus_ad_s64(*(const Magnus *)matrix_elementum(
                e.vectores, (i32)i, (i32)j), &v);
            pm1 = pm1 && v >= -1 && v <= 1;
        }
        pr->pm1_by_norm[n] += pm1;
        pr->pm1 += pm1;
        pr->odd_pm1 += pm1 && n % 2 == 1;
    }
}

static int
same_profile (
    const Profile *a,
    const Profile *b)
{
    int n;

    if (a->rank != b->rank || a->ambient_rank != b->ambient_rank
        || a->count != b->count || !a->complete || !b->complete
        || !a->classified || !b->classified) {
        return 0;
    }
    for (n = 0; n <= a->radius; n++) {
        if (a->by_norm[n] != b->by_norm[n]
            || a->pm1_by_norm[n] != b->pm1_by_norm[n]) {
            return 0;
        }
    }
    return 1;
}

/* ================================================================
 * Part A: construction and checks
 * ================================================================ */

static PointSet nulls24[6][SEEDS_H];
static PointSet nulls12[2][SEEDS];
static PointSet rand_2i[RANDOM_2I];
static PointSet sign_2o[SIGN_DRAWS];
static PointSet sign_2i[SIGN_DRAWS];
static const char *KIND_NAME[6] = { "H6", "H5", "G", "AX", "AN", "LIN" };
static long        lin_det[SEEDS];

static void register_sets (void);
static void registry_check (void);

/* D119's frozen exact rows (D119_B zeta_8, D119_C 2I first 24): sets,
 * float, rule, robust, possible, tied; N = 3 then 4, XOR / AND / MAJ */
static const long D119_ZETA[6][6] = {
    { 2024, 1480, 1456, 1456, 1938, 1217 },
    { 2024, 1907, 1799, 1799, 1970, 1217 },
    { 2024, 1494, 1462, 1462, 1968, 1217 },
    { 10626, 8010, 7908, 7696, 8678, 6770 },
    { 10626, 9723, 9486, 9402, 10046, 6770 },
    { 10626, 7156, 6797, 6611, 8666, 6770 } };
static const long D119_AUREA[6][6] = {
    { 2024, 1580, 1580, 1580, 1960, 1181 },
    { 2024, 1930, 1930, 1930, 1976, 1181 },
    { 2024, 1596, 1596, 1596, 1994, 1181 },
    { 10626, 9114, 9030, 8958, 9399, 7225 },
    { 10626, 10055, 10050, 9983, 10252, 7225 },
    { 10626, 8089, 8031, 7953, 9408, 7225 } };

static int
anchor_matches (
    const Catalogus *cat,
    const Quaternio *axes,     /* NULL: the catalog's own table */
    const long       pins[6][6],
    const char      *name)
{
    Activatio *act = NULL;
    int        n;
    int        f;
    int        ok = 1;

    if (!activatio_creare(cat, axes, axes == NULL ? 0 : TABLE_SIZE,
            CELLAE_CELLA_D94, 0, pool, &act)) {
        fatal("anchor activation");
    }
    for (n = 3; n <= 4; n++) {
        for (f = 0; f < 3; f++) {
            const long *pin = pins[(n - 3) * 3 + f];
            Row         r;

            run_rows(act, (int)catalogus_numerus(cat), n, f, &r);
            printf("    %s N=%d %s: sets %ld rule %ld robust %ld possible"
                " %ld tied %ld concursus %ld undecided %ld\n", name, n,
                FUNCTION_NAME[f], r.sets, r.rule, r.robust, r.possible,
                r.tied, r.clash, r.undecided);
            ok = ok && r.sets == pin[0] && r.rule == pin[2]
                && r.robust == pin[3] && r.possible == pin[4]
                && r.tied == pin[5] && r.undecided == 0;
        }
    }
    return ok;
}

static void
part_a (void)
{
    const Anulus *ring2;
    const Anulus *ring5;
    Quaternio     s_gen;
    Quaternio     t_gen;
    Quaternio     braid_a;
    Quaternio     braid_b;
    int           found = 0;
    int           i;
    int           j;
    int           k;
    int           overlap = 0;
    int           ok;
    char          msg[512];

    printf("\n=== Part A: construction and checks ===\n");
    q2 = extensio_quadratica(2, pool);
    q5 = extensio_quadratica(5, pool);
    ring2 = extensio_anulus(q2);
    ring5 = extensio_anulus(q5);

    /* --- catalogs --- */
    cat_2o = zeta8_catalog();
    from_catalog(cat_2o, "2O (zeta_8)", &set_2o);
    s_gen = qparse(ring5, "[1/2, 1/2, 1/2, 1/2]");
    t_gen = qparse(ring5, "[(a + 1)/4, (a - 1)/4, 1/2, 0]");
    cat_2i_full = close_pair(s_gen, t_gen);
    if (cat_2i_full == NULL) {
        fatal("2I closure");
    }
    cat_2i_d94 = first_k(cat_2i_full, 24);
    from_catalog(cat_2i_d94, "2I-D94 first 24", &set_2i_d94);
    ok = 1;
    for (i = 0; i < set_2o.n; i++) {
        ok = ok && is_unit(set_2o.p[i]);
    }
    for (i = 0; i < (int)catalogus_numerus(cat_2i_full); i++) {
        ok = ok && is_unit(catalogus_elementum(cat_2i_full, (i32)i));
    }
    sprintf(msg, "2O: %d points, 2I: %d points mod sign (24, 60), every "
        "point unit", set_2o.n, (int)catalogus_numerus(cat_2i_full));
    check(msg, ok && set_2o.n == 24 && catalogus_numerus(cat_2i_full)
        == 60);
    {
        int f_2o = canonical_set(&set_2o, &canon_2o);
        int f_2i = canonical_set(&set_2i_d94, &canon_2i_d94);

        printf("    BFS signs (D119) against the canonical rule: %d of 2O's"
            " 24 and %d of 2I-D94's 24 points differ in sign\n", f_2o,
            f_2i);
    }
    sprintf(msg, "D94's pair s, t has orders %d and %d and does NOT "
        "satisfy sts = tst (KC 53.3: not a braid representation)",
        order_of(s_gen, 20), order_of(t_gen, 20));
    check(msg, order_of(s_gen, 20) == 6 && order_of(t_gen, 20) == 10
        && !braid_relation(s_gen, t_gen));

    /* braid-correct pair: the first (in 2I's order) pair of order-10
     * elements (real part cos 36 = (1 + sqrt5)/4) with aba = bab
     * (note 41: axes at cos phi = 1/sqrt5) */
    {
        Algebraicus cos36 = part_of(qparse(ring5, "[(a + 1)/4, 0, 0, 0]"),
            0);
        int         n60 = (int)catalogus_numerus(cat_2i_full);

        for (i = 0; i < n60 && !found; i++) {
            Quaternio a = canonical(catalogus_elementum(cat_2i_full,
                (i32)i));

            if (!algebraicus_aequalis(part_of(a, 0), cos36)) {
                continue;
            }
            for (j = i + 1; j < n60 && !found; j++) {
                Quaternio b = canonical(catalogus_elementum(cat_2i_full,
                    (i32)j));

                if (algebraicus_aequalis(part_of(b, 0), cos36)
                    && braid_relation(a, b)) {
                    braid_a = a;
                    braid_b = b;
                    found = 1;
                    printf("    braid pair: 2I elements %d and %d\n", i,
                        j);
                }
            }
        }
    }
    if (!found) {
        fatal("no braid-correct pair of order-10 elements in 2I");
    }
    {
        Catalogus *full_b = close_pair(braid_a, braid_b);
        Algebraicus inner;
        Algebraicus n_a;
        Algebraicus n_b;
        Algebraicus lhs;
        Algebraicus rhs;
        Algebraicus five = part_of(qparse(ring5, "[5, 0, 0, 0]"), 0);

        /* cos^2 phi = (u.v)^2 / (|u|^2 |v|^2) = 1/5 */
        inner = algebraicus_adde(algebraicus_adde(algebraicus_multiplica(
            part_of(braid_a, 1), part_of(braid_b, 1), pool),
            algebraicus_multiplica(part_of(braid_a, 2), part_of(braid_b, 2),
            pool), pool), algebraicus_multiplica(part_of(braid_a, 3),
            part_of(braid_b, 3), pool), pool);
        n_a = algebraicus_adde(algebraicus_adde(algebraicus_multiplica(
            part_of(braid_a, 1), part_of(braid_a, 1), pool),
            algebraicus_multiplica(part_of(braid_a, 2), part_of(braid_a, 2),
            pool), pool), algebraicus_multiplica(part_of(braid_a, 3),
            part_of(braid_a, 3), pool), pool);
        n_b = algebraicus_adde(algebraicus_adde(algebraicus_multiplica(
            part_of(braid_b, 1), part_of(braid_b, 1), pool),
            algebraicus_multiplica(part_of(braid_b, 2), part_of(braid_b, 2),
            pool), pool), algebraicus_multiplica(part_of(braid_b, 3),
            part_of(braid_b, 3), pool), pool);
        lhs = algebraicus_multiplica(five, algebraicus_multiplica(inner,
            inner, pool), pool);
        rhs = algebraicus_multiplica(n_a, n_b, pool);
        sprintf(msg, "2I-B: both generators have order %d and %d, aba = "
            "bab exactly, axes at cos^2 phi = 1/5 (note 41), closure of "
            "(a, a*, b, b*) = %d points mod sign",
            order_of(braid_a, 20), order_of(braid_b, 20),
            full_b == NULL ? -1 : (int)catalogus_numerus(full_b));
        check(msg, full_b != NULL && order_of(braid_a, 20) == 10
            && order_of(braid_b, 20) == 10 && braid_relation(braid_a,
            braid_b) && algebraicus_aequalis(lhs, rhs)
            && catalogus_numerus(full_b) == 60);
        if (full_b == NULL) {
            fatal("2I-B closure");
        }
        cat_2i_b = first_k(full_b, 24);
        from_catalog(cat_2i_b, "2I-B first 24", &set_2i_b);
        (void)canonical_set(&set_2i_b, &canon_2i_b);
        for (i = 0; i < 24; i++) {
            overlap += contains_up_to_sign(&set_2i_d94, set_2i_b.p[i]);
        }
        sprintf(msg, "2I-B's first 24 differ from 2I-D94's first 24 "
            "(shared: %d of 24)", overlap);
        check(msg, overlap < 24);
    }

    /* 2T: the rational points of 2O */
    {
        i32 idx[24];
        int m = 0;

        for (i = 0; i < canon_2o.n; i++) {
            int rational = 1;

            for (k = 0; k < 4; k++) {
                rational = rational && algebraicus_est_rationalis(part_of(
                    canon_2o.p[i], k), pool, NULL);
            }
            if (rational) {
                idx[m++] = (i32)i;
            }
        }
        if (!catalogus_pars(cat_2o, idx, (i32)m, pool, &cat_2t)) {
            fatal("2T");
        }
        from_catalog(cat_2t, "2T (in 2O)", &set_2t);
        (void)canonical_set(&set_2t, &canon_2t);
        sprintf(msg, "2T = the %d rational points of 2O, closed under "
            "multiplication up to sign", set_2t.n);
        check(msg, set_2t.n == 12 && closed_with_signs(&set_2t));
    }

    /* --- tables --- */
    {
        Activatio *own = NULL;
        int        types[3] = { 0, 0, 0 };   /* 1, 3, 2 nonzero entries */
        long       num;
        long       den;
        long       num_r;
        long       den_r;

        if (!activatio_creare(cat_2o, NULL, 0, CELLAE_CELLA_D94, 0, pool,
                &own) || activatio_axes(own) != TABLE_SIZE) {
            fatal("2O's own table");
        }
        ok = 1;
        for (i = 0; i < TABLE_SIZE; i++) {
            int nonzero = 0;

            ok = ok && rational_direction(activatio_axis(own, (i32)i),
                t13.v[i]);
            for (k = 0; k < 3; k++) {
                nonzero += t13.v[i][k] != 0;
                ok = ok && t13.v[i][k] >= -1 && t13.v[i][k] <= 1;
            }
            if (nonzero >= 1 && nonzero <= 3) {
                types[nonzero - 1]++;
            }
        }
        sprintf(msg, "T13 = 2O's own table in its order, as integer "
            "directions: %d coordinate axes, %d face diagonals, %d body "
            "diagonals (3, 6, 4)", types[0], types[1], types[2]);
        check(msg, ok && types[0] == 3 && types[1] == 6 && types[2] == 4);
        group_free_table();
        max_cos2(&t13, &num, &den);
        max_cos2(&r13, &num_r, &den_r);
        printf("    R13 (seed 127013):");
        for (i = 0; i < TABLE_SIZE; i++) {
            printf(" (%ld,%ld,%ld)", r13.v[i][0], r13.v[i][1],
                r13.v[i][2]);
        }
        printf("\n    largest cos^2 between two axes: T13 %ld/%ld, R13 "
            "%ld/%ld (exact)\n", num, den, num_r, den_r);
        ok = 1;
        for (i = 0; i < TABLE_SIZE; i++) {
            for (j = 0; j < TABLE_SIZE; j++) {
                ok = ok && !parallel(r13.v[i], t13.v[j])
                    && (i == j || !parallel(r13.v[i], r13.v[j]));
            }
        }
        check("R13: 13 pairwise non-parallel integer directions, none "
            "parallel to a T13 axis", ok);
        {
            Activatio *x = NULL;
            int        accepted = 1;

            accepted = accepted && activatio_creare(cat_2i_d94,
                table_axes(&t13, ring5), TABLE_SIZE, CELLAE_CELLA_D94, 0,
                pool, &x) && activatio_axes(x) == TABLE_SIZE;
            accepted = accepted && activatio_creare(cat_2i_d94,
                table_axes(&r13, ring5), TABLE_SIZE, CELLAE_CELLA_D94, 0,
                pool, &x) && activatio_axes(x) == TABLE_SIZE;
            accepted = accepted && activatio_creare(cat_2o,
                table_axes(&r13, ring2), TABLE_SIZE, CELLAE_CELLA_D94, 0,
                pool, &x) && activatio_axes(x) == TABLE_SIZE;
            check("T13 and R13 written into Q(sqrt 5) and Q(sqrt 2): "
                "activations accepted, 13 axes each", accepted);
        }
        for (i = 0; i < canon_2o.n; i++) {
            has_axis_2o[i] = rational_direction(canon_2o.p[i],
                axis_of_2o[i]);
        }
    }

    /* --- nulls --- */
    {
        int units = 1;
        int canon = 1;
        int distinct = 1;
        int closed = 0;
        int ax_ok = 1;
        int an_ok = 1;
        int lin_unit = 0;
        int seed;
        int kind;

        for (seed = 0; seed < SEEDS_H; seed++) {
            null_height(6, 24, seed, &nulls24[0][seed]);
            null_height(5, 24, seed, &nulls24[1][seed]);
        }
        for (seed = 0; seed < SEEDS; seed++) {
            null_generic(seed + 1, &nulls24[2][seed]);
            null_axes(seed, &nulls24[3][seed]);
            null_angles(seed, &nulls24[4][seed]);
            null_linear(seed, &nulls24[5][seed], &lin_det[seed]);
            null_height(6, 12, seed, &nulls12[0][seed]);
            null_height(5, 12, seed, &nulls12[1][seed]);
        }
        for (kind = 0; kind < 6; kind++) {
            for (seed = 0; seed < seeds_of(kind); seed++) {
                PointSet *s = &nulls24[kind][seed];

                for (i = 0; i < s->n; i++) {
                    if (kind == 5) {
                        lin_unit += is_unit(s->p[i]);
                    } else {
                        units = units && is_unit(s->p[i]);
                    }
                    canon = canon && is_canonical(s->p[i]);
                }
                distinct = distinct && s->n == 24 && as_catalog(s) != NULL;
                closed += closed_with_signs(s);
            }
        }
        for (kind = 0; kind < 2; kind++) {
            for (seed = 0; seed < SEEDS; seed++) {
                PointSet *s = &nulls12[kind][seed];

                for (i = 0; i < s->n; i++) {
                    units = units && is_unit(s->p[i]);
                    canon = canon && is_canonical(s->p[i]);
                }
                distinct = distinct && s->n == 12 && as_catalog(s) != NULL;
                closed += closed_with_signs(s);
            }
        }
        sprintf(msg, "%d null sets: N as matched, distinct up to sign "
            "(catalogus_ex_punctis accepts), canonical; unit except LIN; "
            "none closed under multiplication (%d; near-vacuous for "
            "random sets)", 2 * SEEDS_H + 6 * SEEDS, closed);
        check(msg, units && canon && distinct && closed == 0);
        for (seed = 0; seed < SEEDS; seed++) {
            const PointSet *ax = &nulls24[3][seed];
            const PointSet *an = &nulls24[4][seed];

            for (i = 0; i < canon_2o.n; i++) {
                long d[3];
                int  t;

                /* AX point i keeps 2O point i's axis (canonical sign
                 * may flip the vector, not its direction) */
                if (has_axis_2o[i]) {
                    ax_ok = ax_ok && rational_direction(ax->p[i], d)
                        && parallel(d, axis_of_2o[i]);
                    an_ok = an_ok && algebraicus_aequalis(part_of(an->p[i],
                        0), part_of(canon_2o.p[i], 0));
                    if (rational_direction(an->p[i], d)) {
                        for (t = 0; t < TABLE_SIZE; t++) {
                            an_ok = an_ok && !parallel(d, t13.v[t]);
                        }
                    }
                } else {
                    ax_ok = ax_ok && quaternio_aequalis(ax->p[i],
                        canon_2o.p[i]);
                    an_ok = an_ok && quaternio_aequalis(an->p[i],
                        canon_2o.p[i]);
                }
            }
        }
        check("AX: every point on its 2O point's axis (13 directions = "
            "T13), the identity kept", ax_ok);
        check("AN: every point has its 2O point's real part (the angle "
            "multiset of 2O), and no axis parallel to a T13 axis", an_ok);
        sprintf(msg, "LIN: det A = %ld %ld %ld %ld %ld, every seed "
            "invertible; unit points among 120: %d (LIN changes norms, "
            "not only directions)", lin_det[0], lin_det[1], lin_det[2],
            lin_det[3], lin_det[4], lin_unit);
        check(msg, lin_det[0] != 0 && lin_det[1] != 0 && lin_det[2] != 0
            && lin_det[3] != 0 && lin_det[4] != 0 && lin_unit < 120);
    }

    /* --- entry-order and sign controls (review M2, M4) --- */
    {
        int seed;
        int ok_r = 1;
        int ok_s = 1;
        int best = 0;
        int flips[2][SIGN_DRAWS];

        for (seed = 0; seed < RANDOM_2I; seed++) {
            int shared = 0;

            random_2i(seed, &rand_2i[seed]);
            ok_r = ok_r && rand_2i[seed].n == 24
                && as_catalog(&rand_2i[seed]) != NULL;
            for (i = 0; i < 24; i++) {
                ok_r = ok_r && is_canonical(rand_2i[seed].p[i]);
                shared += contains(&canon_2i_d94, rand_2i[seed].p[i]);
            }
            best = shared > best ? shared : best;
        }
        sprintf(msg, "2I-R: %d random 24-subsets of 2I (canonical, distinct,"
            " accepted as catalogs); at most %d of 24 points shared with "
            "2I-D94's first 24", RANDOM_2I, best);
        check(msg, ok_r && best < 24);
        for (seed = 0; seed < SIGN_DRAWS; seed++) {
            flips[0][seed] = signed_copy(&canon_2o, 127500L, seed, "2O",
                &sign_2o[seed]);
            flips[1][seed] = signed_copy(&canon_2i_d94, 127600L, seed,
                "2I-D94", &sign_2i[seed]);
            {
                int differ_o = 0;
                int differ_i = 0;

                for (i = 0; i < 24; i++) {
                    ok_s = ok_s && quaternio_aequalis(canonical(
                        sign_2o[seed].p[i]), canon_2o.p[i])
                        && quaternio_aequalis(canonical(sign_2i[seed].p[i]),
                        canon_2i_d94.p[i]);
                    differ_o += !quaternio_aequalis(sign_2o[seed].p[i],
                        canon_2o.p[i]);
                    differ_i += !quaternio_aequalis(sign_2i[seed].p[i],
                        canon_2i_d94.p[i]);
                }
                /* a draw that flips nothing would be no control */
                ok_s = ok_s && differ_o == flips[0][seed] && differ_o > 0
                    && differ_i == flips[1][seed] && differ_i > 0;
            }
        }
        sprintf(msg, "sign draws: 2O and 2I-D94 with random signs (flips "
            "%d %d %d %d %d and %d %d %d %d %d), the same points up to "
            "sign", flips[0][0], flips[0][1], flips[0][2], flips[0][3],
            flips[0][4], flips[1][0], flips[1][1], flips[1][2],
            flips[1][3], flips[1][4]);
        check(msg, ok_s);
    }
    register_sets();
    registry_check();

    /* --- anchor (not in a worker: it only needs the sets) --- */
    if (in_worker) {
        return;
    }
    printf("  D119 anchor (own tables, ladder {6, 12, 24}):\n");
    check("2O with its own table reproduces D119's exact rows N = 3-4 "
        "(sets, rule, robust, possible, tied; 0 undecided)",
        anchor_matches(cat_2o, NULL, D119_ZETA, "2O"));
    check("2I-D94 with its own table reproduces D119's exact rows N = "
        "3-4", anchor_matches(cat_2i_d94, NULL, D119_AUREA, "2I-D94"));
    check("2O judged with T13 (integer directions, written anew) "
        "reproduces the same rows: T13 IS 2O's table, in its order",
        anchor_matches(cat_2o, table_axes(&t13, extensio_anulus(q2)),
        D119_ZETA, "2O, T13"));
}

/* ================================================================
 * Part B: relation profiles
 * ================================================================ */

/* D126's pins (oracle.py, oracle.py -F): 2O and 2T +-1 shells; Part F
 * rows (6, 24), (5, 24), (6, 12), (5, 12): +-1 total, norm 3, odd +-1 */
static const long PIN_2O_PM1[9] = { 0, 0, 0, 32, 72, 96, 704, 2592, 5976 };
static const long PIN_2T_PM1[11] = { 0, 0, 0, 16, 36, 48, 96, 144, 156,
    112, 0 };
static const long PIN_H6_24[3][8] = {
    { 4559, 4561, 4427, 4459, 4346, 4583, 4301, 4228 },
    { 1, 1, 1, 1, 1, 2, 1, 1 },
    { 1025, 1046, 1001, 999, 991, 1045, 962, 975 } };
static const long PIN_H5_24_PM1[3] = { 1750, 1662, 1882 };
static const long PIN_H6_12[3][5] = {
    { 12, 21, 10, 14, 6 }, { 0, 0, 0, 0, 0 }, { 0, 13, 4, 7, 0 } };
static const long PIN_H5_12_PM1[3] = { 3, 2, 2 };

static long pm1_null24[6][SEEDS_H];   /* +-1 relations within R (Part C) */
static long pm1_null12[2][SEEDS];

static void
print_profile_row (
    const PointSet *s,
    const Profile  *pr)
{
    char h[64];

    magnus_text(height_of(s), h, sizeof(h));
    printf("    %-18s N %2d  height %-14s ambient %d  rank %2d  norm 3 "
        "%4ld  odd +-1 %5ld  +-1 %6ld  (R = %d%s)\n", s->name, s->n, h,
        pr->ambient_rank, pr->rank, pr->by_norm[3], pr->odd_pm1, pr->pm1,
        pr->radius, pr->complete && pr->classified ? ""
        : ", INCOMPLETE");
}

static void
part_b (void)
{
    Profile pr_2o;
    Profile pr;
    int     seed;
    int     kind;
    int     n;
    int     ok;
    int     pins = 1;
    int     sound = 1;
    long    g_total = 0;
    char    msg[256];

    printf("\n=== Part B: relation profiles (signed encoding: entries "
        "in {-1, 0, 1}) ===\n");
    profile(&set_2o, 8, &pr_2o);
    print_profile_row(&set_2o, &pr_2o);
    ok = pr_2o.complete && pr_2o.classified;
    for (n = 1; n <= 8; n++) {
        ok = ok && pr_2o.pm1_by_norm[n] == PIN_2O_PM1[n];
    }
    check("2O's {-1, 0, 1} shells to norm 8 = D126's oracle pins", ok);
    profile(&set_2i_d94, 8, &pr);
    print_profile_row(&set_2i_d94, &pr);
    sound = sound && pr.complete && pr.rows_checked == pr.rank;
    profile(&set_2i_b, 8, &pr);
    print_profile_row(&set_2i_b, &pr);
    sound = sound && pr.complete && pr.rows_checked == pr.rank;
    profile(&set_2t, 10, &pr);
    print_profile_row(&set_2t, &pr);
    ok = pr.complete && pr.classified;
    for (n = 1; n <= 10; n++) {
        ok = ok && pr.pm1_by_norm[n] == PIN_2T_PM1[n];
    }
    check("2T's {-1, 0, 1} shells to norm 10 = D126's oracle pins", ok);
    for (kind = 0; kind < 6; kind++) {
        printf("  %s (N = 24):\n", KIND_NAME[kind]);
        for (seed = 0; seed < seeds_of(kind); seed++) {
            const PointSet *s = &nulls24[kind][seed];

            profile(s, 8, &pr);
            print_profile_row(s, &pr);
            pm1_null24[kind][seed] = pr.pm1;
            sound = sound && pr.complete && pr.classified
                && pr.rows_checked == pr.rank
                && pr.rank == s->n - pr.ambient_rank;
            if (kind == 0 && seed < 8) {
                pins = pins && pr.pm1 == PIN_H6_24[0][seed]
                    && pr.by_norm[3] == PIN_H6_24[1][seed]
                    && pr.odd_pm1 == PIN_H6_24[2][seed];
            }
            if (kind == 1) {
                pins = pins && pr.odd_pm1 == 0 && pr.by_norm[3] == 0
                    && (seed >= 3 || pr.pm1 == PIN_H5_24_PM1[seed]);
            }
            if (kind == 2) {
                g_total += pr.count;
            }
            if (kind == 5) {
                if (!same_profile(&pr, &pr_2o)) {
                    sound = 0;
                    printf("    LIN seed %d: profile differs from 2O's\n",
                        seed);
                }
            }
        }
    }
    for (kind = 0; kind < 2; kind++) {
        printf("  %s (N = 12):\n", KIND_NAME[kind]);
        for (seed = 0; seed < SEEDS; seed++) {
            const PointSet *s = &nulls12[kind][seed];

            profile(s, 10, &pr);
            print_profile_row(s, &pr);
            pm1_null12[kind][seed] = pr.pm1;
            sound = sound && pr.complete && pr.classified
                && pr.rows_checked == pr.rank;
            if (kind == 0) {
                pins = pins && pr.pm1 == PIN_H6_12[0][seed]
                    && pr.by_norm[3] == PIN_H6_12[1][seed]
                    && pr.odd_pm1 == PIN_H6_12[2][seed];
            } else {
                pins = pins && pr.odd_pm1 == 0
                    && (seed >= 3 || pr.pm1 == PIN_H5_12_PM1[seed]);
            }
        }
    }
    check("every profile complete and classified; basis rows re-checked "
        "as relations in quaternion arithmetic; LIN's profile = 2O's "
        "(the whole relation lattice, D126 Part E)", sound);
    check("H6 and H5 = D126 Part F's generator: seeds 0-7 (H6) and 0-2 "
        "(H5) match D126's oracle pins (+-1 totals, norm 3, odd +-1); "
        "every H5 set: no odd-norm relation (the odd-height lemma)", pins);
    sprintf(msg, "G (D126 Part D's generator and seeds): no relation of "
        "norm <= 8, as D126 found (%ld)", g_total);
    check(msg, g_total == 0);
}

/* ================================================================
 * Part C: passing subsets per N
 *
 * A JOB is one (point set, table): N = 3-7, every subset of the set
 * (exhaustive at every N, so no row mixes scales), XOR / AND / MAJ.
 * The counts are numbers of PASSING SUBSETS per N, not D94's
 * "capacity" (the largest N with a pass). The parent runs each job in
 * a child process (this binary, "--opus J"), at most D127_OPERARII (8)
 * at once, and reads the child's counts back from one "RESULT" line.
 * Printing reads only the result arrays, in a fixed order, so the
 * output does not depend on which child finishes first. D127_SERIAL=1
 * runs every job in this process.
 * ================================================================ */

#define N_LOW 3
#define N_HIGH 7
#define N_ROWS (N_HIGH - N_LOW + 1)
#define MAX_SETS 120
#define TABLES 3
#define MAX_JOBS (MAX_SETS * TABLES)
#define FIELDS 7

static const char *TABLE_NAME[TABLES] = { "own", "T13", "R13" };

/* D127_CELER=1 (plants): N = 3-4 only; children inherit it */
static int n_high = N_HIGH;

/* the registry: every judged set, in named groups (fixed order) */
enum {
    GR_2O, GR_2I_D94, GR_2I_B, GR_2T,
    GR_2O_BFS, GR_2I_D94_BFS, GR_2I_B_BFS, GR_2T_BFS,
    GR_2I_R, GR_2O_SGN, GR_2I_D94_SGN,
    GR_H6, GR_H5, GR_G, GR_AX, GR_AN, GR_LIN,
    GR_H6_12, GR_H5_12,
    GROUPS
};

static const char *GROUP_LABEL[GROUPS] = {
    "2O", "2I-D94", "2I-B", "2T",
    "2O bfs", "2I-D94 bfs", "2I-B bfs", "2T bfs",
    "2I-R", "2O sgn", "2I-D94 sgn",
    "H6", "H5", "G", "AX", "AN", "LIN",
    "H6", "H5"
};

static const PointSet *registry[MAX_SETS];
static int             n_sets = 0;
static int             group_first[GROUPS];
static int             group_count[GROUPS];

static void
add_group (
    int             group,
    const PointSet *sets,
    int             count)
{
    int k;

    group_first[group] = n_sets;
    group_count[group] = count;
    for (k = 0; k < count; k++) {
        if (n_sets >= MAX_SETS) {
            fatal("registry full");
        }
        registry[n_sets++] = &sets[k];
    }
}

static void
register_sets (void)
{
    int kind;

    n_sets = 0;
    add_group(GR_2O, &canon_2o, 1);
    add_group(GR_2I_D94, &canon_2i_d94, 1);
    add_group(GR_2I_B, &canon_2i_b, 1);
    add_group(GR_2T, &canon_2t, 1);
    add_group(GR_2O_BFS, &set_2o, 1);
    add_group(GR_2I_D94_BFS, &set_2i_d94, 1);
    add_group(GR_2I_B_BFS, &set_2i_b, 1);
    add_group(GR_2T_BFS, &set_2t, 1);
    add_group(GR_2I_R, rand_2i, RANDOM_2I);
    add_group(GR_2O_SGN, sign_2o, SIGN_DRAWS);
    add_group(GR_2I_D94_SGN, sign_2i, SIGN_DRAWS);
    for (kind = 0; kind < 6; kind++) {
        add_group(GR_H6 + kind, nulls24[kind], seeds_of(kind));
    }
    add_group(GR_H6_12, nulls12[0], SEEDS);
    add_group(GR_H5_12, nulls12[1], SEEDS);
}

static void
registry_check (void)
{
    char msg[256];

    sprintf(msg, "registry: %d sets; H6 and H5 %d seeds each, 2I-R %d, sign "
        "draws %d + %d, G/AX/AN/LIN %d each", n_sets, group_count[GR_H6],
        group_count[GR_2I_R], group_count[GR_2O_SGN],
        group_count[GR_2I_D94_SGN], group_count[GR_G]);
    check(msg, group_count[GR_H6] == 20 && group_count[GR_H5] == 20
        && group_count[GR_2I_R] == 20 && group_count[GR_2O_SGN] == 5
        && group_count[GR_2I_D94_SGN] == 5 && group_count[GR_G] == 5
        && group_count[GR_LIN] == 5 && n_sets == 108);
}

static Row results[MAX_SETS][TABLES][N_ROWS][3];

static int
n_jobs (void)
{
    return n_sets * TABLES;
}

static void
job_execute (
    int job)
{
    int              set = job / TABLES;
    int              table = job % TABLES;
    const PointSet  *s = registry[set];
    Catalogus       *cat = as_catalog(s);
    Activatio       *act = NULL;
    const Quaternio *axes = NULL;
    int              n;
    int              f;

    if (cat == NULL) {
        fatal("job: catalog refused");
    }
    if (table > 0) {
        axes = table_axes(table == 1 ? &t13 : &r13, s->ring);
    }
    if (!activatio_creare(cat, axes, axes == NULL ? 0 : TABLE_SIZE,
            CELLAE_CELLA_D94, 0, pool, &act)) {
        fatal("job: activation refused");
    }
    for (n = N_LOW; n <= n_high; n++) {
        for (f = 0; f < 3; f++) {
            run_rows(act, s->n, n, f, &results[set][table][n - N_LOW][f]);
        }
    }
}

static void
job_print (
    int job)
{
    int set = job / TABLES;
    int table = job % TABLES;
    int n;
    int f;

    printf("RESULT %d", job);
    for (n = 0; n < N_ROWS; n++) {
        for (f = 0; f < 3; f++) {
            const Row *r = &results[set][table][n][f];

            printf(" %ld %ld %ld %ld %ld %ld %ld", r->sets, r->rule,
                r->robust, r->possible, r->tied, r->clash, r->undecided);
        }
    }
    printf("\n");
}

/* the RESULT line of a child's output into the result arrays */
static int
job_parse (
    int         job,
    const char *text)
{
    const char *line = strstr(text, "RESULT ");
    int         set = job / TABLES;
    int         table = job % TABLES;
    int         got;
    int         read;
    int         n;
    int         f;

    if (line == NULL || sscanf(line, "RESULT %d%n", &got, &read) != 1
        || got != job) {
        return 0;
    }
    line += read;
    for (n = 0; n < N_ROWS; n++) {
        for (f = 0; f < 3; f++) {
            Row *r = &results[set][table][n][f];

            if (sscanf(line, " %ld %ld %ld %ld %ld %ld %ld%n", &r->sets,
                    &r->rule, &r->robust, &r->possible, &r->tied,
                    &r->clash, &r->undecided, &read) != FIELDS) {
                return 0;
            }
            line += read;
        }
    }
    return 1;
}

/* measured: every N = 24 job ~55 s (the filter decides almost every
 * cell, whatever the height); N = 12 jobs are tiny */
static long
job_cost (
    int job)
{
    return registry[job / TABLES]->n == 12 ? 1 : 10;
}

static void
jobs_run_parallel (
    const char *argv0,
    int         workers)
{
    static Processus *slot_p[64];
    static Piscina   *slot_pool[64];
    static int        slot_job[64];
    static char       arg_n[MAX_JOBS][16];
    static int        order[MAX_JOBS];
    int               jobs = n_jobs();
    int               next = 0;
    int               running = 0;
    int               i;
    int               k;

    if (workers > 64) {
        workers = 64;
    }
    for (i = 0; i < jobs; i++) {
        order[i] = i;
    }
    for (i = 1; i < jobs; i++) {        /* stable: cost descending */
        int x = order[i];

        for (k = i; k > 0 && job_cost(order[k - 1]) < job_cost(x); k--) {
            order[k] = order[k - 1];
        }
        order[k] = x;
    }
    for (i = 0; i < workers; i++) {
        slot_p[i] = NULL;
    }
    while (next < jobs || running > 0) {
        int reaped = 0;

        for (i = 0; i < workers; i++) {
            if (slot_p[i] == NULL && next < jobs) {
                const char *args[4];
                int         job = order[next++];

                sprintf(arg_n[job], "%d", job);
                args[0] = argv0;
                args[1] = "--opus";
                args[2] = arg_n[job];
                args[3] = NULL;
                slot_pool[i] = piscina_generare_dynamicum("d127-opus",
                    1 << 16);
                if (slot_pool[i] == NULL) {
                    fatal("piscina (opus)");
                }
                slot_p[i] = processus_incipere((const char *const *)args,
                    0, slot_pool[i]);
                if (slot_p[i] == NULL) {
                    fatal("processus_incipere");
                }
                slot_job[i] = job;
                running++;
            }
        }
        for (i = 0; i < workers; i++) {
            ProcessusResultus res;
            char             *text;

            if (slot_p[i] == NULL
                || processus_pulsare(slot_p[i]) != PROCESSUS_PARATUS) {
                continue;
            }
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
            if (text == NULL) {
                fatal("malloc");
            }
            memcpy(text, res.effusio.datum, (size_t)res.effusio.mensura);
            text[res.effusio.mensura] = '\0';
            if (!job_parse(slot_job[i], text)) {
                printf("  FATAL: job %d: unreadable result\n",
                    slot_job[i]);
                exit(1);
            }
            free(text);
            piscina_destruere(slot_pool[i]);
            slot_p[i] = NULL;
            running--;
            reaped++;
        }
        if (reaped == 0) {
            fenestra_dormire(20000);
        }
    }
}

static long
binomial (
    int n,
    int k)
{
    long r = 1;
    int  i;

    for (i = 1; i <= k; i++) {
        r = r * (n - k + i) / i;
    }
    return r;
}

static long
field_of (
    const Row *r,
    int        mode)
{
    return mode == 0 ? r->rule : mode == 1 ? r->robust : mode == 2
        ? r->possible : mode == 3 ? r->tied : r->clash;
}

static const char *MODE_NAME[5] = { "rule", "robust", "possible", "tied",
    "concursus" };

/* one line: a group's count (one set) or min-max over its sets */
static void
print_line (
    const char *label,
    int         group,
    int         table,
    int         f,
    int         mode)
{
    int n;

    printf("      %-11s", label);
    for (n = 0; n < N_ROWS; n++) {
        long lo = -1;
        long hi = -1;
        int  i;
        char cell[48];

        for (i = 0; i < group_count[group]; i++) {
            long v = field_of(&results[group_first[group] + i][table][n][f],
                mode);

            if (lo < 0 || v < lo) {
                lo = v;
            }
            if (v > hi) {
                hi = v;
            }
        }
        if (lo == hi) {
            sprintf(cell, "%ld", lo);
        } else {
            sprintf(cell, "%ld-%ld", lo, hi);
        }
        printf(" %15s", cell);
    }
    if (group_count[group] > 1) {
        printf("   (%d sets)", group_count[group]);
    }
    printf("\n");
}

static void
print_head (
    int points)
{
    int n;

    printf("      %-11s", "N");
    for (n = N_LOW; n <= N_HIGH; n++) {
        char head[32];

        sprintf(head, "%d (%ld)", n, binomial(points, n));
        printf(" %15s", head);
    }
    printf("\n");
}

static void
print_block (
    int table,
    int f,
    int mode)
{
    static const int ROWS[] = { GR_2O, GR_2I_D94, GR_2I_B, GR_2I_R, GR_H6,
        GR_H5, GR_G, GR_AX, GR_AN, GR_LIN };
    int r;

    printf("    %s, %s, %s (N = 24 sets; passing subsets of C(24, N)):\n",
        TABLE_NAME[table], FUNCTION_NAME[f], MODE_NAME[mode]);
    print_head(24);
    for (r = 0; r < (int)(sizeof(ROWS) / sizeof(ROWS[0])); r++) {
        print_line(GROUP_LABEL[ROWS[r]], ROWS[r], table, f, mode);
    }
}

/* D119_ZETA / D119_AUREA rows N = 5, 6 (lentus), as in Part A */
static const long D119_ZETA_56[6][6] = {
    { 42504, 17201, 16479, 15497, 24714, 30822 },
    { 42504, 37835, 37309, 36707, 39072, 30822 },
    { 42504, 18368, 17282, 16514, 25625, 30822 },
    { 134596, 12983, 12268, 10070, 22979, 107035 },
    { 134596, 111290, 110495, 107663, 116323, 107035 },
    { 134596, 10031, 9635, 7911, 18245, 107035 } };
static const long D119_AUREA_56[6][6] = {
    { 42504, 20805, 20453, 19693, 27983, 33885 },
    { 42504, 38688, 38702, 38044, 39557, 33885 },
    { 42504, 24333, 24022, 23499, 31738, 33885 },
    { 134596, 21679, 20936, 17034, 28850, 117888 },
    { 134596, 112556, 112469, 109058, 116439, 117888 },
    { 134596, 21237, 20998, 17547, 28484, 117888 } };

/* 2O (canonical) under R13: the review's independent oracle (orac.py,
 * recensio D127, 2026-10-14), N = 3-7 x XOR / AND / MAJ: sets, rule,
 * robust, possible, tied, concursus */
static const long ORACLE_2O_R13[15][6] = {
    { 2024, 1472, 1472, 1954, 965, 32 },
    { 2024, 1803, 1803, 1964, 965, 0 },
    { 2024, 1466, 1466, 1962, 965, 32 },
    { 10626, 8172, 8168, 8812, 4556, 672 },
    { 10626, 9603, 9595, 10040, 4556, 0 },
    { 10626, 7030, 7010, 8601, 4556, 696 },
    { 42504, 16713, 16693, 23206, 17969, 6672 },
    { 42504, 37695, 37574, 38737, 17969, 0 },
    { 42504, 18020, 18004, 24708, 17969, 7104 },
    { 134596, 10074, 10053, 16691, 58203, 40696 },
    { 134596, 112189, 111512, 114377, 58203, 0 },
    { 134596, 8224, 8051, 13343, 58203, 44110 },
    { 346104, 265, 265, 940, 155535, 168864 },
    { 346104, 252922, 250888, 257802, 155535, 0 },
    { 346104, 3435, 3434, 7704, 155535, 184518 } };

static int
row_is (
    const Row  *r,
    const long *pin)
{
    return r->sets == pin[0] && r->rule == pin[1] && r->robust == pin[2]
        && r->possible == pin[3] && r->tied == pin[4];
}

/* is 'set' one whose points are all canonical (first part positive)? */
static int
canonical_group (
    int set)
{
    int g;

    for (g = 0; g < GROUPS; g++) {
        if (set >= group_first[g] && set < group_first[g] + group_count[g]) {
            return g != GR_2O_BFS && g != GR_2I_D94_BFS && g != GR_2I_B_BFS
                && g != GR_2T_BFS && g != GR_2O_SGN && g != GR_2I_D94_SGN;
        }
    }
    return 0;
}

static int
same_xor (
    int a,
    int b)
{
    int table;
    int n;

    for (table = 0; table < TABLES; table++) {
        for (n = 0; n <= n_high - N_LOW; n++) {
            if (memcmp(&results[a][table][n][0], &results[b][table][n][0],
                    sizeof(Row)) != 0) {
                return 0;
            }
        }
    }
    return 1;
}

static void
part_c (
    const char *argv0)
{
    int  set;
    int  table;
    int  n;
    int  f;
    int  g;
    int  i;
    int  counts = 1;
    int  undecided = 0;
    int  xor_signs = 1;
    int  lin_clash = 1;
    long lin_other_equal = 0;
    long lin_other_rows = 0;
    int  free_clash = 1;
    int  h5_xor = 1;
    int  and_clash = 1;
    int  pinned = 1;
    int  oracle = 1;
    char msg[256];
    s64  t0 = fenestra_tempus_ms();

    printf("\n=== Part C: passing subsets per N (D94 phase cell, signed "
        "encoding, k in {6, 12, 24}; N = 3-7 exhaustive) ===\n");
    printf("  %d sets x %d tables = %d jobs\n", n_sets, TABLES, n_jobs());
    if (n_high < N_HIGH) {
        printf("  (quick mode: N = 3-%d)\n", n_high);
    }
    if (getenv("D127_SERIAL") != NULL) {
        for (i = 0; i < n_jobs(); i++) {
            job_execute(i);
        }
    } else {
        const char *w = getenv("D127_OPERARII");

        jobs_run_parallel(argv0, w != NULL && atoi(w) > 0 ? atoi(w) : 8);
    }
    if (getenv("D127_TEMPORA") != NULL) {
        fprintf(stderr, "  [%d jobs: %.1f s wall]\n", n_jobs(),
            (double)(fenestra_tempus_ms() - t0) / 1000.0);
    }

    /* --- checks --- */
    for (set = 0; set < n_sets; set++) {
        int points = registry[set]->n;

        for (table = 0; table < TABLES; table++) {
            for (n = 0; n <= n_high - N_LOW; n++) {
                for (f = 0; f < 3; f++) {
                    const Row *r = &results[set][table][n][f];

                    counts = counts && r->sets == binomial(points,
                        n + N_LOW) && r->robust <= r->rule
                        && r->rule <= r->possible;
                    undecided += r->undecided > 0;
                    /* canonical points lie in one lexicographic half-
                     * space: no nonempty sum of them vanishes, so the
                     * all-ones mask (AND = 1) shares its sum with none */
                    if (f == 1 && canonical_group(set)) {
                        and_clash = and_clash && r->clash == 0;
                    }
                }
            }
        }
    }
    sprintf(msg, "%d jobs: every row counts C(N_set, N) subsets, robust <= "
        "rule <= possible", n_jobs());
    check(msg, counts);
    sprintf(msg, "no undecided set (SAT budget never reached): %d rows "
        "with one", undecided);
    check(msg, undecided == 0);
    /* the workers' rows against D119 (BFS signs): 2O own and T13, 2I-D94
     * own, N = 3-4, and N = 5-6 when run */
    for (n = 0; n <= n_high - N_LOW && n < 4; n++) {
        for (f = 0; f < 3; f++) {
            const long *z = n < 2 ? D119_ZETA[n * 3 + f]
                : D119_ZETA_56[(n - 2) * 3 + f];
            const long *a = n < 2 ? D119_AUREA[n * 3 + f]
                : D119_AUREA_56[(n - 2) * 3 + f];
            long        zp[5];
            long        ap[5];

            for (i = 0; i < 5; i++) {
                zp[i] = z[i == 0 ? 0 : i + 1];
                ap[i] = a[i == 0 ? 0 : i + 1];
            }
            pinned = pinned
                && row_is(&results[group_first[GR_2O_BFS]][0][n][f], zp)
                && row_is(&results[group_first[GR_2O_BFS]][1][n][f], zp)
                && row_is(&results[group_first[GR_2I_D94_BFS]][0][n][f],
                    ap);
        }
    }
    check("the workers' rows reproduce D119 (2O own and T13, 2I-D94 "
        "own; BFS signs; N = 3-6): jobs, sets and tables line up",
        pinned);
    for (n = 0; n <= n_high - N_LOW; n++) {
        for (f = 0; f < 3; f++) {
            const long *pin = ORACLE_2O_R13[n * 3 + f];
            const Row  *r = &results[group_first[GR_2O]][2][n][f];

            oracle = oracle && row_is(r, pin) && r->clash == pin[5];
        }
    }
    check("2O under R13 = the review's independent oracle in every row "
        "run (sets, rule, robust, possible, tied, concursus): R13 is "
        "anchored", oracle);
    /* a weight's sign flips every XOR label: XOR cannot change */
    for (g = 0; g < 4; g++) {
        xor_signs = xor_signs && same_xor(group_first[GR_2O + g],
            group_first[GR_2O_BFS + g]);
    }
    for (i = 0; i < SIGN_DRAWS; i++) {
        xor_signs = xor_signs && same_xor(group_first[GR_2O],
            group_first[GR_2O_SGN] + i) && same_xor(group_first[GR_2I_D94],
            group_first[GR_2I_D94_SGN] + i);
    }
    check("XOR identical under every sign choice: D119's BFS signs and 5 "
        "random sign draws each of 2O and 2I-D94, every table and N (a "
        "weight's sign flips every XOR label)", xor_signs);
    for (i = 0; i < group_count[GR_LIN]; i++) {
        int lin = group_first[GR_LIN] + i;

        for (table = 0; table < TABLES; table++) {
            for (n = 0; n <= n_high - N_LOW; n++) {
                lin_clash = lin_clash && results[lin][table][n][0].clash
                    == results[group_first[GR_2O]][table][n][0].clash;
                for (f = 1; f < 3; f++) {
                    lin_other_rows++;
                    lin_other_equal += results[lin][table][n][f].clash
                        == results[group_first[GR_2O]][table][n][f].clash;
                }
            }
        }
    }
    check("LIN's XOR concursus = 2O's in every row (the same relation "
        "lattice; a coincidence's XOR clash depends only on the "
        "relation's parity)", lin_clash);
    printf("    observed, not a theorem: LIN's AND/MAJ concursus = 2O's in "
        "%ld of %ld rows (the sign of each point enters; both sets are "
        "canonical)\n", lin_other_equal, lin_other_rows);
    for (g = GR_H6; g <= GR_LIN; g++) {
        int kind = g - GR_H6;

        for (i = 0; i < group_count[g]; i++) {
            int set = group_first[g] + i;

            for (table = 0; table < TABLES; table++) {
                for (n = 0; n <= n_high - N_LOW; n++) {
                    for (f = 0; f < 3; f++) {
                        long c = results[set][table][n][f].clash;

                        /* a coincidence among <= 7 weights is a +-1
                         * relation of norm <= 7 <= R = 8 */
                        if (pm1_null24[kind][i] == 0) {
                            free_clash = free_clash && c == 0;
                        }
                        if (g == GR_H5 && f == 0) {
                            h5_xor = h5_xor && c == 0;
                        }
                    }
                }
            }
        }
    }
    check("every null with no +-1 relation within norm 8 has concursus 0 "
        "in every row (Part B and Part C agree)", free_clash);
    check("H5: XOR concursus 0 in every row, 20 seeds (the odd-height "
        "lemma)", h5_xor);
    check("AND concursus 0 for every canonical set (theorem: canonical "
        "points lie in one lexicographic half-space) - the AND concursus "
        "rows carry no information and are not printed", and_clash);

    /* --- own-table sizes --- */
    printf("\n  Own-table sizes (axes), per group:");
    for (g = 0; g < GROUPS; g++) {
        int lo = 1000;
        int hi = 0;

        for (i = 0; i < group_count[g]; i++) {
            Activatio *act = NULL;
            int        set = group_first[g] + i;
            int        a;

            if (!activatio_creare(as_catalog(registry[set]), NULL, 0,
                    CELLAE_CELLA_D94, 0, pool, &act)) {
                fatal("own table");
            }
            a = (int)activatio_axes(act);
            lo = a < lo ? a : lo;
            hi = a > hi ? a : hi;
        }
        if (lo == hi) {
            printf(" %s %d;", GROUP_LABEL[g], lo);
        } else {
            printf(" %s %d-%d;", GROUP_LABEL[g], lo, hi);
        }
    }
    printf("\n");

    /* --- tables --- */
    for (table = 1; table < TABLES; table++) {
        printf("\n  Table %s (common to every set):\n", TABLE_NAME[table]);
        print_block(table, 0, 1);
        print_block(table, 0, 0);
        print_block(table, 0, 2);
        print_block(table, 0, 3);
        print_block(table, 0, 4);
        print_block(table, 2, 4);
        printf("    (AND and MAJ below depend on the sign chosen for each "
            "weight: not properties of the point sets)\n");
        print_block(table, 1, 1);
        print_block(table, 2, 1);
    }
    printf("\n  Own tables (sizes above differ: no comparison across "
        "sets):\n");
    print_block(0, 0, 1);
    printf("\n  AND and MAJ robust under sign choices (canonical, D119's "
        "BFS signs, 5 random draws):\n");
    for (table = 0; table < TABLES; table++) {
        for (f = 1; f < 3; f++) {
            printf("    %s, %s:\n", TABLE_NAME[table], FUNCTION_NAME[f]);
            print_head(24);
            print_line("2O", GR_2O, table, f, 1);
            print_line("2O bfs", GR_2O_BFS, table, f, 1);
            print_line("2O sgn", GR_2O_SGN, table, f, 1);
            print_line("2I-D94", GR_2I_D94, table, f, 1);
            print_line("2I-D94 bfs", GR_2I_D94_BFS, table, f, 1);
            print_line("2I-D94 sgn", GR_2I_D94_SGN, table, f, 1);
        }
    }
    printf("\n  2T (N = 12) against H6 and H5 at N = 12, robust:\n");
    for (table = 0; table < TABLES; table++) {
        for (f = 0; f < 3; f++) {
            printf("    %s, %s (passing subsets of C(12, N)):\n",
                TABLE_NAME[table], FUNCTION_NAME[f]);
            print_head(12);
            print_line("2T", GR_2T, table, f, 1);
            print_line("2T bfs", GR_2T_BFS, table, f, 1);
            print_line("H6", GR_H6_12, table, f, 1);
            print_line("H5", GR_H5_12, table, f, 1);
        }
    }
}

/* ================================================================
 * Main
 * ================================================================ */

int
main (
    int    argc,
    char **argv)
{
    pool = piscina_generare_dynamicum("demo_127", 1 << 22);
    if (getenv("D127_CELER") != NULL) {
        n_high = 4;
    }
    if (argc == 3 && strcmp(argv[1], "--opus") == 0) {
        int job = atoi(argv[2]);

        in_worker = 1;
        part_a();
        if (n_fail > 0) {
            fatal("--opus: Part A failed");
        }
        if (job < 0 || job >= n_jobs()) {
            fatal("--opus: no such job");
        }
        job_execute(job);
        job_print(job);
        return 0;
    }
    printf("KNOTAPEL DEMO 127: Group Catalogs against Matched Nulls (P1)"
        "\n");
    printf("==========================================================="
        "\n");
    part_a();
    if (n_fail > 0) {
        /* wrong sets: every later part would measure something else */
        printf("\nPart A failed: stopping.\n%d passed, %d failed\n",
            n_pass, n_fail);
        return 1;
    }
    part_b();
    part_c(argv[0]);
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pool);
    return n_fail == 0 ? 0 : 1;
}
