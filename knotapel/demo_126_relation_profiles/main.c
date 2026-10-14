/*
 * KNOTAPEL DEMO 126: Relation Profiles of 2T, 2O, 2I
 * ================================================================
 *
 * A neuron sees only sums of its weight points (claims.md KC 47.1.2).
 * Two input patterns give the same sum exactly when their difference is
 * an integer relation among the points with coefficients in {-1, 0, 1}:
 * the two patterns are then indistinguishable under ANY activation
 * (boundary ties, by contrast, depend on it). All integer relations of
 * a point set S = (p_1 .. p_N) form the lattice
 *
 *     L(S) = { x in Z^N : sum x_i p_i = 0 },
 *
 * and its rank, shells by norm and shortest vectors are the set's
 * RELATION PROFILE. This demo measures it exactly (house library
 * reticulum: integral LLL + integer Fincke-Pohst) for the group catalogs
 * and for knot-free point sets. It measures no capacity and no
 * activation. Plan: plan.md (approved 2026-10-13).
 *
 *   Part A  catalogs: 2O from D66's generators over Q(sqrt 2) and 2I
 *           from D94's over Q(sqrt 5), both by exact closure as in D118;
 *           2T = the rational elements of 2O (a subgroup of order 24).
 *           Points modulo sign: one representative per {g, -g},
 *           canonical (first nonzero part positive): 12, 24, 60.
 *   Part B  profiles: rank of L(S); LLL-reduced basis, every row
 *           re-checked as a relation in quaternion arithmetic; shells by
 *           norm up to R (counted modulo sign, completeness asserted);
 *           the signed coincidences (entries in {-1, 0, 1}) among them;
 *           the shortest norm.
 *   Part C  invariance: r S r^-1, r S and a re-signed S give identical
 *           profiles (relations are invariant under any linear map
 *           applied to all points; a sign flip is an isometry of Z^N).
 *   Part D  baseline nulls (knot-free, exact, fixed seeds): (a) generic
 *           unit quaternions over the catalog's own field, r g with r a
 *           random rational unit quaternion and g a random group
 *           element; (b) generic rational unit quaternions.
 *
 * Embedding: a coordinate a + b sqrt d becomes (a, b), a point a vector
 * in Q^8; 1 and sqrt d are independent over Q, so the relation lattice
 * of the coordinates IS that of the points.
 *
 * Independent oracle: oracle.py (Python Fractions, its own closure and
 * field arithmetic; signed coincidences by direct search, no lattice).
 *
 * House libraries: includes quaternio.h, extensio.h and reticulum.h,
 * hence latina.h (Roman numerals and Latin keywords are macros here).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_126_relation_profiles/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 */

#include "quaternio.h"
#include "extensio.h"
#include "reticulum.h"
#include "matrix.h"
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

/* ================================================================
 * Finite groups of quaternions, exactly (as in D118)
 * ================================================================ */

#define MAX_GROUP 120

typedef struct {
    const Anulus *ring;
    Quaternio     el[MAX_GROUP];
    int           n;
    int           mul[MAX_GROUP][MAX_GROUP];
    int           neg[MAX_GROUP];
} Group;

static int
group_find (
    const Group *g,
    Quaternio    q)
{
    int k;

    for (k = 0; k < g->n; k++) {
        if (quaternio_aequalis(g->el[k], q)) {
            return k;
        }
    }
    return -1;
}

/* closure of {1} under right multiplication by the generators, then the
 * full table and negation; 0 if more than MAX_GROUP elements */
static int
group_close (
    Group           *g,
    const Anulus    *ring,
    const Quaternio *gens,
    int              n_gens)
{
    int       cursor = 0;
    int       a;
    int       b;
    Quaternio unit;

    g->ring = ring;
    g->n = 0;
    (void)quaternio_unum(ring, pool, &unit);
    g->el[g->n++] = unit;
    while (cursor < g->n) {
        for (a = 0; a < n_gens; a++) {
            PiscinaNotatio mark = piscina_notare(pool);
            Quaternio      x;

            (void)quaternio_multiplica(g->el[cursor], gens[a], pool, &x);
            if (group_find(g, x) >= 0) {
                piscina_reficere(pool, mark);
                continue;
            }
            if (g->n >= MAX_GROUP) {
                return 0;
            }
            g->el[g->n++] = x;
        }
        cursor++;
    }
    for (a = 0; a < g->n; a++) {
        for (b = 0; b < g->n; b++) {
            PiscinaNotatio mark = piscina_notare(pool);
            Quaternio      x;

            (void)quaternio_multiplica(g->el[a], g->el[b], pool, &x);
            g->mul[a][b] = group_find(g, x);
            piscina_reficere(pool, mark);
            if (g->mul[a][b] < 0) {
                return 0;
            }
        }
        {
            PiscinaNotatio mark = piscina_notare(pool);

            g->neg[a] = group_find(g, negate(g->el[a]));
            piscina_reficere(pool, mark);
            if (g->neg[a] < 0) {
                return 0;
            }
        }
    }
    return 1;
}

