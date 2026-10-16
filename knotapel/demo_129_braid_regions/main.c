/*
 * KNOTAPEL DEMO 129: How Many Linear Regions Does a Braid Network Carve? (N6)
 * ===========================================================================
 *
 * A braid word w on n strands is an exact, bias-free ReLU network on the
 * 2n - 4 Dynnikov coordinates (D128, KC 55; plexus_nervus). Its iterate
 * w^m is m copies composed. How does the number of its LINEAR REGIONS
 * grow with m, and does the braid's dynamics (periodic P, positive
 * entropy E, zero entropy Z) decide it? Plan: plan.md (approved
 * 2026-10-16, amended after the T5.0 review: regions belong to the WORD,
 * pieces to the braid).
 *
 *   Part A  (T5a) B_3, EXACT: every class up to length 8 (2,589), every
 *           geodesic word of each class, m = 1..12: activation regions
 *           (sectors, nervus_regiones_plani) and linear pieces of w^m.
 *           The registered rule H_lin vs H_exp.
 *   Part B  (T5a) B_3, the lattice lower bound (interior points of the
 *           shells max |x_i| = R, itineraries by refinement) against
 *           Part A's exact count.
 *   Part C  (T5b) B_4 lattice bounds; the period-2 side question.
 *   Part D  (T5b) nulls.
 *
 * Classes on B_3 are EXACT: P iff beta^k is a power of Delta^2
 * (plexus_periodus); otherwise the type is read from the trace of the
 * reduced Burau matrix at t = -1, the image of beta in SL(2, Z): E iff
 * |tr| > 2, with dilatation lambda = (|tr| + sqrt(tr^2 - 4)) / 2, Z iff
 * |tr| = 2 (reducible); |tr| < 2 is always periodic (B_3 / Z(B_3) =
 * PSL(2, Z); Murasugi 1974; cited, and checked against D128's table).
 *
 * House libraries: plexus, nervus (and their closure), hence latina.h:
 * Roman numerals and Latin keywords are macros here (never name a
 * variable I, V, X, L, C, D or M).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_129_braid_regions/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * Quick mode for plants: D129_CELER=1 (B_3 up to length 5, shells 4, 16).
 */

#include "plexus.h"
#include "nervus.h"
#include "radices.h"
#include "tabula_dispersa.h"
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

static Piscina *pool;

static void
fatal (
    const char *msg)
{
    printf("  FATAL: %s\n", msg);
    exit(1);
}

static Plexus
braid (
    int         n,
    const char *text)
{
    Plexus p;

    if (!plexus_ex_chorda((i32)n, chorda_ex_literis(text, pool), pool,
            &p)) {
        printf("  FATAL: bad braid %s\n", text);
        exit(1);
    }
    return p;
}

/* a word as text into buf (letters separated by spaces) */
static void
word_text (
    Plexus p,
    char  *buf,
    int    size)
{
    int k;
    int used = 0;

    buf[0] = '\0';
    for (k = 0; k < (int)p.longitudo && used < size - 8; k++) {
        used += sprintf(buf + used, k ? " %d" : "%d", (int)p.litterae[k]);
    }
    if (p.longitudo == 0) {
        sprintf(buf, "(empty)");
    }
}

/* one decimal of num / den, integers only */
static void
mean_text (
    long  num,
    long  den,
    char *buf)
{
    long t;

    if (den == 0) {
        sprintf(buf, "-");
        return;
    }
    t = (10 * num + den / 2) / den;
    sprintf(buf, "%ld.%ld", t / 10, t % 10);
}

static long
pow3 (
    int e)
{
    long r = 1;

    while (e-- > 0) r *= 3;
    return r;
}

/* length after free CYCLIC reduction (strip first/last when inverse) */
static int
cyclic_length (
    Plexus p)
{
    int lo = 0;
    int hi = (int)p.longitudo - 1;

    while (hi - lo >= 1 && p.litterae[lo] == -p.litterae[hi]) {
        lo++;
        hi--;
    }
    return hi - lo + 1;
}

static int
words_equal (
    Plexus a,
    Plexus b)
{
    int k;

    if (a.longitudo != b.longitudo) return 0;
    for (k = 0; k < (int)a.longitudo; k++) {
        if (a.litterae[k] != b.litterae[k]) return 0;
    }
    return 1;
}

/* ================================================================
 * The census, its classes and its geodesic words
 * ================================================================ */

#define MMAX 12
#define CLS_P 0
#define CLS_E 1
#define CLS_Z 2

static const char *cls_name[3] = { "P", "E", "Z" };

typedef struct {
    Plexus word;              /* census representative */
    int    len;
    int    cls;
    int    period;
    long   tr;                /* trace of Burau(-1) */
    int    ngeo;              /* geodesic words of the class */
    int    geo0;              /* first in geo[] */
    int    rep;               /* the representative's index in geo[] */
    int    pieces[MMAX + 1];
    int    rmin[MMAX + 1];
    int    rmax[MMAX + 1];
    int    rrep[MMAX + 1];
    int    rbest[MMAX + 1];   /* the word with the most regions at MMAX */
    int    best;              /* its index in geo[] */
    int    null_sectors;      /* sectors with an identically-0 argument */
} Klass;

static int     celer = 0;
static int     maxlen = 8;
static Klass  *ks;
static int     nk;
static Plexus *geo;           /* geodesic words, grouped by class */
static int     ngeo;
static int    *geo_class;

/* per-length pins (D128 findings, B_3 up to 8) */
static const int pin_P[9] = { 1, 0, 4, 10, 20, 16, 34, 32, 92 };
static const int pin_E[9] = { 0, 0, 4, 12, 32, 92, 248, 544, 1176 };
static const int pin_Z[9] = { 0, 4, 4, 8, 16, 40, 32, 80, 88 };

