/*
 * KNOTAPEL DEMO 118: Exact Quaternions
 * ================================================================
 *
 * Demos 66-97 did their quaternion geometry in doubles: products,
 * eps-dedup of group elements and rotation axes, and nearest-cell
 * ("Voronoi") activations decided by '>' on rounded dot products. This
 * demo redoes the foundations exactly with two house libraries:
 *
 *   - extensio: exact arithmetic in Q(sqrt 2) and Q(sqrt 5), with
 *     EXACT signs (an ordered field: comparisons are decided, never
 *     approximated);
 *   - quaternio: quaternions over any house ring; group elements are
 *     compared exactly, axes by an exact cross product.
 *
 *   Part A  D66's SU(2) generators sigma_1 -> (1+i)/sqrt2, sigma_2 ->
 *           (1-k)/sqrt2 over Q(sqrt 2): braid relation, closure (48),
 *           exact multiplication table, element orders -> the group IS
 *           the binary octahedral group 2O; D66's word enumeration
 *           replayed through the table -> its 24-entry catalog, in
 *           its order
 *   Part B  rotation axes, exactly: 13 for 2O (D66's "13 directions")
 *           split 3 + 4 + 6 by fold
 *   Part C  D66 Part H recounted exactly: XOR6 by the 24-cell Voronoi
 *           activation (nearest catalog entry by |4D dot|) on the
 *           2,024 antipodal triples. Ties are decided exactly and
 *           reported; a float replica of D66's own pipeline is checked
 *           against D66's printed 35 first
 *   Part D  D94's binary icosahedral group over Q(sqrt 5): closure
 *           (120), orders, 60 classes mod sign, 31 axes (6 + 10 + 15)
 *
 * Classification used in A and D (finite subgroups of SU(2) are cyclic,
 * binary dihedral, 2T, 2O or 2I): a group of order 48 whose largest
 * element order is 8 is 2O (cyclic would have order 48, binary dihedral
 * of order 48 has an element of order 24); order 120 with largest
 * element order 10 is 2I.
 *
 * Independent oracle for Part C: oracle.py (Python, its own exact
 * Q(sqrt 2) arithmetic, no house code).
 *
 * House libraries: includes quaternio.h and extensio.h, hence latina.h
 * (Roman numerals and Latin keywords are macros here). Build and run
 * from the repo root:
 *   ./bin/aedilis knotapel/demo_118_exact_quaternions/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 */

#include "quaternio.h"
#include "extensio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

/* 4D dot product p . q = sum of the four part products */
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

/* exact a > b ? (ordered field; refusal is fatal: never guessed) */
static int
greater (
    Algebraicus a,
    Algebraicus b)
{
    s32 s = 0;

    if (!algebraicus_compara(a, b, pool, &s)) {
        printf("  FATAL: comparison refused\n");
        exit(1);
    }
    return s > 0;
}

static int
nonzero_parts (
    Quaternio q)
{
    int k;
    int n = 0;

    for (k = 0; k < 4; k++) {
        if (!algebraicus_est_nullum(part_of(q, k))) {
            n++;
        }
    }
    return n;
}

/* value of an element of Q(sqrt d) as a double (display and the float
 * replica cross-check only - never used to decide anything) */
static double
to_double (
    Algebraicus a,
    double      root)
{
    double v = 0.0;
    int    j;

    for (j = 0; j < 2; j++) {
        Fractio f = algebraicus_coefficiens(a, (i32)j, pool);
        s64 num = 0;
        s64 den = 1;

        if (!magnus_ad_s64(fractio_numerator(f), &num)
            || !magnus_ad_s64(fractio_denominator(f), &den)) {
            printf("  FATAL: coefficient beyond s64\n");
            exit(1);
        }
        v += (double)num / (double)den * (j == 0 ? 1.0 : root);
    }
    return v;
}

/* ================================================================
 * Finite groups of quaternions, exactly
 * ================================================================ */

#define MAX_GROUP 120

typedef struct {
    const Anulus *ring;
    Quaternio     el[MAX_GROUP];
    int           n;
    int           mul[MAX_GROUP][MAX_GROUP];
    int           neg[MAX_GROUP];
    int           one;
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

/* closure of {1} under right multiplication by the generators; then the
 * full table. Returns 0 if the group exceeds MAX_GROUP. */
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
    g->one = 0;
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
            Quaternio      zero;
            Quaternio      minus;

            (void)quaternio_nullum(ring, pool, &zero);
            (void)quaternio_subtrahe(zero, g->el[a], pool, &minus);
            g->neg[a] = group_find(g, minus);
            piscina_reficere(pool, mark);
            if (g->neg[a] < 0) {
                return 0;
            }
        }
    }
    return 1;
}

static int
element_order (
    const Group *g,
    int          a)
{
    int x = a;
    int n = 1;

    while (x != g->one) {
        x = g->mul[x][a];
        n++;
    }
    return n;
}

/* prints the histogram of element orders; returns the largest order */
static int
order_histogram (
    const Group *g,
    int         *hist,
    int          max_order)
{
    int a;
    int largest = 0;
    int o;

    for (o = 0; o <= max_order; o++) {
        hist[o] = 0;
    }
    for (a = 0; a < g->n; a++) {
        o = element_order(g, a);
        if (o <= max_order) {
            hist[o]++;
        }
        if (o > largest) {
            largest = o;
        }
    }
    printf("  orders:");
    for (o = 1; o <= max_order; o++) {
        if (hist[o] > 0) {
            printf(" %d^%d", o, hist[o]);
        }
    }
    printf("   (order^count)\n");
    return largest;
}

/* distinct rotation axes of the non-scalar elements; fold of an axis =
 * (non-scalar elements on it + 2) / 2 (the + 2 are +-1) */
#define MAX_AXES 64

static int
group_axes (
    const Group *g,
    int         *axis_rep,
    int         *axis_count)
{
    int n_axes = 0;
    int a;
    int k;

    for (a = 0; a < g->n; a++) {
        Quaternio q = g->el[a];
        int       found = -1;

        if (algebraicus_est_nullum(part_of(q, 1))
            && algebraicus_est_nullum(part_of(q, 2))
            && algebraicus_est_nullum(part_of(q, 3))) {
            continue;
        }
        for (k = 0; k < n_axes; k++) {
            if (quaternio_eadem_axis(g->el[axis_rep[k]], q, pool)) {
                found = k;
                break;
            }
        }
        if (found < 0) {
            if (n_axes >= MAX_AXES) {
                return -1;
            }
            axis_rep[n_axes] = a;
            axis_count[n_axes] = 0;
            found = n_axes++;
        }
        axis_count[found]++;
    }
    return n_axes;
}

