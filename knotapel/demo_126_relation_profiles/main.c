/*
 * KNOTAPEL DEMO 126: Relation Profiles of 2T, 2O, 2I
 * ================================================================
 *
 * A neuron sees only sums of its weight points (claims.md KC 47.1.2).
 * Two input patterns with the same sum are indistinguishable under ANY
 * activation (boundary ties, by contrast, depend on it). Their
 * difference is an integer relation among the points:
 *   - signed encoding (D94, D119-D123: S = sum +-q_i): coefficients in
 *     {-1, 0, 1}, and every such relation joins two patterns;
 *   - antipodal 0/1 encoding (D66/D118, D79-D84/D124, D125: inputs +q_i
 *     and -q_i per weight): coefficients in {-2 .. 2}.
 * All integer relations of a point set S = (p_1 .. p_N) form the lattice
 *
 *     L(S) = { x in Z^N : sum x_i p_i = 0 },
 *
 * and its rank, shells by norm and shortest vectors are the set's
 * RELATION PROFILE. sum x_i = sum x_i^2 (mod 2), so only relations of
 * ODD norm join patterns of opposite parity (in both encodings): the
 * only ones that can matter for XOR. This demo measures the profile
 * exactly (house library reticulum: integral LLL + integer Fincke-Pohst)
 * for the group catalogs and for knot-free point sets. It measures no
 * capacity and no activation. Plan: plan.md (approved 2026-10-13).
 *
 *   Part A  catalogs: 2O from D66's generators over Q(sqrt 2) and 2I
 *           from D94's over Q(sqrt 5), both by exact closure as in D118;
 *           2T = the rational elements of 2O (a subgroup of order 24).
 *           Points modulo sign: one representative per {g, -g},
 *           canonical (first nonzero part positive): 12, 24, 60.
 *   Part B  profiles: rank of L(S); LLL-reduced basis, every row
 *           re-checked as a relation in quaternion arithmetic; shells by
 *           norm up to R (counted modulo sign, completeness asserted),
 *           with columns {-1, 0, 1} (signed encoding) and {-2 .. 2}
 *           (antipodal); odd norms marked; the shortest norm.
 *   Part C  invariance: r S r^-1, r S and a re-signed S give identical
 *           profiles (relations are invariant under any linear map
 *           applied to all points; a sign flip is an isometry of Z^N).
 *   Part D  high-height nulls (knot-free, exact, fixed seeds): (a) r g,
 *           r a random rational unit quaternion, g a random element of
 *           the catalog's group (same field, same ambient rank); (b)
 *           random rational unit quaternions (ambient Q^4). Heights
 *           (coordinate denominators) are ~10^30 and more.
 *   Part E  the structure of 2O (added after the review of 2026-10-13):
 *           2O = 2T u sigma_1 2T in separate coordinate blocks, so its
 *           shells are 2T's squared; a unit point set with 2O's whole
 *           relation lattice that is NOT congruent to 2O (the review's
 *           counter-example); the quadric evaluation ranks that decide
 *           when a whole-lattice unit null must be congruent.
 *   Part F  height-matched nulls (added after the review): random
 *           subsets of ALL rational unit quaternions of height H (v/H,
 *           v in Z^4), H = 3 .. 25; at H = 2 and 4 those are 2T itself.
 *
 * Embedding: a coordinate a + b sqrt d becomes (a, b), a point a vector
 * in Q^8; 1 and sqrt d are independent over Q, so the relation lattice
 * of the coordinates IS that of the points.
 *
 * Independent oracles (Python Fractions, no lattice): oracle.py (its own
 * closure and field arithmetic; signed coincidences by direct search;
 * -F: Part F replayed) and oracle_nulls.py + oracle_common.py (Part D
 * replayed draw for draw; written by the reviewer).
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
    long   pm2_by_norm[MAX_R + 1];  /* entries in {-2 .. 2} */
    long   odd_by_norm[MAX_R + 1];  /* odd coefficient sum */
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
        int pm2 = 1;
        s64 sum = 0;

        (void)magnus_ad_s64(e.normae[i], &n);
        pr->by_norm[n]++;
        for (j = 0; j < s->n; j++) {
            s64 v = 0;

            (void)magnus_ad_s64(*(const Magnus *)matrix_elementum(
                e.vectores, (i32)i, (i32)j), &v);
            pm1 = pm1 && v >= -1 && v <= 1;
            pm2 = pm2 && v >= -2 && v <= 2;
            sum += v;
        }
        pr->pm1_by_norm[n] += pm1;
        pr->pm2_by_norm[n] += pm2;
        pr->odd_by_norm[n] += (sum % 2 != 0);
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
            printf("      norm %2d: %7ld   entries in {-1,0,1}: %7ld"
                "   in {-2..2}: %7ld%s\n", n, pr->by_norm[n],
                pr->pm1_by_norm[n], pr->pm2_by_norm[n],
                n % 2 ? "   (odd)" : "");
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
            || a->pm1_by_norm[n] != b->pm1_by_norm[n]
            || a->pm2_by_norm[n] != b->pm2_by_norm[n]) {
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

static int
parity_lemma_holds (
    const Profile *pr)
{
    int n;

    for (n = 1; n <= pr->radius; n++) {
        if (pr->odd_by_norm[n] != (n % 2 ? pr->by_norm[n] : 0)) {
            return 0;
        }
    }
    return 1;
}

static int
pm2_is_all (
    const Profile *pr)
{
    int n;

    for (n = 1; n <= pr->radius; n++) {
        if (pr->pm2_by_norm[n] != pr->by_norm[n]) {
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
    check("relation rank = N - ambient rank, every catalog",
        pr_2t.rank == cat_2t.n - pr_2t.ambient_rank
        && pr_2o.rank == cat_2o.n - pr_2o.ambient_rank
        && pr_2i.rank == cat_2i.n - pr_2i.ambient_rank);
    /* sum x_i = sum x_i^2 (mod 2), so the coefficient sum is odd exactly
     * on odd norms: only odd shells join input patterns of opposite
     * parity, in the signed and the antipodal encoding alike */
    check("coefficient sum odd <=> norm odd, every stored vector",
        parity_lemma_holds(&pr_2t) && parity_lemma_holds(&pr_2o)
        && parity_lemma_holds(&pr_2i));
    /* an entry +-3 needs 3 p_a = signed sum of the others: impossible
     * below norm 12 (|3 p_a| = 3 > 2, the most two unit points reach) */
    check("no entry beyond +-2 in any shell (all relations are in "
        "{-2..2}, as they must be below norm 12)",
        pm2_is_all(&pr_2t) && pm2_is_all(&pr_2o) && pm2_is_all(&pr_2i));
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
    int             ambient,   /* expected ambient rank: the catalog's
                                * for (a), 4 for (b) */
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
    if (!units || !pr.complete || pr.rows_checked != pr.rank
        || pr.ambient_rank != ambient
        || pr.rank != s.n - pr.ambient_rank) {
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
    int             ambients[3];
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
    ambients[0] = pr_2t.ambient_rank;
    ambients[1] = pr_2o.ambient_rank;
    ambients[2] = pr_2i.ambient_rank;
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
                    kind, radii[i], kind ? 4 : ambients[i],
                    seed + 100 * i);
            }
        }
    }
    sprintf(msg, "%d null sets: distinct unit points, ambient rank as "
        "labeled ((a) the catalog's, (b) 4), relation rank N - ambient, "
        "basis rows relations, shells complete", null_rows);
    check(msg, null_bad == 0);
    printf("  OBSERVED: relations within the catalog's radius, all %d "
        "null sets together: %ld\n", null_rows, null_total);
}

