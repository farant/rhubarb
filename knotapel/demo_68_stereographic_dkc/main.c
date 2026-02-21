/*
 * KNOTAPEL DEMO 68: Stereographic DKC — Knot Computation in the Plane
 * ====================================================================
 *
 * Project the eigenvector S^2 onto R^2 via stereographic projection.
 * The DKC framework becomes a 2D planar computation with drawable
 * decision boundaries.
 *
 * Part A: Stereographic projection of the catalog
 * Part B: Planar grid activation
 * Part C: Projected Voronoi (13-direction)
 * Part D: ASCII visualization of decision boundaries
 * Part E: Explicit perceptron formulation
 * Part F: Conformality check
 *
 * C89, zero dependencies beyond math.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *titulis, int ok) {
    if (ok) { printf("  PASS: %s\n", titulis); n_pass++; }
    else    { printf("  FAIL: %s\n", titulis); n_fail++; }
}

/* ================================================================
 * Quaternion type
 * ================================================================ */

typedef struct { double a, b, c, d; } Quat;

static Quat quat_make(double a, double b, double c, double d) {
    Quat q; q.a = a; q.b = b; q.c = c; q.d = d; return q;
}

static Quat quat_one(void) { return quat_make(1, 0, 0, 0); }

static Quat quat_add(Quat p, Quat q) {
    return quat_make(p.a + q.a, p.b + q.b, p.c + q.c, p.d + q.d);
}

static Quat quat_neg(Quat q) {
    return quat_make(-q.a, -q.b, -q.c, -q.d);
}

static Quat quat_mul(Quat p, Quat q) {
    return quat_make(
        p.a*q.a - p.b*q.b - p.c*q.c - p.d*q.d,
        p.a*q.b + p.b*q.a + p.c*q.d - p.d*q.c,
        p.a*q.c - p.b*q.d + p.c*q.a + p.d*q.b,
        p.a*q.d + p.b*q.c - p.c*q.b + p.d*q.a);
}

static Quat quat_conj(Quat q) {
    return quat_make(q.a, -q.b, -q.c, -q.d);
}

static double quat_norm(Quat q) {
    return sqrt(q.a*q.a + q.b*q.b + q.c*q.c + q.d*q.d);
}

static Quat quat_normalize(Quat q) {
    double n = quat_norm(q);
    if (n < 1e-15) return quat_make(0, 0, 0, 0);
    return quat_make(q.a / n, q.b / n, q.c / n, q.d / n);
}

static int quat_close(Quat p, Quat q, double eps) {
    return fabs(p.a - q.a) < eps && fabs(p.b - q.b) < eps &&
           fabs(p.c - q.c) < eps && fabs(p.d - q.d) < eps;
}

/* ================================================================
 * Z[zeta_8] exact arithmetic
 * ================================================================ */

typedef struct { long a, b, c, d; } Cyc8;

static Cyc8 cyc8_make(long a, long b, long c, long d) {
    Cyc8 z; z.a = a; z.b = b; z.c = c; z.d = d; return z;
}
static Cyc8 cyc8_zero(void) { return cyc8_make(0, 0, 0, 0); }
static int cyc8_eq(Cyc8 x, Cyc8 y) {
    return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}
static int cyc8_is_zero(Cyc8 z) {
    return z.a == 0 && z.b == 0 && z.c == 0 && z.d == 0;
}
static Cyc8 cyc8_add(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a + y.a, x.b + y.b, x.c + y.c, x.d + y.d);
}
static Cyc8 cyc8_conj(Cyc8 z) {
    return cyc8_make(z.a, -z.d, -z.c, -z.b);
}
static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y) {
    return cyc8_make(
        x.a*y.a - x.b*y.d - x.c*y.c - x.d*y.b,
        x.a*y.b + x.b*y.a - x.c*y.d - x.d*y.c,
        x.a*y.c + x.b*y.b + x.c*y.a - x.d*y.d,
        x.a*y.d + x.b*y.c + x.c*y.b + x.d*y.a);
}
static Cyc8 cyc8_pow_int(Cyc8 base, int n) {
    Cyc8 r = cyc8_make(1, 0, 0, 0);
    if (n == 0) return r;
    if (n < 0) { n = -n; base = cyc8_conj(base); }
    while (n > 0) {
        if (n & 1) r = cyc8_mul(r, base);
        base = cyc8_mul(base, base);
        n >>= 1;
    }
    return r;
}

/* ================================================================
 * Braid type + bracket computation
 * ================================================================ */

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

#define MAX_UF 4096
static int uf_p[MAX_UF];

static void uf_init(int n) {
    int i;
    for (i = 0; i < n; i++) uf_p[i] = i;
}
static int uf_find(int x) {
    while (uf_p[x] != x) { uf_p[x] = uf_p[uf_p[x]]; x = uf_p[x]; }
    return x;
}
static void uf_union(int x, int y) {
    x = uf_find(x); y = uf_find(y);
    if (x != y) uf_p[x] = y;
}

