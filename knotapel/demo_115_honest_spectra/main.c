/*
 * KNOTAPEL DEMO 115: Honest Construction-Word Spectra
 * ================================================================
 *
 * Demo 114 showed that Demo 112's construction-word results rest on
 * singular polygons (10 of its 12 simplified "knots" pass through
 * themselves) and on `long` overflow (the 6_3 "knot factory"). This demo
 * asks D112's questions again on HONEST polygons:
 *
 *   - inputs: D112's 12 raw braid polygons (verified by D114: simple, table
 *     Alexander polynomial), embedded verbatim from D114's d112_export.txt;
 *   - each is simplified by laqueus_simplificare (legal triangle moves
 *     only: an isotopy, the knot is unchanged), from every start vertex
 *     and both orientations; the main polygon is the variant with the
 *     fewest vertices (ties: first by orientation, then start vertex);
 *   - a construction word = vertex k >= 3 reflected through the plane of
 *     vertices 0, 1, 2 iff bit k-3 is set (situs_reflexio), all 2^(n-3)
 *     alternatives computed exactly: simplicity, Alexander, Jones.
 *
 *   Part A  base polygons: variants, the chosen honest polygon, reference
 *           Alexander + Jones per knot (from the raw braid polygon)
 *   Part B  the spectrum of every knot, classes named by Alexander+Jones
 *           (K = D112's braid chirality, K* = its mirror), the mirror
 *           theorem asserted (alternative c XOR all-ones is the mirror
 *           image of alternative c: vertices 0, 1, 2 lie on the plane)
 *   Part C  reachability among the 12 knots, and the "explosion"
 *           question: do alternatives exceed the source's determinant?
 *   Part D  stability: the spectra of ALL fewest-vertex variants of 6_3
 *           and 4_1 (equally honest polygons of the same knot, same size)
 *
 * Jones: on the first generic projection if <= 16 crossings, else on the
 * fewest-crossing projection found with direction components in -1..1,
 * then -2..2 (laqueus_diagramma_minimum), if that has <= 20 crossings;
 * otherwise reported missing. Unknots are certified: Alexander 1 on a
 * diagram of <= 10 crossings (no nontrivial knot of crossing number <= 10
 * has trivial Alexander polynomial; Rolfsen / KnotInfo: the first are
 * 11n34 and 11n42).
 *
 * House libraries: includes laqueus.h, hence latina.h (Roman numerals and
 * Latin keywords are macros here). Build/run from the repo root:
 *   ./bin/aedilis knotapel/demo_115_honest_spectra/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * Frozen copy: demo-snapshot.c (knotapel/archive.sh).
 */

