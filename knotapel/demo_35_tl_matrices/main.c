/*
 * KNOTAPEL DEMO 35: TL_n Matrix Representations over Z[zeta_8]
 * =============================================================
 *
 * THESIS: Generalize Demo 22's hardcoded TL_3 (5x5 matrices over Cx)
 * to parametric TL_n over exact Z[zeta_8] arithmetic, with closure
 * functionals.
 *
 * Part A: Cyc8 arithmetic (exact Z[zeta_8], from Demo 29)
 * Part B: Planar matching enumeration (C_n non-crossing matchings)
 * Part C: TL generator matrices e_i (diagram composition)
 * Part D: Braid generators sigma_i = A*I + A^{-1}*e_i
 * Part E: Braid word evaluation (matrix product chain)
 * Part F: Closure functionals (trace and plat)
 *
 * Key design decisions:
 *   - Cyc8 coefficients (exact integer arithmetic, not floating-point)
 *   - Planar matching basis (C_n x C_n matrices, not 2^n x 2^n tensor)
 *   - Left regular representation
 *   - Verify against Demo 01 (state-sum), Demo 22 (TL_3), Demo 24 (TL_2)
 *
 * Theoretical basis:
 *   TL_n at delta=0: e_i^2 = 0 (nilpotent). Non-semisimple.
 *   Dimension = C_n (nth Catalan number). C_2=2, C_3=5, C_4=14.
 *   Braid generators: sigma_i = A*I + A^{-1}*e_i where A = e^{i*5*pi/4}
 *
 * C89, zero dependencies beyond stdio/stdlib/string/math.
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

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * Complex arithmetic (double precision, for cross-checking)
 * ================================================================ */

typedef struct { double re, im; } Cx;

static Cx cx_make(double re, double im) { Cx z; z.re = re; z.im = im; return z; }
/* cx_add will be used in Part C+ */
/* static Cx cx_add(Cx a, Cx b) { return cx_make(a.re + b.re, a.im + b.im); } */
static Cx cx_sub(Cx a, Cx b) { return cx_make(a.re - b.re, a.im - b.im); }
static Cx cx_mul(Cx a, Cx b) {
    return cx_make(a.re * b.re - a.im * b.im,
                   a.re * b.im + a.im * b.re);
}
static double cx_abs(Cx a) { return sqrt(a.re * a.re + a.im * a.im); }
static Cx cx_exp_i(double theta) { return cx_make(cos(theta), sin(theta)); }

/* ================================================================
 * PART A: Exact Cyclotomic Arithmetic — Z[zeta_8]
 *
 * Basis: {1, zeta_8, zeta_8^2, zeta_8^3} with zeta_8^4 = -1
 *
 * Element: a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3
 *   where zeta_8 = e^{i*pi/4} = (1+i)/sqrt(2)
 *         zeta_8^2 = i
 *         zeta_8^3 = (-1+i)/sqrt(2)
 *
 * Multiplication: (a,b,c,d)*(e,f,g,h) with zeta_8^4 = -1
 * 16 integer muls, 12 adds/subs. Exact.
 * ================================================================ */

typedef struct {
    long a, b, c, d;  /* coefficients in basis {1, zeta_8, zeta_8^2, zeta_8^3} */
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
    /* (a,b,c,d)*(e,f,g,h) with zeta_8^4 = -1 */
    return cyc8_make(
        x.a * y.a - x.b * y.d - x.c * y.c - x.d * y.b,
        x.a * y.b + x.b * y.a - x.c * y.d - x.d * y.c,
        x.a * y.c + x.b * y.b + x.c * y.a - x.d * y.d,
        x.a * y.d + x.b * y.c + x.c * y.b + x.d * y.a
    );
}

static Cyc8 cyc8_scale(Cyc8 x, long s) {
    return cyc8_make(x.a * s, x.b * s, x.c * s, x.d * s);
}

static int cyc8_eq(Cyc8 x, Cyc8 y) {
    return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}

/* Convert to complex double for comparison */
static Cx cyc8_to_cx(Cyc8 z) {
    double inv_sqrt2 = 1.0 / sqrt(2.0);
    double re = (double)z.a + (double)z.b * inv_sqrt2 + (double)z.d * (-inv_sqrt2);
    double im = (double)z.b * inv_sqrt2 + (double)z.c + (double)z.d * inv_sqrt2;
    return cx_make(re, im);
}

/* Complex conjugation: conj(a,b,c,d) = (a, -d, -c, -b) */
static Cyc8 cyc8_conj(Cyc8 z) {
    return cyc8_make(z.a, -z.d, -z.c, -z.b);
}

static void cyc8_print(const char *label, Cyc8 z) {
    printf("  %s = (%ld, %ld, %ld, %ld)", label, z.a, z.b, z.c, z.d);
    {
        Cx c = cyc8_to_cx(z);
        printf("  [%.4f + %.4fi]", c.re, c.im);
    }
    printf("\n");
}

/* ================================================================
 * PART B: Planar Matching Enumeration
 *
 * A planar matching (non-crossing perfect matching) on 2n boundary
 * points of an n-strand TL diagram.
 *
 * Points:
 *   0..n-1   = top strand endpoints (left to right)
 *   n..2n-1  = bottom strand endpoints (left to right)
 *
 * Boundary ordering (going around the rectangle):
 *   top_0, top_1, ..., top_{n-1}, bot_{n-1}, bot_{n-2}, ..., bot_0
 *
 * Algorithm: segment-based recursive enumeration.
 *   1. Start with one segment = all 2n boundary points
 *   2. Pick first point of first non-empty segment
 *   3. Try pairing with segment[j] for each odd j
 *      (odd ensures interior has even count)
 *   4. Each pairing splits the segment into interior (1..j-1)
 *      and exterior (j+1..count-1) — two independent sub-segments
 *   5. Recurse with updated segment list
 *   6. When all segments empty, save completed matching
 *
 * This correctly enforces the non-crossing constraint because
 * each pairing chord divides remaining points into independent
 * arcs (segments), and points in different segments can never
 * be paired together.
 *
 * The number of matchings is the nth Catalan number C_n.
 *   C_2 = 2, C_3 = 5, C_4 = 14, C_5 = 42
 * ================================================================ */

#define MAX_N 5
#define MAX_2N 10
#define MAX_BASIS 42  /* C_5 = 42 */
#define MAX_SEGS 10   /* at most ~n segments at any recursion level */

typedef struct {
    int match[MAX_2N];  /* match[i] = partner of point i */
} PlanarMatch;

typedef struct {
    int points[MAX_2N];
    int count;
} Segment;

/* Build boundary-order array for n strands */
static void build_boundary_order(int n, int *bp) {
    int i;
    /* Top: left to right */
    for (i = 0; i < n; i++)
        bp[i] = i;
    /* Bottom: right to left (going around the rectangle) */
    for (i = 0; i < n; i++)
        bp[n + i] = 2 * n - 1 - i;
}

/* Recursive segment-based enumeration of non-crossing matchings */
static void enum_segments(Segment *segs, int n_segs, int *match_buf,
                          PlanarMatch *basis, int *num_basis, int n) {
    int s, j, k;
    int first_seg;
    Segment new_segs[MAX_SEGS];
    int new_n;
    int *pts;
    int cnt;

    /* Find first non-empty segment */
    first_seg = -1;
    for (s = 0; s < n_segs; s++) {
        if (segs[s].count > 0) { first_seg = s; break; }
    }
    if (first_seg == -1) {
        /* All segments empty — complete matching found */
        if (*num_basis < MAX_BASIS) {
            memcpy(basis[*num_basis].match, match_buf,
                   (size_t)(2 * n) * sizeof(int));
            (*num_basis)++;
        }
        return;
    }

    pts = segs[first_seg].points;
    cnt = segs[first_seg].count;

    /* Pair pts[0] with pts[j] for each odd j */
    for (j = 1; j < cnt; j += 2) {
        match_buf[pts[0]] = pts[j];
        match_buf[pts[j]] = pts[0];

        /* Build new segment list:
         * - segments before first_seg: unchanged
         * - first_seg splits into interior (1..j-1) and exterior (j+1..cnt-1)
         * - segments after first_seg: unchanged
         */
        new_n = 0;
        for (k = 0; k < n_segs; k++) {
            if (k == first_seg) {
                /* Interior segment: pts[1..j-1] */
                if (j > 1) {
                    memcpy(new_segs[new_n].points, &pts[1],
                           (size_t)(j - 1) * sizeof(int));
                    new_segs[new_n].count = j - 1;
                    new_n++;
                }
                /* Exterior segment: pts[j+1..cnt-1] */
                if (cnt - j - 1 > 0) {
                    memcpy(new_segs[new_n].points, &pts[j + 1],
                           (size_t)(cnt - j - 1) * sizeof(int));
                    new_segs[new_n].count = cnt - j - 1;
                    new_n++;
                }
            } else {
                new_segs[new_n] = segs[k];
                new_n++;
            }
        }

        enum_segments(new_segs, new_n, match_buf, basis, num_basis, n);
    }
}

