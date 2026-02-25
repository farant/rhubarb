/*
 * KNOTAPEL DEMO 98: 3-Strand DKC Proof of Concept
 * =================================================
 *
 * Core question: does going from 2-strand to 3-strand braids increase
 * DKC computational capacity?
 *
 * Under Fran's dimensional formula (dim = strands*2 + writhe),
 * 3-strand = dimension 6 vs 2-strand = dimension 4.
 *
 * Phase 1: Build 3-strand BFS catalog (5x5 TL matrices over Z[zeta_8])
 * Phase 2: Trace readout activation function
 * Phase 3: 3-strand XOR capacity
 * Phase 4: Matched comparison (3-strand vs 2-strand at same catalog size)
 * Phase 5: Interleaving analysis (coupled vs uncoupled strands)
 *
 * Reuses Demo 35 TL infrastructure: Cyc8, TLMatrix, diagram composition,
 * braid generators sigma_i = A*I + A^{-1}*e_i.
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

/* Convert to complex double */
static void cyc8_to_double(Cyc8 z, double *re, double *im) {
    double inv_sqrt2 = 1.0 / sqrt(2.0);
    *re = (double)z.a + (double)z.b * inv_sqrt2 + (double)z.d * (-inv_sqrt2);
    *im = (double)z.b * inv_sqrt2 + (double)z.c + (double)z.d * inv_sqrt2;
}

/* ================================================================
 * TL Matrix: 5x5 over Cyc8 (for TL_3, dim = C_3 = 5)
 * ================================================================ */

#define TL_DIM 5

typedef struct {
    Cyc8 e[TL_DIM][TL_DIM];
} TLMat;

static TLMat tlm_zero(void) {
    TLMat m;
    int i, j;
    for (i = 0; i < TL_DIM; i++)
        for (j = 0; j < TL_DIM; j++)
            m.e[i][j] = cyc8_zero();
    return m;
}

static TLMat tlm_identity(void) {
    TLMat m;
    int i;
    m = tlm_zero();
    for (i = 0; i < TL_DIM; i++)
        m.e[i][i] = cyc8_one();
    return m;
}

static TLMat tlm_add(const TLMat *a, const TLMat *b) {
    TLMat m;
    int i, j;
    for (i = 0; i < TL_DIM; i++)
        for (j = 0; j < TL_DIM; j++)
            m.e[i][j] = cyc8_add(a->e[i][j], b->e[i][j]);
    return m;
}

static TLMat tlm_neg(const TLMat *a) {
    TLMat m;
    int i, j;
    for (i = 0; i < TL_DIM; i++)
        for (j = 0; j < TL_DIM; j++)
            m.e[i][j] = cyc8_neg(a->e[i][j]);
    return m;
}

static TLMat tlm_scale(Cyc8 s, const TLMat *a) {
    TLMat m;
    int i, j;
    for (i = 0; i < TL_DIM; i++)
        for (j = 0; j < TL_DIM; j++)
            m.e[i][j] = cyc8_mul(s, a->e[i][j]);
    return m;
}

static TLMat tlm_mul(const TLMat *a, const TLMat *b) {
    TLMat m;
    int i, j, k;
    for (i = 0; i < TL_DIM; i++)
        for (j = 0; j < TL_DIM; j++) {
            m.e[i][j] = cyc8_zero();
            for (k = 0; k < TL_DIM; k++)
                m.e[i][j] = cyc8_add(m.e[i][j],
                    cyc8_mul(a->e[i][k], b->e[k][j]));
        }
    return m;
}

static int tlm_eq(const TLMat *a, const TLMat *b) {
    int i, j;
    for (i = 0; i < TL_DIM; i++)
        for (j = 0; j < TL_DIM; j++)
            if (!cyc8_eq(a->e[i][j], b->e[i][j])) return 0;
    return 1;
}

static Cyc8 tlm_trace(const TLMat *a) {
    Cyc8 t = cyc8_zero();
    int i;
    for (i = 0; i < TL_DIM; i++)
        t = cyc8_add(t, a->e[i][i]);
    return t;
}

/* ================================================================
 * Planar Matching for n=3 (TL_3 basis)
 * 6 boundary points, C_3 = 5 non-crossing matchings
 * ================================================================ */

#define N_STRANDS 3
#define N_2N 6
#define N_BASIS 5
#define MAX_SEGS 6

typedef struct {
    int match[N_2N];
} PMatch;

typedef struct {
    int points[N_2N];
    int count;
} Segment;

static PMatch g_basis[N_BASIS];
static int g_num_basis = 0;

static void build_boundary_order(int *bp) {
    int i;
    for (i = 0; i < N_STRANDS; i++)
        bp[i] = i;
    for (i = 0; i < N_STRANDS; i++)
        bp[N_STRANDS + i] = N_2N - 1 - i;
}

static void enum_segments(Segment *segs, int n_segs, int *match_buf) {
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
        if (g_num_basis < N_BASIS) {
            memcpy(g_basis[g_num_basis].match, match_buf,
                   (size_t)N_2N * sizeof(int));
            g_num_basis++;
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

        enum_segments(new_segs, new_n, match_buf);
    }
}

static void enumerate_basis(void) {
    Segment segs[1];
    int match_buf[N_2N];

    g_num_basis = 0;
    build_boundary_order(segs[0].points);
    segs[0].count = N_2N;
    memset(match_buf, -1, sizeof(match_buf));

    enum_segments(segs, 1, match_buf);
}

/* ================================================================
 * Diagram composition and generator construction
 * ================================================================ */

static PMatch make_identity_diagram(void) {
    PMatch m;
    int k;
    for (k = 0; k < N_STRANDS; k++) {
        m.match[k] = N_STRANDS + k;
        m.match[N_STRANDS + k] = k;
    }
    return m;
}

static PMatch make_generator_diagram(int gen) {
    PMatch m;
    int k;
    for (k = 0; k < N_STRANDS; k++) {
        m.match[k] = N_STRANDS + k;
        m.match[N_STRANDS + k] = k;
    }
    m.match[gen] = gen + 1;
    m.match[gen + 1] = gen;
    m.match[N_STRANDS + gen] = N_STRANDS + gen + 1;
    m.match[N_STRANDS + gen + 1] = N_STRANDS + gen;
    return m;
}

