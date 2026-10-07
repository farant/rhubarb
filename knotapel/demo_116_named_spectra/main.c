/*
 * KNOTAPEL DEMO 116: Named Spectra
 * ================================================================
 *
 * Demo 115 computed every construction-word alternative of D112's 12
 * honest polygons exactly, but could only name classes against the 12
 * source knots (plus hand-checked trefoil composites and 8_20), and 1,432
 * of 7_2's alternatives had no Jones polynomial (their best projections
 * had 21-30 crossings). This demo re-runs D115 with two new house tools:
 *
 *   - laqueus_pd_simplificare: greedy Reidemeister I/II on the PD code
 *     of a projection. Exact (an isotopy); a result with 0 crossings is
 *     a PROOF of the unknot, and the reduced crossing count is an upper
 *     bound on the crossing number;
 *   - tabula_nodorum: the 250 prime knots up to 10 crossings (names,
 *     symmetry and PD codes from KnotInfo, polynomials computed by
 *     laqueus) and tabula_nodorum_agnoscere, which names a knot by
 *     Alexander + Jones as a prime (as drawn or mirrored) or a sum of two
 *     table knots.
 *
 * Names are KnotInfo's: "K" is the KnotInfo diagram's chirality, "K*"
 * its mirror (D115 used D112's braid chirality - Part A translates).
 * A name means Alexander AND Jones match - not a proof of type (5_1 and
 * 10_132* share both; every such ambiguity is printed, "a|b").
 * Beyond the table: span(Jones) <= crossing number for every knot
 * (Kauffman, Murasugi, Thistlethwaite), so a class whose Jones span
 * exceeds 10 is PROVEN to have more than 10 crossings; every prime knot
 * with <= 10 crossings is in the table.
 *
 *   Part A  the honest polygons (as D115) and the chirality translation
 *           D112 -> KnotInfo for the 12 sources
 *   Part B  every spectrum, named; D115's Alexander-level numbers
 *           reproduced exactly; mirror theorem checked, then used
 *   Part C  reachability with real names, everything beyond the 12, and
 *           crossing-number intervals [span(Jones), reduced crossings]
 *           for what the table cannot name
 *   Part D  stability (all fewest-vertex polygons of 6_3 and 4_1), named
 *
 * Jones: generic projection -> PD -> R1/R2 reduction; if still above 20
 * crossings, the fewest-crossing projections with direction components
 * in -1..1, then -2..2, each reduced the same way. Unknots PROVEN here:
 * reduced to 0 crossings, or Alexander 1 on a reduced diagram of <= 10
 * crossings (no nontrivial knot with <= 10 crossings has trivial
 * Alexander polynomial; the first are 11n34 and 11n42). Unknots by
 * CITATION, "unknot (TS)": Alexander 1 and Jones 1 computed on a diagram
 * of <= 20 crossings - the Jones polynomial detects the unknot for every
 * knot up to 22 crossings (Tuzun, Sikora, J. Knot Theory Ramifications
 * 27(3), 2018; extended to 24 crossings, arXiv:2003.06724) - an external
 * computer verification, kept apart from what this demo proves.
 *
 * House libraries: includes laqueus.h and tabula_nodorum.h, hence
 * latina.h (Roman numerals and Latin keywords are macros here). Build and
 * run from the repo root:
 *   ./bin/aedilis knotapel/demo_116_named_spectra/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * Frozen copy: demo-snapshot.c (knotapel/archive.sh).
 */

#include "laqueus.h"
#include "tabula_nodorum.h"
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

/* D112's published reachability out-degree */
static const unsigned D112_OUT_DEGREE[N_KNOTS] = {
    1, 3, 1, 3, 1, 3, 10, 1, 9, 1, 1, 1
};

/* D115's published numbers (findings.md, after review I): the
 * Alexander-level results do not depend on how Jones is computed or on
 * naming, so D116 must reproduce them exactly */