/* Top-level enumeration: returns number of matchings (should be C_n) */
static int enumerate_basis(int n, PlanarMatch *basis) {
    Segment segs[1];
    int match_buf[MAX_2N];
    int num_basis = 0;

    build_boundary_order(n, segs[0].points);
    segs[0].count = 2 * n;
    memset(match_buf, -1, sizeof(match_buf));

    enum_segments(segs, 1, match_buf, basis, &num_basis, n);

    return num_basis;
}

/* Pretty-print a matching */
static void print_matching(const PlanarMatch *m, int n) {
    int i;
    printf("    {");
    for (i = 0; i < 2 * n; i++) {
        if (i > 0) printf(",");
        printf("%d", m->match[i]);
    }
    printf("}");

    /* Also show as pairs */
    printf("  pairs:");
    for (i = 0; i < 2 * n; i++) {
        if (m->match[i] > i)
            printf(" (%d-%d)", i, m->match[i]);
    }
    printf("\n");
}

/* Check if a matching is the identity: match[i] = i + n for i < n */
static int is_identity_matching(const PlanarMatch *m, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (m->match[i] != i + n) return 0;
    }
    return 1;
}

/* ================================================================
 * PART C: TL Generator Matrices via Diagram Composition
 *
 * TL_n matrix: C_n x C_n matrix of Cyc8 entries.
 * Generator e_i maps each basis diagram to e_i composed with it.
 * At delta=0: entries are 0 or 1 (loops kill the term).
 * ================================================================ */

#define MAX_DIM 42  /* C_5 = 42, sufficient for n <= 5 */

typedef struct {
    Cyc8 entries[MAX_DIM][MAX_DIM]; /* entries[row][col] */
    int dim;
} TLMatrix;

static TLMatrix tlmat_zero(int dim) {
    TLMatrix m;
    int i, j;
    m.dim = dim;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            m.entries[i][j] = cyc8_zero();
    return m;
}

static TLMatrix tlmat_identity(int dim) {
    TLMatrix m;
    int i;
    m = tlmat_zero(dim);
    for (i = 0; i < dim; i++)
        m.entries[i][i] = cyc8_one();
    return m;
}

static TLMatrix tlmat_add(const TLMatrix *a, const TLMatrix *b) {
    TLMatrix m;
    int i, j;
    m.dim = a->dim;
    for (i = 0; i < m.dim; i++)
        for (j = 0; j < m.dim; j++)
            m.entries[i][j] = cyc8_add(a->entries[i][j], b->entries[i][j]);
    return m;
}

static TLMatrix tlmat_scale(Cyc8 s, const TLMatrix *a) {
    TLMatrix m;
    int i, j;
    m.dim = a->dim;
    for (i = 0; i < m.dim; i++)
        for (j = 0; j < m.dim; j++)
            m.entries[i][j] = cyc8_mul(s, a->entries[i][j]);
    return m;
}

static TLMatrix tlmat_mul(const TLMatrix *a, const TLMatrix *b) {
    TLMatrix m;
    int i, j, k;
    m.dim = a->dim;
    for (i = 0; i < m.dim; i++)
        for (j = 0; j < m.dim; j++) {
            m.entries[i][j] = cyc8_zero();
            for (k = 0; k < m.dim; k++)
                m.entries[i][j] = cyc8_add(m.entries[i][j],
                    cyc8_mul(a->entries[i][k], b->entries[k][j]));
        }
    return m;
}

static int tlmat_eq(const TLMatrix *a, const TLMatrix *b) {
    int i, j;
    if (a->dim != b->dim) return 0;
    for (i = 0; i < a->dim; i++)
        for (j = 0; j < a->dim; j++)
            if (!cyc8_eq(a->entries[i][j], b->entries[i][j])) return 0;
    return 1;
}

static int tlmat_is_zero(const TLMatrix *m) {
    int i, j;
    for (i = 0; i < m->dim; i++)
        for (j = 0; j < m->dim; j++)
            if (!cyc8_eq(m->entries[i][j], cyc8_zero())) return 0;
    return 1;
}

static int tlmat_nonzero_count(const TLMatrix *m) {
    int i, j, count = 0;
    for (i = 0; i < m->dim; i++)
        for (j = 0; j < m->dim; j++)
            if (!cyc8_eq(m->entries[i][j], cyc8_zero())) count++;
    return count;
}

static void tlmat_print(const TLMatrix *m, const char *label) {
    int i, j;
    printf("  %s (%dx%d, %d nonzero):\n", label, m->dim, m->dim,
           tlmat_nonzero_count(m));
    for (i = 0; i < m->dim; i++)
        for (j = 0; j < m->dim; j++)
            if (!cyc8_eq(m->entries[i][j], cyc8_zero()))
                printf("    [%d][%d] = (%ld,%ld,%ld,%ld)\n", i, j,
                       m->entries[i][j].a, m->entries[i][j].b,
                       m->entries[i][j].c, m->entries[i][j].d);
}

/* ----------------------------------------------------------------
 * Diagram composition: stack d1 (top) on d2 (bottom)
 *
 * D1 top (0..n-1) = external top of result
 * D1 bottom (n..2n-1) glued to D2 top (0..n-1)
 * D2 bottom (n..2n-1) = external bottom of result
 *
 * Returns number of closed loops formed at the glue.
 * At delta=0, any loops mean the result is zero.
 * ---------------------------------------------------------------- */

static int compose_diagrams(int n, const PlanarMatch *d1, const PlanarMatch *d2,
                            PlanarMatch *result) {
    int glue_visited[MAX_N];
    int i, loops;

    memset(result->match, -1, (size_t)(2 * n) * sizeof(int));
    memset(glue_visited, 0, (size_t)n * sizeof(int));
    loops = 0;

    /* Phase 1: trace paths from each external point */
    for (i = 0; i < 2 * n; i++) {
        int start = i;
        int in_d1;  /* 1 = currently in D1, 0 = in D2 */
        int cur;    /* current point index (0..2n-1) within current diagram */
        int partner;

        if (result->match[i] >= 0) continue; /* already paired */

        if (i < n) {
            in_d1 = 1; cur = i;           /* start at D1 top */
        } else {
            in_d1 = 0; cur = i;           /* start at D2 bottom */
        }

        /* Bounce between diagrams until we hit another external point */
        for (;;) {
            if (in_d1) {
                partner = d1->match[cur];
                if (partner < n) {
                    /* Reached D1 top = external */
                    result->match[start] = partner;
                    result->match[partner] = start;
                    break;
                }
                /* D1 bottom → cross glue to D2 top */
                glue_visited[partner - n] = 1;
                in_d1 = 0;
                cur = partner - n;
            } else {
                partner = d2->match[cur];
                if (partner >= n) {
                    /* Reached D2 bottom = external */
                    result->match[start] = partner;
                    result->match[partner] = start;
                    break;
                }
                /* D2 top → cross glue to D1 bottom */
                glue_visited[partner] = 1;
                in_d1 = 1;
                cur = n + partner;
            }
        }
    }

    /* Phase 2: count closed loops from unvisited glue points */
    for (i = 0; i < n; i++) {
        int cur, p, q;
        if (glue_visited[i]) continue;

        loops++;
        cur = i;
        do {
            glue_visited[cur] = 1;
            p = d2->match[cur];         /* D2 top → D2 top (internal) */
            glue_visited[p] = 1;
            q = d1->match[n + p];       /* D1 bottom → D1 bottom (internal) */
            cur = q - n;
        } while (cur != i);
    }

    return loops;
}

/* ----------------------------------------------------------------
 * Generator and identity diagram construction
 * ---------------------------------------------------------------- */

static PlanarMatch make_identity_diagram(int n) {
    PlanarMatch m;
    int k;
    for (k = 0; k < n; k++) {
        m.match[k] = n + k;
        m.match[n + k] = k;
    }
    return m;
}

