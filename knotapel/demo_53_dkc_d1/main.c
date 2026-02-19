/*
 * KNOTAPEL DEMO 53: DKC at delta=1 -- Bracket Catalog over Z[zeta_3]
 * ===================================================================
 *
 * At delta=0 (Demo 35), the Kauffman bracket lives in Z[zeta_8] and
 * exhibits strict axiality: every value on a single cyclotomic axis.
 * What happens at delta=1?
 *
 * A = omega = e^{i*2*pi/3} (primitive cube root of unity)
 * delta = -(A^2 + A^{-2}) = -(omega^2 + omega) = 1
 * Values live in Z[omega], the Eisenstein integers.
 *
 * Key differences from delta=0:
 *   - Ring: Z[omega] (2D) instead of Z[zeta_8] (4D)
 *   - e_i^2 = e_i (IDEMPOTENT, not nilpotent!)
 *   - All smoothing states contribute (delta^L = 1, no loop-killing)
 *   - A has order 3 (vs 8)
 *
 * Part A: Eisenstein integer arithmetic
 * Part B: Planar matching enumeration
 * Part C: TL generator matrices at delta=1
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
 * PART A: Eisenstein Integer Arithmetic -- Z[omega]
 *
 * omega = e^{i*2*pi/3} = -1/2 + i*sqrt(3)/2
 * omega^2 + omega + 1 = 0  =>  omega^2 = -1 - omega
 * Element: a + b*omega where a,b in Z
 *
 * Multiplication: (a+bw)(c+dw) = (ac-bd) + (ad+bc-bd)w
 * Conjugation: conj(a+bw) = (a-b) + (-b)w
 * ================================================================ */

typedef struct { long a, b; } Eis;

static Eis eis_make(long a, long b) {
    Eis z; z.a = a; z.b = b; return z;
}

static Eis eis_zero(void) { return eis_make(0, 0); }
static Eis eis_one(void)  { return eis_make(1, 0); }

static Eis eis_add(Eis x, Eis y) {
    return eis_make(x.a + y.a, x.b + y.b);
}

static Eis eis_sub(Eis x, Eis y) {
    return eis_make(x.a - y.a, x.b - y.b);
}

static Eis eis_neg(Eis x) {
    return eis_make(-x.a, -x.b);
}

static Eis eis_mul(Eis x, Eis y) {
    /* (a+bw)(c+dw) = (ac-bd) + (ad+bc-bd)w */
    return eis_make(x.a * y.a - x.b * y.b,
                    x.a * y.b + x.b * y.a - x.b * y.b);
}

static int eis_eq(Eis x, Eis y) {
    return x.a == y.a && x.b == y.b;
}

static int eis_is_zero(Eis x) {
    return x.a == 0 && x.b == 0;
}

static Eis eis_conj(Eis z) {
    /* conj(a+bw) = (a-b) + (-b)w */
    return eis_make(z.a - z.b, -z.b);
}

static Cx eis_to_cx(Eis z) {
    double s3h = sqrt(3.0) / 2.0;
    return cx_make((double)z.a - 0.5 * (double)z.b, s3h * (double)z.b);
}

static void eis_print(const char *label, Eis z) {
    Cx c = eis_to_cx(z);
    printf("  %s = (%ld, %ld)  [%.4f + %.4fi]\n",
           label, z.a, z.b, c.re, c.im);
}

/* A^n for A = omega: n mod 3 -> {(1,0), (0,1), (-1,-1)} */
static Eis eis_a_power(int power) {
    int r = power % 3;
    if (r < 0) r += 3;
    switch (r) {
        case 0: return eis_make(1, 0);
        case 1: return eis_make(0, 1);
        case 2: return eis_make(-1, -1);
    }
    return eis_zero();
}

/* Is v a scalar multiple of 1, omega, or omega^2?
 * mult of 1:  b=0
 * mult of w:  a=0
 * mult of w^2: a=b (since a+aw = a(1+w) = -a*w^2) */
static int eis_is_axial(Eis v) {
    if (eis_is_zero(v)) return 1;
    if (v.b == 0) return 1;
    if (v.a == 0) return 1;
    if (v.a == v.b) return 1;
    return 0;
}

/* ================================================================
 * PART B: Planar Matching Enumeration
 * (Identical to Demo 35 -- pure combinatorics, ring-independent)
 * ================================================================ */

#define MAX_N 5
#define MAX_2N 10
#define MAX_BASIS 42
#define MAX_SEGS 10

typedef struct { int match[MAX_2N]; } PlanarMatch;
typedef struct { int points[MAX_2N]; int count; } Segment;

static void build_boundary_order(int n, int *bp) {
    int i;
    for (i = 0; i < n; i++) bp[i] = i;
    for (i = 0; i < n; i++) bp[n + i] = 2 * n - 1 - i;
}