static int
largest_order (
    const Group *g)
{
    int a;
    int largest = 0;

    for (a = 0; a < g->n; a++) {
        int x = a;
        int o = 1;

        while (!quaternio_aequalis(g->el[x], g->el[0])) {
            x = g->mul[x][a];
            o++;
        }
        if (o > largest) {
            largest = o;
        }
    }
    return largest;
}

/* ================================================================
 * Point sets and their coordinates over Q
 * ================================================================ */

#define MAX_POINTS 120

typedef struct {
    char          name[64];
    const Anulus *ring;
    int           n;
    Quaternio     p[MAX_POINTS];
} PointSet;

/* one canonical representative per {g, -g} */
static void
mod_sign (
    const Group *g,
    const int   *members,
    int          n_members,
    const char  *name,
    PointSet    *s)
{
    int k;

    strcpy(s->name, name);
    s->ring = g->ring;
    s->n = 0;
    for (k = 0; k < n_members; k++) {
        int a = members[k];

        if (a < g->neg[a]) {
            s->p[s->n++] = canonical(g->el[a]);
        }
    }
}

/* 8 x N over Q: column j = point j; row 2k + e = coefficient of
 * sqrt(d)^e in part k */
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

/* sum x_i p_i == 0 in quaternion arithmetic (a second route) */
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

/* ================================================================
 * Relation profiles
 * ================================================================ */

#define MAX_R 16

/* shell radii (|x|^2) and storage: chosen by measurement, see
 * findings.md */
#define RADIUS_2T 10
#define RADIUS_2O 8
#define STORE_LIMIT 200000L
/* node budget for every enumeration: ~20x what 2I to norm 5 needs. A
 * wrong configuration then shows as INCOMPLETE (a failed check), not as
 * a run that never ends. */
#define NODE_BUDGET 40000000L

/* 2I to norm 5: ~1.9M nodes, ~30 MB; norm 6 (one-off, 15.9M nodes,
 * 289 MB, 169,100 relations - oracle.py agrees) is left out of the run.
 * D126_CELER=1 (plants): norm 4. */
static int radius_2i = 5;
#define RADIUS_2I radius_2i

typedef struct {
    int    ambient_rank;    /* rank of the 8 x N coordinate matrix */
    int    rank;            /* rank of L(S) = N - ambient_rank */
    int    rows_checked;    /* basis rows re-checked as relations */
    int    radius;
    int    complete;
    long   count;           /* relations with 0 < |x|^2 <= R, mod sign */
    long   by_norm[MAX_R + 1];
    long   pm1_by_norm[MAX_R + 1];  /* entries in {-1, 0, 1} */
    int    classified;      /* all vectors stored, so pm1 is exact */
    int    shortest;        /* first nonempty shell, -1 if none <= R */
    long   nodes;
    unsigned long apex;
    Magnus first_norm;      /* |b_1|^2 of the reduced basis: lambda_1 <= */
} Profile;

static void
profile (
    const PointSet *s,
    int             radius,
    long            store_limit,
    long            node_budget,
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
    pr->first_norm = magnus_ex_s64(0);
    for (j = 0; pr->rank > 0 && j < s->n; j++) {
        const Magnus *x = (const Magnus *)matrix_elementum(basis, 0,
            (i32)j);

        pr->first_norm = magnus_adde(pr->first_norm, magnus_multiplica(*x,
            *x, pool), pool);
    }
    q.radius_quadratus = magnus_ex_s64(radius);
    q.limes_servandi = (i32)store_limit;
    q.limes_nodorum = (i64)node_budget;
    if (!reticulum_enumera(basis, &q, pool, &e)) {
        fatal("reticulum_enumera refused");
    }
    pr->complete = (int)e.completa;
    pr->count = (long)e.numerus;
    pr->nodes = (long)e.nodi;
    pr->apex = (unsigned long)reticulum_apex_officinarum();
    pr->classified = e.servati == (i32)e.numerus;
    pr->shortest = -1;
    for (i = 0; i < (int)e.servati; i++) {
        s64 n = 0;
        int pm1 = 1;

        (void)magnus_ad_s64(e.normae[i], &n);
        pr->by_norm[n]++;
        for (j = 0; j < s->n && pm1; j++) {
            s64 v = 0;

            (void)magnus_ad_s64(*(const Magnus *)matrix_elementum(
                e.vectores, (i32)i, (i32)j), &v);
            pm1 = v >= -1 && v <= 1;
        }
        pr->pm1_by_norm[n] += pm1;
        if (pr->shortest < 0) {
            pr->shortest = (int)n;
        }
    }
}

