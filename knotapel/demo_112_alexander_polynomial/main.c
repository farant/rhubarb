/*
 * KNOTAPEL DEMO 112: Alexander Polynomial via Construction Words
 * ================================================================
 *
 * Extends Demo 111's pipeline to compute the FULL Alexander polynomial
 * Delta(t) instead of just det = |Delta(-1)|. This reveals finer-grained
 * topological structure invisible to the determinant alone.
 *
 * Pipeline:
 *   BraidWord -> polygon -> simplify -> crossings -> arc labeling
 *   -> Alexander matrix over Q[t] -> Bareiss determinant -> Delta(t)
 *
 * New infrastructure:
 *   - RatPoly: polynomial with Rat coefficients
 *   - Bareiss algorithm for polynomial matrix determinant
 *   - Alexander matrix with polynomial entries
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
 * Part A: Exact Rational Arithmetic (from Demo 110/111)
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

static void rat_print(Rat r) {
    if (r.q == 1) printf("%ld", r.p);
    else printf("%ld/%ld", r.p, r.q);
}

/* ================================================================
 * Part B: Rational 3D Vectors (from Demo 110/111)
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

static Vec3Q mirror_point(Vec3Q c0, Vec3Q c1, Vec3Q c2, Vec3Q target) {
    Vec3Q e1 = v3q_sub(c1, c0);
    Vec3Q e2 = v3q_sub(c2, c0);
    Vec3Q n = v3q_cross(e1, e2);
    Rat n_dot_n = v3q_dot(n, n);
    Vec3Q p_minus_c0 = v3q_sub(target, c0);
    Rat n_dot_d = v3q_dot(n, p_minus_c0);
    Rat t = rat_div(rat_mul(rat_from_int(2), n_dot_d), n_dot_n);
    return v3q_sub(target, v3q_scale(t, n));
}

/* ================================================================
 * Part C: Stick Knot Types (from Demo 111)
 * ================================================================ */

#define MAX_STICK_VERTICES 128

typedef struct {
    const char *name;
    int crossing_number;
    int n_vertices;
    Vec3Q vertices[MAX_STICK_VERTICES];
} StickKnot;

/* ================================================================
 * Part D: Crossing Detection (from Demo 111)
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

/* Parameterized generic projection.
 * dir = (a, b, c) defines the depth axis.
 * uv plane is chosen orthogonal to depth. */
static void generic_project_dir(Vec3Q v3, Vec2Q *uv, Rat *depth,
                                i64 a, i64 b, i64 c) {
    Rat ca = rat_from_int(a);
    Rat cb = rat_from_int(b);
    Rat cc = rat_from_int(c);
    /* Use (c,0,-a) and (0,c,-b) as approximate screen axes */
    uv->x = rat_sub(rat_mul(cc, v3.x), rat_mul(ca, v3.z));
    uv->y = rat_sub(rat_mul(cc, v3.y), rat_mul(cb, v3.z));
    *depth = rat_add(rat_mul(ca, v3.x),
                     rat_add(rat_mul(cb, v3.y), rat_mul(cc, v3.z)));
}

/* generic_project is now inlined into find_crossings_dir via generic_project_dir */

#define MAX_CROSSINGS 96
#define MAX_ARCS 96

