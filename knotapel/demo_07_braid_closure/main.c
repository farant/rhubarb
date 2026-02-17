/*
 * KNOTAPEL DEMO 07: Braid Closure Bridge
 * =======================================
 *
 * Demos 01-06 had two separate worlds:
 *   - Demo 01: PD notation -> state-sum bracket (O(2^n) oracle)
 *   - Demos 02-06: braid word -> matrix pipeline bracket (O(n))
 *
 * This demo builds the BRIDGE: a function that converts braid words
 * to PD notation via braid closure, proving both worlds compute the
 * same invariant.
 *
 * PLAN:
 *   Part A: braid_to_pd() construction and arc count verification
 *   Part B: Bracket match: PD bracket from generated PD == braid bracket
 *   Part C: Seifert circles from PD notation
 *   Part D: Link component count from braid closure
 *
 * C89 compliant, zero dependencies.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Laurent polynomial ring (reused from Demos 01-06)
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

static void p_print(const Poly *p, const char *name) {
    int i, e, first;
    printf("%s = ", name);
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
 * PD notation (from Demo 01)
 * ================================================================ */
#define MAX_X 20
#define MAX_ARCS 64

typedef struct {
    int arcs[4];
    int sign;
} Xing;

typedef struct {
    Xing x[MAX_X];
    int n;
    int num_arcs;
} Knot;

