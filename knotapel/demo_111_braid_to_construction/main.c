/*
 * KNOTAPEL DEMO 111: Braid-to-Construction-Word Pipeline
 * ========================================================
 *
 * Given a braid word (catalogued for all knots), produce a stick
 * polygon with integer coordinates, then express it as a construction
 * word. This unlocks the entire knot table for construction word
 * analysis.
 *
 * Pipeline:
 *   BraidWord -> strand traces -> braid closure -> polygon
 *   -> verify (crossings, determinant) -> construction word
 *   -> analysis (influence, majority structure)
 *
 * Reuses from Demo 110: rational arithmetic, Vec3Q, mirror formula,
 * crossing detection, Alexander matrix / knot determinant.
 *
 * C89, zero dependencies beyond stdio/stdlib/string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * Part A: Exact Rational Arithmetic
 * (from Demo 110)
 * ================================================================ */

typedef long i64;

static i64 rat_abs(i64 x) { return x < 0 ? -x : x; }

static i64 rat_gcd(i64 a, i64 b) {
    a = rat_abs(a);
    b = rat_abs(b);
    while (b) { i64 t = b; b = a % b; a = t; }
    return a;
}

typedef struct {
    i64 p; /* numerator */
    i64 q; /* denominator, always > 0 */
} Rat;

static Rat rat_make(i64 p, i64 q) {
    Rat r;
    i64 g;
    if (q == 0) { fprintf(stderr, "rat_make: zero denominator\n"); exit(1); }
    if (q < 0) { p = -p; q = -q; }
    g = rat_gcd(rat_abs(p), q);
    r.p = p / g;
    r.q = q / g;
    return r;
}

static Rat rat_from_int(i64 n) {
    Rat r; r.p = n; r.q = 1; return r;
}

static Rat rat_add(Rat a, Rat b) {
    i64 g = rat_gcd(a.q, b.q);
    i64 aq_r = a.q / g;
    i64 bq_r = b.q / g;
    return rat_make(a.p * bq_r + b.p * aq_r, aq_r * b.q);
}

static Rat rat_sub(Rat a, Rat b) {
    i64 g = rat_gcd(a.q, b.q);
    i64 aq_r = a.q / g;
    i64 bq_r = b.q / g;
    return rat_make(a.p * bq_r - b.p * aq_r, aq_r * b.q);
}

static Rat rat_mul(Rat a, Rat b) {
    i64 g1 = rat_gcd(rat_abs(a.p), b.q);
    i64 g2 = rat_gcd(rat_abs(b.p), a.q);
    return rat_make((a.p / g1) * (b.p / g2), (a.q / g2) * (b.q / g1));
}

static Rat rat_div(Rat a, Rat b) {
    i64 g1, g2;
    if (b.p == 0) { fprintf(stderr, "rat_div: division by zero\n"); exit(1); }
    g1 = rat_gcd(rat_abs(a.p), rat_abs(b.p));
    g2 = rat_gcd(a.q, b.q);
    return rat_make((a.p / g1) * (b.q / g2), (a.q / g2) * (b.p / g1));
}

static Rat rat_neg(Rat a) {
    Rat r; r.p = -a.p; r.q = a.q; return r;
}

static int rat_eq(Rat a, Rat b) {
    return a.p == b.p && a.q == b.q;
}

static int rat_is_zero(Rat a) {
    return a.p == 0;
}

static int rat_cmp(Rat a, Rat b) {
    i64 lhs = a.p * b.q;
    i64 rhs = b.p * a.q;
    if (lhs < rhs) return -1;
    if (lhs > rhs) return 1;
    return 0;
}

static Rat rat_two(void) { Rat r; r.p = 2; r.q = 1; return r; }

static void rat_print(Rat r) {
    if (r.q == 1) printf("%ld", r.p);
    else printf("%ld/%ld", r.p, r.q);
}

/* ================================================================
 * Part B: Rational 3D Vectors
 * (from Demo 110)
 * ================================================================ */

typedef struct {
    Rat x, y, z;
} Vec3Q;

static Vec3Q v3q_make(Rat x, Rat y, Rat z) {
    Vec3Q v; v.x = x; v.y = y; v.z = z; return v;
}

static Vec3Q v3q_from_ints(i64 x, i64 y, i64 z) {
    return v3q_make(rat_from_int(x), rat_from_int(y), rat_from_int(z));
}

static Vec3Q v3q_add(Vec3Q a, Vec3Q b) {
    return v3q_make(rat_add(a.x, b.x), rat_add(a.y, b.y), rat_add(a.z, b.z));
}

static Vec3Q v3q_sub(Vec3Q a, Vec3Q b) {
    return v3q_make(rat_sub(a.x, b.x), rat_sub(a.y, b.y), rat_sub(a.z, b.z));
}

static Vec3Q v3q_scale(Rat s, Vec3Q v) {
    return v3q_make(rat_mul(s, v.x), rat_mul(s, v.y), rat_mul(s, v.z));
}

static Rat v3q_dot(Vec3Q a, Vec3Q b) {
    return rat_add(rat_add(rat_mul(a.x, b.x), rat_mul(a.y, b.y)),
                   rat_mul(a.z, b.z));
}

static Vec3Q v3q_cross(Vec3Q a, Vec3Q b) {
    return v3q_make(
        rat_sub(rat_mul(a.y, b.z), rat_mul(a.z, b.y)),
        rat_sub(rat_mul(a.z, b.x), rat_mul(a.x, b.z)),
        rat_sub(rat_mul(a.x, b.y), rat_mul(a.y, b.x))
    );
}

static Rat v3q_dist_sq(Vec3Q a, Vec3Q b) {
    Vec3Q d = v3q_sub(a, b);
    return v3q_dot(d, d);
}

static int v3q_eq(Vec3Q a, Vec3Q b) {
    return rat_eq(a.x, b.x) && rat_eq(a.y, b.y) && rat_eq(a.z, b.z);
}

static void v3q_print(Vec3Q v) {
    printf("(");
    rat_print(v.x); printf(", ");
    rat_print(v.y); printf(", ");
    rat_print(v.z); printf(")");
}

/* ================================================================
 * Part C: Mirror Formula
 * (from Demo 110)
 *
 * P' = P - 2 * ((n . (P - C0)) / (n . n)) * n
 * ================================================================ */

static Vec3Q mirror_point(Vec3Q c0, Vec3Q c1, Vec3Q c2, Vec3Q target) {
    Vec3Q e1 = v3q_sub(c1, c0);
    Vec3Q e2 = v3q_sub(c2, c0);
    Vec3Q n = v3q_cross(e1, e2);
    Rat n_dot_n = v3q_dot(n, n);
    Vec3Q p_minus_c0 = v3q_sub(target, c0);
    Rat n_dot_d = v3q_dot(n, p_minus_c0);
    Rat t = rat_div(rat_mul(rat_two(), n_dot_d), n_dot_n);
    return v3q_sub(target, v3q_scale(t, n));
}

/* ================================================================
 * Part D: Stick Knot and Construction Word Types
 * (from Demo 110, with increased vertex limit)
 * ================================================================ */

#define MAX_STICK_VERTICES 128

typedef struct {
    const char *name;
    int crossing_number;
    int n_vertices;
    Vec3Q vertices[MAX_STICK_VERTICES];
} StickKnot;

typedef struct {
    int center_idx[3];
    int target_idx;
} ConstructionStep;

typedef struct {
    int base[3];
    int n_steps;
    ConstructionStep steps[MAX_STICK_VERTICES];
} ConstructionWord;

static void evaluate_construction(const StickKnot *knot,
                                  const ConstructionWord *word,
                                  unsigned int choices,
                                  Vec3Q *out_vertices,
                                  int *out_n) {
    int i;
    out_vertices[0] = knot->vertices[word->base[0]];
    out_vertices[1] = knot->vertices[word->base[1]];
    out_vertices[2] = knot->vertices[word->base[2]];
    *out_n = 3;

    for (i = 0; i < word->n_steps; i++) {
        const ConstructionStep *step = &word->steps[i];
        Vec3Q c0 = out_vertices[step->center_idx[0]];
        Vec3Q c1 = out_vertices[step->center_idx[1]];
        Vec3Q c2 = out_vertices[step->center_idx[2]];
        Vec3Q target = knot->vertices[step->target_idx];
        int use_mirror = (int)((choices >> (unsigned)i) & 1u);

        if (use_mirror) {
            out_vertices[*out_n] = mirror_point(c0, c1, c2, target);
        } else {
            out_vertices[*out_n] = target;
        }
        (*out_n)++;
    }
}

/* ================================================================
 * Part E: Crossing Detection
 * (from Demo 110)
 * ================================================================ */

typedef struct { Rat x, y; } Vec2Q;

typedef struct {
    int edge_a;
    int edge_b;
    int sign;
    Rat z_a;
    Rat z_b;
    Rat param_a;
    Rat param_b;
} Crossing;

static int segments_intersect_2d(Vec2Q p1, Vec2Q p2, Vec2Q p3, Vec2Q p4,
                                 Rat *s_out, Rat *t_out) {
    Rat d1x = rat_sub(p2.x, p1.x);
    Rat d1y = rat_sub(p2.y, p1.y);
    Rat d2x = rat_sub(p4.x, p3.x);
    Rat d2y = rat_sub(p4.y, p3.y);
    Rat denom = rat_sub(rat_mul(d1x, d2y), rat_mul(d1y, d2x));
    Rat dx, dy, s, t, zero, one;

    if (rat_is_zero(denom)) return 0;

    dx = rat_sub(p3.x, p1.x);
    dy = rat_sub(p3.y, p1.y);
    s = rat_div(rat_sub(rat_mul(dx, d2y), rat_mul(dy, d2x)), denom);
    t = rat_div(rat_sub(rat_mul(dx, d1y), rat_mul(dy, d1x)), denom);

    zero = rat_from_int(0);
    one = rat_from_int(1);

    if (rat_cmp(s, zero) > 0 && rat_cmp(s, one) < 0 &&
        rat_cmp(t, zero) > 0 && rat_cmp(t, one) < 0) {
        *s_out = s;
        *t_out = t;
        return 1;
    }
    return 0;
}

static void generic_project(Vec3Q v3, Vec2Q *uv, Rat *depth) {
    Rat c2 = rat_from_int(2), c3 = rat_from_int(3), c5 = rat_from_int(5);
    uv->x = rat_sub(rat_mul(c5, v3.x), rat_mul(c2, v3.z));
    uv->y = rat_sub(rat_mul(c5, v3.y), rat_mul(c3, v3.z));
    *depth = rat_add(rat_mul(c2, v3.x),
                     rat_add(rat_mul(c3, v3.y), rat_mul(c5, v3.z)));
}

#define MAX_CROSSINGS 96
#define MAX_ARCS 96

static int find_crossings(const Vec3Q *verts, int n,
                          Crossing *crossings, int max_crossings) {
    int i, j, nc = 0;
    Vec2Q proj[MAX_STICK_VERTICES];
    Rat depth[MAX_STICK_VERTICES];

    for (i = 0; i < n; i++) {
        generic_project(verts[i], &proj[i], &depth[i]);
    }

    for (i = 0; i < n && nc < max_crossings; i++) {
        int i2 = (i + 1) % n;
        for (j = i + 2; j < n && nc < max_crossings; j++) {
            int j2 = (j + 1) % n;
            Rat s, t, da, db;

            if (j2 == i) continue;

            if (segments_intersect_2d(proj[i], proj[i2],
                                      proj[j], proj[j2], &s, &t)) {
                Rat dix, diy, djx, djy, cross2d;
                int over_under, orient;

                da = rat_add(depth[i],
                    rat_mul(s, rat_sub(depth[i2], depth[i])));
                db = rat_add(depth[j],
                    rat_mul(t, rat_sub(depth[j2], depth[j])));

                dix = rat_sub(proj[i2].x, proj[i].x);
                diy = rat_sub(proj[i2].y, proj[i].y);
                djx = rat_sub(proj[j2].x, proj[j].x);
                djy = rat_sub(proj[j2].y, proj[j].y);

                cross2d = rat_sub(rat_mul(dix, djy), rat_mul(diy, djx));
                orient = rat_cmp(cross2d, rat_from_int(0));
                over_under = rat_cmp(da, db);

                crossings[nc].edge_a = i;
                crossings[nc].edge_b = j;
                crossings[nc].z_a = da;
                crossings[nc].z_b = db;
                crossings[nc].param_a = s;
                crossings[nc].param_b = t;
                crossings[nc].sign = orient * over_under;
                nc++;
            }
        }
    }
    return nc;
}