static const unsigned D115_VERTS[N_KNOTS] = {
    6, 8, 10, 11, 11, 10, 11, 14, 15, 13, 13, 12
};
static const unsigned D115_DISTINCT[N_KNOTS] = {
    2, 2, 3, 4, 6, 4, 6, 4, 47, 6, 4, 6
};
static const unsigned long D115_MAX_DET[N_KNOTS] = {
    3, 5, 5, 7, 15, 11, 13, 7, 85, 13, 15, 45
};
static const unsigned D115_TRIVIAL[N_KNOTS] = {
    6, 28, 80, 176, 156, 80, 144, 1120, 1844, 676, 576, 336
};
static const unsigned D115_CERTIFIED[N_KNOTS] = {
    6, 28, 80, 176, 156, 80, 144, 1120, 106, 676, 576, 288
};
/* itself + mirror after mirror filling (D115 table "itself / mirror") */
static const unsigned D115_SELF[N_KNOTS] = {
    2, 4, 8, 16, 8, 8, 8, 32, 16, 4, 64, 8
};
static const unsigned D115_MISSING_7_2 = 1432;

#define JONES_CAP        20
#define JONES_CAP_REF    22
#define UNKNOT_CERT      10
#define TABLE_MAX        10
#define NO_DIAGRAM      999u
#define MAX_VERTS       128
#define MAX_ALTS       4096
#define MAX_CLASSES     256
#define MAX_CAND          8

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
    unsigned      crossings;   /* generic projection */
    unsigned      reduced;     /* fewest crossings after R1/R2 (an upper
                                  bound on the crossing number);
                                  NO_DIAGRAM if not computed */
    unsigned long det;
    unsigned      alex_span;   /* degree of the normalized Alexander */
    int           unknot_proven;
    chorda        alexander;
    chorda        jones;       /* NULL: not computed */
    chorda        jones_mirror;
    int           jones_filled; /* taken from the mirror partner */
} Exact;

