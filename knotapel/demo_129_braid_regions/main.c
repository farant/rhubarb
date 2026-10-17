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
#include "sors.h"
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
    if (sizeof(int) != 4 || nrelu > 96) fatal("itinerary key too small");
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
 * Part C: B_4 - a weak lattice bound, and the period-2 question
 * ================================================================
 *
 * Classes on B_4 (D128's method, all certified up to length 6): P iff
 * plexus_periodus > 0; E iff Burau(-1) has spectral radius > 1 or
 * Burau at a primitive cube root of unity has radius > 1 (Kolev's
 * bound, plexus); Z the rest (D128 checked their growth is
 * quasi-affine). */

static const int pin4_P[7] = { 1, 0, 0, 8, 24, 36, 102 };
static const int pin4_E[7] = { 0, 0, 8, 48, 198, 802, 2958 };
static const int pin4_Z[7] = { 0, 6, 18, 42, 116, 272, 482 };

typedef struct {
    Plexus word;
    int    len;
    int    cls;
    int    cube_only;      /* E certified only at a cube root */
} Klass4;

static Klass4 *k4;
static int     nk4;

static void
build_census4 (
    int len4)
{
    PlexusCensus cs;
    int          i;
    int          len;
    int          ok = 1;

    if (!plexus_enumerare(4, (i32)len4, 100000, pool, &cs)) {
        fatal("B_4 census refused");
    }
    nk4 = (int)cs.numerus;
    k4 = (Klass4 *)calloc((size_t)nk4, sizeof(Klass4));
    for (i = 0; i < nk4; i++) {
        i32         period = 0;
        RadixRealis rho;
        s32         sg = 0;
        b32         cube = FALSUM;

        k4[i].word = cs.verba[i];
        k4[i].len = (int)cs.verba[i].longitudo;
        if (!plexus_periodus(k4[i].word, pool, &period)
                || !plexus_burau_radix_maxima(k4[i].word, pool, &rho)
                || !radix_compara(rho, radix_ex_fractione(fractio_ex_s64(1),
                    pool), pool, &sg)
                || !plexus_burau_radius_maior(k4[i].word, 3, &cube)) {
            fatal("B_4 classification refused");
        }
        if (period > 0) {
            k4[i].cls = CLS_P;
        } else if (sg > 0 || cube) {
            k4[i].cls = CLS_E;
            k4[i].cube_only = sg > 0 ? 0 : 1;
        } else {
            k4[i].cls = CLS_Z;
        }
    }
    for (len = 0; len <= len4; len++) {
        int p = 0, e = 0, z = 0;

        for (i = 0; i < nk4; i++) {
            if (k4[i].len != len) continue;
            if (k4[i].cls == CLS_P) p++;
            else if (k4[i].cls == CLS_E) e++;
            else z++;
        }
        if (p != pin4_P[len] || e != pin4_E[len] || z != pin4_Z[len]) {
            ok = 0;
        }
    }
    check("B_4 classes per length = D128's table", ok);
}

/* ---------------- the weak lattice bound in d = 4 ---------------- */

#define L4_MMAX 8

static int ok4_monotone = 1;      /* refinement: count(m+1) >= count(m) */
static int ok4_union = 1;
static int ok4_homog = 1;         /* 2 p (shell 8) has p's itinerary */
static long homog4_pairs = 0;

/* the points of the shells max |x_i| = 4 and = 8 (shell 0 / 1) */
static long (*pts4)[4];
static int  *shell4;
static int  *twice4;              /* index of 2 p, or -1 */
static int   npts4;

static int
shell_index (
    const long *x,
    long        R)
{
    /* rank of x in the box [-R, R]^4, for the 2 p lookup */
    long w = 2 * R + 1;

    return (int)((((x[0] + R) * w + (x[1] + R)) * w + (x[2] + R)) * w
        + (x[3] + R));
}

static long shells4[2] = { 4, 8 };   /* quick mode: 2, 4 */

static void
make_points4 (
    int nsh)
{
    int   s, i;
    int  *rank8;
    long *sizes = shells4;

    npts4 = 0;
    for (s = 0; s < nsh; s++) {
        long R = sizes[s];
        long a = (2 * R + 1), b = (2 * R - 1);

        npts4 += (int)(a * a * a * a - b * b * b * b);
    }
    pts4 = (long (*)[4])malloc((size_t)npts4 * sizeof(*pts4));
    shell4 = (int *)malloc((size_t)npts4 * sizeof(int));
    twice4 = (int *)malloc((size_t)npts4 * sizeof(int));
    npts4 = 0;
    for (s = 0; s < nsh; s++) {
        long R = sizes[s];
        long x[4];

        for (x[0] = -R; x[0] <= R; x[0]++)
        for (x[1] = -R; x[1] <= R; x[1]++)
        for (x[2] = -R; x[2] <= R; x[2]++)
        for (x[3] = -R; x[3] <= R; x[3]++) {
            long mx = 0;
            int  t;

            for (t = 0; t < 4; t++) {
                long a = x[t] < 0 ? -x[t] : x[t];

                if (a > mx) mx = a;
            }
            if (mx != R) continue;
            for (t = 0; t < 4; t++) pts4[npts4][t] = x[t];
            shell4[npts4] = s;
            npts4++;
        }
    }
    /* 2 p for p on shell 4 lies on shell 8 */
    {
        long w = 2 * shells4[1] + 1;
        long box = w * w * w * w;

        rank8 = (int *)malloc((size_t)box * sizeof(int));
        for (i = 0; i < (int)box; i++) rank8[i] = -1;
    }
    for (i = 0; i < npts4; i++) {
        if (shell4[i] == 1) rank8[shell_index(pts4[i], shells4[1])] = i;
    }
    for (i = 0; i < npts4; i++) {
        long y[4];
        int  t;

        twice4[i] = -1;
        if (shell4[i] != 0 || nsh < 2) continue;
        for (t = 0; t < 4; t++) y[t] = 2 * pts4[i][t];
        twice4[i] = rank8[shell_index(y, shells4[1])];
    }
    free(rank8);
}

/* lattice counts low[s][m], m = 1..L4_MMAX, for one B_4 word */
static void
lattice4_class (
    Plexus w,
    int    nsh,
    int    low[2][L4_MMAX + 1])
{
    Piscina        *cpool = piscina_generare_dynamicum("d129_c4", 1 << 16);
    Piscina        *vpool[2];
    Nervus         *one;
    TabulaDispersa *tab = tabula_dispersa_creare_chorda(cpool, 4096);
    int            *ids = (int *)malloc((size_t)npts4 * sizeof(int));
    int            *prev = (int *)malloc((size_t)npts4 * sizeof(int));
    int            *alive = (int *)malloc((size_t)npts4 * sizeof(int));
    Magnus        (*v)[4] = (Magnus (*)[4])malloc((size_t)npts4
        * sizeof(*v));
    int            *store;
    int            *seen;
    int             nstore = 0;
    int             capstore = npts4 * L4_MMAX + 1;
    int             nrelu;
    int             m, i, s;

    vpool[0] = piscina_generare_dynamicum("d129_c4v0", 1 << 16);
    vpool[1] = piscina_generare_dynamicum("d129_c4v1", 1 << 16);
    if (!plexus_nervus(w, cpool, &one)) fatal("network refused");
    nrelu = (int)one->relu;
    if (sizeof(int) != 4 || nrelu > 96) fatal("itinerary key too small");
    store = (int *)malloc((size_t)capstore * sizeof(int));
    seen = (int *)calloc((size_t)capstore + 1, sizeof(int));
    for (i = 0; i < npts4; i++) {
        int t;

        ids[i] = 0;
        alive[i] = 1;
        for (t = 0; t < 4; t++) v[i][t] = magnus_ex_s64(pts4[i][t]);
    }
    for (m = 1; m <= L4_MMAX; m++) {
        Piscina *vp = vpool[m % 2];

        piscina_vacare(vp);
        for (i = 0; i < npts4; i++) {
            PiscinaNotatio mark = piscina_notare(pool);
            NervusForma    f;
            Magnus         out[4];
            char           keybuf[16];
            chorda         key;
            vacuum        *val = NULL;
            int            b, t;

            if (!nervus_forma_creare(one, pool, &f)
                    || !nervus_aestimare(one, v[i], out, &f, pool)) {
                fatal("evaluation refused");
            }
            prev[i] = ids[i];
            for (t = 0; t < 4; t++) v[i][t] = magnus_transcribe(out[t], vp);
            if (f.nullae > 0) alive[i] = 0;
            if (alive[i]) {
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
        for (s = 0; s < nsh; s++) {
            int count = 0;
            int tag = 1 + s + 4 * m;

            for (i = 0; i < npts4; i++) {
                if (!alive[i] || shell4[i] > s) continue;
                if (seen[ids[i]] != tag) {
                    seen[ids[i]] = tag;
                    count++;
                }
            }
            low[s][m] = count;
            if (s > 0 && count < low[s - 1][m]) ok4_union = 0;
        }
        /* refinement: among points still interior, the m-itineraries
         * are at least as many as the (m-1)-itineraries */
        if (m > 1) {
            int now = 0, before = 0;

            for (i = 0; i < npts4; i++) {
                if (!alive[i]) continue;
                if (seen[ids[i]] != -m) {
                    seen[ids[i]] = -m;
                    now++;
                }
                if (seen[prev[i]] != -m - 100) {
                    seen[prev[i]] = -m - 100;
                    before++;
                }
            }
            if (now < before) ok4_monotone = 0;
        }
    }
    for (i = 0; i < npts4; i++) {
        int j = twice4[i];

        if (j < 0) continue;
        homog4_pairs++;
        if (ids[j] != ids[i] || alive[j] != alive[i]) ok4_homog = 0;
    }
    free(seen);
    free(store);
    free(v);
    free(alive);
    free(prev);
    free(ids);
    piscina_destruere(vpool[0]);
    piscina_destruere(vpool[1]);
    piscina_destruere(cpool);
}

static void
part_c_lattice (
    int len4,
    int nsh)
{
    long sum[3][2][L4_MMAX + 1];
    int  ncls[3] = { 0, 0, 0 };
    int  lin[3] = { 0, 0, 0 }, ex[3] = { 0, 0, 0 };
    int  i, s, m, c;
    char b1[32];

    printf("\n  (a) Weak lattice LOWER bound on B_4 up to length %d, shells "
        "R = %ld, %ld, m = 1..%d.\n  Part B showed this bound misses most "
        "regions of E braids on B_3 (13 of 88 at\n  R <= 256): read it "
        "as a floor, never as a count.\n", len4, shells4[0], shells4[1],
        L4_MMAX);
    make_points4(nsh);
    memset(sum, 0, sizeof(sum));
    for (i = 0; i < nk4; i++) {
        int low[2][L4_MMAX + 1];
        int seq[MMAX + 1];

        if (k4[i].len > len4) continue;
        lattice4_class(k4[i].word, nsh, low);
        c = k4[i].cls;
        ncls[c]++;
        for (m = 1; m <= L4_MMAX; m++) {
            for (s = 0; s < nsh; s++) sum[c][s][m] += low[s][m];
        }
        /* the rule's shapes on the bound (m = 4..8): increments repeat
         * with period <= 2, or ratio >= 1.5 at m = 6..7 */
        memset(seq, 0, sizeof(seq));
        for (m = 1; m <= L4_MMAX; m++) seq[m] = low[nsh - 1][m];
        {
            int q, okq = 0;

            for (q = 1; q <= 2 && !okq; q++) {
                int t, all = 1;

                for (t = 4; t + q + 1 <= L4_MMAX; t++) {
                    if (seq[t + q + 1] - seq[t + q] != seq[t + 1] - seq[t]) {
                        all = 0;
                    }
                }
                okq = all;
            }
            lin[c] += okq;
            ex[c] += 2 * seq[7] >= 3 * seq[6] && 2 * seq[8] >= 3 * seq[7];
        }
    }
    check("B_4 lattice: itineraries refine (points still interior keep "
        "at least their earlier\n        distinct itineraries), and the "
        "shell union never decreases", ok4_monotone && ok4_union);
    {
        long expect = 0;

        for (i = 0; i < npts4; i++) if (twice4[i] >= 0) expect++;
        check("B_4 lattice: homogeneity, 2 p has p's itinerary (every point "
            "of the inner shell, every class)", ok4_homog && (nsh < 2
            || (expect > 0 && homog4_pairs == expect * (long)(ncls[0]
            + ncls[1] + ncls[2]))));
    }
    printf("    %4s %8s", "type", "classes");
    for (m = 1; m <= L4_MMAX; m++) printf(" %6d", m);
    printf("   lin   exp\n");
    for (c = 0; c < 3; c++) {
        printf("    %4s %8d", cls_name[c], ncls[c]);
        for (m = 1; m <= L4_MMAX; m++) {
            mean_text(sum[c][nsh - 1][m], ncls[c], b1);
            printf(" %6s", b1);
        }
        printf(" %5d %5d\n", lin[c], ex[c]);
    }
    printf("  (mean lattice count, shells <= R = %ld; lin = increments over "
        "m = 4..8 repeat with\n  period <= 2, exp = ratio >= 1.5 at m = 6 "
        "and 7: shapes of a FLOOR, consistent-with only)\n",
        shells4[nsh - 1]);
}

/* ---------------- the period-2 question ---------------- */

#define O_PASSES 60
#define O_QMAX   12
#define O_TAIL   20
#define O_TERMS  12

/* minimal polynomial of a vector sequence y_0, y_1, ... (dim rows):
 * the first d with a kernel of [y_0 .. y_d]; FALSE if none <= dmax */
static int
min_poly (
    Magnus     **y,         /* y[j][r] */
    int          rows,
    int          nterms,
    int          dmax,
    Polynomium  *out,
    int         *deg)
{
    int d;

    for (d = 0; d <= dmax && d < nterms; d++) {
        Matrix a;
        Matrix ker;
        int    r, j;

        /* scalar sequences: Hankel rows = shifts, so the relation holds
         * for every window that fits. Vectors: ONE window, the Krylov
         * vectors y_0 .. y_d themselves; if the tail is one linear map,
         * the first dependency is its minimal polynomial and holds on
         * every later window (the caller rechecks that) */
        if (rows == 1) {
            int nr = nterms - d;

            if (nr < d + 1) return 0;
            if (!matrix_nulla(&ANULUS_INTEGRORUM, (i32)nr, (i32)(d + 1),
                    pool, &a)) fatal("matrix refused");
            for (r = 0; r < nr; r++) {
                for (j = 0; j <= d; j++) {
                    matrix_pone(&a, (i32)r, (i32)j, &y[r + j][0]);
                }
            }
        } else {
            if (!matrix_nulla(&ANULUS_INTEGRORUM, (i32)rows, (i32)(d + 1),
                    pool, &a)) fatal("matrix refused");
            for (r = 0; r < rows; r++) {
                for (j = 0; j <= d; j++) {
                    matrix_pone(&a, (i32)r, (i32)j, &y[j][r]);
                }
            }
        }
        if (!matrix_nucleus(a, pool, &ker)) fatal("kernel refused");
        if (matrix_columnae(ker) > 0) {
            Magnus c[8];

            for (j = 0; j <= d; j++) {
                c[j] = *(const Magnus *)matrix_elementum(ker, (i32)j, 0);
            }
            if (!polynomium_ex_coefficientibus(c, (i32)(d + 1), 0, pool,
                    out)) fatal("polynomial refused");
            *deg = d;
            return 1;
        }
    }
    return 0;
}

/* the argument of ReLU k of w as its own network (prefix, then the
 * ReLU's sum as an output) */
static Nervus *
argument_net (
    const Nervus *net,
    int           k)
{
    Nervus *a;
    int     t, r = 0;
    s32     reg = -1;

    if (!nervus_creare(net->introitus, pool, &a)) fatal("net refused");
    for (t = 0; t < (int)net->numerus; t++) {
        const NervusInstructio *in = &net->instructiones[t];

        if (in->genus == NERVUS_RELU && r++ == k) {
            if (!nervus_affinis(a, in->termini, in->fontes,
                    in->coefficientes, in->addendum, &reg)) {
                fatal("argument refused");
            }
            break;
        }
        if (!(in->genus == NERVUS_RELU ? nervus_relu : nervus_affinis)(a,
                in->termini, in->fontes, in->coefficientes, in->addendum,
                &reg)) {
            fatal("copy refused");
        }
    }
    if (!nervus_exitus(a, 1, &reg)) fatal("exit refused");
    return a;
}

typedef struct {
    int         settled;
    int         start;
    int         period;
    int         tie;      /* some argument vanishes in the limit */
    int         ok_mu;    /* mu real, > 1, simple; orbit sees it */
    int         grows;    /* mu > 1; else the loop is invariant (its
                           * orbit does not grow: a reducing curve) */
    RadixRealis mu;       /* over the braid's period Q (lambda^Q) */
    Magnus     *x[O_PASSES + 1];
    NervusForma f[O_PASSES];  /* pattern of each pass */
} Orbit4;

static int ok_q_tie = 1;          /* theorem: q >= 2 => tie */
static int ok_mu_all = 1;
static int ok_same_limit = 1;     /* the loops of one braid agree */
static int ok_recurrence = 1;     /* the tail obeys pi_x */
static int ok_invariant = 1;      /* a loop that does not grow is fixed:
                                   * tail polynomial t - 1 */
static int ok_vanish_pos = 1;     /* vanishing is the same at every
                                   * position of the cycle */
static int ok_varies_vanish = 1;  /* a bit that varies on the tail has a
                                   * vanishing argument */

/* phase 1: the orbit of loop E_k and its settled period */
static void
orbit4_settle (
    Nervus *net,
    int     k,
    Orbit4 *o)
{
    chorda pat[O_PASSES];
    Ansa   e;
    int    t, q;

    memset(o, 0, sizeof(*o));
    if (!ansa_circa(4, (i32)k, pool, &e)) fatal("loop refused");
    o->x[0] = (Magnus *)malloc(4 * sizeof(Magnus));
    o->x[0][0] = e.a[0];
    o->x[0][1] = e.a[1];
    o->x[0][2] = e.b[0];
    o->x[0][3] = e.b[1];
    for (t = 0; t < O_PASSES; t++) {
        o->x[t + 1] = (Magnus *)malloc(4 * sizeof(Magnus));
        if (!nervus_forma_creare(net, pool, &o->f[t])
                || !nervus_aestimare(net, o->x[t], o->x[t + 1], &o->f[t],
                    pool)
                || !nervus_forma_clavis(&o->f[t], pool, &pat[t])) {
            fatal("orbit refused");
        }
    }
    for (q = 1; q <= O_QMAX && !o->settled; q++) {
        int s = 0;

        for (t = O_PASSES - 1 - q; t >= 0; t--) {
            if (!chorda_aequalis(pat[t], pat[t + q])) {
                s = t + 1;
                break;
            }
        }
        if (O_PASSES - s >= O_TAIL && O_PASSES - s >= 3 * q) {
            o->settled = 1;
            o->start = s;
            o->period = q;
        }
    }
}

/* phase 2 (E): mu over the braid's period Q (a multiple of the loop's
 * own q), and the tie test at every position of a Q-cycle */
static void
orbit4_limit (
    Nervus **args,
    int      nrelu,
    int      start,
    int      Q,
    Orbit4  *o)
{
    Magnus      *y[O_TERMS + 1];
    Polynomium   px;
    Polynomium   dpx;
    RadixRealis *roots;
    i32          nroots = 0;
    s32          sg;
    int          deg = 0;
    int          nt = 0;
    int          j, p, kk;

    for (j = 0; j <= O_TERMS && start + j * Q <= O_PASSES; j++) {
        y[j] = o->x[start + j * Q];
        nt++;
    }
    if (!min_poly(y, 4, nt, 4, &px, &deg) || deg < 1) {
        ok_mu_all = 0;
        return;
    }
    /* the relation holds on every later window */
    for (j = 0; j + deg < nt; j++) {
        int r;

        for (r = 0; r < 4; r++) {
            Magnus sum = magnus_ex_s64(0);
            int    i;

            for (i = 0; i <= deg; i++) {
                sum = magnus_adde(sum, magnus_multiplica(
                    polynomium_coefficiens(px, (s32)i), y[j + i][r], pool),
                    pool);
            }
            if (magnus_signum(sum) != 0) ok_recurrence = 0;
        }
    }
    if (!radices_reales(px, pool, &roots, &nroots) || nroots < 1) {
        ok_mu_all = 0;
        return;
    }
    o->mu = roots[nroots - 1];
    if (!radix_compara(o->mu, radix_ex_fractione(fractio_ex_s64(1), pool),
            pool, &sg)) {
        fatal("compare refused");
    }
    if (sg <= 0) {
        /* the orbit does not grow: this loop is invariant under a
         * reducible braid (E with a pseudo-Anosov PIECE); it does not
         * flow to the attractor and is left out of the limit. Invariant
         * means x fixed on the tail: the polynomial is t - 1 exactly
         * (a Jordan block at 1 would grow linearly; checked) */
        if (deg != 1 || !magnus_aequalis(polynomium_coefficiens(px, 0),
                magnus_nega(polynomium_coefficiens(px, 1), pool))) {
            ok_invariant = 0;
        }
        o->grows = 0;
        return;
    }
    o->grows = 1;
    if (!polynomium_derivata(px, pool, &dpx)
            || !radix_signum_polynomii(dpx, o->mu, pool, &sg) || sg == 0) {
        ok_mu_all = 0;     /* mu must be simple */
        return;
    }
    o->ok_mu = 1;
    /* each argument at each position of the Q-cycle: if mu is not a
     * root of its minimal polynomial, it vanishes against |x|. The
     * argument is a continuous function of the input and every
     * position tends to the same direction, so the answer must not
     * depend on the position (checked). */
    for (kk = 0; kk < nrelu; kk++) {
        int vanish = -1;
        int t;
        int varies = 0;

        for (p = 0; p < Q; p++) {
            Magnus     *a[O_TERMS + 1];
            Magnus      store[O_TERMS + 1];
            Polynomium  pa;
            int         zero = 1;
            int         v;

            nt = 0;
            for (j = 0; j <= O_TERMS && start + p + j * Q < O_PASSES; j++) {
                if (!nervus_aestimare(args[kk], o->x[start + p + j * Q],
                        &store[j], NULL, pool)) {
                    fatal("argument evaluation refused");
                }
                a[j] = &store[j];
                if (magnus_signum(store[j]) != 0) zero = 0;
                nt++;
            }
            if (zero) {
                v = 1;
            } else {
                if (!min_poly(a, 1, nt, 4, &pa, &deg)) {
                    ok_mu_all = 0;
                    continue;
                }
                if (!radix_signum_polynomii(pa, o->mu, pool, &sg)) {
                    fatal("sign refused");
                }
                v = sg != 0;
            }
            if (vanish >= 0 && v != vanish) ok_vanish_pos = 0;
            vanish = v;
        }
        if (vanish == 1) o->tie = 1;
        for (t = o->start + 1; t < O_PASSES; t++) {
            if (nervus_forma_bitus(&o->f[t], (i32)kk)
                    != nervus_forma_bitus(&o->f[o->start], (i32)kk)) {
                varies = 1;
            }
        }
        if (varies && vanish != 1) ok_varies_vanish = 0;
    }
    if (o->period >= 2 && !o->tie) ok_q_tie = 0;
}

static void
orbit4_free (
    Orbit4 *o)
{
    int t;

    for (t = 0; t <= O_PASSES; t++) free(o->x[t]);
}

static int
lcm2 (
    int a,
    int b)
{
    int x = a, y = b;

    while (y) {
        int r = x % y;

        x = y;
        y = r;
    }
    return a / x * b;
}

static void
part_c_period (
    int len4)
{
    int tab[2][3][2];          /* cert (0 t = -1, 1 cube only), q, tie */
    int reducible[2] = { 0, 0 };  /* E with an invariant loop */
    int no_growth = 0;         /* E with no growing loop at all */
    int qcount[3][3];          /* type x (q = 1, 2, > 2) */
    int i, c;
    int settled_all = 1;

    memset(tab, 0, sizeof(tab));
    memset(qcount, 0, sizeof(qcount));
    printf("\n  (b) The period-2 question (KC 55.2.3.1), B_4 up to length "
        "%d: each braid on\n  every loop E_1..E_3, %d passes; settled period "
        "q per braid = lcm over loops\n  (D128 Part D). For E, exactly: mu "
        "= the largest real root of the tail's\n  minimal polynomial (dominant "
        "by north-south dynamics: ASSUMED, not checked);\n  an argument "
        "VANISHES in the limit iff mu is not a root of its own minimal\n  "
        "polynomial (its mu-component is 0). Tie = some argument vanishes "
        "at some\n  position of the cycle, on a loop whose orbit GROWS (mu > "
        "1; a loop that does\n  not grow is invariant and never reaches the "
        "attractor).\n", len4, O_PASSES);
    for (i = 0; i < nk4; i++) {
        PiscinaNotatio mark = piscina_notare(pool);
        Nervus        *net;
        Nervus       **args;
        Orbit4         o[3];
        int            k, q = 1, tie = 0, settled = 1, fixed_loop = 0;
        int            is_e = k4[i].cls == CLS_E;

        if (k4[i].len > len4) {
            piscina_reficere(pool, mark);
            continue;
        }
        if (!plexus_nervus(k4[i].word, pool, &net)) fatal("net refused");
        args = (Nervus **)malloc((size_t)(net->relu + 1) * sizeof(Nervus *));
        for (k = 0; k < (int)net->relu; k++) args[k] = argument_net(net, k);
        for (k = 1; k <= 3; k++) {
            orbit4_settle(net, k, &o[k - 1]);
            if (!o[k - 1].settled) settled = 0;
            else q = lcm2(q, o[k - 1].period);
        }
        if (is_e && settled) {
            int start = 0;

            for (k = 0; k < 3; k++) {
                if (o[k].start > start) start = o[k].start;
            }
            for (k = 0; k < 3; k++) {
                orbit4_limit(args, (int)net->relu, start, q, &o[k]);
                if (o[k].grows) tie |= o[k].tie;
                else fixed_loop = 1;
            }
        }
        if (is_e && settled) {
            int ref = -1;

            for (k = 0; k < 3; k++) {
                s32 sg = 1;

                if (!o[k].grows) continue;
                if (ref < 0) {
                    ref = k;
                    continue;
                }
                if (o[k].ok_mu && o[ref].ok_mu
                        && (!radix_compara(o[k].mu, o[ref].mu, pool, &sg)
                        || sg != 0)) {
                    ok_same_limit = 0;
                }
                if (o[k].tie != o[ref].tie) ok_same_limit = 0;
            }
            if (ref < 0) no_growth++;
            if (fixed_loop) reducible[k4[i].cube_only]++;
        }
        for (k = 0; k < 3; k++) orbit4_free(&o[k]);
        if (!settled) settled_all = 0;
        else {
            qcount[k4[i].cls][q == 1 ? 0 : q == 2 ? 1 : 2]++;
            if (is_e && q <= 2) {
                tab[k4[i].cube_only][q - 1][tie]++;
            }
        }
        free(args);
        piscina_reficere(pool, mark);
    }
    check("B_4: every orbit settles within the run (as in D128)",
        settled_all);
    {
        /* D128 Part D: full (length 6) and quick (length 4) runs */
        int e6[3] = { 3630, 384, 0 }, z6[3] = { 488, 200, 248 };
        int e4[3] = { 222, 32, 0 }, z4[3] = { 92, 26, 64 };
        int c6[2][2] = { { 2950, 56 }, { 680, 328 } };
        int c4[2][2] = { { 186, 0 }, { 36, 32 } };
        int *e = len4 == 6 ? e6 : e4, *z = len4 == 6 ? z6 : z4;
        int ok = len4 == 6 || len4 == 4;
        int t;

        for (t = 0; t < 3; t++) {
            if (qcount[CLS_E][t] != e[t] || qcount[CLS_Z][t] != z[t]) ok = 0;
        }
        for (c = 0; c < 2; c++) {
            for (t = 0; t < 2; t++) {
                int have = tab[c][t][0] + tab[c][t][1];

                if (have != (len4 == 6 ? c6[c][t] : c4[c][t])) ok = 0;
            }
        }
        check("B_4 periods per braid = D128 Part D (E and Z by q, E by "
            "certificate and q)", ok);
    }
    check("E tails: the minimal polynomial holds on the whole tail, its "
        "largest real root\n        mu is > 1 and simple", ok_recurrence
        && ok_mu_all);
    check("theorem: every E orbit of period >= 2 has a tie (continuity at "
        "the limit)", ok_q_tie);
    check("theorem, per argument: a bit that varies on the settled tail "
        "has a vanishing argument", ok_varies_vanish);
    check("vanishing does not depend on the position in the cycle (one "
        "limit direction)", ok_vanish_pos);
    check("the growing loops of an E braid reach the same limit (same mu "
        "= lambda^Q over the\n        braid's period Q, same tie)",
        ok_same_limit);
    check("every E braid has a growing loop", no_growth == 0);
    check("a loop that does not grow is fixed on its tail (polynomial "
        "t - 1)", ok_invariant);
    printf("    E braids with an INVARIANT loop (orbit does not grow: "
        "reducible, a pseudo-Anosov\n    piece): %d certified at t = -1, "
        "%d only at a cube root (left out of the limit)\n", reducible[0],
        reducible[1]);
    printf("\n    %-32s %10s %10s %10s %10s\n", "E braids", "q=1 no tie",
        "q=1 tie", "q=2 tie", "q=2 no tie");
    for (c = 0; c < 2; c++) {
        printf("    %-32s %10d %10d %10d %10d\n", c == 0
            ? "Burau(-1) radius > 1" : "only at a cube root of unity",
            tab[c][0][0], tab[c][0][1], tab[c][1][1], tab[c][1][0]);
    }
    printf("    registered prediction (period 2 iff tie): %s\n",
        tab[0][0][1] + tab[1][0][1] == 0 && tab[0][1][0] + tab[1][1][0] == 0
        ? "HOLDS" : "FAILS (q = 1 braids with a tie, or q = 2 without)");
}

static void
part_c (void)
{
    int len4 = celer ? 4 : 6;
    int lat = celer ? 3 : 4;

    printf("\n=== Part C: B_4 ===\n");
    build_census4(len4);
    if (celer) {
        shells4[0] = 2;
        shells4[1] = 4;
    }
    part_c_lattice(lat, 2);
    part_c_period(len4);
}

/* ================================================================
 * Part D: nulls (B_3, exact)
 * ================================================================
 *
 * (a) a LINEAR network (Burau(-1) of the word, two affine instructions):
 *     one region by construction - the floor;
 * (b) the census word's own network with each instruction's
 *     coefficients permuted at random (same sources, same terms, same
 *     depth and ReLU count);
 * (c) the census word's letters in random order: a different braid,
 *     whose type is looked up in the census. */

#define D_REGION_CAP 20000

static Sors srng;
static int  shuffled_type_ok = 1;

static int null_changed = 0;      /* permuted networks that differ */

static Nervus *
shuffled_net (
    const Nervus *net)
{
    int changed = 0;

    Nervus *a;
    int     t;

    if (!nervus_creare(net->introitus, pool, &a)) fatal("net refused");
    for (t = 0; t < (int)net->numerus; t++) {
        const NervusInstructio *in = &net->instructiones[t];
        Magnus                  c[16];
        s32                     reg;
        int                     j;

        if (in->termini > 16) fatal("too many terms");
        for (j = 0; j < (int)in->termini; j++) c[j] = in->coefficientes[j];
        for (j = (int)in->termini - 1; j > 0; j--) {
            int    r = (int)sors_intra(&srng, (i32)(j + 1));
            Magnus tmp = c[j];

            c[j] = c[r];
            c[r] = tmp;
        }
        for (j = 0; j < (int)in->termini; j++) {
            if (!magnus_aequalis(c[j], in->coefficientes[j])) changed = 1;
        }
        if (!(in->genus == NERVUS_RELU ? nervus_relu : nervus_affinis)(a,
                in->termini, in->fontes, c, in->addendum, &reg)) {
            fatal("copy refused");
        }
    }
    if (!nervus_exitus(a, net->exitus_numerus, net->exitus)) {
        fatal("exit refused");
    }
    null_changed += changed;
    return a;
}

/* regions and pieces of the iterates of one 2 -> 2 network; returns
 * the last m reached (stops past D_REGION_CAP) */
static int
sweep_net (
    Nervus *one,
    int    *r,
    int    *p)
{
    PiscinaNotatio mark = piscina_notare(pool);
    Nervus        *net = one;
    int            m;
    int            last = 0;

    for (m = 1; m <= MMAX; m++) {
        NervusPlanum pl;

        if (m > 1 && !nervus_componere(net, one, pool, &net)) {
            fatal("composition refused");
        }
        if (!nervus_regiones_plani(net, pool, &pl)) fatal("sweep refused");
        r[m] = (int)pl.numerus;
        p[m] = (int)pl.frusta;
        last = m;
        if (r[m] > D_REGION_CAP) break;
    }
    piscina_reficere(pool, mark);
    return last;
}

static void
part_d (void)
{
    int  i, c, m;
    int  floor_ok = 1;
    int  shape_ok = 1;
    long sr[3][MMAX + 1], sp[3][MMAX + 1];
    int  n[3] = { 0, 0, 0 }, lin[3] = { 0, 0, 0 }, ex[3] = { 0, 0, 0 };
    int  capped[3] = { 0, 0, 0 };
    int  trans[3][3];
    long wr[3][MMAX + 1];
    int  wn[3] = { 0, 0, 0 }, wlin[3] = { 0, 0, 0 }, wex[3] = { 0, 0, 0 };
    char b1[32], b2[32];
    TabulaDispersa *index = tabula_dispersa_creare_chorda(pool, 4096);
    int            *slot = (int *)malloc((size_t)nk * sizeof(int));

    printf("\n=== Part D: nulls on B_3 (exact) ===\n");
    sors_seminare(&srng, 129, 1);
    memset(sr, 0, sizeof(sr));
    memset(sp, 0, sizeof(sp));
    memset(trans, 0, sizeof(trans));
    memset(wr, 0, sizeof(wr));
    for (i = 0; i < nk; i++) {
        chorda key;

        slot[i] = i;
        plexus_clavis(ks[i].word, pool, &key);
        tabula_dispersa_inserere(index, key, &slot[i]);
    }
    for (i = 0; i < nk; i++) {
        PiscinaNotatio mark = piscina_notare(pool);
        Nervus        *one;
        Nervus        *sh;
        NervusFigura   f1, f2;
        int            r[MMAX + 1], p[MMAX + 1];
        int            last;

        c = ks[i].cls;
        /* (a) the linear floor: Burau(-1) as two affine instructions */
        {
            Matrix  bm;
            Nervus *lin_net;
            s32     f[2] = { 0, 1 };
            s32     out[2];
            Magnus  row[2];
            NervusPlanum pl;
            int     t;

            if (!plexus_burau_minus_uno(ks[i].word, pool, &bm)
                    || !nervus_creare(2, pool, &lin_net)) {
                fatal("linear null refused");
            }
            for (t = 0; t < 2; t++) {
                row[0] = *(const Magnus *)matrix_elementum(bm, (i32)t, 0);
                row[1] = *(const Magnus *)matrix_elementum(bm, (i32)t, 1);
                if (!nervus_affinis(lin_net, 2, f, row, magnus_ex_s64(0),
                        &out[t])) fatal("affine refused");
            }
            nervus_exitus(lin_net, 2, out);
            if (!nervus_regiones_plani(lin_net, pool, &pl)
                    || pl.numerus != 1 || pl.frusta != 1) {
                floor_ok = 0;
            }
        }
        /* (b) coefficients permuted */
        if (!plexus_nervus(ks[i].word, pool, &one)) fatal("net refused");
        sh = shuffled_net(one);
        if (!nervus_figura(one, &f1) || !nervus_figura(sh, &f2)
                || f1.relu != f2.relu || f1.profunditas != f2.profunditas
                || f1.profunditas_relu != f2.profunditas_relu
                || f1.termini != f2.termini) {
            shape_ok = 0;
        }
        last = sweep_net(sh, r, p);
        n[c]++;
        if (last < MMAX) {
            capped[c]++;
        } else {
            for (m = 1; m <= MMAX; m++) {
                sr[c][m] += r[m];
                sp[c][m] += p[m];
            }
            lin[c] += h_lin(r);
            ex[c] += h_exp(r);
        }
        piscina_reficere(pool, mark);
        /* (c) letters shuffled */
        {
            Plexus  w = ks[i].word;
            s32     lt[16];
            Plexus  ws;
            chorda  key;
            vacuum *val = NULL;
            int     j, nc;
            Nervus *wn_net;

            mark = piscina_notare(pool);
            for (j = 0; j < (int)w.longitudo; j++) lt[j] = w.litterae[j];
            for (j = (int)w.longitudo - 1; j > 0; j--) {
                int r2 = (int)sors_intra(&srng, (i32)(j + 1));
                s32 tmp = lt[j];

                lt[j] = lt[r2];
                lt[r2] = tmp;
            }
            plexus_creare(3, lt, w.longitudo, pool, &ws);
            plexus_clavis(ws, pool, &key);
            if (!tabula_dispersa_invenire(index, key, &val)) {
                fatal("shuffled word has no class");
            }
            nc = ks[*(int *)val].cls;
            {
                /* the type straight from the word (period, trace) */
                i32  period = 0;
                long tr = burau_trace(ws);
                long at = tr < 0 ? -tr : tr;
                int  direct;

                if (!plexus_periodus(ws, pool, &period)) {
                    fatal("period refused");
                }
                direct = period > 0 ? CLS_P : at > 2 ? CLS_E : CLS_Z;
                if (direct != nc) shuffled_type_ok = 0;
            }
            trans[c][nc]++;
            if (!plexus_nervus(ws, pool, &wn_net)) fatal("net refused");
            (void)sweep_net(wn_net, r, p);
            wn[nc]++;
            for (m = 1; m <= MMAX; m++) wr[nc][m] += r[m];
            wlin[nc] += h_lin(r);
            wex[nc] += h_exp(r);
            piscina_reficere(pool, mark);
        }
    }
    check("(a) a linear network has one region and one piece (the floor)",
        floor_ok);
    check("(b) permuted coefficients keep the shape (ReLUs, depth, ReLU "
        "depth, terms)", shape_ok);
    printf("  (b) permuted networks that differ from the original: %d of "
        "%d\n", null_changed, nk);
    check("(b) the permutation changes most networks", 2 * null_changed
        > nk);
    check("(c) each shuffled word's type (census lookup) = its own period "
        "and trace", shuffled_type_ok);
    printf("\n  (b) Permuted coefficients, by the original braid's type "
        "(mean over classes;\n  networks past %d regions are stopped and "
        "counted apart):\n", D_REGION_CAP);
    printf("    %4s %8s %7s %9s %9s %9s %9s %6s %6s\n", "type", "classes",
        "capped", "r(1)", "r(6)", "r(12)", "p(12)", "lin", "exp");
    for (c = 0; c < 3; c++) {
        int k = n[c] - capped[c];
        char b3[32], b4[32];

        mean_text(sr[c][1], k, b1);
        mean_text(sr[c][6], k, b2);
        mean_text(sr[c][MMAX], k, b3);
        mean_text(sp[c][MMAX], k, b4);
        printf("    %4s %8d %7d %9s %9s %9s %9s %6d %6d\n", cls_name[c],
            n[c], capped[c], b1, b2, b3, b4, lin[c], ex[c]);
    }
    printf("\n  (c) Letters shuffled: type of the new braid (rows: "
        "original, columns: new):\n");
    printf("    %4s %6s %6s %6s\n", "", "P", "E", "Z");
    for (c = 0; c < 3; c++) {
        printf("    %4s %6d %6d %6d\n", cls_name[c], trans[c][0],
            trans[c][1], trans[c][2]);
    }
    printf("    by the NEW type: mean r(6), r(12), and the rule\n");
    printf("    %4s %8s %9s %9s %6s %6s\n", "type", "words", "r(6)",
        "r(12)", "lin", "exp");
    for (c = 0; c < 3; c++) {
        mean_text(wr[c][6], wn[c], b1);
        mean_text(wr[c][MMAX], wn[c], b2);
        printf("    %4s %8d %9s %9s %6d %6d\n", cls_name[c], wn[c], b1, b2,
            wlin[c], wex[c]);
    }
    free(slot);
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
    part_c();
    part_d();
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    return n_fail ? 1 : 0;
}
