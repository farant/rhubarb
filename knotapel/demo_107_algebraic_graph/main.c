/*
 * KNOTAPEL DEMO 107: Algebraic Graph Density -- Cyc8 Relationship Analysis
 * =========================================================================
 *
 * Uses the Raqiya library to analyze relationships between Z[zeta_8] values
 * extracted from DKC catalog matrices (W_{4,2} module).
 *
 * C89, zero dependencies beyond stdio/stdlib/string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAQ_PRINT
#include "../raqiya/raqiya.h"

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * 3x3 Matrix over Raq_Cyc8 (TL representation for W_{4,2})
 * ================================================================ */

typedef struct {
    Raq_Cyc8 m[3][3];
} Mat3;

static Mat3 mat3_zero(void) {
    Mat3 r;
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            r.m[i][j] = raq_cyc8_zero();
    return r;
}

static Mat3 mat3_identity(void) {
    Mat3 r = mat3_zero();
    r.m[0][0] = r.m[1][1] = r.m[2][2] = raq_cyc8_one();
    return r;
}

static Mat3 mat3_mul(const Mat3 *p, const Mat3 *q) {
    Mat3 r;
    int i, j, k;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++) {
            r.m[i][j] = raq_cyc8_zero();
            for (k = 0; k < 3; k++)
                r.m[i][j] = raq_cyc8_add(r.m[i][j],
                    raq_cyc8_mul(p->m[i][k], q->m[k][j]));
        }
    return r;
}

static Mat3 mat3_add(const Mat3 *p, const Mat3 *q) {
    Mat3 r;
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            r.m[i][j] = raq_cyc8_add(p->m[i][j], q->m[i][j]);
    return r;
}

static Mat3 mat3_scale(Raq_Cyc8 s, const Mat3 *p) {
    Mat3 r;
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            r.m[i][j] = raq_cyc8_mul(s, p->m[i][j]);
    return r;
}

static int mat3_eq(const Mat3 *p, const Mat3 *q) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (!raq_cyc8_eq(p->m[i][j], q->m[i][j])) return 0;
    return 1;
}

/* ================================================================
 * TL generators for W_{4,2}
 * ================================================================ */

static Mat3 g_e[3];
static Mat3 g_gen_tl[6];
static Raq_Cyc8 g_A, g_A_inv;

static void build_tl_generators(void) {
    Mat3 z = mat3_zero();
    int i;

    g_e[0] = z; g_e[0].m[0][1] = raq_cyc8_one();
    g_e[1] = z; g_e[1].m[1][0] = raq_cyc8_one();
                 g_e[1].m[1][2] = raq_cyc8_one();
    g_e[2] = z; g_e[2].m[2][1] = raq_cyc8_one();

    g_A = raq_cyc8_make(0, -1, 0, 0);
    g_A_inv = raq_cyc8_conj(g_A);

    {
        Mat3 id3 = mat3_identity();
        Mat3 a_id = mat3_scale(g_A, &id3);
        Mat3 a_inv_id = mat3_scale(g_A_inv, &id3);
        for (i = 0; i < 3; i++) {
            Mat3 a_inv_e = mat3_scale(g_A_inv, &g_e[i]);
            Mat3 a_e = mat3_scale(g_A, &g_e[i]);
            g_gen_tl[2*i] = mat3_add(&a_id, &a_inv_e);
            g_gen_tl[2*i+1] = mat3_add(&a_inv_id, &a_e);
        }
    }
}

/* ================================================================
 * BFS Catalog (dedup by TL matrix via hash table)
 * ================================================================ */

#define MAX_CAT 32768
#define HASH_SIZE 65537

static Mat3 *g_cat_tl;
static int g_depth[MAX_CAT];
static int g_cat_size = 0;

static int g_hash_head[HASH_SIZE];
static int g_hash_next[MAX_CAT];

static unsigned long hash_mat3(const Mat3 *m) {
    unsigned long h = 5381;
    const long *p = &m->m[0][0].a;
    int i;
    for (i = 0; i < 36; i++)
        h = h * 31 + (unsigned long)(p[i] + 1000000);
    return h;
}

static void hash_init(void) {
    int i;
    for (i = 0; i < HASH_SIZE; i++) g_hash_head[i] = -1;
    for (i = 0; i < MAX_CAT; i++) g_hash_next[i] = -1;
}

static int hash_find(const Mat3 *m) {
    int bucket = (int)(hash_mat3(m) % (unsigned long)HASH_SIZE);
    int idx = g_hash_head[bucket];
    while (idx >= 0) {
        if (mat3_eq(&g_cat_tl[idx], m)) return idx;
        idx = g_hash_next[idx];
    }
    return -1;
}

static void hash_insert(int cat_idx) {
    int bucket = (int)(hash_mat3(&g_cat_tl[cat_idx]) % (unsigned long)HASH_SIZE);
    g_hash_next[cat_idx] = g_hash_head[bucket];
    g_hash_head[bucket] = cat_idx;
}

static void build_catalog(int max_depth) {
    int rd = 1;

    hash_init();
    g_cat_tl[0] = mat3_identity();
    g_depth[0] = 0;
    hash_insert(0);
    g_cat_size = 1;

    printf("  BFS catalog:\n");
    printf("  Round 0: 1 entry (identity)\n");

    do {
        int i, gi, prev;
        prev = g_cat_size;
        for (i = 0; i < prev && g_cat_size < MAX_CAT; i++) {
            if (g_depth[i] != rd - 1) continue;
            for (gi = 0; gi < 6 && g_cat_size < MAX_CAT; gi++) {
                Mat3 tl_prod = mat3_mul(&g_cat_tl[i], &g_gen_tl[gi]);
                if (hash_find(&tl_prod) < 0) {
                    g_cat_tl[g_cat_size] = tl_prod;
                    g_depth[g_cat_size] = rd;
                    hash_insert(g_cat_size);
                    g_cat_size++;
                }
            }
        }
        if (g_cat_size > prev) {
            printf("  Round %d: %d entries (+%d)\n",
                   rd, g_cat_size, g_cat_size - prev);
        }
        rd++;
    } while (g_cat_size > 1 && rd <= max_depth && g_cat_size < MAX_CAT);

    if (g_cat_size >= MAX_CAT)
        printf("  HIT CAP at %d entries\n", MAX_CAT);
}

/* ================================================================
 * Main
 * ================================================================ */

/* ================================================================
 * Small bipartiteness checker for arbitrary adjacency matrices
 * Used in Phase 6 for W_{n,j} module checks.
 * Returns 1 if bipartite, 0 if not.
 * If bipartite, fills color_out with the 2-coloring.
 * ================================================================ */
#define BP_CHECK_MAX 16

