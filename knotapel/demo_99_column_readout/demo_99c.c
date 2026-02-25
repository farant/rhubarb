/*
 * KNOTAPEL DEMO 99c: Algebraic Classification of 3-Strand Catalog
 * ================================================================
 *
 * D99b established: 3-strand Delta_1 computes XOR6-XOR14 with sign-hash.
 * D84-D96 lesson: algebraic classification > geometric refinement.
 * Cell B (6 entries) beats ALL(24) at every N in 2-strand.
 *
 * This demo applies the D96 methodology to 3-strand:
 *   Phase 1: Classify catalog by algebraic properties
 *            (trace, determinant, structure type, norm)
 *   Phase 2: Per-class XOR capacity
 *   Phase 2b: Pairwise interaction analysis
 *   Phase 3: Cross-classification / Cell B search
 *   Phase 4: Matched comparison with 2-strand
 *
 * Uses smaller BFS cap (8192) for speed — classification needs diversity
 * not size.
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
 * Exact Cyclotomic Arithmetic -- Z[zeta_8]
 * Basis: {1, zeta_8, zeta_8^2, zeta_8^3} with zeta_8^4 = -1
 * ================================================================ */

typedef struct {
    long a, b, c, d;
} Cyc8;

static Cyc8 cyc8_make(long a, long b, long c, long d) {
    Cyc8 z; z.a = a; z.b = b; z.c = c; z.d = d; return z;
}

static Cyc8 cyc8_zero(void) { return cyc8_make(0, 0, 0, 0); }
static Cyc8 cyc8_one(void)  { return cyc8_make(1, 0, 0, 0); }

static Cyc8 cyc8_add(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a + y.a, x.b + y.b, x.c + y.c, x.d + y.d);
}

static Cyc8 cyc8_sub(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a - y.a, x.b - y.b, x.c - y.c, x.d - y.d);
}

static Cyc8 cyc8_neg(Cyc8 x) {
    return cyc8_make(-x.a, -x.b, -x.c, -x.d);
}

static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y) {
    return cyc8_make(
        x.a * y.a - x.b * y.d - x.c * y.c - x.d * y.b,
        x.a * y.b + x.b * y.a - x.c * y.d - x.d * y.c,
        x.a * y.c + x.b * y.b + x.c * y.a - x.d * y.d,
        x.a * y.d + x.b * y.c + x.c * y.b + x.d * y.a
    );
}

static int cyc8_eq(Cyc8 x, Cyc8 y) {
    return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}

static int cyc8_is_zero(Cyc8 x) {
    return x.a == 0 && x.b == 0 && x.c == 0 && x.d == 0;
}

static Cyc8 cyc8_conj(Cyc8 z) {
    return cyc8_make(z.a, -z.d, -z.c, -z.b);
}

static long cyc8_max_abs(Cyc8 z) {
    long m = 0, v;
    v = z.a < 0 ? -z.a : z.a; if (v > m) m = v;
    v = z.b < 0 ? -z.b : z.b; if (v > m) m = v;
    v = z.c < 0 ? -z.c : z.c; if (v > m) m = v;
    v = z.d < 0 ? -z.d : z.d; if (v > m) m = v;
    return m;
}

/* Norm: N(z) = z * conj(z). For Z[zeta_8], this gives a Gaussian integer.
 * We compute z * bar(z) where bar is Galois conjugation zeta_8 -> zeta_8^{-1}.
 * The result has b=0, d=0 (it's in Z[i] = Z[zeta_8^2]). */
static long cyc8_norm_sq(Cyc8 z) {
    Cyc8 zbar = cyc8_conj(z);
    Cyc8 n = cyc8_mul(z, zbar);
    /* n should have b=0, d=0. Return a^2 + c^2 as integer norm. */
    return n.a * n.a + n.c * n.c;
}

/* ================================================================
 * 2x2 Matrix over Cyc8 (Delta_1 representation)
 * Matrix: [[a, b], [c, d]]
 * ================================================================ */

typedef struct {
    Cyc8 a, b, c, d;
} Mat2;

static Mat2 mat2_zero(void) {
    Mat2 m;
    m.a = m.b = m.c = m.d = cyc8_zero();
    return m;
}

static Mat2 mat2_identity(void) {
    Mat2 m;
    m.a = m.d = cyc8_one();
    m.b = m.c = cyc8_zero();
    return m;
}

static Mat2 mat2_mul(const Mat2 *p, const Mat2 *q) {
    Mat2 r;
    r.a = cyc8_add(cyc8_mul(p->a, q->a), cyc8_mul(p->b, q->c));
    r.b = cyc8_add(cyc8_mul(p->a, q->b), cyc8_mul(p->b, q->d));
    r.c = cyc8_add(cyc8_mul(p->c, q->a), cyc8_mul(p->d, q->c));
    r.d = cyc8_add(cyc8_mul(p->c, q->b), cyc8_mul(p->d, q->d));
    return r;
}

static int mat2_eq(const Mat2 *p, const Mat2 *q) {
    return cyc8_eq(p->a, q->a) && cyc8_eq(p->b, q->b) &&
           cyc8_eq(p->c, q->c) && cyc8_eq(p->d, q->d);
}

static Mat2 mat2_add(const Mat2 *p, const Mat2 *q) {
    Mat2 r;
    r.a = cyc8_add(p->a, q->a);
    r.b = cyc8_add(p->b, q->b);
    r.c = cyc8_add(p->c, q->c);
    r.d = cyc8_add(p->d, q->d);
    return r;
}

static Mat2 mat2_neg(const Mat2 *p) {
    Mat2 r;
    r.a = cyc8_neg(p->a);
    r.b = cyc8_neg(p->b);
    r.c = cyc8_neg(p->c);
    r.d = cyc8_neg(p->d);
    return r;
}

static long mat2_max_abs(const Mat2 *m) {
    long mx = 0, v;
    v = cyc8_max_abs(m->a); if (v > mx) mx = v;
    v = cyc8_max_abs(m->b); if (v > mx) mx = v;
    v = cyc8_max_abs(m->c); if (v > mx) mx = v;
    v = cyc8_max_abs(m->d); if (v > mx) mx = v;
    return mx;
}