static long
burau_trace (
    Plexus p)
{
    Matrix m;
    s64    a = 0;
    s64    d = 0;

    if (!plexus_burau_minus_uno(p, pool, &m)
            || !magnus_ad_s64(*(const Magnus *)matrix_elementum(m, 0, 0),
                &a)
            || !magnus_ad_s64(*(const Magnus *)matrix_elementum(m, 1, 1),
                &d)) {
        fatal("Burau(-1) refused");
    }
    return (long)(a + d);
}

static void
build_census (void)
{
    PlexusCensus cs;
    int          i;
    int          ok_cls = 1;
    int          ok_trace = 1;
    int          ok_radix = 1;
    int          len;

    if (!plexus_enumerare(3, (i32)maxlen, 100000, pool, &cs)) {
        fatal("census refused");
    }
    nk = (int)cs.numerus;
    ks = (Klass *)calloc((size_t)nk, sizeof(Klass));
    for (i = 0; i < nk; i++) {
        i32          period = 0;
        RadixRealis  rho;
        s32          sg = 0;
        long         atr;

        ks[i].word = cs.verba[i];
        ks[i].len = (int)cs.verba[i].longitudo;
        if (!plexus_periodus(ks[i].word, pool, &period)) {
            fatal("period refused");
        }
        ks[i].period = (int)period;
        ks[i].tr = burau_trace(ks[i].word);
        atr = ks[i].tr < 0 ? -ks[i].tr : ks[i].tr;
        if (period > 0) {
            ks[i].cls = CLS_P;
            if (atr > 2) ok_trace = 0;       /* periodic: |tr| <= 2 */
        } else if (atr > 2) {
            ks[i].cls = CLS_E;
        } else {
            ks[i].cls = CLS_Z;
            if (atr != 2) ok_trace = 0;      /* |tr| < 2 is periodic */
        }
        /* Burau(-1) spectral radius > 1 iff |tr| > 2 (2 x 2, det 1) */
        if (!plexus_burau_radix_maxima(ks[i].word, pool, &rho)
                || !radix_compara(rho, radix_ex_fractione(
                    fractio_ex_s64(1), pool), pool, &sg)) {
            fatal("radix refused");
        }
        if ((sg > 0) != (atr > 2)) ok_radix = 0;
    }
    for (len = 0; len <= maxlen; len++) {
        int p = 0, e = 0, z = 0;

        for (i = 0; i < nk; i++) {
            if (ks[i].len != len) continue;
            if (ks[i].cls == CLS_P) p++;
            else if (ks[i].cls == CLS_E) e++;
            else z++;
        }
        if (p != pin_P[len] || e != pin_E[len] || z != pin_Z[len]) {
            ok_cls = 0;
        }
    }
    check("classes per length = D128's table (P exact, E certified)",
        ok_cls);
    check("trace law: periodic => |tr| <= 2, not periodic and |tr| <= 2 "
        "=> |tr| = 2", ok_trace);
    check("Burau(-1) radius > 1 iff |tr| > 2 (plexus radix)", ok_radix);
}

/* every freely reduced word of length <= maxlen, keyed to its class;
 * geodesic = length equals the class's census length */
static void
collect_geodesics (void)
{
    TabulaDispersa *index = tabula_dispersa_creare_chorda(pool, 4096);
    int            *slot = (int *)calloc((size_t)nk, sizeof(int));
    Plexus         *cand;
    int            *cand_class;
    int             ncand = 0;
    int             cap = 1;
    int             len;
    int             i;
    int             ok_rep = 1;
    int             ok_found = 1;
    int             ok_short = 1;
    int             letters[4] = { -2, -1, 1, 2 };
    int            *counter;

    for (i = 0; i < nk; i++) {
        chorda key;

        slot[i] = i;
        if (!plexus_clavis(ks[i].word, pool, &key)) fatal("key refused");
        tabula_dispersa_inserere(index, key, &slot[i]);
    }
    for (len = 1; len <= maxlen; len++) cap += 4 * (int)pow3(len - 1);
    cand = (Plexus *)malloc((size_t)cap * sizeof(Plexus));
    cand_class = (int *)malloc((size_t)cap * sizeof(int));
    /* breadth-first over reduced words */
    {
        Plexus empty = braid(3, "");
        int    lo = 0;
        int    hi;

        cand[ncand++] = empty;
        for (len = 1; len <= maxlen; len++) {
            int j;

            hi = ncand;
            for (j = lo; j < hi; j++) {
                int t;

                for (t = 0; t < 4; t++) {
                    s32    l = (s32)letters[t];
                    Plexus one;
                    Plexus w;

                    if (cand[j].longitudo > 0 && cand[j].litterae[
                            cand[j].longitudo - 1] == -l) {
                        continue;
                    }
                    plexus_creare(3, &l, 1, pool, &one);
                    plexus_iungere(cand[j], one, pool, &w);
                    cand[ncand++] = w;
                }
            }
            lo = hi;
        }
    }
    for (i = 0; i < ncand; i++) {
        chorda  key;
        vacuum *val = NULL;

        if (!plexus_clavis(cand[i], pool, &key)) fatal("key refused");
        if (!tabula_dispersa_invenire(index, key, &val)) {
            ok_found = 0;           /* every word of length <= L is in */
            cand_class[i] = -1;
            continue;
        }
        cand_class[i] = *(int *)val;
        if ((int)cand[i].longitudo < ks[cand_class[i]].len) ok_short = 0;
    }
    check("every reduced word up to the census length finds its class",
        ok_found);
    check("no word is shorter than its class's census length (census "
        "levels are geodesic)", ok_short);
    /* group geodesic words by class, in enumeration order */
    for (i = 0; i < ncand; i++) {
        if (cand_class[i] >= 0 && (int)cand[i].longitudo
                == ks[cand_class[i]].len) {
            ks[cand_class[i]].ngeo++;
        }
    }
    ngeo = 0;
    for (i = 0; i < nk; i++) {
        ks[i].geo0 = ngeo;
        ngeo += ks[i].ngeo;
    }
    geo = (Plexus *)malloc((size_t)ngeo * sizeof(Plexus));
    geo_class = (int *)malloc((size_t)ngeo * sizeof(int));
    counter = (int *)calloc((size_t)nk, sizeof(int));
    for (i = 0; i < ncand; i++) {
        int c = cand_class[i];

        if (c >= 0 && (int)cand[i].longitudo == ks[c].len) {
            geo[ks[c].geo0 + counter[c]] = cand[i];
            geo_class[ks[c].geo0 + counter[c]] = c;
            counter[c]++;
        }
    }
    for (i = 0; i < nk; i++) {
        int j;

        ks[i].rep = -1;
        for (j = ks[i].geo0; j < ks[i].geo0 + ks[i].ngeo; j++) {
            if (words_equal(geo[j], ks[i].word)) ks[i].rep = j;
        }
        if (ks[i].rep < 0) ok_rep = 0;
    }
    check("each class's representative is among its geodesic words",
        ok_rep);
    printf("  reduced words up to length %d: %d; geodesic: %d\n", maxlen,
        ncand, ngeo);
    free(counter);
    free(slot);
    free(cand_class);
    free(cand);
}