static int compose_diagrams(const PMatch *d1, const PMatch *d2,
                            PMatch *result) {
    int glue_visited[N_STRANDS];
    int i, loops;

    memset(result->match, -1, (size_t)N_2N * sizeof(int));
    memset(glue_visited, 0, (size_t)N_STRANDS * sizeof(int));
    loops = 0;

    for (i = 0; i < N_2N; i++) {
        int in_d1;
        int cur;

        if (result->match[i] >= 0) continue;

        if (i < N_STRANDS) {
            in_d1 = 1; cur = i;
        } else {
            in_d1 = 0; cur = i;
        }

        for (;;) {
            int partner;
            if (in_d1) {
                partner = d1->match[cur];
                if (partner < N_STRANDS) {
                    result->match[i] = partner;
                    result->match[partner] = i;
                    break;
                }
                glue_visited[partner - N_STRANDS] = 1;
                in_d1 = 0;
                cur = partner - N_STRANDS;
            } else {
                partner = d2->match[cur];
                if (partner >= N_STRANDS) {
                    result->match[i] = partner;
                    result->match[partner] = i;
                    break;
                }
                glue_visited[partner] = 1;
                in_d1 = 1;
                cur = N_STRANDS + partner;
            }
        }
    }

    for (i = 0; i < N_STRANDS; i++) {
        int cur, p, q;
        if (glue_visited[i]) continue;
        loops++;
        cur = i;
        do {
            glue_visited[cur] = 1;
            p = d2->match[cur];
            glue_visited[p] = 1;
            q = d1->match[N_STRANDS + p];
            cur = q - N_STRANDS;
        } while (cur != i);
    }

    return loops;
}

static int find_basis_index(const PMatch *m) {
    int i, j, eq;
    for (i = 0; i < g_num_basis; i++) {
        eq = 1;
        for (j = 0; j < N_2N; j++) {
            if (m->match[j] != g_basis[i].match[j]) { eq = 0; break; }
        }
        if (eq) return i;
    }
    return -1;
}

static TLMat build_gen_matrix(int gen) {
    TLMat m;
    PMatch gen_diag, result;
    int col, loops, row;

    m = tlm_zero();
    gen_diag = make_generator_diagram(gen);

    for (col = 0; col < N_BASIS; col++) {
        loops = compose_diagrams(&gen_diag, &g_basis[col], &result);
        if (loops > 0) continue;
        row = find_basis_index(&result);
        if (row >= 0)
            m.e[row][col] = cyc8_one();
    }
    return m;
}

/* ================================================================
 * Braid generators and evaluation
 * A = -zeta_8 = (0,-1,0,0), A_inv = conj(A) = (0,0,0,1)
 * sigma_i     = A*I + A^{-1}*e_i
 * sigma_i^{-1} = A^{-1}*I + A*e_i
 * ================================================================ */

static Cyc8 g_A, g_A_inv;

/* 4 braid generators: sigma1+, sigma1-, sigma2+, sigma2- */
static TLMat g_braid_gen[4];

static void build_braid_generators(void) {
    TLMat I_mat, e1, e2;
    TLMat t1, t2;

    g_A = cyc8_make(0, -1, 0, 0);
    g_A_inv = cyc8_conj(g_A);

    I_mat = tlm_identity();
    e1 = build_gen_matrix(0);
    e2 = build_gen_matrix(1);

    /* sigma1+ = A*I + A_inv*e1 */
    t1 = tlm_scale(g_A, &I_mat);
    t2 = tlm_scale(g_A_inv, &e1);
    g_braid_gen[0] = tlm_add(&t1, &t2);

    /* sigma1- = A_inv*I + A*e1 */
    t1 = tlm_scale(g_A_inv, &I_mat);
    t2 = tlm_scale(g_A, &e1);
    g_braid_gen[1] = tlm_add(&t1, &t2);

    /* sigma2+ = A*I + A_inv*e2 */
    t1 = tlm_scale(g_A, &I_mat);
    t2 = tlm_scale(g_A_inv, &e2);
    g_braid_gen[2] = tlm_add(&t1, &t2);

    /* sigma2- = A_inv*I + A*e2 */
    t1 = tlm_scale(g_A_inv, &I_mat);
    t2 = tlm_scale(g_A, &e2);
    g_braid_gen[3] = tlm_add(&t1, &t2);
}

/* Trace closure: count loops from matching */
static int trace_closure_loops(const PMatch *m) {
    int visited[N_2N];
    int i, cur, loops = 0;
    memset(visited, 0, (size_t)N_2N * sizeof(int));

    for (i = 0; i < N_2N; i++) {
        if (visited[i]) continue;
        loops++;
        cur = i;
        do {
            visited[cur] = 1;
            cur = m->match[cur];
            visited[cur] = 1;
            cur = (cur < N_STRANDS) ? cur + N_STRANDS : cur - N_STRANDS;
        } while (cur != i);
    }
    return loops;
}

/* Extract bracket from matrix via trace closure at delta=0 */
static Cyc8 compute_bracket(const TLMat *braid_mat, int id_idx) {
    Cyc8 bracket = cyc8_zero();
    int i;
    for (i = 0; i < N_BASIS; i++) {
        if (trace_closure_loops(&g_basis[i]) == 1)
            bracket = cyc8_add(bracket, braid_mat->e[i][id_idx]);
    }
    return bracket;
}

/* ================================================================
 * 3-strand BFS catalog
 * ================================================================ */

#define MAX_3CAT 2048

static TLMat g_3cat[MAX_3CAT];
static Cyc8 g_3bracket[MAX_3CAT];
static int g_3depth[MAX_3CAT];
static int g_3writhe[MAX_3CAT];
static int g_3interleave[MAX_3CAT]; /* bitmask: bit0=uses_sigma1, bit1=uses_sigma2 */
static int g_3cat_size = 0;

/* Generator properties:
 * gen 0: sigma1+  (writhe +1, uses sigma1)
 * gen 1: sigma1-  (writhe -1, uses sigma1)
 * gen 2: sigma2+  (writhe +1, uses sigma2)
 * gen 3: sigma2-  (writhe -1, uses sigma2)
 */