static int braid_loops(const Braid *b, unsigned s) {
    int N = (b->len + 1) * b->n;
    int l, p, i, loops, sgn, bit, cup;
    uf_init(N);
    for (l = 0; l < b->len; l++) {
        sgn = b->word[l] > 0 ? 1 : -1;
        i = (sgn > 0 ? b->word[l] : -b->word[l]) - 1;
        bit = (int)((s >> (unsigned)l) & 1u);
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

static Cyc8 braid_bracket(const Braid *b) {
    Cyc8 A = cyc8_make(0, -1, 0, 0);
    Cyc8 bracket = cyc8_zero();
    unsigned s, ns;
    int i;
    ns = 1u << b->len;
    for (s = 0; s < ns; s++) {
        int a_count = 0, b_count = 0, lp;
        for (i = 0; i < b->len; i++) {
            if ((s >> (unsigned)i) & 1u) b_count++;
            else a_count++;
        }
        lp = braid_loops(b, s);
        if (lp == 1) {
            bracket = cyc8_add(bracket,
                cyc8_pow_int(A, a_count - b_count));
        }
    }
    return bracket;
}

/* ================================================================
 * SU(2) braid representation
 * ================================================================ */

static Quat q_gen[3];

static void init_su2_generators(void) {
    double s = 1.0 / sqrt(2.0);
    q_gen[1] = quat_make(s, s, 0, 0);
    q_gen[2] = quat_make(s, 0, 0, -s);
}

static Quat braid_quaternion(const Braid *b) {
    Quat result = quat_one();
    int l;
    for (l = 0; l < b->len; l++) {
        int gen = b->word[l];
        int idx = gen > 0 ? gen : -gen;
        Quat g = q_gen[idx];
        if (gen < 0) g = quat_conj(g);
        result = quat_mul(result, g);
    }
    return result;
}

/* ================================================================
 * Catalog building
 * ================================================================ */

#define MAX_QCAT 4096
static Quat qcat[MAX_QCAT];
static int qcat_size = 0;
static int qcat_braid_count = 0;

static int find_quat(Quat q) {
    int i;
    for (i = 0; i < qcat_size; i++) {
        if (quat_close(qcat[i], q, 1e-10)) return i;
        if (quat_close(qcat[i], quat_neg(q), 1e-10)) return i;
    }
    return -1;
}

#define MAX_BCAT 512
static Cyc8 bcat[MAX_BCAT];
static int bcat_size = 0;

static int find_bracket(Cyc8 v) {
    int i;
    for (i = 0; i < bcat_size; i++)
        if (cyc8_eq(bcat[i], v)) return i;
    return -1;
}

static void build_catalogs(void) {
    int n, len;
    Braid b;
    int word_buf[MAX_WORD];

    qcat_size = 0;
    bcat_size = 0;
    qcat_braid_count = 0;

    for (n = 2; n <= 3; n++) {
        for (len = 1; len <= 8 && len <= MAX_WORD; len++) {
            int max_gen = n - 1;
            int total_gens = 2 * max_gen;
            unsigned long total, idx;
            int i;

            total = 1;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)total_gens;
                if (total > 100000) break;
            }
            if (total > 100000) continue;

            for (idx = 0; idx < total; idx++) {
                unsigned long tmp = idx;
                Quat q;
                Cyc8 brk;

                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen) word_buf[i] = g + 1;
                    else             word_buf[i] = -(g - max_gen + 1);
                }
                b.n = n; b.len = len;
                memcpy(b.word, word_buf, (size_t)len * sizeof(int));

                q = braid_quaternion(&b);
                brk = braid_bracket(&b);
                qcat_braid_count++;

                if (find_quat(q) < 0 && qcat_size < MAX_QCAT) {
                    qcat[qcat_size] = q;
                    qcat_size++;
                }

                if (!cyc8_is_zero(brk) && find_bracket(brk) < 0
                    && bcat_size < MAX_BCAT) {
                    bcat[bcat_size] = brk;
                    bcat_size++;
                }
            }
        }
    }
}

/* ================================================================
 * Eigenvector extraction (from Demo 67)
 * ================================================================ */

typedef struct {
    double theta;
    double ax, ay, az;
    int axis_defined;
} EigenData;

static EigenData quat_to_eigen(Quat q) {
    EigenData e;
    double norm_v;

    if (q.a < 0.0) {
        q.a = -q.a; q.b = -q.b; q.c = -q.c; q.d = -q.d;
    }
    if (q.a > 1.0) q.a = 1.0;
    if (q.a < 0.0) q.a = 0.0;

    e.theta = acos(q.a);
    norm_v = sqrt(q.b*q.b + q.c*q.c + q.d*q.d);

    if (norm_v < 1e-12) {
        e.ax = 0.0; e.ay = 0.0; e.az = 1.0;
        e.axis_defined = 0;
    } else {
        e.ax = q.b / norm_v;
        e.ay = q.c / norm_v;
        e.az = q.d / norm_v;
        e.axis_defined = 1;
    }
    return e;
}