/* ================================================================
 * Part A: exact sectors of w^m
 * ================================================================ */

/* the 2 x 2 sector matrix applied to (x, y) equals a 2-vector */
static int
matrix_maps (
    Matrix        m,
    Magnus        x,
    Magnus        y,
    const Magnus *out)
{
    i32 r;

    for (r = 0; r < 2; r++) {
        Magnus v = magnus_adde(magnus_multiplica(*(const Magnus *)
            matrix_elementum(m, r, 0), x, pool), magnus_multiplica(
            *(const Magnus *)matrix_elementum(m, r, 1), y, pool), pool);

        if (!magnus_aequalis(v, out[r])) return 0;
    }
    return 1;
}

static int ok_monotone = 1;
static int ok_sector_eval = 1;     /* pattern, nullae, M x at medius */
static int ok_sector_dyn = 1;      /* M x = ansa_agere^m at medius */
static int ok_pieces_class = 1;    /* every geodesic word: same pieces */
static int ok_periodic_r = 1;      /* r(m) = r(k), m >= k */
static int ok_periodic_p = 1;      /* p(k) = 1, M = I; p(m + k) = p(m) */

/* checks every sector of the representative's w^m at its medius */
static void
check_sectors (
    Plexus              w,
    const Nervus       *net,
    const NervusPlanum *pl,
    int                 m)
{
    int k;

    for (k = 0; k < (int)pl->numerus; k++) {
        const NervusSector *s = &pl->regiones[k];
        NervusForma         f;
        Magnus              in[2];
        Magnus              out[2];
        Ansa                u;
        int                 t;

        in[0] = s->medius.x;
        in[1] = s->medius.y;
        if (!nervus_forma_creare(net, pool, &f)
                || !nervus_aestimare(net, in, out, &f, pool)) {
            fatal("evaluation refused");
        }
        if (!nervus_formae_aequales(&f, &s->forma)
                || f.nullae != s->forma.nullae
                || !matrix_maps(s->m, in[0], in[1], out)) {
            ok_sector_eval = 0;
        }
        /* independent: the Dynnikov action itself, m passes */
        if (!ansa_ex_coordinatis(3, &in[0], &in[1], pool, &u)) {
            fatal("loop refused");
        }
        for (t = 0; t < m; t++) {
            if (!ansa_agere(u, w, pool, &u)) fatal("action refused");
        }
        if (!magnus_aequalis(u.a[0], out[0])
                || !magnus_aequalis(u.b[0], out[1])) {
            ok_sector_dyn = 0;
        }
    }
}

/* matrix is the 2 x 2 identity */
static int
is_identity (
    Matrix m)
{
    i32 r, c;

    for (r = 0; r < 2; r++) {
        for (c = 0; c < 2; c++) {
            Magnus e = *(const Magnus *)matrix_elementum(m, r, c);

            if (!magnus_aequalis(e, magnus_ex_s64(r == c ? 1 : 0))) {
                return 0;
            }
        }
    }
    return 1;
}

/* regions and pieces of w^1 .. w^MMAX for one word */
static void
sweep_word (
    Klass *kl,
    int    g,
    int   *r,
    int   *p,
    int    is_rep)
{
    PiscinaNotatio mark = piscina_notare(pool);
    Nervus        *one;
    Nervus        *net;
    int            m;

    if (!plexus_nervus(geo[g], pool, &one)) fatal("network refused");
    net = one;
    for (m = 1; m <= MMAX; m++) {
        NervusPlanum pl;
        int          k;

        if (m > 1 && !nervus_componere(net, one, pool, &net)) {
            fatal("composition refused");
        }
        if (!nervus_regiones_plani(net, pool, &pl)) fatal("sweep refused");
        r[m] = (int)pl.numerus;
        p[m] = (int)pl.frusta;
        for (k = 0; k < (int)pl.numerus; k++) {
            if (pl.regiones[k].forma.nullae > 0) kl->null_sectors++;
        }
        if (is_rep) {
            check_sectors(geo[g], net, &pl, m);
        }
        if (kl->cls == CLS_P && m == kl->period) {
            /* w^k is the identity map: one piece, M = I */
            if (pl.frusta != 1 || !is_identity(pl.regiones[0].m)) {
                ok_periodic_p = 0;
            }
        }
    }
    piscina_reficere(pool, mark);
    for (m = 2; m <= MMAX; m++) {
        if (r[m] < r[m - 1]) ok_monotone = 0;
    }
    if (kl->cls == CLS_P) {
        int k = kl->period;

        for (m = k; m <= MMAX; m++) {
            if (r[m] != r[k]) ok_periodic_r = 0;
        }
        for (m = 1; m + k <= MMAX; m++) {
            if (p[m + k] != p[m]) ok_periodic_p = 0;
        }
    }
}

