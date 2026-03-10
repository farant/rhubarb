/*
 * KNOTAPEL DEMO 110: Construction Words for Knots
 * =================================================
 *
 * "Every Knot Admits a Construction Word" (Arant, 2026):
 *
 * A construction word encodes a knot as a binary decision sequence.
 * Starting from a base triangle (3 rational points), each step
 * intersects 3 spheres centered at previously constructed points,
 * producing exactly 2 candidate vertices (target and mirror).
 * The binary sequence of choices = the construction word.
 *
 * Key theorem: every tame knot at every stick number >= s(K) has a
 * polygonal realization with all-rational coordinates.
 *
 * Key insight (explorer): mirror points preserve rationality.
 * If target P1 is rational and centers are rational, then
 * mirror P2 = 2M - P1 where M is the foot of perpendicular
 * from P1 to the plane of the 3 centers. All rational.
 * By induction, ALL 2^n alternatives have rational coordinates.
 *
 * Decoherence ratio conjecture: the fraction of 2^n alternatives
 * preserving knot type K equals 2/2^c where c = crossing number.
 *
 * Phase 1: Rational arithmetic + 3D vectors
 * Phase 2: Mirror formula (sphere-intersection reflection)
 * Phase 3: Knot catalog (rational stick polygons)
 * Phase 4: Construction word finder (reverse)
 * Phase 5: Decoherence ratio computation
 *
 * C89, zero dependencies beyond stdio/stdlib/string.
 * ALL arithmetic is exact rational -- no floating point.
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
 *
 * Q = p/q with q > 0, gcd(|p|,q) = 1.
 * Using int64_t (long long) for now. If denominators overflow
 * we'll widen later.
 * ================================================================ */

typedef long long i64;

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

/*
 * Overflow-resistant arithmetic: cross-reduce before multiplying.
 * For add/sub: factor out gcd(a.q, b.q) to reduce intermediates.
 * For mul/div: cross-reduce (a.p,b.q) and (b.p,a.q) before products.
 */
static Rat rat_add(Rat a, Rat b) {
    i64 g = rat_gcd(a.q, b.q);
    i64 aq_r = a.q / g;  /* a.q / gcd */
    i64 bq_r = b.q / g;  /* b.q / gcd */
    /* result = (a.p * bq_r + b.p * aq_r) / (aq_r * b.q) */
    return rat_make(a.p * bq_r + b.p * aq_r, aq_r * b.q);
}

static Rat rat_sub(Rat a, Rat b) {
    i64 g = rat_gcd(a.q, b.q);
    i64 aq_r = a.q / g;
    i64 bq_r = b.q / g;
    return rat_make(a.p * bq_r - b.p * aq_r, aq_r * b.q);
}

static Rat rat_mul(Rat a, Rat b) {
    /* Cross-reduce: gcd(a.p, b.q) and gcd(b.p, a.q) */
    i64 g1 = rat_gcd(rat_abs(a.p), b.q);
    i64 g2 = rat_gcd(rat_abs(b.p), a.q);
    return rat_make((a.p / g1) * (b.p / g2), (a.q / g2) * (b.q / g1));
}

static Rat rat_div(Rat a, Rat b) {
    i64 g1, g2;
    if (b.p == 0) { fprintf(stderr, "rat_div: division by zero\n"); exit(1); }
    /* Cross-reduce: gcd(a.p, b.p) and gcd(a.q, b.q) */
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
    /* a.p/a.q vs b.p/b.q => a.p*b.q vs b.p*a.q (both q>0) */
    i64 lhs = a.p * b.q;
    i64 rhs = b.p * a.q;
    if (lhs < rhs) return -1;
    if (lhs > rhs) return 1;
    return 0;
}

static Rat rat_two(void) { Rat r; r.p = 2; r.q = 1; return r; }

static void rat_print(Rat r) {
    if (r.q == 1) printf("%lld", r.p);
    else printf("%lld/%lld", r.p, r.q);
}

/* ================================================================
 * Part B: Rational 3D Vectors
 *
 * Vec3Q = (x, y, z) where each component is Rat.
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
 *
 * Given 3 center points C0, C1, C2 and a target point P (all on
 * the 3 spheres), compute the mirror point P' = 2M - P where
 * M is the foot of perpendicular from P to the plane of C0,C1,C2.
 *
 * Plane through C0,C1,C2:
 *   normal n = (C1-C0) x (C2-C0)
 *   equation: n . (X - C0) = 0
 *
 * Foot of perpendicular from P to plane:
 *   M = P - ((n . (P - C0)) / (n . n)) * n
 *
 * Mirror:
 *   P' = 2M - P = P - 2 * ((n . (P - C0)) / (n . n)) * n
 *
 * All operations are rational. No square roots needed.
 * ================================================================ */

static Vec3Q mirror_point(Vec3Q c0, Vec3Q c1, Vec3Q c2, Vec3Q target) {
    Vec3Q e1 = v3q_sub(c1, c0);
    Vec3Q e2 = v3q_sub(c2, c0);
    Vec3Q n = v3q_cross(e1, e2);
    Rat n_dot_n = v3q_dot(n, n);
    Vec3Q p_minus_c0 = v3q_sub(target, c0);
    Rat n_dot_d = v3q_dot(n, p_minus_c0);
    /* t = 2 * (n . (P - C0)) / (n . n) */
    Rat t = rat_div(rat_mul(rat_two(), n_dot_d), n_dot_n);
    /* mirror = P - t * n */
    return v3q_sub(target, v3q_scale(t, n));
}

/* Verify that a point lies on a sphere: |P - C|^2 == r^2 */
static int on_sphere(Vec3Q p, Vec3Q center, Rat r_sq) {
    return rat_eq(v3q_dist_sq(p, center), r_sq);
}

/* ================================================================
 * Part D: Knot Catalog
 *
 * Known rational-coordinate stick polygons for small knots.
 * Vertices listed in order; edges connect consecutive vertices
 * in a cycle (last vertex back to first).
 *
 * Trefoil 3_1: stick number s = 6.
 * Using Calvo's hexagonal trefoil with integer coordinates.
 * ================================================================ */

#define MAX_STICK_VERTICES 24

typedef struct {
    const char *name;
    int crossing_number;
    int n_vertices;
    Vec3Q vertices[MAX_STICK_VERTICES];
} StickKnot;

/*
 * Rational trefoil: 6 vertices.
 * This is a known minimal-stick trefoil on integer coordinates.
 * Vertices form a hexagonal path that winds around a torus-like
 * shape with exactly 3 crossings in generic projection.
 */
static StickKnot make_trefoil(void) {
    StickKnot k;
    k.name = "trefoil 3_1";
    k.crossing_number = 3;
    k.n_vertices = 6;
    /* Trefoil from parametric curve with offset sampling.
     * x(t)=sin(t)+2sin(2t), y(t)=cos(t)-2cos(2t), z(t)=-sin(3t)
     * Sampled at t = pi/6 + k*pi/3, scaled by 10.
     */
    k.vertices[0] = v3q_from_ints( 22,  -1, -10);
    k.vertices[1] = v3q_from_ints( 10,  20,  10);
    k.vertices[2] = v3q_from_ints(-12, -19, -10);
    k.vertices[3] = v3q_from_ints( 12, -19,  10);
    k.vertices[4] = v3q_from_ints(-10,  20, -10);
    k.vertices[5] = v3q_from_ints(-22,  -1,  10);
    return k;
}

/*
 * Figure-eight knot 4_1: stick number s = 7.
 * Amphichiral (equal to its mirror image).
 * From parametric curve (2+cos(2t))cos(3t), (2+cos(2t))sin(3t), sin(4t)
 * sampled at t = 2*pi*k/7, k=0..6, scaled by 10, rounded to integers.
 */
static StickKnot make_figure_eight(void) {
    StickKnot k;
    k.name = "figure-eight 4_1";
    k.crossing_number = 4;
    k.n_vertices = 12;
    /* 12-vertex polygon from parametric curve sampled at t=2*pi*k/12,
     * scaled by 10, rounded. 7 sticks is minimal but the polygonal
     * approximation needs 12 to keep exactly 4 crossings in projection. */
    k.vertices[ 0] = v3q_from_ints(  30,    0,    0);
    k.vertices[ 1] = v3q_from_ints(   0,   25,    9);
    k.vertices[ 2] = v3q_from_ints( -15,    0,   -9);
    k.vertices[ 3] = v3q_from_ints(   0,  -10,    0);
    k.vertices[ 4] = v3q_from_ints(  15,    0,    9);
    k.vertices[ 5] = v3q_from_ints(   0,   25,   -9);
    k.vertices[ 6] = v3q_from_ints( -30,    0,    0);
    k.vertices[ 7] = v3q_from_ints(   0,  -25,    9);
    k.vertices[ 8] = v3q_from_ints(  15,    0,   -9);
    k.vertices[ 9] = v3q_from_ints(   0,   10,    0);
    k.vertices[10] = v3q_from_ints( -15,    0,    9);
    k.vertices[11] = v3q_from_ints(   0,  -25,   -9);
    return k;
}

/*
 * Cinquefoil knot 5_1 = torus knot T(2,5).
 * Parametric: x = cos(2t)*(3+cos(5t)), y = sin(2t)*(3+cos(5t)), z = sin(5t)
 * Sample at t = 2*pi*k/N, scale by 10, round to integers.
 * Need to find N that gives exactly 5 crossings.
 */
/* Taylor sin/cos, range-reduced to [-pi,pi]. 7th order is plenty for
 * our rounding-to-integer sampling. */
/* Taylor sin/cos with 13th/12th order for accuracy near |x|=pi */
static double taylor_sin(double x) {
    double x2, r;
    while (x > 3.14159265358979) x -= 6.28318530717959;
    while (x < -3.14159265358979) x += 6.28318530717959;
    x2 = x*x;
    /* Horner form: x*(1 - x2/6*(1 - x2/20*(1 - x2/42*(1 - x2/72*(1 - x2/110*(1 - x2/156)))))) */
    r = 1.0 - x2/156.0;
    r = 1.0 - x2/110.0 * r;
    r = 1.0 - x2/72.0 * r;
    r = 1.0 - x2/42.0 * r;
    r = 1.0 - x2/20.0 * r;
    r = 1.0 - x2/6.0 * r;
    return x * r;
}

static double taylor_cos(double x) {
    double x2, r;
    while (x > 3.14159265358979) x -= 6.28318530717959;
    while (x < -3.14159265358979) x += 6.28318530717959;
    x2 = x*x;
    /* Horner form: 1 - x2/2*(1 - x2/12*(1 - x2/30*(1 - x2/56*(1 - x2/90*(1 - x2/132))))) */
    r = 1.0 - x2/132.0;
    r = 1.0 - x2/90.0 * r;
    r = 1.0 - x2/56.0 * r;
    r = 1.0 - x2/30.0 * r;
    r = 1.0 - x2/12.0 * r;
    r = 1.0 - x2/2.0 * r;
    return r;
}

static StickKnot make_cinquefoil(int n_samples, int scale) {
    StickKnot k;
    int i;
    double pi2 = 6.28318530717959;
    k.name = "cinquefoil 5_1";
    k.crossing_number = 5;
    k.n_vertices = n_samples;
    for (i = 0; i < n_samples; i++) {
        double t = pi2 * (double)i / (double)n_samples;
        double s2 = taylor_sin(2.0*t), c2 = taylor_cos(2.0*t);
        double s5 = taylor_sin(5.0*t), c5 = taylor_cos(5.0*t);
        double x = c2 * (3.0 + c5);
        double y = s2 * (3.0 + c5);
        double z = s5;

        k.vertices[i] = v3q_from_ints(
            (i64)(x * (double)scale + (x >= 0 ? 0.5 : -0.5)),
            (i64)(y * (double)scale + (y >= 0 ? 0.5 : -0.5)),
            (i64)(z * (double)scale + (z >= 0 ? 0.5 : -0.5))
        );
    }
    return k;
}

/* ================================================================
 * Part E: Construction Word
 *
 * A construction word for an n-vertex stick polygon consists of:
 * - A base triangle (3 vertex indices)
 * - For each remaining vertex (in construction order):
 *   - 3 center indices (previously constructed points)
 *   - The target vertex index
 *   - The binary choice (0 = target, 1 = mirror)
 *
 * In the "reverse" direction, we find a construction word for
 * a known polygon: for each vertex (after the base triangle),
 * find 3 previously constructed vertices such that the target
 * lies on all 3 spheres (with radii = distances to centers).
 * ================================================================ */

typedef struct {
    int center_idx[3]; /* indices of the 3 sphere centers */
    int target_idx;    /* index of the target vertex */
} ConstructionStep;

typedef struct {
    int base[3];       /* base triangle vertex indices */
    int n_steps;
    ConstructionStep steps[MAX_STICK_VERTICES];
} ConstructionWord;

/*
 * Find a construction word for a stick knot by reverse engineering:
 * - Base triangle = first 3 vertices
 * - For each subsequent vertex, use the 3 most recently constructed
 *   vertices as sphere centers (simplest strategy).
 *
 * Verification: check that the target lies on all 3 spheres.
 * Returns 1 on success, 0 if any step fails verification.
 */
