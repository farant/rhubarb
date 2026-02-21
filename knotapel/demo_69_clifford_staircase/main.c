/*
 * KNOTAPEL DEMO 69: Clifford Staircase on S^2
 * =============================================
 *
 * Compare eigenvector S^2 geometry across roots of unity:
 * zeta_8 (pi/4), zeta_16 (pi/8), zeta_32 (pi/16).
 *
 * Key question: is the 13-direction eigenvector structure
 * universal or specific to zeta_8?
 *
 * Part A: zeta_16 quaternionic catalog
 * Part B: zeta_16 eigenvector extraction
 * Part C: zeta_16 S^2 activation (XOR6)
 * Part D: zeta_32 catalog + eigenvectors
 * Part E: zeta_32 S^2 activation
 * Part F: Head-to-head comparison table
 * Part G: Direction stability analysis
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

static int g_pass = 0, g_fail = 0;

static void check(const char *titulis, int ok) {
    if (ok) { printf("  PASS: %s\n", titulis); g_pass++; }
    else    { printf("  FAIL: %s\n", titulis); g_fail++; }
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
    return quat_make(p.a+q.a, p.b+q.b, p.c+q.c, p.d+q.d);
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
    return quat_make(q.a/n, q.b/n, q.c/n, q.d/n);
}

/* ================================================================
 * Eigenvector extraction
 * ================================================================ */

typedef struct {
    double theta;
    double ax, ay, az;
    int axis_defined;
} EigenData;

static EigenData quat_to_eigen(Quat q) {
    EigenData e;
    double norm_v;
    if (q.a < 0.0) { q.a = -q.a; q.b = -q.b; q.c = -q.c; q.d = -q.d; }
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

/* ================================================================
 * Root-specific data structure
 * ================================================================ */

#define MAX_QCAT 32768
#define MAX_ECAT 4096
#define MAX_WORD 64
#define XOR6_CAP 150
#define MAX_ANGLES 64

typedef struct {
    Quat quats[MAX_QCAT];
    int n_quats;
    int n_braids;
    EigenData eigen[MAX_QCAT];
    double dirs[MAX_ECAT][3];
    int n_dirs;
    double theta;
    const char *label;
    int N;
    int voronoi_solutions;
} RootData;

static RootData root_8, root_16, root_32;

/* ================================================================
 * Braid representation (parameterized by generators)
 * ================================================================ */

static Quat g_gen1, g_gen2;

static void set_generators(double theta) {
    double c = cos(theta), s = sin(theta);
    g_gen1 = quat_make(c, s, 0, 0);
    g_gen2 = quat_make(c, 0, 0, -s);
}

static Quat braid_quat(const int *word, int len) {
    Quat r = quat_one();
    int l;
    for (l = 0; l < len; l++) {
        int g = word[l];
        Quat gen;
        if (g == 1)       gen = g_gen1;
        else if (g == 2)  gen = g_gen2;
        else if (g == -1) gen = quat_conj(g_gen1);
        else              gen = quat_conj(g_gen2);
        r = quat_mul(r, gen);
    }
    return r;
}

/* ================================================================
 * Catalog building
 * ================================================================ */

static int find_quat_in(const Quat *arr, int sz, Quat q) {
    int i;
    for (i = 0; i < sz; i++) {
        double d1 = fabs(arr[i].a-q.a) + fabs(arr[i].b-q.b) +
                     fabs(arr[i].c-q.c) + fabs(arr[i].d-q.d);
        double d2 = fabs(arr[i].a+q.a) + fabs(arr[i].b+q.b) +
                     fabs(arr[i].c+q.c) + fabs(arr[i].d+q.d);
        if (d1 < 1e-10 || d2 < 1e-10) return i;
    }
    return -1;
}

static void build_catalog(RootData *rd) {
    int n, len, word[MAX_WORD];
    set_generators(rd->theta);
    rd->n_quats = 0;
    rd->n_braids = 0;

    for (n = 2; n <= 3; n++) {
        int mg = n - 1;
        int tg = 2 * mg;
        for (len = 1; len <= 10; len++) {
            unsigned long total = 1, idx;
            int i;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)tg;
                if (total > 500000) break;
            }
            if (total > 500000) continue;
            for (idx = 0; idx < total; idx++) {
                unsigned long tmp = idx;
                Quat q;
                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)tg);
                    tmp /= (unsigned long)tg;
                    if (g < mg) word[i] = g + 1;
                    else        word[i] = -(g - mg + 1);
                }
                q = braid_quat(word, len);
                rd->n_braids++;
                if (find_quat_in(rd->quats, rd->n_quats, q) < 0 &&
                    rd->n_quats < MAX_QCAT) {
                    rd->quats[rd->n_quats] = q;
                    rd->n_quats++;
                }
            }
        }
    }
}