static void
part_a_sweeps (void)
{
    int i;

    for (i = 0; i < nk; i++) {
        Klass *kl = &ks[i];
        int    g;
        int    m;

        for (m = 0; m <= MMAX; m++) {
            kl->rmin[m] = 1 << 30;
            kl->rmax[m] = 0;
        }
        kl->best = -1;
        for (g = kl->geo0; g < kl->geo0 + kl->ngeo; g++) {
            int r[MMAX + 1];
            int p[MMAX + 1];

            sweep_word(kl, g, r, p, g == kl->rep);
            for (m = 1; m <= MMAX; m++) {
                if (r[m] < kl->rmin[m]) kl->rmin[m] = r[m];
                if (r[m] > kl->rmax[m]) kl->rmax[m] = r[m];
            }
            if (g == kl->geo0) {
                memcpy(kl->pieces, p, sizeof(p));
            } else if (memcmp(kl->pieces + 1, p + 1, MMAX * sizeof(int))) {
                ok_pieces_class = 0;
            }
            if (g == kl->rep) memcpy(kl->rrep, r, sizeof(r));
            if (kl->best < 0 || r[MMAX] > kl->rbest[MMAX]) {
                kl->best = g;
                memcpy(kl->rbest, r, sizeof(r));
            }
        }
    }
}

/* ---------------- the registered rule ---------------- */

/* H_lin: increments d(m) = s(m+1) - s(m), m = 6..11, repeat with some
 * period q <= 3 */
static int
h_lin (
    const int *s)
{
    int q;

    for (q = 1; q <= 3; q++) {
        int m;
        int ok = 1;

        for (m = 6; m + q <= 11; m++) {
            if (s[m + q + 1] - s[m + q] != s[m + 1] - s[m]) ok = 0;
        }
        if (ok) return 1;
    }
    return 0;
}

/* H_exp: s(m+1) / s(m) >= 1.5 for every m in 8..11 */
static int
h_exp (
    const int *s)
{
    int m;

    for (m = 8; m <= 11; m++) {
        if (2 * s[m + 1] < 3 * s[m]) return 0;
    }
    return 1;
}

static void
part_a_report (void)
{
    int  c, m, i, len;
    char b1[32], b2[32], b3[32], b4[32];
    long nulls = 0;

    printf("\n  Classes and geodesic words by length:\n");
    printf("    %4s %8s %5s %5s %5s %10s %10s\n", "len", "classes", "P", "E",
        "Z", "geodesics", "per class");
    for (len = 0; len <= maxlen; len++) {
        int n = 0, cnt[3] = { 0, 0, 0 };
        long g = 0;

        for (i = 0; i < nk; i++) {
            if (ks[i].len != len) continue;
            n++;
            cnt[ks[i].cls]++;
            g += ks[i].ngeo;
        }
        mean_text(g, n, b1);
        printf("    %4d %8d %5d %5d %5d %10ld %10s\n", len, n, cnt[0],
            cnt[1], cnt[2], g, b1);
    }
    for (i = 0; i < nk; i++) nulls += ks[i].null_sectors;
    printf("\n  Sectors with an identically-zero ReLU argument, over all "
        "geodesic words\n  and m = 1..%d: %ld\n", MMAX, nulls);

    printf("\n  Means over classes of pieces p(m) and regions (min / max "
        "over geodesic words):\n");
    for (c = 0; c < 3; c++) {
        int n = 0;

        for (i = 0; i < nk; i++) if (ks[i].cls == c) n++;
        printf("    %s (%d classes)\n      %3s %8s %8s %8s %8s\n",
            cls_name[c], n, "m", "pieces", "r min", "r max", "r rep");
        for (m = 1; m <= MMAX; m++) {
            long sp = 0, s1 = 0, s2 = 0, s3 = 0;

            for (i = 0; i < nk; i++) {
                if (ks[i].cls != c) continue;
                sp += ks[i].pieces[m];
                s1 += ks[i].rmin[m];
                s2 += ks[i].rmax[m];
                s3 += ks[i].rrep[m];
            }
            mean_text(sp, n, b1);
            mean_text(s1, n, b2);
            mean_text(s2, n, b3);
            mean_text(s3, n, b4);
            printf("      %3d %8s %8s %8s %8s\n", m, b1, b2, b3, b4);
        }
    }

    /* pieces at m = MMAX, by type */
    printf("\n  Pieces p(%d) by type (count of classes per value):\n", MMAX);
    for (c = 0; c < 3; c++) {
        int v;

        printf("    %s:", cls_name[c]);
        for (v = 1; v <= 64; v++) {
            int n = 0;

            for (i = 0; i < nk; i++) {
                if (ks[i].cls == c && ks[i].pieces[MMAX] == v) n++;
            }
            if (n) printf("  %d:%d", v, n);
        }
        printf("\n");
    }
    /* E: regions added over m = 6..12 (most-regions word) vs pieces */
    printf("\n  E: regions added over m = 6..%d by the most-regions word, "
        "r(%d) - r(6),\n  against the pieces p(%d) (count of classes):\n",
        MMAX, MMAX, MMAX);
    {
        int d;

        for (d = 0; d <= 64; d++) {
            int v, any = 0;

            for (v = 1; v <= 64; v++) {
                int n = 0;

                for (i = 0; i < nk; i++) {
                    if (ks[i].cls == CLS_E && ks[i].pieces[MMAX] == v
                            && ks[i].rbest[MMAX] - ks[i].rbest[6] == d) {
                        n++;
                    }
                }
                if (n) {
                    if (!any) printf("    added %2d:", d);
                    any = 1;
                    printf("  p=%d:%d", v, n);
                }
            }
            if (any) printf("\n");
        }
    }

    /* exploratory (post hoc, not registered): is the eventual growth
     * per pass the cyclically reduced length of the word? */
    {
        int n = 0, eq = 0, lt = 0, gt = 0;

        for (i = 0; i < nk; i++) {
            int added, cyc;

            if (ks[i].cls != CLS_E) continue;
            n++;
            added = ks[i].rbest[MMAX] - ks[i].rbest[6];
            cyc = cyclic_length(geo[ks[i].best]);
            if (added == (MMAX - 6) * cyc) eq++;
            else if (added < (MMAX - 6) * cyc) lt++;
            else gt++;
        }
        printf("\n  E, exploratory: r(%d) - r(6) of the most-regions word "
            "against %d x its\n  cyclically reduced length: equal %d, "
            "below %d, above %d (of %d)\n", MMAX, MMAX - 6, eq, lt, gt, n);
    }

    /* word dependence */
    printf("\n  Word dependence at m = %d (classes with r min < r max):\n",
        MMAX);
    for (c = 0; c < 3; c++) {
        int n = 0, differ = 0, gap = 0, several = 0;

        for (i = 0; i < nk; i++) {
            if (ks[i].cls != c) continue;
            n++;
            if (ks[i].ngeo > 1) several++;
            if (ks[i].rmin[MMAX] < ks[i].rmax[MMAX]) differ++;
            if (ks[i].rmax[MMAX] - ks[i].rmin[MMAX] > gap) {
                gap = ks[i].rmax[MMAX] - ks[i].rmin[MMAX];
            }
        }
        printf("    %s: %d of %d (with > 1 geodesic word: %d); largest gap "
            "%d\n", cls_name[c], differ, n, several, gap);
    }
    {
        Plexus q = braid(3, "2 1 2 1");
        chorda key;
        chorda k2;

        plexus_clavis(q, pool, &key);
        for (i = 0; i < nk; i++) {
            plexus_clavis(ks[i].word, pool, &k2);
            if (chorda_aequalis(key, k2)) break;
        }
        if (i < nk) {
            int g;
            int saved = ks[i].null_sectors;

            printf("    Delta sigma_1 (%d geodesic words), r(m) m = 1..6 "
                "and pieces:\n", ks[i].ngeo);
            for (g = ks[i].geo0; g < ks[i].geo0 + ks[i].ngeo; g++) {
                int  r[MMAX + 1], p[MMAX + 1];
                char w[64];

                sweep_word(&ks[i], g, r, p, 0);
                word_text(geo[g], w, 64);
                printf("      [%s]%s  r: %d %d %d %d %d %d   p: %d %d %d %d "
                    "%d %d\n", w, g == ks[i].rep ? " (census)" : "", r[1],
                    r[2], r[3], r[4], r[5], r[6], p[1], p[2], p[3], p[4],
                    p[5], p[6]);
            }
            ks[i].null_sectors = saved;  /* the reprint counted again */
        }
    }
}

