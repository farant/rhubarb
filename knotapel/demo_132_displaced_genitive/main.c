/*
 * KNOTAPEL DEMO 132: Real Latin and the Displaced Genitive (N15)
 * ==============================================================
 *
 * D131 (KC 58) found, in a toy world, that endings cannot place a
 * displaced GENITIVE: its ending names a role, not a head, so any noun
 * could host it. Real Latin displaces genitives anyway (D130's Part C:
 * nmod over-represented among non-projective arcs in five of six Latin
 * treebanks). Hypothesis (plan.md, approved 2026-10-19): writers
 * displace a genitive where the word order itself leaves no NESTED
 * alternative - no other noun could take it with a projective arc - so
 * a nesting-first reader cannot be misled.
 *
 *   Part A  (T1) the trees (D130's reader, headline without PUNCT), the
 *           genitive arcs, the plan's hand walk-through as hand trees,
 *           the fast arc test against dependentia_arcus on every arc,
 *           the null containing the observation, reversal, cycles.
 *   Part B  (T1) per treebank: displaced genitives N, safe S, the
 *           positional null's expected safe count E (exact, fractio);
 *           registered G1 (S > E in >= 5 of 6) and G2 (S / N >= 1/2 in
 *           >= 5 of 6); reports: noun-headed subset, Perseus prose and
 *           verse, the unsafe ones' alternatives, the adjective
 *           contrast (agreement-filtered and unfiltered).
 *
 * Definitions (plan.md): a genitive arc is nmod (any subtype) with
 * Case=Gen on the dependent; displaced = per-arc non-projective; a
 * candidate host is a NOUN or PROPN other than the true head and not in
 * the genitive's subtree; a nested alternative is a candidate whose arc
 * to the genitive would be projective with the genitive re-hung under
 * it; SAFE = displaced with no nested alternative. The null moves the
 * genitive ALONE through every position of its sentence (dependents
 * stay attached); among the placements where its true arc is still
 * displaced, the share that are safe. E = the sum of those shares.
 *
 * Data: D130's six Latin treebanks. In the repo (CC BY-SA 4.0):
 * CIRCSE test, LLCT dev + test. NC (PROIEL, Perseus, ITTB, UDante test)
 * read from $D132_UD_NC (default oratio/build/ud); AGGREGATES ONLY,
 * never a sentence (Fran's decision for D130, kept).
 *
 * House libraries: conllu, dependentia, fractio (and their closure),
 * hence latina.h: Roman numerals and Latin keywords are macros here
 * (never name a variable I, V, X, L, C, D or M, or per, si, dum, ...).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_132_displaced_genitive/main.c &&
 *   bash build/aedilis/main/struere.sh &&
 *   D132_UD_NC=<dir with the NC .conllu files> ./build/aedilis/main/main
 * Quick mode for plants: D132_CELER=1 (repo treebanks only; verdicts
 * not decided).
 */

#include "conllu.h"
#include "dependentia.h"
#include "fractio.h"
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

/* num / den as a percentage, two decimals, integer arithmetic */
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
#define N_ROWS  8          /* the six banks, then Perseus prose, verse */
#define ROW_PERSEUS_PROSE 6
#define ROW_PERSEUS_VERSE 7
#define MAXN    1024       /* longest sentence handled */

