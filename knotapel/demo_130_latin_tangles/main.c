/*
 * KNOTAPEL DEMO 130: Latin Dependency Trees as Tangles (N9)
 * =========================================================
 *
 * The brainstorm's picture of a Latin sentence (brainstorm-skeleton-
 * settling.md IV, XI): a planar, Temperley-Lieb-like skeleton of nested
 * dependencies, with CROSSINGS where word order separates words that
 * belong together (hyperbaton). Before building on the picture (N14),
 * measure whether real Latin looks like it. Plan: plan.md (approved
 * 2026-10-17; definitions checked against Kuhlmann and Nivre 2006; the
 * non-projective arc defined after the T1a review, before any census).
 *
 *   Part A  (T1b) the trees: every treebank read (conllu), counts against
 *           FONTES.md, every sentence a sound tree, the three routes to
 *           projectivity agree, projective => planar => well-nested,
 *           Kuhlmann and Nivre's figures, the 1/3 law over ALL orders of
 *           every short real tree, the planar core against an
 *           independent search, reversal invariance.
 *   Part B  (T1b) the crossing census per treebank: projective and
 *           planar shares, crossings per sentence (distribution, exact
 *           mean), non-projective and non-planar arcs, observed /
 *           expected crossings under random word order (exact null:
 *           E = disjoint arc pairs / 3), by sentence length, planar core
 *           deficit. Registered predictions P1 and P4.
 *   Part C  (T2) relations and agreement (P2).
 *   Part D  (T2) mildness and shape (P3).
 *
 * Headline = trees WITHOUT punctuation (UPOS PUNCT removed, dependents
 * re-attached to its head, dependentia_omittere); a sentence whose root
 * is PUNCT is set aside in the headline and counted. Second column =
 * trees as annotated.
 *
 * Data. In the repo (CC BY-SA 4.0): oratio/probationes/fixa/ud/ -
 * CIRCSE test (Seneca, verse), LLCT dev + test (charters), EWT dev +
 * test (English). NC treebanks (PROIEL, Perseus, ITTB, UDante, test
 * splits) are read from $D130_UD_NC (default oratio/build/ud, where
 * ./oratio/oraculum.sh -petere fetches them); by Fran's decision only
 * AGGREGATE numbers are printed, never a sentence. A missing NC file is
 * reported and its rows skipped.
 *
 * House libraries: conllu, dependentia (and their closure), hence
 * latina.h: Roman numerals and Latin keywords are macros here (never
 * name a variable I, V, X, L, C, D or M, or per, si, dum, ...).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_130_latin_tangles/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * Quick mode for plants: D130_CELER=1 (repo treebanks only, 1/3 law up
 * to 5 words).
 */

#include "conllu.h"
#include "dependentia.h"
#include "filum.h"
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

static void
fatal (
    const char *msg)
{
    printf("  FATAL: %s\n", msg);
    exit(1);
}

static int quick = 0;

/* ================================================================
 * Exact rationals (counts are small: products stay far below 2^63)
 * ================================================================ */

typedef struct {
    s64 p;
    s64 q;
} Ratio;

