/*
 * KNOTAPEL DEMO 117: Exact Audit of the Cyclotomic Demos
 * ================================================================
 *
 * Demos 29 to 109 (57 of them) did their Z[zeta_8] / Z[zeta_16] / ...
 * arithmetic by hand, with `long` coefficients and hand-written
 * multiplication tables. Demo 114 showed what unchecked homemade
 * arithmetic did to demos 110-112. This demo audits the cyclotomic line
 * with the house libraries (cyclotomia: exact Z[zeta_n] with big-integer
 * coefficients; polynomium; matrix):
 *
 *   Part 0  triage (triage.sh, not this program): every cyclotomic demo
 *           compiled UNMODIFIED with -fsanitize=undefined and run; the
 *           table is triage.tsv in this folder
 *   Part A  demo 29: every bracket recomputed by a DIFFERENT route - the
 *           full Laurent polynomial in A with delta = -A^2 - A^-2 kept
 *           symbolic, then evaluated at A = zeta_8^5 by cyclotomia - and
 *           compared with demo 29's Cyc8 value (delta = 0 shortcut,
 *           hand-written product); over ALL 87,890 braids, not only the
 *           first 8,192 its catalog keeps
 *   Part B  demo 56: its partition-function catalog recomputed exactly,
 *           and its 2- and 3-input Boolean searches redone with EXACT
 *           products (its `long` triple products w1 w2 w3 overflow);
 *           classifications within float error of a boundary are counted
 *           as undecided, not guessed
 *
 * Code copied from the audited demos is marked "from demo NN" and kept
 * verbatim, so the comparison is against what they actually computed.
 * The loop counting of a closed braid state is demo 29's own (copied);
 * what is independent is the arithmetic, delta, and A.
 *
 * House libraries: includes cyclotomia.h, hence latina.h (Roman numerals
 * and Latin keywords are macros here). Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_117_cyclotomic_audit/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 */

#include "cyclotomia.h"
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

static int n_pass = 0;
static int n_fail = 0;

static void
check (
    const char *msg,
    int         ok)
{
    if (ok) {
        n_pass++;
        printf("  PASS: %s\n", msg);
    } else {
        n_fail++;
        printf("  FAIL: %s\n", msg);
    }
}

/* ================================================================
 * From demo 29 (verbatim): Cx, Cyc8, braids, loop counting, brackets
 * ================================================================ */

typedef struct { double re, im; } Cx;

static Cx cx_make(double re, double im) { Cx z; z.re = re; z.im = im; return z; }
static Cx cx_zero(void) { return cx_make(0.0, 0.0); }
static Cx cx_one(void)  { return cx_make(1.0, 0.0); }
static Cx cx_add(Cx a, Cx b) { return cx_make(a.re + b.re, a.im + b.im); }
static Cx cx_neg(Cx a) { return cx_make(-a.re, -a.im); }
static Cx cx_mul(Cx a, Cx b) {
    return cx_make(a.re * b.re - a.im * b.im,
                   a.re * b.im + a.im * b.re);
}
static double cx_abs(Cx a) { return sqrt(a.re * a.re + a.im * a.im); }
static Cx cx_exp_i(double theta) { return cx_make(cos(theta), sin(theta)); }

static Cx cx_pow_int(Cx a, int n) {
    Cx r = cx_one();
    Cx base;
    int neg;
    if (n == 0) return r;
    neg = (n < 0);
    if (neg) n = -n;
    base = a;
    while (n > 0) {
        if (n & 1) r = cx_mul(r, base);
        base = cx_mul(base, base);
        n >>= 1;
    }
    if (neg) {
        double d = r.re * r.re + r.im * r.im;
        r = cx_make(r.re / d, -r.im / d);
    }
    return r;
}

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
static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y) {
    /* (a,b,c,d)*(e,f,g,h) with zeta_8^4 = -1 */
    return cyc8_make(
        x.a * y.a - x.b * y.d - x.c * y.c - x.d * y.b,  /* was wrong in notes, rechecked */
        x.a * y.b + x.b * y.a - x.c * y.d - x.d * y.c,
        x.a * y.c + x.b * y.b + x.c * y.a - x.d * y.d,
        x.a * y.d + x.b * y.c + x.c * y.b + x.d * y.a
    );
}
static Cyc8 cyc8_conj(Cyc8 z) {
    return cyc8_make(z.a, -z.d, -z.c, -z.b);
}
static Cyc8 cyc8_pow_int(Cyc8 base, int n) {
    Cyc8 r = cyc8_one();
    if (n == 0) return r;
    if (n < 0) {
        n = -n;
        base = cyc8_conj(base); /* base^{-1} for units */
    }
    while (n > 0) {
        if (n & 1) r = cyc8_mul(r, base);
        base = cyc8_mul(base, base);
        n >>= 1;
    }
    return r;
}

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

