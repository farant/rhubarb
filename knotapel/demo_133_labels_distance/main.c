/*
 * KNOTAPEL DEMO 133: Labels Buy Distance (N19)
 * ============================================
 *
 * D131 and D132 (KC 58, 59) suggest two ways a displaced word stays
 * recoverable: a LABEL (agreement repeats the head's features: the head
 * can be found by content - adjectives) or CLOSENESS (a genitive's
 * ending names no head; displaced genitives are few and short). In umbra
 * terms (brainstorm XV): a labelled open slot can WAIT, an unlabelled
 * one must close soon. Plan: plan.md (approved 2026-10-19 after a design
 * review before any data).
 *
 *   Part A  (T1) the treebanks (D132's reader, headline without PUNCT);
 *           populations and exclusions; D132's displaced counts and
 *           genitive distance-limited E reproduced; F two ways; the
 *           design review's hand sentences (A1-A6, G1-G5); the exact
 *           stratified test against brute force.
 *   Part B  (T1) registered: L1 (displaced adjectives wait longer than
 *           displaced genitives: pooled exact test p <= 1/20 AND T > E in
 *           four of the five non-LLCT treebanks), L1u (dependent-first
 *           only), L3 (within adjectives: a UNIQUE label waits longer than
 *           a SHARED one). Descriptive: the excess beyond distance (L2),
 *           sensitivities, baselines.
 *
 * Operand F: the words strictly inside an arc's span that its head does
 * not dominate (the witnesses of its non-projectivity; >= 1 for a
 * displaced arc). Statistic: the total of min(F, 10) over a group.
 * Strata: treebank (Perseus split into prose and verse) x direction
 * (dependent-first / head-first) x sentence length (<= 10, 11-20, > 20).
 * Exact test: group labels permuted within strata; per stratum the
 * subset-sum count distribution (dynamic programming, exact big
 * integers, magnus), convolved across strata; p = P(T >= t_obs).
 *
 * Data: D130's six Latin treebanks. In the repo (CC BY-SA 4.0): CIRCSE
 * test, LLCT dev + test. NC (PROIEL, Perseus, ITTB, UDante test) read
 * from $D133_UD_NC (default oratio/build/ud); AGGREGATES ONLY.
 *
 * House libraries: conllu, dependentia, fractio, magnus (and their
 * closure), hence latina.h: Roman numerals and Latin keywords are macros
 * here (never name a variable I, V, X, L, C, D or M, or per, si, ...).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_133_labels_distance/main.c &&
 *   bash build/aedilis/main/struere.sh &&
 *   D133_UD_NC=<dir with the NC .conllu files> ./build/aedilis/main/main
 * Quick mode for plants: D133_CELER=1 (repo treebanks only; verdicts not
 * decided).
 */

#include "conllu.h"
#include "dependentia.h"
#include "fractio.h"
#include "magnus.h"
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

static void
pct (
    char *buf,
    int   size,
    long  num,
    long  den)
{
    long x;

    if (den == 0) {
        snprintf(buf, (size_t)size, "-");
        return;
    }
    x = (num * 20000 + den) / (2 * den);
    snprintf(buf, (size_t)size, "%ld.%02ld%%", x / 100, x % 100);
}

/* ================================================================
 * Treebanks
 * ================================================================ */

#define N_FILES 7
#define N_BANKS 6
#define N_BANK7 7         /* strata banks: Perseus prose (3) and verse (6) */
#define BANK_LLCT 1
#define BANK_PERSEUS 3
#define MAXN    1024
#define FCAP    10        /* min(F, FCAP) */

typedef struct {
    const char *file;
    int         nc;
    int         bank;
    long        sentences;
    long        words;
    long        ranges;
} TreebankFile;

static const TreebankFile files[N_FILES] = {
    { "la_circse-ud-test.conllu", 0, 0, 893, 11503, 316 },
    { "la_llct-ud-dev.conllu", 0, 1, 850, 24189, 3 },
    { "la_llct-ud-test.conllu", 0, 1, 884, 24079, 2 },
    { "la_proiel-ud-test.conllu", 1, 2, -1, -1, -1 },
    { "la_perseus-ud-test.conllu", 1, 3, -1, -1, -1 },
    { "la_ittb-ud-test.conllu", 1, 4, -1, -1, -1 },
    { "la_udante-ud-test.conllu", 1, 5, -1, -1, -1 }
};

static const char *const bank_names[N_BANKS] = {
    "CIRCSE", "LLCT", "PROIEL", "Perseus", "ITTB", "UDante"
};

static int bank_present[N_BANKS];

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
}

/* ================================================================
 * Sentences: heads, dominance, the fast arc test (as D132)
 * ================================================================ */

typedef struct {
    int            n;
    int            head[MAXN];
    unsigned char *dom;            /* dom[a * n + b]: a is b or above b */
} Sent;

static void
sent_dominance (
    Sent *s)
{
    int b, a, steps;

    s->dom = (unsigned char *)calloc((size_t)s->n * (size_t)s->n, 1);
    if (s->dom == NULL) fatal("memory");
    for (b = 0; b < s->n; b++) {
        a = b;
        steps = 0;
        for (;;) {
            s->dom[a * s->n + b] = 1;
            if (s->head[a] == 0) break;
            a = s->head[a] - 1;
            if (++steps > s->n) fatal("cycle");
        }
    }
}

static int
arc_displaced (
    const Sent *s,
    const int  *pos,
    int         c,
    int         g,
    int         rehung)
{
    int lo = pos[c] < pos[g] ? pos[c] : pos[g];
    int hi = pos[c] < pos[g] ? pos[g] : pos[c];
    int w;

    for (w = 0; w < s->n; w++) {
        int dominated;

        if (w == c || w == g || pos[w] <= lo || pos[w] >= hi) continue;
        dominated = s->dom[c * s->n + w]
            || (rehung && s->dom[g * s->n + w]);
        if (!dominated) return 1;
    }
    return 0;
}

/* F: words strictly inside the arc (head h, dependent g, identity
 * positions) that h does not dominate */
static int
foreign_words (
    const Sent *s,
    int         h,
    int         g)
{
    int lo = h < g ? h : g, hi = h < g ? g : h, w, f = 0;

    for (w = lo + 1; w < hi; w++)
        if (!s->dom[h * s->n + w]) f++;
    return f;
}

/* the same count by walking each in-span word's ancestors (a check) */
static int
foreign_words_walk (
    const Sent *s,
    int         h,
    int         g)
{
    int lo = h < g ? h : g, hi = h < g ? g : h, w, f = 0;

    for (w = lo + 1; w < hi; w++) {
        int a = w, under = 0;

        for (;;) {
            if (a == h) {
                under = 1;
                break;
            }
            if (s->head[a] == 0) break;
            a = s->head[a] - 1;
        }
        f += !under;
    }
    return f;
}

static int
alternatives (
    const Sent          *s,
    const int           *pos,
    int                  g,
    const unsigned char *host)
{
    int c, alt = 0, h = s->head[g] - 1;

    for (c = 0; c < s->n; c++) {
        if (!host[c] || c == h || c == g || s->dom[g * s->n + c]) continue;
        if (!arc_displaced(s, pos, c, g, 1)) alt++;
    }
    return alt;
}