/* diagram -> PD -> greedy Reidemeister I/II */
static int
reduced_pd (
    Diagramma  d,
    Piscina   *pool,
    i32      **pd,
    i32       *c)
{
    i32 *raw;

    return diagramma_pd(d, pool, &raw)
        && laqueus_pd_simplificare(raw, diagramma_numerus(d), pool, pd, c);
}

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

    e.simple        = laqueus_simplex(l, scratch) ? 1 : 0;
    e.crossings     = 0;
    e.reduced       = NO_DIAGRAM;
    e.det           = 0;
    e.alex_span     = 0;
    e.unknot_proven = 0;
    e.alexander     = empty_chorda();
    e.jones         = empty_chorda();
    e.jones_mirror  = empty_chorda();
    e.jones_filled  = 0;
    if (e.simple && laqueus_diagramma_genericum(l, scratch, &d)) {
        Magnus m;
        i32   *pd = NULL;
        i32    c  = 0;

        e.crossings = (unsigned)diagramma_numerus(d);
        if (diagramma_alexander(d, scratch, &p)) {
            e.alexander = chorda_transcribere(
                polynomium_ad_chordam(p, 't', scratch), out_pool);
            e.alex_span = (unsigned)(polynomium_gradus_summus(p)
                - polynomium_gradus_imus(p));
        }
        if (diagramma_determinans(d, scratch, &m))
            e.det = chorda_to_ulong(magnus_ad_chordam(m, scratch));
        if (jones_cap > 0 && reduced_pd(d, scratch, &pd, &c)) {
            int trivial = chorda_is(e.alexander, "1");
            int radius;

            /* search other directions only while Jones is out of reach
             * or a trivial Alexander polynomial is not yet certified */
            for (radius = 1; radius <= 2; radius++) {
                Diagramma dm;
                i32      *pdm;
                i32       cm;

                if (!((unsigned)c > jones_cap
                        || (trivial && c > 0 && c > UNKNOT_CERT)))
                    break;
                if (laqueus_diagramma_minimum(l, (i32)radius, scratch, &dm)
                    && reduced_pd(dm, scratch, &pdm, &cm) && cm < c) {
                    pd = pdm;
                    c  = cm;
                }
            }
            e.reduced = (unsigned)c;
            if (c == 0) {
                e.jones        = chorda_ex_literis("1", out_pool);
                e.jones_mirror = e.jones;
            } else if ((unsigned)c <= jones_cap
                       && laqueus_jones_ex_pd(pd, c, scratch, &p)) {
                e.jones = chorda_transcribere(
                    polynomium_ad_chordam(p, 't', scratch), out_pool);
                e.jones_mirror = chorda_transcribere(polynomium_ad_chordam(
                    polynomium_inversum(p, scratch), 't', scratch), out_pool);
            }
            e.unknot_proven = c == 0 || (trivial && c <= UNKNOT_CERT);
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
 * Naming with the knot table
 * ================================================================ */

typedef struct {
    chorda alexander;
    chorda jones;          /* the raw braid polygon's (D112's chirality) */
} Reference;

static Reference      refs[N_KNOTS];
static TabulaNodorum *table;

enum {
    ST_UNKNOT,      /* proven */
    ST_UNKNOT_Q,    /* Alexander 1 and Jones 1 on a <= 20-crossing diagram:
                       the unknot by Tuzun-Sikora (cited, not proven) */
    ST_TABLE,       /* agnoscere found candidates */
    ST_BEYOND,      /* no candidate, span(Jones) > 10: proven > 10 crossings */
    ST_UNMATCHED,   /* no candidate, span(Jones) <= 10 */
    ST_NOJONES      /* Jones not computed: Alexander only */
};

static const char *const STATUS_NAME[] = {
    "unknot", "unknot (TS)", "table", "beyond", "unmatched", "no Jones"
};

static unsigned
jones_span (
    chorda   j,
    Piscina *scratch)
{
    PiscinaNotatio mark = piscina_notare(scratch);
    Polynomium     p;
    unsigned       s = 0;

    if (j.datum != NULL && polynomium_ex_chorda(j, 't', scratch, &p))
        s = (unsigned)(polynomium_gradus_summus(p)
            - polynomium_gradus_imus(p));
    piscina_reficere(scratch, mark);
    return s;
}

static void
append_name (
    char       *buf,
    size_t      size,
    const char *piece)
{
    size_t n = strlen(buf);

    if (n + strlen(piece) + 2 < size) {
        if (n > 0)
            strcat(buf, "|");
        strcat(buf, piece);
    } else if (n + 4 < size && strcmp(buf + n - 3, "...") != 0) {
        strcat(buf, "...");
    }
}

static void
agnitio_name (
    const Agnitio *a,
    char          *out)
{
    if (a->secundus == NULL)
        sprintf(out, "%s%s", a->primus->titulus,
            a->primus_speculum ? "*" : "");
    else
        sprintf(out, "%s%s#%s%s", a->primus->titulus,
            a->primus_speculum ? "*" : "", a->secundus->titulus,
            a->secundus_speculum ? "*" : "");
}

/* index of the 12 sources this candidate is (prime, either chirality) */
static int
source_index (
    const Agnitio *a)
{
    int k;

    if (a->secundus != NULL)
        return -1;
    for (k = 0; k < N_KNOTS; k++)
        if (strcmp(a->primus->titulus, KNOT_CODE[k]) == 0)
            return k;
    return -1;
}

typedef struct {
    int      status;
    int      source;       /* one of the 12 (either chirality) or -1 */
    int      composite;    /* first candidate is a sum */
    unsigned candidates;
    unsigned span;         /* Jones span: lower bound on crossing number */
    unsigned cand_c[MAX_CAND];     /* crossing number (sum for a sum) */
    int      cand_prime[MAX_CAND];
    char     first[16];    /* first candidate's table name if prime */
    char     name[96];
} Naming;

static Naming
name_class (
    chorda   alexander,
    chorda   jones,
    int      unknot_proven,
    Piscina *scratch)
{
    PiscinaNotatio mark = piscina_notare(scratch);
    Naming         nm;
    Polynomium     pa, pj;
    Agnitio        cand[MAX_CAND];
    i32            n, k;

    nm.status     = ST_UNMATCHED;
    nm.source     = -1;
    nm.composite  = 0;
    nm.candidates = 0;
    nm.span       = jones_span(jones, scratch);
    nm.name[0]    = '\0';
    nm.first[0]   = '\0';
    if (chorda_is(alexander, "1") && (unknot_proven || chorda_is(jones, "1"))) {
        nm.status = unknot_proven ? ST_UNKNOT : ST_UNKNOT_Q;
        /* Jones 1 is only ever computed on a diagram of <= JONES_CAP
         * crossings (or its mirror partner's) */
        strcpy(nm.name, unknot_proven ? "unknot" : "unknot (TS)");
    } else if (jones.datum == NULL) {
        /* Alexander only: the table primes that share it */
        nm.status = ST_NOJONES;
        for (k = 0; k < (i32)tabula_nodorum_numerus(); k++) {
            const NodusTabulae *t = tabula_nodorum_nodus((i32)k);

            if (chorda_is(alexander, t->alexander)) {
                append_name(nm.name, sizeof(nm.name), t->titulus);
                nm.candidates++;
            }
        }
        if (nm.candidates == 0)
            strcpy(nm.name, "?");
    } else if (polynomium_ex_chorda(alexander, 't', scratch, &pa)
               && polynomium_ex_chorda(jones, 't', scratch, &pj)) {
        n = tabula_nodorum_agnoscere(table, pa, pj, scratch, cand, MAX_CAND);
        nm.candidates = (unsigned)n;
        if (n == 0) {
            nm.status = nm.span > TABLE_MAX ? ST_BEYOND : ST_UNMATCHED;
            strcpy(nm.name, nm.span > TABLE_MAX ? "beyond" : "?");
        } else {
            nm.status    = ST_TABLE;
            nm.composite = cand[0].secundus != NULL;
            for (k = 0; k < n && k < MAX_CAND; k++) {
                char piece[48];

                agnitio_name(&cand[k], piece);
                append_name(nm.name, sizeof(nm.name), piece);
                nm.cand_prime[k] = cand[k].secundus == NULL;
                nm.cand_c[k]     = (unsigned)cand[k].primus->transitus
                    + (cand[k].secundus != NULL
                       ? (unsigned)cand[k].secundus->transitus : 0u);
                if (k == 0 && cand[k].secundus == NULL)
                    strcpy(nm.first, cand[k].primus->titulus);
                if (nm.source < 0)
                    nm.source = source_index(&cand[k]);
            }
        }
    }
    piscina_reficere(scratch, mark);
    return nm;
}

/* ================================================================
 * Spectrum: classes (Alexander | Jones) with populations
 * ================================================================ */

typedef struct {
    chorda        key;
    chorda        alexander;
    chorda        jones;
    unsigned      count;
    Naming        nm;
    unsigned long det;
    unsigned      alex_span;
    unsigned      reduced;     /* min over members */
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
 * whether the unknot is proven - so every member of a class has the
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
        k = chorda_concatenare(k, chorda_ex_literis(e.unknot_proven
            ? "  | proven" : "  | unproven", pool), pool);
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
            if (e.reduced < s->c[k].reduced)
                s->c[k].reduced = e.reduced;
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
    s->c[s->n].alex_span = e.alex_span;
    s->c[s->n].reduced   = e.reduced;
    s->c[s->n].nm        = name_class(e.alexander, e.jones, e.unknot_proven,
        work);
    s->n++;
}

/* ================================================================
 * Part A: base polygons and the chirality translation
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

    printf("\n=== Part A: honest polygons (as D115) and D112 -> KnotInfo "
        "chirality ===\n");
    printf("  %-5s %4s %7s %6s %8s  %-22s %s\n", "knot", "raw", "fewest",
        "#var", "chosen", "KnotInfo name(s)", "raw polygon reduced");
    for (k = 0; k < N_KNOTS; k++) {
        const char *code = KNOT_CODE[k];
        Laqueus     raw;
        Exact       er, ec;
        Naming      nm;
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
        refs[k].alexander = er.alexander;
        refs[k].jones     = er.jones;
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
        nm = name_class(er.alexander, er.jones, er.unknot_proven, scratch);
        printf("  %-5s %4ld %7d %6d  %s%-6ld  %-22s %u -> %u\n", code, n, best,
            n_best, best_rev ? "r" : "+", best_shift, nm.name, er.crossings,
            er.reduced);
        sprintf(msg, "%s: raw polygon simple, table Alexander, Jones computed",
            code);
        check(msg, er.simple && chorda_is(er.alexander, KNOT_ALEXANDER[k])
            && er.jones.datum != NULL);
        sprintf(msg, "%s: the table names the source %s (Alexander + Jones; "
            "either chirality)", code, code);
        check(msg, nm.status == ST_TABLE && nm.source == k);
        sprintf(msg, "%s: honest polygon has D115's vertex count and the raw "
            "polygon's Alexander and Jones", code);
        check(msg, ec.simple && (unsigned)best == D115_VERTS[k]
            && chorda_same(ec.alexander, er.alexander)
            && chorda_same(ec.jones, er.jones));
    }
    printf("  (KnotInfo name of D112's braid knot: K = the KnotInfo diagram's "
        "chirality, K* = its mirror)\n");
}

/* ================================================================
 * Part B: spectra
 * ================================================================ */

static Spectrum spectra[N_KNOTS];
static Exact    alt_exact[MAX_ALTS];
static unsigned missing_jones[N_KNOTS];

/* All alternatives of base classified. The mirror theorem (alternative
 * c ^ mask is the mirror image of c) is CHECKED on independently
 * computed pairs first; only then USED to fill Jones, the unknot proof
 * and the reduced crossing count from the partner. */
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
        alt_exact[c] = classify_alternative(base, c, JONES_CAP, scratch,
            keep);
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
        if (b->reduced < a->reduced)
            a->reduced = b->reduced;
        if (b->unknot_proven && chorda_is(a->alexander, "1"))
            a->unknot_proven = 1;
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

    printf("\n=== Part B: named spectra ===\n");
    printf("  (a name means Alexander AND Jones match; a|b = several table "
        "matches; '[c..r]' = crossing number between span(Jones) and the "
        "reduced diagram)\n");
    for (k = 0; k < N_KNOTS; k++) {
        const char   *code = KNOT_CODE[k];
        unsigned long count, c;
        unsigned      n_simple = 0, missing = 0, trivial = 0, proven = 0;
        unsigned      pair_bad, filled, self = 0, q;
        unsigned      n_alex = 0;
        unsigned long max_det = 0;
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
                if (e->unknot_proven) proven++;
            }
            if (e->det > max_det) max_det = e->det;
        }
        missing_jones[k] = missing;
        for (q = 0; q < sp->n; q++) {
            unsigned r;
            int      first = 1;

            for (r = 0; r < q; r++)
                if (chorda_aequalis(sp->c[r].alexander, sp->c[q].alexander))
                    first = 0;
            n_alex += (unsigned)first;
            if (sp->c[q].nm.source == k)
                self += sp->c[q].count;
        }
        printf("\n  %s: %u vertices, %lu alternatives, %u simple, %u classes, "
            "%u distinct Alexander; itself (either chirality) %u; unknots "
            "proven %u of %u trivial-Alexander (D115: %u); Jones from the "
            "mirror partner %u, still missing %u; max det %lu (source "
            "%lu)\n", code, (unsigned)laqueus_numerus(chosen[k]), count,
            n_simple, sp->n, n_alex, self, proven, trivial,
            D115_CERTIFIED[k], filled, missing, max_det, KNOT_DET[k]);
        for (q = 0; q < sp->n; q++) {
            const Class *cl = &sp->c[q];

            printf("      %-24s [%4u]  det %-4lu ", cl->nm.name, cl->count,
                cl->det);
            if (cl->nm.status != ST_UNKNOT && cl->nm.status != ST_UNKNOT_Q
                && cl->nm.status != ST_TABLE) {
                if (cl->jones.datum != NULL)
                    printf("[%u..", cl->nm.span);
                else
                    printf("[?..");
                if (cl->reduced == NO_DIAGRAM) printf("?] ");
                else printf("%u] ", cl->reduced);
            }
            print_chorda(cl->alexander);
            printf("  |  ");
            print_chorda(cl->jones);
            printf("\n");
        }
        sprintf(msg, "%s: mirror theorem on independently computed pairs",
            code);
        check(msg, pair_bad == 0);
        sprintf(msg, "%s: D115's Alexander-level numbers reproduced (%lu "
            "alternatives all simple, %u distinct Alexander, max det %lu, %u "
            "trivial)", code, count, D115_DISTINCT[k], D115_MAX_DET[k],
            D115_TRIVIAL[k]);
        check(msg, n_simple == count && sp->overflow == 0
            && n_alex == D115_DISTINCT[k] && max_det == D115_MAX_DET[k]
            && trivial == D115_TRIVIAL[k]);
        sprintf(msg, "%s: every D115 unknot certificate holds (%u proven >= "
            "%u)", code, proven, D115_CERTIFIED[k]);
        check(msg, proven >= D115_CERTIFIED[k]);
        if (k == 8) {
            sprintf(msg, "7_2: itself + mirror at least D115's %u; missing "
                "Jones %u (D115: %u)", D115_SELF[k], missing,
                D115_MISSING_7_2);
            check(msg, self >= D115_SELF[k] && missing <= D115_MISSING_7_2);
        } else {
            sprintf(msg, "%s: itself + mirror = D115's %u, no Jones missing",
                code, D115_SELF[k]);
            check(msg, self == D115_SELF[k] && missing == 0);
        }
    }
}

