/*
 * KNOTAPEL DEMO 08: Vogel's Algorithm (PD -> Braid Word)
 * ======================================================
 *
 * Demo 07 built the bridge from braids to PD notation.
 * This demo builds the compiler in the other direction:
 * given PD notation, produce a braid word whose closure
 * gives the same knot/link.
 *
 * Algorithm (for path-graph Seifert graphs):
 *   Step 1: Seifert circle membership via union-find
 *   Step 2: Crossing-circle map (which circles each crossing connects)
 *   Step 3: Build reduced Seifert graph, verify it's a path
 *   Step 4: Order circles as braid strands (BFS from leaf)
 *   Step 5: Trace circles for cyclic crossing order
 *   Step 6: Find consistent linear ordering (brute-force cut + topo sort)
 *   Step 7: Emit braid generators
 *
 * For standard small knots (trefoil, figure-eight, Hopf), the Seifert
 * graph is always a path graph, so no Vogel moves are needed.
 *
 * PLAN:
 *   Part A: Seifert membership verification
 *   Part B: PD -> Braid basic conversion
 *   Part C: Round-trip bracket (PD -> Braid bracket == PD bracket)
 *   Part D: Double round-trip (Braid -> PD -> Braid -> PD bracket match)
 *
 * C89 compliant, zero dependencies.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Laurent polynomial ring (reused from Demo 07)
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
 * PD notation and state-sum Kauffman bracket (from Demo 07)
 * ================================================================ */