/* ================================================================
 * Eigenvector catalog
 * ================================================================ */

static int find_dir_in(const double dirs[][3], int sz,
                        double ax, double ay, double az) {
    int i;
    for (i = 0; i < sz; i++) {
        double d1 = fabs(dirs[i][0]-ax) + fabs(dirs[i][1]-ay) +
                     fabs(dirs[i][2]-az);
        double d2 = fabs(dirs[i][0]+ax) + fabs(dirs[i][1]+ay) +
                     fabs(dirs[i][2]+az);
        if (d1 < 1e-8 || d2 < 1e-8) return i;
    }
    return -1;
}

static void build_eigen(RootData *rd) {
    int i;
    rd->n_dirs = 0;
    for (i = 0; i < rd->n_quats; i++) {
        rd->eigen[i] = quat_to_eigen(rd->quats[i]);
        if (rd->eigen[i].axis_defined) {
            if (find_dir_in(rd->dirs, rd->n_dirs,
                            rd->eigen[i].ax, rd->eigen[i].ay,
                            rd->eigen[i].az) < 0 &&
                rd->n_dirs < MAX_ECAT) {
                rd->dirs[rd->n_dirs][0] = rd->eigen[i].ax;
                rd->dirs[rd->n_dirs][1] = rd->eigen[i].ay;
                rd->dirs[rd->n_dirs][2] = rd->eigen[i].az;
                rd->n_dirs++;
            }
        }
    }
}

/* ================================================================
 * S^2 Voronoi cell assignment
 * ================================================================ */

static int s2_voronoi_rd(double ax, double ay, double az,
                          const RootData *rd) {
    int i, best = 0;
    double best_d = -2.0, d;
    for (i = 0; i < rd->n_dirs; i++) {
        d = fabs(ax*rd->dirs[i][0] + ay*rd->dirs[i][1] + az*rd->dirs[i][2]);
        if (d > best_d) { best_d = d; best = i; }
    }
    return best;
}

/* ================================================================
 * S^2 lat/lon grid cell assignment
 * ================================================================ */

static int s2_latlon(double ax, double ay, double az, int nlat, int nlon) {
    double colat, lon, caz = az;
    int il, io;
    if (caz < -1.0) caz = -1.0;
    if (caz > 1.0) caz = 1.0;
    colat = acos(caz);
    lon = atan2(ay, ax);
    if (lon < 0.0) lon += 2.0 * M_PI;
    il = (int)(colat / M_PI * (double)nlat);
    if (il >= nlat) il = nlat - 1;
    io = (int)(lon / (2.0 * M_PI) * (double)nlon);
    if (io >= nlon) io = nlon - 1;
    return il * nlon + io;
}

/* ================================================================
 * XOR6 checkers
 * ================================================================ */

static int check_xor6_voronoi(const Quat *w, const RootData *rd) {
    int nc = rd->n_dirs;
    int seen[MAX_ECAT + 1];
    int mask, i, cell;
    for (i = 0; i <= nc; i++) seen[i] = 0;
    for (mask = 0; mask < 64; mask++) {
        Quat sum = quat_make(0, 0, 0, 0);
        int par = 0;
        EigenData ed;
        for (i = 0; i < 6; i++) {
            if (mask & (1 << i)) { sum = quat_add(sum, w[i]); par ^= 1; }
        }
        if (quat_norm(sum) < 1e-12) {
            cell = nc;
        } else {
            sum = quat_normalize(sum);
            ed = quat_to_eigen(sum);
            cell = ed.axis_defined ? s2_voronoi_rd(ed.ax,ed.ay,ed.az,rd) : nc;
        }
        seen[cell] |= (1 << par);
        if (seen[cell] == 3) return 0;
    }
    return 1;
}