#define MAX_UF 4096
static int uf_p[MAX_UF];
static void uf_init(int n) { int i; for (i = 0; i < n; i++) uf_p[i] = i; }
static int uf_find(int x) {
    while (uf_p[x] != x) { uf_p[x] = uf_p[uf_p[x]]; x = uf_p[x]; }
    return x;
}
static void uf_union(int x, int y) {
    x = uf_find(x); y = uf_find(y); if (x != y) uf_p[x] = y;
}

static int braid_loops(const Braid *b, unsigned s) {
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

static Cx braid_bracket_at(const Braid *b, Cx A) {
    unsigned s, ns;
    int i, a_count, b_count, lp, j;
    Cx result, delta, d_power, term, coeff;

    delta = cx_neg(cx_add(cx_pow_int(A, 2), cx_pow_int(A, -2)));

    result = cx_zero();
    if (!b->len) {
        result = cx_one();
        for (i = 0; i < b->n - 1; i++)
            result = cx_mul(result, delta);
        return result;
    }

    ns = 1u << b->len;
    for (s = 0; s < ns; s++) {
        a_count = 0; b_count = 0;
        for (i = 0; i < b->len; i++) {
            if ((s >> (unsigned)i) & 1u) b_count++;
            else a_count++;
        }
        lp = braid_loops(b, s);

        coeff = cx_pow_int(A, a_count - b_count);
        d_power = cx_one();
        for (j = 0; j < lp - 1; j++)
            d_power = cx_mul(d_power, delta);
        term = cx_mul(coeff, d_power);
        result = cx_add(result, term);
    }
    return result;
}

/* demo 29's EXACT bracket: delta = 0, so only single-loop states */
static Cyc8 braid_bracket_exact(const Braid *b, Cyc8 A) {
    unsigned s, ns;
    int i, a_count, b_count, lp;
    Cyc8 result, term, coeff;

    result = cyc8_zero();
    ns = 1u << b->len;
    for (s = 0; s < ns; s++) {
        a_count = 0; b_count = 0;
        for (i = 0; i < b->len; i++) {
            if ((s >> (unsigned)i) & 1u) b_count++;
            else a_count++;
        }
        lp = braid_loops(b, s);
        if (lp != 1) continue;
        coeff = cyc8_pow_int(A, a_count - b_count);
        term = coeff;
        result = cyc8_add(result, term);
    }
    return result;
}

/* ================================================================
 * The house route: Laurent polynomial in A, delta symbolic
 * ================================================================ */

/* <closure of b> = sum over states A^(a-b) delta^(loops-1), as an exact
 * Laurent polynomial: states are tallied by (a-b, loops) first, then
 * delta = -A^2 - A^-2 is expanded by polynomium. Independent of demo 29's
 * arithmetic, delta shortcut and choice of A (the loop count is demo
 * 29's). */
static Polynomium
bracket_polynomial (
    const Braid *b,
    Piscina     *pool)
{
    long          tally[2 * MAX_WORD + 1][MAX_WORD + 8];
    unsigned long s, ns = 1UL << b->len;
    int           e, l, i;
    int           max_loops = b->n + b->len + 1;
    Polynomium    delta, sum = polynomium_nullum();

    memset(tally, 0, sizeof(tally));
    for (s = 0; s < ns; s++) {
        int a_count = 0;

        for (i = 0; i < b->len; i++)
            if (!((s >> (unsigned)i) & 1UL))
                a_count++;
        /* a - b = 2a - len, stored at index 2a */
        tally[2 * a_count][braid_loops(b, (unsigned)s)]++;
    }
    (void)polynomium_ex_chorda(chorda_ex_literis("-A^2 - A^-2", pool), 'A',
        pool, &delta);
    for (e = 0; e <= 2 * b->len; e++) {
        for (l = 1; l <= max_loops && l < MAX_WORD + 8; l++) {
            Polynomium term, dpow;

            if (tally[e][l] == 0)
                continue;
            (void)polynomium_monomium(magnus_ex_s64(tally[e][l]),
                (s32)(e - b->len), pool, &term);
            (void)polynomium_potentia(delta, (i32)(l - 1), pool, &dpow);
            (void)polynomium_multiplica(term, dpow, pool, &term);
            sum = polynomium_adde(sum, term, pool);
        }
    }
    return sum;
}

static int
cyc8_to_house (
    Cyc8                z,
    const Cyclotomia   *r8,
    Piscina            *pool,
    Cyclotomicus       *out)
{
    s64 c[4];

    c[0] = (s64)z.a;
    c[1] = (s64)z.b;
    c[2] = (s64)z.c;
    c[3] = (s64)z.d;
    return cyclotomicus_ex_s64(r8, c, 4, pool, out);
}

/* ================================================================
 * Part A: demo 29
 * ================================================================ */

#define MAX_DISTINCT 4096

static s64 distinct[MAX_DISTINCT][4];
static int n_distinct = 0;

static int
note_distinct (
    const s64 *c)
{
    int k;

    for (k = 0; k < n_distinct; k++)
        if (memcmp(distinct[k], c, sizeof(distinct[k])) == 0)
            return 0;
    if (n_distinct < MAX_DISTINCT)
        memcpy(distinct[n_distinct++], c, sizeof(distinct[0]));
    return 1;
}

static void
part_a (
    Piscina *keep,
    Piscina *work)
{
    const Cyclotomia *r8 = cyclotomia_creare(8, keep);
    Cyc8              A29 = cyc8_make(0, -1, 0, 0);
    Cx                fA  = cx_exp_i(5.0 * M_PI / 4.0);
    Cyclotomicus      A_house = cyclotomicus_radix(r8, 5, keep);
    Cyclotomicus      A_29;
    unsigned long     total = 0, kept_29 = 0, nonzero = 0;
    unsigned long     mismatch = 0, filter_disagree = 0;
    int               distinct_first_8192 = -1;
    int               n, len;
    char              msg[200];

    printf("\n=== Part A: demo 29, every bracket by a second route ===\n");
    (void)cyc8_to_house(A29, r8, keep, &A_29);
    check("demo 29's A = (0,-1,0,0) is zeta_8^5 (A = e^{i 5pi/4})",
        cyclotomicus_aequalis(A_29, A_house));

    /* demo 29's hand-written product against the house ring */
    {
        unsigned long seed = 12345UL;
        int           t, bad = 0;

        for (t = 0; t < 10000; t++) {
            long         v[8];
            int          k;
            Cyclotomicus x, y, prod;
            s64          got[4];
            Cyc8         p29;

            for (k = 0; k < 8; k++) {
                seed = seed * 6364136223846793005UL + 1442695040888963407UL;
                v[k] = (long)((seed >> 33) % 2001UL) - 1000L;
            }
            p29 = cyc8_mul(cyc8_make(v[0], v[1], v[2], v[3]),
                cyc8_make(v[4], v[5], v[6], v[7]));
            (void)cyc8_to_house(cyc8_make(v[0], v[1], v[2], v[3]), r8, work, &x);
            (void)cyc8_to_house(cyc8_make(v[4], v[5], v[6], v[7]), r8, work, &y);
            prod = cyclotomicus_multiplica(x, y, work);
            if (!cyclotomicus_ad_s64(prod, got) || got[0] != p29.a
                || got[1] != p29.b || got[2] != p29.c || got[3] != p29.d)
                bad++;
        }
        sprintf(msg, "demo 29's cyc8_mul table equals Z[zeta_8] multiplication "
            "(10,000 random pairs, coefficients in [-1000, 1000]; %d differ)",
            bad);
        check(msg, bad == 0);
    }

    /* every braid demo 29 enumerates: n = 2, 3; length 1..8 */
    for (n = 2; n <= 3; n++) {
        for (len = 1; len <= 8; len++) {
            int           max_gen = n - 1, total_gens = 2 * max_gen, i;
            unsigned long count = 1, idx;

            for (i = 0; i < len; i++)
                count *= (unsigned long)total_gens;
            for (idx = 0; idx < count; idx++) {
                PiscinaNotatio mark = piscina_notare(work);
                unsigned long  tmp = idx;
                Braid          b;
                Cyc8           v29;
                Cx             vf;
                Cyclotomicus   v29h, vh;
                s64            c[4];
                int            keeps;

                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);

                    tmp /= (unsigned long)total_gens;
                    b.word[i] = g < max_gen ? g + 1 : -(g - max_gen + 1);
                }
                b.n = n;
                b.len = len;
                total++;
                v29 = braid_bracket_exact(&b, A29);
                vf  = braid_bracket_at(&b, fA);
                keeps = cx_abs(vf) > 0.5;
                (void)cyc8_to_house(v29, r8, work, &v29h);
                (void)cyclotomicus_ex_polynomio(r8, bracket_polynomial(&b, work),
                    5, work, &vh);
                if (!cyclotomicus_aequalis(v29h, vh))
                    mismatch++;
                if (!cyclotomicus_est_nullum(vh))
                    nonzero++;
                if (keeps != !cyclotomicus_est_nullum(vh))
                    filter_disagree++;
                if (keeps) {
                    kept_29++;
                    (void)cyclotomicus_ad_s64(vh, c);
                    (void)note_distinct(c);
                    if (kept_29 == 8192)
                        distinct_first_8192 = n_distinct;
                }
                piscina_reficere(work, mark);
            }
        }
    }
    printf("  braids enumerated: %lu; kept by demo 29's float filter (|z| > 0.5): "
        "%lu; exact nonzero: %lu\n", total, kept_29, nonzero);
    printf("  distinct exact values among the first 8,192 kept (demo 29's "
        "catalog cap): %d; among all kept: %d\n", distinct_first_8192,
        n_distinct);
    sprintf(msg, "every demo 29 bracket equals the symbolic-delta bracket at "
        "zeta_8^5 (%lu braids, %lu differ)", total, mismatch);
    check(msg, mismatch == 0);
    sprintf(msg, "demo 29's float filter |z| > 0.5 keeps exactly the nonzero "
        "brackets (%lu disagree)", filter_disagree);
    check(msg, filter_disagree == 0);
    sprintf(msg, "demo 29's catalog of 8,192 has 64 distinct values (got %d)",
        distinct_first_8192);
    check(msg, distinct_first_8192 == 64);
}