static PlanarMatch make_generator_diagram(int n, int gen) {
    PlanarMatch m;
    int k;
    /* Through-strands for everything */
    for (k = 0; k < n; k++) {
        m.match[k] = n + k;
        m.match[n + k] = k;
    }
    /* Cap on top: gen <-> gen+1 */
    m.match[gen] = gen + 1;
    m.match[gen + 1] = gen;
    /* Cup on bottom: (n+gen) <-> (n+gen+1) */
    m.match[n + gen] = n + gen + 1;
    m.match[n + gen + 1] = n + gen;
    return m;
}

/* Find which basis element a matching corresponds to (-1 if not found) */
static int find_basis_index(const PlanarMatch *m, const PlanarMatch *basis,
                            int num_basis, int n) {
    int i, j, eq;
    for (i = 0; i < num_basis; i++) {
        eq = 1;
        for (j = 0; j < 2 * n; j++) {
            if (m->match[j] != basis[i].match[j]) { eq = 0; break; }
        }
        if (eq) return i;
    }
    return -1;
}

/* ----------------------------------------------------------------
 * Build TL generator matrix e_i in basis representation
 *
 * M[row][col] = coefficient of basis[row] in e_i * basis[col]
 * At delta=0: either 0 (loops formed) or 1 (no loops, result=basis[row])
 * ---------------------------------------------------------------- */

static TLMatrix build_generator_matrix(int n, int gen,
                                       PlanarMatch *basis, int num_basis) {
    TLMatrix m;
    PlanarMatch gen_diag, result;
    int col, loops, row;

    m = tlmat_zero(num_basis);
    gen_diag = make_generator_diagram(n, gen);

    for (col = 0; col < num_basis; col++) {
        loops = compose_diagrams(n, &gen_diag, &basis[col], &result);
        if (loops > 0) continue; /* delta^loops = 0 */

        row = find_basis_index(&result, basis, num_basis, n);
        if (row >= 0)
            m.entries[row][col] = cyc8_one();
    }
    return m;
}

/* ----------------------------------------------------------------
 * PART D: Braid generators
 *
 * sigma_i     = A * I + A^{-1} * e_i   (positive crossing)
 * sigma_i^{-1} = A^{-1} * I + A * e_i   (negative crossing)
 * ---------------------------------------------------------------- */

static TLMatrix build_braid_generator(int n, int gen, int positive,
                                      PlanarMatch *basis, int num_basis) {
    Cyc8 A = cyc8_make(0, -1, 0, 0);
    Cyc8 A_inv = cyc8_conj(A);
    TLMatrix I_mat = tlmat_identity(num_basis);
    TLMatrix e_mat = build_generator_matrix(n, gen, basis, num_basis);
    TLMatrix term1, term2;

    if (positive) {
        term1 = tlmat_scale(A, &I_mat);
        term2 = tlmat_scale(A_inv, &e_mat);
    } else {
        term1 = tlmat_scale(A_inv, &I_mat);
        term2 = tlmat_scale(A, &e_mat);
    }
    return tlmat_add(&term1, &term2);
}

/* ================================================================
 * PART E: Braid Word Evaluation
 *
 * A braid word is a sequence of crossings. Each crossing specifies
 * which generator (0-indexed) and sign (+1 positive, -1 negative).
 * Evaluation = product of precomputed generator matrices.
 * ================================================================ */

typedef struct {
    int gen;   /* 0-indexed: 0..n-2 */
    int sign;  /* +1 positive, -1 negative */
} BraidCrossing;

static TLMatrix eval_braid_word(const BraidCrossing *word, int word_len,
                                const TLMatrix *sigma_pos,
                                const TLMatrix *sigma_neg,
                                int num_basis) {
    TLMatrix result = tlmat_identity(num_basis);
    int i;
    for (i = 0; i < word_len; i++) {
        const TLMatrix *gen_mat;
        if (word[i].sign > 0)
            gen_mat = &sigma_pos[word[i].gen];
        else
            gen_mat = &sigma_neg[word[i].gen];
        result = tlmat_mul(&result, gen_mat);
    }
    return result;
}

/* ================================================================
 * PART F: Closure Functionals
 *
 * Trace closure: connect top k to bottom k (k <-> n+k).
 * Each point has degree 2 (one matching edge, one closure edge).
 * Count disjoint cycles by alternating edge types.
 *
 * Bracket from matrix: braid beta as TL element has coefficients
 *   M[i][id_idx] for each basis element i.
 * Trace closure bracket at delta=0:
 *   <beta_hat> = sum_{i : trace_loops(basis[i]) == 1} M[i][id_idx]
 * because delta^{L-1} = 1 only when L = 1.
 * ================================================================ */

/* Count loops in trace closure of a TL diagram */
static int trace_closure_loops(int n, const PlanarMatch *m) {
    int visited[MAX_2N];
    int i, cur, loops = 0;
    memset(visited, 0, (size_t)(2 * n) * sizeof(int));

    for (i = 0; i < 2 * n; i++) {
        if (visited[i]) continue;
        loops++;
        cur = i;
        do {
            visited[cur] = 1;
            cur = m->match[cur];       /* follow matching edge */
            visited[cur] = 1;
            cur = (cur < n) ? cur + n : cur - n;  /* follow closure edge */
        } while (cur != i);
    }
    return loops;
}

/* Extract bracket from braid matrix via trace closure */
static Cyc8 compute_bracket_trace(const TLMatrix *braid_mat,
                                  const PlanarMatch *basis, int num_basis,
                                  int n, int id_idx) {
    Cyc8 bracket = cyc8_zero();
    int i;
    for (i = 0; i < num_basis; i++) {
        if (trace_closure_loops(n, &basis[i]) == 1)
            bracket = cyc8_add(bracket, braid_mat->entries[i][id_idx]);
    }
    return bracket;
}

/* Compute A^power exactly in Z[zeta_8] */
static Cyc8 cyc8_a_power(int power) {
    Cyc8 A = cyc8_make(0, -1, 0, 0);
    Cyc8 A_inv = cyc8_conj(A);
    Cyc8 result = cyc8_one();
    Cyc8 base;
    int abs_pow, k;

    if (power == 0) return cyc8_one();
    abs_pow = power > 0 ? power : -power;
    base = power > 0 ? A : A_inv;
    for (k = 0; k < abs_pow; k++)
        result = cyc8_mul(result, base);
    return result;
}

/* State-sum bracket for cross-validation.
 * For each of 2^word_len states, resolve crossings as identity or e_i,
 * compose all resolved tangles, count total loops (glue + trace closure).
 * Only states with exactly 1 total loop contribute at delta=0. */
static Cyc8 state_sum_bracket(int n, const BraidCrossing *word, int word_len) {
    Cyc8 bracket = cyc8_zero();
    int num_states, s;

    if (word_len > 20) {
        printf("  WARNING: word_len %d too large for state sum\n", word_len);
        return bracket;
    }

    num_states = 1 << word_len;

    for (s = 0; s < num_states; s++) {
        PlanarMatch current;
        int total_glue_loops = 0;
        int a_power = 0;
        int j;

        current = make_identity_diagram(n);

        for (j = 0; j < word_len; j++) {
            PlanarMatch resolved, result;
            int glue_loops, bit;

            bit = (s >> j) & 1;

            if (bit == 0) {
                /* 0-smoothing: identity at this crossing */
                resolved = make_identity_diagram(n);
                a_power += word[j].sign;
            } else {
                /* 1-smoothing: e_i at this crossing */
                resolved = make_generator_diagram(n, word[j].gen);
                a_power -= word[j].sign;
            }

            glue_loops = compose_diagrams(n, &current, &resolved, &result);
            total_glue_loops += glue_loops;
            current = result;
        }

        {
            int tl = trace_closure_loops(n, &current);
            int total = total_glue_loops + tl;

            if (total == 1)
                bracket = cyc8_add(bracket, cyc8_a_power(a_power));
        }
    }
    return bracket;
}

/* ================================================================
 * PART A TESTS: Quick Cyc8 Sanity
 * ================================================================ */

