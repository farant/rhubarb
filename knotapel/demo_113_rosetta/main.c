/*
 * KNOTAPEL DEMO 113: Rosetta -- the delta=0 bracket is the determinant
 * ====================================================================
 *
 * QUESTION: what IS the set of bracket values the DKC demos use as
 * weights? At A = e^{i5pi/4} (delta = 0) every value demo 29 printed was
 * a single integer times a single 8th root of unity, and demo 35's
 * magnitudes (Hopf 2, trefoil 3, figure-eight 5) are knot determinants.
 *
 * CLASSICAL BACKGROUND (Kauffman 1987, Jones 1985):
 *   <L> = (-A^3)^w * V_L(t),  t = A^{-4}
 * At A = e^{i5pi/4}: t = A^{-4} = -1 and t^{1/2} = A^{-2} = -i.
 *   |V_L(-1)| = |Delta_L(-1)| = det(L)        (the knot determinant)
 *   V_L lies in t^{(c-1)/2} Z[t, t^{-1}]       (c = number of components)
 * so V_L(-1) is a REAL integer when c is odd and an IMAGINARY one when c
 * is even. Nothing here is new mathematics; what is tested is whether the
 * project's own bracket code obeys it, braid by braid, so that the DKC
 * weight set can be read in classical terms.
 *
 * PREDICTIONS (written before the first run, 2026-10-03):
 *   P1  Every bracket value at delta=0 is a monomial n * zeta_8^k.
 *   P2  |n| = det(L), with det computed INDEPENDENTLY of the state sum:
 *       reduced Burau representation, det(I - B(t)) / (1+t+...+t^{n-1})
 *       = Delta_L(t) up to a unit, evaluated at t = -1.
 *   P3  bracket = zeta_8^{3w*s} * u * det with u = +-1 for odd c and
 *       u = +-i for even c, for ONE writhe convention s in {+1,-1}
 *       (+1 is the textbook (-A^3)^w; the knotapel crossing convention is
 *       mirrored, docs/knotapel.html note 2, so -1 is expected).
 *   P4  bracket = 0 exactly when det = 0 (split links and their kin).
 *   P5  Hence a catalog's distinct values are {phase * det}: at most 8
 *       phases per determinant.
 *   P6  All of the above at 2, 3 AND 4 strands (the verification
 *       hierarchy: one size is a hypothesis, several are a result).
 *
 * NEGATIVE CONTROLS (must FAIL to match, so the tests can fail):
 *   N1  |Delta(+1)| in place of |Delta(-1)| (Delta(1) = +-1 for knots).
 *   N2  the wrong writhe convention (whichever of s = +-1 loses P3).
 *
 * Exact arithmetic only: Z[zeta_8] for brackets, Z[t, t^{-1}] for Burau.
 * No floating point anywhere.
 *
 * Catalogs (complete enumeration, no caps):
 *   2 strands: every word of length 0..12
 *   3 strands: every word of length 0..8
 *   4 strands: every word of length 0..6
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

static void check(const char *msg, int ok)
{
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * Part A: Exact arithmetic
 * ================================================================ */

/* Z[zeta_8]: a + b z + c z^2 + d z^3, z^4 = -1 (from Demo 29) */
typedef struct { long a, b, c, d; } Cyc8;

static Cyc8 cyc8_make(long a, long b, long c, long d)
{
    Cyc8 z;
    z.a = a; z.b = b; z.c = c; z.d = d;
    return z;
}

static Cyc8 cyc8_add(Cyc8 x, Cyc8 y)
{
    return cyc8_make(x.a + y.a, x.b + y.b, x.c + y.c, x.d + y.d);
}

static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y)
{
    return cyc8_make(
        x.a * y.a - x.b * y.d - x.c * y.c - x.d * y.b,
        x.a * y.b + x.b * y.a - x.c * y.d - x.d * y.c,
        x.a * y.c + x.b * y.b + x.c * y.a - x.d * y.d,
        x.a * y.d + x.b * y.c + x.c * y.b + x.d * y.a);
}

static int cyc8_is_zero(Cyc8 x)
{
    return x.a == 0 && x.b == 0 && x.c == 0 && x.d == 0;
}

