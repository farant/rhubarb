/*
 * KNOTAPEL DEMO 39: Symmetry Decomposition of TL_n
 * =================================================
 *
 * Through-strand sector decomposition of Temperley-Lieb algebras.
 * Verifies cellular structure (sector sizes = d(n,j)^2), computes
 * per-sector Gram ranks at delta=0, quantifies cross-sector coupling,
 * verifies cell filtration ideals, and compares sector liveness across
 * multiple delta values.
 *
 * Parts:
 *   A: Sector sizes = d(n,j)^2 (cell module dimension formula)
 *   B: Per-sector Gram rank at delta=0
 *   C: Cross-sector coupling (sum of sector ranks vs full rank)
 *   D: Cell filtration ideal verification
 *   E: Multi-delta sector liveness comparison
 *
 * C89, zero dependencies beyond stdio/stdlib/string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Constants
 * ================================================================ */

#define MAX_N 9
#define MAX_2N 18
#define MAX_BASIS 1430   /* C_8 = 1430 */
#define MAX_SEGS 16
#define MAX_SECTOR 800   /* max sector size (n=8, j=2: 784) */
#define MAX_LOOPS 20

static const int catalan[] = {1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862};

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *tname, int ok) {
    if (ok) { printf("  PASS: %s\n", tname); n_pass++; }
    else    { printf("  FAIL: %s\n", tname); n_fail++; }
}

/* ================================================================
 * Types
 * ================================================================ */

typedef struct { int match[MAX_2N]; } PlanarMatch;
typedef struct { int points[MAX_2N]; int count; } Segment;

typedef struct {
    int n, dim, id_idx, gen_idx[MAX_N], n_gens;
    PlanarMatch basis[MAX_BASIS];
    int mt_result[MAX_BASIS][MAX_BASIS];
    int mt_loops[MAX_BASIS][MAX_BASIS];
    int max_loops;
} TLAlgebra;

/* ================================================================
 * Modular Arithmetic
 * ================================================================ */

static long g_mod_p = 1000002361L;  /* p ≡ 1 mod 840: all roots exist */

static long mod_reduce(long x) {
    long r = x % g_mod_p;
    return r < 0 ? r + g_mod_p : r;
}

static long mod_inv(long a) {
    long t = 0, newt = 1, r = g_mod_p, newr = mod_reduce(a);
    while (newr != 0) {
        long q = r / newr, tmp;
        tmp = t - q * newt; t = newt; newt = tmp;
        tmp = r - q * newr; r = newr; newr = tmp;
    }
    return t < 0 ? t + g_mod_p : t;
}

static long mod_pow(long base, long exp) {
    long result = 1;
    base = mod_reduce(base);
    while (exp > 0) {
        if (exp & 1) result = mod_reduce(result * base);
        base = mod_reduce(base * base);
        exp >>= 1;
    }
    return result;
}

/* Tonelli-Shanks: sqrt(n) mod p.  Returns -1 if n is a non-residue. */
static long mod_sqrt(long n) {
    long S, Q, z, M, c, t, R;
    n = mod_reduce(n);
    if (n == 0) return 0;
    if (mod_pow(n, (g_mod_p - 1) / 2) != 1) return -1;
    if (g_mod_p % 4 == 3)
        return mod_pow(n, (g_mod_p + 1) / 4);
    S = 0; Q = g_mod_p - 1;
    while (Q % 2 == 0) { S++; Q /= 2; }
    z = 2;
    while (mod_pow(z, (g_mod_p - 1) / 2) != g_mod_p - 1) z++;
    M = S;
    c = mod_pow(z, Q);
    t = mod_pow(n, Q);
    R = mod_pow(n, (Q + 1) / 2);
    while (t != 1) {
        long i = 1, tt = mod_reduce(t * t);
        while (tt != 1) { tt = mod_reduce(tt * tt); i++; }
        { long b = c; long j;
          for (j = 0; j < M - i - 1; j++) b = mod_reduce(b * b);
          R = mod_reduce(R * b);
          c = mod_reduce(b * b);
          t = mod_reduce(t * c);
          M = i; }
    }
    return R;
}

/* ================================================================
 * TL Infrastructure (from Demo 38/60)
 * ================================================================ */

static void build_boundary_order(int n, int *bp) {
    int i;
    for (i = 0; i < n; i++) bp[i] = i;
    for (i = 0; i < n; i++) bp[n+i] = 2*n-1-i;
}

static void enum_segments(Segment *segs, int n_segs, int *mb,
                          PlanarMatch *basisarr, int *nb, int n) {
    int s, j, k, first_seg = -1;
    Segment ns[MAX_SEGS]; int nn; int *pts; int cnt;
    for (s = 0; s < n_segs; s++)
        if (segs[s].count > 0) { first_seg = s; break; }
    if (first_seg == -1) {
        if (*nb < MAX_BASIS) {
            memcpy(basisarr[*nb].match, mb,
                   (size_t)(2*n) * sizeof(int));
            (*nb)++;
        }
        return;
    }
    pts = segs[first_seg].points;
    cnt = segs[first_seg].count;
    for (j = 1; j < cnt; j += 2) {
        mb[pts[0]] = pts[j]; mb[pts[j]] = pts[0];
        nn = 0;
        for (k = 0; k < n_segs; k++) {
            if (k == first_seg) {
                if (j > 1) {
                    memcpy(ns[nn].points, &pts[1],
                           (size_t)(j-1)*sizeof(int));
                    ns[nn].count = j-1; nn++;
                }
                if (cnt-j-1 > 0) {
                    memcpy(ns[nn].points, &pts[j+1],
                           (size_t)(cnt-j-1)*sizeof(int));
                    ns[nn].count = cnt-j-1; nn++;
                }
            } else {
                ns[nn] = segs[k]; nn++;
            }
        }
        enum_segments(ns, nn, mb, basisarr, nb, n);
    }
}

