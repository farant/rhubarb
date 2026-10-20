/*
 * KNOTAPEL DEMO 134: umbra width of the toy world
 * ===============================================
 *
 * Proposal N17 (plan.md, approved 2026-10-20 after a design review;
 * brainstorm sections XI, XV, XVII). Reading left to right, a reader
 * carries what the structure has promised and not yet delivered: a word
 * waiting for its head, a verb waiting for its object. These open
 * strand ends are UMBRAE. D131 measured garden paths on its toy world;
 * D134 measures the mechanism on the same world: how many umbrae are
 * open at once (WIDTH), how deep one is when it closes (DEPTH), and
 * whether a displaced word is misread because a nearer word competes
 * for it (COMPETITION) or because the arriving word cannot single out
 * its partner (RETRIEVAL).
 *
 *   Task 0  the world, copied from D131 (the toy grammar is not a
 *           general algorithm, so not a house library): its paradigms,
 *           levels, generator against analyser, tree table, word order,
 *           hand sentences and Part B measures must reproduce D131's
 *           pinned five-word totals exactly (oracle_partb.py's
 *           numbers, carried over). Then SIX words, counts only: the
 *           generator and the analyser timed and sized, pairs by length
 *           against D131 task 0's count, projective and crossed six-word
 *           pairs, and the oracle's deterministic six-word subset. No
 *           umbra, label or garden-path number at six words in task 0.
 *
 * The world (syntax only, no semantics): nouns puella (1st, f), dominus
 * (2nd, m), donum (2nd, n), rex (3rd, m); adjectives bonus (1st/2nd),
 * fortis (3rd); verbs amat / amant (subject + object), dat / dant
 * (subject + object + dative). Cases nom, gen, dat, acc, abl. Slots:
 * exactly one nominative subject agreeing in number with the verb,
 * exactly one accusative object, for dat / dant exactly one dative, any
 * number of ablative adjuncts; a genitive depends on another noun (at
 * most one per noun); an adjective on a noun of the same case, gender
 * and number. No pro-drop, vocative or locative. Up to six words.
 * Levels: L0 Latin (Bennett's paradigms, macrons dropped), L1 ideal
 * (every reading its own form), L2 case-stripped (one form per gender
 * and number), L3 bare (as L2, adjectives invariant).
 *
 * A PAIR = (string, one analysis of it): a head per position and a
 * reading per position. Each analysis yields one string at each level,
 * so the population of pairs is the same at every level.
 *
 * Sources: Charles E. Bennett, New Latin Grammar (1895/1908/1918, public
 * domain; Project Gutenberg #15665): sec. 20 porta (= puella), 23 hortus
 * and bellum (= dominus, donum), 32 dux (= rex, regis), 63 bonus, 69
 * fortis (first-listed forms; acc. pl. -is omitted), 101 amat / amant,
 * 127 dat / dant. Cross-check: Whitaker's WORDS, INFLECTS.LAT (vendored
 * at oratio/vocabularium/la/), entries AGE X, FREQ A, read as data.
 *
 * House library: dependentia (and its closure), hence latina.h: Roman
 * numerals and Latin keywords are macros here (never name a variable I,
 * V, X, L, C, D or M, or per, si, dum, casus, ...).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_134_umbra_width/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * Quick mode for plants: D134_CELER=1 (exhaustive passes up to four
 * words; no six-word part).
 */

#include "dependentia.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
 * The lexicon (Bennett, macrons dropped)
 * ================================================================ */

enum { NOM, GEN, DAT, ACC, ABL, NCASES };
enum { MASC, FEM, NEUT, NGENDERS };
enum { SG, PL, NNUMBERS };

static const char *const case_name[NCASES] = {
    "nom", "gen", "dat", "acc", "abl"
};
static const char *const gender_name[NGENDERS] = { "m", "f", "n" };
static const char *const number_name[NNUMBERS] = { "sg", "pl" };

#define NNOUNS 4
#define NADJS  2
#define NVERBS 4

typedef struct {
    const char *lemma;
    int         gender;
    int         decl, var;          /* Whitaker's declension, variant */
    const char *stem1, *stem2;      /* Whitaker's stem keys 1, 2 */
    const char *form[NNUMBERS][NCASES];
} Noun;

static const Noun nouns[NNOUNS] = {
    { "puella", FEM, 1, 1, "puell", "puell",           /* Bennett 20 */
      { { "puella", "puellae", "puellae", "puellam", "puella" },
        { "puellae", "puellarum", "puellis", "puellas", "puellis" } } },
    { "dominus", MASC, 2, 1, "domin", "domin",         /* Bennett 23 */
      { { "dominus", "domini", "domino", "dominum", "domino" },
        { "domini", "dominorum", "dominis", "dominos", "dominis" } } },
    { "donum", NEUT, 2, 2, "don", "don",               /* Bennett 23 */
      { { "donum", "doni", "dono", "donum", "dono" },
        { "dona", "donorum", "donis", "dona", "donis" } } },
    { "rex", MASC, 3, 1, "rex", "reg",                 /* Bennett 32 */
      { { "rex", "regis", "regi", "regem", "rege" },
        { "reges", "regum", "regibus", "reges", "regibus" } } },
};

typedef struct {
    const char *lemma;
    int         decl, var;
    const char *stem1, *stem2;
    const char *form[NGENDERS][NNUMBERS][NCASES];
} Adj;

static const Adj adjs[NADJS] = {
    { "bonus", 1, 1, "bon", "bon",                     /* Bennett 63 */
      { { { "bonus", "boni", "bono", "bonum", "bono" },
          { "boni", "bonorum", "bonis", "bonos", "bonis" } },
        { { "bona", "bonae", "bonae", "bonam", "bona" },
          { "bonae", "bonarum", "bonis", "bonas", "bonis" } },
        { { "bonum", "boni", "bono", "bonum", "bono" },
          { "bona", "bonorum", "bonis", "bona", "bonis" } } } },
    { "fortis", 3, 2, "fort", "fort",                  /* Bennett 69 */
      { { { "fortis", "fortis", "forti", "fortem", "forti" },
          { "fortes", "fortium", "fortibus", "fortes", "fortibus" } },
        { { "fortis", "fortis", "forti", "fortem", "forti" },
          { "fortes", "fortium", "fortibus", "fortes", "fortibus" } },
        { { "forte", "fortis", "forti", "forte", "forti" },
          { "fortia", "fortium", "fortibus", "fortia", "fortibus" } } } },
};

typedef struct {
    const char *form;
    int         number;
    int         ditrans;
} Verb;

static const Verb verbs[NVERBS] = {              /* Bennett 101, 127 */
    { "amat", SG, 0 }, { "amant", PL, 0 }, { "dat", SG, 1 }, { "dant", PL, 1 }
};

/* ================================================================
 * Readings and the four levels
 * ================================================================ */

enum { K_NOUN, K_ADJ, K_VERB };

typedef struct {
    int kind, lemma, cas, gender, number;
} Reading;

#define MAXR  128
#define MAXF  128
#define MAXRF 32
#define NLEVELS 4

static Reading readings[MAXR];
static int     n_readings = 0;

static const char *const level_name[NLEVELS] = {
    "L0 Latin", "L1 ideal", "L2 case-stripped", "L3 bare"
};

static char form_text[NLEVELS][MAXF][40];
static int  n_forms[NLEVELS];
static int  form_of[NLEVELS][MAXR];
static int  form_rd[NLEVELS][MAXF][MAXRF];
static int  form_nr[NLEVELS][MAXF];

static int
rd_noun (
    int lemma,
    int cas,
    int number)
{
    return (lemma * NNUMBERS + number) * NCASES + cas;
}

static int
rd_adj (
    int lemma,
    int gender,
    int cas,
    int number)
{
    return NNOUNS * NNUMBERS * NCASES
        + ((lemma * NGENDERS + gender) * NNUMBERS + number) * NCASES + cas;
}

static int
rd_verb (
    int v)
{
    return NNOUNS * NNUMBERS * NCASES + NADJS * NGENDERS * NNUMBERS * NCASES
        + v;
}

/* the L0 form of a reading */
static const char *
form_l0 (
    int r)
{
    const Reading *x = &readings[r];

    if (x->kind == K_NOUN) return nouns[x->lemma].form[x->number][x->cas];
    if (x->kind == K_ADJ)
        return adjs[x->lemma].form[x->gender][x->number][x->cas];
    return verbs[x->lemma].form;
}

static int
intern (
    int         lv,
    const char *text)
{
    int f;

    for (f = 0; f < n_forms[lv]; f++)
        if (strcmp(form_text[lv][f], text) == 0) return f;
    if (n_forms[lv] == MAXF) fatal("too many forms");
    snprintf(form_text[lv][n_forms[lv]], sizeof form_text[lv][0], "%s", text);
    return n_forms[lv]++;
}

static void
build_world (void)
{
    int  l, g, n, c, v, lv, r;
    char buf[40];

    for (l = 0; l < NNOUNS; l++)
        for (n = 0; n < NNUMBERS; n++)
            for (c = 0; c < NCASES; c++) {
                Reading *x = &readings[rd_noun(l, c, n)];
                x->kind = K_NOUN; x->lemma = l; x->cas = c;
                x->gender = nouns[l].gender; x->number = n;
            }
    for (l = 0; l < NADJS; l++)
        for (g = 0; g < NGENDERS; g++)
            for (n = 0; n < NNUMBERS; n++)
                for (c = 0; c < NCASES; c++) {
                    Reading *x = &readings[rd_adj(l, g, c, n)];
                    x->kind = K_ADJ; x->lemma = l; x->cas = c;
                    x->gender = g; x->number = n;
                }
    for (v = 0; v < NVERBS; v++) {
        Reading *x = &readings[rd_verb(v)];
        x->kind = K_VERB; x->lemma = v; x->cas = -1;
        x->gender = -1; x->number = verbs[v].number;
    }
    n_readings = rd_verb(NVERBS - 1) + 1;

    for (lv = 0; lv < NLEVELS; lv++) {
        n_forms[lv] = 0;
        for (r = 0; r < n_readings; r++) {
            const Reading *x = &readings[r];
            const char    *t = form_l0(r);

            if (x->kind != K_VERB) {
                if (lv == 1) {
                    snprintf(buf, sizeof buf, "%s.%s.%s.%s",
                        x->kind == K_NOUN ? nouns[x->lemma].lemma
                                          : adjs[x->lemma].lemma,
                        case_name[x->cas], gender_name[x->gender],
                        number_name[x->number]);
                    t = buf;
                } else if (lv >= 2 && x->kind == K_NOUN) {
                    t = nouns[x->lemma].form[x->number][NOM];
                } else if (lv == 2) {
                    t = adjs[x->lemma].form[x->gender][x->number][NOM];
                } else if (lv == 3) {
                    t = adjs[x->lemma].form[MASC][SG][NOM];
                }
            }
            form_of[lv][r] = intern(lv, t);
        }
        for (r = 0; r < n_forms[lv]; r++) form_nr[lv][r] = 0;
        for (r = 0; r < n_readings; r++) {
            int f = form_of[lv][r];
            if (form_nr[lv][f] == MAXRF) fatal("too many readings per form");
            form_rd[lv][f][form_nr[lv][f]++] = r;
        }
    }
}

static int
form_kind (
    int lv,
    int f)
{
    return readings[form_rd[lv][f][0]].kind;
}

/* ================================================================
 * Whitaker's INFLECTS.LAT (data), for the cross-check
 * ================================================================ */

typedef struct {
    int  adj;                 /* 0 = N, 1 = ADJ */
    int  decl, var, cas, number;
    char gender;              /* 'M' 'F' 'N' 'C' (m or f) 'X' (any) */
    int  pos;                 /* ADJ: degree POS */
    int  key;
    char ending[12];
    int  standard;            /* AGE X and FREQ A */
} WEntry;

#define MAXW_ENTRIES 4096
static WEntry went[MAXW_ENTRIES];
static int    n_went = 0;

static int
w_case (
    const char *s)
{
    if (strcmp(s, "NOM") == 0) return NOM;
    if (strcmp(s, "GEN") == 0) return GEN;
    if (strcmp(s, "DAT") == 0) return DAT;
    if (strcmp(s, "ACC") == 0) return ACC;
    if (strcmp(s, "ABL") == 0) return ABL;
    return -1;
}

static int
read_whitaker (
    const char *path)
{
    Piscina *p = piscina_generare_dynamicum("d134_whitaker", 1 << 22);
    chorda   src = filum_legere_totum(path, p);
    i32      at = 0;

    if (src.mensura == 0) {
        piscina_destruere(p);
        return 0;
    }
    while (at < src.mensura) {
        char  line[256];
        char *tok[16];
        int   nt = 0, len = 0;
        char *s, *cm;

        while (at < src.mensura && src.datum[at] != '\n') {
            if (len < 255) line[len++] = (char)src.datum[at];
            at++;
        }
        at++;
        line[len] = '\0';
        cm = strstr(line, "--");
        if (cm != NULL) *cm = '\0';
        for (s = strtok(line, " \t\r"); s != NULL && nt < 16;
             s = strtok(NULL, " \t\r"))
            tok[nt++] = s;
        if (nt < 9) continue;
        {
            WEntry e;
            int    adj = strcmp(tok[0], "ADJ") == 0;
            int    b = adj ? 7 : 6;   /* index of the stem key */
            int    elen;

            if (!adj && strcmp(tok[0], "N") != 0) continue;
            memset(&e, 0, sizeof e);
            e.adj = adj;
            e.decl = atoi(tok[1]);
            e.var = atoi(tok[2]);
            e.cas = w_case(tok[3]);
            if (e.cas < 0) continue;  /* VOC, LOC: outside the world */
            e.number = strcmp(tok[4], "S") == 0 ? SG : PL;
            e.gender = tok[5][0];
            e.pos = adj ? strcmp(tok[6], "POS") == 0 : 1;
            if (nt < b + 4) continue;
            e.key = atoi(tok[b]);
            elen = atoi(tok[b + 1]);
            {
                /* [ending] AGE FREQ */
                int age = elen > 0 ? b + 3 : b + 2;

                if (nt < age + 2) continue;
                if (elen > 0)
                    snprintf(e.ending, sizeof e.ending, "%s", tok[b + 2]);
                e.standard = strcmp(tok[age], "X") == 0
                    && strcmp(tok[age + 1], "A") == 0;
            }
            if (n_went == MAXW_ENTRIES) fatal("too many Whitaker entries");
            went[n_went++] = e;
        }
    }
    piscina_destruere(p);
    return 1;
}

static int
w_gender_ok (
    char eg,
    int  g)
{
    if (eg == 'X') return 1;
    if (eg == 'C') return g == MASC || g == FEM;
    return (eg == 'M' && g == MASC) || (eg == 'F' && g == FEM)
        || (eg == 'N' && g == NEUT);
}