/* ================================================================
 * From demo 56 (verbatim apart from the fk_uf_ prefix and two renamed
 * locals): strip graphs,
 * component counting, activations, NPN classes
 * ================================================================ */

#define MAX_SITES 25
#define MAX_EDGES 50

typedef struct {
    int n_sites;
    int n_edges;
    int width, height;
    int edge_u[MAX_EDGES];
    int edge_v[MAX_EDGES];
} LatticeStrip;

static void build_strip(LatticeStrip *g, int w, int h) {
    int x, y;
    g->width = w;
    g->height = h;
    g->n_sites = w * h;
    g->n_edges = 0;
    for (y = 0; y < h; y++)
        for (x = 0; x < w - 1; x++) {
            g->edge_u[g->n_edges] = y * w + x;
            g->edge_v[g->n_edges] = y * w + x + 1;
            g->n_edges++;
        }
    for (y = 0; y < h - 1; y++)
        for (x = 0; x < w; x++) {
            g->edge_u[g->n_edges] = y * w + x;
            g->edge_v[g->n_edges] = (y + 1) * w + x;
            g->n_edges++;
        }
}

static int fk_uf_parent[MAX_SITES];
static void fk_uf_init(int n) { int i; for (i = 0; i < n; i++) fk_uf_parent[i] = i; }
static int fk_uf_find(int x) {
    while (fk_uf_parent[x] != x) {
        fk_uf_parent[x] = fk_uf_parent[fk_uf_parent[x]];
        x = fk_uf_parent[x];
    }
    return x;
}
static void fk_uf_union(int a, int b) {
    a = fk_uf_find(a); b = fk_uf_find(b); if (a != b) fk_uf_parent[a] = b;
}