typedef struct {
    const char *file;
    int         nc;          /* NC: read from $D132_UD_NC */
    int         bank;
    long        sentences;   /* FONTES.md (repo only; -1 = not pinned) */
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

static const char *const row_names[N_ROWS] = {
    "CIRCSE", "LLCT", "PROIEL", "Perseus", "ITTB", "UDante",
    "Perseus prose", "Perseus verse"
};

static int bank_present[N_BANKS];

/* Perseus by the work code that starts each sent_id (as D130) */
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
 * Sentences as plain arrays: heads, dominance, the fast arc test
 * ================================================================ */

typedef struct {
    int            n;
    int            head[MAXN];     /* head[w] = 1-based head of word w
                                      (0-based), 0 = root */
    unsigned char *dom;            /* dom[a * n + b]: a is b or an
                                      ancestor of b */
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

/* Is the arc from c to g non-projective, with words at positions
 * pos[w]? rehung: g (with its subtree) re-hung under c, so g's subtree
 * counts as dominated by c. */
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

/* nested alternatives for g among hosts (host[w] != 0), at pos[] */
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

/* g moved through every position of base[] (the words in order):
 * *m = placements where g's true arc is displaced, *k = of those, safe.
 * *at_own_disp and *at_own_safe: the placement at g's own index in
 * base[] (which reproduces base[]'s order) */
static void
null_share (
    const Sent          *s,
    const int           *base,
    int                  g,
    const unsigned char *host,
    int                 *m,
    int                 *k,
    int                 *at_own_disp,
    int                 *at_own_safe)
{
    static int others[MAXN], pos[MAXN];
    int        no = 0, own = -1, p, i, h = s->head[g] - 1;

    for (i = 0; i < s->n; i++) {
        if (base[i] == g) {
            own = i;
            continue;
        }
        others[no++] = base[i];
    }
    *m = *k = 0;
    *at_own_disp = *at_own_safe = 0;
    for (p = 0; p < s->n; p++) {
        int q = 0, d, sf;

        for (i = 0; i < s->n; i++) {
            if (i == p) {
                pos[g] = i;
                continue;
            }
            pos[others[q++]] = i;
        }
        d = arc_displaced(s, pos, h, g, 0);
        sf = d && alternatives(s, pos, g, host) == 0;
        if (d) {
            (*m)++;
            *k += sf;
        }
        if (p == own) {
            *at_own_disp = d;
            *at_own_safe = sf;
        }
    }
}

/* ================================================================
 * Statistics per row
 * ================================================================ */

typedef struct {
    long sentences, unsound, aside_root, trees;
    long gen_arcs, gen_noun_head, nested;
    long n_disp, s_safe;
    long alt_hist[4];                /* unsafe: 1, 2, 3, 4+ alternatives */
    long k_null[MAXN + 1];           /* by m: sum of safe placements */
    long nh_disp, nh_safe, nh_k[MAXN + 1];   /* noun-headed subset */
    long adj_arcs, a_disp, a_safe, a_k[MAXN + 1];      /* filtered */
    long au_safe, au_k[MAXN + 1];                      /* unfiltered */
    long a_degenerate;    /* filtered: no other agreeing noun at all */
    int  pk[64], pm[64], np;   /* this sentence's displaced genitives:
                                  null share pk / pm each */
} Stats;

static Stats stats[N_ROWS];

/* per row, every displaced genitive's null share (for the tail) */
static int row_pk[N_ROWS][MAXN], row_pm[N_ROWS][MAXN], row_np[N_ROWS];

/* per-run checks */
static long arcs_checked = 0, arcs_mismatch = 0;
static long own_checked = 0, own_mismatch = 0;
static long rev_checked = 0, rev_mismatch = 0;

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
feat_equal (
    const ConlluLexema *a,
    const ConlluLexema *b,
    const char         *key)
{
    chorda x = conllu_proprietas(a->feats, key);
    chorda y = conllu_proprietas(b->feats, key);

    return x.mensura > 0 && y.mensura > 0 && chorda_aequalis(x, y);
}

static int
has_feat (
    const ConlluLexema *x,
    const char         *key)
{
    return conllu_proprietas(x->feats, key).mensura > 0;
}

static void
add_row (
    int          r,
    const Stats *t)
{
    Stats *s = &stats[r];
    int    i;

    s->sentences += t->sentences;
    s->unsound += t->unsound;
    s->aside_root += t->aside_root;
    s->trees += t->trees;
    s->gen_arcs += t->gen_arcs;
    s->gen_noun_head += t->gen_noun_head;
    s->nested += t->nested;
    s->n_disp += t->n_disp;
    s->s_safe += t->s_safe;
    for (i = 0; i < 4; i++) s->alt_hist[i] += t->alt_hist[i];
    s->nh_disp += t->nh_disp;
    s->nh_safe += t->nh_safe;
    s->adj_arcs += t->adj_arcs;
    s->a_disp += t->a_disp;
    s->a_safe += t->a_safe;
    s->au_safe += t->au_safe;
    s->a_degenerate += t->a_degenerate;
    for (i = 0; i < t->np; i++) {
        if (row_np[r] == MAXN) fatal("too many displaced genitives");
        row_pk[r][row_np[r]] = t->pk[i];
        row_pm[r][row_np[r]] = t->pm[i];
        row_np[r]++;
    }
    for (i = 0; i <= MAXN; i++) {
        s->k_null[i] += t->k_null[i];
        s->nh_k[i] += t->nh_k[i];
        s->a_k[i] += t->a_k[i];
        s->au_k[i] += t->au_k[i];
    }
}

static Stats  tstat;              /* one tree's contribution */
static Sent   sent;
static int    base_fwd[MAXN], base_rev[MAXN], idpos[MAXN];
static unsigned char nounmask[MAXN], agreemask[MAXN];

/* one sound headline tree */
static void
analyse (
    Piscina                   *scratch,
    const DependentiaArbor    *t,
    const ConlluLexema *const *w)
{
    DependentiaArcus *arcs;
    int               n = (int)t->numerus, g, i;

    if (n > MAXN) fatal("sentence longer than MAXN");
    sent.n = n;
    for (i = 0; i < n; i++) {
        sent.head[i] = (int)t->capita[i];
        base_fwd[i] = i;
        base_rev[i] = n - 1 - i;
        idpos[i] = i;
        nounmask[i] = (unsigned char)(upos_is(w[i], "NOUN")
            || upos_is(w[i], "PROPN"));
    }
    sent_dominance(&sent);
    arcs = (DependentiaArcus *)piscina_allocare(scratch,
        (memoriae_index)n * sizeof(DependentiaArcus));
    if (!dependentia_arcus(scratch, t, arcs)) fatal("arcus");
    tstat.trees++;
    /* the fast test against the library, every arc */
    for (g = 0; g < n; g++) {
        if (sent.head[g] == 0) continue;
        arcs_checked++;
        if (arc_displaced(&sent, idpos, sent.head[g] - 1, g, 0)
            != (arcs[g].non_proiectivus != 0))
            arcs_mismatch++;
    }
    for (g = 0; g < n; g++) {
        int h = sent.head[g] - 1;
        int is_gen, is_adj;

        if (h < 0) continue;
        is_gen = deprel_base_is(w[g], "nmod") && feat_is(w[g], "Case", "Gen");
        is_adj = deprel_base_is(w[g], "amod") && upos_is(w[g], "ADJ")
            && has_feat(w[g], "Case") && has_feat(w[g], "Gender")
            && has_feat(w[g], "Number");
        if (is_gen) {
            int nh = nounmask[h];

            tstat.gen_arcs++;
            tstat.gen_noun_head += nh;
            if (!arcs[g].non_proiectivus) {
                tstat.nested++;
            } else {
                int alt = alternatives(&sent, idpos, g, nounmask);
                int m, k, od, os;

                tstat.n_disp++;
                tstat.s_safe += alt == 0;
                if (alt > 0) tstat.alt_hist[alt >= 4 ? 3 : alt - 1]++;
                null_share(&sent, base_fwd, g, nounmask, &m, &k, &od, &os);
                own_checked++;
                if (!od || os != (alt == 0) || m < 1) own_mismatch++;
                tstat.k_null[m] += k;
                if (tstat.np == 64) fatal("too many in one sentence");
                tstat.pk[tstat.np] = k;
                tstat.pm[tstat.np] = m;
                tstat.np++;
                if (nh) {
                    tstat.nh_disp++;
                    tstat.nh_safe += alt == 0;
                    tstat.nh_k[m] += k;
                }
                /* reversal: the same verdict and the same null share */
                {
                    int m2, k2, od2, os2;
                    for (i = 0; i < n; i++) idpos[i] = n - 1 - i;
                    rev_checked++;
                    if (arc_displaced(&sent, idpos, h, g, 0) != 1
                        || (alternatives(&sent, idpos, g, nounmask) == 0)
                            != (alt == 0))
                        rev_mismatch++;
                    for (i = 0; i < n; i++) idpos[i] = i;
                    null_share(&sent, base_rev, g, nounmask, &m2, &k2, &od2,
                        &os2);
                    if (m2 != m || k2 != k || !od2 || os2 != (alt == 0))
                        rev_mismatch++;
                }
            }
        } else if (is_adj) {
            int other = 0;

            tstat.adj_arcs++;
            if (!arcs[g].non_proiectivus) continue;
            for (i = 0; i < n; i++) {
                agreemask[i] = (unsigned char)(nounmask[i]
                    && feat_equal(w[i], w[g], "Case")
                    && feat_equal(w[i], w[g], "Gender")
                    && feat_equal(w[i], w[g], "Number"));
                if (agreemask[i] && i != h && !sent.dom[g * n + i]) other = 1;
            }
            tstat.a_disp++;
            tstat.a_degenerate += !other;
            {
                int m, k, od, os;
                int alt_f = alternatives(&sent, idpos, g, agreemask);
                int alt_u = alternatives(&sent, idpos, g, nounmask);

                tstat.a_safe += alt_f == 0;
                tstat.au_safe += alt_u == 0;
                null_share(&sent, base_fwd, g, agreemask, &m, &k, &od, &os);
                tstat.a_k[m] += k;
                null_share(&sent, base_fwd, g, nounmask, &m, &k, &od, &os);
                tstat.au_k[m] += k;
            }
        }
    }
    free(sent.dom);
    sent.dom = NULL;
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
        dir = getenv("D132_UD_NC");
        if (dir == NULL) dir = "oratio/build/ud";
    }
    if (strlen(dir) + strlen(f->file) + 2 > sizeof(path))
        fatal("path too long");
    sprintf(path, "%s/%s", dir, f->file);
    p = piscina_generare_dynamicum("d132_file", 1 << 22);
    scratch = piscina_generare_dynamicum("d132_tree", 1 << 20);
    if (p == NULL || scratch == NULL) fatal("pool");
    src = filum_legere_totum(path, p);
    if (src.datum == NULL) {
        printf("  %-28s ABSENT (%s)\n", f->file, f->nc
            ? "NC: set D132_UD_NC or ./oratio/oraculum.sh -petere"
            : "repo fixture missing");
        if (!f->nc) check("repo treebank present", 0);
        piscina_destruere(scratch);
        piscina_destruere(p);
        return;
    }
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
        int          genre = bank == 3 ? perseus_genre(s->id) : -1;

        for (j = 0; j < xar_numerus(s->lexemata); j++)
            if (((const ConlluLexema *)xar_obtinere(s->lexemata, j))
                ->ranga)
                ranges++;
        piscina_vacare(scratch);
        memset(&tstat, 0, sizeof tstat);
        tstat.sentences = 1;
        if (!conllu_arbor(scratch, s, &a)
            || dependentia_validare(&a.arbor) != DEPENDENTIA_SANA) {
            unsound++;
            tstat.unsound = 1;
        } else {
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

                if (r == DEPENDENTIA_OMISSA_RADIX) {
                    tstat.aside_root = 1;
                } else if (r != DEPENDENTIA_OMISSA_FACTA) {
                    fatal("omittere");
                } else {
                    const ConlluLexema **kw = (const ConlluLexema **)
                        piscina_allocare(scratch, (memoriae_index)e.numerus
                        * sizeof(const ConlluLexema *));
                    i32 q = 0;

                    for (k = 0; k < a.arbor.numerus; k++)
                        if (!punct[k]) kw[q++] = a.verba[k];
                    if (q != e.numerus) fatal("kept words");
                    analyse(scratch, &e, kw);
                }
            }
        }
        add_row(bank, &tstat);
        if (genre == 0) add_row(ROW_PERSEUS_PROSE, &tstat);
        if (genre == 1) add_row(ROW_PERSEUS_VERSE, &tstat);
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

/* the plan's hand walk-through, as hand trees */
typedef struct {
    const char *title;
    int         n, g;              /* g 1-based */
    int         head[5];
    int         host[5];           /* host flags */
    int         disp, safe, m, k;  /* expected */
} HandTree;

static void
hand_trees (void)
{
    /* regis(gen) / puella / filiam / amat; bonam = an agreeing adjective */
    static const HandTree h[6] = {
        { "(a) filiam regis amat: nested", 3, 2, { 3, 1, 0, 0, 0 },
          { 1, 1, 0, 0, 0 }, 0, 0, 0, 0 },
        { "(b) regis puella filiam amat: displaced, puella can host - "
          "UNSAFE, null 1/2", 4, 1, { 3, 4, 4, 0, 0 }, { 1, 1, 1, 0, 0 },
          1, 0, 2, 1 },
        { "(c) regis amat puella filiam: displaced, every other host "
          "crosses the verb - SAFE, null 1/2", 4, 1, { 4, 0, 2, 2, 0 },
          { 1, 0, 1, 1, 0 }, 1, 1, 2, 1 },
        { "(d) bonam puella filiam amat, agreeing hosts only: SAFE, null "
          "2/2", 4, 1, { 3, 4, 4, 0, 0 }, { 0, 0, 1, 0, 0 }, 1, 1, 2, 2 },
        { "(d) the same, every noun a host: UNSAFE, null 1/2", 4, 1,
          { 3, 4, 4, 0, 0 }, { 0, 1, 1, 0, 0 }, 1, 0, 2, 1 },
        /* the genitive's own dependent (magni) moves with it: puella is
         * a nested host only because magni counts as re-hung under it */
        { "(e) puella magni regis amat filiam: magni goes with regis, so "
          "puella can host - UNSAFE, null 0/3", 5, 3, { 4, 3, 5, 0, 4 },
          { 1, 0, 1, 0, 1 }, 1, 0, 3, 0 }
    };
    int t, i;
    char msg[300];

    printf("\n--- the plan's hand walk-through ---\n");
    for (t = 0; t < 6; t++) {
        const HandTree *x = &h[t];
        unsigned char   host[5];
        int             g = x->g - 1, disp, safe = 0, m = 0, k = 0, od, os;

        sent.n = x->n;
        for (i = 0; i < x->n; i++) {
            sent.head[i] = x->head[i];
            base_fwd[i] = i;
            idpos[i] = i;
            host[i] = (unsigned char)x->host[i];
        }
        sent_dominance(&sent);
        disp = arc_displaced(&sent, idpos, sent.head[g] - 1, g, 0);
        if (disp) {
            safe = alternatives(&sent, idpos, g, host) == 0;
            null_share(&sent, base_fwd, g, host, &m, &k, &od, &os);
        }
        free(sent.dom);
        sent.dom = NULL;
        snprintf(msg, sizeof msg, "%s (got displaced %d, safe %d, null "
            "%d/%d)", x->title, disp, safe, k, m);
        check(msg, disp == x->disp && safe == x->safe && m == x->m
            && k == x->k);
    }
}

static void
part_a (void)
{
    int fi, b;

    printf("\n--- Part A: the treebanks (headline: without PUNCT) ---\n");
    for (fi = 0; fi < N_FILES; fi++) {
        if (quick && files[fi].nc) continue;
        read_file(fi);
    }
    {
        char msg[200];
        snprintf(msg, sizeof msg, "the fast arc test == dependentia_arcus "
            "on every arc of every headline tree (%ld arcs)", arcs_checked);
        check(msg, arcs_mismatch == 0 && arcs_checked > 0);
        snprintf(msg, sizeof msg, "every displaced genitive's null contains "
            "it: the placement at its own position is displaced, with its "
            "own verdict (%ld genitives)", own_checked);
        check(msg, own_mismatch == 0 && own_checked > 0);
        snprintf(msg, sizeof msg, "reversal: every displaced genitive keeps "
            "its verdict and its null share (%ld)", rev_checked);
        check(msg, rev_mismatch == 0 && rev_checked > 0);
    }
    for (b = 0; b < N_BANKS; b++)
        if (!bank_present[b]) printf("  %s: ABSENT\n", row_names[b]);
    hand_trees();
}

/* ================================================================
 * Part B: S, E, N; G1 and G2
 * ================================================================ */

static Piscina *fpool;

/* E = sum over m of k[m] / m, exact */
static Fractio
expected (
    const long *k)
{
    Fractio e = fractio_ex_s64(0), t;
    int     m;

    for (m = 1; m <= MAXN; m++) {
        if (k[m] == 0) continue;
        if (!fractio_ex_s64_s64((s64)k[m], (s64)m, fpool, &t))
            fatal("fractio");
        e = fractio_adde(e, t, fpool);
    }
    return e;
}

/* x * scale, rounded, as s64 */
static s64
scaled (
    Fractio x,
    s64     num,
    s64     den)
{
    Fractio f, r;
    s64     v;

    if (!fractio_ex_s64_s64(num, den, fpool, &f)) fatal("fractio");
    r = fractio_multiplica(x, f, fpool);
    if (!magnus_ad_s64(fractio_rotunda(r, fpool), &v)) fatal("magnus");
    return v;
}

/* P(null safe count >= s): each displaced genitive of the row safe
 * independently with its own share pk / pm (Poisson-binomial), exact */
static Fractio
null_tail (
    int  r,
    long s_safe)
{
    static Fractio dist[MAXN + 2];
    Fractio        one = fractio_ex_s64(1), p, q, t;
    int            i, j, len = 1;

    dist[0] = one;
    for (i = 0; i < row_np[r]; i++) {
        if (!fractio_ex_s64_s64((s64)row_pk[r][i], (s64)row_pm[r][i], fpool,
            &p))
            fatal("fractio");
        q = fractio_subtrahe(one, p, fpool);
        dist[len] = fractio_ex_s64(0);
        for (j = len; j >= 1; j--)
            dist[j] = fractio_adde(fractio_multiplica(dist[j], q, fpool),
                fractio_multiplica(dist[j - 1], p, fpool), fpool);
        dist[0] = fractio_multiplica(dist[0], q, fpool);
        len++;
    }
    t = fractio_ex_s64(0);
    for (j = (int)s_safe; j < len; j++) t = fractio_adde(t, dist[j], fpool);
    return t;
}

/* S > E, exactly */
static int
safe_above_null (
    long        s_safe,
    const long *k)
{
    return fractio_compara(fractio_ex_s64((s64)s_safe), expected(k), fpool)
        > 0;
}

/* registered: G1 holds iff S > E in at least five of the six banks */
static int
g1_rule (
    int banks_above)
{
    return banks_above >= 5;
}

/* registered: per bank, S / N >= 1/2 (N > 0); G2 iff in >= 5 of 6 */
static int
g2_bank (
    long s_safe,
    long n_disp)
{
    return n_disp > 0 && 2 * s_safe >= n_disp;
}

static int
g2_rule (
    int banks_half)
{
    return banks_half >= 5;
}

static void
row_line (
    const char *title,
    long        n_disp,
    long        s_safe,
    const long *k)
{
    Fractio e = expected(k);
    s64     e100 = scaled(e, 100, 1);
    s64     eshare = n_disp > 0 ? scaled(e, 10000, (s64)n_disp) : 0;
    char    s1[32];

    pct(s1, 32, s_safe, n_disp);
    printf("  %-16s N %5ld  S %5ld (%8s)  E %lld.%02lld (%lld.%02lld%%)  "
        "S %s E\n", title, n_disp, s_safe, s1, (long long)(e100 / 100),
        (long long)(e100 % 100), (long long)(eshare / 100),
        (long long)(eshare % 100),
        fractio_compara(fractio_ex_s64((s64)s_safe), e, fpool) > 0 ? ">"
        : fractio_compara(fractio_ex_s64((s64)s_safe), e, fpool) == 0 ? "="
        : "<");
}

/* oracle_d132.py (independent: own reader, re-attachment and rebuilt
 * sentences, fractions.Fraction); totals per row, E exact */
typedef struct {
    long        trees, gen, gen_nh, nested, n_disp, s_safe;
    const char *e;
    long        nh_disp, nh_safe;
    const char *nh_e;
    long        alt[4];
    long        adj, a_disp, a_deg, a_safe;
    const char *a_e;
    long        au_safe;
    const char *au_e;
    long        tail_e6;
} OraclePin;

static const OraclePin opin[N_ROWS] = {
    { 893, 477, 455, 397, 80, 36,
      "65257973812397468891/2312713359404327880", 79, 35,
      "62945260452993141011/2312713359404327880", { 35, 8, 1, 0 },
      1031, 445, 312, 438,
      "14363459706565846076047249/33782971056349257943200", 158,
      "232693883939639515209720061/1587799639648415123330400", 27751 },
    { 1734, 2382, 2371, 2377, 5, 3, "169727/310300", 5, 3,
      "169727/310300", { 2, 0, 0, 0 }, 2037, 68, 1, 67,
      "6642024128343551/116507455125600", 0,
      "971255450747/656996927400", 8446 },
    { 1260, 458, 406, 414, 44, 29, "12847568178043/534972952800", 26, 16,
      "3537559619/286322400", { 10, 4, 1, 0 }, 363, 28, 23, 28,
      "161884/5957", 19, "4043114837/323703380", 36797 },
    { 939, 255, 226, 213, 42, 27, "7832985187/411863760", 39, 26,
      "7174003171/411863760", { 11, 3, 1, 0 }, 509, 178, 142, 175,
      "18963551615351/110909026800", 84, "677482550438741/9104622836400",
      3280 },
    { 2101, 1098, 984, 1027, 71, 47, "2501476656761/77636318760", 61, 42,
      "2170967756897/77636318760", { 21, 2, 1, 0 }, 880, 78, 62, 73,
      "13682737/196350", 58, "53156601053671/1383951769200", 1 },
    { 421, 438, 390, 407, 31, 23, "2700854729/172393650", 28, 21,
      "1258803332/86196825", { 7, 1, 0, 0 }, 415, 34, 24, 34,
      "817646261999/24730249440", 26,
      "12307977539138734151/756531221601355200", 1644 },
    { 547, 117, 101, 111, 6, 4, "671/210", 4, 3, "67/42", { 2, 0, 0, 0 },
      202, 30, 23, 29, "2066201/73080", 17, "1495478923/118438320",
      383900 },
    { 392, 138, 125, 102, 36, 23, "6516982411/411863760", 35, 23,
      "6516982411/411863760", { 9, 3, 1, 0 }, 307, 148, 119, 146,
      "3820504723103/26771144400", 67, "106685147498827/1726738813800",
      4141 }
};

static int
fraction_is (
    Fractio     x,
    const char *text)
{
    Fractio f;

    if (!fractio_ex_chorda(chorda_ex_literis(text, fpool), fpool, &f))
        fatal("pin fraction");
    return fractio_aequalis(x, f);
}

static void
pinned_checks (void)
{
    int  r, counts_ok = 1, e_ok = 1, tail_ok = 1, rows = 0, i;
    char msg[300];

    for (r = 0; r < N_ROWS; r++) {
        const Stats     *s = &stats[r];
        const OraclePin *o = &opin[r];

        if (r < N_BANKS && !bank_present[r]) continue;
        if (r >= N_BANKS && !bank_present[3]) continue;
        rows++;
        if (s->trees != o->trees || s->gen_arcs != o->gen
            || s->gen_noun_head != o->gen_nh || s->nested != o->nested
            || s->n_disp != o->n_disp || s->s_safe != o->s_safe
            || s->nh_disp != o->nh_disp || s->nh_safe != o->nh_safe
            || s->adj_arcs != o->adj || s->a_disp != o->a_disp
            || s->a_degenerate != o->a_deg || s->a_safe != o->a_safe
            || s->au_safe != o->au_safe)
            counts_ok = 0;
        for (i = 0; i < 4; i++)
            if (s->alt_hist[i] != o->alt[i]) counts_ok = 0;
        if (!fraction_is(expected(s->k_null), o->e)
            || !fraction_is(expected(s->nh_k), o->nh_e)
            || !fraction_is(expected(s->a_k), o->a_e)
            || !fraction_is(expected(s->au_k), o->au_e))
            e_ok = 0;
        if (scaled(null_tail(r, s->s_safe), 1000000, 1) != (s64)o->tail_e6)
            tail_ok = 0;
    }
    snprintf(msg, sizeof msg, "every count (trees, genitive arcs, nested, N, "
        "S, the noun-headed subset, the alternatives, the adjective "
        "contrast) == oracle_d132.py, %d rows (a pin)", rows);
    check(msg, counts_ok && rows > 0);
    check("every E (registered, noun-headed, adjectives filtered and "
        "unfiltered) == oracle_d132.py's exact fraction (a pin)", e_ok);
    check("the null tail probabilities, rounded to 1e-6, == "
        "oracle_d132.py's (a pin)", tail_ok);
}

static void
part_b (void)
{
    int  r, above = 0, half = 0, present = 0;
    char s1[32], s2[32];

    printf("\n--- Part B: displaced genitives ---\n");
    printf("  N displaced, S safe (no nested alternative host), E the "
        "positional null's\n  expected safe count (exact; shown rounded)\n");
    printf("\n  %-16s %8s %8s %8s %8s\n", "treebank", "trees", "gen arcs",
        "noun head", "nested");
    for (r = 0; r < N_ROWS; r++) {
        const Stats *s = &stats[r];

        if (r < N_BANKS && !bank_present[r]) continue;
        if (r >= N_BANKS && !bank_present[3]) continue;
        pct(s1, 32, s->gen_noun_head, s->gen_arcs);
        printf("  %-16s %8ld %8ld %8s %8ld\n", row_names[r], s->trees,
            s->gen_arcs, s1, s->nested);
    }
    printf("\n  all displaced genitives (registered population)\n");
    for (r = 0; r < N_ROWS; r++) {
        if (r < N_BANKS && !bank_present[r]) continue;
        if (r >= N_BANKS && !bank_present[3]) continue;
        row_line(row_names[r], stats[r].n_disp, stats[r].s_safe,
            stats[r].k_null);
    }
    printf("\n  P(the null's safe count >= S), exact, rounded to 1e-6 (each "
        "displaced genitive\n  safe independently with its own null share; "
        "descriptive, not registered)\n");
    for (r = 0; r < N_ROWS; r++) {
        s64 t6;

        if (r < N_BANKS && !bank_present[r]) continue;
        if (r >= N_BANKS && !bank_present[3]) continue;
        t6 = scaled(null_tail(r, stats[r].s_safe), 1000000, 1);
        printf("  %-16s %lld.%06lld\n", row_names[r], (long long)(t6 / 1000000),
            (long long)(t6 % 1000000));
    }
    printf("\n  noun-headed displaced genitives (reported, not decided)\n");
    for (r = 0; r < N_ROWS; r++) {
        if (r < N_BANKS && !bank_present[r]) continue;
        if (r >= N_BANKS && !bank_present[3]) continue;
        row_line(row_names[r], stats[r].nh_disp, stats[r].nh_safe,
            stats[r].nh_k);
    }
    printf("\n  unsafe displaced genitives by number of nested alternatives "
        "(1, 2, 3, 4+)\n");
    for (r = 0; r < N_BANKS; r++) {
        if (!bank_present[r]) continue;
        printf("  %-16s %ld, %ld, %ld, %ld\n", row_names[r],
            stats[r].alt_hist[0], stats[r].alt_hist[1], stats[r].alt_hist[2],
            stats[r].alt_hist[3]);
    }
    printf("\n  adjective contrast (amod ADJ with Case, Gender, Number; "
        "reported, not decided)\n");
    for (r = 0; r < N_ROWS; r++) {
        const Stats *s = &stats[r];

        if (r < N_BANKS && !bank_present[r]) continue;
        if (r >= N_BANKS && !bank_present[3]) continue;
        pct(s2, 32, s->a_degenerate, s->a_disp);
        printf("  %-16s amod arcs %ld; displaced %ld, with no other "
            "agreeing noun %ld (%s)\n", row_names[r], s->adj_arcs, s->a_disp,
            s->a_degenerate, s2);
        row_line("   agreeing hosts", s->a_disp, s->a_safe, s->a_k);
        row_line("   every noun", s->a_disp, s->au_safe, s->au_k);
    }

    pinned_checks();

    printf("\n--- the registered rules on constructed totals ---\n");
    {
        static const long k_half[MAXN + 1];   /* all zero: E = 0 */
        long kk[MAXN + 1];

        memset(kk, 0, sizeof kk);
        kk[2] = 2;                            /* E = 2/2 = 1 */
        check("S > E is strict: S = E = 1 fails, S = 2 > 1 holds, S = 0 > "
            "E = 0 fails", !safe_above_null(1, kk) && safe_above_null(2, kk)
            && !safe_above_null(0, k_half));
        kk[2] = 0;
        kk[3] = 1;                            /* E = 1/3 */
        check("S > E with E a fraction: S = 1 > 1/3 holds, S = 0 fails",
            safe_above_null(1, kk) && !safe_above_null(0, kk));
        check("G2 per bank at its threshold: S / N = 1/2 holds, (S, N) = "
            "(2, 5) fails, N = 0 fails", g2_bank(2, 4) && !g2_bank(2, 5)
            && !g2_bank(0, 0));
        check("G1 and G2: five banks of six hold, four fail",
            g1_rule(5) && !g1_rule(4) && g2_rule(5) && !g2_rule(4)
            && g1_rule(6) && g2_rule(6));
    }

    printf("\n--- verdicts (registered) ---\n");
    for (r = 0; r < N_BANKS; r++) {
        if (!bank_present[r]) continue;
        present++;
        above += safe_above_null(stats[r].s_safe, stats[r].k_null);
        half += g2_bank(stats[r].s_safe, stats[r].n_disp);
    }
    if (present < N_BANKS || quick) {
        printf("  only %d of 6 treebanks present%s: the verdicts are not "
            "decided\n", present, quick ? " (quick mode)" : "");
        return;
    }
    printf("  G1 (S > E): %d of 6 treebanks -> G1 %s\n", above,
        g1_rule(above) ? "HOLDS" : "FAILS");
    printf("  G2 (S / N >= 1/2): %d of 6 treebanks -> G2 %s\n", half,
        g2_rule(half) ? "HOLDS" : "FAILS");
}

int
main (void)
{
    quick = getenv("D132_CELER") != NULL;
    printf("KNOTAPEL DEMO 132: real Latin and the displaced genitive%s\n",
        quick ? " (QUICK MODE: repo treebanks only)" : "");
    fpool = piscina_generare_dynamicum("d132_fractio", 1 << 22);
    if (fpool == NULL) fatal("pool");
    part_a();
    part_b();
    piscina_destruere(fpool);
    printf("\n=== %d pass, %d fail ===\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