/* the rule's code on hand sequences (index 0 unused) */
static void
check_rule (void)
{
    int lin[MMAX + 1], per2[MMAX + 1], quad[MMAX + 1], dbl[MMAX + 1];
    int near[MMAX + 1];
    int m;

    for (m = 0; m <= MMAX; m++) {
        lin[m] = 5 + 3 * m;                      /* increments 3 */
        per2[m] = 2 * m + (m % 2);               /* increments 3, 1 */
        quad[m] = m * m;                         /* increments grow */
        dbl[m] = 1 << m;                         /* ratio 2 */
        near[m] = m;
    }
    /* ratio exactly 1.5 at m = 8..11: 16 * 3^(m-8) * 2^(12-m) */
    for (m = 8; m <= MMAX; m++) {
        int t, v = 16;

        for (t = 8; t < m; t++) v *= 3;
        for (t = m; t < 12; t++) v *= 2;
        near[m] = v;
    }
    check("rule: H_lin on a line and on period-2 increments, not on m^2",
        h_lin(lin) && h_lin(per2) && !h_lin(quad));
    check("rule: H_exp on 2^m and at ratio exactly 1.5, not on a line",
        h_exp(dbl) && h_exp(near) && !h_exp(lin));
}

static void
part_a_rule (void)
{
    int c, i;

    printf("\n  Registered rule over m = 6..%d (H_lin: increments repeat "
        "with period <= 3;\n  H_exp: ratio >= 1.5 for every m in 8..11), "
        "on pieces and on the geodesic\n  word with the most regions at "
        "m = %d:\n", MMAX, MMAX);
    printf("    %4s %8s %10s %10s %8s %10s %10s\n", "type", "classes",
        "lin(p)", "lin(r)", "lin both", "exp(p)", "exp(r)");
    for (c = 0; c < 3; c++) {
        int n = 0, lp = 0, lr = 0, lb = 0, ep = 0, er = 0;

        for (i = 0; i < nk; i++) {
            int a, b;

            if (ks[i].cls != c) continue;
            n++;
            a = h_lin(ks[i].pieces);
            b = h_lin(ks[i].rbest);
            lp += a;
            lr += b;
            lb += a && b;
            ep += h_exp(ks[i].pieces);
            er += h_exp(ks[i].rbest);
        }
        printf("    %4s %8d %10d %10d %8d %10d %10d\n", cls_name[c], n, lp,
            lr, lb, ep, er);
        if (c == CLS_E) {
            printf("    prediction (H_lin on both for >= 90%% of E, H_exp "
                "for none): %s\n", (10 * lb >= 9 * n && ep == 0 && er == 0)
                ? "HOLDS" : "FAILS");
        }
    }
    /* E by trace: growth per pass of pieces and of max regions */
    printf("\n  E classes by |tr| (lambda = (|tr| + sqrt(tr^2 - 4)) / 2): "
        "mean growth\n  per pass over m = 6..%d, (s(%d) - s(6)) / 6:\n",
        MMAX, MMAX);
    printf("    %5s %8s %10s %12s\n", "|tr|", "classes", "pieces",
        "max regions");
    {
        long t;
        long top = 0;

        for (i = 0; i < nk; i++) {
            long a = ks[i].tr < 0 ? -ks[i].tr : ks[i].tr;

            if (ks[i].cls == CLS_E && a > top) top = a;
        }
        for (t = 3; t <= top; t++) {
            int  n = 0;
            long sp = 0, sr = 0;
            char b1[32], b2[32];

            for (i = 0; i < nk; i++) {
                long a = ks[i].tr < 0 ? -ks[i].tr : ks[i].tr;

                if (ks[i].cls != CLS_E || a != t) continue;
                n++;
                sp += ks[i].pieces[MMAX] - ks[i].pieces[6];
                sr += ks[i].rbest[MMAX] - ks[i].rbest[6];
            }
            if (n == 0) continue;
            mean_text(sp, 6L * n, b1);
            mean_text(sr, 6L * n, b2);
            printf("    %5ld %8d %10s %12s\n", t, n, b1, b2);
        }
    }
}