/* g moved alone through every position (others keep their order);
 * maxdist >= 0 keeps only placements no farther from the head than that.
 * *m displaced placements, *k of them safe; *less / *equal count the
 * displaced placements whose distance to the head is below / equal to
 * obs_dist (for the mid-rank percentile) */
static void
null_share (
    const Sent          *s,
    int                  g,
    const unsigned char *host,
    int                  maxdist,
    int                  obs_dist,
    int                 *m,
    int                 *k,
    int                 *less,
    int                 *equal)
{
    static int others[MAXN], pos[MAXN];
    int        no = 0, p, i, h = s->head[g] - 1;

    for (i = 0; i < s->n; i++)
        if (i != g) others[no++] = i;
    *m = *k = *less = *equal = 0;
    for (p = 0; p < s->n; p++) {
        int q = 0, dist;

        for (i = 0; i < s->n; i++) {
            if (i == p) {
                pos[g] = i;
                continue;
            }
            pos[others[q++]] = i;
        }
        dist = pos[g] > pos[h] ? pos[g] - pos[h] : pos[h] - pos[g];
        if (maxdist >= 0 && dist > maxdist) continue;
        if (!arc_displaced(s, pos, h, g, 0)) continue;
        (*m)++;
        *k += alternatives(s, pos, g, host) == 0;
        if (dist < obs_dist) (*less)++;
        if (dist == obs_dist) (*equal)++;
    }
}

/* ================================================================
 * Items: every displaced arc of the registered populations
 * ================================================================ */

typedef struct {
    int  kind;          /* 0 adjective, 1 genitive */
    int  bank, bank7;
    int  dep_first;
    int  lenbin;
    int  f, dist;
    int  unique;        /* adjectives: no other agreeing noun */
    int  fw;            /* foreign words all function words */
    int  first;         /* first displaced arc of its kind in its sentence */
    int  safe;          /* every-noun hosts */
    int  kn, mn;        /* distance-limited null share, every-noun hosts */
    int  pl, pe, pm;    /* percentile counts: less, equal, of */
} Item;

static Item *items = NULL;
static long  n_items = 0, cap_items = 0;

static void
push_item (
    const Item *it)
{
    if (n_items == cap_items) {
        cap_items = cap_items ? cap_items * 2 : 1024;
        items = (Item *)realloc(items, (size_t)cap_items * sizeof(Item));
        if (items == NULL) fatal("memory");
    }
    items[n_items++] = *it;
}

typedef struct {
    long trees;
    long gen_all, gen_rel, gen_prs, gen_nonnoun, gen_pop, gen_disp;
    long adj_all, adj_incomplete, adj_multigender, adj_nonnoun, adj_pop,
         adj_disp;
    long d132_gen_disp, d132_adj_disp;     /* D132's full populations */
    long d132_gen_k[MAXN + 1];             /* D132 genitive near null */
    long nest_n[2], nest_dist[2];          /* nested arcs: count, raw sum */
} Stats;

static Stats stats[N_BANKS];

static long f_checked = 0, f_mismatch = 0, f_zero_nested_bad = 0;

static int
upos_is (
    const ConlluLexema *x,
    const char         *u)
{
    return chorda_aequalis_literis(x->upos, u);
}

static int
deprel_base_is (
    const ConlluLexema *x,
    const char         *r)
{
    size_t len = strlen(r);

    if ((size_t)x->deprel.mensura < len) return 0;
    if (memcmp(x->deprel.datum, r, len) != 0) return 0;
    return (size_t)x->deprel.mensura == len || x->deprel.datum[len] == ':';
}

static int
feat_is (
    const ConlluLexema *x,
    const char         *key,
    const char         *val)
{
    chorda v = conllu_proprietas(x->feats, key);

    return v.mensura > 0 && chorda_aequalis_literis(v, val);
}

static int
has_feat (
    const ConlluLexema *x,
    const char         *key)
{
    return conllu_proprietas(x->feats, key).mensura > 0;
}

/* does a comma-separated feature value contain the token t (len tl)? */
static int
value_has (
    chorda      v,
    const char *t,
    int         tl)
{
    int i = 0;

    while (i < (int)v.mensura) {
        int j = i;

        while (j < (int)v.mensura && v.datum[j] != ',') j++;
        if (j - i == tl && memcmp(v.datum + i, t, (size_t)tl) == 0) return 1;
        i = j + 1;
    }
    return 0;
}

/* agreement by SET INTERSECTION of the values (multi-valued Gender) */
static int
feat_meet (
    const ConlluLexema *a,
    const ConlluLexema *b,
    const char         *key)
{
    chorda x = conllu_proprietas(a->feats, key);
    chorda y = conllu_proprietas(b->feats, key);
    int    i = 0;

    if (x.mensura == 0 || y.mensura == 0) return 0;
    while (i < (int)x.mensura) {
        int j = i;

        while (j < (int)x.mensura && x.datum[j] != ',') j++;
        if (value_has(y, (const char *)x.datum + i, j - i)) return 1;
        i = j + 1;
    }
    return 0;
}

static int
is_function_word (
    const ConlluLexema *x)
{
    static const char *const particles[8] = {
        "autem", "enim", "igitur", "vero", "quidem", "quoque", "tamen",
        "ergo"
    };
    int i;

    if (upos_is(x, "PART") || upos_is(x, "CCONJ") || upos_is(x, "ADV"))
        return 1;
    for (i = 0; i < 8; i++)
        if (chorda_aequalis_literis(x->lemma, particles[i])) return 1;
    return 0;
}

static Sent          sent;
static unsigned char nounmask[MAXN];
static int           idpos[MAXN];
static int           first_seen[2];