static void
print_profile (
    const PointSet *s,
    const Profile  *pr)
{
    int  n;
    char b1[128];

    snprintf(b1, sizeof(b1), "%s", (const char *)magnus_ad_chordam(
        pr->first_norm, pool).datum);
    printf("  %-24s N %3d  ambient rank %d  relation rank %2d"
        "  |b_1|^2 %s\n", s->name, s->n, pr->ambient_rank, pr->rank,
        b1);
    printf("    shells to R = %d (%s, %ld nodes, apex %lu): %ld relations"
        " mod sign\n", pr->radius, pr->complete ? "complete"
        : "INCOMPLETE - lower bounds", pr->nodes, pr->apex, pr->count);
    for (n = 1; n <= pr->radius; n++) {
        if (pr->by_norm[n] > 0) {
            printf("      norm %2d: %7ld   of which entries in {-1,0,1}:"
                " %ld\n", n, pr->by_norm[n], pr->pm1_by_norm[n]);
        }
    }
    if (pr->complete) {
        if (pr->shortest > 0) {
            printf("    shortest relation: norm %d (%ld of them)\n",
                pr->shortest, pr->by_norm[pr->shortest]);
        } else {
            printf("    no relation of norm <= %d: lambda_1 > %d\n",
                pr->radius, pr->radius);
        }
    }
}