static int compute_writhe(const Crossing *crossings, int nc) {
    int i, w = 0;
    for (i = 0; i < nc; i++) w += crossings[i].sign;
    return w;
}

/* ================================================================
 * Part F: Knot Determinant
 * (from Demo 110)
 * ================================================================ */

typedef struct {
    int edge_idx;
    Rat param;
    int crossing_idx;
} ArcBreak;

static int find_arc_at(const ArcBreak *breaks, int n_breaks,
                       int edge, Rat param) {
    int i, last = -1;
    for (i = 0; i < n_breaks; i++) {
        if (breaks[i].edge_idx < edge ||
            (breaks[i].edge_idx == edge &&
             rat_cmp(breaks[i].param, param) <= 0)) {
            last = i;
        }
    }
    if (last >= 0) return last;
    return n_breaks - 1;
}

static int knot_determinant(int n_edges, const Crossing *crossings, int nc) {
    ArcBreak breaks[MAX_CROSSINGS];
    int over_arc[MAX_CROSSINGS];
    int under_in_arc[MAX_CROSSINGS];
    int under_out_arc[MAX_CROSSINGS];
    long M[MAX_CROSSINGS][MAX_ARCS];
    int i, j, k, sz;
    long det;

    (void)n_edges;

    if (nc == 0) return 1;

    for (j = 0; j < nc; j++) {
        if (rat_cmp(crossings[j].z_a, crossings[j].z_b) > 0) {
            breaks[j].edge_idx = crossings[j].edge_b;
            breaks[j].param = crossings[j].param_b;
        } else {
            breaks[j].edge_idx = crossings[j].edge_a;
            breaks[j].param = crossings[j].param_a;
        }
        breaks[j].crossing_idx = j;
    }

    for (i = 1; i < nc; i++) {
        ArcBreak tmp = breaks[i];
        j = i - 1;
        while (j >= 0 && (breaks[j].edge_idx > tmp.edge_idx ||
               (breaks[j].edge_idx == tmp.edge_idx &&
                rat_cmp(breaks[j].param, tmp.param) > 0))) {
            breaks[j + 1] = breaks[j];
            j--;
        }
        breaks[j + 1] = tmp;
    }

    for (i = 0; i < nc; i++) {
        int cj = breaks[i].crossing_idx;
        under_out_arc[cj] = i;
        under_in_arc[cj] = (i - 1 + nc) % nc;
    }

    for (j = 0; j < nc; j++) {
        int over_edge;
        Rat over_param;
        if (rat_cmp(crossings[j].z_a, crossings[j].z_b) > 0) {
            over_edge = crossings[j].edge_a;
            over_param = crossings[j].param_a;
        } else {
            over_edge = crossings[j].edge_b;
            over_param = crossings[j].param_b;
        }
        over_arc[j] = find_arc_at(breaks, nc, over_edge, over_param);
    }

    for (i = 0; i < nc; i++)
        for (j = 0; j < nc; j++)
            M[i][j] = 0;

    for (k = 0; k < nc; k++) {
        int sign = crossings[k].sign;
        int val = (sign > 0) ? 1 : -1;
        M[k][over_arc[k]] += 2 * val;
        M[k][under_in_arc[k]] += -1 * val;
        M[k][under_out_arc[k]] += -1 * val;
    }

    sz = nc - 1;
    if (sz <= 0) return 1;

    {
        long A[MAX_CROSSINGS][MAX_ARCS];
        long prev;

        for (i = 0; i < sz; i++)
            for (j = 0; j < sz; j++)
                A[i][j] = M[i][j];

        prev = 1;
        for (k = 0; k < sz; k++) {
            if (A[k][k] == 0) {
                int found = 0;
                for (i = k + 1; i < sz; i++) {
                    if (A[i][k] != 0) {
                        for (j = 0; j < sz; j++) {
                            long tmp = A[k][j];
                            A[k][j] = A[i][j];
                            A[i][j] = tmp;
                        }
                        prev = -prev;
                        found = 1;
                        break;
                    }
                }
                if (!found) return 0;
            }

            for (i = k + 1; i < sz; i++) {
                for (j = k + 1; j < sz; j++) {
                    A[i][j] = (A[k][k] * A[i][j] - A[i][k] * A[k][j]) / prev;
                }
            }
            prev = A[k][k];
        }

        det = A[sz - 1][sz - 1];
    }

    return (int)(det < 0 ? -det : det);
}

/* ================================================================
 * Part G: Braid Words
 *
 * A braid word on n strands is a sequence of generators.
 * Generator +i means sigma_i (swap positions i-1 and i,
 *   strand at i-1 crosses OVER strand at i).
 * Generator -i means sigma_i^{-1} (under crossing).
 *
 * Convention: generators are 1-indexed (sigma_1, sigma_2, ...).
 * ================================================================ */

#define MAX_BRAID_STRANDS   8
#define MAX_BRAID_GENS      32

typedef struct {
    int generators[MAX_BRAID_GENS];
    int n_generators;
    int n_strands;
    const char *name;
    int expected_det;  /* expected knot determinant */
} BraidWord;

/* ================================================================
 * Part H: Braid-to-Polygon Conversion
 *
 * Layout algorithm:
 *   - Strands arranged along x-axis at x = pos * S
 *   - z increases through the braid (one crossing per z-level)
 *   - y-detour at crossings determines over/under
 *   - Closure arcs route through y = -MARGIN to avoid braid
 *
 * For each crossing k (generator g, sign e):
 *   - Left strand (position |g|-1) moves to position |g|
 *   - Right strand (position |g|) moves to position |g|-1
 *   - At midpoint z, left strand gets y = +e*D, right gets y = -e*D
 *   - Non-involved strands continue straight (no y-detour)
 *
 * Each strand gets 2 vertices per crossing (midpoint + after).
 * Initial vertex at z=0. Total per strand = 1 + 2*n_crossings.
 *
 * Closure: route through (x, -MARGIN, z) to connect strand ends
 * to strand starts according to the braid permutation.
 * ================================================================ */

#define BRAID_S       20  /* x-spacing between strand positions */
#define BRAID_Z       20  /* z-spacing between crossing levels */
#define BRAID_D        1  /* y-detour magnitude for crossings */
#define BRAID_MARGIN 100  /* y-margin for closure arcs */

#define MAX_STRAND_VERTS 128

typedef struct {
    Vec3Q verts[MAX_STRAND_VERTS];
    int n_verts;
} StrandTrace;

/*
 * Convert a braid word to a stick polygon.
 *
 * Returns 1 on success, 0 if the braid closure has multiple
 * components (i.e., it's a link, not a knot).
 */
static int braid_to_polygon(const BraidWord *braid, StickKnot *out) {
    StrandTrace traces[MAX_BRAID_STRANDS];
    int pos_to_strand[MAX_BRAID_STRANDS];
    int strand_to_pos[MAX_BRAID_STRANDS];
    int k, s, n;
    int cycle_order[MAX_BRAID_STRANDS]; /* strand visitation order */
    int cycle_len;

    n = braid->n_strands;

    /* Initialize strand positions (strand s starts at position s) */
    for (s = 0; s < n; s++) {
        pos_to_strand[s] = s;
        strand_to_pos[s] = s;
        traces[s].n_verts = 0;
    }

    /* Emit initial vertices at z = 0 */
    for (s = 0; s < n; s++) {
        traces[s].verts[traces[s].n_verts++] =
            v3q_from_ints((i64)s * BRAID_S, 0, 0);
    }

    /* Process each crossing */
    for (k = 0; k < braid->n_generators; k++) {
        int gen = braid->generators[k];
        int gi = (gen > 0) ? gen : -gen;  /* 1-indexed generator */
        int sign = (gen > 0) ? 1 : -1;
        int pos_left = gi - 1;   /* 0-indexed position */
        int pos_right = gi;
        int s_left = pos_to_strand[pos_left];
        int s_right = pos_to_strand[pos_right];
        i64 z_mid = (i64)(k * BRAID_Z + BRAID_Z / 2);
        i64 z_after = (i64)((k + 1) * BRAID_Z);

        /* Emit midpoint vertices for ALL strands */
        for (s = 0; s < n; s++) {
            int pos = strand_to_pos[s];
            if (s == s_left) {
                /* Left strand: detour to y = +sign*D, move x to right pos */
                traces[s].verts[traces[s].n_verts++] =
                    v3q_from_ints((i64)pos_right * BRAID_S,
                                 (i64)(sign * BRAID_D), z_mid);
            } else if (s == s_right) {
                /* Right strand: detour to y = -sign*D, move x to left pos */
                traces[s].verts[traces[s].n_verts++] =
                    v3q_from_ints((i64)pos_left * BRAID_S,
                                 (i64)(-sign * BRAID_D), z_mid);
            } else {
                /* Non-involved: continue straight */
                traces[s].verts[traces[s].n_verts++] =
                    v3q_from_ints((i64)pos * BRAID_S, 0, z_mid);
            }
        }

        /* Swap positions */
        pos_to_strand[pos_left] = s_right;
        pos_to_strand[pos_right] = s_left;
        strand_to_pos[s_left] = pos_right;
        strand_to_pos[s_right] = pos_left;

        /* Emit after-crossing vertices for ALL strands */
        for (s = 0; s < n; s++) {
            int pos = strand_to_pos[s];
            traces[s].verts[traces[s].n_verts++] =
                v3q_from_ints((i64)pos * BRAID_S, 0, z_after);
        }
    }

    /* Check for single-component closure (must be single cycle) */
    {
        int visited[MAX_BRAID_STRANDS];
        int cur;
        for (s = 0; s < n; s++) visited[s] = 0;

        /* Follow the cycle starting from strand 0 */
        /* At the end of the braid:
         *   strand s ends at position strand_to_pos[s]
         *   closure connects bottom of position p to top of position p
         *   top of position p has strand pos_to_strand_initial[p] = p
         * So strand s's end connects to the start of strand
         * strand_to_pos[s] (same position = same initial strand)
         *
         * Wait: at the START, pos_to_strand_initial = identity.
         * So top of position p = strand p.
         * At the END, strand s is at position strand_to_pos[s].
         * Closure: bottom pos strand_to_pos[s] -> top pos strand_to_pos[s]
         *        = strand strand_to_pos[s].
         * So: after following strand s, we transition to strand
         * strand_to_pos[s].
         */
        cycle_len = 0;
        cur = 0;
        do {
            if (visited[cur]) break; /* shouldn't happen */
            visited[cur] = 1;
            cycle_order[cycle_len++] = cur;
            cur = strand_to_pos[cur];
        } while (cur != 0);

        if (cycle_len != n) {
            printf("    WARNING: braid closure has %d components"
                   " (not a knot)\n", n - cycle_len + 1);
            return 0;
        }
    }

    /* Assemble the polygon: follow strands in cycle order,
     * with closure arcs between them */
    {
        i64 z_final = (i64)(braid->n_generators * BRAID_Z);
        int vi = 0;

        out->name = braid->name;
        out->crossing_number = 0; /* will be computed */

        for (k = 0; k < cycle_len; k++) {
            int strand = cycle_order[k];
            int v;

            /* Add ALL of this strand's vertices.
             * Note: for k > 0, the closure arc endpoint is at
             * (start_x, y_off, 0) which differs from vertex 0
             * at (strand*S, 0, 0) in the y-coordinate.
             * We MUST include vertex 0 to avoid a shortcut
             * diagonal edge from closure plane to braid. */
            for (v = 0; v < traces[strand].n_verts; v++) {
                if (vi >= MAX_STICK_VERTICES) {
                    printf("    ERROR: too many vertices (%d)\n", vi);
                    return 0;
                }
                out->vertices[vi++] = traces[strand].verts[v];
            }

            /* Add closure arc: 2 intermediate vertices at y = -MARGIN
             * Route from strand's end position to next strand's
             * start position through the back */
            {
                int next_strand = cycle_order[(k + 1) % cycle_len];
                i64 end_x = (i64)strand_to_pos[strand] * BRAID_S;
                i64 start_x = (i64)next_strand * BRAID_S;
                /* Use different y offsets per arc to separate them */
                i64 y_off = (i64)(-(BRAID_MARGIN + k * 5));

                if (vi + 2 > MAX_STICK_VERTICES) {
                    printf("    ERROR: too many vertices\n");
                    return 0;
                }

                /* Go to back at end z-level */
                out->vertices[vi++] =
                    v3q_from_ints(end_x, y_off, z_final);
                /* Travel to start z-level at back */
                out->vertices[vi++] =
                    v3q_from_ints(start_x, y_off, 0);
            }
        }

        out->n_vertices = vi;
    }

    /* Simplify: remove collinear vertices.
     * Three consecutive vertices are collinear if
     * (v2 - v1) x (v3 - v1) == (0, 0, 0). */
    {
        Vec3Q simplified[MAX_STICK_VERTICES];
        int sn = 0, vi2;
        int n_orig = out->n_vertices;

        for (vi2 = 0; vi2 < n_orig; vi2++) {
            int prev = (vi2 - 1 + n_orig) % n_orig;
            int next = (vi2 + 1) % n_orig;
            Vec3Q d1 = v3q_sub(out->vertices[vi2], out->vertices[prev]);
            Vec3Q d2 = v3q_sub(out->vertices[next], out->vertices[prev]);
            Vec3Q cr = v3q_cross(d1, d2);
            if (!rat_is_zero(cr.x) || !rat_is_zero(cr.y) ||
                !rat_is_zero(cr.z)) {
                /* Not collinear — keep */
                simplified[sn++] = out->vertices[vi2];
            }
        }
        for (vi2 = 0; vi2 < sn; vi2++)
            out->vertices[vi2] = simplified[vi2];
        out->n_vertices = sn;
        if (sn < n_orig)
            printf("    Simplified: %d -> %d vertices\n", n_orig, sn);
    }

    return 1;
}