static int enumerate_basis(int n, PlanarMatch *basisarr) {
    Segment segs[1]; int mb[MAX_2N]; int nb = 0;
    build_boundary_order(n, segs[0].points);
    segs[0].count = 2*n;
    memset(mb, -1, sizeof(mb));
    enum_segments(segs, 1, mb, basisarr, &nb, n);
    return nb;
}

static void compose_diagrams(int n, const PlanarMatch *d1,
                              const PlanarMatch *d2,
                              PlanarMatch *r, int *loops_out) {
    int gv[MAX_N];
    int i, loops = 0;
    memset(r->match, -1, (size_t)(2*n)*sizeof(int));
    memset(gv, 0, (size_t)n*sizeof(int));
    for (i = 0; i < 2*n; i++) {
        int in1, cur;
        if (r->match[i] >= 0) continue;
        if (i < n) { in1=1; cur=i; } else { in1=0; cur=i; }
        for (;;) {
            int p;
            if (in1) {
                p = d1->match[cur];
                if (p < n) { r->match[i]=p; r->match[p]=i; break; }
                gv[p-n]=1; in1=0; cur=p-n;
            } else {
                p = d2->match[cur];
                if (p >= n) { r->match[i]=p; r->match[p]=i; break; }
                gv[p]=1; in1=1; cur=n+p;
            }
        }
    }
    for (i = 0; i < n; i++) {
        int cur, p, q;
        if (gv[i]) continue;
        loops++; cur = i;
        do {
            gv[cur]=1; p=d2->match[cur]; gv[p]=1;
            q=d1->match[n+p]; cur=q-n;
        } while (cur != i);
    }
    *loops_out = loops;
}

static int find_basis_index(const PlanarMatch *m, const PlanarMatch *b,
                             int nb, int n) {
    int i, j;
    for (i = 0; i < nb; i++) {
        int eq = 1;
        for (j = 0; j < 2*n; j++)
            if (m->match[j] != b[i].match[j]) { eq=0; break; }
        if (eq) return i;
    }
    return -1;
}

static void init_tl(TLAlgebra *a, int n) {
    PlanarMatch d;
    int g, i, j;
    a->n = n;
    a->dim = enumerate_basis(n, a->basis);
    a->n_gens = n - 1;
    a->max_loops = 0;
    memset(&d, 0, sizeof(d));
    for (g = 0; g < n; g++) {
        d.match[g] = n + g;
        d.match[n+g] = g;
    }
    a->id_idx = find_basis_index(&d, a->basis, a->dim, n);
    for (g = 0; g < n-1; g++) {
        PlanarMatch gen; int k;
        for (k = 0; k < n; k++) {
            gen.match[k] = n+k; gen.match[n+k] = k;
        }
        gen.match[g] = g+1; gen.match[g+1] = g;
        gen.match[n+g] = n+g+1; gen.match[n+g+1] = n+g;
        a->gen_idx[g] = find_basis_index(&gen, a->basis, a->dim, n);
    }
    printf("    Building mult table (%d x %d)...\n", a->dim, a->dim);
    for (i = 0; i < a->dim; i++) {
        for (j = 0; j < a->dim; j++) {
            PlanarMatch r; int loops;
            compose_diagrams(n, &a->basis[i], &a->basis[j], &r, &loops);
            a->mt_result[i][j] =
                find_basis_index(&r, a->basis, a->dim, n);
            a->mt_loops[i][j] = loops;
            if (loops > a->max_loops) a->max_loops = loops;
        }
    }
}

/* ================================================================
 * Utility: through-strand count, binomial, cell module dimension
 * ================================================================ */

static int count_through_strands(const PlanarMatch *m, int n) {
    int i, count = 0;
    for (i = 0; i < n; i++)
        if (m->match[i] >= n) count++;
    return count;
}

static int binom(int n, int k) {
    int i, result = 1;
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    for (i = 0; i < k; i++)
        result = result * (n - i) / (i + 1);
    return result;
}

/* d(n,j) = C(n,(n-j)/2) - C(n,(n-j)/2-1) */
static int cell_module_dim(int n, int j) {
    int k;
    if (j < 0 || j > n || (n - j) % 2 != 0) return 0;
    k = (n - j) / 2;
    return binom(n, k) - binom(n, k - 1);
}

/* ================================================================
 * Gram matrix at general delta (mod p)
 * ================================================================ */

/* Precompute delta powers */
static void make_dpow(long delta_val, int max_loops, long *dpow) {
    int k;
    dpow[0] = 1;
    for (k = 1; k <= max_loops && k <= MAX_LOOPS; k++)
        dpow[k] = mod_reduce(dpow[k-1] * delta_val);
}

/* Compute fixpt table: fixpt[p] = sum_{k:result(p,k)==k} delta^loops */
static void compute_fixpt(const TLAlgebra *alg, const long *dpow,
                           long *fixpt) {
    int i, k;
    for (i = 0; i < alg->dim; i++) {
        fixpt[i] = 0;
        for (k = 0; k < alg->dim; k++)
            if (alg->mt_result[i][k] == k)
                fixpt[i] = mod_reduce(
                    fixpt[i] + dpow[alg->mt_loops[i][k]]);
    }
}

/* Build Gram submatrix for elements in elems[].
 * gram is flat n_elems x n_elems array. */
static void build_gram_sub(const TLAlgebra *alg,
                            const long *fixpt, const long *dpow,
                            const int *elems, int n_elems,
                            long *gram) {
    int i, j;
    for (i = 0; i < n_elems; i++) {
        int si = elems[i];
        for (j = 0; j < n_elems; j++) {
            int sj = elems[j];
            int p = alg->mt_result[si][sj];
            int l = alg->mt_loops[si][sj];
            gram[i * n_elems + j] = mod_reduce(dpow[l] * fixpt[p]);
        }
    }
}