/* ================================================================
 * Part E: the structure of 2O (after the review of 2026-10-13)
 * ================================================================ */

static Algebraicus
dot4 (
    Quaternio p,
    Quaternio q)
{
    Algebraicus s = algebraicus_multiplica(part_of(p, 0), part_of(q, 0),
        pool);
    int k;

    for (k = 1; k < 4; k++) {
        s = algebraicus_adde(s, algebraicus_multiplica(part_of(p, k),
            part_of(q, k), pool), pool);
    }
    return s;
}

static int
distinct_inner_products (
    const PointSet *s)
{
    static Algebraicus seen[MAX_POINTS * MAX_POINTS / 2];
    int n_seen = 0;
    int a;
    int b;
    int k;

    for (a = 0; a < s->n; a++) {
        for (b = a + 1; b < s->n; b++) {
            Algebraicus d = dot4(s->p[a], s->p[b]);
            int         found = 0;

            for (k = 0; k < n_seen && !found; k++) {
                found = algebraicus_aequalis(seen[k], d);
            }
            if (!found) {
                seen[n_seen++] = d;
            }
        }
    }
    return n_seen;
}

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

/* S u -S closed under multiplication? */
static int
closed_with_signs (
    const PointSet *s)
{
    int a;
    int b;

    for (a = 0; a < s->n; a++) {
        for (b = 0; b < s->n; b++) {
            Quaternio x;

            (void)quaternio_multiplica(s->p[a], s->p[b], pool, &x);
            if (!contains(s, canonical(x))) {
                return 0;
            }
        }
    }
    return 1;
}