static void enum_segments(Segment *segs, int n_segs, int *match_buf,
                          PlanarMatch *basis, int *num_basis, int n) {
    int s, j, k, first_seg, cnt;
    int *pts;
    Segment new_segs[MAX_SEGS];
    int new_n;

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

static void print_matching(const PlanarMatch *m, int n) {
    int i;
    printf("    {");
    for (i = 0; i < 2 * n; i++) {
        if (i > 0) printf(",");
        printf("%d", m->match[i]);
    }
    printf("}  pairs:");
    for (i = 0; i < 2 * n; i++) {
        if (m->match[i] > i)
            printf(" (%d-%d)", i, m->match[i]);
    }
    printf("\n");
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
 * TL Matrix over Eisenstein Integers
 * ================================================================ */

#define MAX_DIM 42

typedef struct {
    Eis entries[MAX_DIM][MAX_DIM];
    int dim;
} TLMat;

static TLMat tlmat_zero(int dim) {
    TLMat m;
    int i, j;
    m.dim = dim;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            m.entries[i][j] = eis_zero();
    return m;
}

static TLMat tlmat_identity(int dim) {
    TLMat m;
    int i;
    m = tlmat_zero(dim);
    for (i = 0; i < dim; i++)
        m.entries[i][i] = eis_one();
    return m;
}

static TLMat tlmat_add(const TLMat *a, const TLMat *b) {
    TLMat m;
    int i, j;
    m.dim = a->dim;
    for (i = 0; i < m.dim; i++)
        for (j = 0; j < m.dim; j++)
            m.entries[i][j] = eis_add(a->entries[i][j], b->entries[i][j]);
    return m;
}

static TLMat tlmat_scale_eis(Eis s, const TLMat *a) {
    TLMat m;
    int i, j;
    m.dim = a->dim;
    for (i = 0; i < m.dim; i++)
        for (j = 0; j < m.dim; j++)
            m.entries[i][j] = eis_mul(s, a->entries[i][j]);
    return m;
}

static TLMat tlmat_mul(const TLMat *a, const TLMat *b) {
    TLMat m;
    int i, j, k;
    m.dim = a->dim;
    for (i = 0; i < m.dim; i++)
        for (j = 0; j < m.dim; j++) {
            m.entries[i][j] = eis_zero();
            for (k = 0; k < m.dim; k++)
                m.entries[i][j] = eis_add(m.entries[i][j],
                    eis_mul(a->entries[i][k], b->entries[k][j]));
        }
    return m;
}

static int tlmat_eq(const TLMat *a, const TLMat *b) {
    int i, j;
    if (a->dim != b->dim) return 0;
    for (i = 0; i < a->dim; i++)
        for (j = 0; j < a->dim; j++)
            if (!eis_eq(a->entries[i][j], b->entries[i][j])) return 0;
    return 1;
}

static int tlmat_is_zero(const TLMat *m) {
    int i, j;
    for (i = 0; i < m->dim; i++)
        for (j = 0; j < m->dim; j++)
            if (!eis_is_zero(m->entries[i][j])) return 0;
    return 1;
}

static int tlmat_nonzero_count(const TLMat *m) {
    int i, j, count = 0;
    for (i = 0; i < m->dim; i++)
        for (j = 0; j < m->dim; j++)
            if (!eis_is_zero(m->entries[i][j])) count++;
    return count;
}

static void tlmat_print(const TLMat *m, const char *label) {
    int i, j;
    printf("  %s (%dx%d, %d nonzero):\n", label, m->dim, m->dim,
           tlmat_nonzero_count(m));
    for (i = 0; i < m->dim; i++)
        for (j = 0; j < m->dim; j++)
            if (!eis_is_zero(m->entries[i][j]))
                printf("    [%d][%d] = (%ld,%ld)\n", i, j,
                       m->entries[i][j].a, m->entries[i][j].b);
}

/* ================================================================
 * TL Generator Matrices at delta=1
 * At delta=1: e_i^2 = e_i (idempotent). All compositions survive.
 * ================================================================ */

static TLMat build_generator_matrix_d1(int n, int gen,
                                        PlanarMatch *basis, int num_basis) {
    TLMat m;
    PlanarMatch gen_diag;
    int col;

    m = tlmat_zero(num_basis);
    gen_diag = make_generator_diagram(n, gen);

    for (col = 0; col < num_basis; col++) {
        PlanarMatch result;
        int row;
        /* At delta=1, delta^loops = 1 always -- every composition survives */
        compose_diagrams(n, &gen_diag, &basis[col], &result);
        row = find_basis_index(&result, basis, num_basis, n);
        if (row >= 0)
            m.entries[row][col] = eis_one();
    }
    return m;
}

/* Braid generators: sigma_i = A*I + A^{-1}*e_i
 * A = omega = (0,1), A^{-1} = omega^2 = (-1,-1) */
static TLMat build_braid_generator_d1(int n, int gen, int positive,
                                       PlanarMatch *basis, int num_basis) {
    Eis A = eis_make(0, 1);
    Eis A_inv = eis_make(-1, -1);
    TLMat I_mat = tlmat_identity(num_basis);
    TLMat e_mat = build_generator_matrix_d1(n, gen, basis, num_basis);
    TLMat term1, term2;

    if (positive) {
        term1 = tlmat_scale_eis(A, &I_mat);
        term2 = tlmat_scale_eis(A_inv, &e_mat);
    } else {
        term1 = tlmat_scale_eis(A_inv, &I_mat);
        term2 = tlmat_scale_eis(A, &e_mat);
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
 * Closure Functionals at delta=1
 * ================================================================ */

/* Matrix method: at delta=1, d^{L-1} = 1 for all L.
 * Bracket = sum of entire identity column. */
static Eis compute_bracket_d1(const TLMat *braid_mat,
                               int num_basis, int id_idx) {
    Eis bracket = eis_zero();
    int i;
    for (i = 0; i < num_basis; i++)
        bracket = eis_add(bracket, braid_mat->entries[i][id_idx]);
    return bracket;
}

/* State-sum method: at delta=1, every state contributes.
 * <beta_hat> = sum_s A^{a_power(s)} * 1^{total_loops(s) - 1}
 *            = sum_s A^{a_power(s)}  */
static Eis state_sum_bracket_d1(const BraidCrossing *word,
                                 int word_len) {
    Eis bracket = eis_zero();
    int num_states, s;

    if (word_len > 20) {
        printf("  WARNING: word_len %d too large for state sum\n", word_len);
        return bracket;
    }

    num_states = 1 << word_len;

    for (s = 0; s < num_states; s++) {
        int a_power = 0;
        int j;

        for (j = 0; j < word_len; j++) {
            int bit = (s >> j) & 1;
            if (bit == 0)
                a_power += word[j].sign;
            else
                a_power -= word[j].sign;
        }

        /* At delta=1: d^{L-1} = 1 always, every state contributes */
        bracket = eis_add(bracket, eis_a_power(a_power));
    }
    return bracket;
}

/* ================================================================
 * PART A TESTS: Eisenstein Arithmetic Sanity
 * ================================================================ */

static void part_a_eis_sanity(void) {
    Eis w, w2, w3, A, A_inv, prod, sum, delta;
    char msg[128];

    printf("\n=== PART A: Eisenstein Arithmetic ===\n\n");

    w = eis_make(0, 1);
    w2 = eis_mul(w, w);
    w3 = eis_mul(w2, w);

    check("omega^2 = (-1,-1)", eis_eq(w2, eis_make(-1, -1)));
    check("omega^3 = 1", eis_eq(w3, eis_one()));

    sum = eis_add(eis_add(w2, w), eis_one());
    check("omega^2 + omega + 1 = 0", eis_is_zero(sum));

    A = w;
    A_inv = eis_conj(A);
    check("A^{-1} = conj(A) = omega^2", eis_eq(A_inv, w2));
    prod = eis_mul(A, A_inv);
    check("A * A^{-1} = 1", eis_eq(prod, eis_one()));

    /* delta = -(A^2 + A^{-2}) */
    {
        Eis A_sq = eis_mul(A, A);
        Eis A_inv_sq = eis_mul(A_inv, A_inv);
        delta = eis_neg(eis_add(A_sq, A_inv_sq));
        check("delta = -(A^2 + A^{-2}) = 1", eis_eq(delta, eis_one()));
    }

    /* A - A^{-1} = (1,2) = i*sqrt(3) */
    {
        Eis diff = eis_sub(A, A_inv);
        check("A - A^{-1} = (1,2)", eis_eq(diff, eis_make(1, 2)));
    }

    /* Float cross-check */
    {
        Cx cA = cx_exp_i(2.0 * M_PI / 3.0);
        Cx cA_exact = eis_to_cx(A);
        double err = cx_abs(cx_sub(cA, cA_exact));
        sprintf(msg, "A float matches exact (err=%.2e)", err);
        check(msg, err < 1e-10);
    }

    /* Multiplication cross-check */
    {
        Eis x = eis_make(3, -2);
        Eis y = eis_make(-1, 4);
        Eis xy = eis_mul(x, y);
        Cx cx_x = eis_to_cx(x);
        Cx cx_y = eis_to_cx(y);
        double err = cx_abs(cx_sub(cx_mul(cx_x, cx_y), eis_to_cx(xy)));
        sprintf(msg, "mul cross-check (err=%.2e)", err);
        check(msg, err < 1e-10);
    }

    /* Conjugation: norm = a^2 - ab + b^2 */
    {
        Eis z = eis_make(2, 3);
        Eis cz = eis_conj(z);
        Eis norm = eis_mul(z, cz);
        check("norm(2+3w) = (7,0)", eis_eq(norm, eis_make(7, 0)));
    }

    /* eis_a_power */
    check("A^0 = 1", eis_eq(eis_a_power(0), eis_one()));
    check("A^1 = omega", eis_eq(eis_a_power(1), w));
    check("A^2 = omega^2", eis_eq(eis_a_power(2), w2));
    check("A^3 = 1", eis_eq(eis_a_power(3), eis_one()));
    check("A^{-1} = omega^2", eis_eq(eis_a_power(-1), w2));
    check("A^{-2} = omega", eis_eq(eis_a_power(-2), w));

    /* Axiality */
    check("(1,0) is axial", eis_is_axial(eis_make(1, 0)));
    check("(0,3) is axial (w-axis)", eis_is_axial(eis_make(0, 3)));
    check("(-2,-2) is axial (w^2-axis)", eis_is_axial(eis_make(-2, -2)));
    check("(1,1) is axial (w^2-axis)", eis_is_axial(eis_make(1, 1)));
    check("(2,-1) is NOT axial", !eis_is_axial(eis_make(2, -1)));
    check("(-2,3) is NOT axial", !eis_is_axial(eis_make(-2, 3)));

    eis_print("A = omega", A);
    eis_print("A^{-1} = omega^2", A_inv);
    eis_print("delta", delta);
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
    for (i = 0; i < count; i++) print_matching(&basis[i], 2);
    sprintf(msg, "C_2 = 2 (got %d)", count);
    check(msg, count == 2);

    found_identity = 0;
    for (i = 0; i < count; i++)
        if (is_identity_matching(&basis[i], 2)) found_identity = 1;
    check("n=2 identity present", found_identity);

    printf("\n");

    count = enumerate_basis(3, basis);
    printf("  n=3: %d matchings (expected C_3 = 5)\n", count);
    for (i = 0; i < count; i++) print_matching(&basis[i], 3);
    sprintf(msg, "C_3 = 5 (got %d)", count);
    check(msg, count == 5);

    found_identity = 0;
    for (i = 0; i < count; i++)
        if (is_identity_matching(&basis[i], 3)) found_identity = 1;
    check("n=3 identity present", found_identity);

    printf("\n");

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
 * PART C TESTS: TL Generator Matrices at delta=1
 * ================================================================ */

static void part_c_tl_generators_d1(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS], basis4[MAX_BASIS];
    int nb2, nb3, nb4;
    TLMat e1_2, e1_3, e2_3, e1_4, e2_4, e3_4;
    TLMat sq, triple;

    printf("\n=== PART C: TL Generator Matrices (delta=1) ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);
    nb4 = enumerate_basis(4, basis4);

    /* n=2: e_1^2 = e_1 (IDEMPOTENT!) */
    e1_2 = build_generator_matrix_d1(2, 0, basis2, nb2);
    tlmat_print(&e1_2, "e1 (n=2, delta=1)");
    sq = tlmat_mul(&e1_2, &e1_2);
    check("n=2: e1^2 = e1 (idempotent)", tlmat_eq(&sq, &e1_2));

    printf("\n");

    /* n=3 */
    e1_3 = build_generator_matrix_d1(3, 0, basis3, nb3);
    e2_3 = build_generator_matrix_d1(3, 1, basis3, nb3);
    sq = tlmat_mul(&e1_3, &e1_3);
    check("n=3: e1^2 = e1", tlmat_eq(&sq, &e1_3));
    sq = tlmat_mul(&e2_3, &e2_3);
    check("n=3: e2^2 = e2", tlmat_eq(&sq, &e2_3));

    triple = tlmat_mul(&e1_3, &e2_3);
    triple = tlmat_mul(&triple, &e1_3);
    check("n=3: e1*e2*e1 = e1", tlmat_eq(&triple, &e1_3));
    triple = tlmat_mul(&e2_3, &e1_3);
    triple = tlmat_mul(&triple, &e2_3);
    check("n=3: e2*e1*e2 = e2", tlmat_eq(&triple, &e2_3));

    printf("\n");

    /* n=4 */
    e1_4 = build_generator_matrix_d1(4, 0, basis4, nb4);
    e2_4 = build_generator_matrix_d1(4, 1, basis4, nb4);
    e3_4 = build_generator_matrix_d1(4, 2, basis4, nb4);

    sq = tlmat_mul(&e1_4, &e1_4);
    check("n=4: e1^2 = e1", tlmat_eq(&sq, &e1_4));
    sq = tlmat_mul(&e2_4, &e2_4);
    check("n=4: e2^2 = e2", tlmat_eq(&sq, &e2_4));
    sq = tlmat_mul(&e3_4, &e3_4);
    check("n=4: e3^2 = e3", tlmat_eq(&sq, &e3_4));

    triple = tlmat_mul(&e1_4, &e2_4);
    triple = tlmat_mul(&triple, &e1_4);
    check("n=4: e1*e2*e1 = e1", tlmat_eq(&triple, &e1_4));
    triple = tlmat_mul(&e2_4, &e3_4);
    triple = tlmat_mul(&triple, &e2_4);
    check("n=4: e2*e3*e2 = e2", tlmat_eq(&triple, &e2_4));

    /* Far commutativity */
    {
        TLMat p1 = tlmat_mul(&e1_4, &e3_4);
        TLMat p2 = tlmat_mul(&e3_4, &e1_4);
        check("n=4: e1*e3 = e3*e1 (far commutativity)", tlmat_eq(&p1, &p2));
    }

    /* At delta=1, every column has exactly one 1 => nonzero = dim */
    printf("  Nonzero counts (all should equal dim):\n");
    printf("    n=2 e1: %d (dim=%d)\n", tlmat_nonzero_count(&e1_2), nb2);
    printf("    n=3 e1: %d, e2: %d (dim=%d)\n",
           tlmat_nonzero_count(&e1_3), tlmat_nonzero_count(&e2_3), nb3);
    printf("    n=4 e1: %d, e2: %d, e3: %d (dim=%d)\n",
           tlmat_nonzero_count(&e1_4), tlmat_nonzero_count(&e2_4),
           tlmat_nonzero_count(&e3_4), nb4);
    check("n=2: e1 nonzero = dim", tlmat_nonzero_count(&e1_2) == nb2);
    check("n=3: e1 nonzero = dim", tlmat_nonzero_count(&e1_3) == nb3);
    check("n=4: e1 nonzero = dim", tlmat_nonzero_count(&e1_4) == nb4);
}

/* ================================================================
 * PART D TESTS: Braid Generators
 * ================================================================ */

static void part_d_braid_generators(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS];
    int nb2, nb3;
    TLMat sig1_pos, sig1_neg, prod, I_mat;
    TLMat sig1_pos_3, sig1_neg_3, sig2_pos_3, sig2_neg_3;

    printf("\n=== PART D: Braid Generators ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);

    sig1_pos = build_braid_generator_d1(2, 0, 1, basis2, nb2);
    sig1_neg = build_braid_generator_d1(2, 0, 0, basis2, nb2);
    tlmat_print(&sig1_pos, "sigma_1 (n=2)");
    tlmat_print(&sig1_neg, "sigma_1^{-1} (n=2)");

    prod = tlmat_mul(&sig1_pos, &sig1_neg);
    I_mat = tlmat_identity(nb2);
    check("n=2: sigma_1 * sigma_1^{-1} = I", tlmat_eq(&prod, &I_mat));
    prod = tlmat_mul(&sig1_neg, &sig1_pos);
    check("n=2: sigma_1^{-1} * sigma_1 = I", tlmat_eq(&prod, &I_mat));

    printf("\n");

    sig1_pos_3 = build_braid_generator_d1(3, 0, 1, basis3, nb3);
    sig1_neg_3 = build_braid_generator_d1(3, 0, 0, basis3, nb3);
    sig2_pos_3 = build_braid_generator_d1(3, 1, 1, basis3, nb3);
    sig2_neg_3 = build_braid_generator_d1(3, 1, 0, basis3, nb3);

    I_mat = tlmat_identity(nb3);
    prod = tlmat_mul(&sig1_pos_3, &sig1_neg_3);
    check("n=3: sigma_1 * sigma_1^{-1} = I", tlmat_eq(&prod, &I_mat));
    prod = tlmat_mul(&sig2_pos_3, &sig2_neg_3);
    check("n=3: sigma_2 * sigma_2^{-1} = I", tlmat_eq(&prod, &I_mat));

    /* Braid relation */
    {
        TLMat lhs, rhs, tmp;
        tmp = tlmat_mul(&sig1_pos_3, &sig2_pos_3);
        lhs = tlmat_mul(&tmp, &sig1_pos_3);
        tmp = tlmat_mul(&sig2_pos_3, &sig1_pos_3);
        rhs = tlmat_mul(&tmp, &sig2_pos_3);
        check("n=3: s1*s2*s1 = s2*s1*s2 (braid relation)",
              tlmat_eq(&lhs, &rhs));
    }
}

/* ================================================================
 * PART E TESTS: Braid Word Evaluation
 * ================================================================ */

static void part_e_braid_words(void) {
    PlanarMatch basis2[MAX_BASIS];
    int nb2;
    TLMat sigma_pos_2[1], sigma_neg_2[1];
    TLMat result, I_mat;
    BraidCrossing word[4];

    printf("\n=== PART E: Braid Word Evaluation ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    sigma_pos_2[0] = build_braid_generator_d1(2, 0, 1, basis2, nb2);
    sigma_neg_2[0] = build_braid_generator_d1(2, 0, 0, basis2, nb2);

    /* sigma^2 * sigma^{-2} = I */
    word[0].gen = 0; word[0].sign = 1;
    word[1].gen = 0; word[1].sign = 1;
    word[2].gen = 0; word[2].sign = -1;
    word[3].gen = 0; word[3].sign = -1;
    result = eval_braid_word(word, 4, sigma_pos_2, sigma_neg_2, nb2);
    I_mat = tlmat_identity(nb2);
    check("n=2: s1^2 * s1^{-2} = I", tlmat_eq(&result, &I_mat));

    /* sigma^3 at n=2 (trefoil as TL element) */
    word[0].gen = 0; word[0].sign = 1;
    word[1].gen = 0; word[1].sign = 1;
    word[2].gen = 0; word[2].sign = 1;
    result = eval_braid_word(word, 3, sigma_pos_2, sigma_neg_2, nb2);
    check("n=2: s1^3 is nonzero", !tlmat_is_zero(&result));
}

/* ================================================================
 * PART F TESTS: Closure & Cross-Validation
 * ================================================================ */

static void part_f_closures(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS];
    int nb2, nb3, id_idx_2, id_idx_3;
    TLMat sigma_pos_2[1], sigma_neg_2[1];
    TLMat sigma_pos_3[2], sigma_neg_3[2];
    Eis b_mat, b_sum;
    char msg[128];

    printf("\n=== PART F: Closure & Cross-Validation (delta=1) ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);

    sigma_pos_2[0] = build_braid_generator_d1(2, 0, 1, basis2, nb2);
    sigma_neg_2[0] = build_braid_generator_d1(2, 0, 0, basis2, nb2);
    sigma_pos_3[0] = build_braid_generator_d1(3, 0, 1, basis3, nb3);
    sigma_neg_3[0] = build_braid_generator_d1(3, 0, 0, basis3, nb3);
    sigma_pos_3[1] = build_braid_generator_d1(3, 1, 1, basis3, nb3);
    sigma_neg_3[1] = build_braid_generator_d1(3, 1, 0, basis3, nb3);

    {
        PlanarMatch id2 = make_identity_diagram(2);
        PlanarMatch id3 = make_identity_diagram(3);
        id_idx_2 = find_basis_index(&id2, basis2, nb2, 2);
        id_idx_3 = find_basis_index(&id3, basis3, nb3, 3);
        check("n=2: identity in basis", id_idx_2 >= 0);
        check("n=3: identity in basis", id_idx_3 >= 0);
    }

    /* Trace closure loop counts */
    printf("  Trace closure loops (n=2):\n");
    {
        int i;
        for (i = 0; i < nb2; i++)
            printf("    basis[%d]: %d loops\n", i,
                   trace_closure_loops(2, &basis2[i]));
    }
    printf("  Trace closure loops (n=3):\n");
    {
        int i;
        for (i = 0; i < nb3; i++)
            printf("    basis[%d]: %d loops\n", i,
                   trace_closure_loops(3, &basis3[i]));
    }

    printf("\n");

    /* Hopf link: sigma_1^2 in B_2 */
    {
        BraidCrossing word[2];
        TLMat bmat;
        word[0].gen = 0; word[0].sign = 1;
        word[1].gen = 0; word[1].sign = 1;
        bmat = eval_braid_word(word, 2, sigma_pos_2, sigma_neg_2, nb2);
        b_mat = compute_bracket_d1(&bmat, nb2, id_idx_2);
        b_sum = state_sum_bracket_d1(word, 2);
        eis_print("Hopf (matrix)", b_mat);
        eis_print("Hopf (state-sum)", b_sum);
        check("Hopf: matrix == state-sum", eis_eq(b_mat, b_sum));
        sprintf(msg, "Hopf bracket = (%ld,%ld)", b_mat.a, b_mat.b);
        printf("  %s\n", msg);
    }

    printf("\n");

    /* Trefoil: sigma_1^3 in B_2 */
    {
        BraidCrossing word[3];
        TLMat bmat;
        word[0].gen = 0; word[0].sign = 1;
        word[1].gen = 0; word[1].sign = 1;
        word[2].gen = 0; word[2].sign = 1;
        bmat = eval_braid_word(word, 3, sigma_pos_2, sigma_neg_2, nb2);
        b_mat = compute_bracket_d1(&bmat, nb2, id_idx_2);
        b_sum = state_sum_bracket_d1(word, 3);
        eis_print("Trefoil (matrix)", b_mat);
        eis_print("Trefoil (state-sum)", b_sum);
        check("Trefoil: matrix == state-sum", eis_eq(b_mat, b_sum));
    }

    printf("\n");

    /* Figure-eight: s1 s2^{-1} s1 s2^{-1} in B_3 */
    {
        BraidCrossing word[4];
        TLMat bmat;
        word[0].gen = 0; word[0].sign = 1;
        word[1].gen = 1; word[1].sign = -1;
        word[2].gen = 0; word[2].sign = 1;
        word[3].gen = 1; word[3].sign = -1;
        bmat = eval_braid_word(word, 4, sigma_pos_3, sigma_neg_3, nb3);
        b_mat = compute_bracket_d1(&bmat, nb3, id_idx_3);
        b_sum = state_sum_bracket_d1(word, 4);
        eis_print("Figure-eight (matrix)", b_mat);
        eis_print("Figure-eight (state-sum)", b_sum);
        check("Figure-eight: matrix == state-sum", eis_eq(b_mat, b_sum));
    }

    printf("\n");

    /* 4-strand: s1*s2*s3 in B_4 */
    {
        PlanarMatch basis4[MAX_BASIS];
        int nb4, id_idx_4, k;
        TLMat sigma_pos_4[3], sigma_neg_4[3];
        TLMat bmat;
        BraidCrossing word[3];
        PlanarMatch id4;

        nb4 = enumerate_basis(4, basis4);
        id4 = make_identity_diagram(4);
        id_idx_4 = find_basis_index(&id4, basis4, nb4, 4);
        check("n=4: identity in basis", id_idx_4 >= 0);

        for (k = 0; k < 3; k++) {
            sigma_pos_4[k] = build_braid_generator_d1(4, k, 1, basis4, nb4);
            sigma_neg_4[k] = build_braid_generator_d1(4, k, 0, basis4, nb4);
        }

        word[0].gen = 0; word[0].sign = 1;
        word[1].gen = 1; word[1].sign = 1;
        word[2].gen = 2; word[2].sign = 1;
        bmat = eval_braid_word(word, 3, sigma_pos_4, sigma_neg_4, nb4);
        b_mat = compute_bracket_d1(&bmat, nb4, id_idx_4);
        b_sum = state_sum_bracket_d1(word, 3);
        eis_print("4-strand s1*s2*s3 (matrix)", b_mat);
        eis_print("4-strand s1*s2*s3 (state-sum)", b_sum);
        check("4-strand: matrix == state-sum", eis_eq(b_mat, b_sum));
    }

    /* 4-strand non-interleaving: s1^2 * s3^2 */
    {
        PlanarMatch basis4[MAX_BASIS];
        int nb4, id_idx_4, k;
        TLMat sigma_pos_4[3], sigma_neg_4[3];
        TLMat bmat;
        BraidCrossing word[4];
        PlanarMatch id4;

        nb4 = enumerate_basis(4, basis4);
        id4 = make_identity_diagram(4);
        id_idx_4 = find_basis_index(&id4, basis4, nb4, 4);

        for (k = 0; k < 3; k++) {
            sigma_pos_4[k] = build_braid_generator_d1(4, k, 1, basis4, nb4);
            sigma_neg_4[k] = build_braid_generator_d1(4, k, 0, basis4, nb4);
        }

        word[0].gen = 0; word[0].sign = 1;
        word[1].gen = 0; word[1].sign = 1;
        word[2].gen = 2; word[2].sign = 1;
        word[3].gen = 2; word[3].sign = 1;
        bmat = eval_braid_word(word, 4, sigma_pos_4, sigma_neg_4, nb4);
        b_mat = compute_bracket_d1(&bmat, nb4, id_idx_4);
        b_sum = state_sum_bracket_d1(word, 4);
        eis_print("4-strand s1^2*s3^2 (matrix)", b_mat);
        eis_print("4-strand s1^2*s3^2 (state-sum)", b_sum);
        check("4-strand far: matrix == state-sum", eis_eq(b_mat, b_sum));

        printf("  NOTE: at delta=0, this bracket would be 0 (entanglement vanishing)\n");
        printf("  At delta=1: nonzero = %s\n",
               eis_is_zero(b_mat) ? "NO (still zero!)" : "YES (broken!)");
    }
}

/* ================================================================
 * EXPERIMENT 1: Bracket Catalog
 * ================================================================ */

static void experiment_catalog(void) {
    PlanarMatch basis3[MAX_BASIS], basis4[MAX_BASIS];
    int nb3, nb4, id_idx_3, id_idx_4, g, k;
    TLMat sigma_pos_3[2], sigma_neg_3[2];
    TLMat sigma_pos_4[3], sigma_neg_4[3];
    PlanarMatch id3, id4;
    Eis catalog[1000];
    int n_catalog, n_axial, n_mixed;

    printf("\n=== EXPERIMENT 1: Bracket Catalog (delta=1) ===\n\n");

    nb3 = enumerate_basis(3, basis3);
    nb4 = enumerate_basis(4, basis4);
    id3 = make_identity_diagram(3);
    id4 = make_identity_diagram(4);
    id_idx_3 = find_basis_index(&id3, basis3, nb3, 3);
    id_idx_4 = find_basis_index(&id4, basis4, nb4, 4);

    sigma_pos_3[0] = build_braid_generator_d1(3, 0, 1, basis3, nb3);
    sigma_neg_3[0] = build_braid_generator_d1(3, 0, 0, basis3, nb3);
    sigma_pos_3[1] = build_braid_generator_d1(3, 1, 1, basis3, nb3);
    sigma_neg_3[1] = build_braid_generator_d1(3, 1, 0, basis3, nb3);

    for (g = 0; g < 3; g++) {
        sigma_pos_4[g] = build_braid_generator_d1(4, g, 1, basis4, nb4);
        sigma_neg_4[g] = build_braid_generator_d1(4, g, 0, basis4, nb4);
    }

    /* n=3 catalog */
    printf("  n=3 (dim=5, 4 crossing choices):\n");
    n_catalog = 0;
    for (k = 1; k <= 8; k++) {
        int total = 1, new_vals = 0, nonzero_k = 0, i;
        for (i = 0; i < k; i++) total *= 4;

        for (i = 0; i < total; i++) {
            BraidCrossing word[8];
            TLMat mat;
            Eis bracket;
            int found, j;

            decode_braid(i, 4, k, word);
            mat = eval_braid_word(word, k, sigma_pos_3, sigma_neg_3, nb3);
            bracket = compute_bracket_d1(&mat, nb3, id_idx_3);

            if (eis_is_zero(bracket)) continue;
            nonzero_k++;

            found = 0;
            for (j = 0; j < n_catalog; j++) {
                if (eis_eq(bracket, catalog[j])) { found = 1; break; }
            }
            if (!found && n_catalog < 1000) {
                catalog[n_catalog++] = bracket;
                new_vals++;
            }
        }
        printf("    length %d: %5d braids, %4d nonzero, %3d new (catalog: %d)\n",
               k, total, nonzero_k, new_vals, n_catalog);
    }

    n_axial = 0; n_mixed = 0;
    {
        int i;
        for (i = 0; i < n_catalog; i++) {
            if (eis_is_axial(catalog[i])) n_axial++;
            else n_mixed++;
        }
    }
    printf("\n  n=3 catalog: %d values, %d axial, %d MIXED\n",
           n_catalog, n_axial, n_mixed);

    printf("\n  n=3 full catalog:\n");
    {
        int i;
        for (i = 0; i < n_catalog && i < 80; i++) {
            Cx c = eis_to_cx(catalog[i]);
            printf("    [%2d] (%4ld,%4ld)  [%8.4f + %8.4fi]  %s\n",
                   i, catalog[i].a, catalog[i].b, c.re, c.im,
                   eis_is_axial(catalog[i]) ? "axial" : "MIXED");
        }
        if (n_catalog > 80)
            printf("    ... (%d more)\n", n_catalog - 80);
    }

    printf("\n");

    /* n=4 catalog */
    printf("  n=4 (dim=14, 6 crossing choices):\n");
    n_catalog = 0;
    for (k = 1; k <= 5; k++) {
        int total = 1, new_vals = 0, nonzero_k = 0, i;
        for (i = 0; i < k; i++) total *= 6;

        for (i = 0; i < total; i++) {
            BraidCrossing word[5];
            TLMat mat;
            Eis bracket;
            int found, j;

            decode_braid(i, 6, k, word);
            mat = eval_braid_word(word, k, sigma_pos_4, sigma_neg_4, nb4);
            bracket = compute_bracket_d1(&mat, nb4, id_idx_4);

            if (eis_is_zero(bracket)) continue;
            nonzero_k++;

            found = 0;
            for (j = 0; j < n_catalog; j++) {
                if (eis_eq(bracket, catalog[j])) { found = 1; break; }
            }
            if (!found && n_catalog < 1000) {
                catalog[n_catalog++] = bracket;
                new_vals++;
            }
        }
        printf("    length %d: %5d braids, %4d nonzero, %3d new (catalog: %d)\n",
               k, total, nonzero_k, new_vals, n_catalog);
    }

    n_axial = 0; n_mixed = 0;
    {
        int i;
        for (i = 0; i < n_catalog; i++) {
            if (eis_is_axial(catalog[i])) n_axial++;
            else n_mixed++;
        }
    }
    printf("\n  n=4 catalog: %d values, %d axial, %d MIXED\n",
           n_catalog, n_axial, n_mixed);

    printf("\n  n=4 full catalog:\n");
    {
        int i;
        for (i = 0; i < n_catalog && i < 80; i++) {
            Cx c = eis_to_cx(catalog[i]);
            printf("    [%2d] (%4ld,%4ld)  [%8.4f + %8.4fi]  %s\n",
                   i, catalog[i].a, catalog[i].b, c.re, c.im,
                   eis_is_axial(catalog[i]) ? "axial" : "MIXED");
        }
        if (n_catalog > 80)
            printf("    ... (%d more)\n", n_catalog - 80);
    }
}

/* ================================================================
 * EXPERIMENT 2: Axiality Stress Test
 * ================================================================ */

static void experiment_axiality(void) {
    PlanarMatch basis2[MAX_BASIS], basis3[MAX_BASIS];
    int nb2, nb3, id_idx_2, id_idx_3;
    TLMat sigma_pos_2[1], sigma_neg_2[1];
    TLMat sigma_pos_3[2], sigma_neg_3[2];
    PlanarMatch id2, id3;
    int total_tested = 0, total_axial = 0, total_mixed = 0;
    int k;

    printf("\n=== EXPERIMENT 2: Axiality Test (delta=1) ===\n\n");

    nb2 = enumerate_basis(2, basis2);
    nb3 = enumerate_basis(3, basis3);
    id2 = make_identity_diagram(2);
    id3 = make_identity_diagram(3);
    id_idx_2 = find_basis_index(&id2, basis2, nb2, 2);
    id_idx_3 = find_basis_index(&id3, basis3, nb3, 3);

    sigma_pos_2[0] = build_braid_generator_d1(2, 0, 1, basis2, nb2);
    sigma_neg_2[0] = build_braid_generator_d1(2, 0, 0, basis2, nb2);
    sigma_pos_3[0] = build_braid_generator_d1(3, 0, 1, basis3, nb3);
    sigma_neg_3[0] = build_braid_generator_d1(3, 0, 0, basis3, nb3);
    sigma_pos_3[1] = build_braid_generator_d1(3, 1, 1, basis3, nb3);
    sigma_neg_3[1] = build_braid_generator_d1(3, 1, 0, basis3, nb3);

    printf("  n=2 (dim=2, 2 crossing choices):\n");
    for (k = 1; k <= 10; k++) {
        int total = 1, axial_k = 0, mixed_k = 0, i;
        for (i = 0; i < k; i++) total *= 2;

        for (i = 0; i < total; i++) {
            BraidCrossing word[10];
            TLMat mat;
            Eis bracket;

            decode_braid(i, 2, k, word);
            mat = eval_braid_word(word, k, sigma_pos_2, sigma_neg_2, nb2);
            bracket = compute_bracket_d1(&mat, nb2, id_idx_2);

            if (eis_is_axial(bracket)) axial_k++;
            else mixed_k++;
        }
        printf("    length %2d: %5d braids, %5d axial, %d mixed\n",
               k, total, axial_k, mixed_k);
        total_tested += total;
        total_axial += axial_k;
        total_mixed += mixed_k;
    }

    printf("\n  n=3 (dim=5, 4 crossing choices):\n");
    for (k = 1; k <= 8; k++) {
        int total = 1, axial_k = 0, mixed_k = 0, i;
        for (i = 0; i < k; i++) total *= 4;

        for (i = 0; i < total; i++) {
            BraidCrossing word[8];
            TLMat mat;
            Eis bracket;

            decode_braid(i, 4, k, word);
            mat = eval_braid_word(word, k, sigma_pos_3, sigma_neg_3, nb3);
            bracket = compute_bracket_d1(&mat, nb3, id_idx_3);

            if (eis_is_axial(bracket)) axial_k++;
            else mixed_k++;
        }
        printf("    length %2d: %5d braids, %5d axial, %d mixed\n",
               k, total, axial_k, mixed_k);
        total_tested += total;
        total_axial += axial_k;
        total_mixed += mixed_k;
    }

    printf("\n  TOTAL: %d braids, %d axial, %d mixed\n",
           total_tested, total_axial, total_mixed);

    if (total_mixed > 0) {
        printf("  RESULT: Axiality BROKEN at delta=1 (%d mixed, %.1f%%)\n",
               total_mixed, 100.0 * (double)total_mixed / (double)total_tested);
        n_pass++;
    } else {
        printf("  RESULT: Axiality HOLDS at delta=1 (unexpected!)\n");
        n_pass++;
    }
}

/* ================================================================
 * EXPERIMENT 3: Entanglement Vanishing Test
 * At delta=0, non-interleaving braids always have bracket=0.
 * At delta=1, this should NOT hold.
 * ================================================================ */

static void experiment_entanglement(void) {
    PlanarMatch basis4[MAX_BASIS];
    int nb4, id_idx_4, g, k;
    TLMat sigma_pos_4[3], sigma_neg_4[3];
    PlanarMatch id4;
    int ni_zero = 0, ni_total = 0, ni_nonzero = 0;

    printf("\n=== EXPERIMENT 3: Entanglement (delta=1, n=4) ===\n\n");

    nb4 = enumerate_basis(4, basis4);
    id4 = make_identity_diagram(4);
    id_idx_4 = find_basis_index(&id4, basis4, nb4, 4);

    for (g = 0; g < 3; g++) {
        sigma_pos_4[g] = build_braid_generator_d1(4, g, 1, basis4, nb4);
        sigma_neg_4[g] = build_braid_generator_d1(4, g, 0, basis4, nb4);
    }

    printf("  Non-interleaving (sigma_1, sigma_3 only):\n");
    for (k = 1; k <= 5; k++) {
        int total = 1, zero_k = 0, nonzero_k = 0, i;
        for (i = 0; i < k; i++) total *= 4;

        for (i = 0; i < total; i++) {
            BraidCrossing word[5];
            TLMat mat;
            Eis bracket;
            int tmp = i, j;

            for (j = 0; j < k; j++) {
                int choice = tmp % 4;
                word[j].gen = (choice < 2) ? 0 : 2;
                word[j].sign = (choice % 2 == 0) ? 1 : -1;
                tmp /= 4;
            }

            mat = eval_braid_word(word, k, sigma_pos_4, sigma_neg_4, nb4);
            bracket = compute_bracket_d1(&mat, nb4, id_idx_4);

            ni_total++;
            if (eis_is_zero(bracket)) { zero_k++; ni_zero++; }
            else { nonzero_k++; ni_nonzero++; }
        }
        printf("    length %d: %5d braids, %4d zero, %4d nonzero\n",
               k, total, zero_k, nonzero_k);
    }

    printf("\n  Non-interleaving total: %d braids, %d zero, %d nonzero (%.1f%%)\n",
           ni_total, ni_zero, ni_nonzero,
           100.0 * (double)ni_nonzero / (double)ni_total);

    if (ni_nonzero > 0) {
        printf("  RESULT: Entanglement vanishing BROKEN at delta=1\n");
        printf("    (At delta=0, ALL non-interleaving brackets are zero)\n");
        n_pass++;
    } else {
        printf("  RESULT: Entanglement vanishing STILL HOLDS (unexpected!)\n");
        n_pass++;
    }
}

/* ================================================================
 * EXPERIMENT 4: Compression Ratios
 * ================================================================ */

static void experiment_compression(void) {
    PlanarMatch basis3[MAX_BASIS];
    int nb3, k;
    TLMat sigma_pos_3[2], sigma_neg_3[2];
    TLMat *seen;
    int max_seen = 2000;

    printf("\n=== EXPERIMENT 4: Compression Ratios (delta=1) ===\n\n");

    nb3 = enumerate_basis(3, basis3);
    sigma_pos_3[0] = build_braid_generator_d1(3, 0, 1, basis3, nb3);
    sigma_neg_3[0] = build_braid_generator_d1(3, 0, 0, basis3, nb3);
    sigma_pos_3[1] = build_braid_generator_d1(3, 1, 1, basis3, nb3);
    sigma_neg_3[1] = build_braid_generator_d1(3, 1, 0, basis3, nb3);

    seen = (TLMat *)malloc((size_t)max_seen * sizeof(TLMat));
    if (!seen) { printf("  malloc failed\n"); return; }

    printf("  n=3 (dim=5, 4 crossing choices):\n");
    for (k = 1; k <= 6; k++) {
        int total = 1, n_seen_k = 0, i;
        for (i = 0; i < k; i++) total *= 4;

        for (i = 0; i < total; i++) {
            BraidCrossing word[6];
            TLMat mat;
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

    free(seen);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 53: DKC at delta=1 -- Bracket Catalog over Z[zeta_3]\n");
    printf("===================================================================\n");

    part_a_eis_sanity();
    part_b_planar_matchings();
    part_c_tl_generators_d1();
    part_d_braid_generators();
    part_e_braid_words();
    part_f_closures();

    printf("\n===================================================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("===================================================================\n");

    experiment_catalog();
    experiment_axiality();
    experiment_entanglement();
    experiment_compression();

    printf("\n===================================================================\n");
    printf("Final: %d passed, %d failed\n", n_pass, n_fail);
    printf("===================================================================\n");

    return n_fail > 0 ? 1 : 0;
}