/* Whitaker's standard forms for one cell: into out[], count returned */
static int
whitaker_forms (
    int          adj,
    int          decl,
    int          var,
    int          gender,
    const char  *stem1,
    const char  *stem2,
    int          cas,
    int          number,
    char         out[][40],
    int          cap)
{
    int k, j, n = 0;

    for (k = 0; k < n_went; k++) {
        const WEntry *e = &went[k];
        char          f[40];
        int           dup = 0;

        if (e->adj != adj || e->decl != decl) continue;
        if (e->var != 0 && e->var != var) continue;
        if (e->cas != cas || e->number != number) continue;
        if (!w_gender_ok(e->gender, gender) || !e->pos || !e->standard)
            continue;
        snprintf(f, sizeof f, "%s%s", e->key == 1 ? stem1 : stem2, e->ending);
        for (j = 0; j < n; j++)
            if (strcmp(out[j], f) == 0) dup = 1;
        if (!dup && n < cap) snprintf(out[n++], 40, "%s", f);
    }
    return n;
}

/* ================================================================
 * Analyses
 * ================================================================ */

#define MAXW 6
#define MAXA 4096

enum { R_ROOT, R_NSUBJ, R_OBJ, R_IOBJ, R_OBL, R_NMOD, R_AMOD };

typedef struct {
    int head[MAXW];           /* 1-based position, 0 = root */
    int rd[MAXW];
} Analysis;

static i64
analysis_key (
    const Analysis *a,
    int             n)
{
    i64 k = (i64)n;
    int i;

    for (i = 0; i < n; i++)
        k |= ((i64)a->head[i] | ((i64)a->rd[i] << 3)) << (3 + 10 * i);
    return k;
}

static i64
bag_code (
    const int *form,
    int        n)
{
    i64 k = (i64)n;
    int i;

    for (i = 0; i < n; i++) k |= (i64)form[i] << (3 + 7 * i);
    return k;
}

/* --- the analyser: constraints, order-free by intent --- */

typedef struct {
    int head, rel, rd;
} Opt;

static int        an_lv, an_n, an_vpos, an_ditrans, an_vnum;
static const int *an_form;
static int        an_nrd[MAXW];           /* chosen noun readings */
static Opt        an_opt[MAXW][MAXW * 2 + 2];
static int        an_nopt[MAXW];
static int        an_pick[MAXW];
static Analysis  *an_out;
static int        an_count, an_cap;

static int
an_tree_ok (void)
{
    int i, j, steps;

    for (i = 0; i < an_n; i++) {
        j = i;
        steps = 0;
        while (an_opt[j][an_pick[j]].head != 0) {
            j = an_opt[j][an_pick[j]].head - 1;
            if (++steps > an_n) return 0;
        }
    }
    return 1;
}

static void
an_assign (
    int i,
    int nsubj,
    int obj,
    int iobj,
    int genmask)
{
    int k;

    if (i == an_n) {
        if (nsubj != 1 || obj != 1 || iobj != an_ditrans) return;
        if (!an_tree_ok()) return;
        if (an_count == an_cap) fatal("too many analyses for one string");
        for (k = 0; k < an_n; k++) {
            an_out[an_count].head[k] = an_opt[k][an_pick[k]].head;
            an_out[an_count].rd[k] = an_opt[k][an_pick[k]].rd;
        }
        an_count++;
        return;
    }
    for (k = 0; k < an_nopt[i]; k++) {
        const Opt *o = &an_opt[i][k];
        int        gm = genmask;

        if (o->rel == R_NSUBJ && nsubj == 1) continue;
        if (o->rel == R_OBJ && obj == 1) continue;
        if (o->rel == R_IOBJ && iobj == an_ditrans) continue;
        if (o->rel == R_NMOD) {
            if (genmask & (1 << (o->head - 1))) continue;
            gm |= 1 << (o->head - 1);
        }
        an_pick[i] = k;
        an_assign(i + 1, nsubj + (o->rel == R_NSUBJ), obj + (o->rel == R_OBJ),
            iobj + (o->rel == R_IOBJ), gm);
    }
}

static void
an_options (void)
{
    int i, j, k;

    for (i = 0; i < an_n; i++) {
        int f = an_form[i];
        int kind = form_kind(an_lv, f);

        an_nopt[i] = 0;
        if (kind == K_VERB) {
            Opt o = { 0, R_ROOT, 0 };
            o.rd = form_rd[an_lv][f][0];
            an_opt[i][an_nopt[i]++] = o;
        } else if (kind == K_NOUN) {
            const Reading *x = &readings[an_nrd[i]];
            Opt            o;

            o.rd = an_nrd[i];
            o.head = an_vpos + 1;
            if (x->cas == NOM && x->number == an_vnum) {
                o.rel = R_NSUBJ; an_opt[i][an_nopt[i]++] = o;
            }
            if (x->cas == ACC) { o.rel = R_OBJ; an_opt[i][an_nopt[i]++] = o; }
            if (x->cas == DAT && an_ditrans) {
                o.rel = R_IOBJ; an_opt[i][an_nopt[i]++] = o;
            }
            if (x->cas == ABL) { o.rel = R_OBL; an_opt[i][an_nopt[i]++] = o; }
            if (x->cas == GEN)
                for (j = 0; j < an_n; j++) {
                    if (j == i || form_kind(an_lv, an_form[j]) != K_NOUN)
                        continue;
                    o.head = j + 1; o.rel = R_NMOD;
                    an_opt[i][an_nopt[i]++] = o;
                }
        } else {
            for (j = 0; j < an_n; j++) {
                const Reading *y;

                if (j == i || form_kind(an_lv, an_form[j]) != K_NOUN) continue;
                y = &readings[an_nrd[j]];
                for (k = 0; k < form_nr[an_lv][f]; k++) {
                    const Reading *x = &readings[form_rd[an_lv][f][k]];
                    if (x->cas == y->cas && x->gender == y->gender
                        && x->number == y->number) {
                        Opt o;
                        o.head = j + 1; o.rel = R_AMOD;
                        o.rd = form_rd[an_lv][f][k];
                        an_opt[i][an_nopt[i]++] = o;
                    }
                }
            }
        }
        if (an_nopt[i] == 0) return;
    }
    an_assign(0, 0, 0, 0, 0);
}

static void
an_nouns (
    int i)
{
    int k;

    if (i == an_n) {
        an_options();
        return;
    }
    if (form_kind(an_lv, an_form[i]) != K_NOUN) {
        an_nouns(i + 1);
        return;
    }
    for (k = 0; k < form_nr[an_lv][an_form[i]]; k++) {
        an_nrd[i] = form_rd[an_lv][an_form[i]][k];
        an_nouns(i + 1);
    }
}

/* all analyses of the string form[0..n-1] at level lv */
static int
analyse (
    int        lv,
    const int *form,
    int        n,
    Analysis  *out,
    int        cap)
{
    int i, nv = 0;

    an_lv = lv; an_form = form; an_n = n;
    an_out = out; an_count = 0; an_cap = cap;
    for (i = 0; i < n; i++)
        if (form_kind(lv, form[i]) == K_VERB) {
            nv++;
            an_vpos = i;
        }
    if (nv != 1) return 0;
    an_vnum = readings[form_rd[lv][form[an_vpos]][0]].number;
    an_ditrans = verbs[readings[form_rd[lv][form[an_vpos]][0]].lemma].ditrans;
    an_nouns(0);
    return an_count;
}

/* --- the generator: structures built slot by slot --- */

typedef struct {
    int rd, head;             /* head = token index + 1, 0 = root */
} Tok;

enum { T_NP, T_ADJS, T_GEN, T_ABL };

typedef struct {
    int type, cas, head, numfix, minlem;
} Task;

#define MAXTASKS 32

static Tok gtok[MAXW];
static int gn = 0;
static int gmaxw = MAXW;
static int perms[MAXW + 1][720][MAXW];
static int nperms[MAXW + 1];

typedef struct {
    i64 bag, key;
} Entry;

typedef struct {
    Entry *e;
    long   n, cap;
} Entries;

static Entries gen_entries[NLEVELS];
static long    gen_emitted = 0;

static void
push_entry (
    Entries *v,
    i64      bag,
    i64      key)
{
    if (v->n == v->cap) {
        v->cap = v->cap ? v->cap * 2 : 1 << 16;
        v->e = (Entry *)realloc(v->e, (size_t)v->cap * sizeof(Entry));
        if (v->e == NULL) fatal("memory");
    }
    v->e[v->n].bag = bag;
    v->e[v->n].key = key;
    v->n++;
}

static int
entry_cmp (
    const void *a,
    const void *b)
{
    const Entry *x = (const Entry *)a, *y = (const Entry *)b;

    if (x->bag != y->bag) return x->bag < y->bag ? -1 : 1;
    if (x->key != y->key) return x->key < y->key ? -1 : 1;
    return 0;
}

static long
sort_unique (
    Entries *v)
{
    long i, m = 0, dups = 0;

    if (v->n == 0) return 0;
    qsort(v->e, (size_t)v->n, sizeof(Entry), entry_cmp);
    for (i = 1; i < v->n; i++) {
        if (entry_cmp(&v->e[i], &v->e[m]) == 0) {
            dups++;
            continue;
        }
        v->e[++m] = v->e[i];
    }
    v->n = m + 1;
    return dups;
}

static int
i64_cmp (
    const void *a,
    const void *b)
{
    i64 x = *(const i64 *)a, y = *(const i64 *)b;

    return x < y ? -1 : x > y ? 1 : 0;
}

static int
int_cmp (
    const void *a,
    const void *b)
{
    return *(const int *)a - *(const int *)b;
}

/* every placement of the tokens on the canonical (sorted) positions */
static void
gen_emit (void)
{
    int lv, p, i;

    gen_emitted++;
    for (lv = 0; lv < NLEVELS; lv++) {
        int form[MAXW], bag[MAXW];

        for (i = 0; i < gn; i++) bag[i] = form[i] = form_of[lv][gtok[i].rd];
        qsort(bag, (size_t)gn, sizeof(int), int_cmp);
        for (p = 0; p < nperms[gn]; p++) {
            const int *pos = perms[gn][p];   /* token i -> position pos[i] */
            Analysis   a;
            int        ok = 1;

            for (i = 0; i < gn && ok; i++)
                if (form[i] != bag[pos[i]]) ok = 0;
            if (!ok) continue;
            for (i = 0; i < gn; i++) {
                a.head[pos[i]] = gtok[i].head == 0 ? 0
                    : pos[gtok[i].head - 1] + 1;
                a.rd[pos[i]] = gtok[i].rd;
            }
            push_entry(&gen_entries[lv], bag_code(bag, gn), analysis_key(&a, gn));
        }
    }
}

static void
gen_run (
    const Task *stack,
    int         top)
{
    Task st[MAXTASKS];
    Task t;
    int  l, n, lo, hi;

    if (top == 0) {
        gen_emit();
        return;
    }
    memcpy(st, stack, (size_t)top * sizeof(Task));
    t = st[--top];
    if (top + 3 > MAXTASKS) fatal("task stack");
    switch (t.type) {
    case T_NP:
        if (gn == gmaxw) return;
        lo = t.numfix >= 0 ? t.numfix : 0;
        hi = t.numfix >= 0 ? t.numfix : NNUMBERS - 1;
        for (l = 0; l < NNOUNS; l++)
            for (n = lo; n <= hi; n++) {
                int ni = gn;

                gtok[gn].rd = rd_noun(l, t.cas, n);
                gtok[gn].head = t.head;
                gn++;
                st[top].type = T_GEN; st[top].head = ni;
                st[top + 1].type = T_ADJS; st[top + 1].head = ni;
                st[top + 1].minlem = 0;
                gen_run(st, top + 2);
                gn--;
            }
        return;
    case T_ADJS:
        gen_run(st, top);
        if (gn == gmaxw) return;
        for (l = t.minlem; l < NADJS; l++) {
            const Reading *x = &readings[gtok[t.head].rd];

            gtok[gn].rd = rd_adj(l, x->gender, x->cas, x->number);
            gtok[gn].head = t.head + 1;
            gn++;
            st[top].type = T_ADJS; st[top].head = t.head; st[top].minlem = l;
            gen_run(st, top + 1);
            gn--;
        }
        return;
    case T_GEN:
        gen_run(st, top);
        st[top].type = T_NP; st[top].cas = GEN; st[top].head = t.head + 1;
        st[top].numfix = -1;
        gen_run(st, top + 1);
        return;
    case T_ABL:
        gen_run(st, top);
        st[top].type = T_ABL;
        st[top + 1].type = T_NP; st[top + 1].cas = ABL; st[top + 1].head = 1;
        st[top + 1].numfix = -1;
        gen_run(st, top + 2);
        return;
    default:
        fatal("task");
    }
}

static void
generate (void)
{
    int  v, top;
    Task st[MAXTASKS];

    for (v = 0; v < NVERBS; v++) {
        gn = 1;
        gtok[0].rd = rd_verb(v);
        gtok[0].head = 0;
        top = 0;
        st[top].type = T_ABL; top++;
        if (verbs[v].ditrans) {
            st[top].type = T_NP; st[top].cas = DAT; st[top].head = 1;
            st[top].numfix = -1; top++;
        }
        st[top].type = T_NP; st[top].cas = ACC; st[top].head = 1;
        st[top].numfix = -1; top++;
        st[top].type = T_NP; st[top].cas = NOM; st[top].head = 1;
        st[top].numfix = verbs[v].number; top++;
        gen_run(st, top);
    }
}

static void
build_perms (void)
{
    int n, p, i, j;

    for (n = 0; n <= MAXW; n++) {
        int a[MAXW];

        for (i = 0; i < n; i++) a[i] = i;
        nperms[n] = 0;
        for (;;) {
            memcpy(perms[n][nperms[n]++], a, sizeof a);
            /* next permutation (lexicographic) */
            for (i = n - 2; i >= 0 && a[i] > a[i + 1]; i--) {}
            if (i < 0) break;
            for (j = n - 1; a[j] < a[i]; j--) {}
            p = a[i]; a[i] = a[j]; a[j] = p;
            for (i++, j = n - 1; i < j; i++, j--) {
                p = a[i]; a[i] = a[j]; a[j] = p;
            }
        }
    }
}

/* ================================================================
 * The tree table: every rooted tree on n <= 6 positions, measured once
 * ================================================================ */

static int tree_index (const int *head, int n)
{
    int k, c = 0, b = 1;

    for (k = 0; k < n; k++) {
        c += head[k] * b;
        b *= MAXW + 1;
    }
    return c;
}

#define TCODES 117649          /* 7^6 */
static unsigned char t_valid[MAXW + 1][TCODES];
static unsigned char t_proj[MAXW + 1][TCODES];
static unsigned char t_proj2[MAXW + 1][TCODES];
static unsigned char t_mild[MAXW + 1][TCODES];
static unsigned char t_npmask[MAXW + 1][TCODES];
static unsigned char t_cross[MAXW + 1][TCODES];   /* crossings, root arc
                                                     included */