/* rank of the N x 36 matrix of quadratic monomials c_j c_k (j <= k) of a
 * point's 8 rational coordinates. A Q-linear A with p'_i = A p_i keeps
 * unit norm only if both the rational and the sqrt d part of the norm
 * agree on every p_i; at full rank on the coordinates the points span,
 * every inner product is then preserved (congruence). */
static int
quadric_rank (
    const PointSet *s)
{
    Matrix c = coordinates(s);
    Matrix m;
    i32    g = 0;
    int    i;
    int    j;
    int    k;
    int    col;

    if (!matrix_nulla(&ANULUS_RATIONALIUM, (i32)s->n, 36, pool, &m)) {
        fatal("matrix");
    }
    for (i = 0; i < s->n; i++) {
        col = 0;
        for (j = 0; j < 8; j++) {
            for (k = j; k < 8; k++) {
                Fractio f = fractio_multiplica(
                    *(const Fractio *)matrix_elementum(c, (i32)j, (i32)i),
                    *(const Fractio *)matrix_elementum(c, (i32)k, (i32)i),
                    pool);

                matrix_pone(&m, (i32)i, (i32)col, &f);
                col++;
            }
        }
    }
    if (!matrix_gradus(m, pool, &g)) {
        fatal("rank");
    }
    return (int)g;
}

/* mod-sign shell counts of L1 + L2 (orthogonal sum) from those of L1, L2
 * (all vectors: 1 at norm 0, 2 c(n) above) */
static long
square_shell (
    const long *c,
    int         n)
{
    long total = 0;
    int  a;

    for (a = 0; a <= n; a++) {
        long ta = a == 0 ? 1 : 2 * c[a];
        long tb = n - a == 0 ? 1 : 2 * c[n - a];

        total += ta * tb;
    }
    return total / 2;
}