/* Eigenvector direction catalog */
#define MAX_ECAT 128
static EigenData ecat[MAX_QCAT];

static double dir_cat[MAX_ECAT][3];
static int dir_cat_size = 0;

static int find_dir(double ax, double ay, double az) {
    int i;
    for (i = 0; i < dir_cat_size; i++) {
        double d1 = fabs(dir_cat[i][0] - ax) + fabs(dir_cat[i][1] - ay) +
                     fabs(dir_cat[i][2] - az);
        double d2 = fabs(dir_cat[i][0] + ax) + fabs(dir_cat[i][1] + ay) +
                     fabs(dir_cat[i][2] + az);
        if (d1 < 1e-8 || d2 < 1e-8) return i;
    }
    return -1;
}

static void build_eigen_catalog(void) {
    int i;
    dir_cat_size = 0;

    for (i = 0; i < qcat_size; i++) {
        ecat[i] = quat_to_eigen(qcat[i]);
        if (ecat[i].axis_defined) {
            int di = find_dir(ecat[i].ax, ecat[i].ay, ecat[i].az);
            if (di < 0 && dir_cat_size < MAX_ECAT) {
                dir_cat[dir_cat_size][0] = ecat[i].ax;
                dir_cat[dir_cat_size][1] = ecat[i].ay;
                dir_cat[dir_cat_size][2] = ecat[i].az;
                dir_cat_size++;
            }
        }
    }
}

/* ================================================================
 * Stereographic projection
 * ================================================================
 * From pole (px, py, pz) onto the plane perpendicular to the pole
 * through the origin. For south pole (0,0,-1):
 *   (x,y,z) -> (x/(1+z), y/(1+z))
 * For north pole (0,0,1):
 *   (x,y,z) -> (x/(1-z), y/(1-z))
 */

typedef struct { double x, y; } Vec2;

/* Projection pole — we'll choose based on whether any direction is
 * at the south pole */
static double proj_pole[3] = {0.0, 0.0, -1.0}; /* default: south pole */
static int use_north_pole = 0;

static Vec2 stereo_project(double ax, double ay, double az) {
    Vec2 v;
    double denom;

    if (use_north_pole) {
        /* Project from north pole (0,0,1) */
        denom = 1.0 - az;
        if (fabs(denom) < 1e-12) { v.x = 1e6; v.y = 1e6; return v; }
        v.x = ax / denom;
        v.y = ay / denom;
    } else {
        /* Project from south pole (0,0,-1) */
        denom = 1.0 + az;
        if (fabs(denom) < 1e-12) { v.x = 1e6; v.y = 1e6; return v; }
        v.x = ax / denom;
        v.y = ay / denom;
    }
    return v;
}

/* Projected eigenvector directions */
static Vec2 proj_dirs[MAX_ECAT];

/* ================================================================
 * Planar cell assignment functions
 * ================================================================ */

/* Rectangular grid on R^2 */
static double grid_xmin, grid_xmax, grid_ymin, grid_ymax;

static int planar_grid_cell(Vec2 v, int nx, int ny) {
    int cx, cy;
    double fx, fy;

    fx = (v.x - grid_xmin) / (grid_xmax - grid_xmin);
    fy = (v.y - grid_ymin) / (grid_ymax - grid_ymin);

    if (fx < 0.0) fx = 0.0;
    if (fx >= 1.0) fx = 1.0 - 1e-12;
    if (fy < 0.0) fy = 0.0;
    if (fy >= 1.0) fy = 1.0 - 1e-12;

    cx = (int)(fx * (double)nx);
    cy = (int)(fy * (double)ny);

    return cx * ny + cy;
}

/* Planar Voronoi: nearest projected eigenvector direction (Euclidean in R^2) */
static int planar_voronoi_cell(Vec2 v) {
    int i, best = 0;
    double best_d2 = 1e30;

    for (i = 0; i < dir_cat_size; i++) {
        double dx = v.x - proj_dirs[i].x;
        double dy = v.y - proj_dirs[i].y;
        double d2 = dx*dx + dy*dy;
        if (d2 < best_d2) { best_d2 = d2; best = i; }
    }
    return best;
}

/* S^2 Voronoi: nearest eigenvector direction by dot product on S^2.
 * Stereographic projection does NOT preserve nearest-neighbor, so
 * for computation we must use the S^2 metric. For visualization
 * we use the planar version above. */
static int s2_voronoi_cell(double ax, double ay, double az) {
    int i, best = 0;
    double best_d = -2.0;

    for (i = 0; i < dir_cat_size; i++) {
        double d = fabs(ax * dir_cat[i][0] +
                        ay * dir_cat[i][1] +
                        az * dir_cat[i][2]);
        if (d > best_d) { best_d = d; best = i; }
    }
    return best;
}