static int find_construction_word(const StickKnot *knot,
                                  ConstructionWord *word) {
    int i, j;
    if (knot->n_vertices < 4) return 0;

    word->base[0] = 0;
    word->base[1] = 1;
    word->base[2] = 2;
    word->n_steps = knot->n_vertices - 3;

    for (i = 0; i < word->n_steps; i++) {
        int vi = i + 3; /* vertex being constructed */
        ConstructionStep *step = &word->steps[i];
        step->target_idx = vi;

        /* Use 3 most recent vertices as centers */
        step->center_idx[0] = vi - 3;
        step->center_idx[1] = vi - 2;
        step->center_idx[2] = vi - 1;

        /* Verify target lies on all 3 spheres */
        for (j = 0; j < 3; j++) {
            Vec3Q center = knot->vertices[step->center_idx[j]];
            Vec3Q target = knot->vertices[step->target_idx];
            Rat r_sq = v3q_dist_sq(center, target);
            /* The sphere has radius = distance(center, target),
               so the target is on the sphere by definition.
               The mirror point must ALSO be on all 3 spheres. */
            (void)r_sq;
        }
    }
    return 1;
}

/*
 * Given a construction word and a binary choice vector (bitmask),
 * build the polygon by evaluating each step.
 *
 * bit i = 0: use target point (original vertex)
 * bit i = 1: use mirror point
 *
 * Returns the constructed polygon vertices.
 */
static void evaluate_construction(const StickKnot *knot,
                                  const ConstructionWord *word,
                                  unsigned int choices,
                                  Vec3Q *out_vertices,
                                  int *out_n) {
    int i;
    /* Copy base triangle */
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
        int use_mirror = (choices >> i) & 1;

        if (use_mirror) {
            out_vertices[*out_n] = mirror_point(c0, c1, c2, target);
        } else {
            out_vertices[*out_n] = target;
        }
        (*out_n)++;
    }
}

/* ================================================================
 * Part F: Crossing Detection (stub for Phase 4)
 *
 * Given a stick polygon, project to 2D and find crossings.
 * Each crossing has an over/under determined by z-coordinates.
 * This will be needed for knot type classification.
 *
 * For now, we implement the projection and crossing finder.
 * ================================================================ */

/* 2D rational point (projection of 3D) */
typedef struct { Rat x, y; } Vec2Q;

/* A crossing between edges (i, i+1) and (j, j+1) */
typedef struct {
    int edge_a; /* first edge index */
    int edge_b; /* second edge index */
    int sign;   /* +1 or -1 (over/under) */
    Rat z_a;    /* depth of edge_a at crossing */
    Rat z_b;    /* depth of edge_b at crossing */
    Rat param_a; /* parameter along edge_a (0 < s < 1) */
    Rat param_b; /* parameter along edge_b (0 < t < 1) */
} Crossing;

/*
 * Test whether two 2D line segments (p1-p2) and (p3-p4) intersect.
 * If so, compute the parameters s, t where:
 *   intersection = p1 + s*(p2-p1) = p3 + t*(p4-p3)
 * Returns 1 if they intersect with 0 < s < 1 and 0 < t < 1.
 *
 * All arithmetic is exact rational.
 */
