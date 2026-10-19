/*
 * KNOTAPEL DEMO 131: Toy Latin Sudoku - does morphology pay for crossing?
 * ======================================================================
 *
 * Slice 1 of proposal N14 (plan.md, approved 2026-10-19; brainstorm
 * sections XI, XIII, XIV). D130 found that where Latin crosses freely
 * the DISPLACED word of a crossing carries agreement with its head, and
 * that English, without case labels, almost never crosses. Hypothesis:
 * morphology pays for crossing. Tested exactly on a toy world small
 * enough to enumerate, with REAL Latin endings (and their syncretism),
 * at four morphology levels:
 *   L0 Latin           the real paradigms (Bennett, macrons dropped)
 *   L1 ideal           every reading its own form (counterfactual)
 *   L2 case-stripped   one form per gender and number (the nominative)
 *   L3 bare            as L2, and adjectives invariant (no agreement)
 *
 *   Part A  (T1) the world: paradigms against Bennett and Whitaker's
 *           INFLECTS.LAT, the levels, the generator against an
 *           independent analyser (every positioned analysis, readings
 *           included, both ways), word order cannot change F(s), the
 *           tree table (two projectivity routes, projective => mild,
 *           reversal), sizes against task 0, hand-worked sentences.
 *   Part B  (T2) licensing cost A and garden paths B; Q1, Q2.
 *
 * The world (syntax only, no semantics): nouns puella (1st, f), dominus
 * (2nd, m), donum (2nd, n), rex (3rd, m); adjectives bonus (1st/2nd),
 * fortis (3rd); verbs amat / amant (subject + object), dat / dant
 * (subject + object + dative). Cases nom, gen, dat, acc, abl. Slots:
 * exactly one nominative subject agreeing in number with the verb,
 * exactly one accusative object, for dat / dant exactly one dative, any
 * number of ablative adjuncts; a genitive depends on another noun (at
 * most one per noun); an adjective on a noun of the same case, gender
 * and number. No pro-drop, vocative or locative. Up to five words.
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
 *   ./bin/aedilis knotapel/demo_131_latin_sudoku/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * Quick mode for plants: D131_CELER=1 (exhaustive passes up to four
 * words).
 */

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
    Piscina *p = piscina_generare_dynamicum("d131_whitaker", 1 << 22);
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

#define MAXW 5
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
static int perms[MAXW + 1][120][MAXW];
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
 * The tree table: every rooted tree on n <= 5 positions, measured once
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

#define TCODES 7776            /* 6^5 */
static unsigned char t_valid[MAXW + 1][TCODES];
static unsigned char t_proj[MAXW + 1][TCODES];
static unsigned char t_proj2[MAXW + 1][TCODES];
static unsigned char t_mild[MAXW + 1][TCODES];
static unsigned char t_npmask[MAXW + 1][TCODES];

static void
part_trees (void)
{
    Piscina *scratch = piscina_generare_dynamicum("d131_tree", 1 << 16);
    int      n, k, code, ncodes;
    int      cayley_ok = 1, routes_ok = 1, mild_ok = 1, rev_ok = 1;
    int      counts_ok = 1;
    long     ntrees = 0;
    static const long proj_count[MAXW + 1] = { 0, 1, 2, 7, 30, 143 };
    static const long mild_count[MAXW + 1] = { 0, 1, 2, 9, 64, 569 };
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
        "trees on n <= 5 positions: n^(n-1) for every n (Cayley), %ld in all",
        ntrees);
    check(msg, cayley_ok);
    check("projective by crossings (root arc) == by interval yields, and == "
        "no non-projective arc, on every tree", routes_ok);
    check("projective => mild on every tree", mild_ok);
    check("projective trees per n = 1, 2, 7, 30, 143 and mild trees = 1, 2, "
        "9, 64, 569 (independent Python count, a pin)", counts_ok);
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
        "nouns and bonus: Bennett == Whitaker (AGE X, FREQ A) in %d of %d "
        "cells", equal, cells);
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

    /* sizes per level */
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
    check("pairs by length = task 0's count (3: 384, 4: 55,296, 5: 6,696,960; "
        "T0 Python, a pin)", level_pairs[0][1] == 0 && level_pairs[0][2] == 0
        && level_pairs[0][3] == 384 && level_pairs[0][4] == 55296
        && (quick || level_pairs[0][5] == 6696960));
}