/* Forward elimination rank computation. Destroys gram in-place. */
static int compute_rank(long *gram, int dim) {
    int k, i, j, rank = 0;
    for (k = 0; k < dim; k++) {
        int pivot = -1;
        for (i = rank; i < dim; i++)
            if (gram[i * dim + k] != 0) { pivot = i; break; }
        if (pivot == -1) continue;
        if (pivot != rank) {
            for (j = 0; j < dim; j++) {
                long t = gram[rank * dim + j];
                gram[rank * dim + j] = gram[pivot * dim + j];
                gram[pivot * dim + j] = t;
            }
        }
        {
            long inv = mod_inv(gram[rank * dim + k]);
            for (i = rank + 1; i < dim; i++) {
                if (gram[i * dim + k] != 0) {
                    long fac = mod_reduce(gram[i * dim + k] * inv);
                    for (j = k; j < dim; j++)
                        gram[i * dim + j] = mod_reduce(
                            gram[i * dim + j]
                            - mod_reduce(fac * gram[rank * dim + j]));
                }
            }
        }
        rank++;
    }
    return rank;
}

/* Static buffers for Gram matrices */
static long g_sub_gram[MAX_SECTOR * MAX_SECTOR];
static long g_full_gram[MAX_BASIS * MAX_BASIS];

/* ================================================================
 * Sector analysis helpers
 * ================================================================ */

/* Classify basis elements by through-strand count.
 * Returns sector j-values in sectors[], counts in sect_count[],
 * number of distinct sectors in *n_sectors. */
static void classify_sectors(const TLAlgebra *alg,
                              int *n_through,
                              int *sectors, int *sect_count,
                              int *n_sectors) {
    int b, j, n = alg->n;
    *n_sectors = 0;

    for (b = 0; b < alg->dim; b++)
        n_through[b] = count_through_strands(&alg->basis[b], n);

    for (j = n % 2; j <= n; j += 2) {
        int count = 0;
        for (b = 0; b < alg->dim; b++)
            if (n_through[b] == j) count++;
        sectors[*n_sectors] = j;
        sect_count[*n_sectors] = count;
        (*n_sectors)++;
    }
}

/* Collect basis indices for sector with j through-strands */
static int collect_sector(const int *n_through, int dim,
                           int j, int *elems) {
    int b, n_elems = 0;
    for (b = 0; b < dim; b++)
        if (n_through[b] == j)
            elems[n_elems++] = b;
    return n_elems;
}

/* ================================================================
 * Markov trace: close a diagram and count loops.
 * Closure connects top i to bottom n+i.
 * ================================================================ */

static int closure_loops(const PlanarMatch *m, int n) {
    int visited[MAX_2N];
    int i, loops = 0;
    memset(visited, 0, (size_t)(2*n) * sizeof(int));
    for (i = 0; i < 2*n; i++) {
        if (!visited[i]) {
            int cur = i;
            loops++;
            do {
                visited[cur] = 1;
                cur = m->match[cur];  /* follow matching */
                visited[cur] = 1;
                /* closure: top i <-> bottom n+i */
                if (cur < n) cur = cur + n;
                else         cur = cur - n;
            } while (!visited[cur]);
        }
    }
    return loops;
}

/* Build Markov-trace Gram matrix for given elements.
 * G_M[i][j] = delta^{inner_loops(i,j) + closure_loops(result(i,j))} */
static void build_markov_gram(const TLAlgebra *alg,
                               const long *dpow,
                               const int *cl_loops,
                               const int *elems, int n_elems,
                               long *gram) {
    int i, j;
    for (i = 0; i < n_elems; i++) {
        int si = elems[i];
        for (j = 0; j < n_elems; j++) {
            int sj = elems[j];
            int total = alg->mt_loops[si][sj]
                        + cl_loops[alg->mt_result[si][sj]];
            gram[i * n_elems + j] = dpow[total];
        }
    }
}

/* ================================================================
 * Cell Module Gram: d(n,j) x d(n,j) bilinear form
 *
 * For sector j, extracts left/right link states from TL basis,
 * picks a reference right state r0, computes:
 *   phi(s_i, s_j) = delta^loops   if product stays in j-sector
 *                 = 0             otherwise (lower cell)
 * Returns rank of the cell module Gram matrix.
 * ================================================================ */

#define MAX_CELL_DIM 30