static int check_xor6_ll(const Quat *w, int nlat, int nlon) {
    int tc = nlat * nlon;
    int seen[257];
    int mask, i, cell;
    if (tc > 256) return 0;
    for (i = 0; i <= tc; i++) seen[i] = 0;
    for (mask = 0; mask < 64; mask++) {
        Quat sum = quat_make(0, 0, 0, 0);
        int par = 0;
        EigenData ed;
        for (i = 0; i < 6; i++) {
            if (mask & (1 << i)) { sum = quat_add(sum, w[i]); par ^= 1; }
        }
        if (quat_norm(sum) < 1e-12) {
            cell = tc;
        } else {
            sum = quat_normalize(sum);
            ed = quat_to_eigen(sum);
            cell = ed.axis_defined ? s2_latlon(ed.ax,ed.ay,ed.az,nlat,nlon) : tc;
        }
        seen[cell] |= (1 << par);
        if (seen[cell] == 3) return 0;
    }
    return 1;
}

/* ================================================================
 * Solution counters
 * ================================================================ */

static int count_voronoi(const RootData *rd) {
    int ai, aj, ak, count = 0;
    int n = rd->n_quats < XOR6_CAP ? rd->n_quats : XOR6_CAP;
    long nt = (long)n * (long)(n-1) * (long)(n-2) / 6;
    if (rd->n_quats > XOR6_CAP)
        printf("  INFO: %s has %d quats, capping at %d for XOR6\n",
               rd->label, rd->n_quats, XOR6_CAP);
    printf("  INFO: %s Voronoi (%d+1 cells), testing %ld triples...",
           rd->label, rd->n_dirs, nt);
    fflush(stdout);
    for (ai = 0; ai < n; ai++)
        for (aj = ai+1; aj < n; aj++)
            for (ak = aj+1; ak < n; ak++) {
                Quat w[6];
                w[0]=rd->quats[ai]; w[1]=quat_neg(rd->quats[ai]);
                w[2]=rd->quats[aj]; w[3]=quat_neg(rd->quats[aj]);
                w[4]=rd->quats[ak]; w[5]=quat_neg(rd->quats[ak]);
                if (check_xor6_voronoi(w, rd)) count++;
            }
    printf(" %d solutions\n", count);
    return count;
}

static int count_ll(const RootData *rd, int nlat, int nlon) {
    int ai, aj, ak, count = 0;
    int n = rd->n_quats < XOR6_CAP ? rd->n_quats : XOR6_CAP;
    for (ai = 0; ai < n; ai++)
        for (aj = ai+1; aj < n; aj++)
            for (ak = aj+1; ak < n; ak++) {
                Quat w[6];
                w[0]=rd->quats[ai]; w[1]=quat_neg(rd->quats[ai]);
                w[2]=rd->quats[aj]; w[3]=quat_neg(rd->quats[aj]);
                w[4]=rd->quats[ak]; w[5]=quat_neg(rd->quats[ak]);
                if (check_xor6_ll(w, nlat, nlon)) count++;
            }
    return count;
}

/* ================================================================
 * Analysis helpers
 * ================================================================ */