static void
part_e (void)
{
    Quaternio sigma1 = qparse(cat_2o.ring, "[a/2, a/2, 0, 0]");
    Quaternio r = rotor(cat_2o.ring);
    PointSet  coset;
    PointSet  o2;
    PointSet  moved;
    Profile   pr_moved;
    Matrix    p;
    Matrix    q;
    Matrix    stacked;
    i32       g_p = 0;
    i32       g_q = 0;
    i32       g_s = 0;
    int       ok;
    int       j;
    int       k;
    int       n;
    int       blocks = 1;
    char      msg[256];

    printf("\n=== Part E: the structure of 2O ===\n");
    coset = cat_2t;
    strcpy(coset.name, "sigma_1 2T");
    for (j = 0; j < cat_2t.n; j++) {
        Quaternio x;

        (void)quaternio_multiplica(sigma1, cat_2t.p[j], pool, &x);
        coset.p[j] = canonical(x);
    }
    o2 = cat_2t;
    strcpy(o2.name, "2T + sigma_1 2T");
    for (j = 0; j < coset.n; j++) {
        o2.p[o2.n++] = coset.p[j];
    }
    ok = o2.n == cat_2o.n;
    for (j = 0; j < cat_2o.n && ok; j++) {
        ok = contains(&o2, cat_2o.p[j]);
    }
    check("2O mod sign = 2T u sigma_1 2T (as sets)", ok);
    for (j = 0; j < cat_2t.n; j++) {
        for (k = 0; k < 4; k++) {
            Fractio a = algebraicus_coefficiens(part_of(cat_2t.p[j], k), 0,
                pool);
            Fractio b = algebraicus_coefficiens(part_of(cat_2t.p[j], k), 1,
                pool);
            Fractio c = algebraicus_coefficiens(part_of(coset.p[j], k), 0,
                pool);

            blocks = blocks && fractio_signum(b) == 0
                && fractio_signum(c) == 0;
            (void)a;
        }
    }
    check("block form: 2T rational, sigma_1 2T pure sqrt 2 - so L(2O) = "
        "L(2T) + L(sigma_1 2T), and sigma_1 2T is a Q-linear image of 2T",
        blocks);
    ok = 1;
    for (n = 1; n <= RADIUS_2O; n++) {
        ok = ok && square_shell(pr_2t.by_norm, n) == pr_2o.by_norm[n]
            && square_shell(pr_2t.pm1_by_norm, n) == pr_2o.pm1_by_norm[n]
            && square_shell(pr_2t.pm2_by_norm, n) == pr_2o.pm2_by_norm[n];
    }
    check("2O's shells = 2T's squared (theta_2O = theta_2T^2), every "
        "column to norm 8: 2O adds no relation beyond two copies of 2T",
        ok);
    /* the review's counter-example: rotate one block by a rational r */
    moved = cat_2t;
    strcpy(moved.name, "2T + r sigma_1 2T");
    for (j = 0; j < coset.n; j++) {
        Quaternio x;

        (void)quaternio_multiplica(r, coset.p[j], pool, &x);
        moved.p[moved.n++] = x;
    }
    ok = 1;
    for (j = 0; j < moved.n; j++) {
        ok = ok && is_unit(moved.p[j]);
    }
    p = coordinates(&o2);
    q = coordinates(&moved);
    if (!matrix_nulla(&ANULUS_RATIONALIUM, 16, (i32)o2.n, pool,
            &stacked)) {
        fatal("matrix");
    }
    for (j = 0; j < 8; j++) {
        for (k = 0; k < o2.n; k++) {
            matrix_pone(&stacked, (i32)j, (i32)k, matrix_elementum(p,
                (i32)j, (i32)k));
            matrix_pone(&stacked, (i32)(8 + j), (i32)k, matrix_elementum(q,
                (i32)j, (i32)k));
        }
    }
    (void)matrix_gradus(p, pool, &g_p);
    (void)matrix_gradus(q, pool, &g_q);
    (void)matrix_gradus(stacked, pool, &g_s);
    profile(&moved, RADIUS_2O, STORE_LIMIT, NODE_BUDGET, &pr_moved);
    sprintf(msg, "S' = 2T u r sigma_1 2T: unit points, SAME kernel as 2O "
        "(ranks %d, %d, stacked %d), same profile", (int)g_p, (int)g_q,
        (int)g_s);
    check(msg, ok && g_p == 8 && g_q == 8 && g_s == 8
        && same_profile(&pr_moved, &pr_2o));
    {
        int d_o = distinct_inner_products(&o2);
        int d_s = distinct_inner_products(&moved);
        int c_o = closed_with_signs(&o2);
        int c_s = closed_with_signs(&moved);

        sprintf(msg, "but not congruent: distinct inner products 2O %d, S'"
            " %d; closed under multiplication: 2O %s, S' %s", d_o, d_s,
            c_o ? "yes" : "no", c_s ? "yes" : "no");
        check(msg, d_o == 5 && d_s == 11 && c_o && !c_s);
    }
    {
        int r_t = quadric_rank(&cat_2t);
        int r_o = quadric_rank(&cat_2o);
        int r_i = quadric_rank(&cat_2i);

        printf("  quadric evaluation rank (of 10 for 2T's 4 live "
            "coordinates, 36 for 8): 2T %d, 2O %d, 2I %d\n", r_t, r_o,
            r_i);
        check("2T pins every quadratic form on its span (10 of 10): a "
            "unit-norm null with 2T's whole lattice is congruent to 2T",
            r_t == 10);
        check("2O does not (20 of 36; S' above), 2I nearly (35 of 36: open)",
            r_o == 20 && r_i == 35);
    }
}

/* ================================================================
 * Part F: height-matched rational nulls (after the review)
 * ================================================================ */

#define MAX_SPHERE 4096

static long sphere_v[MAX_SPHERE][4];

/* v in Z^4 with |v|^2 = h^2, first nonzero entry positive (one per
 * {v, -v}), in lexicographic order over [-h, h]^4 */
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

static Quaternio
sphere_quaternion (
    int  i,
    long h)
{
    char text[256];

    sprintf(text, "[%ld/%ld, %ld/%ld, %ld/%ld, %ld/%ld]", sphere_v[i][0],
        h, sphere_v[i][1], h, sphere_v[i][2], h, sphere_v[i][3], h);
    return qparse(cat_2o.ring, text);
}

typedef struct {
    long h;
    int  n;
    int  radius;
} HeightRow;

static const HeightRow height_rows[] = {
    { 3, 12, 10 }, { 5, 12, 10 },
    { 5, 24, 8 }, { 6, 24, 8 }, { 9, 24, 8 }, { 25, 24, 8 },
    { 5, 60, 5 }, { 25, 60, 5 }
};

#define HEIGHT_SEEDS 3

static long height_pm1[8][HEIGHT_SEEDS];
static long height_norm3[8][HEIGHT_SEEDS];

/* pinned from oracle.py -F (same sphere order and LCG subsets; signed
 * coincidences by direct search) */
