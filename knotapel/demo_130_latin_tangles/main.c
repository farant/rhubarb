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
 *           FONTES.md, every sentence a sound tree, the four routes to
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
 *   Part C  (T2) which relations are non-projective (and non-planar),
 *           and whether those arcs agree in Case, Gender, Number;
 *           amod + det (hyperbaton). Registered prediction P2.
 *   Part D  (T2) gap degree, well-nestedness, MILD (gap degree <= 1 and
 *           well-nested), edge degree; the crossing graph's components
 *           (size, single crossing pairs). Registered prediction P3
 *           (prose Latin; Perseus is not split by genre here, so it is
 *           reported like CIRCSE, without a prediction).
 * Repo totals of Parts B-D are pinned from oracle_partb.py and
 * oracle_partcd.py (independent Python counts, in this directory).
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
#define N_REL     21   /* relations reported (last = other) */

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
    long len_words[N_LEN];
    double ratio_sum;      /* per-tree obs/exp, summed over trees with a
                            * disjoint pair (an unweighted mean) */
    long ratio_trees;
    /* Part C: per relation (all arcs, non-projective, non-planar) */
    long rel_all[N_REL];
    long rel_np[N_REL];
    long rel_nplan[N_REL];
    /* agreement (Case, Gender, Number, all three) on all, non-projective,
     * non-planar arcs; and on amod + det arcs (all, non-projective) */
    long agr_all[4];
    long agr_np[4];
    long agr_nplan[4];
    long ad_all, ad_np;
    /* P2's operand as REGISTERED (plan 1c4c1b0c): arcs in at least one
     * crossing, the root arc included (a real arc crossing another real
     * arc or the root arc) */
    long reg_arcs;
    long reg_agr[4];
    long reg_ad;
    long ad_agr_all[4];
    long ad_agr_np[4];
    /* Part D */
    long gd[4];            /* trees with gap degree 0, 1, 2, 3+ */
    long ed[4];            /* trees with edge degree 0, 1, 2, 3+ */
    long mild;             /* gap degree <= 1 and well-nested */
    long comps;            /* crossing-graph components with >= 2 arcs */
    long comp_size[5];     /* 2, 3, 4, 5-8, 9+ arcs */
    long comp_pair;        /* exactly two arcs and one crossing */
    long comp_arcs;
    s64  comp_cross;
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

static const char *const rel_names[N_REL] = {
    "nsubj", "obj", "iobj", "obl", "advmod", "advcl", "amod", "det",
    "nmod", "acl", "conj", "cc", "case", "mark", "cop", "aux", "xcomp",
    "ccomp", "appos", "nummod", "other"
};

#define REL_AMOD 6
#define REL_DET  7

/* universal relation = DEPREL up to ':' (obl:arg -> obl) */
static int
rel_index (
    chorda deprel)
{
    int i;
    i32 n = 0;

    while (n < deprel.mensura && deprel.datum[n] != ':') n++;
    for (i = 0; i < N_REL - 1; i++)
        if ((i32)strlen(rel_names[i]) == n
            && memcmp(deprel.datum, rel_names[i], (size_t)n) == 0)
            return i;
    return N_REL - 1;
}

static const char *const feat_names[3] = { "Case", "Gender", "Number" };

/* agree[0..2]: head and dependent both carry the feature, values equal;
 * agree[3]: all three */
static void
agreement (
    const ConlluLexema *h,
    const ConlluLexema *d,
    int                *agree)
{
    int f;

    agree[3] = 1;
    for (f = 0; f < 3; f++) {
        chorda a = conllu_proprietas(h->feats, feat_names[f]);
        chorda b = conllu_proprietas(d->feats, feat_names[f]);

        agree[f] = a.mensura > 0 && b.mensura > 0 && chorda_aequalis(a, b);
        if (!agree[f]) agree[3] = 0;
    }
}

static int
size_bin (
    long n)
{
    if (n <= 4) return (int)n - 2;
    if (n <= 8) return 3;
    return 4;
}

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
static long core_skipped = 0;
static long rev_bad = 0;         /* reversal invariance */
static long third_bad = 0;       /* 1/3 law over all orders */
static long third_checked = 0;
static long third_informative = 0;   /* with at least one disjoint pair */

#define COMP_MAX 63   /* arcs per crossing component in the search */

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

/* independent planar core: the crossing graph split into its connected
 * components (union-find), a maximum independent set of each by the
 * branching search above, summed; -1 if a component has more than
 * COMP_MAX arcs (none does in these treebanks) */
static int
find_root (
    int *parent,
    int  x)
{
    while (parent[x] != x) x = parent[x];
    return x;
}

