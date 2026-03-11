/*
 * Demo 004: Non-Inertness of the 16-Point Bootstrap
 *
 * Starting from the 16 integer points produced by the bootstrap (Prop III.4),
 * we systematically enumerate all line-plane intersections using only
 * rational operations (T_Q) and show that new points emerge.
 *
 * This proves that the 16-point set is NOT inert -- unlike the original
 * 4 points which were inert under T_Q (Theorem II.4).
 *
 * Key questions:
 *   1. How many new points emerge from one generation?
 *   2. What denominators appear? Does 1/3 appear?
 *   3. What is the structure of the new point set?
 */

#include <stdio.h>

/* ---- types ---- */

typedef struct { long x, y, z; } Vec3;
typedef struct { long p, q; } Rat;
typedef struct { Rat x, y, z; } RVec3;

/* Normalized plane: nx*X + ny*Y + nz*Z = d
 * with gcd(|nx|,|ny|,|nz|) = 1, first nonzero component positive.
 * d is integer (since normal and points are all integer). */
typedef struct { long nx, ny, nz, d; } NPlane;

/* ---- integer vector ops ---- */

static Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    Vec3 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

static long vec3_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    Vec3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

/* ---- rational arithmetic ---- */

static long gcd_long(long a, long b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { long t = b; b = a % b; a = t; }
    return a;
}

static Rat rat_make(long p, long q)
{
    Rat r;
    long g;
    if (q < 0) { p = -p; q = -q; }
    if (p == 0) { r.p = 0; r.q = 1; return r; }
    g = gcd_long(p, q);
    r.p = p / g;
    r.q = q / g;
    return r;
}

static int rat_eq(Rat a, Rat b)
{
    return a.p == b.p && a.q == b.q;
}

static void rat_print(Rat r)
{
    if (r.q == 1)
        printf("%ld", r.p);
    else
        printf("%ld/%ld", r.p, r.q);
}

static void rvec3_print(RVec3 v)
{
    printf("(");
    rat_print(v.x);
    printf(", ");
    rat_print(v.y);
    printf(", ");
    rat_print(v.z);
    printf(")");
}

static int rvec3_eq(RVec3 a, RVec3 b)
{
    return rat_eq(a.x, b.x) && rat_eq(a.y, b.y) && rat_eq(a.z, b.z);
}

/* ---- plane normalization ---- */

static long gcd3(long a, long b, long c)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    if (c < 0) c = -c;
    return gcd_long(gcd_long(a, b), c);
}

/*
 * Compute normalized plane through 3 integer points.
 * Returns 0 if collinear.
 */
static int make_nplane(Vec3 P, Vec3 Q, Vec3 R, NPlane *out)
{
    Vec3 n = vec3_cross(vec3_sub(Q, P), vec3_sub(R, P));
    long g;

    if (n.x == 0 && n.y == 0 && n.z == 0) return 0;

    g = gcd3(n.x, n.y, n.z);
    n.x /= g;
    n.y /= g;
    n.z /= g;

    /* first nonzero component positive */
    if (n.x < 0 || (n.x == 0 && n.y < 0) ||
        (n.x == 0 && n.y == 0 && n.z < 0)) {
        n.x = -n.x;
        n.y = -n.y;
        n.z = -n.z;
    }

    out->nx = n.x;
    out->ny = n.y;
    out->nz = n.z;
    out->d = vec3_dot(n, P);
    return 1;
}

static int nplane_eq(NPlane a, NPlane b)
{
    return a.nx == b.nx && a.ny == b.ny && a.nz == b.nz && a.d == b.d;
}

/* ---- line-plane intersection ---- */

/*
 * Line through integer points P, Q.
 * Plane nx*X + ny*Y + nz*Z = d (all integer).
 * Returns 0 if parallel.
 */
static int line_nplane_intersect(Vec3 P, Vec3 Q, NPlane pl, RVec3 *out)
{
    Vec3 dir = vec3_sub(Q, P);
    Vec3 n;
    long denom, numer;
    Rat t;

    n.x = pl.nx;
    n.y = pl.ny;
    n.z = pl.nz;
    denom = vec3_dot(n, dir);
    if (denom == 0) return 0;

    numer = pl.d - vec3_dot(n, P);
    t = rat_make(numer, denom);

    out->x = rat_make(P.x * t.q + t.p * dir.x, t.q);
    out->y = rat_make(P.y * t.q + t.p * dir.y, t.q);
    out->z = rat_make(P.z * t.q + t.p * dir.z, t.q);
    return 1;
}

