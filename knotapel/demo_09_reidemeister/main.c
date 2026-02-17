/*
 * KNOTAPEL DEMO 09: Reidemeister Move Invariance
 * ================================================
 *
 * HYPOTHESIS: The Kauffman bracket satisfies specific algebraic
 * relations under Reidemeister moves, and the Jones polynomial
 * (f-polynomial) is a true knot invariant, unchanged by all
 * three Reidemeister moves.
 *
 * Key relations:
 *   R1: bracket(K + kink_s) = -A^{3s} * bracket(K)
 *       writhe changes by s, Jones is invariant
 *   R2: bracket unchanged, writhe unchanged, Jones unchanged
 *   R3: bracket unchanged, writhe unchanged, Jones unchanged
 *
 * R1 and R2 are implemented at the PD level (apply_r1, apply_r2).
 * R3 is tested via the braid relation sigma_1*sigma_2*sigma_1
 * = sigma_2*sigma_1*sigma_2, which corresponds to R3.
 *
 * PLAN:
 *   Part A: R1 bracket relation (6 tests)
 *   Part B: R1 writhe and Jones invariance (6 tests)
 *   Part C: R2 bracket, writhe, and Jones invariance (11 tests)
 *   Part D: R3 via braid relation (4 tests)
 *   Part E: Combined move invariance (3 tests)
 *
 * C89 compliant, zero dependencies.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Laurent polynomial ring (from Demo 08)
 * ================================================================ */
#define MAX_TERMS 64

typedef struct { int c[MAX_TERMS]; int lo, len; } Poly;

static void p_zero(Poly *p) { memset(p, 0, sizeof(Poly)); }

static void p_mono(Poly *p, int coeff, int exp) {
    p_zero(p);
    if (!coeff) return;
    p->lo = exp; p->len = 1; p->c[0] = coeff;
}

static void p_trim(Poly *p) {
    int a = 0, b;
    if (!p->len) return;
    while (a < p->len && !p->c[a]) a++;
    if (a == p->len) { p_zero(p); return; }
    b = p->len - 1;
    while (b > a && !p->c[b]) b--;
    if (a > 0) memmove(p->c, p->c + a, (size_t)(b - a + 1) * sizeof(int));
    p->lo += a; p->len = b - a + 1;
}

static void p_add(Poly *r, const Poly *a, const Poly *b) {
    Poly t; int lo, hi, i;
    if (!a->len) { *r = *b; return; }
    if (!b->len) { *r = *a; return; }
    lo = a->lo < b->lo ? a->lo : b->lo;
    hi = (a->lo + a->len - 1) > (b->lo + b->len - 1)
       ? (a->lo + a->len - 1) : (b->lo + b->len - 1);
    p_zero(&t); t.lo = lo; t.len = hi - lo + 1;
    for (i = 0; i < a->len; i++) t.c[(a->lo + i) - lo] += a->c[i];
    for (i = 0; i < b->len; i++) t.c[(b->lo + i) - lo] += b->c[i];
    p_trim(&t); *r = t;
}

static void p_mul(Poly *r, const Poly *a, const Poly *b) {
    Poly t; int i, j;
    if (!a->len || !b->len) { p_zero(r); return; }
    p_zero(&t); t.lo = a->lo + b->lo; t.len = a->len + b->len - 1;
    for (i = 0; i < a->len; i++)
        for (j = 0; j < b->len; j++)
            t.c[i + j] += a->c[i] * b->c[j];
    p_trim(&t); *r = t;
}

static int p_eq(const Poly *a, const Poly *b) {
    if (a->len != b->len) return 0;
    if (!a->len) return 1;
    if (a->lo != b->lo) return 0;
    return memcmp(a->c, b->c, (size_t)a->len * sizeof(int)) == 0;
}

static void p_print(const Poly *p, const char *label) {
    int i, e, first;
    printf("%s = ", label);
    if (!p->len) { printf("0\n"); return; }
    first = 1;
    for (i = 0; i < p->len; i++) {
        if (!p->c[i]) continue;
        e = p->lo + i;
        if (!first && p->c[i] > 0) printf(" + ");
        if (!first && p->c[i] < 0) printf(" - ");
        if (first && p->c[i] < 0) printf("-");
        first = 0;
        if (abs(p->c[i]) != 1 || e == 0) printf("%d", abs(p->c[i]));
        if (e == 1) printf("A");
        else if (e == -1) printf("A^-1");
        else if (e) printf("A^%d", e);
    }
    printf("\n");
}