static void
analyse (
    Piscina                   *scratch,
    const DependentiaArbor    *t,
    const ConlluLexema *const *w,
    int                        bank,
    int                        bank7)
{
    DependentiaArcus *arcs;
    int               n = (int)t->numerus, g, i;
    Stats            *st = &stats[bank];
    int               lenbin = n <= 10 ? 0 : n <= 20 ? 1 : 2;

    if (n > MAXN) fatal("sentence longer than MAXN");
    sent.n = n;
    for (i = 0; i < n; i++) {
        sent.head[i] = (int)t->capita[i];
        idpos[i] = i;
        nounmask[i] = (unsigned char)(upos_is(w[i], "NOUN")
            || upos_is(w[i], "PROPN"));
    }
    sent_dominance(&sent);
    arcs = (DependentiaArcus *)piscina_allocare(scratch,
        (memoriae_index)n * sizeof(DependentiaArcus));
    if (!dependentia_arcus(scratch, t, arcs)) fatal("arcus");
    st->trees++;
    first_seen[0] = first_seen[1] = 0;
    for (g = 0; g < n; g++) {
        int h = sent.head[g] - 1, kind = -1, disp;
        int is_gen_full, is_adj_full;

        if (h < 0) continue;
        disp = arcs[g].non_proiectivus != 0;
        is_gen_full = deprel_base_is(w[g], "nmod")
            && feat_is(w[g], "Case", "Gen");
        is_adj_full = deprel_base_is(w[g], "amod") && upos_is(w[g], "ADJ")
            && has_feat(w[g], "Case") && has_feat(w[g], "Gender")
            && has_feat(w[g], "Number");
        if (is_gen_full) {
            chorda pt = conllu_proprietas(w[g]->feats, "PronType");
            int    rel = value_has(pt, "Rel", 3) || value_has(pt, "Int", 3);

            st->gen_all++;
            if (disp) {
                int m, k, l, e;
                st->d132_gen_disp++;
                null_share(&sent, g, nounmask, g > h ? g - h : h - g, 0,
                    &m, &k, &l, &e);
                st->d132_gen_k[m] += k;
            }
            if (rel) st->gen_rel++;
            else if (!nounmask[h]) st->gen_nonnoun++;
            else {
                if (value_has(pt, "Prs", 3)) st->gen_prs++;
                kind = 1;
            }
        } else if (deprel_base_is(w[g], "amod") && upos_is(w[g], "ADJ")) {
            st->adj_all++;
            if (!is_adj_full) {
                st->adj_incomplete++;
            } else {
                chorda gv = conllu_proprietas(w[g]->feats, "Gender");
                if (value_has(gv, "Fem", 3) + value_has(gv, "Masc", 4)
                    + value_has(gv, "Neut", 4) > 1)
                    st->adj_multigender++;
                if (disp) st->d132_adj_disp++;
                if (!nounmask[h]) st->adj_nonnoun++;
                else kind = 0;
            }
        }
        if (kind < 0) continue;
        if (kind == 0) st->adj_pop++;
        else st->gen_pop++;
        {
            int f = foreign_words(&sent, h, g);

            f_checked++;
            if (f != foreign_words_walk(&sent, h, g)) f_mismatch++;
            if (!disp) {
                if (f != 0) f_zero_nested_bad++;
                st->nest_n[kind]++;
                st->nest_dist[kind] += g > h ? g - h : h - g;
                continue;
            }
            if (f < 1) f_zero_nested_bad++;
        }
        /* a displaced arc of a registered population */
        {
            Item it;
            int  c, fw = 1, lo = g < h ? g : h, hi = g < h ? h : g, l, e;

            memset(&it, 0, sizeof it);
            it.kind = kind;
            it.bank = bank;
            it.bank7 = bank7;
            it.dep_first = g < h;
            it.lenbin = lenbin;
            it.f = foreign_words(&sent, h, g);
            it.dist = hi - lo;
            for (c = lo + 1; c < hi; c++)
                if (!sent.dom[h * n + c] && !is_function_word(w[c])) fw = 0;
            it.fw = fw;
            it.first = !first_seen[kind];
            first_seen[kind] = 1;
            if (kind == 0) {
                int other = 0;
                for (c = 0; c < n; c++)
                    if (nounmask[c] && c != h && !sent.dom[g * n + c]
                        && feat_meet(w[c], w[g], "Case")
                        && feat_meet(w[c], w[g], "Gender")
                        && feat_meet(w[c], w[g], "Number"))
                        other = 1;
                it.unique = !other;
                st->adj_disp++;
            } else {
                st->gen_disp++;
            }
            it.safe = alternatives(&sent, idpos, g, nounmask) == 0;
            null_share(&sent, g, nounmask, it.dist, it.dist, &it.mn, &it.kn,
                &l, &e);
            {
                int m2, k2;
                null_share(&sent, g, nounmask, -1, it.dist, &m2, &k2,
                    &it.pl, &it.pe);
                it.pm = m2;
            }
            push_item(&it);
        }
    }
    free(sent.dom);
    sent.dom = NULL;
}

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
        dir = getenv("D133_UD_NC");
        if (dir == NULL) dir = "oratio/build/ud";
    }
    if (strlen(dir) + strlen(f->file) + 2 > sizeof(path))
        fatal("path too long");
    sprintf(path, "%s/%s", dir, f->file);
    p = piscina_generare_dynamicum("d133_file", 1 << 22);
    scratch = piscina_generare_dynamicum("d133_tree", 1 << 20);
    if (p == NULL || scratch == NULL) fatal("pool");
    src = filum_legere_totum(path, p);
    if (src.datum == NULL) {
        printf("  %-28s ABSENT (%s)\n", f->file, f->nc
            ? "NC: set D133_UD_NC or ./oratio/oraculum.sh -petere"
            : "repo fixture missing");
        if (!f->nc) check("repo treebank present", 0);
        piscina_destruere(scratch);
        piscina_destruere(p);
        return;
    }
    sents = conllu_legere(p, src, &err);
    if (sents == NULL) fatal("conllu_legere");
    bank_present[bank] = 1;
    for (si_ = 0; si_ < xar_numerus(sents); si_++) {
        const ConlluSententia *s =
            (const ConlluSententia *)xar_obtinere(sents, si_);
        ConlluArbor  a;
        b32         *punct;
        i32          k, j;
        int          bank7 = bank;

        if (bank == BANK_PERSEUS) {
            int genre = perseus_genre(s->id);
            if (genre == 2) fatal("Perseus work code unknown");
            bank7 = genre == 0 ? BANK_PERSEUS : 6;
        }
        for (j = 0; j < xar_numerus(s->lexemata); j++)
            if (((const ConlluLexema *)xar_obtinere(s->lexemata, j))
                ->ranga)
                ranges++;
        piscina_vacare(scratch);
        if (!conllu_arbor(scratch, s, &a)
            || dependentia_validare(&a.arbor) != DEPENDENTIA_SANA) {
            unsound++;
            continue;
        }
        words += a.arbor.numerus;
        punct = (b32 *)piscina_allocare(scratch,
            (memoriae_index)a.arbor.numerus * sizeof(b32));
        for (k = 0; k < a.arbor.numerus; k++)
            punct[k] = chorda_aequalis_literis(a.verba[k]->upos, "PUNCT");
        {
            DependentiaArbor  e;
            i32               moved = 0;
            DependentiaOmissa r = dependentia_omittere(scratch, &a.arbor,
                punct, &e, &moved);

            if (r == DEPENDENTIA_OMISSA_RADIX) continue;
            if (r != DEPENDENTIA_OMISSA_FACTA) fatal("omittere");
            {
                const ConlluLexema **kw = (const ConlluLexema **)
                    piscina_allocare(scratch, (memoriae_index)e.numerus
                    * sizeof(const ConlluLexema *));
                i32 q = 0;

                for (k = 0; k < a.arbor.numerus; k++)
                    if (!punct[k]) kw[q++] = a.verba[k];
                if (q != e.numerus) fatal("kept words");
                analyse(scratch, &e, kw, bank, bank7);
            }
        }
    }
    printf("  %-28s %5u sentences  %6ld words  %4ld ranges  %ld not trees\n",
        f->file, (unsigned)xar_numerus(sents), words, ranges, unsound);
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

/* ================================================================
 * The exact stratified permutation test
 * ================================================================ */

static Piscina *fpool;             /* long-lived exact numbers */

typedef struct {
    Magnus *d;                     /* d[t] = count of label assignments
                                      giving sum t */
    int     len;                   /* entries 0..len-1 */
} Dist;

/* counts of k-subsets of vals[0..n-1] by their sum (values >= 0), into
 * pool keep; two working pools alternate so memory stays bounded */