static int compute_cell_gram_rank(
    const TLAlgebra *alg, const long *dpow,
    const int *through_counts, int j, int n)
{
    static int left_pat[MAX_BASIS][MAX_N];
    static int right_pat[MAX_BASIS][MAX_N];
    int unique_left[MAX_CELL_DIM][MAX_N];
    int n_unique = 0;
    int ref_right[MAX_N];
    int b_for_left[MAX_CELL_DIM];
    long cell_gram[MAX_CELL_DIM * MAX_CELL_DIM];
    int bi, ei, ui, uj, k, n_elems;
    int elems[MAX_BASIS];

    /* Collect j-sector basis elements */
    n_elems = 0;
    for (bi = 0; bi < alg->dim; bi++)
        if (through_counts[bi] == j)
            elems[n_elems++] = bi;

    if (n_elems == 0) return 0;

    /* Extract left and right link state patterns */
    for (ei = 0; ei < n_elems; ei++) {
        bi = elems[ei];
        for (k = 0; k < n; k++) {
            left_pat[ei][k] = alg->basis[bi].match[k] < n
                              ? alg->basis[bi].match[k] : -1;
            right_pat[ei][k] = alg->basis[bi].match[n + k] >= n
                               ? alg->basis[bi].match[n + k] - n : -1;
        }
    }

    /* Find unique left states */
    for (ei = 0; ei < n_elems; ei++) {
        int dup = 0;
        for (ui = 0; ui < n_unique; ui++) {
            int eq = 1;
            for (k = 0; k < n; k++)
                if (left_pat[ei][k] != unique_left[ui][k])
                    { eq = 0; break; }
            if (eq) { dup = 1; break; }
        }
        if (!dup && n_unique < MAX_CELL_DIM) {
            memcpy(unique_left[n_unique], left_pat[ei],
                   (size_t)n * sizeof(int));
            n_unique++;
        }
    }

    /* Pick first right state as reference */
    memcpy(ref_right, right_pat[0], (size_t)n * sizeof(int));

    /* For each unique left state, find basis element with ref right */
    for (ui = 0; ui < n_unique; ui++) {
        b_for_left[ui] = -1;
        for (ei = 0; ei < n_elems; ei++) {
            int match_l = 1, match_r = 1;
            for (k = 0; k < n; k++) {
                if (left_pat[ei][k] != unique_left[ui][k])
                    { match_l = 0; break; }
            }
            if (!match_l) continue;
            for (k = 0; k < n; k++) {
                if (right_pat[ei][k] != ref_right[k])
                    { match_r = 0; break; }
            }
            if (match_r) {
                b_for_left[ui] = elems[ei];
                break;
            }
        }
    }

    /* Build cell module Gram matrix:
     * G[i][j] = delta^loops(transpose(b_{si,r0}) * b_{sj,r0})
     *           if product has j through-strands, else 0 */
    for (ui = 0; ui < n_unique; ui++) {
        for (uj = 0; uj < n_unique; uj++) {
            int b_si = b_for_left[ui];
            int b_sj = b_for_left[uj];
            PlanarMatch flipped;
            int fi, ri, loops, rts;

            if (b_si < 0 || b_sj < 0) {
                cell_gram[ui * n_unique + uj] = 0;
                continue;
            }

            /* Transpose b_{s_i, r0} to get b_{r0, s_i} */
            for (k = 0; k < 2 * n; k++)
                flipped.match[k] =
                    (alg->basis[b_si].match[(k + n) % (2 * n)]
                     + n) % (2 * n);

            fi = find_basis_index(&flipped, alg->basis,
                                   alg->dim, n);
            if (fi < 0) {
                cell_gram[ui * n_unique + uj] = 0;
                continue;
            }

            /* Product: b_{r0, s_i} * b_{s_j, r0} */
            ri = alg->mt_result[fi][b_sj];
            loops = alg->mt_loops[fi][b_sj];
            rts = through_counts[ri];

            cell_gram[ui * n_unique + uj] =
                (rts == j) ? dpow[loops] : 0;
        }
    }

    /* Debug: print cell Gram for small cases */
    if (n_unique <= 6) {
        int ri, ci;
        printf("      Cell Gram (%dx%d) for j=%d:\n", n_unique, n_unique, j);
        printf("      n_unique_left=%d (expect d=%d)\n",
               n_unique, cell_module_dim(n, j));
        for (ri = 0; ri < n_unique; ri++) {
            printf("      ");
            for (ci = 0; ci < n_unique; ci++)
                printf(" %4ld", cell_gram[ri * n_unique + ci]);
            printf("\n");
        }
        /* Check for missing basis elements */
        for (ui = 0; ui < n_unique; ui++) {
            if (b_for_left[ui] < 0)
                printf("      WARNING: left state %d has no"
                       " basis with ref right!\n", ui);
        }
    }

    return compute_rank(cell_gram, n_unique);
}

/* ================================================================
 * Cubic root finder for ell=7: delta = 2cos(pi/7)
 * Minimal polynomial: x^3 - x^2 - 2x + 1 = 0
 * Uses Cantor-Zassenhaus: polynomial arithmetic mod the cubic,
 * then gcd with (x+a)^{(p-1)/2} - 1 to split.
 * ================================================================ */

typedef struct { long c[3]; } Poly3; /* c[0] + c[1]*x + c[2]*x^2 */

/* Multiply two polynomials mod f(x) = x^3 - x^2 - 2x + 1 */
/* Reduction: x^3 = x^2 + 2x - 1,  x^4 = 3x^2 + x - 1 */
static Poly3 p3_mul(Poly3 a, Poly3 b) {
    Poly3 r;
    long c3 = mod_reduce(mod_reduce(a.c[1]*b.c[2]) +
                          mod_reduce(a.c[2]*b.c[1]));
    long c4 = mod_reduce(a.c[2]*b.c[2]);
    r.c[0] = mod_reduce(mod_reduce(a.c[0]*b.c[0])
                         + g_mod_p - c3 + g_mod_p - c4);
    r.c[1] = mod_reduce(mod_reduce(a.c[0]*b.c[1])
                         + mod_reduce(a.c[1]*b.c[0])
                         + mod_reduce(2*c3) + c4);
    r.c[2] = mod_reduce(mod_reduce(a.c[0]*b.c[2])
                         + mod_reduce(a.c[1]*b.c[1])
                         + mod_reduce(a.c[2]*b.c[0])
                         + c3 + mod_reduce(3*c4));
    return r;
}

static Poly3 p3_pow(Poly3 base, long exp) {
    Poly3 result;
    result.c[0] = 1; result.c[1] = 0; result.c[2] = 0;
    while (exp > 0) {
        if (exp & 1) result = p3_mul(result, base);
        base = p3_mul(base, base);
        exp >>= 1;
    }
    return result;
}

/* Verify x is a root of x^3 - x^2 - 2x + 1 mod p */
static int verify_cubic_root(long x) {
    long x2 = mod_reduce(x * x);
    long x3 = mod_reduce(x2 * x);
    long val = mod_reduce(x3 + g_mod_p - x2
                          + g_mod_p - mod_reduce(2*x) + 1);
    return val == 0;
}