/* ================================================================
 * Part C: reachability, beyond the 12, and what the table cannot name
 * ================================================================ */

static void
part_c (void)
{
    int      k, j;
    unsigned status_classes[6], status_alts[6], s;

    printf("\n=== Part C: reachability (x = source j, either chirality, "
        "among i's alternatives) ===\n  %-6s", "");
    for (j = 0; j < N_KNOTS; j++)
        printf("%5s", KNOT_CODE[j]);
    printf("  out (D112)\n");
    for (k = 0; k < N_KNOTS; k++) {
        unsigned out = 0, q;

        printf("  %-6s", KNOT_CODE[k]);
        for (j = 0; j < N_KNOTS; j++) {
            int hit = 0;

            for (q = 0; q < spectra[k].n; q++)
                if (spectra[k].c[q].nm.source == j)
                    hit = 1;
            printf("%5s", hit ? "x" : ".");
            out += (unsigned)hit;
        }
        printf("  %3u (%2u)\n", out, D112_OUT_DEGREE[k]);
    }
    printf("\n  named beyond the 12 sources (class [alternatives]):\n");
    for (k = 0; k < N_KNOTS; k++) {
        unsigned q, any = 0;

        for (q = 0; q < spectra[k].n; q++) {
            const Class *cl = &spectra[k].c[q];

            if (cl->nm.status != ST_TABLE || cl->nm.source >= 0)
                continue;
            if (!any)
                printf("    %-5s", KNOT_CODE[k]);
            printf(" %s[%u]", cl->nm.name, cl->count);
            any = 1;
        }
        if (any)
            printf("\n");
    }
    for (s = 0; s < 6; s++) {
        status_classes[s] = 0;
        status_alts[s]    = 0;
    }
    for (k = 0; k < N_KNOTS; k++) {
        unsigned q;

        for (q = 0; q < spectra[k].n; q++) {
            status_classes[spectra[k].c[q].nm.status]++;
            status_alts[spectra[k].c[q].nm.status] += spectra[k].c[q].count;
        }
    }
    printf("\n  all spectra by status (classes / alternatives):\n");
    for (s = 0; s < 6; s++)
        printf("    %-10s %4u / %u\n", STATUS_NAME[s], status_classes[s],
            status_alts[s]);
    {
        unsigned named = 0, inconsistent = 0, small_unmatched = 0;

        printf("\n  distinct prime table knots reached (first candidate, "
            "up to mirror):\n");
        for (k = 0; k < N_KNOTS; k++) {
            char     seen[MAX_CLASSES][16];
            unsigned n_seen = 0, q, u;

            for (q = 0; q < spectra[k].n; q++) {
                const Class *cl = &spectra[k].c[q];
                int          dup = 0;

                if (cl->nm.status != ST_TABLE || cl->nm.first[0] == '\0')
                    continue;
                for (u = 0; u < n_seen; u++)
                    if (strcmp(seen[u], cl->nm.first) == 0)
                        dup = 1;
                if (!dup)
                    strcpy(seen[n_seen++], cl->nm.first);
            }
            printf("    %-5s %u\n", KNOT_CODE[k], n_seen);
        }
        /* the reduced diagram is a diagram of the knot, so its crossing
         * count is at least the crossing number of the named candidate
         * (span(V) <= c(K) holds by construction here: the Jones IS the
         * table knot's) */
        for (k = 0; k < N_KNOTS; k++) {
            unsigned q;

            for (q = 0; q < spectra[k].n; q++) {
                const Class *cl = &spectra[k].c[q];
                unsigned     m;
                int          fits = 0;

                if (cl->nm.status == ST_UNMATCHED && cl->reduced <= TABLE_MAX)
                    small_unmatched++;
                if (cl->nm.status != ST_TABLE)
                    continue;
                named++;
                for (m = 0; m < cl->nm.candidates && m < MAX_CAND; m++)
                    if (!cl->nm.cand_prime[m]
                        || cl->nm.cand_c[m] <= cl->reduced)
                        fits = 1;
                if (!fits) {
                    inconsistent++;
                    printf("    INCONSISTENT: %s in %s (reduced %u)\n",
                        cl->nm.name, KNOT_CODE[k], cl->reduced);
                }
            }
        }
        check("every table-named prime class: some candidate's crossing "
            "number <= the reduced crossing count", named > 0
            && inconsistent == 0);
        check("every unmatched class has a reduced diagram above 10 "
            "crossings (primes and two-knot sums up to 10 crossings are "
            "searched; sums of three or more are not)", small_unmatched
            == 0);
    }
    printf("\n  not named by the table (crossing number in [span(Jones), "
        "reduced]):\n");
    for (k = 0; k < N_KNOTS; k++) {
        unsigned q;

        for (q = 0; q < spectra[k].n; q++) {
            const Class *cl = &spectra[k].c[q];

            if (cl->nm.status != ST_BEYOND && cl->nm.status != ST_UNMATCHED)
                continue;
            printf("    %-5s %-9s [%4u] det %-4lu c in [%u, ", KNOT_CODE[k],
                cl->nm.name, cl->count, cl->det, cl->nm.span);
            if (cl->reduced == NO_DIAGRAM) printf("?]  ");
            else printf("%u]  ", cl->reduced);
            print_chorda(cl->alexander);
            printf("\n");
        }
    }
}