/* ================================================================
 * PD notation and state-sum Kauffman bracket (from Demo 08)
 * ================================================================ */
#define MAX_X 20
#define MAX_ARCS 80

typedef struct { int arcs[4]; int sign; } Xing;
typedef struct { Xing x[MAX_X]; int n; int num_arcs; } Knot;

static int count_loops(const Knot *k, unsigned int state) {
    int app_x[MAX_ARCS][2], app_p[MAX_ARCS][2], app_n[MAX_ARCS];
    int pair[MAX_X][4], visited[MAX_ARCS];
    int i, j, loops, cur_arc, cur_side;
    memset(app_n, 0, sizeof(app_n));
    for (i = 0; i < k->n; i++)
        for (j = 0; j < 4; j++) {
            int arc = k->x[i].arcs[j];
            if (app_n[arc] < 2) {
                app_x[arc][app_n[arc]] = i;
                app_p[arc][app_n[arc]] = j;
            }
            app_n[arc]++;
        }
    for (i = 0; i < k->n; i++) {
        if ((state >> i) & 1) {
            pair[i][0] = 1; pair[i][1] = 0;
            pair[i][2] = 3; pair[i][3] = 2;
        } else {
            pair[i][0] = 3; pair[i][3] = 0;
            pair[i][1] = 2; pair[i][2] = 1;
        }
    }
    memset(visited, 0, sizeof(visited));
    loops = 0;
    for (i = 0; i < k->num_arcs; i++) {
        if (visited[i]) continue;
        cur_arc = i; cur_side = 0;
        while (!visited[cur_arc]) {
            int cx, pos, partner_pos, partner_arc, other_side;
            visited[cur_arc] = 1;
            cx = app_x[cur_arc][cur_side];
            pos = app_p[cur_arc][cur_side];
            partner_pos = pair[cx][pos];
            partner_arc = k->x[cx].arcs[partner_pos];
            if (app_x[partner_arc][0] == cx && app_p[partner_arc][0] == partner_pos)
                other_side = 1;
            else
                other_side = 0;
            cur_arc = partner_arc;
            cur_side = other_side;
        }
        loops++;
    }
    return loops;
}

static void pd_bracket(Poly *result, const Knot *k) {
    unsigned int state, ns;
    int i, ac, bc, lp, j;
    Poly d, dp, term, contrib, t1, t2;
    p_zero(result);
    if (k->n == 0) { p_mono(result, 1, 0); return; }
    p_mono(&t1, -1, 2); p_mono(&t2, -1, -2); p_add(&d, &t1, &t2);
    ns = 1u << k->n;
    for (state = 0; state < ns; state++) {
        ac = 0; bc = 0;
        for (i = 0; i < k->n; i++) {
            if ((state >> i) & 1) bc++; else ac++;
        }
        lp = count_loops(k, state);
        p_mono(&term, 1, ac - bc);
        p_mono(&dp, 1, 0);
        for (j = 0; j < lp - 1; j++) p_mul(&dp, &dp, &d);
        p_mul(&contrib, &term, &dp);
        p_add(result, result, &contrib);
    }
    p_trim(result);
}

/* ================================================================
 * Braid type and braid closure -> PD notation (from Demo 07/08)
 * ================================================================ */
#define MAX_WORD 20
#define MAX_STRANDS 8

typedef struct { int word[MAX_WORD]; int len, n; } Braid;