static void part_a_cyc8_sanity(void) {
    Cyc8 z8, z8_4, A, A_inv, prod, sum;
    char msg[128];

    printf("\n=== PART A: Cyc8 Arithmetic Sanity ===\n\n");

    z8 = cyc8_make(0, 1, 0, 0);

    /* zeta_8^4 = -1 */
    z8_4 = cyc8_mul(cyc8_mul(z8, z8), cyc8_mul(z8, z8));
    check("zeta_8^4 = -1", cyc8_eq(z8_4, cyc8_make(-1, 0, 0, 0)));

    /* A = -zeta_8, A^{-1} = conj(A), A*A^{-1} = 1 */
    A = cyc8_make(0, -1, 0, 0);
    A_inv = cyc8_conj(A);
    prod = cyc8_mul(A, A_inv);
    check("A * A^{-1} = 1", cyc8_eq(prod, cyc8_one()));

    /* delta = 0: A^2 + A^{-2} = 0 */
    sum = cyc8_add(cyc8_mul(A, A), cyc8_mul(A_inv, A_inv));
    check("delta = 0 (A^2 + A^{-2} = 0)", cyc8_eq(sum, cyc8_zero()));

    /* Cross-check A with float */
    {
        Cx cA = cx_exp_i(5.0 * M_PI / 4.0);
        Cx cA_exact = cyc8_to_cx(A);
        double err = cx_abs(cx_sub(cA, cA_exact));
        sprintf(msg, "A float matches exact (err=%.2e)", err);
        check(msg, err < 1e-10);
    }

    /* Multiplication cross-check */
    {
        Cyc8 x = cyc8_make(2, -1, 3, 1);
        Cyc8 y = cyc8_make(-1, 2, 0, -3);
        Cyc8 xy = cyc8_mul(x, y);
        Cx cx_x = cyc8_to_cx(x);
        Cx cx_y = cyc8_to_cx(y);
        double err = cx_abs(cx_sub(cx_mul(cx_x, cx_y), cyc8_to_cx(xy)));
        sprintf(msg, "mul cross-check (err=%.2e)", err);
        check(msg, err < 1e-10);
    }

    /* scale, sub, neg quick checks */
    {
        Cyc8 x = cyc8_make(1, 2, 3, 4);
        Cyc8 neg_x = cyc8_neg(x);
        Cyc8 x3 = cyc8_scale(x, 3);
        check("neg(1,2,3,4) = (-1,-2,-3,-4)",
              cyc8_eq(neg_x, cyc8_make(-1, -2, -3, -4)));
        check("3*(1,2,3,4) = (3,6,9,12)",
              cyc8_eq(x3, cyc8_make(3, 6, 9, 12)));
        check("x - x = 0", cyc8_eq(cyc8_sub(x, x), cyc8_zero()));
    }

    /* cyc8_print test (visual) */
    cyc8_print("A = -zeta_8", A);
    cyc8_print("A^{-1}", A_inv);
}

/* ================================================================
 * PART B TESTS: Planar Matching Enumeration
 * ================================================================ */

static void part_b_planar_matchings(void) {
    PlanarMatch basis[MAX_BASIS];
    int count, i;
    int found_identity;
    char msg[128];

    printf("\n=== PART B: Planar Matching Enumeration ===\n\n");

    /* n=2: C_2 = 2 */
    count = enumerate_basis(2, basis);
    printf("  n=2: %d matchings (expected C_2 = 2)\n", count);
    for (i = 0; i < count; i++)
        print_matching(&basis[i], 2);
    sprintf(msg, "C_2 = 2 (got %d)", count);
    check(msg, count == 2);

    found_identity = 0;
    for (i = 0; i < count; i++) {
        if (is_identity_matching(&basis[i], 2))
            found_identity = 1;
    }
    check("n=2 identity matching present", found_identity);

    printf("\n");

    /* n=3: C_3 = 5 */
    count = enumerate_basis(3, basis);
    printf("  n=3: %d matchings (expected C_3 = 5)\n", count);
    for (i = 0; i < count; i++)
        print_matching(&basis[i], 3);
    sprintf(msg, "C_3 = 5 (got %d)", count);
    check(msg, count == 5);

    found_identity = 0;
    for (i = 0; i < count; i++) {
        if (is_identity_matching(&basis[i], 3))
            found_identity = 1;
    }
    check("n=3 identity matching present", found_identity);

    printf("\n");

    /* n=4: C_4 = 14 */
    count = enumerate_basis(4, basis);
    printf("  n=4: %d matchings (expected C_4 = 14)\n", count);
    for (i = 0; i < count; i++)
        print_matching(&basis[i], 4);
    sprintf(msg, "C_4 = 14 (got %d)", count);
    check(msg, count == 14);

    found_identity = 0;
    for (i = 0; i < count; i++) {
        if (is_identity_matching(&basis[i], 4))
            found_identity = 1;
    }
    check("n=4 identity matching present", found_identity);

    printf("\n");

    /* n=5: C_5 = 42 (just count, don't print all) */
    count = enumerate_basis(5, basis);
    printf("  n=5: %d matchings (expected C_5 = 42)\n", count);
    sprintf(msg, "C_5 = 42 (got %d)", count);
    check(msg, count == 42);
}

/* ================================================================
 * PART C TESTS: TL Generator Matrices
 * ================================================================ */

static void part_c_tl_generators(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS], basis4[MAX_BASIS];
    int nb2, nb3, nb4;
    TLMatrix e1_2;        /* e_1 for n=2 (the only generator) */
    TLMatrix e1_3, e2_3;  /* e_1, e_2 for n=3 */
    TLMatrix e1_4, e2_4, e3_4; /* e_1, e_2, e_3 for n=4 */
    TLMatrix sq, triple;
    char msg[128];

    printf("\n=== PART C: TL Generator Matrices ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);
    nb4 = enumerate_basis(4, basis4);

    /* --- n=2 (dim=2): one generator e_1 --- */
    e1_2 = build_generator_matrix(2, 0, basis2, nb2);
    tlmat_print(&e1_2, "e1 (n=2)");
    sprintf(msg, "n=2: e1 has %d nonzero entries", tlmat_nonzero_count(&e1_2));
    check(msg, tlmat_nonzero_count(&e1_2) > 0);

    /* e1^2 = 0 (nilpotent at delta=0) */
    sq = tlmat_mul(&e1_2, &e1_2);
    check("n=2: e1^2 = 0 (nilpotent)", tlmat_is_zero(&sq));

    printf("\n");

    /* --- n=3 (dim=5): generators e_1, e_2 --- */
    e1_3 = build_generator_matrix(3, 0, basis3, nb3);
    e2_3 = build_generator_matrix(3, 1, basis3, nb3);
    tlmat_print(&e1_3, "e1 (n=3)");
    tlmat_print(&e2_3, "e2 (n=3)");

    /* e1^2 = 0, e2^2 = 0 */
    sq = tlmat_mul(&e1_3, &e1_3);
    check("n=3: e1^2 = 0", tlmat_is_zero(&sq));
    sq = tlmat_mul(&e2_3, &e2_3);
    check("n=3: e2^2 = 0", tlmat_is_zero(&sq));

    /* e1*e2*e1 = e1 (TL relation) */
    triple = tlmat_mul(&e1_3, &e2_3);
    triple = tlmat_mul(&triple, &e1_3);
    check("n=3: e1*e2*e1 = e1", tlmat_eq(&triple, &e1_3));

    /* e2*e1*e2 = e2 */
    triple = tlmat_mul(&e2_3, &e1_3);
    triple = tlmat_mul(&triple, &e2_3);
    check("n=3: e2*e1*e2 = e2", tlmat_eq(&triple, &e2_3));

    printf("\n");

    /* --- n=4 (dim=14): generators e_1, e_2, e_3 --- */
    e1_4 = build_generator_matrix(4, 0, basis4, nb4);
    e2_4 = build_generator_matrix(4, 1, basis4, nb4);
    e3_4 = build_generator_matrix(4, 2, basis4, nb4);

    printf("  n=4: e1 has %d nonzero, e2 has %d, e3 has %d\n",
           tlmat_nonzero_count(&e1_4), tlmat_nonzero_count(&e2_4),
           tlmat_nonzero_count(&e3_4));

    /* Nilpotent */
    sq = tlmat_mul(&e1_4, &e1_4);
    check("n=4: e1^2 = 0", tlmat_is_zero(&sq));
    sq = tlmat_mul(&e2_4, &e2_4);
    check("n=4: e2^2 = 0", tlmat_is_zero(&sq));
    sq = tlmat_mul(&e3_4, &e3_4);
    check("n=4: e3^2 = 0", tlmat_is_zero(&sq));

    /* Adjacent: e1*e2*e1 = e1, e2*e3*e2 = e2, e2*e1*e2 = e2, e3*e2*e3 = e3 */
    triple = tlmat_mul(&e1_4, &e2_4);
    triple = tlmat_mul(&triple, &e1_4);
    check("n=4: e1*e2*e1 = e1", tlmat_eq(&triple, &e1_4));

    triple = tlmat_mul(&e2_4, &e1_4);
    triple = tlmat_mul(&triple, &e2_4);
    check("n=4: e2*e1*e2 = e2", tlmat_eq(&triple, &e2_4));

    triple = tlmat_mul(&e2_4, &e3_4);
    triple = tlmat_mul(&triple, &e2_4);
    check("n=4: e2*e3*e2 = e2", tlmat_eq(&triple, &e2_4));

    triple = tlmat_mul(&e3_4, &e2_4);
    triple = tlmat_mul(&triple, &e3_4);
    check("n=4: e3*e2*e3 = e3", tlmat_eq(&triple, &e3_4));

    /* Far commutativity: e1*e3 = e3*e1 (|1-3| = 2 >= 2) */
    {
        TLMatrix p1 = tlmat_mul(&e1_4, &e3_4);
        TLMatrix p2 = tlmat_mul(&e3_4, &e1_4);
        check("n=4: e1*e3 = e3*e1 (far commutativity)", tlmat_eq(&p1, &p2));
    }

    /* Identity composition test: I * e_i = e_i */
    {
        PlanarMatch id_diag = make_identity_diagram(3);
        PlanarMatch gen_diag = make_generator_diagram(3, 0);
        PlanarMatch result;
        int loops = compose_diagrams(3, &id_diag, &gen_diag, &result);
        int match = (loops == 0);
        if (match) {
            int j;
            for (j = 0; j < 6; j++) {
                if (result.match[j] != gen_diag.match[j]) { match = 0; break; }
            }
        }
        check("n=3: I compose e1 = e1 (identity is neutral)", match);
    }

    /* --- n=5 sparsity check: predicted C_5 - C_4 = 42 - 14 = 28 --- */
    {
        PlanarMatch basis5[MAX_BASIS];
        int nb5, nz;
        TLMatrix e1_5;

        nb5 = enumerate_basis(5, basis5);
        e1_5 = build_generator_matrix(5, 0, basis5, nb5);
        nz = tlmat_nonzero_count(&e1_5);
        printf("\n  n=5: e1 has %d nonzero (predicted C_5-C_4 = %d)\n", nz, 42 - 14);
        sprintf(msg, "n=5: sparsity = C_n - C_{n-1} = 28 (got %d)", nz);
        check(msg, nz == 28);
    }
}