static double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }

/* locals renamed: `si` is latina.h's `if` */
static int split_sigmoid_classify(Cx z) {
    double s_re = sigmoid(z.re);
    double s_im = sigmoid(z.im);
    double val = s_re * (1.0 - s_im) + (1.0 - s_re) * s_im;
    return val > 0.5 ? 1 : 0;
}

static int sector_classify(Cx z, int k) {
    double angle, sector_width;
    int sector;
    if (cx_abs(z) < 1e-15) return 0;
    angle = atan2(z.im, z.re);
    if (angle < 0.0) angle += 2.0 * M_PI;
    sector_width = 2.0 * M_PI / (double)k;
    sector = (int)(angle / sector_width);
    if (sector >= k) sector = k - 1;
    return sector % 2;
}

static int re_positive_classify(Cx z) { return z.re > 0.0 ? 1 : 0; }

static const int perms3[6][3] = {
    {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
};

static int npn_transform(int tt, const int sigma[3], int neg_in, int neg_out) {
    int result = 0, idx;
    for (idx = 0; idx < 8; idx++) {
        int x[3], y[3], src, out;
        x[0] = (idx >> 2) & 1;
        x[1] = (idx >> 1) & 1;
        x[2] = idx & 1;
        y[0] = x[sigma[0]] ^ ((neg_in >> 0) & 1);
        y[1] = x[sigma[1]] ^ ((neg_in >> 1) & 1);
        y[2] = x[sigma[2]] ^ ((neg_in >> 2) & 1);
        src = (y[0] << 2) | (y[1] << 1) | y[2];
        out = (tt >> src) & 1;
        if (neg_out) out ^= 1;
        result |= (out << idx);
    }
    return result;
}

static int npn_canon[256];

static void npn_init(void) {
    int i, pi, ni, no;
    for (i = 0; i < 256; i++) {
        int min_tt = i;
        for (pi = 0; pi < 6; pi++)
            for (ni = 0; ni < 8; ni++)
                for (no = 0; no < 2; no++) {
                    int t = npn_transform(i, perms3[pi], ni, no);
                    if (t < min_tt) min_tt = t;
                }
        npn_canon[i] = min_tt;
    }
}

/* ================================================================
 * Part B: demo 56
 * ================================================================ */

/* demo 56's ten printed values (its Part C, S(w,h) at Q = 2,
 * v = -zeta_16^6), coefficients of zeta_16^0..7 */
static const int  D56_W[10] = { 2, 2, 2, 2, 2, 3, 3, 3, 4, 4 };
static const int  D56_H[10] = { 2, 3, 4, 5, 6, 3, 4, 5, 3, 4 };
static const long D56_Z[10][8] = {
    { 14, 0, -8, 0, -24, 0, -32, 0 },
    { -84, 0, -278, 0, -322, 0, -170, 0 },
    { -3048, 0, -3464, 0, -1854, 0, 860, 0 },
    { -37582, 0, -20196, 0, 9060, 0, 32984, 0 },
    { -220448, 0, 96096, 0, 356354, 0, 407812, 0 },
    { -13774, 0, -9944, 0, -268, 0, 9560, 0 },
    { -162720, 0, 276366, 0, 553534, 0, 506434, 0 },
    { 20558690, 0, 23495912, 0, 12669556, 0, -5578384, 0 },
    { -162720, 0, 276366, 0, 553534, 0, 506434, 0 },
    { 93241634, 0, 67842368, 0, 2702072, 0, -64021104, 0 }
};

/* demo 56's published Part G (its run, unmodified): reachable classes
 * and parity (XNOR3, 0x69) solutions, 3-input; 2-input achievable */
#define N_ACT 5
static const char *const ACT_NAME[N_ACT] = {
    "Re(z) > 0", "Split-sigmoid", "Sector k=2", "Sector k=4", "Sector k=6"
};
static const int D56_REACH[N_ACT]  = { 11, 10, 12, 10, 11 };
static const int D56_PARITY[N_ACT] = { 113, 19, 32, 19, 32 };

/* exact Z[zeta_16] element -> Cx (double from the exact coefficients) and
 * an absolute error bound for that conversion */
static Cx
exact_to_cx (
    Cyclotomicus  z,
    Piscina      *pool,
    double       *bound)
{
    Cx     r = cx_make(0.0, 0.0);
    double maxabs = 0.0;
    int    k;

    for (k = 0; k < 8; k++) {
        Magnus m = cyclotomicus_coefficiens(z, (i32)k);
        s64    small;
        double c;

        if (magnus_ad_s64(m, &small)) {
            c = (double)small;
        } else {
            char   buf[256];
            chorda t = magnus_ad_chordam(m, pool);
            int    n = t.mensura < 255 ? (int)t.mensura : 255;

            memcpy(buf, t.datum, (size_t)n);
            buf[n] = '\0';
            c = strtod(buf, NULL);
        }
        if (fabs(c) > maxabs)
            maxabs = fabs(c);
        r.re += c * cos((double)k * M_PI / 8.0);
        r.im += c * sin((double)k * M_PI / 8.0);
    }
    /* 8 terms, each c (1 +- 2^-52) times cos/sin (1 +- 2^-52), summed:
     * a generous 64 ulp of the largest term */
    *bound = 64.0 * maxabs * 2.220446049250313e-16;
    return r;
}

/* demo 56's classifier for activation a, on a Cx */
static int
classify_float (
    int act,
    Cx  z)
{
    switch (act) {
    case 0:  return re_positive_classify(z);
    case 1:  return split_sigmoid_classify(z);
    case 2:  return sector_classify(z, 2);
    case 3:  return sector_classify(z, 4);
    default: return sector_classify(z, 6);
    }
}

/* The classification of the EXACT value z under demo 56's activation a.
 * Away from a boundary (by more than the conversion bound) the float
 * classification of the exact value is right. Near one, exact
 * predicates decide when z lies ON an axis (z = conj z: real; z =
 * -conj z: imaginary); the 60-degree lines of sector k = 6 cannot be hit
 * exactly by a nonzero element of Q(zeta_16), since e^{2 pi i/3} is not
 * in that field. *decided = 0 if z is near a boundary but not on one;
 * *float_class = what demo 56's float classification of z gives. */
static int
classify_exact (
    int           act,
    Cyclotomicus  z,
    Piscina      *pool,
    int          *decided,
    int          *float_class)
{
    double bound;
    Cx     c = exact_to_cx(z, pool, &bound);
    int    near;

    *decided     = 1;
    *float_class = classify_float(act, c);
    if (act == 0) {
        near = fabs(c.re) <= bound;
    } else if (act == 1) {
        near = fabs(c.re) <= bound || fabs(c.im) <= bound;
    } else {
        int    k = act == 2 ? 2 : act == 3 ? 4 : 6;
        double angle = atan2(c.im, c.re), width = 2.0 * M_PI / (double)k;
        double d;

        if (angle < 0.0) angle += 2.0 * M_PI;
        d = fmod(angle, width);
        if (width - d < d) d = width - d;
        near = cx_abs(c) * d <= bound;
    }
    if (!near)
        return *float_class;
    {
        Cyclotomicus conj = cyclotomicus_conjugatum(z, pool);
        int          real = cyclotomicus_aequalis(z, conj);
        int          imag = cyclotomicus_aequalis(z,
            cyclotomicus_nega(conj, pool));
        int          q;    /* angle = q pi/2 */

        if (real && imag)            /* z = 0 */
            return 0;
        if (!real && !imag) {
            *decided = 0;
            return *float_class;
        }
        if (act == 0)
            return real ? c.re > 0.0 : 0;
        if (act == 1)
            return 0;   /* a zero coordinate: value exactly 1/2, not > 1/2 */
        /* on an axis the other coordinate is large: its float sign is
         * safe */
        if (real)
            q = c.re > 0.0 ? 0 : 2;
        else
            q = c.im > 0.0 ? 1 : 3;
        {
            int k = act == 2 ? 2 : act == 3 ? 4 : 6;

            /* sector = floor(angle / (2 pi / k)) = floor(q k / 4) */
            return ((q * k) / 4) % 2;
        }
    }
}

static void
part_b (
    Piscina *keep,
    Piscina *work)
{
    const Cyclotomia *r16 = cyclotomia_creare(16, keep);
    Cyclotomicus      catalog[10];
    Cyclotomicus      v, two;
    int               n_cat = 0, t, a;
    int               catalog_bad = 0;
    char              msg[240];

    printf("\n=== Part B: demo 56, exact products in its Boolean search ===\n");
    v   = cyclotomicus_nega(cyclotomicus_radix(r16, 6, keep), keep);
    two = cyclotomicus_integer(r16, magnus_ex_s64(2), keep);

    /* the catalog, recomputed: tally (bonds, components) over all edge
     * subsets, then Z = sum count v^bonds 2^components exactly */
    for (t = 0; t < 10; t++) {
        LatticeStrip   g;
        static long    tally[MAX_EDGES + 1][MAX_SITES + 1];
        unsigned long  mask, total;
        Cyclotomicus   z = cyclotomicus_nullum(r16), d56;
        s64            c[8];
        int            b, k, j, dup = 0;

        build_strip(&g, D56_W[t], D56_H[t]);
        memset(tally, 0, sizeof(tally));
        total = 1UL << g.n_edges;
        for (mask = 0; mask < total; mask++) {
            int bonds = 0, comps = 0, i;

            fk_uf_init(g.n_sites);
            for (i = 0; i < g.n_edges; i++)
                if (mask & (1UL << i)) {
                    fk_uf_union(g.edge_u[i], g.edge_v[i]);
                    bonds++;
                }
            for (i = 0; i < g.n_sites; i++)
                if (fk_uf_find(i) == i)
                    comps++;
            tally[bonds][comps]++;
        }
        for (b = 0; b <= g.n_edges; b++)
            for (k = 0; k <= g.n_sites; k++) {
                if (tally[b][k] == 0)
                    continue;
                z = cyclotomicus_adde(z, cyclotomicus_multiplica(
                    cyclotomicus_integer(r16, magnus_ex_s64(tally[b][k]), keep),
                    cyclotomicus_multiplica(cyclotomicus_potentia(v, (i32)b,
                    keep), cyclotomicus_potentia(two, (i32)k, keep), keep), keep),
                    keep);
            }
        for (j = 0; j < 8; j++)
            c[j] = (s64)D56_Z[t][j];
        (void)cyclotomicus_ex_s64(r16, c, 8, keep, &d56);
        if (!cyclotomicus_aequalis(z, d56))
            catalog_bad++;
        for (j = 0; j < n_cat; j++)
            if (cyclotomicus_aequalis(catalog[j], z))
                dup = 1;
        if (!dup && !cyclotomicus_est_nullum(z))
            catalog[n_cat++] = z;
    }
    sprintf(msg, "demo 56's ten partition functions recomputed exactly "
        "(%d differ); %d distinct nonzero (demo 56: 9)", catalog_bad, n_cat);
    check(msg, catalog_bad == 0 && n_cat == 9);

    npn_init();
    for (a = 0; a < N_ACT; a++) {
        long          tt3[256];
        int           tt2[16];
        long          undecided = 0, parity = 0, overflow_triples = 0;
        long          float_wrong = 0;
        Cyclotomicus  one = cyclotomicus_integer(r16, magnus_ex_s64(1), keep);
        int           reach = 0, achievable = 0, i1, i2, i3, i, j;
        int           seen[256];

        memset(tt3, 0, sizeof(tt3));
        memset(tt2, 0, sizeof(tt2));
        memset(seen, 0, sizeof(seen));
        for (i1 = 0; i1 < n_cat; i1++) {
            for (i2 = 0; i2 < n_cat; i2++) {
                PiscinaNotatio mark = piscina_notare(work);
                Cyclotomicus   w12 = cyclotomicus_multiplica(catalog[i1],
                    catalog[i2], work);
                int            bits2 = 0, d, fc, cl;

                /* 2-input: inputs 1, w2, w1, w1 w2 */
                cl = classify_exact(a, one, work, &d, &fc);
                if (cl) bits2 |= 1;
                cl = classify_exact(a, catalog[i2], work, &d, &fc);
                if (cl) bits2 |= 2;
                cl = classify_exact(a, catalog[i1], work, &d, &fc);
                if (cl) bits2 |= 4;
                cl = classify_exact(a, w12, work, &d, &fc);
                if (cl) bits2 |= 8;
                if (a != 2 && a != 4)
                    tt2[bits2]++;
                for (i3 = 0; i3 < n_cat; i3++) {
                    Cyclotomicus w13 = cyclotomicus_multiplica(catalog[i1],
                        catalog[i3], work);
                    Cyclotomicus w23 = cyclotomicus_multiplica(catalog[i2],
                        catalog[i3], work);
                    Cyclotomicus w123 = cyclotomicus_multiplica(w12,
                        catalog[i3], work);
                    s64          probe[8];
                    Cyclotomicus in[8];
                    int          bits = 0, ok = 1, m;

                    if (!cyclotomicus_ad_s64(w123, probe))
                        overflow_triples++;
                    in[0] = one;
                    in[1] = catalog[i3];
                    in[2] = catalog[i2];
                    in[3] = w23;
                    in[4] = catalog[i1];
                    in[5] = w13;
                    in[6] = w12;
                    in[7] = w123;
                    for (m = 0; m < 8; m++) {
                        int d3, fc3, cl3 = classify_exact(a, in[m], work, &d3,
                            &fc3);

                        if (cl3) bits |= 1 << m;
                        ok &= d3;
                        if (cl3 != fc3)
                            float_wrong++;
                    }
                    if (!ok)
                        undecided++;
                    tt3[bits]++;
                }
                piscina_reficere(work, mark);
            }
        }
        for (i = 0; i < 256; i++) {
            if (i == 0 || i == 255 || tt3[i] == 0)
                continue;
            seen[npn_canon[i]] = 1;
            if (npn_canon[i] == 0x69)
                parity += tt3[i];
        }
        for (i = 0; i < 256; i++)
            reach += seen[i];
        if (a == 2) {
            /* per class, to compare with demo 56's published table */
            long per_class[256];

            memset(per_class, 0, sizeof(per_class));
            for (i = 1; i < 255; i++)
                per_class[npn_canon[i]] += tt3[i];
            printf("  Sector k=2, exact solutions per NPN class:");
            for (i = 1; i < 255; i++)
                if (npn_canon[i] == i)
                    printf(" 0x%02X:%ld", (unsigned)i, per_class[i]);
            printf("\n");
            /* demo 56 published 0x06 ~A(B^C): 3, AND3' 20, AND2' 114,
             * ~A~(BC) 57, BUF 69 */
            sprintf(msg, "Sector k=2: demo 56's twelfth class ~A(B^C) "
                "(0x06, 3 solutions) has 0 exact solutions - an overflow "
                "artifact; AND3' %ld (20), AND2' %ld (114), ~A~(BC) %ld "
                "(57), BUF %ld (69)", per_class[0x01], per_class[0x03],
                per_class[0x07], per_class[0x0F]);
            check(msg, per_class[0x06] == 0);
        }
        for (j = 0; j < 16; j++)
            achievable += tt2[j] > 0;
        printf("  %-14s 3-input: %2d / 13 classes, parity %3ld solutions "
            "(demo 56: %2d, %3d); w1 w2 w3 beyond long: %ld of %d; "
            "undecided: %ld; float misclassifications of exact values: %ld",
            ACT_NAME[a], reach, parity, D56_REACH[a], D56_PARITY[a],
            overflow_triples, n_cat * n_cat * n_cat, undecided, float_wrong);
        if (a != 2 && a != 4)
            printf("; 2-input %d / 16", achievable);
        printf("\n");
        sprintf(msg, "%s: no classification within float error of a "
            "boundary (exact values decide); demo 56's float classification "
            "of every exact value agrees", ACT_NAME[a]);
        check(msg, undecided == 0 && float_wrong == 0);
        sprintf(msg, "%s: parity survives exactly (%ld solutions, demo 56 "
            "%d)", ACT_NAME[a], parity, D56_PARITY[a]);
        check(msg, parity == D56_PARITY[a]);
        if (a == 2) {
            sprintf(msg, "Sector k=2 reaches %d of 13 classes exactly, not "
                "demo 56's %d (its headline '12 of 13 including parity')",
                reach, D56_REACH[a]);
            check(msg, reach == 11 && D56_REACH[a] == 12);
        } else {
            sprintf(msg, "%s: %d of 13 classes, as demo 56 (overflowed "
                "triples did not change this total)", ACT_NAME[a], reach);
            check(msg, reach == D56_REACH[a]);
        }
    }
}

/* ================================================================
 * main
 * ================================================================ */

int
main (void)
{
    Piscina *keep = piscina_generare_dynamicum("d117_keep", 1 << 22);
    Piscina *work = piscina_generare_dynamicum("d117_work", 1 << 22);

    printf("KNOTAPEL DEMO 117: Exact Audit of the Cyclotomic Demos\n");
    printf("======================================================\n");
    if (keep == NULL || work == NULL) {
        printf("pool allocation failed\n");
        return 1;
    }
    part_a(keep, work);
    part_b(keep, work);
    printf("\n======================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    piscina_destruere(work);
    piscina_destruere(keep);
    return n_fail > 0 ? 1 : 0;
}
