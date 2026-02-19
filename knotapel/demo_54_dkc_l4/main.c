/*
 * KNOTAPEL DEMO 54: DKC at ell=4 (delta=sqrt(2)) -- Bracket over Z[zeta_16]
 * ==========================================================================
 *
 * Two-source hypothesis test: at ell=3 (delta=1), the bracket collapsed to
 * binary {-1, +1}. Is this because ell=3 has too few representations, or
 * because delta=0 is uniquely special?
 *
 * At ell=4: SU(2) level 2 = Ising anyons. 3 simple objects.
 *   A = zeta_16^5 = e^{i*5*pi/8} (primitive 16th root of unity)
 *   delta = -(A^2 + A^{-2}) = sqrt(2) = zeta_16^2 - zeta_16^6
 *   Values live in Z[zeta_16], the 16th cyclotomic integers.
 *
 * Key differences from delta=0 and delta=1:
 *   - Ring: Z[zeta_16] (8D) instead of Z[zeta_8] (4D) or Z[omega] (2D)
 *   - e_i^2 = sqrt(2)*e_i (neither idempotent nor nilpotent!)
 *   - Loops contribute IRRATIONAL weight: delta^L = (sqrt(2))^L
 *   - A has order 16 (vs 8 at delta=0, 3 at delta=1)
 *
 * Part A: Z[zeta_16] arithmetic
 * Part B: Planar matching enumeration
 * Part C: TL generator matrices at delta=sqrt(2)
 * Part D: Braid generators
 * Part E: Braid word evaluation
 * Part F: Closure functionals and cross-validation
 *
 * C89, zero dependencies.
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
 * Complex arithmetic (for cross-checking)
 * ================================================================ */

typedef struct { double re, im; } Cx;

static Cx cx_make(double re, double im) {
    Cx z; z.re = re; z.im = im; return z;
}

static Cx cx_add(Cx a, Cx b) {
    return cx_make(a.re + b.re, a.im + b.im);
}

static Cx cx_sub(Cx a, Cx b) {
    return cx_make(a.re - b.re, a.im - b.im);
}

static Cx cx_mul(Cx a, Cx b) {
    return cx_make(a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re);
}

static double cx_abs(Cx a) {
    return sqrt(a.re*a.re + a.im*a.im);
}

static Cx cx_exp_i(double theta) {
    return cx_make(cos(theta), sin(theta));
}

/* ================================================================
 * PART A: Z[zeta_16] Arithmetic
 *
 * zeta = zeta_16 = e^{i*pi/8}, a primitive 16th root of unity.
 * Minimal polynomial: zeta^8 + 1 = 0, so zeta^8 = -1.
 * Elements: c[0] + c[1]*z + c[2]*z^2 + ... + c[7]*z^7
 *
 * Multiplication: convolution with wraparound negation.
 * For product c = a * b:
 *   c[k] = sum_{i+j=k} a[i]*b[j] - sum_{i+j=k+8} a[i]*b[j]
 *
 * Key values:
 *   A = zeta^5
 *   A^{-1} = zeta^{-5} = zeta^{11} = -zeta^3
 *   delta = -(A^2 + A^{-2}) = zeta^2 - zeta^6 = sqrt(2)
 * ================================================================ */

typedef struct { long c[8]; } Cyc16;

static Cyc16 cyc16_zero(void) {
    Cyc16 r;
    memset(r.c, 0, sizeof(r.c));
    return r;
}

static Cyc16 cyc16_one(void) {
    Cyc16 r;
    memset(r.c, 0, sizeof(r.c));
    r.c[0] = 1;
    return r;
}

static Cyc16 cyc16_make(long c0, long c1, long c2, long c3,
                         long c4, long c5, long c6, long c7) {
    Cyc16 r;
    r.c[0] = c0; r.c[1] = c1; r.c[2] = c2; r.c[3] = c3;
    r.c[4] = c4; r.c[5] = c5; r.c[6] = c6; r.c[7] = c7;
    return r;
}

/* zeta^k with reduction: zeta^8 = -1 */
static Cyc16 cyc16_zeta_power(int k) {
    Cyc16 r;
    memset(r.c, 0, sizeof(r.c));
    k = k % 16;
    if (k < 0) k += 16;
    if (k < 8) {
        r.c[k] = 1;
    } else {
        r.c[k - 8] = -1;
    }
    return r;
}

static Cyc16 cyc16_add(Cyc16 a, Cyc16 b) {
    Cyc16 r;
    int i;
    for (i = 0; i < 8; i++) r.c[i] = a.c[i] + b.c[i];
    return r;
}

static Cyc16 cyc16_neg(Cyc16 a) {
    Cyc16 r;
    int i;
    for (i = 0; i < 8; i++) r.c[i] = -a.c[i];
    return r;
}

static Cyc16 cyc16_scale(Cyc16 a, long s) {
    Cyc16 r;
    int i;
    for (i = 0; i < 8; i++) r.c[i] = a.c[i] * s;
    return r;
}

/* Multiplication: convolution mod z^8 = -1 */
static Cyc16 cyc16_mul(Cyc16 a, Cyc16 b) {
    Cyc16 r;
    int i, j;
    memset(r.c, 0, sizeof(r.c));
    for (i = 0; i < 8; i++) {
        if (a.c[i] == 0) continue;
        for (j = 0; j < 8; j++) {
            if (b.c[j] == 0) continue;
            if (i + j < 8)
                r.c[i + j] += a.c[i] * b.c[j];
            else
                r.c[i + j - 8] -= a.c[i] * b.c[j];
        }
    }
    return r;
}

static int cyc16_eq(Cyc16 a, Cyc16 b) {
    int i;
    for (i = 0; i < 8; i++)
        if (a.c[i] != b.c[i]) return 0;
    return 1;
}

static int cyc16_is_zero(Cyc16 a) {
    int i;
    for (i = 0; i < 8; i++)
        if (a.c[i] != 0) return 0;
    return 1;
}

/* Convert to complex float for display/checking */
static Cx cyc16_to_cx(Cyc16 a) {
    Cx r = cx_make(0.0, 0.0);
    int k;
    for (k = 0; k < 8; k++) {
        if (a.c[k] != 0) {
            double angle = (double)k * M_PI / 8.0;
            r = cx_add(r, cx_make((double)a.c[k] * cos(angle),
                                   (double)a.c[k] * sin(angle)));
        }
    }
    return r;
}

/* A^power: A = zeta^5, so A^k = zeta^{5k} */
static Cyc16 cyc16_a_power(int power) {
    int zp = (5 * power) % 16;
    if (zp < 0) zp += 16;
    return cyc16_zeta_power(zp);
}

/* delta^k: delta = sqrt(2), so delta^{2m} = 2^m, delta^{2m+1} = 2^m * delta
 * delta = zeta^2 - zeta^6 = (0,0,1,0,0,0,-1,0) */
static Cyc16 cyc16_delta_power(int k) {
    Cyc16 r;
    long two_pow;
    int m, i;
    memset(r.c, 0, sizeof(r.c));
    if (k == 0) { r.c[0] = 1; return r; }
    if (k < 0) {
        printf("  ERROR: negative delta power %d\n", k);
        return r;
    }
    m = k / 2;
    two_pow = 1;
    for (i = 0; i < m; i++) two_pow *= 2;
    if (k % 2 == 0) {
        r.c[0] = two_pow;
    } else {
        r.c[2] = two_pow;
        r.c[6] = -two_pow;
    }
    return r;
}

/* Strict axiality: at most one nonzero component */
static int cyc16_is_axial(Cyc16 a) {
    int nonzero = 0;
    int i;
    for (i = 0; i < 8; i++)
        if (a.c[i] != 0) nonzero++;
    return nonzero <= 1;
}

/* Z[i]-axiality: value = (a+bi)*zeta^c for integers a,b and some c.
 * Since zeta^4 = i, the 8 components pair as (0,4),(1,5),(2,6),(3,7).
 * Z[i]-axial means at most one pair has nonzero components. */
static int cyc16_is_zi_axial(Cyc16 a) {
    int pairs_active = 0;
    int k;
    for (k = 0; k < 4; k++) {
        if (a.c[k] != 0 || a.c[k + 4] != 0)
            pairs_active++;
    }
    return pairs_active <= 1;
}