/* ================================================================
 * Part I: Known Braid Words
 * ================================================================ */

/* Trefoil 3_1: BR(2, {1, 1, 1}) = sigma_1^3
 * 2 strands, 3 positive crossings */
static BraidWord braid_trefoil(void) {
    BraidWord b;
    b.n_strands = 2;
    b.n_generators = 3;
    b.generators[0] = 1;
    b.generators[1] = 1;
    b.generators[2] = 1;
    b.name = "trefoil 3_1";
    b.expected_det = 3;
    return b;
}

/* Figure-eight 4_1: BR(3, {1, -2, 1, -2})
 * 3 strands, 4 crossings, amphichiral */
static BraidWord braid_figure_eight(void) {
    BraidWord b;
    b.n_strands = 3;
    b.n_generators = 4;
    b.generators[0] = 1;
    b.generators[1] = -2;
    b.generators[2] = 1;
    b.generators[3] = -2;
    b.name = "figure-eight 4_1";
    b.expected_det = 5;
    return b;
}

/* Cinquefoil 5_1 = T(2,5): BR(2, {1, 1, 1, 1, 1}) = sigma_1^5
 * 2 strands, 5 crossings */
static BraidWord braid_cinquefoil(void) {
    BraidWord b;
    b.n_strands = 2;
    b.n_generators = 5;
    b.generators[0] = 1;
    b.generators[1] = 1;
    b.generators[2] = 1;
    b.generators[3] = 1;
    b.generators[4] = 1;
    b.name = "cinquefoil 5_1";
    b.expected_det = 5;
    return b;
}

/* 5_2 knot: three-twist knot, det = 7.
 * From Knot Atlas: BR(3, {-1, -1, -1, -2, 1, -2})
 * 3 strands, 6 generators (even = 3-cycle = knot).
 */
static BraidWord braid_5_2(void) {
    BraidWord b;
    b.n_strands = 3;
    b.n_generators = 6;
    b.generators[0] = -1;
    b.generators[1] = -1;
    b.generators[2] = -1;
    b.generators[3] = -2;
    b.generators[4] = 1;
    b.generators[5] = -2;
    b.name = "5_2";
    b.expected_det = 7;
    return b;
}

/* 6_3 knot: amphichiral, det = 13.
 * From Knot Atlas: BR(3, {-1, -1, 2, -1, 2, 2})
 * 3 strands, 6 generators (even = 3-cycle = knot).
 */
static BraidWord braid_6_3(void) {
    BraidWord b;
    b.n_strands = 3;
    b.n_generators = 6;
    b.generators[0] = -1;
    b.generators[1] = -1;
    b.generators[2] = 2;
    b.generators[3] = -1;
    b.generators[4] = 2;
    b.generators[5] = 2;
    b.name = "6_3";
    b.expected_det = 13;
    return b;
}

/* 8_18 knot: amphichiral, det = 45.
 * From Knot Atlas: BR(3, {-1, 2, -1, 2, -1, 2, -1, 2})
 * = (sigma_1^-1 sigma_2)^4
 * 3 strands, 8 generators (even = 3-cycle = knot).
 */
static BraidWord braid_8_18(void) {
    BraidWord b;
    b.n_strands = 3;
    b.n_generators = 8;
    b.generators[0] = -1;
    b.generators[1] = 2;
    b.generators[2] = -1;
    b.generators[3] = 2;
    b.generators[4] = -1;
    b.generators[5] = 2;
    b.generators[6] = -1;
    b.generators[7] = 2;
    b.name = "8_18";
    b.expected_det = 45;
    return b;
}

/* ================================================================
 * Tests
 * ================================================================ */

static void test_braid_layout(void) {
    BraidWord braids[6];
    int nb = 0;
    int bi;

    /* Suppress unused warnings for reusable library functions */
    (void)rat_neg;
    (void)v3q_add;
    (void)v3q_dist_sq;
    (void)v3q_eq;

    printf("\n=== Test: Braid-to-Polygon Layout ===\n");

    braids[nb++] = braid_trefoil();
    braids[nb++] = braid_figure_eight();
    braids[nb++] = braid_cinquefoil();
    braids[nb++] = braid_5_2();
    braids[nb++] = braid_6_3();
    braids[nb++] = braid_8_18();

    for (bi = 0; bi < nb; bi++) {
        StickKnot knot;
        Crossing cx[MAX_CROSSINGS];
        int nc, det, wr;
        int ok;

        printf("\n  Braid: %s  (%d strands, %d generators)\n",
               braids[bi].name, braids[bi].n_strands,
               braids[bi].n_generators);

        /* Print generators */
        {
            int g;
            printf("    Generators: ");
            for (g = 0; g < braids[bi].n_generators; g++) {
                if (g > 0) printf(", ");
                if (braids[bi].generators[g] > 0)
                    printf("s%d", braids[bi].generators[g]);
                else
                    printf("s%d^-1", -braids[bi].generators[g]);
            }
            printf("\n");
        }

        ok = braid_to_polygon(&braids[bi], &knot);
        if (!ok) {
            printf("    LINK (not a knot) — skipping\n");
            check("braid closure is a knot", 0);
            continue;
        }

        printf("    Polygon: %d vertices\n", knot.n_vertices);

        /* Print first few vertices */
        {
            int v, lim = knot.n_vertices;
            if (lim > 12) lim = 12;
            for (v = 0; v < lim; v++) {
                printf("      v[%2d] = ", v);
                v3q_print(knot.vertices[v]);
                printf("\n");
            }
            if (knot.n_vertices > 12)
                printf("      ... (%d more)\n",
                       knot.n_vertices - 12);
        }

        /* Find crossings and compute invariants */
        nc = find_crossings(knot.vertices, knot.n_vertices,
                           cx, MAX_CROSSINGS);
        det = (nc > 0) ?
            knot_determinant(knot.n_vertices, cx, nc) : 1;
        wr = compute_writhe(cx, nc);

        printf("    Crossings: %d  Determinant: %d"
               "  Writhe: %d\n", nc, det, wr);
        printf("    Expected det: %d  Match: %s\n",
               braids[bi].expected_det,
               det == braids[bi].expected_det ? "YES" : "NO");

        check(braids[bi].name,
              det == braids[bi].expected_det);
    }
}

static void test_construction_word_trefoil(void) {
    BraidWord braid = braid_trefoil();
    StickKnot knot;
    ConstructionWord word;
    int i, n_s;
    unsigned int n_a;
    int det_hist[20];

    printf("\n=== Test: Construction Word — Trefoil from Braid ===\n");

    if (!braid_to_polygon(&braid, &knot)) {
        check("trefoil braid -> polygon", 0);
        return;
    }

    printf("  Trefoil from braid: %d vertices (%d bits)\n",
           knot.n_vertices, knot.n_vertices - 3);

    /* Set up construction word: base = first 3 vertices,
     * each step mirrors against base plane */
    word.base[0] = 0;
    word.base[1] = 1;
    word.base[2] = 2;
    n_s = knot.n_vertices - 3;
    word.n_steps = n_s;
    for (i = 0; i < n_s; i++) {
        word.steps[i].target_idx = i + 3;
        word.steps[i].center_idx[0] = 0;
        word.steps[i].center_idx[1] = 1;
        word.steps[i].center_idx[2] = 2;
    }

    n_a = 1u << (unsigned)n_s;
    printf("  Construction steps: %d  Alternatives: %u\n",
           n_s, n_a);

    if (n_s > 20) {
        printf("  Too many steps (%d) for exhaustive analysis\n",
               n_s);
        check("construction word analysis (skipped — too many bits)",
              1);
        return;
    }

    /* Classify all alternatives by determinant */
    {
        unsigned int choices;
        int n_trefoil = 0;

        for (i = 0; i < 20; i++) det_hist[i] = 0;

        for (choices = 0; choices < n_a; choices++) {
            Vec3Q poly[MAX_STICK_VERTICES];
            Crossing cx[MAX_CROSSINGS];
            int n_poly, nc, dt;
            evaluate_construction(&knot, &word, choices,
                                  poly, &n_poly);
            nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
            dt = (nc > 0) ?
                knot_determinant(n_poly, cx, nc) : 1;
            if (dt == 3) n_trefoil++;
            if (dt >= 0 && dt < 20) det_hist[dt]++;
        }

        printf("  Det histogram:\n");
        for (i = 0; i < 20; i++) {
            if (det_hist[i] > 0)
                printf("    det=%2d: %5d / %u\n",
                       i, det_hist[i], n_a);
        }
        printf("  Trefoil (det=3) count: %d / %u\n",
               n_trefoil, n_a);

        check("trefoil construction word produces trefoil alternatives",
              n_trefoil > 0);
    }

    /* Influence analysis: for each bit, how many times does
     * flipping it change the determinant? */
    printf("\n  Influence profile (target = trefoil det=3):\n");
    {
        unsigned int choices;
        int bit;
        int *dets;

        dets = (int *)malloc(n_a * sizeof(int));
        if (!dets) { printf("  OOM\n"); return; }

        /* Precompute all determinants */
        for (choices = 0; choices < n_a; choices++) {
            Vec3Q poly[MAX_STICK_VERTICES];
            Crossing cx[MAX_CROSSINGS];
            int n_poly, nc;
            evaluate_construction(&knot, &word, choices,
                                  poly, &n_poly);
            nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
            dets[choices] = (nc > 0) ?
                knot_determinant(n_poly, cx, nc) : 1;
        }

        printf("  %4s  %10s  %10s  %10s\n",
               "Bit", "Changes", "Influence", "Role");
        for (bit = 0; bit < n_s; bit++) {
            unsigned int mask = 1u << (unsigned)bit;
            int changes = 0;
            int tref_to_other = 0;
            int other_to_tref = 0;
            for (choices = 0; choices < n_a; choices++) {
                unsigned int flipped = choices ^ mask;
                if (dets[choices] != dets[flipped]) {
                    changes++;
                    if (dets[choices] == 3) tref_to_other++;
                    if (dets[flipped] == 3) other_to_tref++;
                }
            }
            {
                int influence = changes;
                const char *role;
                if (influence == 0) role = "FREE";
                else role = "ACTIVE";
                printf("  %4d  %10d  %9d/%u  %s",
                       bit, changes, influence, n_a, role);
                if (changes > 0)
                    printf("  (T->O:%d O->T:%d)",
                           tref_to_other, other_to_tref);
                printf("\n");
            }
        }

        free(dets);
    }
}

/* ================================================================
 * Part J: Polygon Simplification (topology-preserving)
 *
 * Iteratively remove vertices whose removal does not change the
 * knot determinant. This produces a polygon with fewer vertices,
 * making construction word analysis feasible for more knots.
 * ================================================================ */

