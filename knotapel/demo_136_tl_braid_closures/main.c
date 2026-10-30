/*
 * KNOTAPEL DEMO 136: brackets of large braid closures through
 * Temperley-Lieb standard modules
 * ============================================================
 *
 * Plan: plan.md (v2 after a design review before any data; T0 amendment).
 * The Kauffman bracket of the closure of a braid b in B_n is a weighted
 * sum of traces of b in the Temperley-Lieb standard modules W_{n,j}:
 *
 *   <closure(b)>_delta = sum_j Delta_j(delta) tr rho_j(b),
 *   Delta_0 = 1, Delta_1 = delta, Delta_{j+1} = delta Delta_j - Delta_{j-1},
 *   <closure(b)> = <closure(b)>_delta / delta          (<O> = 1),
 *   sigma_i^{+-1} -> A^{+-1} 1 + A^{-+1} e_i,  delta = -A^2 - A^-2 (conv0).
 *
 * The modules' dimensions square-sum to Catalan(n) (208,012 at n = 12)
 * instead of the vertex model's ~2^(2n) states. Checked here (plan.md):
 *
 *   R0   the test braids are the projection run's braids (bound, width)
 *   R1   TL bracket == state sum (c <= 24) and == vertex model (full)
 *   R1p  TL value == vertex value == reduced exact TL polynomial at
 *        points of a prime p* the interpolation never uses
 *   R2   link-state counts, validity, sum d^2 = Catalan(n), n <= 16
 *   R3   TL and braid relations on every basis state, n <= 16
 *   R4   V(1) = 1, V(e^(2 pi i/3)) = 1, V(i) = +-1, |V(-1)| = det
 *   R5   stabilization (back, front), index flip, conjugation, at points
 *   R6   speed per point against the vertex model (12 x 401, 12 x 801)
 *   plus conv0 (the trefoil against the state sum at points; the
 *   one-strand trivial braid against 1).
 *
 * House libraries: piscina, sors, magnus, polynomium, interpolatio,
 * congruentia, laqueus (and closure), hence latina.h: Roman numerals and
 * Latin keywords are macros here (never name a variable I, V, X, L, C, D
 * or M, or per, si, dum, ...). House i32/i64 are UNSIGNED; s32 is int.
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_136_tl_braid_closures/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * D136_MODUS=t1 runs the small set (T1; prints PARTIAL RUN); =p8 adds
 * 10 x 401 (R1p, R4: plant 8); default = the registered test set (T2);
 * =desc runs the descriptive measurements (no predictions; D136_PARS =
 * times | spans | integers | polynomials picks one). Times print only
 * with D136_TEMPORA=1 (except in desc), so archived output is stable.
 * Run under tools/custos.sh.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "latina.h"
#include "piscina.h"
#include "sors.h"
#include "magnus.h"
#include "polynomium.h"
#include "interpolatio.h"
#include "congruentia.h"
#include "contractio.h"
#include "laqueus.h"

#define NMAX 16
#define WORKSPACE_MAX ((i64)0x80000000UL)   /* 2 GiB, as uncinus_mensura */

typedef i32 res;                            /* residue mod p < 2^32 */
typedef unsigned long long ull;

/* ---------- checks and verdicts ---------- */

enum {
    P_R0, P_R1, P_R1P, P_R2, P_R3, P_R4, P_R5, P_R6, P_CONV, P_NUM
};

static const char *pred_name[P_NUM] = {
    "R0", "R1", "R1p", "R2", "R3", "R4", "R5", "R6", "conv0"
};

static long pred_checks[P_NUM];
static long pred_fails[P_NUM];
static int n_pass = 0;
static int n_fail = 0;

static void
check (
    int         pred,
    const char *msg,
    int         ok)
{
    pred_checks[pred]++;
    if (ok) {
        n_pass++;
        printf("  PASS: %s: %s\n", pred_name[pred], msg);
    } else {
        n_fail++;
        pred_fails[pred]++;
        printf("  FAIL: %s: %s\n", pred_name[pred], msg);
    }
    fflush(stdout);
}

static void
fatal (
    const char *msg)
{
    printf("  FATAL: %s\n", msg);
    exit(1);
}

static double
seconds_since (
    clock_t start)
{
    return (double)(clock() - start) / (double)CLOCKS_PER_SEC;
}

/* ---------- arithmetic mod p (p < 2^32) ---------- */

static res
mulm (res a, res b, res p)
{
    return (res)((ull)a * (ull)b % (ull)p);
}

static res
addm (res a, res b, res p)
{
    ull s = (ull)a + (ull)b;

    return (res)(s >= (ull)p ? s - (ull)p : s);
}

static res
subm (res a, res b, res p)
{
    return a >= b ? a - b : (res)((ull)a + (ull)p - (ull)b);
}

static res
powm (res a, ull e, res p)
{
    ull r = 1, b = (ull)a % (ull)p;

    while (e > 0) {
        if (e & 1ULL) r = r * b % (ull)p;
        b = b * b % (ull)p;
        e >>= 1;
    }
    return (res)r;
}

static res
invm (res a, res p)
{
    return powm(a, (ull)p - 2ULL, p);
}

/* ---------- link states and e_i tables ---------- */

typedef struct {
    int            n, j, d;
    unsigned int  *key;        /* sorted; 2 bits per point: 0 defect, 1 open, 2 close */
    signed char   *partner;    /* d * n; -1 = defect */
    int           *image;      /* (n - 1) * d: image index, -1 = zero */
    unsigned char *tag;        /* (n - 1) * d: 0 zero, 1 one, 2 delta */
} Module;

static Module modules[NMAX + 2][NMAX + 2];
static int modules_built[NMAX + 2];

static signed char cur[NMAX];
static int open_stack[NMAX];
static int fill_count;
static Module *fill_target;

static void
enum_rec (
    int n,
    int j,
    int k,
    int depth,
    int defects)
{
    if (k == n) {
        if (depth == 0 && defects == j) {
            if (fill_target != NULL)
                memcpy(fill_target->partner + (long)fill_count * n, cur, (size_t)n);
            fill_count++;
        }
        return;
    }
    if (depth + 1 <= n - k - 1) {                     /* open an arc */
        open_stack[depth] = k;
        enum_rec(n, j, k + 1, depth + 1, defects);
    }
    if (depth > 0) {                                  /* close the innermost arc */
        int a = open_stack[depth - 1];

        cur[a] = (signed char)k;
        cur[k] = (signed char)a;
        enum_rec(n, j, k + 1, depth - 1, defects);
        open_stack[depth - 1] = a;
    }
    if (depth == 0 && defects < j) {                  /* defect: outside every arc */
        cur[k] = -1;
        enum_rec(n, j, k + 1, depth, defects + 1);
    }
}

static unsigned int
state_key (
    const signed char *s,
    int                n)
{
    unsigned int key = 0;
    int x;

    for (x = 0; x < n; x++) {
        unsigned int code = s[x] < 0 ? 0U : (s[x] > x ? 1U : 2U);

        key |= code << (2 * x);
    }
    return key;
}

static Module *sort_module;

static int
cmp_by_key (
    const void *a,
    const void *b)
{
    unsigned int ka = state_key(sort_module->partner + (long)*(const int *)a * sort_module->n, sort_module->n);
    unsigned int kb = state_key(sort_module->partner + (long)*(const int *)b * sort_module->n, sort_module->n);

    return ka < kb ? -1 : (ka > kb ? 1 : 0);
}