static const int gen_writhe[4] = {1, -1, 1, -1};
static const int gen_sigma[4] = {1, 1, 2, 2}; /* which sigma used */

static int find_in_3cat(const TLMat *m) {
    int i;
    for (i = 0; i < g_3cat_size; i++) {
        if (tlm_eq(&g_3cat[i], m)) return i;
    }
    return -1;
}

static void build_3strand_catalog(void) {
    int prev, gi, i, rd;
    int id_idx;

    g_3cat_size = 0;

    /* Find identity basis index for bracket computation */
    {
        PMatch id_diag = make_identity_diagram();
        id_idx = find_basis_index(&id_diag);
    }

    /* Entry 0: identity matrix */
    g_3cat[0] = tlm_identity();
    g_3bracket[0] = compute_bracket(&g_3cat[0], id_idx);
    g_3depth[0] = 0;
    g_3writhe[0] = 0;
    g_3interleave[0] = 0;
    g_3cat_size = 1;

    printf("  Round 0: %d entries\n", g_3cat_size);

    rd = 1;
    do {
        prev = g_3cat_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                TLMat prod = tlm_mul(&g_3cat[i], &g_braid_gen[gi]);
                if (find_in_3cat(&prod) < 0) {
                    if (g_3cat_size < MAX_3CAT) {
                        g_3cat[g_3cat_size] = prod;
                        g_3bracket[g_3cat_size] = compute_bracket(&prod, id_idx);
                        g_3depth[g_3cat_size] = rd;
                        g_3writhe[g_3cat_size] = g_3writhe[i] + gen_writhe[gi];
                        g_3interleave[g_3cat_size] = g_3interleave[i] | gen_sigma[gi];
                        g_3cat_size++;
                    }
                }
            }
        }
        if (g_3cat_size > prev) {
            printf("  Round %d: %d entries (+%d)\n",
                   rd, g_3cat_size, g_3cat_size - prev);
        }
        rd++;
    } while (g_3cat_size > prev && rd < 20);

    printf("  Total: %d entries in %d rounds\n", g_3cat_size, rd - 1);
}

/* ================================================================
 * 2-strand z8 catalog (quaternionic, for comparison)
 * ================================================================ */

typedef struct { double a, b, c, d; } Quat;

static Quat qmul(const Quat *p, const Quat *g) {
    Quat r;
    r.a = p->a*g->a - p->b*g->b - p->c*g->c - p->d*g->d;
    r.b = p->a*g->b + p->b*g->a + p->c*g->d - p->d*g->c;
    r.c = p->a*g->c - p->b*g->d + p->c*g->a + p->d*g->b;
    r.d = p->a*g->d + p->b*g->c - p->c*g->b + p->d*g->a;
    return r;
}

#define MAX_2CAT 64

static Quat g_2cat[MAX_2CAT];
static int g_2depth[MAX_2CAT];
static int g_2cat_size = 0;

static int find_in_2cat(const Quat *q) {
    int i;
    for (i = 0; i < g_2cat_size; i++) {
        if ((fabs(g_2cat[i].a - q->a) < 1e-10 &&
             fabs(g_2cat[i].b - q->b) < 1e-10 &&
             fabs(g_2cat[i].c - q->c) < 1e-10 &&
             fabs(g_2cat[i].d - q->d) < 1e-10) ||
            (fabs(g_2cat[i].a + q->a) < 1e-10 &&
             fabs(g_2cat[i].b + q->b) < 1e-10 &&
             fabs(g_2cat[i].c + q->c) < 1e-10 &&
             fabs(g_2cat[i].d + q->d) < 1e-10))
            return i;
    }
    return -1;
}

static void build_2strand_catalog(void) {
    double half_angle = M_PI / 4.0;
    double co = cos(half_angle), si = sin(half_angle);
    Quat gens[4];
    int prev, i, gi, rd;

    g_2cat_size = 0;

    /* sigma1 and inverse */
    gens[0].a = co; gens[0].b = si; gens[0].c = 0; gens[0].d = 0;
    gens[1].a = co; gens[1].b = -si; gens[1].c = 0; gens[1].d = 0;
    /* sigma2 and inverse (for 2-strand, only sigma1 matters but keep gen structure) */
    gens[2].a = co; gens[2].b = 0; gens[2].c = 0; gens[2].d = -si;
    gens[3].a = co; gens[3].b = 0; gens[3].c = 0; gens[3].d = si;

    /* Identity */
    g_2cat[0].a = 1; g_2cat[0].b = 0; g_2cat[0].c = 0; g_2cat[0].d = 0;
    g_2depth[0] = 0;
    g_2cat_size = 1;

    rd = 1;
    do {
        prev = g_2cat_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                Quat prod = qmul(&g_2cat[i], &gens[gi]);
                if (find_in_2cat(&prod) < 0 && g_2cat_size < MAX_2CAT) {
                    g_2depth[g_2cat_size] = rd;
                    g_2cat[g_2cat_size++] = prod;
                }
            }
        }
        rd++;
    } while (g_2cat_size > prev && rd < 10);
}

/* ================================================================
 * Phase-cell activation (adapted from D93-97)
 * For 3-strand trace readout: trace is a Cyc8 scalar.
 * Convert to complex, use angle-based activation.
 * ================================================================ */

static int phase_cell_cyc8(Cyc8 val, int k_sec) {
    double re, im, angle;
    int sec;
    if (cyc8_is_zero(val)) return k_sec; /* degenerate cell */
    cyc8_to_double(val, &re, &im);
    angle = atan2(im, re);
    if (angle < 0) angle += 2.0 * M_PI;
    sec = (int)(angle * (double)k_sec / (2.0 * M_PI));
    if (sec >= k_sec) sec = k_sec - 1;
    if (sec < 0) sec = 0;
    return sec;
}

/* For 2-strand: quaternion phase cell (combined Voronoi + sector) */
#define MAX_DIR_2S 32

static double g_2dir[MAX_DIR_2S][3];
static int g_2nd = 0;