static void braid_to_pd(const Braid *b, Knot *k) {
    int xing_list[MAX_STRANDS][MAX_WORD];
    int xing_side[MAX_STRANDS][MAX_WORD];
    int xing_count[MAX_STRANDS];
    int in_arc[MAX_X][2], out_arc[MAX_X][2];
    int arc_id, i, j, m, n;

    m = b->len; n = b->n;
    memset(xing_count, 0, sizeof(xing_count));

    for (i = 0; i < m; i++) {
        int gen = b->word[i];
        int left = (gen > 0 ? gen : -gen) - 1;
        int right = left + 1;
        xing_list[left][xing_count[left]] = i;
        xing_side[left][xing_count[left]] = 0;
        xing_count[left]++;
        xing_list[right][xing_count[right]] = i;
        xing_side[right][xing_count[right]] = 1;
        xing_count[right]++;
    }

    arc_id = 0;
    memset(in_arc, -1, sizeof(in_arc));
    memset(out_arc, -1, sizeof(out_arc));

    for (j = 0; j < n; j++) {
        int cnt = xing_count[j];
        if (cnt == 0) continue;
        for (i = 0; i < cnt; i++) {
            int cur_xing = xing_list[j][i];
            int cur_side = xing_side[j][i];
            int next_xing = xing_list[j][(i + 1) % cnt];
            int next_side = xing_side[j][(i + 1) % cnt];
            out_arc[cur_xing][cur_side] = arc_id;
            in_arc[next_xing][next_side] = arc_id;
            arc_id++;
        }
    }

    k->n = m; k->num_arcs = arc_id;

    for (i = 0; i < m; i++) {
        int gen = b->word[i];
        int sign = gen > 0 ? 1 : -1;
        int il = in_arc[i][0], ir = in_arc[i][1];
        int ol = out_arc[i][0], or_ = out_arc[i][1];

        if (sign > 0) {
            k->x[i].arcs[0] = ir;  k->x[i].arcs[1] = or_;
            k->x[i].arcs[2] = ol;  k->x[i].arcs[3] = il;
        } else {
            k->x[i].arcs[0] = il;  k->x[i].arcs[1] = ir;
            k->x[i].arcs[2] = or_; k->x[i].arcs[3] = ol;
        }
        k->x[i].sign = sign;
    }
}

/* ================================================================
 * Known PD knots (from Demo 08)
 * ================================================================ */
static void make_trefoil_pd(Knot *k) {
    k->n = 3; k->num_arcs = 6;
    k->x[0].arcs[0]=0; k->x[0].arcs[1]=4; k->x[0].arcs[2]=1; k->x[0].arcs[3]=3; k->x[0].sign=1;
    k->x[1].arcs[0]=2; k->x[1].arcs[1]=0; k->x[1].arcs[2]=3; k->x[1].arcs[3]=5; k->x[1].sign=1;
    k->x[2].arcs[0]=4; k->x[2].arcs[1]=2; k->x[2].arcs[2]=5; k->x[2].arcs[3]=1; k->x[2].sign=1;
}

static void make_figure_eight_pd(Knot *k) {
    k->n = 4; k->num_arcs = 8;
    k->x[0].arcs[0]=3; k->x[0].arcs[1]=1; k->x[0].arcs[2]=4; k->x[0].arcs[3]=0; k->x[0].sign=1;
    k->x[1].arcs[0]=7; k->x[1].arcs[1]=5; k->x[1].arcs[2]=0; k->x[1].arcs[3]=4; k->x[1].sign=1;
    k->x[2].arcs[0]=5; k->x[2].arcs[1]=2; k->x[2].arcs[2]=6; k->x[2].arcs[3]=3; k->x[2].sign=-1;
    k->x[3].arcs[0]=1; k->x[3].arcs[1]=6; k->x[3].arcs[2]=2; k->x[3].arcs[3]=7; k->x[3].sign=-1;
}

static void make_hopf_pd(Knot *k) {
    k->n = 2; k->num_arcs = 4;
    k->x[0].arcs[0]=3; k->x[0].arcs[1]=0; k->x[0].arcs[2]=2; k->x[0].arcs[3]=1; k->x[0].sign=1;
    k->x[1].arcs[0]=1; k->x[1].arcs[1]=2; k->x[1].arcs[2]=0; k->x[1].arcs[3]=3; k->x[1].sign=1;
}

/* ================================================================
 * NEW: Writhe and Jones polynomial
 *
 * writhe(K) = sum of crossing signs
 * Jones: f(K) = (-A^3)^{-w} * bracket(K)
 * ================================================================ */
static int writhe(const Knot *k) {
    int i, w = 0;
    for (i = 0; i < k->n; i++) w += k->x[i].sign;
    return w;
}