static int
core_search (
    Piscina                *scratch,
    const DependentiaArbor *t)
{
    i32           n = t->numerus, k;
    i32          *dep = (i32 *)piscina_allocare(scratch,
        (memoriae_index)n * sizeof(i32));
    int          *parent = (int *)piscina_allocare(scratch,
        (memoriae_index)n * sizeof(int));
    int          *local = (int *)piscina_allocare(scratch,
        (memoriae_index)n * sizeof(int));
    unsigned long adj[COMP_MAX];
    int           m = 0, i, j, r, total = 0;

    for (k = 1; k <= n; k++)
        if (t->capita[k - 1] != 0) dep[m++] = k;
    for (i = 0; i < m; i++) parent[i] = i;
    for (i = 0; i < m; i++)
        for (j = i + 1; j < m; j++)
            if (crosses(t->capita[dep[i] - 1], dep[i],
                        t->capita[dep[j] - 1], dep[j]))
                parent[find_root(parent, j)] = find_root(parent, i);
    for (r = 0; r < m; r++) {
        int size = 0;

        if (find_root(parent, r) != r) continue;
        for (i = 0; i < m; i++)
            if (find_root(parent, i) == r) {
                if (size == COMP_MAX) return -1;
                local[size++] = i;
            }
        for (i = 0; i < size; i++) adj[i] = 0;
        for (i = 0; i < size; i++)
            for (j = i + 1; j < size; j++)
                if (crosses(t->capita[dep[local[i]] - 1], dep[local[i]],
                            t->capita[dep[local[j]] - 1], dep[local[j]])) {
                    adj[i] |= 1UL << j;
                    adj[j] |= 1UL << i;
                }
        total += mis(size == 0 ? 0UL : (1UL << size) - 1UL, adj);
    }
    return total;
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

static long comp_bad = 0;

/* Perseus by work, from the PHI code that starts each sent_id (T4
 * review): phi0972 Petronius = prose; phi0959 Ovid, phi0975 Phaedrus,
 * phi0690 Vergil = verse; anything else = unknown (none in the test
 * split) */
static int  cur_genre = -1;          /* 0 prose, 1 verse, 2 unknown */
static long perseus_trees[3][2];
static long perseus_mild[3][2];

static int
perseus_genre (
    chorda id)
{
    static const char *const prose[1] = { "phi0972" };
    static const char *const verse[3] = { "phi0959", "phi0975", "phi0690" };
    int i;

    for (i = 0; i < 1; i++)
        if (id.mensura >= 7 && memcmp(id.datum, prose[i], 7) == 0) return 0;
    for (i = 0; i < 3; i++)
        if (id.mensura >= 7 && memcmp(id.datum, verse[i], 7) == 0) return 1;
    return 2;
}        /* crossing components vs tree totals */

/* analyse one sound tree into stats[bank][col]; w[k - 1] = lexeme of
 * word k; per-tree checks */
static void
analyse (
    Piscina                   *scratch,
    const DependentiaArbor    *t,
    const ConlluLexema *const *w,
    int                        bank,
    int                        col,
    int                        law)
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
    {
        int cs = core_search(scratch, t);

        if (cs < 0) {
            core_skipped++;
        } else {
            core_checked++;
            if (cs != (int)core) core_bad++;
        }
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
        if (x.paria_disiuncta > 0) third_informative++;
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
    s->len_words[b] += n;
    if (x.paria_disiuncta > 0) {
        s->ratio_sum += 3.0 * (double)x.cruces / (double)x.paria_disiuncta;
        s->ratio_trees++;
    }

    /* Part C accumulation: relation and agreement per real arc */
    for (k = 1; k <= n; k++) {
        i32 h = t->capita[k - 1];
        int r, f, agree[4], ad;

        if (h == 0) continue;
        r = rel_index(w[k - 1]->deprel);
        agreement(w[h - 1], w[k - 1], agree);
        ad = r == REL_AMOD || r == REL_DET;
        s->rel_all[r]++;
        if (ad) s->ad_all++;
        for (f = 0; f < 4; f++) {
            s->agr_all[f] += agree[f];
            if (ad) s->ad_agr_all[f] += agree[f];
        }
        if (arcs[k - 1].non_proiectivus) {
            s->rel_np[r]++;
            if (ad) s->ad_np++;
            for (f = 0; f < 4; f++) {
                s->agr_np[f] += agree[f];
                if (ad) s->ad_agr_np[f] += agree[f];
            }
        }
        if (arcs[k - 1].cruces > 0 || arcs[k - 1].secat_radicem) {
            s->reg_arcs++;
            if (ad) s->reg_ad++;
            for (f = 0; f < 4; f++) s->reg_agr[f] += agree[f];
        }
        if (arcs[k - 1].cruces > 0) {
            s->rel_nplan[r]++;
            for (f = 0; f < 4; f++) s->agr_nplan[f] += agree[f];
        }
    }

    /* Part D accumulation */
    s->gd[m.gradus_lacunarum < 3 ? m.gradus_lacunarum : 3]++;
    s->ed[m.gradus_arcuum < 3 ? m.gradus_arcuum : 3]++;
    if (m.gradus_lacunarum <= 1 && m.bene_nidificata) s->mild++;
    if (bank == 4 && cur_genre >= 0) {
        perseus_trees[cur_genre][col]++;
        if (m.gradus_lacunarum <= 1 && m.bene_nidificata)
            perseus_mild[cur_genre][col]++;
    }
    {
        s32  *comp = (s32 *)piscina_allocare(scratch,
            (memoriae_index)n * sizeof(s32));
        s32   nc = dependentia_componentes(scratch, t, comp);
        long *size, *cr;
        long  sum_arcs = 0;
        s64   sum_cross = 0;
        int   bad = 0;
        s32   c;

        if (nc < 0) fatal("componentes");
        size = (long *)piscina_allocare(scratch,
            (memoriae_index)(nc + 1) * sizeof(long));
        cr = (long *)piscina_allocare(scratch,
            (memoriae_index)(nc + 1) * sizeof(long));
        for (c = 0; c < nc; c++) size[c] = cr[c] = 0;
        for (k = 0; k < n; k++) {
            if (comp[k] < 0) continue;
            size[comp[k]]++;
            cr[comp[k]] += (long)arcs[k].cruces;   /* each crossing twice */
        }
        for (c = 0; c < nc; c++) {
            if (size[c] < 2) {
                if (cr[c] != 0) bad = 1;
                continue;
            }
            if (cr[c] % 2 != 0) bad = 1;
            cr[c] /= 2;
            if (size[c] == 2 && cr[c] != 1) bad = 1;
            s->comps++;
            s->comp_size[size_bin(size[c])]++;
            if (size[c] == 2) s->comp_pair++;
            s->comp_arcs += size[c];
            s->comp_cross += cr[c];
            sum_arcs += size[c];
            sum_cross += cr[c];
        }
        if (bad || sum_arcs != (long)x.arcus_secti || sum_cross != x.cruces)
            comp_bad++;
    }
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
        cur_genre = bank == 4 ? perseus_genre(s->id) : -1;
        /* column 1: as annotated (the 1/3 law runs on repo trees) */
        analyse(scratch, &a.arbor, a.verba, bank, 1, !f->nc);
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
                /* kept words in order: the renumbering of omittere */
                const ConlluLexema **kw = (const ConlluLexema **)
                    piscina_allocare(scratch, (memoriae_index)e.numerus
                    * sizeof(const ConlluLexema *));
                i32 q = 0;

                for (k = 0; k < a.arbor.numerus; k++)
                    if (!punct[k]) kw[q++] = a.verba[k];
                if (q != e.numerus) fatal("kept words");
                stats[bank][0].contractions += (long)moved;
                analyse(scratch, &e, kw, bank, 0, 0);
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
        sprintf(msg, "planar core (interval DP) == independent search per "
            "crossing component, on %ld tree analyses (both columns; %ld "
            "skipped)", core_checked, core_skipped);
        check(msg, core_bad == 0 && core_checked > 0);
        sprintf(msg, "reversing word order keeps crossings, root crossings,"
            " gap and edge degree, well-nestedness, planar core");
        check(msg, rev_bad == 0);
        check("crossing components: arcs and crossings sum to the tree's "
            "non-planar arcs and crossings; every 2-arc component is one "
            "crossing", comp_bad == 0);
        sprintf(msg, "1/3 law: sum of crossings over all n! orders == n! "
            "x pairs / 3, on %ld repo trees with <= %d words (%ld with a "
            "disjoint pair)", third_checked, quick ? 5 : 7, third_informative);
        check(msg, third_bad == 0 && third_informative > 0);
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
static const long pin_lens[3][N_LEN][4] = {
    { { 484, 321, 2977, 2738 }, { 267, 754, 18597, 3926 },
      { 77, 355, 17423, 1885 }, { 49, 383, 28096, 1808 },
      { 16, 355, 38905, 1146 } },
    { { 944, 48, 11862, 6934 }, { 177, 130, 10378, 2432 },
      { 166, 300, 34790, 3939 }, { 143, 286, 98610, 5766 },
      { 304, 1316, 804679, 22392 } },
    { { 2387, 3, 10620, 11429 }, { 1063, 24, 69298, 15609 },
      { 400, 30, 90497, 9905 }, { 166, 28, 94361, 6150 },
      { 17, 7, 24859, 978 } }
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
                || s->len_pairs[k] != (s64)pin_lens[b][k][2]
                || s->len_words[k] != pin_lens[b][k][3])
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
    printf("\n  headline: crossings per WORD by sentence length (the pairs "
        "that could cross grow ~ n^2,\n  so obs/exp falls with length by "
        "construction; this is the raw rate)\n  %-8s", "bank");
    for (i = 0; i < N_LEN; i++) printf(" %8s", len_names[i]);
    printf("\n");
    for (b = 0; b < N_BANKS; b++) {
        const Stats *s = &stats[b][0];

        if (!bank_present[b] || s->trees == 0) continue;
        printf("  %-8s", bank_short[b]);
        for (i = 0; i < N_LEN; i++) {
            if (s->len_words[i] == 0)
                printf(" %8s", "-");
            else
                printf(" %8.4f", (double)s->len_cross[i]
                    / (double)s->len_words[i]);
        }
        printf("\n");
    }
}

/* ================================================================
 * Part C: which relations cross, and do they agree
 * ================================================================ */

static double
pct (
    long a,
    long b)
{
    return b == 0 ? 0.0 : 100.0 * (double)a / (double)b;
}

/* Parts C and D totals of the repo treebanks, both columns, pinned from
 * oracle_partcd.py (independent Python: own FEATS parsing, head walks,
 * well-nestedness over ALL disjoint subtree pairs, edge degree by
 * union-find with least-depth roots) - constants, said so */
typedef struct {
    long gd[4], ed[4], wn, mild, comps, size[5];
    s64  ccross;
    long agr_all[4], agr_np[4], ad_all, ad_np, ad_agr_all[4], ad_agr_np[4];
    long amod_np, det_np;
    long agr_nplan[4], agr_reg[4], reg, ad_reg;
} PinCD;

static const PinCD pins_cd[3][2] = {
    { { { 375, 458, 58, 2 }, { 375, 440, 65, 13 }, 828, 787, 782,
        { 339, 170, 122, 126, 25 }, 2168,
        { 2582, 2537, 6261, 2193 }, { 930, 877, 1034, 829 }, 1561, 702,
        { 1521, 1479, 1531, 1464 }, { 693, 666, 693, 662 }, 447, 255,
        { 1029, 963, 1898, 910 }, { 1057, 987, 1927, 931 }, 2792, 741 },
      { { 375, 458, 58, 2 }, { 375, 440, 65, 13 }, 828, 787, 782,
        { 339, 170, 122, 126, 25 }, 2168,
        { 2582, 2537, 6261, 2193 }, { 930, 877, 1034, 829 }, 1561, 702,
        { 1521, 1479, 1531, 1464 }, { 693, 666, 693, 662 }, 447, 255,
        { 1029, 963, 1898, 910 }, { 1057, 987, 1927, 931 }, 2792, 741 } },
    { { { 1248, 445, 38, 3 }, { 1248, 463, 17, 6 }, 1732, 1692, 576,
        { 256, 103, 59, 132, 26 }, 2080,
        { 12139, 12883, 22840, 8990 }, { 306, 298, 364, 251 }, 6883, 128,
        { 5772, 5814, 6651, 5211 }, { 125, 127, 126, 124 }, 95, 33,
        { 382, 330, 772, 268 }, { 383, 331, 773, 269 }, 2194, 130 },
      { { 1248, 445, 38, 3 }, { 1248, 463, 17, 6 }, 1732, 1692, 592,
        { 224, 111, 60, 158, 39 }, 2658,
        { 12139, 12883, 22840, 8990 }, { 306, 298, 364, 251 }, 6883, 128,
        { 5772, 5814, 6651, 5211 }, { 125, 127, 126, 124 }, 95, 33,
        { 383, 331, 773, 269 }, { 383, 331, 773, 269 }, 2449, 130 } },
    { { { 3973, 60, 0, 0 }, { 3973, 59, 1, 0 }, 4033, 4033, 56,
        { 37, 10, 3, 6, 0 }, 92,
        { 3, 0, 7941, 0 }, { 0, 0, 13, 0 }, 6268, 0,
        { 0, 0, 273, 0 }, { 0, 0, 0, 0 }, 0, 0,
        { 0, 0, 31, 0 }, { 0, 0, 31, 0 }, 152, 5 },
      { { 4018, 60, 0, 0 }, { 4018, 59, 1, 0 }, 4078, 4078, 61,
        { 36, 14, 3, 8, 0 }, 111,
        { 3, 0, 7941, 0 }, { 0, 0, 13, 0 }, 6268, 0,
        { 0, 0, 273, 0 }, { 0, 0, 0, 0 }, 0, 0,
        { 0, 0, 31, 0 }, { 0, 0, 31, 0 }, 171, 5 } }
};

static int
same4 (
    const long *a,
    const long *b,
    int         n)
{
    int i;

    for (i = 0; i < n; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

static void
pinned_cd (void)
{
    int b, col;

    for (b = 0; b < 3; b++)
        for (col = 0; col < 2; col++) {
            const Stats *s = &stats[b][col];
            const PinCD *q = &pins_cd[b][col];
            const char  *cn = col == 0 ? "headline" : "annotated";
            char         msg[200];

            sprintf(msg, "%s %s: Part C (relations, agreement, registered "
                "P2 operand) = oracle_partcd.py", bank_short[b], cn);
            check(msg, same4(s->agr_all, q->agr_all, 4)
                && same4(s->agr_np, q->agr_np, 4) && s->ad_all == q->ad_all
                && s->ad_np == q->ad_np
                && same4(s->ad_agr_all, q->ad_agr_all, 4)
                && same4(s->ad_agr_np, q->ad_agr_np, 4)
                && s->rel_np[REL_AMOD] == q->amod_np
                && s->rel_np[REL_DET] == q->det_np
                && same4(s->agr_nplan, q->agr_nplan, 4)
                && same4(s->reg_agr, q->agr_reg, 4)
                && s->reg_arcs == q->reg && s->reg_ad == q->ad_reg);
            sprintf(msg, "%s %s: Part D (gap and edge degree, well-nested, "
                "mild, components) = oracle_partcd.py", bank_short[b], cn);
            check(msg, same4(s->gd, q->gd, 4) && same4(s->ed, q->ed, 4)
                && s->well_nested == q->wn && s->mild == q->mild
                && s->comps == q->comps && same4(s->comp_size, q->size, 5)
                && s->comp_cross == q->ccross && s->comp_pair == q->size[0]);
        }
}

static void
relation_table (
    int col)
{
    int b, r;

    printf("\n  %s; per relation: share of NON-PROJECTIVE arcs / share of "
        "ALL real arcs (%%)\n  %-7s", col == 0 ? "headline" : "annotated",
        "rel");
    for (b = 0; b < N_BANKS; b++)
        if (bank_present[b]) printf(" %12s", bank_short[b]);
    printf("\n");
    for (r = 0; r < N_REL; r++) {
        printf("  %-7s", rel_names[r]);
        for (b = 0; b < N_BANKS; b++) {
            const Stats *s = &stats[b][col];

            if (!bank_present[b]) continue;
            printf("  %5.1f/%5.1f", pct(s->rel_np[r], s->nonproj_arcs),
                pct(s->rel_all[r], s->arcs));
        }
        printf("\n");
    }
}

static void
agreement_table (
    int col)
{
    int b, f;

    printf("\n  %s; agreement = head and dependent both carry the feature, "
        "values equal (%% of arcs);\n  REG = P2's registered operand (arcs "
        "in a crossing, root arc included), nonplan = real crossings only\n",
        col == 0 ? "headline" : "annotated");
    printf("  %-8s %-24s %-24s %6s %6s %13s\n", "",
        "all: Case Gend Numb all3", "nonproj: C    G    N  all3", "REG",
        "nplan", "a+d all/np");
    for (b = 0; b < N_BANKS; b++) {
        const Stats *s = &stats[b][col];

        if (!bank_present[b]) continue;
        printf("  %-8s     ", bank_short[b]);
        for (f = 0; f < 4; f++) printf(" %4.1f", pct(s->agr_all[f], s->arcs));
        printf("         ");
        for (f = 0; f < 4; f++)
            printf(" %4.1f", pct(s->agr_np[f], s->nonproj_arcs));
        printf(" %6.1f %6.1f %6.1f/%5.1f\n",
            pct(s->reg_agr[3], s->reg_arcs),
            pct(s->agr_nplan[3], s->nonplanar_arcs),
            pct(s->ad_agr_all[3], s->ad_all), pct(s->ad_agr_np[3], s->ad_np));
    }
    printf("\n  %s; amod + det: share of all real arcs -> of non-projective "
        "arcs -> of REG arcs (%%)\n", col == 0 ? "headline" : "annotated");
    for (b = 0; b < N_BANKS; b++) {
        const Stats *s = &stats[b][col];

        if (!bank_present[b]) continue;
        printf("  %-8s %5.1f -> %5.1f -> %5.1f   (amod %5.1f -> %5.1f, det "
            "%5.1f -> %5.1f)\n", bank_short[b], pct(s->ad_all, s->arcs),
            pct(s->ad_np, s->nonproj_arcs), pct(s->reg_ad, s->reg_arcs),
            pct(s->rel_all[REL_AMOD], s->arcs),
            pct(s->rel_np[REL_AMOD], s->nonproj_arcs),
            pct(s->rel_all[REL_DET], s->arcs),
            pct(s->rel_np[REL_DET], s->nonproj_arcs));
    }
}

static void
part_c (void)
{
    int b, r;

    printf("\n=== Part C: which relations cross, and do they agree ===\n");
    relation_table(0);
    printf("\n  headline; per relation: share of NON-PLANAR (crossed) arcs "
        "(%%), beside\n  %-7s", "rel");
    for (b = 0; b < N_BANKS; b++)
        if (bank_present[b]) printf(" %8s", bank_short[b]);
    printf("\n");
    for (r = 0; r < N_REL; r++) {
        printf("  %-7s", rel_names[r]);
        for (b = 0; b < N_BANKS; b++)
            if (bank_present[b])
                printf(" %8.1f", pct(stats[b][0].rel_nplan[r],
                    stats[b][0].nonplanar_arcs));
        printf("\n");
    }
    agreement_table(0);
    relation_table(1);
    agreement_table(1);
    printf("\n");
    pinned_cd();
    if (bank_present[4]) {
        /* Perseus by work: aggregates from the T4 review's own oracle
         * (recensor-d130, sent_id work codes) - constants, said so */
        check("Perseus headline by work = the T4 review's oracle (prose 547"
            " trees, 536 mild; verse 392, 347; unknown 0)",
            perseus_trees[0][0] == 547 && perseus_mild[0][0] == 536
            && perseus_trees[1][0] == 392 && perseus_mild[1][0] == 347
            && perseus_trees[2][0] == 0);
    }
}

/* ================================================================
 * Part D: mildness and shape
 * ================================================================ */

static void
part_d (void)
{
    int b, i, col, g;

    printf("\n=== Part D: mildness and shape ===\n");
    for (col = 0; col < 2; col++) {
        printf("\n  %s; %% of trees by gap degree (0 1 2 3+), well-nested, "
            "MILD = gap degree <= 1 and well-nested,\n  by edge degree "
            "(0 1 2 3+)\n", col == 0 ? "headline" : "annotated");
        printf("  %-8s %5s %5s %5s %5s %6s %6s   %5s %5s %5s %5s\n", "bank",
            "gd0", "gd1", "gd2", "gd3+", "wn", "MILD", "ed0", "ed1", "ed2",
            "ed3+");
        for (b = 0; b < N_BANKS; b++) {
            const Stats *s = &stats[b][col];

            if (!bank_present[b]) continue;
            printf("  %-8s", bank_short[b]);
            for (i = 0; i < 4; i++) printf(" %5.1f", pct(s->gd[i], s->trees));
            printf(" %6.2f %6.2f  ", pct(s->well_nested, s->trees),
                pct(s->mild, s->trees));
            for (i = 0; i < 4; i++) printf(" %5.1f", pct(s->ed[i], s->trees));
            printf("\n");
        }
        if (bank_present[4]) {
            static const char *const gname[3] = {
                "prose (Petronius)", "verse (Ovid, Phaedrus, Vergil)",
                "unknown work"
            };

            for (g = 0; g < 3; g++)
                if (perseus_trees[g][col] > 0)
                    printf("  Perseus %-31s mild %6.2f%% (%ld of %ld)\n",
                        gname[g], pct(perseus_mild[g][col],
                        perseus_trees[g][col]), perseus_mild[g][col],
                        perseus_trees[g][col]);
        }
        printf("\n  %s; crossing-graph components (>= 2 arcs): number, per "
            "100 words, by size in arcs\n  (2 3 4 5-8 9+, %% of components),"
            " share that are ONE crossing pair, crossings per component\n",
            col == 0 ? "headline" : "annotated");
        printf("  %-8s %6s %7s %5s %5s %5s %5s %5s %6s %8s\n", "bank",
            "comps", "/100w", "2", "3", "4", "5-8", "9+", "pair%", "cross/c");
        for (b = 0; b < N_BANKS; b++) {
            const Stats *s = &stats[b][col];

            if (!bank_present[b]) continue;
            printf("  %-8s %6ld %7.3f", bank_short[b], s->comps,
                100.0 * (double)s->comps / (double)s->words);
            for (i = 0; i < 5; i++)
                printf(" %5.1f", pct(s->comp_size[i], s->comps));
            printf(" %6.1f %8.3f\n", pct(s->comp_pair, s->comps),
                s->comps == 0 ? 0.0
                : (double)s->comp_cross / (double)s->comps);
        }
    }
}

/* ================================================================
 * Registered predictions
 * ================================================================ */

/* the registered rules, per treebank (exact integer comparisons) */
static int
p1_rule (
    const Stats *s)
{
    return 12 * s->crossings < s->pairs;    /* 3c / pairs < 1/4 */
}

/* P2 on its REGISTERED operand (plan 1c4c1b0c: "Non-projective arcs =
 * arcs in at least one crossing", root arc included) */
static int
p2_agree_reg (
    const Stats *s)
{
    return 2 * (s64)s->reg_agr[3] * (s64)s->arcs
        >= 3 * (s64)s->agr_all[3] * (s64)s->reg_arcs;
}

static int
p2_ad_reg (
    const Stats *s)
{
    return (s64)s->reg_ad * (s64)s->arcs > (s64)s->ad_all * (s64)s->reg_arcs;
}

/* P2 on the AMENDED operand (ce8816c8: the per-arc non-projective arc) */
static int
p2_agree_rule (
    const Stats *s)
{
    return 2 * (s64)s->agr_np[3] * (s64)s->arcs
        >= 3 * (s64)s->agr_all[3] * (s64)s->nonproj_arcs;
}

static int
p2_ad_rule (
    const Stats *s)
{
    return (s64)s->ad_np * (s64)s->arcs > (s64)s->ad_all * (s64)s->nonproj_arcs;
}

static int
p3_rule (
    const Stats *s)
{
    return 20 * s->mild >= 19 * s->trees;
}

/* the rules on the pinned repo totals, against answers computed by hand
 * from oracle_partb.py / oracle_partcd.py:
 *   P1 CIRCSE 12 * 2168 = 26016 < 105998; LLCT 24960 < 960319
 *   P2 agreement CIRCSE 2 * 829 * 10610 >= 3 * 2193 * 1284 (yes),
 *     LLCT 2 * 251 * 39729 = 19.9M < 3 * 8990 * 1191 = 32.1M (no)
 *   P2 amod+det CIRCSE 702 * 10610 > 1561 * 1284 (yes),
 *     LLCT 128 * 39729 = 5.09M < 6883 * 1191 = 8.20M (no)
 *   P3 LLCT 20 * 1692 = 33840 >= 19 * 1734 = 32946 (yes),
 *     CIRCSE 20 * 787 = 15740 < 19 * 893 = 16967 (no)
 *   P2 registered operand, agreement CIRCSE 2 * 931 * 10610 = 19,755,820
 *     >= 3 * 2193 * 2792 = 18,368,568 (yes), LLCT 2 * 269 * 39729 =
 *     21,374,202 < 3 * 8990 * 2194 = 59,172,180 (no); amod+det CIRCSE
 *     741 * 10610 = 7,862,010 > 1561 * 2792 = 4,358,312 (yes), LLCT
 *     130 * 39729 = 5,164,770 < 6883 * 2194 = 15,101,302 (no) */
static void
rules_check (void)
{
    check("registered rules on the repo totals give the hand-computed "
        "answers (P1 yes/yes, P2 registered and amended operands yes/no "
        "for both halves, P3 LLCT yes, CIRCSE no)",
        p1_rule(&stats[0][0]) && p1_rule(&stats[1][0])
        && p2_agree_rule(&stats[0][0]) && !p2_agree_rule(&stats[1][0])
        && p2_ad_rule(&stats[0][0]) && !p2_ad_rule(&stats[1][0])
        && p2_agree_reg(&stats[0][0]) && !p2_agree_reg(&stats[1][0])
        && p2_ad_reg(&stats[0][0]) && !p2_ad_reg(&stats[1][0])
        && p3_rule(&stats[1][0]) && !p3_rule(&stats[0][0]));
}

/* each rule at its threshold on constructed totals: P1 1/4 is NOT
 * below 1/4, 1/4.33 is; P2 agreement x1.5 exactly passes, x1.45 fails
 * (both operands); amod + det at the same share fails (strictly
 * larger), one more passes; P3 exactly 95% passes, 94.5% fails */
static void
threshold_check (void)
{
    Stats a, b;
    int   ok = 1;

    memset(&a, 0, sizeof(a));
    a.crossings = 1; a.pairs = 12;
    if (p1_rule(&a)) ok = 0;
    a.pairs = 13;
    if (!p1_rule(&a)) ok = 0;

    memset(&a, 0, sizeof(a));
    a.arcs = 100; a.agr_all[3] = 20;
    a.reg_arcs = 100; a.reg_agr[3] = 30;          /* x1.50 */
    a.nonproj_arcs = 100; a.agr_np[3] = 30;
    b = a;
    b.reg_agr[3] = 29; b.agr_np[3] = 29;          /* x1.45 */
    if (!p2_agree_reg(&a) || !p2_agree_rule(&a)) ok = 0;
    if (p2_agree_reg(&b) || p2_agree_rule(&b)) ok = 0;

    memset(&a, 0, sizeof(a));
    a.arcs = 100; a.ad_all = 10;
    a.reg_arcs = 50; a.reg_ad = 5;                /* same share */
    a.nonproj_arcs = 50; a.ad_np = 5;
    b = a;
    b.reg_ad = 6; b.ad_np = 6;
    if (p2_ad_reg(&a) || p2_ad_rule(&a)) ok = 0;
    if (!p2_ad_reg(&b) || !p2_ad_rule(&b)) ok = 0;

    memset(&a, 0, sizeof(a));
    a.trees = 200; a.mild = 190;                  /* 95% */
    b = a;
    b.mild = 189;                                 /* 94.5% */
    if (!p3_rule(&a) || p3_rule(&b)) ok = 0;

    check("each registered rule at its threshold on constructed totals "
        "(1/4 not below 1/4; x1.5 passes, x1.45 fails; equal amod+det "
        "share fails; 95% passes, 94.5% fails)", ok);
}

static void
verdicts (void)
{
    int   b, i, all = 1, missing = 0, english_lower = 1;
    Ratio en;

    printf("\n=== Registered predictions ===\n\n");
    rules_check();
    threshold_check();
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
        if (!p1_rule(&stats[b][0])) all = 0;
        if (bank_present[2] && !ratio_less(en, oe)) english_lower = 0;
    }
    printf("  P1 (rarity): POOLED obs/exp < 1/4 in every Latin treebank: %s; "
        "English lower than every Latin\n  treebank: %s.  P1 %s\n",
        all ? "yes" : "NO", english_lower ? "yes" : "NO",
        all && english_lower ? "HOLDS" : "FAILS");
    printf("     (caveat, not the rule: the UNWEIGHTED mean of per-tree "
        "obs/exp is");
    for (b = 0; b < N_BANKS; b++)
        if (bank_present[b] && stats[b][0].ratio_trees > 0)
            printf(" %s %.4f", bank_short[b], stats[b][0].ratio_sum
                / (double)stats[b][0].ratio_trees);
    printf(")\n");

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

    /* P2: the operand as REGISTERED (arcs in at least one crossing, root
     * arc included; plan 1c4c1b0c) decides; the AMENDED per-arc operand
     * (ce8816c8, after the T1a review, before any census) beside it */
    {
        int op;

        for (op = 0; op < 2; op++) {
            int agree_ok = 1, ad_ok = 1;

            printf("  P2 %s\n", op == 0
                ? "REGISTERED operand: arcs in at least one crossing (root "
                  "arc included)"
                : "AMENDED operand (disclosed): per-arc non-projective arcs");
            for (b = 0; b < N_BANKS; b++) {
                const Stats *s = &stats[b][0];
                long  sel = op == 0 ? s->reg_arcs : s->nonproj_arcs;
                long  agr = op == 0 ? s->reg_agr[3] : s->agr_np[3];
                long  ad = op == 0 ? s->reg_ad : s->ad_np;
                int   a_ok, d_ok;

                if (!bank_latin[b] || !bank_present[b]) continue;
                a_ok = op == 0 ? p2_agree_reg(s) : p2_agree_rule(s);
                d_ok = op == 0 ? p2_ad_reg(s) : p2_ad_rule(s);
                if (!a_ok) agree_ok = 0;
                if (!d_ok) ad_ok = 0;
                printf("     %-8s all3 %5.1f%% vs all %5.1f%% (x%.3f) %s; "
                    "amod+det %5.1f%% vs %5.1f%% %s\n", bank_short[b],
                    pct(agr, sel), pct(s->agr_all[3], s->arcs),
                    s->agr_all[3] == 0 || sel == 0 ? 0.0
                    : ((double)agr / (double)sel)
                      / ((double)s->agr_all[3] / (double)s->arcs),
                    a_ok ? "yes" : "no", pct(ad, sel),
                    pct(s->ad_all, s->arcs), d_ok ? "yes" : "no");
            }
            printf("     >= 1.5x in every Latin treebank: %s; amod + det "
                "over-represented in every one: %s.  P2 (%s) %s\n",
                agree_ok ? "yes" : "NO", ad_ok ? "yes" : "NO",
                op == 0 ? "registered" : "amended",
                agree_ok && ad_ok ? "HOLDS" : "FAILS");
        }
    }

    /* P3: >= 95% of trees mild in every PROSE Latin treebank: LLCT,
     * PROIEL, ITTB, UDante, and Perseus's prose (Petronius, separated by
     * the work code of its sent_id - T4 review) */
    {
        static const int prose[4] = { 1, 3, 5, 6 };
        int              ok = 1, any = 0;

        for (i = 0; i < 4; i++) {
            const Stats *s = &stats[prose[i]][0];

            if (!bank_present[prose[i]]) continue;
            any = 1;
            if (!p3_rule(s)) ok = 0;
            printf("  P3 %-8s mild %6.2f%% (%ld of %ld)\n",
                bank_short[prose[i]], pct(s->mild, s->trees), s->mild,
                s->trees);
        }
        if (bank_present[4] && perseus_trees[0][0] > 0) {
            if (!(20 * perseus_mild[0][0] >= 19 * perseus_trees[0][0]))
                ok = 0;
            printf("  P3 Perseus prose (Petronius) mild %6.2f%% (%ld of "
                "%ld)\n", pct(perseus_mild[0][0], perseus_trees[0][0]),
                perseus_mild[0][0], perseus_trees[0][0]);
        }
        if (bank_present[0])
            printf("  (no prediction, verse) CIRCSE mild %6.2f%%\n",
                pct(stats[0][0].mild, stats[0][0].trees));
        if (bank_present[4] && perseus_trees[1][0] > 0)
            printf("  (no prediction, verse) Perseus verse mild %6.2f%% (%ld "
                "of %ld)\n", pct(perseus_mild[1][0], perseus_trees[1][0]),
                perseus_mild[1][0], perseus_trees[1][0]);
        if (any)
            printf("  P3 (mildness): >= 95%% mild in every prose Latin "
                "treebank: %s.  P3 %s\n", ok ? "yes" : "NO",
                ok ? "HOLDS" : "FAILS");
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
    part_c();
    part_d();
    verdicts();
    printf("\n=== %d pass, %d fail ===\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