/* word order cannot change F(s): every distinct order of every bag */
static void
part_order (void)
{
    int  lv;
    char msg[256];

    printf("\n--- word order and F(s) ---\n");
    for (lv = 0; lv < NLEVELS; lv++) {
        long i = 0, strings = 0, bad = 0;

        while (i < an_entries[lv].n) {
            long     j = i, s;
            int      bag[MAXW], nb, p, k;
            i64      seen[120];
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
    int         attach, reading;   /* forced cells under F */
} Hand;

static void
part_hand (void)
{
    Hand h[7];
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
    /* the same words, nested */
    h[1].title = "puella bonum dominum amat (nested)";
    h[1].lv = 0; h[1].n = 4; h[1].text = "puella bonum dominum amat";
    h[1].rd[0] = rd_noun(0, NOM, SG);      h[1].head[0] = 4;
    h[1].rd[1] = rd_adj(0, MASC, ACC, SG); h[1].head[1] = 3;
    h[1].rd[2] = rd_noun(1, ACC, SG);      h[1].head[2] = 4;
    h[1].rd[3] = rd_verb(0);               h[1].head[3] = 0;
    h[1].nf = 1; h[1].np = 1; h[1].nm = 1; h[1].truth_read = 1;
    h[1].attach = 4; h[1].reading = 4;
    /* bona (n acc pl) displaced: syncretic, a nested misreading exists */
    h[2].title = "bona puella dona amat (bona -> dona, crossed: garden path)";
    h[2].lv = 0; h[2].n = 4; h[2].text = "bona puella dona amat";
    h[2].rd[0] = rd_adj(0, NEUT, ACC, PL); h[2].head[0] = 3;
    h[2].rd[1] = rd_noun(0, NOM, SG);      h[2].head[1] = 4;
    h[2].rd[2] = rd_noun(2, ACC, PL);      h[2].head[2] = 4;
    h[2].rd[3] = rd_verb(0);               h[2].head[3] = 0;
    h[2].nf = 2; h[2].np = 1; h[2].nm = 2; h[2].truth_read = 0;
    h[2].attach = 3; h[2].reading = 3;
    /* the same analysis at L1: the ending says which noun */
    h[3] = h[2];
    h[3].title = "the same analysis at L1 (ideal endings): no garden path";
    h[3].lv = 1; h[3].text = NULL;
    h[3].nf = 1; h[3].np = 0; h[3].nm = 1; h[3].truth_read = 1;
    h[3].attach = 4; h[3].reading = 4;
    /* neuter nominative = accusative: readings open, attachments forced */
    h[4].title = "donum donum amat (which donum is the subject?)";
    h[4].lv = 0; h[4].n = 3; h[4].text = "donum donum amat";
    h[4].rd[0] = rd_noun(2, NOM, SG);      h[4].head[0] = 3;
    h[4].rd[1] = rd_noun(2, ACC, SG);      h[4].head[1] = 3;
    h[4].rd[2] = rd_verb(0);               h[4].head[2] = 0;
    h[4].nf = 2; h[4].np = 2; h[4].nm = 2; h[4].truth_read = 1;
    h[4].attach = 3; h[4].reading = 1;
    /* case stripped: subject and object trade places */
    h[5].title = "puella dominus amat at L2 (puella dominum amat at L0)";
    h[5].lv = 2; h[5].n = 3; h[5].text = "puella dominus amat";
    h[5].rd[0] = rd_noun(0, NOM, SG);      h[5].head[0] = 3;
    h[5].rd[1] = rd_noun(1, ACC, SG);      h[5].head[1] = 3;
    h[5].rd[2] = rd_verb(0);               h[5].head[2] = 0;
    h[5].nf = 2; h[5].np = 2; h[5].nm = 2; h[5].truth_read = 1;
    h[5].attach = 3; h[5].reading = 1;
    h[6] = h[5];
    h[6].title = "puella dominum amat at L0";
    h[6].lv = 0; h[6].text = "puella dominum amat";
    h[6].nf = 1; h[6].np = 1; h[6].nm = 1; h[6].attach = 3; h[6].reading = 3;

    for (k = 0; k < 7; k++) {
        const Hand *x = &h[k];
        int         str[MAXW], na, t = -1, fa, fr, ok;
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
        ok = (x->text == NULL || strcmp(text, x->text) == 0) && t >= 0
            && r.nf == x->nf && r.np == x->np && r.nm == x->nm
            && r.in_rp[t] == x->truth_read && fa == x->attach
            && fr == x->reading;
        snprintf(msg, sizeof msg, "%s: |F| %d, |P| %d, |M| %d, truth %s by "
            "R_P, forced cells %d attachment / %d reading", x->title, r.nf,
            r.np, r.nm, t >= 0 && r.in_rp[t] ? "read" : "MISREAD", fa, fr);
        check(msg, ok);
    }
}

int
main (void)
{
    quick = getenv("D131_CELER") != NULL;
    if (quick) gmaxw = 4;
    printf("KNOTAPEL DEMO 131: toy Latin Sudoku - does morphology pay for "
        "crossing?%s\n", quick ? " (QUICK MODE: up to four words)" : "");
    build_world();
    build_perms();
    part_paradigms();
    part_trees();
    part_world();
    part_order();
    part_hand();
    printf("\n=== %d pass, %d fail ===\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