static Dist
subset_counts (
    const int *vals,
    int        n,
    int        k,
    Piscina   *keep)
{
    int      maxs = 0, i, j, s, cols;
    Magnus  *ta, *tb, *tmp;
    Piscina *pa = piscina_generare_dynamicum("d133_dp_a", 1 << 20);
    Piscina *pb = piscina_generare_dynamicum("d133_dp_b", 1 << 20);
    Piscina *pt;
    Dist     out;

    for (i = 0; i < n; i++) maxs += vals[i];
    cols = maxs + 1;
    ta = (Magnus *)malloc((size_t)(k + 1) * (size_t)cols * sizeof(Magnus));
    tb = (Magnus *)malloc((size_t)(k + 1) * (size_t)cols * sizeof(Magnus));
    if (ta == NULL || tb == NULL || pa == NULL || pb == NULL) fatal("memory");
    for (j = 0; j <= k; j++)
        for (s = 0; s < cols; s++) ta[j * cols + s] = magnus_ex_s64(0);
    ta[0] = magnus_ex_s64(1);
    for (i = 0; i < n; i++) {
        int v = vals[i];

        for (j = 0; j <= k; j++)
            for (s = 0; s < cols; s++) {
                Magnus x = ta[j * cols + s];

                if (j > 0 && s >= v
                    && magnus_signum(ta[(j - 1) * cols + s - v]) != 0)
                    x = magnus_adde(x, ta[(j - 1) * cols + s - v], pb);
                else
                    x = magnus_transcribe(x, pb);
                tb[j * cols + s] = x;
            }
        piscina_vacare(pa);
        tmp = ta; ta = tb; tb = tmp;
        pt = pa; pa = pb; pb = pt;
    }
    out.len = cols;
    out.d = (Magnus *)piscina_allocare(keep,
        (memoriae_index)cols * sizeof(Magnus));
    for (s = 0; s < cols; s++)
        out.d[s] = magnus_transcribe(ta[k * cols + s], keep);
    free(ta);
    free(tb);
    piscina_destruere(pa);
    piscina_destruere(pb);
    return out;
}

static Dist
convolve (
    Dist     a,
    Dist     b,
    Piscina *keep)
{
    Dist out;
    int  i, j;

    out.len = a.len + b.len - 1;
    out.d = (Magnus *)piscina_allocare(keep,
        (memoriae_index)out.len * sizeof(Magnus));
    for (i = 0; i < out.len; i++) out.d[i] = magnus_ex_s64(0);
    for (i = 0; i < a.len; i++) {
        if (magnus_signum(a.d[i]) == 0) continue;
        for (j = 0; j < b.len; j++) {
            if (magnus_signum(b.d[j]) == 0) continue;
            out.d[i + j] = magnus_adde(out.d[i + j],
                magnus_multiplica(a.d[i], b.d[j], keep), keep);
        }
    }
    return out;
}

static Magnus
dist_total (
    Dist     d,
    Piscina *pool)
{
    Magnus t = magnus_ex_s64(0);
    int    i;

    for (i = 0; i < d.len; i++) t = magnus_adde(t, d.d[i], pool);
    return t;
}

/* P(sum >= t) as an exact fraction */
static Fractio
dist_upper (
    Dist     d,
    long     t,
    Piscina *pool)
{
    Magnus  num = magnus_ex_s64(0);
    Fractio f;
    int     i;

    for (i = 0; i < d.len; i++)
        if ((long)i >= t) num = magnus_adde(num, d.d[i], pool);
    if (!fractio_ex_magnis(num, dist_total(d, pool), pool, &f))
        fatal("fractio");
    return f;
}

/* the mean of the distribution, exact */
static Fractio
dist_mean (
    Dist     d,
    Piscina *pool)
{
    Magnus  num = magnus_ex_s64(0);
    Fractio f;
    int     i;

    for (i = 0; i < d.len; i++)
        num = magnus_adde(num, magnus_multiplica(d.d[i],
            magnus_ex_s64((s64)i), pool), pool);
    if (!fractio_ex_magnis(num, dist_total(d, pool), pool, &f))
        fatal("fractio");
    return f;
}

/* ---- a test over the items ---- */

enum { POP_BOTH, POP_ADJ };
enum { GRP_ADJ, GRP_UNIQUE };

typedef struct {
    int pop, grp;
    int by_dir;            /* direction in the strata */
    int dep_first_only;
    int exclude_llct;
    int merge_perseus;     /* Perseus as one stratum bank */
    int raw;               /* raw distance instead of F */
    int first_only;        /* the first displaced arc per sentence / kind */
    int no_fw;             /* drop function-word-flagged arcs */
    int force;             /* 0 auto, 1 the group's own sum, 2 the
                              complement's (a check: both agree) */
} Spec;

typedef struct {
    long    t_obs;
    Fractio p;             /* P(T >= t_obs) */
    Fractio e_perm;        /* E[T], from the strata */
    Fractio e_dist;        /* the distribution's mean (a check) */
    long    n_pop, n_grp;
    int     strata;
    long    tb_t[N_BANKS];
    Fractio tb_e[N_BANKS];
    Fractio tb_q[N_BANKS]; /* P(T_tb > E_tb) under the null */
    int     tb_has[N_BANKS];
} Result;

static int
in_pop (
    const Spec *sp,
    const Item *it)
{
    if (sp->pop == POP_ADJ && it->kind != 0) return 0;
    if (sp->dep_first_only && !it->dep_first) return 0;
    if (sp->exclude_llct && it->bank == BANK_LLCT) return 0;
    if (sp->first_only && !it->first) return 0;
    if (sp->no_fw && it->fw) return 0;
    return 1;
}

static int
in_grp (
    const Spec *sp,
    const Item *it)
{
    return sp->grp == GRP_ADJ ? it->kind == 0 : it->unique;
}

static int
value_of (
    const Spec *sp,
    const Item *it)
{
    int v = sp->raw ? it->dist : it->f;

    return v > FCAP ? FCAP : v;
}

static int
stratum_of (
    const Spec *sp,
    const Item *it)
{
    int b = sp->merge_perseus ? it->bank : it->bank7;

    return (b * 2 + (sp->by_dir ? it->dep_first : 0)) * 3 + it->lenbin;
}

#define N_STRATA (N_BANK7 * 2 * 3)