static void build_2strand_dirs(void) {
    int i, j;
    g_2nd = 0;
    for (i = 0; i < g_2cat_size; i++) {
        double qa = g_2cat[i].a, qb = g_2cat[i].b;
        double qc = g_2cat[i].c, qd = g_2cat[i].d;
        double nv, ax, ay, az;
        int found = 0;
        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        if (nv < 1e-12) continue;
        ax = qb/nv; ay = qc/nv; az = qd/nv;
        for (j = 0; j < g_2nd; j++) {
            double d1 = fabs(g_2dir[j][0]-ax) + fabs(g_2dir[j][1]-ay) +
                         fabs(g_2dir[j][2]-az);
            double d2 = fabs(g_2dir[j][0]+ax) + fabs(g_2dir[j][1]+ay) +
                         fabs(g_2dir[j][2]+az);
            if (d1 < 1e-8 || d2 < 1e-8) { found = 1; break; }
        }
        if (!found && g_2nd < MAX_DIR_2S) {
            g_2dir[g_2nd][0] = ax; g_2dir[g_2nd][1] = ay; g_2dir[g_2nd][2] = az;
            g_2nd++;
        }
    }
}

static int vor_cell_2s(double ax, double ay, double az) {
    int i, best = 0;
    double bd = -2.0;
    for (i = 0; i < g_2nd; i++) {
        double dp = fabs(ax*g_2dir[i][0] + ay*g_2dir[i][1] + az*g_2dir[i][2]);
        if (dp > bd) { bd = dp; best = i; }
    }
    return best;
}

static int combined_cell_2s(double sa, double sb, double sc, double sd,
                            int k_sec) {
    double n_val = sqrt(sa*sa + sb*sb + sc*sc + sd*sd);
    double qa, nv;
    int sec, vor, n_vor;

    n_vor = g_2nd + 1;
    if (n_val < 1e-12) return (k_sec - 1) * n_vor + g_2nd;

    qa = sa / n_val;
    if (qa < 0) { qa = -qa; sb = -sb; sc = -sc; sd = -sd; }
    if (qa > 1.0) qa = 1.0;

    nv = sqrt(sb*sb + sc*sc + sd*sd) / n_val;
    {
        double half_ang = acos(qa);
        double ang = 2.0 * half_ang * 180.0 / M_PI;
        sec = (int)(ang * (double)k_sec / 360.0);
        if (sec >= k_sec) sec = k_sec - 1;
        if (sec < 0) sec = 0;
    }

    if (nv < 1e-12) {
        vor = g_2nd;
    } else {
        double inv = 1.0 / (nv * n_val);
        vor = vor_cell_2s(sb * inv, sc * inv, sd * inv);
    }

    return sec * n_vor + vor;
}

/* ================================================================
 * XOR test: 3-strand (trace-based)
 * 1wpi encoding: each weight = one catalog entry, sign = +1 or -1
 * Sum = sum of ±M_i (matrix addition). Readout = Tr(sum).
 * Activation = phase_cell on trace.
 * ================================================================ */

#define MAX_ACT 4096

static int cell_even_3[MAX_ACT], cell_odd_3[MAX_ACT];
static int touched_3[MAX_ACT];

static int test_xor_3strand(const int *indices, int n_weights, int k_sec) {
    int n_inputs = 2 * n_weights;
    int n_masks;
    int n_cells;
    int n_touched = 0;
    int mask, i, result = 1;

    n_masks = 1 << n_inputs;
    n_cells = k_sec + 1; /* k_sec angle sectors + 1 degenerate cell */

    if (n_cells > MAX_ACT || n_inputs > 14) return 0;

    for (mask = 0; mask < n_masks; mask++) {
        TLMat sum;
        Cyc8 tr;
        int par = 0, cell;

        sum = tlm_zero();
        for (i = 0; i < n_inputs; i++) {
            int cat_idx = indices[i / 2];
            if (mask & (1 << i)) {
                if (i % 2 == 0) {
                    sum = tlm_add(&sum, &g_3cat[cat_idx]);
                } else {
                    TLMat neg = tlm_neg(&g_3cat[cat_idx]);
                    sum = tlm_add(&sum, &neg);
                }
                par ^= 1;
            }
        }

        tr = tlm_trace(&sum);
        cell = phase_cell_cyc8(tr, k_sec);

        if (cell_even_3[cell] == 0 && cell_odd_3[cell] == 0)
            touched_3[n_touched++] = cell;

        if (par == 0) {
            cell_even_3[cell]++;
            if (cell_odd_3[cell] > 0) { result = 0; goto cleanup; }
        } else {
            cell_odd_3[cell]++;
            if (cell_even_3[cell] > 0) { result = 0; goto cleanup; }
        }
    }

cleanup:
    for (i = 0; i < n_touched; i++) {
        cell_even_3[touched_3[i]] = 0;
        cell_odd_3[touched_3[i]] = 0;
    }
    return result;
}

/* XOR test: 2-strand (quaternionic, standard) */
static int cell_even_2[MAX_ACT], cell_odd_2[MAX_ACT];
static int touched_2[MAX_ACT];

static int test_xor_2strand(const int *indices, int n_weights, int k_sec) {
    int n_inputs = 2 * n_weights;
    int n_masks, n_vor, n_cells;
    double wa[14], wb[14], wc[14], wd[14];
    int n_touched = 0;
    int mask, i, result = 1;

    n_masks = 1 << n_inputs;
    n_vor = g_2nd + 1;
    n_cells = k_sec * n_vor;

    if (n_cells > MAX_ACT || n_inputs > 14) return 0;

    for (i = 0; i < n_weights; i++) {
        const Quat *q = &g_2cat[indices[i]];
        wa[2*i]   =  q->a; wb[2*i]   =  q->b;
        wc[2*i]   =  q->c; wd[2*i]   =  q->d;
        wa[2*i+1] = -q->a; wb[2*i+1] = -q->b;
        wc[2*i+1] = -q->c; wd[2*i+1] = -q->d;
    }

    for (mask = 0; mask < n_masks; mask++) {
        double sa = 0, sb = 0, sc = 0, sd = 0;
        int par = 0, cell;

        for (i = 0; i < n_inputs; i++) {
            if (mask & (1 << i)) {
                sa += wa[i]; sb += wb[i]; sc += wc[i]; sd += wd[i];
                par ^= 1;
            }
        }

        cell = combined_cell_2s(sa, sb, sc, sd, k_sec);

        if (cell_even_2[cell] == 0 && cell_odd_2[cell] == 0)
            touched_2[n_touched++] = cell;

        if (par == 0) {
            cell_even_2[cell]++;
            if (cell_odd_2[cell] > 0) { result = 0; goto cleanup2; }
        } else {
            cell_odd_2[cell]++;
            if (cell_even_2[cell] > 0) { result = 0; goto cleanup2; }
        }
    }

cleanup2:
    for (i = 0; i < n_touched; i++) {
        cell_even_2[touched_2[i]] = 0;
        cell_odd_2[touched_2[i]] = 0;
    }
    return result;
}