/* ================================================================
 * PART D TESTS: Braid Generators
 * ================================================================ */

static void part_d_braid_generators(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS];
    int nb2, nb3;
    TLMatrix sig1_pos, sig1_neg, prod;
    TLMatrix sig1_pos_3, sig1_neg_3, sig2_pos_3, sig2_neg_3;
    TLMatrix I_mat;

    printf("\n=== PART D: Braid Generators ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);

    /* --- n=2: sigma_1 and sigma_1^{-1} --- */
    sig1_pos = build_braid_generator(2, 0, 1, basis2, nb2);
    sig1_neg = build_braid_generator(2, 0, 0, basis2, nb2);
    tlmat_print(&sig1_pos, "sigma_1 (n=2)");
    tlmat_print(&sig1_neg, "sigma_1^{-1} (n=2)");

    /* sigma_1 * sigma_1^{-1} = I */
    prod = tlmat_mul(&sig1_pos, &sig1_neg);
    I_mat = tlmat_identity(nb2);
    check("n=2: sigma_1 * sigma_1^{-1} = I", tlmat_eq(&prod, &I_mat));

    /* sigma_1^{-1} * sigma_1 = I */
    prod = tlmat_mul(&sig1_neg, &sig1_pos);
    check("n=2: sigma_1^{-1} * sigma_1 = I", tlmat_eq(&prod, &I_mat));

    printf("\n");

    /* --- n=3: sigma_1, sigma_2, inverses --- */
    sig1_pos_3 = build_braid_generator(3, 0, 1, basis3, nb3);
    sig1_neg_3 = build_braid_generator(3, 0, 0, basis3, nb3);
    sig2_pos_3 = build_braid_generator(3, 1, 1, basis3, nb3);
    sig2_neg_3 = build_braid_generator(3, 1, 0, basis3, nb3);

    /* Inverse relations */
    I_mat = tlmat_identity(nb3);
    prod = tlmat_mul(&sig1_pos_3, &sig1_neg_3);
    check("n=3: sigma_1 * sigma_1^{-1} = I", tlmat_eq(&prod, &I_mat));

    prod = tlmat_mul(&sig2_pos_3, &sig2_neg_3);
    check("n=3: sigma_2 * sigma_2^{-1} = I", tlmat_eq(&prod, &I_mat));

    /* Braid relation: sigma_1 * sigma_2 * sigma_1 = sigma_2 * sigma_1 * sigma_2 */
    {
        TLMatrix lhs, rhs, tmp;
        tmp = tlmat_mul(&sig1_pos_3, &sig2_pos_3);
        lhs = tlmat_mul(&tmp, &sig1_pos_3);
        tmp = tlmat_mul(&sig2_pos_3, &sig1_pos_3);
        rhs = tlmat_mul(&tmp, &sig2_pos_3);
        check("n=3: sigma_1*sigma_2*sigma_1 = sigma_2*sigma_1*sigma_2 (braid relation)",
              tlmat_eq(&lhs, &rhs));
    }

    /* Print some entries to show they are non-trivial Cyc8 */
    printf("  sigma_1 (n=3) sample entries:\n");
    {
        int i, j;
        int printed = 0;
        for (i = 0; i < nb3 && printed < 4; i++)
            for (j = 0; j < nb3 && printed < 4; j++)
                if (!cyc8_eq(sig1_pos_3.entries[i][j], cyc8_zero())) {
                    printf("    [%d][%d] = (%ld,%ld,%ld,%ld)\n", i, j,
                           sig1_pos_3.entries[i][j].a,
                           sig1_pos_3.entries[i][j].b,
                           sig1_pos_3.entries[i][j].c,
                           sig1_pos_3.entries[i][j].d);
                    printed++;
                }
    }
}

/* ================================================================
 * PART E TESTS: Braid Word Evaluation
 * ================================================================ */

static void part_e_braid_words(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS];
    int nb2, nb3;
    TLMatrix sigma_pos_2[1], sigma_neg_2[1];
    TLMatrix sigma_pos_3[2], sigma_neg_3[2];
    TLMatrix hopf, trefoil, fig8;
    BraidCrossing hopf_word[2], trefoil_word[3], fig8_word[4];

    printf("\n=== PART E: Braid Word Evaluation ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);

    /* Precompute braid generator matrices */
    sigma_pos_2[0] = build_braid_generator(2, 0, 1, basis2, nb2);
    sigma_neg_2[0] = build_braid_generator(2, 0, 0, basis2, nb2);
    sigma_pos_3[0] = build_braid_generator(3, 0, 1, basis3, nb3);
    sigma_neg_3[0] = build_braid_generator(3, 0, 0, basis3, nb3);
    sigma_pos_3[1] = build_braid_generator(3, 1, 1, basis3, nb3);
    sigma_neg_3[1] = build_braid_generator(3, 1, 0, basis3, nb3);

    /* Hopf link: sigma_1^2 in B_2 */
    hopf_word[0].gen = 0; hopf_word[0].sign = 1;
    hopf_word[1].gen = 0; hopf_word[1].sign = 1;
    hopf = eval_braid_word(hopf_word, 2, sigma_pos_2, sigma_neg_2, nb2);
    printf("  Hopf link (sigma_1^2, B_2): %d nonzero entries\n",
           tlmat_nonzero_count(&hopf));
    check("Hopf link matrix is nonzero", tlmat_nonzero_count(&hopf) > 0);

    /* Trefoil: sigma_1^3 in B_2 */
    trefoil_word[0].gen = 0; trefoil_word[0].sign = 1;
    trefoil_word[1].gen = 0; trefoil_word[1].sign = 1;
    trefoil_word[2].gen = 0; trefoil_word[2].sign = 1;
    trefoil = eval_braid_word(trefoil_word, 3, sigma_pos_2, sigma_neg_2, nb2);
    printf("  Trefoil (sigma_1^3, B_2): %d nonzero entries\n",
           tlmat_nonzero_count(&trefoil));
    check("Trefoil matrix is nonzero", tlmat_nonzero_count(&trefoil) > 0);

    /* Figure-eight: sigma_1 sigma_2^{-1} sigma_1 sigma_2^{-1} in B_3 */
    fig8_word[0].gen = 0; fig8_word[0].sign = 1;
    fig8_word[1].gen = 1; fig8_word[1].sign = -1;
    fig8_word[2].gen = 0; fig8_word[2].sign = 1;
    fig8_word[3].gen = 1; fig8_word[3].sign = -1;
    fig8 = eval_braid_word(fig8_word, 4, sigma_pos_3, sigma_neg_3, nb3);
    printf("  Figure-eight (sigma_1*sigma_2^-1*sigma_1*sigma_2^-1, B_3): %d nonzero\n",
           tlmat_nonzero_count(&fig8));
    check("Figure-eight matrix is nonzero", tlmat_nonzero_count(&fig8) > 0);

    /* sigma_1^2 * sigma_1^{-2} = I (inverse word cancellation) */
    {
        BraidCrossing cancel_word[4];
        TLMatrix cancel_mat, I_mat;
        cancel_word[0].gen = 0; cancel_word[0].sign = 1;
        cancel_word[1].gen = 0; cancel_word[1].sign = 1;
        cancel_word[2].gen = 0; cancel_word[2].sign = -1;
        cancel_word[3].gen = 0; cancel_word[3].sign = -1;
        cancel_mat = eval_braid_word(cancel_word, 4, sigma_pos_2, sigma_neg_2, nb2);
        I_mat = tlmat_identity(nb2);
        check("n=2: sigma_1^2 * sigma_1^{-2} = I", tlmat_eq(&cancel_mat, &I_mat));
    }
}