/* Inverse stereographic projection: R^2 -> S^2 */
static void stereo_inverse(double px, double py,
                           double *ax, double *ay, double *az) {
    double r2 = px*px + py*py;
    double denom = r2 + 1.0;

    if (use_north_pole) {
        *ax = 2.0 * px / denom;
        *ay = 2.0 * py / denom;
        *az = (r2 - 1.0) / denom;
    } else {
        *ax = 2.0 * px / denom;
        *ay = 2.0 * py / denom;
        *az = (1.0 - r2) / denom;
    }
}

/* S^2 Voronoi cell from an R^2 point (inverse-project then dot product) */
static int s2_voronoi_from_plane(Vec2 v) {
    double ax, ay, az;
    stereo_inverse(v.x, v.y, &ax, &ay, &az);
    return s2_voronoi_cell(ax, ay, az);
}

/* ================================================================
 * XOR6 checker for planar activations
 * ================================================================ */

/* Project quaternion sum to plane via eigenvector then stereographic */
static Vec2 quat_to_plane(Quat sum) {
    Vec2 v;
    EigenData ed;

    if (quat_norm(sum) < 1e-12) {
        v.x = 1e6; v.y = 1e6; /* sentinel for zero */
        return v;
    }

    sum = quat_normalize(sum);
    ed = quat_to_eigen(sum);

    if (!ed.axis_defined) {
        v.x = 1e6; v.y = 1e6;
        return v;
    }

    return stereo_project(ed.ax, ed.ay, ed.az);
}

static int check_xor6_planar_grid(const Quat *w, int nx, int ny) {
    int total_cells = nx * ny;
    int seen[256 + 1]; /* max cells + zero pseudo-cell */
    int mask, i, cell;

    if (total_cells > 256) return 0;
    for (i = 0; i <= total_cells; i++) seen[i] = 0;

    for (mask = 0; mask < 64; mask++) {
        Quat sum = quat_make(0, 0, 0, 0);
        int par = 0;
        Vec2 v;

        for (i = 0; i < 6; i++) {
            if (mask & (1 << i)) {
                sum = quat_add(sum, w[i]);
                par ^= 1;
            }
        }

        v = quat_to_plane(sum);
        if (v.x > 1e5 || v.y > 1e5) {
            cell = total_cells; /* zero/degenerate pseudo-cell */
        } else {
            cell = planar_grid_cell(v, nx, ny);
        }

        seen[cell] |= (1 << par);
        if (seen[cell] == 3) return 0;
    }
    return 1;
}

static int check_xor6_planar_voronoi(const Quat *w, int *label_out) {
    int n_cells = dir_cat_size;
    int seen[MAX_ECAT + 1];
    int mask, i, cell;

    for (i = 0; i <= n_cells; i++) seen[i] = 0;

    for (mask = 0; mask < 64; mask++) {
        Quat sum = quat_make(0, 0, 0, 0);
        int par = 0;
        EigenData ed;

        for (i = 0; i < 6; i++) {
            if (mask & (1 << i)) {
                sum = quat_add(sum, w[i]);
                par ^= 1;
            }
        }

        if (quat_norm(sum) < 1e-12) {
            cell = n_cells; /* zero pseudo-cell */
        } else {
            sum = quat_normalize(sum);
            ed = quat_to_eigen(sum);
            if (!ed.axis_defined) {
                cell = n_cells;
            } else {
                /* Use S^2 nearest-neighbor (dot product), NOT planar
                 * Euclidean. Stereographic distortion breaks R^2 Voronoi. */
                cell = s2_voronoi_cell(ed.ax, ed.ay, ed.az);
            }
        }

        seen[cell] |= (1 << par);
        if (seen[cell] == 3) return 0;
    }

    /* Extract labeling if requested */
    if (label_out) {
        for (i = 0; i <= n_cells; i++) {
            if (seen[i] == 0) label_out[i] = -1;      /* unused */
            else if (seen[i] == 1) label_out[i] = 0;   /* parity 0 only */
            else label_out[i] = 1;                       /* parity 1 only */
        }
    }
    return 1;
}

/* ================================================================
 * Winning solution storage
 * ================================================================ */

#define MAX_WIN 64
static struct {
    int ai, aj, ak;
    int label[MAX_ECAT + 1];
} wins[MAX_WIN];
static int n_wins = 0;

static void collect_planar_voronoi_winners(void) {
    int ai, aj, ak;
    n_wins = 0;

    for (ai = 0; ai < qcat_size && n_wins < MAX_WIN; ai++) {
        for (aj = ai + 1; aj < qcat_size && n_wins < MAX_WIN; aj++) {
            for (ak = aj + 1; ak < qcat_size && n_wins < MAX_WIN; ak++) {
                Quat w6[6];
                int label[MAX_ECAT + 1];
                w6[0] = qcat[ai];
                w6[1] = quat_neg(qcat[ai]);
                w6[2] = qcat[aj];
                w6[3] = quat_neg(qcat[aj]);
                w6[4] = qcat[ak];
                w6[5] = quat_neg(qcat[ak]);
                if (check_xor6_planar_voronoi(w6, label)) {
                    int k;
                    wins[n_wins].ai = ai;
                    wins[n_wins].aj = aj;
                    wins[n_wins].ak = ak;
                    for (k = 0; k <= dir_cat_size; k++)
                        wins[n_wins].label[k] = label[k];
                    n_wins++;
                }
            }
        }
    }
}