static s64
gcd64 (
    s64 a,
    s64 b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        s64 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static Ratio
ratio (
    s64 p,
    s64 q)
{
    Ratio r;
    s64   g = gcd64(p, q);

    if (g == 0) g = 1;
    r.p = p / g;
    r.q = q / g;
    return r;
}

/* a < b for q > 0 */
static int
ratio_less (
    Ratio a,
    Ratio b)
{
    return a.p * b.q < b.p * a.q;
}

static void
ratio_print (
    Ratio r)
{
    if (r.q == 0) {
        printf("-");
        return;
    }
    printf("%ld/%ld (%.4f)", (long)r.p, (long)r.q,
        (double)r.p / (double)r.q);
}

/* ================================================================
 * Treebanks
 * ================================================================ */

#define N_FILES   9
#define N_BANKS   7
#define N_LEN     5    /* sentence-length bins */
#define N_DIST    6    /* crossing-count bins */

typedef struct {
    const char *file;
    int         nc;          /* NC: read from $D130_UD_NC */
    int         bank;        /* index into banks[] */
    long        sentences;   /* FONTES.md (repo only; -1 = not pinned) */
    long        words;
    long        ranges;
} TreebankFile;

static const TreebankFile files[N_FILES] = {
    { "la_circse-ud-test.conllu", 0, 0, 893, 11503, 316 },
    { "la_llct-ud-dev.conllu", 0, 1, 850, 24189, 3 },
    { "la_llct-ud-test.conllu", 0, 1, 884, 24079, 2 },
    { "en_ewt-ud-dev.conllu", 0, 2, 2001, 25148, 359 },
    { "en_ewt-ud-test.conllu", 0, 2, 2077, 25094, 354 },
    { "la_proiel-ud-test.conllu", 1, 3, -1, -1, -1 },
    { "la_perseus-ud-test.conllu", 1, 4, -1, -1, -1 },
    { "la_ittb-ud-test.conllu", 1, 5, -1, -1, -1 },
    { "la_udante-ud-test.conllu", 1, 6, -1, -1, -1 }
};

static const char *const bank_names[N_BANKS] = {
    "CIRCSE (Seneca, verse)",
    "LLCT (charters)",
    "EWT (English)",
    "PROIEL (classical, Vulgate)",
    "Perseus (classical)",
    "ITTB (Aquinas)",
    "UDante (Dante)"
};

static const char *const bank_short[N_BANKS] = {
    "CIRCSE", "LLCT", "EWT", "PROIEL", "Perseus", "ITTB", "UDante"
};

static const int bank_latin[N_BANKS] = { 1, 1, 0, 1, 1, 1, 1 };

/* per bank and column (0 = headline without PUNCT, 1 = as annotated) */
typedef struct {
    long sentences;        /* read */
    long aside_unsound;    /* not a tree as annotated */
    long aside_root;       /* headline: PUNCT root */
    long trees;            /* analysed */
    long words;            /* words in analysed trees */
    long arcs;             /* real arcs */
    long projective;
    long planar;
    long well_nested;
    s64  crossings;
    s64  root_crossings;
    s64  pairs;            /* disjoint arc pairs: E = pairs / 3 */
    long nonproj_arcs;
    long nonplanar_arcs;
    long core_deficit;     /* arcs - planar core */
    long contractions;     /* PUNCT dependents re-attached */
    long dist[N_DIST];     /* trees with 0,1,2,3,4-7,8+ crossings */
    s64  len_cross[N_LEN];
    s64  len_pairs[N_LEN];
    long len_trees[N_LEN];
} Stats;

static Stats stats[N_BANKS][2];
static int   bank_present[N_BANKS];
static long  file_bytes[N_FILES];

static int
len_bin (
    i32 n)
{
    if (n <= 10) return 0;
    if (n <= 20) return 1;
    if (n <= 30) return 2;
    if (n <= 50) return 3;
    return 4;
}

static const char *const len_names[N_LEN] = {
    "1-10", "11-20", "21-30", "31-50", "51+"
};

static int
dist_bin (
    s64 c)
{
    if (c <= 3) return (int)c;
    if (c <= 7) return 4;
    return 5;
}

/* ================================================================
 * Per-tree checks (Part A), run on every analysed tree
 * ================================================================ */

static long viae_bad = 0;        /* three projectivity routes disagree */
static long hier_bad = 0;        /* projective => planar => well-nested */
static long arcs_bad = 0;        /* per-arc measures vs tree totals */
static long core_bad = 0;        /* planar core DP vs independent search */
static long core_checked = 0;
static long rev_bad = 0;         /* reversal invariance */
static long third_bad = 0;       /* 1/3 law over all orders */
static long third_checked = 0;

#define CORE_MAX_ARCS 24

/* maximum independent set of the crossing graph, by branching on the
 * lowest candidate (an algorithm unrelated to the library's interval
 * DP); adj[v] = arcs crossing arc v. Branching lowest-first only ever
 * reads the HIGHER neighbours of v (lower ones are already decided), so
 * the lower half of adj is never used - a plant dropping it stayed green
 * for that reason. */
static int
mis (
    unsigned long        cand,
    const unsigned long *adj)
{
    int           v = 0;
    unsigned long bit;
    int           a;
    int           b;

    if (cand == 0) return 0;
    while (!(cand & (1UL << v))) v++;
    bit = 1UL << v;
    if ((adj[v] & cand) == 0)
        return 1 + mis(cand & ~bit, adj);
    a = mis(cand & ~bit, adj);
    b = 1 + mis(cand & ~bit & ~adj[v], adj);
    return a > b ? a : b;
}

static int
crosses (
    i32 h1,
    i32 d1,
    i32 h2,
    i32 d2)
{
    i32 a = h1 < d1 ? h1 : d1, b = h1 < d1 ? d1 : h1;
    i32 c = h2 < d2 ? h2 : d2, e = h2 < d2 ? d2 : h2;

    return (a < c && c < b && b < e) || (c < a && a < e && e < b);
}

/* independent planar core; -1 if too many arcs */
static int
core_search (
    const DependentiaArbor *t)
{
    unsigned long adj[CORE_MAX_ARCS];
    i32           dep[CORE_MAX_ARCS];
    int           m = 0;
    int           i, j;
    i32           k;

    for (k = 1; k <= t->numerus; k++) {
        if (t->capita[k - 1] == 0) continue;
        if (m == CORE_MAX_ARCS) return -1;
        dep[m++] = k;
    }
    for (i = 0; i < m; i++) adj[i] = 0;
    for (i = 0; i < m; i++)
        for (j = i + 1; j < m; j++)
            if (crosses(t->capita[dep[i] - 1], dep[i],
                        t->capita[dep[j] - 1], dep[j])) {
                adj[i] |= 1UL << j;
                adj[j] |= 1UL << i;
            }
    return mis(m == 0 ? 0UL : (m == 64 ? ~0UL : (1UL << m) - 1UL), adj);
}

/* next permutation in lexicographic order; 0 after the last */
static int
next_perm (
    i32 *s,
    int  n)
{
    int i = n - 2, j;
    i32 t;

    while (i >= 0 && s[i] >= s[i + 1]) i--;
    if (i < 0) return 0;
    j = n - 1;
    while (s[j] <= s[i]) j--;
    t = s[i]; s[i] = s[j]; s[j] = t;
    for (i = i + 1, j = n - 1; i < j; i++, j--) {
        t = s[i]; s[i] = s[j]; s[j] = t;
    }
    return 1;
}

/* sum of crossings over ALL n! orders == n! * pairs / 3 (exactly:
 * 3 * sum == n! * pairs); the crossings of each order are counted by the
 * library, the 1/3 is not used */
static int
third_law (
    Piscina                *scratch,
    const DependentiaArbor *t,
    s64                     pairs)
{
    i32 sigma[8], moved[8];
    s64 sum = 0, orders = 0;
    i32 k, n = t->numerus;

    for (k = 0; k < n; k++) sigma[k] = k + 1;
    do {
        DependentiaArbor  u;
        DependentiaCruces x;

        for (k = 1; k <= n; k++) {
            i32 h = t->capita[k - 1];
            moved[sigma[k - 1] - 1] = h == 0 ? 0 : sigma[h - 1];
        }
        u.numerus = n;
        u.capita = moved;
        if (!dependentia_cruces(scratch, &u, &x)) fatal("cruces (orders)");
        sum += x.cruces;
        orders++;
    } while (next_perm(sigma, (int)n));
    return 3 * sum == orders * pairs;
}

/* analyse one sound tree into stats[bank][col]; per-tree checks */
static void
analyse (
    Piscina                *scratch,
    const DependentiaArbor *t,
    int                     bank,
    int                     col,
    int                     law)
{
    Stats             *s = &stats[bank][col];
    DependentiaCruces  x, xr;
    DependentiaMensura m, mr;
    DependentiaArcus  *arcs;
    DependentiaArbor   rev;
    i32               *rc;
    s32                core, core_r;
    i32                k, n = t->numerus;
    long               nonproj = 0, crossed = 0, maxdeg = 0;
    s64                arc_cross = 0;
    int                b;

    if (!dependentia_cruces(scratch, t, &x)) fatal("cruces");
    if (!dependentia_mensurare(scratch, t, NULL, &m)) fatal("mensurare");
    arcs = (DependentiaArcus *)piscina_allocare(scratch,
        (memoriae_index)n * sizeof(DependentiaArcus));
    if (!dependentia_arcus(scratch, t, arcs)) fatal("arcus");
    core = dependentia_nucleus_planaris(scratch, t);
    if (core < 0) fatal("nucleus");

    for (k = 0; k < n; k++) {
        arc_cross += arcs[k].cruces;
        if (arcs[k].cruces > 0) crossed++;
        if (arcs[k].non_proiectivus) nonproj++;
        if ((long)arcs[k].gradus > maxdeg) maxdeg = (long)arcs[k].gradus;
    }

    /* Part A checks */
    if (x.proiectiva != m.proiectiva
        || m.proiectiva != (m.gradus_arcuum == 0)
        || m.proiectiva != (nonproj == 0))
        viae_bad++;
    if ((x.proiectiva && !x.planaris) || (x.planaris && !m.bene_nidificata))
        hier_bad++;
    if (arc_cross != 2 * x.cruces || crossed != (long)x.arcus_secti
        || maxdeg != (long)m.gradus_arcuum)
        arcs_bad++;
    if (n - 1 <= CORE_MAX_ARCS) {
        core_checked++;
        if (core_search(t) != (int)core) core_bad++;
    }
    rc = (i32 *)piscina_allocare(scratch, (memoriae_index)n * sizeof(i32));
    for (k = 1; k <= n; k++) {
        i32 h = t->capita[k - 1];
        rc[n - k] = h == 0 ? 0 : n + 1 - h;
    }
    rev.numerus = n;
    rev.capita = rc;
    if (!dependentia_cruces(scratch, &rev, &xr)
        || !dependentia_mensurare(scratch, &rev, NULL, &mr))
        fatal("reversed");
    core_r = dependentia_nucleus_planaris(scratch, &rev);
    if (xr.cruces != x.cruces || xr.cruces_radicis != x.cruces_radicis
        || mr.gradus_lacunarum != m.gradus_lacunarum
        || mr.gradus_arcuum != m.gradus_arcuum
        || mr.bene_nidificata != m.bene_nidificata || core_r != core)
        rev_bad++;
    if (law && n <= (quick ? 5 : 7)) {
        third_checked++;
        if (!third_law(scratch, t, x.paria_disiuncta)) third_bad++;
    }

    /* Part B accumulation */
    s->trees++;
    s->words += n;
    s->arcs += n - 1;
    if (x.proiectiva) s->projective++;
    if (x.planaris) s->planar++;
    if (m.bene_nidificata) s->well_nested++;
    s->crossings += x.cruces;
    s->root_crossings += x.cruces_radicis;
    s->pairs += x.paria_disiuncta;
    s->nonproj_arcs += nonproj;
    s->nonplanar_arcs += (long)x.arcus_secti;
    s->core_deficit += (long)(n - 1) - (long)core;
    s->dist[dist_bin(x.cruces)]++;
    b = len_bin(n);
    s->len_cross[b] += x.cruces;
    s->len_pairs[b] += x.paria_disiuncta;
    s->len_trees[b]++;
}

/* ================================================================
 * Part A: read every treebank
 * ================================================================ */

static void
read_file (
    int fi)
{
    const TreebankFile *f = &files[fi];
    const char         *dir = "oratio/probationes/fixa/ud";
    char                path[1024];
    Piscina            *p, *scratch;
    chorda              src;
    Xar                *sents;
    ConlluVitium        err = { 0, NULL };
    long                words = 0, ranges = 0, unsound = 0;
    i32                 si_;
    int                 bank = f->bank;

    if (f->nc) {
        dir = getenv("D130_UD_NC");
        if (dir == NULL) dir = "oratio/build/ud";
    }
    if (strlen(dir) + strlen(f->file) + 2 > sizeof(path))
        fatal("path too long");
    sprintf(path, "%s/%s", dir, f->file);
    p = piscina_generare_dynamicum("d130_file", 1 << 22);
    scratch = piscina_generare_dynamicum("d130_tree", 1 << 20);
    if (p == NULL || scratch == NULL) fatal("pool");
    src = filum_legere_totum(path, p);
    if (src.datum == NULL) {
        printf("  %-28s ABSENT (%s)\n", f->file, f->nc
            ? "NC: set D130_UD_NC or ./oratio/oraculum.sh -petere"
            : "repo fixture missing");
        if (!f->nc) check("repo treebank present", 0);
        piscina_destruere(scratch);
        piscina_destruere(p);
        return;
    }
    file_bytes[fi] = (long)src.mensura;
    sents = conllu_legere(p, src, &err);
    if (sents == NULL) {
        printf("  %s: line %u: %s\n", f->file, (unsigned)err.linea,
            err.causa ? err.causa : "?");
        fatal("conllu_legere");
    }
    bank_present[bank] = 1;
    for (si_ = 0; si_ < xar_numerus(sents); si_++) {
        const ConlluSententia *s =
            (const ConlluSententia *)xar_obtinere(sents, si_);
        ConlluArbor  a;
        b32         *punct;
        i32          k, j;

        for (j = 0; j < xar_numerus(s->lexemata); j++)
            if (((const ConlluLexema *)xar_obtinere(s->lexemata, j))
                ->ranga)
                ranges++;
        piscina_vacare(scratch);
        stats[bank][0].sentences++;
        stats[bank][1].sentences++;
        if (!conllu_arbor(scratch, s, &a)
            || dependentia_validare(&a.arbor) != DEPENDENTIA_SANA) {
            unsound++;
            stats[bank][0].aside_unsound++;
            stats[bank][1].aside_unsound++;
            continue;
        }
        words += a.arbor.numerus;
        /* column 1: as annotated (the 1/3 law runs on repo trees) */
        analyse(scratch, &a.arbor, bank, 1, !f->nc);
        /* column 0: without PUNCT */
        punct = (b32 *)piscina_allocare(scratch,
            (memoriae_index)a.arbor.numerus * sizeof(b32));
        for (k = 0; k < a.arbor.numerus; k++)
            punct[k] = chorda_aequalis_literis(a.verba[k]->upos, "PUNCT");
        {
            DependentiaArbor  e;
            i32               moved = 0;
            DependentiaOmissa r = dependentia_omittere(scratch, &a.arbor,
                punct, &e, &moved);

            if (r == DEPENDENTIA_OMISSA_RADIX) {
                stats[bank][0].aside_root++;
            } else if (r != DEPENDENTIA_OMISSA_FACTA) {
                fatal("omittere");
            } else {
                stats[bank][0].contractions += (long)moved;
                analyse(scratch, &e, bank, 0, 0);
            }
        }
    }
    printf("  %-28s %7ld bytes  %5u sentences  %6ld words  %4ld ranges"
        "  %ld not trees\n", f->file, file_bytes[fi],
        (unsigned)xar_numerus(sents), words, ranges, unsound);
    if (f->sentences >= 0) {
        char msg[200];
        sprintf(msg, "%s: sentences, words, ranges = FONTES.md "
            "(%ld, %ld, %ld)", f->file, f->sentences, f->words, f->ranges);
        check(msg, (long)xar_numerus(sents) == f->sentences
            && words == f->words && ranges == f->ranges);
    }
    {
        char msg[200];
        sprintf(msg, "%s: every sentence is a sound tree", f->file);
        check(msg, unsound == 0);
    }
    piscina_destruere(scratch);
    piscina_destruere(p);
}

/* Kuhlmann and Nivre's figures (reconstructed from every property their
 * text states) */
static void
figures (
    Piscina *scratch)
{
    static const i32 f2a[3] = { 2, 0, 1 };
    static const i32 f3a[6] = { 0, 1, 2, 3, 1, 5 };
    static const i32 f3b[6] = { 0, 1, 2, 5, 1, 3 };
    static const i32 f3c[6] = { 0, 1, 5, 2, 1, 4 };
    DependentiaArbor   t;
    DependentiaCruces  x;
    DependentiaMensura m;

    t.numerus = 3; t.capita = f2a;
    dependentia_cruces(scratch, &t, &x);
    dependentia_mensurare(scratch, &t, NULL, &m);
    check("K&N Fig. 2a: planar, not projective, gap degree 1",
        x.planaris && !x.proiectiva && !m.proiectiva
        && m.gradus_lacunarum == 1);
    t.numerus = 6; t.capita = f3a;
    dependentia_mensurare(scratch, &t, NULL, &m);
    check("K&N Fig. 3a: gd 0, ed 0, well-nested",
        m.gradus_lacunarum == 0 && m.gradus_arcuum == 0
        && m.bene_nidificata);
    t.capita = f3b;
    dependentia_mensurare(scratch, &t, NULL, &m);
    check("K&N Fig. 3b: gd 1, ed 1, well-nested",
        m.gradus_lacunarum == 1 && m.gradus_arcuum == 1
        && m.bene_nidificata);
    t.capita = f3c;
    dependentia_mensurare(scratch, &t, NULL, &m);
    check("K&N Fig. 3c: gd 2, ed 1, NOT well-nested",
        m.gradus_lacunarum == 2 && m.gradus_arcuum == 1
        && !m.bene_nidificata);
}

static void
part_a (void)
{
    Piscina *scratch = piscina_generare_dynamicum("d130_fig", 1 << 16);
    int      fi;

    printf("\n=== Part A: the trees ===\n\n");
    if (scratch == NULL) fatal("pool");
    figures(scratch);
    piscina_destruere(scratch);
    printf("\n");
    for (fi = 0; fi < N_FILES; fi++) {
        if (quick && files[fi].nc) continue;
        read_file(fi);
    }
    printf("\n  per-tree checks over every analysed tree (both columns):\n");
    check("projective by crossings == by yields == edge degree 0 == no "
        "non-projective arc", viae_bad == 0);
    check("projective => planar => well-nested", hier_bad == 0);
    check("per-arc crossings sum to 2 x crossings; crossed arcs and max "
        "edge degree match the tree", arcs_bad == 0);
    {
        char msg[200];
        sprintf(msg, "planar core (interval DP) == independent branching "
            "search, on %ld trees with <= %d arcs", core_checked,
            CORE_MAX_ARCS);
        check(msg, core_bad == 0 && core_checked > 0);
        sprintf(msg, "reversing word order keeps crossings, root crossings,"
            " gap and edge degree, well-nestedness, planar core");
        check(msg, rev_bad == 0);
        sprintf(msg, "1/3 law: sum of crossings over all n! orders == n! "
            "x pairs / 3, on %ld repo trees with <= %d words",
            third_checked, quick ? 5 : 7);
        check(msg, third_bad == 0 && third_checked > 0);
    }
}

/* ================================================================
 * Part B: the crossing census
 * ================================================================ */

static Ratio
obs_exp (
    const Stats *s)
{
    return ratio(3 * s->crossings, s->pairs);
}

/* Part B totals of the repo treebanks pinned from oracle_partb.py (an
 * independent Python count: own parsing, own PUNCT removal, all-pairs
 * crossings, interval yields, head walks) - constants, said so */
typedef struct {
    int  bank, col;
    long trees, aside, words;
    s64  cross, pairs, rootc;
    long nonproj, nonplan, proj, planar;
} Pin;

static const Pin pins[6] = {
    { 0, 0, 893, 0, 11503, 2168, 105998, 254, 1284, 2750, 375, 401 },
    { 0, 1, 893, 0, 11503, 2168, 105998, 254, 1284, 2750, 375, 401 },
    { 1, 0, 1734, 0, 41463, 2080, 960319, 466, 1191, 2172, 1248, 1260 },
    { 1, 1, 1734, 0, 48268, 2658, 1223918, 466, 1191, 2449, 1248, 1248 },
    { 2, 0, 4033, 45, 44071, 92, 289635, 11, 67, 147, 3973, 3978 },
    { 2, 1, 4078, 0, 50242, 111, 371227, 11, 67, 171, 4018, 4018 }
};

/* headline, per bank: sentence-length bins (trees, crossings, pairs)
 * and trees by crossing count, from oracle_partb.py */
static const long pin_lens[3][N_LEN][3] = {
    { { 484, 321, 2977 }, { 267, 754, 18597 }, { 77, 355, 17423 },
      { 49, 383, 28096 }, { 16, 355, 38905 } },
    { { 944, 48, 11862 }, { 177, 130, 10378 }, { 166, 300, 34790 },
      { 143, 286, 98610 }, { 304, 1316, 804679 } },
    { { 2387, 3, 10620 }, { 1063, 24, 69298 }, { 400, 30, 90497 },
      { 166, 28, 94361 }, { 17, 7, 24859 } }
};

static const long pin_dist[3][N_DIST] = {
    { 401, 134, 95, 71, 128, 64 },
    { 1260, 176, 76, 41, 96, 85 },
    { 3978, 36, 9, 4, 6, 0 }
};

static void
pinned_checks (void)
{
    int i, b, k;

    for (b = 0; b < 3; b++) {
        const Stats *s = &stats[b][0];
        Ratio        oe = obs_exp(s);
        int          ok = 1;
        char         msg[200];

        for (k = 0; k < N_LEN; k++)
            if (s->len_trees[k] != pin_lens[b][k][0]
                || s->len_cross[k] != (s64)pin_lens[b][k][1]
                || s->len_pairs[k] != (s64)pin_lens[b][k][2])
                ok = 0;
        for (k = 0; k < N_DIST; k++)
            if (s->dist[k] != pin_dist[b][k]) ok = 0;
        /* observed / expected from the oracle's own totals */
        if (oe.p * (s64)pins[2 * b].pairs != 3 * pins[2 * b].cross * oe.q)
            ok = 0;
        sprintf(msg, "%s headline: length bins, crossing distribution and "
            "obs/exp = oracle_partb.py", bank_short[b]);
        check(msg, ok);
    }

    for (i = 0; i < 6; i++) {
        const Pin   *q = &pins[i];
        const Stats *s = &stats[q->bank][q->col];
        char         msg[200];

        sprintf(msg, "%s %s: totals = oracle_partb.py (trees %ld, words %ld,"
            " crossings %ld, pairs %ld, ...)", bank_short[q->bank],
            q->col == 0 ? "headline" : "annotated", q->trees, q->words,
            (long)q->cross, (long)q->pairs);
        check(msg, s->trees == q->trees
            && s->aside_unsound + s->aside_root == q->aside
            && s->words == q->words && s->crossings == q->cross
            && s->pairs == q->pairs && s->root_crossings == q->rootc
            && s->nonproj_arcs == q->nonproj
            && s->nonplanar_arcs == q->nonplan
            && s->projective == q->proj && s->planar == q->planar);
    }
}

static void
census_table (
    int col)
{
    int b;

    printf("\n  %s\n", col == 0
        ? "HEADLINE: without punctuation"
        : "as annotated (punctuation included)");
    printf("  %-8s %6s %5s %5s %7s %7s %7s %8s %8s %8s %8s\n",
        "bank", "trees", "aside", "moved", "proj%", "planar%", "wn%",
        "cross/s", "nonproj%", "nonplan%", "obs/exp");
    for (b = 0; b < N_BANKS; b++) {
        const Stats *s = &stats[b][col];
        Ratio        oe;

        if (!bank_present[b] || s->trees == 0) continue;
        oe = obs_exp(s);
        printf("  %-8s %6ld %5ld %5ld %7.2f %7.2f %7.2f %8.4f %8.3f "
            "%8.3f %8.4f\n", bank_short[b], s->trees,
            s->aside_unsound + s->aside_root, s->contractions,
            100.0 * (double)s->projective / (double)s->trees,
            100.0 * (double)s->planar / (double)s->trees,
            100.0 * (double)s->well_nested / (double)s->trees,
            (double)s->crossings / (double)s->trees,
            100.0 * (double)s->nonproj_arcs / (double)s->arcs,
            100.0 * (double)s->nonplanar_arcs / (double)s->arcs,
            (double)oe.p / (double)oe.q);
    }
}

static void
part_b (void)
{
    int b, i;

    printf("\n=== Part B: the crossing census ===\n\n");
    for (b = 0; b < N_BANKS; b++)
        if (bank_present[b])
            printf("  %-8s = %s\n", bank_short[b], bank_names[b]);
    printf("\n  bank = treebank (LLCT and EWT: dev + test); aside = not a "
        "tree, or PUNCT root (headline);\n  moved = PUNCT dependents "
        "re-attached; wn = well-nested; cross/s = crossings per tree;\n"
        "  nonproj/nonplan = share of real arcs; obs/exp = crossings / "
        "(disjoint pairs / 3).\n");
    census_table(0);
    census_table(1);
    printf("\n");
    pinned_checks();
    {
        Ratio a = ratio(2, 4), b = ratio(1, 3), c = ratio(-6, 9);
        check("rationals: 2/4 = 1/2, -6/9 = -2/3, 1/3 < 1/2, not 1/2 < 1/3",
            a.p == 1 && a.q == 2 && c.p == -2 && c.q == 3
            && ratio_less(b, a) && !ratio_less(a, b));
    }

    printf("\n  exact (headline): mean crossings per tree, crossings per "
        "word, observed / expected\n");
    for (b = 0; b < N_BANKS; b++) {
        const Stats *s = &stats[b][0];

        if (!bank_present[b] || s->trees == 0) continue;
        printf("  %-8s ", bank_short[b]);
        ratio_print(ratio(s->crossings, s->trees));
        printf("  ");
        ratio_print(ratio(s->crossings, s->words));
        printf("  ");
        ratio_print(obs_exp(s));
        printf("\n");
    }

    printf("\n  headline: trees by number of crossings (0, 1, 2, 3, 4-7, "
        "8+), root-arc crossings per tree,\n  planar core deficit (share "
        "of real arcs outside a largest crossing-free set)\n");
    for (b = 0; b < N_BANKS; b++) {
        const Stats *s = &stats[b][0];

        if (!bank_present[b] || s->trees == 0) continue;
        printf("  %-8s", bank_short[b]);
        for (i = 0; i < N_DIST; i++) printf(" %6ld", s->dist[i]);
        printf("   %.4f   %.3f%%\n",
            (double)s->root_crossings / (double)s->trees,
            100.0 * (double)s->core_deficit / (double)s->arcs);
    }

    printf("\n  headline: observed / expected by sentence length "
        "(words after PUNCT removal; trees in brackets)\n  %-8s", "bank");
    for (i = 0; i < N_LEN; i++) printf(" %15s", len_names[i]);
    printf("\n");
    for (b = 0; b < N_BANKS; b++) {
        const Stats *s = &stats[b][0];

        if (!bank_present[b] || s->trees == 0) continue;
        printf("  %-8s", bank_short[b]);
        for (i = 0; i < N_LEN; i++) {
            if (s->len_pairs[i] == 0)
                printf(" %15s", "-");
            else
                printf("  %6.4f (%5ld)", 3.0 * (double)s->len_cross[i]
                    / (double)s->len_pairs[i], s->len_trees[i]);
        }
        printf("\n");
    }
}

/* ================================================================
 * Registered predictions decided by Part B
 * ================================================================ */

static void
verdicts (void)
{
    int   b, all = 1, missing = 0, english_lower = 1;
    Ratio quarter = ratio(1, 4);
    Ratio en;

    printf("\n=== Registered predictions (Part B) ===\n\n");
    for (b = 0; b < N_BANKS; b++)
        if (bank_latin[b] && !bank_present[b]) missing = 1;
    if (!bank_present[2]) missing = 1;
    if (missing)
        printf("  (some treebanks absent: verdicts below cover only the "
            "present ones and are NOT the registered verdicts)\n");

    /* P1: obs/exp < 1/4 in every Latin treebank; English below every
     * Latin treebank */
    en = obs_exp(&stats[2][0]);
    for (b = 0; b < N_BANKS; b++) {
        Ratio oe;

        if (!bank_latin[b] || !bank_present[b]) continue;
        oe = obs_exp(&stats[b][0]);
        if (!ratio_less(oe, quarter)) all = 0;
        if (bank_present[2] && !ratio_less(en, oe)) english_lower = 0;
    }
    printf("  P1 (rarity): obs/exp < 1/4 in every Latin treebank: %s; "
        "English lower than every Latin\n  treebank: %s.  P1 %s\n",
        all ? "yes" : "NO", english_lower ? "yes" : "NO",
        all && english_lower ? "HOLDS" : "FAILS");

    /* P4: crossings per word CIRCSE > PROIEL, Perseus > LLCT */
    {
        Ratio c  = ratio(stats[0][0].crossings, stats[0][0].words);
        Ratio l  = ratio(stats[1][0].crossings, stats[1][0].words);
        Ratio pr = ratio(stats[3][0].crossings, stats[3][0].words);
        Ratio pe = ratio(stats[4][0].crossings, stats[4][0].words);
        int   ok = 1;

        if (!bank_present[3] || !bank_present[4]) {
            printf("  P4 (genre): PROIEL or Perseus absent - not decided\n");
        } else {
            ok = ratio_less(pr, c) && ratio_less(pe, c)
                && ratio_less(l, pr) && ratio_less(l, pe);
            printf("  P4 (genre): crossings per word CIRCSE > {PROIEL, "
                "Perseus} > LLCT: %s.  P4 %s\n",
                ok ? "yes" : "NO", ok ? "HOLDS" : "FAILS");
        }
    }
}

int
main (void)
{
    quick = getenv("D130_CELER") != NULL;
    printf("KNOTAPEL DEMO 130: Latin dependency trees as tangles%s\n",
        quick ? " (QUICK MODE: repo treebanks only)" : "");
    part_a();
    part_b();
    verdicts();
    printf("\n=== %d pass, %d fail ===\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