/* ================================================================
 * XOR brute-force search
 * ================================================================ */

static int count_xor_bf(int cat_size, int n_weights, int k_sec,
                        int is_3strand, int bf_limit) {
    int ai, aj, ak;
    int count = 0;
    int indices[3];
    int limit;

    if (n_weights != 3) return 0; /* only XOR6 for now */
    limit = cat_size < bf_limit ? cat_size : bf_limit;

    for (ai = 0; ai < limit; ai++)
    for (aj = ai+1; aj < limit; aj++)
    for (ak = aj+1; ak < limit; ak++) {
        indices[0] = ai; indices[1] = aj; indices[2] = ak;
        if (is_3strand) {
            if (test_xor_3strand(indices, 3, k_sec))
                count++;
        } else {
            if (test_xor_2strand(indices, 3, k_sec))
                count++;
        }
    }
    return count;
}

/* Extended search trying multiple k_sec values */
static int search_xor(int cat_size, int n_weights, int is_3strand,
                      int bf_limit, int *best_k) {
    static const int k_vals[] = {4, 6, 8, 10, 12, 16, 20, 24, 32};
    int nk = 9;
    int ki, best = 0;

    *best_k = 0;
    for (ki = 0; ki < nk; ki++) {
        int n_vor, n_cells;
        int c;

        if (is_3strand) {
            n_cells = k_vals[ki] + 1;
        } else {
            n_vor = g_2nd + 1;
            n_cells = k_vals[ki] * n_vor;
        }
        if (n_cells > MAX_ACT) continue;

        c = count_xor_bf(cat_size, n_weights, k_vals[ki], is_3strand, bf_limit);
        if (c > best) {
            best = c;
            *best_k = k_vals[ki];
        }
        if (c > 0) {
            printf("    k=%d: %d XOR6 winners\n", k_vals[ki], c);
        }
    }
    return best;
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    int i;
    char msg[256];

    printf("KNOTAPEL DEMO 98: 3-Strand DKC Proof of Concept\n");
    printf("=================================================\n\n");

    /* ============================================================
     * Setup: build TL_3 basis and braid generators
     * ============================================================ */
    printf("  Building TL_3 basis...\n");
    enumerate_basis();
    printf("  TL_3 basis: %d elements (C_3 = 5)\n", g_num_basis);
    check("TL_3 basis has 5 elements", g_num_basis == 5);

    printf("  Building braid generators...\n");
    build_braid_generators();

    /* Verify: sigma1 * sigma1_inv = I */
    {
        TLMat prod = tlm_mul(&g_braid_gen[0], &g_braid_gen[1]);
        TLMat I_mat = tlm_identity();
        check("sigma1 * sigma1_inv = I", tlm_eq(&prod, &I_mat));
    }
    {
        TLMat prod = tlm_mul(&g_braid_gen[2], &g_braid_gen[3]);
        TLMat I_mat = tlm_identity();
        check("sigma2 * sigma2_inv = I", tlm_eq(&prod, &I_mat));
    }

    /* Verify braid relation: sigma1*sigma2*sigma1 = sigma2*sigma1*sigma2 */
    {
        TLMat lhs, rhs, t;
        t = tlm_mul(&g_braid_gen[0], &g_braid_gen[2]);
        lhs = tlm_mul(&t, &g_braid_gen[0]);
        t = tlm_mul(&g_braid_gen[2], &g_braid_gen[0]);
        rhs = tlm_mul(&t, &g_braid_gen[2]);
        check("Braid relation: s1*s2*s1 = s2*s1*s2", tlm_eq(&lhs, &rhs));
    }

    /* ============================================================
     * Phase 1: Build 3-strand BFS catalog
     * ============================================================ */
    printf("\n=== Phase 1: 3-Strand BFS Catalog ===\n\n");
    build_3strand_catalog();

    {
        int n_interleave = 0, n_zero_bracket = 0;
        int n_non_inter = 0, n_non_inter_zero = 0;
        int w_min = 9999, w_max = -9999;

        for (i = 0; i < g_3cat_size; i++) {
            if (g_3interleave[i] == 3) n_interleave++;
            if (cyc8_is_zero(g_3bracket[i])) n_zero_bracket++;
            if (g_3interleave[i] != 3) {
                n_non_inter++;
                if (cyc8_is_zero(g_3bracket[i])) n_non_inter_zero++;
            }
            if (g_3writhe[i] < w_min) w_min = g_3writhe[i];
            if (g_3writhe[i] > w_max) w_max = g_3writhe[i];
        }

        printf("\n  Catalog size: %d\n", g_3cat_size);
        printf("  Interleaving (uses both sigma1 & sigma2): %d / %d (%.1f%%)\n",
               n_interleave, g_3cat_size,
               100.0 * (double)n_interleave / (double)g_3cat_size);
        printf("  Non-interleaving: %d (of which %d have zero bracket = %.1f%%)\n",
               n_non_inter, n_non_inter_zero,
               n_non_inter > 0 ? 100.0 * (double)n_non_inter_zero / (double)n_non_inter : 0.0);
        printf("  Zero-bracket entries: %d / %d (%.1f%%)\n",
               n_zero_bracket, g_3cat_size,
               100.0 * (double)n_zero_bracket / (double)g_3cat_size);
        printf("  Writhe range: [%d, %d]\n", w_min, w_max);

        /* Depth histogram */
        printf("\n  Depth | Count | Interleaving | Zero-bracket\n");
        printf("  ------|-------|--------------|-------------\n");
        {
            int d;
            for (d = 0; d <= 10; d++) {
                int cnt = 0, inter = 0, zb = 0;
                for (i = 0; i < g_3cat_size; i++) {
                    if (g_3depth[i] == d) {
                        cnt++;
                        if (g_3interleave[i] == 3) inter++;
                        if (cyc8_is_zero(g_3bracket[i])) zb++;
                    }
                }
                if (cnt > 0)
                    printf("  %5d | %5d | %12d | %11d\n", d, cnt, inter, zb);
            }
        }

        /* Sample brackets */
        printf("\n  Sample brackets (first 10 non-zero):\n");
        {
            int shown = 0;
            for (i = 0; i < g_3cat_size && shown < 10; i++) {
                if (!cyc8_is_zero(g_3bracket[i])) {
                    double re, im;
                    cyc8_to_double(g_3bracket[i], &re, &im);
                    printf("    [%d] depth=%d writhe=%d inter=%d bracket=(%ld,%ld,%ld,%ld) [%.3f+%.3fi]\n",
                           i, g_3depth[i], g_3writhe[i], g_3interleave[i],
                           g_3bracket[i].a, g_3bracket[i].b,
                           g_3bracket[i].c, g_3bracket[i].d, re, im);
                    shown++;
                }
            }
        }

        sprintf(msg, "3-strand catalog has %d entries", g_3cat_size);
        check(msg, g_3cat_size > 1);
        check("Entanglement vanishing: non-interleaving have zero bracket",
              n_non_inter > 0 && n_non_inter_zero == n_non_inter);
    }

    /* ============================================================
     * Phase 2: Trace readout test
     * ============================================================ */
    printf("\n=== Phase 2: Trace Readout Activation ===\n\n");
    {
        int n_distinct_traces = 0;
        Cyc8 traces[MAX_3CAT];
        int tr_count = 0;

        for (i = 0; i < g_3cat_size; i++) {
            Cyc8 tr = tlm_trace(&g_3cat[i]);
            int j, found = 0;
            for (j = 0; j < tr_count; j++) {
                if (cyc8_eq(traces[j], tr)) { found = 1; break; }
            }
            if (!found && tr_count < MAX_3CAT) {
                traces[tr_count++] = tr;
            }
        }
        n_distinct_traces = tr_count;

        printf("  Distinct trace values: %d (from %d matrices)\n",
               n_distinct_traces, g_3cat_size);

        /* Show a few */
        printf("  Sample traces:\n");
        {
            int shown = 0;
            for (i = 0; i < tr_count && shown < 8; i++) {
                double re, im;
                cyc8_to_double(traces[i], &re, &im);
                printf("    (%ld,%ld,%ld,%ld) [%.3f+%.3fi]\n",
                       traces[i].a, traces[i].b, traces[i].c, traces[i].d,
                       re, im);
                shown++;
            }
        }

        sprintf(msg, "Distinct traces: %d", n_distinct_traces);
        check(msg, n_distinct_traces > 1);
    }

    /* ============================================================
     * Phase 2b: Column-0 diversity analysis
     * ============================================================ */
    printf("\n=== Phase 2b: Column-0 Vector Diversity ===\n\n");
    {
        /* Column 0 = identity column of each matrix */
        /* Each col-0 vector is 5 Cyc8 values = 20 integers */
        typedef struct { Cyc8 v[TL_DIM]; } Col0;
        Col0 distinct[MAX_3CAT];
        int n_distinct_col0 = 0;

        for (i = 0; i < g_3cat_size; i++) {
            Col0 c;
            int j, found = 0;
            int r;
            for (r = 0; r < TL_DIM; r++)
                c.v[r] = g_3cat[i].e[r][0];

            for (j = 0; j < n_distinct_col0; j++) {
                int match = 1;
                for (r = 0; r < TL_DIM; r++) {
                    if (!cyc8_eq(c.v[r], distinct[j].v[r])) {
                        match = 0; break;
                    }
                }
                if (match) { found = 1; break; }
            }
            if (!found && n_distinct_col0 < MAX_3CAT) {
                distinct[n_distinct_col0++] = c;
            }
        }

        printf("  Distinct column-0 vectors: %d (vs %d traces, from %d matrices)\n",
               n_distinct_col0, 105, g_3cat_size);

        /* Show sample column-0 vectors */
        printf("\n  Sample column-0 vectors (first 5 non-identity):\n");
        {
            int shown = 0;
            for (i = 1; i < g_3cat_size && shown < 5; i++) {
                int r;
                int all_zero = 1;
                for (r = 0; r < TL_DIM; r++) {
                    if (!cyc8_is_zero(g_3cat[i].e[r][0])) { all_zero = 0; break; }
                }
                if (all_zero) continue;

                printf("    [%d] depth=%d writhe=%d: [", i, g_3depth[i], g_3writhe[i]);
                for (r = 0; r < TL_DIM; r++) {
                    Cyc8 v = g_3cat[i].e[r][0];
                    printf("(%ld,%ld,%ld,%ld)", v.a, v.b, v.c, v.d);
                    if (r < TL_DIM - 1) printf(", ");
                }
                printf("]\n");
                shown++;
            }
        }

        /* Also check all 5 columns and count distinct per column */
        printf("\n  Distinct vectors per column:\n");
        {
            int col;
            for (col = 0; col < TL_DIM; col++) {
                /* Quick count using nested loop */
                int n_dist = 0;
                int ii, jj;
                for (ii = 0; ii < g_3cat_size; ii++) {
                    int dup = 0;
                    for (jj = 0; jj < ii; jj++) {
                        int match = 1;
                        int r;
                        for (r = 0; r < TL_DIM; r++) {
                            if (!cyc8_eq(g_3cat[ii].e[r][col],
                                         g_3cat[jj].e[r][col])) {
                                match = 0; break;
                            }
                        }
                        if (match) { dup = 1; break; }
                    }
                    if (!dup) n_dist++;
                }
                printf("    Column %d: %d distinct\n", col, n_dist);
            }
        }

        sprintf(msg, "Column-0 distinct: %d (vs 105 traces)", n_distinct_col0);
        check(msg, 1);
    }

    /* ============================================================
     * Phase 3: 3-strand XOR capacity
     * ============================================================ */
    printf("\n=== Phase 3: 3-Strand XOR Capacity ===\n\n");
    {
        int best_k;
        int xor6_count;
        int bf_limit = g_3cat_size < 50 ? g_3cat_size : 50;

        printf("  Testing XOR6 on %d entries (bf_limit=%d)...\n",
               g_3cat_size, bf_limit);
        xor6_count = search_xor(g_3cat_size, 3, 1, bf_limit, &best_k);

        printf("\n  Best XOR6: %d winners at k=%d\n", xor6_count, best_k);

        /* If no XOR6 found, try with only non-zero-bracket entries */
        if (xor6_count == 0) {
            int nz_indices[MAX_3CAT];
            int nz_count = 0;

            printf("\n  No XOR6 found. Trying non-zero-bracket entries only...\n");
            for (i = 0; i < g_3cat_size; i++) {
                if (!cyc8_is_zero(g_3bracket[i]))
                    nz_indices[nz_count++] = i;
            }
            printf("  Non-zero-bracket: %d entries\n", nz_count);

            /* Re-order catalog: move non-zero entries to front */
            if (nz_count > 0 && nz_count < g_3cat_size) {
                TLMat temp_cat[MAX_3CAT];
                Cyc8 temp_br[MAX_3CAT];
                int temp_d[MAX_3CAT], temp_w[MAX_3CAT], temp_il[MAX_3CAT];
                int j = 0;

                for (i = 0; i < g_3cat_size; i++) {
                    if (!cyc8_is_zero(g_3bracket[i])) {
                        temp_cat[j] = g_3cat[i];
                        temp_br[j] = g_3bracket[i];
                        temp_d[j] = g_3depth[i];
                        temp_w[j] = g_3writhe[i];
                        temp_il[j] = g_3interleave[i];
                        j++;
                    }
                }
                for (i = 0; i < g_3cat_size; i++) {
                    if (cyc8_is_zero(g_3bracket[i])) {
                        temp_cat[j] = g_3cat[i];
                        temp_br[j] = g_3bracket[i];
                        temp_d[j] = g_3depth[i];
                        temp_w[j] = g_3writhe[i];
                        temp_il[j] = g_3interleave[i];
                        j++;
                    }
                }
                memcpy(g_3cat, temp_cat, sizeof(TLMat) * (size_t)g_3cat_size);
                memcpy(g_3bracket, temp_br, sizeof(Cyc8) * (size_t)g_3cat_size);
                memcpy(g_3depth, temp_d, sizeof(int) * (size_t)g_3cat_size);
                memcpy(g_3writhe, temp_w, sizeof(int) * (size_t)g_3cat_size);
                memcpy(g_3interleave, temp_il, sizeof(int) * (size_t)g_3cat_size);

                bf_limit = nz_count < 50 ? nz_count : 50;
                printf("  Re-testing XOR6 on %d non-zero entries (bf=%d)...\n",
                       nz_count, bf_limit);
                xor6_count = search_xor(nz_count, 3, 1, bf_limit, &best_k);
                printf("  Non-zero XOR6: %d winners at k=%d\n",
                       xor6_count, best_k);
            }
        }

        sprintf(msg, "3-strand XOR6: %d winners", xor6_count);
        check(msg, 1); /* report even if zero */
    }

    /* ============================================================
     * Phase 4: Matched comparison (3-strand vs 2-strand)
     * ============================================================ */
    printf("\n=== Phase 4: Matched Comparison ===\n\n");

    printf("  Building 2-strand catalog...\n");
    build_2strand_catalog();
    build_2strand_dirs();
    printf("  2-strand: %d entries, %d directions\n", g_2cat_size, g_2nd);

    {
        int match_size;
        int best_k_3, best_k_2;
        int xor6_3, xor6_2;
        int bf;
        static const int k_vals[] = {4, 6, 8, 10, 12, 16, 20, 24};
        int nk = 8;
        int ki;

        match_size = g_3cat_size < g_2cat_size ? g_3cat_size : g_2cat_size;
        if (match_size > 30) match_size = 30;
        bf = match_size;

        printf("\n  Matched comparison at size %d:\n\n", match_size);

        /* 3-strand at matched size */
        printf("  3-strand (first %d entries):\n", match_size);
        xor6_3 = 0; best_k_3 = 0;
        for (ki = 0; ki < nk; ki++) {
            int c;
            int n_cells = k_vals[ki] + 1;
            if (n_cells > MAX_ACT) continue;
            c = count_xor_bf(match_size, 3, k_vals[ki], 1, bf);
            if (c > xor6_3) { xor6_3 = c; best_k_3 = k_vals[ki]; }
            if (c > 0) printf("    k=%d: %d\n", k_vals[ki], c);
        }
        printf("  Best 3-strand XOR6: %d at k=%d\n\n", xor6_3, best_k_3);

        /* 2-strand at matched size */
        printf("  2-strand (first %d entries):\n", match_size);
        xor6_2 = 0; best_k_2 = 0;
        for (ki = 0; ki < nk; ki++) {
            int c;
            int n_vor = g_2nd + 1;
            int n_cells = k_vals[ki] * n_vor;
            if (n_cells > MAX_ACT) continue;
            c = count_xor_bf(match_size, 3, k_vals[ki], 0, bf);
            if (c > xor6_2) { xor6_2 = c; best_k_2 = k_vals[ki]; }
            if (c > 0) printf("    k=%d: %d\n", k_vals[ki], c);
        }
        printf("  Best 2-strand XOR6: %d at k=%d\n\n", xor6_2, best_k_2);

        printf("  +----------------+---------+---------+\n");
        printf("  | Metric         | 3-strand| 2-strand|\n");
        printf("  +----------------+---------+---------+\n");
        printf("  | XOR6 winners   | %7d | %7d |\n", xor6_3, xor6_2);
        printf("  | Best k_sec     | %7d | %7d |\n", best_k_3, best_k_2);
        printf("  | Catalog used   | %7d | %7d |\n", match_size, match_size);
        printf("  +----------------+---------+---------+\n\n");

        if (xor6_3 > xor6_2) {
            check("3-strand outperforms 2-strand at matched size", 1);
            printf("  --> 3-STRAND WINS: dimensional thesis supported\n");
        } else if (xor6_3 == xor6_2 && xor6_3 > 0) {
            check("3-strand ties 2-strand", 1);
            printf("  --> TIE: both achieve XOR6\n");
        } else if (xor6_2 > xor6_3) {
            check("2-strand outperforms 3-strand (unexpected)", 1);
            printf("  --> 2-STRAND WINS: dimensional thesis challenged\n");
        } else {
            check("Neither achieves XOR6 at this size", 1);
            printf("  --> INCONCLUSIVE: need larger catalogs\n");
        }
    }

    /* ============================================================
     * Phase 5: Interleaving analysis
     * ============================================================ */
    printf("\n=== Phase 5: Interleaving Analysis ===\n\n");
    {
        int inter_indices[MAX_3CAT], n_inter = 0;
        int non_inter_indices[MAX_3CAT], n_non_inter = 0;
        int xor6_inter, xor6_non, best_k_i, best_k_ni;
        int bf;
        static const int k_vals[] = {4, 6, 8, 10, 12, 16, 20, 24};
        int nk = 8;
        int ki;

        /* Split catalog */
        for (i = 0; i < g_3cat_size; i++) {
            if (g_3interleave[i] == 3) {
                inter_indices[n_inter++] = i;
            } else {
                non_inter_indices[n_non_inter++] = i;
            }
        }
        printf("  Interleaving entries: %d\n", n_inter);
        printf("  Non-interleaving entries: %d\n", n_non_inter);

        /* Test interleaving subset */
        if (n_inter > 2) {
            TLMat saved_cat[MAX_3CAT];
            Cyc8 saved_br[MAX_3CAT];
            int saved_d[MAX_3CAT], saved_w[MAX_3CAT], saved_il[MAX_3CAT];
            int saved_size = g_3cat_size;

            /* Temporarily rearrange: interleaving entries first */
            memcpy(saved_cat, g_3cat, sizeof(TLMat) * (size_t)g_3cat_size);
            memcpy(saved_br, g_3bracket, sizeof(Cyc8) * (size_t)g_3cat_size);
            memcpy(saved_d, g_3depth, sizeof(int) * (size_t)g_3cat_size);
            memcpy(saved_w, g_3writhe, sizeof(int) * (size_t)g_3cat_size);
            memcpy(saved_il, g_3interleave, sizeof(int) * (size_t)g_3cat_size);

            for (i = 0; i < n_inter; i++) {
                g_3cat[i] = saved_cat[inter_indices[i]];
                g_3bracket[i] = saved_br[inter_indices[i]];
                g_3depth[i] = saved_d[inter_indices[i]];
                g_3writhe[i] = saved_w[inter_indices[i]];
                g_3interleave[i] = saved_il[inter_indices[i]];
            }

            bf = n_inter < 50 ? n_inter : 50;
            printf("\n  Interleaving XOR6 (bf=%d):\n", bf);
            xor6_inter = 0; best_k_i = 0;
            for (ki = 0; ki < nk; ki++) {
                int c;
                int n_cells = k_vals[ki] + 1;
                if (n_cells > MAX_ACT) continue;
                c = count_xor_bf(n_inter, 3, k_vals[ki], 1, bf);
                if (c > xor6_inter) { xor6_inter = c; best_k_i = k_vals[ki]; }
                if (c > 0) printf("    k=%d: %d\n", k_vals[ki], c);
            }
            printf("  Best interleaving XOR6: %d at k=%d\n", xor6_inter, best_k_i);

            /* Non-interleaving subset */
            for (i = 0; i < n_non_inter; i++) {
                g_3cat[i] = saved_cat[non_inter_indices[i]];
                g_3bracket[i] = saved_br[non_inter_indices[i]];
                g_3depth[i] = saved_d[non_inter_indices[i]];
                g_3writhe[i] = saved_w[non_inter_indices[i]];
                g_3interleave[i] = saved_il[non_inter_indices[i]];
            }

            bf = n_non_inter < 50 ? n_non_inter : 50;
            printf("\n  Non-interleaving XOR6 (bf=%d):\n", bf);
            xor6_non = 0; best_k_ni = 0;
            for (ki = 0; ki < nk; ki++) {
                int c;
                int n_cells = k_vals[ki] + 1;
                if (n_cells > MAX_ACT) continue;
                c = count_xor_bf(n_non_inter, 3, k_vals[ki], 1, bf);
                if (c > xor6_non) { xor6_non = c; best_k_ni = k_vals[ki]; }
                if (c > 0) printf("    k=%d: %d\n", k_vals[ki], c);
            }
            printf("  Best non-interleaving XOR6: %d at k=%d\n", xor6_non, best_k_ni);

            /* Restore catalog */
            memcpy(g_3cat, saved_cat, sizeof(TLMat) * (size_t)saved_size);
            memcpy(g_3bracket, saved_br, sizeof(Cyc8) * (size_t)saved_size);
            memcpy(g_3depth, saved_d, sizeof(int) * (size_t)saved_size);
            memcpy(g_3writhe, saved_w, sizeof(int) * (size_t)saved_size);
            memcpy(g_3interleave, saved_il, sizeof(int) * (size_t)saved_size);
            g_3cat_size = saved_size;

            printf("\n  +-------------------+---------+---------+\n");
            printf("  | Subset            | XOR6    | Best k  |\n");
            printf("  +-------------------+---------+---------+\n");
            printf("  | Interleaving (%3d) | %7d | %7d |\n", n_inter, xor6_inter, best_k_i);
            printf("  | Non-interleav(%3d) | %7d | %7d |\n", n_non_inter, xor6_non, best_k_ni);
            printf("  +-------------------+---------+---------+\n\n");

            if (xor6_inter > 0 && xor6_non == 0) {
                check("Only interleaving entries compute XOR", 1);
                printf("  --> ENTANGLEMENT IS CAPACITY\n");
            } else if (xor6_inter > xor6_non) {
                check("Interleaving outperforms non-interleaving", 1);
            } else {
                check("Interleaving analysis complete", 1);
            }
        } else {
            printf("  Not enough interleaving entries for XOR6 test\n");
            check("Interleaving analysis: insufficient data", 1);
        }
    }

    /* ============================================================
     * Final results
     * ============================================================ */
    printf("\n=================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