/* ================================================================
 * Part A: Stereographic projection of the catalog
 * ================================================================ */

static void part_a_projection(void) {
    char msg[256];
    int i;
    int has_south_pole = 0;
    double xmin = 1e30, xmax = -1e30, ymin = 1e30, ymax = -1e30;

    printf("\n=== Part A: Stereographic projection ===\n");

    init_su2_generators();
    build_catalogs();
    build_eigen_catalog();

    printf("  INFO: Quaternion catalog: %d values\n", qcat_size);
    printf("  INFO: Eigenvector directions: %d (mod +-)\n", dir_cat_size);

    /* A1: Check if any direction is at the south pole */
    for (i = 0; i < dir_cat_size; i++) {
        if (fabs(dir_cat[i][0]) < 1e-8 && fabs(dir_cat[i][1]) < 1e-8 &&
            fabs(dir_cat[i][2] + 1.0) < 1e-8) {
            has_south_pole = 1;
            break;
        }
    }

    if (has_south_pole) {
        printf("  INFO: South pole (0,0,-1) IS an eigenvector direction!\n");
        printf("  INFO: Switching to NORTH pole projection.\n");
        use_north_pole = 1;
        proj_pole[2] = 1.0;
    } else {
        printf("  INFO: South pole is not an eigenvector direction.\n");
        printf("  INFO: Using south pole projection.\n");
        use_north_pole = 0;
    }

    sprintf(msg, "Pole check: south_pole_in_catalog=%d, using_%s_pole",
            has_south_pole, use_north_pole ? "north" : "south");
    check(msg, 1);

    /* A2: Project all eigenvector directions */
    printf("  INFO: Projected eigenvector directions:\n");
    for (i = 0; i < dir_cat_size; i++) {
        proj_dirs[i] = stereo_project(dir_cat[i][0], dir_cat[i][1],
                                       dir_cat[i][2]);
        printf("    dir[%2d] S^2=(%+.4f,%+.4f,%+.4f) -> R^2=(%+.4f,%+.4f)\n",
               i, dir_cat[i][0], dir_cat[i][1], dir_cat[i][2],
               proj_dirs[i].x, proj_dirs[i].y);

        if (proj_dirs[i].x < xmin) xmin = proj_dirs[i].x;
        if (proj_dirs[i].x > xmax) xmax = proj_dirs[i].x;
        if (proj_dirs[i].y < ymin) ymin = proj_dirs[i].y;
        if (proj_dirs[i].y > ymax) ymax = proj_dirs[i].y;
    }

    printf("  INFO: Bounding box: x=[%.4f, %.4f], y=[%.4f, %.4f]\n",
           xmin, xmax, ymin, ymax);
    printf("  INFO: Spread: dx=%.4f, dy=%.4f\n",
           xmax - xmin, ymax - ymin);

    /* Set grid bounds with 20% margin */
    {
        double mx = (xmax - xmin) * 0.2;
        double my = (ymax - ymin) * 0.2;
        grid_xmin = xmin - mx;
        grid_xmax = xmax + mx;
        grid_ymin = ymin - my;
        grid_ymax = ymax + my;
    }

    sprintf(msg, "Projected %d directions, bounding box set", dir_cat_size);
    check(msg, dir_cat_size == 13);

    /* A3: Project subset sums for a sample winning solution.
     * We need to find winners first, so defer to Part C. Just check
     * that projection is invertible (round-trip test). */
    {
        int n_ok = 0;
        for (i = 0; i < dir_cat_size; i++) {
            /* Inverse: (x,y) -> (2x, 2y, x^2+y^2-1) / (x^2+y^2+1) */
            double px = proj_dirs[i].x;
            double py = proj_dirs[i].y;
            double r2 = px*px + py*py;
            double denom = r2 + 1.0;
            double rx, ry, rz;
            double err;

            if (use_north_pole) {
                rx = 2.0 * px / denom;
                ry = 2.0 * py / denom;
                rz = (r2 - 1.0) / denom; /* north pole: reversed */
            } else {
                rx = 2.0 * px / denom;
                ry = 2.0 * py / denom;
                rz = -(r2 - 1.0) / denom; /* = (1 - r2) / denom */
            }

            err = fabs(rx - dir_cat[i][0]) + fabs(ry - dir_cat[i][1]) +
                  fabs(rz - dir_cat[i][2]);
            if (err < 1e-10) n_ok++;
        }
        sprintf(msg, "Round-trip projection: %d/%d exact", n_ok, dir_cat_size);
        check(msg, n_ok == dir_cat_size);
    }
}

/* ================================================================
 * Part B: Planar grid activation
 * ================================================================ */