/* Decompose Z[i]-axial value: find c, a_part, b_part such that
 * value = (a_part + b_part*i) * zeta^c.
 * For c in {0..3}: component c = a_part, component c+4 = b_part.
 * For c in {4..7}: component c = a_part, component c-4 = -b_part.
 * Returns 1 if decomposition found, 0 if not Z[i]-axial. */
static int cyc16_zi_decompose(Cyc16 v, int *c_out, long *a_out, long *b_out) {
    int k, active_pair = -1;
    for (k = 0; k < 4; k++) {
        if (v.c[k] != 0 || v.c[k + 4] != 0) {
            if (active_pair >= 0) return 0; /* two pairs active */
            active_pair = k;
        }
    }
    if (active_pair < 0) {
        /* zero element */
        *c_out = 0; *a_out = 0; *b_out = 0;
        return 1;
    }
    /* Canonical form: use c = active_pair (0..3) */
    *c_out = active_pair;
    *a_out = v.c[active_pair];
    *b_out = v.c[active_pair + 4];
    return 1;
}

static void cyc16_print(const char *label, Cyc16 a) {
    Cx z = cyc16_to_cx(a);
    printf("  %s = (%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld)  [%.4f + %.4fi]\n",
           label,
           a.c[0], a.c[1], a.c[2], a.c[3],
           a.c[4], a.c[5], a.c[6], a.c[7],
           z.re, z.im);
}

/* ================================================================
 * PART B: Planar Matching Enumeration
 * Same as Demo 35/53 -- enumerate non-crossing planar matchings.
 * ================================================================ */

#define MAX_N 6
#define MAX_2N (2 * MAX_N)
#define MAX_BASIS 42

typedef struct { int match[MAX_2N]; } PlanarMatch;

typedef struct {
    int points[MAX_2N];
    int count;
} Segment;

static void build_boundary_order(int n, int *order) {
    int i;
    for (i = 0; i < n; i++) order[i] = i;
    for (i = 0; i < n; i++) order[n + i] = 2 * n - 1 - i;
}

static void enum_segments(Segment *segs, int n_segs, int *match_buf,
                           PlanarMatch *basis, int *num_basis, int n) {
    int first_seg, j, k, new_n, cnt;
    int *pts;
    Segment new_segs[MAX_2N];

    if (n_segs == 0) {
        if (*num_basis < MAX_BASIS) {
            memcpy(basis[*num_basis].match, match_buf,
                   (size_t)(2 * n) * sizeof(int));
            (*num_basis)++;
        }
        return;
    }

    first_seg = -1;
    for (k = 0; k < n_segs; k++) {
        if (segs[k].count >= 2) { first_seg = k; break; }
    }
    if (first_seg < 0) return;

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

static int is_identity_matching(const PlanarMatch *m, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (m->match[i] != i + n) return 0;
    }
    return 1;
}

/* Diagram composition: stack d1 (top) on d2 (bottom).
 * Returns number of closed loops formed at the glue. */
static int compose_diagrams(int n, const PlanarMatch *d1,
                            const PlanarMatch *d2, PlanarMatch *result) {
    int glue_visited[MAX_N];
    int i, loops;

    memset(result->match, -1, (size_t)(2 * n) * sizeof(int));
    memset(glue_visited, 0, (size_t)n * sizeof(int));
    loops = 0;

    for (i = 0; i < 2 * n; i++) {
        int start = i;
        int in_d1, cur, partner;

        if (result->match[i] >= 0) continue;
        if (i < n) { in_d1 = 1; cur = i; }
        else       { in_d1 = 0; cur = i; }

        for (;;) {
            if (in_d1) {
                partner = d1->match[cur];
                if (partner < n) {
                    result->match[start] = partner;
                    result->match[partner] = start;
                    break;
                }
                glue_visited[partner - n] = 1;
                in_d1 = 0;
                cur = partner - n;
            } else {
                partner = d2->match[cur];
                if (partner >= n) {
                    result->match[start] = partner;
                    result->match[partner] = start;
                    break;
                }
                glue_visited[partner] = 1;
                in_d1 = 1;
                cur = n + partner;
            }
        }
    }

    for (i = 0; i < n; i++) {
        int cur2, p, q;
        if (glue_visited[i]) continue;
        loops++;
        cur2 = i;
        do {
            glue_visited[cur2] = 1;
            p = d2->match[cur2];
            glue_visited[p] = 1;
            q = d1->match[n + p];
            cur2 = q - n;
        } while (cur2 != i);
    }

    return loops;
}

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
            cur = m->match[cur];
            visited[cur] = 1;
            cur = (cur < n) ? cur + n : cur - n;
        } while (cur != i);
    }
    return loops;
}

/* ================================================================
 * TL Matrix over Z[zeta_16]
 * ================================================================ */

#define MAX_DIM 42

typedef struct {
    Cyc16 entries[MAX_DIM][MAX_DIM];
    int dim;
} TLMat;

static TLMat tlmat_zero(int dim) {
    TLMat m;
    int i, j;
    m.dim = dim;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            m.entries[i][j] = cyc16_zero();
    return m;
}

static TLMat tlmat_identity(int dim) {
    TLMat m;
    int i;
    m = tlmat_zero(dim);
    for (i = 0; i < dim; i++)
        m.entries[i][i] = cyc16_one();
    return m;
}

static TLMat tlmat_add(const TLMat *a, const TLMat *b) {
    TLMat m;
    int i, j;
    m.dim = a->dim;
    for (i = 0; i < m.dim; i++)
        for (j = 0; j < m.dim; j++)
            m.entries[i][j] = cyc16_add(a->entries[i][j], b->entries[i][j]);
    return m;
}

static TLMat tlmat_scale(Cyc16 s, const TLMat *a) {
    TLMat m;
    int i, j;
    m.dim = a->dim;
    for (i = 0; i < m.dim; i++)
        for (j = 0; j < m.dim; j++)
            m.entries[i][j] = cyc16_mul(s, a->entries[i][j]);
    return m;
}

static TLMat tlmat_mul(const TLMat *a, const TLMat *b) {
    TLMat m;
    int i, j, k;
    m.dim = a->dim;
    for (i = 0; i < m.dim; i++)
        for (j = 0; j < m.dim; j++) {
            m.entries[i][j] = cyc16_zero();
            for (k = 0; k < m.dim; k++)
                m.entries[i][j] = cyc16_add(m.entries[i][j],
                    cyc16_mul(a->entries[i][k], b->entries[k][j]));
        }
    return m;
}

static int tlmat_eq(const TLMat *a, const TLMat *b) {
    int i, j;
    if (a->dim != b->dim) return 0;
    for (i = 0; i < a->dim; i++)
        for (j = 0; j < a->dim; j++)
            if (!cyc16_eq(a->entries[i][j], b->entries[i][j])) return 0;
    return 1;
}

static int tlmat_nonzero_count(const TLMat *m) {
    int i, j, count = 0;
    for (i = 0; i < m->dim; i++)
        for (j = 0; j < m->dim; j++)
            if (!cyc16_is_zero(m->entries[i][j])) count++;
    return count;
}

static void tlmat_print(const TLMat *m, const char *label) {
    int i, j;
    printf("  %s (%dx%d, %d nonzero):\n", label, m->dim, m->dim,
           tlmat_nonzero_count(m));
    for (i = 0; i < m->dim; i++)
        for (j = 0; j < m->dim; j++)
            if (!cyc16_is_zero(m->entries[i][j]))
                printf("    [%d][%d] = (%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld)\n",
                       i, j,
                       m->entries[i][j].c[0], m->entries[i][j].c[1],
                       m->entries[i][j].c[2], m->entries[i][j].c[3],
                       m->entries[i][j].c[4], m->entries[i][j].c[5],
                       m->entries[i][j].c[6], m->entries[i][j].c[7]);
}

/* ================================================================
 * TL Generator Matrices at delta=sqrt(2)
 * At delta=sqrt(2): e_i^2 = sqrt(2)*e_i (delta-potent).
 * Loops contribute delta^{loop_count}.
 * ================================================================ */