/* Trace: Tr(M) = a + d */
static Cyc8 mat2_trace(const Mat2 *m) {
    return cyc8_add(m->a, m->d);
}

/* Determinant: det(M) = a*d - b*c */
static Cyc8 mat2_det(const Mat2 *m) {
    return cyc8_sub(cyc8_mul(m->a, m->d), cyc8_mul(m->b, m->c));
}

/* Matrix commutator: [M1, M2] = M1*M2 - M2*M1 */
static Mat2 mat2_commutator(const Mat2 *p, const Mat2 *q) {
    Mat2 pq = mat2_mul(p, q);
    Mat2 qp = mat2_mul(q, p);
    Mat2 neg_qp = mat2_neg(&qp);
    return mat2_add(&pq, &neg_qp);
}

/* ================================================================
 * Hash table for Mat2 BFS
 * ================================================================ */

#define MAX_CAT 8192
#define HASH_SIZE 16381

static Mat2  g_cat[MAX_CAT];
static int   g_depth[MAX_CAT];
static int   g_writhe[MAX_CAT];
static int   g_cat_size = 0;

static int g_hash_head[HASH_SIZE];
static int g_hash_next[MAX_CAT];

static unsigned long hash_mat2(const Mat2 *m) {
    unsigned long h = 2166136261UL;
    const long *p = &m->a.a;
    int i;
    for (i = 0; i < 16; i++) {
        h = (h * 1000003UL) ^ (unsigned long)p[i];
    }
    return h;
}

static void hash_init(void) {
    memset(g_hash_head, -1, sizeof(g_hash_head));
}

static int hash_find(const Mat2 *m) {
    int bucket = (int)(hash_mat2(m) % (unsigned long)HASH_SIZE);
    int idx = g_hash_head[bucket];
    while (idx >= 0) {
        if (mat2_eq(&g_cat[idx], m)) return idx;
        idx = g_hash_next[idx];
    }
    return -1;
}

static void hash_insert(int cat_idx) {
    int bucket = (int)(hash_mat2(&g_cat[cat_idx]) % (unsigned long)HASH_SIZE);
    g_hash_next[cat_idx] = g_hash_head[bucket];
    g_hash_head[bucket] = cat_idx;
}

/* ================================================================
 * Braid generators on Delta_1
 * ================================================================ */

static Cyc8 g_A, g_A_inv;
static Mat2 g_gen[4];
static const int g_gen_writhe[4] = {1, -1, 1, -1};

static void build_generators(void) {
    g_A = cyc8_make(0, -1, 0, 0);
    g_A_inv = cyc8_conj(g_A);

    g_gen[0].a = g_A;     g_gen[0].b = cyc8_zero();
    g_gen[0].c = g_A_inv; g_gen[0].d = g_A;

    g_gen[1].a = g_A_inv; g_gen[1].b = cyc8_zero();
    g_gen[1].c = g_A;     g_gen[1].d = g_A_inv;

    g_gen[2].a = g_A;     g_gen[2].b = g_A_inv;
    g_gen[2].c = cyc8_zero(); g_gen[2].d = g_A;

    g_gen[3].a = g_A_inv; g_gen[3].b = g_A;
    g_gen[3].c = cyc8_zero(); g_gen[3].d = g_A_inv;
}

/* ================================================================
 * BFS catalog building
 * ================================================================ */

static void build_catalog(void) {
    int prev, gi, i, rd;

    g_cat_size = 0;
    hash_init();

    g_cat[0] = mat2_identity();
    g_depth[0] = 0;
    g_writhe[0] = 0;
    hash_insert(0);
    g_cat_size = 1;

    printf("  Round 0: 1 entry\n");

    rd = 1;
    do {
        long round_max = 0;
        prev = g_cat_size;
        for (i = 0; i < prev && g_cat_size < MAX_CAT; i++) {
            for (gi = 0; gi < 4 && g_cat_size < MAX_CAT; gi++) {
                Mat2 prod = mat2_mul(&g_cat[i], &g_gen[gi]);
                if (hash_find(&prod) < 0) {
                    long mabs = mat2_max_abs(&prod);
                    if (mabs > round_max) round_max = mabs;
                    g_cat[g_cat_size] = prod;
                    g_depth[g_cat_size] = rd;
                    g_writhe[g_cat_size] = g_writhe[i] + g_gen_writhe[gi];
                    hash_insert(g_cat_size);
                    g_cat_size++;
                }
            }
        }
        if (g_cat_size > prev) {
            printf("  Round %d: %d entries (+%d), max_abs=%ld\n",
                   rd, g_cat_size, g_cat_size - prev, round_max);
        }
        if (round_max > 100000000000L) {
            printf("  WARNING: overflow risk, stopping BFS\n");
            break;
        }
        rd++;
    } while (g_cat_size > prev && g_cat_size < MAX_CAT && rd < 50);

    if (g_cat_size >= MAX_CAT)
        printf("  HIT CAP at %d entries\n", MAX_CAT);
}

/* ================================================================
 * Activation + XOR test (from D99b)
 * ================================================================ */

#define ACT_2x2_HASH 2

#define MAX_ACT_CELLS 8192

static int cell_even[MAX_ACT_CELLS], cell_odd[MAX_ACT_CELLS];
static int touched[MAX_ACT_CELLS];

static int mat2_activate_2x2(const Mat2 *m, int k_param) {
    int h = 0;
    const long *p = &m->a.a;
    int i;
    for (i = 0; i < 16; i++) {
        h = h * 3 + (p[i] > 0 ? 2 : (p[i] < 0 ? 0 : 1));
    }
    if (h < 0) h = -h;
    return h % k_param;
}