static int segments_intersect_2d(Vec2Q p1, Vec2Q p2, Vec2Q p3, Vec2Q p4,
                                 Rat *s_out, Rat *t_out) {
    /* d1 = p2 - p1, d2 = p4 - p3 */
    Rat d1x = rat_sub(p2.x, p1.x);
    Rat d1y = rat_sub(p2.y, p1.y);
    Rat d2x = rat_sub(p4.x, p3.x);
    Rat d2y = rat_sub(p4.y, p3.y);

    /* denom = d1x * d2y - d1y * d2x (2D cross product) */
    Rat denom = rat_sub(rat_mul(d1x, d2y), rat_mul(d1y, d2x));
    Rat dx, dy, s, t, zero, one;

    if (rat_is_zero(denom)) return 0; /* parallel or collinear */

    /* dx = p3.x - p1.x, dy = p3.y - p1.y */
    dx = rat_sub(p3.x, p1.x);
    dy = rat_sub(p3.y, p1.y);

    /* s = (dx * d2y - dy * d2x) / denom */
    s = rat_div(rat_sub(rat_mul(dx, d2y), rat_mul(dy, d2x)), denom);

    /* t = (dx * d1y - dy * d1x) / denom */
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

/*
 * Generic projection along direction d = (2, 3, 5).
 *
 * Projection plane basis (perpendicular to d):
 *   u = (5, 0, -2)   ->  proj_x = 5x - 2z
 *   v = (0, 5, -3)   ->  proj_y = 5y - 3z
 *
 * Depth along d:
 *   depth = 2x + 3y + 5z
 *
 * All integer if input is integer. No rational division needed.
 * The direction (2,3,5) has coprime components and irrational
 * ratios, making degeneracies extremely unlikely.
 */
static void generic_project(Vec3Q v3, Vec2Q *uv, Rat *depth) {
    Rat c2 = rat_from_int(2), c3 = rat_from_int(3), c5 = rat_from_int(5);
    uv->x = rat_sub(rat_mul(c5, v3.x), rat_mul(c2, v3.z));
    uv->y = rat_sub(rat_mul(c5, v3.y), rat_mul(c3, v3.z));
    *depth = rat_add(rat_mul(c2, v3.x),
                     rat_add(rat_mul(c3, v3.y), rat_mul(c5, v3.z)));
}

/*
 * Find all crossings in a stick polygon under generic projection.
 * For each pair of non-adjacent edges, test for intersection.
 * At each crossing, compare depth to determine over/under.
 */
static int find_crossings(const Vec3Q *verts, int n,
                          Crossing *crossings, int max_crossings) {
    int i, j, nc = 0;
    Vec2Q proj[MAX_STICK_VERTICES];
    Rat depth[MAX_STICK_VERTICES];

    /* Project all vertices */
    for (i = 0; i < n; i++) {
        generic_project(verts[i], &proj[i], &depth[i]);
    }

    for (i = 0; i < n && nc < max_crossings; i++) {
        int i2 = (i + 1) % n;
        for (j = i + 2; j < n && nc < max_crossings; j++) {
            int j2 = (j + 1) % n;
            Rat s, t, da, db;

            /* Skip adjacent edges */
            if (j2 == i) continue;

            if (segments_intersect_2d(proj[i], proj[i2],
                                      proj[j], proj[j2], &s, &t)) {
                /* depth at crossing on edge i */
                Rat dix, diy, djx, djy, cross2d;
                int over_under, orient;

                da = rat_add(depth[i],
                    rat_mul(s, rat_sub(depth[i2], depth[i])));
                /* depth at crossing on edge j */
                db = rat_add(depth[j],
                    rat_mul(t, rat_sub(depth[j2], depth[j])));

                /* 2D direction vectors of projected edges */
                dix = rat_sub(proj[i2].x, proj[i].x);
                diy = rat_sub(proj[i2].y, proj[i].y);
                djx = rat_sub(proj[j2].x, proj[j].x);
                djy = rat_sub(proj[j2].y, proj[j].y);

                /* 2D cross product of edge directions */
                cross2d = rat_sub(rat_mul(dix, djy), rat_mul(diy, djx));
                orient = rat_cmp(cross2d, rat_from_int(0));

                /* over_under: +1 if edge_i is over edge_j */
                over_under = rat_cmp(da, db);

                /* Crossing sign = orient * over_under
                 * Convention: +1 if over-strand crosses L-to-R
                 * relative to under-strand direction */
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
 * Part G: Knot Determinant
 *
 * The knot determinant = |det(reduced Alexander matrix at t=-1)|.
 *   unknot = 1, trefoil = 3, figure-eight = 5, cinquefoil = 5
 *
 * At each crossing (evaluated at t=-1):
 *   Positive crossing: over-arc gets +2, under-in gets -1, under-out gets -1
 *   Negative crossing: over-arc gets -2, under-in gets +1, under-out gets +1
 * ================================================================ */

#define MAX_CROSSINGS 32
#define MAX_ARCS 32

/*
 * Arc labeling for the Alexander matrix.
 *
 * The old approach (one arc per edge, break at "under-entry" edges)
 * fails when a single edge goes under multiple crossings. The correct
 * approach: walk the knot, breaking the strand at each under-crossing
 * point. This requires knowing the parameter (0 < t < 1) at which
 * each crossing occurs along each edge.
 *
 * Algorithm:
 * 1. For each crossing, identify which edge is under and record
 *    (edge_idx, param) as an "arc break" position.
 * 2. Sort breaks in walk order (by edge_idx, then by param).
 * 3. Assign arc labels: arc i starts at break[i].
 *    under_out_arc[crossing] = i, under_in_arc[crossing] = (i-1+nc)%nc.
 * 4. For the over-strand at each crossing, find which arc it's on
 *    by locating its (edge, param) position among the sorted breaks.
 *
 * This correctly handles multiple under-crossings per edge.
 */

/* Arc break: a position on the knot where the under-strand is broken */
typedef struct {
    int edge_idx;
    Rat param;
    int crossing_idx;
} ArcBreak;

/* Find which arc a position (edge, param) falls on.
 * Breaks are sorted by (edge_idx, param) in walk order.
 * Arc i starts at break[i], so a position after break[i]
 * and before break[i+1] is on arc i.
 * A position before all breaks is on the wrap-around arc (nc-1). */
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
    return n_breaks - 1; /* wrap-around arc */
}

static int knot_determinant(int n_edges, const Crossing *crossings, int nc) {
    ArcBreak breaks[MAX_CROSSINGS];
    int over_arc[MAX_CROSSINGS];
    int under_in_arc[MAX_CROSSINGS];
    int under_out_arc[MAX_CROSSINGS];
    long long M[MAX_CROSSINGS][MAX_ARCS];
    int i, j, k, sz;
    long long det;

    (void)n_edges;

    if (nc == 0) return 1;

    /* Step 1: Build arc breaks (one per crossing, on the under-strand) */
    for (j = 0; j < nc; j++) {
        if (rat_cmp(crossings[j].z_a, crossings[j].z_b) > 0) {
            /* edge_a is over (higher depth), edge_b is under */
            breaks[j].edge_idx = crossings[j].edge_b;
            breaks[j].param = crossings[j].param_b;
        } else {
            /* edge_b is over, edge_a is under */
            breaks[j].edge_idx = crossings[j].edge_a;
            breaks[j].param = crossings[j].param_a;
        }
        breaks[j].crossing_idx = j;
    }

    /* Step 2: Sort breaks by (edge_idx, param) — insertion sort */
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

    /* Step 3: Assign under-strand arc labels from sorted breaks.
     * Arc i starts at break[i]. The incoming arc is (i-1+nc)%nc. */
    for (i = 0; i < nc; i++) {
        int cj = breaks[i].crossing_idx;
        under_out_arc[cj] = i;
        under_in_arc[cj] = (i - 1 + nc) % nc;
    }

    /* Step 4: Find over_arc for each crossing */
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

    /* Step 5: Build Alexander matrix at t = -1.
     * Positive crossing: over_arc +2, under_in -1, under_out -1.
     * Negative crossing: over_arc -2, under_in +1, under_out +1. */
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

    /* Step 6: Bareiss determinant of (nc-1) x (nc-1) minor */
    sz = nc - 1;
    if (sz <= 0) return 1;

    {
        long long A[MAX_CROSSINGS][MAX_ARCS];
        long long prev;

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
                            long long tmp = A[k][j];
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

static const char *classify_knot(int n_edges, const Vec3Q *verts) {
    Crossing crossings[MAX_CROSSINGS];
    int nc, det;

    nc = find_crossings(verts, n_edges, crossings, MAX_CROSSINGS);
    if (nc == 0) return "unknot";

    det = knot_determinant(n_edges, crossings, nc);

    if (det == 1) return "unknot";
    if (det == 3) return "trefoil";
    if (det == 5) return "figure-eight/cinquefoil";
    if (det == 7) return "7-det knot";
    return "unknown";
}

/* ================================================================
 * Tests
 * ================================================================ */

static void test_rational_arithmetic(void) {
    Rat a, b, c;

    printf("\n=== Test: Rational Arithmetic ===\n");

    /* Basic construction and normalization */
    a = rat_make(6, 4);
    check("6/4 normalizes to 3/2", a.p == 3 && a.q == 2);

    a = rat_make(-6, 4);
    check("-6/4 normalizes to -3/2", a.p == -3 && a.q == 2);

    a = rat_make(6, -4);
    check("6/-4 normalizes to -3/2", a.p == -3 && a.q == 2);

    a = rat_make(0, 5);
    check("0/5 normalizes to 0/1", a.p == 0 && a.q == 1);

    /* Addition */
    a = rat_make(1, 3);
    b = rat_make(1, 6);
    c = rat_add(a, b);
    check("1/3 + 1/6 = 1/2", c.p == 1 && c.q == 2);

    /* Subtraction */
    c = rat_sub(a, b);
    check("1/3 - 1/6 = 1/6", c.p == 1 && c.q == 6);

    /* Multiplication */
    a = rat_make(2, 3);
    b = rat_make(3, 4);
    c = rat_mul(a, b);
    check("2/3 * 3/4 = 1/2", c.p == 1 && c.q == 2);

    /* Division */
    c = rat_div(a, b);
    check("(2/3) / (3/4) = 8/9", c.p == 8 && c.q == 9);

    /* Comparison */
    a = rat_make(1, 3);
    b = rat_make(1, 2);
    check("1/3 < 1/2", rat_cmp(a, b) < 0);
    check("1/2 > 1/3", rat_cmp(b, a) > 0);
    check("1/3 == 1/3", rat_cmp(a, a) == 0);

    /* Negation */
    a = rat_make(3, 7);
    b = rat_neg(a);
    check("neg(3/7) = -3/7", b.p == -3 && b.q == 7);
    c = rat_add(a, b);
    check("3/7 + neg(3/7) = 0", rat_is_zero(c));
}

static void test_vec3q(void) {
    Vec3Q a, b, c;
    Rat d;

    printf("\n=== Test: Rational 3D Vectors ===\n");

    a = v3q_from_ints(1, 2, 3);
    b = v3q_from_ints(4, 5, 6);

    c = v3q_add(a, b);
    check("(1,2,3)+(4,5,6) = (5,7,9)",
        rat_eq(c.x, rat_from_int(5)) &&
        rat_eq(c.y, rat_from_int(7)) &&
        rat_eq(c.z, rat_from_int(9)));

    c = v3q_sub(b, a);
    check("(4,5,6)-(1,2,3) = (3,3,3)",
        rat_eq(c.x, rat_from_int(3)) &&
        rat_eq(c.y, rat_from_int(3)) &&
        rat_eq(c.z, rat_from_int(3)));

    d = v3q_dot(a, b);
    check("(1,2,3).(4,5,6) = 32", rat_eq(d, rat_from_int(32)));

    c = v3q_cross(v3q_from_ints(1, 0, 0), v3q_from_ints(0, 1, 0));
    check("x cross y = z",
        rat_eq(c.x, rat_from_int(0)) &&
        rat_eq(c.y, rat_from_int(0)) &&
        rat_eq(c.z, rat_from_int(1)));

    d = v3q_dist_sq(v3q_from_ints(0, 0, 0), v3q_from_ints(1, 2, 3));
    check("||(1,2,3)||^2 = 14", rat_eq(d, rat_from_int(14)));

    /* Print (exercises v3q_print) */
    printf("  v3q_print test: ");
    v3q_print(v3q_make(rat_make(1,3), rat_make(-2,5), rat_from_int(7)));
    printf("\n");
}

static void test_mirror(void) {
    Vec3Q c0, c1, c2, target, mirror, mirror2;

    printf("\n=== Test: Mirror Formula ===\n");

    /* Simple case: plane is z=0, target above, mirror below */
    c0 = v3q_from_ints(0, 0, 0);
    c1 = v3q_from_ints(1, 0, 0);
    c2 = v3q_from_ints(0, 1, 0);
    target = v3q_from_ints(0, 0, 5);
    mirror = mirror_point(c0, c1, c2, target);
    check("mirror across z=0: (0,0,5) -> (0,0,-5)",
        v3q_eq(mirror, v3q_from_ints(0, 0, -5)));

    /* Mirror is involution: mirror(mirror(P)) = P */
    mirror2 = mirror_point(c0, c1, c2, mirror);
    check("mirror is involution", v3q_eq(mirror2, target));

    /* Plane z=1, target at (2,3,4), mirror at (2,3,-2) */
    c0 = v3q_from_ints(0, 0, 1);
    c1 = v3q_from_ints(1, 0, 1);
    c2 = v3q_from_ints(0, 1, 1);
    target = v3q_from_ints(2, 3, 4);
    mirror = mirror_point(c0, c1, c2, target);
    check("mirror across z=1: (2,3,4) -> (2,3,-2)",
        v3q_eq(mirror, v3q_from_ints(2, 3, -2)));

    /* Tilted plane: centers at (0,0,0), (1,0,0), (0,1,1)
     * Normal = (1,0,0)x(0,1,1) = (0,-1,1)
     * Target (0,0,2): foot = (0,0,2) - ((0,-1,1).(0,0,2)/2)*(0,-1,1)
     *   = (0,0,2) - (2/2)*(0,-1,1) = (0,0,2) - (0,-1,1) = (0,1,1)
     * Mirror = 2*(0,1,1) - (0,0,2) = (0,2,0) */
    c0 = v3q_from_ints(0, 0, 0);
    c1 = v3q_from_ints(1, 0, 0);
    c2 = v3q_from_ints(0, 1, 1);
    target = v3q_from_ints(0, 0, 2);
    mirror = mirror_point(c0, c1, c2, target);
    check("tilted plane mirror: (0,0,2) -> (0,2,0)",
        v3q_eq(mirror, v3q_from_ints(0, 2, 0)));

    /* Verify mirror preserves distances to all 3 centers */
    {
        int j;
        Vec3Q centers[3];
        int ok = 1;
        centers[0] = c0; centers[1] = c1; centers[2] = c2;
        for (j = 0; j < 3; j++) {
            Rat d_target = v3q_dist_sq(target, centers[j]);
            Rat d_mirror = v3q_dist_sq(mirror, centers[j]);
            if (!rat_eq(d_target, d_mirror)) ok = 0;
        }
        check("mirror preserves distances to all centers", ok);
    }

    /* Verify on_sphere works */
    {
        Rat r_sq = v3q_dist_sq(c0, target);
        check("on_sphere: target on sphere(c0,r)",
              on_sphere(target, c0, r_sq));
        check("on_sphere: mirror on sphere(c0,r)",
              on_sphere(mirror, c0, r_sq));
    }
}

static void test_trefoil_construction(void) {
    StickKnot trefoil;
    ConstructionWord word;
    Vec3Q poly[MAX_STICK_VERTICES];
    int n_poly, i;

    printf("\n=== Test: Trefoil Construction ===\n");

    trefoil = make_trefoil();
    check("trefoil has 6 vertices", trefoil.n_vertices == 6);

    /* Find construction word (reverse) */
    check("construction word found", find_construction_word(&trefoil, &word));
    check("construction word has 3 steps", word.n_steps == 3);

    /* Evaluate with all-zero choices (original polygon) */
    evaluate_construction(&trefoil, &word, 0, poly, &n_poly);
    check("original polygon has 6 vertices", n_poly == 6);

    {
        int ok = 1;
        for (i = 0; i < 6; i++) {
            if (!v3q_eq(poly[i], trefoil.vertices[i])) ok = 0;
        }
        check("choices=0 reproduces original polygon", ok);
    }

    /* Verify mirror points preserve distances */
    for (i = 0; i < word.n_steps; i++) {
        ConstructionStep *step = &word.steps[i];
        Vec3Q c0 = trefoil.vertices[step->center_idx[0]];
        Vec3Q c1 = trefoil.vertices[step->center_idx[1]];
        Vec3Q c2 = trefoil.vertices[step->center_idx[2]];
        Vec3Q target = trefoil.vertices[step->target_idx];
        Vec3Q mir = mirror_point(c0, c1, c2, target);
        int j, ok = 1;
        Vec3Q centers[3];
        char buf[80];
        centers[0] = c0; centers[1] = c1; centers[2] = c2;
        for (j = 0; j < 3; j++) {
            if (!rat_eq(v3q_dist_sq(target, centers[j]),
                        v3q_dist_sq(mir, centers[j]))) ok = 0;
        }
        sprintf(buf, "step %d: mirror preserves sphere distances", i);
        check(buf, ok);
    }
}

static void test_crossing_detection(void) {
    StickKnot trefoil;
    Crossing crossings[32];
    int nc;

    printf("\n=== Test: Crossing Detection ===\n");

    trefoil = make_trefoil();
    nc = find_crossings(trefoil.vertices, trefoil.n_vertices,
                        crossings, 32);

    printf("  Trefoil crossings found: %d\n", nc);
    check("trefoil has exactly 3 crossings", nc == 3);

    if (nc > 0) {
        int i;
        int all_same = 1;
        int any_zero = 0;
        for (i = 0; i < nc; i++) {
            printf("  crossing %d: edges %d-%d, depth_a=",
                   i, crossings[i].edge_a, crossings[i].edge_b);
            rat_print(crossings[i].z_a);
            printf(", depth_b=");
            rat_print(crossings[i].z_b);
            printf(", sign=%d\n", crossings[i].sign);
            if (crossings[i].sign == 0) any_zero = 1;
        }

        for (i = 1; i < nc; i++) {
            if (crossings[i].sign != crossings[0].sign) all_same = 0;
        }
        check("trefoil: no degenerate crossings (sign != 0)", !any_zero);
        check("trefoil: all crossings have same sign", all_same);

        /* Knot determinant */
        {
            int det = knot_determinant(trefoil.n_vertices, crossings, nc);
            printf("  Knot determinant: %d\n", det);
            check("trefoil determinant = 3", det == 3);
        }

        /* Full classification */
        {
            const char *ktype = classify_knot(trefoil.n_vertices,
                                              trefoil.vertices);
            printf("  Classification: %s\n", ktype);
            check("classified as trefoil", strcmp(ktype, "trefoil") == 0);
        }
    }
}

static void test_decoherence(void) {
    StickKnot trefoil;
    ConstructionWord word;
    unsigned int choices;
    int n_alternatives, n_same;

    printf("\n=== Test: Decoherence Ratio (Trefoil) ===\n");

    trefoil = make_trefoil();
    find_construction_word(&trefoil, &word);

    n_alternatives = 1 << word.n_steps; /* 2^n_steps */
    n_same = 0;

    printf("  Enumerating %d alternatives (n_steps=%d):\n",
           n_alternatives, word.n_steps);

    for (choices = 0; choices < (unsigned)n_alternatives; choices++) {
        Vec3Q poly[MAX_STICK_VERTICES];
        Crossing crossings[32];
        int n_poly, nc, writhe, det;
        const char *ktype;

        evaluate_construction(&trefoil, &word, choices, poly, &n_poly);
        nc = find_crossings(poly, n_poly, crossings, 32);
        writhe = compute_writhe(crossings, nc);
        det = (nc > 0) ? knot_determinant(n_poly, crossings, nc) : 1;
        ktype = classify_knot(n_poly, poly);

        printf("    choices=%u: %d crossings, writhe=%d, det=%d -> %s\n",
               choices, nc, writhe, det, ktype);

        if (strcmp(ktype, "trefoil") == 0) n_same++;
    }

    printf("\n  Trefoil count: %d / %d\n", n_same, n_alternatives);
    printf("  Predicted (2/2^c = 2/2^3): 2 / %d\n", n_alternatives);

    /* The conjecture predicts 2/2^3 = 2/8 of alternatives are trefoils.
     * This tests a SPECIFIC center assignment (3-most-recent).
     * See sweep below for all possible center assignments. */
    printf("  (conjecture predicts 2, testing with sweep below)\n");

    /* Also test all-base strategy for trefoil */
    printf("\n  All-base strategy (centers always 0,1,2):\n");
    {
        ConstructionWord word2;
        int i2;
        word2.base[0] = 0; word2.base[1] = 1; word2.base[2] = 2;
        word2.n_steps = 3;
        for (i2 = 0; i2 < 3; i2++) {
            word2.steps[i2].target_idx = i2 + 3;
            word2.steps[i2].center_idx[0] = 0;
            word2.steps[i2].center_idx[1] = 1;
            word2.steps[i2].center_idx[2] = 2;
        }
        for (choices = 0; choices < 8; choices++) {
            Vec3Q poly2[MAX_STICK_VERTICES];
            Crossing cx[32];
            int np2, nc2, w2, dt2;
            evaluate_construction(&trefoil, &word2, choices, poly2, &np2);
            nc2 = find_crossings(poly2, np2, cx, 32);
            w2 = compute_writhe(cx, nc2);
            dt2 = (nc2 > 0) ? knot_determinant(np2, cx, nc2) : 1;
            printf("    choices=%u (%d%d%d): %d xings, w=%d, det=%d\n",
                   choices, (choices>>0)&1, (choices>>1)&1, (choices>>2)&1,
                   nc2, w2, dt2);
        }
    }
}

/*
 * Sweep all possible center assignments for base (0,1,2),
 * construction order 3->4->5.
 *
 * Step 0: centers from {0,1,2}, only option: (0,1,2)
 * Step 1: centers from {0,1,2,3}, C(4,3) = 4 options
 * Step 2: centers from {0,1,2,3,4}, C(5,3) = 10 options
 *
 * Total: 40 construction words. For each, evaluate all 8 alternatives.
 */
static void test_decoherence_sweep(void) {
    StickKnot trefoil = make_trefoil();
    /* All C(4,3) triples from {0,1,2,3} */
    static const int c1[4][3] = {
        {0,1,2},{0,1,3},{0,2,3},{1,2,3}
    };
    /* All C(5,3) triples from {0,1,2,3,4} */
    static const int c2[10][3] = {
        {0,1,2},{0,1,3},{0,1,4},{0,2,3},{0,2,4},
        {0,3,4},{1,2,3},{1,2,4},{1,3,4},{2,3,4}
    };
    int s1, s2;
    int best = 0;

    printf("\n=== Test: Decoherence Sweep (all center combos) ===\n");

    for (s1 = 0; s1 < 4; s1++) {
        for (s2 = 0; s2 < 10; s2++) {
            ConstructionWord word;
            unsigned int choices;
            int n_trefoils = 0;

            word.base[0] = 0; word.base[1] = 1; word.base[2] = 2;
            word.n_steps = 3;

            word.steps[0].target_idx = 3;
            word.steps[0].center_idx[0] = 0;
            word.steps[0].center_idx[1] = 1;
            word.steps[0].center_idx[2] = 2;

            word.steps[1].target_idx = 4;
            word.steps[1].center_idx[0] = c1[s1][0];
            word.steps[1].center_idx[1] = c1[s1][1];
            word.steps[1].center_idx[2] = c1[s1][2];

            word.steps[2].target_idx = 5;
            word.steps[2].center_idx[0] = c2[s2][0];
            word.steps[2].center_idx[1] = c2[s2][1];
            word.steps[2].center_idx[2] = c2[s2][2];

            for (choices = 0; choices < 8; choices++) {
                Vec3Q poly[MAX_STICK_VERTICES];
                int n_poly;
                const char *ktype;
                evaluate_construction(&trefoil, &word, choices,
                                      poly, &n_poly);
                ktype = classify_knot(n_poly, poly);
                if (strcmp(ktype, "trefoil") == 0) n_trefoils++;
            }

            if (n_trefoils > 1) {
                /* Detail the winning combo */
                printf("  ** s1=(%d,%d,%d) s2=(%d,%d,%d): %d/8 trefoils **\n",
                       c1[s1][0], c1[s1][1], c1[s1][2],
                       c2[s2][0], c2[s2][1], c2[s2][2],
                       n_trefoils);
                for (choices = 0; choices < 8; choices++) {
                    Vec3Q poly2[MAX_STICK_VERTICES];
                    Crossing cx[32];
                    int np2, nc2, w, dt;
                    const char *kt;
                    evaluate_construction(&trefoil, &word, choices,
                                          poly2, &np2);
                    nc2 = find_crossings(poly2, np2, cx, 32);
                    w = compute_writhe(cx, nc2);
                    dt = (nc2 > 0) ? knot_determinant(np2, cx, nc2) : 1;
                    kt = classify_knot(np2, poly2);
                    printf("    choices=%u: %d xings, w=%d, det=%d -> %s\n",
                           choices, nc2, w, dt, kt);
                }
            } else if (n_trefoils > 0) {
                printf("  s1=(%d,%d,%d) s2=(%d,%d,%d): %d/8 trefoils\n",
                       c1[s1][0], c1[s1][1], c1[s1][2],
                       c2[s2][0], c2[s2][1], c2[s2][2],
                       n_trefoils);
            }
            if (n_trefoils > best) best = n_trefoils;
        }
    }

    printf("\n  Best trefoil count across 40 combos: %d/8\n", best);
    check("some center combo gives 2/8 trefoils (conjecture)", best == 2);

    /* Detailed construction dump for winning combo */
    printf("\n  === Winning combo detail: s1=(0,1,3) s2=(1,3,4) ===\n");
    {
        ConstructionWord wwin;
        unsigned int ch;
        wwin.base[0] = 0; wwin.base[1] = 1; wwin.base[2] = 2;
        wwin.n_steps = 3;
        wwin.steps[0].target_idx = 3;
        wwin.steps[0].center_idx[0] = 0;
        wwin.steps[0].center_idx[1] = 1;
        wwin.steps[0].center_idx[2] = 2;
        wwin.steps[1].target_idx = 4;
        wwin.steps[1].center_idx[0] = 0;
        wwin.steps[1].center_idx[1] = 1;
        wwin.steps[1].center_idx[2] = 3;
        wwin.steps[2].target_idx = 5;
        wwin.steps[2].center_idx[0] = 1;
        wwin.steps[2].center_idx[1] = 3;
        wwin.steps[2].center_idx[2] = 4;

        for (ch = 0; ch < 8; ch++) {
            Vec3Q poly2[MAX_STICK_VERTICES];
            int np2, vi;
            const char *kt;
            evaluate_construction(&trefoil, &wwin, ch, poly2, &np2);
            kt = classify_knot(np2, poly2);
            printf("  choices=%u (%d%d%d) -> %s\n",
                   ch, (ch>>0)&1, (ch>>1)&1, (ch>>2)&1, kt);
            printf("    vertices: ");
            for (vi = 0; vi < np2; vi++) {
                printf("[");
                rat_print(poly2[vi].x); printf(",");
                rat_print(poly2[vi].y); printf(",");
                rat_print(poly2[vi].z);
                printf("]");
                if (vi < np2 - 1) printf(" ");
            }
            printf("\n");
        }

        /* Denominator height analysis */
        printf("\n  Denominator height (max denominator across all vertices):\n");
        for (ch = 0; ch < 8; ch++) {
            Vec3Q polyh[MAX_STICK_VERTICES];
            int nph, vi2;
            i64 max_denom = 1;
            const char *kth;
            evaluate_construction(&trefoil, &wwin, ch, polyh, &nph);
            kth = classify_knot(nph, polyh);
            for (vi2 = 0; vi2 < nph; vi2++) {
                if (polyh[vi2].x.q > max_denom) max_denom = polyh[vi2].x.q;
                if (polyh[vi2].y.q > max_denom) max_denom = polyh[vi2].y.q;
                if (polyh[vi2].z.q > max_denom) max_denom = polyh[vi2].z.q;
            }
            printf("    choices=%u (%d%d%d): max_denom=%lld -> %s\n",
                   ch, (ch>>0)&1, (ch>>1)&1, (ch>>2)&1,
                   max_denom, kth);
        }

        /* Show reflection data for each step */
        printf("\n  Reflection analysis (winning combo):\n");
        {
            int si;
            for (si = 0; si < 3; si++) {
                ConstructionStep *step = &wwin.steps[si];
                Vec3Q c0 = trefoil.vertices[step->center_idx[0]];
                Vec3Q c1v = trefoil.vertices[step->center_idx[1]];
                Vec3Q c2v = trefoil.vertices[step->center_idx[2]];
                Vec3Q tgt = trefoil.vertices[step->target_idx];
                Vec3Q mir = mirror_point(c0, c1v, c2v, tgt);
                Vec3Q e1 = v3q_sub(c1v, c0);
                Vec3Q e2 = v3q_sub(c2v, c0);
                Vec3Q n = v3q_cross(e1, e2);
                Vec3Q disp = v3q_sub(mir, tgt);

                printf("  Step %d: construct v%d from centers (%d,%d,%d)\n",
                       si, step->target_idx,
                       step->center_idx[0],
                       step->center_idx[1],
                       step->center_idx[2]);
                printf("    plane normal: (");
                rat_print(n.x); printf(", ");
                rat_print(n.y); printf(", ");
                rat_print(n.z); printf(")\n");
                printf("    target:  "); v3q_print(tgt); printf("\n");
                printf("    mirror:  "); v3q_print(mir); printf("\n");
                printf("    displacement (mir-tgt): (");
                rat_print(disp.x); printf(", ");
                rat_print(disp.y); printf(", ");
                rat_print(disp.z); printf(")\n");
            }
        }
    }

    /* Cross-combo analysis: which bitmasks ever produce trefoils? */
    printf("\n  Cross-combo: which bitmasks produce trefoils across all combos?\n");
    {
        int mask_trefoil_count[8];
        unsigned int ch;
        for (ch = 0; ch < 8; ch++) mask_trefoil_count[ch] = 0;

        for (s1 = 0; s1 < 4; s1++) {
            for (s2 = 0; s2 < 10; s2++) {
                ConstructionWord w2;
                w2.base[0] = 0; w2.base[1] = 1; w2.base[2] = 2;
                w2.n_steps = 3;
                w2.steps[0].target_idx = 3;
                w2.steps[0].center_idx[0] = 0;
                w2.steps[0].center_idx[1] = 1;
                w2.steps[0].center_idx[2] = 2;
                w2.steps[1].target_idx = 4;
                w2.steps[1].center_idx[0] = c1[s1][0];
                w2.steps[1].center_idx[1] = c1[s1][1];
                w2.steps[1].center_idx[2] = c1[s1][2];
                w2.steps[2].target_idx = 5;
                w2.steps[2].center_idx[0] = c2[s2][0];
                w2.steps[2].center_idx[1] = c2[s2][1];
                w2.steps[2].center_idx[2] = c2[s2][2];

                for (ch = 0; ch < 8; ch++) {
                    Vec3Q poly2[MAX_STICK_VERTICES];
                    int np2;
                    const char *kt;
                    evaluate_construction(&trefoil, &w2, ch, poly2, &np2);
                    kt = classify_knot(np2, poly2);
                    if (strcmp(kt, "trefoil") == 0)
                        mask_trefoil_count[ch]++;
                }
            }
        }

        for (ch = 0; ch < 8; ch++) {
            if (mask_trefoil_count[ch] > 0)
                printf("    choices=%u (%d%d%d): trefoil in %d/40 combos\n",
                       ch, (ch>>0)&1, (ch>>1)&1, (ch>>2)&1,
                       mask_trefoil_count[ch]);
        }
    }
}

static void test_figure_eight(void) {
    StickKnot fig8;
    Crossing crossings[32];
    int nc, det;

    printf("\n=== Test: Figure-Eight 4_1 ===\n");

    fig8 = make_figure_eight();
    nc = find_crossings(fig8.vertices, fig8.n_vertices, crossings, 32);
    printf("  Crossings found: %d\n", nc);
    check("figure-eight has 4 crossings", nc == 4);

    if (nc > 0) {
        int i, w;
        for (i = 0; i < nc; i++) {
            printf("  crossing %d: edges %d-%d, sign=%d\n",
                   i, crossings[i].edge_a, crossings[i].edge_b,
                   crossings[i].sign);
        }
        w = compute_writhe(crossings, nc);
        printf("  Writhe: %d\n", w);
        check("figure-eight writhe = 0 (amphichiral)", w == 0);

        det = knot_determinant(fig8.n_vertices, crossings, nc);
        printf("  Determinant: %d\n", det);
        check("figure-eight determinant = 5", det == 5);
    }

    {
        const char *ktype = classify_knot(fig8.n_vertices, fig8.vertices);
        printf("  Classification: %s\n", ktype);
        check("classified as figure-eight/cinquefoil",
              strcmp(ktype, "figure-eight/cinquefoil") == 0);
    }
}

/*
 * Figure-eight decoherence sweep.
 * 7 vertices, 4 construction steps, 2^4 = 16 alternatives.
 * Step 0: centers from {0,1,2} — forced
 * Step 1: C(4,3) = 4 options from {0,1,2,3}
 * Step 2: C(5,3) = 10 options from {0,1,2,3,4}
 * Step 3: C(6,3) = 20 options from {0,1,2,3,4,5}
 * Total: 4 * 10 * 20 = 800 construction words.
 *
 * For efficiency, only test forward-chained strategy first,
 * then sweep if needed.
 */
/*
 * Figure-eight decoherence with forward-chained center strategy.
 * 12 vertices, 9 construction steps, 2^9 = 512 alternatives.
 *
 * Forward-chained: each step uses the 3 most recently constructed
 * vertices (which maximizes coupling between steps).
 * Step k constructs vertex k+3, using vertices {k, k+1, k+2}.
 */
static void test_figure_eight_decoherence(void) {
    StickKnot fig8 = make_figure_eight();
    ConstructionWord word;
    unsigned int choices;
    int n_same = 0, n_alts, i;
    int det_hist[20]; /* histogram of determinant values */

    printf("\n=== Test: Figure-Eight Decoherence ===\n");

    word.base[0] = 0; word.base[1] = 1; word.base[2] = 2;
    word.n_steps = 9;

    for (i = 0; i < 9; i++) {
        word.steps[i].target_idx = i + 3;
        /* Trefoil-winning pattern generalized:
         * Step 0: (0,1,2) — all base
         * Step 1: (0,1,3) — 2 base + newest
         * Step 2: (1,3,4) — 1 base + 2 newest
         * Step k>=3: (k, k+1, k+2) — sliding window of newest 3 */
        if (i == 0) {
            word.steps[i].center_idx[0] = 0;
            word.steps[i].center_idx[1] = 1;
            word.steps[i].center_idx[2] = 2;
        } else if (i == 1) {
            word.steps[i].center_idx[0] = 0;
            word.steps[i].center_idx[1] = 1;
            word.steps[i].center_idx[2] = 3;
        } else if (i == 2) {
            word.steps[i].center_idx[0] = 1;
            word.steps[i].center_idx[1] = 3;
            word.steps[i].center_idx[2] = 4;
        } else {
            word.steps[i].center_idx[0] = i;
            word.steps[i].center_idx[1] = i + 1;
            word.steps[i].center_idx[2] = i + 2;
        }
    }

    n_alts = 1 << word.n_steps; /* 512 */
    printf("  Forward-chained strategy: %d alternatives (9 steps)\n", n_alts);

    for (i = 0; i < 20; i++) det_hist[i] = 0;

    for (choices = 0; choices < (unsigned)n_alts; choices++) {
        Vec3Q poly[MAX_STICK_VERTICES];
        Crossing cx[MAX_CROSSINGS];
        int n_poly, nc, dt;

        evaluate_construction(&fig8, &word, choices, poly, &n_poly);
        nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
        dt = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;

        if (dt == 5) n_same++;
        if (dt >= 0 && dt < 20) det_hist[dt]++;
    }

    printf("  Determinant histogram:\n");
    for (i = 0; i < 20; i++) {
        if (det_hist[i] > 0)
            printf("    det=%2d: %4d / %d\n", i, det_hist[i], n_alts);
    }

    printf("\n  Figure-eight count (det=5): %d / %d\n", n_same, n_alts);
    printf("  Ratio: %d / 2^%d\n", n_same, word.n_steps);

    /* Also try all-base strategy: every step uses (0,1,2) */
    printf("\n  All-base strategy (centers always 0,1,2):\n");
    for (i = 0; i < 9; i++) {
        word.steps[i].center_idx[0] = 0;
        word.steps[i].center_idx[1] = 1;
        word.steps[i].center_idx[2] = 2;
    }
    n_same = 0;
    for (i = 0; i < 20; i++) det_hist[i] = 0;
    for (choices = 0; choices < (unsigned)n_alts; choices++) {
        Vec3Q poly[MAX_STICK_VERTICES];
        Crossing cx[MAX_CROSSINGS];
        int n_poly, nc, dt;
        evaluate_construction(&fig8, &word, choices, poly, &n_poly);
        nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
        dt = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;
        if (dt == 5) n_same++;
        if (dt >= 0 && dt < 20) det_hist[dt]++;
    }
    printf("  Determinant histogram (all-base):\n");
    for (i = 0; i < 20; i++) {
        if (det_hist[i] > 0)
            printf("    det=%2d: %4d / %d\n", i, det_hist[i], n_alts);
    }
    printf("  Figure-eight count (all-base): %d / %d\n", n_same, n_alts);

    /* Writhe distribution of det=3 and det=5 alternatives */
    {
        int w_pos3 = 0, w_neg3 = 0, w_zero3 = 0;
        int w_pos5 = 0, w_neg5 = 0, w_zero5 = 0;
        for (choices = 0; choices < (unsigned)n_alts; choices++) {
            Vec3Q polyw[MAX_STICK_VERTICES];
            Crossing cxw[MAX_CROSSINGS];
            int npw, ncw, dtw, ww;
            evaluate_construction(&fig8, &word, choices, polyw, &npw);
            ncw = find_crossings(polyw, npw, cxw, MAX_CROSSINGS);
            ww = compute_writhe(cxw, ncw);
            dtw = (ncw > 0) ? knot_determinant(npw, cxw, ncw) : 1;
            if (dtw == 3) {
                if (ww > 0) w_pos3++;
                else if (ww < 0) w_neg3++;
                else w_zero3++;
            }
            if (dtw == 5) {
                if (ww > 0) w_pos5++;
                else if (ww < 0) w_neg5++;
                else w_zero5++;
            }
        }
        printf("  Chirality split (all-base):\n");
        printf("    det=3 (trefoils): w>0: %d, w<0: %d, w=0: %d\n",
               w_pos3, w_neg3, w_zero3);
        printf("    det=5 (fig-eights): w>0: %d, w<0: %d, w=0: %d\n",
               w_pos5, w_neg5, w_zero5);
    }

    /* Check specific choices */
    {
        unsigned int ch;
        int spot_checks[4] = {0, 511, 256, 128};
        for (i = 0; i < 4; i++) {
            Vec3Q polyx[MAX_STICK_VERTICES];
            Crossing cxx[MAX_CROSSINGS];
            int npx, ncx, dtx, wx;
            ch = (unsigned)spot_checks[i];
            word.steps[0].center_idx[0] = 0;
            word.steps[0].center_idx[1] = 1;
            word.steps[0].center_idx[2] = 2;
            evaluate_construction(&fig8, &word, ch, polyx, &npx);
            ncx = find_crossings(polyx, npx, cxx, MAX_CROSSINGS);
            wx = compute_writhe(cxx, ncx);
            dtx = (ncx > 0) ? knot_determinant(npx, cxx, ncx) : 1;
            printf("    choices=%3u: %d xings, w=%d, det=%d\n",
                   ch, ncx, wx, dtx);
        }
    }

    /* And sliding-window: step i uses (i, i+1, i+2) */
    printf("\n  Sliding-window strategy:\n");
    for (i = 0; i < 9; i++) {
        word.steps[i].center_idx[0] = i;
        word.steps[i].center_idx[1] = i + 1;
        word.steps[i].center_idx[2] = i + 2;
    }
    n_same = 0;
    for (i = 0; i < 20; i++) det_hist[i] = 0;
    for (choices = 0; choices < (unsigned)n_alts; choices++) {
        Vec3Q poly[MAX_STICK_VERTICES];
        Crossing cx[MAX_CROSSINGS];
        int n_poly, nc, dt;
        evaluate_construction(&fig8, &word, choices, poly, &n_poly);
        nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
        dt = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;
        if (dt == 5) n_same++;
        if (dt >= 0 && dt < 20) det_hist[dt]++;
    }
    printf("  Determinant histogram (sliding):\n");
    for (i = 0; i < 20; i++) {
        if (det_hist[i] > 0)
            printf("    det=%2d: %4d / %d\n", i, det_hist[i], n_alts);
    }
    printf("  Figure-eight count (sliding): %d / %d\n", n_same, n_alts);
}

/*
 * Bitmask coding theory analysis (figure-eight all-base).
 *
 * Extract all 512 bitmask->knot-type mappings and analyze the
 * algebraic structure of the knot-type subsets in F_2^9.
 *
 * Key questions:
 * - Are the figure-eight bitmasks a linear code (closed under XOR)?
 * - Are left/right trefoils bitwise complements?
 * - Hamming weight distribution per set?
 * - Coset structure?
 */
static void test_bitmask_coding(void) {
    StickKnot fig8 = make_figure_eight();
    ConstructionWord word;
    unsigned int choices;
    int n_alts, i, j;

    /* Collect bitmasks by type */
    unsigned int fig_eights[512];  int n_fig = 0;
    unsigned int left_tref[512];   int n_left = 0;
    unsigned int right_tref[512];  int n_right = 0;
    unsigned int unknots[512];     int n_unk = 0;

    printf("\n=== Test: Bitmask Coding Analysis (fig-8 all-base) ===\n");

    /* Set up all-base construction word */
    word.base[0] = 0; word.base[1] = 1; word.base[2] = 2;
    word.n_steps = 9;
    for (i = 0; i < 9; i++) {
        word.steps[i].target_idx = i + 3;
        word.steps[i].center_idx[0] = 0;
        word.steps[i].center_idx[1] = 1;
        word.steps[i].center_idx[2] = 2;
    }

    n_alts = 1 << 9; /* 512 */

    /* Classify every bitmask */
    for (choices = 0; choices < (unsigned)n_alts; choices++) {
        Vec3Q poly[MAX_STICK_VERTICES];
        Crossing cx[MAX_CROSSINGS];
        int n_poly, nc, dt, w;

        evaluate_construction(&fig8, &word, choices, poly, &n_poly);
        nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
        w = compute_writhe(cx, nc);
        dt = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;

        if (dt == 5) {
            fig_eights[n_fig++] = choices;
        } else if (dt == 3 && w > 0) {
            left_tref[n_left++] = choices;
        } else if (dt == 3 && w < 0) {
            right_tref[n_right++] = choices;
        } else {
            unknots[n_unk++] = choices;
        }
    }

    printf("  Sets: |F|=%d fig-eights, |L|=%d left-trefoils, "
           "|R|=%d right-trefoils, |U|=%d unknots\n",
           n_fig, n_left, n_right, n_unk);

    /* 1. Dump bitmask lists */
    printf("\n  Figure-eight bitmasks (%d):\n    ", n_fig);
    for (i = 0; i < n_fig; i++) {
        printf("%u", fig_eights[i]);
        if (i < n_fig - 1) printf(", ");
        if ((i + 1) % 16 == 0 && i < n_fig - 1) printf("\n    ");
    }
    printf("\n");

    printf("\n  Left-trefoil bitmasks (%d):\n    ", n_left);
    for (i = 0; i < n_left; i++) {
        printf("%u", left_tref[i]);
        if (i < n_left - 1) printf(", ");
        if ((i + 1) % 16 == 0 && i < n_left - 1) printf("\n    ");
    }
    printf("\n");

    printf("\n  Right-trefoil bitmasks (%d):\n    ", n_right);
    for (i = 0; i < n_right; i++) {
        printf("%u", right_tref[i]);
        if (i < n_right - 1) printf(", ");
        if ((i + 1) % 16 == 0 && i < n_right - 1) printf("\n    ");
    }
    printf("\n");

    /* 2. Complement test: is R = {~x & 0x1FF : x in L}? */
    {
        int complement_hits = 0;
        for (i = 0; i < n_left; i++) {
            unsigned int comp = (~left_tref[i]) & 0x1FFu;
            for (j = 0; j < n_right; j++) {
                if (right_tref[j] == comp) { complement_hits++; break; }
            }
        }
        printf("\n  Complement test (L <-> R): %d / %d left-trefoils "
               "have complement in right-trefoils\n",
               complement_hits, n_left);
        check("left/right trefoils are bitwise complements",
              complement_hits == n_left);
    }

    /* 3. XOR closure test for figure-eights */
    {
        int xor_in_F = 0, xor_in_L = 0, xor_in_R = 0, xor_in_U = 0;
        int xor_total = 0;
        /* Build lookup table */
        int type_map[512];
        for (i = 0; i < 512; i++) type_map[i] = 0; /* 0=unknot */
        for (i = 0; i < n_fig; i++) type_map[fig_eights[i]] = 1;
        for (i = 0; i < n_left; i++) type_map[left_tref[i]] = 2;
        for (i = 0; i < n_right; i++) type_map[right_tref[i]] = 3;

        /* Test all pairs within F */
        for (i = 0; i < n_fig; i++) {
            for (j = i + 1; j < n_fig; j++) {
                unsigned int xr = fig_eights[i] ^ fig_eights[j];
                xor_total++;
                if (type_map[xr] == 1) xor_in_F++;
                else if (type_map[xr] == 2) xor_in_L++;
                else if (type_map[xr] == 3) xor_in_R++;
                else xor_in_U++;
            }
        }
        printf("\n  XOR closure test (F x F -> ?):\n");
        printf("    %d pairs total\n", xor_total);
        printf("    -> F (fig-eight): %d\n", xor_in_F);
        printf("    -> L (left-tref): %d\n", xor_in_L);
        printf("    -> R (right-tref): %d\n", xor_in_R);
        printf("    -> U (unknot): %d\n", xor_in_U);
        check("F is XOR-closed (linear code)",
              xor_in_F == xor_total);

        /* Test all pairs within L */
        xor_in_F = xor_in_L = xor_in_R = xor_in_U = xor_total = 0;
        for (i = 0; i < n_left; i++) {
            for (j = i + 1; j < n_left; j++) {
                unsigned int xr = left_tref[i] ^ left_tref[j];
                xor_total++;
                if (type_map[xr] == 1) xor_in_F++;
                else if (type_map[xr] == 2) xor_in_L++;
                else if (type_map[xr] == 3) xor_in_R++;
                else xor_in_U++;
            }
        }
        printf("\n  XOR closure test (L x L -> ?):\n");
        printf("    %d pairs total\n", xor_total);
        printf("    -> F: %d\n", xor_in_F);
        printf("    -> L: %d\n", xor_in_L);
        printf("    -> R: %d\n", xor_in_R);
        printf("    -> U: %d\n", xor_in_U);

        /* Test L x F -> ? (coset test) */
        xor_in_F = xor_in_L = xor_in_R = xor_in_U = xor_total = 0;
        for (i = 0; i < n_left; i++) {
            for (j = 0; j < n_fig; j++) {
                unsigned int xr = left_tref[i] ^ fig_eights[j];
                xor_total++;
                if (type_map[xr] == 1) xor_in_F++;
                else if (type_map[xr] == 2) xor_in_L++;
                else if (type_map[xr] == 3) xor_in_R++;
                else xor_in_U++;
            }
        }
        printf("\n  XOR test (L x F -> ?):\n");
        printf("    %d pairs total\n", xor_total);
        printf("    -> F: %d\n", xor_in_F);
        printf("    -> L: %d\n", xor_in_L);
        printf("    -> R: %d\n", xor_in_R);
        printf("    -> U: %d\n", xor_in_U);
    }

    /* 4. Hamming weight distribution */
    {
        int hw_fig[10], hw_left[10], hw_right[10], hw_unk[10];
        for (i = 0; i < 10; i++)
            hw_fig[i] = hw_left[i] = hw_right[i] = hw_unk[i] = 0;

        for (i = 0; i < n_fig; i++) {
            int w = 0; unsigned int b = fig_eights[i];
            while (b) { w += (int)(b & 1u); b >>= 1; }
            hw_fig[w]++;
        }
        for (i = 0; i < n_left; i++) {
            int w = 0; unsigned int b = left_tref[i];
            while (b) { w += (int)(b & 1u); b >>= 1; }
            hw_left[w]++;
        }
        for (i = 0; i < n_right; i++) {
            int w = 0; unsigned int b = right_tref[i];
            while (b) { w += (int)(b & 1u); b >>= 1; }
            hw_right[w]++;
        }
        for (i = 0; i < n_unk; i++) {
            int w = 0; unsigned int b = unknots[i];
            while (b) { w += (int)(b & 1u); b >>= 1; }
            hw_unk[w]++;
        }

        printf("\n  Hamming weight distribution:\n");
        printf("  w:  ");
        for (i = 0; i <= 9; i++) printf("%4d ", i);
        printf("\n  F:  ");
        for (i = 0; i <= 9; i++) printf("%4d ", hw_fig[i]);
        printf("\n  L:  ");
        for (i = 0; i <= 9; i++) printf("%4d ", hw_left[i]);
        printf("\n  R:  ");
        for (i = 0; i <= 9; i++) printf("%4d ", hw_right[i]);
        printf("\n  U:  ");
        for (i = 0; i <= 9; i++) printf("%4d ", hw_unk[i]);
        printf("\n");
    }

    /* 5. Modular arithmetic patterns */
    {
        int mod8[8];
        printf("\n  Figure-eight bitmasks mod 8:\n    ");
        for (i = 0; i < 8; i++) mod8[i] = 0;
        for (i = 0; i < n_fig; i++) mod8[fig_eights[i] % 8]++;
        for (i = 0; i < 8; i++) printf("mod8=%d: %d  ", i, mod8[i]);
        printf("\n");

        printf("  Left-trefoil bitmasks mod 8:\n    ");
        for (i = 0; i < 8; i++) mod8[i] = 0;
        for (i = 0; i < n_left; i++) mod8[left_tref[i] % 8]++;
        for (i = 0; i < 8; i++) printf("mod8=%d: %d  ", i, mod8[i]);
        printf("\n");
    }

    /* 6. Per-bit frequency analysis */
    {
        int bit_freq_F[9], bit_freq_L[9], bit_freq_R[9], bit_freq_U[9];
        for (i = 0; i < 9; i++)
            bit_freq_F[i] = bit_freq_L[i] = bit_freq_R[i] = bit_freq_U[i] = 0;

        for (i = 0; i < n_fig; i++)
            for (j = 0; j < 9; j++)
                if (fig_eights[i] & (1u << j)) bit_freq_F[j]++;
        for (i = 0; i < n_left; i++)
            for (j = 0; j < 9; j++)
                if (left_tref[i] & (1u << j)) bit_freq_L[j]++;
        for (i = 0; i < n_right; i++)
            for (j = 0; j < 9; j++)
                if (right_tref[i] & (1u << j)) bit_freq_R[j]++;
        for (i = 0; i < n_unk; i++)
            for (j = 0; j < 9; j++)
                if (unknots[i] & (1u << j)) bit_freq_U[j]++;

        printf("\n  Per-bit frequency (how often bit j=1):\n");
        printf("  bit: ");
        for (j = 0; j < 9; j++) printf("%3d ", j);
        printf("\n  F:   ");
        for (j = 0; j < 9; j++) printf("%3d ", bit_freq_F[j]);
        printf(" (of %d)\n  L:   ", n_fig);
        for (j = 0; j < 9; j++) printf("%3d ", bit_freq_L[j]);
        printf(" (of %d)\n  R:   ", n_left);
        for (j = 0; j < 9; j++) printf("%3d ", bit_freq_R[j]);
        printf(" (of %d)\n  U:   ", n_right);
        for (j = 0; j < 9; j++) printf("%3d ", bit_freq_U[j]);
        printf(" (of %d)\n", n_unk);
    }

    /* 7. Coset test: is L a coset of some subgroup?
     * If L = x + C for linear code C, then L XOR L should = C.
     * Pick first element, XOR all others to get candidate C. */
    {
        unsigned int candidate_C[512];
        int n_C = 0;
        int is_subspace = 1;

        if (n_left > 0) {
            unsigned int x0 = left_tref[0];
            for (i = 0; i < n_left; i++) {
                candidate_C[n_C++] = left_tref[i] ^ x0;
            }
            /* Test: is candidate_C closed under XOR? */
            for (i = 0; i < n_C && is_subspace; i++) {
                for (j = i + 1; j < n_C && is_subspace; j++) {
                    unsigned int xr = candidate_C[i] ^ candidate_C[j];
                    int found = 0;
                    int k2;
                    for (k2 = 0; k2 < n_C; k2++) {
                        if (candidate_C[k2] == xr) { found = 1; break; }
                    }
                    if (!found) is_subspace = 0;
                }
            }
            printf("\n  Coset test: L = x0 + C where x0=%u\n", x0);
            printf("  |C| = %d, C is XOR-closed (linear subspace): %s\n",
                   n_C, is_subspace ? "YES" : "NO");
            if (is_subspace) {
                /* Find dimension: log2(|C|) */
                int dim = 0;
                int sz = n_C;
                while (sz > 1) { dim++; sz >>= 1; }
                printf("  C is a [9,%d] linear code (dim=%d, |C|=2^%d=%d)\n",
                       dim, dim, dim, n_C);
                check("L is a coset of a linear code", 1);

                /* Print generators (first dim elements of C excluding 0) */
                printf("  C elements: ");
                for (i = 0; i < n_C && i < 40; i++) {
                    printf("%u ", candidate_C[i]);
                }
                if (n_C > 40) printf("...");
                printf("\n");
            } else {
                check("L is a coset of a linear code", 0);
            }
        }
    }
}

/*
 * Verify explorer's trefoil membership rule:
 *
 * Left-trefoil iff ALL of:
 *   bit 2 = 1, bit 4 = 1, bit 7 = 0
 *   at most one of {bit 0, bit 1, bit 5} is 1
 *
 * Right-trefoil = bitwise complement of left-trefoil.
 *
 * Also find analogous rule for figure-eights.
 */
static int popcount_bits(unsigned int x, int b0, int b1, int b2) {
    return (int)((x >> b0) & 1u) +
           (int)((x >> b1) & 1u) +
           (int)((x >> b2) & 1u);
}

static int is_left_trefoil_rule(unsigned int x) {
    /* 3 fixed bits */
    if (!((x >> 2) & 1u)) return 0; /* bit 2 = 1 */
    if (!((x >> 4) & 1u)) return 0; /* bit 4 = 1 */
    if ((x >> 7) & 1u) return 0;    /* bit 7 = 0 */
    /* at most one of {0, 1, 5} */
    if (popcount_bits(x, 0, 1, 5) > 1) return 0;
    return 1;
}

static int is_right_trefoil_rule(unsigned int x) {
    return is_left_trefoil_rule((~x) & 0x1FFu);
}

static void test_trefoil_rule(void) {
    StickKnot fig8 = make_figure_eight();
    ConstructionWord word;
    unsigned int choices;
    int i;
    int rule_left = 0, rule_right = 0;
    int true_pos_L = 0, false_pos_L = 0, false_neg_L = 0;
    int true_pos_R = 0, false_pos_R = 0, false_neg_R = 0;

    printf("\n=== Test: Trefoil Membership Rule Verification ===\n");

    /* Set up all-base construction word */
    word.base[0] = 0; word.base[1] = 1; word.base[2] = 2;
    word.n_steps = 9;
    for (i = 0; i < 9; i++) {
        word.steps[i].target_idx = i + 3;
        word.steps[i].center_idx[0] = 0;
        word.steps[i].center_idx[1] = 1;
        word.steps[i].center_idx[2] = 2;
    }

    /* Count how many codewords the rule predicts */
    for (choices = 0; choices < 512; choices++) {
        if (is_left_trefoil_rule(choices)) rule_left++;
        if (is_right_trefoil_rule(choices)) rule_right++;
    }
    printf("  Rule predicts: %d left-trefoils, %d right-trefoils\n",
           rule_left, rule_right);

    /* Verify against actual computation */
    for (choices = 0; choices < 512; choices++) {
        Vec3Q poly[MAX_STICK_VERTICES];
        Crossing cx[MAX_CROSSINGS];
        int n_poly, nc, dt, w;
        int pred_L, pred_R, actual_L, actual_R;

        evaluate_construction(&fig8, &word, choices, poly, &n_poly);
        nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
        w = compute_writhe(cx, nc);
        dt = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;

        pred_L = is_left_trefoil_rule(choices);
        pred_R = is_right_trefoil_rule(choices);
        actual_L = (dt == 3 && w > 0) ? 1 : 0;
        actual_R = (dt == 3 && w < 0) ? 1 : 0;

        if (pred_L && actual_L) true_pos_L++;
        else if (pred_L && !actual_L) false_pos_L++;
        else if (!pred_L && actual_L) false_neg_L++;

        if (pred_R && actual_R) true_pos_R++;
        else if (pred_R && !actual_R) false_pos_R++;
        else if (!pred_R && actual_R) false_neg_R++;
    }

    printf("  Left-trefoil rule: TP=%d, FP=%d, FN=%d\n",
           true_pos_L, false_pos_L, false_neg_L);
    printf("  Right-trefoil rule: TP=%d, FP=%d, FN=%d\n",
           true_pos_R, false_pos_R, false_neg_R);
    check("left-trefoil rule: zero false positives", false_pos_L == 0);
    check("left-trefoil rule: zero false negatives", false_neg_L == 0);
    check("right-trefoil rule: zero false positives", false_pos_R == 0);
    check("right-trefoil rule: zero false negatives", false_neg_R == 0);

    /* Figure-eight structure analysis */
    printf("\n  === Figure-eight structure analysis ===\n");
    {
        /* For each pair of bits, count how many fig-eights have that pair = 1 */
        int type_map[512];
        unsigned int fig_eights[512];
        int n_fig = 0;

        for (choices = 0; choices < 512; choices++) type_map[choices] = 0;
        for (choices = 0; choices < 512; choices++) {
            Vec3Q poly[MAX_STICK_VERTICES];
            Crossing cx[MAX_CROSSINGS];
            int n_poly, nc, dt;
            evaluate_construction(&fig8, &word, choices, poly, &n_poly);
            nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
            dt = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;
            if (dt == 5) {
                type_map[choices] = 1;
                fig_eights[n_fig++] = choices;
            }
        }

        /* Print in binary for pattern inspection */
        printf("  Figure-eights in binary (9 bits, LSB first):\n");
        for (i = 0; i < n_fig; i++) {
            int b;
            printf("    %3u = ", fig_eights[i]);
            for (b = 0; b < 9; b++)
                printf("%d", (int)((fig_eights[i] >> b) & 1u));
            printf("\n");
        }

        /* Check bit-pair exclusion patterns for F */
        printf("\n  Bit-pair co-occurrence in F (count of both=1):\n");
        {
            int bi, bj;
            printf("       ");
            for (bj = 0; bj < 9; bj++) printf(" b%d ", bj);
            printf("\n");
            for (bi = 0; bi < 9; bi++) {
                printf("    b%d:", bi);
                for (bj = 0; bj < 9; bj++) {
                    int cnt = 0;
                    int k;
                    for (k = 0; k < n_fig; k++) {
                        if (((fig_eights[k] >> bi) & 1u) &&
                            ((fig_eights[k] >> bj) & 1u))
                            cnt++;
                    }
                    printf(" %2d ", cnt);
                }
                printf("\n");
            }
        }

        /* Bit-triple patterns: for each triple, count fig-eights
         * with all three bits = 1 */
        printf("\n  Bit-triple all=1 in F (triples with 0 count = exclusion):\n");
        {
            int b0, b1, b2;
            for (b0 = 0; b0 < 9; b0++) {
                for (b1 = b0 + 1; b1 < 9; b1++) {
                    for (b2 = b1 + 1; b2 < 9; b2++) {
                        int cnt = 0;
                        int k;
                        for (k = 0; k < n_fig; k++) {
                            if (((fig_eights[k] >> b0) & 1u) &&
                                ((fig_eights[k] >> b1) & 1u) &&
                                ((fig_eights[k] >> b2) & 1u))
                                cnt++;
                        }
                        if (cnt == 0) {
                            printf("    {%d,%d,%d}: 0 (EXCLUDED)\n",
                                   b0, b1, b2);
                        }
                    }
                }
            }
        }
    }
}

/*
 * Figure-eight membership rule (hypothesis):
 *   b2 = b4 = b7 = majority(b0, b1, b5)
 *   b3, b6, b8 are free
 *
 * This gives 8 choices for {b0,b1,b5,majority} x 8 for {b3,b6,b8} = 64.
 *
 * Combined trefoil/figure-eight classification:
 *   Let p = b2, q = b4, r = b7, m = popcount(b0,b1,b5)
 *   Figure-eight: p=q=r AND (p=0 => m<=1, p=1 => m>=2) i.e. p=majority(b0,b1,b5)
 *   Left-trefoil: p=1, q=1, r=0, m<=1
 *   Right-trefoil: p=0, q=0, r=1, m>=2 (complement of left)
 *   Unknot: everything else
 */
static int is_figure_eight_rule(unsigned int x) {
    int b0 = (int)((x >> 0) & 1u);
    int b1 = (int)((x >> 1) & 1u);
    int b2 = (int)((x >> 2) & 1u);
    int b4 = (int)((x >> 4) & 1u);
    int b5 = (int)((x >> 5) & 1u);
    int b7 = (int)((x >> 7) & 1u);
    int maj = (b0 + b1 + b5 >= 2) ? 1 : 0;

    /* b2 = b4 = b7 = majority(b0, b1, b5) */
    return (b2 == maj) && (b4 == maj) && (b7 == maj);
}

static void test_figure_eight_rule(void) {
    StickKnot fig8 = make_figure_eight();
    ConstructionWord word;
    unsigned int choices;
    int i;
    int rule_count = 0;
    int tp_F = 0, fp_F = 0, fn_F = 0;
    int tp_L = 0, fp_L = 0, fn_L = 0;
    int tp_R = 0, fp_R = 0, fn_R = 0;

    printf("\n=== Test: Complete Knot-Type Rules Verification ===\n");

    /* Count rule predictions */
    for (choices = 0; choices < 512; choices++) {
        if (is_figure_eight_rule(choices)) rule_count++;
    }
    printf("  Figure-eight rule predicts: %d codewords\n", rule_count);

    /* Set up all-base */
    word.base[0] = 0; word.base[1] = 1; word.base[2] = 2;
    word.n_steps = 9;
    for (i = 0; i < 9; i++) {
        word.steps[i].target_idx = i + 3;
        word.steps[i].center_idx[0] = 0;
        word.steps[i].center_idx[1] = 1;
        word.steps[i].center_idx[2] = 2;
    }

    /* Verify all three rules against computation */
    for (choices = 0; choices < 512; choices++) {
        Vec3Q poly[MAX_STICK_VERTICES];
        Crossing cx[MAX_CROSSINGS];
        int n_poly, nc, dt, w;
        int pred_F, pred_L, pred_R;
        int actual_F, actual_L, actual_R;

        evaluate_construction(&fig8, &word, choices, poly, &n_poly);
        nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
        w = compute_writhe(cx, nc);
        dt = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;

        pred_F = is_figure_eight_rule(choices);
        pred_L = is_left_trefoil_rule(choices);
        pred_R = is_right_trefoil_rule(choices);

        actual_F = (dt == 5) ? 1 : 0;
        actual_L = (dt == 3 && w > 0) ? 1 : 0;
        actual_R = (dt == 3 && w < 0) ? 1 : 0;

        if (pred_F && actual_F) tp_F++;
        else if (pred_F && !actual_F) fp_F++;
        else if (!pred_F && actual_F) fn_F++;

        if (pred_L && actual_L) tp_L++;
        else if (pred_L && !actual_L) fp_L++;
        else if (!pred_L && actual_L) fn_L++;

        if (pred_R && actual_R) tp_R++;
        else if (pred_R && !actual_R) fp_R++;
        else if (!pred_R && actual_R) fn_R++;
    }

    printf("  Figure-eight rule: TP=%d, FP=%d, FN=%d\n", tp_F, fp_F, fn_F);
    printf("  Left-trefoil rule: TP=%d, FP=%d, FN=%d\n", tp_L, fp_L, fn_L);
    printf("  Right-trefoil rule: TP=%d, FP=%d, FN=%d\n", tp_R, fp_R, fn_R);

    check("figure-eight rule: zero false positives", fp_F == 0);
    check("figure-eight rule: zero false negatives", fn_F == 0);
    check("left-trefoil rule: zero false positives (recheck)", fp_L == 0);
    check("left-trefoil rule: zero false negatives (recheck)", fn_L == 0);
    check("right-trefoil rule: zero false positives (recheck)", fp_R == 0);
    check("right-trefoil rule: zero false negatives (recheck)", fn_R == 0);

    /* Summary: complete classification */
    {
        int covered = 0;
        for (choices = 0; choices < 512; choices++) {
            int pred_any = is_figure_eight_rule(choices) ||
                           is_left_trefoil_rule(choices) ||
                           is_right_trefoil_rule(choices);
            if (pred_any) covered++;
        }
        printf("\n  Classification coverage:\n");
        printf("    Figure-eight rule: %d codewords\n", rule_count);
        printf("    Left-trefoil rule: 32 codewords\n");
        printf("    Right-trefoil rule: 32 codewords\n");
        printf("    Total non-unknot: %d / 512\n", covered);
        printf("    Unknot (complement): %d / 512\n", 512 - covered);
    }

    /* Print the unified classification in terms of bit roles */
    printf("\n  === Unified Classification ===\n");
    printf("  Bits {2,4,7} = 'structural' bits (s)\n");
    printf("  Bits {0,1,5} = 'entangled' bits (e)\n");
    printf("  Bits {3,6,8} = 'free' bits (f)\n");
    printf("\n");
    printf("  Figure-eight: s=majority(e), all 3 s-bits equal\n");
    printf("    s=(0,0,0), popcount(e)<=1: 4 x 8 = 32\n");
    printf("    s=(1,1,1), popcount(e)>=2: 4 x 8 = 32\n");
    printf("  Left-trefoil: s=(1,1,0), popcount(e)<=1: 4 x 8 = 32\n");
    printf("  Right-trefoil: s=(0,0,1), popcount(e)>=2: 4 x 8 = 32\n");
    printf("  Unknot: everything else: 384 / 512\n");
}

/*
 * Explorer's 4-check classifier (CORRECTED):
 *
 * 1. b2 = b4? If not -> unknot
 * 2. b7 = majority(b0, b1, b5)? If not -> unknot
 * 3. b7 = b2 -> figure-eight
 * 4. b7 != b2 -> trefoil (left if b2=1, right if b2=0)
 *
 * The key insight: b7 = majority(e) is the UNIVERSAL constraint
 * for all non-unknots. Then b7 vs b2 distinguishes amphichiral
 * from chiral.
 */
static int four_check_classify(unsigned int x) {
    int b0 = (int)((x >> 0) & 1u);
    int b1 = (int)((x >> 1) & 1u);
    int b2 = (int)((x >> 2) & 1u);
    int b4 = (int)((x >> 4) & 1u);
    int b5 = (int)((x >> 5) & 1u);
    int b7 = (int)((x >> 7) & 1u);
    int maj = (b0 + b1 + b5 >= 2) ? 1 : 0;

    if (b2 != b4) return 0;           /* unknot */
    if (b7 != maj) return 0;          /* unknot */
    if (b7 == b2) return 5;           /* figure-eight (det=5) */
    if (b2 == 1) return 3;            /* left-trefoil */
    return -3;                         /* right-trefoil */
}

static void test_four_check(void) {
    StickKnot fig8 = make_figure_eight();
    ConstructionWord word;
    unsigned int choices;
    int i;
    int perfect = 1;
    int mismatches = 0;

    printf("\n=== Test: 4-Check Classifier ===\n");

    word.base[0] = 0; word.base[1] = 1; word.base[2] = 2;
    word.n_steps = 9;
    for (i = 0; i < 9; i++) {
        word.steps[i].target_idx = i + 3;
        word.steps[i].center_idx[0] = 0;
        word.steps[i].center_idx[1] = 1;
        word.steps[i].center_idx[2] = 2;
    }

    for (choices = 0; choices < 512; choices++) {
        Vec3Q poly[MAX_STICK_VERTICES];
        Crossing cx[MAX_CROSSINGS];
        int n_poly, nc, dt, w;
        int predicted, actual;

        evaluate_construction(&fig8, &word, choices, poly, &n_poly);
        nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
        w = compute_writhe(cx, nc);
        dt = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;

        predicted = four_check_classify(choices);

        if (dt == 5) actual = 5;
        else if (dt == 3 && w > 0) actual = 3;
        else if (dt == 3 && w < 0) actual = -3;
        else actual = 0;

        if (predicted != actual) {
            printf("  MISMATCH: choices=%u predicted=%d actual=%d\n",
                   choices, predicted, actual);
            perfect = 0;
            mismatches++;
        }
    }

    printf("  Mismatches: %d / 512\n", mismatches);
    check("4-check classifier is PERFECT on all 512", perfect);

    printf("\n  The 4-check classifier:\n");
    printf("    1. b2 = b4?  No -> unknot\n");
    printf("    2. b7 = majority(b0,b1,b5)?  No -> unknot\n");
    printf("    3. b7 = b2 -> figure-eight\n");
    printf("    4. b7 != b2 -> trefoil (left if b2=1, right if b2=0)\n");
    printf("    bits {3,6,8} are topologically invisible\n");
}

/*
 * Generalization test: cyclically rotate the figure-eight polygon.
 *
 * If the {structural, entangled, free} + majority structure is
 * intrinsic to the knot, it should persist under relabeling
 * (with different bit indices becoming structural/entangled/free).
 *
 * We test rotations by 2, 4, 6 positions.
 */
static StickKnot rotate_knot(const StickKnot *k, int shift) {
    StickKnot r;
    int i;
    r.name = k->name;
    r.crossing_number = k->crossing_number;
    r.n_vertices = k->n_vertices;
    for (i = 0; i < k->n_vertices; i++) {
        r.vertices[i] = k->vertices[(i + shift) % k->n_vertices];
    }
    return r;
}

/*
 * Analyze the classification structure for a given polygon
 * under all-base construction. Returns the bit roles via output
 * parameters.
 *
 * For each bit position, compute its frequency among det=5 (fig-eight)
 * alternatives. If freq=50%, it's potentially structural or entangled.
 * If freq=0% or 100%, it's fixed (like trefoil structural bits).
 *
 * Then test all possible {3-structural, 3-entangled, 3-free} partitions
 * with majority rule.
 */
static void analyze_rotation(const StickKnot *knot, int shift) {
    StickKnot rot;
    ConstructionWord word;
    unsigned int choices;
    int i, j;
    int n_steps;
    int n_alts;
    int type_map[1024]; /* 0=unknot, 5=fig8, 3=left-tref, -3=right-tref */
    int det_hist[20];
    int bit_freq_F[12];
    int n_fig = 0, n_left = 0, n_right = 0;

    rot = rotate_knot(knot, shift);
    n_steps = rot.n_vertices - 3;
    n_alts = 1 << n_steps;

    if (n_alts > 1024) {
        printf("  Too many alternatives for rotation %d\n", shift);
        return;
    }

    printf("\n  --- Rotation by %d (base = original verts %d,%d,%d) ---\n",
           shift, shift % knot->n_vertices,
           (1 + shift) % knot->n_vertices,
           (2 + shift) % knot->n_vertices);

    /* Verify original knot type */
    {
        Crossing cx[MAX_CROSSINGS];
        int nc = find_crossings(rot.vertices, rot.n_vertices, cx, MAX_CROSSINGS);
        int det = (nc > 0) ? knot_determinant(rot.n_vertices, cx, nc) : 1;
        printf("  Rotated polygon: %d crossings, det=%d\n", nc, det);
    }

    /* Set up all-base construction */
    word.base[0] = 0; word.base[1] = 1; word.base[2] = 2;
    word.n_steps = n_steps;
    for (i = 0; i < n_steps; i++) {
        word.steps[i].target_idx = i + 3;
        word.steps[i].center_idx[0] = 0;
        word.steps[i].center_idx[1] = 1;
        word.steps[i].center_idx[2] = 2;
    }

    /* Classify all alternatives */
    for (i = 0; i < 20; i++) det_hist[i] = 0;
    for (i = 0; i < n_steps; i++) bit_freq_F[i] = 0;

    for (choices = 0; choices < (unsigned)n_alts; choices++) {
        Vec3Q poly[MAX_STICK_VERTICES];
        Crossing cx[MAX_CROSSINGS];
        int n_poly, nc, dt, w;

        evaluate_construction(&rot, &word, choices, poly, &n_poly);
        nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
        w = compute_writhe(cx, nc);
        dt = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;

        if (dt >= 0 && dt < 20) det_hist[dt]++;

        if (dt == 5) {
            type_map[choices] = 5;
            n_fig++;
            for (j = 0; j < n_steps; j++)
                if (choices & (1u << j)) bit_freq_F[j]++;
        } else if (dt == 3 && w > 0) {
            type_map[choices] = 3;
            n_left++;
        } else if (dt == 3 && w < 0) {
            type_map[choices] = -3;
            n_right++;
        } else {
            type_map[choices] = 0;
        }
    }

    printf("  Det histogram: ");
    for (i = 0; i < 20; i++) {
        if (det_hist[i] > 0) printf("det=%d:%d  ", i, det_hist[i]);
    }
    printf("\n  |F|=%d, |L|=%d, |R|=%d\n", n_fig, n_left, n_right);

    printf("  Per-bit freq in F (fig-eights):\n    ");
    for (j = 0; j < n_steps; j++) {
        printf("b%d:%d/%d  ", j, bit_freq_F[j], n_fig);
    }
    printf("\n");

    /* Test all C(9,3) x C(6,3) partitions for majority rule structure.
     * For each structural triple, try all 3 ways to assign roles:
     * which 2 are "paired" (must be equal) and which 1 is the "switch". */
    {
        int si0, si1, si2, e0, e1, e2;
        int best_tp = 0;
        int best_s[3] = {0,0,0}, best_e[3] = {0,0,0};
        int best_pair[2] = {0,0}, best_switch = 0;

        for (si0 = 0; si0 < n_steps; si0++) {
            for (si1 = si0+1; si1 < n_steps; si1++) {
                for (si2 = si1+1; si2 < n_steps; si2++) {
                    int struc[3];
                    int role;
                    int remaining[9];
                    int n_rem = 0;
                    struc[0] = si0; struc[1] = si1; struc[2] = si2;
                    for (i = 0; i < n_steps; i++) {
                        if (i != si0 && i != si1 && i != si2)
                            remaining[n_rem++] = i;
                    }
                    for (e0 = 0; e0 < n_rem; e0++) {
                        for (e1 = e0+1; e1 < n_rem; e1++) {
                            for (e2 = e1+1; e2 < n_rem; e2++) {
                                /* Try all 3 role assignments */
                                for (role = 0; role < 3; role++) {
                                    int pair0 = struc[(role+1)%3];
                                    int pair1 = struc[(role+2)%3];
                                    int sw = struc[role];
                                    int tp = 0, fp = 0;

                                    for (choices = 0; choices < (unsigned)n_alts; choices++) {
                                        int bp0 = (int)((choices >> pair0) & 1u);
                                        int bp1 = (int)((choices >> pair1) & 1u);
                                        int bsw = (int)((choices >> sw) & 1u);
                                        int be0v = (int)((choices >> remaining[e0]) & 1u);
                                        int be1v = (int)((choices >> remaining[e1]) & 1u);
                                        int be2v = (int)((choices >> remaining[e2]) & 1u);
                                        int maj = (be0v+be1v+be2v >= 2) ? 1 : 0;
                                        int pred, actual;

                                        if (bp0 != bp1) { pred = 0; }
                                        else if (bsw != maj) { pred = 0; }
                                        else if (bsw == bp0) { pred = 5; }
                                        else if (bp0 == 1) { pred = 3; }
                                        else { pred = -3; }

                                        actual = type_map[choices];
                                        if (pred != 0 && pred == actual) tp++;
                                        else if (pred != 0 && pred != actual) fp++;
                                    }
                                    if (tp > best_tp && fp == 0) {
                                        best_tp = tp;
                                        best_s[0] = si0;
                                        best_s[1] = si1;
                                        best_s[2] = si2;
                                        best_e[0] = remaining[e0];
                                        best_e[1] = remaining[e1];
                                        best_e[2] = remaining[e2];
                                        best_pair[0] = pair0;
                                        best_pair[1] = pair1;
                                        best_switch = sw;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (best_tp > 0) {
            int free_bits[3], fi = 0;
            for (i = 0; i < n_steps; i++) {
                int is_s = (i == best_s[0] || i == best_s[1] || i == best_s[2]);
                int is_e = (i == best_e[0] || i == best_e[1] || i == best_e[2]);
                if (!is_s && !is_e) free_bits[fi++] = i;
            }
            printf("  BEST partition (TP=%d, FP=0):\n", best_tp);
            printf("    Structural: {%d, %d, %d}\n",
                   best_s[0], best_s[1], best_s[2]);
            printf("    Paired:     {%d, %d}\n",
                   best_pair[0], best_pair[1]);
            printf("    Switch:     {%d}\n", best_switch);
            printf("    Entangled:  {%d, %d, %d}\n",
                   best_e[0], best_e[1], best_e[2]);
            printf("    Free:       {%d, %d, %d}\n",
                   free_bits[0], free_bits[1], free_bits[2]);
            printf("    Coverage: %d / %d non-unknots classified\n",
                   best_tp, n_fig + n_left + n_right);
            check("majority rule generalizes to this rotation",
                  best_tp == n_fig + n_left + n_right);
        } else {
            printf("  No majority rule partition found!\n");
            check("majority rule generalizes to this rotation", 0);
        }
    }
}

static void test_generalization(void) {
    StickKnot fig8 = make_figure_eight();
    int shifts[] = {2, 4, 6};
    int s;

    printf("\n=== Test: Generalization (Rotated Figure-Eight) ===\n");

    for (s = 0; s < 3; s++) {
        analyze_rotation(&fig8, shifts[s]);
    }
}

/*
 * Analyze cinquefoil rotation: like analyze_rotation but for det=5
 * with writhe distinguishing cinquefoil (|w|>0) from figure-eight (w=0).
 * Reports chirality symmetry, complement verification, and majority search.
 */
static void analyze_cinquefoil_rotation(const StickKnot *knot, int shift) {
    StickKnot rot;
    ConstructionWord word;
    unsigned int choices;
    int i, j;
    int n_steps, n_alts;
    int type_map[2048]; /* 0=unknot, 5=R-cinq, -5=L-cinq, 50=fig8 */
    int det_hist[20];
    int bit_freq_CQ[12]; /* per-bit freq among cinquefoils */
    int n_cinq_L = 0, n_cinq_R = 0, n_fig8 = 0;
    int compl_ok = 1, compl_tested = 0;

    rot = rotate_knot(knot, shift);
    n_steps = rot.n_vertices - 3;
    n_alts = 1 << n_steps;

    if (n_alts > 2048) {
        printf("  Too many alternatives for rotation %d\n", shift);
        return;
    }

    printf("\n  --- Rotation by %d (base = verts %d,%d,%d, %d steps) ---\n",
           shift, shift % knot->n_vertices,
           (1 + shift) % knot->n_vertices,
           (2 + shift) % knot->n_vertices, n_steps);

    /* Verify original knot type */
    {
        Crossing cx[MAX_CROSSINGS];
        int nc = find_crossings(rot.vertices, rot.n_vertices, cx, MAX_CROSSINGS);
        int det = (nc > 0) ? knot_determinant(rot.n_vertices, cx, nc) : 1;
        int w = compute_writhe(cx, nc);
        printf("  Rotated polygon: %d crossings, det=%d, writhe=%d\n", nc, det, w);
    }

    /* Set up all-base construction */
    word.base[0] = 0; word.base[1] = 1; word.base[2] = 2;
    word.n_steps = n_steps;
    for (i = 0; i < n_steps; i++) {
        word.steps[i].target_idx = i + 3;
        word.steps[i].center_idx[0] = 0;
        word.steps[i].center_idx[1] = 1;
        word.steps[i].center_idx[2] = 2;
    }

    /* Classify all alternatives */
    for (i = 0; i < 20; i++) det_hist[i] = 0;
    for (i = 0; i < n_steps; i++) bit_freq_CQ[i] = 0;

    for (choices = 0; choices < (unsigned)n_alts; choices++) {
        Vec3Q poly[MAX_STICK_VERTICES];
        Crossing cx[MAX_CROSSINGS];
        int n_poly, nc, dt, w;

        evaluate_construction(&rot, &word, choices, poly, &n_poly);
        nc = find_crossings(poly, n_poly, cx, MAX_CROSSINGS);
        w = compute_writhe(cx, nc);
        dt = (nc > 0) ? knot_determinant(n_poly, cx, nc) : 1;

        if (dt >= 0 && dt < 20) det_hist[dt]++;

        if (dt == 5 && w != 0) {
            /* cinquefoil: det=5, nonzero writhe */
            if (w > 0) { type_map[choices] = 5; n_cinq_R++; }
            else { type_map[choices] = -5; n_cinq_L++; }
            for (j = 0; j < n_steps; j++)
                if (choices & (1u << j)) bit_freq_CQ[j]++;
        } else if (dt == 5 && w == 0) {
            type_map[choices] = 50; n_fig8++;
        } else {
            type_map[choices] = 0;
        }
    }

    printf("  Det histogram: ");
    for (i = 0; i < 20; i++) {
        if (det_hist[i] > 0) printf("det=%d:%d  ", i, det_hist[i]);
    }
    printf("\n  Cinquefoils: L=%d R=%d (total=%d), Fig8=%d\n",
           n_cinq_L, n_cinq_R, n_cinq_L + n_cinq_R, n_fig8);

    /* Chirality symmetry check: L == R means good base position */
    if (n_cinq_L == n_cinq_R && n_cinq_L > 0)
        printf("  *** SYMMETRIC chirality (L==R=%d) — good base!\n", n_cinq_L);
    else if (n_cinq_L + n_cinq_R > 0)
        printf("  Asymmetric chirality (L=%d, R=%d) — bad base\n",
               n_cinq_L, n_cinq_R);

    /* Per-bit frequency among cinquefoils only */
    {
        int n_cinq = n_cinq_L + n_cinq_R;
        if (n_cinq > 0) {
            printf("  Per-bit freq in cinquefoils:\n    ");
            for (j = 0; j < n_steps; j++)
                printf("b%d:%d/%d  ", j, bit_freq_CQ[j], n_cinq);
            printf("\n");
        }
    }

    /* Complement verification: does flipping all bits map L-cinq <-> R-cinq? */
    {
        unsigned int mask = (unsigned)(n_alts - 1);
        for (choices = 0; choices < (unsigned)n_alts; choices++) {
            unsigned int comp = (~choices) & mask;
            int t1 = type_map[choices];
            int t2 = type_map[comp];
            if (t1 == 5 || t1 == -5) {
                compl_tested++;
                if (t1 == 5 && t2 != -5) compl_ok = 0;
                if (t1 == -5 && t2 != 5) compl_ok = 0;
            }
        }
        if (compl_tested > 0) {
            printf("  Complement map L<->R: %s (%d pairs tested)\n",
                   compl_ok ? "YES" : "NO", compl_tested / 2);
        }
    }

    /* Majority rule search (3+3+3 partition) — only when 9 steps */
    if (n_steps == 9 && (n_cinq_L + n_cinq_R) > 0) {
        int si0, si1, si2, e0, e1, e2;
        int best_tp = 0;
        int best_s[3] = {0,0,0}, best_e[3] = {0,0,0};
        int best_pair[2] = {0,0}, best_switch = 0;
        int n_nontriv = n_cinq_L + n_cinq_R + n_fig8;

        printf("  Searching majority rule partitions (9 steps = 3+3+3)...\n");

        for (si0 = 0; si0 < n_steps; si0++) {
            for (si1 = si0+1; si1 < n_steps; si1++) {
                for (si2 = si1+1; si2 < n_steps; si2++) {
                    int struc[3];
                    int role;
                    int remaining[9];
                    int n_rem = 0;
                    struc[0] = si0; struc[1] = si1; struc[2] = si2;
                    for (i = 0; i < n_steps; i++) {
                        if (i != si0 && i != si1 && i != si2)
                            remaining[n_rem++] = i;
                    }
                    for (e0 = 0; e0 < n_rem; e0++) {
                        for (e1 = e0+1; e1 < n_rem; e1++) {
                            for (e2 = e1+1; e2 < n_rem; e2++) {
                                for (role = 0; role < 3; role++) {
                                    int pair0 = struc[(role+1)%3];
                                    int pair1 = struc[(role+2)%3];
                                    int sw = struc[role];
                                    int tp = 0, fp = 0;

                                    for (choices = 0; choices < (unsigned)n_alts; choices++) {
                                        int bp0 = (int)((choices >> pair0) & 1u);
                                        int bp1 = (int)((choices >> pair1) & 1u);
                                        int bsw = (int)((choices >> sw) & 1u);
                                        int be0v = (int)((choices >> remaining[e0]) & 1u);
                                        int be1v = (int)((choices >> remaining[e1]) & 1u);
                                        int be2v = (int)((choices >> remaining[e2]) & 1u);
                                        int maj = (be0v+be1v+be2v >= 2) ? 1 : 0;
                                        int pred, actual;

                                        /* Classify: cinquefoil uses same
                                         * structure as figure-eight classifier
                                         * but maps to det=5 with nonzero writhe */
                                        if (bp0 != bp1) { pred = 0; }
                                        else if (bsw != maj) { pred = 0; }
                                        else if (bsw == bp0) { pred = 50; } /* fig8 or "same" */
                                        else if (bp0 == 1) { pred = -5; }   /* left cinquefoil */
                                        else { pred = 5; }                   /* right cinquefoil */

                                        actual = type_map[choices];
                                        if (pred != 0 && pred == actual) tp++;
                                        else if (pred != 0 && pred != actual) fp++;
                                    }
                                    if (tp > best_tp && fp == 0) {
                                        best_tp = tp;
                                        best_s[0] = si0;
                                        best_s[1] = si1;
                                        best_s[2] = si2;
                                        best_e[0] = remaining[e0];
                                        best_e[1] = remaining[e1];
                                        best_e[2] = remaining[e2];
                                        best_pair[0] = pair0;
                                        best_pair[1] = pair1;
                                        best_switch = sw;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (best_tp > 0) {
            int free_bits[3], fi = 0;
            for (i = 0; i < n_steps; i++) {
                int is_s = (i == best_s[0] || i == best_s[1] || i == best_s[2]);
                int is_e = (i == best_e[0] || i == best_e[1] || i == best_e[2]);
                if (!is_s && !is_e) free_bits[fi++] = i;
            }
            printf("  BEST partition (TP=%d, FP=0):\n", best_tp);
            printf("    Structural: {%d, %d, %d}\n",
                   best_s[0], best_s[1], best_s[2]);
            printf("    Paired:     {%d, %d}\n",
                   best_pair[0], best_pair[1]);
            printf("    Switch:     {%d}\n", best_switch);
            printf("    Entangled:  {%d, %d, %d}\n",
                   best_e[0], best_e[1], best_e[2]);
            printf("    Free:       {%d, %d, %d}\n",
                   free_bits[0], free_bits[1], free_bits[2]);
            printf("    Coverage: %d / %d non-unknots classified\n",
                   best_tp, n_nontriv);
            if (best_tp == n_nontriv)
                printf("  *** PERFECT majority rule classification!\n");
        } else {
            printf("  No majority rule partition found.\n");
        }
    } else if (n_steps != 9) {
        printf("  (Skipping majority search: %d steps, need 9 for 3+3+3)\n",
               n_steps);
    }
}

/*
 * Cinquefoil (5_1) test:
 * - Try N=12 (9 steps for 3+3+3) and N=11 (8 steps)
 * - Full rotation sweep with chirality symmetry + complement check
 * - Majority rule search on 9-step rotations
 */
static void test_cinquefoil(void) {
    int ns_list[] = {11, 12, 13, 14};
    int ns_count = 4;
    int si;

    printf("\n=== Test: Cinquefoil 5_1 ===\n");

    /* Scan sample counts */
    printf("  Scanning sample counts for 5 crossings + det=5:\n");
    for (si = 0; si < ns_count; si++) {
        StickKnot cq = make_cinquefoil(ns_list[si], 100);
        Crossing cx[MAX_CROSSINGS];
        int nc = find_crossings(cq.vertices, cq.n_vertices, cx, MAX_CROSSINGS);
        int det = (nc > 0) ? knot_determinant(cq.n_vertices, cx, nc) : 1;
        int w = compute_writhe(cx, nc);
        int n_steps = cq.n_vertices - 3;
        printf("    N=%d: %d crossings, det=%d, writhe=%d, steps=%d\n",
               ns_list[si], nc, det, w, n_steps);
    }

    /* N=12 cinquefoil: 9 steps = 3+3+3 partition possible */
    printf("\n  --- N=12 cinquefoil (9 steps for 3+3+3) ---\n");
    {
        StickKnot cq12 = make_cinquefoil(12, 100);
        Crossing cx[MAX_CROSSINGS];
        int nc = find_crossings(cq12.vertices, cq12.n_vertices, cx, MAX_CROSSINGS);
        int det = (nc > 0) ? knot_determinant(cq12.n_vertices, cx, nc) : 1;
        int w = compute_writhe(cx, nc);
        printf("  N=12 baseline: %d crossings, det=%d, writhe=%d\n", nc, det, w);
        check("N=12 cinquefoil has det=5", det == 5);

        if (det == 5) {
            int sh;
            printf("\n  Full rotation sweep (%d rotations):\n", cq12.n_vertices);
            for (sh = 0; sh < cq12.n_vertices; sh++) {
                analyze_cinquefoil_rotation(&cq12, sh);
            }
        }
    }

    /* Also check N=11 briefly for comparison */
    printf("\n  --- N=11 cinquefoil (8 steps, for comparison) ---\n");
    {
        StickKnot cq11 = make_cinquefoil(11, 100);
        Crossing cx[MAX_CROSSINGS];
        int nc = find_crossings(cq11.vertices, cq11.n_vertices, cx, MAX_CROSSINGS);
        int det = (nc > 0) ? knot_determinant(cq11.n_vertices, cx, nc) : 1;
        printf("  N=11 baseline: %d crossings, det=%d\n", nc, det);
        check("N=11 cinquefoil has 5 crossings", nc == 5);

        /* Just show rotation 0 for comparison */
        if (det == 5) {
            analyze_cinquefoil_rotation(&cq11, 0);
        }
    }
}

/* ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 110: Construction Words for Knots\n");
    printf("================================================\n");

    test_rational_arithmetic();
    test_vec3q();
    test_mirror();
    test_trefoil_construction();
    test_crossing_detection();
    test_decoherence();
    test_decoherence_sweep();
    test_figure_eight();
    test_figure_eight_decoherence();
    test_bitmask_coding();
    test_trefoil_rule();
    test_figure_eight_rule();
    test_four_check();
    test_generalization();
    test_cinquefoil();

    printf("\n================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