/* zeta_8^k for any integer k */
static Cyc8 zeta_pow(int k)
{
    int m = ((k % 8) + 8) % 8;
    long sgn = (m >= 4) ? -1L : 1L;
    if (m >= 4) m -= 4;
    if (m == 0) return cyc8_make(sgn, 0, 0, 0);
    if (m == 1) return cyc8_make(0, sgn, 0, 0);
    if (m == 2) return cyc8_make(0, 0, sgn, 0);
    return cyc8_make(0, 0, 0, sgn);
}

/* If x = n * zeta^k (a single nonzero coordinate), store |n| and the
 * phase index k in 0..7 (k = j for positive coefficient on zeta^j,
 * j + 4 for negative) and return 1. Zero returns 1 with n = 0, k = 0. */
static int cyc8_monomial(Cyc8 x, long *n, int *k)
{
    long v[4];
    int j, nz = 0, at = 0;
    v[0] = x.a; v[1] = x.b; v[2] = x.c; v[3] = x.d;
    for (j = 0; j < 4; j++)
        if (v[j] != 0) { nz++; at = j; }
    if (nz == 0) { *n = 0; *k = 0; return 1; }
    if (nz > 1) return 0;
    if (v[at] > 0) { *n = v[at]; *k = at; }
    else           { *n = -v[at]; *k = at + 4; }
    return 1;
}

/* Laurent polynomials in t with integer coefficients */
#define MAXP 80

typedef struct { long c[MAXP]; int lo, len; } LP;

static void lp_zero(LP *p) { memset(p, 0, sizeof(LP)); }

static void lp_mono(LP *p, long coeff, int e)
{
    lp_zero(p);
    if (coeff == 0) return;
    p->lo = e; p->len = 1; p->c[0] = coeff;
}

static void lp_trim(LP *p)
{
    int a = 0, b;
    if (p->len == 0) return;
    while (a < p->len && p->c[a] == 0) a++;
    if (a == p->len) { lp_zero(p); return; }
    b = p->len - 1;
    while (b > a && p->c[b] == 0) b--;
    if (a > 0) memmove(p->c, p->c + a, (size_t)(b - a + 1) * sizeof(long));
    p->lo += a;
    p->len = b - a + 1;
}

static void lp_addsub(LP *r, const LP *x, const LP *y, long sy)
{
    LP t;
    int lo, hi, i;
    if (x->len == 0 && y->len == 0) { lp_zero(r); return; }
    if (x->len == 0) lo = y->lo;
    else if (y->len == 0) lo = x->lo;
    else lo = x->lo < y->lo ? x->lo : y->lo;
    hi = lo;
    if (x->len && x->lo + x->len - 1 > hi) hi = x->lo + x->len - 1;
    if (y->len && y->lo + y->len - 1 > hi) hi = y->lo + y->len - 1;
    if (hi - lo + 1 > MAXP) {
        fprintf(stderr, "lp_addsub: degree overflow\n");
        exit(2);
    }
    lp_zero(&t);
    t.lo = lo;
    t.len = hi - lo + 1;
    for (i = 0; i < x->len; i++) t.c[x->lo + i - lo] += x->c[i];
    for (i = 0; i < y->len; i++) t.c[y->lo + i - lo] += sy * y->c[i];
    lp_trim(&t);
    *r = t;
}

static void lp_mul(LP *r, const LP *x, const LP *y)
{
    LP t;
    int i, j;
    if (x->len == 0 || y->len == 0) { lp_zero(r); return; }
    if (x->len + y->len - 1 > MAXP) {
        fprintf(stderr, "lp_mul: degree overflow\n");
        exit(2);
    }
    lp_zero(&t);
    t.lo = x->lo + y->lo;
    t.len = x->len + y->len - 1;
    for (i = 0; i < x->len; i++)
        for (j = 0; j < y->len; j++)
            t.c[i + j] += x->c[i] * y->c[j];
    lp_trim(&t);
    *r = t;
}

/* Divide by S_n(t) = 1 + t + ... + t^{n-1}; returns 1 if exact. */
static int lp_div_sn(LP *q, const LP *x, int n)
{
    long w[MAXP];
    int i, j, qlen;
    if (x->len == 0) { lp_zero(q); return 1; }
    if (n == 1) { *q = *x; return 1; }
    if (x->len < n) return 0;
    memcpy(w, x->c, (size_t)x->len * sizeof(long));
    qlen = x->len - (n - 1);
    lp_zero(q);
    q->lo = x->lo;
    q->len = qlen;
    for (i = x->len - 1; i >= n - 1; i--) {
        long lead = w[i];
        q->c[i - (n - 1)] = lead;
        for (j = 0; j < n; j++) w[i - j] -= lead;
    }
    for (i = 0; i < n - 1; i++)
        if (w[i] != 0) return 0;
    lp_trim(q);
    return 1;
}