static int cmp_double(const void *a, const void *b) {
    double da = *(const double *)a, db = *(const double *)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

static int count_angles(const RootData *rd, double *out) {
    int na = 0, i, j, found;
    for (i = 0; i < rd->n_quats; i++) {
        found = 0;
        for (j = 0; j < na; j++) {
            if (fabs(rd->eigen[i].theta - out[j]) < 1e-8) { found=1; break; }
        }
        if (!found && na < MAX_ANGLES) { out[na++] = rd->eigen[i].theta; }
    }
    qsort(out, (size_t)na, sizeof(double), cmp_double);
    return na;
}

static int dir_overlap(const RootData *r1, const RootData *r2) {
    int i, count = 0;
    for (i = 0; i < r1->n_dirs; i++) {
        if (find_dir_in(r2->dirs, r2->n_dirs,
                        r1->dirs[i][0], r1->dirs[i][1], r1->dirs[i][2]) >= 0)
            count++;
    }
    return count;
}

static double nearest_dir_deg(const double dir[3],
                               const double dirs[][3], int nd) {
    int i;
    double best = 0.0, d;
    for (i = 0; i < nd; i++) {
        d = fabs(dir[0]*dirs[i][0] + dir[1]*dirs[i][1] + dir[2]*dirs[i][2]);
        if (d > best) best = d;
    }
    if (best > 1.0) best = 1.0;
    return acos(best) * 180.0 / M_PI;
}

/* ================================================================
 * Part A: zeta_16 quaternionic catalog
 * ================================================================ */

static void part_a(void) {
    char msg[256];

    printf("\n=== Part A: zeta_16 quaternionic catalog ===\n");
    printf("  INFO: Generator angle: pi/8 = %.6f rad = %.2f deg\n",
           root_16.theta, root_16.theta * 180.0 / M_PI);
    printf("  INFO: sigma_1 = (%.6f, %.6f, 0, 0)\n",
           cos(root_16.theta), sin(root_16.theta));
    printf("  INFO: sigma_2 = (%.6f, 0, 0, %.6f)\n",
           cos(root_16.theta), -sin(root_16.theta));
    printf("  INFO: Braids enumerated: %d\n", root_16.n_braids);
    printf("  INFO: Distinct quaternions (mod +-): %d\n", root_16.n_quats);
    printf("  INFO: (zeta_8 reference: %d quaternions)\n", root_8.n_quats);

    sprintf(msg, "zeta_16: %d quaternions from %d braids",
            root_16.n_quats, root_16.n_braids);
    check(msg, root_16.n_quats > 0);
}

/* ================================================================
 * Part B: zeta_16 eigenvector extraction
 * ================================================================ */

static void part_b(void) {
    char msg[256];
    double angles_16[MAX_ANGLES], angles_8[MAX_ANGLES];
    int na_16, na_8, i, ov;

    printf("\n=== Part B: zeta_16 eigenvector extraction ===\n");

    na_16 = count_angles(&root_16, angles_16);
    na_8 = count_angles(&root_8, angles_8);

    printf("  INFO: zeta_16 eigenvalue angles (%d):", na_16);
    for (i = 0; i < na_16; i++) printf(" %.2f", angles_16[i] * 180.0 / M_PI);
    printf("\n");
    printf("  INFO: zeta_8 eigenvalue angles (%d):", na_8);
    for (i = 0; i < na_8; i++) printf(" %.2f", angles_8[i] * 180.0 / M_PI);
    printf("\n");

    printf("  INFO: zeta_16 eigenvector directions: %d\n", root_16.n_dirs);
    printf("  INFO: (zeta_8 reference: %d directions)\n", root_8.n_dirs);

    ov = dir_overlap(&root_8, &root_16);
    printf("  INFO: zeta_8 dirs in zeta_16: %d/%d\n", ov, root_8.n_dirs);
    ov = dir_overlap(&root_16, &root_8);
    printf("  INFO: zeta_16 dirs in zeta_8: %d/%d\n", ov, root_16.n_dirs);

    /* Print first directions */
    {
        int lim = root_16.n_dirs < 20 ? root_16.n_dirs : 20;
        printf("  INFO: First %d zeta_16 directions:\n", lim);
        for (i = 0; i < lim; i++) {
            int in8 = find_dir_in(root_8.dirs, root_8.n_dirs,
                                   root_16.dirs[i][0], root_16.dirs[i][1],
                                   root_16.dirs[i][2]);
            printf("    [%2d] (%+.4f,%+.4f,%+.4f)%s\n",
                   i, root_16.dirs[i][0], root_16.dirs[i][1],
                   root_16.dirs[i][2],
                   in8 >= 0 ? " <-- shared with zeta_8" : "");
        }
    }

    sprintf(msg, "zeta_16: %d angles, %d directions", na_16, root_16.n_dirs);
    check(msg, root_16.n_dirs > 0);
}

/* ================================================================
 * Part C: zeta_16 S^2 activation
 * ================================================================ */

static void part_c(void) {
    char msg[256];
    int ll44, ll66, ll88;

    printf("\n=== Part C: zeta_16 S^2 activation ===\n");

    root_16.voronoi_solutions = count_voronoi(&root_16);

    sprintf(msg, "zeta_16 Voronoi: %d solutions at %d cells",
            root_16.voronoi_solutions, root_16.n_dirs + 1);
    check(msg, 1);

    printf("  INFO: Lat/lon grid search...\n");
    ll44 = count_ll(&root_16, 4, 4);
    ll66 = count_ll(&root_16, 6, 6);
    ll88 = count_ll(&root_16, 8, 8);
    printf("    4x4 (16 cells): %d\n", ll44);
    printf("    6x6 (36 cells): %d\n", ll66);
    printf("    8x8 (64 cells): %d\n", ll88);

    sprintf(msg, "zeta_16 lat/lon 8x8: %d solutions", ll88);
    check(msg, 1);
}

/* ================================================================
 * Part D: zeta_32 catalog + eigenvectors
 * ================================================================ */

static void part_d(void) {
    char msg[256];
    double angles_32[MAX_ANGLES];
    int na_32, i, ov;

    printf("\n=== Part D: zeta_32 catalog + eigenvectors ===\n");
    printf("  INFO: Generator angle: pi/16 = %.6f rad = %.2f deg\n",
           root_32.theta, root_32.theta * 180.0 / M_PI);
    printf("  INFO: Braids enumerated: %d\n", root_32.n_braids);
    printf("  INFO: Distinct quaternions (mod +-): %d\n", root_32.n_quats);

    na_32 = count_angles(&root_32, angles_32);
    printf("  INFO: zeta_32 eigenvalue angles (%d):", na_32);
    for (i = 0; i < na_32 && i < 20; i++)
        printf(" %.2f", angles_32[i] * 180.0 / M_PI);
    if (na_32 > 20) printf(" ...");
    printf("\n");

    printf("  INFO: zeta_32 eigenvector directions: %d\n", root_32.n_dirs);

    ov = dir_overlap(&root_8, &root_32);
    printf("  INFO: zeta_8 dirs in zeta_32: %d/%d\n", ov, root_8.n_dirs);
    ov = dir_overlap(&root_16, &root_32);
    printf("  INFO: zeta_16 dirs in zeta_32: %d/%d\n", ov, root_16.n_dirs);
    ov = dir_overlap(&root_32, &root_8);
    printf("  INFO: zeta_32 dirs in zeta_8: %d/%d\n", ov, root_32.n_dirs);

    /* Print first directions */
    {
        int lim = root_32.n_dirs < 20 ? root_32.n_dirs : 20;
        printf("  INFO: First %d zeta_32 directions:\n", lim);
        for (i = 0; i < lim; i++) {
            int in8 = find_dir_in(root_8.dirs, root_8.n_dirs,
                                   root_32.dirs[i][0], root_32.dirs[i][1],
                                   root_32.dirs[i][2]);
            int in16 = find_dir_in(root_16.dirs, root_16.n_dirs,
                                    root_32.dirs[i][0], root_32.dirs[i][1],
                                    root_32.dirs[i][2]);
            printf("    [%2d] (%+.4f,%+.4f,%+.4f)", i,
                   root_32.dirs[i][0], root_32.dirs[i][1], root_32.dirs[i][2]);
            if (in8 >= 0) printf(" [z8]");
            if (in16 >= 0) printf(" [z16]");
            printf("\n");
        }
    }

    sprintf(msg, "zeta_32: %d quats, %d angles, %d directions",
            root_32.n_quats, na_32, root_32.n_dirs);
    check(msg, root_32.n_dirs > 0);
}

/* ================================================================
 * Part E: zeta_32 S^2 activation
 * ================================================================ */

static void part_e(void) {
    char msg[256];
    int ll44, ll66, ll88;

    printf("\n=== Part E: zeta_32 S^2 activation ===\n");

    root_32.voronoi_solutions = count_voronoi(&root_32);

    sprintf(msg, "zeta_32 Voronoi: %d solutions at %d cells",
            root_32.voronoi_solutions, root_32.n_dirs + 1);
    check(msg, 1);

    printf("  INFO: Lat/lon grid search...\n");
    ll44 = count_ll(&root_32, 4, 4);
    ll66 = count_ll(&root_32, 6, 6);
    ll88 = count_ll(&root_32, 8, 8);
    printf("    4x4 (16 cells): %d\n", ll44);
    printf("    6x6 (36 cells): %d\n", ll66);
    printf("    8x8 (64 cells): %d\n", ll88);

    sprintf(msg, "zeta_32 lat/lon 8x8: %d solutions", ll88);
    check(msg, 1);
}

/* ================================================================
 * Part F: Head-to-head comparison table
 * ================================================================ */

static void part_f(void) {
    char msg[256];
    double angles_8[MAX_ANGLES], angles_16[MAX_ANGLES], angles_32[MAX_ANGLES];
    int na8, na16, na32;
    double sc8, sc16, sc32;

    printf("\n=== Part F: Head-to-head comparison ===\n");

    /* Compute zeta_8 solutions */
    root_8.voronoi_solutions = count_voronoi(&root_8);

    na8 = count_angles(&root_8, angles_8);
    na16 = count_angles(&root_16, angles_16);
    na32 = count_angles(&root_32, angles_32);

    sc8 = root_8.n_dirs > 0 ?
        (double)root_8.voronoi_solutions / (double)(root_8.n_dirs + 1) : 0.0;
    sc16 = root_16.n_dirs > 0 ?
        (double)root_16.voronoi_solutions / (double)(root_16.n_dirs + 1) : 0.0;
    sc32 = root_32.n_dirs > 0 ?
        (double)root_32.voronoi_solutions / (double)(root_32.n_dirs + 1) : 0.0;

    printf("\n  | Root    | Quats | Angles | Dirs | Cells | XOR6 | Sol/Cell |\n");
    printf("  |---------|-------|--------|------|-------|------|---------|\n");
    printf("  | zeta_8  | %5d | %6d | %4d | %5d | %4d | %7.2f |\n",
           root_8.n_quats, na8, root_8.n_dirs,
           root_8.n_dirs + 1, root_8.voronoi_solutions, sc8);
    printf("  | zeta_16 | %5d | %6d | %4d | %5d | %4d | %7.2f |\n",
           root_16.n_quats, na16, root_16.n_dirs,
           root_16.n_dirs + 1, root_16.voronoi_solutions, sc16);
    printf("  | zeta_32 | %5d | %6d | %4d | %5d | %4d | %7.2f |\n",
           root_32.n_quats, na32, root_32.n_dirs,
           root_32.n_dirs + 1, root_32.voronoi_solutions, sc32);

    printf("\n  Direction overlap matrix:\n");
    printf("  | From\\To  | zeta_8 | zeta_16 | zeta_32 |\n");
    printf("  |----------|--------|---------|--------|\n");
    printf("  | zeta_8   | %2d/%2d  | %2d/%2d   | %2d/%2d  |\n",
           root_8.n_dirs, root_8.n_dirs,
           dir_overlap(&root_8, &root_16), root_8.n_dirs,
           dir_overlap(&root_8, &root_32), root_8.n_dirs);
    printf("  | zeta_16  | %2d/%2d  | %2d/%2d   | %2d/%2d  |\n",
           dir_overlap(&root_16, &root_8), root_16.n_dirs,
           root_16.n_dirs, root_16.n_dirs,
           dir_overlap(&root_16, &root_32), root_16.n_dirs);
    printf("  | zeta_32  | %2d/%2d  | %2d/%2d   | %2d/%2d  |\n",
           dir_overlap(&root_32, &root_8), root_32.n_dirs,
           dir_overlap(&root_32, &root_16), root_32.n_dirs,
           root_32.n_dirs, root_32.n_dirs);

    sprintf(msg, "Comparison: z8=%d, z16=%d, z32=%d solutions",
            root_8.voronoi_solutions, root_16.voronoi_solutions,
            root_32.voronoi_solutions);
    check(msg, 1);
}

/* ================================================================
 * Part G: Direction stability analysis
 * ================================================================ */

static void part_g(void) {
    char msg[256];
    int i;
    double sum_16 = 0.0, sum_32 = 0.0;
    double max_16 = 0.0, max_32 = 0.0;
    double a16, a32;
    const char *class_16;
    const char *class_32;
    int novel_16 = 0, novel_32 = 0;

    printf("\n=== Part G: Direction stability analysis ===\n");
    printf("  zeta_8 directions -> nearest in zeta_16 and zeta_32:\n\n");
    printf("  | Dir | S^2 coord              | -> z16 (deg)  | -> z32 (deg)  |\n");
    printf("  |-----|------------------------|---------------|---------------|\n");

    for (i = 0; i < root_8.n_dirs; i++) {
        a16 = nearest_dir_deg(root_8.dirs[i], root_16.dirs, root_16.n_dirs);
        a32 = nearest_dir_deg(root_8.dirs[i], root_32.dirs, root_32.n_dirs);
        printf("  | %3d | (%+.3f,%+.3f,%+.3f) | %13.4f | %13.4f |\n",
               i, root_8.dirs[i][0], root_8.dirs[i][1], root_8.dirs[i][2],
               a16, a32);
        sum_16 += a16;
        sum_32 += a32;
        if (a16 > max_16) max_16 = a16;
        if (a32 > max_32) max_32 = a32;
    }

    printf("\n  Summary:\n");
    printf("    zeta_8 -> zeta_16: avg=%.4f deg, max=%.4f deg\n",
           sum_16 / (double)root_8.n_dirs, max_16);
    printf("    zeta_8 -> zeta_32: avg=%.4f deg, max=%.4f deg\n",
           sum_32 / (double)root_8.n_dirs, max_32);

    /* Classification */
    if (max_16 < 0.01)      class_16 = "RIGIDITY (identical directions)";
    else if (max_16 < 5.0)  class_16 = "DRIFT (small angular shifts)";
    else                     class_16 = "INDEPENDENCE (different geometry)";

    if (max_32 < 0.01)      class_32 = "RIGIDITY (identical directions)";
    else if (max_32 < 5.0)  class_32 = "DRIFT (small angular shifts)";
    else                     class_32 = "INDEPENDENCE (different geometry)";

    printf("\n  Classification:\n");
    printf("    zeta_8 -> zeta_16: %s\n", class_16);
    printf("    zeta_8 -> zeta_32: %s\n", class_32);

    /* Novel directions */
    for (i = 0; i < root_16.n_dirs; i++) {
        if (find_dir_in(root_8.dirs, root_8.n_dirs,
                        root_16.dirs[i][0], root_16.dirs[i][1],
                        root_16.dirs[i][2]) < 0)
            novel_16++;
    }
    for (i = 0; i < root_32.n_dirs; i++) {
        if (find_dir_in(root_8.dirs, root_8.n_dirs,
                        root_32.dirs[i][0], root_32.dirs[i][1],
                        root_32.dirs[i][2]) < 0)
            novel_32++;
    }
    printf("    zeta_16 novel directions (not in zeta_8): %d/%d\n",
           novel_16, root_16.n_dirs);
    printf("    zeta_32 novel directions (not in zeta_8): %d/%d\n",
           novel_32, root_32.n_dirs);

    sprintf(msg, "Stability: avg_16=%.4f, avg_32=%.4f deg",
            sum_16 / (double)root_8.n_dirs, sum_32 / (double)root_8.n_dirs);
    check(msg, 1);
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 69: Clifford Staircase on S^2\n");
    printf("=============================================\n");

    /* Build all catalogs up front */
    printf("\n  Building catalogs...\n");

    root_8.theta = M_PI / 4.0;
    root_8.label = "zeta_8";
    root_8.N = 8;
    build_catalog(&root_8);
    build_eigen(&root_8);
    printf("    zeta_8:  %d quats, %d dirs (%d braids)\n",
           root_8.n_quats, root_8.n_dirs, root_8.n_braids);

    root_16.theta = M_PI / 8.0;
    root_16.label = "zeta_16";
    root_16.N = 16;
    build_catalog(&root_16);
    build_eigen(&root_16);
    printf("    zeta_16: %d quats, %d dirs (%d braids)\n",
           root_16.n_quats, root_16.n_dirs, root_16.n_braids);

    root_32.theta = M_PI / 16.0;
    root_32.label = "zeta_32";
    root_32.N = 32;
    build_catalog(&root_32);
    build_eigen(&root_32);
    printf("    zeta_32: %d quats, %d dirs (%d braids)\n",
           root_32.n_quats, root_32.n_dirs, root_32.n_braids);

    part_a();
    part_b();
    part_c();
    part_d();
    part_e();
    part_f();
    part_g();

    printf("\n=============================================\n");
    printf("Results: %d pass, %d fail\n", g_pass, g_fail);
    return g_fail > 0 ? 1 : 0;
}
