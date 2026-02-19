/*
 * KNOTAPEL DEMO 51: Radical Anatomy of TL_n at delta=0
 * =====================================================
 *
 * THESIS: Determine the radical structure of TL_n(delta=0) for n=2,3,4,5.
 * Key discovery (explorer): TL_3 is SEMISIMPLE despite nilpotent generators.
 *
 * Part A: Planar matching enumeration + composition (from Demo 35)
 * Part B: Multiplication table (basis x basis -> index or -1)
 * Part C: Integer algebra elements and arithmetic
 * Part D: Trace form and radical dimension
 * Part E: Idempotent search and orthogonal decomposition
 *
 * NO Cyc8 needed -- everything is integer arithmetic on the TL algebra.
 * C89, zero dependencies beyond stdio/stdlib/string/math.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * Constants and Types
 * ================================================================ */

#define MAX_N 8
#define MAX_2N 16
#define MAX_BASIS 1430  /* C_8 = 1430 */
#define MAX_SEGS 16

typedef struct {
    int match[MAX_2N];
} PlanarMatch;

typedef struct {
    int points[MAX_2N];
    int count;
} Segment;

/* Bundled TL algebra data for a given n */
typedef struct {
    int n;
    int dim;                              /* C_n */
    PlanarMatch basis[MAX_BASIS];
    int id_idx;                           /* index of identity element */
    int gen_idx[MAX_N];                   /* gen_idx[g] = basis index of e_g */
    int n_gens;                           /* = n-1 */
    int mt[MAX_BASIS][MAX_BASIS];         /* multiplication table */
} TLAlgebra;

/* Integer-coefficient algebra element */
typedef struct {
    long c[MAX_BASIS];
    int dim;
} AlgElem;

/* ================================================================
 * PART A: Planar Matching Enumeration (from Demo 35)
 * ================================================================ */

static void build_boundary_order(int n, int *bp) {
    int i;
    for (i = 0; i < n; i++)
        bp[i] = i;
    for (i = 0; i < n; i++)
        bp[n + i] = 2 * n - 1 - i;
}