static long lp_eval_pm1(const LP *p, int sign)
{
    long s = 0;
    int i;
    for (i = 0; i < p->len; i++) {
        int e = p->lo + i;
        long term = p->c[i];
        if (sign < 0 && (e % 2 != 0)) term = -term;
        s += term;
    }
    return s;
}

/* ================================================================
 * Part B: Braids, the state sum at delta=0, Burau
 * ================================================================ */

#define MAX_WORD 16
#define MAX_N 4

typedef struct { int word[MAX_WORD]; int len, n; } Braid;

#define MAX_UF 128
static int uf_p[MAX_UF];

static void uf_init(int n)
{
    int i;
    for (i = 0; i < n; i++) uf_p[i] = i;
}

static int uf_find(int x)
{
    while (uf_p[x] != x) { uf_p[x] = uf_p[uf_p[x]]; x = uf_p[x]; }
    return x;
}

static void uf_union(int x, int y)
{
    x = uf_find(x); y = uf_find(y);
    if (x != y) uf_p[x] = y;
}

/* Loops of a resolved braid closure (Demo 29's convention, unchanged). */
static int braid_loops(const Braid *b, unsigned s)
{
    int N = (b->len + 1) * b->n, l, p, i, loops, sgn, bit, cup;
    uf_init(N);
    for (l = 0; l < b->len; l++) {
        sgn = b->word[l] > 0 ? 1 : -1;
        i = (sgn > 0 ? b->word[l] : -b->word[l]) - 1;
        bit = (int)((s >> l) & 1u);
        cup = (sgn > 0) ? (bit == 0) : (bit == 1);
        if (cup) {
            uf_union(l * b->n + i, l * b->n + i + 1);
            uf_union((l + 1) * b->n + i, (l + 1) * b->n + i + 1);
            for (p = 0; p < b->n; p++)
                if (p != i && p != i + 1)
                    uf_union(l * b->n + p, (l + 1) * b->n + p);
        } else {
            for (p = 0; p < b->n; p++)
                uf_union(l * b->n + p, (l + 1) * b->n + p);
        }
    }
    for (p = 0; p < b->n; p++)
        uf_union(p, b->len * b->n + p);
    loops = 0;
    for (i = 0; i < N; i++)
        if (uf_find(i) == i) loops++;
    return loops;
}

/* Exact bracket at A = e^{i5pi/4} = zeta^5: only one-loop states
 * survive (delta = 0), each contributing A^{#A - #B}. */
static Cyc8 bracket_delta0(const Braid *b)
{
    unsigned s, ns;
    int i, a_count, b_count;
    Cyc8 result = cyc8_make(0, 0, 0, 0);
    if (b->len == 0)
        return (b->n == 1) ? cyc8_make(1, 0, 0, 0) : result;
    ns = 1u << b->len;
    for (s = 0; s < ns; s++) {
        a_count = 0; b_count = 0;
        for (i = 0; i < b->len; i++) {
            if ((s >> (unsigned)i) & 1u) b_count++;
            else a_count++;
        }
        if (braid_loops(b, s) != 1) continue;
        result = cyc8_add(result, zeta_pow(5 * (a_count - b_count)));
    }
    return result;
}

static int braid_writhe(const Braid *b)
{
    int i, w = 0;
    for (i = 0; i < b->len; i++) w += b->word[i] > 0 ? 1 : -1;
    return w;
}

/* Components of the closure = cycles of the braid permutation. */
static int braid_components(const Braid *b)
{
    int pos[MAX_N], perm[MAX_N], seen[MAX_N];
    int i, c = 0, x, t;
    for (i = 0; i < b->n; i++) pos[i] = i;
    for (i = 0; i < b->len; i++) {
        int g = (b->word[i] > 0 ? b->word[i] : -b->word[i]) - 1;
        t = pos[g]; pos[g] = pos[g + 1]; pos[g + 1] = t;
    }
    for (i = 0; i < b->n; i++) perm[pos[i]] = i;
    memset(seen, 0, sizeof(seen));
    for (i = 0; i < b->n; i++) {
        if (seen[i]) continue;
        c++;
        x = i;
        while (!seen[x]) { seen[x] = 1; x = perm[x]; }
    }
    return c;
}