static void
part_trees (void)
{
    Piscina *scratch = piscina_generare_dynamicum("d134_tree", 1 << 16);
    int      n, k, code, ncodes;
    int      cayley_ok = 1, routes_ok = 1, mild_ok = 1, rev_ok = 1;
    int      counts_ok = 1;
    long     ntrees = 0;
    static const long proj_count[MAXW + 1] = { 0, 1, 2, 7, 30, 143, 728 };
    static const long mild_count[MAXW + 1] = { 0, 1, 2, 9, 64, 569, 5652 };
    char     msg[256];

    printf("\n--- the tree table ---\n");
    for (n = 1; n <= MAXW; n++) {
        long expect = 1, count = 0, nproj = 0, nmild = 0;

        ncodes = 1;
        for (k = 0; k < n; k++) ncodes *= n + 1;
        for (k = 0; k < n - 1; k++) expect *= n;
        for (code = 0; code < ncodes; code++) {
            int               head[MAXW], c = code, self = 0;
            i32               cap[MAXW];
            DependentiaArbor  t;
            DependentiaCruces x;
            DependentiaMensura m;
            DependentiaArcus  arcs[MAXW];

            for (k = 0; k < n; k++) {
                head[k] = c % (n + 1);
                c /= n + 1;
                if (head[k] == k + 1) self = 1;
                cap[k] = (i32)head[k];
            }
            if (self) continue;
            t.numerus = (i32)n;
            t.capita = cap;
            if (dependentia_validare(&t) != DEPENDENTIA_SANA) continue;
            count++;
            piscina_vacare(scratch);
            if (!dependentia_cruces(scratch, &t, &x)
                || !dependentia_mensurare(scratch, &t, NULL, &m)
                || !dependentia_arcus(scratch, &t, arcs))
                fatal("dependentia");
            {
                int ix = tree_index(head, n), mask = 0;

                t_valid[n][ix] = 1;
                t_proj[n][ix] = (unsigned char)(x.proiectiva != 0);
                t_proj2[n][ix] = (unsigned char)(m.proiectiva != 0);
                t_mild[n][ix] = (unsigned char)(m.gradus_lacunarum <= 1
                    && m.bene_nidificata);
                for (k = 0; k < n; k++)
                    if (arcs[k].non_proiectivus) mask |= 1 << k;
                t_npmask[n][ix] = (unsigned char)mask;
                t_cross[n][ix] = (unsigned char)(x.cruces + x.cruces_radicis);
                nproj += t_proj[n][ix];
                nmild += t_mild[n][ix];
                if (t_proj[n][ix] != t_proj2[n][ix]) routes_ok = 0;
                if (t_proj[n][ix] && !t_mild[n][ix]) mild_ok = 0;
                if (t_proj[n][ix] != (mask == 0)) routes_ok = 0;
            }
        }
        if (count != expect) cayley_ok = 0;
        if (nproj != proj_count[n] || nmild != mild_count[n]) counts_ok = 0;
        ntrees += count;
    }
    /* reversal: position k -> n + 1 - k */
    for (n = 1; n <= MAXW; n++) {
        ncodes = 1;
        for (k = 0; k < n; k++) ncodes *= n + 1;
        for (code = 0; code < ncodes; code++) {
            int head[MAXW], rh[MAXW], c = code, ix, rx, mask, rmask = 0;

            for (k = 0; k < n; k++) {
                head[k] = c % (n + 1);
                c /= n + 1;
            }
            ix = tree_index(head, n);
            if (!t_valid[n][ix]) continue;
            for (k = 0; k < n; k++)
                rh[n - 1 - k] = head[k] == 0 ? 0 : n + 1 - head[k];
            rx = tree_index(rh, n);
            mask = t_npmask[n][ix];
            for (k = 0; k < n; k++)
                if (mask & (1 << k)) rmask |= 1 << (n - 1 - k);
            if (!t_valid[n][rx] || t_proj[n][rx] != t_proj[n][ix]
                || t_mild[n][rx] != t_mild[n][ix] || t_npmask[n][rx] != rmask)
                rev_ok = 0;
        }
    }
    snprintf(msg, sizeof msg,
        "trees on n <= 6 positions: n^(n-1) for every n (Cayley), %ld in all",
        ntrees);
    check(msg, cayley_ok);
    check("projective by crossings (root arc) == by interval yields, and == "
        "no non-projective arc, on every tree", routes_ok);
    check("projective => mild on every tree", mild_ok);
    check("projective trees per n = 1, 2, 7, 30, 143, 728 and mild trees = "
        "1, 2, 9, 64, 569, 5652 (D131's oracle_trees.py run to n = 6, "
        "independent Python; a pin)", counts_ok);
    check("reversal keeps projective and mild and mirrors the non-projective "
        "arcs, on every tree", rev_ok);
    piscina_destruere(scratch);
}

/* ================================================================
 * Part A
 * ================================================================ */

static void
part_paradigms (void)
{
    const char *path = "oratio/vocabularium/la/INFLECTS.LAT";
    static const struct { int gender, cas, number; const char *form; }
        extras[8] = {
        { MASC, ABL, SG, "forte" }, { FEM, ABL, SG, "forte" },
        { NEUT, ABL, SG, "forte" }, { MASC, GEN, PL, "fortum" },
        { FEM, GEN, PL, "fortum" }, { NEUT, GEN, PL, "fortum" },
        { MASC, ACC, PL, "fortis" }, { FEM, ACC, PL, "fortis" }
    };
    char out[16][40];
    int  l, g, n, c, k, j, equal = 0, cells = 0, member = 0, fcells = 0;
    int  extra_found = 0, extra_listed = 1, lv, single = 1, derived = 1;
    int  onekind = 1;
    char msg[256];

    printf("\n--- paradigms and levels ---\n");
    if (!read_whitaker(path)) fatal("cannot read INFLECTS.LAT");
    for (l = 0; l < NNOUNS; l++)
        for (n = 0; n < NNUMBERS; n++)
            for (c = 0; c < NCASES; c++) {
                const Noun *x = &nouns[l];
                int m = whitaker_forms(0, x->decl, x->var, x->gender,
                    x->stem1, x->stem2, c, n, out, 16);
                cells++;
                if (m == 1 && strcmp(out[0], x->form[n][c]) == 0) equal++;
            }
    for (g = 0; g < NGENDERS; g++)
        for (n = 0; n < NNUMBERS; n++)
            for (c = 0; c < NCASES; c++) {
                const Adj *x = &adjs[0];
                int m = whitaker_forms(1, x->decl, x->var, g,
                    x->stem1, x->stem2, c, n, out, 16);
                cells++;
                if (m == 1 && strcmp(out[0], x->form[g][n][c]) == 0) equal++;
            }
    snprintf(msg, sizeof msg,
        "nouns and bonus: the table (Bennett's, cited) == Whitaker (AGE X, "
        "FREQ A) in %d of %d cells", equal, cells);
    check(msg, equal == cells && cells == 70);
    for (g = 0; g < NGENDERS; g++)
        for (n = 0; n < NNUMBERS; n++)
            for (c = 0; c < NCASES; c++) {
                const Adj *x = &adjs[1];
                int m = whitaker_forms(1, x->decl, x->var, g,
                    x->stem1, x->stem2, c, n, out, 16);
                int in = 0;

                fcells++;
                for (k = 0; k < m; k++) {
                    if (strcmp(out[k], x->form[g][n][c]) == 0) {
                        in = 1;
                        continue;
                    }
                    extra_found++;
                    {
                        int listed = 0;
                        for (j = 0; j < 8; j++)
                            if (extras[j].gender == g && extras[j].cas == c
                                && extras[j].number == n
                                && strcmp(extras[j].form, out[k]) == 0)
                                listed = 1;
                        if (!listed) extra_listed = 0;
                    }
                }
                member += in;
            }
    snprintf(msg, sizeof msg,
        "fortis: Bennett's form among Whitaker's in %d of %d cells; Whitaker's "
        "%d other forms are exactly the 8 listed (abl -e, gen pl -um, acc pl "
        "-is)", member, fcells, extra_found);
    check(msg, member == fcells && fcells == 30 && extra_found == 8
        && extra_listed);

    for (lv = 0; lv < NLEVELS; lv++) {
        int f, nominal = 0;

        for (f = 0; f < n_forms[lv]; f++) {
            int kind = form_kind(lv, f), r;

            if (kind != K_VERB) nominal++;
            for (r = 0; r < form_nr[lv][f]; r++)
                if (readings[form_rd[lv][f][r]].kind != kind) onekind = 0;
            if (lv == 1 && form_nr[lv][f] != 1) single = 0;
            if (lv >= 2) {
                int r0, found = 0;
                for (r0 = 0; r0 < n_readings; r0++)
                    if (strcmp(form_l0(r0), form_text[lv][f]) == 0) found = 1;
                if (!found) derived = 0;
            }
        }
        printf("  %-17s %3d nominal forms + %d verbs\n", level_name[lv],
            nominal, NVERBS);
        snprintf(msg, sizeof msg, "%s: %d nominal forms (L0 47, L1 100, L2 17, "
            "L3 10; counted by hand from the tables)", level_name[lv], nominal);
        check(msg, nominal == (lv == 0 ? 47 : lv == 1 ? 100 : lv == 2 ? 17 : 10)
            && n_forms[lv] == nominal + NVERBS);
    }
    check("every form's readings are of one kind (noun, adjective or verb)",
        onekind);
    check("L1: every form has exactly one reading", single);
    check("L2, L3: every form is an L0 form (derived, never hand-written)",
        derived);
}

/* the analyser's set for every bag, kept for the later parts */
static Entries an_entries[NLEVELS];

static long
multinomial (
    const int *bag,
    int        n)
{
    long m = 1;
    int  i, run = 1, k;

    for (k = 2; k <= n; k++) m *= k;
    for (i = 1; i <= n; i++) {
        if (i < n && bag[i] == bag[i - 1]) {
            run++;
            continue;
        }
        for (k = 2; k <= run; k++) m /= k;
        run = 1;
    }
    return m;
}

static void
decode_bag (
    i64  code,
    int *bag,
    int *n)
{
    int i;

    *n = (int)(code & 7);
    for (i = 0; i < *n; i++) bag[i] = (int)((code >> (3 + 7 * i)) & 127);
}

static void
decode_key (
    i64       key,
    Analysis *a,
    int       n)
{
    int i;

    for (i = 0; i < n; i++) {
        i64 w = (key >> (3 + 10 * i)) & 1023;
        a->head[i] = (int)(w & 7);
        a->rd[i] = (int)(w >> 3);
    }
}

static Analysis abuf[MAXA];

/* every multiset of nominal forms with one verb: the analyser's sets */
static void
analyse_all_bags (
    int  lv,
    long *bags_tried)
{
    int nominal[MAXF], nn = 0, f, v, m;
    int idx[MAXW];

    for (f = 0; f < n_forms[lv]; f++)
        if (form_kind(lv, f) != K_VERB) nominal[nn++] = f;
    for (v = 0; v < n_forms[lv]; v++) {
        if (form_kind(lv, v) != K_VERB) continue;
        for (m = 0; m < gmaxw; m++) {      /* m nominal words + the verb */
            int i, done = 0;

            for (i = 0; i < m; i++) idx[i] = 0;
            while (!done) {
                int bag[MAXW], n = m + 1, a, na;

                for (i = 0; i < m; i++) bag[i] = nominal[idx[i]];
                bag[m] = v;
                qsort(bag, (size_t)n, sizeof(int), int_cmp);
                (*bags_tried)++;
                na = analyse(lv, bag, n, abuf, MAXA);
                for (a = 0; a < na; a++)
                    push_entry(&an_entries[lv], bag_code(bag, n),
                        analysis_key(&abuf[a], n));
                /* next multiset: non-decreasing idx */
                for (i = m - 1; i >= 0 && idx[i] == nn - 1; i--) {}
                if (i < 0) {
                    done = 1;
                } else {
                    int val = ++idx[i];
                    for (i++; i < m; i++) idx[i] = val;
                }
                if (m == 0) done = 1;
            }
        }
    }
}

static long level_pairs[NLEVELS][MAXW + 1];
static long level_strings[NLEVELS];
static long level_bags[NLEVELS];
static int  level_maxf[NLEVELS];

/* pairs by length, strings, bags and the largest |F| per level, from
 * the analyser's sets (pairs = distinct orders x |F| per bag) */
static void
sizes (void)
{
    int lv, n;

    for (lv = 0; lv < NLEVELS; lv++) {
        long i = 0;

        for (n = 0; n <= MAXW; n++) level_pairs[lv][n] = 0;
        level_strings[lv] = level_bags[lv] = 0;
        level_maxf[lv] = 0;
        while (i < an_entries[lv].n) {
            long j = i;
            int  bag[MAXW], nb;

            while (j < an_entries[lv].n && an_entries[lv].e[j].bag
                == an_entries[lv].e[i].bag)
                j++;
            decode_bag(an_entries[lv].e[i].bag, bag, &nb);
            level_pairs[lv][nb] += multinomial(bag, nb) * (j - i);
            level_strings[lv] += multinomial(bag, nb);
            level_bags[lv]++;
            if (j - i > level_maxf[lv]) level_maxf[lv] = (int)(j - i);
            i = j;
        }
    }
}

static void
part_world (void)
{
    int  lv, n;
    long gdups[NLEVELS], adups[NLEVELS], tried[NLEVELS];
    char msg[256];

    printf("\n--- generator and analyser ---\n");
    generate();
    printf("  generator: %ld abstract analyses (with repeats) up to %d words\n",
        gen_emitted, gmaxw);
    for (lv = 0; lv < NLEVELS; lv++) {
        long i;
        int  same;

        gdups[lv] = sort_unique(&gen_entries[lv]);
        tried[lv] = 0;
        analyse_all_bags(lv, &tried[lv]);
        adups[lv] = sort_unique(&an_entries[lv]);
        same = gen_entries[lv].n == an_entries[lv].n;
        for (i = 0; same && i < gen_entries[lv].n; i++)
            if (entry_cmp(&gen_entries[lv].e[i], &an_entries[lv].e[i]) != 0)
                same = 0;
        snprintf(msg, sizeof msg, "%s: generated == analysed, %ld positioned "
            "analyses on canonical strings (%ld bags tried), readings included",
            level_name[lv], an_entries[lv].n, tried[lv]);
        check(msg, same);
        snprintf(msg, sizeof msg, "%s: the analyser never yields an analysis "
            "twice", level_name[lv]);
        check(msg, adups[lv] == 0);
    }

    sizes();
    printf("\n  %-17s %8s %10s %8s %8s %10s %6s\n", "level", "bags",
        "strings", "pairs 3", "pairs 4", "pairs 5", "max|F|");
    for (lv = 0; lv < NLEVELS; lv++)
        printf("  %-17s %8ld %10ld %8ld %8ld %10ld %6d\n", level_name[lv],
            level_bags[lv], level_strings[lv], level_pairs[lv][3],
            level_pairs[lv][4], level_pairs[lv][5], level_maxf[lv]);
    {
        int same = 1;

        for (lv = 1; lv < NLEVELS; lv++)
            for (n = 0; n <= MAXW; n++)
                if (level_pairs[lv][n] != level_pairs[0][n]) same = 0;
        check("the population of pairs is the same at every level, length by "
            "length", same);
    }
    check("pairs by length = oracle_partb.py's (3: 384, 4: 55,296, 5: "
        "6,696,960; = task 0's count; a pin)", level_pairs[0][1] == 0
        && level_pairs[0][2] == 0 && level_pairs[0][6] == 0
        && level_pairs[0][3] == 384 && level_pairs[0][4] == 55296
        && (quick || level_pairs[0][5] == 6696960));
}

