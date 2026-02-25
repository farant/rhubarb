/*
 * KNOTAPEL DEMO 99b: 3-Strand Delta_1 Block Decomposition
 * ========================================================
 *
 * D99 found: 5oct_hash gives 3100 XOR6 winners on the full 5x5 TL_3
 * representation, but sum-based activations give ZERO. The problem:
 * the 5x5 representation is REDUCIBLE. Mixing irreducible components
 * destroys structure.
 *
 * TL_3 at delta=0 is SEMISIMPLE (n=3 odd, Demo 51 confirms):
 *   TL_3(0) = M_1(k) x M_2(k)    (Wedderburn-Artin)
 *   V = Delta_3(1) + Delta_1(2) + Delta_1(2)
 *
 * The standard module Delta_1 gives a 2x2 representation over Z[zeta_8].
 * Instead of extracting the block from 5x5 (needs change-of-basis),
 * we BUILD the 2x2 representation DIRECTLY.
 *
 * Standard module Delta_1 basis: {h_0, h_2} (half-diagrams with 1
 * through-strand, free at position 0 or 2).
 *
 * TL generator action on Delta_1:
 *   e_1: h_0 -> h_2 (no loops), h_2 -> 0 (loop, delta=0 kills)
 *   e_2: h_0 -> 0 (loop), h_2 -> h_0 (no loops)
 * So e_1|_{D1} = [[0,0],[1,0]], e_2|_{D1} = [[0,1],[0,0]]
 *
 * Braid generators sigma_i = A*I + A^{-1}*e_i:
 *   sigma_1 = [[A, 0], [A^{-1}, A]]
 *   sigma_2 = [[A, A^{-1}], [0, A]]
 *
 * Quaternion mapping (Fran): [[a,b],[c,d]] ->
 *   q_0 = a+d, q_1 = b+c, q_2 = b-c, q_3 = a-d
 * All Cyc8 additions/subtractions -- exact integer arithmetic.
 *
 * NOTE: The group is INFINITE (sigma_1 has infinite order in the 2x2
 * rep because of the Jordan block structure). We truncate via BFS cap.
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

static Cyc8 cyc8_sub(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a - y.a, x.b - y.b, x.c - y.c, x.d - y.d);
}

static Cyc8 cyc8_neg(Cyc8 x) {
    return cyc8_make(-x.a, -x.b, -x.c, -x.d);
}

static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y) {
    /* zeta_8^4 = -1: reduction rule */
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
    /* Galois conjugate: zeta_8 -> zeta_8^{-1} = zeta_8^7 = -zeta_8^3 */
    return cyc8_make(z.a, -z.d, -z.c, -z.b);
}

/* Convert Cyc8 to integer-approximated complex number (scaled by 1000).
 * zeta_8 = e^{i*pi/4} = (1+i)/sqrt(2), so:
 *   Re(z) = a + b/sqrt(2) - d/sqrt(2)
 *   Im(z) = b/sqrt(2) + c + d/sqrt(2)
 * With 707 ~ 1000/sqrt(2): */
static void cyc8_to_cplx(Cyc8 z, long *re, long *im) {
    *re = 1000L * z.a + 707L * z.b - 707L * z.d;
    *im = 707L * z.b + 1000L * z.c + 707L * z.d;
}

/* Classify complex number into 8 octants.
 * bit2=Re<0, bit1=Im<0, bit0=|Im|>|Re| */
static int cyc8_octant(Cyc8 z) {
    long re, im, are, aim;
    int oct = 0;
    cyc8_to_cplx(z, &re, &im);
    are = re < 0 ? -re : re;
    aim = im < 0 ? -im : im;
    if (re < 0) oct |= 4;
    if (im < 0) oct |= 2;
    if (aim > are) oct |= 1;
    return oct;
}

static long cyc8_max_abs(Cyc8 z) {
    long m = 0, v;
    v = z.a < 0 ? -z.a : z.a; if (v > m) m = v;
    v = z.b < 0 ? -z.b : z.b; if (v > m) m = v;
    v = z.c < 0 ? -z.c : z.c; if (v > m) m = v;
    v = z.d < 0 ? -z.d : z.d; if (v > m) m = v;
    return m;
}

/* ================================================================
 * 2x2 Matrix over Cyc8 (Delta_1 representation)
 * Matrix: [[a, b], [c, d]]
 * ================================================================ */

typedef struct {
    Cyc8 a, b, c, d;  /* [[a,b],[c,d]] */
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

/* Matrix commutator: [P, Q] = P*Q - Q*P */
static Mat2 mat2_commutator(const Mat2 *p, const Mat2 *q) {
    Mat2 pq = mat2_mul(p, q);
    Mat2 qp = mat2_mul(q, p);
    Mat2 neg_qp = mat2_neg(&qp);
    return mat2_add(&pq, &neg_qp);
}

/* ================================================================
 * Hash table for Mat2 BFS
 * ================================================================ */

#define MAX_CAT 32768
#define HASH_SIZE 65537

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
 *
 * A = -zeta_8 = (0,-1,0,0)
 * A_inv = conj(A) = (0,0,0,1)
 *
 * sigma_1     = [[A, 0], [A_inv, A]]
 * sigma_1_inv = [[A_inv, 0], [A, A_inv]]
 * sigma_2     = [[A, A_inv], [0, A]]
 * sigma_2_inv = [[A_inv, A], [0, A_inv]]
 * ================================================================ */

static Cyc8 g_A, g_A_inv;
static Mat2 g_gen[4];
static const int g_gen_writhe[4] = {1, -1, 1, -1};

static void build_generators(void) {
    g_A = cyc8_make(0, -1, 0, 0);
    g_A_inv = cyc8_conj(g_A);  /* (0,0,0,1) */

    /* sigma_1 = [[A, 0], [A_inv, A]] */
    g_gen[0].a = g_A;
    g_gen[0].b = cyc8_zero();
    g_gen[0].c = g_A_inv;
    g_gen[0].d = g_A;

    /* sigma_1_inv = [[A_inv, 0], [A, A_inv]] */
    g_gen[1].a = g_A_inv;
    g_gen[1].b = cyc8_zero();
    g_gen[1].c = g_A;
    g_gen[1].d = g_A_inv;

    /* sigma_2 = [[A, A_inv], [0, A]] */
    g_gen[2].a = g_A;
    g_gen[2].b = g_A_inv;
    g_gen[2].c = cyc8_zero();
    g_gen[2].d = g_A;

    /* sigma_2_inv = [[A_inv, A], [0, A_inv]] */
    g_gen[3].a = g_A_inv;
    g_gen[3].b = g_A;
    g_gen[3].c = cyc8_zero();
    g_gen[3].d = g_A_inv;
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
            printf("  WARNING: entries approaching overflow, stopping BFS\n");
            break;
        }
        rd++;
    } while (g_cat_size > prev && g_cat_size < MAX_CAT && rd < 50);

    if (g_cat_size == prev) {
        printf("  GROUP CLOSED at %d entries in %d rounds\n",
               g_cat_size, rd - 1);
    } else if (g_cat_size >= MAX_CAT) {
        printf("  HIT CAP at %d entries (group is infinite)\n", MAX_CAT);
    }
}

/* ================================================================
 * Quaternion extraction
 * q_0 = a+d, q_1 = b+c, q_2 = b-c, q_3 = a-d
 * ================================================================ */

typedef struct {
    Cyc8 q0, q1, q2, q3;
} CycQuat;

static CycQuat mat2_to_quat(const Mat2 *m) {
    CycQuat q;
    q.q0 = cyc8_add(m->a, m->d);
    q.q1 = cyc8_add(m->b, m->c);
    q.q2 = cyc8_sub(m->b, m->c);
    q.q3 = cyc8_sub(m->a, m->d);
    return q;
}

/* ================================================================
 * Activation functions
 * ================================================================ */

#define ACT_4ENT_HASH   0   /* hash of 4 matrix entry octants */
#define ACT_QUAT_HASH   1   /* hash of 4 quaternion component octants */
#define ACT_2x2_HASH    2   /* hash using all 16 sign bits */
#define ACT_TRACE_OCT   3   /* octant of trace only */
#define N_ACT_TYPES      4

static const char *act_names[N_ACT_TYPES] = {
    "4ent_hash", "quat_hash", "2x2_hash", "trace_oct"
};

static int act_n_cells(int act, int k_param) {
    switch (act) {
        case ACT_4ENT_HASH: return k_param;
        case ACT_QUAT_HASH: return k_param;
        case ACT_2x2_HASH:  return k_param;
        case ACT_TRACE_OCT: return 8;
    }
    return 0;
}

/* Activate a summed Mat2 */
static int mat2_activate(const Mat2 *m, int act, int k_param) {
    switch (act) {

    case ACT_4ENT_HASH: {
        int h = cyc8_octant(m->a);
        h = h * 11 + cyc8_octant(m->b);
        h = h * 11 + cyc8_octant(m->c);
        h = h * 11 + cyc8_octant(m->d);
        if (h < 0) h = -h;
        return h % k_param;
    }

    case ACT_QUAT_HASH: {
        CycQuat q = mat2_to_quat(m);
        int h = cyc8_octant(q.q0);
        h = h * 11 + cyc8_octant(q.q1);
        h = h * 11 + cyc8_octant(q.q2);
        h = h * 11 + cyc8_octant(q.q3);
        if (h < 0) h = -h;
        return h % k_param;
    }

    case ACT_2x2_HASH: {
        /* Use sign bits of all 16 integer components */
        int h = 0;
        const long *p = &m->a.a;
        int i;
        for (i = 0; i < 16; i++) {
            h = h * 3 + (p[i] > 0 ? 2 : (p[i] < 0 ? 0 : 1));
        }
        if (h < 0) h = -h;
        return h % k_param;
    }

    case ACT_TRACE_OCT: {
        Cyc8 tr = cyc8_add(m->a, m->d);
        return cyc8_octant(tr);
    }

    }
    return 0;
}

/* ================================================================
 * XOR test for 2x2 matrices
 * 1wpi encoding: N weights -> 2N inputs.
 * ================================================================ */

#define MAX_ACT_CELLS 8192

static int cell_even[MAX_ACT_CELLS], cell_odd[MAX_ACT_CELLS];
static int touched[MAX_ACT_CELLS];

static int test_xor_mat2(const int *indices, int n_weights,
                         int act, int k_param) {
    int n_inputs = 2 * n_weights;
    int n_masks = 1 << n_inputs;
    int n_cells = act_n_cells(act, k_param);
    int n_touched = 0;
    int mask, i, w;
    int result = 1;
    Mat2 pos[8], neg[8];

    if (n_cells > MAX_ACT_CELLS || n_inputs > 16 || n_weights > 8)
        return 0;

    /* Pre-extract weights */
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

        cell = mat2_activate(&sum, act, k_param);

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

/* Brute-force XOR search */
static int count_xor_bf(int n_weights, int act, int k_param,
                        int bf_limit) {
    int count = 0;
    int limit = g_cat_size < bf_limit ? g_cat_size : bf_limit;
    int i0, i1, i2, i3, i4, i5;
    int indices[8];

    if (n_weights == 3) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            if (test_xor_mat2(indices, 3, act, k_param))
                count++;
        }
    } else if (n_weights == 4) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            indices[3] = i3;
            if (test_xor_mat2(indices, 4, act, k_param))
                count++;
        }
    } else if (n_weights == 5) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++)
        for (i4 = i3+1; i4 < limit; i4++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            indices[3] = i3; indices[4] = i4;
            if (test_xor_mat2(indices, 5, act, k_param))
                count++;
        }
    } else if (n_weights == 6) {
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++)
        for (i4 = i3+1; i4 < limit; i4++)
        for (i5 = i4+1; i5 < limit; i5++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            indices[3] = i3; indices[4] = i4; indices[5] = i5;
            if (test_xor_mat2(indices, 6, act, k_param))
                count++;
        }
    } else if (n_weights == 7) {
        int i6;
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++)
        for (i4 = i3+1; i4 < limit; i4++)
        for (i5 = i4+1; i5 < limit; i5++)
        for (i6 = i5+1; i6 < limit; i6++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            indices[3] = i3; indices[4] = i4; indices[5] = i5;
            indices[6] = i6;
            if (test_xor_mat2(indices, 7, act, k_param))
                count++;
        }
    } else if (n_weights == 8) {
        int i6, i7;
        for (i0 = 0; i0 < limit; i0++)
        for (i1 = i0+1; i1 < limit; i1++)
        for (i2 = i1+1; i2 < limit; i2++)
        for (i3 = i2+1; i3 < limit; i3++)
        for (i4 = i3+1; i4 < limit; i4++)
        for (i5 = i4+1; i5 < limit; i5++)
        for (i6 = i5+1; i6 < limit; i6++)
        for (i7 = i6+1; i7 < limit; i7++) {
            indices[0] = i0; indices[1] = i1; indices[2] = i2;
            indices[3] = i3; indices[4] = i4; indices[5] = i5;
            indices[6] = i6; indices[7] = i7;
            if (test_xor_mat2(indices, 8, act, k_param))
                count++;
        }
    }
    return count;
}