static int
find_state (
    const Module *m,
    unsigned int  key)
{
    int lo = 0, hi = m->d - 1;

    while (lo <= hi) {
        int mid = (lo + hi) / 2;

        if (m->key[mid] == key) return mid;
        if (m->key[mid] < key) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

static void
build_module (
    Module *m,
    int     n,
    int     j)
{
    int *order;
    signed char *sorted;
    signed char tmp[NMAX];
    int s, i;

    m->n = n;
    m->j = j;
    fill_target = NULL;
    fill_count = 0;
    enum_rec(n, j, 0, 0, 0);
    m->d = fill_count;
    m->partner = (signed char *)malloc((size_t)(m->d > 0 ? m->d : 1) * (size_t)n);
    fill_target = m;
    fill_count = 0;
    enum_rec(n, j, 0, 0, 0);
    fill_target = NULL;

    order = (int *)malloc((size_t)(m->d > 0 ? m->d : 1) * sizeof(int));
    for (s = 0; s < m->d; s++) order[s] = s;
    sort_module = m;
    qsort(order, (size_t)m->d, sizeof(int), cmp_by_key);
    sorted = (signed char *)malloc((size_t)(m->d > 0 ? m->d : 1) * (size_t)n);
    for (s = 0; s < m->d; s++)
        memcpy(sorted + (long)s * n, m->partner + (long)order[s] * n, (size_t)n);
    free(m->partner);
    m->partner = sorted;
    free(order);
    m->key = (unsigned int *)malloc((size_t)(m->d > 0 ? m->d : 1) * sizeof(unsigned int));
    for (s = 0; s < m->d; s++) m->key[s] = state_key(m->partner + (long)s * n, n);

    m->image = (int *)malloc((size_t)((n > 1 ? n - 1 : 1) * (m->d > 0 ? m->d : 1)) * sizeof(int));
    m->tag = (unsigned char *)malloc((size_t)((n > 1 ? n - 1 : 1) * (m->d > 0 ? m->d : 1)));
    for (i = 0; i + 1 < n; i++) {
        for (s = 0; s < m->d; s++) {
            int a = i, b = i + 1, pa, pb, img = -1;
            unsigned char tag;

            memcpy(tmp, m->partner + (long)s * n, (size_t)n);
            pa = tmp[a];
            pb = tmp[b];
            if (pa == b) {                            /* (a) */
                tag = 2;
            } else if (pa >= 0 && pb >= 0) {          /* (b) */
                tmp[a] = (signed char)b;
                tmp[b] = (signed char)a;
                tmp[pa] = (signed char)pb;
                tmp[pb] = (signed char)pa;
                tag = 1;
            } else if (pa < 0 && pb < 0) {            /* (d) */
                tag = 0;
            } else {                                  /* (c) */
                int other = pa < 0 ? pb : pa;

                tmp[a] = (signed char)b;
                tmp[b] = (signed char)a;
                tmp[other] = -1;
                tag = 1;   /* case (c) */
            }
            if (tag == 2) img = s;
            else if (tag == 1) {
                img = find_state(m, state_key(tmp, n));
                if (img < 0) fatal("e_i image is not a link state of the module");
            }
            m->image[(long)i * m->d + s] = img;
            m->tag[(long)i * m->d + s] = tag;
        }
    }
}

static void
ensure_modules (
    int n)
{
    int j;

    if (n < 1 || n > NMAX) fatal("strand count out of range");
    if (modules_built[n]) return;
    for (j = n % 2; j <= n; j += 2) build_module(&modules[n][j], n, j);
    modules_built[n] = 1;
}

static long
binom (int n, int k)
{
    long r = 1;
    int t;

    if (k < 0 || k > n) return 0;
    for (t = 1; t <= k; t++) r = r * (long)(n - k + t) / (long)t;
    return r;
}

static long
dim_formula (int n, int j)
{
    if ((n - j) % 2 != 0 || j < 0 || j > n) return 0;
    return binom(n, (n - j) / 2) - binom(n, (n - j) / 2 - 1);
}

/* ---------- the TL value at a point ---------- */

static res *buf_a = NULL, *buf_b = NULL;
static long buf_size = 0;

/* bracket <closure(word)> (<O> = 1) at A mod p; *ok = 0 if delta = 0 */
static res
tl_value (
    int        n,
    const int *word,
    int        len,
    res        A,
    res        p,
    int       *ok)
{
    res Ainv = invm(A, p);
    res delta = subm(0, addm(mulm(A, A, p), mulm(Ainv, Ainv, p), p), p);
    res Dl[NMAX + 3];
    res total = 0, result;
    int j, k;

    *ok = 1;
    if (delta == 0) {
        *ok = 0;
        return 0;
    }
    ensure_modules(n);
    Dl[0] = 1;
    Dl[1] = delta;
    for (j = 1; j + 1 <= n; j++)
        Dl[j + 1] = subm(mulm(delta, Dl[j], p), Dl[j - 1], p);
    for (j = n % 2; j <= n; j += 2) {
        const Module *m = &modules[n][j];
        long d = m->d, e, s;
        res tr = 0;
        res *mat, *nb;

        if (d * d > buf_size) {
            free(buf_a);
            free(buf_b);
            buf_size = d * d;
            buf_a = (res *)malloc((size_t)buf_size * sizeof(res));
            buf_b = (res *)malloc((size_t)buf_size * sizeof(res));
            if (buf_a == NULL || buf_b == NULL) fatal("out of memory (trace buffers)");
        }
        mat = buf_a;
        nb = buf_b;
        for (e = 0; e < d * d; e++) mat[e] = 0;
        for (s = 0; s < d; s++) mat[s * d + s] = 1;
        /* M = rho(w_1) rho(w_2) ... rho(w_len): apply letters last to first */
        for (k = len - 1; k >= 0; k--) {
            int letter = word[k];
            int i = (letter > 0 ? letter : -letter) - 1;
            res c1, c2, *swap;
            const int *img = m->image + (long)i * d;
            const unsigned char *tg = m->tag + (long)i * d;

            if (letter > 0) { c1 = A; c2 = Ainv; } else { c1 = Ainv; c2 = A; }
            for (e = 0; e < d * d; e++) nb[e] = mulm(c1, mat[e], p);
            for (s = 0; s < d; s++) {
                res f;
                res *dst;
                const res *src;
                long c;

                if (tg[s] == 0) continue;
                f = tg[s] == 2 ? mulm(c2, delta, p) : c2;
                dst = nb + (long)img[s] * d;
                src = mat + s * d;
                for (c = 0; c < d; c++)
                    dst[c] = (res)(((ull)dst[c] + (ull)f * (ull)src[c]) % (ull)p);
            }
            swap = mat;
            mat = nb;
            nb = swap;
        }
        for (s = 0; s < d; s++) tr = addm(tr, mat[s * d + s], p);
        total = addm(total, mulm(Dl[j], tr, p), p);
    }
    result = mulm(total, invm(delta, p), p);
    return result;
}

/* ---------- points: the vertex model's choice (A = i omega^2) ---------- */

typedef struct {
    res p;
    res radix_i;
} PrimeCache;

static s32
choose_point (
    PrimeCache *pc,
    res         p,
    res         omega,
    res        *A)
{
    res w, winv, d;

    if (p < 5 || p % 4 != 1) return INTERPOLATIO_ERRATUM;
    if (pc->p != p) {
        res g = 2;

        if (!congruentia_est_primus(p)) return INTERPOLATIO_ERRATUM;
        while (g < p && powm(g, ((ull)p - 1ULL) / 2ULL, p) != p - 1) g++;
        if (g == p) return INTERPOLATIO_ERRATUM;
        pc->radix_i = powm(g, ((ull)p - 1ULL) / 4ULL, p);
        pc->p = p;
    }
    w = omega % p;
    if (w == 0) return INTERPOLATIO_INUTILE;
    winv = invm(w, p);
    d = addm(powm(w, 4, p), powm(winv, 4, p), p);
    if (d == 0) return INTERPOLATIO_INUTILE;
    *A = mulm(pc->radix_i, mulm(w, w, p), p);
    return INTERPOLATIO_UTILE;
}

typedef struct {
    int        n;
    const int *word;
    int        len;
    PrimeCache pc;
    res        min_p;
    long       evaluations;
} TLContext;

static s32
tl_estimator (
    void *contextus,
    i32   index,
    i32   p,
    i32  *punctum,
    i32  *valor)
{
    TLContext *ctx = (TLContext *)contextus;
    res A = 0;
    s32 st = choose_point(&ctx->pc, p, index + 2U, &A);
    int ok;

    if (st != INTERPOLATIO_UTILE) return st;
    if (ctx->min_p == 0 || p < ctx->min_p) ctx->min_p = p;
    *punctum = A;
    *valor = tl_value(ctx->n, ctx->word, ctx->len, A, p, &ok);
    ctx->evaluations++;
    return ok ? INTERPOLATIO_UTILE : INTERPOLATIO_INUTILE;
}

/* ---------- copied from tools/uncinus_mensura.c (interior there) ---------- */

static i32
_radix (
     i32* pater,
     i32  x)
{
    while (pater[x] != x)
    {
        pater[x]  = pater[pater[x]];
        x         = pater[x];
    }
    return x;
}

static s64
_modulo_quattuor (
    s64 x)
{
    s64 r = x % (s64)IV;

    return r < ZEPHYRUM ? r + (s64)IV : r;
}

static s32
_signum (
    const i32* x,
          i32  m)
{
    return x[I] == x[III] % m + I ? (s32)I : -(s32)I;
}

/* fenestra Kauffman in classe 3w, ut laqueus_modulus_vertex_creare */
static void
_fenestra (
    const i32* pd,
          i32  c,
      Piscina* piscina,
          s32* imus,
          s32* summus)
{
    i32  m     = II * c;
    i32* pater = (i32*)piscina_allocare(piscina, (memoriae_index)m
        * sizeof(i32));
    i32 ansae_a  = ZEPHYRUM;
    i32 ansae_b  = ZEPHYRUM;
    s32 w        = ZEPHYRUM;
    s32 classis;
    i32 l;
    i32 k;
    i32 status;

    for (status = ZEPHYRUM; status < II; status++)
    {
        for (l = ZEPHYRUM; l < m; l++)
        {
            pater[l] = l;
        }
        for (k = ZEPHYRUM; k < c; k++)
        {
            const i32* x = pd + IV * k;

            if (status == ZEPHYRUM)
            {
                pater[_radix(pater, x[0] - I)] = _radix(pater, x[I]
                    - I);
                pater[_radix(pater, x[II] - I)] = _radix(pater, x[III]
                    - I);
            }
            else
            {
                pater[_radix(pater, x[0] - I)] = _radix(pater, x[III]
                    - I);
                pater[_radix(pater, x[I] - I)] = _radix(pater, x[II]
                    - I);
            }
        }
        for (l = ZEPHYRUM; l < m; l++)
        {
            if (_radix(pater, l) == l)
            {
                if (status == ZEPHYRUM)
                {
                    ansae_a++;
                }
                else
                {
                    ansae_b++;
                }
            }
        }
    }
    for (k = ZEPHYRUM; k < c; k++)
    {
        w += _signum(pd + IV * k, m);
    }
    classis  = (s32)_modulo_quattuor((s64)III * (s64)w);
    *imus    = -(s32)c - II * (s32)ansae_b + II;
    *imus    += (s32)_modulo_quattuor((s64)classis - (s64)*imus);
    *summus  = (s32)c + II * (s32)ansae_a - II;
    *summus  -= (s32)_modulo_quattuor((s64)*summus - (s64)classis);
}

/* R0 generator: verbatim copy of _plexus_fortuitus, plus one output
 * parameter (the word) so the TL route can read the braid itself. */
static i32*
_plexus_fortuitus (
       Sors* s,
        i32  n,
        i32* longitudo,
    Piscina* piscina,
       s32** verbum_exitus)
{
    s32* verbum;
    i32  conatus;

    if ((*longitudo + n - I) % II != ZEPHYRUM)
    {
        (*longitudo)++;
    }
    verbum = (s32*)piscina_allocare(piscina, (memoriae_index)*longitudo
        * sizeof(s32));
    for (conatus = ZEPHYRUM; conatus < M; conatus++)
    {
        PiscinaNotatio  nota = piscina_notare(piscina);
                   i32* pd;
                   i32  transitus;
                   i32  k;

        for (k = ZEPHYRUM; k < *longitudo; k++)
        {
            s32 g = (s32)(I + sors_intra(s, n - I));

            verbum[k] = sors_intra(s, II) == ZEPHYRUM ? g : -g;
        }
        if (laqueus_pd_ex_plexu(verbum, *longitudo, n, piscina, &pd,
            &transitus))
        {
            *verbum_exitus = verbum;
            return pd;
        }
        piscina_reficere(piscina, nota);
    }
    return NULL;
}

/* ---------- polynomials ---------- */

/* sum c_k A^k mod p */
static res
poly_at (
    Polynomium f,
    res        A,
    res        p)
{
    s32 e, lo, hi;
    res Ainv = invm(A, p), acc = 0;

    if (polynomium_est_nullum(f)) return 0;
    lo = polynomium_gradus_imus(f);
    hi = polynomium_gradus_summus(f);
    for (e = lo; e <= hi; e++) {
        res c = congruentia_ex_magno(polynomium_coefficiens(f, e), p);
        res pw = e >= 0 ? powm(A, (ull)e, p) : powm(Ainv, (ull)(-e), p);

        acc = addm(acc, mulm(c, pw, p), p);
    }
    return acc;
}

static i32
max_coeff_bits (
    Polynomium f)
{
    s32 e;
    i32 b = 0;

    if (polynomium_est_nullum(f)) return 0;
    for (e = polynomium_gradus_imus(f); e <= polynomium_gradus_summus(f); e++) {
        i32 x = magnus_bitorum(polynomium_coefficiens(f, e));

        if (x > b) b = x;
    }
    return b;
}

/* ---------- colouring-matrix determinant (R4, every c) ---------- */

static res
colouring_det_mod (
    const i32 *pd,
    int        c,
    res        p)
{
    int m = 2 * c, k, r, q, n = c - 1, arcs = 0;
    i32 *parent = (i32 *)malloc((size_t)(m + 1) * sizeof(i32));
    int *index = (int *)malloc((size_t)(m + 1) * sizeof(int));
    res *A;
    res det = 1;

    for (k = 0; k <= m; k++) {
        parent[k] = (i32)k;
        index[k] = -1;
    }
    for (k = 0; k < c; k++) {
        i32 x = _radix(parent, pd[4 * k + 1]);
        i32 y = _radix(parent, pd[4 * k + 3]);

        if (x != y) parent[x] = y;
    }
    for (k = 1; k <= m; k++) {
        i32 x = _radix(parent, (i32)k);

        if (index[x] < 0) index[x] = arcs++;
    }
    if (arcs != c) fatal("colouring matrix: arc count != crossing count");
    A = (res *)calloc((size_t)c * (size_t)c, sizeof(res));
    for (k = 0; k < c; k++) {
        int over = index[_radix(parent, pd[4 * k + 1])];
        int ua = index[_radix(parent, pd[4 * k])];
        int ub = index[_radix(parent, pd[4 * k + 2])];
        res *row = A + (long)k * c;

        row[over] = addm(row[over], 2, p);
        row[ua] = subm(row[ua], 1, p);
        row[ub] = subm(row[ub], 1, p);
    }
    for (q = 0; q < n; q++) {
        int pivot = -1;
        res inv;

        for (r = q; r < n; r++)
            if (A[(long)r * c + q] != 0) { pivot = r; break; }
        if (pivot < 0) { det = 0; break; }
        if (pivot != q) {
            for (k = 0; k < n; k++) {
                res x = A[(long)q * c + k];

                A[(long)q * c + k] = A[(long)pivot * c + k];
                A[(long)pivot * c + k] = x;
            }
            det = subm(0, det, p);
        }
        det = mulm(det, A[(long)q * c + q], p);
        inv = invm(A[(long)q * c + q], p);
        for (r = q + 1; r < n; r++) {
            res f = mulm(A[(long)r * c + q], inv, p);

            if (f == 0) continue;
            for (k = q; k < n; k++)
                A[(long)r * c + k] = subm(A[(long)r * c + k], mulm(f, A[(long)q * c + k], p), p);
        }
    }
    free(parent);
    free(index);
    free(A);
    return det;
}

/* ---------- a test braid ---------- */

typedef struct {
    char       name[48];
    int        n;
    int        len;         /* crossings */
    const int *word;
    i32       *pd;
    s32        imus, summus;
    i32        limes;
    LaqueusModulusVertex *mv;
} Braid;

static int word_writhe (const int *word, int len)
{
    int k, w = 0;

    for (k = 0; k < len; k++) w += word[k] > 0 ? 1 : -1;
    return w;
}

static int
prepare_braid (
    Braid     *b,
    const char *name,
    int        n,
    const int *word,
    int        len,
    i32       *pd,
    Piscina   *piscina)
{
    strncpy(b->name, name, sizeof(b->name) - 1);
    b->name[sizeof(b->name) - 1] = 0;
    b->n = n;
    b->len = len;
    b->word = word;
    if (pd == NULL) {
        s32 *w32 = (s32 *)piscina_allocare(piscina, (memoriae_index)len * sizeof(s32));
        i32 c;
        int k;

        for (k = 0; k < len; k++) w32[k] = word[k];
        if (!laqueus_pd_ex_plexu(w32, (i32)len, (i32)n, piscina, &pd, &c)) return 0;
    }
    b->pd = pd;
    _fenestra(pd, (i32)len, piscina, &b->imus, &b->summus);
    b->mv = laqueus_modulus_vertex_creare(pd, (i32)len, WORKSPACE_MAX, piscina);
    b->limes = laqueus_modulus_vertex_limes(b->mv);
    return b->mv != NULL;
}

static int
random_braid (
    Braid   *b,
    int      n,
    int      len_req,
    Piscina *piscina)
{
    Sors s;
    i32 len = (i32)len_req;
    s32 *w32 = NULL;
    i32 *pd;
    int *word;
    int k;
    char name[48];

    sors_seminare(&s, (i64)2028, 0);
    pd = _plexus_fortuitus(&s, (i32)n, &len, piscina, &w32);
    if (pd == NULL) return 0;
    word = (int *)piscina_allocare(piscina, (memoriae_index)len * sizeof(int));
    for (k = 0; k < (int)len; k++) word[k] = w32[k];
    sprintf(name, "plexus%d x %u", n, len);
    return prepare_braid(b, name, n, word, (int)len, pd, piscina);
}

/* full TL bracket by interpolation, settings as _uncinus_per_rete */
static int
tl_bracket (
    const Braid *b,
    Piscina     *piscina,
    Polynomium  *f,
    TLContext   *ctx)
{
    InterpolatioQuaestio q = interpolatio_quaestio_ordinaria();

    memset(ctx, 0, sizeof(*ctx));
    ctx->n = b->n;
    ctx->word = b->word;
    ctx->len = b->len;
    q.gradus_imus = b->imus;
    q.gradus_summus = b->summus;
    q.passus = IV;
    q.limes_bitorum = b->limes;
    q.residuum_primi = I;
    q.modulus_primi = IV;
    q.puncta_addita = II;
    q.conatus_maximi = (i32)((b->summus - b->imus) / IV) + I + II + LXIV;
    return interpolatio_polynomium(tl_estimator, ctx, &q, piscina, f);
}

/* ---------- R4 on an exact bracket ---------- */

static void
check_r4 (
    const Braid *b,
    Polynomium   bracket,
    Piscina     *piscina,
    int          run_alexander)
{
    int w = word_writhe(b->word, b->len);
    long bump = 0;
    Magnus S[4], T[3], v1, vm1, zero = magnus_ex_s64(0);
    s32 e, lo, hi;
    int r, bad_class = 0, first = 1;
    char msg[256];

    for (r = 0; r < 4; r++) S[r] = zero;
    for (r = 0; r < 3; r++) T[r] = zero;
    lo = polynomium_gradus_imus(bracket);
    hi = polynomium_gradus_summus(bracket);
    for (e = lo; e <= hi; e++) {
        Magnus cf = polynomium_coefficiens(bracket, e);
        long ep = (long)e - 3L * (long)w, kk;
        int k4, k3;

        if (magnus_signum(cf) == 0) continue;
        if (ep % 4 != 0) { bad_class = 1; continue; }
        if (w % 2 != 0) cf = magnus_nega(cf, piscina);     /* (-1)^w */
        if (first) {
            cf = magnus_adde(cf, magnus_ex_s64((s64)bump), piscina);
            first = 0;
        }
        kk = -ep / 4;                                       /* t = A^-4 */
        k4 = (int)(((kk % 4) + 4) % 4);
        k3 = (int)(((kk % 3) + 3) % 3);
        S[k4] = magnus_adde(S[k4], cf, piscina);
        T[k3] = magnus_adde(T[k3], cf, piscina);
    }
    sprintf(msg, "%s: Jones exponents integral (A-exponents - 3w in 4Z)", b->name);
    check(P_R4, msg, !bad_class);
    v1 = magnus_adde(magnus_adde(S[0], S[1], piscina), magnus_adde(S[2], S[3], piscina), piscina);
    vm1 = magnus_subtrahe(magnus_adde(S[0], S[2], piscina), magnus_adde(S[1], S[3], piscina), piscina);
    sprintf(msg, "%s: V(1) = 1", b->name);
    check(P_R4, msg, magnus_aequalis(v1, magnus_ex_s64(1)));
    {
        Magnus d = magnus_subtrahe(T[0], T[2], piscina);

        sprintf(msg, "%s: V(e^(2 pi i/3)) = 1 (T1 = T2, T0 - T2 = 1)", b->name);
        check(P_R4, msg, magnus_aequalis(T[1], T[2]) && magnus_aequalis(d, magnus_ex_s64(1)));
    }
    {
        Magnus d = magnus_subtrahe(S[0], S[2], piscina);

        sprintf(msg, "%s: V(i) = +-1 (S1 = S3, S0 - S2 = +-1)", b->name);
        check(P_R4, msg, magnus_aequalis(S[1], S[3])
            && (magnus_aequalis(d, magnus_ex_s64(1)) || magnus_aequalis(d, magnus_ex_s64(-1))));
    }
    /* det, route 1: laqueus Alexander (c <= 401) */
    if (run_alexander && b->len <= 401) {
        Polynomium alex;

        if (laqueus_alexander_ex_pd(b->pd, (i32)b->len, piscina, &alex)) {
            Magnus a = zero;

            for (e = polynomium_gradus_imus(alex); e <= polynomium_gradus_summus(alex); e++) {
                Magnus cf = polynomium_coefficiens(alex, e);

                if (e % 2 != 0) cf = magnus_nega(cf, piscina);
                a = magnus_adde(a, cf, piscina);
            }
            sprintf(msg, "%s: |V(-1)| = |Delta(-1)| (laqueus Alexander, %u bits)", b->name,
                magnus_bitorum(a));
            check(P_R4, msg, magnus_aequalis(magnus_absolutum(a, piscina), magnus_absolutum(vm1, piscina)));
        } else {
            sprintf(msg, "%s: laqueus Alexander refused", b->name);
            check(P_R4, msg, 0);
        }
    }
    /* det, route 2: colouring minor mod primes; V(-1)^2 == det^2 */
    {
        double need = (double)(b->len - 1) * 2.585 + 2.0;     /* log2 6^(c-1) */
        double have = 0.0;
        double vbits = 2.0 * (double)magnus_bitorum(vm1) + 2.0;
        res p = 0x7FFFFFFFU;
        int primes = 0, agree = 1;

        if (vbits > need) need = vbits;
        while (have < need) {
            res v, det;

            p = congruentia_primus_infra(p);
            v = congruentia_ex_magno(vm1, p);
            det = colouring_det_mod(b->pd, b->len, p);
            if (mulm(v, v, p) != mulm(det, det, p)) agree = 0;
            have += 30.9;
            primes++;
        }
        sprintf(msg, "%s: |V(-1)| = |colouring minor| (%d primes, %u bits)", b->name, primes,
            magnus_bitorum(vm1));
        check(P_R4, msg, agree);
    }
}

/* ---------- R5 at points ---------- */

typedef struct {
    int    n;
    int    len;
    int   *word;
} Word;

static Word
word_make (int n, int len)
{
    Word w;

    w.n = n;
    w.len = len;
    w.word = (int *)malloc((size_t)(len > 0 ? len : 1) * sizeof(int));
    return w;
}

static void
check_r5 (
    const Braid *b,
    res          p,
    int          points)
{
    PrimeCache pc;
    res *A = (res *)malloc((size_t)points * sizeof(res));
    res *v0 = (res *)malloc((size_t)points * sizeof(res));
    int got = 0, t, k, n = b->n, len = b->len, ok;
    res omega = 2;
    Word back[2], front[2], flip, conj;
    int sgn, kc = n / 2 > 0 ? n / 2 : 1;
    char msg[256];
    long bad;

    memset(&pc, 0, sizeof(pc));
    while (got < points) {
        res a = 0;

        if (choose_point(&pc, p, omega++, &a) == INTERPOLATIO_UTILE) {
            v0[got] = tl_value(n, b->word, len, a, p, &ok);
            if (!ok) continue;
            A[got++] = a;
        }
    }
    for (sgn = 0; sgn < 2; sgn++) {
        int s = sgn == 0 ? 1 : -1;

        back[sgn] = word_make(n + 1, len + 1);
        for (k = 0; k < len; k++) back[sgn].word[k] = b->word[k];
        back[sgn].word[len] = s * n;
        front[sgn] = word_make(n + 1, len + 1);
        front[sgn].word[0] = s;
        for (k = 0; k < len; k++)
            front[sgn].word[k + 1] = b->word[k] > 0 ? b->word[k] + 1 : b->word[k] - 1;
    }
    flip = word_make(n, len);
    for (k = 0; k < len; k++)
        flip.word[k] = b->word[k] > 0 ? n - b->word[k] : -(n + b->word[k]);
    conj = word_make(n, len + 2);
    conj.word[0] = kc;
    for (k = 0; k < len; k++) conj.word[k + 1] = b->word[k];
    conj.word[len + 1] = -kc;

    for (sgn = 0; sgn < 2; sgn++) {
        int end;

        for (end = 0; end < 2; end++) {
            Word *wv = end == 0 ? &back[sgn] : &front[sgn];

            bad = 0;
            for (t = 0; t < points; t++) {
                res a3 = powm(A[t], 3, p);
                res factor = subm(0, sgn == 0 ? a3 : invm(a3, p), p);
                res v = tl_value(wv->n, wv->word, wv->len, A[t], p, &ok);

                if (!ok || v != mulm(factor, v0[t], p)) bad++;
            }
            sprintf(msg, "%s: stabilization at the %s, sigma^%s on %d strands: value = -A^%s3 value (%d points, %ld off)",
                b->name, end == 0 ? "back" : "front", sgn == 0 ? "+1" : "-1", n + 1,
                sgn == 0 ? "+" : "-", points, bad);
            check(P_R5, msg, bad == 0);
        }
    }
    bad = 0;
    for (t = 0; t < points; t++)
        if (tl_value(n, flip.word, len, A[t], p, &ok) != v0[t]) bad++;
    sprintf(msg, "%s: index flip sigma_i -> sigma_(n-i): same value (%d points, %ld off)", b->name, points, bad);
    check(P_R5, msg, bad == 0);
    bad = 0;
    for (t = 0; t < points; t++)
        if (tl_value(n, conj.word, len + 2, A[t], p, &ok) != v0[t]) bad++;
    sprintf(msg, "%s: conjugation by sigma_%d (smoke): same value (%d points, %ld off)", b->name, kc, points, bad);
    check(P_R5, msg, bad == 0);
    for (sgn = 0; sgn < 2; sgn++) {
        free(back[sgn].word);
        free(front[sgn].word);
    }
    free(flip.word);
    free(conj.word);
    free(A);
    free(v0);
}

/* ---------- R1p at off-interpolation points ---------- */

static void
check_r1p (
    const Braid *b,
    Polynomium   bracket,
    res          pstar,
    res          min_interp_p,
    int          points,
    double      *t_tl,
    double      *t_vx)
{
    PrimeCache pc;
    res omega = 2;
    int got = 0;
    long bad = 0, bad_point = 0;
    char msg[256];

    sprintf(msg, "%s: p* = %u below every interpolation prime (lowest used %u)", b->name, pstar, min_interp_p);
    check(P_R1P, msg, min_interp_p > pstar);
    memset(&pc, 0, sizeof(pc));
    *t_tl = 0.0;
    *t_vx = 0.0;
    while (got < points) {
        res a = 0, vt, vp;
        i32 av = 0, vv = 0;
        s32 st = choose_point(&pc, pstar, omega, &a);
        s32 sv;
        clock_t t0;
        int ok;

        if (st != INTERPOLATIO_UTILE) { omega++; continue; }
        t0 = clock();
        vt = tl_value(b->n, b->word, b->len, a, pstar, &ok);
        *t_tl += seconds_since(t0);
        t0 = clock();
        sv = laqueus_modulus_vertex_aestimare(b->mv, omega, pstar, &av, &vv);
        *t_vx += seconds_since(t0);
        vp = poly_at(bracket, a, pstar);
        if (sv != INTERPOLATIO_UTILE || av != a) bad_point++;
        else if (vt != vv || vp != vv) bad++;
        got++;
        omega++;
    }
    sprintf(msg, "%s: TL value == vertex value == reduced exact polynomial at %d points of p* (A equal: %ld off; values: %ld off)",
        b->name, points, bad_point, bad);
    check(P_R1P, msg, bad == 0 && bad_point == 0);
}

/* ---------- R2, R3 ---------- */

static void
part_r2 (void)
{
    int n, j, s, x, y;

    printf("\nR2: link states of W_{n,j}, n <= 16\n");
    for (n = 1; n <= NMAX; n++) {
        long sum_d = 0, sum_d2 = 0, bad_count = 0, bad_valid = 0, bad_order = 0;
        char msg[256];

        ensure_modules(n);
        for (j = n % 2; j <= n; j += 2) {
            const Module *m = &modules[n][j];

            if ((long)m->d != dim_formula(n, j)) bad_count++;
            sum_d += m->d;
            sum_d2 += (long)m->d * (long)m->d;
            for (s = 0; s < m->d; s++) {
                const signed char *st = m->partner + (long)s * n;
                int defects = 0;

                if (s > 0 && !(m->key[s - 1] < m->key[s])) bad_order++;
                for (x = 0; x < n; x++) {
                    if (st[x] < 0) { defects++; continue; }
                    if (st[x] >= n || st[(int)st[x]] != x || st[x] == x) bad_valid++;
                }
                if (defects != j) bad_valid++;
                for (x = 0; x < n; x++) {
                    int a = x, bb = st[x];

                    if (bb <= a) continue;
                    for (y = a + 1; y < bb; y++) {
                        if (st[y] < 0) bad_valid++;                       /* enclosed defect */
                        else if (st[y] < a || st[y] > bb) bad_valid++;    /* crossing */
                    }
                }
            }
        }
        sprintf(msg, "n=%2d: d_{n,j} = formula for every j, distinct (%ld), valid (%ld), sum d = C(n,n/2) = %ld, sum d^2 = Catalan(n) = %ld",
            n, bad_order, bad_valid, sum_d, sum_d2);
        check(P_R2, msg, bad_count == 0 && bad_order == 0 && bad_valid == 0
            && sum_d == binom(n, n / 2) && sum_d2 == binom(2 * n, n) / (n + 1));
    }
}

/* sparse vector: up to 64 (index, coefficient) terms */
typedef struct {
    int  k;
    int  idx[64];
    res  cf[64];
} Sparse;

static void
sp_norm (Sparse *v, res p)
{
    int a, b, out = 0;

    for (a = 1; a < v->k; a++) {                      /* insertion sort by index */
        int ti = v->idx[a];
        res tc = v->cf[a];

        b = a - 1;
        while (b >= 0 && v->idx[b] > ti) {
            v->idx[b + 1] = v->idx[b];
            v->cf[b + 1] = v->cf[b];
            b--;
        }
        v->idx[b + 1] = ti;
        v->cf[b + 1] = tc;
    }
    for (a = 0; a < v->k; a++) {
        if (out > 0 && v->idx[out - 1] == v->idx[a]) v->cf[out - 1] = addm(v->cf[out - 1], v->cf[a], p);
        else { v->idx[out] = v->idx[a]; v->cf[out] = v->cf[a]; out++; }
    }
    v->k = 0;
    for (a = 0; a < out; a++)
        if (v->cf[a] != 0) { v->idx[v->k] = v->idx[a]; v->cf[v->k] = v->cf[a]; v->k++; }
}

/* apply e_i (gen = 0) or sigma_i^{+-1} (gen = +-1) */
static Sparse
sp_apply (const Module *m, int i, int gen, const Sparse *v, res A, res delta, res p)
{
    Sparse out;
    int a;
    res Ainv = invm(A, p);

    out.k = 0;
    for (a = 0; a < v->k; a++) {
        int s = v->idx[a];
        int img = m->image[(long)i * m->d + s];
        unsigned char tg = m->tag[(long)i * m->d + s];
        res ce = gen == 0 ? 1 : (gen > 0 ? Ainv : A);
        res c1 = gen > 0 ? A : Ainv;

        if (gen != 0) {
            out.idx[out.k] = s;
            out.cf[out.k] = mulm(c1, v->cf[a], p);
            out.k++;
        }
        if (tg != 0) {
            res f = tg == 2 ? mulm(ce, delta, p) : ce;

            out.idx[out.k] = img;
            out.cf[out.k] = mulm(f, v->cf[a], p);
            out.k++;
        }
        if (out.k > 60) fatal("sparse vector overflow");
    }
    sp_norm(&out, p);
    return out;
}

static int
sp_equal (const Sparse *a, const Sparse *b)
{
    int t;

    if (a->k != b->k) return 0;
    for (t = 0; t < a->k; t++)
        if (a->idx[t] != b->idx[t] || a->cf[t] != b->cf[t]) return 0;
    return 1;
}

static void
part_r3 (void)
{
    res p = 2147483629U, A = 123456789U;
    res Ainv = invm(A, p);
    res delta = subm(0, addm(mulm(A, A, p), mulm(Ainv, Ainv, p), p), p);
    int n, j, s, i, k;

    printf("\nR3: relations on every basis state of W_{n,j}, n <= 16 (mod p, A fixed)\n");
    for (n = 2; n <= NMAX; n++) {
        long bad_sq = 0, bad_tri = 0, bad_far = 0, bad_braid = 0, bad_inv = 0;
        char msg[256];

        ensure_modules(n);
        for (j = n % 2; j <= n; j += 2) {
            const Module *m = &modules[n][j];

            for (s = 0; s < m->d; s++) {
                Sparse e1;

                e1.k = 1;
                e1.idx[0] = s;
                e1.cf[0] = 1;
                for (i = 0; i + 1 < n; i++) {
                    Sparse x = sp_apply(m, i, 0, &e1, A, delta, p), xx, ref;

                    /* e_i^2 = delta e_i */
                    xx = sp_apply(m, i, 0, &x, A, delta, p);
                    ref = x;
                    for (k = 0; k < ref.k; k++) ref.cf[k] = mulm(ref.cf[k], delta, p);
                    if (!sp_equal(&xx, &ref)) bad_sq++;
                    /* sigma_i sigma_i^-1 = 1 */
                    {
                        Sparse y = sp_apply(m, i, 1, &e1, A, delta, p);

                        y = sp_apply(m, i, -1, &y, A, delta, p);
                        if (!sp_equal(&y, &e1)) bad_inv++;
                    }
                    if (i + 2 < n) {
                        Sparse y, z;

                        /* e_i e_{i+1} e_i = e_i and e_{i+1} e_i e_{i+1} = e_{i+1} */
                        y = sp_apply(m, i, 0, &e1, A, delta, p);
                        y = sp_apply(m, i + 1, 0, &y, A, delta, p);
                        y = sp_apply(m, i, 0, &y, A, delta, p);
                        if (!sp_equal(&y, &x)) bad_tri++;
                        z = sp_apply(m, i + 1, 0, &e1, A, delta, p);
                        y = sp_apply(m, i, 0, &z, A, delta, p);
                        y = sp_apply(m, i + 1, 0, &y, A, delta, p);
                        if (!sp_equal(&y, &z)) bad_tri++;
                        /* sigma_i sigma_{i+1} sigma_i = sigma_{i+1} sigma_i sigma_{i+1} */
                        y = sp_apply(m, i, 1, &e1, A, delta, p);
                        y = sp_apply(m, i + 1, 1, &y, A, delta, p);
                        y = sp_apply(m, i, 1, &y, A, delta, p);
                        z = sp_apply(m, i + 1, 1, &e1, A, delta, p);
                        z = sp_apply(m, i, 1, &z, A, delta, p);
                        z = sp_apply(m, i + 1, 1, &z, A, delta, p);
                        if (!sp_equal(&y, &z)) bad_braid++;
                    }
                    for (k = i + 2; k + 1 < n; k++) {
                        Sparse y, z;

                        y = sp_apply(m, i, 0, &e1, A, delta, p);
                        y = sp_apply(m, k, 0, &y, A, delta, p);
                        z = sp_apply(m, k, 0, &e1, A, delta, p);
                        z = sp_apply(m, i, 0, &z, A, delta, p);
                        if (!sp_equal(&y, &z)) bad_far++;
                    }
                }
            }
        }
        sprintf(msg, "n=%2d: e_i^2 = delta e_i (%ld off), e_i e_(i+-1) e_i = e_i (%ld off), far commute (%ld off), braid relation (%ld off), sigma sigma^-1 = 1 (%ld off)",
            n, bad_sq, bad_tri, bad_far, bad_braid, bad_inv);
        check(P_R3, msg, bad_sq + bad_tri + bad_far + bad_braid + bad_inv == 0);
    }
}

/* ---------- conv0: trefoil against the state sum; trivial braid ---------- */

static void
part_conv (Piscina *piscina)
{
    static const int trefoil[3] = { 1, 1, 1 };
    s32 w32[3] = { 1, 1, 1 };
    i32 *pd, c, k;
    Polynomium ss;
    PrimeCache pc;
    res p = congruentia_primus_infra(0xFFFFFFFFU), omega = 2;
    int got = 0, bad = 0, ok;
    char msg[256];

    printf("\nconv0: the trefoil sigma_1^3 against the state sum at points; the trivial braid\n");
    while (p % 4 != 1) p = congruentia_primus_infra(p);
    if (!laqueus_pd_ex_plexu(w32, 3, 2, piscina, &pd, &c) || !laqueus_uncinus_ex_pd(pd, c, piscina, &ss))
        fatal("trefoil: PD or state sum refused");
    {
        s32 wpd = 0;

        for (k = 0; k < c; k++) wpd += _signum(pd + 4 * k, 2 * c);
        sprintf(msg, "trefoil: PD writhe %d = word writhe %d (sigma^+1 = positive crossing)", wpd,
            word_writhe(trefoil, 3));
        check(P_CONV, msg, wpd == word_writhe(trefoil, 3));
    }
    memset(&pc, 0, sizeof(pc));
    while (got < 8) {
        res a = 0;

        if (choose_point(&pc, p, omega++, &a) != INTERPOLATIO_UTILE) continue;
        if (tl_value(2, trefoil, 3, a, p, &ok) != poly_at(ss, a, p) || !ok) bad++;
        got++;
    }
    sprintf(msg, "trefoil: TL value (conv0) == state sum at 8 points (%d off)", bad);
    check(P_CONV, msg, bad == 0);
    bad = 0;
    got = 0;
    omega = 2;
    while (got < 8) {
        res a = 0;

        if (choose_point(&pc, p, omega++, &a) != INTERPOLATIO_UTILE) continue;
        if (tl_value(1, trefoil, 0, a, p, &ok) != 1 || !ok) bad++;
        got++;
    }
    sprintf(msg, "trivial braid, one strand: value 1 at 8 points (%d off)", bad);
    check(P_CONV, msg, bad == 0);
}

/* ---------- one braid through R1, R4, R5 (and R1p, R6) ---------- */

static res pstar = 0;
static int show_times = 0;                   /* D136_TEMPORA=1 */

/* span of V = span of the bracket in A over 4 (t = A^-4) */
static void
print_span (const Braid *b, Polynomium tl)
{
    int span = (int)((polynomium_gradus_summus(tl) - polynomium_gradus_imus(tl)) / 4);

    printf("  span of V %d (c - span = %d)\n", span, b->len - span);
}

static int
vertex_full_allowed (const Braid *b)
{
    return (b->n <= 12 && b->len <= 202) || (b->n == 8 && (b->len == 401 || b->len == 801));
}

static void
run_braid (
    Braid   *b,
    Piscina *piscina,
    int      r1p_points,
    int      r6)
{
    Polynomium tl, ref;
    TLContext ctx;
    clock_t t0 = clock();
    double t_tl, t_vx;
    char msg[512];
    int have_tl, r5_points;

    printf("\n%s (n = %d, c = %d, window [%d, %d], bound %u bits)\n", b->name, b->n, b->len,
        b->imus, b->summus, b->limes);
    have_tl = tl_bracket(b, piscina, &tl, &ctx);
    if (show_times)
        printf("  TL bracket: %s, %.2f s, %ld evaluations\n", have_tl ? "ok" : "REFUSED",
            seconds_since(t0), ctx.evaluations);
    else
        printf("  TL bracket: %s, %ld evaluations\n", have_tl ? "ok" : "REFUSED", ctx.evaluations);
    if (!have_tl) {
        sprintf(msg, "%s: TL bracket by interpolation", b->name);
        check(P_R1, msg, 0);
        return;
    }
    printf("  largest coefficient %u bits (bound %u)\n", max_coeff_bits(tl), b->limes);
    print_span(b, tl);
    if (b->len <= 24) {
        if (laqueus_uncinus_ex_pd(b->pd, (i32)b->len, piscina, &ref)) {
            sprintf(msg, "%s: TL bracket == state sum", b->name);
            check(P_R1, msg, polynomium_aequalis(tl, ref));
        } else {
            sprintf(msg, "%s: state sum refused", b->name);
            check(P_R1, msg, 0);
        }
    }
    if (vertex_full_allowed(b)) {
        t0 = clock();
        if (laqueus_modulus_vertex_uncinus(b->mv, piscina, &ref)) {
            if (show_times)
                sprintf(msg, "%s: TL bracket == vertex-model bracket (vertex %.2f s)", b->name,
                    seconds_since(t0));
            else
                sprintf(msg, "%s: TL bracket == vertex-model bracket", b->name);
            check(P_R1, msg, polynomium_aequalis(tl, ref));
        } else {
            sprintf(msg, "%s: vertex-model bracket refused", b->name);
            check(P_R1, msg, 0);
        }
    }
    check_r4(b, tl, piscina, 1);
    r5_points = (int)((b->summus - b->imus) / 4) + 3;
    check_r5(b, pstar, r5_points);
    if (r1p_points > 0) {
        check_r1p(b, tl, pstar, ctx.min_p, r1p_points, &t_tl, &t_vx);
        if (show_times) printf("  R1p timing: TL %.4f s, vertex %.4f s over %d points (ratio %.1f)\n", t_tl, t_vx,
            r1p_points, t_tl > 0 ? t_vx / t_tl : 0.0);
        if (r6 > 0) {
            sprintf(msg, "%s: TL per point >= %dx faster than the vertex model (measured %.1fx)",
                b->name, r6, t_tl > 0 ? t_vx / t_tl : 0.0);
            check(P_R6, msg, t_tl > 0 && t_vx / t_tl >= (double)r6);
        }
    }
}

/* ---------- R0 ---------- */

typedef struct { int n, len; i32 limes, latitudo; } Fingerprint;

static void
part_r0 (void)
{
    static const Fingerprint fp[4] = {
        { 8, 801, 642, 16 }, { 10, 401, 334, 22 }, { 12, 401, 333, 24 }, { 12, 801, 654, 26 }
    };
    int t;

    printf("\nR0: the test braids are the projection run's braids\n");
    for (t = 0; t < 4; t++) {
        Piscina *pisc = piscina_generare_dynamicum("d136_r0", 1048576);
        Braid b;
        char msg[256];

        if (!random_braid(&b, fp[t].n, fp[t].len, pisc)) {
            sprintf(msg, "%d x %d: braid or vertex structure refused", fp[t].n, fp[t].len);
            check(P_R0, msg, 0);
        } else {
            i32 lat = laqueus_modulus_vertex_sumptus(b.mv).latitudo;

            sprintf(msg, "%s: limes %u (registered %u), latitudo %u (registered %u)", b.name,
                b.limes, fp[t].limes, lat, fp[t].latitudo);
            check(P_R0, msg, b.limes == fp[t].limes && lat == fp[t].latitudo);
        }
        piscina_destruere(pisc);
    }
}

/* ---------- descriptive (no prediction): plan.md "Descriptive" ---------- */

/* Spans: the TL bracket of every test braid, span of V against c. */
static void
desc_spans (void)
{
    static const int lens[4] = { 20, 50, 101, 201 };
    static const int big[5][2] = { { 8, 401 }, { 8, 801 }, { 10, 401 }, { 12, 401 }, { 12, 801 } };
    int n, k;

    printf("\nDescriptive: span of V against c (reduced alternating iff c - span = 0)\n");
    for (k = 0; k < 4 * 10 + 5; k++) {
        Piscina *pisc = piscina_generare_dynamicum("d136_span", 1048576);
        Braid b;
        Polynomium tl;
        TLContext ctx;
        int len;

        n = k < 40 ? 3 + k / 4 : big[k - 40][0];
        len = k < 40 ? lens[k % 4] : big[k - 40][1];
        if (!random_braid(&b, n, len, pisc) || !tl_bracket(&b, pisc, &tl, &ctx))
            printf("%d x %d: REFUSED\n", n, len);
        else {
            printf("%s: largest coefficient %u bits (bound %u)\n", b.name, max_coeff_bits(tl), b.limes);
            print_span(&b, tl);
        }
        fflush(stdout);
        piscina_destruere(pisc);
    }
}

/* Per-point time at c = 801, n = 4 ... 16 (seed 2028 per n, as R0). */
static void
desc_times (void)
{
    int n;

    printf("\nDescriptive: TL time per point at c = 801 (3 points of p*)\n");
    for (n = 4; n <= NMAX; n++) {
        Piscina *pisc = piscina_generare_dynamicum("d136_times", 1048576);
        Sors s;
        i32 len = 801;
        s32 *w32 = NULL;
        int *word;
        PrimeCache pc;
        double secs = 0;
        long catalan = binom(2 * n, n) / (n + 1), dmax = 0;
        int k, j, ok, pts = 0;

        sors_seminare(&s, (i64)2028, 0);
        if (_plexus_fortuitus(&s, (i32)n, &len, pisc, &w32) == NULL) {
            printf("  n = %2d: no knot\n", n);
            piscina_destruere(pisc);
            continue;
        }
        word = (int *)piscina_allocare(pisc, (memoriae_index)len * sizeof(int));
        for (k = 0; k < (int)len; k++) word[k] = w32[k];
        ensure_modules(n);
        for (j = n % 2; j <= n; j += 2)
            if (modules[n][j].d > dmax) dmax = modules[n][j].d;
        memset(&pc, 0, sizeof(pc));
        for (k = 0; k < 3; k++) {
            res A = 0;
            clock_t t0;

            if (choose_point(&pc, pstar, (res)(k + 2), &A) != INTERPOLATIO_UTILE) continue;
            t0 = clock();
            (void)tl_value(n, word, (int)len, A, pstar, &ok);
            secs += seconds_since(t0);
            pts++;
        }
        if (pts > 0) secs /= pts;
        printf("  n = %2d  c = %u  Catalan %9ld  largest module %5ld  %9.3f s per point  (%.2f ns per c x Catalan)\n",
            n, len, catalan, dmax, secs, secs * 1e9 / ((double)len * (double)catalan));
        fflush(stdout);
        piscina_destruere(pisc);
    }
}

/* Exact traces over Z[x], x = A^2, with no primes. Each letter is scaled
 * by A^3 so every entry stays an integer polynomial:
 *   A^3 sigma = x^2 1 + E~,   A^3 sigma^-1 = x 1 + x E~,
 *   E~ = A^2 e: coefficient x (re-pair, moved defect), -(x^2 + 1) (loop).
 * The degree rises by at most 3 per letter, so 3c + 1 coefficients.
 * terms == 1: the same at the single point A = 2 (x = 4), entries in Z;
 * there the scaling adds up to 3c bits (8^c), in Z[x] it is a shift. */

static const s64 ex_id_pos[4] = { 0, 0, 1, 0 };
static const s64 ex_id_neg[4] = { 0, 1, 0, 0 };
static const s64 ex_e_pos[3][4] = { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { -1, 0, -1, 0 } };
static const s64 ex_e_neg[3][4] = { { 0, 0, 0, 0 }, { 0, 0, 1, 0 }, { 0, -1, 0, -1 } };

typedef struct {
    int            terms;
    i32            max_bits;     /* every intermediate entry, every module */
    i32            mark_bits[4]; /* after c/4, c/2, 3c/4, c letters */
    memoriae_index arena_peak;
    long           ops;
} ExactStats;

/* dst += f src, f a polynomial in x of degree <= 3 (point mode: f(4)).
 * magnus_adde and _multiplica always allocate the result in pisc, so
 * nothing in dst shares storage with src. */
static void
ex_axpy (
    Magnus       *dst,
    const Magnus *src,
    const s64    *f,
    int           terms,
    int           hi,
    Piscina      *pisc,
    long         *ops)
{
    int k, e;

    if (terms == 1) {
        s64 v = f[0] + 4 * f[1] + 16 * f[2] + 64 * f[3];

        if (v == 0 || magnus_signum(src[0]) == 0) return;
        dst[0] = magnus_adde(dst[0], magnus_multiplica(src[0], magnus_ex_s64(v), pisc), pisc);
        (*ops)++;
        return;
    }
    for (k = 0; k < 4; k++) {
        if (f[k] == 0) continue;
        for (e = 0; e <= hi; e++) {
            if (magnus_signum(src[e]) == 0) continue;
            if (e + k >= terms) fatal("exact trace: degree bound exceeded");
            if (f[k] == 1) dst[e + k] = magnus_adde(dst[e + k], src[e], pisc);
            else dst[e + k] = magnus_subtrahe(dst[e + k], src[e], pisc);
            (*ops)++;
        }
    }
}

/* tr[j] = the trace of A^(3c) rho_j(word), terms coefficients, in keep */
static void
exact_traces (
    int         n,
    const int  *word,
    int         len,
    Magnus    **tr,
    Piscina    *keep,
    ExactStats *st)
{
    Piscina *ar[2];
    int terms = st->terms, j, q;

    ar[0] = piscina_generare_dynamicum("d136_exact_a", 1048576);
    ar[1] = piscina_generare_dynamicum("d136_exact_b", 1048576);
    ensure_modules(n);
    for (j = n % 2; j <= n; j += 2) {
        const Module *m = &modules[n][j];
        long d = m->d, cells = d * d * terms, e, s, r, c;
        Magnus *mat = (Magnus *)malloc((size_t)cells * sizeof(Magnus));
        Magnus *nb = (Magnus *)malloc((size_t)cells * sizeof(Magnus));
        Magnus *swap, zero = magnus_ex_s64(0);
        int k, hi = 0, step = 0;

        if (mat == NULL || nb == NULL) fatal("out of memory (exact trace)");
        for (e = 0; e < cells; e++) mat[e] = zero;
        for (s = 0; s < d; s++) mat[(s * d + s) * terms] = magnus_ex_s64(1);
        for (k = len - 1; k >= 0; k--, step++) {
            int letter = word[k];
            int i = (letter > 0 ? letter : -letter) - 1;
            const int *img = m->image + (long)i * d;
            const unsigned char *tg = m->tag + (long)i * d;
            const s64 *fid = letter > 0 ? ex_id_pos : ex_id_neg;
            int nhi = terms == 1 ? 0 : hi + (letter > 0 ? 2 : 3);
            Piscina *t = ar[step % 2];
            i32 b = 0;

            if (nhi >= terms) nhi = terms - 1;
            piscina_vacare(t);
            for (e = 0; e < cells; e++) nb[e] = zero;
            for (r = 0; r < d * d; r++) ex_axpy(nb + r * terms, mat + r * terms, fid, terms, hi, t, &st->ops);
            for (s = 0; s < d; s++) {
                const s64 *fe;

                if (tg[s] == 0) continue;
                fe = letter > 0 ? ex_e_pos[tg[s]] : ex_e_neg[tg[s]];
                for (c = 0; c < d; c++)
                    ex_axpy(nb + ((long)img[s] * d + c) * terms, mat + (s * d + c) * terms, fe, terms,
                        hi, t, &st->ops);
            }
            hi = nhi;
            swap = mat;
            mat = nb;
            nb = swap;
            for (e = 0; e < cells; e++) {
                i32 x = magnus_bitorum(mat[e]);

                if (x > b) b = x;
            }
            if (b > st->max_bits) st->max_bits = b;
            for (q = 0; q < 4; q++)
                if (step + 1 == (len * (q + 1)) / 4 && b > st->mark_bits[q]) st->mark_bits[q] = b;
        }
        tr[j] = (Magnus *)piscina_allocare(keep, (memoriae_index)terms * sizeof(Magnus));
        for (e = 0; e < terms; e++) {
            Magnus acc = zero;

            for (s = 0; s < d; s++) acc = magnus_adde(acc, mat[(s * d + s) * terms + e], keep);
            tr[j][e] = acc;
        }
        free(mat);
        free(nb);
    }
    st->arena_peak = piscina_summa_apex_usus(ar[0]) + piscina_summa_apex_usus(ar[1]);
    piscina_destruere(ar[0]);
    piscina_destruere(ar[1]);
}

/* the bracket at A = 2 mod p from the exact traces */
static res
exact_at_two (
    int      n,
    int      len,
    Magnus **tr,
    int      terms,
    res      p)
{
    res Ainv = invm(2, p);
    res delta = subm(0, addm(4, mulm(Ainv, Ainv, p), p), p);
    res Dl[NMAX + 3], total = 0;
    int j, e;

    Dl[0] = 1;
    Dl[1] = delta;
    for (j = 1; j + 1 <= n; j++) Dl[j + 1] = subm(mulm(delta, Dl[j], p), Dl[j - 1], p);
    for (j = n % 2; j <= n; j += 2) {
        res v = 0, xe = 1;

        for (e = 0; e < terms; e++) {
            v = addm(v, mulm(magnus_residuum_parvum(tr[j][e], p), xe, p), p);
            xe = mulm(xe, 4, p);
        }
        total = addm(total, mulm(Dl[j], v, p), p);
    }
    total = mulm(total, powm(Ainv, 3ULL * (ull)len, p), p);
    return mulm(total, invm(delta, p), p);
}

static void
desc_exact (int n, int len_req, int poly)
{
    Piscina *pisc = piscina_generare_dynamicum("d136_exact", 1048576);
    Braid b;
    Magnus *tr[NMAX + 1];
    ExactStats st;
    clock_t t0;
    i32 trbits = 0;
    res mine, ref;
    int j, e, ok;

    if (!random_braid(&b, n, len_req, pisc)) {
        printf("  %d x %d: REFUSED\n", n, len_req);
        piscina_destruere(pisc);
        return;
    }
    memset(&st, 0, sizeof(st));
    st.terms = poly ? 3 * b.len + 1 : 1;
    t0 = clock();
    exact_traces(n, b.word, b.len, tr, pisc, &st);
    for (j = n % 2; j <= n; j += 2)
        for (e = 0; e < st.terms; e++) {
            i32 x = magnus_bitorum(tr[j][e]);

            if (x > trbits) trbits = x;
        }
    mine = exact_at_two(n, b.len, tr, st.terms, pstar);
    ref = tl_value(n, b.word, b.len, 2, pstar, &ok);
    printf("  %s: entries <= %u bits (after c/4, c/2, 3c/4, c: %u, %u, %u, %u); traces %u bits\n",
        b.name, st.max_bits, st.mark_bits[0], st.mark_bits[1], st.mark_bits[2], st.mark_bits[3], trbits);
    printf("    %ld big-number operations, arenas peak %.1f MB, %.1f s; at A = 2 mod p*: %s\n",
        st.ops, (double)st.arena_peak / 1048576.0, seconds_since(t0),
        ok && mine == ref ? "agrees with the modular route" : "DISAGREES");
    if (poly) {
        Polynomium tl;
        TLContext ctx;

        if (tl_bracket(&b, pisc, &tl, &ctx))
            printf("    bracket coefficients %u bits (bound %u)\n", max_coeff_bits(tl), b.limes);
    } else printf("    (scaling 8^c adds up to %d bits)\n", 3 * b.len);
    fflush(stdout);
    piscina_destruere(pisc);
}

static void
desc_integers (void)
{
    static const int set[7][2] = { { 4, 201 }, { 8, 201 }, { 12, 201 }, { 8, 401 }, { 8, 801 }, { 12, 401 }, { 12, 801 } };
    int k;

    printf("\nDescriptive: largest entry over Z at A = 2 (no primes)\n");
    for (k = 0; k < 7; k++) desc_exact(set[k][0], set[k][1], 0);
}

static void
desc_polynomials (void)
{
    /* the last two go past the plan's c <= 201 (cheap at 8 strands) */
    static const int set[15][2] = {
        { 4, 50 }, { 4, 101 }, { 4, 201 }, { 6, 50 }, { 6, 101 }, { 6, 201 },
        { 8, 50 }, { 8, 101 }, { 8, 201 }, { 10, 50 }, { 10, 101 }, { 12, 101 }, { 12, 201 },
        { 8, 401 }, { 8, 801 }
    };
    int k;

    printf("\nDescriptive: largest coefficient over Z[A] (no primes), c <= 201 (+ 8 x 401, 8 x 801)\n");
    for (k = 0; k < 15; k++) desc_exact(set[k][0], set[k][1], 1);
}

static void
run_descriptive (const char *pars)
{
    int all = pars == NULL;

    if (all || strcmp(pars, "times") == 0) desc_times();
    if (all || strcmp(pars, "spans") == 0) desc_spans();
    if (all || strcmp(pars, "integers") == 0) desc_integers();
    if (all || strcmp(pars, "polynomials") == 0) desc_polynomials();
}

/* ---------- main ---------- */

static void
run_named (const char *name, int n, const int *word, int len)
{
    Piscina *pisc = piscina_generare_dynamicum("d136", 1048576);
    Braid b;

    if (!prepare_braid(&b, name, n, word, len, NULL, pisc)) {
        char msg[128];

        sprintf(msg, "%s: PD or vertex structure refused", name);
        check(P_R1, msg, 0);
    } else run_braid(&b, pisc, 0, 0);
    piscina_destruere(pisc);
}

static void
run_random (int n, int len_req, int r1p_points, int r6)
{
    Piscina *pisc = piscina_generare_dynamicum("d136", 1048576);
    Braid b;

    if (!random_braid(&b, n, len_req, pisc)) {
        char msg[128];

        sprintf(msg, "%d x %d: no knot or vertex structure refused", n, len_req);
        check(P_R1, msg, 0);
    } else run_braid(&b, pisc, r1p_points, r6);
    piscina_destruere(pisc);
}

int
main (void)
{
    static const int w31[3] = { 1, 1, 1 };
    static const int w41[4] = { 1, -2, 1, -2 };
    static int t34[8], t45[15];
    const char *modus = getenv("D136_MODUS");
    int full = modus == NULL || strcmp(modus, "plene") == 0;
    int desc = modus != NULL && strcmp(modus, "desc") == 0;
    int p8 = modus != NULL && strcmp(modus, "p8") == 0;
    Piscina *pisc = piscina_generare_dynamicum("d136_main", 1048576);
    int k, n, pred;

    for (k = 0; k < 8; k++) t34[k] = k % 2 + 1;
    for (k = 0; k < 15; k++) t45[k] = k % 3 + 1;
    printf("KNOTAPEL DEMO 136: brackets of braid closures through TL standard modules\n");
    printf("mode: %s\n", full ? "registered test set (plan.md v2)" : desc ? "descriptive (no predictions)" :
        (p8 ? "p8 (t1 + 10 x 401)" : "t1 (PARTIAL RUN)"));
    show_times = getenv("D136_TEMPORA") != NULL;

    pstar = 1U << 30;
    do pstar = congruentia_primus_infra(pstar); while (pstar % 4 != 1);
    printf("p* = %u (largest prime = 1 mod 4 below 2^30)\n", pstar);
    if (desc) {
        run_descriptive(getenv("D136_PARS"));
        piscina_destruere(pisc);
        return 0;
    }

    part_r2();
    part_r3();
    part_conv(pisc);
    part_r0();

    run_named("3_1 = sigma_1^3", 2, w31, 3);
    run_named("4_1 = s1 s2^-1 s1 s2^-1", 3, w41, 4);
    run_named("T(3,4) = (s1 s2)^4", 3, t34, 8);
    run_named("T(4,5) = (s1 s2 s3)^5", 4, t45, 15);

    if (full) {
        static const int lens[4] = { 20, 50, 101, 201 };

        for (n = 3; n <= 12; n++)
            for (k = 0; k < 4; k++) run_random(n, lens[k], 0, 0);
        run_random(8, 401, 0, 0);
        run_random(8, 801, 0, 0);
        run_random(10, 401, 16, 0);
        run_random(12, 401, 8, 25);
        run_random(12, 801, 4, 100);
    } else {
        for (n = 3; n <= 6; n++) {
            run_random(n, 20, 0, 0);
            run_random(n, 50, 0, 0);
        }
        run_random(8, 101, 8, 0);                     /* R1p smoke */
        if (p8) run_random(10, 401, 16, 0);
    }

    printf("\nVerdicts (plan.md v2):\n");
    for (pred = 0; pred < P_NUM; pred++) {
        const char *state;

        if (pred_checks[pred] == 0) state = "NOT TESTED";
        else if (pred_fails[pred] != 0) state = "FALLS";
        else if (!full) state = "holds so far (PARTIAL RUN)";
        else state = "HOLDS";
        printf("  %-6s %s (%ld checks, %ld failed)\n", pred_name[pred], state,
            pred_checks[pred], pred_fails[pred]);
    }
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pisc);
    return n_fail == 0 ? 0 : 1;
}