static void
fold_census (
    int        n_axes,
    const int *axis_count,
    int       *by_fold,
    int        max_fold)
{
    int k;
    int f;

    for (f = 0; f <= max_fold; f++) {
        by_fold[f] = 0;
    }
    for (k = 0; k < n_axes; k++) {
        f = (axis_count[k] + 2) / 2;
        if (f <= max_fold) {
            by_fold[f]++;
        }
    }
}

/* ================================================================
 * Part A: 2O from D66's generators, exactly
 * ================================================================ */

static Extensio *q2;          /* Q(sqrt 2), alpha = +sqrt 2 */
static Group     g48;
static int       gen_index[3][2];   /* [sigma][0 = +, 1 = inverse] */
static int       cat_index[24];     /* D66's catalog, its order */
static int       cat_size = 0;

/* D66's enumeration: n = 2..3 strands, length 1..8, words by base-
 * (2(n-1)) digits, first generator first; sigma_i^-1 = conj(sigma_i).
 * Catalog = first occurrence of each element up to sign. */
static void
replay_d66_catalog (
    int *reached)
{
    int n;
    int len;
    int a;

    for (a = 0; a < g48.n; a++) {
        reached[a] = 0;
    }
    cat_size = 0;
    for (n = 2; n <= 3; n++) {
        for (len = 1; len <= 8; len++) {
            int           max_gen = n - 1;
            int           total_gens = 2 * max_gen;
            unsigned long total = 1;
            unsigned long idx;
            int           j;

            for (j = 0; j < len; j++) {
                total *= (unsigned long)total_gens;
            }
            if (total > 100000UL) {
                continue;
            }
            for (idx = 0; idx < total; idx++) {
                unsigned long tmp = idx;
                int           cur = g48.one;
                int           seen = 0;

                for (j = 0; j < len; j++) {
                    int digit = (int)(tmp % (unsigned long)total_gens);

                    tmp /= (unsigned long)total_gens;
                    if (digit < max_gen) {
                        cur = g48.mul[cur][gen_index[digit + 1][0]];
                    } else {
                        cur = g48.mul[cur][gen_index[digit - max_gen + 1][1]];
                    }
                }
                reached[cur] = 1;
                for (j = 0; j < cat_size; j++) {
                    if (cat_index[j] == cur || cat_index[j] == g48.neg[cur]) {
                        seen = 1;
                        break;
                    }
                }
                if (!seen && cat_size < 24) {
                    cat_index[cat_size++] = cur;
                }
            }
        }
    }
}

static const char *
vertex_type (
    Quaternio q)
{
    int nz = nonzero_parts(q);

    return nz == 1 ? "axis" : nz == 2 ? "edge" : nz == 4 ? "body" : "???";
}

static void
part_a (void)
{
    Quaternio gens[4];
    Quaternio lhs;
    Quaternio rhs;
    Quaternio t;
    int       hist[64];
    int       reached[MAX_GROUP];
    int       n_reached = 0;
    int       largest;
    int       a;
    char      msg[256];

    printf("\n=== Part A: D66's generators over Q(sqrt 2) -> 2O ===\n");
    q2 = extensio_quadratica(2, pool);
    check("Q(sqrt 2) built, ordered", q2 != NULL && extensio_ordinata(q2));
    gens[0] = qparse(extensio_anulus(q2), "[a/2, a/2, 0, 0]");
    gens[1] = qparse(extensio_anulus(q2), "[a/2, -a/2, 0, 0]");
    gens[2] = qparse(extensio_anulus(q2), "[a/2, 0, 0, -a/2]");
    gens[3] = qparse(extensio_anulus(q2), "[a/2, 0, 0, a/2]");
    {
        Quaternio c0;
        Quaternio c2;

        (void)quaternio_conjugatum(gens[0], pool, &c0);
        (void)quaternio_conjugatum(gens[2], pool, &c2);
        check("sigma_i^-1 = conj(sigma_i) (generators 1, 3)",
            quaternio_aequalis(c0, gens[1])
            && quaternio_aequalis(c2, gens[3]));
    }
    (void)quaternio_multiplica(gens[0], gens[2], pool, &t);
    (void)quaternio_multiplica(t, gens[0], pool, &lhs);
    (void)quaternio_multiplica(gens[2], gens[0], pool, &t);
    (void)quaternio_multiplica(t, gens[2], pool, &rhs);
    check("braid relation s1 s2 s1 = s2 s1 s2 EXACTLY",
        quaternio_aequalis(lhs, rhs));
    check("s1 s2 s1 = (i - k)/sqrt 2 exactly", quaternio_aequalis(lhs,
        qparse(extensio_anulus(q2), "[0, a/2, 0, -a/2]")));

    if (!group_close(&g48, extensio_anulus(q2), gens, 4)) {
        check("closure of <sigma_1, sigma_2> is finite (<= 120)", 0);
        return;
    }
    sprintf(msg, "closure has exactly 48 elements (got %d)", g48.n);
    check(msg, g48.n == 48);
    largest = order_histogram(&g48, hist, 63);
    check("orders 1^1 2^1 3^8 4^18 6^8 8^12 (2O)",
        hist[1] == 1 && hist[2] == 1 && hist[3] == 8 && hist[4] == 18
        && hist[6] == 8 && hist[8] == 12);
    check("largest order 8 -> the group IS 2O (classification)",
        largest == 8);
    for (a = 0; a < g48.n; a++) {
        int ok = g48.neg[a] != a && g48.neg[g48.neg[a]] == a;

        if (!ok) {
            check("negation is a fixed-point-free involution", 0);
            return;
        }
    }
    check("negation is a fixed-point-free involution (24 classes)", 1);

    gen_index[1][0] = group_find(&g48, gens[0]);
    gen_index[1][1] = group_find(&g48, gens[1]);
    gen_index[2][0] = group_find(&g48, gens[2]);
    gen_index[2][1] = group_find(&g48, gens[3]);
    replay_d66_catalog(reached);
    for (a = 0; a < g48.n; a++) {
        n_reached += reached[a];
    }
    sprintf(msg, "D66 words (2-3 strands, length <= 8) reach all %d "
        "elements (got %d)", g48.n, n_reached);
    check(msg, n_reached == 48);
    sprintf(msg, "D66 catalog replayed: 24 classes mod sign (got %d) - "
        "D66 printed 24", cat_size);
    check(msg, cat_size == 24);
    {
        int n_axis = 0;
        int n_edge = 0;
        int n_body = 0;

        for (a = 0; a < cat_size; a++) {
            const char *vt = vertex_type(g48.el[cat_index[a]]);

            n_axis += strcmp(vt, "axis") == 0;
            n_edge += strcmp(vt, "edge") == 0;
            n_body += strcmp(vt, "body") == 0;
        }
        printf("  catalog vertex types: axis %d, edge %d, body %d\n",
            n_axis, n_edge, n_body);
        check("catalog = 4 axis + 12 edge + 8 body (24-cell + dual: F4 "
            "roots mod sign)", n_axis == 4 && n_edge == 12 && n_body == 8);
    }
}