static void jones_poly(Poly *jones, const Knot *k) {
    Poly bracket, norm;
    int w;
    pd_bracket(&bracket, k);
    w = writhe(k);
    p_mono(&norm, (w % 2 == 0) ? 1 : -1, -3 * w);
    p_mul(jones, &norm, &bracket);
}

/* ================================================================
 * NEW: R1 -- Add a kink (twist) to an edge
 *
 * Splits edge into a1 (first occurrence) and a2 (second occurrence),
 * creates loop arc L.
 *
 * Positive kink: [a1, L, L, a2], sign = +1
 * Negative kink: [L, L, a1, a2], sign = -1
 *
 * Bracket: bracket(K') = -A^{3s} * bracket(K)
 * Writhe:  writhe(K') = writhe(K) + s
 * ================================================================ */

/* Replace second occurrence of old_arc with new_arc */
static void remap_second(Knot *k, int old_arc, int new_arc) {
    int i, j, count = 0;
    for (i = 0; i < k->n; i++)
        for (j = 0; j < 4; j++)
            if (k->x[i].arcs[j] == old_arc) {
                count++;
                if (count == 2) {
                    k->x[i].arcs[j] = new_arc;
                    return;
                }
            }
}

static void apply_r1(const Knot *k, int edge, int positive, Knot *out) {
    int a2, L;
    *out = *k;
    a2 = k->num_arcs;
    L  = k->num_arcs + 1;
    out->num_arcs = k->num_arcs + 2;

    remap_second(out, edge, a2);

    if (positive) {
        out->x[out->n].arcs[0] = edge;
        out->x[out->n].arcs[1] = L;
        out->x[out->n].arcs[2] = L;
        out->x[out->n].arcs[3] = a2;
        out->x[out->n].sign = 1;
    } else {
        out->x[out->n].arcs[0] = L;
        out->x[out->n].arcs[1] = L;
        out->x[out->n].arcs[2] = edge;
        out->x[out->n].arcs[3] = a2;
        out->x[out->n].sign = -1;
    }
    out->n++;
}

/* ================================================================
 * NEW: R2 -- Add a poke (pair of opposite crossings)
 *
 * Splits edge_a into (edge_a, a2) and edge_b into (edge_b, b2).
 * Creates intermediate arcs m1, m2.
 *
 * C_pos (+1): [edge_a, m2, m1, edge_b]
 * C_neg (-1): [m1, m2, a2, b2]
 *
 * Bracket: unchanged
 * Writhe:  unchanged (+1 and -1 cancel)
 * ================================================================ */
static void apply_r2(const Knot *k, int edge_a, int edge_b, Knot *out) {
    int a2, b2, m1, m2;
    *out = *k;
    a2 = k->num_arcs;
    b2 = k->num_arcs + 1;
    m1 = k->num_arcs + 2;
    m2 = k->num_arcs + 3;
    out->num_arcs = k->num_arcs + 4;

    remap_second(out, edge_a, a2);
    remap_second(out, edge_b, b2);

    /* Positive crossing */
    out->x[out->n].arcs[0] = edge_a;
    out->x[out->n].arcs[1] = m2;
    out->x[out->n].arcs[2] = m1;
    out->x[out->n].arcs[3] = edge_b;
    out->x[out->n].sign = 1;
    out->n++;

    /* Negative crossing */
    out->x[out->n].arcs[0] = m1;
    out->x[out->n].arcs[1] = m2;
    out->x[out->n].arcs[2] = a2;
    out->x[out->n].arcs[3] = b2;
    out->x[out->n].sign = -1;
    out->n++;
}