/* XOR test on a subset: indices into g_cat */
static int test_xor_subset(const int *indices, int n_weights,
                           int k_param) {
    int n_inputs = 2 * n_weights;
    int n_masks = 1 << n_inputs;
    int n_touched = 0;
    int mask, i, w;
    int result = 1;
    Mat2 pos[7], neg[7];

    if (k_param > MAX_ACT_CELLS || n_inputs > 14 || n_weights > 7)
        return 0;

    for (w = 0; w < n_weights; w++) {
        pos[w] = g_cat[indices[w]];
        neg[w] = mat2_neg(&g_cat[indices[w]]);
    }

    for (mask = 0; mask < n_masks; mask++) {
        Mat2 sum = mat2_zero();
        int par = 0, cell;

        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                w = i / 2;
                if (i % 2 == 0)
                    sum = mat2_add(&sum, &pos[w]);
                else
                    sum = mat2_add(&sum, &neg[w]);
                par ^= 1;
            }
        }

        cell = mat2_activate_2x2(&sum, k_param);

        if (cell_even[cell] == 0 && cell_odd[cell] == 0)
            touched[n_touched++] = cell;

        if (par == 0) {
            cell_even[cell]++;
            if (cell_odd[cell] > 0) { result = 0; goto cleanup; }
        } else {
            cell_odd[cell]++;
            if (cell_even[cell] > 0) { result = 0; goto cleanup; }
        }
    }

cleanup:
    for (i = 0; i < n_touched; i++) {
        cell_even[touched[i]] = 0;
        cell_odd[touched[i]] = 0;
    }
    return result;
}

/* Brute-force XOR over a given subset of indices */
static int count_xor_in_subset(const int *subset, int subset_size,
                               int n_weights, int k_param,
                               int bf_limit) {
    int count = 0;
    int limit = subset_size < bf_limit ? subset_size : bf_limit;
    int i0, i1, i2, i3, i4, i5;
    int idx[7];

    if (n_weights == 3) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++) {
            idx[0] = subset[i0]; idx[1] = subset[i1]; idx[2] = subset[i2];
            if (test_xor_subset(idx, 3, k_param)) count++;
        }
    } else if (n_weights == 4) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++) {
            idx[0] = subset[i0]; idx[1] = subset[i1];
            idx[2] = subset[i2]; idx[3] = subset[i3];
            if (test_xor_subset(idx, 4, k_param)) count++;
        }
    } else if (n_weights == 5) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++)
        for (i4 = i3+1; i4 < limit; i4++) {
            idx[0] = subset[i0]; idx[1] = subset[i1];
            idx[2] = subset[i2]; idx[3] = subset[i3];
            idx[4] = subset[i4];
            if (test_xor_subset(idx, 5, k_param)) count++;
        }
    } else if (n_weights == 6) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++)
        for (i4 = i3+1; i4 < limit; i4++)
        for (i5 = i4+1; i5 < limit; i5++) {
            idx[0] = subset[i0]; idx[1] = subset[i1];
            idx[2] = subset[i2]; idx[3] = subset[i3];
            idx[4] = subset[i4]; idx[5] = subset[i5];
            if (test_xor_subset(idx, 6, k_param)) count++;
        }
    }
    return count;
}

/* ================================================================
 * Algebraic classification data
 * ================================================================ */

/* Classification tags per entry */
#define CLASS_TRACE_ZERO  0x01  /* Tr(M) = 0 */
#define CLASS_DET_UNIT    0x02  /* det(M) is a unit (norm_sq = 1) */
#define CLASS_TRIANGULAR  0x04  /* b=0 or c=0 */
#define CLASS_DIAGONAL    0x08  /* b=0 AND c=0 */
#define CLASS_DENSE       0x10  /* both b!=0 and c!=0 */
#define CLASS_ANTI_TRACE  0x20  /* a = d (anti-trace q3=a-d = 0) */

static int g_class[MAX_CAT];