/* ================================================================
 * Part D: stability across equally honest polygons, named
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

                printf(" %s[%u]", sp.c[q].nm.name, sp.c[q].count);
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
            printf("\n");
        }
    }
    printf("    union over %d variants (class [variants containing it]):",
        variants);
    for (q = 0; q < uni.n; q++)
        printf(" %s[%u]", uni.c[q].nm.name, uni.c[q].count);
    printf("\n    core (in every variant):");
    for (q = 0; q < uni.n; q++)
        if (uni.c[q].count == (unsigned)variants)
            printf(" %s", uni.c[q].nm.name);
    printf("\n");
    sprintf(msg, "%s: stability covers the %d fewest-vertex variants; union "
        "complete; mirror theorem holds in every variant", KNOT_CODE[k],
        fewest[k]);
    check(msg, variants == fewest[k] && uni_overflow == 0 && bad_total == 0);
}

static void
part_d (
    Piscina *keep,
    Piscina *scratch,
    Piscina *work)
{
    printf("\n=== Part D: stability - all fewest-vertex variants, named "
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
    Piscina *keep    = piscina_generare_dynamicum("d116_keep", 1 << 24);
    Piscina *scratch = piscina_generare_dynamicum("d116_scratch", 1 << 22);
    Piscina *work    = piscina_generare_dynamicum("d116_work", 1 << 20);

    printf("KNOTAPEL DEMO 116: Named Spectra\n");
    printf("================================\n");
    if (keep == NULL || scratch == NULL || work == NULL) {
        printf("pool allocation failed\n");
        return 1;
    }
    table = tabula_nodorum_aperire(keep);
    check("knot table opened (250 knots, KnotInfo 2026.10.5)",
        table != NULL && tabula_nodorum_numerus() == 250);
    printf("  table source: %s\n", TABULA_NODORUM_FONS);
    part_a(keep, scratch);
    part_b(keep, scratch, work);
    part_c();
    part_d(keep, scratch, work);
    check("every reflection through the base plane was defined",
        reflexio_failures == 0);
    printf("\n================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    piscina_destruere(work);
    piscina_destruere(scratch);
    piscina_destruere(keep);
    return n_fail > 0 ? 1 : 0;
}