/* ================================================================
 * PART F TESTS: Closure Functionals & Cross-Validation
 * ================================================================ */

static void part_f_closures(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS];
    int nb2, nb3, id_idx_2, id_idx_3;
    TLMatrix sigma_pos_2[1], sigma_neg_2[1];
    TLMatrix sigma_pos_3[2], sigma_neg_3[2];
    Cyc8 b_mat, b_sum;
    int i;
    char msg[128];

    printf("\n=== PART F: Closure Functionals & Cross-Validation ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);

    /* Precompute generator matrices */
    sigma_pos_2[0] = build_braid_generator(2, 0, 1, basis2, nb2);
    sigma_neg_2[0] = build_braid_generator(2, 0, 0, basis2, nb2);
    sigma_pos_3[0] = build_braid_generator(3, 0, 1, basis3, nb3);
    sigma_neg_3[0] = build_braid_generator(3, 0, 0, basis3, nb3);
    sigma_pos_3[1] = build_braid_generator(3, 1, 1, basis3, nb3);
    sigma_neg_3[1] = build_braid_generator(3, 1, 0, basis3, nb3);

    /* Find identity indices */
    {
        PlanarMatch id2 = make_identity_diagram(2);
        PlanarMatch id3 = make_identity_diagram(3);
        id_idx_2 = find_basis_index(&id2, basis2, nb2, 2);
        id_idx_3 = find_basis_index(&id3, basis3, nb3, 3);
        check("n=2: identity found in basis", id_idx_2 >= 0);
        check("n=3: identity found in basis", id_idx_3 >= 0);
    }

    /* Trace closure loop counts */
    printf("  Trace closure loops (n=2 basis):\n");
    for (i = 0; i < nb2; i++)
        printf("    basis[%d]: %d loops\n", i, trace_closure_loops(2, &basis2[i]));
    check("n=2: identity trace closure has 2 loops",
          trace_closure_loops(2, &basis2[id_idx_2]) == 2);

    printf("  Trace closure loops (n=3 basis):\n");
    for (i = 0; i < nb3; i++)
        printf("    basis[%d]: %d loops\n", i, trace_closure_loops(3, &basis3[i]));
    check("n=3: identity trace closure has 3 loops",
          trace_closure_loops(3, &basis3[id_idx_3]) == 3);

    printf("\n");

    /* === Hopf link: sigma_1^2 in B_2 === */
    {
        BraidCrossing word[2];
        TLMatrix bmat;
        word[0].gen = 0; word[0].sign = 1;
        word[1].gen = 0; word[1].sign = 1;
        bmat = eval_braid_word(word, 2, sigma_pos_2, sigma_neg_2, nb2);

        b_mat = compute_bracket_trace(&bmat, basis2, nb2, 2, id_idx_2);
        b_sum = state_sum_bracket(2, word, 2);

        cyc8_print("Hopf link (matrix)", b_mat);
        cyc8_print("Hopf link (state-sum)", b_sum);
        check("Hopf link: matrix == state-sum", cyc8_eq(b_mat, b_sum));

        sprintf(msg, "Hopf link bracket = (%ld,%ld,%ld,%ld)",
                b_mat.a, b_mat.b, b_mat.c, b_mat.d);
        printf("  %s\n", msg);
    }

    printf("\n");

    /* === Trefoil: sigma_1^3 in B_2 === */
    {
        BraidCrossing word[3];
        TLMatrix bmat;
        word[0].gen = 0; word[0].sign = 1;
        word[1].gen = 0; word[1].sign = 1;
        word[2].gen = 0; word[2].sign = 1;
        bmat = eval_braid_word(word, 3, sigma_pos_2, sigma_neg_2, nb2);

        b_mat = compute_bracket_trace(&bmat, basis2, nb2, 2, id_idx_2);
        b_sum = state_sum_bracket(2, word, 3);

        cyc8_print("Trefoil (matrix)", b_mat);
        cyc8_print("Trefoil (state-sum)", b_sum);
        check("Trefoil: matrix == state-sum", cyc8_eq(b_mat, b_sum));
    }

    printf("\n");

    /* === Figure-eight: sigma_1 sigma_2^{-1} sigma_1 sigma_2^{-1} in B_3 === */
    {
        BraidCrossing word[4];
        TLMatrix bmat;
        word[0].gen = 0; word[0].sign = 1;
        word[1].gen = 1; word[1].sign = -1;
        word[2].gen = 0; word[2].sign = 1;
        word[3].gen = 1; word[3].sign = -1;
        bmat = eval_braid_word(word, 4, sigma_pos_3, sigma_neg_3, nb3);

        b_mat = compute_bracket_trace(&bmat, basis3, nb3, 3, id_idx_3);
        b_sum = state_sum_bracket(3, word, 4);

        cyc8_print("Figure-eight (matrix)", b_mat);
        cyc8_print("Figure-eight (state-sum)", b_sum);
        check("Figure-eight: matrix == state-sum", cyc8_eq(b_mat, b_sum));
    }

    printf("\n");

    /* === 4-strand braid: sigma_1 sigma_2 sigma_3 in B_4 === */
    {
        PlanarMatch basis4[MAX_BASIS];
        int nb4, id_idx_4, k;
        TLMatrix sigma_pos_4[3], sigma_neg_4[3];
        TLMatrix bmat;
        BraidCrossing word[3];
        PlanarMatch id4;

        nb4 = enumerate_basis(4, basis4);
        id4 = make_identity_diagram(4);
        id_idx_4 = find_basis_index(&id4, basis4, nb4, 4);
        check("n=4: identity found in basis", id_idx_4 >= 0);

        for (k = 0; k < 3; k++) {
            sigma_pos_4[k] = build_braid_generator(4, k, 1, basis4, nb4);
            sigma_neg_4[k] = build_braid_generator(4, k, 0, basis4, nb4);
        }

        word[0].gen = 0; word[0].sign = 1;
        word[1].gen = 1; word[1].sign = 1;
        word[2].gen = 2; word[2].sign = 1;
        bmat = eval_braid_word(word, 3, sigma_pos_4, sigma_neg_4, nb4);

        b_mat = compute_bracket_trace(&bmat, basis4, nb4, 4, id_idx_4);
        b_sum = state_sum_bracket(4, word, 3);

        cyc8_print("4-strand sig1*sig2*sig3 (matrix)", b_mat);
        cyc8_print("4-strand sig1*sig2*sig3 (state-sum)", b_sum);
        check("4-strand braid: matrix == state-sum", cyc8_eq(b_mat, b_sum));
    }

    /* === 4-strand: sigma_1^2 * sigma_3^2 (link, far generators) === */
    {
        PlanarMatch basis4[MAX_BASIS];
        int nb4, id_idx_4, k;
        TLMatrix sigma_pos_4[3], sigma_neg_4[3];
        TLMatrix bmat;
        BraidCrossing word[4];
        PlanarMatch id4;

        nb4 = enumerate_basis(4, basis4);
        id4 = make_identity_diagram(4);
        id_idx_4 = find_basis_index(&id4, basis4, nb4, 4);

        for (k = 0; k < 3; k++) {
            sigma_pos_4[k] = build_braid_generator(4, k, 1, basis4, nb4);
            sigma_neg_4[k] = build_braid_generator(4, k, 0, basis4, nb4);
        }

        word[0].gen = 0; word[0].sign = 1;
        word[1].gen = 0; word[1].sign = 1;
        word[2].gen = 2; word[2].sign = 1;
        word[3].gen = 2; word[3].sign = 1;
        bmat = eval_braid_word(word, 4, sigma_pos_4, sigma_neg_4, nb4);

        b_mat = compute_bracket_trace(&bmat, basis4, nb4, 4, id_idx_4);
        b_sum = state_sum_bracket(4, word, 4);

        cyc8_print("4-strand sig1^2*sig3^2 (matrix)", b_mat);
        cyc8_print("4-strand sig1^2*sig3^2 (state-sum)", b_sum);
        check("4-strand far link: matrix == state-sum", cyc8_eq(b_mat, b_sum));
    }
}