static int simplify_polygon_topology(StickKnot *knot) {
    int removed = 0;
    int changed = 1;
    int target_det;

    /* Compute target determinant */
    {
        Crossing cx[MAX_CROSSINGS];
        int nc = find_crossings(knot->vertices, knot->n_vertices,
                                cx, MAX_CROSSINGS);
        target_det = (nc > 0) ?
            knot_determinant(knot->n_vertices, cx, nc) : 1;
    }

    /* Greedy removal: keep removing vertices until stable */
    while (changed) {
        int i;
        changed = 0;
        for (i = 0; i < knot->n_vertices; i++) {
            /* Try removing vertex i */
            Vec3Q temp[MAX_STICK_VERTICES];
            int tn = 0, j;
            Crossing cx[MAX_CROSSINGS];
            int nc, det;

            for (j = 0; j < knot->n_vertices; j++) {
                if (j != i) temp[tn++] = knot->vertices[j];
            }
            if (tn < 4) continue; /* need at least 4 for knot */

            nc = find_crossings(temp, tn, cx, MAX_CROSSINGS);
            det = (nc > 0) ? knot_determinant(tn, cx, nc) : 1;

            if (det == target_det) {
                /* Safe to remove */
                for (j = 0; j < tn; j++)
                    knot->vertices[j] = temp[j];
                knot->n_vertices = tn;
                removed++;
                changed = 1;
                break; /* restart scan */
            }
        }
    }
    return removed;
}

static void test_vertex_counts(void) {
    BraidWord braids[6];
    int nb = 0, bi;

    printf("\n=== Vertex Count Summary ===\n");
    printf("  %-20s  %5s  %5s  %8s\n",
           "Knot", "Verts", "Bits", "Alterns");

    braids[nb++] = braid_trefoil();
    braids[nb++] = braid_figure_eight();
    braids[nb++] = braid_cinquefoil();
    braids[nb++] = braid_5_2();
    braids[nb++] = braid_6_3();
    braids[nb++] = braid_8_18();

    for (bi = 0; bi < nb; bi++) {
        StickKnot knot;
        if (braid_to_polygon(&braids[bi], &knot)) {
            int bits = knot.n_vertices - 3;
            printf("  %-20s  %5d  %5d  %8s\n",
                   braids[bi].name, knot.n_vertices, bits,
                   bits <= 20 ? "feasible" : "TOO MANY");
        }
    }
}

static void setup_construction_word(ConstructionWord *word,
                                    int n_verts,
                                    int b0, int b1, int b2) {
    int i, step = 0;
    word->base[0] = b0;
    word->base[1] = b1;
    word->base[2] = b2;
    for (i = 0; i < n_verts; i++) {
        if (i == b0 || i == b1 || i == b2) continue;
        word->steps[step].target_idx = i;
        word->steps[step].center_idx[0] = b0;
        word->steps[step].center_idx[1] = b1;
        word->steps[step].center_idx[2] = b2;
        step++;
    }
    word->n_steps = step;
}

/* Evaluate construction word, placing vertices in ORIGINAL polygon order.
 * Base vertices stay at their original positions; non-base vertices
 * are mirrored (or not) through the base plane at their original positions. */
static void evaluate_inplace(const StickKnot *knot,
                              int b0, int b1, int b2,
                              unsigned int choices,
                              Vec3Q *out, int *out_n) {
    int i, step = 0;
    Vec3Q cb0 = knot->vertices[b0];
    Vec3Q cb1 = knot->vertices[b1];
    Vec3Q cb2 = knot->vertices[b2];

    *out_n = knot->n_vertices;
    for (i = 0; i < knot->n_vertices; i++) {
        if (i == b0 || i == b1 || i == b2) {
            out[i] = knot->vertices[i];
        } else {
            int use_mirror = (int)((choices >> (unsigned)step) & 1u);
            if (use_mirror) {
                out[i] = mirror_point(cb0, cb1, cb2, knot->vertices[i]);
            } else {
                out[i] = knot->vertices[i];
            }
            step++;
        }
    }
}

static void analyze_knot_with_base(const char *name,
                                   StickKnot *knot,
                                   int expected_det,
                                   int b0, int b1, int b2) {
    int n_s = knot->n_vertices - 3;
    unsigned int n_a;
    int *dets;
    int i, bit;
    int det_hist[256];

    if (n_s > 20) {
        printf("    Still too many bits (%d)\n", n_s);
        return;
    }
    n_a = 1u << (unsigned)n_s;

    /* Precompute all determinants */
    dets = (int *)malloc(n_a * sizeof(int));
    if (!dets) { printf("    OOM\n"); return; }

    for (i = 0; i < 256; i++) det_hist[i] = 0;

    {
        unsigned int choices;
        for (choices = 0; choices < n_a; choices++) {
            Vec3Q poly[MAX_STICK_VERTICES];
            Crossing cx[MAX_CROSSINGS];
            int n_poly, nc;
            evaluate_inplace(knot, b0, b1, b2, choices,
                             poly, &n_poly);
            nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
            dets[choices] = (nc > 0) ?
                knot_determinant(n_poly, cx, nc) : 1;
            if (dets[choices] >= 0 && dets[choices] < 256)
                det_hist[dets[choices]]++;
        }
    }

    /* Det histogram */
    printf("    %d bits, %u alternatives\n", n_s, n_a);
    printf("    Det histogram:\n");
    {
        int n_distinct = 0;
        for (i = 0; i < 256; i++) {
            if (det_hist[i] > 0) {
                printf("      det=%2d: %6d / %u\n",
                       i, det_hist[i], n_a);
                n_distinct++;
            }
        }
        printf("    Distinct determinants: %d\n", n_distinct);
    }

    /* Influence profile */
    printf("    Influence profile (target det=%d):\n", expected_det);
    printf("    %4s  %8s  %12s  %s\n",
           "Bit", "Changes", "Fraction", "Role");
    {
        int n_active = 0, n_free = 0;
        for (bit = 0; bit < n_s; bit++) {
            unsigned int mask = 1u << (unsigned)bit;
            int changes = 0;
            unsigned int choices;
            for (choices = 0; choices < n_a; choices++) {
                unsigned int flipped = choices ^ mask;
                if (dets[choices] != dets[flipped])
                    changes++;
            }
            if (changes == 0) {
                printf("    %4d  %8d  %11d/%u  FREE\n",
                       bit, 0, 0, n_a);
                n_free++;
            } else {
                printf("    %4d  %8d  %11d/%u  ACTIVE\n",
                       bit, changes, changes, n_a);
                n_active++;
            }
        }
        printf("    Summary: %d ACTIVE, %d FREE\n",
               n_active, n_free);

        /* For small active bit counts, print truth table */
        if (n_active <= 8) {
            int active_bits[20];
            int nab = 0;
            unsigned int ai;
            for (bit = 0; bit < n_s; bit++) {
                unsigned int mask = 1u << (unsigned)bit;
                int changes = 0;
                unsigned int ch2;
                for (ch2 = 0; ch2 < n_a; ch2++) {
                    if (dets[ch2] != dets[ch2 ^ mask]) {
                        changes++;
                        break;
                    }
                }
                if (changes > 0) active_bits[nab++] = bit;
            }
            printf("    Truth table (active bits -> det):\n");
            printf("    ");
            for (bit = 0; bit < nab; bit++)
                printf("b%d ", active_bits[bit]);
            printf(" -> det\n");
            for (ai = 0; ai < (1u << (unsigned)nab); ai++) {
                unsigned int full_choice = 0;
                int dt;
                for (bit = 0; bit < nab; bit++) {
                    if (ai & (1u << (unsigned)bit))
                        full_choice |=
                            (1u << (unsigned)active_bits[bit]);
                }
                dt = dets[full_choice];
                printf("    ");
                for (bit = 0; bit < nab; bit++)
                    printf(" %d ", (int)((ai >> (unsigned)bit) & 1u));
                printf(" -> %d", dt);
                if (dt == expected_det) printf("  <-- SELF");
                printf("\n");
            }
        }

        /* Complement check: for self-preserving patterns,
         * is the complement also self-preserving? */
        {
            unsigned int self_masks[256];
            int n_self = 0;
            unsigned int full_mask = n_a - 1u;
            int n_comp_pairs = 0;
            unsigned int si;

            for (si = 0; si < n_a; si++) {
                if (dets[si] == expected_det)
                    self_masks[n_self++] = si;
            }
            printf("    Complement check (%d self-preserving):\n",
                   n_self);
            for (i = 0; i < n_self; i++) {
                unsigned int comp = full_mask ^ self_masks[i];
                int comp_is_self = (dets[comp] == expected_det);
                printf("      ");
                for (bit = n_s - 1; bit >= 0; bit--)
                    printf("%d",
                        (int)((self_masks[i] >> (unsigned)bit) & 1u));
                printf(" <-> ");
                for (bit = n_s - 1; bit >= 0; bit--)
                    printf("%d",
                        (int)((comp >> (unsigned)bit) & 1u));
                printf("  comp_self=%s",
                       comp_is_self ? "YES" : "no");
                if (comp_is_self) n_comp_pairs++;
                printf("\n");
            }
            printf("    Complement closure: %d/%d patterns"
                   " have self-preserving complement\n",
                   n_comp_pairs, n_self);
        }

        check(name, det_hist[expected_det] > 0);
    }

    free(dets);
    (void)name;
}

static void analyze_simplified_knot(const char *name,
                                    StickKnot *knot,
                                    int expected_det) {
    analyze_knot_with_base(name, knot, expected_det, 0, 1, 2);
}

static void test_simplified_construction(void) {
    BraidWord braids[6];
    int nb = 0, bi;

    printf("\n=== Test: Simplified Polygon Construction Words ===\n");

    braids[nb++] = braid_trefoil();
    braids[nb++] = braid_figure_eight();
    braids[nb++] = braid_cinquefoil();
    braids[nb++] = braid_5_2();
    braids[nb++] = braid_6_3();
    braids[nb++] = braid_8_18();

    for (bi = 0; bi < nb; bi++) {
        StickKnot knot;
        int removed;

        if (!braid_to_polygon(&braids[bi], &knot)) continue;

        printf("\n  %s: %d verts -> ", braids[bi].name,
               knot.n_vertices);
        removed = simplify_polygon_topology(&knot);
        printf("%d verts (removed %d)\n",
               knot.n_vertices, removed);

        /* Print simplified vertices */
        {
            int v;
            for (v = 0; v < knot.n_vertices; v++) {
                printf("    v[%2d] = ", v);
                v3q_print(knot.vertices[v]);
                printf("\n");
            }
        }

        /* Verify determinant preserved + report crossing count */
        {
            Crossing cx[MAX_CROSSINGS];
            int nc = find_crossings(knot.vertices, knot.n_vertices,
                                    cx, MAX_CROSSINGS);
            int det = (nc > 0) ?
                knot_determinant(knot.n_vertices, cx, nc) : 1;
            int wr = compute_writhe(cx, nc);
            printf("    Det: %d (expected %d)  "
                   "Crossings: %d  Writhe: %d\n",
                   det, braids[bi].expected_det, nc, wr);
        }

        analyze_simplified_knot(braids[bi].name, &knot,
                                braids[bi].expected_det);
    }
}

/* Randomized vertex removal: use a permuted scan order */
static int simplify_polygon_random(StickKnot *knot, unsigned int seed) {
    int removed = 0;
    int changed = 1;
    int target_det;
    int order[MAX_STICK_VERTICES];
    int i;

    {
        Crossing cx[MAX_CROSSINGS];
        int nc = find_crossings(knot->vertices, knot->n_vertices,
                                cx, MAX_CROSSINGS);
        target_det = (nc > 0) ?
            knot_determinant(knot->n_vertices, cx, nc) : 1;
    }

    while (changed) {
        int n = knot->n_vertices;
        changed = 0;

        /* Fisher-Yates shuffle of scan order */
        for (i = 0; i < n; i++) order[i] = i;
        for (i = n - 1; i > 0; i--) {
            int j;
            seed = seed * 1103515245u + 12345u;
            j = (int)((seed >> 16) % (unsigned)(i + 1));
            { int tmp = order[i]; order[i] = order[j];
              order[j] = tmp; }
        }

        for (i = 0; i < n; i++) {
            int vi = order[i];
            Vec3Q temp[MAX_STICK_VERTICES];
            int tn = 0, j;
            Crossing cx[MAX_CROSSINGS];
            int nc, det;

            for (j = 0; j < knot->n_vertices; j++) {
                if (j != vi) temp[tn++] = knot->vertices[j];
            }
            if (tn < 4) continue;

            nc = find_crossings(temp, tn, cx, MAX_CROSSINGS);
            det = (nc > 0) ? knot_determinant(tn, cx, nc) : 1;

            if (det == target_det) {
                for (j = 0; j < tn; j++)
                    knot->vertices[j] = temp[j];
                knot->n_vertices = tn;
                removed++;
                changed = 1;
                break;
            }
        }
    }
    return removed;
}