static TLMat build_generator_matrix(int n, int gen,
                                     PlanarMatch *basis, int num_basis) {
    TLMat m;
    PlanarMatch gen_diag;
    int col;

    m = tlmat_zero(num_basis);
    gen_diag = make_generator_diagram(n, gen);

    for (col = 0; col < num_basis; col++) {
        PlanarMatch result;
        int loops, row;
        loops = compose_diagrams(n, &gen_diag, &basis[col], &result);
        row = find_basis_index(&result, basis, num_basis, n);
        if (row >= 0)
            m.entries[row][col] = cyc16_delta_power(loops);
    }
    return m;
}

/* Braid generators: sigma_i = A*I + A^{-1}*e_i
 * A = zeta^5 = (0,0,0,0,0,1,0,0)
 * A^{-1} = -zeta^3 = (0,0,0,-1,0,0,0,0) */
static TLMat build_braid_generator(int n, int gen, int positive,
                                    PlanarMatch *basis, int num_basis) {
    Cyc16 A = cyc16_zeta_power(5);
    Cyc16 A_inv = cyc16_neg(cyc16_zeta_power(3));
    TLMat I_mat = tlmat_identity(num_basis);
    TLMat e_mat = build_generator_matrix(n, gen, basis, num_basis);
    TLMat term1, term2;

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
 * Braid Word Evaluation
 * ================================================================ */

typedef struct {
    int gen;   /* 0-indexed: 0..n-2 */
    int sign;  /* +1 positive, -1 negative */
} BraidCrossing;

static TLMat eval_braid_word(const BraidCrossing *word, int word_len,
                              const TLMat *sigma_pos,
                              const TLMat *sigma_neg,
                              int num_basis) {
    TLMat result = tlmat_identity(num_basis);
    int i;
    for (i = 0; i < word_len; i++) {
        const TLMat *gen_mat;
        if (word[i].sign > 0)
            gen_mat = &sigma_pos[word[i].gen];
        else
            gen_mat = &sigma_neg[word[i].gen];
        result = tlmat_mul(&result, gen_mat);
    }
    return result;
}

/* Decode braid index into crossing sequence */
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

/* ================================================================
 * Closure Functionals at general delta
 *
 * Matrix method:
 *   bracket = sum_i M[i][id_idx] * delta^{trace_closure_loops(basis[i]) - 1}
 *
 * State-sum method:
 *   bracket = sum_s A^{a_power(s)} * delta^{total_loops(s) - 1}
 *   where total_loops = glue_loops + trace_closure_loops
 * ================================================================ */

static Cyc16 compute_bracket(const TLMat *braid_mat,
                              const PlanarMatch *basis, int num_basis,
                              int n, int id_idx) {
    Cyc16 bracket = cyc16_zero();
    int i;
    (void)id_idx; /* we iterate all basis elements */
    for (i = 0; i < num_basis; i++) {
        int tl = trace_closure_loops(n, &basis[i]);
        Cyc16 weight = cyc16_delta_power(tl - 1);
        Cyc16 contrib = cyc16_mul(weight, braid_mat->entries[i][id_idx]);
        bracket = cyc16_add(bracket, contrib);
    }
    return bracket;
}

static Cyc16 state_sum_bracket(int n, const BraidCrossing *word,
                                int word_len) {
    Cyc16 bracket = cyc16_zero();
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
                resolved = make_identity_diagram(n);
                a_power += word[j].sign;
            } else {
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
            Cyc16 weight = cyc16_delta_power(total - 1);
            Cyc16 contrib = cyc16_mul(weight, cyc16_a_power(a_power));
            bracket = cyc16_add(bracket, contrib);
        }
    }
    return bracket;
}

/* ================================================================
 * PART A TESTS: Z[zeta_16] Arithmetic
 * ================================================================ */

static void part_a_cyc16_sanity(void) {
    Cyc16 z, z8, z16, A, A_inv, prod, delta;
    char msg[256];

    printf("\n=== PART A: Z[zeta_16] Arithmetic ===\n\n");

    /* zeta^8 = -1 */
    z = cyc16_zeta_power(1);
    z8 = cyc16_zeta_power(8);
    check("zeta^8 = -1", cyc16_eq(z8, cyc16_neg(cyc16_one())));

    /* zeta^16 = 1 */
    z16 = cyc16_zeta_power(16);
    check("zeta^16 = 1", cyc16_eq(z16, cyc16_one()));

    /* zeta * zeta^7 = zeta^8 = -1 */
    {
        Cyc16 z7 = cyc16_zeta_power(7);
        Cyc16 p = cyc16_mul(z, z7);
        check("zeta * zeta^7 = -1", cyc16_eq(p, cyc16_neg(cyc16_one())));
    }

    /* A and A^{-1} */
    A = cyc16_zeta_power(5);
    A_inv = cyc16_neg(cyc16_zeta_power(3));

    check("A = zeta^5", cyc16_eq(A, cyc16_make(0, 0, 0, 0, 0, 1, 0, 0)));
    check("A^{-1} = -zeta^3", cyc16_eq(A_inv, cyc16_make(0, 0, 0, -1, 0, 0, 0, 0)));

    prod = cyc16_mul(A, A_inv);
    check("A * A^{-1} = 1", cyc16_eq(prod, cyc16_one()));

    /* delta = -(A^2 + A^{-2}) */
    {
        Cyc16 A_sq = cyc16_mul(A, A);
        Cyc16 A_inv_sq = cyc16_mul(A_inv, A_inv);
        delta = cyc16_neg(cyc16_add(A_sq, A_inv_sq));
        check("delta = zeta^2 - zeta^6",
              cyc16_eq(delta, cyc16_make(0, 0, 1, 0, 0, 0, -1, 0)));
    }

    /* delta^2 = 2 */
    {
        Cyc16 d2 = cyc16_mul(delta, delta);
        check("delta^2 = 2",
              cyc16_eq(d2, cyc16_make(2, 0, 0, 0, 0, 0, 0, 0)));
    }

    /* delta_power checks */
    check("delta^0 = 1",
          cyc16_eq(cyc16_delta_power(0), cyc16_one()));
    check("delta^1 = delta",
          cyc16_eq(cyc16_delta_power(1), delta));
    check("delta^2 = 2",
          cyc16_eq(cyc16_delta_power(2), cyc16_make(2, 0, 0, 0, 0, 0, 0, 0)));
    check("delta^3 = 2*delta",
          cyc16_eq(cyc16_delta_power(3), cyc16_scale(delta, 2)));
    check("delta^4 = 4",
          cyc16_eq(cyc16_delta_power(4), cyc16_make(4, 0, 0, 0, 0, 0, 0, 0)));

    /* A^k cycle: A has order 16 */
    check("A^0 = 1", cyc16_eq(cyc16_a_power(0), cyc16_one()));
    check("A^1 = zeta^5", cyc16_eq(cyc16_a_power(1), cyc16_zeta_power(5)));
    check("A^2 = -zeta^2", cyc16_eq(cyc16_a_power(2), cyc16_neg(cyc16_zeta_power(2))));
    check("A^8 = -1", cyc16_eq(cyc16_a_power(8), cyc16_neg(cyc16_one())));
    check("A^16 = 1", cyc16_eq(cyc16_a_power(16), cyc16_one()));
    check("A^{-1} = -zeta^3", cyc16_eq(cyc16_a_power(-1), A_inv));

    /* Float cross-check */
    {
        Cx cA = cx_exp_i(5.0 * M_PI / 8.0);
        Cx cA_exact = cyc16_to_cx(A);
        double err = cx_abs(cx_sub(cA, cA_exact));
        sprintf(msg, "A float matches exact (err=%.2e)", err);
        check(msg, err < 1e-10);
    }

    /* delta float cross-check */
    {
        Cx cd = cyc16_to_cx(delta);
        double err = fabs(cd.re - sqrt(2.0)) + fabs(cd.im);
        sprintf(msg, "delta float = sqrt(2) (err=%.2e)", err);
        check(msg, err < 1e-10);
    }

    /* Multiplication cross-check */
    {
        Cyc16 x = cyc16_make(1, 0, -1, 0, 2, 0, 0, 1);
        Cyc16 y = cyc16_make(0, 1, 0, -1, 0, 0, 1, 0);
        Cyc16 xy = cyc16_mul(x, y);
        Cx cx_x = cyc16_to_cx(x);
        Cx cx_y = cyc16_to_cx(y);
        double err = cx_abs(cx_sub(cx_mul(cx_x, cx_y), cyc16_to_cx(xy)));
        sprintf(msg, "mul cross-check (err=%.2e)", err);
        check(msg, err < 1e-10);
    }

    cyc16_print("A = zeta^5", A);
    cyc16_print("A^{-1} = -zeta^3", A_inv);
    cyc16_print("delta = sqrt(2)", delta);
}