/* ================================================================
 * Part B: rotation axes of 2O
 * ================================================================ */

static void
part_b (void)
{
    int  axis_rep[MAX_AXES];
    int  axis_count[MAX_AXES];
    int  by_fold[16];
    int  n_axes;
    char msg[256];

    printf("\n=== Part B: rotation axes of 2O, exactly ===\n");
    n_axes = group_axes(&g48, axis_rep, axis_count);
    sprintf(msg, "2O has exactly 13 rotation axes (got %d) - D66's 13 "
        "directions", n_axes);
    check(msg, n_axes == 13);
    fold_census(n_axes, axis_count, by_fold, 15);
    printf("  folds: 4-fold %d, 3-fold %d, 2-fold %d\n", by_fold[4],
        by_fold[3], by_fold[2]);
    check("3 four-fold (faces) + 4 three-fold (vertices) + 6 two-fold "
        "(edges) of the cube", by_fold[4] == 3 && by_fold[3] == 4
        && by_fold[2] == 6);
}

/* ================================================================
 * Part C: D66 Part H, exactly
 *
 * Weights: three antipodal pairs +-c_a, +-c_b, +-c_c of catalog
 * entries (a < b < c). A mask's sum is x_a c_a + x_b c_b + x_c c_c
 * with x in {-1, 0, 1} (both of a pair = 0), and its parity is the
 * number of nonzero x (a pair contributes 0 or 2 bits when x = 0):
 * the 64 masks give 27 x-PATTERNS, each with one parity. As VECTORS
 * there may be fewer (19-27): linear relations among c_a, c_b, c_c can
 * give two patterns the same sum with opposite parities - such a set
 * fails outright (one point, one cell). Cell of a sum: the catalog
 * entry maximizing |sum . c_m| (25th cell: the zero sum). XOR6 is
 * computed iff no cell receives both parities.
 *
 * Exactly: dot(sum, c_m) = sum_j x_j G[j][m] with the Gram table G;
 * |d| is compared as d^2. T(s) = the set of tied maxima.
 *   first-index  ties to the lowest index (D66's '>' rule, if its
 *                float dots had been exact)
 *   robust       passes under EVERY tie resolution
 *   possible     passes under SOME tie resolution (a tie rule is a
 *                function of the point: one vector, one cell)
 *
 * Characterization (asserted set by set; recensio D118 M2):
 *   possible <=> c_a, c_b, c_c mutually orthogonal;
 *   robust   <=> the triple lies inside one coset g Q8 of the quaternion
 *                group Q8 = {+-1, +-i, +-j, +-k} (6 cosets, each an
 *                orthonormal 4-frame; the catalog has 24 such frames,
 *                and the 18 others contain no robust triple);
 *   a non-orthogonal set has a pair sum c_i +- c_j tied between its
 *   own two members and fails under every tie rule.
 * ================================================================ */

#define N_CELLS 25
#define ZERO_CELL 24

static Algebraicus gram[24][24];

typedef struct {
    int parity;
    int n_ties;
    int ties[N_CELLS];
} SumCell;

static void
cells_of_triple (
    int      a,
    int      b,
    int      c,
    SumCell *out)
{
    int xs[3];
    int t = 0;

    for (xs[0] = -1; xs[0] <= 1; xs[0]++) {
        for (xs[1] = -1; xs[1] <= 1; xs[1]++) {
            for (xs[2] = -1; xs[2] <= 1; xs[2]++) {
                PiscinaNotatio mark = piscina_notare(pool);
                int            idx[3];
                Algebraicus    best;
                int            m;
                int            j;
                int            all_zero = 1;
                SumCell       *s = &out[t++];

                idx[0] = a;
                idx[1] = b;
                idx[2] = c;
                s->parity = ((xs[0] != 0) + (xs[1] != 0) + (xs[2] != 0)) & 1;
                s->n_ties = 0;
                best = algebraicus_ex_fractione(q2, fractio_ex_s64(0), pool);
                for (m = 0; m < 24; m++) {
                    Algebraicus d = algebraicus_ex_fractione(q2,
                        fractio_ex_s64(0), pool);
                    Algebraicus sq;

                    for (j = 0; j < 3; j++) {
                        if (xs[j] > 0) {
                            d = algebraicus_adde(d, gram[idx[j]][m], pool);
                        } else if (xs[j] < 0) {
                            d = algebraicus_subtrahe(d, gram[idx[j]][m],
                                pool);
                        }
                    }
                    if (!algebraicus_est_nullum(d)) {
                        all_zero = 0;
                    }
                    sq = algebraicus_multiplica(d, d, pool);
                    if (s->n_ties == 0 || greater(sq, best)) {
                        best = sq;
                        s->n_ties = 0;
                        s->ties[s->n_ties++] = m;
                    } else if (algebraicus_aequalis(sq, best)) {
                        s->ties[s->n_ties++] = m;
                    }
                }
                if (all_zero) {
                    /* the catalog spans R^4: all dots 0 <=> sum 0 */
                    s->n_ties = 1;
                    s->ties[0] = ZERO_CELL;
                }
                piscina_reficere(pool, mark);
            }
        }
    }
}