static Result
run_test (
    const Spec *sp,
    Piscina    *pool)
{
    static int vals[N_STRATA][4096];
    int        nv[N_STRATA], kx[N_STRATA], sumv[N_STRATA], bank_of[N_STRATA];
    long       t_obs = 0, tb_t[N_BANKS];
    int        s, b, use_comp, kk_total = 0, k_total = 0;
    long       i;
    Result     res;
    Dist       tb_dist[N_BANKS];
    int        tb_any[N_BANKS];

    memset(&res, 0, sizeof res);
    memset(nv, 0, sizeof nv);
    memset(kx, 0, sizeof kx);
    memset(sumv, 0, sizeof sumv);
    memset(tb_t, 0, sizeof tb_t);
    memset(tb_any, 0, sizeof tb_any);
    for (i = 0; i < n_items; i++) {
        const Item *it = &items[i];
        int         v;

        if (!in_pop(sp, it)) continue;
        s = stratum_of(sp, it);
        v = value_of(sp, it);
        if (nv[s] == 4096) fatal("stratum too large");
        vals[s][nv[s]++] = v;
        sumv[s] += v;
        bank_of[s] = it->bank;
        res.n_pop++;
        if (in_grp(sp, it)) {
            kx[s]++;
            t_obs += v;
            tb_t[it->bank] += v;
            res.n_grp++;
        }
    }
    /* work with the smaller group's sum (fewer, smaller sums) */
    for (s = 0; s < N_STRATA; s++) {
        k_total += kx[s];
        kk_total += nv[s] - kx[s];
    }
    use_comp = sp->force == 1 ? 0 : sp->force == 2 ? 1 : kk_total < k_total;
    res.t_obs = t_obs;
    res.e_perm = fractio_ex_s64(0);
    for (b = 0; b < N_BANKS; b++) {
        res.tb_e[b] = fractio_ex_s64(0);
        res.tb_t[b] = tb_t[b];
    }
    for (s = 0; s < N_STRATA; s++) {
        Fractio e;
        Dist    d;
        int     k;

        if (nv[s] == 0) continue;
        res.strata++;
        if (!fractio_ex_s64_s64((s64)kx[s] * (s64)sumv[s], (s64)nv[s], pool,
            &e))
            fatal("fractio");
        res.e_perm = fractio_adde(res.e_perm, e, pool);
        res.tb_e[bank_of[s]] = fractio_adde(res.tb_e[bank_of[s]], e, pool);
        k = use_comp ? nv[s] - kx[s] : kx[s];
        d = subset_counts(vals[s], nv[s], k, pool);
        if (!tb_any[bank_of[s]]) {
            tb_dist[bank_of[s]] = d;
            tb_any[bank_of[s]] = 1;
        } else {
            tb_dist[bank_of[s]] = convolve(tb_dist[bank_of[s]], d, pool);
        }
    }
    /* per treebank: P(T_tb > E_tb); then the pooled distribution */
    {
        Dist all;
        int  first = 1;
        long total_all = 0;

        for (b = 0; b < N_BANKS; b++) {
            long total_b = 0;
            int  sb;

            if (!tb_any[b]) continue;
            res.tb_has[b] = 1;
            for (sb = 0; sb < N_STRATA; sb++)
                if (nv[sb] > 0 && bank_of[sb] == b) total_b += sumv[sb];
            total_all += total_b;
            /* T > E  <=>  T >= floor(E) + 1; with the complement C =
             * total - T: C <= total - floor(E) - 1 */
            {
                Magnus fl = fractio_pavimentum(res.tb_e[b], pool);
                s64    fe;
                long   thr;

                if (!magnus_ad_s64(fl, &fe)) fatal("magnus");
                thr = (long)fe + 1;
                if (!use_comp) {
                    res.tb_q[b] = dist_upper(tb_dist[b], thr, pool);
                } else {
                    Fractio up = dist_upper(tb_dist[b], total_b - thr + 1,
                        pool);
                    res.tb_q[b] = fractio_subtrahe(fractio_ex_s64(1), up,
                        pool);
                }
            }
            all = first ? tb_dist[b] : convolve(all, tb_dist[b], pool);
            first = 0;
        }
        if (first) fatal("empty test");
        if (!use_comp) {
            res.p = dist_upper(all, t_obs, pool);
            res.e_dist = dist_mean(all, pool);
        } else {
            /* T >= t  <=>  C <= total - t */
            Fractio up = dist_upper(all, total_all - t_obs + 1, pool);
            res.p = fractio_subtrahe(fractio_ex_s64(1), up, pool);
            res.e_dist = fractio_subtrahe(fractio_ex_s64(total_all),
                dist_mean(all, pool), pool);
        }
    }
    return res;
}

/* ================================================================
 * Part A
 * ================================================================ */

/* the design review's hand sentences (plan.md "Hand walk-through") */
typedef struct {
    const char *title;
    int         n, g;              /* g 1-based */
    int         head[6];
    int         noun[6];
    int         agree[6];          /* agreeing hosts (adjectives) */
    int         dist, f, dep_first, disp, safe, safe_f;
    int         k, m, kn, mn;      /* null shares: registered, limited */
} HandTree;