/* ================================================================
 * PART B TESTS: Planar Matching Enumeration
 * ================================================================ */

static void part_b_planar_matchings(void) {
    PlanarMatch basis[MAX_BASIS];
    int count, i, found_identity;
    char msg[128];

    printf("\n=== PART B: Planar Matching Enumeration ===\n\n");

    count = enumerate_basis(2, basis);
    printf("  n=2: %d matchings (expected C_2 = 2)\n", count);
    sprintf(msg, "C_2 = 2 (got %d)", count);
    check(msg, count == 2);
    found_identity = 0;
    for (i = 0; i < count; i++)
        if (is_identity_matching(&basis[i], 2)) found_identity = 1;
    check("n=2 identity present", found_identity);

    count = enumerate_basis(3, basis);
    printf("  n=3: %d matchings (expected C_3 = 5)\n", count);
    sprintf(msg, "C_3 = 5 (got %d)", count);
    check(msg, count == 5);

    count = enumerate_basis(4, basis);
    printf("  n=4: %d matchings (expected C_4 = 14)\n", count);
    sprintf(msg, "C_4 = 14 (got %d)", count);
    check(msg, count == 14);

    count = enumerate_basis(5, basis);
    printf("  n=5: %d matchings (expected C_5 = 42)\n", count);
    sprintf(msg, "C_5 = 42 (got %d)", count);
    check(msg, count == 42);
}

/* ================================================================
 * PART C TESTS: TL Generator Matrices at delta=sqrt(2)
 * ================================================================ */

static void part_c_tl_generators(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS], basis4[MAX_BASIS];
    int nb2, nb3, nb4;
    TLMat e1_2, e1_3, e2_3, e1_4, e2_4, e3_4;
    TLMat sq, triple;
    Cyc16 delta;

    printf("\n=== PART C: TL Generator Matrices (delta=sqrt(2)) ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);
    nb4 = enumerate_basis(4, basis4);

    delta = cyc16_make(0, 0, 1, 0, 0, 0, -1, 0);

    /* n=2: e_1^2 = delta * e_1 */
    e1_2 = build_generator_matrix(2, 0, basis2, nb2);
    tlmat_print(&e1_2, "e1 (n=2, delta=sqrt(2))");
    sq = tlmat_mul(&e1_2, &e1_2);
    {
        TLMat expected = tlmat_scale(delta, &e1_2);
        check("n=2: e1^2 = delta*e1", tlmat_eq(&sq, &expected));
    }

    printf("\n");

    /* n=3 */
    e1_3 = build_generator_matrix(3, 0, basis3, nb3);
    e2_3 = build_generator_matrix(3, 1, basis3, nb3);

    sq = tlmat_mul(&e1_3, &e1_3);
    {
        TLMat expected = tlmat_scale(delta, &e1_3);
        check("n=3: e1^2 = delta*e1", tlmat_eq(&sq, &expected));
    }
    sq = tlmat_mul(&e2_3, &e2_3);
    {
        TLMat expected = tlmat_scale(delta, &e2_3);
        check("n=3: e2^2 = delta*e2", tlmat_eq(&sq, &expected));
    }

    triple = tlmat_mul(&e1_3, &e2_3);
    triple = tlmat_mul(&triple, &e1_3);
    check("n=3: e1*e2*e1 = e1", tlmat_eq(&triple, &e1_3));
    triple = tlmat_mul(&e2_3, &e1_3);
    triple = tlmat_mul(&triple, &e2_3);
    check("n=3: e2*e1*e2 = e2", tlmat_eq(&triple, &e2_3));

    printf("\n");

    /* n=4 */
    e1_4 = build_generator_matrix(4, 0, basis4, nb4);
    e2_4 = build_generator_matrix(4, 1, basis4, nb4);
    e3_4 = build_generator_matrix(4, 2, basis4, nb4);

    sq = tlmat_mul(&e1_4, &e1_4);
    {
        TLMat expected = tlmat_scale(delta, &e1_4);
        check("n=4: e1^2 = delta*e1", tlmat_eq(&sq, &expected));
    }
    sq = tlmat_mul(&e2_4, &e2_4);
    {
        TLMat expected = tlmat_scale(delta, &e2_4);
        check("n=4: e2^2 = delta*e2", tlmat_eq(&sq, &expected));
    }
    sq = tlmat_mul(&e3_4, &e3_4);
    {
        TLMat expected = tlmat_scale(delta, &e3_4);
        check("n=4: e3^2 = delta*e3", tlmat_eq(&sq, &expected));
    }

    triple = tlmat_mul(&e1_4, &e2_4);
    triple = tlmat_mul(&triple, &e1_4);
    check("n=4: e1*e2*e1 = e1", tlmat_eq(&triple, &e1_4));
    triple = tlmat_mul(&e2_4, &e3_4);
    triple = tlmat_mul(&triple, &e2_4);
    check("n=4: e2*e3*e2 = e2", tlmat_eq(&triple, &e2_4));

    {
        TLMat p1 = tlmat_mul(&e1_4, &e3_4);
        TLMat p2 = tlmat_mul(&e3_4, &e1_4);
        check("n=4: e1*e3 = e3*e1 (far commutativity)", tlmat_eq(&p1, &p2));
    }

    printf("  Nonzero counts:\n");
    printf("    n=2 e1: %d (dim=2)\n", tlmat_nonzero_count(&e1_2));
    printf("    n=3 e1: %d, e2: %d (dim=5)\n",
           tlmat_nonzero_count(&e1_3), tlmat_nonzero_count(&e2_3));
    printf("    n=4 e1: %d, e2: %d, e3: %d (dim=14)\n",
           tlmat_nonzero_count(&e1_4), tlmat_nonzero_count(&e2_4),
           tlmat_nonzero_count(&e3_4));
}

/* ================================================================
 * PART D TESTS: Braid Generators
 * ================================================================ */

static void part_d_braid_generators(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS];
    int nb2, nb3;
    TLMat s1_pos, s1_neg, s2_pos, s2_neg;
    TLMat prod, I_mat;

    printf("\n=== PART D: Braid Generators ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);

    s1_pos = build_braid_generator(2, 0, 1, basis2, nb2);
    s1_neg = build_braid_generator(2, 0, 0, basis2, nb2);

    tlmat_print(&s1_pos, "sigma_1 (n=2)");
    tlmat_print(&s1_neg, "sigma_1^{-1} (n=2)");

    I_mat = tlmat_identity(nb2);
    prod = tlmat_mul(&s1_pos, &s1_neg);
    check("n=2: sigma_1 * sigma_1^{-1} = I",
          tlmat_eq(&prod, &I_mat));
    prod = tlmat_mul(&s1_neg, &s1_pos);
    check("n=2: sigma_1^{-1} * sigma_1 = I",
          tlmat_eq(&prod, &I_mat));

    printf("\n");

    s1_pos = build_braid_generator(3, 0, 1, basis3, nb3);
    s1_neg = build_braid_generator(3, 0, 0, basis3, nb3);
    s2_pos = build_braid_generator(3, 1, 1, basis3, nb3);
    s2_neg = build_braid_generator(3, 1, 0, basis3, nb3);

    I_mat = tlmat_identity(nb3);
    prod = tlmat_mul(&s1_pos, &s1_neg);
    check("n=3: sigma_1 * sigma_1^{-1} = I",
          tlmat_eq(&prod, &I_mat));
    prod = tlmat_mul(&s2_pos, &s2_neg);
    check("n=3: sigma_2 * sigma_2^{-1} = I",
          tlmat_eq(&prod, &I_mat));

    /* Braid relation: s1*s2*s1 = s2*s1*s2 */
    {
        TLMat lhs = tlmat_mul(&s1_pos, &s2_pos);
        TLMat rhs = tlmat_mul(&s2_pos, &s1_pos);
        lhs = tlmat_mul(&lhs, &s1_pos);
        rhs = tlmat_mul(&rhs, &s2_pos);
        check("n=3: s1*s2*s1 = s2*s1*s2 (braid relation)",
              tlmat_eq(&lhs, &rhs));
    }
}