static int
verdict_first_index (
    const SumCell *s)
{
    int seen[N_CELLS];
    int t;

    memset(seen, 0, sizeof(seen));
    for (t = 0; t < 27; t++) {
        int cell = s[t].ties[0];

        seen[cell] |= 1 << s[t].parity;
        if (seen[cell] == 3) {
            return 0;
        }
    }
    return 1;
}

/* every resolution passes <=> no cell is reachable from both parities */
static int
verdict_robust (
    const SumCell *s)
{
    int seen[N_CELLS];
    int t;
    int k;

    memset(seen, 0, sizeof(seen));
    for (t = 0; t < 27; t++) {
        for (k = 0; k < s[t].n_ties; k++) {
            seen[s[t].ties[k]] |= 1 << s[t].parity;
        }
    }
    for (k = 0; k < N_CELLS; k++) {
        if (seen[k] == 3) {
            return 0;
        }
    }
    return 1;
}

/* some resolution passes <=> some labeling L of cells has, for every
 * sum, a tied cell labeled with the sum's parity. Forced labels from
 * single-cell sums, then exhaustive search over the free cells. */
static int
verdict_possible (
    const SumCell *s,
    int            clash,
    int           *undecided)
{
    int label[N_CELLS];
    int free_cells[N_CELLS];
    int n_free = 0;
    int t;
    int k;
    unsigned long bits;

    if (clash) {
        return 0;
    }
    for (k = 0; k < N_CELLS; k++) {
        label[k] = -1;
    }
    for (t = 0; t < 27; t++) {
        if (s[t].n_ties == 1) {
            int cell = s[t].ties[0];

            if (label[cell] >= 0 && label[cell] != s[t].parity) {
                return 0;
            }
            label[cell] = s[t].parity;
        }
    }
    for (t = 0; t < 27; t++) {
        for (k = 0; k < s[t].n_ties; k++) {
            int cell = s[t].ties[k];
            int j;
            int known = 0;

            if (label[cell] >= 0) {
                continue;
            }
            for (j = 0; j < n_free; j++) {
                known |= free_cells[j] == cell;
            }
            if (!known) {
                free_cells[n_free++] = cell;
            }
        }
    }
    if (n_free > 20) {
        (*undecided)++;
        return 0;
    }
    for (bits = 0; bits < (1UL << n_free); bits++) {
        int ok = 1;

        for (k = 0; k < n_free; k++) {
            label[free_cells[k]] = (int)((bits >> k) & 1UL);
        }
        for (t = 0; t < 27 && ok; t++) {
            int hit = 0;

            for (k = 0; k < s[t].n_ties; k++) {
                hit |= label[s[t].ties[k]] == s[t].parity;
            }
            ok = hit;
        }
        if (ok) {
            return 1;
        }
    }
    return 0;
}

/* ---------- float replica of D66 (its code, its order) ---------- */

typedef struct { double a, b, c, d; } FQuat;

static FQuat fq_make (double a, double b, double c, double d)
{ FQuat q; q.a = a; q.b = b; q.c = c; q.d = d; return q; }

static FQuat fq_mul (FQuat p, FQuat q)
{
    return fq_make(
        p.a*q.a - p.b*q.b - p.c*q.c - p.d*q.d,
        p.a*q.b + p.b*q.a + p.c*q.d - p.d*q.c,
        p.a*q.c - p.b*q.d + p.c*q.a + p.d*q.b,
        p.a*q.d + p.b*q.c - p.c*q.b + p.d*q.a);
}

static FQuat fq_conj (FQuat q) { return fq_make(q.a, -q.b, -q.c, -q.d); }
static FQuat fq_neg (FQuat q) { return fq_make(-q.a, -q.b, -q.c, -q.d); }
static FQuat fq_add (FQuat p, FQuat q)
{ return fq_make(p.a + q.a, p.b + q.b, p.c + q.c, p.d + q.d); }
static double fq_dot (FQuat p, FQuat q)
{ return p.a*q.a + p.b*q.b + p.c*q.c + p.d*q.d; }
static double fq_norm (FQuat q) { return sqrt(fq_dot(q, q)); }

static int fq_close (FQuat p, FQuat q, double eps)
{
    return fabs(p.a - q.a) < eps && fabs(p.b - q.b) < eps &&
           fabs(p.c - q.c) < eps && fabs(p.d - q.d) < eps;
}

static FQuat fcat[64];
static int   fcat_size = 0;

static void
float_build_catalog (void)
{
    double s = 1.0 / sqrt(2.0);
    FQuat  gen[3];
    int    n;
    int    len;

    gen[1] = fq_make(s, s, 0, 0);
    gen[2] = fq_make(s, 0, 0, -s);
    fcat_size = 0;
    for (n = 2; n <= 3; n++) {
        for (len = 1; len <= 8; len++) {
            int           max_gen = n - 1;
            int           total_gens = 2 * max_gen;
            unsigned long total = 1;
            unsigned long idx;
            int           j;

            for (j = 0; j < len; j++) {
                total *= (unsigned long)total_gens;
            }
            if (total > 100000UL) {
                continue;
            }
            for (idx = 0; idx < total; idx++) {
                unsigned long tmp = idx;
                FQuat         r = fq_make(1, 0, 0, 0);
                int           found = 0;

                for (j = 0; j < len; j++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    int w;

                    tmp /= (unsigned long)total_gens;
                    w = g < max_gen ? g + 1 : -(g - max_gen + 1);
                    r = fq_mul(r, w > 0 ? gen[w] : fq_conj(gen[-w]));
                }
                for (j = 0; j < fcat_size; j++) {
                    if (fq_close(fcat[j], r, 1e-10)
                        || fq_close(fcat[j], fq_neg(r), 1e-10)) {
                        found = 1;
                        break;
                    }
                }
                if (!found && fcat_size < 64) {
                    fcat[fcat_size++] = r;
                }
            }
        }
    }
}