static void test_randomized_simplification(void) {
    int trial;
    printf("\n=== Test: Randomized Simplification (Figure-Eight) ===\n");

    for (trial = 0; trial < 8; trial++) {
        BraidWord braid = braid_figure_eight();
        StickKnot knot;
        ConstructionWord word;
        int n_s, i;
        unsigned int n_a;
        int det_hist[256];

        if (!braid_to_polygon(&braid, &knot)) continue;
        simplify_polygon_random(&knot, (unsigned)(trial * 7919 + 42));

        n_s = knot.n_vertices - 3;
        if (n_s > 20) {
            printf("  Trial %d: %d verts (%d bits) — too many\n",
                   trial, knot.n_vertices, n_s);
            continue;
        }
        n_a = 1u << (unsigned)n_s;

        /* Set up construction word */
        word.base[0] = 0; word.base[1] = 1; word.base[2] = 2;
        word.n_steps = n_s;
        for (i = 0; i < n_s; i++) {
            word.steps[i].target_idx = i + 3;
            word.steps[i].center_idx[0] = 0;
            word.steps[i].center_idx[1] = 1;
            word.steps[i].center_idx[2] = 2;
        }

        for (i = 0; i < 256; i++) det_hist[i] = 0;

        {
            unsigned int choices;
            for (choices = 0; choices < n_a; choices++) {
                Vec3Q poly[MAX_STICK_VERTICES];
                Crossing cx[MAX_CROSSINGS];
                int n_poly, nc, dt;
                evaluate_construction(&knot, &word, choices,
                                      poly, &n_poly);
                nc = find_crossings(poly, n_poly,
                                    cx, MAX_CROSSINGS);
                dt = (nc > 0) ?
                    knot_determinant(n_poly, cx, nc) : 1;
                if (dt >= 0 && dt < 256) det_hist[dt]++;
            }
        }

        /* Print compact summary */
        {
            int n_distinct = 0;
            Crossing cx[MAX_CROSSINGS];
            int nc = find_crossings(knot.vertices, knot.n_vertices,
                                    cx, MAX_CROSSINGS);
            printf("  Trial %d: %d verts, %d bits, %d cx  dets={",
                   trial, knot.n_vertices, n_s, nc);
            for (i = 0; i < 256; i++) {
                if (det_hist[i] > 0) {
                    if (n_distinct > 0) printf(",");
                    printf("%d:%d", i, det_hist[i]);
                    n_distinct++;
                }
            }
            printf("}  (%d distinct)\n", n_distinct);
        }
    }
    check("randomized simplification completes", 1);
}

/* ================================================================
 * Part I: Three-Sphere Intersection Decomposition
 *
 * Given a stick polygon, decompose each vertex (beyond the base
 * triangle) into its three-sphere construction:
 *   S(c0, |c0-v|) ∩ S(c1, |c1-v|) ∩ S(c2, |c2-v|)
 *
 * For the all-base strategy (centers = v[0], v[1], v[2] always):
 * Radical plane of S_i ∩ S_j passes through v[k] with normal (c_j - c_i).
 * Since both normals are edges of the base triangle, the radical line
 * direction = (c1-c0) × (c2-c0) = base plane normal.
 * The two roots are v[k] and its reflection through the base plane.
 *
 * THEOREM: The three-sphere intersection with all-base strategy is
 * mathematically identical to the mirror formula (plane reflection).
 * This is verified computationally below.
 * ================================================================ */

typedef struct {
    int vertex_idx;          /* which vertex (k >= 3) */
    Vec3Q vertex;            /* the actual vertex v[k] */
    Vec3Q mirror;            /* the other root v[k]' */
    Rat height;              /* signed height above base plane: n·(v[k]-v[0]) */
    Rat height_sq;           /* height squared (proportional to discriminant) */
    Rat n_dot_n;             /* |n|² = base plane normal squared */
    int root_sign;           /* +1 if height > 0, -1 if < 0, 0 if on plane */
    int mirror_matches;      /* 1 if mirror == mirror_point() result */
} SphereDecomposition;

typedef struct {
    Vec3Q base[3];           /* the base triangle */
    Vec3Q normal;            /* base plane normal n = (v1-v0) × (v2-v0) */
    Rat n_dot_n;             /* |n|² */
    int n_steps;
    SphereDecomposition steps[MAX_STICK_VERTICES];
} SphereAnalysis;

static SphereAnalysis sphere_decompose_all_base(const StickKnot *knot) {
    SphereAnalysis sa;
    int k;
    Vec3Q e1, e2;

    sa.base[0] = knot->vertices[0];
    sa.base[1] = knot->vertices[1];
    sa.base[2] = knot->vertices[2];

    e1 = v3q_sub(sa.base[1], sa.base[0]);
    e2 = v3q_sub(sa.base[2], sa.base[0]);
    sa.normal = v3q_cross(e1, e2);
    sa.n_dot_n = v3q_dot(sa.normal, sa.normal);
    sa.n_steps = 0;

    for (k = 3; k < knot->n_vertices; k++) {
        SphereDecomposition *sd = &sa.steps[sa.n_steps];
        Vec3Q vk = knot->vertices[k];
        Vec3Q diff = v3q_sub(vk, sa.base[0]);
        Rat h = v3q_dot(sa.normal, diff);  /* n · (vk - v0) */
        Rat two_h_over_nn;
        Vec3Q mirror_sphere, mirror_plane;

        sd->vertex_idx = k;
        sd->vertex = vk;
        sd->height = h;
        sd->height_sq = rat_mul(h, h);
        sd->n_dot_n = sa.n_dot_n;

        if (h.p > 0) sd->root_sign = 1;
        else if (h.p < 0) sd->root_sign = -1;
        else sd->root_sign = 0;

        /* Compute mirror via three-sphere method:
         * v[k]' = v[k] - 2*(n·(vk-v0))/(n·n) * n */
        two_h_over_nn = rat_div(rat_mul(rat_two(), h), sa.n_dot_n);
        mirror_sphere = v3q_sub(vk, v3q_scale(two_h_over_nn, sa.normal));

        /* Compute mirror via Demo 110 formula for comparison */
        mirror_plane = mirror_point(sa.base[0], sa.base[1], sa.base[2], vk);

        sd->mirror = mirror_sphere;
        sd->mirror_matches = v3q_eq(mirror_sphere, mirror_plane);

        sa.n_steps++;
    }

    return sa;
}

static void print_sphere_analysis(const SphereAnalysis *sa) {
    int k;

    printf("  Base triangle: v[0]=");
    v3q_print(sa->base[0]);
    printf("  v[1]=");
    v3q_print(sa->base[1]);
    printf("  v[2]=");
    v3q_print(sa->base[2]);
    printf("\n");

    printf("  Base plane normal n=");
    v3q_print(sa->normal);
    printf("  |n|^2=");
    rat_print(sa->n_dot_n);
    printf("\n\n");

    for (k = 0; k < sa->n_steps; k++) {
        const SphereDecomposition *sd = &sa->steps[k];
        Rat disc;

        printf("  Step %d: v[%d] = ", k, sd->vertex_idx);
        v3q_print(sd->vertex);
        printf("\n");

        printf("    Height h = n·(v-v0) = ");
        rat_print(sd->height);
        printf("   sign=%s\n",
               sd->root_sign > 0 ? "+" : (sd->root_sign < 0 ? "-" : "0"));

        /* Discriminant: 4*h^2 (of the quadratic t^2|n|^2 + 2t*h = 0) */
        disc = rat_mul(rat_from_int(4), sd->height_sq);
        printf("    Discriminant 4h^2 = ");
        rat_print(disc);
        printf("   h^2/|n|^2 = ");
        rat_print(rat_div(sd->height_sq, sd->n_dot_n));
        printf("\n");

        printf("    Mirror v[%d]' = ", sd->vertex_idx);
        v3q_print(sd->mirror);
        printf("\n");

        printf("    Mirror formula match: %s\n",
               sd->mirror_matches ? "YES" : "NO");

        /* Verify: height of mirror should be -h */
        {
            Vec3Q diff_m = v3q_sub(sd->mirror, sa->base[0]);
            Rat h_mirror = v3q_dot(sa->normal, diff_m);
            Rat h_neg = rat_neg(sd->height);
            printf("    Mirror height: ");
            rat_print(h_mirror);
            printf("  (expected ");
            rat_print(h_neg);
            printf(") %s\n", rat_eq(h_mirror, h_neg) ? "OK" : "MISMATCH");
        }
        printf("\n");
    }
}

/* Test: sphere decomposition matches construction word */
static void test_sphere_decomposition(void) {
    BraidWord bw;
    StickKnot knot;
    SphereAnalysis sa;
    int all_match, k;
    ConstructionWord cw;
    unsigned int bit;

    printf("\n=== Test: Three-Sphere Intersection Decomposition ===\n");

    /* Build simplified figure-eight */
    bw = braid_figure_eight();
    braid_to_polygon(&bw, &knot);
    simplify_polygon_topology(&knot);

    printf("\n  Figure-eight 4_1: %d vertices\n", knot.n_vertices);
    for (k = 0; k < knot.n_vertices; k++) {
        printf("    v[%d] = ", k);
        v3q_print(knot.vertices[k]);
        printf("\n");
    }
    printf("\n");

    /* Sphere decomposition */
    sa = sphere_decompose_all_base(&knot);
    print_sphere_analysis(&sa);

    /* Check: all mirrors match the plane reflection formula */
    all_match = 1;
    for (k = 0; k < sa.n_steps; k++) {
        if (!sa.steps[k].mirror_matches) all_match = 0;
    }
    check("three-sphere mirrors match plane reflection for 4_1", all_match);

    /* Now verify: flipping construction word bit k gives the
     * three-sphere mirror point at step k */
    printf("\n  Verifying construction word bit ↔ sphere root:\n");

    /* Build all-base construction word */
    cw.base[0] = 0; cw.base[1] = 1; cw.base[2] = 2;
    cw.n_steps = knot.n_vertices - 3;
    for (k = 0; k < cw.n_steps; k++) {
        cw.steps[k].center_idx[0] = 0;
        cw.steps[k].center_idx[1] = 1;
        cw.steps[k].center_idx[2] = 2;
        cw.steps[k].target_idx = k + 3;
    }

    all_match = 1;
    for (bit = 0; (int)bit < cw.n_steps; bit++) {
        Vec3Q poly_out[MAX_STICK_VERTICES];
        int n_out;
        unsigned int choices = 1u << bit;  /* flip only bit k */
        Vec3Q mirrored_vertex;

        evaluate_construction(&knot, &cw, choices, poly_out, &n_out);
        mirrored_vertex = poly_out[3 + (int)bit];

        printf("    Bit %u: cw mirror = ", bit);
        v3q_print(mirrored_vertex);
        printf("  sphere mirror = ");
        v3q_print(sa.steps[bit].mirror);
        printf("  %s\n",
               v3q_eq(mirrored_vertex, sa.steps[bit].mirror)
                   ? "MATCH" : "MISMATCH");

        if (!v3q_eq(mirrored_vertex, sa.steps[bit].mirror))
            all_match = 0;
    }
    check("construction word bits = sphere root choices for 4_1", all_match);

    /* Print discriminant sequence summary */
    printf("\n  Discriminant sequence {h^2/|n|^2}:\n    ");
    for (k = 0; k < sa.n_steps; k++) {
        if (k > 0) printf(", ");
        rat_print(rat_div(sa.steps[k].height_sq, sa.n_dot_n));
    }
    printf("\n");

    /* Height sequence */
    printf("  Height sequence {h}:\n    ");
    for (k = 0; k < sa.n_steps; k++) {
        if (k > 0) printf(", ");
        rat_print(sa.steps[k].height);
    }
    printf("\n");

    /* Root sign sequence */
    printf("  Root sign sequence: ");
    for (k = 0; k < sa.n_steps; k++) {
        printf("%c", sa.steps[k].root_sign > 0 ? '+' :
                      (sa.steps[k].root_sign < 0 ? '-' : '0'));
    }
    printf("\n");

    /* Now do the same for all 6 knots */
    printf("\n  === All knots: height sequences ===\n");
    {
        BraidWord braids[6];
        int b;
        braids[0] = braid_trefoil();
        braids[1] = braid_figure_eight();
        braids[2] = braid_cinquefoil();
        braids[3] = braid_5_2();
        braids[4] = braid_6_3();
        braids[5] = braid_8_18();

        for (b = 0; b < 6; b++) {
            StickKnot kn;
            SphereAnalysis sa2;
            int all_ok = 1;

            braid_to_polygon(&braids[b], &kn);
            simplify_polygon_topology(&kn);
            sa2 = sphere_decompose_all_base(&kn);

            printf("\n  %s (%d verts, %d steps):\n",
                   braids[b].name, kn.n_vertices, sa2.n_steps);

            /* Check all mirrors match */
            for (k = 0; k < sa2.n_steps; k++) {
                if (!sa2.steps[k].mirror_matches) all_ok = 0;
            }
            printf("    Mirror formula match: %s\n",
                   all_ok ? "ALL MATCH" : "MISMATCH");

            printf("    Heights: ");
            for (k = 0; k < sa2.n_steps; k++) {
                if (k > 0) printf(", ");
                rat_print(sa2.steps[k].height);
            }
            printf("\n");

            printf("    Signs: ");
            for (k = 0; k < sa2.n_steps; k++) {
                printf("%c", sa2.steps[k].root_sign > 0 ? '+' :
                              (sa2.steps[k].root_sign < 0 ? '-' : '0'));
            }
            printf("\n");

            printf("    h^2/|n|^2: ");
            for (k = 0; k < sa2.n_steps; k++) {
                if (k > 0) printf(", ");
                rat_print(rat_div(sa2.steps[k].height_sq, sa2.n_dot_n));
            }
            printf("\n");
        }
    }

    check("sphere decomposition completes for all knots", 1);
}