#include "laqueus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * Data
 *
 * RAW_DATA: the "RAW" lines of knotapel/demo_114_exact_audit/
 * d112_export.txt (Demo 112's braid_to_polygon output, exported by
 * compiling D112's main.c unmodified), verbatim.
 * ================================================================ */

static const char *const RAW_DATA[] = {
    "RAW 3_1 18 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 20,-100,60 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 0,-105,60 0,-105,0",
    "RAW 4_1 29 0,0,0 20,1,10 20,0,20 40,-1,30 40,0,40 40,0,60 20,1,70 20,0,80 20,-100,80 20,-100,0 20,0,0 0,-1,10 0,0,20 0,0,40 20,1,50 20,0,60 40,-1,70 40,0,80 40,-105,80 40,-105,0 40,0,0 40,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 0,0,80 0,-110,80 0,-110,0",
    "RAW 5_1 26 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 20,1,90 20,0,100 20,-100,100 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,-105,100 0,-105,0",
    "RAW 5_2 37 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 20,-1,50 20,0,60 40,-1,70 40,0,80 40,0,100 20,1,110 20,0,120 20,-100,120 20,-100,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,1,90 20,0,100 40,-1,110 40,0,120 40,-105,120 40,-105,0 40,0,0 40,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,0,120 0,-110,120 0,-110,0",
    "RAW 6_1 48 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 0,0,60 20,1,70 20,0,80 20,0,100 40,-1,110 40,0,120 60,1,130 60,0,140 60,-100,140 60,-100,0 60,0,0 60,0,80 40,-1,90 40,0,100 20,1,110 20,0,120 20,0,140 20,-105,140 20,-105,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 40,-1,50 40,0,60 40,0,80 60,1,90 60,0,100 60,0,120 40,-1,130 40,0,140 40,-110,140 40,-110,0 40,0,0 40,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 0,0,140 0,-115,140 0,-115,0",
    "RAW 6_2 37 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 20,-1,50 20,0,60 40,1,70 40,0,80 40,0,100 20,-1,110 20,0,120 20,-100,120 20,-100,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 40,-105,120 40,-105,0 40,0,0 40,0,60 20,-1,70 20,0,80 0,1,90 0,0,100 0,0,120 0,-110,120 0,-110,0",
    "RAW 6_3 37 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 0,0,60 20,-1,70 20,0,80 40,1,90 40,0,100 20,-1,110 20,0,120 20,-100,120 20,-100,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 40,1,50 40,0,60 40,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 40,-105,120 40,-105,0 40,0,0 40,0,40 20,-1,50 20,0,60 0,1,70 0,0,80 0,0,120 0,-110,120 0,-110,0",
    "RAW 7_1 34 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 20,1,90 20,0,100 0,-1,110 0,0,120 20,1,130 20,0,140 20,-100,140 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 20,1,110 20,0,120 0,-1,130 0,0,140 0,-105,140 0,-105,0",
    "RAW 7_2 56 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 20,-1,50 20,0,60 40,-1,70 40,0,80 40,0,100 20,1,110 20,0,120 20,0,140 40,1,150 40,0,160 60,-1,170 60,0,180 60,-100,180 60,-100,0 60,0,0 60,0,120 40,1,130 40,0,140 20,-1,150 20,0,160 20,0,180 20,-105,180 20,-105,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,1,90 20,0,100 40,-1,110 40,0,120 60,-1,130 60,0,140 60,0,160 40,1,170 40,0,180 40,-110,180 40,-110,0 40,0,0 40,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,0,180 0,-115,180 0,-115,0",
    "RAW 7_3 45 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 20,1,90 20,0,100 40,1,110 40,0,120 40,0,140 20,-1,150 20,0,160 20,-100,160 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,0,120 20,-1,130 20,0,140 40,1,150 40,0,160 40,-105,160 40,-105,0 40,0,0 40,0,100 20,-1,110 20,0,120 0,1,130 0,0,140 0,0,160 0,-110,160 0,-110,0",
    "RAW 7_4 56 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 0,0,60 20,-1,70 20,0,80 40,1,90 40,0,100 20,-1,110 20,0,120 20,0,140 40,-1,150 40,0,160 60,1,170 60,0,180 60,-100,180 60,-100,0 60,0,0 60,0,120 40,-1,130 40,0,140 20,1,150 20,0,160 20,0,180 20,-105,180 20,-105,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 40,1,50 40,0,60 40,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 60,1,130 60,0,140 60,0,160 40,-1,170 40,0,180 40,-110,180 40,-110,0 40,0,0 40,0,40 20,-1,50 20,0,60 0,1,70 0,0,80 0,0,180 0,-115,180 0,-115,0",
    "RAW 8_18 49 0,0,0 20,-1,10 20,0,20 40,1,30 40,0,40 40,0,60 20,-1,70 20,0,80 0,1,90 0,0,100 0,0,120 20,-1,130 20,0,140 40,1,150 40,0,160 40,-100,160 40,-100,0 40,0,0 40,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 40,0,140 20,-1,150 20,0,160 20,-105,160 20,-105,0 20,0,0 0,1,10 0,0,20 0,0,40 20,-1,50 20,0,60 40,1,70 40,0,80 40,0,100 20,-1,110 20,0,120 0,1,130 0,0,140 0,0,160 0,-110,160 0,-110,0",
    NULL
};

#define N_KNOTS 12

static const char *const KNOT_CODE[N_KNOTS] = {
    "3_1", "4_1", "5_1", "5_2", "6_1", "6_2",
    "6_3", "7_1", "7_2", "7_3", "7_4", "8_18"
};

/* table Alexander polynomials (laqueus normal form) */
static const char *const KNOT_ALEXANDER[N_KNOTS] = {
    "t^2 - t + 1",
    "t^2 - 3t + 1",
    "t^4 - t^3 + t^2 - t + 1",
    "2t^2 - 3t + 2",
    "2t^2 - 5t + 2",
    "t^4 - 3t^3 + 3t^2 - 3t + 1",
    "t^4 - 3t^3 + 5t^2 - 3t + 1",
    "t^6 - t^5 + t^4 - t^3 + t^2 - t + 1",
    "3t^2 - 5t + 3",
    "2t^4 - 3t^3 + 3t^2 - 3t + 2",
    "4t^2 - 7t + 4",
    "t^6 - 5t^5 + 10t^4 - 13t^3 + 10t^2 - 5t + 1"
};

/* table determinants |Delta(-1)| */
static const unsigned long KNOT_DET[N_KNOTS] = {
    3, 5, 5, 7, 9, 11, 13, 7, 11, 13, 15, 45
};

/* D112 (published) and D114 (exact, on D112's singular-base polygons):
 * distinct Alexander polynomials and reachability out-degree */
static const unsigned D112_DISTINCT[N_KNOTS] = {
    2, 4, 2, 4, 1, 4, 37, 2, 68, 1, 1, 1
};
static const unsigned D114_DISTINCT[N_KNOTS] = {
    1, 3, 1, 3, 0, 3, 6, 1, 7, 0, 0, 0
};
static const unsigned D112_OUT_DEGREE[N_KNOTS] = {
    1, 3, 1, 3, 1, 3, 10, 1, 9, 1, 1, 1
};

#define JONES_CAP_FIRST   16
#define JONES_CAP_SEARCH  20
#define JONES_CAP_REF     22
#define UNKNOT_CERT       10
#define MAX_VERTS        128
#define MAX_ALTS        4096
#define MAX_CLASSES      256

/* ================================================================
 * Small helpers
 * ================================================================ */

static chorda
empty_chorda (void)
{
    chorda c;

    c.datum   = NULL;
    c.mensura = 0;
    return c;
}

static int
chorda_is (
    chorda      c,
    const char *lit)
{
    return c.datum != NULL && chorda_aequalis_literis(c, lit);
}

static int
chorda_same (
    chorda a,
    chorda b)
{
    return a.datum != NULL && b.datum != NULL && chorda_aequalis(a, b);
}

static void
print_chorda (
    chorda c)
{
    if (c.datum == NULL)
        printf("-");
    else
        printf("%.*s", (int)c.mensura, (const char*)c.datum);
}

static unsigned long
chorda_to_ulong (
    chorda c)
{
    char buf[32];
    int  k;
    int  n = c.mensura < 31 ? (int)c.mensura : 31;

    for (k = 0; k < n; k++)
        buf[k] = (char)c.datum[k];
    buf[n] = '\0';
    return strtoul(buf, NULL, 10);
}

/* "RAW <code> <n> x,y,z ..." -> vertex array */
static long
raw_polygon (
    const char *code,
    Piscina    *pool,
    Punctum   **out)
{
    size_t lc = strlen(code);
    int    k;

    for (k = 0; RAW_DATA[k] != NULL; k++) {
        const char *s = RAW_DATA[k];

        if (strncmp(s, "RAW ", 4) == 0 && strncmp(s + 4, code, lc) == 0
            && s[4 + lc] == ' ') {
            char *end;
            long  n = strtol(s + 5 + lc, &end, 10);
            long  j;

            if (n < 3 || n > MAX_VERTS)
                return 0;
            *out = (Punctum*)piscina_allocare(pool,
                (memoriae_index)n * sizeof(Punctum));
            for (j = 0; j < n; j++) {
                long x, y, z;

                x = strtol(end, &end, 10);
                y = strtol(end + 1, &end, 10);
                z = strtol(end + 1, &end, 10);
                (*out)[j] = situs_punctum((s64)x, (s64)y, (s64)z);
            }
            return n;
        }
    }
    return 0;
}

static int
make_knot (
    const Punctum *pts,
    long           n,
    Piscina       *pool,
    Laqueus       *out)
{
    i32 starts[2];

    starts[0] = 0;
    starts[1] = (i32)n;
    return laqueus_ex_punctis(pts, starts, 1, pool, out) ? 1 : 0;
}

/* the raw polygon started at vertex `shift`, reversed if rev, then
 * simplified by legal moves */
static int
variant (
    const Punctum *raw,
    long           n,
    long           shift,
    int            rev,
    Piscina       *pool,
    Laqueus       *out)
{
    Punctum *v = (Punctum*)piscina_allocare(pool,
        (memoriae_index)n * sizeof(Punctum));
    Laqueus  l;
    long     j;

    for (j = 0; j < n; j++)
        v[j] = raw[rev ? (shift - j + n) % n : (shift + j) % n];
    return make_knot(v, n, pool, &l) && laqueus_simplificare(l, pool, out);
}

/* alternative `choices` of base: vertex k >= 3 reflected through the
 * plane of vertices 0, 1, 2 iff bit k-3 is set; 0 if the reflection is
 * undefined (vertices 0, 1, 2 collinear) */
static int
build_alternative (
    Laqueus        base,
    unsigned long  choices,
    Piscina       *pool,
    Laqueus       *out)
{
    i32      n   = laqueus_numerus(base);
    Punctum *pts = (Punctum*)piscina_allocare(pool,
        (memoriae_index)n * sizeof(Punctum));
    Punctum  c0  = laqueus_vertex(base, 0);
    Punctum  c1  = laqueus_vertex(base, 1);
    Punctum  c2  = laqueus_vertex(base, 2);
    i32      k;
    int      ok  = 1;

    for (k = 0; k < n; k++) {
        Punctum v = laqueus_vertex(base, k);

        if (k >= 3 && ((choices >> (k - 3)) & 1UL)) {
            if (!situs_reflexio(c0, c1, c2, v, pool, &pts[k])) {
                ok     = 0;
                pts[k] = v;
            }
        } else {
            pts[k] = v;
        }
    }
    return make_knot(pts, (long)n, pool, out) && ok;
}

/* ================================================================
 * Exact classification
 * ================================================================ */

typedef struct {
    int           simple;
    unsigned      crossings;   /* projection Jones was computed on */
    unsigned long det;
    unsigned      alex_span;   /* degree of the normalized Alexander */
    int           unknot_certified;
    chorda        alexander;
    chorda        jones;       /* NULL: not computed */
    chorda        jones_mirror;
    int           jones_filled; /* taken from the mirror partner */
} Exact;

/* jones_cap 0: Alexander only */
static Exact
classify (
    Laqueus   l,
    unsigned  jones_cap,
    Piscina  *scratch,
    Piscina  *out_pool)
{
    PiscinaNotatio mark = piscina_notare(scratch);
    Exact          e;
    Diagramma      d;
    Polynomium     p;

    e.simple           = laqueus_simplex(l, scratch) ? 1 : 0;
    e.crossings        = 0;
    e.det              = 0;
    e.alex_span        = 0;
    e.unknot_certified = 0;
    e.alexander        = empty_chorda();
    e.jones            = empty_chorda();
    e.jones_mirror     = empty_chorda();
    e.jones_filled     = 0;
    if (e.simple && laqueus_diagramma_genericum(l, scratch, &d)) {
        Magnus m;

        e.crossings = (unsigned)diagramma_numerus(d);
        if (diagramma_alexander(d, scratch, &p)) {
            e.alexander = chorda_transcribere(
                polynomium_ad_chordam(p, 't', scratch), out_pool);
            e.alex_span = (unsigned)(polynomium_gradus_summus(p)
                - polynomium_gradus_imus(p));
        }
        if (diagramma_determinans(d, scratch, &m))
            e.det = chorda_to_ulong(magnus_ad_chordam(m, scratch));
        if (jones_cap > 0) {
            int need_cert = chorda_is(e.alexander, "1")
                && e.crossings > UNKNOT_CERT;

            if (e.crossings > JONES_CAP_FIRST || need_cert) {
                Diagramma dm;

                if (laqueus_diagramma_minimum(l, 1, scratch, &dm)
                    && (unsigned)diagramma_numerus(dm) < e.crossings) {
                    d           = dm;
                    e.crossings = (unsigned)diagramma_numerus(dm);
                }
                if ((e.crossings > JONES_CAP_FIRST
                        || (need_cert && e.crossings > UNKNOT_CERT))
                    && laqueus_diagramma_minimum(l, 2, scratch, &dm)
                    && (unsigned)diagramma_numerus(dm) < e.crossings) {
                    d           = dm;
                    e.crossings = (unsigned)diagramma_numerus(dm);
                }
            }
            if (e.crossings <= jones_cap && diagramma_jones(d, scratch, &p)) {
                e.jones = chorda_transcribere(
                    polynomium_ad_chordam(p, 't', scratch), out_pool);
                e.jones_mirror = chorda_transcribere(polynomium_ad_chordam(
                    polynomium_inversum(p, scratch), 't', scratch), out_pool);
            }
            e.unknot_certified = chorda_is(e.alexander, "1")
                && e.crossings <= UNKNOT_CERT;
        }
    }
    piscina_reficere(scratch, mark);
    return e;
}

static unsigned reflexio_failures = 0;

static Exact
classify_alternative (
    Laqueus        base,
    unsigned long  choices,
    unsigned       jones_cap,
    Piscina       *scratch,
    Piscina       *out_pool)
{
    PiscinaNotatio mark = piscina_notare(scratch);
    Laqueus        alt;
    Exact          e;

    if (!build_alternative(base, choices, scratch, &alt))
        reflexio_failures++;
    e = classify(alt, jones_cap, scratch, out_pool);
    piscina_reficere(scratch, mark);
    return e;
}

/* ================================================================
 * Naming against the 12 reference knots
 * ================================================================ */

typedef struct {
    chorda alexander;
    chorda jones;          /* the raw braid polygon's (D112's chirality) */
    chorda jones_mirror;
} Reference;

static Reference refs[N_KNOTS];

/* Beyond the 12: the trefoil composites, from the reference trefoil's
 * Jones (V is multiplicative under connected sum; Alexander of both
 * composites is (t^2 - t + 1)^2), and 8_20, whose Alexander is the same
 * (t^2 - t + 1)^2. 8_20's Jones is taken from the Knot Atlas page 8_20
 * (fetched 2026-10-07; q = t): -q + 2 - q^-1 + 2q^-2 - q^-3 + q^-4 -
 * q^-5. The Knot Atlas 3_1 page has the same chirality as D112's braid
 * trefoil (Jones t^-1 + t^-3 - t^-4), so "8_20" below is the Atlas
 * diagram's chirality. External table value: not verified by this
 * demo. */
#define N_EXTRA 5
#define ALEX_TREFOIL_SQUARED "t^4 - 2t^3 + 3t^2 - 2t + 1"
#define JONES_8_20_ATLAS "-t + 2 - t^-1 + 2t^-2 - t^-3 + t^-4 - t^-5"

typedef struct {
    char   name[16];
    chorda jones;
} Extra;

static Extra extras[N_EXTRA];

static int
init_extras (
    Piscina *keep)
{
    Polynomium j, jm, q, a;

    if (!polynomium_ex_chorda(refs[0].jones, 't', keep, &j)
        || !polynomium_ex_chorda(refs[0].jones_mirror, 't', keep, &jm)
        || !polynomium_ex_chorda(chorda_ex_literis(JONES_8_20_ATLAS, keep),
               't', keep, &a))
        return 0;
    strcpy(extras[0].name, "3_1#3_1");
    if (!polynomium_multiplica(j, j, keep, &q)) return 0;
    extras[0].jones = polynomium_ad_chordam(q, 't', keep);
    strcpy(extras[1].name, "3_1*#3_1*");
    if (!polynomium_multiplica(jm, jm, keep, &q)) return 0;
    extras[1].jones = polynomium_ad_chordam(q, 't', keep);
    strcpy(extras[2].name, "3_1#3_1*");
    if (!polynomium_multiplica(j, jm, keep, &q)) return 0;
    extras[2].jones = polynomium_ad_chordam(q, 't', keep);
    strcpy(extras[3].name, "8_20");
    extras[3].jones = polynomium_ad_chordam(a, 't', keep);
    strcpy(extras[4].name, "8_20*");
    extras[4].jones = polynomium_ad_chordam(polynomium_inversum(a, keep),
        't', keep);
    return 1;
}

/* writes a name into buf. A name means "Alexander and Jones both match"
 * - not a proof of the knot type (e.g. 5_1 and 10_132 share both).
 * Returns the knot index 0..11, -2 unknot, -3 a named knot beyond the
 * 12 (composites, 8_20), -1 unnamed. */
static int
name_class (
    Exact  e,
    char  *buf)
{
    int k;

    if (e.unknot_certified && chorda_is(e.jones, "1")) {
        strcpy(buf, "unknot");
        return -2;
    }
    if (chorda_is(e.alexander, "1")) {
        /* trivial Alexander, no <= 10-crossing diagram found: not
         * certified (Jones 1 is consistent with, not proof of, the
         * unknot) */
        strcpy(buf, chorda_is(e.jones, "1") ? "unknot?" : "?");
        return -1;
    }
    for (k = 0; k < N_KNOTS; k++) {
        if (!chorda_same(e.alexander, refs[k].alexander))
            continue;
        if (chorda_same(e.jones, refs[k].jones)) {
            strcpy(buf, KNOT_CODE[k]);
            return k;
        }
        if (chorda_same(e.jones, refs[k].jones_mirror)) {
            sprintf(buf, "%s*", KNOT_CODE[k]);
            return k;
        }
    }
    if (chorda_is(e.alexander, ALEX_TREFOIL_SQUARED)) {
        for (k = 0; k < N_EXTRA; k++) {
            if (chorda_same(e.jones, extras[k].jones)) {
                strcpy(buf, extras[k].name);
                return -3;
            }
        }
    }
    strcpy(buf, "?");
    return -1;
}

/* ================================================================
 * Spectrum: classes (Alexander | Jones) with populations
 * ================================================================ */

typedef struct {
    chorda        key;
    chorda        alexander;
    chorda        jones;
    unsigned      count;
    int           knot;        /* index, -1 unnamed, -2 unknot */
    char          name[16];
    unsigned long det;
    unsigned      span;
} Class;

typedef struct {
    Class    c[MAX_CLASSES];
    unsigned n;
    unsigned overflow;
} Spectrum;

static void
spectrum_clear (
    Spectrum *s)
{
    s->n        = 0;
    s->overflow = 0;
}

/* Alexander | Jones (or "?"), and for a trivial Alexander polynomial
 * whether the unknot is certified - so every member of a class has the
 * same name */
static chorda
class_key (
    Exact    e,
    Piscina *pool)
{
    chorda bar = chorda_ex_literis("  |  ", pool);
    chorda j   = e.jones.datum != NULL ? e.jones
               : chorda_ex_literis("?", pool);
    chorda k   = chorda_concatenare(chorda_concatenare(e.alexander, bar,
        pool), j, pool);

    if (chorda_is(e.alexander, "1"))
        k = chorda_concatenare(k, chorda_ex_literis(e.unknot_certified
            ? "  | certified" : "  | uncertified", pool), pool);
    return k;
}

static void
spectrum_add (
    Spectrum *s,
    Exact     e,
    Piscina  *work,
    Piscina  *keep)
{
    chorda   key = class_key(e, work);
    unsigned k;

    for (k = 0; k < s->n; k++) {
        if (chorda_aequalis(s->c[k].key, key)) {
            s->c[k].count++;
            return;
        }
    }
    if (s->n >= MAX_CLASSES) {
        s->overflow++;
        return;
    }
    s->c[s->n].key       = chorda_transcribere(key, keep);
    s->c[s->n].alexander = chorda_transcribere(e.alexander, keep);
    s->c[s->n].jones     = e.jones.datum != NULL
        ? chorda_transcribere(e.jones, keep) : empty_chorda();
    s->c[s->n].count     = 1;
    s->c[s->n].det       = e.det;
    s->c[s->n].span      = e.alex_span;
    s->c[s->n].knot      = name_class(e, s->c[s->n].name);
    s->n++;
}

/* ================================================================
 * Part A: base polygons
 * ================================================================ */

static Laqueus  chosen[N_KNOTS];
static Punctum *raw_pts[N_KNOTS];
static long     raw_n[N_KNOTS];
static int      fewest[N_KNOTS];

static void
part_a (
    Piscina *keep,
    Piscina *scratch)
{
    int k;

    printf("\n=== Part A: honest polygons (legal simplification of D112's raw "
        "braid polygons) ===\n");
    printf("  %-5s %4s %8s %8s %10s %9s  %s\n", "knot", "raw", "default",
        "fewest", "#fewest", "chosen", "reference Jones (raw polygon)");
    for (k = 0; k < N_KNOTS; k++) {
        const char *code = KNOT_CODE[k];
        Laqueus     raw, def;
        Exact       er, ec;
        long        n, shift;
        int         rev, best = MAX_VERTS + 1, n_best = 0;
        long        best_shift = 0;
        int         best_rev = 0;
        char        msg[200];

        n = raw_polygon(code, keep, &raw_pts[k]);
        raw_n[k] = n;
        if (n == 0 || !make_knot(raw_pts[k], n, keep, &raw)) {
            check("raw polygon parses", 0);
            continue;
        }
        er = classify(raw, JONES_CAP_REF, scratch, keep);
        refs[k].alexander    = er.alexander;
        refs[k].jones        = er.jones;
        refs[k].jones_mirror = er.jones_mirror;
        (void)variant(raw_pts[k], n, 0, 0, keep, &def);
        for (rev = 0; rev < 2; rev++) {
            for (shift = 0; shift < n; shift++) {
                PiscinaNotatio mark = piscina_notare(keep);
                Laqueus        v;
                int            m;

                if (variant(raw_pts[k], n, shift, rev, keep, &v)) {
                    m = (int)laqueus_numerus(v);
                    if (m < best) {
                        best       = m;
                        n_best     = 1;
                        best_shift = shift;
                        best_rev   = rev;
                    } else if (m == best) {
                        n_best++;
                    }
                }
                piscina_reficere(keep, mark);
            }
        }
        fewest[k] = n_best;
        (void)variant(raw_pts[k], n, best_shift, best_rev, keep, &chosen[k]);
        ec = classify(chosen[k], JONES_CAP_REF, scratch, keep);
        printf("  %-5s %4ld %8u %8d %10d  %s%-6ld  ", code, n,
            (unsigned)laqueus_numerus(def), best, n_best,
            best_rev ? "r" : "+", best_shift);
        print_chorda(er.jones);
        printf("\n");
        sprintf(msg, "%s: raw polygon simple with the table Alexander "
            "polynomial and a Jones polynomial", code);
        check(msg, er.simple && chorda_is(er.alexander, KNOT_ALEXANDER[k])
            && er.jones.datum != NULL);
        sprintf(msg, "%s: honest polygon simple, same Alexander and Jones as "
            "the raw polygon", code);
        check(msg, ec.simple && chorda_same(ec.alexander, er.alexander)
            && chorda_same(ec.jones, er.jones));
    }
    printf("  (chosen: + forward / r reversed, start vertex; ties go to the "
        "first)\n");
    check("composite and 8_20 references built from the trefoil's Jones "
        "and the cited Knot Atlas value", init_extras(keep));
}

/* ================================================================
 * Part B: spectra
 * ================================================================ */

static Spectrum spectra[N_KNOTS];
static Exact    alt_exact[MAX_ALTS];

/* All alternatives of base classified (strings in keep). The mirror
 * theorem is CHECKED on independently computed pairs first (pair_bad);
 * only then is it USED: alternative c ^ mask is the mirror image of c, so
 * Jones(c) = Jones(c ^ mask)(1/t), and a diagram of one mirrors to a
 * diagram of the other with the same crossings (the unknot certificate
 * transfers). Exact, not an estimate. */
static unsigned long
classify_all (
    Laqueus   base,
    Piscina  *keep,
    Piscina  *scratch,
    unsigned *pair_bad,
    unsigned *filled)
{
    unsigned long count = 1UL << (laqueus_numerus(base) - 3);
    unsigned long mask  = count - 1UL;
    unsigned long c;

    *pair_bad = 0;
    *filled   = 0;
    for (c = 0; c < count; c++)
        alt_exact[c] = classify_alternative(base, c, JONES_CAP_SEARCH,
            scratch, keep);
    for (c = 0; c < count; c++) {
        const Exact *a = &alt_exact[c];
        const Exact *b = &alt_exact[c ^ mask];

        if (a->simple != b->simple)
            (*pair_bad)++;
        else if (a->simple
                 && (!chorda_same(a->alexander, b->alexander)
                     || (a->jones.datum != NULL && b->jones.datum != NULL
                         && !chorda_aequalis(a->jones, b->jones_mirror))))
            (*pair_bad)++;
    }
    for (c = 0; c < count; c++) {
        Exact       *a = &alt_exact[c];
        const Exact *b = &alt_exact[c ^ mask];

        if (!a->simple)
            continue;
        if (a->jones.datum == NULL && b->jones.datum != NULL
            && !b->jones_filled) {
            a->jones        = b->jones_mirror;
            a->jones_mirror = b->jones;
            a->jones_filled = 1;
            (*filled)++;
        }
        if (chorda_is(a->alexander, "1") && !a->unknot_certified
            && b->unknot_certified)
            a->unknot_certified = 1;
    }
    return count;
}

static void
part_b (
    Piscina *keep,
    Piscina *scratch,
    Piscina *work)
{
    int k;

    printf("\n=== Part B: construction-word spectra of the honest polygons "
        "===\n");
    printf("  (a name means Alexander AND Jones match the named knot - e.g. "
        "5_1 and 10_132 share both)\n");
    for (k = 0; k < N_KNOTS; k++) {
        const char   *code = KNOT_CODE[k];
        unsigned long count, c;
        unsigned      n_simple = 0, missing = 0, trivial = 0, cert = 0;
        unsigned      pair_bad, filled, self = 0, self_mirror = 0, q;
        unsigned      n_alex = 0;
        unsigned long max_det = 0;
        unsigned      max_span = 0;
        char          msg[200];
        Spectrum     *sp = &spectra[k];

        spectrum_clear(sp);
        if ((1UL << (laqueus_numerus(chosen[k]) - 3)) > MAX_ALTS) {
            sprintf(msg, "%s: spectrum within %d alternatives", code,
                MAX_ALTS);
            check(msg, 0);
            continue;
        }
        count = classify_all(chosen[k], keep, scratch, &pair_bad, &filled);
        for (c = 0; c < count; c++) {
            const Exact *e = &alt_exact[c];

            if (!e->simple)
                continue;
            n_simple++;
            spectrum_add(sp, *e, work, keep);
            if (e->jones.datum == NULL) missing++;
            if (chorda_is(e->alexander, "1")) {
                trivial++;
                if (e->unknot_certified) cert++;
            }
            if (e->det > max_det) max_det = e->det;
            if (e->alex_span > max_span) max_span = e->alex_span;
        }
        for (q = 0; q < sp->n; q++) {
            unsigned r;
            int      first = 1;

            for (r = 0; r < q; r++)
                if (chorda_aequalis(sp->c[r].alexander, sp->c[q].alexander))
                    first = 0;
            n_alex += (unsigned)first;
            if (sp->c[q].knot == k && strchr(sp->c[q].name, '*') == NULL)
                self = sp->c[q].count;
            if (sp->c[q].knot == k && strchr(sp->c[q].name, '*') != NULL)
                self_mirror = sp->c[q].count;
        }
        printf("\n  %s: %u vertices, %lu alternatives, %u simple, %u classes; "
            "distinct Alexander %u (D112 %u, D114 on D112's polygon %u)\n",
            code, (unsigned)laqueus_numerus(chosen[k]), count, n_simple, sp->n,
            n_alex, D112_DISTINCT[k], D114_DISTINCT[k]);
        printf("    itself %u, its mirror %u; unknots %u of %u trivial-"
            "Alexander certified; Jones from the mirror partner %u, still "
            "missing %u; max det %lu (source %lu); max Alexander degree %u\n",
            self, self_mirror, cert, trivial, filled, missing, max_det,
            KNOT_DET[k], max_span);
        for (q = 0; q < sp->n; q++) {
            printf("      %-9s [%4u]  det %-4lu ", sp->c[q].name,
                sp->c[q].count, sp->c[q].det);
            print_chorda(sp->c[q].alexander);
            printf("  |  ");
            print_chorda(sp->c[q].jones);
            printf("\n");
        }
        sprintf(msg, "%s: mirror theorem on independently computed pairs - "
            "same simplicity and Alexander, mirror Jones", code);
        check(msg, pair_bad == 0);
        sprintf(msg, "%s: the all-zero alternative (the polygon itself) is "
            "named %s", code, code);
        check(msg, self >= 1);
        sprintf(msg, "%s: every alternative is simple and classified", code);
        check(msg, n_simple == count && sp->overflow == 0);
        if (missing == 0) {
            sprintf(msg, "%s: after mirror filling, chirality is exactly "
                "symmetric (itself = mirror, or amphichiral)", code);
            check(msg, self_mirror == 0 || self == self_mirror);
        }
    }
}

/* ================================================================
 * Part C: reachability and the "explosion" question
 * ================================================================ */

static void
part_c (void)
{
    int k, j;

    printf("\n=== Part C: reachability (x = knot j or its mirror occurs among "
        "i's alternatives) ===\n  %-6s", "");
    for (j = 0; j < N_KNOTS; j++)
        printf("%5s", KNOT_CODE[j]);
    printf("  out (D112)  beyond-12  unnamed (no Jones / Jones)\n");
    for (k = 0; k < N_KNOTS; k++) {
        unsigned out = 0, beyond = 0, un_nojones = 0, un_jones = 0, q;

        printf("  %-6s", KNOT_CODE[k]);
        for (j = 0; j < N_KNOTS; j++) {
            int hit = 0;

            for (q = 0; q < spectra[k].n; q++)
                if (spectra[k].c[q].knot == j)
                    hit = 1;
            printf("%5s", hit ? "x" : ".");
            out += (unsigned)hit;
        }
        for (q = 0; q < spectra[k].n; q++) {
            if (spectra[k].c[q].knot == -3)
                beyond++;
            if (spectra[k].c[q].knot == -1) {
                if (spectra[k].c[q].jones.datum == NULL) un_nojones++;
                else un_jones++;
            }
        }
        printf("  %3u (%2u)  %9u  %7u / %u\n", out, D112_OUT_DEGREE[k],
            beyond, un_nojones, un_jones);
    }
    printf("\n  above the source's determinant (classes / distinct Alexander / "
        "alternatives):\n");
    for (k = 0; k < N_KNOTS; k++) {
        unsigned classes = 0, alex = 0, alts = 0, q, r;

        for (q = 0; q < spectra[k].n; q++) {
            int first = 1;

            if (spectra[k].c[q].det <= KNOT_DET[k])
                continue;
            classes++;
            alts += spectra[k].c[q].count;
            for (r = 0; r < q; r++)
                if (spectra[k].c[r].det > KNOT_DET[k]
                    && chorda_aequalis(spectra[k].c[r].alexander,
                        spectra[k].c[q].alexander))
                    first = 0;
            alex += (unsigned)first;
        }
        if (classes > 0)
            printf("    %-5s %u / %u / %u\n", KNOT_CODE[k], classes, alex,
                alts);
    }
    for (k = 0; k < N_KNOTS; k++) {
        unsigned q;

        for (q = 0; q < spectra[k].n; q++) {
            if (spectra[k].c[q].knot != -1 && spectra[k].c[q].knot != -3)
                continue;
            if (k == 8 && spectra[k].c[q].knot == -1)
                continue;   /* 7_2's unnamed classes are listed in Part B */
            printf("    %s in %s [%u]: det %lu, Alexander ",
                spectra[k].c[q].name, KNOT_CODE[k], spectra[k].c[q].count,
                spectra[k].c[q].det);
            print_chorda(spectra[k].c[q].alexander);
            printf(", Jones ");
            print_chorda(spectra[k].c[q].jones);
            printf("\n");
        }
    }
}

/* ================================================================
 * Part D: stability across equally honest polygons
 * ================================================================ */

static void
stability (
    int      k,
    Piscina *keep,
    Piscina *scratch,
    Piscina *work)
{
    long     n = raw_n[k], shift;
    int      rev, best = (int)laqueus_numerus(chosen[k]), variants = 0;
    int      above_variants = 0;
    Spectrum uni;              /* count = number of variants */
    unsigned q, uni_overflow = 0, bad_total = 0;
    char     msg[160];

    spectrum_clear(&uni);
    printf("\n  %s: every fewest-vertex variant (%d vertices)\n", KNOT_CODE[k],
        best);
    for (rev = 0; rev < 2; rev++) {
        for (shift = 0; shift < n; shift++) {
            Laqueus       v;
            Spectrum      sp;
            unsigned long count, c;
            unsigned      pair_bad, filled;
            int           above = 0;

            if (!variant(raw_pts[k], n, shift, rev, keep, &v)
                || (int)laqueus_numerus(v) != best)
                continue;
            variants++;
            spectrum_clear(&sp);
            count = classify_all(v, keep, scratch, &pair_bad, &filled);
            bad_total += pair_bad;
            for (c = 0; c < count; c++)
                if (alt_exact[c].simple)
                    spectrum_add(&sp, alt_exact[c], work, keep);
            printf("    %s%-3ld %3u classes:", rev ? "r" : "+", shift, sp.n);
            for (q = 0; q < sp.n; q++) {
                unsigned u;
                int      seen = 0;

                printf(" %s[%u]", sp.c[q].name, sp.c[q].count);
                if (sp.c[q].knot == -1 && sp.c[q].det > 1)
                    printf("(det %lu)", sp.c[q].det);
                if (sp.c[q].det > KNOT_DET[k])
                    above = 1;
                for (u = 0; u < uni.n; u++) {
                    if (chorda_aequalis(uni.c[u].key, sp.c[q].key)) {
                        uni.c[u].count++;
                        seen = 1;
                    }
                }
                if (!seen) {
                    if (uni.n < MAX_CLASSES) {
                        uni.c[uni.n]       = sp.c[q];
                        uni.c[uni.n].count = 1;
                        uni.n++;
                    } else {
                        uni_overflow++;
                    }
                }
            }
            if (above) {
                above_variants++;
                printf("  <- above source det");
            }
            printf("\n");
        }
    }
    printf("    union over %d variants (class [variants containing it]):",
        variants);
    for (q = 0; q < uni.n; q++)
        printf(" %s[%u]", uni.c[q].name, uni.c[q].count);
    printf("\n    core (in every variant):");
    for (q = 0; q < uni.n; q++)
        if (uni.c[q].count == (unsigned)variants)
            printf(" %s", uni.c[q].name);
    printf("\n    variants with a class above the source's determinant: %d\n",
        above_variants);
    sprintf(msg, "%s: stability covers exactly the %d fewest-vertex variants "
        "found in Part A; union complete; mirror theorem holds in every "
        "variant", KNOT_CODE[k], fewest[k]);
    check(msg, variants == fewest[k] && uni_overflow == 0 && bad_total == 0);
}

static void
part_d (
    Piscina *keep,
    Piscina *scratch,
    Piscina *work)
{
    printf("\n=== Part D: stability - spectra of all fewest-vertex variants "
        "===\n");
    stability(6, keep, scratch, work);
    stability(1, keep, scratch, work);
}

/* ================================================================
 * main
 * ================================================================ */

int
main (void)
{
    Piscina *keep    = piscina_generare_dynamicum("d115_keep", 1 << 24);
    Piscina *scratch = piscina_generare_dynamicum("d115_scratch", 1 << 22);
    Piscina *work    = piscina_generare_dynamicum("d115_work", 1 << 20);

    printf("KNOTAPEL DEMO 115: Honest Construction-Word Spectra\n");
    printf("===================================================\n");
    if (keep == NULL || scratch == NULL || work == NULL) {
        printf("pool allocation failed\n");
        return 1;
    }
    part_a(keep, scratch);
    part_b(keep, scratch, work);
    part_c();
    part_d(keep, scratch, work);
    check("every reflection through the base plane was defined",
        reflexio_failures == 0);
    printf("\n===================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    piscina_destruere(work);
    piscina_destruere(scratch);
    piscina_destruere(keep);
    return n_fail > 0 ? 1 : 0;
}