/* D66's voronoi_cell_24 + check_gen_xor6_voronoi, verbatim logic */
static int
float_xor6_voronoi (
    int a,
    int b,
    int c)
{
    FQuat w[6];
    int   seen[65];
    int   mask;
    int   i;

    w[0] = fcat[a]; w[1] = fq_neg(fcat[a]);
    w[2] = fcat[b]; w[3] = fq_neg(fcat[b]);
    w[4] = fcat[c]; w[5] = fq_neg(fcat[c]);
    for (i = 0; i <= fcat_size; i++) {
        seen[i] = 0;
    }
    for (mask = 0; mask < 64; mask++) {
        FQuat sum = fq_make(0, 0, 0, 0);
        int   par = 0;
        int   cell;

        for (i = 0; i < 6; i++) {
            if (mask & (1 << i)) {
                sum = fq_add(sum, w[i]);
                par ^= 1;
            }
        }
        if (fq_norm(sum) < 1e-12) {
            cell = fcat_size;
        } else {
            double nrm = fq_norm(sum);
            double best_sim = -2.0;
            int    m;

            sum = fq_make(sum.a / nrm, sum.b / nrm, sum.c / nrm,
                sum.d / nrm);
            cell = 0;
            for (m = 0; m < fcat_size; m++) {
                double sim = fabs(fq_dot(sum, fcat[m]));

                if (sim > best_sim) {
                    best_sim = sim;
                    cell = m;
                }
            }
        }
        seen[cell] |= (1 << par);
        if (seen[cell] == 3) {
            return 0;
        }
    }
    return 1;
}

static unsigned char v_robust_of[24][24][24];
static unsigned char v_first_of[24][24][24];
static unsigned char v_possible_of[24][24][24];

/* catalog class of a group element (q ~ -q) */
static int
class_of (
    int e)
{
    int j;

    for (j = 0; j < cat_size; j++) {
        if (cat_index[j] == e || cat_index[j] == g48.neg[e]) {
            return j;
        }
    }
    return -1;
}

static int
sorted_lookup (
    unsigned char tab[24][24][24],
    int           x,
    int           y,
    int           z)
{
    int t;

    if (x > y) { t = x; x = y; y = t; }
    if (y > z) { t = y; y = z; z = t; }
    if (x > y) { t = x; x = y; y = t; }
    return tab[x][y][z];
}

static int perms[96][24];

/* distinct permutations among the 96 maps (left and right coincide
 * only for +-1: 24 + 24 - 1 = 47, D66 Part I printed 47) */
static int
distinct_perms (void)
{
    int n = 0;
    int k;
    int j;

    for (k = 0; k < 96; k++) {
        int fresh = 1;

        for (j = 0; j < k && fresh; j++) {
            fresh = memcmp(perms[k], perms[j], sizeof(perms[k])) != 0;
        }
        n += fresh;
    }
    return n;
}

/* each of the 96 maps q -> g q, q -> q g permutes the catalog and keeps
 * every dot product, so an exact verdict must be invariant under it.
 * Returns the number of maps under which 'tab' is NOT invariant. */
static int
symmetry_breaks (
    unsigned char tab[24][24][24])
{
    int g;
    int side;
    int broken = 0;

    for (g = 0; g < g48.n; g++) {
        for (side = 0; side < 2; side++) {
            int perm[24];
            int m;
            int a;
            int b;
            int c;
            int ok = 1;

            for (m = 0; m < 24; m++) {
                int e = side == 0 ? g48.mul[g][cat_index[m]]
                    : g48.mul[cat_index[m]][g];

                perm[m] = class_of(e);
                perms[g * 2 + side][m] = perm[m];
            }
            for (a = 0; a < 24 && ok; a++) {
                for (b = a + 1; b < 24 && ok; b++) {
                    for (c = b + 1; c < 24 && ok; c++) {
                        ok = tab[a][b][c] == sorted_lookup(tab, perm[a],
                            perm[b], perm[c]);
                    }
                }
            }
            broken += !ok;
        }
    }
    return broken;
}

/* D66's actual Part H winners: knotapel/demo_66_quaternionic_dkc/
 * main.c compiled unmodified except one fprintf of (ai, aj, ak) per
 * winner (scratch copy, clang -O2), 2026-10-08. */
static const int D66_WINNERS[35][3] = {
    {0,1,18}, {0,1,22}, {0,1,23}, {0,18,22}, {1,18,22}, {2,3,9},
    {2,3,23}, {2,9,19}, {2,9,21}, {2,9,23}, {2,18,22}, {3,9,23},
    {4,5,15}, {4,5,16}, {4,5,23}, {4,15,16}, {5,15,16}, {6,7,11},
    {6,7,12}, {6,11,12}, {6,12,20}, {7,11,12}, {7,11,17}, {8,10,13},
    {8,10,14}, {8,13,14}, {8,14,20}, {9,15,16}, {10,13,14}, {10,13,17},
    {17,19,20}, {17,19,21}, {17,20,21}, {17,20,23}, {19,20,21}
};

static int
d66_winner (
    int a,
    int b,
    int c)
{
    int k;

    for (k = 0; k < 35; k++) {
        if (D66_WINNERS[k][0] == a && D66_WINNERS[k][1] == b
            && D66_WINNERS[k][2] == c) {
            return 1;
        }
    }
    return 0;
}

/* the 27 x-pattern sums as exact VECTORS: distinct count, x != 0
 * patterns summing to 0, and whether one vector has both parities */