/* Reduced Burau matrices, (n-1)x(n-1), entries in Z[t, t^{-1}]. */
typedef struct { LP m[MAX_N - 1][MAX_N - 1]; int d; } LMat;

static LMat burau_gen[MAX_N + 1][2 * MAX_N]; /* [n][slot] */

static void lmat_identity(LMat *M, int d)
{
    int i, j;
    M->d = d;
    for (i = 0; i < d; i++)
        for (j = 0; j < d; j++) {
            if (i == j) lp_mono(&M->m[i][j], 1, 0);
            else lp_zero(&M->m[i][j]);
        }
}

static void lmat_mul(LMat *R, const LMat *X, const LMat *Y)
{
    LMat T;
    LP prod;
    int i, j, k;
    T.d = X->d;
    for (i = 0; i < X->d; i++)
        for (j = 0; j < X->d; j++) {
            lp_zero(&T.m[i][j]);
            for (k = 0; k < X->d; k++) {
                lp_mul(&prod, &X->m[i][k], &Y->m[k][j]);
                lp_addsub(&T.m[i][j], &T.m[i][j], &prod, 1);
            }
        }
    *R = T;
}

static void lmat_det(LP *r, const LMat *M)
{
    LP t1, t2, t3, acc;
    if (M->d == 1) { *r = M->m[0][0]; return; }
    if (M->d == 2) {
        lp_mul(&t1, &M->m[0][0], &M->m[1][1]);
        lp_mul(&t2, &M->m[0][1], &M->m[1][0]);
        lp_addsub(r, &t1, &t2, -1);
        return;
    }
    /* d == 3: cofactor expansion along the first row */
    lp_zero(&acc);
    lp_mul(&t1, &M->m[1][1], &M->m[2][2]);
    lp_mul(&t2, &M->m[1][2], &M->m[2][1]);
    lp_addsub(&t3, &t1, &t2, -1);
    lp_mul(&t1, &M->m[0][0], &t3);
    lp_addsub(&acc, &acc, &t1, 1);
    lp_mul(&t1, &M->m[1][0], &M->m[2][2]);
    lp_mul(&t2, &M->m[1][2], &M->m[2][0]);
    lp_addsub(&t3, &t1, &t2, -1);
    lp_mul(&t1, &M->m[0][1], &t3);
    lp_addsub(&acc, &acc, &t1, -1);
    lp_mul(&t1, &M->m[1][0], &M->m[2][1]);
    lp_mul(&t2, &M->m[1][1], &M->m[2][0]);
    lp_addsub(&t3, &t1, &t2, -1);
    lp_mul(&t1, &M->m[0][2], &t3);
    lp_addsub(&acc, &acc, &t1, 1);
    *r = acc;
}

/* slot 2*(i-1) = sigma_i, slot 2*(i-1)+1 = sigma_i^{-1} (i 1-based) */
static void burau_build(int n)
{
    int d = n - 1, i, s;
    for (i = 1; i <= n - 1; i++) {
        for (s = 0; s < 2; s++) {
            LMat *G = &burau_gen[n][2 * (i - 1) + s];
            int r0 = i - 2; /* top-left of the 3x3 block (may be -1) */
            lmat_identity(G, d);
            if (d == 1) {
                /* n = 2: sigma -> [-t], sigma^{-1} -> [-t^{-1}] */
                lp_mono(&G->m[0][0], -1, s == 0 ? 1 : -1);
                continue;
            }
            /* Block [[1, t, 0], [0, -t, 0], [0, 1, 1]] at rows/cols
             * r0..r0+2, clipped to the matrix (sigma_1, sigma_{n-1}).
             * Inverse block [[1, 1, 0], [0, -t^{-1}, 0], [0, t^{-1}, 1]]. */
            {
                long blk_c[3][3];
                int blk_e[3][3];
                int a, b;
                for (a = 0; a < 3; a++)
                    for (b = 0; b < 3; b++) {
                        blk_c[a][b] = (a == b) ? 1L : 0L;
                        blk_e[a][b] = 0;
                    }
                if (s == 0) {
                    blk_c[0][1] = 1; blk_e[0][1] = 1;
                    blk_c[1][1] = -1; blk_e[1][1] = 1;
                    blk_c[2][1] = 1; blk_e[2][1] = 0;
                } else {
                    blk_c[0][1] = 1; blk_e[0][1] = 0;
                    blk_c[1][1] = -1; blk_e[1][1] = -1;
                    blk_c[2][1] = 1; blk_e[2][1] = -1;
                }
                for (a = 0; a < 3; a++)
                    for (b = 0; b < 3; b++) {
                        int ra = r0 + a, cb = r0 + b;
                        if (ra < 0 || ra >= d || cb < 0 || cb >= d) continue;
                        lp_mono(&G->m[ra][cb], blk_c[a][b], blk_e[a][b]);
                    }
            }
        }
    }
}