static void
part_a (void)
{
    printf("\n=== Part A: exact regions and pieces on B_3 ===\n");
    build_census();
    collect_geodesics();
    part_a_sweeps();
    check("regions never decrease in m (every geodesic word)", ok_monotone);
    check("every sector of the census word's w^m: evaluation at medius "
        "gives its pattern, nullae and M x", ok_sector_eval);
    check("... and M x equals the Dynnikov action m times (ansa_agere, "
        "not nervus)", ok_sector_dyn);
    check("pieces are a class invariant (every geodesic word, every m)",
        ok_pieces_class);
    check("periodic: r(m) = r(k) for m >= k (every geodesic word)",
        ok_periodic_r);
    check("periodic: w^k is the identity (one piece, M = I), "
        "p(m + k) = p(m)", ok_periodic_p);
    part_a_report();
    check_rule();
    part_a_rule();
}

/* ================================================================
 * Part B: the lattice lower bound on B_3
 * ================================================================
 *
 * Points of the shells max(|x|, |y|) = R. A point is INTERIOR at m if
 * no ReLU argument was exactly 0 in passes 1..m. Its itinerary at m is
 * interned by refinement: id_m = intern(id_{m-1}, pattern of pass m).
 * Distinct interior itineraries are distinct full-dimensional regions,
 * so their number is a lower bound on r(m); the union over shells
 * R' <= R grows with R. The exact sectors (Part A, census word) check
 * every interior point: it lies strictly inside one sector whose
 * pattern equals its itinerary. */

static int shells[4] = { 4, 16, 64, 256 };
static int nshells = 4;

typedef struct {
    long   x, y;
    int    shell;      /* index into shells */
    int    half;       /* 0: angle in [0, pi), 1: [pi, 2 pi) */
    int    times4;     /* index of 4 p (next shell), or -1 */
} Point;

static Point *pts;
static int    npts;

/* exact angular order */
static int
point_cmp (
    const void *a,
    const void *b)
{
    const Point *p = (const Point *)a;
    const Point *q = (const Point *)b;
    long         cr;

    if (p->half != q->half) return p->half - q->half;
    cr = p->x * q->y - p->y * q->x;
    if (cr > 0) return -1;
    if (cr < 0) return 1;
    /* same ray: smaller shell first */
    return p->shell - q->shell;
}

static void
make_points (void)
{
    int s;

    npts = 0;
    for (s = 0; s < nshells; s++) npts += 8 * shells[s];
    pts = (Point *)malloc((size_t)npts * sizeof(Point));
    npts = 0;
    for (s = 0; s < nshells; s++) {
        long R = shells[s];
        long t;

        for (t = -R; t < R; t++) {
            long xy[4][2];
            int  k;

            xy[0][0] = R;  xy[0][1] = t;
            xy[1][0] = -t; xy[1][1] = R;
            xy[2][0] = -R; xy[2][1] = -t;
            xy[3][0] = t;  xy[3][1] = -R;
            for (k = 0; k < 4; k++) {
                Point *p = &pts[npts++];

                p->x = xy[k][0];
                p->y = xy[k][1];
                p->shell = s;
                p->half = (p->y > 0 || (p->y == 0 && p->x > 0)) ? 0 : 1;
            }
        }
    }
    qsort(pts, (size_t)npts, sizeof(Point), point_cmp);
    /* 4 p sits on the same ray, one shell further: in angular order the
     * points of one ray are adjacent, by shell */
    {
        int i;

        for (i = 0; i < npts; i++) {
            int j;

            pts[i].times4 = -1;
            for (j = i + 1; j < npts && j <= i + nshells; j++) {
                if (pts[j].x == 4 * pts[i].x && pts[j].y == 4 * pts[i].y) {
                    pts[i].times4 = j;
                }
            }
        }
    }
}

/* angle(u) against angle(x, y) in [0, 2 pi): -1, 0, +1 */
static int
ray_vs_point (
    NervusRadius u,
    long         x,
    long         y)
{
    int hu = (magnus_signum(u.y) > 0 || (magnus_signum(u.y) == 0
        && magnus_signum(u.x) > 0)) ? 0 : 1;
    int hp = (y > 0 || (y == 0 && x > 0)) ? 0 : 1;
    int cr;

    if (hu != hp) return hu < hp ? -1 : 1;
    cr = magnus_signum(magnus_subtrahe(magnus_multiplica(u.x,
        magnus_ex_s64(y), pool), magnus_multiplica(u.y, magnus_ex_s64(x),
        pool), pool));
    return -cr;
}

/* sign of cross(u, (x, y)) */
static int
cross_sign (
    NervusRadius u,
    long         x,
    long         y)
{
    return magnus_signum(magnus_subtrahe(magnus_multiplica(u.x,
        magnus_ex_s64(y), pool), magnus_multiplica(u.y, magnus_ex_s64(x),
        pool), pool));
}