/* ================================================================
 * EXPERIMENTS
 * ================================================================ */

/* Decode braid index into crossing sequence.
 * n_choices = 2*(n-1): each generator pos/neg.
 * choice 0 = gen 0 positive, 1 = gen 0 negative, 2 = gen 1 pos, ... */
static void decode_braid(int idx, int n_choices, int length,
                         BraidCrossing *word) {
    int i;
    for (i = 0; i < length; i++) {
        int choice = idx % n_choices;
        word[i].gen = choice / 2;
        word[i].sign = (choice % 2 == 0) ? 1 : -1;
        idx /= n_choices;
    }
}

/* ----------------------------------------------------------------
 * Experiment 1: Compression ratios
 *
 * Enumerate all braids of length 1..max_len, compute TL matrices,
 * count distinct. Reproduces Demo 22's measurement in exact Cyc8.
 * n=3: 4 choices/crossing (sigma_1+-, sigma_2+-), length 1..6
 * n=4: 6 choices/crossing (sigma_1+-, sigma_2+-, sigma_3+-), length 1..4
 * ---------------------------------------------------------------- */

static void experiment_compression(void) {
    PlanarMatch basis3[MAX_BASIS], basis4[MAX_BASIS];
    int nb3, nb4, k, g;
    TLMatrix sigma_pos_3[2], sigma_neg_3[2];
    TLMatrix sigma_pos_4[3], sigma_neg_4[3];
    TLMatrix *seen;
    int max_seen = 2000;

    printf("\n=== EXPERIMENT 1: Compression Ratios ===\n\n");

    nb3 = enumerate_basis(3, basis3);
    nb4 = enumerate_basis(4, basis4);

    sigma_pos_3[0] = build_braid_generator(3, 0, 1, basis3, nb3);
    sigma_neg_3[0] = build_braid_generator(3, 0, 0, basis3, nb3);
    sigma_pos_3[1] = build_braid_generator(3, 1, 1, basis3, nb3);
    sigma_neg_3[1] = build_braid_generator(3, 1, 0, basis3, nb3);

    for (g = 0; g < 3; g++) {
        sigma_pos_4[g] = build_braid_generator(4, g, 1, basis4, nb4);
        sigma_neg_4[g] = build_braid_generator(4, g, 0, basis4, nb4);
    }

    seen = (TLMatrix *)malloc((size_t)max_seen * sizeof(TLMatrix));
    if (!seen) { printf("  malloc failed\n"); return; }

    /* n=3: length 1..6, 4 choices per crossing */
    printf("  n=3 (dim=5, 4 crossing choices):\n");
    for (k = 1; k <= 6; k++) {
        int total = 1, n_seen_k = 0, i;
        for (i = 0; i < k; i++) total *= 4;

        for (i = 0; i < total; i++) {
            BraidCrossing word[6];
            TLMatrix mat;
            int found, j;

            decode_braid(i, 4, k, word);
            mat = eval_braid_word(word, k, sigma_pos_3, sigma_neg_3, nb3);

            found = 0;
            for (j = 0; j < n_seen_k; j++) {
                if (tlmat_eq(&mat, &seen[j])) { found = 1; break; }
            }
            if (!found && n_seen_k < max_seen)
                seen[n_seen_k++] = mat;
        }
        printf("    length %d: %5d braids -> %4d distinct (%5.1fx)\n",
               k, total, n_seen_k, (double)total / (double)n_seen_k);
    }

    /* n=4: length 1..5, 6 choices per crossing */
    printf("\n  n=4 (dim=14, 6 crossing choices):\n");
    for (k = 1; k <= 5; k++) {
        int total = 1, n_seen_k = 0, i;
        for (i = 0; i < k; i++) total *= 6;

        for (i = 0; i < total; i++) {
            BraidCrossing word[5];
            TLMatrix mat;
            int found, j;

            decode_braid(i, 6, k, word);
            mat = eval_braid_word(word, k, sigma_pos_4, sigma_neg_4, nb4);

            found = 0;
            for (j = 0; j < n_seen_k; j++) {
                if (tlmat_eq(&mat, &seen[j])) { found = 1; break; }
            }
            if (!found && n_seen_k < max_seen)
                seen[n_seen_k++] = mat;
        }
        printf("    length %d: %5d braids -> %4d distinct (%5.1fx)\n",
               k, total, n_seen_k, (double)total / (double)n_seen_k);
    }

    free(seen);
}

/* ----------------------------------------------------------------
 * Experiment 2: Entanglement vanishing
 *
 * At delta=0, non-interleaving braids (generators from disjoint
 * strand ranges, e.g. sigma_1 and sigma_3 in B_4) should ALL have
 * bracket = 0 because trace closure decomposes into >= 2 loops.
 * delta=0 is an ENTANGLEMENT DETECTOR.
 * ---------------------------------------------------------------- */

static void experiment_entanglement(void) {
    PlanarMatch basis4[MAX_BASIS];
    int nb4, id_idx_4, g, k;
    TLMatrix sigma_pos_4[3], sigma_neg_4[3];
    PlanarMatch id4;
    int ni_zero = 0, ni_total = 0;
    int il_nonzero = 0, il_total = 0;

    printf("\n=== EXPERIMENT 2: Entanglement Vanishing (n=4) ===\n\n");

    nb4 = enumerate_basis(4, basis4);
    id4 = make_identity_diagram(4);
    id_idx_4 = find_basis_index(&id4, basis4, nb4, 4);

    for (g = 0; g < 3; g++) {
        sigma_pos_4[g] = build_braid_generator(4, g, 1, basis4, nb4);
        sigma_neg_4[g] = build_braid_generator(4, g, 0, basis4, nb4);
    }

    /* Non-interleaving: only sigma_1 and sigma_3 (generators 0 and 2) */
    printf("  Non-interleaving (sigma_1, sigma_3 only):\n");
    for (k = 1; k <= 5; k++) {
        int total = 1, all_zero = 1, i;
        for (i = 0; i < k; i++) total *= 4;

        for (i = 0; i < total; i++) {
            BraidCrossing word[5];
            TLMatrix mat;
            Cyc8 bracket;
            int tmp = i, j;

            for (j = 0; j < k; j++) {
                int choice = tmp % 4;
                /* 0=sigma_1+, 1=sigma_1-, 2=sigma_3+, 3=sigma_3- */
                word[j].gen = (choice < 2) ? 0 : 2;
                word[j].sign = (choice % 2 == 0) ? 1 : -1;
                tmp /= 4;
            }

            mat = eval_braid_word(word, k, sigma_pos_4, sigma_neg_4, nb4);
            bracket = compute_bracket_trace(&mat, basis4, nb4, 4, id_idx_4);

            ni_total++;
            if (cyc8_eq(bracket, cyc8_zero()))
                ni_zero++;
            else
                all_zero = 0;
        }
        printf("    length %d: %5d braids, all zero = %s\n",
               k, total, all_zero ? "YES" : "NO");
    }

    check("All non-interleaving braids have bracket = 0",
          ni_zero == ni_total);

    /* Interleaving: all generators (sigma_1, sigma_2, sigma_3) */
    printf("\n  Interleaving (all generators):\n");
    for (k = 1; k <= 5; k++) {
        int total = 1, nonzero_k = 0, i;
        for (i = 0; i < k; i++) total *= 6;

        for (i = 0; i < total; i++) {
            BraidCrossing word[5];
            TLMatrix mat;
            Cyc8 bracket;

            decode_braid(i, 6, k, word);
            mat = eval_braid_word(word, k, sigma_pos_4, sigma_neg_4, nb4);
            bracket = compute_bracket_trace(&mat, basis4, nb4, 4, id_idx_4);

            il_total++;
            if (!cyc8_eq(bracket, cyc8_zero()))
                nonzero_k++;
        }
        il_nonzero += nonzero_k;
        printf("    length %d: %5d braids, %4d nonzero (%5.1f%%)\n",
               k, total, nonzero_k,
               100.0 * (double)nonzero_k / (double)total);
    }

    printf("\n  Interleaving total: %d braids, %d nonzero (%.1f%%)\n",
           il_total, il_nonzero,
           100.0 * (double)il_nonzero / (double)il_total);
}