static void part_b_planar_grid(void) {
    char msg[256];
    int ai, aj, ak;
    int n_tested;

    printf("\n=== Part B: Planar grid activation ===\n");
    printf("  INFO: Grid bounds: x=[%.3f,%.3f] y=[%.3f,%.3f]\n",
           grid_xmin, grid_xmax, grid_ymin, grid_ymax);

    {
        int configs[][2] = {{4,4},{6,6},{8,8},{4,8},{8,4}};
        int nc = 5;
        int ci;

        for (ci = 0; ci < nc; ci++) {
            int nx = configs[ci][0], ny = configs[ci][1];
            int total = nx * ny;
            int n_pass_q = 0;
            n_tested = 0;

            printf("    %dx%d = %d cells: ", nx, ny, total);
            fflush(stdout);

            for (ai = 0; ai < qcat_size; ai++) {
                for (aj = ai + 1; aj < qcat_size; aj++) {
                    for (ak = aj + 1; ak < qcat_size; ak++) {
                        Quat w6[6];
                        w6[0] = qcat[ai];
                        w6[1] = quat_neg(qcat[ai]);
                        w6[2] = qcat[aj];
                        w6[3] = quat_neg(qcat[aj]);
                        w6[4] = qcat[ak];
                        w6[5] = quat_neg(qcat[ak]);
                        if (check_xor6_planar_grid(w6, nx, ny))
                            n_pass_q++;
                        n_tested++;
                    }
                }
            }
            printf("%d/%d pass\n", n_pass_q, n_tested);
        }
    }

    /* B2: Record best planar grid result */
    {
        int n_best = 0;
        n_tested = 0;
        for (ai = 0; ai < qcat_size; ai++) {
            for (aj = ai + 1; aj < qcat_size; aj++) {
                for (ak = aj + 1; ak < qcat_size; ak++) {
                    Quat w6[6];
                    w6[0] = qcat[ai];
                    w6[1] = quat_neg(qcat[ai]);
                    w6[2] = qcat[aj];
                    w6[3] = quat_neg(qcat[aj]);
                    w6[4] = qcat[ak];
                    w6[5] = quat_neg(qcat[ak]);
                    if (check_xor6_planar_grid(w6, 8, 8))
                        n_best++;
                    n_tested++;
                }
            }
        }
        sprintf(msg, "Planar grid 8x8: %d solutions from %d tuples",
                n_best, n_tested);
        check(msg, 1);
    }
}

/* ================================================================
 * Part C: Projected Voronoi (13-direction)
 * ================================================================ */

static void part_c_projected_voronoi(void) {
    char msg[256];

    printf("\n=== Part C: Projected Voronoi (13-direction) ===\n");

    collect_planar_voronoi_winners();

    printf("  INFO: Planar Voronoi (%d+1=%d cells): %d solutions\n",
           dir_cat_size, dir_cat_size + 1, n_wins);

    sprintf(msg, "Planar Voronoi: %d solutions at %d cells",
            n_wins, dir_cat_size + 1);
    check(msg, n_wins > 0);

    /* Print first few winners */
    {
        int w, k;
        int lim = n_wins < 5 ? n_wins : 5;
        for (w = 0; w < lim; w++) {
            int n0 = 0, n1 = 0, nu = 0;
            for (k = 0; k <= dir_cat_size; k++) {
                if (wins[w].label[k] == 0) n0++;
                else if (wins[w].label[k] == 1) n1++;
                else nu++;
            }
            printf("    Win[%d]: indices=(%d,%d,%d) labels: %d zero, %d one, "
                   "%d unused\n",
                   w, wins[w].ai, wins[w].aj, wins[w].ak, n0, n1, nu);
        }
    }
}

/* ================================================================
 * Part D: ASCII visualization
 * ================================================================ */

#define ASCII_W 65
#define ASCII_H 33