static int
vector_clash (
    int  a,
    int  b,
    int  c,
    int *n_distinct,
    int *n_zero)
{
    PiscinaNotatio mark = piscina_notare(pool);
    Quaternio      vec[27];
    int            par[27];
    int            seen_par[27];
    int            rep[27];
    int            xs[3];
    int            idx[3];
    int            t = 0;
    int            u;
    int            clash = 0;

    idx[0] = a;
    idx[1] = b;
    idx[2] = c;
    *n_distinct = 0;
    *n_zero = 0;
    for (xs[0] = -1; xs[0] <= 1; xs[0]++) {
        for (xs[1] = -1; xs[1] <= 1; xs[1]++) {
            for (xs[2] = -1; xs[2] <= 1; xs[2]++) {
                Quaternio sum;
                int       j;

                (void)quaternio_nullum(extensio_anulus(q2), pool, &sum);
                for (j = 0; j < 3; j++) {
                    Quaternio next;

                    if (xs[j] > 0) {
                        (void)quaternio_adde(sum, g48.el[cat_index[idx[j]]],
                            pool, &next);
                        sum = next;
                    } else if (xs[j] < 0) {
                        (void)quaternio_subtrahe(sum,
                            g48.el[cat_index[idx[j]]], pool, &next);
                        sum = next;
                    }
                }
                par[t] = ((xs[0] != 0) + (xs[1] != 0) + (xs[2] != 0)) & 1;
                if ((xs[0] || xs[1] || xs[2]) && quaternio_est_nullum(sum)) {
                    (*n_zero)++;
                }
                rep[t] = -1;
                for (u = 0; u < *n_distinct; u++) {
                    if (quaternio_aequalis(vec[u], sum)) {
                        rep[t] = u;
                        break;
                    }
                }
                if (rep[t] < 0) {
                    vec[*n_distinct] = sum;
                    seen_par[*n_distinct] = 0;
                    rep[t] = (*n_distinct)++;
                }
                seen_par[rep[t]] |= 1 << par[t];
                if (seen_par[rep[t]] == 3) {
                    clash = 1;
                }
                t++;
            }
        }
    }
    piscina_reficere(pool, mark);
    return clash;
}

/* orthonormal 4-frames of the catalog (4 mutually orthogonal classes)
 * and the cosets of Q8 = {+-1, +-i, +-j, +-k} (normal in 2O): each
 * coset g Q8 is 8 elements = 4 classes, and left multiplication keeps
 * the frame {1, i, j, k} orthonormal - so the 6 cosets are 6 of the
 * frames. coset_of[m] = coset index of class m. */
static int orth[24][24];
static int coset_of[24];

static int class_of (int e);

static int
count_frames (void)
{
    int a;
    int b;
    int c;
    int d;
    int n = 0;

    for (a = 0; a < 24; a++) {
        for (b = 0; b < 24; b++) {
            orth[a][b] = algebraicus_est_nullum(gram[a][b]);
        }
    }
    for (a = 0; a < 24; a++) {
        for (b = a + 1; b < 24; b++) {
            for (c = b + 1; c < 24; c++) {
                for (d = c + 1; d < 24; d++) {
                    n += orth[a][b] && orth[a][c] && orth[a][d] && orth[b][c]
                        && orth[b][d] && orth[c][d];
                }
            }
        }
    }
    return n;
}

/* returns the number of cosets; 0 if Q8 is not 8 elements or a coset is
 * not an orthonormal 4-frame */
static int
q8_cosets (void)
{
    int q8[8];
    int n_q8 = 0;
    int e;
    int m;
    int k;
    int n = 0;

    for (e = 0; e < g48.n; e++) {
        if (nonzero_parts(g48.el[e]) == 1) {
            if (n_q8 >= 8) {
                return 0;
            }
            q8[n_q8++] = e;
        }
    }
    if (n_q8 != 8) {
        return 0;
    }
    for (m = 0; m < 24; m++) {
        coset_of[m] = -1;
    }
    for (m = 0; m < 24; m++) {
        int members[8];
        int n_members = 0;
        int j;

        if (coset_of[m] >= 0) {
            continue;
        }
        for (k = 0; k < 8; k++) {
            int cl = class_of(g48.mul[cat_index[m]][q8[k]]);
            int known = 0;

            for (j = 0; j < n_members; j++) {
                known |= members[j] == cl;
            }
            if (!known) {
                members[n_members++] = cl;
            }
        }
        if (n_members != 4) {
            return 0;
        }
        printf("  coset %d of Q8: classes {%d,%d,%d,%d}\n", n, members[0],
            members[1], members[2], members[3]);
        for (j = 0; j < 4; j++) {
            int i2;

            coset_of[members[j]] = n;
            for (i2 = j + 1; i2 < 4; i2++) {
                if (!orth[members[j]][members[i2]]) {
                    return 0;
                }
            }
        }
        n++;
    }
    return n;
}

/* size of the orbit of a triple under the 96 maps (perms filled by
 * symmetry_breaks) */
static unsigned char in_orbit[24][24][24];

static void
sort3 (
    int *x)
{
    int t;

    if (x[0] > x[1]) { t = x[0]; x[0] = x[1]; x[1] = t; }
    if (x[1] > x[2]) { t = x[1]; x[1] = x[2]; x[2] = t; }
    if (x[0] > x[1]) { t = x[0]; x[0] = x[1]; x[1] = t; }
}

static int
orbit_size (
    int a,
    int b,
    int c)
{
    static int queue[2024][3];
    int        head = 0;
    int        tail = 0;

    memset(in_orbit, 0, sizeof(in_orbit));
    queue[tail][0] = a;
    queue[tail][1] = b;
    queue[tail][2] = c;
    tail++;
    in_orbit[a][b][c] = 1;
    while (head < tail) {
        int k;

        for (k = 0; k < 96; k++) {
            int x[3];

            x[0] = perms[k][queue[head][0]];
            x[1] = perms[k][queue[head][1]];
            x[2] = perms[k][queue[head][2]];
            sort3(x);
            if (!in_orbit[x[0]][x[1]][x[2]]) {
                in_orbit[x[0]][x[1]][x[2]] = 1;
                queue[tail][0] = x[0];
                queue[tail][1] = x[1];
                queue[tail][2] = x[2];
                tail++;
            }
        }
        head++;
    }
    return tail;
}