/* ----------------------------------------------------------------
 * Experiment 3: Bracket catalog for n=4
 *
 * Collect distinct nonzero bracket values from all n=4 braids
 * of length 1..4. First n=4 catalog in the project.
 * ---------------------------------------------------------------- */

static void experiment_catalog(void) {
    PlanarMatch basis4[MAX_BASIS];
    int nb4, id_idx_4, g, k;
    TLMatrix sigma_pos_4[3], sigma_neg_4[3];
    PlanarMatch id4;
    Cyc8 catalog[500];
    int n_catalog = 0;

    printf("\n=== EXPERIMENT 3: Bracket Catalog (n=4) ===\n\n");

    nb4 = enumerate_basis(4, basis4);
    id4 = make_identity_diagram(4);
    id_idx_4 = find_basis_index(&id4, basis4, nb4, 4);

    for (g = 0; g < 3; g++) {
        sigma_pos_4[g] = build_braid_generator(4, g, 1, basis4, nb4);
        sigma_neg_4[g] = build_braid_generator(4, g, 0, basis4, nb4);
    }

    for (k = 1; k <= 5; k++) {
        int total = 1, new_vals = 0, nonzero_k = 0, i;
        for (i = 0; i < k; i++) total *= 6;

        for (i = 0; i < total; i++) {
            BraidCrossing word[5];
            TLMatrix mat;
            Cyc8 bracket;
            int found, j;

            decode_braid(i, 6, k, word);
            mat = eval_braid_word(word, k, sigma_pos_4, sigma_neg_4, nb4);
            bracket = compute_bracket_trace(&mat, basis4, nb4, 4, id_idx_4);

            if (cyc8_eq(bracket, cyc8_zero())) continue;
            nonzero_k++;

            found = 0;
            for (j = 0; j < n_catalog; j++) {
                if (cyc8_eq(bracket, catalog[j])) { found = 1; break; }
            }
            if (!found && n_catalog < 500) {
                catalog[n_catalog++] = bracket;
                new_vals++;
            }
        }
        printf("  length %d: %5d braids, %4d nonzero, %3d new (catalog: %d)\n",
               k, total, nonzero_k, new_vals, n_catalog);
    }

    /* Print full catalog */
    printf("\n  Full catalog (%d distinct nonzero bracket values):\n", n_catalog);
    {
        int i;
        for (i = 0; i < n_catalog && i < 60; i++) {
            Cx c = cyc8_to_cx(catalog[i]);
            printf("    [%2d] (%3ld,%3ld,%3ld,%3ld)  [%8.4f + %8.4fi]\n",
                   i, catalog[i].a, catalog[i].b, catalog[i].c, catalog[i].d,
                   c.re, c.im);
        }
        if (n_catalog > 60)
            printf("    ... (%d more)\n", n_catalog - 60);
    }
}

/* ----------------------------------------------------------------
 * EXPERIMENT 4: Axiality Stress Test
 * Verify every bracket value at n=2 (length 1-10) and n=3 (length 1-8)
 * has at most one nonzero Cyc8 component.
 * ---------------------------------------------------------------- */

static int cyc8_is_axial(Cyc8 v) {
    int nz = 0;
    if (v.a != 0) nz++;
    if (v.b != 0) nz++;
    if (v.c != 0) nz++;
    if (v.d != 0) nz++;
    return nz <= 1;
}

static void experiment_axiality(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS];
    int nb2, nb3, id_idx_2, id_idx_3;
    TLMatrix sigma_pos_2[1], sigma_neg_2[1];
    TLMatrix sigma_pos_3[2], sigma_neg_3[2];
    PlanarMatch id2, id3;
    int total_tested = 0, total_axial = 0, total_mixed = 0;
    int k;

    printf("\n=== EXPERIMENT 4: Axiality Stress Test ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);
    id2 = make_identity_diagram(2);
    id3 = make_identity_diagram(3);
    id_idx_2 = find_basis_index(&id2, basis2, nb2, 2);
    id_idx_3 = find_basis_index(&id3, basis3, nb3, 3);

    sigma_pos_2[0] = build_braid_generator(2, 0, 1, basis2, nb2);
    sigma_neg_2[0] = build_braid_generator(2, 0, 0, basis2, nb2);
    sigma_pos_3[0] = build_braid_generator(3, 0, 1, basis3, nb3);
    sigma_neg_3[0] = build_braid_generator(3, 0, 0, basis3, nb3);
    sigma_pos_3[1] = build_braid_generator(3, 1, 1, basis3, nb3);
    sigma_neg_3[1] = build_braid_generator(3, 1, 0, basis3, nb3);

    /* n=2: length 1..10, 2 choices per crossing */
    printf("  n=2 (dim=2, 2 crossing choices):\n");
    for (k = 1; k <= 10; k++) {
        int total = 1, axial_k = 0, mixed_k = 0, i;
        for (i = 0; i < k; i++) total *= 2;

        for (i = 0; i < total; i++) {
            BraidCrossing word[10];
            TLMatrix mat;
            Cyc8 bracket;

            decode_braid(i, 2, k, word);
            mat = eval_braid_word(word, k, sigma_pos_2, sigma_neg_2, nb2);
            bracket = compute_bracket_trace(&mat, basis2, nb2, 2, id_idx_2);

            if (cyc8_is_axial(bracket)) axial_k++;
            else mixed_k++;
        }
        printf("    length %2d: %5d braids, %5d axial, %d mixed\n",
               k, total, axial_k, mixed_k);
        total_tested += total;
        total_axial += axial_k;
        total_mixed += mixed_k;
    }

    /* n=3: length 1..8, 4 choices per crossing */
    printf("\n  n=3 (dim=5, 4 crossing choices):\n");
    for (k = 1; k <= 8; k++) {
        int total = 1, axial_k = 0, mixed_k = 0, i;
        for (i = 0; i < k; i++) total *= 4;

        for (i = 0; i < total; i++) {
            BraidCrossing word[8];
            TLMatrix mat;
            Cyc8 bracket;

            decode_braid(i, 4, k, word);
            mat = eval_braid_word(word, k, sigma_pos_3, sigma_neg_3, nb3);
            bracket = compute_bracket_trace(&mat, basis3, nb3, 3, id_idx_3);

            if (cyc8_is_axial(bracket)) axial_k++;
            else mixed_k++;
        }
        printf("    length %2d: %5d braids, %5d axial, %d mixed\n",
               k, total, axial_k, mixed_k);
        total_tested += total;
        total_axial += axial_k;
        total_mixed += mixed_k;
    }

    printf("\n  TOTAL: %d braids tested, %d axial, %d mixed\n",
           total_tested, total_axial, total_mixed);
    if (total_mixed == 0) {
        printf("  PASS: Axiality theorem holds for ALL braids tested\n");
        n_pass++;
    } else {
        printf("  FAIL: %d mixed (non-axial) brackets found!\n", total_mixed);
        n_fail++;
    }
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 35: TL_n Matrix Representations over Z[zeta_8]\n");
    printf("=============================================================\n");

    part_a_cyc8_sanity();
    part_b_planar_matchings();
    part_c_tl_generators();
    part_d_braid_generators();
    part_e_braid_words();
    part_f_closures();

    printf("\n=============================================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("=============================================================\n");

    experiment_compression();
    experiment_entanglement();
    experiment_catalog();
    experiment_axiality();

    printf("\n=============================================================\n");
    printf("Final: %d passed, %d failed\n", n_pass, n_fail);
    printf("=============================================================\n");

    return n_fail > 0 ? 1 : 0;
}