/* ---- point set management ---- */

#define MAX_NEW 16384
#define MAX_PLANES 2048

static RVec3 new_pts[MAX_NEW];
static int n_new = 0;

static int is_known(RVec3 p, Vec3 *orig, int n_orig)
{
    int i;
    RVec3 o;

    for (i = 0; i < n_orig; i++) {
        o.x = rat_make(orig[i].x, 1);
        o.y = rat_make(orig[i].y, 1);
        o.z = rat_make(orig[i].z, 1);
        if (rvec3_eq(p, o)) return 1;
    }
    for (i = 0; i < n_new; i++) {
        if (rvec3_eq(p, new_pts[i])) return 1;
    }
    return 0;
}

static void add_if_new(RVec3 p, Vec3 *orig, int n_orig)
{
    if (is_known(p, orig, n_orig)) return;
    if (n_new < MAX_NEW) {
        new_pts[n_new++] = p;
    }
}

/* ---- test framework ---- */

static int tests_run = 0;
static int tests_passed = 0;

static void check(const char *desc, int cond)
{
    tests_run++;
    if (cond) {
        tests_passed++;
        printf("  PASS: %s\n", desc);
    } else {
        printf("  FAIL: %s\n", desc);
    }
}

/* ---- main ---- */

int main(void)
{
    Vec3 pts[16];
    int npts = 0;

    NPlane planes[MAX_PLANES];
    int nplanes = 0;

    int i, j, k;
    int collinear_triples = 0;
    int intersections_computed = 0;
    int parallel_skipped = 0;

    /* denominator histogram */
    long max_denom = 0;
    int has_third = 0;

    printf("=== Demo 004: Non-Inertness of the 16-Point Bootstrap ===\n\n");

    /* ---- The 16 bootstrap points ---- */

    /* Original 4 */
    pts[npts].x = 0; pts[npts].y = 0; pts[npts].z = 0; npts++;
    pts[npts].x = 1; pts[npts].y = 0; pts[npts].z = 0; npts++;
    pts[npts].x = 0; pts[npts].y = 1; pts[npts].z = 0; npts++;
    pts[npts].x = 0; pts[npts].y = 0; pts[npts].z = 1; npts++;

    /* 6 axis reflections */
    pts[npts].x = -1; pts[npts].y =  0; pts[npts].z =  0; npts++;
    pts[npts].x =  2; pts[npts].y =  0; pts[npts].z =  0; npts++;
    pts[npts].x =  0; pts[npts].y = -1; pts[npts].z =  0; npts++;
    pts[npts].x =  0; pts[npts].y =  2; pts[npts].z =  0; npts++;
    pts[npts].x =  0; pts[npts].y =  0; pts[npts].z = -1; npts++;
    pts[npts].x =  0; pts[npts].y =  0; pts[npts].z =  2; npts++;

    /* 6 vertex reflections */
    pts[npts].x =  2; pts[npts].y = -1; pts[npts].z =  0; npts++;
    pts[npts].x = -1; pts[npts].y =  2; pts[npts].z =  0; npts++;
    pts[npts].x =  2; pts[npts].y =  0; pts[npts].z = -1; npts++;
    pts[npts].x = -1; pts[npts].y =  0; pts[npts].z =  2; npts++;
    pts[npts].x =  0; pts[npts].y =  2; pts[npts].z = -1; npts++;
    pts[npts].x =  0; pts[npts].y = -1; pts[npts].z =  2; npts++;

    printf("Starting with %d bootstrap points.\n\n", npts);

    /* ---- Step 1: Enumerate distinct planes ---- */
    printf("--- Step 1: Enumerating distinct planes ---\n");

    for (i = 0; i < npts; i++) {
        for (j = i + 1; j < npts; j++) {
            for (k = j + 1; k < npts; k++) {
                NPlane pl;
                int dup, m;
                if (!make_nplane(pts[i], pts[j], pts[k], &pl)) {
                    collinear_triples++;
                    continue;
                }
                dup = 0;
                for (m = 0; m < nplanes; m++) {
                    if (nplane_eq(planes[m], pl)) {
                        dup = 1;
                        break;
                    }
                }
                if (!dup) {
                    if (nplanes < MAX_PLANES) {
                        planes[nplanes++] = pl;
                    } else {
                        printf("WARNING: plane buffer full!\n");
                    }
                }
            }
        }
    }

    printf("  Total triples C(16,3) = %d\n", 16 * 15 * 14 / 6);
    printf("  Collinear triples (degenerate): %d\n", collinear_triples);
    printf("  Distinct planes: %d\n\n", nplanes);

    check("16 points generate more than 4 planes",
          nplanes > 4);

    /* ---- Step 2: Compute all line-plane intersections ---- */
    printf("\n--- Step 2: Computing line-plane intersections ---\n");
    printf("  Lines: C(16,2) = %d\n", 16 * 15 / 2);
    printf("  Intersections to check: %d x %d = %d\n",
           120, nplanes, 120 * nplanes);

    for (i = 0; i < npts; i++) {
        for (j = i + 1; j < npts; j++) {
            for (k = 0; k < nplanes; k++) {
                RVec3 hit;
                if (!line_nplane_intersect(pts[i], pts[j], planes[k], &hit)) {
                    parallel_skipped++;
                    continue;
                }
                intersections_computed++;
                add_if_new(hit, pts, npts);
            }
        }
    }

    printf("  Parallel (skipped): %d\n", parallel_skipped);
    printf("  Intersections computed: %d\n", intersections_computed);
    printf("  NEW distinct points found: %d\n\n", n_new);

    check("16-point bootstrap is NOT inert (new points found)",
          n_new > 0);

    /* ---- Step 3: Analysis of new points ---- */
    printf("--- Step 3: Analysis of new points ---\n\n");

    /* Denominator analysis */
    {
        int denom_counts[64];
        int d_idx;

        for (d_idx = 0; d_idx < 64; d_idx++) denom_counts[d_idx] = 0;

        for (i = 0; i < n_new; i++) {
            long dx = new_pts[i].x.q;
            long dy = new_pts[i].y.q;
            long dz = new_pts[i].z.q;
            /* LCD of the three denominators */
            long lcd = dx;
            lcd = lcd / gcd_long(lcd, dy) * dy;
            lcd = lcd / gcd_long(lcd, dz) * dz;

            if (lcd > max_denom) max_denom = lcd;
            if (lcd < 64) denom_counts[(int)lcd]++;

            /* Check for 1/3 coordinates */
            if (dx == 3 || dy == 3 || dz == 3) has_third = 1;
        }

        printf("Denominator distribution (LCD of coordinates):\n");
        for (d_idx = 1; d_idx < 64; d_idx++) {
            if (denom_counts[d_idx] > 0)
                printf("  LCD = %d: %d points\n", d_idx, denom_counts[d_idx]);
        }
        if (max_denom >= 64)
            printf("  LCD >= 64: (some points have larger denominators)\n");
        printf("  Max denominator seen: %ld\n\n", max_denom);
    }

    check("1/3 denominators appear", has_third);

    /* Print some interesting new points */
    printf("First 30 new points (in order found):\n");
    for (i = 0; i < n_new && i < 30; i++) {
        printf("  %3d: ", i + 1);
        rvec3_print(new_pts[i]);
        printf("\n");
    }
    if (n_new > 30) printf("  ... and %d more\n", n_new - 30);

    /* ---- Step 4: Look specifically for centroid (1/3, 1/3, 0) ---- */
    printf("\n--- Step 4: Searching for specific points ---\n");
    {
        RVec3 centroid;
        int found_centroid = 0;

        centroid.x = rat_make(1, 3);
        centroid.y = rat_make(1, 3);
        centroid.z = rat_make(0, 1);

        for (i = 0; i < n_new; i++) {
            if (rvec3_eq(new_pts[i], centroid)) {
                found_centroid = 1;
                break;
            }
        }

        printf("  Centroid (1/3, 1/3, 0): %s\n",
               found_centroid ? "FOUND" : "not found (requires midpoints)");
        /* The centroid is NOT directly constructible from the 16 integer
         * bootstrap points.  No line through two of the 16 passes through
         * (1/3, 1/3, 0) -- it lies on the median from O to midpoint(A,B),
         * and midpoint(A,B) = (1/2,1/2,0) is not in the bootstrap set.
         * Constructing the centroid requires spheres for the midpoint step. */
        check("centroid (1/3,1/3,0) requires midpoints (not in bootstrap)",
              !found_centroid);
    }

    /* Check for (0, -1/3, 0) -- from our hand calculation */
    {
        RVec3 target;
        int found = 0;
        target.x = rat_make(0, 1);
        target.y = rat_make(-1, 3);
        target.z = rat_make(0, 1);
        for (i = 0; i < n_new; i++) {
            if (rvec3_eq(new_pts[i], target)) { found = 1; break; }
        }
        printf("  (0, -1/3, 0): %s\n", found ? "FOUND" : "not found");
    }

    /* ---- Step 5: Count points by type ---- */
    printf("\n--- Step 5: Point classification ---\n");
    {
        int n_integer = 0;
        int n_half = 0;
        int n_third = 0;
        int n_other = 0;

        for (i = 0; i < n_new; i++) {
            long dx = new_pts[i].x.q;
            long dy = new_pts[i].y.q;
            long dz = new_pts[i].z.q;
            long lcd = dx;
            lcd = lcd / gcd_long(lcd, dy) * dy;
            lcd = lcd / gcd_long(lcd, dz) * dz;

            if (lcd == 1) n_integer++;
            else if (lcd == 2) n_half++;
            else if (lcd == 3) n_third++;
            else n_other++;
        }

        printf("  New integer points: %d\n", n_integer);
        printf("  Half-integer (LCD=2): %d\n", n_half);
        printf("  Third-integer (LCD=3): %d\n", n_third);
        printf("  Other: %d\n", n_other);
    }

    /* ---- Step 6: Bounding box and spatial analysis ---- */
    printf("\n--- Step 6: Bounding box of new points ---\n");
    {
        long min_num = 0, max_num = 0;  /* track as numerators over LCD */
        int n_inside_unit = 0;          /* points with all coords in [0,1] */
        int n_inside_bootstrap = 0;     /* points with all coords in [-1,2] */

        for (i = 0; i < n_new; i++) {
            Rat coords[3];
            int inside_unit = 1;
            int inside_bootstrap = 1;
            int c;

            coords[0] = new_pts[i].x;
            coords[1] = new_pts[i].y;
            coords[2] = new_pts[i].z;

            for (c = 0; c < 3; c++) {
                /* Compare coord to min/max (as integers * common denom) */
                long val_x_1 = coords[c].p; /* coord value * 1/q */
                /* simple: just check sign and magnitude */
                if (coords[c].p * 1 < 0 * coords[c].q ||
                    coords[c].p > 1 * coords[c].q)
                    inside_unit = 0;
                if (coords[c].p < -1 * coords[c].q ||
                    coords[c].p > 2 * coords[c].q)
                    inside_bootstrap = 0;

                /* Track extremes as rationals - just print the most extreme */
                if (i == 0 && c == 0) {
                    min_num = coords[c].p;
                    max_num = coords[c].p;
                }
                /* Approximate comparison (good enough for reporting) */
                if (coords[c].p * 100 / coords[c].q <
                    min_num) {
                    min_num = coords[c].p * 100 / coords[c].q;
                }
                if (coords[c].p * 100 / coords[c].q >
                    max_num) {
                    max_num = coords[c].p * 100 / coords[c].q;
                }
                (void)val_x_1;
            }
            if (inside_unit) n_inside_unit++;
            if (inside_bootstrap) n_inside_bootstrap++;
        }

        printf("  Points inside [0,1]^3: %d\n", n_inside_unit);
        printf("  Points inside [-1,2]^3 (bootstrap box): %d\n",
               n_inside_bootstrap);
        printf("  Points OUTSIDE bootstrap box: %d\n",
               n_new - n_inside_bootstrap);
    }

    /* ---- Summary ---- */
    printf("\n--- Summary ---\n");
    printf("The 16-point bootstrap is NOT inert under T_Q.\n");
    printf("One generation of line-plane intersections produces %d new points.\n",
           n_new);
    printf("This breaks the impasse of Theorem II.4:\n");
    printf("  4 points in general position → 0 new points (inert)\n");
    printf("  4 points + sphere bootstrap → 16 points → %d new points\n",
           n_new);
    printf("Spheres were needed only once, to break inertness.\n");
    printf("After that, pure rational T_Q operations generate new points.\n");

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);

    return tests_passed == tests_run ? 0 : 1;
}