/* sig (position q holds canonical token sig[q]) is the first order of
 * its string, in the lexicographic order of perms, iff equal tokens keep
 * their canonical order; D131 found the same orders by a list of the
 * strings seen, which costs n!^2 per bag at six words */
static int
first_order (
    const int *bag,
    const int *sig,
    int        n)
{
    int q, r;

    for (q = 0; q < n; q++)
        for (r = q + 1; r < n; r++)
            if (bag[sig[q]] == bag[sig[r]] && sig[q] > sig[r]) return 0;
    return 1;
}

/* word order cannot change F(s): every distinct order of every bag */
static void
part_order (void)
{
    int  lv;
    long order_bad = 0;
    char msg[256];

    printf("\n--- word order and F(s) ---\n");
    for (lv = 0; lv < NLEVELS; lv++) {
        long i = 0, strings = 0, bad = 0;

        while (i < an_entries[lv].n) {
            long     j = i, s;
            int      bag[MAXW], nb, p, k;
            i64      seen[720];
            int      nseen = 0;
            Analysis canon[MAXA];
            int      nc;

            while (j < an_entries[lv].n && an_entries[lv].e[j].bag
                == an_entries[lv].e[i].bag)
                j++;
            decode_bag(an_entries[lv].e[i].bag, bag, &nb);
            nc = (int)(j - i);
            for (s = i; s < j; s++) decode_key(an_entries[lv].e[s].key,
                &canon[s - i], nb);
            for (p = 0; p < nperms[nb]; p++) {
                const int *sig = perms[nb][p];   /* new position q holds
                                                    canonical sig[q] */
                int        str[MAXW], inv[MAXW], dup = 0, na, a;
                i64        code, got[MAXA], want[MAXA];

                for (k = 0; k < nb; k++) {
                    str[k] = bag[sig[k]];
                    inv[sig[k]] = k;
                }
                code = bag_code(str, nb);
                for (k = 0; k < nseen; k++)
                    if (seen[k] == code) dup = 1;
                if (first_order(bag, sig, nb) != !dup) order_bad++;
                if (dup) continue;
                seen[nseen++] = code;
                strings++;
                na = analyse(lv, str, nb, abuf, MAXA);
                for (a = 0; a < na; a++) got[a] = analysis_key(&abuf[a], nb);
                for (a = 0; a < nc; a++) {
                    Analysis m;
                    for (k = 0; k < nb; k++) {
                        int h = canon[a].head[k];
                        m.head[inv[k]] = h == 0 ? 0 : inv[h - 1] + 1;
                        m.rd[inv[k]] = canon[a].rd[k];
                    }
                    want[a] = analysis_key(&m, nb);
                }
                if (na != nc) {
                    bad++;
                    continue;
                }
                qsort(got, (size_t)na, sizeof(i64), i64_cmp);
                qsort(want, (size_t)nc, sizeof(i64), i64_cmp);
                if (memcmp(got, want, (size_t)na * sizeof(i64)) != 0) bad++;
            }
            i = j;
        }
        snprintf(msg, sizeof msg, "%s: F(s) of every one of %ld strings == "
            "its bag's F carried to that order", level_name[lv], strings);
        check(msg, bad == 0 && strings == level_strings[lv]);
    }
    check("first_order (equal tokens keep their canonical order) picks "
        "exactly the orders D131's list of strings seen picks, every order "
        "of every bag at every level", order_bad == 0);
}

/* ================================================================
 * Readers and hand-worked sentences
 * ================================================================ */

/* P(s), M(s), R_P(s) as membership flags over the analyses of s */
typedef struct {
    int nf, np, nm, nrp;
    int in_p[MAXA], in_m[MAXA], in_rp[MAXA];
} Readers;

static void
read_with (
    const Analysis *a,
    int             na,
    int             n,
    Readers        *r)
{
    int k;

    r->nf = na;
    r->np = r->nm = r->nrp = 0;
    for (k = 0; k < na; k++) {
        int ix = tree_index(a[k].head, n);

        if (!t_valid[n][ix]) fatal("analysis is not a tree");
        r->in_p[k] = t_proj[n][ix];
        r->in_m[k] = t_mild[n][ix];
        r->np += r->in_p[k];
        r->nm += r->in_m[k];
    }
    for (k = 0; k < na; k++) {
        r->in_rp[k] = r->np > 0 ? r->in_p[k] : 1;
        r->nrp += r->in_rp[k];
    }
}

/* cells forced over the analyses selected by sel (NULL = all) */
static void
forced_cells (
    const Analysis *a,
    int             na,
    const int      *sel,
    int             n,
    int            *attach,
    int            *reading)
{
    int i, k, first;

    *attach = *reading = 0;
    for (i = 0; i < n; i++) {
        int h = -1, rd = -1, fa = 1, fr = 1;

        first = 1;
        for (k = 0; k < na; k++) {
            if (sel != NULL && !sel[k]) continue;
            if (first) {
                h = a[k].head[i];
                rd = a[k].rd[i];
                first = 0;
                continue;
            }
            if (a[k].head[i] != h) fa = 0;
            if (a[k].rd[i] != rd) fr = 0;
        }
        if (first) fa = fr = 0;
        *attach += fa;
        *reading += fr;
    }
}

typedef struct {
    const char *title;
    int         lv;
    int         n;
    int         rd[MAXW];
    int         head[MAXW];
    const char *text;          /* the string at that level (NULL: L1) */
    int         nf, np, nm;
    int         truth_read;    /* truth in R_P(s) */
    int         truth_read_m;  /* truth in R_M(s) */
    int         attach, reading;   /* forced cells under F */
    int         attach_p;      /* forced attachment cells under P (-1: P
                                  empty) */
} Hand;

static void
part_hand (void)
{
    Hand h[9];
    int  k, i;

    printf("\n--- hand-worked sentences ---\n");
    memset(h, 0, sizeof h);
    /* bonum (m acc sg) displaced across puella's arc: unambiguous */
    h[0].title = "bonum puella dominum amat (bonum -> dominum, crossed)";
    h[0].lv = 0; h[0].n = 4; h[0].text = "bonum puella dominum amat";
    h[0].rd[0] = rd_adj(0, MASC, ACC, SG); h[0].head[0] = 3;
    h[0].rd[1] = rd_noun(0, NOM, SG);      h[0].head[1] = 4;
    h[0].rd[2] = rd_noun(1, ACC, SG);      h[0].head[2] = 4;
    h[0].rd[3] = rd_verb(0);               h[0].head[3] = 0;
    h[0].nf = 1; h[0].np = 0; h[0].nm = 1; h[0].truth_read = 1;
    h[0].attach = 4; h[0].reading = 4;
    h[0].truth_read_m = 1; h[0].attach_p = -1;
    /* the same words, nested */
    h[1].title = "puella bonum dominum amat (nested)";
    h[1].lv = 0; h[1].n = 4; h[1].text = "puella bonum dominum amat";
    h[1].rd[0] = rd_noun(0, NOM, SG);      h[1].head[0] = 4;
    h[1].rd[1] = rd_adj(0, MASC, ACC, SG); h[1].head[1] = 3;
    h[1].rd[2] = rd_noun(1, ACC, SG);      h[1].head[2] = 4;
    h[1].rd[3] = rd_verb(0);               h[1].head[3] = 0;
    h[1].nf = 1; h[1].np = 1; h[1].nm = 1; h[1].truth_read = 1;
    h[1].attach = 4; h[1].reading = 4;
    h[1].truth_read_m = 1; h[1].attach_p = 4;
    /* bona (n acc pl) displaced: syncretic, a nested misreading exists */
    h[2].title = "bona puella dona amat (bona -> dona, crossed: garden path)";
    h[2].lv = 0; h[2].n = 4; h[2].text = "bona puella dona amat";
    h[2].rd[0] = rd_adj(0, NEUT, ACC, PL); h[2].head[0] = 3;
    h[2].rd[1] = rd_noun(0, NOM, SG);      h[2].head[1] = 4;
    h[2].rd[2] = rd_noun(2, ACC, PL);      h[2].head[2] = 4;
    h[2].rd[3] = rd_verb(0);               h[2].head[3] = 0;
    h[2].nf = 2; h[2].np = 1; h[2].nm = 2; h[2].truth_read = 0;
    h[2].attach = 3; h[2].reading = 3;
    h[2].truth_read_m = 1; h[2].attach_p = 4;
    /* the same analysis at L1: the ending says which noun */
    h[3] = h[2];
    h[3].title = "the same analysis at L1 (ideal endings): no garden path";
    h[3].lv = 1; h[3].text = NULL;
    h[3].nf = 1; h[3].np = 0; h[3].nm = 1; h[3].truth_read = 1;
    h[3].attach = 4; h[3].reading = 4;
    h[3].truth_read_m = 1; h[3].attach_p = -1;
    /* neuter nominative = accusative: readings open, attachments forced */
    h[4].title = "donum donum amat (which donum is the subject?)";
    h[4].lv = 0; h[4].n = 3; h[4].text = "donum donum amat";
    h[4].rd[0] = rd_noun(2, NOM, SG);      h[4].head[0] = 3;
    h[4].rd[1] = rd_noun(2, ACC, SG);      h[4].head[1] = 3;
    h[4].rd[2] = rd_verb(0);               h[4].head[2] = 0;
    h[4].nf = 2; h[4].np = 2; h[4].nm = 2; h[4].truth_read = 1;
    h[4].attach = 3; h[4].reading = 1;
    h[4].truth_read_m = 1; h[4].attach_p = 3;
    /* case stripped: subject and object trade places */
    h[5].title = "puella dominus amat at L2 (puella dominum amat at L0)";
    h[5].lv = 2; h[5].n = 3; h[5].text = "puella dominus amat";
    h[5].rd[0] = rd_noun(0, NOM, SG);      h[5].head[0] = 3;
    h[5].rd[1] = rd_noun(1, ACC, SG);      h[5].head[1] = 3;
    h[5].rd[2] = rd_verb(0);               h[5].head[2] = 0;
    h[5].nf = 2; h[5].np = 2; h[5].nm = 2; h[5].truth_read = 1;
    h[5].attach = 3; h[5].reading = 1;
    h[5].truth_read_m = 1; h[5].attach_p = 3;
    h[6] = h[5];
    h[6].title = "puella dominum amat at L0";
    h[6].lv = 0; h[6].text = "puella dominum amat";
    h[6].nf = 1; h[6].np = 1; h[6].nm = 1; h[6].attach = 3; h[6].reading = 3;
    /* neuter plurals (the registered dona pair): bona's head is open
     * under F, forced under P - one cell of A */
    h[7].title = "amant bona dona dona (bona -> first dona; subject and "
        "object neuter plurals)";
    h[7].lv = 0; h[7].n = 4; h[7].text = "amant bona dona dona";
    h[7].rd[0] = rd_verb(1);               h[7].head[0] = 0;
    h[7].rd[1] = rd_adj(0, NEUT, NOM, PL); h[7].head[1] = 3;
    h[7].rd[2] = rd_noun(2, NOM, PL);      h[7].head[2] = 1;
    h[7].rd[3] = rd_noun(2, ACC, PL);      h[7].head[3] = 1;
    h[7].nf = 4; h[7].np = 2; h[7].nm = 4; h[7].truth_read = 1;
    h[7].truth_read_m = 1; h[7].attach = 3; h[7].reading = 1;
    h[7].attach_p = 4;
    /* two adjectives crossing to two nouns: not well-nested, and the
     * string has mild alternatives - R_M misreads it too */
    h[8].title = "amant bona bona dona dona (bona -> dona, bona -> dona, "
        "interleaved: not mild)";
    h[8].lv = 0; h[8].n = 5; h[8].text = "amant bona bona dona dona";
    h[8].rd[0] = rd_verb(1);               h[8].head[0] = 0;
    h[8].rd[1] = rd_adj(0, NEUT, NOM, PL); h[8].head[1] = 4;
    h[8].rd[2] = rd_adj(0, NEUT, ACC, PL); h[8].head[2] = 5;
    h[8].rd[3] = rd_noun(2, NOM, PL);      h[8].head[3] = 1;
    h[8].rd[4] = rd_noun(2, ACC, PL);      h[8].head[4] = 1;
    h[8].nf = 8; h[8].np = 2; h[8].nm = 6; h[8].truth_read = 0;
    h[8].truth_read_m = 0; h[8].attach = 3; h[8].reading = 1;
    h[8].attach_p = 5;

    for (k = 0; k < 9; k++) {
        const Hand *x = &h[k];
        int         str[MAXW], na, t = -1, fa, fr, ok, fap = -1, frp, rm;
        char        text[200], msg[400];
        Analysis    truth;
        Readers     r;

        text[0] = '\0';
        for (i = 0; i < x->n; i++) {
            str[i] = form_of[x->lv][x->rd[i]];
            truth.head[i] = x->head[i];
            truth.rd[i] = x->rd[i];
            if (i > 0) strcat(text, " ");
            strcat(text, form_text[x->lv][str[i]]);
        }
        na = analyse(x->lv, str, x->n, abuf, MAXA);
        for (i = 0; i < na; i++)
            if (analysis_key(&abuf[i], x->n) == analysis_key(&truth, x->n))
                t = i;
        read_with(abuf, na, x->n, &r);
        forced_cells(abuf, na, NULL, x->n, &fa, &fr);
        if (r.np > 0) forced_cells(abuf, na, r.in_p, x->n, &fap, &frp);
        rm = t >= 0 && (r.nm > 0 ? r.in_m[t] : 1);
        ok = (x->text == NULL || strcmp(text, x->text) == 0) && t >= 0
            && r.nf == x->nf && r.np == x->np && r.nm == x->nm
            && r.in_rp[t] == x->truth_read && rm == x->truth_read_m
            && fa == x->attach && fr == x->reading && fap == x->attach_p;
        snprintf(msg, sizeof msg, "%s: |F| %d, |P| %d, |M| %d, truth %s by "
            "R_P and %s by R_M, forced cells under F %d attachment / %d "
            "reading, under P %d attachment", x->title, r.nf, r.np, r.nm,
            t >= 0 && r.in_rp[t] ? "read" : "MISREAD", rm ? "read"
            : "MISREAD", fa, fr, fap);
        check(msg, ok);
    }
}

