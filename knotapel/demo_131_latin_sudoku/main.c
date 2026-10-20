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
 *   Part B  (T2) licensing cost A (projective pairs: attachment cells
 *           open under F but forced under P) and garden paths B (crossed
 *           pairs misread by R_P, nested first), split by the displaced
 *           word's form; registered Q1 and Q2, the rules on constructed
 *           totals; totals pinned from oracle_partb.py (independent
 *           Python, tree-first). Descriptive, not registered: B by WHAT is
 *           displaced (adjectives only / genitives only / both, each by
 *           form), by mildness and by crossing count; the open cells
 *           behind A. Tree counts pinned from oracle_trees.py. (T4 review
 *           fixes, 2026-10-19.)
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
static unsigned char t_cross[MAXW + 1][TCODES];   /* crossings, root arc
                                                     included */

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
        "trees on n <= 5 positions: n^(n-1) for every n (Cayley), %ld in all",
        ntrees);
    check(msg, cayley_ok);
    check("projective by crossings (root arc) == by interval yields, and == "
        "no non-projective arc, on every tree", routes_ok);
    check("projective => mild on every tree", mild_ok);
    check("projective trees per n = 1, 2, 7, 30, 143 and mild trees = 1, 2, "
        "9, 64, 569 (oracle_trees.py, independent Python; a pin)", counts_ok);
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
    check("pairs by length = oracle_partb.py's (3: 384, 4: 55,296, 5: "
        "6,696,960; = task 0's count; a pin)", level_pairs[0][1] == 0 && level_pairs[0][2] == 0
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
        i64      seen[120];
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

/* Q1: A(L1) <= A(L0) < A(L2) <= A(L3), and A(L0) - A(L1) <= A(L2) -
 * A(L0); the four A share one denominator (the projective pairs' cells,
 * the same at every level), so the rule reads numerators */
static int
q1_rule (
    s64 a1,
    s64 a0,
    s64 a2,
    s64 a3)
{
    return a1 <= a0 && a0 < a2 && a2 <= a3 && 2 * a0 <= a1 + a2;
}

/* Q2: at L0, B over syncretic displaced > B over unambiguous displaced;
 * and B(L0) < B(L2) (one denominator: the crossed pairs) */
static int
q2_rule (
    s64 bs,
    s64 bs_den,
    s64 bu,
    s64 bu_den,
    s64 b0,
    s64 b2)
{
    return bs_den > 0 && bu_den > 0 && bs * bu_den > bu * bs_den && b0 < b2;
}

static void
rules_check (void)
{
    printf("\n--- the registered rules on constructed totals ---\n");
    check("Q1 rule: (1, 2, 4, 4) holds; (1, 3, 4, 4) fails the second half",
        q1_rule(1, 2, 4, 4) && !q1_rule(1, 3, 4, 4));
    check("Q1 rule at its thresholds: A1 = A0 holds, A0 = A2 fails (2, 2, 2, "
        "3: every other clause holds), A2 = A3 holds, 2 A0 = A1 + A2 holds, "
        "2 A0 = A1 + A2 + 1 fails, A1 = A0 + 1 fails, A2 > A3 alone fails",
        q1_rule(2, 2, 3, 3) && !q1_rule(2, 2, 2, 3) && !q1_rule(1, 2, 4, 3)
        && q1_rule(1, 2, 3, 3) && q1_rule(1, 2, 3, 4) && !q1_rule(1, 3, 4, 5)
        && !q1_rule(3, 2, 4, 4));
    check("Q2 rule: (2/4 vs 1/4, 1 < 2) holds; equal shares fail; B0 = B2 "
        "fails; an empty side fails", q2_rule(2, 4, 1, 4, 1, 2)
        && !q2_rule(1, 2, 2, 4, 1, 2) && !q2_rule(2, 4, 1, 4, 2, 2)
        && !q2_rule(2, 4, 0, 0, 1, 2));
    check("Q2 rule at its threshold: 3/7 vs 3/7 written 3/7 and 6/14 fails, "
        "4/7 vs 6/14 holds", !q2_rule(3, 7, 6, 14, 1, 2)
        && q2_rule(4, 7, 6, 14, 1, 2));
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
            if (meas[lv].vdisp != 0 || meas[lv].open_f - meas[lv].open_p
                != meas[lv].a_num)
                ok = 0;
        check("no crossed pair displaces a verb dependent, counted directly "
            "(the root word dominates every word); open F - open P == A's "
            "numerator", ok);
    }
}

static void
verdicts (void)
{
    const Measures *m0 = &meas[0], *m1 = &meas[1], *m2 = &meas[2],
                   *m3 = &meas[3];
    char            s1[32], s2[32];
    int             q1, q2;

    printf("\n--- verdicts (registered, five words) ---\n");
    if (quick) {
        printf("  (quick mode: four words; the registered verdicts are "
            "decided on five)\n");
        return;
    }
    q1 = q1_rule(m1->a_num, m0->a_num, m2->a_num, m3->a_num);
    printf("  Q1 (morphology pays): A numerators L1 %lld, L0 %lld, L2 %lld, "
        "L3 %lld over %lld\n", (long long)m1->a_num, (long long)m0->a_num,
        (long long)m2->a_num, (long long)m3->a_num, (long long)m0->a_den);
    printf("      A(L1) <= A(L0): %s; A(L0) < A(L2): %s; A(L2) <= A(L3): %s; "
        "A(L0) - A(L1) <= A(L2) - A(L0): %s\n",
        m1->a_num <= m0->a_num ? "yes" : "NO", m0->a_num < m2->a_num ? "yes"
        : "NO", m2->a_num <= m3->a_num ? "yes" : "NO",
        2 * m0->a_num <= m1->a_num + m2->a_num ? "yes" : "NO");
    printf("  Q1 %s\n", q1 ? "HOLDS" : "FAILS");
    q2 = q2_rule(m0->bs_num, m0->bs_den, m0->bu_num, m0->bu_den, m0->b_num,
        m2->b_num);
    pct(s1, 32, m0->bs_num, m0->bs_den);
    pct(s2, 32, m0->bu_num, m0->bu_den);
    printf("  Q2 (syncretism makes the garden paths): at L0 B syncretic %s > "
        "unambiguous %s: %s;\n      B(L0) %lld < B(L2) %lld: %s\n", s1, s2,
        m0->bs_num * m0->bu_den > m0->bu_num * m0->bs_den ? "yes" : "NO",
        (long long)m0->b_num, (long long)m2->b_num,
        m0->b_num < m2->b_num ? "yes" : "NO");
    printf("  Q2 %s\n", q2 ? "HOLDS" : "FAILS");
    check("the verdicts re-derived from the oracle's totals agree (Q1 and Q2 "
        "applied to oracle_partb.py's numbers)",
        q1 == q1_rule(oracle_b[0][1][0], oracle_b[0][0][0], oracle_b[0][2][0],
            oracle_b[0][3][0])
        && q2 == q2_rule(oracle_b[0][0][5], oracle_b[0][0][6],
            oracle_b[0][0][7], oracle_b[0][0][8], oracle_b[0][0][3],
            oracle_b[0][2][3]));
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
    part_measures();
    rules_check();
    verdicts();
    printf("\n=== %d pass, %d fail ===\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