static void
hand_trees (void)
{
    static const HandTree h[12] = {
        { "A1 magnam puer in silva vidit ursam", 6, 1, { 6, 5, 4, 5, 0, 5 },
          { 0, 1, 0, 1, 0, 1 }, { 0, 0, 0, 0, 0, 1 }, 5, 4, 1, 1, 0, 1,
          0, 4, 0, 4 },
        { "A2 magnam habet domum", 3, 1, { 3, 0, 2, 0, 0, 0 },
          { 0, 0, 1, 0, 0, 0 }, { 0, 0, 1, 0, 0, 0 }, 2, 1, 1, 1, 1, 1,
          1, 1, 1, 1 },
        { "A3 magna autem pars militum fugit", 5, 1, { 3, 5, 5, 3, 0, 0 },
          { 0, 0, 1, 1, 0, 0 }, { 0, 0, 1, 0, 0, 0 }, 2, 1, 1, 1, 1, 1,
          2, 2, 1, 1 },
        { "A4 reginam amat puella bonam", 4, 4, { 2, 0, 2, 1, 0, 0 },
          { 1, 0, 1, 0, 0, 0 }, { 1, 0, 0, 0, 0, 0 }, 3, 2, 0, 1, 0, 1,
          0, 2, 0, 2 },
        { "A5 bonam reginam amat puellam", 4, 1, { 4, 3, 0, 3, 0, 0 },
          { 0, 1, 0, 1, 0, 0 }, { 0, 1, 0, 1, 0, 0 }, 3, 2, 1, 1, 0, 0,
          0, 2, 0, 2 },
        { "A6 magnam parvus amat puer urbem (outer)", 5, 1,
          { 5, 4, 0, 3, 3, 0 }, { 0, 0, 0, 1, 1, 0 }, { 0, 0, 0, 0, 1, 0 },
          4, 3, 1, 1, 1, 1, 2, 3, 2, 3 },
        { "A6 magnam parvus amat puer urbem (inner)", 5, 2,
          { 5, 4, 0, 3, 3, 0 }, { 0, 0, 0, 1, 1, 0 }, { 0, 0, 0, 1, 0, 0 },
          2, 1, 1, 1, 1, 1, 2, 3, 1, 2 },
        { "G1 regis vidi filiam in urbe", 5, 1, { 3, 0, 2, 5, 2, 0 },
          { 1, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 0, 0 }, 2, 1, 1, 1, 1, 1,
          1, 3, 1, 2 },
        { "G2 rex cuius heri filiam vidi venit", 6, 2, { 6, 4, 5, 5, 1, 0 },
          { 1, 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, 2, 1, 1, 1, 0, 1,
          1, 4, 0, 2 },
        { "G3 Caesaris in castra venerunt legati", 5, 1,
          { 5, 3, 4, 0, 4, 0 }, { 1, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 0, 0 },
          4, 3, 1, 1, 0, 1, 0, 3, 0, 3 },
        { "G4 regis magni amat filiam", 4, 1, { 4, 1, 0, 3, 0, 0 },
          { 1, 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, 3, 1, 1, 1, 1, 1,
          2, 2, 2, 2 },
        { "G5 filiam et filium regis vidi: not displaced", 5, 4,
          { 5, 3, 1, 1, 0, 0 }, { 1, 0, 1, 1, 0, 0 }, { 0, 0, 0, 0, 0, 0 },
          3, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };
    int  t, i;
    char msg[400];

    printf("\n--- the design review's hand sentences ---\n");
    for (t = 0; t < 12; t++) {
        const HandTree *x = &h[t];
        unsigned char   noun[6], agree[6];
        int             g = x->g - 1, hd, dist, f, disp, safe = 0, safe_f = 0;
        int             k = 0, m = 0, kn = 0, mn = 0, l, e;

        sent.n = x->n;
        for (i = 0; i < x->n; i++) {
            sent.head[i] = x->head[i];
            idpos[i] = i;
            noun[i] = (unsigned char)x->noun[i];
            agree[i] = (unsigned char)x->agree[i];
        }
        sent_dominance(&sent);
        hd = sent.head[g] - 1;
        dist = g > hd ? g - hd : hd - g;
        f = foreign_words(&sent, hd, g);
        disp = arc_displaced(&sent, idpos, hd, g, 0);
        if (disp) {
            safe = alternatives(&sent, idpos, g, noun) == 0;
            safe_f = alternatives(&sent, idpos, g, agree) == 0;
            null_share(&sent, g, noun, -1, dist, &m, &k, &l, &e);
            null_share(&sent, g, noun, dist, dist, &mn, &kn, &l, &e);
        }
        free(sent.dom);
        sent.dom = NULL;
        snprintf(msg, sizeof msg, "%s: distance %d, F %d, %s, %s; every "
            "noun %s, agreeing %s; null %d/%d, limited %d/%d", x->title,
            dist, f, g < hd ? "dependent-first" : "head-first",
            disp ? "displaced" : "nested", safe ? "SAFE" : "unsafe",
            safe_f ? "SAFE" : "unsafe", k, m, kn, mn);
        check(msg, dist == x->dist && f == x->f
            && (g < hd) == x->dep_first && disp == x->disp
            && safe == x->safe && safe_f == x->safe_f && k == x->k
            && m == x->m && kn == x->kn && mn == x->mn);
    }
}

/* the exact test against brute force on small constructed strata */
static void
brute_force_check (void)
{
    static const int sa[5] = { 1, 3, 2, 5, 4 };
    static const int sb[3] = { 2, 2, 7 };
    static const int sc[4] = { 10, 1, 1, 3 };
    long             count[40];
    int              ma, mb, mc, ok = 1, t;
    Piscina         *pool = piscina_generare_dynamicum("d133_brute", 1 << 16);
    Dist             da, db, dc, all;

    memset(count, 0, sizeof count);
    /* choose 2 of sa, 1 of sb, 3 of sc: every combination */
    for (ma = 0; ma < 32; ma++) {
        int pa = 0, va = 0, i;
        for (i = 0; i < 5; i++) if (ma & (1 << i)) { pa++; va += sa[i]; }
        if (pa != 2) continue;
        for (mb = 0; mb < 8; mb++) {
            int pb = 0, vb = 0;
            for (i = 0; i < 3; i++) if (mb & (1 << i)) { pb++; vb += sb[i]; }
            if (pb != 1) continue;
            for (mc = 0; mc < 16; mc++) {
                int pc = 0, vc = 0;
                for (i = 0; i < 4; i++)
                    if (mc & (1 << i)) { pc++; vc += sc[i]; }
                if (pc != 3) continue;
                count[va + vb + vc]++;
            }
        }
    }
    da = subset_counts(sa, 5, 2, pool);
    db = subset_counts(sb, 3, 1, pool);
    dc = subset_counts(sc, 4, 3, pool);
    all = convolve(convolve(da, db, pool), dc, pool);
    for (t = 0; t < 40; t++) {
        s64 v = 0;
        if (t < all.len && !magnus_ad_s64(all.d[t], &v)) ok = 0;
        if (v != (s64)count[t]) ok = 0;
    }
    check("the exact test's distribution == brute force over every "
        "combination of three small strata (10 x 3 x 4 = 120 assignments)",
        ok);
    piscina_destruere(pool);
}

static const long d132_gen_disp[N_BANKS] = { 80, 5, 44, 42, 71, 31 };
static const long d132_adj_disp[N_BANKS] = { 445, 68, 28, 178, 78, 34 };
static const char *const d132_gen_near[N_BANKS] = {
    "28687/840", "31/21", "107/4", "10187/420", "37271/840", "85/4"
};

static Fractio
expected_k (
    const long *k)
{
    Fractio e = fractio_ex_s64(0), t;
    int     m;

    for (m = 1; m <= MAXN; m++) {
        if (k[m] == 0) continue;
        if (!fractio_ex_s64_s64((s64)k[m], (s64)m, fpool, &t)) fatal("fr");
        e = fractio_adde(e, t, fpool);
    }
    return e;
}

static void
part_a (void)
{
    int  fi, b, ok_disp = 1, ok_near = 1, present = 0;
    char msg[300];

    printf("\n--- Part A: the treebanks (headline: without PUNCT) ---\n");
    for (fi = 0; fi < N_FILES; fi++) {
        if (quick && files[fi].nc) continue;
        read_file(fi);
    }
    for (b = 0; b < N_BANKS; b++) {
        Fractio f;

        if (!bank_present[b]) continue;
        present++;
        if (stats[b].d132_gen_disp != d132_gen_disp[b]
            || stats[b].d132_adj_disp != d132_adj_disp[b])
            ok_disp = 0;
        if (!fractio_ex_chorda(chorda_ex_literis(d132_gen_near[b], fpool),
            fpool, &f))
            fatal("pin");
        if (!fractio_aequalis(expected_k(stats[b].d132_gen_k), f))
            ok_near = 0;
    }
    snprintf(msg, sizeof msg, "D132's full populations reproduced: "
        "displaced genitives and adjectives per treebank (%d treebanks)",
        present);
    check(msg, ok_disp && present > 0);
    check("D132's distance-limited E for genitives reproduced as exact "
        "fractions (the L2 thresholds' source)", ok_near && present > 0);
    snprintf(msg, sizeof msg, "F by the dominance matrix == F by ancestor "
        "walks on every registered arc (%ld); F = 0 exactly when nested",
        f_checked);
    check(msg, f_mismatch == 0 && f_zero_nested_bad == 0 && f_checked > 0);
    hand_trees();
    brute_force_check();
}

/* ================================================================
 * Part B
 * ================================================================ */

static s64
scaled (
    Fractio x,
    s64     num,
    s64     den)
{
    Fractio f;
    s64     v;

    if (!fractio_ex_s64_s64(num, den, fpool, &f)) fatal("fractio");
    if (!magnus_ad_s64(fractio_rotunda(fractio_multiplica(x, f, fpool),
        fpool), &v))
        fatal("magnus");
    return v;
}

/* registered rules */
static int
p_rule (
    Fractio p)
{
    Fractio a;

    if (!fractio_ex_s64_s64(1, 20, fpool, &a)) fatal("fractio");
    return fractio_compara(p, a, fpool) <= 0;
}

static int
l1_rule (
    int pooled_ok,
    int banks_above)
{
    return pooled_ok && banks_above >= 4;
}

static void
print_result (
    const char   *title,
    const Result *r)
{
    s64 p6 = scaled(r->p, 1000000, 1);
    s64 t2 = scaled(fractio_ex_s64((s64)r->t_obs), 100, 1);
    s64 e2 = scaled(r->e_perm, 100, 1);

    printf("  %-44s N %4ld (group %4ld), %2d strata: T %lld.%02lld, "
        "E %lld.%02lld, p %lld.%06lld\n", title, r->n_pop, r->n_grp,
        r->strata, (long long)(t2 / 100), (long long)(t2 % 100),
        (long long)(e2 / 100), (long long)(e2 % 100),
        (long long)(p6 / 1000000), (long long)(p6 % 1000000));
}

static Result res_l1, res_l1u, res_l3;

static void
part_b (void)
{
    Spec   sp;
    int    b, above = 0;
    char   s1[32], s2[32];
    Result r;

    printf("\n--- Part B: populations ---\n");
    printf("  %-8s %6s %5s %5s %6s %6s %6s | %6s %6s %6s %6s %6s %6s\n",
        "", "gen", "rel", "prs", "noun-", "pop", "displ", "adj", "incompl",
        "multiG", "noun-", "pop", "displ");
    for (b = 0; b < N_BANKS; b++) {
        const Stats *s = &stats[b];

        if (!bank_present[b]) continue;
        printf("  %-8s %6ld %5ld %5ld %6ld %6ld %6ld | %6ld %6ld %6ld %6ld "
            "%6ld %6ld\n", bank_names[b], s->gen_all, s->gen_rel, s->gen_prs,
            s->gen_nonnoun, s->gen_pop, s->gen_disp, s->adj_all,
            s->adj_incomplete, s->adj_multigender, s->adj_nonnoun,
            s->adj_pop, s->adj_disp);
    }
    printf("  (rel: relative / interrogative genitives, excluded; prs: "
        "personal-pronoun genitives, kept;\n   noun-: head not NOUN or "
        "PROPN, excluded; incompl: adjectives lacking Case, Gender or "
        "Number)\n");

    printf("\n--- the registered tests ---\n");
    memset(&sp, 0, sizeof sp);
    sp.pop = POP_BOTH; sp.grp = GRP_ADJ; sp.by_dir = 1;
    res_l1 = run_test(&sp, fpool);
    print_result("L1: adjectives vs genitives, F", &res_l1);
    printf("     per treebank (T vs E_perm; P(T > E) under the null):\n");
    for (b = 0; b < N_BANKS; b++) {
        s64 t2, e2, q6;

        if (!res_l1.tb_has[b]) continue;
        t2 = scaled(fractio_ex_s64((s64)res_l1.tb_t[b]), 100, 1);
        e2 = scaled(res_l1.tb_e[b], 100, 1);
        q6 = scaled(res_l1.tb_q[b], 1000000, 1);
        printf("     %-8s T %lld.%02lld  E %lld.%02lld  %s  P(T > E) "
            "%lld.%06lld%s\n", bank_names[b], (long long)(t2 / 100),
            (long long)(t2 % 100), (long long)(e2 / 100),
            (long long)(e2 % 100),
            fractio_compara(fractio_ex_s64((s64)res_l1.tb_t[b]),
            res_l1.tb_e[b], fpool) > 0 ? "T > E" : "T <= E",
            (long long)(q6 / 1000000), (long long)(q6 % 1000000),
            b == BANK_LLCT ? "  (not counted)" : "");
        if (b != BANK_LLCT && fractio_compara(fractio_ex_s64(
            (s64)res_l1.tb_t[b]), res_l1.tb_e[b], fpool) > 0)
            above++;
    }
    /* the clause's chance: P(at least four of the five) */
    {
        Fractio dist[6], one = fractio_ex_s64(1), chance;
        int     j, nb = 0;
        s64     c6;

        dist[0] = one;
        for (j = 1; j < 6; j++) dist[j] = fractio_ex_s64(0);
        for (b = 0; b < N_BANKS; b++) {
            Fractio q, nq;
            if (b == BANK_LLCT || !res_l1.tb_has[b]) continue;
            q = res_l1.tb_q[b];
            nq = fractio_subtrahe(one, q, fpool);
            nb++;
            for (j = nb; j >= 1; j--)
                dist[j] = fractio_adde(fractio_multiplica(dist[j], nq,
                    fpool), fractio_multiplica(dist[j - 1], q, fpool),
                    fpool);
            dist[0] = fractio_multiplica(dist[0], nq, fpool);
        }
        chance = fractio_ex_s64(0);
        for (j = 4; j <= nb; j++) chance = fractio_adde(chance, dist[j],
            fpool);
        c6 = scaled(chance, 1000000, 1);
        printf("     clause (b) chance under the null, P(>= 4 of %d): "
            "%lld.%06lld\n", nb, (long long)(c6 / 1000000),
            (long long)(c6 % 1000000));
    }
    memset(&sp, 0, sizeof sp);
    sp.pop = POP_BOTH; sp.grp = GRP_ADJ; sp.dep_first_only = 1;
    res_l1u = run_test(&sp, fpool);
    print_result("L1u: dependent-first only", &res_l1u);
    memset(&sp, 0, sizeof sp);
    sp.pop = POP_ADJ; sp.grp = GRP_UNIQUE; sp.exclude_llct = 1;
    res_l3 = run_test(&sp, fpool);
    print_result("L3: unique vs shared label (adjectives, no LLCT)", &res_l3);

    check("E_perm (from the strata) == the exact distribution's mean, for "
        "L1, L1u and L3", fractio_aequalis(res_l1.e_perm, res_l1.e_dist)
        && fractio_aequalis(res_l1u.e_perm, res_l1u.e_dist)
        && fractio_aequalis(res_l3.e_perm, res_l3.e_dist));
    {
        Result a1, a2, b1, b2;

        memset(&sp, 0, sizeof sp);
        sp.pop = POP_BOTH; sp.grp = GRP_ADJ; sp.by_dir = 1;
        sp.force = 1; a1 = run_test(&sp, fpool);
        sp.force = 2; a2 = run_test(&sp, fpool);
        memset(&sp, 0, sizeof sp);
        sp.pop = POP_ADJ; sp.grp = GRP_UNIQUE; sp.exclude_llct = 1;
        sp.force = 1; b1 = run_test(&sp, fpool);
        sp.force = 2; b2 = run_test(&sp, fpool);
        check("the group's own sum and the complement's give the same exact "
            "p and mean (L1 and L3, both ways)",
            fractio_aequalis(a1.p, a2.p) && fractio_aequalis(a1.p, res_l1.p)
            && fractio_aequalis(a1.e_dist, a2.e_dist)
            && fractio_aequalis(b1.p, b2.p) && fractio_aequalis(b1.p, res_l3.p)
            && fractio_aequalis(b1.e_dist, b2.e_dist));
    }

    printf("\n--- sensitivities (reported, not decided) ---\n");
    {
        static const char *const names[4] = {
            "one arc per sentence per kind", "function-word arcs removed",
            "raw distance instead of F", "Perseus as one stratum"
        };
        int v;

        for (v = 0; v < 4; v++) {
            int which;
            for (which = 0; which < 2; which++) {
                char title[120];
                memset(&sp, 0, sizeof sp);
                if (which == 0) {
                    sp.pop = POP_BOTH; sp.grp = GRP_ADJ; sp.by_dir = 1;
                } else {
                    sp.pop = POP_ADJ; sp.grp = GRP_UNIQUE;
                    sp.exclude_llct = 1;
                }
                if (v == 0) sp.first_only = 1;
                if (v == 1) sp.no_fw = 1;
                if (v == 2) sp.raw = 1;
                if (v == 3) sp.merge_perseus = 1;
                r = run_test(&sp, fpool);
                snprintf(title, sizeof title, "%s, %s", which == 0 ? "L1"
                    : "L3", names[v]);
                print_result(title, &r);
            }
        }
        {
            long i, nhf = 0;
            for (i = 0; i < n_items; i++) nhf += !items[i].dep_first;
            printf("  head-first displaced arcs (reported only): %ld\n", nhf);
        }
    }

    printf("\n--- descriptive: the excess beyond distance (L2), every-noun "
        "hosts ---\n");
    printf("  %-8s %-10s %5s %8s %10s %8s %10s\n", "", "kind", "N", "S/N",
        "E_near/N", "points", "odds ratio");
    for (b = 0; b < N_BANKS; b++) {
        int kind;

        if (!bank_present[b]) continue;
        for (kind = 0; kind < 2; kind++) {
            long    nn = 0, ss = 0, i;
            Fractio e = fractio_ex_s64(0), t;
            s64     e4, pts, orx;

            for (i = 0; i < n_items; i++) {
                const Item *it = &items[i];
                if (it->bank != b || it->kind != kind) continue;
                nn++;
                ss += it->safe;
                if (!fractio_ex_s64_s64((s64)it->kn, (s64)it->mn, fpool, &t))
                    fatal("fractio");
                e = fractio_adde(e, t, fpool);
            }
            if (nn == 0) continue;
            pct(s1, 32, ss, nn);
            e4 = scaled(e, 10000, (s64)nn);
            pts = (s64)((ss * 20000 + nn) / (2 * nn)) - e4;
            /* odds ratio S/(N-S) : E/(N-E), exact, shown rounded */
            if (ss == nn || ss == 0
                || fractio_compara(e, fractio_ex_s64((s64)nn), fpool) == 0
                || fractio_signum(e) == 0) {
                snprintf(s2, sizeof s2, "-");
            } else {
                Fractio ne = fractio_subtrahe(fractio_ex_s64((s64)nn), e,
                    fpool), q, ratio;
                if (!fractio_divide(e, ne, fpool, &q)) fatal("fractio");
                if (!fractio_ex_s64_s64((s64)ss, (s64)(nn - ss), fpool,
                    &ratio))
                    fatal("fractio");
                if (!fractio_divide(ratio, q, fpool, &ratio)) fatal("fr");
                orx = scaled(ratio, 100, 1);
                snprintf(s2, sizeof s2, "%lld.%02lld", (long long)(orx / 100),
                    (long long)(orx % 100));
            }
            printf("  %-8s %-10s %5ld %8s %7lld.%02lld%% %+5lld.%02lld %10s\n",
                bank_names[b], kind == 0 ? "adjective" : "genitive", nn, s1,
                (long long)(e4 / 100), (long long)(e4 % 100),
                (long long)(pts / 100), (long long)(pts < 0 ? -pts % 100
                : pts % 100), s2);
        }
    }

    printf("\n--- descriptive: baselines and the shape-free operand ---\n");
    for (b = 0; b < N_BANKS; b++) {
        const Stats *s = &stats[b];
        int          kind;

        if (!bank_present[b]) continue;
        printf("  %-8s nested arcs: mean raw distance", bank_names[b]);
        for (kind = 0; kind < 2; kind++) {
            Fractio f;
            s64     v;
            if (s->nest_n[kind] == 0) {
                printf("  %s -", kind == 0 ? "adj" : "gen");
                continue;
            }
            if (!fractio_ex_s64_s64((s64)s->nest_dist[kind],
                (s64)s->nest_n[kind], fpool, &f))
                fatal("fractio");
            v = scaled(f, 100, 1);
            printf("  %s %lld.%02lld", kind == 0 ? "adj" : "gen",
                (long long)(v / 100), (long long)(v % 100));
        }
        /* mean mid-rank percentile of the observed distance among the
         * move-alone null's displaced placements: adjectives unique,
         * shared; genitives */
        {
            int grp;
            static const char *const gname[3] = { "adj unique", "adj shared",
                "gen" };

            printf(";  mean percentile");
            for (grp = 0; grp < 3; grp++) {
                Fractio sum = fractio_ex_s64(0), t;
                long    cnt = 0, i;
                s64     v;

                for (i = 0; i < n_items; i++) {
                    const Item *it = &items[i];
                    int match;
                    if (it->bank != b) continue;
                    match = grp == 2 ? it->kind == 1 : it->kind == 0
                        && it->unique == (grp == 0);
                    if (!match) continue;
                    if (!fractio_ex_s64_s64((s64)(2 * it->pl + it->pe),
                        (s64)(2 * it->pm), fpool, &t))
                        fatal("fractio");
                    sum = fractio_adde(sum, t, fpool);
                    cnt++;
                }
                if (cnt == 0) {
                    printf("  %s -", gname[grp]);
                    continue;
                }
                v = scaled(sum, 100, (s64)cnt);
                printf("  %s %lld.%02lld", gname[grp], (long long)(v / 100),
                    (long long)(v % 100));
            }
        }
        printf("\n");
    }

    printf("\n--- the registered rules on constructed values ---\n");
    {
        Fractio a, b2, c;

        (void)fractio_ex_s64_s64(1, 20, fpool, &a);
        (void)fractio_ex_s64_s64(51, 1000, fpool, &b2);
        (void)fractio_ex_s64_s64(49, 1000, fpool, &c);
        check("p <= 1/20 at its threshold: 1/20 holds, 51/1000 fails, "
            "49/1000 holds", p_rule(a) && !p_rule(b2) && p_rule(c));
        check("L1: pooled and four of five hold; three of five fails; "
            "pooled failing fails", l1_rule(1, 4) && l1_rule(1, 5)
            && !l1_rule(1, 3) && !l1_rule(0, 5));
    }

    printf("\n--- verdicts (registered) ---\n");
    {
        int present = 0;

        for (b = 0; b < N_BANKS; b++) present += bank_present[b];
        if (present < N_BANKS || quick) {
            printf("  only %d of 6 treebanks present%s: the verdicts are "
                "not decided\n", present, quick ? " (quick mode)" : "");
            return;
        }
    }
    printf("  L1: pooled p <= 1/20: %s; T > E in %d of the 5 non-LLCT "
        "treebanks -> L1 %s\n", p_rule(res_l1.p) ? "yes" : "NO", above,
        l1_rule(p_rule(res_l1.p), above) ? "HOLDS" : "FAILS");
    printf("  L1u: pooled p <= 1/20 -> L1u %s\n",
        p_rule(res_l1u.p) ? "HOLDS" : "FAILS");
    printf("  L3: pooled p <= 1/20 -> L3 %s\n",
        p_rule(res_l3.p) ? "HOLDS" : "FAILS");
}

int
main (void)
{
    quick = getenv("D133_CELER") != NULL;
    printf("KNOTAPEL DEMO 133: labels buy distance%s\n",
        quick ? " (QUICK MODE: repo treebanks only)" : "");
    fpool = piscina_generare_dynamicum("d133_fractio", 1 << 22);
    if (fpool == NULL) fatal("pool");
    part_a();
    part_b();
    piscina_destruere(fpool);
    printf("\n=== %d pass, %d fail ===\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