/* ================================================================
 * Part B: licensing cost A and garden paths B
 * ================================================================ */

enum { DC_ADJ, DC_GEN, DC_MIX, NDC };    /* what the displaced words are */

static const char *const dc_name[NDC] = {
    "adjectives only", "genitives only", "both"
};

typedef struct {
    s64 pairs;
    s64 a_num, a_den, ar_num;            /* projective pairs */
    s64 b_num, b_den;                    /* crossed pairs: misread by R_P */
    s64 bs_num, bs_den, bu_num, bu_den;  /* displaced word syncretic or not */
    s64 bm_num;                          /* misread by R_M */
    s64 len_a_num[MAXW + 1], len_a_den[MAXW + 1];
    s64 len_b_num[MAXW + 1], len_b_den[MAXW + 1];
    s64 cls_num[NDC], cls_den[NDC];      /* descriptive, not registered */
    s64 cls_s_num[NDC], cls_s_den[NDC];  /* ... a syncretic displaced word */
    s64 cls_u_num[NDC], cls_u_den[NDC];  /* ... all unambiguous */
    s64 vdisp;                           /* crossed pairs displacing a verb
                                          * dependent (0: 58.2) */
    s64 mild_num[2], mild_den[2];        /* crossed pairs, [1] = mild */
    s64 cr_num[5], cr_den[5];            /* by crossings 1..3, [4] = 4+ */
    s64 open_f, open_p;                  /* projective pairs: open cells
                                          * under F, under P (A = diff) */
    s64 f_proj, f_cross;                 /* sum of |F(s)| over the pairs */
} Measures;

static Measures meas[NLEVELS];
static Analysis mapped[MAXA];
static Readers  rdr;

static void
measure_level (
    int       lv,
    Measures *ms)
{
    long i = 0;

    memset(ms, 0, sizeof *ms);
    while (i < an_entries[lv].n) {
        long     j = i, s;
        int      bag[MAXW], nb, p, k, a;
        i64      seen[720];
        int      nseen = 0, nc;
        Analysis canon[MAXA];

        while (j < an_entries[lv].n && an_entries[lv].e[j].bag
            == an_entries[lv].e[i].bag)
            j++;
        decode_bag(an_entries[lv].e[i].bag, bag, &nb);
        nc = (int)(j - i);
        for (s = i; s < j; s++)
            decode_key(an_entries[lv].e[s].key, &canon[s - i], nb);
        for (p = 0; p < nperms[nb]; p++) {
            const int *sig = perms[nb][p];
            int        str[MAXW], inv[MAXW], dup = 0, faf, frf, fap = 0, frp = 0;
            i64        code;

            for (k = 0; k < nb; k++) {
                str[k] = bag[sig[k]];
                inv[sig[k]] = k;
            }
            code = bag_code(str, nb);
            for (k = 0; k < nseen; k++)
                if (seen[k] == code) dup = 1;
            if (dup) continue;
            seen[nseen++] = code;
            for (a = 0; a < nc; a++)
                for (k = 0; k < nb; k++) {
                    int h = canon[a].head[k];
                    mapped[a].head[inv[k]] = h == 0 ? 0 : inv[h - 1] + 1;
                    mapped[a].rd[inv[k]] = canon[a].rd[k];
                }
            read_with(mapped, nc, nb, &rdr);
            forced_cells(mapped, nc, NULL, nb, &faf, &frf);
            if (rdr.np > 0)
                forced_cells(mapped, nc, rdr.in_p, nb, &fap, &frp);
            for (a = 0; a < nc; a++) {
                int ix = tree_index(mapped[a].head, nb);

                ms->pairs++;
                if (rdr.in_p[a]) {
                    ms->a_num += fap - faf;
                    ms->ar_num += frp - frf;
                    ms->a_den += nb;
                    ms->len_a_num[nb] += fap - faf;
                    ms->len_a_den[nb] += nb;
                    ms->open_f += nb - faf;
                    ms->open_p += nb - fap;
                    ms->f_proj += nc;
                } else {
                    int mis = rdr.np > 0, mask = t_npmask[nb][ix];
                    int syn = 0, nadj = 0, ngen = 0, nother = 0, cls, cr;

                    ms->b_den++;
                    ms->b_num += mis;
                    ms->len_b_den[nb]++;
                    ms->len_b_num[nb] += mis;
                    ms->f_cross += nc;
                    for (k = 0; k < nb; k++) {
                        const Reading *x;

                        if (!(mask & (1 << k))) continue;
                        x = &readings[mapped[a].rd[k]];
                        if (form_nr[lv][str[k]] >= 2) syn = 1;
                        if (x->kind == K_ADJ) nadj++;
                        else if (x->kind == K_NOUN && mapped[a].head[k] != 0
                            && readings[mapped[a].rd[mapped[a].head[k] - 1]]
                            .kind == K_NOUN)
                            ngen++;
                        else nother++;
                    }
                    if (syn) {
                        ms->bs_den++;
                        ms->bs_num += mis;
                    } else {
                        ms->bu_den++;
                        ms->bu_num += mis;
                    }
                    if (!rdr.in_m[a] && rdr.nm > 0) ms->bm_num++;
                    if (nother > 0) ms->vdisp++;
                    cls = ngen == 0 ? DC_ADJ : nadj == 0 ? DC_GEN : DC_MIX;
                    ms->cls_den[cls]++;
                    ms->cls_num[cls] += mis;
                    if (syn) {
                        ms->cls_s_den[cls]++;
                        ms->cls_s_num[cls] += mis;
                    } else {
                        ms->cls_u_den[cls]++;
                        ms->cls_u_num[cls] += mis;
                    }
                    ms->mild_den[rdr.in_m[a] ? 1 : 0]++;
                    ms->mild_num[rdr.in_m[a] ? 1 : 0] += mis;
                    cr = t_cross[nb][ix] > 4 ? 4 : t_cross[nb][ix];
                    ms->cr_den[cr]++;
                    ms->cr_num[cr] += mis;
                }
            }
        }
        i = j;
    }
}

/* num / den as a percentage with two decimals, integer arithmetic */
static void
pct (
    char *buf,
    int   size,
    s64   num,
    s64   den)
{
    s64 x;

    if (den == 0) {
        snprintf(buf, (size_t)size, "-");
        return;
    }
    x = (num * 20000 + den) / (2 * den);   /* hundredths of a percent */
    snprintf(buf, (size_t)size, "%lld.%02lld%%", (long long)(x / 100),
        (long long)(x % 100));
}

/* oracle_partb.py: [quick][level] = a_num, a_den, ar_num, b_num, b_den,
 * bs_num, bs_den, bu_num, bu_den, bm_num */
static const s64 oracle_b[2][NLEVELS][10] = {
    { { 878568, 13631616, 79068, 2420352, 4018176, 758358, 1665120,
        1661994, 2353056, 383068 },
      { 884736, 13631616, 0, 2293760, 4018176, 0, 0, 2293760, 4018176,
        376832 },
      { 993296, 13631616, 37632, 3470872, 4018176, 3470872, 4018176, 0, 0,
        397112 },
      { 996352, 13631616, 29696, 3853312, 4018176, 3853312, 4018176, 0, 0,
        406528 } },
    { { 8208, 160896, 464, 8720, 15360, 2568, 5568, 6152, 9792, 0 },
      { 8192, 160896, 0, 8192, 15360, 0, 0, 8192, 15360, 0 },
      { 9760, 160896, 32, 12832, 15360, 12832, 15360, 0, 0, 0 },
      { 11264, 160896, 1024, 14336, 15360, 14336, 15360, 0, 0, 0 } },
};

/* oracle_partb.py, descriptive split: [quick][level] = for adjectives
 * only, genitives only, both: (num, den, syncretic num, den, unambiguous
 * num, den) */
static const s64 oracle_cls[2][NLEVELS][18] = {
    { { 43088, 663552, 42134, 578784, 954, 84768,
        2330624, 3108864, 673280, 869376, 1657344, 2239488,
        46640, 245760, 42944, 216960, 3696, 28800 },
      { 0, 663552, 0, 0, 0, 663552,
        2269184, 3108864, 0, 0, 2269184, 3108864,
        24576, 245760, 0, 0, 24576, 245760 },
      { 236864, 663552, 236864, 663552, 0, 0,
        3108864, 3108864, 3108864, 3108864, 0, 0,
        125144, 245760, 125144, 245760, 0, 0 },
      { 523264, 663552, 523264, 663552, 0, 0,
        3108864, 3108864, 3108864, 3108864, 0, 0,
        221184, 245760, 221184, 245760, 0, 0 } },
    { { 272, 3072, 264, 2496, 8, 576,
        8448, 12288, 2304, 3072, 6144, 9216, 0, 0, 0, 0, 0, 0 },
      { 0, 3072, 0, 0, 0, 3072,
        8192, 12288, 0, 0, 8192, 12288, 0, 0, 0, 0, 0, 0 },
      { 544, 3072, 544, 3072, 0, 0,
        12288, 12288, 12288, 12288, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 2048, 3072, 2048, 3072, 0, 0,
        12288, 12288, 12288, 12288, 0, 0, 0, 0, 0, 0, 0, 0 } },
};

/* oracle_partb.py: [quick][level] = open cells under F, under P
 * (projective pairs); B over mild crossed pairs (num, den), over the
 * others (num, den); B by crossings 1, 2, 3, 4+ (num, den each) */
static const s64 oracle_x[2][NLEVELS][14] = {
    { { 2209476, 1330908, 2264456, 3611648, 155896, 406528,
        1232342, 1788928, 788644, 1332224, 349528, 691712, 49838, 205312 },
      { 1953792, 1069056, 2154496, 3611648, 139264, 406528,
        1185792, 1788928, 743424, 1332224, 323584, 691712, 40960, 205312 },
      { 9736704, 8743408, 3131120, 3611648, 339752, 406528,
        1606828, 1788928, 1131080, 1332224, 573400, 691712, 159564, 205312 },
      { 10017792, 9021440, 3476480, 3611648, 376832, 406528,
        1739264, 1788928, 1268224, 1332224, 651776, 691712, 194048,
        205312 } },
    { { 13464, 5256, 8720, 15360, 0, 0, 6476, 10240, 2244, 5120, 0, 0, 0, 0 },
      { 12288, 4096, 8192, 15360, 0, 0, 6144, 10240, 2048, 5120, 0, 0, 0, 0 },
      { 68400, 58640, 12832, 15360, 0, 0, 8600, 10240, 4232, 5120, 0, 0, 0,
        0 },
      { 70656, 59392, 14336, 15360, 0, 0, 9728, 10240, 4608, 5120, 0, 0, 0,
        0 } },
};

/* the sums of |F(s)| over projective and over crossed pairs: [quick]
 * [level]; from oracle_partb.py with one line added (each pair adds
 * len(an) to f_proj or f_cross), D134 task 0 - mean |F| exactly */
static const s64 oracle_f[2][NLEVELS][2] = {
    { { 8549790, 17011152 }, { 6752640, 14032896 },
      { 182622792, 271261368 }, { 188821056, 285760512 } },
    { { 59118, 31824 }, { 52608, 27648 },
      { 350832, 153648 }, { 353856, 156672 } },
};