static const long oracle_height_pm1[8][HEIGHT_SEEDS] = {
    { 20, 31, 44 }, { 3, 2, 2 }, { 1750, 1662, 1882 },
    { 4559, 4561, 4427 }, { 195, 169, 176 }, { 3, 3, 5 },
    { 416, 318, 327 }, { 0, 0, 1 }
};
static const long oracle_height_norm3[8][HEIGHT_SEEDS] = {
    { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, { 0, 0, 0 },
    { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }
};

static void
part_f (void)
{
    int  rows = (int)(sizeof(height_rows) / sizeof(height_rows[0]));
    int  idx[MAX_SPHERE];
    int  row;
    int  seed;
    int  i;
    int  m;
    int  bad = 0;
    int  closed = 0;
    int  pins = 1;
    long odd_at_odd_height = 0;
    long h;
    char msg[256];

    printf("\n=== Part F: height-matched rational nulls ===\n");
    printf("  points v/H, v in Z^4, |v| = H (all rational unit "
        "quaternions of height H), random subsets of N (no group)\n");
    for (h = 2; h <= 4; h += 2) {
        PointSet s;
        int      ok;

        m = sphere_points(h);
        s = cat_2t;
        s.n = m;
        for (i = 0; i < m && i < MAX_POINTS; i++) {
            s.p[i] = sphere_quaternion(i, h);
        }
        ok = m == cat_2t.n;
        for (i = 0; i < cat_2t.n && ok; i++) {
            ok = contains(&s, cat_2t.p[i]);
        }
        sprintf(msg, "height %ld: the rational unit quaternions mod sign "
            "ARE 2T (%d points) - height and group cannot be separated "
            "there", h, m);
        check(msg, ok);
    }
    for (row = 0; row < rows; row++) {
        const HeightRow *hr = &height_rows[row];

        m = sphere_points(hr->h);
        printf("  H = %2ld (%4d points mod sign), N = %d, R = %d:\n",
            hr->h, m, hr->n, hr->radius);
        for (seed = 0; seed < HEIGHT_SEEDS; seed++) {
            PointSet s;
            Profile  pr;
            long     pm1 = 0;
            long     odd = 0;
            int      n;

            rng_state = 1000L * hr->h + hr->n + seed;
            for (i = 0; i < m; i++) {
                idx[i] = i;
            }
            s = cat_2t;
            sprintf(s.name, "H %ld N %d seed %d", hr->h, hr->n, seed);
            s.n = hr->n;
            for (i = 0; i < hr->n; i++) {
                int k = i + (int)rng(m - i);
                int t = idx[i];

                idx[i] = idx[k];
                idx[k] = t;
                s.p[i] = sphere_quaternion(idx[i], hr->h);
            }
            profile(&s, hr->radius, STORE_LIMIT, NODE_BUDGET, &pr);
            for (n = 1; n <= hr->radius; n++) {
                pm1 += pr.pm1_by_norm[n];
                if (n % 2) {
                    odd += pr.by_norm[n];
                }
            }
            height_pm1[row][seed] = pm1;
            height_norm3[row][seed] = pr.by_norm[3];
            pins = pins && pm1 == oracle_height_pm1[row][seed]
                && pr.by_norm[3] == oracle_height_norm3[row][seed];
            if (hr->h % 2) {
                odd_at_odd_height += odd;
            }
            closed += closed_with_signs(&s);
            printf("    seed %d: %6ld relations to R (%s), +-1: %6ld, "
                "norm 3: %ld, odd norms: %ld\n", seed, pr.count,
                pr.complete ? "complete" : "INCOMPLETE", pm1, pr.by_norm[3],
                odd);
            if (!pr.complete || !pr.classified
                || pr.rank != s.n - pr.ambient_rank
                || pr.ambient_rank != 4) {
                bad++;
            }
        }
    }
    sprintf(msg, "%d height-matched sets: shells complete, ambient rank 4,"
        " relation rank N - 4; none closed under multiplication",
        rows * HEIGHT_SEEDS);
    check(msg, bad == 0 && closed == 0);
    check("every set's +-1 total and norm-3 count = oracle.py -F", pins);
    /* |v|^2 = h^2 odd => sum of v's coordinates odd; a relation
     * sum x_i v_i = 0 then forces sum x_i even: at odd height no
     * coincidence joins patterns of opposite parity */
    sprintf(msg, "odd heights: no relation of odd norm (%ld) - none can "
        "matter for parity", odd_at_odd_height);
    check(msg, odd_at_odd_height == 0);
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
    part_e();
    part_f();
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pool);
    return n_fail == 0 ? 0 : 1;
}