/* det(L) = |Delta(sign)|, Delta = det(I - B) / S_n. Returns -1 if the
 * division is not exact (would falsify the Burau computation itself). */
static long burau_det_at(const Braid *b, int sign)
{
    LMat P, M;
    LP dt, q;
    int i, j;
    int d = b->n - 1;
    lmat_identity(&P, d);
    for (i = 0; i < b->len; i++) {
        int g = b->word[i] > 0 ? b->word[i] : -b->word[i];
        int slot = 2 * (g - 1) + (b->word[i] > 0 ? 0 : 1);
        lmat_mul(&P, &P, &burau_gen[b->n][slot]);
    }
    M.d = d;
    for (i = 0; i < d; i++)
        for (j = 0; j < d; j++) {
            LP id;
            if (i == j) lp_mono(&id, 1, 0); else lp_zero(&id);
            lp_addsub(&M.m[i][j], &id, &P.m[i][j], -1);
        }
    lmat_det(&dt, &M);
    if (!lp_div_sn(&q, &dt, b->n)) return -1;
    {
        long v = lp_eval_pm1(&q, sign);
        return v < 0 ? -v : v;
    }
}

/* ================================================================
 * Part C-F: one catalog = all words on n strands, length 0..maxlen
 * ================================================================ */

typedef struct {
    long braids, monomial, det_match, zero_iff, burau_bad;
    long phase_ok[2];     /* [0]: s = +1, [1]: s = -1 */
    long n1_match;        /* negative control: |Delta(+1)| */
    long max_det;
} Tally;

/* distinct (det, phase) pairs for the demo-29 comparison */
#define MAX_DET 512
static unsigned char seen_phase[MAX_DET][8];

static void tally_braid(const Braid *b, Tally *t, int record)
{
    Cyc8 v = bracket_delta0(b);
    long n_abs, det, det1;
    int k, w, c, s;
    t->braids++;
    if (!cyc8_monomial(v, &n_abs, &k)) return;
    t->monomial++;
    det = burau_det_at(b, -1);
    det1 = burau_det_at(b, 1);
    if (det < 0) { t->burau_bad++; return; }
    if (n_abs == det) t->det_match++;
    if (n_abs == det1) t->n1_match++;
    if ((n_abs == 0) == (det == 0)) t->zero_iff++;
    if (det > t->max_det) t->max_det = det;
    w = braid_writhe(b);
    c = braid_components(b);
    for (s = 0; s < 2; s++) {
        int sg = s == 0 ? 1 : -1;
        Cyc8 r = cyc8_mul(v, zeta_pow(-3 * w * sg));
        long rn;
        int rk;
        if (!cyc8_monomial(r, &rn, &rk)) continue;
        if (rn != det) continue;
        if (det == 0) { t->phase_ok[s]++; continue; }
        /* odd c: rk in {0 (+1), 4 (-1)}; even c: rk in {2 (+i), 6 (-i)} */
        if ((c % 2 == 1) && (rk == 0 || rk == 4)) t->phase_ok[s]++;
        if ((c % 2 == 0) && (rk == 2 || rk == 6)) t->phase_ok[s]++;
    }
    if (record && n_abs > 0 && n_abs < MAX_DET) seen_phase[n_abs][k] = 1;
}

static void enumerate(int n, int maxlen, Tally *t, int record_maxlen)
{
    Braid b;
    int len, ngen = 2 * (n - 1);
    memset(t, 0, sizeof(Tally));
    b.n = n;
    for (len = 0; len <= maxlen; len++) {
        long total = 1, idx;
        int i;
        for (i = 0; i < len; i++) total *= ngen;
        b.len = len;
        for (idx = 0; idx < total; idx++) {
            long x = idx;
            for (i = 0; i < len; i++) {
                int g = (int)(x % ngen);
                x /= ngen;
                b.word[i] = (g / 2 + 1) * (g % 2 == 0 ? 1 : -1);
            }
            tally_braid(&b, t, len >= 1 && len <= record_maxlen);
        }
    }
}