/* strictly inside sector s (angle <= pi, or the whole plane) */
static int
inside (
    const NervusPlanum *pl,
    int                 k,
    long                x,
    long                y)
{
    const NervusSector *s = &pl->regiones[k];
    int                 a, b, uw;

    if (pl->numerus == 1) return 1;
    a = cross_sign(s->initium, x, y);
    uw = magnus_signum(magnus_subtrahe(magnus_multiplica(s->initium.x,
        s->finis.y, pool), magnus_multiplica(s->initium.y, s->finis.x,
        pool), pool));
    if (uw == 0) return a > 0;        /* half-plane */
    b = -cross_sign(s->finis, x, y);
    return a > 0 && b > 0;
}

static int ok_lattice_le = 1;     /* lattice count <= exact r(m) */
static int ok_lattice_sector = 1; /* interior point: inside one sector,
                                   * same pattern */
static int ok_homogeneous = 1;    /* id(4 p) = id(p) */
static int ok_union = 1;          /* union over shells nondecreasing */
static int ok_ids_sectors = 1;    /* distinct itineraries = distinct
                                   * sectors hit (all shells) */
static long homog_pairs = 0;

static long tot_points = 0;
static long tot_wall = 0;         /* points with nullae > 0 by m = MMAX */

/* per class: lattice counts low[s][m] (union of shells <= s) */
static void
lattice_class (
    Klass *kl,
    int    low[4][MMAX + 1])
{
    Piscina        *cpool = piscina_generare_dynamicum("d129_class",
        1 << 16);
    Piscina        *vpool[2];
    Nervus         *one;
    Nervus         *net;
    NervusPlanum    pl[MMAX + 1];
    TabulaDispersa *tab = tabula_dispersa_creare_chorda(cpool, 1024);
    int            *ids = (int *)malloc((size_t)npts * sizeof(int));
    int            *alive = (int *)malloc((size_t)npts * sizeof(int));
    Magnus         *vx = (Magnus *)malloc((size_t)npts * sizeof(Magnus));
    Magnus         *vy = (Magnus *)malloc((size_t)npts * sizeof(Magnus));
    unsigned char  *bits;            /* per point, all passes */
    int            *store;           /* id values */
    int            *seen;
    int             nstore = 0;
    int             capstore = npts * MMAX + 1;
    int             nrelu;
    int             width;
    int             m, i, s;

    vpool[0] = piscina_generare_dynamicum("d129_v0", 1 << 16);
    vpool[1] = piscina_generare_dynamicum("d129_v1", 1 << 16);
    if (!plexus_nervus(kl->word, cpool, &one)) fatal("network refused");
    nrelu = (int)one->relu;
    width = nrelu * MMAX;
    bits = (unsigned char *)malloc((size_t)npts * (size_t)(width + 1));
    store = (int *)malloc((size_t)capstore * sizeof(int));
    seen = (int *)calloc((size_t)capstore + 1, sizeof(int));
    net = one;
    for (m = 1; m <= MMAX; m++) {
        if (m > 1 && !nervus_componere(net, one, cpool, &net)) {
            fatal("composition refused");
        }
        if (!nervus_regiones_plani(net, cpool, &pl[m])) {
            fatal("sweep refused");
        }
    }
    for (i = 0; i < npts; i++) {
        ids[i] = 0;
        alive[i] = 1;
        vx[i] = magnus_ex_s64(pts[i].x);
        vy[i] = magnus_ex_s64(pts[i].y);
    }
    for (m = 1; m <= MMAX; m++) {
        Piscina *vp = vpool[m % 2];

        piscina_vacare(vp);       /* held pass m - 2 */
        for (i = 0; i < npts; i++) {
            PiscinaNotatio mark = piscina_notare(pool);
            NervusForma    f;
            Magnus         in[2];
            Magnus         out[2];
            char           keybuf[16];
            chorda         key;
            vacuum        *val = NULL;
            int            b;

            in[0] = vx[i];
            in[1] = vy[i];
            if (!nervus_forma_creare(one, pool, &f)
                    || !nervus_aestimare(one, in, out, &f, pool)) {
                fatal("evaluation refused");
            }
            vx[i] = magnus_transcribe(out[0], vp);
            vy[i] = magnus_transcribe(out[1], vp);
            if (f.nullae > 0) alive[i] = 0;
            for (b = 0; b < nrelu; b++) {
                bits[(size_t)i * (size_t)(width + 1)
                    + (size_t)((m - 1) * nrelu + b)]
                    = nervus_forma_bitus(&f, (i32)b) ? 1 : 0;
            }
            if (alive[i]) {
                /* key: previous id (4 bytes) + this pass's pattern */
                memcpy(keybuf, &ids[i], 4);
                for (b = 0; b < (nrelu + 7) / 8; b++) {
                    keybuf[4 + b] = (char)f.bitus[b];
                }
                if (nrelu % 8) {
                    keybuf[4 + (nrelu - 1) / 8] = (char)(keybuf[4
                        + (nrelu - 1) / 8] & ((1 << (nrelu % 8)) - 1));
                }
                key.datum = (i8 *)keybuf;
                key.mensura = (i32)(4 + (nrelu + 7) / 8);
                if (tabula_dispersa_invenire(tab, key, &val)) {
                    ids[i] = *(int *)val;
                } else {
                    chorda copy = chorda_transcribere(key, cpool);

                    if (nstore >= capstore) fatal("itinerary store full");
                    store[nstore] = nstore + 1;
                    tabula_dispersa_inserere(tab, copy, &store[nstore]);
                    ids[i] = nstore + 1;
                    nstore++;
                }
            }
            piscina_reficere(pool, mark);
        }
        /* counts: distinct ids among interior points of shells <= s */
        for (s = 0; s < nshells; s++) {
            int count = 0;
            int tag = 1 + s + 8 * m;

            for (i = 0; i < npts; i++) {
                if (!alive[i] || pts[i].shell > s) continue;
                if (seen[ids[i]] != tag) {
                    seen[ids[i]] = tag;
                    count++;
                }
            }
            low[s][m] = count;
            if (count > kl->rrep[m]) ok_lattice_le = 0;
            if (s > 0 && count < low[s - 1][m]) ok_union = 0;
        }
        /* angular walk: each interior point strictly inside the sector
         * whose wall precedes it, with its itinerary as pattern */
        {
            const NervusPlanum *q = &pl[m];
            int                 j = 0;
            int                *hit = (int *)calloc((size_t)q->numerus,
                sizeof(int));
            int                 nhit = 0;

            for (i = 0; i < npts; i++) {
                int k, b, on_wall = 0;

                while (j + 1 < (int)q->numerus && ray_vs_point(
                        q->regiones[j + 1].initium, pts[i].x,
                        pts[i].y) <= 0) {
                    j++;
                }
                k = j;
                if (q->numerus > 1) {
                    int c0 = ray_vs_point(q->regiones[j].initium, pts[i].x,
                        pts[i].y);

                    if (c0 > 0) k = (int)q->numerus - 1;   /* wraps */
                    if (c0 == 0) on_wall = 1;
                }
                if (!alive[i]) continue;
                if (on_wall || !inside(q, k, pts[i].x, pts[i].y)) {
                    ok_lattice_sector = 0;
                    continue;
                }
                for (b = 0; b < nrelu * m; b++) {
                    if (bits[(size_t)i * (size_t)(width + 1) + (size_t)b]
                            != (nervus_forma_bitus(&q->regiones[k].forma,
                            (i32)b) ? 1 : 0)) {
                        ok_lattice_sector = 0;
                    }
                }
                if (!hit[k]) {
                    hit[k] = 1;
                    nhit++;
                }
            }
            if (nhit != low[nshells - 1][m]) ok_ids_sectors = 0;
            free(hit);
        }
    }
    /* homogeneity: 4 p (next shell) has p's itinerary */
    for (i = 0; i < npts; i++) {
        int j = pts[i].times4;

        if (j < 0) continue;
        homog_pairs++;
        if (ids[j] != ids[i] || alive[j] != alive[i]) ok_homogeneous = 0;
    }
    for (i = 0; i < npts; i++) {
        tot_points++;
        if (!alive[i]) tot_wall++;
    }
    free(seen);
    free(store);
    free(bits);
    free(vy);
    free(vx);
    free(alive);
    free(ids);
    piscina_destruere(vpool[0]);
    piscina_destruere(vpool[1]);
    piscina_destruere(cpool);
}