static void
part_c (void)
{
    int     a;
    int     b;
    int     c;
    int     n_sets = 0;
    int     n_float = 0;
    int     n_first = 0;
    int     n_robust = 0;
    int     n_possible = 0;
    int     n_undecided = 0;
    int     n_tied_sets = 0;
    int     n_float_not_robust = 0;
    int     n_float_vs_first = 0;
    int     n_float_outside_possible = 0;
    int     max_ties = 0;
    int     n_clash = 0;
    int     min_distinct = 99;
    int     max_distinct = 0;
    int     n_zero_patterns = 0;
    int     n_orth = 0;
    int     n_in_frame = 0;
    int     n_pair_tie = 0;
    int     bad_possible = 0;
    int     bad_robust = 0;
    int     bad_pair_tie = 0;
    int     bad_tie_size = 0;
    int     d66_vs_float = 0;
    int     d66_vs_first = 0;
    int     n_frames;
    int     first_robust[3];
    char    msg[256];

    printf("\n=== Part C: D66 Part H (24-cell Voronoi XOR6), exactly ===\n");
    if (cat_size != 24) {
        check("Part A catalog available", 0);
        return;
    }
    float_build_catalog();
    sprintf(msg, "float replica builds D66's catalog: %d entries",
        fcat_size);
    check(msg, fcat_size == 24);
    {
        double worst = 0.0;
        double r2 = sqrt(2.0);

        for (a = 0; a < 24; a++) {
            Quaternio q = g48.el[cat_index[a]];
            FQuat     e = fq_make(to_double(part_of(q, 0), r2),
                to_double(part_of(q, 1), r2), to_double(part_of(q, 2), r2),
                to_double(part_of(q, 3), r2));
            double    d = fabs(e.a - fcat[a].a) + fabs(e.b - fcat[a].b)
                + fabs(e.c - fcat[a].c) + fabs(e.d - fcat[a].d);

            if (d > worst) {
                worst = d;
            }
        }
        sprintf(msg, "exact and float catalogs agree entry by entry, same "
            "sign, same order (max |diff| %.1e)", worst);
        check(msg, worst < 1e-12);
    }
    for (a = 0; a < 24; a++) {
        for (b = 0; b < 24; b++) {
            gram[a][b] = dot4(g48.el[cat_index[a]], g48.el[cat_index[b]]);
        }
    }
    n_frames = count_frames();
    printf("  orthonormal 4-frames among the 24 classes: %d\n", n_frames);
    check("24 orthonormal 4-frames; the 6 cosets of Q8 are 6 of them and "
        "partition the classes", n_frames == 24 && q8_cosets() == 6);
    first_robust[0] = -1;
    for (a = 0; a < 24; a++) {
        for (b = a + 1; b < 24; b++) {
            for (c = b + 1; c < 24; c++) {
                SumCell s[27];
                int     t;
                int     tied = 0;
                int     v_float = float_xor6_voronoi(a, b, c);
                int     v_first;
                int     v_robust;
                int     v_possible;
                int     n_distinct;
                int     n_zero;
                int     clash = vector_clash(a, b, c, &n_distinct, &n_zero);
                int     is_orth = orth[a][b] && orth[a][c] && orth[b][c];
                int     is_frame = coset_of[a] >= 0 && coset_of[a] == coset_of[b]
                    && coset_of[a] == coset_of[c];
                int     pair_tie = 0;
                int     idx[3];

                idx[0] = a;
                idx[1] = b;
                idx[2] = c;
                cells_of_triple(a, b, c, s);
                for (t = 0; t < 27; t++) {
                    int x0 = t / 9 - 1;
                    int x1 = (t / 3) % 3 - 1;
                    int x2 = t % 3 - 1;

                    if (s[t].n_ties > 1) {
                        tied = 1;
                    }
                    if (s[t].n_ties > max_ties) {
                        max_ties = s[t].n_ties;
                    }
                    if (s[t].n_ties == 3 || s[t].n_ties > 4) {
                        bad_tie_size++;
                    }
                    /* pair pattern: exactly two nonzero x; tie set holds
                     * both of its own members */
                    if ((x0 != 0) + (x1 != 0) + (x2 != 0) == 2) {
                        int own[2];
                        int n_own = 0;
                        int hits = 0;
                        int k;
                        int j;

                        if (x0 != 0) own[n_own++] = idx[0];
                        if (x1 != 0) own[n_own++] = idx[1];
                        if (x2 != 0) own[n_own++] = idx[2];
                        for (j = 0; j < 2; j++) {
                            for (k = 0; k < s[t].n_ties; k++) {
                                hits += s[t].ties[k] == own[j];
                            }
                        }
                        if (hits == 2) {
                            pair_tie = 1;
                        }
                    }
                }
                n_clash += clash;
                n_zero_patterns += n_zero;
                if (n_distinct < min_distinct) min_distinct = n_distinct;
                if (n_distinct > max_distinct) max_distinct = n_distinct;
                v_first = verdict_first_index(s);
                v_robust = verdict_robust(s);
                v_possible = verdict_possible(s, clash, &n_undecided);
                n_orth += is_orth;
                n_in_frame += is_frame;
                n_pair_tie += pair_tie;
                bad_possible += v_possible != is_orth;
                bad_robust += v_robust != is_frame;
                bad_pair_tie += pair_tie != !is_orth
                    || (pair_tie && (v_first || v_robust || v_possible));
                d66_vs_float += d66_winner(a, b, c) != v_float;
                d66_vs_first += d66_winner(a, b, c) != v_first;
                if (v_robust && first_robust[0] < 0) {
                    first_robust[0] = a;
                    first_robust[1] = b;
                    first_robust[2] = c;
                }
                v_robust_of[a][b][c] = (unsigned char)v_robust;
                v_first_of[a][b][c] = (unsigned char)v_first;
                v_possible_of[a][b][c] = (unsigned char)v_possible;
                n_sets++;
                n_float += v_float;
                n_first += v_first;
                n_robust += v_robust;
                n_possible += v_possible;
                n_tied_sets += tied;
                n_float_not_robust += v_float && !v_robust;
                n_float_vs_first += v_float != v_first;
                n_float_outside_possible += v_float && !v_possible;
                if (v_float && !v_robust) {
                    printf("  D66 winner [%d,%d,%d] is NOT robust: another "
                        "tie rule fails it\n", a, b, c);
                }
            }
        }
    }
    printf("  antipodal triples: %d\n", n_sets);
    printf("  float replica (D66's code):      %4d pass\n", n_float);
    printf("  exact, ties to lowest index:     %4d pass\n", n_first);
    printf("  exact, robust (every tie rule):  %4d pass\n", n_robust);
    printf("  exact, possible (some tie rule): %4d pass\n", n_possible);
    printf("  sets with a tied sum: %d of %d; largest tie: %d cells\n",
        n_tied_sets, n_sets, max_ties);
    printf("  float passes that are not robust: %d; float != exact "
        "first-index: %d\n", n_float_not_robust, n_float_vs_first);
    check("2,024 antipodal triples (C(24,3))", n_sets == 2024);
    sprintf(msg, "float replica reproduces D66's printed 35 (got %d)",
        n_float);
    check(msg, n_float == 35);
    check("no set left undecided by the tie search", n_undecided == 0);
    check("robust <= first-index <= possible and robust <= possible",
        n_robust <= n_first && n_first <= n_possible);
    check("every float pass is possible under some exact tie rule",
        n_float_outside_possible == 0);
    /* constants as computed by oracle.py (2026-10-08; independent exact
     * Q(sqrt 2) arithmetic, its own 'possible' search) and by the
     * recensio's vector-level oracle - NOT re-run here */
    check("= oracle.py's counts: first-index 35, robust 24, possible 96",
        n_first == 35 && n_robust == 24 && n_possible == 96);
    check("= oracle.py's count: every one of the 2,024 sets has a tied sum",
        n_tied_sets == 2024);
    printf("  sum vectors per set: %d-%d distinct of 27 patterns; sets where "
        "one vector has both parities: %d; nonzero patterns summing to 0: "
        "%d\n", min_distinct, max_distinct, n_clash, n_zero_patterns);
    check("32 sets reach one vector with both parities (19-27 distinct "
        "sums) - all fail", n_clash == 32 && min_distinct == 19
        && max_distinct == 27);
    printf("  orthogonal triples %d, inside a Q8 coset %d, with an own-pair "
        "tie %d\n", n_orth, n_in_frame, n_pair_tie);
    check("possible <=> mutually orthogonal, on every set (96)",
        bad_possible == 0 && n_orth == 96);
    check("robust <=> inside one coset of Q8, on every set (24 = 6 x "
        "C(4,3)); the other 18 frames give none", bad_robust == 0
        && n_in_frame == 24);
    check("own-pair tie <=> not orthogonal (1,928 sets), and such a set "
        "fails under every rule", bad_pair_tie == 0 && n_pair_tie == 1928);
    check("tie sizes are 1, 2 or 4 only", bad_tie_size == 0);
    check("D66's ACTUAL 35 winners == float replica == exact first-index, "
        "set by set", d66_vs_float == 0 && d66_vs_first == 0);
    check("11 of D66's 35 depend on the tie rule", n_float_not_robust == 11);
    {
        int br = symmetry_breaks(v_robust_of);
        int bp = symmetry_breaks(v_possible_of);
        int bf = symmetry_breaks(v_first_of);

        printf("  symmetry maps (left/right by 48 elements) breaking the "
            "verdict: robust %d, possible %d, first-index %d (of 96)\n",
            br, bp, bf);
        check("robust and possible verdicts are invariant under all 96 "
            "symmetry maps", br == 0 && bp == 0);
        check("the 96 maps are 47 distinct permutations (D66 printed 47)",
            distinct_perms() == 47);
        /* left or right by +-1 fixes every class: 4 maps act trivially */
        check("lowest-index tie rule (= D66's floats) breaks every one of "
            "the 92 non-trivial maps", bf == 92);
        if (first_robust[0] >= 0) {
            int o = orbit_size(first_robust[0], first_robust[1],
                first_robust[2]);

            printf("  orbit of robust [%d,%d,%d] under the 96 maps: %d "
                "triples\n", first_robust[0], first_robust[1],
                first_robust[2], o);
            check("the 24 robust triples are ONE orbit", o == 24
                && in_orbit[4][5][15]);
        }
    }
}