static void
part_measures (void)
{
    int  lv, n, c;
    char msg[400], s1[32], s2[32], s3[32], s4[32];

    printf("\n--- Part B: licensing cost A and garden paths B ---\n");
    printf("  A = attachment cells open under F but forced under P, over "
        "projective pairs,\n      as a share of their cells (every word's, "
        "the verb's included)\n");
    printf("  B = crossed pairs misread by R_P (nested first)\n\n");
    for (lv = 0; lv < NLEVELS; lv++) measure_level(lv, &meas[lv]);

    printf("  %-17s %10s %10s %10s %10s %10s\n", "level", "A", "A reading",
        "B", "B R_M", "mean |F|");
    for (lv = 0; lv < NLEVELS; lv++) {
        const Measures *m = &meas[lv];
        char            f[32];

        pct(s1, 32, m->a_num, m->a_den);
        pct(s2, 32, m->ar_num, m->a_den);
        pct(s3, 32, m->b_num, m->b_den);
        pct(s4, 32, m->bm_num, m->b_den);
        {
            s64 h = ((m->f_proj + m->f_cross) * 200 + m->pairs)
                / (2 * m->pairs);              /* hundredths, rounded */
            snprintf(f, sizeof f, "%lld.%02lld", (long long)(h / 100),
                (long long)(h % 100));
        }
        printf("  %-17s %10s %10s %10s %10s %10s\n", level_name[lv], s1, s2,
            s3, s4, f);
    }
    printf("\n  exact: A = num / %lld (projective pairs' cells), B = num / %lld "
        "(crossed pairs)\n", (long long)meas[0].a_den, (long long)meas[0].b_den);
    for (lv = 0; lv < NLEVELS; lv++)
        printf("  %-17s A %lld, B %lld\n", level_name[lv],
            (long long)meas[lv].a_num, (long long)meas[lv].b_num);

    printf("\n  B by the displaced word's form (registered split, Q2)\n");
    for (lv = 0; lv < NLEVELS; lv++) {
        const Measures *m = &meas[lv];

        pct(s1, 32, m->bs_num, m->bs_den);
        pct(s2, 32, m->bu_num, m->bu_den);
        printf("  %-17s syncretic %8s of %8lld   unambiguous %8s of %8lld\n",
            level_name[lv], s1, (long long)m->bs_den, s2,
            (long long)m->bu_den);
    }
    printf("\n  B by what is displaced (descriptive, NOT registered; each "
        "class split by the displaced words' forms)\n");
    for (c = 0; c < NDC; c++)
        for (lv = 0; lv < NLEVELS; lv++) {
            const Measures *m = &meas[lv];

            pct(s1, 32, m->cls_num[c], m->cls_den[c]);
            pct(s2, 32, m->cls_s_num[c], m->cls_s_den[c]);
            pct(s3, 32, m->cls_u_num[c], m->cls_u_den[c]);
            printf("  %-16s %-17s %8s of %8lld  syncretic %8s of %8lld  "
                "unambiguous %8s of %8lld\n", dc_name[c], level_name[lv], s1,
                (long long)m->cls_den[c], s2, (long long)m->cls_s_den[c], s3,
                (long long)m->cls_u_den[c]);
        }
    printf("\n  B by stratum (registered as reported, not predicted): mild / "
        "not mild; crossings 1, 2, 3, 4+ (root arc included)\n");
    for (lv = 0; lv < NLEVELS; lv++) {
        const Measures *m = &meas[lv];

        pct(s1, 32, m->mild_num[1], m->mild_den[1]);
        pct(s2, 32, m->mild_num[0], m->mild_den[0]);
        printf("  %-17s mild %s of %lld, not mild %s of %lld;", level_name[lv],
            s1, (long long)m->mild_den[1], s2, (long long)m->mild_den[0]);
        for (c = 1; c <= 4; c++) {
            pct(s1, 32, m->cr_num[c], m->cr_den[c]);
            printf(" %d: %s of %lld", c, s1, (long long)m->cr_den[c]);
        }
        printf("\n");
    }
    printf("\n  the open cells behind A (projective pairs): open under F, "
        "under P\n");
    for (lv = 0; lv < NLEVELS; lv++)
        printf("  %-17s %lld, %lld (difference %lld = A's numerator)\n",
            level_name[lv], (long long)meas[lv].open_f,
            (long long)meas[lv].open_p,
            (long long)(meas[lv].open_f - meas[lv].open_p));
    printf("\n  by length (A; B)\n");
    for (n = 3; n <= gmaxw; n++) {
        printf("  %d words:", n);
        for (lv = 0; lv < NLEVELS; lv++) {
            pct(s1, 32, meas[lv].len_a_num[n], meas[lv].len_a_den[n]);
            pct(s2, 32, meas[lv].len_b_num[n], meas[lv].len_b_den[n]);
            printf("  %s %s; %s", lv == 0 ? "L0" : lv == 1 ? "L1" : lv == 2
                ? "L2" : "L3", s1, s2);
        }
        printf("\n");
    }

    {
        int same = 1, q = quick ? 1 : 0, ok = 1;

        for (lv = 1; lv < NLEVELS; lv++)
            if (meas[lv].a_den != meas[0].a_den
                || meas[lv].b_den != meas[0].b_den)
                same = 0;
        check("A and B have one denominator each at every level (the "
            "populations do not depend on the endings)", same);
        for (lv = 0; lv < NLEVELS; lv++) {
            const Measures *m = &meas[lv];
            const s64      *o = oracle_b[q][lv];

            if (m->a_num != o[0] || m->a_den != o[1] || m->ar_num != o[2]
                || m->b_num != o[3] || m->b_den != o[4] || m->bs_num != o[5]
                || m->bs_den != o[6] || m->bu_num != o[7] || m->bu_den != o[8]
                || m->bm_num != o[9])
                ok = 0;
        }
        snprintf(msg, sizeof msg, "A, A (reading), B, B split, B (R_M) at "
            "every level == oracle_partb.py (independent, tree-first; a pin, "
            "%s)", quick ? "four words" : "five words");
        check(msg, ok);
        check("L1 has no syncretic form, so no reading-cell cost and no "
            "syncretic displaced word", meas[1].ar_num == 0
            && meas[1].bs_den == 0);
        ok = 1;
        for (lv = 0; lv < NLEVELS; lv++) {
            const Measures *m = &meas[lv];
            const s64      *o = oracle_cls[q][lv];

            for (c = 0; c < NDC; c++)
                if (m->cls_num[c] != o[6 * c] || m->cls_den[c] != o[6 * c + 1]
                    || m->cls_s_num[c] != o[6 * c + 2]
                    || m->cls_s_den[c] != o[6 * c + 3]
                    || m->cls_u_num[c] != o[6 * c + 4]
                    || m->cls_u_den[c] != o[6 * c + 5])
                    ok = 0;
        }
        check("descriptive split (adjectives only / genitives only / both, "
            "each by form) == oracle_partb.py (a pin)", ok);
        ok = 1;
        for (lv = 0; lv < NLEVELS; lv++) {
            const Measures *m = &meas[lv];
            const s64      *o = oracle_x[q][lv];

            if (m->open_f != o[0] || m->open_p != o[1]
                || m->mild_num[1] != o[2] || m->mild_den[1] != o[3]
                || m->mild_num[0] != o[4] || m->mild_den[0] != o[5])
                ok = 0;
            for (c = 1; c <= 4; c++)
                if (m->cr_num[c] != o[4 + 2 * c] || m->cr_den[c] != o[5 + 2 * c])
                    ok = 0;
        }
        check("open cells under F and P, B by mildness and by crossing count "
            "== oracle_partb.py (a pin)", ok);
        ok = 1;
        for (lv = 0; lv < NLEVELS; lv++)
            if (meas[lv].f_proj != oracle_f[q][lv][0]
                || meas[lv].f_cross != oracle_f[q][lv][1])
                ok = 0;
        check("mean |F|: the sums of |F(s)| over projective and over crossed "
            "pairs at every level == oracle_partb.py's (one line added; a "
            "pin)", ok);
        ok = 1;
        for (lv = 0; lv < NLEVELS; lv++)
            if (meas[lv].vdisp != 0 || meas[lv].open_f - meas[lv].open_p
                != meas[lv].a_num)
                ok = 0;
        check("no crossed pair displaces a verb dependent, counted directly "
            "(the root word dominates every word); open F - open P == A's "
            "numerator", ok);
    }
}

static double
seconds (
    clock_t t0)
{
    return (double)(clock() - t0) / (double)CLOCKS_PER_SEC;
}

/* ================================================================
 * Umbrae, width and depth; competition and retrieval (task 1)
 * ================================================================ */

enum { U_DANGLING, U_SLOT, U_ROOT };

typedef struct {
    int type;
    int from, to;           /* opened at word from (0: boundary 0, the
                               root umbra), closed at word to; open at
                               the boundaries t with from <= t < to */
    int word;               /* the dependent; the verb for the root */
} Umbra;

typedef struct {
    int   nu, nback, width;
    Umbra u[MAXW + 1];
    int   depth[MAXW + 1];      /* by the left-to-right sweep */
    int   umbra_of[MAXW + 1];   /* word (1-based) -> its arc's umbra; the
                                   verb -> the root umbra; -1: a backward
                                   attachment */
} Umbrae;

static int
relation_of (
    const Analysis *a,
    int             d)
{
    const Reading *x = &readings[a->rd[d - 1]];
    int            h = a->head[d - 1];

    if (h == 0) return R_ROOT;
    if (x->kind == K_ADJ) return R_AMOD;
    if (readings[a->rd[h - 1]].kind == K_NOUN) return R_NMOD;
    if (x->cas == NOM) return R_NSUBJ;
    if (x->cas == ACC) return R_OBJ;
    if (x->cas == DAT) return R_IOBJ;
    return R_OBL;
}

/* required relations: nsubj, obj, and iobj (which only dat / dant take) */
static int
required (
    int r)
{
    return r == R_NSUBJ || r == R_OBJ || r == R_IOBJ;
}

/* the umbrae of a pair: heads and readings only, no level */
static void
build_umbrae (
    const Analysis *a,
    int             n,
    Umbrae         *u)
{
    int d, t, j, k, i, c, m;
    int open[MAXW + 1], nopen = 0;

    u->nu = u->nback = u->width = 0;
    for (d = 1; d <= n; d++) {
        int    h = a->head[d - 1];
        Umbra *x = &u->u[u->nu];

        if (h == 0) {
            x->type = U_ROOT; x->from = 0; x->to = d;
        } else if (d < h) {
            x->type = U_DANGLING; x->from = d; x->to = h;
        } else if (required(relation_of(a, d))) {
            x->type = U_SLOT; x->from = h; x->to = d;
        } else {
            u->umbra_of[d] = -1;     /* backward attachment */
            u->nback++;
            continue;
        }
        x->word = d;
        u->depth[u->nu] = -1;
        u->umbra_of[d] = u->nu++;
    }
    for (t = 0; t < n; t++) {
        c = 0;
        for (k = 0; k < u->nu; k++)
            if (u->u[k].from <= t && t < u->u[k].to) c++;
        if (c > u->width) u->width = c;
    }
    /* depth by a sweep: when word j closes an umbra, count the umbrae
     * still open that were opened after it and outlive j */
    for (k = 0; k < u->nu; k++)
        if (u->u[k].from == 0) open[nopen++] = k;
    for (j = 1; j <= n; j++) {
        for (k = 0; k < nopen; k++) {
            const Umbra *x = &u->u[open[k]];

            if (x->to != j) continue;
            c = 0;
            for (i = 0; i < nopen; i++) {
                const Umbra *y = &u->u[open[i]];
                if (y->from > x->from && y->to > j) c++;
            }
            u->depth[open[k]] = c;
        }
        m = 0;
        for (k = 0; k < nopen; k++)
            if (u->u[open[k]].to != j) open[m++] = open[k];
        nopen = m;
        for (k = 0; k < u->nu; k++)
            if (u->u[k].from == j) open[nopen++] = k;
    }
}

/* depth, second route: the umbra arcs opened strictly inside umbra k's
 * span that outlive it (the arcs that cross it, later-opened) */
static int
depth_pairwise (
    const Umbrae *u,
    int           k)
{
    int i, c = 0;

    for (i = 0; i < u->nu; i++)
        if (u->u[k].from < u->u[i].from && u->u[i].from < u->u[k].to
            && u->u[i].to > u->u[k].to)
            c++;
    return c;
}

/* compatibility: some reading of form x and some reading of form y
 * satisfy relation r, x the dependent, y the head (local: no global
 * constraint) */
static unsigned char comp_rel[NLEVELS][R_AMOD + 1][MAXF][MAXF];
static unsigned char comp_any[NLEVELS][MAXF][MAXF];

static int
rel_ok (
    const Reading *x,
    const Reading *y,
    int            r)
{
    if (r == R_AMOD)
        return x->kind == K_ADJ && y->kind == K_NOUN && x->cas == y->cas
            && x->gender == y->gender && x->number == y->number;
    if (x->kind != K_NOUN) return 0;
    if (r == R_NMOD) return x->cas == GEN && y->kind == K_NOUN;
    if (y->kind != K_VERB) return 0;
    if (r == R_NSUBJ) return x->cas == NOM && x->number == y->number;
    if (r == R_OBJ) return x->cas == ACC;
    if (r == R_IOBJ) return x->cas == DAT && verbs[y->lemma].ditrans;
    if (r == R_OBL) return x->cas == ABL;
    return 0;
}

static void
build_comp (void)
{
    int lv, fx, fy, i, j, r;

    memset(comp_rel, 0, sizeof comp_rel);
    memset(comp_any, 0, sizeof comp_any);
    for (lv = 0; lv < NLEVELS; lv++)
        for (fx = 0; fx < n_forms[lv]; fx++)
            for (fy = 0; fy < n_forms[lv]; fy++)
                for (i = 0; i < form_nr[lv][fx]; i++)
                    for (j = 0; j < form_nr[lv][fy]; j++)
                        for (r = R_NSUBJ; r <= R_AMOD; r++)
                            if (rel_ok(&readings[form_rd[lv][fx][i]],
                                    &readings[form_rd[lv][fy][j]], r)) {
                                comp_rel[lv][r][fx][fy] = 1;
                                comp_any[lv][fx][fy] = 1;
                            }
}

/* w in d's subtree (w == d included), by walking up from w */
static int
descends (
    const Analysis *a,
    int             w,
    int             d)
{
    int steps = 0;

    while (w != 0 && steps++ <= MAXW) {
        if (w == d) return 1;
        w = a->head[w - 1];
    }
    return 0;
}

/* yields as bitmasks (bit k = word k + 1), second route for subtrees and
 * for the displaced arcs */
static void
yields (
    const Analysis *a,
    int             n,
    int            *yl)
{
    int d, pass;

    for (d = 1; d <= n; d++) yl[d] = 1 << (d - 1);
    for (pass = 0; pass < n; pass++)
        for (d = 1; d <= n; d++)
            if (a->head[d - 1] != 0) yl[a->head[d - 1]] |= yl[d];
}

typedef struct {
    int nd;
    int word[MAXW];             /* displaced dependents, 1-based, in order */
    int dep_first[MAXW];
    int competed[MAXW], retrieved[MAXW];
    int competed_rel[MAXW], retrieved_rel[MAXW];   /* own relation only */
    int competed_blind[MAXW];   /* any noun as near: morphology-blind */
} Labels;

/* competition and retrieval of the displaced arcs (mask: bit d - 1) at
 * level lv */
static void
label_arcs (
    int             lv,
    const Analysis *a,
    int             n,
    int             mask,
    const Umbrae   *u,
    Labels         *lb)
{
    int f[MAXW], k, d;

    for (k = 0; k < n; k++) f[k] = form_of[lv][a->rd[k]];
    lb->nd = 0;
    for (d = 1; d <= n; d++) {
        int h, r, dist, w, c = 0, cr = 0, cb = 0, ret = 1, retr = 1;

        if (!(mask & (1 << (d - 1)))) continue;
        h = a->head[d - 1];
        r = relation_of(a, d);
        dist = h > d ? h - d : d - h;
        for (w = 1; w <= n; w++) {
            if (w == d || w == h || (w > d ? w - d : d - w) > dist) continue;
            if (descends(a, w, d)) continue;
            if (comp_any[lv][f[d - 1]][f[w - 1]]) c = 1;
            if (comp_rel[lv][r][f[d - 1]][f[w - 1]]) cr = 1;
            if (form_kind(lv, f[w - 1]) == K_NOUN) cb = 1;
        }
        if (d < h) {
            /* h arrives: the dangling umbrae open at boundary h - 1 that
             * could depend on h, beyond the ones h closes */
            for (k = 0; k < u->nu; k++) {
                const Umbra *x = &u->u[k];

                if (x->type != U_DANGLING || x->from > h - 1 || x->to <= h - 1
                    || x->to == h)
                    continue;
                if (comp_any[lv][f[x->word - 1]][f[h - 1]]) ret = 0;
                if (comp_rel[lv][r][f[x->word - 1]][f[h - 1]]) retr = 0;
            }
        } else {
            /* d arrives: the earlier words it could depend on, beyond h */
            for (w = 1; w < d; w++) {
                if (w == h) continue;
                if (comp_any[lv][f[d - 1]][f[w - 1]]) ret = 0;
                if (comp_rel[lv][r][f[d - 1]][f[w - 1]]) retr = 0;
            }
        }
        lb->word[lb->nd] = d;
        lb->dep_first[lb->nd] = d < h;
        lb->competed[lb->nd] = c;
        lb->competed_rel[lb->nd] = cr;
        lb->competed_blind[lb->nd] = cb;
        lb->retrieved[lb->nd] = ret;
        lb->retrieved_rel[lb->nd] = retr;
        lb->nd++;
    }
}