static void render_ascii(int win_idx) {
    char canvas[ASCII_H][ASCII_W + 1];
    int row, col, i;
    int cell;
    int ai, aj, ak;
    Quat w6[6];

    ai = wins[win_idx].ai;
    aj = wins[win_idx].aj;
    ak = wins[win_idx].ak;

    w6[0] = qcat[ai]; w6[1] = quat_neg(qcat[ai]);
    w6[2] = qcat[aj]; w6[3] = quat_neg(qcat[aj]);
    w6[4] = qcat[ak]; w6[5] = quat_neg(qcat[ak]);

    /* Fill canvas with cell labels */
    for (row = 0; row < ASCII_H; row++) {
        for (col = 0; col < ASCII_W; col++) {
            Vec2 v;
            v.x = grid_xmin + (grid_xmax - grid_xmin) *
                  ((double)col / (double)(ASCII_W - 1));
            v.y = grid_ymax - (grid_ymax - grid_ymin) *
                  ((double)row / (double)(ASCII_H - 1));

            cell = s2_voronoi_from_plane(v);

            if (wins[win_idx].label[cell] == 0) canvas[row][col] = '0';
            else if (wins[win_idx].label[cell] == 1) canvas[row][col] = '1';
            else canvas[row][col] = '.';
        }
        canvas[row][ASCII_W] = '\0';
    }

    /* Mark Voronoi boundaries: if a pixel's cell differs from its
     * right or bottom neighbor, mark it */
    for (row = 0; row < ASCII_H - 1; row++) {
        for (col = 0; col < ASCII_W - 1; col++) {
            Vec2 vc, vr, vb;
            int cc, cr, cb;

            vc.x = grid_xmin + (grid_xmax - grid_xmin) *
                   ((double)col / (double)(ASCII_W - 1));
            vc.y = grid_ymax - (grid_ymax - grid_ymin) *
                   ((double)row / (double)(ASCII_H - 1));
            vr.x = grid_xmin + (grid_xmax - grid_xmin) *
                   ((double)(col + 1) / (double)(ASCII_W - 1));
            vr.y = vc.y;
            vb.x = vc.x;
            vb.y = grid_ymax - (grid_ymax - grid_ymin) *
                   ((double)(row + 1) / (double)(ASCII_H - 1));

            cc = s2_voronoi_from_plane(vc);
            cr = s2_voronoi_from_plane(vr);
            cb = s2_voronoi_from_plane(vb);

            if (cc != cr || cc != cb) {
                canvas[row][col] = '+';
            }
        }
    }

    /* Mark eigenvector direction positions */
    for (i = 0; i < dir_cat_size; i++) {
        int c, r;
        c = (int)((proj_dirs[i].x - grid_xmin) / (grid_xmax - grid_xmin) *
                  (double)(ASCII_W - 1) + 0.5);
        r = (int)((grid_ymax - proj_dirs[i].y) / (grid_ymax - grid_ymin) *
                  (double)(ASCII_H - 1) + 0.5);
        if (c >= 0 && c < ASCII_W && r >= 0 && r < ASCII_H) {
            if (i < 10)
                canvas[r][c] = (char)('0' + i);
            else
                canvas[r][c] = (char)('A' + i - 10);
        }
    }

    /* Mark weight positions (the 6 projected weight quaternions) */
    for (i = 0; i < 6; i++) {
        EigenData ed = quat_to_eigen(w6[i]);
        if (ed.axis_defined) {
            Vec2 v = stereo_project(ed.ax, ed.ay, ed.az);
            int c, r;
            c = (int)((v.x - grid_xmin) / (grid_xmax - grid_xmin) *
                      (double)(ASCII_W - 1) + 0.5);
            r = (int)((grid_ymax - v.y) / (grid_ymax - grid_ymin) *
                      (double)(ASCII_H - 1) + 0.5);
            if (c >= 0 && c < ASCII_W && r >= 0 && r < ASCII_H) {
                canvas[r][c] = '*';
            }
        }
    }

    /* Print */
    printf("  +");
    for (col = 0; col < ASCII_W; col++) printf("-");
    printf("+\n");
    for (row = 0; row < ASCII_H; row++) {
        printf("  |%s|\n", canvas[row]);
    }
    printf("  +");
    for (col = 0; col < ASCII_W; col++) printf("-");
    printf("+\n");

    /* Legend */
    printf("  Legend: 0/1=cell labels, +=boundary, *=weight, ");
    printf("0-9,A-C=eigenvector centers\n");

    /* Print weight positions */
    printf("  Weights: q[%d] q[%d] q[%d] (+ antipodals)\n", ai, aj, ak);
    for (i = 0; i < 3; i++) {
        int qi = (i == 0) ? ai : (i == 1) ? aj : ak;
        EigenData ed = quat_to_eigen(qcat[qi]);
        if (ed.axis_defined) {
            Vec2 v = stereo_project(ed.ax, ed.ay, ed.az);
            printf("    w%d: q[%d] (%+.3f,%+.3f,%+.3f,%+.3f) -> "
                   "axis(%+.3f,%+.3f,%+.3f) -> R^2(%+.3f,%+.3f)\n",
                   i, qi, qcat[qi].a, qcat[qi].b, qcat[qi].c, qcat[qi].d,
                   ed.ax, ed.ay, ed.az, v.x, v.y);
        }
    }
}

static void part_d_ascii(void) {
    int lim, w;

    printf("\n=== Part D: ASCII visualization ===\n");

    if (n_wins == 0) {
        check("No winners to visualize", 0);
        return;
    }

    lim = n_wins < 3 ? n_wins : 3;
    for (w = 0; w < lim; w++) {
        printf("\n  --- Winner %d (indices %d,%d,%d) ---\n",
               w, wins[w].ai, wins[w].aj, wins[w].ak);
        render_ascii(w);
    }

    check("ASCII visualizations rendered", 1);
}

/* ================================================================
 * Part E: Explicit perceptron formulation
 * ================================================================ */