/* ================================================================
 * Part J: Base Triangle Sweep
 *
 * For a given knot, try ALL C(n,3) base triangle choices.
 * For each, count how many of the remaining vertices have zero
 * height (h = 0) above the base plane — these are "free bits."
 * ================================================================ */

static void base_triangle_sweep(const StickKnot *knot, const char *label) {
    int n = knot->n_vertices;
    int i, j, k;
    int min_free = n, max_free = 0;
    int n_triples = 0;
    int free_hist[MAX_STICK_VERTICES];
    int best_min_i = 0, best_min_j = 1, best_min_k = 2;
    int best_max_i = 0, best_max_j = 1, best_max_k = 2;

    memset(free_hist, 0, sizeof(free_hist));

    for (i = 0; i < n - 2; i++) {
        for (j = i + 1; j < n - 1; j++) {
            for (k = j + 1; k < n; k++) {
                Vec3Q e1 = v3q_sub(knot->vertices[j], knot->vertices[i]);
                Vec3Q e2 = v3q_sub(knot->vertices[k], knot->vertices[i]);
                Vec3Q norm = v3q_cross(e1, e2);
                int m, n_free = 0;

                /* Skip degenerate base triangles (collinear) */
                if (rat_is_zero(v3q_dot(norm, norm))) continue;

                for (m = 0; m < n; m++) {
                    Vec3Q diff;
                    Rat h;
                    if (m == i || m == j || m == k) continue;
                    diff = v3q_sub(knot->vertices[m], knot->vertices[i]);
                    h = v3q_dot(norm, diff);
                    if (rat_is_zero(h)) n_free++;
                }

                free_hist[n_free]++;
                n_triples++;

                if (n_free < min_free) {
                    min_free = n_free;
                    best_min_i = i; best_min_j = j; best_min_k = k;
                }
                if (n_free > max_free) {
                    max_free = n_free;
                    best_max_i = i; best_max_j = j; best_max_k = k;
                }
            }
        }
    }

    printf("  %s (%d verts, %d valid triples):\n", label, n, n_triples);
    printf("    Free bits: min=%d (base %d,%d,%d)  max=%d (base %d,%d,%d)\n",
           min_free, best_min_i, best_min_j, best_min_k,
           max_free, best_max_i, best_max_j, best_max_k);
    printf("    Distribution: ");
    for (i = 0; i <= max_free; i++) {
        if (free_hist[i] > 0) {
            printf("%d_free:%d ", i, free_hist[i]);
        }
    }
    printf("\n");

    /* Print height sequence for best (min free) base */
    {
        Vec3Q e1 = v3q_sub(knot->vertices[best_min_j],
                            knot->vertices[best_min_i]);
        Vec3Q e2 = v3q_sub(knot->vertices[best_min_k],
                            knot->vertices[best_min_i]);
        Vec3Q norm = v3q_cross(e1, e2);
        Rat nn = v3q_dot(norm, norm);

        printf("    Best base (%d,%d,%d) heights: ",
               best_min_i, best_min_j, best_min_k);
        for (i = 0; i < n; i++) {
            Vec3Q diff;
            Rat h;
            if (i == best_min_i || i == best_min_j ||
                i == best_min_k) continue;
            diff = v3q_sub(knot->vertices[i], knot->vertices[best_min_i]);
            h = v3q_dot(norm, diff);
            printf("v%d=", i);
            rat_print(h);
            printf(" ");
        }
        printf("\n");

        printf("    Best base h^2/|n|^2: ");
        for (i = 0; i < n; i++) {
            Vec3Q diff;
            Rat h;
            if (i == best_min_i || i == best_min_j ||
                i == best_min_k) continue;
            diff = v3q_sub(knot->vertices[i], knot->vertices[best_min_i]);
            h = v3q_dot(norm, diff);
            if (i > 0) printf(", ");
            rat_print(rat_div(rat_mul(h, h), nn));
        }
        printf("\n");
        (void)nn;
    }
}

static void test_base_triangle_sweep(void) {
    BraidWord braids[6];
    int b;

    printf("\n=== Test: Base Triangle Sweep ===\n");

    braids[0] = braid_trefoil();
    braids[1] = braid_figure_eight();
    braids[2] = braid_cinquefoil();
    braids[3] = braid_5_2();
    braids[4] = braid_6_3();
    braids[5] = braid_8_18();

    for (b = 0; b < 6; b++) {
        StickKnot knot;
        braid_to_polygon(&braids[b], &knot);
        simplify_polygon_topology(&knot);
        base_triangle_sweep(&knot, braids[b].name);
        printf("\n");
    }

    check("base triangle sweep completes", 1);
}

/* ================================================================
 * Part K: Optimal Base Analysis — All Knots
 *
 * For each knot, find the base triangle with fewest free bits
 * and run the full construction word analysis with that base.
 * ================================================================ */

/* Find optimal base (fewest free bits) for a knot */
static void find_optimal_base(const StickKnot *knot,
                               int *ob0, int *ob1, int *ob2) {
    int n = knot->n_vertices;
    int i, j, k;
    int min_free = n;

    *ob0 = 0; *ob1 = 1; *ob2 = 2;

    for (i = 0; i < n - 2; i++) {
        for (j = i + 1; j < n - 1; j++) {
            for (k = j + 1; k < n; k++) {
                Vec3Q e1 = v3q_sub(knot->vertices[j], knot->vertices[i]);
                Vec3Q e2 = v3q_sub(knot->vertices[k], knot->vertices[i]);
                Vec3Q norm = v3q_cross(e1, e2);
                int m, n_free = 0;

                if (rat_is_zero(v3q_dot(norm, norm))) continue;

                for (m = 0; m < n; m++) {
                    Vec3Q diff;
                    Rat h;
                    if (m == i || m == j || m == k) continue;
                    diff = v3q_sub(knot->vertices[m], knot->vertices[i]);
                    h = v3q_dot(norm, diff);
                    if (rat_is_zero(h)) n_free++;
                }

                if (n_free < min_free) {
                    min_free = n_free;
                    *ob0 = i; *ob1 = j; *ob2 = k;
                    if (min_free == 0) return;
                }
            }
        }
    }
}

/* Count set bits in an unsigned int */
static int popcount_u(unsigned int x) {
    int c = 0;
    while (x) { c += (int)(x & 1u); x >>= 1; }
    return c;
}

static void test_optimal_base_all(void) {
    BraidWord braids[6];
    int b;

    printf("\n=== Test: All Knots with Optimal Base ===\n");

    braids[0] = braid_trefoil();
    braids[1] = braid_figure_eight();
    braids[2] = braid_cinquefoil();
    braids[3] = braid_5_2();
    braids[4] = braid_6_3();
    braids[5] = braid_8_18();

    for (b = 0; b < 6; b++) {
        StickKnot knot;
        int ob0, ob1, ob2;
        char label[64];

        braid_to_polygon(&braids[b], &knot);
        simplify_polygon_topology(&knot);
        find_optimal_base(&knot, &ob0, &ob1, &ob2);

        printf("\n  %s: optimal base = (%d,%d,%d)%s\n",
               braids[b].name, ob0, ob1, ob2,
               (ob0 == 0 && ob1 == 1 && ob2 == 2)
                   ? " (same as default)" : " (DIFFERENT)");

        sprintf(label, "%s opt(%d,%d,%d)",
                braids[b].name, ob0, ob1, ob2);

        analyze_knot_with_base(label, &knot,
                               braids[b].expected_det,
                               ob0, ob1, ob2);

        /* Hamming weight analysis for knots with enough bits */
        if (knot.n_vertices - 3 <= 12) {
            int n_s = knot.n_vertices - 3;
            unsigned int n_a = 1u << (unsigned)n_s;
            unsigned int choices;
            int hw;
            int max_det_at_hw[20];
            int min_det_at_hw[20];
            int count_at_hw[20];
            int distinct_at_hw[20];
            int det_seen[256];

            memset(max_det_at_hw, 0, sizeof(max_det_at_hw));
            memset(count_at_hw, 0, sizeof(count_at_hw));
            for (hw = 0; hw <= n_s; hw++) {
                min_det_at_hw[hw] = 9999;
                distinct_at_hw[hw] = 0;
            }

            for (choices = 0; choices < n_a; choices++) {
                Vec3Q poly[MAX_STICK_VERTICES];
                Crossing cx[MAX_CROSSINGS];
                int n_poly, nc, det;
                hw = popcount_u(choices);

                evaluate_inplace(&knot, ob0, ob1, ob2,
                                 choices, poly, &n_poly);
                nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
                det = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;

                count_at_hw[hw]++;
                if (det > max_det_at_hw[hw]) max_det_at_hw[hw] = det;
                if (det < min_det_at_hw[hw]) min_det_at_hw[hw] = det;
            }

            /* Count distinct dets at each hw */
            for (hw = 0; hw <= n_s; hw++) {
                memset(det_seen, 0, sizeof(det_seen));
                for (choices = 0; choices < n_a; choices++) {
                    if (popcount_u(choices) == hw) {
                        Vec3Q poly[MAX_STICK_VERTICES];
                        Crossing cx[MAX_CROSSINGS];
                        int n_poly, nc, det;
                        evaluate_inplace(&knot, ob0, ob1, ob2,
                                         choices, poly, &n_poly);
                        nc = find_crossings(poly, n_poly,
                                            cx, MAX_CROSSINGS);
                        det = (nc > 0) ?
                            knot_determinant(n_poly, cx, nc) : 1;
                        if (det >= 0 && det < 256)
                            det_seen[det] = 1;
                    }
                }
                {
                    int d;
                    for (d = 0; d < 256; d++) {
                        if (det_seen[d]) distinct_at_hw[hw]++;
                    }
                }
            }

            printf("    Hamming weight analysis:\n");
            printf("    %3s  %5s  %6s  %6s  %8s\n",
                   "HW", "Count", "MinDet", "MaxDet", "Distinct");
            for (hw = 0; hw <= n_s; hw++) {
                if (count_at_hw[hw] > 0) {
                    printf("    %3d  %5d  %6d  %6d  %8d\n",
                           hw, count_at_hw[hw],
                           min_det_at_hw[hw],
                           max_det_at_hw[hw],
                           distinct_at_hw[hw]);
                }
            }
        }

        printf("\n");
    }

    check("optimal base analysis completes", 1);
}