/* ================================================================
 * Part D: D94's binary icosahedral group over Q(sqrt 5)
 * ================================================================ */

static Group g120;

static void
part_d (void)
{
    Extensio *q5 = extensio_quadratica(5, pool);
    Quaternio gens[2];
    int       hist[64];
    int       largest;
    int       axis_rep[MAX_AXES];
    int       axis_count[MAX_AXES];
    int       by_fold[16];
    int       n_axes;
    int       a;
    int       classes = 0;
    char      msg[256];

    printf("\n=== Part D: D94's 2I over Q(sqrt 5) ===\n");
    check("Q(sqrt 5) built, ordered", q5 != NULL && extensio_ordinata(q5));
    /* D94: s = (1 + i + j + k)/2, t = (phi + phi^-1 i + j)/2 with phi =
     * (1 + sqrt 5)/2 */
    gens[0] = qparse(extensio_anulus(q5), "[1/2, 1/2, 1/2, 1/2]");
    gens[1] = qparse(extensio_anulus(q5),
        "[(a + 1)/4, (a - 1)/4, 1/2, 0]");
    if (!group_close(&g120, extensio_anulus(q5), gens, 2)) {
        check("closure of <s, t> is finite (<= 120)", 0);
        return;
    }
    sprintf(msg, "closure has exactly 120 elements (got %d)", g120.n);
    check(msg, g120.n == 120);
    largest = order_histogram(&g120, hist, 63);
    check("orders 1^1 2^1 3^20 4^30 5^24 6^20 10^24 (2I)",
        hist[1] == 1 && hist[2] == 1 && hist[3] == 20 && hist[4] == 30
        && hist[5] == 24 && hist[6] == 20 && hist[10] == 24);
    check("largest order 10 -> the group IS 2I (classification)",
        largest == 10);
    for (a = 0; a < g120.n; a++) {
        classes += a < g120.neg[a];
    }
    sprintf(msg, "60 classes mod sign (got %d) - D94 printed 60", classes);
    check(msg, classes == 60);
    n_axes = group_axes(&g120, axis_rep, axis_count);
    sprintf(msg, "2I has exactly 31 rotation axes (got %d) - D94 printed "
        "31 directions", n_axes);
    check(msg, n_axes == 31);
    fold_census(n_axes, axis_count, by_fold, 15);
    printf("  folds: 5-fold %d, 3-fold %d, 2-fold %d\n", by_fold[5],
        by_fold[3], by_fold[2]);
    check("6 five-fold + 10 three-fold + 15 two-fold (icosahedron)",
        by_fold[5] == 6 && by_fold[3] == 10 && by_fold[2] == 15);
}

int
main (void)
{
    pool = piscina_generare_dynamicum("demo_118", 1 << 22);
    printf("KNOTAPEL DEMO 118: Exact Quaternions\n");
    printf("====================================\n");
    part_a();
    part_b();
    part_c();
    part_d();
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pool);
    return n_fail == 0 ? 0 : 1;
}