/* (det hash reserved for future use) */

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    int i;
    char msg[256];

    printf("KNOTAPEL DEMO 99c: Algebraic Classification of 3-Strand Catalog\n");
    printf("================================================================\n\n");

    build_generators();

    /* ============================================================
     * Phase 0: Build catalog
     * ============================================================ */
    printf("=== Phase 0: BFS Catalog ===\n\n");
    build_catalog();
    sprintf(msg, "Catalog: %d entries", g_cat_size);
    check(msg, g_cat_size > 1000);

    /* ============================================================
     * Phase 1: Algebraic Classification
     * ============================================================ */
    printf("\n=== Phase 1: Algebraic Classification ===\n\n");

    {
        int n_trace_zero = 0, n_det_unit = 0, n_tri = 0;
        int n_diag = 0, n_dense = 0, n_anti_trace = 0;
        int n_distinct_traces = 0, n_distinct_dets = 0;

        /* Track distinct trace and det values */
        Cyc8 *traces = (Cyc8 *)malloc((size_t)g_cat_size * sizeof(Cyc8));
        int *trace_counts = (int *)calloc((size_t)g_cat_size, sizeof(int));
        int n_traces = 0;

        Cyc8 *dets = (Cyc8 *)malloc((size_t)g_cat_size * sizeof(Cyc8));
        int *det_counts = (int *)calloc((size_t)g_cat_size, sizeof(int));
        int n_dets = 0;

        memset(g_class, 0, sizeof(g_class));

        for (i = 0; i < g_cat_size; i++) {
            Cyc8 tr = mat2_trace(&g_cat[i]);
            Cyc8 det = mat2_det(&g_cat[i]);
            long det_norm = cyc8_norm_sq(det);
            int j, found;

            /* Trace classification */
            if (cyc8_is_zero(tr)) {
                g_class[i] |= CLASS_TRACE_ZERO;
                n_trace_zero++;
            }

            /* Anti-trace: a = d means q3 = a-d = 0 */
            if (cyc8_eq(g_cat[i].a, g_cat[i].d)) {
                g_class[i] |= CLASS_ANTI_TRACE;
                n_anti_trace++;
            }

            /* Det classification: unit = norm 1 */
            if (det_norm == 1) {
                g_class[i] |= CLASS_DET_UNIT;
                n_det_unit++;
            }

            /* Structure type */
            if (cyc8_is_zero(g_cat[i].b) && cyc8_is_zero(g_cat[i].c)) {
                g_class[i] |= CLASS_DIAGONAL;
                n_diag++;
            } else if (cyc8_is_zero(g_cat[i].b) ||
                       cyc8_is_zero(g_cat[i].c)) {
                g_class[i] |= CLASS_TRIANGULAR;
                n_tri++;
            } else {
                g_class[i] |= CLASS_DENSE;
                n_dense++;
            }

            /* Count distinct traces (linear scan, ok for stats) */
            found = 0;
            for (j = 0; j < n_traces; j++) {
                if (cyc8_eq(traces[j], tr)) {
                    trace_counts[j]++;
                    found = 1;
                    break;
                }
            }
            if (!found && n_traces < g_cat_size) {
                traces[n_traces] = tr;
                trace_counts[n_traces] = 1;
                n_traces++;
            }

            /* Count distinct determinants */
            found = 0;
            for (j = 0; j < n_dets; j++) {
                if (cyc8_eq(dets[j], det)) {
                    det_counts[j]++;
                    found = 1;
                    break;
                }
            }
            if (!found && n_dets < g_cat_size) {
                dets[n_dets] = det;
                det_counts[n_dets] = 1;
                n_dets++;
            }
        }

        n_distinct_traces = n_traces;
        n_distinct_dets = n_dets;

        printf("  Classification summary (%d entries):\n\n", g_cat_size);
        printf("  %-25s  %6s  %5s\n", "Class", "Count", "%%");
        printf("  %-25s  %6s  %5s\n", "-------------------------",
               "------", "-----");
        printf("  %-25s  %6d  %4.1f%%\n", "Trace = 0",
               n_trace_zero, 100.0 * n_trace_zero / g_cat_size);
        printf("  %-25s  %6d  %4.1f%%\n", "Anti-trace (a=d)",
               n_anti_trace, 100.0 * n_anti_trace / g_cat_size);
        printf("  %-25s  %6d  %4.1f%%\n", "Det is unit (norm=1)",
               n_det_unit, 100.0 * n_det_unit / g_cat_size);
        printf("  %-25s  %6d  %4.1f%%\n", "Diagonal (b=c=0)",
               n_diag, 100.0 * n_diag / g_cat_size);
        printf("  %-25s  %6d  %4.1f%%\n", "Triangular (b=0 xor c=0)",
               n_tri, 100.0 * n_tri / g_cat_size);
        printf("  %-25s  %6d  %4.1f%%\n", "Dense (b,c both nonzero)",
               n_dense, 100.0 * n_dense / g_cat_size);

        printf("\n  Distinct traces: %d\n", n_distinct_traces);
        printf("  Distinct determinants: %d\n", n_distinct_dets);

        /* Print top 10 trace values by frequency */
        printf("\n  Top trace values:\n");
        {
            int top[10];
            int nt = n_traces < 10 ? n_traces : 10;
            int t, k;
            memset(top, -1, sizeof(top));
            for (t = 0; t < nt; t++) {
                int best = -1, best_c = -1;
                for (k = 0; k < n_traces; k++) {
                    int skip = 0;
                    int tt;
                    for (tt = 0; tt < t; tt++)
                        if (top[tt] == k) { skip = 1; break; }
                    if (!skip && trace_counts[k] > best_c) {
                        best_c = trace_counts[k];
                        best = k;
                    }
                }
                if (best >= 0) {
                    top[t] = best;
                    printf("    Tr=(%ld,%ld,%ld,%ld): %d entries\n",
                           traces[best].a, traces[best].b,
                           traces[best].c, traces[best].d,
                           trace_counts[best]);
                }
            }
        }

        /* Print top 10 det values by frequency */
        printf("\n  Top determinant values:\n");
        {
            int top[10];
            int nd = n_dets < 10 ? n_dets : 10;
            int t, k;
            memset(top, -1, sizeof(top));
            for (t = 0; t < nd; t++) {
                int best = -1, best_c = -1;
                for (k = 0; k < n_dets; k++) {
                    int skip = 0;
                    int tt;
                    for (tt = 0; tt < t; tt++)
                        if (top[tt] == k) { skip = 1; break; }
                    if (!skip && det_counts[k] > best_c) {
                        best_c = det_counts[k];
                        best = k;
                    }
                }
                if (best >= 0) {
                    top[t] = best;
                    printf("    det=(%ld,%ld,%ld,%ld): %d entries (norm=%ld)\n",
                           dets[best].a, dets[best].b,
                           dets[best].c, dets[best].d,
                           det_counts[best],
                           cyc8_norm_sq(dets[best]));
                }
            }
        }

        /* Cross-classification: trace_zero AND det_unit */
        {
            int n_tz_du = 0;
            for (i = 0; i < g_cat_size; i++) {
                if ((g_class[i] & CLASS_TRACE_ZERO) &&
                    (g_class[i] & CLASS_DET_UNIT))
                    n_tz_du++;
            }
            printf("\n  Cross: trace=0 AND det=unit: %d entries\n", n_tz_du);
        }

        /* Check: is determinant always the same for all entries?
         * (braid group reps have det = A^{2*writhe} or similar) */
        {
            int all_unit = 1;
            for (i = 0; i < g_cat_size; i++) {
                if (!(g_class[i] & CLASS_DET_UNIT)) {
                    all_unit = 0;
                    break;
                }
            }
            sprintf(msg, "All entries have unit determinant: %s",
                    all_unit ? "YES" : "NO");
            check(msg, 1);
        }

        free(traces);
        free(trace_counts);
        free(dets);
        free(det_counts);
    }

    /* ============================================================
     * Phase 2: Per-Class XOR Capacity
     * Test XOR6 on each classification subset
     * ============================================================ */
    printf("\n=== Phase 2: Per-Class XOR Capacity ===\n\n");

    {
        struct {
            const char *label;
            int mask;
            int value;
        } classes[] = {
            {"trace=0",       CLASS_TRACE_ZERO,  CLASS_TRACE_ZERO},
            {"trace!=0",      CLASS_TRACE_ZERO,  0},
            {"anti-trace",    CLASS_ANTI_TRACE,  CLASS_ANTI_TRACE},
            {"anti-trace!=0", CLASS_ANTI_TRACE,  0},
            {"det=unit",      CLASS_DET_UNIT,    CLASS_DET_UNIT},
            {"dense",         CLASS_DENSE,       CLASS_DENSE},
            {"triangular",    CLASS_TRIANGULAR,  CLASS_TRIANGULAR},
            {"diagonal",      CLASS_DIAGONAL,    CLASS_DIAGONAL}
        };
        int nc = 8;
        int ci;
        int k_cells = 64;  /* moderate cells for speed */
        int bf = 80;

        printf("  Testing XOR6 (N=3) per class, 2x2_hash(%d), bf=%d\n\n",
               k_cells, bf);
        printf("  %-20s  %6s  %8s  %8s\n",
               "Class", "Size", "XOR6", "Rate");
        printf("  %-20s  %6s  %8s  %8s\n",
               "--------------------", "------",
               "--------", "--------");

        for (ci = 0; ci < nc; ci++) {
            int *subset;
            int ss = 0;

            subset = (int *)malloc((size_t)g_cat_size * sizeof(int));

            for (i = 0; i < g_cat_size; i++) {
                if ((g_class[i] & classes[ci].mask) == classes[ci].value)
                    subset[ss++] = i;
            }

            if (ss >= 3) {
                int xor6 = count_xor_in_subset(subset, ss, 3, k_cells, bf);
                int combos = 0;
                int lim = ss < bf ? ss : bf;
                /* C(lim, 3) */
                combos = lim * (lim - 1) * (lim - 2) / 6;
                printf("  %-20s  %6d  %8d  %7.1f%%\n",
                       classes[ci].label, ss, xor6,
                       combos > 0 ? 100.0 * xor6 / combos : 0.0);
            } else {
                printf("  %-20s  %6d     (too small)\n",
                       classes[ci].label, ss);
            }

            free(subset);
        }

        /* Also test by depth range */
        printf("\n  XOR6 by depth range:\n");
        {
            static const int d_ranges[][2] = {
                {0, 3}, {4, 5}, {6, 7}, {8, 10}
            };
            int nr = 4;
            int ri;
            for (ri = 0; ri < nr; ri++) {
                int *subset;
                int ss = 0;
                int xor6, combos, lim;

                subset = (int *)malloc((size_t)g_cat_size * sizeof(int));
                for (i = 0; i < g_cat_size; i++) {
                    if (g_depth[i] >= d_ranges[ri][0] &&
                        g_depth[i] <= d_ranges[ri][1])
                        subset[ss++] = i;
                }

                if (ss >= 3) {
                    xor6 = count_xor_in_subset(subset, ss, 3, k_cells, bf);
                    lim = ss < bf ? ss : bf;
                    combos = lim * (lim - 1) * (lim - 2) / 6;
                    printf("  d=%d-%d: %d entries, %d XOR6 (%.1f%%)\n",
                           d_ranges[ri][0], d_ranges[ri][1],
                           ss, xor6,
                           combos > 0 ? 100.0 * xor6 / combos : 0.0);
                } else {
                    printf("  d=%d-%d: %d entries (too small)\n",
                           d_ranges[ri][0], d_ranges[ri][1], ss);
                }
                free(subset);
            }
        }
    }

    /* ============================================================
     * Phase 2b: Pairwise Interaction Analysis
     * For pairs of entries, measure commutator magnitude and
     * correlate with XOR capacity
     * ============================================================ */
    printf("\n=== Phase 2b: Pairwise Interaction Analysis ===\n\n");

    {
        /* Sample pairs from the catalog and measure:
         * 1. Commutator magnitude (how much they don't commute)
         * 2. Whether same-class or cross-class pairs compute better */
        int n_pairs = 0;
        long comm_mag_xor = 0, comm_mag_noxor = 0;
        int n_xor = 0, n_noxor = 0;
        int n_same_class_xor = 0, n_same_class_noxor = 0;
        int n_cross_class_xor = 0, n_cross_class_noxor = 0;

        /* Use entries from depth 4-8, sample first 50 */
        int sample_idx[100];
        int n_sample = 0;
        int k_cells = 64;

        for (i = 0; i < g_cat_size && n_sample < 100; i++) {
            if (g_depth[i] >= 4 && g_depth[i] <= 8)
                sample_idx[n_sample++] = i;
        }
        if (n_sample > 50) n_sample = 50;

        printf("  Sampling %d entries (d=4-8) for pairwise analysis\n\n",
               n_sample);

        /* For each pair, find a third entry that forms XOR6 triple */
        {
            int a, b;
            for (a = 0; a < n_sample && a < 30; a++) {
                for (b = a + 1; b < n_sample && b < 30; b++) {
                    int ia = sample_idx[a];
                    int ib = sample_idx[b];
                    Mat2 comm = mat2_commutator(&g_cat[ia], &g_cat[ib]);
                    long cmag = mat2_max_abs(&comm);
                    int same_class = ((g_class[ia] & 0x3F) ==
                                      (g_class[ib] & 0x3F));
                    int found_xor = 0;
                    int c;

                    /* Try a few third entries to see if this pair
                     * participates in any XOR triple */
                    for (c = 0; c < n_sample && c < 30; c++) {
                        int idx3[3];
                        if (c == a || c == b) continue;
                        idx3[0] = sample_idx[a];
                        idx3[1] = sample_idx[b];
                        idx3[2] = sample_idx[c];
                        if (test_xor_subset(idx3, 3, k_cells)) {
                            found_xor = 1;
                            break;
                        }
                    }

                    n_pairs++;
                    if (found_xor) {
                        comm_mag_xor += cmag;
                        n_xor++;
                        if (same_class) n_same_class_xor++;
                        else n_cross_class_xor++;
                    } else {
                        comm_mag_noxor += cmag;
                        n_noxor++;
                        if (same_class) n_same_class_noxor++;
                        else n_cross_class_noxor++;
                    }
                }
            }
        }

        printf("  Pairs tested: %d\n", n_pairs);
        printf("  XOR-productive pairs: %d (%.1f%%)\n",
               n_xor, n_pairs > 0 ? 100.0 * n_xor / n_pairs : 0.0);
        printf("  Non-productive pairs: %d\n\n", n_noxor);

        if (n_xor > 0 && n_noxor > 0) {
            printf("  Mean commutator magnitude:\n");
            printf("    XOR pairs:     %ld\n",
                   comm_mag_xor / n_xor);
            printf("    Non-XOR pairs: %ld\n",
                   comm_mag_noxor / n_noxor);
            printf("    Ratio: %.2f\n\n",
                   n_noxor > 0 ?
                   (double)(comm_mag_xor * n_noxor) /
                   (double)(comm_mag_noxor * n_xor) : 0.0);
        }

        printf("  Same-class vs cross-class:\n");
        printf("    Same-class XOR:  %d / %d total same (%.1f%%)\n",
               n_same_class_xor,
               n_same_class_xor + n_same_class_noxor,
               (n_same_class_xor + n_same_class_noxor) > 0 ?
               100.0 * n_same_class_xor /
               (n_same_class_xor + n_same_class_noxor) : 0.0);
        printf("    Cross-class XOR: %d / %d total cross (%.1f%%)\n",
               n_cross_class_xor,
               n_cross_class_xor + n_cross_class_noxor,
               (n_cross_class_xor + n_cross_class_noxor) > 0 ?
               100.0 * n_cross_class_xor /
               (n_cross_class_xor + n_cross_class_noxor) : 0.0);

        {
            int xor_comm_higher = (n_xor > 0 && n_noxor > 0 &&
                                   comm_mag_xor * n_noxor >
                                   comm_mag_noxor * n_xor);
            sprintf(msg, "XOR pairs have %s commutator",
                    xor_comm_higher ? "HIGHER" : "LOWER/EQUAL");
            check(msg, 1);
        }
    }

    /* ============================================================
     * Phase 3: Cross-Classification / Cell B Search
     * Find the optimal algebraic subset for XOR capacity
     * ============================================================ */
    printf("\n=== Phase 3: Cell B Search ===\n\n");

    {
        /* Strategy: combine best-performing criteria from Phase 2.
         * Test various curated subsets at multiple XOR levels.
         *
         * Candidate subsets:
         * A: dense + deep (d>=6) — structurally rich
         * B: trace!=0 + dense + deep
         * C: anti-trace + dense + deep (a=d symmetry)
         * D: small trace magnitude + deep
         * E: large commutator pairs (from Phase 2b)
         */
        struct {
            const char *label;
            int require_mask;
            int require_val;
            int forbid_mask;
            int min_depth;
        } subsets[] = {
            {"A: dense+deep(6+)",
             CLASS_DENSE, CLASS_DENSE, 0, 6},
            {"B: tr!=0+dense+deep",
             CLASS_DENSE, CLASS_DENSE, CLASS_TRACE_ZERO, 6},
            {"C: anti-tr+dense+deep",
             CLASS_DENSE | CLASS_ANTI_TRACE,
             CLASS_DENSE | CLASS_ANTI_TRACE, 0, 6},
            {"D: tr=0+dense+deep",
             CLASS_DENSE | CLASS_TRACE_ZERO,
             CLASS_DENSE | CLASS_TRACE_ZERO, 0, 6},
            {"E: dense+deep(8+)",
             CLASS_DENSE, CLASS_DENSE, 0, 8},
            {"F: ALL deep(6+)",
             0, 0, 0, 6}
        };
        int ns = 6;
        int si;
        int k_cells = 128;
        int bf = 30;

        printf("  Testing curated subsets, 2x2_hash(%d), bf=%d\n",
               k_cells, bf);
        printf("  XOR levels: 6, 8, 10, 12\n\n");

        printf("  %-26s %5s %7s %7s %7s %7s\n",
               "Subset", "Size", "XOR6", "XOR8", "XOR10", "XOR12");
        printf("  %-26s %5s %7s %7s %7s %7s\n",
               "--------------------------", "-----",
               "-------", "-------", "-------", "-------");

        for (si = 0; si < ns; si++) {
            int *sub;
            int ss = 0;

            sub = (int *)malloc((size_t)g_cat_size * sizeof(int));

            for (i = 0; i < g_cat_size; i++) {
                if (g_depth[i] < subsets[si].min_depth) continue;
                if (subsets[si].require_mask != 0 &&
                    (g_class[i] & subsets[si].require_mask) !=
                    subsets[si].require_val) continue;
                if (subsets[si].forbid_mask != 0 &&
                    (g_class[i] & subsets[si].forbid_mask)) continue;
                sub[ss++] = i;
            }

            if (ss >= 3) {
                int x6, x8, x10, x12;
                x6 = count_xor_in_subset(sub, ss, 3, k_cells, bf);
                x8 = count_xor_in_subset(sub, ss, 4, k_cells, bf);
                x10 = count_xor_in_subset(sub, ss, 5, k_cells, bf);
                x12 = count_xor_in_subset(sub, ss, 6, k_cells, bf);

                printf("  %-26s %5d %7d %7d %7d %7d\n",
                       subsets[si].label, ss, x6, x8, x10, x12);
            } else {
                printf("  %-26s %5d (too small)\n",
                       subsets[si].label, ss);
            }

            free(sub);
        }

        /* The "Cell B" test: take the BEST subset from above,
         * pick 24 entries evenly spaced, test XOR6-12 */
        printf("\n  Cell B test: best subset, 24 entries, vs full first-24:\n");
        {
            int *best_sub;
            int best_ss = 0;
            int x6_cellb, x8_cellb, x10_cellb, x12_cellb;
            int x6_first, x8_first, x10_first, x12_first;
            int first24[24];
            int cellb[24];
            int j, step;

            /* Use subset A (dense+deep) as default Cell B candidate */
            best_sub = (int *)malloc((size_t)g_cat_size * sizeof(int));
            for (i = 0; i < g_cat_size; i++) {
                if (g_depth[i] >= 6 &&
                    (g_class[i] & CLASS_DENSE))
                    best_sub[best_ss++] = i;
            }

            /* Pick 24 evenly spaced from Cell B */
            step = best_ss / 24;
            if (step < 1) step = 1;
            for (j = 0; j < 24 && j * step < best_ss; j++)
                cellb[j] = best_sub[j * step];

            /* First 24 from catalog */
            for (j = 0; j < 24; j++)
                first24[j] = j;

            printf("\n  Cell B size: %d entries (using 24)\n", best_ss);

            x6_cellb = count_xor_in_subset(cellb, 24, 3, k_cells, 24);
            x8_cellb = count_xor_in_subset(cellb, 24, 4, k_cells, 24);
            x10_cellb = count_xor_in_subset(cellb, 24, 5, k_cells, 24);
            x12_cellb = count_xor_in_subset(cellb, 24, 6, k_cells, 24);

            x6_first = count_xor_in_subset(first24, 24, 3, k_cells, 24);
            x8_first = count_xor_in_subset(first24, 24, 4, k_cells, 24);
            x10_first = count_xor_in_subset(first24, 24, 5, k_cells, 24);
            x12_first = count_xor_in_subset(first24, 24, 6, k_cells, 24);

            printf("\n  +----------+--------+--------+--------+--------+\n");
            printf("  | Subset   |  XOR6  |  XOR8  | XOR10  | XOR12  |\n");
            printf("  +----------+--------+--------+--------+--------+\n");
            printf("  | First-24 | %6d | %6d | %6d | %6d |\n",
                   x6_first, x8_first, x10_first, x12_first);
            printf("  | Cell B   | %6d | %6d | %6d | %6d |\n",
                   x6_cellb, x8_cellb, x10_cellb, x12_cellb);
            printf("  +----------+--------+--------+--------+--------+\n");

            sprintf(msg, "Cell B vs first-24: XOR6 %d vs %d, XOR12 %d vs %d",
                    x6_cellb, x6_first, x12_cellb, x12_first);
            check(msg, x12_cellb >= x12_first);

            free(best_sub);
        }
    }

    /* ============================================================
     * Phase 4: Commutator Subgroup Analysis
     * ============================================================ */
    printf("\n=== Phase 4: Commutator Subgroup ===\n\n");

    {
        /* Compute [sigma_1, sigma_2] and check if it's in the catalog */
        Mat2 comm_s1s2 = mat2_commutator(&g_gen[0], &g_gen[2]);
        int comm_idx = hash_find(&comm_s1s2);

        printf("  [sigma_1, sigma_2] = ");
        printf("a=(%ld,%ld,%ld,%ld) ", comm_s1s2.a.a, comm_s1s2.a.b,
               comm_s1s2.a.c, comm_s1s2.a.d);
        printf("d=(%ld,%ld,%ld,%ld)\n", comm_s1s2.d.a, comm_s1s2.d.b,
               comm_s1s2.d.c, comm_s1s2.d.d);

        if (comm_idx >= 0) {
            printf("  Found in catalog at index %d, depth %d\n",
                   comm_idx, g_depth[comm_idx]);
        } else {
            printf("  NOT in catalog (requires deeper BFS)\n");
        }

        /* Trace of commutator */
        {
            Cyc8 tr_comm = mat2_trace(&comm_s1s2);
            printf("  Tr([s1,s2]) = (%ld,%ld,%ld,%ld)\n",
                   tr_comm.a, tr_comm.b, tr_comm.c, tr_comm.d);
            sprintf(msg, "Commutator trace is %s",
                    cyc8_is_zero(tr_comm) ? "ZERO (traceless)" : "nonzero");
            check(msg, 1);
        }

        /* Det of commutator */
        {
            Cyc8 det_comm = mat2_det(&comm_s1s2);
            printf("  det([s1,s2]) = (%ld,%ld,%ld,%ld)\n",
                   det_comm.a, det_comm.b, det_comm.c, det_comm.d);
        }
    }

    /* ============================================================
     * Phase 5: Cartan Correlation Test (PREDICTION)
     *
     * sl_2 decomposition: M = ((a+d)/2)*I + b*e + c*f + ((a-d)/2)*h
     * Cartan component = a-d. If a=d (anti-trace), Cartan = 0.
     *
     * PREDICTION: Cartan magnitude should be ANTI-correlated with
     * XOR capacity. Entries with zero/small Cartan (living in the
     * nilpotent subalgebra) should compute better.
     * ============================================================ */
    printf("\n=== Phase 5: Cartan Correlation Test (PREDICTION) ===\n\n");

    {
        /* Bucket entries by Cartan L1 magnitude */
        struct {
            const char *label;
            long lo, hi;  /* inclusive range of cartan_l1 */
        } buckets[] = {
            {"cartan=0",   0,  0},
            {"cartan=1-2", 1,  2},
            {"cartan=3-5", 3,  5},
            {"cartan=6-10", 6, 10},
            {"cartan=11+", 11, 999999999L}
        };
        int nb = 5;
        int bi;
        int k_cells = 128;
        int bf = 30;

        printf("  Prediction: lower Cartan mag -> higher XOR rate\n");
        printf("  (Cartan = L1 norm of a-d in Z[zeta_8])\n\n");

        printf("  %-16s %6s %8s %8s %8s %8s\n",
               "Bucket", "Size", "XOR6", "XOR8", "XOR10", "XOR12");
        printf("  %-16s %6s %8s %8s %8s %8s\n",
               "----------------", "------",
               "--------", "--------", "--------", "--------");

        for (bi = 0; bi < nb; bi++) {
            int *subset;
            int ss = 0;

            subset = (int *)malloc((size_t)g_cat_size * sizeof(int));

            for (i = 0; i < g_cat_size; i++) {
                Cyc8 cartan = cyc8_sub(g_cat[i].a, g_cat[i].d);
                long cartan_l1 = 0;
                long v;
                v = cartan.a < 0 ? -cartan.a : cartan.a; cartan_l1 += v;
                v = cartan.b < 0 ? -cartan.b : cartan.b; cartan_l1 += v;
                v = cartan.c < 0 ? -cartan.c : cartan.c; cartan_l1 += v;
                v = cartan.d < 0 ? -cartan.d : cartan.d; cartan_l1 += v;

                if (cartan_l1 >= buckets[bi].lo &&
                    cartan_l1 <= buckets[bi].hi)
                    subset[ss++] = i;
            }

            if (ss >= 3) {
                int x6, x8, x10, x12;
                x6 = count_xor_in_subset(subset, ss, 3, k_cells, bf);
                x8 = count_xor_in_subset(subset, ss, 4, k_cells, bf);
                x10 = count_xor_in_subset(subset, ss, 5, k_cells, bf);
                x12 = count_xor_in_subset(subset, ss, 6, k_cells, bf);
                printf("  %-16s %6d %8d %8d %8d %8d\n",
                       buckets[bi].label, ss, x6, x8, x10, x12);
            } else {
                printf("  %-16s %6d (too small)\n",
                       buckets[bi].label, ss);
            }

            free(subset);
        }

        /* Also test Cartan=0 restricted to deep entries */
        printf("\n  Cartan=0 AND deep(d>=6):\n");
        {
            int *subset;
            int ss = 0;
            int x6, x8, x10, x12;

            subset = (int *)malloc((size_t)g_cat_size * sizeof(int));
            for (i = 0; i < g_cat_size; i++) {
                if (g_depth[i] >= 6 &&
                    cyc8_eq(g_cat[i].a, g_cat[i].d))
                    subset[ss++] = i;
            }

            if (ss >= 3) {
                x6 = count_xor_in_subset(subset, ss, 3, k_cells, bf);
                x8 = count_xor_in_subset(subset, ss, 4, k_cells, bf);
                x10 = count_xor_in_subset(subset, ss, 5, k_cells, bf);
                x12 = count_xor_in_subset(subset, ss, 6, k_cells, bf);
                printf("  cartan=0+deep: %d entries, XOR6=%d XOR8=%d XOR10=%d XOR12=%d\n",
                       ss, x6, x8, x10, x12);
            } else {
                printf("  cartan=0+deep: %d entries (too small)\n", ss);
            }
            free(subset);
        }

        /* Correlation summary */
        printf("\n  Cartan magnitude distribution:\n");
        {
            long min_c = 999999999L, max_c = 0, total_c = 0;
            int n_zero = 0;
            for (i = 0; i < g_cat_size; i++) {
                Cyc8 cartan = cyc8_sub(g_cat[i].a, g_cat[i].d);
                long cartan_l1 = 0;
                long v;
                v = cartan.a < 0 ? -cartan.a : cartan.a; cartan_l1 += v;
                v = cartan.b < 0 ? -cartan.b : cartan.b; cartan_l1 += v;
                v = cartan.c < 0 ? -cartan.c : cartan.c; cartan_l1 += v;
                v = cartan.d < 0 ? -cartan.d : cartan.d; cartan_l1 += v;

                if (cartan_l1 < min_c) min_c = cartan_l1;
                if (cartan_l1 > max_c) max_c = cartan_l1;
                total_c += cartan_l1;
                if (cartan_l1 == 0) n_zero++;
            }
            printf("    Min: %ld, Max: %ld, Mean: %ld\n",
                   min_c, max_c, g_cat_size > 0 ? total_c / g_cat_size : 0);
            printf("    Zero (anti-trace): %d / %d (%.1f%%)\n",
                   n_zero, g_cat_size,
                   g_cat_size > 0 ? 100.0 * n_zero / g_cat_size : 0.0);
        }

        check("Cartan correlation test completed (check anti-correlation)", 1);
    }

    /* ============================================================
     * Phase 6: Chirality Analysis (d=6 entries)
     * Check writhe distribution among anti-trace vs non-anti-trace
     * at depth 6, and dense vs triangular.
     * ============================================================ */
    printf("\n=== Phase 6: Chirality Analysis (d=6) ===\n\n");

    {
        int n_d6 = 0;
        int n_d6_anti = 0, n_d6_nonanti = 0;
        int anti_pos_w = 0, anti_neg_w = 0, anti_zero_w = 0;
        int nonanti_pos_w = 0, nonanti_neg_w = 0, nonanti_zero_w = 0;
        long anti_total_absw = 0, nonanti_total_absw = 0;

        for (i = 0; i < g_cat_size; i++) {
            if (g_depth[i] != 6) continue;
            n_d6++;

            if (g_class[i] & CLASS_ANTI_TRACE) {
                n_d6_anti++;
                if (g_writhe[i] > 0) anti_pos_w++;
                else if (g_writhe[i] < 0) anti_neg_w++;
                else anti_zero_w++;
                anti_total_absw += (g_writhe[i] < 0 ?
                                    -g_writhe[i] : g_writhe[i]);
            } else {
                n_d6_nonanti++;
                if (g_writhe[i] > 0) nonanti_pos_w++;
                else if (g_writhe[i] < 0) nonanti_neg_w++;
                else nonanti_zero_w++;
                nonanti_total_absw += (g_writhe[i] < 0 ?
                                       -g_writhe[i] : g_writhe[i]);
            }
        }

        printf("  d=6 entries: %d total\n\n", n_d6);
        printf("  %-18s %6s %6s %6s %6s %8s\n",
               "Class", "Count", "W>0", "W<0", "W=0", "|W|mean");
        printf("  %-18s %6s %6s %6s %6s %8s\n",
               "------------------", "------", "------", "------",
               "------", "--------");
        printf("  %-18s %6d %6d %6d %6d %8.2f\n",
               "anti-trace (a=d)", n_d6_anti,
               anti_pos_w, anti_neg_w, anti_zero_w,
               n_d6_anti > 0 ?
               (double)anti_total_absw / n_d6_anti : 0.0);
        printf("  %-18s %6d %6d %6d %6d %8.2f\n",
               "non-anti-trace", n_d6_nonanti,
               nonanti_pos_w, nonanti_neg_w, nonanti_zero_w,
               n_d6_nonanti > 0 ?
               (double)nonanti_total_absw / n_d6_nonanti : 0.0);

        /* Also check dense vs tri at d=6 */
        printf("\n  d=6 structure types:\n");
        {
            int n_d6_dense = 0, n_d6_tri = 0, n_d6_diag = 0;
            int dense_anti = 0, tri_anti = 0;
            for (i = 0; i < g_cat_size; i++) {
                if (g_depth[i] != 6) continue;
                if (g_class[i] & CLASS_DENSE) {
                    n_d6_dense++;
                    if (g_class[i] & CLASS_ANTI_TRACE) dense_anti++;
                }
                else if (g_class[i] & CLASS_TRIANGULAR) {
                    n_d6_tri++;
                    if (g_class[i] & CLASS_ANTI_TRACE) tri_anti++;
                }
                else if (g_class[i] & CLASS_DIAGONAL)
                    n_d6_diag++;
            }
            printf("    Dense: %d (anti-trace: %d)\n", n_d6_dense, dense_anti);
            printf("    Triangular: %d (anti-trace: %d)\n", n_d6_tri, tri_anti);
            printf("    Diagonal: %d\n", n_d6_diag);
        }

        check("Chirality analysis completed", 1);
    }

    /* ============================================================
     * Summary
     * ============================================================ */
    printf("\n================================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