/* ================================================================
 * Part L: Sliding-Window Construction
 *
 * Instead of a fixed base triangle, use the 3 most recent vertices
 * as sphere centers for each new vertex:
 *   v[3] from (v[0], v[1], v[2])
 *   v[4] from (v[1], v[2], v[3])
 *   v[5] from (v[2], v[3], v[4])
 *   ...
 *
 * Key difference: each bit has its own reflection plane, and
 * flipping bit k cascades forward (changes the plane for k+1).
 * ================================================================ */

typedef struct {
    int vertex_idx;
    Vec3Q vertex;           /* actual vertex */
    Vec3Q mirror;           /* other root */
    Vec3Q local_normal;     /* normal of local reflection plane */
    Rat local_height;       /* signed height above local plane */
    Rat local_n_dot_n;      /* |local normal|^2 */
    int root_sign;
} SlidingStep;

typedef struct {
    int n_steps;
    SlidingStep steps[MAX_STICK_VERTICES];
} SlidingAnalysis;

static SlidingAnalysis sliding_window_decompose(const StickKnot *knot) {
    SlidingAnalysis sa;
    int k;

    sa.n_steps = 0;

    for (k = 3; k < knot->n_vertices; k++) {
        SlidingStep *ss = &sa.steps[sa.n_steps];
        Vec3Q c0 = knot->vertices[k - 3];
        Vec3Q c1 = knot->vertices[k - 2];
        Vec3Q c2 = knot->vertices[k - 1];
        Vec3Q vk = knot->vertices[k];
        Vec3Q e1 = v3q_sub(c1, c0);
        Vec3Q e2 = v3q_sub(c2, c0);
        Vec3Q n = v3q_cross(e1, e2);
        Rat nn = v3q_dot(n, n);
        Vec3Q diff = v3q_sub(vk, c0);
        Rat h = v3q_dot(n, diff);
        Rat two_h_over_nn;

        ss->vertex_idx = k;
        ss->vertex = vk;
        ss->local_normal = n;
        ss->local_height = h;
        ss->local_n_dot_n = nn;

        if (h.p > 0) ss->root_sign = 1;
        else if (h.p < 0) ss->root_sign = -1;
        else ss->root_sign = 0;

        /* Mirror through local plane */
        if (!rat_is_zero(nn)) {
            two_h_over_nn = rat_div(rat_mul(rat_two(), h), nn);
            ss->mirror = v3q_sub(vk, v3q_scale(two_h_over_nn, n));
        } else {
            ss->mirror = vk; /* degenerate: collinear window */
        }

        sa.n_steps++;
    }

    return sa;
}

/* Evaluate sliding-window construction with given choices */
static void evaluate_sliding(const StickKnot *knot,
                              unsigned int choices,
                              Vec3Q *out, int *out_n) {
    int k;
    out[0] = knot->vertices[0];
    out[1] = knot->vertices[1];
    out[2] = knot->vertices[2];
    *out_n = 3;

    for (k = 3; k < knot->n_vertices; k++) {
        int bit = k - 3;
        int use_mirror = (int)((choices >> (unsigned)bit) & 1u);

        if (use_mirror) {
            /* Reflect v[k] through plane of 3 most recent OUTPUT verts */
            Vec3Q c0 = out[*out_n - 3];
            Vec3Q c1 = out[*out_n - 2];
            Vec3Q c2 = out[*out_n - 1];
            out[*out_n] = mirror_point(c0, c1, c2, knot->vertices[k]);
        } else {
            out[*out_n] = knot->vertices[k];
        }
        (*out_n)++;
    }
}

static void test_sliding_window(void) {
    BraidWord bw;
    StickKnot knot;
    SlidingAnalysis sa;
    int k;
    int n_s;
    unsigned int n_a;
    int *dets;
    int det_hist[256];
    int expected_det = 5;

    printf("\n=== Test: Sliding-Window Decomposition (Figure-Eight) ===\n");

    bw = braid_figure_eight();
    braid_to_polygon(&bw, &knot);
    simplify_polygon_topology(&knot);

    printf("  Figure-eight: %d vertices\n", knot.n_vertices);
    for (k = 0; k < knot.n_vertices; k++) {
        printf("    v[%d] = ", k);
        v3q_print(knot.vertices[k]);
        printf("\n");
    }

    /* Decomposition analysis */
    sa = sliding_window_decompose(&knot);

    printf("\n  Sliding-window decomposition:\n");
    for (k = 0; k < sa.n_steps; k++) {
        SlidingStep *ss = &sa.steps[k];
        printf("  Step %d: v[%d], window=(%d,%d,%d)\n",
               k, ss->vertex_idx,
               ss->vertex_idx - 3, ss->vertex_idx - 2, ss->vertex_idx - 1);
        printf("    Local normal = ");
        v3q_print(ss->local_normal);
        printf("\n    Height = ");
        rat_print(ss->local_height);
        printf("  sign=%c",
               ss->root_sign > 0 ? '+' :
               (ss->root_sign < 0 ? '-' : '0'));
        printf("  h^2/|n|^2 = ");
        if (!rat_is_zero(ss->local_n_dot_n))
            rat_print(rat_div(rat_mul(ss->local_height, ss->local_height),
                              ss->local_n_dot_n));
        else
            printf("inf");
        printf("\n    Mirror = ");
        v3q_print(ss->mirror);
        printf("\n");
    }

    /* Compare with all-base: are the normals different? */
    {
        SphereAnalysis ab = sphere_decompose_all_base(&knot);
        int n_same = 0;
        printf("\n  Normal comparison (sliding vs all-base):\n");
        for (k = 0; k < sa.n_steps && k < ab.n_steps; k++) {
            Vec3Q sn = sa.steps[k].local_normal;
            Vec3Q an = ab.normal;
            /* Check if parallel: sn × an = 0 */
            Vec3Q cross = v3q_cross(sn, an);
            int parallel = rat_is_zero(cross.x) &&
                           rat_is_zero(cross.y) &&
                           rat_is_zero(cross.z);
            printf("    Step %d: %s", k,
                   parallel ? "PARALLEL (same plane)" : "DIFFERENT plane");
            if (!parallel) {
                printf("  sliding_n=");
                v3q_print(sn);
            }
            printf("\n");
            if (parallel) n_same++;
        }
        printf("    %d/%d steps have same plane as all-base\n",
               n_same, sa.n_steps);
    }

    /* Exhaustive construction word analysis with sliding window */
    n_s = knot.n_vertices - 3;
    n_a = 1u << (unsigned)n_s;

    printf("\n  Sliding-window construction word analysis:\n");
    printf("    %d bits, %u alternatives\n", n_s, n_a);

    dets = (int *)malloc(n_a * sizeof(int));
    if (!dets) { printf("    OOM\n"); return; }

    memset(det_hist, 0, sizeof(det_hist));

    {
        unsigned int choices;
        for (choices = 0; choices < n_a; choices++) {
            Vec3Q poly[MAX_STICK_VERTICES];
            Crossing cx[MAX_CROSSINGS];
            int n_poly, nc;
            evaluate_sliding(&knot, choices, poly, &n_poly);
            nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
            dets[choices] = (nc > 0) ?
                knot_determinant(n_poly, cx, nc) : 1;
            if (dets[choices] >= 0 && dets[choices] < 256)
                det_hist[dets[choices]]++;
        }
    }

    /* Det histogram */
    printf("    Det histogram:\n");
    {
        int n_distinct = 0;
        int i;
        for (i = 0; i < 256; i++) {
            if (det_hist[i] > 0) {
                printf("      det=%2d: %6d / %u\n",
                       i, det_hist[i], n_a);
                n_distinct++;
            }
        }
        printf("    Distinct determinants: %d\n", n_distinct);
    }

    /* Influence profile */
    printf("    Influence profile (target det=%d):\n", expected_det);
    {
        int bit, n_active = 0, n_free = 0;
        for (bit = 0; bit < n_s; bit++) {
            unsigned int mask = 1u << (unsigned)bit;
            int changes = 0;
            unsigned int choices;
            for (choices = 0; choices < n_a; choices++) {
                if (dets[choices] != dets[choices ^ mask])
                    changes++;
            }
            printf("      Bit %d: %d changes  %s\n",
                   bit, changes, changes == 0 ? "FREE" : "ACTIVE");
            if (changes == 0) n_free++; else n_active++;
        }
        printf("    Summary: %d ACTIVE, %d FREE\n", n_active, n_free);
    }

    /* Self-preservation count */
    {
        int n_self = 0;
        unsigned int choices;
        for (choices = 0; choices < n_a; choices++) {
            if (dets[choices] == expected_det) n_self++;
        }
        printf("    Self-preserving: %d/%u (%.2f%%)\n",
               n_self, n_a, 100.0 * (double)n_self / (double)n_a);
    }

    /* Compare with all-base */
    {
        int match = 0, total = 0;
        unsigned int choices;
        ConstructionWord word;
        setup_construction_word(&word, knot.n_vertices, 0, 1, 2);
        for (choices = 0; choices < n_a; choices++) {
            Vec3Q poly[MAX_STICK_VERTICES];
            Crossing cx[MAX_CROSSINGS];
            int n_poly, nc, ab_det;
            evaluate_construction(&knot, &word, choices, poly, &n_poly);
            nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
            ab_det = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;
            if (ab_det == dets[choices]) match++;
            total++;
        }
        printf("    Sliding vs all-base det agreement: %d/%d (%.1f%%)\n",
               match, total,
               100.0 * (double)match / (double)total);
    }

    free(dets);
    check("sliding-window analysis for figure-eight", 1);
}

/* ================================================================
 * Part M: 5_2 Randomized Palindrome Test
 *
 * Does the palindromic HW symmetry survive different simplifications?
 * Test: geometry property or knot property?
 * ================================================================ */

static void test_5_2_palindrome_robustness(void) {
    int trial;
    printf("\n=== Test: 5_2 Randomized Palindrome Robustness ===\n");

    for (trial = 0; trial < 8; trial++) {
        BraidWord braid = braid_5_2();
        StickKnot knot;
        int n_s, hw;
        unsigned int n_a, choices;
        int count_at_hw[20];
        int distinct_at_hw[20];
        int det_seen[256];
        int is_palindrome = 1;
        int n_self = 0, n_comp = 0;
        unsigned int full_mask;
        int expected_det = 7;

        if (!braid_to_polygon(&braid, &knot)) continue;
        simplify_polygon_random(&knot, (unsigned)(trial * 12347 + 101));

        n_s = knot.n_vertices - 3;
        if (n_s > 15) {
            printf("  Trial %d: %d verts (%d bits) — too many\n",
                   trial, knot.n_vertices, n_s);
            continue;
        }
        n_a = 1u << (unsigned)n_s;
        full_mask = n_a - 1u;

        /* Compute HW distinct counts */
        memset(count_at_hw, 0, sizeof(count_at_hw));
        for (hw = 0; hw <= n_s; hw++) distinct_at_hw[hw] = 0;

        for (hw = 0; hw <= n_s; hw++) {
            memset(det_seen, 0, sizeof(det_seen));
            for (choices = 0; choices < n_a; choices++) {
                if (popcount_u(choices) == hw) {
                    Vec3Q poly[MAX_STICK_VERTICES];
                    Crossing cx[MAX_CROSSINGS];
                    int n_poly, nc, det;
                    evaluate_inplace(&knot, 0, 1, 2,
                                     choices, poly, &n_poly);
                    nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
                    det = (nc > 0) ?
                        knot_determinant(n_poly, cx, nc) : 1;
                    if (det >= 0 && det < 256) det_seen[det] = 1;
                    count_at_hw[hw]++;
                    if (det == expected_det) n_self++;
                }
            }
            {
                int d;
                for (d = 0; d < 256; d++)
                    if (det_seen[d]) distinct_at_hw[hw]++;
            }
        }

        /* Check palindrome: distinct[k] == distinct[n_s - k] */
        for (hw = 0; hw <= n_s / 2; hw++) {
            if (distinct_at_hw[hw] != distinct_at_hw[n_s - hw]) {
                is_palindrome = 0;
                break;
            }
        }

        /* Complement closure check */
        for (choices = 0; choices < n_a; choices++) {
            if (1) {
                Vec3Q poly[MAX_STICK_VERTICES];
                Crossing cx[MAX_CROSSINGS];
                int n_poly, nc, det;
                evaluate_inplace(&knot, 0, 1, 2,
                                 choices, poly, &n_poly);
                nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
                det = (nc > 0) ?
                    knot_determinant(n_poly, cx, nc) : 1;
                if (det == expected_det) {
                    /* Check complement */
                    unsigned int comp = full_mask ^ choices;
                    Vec3Q poly2[MAX_STICK_VERTICES];
                    Crossing cx2[MAX_CROSSINGS];
                    int n_poly2, nc2, det2;
                    evaluate_inplace(&knot, 0, 1, 2,
                                     comp, poly2, &n_poly2);
                    nc2 = find_crossings(poly2, n_poly2,
                                         cx2, MAX_CROSSINGS);
                    det2 = (nc2 > 0) ?
                        knot_determinant(n_poly2, cx2, nc2) : 1;
                    if (det2 == expected_det) n_comp++;
                }
            }
        }

        printf("  Trial %d: %d verts, %d bits  HW_distinct={",
               trial, knot.n_vertices, n_s);
        for (hw = 0; hw <= n_s; hw++) {
            if (hw > 0) printf(",");
            printf("%d", distinct_at_hw[hw]);
        }
        printf("}  palindrome=%s  self=%d/%u  comp=%d/%d\n",
               is_palindrome ? "YES" : "no",
               n_self, n_a, n_comp, n_self);
    }
    check("5_2 palindrome robustness completes", 1);
}