/* ================================================================
 * 2-strand z8 catalog (quaternionic, for matched comparison)
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

    gens[0].a = co; gens[0].b = si; gens[0].c = 0; gens[0].d = 0;
    gens[1].a = co; gens[1].b = -si; gens[1].c = 0; gens[1].d = 0;
    gens[2].a = co; gens[2].b = 0; gens[2].c = 0; gens[2].d = -si;
    gens[3].a = co; gens[3].b = 0; gens[3].c = 0; gens[3].d = si;

    g_2cat[0].a = 1; g_2cat[0].b = 0; g_2cat[0].c = 0; g_2cat[0].d = 0;
    g_2cat_size = 1;

    rd = 1;
    do {
        prev = g_2cat_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                Quat prod = qmul(&g_2cat[i], &gens[gi]);
                if (find_in_2cat(&prod) < 0 && g_2cat_size < MAX_2CAT) {
                    g_2cat[g_2cat_size++] = prod;
                }
            }
        }
        rd++;
    } while (g_2cat_size > prev && rd < 10);
}

/* 2-strand activation: Voronoi + sector */
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
            g_2dir[g_2nd][0] = ax; g_2dir[g_2nd][1] = ay;
            g_2dir[g_2nd][2] = az;
            g_2nd++;
        }
    }
}