static long find_l7_root(void) {
    long a;
    for (a = 0; a < 200; a++) {
        Poly3 base, h;
        long root;

        base.c[0] = mod_reduce(a); base.c[1] = 1; base.c[2] = 0;
        h = p3_pow(base, (g_mod_p - 1) / 2);
        h.c[0] = mod_reduce(h.c[0] + g_mod_p - 1);

        /* h is degree <= 2. Try to extract a root. */
        if (h.c[2] == 0 && h.c[1] == 0) continue; /* trivial */

        if (h.c[2] == 0) {
            /* Linear: root = -h0/h1 */
            root = mod_reduce(mod_reduce(g_mod_p - h.c[0])
                              * mod_inv(h.c[1]));
            if (verify_cubic_root(root)) return root;
            continue;
        }

        /* Quadratic h: compute f mod h (Euclidean division) */
        {
            long h2inv = mod_inv(h.c[2]);
            /* Eliminate x^3: f - (1/h2)*x*h */
            long q1 = h2inv; /* leading quotient coeff */
            long a2 = mod_reduce(g_mod_p - 1
                                 + g_mod_p - mod_reduce(q1 * h.c[1]));
            long a1 = mod_reduce(g_mod_p - 2
                                 + g_mod_p - mod_reduce(q1 * h.c[0]));
            long a0 = 1;

            /* Eliminate x^2 if present */
            if (a2 != 0) {
                long q0 = mod_reduce(a2 * h2inv);
                a1 = mod_reduce(a1 + g_mod_p
                                - mod_reduce(q0 * h.c[1]));
                a0 = mod_reduce(a0 + g_mod_p
                                - mod_reduce(q0 * h.c[0]));
            }

            /* Remainder a1*x + a0 */
            if (a1 == 0 && a0 == 0) {
                /* h divides f: quadratic formula on h */
                long disc = mod_reduce(mod_reduce(h.c[1]*h.c[1])
                    + g_mod_p
                    - mod_reduce(4*mod_reduce(h.c[2]*h.c[0])));
                long sq = mod_sqrt(disc);
                long inv2h = mod_inv(mod_reduce(2 * h.c[2]));
                root = mod_reduce(
                    mod_reduce(g_mod_p - h.c[1] + sq) * inv2h);
                if (verify_cubic_root(root)) return root;
                /* Try the other root */
                root = mod_reduce(
                    mod_reduce(g_mod_p - h.c[1] + g_mod_p - sq)
                    * inv2h);
                if (verify_cubic_root(root)) return root;
            } else if (a1 != 0) {
                root = mod_reduce(
                    mod_reduce(g_mod_p - a0) * mod_inv(a1));
                if (verify_cubic_root(root)) return root;
            }
        }
    }
    return -1;
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    static TLAlgebra alg;
    int n;
    char msg[256];

    /* Per-basis-element through-strand counts */
    int n_through[MAX_BASIS];
    /* Sector info */
    int sectors[MAX_N], sect_count[MAX_N], n_sectors;
    /* Sector Gram ranks (per n) */
    int sect_ranks[MAX_N];

    /* Delta values for Part E */
    long delta_sqrt2, delta_phi;

    setbuf(stdout, NULL);

    printf("KNOTAPEL DEMO 39: Symmetry Decomposition of TL_n\n");
    printf("=================================================\n");
    printf("Working mod p = %ld\n\n", g_mod_p);

    /* Precompute delta values for Part E */
    delta_sqrt2 = mod_sqrt(2);
    {
        long sqrt5 = mod_sqrt(5);
        delta_phi = mod_reduce((1 + sqrt5) * mod_inv(2));
    }
    printf("delta values (mod p): sqrt(2)=%ld, phi=%ld\n\n",
           delta_sqrt2, delta_phi);

    /* ===========================================================
     * Part A + B + C: Sector decomposition, Gram ranks, coupling
     * for n = 2..8
     * =========================================================== */

    printf("======== Parts A/B/C: Sector Analysis ========\n\n");

    for (n = 2; n <= 8; n++) {
        int s, sum_sect_rank, full_rank, coupling;
        int all_perfect_sq, all_elems[MAX_BASIS];
        long dpow[MAX_LOOPS + 1];
        long fixpt[MAX_BASIS];

        printf("=== n=%d ===\n", n);
        init_tl(&alg, n);

        classify_sectors(&alg, n_through,
                         sectors, sect_count, &n_sectors);

        /* Part A: verify sector sizes = d(n,j)^2 */
        all_perfect_sq = 1;
        printf("    Sector decomposition (dim=%d = C_%d):\n", alg.dim, n);
        printf("      j | count | d(n,j) | d^2  | match?\n");
        printf("      --|-------|--------|------|-------\n");
        for (s = 0; s < n_sectors; s++) {
            int j = sectors[s];
            int d = cell_module_dim(n, j);
            int d2 = d * d;
            int ok = (sect_count[s] == d2);
            if (!ok) all_perfect_sq = 0;
            printf("      %d | %5d | %5d  | %4d | %s\n",
                   j, sect_count[s], d, d2, ok ? "YES" : "NO");
        }

        if (n >= 4 && n <= 7) {
            sprintf(msg, "n=%d: sector sizes are perfect squares d(n,j)^2",
                    n);
            check(msg, all_perfect_sq);
        }

        /* Part B: per-sector Gram rank at delta=0 */
        make_dpow(0, alg.max_loops, dpow);
        compute_fixpt(&alg, dpow, fixpt);

        printf("\n    Per-sector Gram rank at delta=0:\n");
        printf("      j | count | rank | status\n");
        printf("      --|-------|------|-------\n");

        sum_sect_rank = 0;
        for (s = 0; s < n_sectors; s++) {
            int j = sectors[s];
            int elems[MAX_BASIS];
            int n_elems, srank;
            const char *status;

            n_elems = collect_sector(n_through, alg.dim, j, elems);
            if (n_elems > MAX_SECTOR) {
                printf("      %d | %5d | SKIP (too large)\n",
                       j, n_elems);
                sect_ranks[s] = -1;
                continue;
            }

            build_gram_sub(&alg, fixpt, dpow, elems, n_elems,
                           g_sub_gram);
            srank = compute_rank(g_sub_gram, n_elems);
            sect_ranks[s] = srank;
            sum_sect_rank += srank;

            if (n_elems == 0) status = "empty";
            else if (srank == n_elems) status = "LIVE";
            else if (srank == 0) status = "DEAD";
            else status = "PARTIAL";

            printf("      %d | %5d | %4d | %s",
                   j, n_elems, srank, status);
            if (srank > 0 && srank < n_elems)
                printf(" (%d/%d)", srank, n_elems);
            printf("\n");
        }

        /* Part B tests: j=0 dead for even n, all live for odd n */
        if (n % 2 == 0 && n <= 6) {
            sprintf(msg, "n=%d (even): j=0 sector is DEAD", n);
            check(msg, sect_ranks[0] == 0);
        }
        if (n % 2 == 1 && n <= 5) {
            int all_live = 1;
            for (s = 0; s < n_sectors; s++)
                if (sect_ranks[s] != sect_count[s])
                    all_live = 0;
            sprintf(msg, "n=%d (odd): all sectors fully LIVE", n);
            check(msg, all_live);
        }

        /* Part C: cross-sector coupling */
        if (n % 2 == 0 && n >= 4) {
            int i;
            for (i = 0; i < alg.dim; i++) all_elems[i] = i;

            printf("\n    Cross-sector coupling:\n");

            /* Recompute fixpt (same delta=0) and build full Gram */
            make_dpow(0, alg.max_loops, dpow);
            compute_fixpt(&alg, dpow, fixpt);
            build_gram_sub(&alg, fixpt, dpow, all_elems, alg.dim,
                           g_full_gram);
            full_rank = compute_rank(g_full_gram, alg.dim);

            coupling = sum_sect_rank - full_rank;
            printf("      Sum of sector ranks: %d\n", sum_sect_rank);
            printf("      Full Gram rank: %d\n", full_rank);
            printf("      Coupling (excess): %d\n", coupling);

            if (n == 4) {
                sprintf(msg,
                    "n=4: cross-sector coupling = 0 (sectors independent)");
                check(msg, coupling == 0);
            }
            if (n == 6) {
                sprintf(msg,
                    "n=6: cross-sector coupling > 0 (sectors coupled)");
                check(msg, coupling > 0);
            }
        }

        printf("\n");
    }

    /* ===========================================================
     * Part D: Cell Filtration Verification
     *
     * Verify that I_k = span{b : ts(b) <= k} is a two-sided ideal
     * by checking ts(a*b) <= min(ts(a), ts(b)) for all pairs.
     * =========================================================== */

    printf("======== Part D: Cell Filtration ========\n\n");
    {
        int test_ns[] = {4, 6};
        int ti;

        for (ti = 0; ti < 2; ti++) {
            int nn = test_ns[ti];
            int i, j, ok = 1;
            int ts[MAX_BASIS];

            printf("  n=%d: verifying ts(a*b) <= min(ts(a), ts(b))...\n",
                   nn);
            init_tl(&alg, nn);

            for (i = 0; i < alg.dim; i++)
                ts[i] = count_through_strands(&alg.basis[i], nn);

            for (i = 0; i < alg.dim && ok; i++) {
                for (j = 0; j < alg.dim && ok; j++) {
                    int res = alg.mt_result[i][j];
                    int min_ts = ts[i] < ts[j] ? ts[i] : ts[j];
                    if (res >= 0 && ts[res] > min_ts) {
                        printf("    VIOLATION: ts(b_%d * b_%d) = %d"
                               " > min(%d, %d)\n",
                               i, j, ts[res], ts[i], ts[j]);
                        ok = 0;
                    }
                }
            }

            sprintf(msg, "n=%d: cell filtration verified"
                    " (ts monotone under multiplication)", nn);
            check(msg, ok);
        }
    }

    printf("\n");

    /* ===========================================================
     * Part E: Multi-Delta Sector Liveness Comparison
     *
     * For n=4 and n=6, compare per-sector Gram rank at:
     *   delta=0, delta=1, delta=sqrt(2), delta=phi
     * =========================================================== */

    printf("======== Part E: Multi-Delta Comparison ========\n\n");
    {
        int test_ns[] = {4, 6};
        long deltas[4];
        const char *delta_names[] = {"0", "1", "sqrt(2)", "phi"};
        int ti, di;

        deltas[0] = 0;
        deltas[1] = 1;
        deltas[2] = delta_sqrt2;
        deltas[3] = delta_phi;

        for (ti = 0; ti < 2; ti++) {
            int nn = test_ns[ti];
            int all_elems[MAX_BASIS];
            int i;

            printf("  n=%d:\n", nn);
            init_tl(&alg, nn);

            classify_sectors(&alg, n_through,
                             sectors, sect_count, &n_sectors);

            for (i = 0; i < alg.dim; i++) all_elems[i] = i;

            /* Header */
            printf("    %8s |", "delta");
            for (i = 0; i < n_sectors; i++)
                printf(" j=%d", sectors[i]);
            printf(" | full\n");

            printf("    ---------|");
            for (i = 0; i < n_sectors; i++)
                printf("------");
            printf("-|------\n");

            for (di = 0; di < 4; di++) {
                long dv = deltas[di];
                long dpow[MAX_LOOPS + 1];
                long fixpt[MAX_BASIS];
                int s, full_rank;

                make_dpow(dv, alg.max_loops, dpow);
                compute_fixpt(&alg, dpow, fixpt);

                printf("    %8s |", delta_names[di]);

                for (s = 0; s < n_sectors; s++) {
                    int j = sectors[s];
                    int elems[MAX_BASIS];
                    int n_elems, srank;

                    n_elems = collect_sector(n_through, alg.dim,
                                             j, elems);
                    build_gram_sub(&alg, fixpt, dpow, elems, n_elems,
                                   g_sub_gram);
                    srank = compute_rank(g_sub_gram, n_elems);

                    if (srank == n_elems)
                        printf(" LIVE");
                    else if (srank == 0)
                        printf(" DEAD");
                    else
                        printf(" %d/%d", srank, n_elems);
                }

                /* Full Gram rank */
                build_gram_sub(&alg, fixpt, dpow, all_elems, alg.dim,
                               g_full_gram);
                full_rank = compute_rank(g_full_gram, alg.dim);
                printf(" | %4d\n", full_rank);

                /* Tests for n=4 */
                if (nn == 4 && di == 3) {
                    /* delta=phi (ell=5): n=4 < 5, semisimple */
                    sprintf(msg,
                        "n=4, delta=phi: semisimple (rank=%d = C_4=%d)",
                        full_rank, catalan[4]);
                    check(msg, full_rank == catalan[4]);
                }
                if (nn == 4 && di == 1) {
                    /* delta=1 (ell=3): non-semisimple */
                    sprintf(msg,
                        "n=4, delta=1: non-semisimple (rank=%d < C_4=%d)",
                        full_rank, catalan[4]);
                    check(msg, full_rank < catalan[4]);
                }
            }
            printf("\n");
        }
    }

    /* ===========================================================
     * Part F: Block Structure of Generator Matrices
     *
     * Build left-multiplication maps for TL generators in
     * sector-ordered basis (descending j). The cell filtration
     * ts(a*b) <= min(ts(a), ts(b)) implies block-lower-triangular
     * structure. We verify this and show that even semisimple
     * algebras (odd n at delta=0) are NOT block-diagonal in the
     * standard basis — a true change of basis is needed.
     * =========================================================== */

    printf("======== Part F: Generator Block Structure ========\n\n");
    {
        int test_ns[] = {3, 4, 5};
        int nti;

        for (nti = 0; nti < 3; nti++) {
            int nn = test_ns[nti];
            int perm[MAX_BASIS], inv_perm[MAX_BASIS];
            int blk_of[MAX_BASIS];
            int block_start[MAX_N], block_size[MAX_N], block_j_val[MAX_N];
            int n_blocks, pos, g, s, i, j;
            int all_gen_lower, any_gen_not_diag;
            long dpow[MAX_LOOPS + 1];

            printf("  n=%d:\n", nn);
            init_tl(&alg, nn);
            classify_sectors(&alg, n_through,
                             sectors, sect_count, &n_sectors);

            /* Build permutation: order by decreasing j */
            pos = 0;
            n_blocks = 0;
            for (s = n_sectors - 1; s >= 0; s--) {
                block_start[n_blocks] = pos;
                block_j_val[n_blocks] = sectors[s];
                block_size[n_blocks] = sect_count[s];
                for (i = 0; i < alg.dim; i++) {
                    if (n_through[i] == sectors[s]) {
                        perm[pos] = i;
                        blk_of[pos] = n_blocks;
                        pos++;
                    }
                }
                n_blocks++;
            }
            for (i = 0; i < alg.dim; i++)
                inv_perm[perm[i]] = i;

            printf("    Blocks (decreasing j): ");
            for (i = 0; i < n_blocks; i++)
                printf("[j=%d:%d] ", block_j_val[i], block_size[i]);
            printf("\n");

            make_dpow(0, alg.max_loops, dpow);

            all_gen_lower = 1;
            any_gen_not_diag = 0;

            for (g = 0; g < alg.n_gens; g++) {
                int gidx = alg.gen_idx[g];
                int gen_lower = 1, gen_diag = 1;
                int off_diag = 0, above_diag = 0;

                for (j = 0; j < alg.dim; j++) {
                    int old_col = perm[j];
                    int loops = alg.mt_loops[gidx][old_col];
                    long val = dpow[loops];
                    int new_row, rb, cb;

                    if (val == 0) continue;

                    new_row = inv_perm[alg.mt_result[gidx][old_col]];
                    cb = blk_of[j];
                    rb = blk_of[new_row];

                    if (rb != cb) {
                        gen_diag = 0;
                        off_diag++;
                        if (rb < cb) {
                            /* Maps to higher-j block = above diagonal */
                            gen_lower = 0;
                            above_diag++;
                        }
                    }
                }

                printf("    e_%d: ", g + 1);
                if (gen_diag)
                    printf("block-diagonal");
                else if (gen_lower)
                    printf("block-lower-tri (%d off-diag entries)", off_diag);
                else
                    printf("NOT lower-tri! (%d above-diag)", above_diag);
                printf("\n");

                if (!gen_lower) all_gen_lower = 0;
                if (!gen_diag) any_gen_not_diag = 1;
            }

            sprintf(msg,
                "n=%d: generators block-lower-triangular at delta=0", nn);
            check(msg, all_gen_lower);

            if (nn % 2 == 1 && any_gen_not_diag)
                printf("    (semisimple yet NOT block-diagonal "
                       "in standard sector basis)\n");

            printf("\n");
        }
    }

    /* ===========================================================
     * Part G: Three Gram Forms at n = ell
     *
     * At the critical boundary n = ell, where delta = 2cos(pi/ell),
     * compare three bilinear forms:
     *   1. Fixpt (regular trace): B(a,b) = sum_{k:ab(k)=k} d^loops
     *   2. Markov trace: B_M(a,b) = d^{inner_loops + closure_loops}
     *   3. Cell module Gram: d(n,j) x d(n,j), the Graham-Lehrer form
     *
     * Graham-Lehrer: TL_n(d) is semisimple iff n < ell.
     * =========================================================== */

    printf("======== Part G: Three Gram Forms at n=ell ========\n\n");
    {
        int ells[] = {2, 3, 4, 5, 6, 7};
        long dvals[6];
        const char *dnames[] =
            {"0", "1", "sqrt(2)", "phi", "sqrt(3)", "2cos(pi/7)"};
        int li;
        long delta_sqrt3 = mod_sqrt(3);
        long delta_l7 = find_l7_root();
        int cl_buf[MAX_BASIS];

        dvals[0] = 0;
        dvals[1] = 1;
        dvals[2] = delta_sqrt2;
        dvals[3] = delta_phi;
        dvals[4] = delta_sqrt3;
        dvals[5] = delta_l7;

        printf("  Additional delta values (mod p):\n");
        printf("    sqrt(3) = %ld\n", delta_sqrt3);
        printf("    2cos(pi/7) = %ld (found: %s)\n\n",
               delta_l7, delta_l7 >= 0 ? "yes" : "NO");

        for (li = 0; li < 6; li++) {
            int ell = ells[li];
            long dv = dvals[li];
            long dpow_g[MAX_LOOPS + 1];
            long fixpt_g[MAX_BASIS];
            int s, i_elem, max_cl;

            if (dv < 0 && ell == 7) {
                printf("  ell=%d: SKIPPED (root not found)\n\n", ell);
                continue;
            }

            printf("  ell=%d, n=%d, delta=%s, dim(TL_%d)=%d:\n",
                   ell, ell, dnames[li], ell, catalan[ell]);

            init_tl(&alg, ell);
            classify_sectors(&alg, n_through,
                             sectors, sect_count, &n_sectors);

            /* Precompute closure loops for all basis elements */
            max_cl = 0;
            for (i_elem = 0; i_elem < alg.dim; i_elem++) {
                cl_buf[i_elem] =
                    closure_loops(&alg.basis[i_elem], ell);
                if (cl_buf[i_elem] > max_cl)
                    max_cl = cl_buf[i_elem];
            }

            /* dpow must cover combined loops (inner + closure) */
            {
                int max_total = alg.max_loops + max_cl;
                if (max_total > MAX_LOOPS) max_total = MAX_LOOPS;
                make_dpow(dv, max_total, dpow_g);
            }
            compute_fixpt(&alg, dpow_g, fixpt_g);

            /* Per-sector table: three Gram forms */
            printf("    j  | d(n,j) | d^2 |"
                   " fixpt_rk | markov_rk | cell_rk / d\n");
            printf("    ---|--------|-----|"
                   "----------|-----------|------------\n");

            for (s = 0; s < n_sectors; s++) {
                int j = sectors[s];
                int d = cell_module_dim(ell, j);
                int elems[MAX_BASIS];
                int n_elems, fixpt_rk, markov_rk, cell_rk;

                n_elems = collect_sector(n_through, alg.dim,
                                         j, elems);

                if (n_elems > MAX_SECTOR) {
                    printf("    %-3d| %-7d| %-4d| SKIP\n",
                           j, d, n_elems);
                    continue;
                }

                /* 1. Fixpt (regular trace) sector Gram rank */
                build_gram_sub(&alg, fixpt_g, dpow_g,
                               elems, n_elems, g_sub_gram);
                fixpt_rk = compute_rank(g_sub_gram, n_elems);

                /* 2. Markov trace sector Gram rank */
                build_markov_gram(&alg, dpow_g, cl_buf,
                                  elems, n_elems, g_sub_gram);
                markov_rk = compute_rank(g_sub_gram, n_elems);

                /* 3. Cell module Gram rank (d x d matrix) */
                cell_rk = compute_cell_gram_rank(
                    &alg, dpow_g, n_through, j, ell);

                printf("    %-3d| %-7d| %-4d|"
                       " %-9d| %-10d| %d / %d",
                       j, d, n_elems,
                       fixpt_rk, markov_rk, cell_rk, d);
                if (cell_rk < d)
                    printf("  DEGEN(rad=%d)", d - cell_rk);
                printf("\n");
            }

            printf("    ---|--------|-----|"
                   "----------|-----------|------------\n");

            /* Full Gram ranks (all sectors together) */
            {
                int all_elems[MAX_BASIS];
                int full_fixpt, full_markov;

                for (i_elem = 0; i_elem < alg.dim; i_elem++)
                    all_elems[i_elem] = i_elem;

                /* Full fixpt Gram */
                build_gram_sub(&alg, fixpt_g, dpow_g,
                               all_elems, alg.dim, g_full_gram);
                full_fixpt = compute_rank(g_full_gram, alg.dim);

                /* Full Markov Gram */
                build_markov_gram(&alg, dpow_g, cl_buf,
                                  all_elems, alg.dim,
                                  g_full_gram);
                full_markov = compute_rank(g_full_gram, alg.dim);

                printf("    full ranks: fixpt=%d, markov=%d"
                       " / %d\n",
                       full_fixpt, full_markov, alg.dim);
                if (full_fixpt == alg.dim
                    && full_markov < alg.dim)
                    printf("    ** fixpt says semisimple but"
                           " markov detects radical! **\n");
            }

            /* Test: Graham-Lehrer predicts non-semisimple
             * at n = ell (since n >= ell) */
            if (ell >= 3) {
                int any_cell_degen = 0;
                for (s = 0; s < n_sectors; s++) {
                    int j = sectors[s];
                    int d = cell_module_dim(ell, j);
                    int cr = compute_cell_gram_rank(
                        &alg, dpow_g, n_through, j, ell);
                    if (cr < d) any_cell_degen = 1;
                }
                sprintf(msg,
                    "ell=%d: cell module detects non-semisimple"
                    " at n=ell (Graham-Lehrer)", ell);
                check(msg, any_cell_degen);
            }

            printf("\n");
        }
    }

    /* Summary */
    printf("=================================================\n");
    printf("Final: %d passed, %d failed\n", n_pass, n_fail);
    printf("=================================================\n");

    return n_fail > 0 ? 1 : 0;
}