/* the checks of one pass over the pairs */
typedef struct {
    s64 pairs, crossed, disp, disp_df;
    s64 bad_count;       /* umbrae + backward attachments != arcs + 1 */
    s64 bad_depth;       /* sweep depth != pairwise depth */
    s64 bad_proj;        /* projective pair with an umbra of depth > 0 */
    s64 bad_rel;         /* a displaced arc neither amod nor nmod */
    s64 bad_disp;        /* displaced set != the yield route's */
    s64 bad_mono;        /* L0 RETRIEVED but L1 not, or L0 CLEAR but L1 not */
    s64 bad_tc;          /* the true partner not compatible (T in C, h in C) */
    s64 bad_sub;         /* competition by subtree walk != by yields */
    s64 bad_rt;          /* a root umbra missing or not the verb's */
} UChecks;

/* competition of one displaced arc again, the subtree excluded by yields */
static int
competed_by_yields (
    int             lv,
    const Analysis *a,
    int             n,
    const int      *yl,
    int             d)
{
    int h = a->head[d - 1];
    int dist = h > d ? h - d : d - h;
    int fd = form_of[lv][a->rd[d - 1]];
    int w;

    for (w = 1; w <= n; w++) {
        if (w == d || w == h || (w > d ? w - d : d - w) > dist) continue;
        if (yl[d] & (1 << (w - 1))) continue;
        if (comp_any[lv][fd][form_of[lv][a->rd[w - 1]]]) return 1;
    }
    return 0;
}

/* one pair: umbrae, the displaced arcs, labels at every level, checks */
static void
check_pair (
    const Analysis *a,
    int             n,
    UChecks        *ck)
{
    Umbrae u;
    Labels lb[NLEVELS];
    int    ix = tree_index(a->head, n), mask = t_npmask[n][ix], mask2 = 0;
    int    yl[MAXW + 1], k, d, lv, i, roots = 0;

    build_umbrae(a, n, &u);
    ck->pairs++;
    if (!t_proj[n][ix]) ck->crossed++;
    if (u.nu + u.nback != n) ck->bad_count++;
    for (k = 0; k < u.nu; k++) {
        if (u.depth[k] != depth_pairwise(&u, k)) ck->bad_depth++;
        if (t_proj[n][ix] && u.depth[k] != 0) ck->bad_proj++;
        if (u.u[k].type == U_ROOT) {
            roots++;
            if (a->head[u.u[k].word - 1] != 0
                || readings[a->rd[u.u[k].word - 1]].kind != K_VERB)
                ck->bad_rt++;
        }
    }
    if (roots != 1) ck->bad_rt++;
    yields(a, n, yl);
    for (d = 1; d <= n; d++) {
        int h = a->head[d - 1], lo, hi, w;

        if (h == 0) continue;
        lo = h < d ? h : d;
        hi = h < d ? d : h;
        for (w = lo + 1; w < hi; w++)
            if (!(yl[h] & (1 << (w - 1)))) mask2 |= 1 << (d - 1);
        if ((mask & (1 << (d - 1))) && relation_of(a, d) != R_AMOD
            && relation_of(a, d) != R_NMOD)
            ck->bad_rel++;
    }
    if (mask2 != mask) ck->bad_disp++;
    for (lv = 0; lv < NLEVELS; lv++) {
        label_arcs(lv, a, n, mask, &u, &lb[lv]);
        for (i = 0; i < lb[lv].nd; i++) {
            int h, w;

            d = lb[lv].word[i];
            h = a->head[d - 1];
            if (competed_by_yields(lv, a, n, yl, d) != lb[lv].competed[i])
                ck->bad_sub++;
            if (lb[lv].dep_first[i]) {
                for (w = 1; w < h; w++)
                    if (a->head[w - 1] == h && !comp_any[lv][form_of[lv][
                        a->rd[w - 1]]][form_of[lv][a->rd[h - 1]]])
                        ck->bad_tc++;
            } else if (!comp_any[lv][form_of[lv][a->rd[d - 1]]][
                    form_of[lv][a->rd[h - 1]]]) {
                ck->bad_tc++;
            }
        }
    }
    for (i = 0; i < lb[0].nd; i++) {
        ck->disp++;
        ck->disp_df += lb[0].dep_first[i];
        if ((lb[0].retrieved[i] && !lb[1].retrieved[i])
            || (!lb[0].competed[i] && lb[1].competed[i])
            || (lb[0].retrieved_rel[i] && !lb[1].retrieved_rel[i])
            || (!lb[0].competed_rel[i] && lb[1].competed_rel[i]))
            ck->bad_mono++;
    }
}

/* every pair, once, by the L1 bags (multisets of readings): the labels of
 * one pair at all four levels side by side */
static void
umbra_pass (
    int      maxlen,
    UChecks *ck)
{
    long i = 0;

    memset(ck, 0, sizeof *ck);
    while (i < an_entries[1].n) {
        long     j = i, s;
        int      bag[MAXW], nb, p, k, a, nc;
        Analysis canon[MAXA];

        while (j < an_entries[1].n && an_entries[1].e[j].bag
            == an_entries[1].e[i].bag)
            j++;
        decode_bag(an_entries[1].e[i].bag, bag, &nb);
        if (nb > maxlen) {
            i = j;
            continue;
        }
        nc = (int)(j - i);
        for (s = i; s < j; s++)
            decode_key(an_entries[1].e[s].key, &canon[s - i], nb);
        for (p = 0; p < nperms[nb]; p++) {
            const int *sig = perms[nb][p];
            int        inv[MAXW];

            if (!first_order(bag, sig, nb)) continue;
            for (k = 0; k < nb; k++) inv[sig[k]] = k;
            for (a = 0; a < nc; a++) {
                for (k = 0; k < nb; k++) {
                    int h = canon[a].head[k];
                    mapped[a].head[inv[k]] = h == 0 ? 0 : inv[h - 1] + 1;
                    mapped[a].rd[inv[k]] = canon[a].rd[k];
                }
                check_pair(&mapped[a], nb, ck);
            }
        }
        i = j;
    }
}

static void
umbra_checks (
    const char    *where,
    const UChecks *ck,
    s64            pairs,
    s64            crossed)
{
    char msg[400];

    printf("  %s: %lld pairs, %lld crossed, %lld displaced arcs (%lld "
        "dependent first)\n", where, (long long)ck->pairs,
        (long long)ck->crossed, (long long)ck->disp, (long long)ck->disp_df);
    snprintf(msg, sizeof msg, "%s: the pass meets every pair once (%lld) and "
        "every crossed pair (%lld)", where, (long long)pairs,
        (long long)crossed);
    check(msg, ck->pairs == pairs && ck->crossed == crossed);
    snprintf(msg, sizeof msg, "%s: umbrae (root included) + backward "
        "attachments == arcs + 1, every pair; one root umbra, closed at the "
        "verb", where);
    check(msg, ck->bad_count == 0 && ck->bad_rt == 0);
    snprintf(msg, sizeof msg, "%s: depth by the sweep == depth by crossing "
        "later-opened umbra arcs, every umbra", where);
    check(msg, ck->bad_depth == 0);
    snprintf(msg, sizeof msg, "%s: a projective pair (root arc included) has "
        "depth 0 everywhere", where);
    check(msg, ck->bad_proj == 0);
    snprintf(msg, sizeof msg, "%s: displaced arcs are amod or nmod only "
        "(KC 58.2), and the per-arc set (tree table) == the yield route's",
        where);
    check(msg, ck->bad_rel == 0 && ck->bad_disp == 0);
    snprintf(msg, sizeof msg, "%s: no competitor lies in d's own subtree "
        "(competition with the subtree by walking up == by yields), every "
        "displaced arc at every level", where);
    check(msg, ck->bad_sub == 0);
    snprintf(msg, sizeof msg, "%s: the true partner is always a candidate "
        "(T in C dependent first, h in C head first), every level", where);
    check(msg, ck->bad_tc == 0);
    snprintf(msg, sizeof msg, "%s: exactly half the displaced arcs are "
        "dependent first (reversing a string keeps the pair, its displaced "
        "arcs and their kind, and swaps the two directions)", where);
    check(msg, 2 * ck->disp_df == ck->disp);
    snprintf(msg, sizeof msg, "%s: per arc, RETRIEVED at L0 => at L1 and "
        "CLEAR at L0 => at L1, the relation-specific variants too (L1 "
        "readings are a subset)", where);
    check(msg, ck->bad_mono == 0);
}

/* hand-worked sentences of plan.md: width, depths, labels, R_P */
typedef struct {
    const char *title;
    int         lv, n;
    int         rd[MAXW], head[MAXW];
    const char *text;
    int         width, root_depth, max_depth;
    int         nd;                 /* displaced arcs, in word order: */
    int         dword[MAXW];        /* the dependent */
    int         ddepth[MAXW];       /* its umbra's depth, -1: backward */
    int         dcomp[MAXW], dretr[MAXW];
    int         misread;            /* truth outside R_P(s) */
} HandU;

#define NHANDU 32
static HandU handu[NHANDU];
static int   n_handu = 0;

static HandU *
hand_new (
    const char *title,
    int         lv,
    const char *text,
    int         width,
    int         root_depth,
    int         max_depth,
    int         misread)
{
    HandU *x = &handu[n_handu++];

    memset(x, 0, sizeof *x);
    x->title = title;
    x->lv = lv;
    x->text = text;
    x->width = width;
    x->root_depth = root_depth;
    x->max_depth = max_depth;
    x->misread = misread;
    return x;
}

static void
hand_word (
    HandU *x,
    int    rd,
    int    head)
{
    x->rd[x->n] = rd;
    x->head[x->n] = head;
    x->n++;
}

static void
hand_disp (
    HandU *x,
    int    word,
    int    depth,
    int    competed,
    int    retrieved)
{
    x->dword[x->nd] = word;
    x->ddepth[x->nd] = depth;
    x->dcomp[x->nd] = competed;
    x->dretr[x->nd] = retrieved;
    x->nd++;
}

static void
hand_sentences_umbrae (void)
{
    HandU *x;

    n_handu = 0;
    /* 1: projective; also at L3 */
    x = hand_new("1. puella dominum amat", 0, "puella dominum amat", 3, 0, 0,
        0);
    hand_word(x, rd_noun(0, NOM, SG), 3);
    hand_word(x, rd_noun(1, ACC, SG), 3);
    hand_word(x, rd_verb(0), 0);
    handu[n_handu] = *x;
    x = &handu[n_handu++];
    x->title = "1. at L3 (puella dominus amat)";
    x->lv = 3;
    x->text = "puella dominus amat";
    x->misread = 0;
    /* 2: bonum -> dominum, gender keeps puella out */
    x = hand_new("2. bonum puella dominum amat", 0,
        "bonum puella dominum amat", 3, 0, 1, 0);
    hand_word(x, rd_adj(0, MASC, ACC, SG), 3);
    hand_word(x, rd_noun(0, NOM, SG), 4);
    hand_word(x, rd_noun(1, ACC, SG), 4);
    hand_word(x, rd_verb(0), 0);
    hand_disp(x, 1, 1, 0, 1);
    /* 3: D131's garden path */
    x = hand_new("3. bona puella dona amat", 0, "bona puella dona amat", 3, 0,
        1, 1);
    hand_word(x, rd_adj(0, NEUT, ACC, PL), 3);
    hand_word(x, rd_noun(0, NOM, SG), 4);
    hand_word(x, rd_noun(2, ACC, PL), 4);
    hand_word(x, rd_verb(0), 0);
    hand_disp(x, 1, 1, 1, 1);
    /* 4: crossed via the root arc */
    x = hand_new("4. regis puella amat dominum", 0,
        "regis puella amat dominum", 3, 1, 1, 1);
    hand_word(x, rd_noun(3, GEN, SG), 4);
    hand_word(x, rd_noun(0, NOM, SG), 3);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(1, ACC, SG), 3);
    hand_disp(x, 1, 0, 1, 1);
    /* 5: the golden line */
    x = hand_new("5. bona bonum amat puella dominum", 0,
        "bona bonum amat puella dominum", 4, 2, 2, 0);
    hand_word(x, rd_adj(0, FEM, NOM, SG), 4);
    hand_word(x, rd_adj(0, MASC, ACC, SG), 5);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(0, NOM, SG), 3);
    hand_word(x, rd_noun(1, ACC, SG), 3);
    hand_disp(x, 1, 2, 0, 1);
    hand_disp(x, 2, 0, 0, 1);
    /* 6: competed, not misread (P empty) */
    x = hand_new("6. bona amat puella dona", 0, "bona amat puella dona", 3, 1,
        1, 0);
    hand_word(x, rd_adj(0, NEUT, ACC, PL), 4);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(0, NOM, SG), 2);
    hand_word(x, rd_noun(2, ACC, PL), 2);
    hand_disp(x, 1, 0, 1, 1);
    /* 7: regis -> dominum, puella nearer */
    x = hand_new("7. puella regis amat dominum", 0,
        "puella regis amat dominum", 3, 1, 1, 1);
    hand_word(x, rd_noun(0, NOM, SG), 3);
    hand_word(x, rd_noun(3, GEN, SG), 4);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(1, ACC, SG), 3);
    hand_disp(x, 2, 0, 1, 1);
    /* 8: head first; regis a backward attachment */
    x = hand_new("8. dominum amat regis puella", 0,
        "dominum amat regis puella", 2, 0, 0, 1);
    hand_word(x, rd_noun(1, ACC, SG), 2);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(3, GEN, SG), 1);
    hand_word(x, rd_noun(0, NOM, SG), 2);
    hand_disp(x, 3, -1, 1, 1);
    /* 11: the competitor rule skips d's own subtree (re-check L1):
     * puellarum depends on regis and is nearer than dominum, but would
     * close a cycle; puella is too far, amat a verb: CLEAR. puella and
     * dominum both come earlier and could take regis: INTERFERED. Every
     * analysis crosses the root arc */
    x = hand_new("11. puella dominum amat regis puellarum", 0,
        "puella dominum amat regis puellarum", 3, 0, 0, 0);
    hand_word(x, rd_noun(0, NOM, SG), 3);
    hand_word(x, rd_noun(1, ACC, SG), 3);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(3, GEN, SG), 2);
    hand_word(x, rd_noun(0, GEN, PL), 4);
    hand_disp(x, 4, -1, 0, 0);
    /* six words (the analyser and the tree table take any length up to
     * MAXW, quick mode included), retrieval failing with competition clear. 9: dependent
     * first - when dominum arrives, the open subject puellae (nom pl) has
     * a genitive reading that could depend on it; puella is feminine, so
     * no nearer word can take bonum; one analysis, so R_P reads it */
    x = hand_new("9. bonum puellae dominum amant rege forti (six words)", 0,
        "bonum puellae dominum amant rege forti", 3, 0, 1, 0);
    hand_word(x, rd_adj(0, MASC, ACC, SG), 3);
    hand_word(x, rd_noun(0, NOM, PL), 4);
    hand_word(x, rd_noun(1, ACC, SG), 4);
    hand_word(x, rd_verb(1), 0);
    hand_word(x, rd_noun(3, ABL, SG), 4);
    hand_word(x, rd_adj(1, MASC, ABL, SG), 5);
    hand_disp(x, 1, 1, 0, 0);
    /* 10: head first - rege, puella and dominum all come before regis and
     * could take it; none but dominum is as near (amat is a verb, fortis
     * in regis's own subtree); every analysis crosses the root arc, so P
     * is empty */
    x = hand_new("10. rege puella dominum amat regis fortis (six words)", 0,
        "rege puella dominum amat regis fortis", 4, 0, 0, 0);
    hand_word(x, rd_noun(3, ABL, SG), 4);
    hand_word(x, rd_noun(0, NOM, SG), 4);
    hand_word(x, rd_noun(1, ACC, SG), 4);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(3, GEN, SG), 3);
    hand_word(x, rd_adj(1, MASC, GEN, SG), 5);
    hand_disp(x, 5, -1, 0, 0);
}