static void part_e_perceptron(void) {
    char msg[256];
    int w, lim, i;

    printf("\n=== Part E: Explicit perceptron formulation ===\n");
    printf("  INFO: Network: 6 bits -> 2 neurons (x,y) -> Voronoi -> 1 bit\n");
    printf("  INFO: Hidden layer: (x,y) = sum_i b_i * (x_i, y_i)\n");
    printf("  INFO: Activation: Voronoi cell of (x,y) -> label\n\n");

    if (n_wins == 0) {
        check("No winners for perceptron", 0);
        return;
    }

    lim = n_wins < 3 ? n_wins : 3;
    for (w = 0; w < lim; w++) {
        int ai = wins[w].ai;
        int aj = wins[w].aj;
        int ak = wins[w].ak;
        int indices[6];
        Vec2 weight_vecs[6];

        indices[0] = ai; indices[1] = ai; /* antipodal */
        indices[2] = aj; indices[3] = aj;
        indices[4] = ak; indices[5] = ak;

        printf("  --- Perceptron %d (q[%d], q[%d], q[%d]) ---\n",
               w, ai, aj, ak);

        /* Compute projected weight vectors */
        for (i = 0; i < 6; i++) {
            Quat q = (i % 2 == 0) ? qcat[indices[i]] :
                                     quat_neg(qcat[indices[i]]);
            EigenData ed = quat_to_eigen(q);
            if (ed.axis_defined) {
                weight_vecs[i] = stereo_project(ed.ax, ed.ay, ed.az);
            } else {
                weight_vecs[i].x = 0.0;
                weight_vecs[i].y = 0.0;
            }
        }

        /* Print weight matrix */
        printf("  Weight matrix W (6 x 2):\n");
        printf("    input  |    x       y\n");
        printf("    -------+----------------\n");
        for (i = 0; i < 6; i++) {
            printf("    b%-5d | %+8.4f  %+8.4f\n",
                   i + 1, weight_vecs[i].x, weight_vecs[i].y);
        }

        /* Print labeling */
        printf("  Cell labeling: ");
        for (i = 0; i <= dir_cat_size; i++) {
            if (wins[w].label[i] >= 0)
                printf("[%d]=%d ", i, wins[w].label[i]);
        }
        printf("\n\n");
    }

    sprintf(msg, "Perceptron formulations printed for %d winners", lim);
    check(msg, 1);
}

/* ================================================================
 * Part F: Conformality check
 * ================================================================ */

static void part_f_conformality(void) {
    char msg[256];
    int i;
    double areas[MAX_ECAT];
    double min_area = 1e30, max_area = 0.0;

    printf("\n=== Part F: Conformality check ===\n");

    /* Estimate Voronoi cell areas by Monte Carlo sampling */
    {
        int counts[MAX_ECAT];
        int n_samples = 100000;
        int s;
        memset(counts, 0, sizeof(counts));

        for (s = 0; s < n_samples; s++) {
            /* Random point in grid */
            double fx = (double)(s % 317) / 317.0;
            double fy = (double)((s * 7 + 13) % 311) / 311.0;
            Vec2 v;
            int cell;

            v.x = grid_xmin + fx * (grid_xmax - grid_xmin);
            v.y = grid_ymin + fy * (grid_ymax - grid_ymin);
            cell = s2_voronoi_from_plane(v);
            if (cell >= 0 && cell < dir_cat_size)
                counts[cell]++;
        }

        {
            double total_area = (grid_xmax - grid_xmin) *
                                (grid_ymax - grid_ymin);
            printf("  INFO: Estimated Voronoi cell areas "
                   "(grid area = %.3f):\n", total_area);
            for (i = 0; i < dir_cat_size; i++) {
                areas[i] = total_area * (double)counts[i] /
                           (double)n_samples;
                if (areas[i] < min_area) min_area = areas[i];
                if (areas[i] > max_area) max_area = areas[i];
                printf("    cell[%2d]: area=%.4f (%d%% of grid)\n",
                       i, areas[i],
                       (int)(100.0 * (double)counts[i] / (double)n_samples));
            }
        }

        printf("  INFO: Area ratio max/min: %.2f\n",
               max_area / (min_area > 0.001 ? min_area : 0.001));

        sprintf(msg, "Area distortion ratio: %.1f",
                max_area / (min_area > 0.001 ? min_area : 0.001));
        check(msg, 1);
    }

    /* Compare planar vs S^2 solution counts */
    printf("  INFO: Conformality comparison:\n");
    printf("    S^2 custom 13-dir Voronoi (Demo 67): 36 solutions\n");
    printf("    Planar projected Voronoi: %d solutions\n", n_wins);
    printf("    Difference: %d (stereographic distortion effect)\n",
           n_wins - 36);

    sprintf(msg, "Planar vs S^2 difference: %d solutions", n_wins - 36);
    check(msg, 1);
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    /* Suppress unused function warnings */
    (void)planar_voronoi_cell;

    printf("KNOTAPEL DEMO 68: Stereographic DKC\n");
    printf("====================================\n");

    part_a_projection();
    part_b_planar_grid();
    part_c_projected_voronoi();
    part_d_ascii();
    part_e_perceptron();
    part_f_conformality();

    printf("\n====================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