/* R2-poked unknot (manually constructed, unknot has no edges) */
static void make_r2_unknot(Knot *k) {
    k->n = 2; k->num_arcs = 4;
    /* C1 (+1): single loop passes under then over */
    k->x[0].arcs[0] = 0; k->x[0].arcs[1] = 3;
    k->x[0].arcs[2] = 1; k->x[0].arcs[3] = 2;
    k->x[0].sign = 1;
    /* C2 (-1): same loop, reversed crossing */
    k->x[1].arcs[0] = 3; k->x[1].arcs[1] = 1;
    k->x[1].arcs[2] = 0; k->x[1].arcs[3] = 2;
    k->x[1].sign = -1;
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
 * PART A: R1 Bracket Relation (6 tests)
 *
 * bracket(K + kink_s) = -A^{3s} * bracket(K)
 * ================================================================ */
static void test_part_a(void) {
    Knot k, kinked;
    Poly br_orig, br_kink, factor, expected;

    printf("\n=== PART A: R1 Bracket Relation ===\n");
    printf("    bracket(K + kink_s) = -A^{3s} * bracket(K)\n\n");

    /* Unknot + positive kink: bracket = -A^3 */
    {
        Knot uk;
        Poly br;
        uk.n = 1; uk.num_arcs = 2;
        uk.x[0].arcs[0] = 0; uk.x[0].arcs[1] = 1;
        uk.x[0].arcs[2] = 1; uk.x[0].arcs[3] = 0;
        uk.x[0].sign = 1;
        pd_bracket(&br, &uk);
        p_mono(&expected, -1, 3);
        p_print(&br, "    <unknot+pos_kink>");
        check("unknot + pos kink: bracket = -A^3", p_eq(&br, &expected));
    }

    /* Unknot + negative kink: bracket = -A^{-3} */
    {
        Knot uk;
        Poly br;
        uk.n = 1; uk.num_arcs = 2;
        uk.x[0].arcs[0] = 0; uk.x[0].arcs[1] = 0;
        uk.x[0].arcs[2] = 1; uk.x[0].arcs[3] = 1;
        uk.x[0].sign = -1;
        pd_bracket(&br, &uk);
        p_mono(&expected, -1, -3);
        p_print(&br, "    <unknot+neg_kink>");
        check("unknot + neg kink: bracket = -A^{-3}", p_eq(&br, &expected));
    }

    /* Trefoil + positive kink */
    make_trefoil_pd(&k);
    pd_bracket(&br_orig, &k);
    apply_r1(&k, 0, 1, &kinked);
    pd_bracket(&br_kink, &kinked);
    p_mono(&factor, -1, 3);
    p_mul(&expected, &factor, &br_orig);
    check("trefoil + pos kink: bracket = -A^3 * <trefoil>",
          p_eq(&br_kink, &expected));

    /* Trefoil + negative kink */
    apply_r1(&k, 0, 0, &kinked);
    pd_bracket(&br_kink, &kinked);
    p_mono(&factor, -1, -3);
    p_mul(&expected, &factor, &br_orig);
    check("trefoil + neg kink: bracket = -A^{-3} * <trefoil>",
          p_eq(&br_kink, &expected));

    /* Hopf + positive kink */
    make_hopf_pd(&k);
    pd_bracket(&br_orig, &k);
    apply_r1(&k, 0, 1, &kinked);
    pd_bracket(&br_kink, &kinked);
    p_mono(&factor, -1, 3);
    p_mul(&expected, &factor, &br_orig);
    check("Hopf + pos kink: bracket = -A^3 * <Hopf>",
          p_eq(&br_kink, &expected));

    /* Hopf + negative kink */
    apply_r1(&k, 0, 0, &kinked);
    pd_bracket(&br_kink, &kinked);
    p_mono(&factor, -1, -3);
    p_mul(&expected, &factor, &br_orig);
    check("Hopf + neg kink: bracket = -A^{-3} * <Hopf>",
          p_eq(&br_kink, &expected));
}

/* ================================================================
 * PART B: R1 Writhe and Jones (6 tests)
 *
 * writhe(K + kink_s) = writhe(K) + s
 * Jones(K + kink_s) = Jones(K)
 * ================================================================ */
static void test_part_b(void) {
    Knot k, kinked;
    Poly jones_orig, jones_kink;
    int w_orig;

    printf("\n=== PART B: R1 Writhe and Jones ===\n");

    /* Trefoil */
    make_trefoil_pd(&k);
    w_orig = writhe(&k);
    jones_poly(&jones_orig, &k);
    p_print(&jones_orig, "    Jones(trefoil)");

    apply_r1(&k, 0, 1, &kinked);
    check("trefoil + pos kink: writhe = w + 1",
          writhe(&kinked) == w_orig + 1);
    jones_poly(&jones_kink, &kinked);
    check("trefoil + pos kink: Jones invariant",
          p_eq(&jones_kink, &jones_orig));

    apply_r1(&k, 0, 0, &kinked);
    check("trefoil + neg kink: writhe = w - 1",
          writhe(&kinked) == w_orig - 1);
    jones_poly(&jones_kink, &kinked);
    check("trefoil + neg kink: Jones invariant",
          p_eq(&jones_kink, &jones_orig));

    /* Figure-eight */
    make_figure_eight_pd(&k);
    jones_poly(&jones_orig, &k);

    apply_r1(&k, 0, 1, &kinked);
    jones_poly(&jones_kink, &kinked);
    check("fig-8 + pos kink: Jones invariant",
          p_eq(&jones_kink, &jones_orig));

    apply_r1(&k, 0, 0, &kinked);
    jones_poly(&jones_kink, &kinked);
    check("fig-8 + neg kink: Jones invariant",
          p_eq(&jones_kink, &jones_orig));
}

/* ================================================================
 * PART C: R2 Bracket, Writhe, and Jones Invariance (11 tests)
 *
 * All three quantities are unchanged under R2.
 * ================================================================ */
static void test_part_c(void) {
    Knot k, poked;
    Poly br_orig, br_poked, jones_orig, jones_poked;

    printf("\n=== PART C: R2 Invariance ===\n");

    /* R2-poked unknot */
    {
        Knot r2_uk;
        Poly br, j, one;
        make_r2_unknot(&r2_uk);
        pd_bracket(&br, &r2_uk);
        p_mono(&one, 1, 0);
        p_print(&br, "    <R2 unknot>");
        check("R2 unknot: bracket = 1", p_eq(&br, &one));
        jones_poly(&j, &r2_uk);
        check("R2 unknot: Jones = 1", p_eq(&j, &one));
    }

    /* Trefoil + R2 (poke on edges 0, 2) */
    make_trefoil_pd(&k);
    pd_bracket(&br_orig, &k);
    jones_poly(&jones_orig, &k);
    apply_r2(&k, 0, 2, &poked);
    pd_bracket(&br_poked, &poked);
    check("trefoil + R2: bracket unchanged", p_eq(&br_poked, &br_orig));
    check("trefoil + R2: writhe unchanged",
          writhe(&poked) == writhe(&k));
    jones_poly(&jones_poked, &poked);
    check("trefoil + R2: Jones unchanged",
          p_eq(&jones_poked, &jones_orig));

    /* Figure-eight + R2 */
    make_figure_eight_pd(&k);
    pd_bracket(&br_orig, &k);
    jones_poly(&jones_orig, &k);
    apply_r2(&k, 0, 2, &poked);
    pd_bracket(&br_poked, &poked);
    check("fig-8 + R2: bracket unchanged", p_eq(&br_poked, &br_orig));
    jones_poly(&jones_poked, &poked);
    check("fig-8 + R2: Jones unchanged",
          p_eq(&jones_poked, &jones_orig));

    /* Hopf + R2 */
    make_hopf_pd(&k);
    pd_bracket(&br_orig, &k);
    jones_poly(&jones_orig, &k);
    apply_r2(&k, 0, 2, &poked);
    pd_bracket(&br_poked, &poked);
    check("Hopf + R2: bracket unchanged", p_eq(&br_poked, &br_orig));
    jones_poly(&jones_poked, &poked);
    check("Hopf + R2: Jones unchanged",
          p_eq(&jones_poked, &jones_orig));
}

/* ================================================================
 * PART D: R3 via Braid Relation (4 tests)
 *
 * sigma_1 * sigma_2 * sigma_1 = sigma_2 * sigma_1 * sigma_2
 * corresponds to the R3 (slide) move at the PD level.
 * ================================================================ */
static void test_part_d(void) {
    Braid b1, b2;
    Knot k1, k2;
    Poly br1, br2, j1, j2;

    printf("\n=== PART D: R3 via Braid Relation ===\n");
    printf("    s1*s2*s1 = s2*s1*s2\n\n");

    /* Positive: s1 s2 s1 vs s2 s1 s2 on 3 strands */
    b1.n = 3; b1.len = 3;
    b1.word[0] = 1; b1.word[1] = 2; b1.word[2] = 1;
    b2.n = 3; b2.len = 3;
    b2.word[0] = 2; b2.word[1] = 1; b2.word[2] = 2;

    braid_to_pd(&b1, &k1);
    braid_to_pd(&b2, &k2);
    pd_bracket(&br1, &k1);
    pd_bracket(&br2, &k2);
    p_print(&br1, "    <s1s2s1>");
    p_print(&br2, "    <s2s1s2>");
    check("R3 (pos): s1*s2*s1 bracket == s2*s1*s2 bracket",
          p_eq(&br1, &br2));

    jones_poly(&j1, &k1);
    jones_poly(&j2, &k2);
    check("R3 (pos): s1*s2*s1 Jones == s2*s1*s2 Jones",
          p_eq(&j1, &j2));

    /* Negative: s1^-1 s2^-1 s1^-1 vs s2^-1 s1^-1 s2^-1 */
    b1.word[0] = -1; b1.word[1] = -2; b1.word[2] = -1;
    b2.word[0] = -2; b2.word[1] = -1; b2.word[2] = -2;

    braid_to_pd(&b1, &k1);
    braid_to_pd(&b2, &k2);
    pd_bracket(&br1, &k1);
    pd_bracket(&br2, &k2);
    check("R3 (neg): s1^-1*s2^-1*s1^-1 == s2^-1*s1^-1*s2^-1",
          p_eq(&br1, &br2));

    /* R3 in longer word: s1 s2 s1 s3 vs s2 s1 s2 s3 on 4 strands */
    b1.n = 4; b1.len = 4;
    b1.word[0] = 1; b1.word[1] = 2; b1.word[2] = 1; b1.word[3] = 3;
    b2.n = 4; b2.len = 4;
    b2.word[0] = 2; b2.word[1] = 1; b2.word[2] = 2; b2.word[3] = 3;

    braid_to_pd(&b1, &k1);
    braid_to_pd(&b2, &k2);
    pd_bracket(&br1, &k1);
    pd_bracket(&br2, &k2);
    check("R3 in context: s1*s2*s1*s3 == s2*s1*s2*s3 (4 strands)",
          p_eq(&br1, &br2));
}

/* ================================================================
 * PART E: Combined Move Invariance (3 tests)
 *
 * Multiple Reidemeister moves applied sequentially.
 * ================================================================ */
static void test_part_e(void) {
    Knot k, k1, k2;
    Poly br_orig, br_mod, jones_orig, jones_mod;

    printf("\n=== PART E: Combined Move Invariance ===\n");

    /* R1(+) then R1(-): bracket returns to original
     * (-A^3)(-A^{-3}) = 1, so factors cancel */
    make_trefoil_pd(&k);
    pd_bracket(&br_orig, &k);
    apply_r1(&k, 0, 1, &k1);
    apply_r1(&k1, 1, 0, &k2);
    pd_bracket(&br_mod, &k2);
    check("trefoil + R1(+) + R1(-): bracket unchanged",
          p_eq(&br_mod, &br_orig));

    /* R1 then R2: Jones unchanged */
    make_trefoil_pd(&k);
    jones_poly(&jones_orig, &k);
    apply_r1(&k, 0, 1, &k1);
    apply_r2(&k1, 1, 3, &k2);
    jones_poly(&jones_mod, &k2);
    check("trefoil + R1 + R2: Jones unchanged",
          p_eq(&jones_mod, &jones_orig));

    /* Double R1(+): Jones unchanged */
    make_figure_eight_pd(&k);
    jones_poly(&jones_orig, &k);
    apply_r1(&k, 0, 1, &k1);
    apply_r1(&k1, 2, 1, &k2);
    jones_poly(&jones_mod, &k2);
    check("fig-8 + double R1(+): Jones unchanged",
          p_eq(&jones_mod, &jones_orig));
}

/* ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 09: Reidemeister Move Invariance\n");
    printf("================================================\n");

    test_part_a();
    test_part_b();
    test_part_c();
    test_part_d();
    test_part_e();

    printf("\n================================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("================================================\n");
    return n_fail > 0 ? 1 : 0;
}