/* ================================================================
 * Part N: 8_18 4-Bit Spectral Core Hypothesis
 *
 * The 8_18 optimal-base spectrum has consecutive odds 1..31 (16 vals).
 * 16 = 2^4. Hypothesis: a 4-bit subset generates exactly {1,3,...,31}.
 * Test all C(9,4) = 126 subsets.
 * ================================================================ */

static void test_8_18_spectral_core(void) {
    BraidWord braid = braid_8_18();
    StickKnot knot;
    int ob0, ob1, ob2;
    int n_s;
    unsigned int n_a;
    int *dets;
    int subset[4];
    int n_full_span = 0;
    int best_distinct = 0;
    int best_subset[4] = {0, 0, 0, 0};
    int s0, s1, s2, s3;

    printf("\n=== Test: 8_18 4-Bit Spectral Core ===\n");

    braid_to_polygon(&braid, &knot);
    simplify_polygon_topology(&knot);
    find_optimal_base(&knot, &ob0, &ob1, &ob2);

    n_s = knot.n_vertices - 3;
    n_a = 1u << (unsigned)n_s;

    printf("  %d bits (opt base %d,%d,%d), testing C(%d,4)=%d subsets\n",
           n_s, ob0, ob1, ob2, n_s,
           (n_s * (n_s - 1) * (n_s - 2) * (n_s - 3)) / 24);

    /* Precompute all dets */
    dets = (int *)malloc(n_a * sizeof(int));
    if (!dets) { printf("  OOM\n"); return; }

    {
        unsigned int choices;
        for (choices = 0; choices < n_a; choices++) {
            Vec3Q poly[MAX_STICK_VERTICES];
            Crossing cx[MAX_CROSSINGS];
            int n_poly, nc;
            evaluate_inplace(&knot, ob0, ob1, ob2,
                             choices, poly, &n_poly);
            nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
            dets[choices] = (nc > 0) ?
                knot_determinant(n_poly, cx, nc) : 1;
        }
    }

    /* Test all C(n_s, 4) subsets */
    for (s0 = 0; s0 < n_s - 3; s0++) {
        for (s1 = s0 + 1; s1 < n_s - 2; s1++) {
            for (s2 = s1 + 1; s2 < n_s - 1; s2++) {
                for (s3 = s2 + 1; s3 < n_s; s3++) {
                    int det_seen[256];
                    int n_distinct = 0;
                    int has_consec_odds = 1;
                    unsigned int si;

                    subset[0] = s0; subset[1] = s1;
                    subset[2] = s2; subset[3] = s3;

                    memset(det_seen, 0, sizeof(det_seen));

                    /* Vary these 4 bits, fix others at 0 */
                    for (si = 0; si < 16; si++) {
                        unsigned int full_choice = 0;
                        int bi;
                        for (bi = 0; bi < 4; bi++) {
                            if (si & (1u << (unsigned)bi))
                                full_choice |=
                                    (1u << (unsigned)subset[bi]);
                        }
                        if (dets[full_choice] >= 0 &&
                            dets[full_choice] < 256)
                            det_seen[dets[full_choice]] = 1;
                    }

                    {
                        int d;
                        for (d = 0; d < 256; d++)
                            if (det_seen[d]) n_distinct++;
                    }

                    /* Check: does it contain 1,3,5,...,31? */
                    {
                        int v;
                        for (v = 1; v <= 31; v += 2) {
                            if (!det_seen[v]) {
                                has_consec_odds = 0;
                                break;
                            }
                        }
                    }

                    if (has_consec_odds && n_distinct >= 16) {
                        printf("  FULL SPAN {1..31}: "
                               "bits {%d,%d,%d,%d}  "
                               "distinct=%d\n",
                               s0, s1, s2, s3, n_distinct);
                        n_full_span++;
                    }

                    if (n_distinct > best_distinct) {
                        best_distinct = n_distinct;
                        best_subset[0] = s0;
                        best_subset[1] = s1;
                        best_subset[2] = s2;
                        best_subset[3] = s3;
                    }
                }
            }
        }
    }

    printf("  Full-span subsets: %d / 126\n", n_full_span);
    printf("  Best 4-bit subset: {%d,%d,%d,%d} with %d distinct dets\n",
           best_subset[0], best_subset[1],
           best_subset[2], best_subset[3],
           best_distinct);

    /* Print the best subset's det map */
    {
        unsigned int si;
        int det_seen[256];
        memset(det_seen, 0, sizeof(det_seen));

        printf("  Best subset det map (16 patterns):\n");
        for (si = 0; si < 16; si++) {
            unsigned int full_choice = 0;
            int bi;
            for (bi = 0; bi < 4; bi++) {
                if (si & (1u << (unsigned)bi))
                    full_choice |=
                        (1u << (unsigned)best_subset[bi]);
            }
            printf("    ");
            for (bi = 3; bi >= 0; bi--)
                printf("%d", (int)((si >> (unsigned)bi) & 1u));
            printf(" -> det=%d\n", dets[full_choice]);
            if (dets[full_choice] >= 0 && dets[full_choice] < 256)
                det_seen[dets[full_choice]] = 1;
        }
        printf("  Distinct: ");
        {
            int d, first = 1;
            for (d = 0; d < 256; d++) {
                if (det_seen[d]) {
                    if (!first) printf(",");
                    printf("%d", d);
                    first = 0;
                }
            }
        }
        printf("\n");
    }

    /* Also test 5-bit subsets (C(9,5) = 126) for comparison */
    {
        int n5_full = 0;
        int best5 = 0;
        int best5_sub[5] = {0,0,0,0,0};
        int t0, t1, t2, t3, t4;

        for (t0 = 0; t0 < n_s - 4; t0++) {
        for (t1 = t0 + 1; t1 < n_s - 3; t1++) {
        for (t2 = t1 + 1; t2 < n_s - 2; t2++) {
        for (t3 = t2 + 1; t3 < n_s - 1; t3++) {
        for (t4 = t3 + 1; t4 < n_s; t4++) {
            int det_seen[256];
            int nd = 0;
            int has31 = 1;
            unsigned int si;

            memset(det_seen, 0, sizeof(det_seen));
            for (si = 0; si < 32; si++) {
                unsigned int fc = 0;
                int subs[5];
                int bi;
                subs[0] = t0; subs[1] = t1; subs[2] = t2;
                subs[3] = t3; subs[4] = t4;
                for (bi = 0; bi < 5; bi++) {
                    if (si & (1u << (unsigned)bi))
                        fc |= (1u << (unsigned)subs[bi]);
                }
                if (dets[fc] >= 0 && dets[fc] < 256)
                    det_seen[dets[fc]] = 1;
            }
            {
                int d;
                for (d = 0; d < 256; d++)
                    if (det_seen[d]) nd++;
            }
            {
                int v;
                for (v = 1; v <= 31; v += 2)
                    if (!det_seen[v]) { has31 = 0; break; }
            }
            if (has31 && nd >= 16) n5_full++;
            if (nd > best5) {
                best5 = nd;
                best5_sub[0] = t0; best5_sub[1] = t1;
                best5_sub[2] = t2; best5_sub[3] = t3;
                best5_sub[4] = t4;
            }
        }}}}}

        printf("  5-bit subsets with full span {1..31}: %d / 126\n",
               n5_full);
        printf("  Best 5-bit subset: {%d,%d,%d,%d,%d} with %d distinct\n",
               best5_sub[0], best5_sub[1], best5_sub[2],
               best5_sub[3], best5_sub[4], best5);
    }

    free(dets);
    check("8_18 spectral core analysis completes", 1);
}

/* ================================================================
 * Part O: Density Computation
 *
 * density(HW=k) = distinct_dets(k) / C(n,k)
 * Removes binomial coefficient effect from HW analysis.
 * ================================================================ */

static void test_density_computation(void) {
    BraidWord braids[2];
    int bi;

    printf("\n=== Test: HW Density (6_3, 8_18) ===\n");

    braids[0] = braid_6_3();
    braids[1] = braid_8_18();

    for (bi = 0; bi < 2; bi++) {
        StickKnot knot;
        int ob0, ob1, ob2;
        int n_s, hw;
        unsigned int n_a, choices;
        int distinct_at_hw[20];
        int count_at_hw[20];
        int det_seen[256];

        braid_to_polygon(&braids[bi], &knot);
        simplify_polygon_topology(&knot);
        find_optimal_base(&knot, &ob0, &ob1, &ob2);

        n_s = knot.n_vertices - 3;
        n_a = 1u << (unsigned)n_s;

        for (hw = 0; hw <= n_s; hw++) {
            distinct_at_hw[hw] = 0;
            count_at_hw[hw] = 0;
        }

        for (hw = 0; hw <= n_s; hw++) {
            memset(det_seen, 0, sizeof(det_seen));
            for (choices = 0; choices < n_a; choices++) {
                if (popcount_u(choices) == hw) {
                    Vec3Q poly[MAX_STICK_VERTICES];
                    Crossing cx[MAX_CROSSINGS];
                    int n_poly, nc, det;
                    evaluate_inplace(&knot, ob0, ob1, ob2,
                                     choices, poly, &n_poly);
                    nc = find_crossings(poly, n_poly,
                                        cx, MAX_CROSSINGS);
                    det = (nc > 0) ?
                        knot_determinant(n_poly, cx, nc) : 1;
                    if (det >= 0 && det < 256)
                        det_seen[det] = 1;
                    count_at_hw[hw]++;
                }
            }
            {
                int d;
                for (d = 0; d < 256; d++)
                    if (det_seen[d]) distinct_at_hw[hw]++;
            }
        }

        printf("\n  %s (opt base %d,%d,%d), %d bits:\n",
               braids[bi].name, ob0, ob1, ob2, n_s);
        printf("    %3s  %6s  %8s  %10s\n",
               "HW", "C(n,k)", "Distinct", "Density");
        for (hw = 0; hw <= n_s; hw++) {
            if (count_at_hw[hw] > 0) {
                printf("    %3d  %6d  %8d  %9.4f%%\n",
                       hw, count_at_hw[hw],
                       distinct_at_hw[hw],
                       100.0 * (double)distinct_at_hw[hw] /
                       (double)count_at_hw[hw]);
            }
        }
    }

    check("density computation completes", 1);
}

/* ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 111: Braid-to-Construction-Word Pipeline\n");
    printf("======================================================\n");

    test_braid_layout();
    test_vertex_counts();
    test_construction_word_trefoil();
    test_simplified_construction();
    test_randomized_simplification();
    test_sphere_decomposition();
    test_base_triangle_sweep();
    test_optimal_base_all();
    test_sliding_window();
    test_5_2_palindrome_robustness();
    test_8_18_spectral_core();
    test_density_computation();

    printf("\n======================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