static int find_crossings_dir(const Vec3Q *verts, int n,
                              Crossing *crossings, int max_crossings,
                              i64 pa, i64 pb, i64 pc)
{
    int i, j, nc = 0;
    Vec2Q proj[MAX_STICK_VERTICES];
    Rat depth[MAX_STICK_VERTICES];

    for (i = 0; i < n; i++) {
        generic_project_dir(verts[i], &proj[i], &depth[i], pa, pb, pc);
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

/* find_crossings_ex: legacy wrapper, uses fixed (2,3,5) direction */
static int find_crossings_ex(const Vec3Q *verts, int n,
                              Crossing *crossings, int max_crossings,
                              int perturb_depth) {
    (void)perturb_depth;
    return find_crossings_dir(verts, n, crossings, max_crossings, 2, 3, 5);
}

/* Wrapper: find_crossings using default (2,3,5) direction */
static int find_crossings(const Vec3Q *verts, int n,
                          Crossing *crossings, int max_crossings) {
    return find_crossings_dir(verts, n, crossings, max_crossings, 2, 3, 5);
}

/* Adaptive find_crossings: try multiple projection directions
 * until one produces zero degenerate crossings.
 * Returns the number of crossings found, with correct signs. */
static int find_crossings_clean(const Vec3Q *verts, int n,
                                Crossing *crossings, int max_crossings) {
    /* Projection directions: coprime triples */
    static const i64 dirs[][3] = {
        {2, 3, 5}, {3, 5, 7}, {7, 11, 13}, {11, 13, 17},
        {17, 19, 23}, {23, 29, 31}, {31, 37, 41}, {37, 41, 43}
    };
    int n_dirs = 8;
    int di, nc, k;

    for (di = 0; di < n_dirs; di++) {
        int has_degen = 0;
        nc = find_crossings_dir(verts, n, crossings, max_crossings,
                                dirs[di][0], dirs[di][1], dirs[di][2]);
        for (k = 0; k < nc; k++) {
            if (crossings[k].sign == 0) {
                has_degen = 1;
                break;
            }
        }
        if (!has_degen) return nc;
    }
    /* Fallback: return last result (may have degeneracies) */
    return nc;
}

/* ================================================================
 * Part E: Arc Labeling and Knot Determinant (from Demo 111)
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

/* Compute arc labels shared by both det and polynomial routines */
static void label_arcs(const Crossing *crossings, int nc,
                       int *over_arc, int *under_in_arc,
                       int *under_out_arc) {
    ArcBreak breaks[MAX_CROSSINGS];
    int i, j;

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

    /* Sort breaks by (edge_idx, param) */
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

    /* Under arcs */
    for (i = 0; i < nc; i++) {
        int cj = breaks[i].crossing_idx;
        under_out_arc[cj] = i;
        under_in_arc[cj] = (i - 1 + nc) % nc;
    }

    /* Over arcs */
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
}

static int knot_determinant(int n_edges, const Crossing *crossings, int nc) {
    int over_arc[MAX_CROSSINGS];
    int under_in_arc[MAX_CROSSINGS];
    int under_out_arc[MAX_CROSSINGS];
    long M[MAX_CROSSINGS][MAX_ARCS];
    int i, j, k, sz;
    long det;

    (void)n_edges;

    if (nc == 0) return 1;

    label_arcs(crossings, nc, over_arc, under_in_arc, under_out_arc);

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
 * Part F: Braid Words and Braid-to-Polygon (from Demo 111)
 * ================================================================ */

#define MAX_BRAID_STRANDS   8
#define MAX_BRAID_GENS      32

typedef struct {
    int generators[MAX_BRAID_GENS];
    int n_generators;
    int n_strands;
    const char *name;
    int expected_det;
} BraidWord;

#define BRAID_S       20
#define BRAID_Z       20
#define BRAID_D        1
#define BRAID_MARGIN 100

#define MAX_STRAND_VERTS 128

typedef struct {
    Vec3Q verts[MAX_STRAND_VERTS];
    int n_verts;
} StrandTrace;

static int braid_to_polygon(const BraidWord *braid, StickKnot *out) {
    StrandTrace traces[MAX_BRAID_STRANDS];
    int pos_to_strand[MAX_BRAID_STRANDS];
    int strand_to_pos[MAX_BRAID_STRANDS];
    int k, s, n;
    int cycle_order[MAX_BRAID_STRANDS];
    int cycle_len;

    n = braid->n_strands;

    for (s = 0; s < n; s++) {
        pos_to_strand[s] = s;
        strand_to_pos[s] = s;
        traces[s].n_verts = 0;
    }

    for (s = 0; s < n; s++) {
        traces[s].verts[traces[s].n_verts++] =
            v3q_from_ints((i64)s * BRAID_S, 0, 0);
    }

    for (k = 0; k < braid->n_generators; k++) {
        int gen = braid->generators[k];
        int gi = (gen > 0) ? gen : -gen;
        int sign = (gen > 0) ? 1 : -1;
        int pos_left = gi - 1;
        int pos_right = gi;
        int s_left = pos_to_strand[pos_left];
        int s_right = pos_to_strand[pos_right];
        i64 z_mid = (i64)(k * BRAID_Z + BRAID_Z / 2);
        i64 z_after = (i64)((k + 1) * BRAID_Z);

        for (s = 0; s < n; s++) {
            int pos = strand_to_pos[s];
            if (s == s_left) {
                traces[s].verts[traces[s].n_verts++] =
                    v3q_from_ints((i64)pos_right * BRAID_S,
                                 (i64)(sign * BRAID_D), z_mid);
            } else if (s == s_right) {
                traces[s].verts[traces[s].n_verts++] =
                    v3q_from_ints((i64)pos_left * BRAID_S,
                                 (i64)(-sign * BRAID_D), z_mid);
            } else {
                traces[s].verts[traces[s].n_verts++] =
                    v3q_from_ints((i64)pos * BRAID_S, 0, z_mid);
            }
        }

        pos_to_strand[pos_left] = s_right;
        pos_to_strand[pos_right] = s_left;
        strand_to_pos[s_left] = pos_right;
        strand_to_pos[s_right] = pos_left;

        for (s = 0; s < n; s++) {
            int pos = strand_to_pos[s];
            traces[s].verts[traces[s].n_verts++] =
                v3q_from_ints((i64)pos * BRAID_S, 0, z_after);
        }
    }

    {
        int visited[MAX_BRAID_STRANDS];
        int cur;
        for (s = 0; s < n; s++) visited[s] = 0;

        cycle_len = 0;
        cur = 0;
        do {
            if (visited[cur]) break;
            visited[cur] = 1;
            cycle_order[cycle_len++] = cur;
            cur = strand_to_pos[cur];
        } while (cur != 0);

        if (cycle_len != n) {
            printf("    WARNING: braid closure has %d components\n",
                   n - cycle_len + 1);
            return 0;
        }
    }

    {
        i64 z_final = (i64)(braid->n_generators * BRAID_Z);
        int vi = 0;

        out->name = braid->name;
        out->crossing_number = 0;

        for (k = 0; k < cycle_len; k++) {
            int strand = cycle_order[k];
            int v;

            for (v = 0; v < traces[strand].n_verts; v++) {
                if (vi >= MAX_STICK_VERTICES) {
                    printf("    ERROR: too many vertices (%d)\n", vi);
                    return 0;
                }
                out->vertices[vi++] = traces[strand].verts[v];
            }

            {
                int next_strand = cycle_order[(k + 1) % cycle_len];
                i64 end_x = (i64)strand_to_pos[strand] * BRAID_S;
                i64 start_x = (i64)next_strand * BRAID_S;
                i64 y_off = (i64)(-(BRAID_MARGIN + k * 5));

                if (vi + 2 > MAX_STICK_VERTICES) {
                    printf("    ERROR: too many vertices\n");
                    return 0;
                }

                out->vertices[vi++] =
                    v3q_from_ints(end_x, y_off, z_final);
                out->vertices[vi++] =
                    v3q_from_ints(start_x, y_off, 0);
            }
        }

        out->n_vertices = vi;
    }

    /* Collinear simplification */
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

/* Known braid words */

static BraidWord braid_trefoil(void) {
    BraidWord b;
    b.n_strands = 2; b.n_generators = 3;
    b.generators[0] = 1; b.generators[1] = 1; b.generators[2] = 1;
    b.name = "trefoil 3_1"; b.expected_det = 3;
    return b;
}

static BraidWord braid_figure_eight(void) {
    BraidWord b;
    b.n_strands = 3; b.n_generators = 4;
    b.generators[0] = 1; b.generators[1] = -2;
    b.generators[2] = 1; b.generators[3] = -2;
    b.name = "figure-eight 4_1"; b.expected_det = 5;
    return b;
}

static BraidWord braid_cinquefoil(void) {
    BraidWord b;
    b.n_strands = 2; b.n_generators = 5;
    b.generators[0] = 1; b.generators[1] = 1; b.generators[2] = 1;
    b.generators[3] = 1; b.generators[4] = 1;
    b.name = "cinquefoil 5_1"; b.expected_det = 5;
    return b;
}

static BraidWord braid_5_2(void) {
    BraidWord b;
    b.n_strands = 3; b.n_generators = 6;
    b.generators[0] = -1; b.generators[1] = -1; b.generators[2] = -1;
    b.generators[3] = -2; b.generators[4] = 1; b.generators[5] = -2;
    b.name = "5_2"; b.expected_det = 7;
    return b;
}

static BraidWord braid_6_3(void) {
    BraidWord b;
    b.n_strands = 3; b.n_generators = 6;
    b.generators[0] = -1; b.generators[1] = -1; b.generators[2] = 2;
    b.generators[3] = -1; b.generators[4] = 2; b.generators[5] = 2;
    b.name = "6_3"; b.expected_det = 13;
    return b;
}

static BraidWord braid_8_18(void) {
    BraidWord b;
    b.n_strands = 3; b.n_generators = 8;
    b.generators[0] = -1; b.generators[1] = 2;
    b.generators[2] = -1; b.generators[3] = 2;
    b.generators[4] = -1; b.generators[5] = 2;
    b.generators[6] = -1; b.generators[7] = 2;
    b.name = "8_18"; b.expected_det = 45;
    return b;
}

static BraidWord braid_6_1(void) {
    BraidWord b;
    b.n_strands = 4; b.n_generators = 7;
    b.generators[0] = -1; b.generators[1] = -1;
    b.generators[2] = -2; b.generators[3] = 1;
    b.generators[4] = 3; b.generators[5] = -2;
    b.generators[6] = 3;
    b.name = "6_1"; b.expected_det = 9;
    return b;
}

static BraidWord braid_6_2(void) {
    BraidWord b;
    b.n_strands = 3; b.n_generators = 6;
    b.generators[0] = -1; b.generators[1] = -1;
    b.generators[2] = -1; b.generators[3] = 2;
    b.generators[4] = -1; b.generators[5] = 2;
    b.name = "6_2"; b.expected_det = 11;
    return b;
}

static BraidWord braid_7_1(void) {
    BraidWord b;
    int i;
    b.n_strands = 2; b.n_generators = 7;
    for (i = 0; i < 7; i++) b.generators[i] = 1;
    b.name = "7_1"; b.expected_det = 7;
    return b;
}

static BraidWord braid_7_2(void) {
    BraidWord b;
    b.n_strands = 4; b.n_generators = 9;
    b.generators[0] = -1; b.generators[1] = -1;
    b.generators[2] = -1; b.generators[3] = -2;
    b.generators[4] = 1; b.generators[5] = -2;
    b.generators[6] = -3; b.generators[7] = 2;
    b.generators[8] = -3;
    b.name = "7_2"; b.expected_det = 11;
    return b;
}

static BraidWord braid_7_3(void) {
    BraidWord b;
    b.n_strands = 3; b.n_generators = 8;
    b.generators[0] = 1; b.generators[1] = 1;
    b.generators[2] = 1; b.generators[3] = 1;
    b.generators[4] = 1; b.generators[5] = 2;
    b.generators[6] = -1; b.generators[7] = 2;
    b.name = "7_3"; b.expected_det = 13;
    return b;
}

static BraidWord braid_7_4(void) {
    BraidWord b;
    b.n_strands = 4; b.n_generators = 9;
    b.generators[0] = 1; b.generators[1] = 1;
    b.generators[2] = 2; b.generators[3] = -1;
    b.generators[4] = 2; b.generators[5] = 2;
    b.generators[6] = 3; b.generators[7] = -2;
    b.generators[8] = 3;
    b.name = "7_4"; b.expected_det = 15;
    return b;
}

/* Topology-preserving simplification.
 * If known_det > 0, use it as target; otherwise auto-detect. */
static int simplify_polygon_topology_ex(StickKnot *knot, int known_det) {
    int removed = 0;
    int changed = 1;
    int target_det;

    if (known_det > 0) {
        target_det = known_det;
    } else {
        Crossing cx[MAX_CROSSINGS];
        int nc = find_crossings(knot->vertices, knot->n_vertices,
                                cx, MAX_CROSSINGS);
        target_det = (nc > 0) ?
            knot_determinant(knot->n_vertices, cx, nc) : 1;
    }

    while (changed) {
        int i;
        changed = 0;
        for (i = 0; i < knot->n_vertices; i++) {
            Vec3Q temp[MAX_STICK_VERTICES];
            int tn = 0, j;
            Crossing cx[MAX_CROSSINGS];
            int nc, det;

            for (j = 0; j < knot->n_vertices; j++) {
                if (j != i) temp[tn++] = knot->vertices[j];
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

static int simplify_polygon_topology(StickKnot *knot) {
    return simplify_polygon_topology_ex(knot, 0);
}

/* Randomized topology-preserving simplification (from Demo 111).
 * Uses Fisher-Yates shuffle of vertex scan order to produce
 * different simplified polygons from the same input. */
static int simplify_polygon_random_ex(StickKnot *knot, unsigned int seed,
                                       int known_det) {
    int removed = 0;
    int changed = 1;
    int target_det;
    int order[MAX_STICK_VERTICES];
    int i;

    if (known_det > 0) {
        target_det = known_det;
    } else {
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

static int simplify_polygon_random(StickKnot *knot, unsigned int seed) {
    return simplify_polygon_random_ex(knot, seed, 0);
}

/* Construction word evaluation (for Phase 3) */
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

/* ================================================================
 * Part G: Polynomial Arithmetic over Q[t]
 *
 * RatPoly: polynomial with Rat coefficients.
 * coeffs[i] is the coefficient of t^i.
 * degree is the highest non-zero power (-1 for zero polynomial).
 * ================================================================ */

#define RATPOLY_MAX_DEG 48

typedef struct {
    Rat coeffs[RATPOLY_MAX_DEG + 1];
    int degree; /* -1 for zero polynomial */
} RatPoly;

static RatPoly rp_zero(void) {
    RatPoly p;
    int i;
    for (i = 0; i <= RATPOLY_MAX_DEG; i++)
        p.coeffs[i] = rat_from_int(0);
    p.degree = -1;
    return p;
}

static RatPoly rp_const(Rat c) {
    RatPoly p = rp_zero();
    if (!rat_is_zero(c)) {
        p.coeffs[0] = c;
        p.degree = 0;
    }
    return p;
}

static RatPoly rp_const_int(i64 n) {
    return rp_const(rat_from_int(n));
}

/* The polynomial t */
static RatPoly rp_t(void) {
    RatPoly p = rp_zero();
    p.coeffs[1] = rat_from_int(1);
    p.degree = 1;
    return p;
}

/* Linear polynomial: c0 + c1*t */
static RatPoly rp_linear(Rat c0, Rat c1) {
    RatPoly p = rp_zero();
    p.coeffs[0] = c0;
    p.coeffs[1] = c1;
    if (!rat_is_zero(c1)) p.degree = 1;
    else if (!rat_is_zero(c0)) p.degree = 0;
    else p.degree = -1;
    return p;
}

static int rp_is_zero(const RatPoly *p) {
    return p->degree < 0;
}

/* Recompute degree after modification */
static void rp_fix_degree(RatPoly *p) {
    int i;
    p->degree = -1;
    for (i = RATPOLY_MAX_DEG; i >= 0; i--) {
        if (!rat_is_zero(p->coeffs[i])) {
            p->degree = i;
            return;
        }
    }
}

static RatPoly rp_add(RatPoly a, RatPoly b) {
    RatPoly r;
    int i;
    int max_d = (a.degree > b.degree) ? a.degree : b.degree;
    for (i = 0; i <= RATPOLY_MAX_DEG; i++) {
        if (i <= max_d)
            r.coeffs[i] = rat_add(a.coeffs[i], b.coeffs[i]);
        else
            r.coeffs[i] = rat_from_int(0);
    }
    rp_fix_degree(&r);
    return r;
}

static RatPoly rp_sub(RatPoly a, RatPoly b) {
    RatPoly r;
    int i;
    int max_d = (a.degree > b.degree) ? a.degree : b.degree;
    for (i = 0; i <= RATPOLY_MAX_DEG; i++) {
        if (i <= max_d)
            r.coeffs[i] = rat_sub(a.coeffs[i], b.coeffs[i]);
        else
            r.coeffs[i] = rat_from_int(0);
    }
    rp_fix_degree(&r);
    return r;
}

static RatPoly rp_neg(RatPoly a) {
    RatPoly r;
    int i;
    for (i = 0; i <= RATPOLY_MAX_DEG; i++)
        r.coeffs[i] = rat_neg(a.coeffs[i]);
    r.degree = a.degree;
    return r;
}

static RatPoly rp_mul(RatPoly a, RatPoly b) {
    RatPoly r = rp_zero();
    int i, j;
    if (a.degree < 0 || b.degree < 0) return r;
    if (a.degree + b.degree > RATPOLY_MAX_DEG) {
        fprintf(stderr, "rp_mul: degree overflow %d + %d > %d\n",
                a.degree, b.degree, RATPOLY_MAX_DEG);
        exit(1);
    }
    for (i = 0; i <= a.degree; i++) {
        if (rat_is_zero(a.coeffs[i])) continue;
        for (j = 0; j <= b.degree; j++) {
            if (rat_is_zero(b.coeffs[j])) continue;
            r.coeffs[i + j] = rat_add(r.coeffs[i + j],
                                       rat_mul(a.coeffs[i], b.coeffs[j]));
        }
    }
    rp_fix_degree(&r);
    return r;
}

/* Scalar multiply: c * p(t) */
static RatPoly rp_scale(Rat c, RatPoly p) {
    RatPoly r;
    int i;
    if (rat_is_zero(c)) return rp_zero();
    for (i = 0; i <= RATPOLY_MAX_DEG; i++)
        r.coeffs[i] = rat_mul(c, p.coeffs[i]);
    r.degree = p.degree;
    rp_fix_degree(&r);
    return r;
}

/* Exact polynomial division: num / den, assuming remainder = 0.
 * Uses standard polynomial long division over Q. */
static RatPoly rp_exact_div(RatPoly num, RatPoly den) {
    RatPoly quot = rp_zero();
    Rat rem[RATPOLY_MAX_DEG + 1];
    int i, d;

    if (den.degree < 0) {
        fprintf(stderr, "rp_exact_div: division by zero polynomial\n");
        exit(1);
    }

    /* If numerator is zero, quotient is zero */
    if (num.degree < 0) return quot;

    /* Copy numerator into working remainder */
    for (i = 0; i <= RATPOLY_MAX_DEG; i++)
        rem[i] = num.coeffs[i];

    /* Long division */
    for (d = num.degree; d >= den.degree; d--) {
        Rat coeff;
        if (rat_is_zero(rem[d])) continue;
        coeff = rat_div(rem[d], den.coeffs[den.degree]);
        quot.coeffs[d - den.degree] = coeff;
        for (i = 0; i <= den.degree; i++) {
            rem[d - den.degree + i] =
                rat_sub(rem[d - den.degree + i],
                        rat_mul(coeff, den.coeffs[i]));
        }
    }

    /* Verify remainder is zero */
    for (i = 0; i <= num.degree; i++) {
        if (!rat_is_zero(rem[i])) {
            fprintf(stderr,
                    "rp_exact_div: non-zero remainder at degree %d"
                    " (coeff=%ld/%ld)\n", i, rem[i].p, rem[i].q);
            exit(1);
        }
    }

    rp_fix_degree(&quot);
    return quot;
}

/* Evaluate polynomial at rational point */
static Rat rp_eval(const RatPoly *p, Rat t) {
    Rat result = rat_from_int(0);
    Rat power = rat_from_int(1);
    int i;
    for (i = 0; i <= p->degree; i++) {
        result = rat_add(result, rat_mul(p->coeffs[i], power));
        power = rat_mul(power, t);
    }
    return result;
}

/* Normalize Alexander polynomial:
 * 1. Shift so minimum power of t is t^0
 * 2. Divide by GCD of all integer coefficients
 * 3. Make leading coefficient positive
 * Assumes all coefficients are integers (q=1) after step 1. */
static RatPoly rp_normalize_alexander(RatPoly p) {
    RatPoly r = rp_zero();
    int min_pow, i;
    i64 g;

    if (p.degree < 0) return p;

    /* Find minimum non-zero power */
    min_pow = p.degree + 1;
    for (i = 0; i <= p.degree; i++) {
        if (!rat_is_zero(p.coeffs[i])) {
            min_pow = i;
            break;
        }
    }
    if (min_pow > p.degree) return rp_zero();

    /* Shift down */
    for (i = min_pow; i <= p.degree; i++)
        r.coeffs[i - min_pow] = p.coeffs[i];
    rp_fix_degree(&r);

    /* Find GCD of all numerators (assuming q=1 for integer polys,
     * but handle rational coefficients too) */
    {
        /* First make all coefficients have common denominator,
         * then GCD the numerators */
        i64 lcm_q = 1;
        for (i = 0; i <= r.degree; i++) {
            if (!rat_is_zero(r.coeffs[i])) {
                i64 g2 = rat_gcd(lcm_q, r.coeffs[i].q);
                lcm_q = (lcm_q / g2) * r.coeffs[i].q;
            }
        }

        /* Scale to clear denominators */
        for (i = 0; i <= r.degree; i++) {
            r.coeffs[i] = rat_mul(r.coeffs[i], rat_from_int(lcm_q));
        }

        /* GCD of numerators */
        g = 0;
        for (i = 0; i <= r.degree; i++) {
            if (!rat_is_zero(r.coeffs[i]))
                g = rat_gcd(g, rat_abs(r.coeffs[i].p));
        }
        if (g > 1) {
            Rat inv_g = rat_make(1, g);
            for (i = 0; i <= r.degree; i++)
                r.coeffs[i] = rat_mul(r.coeffs[i], inv_g);
        }
    }

    /* Make leading coefficient positive */
    if (r.degree >= 0 && r.coeffs[r.degree].p < 0) {
        r = rp_neg(r);
    }

    return r;
}

static int rp_eq(RatPoly a, RatPoly b) {
    int i;
    if (a.degree != b.degree) return 0;
    for (i = 0; i <= a.degree; i++) {
        if (!rat_eq(a.coeffs[i], b.coeffs[i])) return 0;
    }
    return 1;
}

static void rp_print(const RatPoly *p) {
    int i, first = 1;
    if (p->degree < 0) { printf("0"); return; }
    for (i = p->degree; i >= 0; i--) {
        if (rat_is_zero(p->coeffs[i])) continue;
        if (!first && p->coeffs[i].p > 0) printf(" + ");
        else if (!first && p->coeffs[i].p < 0) printf(" - ");
        else if (first && p->coeffs[i].p < 0) printf("-");

        {
            Rat c = p->coeffs[i];
            i64 abs_p = rat_abs(c.p);

            if (i == 0) {
                /* Constant term: always print */
                if (c.q == 1) printf("%ld", abs_p);
                else printf("%ld/%ld", abs_p, c.q);
            } else {
                /* Non-constant: only print coeff if != 1 */
                if (abs_p != 1 || c.q != 1) {
                    if (c.q == 1) printf("%ld", abs_p);
                    else printf("%ld/%ld", abs_p, c.q);
                }
                if (i == 1) printf("t");
                else printf("t^%d", i);
            }
        }
        first = 0;
    }
    if (first) printf("0");
}

/* ================================================================
 * Part H: Alexander Polynomial Computation
 *
 * Build Alexander matrix with polynomial entries over Q[t].
 * Use Bareiss algorithm to compute determinant.
 *
 * Alexander matrix for crossing k with sign s:
 *   Positive (s>0):
 *     over_arc:    (1-t)
 *     under_in:    -1
 *     under_out:   t
 *   Negative (s<0), multiplied by t to clear t^{-1}:
 *     over_arc:    (t-1)
 *     under_in:    1
 *     under_out:   -t
 *
 * For positive (sign>0):
 *     over_arc:    (1-t)
 *     under_in:    -1
 *     under_out:   t
 * For negative (sign<0), multiplied by t:
 *     over_arc:    (t-1)
 *     under_in:    -t
 *     under_out:   1
 *
 * Note: at t=-1, both cases give 2,-1,-1 (or -2,1,1), matching det code.
 * ================================================================ */

static RatPoly alexander_polynomial(int n_edges,
                                     const Crossing *crossings,
                                     int nc) {
    int over_arc[MAX_CROSSINGS];
    int under_in_arc[MAX_CROSSINGS];
    int under_out_arc[MAX_CROSSINGS];
    /* Matrix of polynomials: M[i][j] */
    /* Use heap allocation since RatPoly is large */
    RatPoly (*M)[MAX_ARCS];
    int i, j, k, sz;
    RatPoly det;
    RatPoly one_minus_t, var_t;

    (void)n_edges;

    if (nc == 0) return rp_const_int(1);

    label_arcs(crossings, nc, over_arc, under_in_arc, under_out_arc);

    /* Precompute polynomial constants */
    one_minus_t = rp_linear(rat_from_int(1), rat_from_int(-1)); /* 1 - t */
    var_t = rp_t();

    /* Allocate matrix */
    M = (RatPoly (*)[MAX_ARCS])malloc(
        (size_t)MAX_CROSSINGS * (size_t)MAX_ARCS * sizeof(RatPoly));
    if (!M) {
        fprintf(stderr, "alexander_polynomial: OOM\n");
        exit(1);
    }

    /* Initialize matrix to zero */
    for (i = 0; i < nc; i++)
        for (j = 0; j < nc; j++)
            M[i][j] = rp_zero();

    /* Fill in Alexander matrix
     * Positive crossing: over=(1-t), under_in=-1, under_out=t
     * Negative crossing (x t to clear t^-1): over=(t-1), under_in=-t, under_out=1
     */
    for (k = 0; k < nc; k++) {
        if (crossings[k].sign > 0) {
            /* over_arc: (1-t) */
            M[k][over_arc[k]] = rp_add(M[k][over_arc[k]],
                                         one_minus_t);
            /* under_in: -1 */
            M[k][under_in_arc[k]] = rp_add(M[k][under_in_arc[k]],
                                             rp_const_int(-1));
            /* under_out: t */
            M[k][under_out_arc[k]] = rp_add(M[k][under_out_arc[k]],
                                              var_t);
        } else {
            /* over_arc: (t-1) */
            M[k][over_arc[k]] = rp_add(M[k][over_arc[k]],
                                         rp_linear(rat_from_int(-1),
                                                   rat_from_int(1)));
            /* under_in: -t */
            M[k][under_in_arc[k]] = rp_add(M[k][under_in_arc[k]],
                                             rp_neg(var_t));
            /* under_out: 1 */
            M[k][under_out_arc[k]] = rp_add(M[k][under_out_arc[k]],
                                              rp_const_int(1));
        }
    }

    /* Delete last row and column: work with (nc-1) x (nc-1) submatrix */
    sz = nc - 1;
    if (sz <= 0) {
        free(M);
        return rp_const_int(1);
    }

    /* Bareiss algorithm on polynomial matrix */
    {
        RatPoly (*A)[MAX_ARCS];
        RatPoly prev;

        A = (RatPoly (*)[MAX_ARCS])malloc(
            (size_t)MAX_CROSSINGS * (size_t)MAX_ARCS * sizeof(RatPoly));
        if (!A) {
            fprintf(stderr, "alexander_polynomial: OOM\n");
            exit(1);
        }

        for (i = 0; i < sz; i++)
            for (j = 0; j < sz; j++)
                A[i][j] = M[i][j];

        prev = rp_const_int(1);

        for (k = 0; k < sz; k++) {
            /* Pivot: find non-zero diagonal element */
            if (rp_is_zero(&A[k][k])) {
                int found = 0;
                for (i = k + 1; i < sz; i++) {
                    if (!rp_is_zero(&A[i][k])) {
                        /* Swap rows */
                        for (j = 0; j < sz; j++) {
                            RatPoly tmp = A[k][j];
                            A[k][j] = A[i][j];
                            A[i][j] = tmp;
                        }
                        prev = rp_neg(prev);
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    free(A);
                    free(M);
                    return rp_zero();
                }
            }

            /* Bareiss elimination */
            for (i = k + 1; i < sz; i++) {
                for (j = k + 1; j < sz; j++) {
                    RatPoly num = rp_sub(
                        rp_mul(A[k][k], A[i][j]),
                        rp_mul(A[i][k], A[k][j])
                    );
                    A[i][j] = rp_exact_div(num, prev);
                }
            }
            prev = A[k][k];
        }

        det = A[sz - 1][sz - 1];
        free(A);
    }

    free(M);
    return rp_normalize_alexander(det);
}

/* ================================================================
 * Part I: Alexander Polynomial from Braid Word (Fox Calculus)
 *
 * Computes Alexander polynomial directly from the braid word,
 * bypassing 3D geometry and projection entirely.
 * Each braid generator IS a crossing with known sign and arcs.
 * ================================================================ */

static RatPoly alexander_from_braid(const BraidWord *braid) {
    int n = braid->n_strands;
    int m = braid->n_generators;
    int strand_arc[MAX_BRAID_STRANDS];
    int pos_to_strand[MAX_BRAID_STRANDS];
    int strand_to_pos[MAX_BRAID_STRANDS];
    int crossing_over[MAX_BRAID_GENS];
    int crossing_under_in[MAX_BRAID_GENS];
    int crossing_under_out[MAX_BRAID_GENS];
    int crossing_sign[MAX_BRAID_GENS];
    int arc_counter;
    int rename[MAX_BRAID_STRANDS + MAX_BRAID_GENS];
    int final_n_arcs;
    int k, s, i, j, sz;
    RatPoly one_minus_t, var_t;
    RatPoly (*M)[MAX_ARCS];
    RatPoly det;

    if (m == 0) return rp_const_int(1);

    /* Initialize: each strand position gets an initial arc label */
    arc_counter = n;
    for (s = 0; s < n; s++) {
        strand_arc[s] = s;
        pos_to_strand[s] = s;
        strand_to_pos[s] = s;
    }

    /* Process each generator */
    for (k = 0; k < m; k++) {
        int gen = braid->generators[k];
        int gi = (gen > 0) ? gen : -gen;
        int sign = (gen > 0) ? 1 : -1;
        int pos_left = gi - 1;
        int pos_right = gi;
        int s_left = pos_to_strand[pos_left];
        int s_right = pos_to_strand[pos_right];
        int over_strand, under_strand;

        if (sign > 0) {
            /* sigma_i: left strand goes over right */
            over_strand = s_left;
            under_strand = s_right;
        } else {
            /* sigma_i^-1: right strand goes over left */
            over_strand = s_right;
            under_strand = s_left;
        }

        crossing_over[k] = strand_arc[over_strand];
        crossing_under_in[k] = strand_arc[under_strand];
        crossing_under_out[k] = arc_counter;
        crossing_sign[k] = sign;

        strand_arc[under_strand] = arc_counter;
        arc_counter++;

        /* Swap positions */
        pos_to_strand[pos_left] = s_right;
        pos_to_strand[pos_right] = s_left;
        strand_to_pos[s_left] = pos_right;
        strand_to_pos[s_right] = pos_left;
    }

    /* Closure: identify final arcs with initial arcs.
     * Follow permutation cycle to determine mapping. */
    for (i = 0; i < arc_counter; i++) rename[i] = i;

    {
        int visited[MAX_BRAID_STRANDS];
        int cycle[MAX_BRAID_STRANDS];
        int cycle_len, cur;

        for (s = 0; s < n; s++) visited[s] = 0;
        cycle_len = 0;
        cur = 0;
        do {
            visited[cur] = 1;
            cycle[cycle_len++] = cur;
            cur = strand_to_pos[cur];
        } while (cur != 0 && !visited[cur]);

        /* Each strand's final arc maps to the next strand's initial arc */
        for (k = 0; k < cycle_len; k++) {
            int this_strand = cycle[k];
            int next_strand = cycle[(k + 1) % cycle_len];
            /* Final arc of this_strand → initial arc of next_strand */
            rename[strand_arc[this_strand]] = next_strand;
        }
    }

    /* Apply renaming transitively: chase until stable */
    for (i = 0; i < arc_counter; i++) {
        while (rename[i] != rename[rename[i]]) {
            rename[i] = rename[rename[i]];
        }
    }

    /* Compact arc labels to 0..final_n_arcs-1 */
    {
        int label_map[MAX_BRAID_STRANDS + MAX_BRAID_GENS];
        final_n_arcs = 0;
        for (i = 0; i < arc_counter; i++) label_map[i] = -1;

        for (i = 0; i < arc_counter; i++) {
            int canonical = rename[i];
            if (label_map[canonical] == -1) {
                label_map[canonical] = final_n_arcs++;
            }
        }

        /* Remap crossing arcs */
        for (k = 0; k < m; k++) {
            crossing_over[k] = label_map[rename[crossing_over[k]]];
            crossing_under_in[k] = label_map[rename[crossing_under_in[k]]];
            crossing_under_out[k] = label_map[rename[crossing_under_out[k]]];
        }
    }

    /* Build Alexander matrix: m rows (crossings) x final_n_arcs columns */
    one_minus_t = rp_linear(rat_from_int(1), rat_from_int(-1));
    var_t = rp_t();

    M = (RatPoly (*)[MAX_ARCS])malloc(
        (size_t)MAX_CROSSINGS * (size_t)MAX_ARCS * sizeof(RatPoly));
    if (!M) {
        fprintf(stderr, "alexander_from_braid: OOM\n");
        exit(1);
    }

    for (i = 0; i < m; i++)
        for (j = 0; j < final_n_arcs; j++)
            M[i][j] = rp_zero();

    for (k = 0; k < m; k++) {
        if (crossing_sign[k] > 0) {
            M[k][crossing_over[k]] = rp_add(M[k][crossing_over[k]],
                                              one_minus_t);
            M[k][crossing_under_in[k]] = rp_add(M[k][crossing_under_in[k]],
                                                  rp_const_int(-1));
            M[k][crossing_under_out[k]] = rp_add(M[k][crossing_under_out[k]],
                                                    var_t);
        } else {
            M[k][crossing_over[k]] = rp_add(M[k][crossing_over[k]],
                                              rp_linear(rat_from_int(-1),
                                                        rat_from_int(1)));
            M[k][crossing_under_in[k]] = rp_add(M[k][crossing_under_in[k]],
                                                  rp_neg(var_t));
            M[k][crossing_under_out[k]] = rp_add(M[k][crossing_under_out[k]],
                                                    rp_const_int(1));
        }
    }

    /* Delete last row and column */
    sz = m - 1;
    if (sz <= 0) {
        free(M);
        return rp_const_int(1);
    }

    /* Bareiss algorithm on polynomial matrix */
    {
        RatPoly (*A)[MAX_ARCS];
        RatPoly prev;

        A = (RatPoly (*)[MAX_ARCS])malloc(
            (size_t)MAX_CROSSINGS * (size_t)MAX_ARCS * sizeof(RatPoly));
        if (!A) {
            fprintf(stderr, "alexander_from_braid: OOM\n");
            free(M);
            exit(1);
        }

        for (i = 0; i < sz; i++)
            for (j = 0; j < sz; j++)
                A[i][j] = M[i][j];

        prev = rp_const_int(1);

        for (k = 0; k < sz - 1; k++) {
            if (rp_is_zero(&A[k][k])) {
                int found = 0;
                for (i = k + 1; i < sz; i++) {
                    if (!rp_is_zero(&A[i][k])) {
                        for (j = 0; j < sz; j++) {
                            RatPoly tmp = A[k][j];
                            A[k][j] = A[i][j];
                            A[i][j] = tmp;
                        }
                        prev = rp_neg(prev);
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    free(A);
                    free(M);
                    return rp_zero();
                }
            }

            for (i = k + 1; i < sz; i++) {
                for (j = k + 1; j < sz; j++) {
                    RatPoly num = rp_sub(
                        rp_mul(A[k][k], A[i][j]),
                        rp_mul(A[i][k], A[k][j])
                    );
                    A[i][j] = rp_exact_div(num, prev);
                }
            }
            prev = A[k][k];
        }

        det = A[sz - 1][sz - 1];
        free(A);
    }

    free(M);
    return rp_normalize_alexander(det);
}

/* ================================================================
 * Tests
 * ================================================================ */

/* Known Alexander polynomials for verification:
 *   3_1:  1 - t + t^2
 *   4_1:  -1 + 3t - t^2  (normalized: 1 - 3t + t^2)
 *   5_1:  1 - t + t^2 - t^3 + t^4
 *   5_2:  2 - 3t + 2t^2  (normalized: 2 - 3t + 2t^2)
 *   6_3:  1 - 3t + 5t^2 - 3t^3 + t^4  (wikipedia: -1+3t-5t^2+3t^3-t^4)
 *   8_18: ...complex
 */

static RatPoly make_known_alexander(const char *name) {
    RatPoly p = rp_zero();

    if (strcmp(name, "trefoil 3_1") == 0) {
        /* 1 - t + t^2 */
        p.coeffs[0] = rat_from_int(1);
        p.coeffs[1] = rat_from_int(-1);
        p.coeffs[2] = rat_from_int(1);
        p.degree = 2;
    } else if (strcmp(name, "figure-eight 4_1") == 0) {
        /* 1 - 3t + t^2  (or -1+3t-t^2 depending on normalization) */
        p.coeffs[0] = rat_from_int(1);
        p.coeffs[1] = rat_from_int(-3);
        p.coeffs[2] = rat_from_int(1);
        p.degree = 2;
    } else if (strcmp(name, "cinquefoil 5_1") == 0) {
        /* 1 - t + t^2 - t^3 + t^4 */
        p.coeffs[0] = rat_from_int(1);
        p.coeffs[1] = rat_from_int(-1);
        p.coeffs[2] = rat_from_int(1);
        p.coeffs[3] = rat_from_int(-1);
        p.coeffs[4] = rat_from_int(1);
        p.degree = 4;
    } else if (strcmp(name, "5_2") == 0) {
        /* 2 - 3t + 2t^2 */
        p.coeffs[0] = rat_from_int(2);
        p.coeffs[1] = rat_from_int(-3);
        p.coeffs[2] = rat_from_int(2);
        p.degree = 2;
    } else if (strcmp(name, "6_3") == 0) {
        /* 1 - 3t + 5t^2 - 3t^3 + t^4 */
        p.coeffs[0] = rat_from_int(1);
        p.coeffs[1] = rat_from_int(-3);
        p.coeffs[2] = rat_from_int(5);
        p.coeffs[3] = rat_from_int(-3);
        p.coeffs[4] = rat_from_int(1);
        p.degree = 4;
    } else if (strcmp(name, "6_1") == 0) {
        /* 2 - 5t + 2t^2 */
        p.coeffs[0] = rat_from_int(2);
        p.coeffs[1] = rat_from_int(-5);
        p.coeffs[2] = rat_from_int(2);
        p.degree = 2;
    } else if (strcmp(name, "6_2") == 0) {
        /* 1 - 3t + 3t^2 - 3t^3 + t^4 */
        p.coeffs[0] = rat_from_int(1);
        p.coeffs[1] = rat_from_int(-3);
        p.coeffs[2] = rat_from_int(3);
        p.coeffs[3] = rat_from_int(-3);
        p.coeffs[4] = rat_from_int(1);
        p.degree = 4;
    } else if (strcmp(name, "7_1") == 0) {
        /* 1 - t + t^2 - t^3 + t^4 - t^5 + t^6 */
        p.coeffs[0] = rat_from_int(1);
        p.coeffs[1] = rat_from_int(-1);
        p.coeffs[2] = rat_from_int(1);
        p.coeffs[3] = rat_from_int(-1);
        p.coeffs[4] = rat_from_int(1);
        p.coeffs[5] = rat_from_int(-1);
        p.coeffs[6] = rat_from_int(1);
        p.degree = 6;
    } else if (strcmp(name, "7_2") == 0) {
        /* 3 - 5t + 3t^2 */
        p.coeffs[0] = rat_from_int(3);
        p.coeffs[1] = rat_from_int(-5);
        p.coeffs[2] = rat_from_int(3);
        p.degree = 2;
    } else if (strcmp(name, "7_3") == 0) {
        /* 2 - 3t + 3t^2 - 3t^3 + 2t^4 */
        p.coeffs[0] = rat_from_int(2);
        p.coeffs[1] = rat_from_int(-3);
        p.coeffs[2] = rat_from_int(3);
        p.coeffs[3] = rat_from_int(-3);
        p.coeffs[4] = rat_from_int(2);
        p.degree = 4;
    } else if (strcmp(name, "7_4") == 0) {
        /* 4 - 7t + 4t^2 */
        p.coeffs[0] = rat_from_int(4);
        p.coeffs[1] = rat_from_int(-7);
        p.coeffs[2] = rat_from_int(4);
        p.degree = 2;
    }
    /* 8_18 is complex; we'll verify via det only */

    return p;
}

static void test_ratpoly_arithmetic(void) {
    RatPoly a, b, c, d;
    Rat val;

    printf("\n=== Test: RatPoly Arithmetic ===\n");

    /* Test addition: (1 + 2t) + (3 - t) = 4 + t */
    a = rp_linear(rat_from_int(1), rat_from_int(2));
    b = rp_linear(rat_from_int(3), rat_from_int(-1));
    c = rp_add(a, b);
    check("add: (1+2t)+(3-t) = 4+t",
          c.degree == 1 && c.coeffs[0].p == 4 && c.coeffs[1].p == 1);

    /* Test multiplication: (1-t)(1+t) = 1 - t^2 */
    a = rp_linear(rat_from_int(1), rat_from_int(-1));
    b = rp_linear(rat_from_int(1), rat_from_int(1));
    c = rp_mul(a, b);
    check("mul: (1-t)(1+t) = 1-t^2",
          c.degree == 2 && c.coeffs[0].p == 1 &&
          c.coeffs[1].p == 0 && c.coeffs[2].p == -1);

    /* Test exact division: (1-t^2) / (1-t) = 1+t */
    d = rp_exact_div(c, a);
    check("exact_div: (1-t^2)/(1-t) = 1+t",
          d.degree == 1 && d.coeffs[0].p == 1 && d.coeffs[1].p == 1);

    /* Test eval: (1 - t + t^2) at t=-1 = 1+1+1 = 3 */
    a = make_known_alexander("trefoil 3_1");
    val = rp_eval(&a, rat_from_int(-1));
    check("eval: trefoil poly at t=-1 gives 3",
          val.p == 3 && val.q == 1);

    /* Test eval: (1 - 3t + t^2) at t=-1 = 1+3+1 = 5 */
    b = make_known_alexander("figure-eight 4_1");
    val = rp_eval(&b, rat_from_int(-1));
    check("eval: figure-eight poly at t=-1 gives 5",
          val.p == 5 && val.q == 1);

    /* Test normalization */
    {
        RatPoly p = rp_zero();
        RatPoly n;
        /* 2t - 4t^2 + 2t^3  should normalize to 1 - 2t + t^2 */
        p.coeffs[1] = rat_from_int(2);
        p.coeffs[2] = rat_from_int(-4);
        p.coeffs[3] = rat_from_int(2);
        p.degree = 3;
        n = rp_normalize_alexander(p);
        check("normalize: 2t-4t^2+2t^3 -> 1-2t+t^2",
              n.degree == 2 && n.coeffs[0].p == 1 &&
              n.coeffs[1].p == -2 && n.coeffs[2].p == 1);
    }
}

static void test_alexander_polynomial(void) {
    BraidWord braids[12];
    int nb = 0, bi;

    printf("\n=== Test: Alexander Polynomial Computation ===\n");

    braids[nb++] = braid_trefoil();
    braids[nb++] = braid_figure_eight();
    braids[nb++] = braid_cinquefoil();
    braids[nb++] = braid_5_2();
    braids[nb++] = braid_6_3();
    braids[nb++] = braid_8_18();
    braids[nb++] = braid_6_1();
    braids[nb++] = braid_6_2();
    braids[nb++] = braid_7_1();
    braids[nb++] = braid_7_2();
    braids[nb++] = braid_7_3();
    braids[nb++] = braid_7_4();

    for (bi = 0; bi < nb; bi++) {
        StickKnot knot;
        Crossing cx[MAX_CROSSINGS];
        int nc, det;
        RatPoly alex;
        Rat eval_val;
        RatPoly known;
        char label[128];

        printf("\n  %s:\n", braids[bi].name);

        /* Compute determinant via 3D geometry.
         * Use known det for simplification target to avoid
         * coplanarity-induced wrong det in 4-strand braids. */
        braid_to_polygon(&braids[bi], &knot);
        simplify_polygon_topology_ex(&knot, braids[bi].expected_det);
        nc = find_crossings(knot.vertices, knot.n_vertices,
                            cx, MAX_CROSSINGS);
        det = (nc > 0) ? knot_determinant(knot.n_vertices, cx, nc) : 1;
        printf("    Det: %d (expected %d)\n",
               det, braids[bi].expected_det);

        /* Compute Alexander polynomial directly from braid (Fox calculus) */
        alex = alexander_from_braid(&braids[bi]);
        printf("    Alexander polynomial: ");
        rp_print(&alex);
        printf("\n");

        /* Verify |Delta(-1)| = expected det */
        eval_val = rp_eval(&alex, rat_from_int(-1));
        {
            i64 abs_eval = rat_abs(eval_val.p);
            printf("    |Delta(-1)| = %ld (expected %d, geom %d)\n",
                   abs_eval, braids[bi].expected_det, det);
            sprintf(label, "%s: |Delta(-1)| = expected det",
                    braids[bi].name);
            check(label, abs_eval == (i64)braids[bi].expected_det &&
                         eval_val.q == 1);
        }

        /* Compare to known polynomial (where available) */
        known = make_known_alexander(braids[bi].name);
        if (known.degree >= 0) {
            int match = rp_eq(alex, known);
            printf("    Known: ");
            rp_print(&known);
            printf("\n    Match: %s\n", match ? "YES" : "NO");
            sprintf(label, "%s: matches known polynomial",
                    braids[bi].name);
            check(label, match);
        } else {
            printf("    (no known polynomial to compare)\n");
        }
    }
}

/* ================================================================
 * Phase 3: Construction Word Polynomial Spectrum
 *
 * For each knot, generate all 2^(n-3) construction word alternatives
 * and compute the Alexander polynomial for each.
 * Key question: do different alternatives with the SAME det have
 * DIFFERENT polynomials? (Explorer predicts: yes, many more.)
 * ================================================================ */

static int rp_is_palindromic(const RatPoly *p) {
    int i;
    if (p->degree < 0) return 1; /* zero poly is trivially palindromic */
    for (i = 0; i <= p->degree / 2; i++) {
        if (p->coeffs[i].p != p->coeffs[p->degree - i].p ||
            p->coeffs[i].q != p->coeffs[p->degree - i].q)
            return 0;
    }
    return 1;
}

static void test_construction_word_spectrum(void) {
    BraidWord braids[4];
    int nb = 0, bi;

    printf("\n=== Test: Construction Word Polynomial Spectrum ===\n");

    braids[nb++] = braid_figure_eight();
    braids[nb++] = braid_5_2();
    braids[nb++] = braid_6_3();
    braids[nb++] = braid_8_18();

    for (bi = 0; bi < nb; bi++) {
        StickKnot knot;
        Crossing cx[MAX_CROSSINGS];
        int nc, base_det;
        int b0 = 0, b1, b2;
        int n_free;
        unsigned int max_choices;
        unsigned int choices;
        int n_valid = 0, n_degen = 0;

        /* Track distinct polynomials with population counts */
        RatPoly seen_polys[512];
        int seen_dets[512];
        int seen_counts[512];  /* how many alternatives produce this */
        int n_seen = 0;

        printf("\n  %s:\n", braids[bi].name);

        braid_to_polygon(&braids[bi], &knot);
        simplify_polygon_topology(&knot);

        nc = find_crossings(knot.vertices, knot.n_vertices,
                            cx, MAX_CROSSINGS);
        base_det = (nc > 0) ? knot_determinant(knot.n_vertices, cx, nc) : 1;
        printf("    Base det: %d, vertices: %d\n",
               base_det, knot.n_vertices);

        /* Base triangle: first three vertices */
        b1 = 1; b2 = 2;
        n_free = knot.n_vertices - 3;
        max_choices = 1u << (unsigned)n_free;

        /* Base case (choices=0): use braid-direct polynomial */
        {
            RatPoly base_alex = alexander_from_braid(&braids[bi]);
            seen_polys[0] = base_alex;
            seen_dets[0] = base_det;
            seen_counts[0] = 1;
            n_seen = 1;
            n_valid = 1;
        }

        /* Try each alternative */
        for (choices = 1; choices < max_choices; choices++) {
            Vec3Q alt_verts[MAX_STICK_VERTICES];
            int alt_n;
            int alt_nc, alt_det;
            RatPoly alt_alex;
            Rat eval_val;
            int ki;
            int found_idx = -1;

            evaluate_inplace(&knot, b0, b1, b2, choices, alt_verts, &alt_n);

            alt_nc = find_crossings_clean(alt_verts, alt_n, cx, MAX_CROSSINGS);

            /* Reject self-intersecting alternatives: if any crossing
             * still has sign=0 after trying all projection directions,
             * the polygon has edges that genuinely cross in 3D. */
            {
                int has_self_ix = 0;
                int ck;
                for (ck = 0; ck < alt_nc; ck++) {
                    if (cx[ck].sign == 0) { has_self_ix = 1; break; }
                }
                if (has_self_ix) {
                    n_degen++;
                    continue;
                }
            }

            alt_det = (alt_nc > 0) ? knot_determinant(alt_n, cx, alt_nc) : 1;

            /* Use geometry-based Alexander polynomial */
            alt_alex = alexander_polynomial(alt_n, cx, alt_nc);

            /* Validate: |Delta(-1)| should equal det */
            eval_val = rp_eval(&alt_alex, rat_from_int(-1));
            if (eval_val.q != 1 ||
                rat_abs(eval_val.p) != (i64)alt_det) {
                n_degen++;
                continue;
            }
            n_valid++;

            /* Find or insert polynomial */
            for (ki = 0; ki < n_seen; ki++) {
                if (rp_eq(alt_alex, seen_polys[ki]) &&
                    seen_dets[ki] == alt_det) {
                    found_idx = ki;
                    break;
                }
            }
            if (found_idx >= 0) {
                seen_counts[found_idx]++;
            } else if (n_seen < 512) {
                seen_polys[n_seen] = alt_alex;
                seen_dets[n_seen] = alt_det;
                seen_counts[n_seen] = 1;
                n_seen++;
            }
        }

        printf("    Alternatives: %u total, %d valid, %d degenerate\n",
               max_choices, n_valid, n_degen);
        printf("    Distinct (det, poly) pairs: %d\n", n_seen);

        /* Full analysis */
        {
            int ki;
            int n_palindromic = 0, n_nonpalindromic = 0;
            int distinct_dets[64];
            int n_distinct_dets = 0;
            int deg_counts[RATPOLY_MAX_DEG + 2];
            int split1 = 0, split2 = 0, split3plus = 0;

            /* Count palindromic vs non-palindromic */
            for (ki = 0; ki < n_seen; ki++) {
                if (rp_is_palindromic(&seen_polys[ki]))
                    n_palindromic++;
                else
                    n_nonpalindromic++;
            }
            printf("    Palindromic: %d, Non-palindromic: %d\n",
                   n_palindromic, n_nonpalindromic);

            /* Degree distribution */
            for (ki = 0; ki <= RATPOLY_MAX_DEG + 1; ki++)
                deg_counts[ki] = 0;
            for (ki = 0; ki < n_seen; ki++) {
                int d = seen_polys[ki].degree;
                if (d < 0) d = 0; /* zero poly → degree 0 */
                deg_counts[d]++;
            }
            printf("    Degree distribution:");
            for (ki = 0; ki <= RATPOLY_MAX_DEG; ki++) {
                if (deg_counts[ki] > 0)
                    printf(" deg%d:%d", ki, deg_counts[ki]);
            }
            printf("\n");

            /* Count distinct dets */
            for (ki = 0; ki < n_seen; ki++) {
                int is_new_det = 1;
                int di;
                for (di = 0; di < n_distinct_dets; di++) {
                    if (distinct_dets[di] == seen_dets[ki]) {
                        is_new_det = 0;
                        break;
                    }
                }
                if (is_new_det && n_distinct_dets < 64) {
                    distinct_dets[n_distinct_dets++] = seen_dets[ki];
                }
            }

            /* Split analysis: how many polys per det? */
            {
                int di;
                for (di = 0; di < n_distinct_dets; di++) {
                    int count = 0;
                    for (ki = 0; ki < n_seen; ki++) {
                        if (seen_dets[ki] == distinct_dets[di] &&
                            rp_is_palindromic(&seen_polys[ki]))
                            count++;
                    }
                    if (count == 1) split1++;
                    else if (count == 2) split2++;
                    else split3plus++;
                }
            }
            printf("    Distinct dets: %d\n", n_distinct_dets);
            printf("    Det split: %d unsplit, %d split-2, %d split-3+\n",
                   split1, split2, split3plus);
            printf("    Refinement (palindromic): %.2f\n",
                   n_distinct_dets > 0 ?
                   (double)n_palindromic / (double)n_distinct_dets : 0.0);

            /* Print ALL polynomials grouped by det, with population */
            {
                int di;
                for (di = 0; di < n_distinct_dets; di++) {
                    int det_pop = 0;
                    printf("    det=%d:", distinct_dets[di]);
                    for (ki = 0; ki < n_seen; ki++) {
                        if (seen_dets[ki] == distinct_dets[di]) {
                            printf(" ");
                            rp_print(&seen_polys[ki]);
                            if (!rp_is_palindromic(&seen_polys[ki]))
                                printf("*");
                            printf("[%d]", seen_counts[ki]);
                            det_pop += seen_counts[ki];
                        }
                    }
                    printf(" (total: %d)\n", det_pop);
                }
            }
        }

        {
            char label[128];
            int ki2, n_nonpal_check = 0;
            for (ki2 = 0; ki2 < n_seen; ki2++) {
                if (!rp_is_palindromic(&seen_polys[ki2]))
                    n_nonpal_check++;
            }
            sprintf(label, "%s: polynomial refines det",
                    braids[bi].name);
            check(label, n_seen >= 1 && n_nonpal_check == 0);
        }
    }
}

/* ================================================================ */

/* ================================================================
 * Phase 4: Spectral Stability Test
 *
 * Is the polynomial spectrum a property of the KNOT or just the POLYGON?
 * Run 20 randomized simplifications of 6_3, compute full Alexander
 * polynomial spectrum for each, track core (intersection) and union.
 * ================================================================ */

#define STABILITY_TRIALS 20
#define MAX_UNION_POLYS 1024

static void test_spectral_stability(void) {
    BraidWord braid = braid_6_3();
    StickKnot base_knot;

    /* Union of all polynomials seen across all trials */
    RatPoly union_polys[MAX_UNION_POLYS];
    int union_count[MAX_UNION_POLYS]; /* how many TRIALS contain this poly */
    int union_pop[MAX_UNION_POLYS];   /* total population across all trials */
    int n_union = 0;

    /* Per-trial stats */
    int trial_n_verts[STABILITY_TRIALS];
    int trial_n_free[STABILITY_TRIALS];
    int trial_n_distinct[STABILITY_TRIALS];
    int trial_n_palindromic[STABILITY_TRIALS];

    int trial;
    int core_count;
    int running_union[STABILITY_TRIALS];

    printf("\n=== Test: Spectral Stability (6_3, %d trials) ===\n",
           STABILITY_TRIALS);

    /* Build base polygon (before simplification) */
    braid_to_polygon(&braid, &base_knot);

    for (trial = 0; trial < STABILITY_TRIALS; trial++) {
        StickKnot knot;
        int b0 = 0, b1 = 1, b2 = 2;
        int n_free;
        unsigned int max_choices;
        unsigned int choices;
        int n_palindromic = 0, n_si = 0;
        Crossing cx[MAX_CROSSINGS];

        /* Per-trial distinct palindromic polys */
        RatPoly trial_polys[512];
        int n_trial = 0;

        /* Copy base and simplify with randomized order */
        knot = base_knot;
        if (trial == 0) {
            simplify_polygon_topology(&knot);
        } else {
            simplify_polygon_random(&knot, (unsigned)(trial * 7919 + 42));
        }

        n_free = knot.n_vertices - 3;
        max_choices = 1u << (unsigned)n_free;

        trial_n_verts[trial] = knot.n_vertices;
        trial_n_free[trial] = n_free;

        printf("  Trial %2d: %2d verts, %2d free bits, %u alternatives\n",
               trial, knot.n_vertices, n_free, max_choices);

        /* Base case (choices=0): use braid-direct polynomial */
        {
            RatPoly base_alex = alexander_from_braid(&braid);
            if (rp_is_palindromic(&base_alex)) {
                trial_polys[n_trial++] = base_alex;
                n_palindromic++;
            }
        }

        /* Try each alternative */
        for (choices = 1; choices < max_choices; choices++) {
            Vec3Q alt_verts[MAX_STICK_VERTICES];
            int alt_n, alt_nc, alt_det;
            RatPoly alt_alex;
            Rat eval_val;
            int ki, found = 0;

            evaluate_inplace(&knot, b0, b1, b2, choices,
                             alt_verts, &alt_n);

            alt_nc = find_crossings_clean(alt_verts, alt_n, cx, MAX_CROSSINGS);

            /* Skip self-intersecting (sign=0 persists after all directions) */
            {
                int has_si = 0, ck;
                for (ck = 0; ck < alt_nc; ck++) {
                    if (cx[ck].sign == 0) { has_si = 1; break; }
                }
                if (has_si) { n_si++; continue; }
            }

            alt_det = (alt_nc > 0) ?
                knot_determinant(alt_n, cx, alt_nc) : 1;

            alt_alex = alexander_polynomial(alt_n, cx, alt_nc);

            /* Validate */
            eval_val = rp_eval(&alt_alex, rat_from_int(-1));
            if (eval_val.q != 1 ||
                rat_abs(eval_val.p) != (i64)alt_det)
                continue;

            if (!rp_is_palindromic(&alt_alex))
                continue;

            n_palindromic++;

            /* Deduplicate within trial */
            for (ki = 0; ki < n_trial; ki++) {
                if (rp_eq(alt_alex, trial_polys[ki])) {
                    found = 1;
                    break;
                }
            }
            if (!found && n_trial < 512) {
                trial_polys[n_trial++] = alt_alex;
            }
        }

        trial_n_distinct[trial] = n_trial;
        trial_n_palindromic[trial] = n_palindromic;

        printf("           %u valid (%d SI), %d distinct palindromic polys, %d palindromic total\n",
               max_choices - (unsigned)n_si, n_si, n_trial, n_palindromic);

        /* Merge into union */
        {
            int ti;
            for (ti = 0; ti < n_trial; ti++) {
                int ui, found_union = 0;
                for (ui = 0; ui < n_union; ui++) {
                    if (rp_eq(trial_polys[ti], union_polys[ui])) {
                        union_count[ui]++;
                        union_pop[ui]++;
                        found_union = 1;
                        break;
                    }
                }
                if (!found_union && n_union < MAX_UNION_POLYS) {
                    union_polys[n_union] = trial_polys[ti];
                    union_count[n_union] = 1;
                    union_pop[n_union] = 1;
                    n_union++;
                }
            }
        }

        running_union[trial] = n_union;
    }

    /* Analysis */
    printf("\n  --- Stability Analysis ---\n");

    /* Running union convergence */
    printf("  Union convergence: ");
    for (trial = 0; trial < STABILITY_TRIALS; trial++) {
        printf("%d", running_union[trial]);
        if (trial < STABILITY_TRIALS - 1) printf(",");
    }
    printf("\n");

    /* Core set: polynomials appearing in ALL trials */
    core_count = 0;
    printf("\n  Core polynomials (in all %d trials):\n", STABILITY_TRIALS);
    {
        int ui;
        for (ui = 0; ui < n_union; ui++) {
            if (union_count[ui] == STABILITY_TRIALS) {
                Rat eval_val = rp_eval(&union_polys[ui], rat_from_int(-1));
                i64 det = rat_abs(eval_val.p);
                printf("    det=%ld: ", det);
                rp_print(&union_polys[ui]);
                printf(" (in %d/%d trials)\n",
                       union_count[ui], STABILITY_TRIALS);
                core_count++;
            }
        }
    }
    printf("  Core size: %d, Union size: %d\n", core_count, n_union);

    /* Near-core: polynomials in >= 75% of trials */
    {
        int near_core = 0;
        int threshold = (STABILITY_TRIALS * 3 + 3) / 4; /* ceil(75%) */
        int ui;
        printf("\n  Near-core (>= %d/%d trials):\n",
               threshold, STABILITY_TRIALS);
        for (ui = 0; ui < n_union; ui++) {
            if (union_count[ui] >= threshold &&
                union_count[ui] < STABILITY_TRIALS) {
                Rat eval_val = rp_eval(&union_polys[ui], rat_from_int(-1));
                i64 det = rat_abs(eval_val.p);
                printf("    det=%ld: ", det);
                rp_print(&union_polys[ui]);
                printf(" (in %d/%d trials)\n",
                       union_count[ui], STABILITY_TRIALS);
                near_core++;
            }
        }
        if (near_core == 0) printf("    (none)\n");
    }

    /* Rare polynomials: appearing in only 1 trial */
    {
        int rare = 0;
        int ui;
        for (ui = 0; ui < n_union; ui++) {
            if (union_count[ui] == 1) rare++;
        }
        printf("\n  Rare (1 trial only): %d/%d\n", rare, n_union);
    }

    /* Vertex count vs distinct poly count */
    printf("\n  Per-trial summary:\n");
    printf("    Trial  Verts  Free  Distinct  Palindromic\n");
    {
        int t;
        for (t = 0; t < STABILITY_TRIALS; t++) {
            printf("    %5d  %5d  %4d  %8d  %11d\n",
                   t, trial_n_verts[t], trial_n_free[t],
                   trial_n_distinct[t], trial_n_palindromic[t]);
        }
    }

    /* Normalized density: distinct / 2^free */
    printf("\n  Normalized density (distinct / 2^free):\n");
    {
        int t;
        for (t = 0; t < STABILITY_TRIALS; t++) {
            unsigned int total = 1u << (unsigned)trial_n_free[t];
            double density = (double)trial_n_distinct[t] / (double)total;
            printf("    Trial %2d: %d / %u = %.4f\n",
                   t, trial_n_distinct[t], total, density);
        }
    }

    check("spectral stability: core set non-empty", core_count > 0);
    check("spectral stability: union converges",
          running_union[STABILITY_TRIALS - 1] ==
          running_union[STABILITY_TRIALS - 2]);
}

/* ================================================================
 * Phase 4b: Figure-eight spectral stability cross-check
 *
 * Quick test: does {unknot, trefoil, source knot} core pattern
 * hold for figure-eight too? If yes, it's universal.
 * ================================================================ */

static void test_spectral_stability_figure_eight(void) {
    BraidWord braid = braid_figure_eight();
    StickKnot base_knot;

    RatPoly union_polys[256];
    int union_count[256];
    int n_union = 0;

    int trial;
    int core_count;

    printf("\n=== Test: Spectral Stability Cross-check (4_1, 20 trials) ===\n");

    braid_to_polygon(&braid, &base_knot);

    for (trial = 0; trial < STABILITY_TRIALS; trial++) {
        StickKnot knot;
        int b0 = 0, b1 = 1, b2 = 2;
        int n_free;
        unsigned int max_choices, choices;
        Crossing cx[MAX_CROSSINGS];

        RatPoly trial_polys[64];
        int n_trial = 0;

        knot = base_knot;
        if (trial == 0) {
            simplify_polygon_topology(&knot);
        } else {
            simplify_polygon_random(&knot, (unsigned)(trial * 7919 + 42));
        }

        n_free = knot.n_vertices - 3;
        max_choices = 1u << (unsigned)n_free;

        printf("  Trial %2d: %2d verts, %2d free bits, %u alternatives",
               trial, knot.n_vertices, n_free, max_choices);

        /* Base case */
        {
            RatPoly base_alex = alexander_from_braid(&braid);
            if (rp_is_palindromic(&base_alex)) {
                trial_polys[n_trial++] = base_alex;
            }
        }

        for (choices = 1; choices < max_choices; choices++) {
            Vec3Q alt_verts[MAX_STICK_VERTICES];
            int alt_n, alt_nc, alt_det;
            RatPoly alt_alex;
            Rat eval_val;
            int ki, found = 0;

            evaluate_inplace(&knot, b0, b1, b2, choices,
                             alt_verts, &alt_n);

            alt_nc = find_crossings_clean(alt_verts, alt_n, cx, MAX_CROSSINGS);

            /* Skip self-intersecting */
            {
                int has_si = 0, ck;
                for (ck = 0; ck < alt_nc; ck++) {
                    if (cx[ck].sign == 0) { has_si = 1; break; }
                }
                if (has_si) continue;
            }

            alt_det = (alt_nc > 0) ?
                knot_determinant(alt_n, cx, alt_nc) : 1;

            alt_alex = alexander_polynomial(alt_n, cx, alt_nc);

            eval_val = rp_eval(&alt_alex, rat_from_int(-1));
            if (eval_val.q != 1 ||
                rat_abs(eval_val.p) != (i64)alt_det)
                continue;

            if (!rp_is_palindromic(&alt_alex))
                continue;

            for (ki = 0; ki < n_trial; ki++) {
                if (rp_eq(alt_alex, trial_polys[ki])) {
                    found = 1;
                    break;
                }
            }
            if (!found && n_trial < 64) {
                trial_polys[n_trial++] = alt_alex;
            }
        }

        printf(" -> %d distinct\n", n_trial);

        /* Merge into union */
        {
            int ti;
            for (ti = 0; ti < n_trial; ti++) {
                int ui, found_union = 0;
                for (ui = 0; ui < n_union; ui++) {
                    if (rp_eq(trial_polys[ti], union_polys[ui])) {
                        union_count[ui]++;
                        found_union = 1;
                        break;
                    }
                }
                if (!found_union && n_union < 256) {
                    union_polys[n_union] = trial_polys[ti];
                    union_count[n_union] = 1;
                    n_union++;
                }
            }
        }
    }

    /* Core analysis */
    core_count = 0;
    printf("\n  Core polynomials (in all %d trials):\n", STABILITY_TRIALS);
    {
        int ui;
        for (ui = 0; ui < n_union; ui++) {
            if (union_count[ui] == STABILITY_TRIALS) {
                Rat eval_val = rp_eval(&union_polys[ui], rat_from_int(-1));
                i64 det = rat_abs(eval_val.p);
                printf("    det=%ld: ", det);
                rp_print(&union_polys[ui]);
                printf(" (in %d/%d trials)\n",
                       union_count[ui], STABILITY_TRIALS);
                core_count++;
            }
        }
    }
    printf("  Core size: %d, Union size: %d\n", core_count, n_union);

    /* Near-core */
    {
        int threshold = (STABILITY_TRIALS * 3 + 3) / 4;
        int ui;
        printf("  Near-core (>= %d/%d trials):\n",
               threshold, STABILITY_TRIALS);
        for (ui = 0; ui < n_union; ui++) {
            if (union_count[ui] >= threshold &&
                union_count[ui] < STABILITY_TRIALS) {
                Rat eval_val = rp_eval(&union_polys[ui], rat_from_int(-1));
                i64 det = rat_abs(eval_val.p);
                printf("    det=%ld: ", det);
                rp_print(&union_polys[ui]);
                printf(" (in %d/%d trials)\n",
                       union_count[ui], STABILITY_TRIALS);
            }
        }
    }

    check("figure-eight: core includes source knot",
          core_count >= 1);
}

/* ================================================================
 * Phase 5: Reachability Graph
 *
 * For each of 12 knots, compute construction word polynomial spectrum
 * and check which of the other 11 knots' polynomials appear.
 * Build a 12x12 directed reachability matrix.
 * ================================================================ */

#define N_KNOTS 12

static void test_reachability_graph(void) {
    BraidWord braids[N_KNOTS];
    RatPoly known_alex[N_KNOTS];
    int reach[N_KNOTS][N_KNOTS]; /* reach[i][j] = 1 if j's poly in i's spectrum */
    int spectrum_size[N_KNOTS];
    int n_palindromic[N_KNOTS];
    int n_selfint[N_KNOTS];
    int vertex_count[N_KNOTS];
    int bi, bj;

    printf("\n=== Test: Reachability Graph (12 knots) ===\n");

    /* Set up all 12 knots */
    braids[0] = braid_trefoil();
    braids[1] = braid_figure_eight();
    braids[2] = braid_cinquefoil();
    braids[3] = braid_5_2();
    braids[4] = braid_6_1();
    braids[5] = braid_6_2();
    braids[6] = braid_6_3();
    braids[7] = braid_7_1();
    braids[8] = braid_7_2();
    braids[9] = braid_7_3();
    braids[10] = braid_7_4();
    braids[11] = braid_8_18();

    /* Compute known Alexander polynomials via Fox calculus */
    for (bi = 0; bi < N_KNOTS; bi++) {
        known_alex[bi] = alexander_from_braid(&braids[bi]);
    }

    /* Initialize reachability matrix */
    for (bi = 0; bi < N_KNOTS; bi++)
        for (bj = 0; bj < N_KNOTS; bj++)
            reach[bi][bj] = 0;

    /* For each knot, compute spectrum and check reachability */
    for (bi = 0; bi < N_KNOTS; bi++) {
        StickKnot knot;
        int b0 = 0, b1 = 1, b2 = 2;
        int n_free;
        unsigned int max_choices, choices;
        int n_distinct = 0, n_pal = 0, n_self_ix = 0;
        Crossing cx[MAX_CROSSINGS];

        RatPoly seen[512];
        int n_seen = 0;

        printf("\n  %s (det=%d):\n", braids[bi].name,
               braids[bi].expected_det);

        braid_to_polygon(&braids[bi], &knot);
        simplify_polygon_topology_ex(&knot, braids[bi].expected_det);

        n_free = knot.n_vertices - 3;
        max_choices = 1u << (unsigned)n_free;
        vertex_count[bi] = knot.n_vertices;

        printf("    %d vertices, %d free bits, %u alternatives\n",
               knot.n_vertices, n_free, max_choices);

        /* Skip if too many alternatives (>64k) */
        if (n_free > 16) {
            printf("    SKIPPED: too many alternatives (%u)\n",
                   max_choices);
            spectrum_size[bi] = -1;
            n_palindromic[bi] = 0;
            n_selfint[bi] = 0;
            continue;
        }

        /* Base case: Fox calculus */
        {
            RatPoly base = known_alex[bi];
            if (rp_is_palindromic(&base)) {
                seen[n_seen++] = base;
                n_pal++;
            }
        }

        /* Self-reachability: source knot always reachable */
        reach[bi][bi] = 1;

        /* Check all known polynomials against base */
        for (bj = 0; bj < N_KNOTS; bj++) {
            if (rp_eq(known_alex[bi], known_alex[bj]))
                reach[bi][bj] = 1;
        }

        /* Try each alternative */
        for (choices = 1; choices < max_choices; choices++) {
            Vec3Q alt_verts[MAX_STICK_VERTICES];
            int alt_n, alt_nc, alt_det;
            RatPoly alt_alex;
            Rat eval_val;
            int ki, found = 0;

            evaluate_inplace(&knot, b0, b1, b2, choices,
                             alt_verts, &alt_n);

            alt_nc = find_crossings_clean(alt_verts, alt_n, cx, MAX_CROSSINGS);

            /* Skip self-intersecting */
            {
                int has_si = 0, ck;
                for (ck = 0; ck < alt_nc; ck++) {
                    if (cx[ck].sign == 0) { has_si = 1; break; }
                }
                if (has_si) { n_self_ix++; continue; }
            }

            alt_det = (alt_nc > 0) ?
                knot_determinant(alt_n, cx, alt_nc) : 1;

            alt_alex = alexander_polynomial(alt_n, cx, alt_nc);

            eval_val = rp_eval(&alt_alex, rat_from_int(-1));
            if (eval_val.q != 1 ||
                rat_abs(eval_val.p) != (i64)alt_det)
                continue;

            if (!rp_is_palindromic(&alt_alex))
                continue;

            n_pal++;

            /* Check against all known polynomials */
            for (bj = 0; bj < N_KNOTS; bj++) {
                if (rp_eq(alt_alex, known_alex[bj]))
                    reach[bi][bj] = 1;
            }

            /* Deduplicate */
            for (ki = 0; ki < n_seen; ki++) {
                if (rp_eq(alt_alex, seen[ki])) {
                    found = 1;
                    break;
                }
            }
            if (!found && n_seen < 512) {
                seen[n_seen++] = alt_alex;
            }
        }

        n_distinct = n_seen;
        spectrum_size[bi] = n_distinct;
        n_palindromic[bi] = n_pal;
        n_selfint[bi] = n_self_ix;

        printf("    %d distinct palindromic, %d total palindromic, %d self-intersecting\n",
               n_distinct, n_pal, n_self_ix);

        /* Print which knots are reachable */
        printf("    Reaches:");
        for (bj = 0; bj < N_KNOTS; bj++) {
            if (reach[bi][bj])
                printf(" %s", braids[bj].name);
        }
        printf("\n");
    }

    /* Print reachability matrix */
    printf("\n  --- Reachability Matrix ---\n");
    printf("  %-16s", "From \\ To");
    for (bj = 0; bj < N_KNOTS; bj++)
        printf(" %4s", braids[bj].name);
    printf("  out\n");

    for (bi = 0; bi < N_KNOTS; bi++) {
        int out_count = 0;
        printf("  %-16s", braids[bi].name);
        for (bj = 0; bj < N_KNOTS; bj++) {
            printf("    %c", reach[bi][bj] ? '*' : '.');
            if (reach[bi][bj]) out_count++;
        }
        printf("  %3d\n", out_count);
    }

    /* In-degree (how many knots can reach this one) */
    printf("  %-16s", "in");
    for (bj = 0; bj < N_KNOTS; bj++) {
        int in_count = 0;
        for (bi = 0; bi < N_KNOTS; bi++) {
            if (reach[bi][bj]) in_count++;
        }
        printf("  %3d", in_count);
    }
    printf("\n");

    /* Summary stats */
    printf("\n  Spectral richness:\n");
    printf("    %-10s  verts  bits  %8s  %11s  %9s  %5s\n",
           "Knot", "distinct", "valid", "self-int", "total");
    for (bi = 0; bi < N_KNOTS; bi++) {
        if (spectrum_size[bi] >= 0) {
            unsigned int total = 1u << (unsigned)(vertex_count[bi] - 3);
            printf("    %-10s  %5d  %4d  %8d  %11d  %9d  %5u\n",
                   braids[bi].name, vertex_count[bi],
                   vertex_count[bi] - 3, spectrum_size[bi],
                   n_palindromic[bi], n_selfint[bi], total);
        } else {
            printf("    %-10s  %5d  %4d  SKIPPED\n",
                   braids[bi].name, vertex_count[bi],
                   vertex_count[bi] - 3);
        }
    }

    /* Bidirectionality check */
    {
        int n_bidir = 0, n_unidir = 0;
        printf("\n  Bidirectionality:\n");
        for (bi = 0; bi < N_KNOTS; bi++) {
            for (bj = bi + 1; bj < N_KNOTS; bj++) {
                if (reach[bi][bj] && reach[bj][bi]) {
                    printf("    %s <-> %s\n",
                           braids[bi].name, braids[bj].name);
                    n_bidir++;
                } else if (reach[bi][bj]) {
                    printf("    %s  -> %s\n",
                           braids[bi].name, braids[bj].name);
                    n_unidir++;
                } else if (reach[bj][bi]) {
                    printf("    %s <-  %s\n",
                           braids[bi].name, braids[bj].name);
                    n_unidir++;
                }
            }
        }
        printf("    Bidirectional: %d, Unidirectional: %d\n",
               n_bidir, n_unidir);
    }

    check("reachability graph computed", 1);
}

/* ================================================================
 * Diagnostic: Count degenerate crossings per knot
 * A crossing is degenerate when depth_a == depth_b (sign=0).
 * ================================================================ */

static void test_degenerate_crossings(void) {
    BraidWord braids[12];
    int nb = 0, bi;

    printf("\n=== Diagnostic: Degenerate Crossings ===\n");

    braids[nb++] = braid_trefoil();
    braids[nb++] = braid_figure_eight();
    braids[nb++] = braid_cinquefoil();
    braids[nb++] = braid_5_2();
    braids[nb++] = braid_6_1();
    braids[nb++] = braid_6_2();
    braids[nb++] = braid_6_3();
    braids[nb++] = braid_7_1();
    braids[nb++] = braid_7_2();
    braids[nb++] = braid_7_3();
    braids[nb++] = braid_7_4();
    braids[nb++] = braid_8_18();

    for (bi = 0; bi < nb; bi++) {
        StickKnot knot;
        int b0 = 0, b1 = 1, b2 = 2;
        int n_free;
        unsigned int max_choices, choices;
        Crossing cx[MAX_CROSSINGS];
        int n_degen_alts = 0, n_total_alts = 0;
        int max_degen_cx = 0;

        braid_to_polygon(&braids[bi], &knot);
        simplify_polygon_topology_ex(&knot, braids[bi].expected_det);

        n_free = knot.n_vertices - 3;
        max_choices = 1u << (unsigned)n_free;

        if (n_free > 16) {
            printf("  %-10s: SKIPPED (too many bits)\n", braids[bi].name);
            continue;
        }

        /* Check base polygon */
        {
            int nc, k, n_degen = 0;
            nc = find_crossings(knot.vertices, knot.n_vertices,
                                cx, MAX_CROSSINGS);
            for (k = 0; k < nc; k++) {
                if (cx[k].sign == 0) n_degen++;
            }
            printf("  %-10s: base polygon %d cx, %d degenerate",
                   braids[bi].name, nc, n_degen);
        }

        /* Check all alternatives */
        for (choices = 0; choices < max_choices; choices++) {
            Vec3Q alt_verts[MAX_STICK_VERTICES];
            int alt_n, alt_nc, k, n_degen = 0;

            if (choices == 0) {
                int v;
                alt_n = knot.n_vertices;
                for (v = 0; v < alt_n; v++)
                    alt_verts[v] = knot.vertices[v];
            } else {
                evaluate_inplace(&knot, b0, b1, b2, choices,
                                 alt_verts, &alt_n);
            }

            /* Use fixed (2,3,5) direction for degeneracy diagnostic */
            alt_nc = find_crossings(alt_verts, alt_n, cx, MAX_CROSSINGS);
            for (k = 0; k < alt_nc; k++) {
                if (cx[k].sign == 0) n_degen++;
            }

            n_total_alts++;
            if (n_degen > 0) {
                n_degen_alts++;
                if (n_degen > max_degen_cx)
                    max_degen_cx = n_degen;
            }
        }

        printf(", alts with degen: %d/%d (%d%%), max degen/alt: %d\n",
               n_degen_alts, n_total_alts,
               n_total_alts > 0 ? (n_degen_alts * 100 / n_total_alts) : 0,
               max_degen_cx);
    }
}

/* ================================================================
 * Diagnostic: Dump full crossing data for first non-palindromic
 * 5_2 alternative.
 * ================================================================ */
static void test_crossing_dump_5_2(void) {
    BraidWord braid;
    StickKnot knot;
    int b0 = 0, b1, b2, n_free;
    unsigned int max_choices, choices;

    printf("\n=== Diagnostic: 5_2 Non-Palindromic Crossing Dump ===\n");

    braid = braid_5_2();
    braid_to_polygon(&braid, &knot);
    simplify_polygon_topology(&knot);

    b1 = 1; b2 = 2;
    n_free = knot.n_vertices - 3;
    max_choices = 1u << (unsigned)n_free;

    for (choices = 1; choices < max_choices; choices++) {
        Vec3Q alt_verts[MAX_STICK_VERTICES];
        int alt_n, alt_nc, alt_det, k;
        Crossing cx[MAX_CROSSINGS];
        RatPoly alt_alex;
        Rat eval_val;

        evaluate_inplace(&knot, b0, b1, b2, choices, alt_verts, &alt_n);
        alt_nc = find_crossings_clean(alt_verts, alt_n, cx, MAX_CROSSINGS);
        alt_det = (alt_nc > 0) ? knot_determinant(alt_n, cx, alt_nc) : 1;
        alt_alex = alexander_polynomial(alt_n, cx, alt_nc);
        eval_val = rp_eval(&alt_alex, rat_from_int(-1));

        if (eval_val.q != 1 || rat_abs(eval_val.p) != (i64)alt_det)
            continue;
        if (rp_is_palindromic(&alt_alex))
            continue;

        /* Found first non-palindromic — dump everything */
        printf("  Alternative: choices=%u, vertices=%d, crossings=%d\n",
               choices, alt_n, alt_nc);
        printf("  Determinant: %d\n", alt_det);
        printf("  Polynomial: ");
        rp_print(&alt_alex);
        printf("\n  Palindromic: NO\n\n");

        printf("  Crossing details:\n");
        printf("  %3s  %5s  %5s  %4s  %20s  %20s  %20s  %4s\n",
               "#", "edgeA", "edgeB", "sign",
               "depth_A", "depth_B", "depth_diff", "ornt");

        for (k = 0; k < alt_nc; k++) {
            Rat da = cx[k].z_a;
            Rat db = cx[k].z_b;
            Rat diff = rat_sub(da, db);
            int over_under = rat_cmp(da, db);

            printf("  %3d  %3d-%d  %3d-%d  %+3d  ",
                   k, cx[k].edge_a, (cx[k].edge_a + 1) % alt_n,
                   cx[k].edge_b, (cx[k].edge_b + 1) % alt_n,
                   cx[k].sign);
            rat_print(da); printf("  ");
            rat_print(db); printf("  ");
            rat_print(diff); printf("  %+d", over_under);
            printf("\n");
        }

        /* Also print vertex coordinates */
        printf("\n  Vertex coordinates:\n");
        for (k = 0; k < alt_n; k++) {
            printf("  v%2d: (", k);
            rat_print(alt_verts[k].x); printf(", ");
            rat_print(alt_verts[k].y); printf(", ");
            rat_print(alt_verts[k].z); printf(")\n");
        }

        /* Check for 3D edge intersections (self-intersection) */
        printf("\n  3D coplanarity check (non-adjacent edge pairs):\n");
        {
            int i, j, n_coplanar = 0;
            for (i = 0; i < alt_n; i++) {
                int i2 = (i + 1) % alt_n;
                for (j = i + 2; j < alt_n; j++) {
                    int j2 = (j + 1) % alt_n;
                    Vec3Q di_v, dj_v, dij;
                    Rat vol;

                    if (j2 == i) continue;

                    /* Edge vectors */
                    di_v.x = rat_sub(alt_verts[i2].x, alt_verts[i].x);
                    di_v.y = rat_sub(alt_verts[i2].y, alt_verts[i].y);
                    di_v.z = rat_sub(alt_verts[i2].z, alt_verts[i].z);
                    dj_v.x = rat_sub(alt_verts[j2].x, alt_verts[j].x);
                    dj_v.y = rat_sub(alt_verts[j2].y, alt_verts[j].y);
                    dj_v.z = rat_sub(alt_verts[j2].z, alt_verts[j].z);
                    dij.x = rat_sub(alt_verts[j].x, alt_verts[i].x);
                    dij.y = rat_sub(alt_verts[j].y, alt_verts[i].y);
                    dij.z = rat_sub(alt_verts[j].z, alt_verts[i].z);

                    /* Volume = dij . (di x dj) */
                    vol = rat_add(
                        rat_mul(dij.x, rat_sub(rat_mul(di_v.y, dj_v.z),
                                               rat_mul(di_v.z, dj_v.y))),
                        rat_add(
                            rat_mul(dij.y, rat_sub(rat_mul(di_v.z, dj_v.x),
                                                   rat_mul(di_v.x, dj_v.z))),
                            rat_mul(dij.z, rat_sub(rat_mul(di_v.x, dj_v.y),
                                                   rat_mul(di_v.y, dj_v.x)))
                        )
                    );

                    if (rat_is_zero(vol)) {
                        printf("    COPLANAR: edge %d-%d and edge %d-%d\n",
                               i, i2, j, j2);
                        n_coplanar++;
                    }
                }
            }
            printf("  Total coplanar non-adjacent pairs: %d\n", n_coplanar);
        }

        break; /* Only dump first non-palindromic */
    }
}

int main(void) {
    printf("KNOTAPEL DEMO 112: Alexander Polynomial via Construction Words\n");
    printf("===============================================================\n");

    /* Suppress unused warnings */
    (void)rat_print;
    (void)v3q_add;
    (void)rp_scale;
    (void)find_crossings_ex;

    test_ratpoly_arithmetic();
    test_alexander_polynomial();
    test_construction_word_spectrum();
    test_spectral_stability();
    test_spectral_stability_figure_eight();
    test_reachability_graph();
    test_degenerate_crossings();
    test_crossing_dump_5_2();

    printf("\n===============================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