/* ================================================================
 * Known links (classical determinants)
 * ================================================================ */

typedef struct { const char *name; int n, len; int w[MAX_WORD]; long det; } Known;

static const Known known[] = {
    { "unknot (sigma_1)",            2, 1, {1}, 1 },
    { "Hopf link (sigma_1^2)",       2, 2, {1, 1}, 2 },
    { "trefoil (sigma_1^3)",         2, 3, {1, 1, 1}, 3 },
    { "T(2,4) link (sigma_1^4)",     2, 4, {1, 1, 1, 1}, 4 },
    { "cinquefoil (sigma_1^5)",      2, 5, {1, 1, 1, 1, 1}, 5 },
    { "figure-eight",                3, 4, {1, -2, 1, -2}, 5 },
    { "trefoil ((s1 s2)^2)",         3, 4, {1, 2, 1, 2}, 3 },
    { "Hopf (s1 s2 s1, demo 2's)",   3, 3, {1, 2, 1}, 2 },
    { "granny (s1^3 s2^3)",          3, 6, {1, 1, 1, 2, 2, 2}, 9 },
    { "square (s1^3 s2^-3)",         3, 6, {1, 1, 1, -2, -2, -2}, 9 },
    { "3-unlink (identity)",         3, 0, {0}, 0 },
    { "unknot (s1 s2 s3)",           4, 3, {1, 2, 3}, 1 },
    { "T(2,5) on 3 (s1^5 s2)",       3, 6, {1, 1, 1, 1, 1, 2}, 5 }
};