static void
part_b (void)
{
    int  i, s, m, c;
    long sum_exact[3][MMAX + 1];
    long sum_low[3][4][MMAX + 1];
    int  ncls[3] = { 0, 0, 0 };
    int  reach[3][4];             /* classes whose bound reaches r(12) */
    char b1[32];

    printf("\n=== Part B: the lattice lower bound on B_3 ===\n");
    make_points();
    memset(sum_exact, 0, sizeof(sum_exact));
    memset(sum_low, 0, sizeof(sum_low));
    memset(reach, 0, sizeof(reach));
    for (i = 0; i < nk; i++) {
        int low[4][MMAX + 1];

        lattice_class(&ks[i], low);
        ncls[ks[i].cls]++;
        for (m = 1; m <= MMAX; m++) {
            sum_exact[ks[i].cls][m] += ks[i].rrep[m];
            for (s = 0; s < nshells; s++) {
                sum_low[ks[i].cls][s][m] += low[s][m];
            }
        }
        for (s = 0; s < nshells; s++) {
            if (low[s][MMAX] == ks[i].rrep[MMAX]) reach[ks[i].cls][s]++;
        }
    }
    check("lattice count <= exact r(m) (census word, every shell, every m)",
        ok_lattice_le);
    check("every interior lattice point lies strictly inside one exact "
        "sector with its itinerary as pattern", ok_lattice_sector);
    check("union over shells never decreases", ok_union);
    check("distinct interior itineraries = distinct sectors they lie in "
        "(the refinement is exact)", ok_ids_sectors);
    {
        long expect = 0;

        for (s = 0; s + 1 < nshells; s++) expect += 8L * shells[s];
        check("homogeneity: 4 p has p's itinerary and interiority (every "
            "point of every inner shell, every class)", ok_homogeneous
            && homog_pairs == expect * nk);
    }
    printf("\n  Points on walls by m = %d (some argument exactly 0 in some "
        "pass): %ld of %ld\n", MMAX, tot_wall, tot_points);
    printf("\n  Mean regions of the census word at m = %d: exact vs lattice "
        "(shells <= R):\n", MMAX);
    printf("    %4s %8s %8s", "type", "classes", "exact");
    for (s = 0; s < nshells; s++) printf("   R<=%-4d", shells[s]);
    printf("\n");
    for (c = 0; c < 3; c++) {
        mean_text(sum_exact[c][MMAX], ncls[c], b1);
        printf("    %4s %8d %8s", cls_name[c], ncls[c], b1);
        for (s = 0; s < nshells; s++) {
            mean_text(sum_low[c][s][MMAX], ncls[c], b1);
            printf(" %9s", b1);
        }
        printf("\n");
    }
    printf("\n  Classes whose lattice bound reaches the exact r(%d):\n",
        MMAX);
    for (c = 0; c < 3; c++) {
        printf("    %s:", cls_name[c]);
        for (s = 0; s < nshells; s++) {
            printf("  R<=%d %d/%d", shells[s], reach[c][s], ncls[c]);
        }
        printf("\n");
    }
}

/* ================================================================
 * main
 * ================================================================ */

int
main (void)
{
    const char *q = getenv("D129_CELER");

    pool = piscina_generare_dynamicum("demo129", 1 << 20);
    if (q && q[0] == '1') {
        celer = 1;
        maxlen = 5;
        nshells = 2;
    }
    printf("KNOTAPEL DEMO 129: linear regions of braid networks%s\n",
        celer ? " (CELER)" : "");
    part_a();
    part_b();
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    return n_fail ? 1 : 0;
}