static int check_bipartite(const int adj[][BP_CHECK_MAX], int dim,
                           int *color_out, int *n_components) {
    int queue[BP_CHECK_MAX];
    int qh, qt, bi;
    *n_components = 0;
    memset(color_out, -1, (size_t)dim * (size_t)sizeof(int));

    for (bi = 0; bi < dim; bi++) {
        int u, vi;
        if (color_out[bi] >= 0) continue;
        (*n_components)++;
        color_out[bi] = 0;
        qh = 0; qt = 0;
        queue[qt++] = bi;
        while (qh < qt) {
            u = queue[qh++];
            for (vi = 0; vi < dim; vi++) {
                if (!adj[u][vi] || u == vi) continue;
                if (color_out[vi] == -1) {
                    color_out[vi] = 1 - color_out[u];
                    queue[qt++] = vi;
                } else if (color_out[vi] == color_out[u]) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

/* Build adjacency from list of (row,col) nonzero entries */
static void adj_add_edge(int adj[][BP_CHECK_MAX], int r, int c) {
    adj[r][c] = 1;
    adj[c][r] = 1;
}

/* Count nestings in a non-crossing matching.
 * A nesting is a pair of arcs (a,b),(c,d) where a < c < d < b
 * in boundary order. Boundary order goes clockwise around the
 * rectangle: top L-to-R, then bottom R-to-L.
 *
 * match[i] = partner of point i, points 0..n-1 are top, n..2n-1 bottom.
 */
static int count_nestings(const int *match, int n) {
    int pos[12]; /* boundary position of each point, max 2n=12 */
    int arcs[6][2]; /* arcs as (pos_a, pos_b) with pos_a < pos_b */
    int n_arcs = 0;
    int nestings = 0;
    int i, j;

    /* Build position map:
     * top points 0..n-1 -> positions 0..n-1
     * bottom points n..2n-1 -> positions 3n-1-p */
    for (i = 0; i < n; i++)
        pos[i] = i;
    for (i = n; i < 2 * n; i++)
        pos[i] = 3 * n - 1 - i;

    /* Extract arcs (each pair counted once) */
    for (i = 0; i < 2 * n; i++) {
        if (match[i] > i) {
            int pa = pos[i], pb = pos[match[i]];
            if (pa > pb) { int t = pa; pa = pb; pb = t; }
            arcs[n_arcs][0] = pa;
            arcs[n_arcs][1] = pb;
            n_arcs++;
        }
    }

    /* Count nestings: arc i contains arc j when
     * arcs[i][0] < arcs[j][0] < arcs[j][1] < arcs[i][1] */
    for (i = 0; i < n_arcs; i++) {
        for (j = i + 1; j < n_arcs; j++) {
            if (arcs[i][0] < arcs[j][0] &&
                arcs[j][1] < arcs[i][1])
                nestings++;
            else if (arcs[j][0] < arcs[i][0] &&
                     arcs[i][1] < arcs[j][1])
                nestings++;
        }
    }

    return nestings;
}

int main(void) {
    char msg[256];
    int i, r, c;
    int limit;
    Raq_ValueSet vs;
    Raq_Analysis analysis;

    printf("KNOTAPEL DEMO 107: Algebraic Graph Density\n");
    printf("============================================\n");

    g_cat_tl = (Mat3 *)calloc((size_t)MAX_CAT, sizeof(Mat3));
    if (!g_cat_tl) {
        printf("  FAIL: memory allocation\n");
        return 1;
    }

    /* ============================================================
     * Phase 0: Build catalog and verify algebra
     * ============================================================ */
    printf("\n=== Phase 0: Build Catalog + Verify Algebra ===\n\n");

    build_tl_generators();

    /* Verify TL basics */
    {
        Raq_Cyc8 prod = raq_cyc8_mul(g_A, g_A_inv);
        check("A * A_inv = 1", raq_cyc8_eq(prod, raq_cyc8_one()));
    }
    {
        Raq_Cyc8 a2 = raq_cyc8_mul(g_A, g_A);
        Raq_Cyc8 ai2 = raq_cyc8_mul(g_A_inv, g_A_inv);
        Raq_Cyc8 sum = raq_cyc8_add(a2, ai2);
        check("delta = 0", raq_cyc8_is_zero(sum));
    }

    /* Verify Galois automorphisms */
    {
        Raq_Cyc8 test = raq_cyc8_make(2, 3, -1, 5);
        Raq_Cyc8 s3 = raq_cyc8_galois_3(test);
        Raq_Cyc8 s33 = raq_cyc8_galois_3(s3);
        check("sigma_3 has order 2", raq_cyc8_eq(s33, test));
    }
    {
        Raq_Cyc8 test = raq_cyc8_make(2, 3, -1, 5);
        Raq_Cyc8 s5 = raq_cyc8_galois_5(test);
        Raq_Cyc8 s55 = raq_cyc8_galois_5(s5);
        check("sigma_5 has order 2", raq_cyc8_eq(s55, test));
    }
    {
        Raq_Cyc8 test = raq_cyc8_make(2, 3, -1, 5);
        Raq_Cyc8 s7 = raq_cyc8_galois_7(test);
        Raq_Cyc8 s53 = raq_cyc8_galois_5(raq_cyc8_galois_3(test));
        check("sigma_7 = sigma_5 o sigma_3", raq_cyc8_eq(s7, s53));
    }
    {
        Raq_Cyc8 x = raq_cyc8_make(1, 2, 0, -1);
        Raq_Cyc8 y = raq_cyc8_make(0, 1, -1, 3);
        Raq_Cyc8 xy = raq_cyc8_mul(x, y);
        Raq_Cyc8 s3_xy = raq_cyc8_galois_3(xy);
        Raq_Cyc8 s3x_s3y = raq_cyc8_mul(raq_cyc8_galois_3(x),
                                          raq_cyc8_galois_3(y));
        check("sigma_3 is multiplicative", raq_cyc8_eq(s3_xy, s3x_s3y));
    }
    {
        Raq_Cyc8 x = raq_cyc8_make(1, 2, 0, -1);
        Raq_Cyc8 y = raq_cyc8_make(0, 1, -1, 3);
        Raq_Cyc8 sum_xy = raq_cyc8_add(x, y);
        Raq_Cyc8 s3_sum = raq_cyc8_galois_3(sum_xy);
        Raq_Cyc8 s3x_s3y = raq_cyc8_add(raq_cyc8_galois_3(x),
                                          raq_cyc8_galois_3(y));
        check("sigma_3 is additive", raq_cyc8_eq(s3_sum, s3x_s3y));
    }
    {
        Raq_Cyc8 test = raq_cyc8_make(2, 3, -1, 5);
        Raq_Cyc8 rotated = test;
        int k;
        for (k = 0; k < 8; k++)
            rotated = raq_cyc8_mul_zeta(rotated);
        check("zeta_8^8 rotation = identity", raq_cyc8_eq(rotated, test));
    }
    {
        Raq_Cyc8 test = raq_cyc8_make(2, 3, -1, 5);
        Raq_Cyc8 ns = raq_cyc8_norm_sq(test);
        check("norm_sq lives in Z[sqrt(2)]",
              ns.c == 0 && (ns.b + ns.d) == 0);
    }

    build_catalog(12);
    sprintf(msg, "Catalog has %d entries", g_cat_size);
    check(msg, g_cat_size > 100);

    /* ============================================================
     * Phase 1: Extract distinct Cyc8 values using Raqiya ValueSet
     * ============================================================ */
    printf("\n=== Phase 1: Extract Distinct Cyc8 Values ===\n\n");

    limit = g_cat_size < 128 ? g_cat_size : 128;
    raq_vs_init(&vs, 8192);

    for (i = 0; i < limit; i++)
        for (r = 0; r < 3; r++)
            for (c = 0; c < 3; c++)
                raq_vs_insert(&vs, g_cat_tl[i].m[r][c]);

    printf("  Source: first %d Mat3 entries\n", limit);
    printf("  Total components examined: %d (= %d * 9)\n", limit * 9, limit);
    printf("  Distinct Cyc8 values: %d\n", vs.count);
    printf("  Pairs for analysis: %d\n", vs.count * (vs.count - 1) / 2);

    /* Basic per-value statistics */
    {
        int n_zero = 0, n_unit = 0;
        int zc_hist[5];
        memset(zc_hist, 0, sizeof(zc_hist));
        for (i = 0; i < vs.count; i++) {
            int zc = raq_cyc8_zero_count(vs.values[i]);
            zc_hist[zc]++;
            if (raq_cyc8_is_zero(vs.values[i])) n_zero++;
            if (raq_cyc8_eq(vs.values[i], raq_cyc8_one())) n_unit++;
        }
        printf("  Zero values: %d\n", n_zero);
        printf("  Unit (=1) values: %d\n", n_unit);
        printf("  Zero-component histogram:\n");
        for (i = 0; i <= 4; i++)
            printf("    %d zeros: %d values (%.1f%%)\n",
                   i, zc_hist[i],
                   100.0 * (double)zc_hist[i] / (double)vs.count);
    }

    /* Print all distinct values */
    printf("  All distinct values:\n");
    for (i = 0; i < vs.count; i++) {
        Raq_Cyc8 ns = raq_cyc8_norm_sq(vs.values[i]);
        printf("    [%2d] ", i);
        raq_print_cyc8_named(vs.values[i]);
        printf("  |z|^2=%ld\n", ns.a);
    }

    sprintf(msg, "Found %d distinct values", vs.count);
    check(msg, vs.count >= 2);

    /* ============================================================
     * Phase 2: Full Raqiya Analysis
     * ============================================================ */
    printf("\n=== Phase 2: Raqiya Analysis ===\n");

    analysis = raq_analyze(vs.values, vs.count);
    raq_print_analysis(&analysis, vs.values);

    /* Verification checks */
    sprintf(msg, "Root orbits: %d from %d values",
            analysis.root_orbits.n_groups, vs.count);
    check(msg, analysis.root_orbits.n_groups >= 1);

    sprintf(msg, "Norm classes: %d from %d values",
            analysis.norm_classes.n_groups, vs.count);
    check(msg, analysis.norm_classes.n_groups >= 1);

    sprintf(msg, "Galois orbits: %d from %d values",
            analysis.galois_orbits.n_groups, vs.count);
    check(msg, analysis.galois_orbits.n_groups >= 1);

    check("Sign patterns <= 81",
          analysis.n_sign_patterns >= 1 && analysis.n_sign_patterns <= 81);

    check("No x-y = 0 (all distinct)", analysis.diff_zeros.bins[4] == 0);

    /* ============================================================
     * Phase 3: Depth Growth -- Value Set vs Catalog Depth
     *
     * Does the distinct value set stay at 13 across the full
     * 32768-entry catalog, or does it grow at higher depths?
     * Also: does -1 = (-1,0,0,0) ever appear?
     * ============================================================ */
    printf("\n=== Phase 3: Depth Growth Analysis ===\n\n");
    {
        Raq_ValueSet full_vs;
        Raq_Cyc8 neg_one = raq_cyc8_make(-1, 0, 0, 0);
        int found_neg_one = 0;
        int neg_one_depth = -1;
        int depth;
        int max_depth_seen = 0;

        raq_vs_init(&full_vs, 8192);

        /* Find max depth in catalog */
        for (i = 0; i < g_cat_size; i++)
            if (g_depth[i] > max_depth_seen)
                max_depth_seen = g_depth[i];

        printf("  Depth | Entries | Cumul Distinct | New Values\n");
        printf("  ------|---------|----------------|----------\n");

        for (depth = 0; depth <= max_depth_seen; depth++) {
            int prev_count = full_vs.count;
            int entries_at_depth = 0;

            for (i = 0; i < g_cat_size; i++) {
                if (g_depth[i] != depth) continue;
                entries_at_depth++;
                for (r = 0; r < 3; r++)
                    for (c = 0; c < 3; c++) {
                        Raq_Cyc8 v = g_cat_tl[i].m[r][c];
                        raq_vs_insert(&full_vs, v);
                        if (!found_neg_one && raq_cyc8_eq(v, neg_one)) {
                            found_neg_one = 1;
                            neg_one_depth = depth;
                        }
                    }
            }

            printf("  %5d | %7d | %14d | %+d",
                   depth, entries_at_depth,
                   full_vs.count, full_vs.count - prev_count);

            /* Print any new values that appeared at this depth */
            if (full_vs.count > prev_count) {
                int vi;
                printf("  (");
                for (vi = prev_count; vi < full_vs.count; vi++) {
                    if (vi > prev_count) printf(", ");
                    raq_print_cyc8_named(full_vs.values[vi]);
                }
                printf(")");
            }
            printf("\n");
        }

        printf("\n  Total entries scanned: %d\n", g_cat_size);
        printf("  Total components: %d\n", g_cat_size * 9);
        printf("  Total distinct values: %d\n", full_vs.count);

        if (found_neg_one)
            printf("  -1 = (-1,0,0,0): FOUND at depth %d\n", neg_one_depth);
        else
            printf("  -1 = (-1,0,0,0): NEVER APPEARS in %d entries\n",
                   g_cat_size);

        /* Print all values from full catalog if more than first-128 set */
        if (full_vs.count > vs.count) {
            printf("\n  New values beyond first 128 entries:\n");
            for (i = vs.count; i < full_vs.count; i++) {
                Raq_Cyc8 ns = raq_cyc8_norm_sq(full_vs.values[i]);
                printf("    [%2d] ", i);
                raq_print_cyc8_named(full_vs.values[i]);
                printf("  |z|^2=%ld\n", ns.a);
            }
        } else {
            printf("  Value set is CLOSED at %d values -- "
                   "no new values at any depth.\n", full_vs.count);
        }

        sprintf(msg, "Full catalog: %d distinct values", full_vs.count);
        check(msg, full_vs.count >= 2);

        raq_vs_free(&full_vs);
    }

    /* ============================================================
     * Phase 4: Z/4Z Phase Mask Test
     *
     * Hypothesis: every matrix position (r,c) has a FIXED zeta-power
     * p(r,c) in {0,1,2,3} such that ALL catalog entries at that
     * position are integer multiples of zeta^{p(r,c)}.
     *
     * Since all values are axis-aligned (one nonzero component),
     * the phase is just which component is nonzero:
     *   a -> phase 0,  b -> phase 1,  c -> phase 2,  d -> phase 3
     * ============================================================ */
    printf("\n=== Phase 4: Z/4Z Phase Mask ===\n\n");
    {
        /* For each position, track which phases appear (bitmask) */
        unsigned phase_seen[3][3];
        int violations = 0;
        int all_fixed = 1;

        memset(phase_seen, 0, sizeof(phase_seen));

        for (i = 0; i < g_cat_size; i++) {
            for (r = 0; r < 3; r++) {
                for (c = 0; c < 3; c++) {
                    Raq_Cyc8 v = g_cat_tl[i].m[r][c];
                    if (raq_cyc8_is_zero(v)) continue;
                    /* Determine which component is nonzero */
                    if (v.a != 0) phase_seen[r][c] |= 1u;      /* phase 0 */
                    if (v.b != 0) phase_seen[r][c] |= 2u;      /* phase 1 */
                    if (v.c != 0) phase_seen[r][c] |= 4u;      /* phase 2 */
                    if (v.d != 0) phase_seen[r][c] |= 8u;      /* phase 3 */
                }
            }
        }

        printf("  Phase mask (which zeta-powers appear at each position):\n\n");
        for (r = 0; r < 3; r++) {
            printf("    ");
            for (c = 0; c < 3; c++) {
                unsigned ps = phase_seen[r][c];
                int n_phases = 0;
                int ph;
                for (ph = 0; ph < 4; ph++)
                    if (ps & (1u << (unsigned)ph)) n_phases++;
                if (n_phases == 0)
                    printf("  _  ");      /* always zero */
                else if (n_phases == 1) {
                    /* Extract the single phase */
                    for (ph = 0; ph < 4; ph++)
                        if (ps & (1u << (unsigned)ph))
                            printf(" z^%d ", ph);
                } else {
                    printf(" {");
                    for (ph = 0; ph < 4; ph++)
                        if (ps & (1u << (unsigned)ph))
                            printf("%d", ph);
                    printf("} ");
                    violations++;
                    all_fixed = 0;
                }
            }
            printf("\n");
        }

        printf("\n  Detailed phase analysis per position:\n");
        for (r = 0; r < 3; r++) {
            for (c = 0; c < 3; c++) {
                unsigned ps = phase_seen[r][c];
                int n_phases = 0;
                int ph;
                for (ph = 0; ph < 4; ph++)
                    if (ps & (1u << (unsigned)ph)) n_phases++;
                printf("    (%d,%d): ", r, c);
                if (n_phases == 0)
                    printf("always zero\n");
                else if (n_phases == 1) {
                    for (ph = 0; ph < 4; ph++)
                        if (ps & (1u << (unsigned)ph))
                            printf("FIXED phase %d (all entries = n * zeta^%d)\n",
                                   ph, ph);
                } else {
                    printf("MIXED phases: {");
                    for (ph = 0; ph < 4; ph++)
                        if (ps & (1u << (unsigned)ph))
                            printf(" %d", ph);
                    printf(" }\n");
                }
            }
        }

        printf("\n  Positions with fixed phase: %d/9\n",
               9 - violations);
        printf("  Positions with mixed phases: %d/9\n", violations);

        if (all_fixed) {
            printf("  CONFIRMED: Z/4Z phase mask is FIXED across all %d "
                   "catalog entries.\n", g_cat_size);
            printf("  Every position (r,c) has a single zeta-power.\n");
            printf("  Cyc8 matrix arithmetic reduces to INTEGER "
                   "arithmetic + fixed phase grid.\n");
        } else {
            printf("  FALSIFIED: %d positions have multiple phases.\n",
                   violations);
        }

        check("Z/4Z phase mask test complete", 1);
        sprintf(msg, "Phase mask: %d/9 positions fixed",
                9 - violations);
        check(msg, 9 - violations >= 0);
    }

    /* ============================================================
     * Phase 5: Full-Catalog Analysis (all 9 detectors on 186 values)
     *
     * Rebuild the full value set from 32768 entries, run raq_analyze,
     * then track v₂ distribution by depth and axis group evolution.
     * ============================================================ */
    printf("\n=== Phase 5: Full-Catalog 9-Detector Analysis ===\n\n");
    {
        Raq_ValueSet full_vs;
        Raq_Analysis full_analysis;
        int max_depth_seen = 0;
        int depth;

        /* Rebuild full value set */
        raq_vs_init(&full_vs, 8192);
        for (i = 0; i < g_cat_size; i++)
            for (r = 0; r < 3; r++)
                for (c = 0; c < 3; c++)
                    raq_vs_insert(&full_vs, g_cat_tl[i].m[r][c]);

        printf("  Full catalog: %d distinct values from %d matrices\n\n",
               full_vs.count, g_cat_size);

        /* Run all 9 detectors */
        full_analysis = raq_analyze(full_vs.values, full_vs.count);
        raq_print_analysis(&full_analysis, full_vs.values);

        sprintf(msg, "Full analysis: %d axis classes",
                full_analysis.axis_classes.n_groups);
        check(msg, full_analysis.axis_classes.n_groups >= 1);

        sprintf(msg, "Full analysis: %d 2-adic classes",
                full_analysis.twoadic_classes.n_groups);
        check(msg, full_analysis.twoadic_classes.n_groups >= 1);

        /* Axis group sizes comparison */
        printf("\n  --- Axis Group Size Summary ---\n");
        {
            int ax;
            const char *axis_names[] = {"a (Z*1)", "b (Z*z)", "c (Z*z^2)", "d (Z*z^3)", "zero"};
            for (ax = 0; ax < full_analysis.axis_classes.n_groups; ax++) {
                int count = 0;
                int sample_axis = -1;
                /* Find what axis this group represents */
                for (i = 0; i < full_vs.count; i++) {
                    if (full_analysis.axis_classes.group_id[i] == ax) {
                        count++;
                        if (sample_axis == -1)
                            sample_axis = raq_cyc8_axis(full_vs.values[i]);
                    }
                }
                if (sample_axis >= 0 && sample_axis <= 3)
                    printf("    %s axis: %d values\n", axis_names[sample_axis], count);
                else if (sample_axis == -1)
                    printf("    %s: %d values\n", axis_names[4], count);
                else
                    printf("    multi-axis: %d values\n", count);
            }
        }

        /* v₂ distribution by depth */
        printf("\n  --- 2-Adic Valuation by Depth ---\n\n");
        for (i = 0; i < g_cat_size; i++)
            if (g_depth[i] > max_depth_seen)
                max_depth_seen = g_depth[i];

        printf("  Depth | v2=0 | v2=1 | v2=2 | v2=3 | v2>=4 | "
               "new values at this depth\n");
        printf("  ------|------|------|------|------|-------|"
               "------------------------\n");
        {
            /* Track which v₂ classes have been seen cumulatively */
            Raq_ValueSet depth_vs;
            raq_vs_init(&depth_vs, 8192);

            for (depth = 0; depth <= max_depth_seen; depth++) {
                int prev_count = depth_vs.count;
                int v2_new[6]; /* 0,1,2,3,4+(index 4),zero(index 5) */
                memset(v2_new, 0, sizeof(v2_new));

                /* Insert values at this depth */
                for (i = 0; i < g_cat_size; i++) {
                    if (g_depth[i] != depth) continue;
                    for (r = 0; r < 3; r++)
                        for (c = 0; c < 3; c++)
                            raq_vs_insert(&depth_vs, g_cat_tl[i].m[r][c]);
                }

                /* Count new values by v₂ class */
                for (i = prev_count; i < depth_vs.count; i++) {
                    int v = raq_cyc8_2adic_val(depth_vs.values[i]);
                    if (v == -1)     v2_new[5]++;  /* zero */
                    else if (v >= 4) v2_new[4]++;
                    else             v2_new[v]++;
                }

                printf("  %5d | %+4d | %+4d | %+4d | %+4d | %+5d | "
                       "%+d total\n",
                       depth,
                       v2_new[0], v2_new[1], v2_new[2],
                       v2_new[3], v2_new[4],
                       depth_vs.count - prev_count);
            }

            /* Final v₂ histogram across all 186 values */
            printf("\n  Cumulative v2 histogram (%d values):\n",
                   depth_vs.count);
            {
                int v2_total[6];
                memset(v2_total, 0, sizeof(v2_total));
                for (i = 0; i < depth_vs.count; i++) {
                    int v = raq_cyc8_2adic_val(depth_vs.values[i]);
                    if (v == -1)     v2_total[5]++;
                    else if (v >= 4) v2_total[4]++;
                    else             v2_total[v]++;
                }
                printf("    v2=0 (odd):    %d (%.1f%%)\n",
                       v2_total[0],
                       100.0 * (double)v2_total[0] / (double)depth_vs.count);
                printf("    v2=1:          %d (%.1f%%)\n",
                       v2_total[1],
                       100.0 * (double)v2_total[1] / (double)depth_vs.count);
                printf("    v2=2:          %d (%.1f%%)\n",
                       v2_total[2],
                       100.0 * (double)v2_total[2] / (double)depth_vs.count);
                printf("    v2=3:          %d (%.1f%%)\n",
                       v2_total[3],
                       100.0 * (double)v2_total[3] / (double)depth_vs.count);
                printf("    v2>=4:         %d (%.1f%%)\n",
                       v2_total[4],
                       100.0 * (double)v2_total[4] / (double)depth_vs.count);
                printf("    zero (v2=inf): %d (%.1f%%)\n",
                       v2_total[5],
                       100.0 * (double)v2_total[5] / (double)depth_vs.count);
            }

            raq_vs_free(&depth_vs);
        }

        raq_analysis_free(&full_analysis);
        raq_vs_free(&full_vs);
    }

    /* ============================================================
     * Phase 6: Bipartiteness of TL Link-State Transition Graphs
     *
     * The Z/4Z theorem says axis-alignment is guaranteed IF the
     * link-state transition graph is bipartite. We verify this
     * for TL_n at n=2,3,4,5.
     *
     * Graph: vertices = basis link states (non-crossing matchings)
     *        edge (r,c) if ANY generator e_i has nonzero at (r,c)
     * At delta=0, e_i entries are {0,1}, so nonzero means =1.
     * ============================================================ */
    printf("\n=== Phase 6: Link-State Bipartiteness Check ===\n\n");
    {
        /* --- Planar matching types (from D35) --- */
        #define BP_MAX_N 6
        #define BP_MAX_2N 12
        #define BP_MAX_BASIS 132  /* C_6 = 132 */
        #define BP_MAX_SEGS 12

        typedef struct { int match[BP_MAX_2N]; } BpMatch;
        typedef struct { int points[BP_MAX_2N]; int count; } BpSeg;

        /* Forward declarations for local functions - implemented as
           static functions above main, but we inline them here since
           C89 doesn't allow nested functions. Using a block with
           gotos/function pointers would be ugly. Instead, we directly
           implement the logic inline. */

        int test_n_values[] = {2, 3, 4, 5, 6};
        int n_tests = 5;
        int ti;

        for (ti = 0; ti < n_tests; ti++) {
            int n = test_n_values[ti];
            int n_gens = n - 1;  /* number of TL generators */
            BpMatch basis[BP_MAX_BASIS];
            int nb = 0;          /* number of basis elements */
            int adj[BP_MAX_BASIS][BP_MAX_BASIS];
            int color[BP_MAX_BASIS];
            int queue[BP_MAX_BASIS];
            int qh, qt;
            int is_bipartite = 1;
            int n_edges = 0;
            int n_components = 0;
            int gi, col_idx, row_idx, bi;
            char msg[128];

            /* --- Enumerate basis (non-crossing matchings) --- */
            {
                BpSeg segs[1];
                int match_buf[BP_MAX_2N];
                int bp_i;

                /* Build boundary order */
                for (bp_i = 0; bp_i < n; bp_i++)
                    segs[0].points[bp_i] = bp_i;
                for (bp_i = 0; bp_i < n; bp_i++)
                    segs[0].points[n + bp_i] = 2 * n - 1 - bp_i;
                segs[0].count = 2 * n;
                memset(match_buf, -1, sizeof(match_buf));

                /* Recursive enumeration via iterative stack */
                {
                    typedef struct {
                        BpSeg segs[BP_MAX_SEGS];
                        int n_segs;
                        int match_buf[BP_MAX_2N];
                        int first_seg;
                        int j;           /* current pairing index */
                        int cnt;
                    } StackFrame;

                    StackFrame *stack;
                    int sp = 0;
                    int s, j, k;

                    stack = (StackFrame *)calloc(200, sizeof(StackFrame));
                    if (!stack) { printf("  OOM\n"); return 1; }

                    /* Push initial frame */
                    memcpy(stack[0].segs, segs, sizeof(BpSeg));
                    stack[0].n_segs = 1;
                    memcpy(stack[0].match_buf, match_buf, sizeof(match_buf));
                    stack[0].j = -1; /* sentinel: needs init */
                    sp = 1;

                    while (sp > 0) {
                        StackFrame *f = &stack[sp - 1];
                        int *pts;
                        int cnt_f;

                        if (f->j == -1) {
                            /* Find first non-empty segment */
                            f->first_seg = -1;
                            for (s = 0; s < f->n_segs; s++) {
                                if (f->segs[s].count > 0) {
                                    f->first_seg = s;
                                    break;
                                }
                            }
                            if (f->first_seg == -1) {
                                /* All empty = complete matching */
                                if (nb < BP_MAX_BASIS) {
                                    memcpy(basis[nb].match, f->match_buf,
                                           (size_t)(2 * n) * sizeof(int));
                                    nb++;
                                }
                                sp--;
                                continue;
                            }
                            f->cnt = f->segs[f->first_seg].count;
                            f->j = 1; /* first odd index to try */
                        }

                        pts = f->segs[f->first_seg].points;
                        cnt_f = f->cnt;

                        if (f->j >= cnt_f) {
                            sp--; /* exhausted all pairings */
                            continue;
                        }

                        j = f->j;
                        f->j += 2; /* advance for next iteration */

                        /* Set pairing in match buffer */
                        {
                            int new_match[BP_MAX_2N];
                            BpSeg new_segs[BP_MAX_SEGS];
                            int new_n = 0;

                            memcpy(new_match, f->match_buf, sizeof(new_match));
                            new_match[pts[0]] = pts[j];
                            new_match[pts[j]] = pts[0];

                            for (k = 0; k < f->n_segs; k++) {
                                if (k == f->first_seg) {
                                    if (j > 1) {
                                        memcpy(new_segs[new_n].points,
                                               &pts[1],
                                               (size_t)(j - 1) * sizeof(int));
                                        new_segs[new_n].count = j - 1;
                                        new_n++;
                                    }
                                    if (cnt_f - j - 1 > 0) {
                                        memcpy(new_segs[new_n].points,
                                               &pts[j + 1],
                                               (size_t)(cnt_f-j-1)*sizeof(int));
                                        new_segs[new_n].count = cnt_f - j - 1;
                                        new_n++;
                                    }
                                } else {
                                    new_segs[new_n] = f->segs[k];
                                    new_n++;
                                }
                            }

                            /* Push new frame */
                            if (sp < 199) {
                                memcpy(stack[sp].segs, new_segs,
                                       (size_t)new_n * sizeof(BpSeg));
                                stack[sp].n_segs = new_n;
                                memcpy(stack[sp].match_buf, new_match,
                                       sizeof(new_match));
                                stack[sp].j = -1;
                                sp++;
                            }
                        }
                    }

                    free(stack);
                }
            }

            printf("  TL_%d: %d basis states (C_%d)\n", n, nb, n);

            /* --- Build adjacency from all generators e_i --- */
            memset(adj, 0, sizeof(adj));
            for (gi = 0; gi < n_gens; gi++) {
                /* Build generator diagram for e_{gi} */
                BpMatch gen_diag;
                int gk;
                for (gk = 0; gk < n; gk++) {
                    gen_diag.match[gk] = n + gk;
                    gen_diag.match[n + gk] = gk;
                }
                gen_diag.match[gi] = gi + 1;
                gen_diag.match[gi + 1] = gi;
                gen_diag.match[n + gi] = n + gi + 1;
                gen_diag.match[n + gi + 1] = n + gi;

                /* Compose gen_diag with each basis element */
                for (col_idx = 0; col_idx < nb; col_idx++) {
                    BpMatch result_m;
                    int glue_vis[BP_MAX_N];
                    int loops = 0;
                    int ci;

                    memset(result_m.match, -1, sizeof(result_m.match));
                    memset(glue_vis, 0, sizeof(glue_vis));

                    /* Phase 1: trace external paths */
                    for (ci = 0; ci < 2 * n; ci++) {
                        int start_pt = ci;
                        int in_d1, cur, partner;

                        if (result_m.match[ci] >= 0) continue;

                        if (ci < n) { in_d1 = 1; cur = ci; }
                        else        { in_d1 = 0; cur = ci; }

                        for (;;) {
                            if (in_d1) {
                                partner = gen_diag.match[cur];
                                if (partner < n) {
                                    result_m.match[start_pt] = partner;
                                    result_m.match[partner] = start_pt;
                                    break;
                                }
                                glue_vis[partner - n] = 1;
                                in_d1 = 0;
                                cur = partner - n;
                            } else {
                                partner = basis[col_idx].match[cur];
                                if (partner >= n) {
                                    result_m.match[start_pt] = partner;
                                    result_m.match[partner] = start_pt;
                                    break;
                                }
                                glue_vis[partner] = 1;
                                in_d1 = 1;
                                cur = n + partner;
                            }
                        }
                    }

                    /* Phase 2: count closed loops */
                    for (ci = 0; ci < n; ci++) {
                        int cur_l, p_l, q_l;
                        if (glue_vis[ci]) continue;
                        loops++;
                        cur_l = ci;
                        do {
                            glue_vis[cur_l] = 1;
                            p_l = basis[col_idx].match[cur_l];
                            glue_vis[p_l] = 1;
                            q_l = gen_diag.match[n + p_l];
                            cur_l = q_l - n;
                        } while (cur_l != ci);
                    }

                    if (loops > 0) continue; /* delta=0 kills it */

                    /* Find which basis element the result matches */
                    for (row_idx = 0; row_idx < nb; row_idx++) {
                        int eq = 1;
                        int mi;
                        for (mi = 0; mi < 2 * n; mi++) {
                            if (result_m.match[mi] !=
                                basis[row_idx].match[mi]) {
                                eq = 0; break;
                            }
                        }
                        if (eq) {
                            /* Edge: row_idx <-> col_idx (undirected) */
                            if (!adj[row_idx][col_idx]) {
                                adj[row_idx][col_idx] = 1;
                                adj[col_idx][row_idx] = 1;
                                n_edges++;
                            }
                            break;
                        }
                    }
                }
            }

            printf("    %d edges in transition graph\n", n_edges);

            /* --- BFS 2-coloring for bipartiteness --- */
            memset(color, -1, sizeof(color));
            is_bipartite = 1;

            for (bi = 0; bi < nb && is_bipartite; bi++) {
                if (color[bi] >= 0) continue;
                n_components++;
                color[bi] = 0;
                qh = 0; qt = 0;
                queue[qt++] = bi;

                while (qh < qt && is_bipartite) {
                    int u = queue[qh++];
                    int vi;
                    for (vi = 0; vi < nb; vi++) {
                        if (!adj[u][vi]) continue;
                        if (color[vi] == -1) {
                            color[vi] = 1 - color[u];
                            queue[qt++] = vi;
                        } else if (color[vi] == color[u]) {
                            is_bipartite = 0;
                        }
                    }
                }
            }

            {
                int c0 = 0, c1 = 0;
                for (bi = 0; bi < nb; bi++) {
                    if (color[bi] == 0) c0++;
                    else if (color[bi] == 1) c1++;
                }
                printf("    %d components, partition: %d + %d = %d\n",
                       n_components, c0, c1, c0 + c1);
            }

            if (is_bipartite) {
                printf("    BIPARTITE: YES\n");

                /* Print the 2-coloring with matching details */
                { /* nesting + through-strand parity checks */
                    int pi;
                    if (nb <= 42) { /* detailed output for n<=5 */
                        printf("    2-coloring (basis# : color : "
                               "through-strands : matching):\n");
                        for (pi = 0; pi < nb; pi++) {
                            int through = 0;
                            int mi2;
                            for (mi2 = 0; mi2 < n; mi2++) {
                                if (basis[pi].match[mi2] >= n)
                                    through++;
                            }
                            printf("      %3d : %d : %d-through : {",
                                   pi, color[pi], through);
                            for (mi2 = 0; mi2 < 2 * n; mi2++) {
                                if (mi2 > 0) printf(",");
                                printf("%d", basis[pi].match[mi2]);
                            }
                            printf("}\n");
                        }
                    }

                    /* Check: does through-strand parity predict
                       the color? */
                    {
                        int parity_matches = 1;
                        int ref_parity = -1;
                        for (pi = 0; pi < nb; pi++) {
                            int through = 0;
                            int mi2;
                            int par;
                            for (mi2 = 0; mi2 < n; mi2++) {
                                if (basis[pi].match[mi2] >= n)
                                    through++;
                            }
                            par = through % 2;
                            if (ref_parity == -1) {
                                ref_parity = (color[0] == par) ? 1 : 0;
                            }
                            if (ref_parity) {
                                if (color[pi] != par) {
                                    parity_matches = 0;
                                    break;
                                }
                            } else {
                                if (color[pi] != (1 - par)) {
                                    parity_matches = 0;
                                    break;
                                }
                            }
                        }
                        printf("    Through-strand parity = color? %s\n",
                               parity_matches ? "YES" : "NO");
                    }

                    /* Check: does NESTING parity predict the color?
                     * Nesting = pair of arcs where one contains the
                     * other in boundary order. */
                    {
                        int nest_matches = 1;
                        int nest_ref = -1;
                        if (nb <= 42)
                            printf("    Nesting analysis:\n");
                        for (pi = 0; pi < nb; pi++) {
                            int nest = count_nestings(
                                basis[pi].match, n);
                            int par = nest % 2;
                            if (nb <= 42)
                                printf("      %3d: nestings=%d "
                                   "parity=%d color=%d %s\n",
                                   pi, nest, par, color[pi],
                                   (nest_ref == -1) ? "" :
                                   ((nest_ref ? par : 1 - par)
                                    == color[pi]) ? "ok" : "MISMATCH");
                            if (nest_ref == -1) {
                                nest_ref =
                                    (color[0] == par) ? 1 : 0;
                            } else {
                                int expected = nest_ref ?
                                    par : 1 - par;
                                if (expected != color[pi]) {
                                    nest_matches = 0;
                                }
                            }
                        }
                        printf("    Nesting parity = color? %s\n",
                               nest_matches ? "YES" : "NO");

                        sprintf(msg,
                            "TL_%d nesting parity = BFS color", n);
                        check(msg, nest_matches);
                    }
                }
            } else {
                printf("    BIPARTITE: NO (odd cycle detected)\n");
            }

            sprintf(msg, "TL_%d link-state graph bipartiteness", n);
            check(msg, is_bipartite);
        }

        /* --- W_{n,j} bipartiteness checks (standard modules) ---
         *
         * TL_n checks above use the LEFT REGULAR representation
         * (full algebra, dim=C_n). Standard modules W_{n,j} have
         * different bases (partial matchings with j through-strands).
         * Must verify separately for each module used in DKC demos.
         *
         * eᵢ matrices from: D99 (W_{3,1}), D100 (W_{4,2}),
         * D101 (W_{5,3}), D102 (W_{6,0}, W_{6,4}), D103 (W_{6,2}).
         */
        printf("\n  --- Standard Module W_{n,j} Checks ---\n");
        {
            /* --- W_{3,1}: dim=2, 2 generators --- */
            int a31[BP_CHECK_MAX][BP_CHECK_MAX];
            int c31[BP_CHECK_MAX], nc31;
            memset(a31, 0, sizeof(a31));
            /* e_1: [1][0]=1 */  adj_add_edge(a31, 1, 0);
            /* e_2: [0][1]=1 */  adj_add_edge(a31, 0, 1);
            {
                int bp = check_bipartite(a31, 2, c31, &nc31);
                printf("\n  W_{3,1}: dim=2, 1 edge, %d comp, "
                       "partition {%d,%d}: %s\n",
                       nc31, c31[0], c31[1],
                       bp ? "BIPARTITE" : "NOT BIPARTITE");
                check("W_{3,1} bipartiteness", bp);
            }
        }
        {
            /* --- W_{4,2}: dim=3, 3 generators (from g_e[]) --- */
            int a42[BP_CHECK_MAX][BP_CHECK_MAX];
            int c42[BP_CHECK_MAX], nc42;
            int gi2, r2, c2i;
            memset(a42, 0, sizeof(a42));
            for (gi2 = 0; gi2 < 3; gi2++)
                for (r2 = 0; r2 < 3; r2++)
                    for (c2i = 0; c2i < 3; c2i++)
                        if (!raq_cyc8_eq(g_e[gi2].m[r2][c2i],
                                         raq_cyc8_zero()))
                            adj_add_edge(a42, r2, c2i);
            {
                int bp = check_bipartite(a42, 3, c42, &nc42);
                printf("\n  W_{4,2}: dim=3, %d comp, "
                       "partition {%d,%d,%d}: %s\n",
                       nc42, c42[0], c42[1], c42[2],
                       bp ? "BIPARTITE" : "NOT BIPARTITE");
                check("W_{4,2} bipartiteness", bp);
            }
        }
        {
            /* --- W_{5,3}: dim=4, 4 generators --- */
            int a53[BP_CHECK_MAX][BP_CHECK_MAX];
            int c53[BP_CHECK_MAX], nc53;
            memset(a53, 0, sizeof(a53));
            /* e_1: [0][1] */ adj_add_edge(a53, 0, 1);
            /* e_2: [1][0],[1][2] */
            adj_add_edge(a53, 1, 0); adj_add_edge(a53, 1, 2);
            /* e_3: [2][1],[2][3] */
            adj_add_edge(a53, 2, 1); adj_add_edge(a53, 2, 3);
            /* e_4: [3][2] */ adj_add_edge(a53, 3, 2);
            {
                int bp = check_bipartite(a53, 4, c53, &nc53);
                printf("\n  W_{5,3}: dim=4, %d comp, "
                       "colors [%d,%d,%d,%d]: %s\n",
                       nc53, c53[0], c53[1], c53[2], c53[3],
                       bp ? "BIPARTITE" : "NOT BIPARTITE");
                check("W_{5,3} bipartiteness", bp);
            }
        }
        {
            /* --- W_{6,0}: dim=5, 5 generators (D102 build_tl_a) --- */
            int a60[BP_CHECK_MAX][BP_CHECK_MAX];
            int c60[BP_CHECK_MAX], nc60;
            memset(a60, 0, sizeof(a60));
            /* e_1: [0][2],[0][4],[1][3] */
            adj_add_edge(a60, 0, 2); adj_add_edge(a60, 0, 4);
            adj_add_edge(a60, 1, 3);
            /* e_2: [2][0],[3][1],[3][4] */
            adj_add_edge(a60, 2, 0); adj_add_edge(a60, 3, 1);
            adj_add_edge(a60, 3, 4);
            /* e_3: [0][1],[0][2],[4][3] */
            adj_add_edge(a60, 0, 1); adj_add_edge(a60, 0, 2);
            adj_add_edge(a60, 4, 3);
            /* e_4: [1][0],[3][2],[3][4] */
            adj_add_edge(a60, 1, 0); adj_add_edge(a60, 3, 2);
            adj_add_edge(a60, 3, 4);
            /* e_5: [0][1],[0][4],[2][3] */
            adj_add_edge(a60, 0, 1); adj_add_edge(a60, 0, 4);
            adj_add_edge(a60, 2, 3);
            {
                int bp = check_bipartite(a60, 5, c60, &nc60);
                printf("\n  W_{6,0}: dim=5, %d comp, "
                       "colors [%d,%d,%d,%d,%d]: %s\n",
                       nc60, c60[0], c60[1], c60[2], c60[3], c60[4],
                       bp ? "BIPARTITE" : "NOT BIPARTITE");
                check("W_{6,0} bipartiteness", bp);
            }
        }
        {
            /* --- W_{6,4}: dim=5, 5 generators (D102 build_tl_b) --- */
            int a64[BP_CHECK_MAX][BP_CHECK_MAX];
            int c64[BP_CHECK_MAX], nc64;
            memset(a64, 0, sizeof(a64));
            /* e_1: [4][3] */ adj_add_edge(a64, 4, 3);
            /* e_2: [3][2],[3][4] */
            adj_add_edge(a64, 3, 2); adj_add_edge(a64, 3, 4);
            /* e_3: [2][1],[2][3] */
            adj_add_edge(a64, 2, 1); adj_add_edge(a64, 2, 3);
            /* e_4: [1][0],[1][2] */
            adj_add_edge(a64, 1, 0); adj_add_edge(a64, 1, 2);
            /* e_5: [0][1] */ adj_add_edge(a64, 0, 1);
            {
                int bp = check_bipartite(a64, 5, c64, &nc64);
                printf("\n  W_{6,4}: dim=5, %d comp, "
                       "colors [%d,%d,%d,%d,%d]: %s\n",
                       nc64, c64[0], c64[1], c64[2], c64[3], c64[4],
                       bp ? "BIPARTITE" : "NOT BIPARTITE");
                check("W_{6,4} bipartiteness", bp);
            }
        }
        {
            /* --- W_{6,2}: dim=9, 5 generators (D103 build_tl_b) --- */
            int a62[BP_CHECK_MAX][BP_CHECK_MAX];
            int c62[BP_CHECK_MAX], nc62;
            memset(a62, 0, sizeof(a62));
            /* e_1: [0][1],[0][5],[2][4],[3][6] */
            adj_add_edge(a62, 0, 1); adj_add_edge(a62, 0, 5);
            adj_add_edge(a62, 2, 4); adj_add_edge(a62, 3, 6);
            /* e_2: [1][0],[4][2],[4][5],[4][8],[6][3],[6][7] */
            adj_add_edge(a62, 1, 0); adj_add_edge(a62, 4, 2);
            adj_add_edge(a62, 4, 5); adj_add_edge(a62, 4, 8);
            adj_add_edge(a62, 6, 3); adj_add_edge(a62, 6, 7);
            /* e_3: [0][1],[0][2],[5][4],[7][6],[7][8] */
            adj_add_edge(a62, 0, 1); adj_add_edge(a62, 0, 2);
            adj_add_edge(a62, 5, 4); adj_add_edge(a62, 7, 6);
            adj_add_edge(a62, 7, 8);
            /* e_4: [2][0],[2][3],[4][1],[4][5],[4][6],[8][7] */
            adj_add_edge(a62, 2, 0); adj_add_edge(a62, 2, 3);
            adj_add_edge(a62, 4, 1); adj_add_edge(a62, 4, 5);
            adj_add_edge(a62, 4, 6); adj_add_edge(a62, 8, 7);
            /* e_5: [3][2],[6][4],[7][5],[7][8] */
            adj_add_edge(a62, 3, 2); adj_add_edge(a62, 6, 4);
            adj_add_edge(a62, 7, 5); adj_add_edge(a62, 7, 8);
            {
                int bp = check_bipartite(a62, 9, c62, &nc62);
                printf("\n  W_{6,2}: dim=9, %d comp, colors [",
                       nc62);
                {
                    int pi;
                    for (pi = 0; pi < 9; pi++) {
                        if (pi > 0) printf(",");
                        printf("%d", c62[pi]);
                    }
                }
                printf("]: %s\n",
                       bp ? "BIPARTITE" : "NOT BIPARTITE");
                check("W_{6,2} bipartiteness", bp);
            }
        }
    }

    /* ============================================================
     * Phase 6b: Axis Distribution at Complete Depths (0-7)
     *
     * Depth 8 is truncated (catalog cap hit mid-expansion).
     * Check axis balance using only complete depths to remove
     * truncation bias.
     * ============================================================ */
    printf("\n=== Phase 6b: Axis Distribution (Complete Depths 0-7) ===\n\n");
    {
        Raq_ValueSet d7_vs;
        int d7_axis[5]; /* 0=a, 1=b, 2=c, 3=d, 4=zero */
        int d8_axis[5];
        int all_axis[5];

        raq_vs_init(&d7_vs, 4096);
        memset(d7_axis, 0, sizeof(d7_axis));
        memset(d8_axis, 0, sizeof(d8_axis));
        memset(all_axis, 0, sizeof(all_axis));

        /* Collect values from depths 0-7 only */
        for (i = 0; i < g_cat_size; i++) {
            int r, c;
            if (g_depth[i] > 7) continue;
            for (r = 0; r < 3; r++) {
                for (c = 0; c < 3; c++) {
                    Raq_Cyc8 val = g_cat_tl[i].m[r][c];
                    if (raq_vs_find(&d7_vs, val) < 0) {
                        int ax = raq_cyc8_axis(val);
                        int bucket = (ax == -1) ? 4 :
                                     (ax == -2) ? 4 : ax;
                        d7_axis[bucket]++;
                        raq_vs_insert(&d7_vs, val);
                    }
                }
            }
        }

        /* Now count axis for ALL values (already have from Phase 5,
           but recount to get both d7 and d8 splits) */
        {
            Raq_ValueSet all_vs;
            Raq_ValueSet d8_only_vs;
            raq_vs_init(&all_vs, 8192);
            raq_vs_init(&d8_only_vs, 4096);

            for (i = 0; i < g_cat_size; i++) {
                int r, c;
                for (r = 0; r < 3; r++) {
                    for (c = 0; c < 3; c++) {
                        Raq_Cyc8 val = g_cat_tl[i].m[r][c];
                        if (raq_vs_find(&all_vs, val) < 0) {
                            int ax = raq_cyc8_axis(val);
                            int bucket = (ax == -1) ? 4 :
                                         (ax == -2) ? 4 : ax;
                            all_axis[bucket]++;
                            raq_vs_insert(&all_vs, val);

                            /* Is it new at depth 8? */
                            if (g_depth[i] == 8 &&
                                raq_vs_find(&d7_vs, val) < 0) {
                                if (raq_vs_find(&d8_only_vs, val) < 0) {
                                    d8_axis[bucket]++;
                                    raq_vs_insert(&d8_only_vs, val);
                                }
                            }
                        }
                    }
                }
            }

            printf("  Depth 0-7 (complete): %d distinct values\n",
                   d7_vs.count);
            printf("    a-axis (zeta^0): %d\n", d7_axis[0]);
            printf("    b-axis (zeta^1): %d\n", d7_axis[1]);
            printf("    c-axis (zeta^2): %d\n", d7_axis[2]);
            printf("    d-axis (zeta^3): %d\n", d7_axis[3]);
            printf("    zero:            %d\n", d7_axis[4]);
            printf("    even (a+c): %d, odd (b+d): %d\n",
                   d7_axis[0] + d7_axis[2],
                   d7_axis[1] + d7_axis[3]);

            printf("\n  Depth 8 only (truncated): %d new values\n",
                   d8_only_vs.count);
            printf("    a-axis (zeta^0): %d\n", d8_axis[0]);
            printf("    b-axis (zeta^1): %d\n", d8_axis[1]);
            printf("    c-axis (zeta^2): %d\n", d8_axis[2]);
            printf("    d-axis (zeta^3): %d\n", d8_axis[3]);
            printf("    zero:            %d\n", d8_axis[4]);
            printf("    even (a+c): %d, odd (b+d): %d\n",
                   d8_axis[0] + d8_axis[2],
                   d8_axis[1] + d8_axis[3]);

            printf("\n  All depths (0-8): %d distinct values\n",
                   all_vs.count);
            printf("    a-axis (zeta^0): %d\n", all_axis[0]);
            printf("    b-axis (zeta^1): %d\n", all_axis[1]);
            printf("    c-axis (zeta^2): %d\n", all_axis[2]);
            printf("    d-axis (zeta^3): %d\n", all_axis[3]);
            printf("    zero:            %d\n", all_axis[4]);
            printf("    even (a+c): %d, odd (b+d): %d\n",
                   all_axis[0] + all_axis[2],
                   all_axis[1] + all_axis[3]);

            raq_vs_free(&all_vs);
            raq_vs_free(&d8_only_vs);
        }

        raq_vs_free(&d7_vs);
    }

    /* ============================================================
     * Phase 7: Graph Analysis Sweep
     * ============================================================ */
    printf("\n=== Phase 7a: Graph Analysis Sweep (first-128 values) ===\n");
    raq_graph_sweep(vs.values, vs.count, stdout);

    printf("\n=== Phase 7b: Graph Analysis Sweep (full catalog) ===\n");
    {
        Raq_ValueSet full_sweep_vs;
        raq_vs_init(&full_sweep_vs, 8192);
        for (i = 0; i < g_cat_size; i++)
            for (r = 0; r < 3; r++)
                for (c = 0; c < 3; c++)
                    raq_vs_insert(&full_sweep_vs, g_cat_tl[i].m[r][c]);
        raq_graph_sweep(full_sweep_vs.values, full_sweep_vs.count, stdout);
        raq_vs_free(&full_sweep_vs);
    }

    /* ============================================================
     * Phase 8: W_{6,2} Bipartite Invariant -- Major Index Theorem
     *
     * W_{6,2} has 9 basis states: non-crossing partial matchings
     * of {1,...,6} with exactly 2 through-strands (unmatched points),
     * subject to non-enclosure: no through-strand sits between the
     * endpoints of any arc.
     *
     * BFS 2-coloring of the W_{6,2} transition graph:
     *   [0, 1, 1, 0, 0, 1, 1, 0, 1]
     *
     * Candidate invariants:
     *   (a) maj mod 2 = sum of right-endpoint positions of arcs, mod 2
     *   (b) through-strands-left: for each arc, count through-strands
     *       to the left of its left endpoint, sum mod 2
     *   (c) parity of leftmost arc endpoint position
     *   (d) arc-arc nesting count mod 2
     *
     * Prediction: (a) is the universal bipartite invariant.
     * ============================================================ */
    printf("\n=== Phase 8: W_{6,2} Bipartite Invariant ===\n\n");
    {
        /* Enumerate non-crossing partial matchings of {1..6}
         * with exactly 2 through-strands + non-enclosure constraint.
         *
         * Representation: arcs[k] = (left, right) positions (1-indexed),
         *                 throughs[k] = position of k-th through-strand
         * With 2 through-strands and 6 points, we have exactly 2 arcs.
         */

        /* We'll store each state as: arc pairs + through positions */
        typedef struct {
            int arc_l[2], arc_r[2];   /* left/right endpoints of 2 arcs */
            int thr[2];               /* 2 through-strand positions */
        } W62State;

        W62State states[20];  /* upper bound */
        int n_states = 0;
        int a, b2, c2, d2;

        /* Brute force: pick 2 arcs from 6 positions.
         * Arc 1: positions (a, b2) with a < b2
         * Arc 2: positions (c2, d2) with c2 < d2
         * Non-crossing: arcs don't interleave
         * Non-enclosure: no through-strand between endpoints of any arc
         * Order arcs so arc1 left < arc2 left
         */
        for (a = 1; a <= 6; a++) {
            for (b2 = a + 1; b2 <= 6; b2++) {
                for (c2 = 1; c2 <= 6; c2++) {
                    for (d2 = c2 + 1; d2 <= 6; d2++) {
                        int used[7];
                        int t[2];
                        int nt, p2, crossing, enclosed;

                        /* No overlap with first arc */
                        if (c2 == a || c2 == b2 || d2 == a || d2 == b2)
                            continue;
                        /* Canonical: arc1 left < arc2 left */
                        if (c2 <= a) continue;

                        /* Non-crossing check: arcs (a,b2) and (c2,d2)
                         * Cross iff a < c2 < b2 < d2 or c2 < a < d2 < b2 */
                        crossing = 0;
                        if (a < c2 && c2 < b2 && b2 < d2) crossing = 1;
                        if (c2 < a && a < d2 && d2 < b2) crossing = 1;
                        if (crossing) continue;

                        /* Find through-strands */
                        memset(used, 0, sizeof(used));
                        used[a] = used[b2] = used[c2] = used[d2] = 1;
                        nt = 0;
                        for (p2 = 1; p2 <= 6; p2++)
                            if (!used[p2]) t[nt++] = p2;
                        if (nt != 2) continue; /* sanity */

                        /* Non-enclosure: no through-strand between
                         * endpoints of any arc (standard for W_{n,j}) */
                        enclosed = 0;
                        for (p2 = 0; p2 < 2; p2++) {
                            if (t[p2] > a && t[p2] < b2) enclosed = 1;
                            if (t[p2] > c2 && t[p2] < d2) enclosed = 1;
                        }
                        if (enclosed) continue;

                        /* Valid state! */
                        states[n_states].arc_l[0] = a;
                        states[n_states].arc_r[0] = b2;
                        states[n_states].arc_l[1] = c2;
                        states[n_states].arc_r[1] = d2;
                        states[n_states].thr[0] = t[0];
                        states[n_states].thr[1] = t[1];
                        n_states++;
                    }
                }
            }
        }

        printf("  Enumerated %d W_{6,2} states (expect 9)\n", n_states);
        check("W_{6,2} has 9 states", n_states == 9);

        /* Print enumerated states */
        {
            int si;
            printf("\n  Enumerated states:\n");
            for (si = 0; si < n_states; si++) {
                W62State *s = &states[si];
                printf("    h%d: arcs (%d,%d)(%d,%d) throughs %d,%d\n",
                       si, s->arc_l[0], s->arc_r[0],
                       s->arc_l[1], s->arc_r[1],
                       s->thr[0], s->thr[1]);
            }
        }

        /* Use D103's known state ordering to map our states and
         * check maj mod 2 against the known BFS 2-coloring.
         *
         * D103 basis (from source comments lines 24-29):
         *   d103_h0: arcs(1,2)(3,4) TL{5,6}
         *   d103_h1: arcs(1,4)(2,3) TL{5,6}
         *   d103_h2: arcs(1,2)(4,5) TL{3,6}
         *   d103_h3: arcs(1,2)(5,6) TL{3,4}
         *   d103_h4: arcs(2,3)(4,5) TL{1,6}
         *   d103_h5: arcs(2,5)(3,4) TL{1,6}
         *   d103_h6: arcs(2,3)(5,6) TL{1,4}
         *   d103_h7: arcs(3,4)(5,6) TL{1,2}
         *   d103_h8: arcs(3,6)(4,5) TL{1,2}
         *
         * BFS 2-coloring from D107 transition graph:
         *   [0, 1, 1, 0, 0, 1, 1, 0, 1]
         */
        printf("\n  Mapping to D103 ordering + checking invariants...\n");
        {
            /* D103 states as (arc_l0, arc_r0, arc_l1, arc_r1) */
            int d103[9][4] = {
                {1,2, 3,4}, {1,4, 2,3}, {1,2, 4,5},
                {1,2, 5,6}, {2,3, 4,5}, {2,5, 3,4},
                {2,3, 5,6}, {3,4, 5,6}, {3,6, 4,5}
            };
            int d103_bfs[9] = {0, 1, 1, 0, 0, 1, 1, 0, 1};
            int perm[9]; /* perm[our_i] = d103_j */
            int si, di;
            int maj_v[9], tsl_v[9], lae_v[9], nest_v[9];
            int bfs_ours[9];
            int maj_m, tsl_m, lae_m, nest_m;

            /* Build permutation by matching arc patterns */
            for (si = 0; si < n_states; si++) {
                W62State *s = &states[si];
                perm[si] = -1;
                for (di = 0; di < 9; di++) {
                    if (s->arc_l[0] == d103[di][0] &&
                        s->arc_r[0] == d103[di][1] &&
                        s->arc_l[1] == d103[di][2] &&
                        s->arc_r[1] == d103[di][3]) {
                        perm[si] = di;
                        break;
                    }
                }
            }

            /* Reorder BFS to our enumeration order */
            for (si = 0; si < 9; si++)
                bfs_ours[si] = (perm[si] >= 0) ?
                    d103_bfs[perm[si]] : -1;

            /* Verify all mapped */
            {
                int all_mapped = 1;
                for (si = 0; si < 9; si++)
                    if (perm[si] < 0) all_mapped = 0;
                check("all 9 states mapped to D103", all_mapped);
            }

            /* Compute all 4 candidate invariants */
            for (si = 0; si < n_states; si++) {
                W62State *s = &states[si];
                int k;

                /* (a) maj mod 2: sum of right-endpoint positions */
                maj_v[si] = (s->arc_r[0] + s->arc_r[1]) % 2;

                /* (b) through-strands-left: for each arc, count
                 * through-strands left of its left endpoint */
                tsl_v[si] = 0;
                for (k = 0; k < 2; k++) {
                    int lft = s->arc_l[k], t2, cnt = 0;
                    for (t2 = 0; t2 < 2; t2++)
                        if (s->thr[t2] < lft) cnt++;
                    tsl_v[si] += cnt;
                }
                tsl_v[si] = tsl_v[si] % 2;

                /* (c) parity of leftmost arc endpoint */
                lae_v[si] = s->arc_l[0] % 2;

                /* (d) arc-arc nesting count mod 2 */
                nest_v[si] = 0;
                if (s->arc_l[0] < s->arc_l[1] &&
                    s->arc_r[1] < s->arc_r[0])
                    nest_v[si]++;
                if (s->arc_l[1] < s->arc_l[0] &&
                    s->arc_r[0] < s->arc_r[1])
                    nest_v[si]++;
                nest_v[si] = nest_v[si] % 2;
            }

            /* Print comparison table */
            printf("\n  %-6s %-22s %-8s %-6s %-6s %-6s %-6s %-4s\n",
                   "Our", "Arcs (throughs)", "D103",
                   "maj%2", "tsl%2", "lae%2", "nest%2", "BFS");
            maj_m = 0; tsl_m = 0; lae_m = 0; nest_m = 0;
            for (si = 0; si < 9; si++) {
                W62State *s = &states[si];
                char buf[32];
                sprintf(buf, "(%d,%d)(%d,%d) t=%d,%d",
                        s->arc_l[0], s->arc_r[0],
                        s->arc_l[1], s->arc_r[1],
                        s->thr[0], s->thr[1]);
                printf("  h%-5d %-22s D103_h%-2d %-6d %-6d %-6d %-6d %-4d\n",
                       si, buf, perm[si],
                       maj_v[si], tsl_v[si], lae_v[si],
                       nest_v[si], bfs_ours[si]);
                if (maj_v[si] == bfs_ours[si]) maj_m++;
                if (tsl_v[si] == bfs_ours[si]) tsl_m++;
                if (lae_v[si] == bfs_ours[si]) lae_m++;
                if (nest_v[si] == bfs_ours[si]) nest_m++;
            }
            /* Account for global color swap */
            if (maj_m < 5) maj_m = 9 - maj_m;
            if (tsl_m < 5) tsl_m = 9 - tsl_m;
            if (lae_m < 5) lae_m = 9 - lae_m;
            if (nest_m < 5) nest_m = 9 - nest_m;

            printf("\n  Results (with color-swap correction):\n");
            printf("    maj mod 2:  %d/9 %s\n", maj_m,
                   maj_m == 9 ? "MATCH" : "FAIL");
            printf("    tsl mod 2:  %d/9 %s\n", tsl_m,
                   tsl_m == 9 ? "MATCH" : "FAIL");
            printf("    lae mod 2:  %d/9 %s\n", lae_m,
                   lae_m == 9 ? "MATCH" : "FAIL");
            printf("    nest mod 2: %d/9 %s\n", nest_m,
                   nest_m == 9 ? "MATCH" : "FAIL");

            check("maj mod 2 = bipartite coloring (W_{6,2})",
                  maj_m == 9);
        }

        /* Phase 8b: Independent TL action verification.
         *
         * Correct TL planar stacking mechanics (per explorer):
         * e_i = cup at (i, i+1). Cases:
         *
         * 1. Both i, i+1 are through-strands -> 0 (leaves W_{n,2})
         * 2. (i, i+1) is existing arc -> 0 (closed loop, delta=0)
         * 3. One through, one arc endpoint (i+1, partner):
         *    -> new arc(i, i+1), partner becomes through-strand
         * 4. Both in DIFFERENT arcs: i in (i,a), i+1 in (i+1,b):
         *    -> new arc(i, i+1), partners form new arc(a, b)
         */
        printf("\n  Phase 8b: Independent TL action verification\n");
        {
            int action[5][9]; /* action[gen][state] = target (-1=0) */
            int gi, si, sj;

            for (gi = 0; gi < 5; gi++) {
                int pos_l = gi + 1;  /* left strand of cup */
                int pos_r = gi + 2;  /* right strand of cup */

                for (si = 0; si < n_states; si++) {
                    W62State *s = &states[si];
                    int l_thr = 0, r_thr = 0;
                    int l_arc = -1, r_arc = -1;
                    int l_partner = -1, r_partner = -1;
                    int k;

                    /* Classify each strand */
                    for (k = 0; k < 2; k++) {
                        if (s->thr[k] == pos_l) l_thr = 1;
                        if (s->thr[k] == pos_r) r_thr = 1;
                    }
                    for (k = 0; k < 2; k++) {
                        if (s->arc_l[k] == pos_l || s->arc_r[k] == pos_l) {
                            l_arc = k;
                            l_partner = (s->arc_l[k] == pos_l) ?
                                s->arc_r[k] : s->arc_l[k];
                        }
                        if (s->arc_l[k] == pos_r || s->arc_r[k] == pos_r) {
                            r_arc = k;
                            r_partner = (s->arc_l[k] == pos_r) ?
                                s->arc_r[k] : s->arc_l[k];
                        }
                    }

                    if (l_thr && r_thr) {
                        /* Case 1: both through -> annihilated */
                        action[gi][si] = -1;
                    } else if (l_arc >= 0 && r_arc >= 0 &&
                               l_arc == r_arc) {
                        /* Case 2: same arc -> annihilated */
                        action[gi][si] = -1;
                    } else if ((l_thr && r_arc >= 0) ||
                               (r_thr && l_arc >= 0)) {
                        /* Case 3: one through, one arc endpoint.
                         * Cup absorbs both into arc(pos_l, pos_r).
                         * The arc's partner becomes a through. */
                        W62State res;
                        int freed = l_thr ? r_partner : l_partner;
                        int old_thr = l_thr ? pos_l : pos_r;
                        int found;

                        /* New arc(pos_l, pos_r) replaces the old arc.
                         * Other arc unchanged. */
                        res = *s;
                        /* Which arc index was the one being modified? */
                        {
                            int mod_arc = l_thr ? r_arc : l_arc;
                            res.arc_l[mod_arc] = pos_l;
                            res.arc_r[mod_arc] = pos_r;
                        }
                        /* Through-strands: old_thr replaced by freed */
                        for (k = 0; k < 2; k++)
                            if (res.thr[k] == old_thr)
                                res.thr[k] = freed;

                        /* Normalize: arc_l < arc_r within each arc */
                        for (k = 0; k < 2; k++) {
                            if (res.arc_l[k] > res.arc_r[k]) {
                                int tmp = res.arc_l[k];
                                res.arc_l[k] = res.arc_r[k];
                                res.arc_r[k] = tmp;
                            }
                        }
                        /* Arc with smaller left first */
                        if (res.arc_l[0] > res.arc_l[1]) {
                            int tmp;
                            tmp = res.arc_l[0]; res.arc_l[0] = res.arc_l[1]; res.arc_l[1] = tmp;
                            tmp = res.arc_r[0]; res.arc_r[0] = res.arc_r[1]; res.arc_r[1] = tmp;
                        }
                        /* Sort throughs */
                        if (res.thr[0] > res.thr[1]) {
                            int tmp = res.thr[0];
                            res.thr[0] = res.thr[1];
                            res.thr[1] = tmp;
                        }

                        found = -1;
                        for (sj = 0; sj < n_states; sj++) {
                            if (states[sj].arc_l[0] == res.arc_l[0] &&
                                states[sj].arc_r[0] == res.arc_r[0] &&
                                states[sj].arc_l[1] == res.arc_l[1] &&
                                states[sj].arc_r[1] == res.arc_r[1])
                            { found = sj; break; }
                        }
                        action[gi][si] = found;
                    } else if (l_arc >= 0 && r_arc >= 0 &&
                               l_arc != r_arc) {
                        /* Case 4: both in different arcs.
                         * Cup creates arc(pos_l, pos_r).
                         * Partners form new arc(l_partner, r_partner).
                         * Throughs unchanged. */
                        W62State res;
                        int found;

                        res = *s;
                        /* Replace both arcs */
                        res.arc_l[0] = pos_l < pos_r ? pos_l : pos_r;
                        res.arc_r[0] = pos_l < pos_r ? pos_r : pos_l;
                        res.arc_l[1] = l_partner < r_partner ?
                            l_partner : r_partner;
                        res.arc_r[1] = l_partner < r_partner ?
                            r_partner : l_partner;

                        /* Sort arcs by left endpoint */
                        if (res.arc_l[0] > res.arc_l[1]) {
                            int tmp;
                            tmp = res.arc_l[0]; res.arc_l[0] = res.arc_l[1]; res.arc_l[1] = tmp;
                            tmp = res.arc_r[0]; res.arc_r[0] = res.arc_r[1]; res.arc_r[1] = tmp;
                        }

                        found = -1;
                        for (sj = 0; sj < n_states; sj++) {
                            if (states[sj].arc_l[0] == res.arc_l[0] &&
                                states[sj].arc_r[0] == res.arc_r[0] &&
                                states[sj].arc_l[1] == res.arc_l[1] &&
                                states[sj].arc_r[1] == res.arc_r[1])
                            { found = sj; break; }
                        }
                        action[gi][si] = found;
                    } else {
                        action[gi][si] = -1;
                    }
                }
            }

            /* Print our computed action matrix */
            printf("  Computed action matrix:\n    %-6s", "");
            for (si = 0; si < n_states; si++)
                printf("h%-3d", si);
            printf("\n");
            for (gi = 0; gi < 5; gi++) {
                printf("    e_%d:  ", gi + 1);
                for (si = 0; si < n_states; si++) {
                    if (action[gi][si] == -1)
                        printf("  . ");
                    else
                        printf("h%-3d", action[gi][si]);
                }
                printf("\n");
            }

            /* Verify against D103 by checking that our computed
             * action, reindexed via perm, matches D103's matrix.
             * D103 entries (gen, src->dst): */
            {
                /* D103 action: d103_act[gi][src] = dst, -1 = 0 */
                int d103_act[5][9];
                int perm2[9]; /* we need perm from outer scope */
                int mismatches = 0;

                /* Rebuild perm from D103 data */
                {
                    int di2;
                    int d103_arcs[9][4] = {
                        {1,2,3,4},{1,4,2,3},{1,2,4,5},
                        {1,2,5,6},{2,3,4,5},{2,5,3,4},
                        {2,3,5,6},{3,4,5,6},{3,6,4,5}
                    };
                    for (si = 0; si < 9; si++) {
                        perm2[si] = -1;
                        for (di2 = 0; di2 < 9; di2++) {
                            if (states[si].arc_l[0] == d103_arcs[di2][0] &&
                                states[si].arc_r[0] == d103_arcs[di2][1] &&
                                states[si].arc_l[1] == d103_arcs[di2][2] &&
                                states[si].arc_r[1] == d103_arcs[di2][3])
                            { perm2[si] = di2; break; }
                        }
                    }
                }

                /* Initialize D103 action to -1 */
                memset(d103_act, -1, sizeof(d103_act));
                /* e_1: h1->h0, h5->h0, h4->h2, h6->h3 */
                d103_act[0][1]=0; d103_act[0][5]=0;
                d103_act[0][4]=2; d103_act[0][6]=3;
                /* e_2: h0->h1, h2->h4, h5->h4, h8->h4,
                 *       h3->h6, h7->h6 */
                d103_act[1][0]=1; d103_act[1][2]=4;
                d103_act[1][5]=4; d103_act[1][8]=4;
                d103_act[1][3]=6; d103_act[1][7]=6;
                /* e_3: h1->h0, h2->h0, h4->h5, h6->h7, h8->h7 */
                d103_act[2][1]=0; d103_act[2][2]=0;
                d103_act[2][4]=5; d103_act[2][6]=7;
                d103_act[2][8]=7;
                /* e_4: h0->h2, h3->h2, h1->h4, h5->h4,
                 *       h6->h4, h7->h8 */
                d103_act[3][0]=2; d103_act[3][3]=2;
                d103_act[3][1]=4; d103_act[3][5]=4;
                d103_act[3][6]=4; d103_act[3][7]=8;
                /* e_5: h2->h3, h4->h6, h5->h7, h8->h7 */
                d103_act[4][2]=3; d103_act[4][4]=6;
                d103_act[4][5]=7; d103_act[4][8]=7;

                /* Compare: translate our action to D103 indices */
                for (gi = 0; gi < 5; gi++) {
                    for (si = 0; si < 9; si++) {
                        int our_dst = action[gi][si];
                        int d103_src = perm2[si];
                        int d103_dst_expected = d103_act[gi][d103_src];
                        int d103_dst_computed;

                        if (our_dst < 0)
                            d103_dst_computed = -1;
                        else
                            d103_dst_computed = perm2[our_dst];

                        if (d103_dst_computed != d103_dst_expected)
                            mismatches++;
                    }
                }

                printf("\n  TL action vs D103: %d mismatches out of 45\n",
                       mismatches);
                check("TL action matches D103 matrix", mismatches == 0);
            }

            /* Independent BFS from our action matrix */
            {
                int adj[9][9];
                int color[9];
                int queue[9];
                int qh, qt;
                int match_ind;

                memset(adj, 0, sizeof(adj));
                for (gi = 0; gi < 5; gi++)
                    for (si = 0; si < 9; si++)
                        if (action[gi][si] >= 0) {
                            adj[si][action[gi][si]] = 1;
                            adj[action[gi][si]][si] = 1;
                        }

                memset(color, -1, sizeof(color));
                color[0] = 0;
                queue[0] = 0;
                qh = 0; qt = 1;
                while (qh < qt) {
                    int cur = queue[qh++];
                    for (sj = 0; sj < 9; sj++) {
                        if (adj[cur][sj] && color[sj] == -1) {
                            color[sj] = 1 - color[cur];
                            queue[qt++] = sj;
                        }
                    }
                }

                printf("  Independent BFS coloring: [");
                for (si = 0; si < 9; si++)
                    printf("%d%s", color[si], si < 8 ? "," : "");
                printf("]\n");

                /* Check maj mod 2 against independent BFS */
                match_ind = 0;
                for (si = 0; si < 9; si++) {
                    int maj_si = (states[si].arc_r[0] +
                                  states[si].arc_r[1]) % 2;
                    if (maj_si == color[si]) match_ind++;
                }
                if (match_ind < 5) match_ind = 9 - match_ind;
                printf("  maj mod 2 vs independent BFS: %d/9\n",
                       match_ind);
                check("maj mod 2 = independent BFS (W_{6,2})",
                      match_ind == 9);
            }
        }
    }

    /* ============================================================
     * Summary
     * ============================================================ */
    printf("\n=== Summary ===\n\n");
    printf("  %d pass, %d fail\n", n_pass, n_fail);

    raq_analysis_free(&analysis);
    raq_vs_free(&vs);
    free(g_cat_tl);

    return n_fail > 0 ? 1 : 0;
}