/* ================================================================
 * PART E TESTS: Braid Word Evaluation
 * ================================================================ */

static void part_e_braid_eval(void) {
    PlanarMatch basis2[MAX_BASIS];
    int nb2;
    TLMat sigma_pos[MAX_N], sigma_neg[MAX_N];
    BraidCrossing word[4];
    TLMat mat, I_mat, Z_mat;

    printf("\n=== PART E: Braid Word Evaluation ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    sigma_pos[0] = build_braid_generator(2, 0, 1, basis2, nb2);
    sigma_neg[0] = build_braid_generator(2, 0, 0, basis2, nb2);

    I_mat = tlmat_identity(nb2);
    Z_mat = tlmat_zero(nb2);

    /* s1^2 * s1^{-2} = I */
    word[0].gen = 0; word[0].sign = 1;
    word[1].gen = 0; word[1].sign = 1;
    word[2].gen = 0; word[2].sign = -1;
    word[3].gen = 0; word[3].sign = -1;
    mat = eval_braid_word(word, 4, sigma_pos, sigma_neg, nb2);
    check("n=2: s1^2 * s1^{-2} = I", tlmat_eq(&mat, &I_mat));

    /* s1^3 nonzero */
    word[0].gen = 0; word[0].sign = 1;
    word[1].gen = 0; word[1].sign = 1;
    word[2].gen = 0; word[2].sign = 1;
    mat = eval_braid_word(word, 3, sigma_pos, sigma_neg, nb2);
    check("n=2: s1^3 is nonzero", !tlmat_eq(&mat, &Z_mat));
}

/* ================================================================
 * PART F TESTS: Closure & Cross-Validation
 * ================================================================ */

static void part_f_closure(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS], basis4[MAX_BASIS];
    int nb2, nb3, nb4, id_idx_2, id_idx_3, id_idx_4, i;
    TLMat sp2[MAX_N], sn2[MAX_N];
    TLMat sp3[MAX_N], sn3[MAX_N];
    TLMat sp4[MAX_N], sn4[MAX_N];
    TLMat bmat;
    BraidCrossing word[8];
    Cyc16 b_mat, b_sum;

    printf("\n=== PART F: Closure & Cross-Validation (delta=sqrt(2)) ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);
    nb4 = enumerate_basis(4, basis4);

    id_idx_2 = -1; id_idx_3 = -1; id_idx_4 = -1;
    for (i = 0; i < nb2; i++)
        if (is_identity_matching(&basis2[i], 2)) id_idx_2 = i;
    for (i = 0; i < nb3; i++)
        if (is_identity_matching(&basis3[i], 3)) id_idx_3 = i;
    for (i = 0; i < nb4; i++)
        if (is_identity_matching(&basis4[i], 4)) id_idx_4 = i;

    check("n=2: identity in basis", id_idx_2 >= 0);
    check("n=3: identity in basis", id_idx_3 >= 0);
    check("n=4: identity in basis", id_idx_4 >= 0);

    /* Build braid generators */
    sp2[0] = build_braid_generator(2, 0, 1, basis2, nb2);
    sn2[0] = build_braid_generator(2, 0, 0, basis2, nb2);

    for (i = 0; i < 2; i++) {
        sp3[i] = build_braid_generator(3, i, 1, basis3, nb3);
        sn3[i] = build_braid_generator(3, i, 0, basis3, nb3);
    }

    for (i = 0; i < 3; i++) {
        sp4[i] = build_braid_generator(4, i, 1, basis4, nb4);
        sn4[i] = build_braid_generator(4, i, 0, basis4, nb4);
    }

    /* Trace closure loop counts */
    printf("  Trace closure loops (n=2):\n");
    for (i = 0; i < nb2; i++)
        printf("    basis[%d]: %d loops\n", i, trace_closure_loops(2, &basis2[i]));

    printf("  Trace closure loops (n=3):\n");
    for (i = 0; i < nb3; i++)
        printf("    basis[%d]: %d loops\n", i, trace_closure_loops(3, &basis3[i]));

    printf("\n");

    /* Hopf link: sigma_1^2 at n=2 */
    word[0].gen = 0; word[0].sign = 1;
    word[1].gen = 0; word[1].sign = 1;
    bmat = eval_braid_word(word, 2, sp2, sn2, nb2);
    b_mat = compute_bracket(&bmat, basis2, nb2, 2, id_idx_2);
    b_sum = state_sum_bracket(2, word, 2);
    cyc16_print("Hopf (matrix)", b_mat);
    cyc16_print("Hopf (state-sum)", b_sum);
    check("Hopf: matrix == state-sum", cyc16_eq(b_mat, b_sum));

    printf("\n");

    /* Trefoil: sigma_1^3 at n=2 */
    word[0].gen = 0; word[0].sign = 1;
    word[1].gen = 0; word[1].sign = 1;
    word[2].gen = 0; word[2].sign = 1;
    bmat = eval_braid_word(word, 3, sp2, sn2, nb2);
    b_mat = compute_bracket(&bmat, basis2, nb2, 2, id_idx_2);
    b_sum = state_sum_bracket(2, word, 3);
    cyc16_print("Trefoil (matrix)", b_mat);
    cyc16_print("Trefoil (state-sum)", b_sum);
    check("Trefoil: matrix == state-sum", cyc16_eq(b_mat, b_sum));

    printf("\n");

    /* Figure-eight: s1 s2^{-1} s1 s2^{-1} at n=3 */
    word[0].gen = 0; word[0].sign = 1;
    word[1].gen = 1; word[1].sign = -1;
    word[2].gen = 0; word[2].sign = 1;
    word[3].gen = 1; word[3].sign = -1;
    bmat = eval_braid_word(word, 4, sp3, sn3, nb3);
    b_mat = compute_bracket(&bmat, basis3, nb3, 3, id_idx_3);
    b_sum = state_sum_bracket(3, word, 4);
    cyc16_print("Figure-eight (matrix)", b_mat);
    cyc16_print("Figure-eight (state-sum)", b_sum);
    check("Figure-eight: matrix == state-sum", cyc16_eq(b_mat, b_sum));

    printf("\n");

    /* 4-strand: s1*s2*s3 */
    word[0].gen = 0; word[0].sign = 1;
    word[1].gen = 1; word[1].sign = 1;
    word[2].gen = 2; word[2].sign = 1;
    bmat = eval_braid_word(word, 3, sp4, sn4, nb4);
    b_mat = compute_bracket(&bmat, basis4, nb4, 4, id_idx_4);
    b_sum = state_sum_bracket(4, word, 3);
    cyc16_print("4-strand s1*s2*s3 (matrix)", b_mat);
    cyc16_print("4-strand s1*s2*s3 (state-sum)", b_sum);
    check("4-strand: matrix == state-sum", cyc16_eq(b_mat, b_sum));

    printf("\n");

    /* 4-strand far: s1^2 * s3^2 (non-interleaving) */
    word[0].gen = 0; word[0].sign = 1;
    word[1].gen = 0; word[1].sign = 1;
    word[2].gen = 2; word[2].sign = 1;
    word[3].gen = 2; word[3].sign = 1;
    bmat = eval_braid_word(word, 4, sp4, sn4, nb4);
    b_mat = compute_bracket(&bmat, basis4, nb4, 4, id_idx_4);
    b_sum = state_sum_bracket(4, word, 4);
    cyc16_print("4-strand s1^2*s3^2 (matrix)", b_mat);
    cyc16_print("4-strand s1^2*s3^2 (state-sum)", b_sum);
    check("4-strand far: matrix == state-sum", cyc16_eq(b_mat, b_sum));
    printf("  NOTE: at delta=0 this bracket is 0 (entanglement vanishing)\n");
    printf("  At delta=sqrt(2): nonzero = %s\n",
           cyc16_is_zero(b_mat) ? "NO (still zero!)" : "YES (broken!)");
}

/* ================================================================
 * EXPERIMENTS
 * ================================================================ */

/* Catalog entry for storing distinct bracket values */
typedef struct {
    Cyc16 val;
} CatalogEntry;

#define MAX_CATALOG 4096

static int catalog_contains(const CatalogEntry *cat, int n_cat, Cyc16 val) {
    int i;
    for (i = 0; i < n_cat; i++)
        if (cyc16_eq(cat[i].val, val)) return 1;
    return 0;
}

static void experiment_1_catalog(void) {
    PlanarMatch basis3[MAX_BASIS], basis4[MAX_BASIS];
    int nb3, nb4, id_idx_3, id_idx_4, i;
    TLMat sp3[MAX_N], sn3[MAX_N];
    TLMat sp4[MAX_N], sn4[MAX_N];

    printf("\n=== EXPERIMENT 1: Bracket Catalog (delta=sqrt(2)) ===\n\n");

    nb3 = enumerate_basis(3, basis3);
    nb4 = enumerate_basis(4, basis4);

    id_idx_3 = -1; id_idx_4 = -1;
    for (i = 0; i < nb3; i++)
        if (is_identity_matching(&basis3[i], 3)) id_idx_3 = i;
    for (i = 0; i < nb4; i++)
        if (is_identity_matching(&basis4[i], 4)) id_idx_4 = i;

    for (i = 0; i < 2; i++) {
        sp3[i] = build_braid_generator(3, i, 1, basis3, nb3);
        sn3[i] = build_braid_generator(3, i, 0, basis3, nb3);
    }
    for (i = 0; i < 3; i++) {
        sp4[i] = build_braid_generator(4, i, 1, basis4, nb4);
        sn4[i] = build_braid_generator(4, i, 0, basis4, nb4);
    }

    /* n=3 catalog */
    {
        static CatalogEntry catalog[MAX_CATALOG];
        int n_cat = 0;
        int n_choices = 4; /* 2 generators * 2 signs */
        int length, n_axial;

        printf("  n=3 (dim=5, %d crossing choices):\n", n_choices);

        for (length = 1; length <= 8; length++) {
            int total = 1;
            int nonzero = 0, new_vals = 0;
            int idx;

            for (i = 0; i < length; i++) total *= n_choices;

            for (idx = 0; idx < total; idx++) {
                BraidCrossing word[20];
                TLMat mat;
                Cyc16 bracket;

                decode_braid(idx, n_choices, length, word);
                mat = eval_braid_word(word, length, sp3, sn3, nb3);
                bracket = compute_bracket(&mat, basis3, nb3, 3, id_idx_3);

                if (!cyc16_is_zero(bracket)) nonzero++;
                if (!catalog_contains(catalog, n_cat, bracket) && n_cat < MAX_CATALOG) {
                    catalog[n_cat].val = bracket;
                    n_cat++;
                    new_vals++;
                }
            }

            printf("    length %d: %5d braids, %5d nonzero, %3d new (catalog: %d)\n",
                   length, total, nonzero, new_vals, n_cat);
        }

        n_axial = 0;
        {
            int n_zi_axial = 0;
            for (i = 0; i < n_cat; i++) {
                if (cyc16_is_axial(catalog[i].val)) n_axial++;
                if (cyc16_is_zi_axial(catalog[i].val)) n_zi_axial++;
            }
            printf("\n  n=3 catalog: %d values, %d Z-axial, %d Z[i]-axial, %d truly mixed\n",
                   n_cat, n_axial, n_zi_axial, n_cat - n_zi_axial);
        }

        printf("\n  n=3 full catalog:\n");
        for (i = 0; i < n_cat && i < 50; i++) {
            Cx z = cyc16_to_cx(catalog[i].val);
            int c_val; long a_val, b_val;
            printf("    [%2d] (%3ld,%3ld,%3ld,%3ld,%3ld,%3ld,%3ld,%3ld)",
                   i,
                   catalog[i].val.c[0], catalog[i].val.c[1],
                   catalog[i].val.c[2], catalog[i].val.c[3],
                   catalog[i].val.c[4], catalog[i].val.c[5],
                   catalog[i].val.c[6], catalog[i].val.c[7]);
            printf("  [%8.4f + %8.4fi]", z.re, z.im);
            if (cyc16_zi_decompose(catalog[i].val, &c_val, &a_val, &b_val))
                printf("  = (%ld+%ldi)*z^%d", a_val, b_val, c_val);
            else
                printf("  NOT Z[i]-axial");
            printf("\n");
        }
        if (n_cat > 50) printf("    ... (%d more)\n", n_cat - 50);
    }

    printf("\n");

    /* n=4 catalog */
    {
        static CatalogEntry catalog[MAX_CATALOG];
        int n_cat = 0;
        int n_choices = 6; /* 3 generators * 2 signs */
        int length, n_axial;

        printf("  n=4 (dim=14, %d crossing choices):\n", n_choices);

        for (length = 1; length <= 5; length++) {
            int total = 1;
            int nonzero = 0, new_vals = 0;
            int idx;

            for (i = 0; i < length; i++) total *= n_choices;

            for (idx = 0; idx < total; idx++) {
                BraidCrossing word[20];
                TLMat mat;
                Cyc16 bracket;

                decode_braid(idx, n_choices, length, word);
                mat = eval_braid_word(word, length, sp4, sn4, nb4);
                bracket = compute_bracket(&mat, basis4, nb4, 4, id_idx_4);

                if (!cyc16_is_zero(bracket)) nonzero++;
                if (!catalog_contains(catalog, n_cat, bracket) && n_cat < MAX_CATALOG) {
                    catalog[n_cat].val = bracket;
                    n_cat++;
                    new_vals++;
                }
            }

            printf("    length %d: %5d braids, %5d nonzero, %3d new (catalog: %d)\n",
                   length, total, nonzero, new_vals, n_cat);
        }

        n_axial = 0;
        {
            int n_zi_axial = 0;
            for (i = 0; i < n_cat; i++) {
                if (cyc16_is_axial(catalog[i].val)) n_axial++;
                if (cyc16_is_zi_axial(catalog[i].val)) n_zi_axial++;
            }
            printf("\n  n=4 catalog: %d values, %d Z-axial, %d Z[i]-axial, %d truly mixed\n",
                   n_cat, n_axial, n_zi_axial, n_cat - n_zi_axial);
        }

        printf("\n  n=4 full catalog:\n");
        for (i = 0; i < n_cat && i < 50; i++) {
            Cx z = cyc16_to_cx(catalog[i].val);
            int c_val; long a_val, b_val;
            printf("    [%2d] (%3ld,%3ld,%3ld,%3ld,%3ld,%3ld,%3ld,%3ld)",
                   i,
                   catalog[i].val.c[0], catalog[i].val.c[1],
                   catalog[i].val.c[2], catalog[i].val.c[3],
                   catalog[i].val.c[4], catalog[i].val.c[5],
                   catalog[i].val.c[6], catalog[i].val.c[7]);
            printf("  [%8.4f + %8.4fi]", z.re, z.im);
            if (cyc16_zi_decompose(catalog[i].val, &c_val, &a_val, &b_val))
                printf("  = (%ld+%ldi)*z^%d", a_val, b_val, c_val);
            else
                printf("  NOT Z[i]-axial");
            printf("\n");
        }
        if (n_cat > 50) printf("    ... (%d more)\n", n_cat - 50);
    }
}

static void experiment_2_axiality(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS];
    int nb2, nb3, id_idx_2, id_idx_3, i;
    TLMat sp2[MAX_N], sn2[MAX_N];
    TLMat sp3[MAX_N], sn3[MAX_N];
    int total_tested = 0, total_axial = 0, total_zi_axial = 0;

    printf("\n=== EXPERIMENT 2: Axiality Test (delta=sqrt(2)) ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);

    id_idx_2 = -1; id_idx_3 = -1;
    for (i = 0; i < nb2; i++)
        if (is_identity_matching(&basis2[i], 2)) id_idx_2 = i;
    for (i = 0; i < nb3; i++)
        if (is_identity_matching(&basis3[i], 3)) id_idx_3 = i;

    sp2[0] = build_braid_generator(2, 0, 1, basis2, nb2);
    sn2[0] = build_braid_generator(2, 0, 0, basis2, nb2);

    for (i = 0; i < 2; i++) {
        sp3[i] = build_braid_generator(3, i, 1, basis3, nb3);
        sn3[i] = build_braid_generator(3, i, 0, basis3, nb3);
    }

    printf("  n=2 (dim=2, 2 crossing choices):\n");
    {
        int n_choices = 2;
        int length;
        for (length = 1; length <= 10; length++) {
            int total = 1;
            int z_ax = 0, zi_ax = 0;
            int idx;
            for (i = 0; i < length; i++) total *= n_choices;
            for (idx = 0; idx < total; idx++) {
                BraidCrossing word[20];
                TLMat mat;
                Cyc16 bracket;
                decode_braid(idx, n_choices, length, word);
                mat = eval_braid_word(word, length, sp2, sn2, nb2);
                bracket = compute_bracket(&mat, basis2, nb2, 2, id_idx_2);
                if (cyc16_is_axial(bracket)) z_ax++;
                if (cyc16_is_zi_axial(bracket)) zi_ax++;
            }
            printf("    length %2d: %5d braids, %5d Z-axial, %5d Z[i]-axial, %d truly mixed\n",
                   length, total, z_ax, zi_ax, total - zi_ax);
            total_tested += total;
            total_axial += z_ax;
            total_zi_axial += zi_ax;
        }
    }

    printf("\n  n=3 (dim=5, 4 crossing choices):\n");
    {
        int n_choices = 4;
        int length;
        for (length = 1; length <= 8; length++) {
            int total = 1;
            int z_ax = 0, zi_ax = 0;
            int idx;
            for (i = 0; i < length; i++) total *= n_choices;
            for (idx = 0; idx < total; idx++) {
                BraidCrossing word[20];
                TLMat mat;
                Cyc16 bracket;
                decode_braid(idx, n_choices, length, word);
                mat = eval_braid_word(word, length, sp3, sn3, nb3);
                bracket = compute_bracket(&mat, basis3, nb3, 3, id_idx_3);
                if (cyc16_is_axial(bracket)) z_ax++;
                if (cyc16_is_zi_axial(bracket)) zi_ax++;
            }
            printf("    length %2d: %5d braids, %5d Z-axial, %5d Z[i]-axial, %d truly mixed\n",
                   length, total, z_ax, zi_ax, total - zi_ax);
            total_tested += total;
            total_axial += z_ax;
            total_zi_axial += zi_ax;
        }
    }

    printf("\n  TOTAL: %d braids\n", total_tested);
    printf("    Z-axial:    %d (%.1f%%)\n", total_axial,
           100.0 * (double)total_axial / (double)total_tested);
    printf("    Z[i]-axial: %d (%.1f%%)\n", total_zi_axial,
           100.0 * (double)total_zi_axial / (double)total_tested);
    printf("    truly mixed: %d\n", total_tested - total_zi_axial);
    if (total_zi_axial == total_tested)
        printf("  RESULT: Z[i]-AXIALITY HOLDS at delta=sqrt(2)!\n");
    else
        printf("  RESULT: Z[i]-axiality BROKEN (%d truly mixed)\n",
               total_tested - total_zi_axial);
}

static void experiment_3_entanglement(void) {
    PlanarMatch basis4[MAX_BASIS];
    int nb4, id_idx_4, i;
    TLMat sp4[MAX_N], sn4[MAX_N];
    int total_braids = 0, total_zero = 0;

    printf("\n=== EXPERIMENT 3: Entanglement (delta=sqrt(2), n=4) ===\n\n");

    nb4 = enumerate_basis(4, basis4);
    id_idx_4 = -1;
    for (i = 0; i < nb4; i++)
        if (is_identity_matching(&basis4[i], 4)) id_idx_4 = i;

    for (i = 0; i < 3; i++) {
        sp4[i] = build_braid_generator(4, i, 1, basis4, nb4);
        sn4[i] = build_braid_generator(4, i, 0, basis4, nb4);
    }

    printf("  Non-interleaving (sigma_1, sigma_3 only):\n");
    {
        int length;
        /* Only use generators 0 and 2 (sigma_1 and sigma_3) */
        for (length = 1; length <= 5; length++) {
            int n_choices = 4; /* gen0+, gen0-, gen2+, gen2- */
            int total = 1;
            int zero_count = 0, nonzero_count = 0;
            int idx;

            for (i = 0; i < length; i++) total *= n_choices;

            for (idx = 0; idx < total; idx++) {
                BraidCrossing word[20];
                TLMat mat;
                Cyc16 bracket;
                int j, tmp;

                /* Decode using 4 choices: 0=gen0+, 1=gen0-, 2=gen2+, 3=gen2- */
                tmp = idx;
                for (j = 0; j < length; j++) {
                    int choice = tmp % 4;
                    word[j].gen = (choice < 2) ? 0 : 2;
                    word[j].sign = (choice % 2 == 0) ? 1 : -1;
                    tmp /= 4;
                }

                mat = eval_braid_word(word, length, sp4, sn4, nb4);
                bracket = compute_bracket(&mat, basis4, nb4, 4, id_idx_4);

                if (cyc16_is_zero(bracket)) zero_count++;
                else nonzero_count++;
            }

            printf("    length %d: %5d braids, %4d zero, %4d nonzero\n",
                   length, total, zero_count, nonzero_count);
            total_braids += total;
            total_zero += zero_count;
        }
    }

    printf("\n  Non-interleaving total: %d braids, %d zero, %d nonzero (%.1f%%)\n",
           total_braids, total_zero, total_braids - total_zero,
           100.0 * (double)(total_braids - total_zero) / (double)total_braids);
    if (total_zero == total_braids)
        printf("  RESULT: Entanglement vanishing HOLDS at delta=sqrt(2)\n");
    else if (total_zero == 0)
        printf("  RESULT: Entanglement vanishing BROKEN at delta=sqrt(2)\n");
    else
        printf("  RESULT: Entanglement vanishing PARTIAL at delta=sqrt(2)\n");
}

static void experiment_4_compression(void) {
    PlanarMatch basis3[MAX_BASIS];
    int nb3, id_idx_3, i;
    TLMat sp3[MAX_N], sn3[MAX_N];

    printf("\n=== EXPERIMENT 4: Compression Ratios (delta=sqrt(2)) ===\n\n");

    nb3 = enumerate_basis(3, basis3);
    id_idx_3 = -1;
    for (i = 0; i < nb3; i++)
        if (is_identity_matching(&basis3[i], 3)) id_idx_3 = i;

    for (i = 0; i < 2; i++) {
        sp3[i] = build_braid_generator(3, i, 1, basis3, nb3);
        sn3[i] = build_braid_generator(3, i, 0, basis3, nb3);
    }

    printf("  n=3 (dim=5, 4 crossing choices):\n");
    {
        int n_choices = 4;
        int length;

        for (length = 1; length <= 6; length++) {
            static CatalogEntry distinct[MAX_CATALOG];
            int n_distinct = 0;
            int total = 1;
            int idx;

            for (i = 0; i < length; i++) total *= n_choices;

            for (idx = 0; idx < total; idx++) {
                BraidCrossing word[20];
                TLMat mat;
                Cyc16 bracket;

                decode_braid(idx, n_choices, length, word);
                mat = eval_braid_word(word, length, sp3, sn3, nb3);
                bracket = compute_bracket(&mat, basis3, nb3, 3, id_idx_3);

                if (!catalog_contains(distinct, n_distinct, bracket) &&
                    n_distinct < MAX_CATALOG) {
                    distinct[n_distinct].val = bracket;
                    n_distinct++;
                }
            }

            printf("    length %d: %5d braids -> %4d distinct (%5.1fx)\n",
                   length, total, n_distinct,
                   (double)total / (double)n_distinct);
        }
    }
}

/* ================================================================
 * EXPERIMENT 5: Gaussian Integer Catalog Analysis
 *
 * At delta=sqrt(2), every bracket value = (a+bi)*zeta^c.
 * Strip the axis c: how many distinct Gaussian integers (a+bi) appear?
 * Are there negation pairs: (a+bi) and -(a+bi) = (-a-bi)?
 * This is the precursor to DKC Boolean function search.
 * ================================================================ */

typedef struct {
    long a, b;  /* Gaussian integer a + bi */
} GaussInt;

static int gi_eq(GaussInt x, GaussInt y) {
    return x.a == y.a && x.b == y.b;
}

#define MAX_GI_CATALOG 4096

static int gi_catalog_contains(const GaussInt *cat, int n, GaussInt v) {
    int i;
    for (i = 0; i < n; i++)
        if (gi_eq(cat[i], v)) return 1;
    return 0;
}

static void experiment_5_gaussian_catalog(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS], basis4[MAX_BASIS];
    int nb2, nb3, nb4, id_idx_2, id_idx_3, id_idx_4, i;
    TLMat sp2[MAX_N], sn2[MAX_N];
    TLMat sp3[MAX_N], sn3[MAX_N];
    TLMat sp4[MAX_N], sn4[MAX_N];

    printf("\n=== EXPERIMENT 5: Gaussian Integer Catalog ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);
    nb4 = enumerate_basis(4, basis4);

    id_idx_2 = -1; id_idx_3 = -1; id_idx_4 = -1;
    for (i = 0; i < nb2; i++)
        if (is_identity_matching(&basis2[i], 2)) id_idx_2 = i;
    for (i = 0; i < nb3; i++)
        if (is_identity_matching(&basis3[i], 3)) id_idx_3 = i;
    for (i = 0; i < nb4; i++)
        if (is_identity_matching(&basis4[i], 4)) id_idx_4 = i;

    sp2[0] = build_braid_generator(2, 0, 1, basis2, nb2);
    sn2[0] = build_braid_generator(2, 0, 0, basis2, nb2);
    for (i = 0; i < 2; i++) {
        sp3[i] = build_braid_generator(3, i, 1, basis3, nb3);
        sn3[i] = build_braid_generator(3, i, 0, basis3, nb3);
    }
    for (i = 0; i < 3; i++) {
        sp4[i] = build_braid_generator(4, i, 1, basis4, nb4);
        sn4[i] = build_braid_generator(4, i, 0, basis4, nb4);
    }

    /* Process each strand count */
    {
        int nn;
        struct {
            int n;
            int n_choices;
            int max_length;
            PlanarMatch *basis;
            int num_basis;
            int id_idx;
            TLMat *sp;
            TLMat *sn;
        } configs[3];

        configs[0].n = 2; configs[0].n_choices = 2; configs[0].max_length = 10;
        configs[0].basis = basis2; configs[0].num_basis = nb2;
        configs[0].id_idx = id_idx_2; configs[0].sp = sp2; configs[0].sn = sn2;

        configs[1].n = 3; configs[1].n_choices = 4; configs[1].max_length = 8;
        configs[1].basis = basis3; configs[1].num_basis = nb3;
        configs[1].id_idx = id_idx_3; configs[1].sp = sp3; configs[1].sn = sn3;

        configs[2].n = 4; configs[2].n_choices = 6; configs[2].max_length = 5;
        configs[2].basis = basis4; configs[2].num_basis = nb4;
        configs[2].id_idx = id_idx_4; configs[2].sp = sp4; configs[2].sn = sn4;

        for (nn = 0; nn < 3; nn++) {
            static GaussInt gi_catalog[MAX_GI_CATALOG];
            int n_gi = 0;
            int length;
            int axis_counts[4]; /* how many braids land on each axis */
            int neg_pairs;

            memset(axis_counts, 0, sizeof(axis_counts));

            printf("  --- n=%d (choices=%d, max_length=%d) ---\n",
                   configs[nn].n, configs[nn].n_choices, configs[nn].max_length);

            for (length = 1; length <= configs[nn].max_length; length++) {
                int total = 1;
                int idx;

                for (i = 0; i < length; i++) total *= configs[nn].n_choices;

                for (idx = 0; idx < total; idx++) {
                    BraidCrossing word[20];
                    TLMat mat;
                    Cyc16 bracket;
                    int c_val;
                    long a_val, b_val;
                    GaussInt gi;

                    decode_braid(idx, configs[nn].n_choices, length, word);
                    mat = eval_braid_word(word, length,
                                          configs[nn].sp, configs[nn].sn,
                                          configs[nn].num_basis);
                    bracket = compute_bracket(&mat, configs[nn].basis,
                                              configs[nn].num_basis,
                                              configs[nn].n, configs[nn].id_idx);

                    if (cyc16_is_zero(bracket)) continue;

                    if (!cyc16_zi_decompose(bracket, &c_val, &a_val, &b_val))
                        continue; /* should not happen */

                    axis_counts[c_val]++;

                    gi.a = a_val; gi.b = b_val;
                    if (!gi_catalog_contains(gi_catalog, n_gi, gi) &&
                        n_gi < MAX_GI_CATALOG) {
                        gi_catalog[n_gi] = gi;
                        n_gi++;
                    }
                }
            }

            printf("  Distinct Gaussian integers (ignoring axis): %d\n", n_gi);
            printf("  Axis distribution: c=0: %d, c=1: %d, c=2: %d, c=3: %d\n",
                   axis_counts[0], axis_counts[1], axis_counts[2], axis_counts[3]);

            /* Print all Gaussian integers */
            printf("  Full Gaussian integer catalog:\n");
            for (i = 0; i < n_gi && i < 100; i++) {
                printf("    [%2d] %ld + %ldi", i, gi_catalog[i].a, gi_catalog[i].b);
                if (gi_catalog[i].b == 0)
                    printf("  (real: %ld)", gi_catalog[i].a);
                else if (gi_catalog[i].a == 0)
                    printf("  (pure imaginary: %ldi)", gi_catalog[i].b);
                printf("\n");
            }
            if (n_gi > 100) printf("    ... (%d more)\n", n_gi - 100);

            /* Check for negation pairs: (a+bi) and (-a-bi) both in catalog */
            neg_pairs = 0;
            for (i = 0; i < n_gi; i++) {
                GaussInt neg;
                neg.a = -gi_catalog[i].a;
                neg.b = -gi_catalog[i].b;
                if (!gi_eq(gi_catalog[i], neg) &&
                    gi_catalog_contains(gi_catalog, n_gi, neg)) {
                    neg_pairs++;
                }
            }
            neg_pairs /= 2; /* each pair counted twice */
            printf("  Negation pairs: %d (out of %d values)\n", neg_pairs, n_gi);
            if (neg_pairs > 0) {
                printf("  Negation pair details:\n");
                for (i = 0; i < n_gi; i++) {
                    GaussInt neg;
                    int j2;
                    neg.a = -gi_catalog[i].a;
                    neg.b = -gi_catalog[i].b;
                    /* Only print if neg is in catalog AND i < index of neg */
                    for (j2 = i + 1; j2 < n_gi; j2++) {
                        if (gi_eq(gi_catalog[j2], neg)) {
                            printf("    (%ld+%ldi) <-> (%ld+%ldi)\n",
                                   gi_catalog[i].a, gi_catalog[i].b,
                                   neg.a, neg.b);
                            break;
                        }
                    }
                }
            }

            /* Check for conjugation pairs: (a+bi) and (a-bi) */
            {
                int conj_pairs = 0;
                for (i = 0; i < n_gi; i++) {
                    GaussInt conj;
                    conj.a = gi_catalog[i].a;
                    conj.b = -gi_catalog[i].b;
                    if (gi_catalog[i].b != 0 &&
                        gi_catalog_contains(gi_catalog, n_gi, conj))
                        conj_pairs++;
                }
                conj_pairs /= 2;
                printf("  Conjugation pairs (a+bi, a-bi): %d\n", conj_pairs);
            }

            /* Check which Gaussian integers have norm 1 (units) */
            {
                int units = 0;
                for (i = 0; i < n_gi; i++) {
                    long norm = gi_catalog[i].a * gi_catalog[i].a +
                                gi_catalog[i].b * gi_catalog[i].b;
                    if (norm == 1) units++;
                }
                printf("  Units (norm=1): %d\n", units);
            }

            printf("\n");
        }
    }
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 54: DKC at ell=4 (delta=sqrt(2)) -- "
           "Bracket over Z[zeta_16]\n");
    printf("===================================================================\n");

    part_a_cyc16_sanity();
    part_b_planar_matchings();
    part_c_tl_generators();
    part_d_braid_generators();
    part_e_braid_eval();
    part_f_closure();

    printf("\n===================================================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("===================================================================\n");

    experiment_1_catalog();
    experiment_2_axiality();
    experiment_3_entanglement();
    experiment_4_compression();
    experiment_5_gaussian_catalog();

    printf("\n===================================================================\n");
    printf("Final: %d passed, %d failed\n", n_pass, n_fail);
    printf("===================================================================\n");

    return n_fail > 0 ? 1 : 0;
}