static int
same_profile (
    const Profile *a,
    const Profile *b)
{
    int n;

    if (a->rank != b->rank || a->complete != b->complete
        || a->count != b->count || !a->classified || !b->classified) {
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
 * Part A: catalogs
 * ================================================================ */

static Group    g48;
static Group    g120;
static PointSet cat_2t;
static PointSet cat_2o;
static PointSet cat_2i;
static int      members_2t[24];

static void
part_a (void)
{
    Extensio *q2 = extensio_quadratica(2, pool);
    Extensio *q5 = extensio_quadratica(5, pool);
    Quaternio gens[4];
    int       all[MAX_GROUP];
    int       n_rational = 0;
    int       closed = 1;
    int       units = 1;
    int       a;
    int       b;
    int       k;
    char      msg[256];

    printf("\n=== Part A: catalogs, exactly ===\n");
    check("Q(sqrt 2) and Q(sqrt 5) built, ordered", q2 != NULL
        && q5 != NULL && extensio_ordinata(q2) && extensio_ordinata(q5));
    /* D66: sigma_1 -> (1 + i)/sqrt 2, sigma_2 -> (1 - k)/sqrt 2 */
    gens[0] = qparse(extensio_anulus(q2), "[a/2, a/2, 0, 0]");
    gens[1] = qparse(extensio_anulus(q2), "[a/2, -a/2, 0, 0]");
    gens[2] = qparse(extensio_anulus(q2), "[a/2, 0, 0, -a/2]");
    gens[3] = qparse(extensio_anulus(q2), "[a/2, 0, 0, a/2]");
    if (!group_close(&g48, extensio_anulus(q2), gens, 4)) {
        fatal("closure of D66's generators");
    }
    sprintf(msg, "D66's generators close to 48 elements, largest order 8"
        " (2O) - got %d, %d", g48.n, largest_order(&g48));
    check(msg, g48.n == 48 && largest_order(&g48) == 8);
    /* D94: s = (1 + i + j + k)/2, t = (phi + phi^-1 i + j)/2 */
    gens[0] = qparse(extensio_anulus(q5), "[1/2, 1/2, 1/2, 1/2]");
    gens[1] = qparse(extensio_anulus(q5),
        "[(a + 1)/4, (a - 1)/4, 1/2, 0]");
    if (!group_close(&g120, extensio_anulus(q5), gens, 2)) {
        fatal("closure of D94's generators");
    }
    sprintf(msg, "D94's generators close to 120 elements, largest order"
        " 10 (2I) - got %d, %d", g120.n, largest_order(&g120));
    check(msg, g120.n == 120 && largest_order(&g120) == 10);
    for (a = 0; a < g48.n; a++) {
        units = units && is_unit(g48.el[a]);
    }
    for (a = 0; a < g120.n; a++) {
        units = units && is_unit(g120.el[a]);
    }
    check("every element has norm exactly 1", units);
    /* 2T: elements of 2O with rational coordinates */
    for (a = 0; a < g48.n; a++) {
        int rational = 1;

        for (k = 0; k < 4; k++) {
            rational = rational && algebraicus_est_rationalis(part_of(
                g48.el[a], k), pool, NULL);
        }
        if (rational) {
            if (n_rational < 24) {
                members_2t[n_rational] = a;
            }
            n_rational++;
        }
    }
    for (a = 0; a < n_rational && a < 24; a++) {
        for (b = 0; b < n_rational && b < 24; b++) {
            int product = g48.mul[members_2t[a]][members_2t[b]];
            int found = 0;

            for (k = 0; k < 24; k++) {
                found = found || members_2t[k] == product;
            }
            closed = closed && found;
        }
    }
    sprintf(msg, "2T = the rational elements of 2O: 24, closed under "
        "multiplication (got %d)", n_rational);
    check(msg, n_rational == 24 && closed);
    mod_sign(&g48, members_2t, 24, "2T (in 2O)", &cat_2t);
    for (a = 0; a < g48.n; a++) {
        all[a] = a;
    }
    mod_sign(&g48, all, g48.n, "2O (D66)", &cat_2o);
    for (a = 0; a < g120.n; a++) {
        all[a] = a;
    }
    mod_sign(&g120, all, g120.n, "2I (D94)", &cat_2i);
    sprintf(msg, "points mod sign: 2T %d, 2O %d, 2I %d (12, 24, 60)",
        cat_2t.n, cat_2o.n, cat_2i.n);
    check(msg, cat_2t.n == 12 && cat_2o.n == 24 && cat_2i.n == 60);
}

/* ================================================================
 * Part B: relation profiles of the catalogs
 * ================================================================ */

static Profile pr_2t;
static Profile pr_2o;
static Profile pr_2i;

/* pinned from oracle.py (independent: direct search of signed subset
 * sums; for 2T every relation by halves): index = norm */
static const long oracle_2t_all[11] = { 0, 0, 0, 16, 36, 48, 96, 144,
    252, 400, 288 };
static const long oracle_2t_pm1[11] = { 0, 0, 0, 16, 36, 48, 96, 144,
    156, 112, 0 };
static const long oracle_2o_pm1[9] = { 0, 0, 0, 32, 72, 96, 704, 2592,
    5976 };
static const long oracle_2i_pm1[7] = { 0, 0, 0, 200, 1800, 16392,
    169100 };

static int
agrees (
    const Profile *pr,
    const long    *expected,
    int            pm1)
{
    int n;

    for (n = 1; n <= pr->radius; n++) {
        if ((pm1 ? pr->pm1_by_norm[n] : pr->by_norm[n]) != expected[n]) {
            return 0;
        }
    }
    return 1;
}

static void
pins_against_oracle (void)
{
    check("2T: every shell to norm 10 = oracle.py (all relations, any "
        "entries)", agrees(&pr_2t, oracle_2t_all, 0));
    check("2T: signed coincidences by support 3..10 = oracle.py",
        agrees(&pr_2t, oracle_2t_pm1, 1));
    check("2O: signed coincidences by support 3..8 = oracle.py",
        agrees(&pr_2o, oracle_2o_pm1, 1));
    check("2I: signed coincidences by support 3..R = oracle.py",
        agrees(&pr_2i, oracle_2i_pm1, 1));
    /* not covered by the oracle (entries beyond {-1, 0, 1} in 2O):
     * pinned as a regression of the library only */
    check("2O: norm 8 has 6168 relations, 192 with an entry +-2 "
        "(library only, pinned)", pr_2o.by_norm[8] == 6168);
}

static void
part_b (void)
{
    char msg[256];

    printf("\n=== Part B: relation profiles of the catalogs ===\n");
    profile(&cat_2t, RADIUS_2T, STORE_LIMIT, NODE_BUDGET, &pr_2t);
    print_profile(&cat_2t, &pr_2t);
    profile(&cat_2o, RADIUS_2O, STORE_LIMIT, NODE_BUDGET, &pr_2o);
    print_profile(&cat_2o, &pr_2o);
    profile(&cat_2i, RADIUS_2I, STORE_LIMIT, NODE_BUDGET, &pr_2i);
    print_profile(&cat_2i, &pr_2i);
    pins_against_oracle();
    sprintf(msg, "relation ranks N - ambient rank: 2T %d, 2O %d, 2I %d "
        "(8, 16, 52)", pr_2t.rank, pr_2o.rank, pr_2i.rank);
    check(msg, pr_2t.rank == 8 && pr_2o.rank == 16 && pr_2i.rank == 52);
    check("every basis row is a relation in quaternion arithmetic",
        pr_2t.rows_checked == pr_2t.rank
        && pr_2o.rows_checked == pr_2o.rank
        && pr_2i.rows_checked == pr_2i.rank);
    check("every shell count complete, every vector stored",
        pr_2t.complete && pr_2o.complete && pr_2i.complete
        && pr_2t.classified && pr_2o.classified && pr_2i.classified);
}

/* ================================================================
 * Part C: invariance of the profile
 * ================================================================ */

/* r = (1 + 2i + 2j)/3, a rational unit quaternion */
static Quaternio
rotor (
    const Anulus *ring)
{
    return qparse(ring, "[1/3, 2/3, 2/3, 0]");
}

static void
transformed (
    const PointSet *s,
    int             mode,      /* 0: r p r^-1, 1: r p, 2: re-signed */
    PointSet       *out)
{
    Quaternio r = rotor(s->ring);
    Quaternio rc;
    int       j;

    (void)quaternio_conjugatum(r, pool, &rc);
    *out = *s;
    for (j = 0; j < s->n; j++) {
        Quaternio t;

        if (mode == 0) {
            (void)quaternio_multiplica(r, s->p[j], pool, &t);
            (void)quaternio_multiplica(t, rc, pool, &t);
            out->p[j] = canonical(t);
        } else if (mode == 1) {
            (void)quaternio_multiplica(r, s->p[j], pool, &t);
            out->p[j] = canonical(t);
        } else {
            out->p[j] = (j % 3 == 1) ? negate(s->p[j]) : s->p[j];
        }
    }
}

static void
part_c (void)
{
    const PointSet *sets[3];
    const Profile  *base[3];
    int             radii[3];
    int             i;
    int             mode;
    char            msg[256];

    printf("\n=== Part C: invariance of the profile ===\n");
    sets[0] = &cat_2t;
    sets[1] = &cat_2o;
    sets[2] = &cat_2i;
    base[0] = &pr_2t;
    base[1] = &pr_2o;
    base[2] = &pr_2i;
    radii[0] = RADIUS_2T;
    radii[1] = RADIUS_2O;
    radii[2] = RADIUS_2I;
    check("r = (1 + 2i + 2j)/3 is a unit", is_unit(rotor(cat_2o.ring)));
    for (i = 0; i < 3; i++) {
        for (mode = 0; mode < 3; mode++) {
            PointSet t;
            Profile  pr;
            static const char *what[3] = { "r S r^-1", "r S",
                "re-signed S" };

            transformed(sets[i], mode, &t);
            profile(&t, radii[i], STORE_LIMIT, NODE_BUDGET, &pr);
            sprintf(msg, "%s: profile of %s identical (rank %d, %ld "
                "relations to R = %d)", sets[i]->name, what[mode], pr.rank,
                pr.count, radii[i]);
            check(msg, same_profile(&pr, base[i]));
        }
    }
}

/* ================================================================
 * Part D: baseline nulls
 * ================================================================ */

static long rng_state;

static long
rng (
    long modulus)
{
    rng_state = (rng_state * 1103515245L + 12345L) & 0x7fffffffL;
    return rng_state % modulus;
}

/* rational unit quaternion by inverse stereographic projection of a
 * random rational point u = (n_1/m_1, n_2/m_2, n_3/m_3), |n| <= 9,
 * 1 <= m <= 9: q = (|u|^2 - 1, 2u) / (|u|^2 + 1) */
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

#define NULL_SEEDS 5

static long null_total;
static int  null_rows;
static int  null_bad;
static int  all_48[48];
static int  all_120[120];

/* a null point set matched in count to the catalog; points distinct
 * modulo sign (a repeat would be a relation of norm 2 by construction,
 * not by structure - seen at heights <= 5) */
static void
null_row (
    const PointSet *cat,
    const Group    *g,
    const int      *members,   /* the catalog's group elements */
    int             n_members,
    int             kind,      /* 0: r g over K, 1: rational */
    int             radius,
    long            seed)
{
    PointSet s;
    Profile  pr;
    int      j;
    int      k;
    int      units = 1;
    int      rejected = 0;
    char     b1[128];

    rng_state = seed;
    s = *cat;
    sprintf(s.name, "%s null %c seed %ld", cat->name, kind ? 'b' : 'a',
        seed);
    for (j = 0; j < cat->n; j++) {
        Quaternio r = rational_unit(cat->ring);
        Quaternio x = r;
        int       repeat = 0;

        if (kind == 0) {
            (void)quaternio_multiplica(r, g->el[members[rng(n_members)]],
                pool, &x);
        }
        units = units && is_unit(x);
        x = canonical(x);
        for (k = 0; k < j && !repeat; k++) {
            repeat = quaternio_aequalis(s.p[k], x);
        }
        if (repeat) {
            rejected++;
            j--;
            continue;
        }
        s.p[j] = x;
    }
    profile(&s, radius, STORE_LIMIT, NODE_BUDGET, &pr);
    snprintf(b1, sizeof(b1), "%s", (const char *)magnus_ad_chordam(
        pr.first_norm, pool).datum);
    printf("    %c seed %ld: ambient rank %d, relation rank %2d, "
        "relations to R = %d: %ld (%s), |b_1|^2 %s%s\n", kind ? 'b' : 'a',
        seed, pr.ambient_rank, pr.rank, radius, pr.count,
        pr.complete ? "complete" : "INCOMPLETE", b1,
        rejected > 0 ? " (repeats redrawn)" : "");
    if (!units || !pr.complete || pr.rows_checked != pr.rank) {
        null_bad++;
    }
    null_total += pr.count;
    null_rows++;
}

static void
part_d (void)
{
    const PointSet *sets[3];
    const Group    *groups[3];
    const int      *members[3];
    int             n_members[3];
    int             radii[3];
    int             i;
    int             kind;
    long            seed;
    char            msg[256];

    printf("\n=== Part D: baseline nulls (knot-free, exact) ===\n");
    printf("  (a) r g: r random rational unit, g random group element -"
        " same field, same ambient rank\n");
    printf("  (b) random rational unit quaternions - ambient Q^4\n");
    sets[0] = &cat_2t;
    sets[1] = &cat_2o;
    sets[2] = &cat_2i;
    groups[0] = &g48;
    groups[1] = &g48;
    groups[2] = &g120;
    members[0] = members_2t;
    members[1] = all_48;
    members[2] = all_120;
    n_members[0] = 24;
    n_members[1] = 48;
    n_members[2] = 120;
    radii[0] = RADIUS_2T;
    radii[1] = RADIUS_2O;
    radii[2] = RADIUS_2I;
    for (i = 0; i < 120; i++) {
        all_120[i] = i;
        if (i < 48) {
            all_48[i] = i;
        }
    }
    null_total = 0;
    null_rows = 0;
    null_bad = 0;
    for (i = 0; i < 3; i++) {
        printf("  %s, N = %d:\n", sets[i]->name, sets[i]->n);
        for (kind = 0; kind < 2; kind++) {
            for (seed = 1; seed <= NULL_SEEDS; seed++) {
                null_row(sets[i], groups[i], members[i], n_members[i],
                    kind, radii[i], seed + 100 * i);
            }
        }
    }
    sprintf(msg, "%d null sets: distinct unit points, every basis row a "
        "relation, every shell complete", null_rows);
    check(msg, null_bad == 0);
    printf("  OBSERVED: relations within the catalog's radius, all %d "
        "null sets together: %ld\n", null_rows, null_total);
}

/* ================================================================
 * Main
 * ================================================================ */

int
main (void)
{
    pool = piscina_generare_dynamicum("demo_126", 1 << 22);
    if (getenv("D126_CELER") != NULL) {
        radius_2i = 4;
        printf("(quick mode: 2I to norm 4)\n");
    }
    printf("KNOTAPEL DEMO 126: Relation Profiles of 2T, 2O, 2I\n");
    printf("==================================================\n");
    part_a();
    if (n_fail > 0) {
        /* wrong catalogs: every later part would measure something else */
        printf("\nPart A failed: stopping.\n%d passed, %d failed\n",
            n_pass, n_fail);
        return 1;
    }
    part_b();
    part_c();
    part_d();
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pool);
    return n_fail == 0 ? 0 : 1;
}