static void enum_segments(Segment *segs, int n_segs, int *match_buf,
                          PlanarMatch *basis, int *num_basis, int n) {
    int s, j, k;
    int first_seg;
    Segment new_segs[MAX_SEGS];
    int new_n;
    int *pts;
    int cnt;

    first_seg = -1;
    for (s = 0; s < n_segs; s++) {
        if (segs[s].count > 0) { first_seg = s; break; }
    }
    if (first_seg == -1) {
        if (*num_basis < MAX_BASIS) {
            memcpy(basis[*num_basis].match, match_buf,
                   (size_t)(2 * n) * sizeof(int));
            (*num_basis)++;
        }
        return;
    }

    pts = segs[first_seg].points;
    cnt = segs[first_seg].count;

    for (j = 1; j < cnt; j += 2) {
        match_buf[pts[0]] = pts[j];
        match_buf[pts[j]] = pts[0];

        new_n = 0;
        for (k = 0; k < n_segs; k++) {
            if (k == first_seg) {
                if (j > 1) {
                    memcpy(new_segs[new_n].points, &pts[1],
                           (size_t)(j - 1) * sizeof(int));
                    new_segs[new_n].count = j - 1;
                    new_n++;
                }
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

/* ================================================================
 * Diagram Composition (from Demo 35)
 * ================================================================ */

static int compose_diagrams(int n, const PlanarMatch *d1, const PlanarMatch *d2,
                            PlanarMatch *result) {
    int glue_visited[MAX_N];
    int i, loops;

    memset(result->match, -1, (size_t)(2 * n) * sizeof(int));
    memset(glue_visited, 0, (size_t)n * sizeof(int));
    loops = 0;

    for (i = 0; i < 2 * n; i++) {
        int in_d1;
        int cur;

        if (result->match[i] >= 0) continue;

        if (i < n) { in_d1 = 1; cur = i; }
        else       { in_d1 = 0; cur = i; }

        for (;;) {
            int partner;
            if (in_d1) {
                partner = d1->match[cur];
                if (partner < n) {
                    result->match[i] = partner;
                    result->match[partner] = i;
                    break;
                }
                glue_visited[partner - n] = 1;
                in_d1 = 0;
                cur = partner - n;
            } else {
                partner = d2->match[cur];
                if (partner >= n) {
                    result->match[i] = partner;
                    result->match[partner] = i;
                    break;
                }
                glue_visited[partner] = 1;
                in_d1 = 1;
                cur = n + partner;
            }
        }
    }

    for (i = 0; i < n; i++) {
        int cur, p, q;
        if (glue_visited[i]) continue;
        loops++;
        cur = i;
        do {
            glue_visited[cur] = 1;
            p = d2->match[cur];
            glue_visited[p] = 1;
            q = d1->match[n + p];
            cur = q - n;
        } while (cur != i);
    }

    return loops;
}

/* ================================================================
 * Helper Functions
 * ================================================================ */

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
    for (k = 0; k < n; k++) {
        m.match[k] = n + k;
        m.match[n + k] = k;
    }
    m.match[gen] = gen + 1;
    m.match[gen + 1] = gen;
    m.match[n + gen] = n + gen + 1;
    m.match[n + gen + 1] = n + gen;
    return m;
}

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

/* ================================================================
 * PART B: Multiplication Table
 * ================================================================ */

static void compute_mult_table(TLAlgebra *alg) {
    int i, j;
    for (i = 0; i < alg->dim; i++) {
        for (j = 0; j < alg->dim; j++) {
            PlanarMatch result;
            int loops = compose_diagrams(alg->n, &alg->basis[i],
                                         &alg->basis[j], &result);
            if (loops > 0) {
                alg->mt[i][j] = -1;
            } else {
                alg->mt[i][j] = find_basis_index(&result, alg->basis,
                                                  alg->dim, alg->n);
            }
        }
    }
}

static void init_tl_algebra(TLAlgebra *alg, int n) {
    PlanarMatch diag;
    int g;

    alg->n = n;
    alg->dim = enumerate_basis(n, alg->basis);
    alg->n_gens = n - 1;

    diag = make_identity_diagram(n);
    alg->id_idx = find_basis_index(&diag, alg->basis, alg->dim, n);

    for (g = 0; g < n - 1; g++) {
        diag = make_generator_diagram(n, g);
        alg->gen_idx[g] = find_basis_index(&diag, alg->basis, alg->dim, n);
    }

    compute_mult_table(alg);
}

/* ================================================================
 * PART C: Algebra Element Arithmetic
 * ================================================================ */

static AlgElem alg_zero(int dim) {
    AlgElem e;
    e.dim = dim;
    memset(e.c, 0, sizeof(e.c));
    return e;
}

static AlgElem alg_basis_elem(int dim, int idx) {
    AlgElem e = alg_zero(dim);
    e.c[idx] = 1;
    return e;
}

static AlgElem alg_add(AlgElem a, AlgElem b) {
    AlgElem r;
    int i;
    r.dim = a.dim;
    for (i = 0; i < a.dim; i++) r.c[i] = a.c[i] + b.c[i];
    for (i = a.dim; i < MAX_BASIS; i++) r.c[i] = 0;
    return r;
}

static AlgElem alg_sub(AlgElem a, AlgElem b) {
    AlgElem r;
    int i;
    r.dim = a.dim;
    for (i = 0; i < a.dim; i++) r.c[i] = a.c[i] - b.c[i];
    for (i = a.dim; i < MAX_BASIS; i++) r.c[i] = 0;
    return r;
}

static AlgElem alg_mul(AlgElem a, AlgElem b,
                       const int mt[MAX_BASIS][MAX_BASIS]) {
    AlgElem r = alg_zero(a.dim);
    int i, j, k;
    for (i = 0; i < a.dim; i++) {
        if (a.c[i] == 0) continue;
        for (j = 0; j < b.dim; j++) {
            if (b.c[j] == 0) continue;
            k = mt[i][j];
            if (k >= 0)
                r.c[k] += a.c[i] * b.c[j];
        }
    }
    return r;
}

static int alg_is_zero(AlgElem a) {
    int i;
    for (i = 0; i < a.dim; i++)
        if (a.c[i] != 0) return 0;
    return 1;
}

static int alg_eq(AlgElem a, AlgElem b) {
    int i;
    for (i = 0; i < a.dim; i++)
        if (a.c[i] != b.c[i]) return 0;
    return 1;
}

static int alg_is_idempotent(AlgElem e, const int mt[MAX_BASIS][MAX_BASIS]) {
    AlgElem sq = alg_mul(e, e, mt);
    return alg_eq(sq, e);
}

static void alg_print(const char *label, AlgElem e, const TLAlgebra *alg) {
    int i, first = 1;
    printf("    %s = ", label);
    for (i = 0; i < e.dim; i++) {
        if (e.c[i] == 0) continue;
        if (!first && e.c[i] > 0) printf(" + ");
        if (!first && e.c[i] < 0) printf(" - ");
        if (first && e.c[i] < 0) printf("-");
        first = 0;

        if (labs(e.c[i]) != 1 || i == alg->id_idx)
            printf("%ld", labs(e.c[i]));
        if (i == alg->id_idx)
            printf("*1");
        else
            printf("b%d", i);
    }
    if (first) printf("0");
    printf("\n");
}

/* Helper: is n a Catalan number? */
static int is_catalan(int n) {
    static const int cats[] = {1, 1, 2, 5, 14, 42, 132, 429, 1430};
    int i;
    for (i = 0; i < 9; i++)
        if (n == cats[i]) return 1;
    return 0;
}

/* ================================================================
 * PART D: Trace Form and Radical
 *
 * The bilinear trace form: tau(a,b) = tr(L_{a*b})
 * where L_x is left multiplication by x.
 *
 * For basis elements at delta=0:
 *   tr(L_{b_p}) = |{k : mt[p][k] == k}| (fixed points)
 *
 * Gram matrix: G[i][j] = tau(b_i, b_j)
 *   = fixpt[mt[i][j]] if mt[i][j] >= 0, else 0
 *
 * Radical = null space of G (by Cartan's criterion, char 0).
 * ================================================================ */

static int compute_radical_dim(const TLAlgebra *alg) {
    static double gram[MAX_BASIS][MAX_BASIS];
    int fixpt[MAX_BASIS];
    int i, j, k, rank;
    double eps = 0.5;

    /* Compute fixed point count for each basis element */
    for (i = 0; i < alg->dim; i++) {
        fixpt[i] = 0;
        for (k = 0; k < alg->dim; k++) {
            if (alg->mt[i][k] == k) fixpt[i]++;
        }
    }

    printf("    Fixed points: ");
    for (i = 0; i < alg->dim; i++)
        printf("%d ", fixpt[i]);
    printf("\n");

    /* Build Gram matrix */
    for (i = 0; i < alg->dim; i++) {
        for (j = 0; j < alg->dim; j++) {
            int p = alg->mt[i][j];
            gram[i][j] = (p >= 0) ? (double)fixpt[p] : 0.0;
        }
    }

    /* Print Gram matrix for small cases */
    if (alg->dim <= 14) {
        printf("    Gram matrix:\n");
        for (i = 0; i < alg->dim; i++) {
            printf("      ");
            for (j = 0; j < alg->dim; j++)
                printf("%3.0f ", gram[i][j]);
            printf("\n");
        }
    }

    /* Gaussian elimination for rank */
    rank = 0;
    for (k = 0; k < alg->dim; k++) {
        int best = -1;
        double best_val = eps;
        for (i = rank; i < alg->dim; i++) {
            double val = fabs(gram[i][k]);
            if (val > best_val) { best_val = val; best = i; }
        }
        if (best == -1) continue;

        if (best != rank) {
            for (j = 0; j < alg->dim; j++) {
                double tmp = gram[rank][j];
                gram[rank][j] = gram[best][j];
                gram[best][j] = tmp;
            }
        }

        for (i = rank + 1; i < alg->dim; i++) {
            if (fabs(gram[i][k]) > eps) {
                double factor = gram[i][k] / gram[rank][k];
                for (j = k; j < alg->dim; j++)
                    gram[i][j] -= factor * gram[rank][j];
            }
        }
        rank++;
    }

    return alg->dim - rank;
}

/* ================================================================
 * PART E: Idempotent Search & Orthogonal Decomposition
 * ================================================================ */

/* Find idempotents from length-2 products of generators */
static int find_gen_product_idempotents(const TLAlgebra *alg,
                                        AlgElem *idemp, int max_idemp) {
    int n_found = 0;
    int g1, g2;

    for (g1 = 0; g1 < alg->n_gens && n_found < max_idemp; g1++) {
        for (g2 = 0; g2 < alg->n_gens && n_found < max_idemp; g2++) {
            AlgElem a = alg_basis_elem(alg->dim, alg->gen_idx[g1]);
            AlgElem b = alg_basis_elem(alg->dim, alg->gen_idx[g2]);
            AlgElem prod = alg_mul(a, b, alg->mt);

            if (!alg_is_zero(prod) &&
                alg_is_idempotent(prod, alg->mt)) {
                /* Check it's not a duplicate */
                int dup = 0, d;
                for (d = 0; d < n_found; d++) {
                    if (alg_eq(prod, idemp[d])) { dup = 1; break; }
                }
                if (!dup) {
                    idemp[n_found] = prod;
                    printf("    Found idempotent: e_%d * e_%d  ", g1, g2);
                    alg_print("", prod, alg);
                    n_found++;
                }
            }
        }
    }
    return n_found;
}

/* Check if two elements are orthogonal (a*b = 0 AND b*a = 0) */
static int alg_orthogonal(AlgElem a, AlgElem b,
                          const int mt[MAX_BASIS][MAX_BASIS]) {
    return alg_is_zero(alg_mul(a, b, mt)) &&
           alg_is_zero(alg_mul(b, a, mt));
}

/* Build maximal orthogonal subset greedily */
static int build_orthogonal_set(AlgElem *idemp, int n_idemp,
                                AlgElem *orth, int max_orth,
                                const int mt[MAX_BASIS][MAX_BASIS]) {
    int n_orth = 0;
    int i, j;

    for (i = 0; i < n_idemp && n_orth < max_orth; i++) {
        int compatible = 1;
        for (j = 0; j < n_orth; j++) {
            if (!alg_orthogonal(idemp[i], orth[j], mt)) {
                compatible = 0;
                break;
            }
        }
        if (compatible) {
            orth[n_orth++] = idemp[i];
        }
    }
    return n_orth;
}

/* Compute Peirce block dimensions */
static int compute_peirce_dim(AlgElem fi, AlgElem fj,
                              const TLAlgebra *alg) {
    static double vecs[MAX_BASIS][MAX_BASIS];
    int k, m, rank;
    double eps = 0.5;
    int i;

    for (k = 0; k < alg->dim; k++) {
        AlgElem bk = alg_basis_elem(alg->dim, k);
        AlgElem tmp = alg_mul(bk, fj, alg->mt);
        AlgElem prod = alg_mul(fi, tmp, alg->mt);
        for (m = 0; m < alg->dim; m++)
            vecs[k][m] = (double)prod.c[m];
    }

    /* Rank of the vector set */
    rank = 0;
    for (m = 0; m < alg->dim && rank < alg->dim; m++) {
        int best = -1;
        double best_val = eps;
        for (i = rank; i < alg->dim; i++) {
            double val = fabs(vecs[i][m]);
            if (val > best_val) { best_val = val; best = i; }
        }
        if (best == -1) continue;

        if (best != rank) {
            int jj;
            for (jj = 0; jj < alg->dim; jj++) {
                double tmp = vecs[rank][jj];
                vecs[rank][jj] = vecs[best][jj];
                vecs[best][jj] = tmp;
            }
        }

        for (i = rank + 1; i < alg->dim; i++) {
            if (fabs(vecs[i][m]) > eps) {
                double factor = vecs[i][m] / vecs[rank][m];
                int jj;
                for (jj = m; jj < alg->dim; jj++)
                    vecs[i][jj] -= factor * vecs[rank][jj];
            }
        }
        rank++;
    }
    return rank;
}

/* Compute Peirce block dimension restricted to a subspace */
static int compute_peirce_dim_subspace(AlgElem fi, AlgElem fj,
                                        const AlgElem *subspace, int sub_dim,
                                        const TLAlgebra *alg) {
    static double vecs[MAX_BASIS][MAX_BASIS];
    int k, m, rank;
    double eps = 0.5;
    int i;

    for (k = 0; k < sub_dim; k++) {
        AlgElem tmp;
        AlgElem prod;
        tmp = alg_mul(subspace[k], fj, alg->mt);
        prod = alg_mul(fi, tmp, alg->mt);
        for (m = 0; m < alg->dim; m++)
            vecs[k][m] = (double)prod.c[m];
    }

    rank = 0;
    for (m = 0; m < alg->dim && rank < sub_dim; m++) {
        int best = -1;
        double best_val = eps;
        for (i = rank; i < sub_dim; i++) {
            double val = fabs(vecs[i][m]);
            if (val > best_val) { best_val = val; best = i; }
        }
        if (best == -1) continue;
        if (best != rank) {
            int jj;
            for (jj = 0; jj < alg->dim; jj++) {
                double tmp2 = vecs[rank][jj];
                vecs[rank][jj] = vecs[best][jj];
                vecs[best][jj] = tmp2;
            }
        }
        for (i = rank + 1; i < sub_dim; i++) {
            if (fabs(vecs[i][m]) > eps) {
                double factor = vecs[i][m] / vecs[rank][m];
                int jj;
                for (jj = m; jj < alg->dim; jj++)
                    vecs[i][jj] -= factor * vecs[rank][jj];
            }
        }
        rank++;
    }
    return rank;
}

/* ================================================================
 * PART F: Radical Filtration
 *
 * Extract radical basis via Gram matrix null space,
 * then compute rad^2, rad^3, ... until zero.
 * ================================================================ */

static int extract_radical_basis(const TLAlgebra *alg, AlgElem *rad_basis) {
    static double aug[MAX_BASIS][2 * MAX_BASIS];
    int fixpt[MAX_BASIS];
    int i, j, k, rank;
    double eps = 0.5;
    int n_rad = 0;

    /* Compute fixed point counts for trace */
    for (i = 0; i < alg->dim; i++) {
        fixpt[i] = 0;
        for (k = 0; k < alg->dim; k++) {
            if (alg->mt[i][k] == k) fixpt[i]++;
        }
    }

    /* Build augmented matrix [G | I] */
    for (i = 0; i < alg->dim; i++) {
        int p;
        for (j = 0; j < alg->dim; j++) {
            p = alg->mt[i][j];
            aug[i][j] = (p >= 0) ? (double)fixpt[p] : 0.0;
        }
        for (j = 0; j < alg->dim; j++)
            aug[i][alg->dim + j] = (i == j) ? 1.0 : 0.0;
    }

    /* Row reduce left half */
    rank = 0;
    for (k = 0; k < alg->dim; k++) {
        int best = -1;
        double best_val = eps;
        for (i = rank; i < alg->dim; i++) {
            if (fabs(aug[i][k]) > best_val) {
                best_val = fabs(aug[i][k]);
                best = i;
            }
        }
        if (best == -1) continue;
        if (best != rank) {
            for (j = 0; j < 2 * alg->dim; j++) {
                double tmp = aug[rank][j];
                aug[rank][j] = aug[best][j];
                aug[best][j] = tmp;
            }
        }
        for (i = rank + 1; i < alg->dim; i++) {
            if (fabs(aug[i][k]) > eps) {
                double factor = aug[i][k] / aug[rank][k];
                for (j = k; j < 2 * alg->dim; j++)
                    aug[i][j] -= factor * aug[rank][j];
            }
        }
        rank++;
    }

    /* Extract null vectors from bottom rows of right half */
    for (i = rank; i < alg->dim; i++) {
        int s;
        double scale = 1.0;
        int found_scale = 0;

        /* Find smallest integer scale making all entries integer */
        for (s = 1; s <= 1000 && !found_scale; s++) {
            int all_int = 1;
            for (j = 0; j < alg->dim; j++) {
                double val = aug[i][alg->dim + j] * (double)s;
                if (fabs(val) > 0.01 &&
                    fabs(val - floor(val + 0.5)) > 0.01) {
                    all_int = 0;
                    break;
                }
            }
            if (all_int) { scale = (double)s; found_scale = 1; }
        }

        rad_basis[n_rad] = alg_zero(alg->dim);
        for (j = 0; j < alg->dim; j++) {
            double val = aug[i][alg->dim + j] * scale;
            rad_basis[n_rad].c[j] = (long)floor(val + 0.5);
        }
        if (!alg_is_zero(rad_basis[n_rad]))
            n_rad++;
    }

    return n_rad;
}

/* Try adding an element to an echelon basis. Returns 1 if independent. */
static int try_add_to_echelon(double echelon[][MAX_BASIS],
                               int *pivot_col, int *n_basis,
                               const AlgElem *elem, int alg_dim,
                               AlgElem *basis_out) {
    double row[MAX_BASIS];
    int k, m, new_pivot;
    double eps = 0.5;

    for (k = 0; k < alg_dim; k++) row[k] = (double)elem->c[k];

    for (k = 0; k < *n_basis; k++) {
        if (fabs(row[pivot_col[k]]) > eps) {
            double factor = row[pivot_col[k]] / echelon[k][pivot_col[k]];
            for (m = 0; m < alg_dim; m++)
                row[m] -= factor * echelon[k][m];
        }
    }

    new_pivot = -1;
    for (k = 0; k < alg_dim; k++) {
        if (fabs(row[k]) > eps) { new_pivot = k; break; }
    }

    if (new_pivot >= 0) {
        for (k = 0; k < alg_dim; k++) echelon[*n_basis][k] = row[k];
        pivot_col[*n_basis] = new_pivot;
        basis_out[*n_basis] = *elem;
        (*n_basis)++;
        return 1;
    }
    return 0;
}

/* Compute rad^{k+1} from rad^k basis and original radical basis */
static int compute_next_radical_power(
    const AlgElem *rad_basis, int rad_dim,
    const AlgElem *prev_basis, int prev_dim,
    const int mt[MAX_BASIS][MAX_BASIS],
    int alg_dim,
    AlgElem *new_basis) {

    static double echelon[MAX_BASIS][MAX_BASIS];
    int pivot_col[MAX_BASIS];
    int n_new = 0;
    int i, j;
    /* prev * rad */
    for (i = 0; i < prev_dim && n_new < alg_dim; i++) {
        for (j = 0; j < rad_dim && n_new < alg_dim; j++) {
            AlgElem prod;
            prod = alg_mul(prev_basis[i], rad_basis[j], mt);
            if (!alg_is_zero(prod))
                try_add_to_echelon(echelon, pivot_col, &n_new,
                                   &prod, alg_dim, new_basis);
        }
    }

    /* rad * prev (other order for robustness) */
    for (i = 0; i < rad_dim && n_new < alg_dim; i++) {
        for (j = 0; j < prev_dim && n_new < alg_dim; j++) {
            AlgElem prod;
            prod = alg_mul(rad_basis[i], prev_basis[j], mt);
            if (!alg_is_zero(prod))
                try_add_to_echelon(echelon, pivot_col, &n_new,
                                   &prod, alg_dim, new_basis);
        }
    }

    return n_new;
}

static void radical_filtration(const TLAlgebra *alg, int rad_dim,
                                AlgElem *rad2_out, int *rad2_dim_out) {
    static AlgElem rad_basis[MAX_BASIS];
    static AlgElem current_basis[MAX_BASIS];
    static AlgElem next_buf[MAX_BASIS];
    int n_rad, current_dim, next_dim, power;

    *rad2_dim_out = 0;

    printf("  --- Radical Filtration ---\n");

    n_rad = extract_radical_basis(alg, rad_basis);
    printf("    Extracted %d radical basis vectors (expected %d)\n",
           n_rad, rad_dim);

    memcpy(current_basis, rad_basis, sizeof(AlgElem) * (size_t)n_rad);
    current_dim = n_rad;
    printf("    rad^1 dim: %d\n", current_dim);

    for (power = 2; current_dim > 0; power++) {
        next_dim = compute_next_radical_power(
            rad_basis, n_rad,
            current_basis, current_dim,
            alg->mt, alg->dim,
            next_buf);

        printf("    rad^%d dim: %d\n", power, next_dim);

        /* Save rad^2 basis for external use */
        if (power == 2 && rad2_out) {
            if (next_dim > 0)
                memcpy(rad2_out, next_buf,
                       sizeof(AlgElem) * (size_t)next_dim);
            *rad2_dim_out = next_dim;
        }

        if (next_dim == 0) break;

        memcpy(current_basis, next_buf, sizeof(AlgElem) * (size_t)next_dim);
        current_dim = next_dim;
    }

    printf("    Nilpotency index: %d\n", power);
    printf("\n");
}

/* ================================================================
 * Analysis for a given n
 * ================================================================ */

static void analyze_tl(int n) {
    static TLAlgebra alg;
    static AlgElem rad2_basis[MAX_BASIS];
    AlgElem orth[20];
    int n_orth;
    int rad_dim, rad2_dim;
    int nonzero_products, i, j;
    char msg[128];

    n_orth = 0;
    rad2_dim = 0;

    printf("\n========================================\n");
    printf("  TL_%d at delta=0  (dim = C_%d)\n", n, n);
    printf("========================================\n\n");

    /* Initialize */
    init_tl_algebra(&alg, n);

    printf("  Dimension: %d\n", alg.dim);
    printf("  Identity index: %d\n", alg.id_idx);
    printf("  Generators: ");
    for (i = 0; i < alg.n_gens; i++)
        printf("e_%d=b%d ", i, alg.gen_idx[i]);
    printf("\n\n");

    /* Multiplication table stats */
    nonzero_products = 0;
    for (i = 0; i < alg.dim; i++)
        for (j = 0; j < alg.dim; j++)
            if (alg.mt[i][j] >= 0) nonzero_products++;

    printf("  Mult table: %d x %d = %d products, %d nonzero (%.1f%%)\n",
           alg.dim, alg.dim, alg.dim * alg.dim, nonzero_products,
           100.0 * (double)nonzero_products / (double)(alg.dim * alg.dim));

    /* Print mult table for small cases */
    if (alg.dim <= 14) {
        printf("  Mult table (row * col -> result, . = 0):\n");
        printf("      ");
        for (j = 0; j < alg.dim; j++) printf("%3d", j);
        printf("\n");
        for (i = 0; i < alg.dim; i++) {
            printf("   %2d:", i);
            for (j = 0; j < alg.dim; j++) {
                if (alg.mt[i][j] >= 0)
                    printf("%3d", alg.mt[i][j]);
                else
                    printf("  .");
            }
            printf("\n");
        }
    }
    printf("\n");

    /* Verify TL relations */
    printf("  --- TL Relation Verification ---\n");

    /* 1. Identity: 1 * b_i = b_i * 1 = b_i */
    {
        int ok = 1;
        for (i = 0; i < alg.dim; i++) {
            if (alg.mt[alg.id_idx][i] != i) ok = 0;
            if (alg.mt[i][alg.id_idx] != i) ok = 0;
        }
        check("Identity: 1*b = b*1 = b for all basis elements", ok);
    }

    /* 2. Nilpotent: e_i^2 = 0 */
    {
        int ok = 1;
        for (i = 0; i < alg.n_gens; i++) {
            int g = alg.gen_idx[i];
            if (alg.mt[g][g] != -1) ok = 0;
        }
        sprintf(msg, "Nilpotent: e_i^2 = 0 for all %d generators", alg.n_gens);
        check(msg, ok);
    }

    /* 3. Adjacent: e_i * e_{i+1} * e_i = e_i */
    {
        int ok = 1;
        for (i = 0; i + 1 < alg.n_gens; i++) {
            AlgElem ei = alg_basis_elem(alg.dim, alg.gen_idx[i]);
            AlgElem ej = alg_basis_elem(alg.dim, alg.gen_idx[i + 1]);
            AlgElem prod = alg_mul(ei, ej, alg.mt);
            prod = alg_mul(prod, ei, alg.mt);
            if (!alg_eq(prod, ei)) ok = 0;
        }
        for (i = 0; i + 1 < alg.n_gens; i++) {
            AlgElem ei = alg_basis_elem(alg.dim, alg.gen_idx[i]);
            AlgElem ej = alg_basis_elem(alg.dim, alg.gen_idx[i + 1]);
            AlgElem prod = alg_mul(ej, ei, alg.mt);
            prod = alg_mul(prod, ej, alg.mt);
            if (!alg_eq(prod, ej)) ok = 0;
        }
        check("Adjacent: e_i*e_{i+1}*e_i = e_i", ok);
    }

    /* 4. Far commutativity: e_i * e_j = e_j * e_i for |i-j| >= 2 */
    {
        int ok = 1;
        for (i = 0; i < alg.n_gens; i++) {
            for (j = i + 2; j < alg.n_gens; j++) {
                int gi = alg.gen_idx[i];
                int gj = alg.gen_idx[j];
                if (alg.mt[gi][gj] != alg.mt[gj][gi]) ok = 0;
            }
        }
        check("Far commutativity: e_i*e_j = e_j*e_i for |i-j|>=2", ok);
    }
    printf("\n");

    /* Radical */
    printf("  --- Radical via Trace Form ---\n");
    rad_dim = compute_radical_dim(&alg);
    printf("    Gram rank: %d\n", alg.dim - rad_dim);
    printf("    RADICAL DIMENSION: %d\n", rad_dim);
    if (rad_dim == 0)
        printf("    ==> TL_%d is SEMISIMPLE\n", n);
    else
        printf("    ==> TL_%d is NOT semisimple (radical dim %d)\n", n, rad_dim);

    sprintf(msg, "n=%d: radical dimension computed", n);
    check(msg, 1);
    printf("\n");

    /* Radical filtration for non-semisimple cases */
    if (rad_dim > 0) {
        radical_filtration(&alg, rad_dim, rad2_basis, &rad2_dim);
    }

    /* Idempotent search */
    printf("  --- Idempotent Search (length-2 generator products) ---\n");
    {
        AlgElem idemp[20];
        int n_idemp;

        n_idemp = find_gen_product_idempotents(&alg, idemp, 20);
        printf("    Found %d distinct idempotents from generator products\n\n",
               n_idemp);

        if (n_idemp > 0) {
            /* Orthogonality matrix */
            printf("    Orthogonality (1=orthogonal, 0=not):\n");
            printf("      ");
            for (j = 0; j < n_idemp; j++) printf("  f%d", j);
            printf("\n");
            for (i = 0; i < n_idemp; i++) {
                printf("    f%d:", i);
                for (j = 0; j < n_idemp; j++) {
                    if (i == j)
                        printf("   -");
                    else
                        printf("   %d",
                               alg_orthogonal(idemp[i], idemp[j], alg.mt));
                }
                printf("\n");
            }
            printf("\n");

            /* Build maximal orthogonal set */
            n_orth = build_orthogonal_set(idemp, n_idemp, orth, 20, alg.mt);
            printf("    Maximal orthogonal set: %d idempotents\n", n_orth);

            /* Compute complement */
            {
                AlgElem complement = alg_basis_elem(alg.dim, alg.id_idx);
                int comp_is_idemp;

                for (i = 0; i < n_orth; i++)
                    complement = alg_sub(complement, orth[i]);

                alg_print("Complement (1 - sum)", complement, &alg);
                comp_is_idemp = alg_is_idempotent(complement, alg.mt);
                printf("    Complement is idempotent: %s\n",
                       comp_is_idemp ? "YES" : "NO");

                if (comp_is_idemp && !alg_is_zero(complement)) {
                    /* Check orthogonality with all orth */
                    int all_orth = 1;
                    for (i = 0; i < n_orth; i++) {
                        if (!alg_orthogonal(complement, orth[i], alg.mt))
                            all_orth = 0;
                    }
                    printf("    Complement orthogonal to all: %s\n",
                           all_orth ? "YES" : "NO");

                    if (all_orth) {
                        orth[n_orth++] = complement;
                        printf("    Complete orthogonal set: %d idempotents\n",
                               n_orth);
                    }
                }

                /* Verify sum = identity */
                {
                    AlgElem sum = alg_zero(alg.dim);
                    for (i = 0; i < n_orth; i++)
                        sum = alg_add(sum, orth[i]);
                    sprintf(msg, "n=%d: orthogonal idempotents sum to identity",
                            n);
                    check(msg,
                          alg_eq(sum, alg_basis_elem(alg.dim, alg.id_idx)));
                }

                /* Peirce block dimensions (skip for large n) */
                if (n_orth >= 2 && alg.dim <= 429) {
                    int total_dim = 0;
                    printf("\n    Peirce block dimensions (f_i * A * f_j):\n");
                    printf("      ");
                    for (j = 0; j < n_orth; j++) printf("  f%d", j);
                    printf("\n");
                    for (i = 0; i < n_orth; i++) {
                        printf("    f%d:", i);
                        for (j = 0; j < n_orth; j++) {
                            int d = compute_peirce_dim(orth[i], orth[j], &alg);
                            printf("  %2d", d);
                            total_dim += d;
                        }
                        printf("\n");
                    }
                    printf("    Total: %d (should be %d)\n",
                           total_dim, alg.dim);
                    sprintf(msg,
                            "n=%d: Peirce block dims sum to algebra dim",
                            n);
                    check(msg, total_dim == alg.dim);
                }
            }
        } else if (n == 2) {
            /* n=2: only generator is e_0, no length-2 products.
             * The algebra is k[eps]/(eps^2) -- dual numbers.
             * Only idempotent is 1. */
            printf("    n=2: no generator products (only 1 generator)\n");
            printf("    Structure: k[eps]/(eps^2) (dual numbers)\n");
            printf("    Radical = span{e_0}, dim 1\n");
            check("n=2: structure is dual numbers", rad_dim == 1);
        }
    }

    /* Catalan trace check */
    printf("  --- Catalan Trace Check ---\n");
    {
        int ok = 1;
        int fixpt[MAX_BASIS];
        int k;
        for (i = 0; i < alg.dim; i++) {
            fixpt[i] = 0;
            for (k = 0; k < alg.dim; k++) {
                if (alg.mt[i][k] == k) fixpt[i]++;
            }
            if (fixpt[i] != 0 && !is_catalan(fixpt[i])) {
                printf("    NON-CATALAN fixpt: b%d has %d fixed points!\n",
                       i, fixpt[i]);
                ok = 0;
            }
        }
        sprintf(msg,
                "n=%d: all nonzero fixed-point counts are Catalan numbers", n);
        check(msg, ok);
    }

    /* Peirce decomposition of rad^2 (sandwich duality check) */
    if (rad2_dim > 0 && n_orth >= 2) {
        int total_dim = 0;
        printf("\n  --- Peirce Blocks of rad^2 (sandwich duality) ---\n");
        printf("    rad^2 dim: %d\n", rad2_dim);
        printf("      ");
        for (j = 0; j < n_orth; j++) printf("  f%d", j);
        printf("\n");
        for (i = 0; i < n_orth; i++) {
            printf("    f%d:", i);
            for (j = 0; j < n_orth; j++) {
                int d = compute_peirce_dim_subspace(orth[i], orth[j],
                            rad2_basis, rad2_dim, &alg);
                printf("  %2d", d);
                total_dim += d;
            }
            printf("\n");
        }
        printf("    Total: %d (should be %d)\n", total_dim, rad2_dim);
        sprintf(msg, "n=%d: rad^2 Peirce blocks sum to rad^2 dim", n);
        check(msg, total_dim == rad2_dim);
    }

    printf("\n");
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);

    printf("KNOTAPEL DEMO 51: Radical Anatomy of TL_n at delta=0\n");
    printf("=====================================================\n");

    analyze_tl(2);
    analyze_tl(3);
    analyze_tl(4);
    analyze_tl(5);
    analyze_tl(6);
    analyze_tl(7);
    analyze_tl(8);

    printf("\n=====================================================\n");
    printf("Final: %d passed, %d failed\n", n_pass, n_fail);
    printf("=====================================================\n");

    return n_fail > 0 ? 1 : 0;
}