int main(void)
{
    Tally t2, t3, t4;
    int i, n;
    char msg[256];

    printf("KNOTAPEL DEMO 113: Rosetta -- the delta=0 bracket is the determinant\n");
    printf("=====================================================================\n");

    for (n = 2; n <= MAX_N; n++) burau_build(n);

    printf("\n=== PART A: Arithmetic sanity ===\n");
    {
        Cyc8 A = zeta_pow(5);
        Cyc8 A2 = cyc8_mul(A, A);
        Cyc8 Am2 = zeta_pow(-10);
        check("A = zeta^5 = (0,-1,0,0)", A.a == 0 && A.b == -1 && A.c == 0 && A.d == 0);
        check("delta = -(A^2 + A^-2) = 0", cyc8_is_zero(cyc8_add(A2, Am2)));
    }
    {
        LP x, q;
        lp_zero(&x); x.lo = 0; x.len = 4; x.c[0] = 1; x.c[3] = 1; /* 1 + t^3 */
        check("(1 + t^3) / (1 + t) is exact", lp_div_sn(&q, &x, 2));
        check("... = 1 - t + t^2, which is 3 at t = -1", lp_eval_pm1(&q, -1) == 3);
    }

    printf("\n=== PART B: Known links (classical determinants) ===\n");
    printf("  %-28s %8s %6s %6s %3s %3s  %s\n", "link", "bracket", "|n|", "det",
           "w", "c", "Burau det");
    for (i = 0; i < (int)(sizeof(known) / sizeof(known[0])); i++) {
        Braid b;
        Cyc8 v;
        long nb, det;
        int k, j, mono;
        b.n = known[i].n;
        b.len = known[i].len;
        for (j = 0; j < b.len; j++) b.word[j] = known[i].w[j];
        v = bracket_delta0(&b);
        mono = cyc8_monomial(v, &nb, &k);
        det = burau_det_at(&b, -1);
        printf("  %-28s %2ld*z^%d %6ld %6ld %3d %3d  %ld\n", known[i].name,
               mono ? nb : -1L, k, nb, known[i].det, braid_writhe(&b),
               braid_components(&b), det);
        sprintf(msg, "%s: |bracket| = %ld and Burau det = %ld", known[i].name,
                known[i].det, known[i].det);
        check(msg, mono && nb == known[i].det && det == known[i].det);
    }

    printf("\n=== PART C: Complete catalogs ===\n");
    memset(seen_phase, 0, sizeof(seen_phase));
    enumerate(2, 12, &t2, 8);
    enumerate(3, 8, &t3, 8);
    enumerate(4, 6, &t4, 0);
    printf("  %-9s %9s %9s %9s %9s %9s %9s %8s\n", "strands", "braids", "monomial",
           "|n|=det", "0<->0", "s=+1", "s=-1", "max det");
    {
        Tally *ts[3];
        const char *nm[3];
        ts[0] = &t2; ts[1] = &t3; ts[2] = &t4;
        nm[0] = "2 (<=12)"; nm[1] = "3 (<=8)"; nm[2] = "4 (<=6)";
        for (i = 0; i < 3; i++)
            printf("  %-9s %9ld %9ld %9ld %9ld %9ld %9ld %8ld\n", nm[i], ts[i]->braids,
                   ts[i]->monomial, ts[i]->det_match, ts[i]->zero_iff,
                   ts[i]->phase_ok[0], ts[i]->phase_ok[1], ts[i]->max_det);
        printf("\n");
        for (i = 0; i < 3; i++) {
            Tally *t = ts[i];
            sprintf(msg, "P1 [%s]: every bracket is n*zeta^k (%ld/%ld)", nm[i],
                    t->monomial, t->braids);
            check(msg, t->monomial == t->braids);
            sprintf(msg, "Burau division exact for every braid [%s] (%ld failures)",
                    nm[i], t->burau_bad);
            check(msg, t->burau_bad == 0);
            sprintf(msg, "P2 [%s]: |bracket| = det for every braid (%ld/%ld)", nm[i],
                    t->det_match, t->braids);
            check(msg, t->det_match == t->braids);
            sprintf(msg, "P4 [%s]: bracket = 0 <=> det = 0 (%ld/%ld)", nm[i],
                    t->zero_iff, t->braids);
            check(msg, t->zero_iff == t->braids);
        }
    }

    printf("\n=== PART D: The phase law (P3) ===\n");
    printf("  bracket * zeta^{-3 w s} should be +-det (odd c) or +-i*det (even c)\n");
    {
        int s_win = -1, s;
        long need = t2.braids + t3.braids + t4.braids;
        for (s = 0; s < 2; s++) {
            long got = t2.phase_ok[s] + t3.phase_ok[s] + t4.phase_ok[s];
            printf("  s = %+d: %ld / %ld braids\n", s == 0 ? 1 : -1, got, need);
            if (got == need) s_win = s;
        }
        check("P3: one writhe convention gives the phase law for EVERY braid",
              s_win >= 0);
        if (s_win >= 0)
            printf("  convention: bracket = zeta_8^{%s3w} * u * det\n",
                   s_win == 0 ? "+" : "-");
        printf("\n=== PART E: Negative controls ===\n");
        {
            int s_lose = s_win >= 0 ? 1 - s_win : 0;
            long lose = t2.phase_ok[s_lose] + t3.phase_ok[s_lose] + t4.phase_ok[s_lose];
            long n1 = t2.n1_match + t3.n1_match + t4.n1_match;
            sprintf(msg, "N1: |Delta(+1)| does NOT match the bracket (%ld/%ld match)",
                    n1, need);
            check(msg, n1 < need);
            sprintf(msg, "N2: the other convention does NOT give the law (%ld/%ld)",
                    lose, need);
            check(msg, lose < need);
        }
    }

    printf("\n=== PART F: What the DKC catalog is (P5) ===\n");
    printf("  Demo 29's catalog definition: 2-3 strands, length 1-8, nonzero.\n");
    printf("  Demo 29 stored at most MAX_CATALOG = 8192 braids; here: all.\n\n");
    {
        long distinct = 0, dets = 0, d;
        int k, over = 0;
        printf("  det | phases seen (k: value = det * zeta_8^k)\n");
        for (d = 1; d < MAX_DET; d++) {
            int cnt = 0;
            for (k = 0; k < 8; k++) cnt += seen_phase[d][k];
            if (!cnt) continue;
            dets++;
            distinct += cnt;
            if (cnt > 8) over = 1;
            printf("  %3ld | ", d);
            for (k = 0; k < 8; k++)
                if (seen_phase[d][k]) printf("%d ", k);
            printf("\n");
        }
        printf("\n  distinct nonzero values: %ld (demo 29 reported 64 from its\n", distinct);
        printf("  capped catalog), over %ld distinct determinants\n", dets);
        sprintf(msg, "P5: distinct values = determinant x phase (%ld values, %ld dets)",
                distinct, dets);
        check(msg, !over && distinct > 0);
    }

    printf("\n=====================================================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("=====================================================================\n");
    return n_fail > 0 ? 1 : 0;
}