#define MAX_X 20
#define MAX_ARCS 64

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
 * Braid type and braid bracket (from Demo 07)
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
static void uf_union(int a, int b) {
    a = uf_find(a); b = uf_find(b); if (a != b) uf_p[a] = b;
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
 * BRAID CLOSURE -> PD NOTATION (from Demo 07, for round-trip tests)
 * ================================================================ */
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
 * Demo 01's known knots in PD notation
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
 * NEW: Seifert circle membership via union-find
 *
 * Resolves each crossing according to Seifert smoothing:
 *   Positive: B-smoothing pairs (0,1) and (2,3)
 *   Negative: A-smoothing pairs (0,3) and (1,2)
 *
 * Returns circle_id[arc] and total number of circles.
 * ================================================================ */
static void seifert_membership(const Knot *k, int *circle_id,
                               int *num_circles) {
    int seen[MAX_ARCS];
    int i, nc;

    uf_init(k->num_arcs);

    for (i = 0; i < k->n; i++) {
        if (k->x[i].sign > 0) {
            /* Positive: B-smoothing, pairs (0,1) and (2,3) */
            uf_union(k->x[i].arcs[0], k->x[i].arcs[1]);
            uf_union(k->x[i].arcs[2], k->x[i].arcs[3]);
        } else {
            /* Negative: A-smoothing, pairs (0,3) and (1,2) */
            uf_union(k->x[i].arcs[0], k->x[i].arcs[3]);
            uf_union(k->x[i].arcs[1], k->x[i].arcs[2]);
        }
    }

    /* Normalize circle IDs to 0, 1, 2, ... */
    memset(seen, -1, sizeof(seen));
    nc = 0;
    for (i = 0; i < k->num_arcs; i++) {
        int root = uf_find(i);
        if (seen[root] < 0) seen[root] = nc++;
        circle_id[i] = seen[root];
    }
    *num_circles = nc;
}

/* ================================================================
 * NEW: Trace Seifert circles to get cyclic crossing order
 *
 * For each circle, traces arcs through Seifert-resolved crossings
 * and records the sequence of crossings encountered.
 *
 * Output: trace[circle][k] = k-th crossing in cyclic order
 *         trace_len[circle] = number of crossings visited
 * ================================================================ */
static void trace_circles(const Knot *k, const int *circle_id, int nc,
                          int trace[][MAX_X], int *trace_len) {
    int app_x[MAX_ARCS][2], app_p[MAX_ARCS][2], app_n[MAX_ARCS];
    int pair[MAX_X][4];
    int visited[MAX_ARCS];
    int i, j;

    /* Build appearance table: for each arc, which two crossings it appears in */
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

    /* Seifert pairing at each crossing */
    for (i = 0; i < k->n; i++) {
        if (k->x[i].sign > 0) {
            pair[i][0] = 1; pair[i][1] = 0;
            pair[i][2] = 3; pair[i][3] = 2;
        } else {
            pair[i][0] = 3; pair[i][3] = 0;
            pair[i][1] = 2; pair[i][2] = 1;
        }
    }

    for (i = 0; i < nc; i++) trace_len[i] = 0;
    memset(visited, 0, sizeof(visited));

    for (i = 0; i < k->num_arcs; i++) {
        int cur_arc, cur_side, cid;
        if (visited[i]) continue;

        cur_arc = i; cur_side = 0;
        cid = circle_id[i];

        while (!visited[cur_arc]) {
            int cx, pos, pp, pa, os;
            visited[cur_arc] = 1;

            /* Visit the crossing on this side of the arc */
            cx = app_x[cur_arc][cur_side];
            pos = app_p[cur_arc][cur_side];
            trace[cid][trace_len[cid]++] = cx;

            /* Follow Seifert pairing to partner arc */
            pp = pair[cx][pos];
            pa = k->x[cx].arcs[pp];

            /* Continue from the OTHER crossing on the partner arc */
            os = (app_x[pa][0] == cx && app_p[pa][0] == pp) ? 1 : 0;
            cur_arc = pa;
            cur_side = os;
        }
    }
}

/* ================================================================
 * NEW: Find braid word ordering via brute-force cut + topo sort
 *
 * Each Seifert circle has a cyclic sequence of crossings. We try
 * all combinations of "cut points" (where to break each cycle into
 * a linear sequence), then check if the combined constraints form
 * an acyclic DAG via Kahn's topological sort.
 *
 * Key insight: crossings with the SAME generator index (connecting
 * the same pair of adjacent circles) commute, so we skip ordering
 * constraints between them. This resolves cases like the trefoil
 * where all crossings have the same generator.
 *
 * Returns 1 on success (order[] filled), 0 on failure.
 * ================================================================ */
static int find_braid_order(int nc, int num_xings,
                            int trace[][MAX_X], const int *trace_len,
                            const int *gen_idx, int *order) {
    int cuts[MAX_X];
    int ci, i, j;

    memset(cuts, 0, sizeof(cuts));

    while (1) {
        /* Build DAG from current cut combination */
        int dag[MAX_X][MAX_X];
        int indeg[MAX_X];
        int valid = 1;

        memset(dag, 0, sizeof(dag));
        memset(indeg, 0, sizeof(indeg));

        for (ci = 0; ci < nc && valid; ci++) {
            int len = trace_len[ci];
            if (len <= 1) continue;
            for (j = 0; j < len - 1; j++) {
                int from = trace[ci][(cuts[ci] + j) % len];
                int to   = trace[ci][(cuts[ci] + j + 1) % len];
                /* Skip same-generator crossings (they commute) */
                if (gen_idx[from] == gen_idx[to]) continue;
                /* Check for contradiction */
                if (dag[to][from]) { valid = 0; break; }
                if (!dag[from][to]) {
                    dag[from][to] = 1;
                    indeg[to]++;
                }
            }
        }

        if (valid) {
            /* Kahn's topological sort */
            int q[MAX_X], qh = 0, qt = 0, sorted = 0;
            int td[MAX_X];
            memcpy(td, indeg, (size_t)num_xings * sizeof(int));
            for (i = 0; i < num_xings; i++)
                if (td[i] == 0) q[qt++] = i;
            while (qh < qt) {
                int v = q[qh++];
                order[sorted++] = v;
                for (i = 0; i < num_xings; i++) {
                    if (dag[v][i]) {
                        td[i]--;
                        if (td[i] == 0) q[qt++] = i;
                    }
                }
            }
            if (sorted == num_xings) return 1; /* Success */
        }

        /* Increment cuts (odometer) */
        for (ci = 0; ci < nc; ci++) {
            cuts[ci]++;
            if (cuts[ci] < trace_len[ci]) break;
            cuts[ci] = 0;
        }
        if (ci == nc) break; /* All combinations exhausted */
    }

    return 0; /* Failed */
}

/* ================================================================
 * NEW: PD -> Braid compiler
 *
 * Converts PD notation to a braid word whose closure gives the
 * same knot/link. Currently handles path-graph Seifert graphs only
 * (no Vogel moves).
 *
 * Returns 0 on success, -1 if Seifert graph is not a path,
 * -2 if no valid braid ordering found.
 * ================================================================ */
static int pd_to_braid(const Knot *k, Braid *b) {
    int circle_id[MAX_ARCS], nc;
    int xc[MAX_X][2];          /* two circles for each crossing */
    int adj[MAX_X][MAX_X];
    int degree[MAX_X];
    int strand_of[MAX_X];      /* strand index for each circle (0-based) */
    int gen_idx[MAX_X];        /* generator index for each crossing */
    int trace_data[MAX_X][MAX_X];
    int trace_len_data[MAX_X];
    int order[MAX_X];
    int i, j;

    if (k->n == 0) { b->n = 1; b->len = 0; return 0; }

    /* Step 1: Seifert circle membership */
    seifert_membership(k, circle_id, &nc);

    /* Step 2: Crossing-circle map.
     * Positive crossing: arcs[0,1] in one circle, arcs[2,3] in another
     * Negative crossing: arcs[0,3] in one circle, arcs[1,2] in another */
    for (i = 0; i < k->n; i++) {
        if (k->x[i].sign > 0) {
            xc[i][0] = circle_id[k->x[i].arcs[0]];
            xc[i][1] = circle_id[k->x[i].arcs[2]];
        } else {
            xc[i][0] = circle_id[k->x[i].arcs[0]];
            xc[i][1] = circle_id[k->x[i].arcs[1]];
        }
    }

    /* Step 3: Build reduced Seifert graph, verify it's a path */
    memset(adj, 0, sizeof(adj));
    memset(degree, 0, sizeof(degree));
    for (i = 0; i < k->n; i++) {
        int ca = xc[i][0], cb = xc[i][1];
        if (ca != cb && !adj[ca][cb]) {
            adj[ca][cb] = 1; adj[cb][ca] = 1;
            degree[ca]++; degree[cb]++;
        }
    }

    /* Path graph check: all vertices degree <= 2, exactly 2 leaves */
    {
        int leaves = 0;
        for (i = 0; i < nc; i++) {
            if (degree[i] > 2) return -1; /* Not a path graph */
            if (degree[i] <= 1) leaves++;
        }
        if (nc > 1 && leaves != 2) return -1;
    }

    /* Step 4: Order circles as braid strands (walk from leaf) */
    {
        int leaf = 0, cur, prev, nxt, si;
        for (i = 0; i < nc; i++)
            if (degree[i] <= 1) { leaf = i; break; }
        cur = leaf; prev = -1; si = 0;
        while (1) {
            strand_of[cur] = si++;
            nxt = -1;
            for (j = 0; j < nc; j++)
                if (adj[cur][j] && j != prev) { nxt = j; break; }
            if (nxt < 0) break;
            prev = cur; cur = nxt;
        }
    }

    /* Compute generator index for each crossing */
    for (i = 0; i < k->n; i++) {
        int sa = strand_of[xc[i][0]], sb = strand_of[xc[i][1]];
        gen_idx[i] = sa < sb ? sa : sb;
    }

    /* Step 5: Trace Seifert circles for cyclic crossing order */
    trace_circles(k, circle_id, nc, trace_data, trace_len_data);

    /* Step 6: Find consistent braid word ordering */
    if (!find_braid_order(nc, k->n, trace_data, trace_len_data,
                          gen_idx, order))
        return -2;

    /* Step 7: Emit braid generators */
    b->n = nc;
    b->len = k->n;
    for (i = 0; i < k->n; i++) {
        int xi = order[i];
        int gen = gen_idx[xi] + 1; /* 1-indexed */
        b->word[i] = k->x[xi].sign > 0 ? gen : -gen;
    }

    return 0;
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
 * PART A: Seifert circle membership verification
 * ================================================================ */
static void test_part_a(void) {
    Knot k;
    int cid[MAX_ARCS], nc;

    printf("\n=== PART A: Seifert Circle Membership ===\n");

    /* Trefoil: 6 arcs -> 2 circles: {0,2,4} and {1,3,5} */
    make_trefoil_pd(&k);
    seifert_membership(&k, cid, &nc);
    check("trefoil: 2 Seifert circles", nc == 2);
    check("trefoil: arcs 0,2 same circle", cid[0] == cid[2]);
    check("trefoil: arcs 0,4 same circle", cid[0] == cid[4]);
    check("trefoil: arcs 1,3 same circle", cid[1] == cid[3]);
    check("trefoil: arcs 1,5 same circle", cid[1] == cid[5]);
    check("trefoil: arcs 0,1 different circles", cid[0] != cid[1]);

    /* Figure-eight: 8 arcs -> 3 circles: {0,4}, {1,3,5,7}, {2,6} */
    make_figure_eight_pd(&k);
    seifert_membership(&k, cid, &nc);
    check("figure-eight: 3 Seifert circles", nc == 3);
    check("fig-8: arcs 0,4 same circle", cid[0] == cid[4]);
    check("fig-8: arcs 1,3 same circle", cid[1] == cid[3]);
    check("fig-8: arcs 1,5 same circle", cid[1] == cid[5]);
    check("fig-8: arcs 1,7 same circle", cid[1] == cid[7]);
    check("fig-8: arcs 2,6 same circle", cid[2] == cid[6]);
    check("fig-8: three distinct circles",
          cid[0] != cid[1] && cid[1] != cid[2] && cid[0] != cid[2]);

    /* Hopf link: 4 arcs -> 2 circles: {0,3} and {1,2} */
    make_hopf_pd(&k);
    seifert_membership(&k, cid, &nc);
    check("Hopf: 2 Seifert circles", nc == 2);
    check("Hopf: arcs 0,3 same circle", cid[0] == cid[3]);
    check("Hopf: arcs 1,2 same circle", cid[1] == cid[2]);
    check("Hopf: arcs 0,1 different circles", cid[0] != cid[1]);
}

/* ================================================================
 * PART B: PD -> Braid basic conversion
 * ================================================================ */
static void test_part_b(void) {
    Knot k;
    Braid b;
    int ret, i;

    printf("\n=== PART B: PD -> Braid Conversion ===\n");

    /* Trefoil -> sigma_1^3 on 2 strands */
    make_trefoil_pd(&k);
    ret = pd_to_braid(&k, &b);
    check("trefoil: conversion succeeds", ret == 0);
    check("trefoil: 2 strands", b.n == 2);
    check("trefoil: 3 crossings", b.len == 3);
    {
        int all_s1 = 1;
        for (i = 0; i < b.len; i++)
            if (b.word[i] != 1) all_s1 = 0;
        check("trefoil: all sigma_1", all_s1);
    }
    printf("    trefoil braid: [");
    for (i = 0; i < b.len; i++) printf("%s%d", i ? "," : "", b.word[i]);
    printf("]\n");

    /* Figure-eight -> 2x sigma_1 + 2x sigma_2^{-1} on 3 strands */
    make_figure_eight_pd(&k);
    ret = pd_to_braid(&k, &b);
    check("figure-eight: conversion succeeds", ret == 0);
    check("figure-eight: 3 strands", b.n == 3);
    check("figure-eight: 4 crossings", b.len == 4);
    {
        int s1 = 0, s2m = 0;
        for (i = 0; i < b.len; i++) {
            if (b.word[i] == 1) s1++;
            if (b.word[i] == -2) s2m++;
        }
        check("fig-8: 2x sigma_1 + 2x sigma_2^{-1}", s1 == 2 && s2m == 2);
    }
    printf("    figure-eight braid: [");
    for (i = 0; i < b.len; i++) printf("%s%d", i ? "," : "", b.word[i]);
    printf("]\n");

    /* Hopf link -> sigma_1^2 on 2 strands */
    make_hopf_pd(&k);
    ret = pd_to_braid(&k, &b);
    check("Hopf: conversion succeeds", ret == 0);
    check("Hopf: 2 strands", b.n == 2);
    check("Hopf: 2 crossings", b.len == 2);
    {
        int all_s1 = 1;
        for (i = 0; i < b.len; i++)
            if (b.word[i] != 1) all_s1 = 0;
        check("Hopf: all sigma_1", all_s1);
    }
    printf("    Hopf braid: [");
    for (i = 0; i < b.len; i++) printf("%s%d", i ? "," : "", b.word[i]);
    printf("]\n");
}

/* ================================================================
 * PART C: Round-trip bracket verification
 * PD -> Braid -> braid_bracket should equal pd_bracket(PD)
 * ================================================================ */
static void test_part_c(void) {
    Knot k;
    Braid b;
    Poly pd_br, br_br;
    int ret;

    printf("\n=== PART C: Round-Trip Bracket ===\n");

    /* Trefoil */
    make_trefoil_pd(&k);
    pd_bracket(&pd_br, &k);
    ret = pd_to_braid(&k, &b);
    check("trefoil: pd_to_braid succeeds", ret == 0);
    braid_bracket(&br_br, &b);
    p_print(&pd_br, "    PD bracket   ");
    p_print(&br_br, "    braid bracket");
    check("trefoil: PD bracket == braid bracket", p_eq(&pd_br, &br_br));

    /* Figure-eight */
    make_figure_eight_pd(&k);
    pd_bracket(&pd_br, &k);
    ret = pd_to_braid(&k, &b);
    check("fig-8: pd_to_braid succeeds", ret == 0);
    braid_bracket(&br_br, &b);
    p_print(&pd_br, "    PD bracket   ");
    p_print(&br_br, "    braid bracket");
    check("fig-8: PD bracket == braid bracket", p_eq(&pd_br, &br_br));

    /* Hopf link */
    make_hopf_pd(&k);
    pd_bracket(&pd_br, &k);
    ret = pd_to_braid(&k, &b);
    check("Hopf: pd_to_braid succeeds", ret == 0);
    braid_bracket(&br_br, &b);
    p_print(&pd_br, "    PD bracket   ");
    p_print(&br_br, "    braid bracket");
    check("Hopf: PD bracket == braid bracket", p_eq(&pd_br, &br_br));
}

/* ================================================================
 * PART D: Double round-trip
 * Braid_1 -> PD_1 -> Braid_2 -> PD_2
 * Verify pd_bracket(PD_1) == pd_bracket(PD_2)
 * and    braid_bracket(Braid_1) == braid_bracket(Braid_2)
 * ================================================================ */
static void test_part_d(void) {
    Braid b1, b2;
    Knot k1, k2;
    Poly br1, br2, bb1, bb2;
    int ret;

    printf("\n=== PART D: Double Round-Trip ===\n");

    /* Trefoil: sigma_1^3 on 2 strands */
    b1.n = 2; b1.len = 3;
    b1.word[0] = 1; b1.word[1] = 1; b1.word[2] = 1;
    braid_to_pd(&b1, &k1);
    pd_bracket(&br1, &k1);
    ret = pd_to_braid(&k1, &b2);
    check("trefoil: pd_to_braid on braid-generated PD", ret == 0);
    braid_to_pd(&b2, &k2);
    pd_bracket(&br2, &k2);
    p_print(&br1, "    PD1 bracket");
    p_print(&br2, "    PD2 bracket");
    check("trefoil: double round-trip PD bracket match", p_eq(&br1, &br2));
    braid_bracket(&bb1, &b1);
    braid_bracket(&bb2, &b2);
    check("trefoil: double round-trip braid bracket match", p_eq(&bb1, &bb2));

    /* Figure-eight: sigma_1 sigma_2^{-1} sigma_1 sigma_2^{-1} */
    b1.n = 3; b1.len = 4;
    b1.word[0] = 1; b1.word[1] = -2; b1.word[2] = 1; b1.word[3] = -2;
    braid_to_pd(&b1, &k1);
    pd_bracket(&br1, &k1);
    ret = pd_to_braid(&k1, &b2);
    check("fig-8: pd_to_braid on braid-generated PD", ret == 0);
    braid_to_pd(&b2, &k2);
    pd_bracket(&br2, &k2);
    p_print(&br1, "    PD1 bracket");
    p_print(&br2, "    PD2 bracket");
    check("fig-8: double round-trip PD bracket match", p_eq(&br1, &br2));
    braid_bracket(&bb1, &b1);
    braid_bracket(&bb2, &b2);
    check("fig-8: double round-trip braid bracket match", p_eq(&bb1, &bb2));

    /* Hopf link: sigma_1^2 on 2 strands */
    b1.n = 2; b1.len = 2;
    b1.word[0] = 1; b1.word[1] = 1;
    braid_to_pd(&b1, &k1);
    pd_bracket(&br1, &k1);
    ret = pd_to_braid(&k1, &b2);
    check("Hopf: pd_to_braid on braid-generated PD", ret == 0);
    braid_to_pd(&b2, &k2);
    pd_bracket(&br2, &k2);
    p_print(&br1, "    PD1 bracket");
    p_print(&br2, "    PD2 bracket");
    check("Hopf: double round-trip PD bracket match", p_eq(&br1, &br2));
    braid_bracket(&bb1, &b1);
    braid_bracket(&bb2, &b2);
    check("Hopf: double round-trip braid bracket match", p_eq(&bb1, &bb2));

    /* Trefoil on 3 strands: sigma_1 sigma_2 sigma_1 */
    b1.n = 3; b1.len = 3;
    b1.word[0] = 1; b1.word[1] = 2; b1.word[2] = 1;
    braid_to_pd(&b1, &k1);
    pd_bracket(&br1, &k1);
    ret = pd_to_braid(&k1, &b2);
    check("trefoil-3: pd_to_braid on braid-generated PD", ret == 0);
    braid_to_pd(&b2, &k2);
    pd_bracket(&br2, &k2);
    check("trefoil-3: double round-trip PD bracket match", p_eq(&br1, &br2));
    braid_bracket(&bb1, &b1);
    braid_bracket(&bb2, &b2);
    check("trefoil-3: double round-trip braid bracket match", p_eq(&bb1, &bb2));
}

/* ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 08: Vogel's Algorithm (PD -> Braid)\n");
    printf("==================================================\n");

    test_part_a();
    test_part_b();
    test_part_c();
    test_part_d();

    printf("\n==================================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("==================================================\n");
    return n_fail > 0 ? 1 : 0;
}