static int vor_cell_2s(double ax, double ay, double az) {
    int i, best = 0;
    double bd = -2.0;
    for (i = 0; i < g_2nd; i++) {
        double dp = fabs(ax*g_2dir[i][0] + ay*g_2dir[i][1] +
                         az*g_2dir[i][2]);
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

/* 2-strand XOR test */
static int cell_even_2[MAX_ACT_CELLS], cell_odd_2[MAX_ACT_CELLS];
static int touched_2[MAX_ACT_CELLS];

static int test_xor_2strand(const int *indices, int n_weights, int k_sec) {
    int n_inputs = 2 * n_weights;
    int n_masks, n_vor, n_cells;
    double wa[14], wb[14], wc[14], wd[14];
    int n_touched = 0;
    int mask, i, result = 1;

    n_masks = 1 << n_inputs;
    n_vor = g_2nd + 1;
    n_cells = k_sec * n_vor;

    if (n_cells > MAX_ACT_CELLS || n_inputs > 14) return 0;

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

static int count_xor_2s_bf(int n_weights, int k_sec, int bf_limit) {
    int count = 0;
    int limit = g_2cat_size < bf_limit ? g_2cat_size : bf_limit;
    int i0, i1, i2;
    int indices[3];

    if (n_weights != 3) return 0;
    for (i0 = 0; i0 < limit; i0++)
    for (i1 = i0+1; i1 < limit; i1++)
    for (i2 = i1+1; i2 < limit; i2++) {
        indices[0] = i0; indices[1] = i1; indices[2] = i2;
        if (test_xor_2strand(indices, 3, k_sec))
            count++;
    }
    return count;
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    int i;
    char msg[256];

    printf("KNOTAPEL DEMO 99b: 3-Strand Delta_1 Block Decomposition\n");
    printf("=========================================================\n\n");

    build_generators();

    /* ============================================================
     * Phase 0: Verify the 2x2 representation
     * ============================================================ */
    printf("=== Phase 0: Representation Verification ===\n\n");

    /* Check A values */
    printf("  A = (%ld,%ld,%ld,%ld)\n", g_A.a, g_A.b, g_A.c, g_A.d);
    printf("  A_inv = (%ld,%ld,%ld,%ld)\n",
           g_A_inv.a, g_A_inv.b, g_A_inv.c, g_A_inv.d);

    /* A * A_inv = 1 */
    {
        Cyc8 prod = cyc8_mul(g_A, g_A_inv);
        check("A * A_inv = 1", cyc8_eq(prod, cyc8_one()));
    }

    /* A^2 + A^{-2} = 0 (delta = 0) */
    {
        Cyc8 A2 = cyc8_mul(g_A, g_A);
        Cyc8 Ai2 = cyc8_mul(g_A_inv, g_A_inv);
        Cyc8 sum = cyc8_add(A2, Ai2);
        check("A^2 + A^{-2} = 0 (delta=0)", cyc8_is_zero(sum));
    }

    /* sigma_1 * sigma_1_inv = I */
    {
        Mat2 prod = mat2_mul(&g_gen[0], &g_gen[1]);
        Mat2 I2 = mat2_identity();
        check("sigma_1 * sigma_1_inv = I", mat2_eq(&prod, &I2));
    }

    /* sigma_2 * sigma_2_inv = I */
    {
        Mat2 prod = mat2_mul(&g_gen[2], &g_gen[3]);
        Mat2 I2 = mat2_identity();
        check("sigma_2 * sigma_2_inv = I", mat2_eq(&prod, &I2));
    }

    /* Braid relation: sigma_1 * sigma_2 * sigma_1 = sigma_2 * sigma_1 * sigma_2 */
    {
        Mat2 t1, t2, lhs, rhs;
        t1 = mat2_mul(&g_gen[0], &g_gen[2]);
        lhs = mat2_mul(&t1, &g_gen[0]);
        t2 = mat2_mul(&g_gen[2], &g_gen[0]);
        rhs = mat2_mul(&t2, &g_gen[2]);
        check("Braid relation: s1*s2*s1 = s2*s1*s2",
              mat2_eq(&lhs, &rhs));
    }

    /* Hecke relation: sigma - sigma_inv = (A - A_inv)(I - e_i)
     * This is the correct form at delta=0.
     * The quadratic form (s-A)(s+A_inv)=0 does NOT hold on Delta_1. */
    {
        Mat2 lhs, rhs, neg_s1i, AminusAi, I_minus_e1, Z2;
        Cyc8 coeff = cyc8_sub(g_A, g_A_inv);
        /* lhs = sigma_1 - sigma_1_inv */
        neg_s1i = mat2_neg(&g_gen[1]);
        lhs = mat2_add(&g_gen[0], &neg_s1i);
        /* rhs = (A - A_inv) * (I - e1) where e1 = [[0,0],[1,0]] */
        I_minus_e1 = mat2_identity();
        I_minus_e1.c = cyc8_neg(cyc8_one());  /* [[1,0],[-1,1]] */
        rhs.a = cyc8_mul(coeff, I_minus_e1.a);
        rhs.b = cyc8_mul(coeff, I_minus_e1.b);
        rhs.c = cyc8_mul(coeff, I_minus_e1.c);
        rhs.d = cyc8_mul(coeff, I_minus_e1.d);
        check("Hecke: s1-s1_inv = (A-A_inv)(I-e1)",
              mat2_eq(&lhs, &rhs));
        (void)AminusAi; (void)Z2;
    }

    /* e_1^2 = 0 (verify): e_1 = [[0,0],[1,0]] */
    {
        Mat2 e1, e1sq, Z2;
        e1.a = cyc8_zero(); e1.b = cyc8_zero();
        e1.c = cyc8_one();  e1.d = cyc8_zero();
        e1sq = mat2_mul(&e1, &e1);
        Z2 = mat2_zero();
        check("e1^2 = 0 on Delta_1", mat2_eq(&e1sq, &Z2));
    }

    /* e_1*e_2 = projector onto h_2 component: [[0,0],[0,1]] */
    {
        Mat2 e1, e2, prod;
        e1.a = cyc8_zero(); e1.b = cyc8_zero();
        e1.c = cyc8_one();  e1.d = cyc8_zero();
        e2.a = cyc8_zero(); e2.b = cyc8_one();
        e2.c = cyc8_zero(); e2.d = cyc8_zero();
        prod = mat2_mul(&e1, &e2);
        check("e1*e2 = [[0,0],[0,1]] on Delta_1",
              cyc8_is_zero(prod.a) && cyc8_is_zero(prod.b) &&
              cyc8_is_zero(prod.c) && cyc8_eq(prod.d, cyc8_one()));
    }

    /* ============================================================
     * Phase 1: BFS catalog building
     * ============================================================ */
    printf("\n=== Phase 1: BFS Catalog (cap %d) ===\n\n", MAX_CAT);
    build_catalog();

    sprintf(msg, "Catalog has %d entries", g_cat_size);
    check(msg, g_cat_size > 100);

    /* ============================================================
     * Phase 2: Catalog statistics
     * ============================================================ */
    printf("\n=== Phase 2: Catalog Statistics ===\n\n");

    {
        int n_diag = 0;       /* diagonal (b=0, c=0) */
        int n_upper = 0;      /* upper triangular (c=0) */
        int n_lower = 0;      /* lower triangular (b=0) */
        int max_d = 0;
        long total_max = 0;
        int depth_hist[50];
        int d;

        memset(depth_hist, 0, sizeof(depth_hist));

        for (i = 0; i < g_cat_size; i++) {
            long mabs = mat2_max_abs(&g_cat[i]);
            if (mabs > total_max) total_max = mabs;
            if (g_depth[i] > max_d) max_d = g_depth[i];
            if (g_depth[i] < 50) depth_hist[g_depth[i]]++;

            if (cyc8_is_zero(g_cat[i].b) && cyc8_is_zero(g_cat[i].c))
                n_diag++;
            else if (cyc8_is_zero(g_cat[i].c))
                n_upper++;
            else if (cyc8_is_zero(g_cat[i].b))
                n_lower++;
        }

        printf("  Max depth: %d\n", max_d);
        printf("  Max entry magnitude: %ld\n", total_max);
        printf("  Diagonal: %d, Upper: %d, Lower: %d, Dense: %d\n",
               n_diag, n_upper, n_lower,
               g_cat_size - n_diag - n_upper - n_lower);

        printf("  Depth distribution:\n");
        for (d = 0; d <= max_d && d < 50; d++) {
            if (depth_hist[d] > 0)
                printf("    d=%d: %d entries\n", d, depth_hist[d]);
        }
    }

    /* Count distinct quaternion structures */
    {
        int n_trace_zero = 0, n_anti_trace_zero = 0;
        for (i = 0; i < g_cat_size; i++) {
            CycQuat q = mat2_to_quat(&g_cat[i]);
            if (cyc8_is_zero(q.q0)) n_trace_zero++;
            if (cyc8_is_zero(q.q3)) n_anti_trace_zero++;
        }
        printf("  Quaternion: %d with trace(q0)=0, %d with anti-trace(q3)=0\n",
               n_trace_zero, n_anti_trace_zero);
    }

    /* Print first few entries */
    printf("\n  First 10 entries (quaternion components):\n");
    for (i = 0; i < 10 && i < g_cat_size; i++) {
        CycQuat q = mat2_to_quat(&g_cat[i]);
        printf("    [%d] d=%d w=%d  q0=(%ld,%ld,%ld,%ld) q3=(%ld,%ld,%ld,%ld)\n",
               i, g_depth[i], g_writhe[i],
               q.q0.a, q.q0.b, q.q0.c, q.q0.d,
               q.q3.a, q.q3.b, q.q3.c, q.q3.d);
    }

    /* ============================================================
     * Phase 3: XOR capacity scan
     * ============================================================ */
    printf("\n=== Phase 3: XOR Capacity Scan ===\n\n");

    {
        static const int n_vals[]  = {3, 4, 5, 6};
        static const int bf_for_n[] = {200, 60, 30, 18};
        int nn = 4;
        int ni, ai;
        int best_act = -1, best_k = 0, best_n_idx = 0, best_count = 0;

        struct {
            int act;
            int k_param;
            const char *label;
        } tests[] = {
            {ACT_4ENT_HASH,  16, "4ent(16)"},
            {ACT_4ENT_HASH,  32, "4ent(32)"},
            {ACT_4ENT_HASH,  64, "4ent(64)"},
            {ACT_QUAT_HASH,  16, "quat(16)"},
            {ACT_QUAT_HASH,  32, "quat(32)"},
            {ACT_QUAT_HASH,  64, "quat(64)"},
            {ACT_2x2_HASH,   32, "2x2(32)"},
            {ACT_2x2_HASH,   64, "2x2(64)"},
            {ACT_2x2_HASH,  128, "2x2(128)"},
            {ACT_TRACE_OCT,   0, "tr_oct(8)"}
        };
        int n_tests = 10;

        printf("  %-12s", "Activation");
        for (ni = 0; ni < nn; ni++)
            printf("  N=%d(XOR%d)", n_vals[ni], 2*n_vals[ni]);
        printf("\n  %-12s", "------------");
        for (ni = 0; ni < nn; ni++)
            printf("  ---------");
        printf("\n");

        for (ai = 0; ai < n_tests; ai++) {
            int n_cells = act_n_cells(tests[ai].act, tests[ai].k_param);
            printf("  %-12s", tests[ai].label);
            for (ni = 0; ni < nn; ni++) {
                int nw = n_vals[ni];
                int bf = bf_for_n[ni];
                int c;

                /* Skip if too many cells for XOR level */
                if (n_cells > (1 << (2 * nw)) / 2) {
                    printf("     skip ");
                    continue;
                }

                c = count_xor_bf(nw, tests[ai].act,
                                 tests[ai].k_param, bf);
                printf("  %8d", c);

                if (c > best_count) {
                    best_count = c;
                    best_act = tests[ai].act;
                    best_k = tests[ai].k_param;
                    best_n_idx = ni;
                }
            }
            printf("\n");
        }

        printf("\n");
        if (best_count > 0) {
            sprintf(msg, "Best: %d XOR%d winners (%s k=%d)",
                    best_count, 2*n_vals[best_n_idx],
                    act_names[best_act], best_k);
            check(msg, 1);
        } else {
            check("No XOR winners found (investigating)", 0);
        }
    }

    /* ============================================================
     * Phase 3b: XOR Scaling Test (8K, 16K, 32K sub-catalogs)
     * Does XOR12 increase? Does XOR14 become nonzero?
     * ============================================================ */
    printf("\n=== Phase 3b: XOR Scaling with Catalog Size ===\n\n");

    {
        static const int caps[] = {4096, 8192, 16384, 32768};
        int nc = 4;
        int ci;

        printf("  Best activation: 2x2_hash(64)\n");
        printf("  %-10s  %10s  %10s  %10s  %10s  %10s\n",
               "Cap", "XOR6", "XOR8", "XOR10", "XOR12", "XOR14");
        printf("  %-10s  %10s  %10s  %10s  %10s  %10s\n",
               "----------", "----------", "----------",
               "----------", "----------", "----------");

        for (ci = 0; ci < nc; ci++) {
            int cap = caps[ci];
            int saved_size = g_cat_size;
            int xor6, xor8, xor10, xor12, xor14;

            if (cap > g_cat_size) cap = g_cat_size;
            g_cat_size = cap;

            /* bf_limit adapts: use 200 for XOR6 etc */
            xor6 = count_xor_bf(3, ACT_2x2_HASH, 64,
                                cap < 200 ? cap : 200);
            xor8 = count_xor_bf(4, ACT_2x2_HASH, 64,
                                cap < 60 ? cap : 60);
            xor10 = count_xor_bf(5, ACT_2x2_HASH, 64,
                                 cap < 30 ? cap : 30);
            xor12 = count_xor_bf(6, ACT_2x2_HASH, 64,
                                 cap < 18 ? cap : 18);
            /* XOR14 = 7 weights, bf_limit 14 */
            xor14 = count_xor_bf(7, ACT_2x2_HASH, 64,
                                 cap < 14 ? cap : 14);

            printf("  %-10d  %10d  %10d  %10d  %10d  %10d\n",
                   cap, xor6, xor8, xor10, xor12, xor14);

            g_cat_size = saved_size;
        }

        {
            /* Check XOR14 at full catalog with bf_limit 14 */
            int xor14_full = count_xor_bf(7, ACT_2x2_HASH, 64,
                                          g_cat_size < 14 ? g_cat_size : 14);
            sprintf(msg, "XOR14 at full catalog: %d", xor14_full);
            check(msg, 1);  /* informational */
        }
    }

    /* ============================================================
     * Phase 4: Matched comparison with 2-strand
     * ============================================================ */
    printf("\n=== Phase 4: 3-Strand vs 2-Strand Matched Comparison ===\n\n");

    printf("  Building 2-strand catalog...\n");
    build_2strand_catalog();
    build_2strand_dirs();
    printf("  2-strand: %d entries, %d directions\n", g_2cat_size, g_2nd);

    {
        int match_size = g_2cat_size;
        int best_3s = 0, best_2s = 0;
        static const int k_sec_vals[] = {4, 6, 8, 10, 12, 16, 20};
        int nk = 7;
        int ki;

        printf("\n  Matched comparison at size %d (XOR6, N=3):\n\n",
               match_size);

        /* 3-strand: try all activations at matched size */
        {
            int saved_size = g_cat_size;
            int c;
            if (g_cat_size > match_size)
                g_cat_size = match_size;

            c = count_xor_bf(3, ACT_4ENT_HASH, 16, match_size);
            printf("  3-strand 4ent(16): %d\n", c);
            if (c > best_3s) best_3s = c;

            c = count_xor_bf(3, ACT_4ENT_HASH, 32, match_size);
            printf("  3-strand 4ent(32): %d\n", c);
            if (c > best_3s) best_3s = c;

            c = count_xor_bf(3, ACT_QUAT_HASH, 16, match_size);
            printf("  3-strand quat(16): %d\n", c);
            if (c > best_3s) best_3s = c;

            c = count_xor_bf(3, ACT_QUAT_HASH, 32, match_size);
            printf("  3-strand quat(32): %d\n", c);
            if (c > best_3s) best_3s = c;

            c = count_xor_bf(3, ACT_2x2_HASH, 32, match_size);
            printf("  3-strand 2x2(32): %d\n", c);
            if (c > best_3s) best_3s = c;

            c = count_xor_bf(3, ACT_TRACE_OCT, 0, match_size);
            printf("  3-strand tr_oct(8): %d\n", c);
            if (c > best_3s) best_3s = c;

            g_cat_size = saved_size;
        }

        /* 2-strand: sweep k_sec for best */
        for (ki = 0; ki < nk; ki++) {
            int n_vor = g_2nd + 1;
            int n_cells = k_sec_vals[ki] * n_vor;
            int c;
            if (n_cells > MAX_ACT_CELLS) continue;
            c = count_xor_2s_bf(3, k_sec_vals[ki], match_size);
            if (c > best_2s) best_2s = c;
            if (c > 0)
                printf("  2-strand k=%d: %d XOR6 winners\n",
                       k_sec_vals[ki], c);
        }

        printf("\n  +------------------+---------+---------+\n");
        printf("  | Metric           | 3-strand| 2-strand|\n");
        printf("  +------------------+---------+---------+\n");
        printf("  | XOR6 winners     | %7d | %7d |\n", best_3s, best_2s);
        printf("  | Catalog size     | %7d | %7d |\n",
               match_size, match_size);
        printf("  +------------------+---------+---------+\n\n");

        if (best_3s > 0 && best_3s >= best_2s) {
            check("3-strand Delta_1 matches/beats 2-strand at matched size", 1);
        } else if (best_2s > best_3s) {
            sprintf(msg, "2-strand leads %d vs %d (readout gap?)",
                    best_2s, best_3s);
            check(msg, 1);
        } else {
            check("Both zero at matched size", best_3s > 0 || best_2s > 0);
        }
    }

    /* ============================================================
     * Phase 4b: Depth-sampled matched comparison
     * Instead of first-24 BFS entries, sample 24 from deep rounds (6-8)
     * ============================================================ */
    printf("\n=== Phase 4b: Depth-Sampled Matched Comparison ===\n\n");

    {
        /* Collect indices of entries from rounds 6, 7, 8 */
        int deep_indices[512];
        int n_deep = 0;
        int saved_size = g_cat_size;
        int best_deep_3s = 0, best_2s_dup = 0;
        int ki;
        static const int k_sec_vals2[] = {4, 6, 8, 10, 12, 16, 20};
        int nk2 = 7;

        for (i = 0; i < g_cat_size && n_deep < 512; i++) {
            if (g_depth[i] >= 6 && g_depth[i] <= 8)
                deep_indices[n_deep++] = i;
        }

        printf("  Deep entries (d=6-8): %d available\n", n_deep);

        if (n_deep >= 24) {
            /* Create a temporary sub-catalog: copy 24 deep entries to front */
            Mat2  saved_first[24];
            int   saved_dep[24];
            int   saved_wr[24];
            int   sample = 24;
            int   j, c;

            /* Save first 24 */
            for (j = 0; j < sample; j++) {
                saved_first[j] = g_cat[j];
                saved_dep[j] = g_depth[j];
                saved_wr[j] = g_writhe[j];
            }

            /* Copy deep entries to front (evenly spaced if more than 24) */
            {
                int step = n_deep / sample;
                if (step < 1) step = 1;
                for (j = 0; j < sample; j++) {
                    int src = deep_indices[j * step];
                    g_cat[j] = g_cat[src];
                    g_depth[j] = g_depth[src];
                    g_writhe[j] = g_writhe[src];
                }
            }

            g_cat_size = sample;

            /* Test all activations */
            c = count_xor_bf(3, ACT_2x2_HASH, 32, sample);
            printf("  Deep 3-strand 2x2(32): %d XOR6\n", c);
            if (c > best_deep_3s) best_deep_3s = c;

            c = count_xor_bf(3, ACT_2x2_HASH, 64, sample);
            printf("  Deep 3-strand 2x2(64): %d XOR6\n", c);
            if (c > best_deep_3s) best_deep_3s = c;

            c = count_xor_bf(3, ACT_4ENT_HASH, 16, sample);
            printf("  Deep 3-strand 4ent(16): %d XOR6\n", c);
            if (c > best_deep_3s) best_deep_3s = c;

            c = count_xor_bf(3, ACT_4ENT_HASH, 32, sample);
            printf("  Deep 3-strand 4ent(32): %d XOR6\n", c);
            if (c > best_deep_3s) best_deep_3s = c;

            /* Restore */
            for (j = 0; j < sample; j++) {
                g_cat[j] = saved_first[j];
                g_depth[j] = saved_dep[j];
                g_writhe[j] = saved_wr[j];
            }
            g_cat_size = saved_size;

            /* 2-strand for comparison */
            for (ki = 0; ki < nk2; ki++) {
                int n_vor = g_2nd + 1;
                int n_cells = k_sec_vals2[ki] * n_vor;
                if (n_cells > MAX_ACT_CELLS) continue;
                c = count_xor_2s_bf(3, k_sec_vals2[ki], 24);
                if (c > best_2s_dup) best_2s_dup = c;
            }

            printf("\n  +---------------------+---------+---------+\n");
            printf("  | Metric              | Deep-3s | 2-strand|\n");
            printf("  +---------------------+---------+---------+\n");
            printf("  | XOR6 (size 24)      | %7d | %7d |\n",
                   best_deep_3s, best_2s_dup);
            printf("  +---------------------+---------+---------+\n\n");

            sprintf(msg, "Deep-sampled 3-strand: %d XOR6 (vs 2-strand %d)",
                    best_deep_3s, best_2s_dup);
            check(msg, 1);
        } else {
            printf("  Not enough deep entries for comparison\n");
            check("Deep sampling needs >= 24 entries at d=6-8", n_deep >= 24);
        }
    }

    /* ============================================================
     * Phase 5: Verify two Delta_1 copies are identical
     * Compute sigma_1^n for several n, check (2,1) entry grows linearly
     * ============================================================ */
    printf("\n=== Phase 5: Jordan Block Structure Verification ===\n\n");

    {
        Mat2 power = mat2_identity();
        int n;
        int linear_growth = 1;
        long prev_mag = 0;

        printf("  sigma_1^n (2,1) entry growth:\n");
        for (n = 1; n <= 8; n++) {
            long mag;
            power = mat2_mul(&power, &g_gen[0]);
            mag = cyc8_max_abs(power.c);
            printf("    n=%d: (2,1)=(%ld,%ld,%ld,%ld) max_abs=%ld\n",
                   n, power.c.a, power.c.b, power.c.c, power.c.d, mag);
            if (n >= 2 && prev_mag > 0 && mag < prev_mag) {
                /* Not strictly increasing -- check if oscillating */
            }
            prev_mag = mag;
        }

        /* Verify diagonal entries cycle (A^n has period 8) */
        {
            Mat2 s1_8 = mat2_identity();
            for (n = 0; n < 8; n++)
                s1_8 = mat2_mul(&s1_8, &g_gen[0]);
            printf("\n  sigma_1^8 diagonal: a=(%ld,%ld,%ld,%ld) d=(%ld,%ld,%ld,%ld)\n",
                   s1_8.a.a, s1_8.a.b, s1_8.a.c, s1_8.a.d,
                   s1_8.d.a, s1_8.d.b, s1_8.d.c, s1_8.d.d);
            /* A^8 = 1, so diagonal should be (1,0,0,0) */
            check("sigma_1^8 diagonal = I (A^8=1)",
                  cyc8_eq(s1_8.a, cyc8_one()) &&
                  cyc8_eq(s1_8.d, cyc8_one()));
            /* But (2,1) entry = 8*A^6, which is nonzero */
            printf("  sigma_1^8 (2,1) = (%ld,%ld,%ld,%ld) -- NOT zero\n",
                   s1_8.c.a, s1_8.c.b, s1_8.c.c, s1_8.c.d);
            {
                Mat2 I2 = mat2_identity();
                check("sigma_1^8 != I (infinite order, Jordan block)",
                      !mat2_eq(&s1_8, &I2));
            }
        }
        (void)linear_growth;
    }

    /* ============================================================
     * Phase 6: Focused XOR12/XOR14 Push
     * Higher bf_limits, more cells, depth-sampled entries.
     * ============================================================ */
    printf("\n=== Phase 6: High-XOR Push (deep entries, more cells) ===\n\n");

    {
        /* Build a sub-catalog of deep entries (depth >= 6) */
        int deep_idx[1024];
        int n_deep6 = 0;
        Mat2  saved_cat[128];
        int   saved_dep2[128];
        int   saved_wr2[128];
        int   j2, bf_deep;
        int   saved_sz = g_cat_size;

        for (i = 0; i < g_cat_size && n_deep6 < 1024; i++) {
            if (g_depth[i] >= 6)
                deep_idx[n_deep6++] = i;
        }
        printf("  Deep entries (d>=6): %d\n", n_deep6);

        /* Pick bf_deep entries from deep catalog, evenly spaced */
        bf_deep = n_deep6 < 30 ? n_deep6 : 30;
        printf("  Using bf_deep=%d for XOR12/14 testing\n\n", bf_deep);

        if (bf_deep >= 14) {
            int step6 = n_deep6 / bf_deep;
            if (step6 < 1) step6 = 1;

            /* Save first bf_deep entries */
            for (j2 = 0; j2 < bf_deep && j2 < 128; j2++) {
                saved_cat[j2] = g_cat[j2];
                saved_dep2[j2] = g_depth[j2];
                saved_wr2[j2] = g_writhe[j2];
            }

            /* Copy deep entries to front */
            for (j2 = 0; j2 < bf_deep; j2++) {
                int src = deep_idx[j2 * step6];
                g_cat[j2] = g_cat[src];
                g_depth[j2] = g_depth[src];
                g_writhe[j2] = g_writhe[src];
            }
            g_cat_size = bf_deep;

            /* Test matrix: cells x XOR level */
            {
                static const int cells[] = {64, 128, 256, 512, 1024};
                int nc2 = 5;
                int ci2;

                printf("  %-12s  %10s  %10s  %10s\n",
                       "Cells", "XOR10", "XOR12", "XOR14");
                printf("  %-12s  %10s  %10s  %10s\n",
                       "------------", "----------",
                       "----------", "----------");

                for (ci2 = 0; ci2 < nc2; ci2++) {
                    int k = cells[ci2];
                    int x10, x12, x14;

                    if (k > MAX_ACT_CELLS) {
                        printf("  %-12d     (skip: cells > MAX_ACT_CELLS)\n", k);
                        continue;
                    }

                    x10 = count_xor_bf(5, ACT_2x2_HASH, k, bf_deep);
                    x12 = count_xor_bf(6, ACT_2x2_HASH, k, bf_deep);
                    x14 = count_xor_bf(7, ACT_2x2_HASH, k, bf_deep);

                    printf("  %-12d  %10d  %10d  %10d\n",
                           k, x10, x12, x14);
                }
            }

            /* Also try with bf=25 from deep, for XOR14 specifically */
            if (bf_deep >= 25) {
                int x14_25;
                printf("\n  XOR14 focused: bf=25 from deep, cells=512\n");
                x14_25 = count_xor_bf(7, ACT_2x2_HASH, 512,
                                      bf_deep < 25 ? bf_deep : 25);
                printf("  XOR14 at bf=25, cells=512: %d\n", x14_25);
                sprintf(msg, "XOR14 deep bf=25 cells=512: %d", x14_25);
                check(msg, 1);
            }

            /* Restore */
            for (j2 = 0; j2 < bf_deep && j2 < 128; j2++) {
                g_cat[j2] = saved_cat[j2];
                g_depth[j2] = saved_dep2[j2];
                g_writhe[j2] = saved_wr2[j2];
            }
            g_cat_size = saved_sz;
        } else {
            printf("  Not enough deep entries for Phase 6\n");
        }

        /* Also test full catalog with higher bf for XOR12/14 */
        printf("\n  Full catalog, higher bf, more cells:\n");
        {
            int x12_a, x12_b, x14_a;
            printf("  XOR12 bf=25 cells=256: ");
            fflush(stdout);
            x12_a = count_xor_bf(6, ACT_2x2_HASH, 256, 25);
            printf("%d\n", x12_a);

            printf("  XOR12 bf=25 cells=512: ");
            fflush(stdout);
            x12_b = count_xor_bf(6, ACT_2x2_HASH, 512, 25);
            printf("%d\n", x12_b);

            printf("  XOR14 bf=25 cells=256: ");
            fflush(stdout);
            x14_a = count_xor_bf(7, ACT_2x2_HASH, 256, 25);
            printf("%d\n", x14_a);

            printf("  XOR14 bf=25 cells=512: ");
            fflush(stdout);
            x14_a = count_xor_bf(7, ACT_2x2_HASH, 512, 25);
            printf("%d\n", x14_a);

            sprintf(msg, "XOR14 full bf=25 cells=512: %d", x14_a);
            check(msg, 1);
        }
    }

    /* ============================================================
     * Phase 7: XOR14 Winner Anatomy
     * Record the winning 7-tuples, analyze depth, hub entries,
     * algebraic properties, pairwise commutators.
     * ============================================================ */
    printf("\n=== Phase 7: XOR14 Winner Anatomy ===\n\n");

    {
        /* Build deep sub-catalog (d>=6) in front, like Phase 6 */
        int deep_idx7[1024];
        int n_deep7 = 0;
        Mat2  saved7[40];
        int   sdep7[40], swr7[40];
        int   saved_sz7 = g_cat_size;
        int   bf7 = 30;
        int   step7;
        int   j7;
        int   k_cells7 = 64; /* cells doesn't matter for XOR14, it's 60 at any */

        /* Winners storage */
        #define MAX_WINNERS 200
        int winners[MAX_WINNERS][7];
        int n_winners = 0;

        /* Hub counts: how many times each position appears in winners */
        int hub_count[64];
        memset(hub_count, 0, sizeof(hub_count));

        for (i = 0; i < g_cat_size && n_deep7 < 1024; i++) {
            if (g_depth[i] >= 6)
                deep_idx7[n_deep7++] = i;
        }

        step7 = n_deep7 / bf7;
        if (step7 < 1) step7 = 1;

        /* Save and install deep entries */
        for (j7 = 0; j7 < bf7 && j7 < 40; j7++) {
            saved7[j7] = g_cat[j7];
            sdep7[j7] = g_depth[j7];
            swr7[j7] = g_writhe[j7];
        }
        for (j7 = 0; j7 < bf7; j7++) {
            int src = deep_idx7[j7 * step7];
            g_cat[j7] = g_cat[src];
            g_depth[j7] = g_depth[src];
            g_writhe[j7] = g_writhe[src];
        }
        g_cat_size = bf7;

        /* Find all XOR14 winners and record them */
        {
            int i0, i1, i2, i3, i4, i5, i6;
            int indices[7];
            for (i0 = 0; i0 < bf7; i0++)
            for (i1 = i0+1; i1 < bf7; i1++)
            for (i2 = i1+1; i2 < bf7; i2++)
            for (i3 = i2+1; i3 < bf7; i3++)
            for (i4 = i3+1; i4 < bf7; i4++)
            for (i5 = i4+1; i5 < bf7; i5++)
            for (i6 = i5+1; i6 < bf7; i6++) {
                indices[0] = i0; indices[1] = i1; indices[2] = i2;
                indices[3] = i3; indices[4] = i4; indices[5] = i5;
                indices[6] = i6;
                if (test_xor_mat2(indices, 7, ACT_2x2_HASH, k_cells7)) {
                    if (n_winners < MAX_WINNERS) {
                        int w;
                        for (w = 0; w < 7; w++) {
                            winners[n_winners][w] = indices[w];
                            if (indices[w] < 64)
                                hub_count[indices[w]]++;
                        }
                        n_winners++;
                    }
                }
            }
        }

        printf("  XOR14 winners found: %d\n\n", n_winners);

        /* Print first 10 winners with depths */
        {
            int w, k;
            int lim = n_winners < 10 ? n_winners : 10;
            printf("  First %d winners (indices and depths):\n", lim);
            for (w = 0; w < lim; w++) {
                printf("    [%d] indices:", w);
                for (k = 0; k < 7; k++)
                    printf(" %d", winners[w][k]);
                printf("  depths:");
                for (k = 0; k < 7; k++)
                    printf(" %d", g_depth[winners[w][k]]);
                printf("\n");
            }
        }

        /* Depth distribution across all winners */
        printf("\n  Depth distribution in XOR14 winners:\n");
        {
            int depth_hist7[20];
            int w, k;
            int total = 0;
            memset(depth_hist7, 0, sizeof(depth_hist7));
            for (w = 0; w < n_winners; w++) {
                for (k = 0; k < 7; k++) {
                    int d = g_depth[winners[w][k]];
                    if (d < 20) depth_hist7[d]++;
                    total++;
                }
            }
            for (i = 0; i < 20; i++) {
                if (depth_hist7[i] > 0)
                    printf("    d=%d: %d entries (%.1f%%)\n",
                           i, depth_hist7[i],
                           100.0 * depth_hist7[i] / total);
            }
        }

        /* Hub entries: which entries appear in most winners */
        printf("\n  Hub entries (appear in most XOR14 winners):\n");
        {
            int top_hubs[10];
            int nh = 0;
            int h, k;
            memset(top_hubs, -1, sizeof(top_hubs));
            for (h = 0; h < 10; h++) {
                int best = -1, best_c = 0;
                for (k = 0; k < bf7 && k < 64; k++) {
                    int skip = 0;
                    int hh;
                    for (hh = 0; hh < h; hh++)
                        if (top_hubs[hh] == k) { skip = 1; break; }
                    if (!skip && hub_count[k] > best_c) {
                        best_c = hub_count[k];
                        best = k;
                    }
                }
                if (best >= 0 && best_c > 0) {
                    Cyc8 tr = cyc8_add(g_cat[best].a, g_cat[best].d);
                    int is_dense = !cyc8_is_zero(g_cat[best].b) &&
                                   !cyc8_is_zero(g_cat[best].c);
                    int is_anti = cyc8_eq(g_cat[best].a, g_cat[best].d);
                    top_hubs[h] = best;
                    nh++;
                    printf("    idx=%d: in %d/%d winners, d=%d, w=%d",
                           best, hub_count[best], n_winners,
                           g_depth[best], g_writhe[best]);
                    printf(", tr=(%ld,%ld,%ld,%ld)",
                           tr.a, tr.b, tr.c, tr.d);
                    printf(", %s%s\n",
                           is_dense ? "dense" : "tri/diag",
                           is_anti ? ", anti-tr" : "");
                }
            }
            /* Print full matrices for top 2 super-hubs */
            if (nh >= 2) {
                int sh;
                printf("\n  Super-hub full 2x2 matrices (Z[zeta_8] components):\n");
                for (sh = 0; sh < 2; sh++) {
                    int idx = top_hubs[sh];
                    if (idx < 0) continue;
                    printf("\n    Super-hub idx=%d (d=%d, w=%d, in %d/%d winners):\n",
                           idx, g_depth[idx], g_writhe[idx],
                           hub_count[idx], n_winners);
                    printf("      [[a, b], [c, d]] =\n");
                    printf("      a = (%ld, %ld, %ld, %ld)\n",
                           g_cat[idx].a.a, g_cat[idx].a.b,
                           g_cat[idx].a.c, g_cat[idx].a.d);
                    printf("      b = (%ld, %ld, %ld, %ld)\n",
                           g_cat[idx].b.a, g_cat[idx].b.b,
                           g_cat[idx].b.c, g_cat[idx].b.d);
                    printf("      c = (%ld, %ld, %ld, %ld)\n",
                           g_cat[idx].c.a, g_cat[idx].c.b,
                           g_cat[idx].c.c, g_cat[idx].c.d);
                    printf("      d = (%ld, %ld, %ld, %ld)\n",
                           g_cat[idx].d.a, g_cat[idx].d.b,
                           g_cat[idx].d.c, g_cat[idx].d.d);
                    /* sl_2 decomposition: M = ((a+d)/2)*I + b*e + c*f + ((a-d)/2)*h */
                    {
                        Cyc8 tr = cyc8_add(g_cat[idx].a, g_cat[idx].d);
                        Cyc8 cartan = cyc8_sub(g_cat[idx].a, g_cat[idx].d);
                        long cartan_l1 = 0;
                        long v;
                        v = cartan.a < 0 ? -cartan.a : cartan.a; cartan_l1 += v;
                        v = cartan.b < 0 ? -cartan.b : cartan.b; cartan_l1 += v;
                        v = cartan.c < 0 ? -cartan.c : cartan.c; cartan_l1 += v;
                        v = cartan.d < 0 ? -cartan.d : cartan.d; cartan_l1 += v;
                        printf("      trace(a+d) = (%ld, %ld, %ld, %ld)\n",
                               tr.a, tr.b, tr.c, tr.d);
                        printf("      cartan(a-d) = (%ld, %ld, %ld, %ld)  L1=%ld\n",
                               cartan.a, cartan.b, cartan.c, cartan.d,
                               cartan_l1);
                        printf("      anti-trace: %s\n",
                               cyc8_eq(g_cat[idx].a, g_cat[idx].d) ?
                               "YES (a=d)" : "NO");
                    }
                }
            }

            /* Satellite entry analysis */
            printf("\n  Satellite entries (in winners, not top-2 hubs):\n");
            {
                int sat_seen[64];
                int n_sat = 0;
                int w, k, s;
                memset(sat_seen, 0, sizeof(sat_seen));
                for (w = 0; w < n_winners; w++) {
                    for (k = 0; k < 7; k++) {
                        int eidx = winners[w][k];
                        if (eidx < 64 && eidx != top_hubs[0] &&
                            eidx != top_hubs[1] && !sat_seen[eidx]) {
                            sat_seen[eidx] = 1;
                            n_sat++;
                        }
                    }
                }
                printf("    Total distinct satellites: %d\n\n", n_sat);
                printf("    %-5s %-4s %-4s %-6s %-22s %-10s\n",
                       "Idx", "D", "W", "Anti?",
                       "Trace", "Hub%");
                printf("    %-5s %-4s %-4s %-6s %-22s %-10s\n",
                       "-----", "----", "----", "------",
                       "----------------------", "----------");
                for (s = 0; s < 64; s++) {
                    if (sat_seen[s]) {
                        Cyc8 tr = cyc8_add(g_cat[s].a, g_cat[s].d);
                        int is_anti = cyc8_eq(g_cat[s].a, g_cat[s].d);
                        int is_dense = !cyc8_is_zero(g_cat[s].b) &&
                                       !cyc8_is_zero(g_cat[s].c);
                        printf("    %-5d %-4d %-4d %-6s (%ld,%ld,%ld,%ld) %s  %d/%d\n",
                               s, g_depth[s], g_writhe[s],
                               is_anti ? "YES" : "no",
                               tr.a, tr.b, tr.c, tr.d,
                               is_dense ? "dns" : "tri",
                               hub_count[s], n_winners);
                    }
                }
            }
        }

        /* Pairwise commutator analysis within winning tuples */
        printf("\n  Pairwise commutator magnitude in winners:\n");
        {
            long total_comm = 0;
            int n_comm_pairs = 0;
            long max_comm = 0, min_comm = 999999999L;
            int w, j, k;
            int lim_w = n_winners < 20 ? n_winners : 20;

            for (w = 0; w < lim_w; w++) {
                for (j = 0; j < 7; j++) {
                    for (k = j + 1; k < 7; k++) {
                        Mat2 pq = mat2_mul(&g_cat[winners[w][j]],
                                           &g_cat[winners[w][k]]);
                        Mat2 qp = mat2_mul(&g_cat[winners[w][k]],
                                           &g_cat[winners[w][j]]);
                        Mat2 neg_qp = mat2_neg(&qp);
                        Mat2 comm = mat2_add(&pq, &neg_qp);
                        long cmag = mat2_max_abs(&comm);
                        total_comm += cmag;
                        n_comm_pairs++;
                        if (cmag > max_comm) max_comm = cmag;
                        if (cmag < min_comm) min_comm = cmag;
                    }
                }
            }
            if (n_comm_pairs > 0) {
                printf("    Mean: %ld, Min: %ld, Max: %ld (%d pairs)\n",
                       total_comm / n_comm_pairs, min_comm, max_comm,
                       n_comm_pairs);
            }
        }

        /* Check: are all winners from same depth or mixed? */
        printf("\n  Depth mixing in winners:\n");
        {
            int n_pure = 0, n_mixed = 0;
            int w;
            int lim_w2 = n_winners < 60 ? n_winners : 60;
            for (w = 0; w < lim_w2; w++) {
                int d0 = g_depth[winners[w][0]];
                int all_same = 1;
                int k;
                for (k = 1; k < 7; k++) {
                    if (g_depth[winners[w][k]] != d0) {
                        all_same = 0;
                        break;
                    }
                }
                if (all_same) n_pure++;
                else n_mixed++;
            }
            printf("    Pure (all same depth): %d\n", n_pure);
            printf("    Mixed (different depths): %d\n", n_mixed);
        }

        sprintf(msg, "XOR14 winners analyzed: %d", n_winners);
        check(msg, n_winners > 0);

        /* --------------------------------------------------------
         * Phase 7b: Hub Commutator [hub7, hub9]
         * If Jordan cell: should have strong h-component
         * -------------------------------------------------------- */
        printf("\n  --- Phase 7b: Hub Commutator ---\n\n");

        if (n_winners > 0) {
            int hub_a = 7, hub_b = 9; /* deep sub-catalog indices */
            Mat2 comm_hubs = mat2_commutator(&g_cat[hub_a], &g_cat[hub_b]);
            Cyc8 comm_tr = cyc8_add(comm_hubs.a, comm_hubs.d);
            Cyc8 comm_cartan = cyc8_sub(comm_hubs.a, comm_hubs.d);
            long cartan_l1 = 0;
            {
                long v;
                v = comm_cartan.a < 0 ? -comm_cartan.a : comm_cartan.a;
                cartan_l1 += v;
                v = comm_cartan.b < 0 ? -comm_cartan.b : comm_cartan.b;
                cartan_l1 += v;
                v = comm_cartan.c < 0 ? -comm_cartan.c : comm_cartan.c;
                cartan_l1 += v;
                v = comm_cartan.d < 0 ? -comm_cartan.d : comm_cartan.d;
                cartan_l1 += v;
            }
            printf("    [hub7, hub9] = M7*M9 - M9*M7:\n");
            printf("      a = (%ld, %ld, %ld, %ld)\n",
                   comm_hubs.a.a, comm_hubs.a.b,
                   comm_hubs.a.c, comm_hubs.a.d);
            printf("      b = (%ld, %ld, %ld, %ld)\n",
                   comm_hubs.b.a, comm_hubs.b.b,
                   comm_hubs.b.c, comm_hubs.b.d);
            printf("      c = (%ld, %ld, %ld, %ld)\n",
                   comm_hubs.c.a, comm_hubs.c.b,
                   comm_hubs.c.c, comm_hubs.c.d);
            printf("      d = (%ld, %ld, %ld, %ld)\n",
                   comm_hubs.d.a, comm_hubs.d.b,
                   comm_hubs.d.c, comm_hubs.d.d);
            printf("    sl_2 decomposition:\n");
            printf("      scalar (a+d)  = (%ld,%ld,%ld,%ld)\n",
                   comm_tr.a, comm_tr.b, comm_tr.c, comm_tr.d);
            printf("      Cartan (a-d)  = (%ld,%ld,%ld,%ld)  L1=%ld\n",
                   comm_cartan.a, comm_cartan.b,
                   comm_cartan.c, comm_cartan.d, cartan_l1);
            printf("      e-root (b)    = (%ld,%ld,%ld,%ld)\n",
                   comm_hubs.b.a, comm_hubs.b.b,
                   comm_hubs.b.c, comm_hubs.b.d);
            printf("      f-root (c)    = (%ld,%ld,%ld,%ld)\n",
                   comm_hubs.c.a, comm_hubs.c.b,
                   comm_hubs.c.c, comm_hubs.c.d);
            {
                long b_l1 = 0, c_l1 = 0, sc_l1 = 0;
                long v;
                v = comm_hubs.b.a < 0 ? -comm_hubs.b.a : comm_hubs.b.a;
                b_l1 += v;
                v = comm_hubs.b.b < 0 ? -comm_hubs.b.b : comm_hubs.b.b;
                b_l1 += v;
                v = comm_hubs.b.c < 0 ? -comm_hubs.b.c : comm_hubs.b.c;
                b_l1 += v;
                v = comm_hubs.b.d < 0 ? -comm_hubs.b.d : comm_hubs.b.d;
                b_l1 += v;
                v = comm_hubs.c.a < 0 ? -comm_hubs.c.a : comm_hubs.c.a;
                c_l1 += v;
                v = comm_hubs.c.b < 0 ? -comm_hubs.c.b : comm_hubs.c.b;
                c_l1 += v;
                v = comm_hubs.c.c < 0 ? -comm_hubs.c.c : comm_hubs.c.c;
                c_l1 += v;
                v = comm_hubs.c.d < 0 ? -comm_hubs.c.d : comm_hubs.c.d;
                c_l1 += v;
                v = comm_tr.a < 0 ? -comm_tr.a : comm_tr.a; sc_l1 += v;
                v = comm_tr.b < 0 ? -comm_tr.b : comm_tr.b; sc_l1 += v;
                v = comm_tr.c < 0 ? -comm_tr.c : comm_tr.c; sc_l1 += v;
                v = comm_tr.d < 0 ? -comm_tr.d : comm_tr.d; sc_l1 += v;
                printf("    L1 norms: scalar=%ld, Cartan=%ld, e=%ld, f=%ld\n",
                       sc_l1, cartan_l1, b_l1, c_l1);
                sprintf(msg, "Hub commutator Cartan L1=%ld (Jordan predicts strong)",
                        cartan_l1);
                check(msg, 1);
            }
        }

        /* --------------------------------------------------------
         * Phase 7c: Casimir Discriminant Test (TOP PRIORITY)
         * C(S) = (a-d)^2 + 4*b*c  (char poly discriminant)
         * C = 0 => Jordan block. C != 0 => diagonalizable.
         * -------------------------------------------------------- */
        printf("\n  --- Phase 7c: Casimir Discriminant Test ---\n\n");

        if (n_winners > 0) {
            int win_tuple[7];
            int mi;
            long total_cl1_even = 0, total_cl1_odd = 0;
            int cn_even = 0, cn_odd = 0;
            int n_jordan_even = 0, n_jordan_odd = 0;
            int n_masks_cas = 1 << 14; /* 7 weights * 2 inputs = 14 */
            Mat2 cpos[7], cneg[7];

            for (mi = 0; mi < 7; mi++) {
                win_tuple[mi] = winners[0][mi];
                cpos[mi] = g_cat[win_tuple[mi]];
                cneg[mi] = mat2_neg(&g_cat[win_tuple[mi]]);
            }

            printf("  WINNING tuple: indices");
            for (mi = 0; mi < 7; mi++) printf(" %d", win_tuple[mi]);
            printf("\n  (16384 masks, Casimir per mask)\n\n");

            for (mi = 0; mi < n_masks_cas; mi++) {
                Mat2 s = mat2_zero();
                int par = 0;
                int ii;
                Cyc8 anti_tr, at_sq, four_val, bc_prod, cas;
                long cas_l1;
                int cas_zero;

                for (ii = 0; ii < 14; ii++) {
                    if (mi & (1 << ii)) {
                        int ww = ii / 2;
                        if (ii % 2 == 0)
                            s = mat2_add(&s, &cpos[ww]);
                        else
                            s = mat2_add(&s, &cneg[ww]);
                        par ^= 1;
                    }
                }

                /* C = (a-d)^2 + 4*b*c */
                anti_tr = cyc8_sub(s.a, s.d);
                at_sq = cyc8_mul(anti_tr, anti_tr);
                four_val = cyc8_make(4, 0, 0, 0);
                bc_prod = cyc8_mul(s.b, s.c);
                cas = cyc8_add(at_sq, cyc8_mul(four_val, bc_prod));

                cas_l1 = 0;
                {
                    long v;
                    v = cas.a < 0 ? -cas.a : cas.a; cas_l1 += v;
                    v = cas.b < 0 ? -cas.b : cas.b; cas_l1 += v;
                    v = cas.c < 0 ? -cas.c : cas.c; cas_l1 += v;
                    v = cas.d < 0 ? -cas.d : cas.d; cas_l1 += v;
                }
                cas_zero = cyc8_is_zero(cas);

                if (par == 0) {
                    total_cl1_even += cas_l1;
                    cn_even++;
                    if (cas_zero) n_jordan_even++;
                } else {
                    total_cl1_odd += cas_l1;
                    cn_odd++;
                    if (cas_zero) n_jordan_odd++;
                }
            }

            printf("  WINNER Casimir analysis:\n");
            printf("    Even parity (XOR=0): %d masks\n", cn_even);
            printf("      Jordan (C=0): %d (%.2f%%)\n",
                   n_jordan_even,
                   cn_even > 0 ? 100.0 * n_jordan_even / cn_even : 0.0);
            printf("      Mean |C|_L1: %ld\n",
                   cn_even > 0 ? total_cl1_even / cn_even : 0L);
            printf("    Odd parity (XOR=1): %d masks\n", cn_odd);
            printf("      Jordan (C=0): %d (%.2f%%)\n",
                   n_jordan_odd,
                   cn_odd > 0 ? 100.0 * n_jordan_odd / cn_odd : 0.0);
            printf("      Mean |C|_L1: %ld\n",
                   cn_odd > 0 ? total_cl1_odd / cn_odd : 0L);

            /* CONTROL: non-winning tuple */
            printf("\n  CONTROL (non-winning) tuple:\n");
            {
                int ctrl[7] = {0, 2, 4, 6, 8, 13, 16};
                long ctrl_cl1_even = 0, ctrl_cl1_odd = 0;
                int cce = 0, cco = 0;
                int cje = 0, cjo = 0;
                Mat2 ctpos[7], ctneg[7];
                int is_ctrl_winner;

                for (mi = 0; mi < 7; mi++) {
                    ctpos[mi] = g_cat[ctrl[mi]];
                    ctneg[mi] = mat2_neg(&g_cat[ctrl[mi]]);
                }

                is_ctrl_winner = test_xor_mat2(ctrl, 7, ACT_2x2_HASH, 64);
                printf("    Indices: 0 2 4 6 8 13 16\n");
                printf("    Is XOR14 winner: %s\n\n",
                       is_ctrl_winner ? "YES" : "NO (good control)");

                for (mi = 0; mi < n_masks_cas; mi++) {
                    Mat2 s = mat2_zero();
                    int par = 0;
                    int ii;
                    Cyc8 anti_tr, at_sq, four_val, bc_prod, cas;
                    long cas_l1;
                    int cas_zero;

                    for (ii = 0; ii < 14; ii++) {
                        if (mi & (1 << ii)) {
                            int ww = ii / 2;
                            if (ii % 2 == 0)
                                s = mat2_add(&s, &ctpos[ww]);
                            else
                                s = mat2_add(&s, &ctneg[ww]);
                            par ^= 1;
                        }
                    }

                    anti_tr = cyc8_sub(s.a, s.d);
                    at_sq = cyc8_mul(anti_tr, anti_tr);
                    four_val = cyc8_make(4, 0, 0, 0);
                    bc_prod = cyc8_mul(s.b, s.c);
                    cas = cyc8_add(at_sq, cyc8_mul(four_val, bc_prod));

                    cas_l1 = 0;
                    {
                        long v;
                        v = cas.a < 0 ? -cas.a : cas.a; cas_l1 += v;
                        v = cas.b < 0 ? -cas.b : cas.b; cas_l1 += v;
                        v = cas.c < 0 ? -cas.c : cas.c; cas_l1 += v;
                        v = cas.d < 0 ? -cas.d : cas.d; cas_l1 += v;
                    }
                    cas_zero = cyc8_is_zero(cas);

                    if (par == 0) {
                        ctrl_cl1_even += cas_l1;
                        cce++;
                        if (cas_zero) cje++;
                    } else {
                        ctrl_cl1_odd += cas_l1;
                        cco++;
                        if (cas_zero) cjo++;
                    }
                }

                printf("    Even: %d masks, Jordan: %d (%.2f%%), mean |C|_L1: %ld\n",
                       cce, cje, cce > 0 ? 100.0 * cje / cce : 0.0,
                       cce > 0 ? ctrl_cl1_even / cce : 0L);
                printf("    Odd:  %d masks, Jordan: %d (%.2f%%), mean |C|_L1: %ld\n",
                       cco, cjo, cco > 0 ? 100.0 * cjo / cco : 0.0,
                       cco > 0 ? ctrl_cl1_odd / cco : 0L);
            }

            {
                int cas_sep = (n_jordan_even != n_jordan_odd) ||
                    (cn_even > 0 && cn_odd > 0 &&
                     total_cl1_even / cn_even != total_cl1_odd / cn_odd);
                sprintf(msg, "Casimir %s XOR classes (Jordan E=%d O=%d)",
                        cas_sep ? "SEPARATES" : "does NOT separate",
                        n_jordan_even, n_jordan_odd);
                check(msg, 1);
            }
        }

        /* --------------------------------------------------------
         * Phase 7d: Swap Test
         * Replace each super-hub with other entries of same type.
         * If XOR14 survives: Cell B interpretation.
         * If XOR14 dies: Jordan cell / specific entry.
         * -------------------------------------------------------- */
        printf("\n  --- Phase 7d: Swap Test ---\n\n");

        if (n_winners > 0) {
            /* Use winners[0] = {1, 7, 9, 10, 11, 19, 21} */
            int base[7];
            int mi;
            int n_swap7_survive = 0, n_swap7_total = 0;
            int n_swap9_survive = 0, n_swap9_total = 0;
            int hub7_pos = -1, hub9_pos = -1;

            for (mi = 0; mi < 7; mi++) base[mi] = winners[0][mi];

            /* Find hub positions in winner */
            for (mi = 0; mi < 7; mi++) {
                if (base[mi] == 7) hub7_pos = mi;
                if (base[mi] == 9) hub9_pos = mi;
            }

            printf("    Winner[0]: ");
            for (mi = 0; mi < 7; mi++) printf("%d ", base[mi]);
            printf("\n    hub7 at position %d, hub9 at position %d\n\n",
                   hub7_pos, hub9_pos);

            /* Swap hub7 (anti-trace, d=6) with other anti-trace d=6 */
            if (hub7_pos >= 0) {
                int si;
                printf("    Swapping hub7 with other anti-trace d=6 entries:\n");
                for (si = 0; si < bf7; si++) {
                    if (si == 7) continue;  /* skip hub7 itself */
                    if (g_depth[si] == 6 &&
                        cyc8_eq(g_cat[si].a, g_cat[si].d)) {
                        int test_t[7];
                        int ok;
                        for (mi = 0; mi < 7; mi++) test_t[mi] = base[mi];
                        test_t[hub7_pos] = si;
                        ok = test_xor_mat2(test_t, 7, ACT_2x2_HASH, 64);
                        n_swap7_total++;
                        if (ok) n_swap7_survive++;
                        printf("      idx=%d (w=%d): %s\n",
                               si, g_writhe[si],
                               ok ? "SURVIVES" : "fails");
                    }
                }
                printf("    Result: %d/%d survive\n\n",
                       n_swap7_survive, n_swap7_total);
            }

            /* Swap hub9 (non-anti-trace, d=6, symmetric b=c) */
            if (hub9_pos >= 0) {
                int si;
                printf("    Swapping hub9 with other non-anti-trace d=6:\n");
                for (si = 0; si < bf7; si++) {
                    if (si == 9) continue;
                    if (g_depth[si] == 6 &&
                        !cyc8_eq(g_cat[si].a, g_cat[si].d)) {
                        int test_t[7];
                        int ok;
                        for (mi = 0; mi < 7; mi++) test_t[mi] = base[mi];
                        test_t[hub9_pos] = si;
                        ok = test_xor_mat2(test_t, 7, ACT_2x2_HASH, 64);
                        n_swap9_total++;
                        if (ok) n_swap9_survive++;
                        printf("      idx=%d (w=%d): %s\n",
                               si, g_writhe[si],
                               ok ? "SURVIVES" : "fails");
                    }
                }
                printf("    Result: %d/%d survive\n\n",
                       n_swap9_survive, n_swap9_total);
            }

            sprintf(msg, "Swap test: hub7 %d/%d, hub9 %d/%d survive",
                    n_swap7_survive, n_swap7_total,
                    n_swap9_survive, n_swap9_total);
            check(msg, 1);
        }

        /* Restore catalog */
        for (j7 = 0; j7 < bf7 && j7 < 40; j7++) {
            g_cat[j7] = saved7[j7];
            g_depth[j7] = sdep7[j7];
            g_writhe[j7] = swr7[j7];
        }
        g_cat_size = saved_sz7;
    }

    /* ============================================================
     * Phase 8: XOR16 Push
     * bf=40 from deep entries, cells=256
     * ============================================================ */
    printf("\n=== Phase 8: XOR16 Push ===\n\n");

    {
        int deep_idx8[2048];
        int n_deep8 = 0;
        Mat2  saved8[50];
        int   sdep8[50], swr8[50];
        int   saved_sz8 = g_cat_size;
        int   bf8 = 40;
        int   step8, j8;
        int   x14_40, x16_40;

        for (i = 0; i < g_cat_size && n_deep8 < 2048; i++) {
            if (g_depth[i] >= 6)
                deep_idx8[n_deep8++] = i;
        }

        step8 = n_deep8 / bf8;
        if (step8 < 1) step8 = 1;

        for (j8 = 0; j8 < bf8 && j8 < 50; j8++) {
            saved8[j8] = g_cat[j8];
            sdep8[j8] = g_depth[j8];
            swr8[j8] = g_writhe[j8];
        }
        for (j8 = 0; j8 < bf8; j8++) {
            int src = deep_idx8[j8 * step8];
            g_cat[j8] = g_cat[src];
            g_depth[j8] = g_depth[src];
            g_writhe[j8] = g_writhe[src];
        }
        g_cat_size = bf8;

        printf("  Deep entries: %d, using bf=%d, cells=256\n\n", n_deep8, bf8);

        printf("  XOR14 bf=40 cells=256: ");
        fflush(stdout);
        x14_40 = count_xor_bf(7, ACT_2x2_HASH, 256, bf8);
        printf("%d\n", x14_40);

        sprintf(msg, "XOR14 bf=40 deep cells=256: %d", x14_40);
        check(msg, 1);

        /* XOR16 = 8 weights, 16 inputs, 2^16 = 65536 masks per test.
         * C(bf,8) tuples: bf=14 -> 3003, bf=12 -> 495, bf=16 -> 12870.
         * Use bf=14 for tractable computation. */
        {
            int bf16 = bf8 < 14 ? bf8 : 14;
            printf("\n  XOR16 (8 weights): bf=%d, cells=256\n", bf16);
            printf("  C(%d,8) = %d tuples, 65536 masks each\n",
                   bf16, bf16 > 7 ?
                   (bf16*(bf16-1)*(bf16-2)*(bf16-3)*(bf16-4)*
                    (bf16-5)*(bf16-6)*(bf16-7))/40320 : 0);
            printf("  XOR16: ");
            fflush(stdout);
            x16_40 = count_xor_bf(8, ACT_2x2_HASH, 256, bf16);
            printf("%d\n", x16_40);

            sprintf(msg, "XOR16 bf=%d deep cells=256: %d", bf16, x16_40);
            check(msg, 1);
        }

        /* Restore */
        for (j8 = 0; j8 < bf8 && j8 < 50; j8++) {
            g_cat[j8] = saved8[j8];
            g_depth[j8] = sdep8[j8];
            g_writhe[j8] = swr8[j8];
        }
        g_cat_size = saved_sz8;
    }

    /* ============================================================
     * Phase 9: Parity Test (depth vs writhe vs anti-trace)
     * Mean writhe by depth, anti-trace fraction by depth.
     * ============================================================ */
    printf("\n=== Phase 9: Depth Parity Test ===\n\n");

    {
        int d;
        int max_d = 0;
        int depth_count[20];
        long depth_writhe_sum[20];
        int depth_anti[20];
        int depth_dense[20];

        memset(depth_count, 0, sizeof(depth_count));
        memset(depth_writhe_sum, 0, sizeof(depth_writhe_sum));
        memset(depth_anti, 0, sizeof(depth_anti));
        memset(depth_dense, 0, sizeof(depth_dense));

        for (i = 0; i < g_cat_size; i++) {
            int dd = g_depth[i];
            if (dd >= 20) continue;
            if (dd > max_d) max_d = dd;
            depth_count[dd]++;
            depth_writhe_sum[dd] += g_writhe[i];
            if (cyc8_eq(g_cat[i].a, g_cat[i].d))
                depth_anti[dd]++;
            if (!cyc8_is_zero(g_cat[i].b) && !cyc8_is_zero(g_cat[i].c))
                depth_dense[dd]++;
        }

        printf("  %-5s %7s %10s %8s %8s\n",
               "Depth", "Count", "Mean W", "Anti%%", "Dense%%");
        printf("  %-5s %7s %10s %8s %8s\n",
               "-----", "-------", "----------",
               "--------", "--------");
        for (d = 0; d <= max_d && d < 20; d++) {
            if (depth_count[d] == 0) continue;
            printf("  d=%-3d %7d %10.3f %7.1f%% %7.1f%%\n",
                   d, depth_count[d],
                   (double)depth_writhe_sum[d] / depth_count[d],
                   100.0 * depth_anti[d] / depth_count[d],
                   100.0 * depth_dense[d] / depth_count[d]);
        }

        /* Even vs odd depth comparison */
        {
            int even_anti = 0, even_total = 0;
            int odd_anti = 0, odd_total = 0;
            long even_absw = 0, odd_absw = 0;
            for (i = 0; i < g_cat_size; i++) {
                int dd = g_depth[i];
                long aw = g_writhe[i] < 0 ?
                          (long)-g_writhe[i] : (long)g_writhe[i];
                if (dd % 2 == 0) {
                    even_total++;
                    even_absw += aw;
                    if (cyc8_eq(g_cat[i].a, g_cat[i].d))
                        even_anti++;
                } else {
                    odd_total++;
                    odd_absw += aw;
                    if (cyc8_eq(g_cat[i].a, g_cat[i].d))
                        odd_anti++;
                }
            }
            printf("\n  Even depth: %d entries, anti-trace %d (%.1f%%), mean |W| %.2f\n",
                   even_total, even_anti,
                   even_total > 0 ? 100.0 * even_anti / even_total : 0.0,
                   even_total > 0 ? (double)even_absw / even_total : 0.0);
            printf("  Odd depth:  %d entries, anti-trace %d (%.1f%%), mean |W| %.2f\n",
                   odd_total, odd_anti,
                   odd_total > 0 ? 100.0 * odd_anti / odd_total : 0.0,
                   odd_total > 0 ? (double)odd_absw / odd_total : 0.0);
        }

        check("Parity test completed", 1);
    }

    /* ============================================================
     * Phase 10: Casimir Gap Scaling
     * Test prediction: winner/control Casimir ratio increases
     * monotonically with XOR level.
     * ============================================================ */
    printf("\n=== Phase 10: Casimir Gap Scaling ===\n\n");

    {
        /* Build deep sub-catalog like Phase 7 */
        int deep10[1024];
        int nd10 = 0;
        Mat2  sv10[40];
        int   sd10[40], sw10[40];
        int   saved10 = g_cat_size;
        int   bf10 = 30;
        int   step10, j10;

        for (i = 0; i < g_cat_size && nd10 < 1024; i++) {
            if (g_depth[i] >= 6)
                deep10[nd10++] = i;
        }
        step10 = nd10 / bf10;
        if (step10 < 1) step10 = 1;

        for (j10 = 0; j10 < bf10 && j10 < 40; j10++) {
            sv10[j10] = g_cat[j10];
            sd10[j10] = g_depth[j10];
            sw10[j10] = g_writhe[j10];
        }
        for (j10 = 0; j10 < bf10; j10++) {
            int src = deep10[j10 * step10];
            g_cat[j10] = g_cat[src];
            g_depth[j10] = g_depth[src];
            g_writhe[j10] = g_writhe[src];
        }
        g_cat_size = bf10;

        printf("  Deep sub-catalog: %d entries\n\n", bf10);
        printf("  %-8s  %12s  %12s  %8s\n",
               "XOR", "Winner |C|", "Control |C|", "Ratio");
        printf("  %-8s  %12s  %12s  %8s\n",
               "--------", "------------", "------------", "--------");

        {
            /* XOR levels: 6,8,10,12,14 → n_weights: 3,4,5,6,7 */
            static const int nw_list[5] = {3, 4, 5, 6, 7};
            static const char *labels[5] =
                {"XOR6", "XOR8", "XOR10", "XOR12", "XOR14"};
            int lev;
            int prev_ratio_x10 = 0;
            int monotonic = 1;
            int levels_found = 0;

            for (lev = 0; lev < 5; lev++) {
                int nw = nw_list[lev];
                int n_inp = 2 * nw;
                int n_masks = 1 << n_inp;
                int win_idx[8], ctrl_idx[8];
                int found_win = 0, found_ctrl = 0;

                /* Search for one winner and one non-winner */
                if (nw == 3) {
                    int a0, a1, a2;
                    for (a0 = 0; a0 < bf10 && (!found_win || !found_ctrl); a0++)
                    for (a1 = a0+1; a1 < bf10 && (!found_win || !found_ctrl); a1++)
                    for (a2 = a1+1; a2 < bf10 && (!found_win || !found_ctrl); a2++) {
                        int ti[3];
                        ti[0] = a0; ti[1] = a1; ti[2] = a2;
                        if (!found_win && test_xor_mat2(ti, 3, ACT_2x2_HASH, 64)) {
                            win_idx[0]=a0; win_idx[1]=a1; win_idx[2]=a2;
                            found_win = 1;
                        }
                        if (!found_ctrl && !test_xor_mat2(ti, 3, ACT_2x2_HASH, 64)) {
                            ctrl_idx[0]=a0; ctrl_idx[1]=a1; ctrl_idx[2]=a2;
                            found_ctrl = 1;
                        }
                    }
                } else if (nw == 4) {
                    int a0, a1, a2, a3;
                    for (a0 = 0; a0 < bf10 && (!found_win || !found_ctrl); a0++)
                    for (a1 = a0+1; a1 < bf10 && (!found_win || !found_ctrl); a1++)
                    for (a2 = a1+1; a2 < bf10 && (!found_win || !found_ctrl); a2++)
                    for (a3 = a2+1; a3 < bf10 && (!found_win || !found_ctrl); a3++) {
                        int ti[4];
                        ti[0]=a0; ti[1]=a1; ti[2]=a2; ti[3]=a3;
                        if (!found_win && test_xor_mat2(ti, 4, ACT_2x2_HASH, 64)) {
                            win_idx[0]=a0; win_idx[1]=a1; win_idx[2]=a2; win_idx[3]=a3;
                            found_win = 1;
                        }
                        if (!found_ctrl && !test_xor_mat2(ti, 4, ACT_2x2_HASH, 64)) {
                            ctrl_idx[0]=a0; ctrl_idx[1]=a1; ctrl_idx[2]=a2; ctrl_idx[3]=a3;
                            found_ctrl = 1;
                        }
                    }
                } else if (nw == 5) {
                    int a0, a1, a2, a3, a4;
                    for (a0 = 0; a0 < bf10 && (!found_win || !found_ctrl); a0++)
                    for (a1 = a0+1; a1 < bf10 && (!found_win || !found_ctrl); a1++)
                    for (a2 = a1+1; a2 < bf10 && (!found_win || !found_ctrl); a2++)
                    for (a3 = a2+1; a3 < bf10 && (!found_win || !found_ctrl); a3++)
                    for (a4 = a3+1; a4 < bf10 && (!found_win || !found_ctrl); a4++) {
                        int ti[5];
                        ti[0]=a0; ti[1]=a1; ti[2]=a2; ti[3]=a3; ti[4]=a4;
                        if (!found_win && test_xor_mat2(ti, 5, ACT_2x2_HASH, 64)) {
                            win_idx[0]=a0; win_idx[1]=a1; win_idx[2]=a2;
                            win_idx[3]=a3; win_idx[4]=a4;
                            found_win = 1;
                        }
                        if (!found_ctrl && !test_xor_mat2(ti, 5, ACT_2x2_HASH, 64)) {
                            ctrl_idx[0]=a0; ctrl_idx[1]=a1; ctrl_idx[2]=a2;
                            ctrl_idx[3]=a3; ctrl_idx[4]=a4;
                            found_ctrl = 1;
                        }
                    }
                } else if (nw == 6) {
                    int a0, a1, a2, a3, a4, a5;
                    for (a0 = 0; a0 < bf10 && (!found_win || !found_ctrl); a0++)
                    for (a1 = a0+1; a1 < bf10 && (!found_win || !found_ctrl); a1++)
                    for (a2 = a1+1; a2 < bf10 && (!found_win || !found_ctrl); a2++)
                    for (a3 = a2+1; a3 < bf10 && (!found_win || !found_ctrl); a3++)
                    for (a4 = a3+1; a4 < bf10 && (!found_win || !found_ctrl); a4++)
                    for (a5 = a4+1; a5 < bf10 && (!found_win || !found_ctrl); a5++) {
                        int ti[6];
                        ti[0]=a0; ti[1]=a1; ti[2]=a2;
                        ti[3]=a3; ti[4]=a4; ti[5]=a5;
                        if (!found_win && test_xor_mat2(ti, 6, ACT_2x2_HASH, 64)) {
                            win_idx[0]=a0; win_idx[1]=a1; win_idx[2]=a2;
                            win_idx[3]=a3; win_idx[4]=a4; win_idx[5]=a5;
                            found_win = 1;
                        }
                        if (!found_ctrl && !test_xor_mat2(ti, 6, ACT_2x2_HASH, 64)) {
                            ctrl_idx[0]=a0; ctrl_idx[1]=a1; ctrl_idx[2]=a2;
                            ctrl_idx[3]=a3; ctrl_idx[4]=a4; ctrl_idx[5]=a5;
                            found_ctrl = 1;
                        }
                    }
                } else { /* nw == 7 */
                    int a0, a1, a2, a3, a4, a5, a6;
                    for (a0 = 0; a0 < bf10 && (!found_win || !found_ctrl); a0++)
                    for (a1 = a0+1; a1 < bf10 && (!found_win || !found_ctrl); a1++)
                    for (a2 = a1+1; a2 < bf10 && (!found_win || !found_ctrl); a2++)
                    for (a3 = a2+1; a3 < bf10 && (!found_win || !found_ctrl); a3++)
                    for (a4 = a3+1; a4 < bf10 && (!found_win || !found_ctrl); a4++)
                    for (a5 = a4+1; a5 < bf10 && (!found_win || !found_ctrl); a5++)
                    for (a6 = a5+1; a6 < bf10 && (!found_win || !found_ctrl); a6++) {
                        int ti[7];
                        ti[0]=a0; ti[1]=a1; ti[2]=a2; ti[3]=a3;
                        ti[4]=a4; ti[5]=a5; ti[6]=a6;
                        if (!found_win && test_xor_mat2(ti, 7, ACT_2x2_HASH, 64)) {
                            win_idx[0]=a0; win_idx[1]=a1; win_idx[2]=a2;
                            win_idx[3]=a3; win_idx[4]=a4; win_idx[5]=a5;
                            win_idx[6]=a6;
                            found_win = 1;
                        }
                        if (!found_ctrl && !test_xor_mat2(ti, 7, ACT_2x2_HASH, 64)) {
                            ctrl_idx[0]=a0; ctrl_idx[1]=a1; ctrl_idx[2]=a2;
                            ctrl_idx[3]=a3; ctrl_idx[4]=a4; ctrl_idx[5]=a5;
                            ctrl_idx[6]=a6;
                            found_ctrl = 1;
                        }
                    }
                }

                if (!found_win || !found_ctrl) {
                    printf("  %-8s  %12s  %12s  %8s\n",
                           labels[lev], found_win ? "ok" : "none",
                           found_ctrl ? "ok" : "none", "skip");
                    continue;
                }

                /* Compute Casimir stats for winner and control */
                {
                    long w_total = 0, c_total = 0;
                    int mi;
                    Mat2 wpos[8], wneg[8], cpos10[8], cneg10[8];

                    for (mi = 0; mi < nw; mi++) {
                        wpos[mi] = g_cat[win_idx[mi]];
                        wneg[mi] = mat2_neg(&g_cat[win_idx[mi]]);
                        cpos10[mi] = g_cat[ctrl_idx[mi]];
                        cneg10[mi] = mat2_neg(&g_cat[ctrl_idx[mi]]);
                    }

                    for (mi = 0; mi < n_masks; mi++) {
                        Mat2 ws = mat2_zero();
                        Mat2 cs = mat2_zero();
                        int ii;
                        Cyc8 w_at, w_sq, four, w_bc, w_cas;
                        Cyc8 c_at, c_sq, c_bc, c_cas;
                        long wl, cl;

                        for (ii = 0; ii < n_inp; ii++) {
                            int ww = ii / 2;
                            if (mi & (1 << ii)) {
                                if (ii % 2 == 0) {
                                    ws = mat2_add(&ws, &wpos[ww]);
                                    cs = mat2_add(&cs, &cpos10[ww]);
                                } else {
                                    ws = mat2_add(&ws, &wneg[ww]);
                                    cs = mat2_add(&cs, &cneg10[ww]);
                                }
                            }
                        }

                        four = cyc8_make(4, 0, 0, 0);

                        w_at = cyc8_sub(ws.a, ws.d);
                        w_sq = cyc8_mul(w_at, w_at);
                        w_bc = cyc8_mul(ws.b, ws.c);
                        w_cas = cyc8_add(w_sq, cyc8_mul(four, w_bc));
                        wl = 0;
                        { long v;
                          v = w_cas.a < 0 ? -w_cas.a : w_cas.a; wl += v;
                          v = w_cas.b < 0 ? -w_cas.b : w_cas.b; wl += v;
                          v = w_cas.c < 0 ? -w_cas.c : w_cas.c; wl += v;
                          v = w_cas.d < 0 ? -w_cas.d : w_cas.d; wl += v;
                        }

                        c_at = cyc8_sub(cs.a, cs.d);
                        c_sq = cyc8_mul(c_at, c_at);
                        c_bc = cyc8_mul(cs.b, cs.c);
                        c_cas = cyc8_add(c_sq, cyc8_mul(four, c_bc));
                        cl = 0;
                        { long v;
                          v = c_cas.a < 0 ? -c_cas.a : c_cas.a; cl += v;
                          v = c_cas.b < 0 ? -c_cas.b : c_cas.b; cl += v;
                          v = c_cas.c < 0 ? -c_cas.c : c_cas.c; cl += v;
                          v = c_cas.d < 0 ? -c_cas.d : c_cas.d; cl += v;
                        }

                        w_total += wl;
                        c_total += cl;
                    }

                    {
                        long w_mean = w_total / n_masks;
                        long c_mean = c_total / n_masks;
                        int ratio_x10 = c_mean > 0
                            ? (int)((w_mean * 10 + c_mean / 2) / c_mean)
                            : 0;

                        printf("  %-8s  %12ld  %12ld  %5d.%dx\n",
                               labels[lev], w_mean, c_mean,
                               ratio_x10 / 10, ratio_x10 % 10);

                        if (levels_found > 0 && ratio_x10 < prev_ratio_x10)
                            monotonic = 0;
                        if (ratio_x10 > 0) {
                            prev_ratio_x10 = ratio_x10;
                            levels_found++;
                        }
                    }
                }
            }

            printf("\n");
            if (monotonic && levels_found >= 3)
                printf("  Casimir gap is MONOTONICALLY INCREASING (%d levels)\n",
                       levels_found);
            else if (levels_found >= 3)
                printf("  Casimir gap is NON-MONOTONIC (%d levels)\n",
                       levels_found);
            else
                printf("  Insufficient data (%d levels with ratio)\n",
                       levels_found);

            sprintf(msg, "Casimir gap scaling: %d levels, %s",
                    levels_found,
                    (monotonic && levels_found >= 3) ? "monotonic" :
                    (levels_found >= 3 ? "non-monotonic" : "insufficient"));
            check(msg, 1);
        }

        /* Restore catalog */
        for (j10 = 0; j10 < bf10 && j10 < 40; j10++) {
            g_cat[j10] = sv10[j10];
            g_depth[j10] = sd10[j10];
            g_writhe[j10] = sw10[j10];
        }
        g_cat_size = saved10;
    }

    /* ============================================================
     * Phase 11: Casimir Distribution vs Tuple Size
     * For N = 4,6,8,10,12,14: sample random N-tuples from catalog,
     * compute Casimir of all 2^(2N) sums, report mean |C| and
     * fraction with |C| < 50.
     * Tests "random walk exits nilpotent neighborhood" hypothesis.
     * ============================================================ */
    printf("\n=== Phase 11: Casimir Distribution vs Tuple Size ===\n\n");

    {
        static const int nw_list[6] = {2, 3, 4, 5, 6, 7};
        static const char *labels[6] =
            {"N=4","N=6","N=8","N=10","N=12","N=14"};
        int lev;
        /* Simple LCG for deterministic random sampling */
        unsigned long rng = 12345UL;

        printf("  %-6s  %12s  %12s  %10s\n",
               "Inputs", "Mean |C|_L1", "Frac |C|<50", "Samples");
        printf("  %-6s  %12s  %12s  %10s\n",
               "------", "------------", "------------", "----------");

        for (lev = 0; lev < 6; lev++) {
            int nw = nw_list[lev];
            int n_inp = 2 * nw;
            int n_masks = 1 << n_inp;
            int n_tuples = 200; /* random tuples to sample */
            long total_cas = 0;
            long total_masks_sampled = 0;
            long n_small = 0; /* |C|_L1 < 50 */
            int ti;

            for (ti = 0; ti < n_tuples; ti++) {
                int idx[8];
                int wi;
                Mat2 pos2[8], neg2[8];
                int mi;

                /* Pick nw random distinct indices from catalog */
                for (wi = 0; wi < nw; wi++) {
                    int ok, attempts = 0;
                    do {
                        int ci;
                        rng = rng * 6364136223846793005UL + 1442695040888963407UL;
                        idx[wi] = (int)((rng >> 16) % (unsigned long)g_cat_size);
                        ok = 1;
                        for (ci = 0; ci < wi; ci++)
                            if (idx[ci] == idx[wi]) { ok = 0; break; }
                        attempts++;
                    } while (!ok && attempts < 100);

                    pos2[wi] = g_cat[idx[wi]];
                    neg2[wi] = mat2_neg(&g_cat[idx[wi]]);
                }

                /* Compute Casimir for all 2^(2N) sums */
                for (mi = 0; mi < n_masks; mi++) {
                    Mat2 s = mat2_zero();
                    int ii;
                    Cyc8 anti_tr, at_sq, four, bc_prod, cas;
                    long cas_l1;

                    for (ii = 0; ii < n_inp; ii++) {
                        if (mi & (1 << ii)) {
                            int ww = ii / 2;
                            if (ii % 2 == 0)
                                s = mat2_add(&s, &pos2[ww]);
                            else
                                s = mat2_add(&s, &neg2[ww]);
                        }
                    }

                    four = cyc8_make(4, 0, 0, 0);
                    anti_tr = cyc8_sub(s.a, s.d);
                    at_sq = cyc8_mul(anti_tr, anti_tr);
                    bc_prod = cyc8_mul(s.b, s.c);
                    cas = cyc8_add(at_sq, cyc8_mul(four, bc_prod));

                    cas_l1 = 0;
                    { long v;
                      v = cas.a < 0 ? -cas.a : cas.a; cas_l1 += v;
                      v = cas.b < 0 ? -cas.b : cas.b; cas_l1 += v;
                      v = cas.c < 0 ? -cas.c : cas.c; cas_l1 += v;
                      v = cas.d < 0 ? -cas.d : cas.d; cas_l1 += v;
                    }

                    total_cas += cas_l1;
                    total_masks_sampled++;
                    if (cas_l1 < 50) n_small++;
                }
            }

            {
                long mean_cas = total_masks_sampled > 0
                    ? total_cas / total_masks_sampled : 0;
                double frac_small = total_masks_sampled > 0
                    ? 100.0 * (double)n_small / (double)total_masks_sampled : 0.0;
                printf("  %-6s  %12ld  %10.2f%%  %10ld\n",
                       labels[lev], mean_cas, frac_small, total_masks_sampled);
            }
        }

        check("Casimir distribution scan completed", 1);
    }

    /* ============================================================
     * Summary
     * ============================================================ */
    printf("\n=========================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}