/* State-sum Kauffman bracket from PD notation (from Demo 01) */
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
 * Braid type and braid bracket (from Demo 06's state-sum approach)
 * ================================================================ */
#define MAX_WORD 20
#define MAX_STRANDS 8

typedef struct { int word[MAX_WORD]; int len, n; } Braid;

#define MAX_UF 256
static int uf_p[MAX_UF];
static void uf_init(int nn) { int i; for (i = 0; i < nn; i++) uf_p[i] = i; }
static int uf_find(int x) {
    while (uf_p[x] != x) { uf_p[x] = uf_p[uf_p[x]]; x = uf_p[x]; }
    return x;
}
static void uf_union(int x, int y) {
    x = uf_find(x); y = uf_find(y); if (x != y) uf_p[x] = y;
}

static int braid_loops(const Braid *b, unsigned s) {
    int N = (b->len + 1) * b->n, l, p, i, loops, sgn, bit, cup;
    uf_init(N);
    for (l = 0; l < b->len; l++) {
        sgn = b->word[l] > 0 ? 1 : -1;
        i = (sgn > 0 ? b->word[l] : -b->word[l]) - 1;
        bit = (int)((s >> l) & 1u);
        cup = (sgn > 0) ? (bit == 0) : (bit == 1);
        if (cup) {
            uf_union(l * b->n + i, l * b->n + i + 1);
            uf_union((l + 1) * b->n + i, (l + 1) * b->n + i + 1);
            for (p = 0; p < b->n; p++)
                if (p != i && p != i + 1)
                    uf_union(l * b->n + p, (l + 1) * b->n + p);
        } else {
            for (p = 0; p < b->n; p++)
                uf_union(l * b->n + p, (l + 1) * b->n + p);
        }
    }
    for (p = 0; p < b->n; p++)
        uf_union(p, b->len * b->n + p);
    loops = 0;
    for (i = 0; i < N; i++)
        if (uf_find(i) == i) loops++;
    return loops;
}

static void braid_bracket(Poly *r, const Braid *b) {
    unsigned s, ns; int i, a, bc, lp, j;
    Poly d, dp, t, c, t1, t2;
    p_zero(r);
    p_mono(&t1, -1, 2); p_mono(&t2, -1, -2); p_add(&d, &t1, &t2);
    if (!b->len) {
        p_mono(r, 1, 0);
        for (i = 0; i < b->n - 1; i++) p_mul(r, r, &d);
        return;
    }
    ns = 1u << b->len;
    for (s = 0; s < ns; s++) {
        a = 0; bc = 0;
        for (i = 0; i < b->len; i++) {
            if ((s >> i) & 1u) bc++; else a++;
        }
        lp = braid_loops(b, s);
        p_mono(&t, 1, a - bc);
        p_mono(&dp, 1, 0);
        for (j = 0; j < lp - 1; j++) p_mul(&dp, &dp, &d);
        p_mul(&c, &t, &dp);
        p_add(r, r, &c);
    }
    p_trim(r);
}

/* ================================================================
 * BRAID CLOSURE -> PD NOTATION
 *
 * The core bridge function. Given a braid word on n strands with
 * m crossings, produce the PD notation of the closed braid.
 *
 * Arc labeling scheme:
 * - For each position j (0..n-1), find the ordered list of crossings
 *   involving that position.
 * - The arc between consecutive crossings at position j gets a label.
 * - Closure connects the last crossing back to the first.
 *
 * For positive sigma_i crossing (left strand OVER right):
 *   PD = [in_right, in_left, out_left, out_right]
 *   (counter-clockwise from incoming under-strand at NE)
 *
 * For negative sigma_i^{-1} crossing (right strand OVER left):
 *   PD = [in_left, out_left, out_right, in_right]
 *   (counter-clockwise from incoming under-strand at NW)
 * ================================================================ */
static void braid_to_pd(const Braid *b, Knot *k) {
    /* For each position, build list of crossings involving it */
    int xing_list[MAX_STRANDS][MAX_WORD];
    int xing_side[MAX_STRANDS][MAX_WORD]; /* 0=left pos, 1=right pos */
    int xing_count[MAX_STRANDS];
    int in_arc[MAX_X][2];  /* in_arc[crossing][side] = incoming arc */
    int out_arc[MAX_X][2]; /* out_arc[crossing][side] = outgoing arc */
    int arc_id, i, j, m, n;

    m = b->len;
    n = b->n;
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

    /* Assign arc labels: arc between crossing k (out) and next crossing (in)
     * at the same position */
    arc_id = 0;
    memset(in_arc, -1, sizeof(in_arc));
    memset(out_arc, -1, sizeof(out_arc));

    for (j = 0; j < n; j++) {
        int cnt = xing_count[j];
        if (cnt == 0) continue; /* strand not involved in any crossing */
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

    /* Build PD crossings */
    k->n = m;
    k->num_arcs = arc_id;

    for (i = 0; i < m; i++) {
        int gen = b->word[i];
        int sign = gen > 0 ? 1 : -1;
        int il = in_arc[i][0];   /* incoming at left position */
        int ir = in_arc[i][1];   /* incoming at right position */
        int ol = out_arc[i][0];  /* outgoing at left position */
        int or_ = out_arc[i][1]; /* outgoing at right position */

        if (sign > 0) {
            /* Positive: left OVER right.
             * Under: NE(ir) -> SW(ol), Over: NW(il) -> SE(or_)
             * Labeling: [NE, SE, SW, NW] = [ir, or_, ol, il]
             * Under at positions 0,2. Over at positions 1,3. */
            k->x[i].arcs[0] = ir;
            k->x[i].arcs[1] = or_;
            k->x[i].arcs[2] = ol;
            k->x[i].arcs[3] = il;
        } else {
            /* Negative: right OVER left.
             * Under: NW(il) -> SE(or_), Over: NE(ir) -> SW(ol)
             * Labeling: [NW, NE, SE, SW] = [il, ir, or_, ol]
             * Under at positions 0,2. Over at positions 1,3. */
            k->x[i].arcs[0] = il;
            k->x[i].arcs[1] = ir;
            k->x[i].arcs[2] = or_;
            k->x[i].arcs[3] = ol;
        }
        k->x[i].sign = sign;
    }
}

/* ================================================================
 * SEIFERT CIRCLES from PD notation
 *
 * The Seifert resolution "uncrosses" each crossing while preserving
 * strand orientation, connecting arcs on the same geometric side.
 *
 * For our PD convention (positions 0,2 = under-strand, 1,3 = over):
 *   Positive crossing: B-smoothing — pairs (0,1) and (2,3)
 *   Negative crossing: A-smoothing — pairs (0,3) and (1,2)
 *
 * This gives Seifert circles = strand count for braid closures.
 * Also works for Demo 01's standard PD convention.
 * ================================================================ */
static int seifert_circles(const Knot *k) {
    int app_x[MAX_ARCS][2], app_p[MAX_ARCS][2], app_n[MAX_ARCS];
    int pair[MAX_X][4];
    int visited[MAX_ARCS];
    int i, j, circles, cur_arc, cur_side;

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

    /* Seifert pairing depends on crossing sign:
     * Positive: B-smoothing = (0,1) and (2,3)
     * Negative: A-smoothing = (0,3) and (1,2)
     * This connects arcs on the same geometric side (left/right). */
    for (i = 0; i < k->n; i++) {
        if (k->x[i].sign > 0) {
            pair[i][0] = 1; pair[i][1] = 0;
            pair[i][2] = 3; pair[i][3] = 2;
        } else {
            pair[i][0] = 3; pair[i][3] = 0;
            pair[i][1] = 2; pair[i][2] = 1;
        }
    }

    memset(visited, 0, sizeof(visited));
    circles = 0;
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
        circles++;
    }
    return circles;
}

/* Count link components by tracing strands through crossings.
 * At each crossing, arc at position p continues at (p+2)%4.
 * (Positions 0,2 are under-strand; 1,3 are over-strand.) */
static int link_components(const Knot *k) {
    /* For link components, we trace the actual knot.
     * At each crossing, entry on arc position p exits on (p+2)%4.
     * This follows the strand through the crossing. */
    int app_x[MAX_ARCS][2], app_p[MAX_ARCS][2], app_n[MAX_ARCS];
    int visited[MAX_ARCS];
    int i, j, comps, cur_arc, cur_side;

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

    memset(visited, 0, sizeof(visited));
    comps = 0;
    for (i = 0; i < k->num_arcs; i++) {
        if (visited[i]) continue;
        cur_arc = i; cur_side = 0;
        while (!visited[cur_arc]) {
            int cx, pos, exit_pos, next_arc, other_side;
            visited[cur_arc] = 1;
            cx = app_x[cur_arc][cur_side];
            pos = app_p[cur_arc][cur_side];
            exit_pos = (pos + 2) % 4;
            next_arc = k->x[cx].arcs[exit_pos];
            if (app_x[next_arc][0] == cx && app_p[next_arc][0] == exit_pos)
                other_side = 1;
            else
                other_side = 0;
            cur_arc = next_arc;
            cur_side = other_side;
        }
        comps++;
    }
    return comps;
}

/* ================================================================
 * Demo 01's known knots in PD notation (for comparison)
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
 * Test infrastructure
 * ================================================================ */
static int n_pass = 0, n_fail = 0;
static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * TESTS
 * ================================================================ */

static void test_part_a(void) {
    /*
     * Part A: braid_to_pd() construction.
     * Verify arc counts are correct (2*m for knots, may differ for links).
     */
    Braid braids[5];
    const char *names[5];
    int expected_arcs[5];
    int nb, i;
    Knot k;

    printf("\n=== PART A: Braid Closure -> PD Construction ===\n");

    nb = 0;

    /* Trefoil: sigma_1^3 on 2 strands */
    braids[nb].n = 2; braids[nb].len = 3;
    braids[nb].word[0] = 1; braids[nb].word[1] = 1; braids[nb].word[2] = 1;
    names[nb] = "trefoil (2-strand)";
    expected_arcs[nb] = 6;
    nb++;

    /* Figure-eight: sigma_1 sigma_2^{-1} sigma_1 sigma_2^{-1} on 3 strands */
    braids[nb].n = 3; braids[nb].len = 4;
    braids[nb].word[0] = 1; braids[nb].word[1] = -2;
    braids[nb].word[2] = 1; braids[nb].word[3] = -2;
    names[nb] = "figure-eight (3-strand)";
    expected_arcs[nb] = 8;
    nb++;

    /* Hopf link: sigma_1^2 on 2 strands */
    braids[nb].n = 2; braids[nb].len = 2;
    braids[nb].word[0] = 1; braids[nb].word[1] = 1;
    names[nb] = "Hopf link (2-strand)";
    expected_arcs[nb] = 4;
    nb++;

    /* Trefoil on 3 strands: sigma_1 sigma_2 sigma_1 */
    braids[nb].n = 3; braids[nb].len = 3;
    braids[nb].word[0] = 1; braids[nb].word[1] = 2; braids[nb].word[2] = 1;
    names[nb] = "trefoil (3-strand)";
    expected_arcs[nb] = 6;
    nb++;

    /* Unknot: sigma_1 sigma_1^{-1} on 2 strands */
    braids[nb].n = 2; braids[nb].len = 2;
    braids[nb].word[0] = 1; braids[nb].word[1] = -1;
    names[nb] = "unknot (sigma sigma^-1)";
    expected_arcs[nb] = 4;
    nb++;

    for (i = 0; i < nb; i++) {
        char msg[120];
        braid_to_pd(&braids[i], &k);
        sprintf(msg, "%s: %d crossings, %d arcs",
                names[i], k.n, k.num_arcs);
        printf("  %s\n", msg);
        sprintf(msg, "%s: arc count = %d", names[i], expected_arcs[i]);
        check(msg, k.num_arcs == expected_arcs[i]);
    }
}

static void test_part_b(void) {
    /*
     * Part B: Bracket verification.
     * braid -> PD -> state-sum bracket == braid -> braid bracket
     */
    Braid braids[5];
    const char *names[5];
    int nb, i;
    Knot k;
    Poly pd_br, braid_br;

    printf("\n=== PART B: Bracket Match (PD vs Braid) ===\n");

    nb = 0;

    /* Trefoil 2-strand */
    braids[nb].n = 2; braids[nb].len = 3;
    braids[nb].word[0] = 1; braids[nb].word[1] = 1; braids[nb].word[2] = 1;
    names[nb] = "trefoil (2-strand)";
    nb++;

    /* Figure-eight */
    braids[nb].n = 3; braids[nb].len = 4;
    braids[nb].word[0] = 1; braids[nb].word[1] = -2;
    braids[nb].word[2] = 1; braids[nb].word[3] = -2;
    names[nb] = "figure-eight";
    nb++;

    /* Hopf */
    braids[nb].n = 2; braids[nb].len = 2;
    braids[nb].word[0] = 1; braids[nb].word[1] = 1;
    names[nb] = "Hopf link";
    nb++;

    /* Trefoil 3-strand */
    braids[nb].n = 3; braids[nb].len = 3;
    braids[nb].word[0] = 1; braids[nb].word[1] = 2; braids[nb].word[2] = 1;
    names[nb] = "trefoil (3-strand)";
    nb++;

    /* Unknot */
    braids[nb].n = 2; braids[nb].len = 2;
    braids[nb].word[0] = 1; braids[nb].word[1] = -1;
    names[nb] = "unknot";
    nb++;

    for (i = 0; i < nb; i++) {
        char msg[120];

        /* Compute bracket from braid directly */
        braid_bracket(&braid_br, &braids[i]);

        /* Convert braid to PD, then compute bracket from PD */
        braid_to_pd(&braids[i], &k);
        pd_bracket(&pd_br, &k);

        p_print(&braid_br, "    braid bracket");
        p_print(&pd_br, "    PD bracket   ");

        sprintf(msg, "%s: PD bracket == braid bracket", names[i]);
        check(msg, p_eq(&pd_br, &braid_br));
    }
}

static void test_part_c(void) {
    /*
     * Part C: Seifert circles.
     * For braid closures, Seifert circle count should equal strand count.
     * For Demo 01's PD codes, verify known Seifert circle counts.
     */
    Braid braids[4];
    const char *bnames[4];
    int nb, i, sc;
    Knot k;
    char msg[120];

    printf("\n=== PART C: Seifert Circles ===\n");

    /* Test on braid-generated PD */
    nb = 0;

    braids[nb].n = 2; braids[nb].len = 3;
    braids[nb].word[0] = 1; braids[nb].word[1] = 1; braids[nb].word[2] = 1;
    bnames[nb] = "trefoil braid (2-strand)";
    nb++;

    braids[nb].n = 3; braids[nb].len = 4;
    braids[nb].word[0] = 1; braids[nb].word[1] = -2;
    braids[nb].word[2] = 1; braids[nb].word[3] = -2;
    bnames[nb] = "figure-eight braid (3-strand)";
    nb++;

    braids[nb].n = 2; braids[nb].len = 2;
    braids[nb].word[0] = 1; braids[nb].word[1] = 1;
    bnames[nb] = "Hopf braid (2-strand)";
    nb++;

    braids[nb].n = 3; braids[nb].len = 3;
    braids[nb].word[0] = 1; braids[nb].word[1] = 2; braids[nb].word[2] = 1;
    bnames[nb] = "trefoil braid (3-strand)";
    nb++;

    printf("  --- Braid-generated PD ---\n");
    for (i = 0; i < nb; i++) {
        braid_to_pd(&braids[i], &k);
        sc = seifert_circles(&k);
        sprintf(msg, "%s: Seifert circles = %d (strands = %d)",
                bnames[i], sc, braids[i].n);
        check(msg, sc == braids[i].n);
    }

    /* Test on Demo 01's hand-coded PD */
    printf("  --- Demo 01 hand-coded PD ---\n");

    make_trefoil_pd(&k);
    sc = seifert_circles(&k);
    sprintf(msg, "trefoil PD: Seifert circles = %d (expected 2)", sc);
    check(msg, sc == 2);

    make_figure_eight_pd(&k);
    sc = seifert_circles(&k);
    sprintf(msg, "figure-eight PD: Seifert circles = %d (expected 3)", sc);
    check(msg, sc == 3);

    make_hopf_pd(&k);
    sc = seifert_circles(&k);
    sprintf(msg, "Hopf PD: Seifert circles = %d (expected 2)", sc);
    check(msg, sc == 2);
}

static void test_part_d(void) {
    /*
     * Part D: Link component count.
     * Verify each braid closure gives correct number of components.
     * - Trefoil (2-strand, 3 crossings): 1 component (knot)
     * - Figure-eight (3-strand, 4 crossings): 1 component (knot)
     * - Hopf link (2-strand, 2 crossings): 2 components (link)
     * - Trefoil (3-strand): 1 component
     * - Unknot (sigma sigma^-1): 1 component
     */
    Braid braids[5];
    const char *names[5];
    int expected_comps[5];
    int nb, i;
    Knot k;
    char msg[120];

    printf("\n=== PART D: Link Components ===\n");

    nb = 0;

    braids[nb].n = 2; braids[nb].len = 3;
    braids[nb].word[0] = 1; braids[nb].word[1] = 1; braids[nb].word[2] = 1;
    names[nb] = "trefoil (2-strand)";
    expected_comps[nb] = 1;
    nb++;

    braids[nb].n = 3; braids[nb].len = 4;
    braids[nb].word[0] = 1; braids[nb].word[1] = -2;
    braids[nb].word[2] = 1; braids[nb].word[3] = -2;
    names[nb] = "figure-eight";
    expected_comps[nb] = 1;
    nb++;

    braids[nb].n = 2; braids[nb].len = 2;
    braids[nb].word[0] = 1; braids[nb].word[1] = 1;
    names[nb] = "Hopf link";
    expected_comps[nb] = 2;
    nb++;

    braids[nb].n = 3; braids[nb].len = 3;
    braids[nb].word[0] = 1; braids[nb].word[1] = 2; braids[nb].word[2] = 1;
    names[nb] = "s1 s2 s1 (3-strand, 2-comp link)";
    expected_comps[nb] = 2;
    nb++;

    braids[nb].n = 2; braids[nb].len = 2;
    braids[nb].word[0] = 1; braids[nb].word[1] = -1;
    names[nb] = "unlink (sigma sigma^-1)";
    expected_comps[nb] = 2;
    nb++;

    for (i = 0; i < nb; i++) {
        int comps;
        braid_to_pd(&braids[i], &k);
        comps = link_components(&k);
        sprintf(msg, "%s: %d component(s)", names[i], comps);
        check(msg, comps == expected_comps[i]);
    }

    /* Also test Demo 01's hand-coded PD */
    printf("  --- Demo 01 PD ---\n");
    make_trefoil_pd(&k);
    check("trefoil PD: 1 component", link_components(&k) == 1);

    make_figure_eight_pd(&k);
    check("figure-eight PD: 1 component", link_components(&k) == 1);

    make_hopf_pd(&k);
    check("Hopf PD: 2 components", link_components(&k) == 2);
}

int main(void) {
    printf("KNOTAPEL DEMO 07: Braid Closure Bridge\n");
    printf("=======================================\n");

    test_part_a();
    test_part_b();
    test_part_c();
    test_part_d();

    printf("\n=======================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("=======================================\n");
    return n_fail > 0 ? 1 : 0;
}