static void
part_hand_umbrae (void)
{
    int k, i;

    printf("\n--- hand-worked sentences: umbrae and labels ---\n");
    hand_sentences_umbrae();
    for (k = 0; k < n_handu; k++) {
        const HandU *x = &handu[k];
        int          str[MAXW], na, t = -1, ok, mask, rootd = -1, maxd = 0;
        char         text[200], msg[600], lab[200];
        Analysis     truth;
        Readers      r;
        Umbrae       u;
        Labels       lb;

        text[0] = '\0';
        for (i = 0; i < x->n; i++) {
            str[i] = form_of[x->lv][x->rd[i]];
            truth.head[i] = x->head[i];
            truth.rd[i] = x->rd[i];
            if (i > 0) strcat(text, " ");
            strcat(text, form_text[x->lv][str[i]]);
        }
        na = analyse(x->lv, str, x->n, abuf, MAXA);
        for (i = 0; i < na; i++)
            if (analysis_key(&abuf[i], x->n) == analysis_key(&truth, x->n))
                t = i;
        if (t < 0) {
            snprintf(msg, sizeof msg, "%s: the truth is an analysis of %s",
                x->title, text);
            check(msg, 0);
            continue;
        }
        read_with(abuf, na, x->n, &r);
        build_umbrae(&truth, x->n, &u);
        for (i = 0; i < u.nu; i++) {
            if (u.u[i].type == U_ROOT) rootd = u.depth[i];
            if (u.depth[i] > maxd) maxd = u.depth[i];
        }
        mask = t_npmask[x->n][tree_index(truth.head, x->n)];
        label_arcs(x->lv, &truth, x->n, mask, &u, &lb);
        ok = strcmp(text, x->text) == 0 && u.width == x->width
            && rootd == x->root_depth && maxd == x->max_depth
            && lb.nd == x->nd && (!r.in_rp[t]) == x->misread;
        lab[0] = '\0';
        for (i = 0; i < lb.nd; i++) {
            int  d = lb.word[i];
            int  dd = u.umbra_of[d] < 0 ? -1 : u.depth[u.umbra_of[d]];
            char one[80];

            if (i >= x->nd || d != x->dword[i] || dd != x->ddepth[i]
                || lb.competed[i] != x->dcomp[i]
                || lb.retrieved[i] != x->dretr[i])
                ok = 0;
            snprintf(one, sizeof one, "%s%s -> %s, %s, depth %d, %s, %s",
                i ? "; " : "", form_text[x->lv][str[d - 1]],
                form_text[x->lv][str[truth.head[d - 1] - 1]],
                lb.dep_first[i] ? "dependent first" : "head first", dd,
                lb.competed[i] ? "COMPETED" : "CLEAR",
                lb.retrieved[i] ? "RETRIEVED" : "INTERFERED");
            strcat(lab, one);
        }
        snprintf(msg, sizeof msg, "%s: width %d, root depth %d, max depth %d; "
            "%s; %s by R_P", x->title, u.width, rootd, maxd,
            lb.nd ? lab : "no displaced arc", r.in_rp[t] ? "read"
            : "MISREAD");
        check(msg, ok);
    }
}

static void
part_umbrae_five (void)
{
    UChecks ck;
    clock_t t0 = clock();
    s64     pairs = 0, crossed = 0;
    int     n;

    printf("\n--- umbrae, width, depth and labels: every pair up to %d words "
        "(checks only) ---\n", gmaxw);
    for (n = 0; n <= gmaxw; n++) pairs += level_pairs[0][n];
    crossed = meas[0].b_den;
    umbra_pass(gmaxw, &ck);
    printf("  (%.1f s)\n", seconds(t0));
    umbra_checks(quick ? "up to four words" : "up to five words", &ck, pairs,
        crossed);
}

/* ================================================================
 * Six words: feasibility (task 0; counts only)
 * ================================================================ */

/* The oracle's six-word subset. Key: the six-word bags at L1, where
 * every reading is its own form, so an L1 bag is a multiset of readings
 * and selects the same pairs at every level. In bag_code order, the bag
 * of rank r is in the subset iff r % SUBSET_K == 0. */
#define SUBSET_K 128

static double
megabytes (
    long cap)
{
    return (double)cap * (double)sizeof(Entry) / (1024.0 * 1024.0);
}

static void
free_entries (void)
{
    int lv;

    for (lv = 0; lv < NLEVELS; lv++) {
        free(gen_entries[lv].e);
        free(an_entries[lv].e);
        memset(&gen_entries[lv], 0, sizeof gen_entries[lv]);
        memset(&an_entries[lv], 0, sizeof an_entries[lv]);
    }
    gen_emitted = 0;
}

/* projective and crossed pairs of len words at one level, by every
 * distinct order of every bag: the skeleton of the per-pair pass the
 * later tasks run (orders, analyses carried to each order, tree table) */
static void
sweep_length (
    int  lv,
    int  len,
    s64 *proj,
    s64 *cross)
{
    long i = 0;

    *proj = *cross = 0;
    while (i < an_entries[lv].n) {
        long     j = i, s;
        int      bag[MAXW], nb, p, k, a, nc;
        Analysis canon[MAXA];

        while (j < an_entries[lv].n && an_entries[lv].e[j].bag
            == an_entries[lv].e[i].bag)
            j++;
        decode_bag(an_entries[lv].e[i].bag, bag, &nb);
        if (nb != len) {
            i = j;
            continue;
        }
        nc = (int)(j - i);
        for (s = i; s < j; s++)
            decode_key(an_entries[lv].e[s].key, &canon[s - i], nb);
        for (p = 0; p < nperms[nb]; p++) {
            const int *sig = perms[nb][p];
            int        inv[MAXW];

            if (!first_order(bag, sig, nb)) continue;
            for (k = 0; k < nb; k++) inv[sig[k]] = k;
            for (a = 0; a < nc; a++) {
                for (k = 0; k < nb; k++) {
                    int h = canon[a].head[k];
                    mapped[a].head[inv[k]] = h == 0 ? 0 : inv[h - 1] + 1;
                }
                if (t_proj[nb][tree_index(mapped[a].head, nb)]) (*proj)++;
                else (*cross)++;
            }
        }
        i = j;
    }
}

static void
part_six (void)
{
    int     lv, n, same_pop = 1, same_split = 1;
    long    tried, raw = 0, gcap = 0;
    clock_t t0;
    double  t_gen;
    s64     proj[NLEVELS], cross[NLEVELS];
    char    msg[400];

    printf("\n--- six words: feasibility (task 0; counts only) ---\n");
    free_entries();
    gmaxw = MAXW;
    t0 = clock();
    generate();
    t_gen = seconds(t0);
    for (lv = 0; lv < NLEVELS; lv++) {
        raw += gen_entries[lv].n;
        gcap += gen_entries[lv].cap;
    }
    printf("  generator: %ld abstract analyses up to %d words in %.1f s; "
        "%ld placements before sorting, four levels (%.0f MB)\n",
        gen_emitted, gmaxw, t_gen, raw, megabytes(gcap));
    for (lv = 0; lv < NLEVELS; lv++) {
        long   gd, ad, i;
        int    same;
        double t_sort, t_an;

        t0 = clock();
        gd = sort_unique(&gen_entries[lv]);
        t_sort = seconds(t0);
        tried = 0;
        t0 = clock();
        analyse_all_bags(lv, &tried);
        ad = sort_unique(&an_entries[lv]);
        t_an = seconds(t0);
        same = gen_entries[lv].n == an_entries[lv].n;
        for (i = 0; same && i < gen_entries[lv].n; i++)
            if (entry_cmp(&gen_entries[lv].e[i], &an_entries[lv].e[i]) != 0)
                same = 0;
        printf("  %-17s generator sort %.1f s (%ld repeats); analyser %.1f s "
            "over %ld bags, %ld analyses (%.0f MB)\n", level_name[lv], t_sort,
            gd, t_an, tried, an_entries[lv].n, megabytes(an_entries[lv].cap));
        snprintf(msg, sizeof msg, "%s: generated == analysed up to six words, "
            "%ld positioned analyses on canonical strings, readings included",
            level_name[lv], an_entries[lv].n);
        check(msg, same);
        snprintf(msg, sizeof msg, "%s: the analyser never yields an analysis "
            "twice (six words)", level_name[lv]);
        check(msg, ad == 0);
        free(gen_entries[lv].e);
        memset(&gen_entries[lv], 0, sizeof gen_entries[lv]);
    }

    sizes();
    printf("\n  %-17s %8s %10s %8s %8s %10s %12s %6s\n", "level", "bags",
        "strings", "pairs 3", "pairs 4", "pairs 5", "pairs 6", "max|F|");
    for (lv = 0; lv < NLEVELS; lv++)
        printf("  %-17s %8ld %10ld %8ld %8ld %10ld %12ld %6d\n",
            level_name[lv], level_bags[lv], level_strings[lv],
            level_pairs[lv][3], level_pairs[lv][4], level_pairs[lv][5],
            level_pairs[lv][6], level_maxf[lv]);
    for (lv = 1; lv < NLEVELS; lv++)
        for (n = 0; n <= MAXW; n++)
            if (level_pairs[lv][n] != level_pairs[0][n]) same_pop = 0;
    check("six words: the population of pairs is the same at every level, "
        "length by length", same_pop);
    check("pairs by length up to six words: 3: 384, 4: 55,296, 5: 6,696,960 "
        "(as at five words) and 6: 809,533,440 (D131 task 0's count; a pin)",
        level_pairs[0][1] == 0 && level_pairs[0][2] == 0
        && level_pairs[0][3] == 384 && level_pairs[0][4] == 55296
        && level_pairs[0][5] == 6696960 && level_pairs[0][6] == 809533440L);
    check("the largest |F| of an L0 string up to six words is 144 (D131 "
        "task 0's count; a pin)", level_maxf[0] == 144);

    printf("\n  six-word pairs, projective and crossed: the per-pair pass "
        "(orders, analyses carried, tree table)\n");
    for (lv = 0; lv < NLEVELS; lv++) {
        double t_sw;

        t0 = clock();
        sweep_length(lv, MAXW, &proj[lv], &cross[lv]);
        t_sw = seconds(t0);
        printf("  %-17s projective %10lld  crossed %10lld  (%.1f s)\n",
            level_name[lv], (long long)proj[lv], (long long)cross[lv], t_sw);
        if (proj[lv] != proj[0] || cross[lv] != cross[0]
            || proj[lv] + cross[lv] != (s64)level_pairs[lv][MAXW])
            same_split = 0;
    }
    check("six words: projective + crossed == the six-word pairs, and the "
        "split is the same at every level", same_split);
    {
        UChecks ck;
        s64     pairs = 0;

        printf("\n  umbrae, width, depth and labels: every pair up to six "
            "words (checks only)\n");
        for (n = 0; n <= MAXW; n++) pairs += level_pairs[0][n];
        t0 = clock();
        umbra_pass(MAXW, &ck);
        printf("  (%.1f s)\n", seconds(t0));
        umbra_checks("up to six words", &ck, pairs, meas[0].b_den
            + cross[0]);
    }
    {
        s64 p5, c5;

        sweep_length(0, 5, &p5, &c5);
        snprintf(msg, sizeof msg, "the same pass at five words: projective "
            "%lld, crossed %lld (D131 task 0's table: 2,694,144 and 4,002,816; "
            "a pin)", (long long)p5, (long long)c5);
        check(msg, p5 == 2694144 && c5 == 4002816);
    }

    {
        long i = 0, rank = 0;
        s64  pairs6 = 0;
        int  r, ident = 1, kk;
        static const int ks[5] = { 32, 64, 128, 256, 512 };
        long sel_bags[5];
        s64  sel_pairs[5];

        for (kk = 0; kk < 5; kk++) {
            sel_bags[kk] = 0;
            sel_pairs[kk] = 0;
        }
        for (r = 0; r < n_readings; r++)
            if (form_of[1][r] != r) ident = 0;
        check("L1: form index == reading index, so an L1 bag is a multiset "
            "of readings (the subset's key is level-free)", ident
            && n_forms[1] == n_readings);
        while (i < an_entries[1].n) {
            long j = i;
            int  bag[MAXW], nb;
            s64  pr;

            while (j < an_entries[1].n && an_entries[1].e[j].bag
                == an_entries[1].e[i].bag)
                j++;
            decode_bag(an_entries[1].e[i].bag, bag, &nb);
            if (nb == MAXW) {
                pr = (s64)multinomial(bag, nb) * (s64)(j - i);
                pairs6 += pr;
                for (kk = 0; kk < 5; kk++)
                    if (rank % ks[kk] == 0) {
                        sel_bags[kk]++;
                        sel_pairs[kk] += pr;
                    }
                rank++;
            }
            i = j;
        }
        printf("\n  the oracle's six-word subset: %ld six-word bags at L1 "
            "(multisets of readings), %lld pairs\n", rank,
            (long long)pairs6);
        for (kk = 0; kk < 5; kk++)
            printf("  %s K = %3d: %7ld bags, %11lld pairs (%.2f%% of the "
                "six-word pairs)\n", ks[kk] == SUBSET_K ? "->" : "  ",
                ks[kk], sel_bags[kk], (long long)sel_pairs[kk],
                100.0 * (double)sel_pairs[kk] / (double)pairs6);
        check("the six-word L1 bags hold every six-word pair", pairs6
            == (s64)level_pairs[1][MAXW]);
    }
}

int
main (void)
{
    quick = getenv("D134_CELER") != NULL;
    gmaxw = quick ? 4 : 5;
    printf("KNOTAPEL DEMO 134: umbra width of the toy world%s\n", quick
        ? " (QUICK MODE: up to four words, no six-word part)" : "");
    build_world();
    build_comp();
    build_perms();
    part_paradigms();
    part_trees();
    part_world();
    part_order();
    part_hand();
    part_measures();
    part_hand_umbrae();
    part_umbrae_five();
    if (!quick) part_six();
    printf("\n=== %d pass, %d fail ===\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
