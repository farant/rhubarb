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
 * words; no six-word part); D134_QUINQUE=1 runs every five-word part
 * and stops before six words (about a minute); D134_TEMPORA=1 prints
 * times.
 */

#include "dependentia.h"
#include "fractio.h"
#include "magnus.h"
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

/* times are printed only with D134_TEMPORA=1: archive.sh needs the live
 * and the frozen run byte-identical */
static int show_times = 0;

static void
tempus (
    const char *what,
    clock_t     t0)
{
    if (show_times) printf("  (%s: %.1f s)\n", what, seconds(t0));
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
    int retrieved_ns[MAXW];     /* candidates that would close a cycle
                                   set aside (descriptive) */
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
        int rns = 1;

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
                if (comp_any[lv][f[x->word - 1]][f[h - 1]]) {
                    ret = 0;
                    if (!descends(a, h, x->word)) rns = 0;
                }
                if (comp_rel[lv][r][f[x->word - 1]][f[h - 1]]) retr = 0;
            }
        } else {
            /* d arrives: the earlier words it could depend on, beyond h */
            for (w = 1; w < d; w++) {
                if (w == h) continue;
                if (comp_any[lv][f[d - 1]][f[w - 1]]) {
                    ret = 0;
                    if (!descends(a, w, d)) rns = 0;
                }
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
        lb->retrieved_ns[lb->nd] = rns;
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
    s64 lab[NLEVELS][MAXW + 1][4];   /* by level and length: displaced
                                        arcs, RETRIEVED, CLEAR, and
                                        INTERFERED only by a cycle */
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

            ck->lab[lv][n][0]++;
            ck->lab[lv][n][1] += lb[lv].retrieved[i];
            ck->lab[lv][n][2] += !lb[lv].competed[i];
            ck->lab[lv][n][3] += !lb[lv].retrieved[i] && lb[lv].retrieved_ns[i];

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
    int         dns[MAXW];          /* RETRIEVED with cycle-closing
                                       candidates set aside (checked if
                                       has_ns) */
    int         has_ns;
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
    x->dns[x->nd] = retrieved;
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
    /* 12: INTERFERED only through a cycle - when regis arrives, the open
     * subject puellae (nom pl; gen sg reading) could depend on it, but
     * regis hangs on puellae; fortis (gen sg f) also fits puellae, nearer:
     * COMPETED; every analysis has fortis cross puellae's arc: read */
    x = hand_new("12. fortis puellae regis amant dominum", 0,
        "fortis puellae regis amant dominum", 3, 0, 1, 0);
    hand_word(x, rd_adj(1, MASC, GEN, SG), 3);
    hand_word(x, rd_noun(0, NOM, PL), 4);
    hand_word(x, rd_noun(3, GEN, SG), 2);
    hand_word(x, rd_verb(1), 0);
    hand_word(x, rd_noun(1, ACC, SG), 4);
    hand_disp(x, 1, 1, 1, 0);
    x->dns[0] = 1;
    x->has_ns = 1;
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
        char         text[200], msg[1000], lab[600];
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
            char one[200];

            if (i >= x->nd || d != x->dword[i] || dd != x->ddepth[i]
                || lb.competed[i] != x->dcomp[i]
                || lb.retrieved[i] != x->dretr[i]
                || (x->has_ns && lb.retrieved_ns[i] != x->dns[i]))
                ok = 0;
            snprintf(one, sizeof one, "%s%s -> %s, %s, depth %d, %s, %s%s",
                i ? "; " : "", form_text[x->lv][str[d - 1]],
                form_text[x->lv][str[truth.head[d - 1] - 1]],
                lb.dep_first[i] ? "dependent first" : "head first", dd,
                lb.competed[i] ? "COMPETED" : "CLEAR",
                lb.retrieved[i] ? "RETRIEVED" : "INTERFERED",
                x->has_ns ? (lb.retrieved_ns[i] ? " (RETRIEVED without "
                "the cycle)" : " (INTERFERED without the cycle)") : "");
            strcat(lab, one);
        }
        snprintf(msg, sizeof msg, "%s: width %d, root depth %d, max depth %d; "
            "%s; %s by R_P", x->title, u.width, rootd, maxd,
            lb.nd ? lab : "no displaced arc", r.in_rp[t] ? "read"
            : "MISREAD");
        check(msg, ok);
    }
}

/* ================================================================
 * Measures (task 2): B in the competition x retrieval 2 x 2 by
 * stratum; width and depth; the shares of displaced arcs RETRIEVED
 * and CLEAR; the registered rules
 * ================================================================ */

#define NSTRATA 9                 /* kind x direction */
#define MAXSEL  4096
#define NV_FULL 1125              /* pinned totals, every level with B */
#define NV_SUB  837               /* pinned totals, B at L0 and L1 only */

enum { POP_ALL, POP_F2, NPOP };   /* crossed pairs; those with |F| >= 2 */
enum { RT_NEAR, RT_FLIP, RT_FAR, RT_OTHER, NROUTES };
enum { V_HOLDS, V_FAILS, V_UNDECIDED };

static const char *const kind_name[3] = {
    "adjectives only", "genitives only", "both"
};
static const char *const dir_name[3] = {
    "dependent first", "head first", "mixed"
};
static const char *const verdict_name[3] = { "HOLDS", "FAILS", "UNDECIDED" };
static const char *const route_name[NROUTES] = {
    "as near, outside d's subtree", "as near, inside it (reversed)",
    "farther", "displaced words kept"
};

typedef struct {
    /* crossed pairs: [population][stratum][competed][interfered]
     * [pairs, misread by R_P] */
    s64 cell[NPOP][NSTRATA][2][2][2];
    s64 cell_rel[NPOP][NSTRATA][2][2][2];   /* relation-specific labels */
    s64 blind[NPOP][NSTRATA][2][2];         /* [blind-competed][pairs,
                                               misread] */
    s64 route[NSTRATA][2][NROUTES];         /* misread pairs [competed]
                                               by route */
    s64 covered_clear;                      /* covered from outside d's
                                               subtree but CLEAR (0, a
                                               theorem) */
    /* displaced arcs [adjective, genitive][dependent first, head first]
     * [arcs, RETRIEVED, CLEAR, RETRIEVED by own relation, CLEAR by own
     * relation, INTERFERED only by candidates that close a cycle] */
    s64 arcs[2][2][6];
    /* pairs by width, umbrae by depth [stratum; NSTRATA = projective] */
    s64 width[NSTRATA + 1][MAXW + 2];
    s64 depth[NSTRATA + 1][MAXW + 2];
    s64 utype[3], back;
    /* T4 (review M2), filled by the L0 pass only: displaced arcs of
     * crossed pairs with |F(s)| >= 2 AT L0 (the registered population),
     * labelled at [level][adjective, genitive][dependent first, head
     * first][arcs, RETRIEVED] */
    s64 arcs_f2[NLEVELS][2][2][2];
    /* T4 (review M1), L0 only: the genitive cell (genitives only,
     * dependent first, COMPETED, |F| >= 2) by [interfered][the
     * interferers' true relations, bit r - 1][some P analysis hangs d on
     * an interferer][some hangs an interferer on h][pairs, misread] */
    s64 gdiag[2][64][2][2][2];
} Acc;

#define ACC_WORDS ((int)(sizeof(Acc) / sizeof(s64)))

static Acc acc[NLEVELS][MAXW + 1];        /* by level and exact length */
static Acc acc_sub[NLEVELS];              /* six words, the oracle's subset */
static i64 sel_codes[MAXSEL];             /* the subset's L1 bag codes */
static int n_sel = 0;
static UChecks ck_five, ck_six;

static int
subset_has (
    i64 code)
{
    int lo = 0, hi = n_sel - 1;

    while (lo <= hi) {
        int mid = (lo + hi) / 2;

        if (sel_codes[mid] == code) return 1;
        if (sel_codes[mid] < code) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;
}

/* the L1 bag of an analysis: its readings, sorted (L1 form = reading) */
static i64
reading_code (
    const Analysis *a,
    int             n)
{
    int r[MAXW], k;

    for (k = 0; k < n; k++) r[k] = a->rd[k];
    qsort(r, (size_t)n, sizeof(int), int_cmp);
    return bag_code(r, n);
}

static int
stratum_of (
    const Analysis *a,
    const Labels   *lb)
{
    int i, nadj = 0, ngen = 0, ndf = 0, kind, dir;

    for (i = 0; i < lb->nd; i++) {
        if (readings[a->rd[lb->word[i] - 1]].kind == K_ADJ) nadj++;
        else ngen++;
        ndf += lb->dep_first[i];
    }
    kind = ngen == 0 ? 0 : nadj == 0 ? 1 : 2;
    dir = ndf == lb->nd ? 0 : ndf == 0 ? 1 : 2;
    return kind * 3 + dir;
}

/* how R_P's projective alternatives move the displaced words: some P
 * analysis puts a displaced dependent on another word at least as near
 * as its true head, outside its own subtree (NEAR) or inside it (FLIP:
 * the dependency reversed - a competitor the competition rule cannot
 * see, since attaching there in the TRUE tree would close a cycle; NEAR
 * and FLIP together are "covered"), only farther (FAR), or none moves a
 * displaced dependent (OTHER: another arc or a role changes). hp[d] =
 * the heads d takes across P(s), as a bitmask */
static int
route_of (
    const Analysis *a,
    int             n,
    const Labels   *lb,
    const int      *hp)
{
    int i, w, flip = 0, far = 0;

    for (i = 0; i < lb->nd; i++) {
        int d = lb->word[i], h = a->head[d - 1];
        int dist = h > d ? h - d : d - h;

        for (w = 1; w <= n; w++) {
            if (w == h || !(hp[d] & (1 << w))) continue;
            if ((w > d ? w - d : d - w) > dist) far = 1;
            else if (descends(a, w, d)) flip = 1;
            else return RT_NEAR;
        }
    }
    return flip ? RT_FLIP : far ? RT_FAR : RT_OTHER;
}

static void
acc_pair (
    Acc            *A,
    const Analysis *a,
    const Umbrae   *u,
    int             proj,
    const Labels   *lb,
    int             nf,
    int             misread,
    int             route)
{
    int s = NSTRATA, k, i, pp, c = 0, it = 0, cr = 0, ir = 0, cb = 0;

    if (!proj) {
        if (lb->nd == 0) fatal("a crossed pair without a displaced arc");
        s = stratum_of(a, lb);
    }
    A->width[s][u->width]++;
    for (k = 0; k < u->nu; k++) {
        A->depth[s][u->depth[k]]++;
        A->utype[u->u[k].type]++;
    }
    A->back += u->nback;
    if (proj) return;
    for (i = 0; i < lb->nd; i++) {
        s64 *x = A->arcs[readings[a->rd[lb->word[i] - 1]].kind == K_ADJ ? 0
            : 1][lb->dep_first[i] ? 0 : 1];

        x[0]++;
        x[1] += lb->retrieved[i];
        x[2] += !lb->competed[i];
        x[3] += lb->retrieved_rel[i];
        x[4] += !lb->competed_rel[i];
        x[5] += !lb->retrieved[i] && lb->retrieved_ns[i];
        c |= lb->competed[i];
        it |= !lb->retrieved[i];
        cr |= lb->competed_rel[i];
        ir |= !lb->retrieved_rel[i];
        cb |= lb->competed_blind[i];
    }
    for (pp = 0; pp < NPOP; pp++) {
        if (pp == POP_F2 && nf < 2) continue;
        A->cell[pp][s][c][it][0]++;
        A->cell[pp][s][c][it][1] += misread;
        A->cell_rel[pp][s][cr][ir][0]++;
        A->cell_rel[pp][s][cr][ir][1] += misread;
        A->blind[pp][s][cb][0]++;
        A->blind[pp][s][cb][1] += misread;
    }
    if (misread) {
        A->route[s][c][route]++;
        if (route == RT_NEAR && !c) A->covered_clear++;
    }
}

/* T4 (review M1): a pair of the genitive cell (genitives only,
 * dependent first, COMPETED; L0, |F| >= 2) keyed by its interferers -
 * the open dangling words that could also take the arriving head - by
 * their TRUE relation, and by whether a projective alternative (hp[x] =
 * the heads x takes across P(s)) hangs d on an interferer or an
 * interferer on h */
static void
genitive_cell (
    Acc            *A,
    const Analysis *a,
    int             n,
    const Umbrae   *u,
    const Labels   *lb,
    const int      *hp,
    int             misread)
{
    int i, k, c = 0, it = 0, roles = 0, pd = 0, px = 0, f[MAXW];

    if (stratum_of(a, lb) != 3) return;
    for (k = 0; k < n; k++) f[k] = form_of[0][a->rd[k]];
    for (i = 0; i < lb->nd; i++) {
        int d = lb->word[i], h = a->head[d - 1];

        c |= lb->competed[i];
        it |= !lb->retrieved[i];
        for (k = 0; k < u->nu; k++) {
            const Umbra *x = &u->u[k];

            if (x->type != U_DANGLING || x->from > h - 1 || x->to <= h - 1
                || x->to == h || !comp_any[0][f[x->word - 1]][f[h - 1]])
                continue;
            roles |= 1 << (relation_of(a, x->word) - 1);
            if (hp[d] & (1 << x->word)) pd = 1;
            if (hp[x->word] & (1 << h)) px = 1;
        }
    }
    if (!c) return;
    A->gdiag[it][roles][pd][px][0]++;
    A->gdiag[it][roles][pd][px][1] += misread;
}

/* every pair of lo..hi words at level lv, by that level's own bags (so
 * F(s) and P(s) are the string's own) */
static void
measure_umbrae (
    int lv,
    int lo,
    int hi)
{
    static unsigned char sel[MAXA];
    long                 i = 0;

    while (i < an_entries[lv].n) {
        long     j = i, s;
        int      bag[MAXW], nb, p, k, a, nc;
        Analysis canon[MAXA];

        while (j < an_entries[lv].n && an_entries[lv].e[j].bag
            == an_entries[lv].e[i].bag)
            j++;
        decode_bag(an_entries[lv].e[i].bag, bag, &nb);
        if (nb < lo || nb > hi) {
            i = j;
            continue;
        }
        nc = (int)(j - i);
        for (s = i; s < j; s++)
            decode_key(an_entries[lv].e[s].key, &canon[s - i], nb);
        for (a = 0; a < nc; a++)
            sel[a] = (unsigned char)(nb == MAXW && n_sel > 0
                && subset_has(reading_code(&canon[a], nb)));
        for (p = 0; p < nperms[nb]; p++) {
            const int *sig = perms[nb][p];
            int        inv[MAXW], hp[MAXW + 1], d;

            if (!first_order(bag, sig, nb)) continue;
            for (k = 0; k < nb; k++) inv[sig[k]] = k;
            for (a = 0; a < nc; a++)
                for (k = 0; k < nb; k++) {
                    int h = canon[a].head[k];
                    mapped[a].head[inv[k]] = h == 0 ? 0 : inv[h - 1] + 1;
                    mapped[a].rd[inv[k]] = canon[a].rd[k];
                }
            read_with(mapped, nc, nb, &rdr);
            for (d = 0; d <= nb; d++) hp[d] = 0;
            for (a = 0; a < nc; a++)
                if (rdr.in_p[a])
                    for (d = 1; d <= nb; d++)
                        hp[d] |= 1 << mapped[a].head[d - 1];
            for (a = 0; a < nc; a++) {
                Umbrae u;
                Labels lb;
                int    ix = tree_index(mapped[a].head, nb);
                int    proj = t_proj[nb][ix], mis = 0, route = RT_OTHER;

                build_umbrae(&mapped[a], nb, &u);
                lb.nd = 0;
                if (!proj) {
                    label_arcs(lv, &mapped[a], nb, t_npmask[nb][ix], &u, &lb);
                    mis = rdr.np > 0;
                    if (mis) route = route_of(&mapped[a], nb, &lb, hp);
                }
                acc_pair(&acc[lv][nb], &mapped[a], &u, proj, &lb, nc, mis,
                    route);
                if (lv == 0 && !proj && nc >= 2) {
                    int lv2, i2;

                    genitive_cell(&acc[lv][nb], &mapped[a], nb, &u, &lb, hp,
                        mis);
                    for (lv2 = 0; lv2 < NLEVELS; lv2++) {
                        Labels lb2;

                        label_arcs(lv2, &mapped[a], nb, t_npmask[nb][ix], &u,
                            &lb2);
                        for (i2 = 0; i2 < lb2.nd; i2++) {
                            s64 *y = acc[lv][nb].arcs_f2[lv2][readings[
                                mapped[a].rd[lb2.word[i2] - 1]].kind == K_ADJ
                                ? 0 : 1][lb2.dep_first[i2] ? 0 : 1];

                            y[0]++;
                            y[1] += lb2.retrieved[i2];
                        }
                    }
                }
                if (sel[a])
                    acc_pair(&acc_sub[lv], &mapped[a], &u, proj, &lb, nc, mis,
                        route);
            }
        }
        i = j;
    }
}

static void
acc_add (
    Acc       *dst,
    const Acc *src)
{
    s64       *x = (s64 *)dst;
    const s64 *y = (const s64 *)src;
    int        k;

    for (k = 0; k < ACC_WORDS; k++) x[k] += y[k];
}

/* the pinned totals in one fixed order (oracle_d134.py prints the same):
 * B cells at the first nb levels; at L0 the relation-specific cells, the
 * blind split, the routes and covered-but-CLEAR; the arc labels at every
 * level; at L0 width, depth, umbra types and backward attachments */
static int
flatten (
    const Acc *const *lv4,
    int               nb,
    s64              *out)
{
    int m = 0, lv, pp, s, c, i, k, r;

    for (lv = 0; lv < nb; lv++)
        for (pp = 0; pp < NPOP; pp++)
            for (s = 0; s < NSTRATA; s++)
                for (c = 0; c < 2; c++)
                    for (i = 0; i < 2; i++)
                        for (k = 0; k < 2; k++)
                            out[m++] = lv4[lv]->cell[pp][s][c][i][k];
    for (pp = 0; pp < NPOP; pp++)
        for (s = 0; s < NSTRATA; s++)
            for (c = 0; c < 2; c++)
                for (i = 0; i < 2; i++)
                    for (k = 0; k < 2; k++)
                        out[m++] = lv4[0]->cell_rel[pp][s][c][i][k];
    for (pp = 0; pp < NPOP; pp++)
        for (s = 0; s < NSTRATA; s++)
            for (c = 0; c < 2; c++)
                for (k = 0; k < 2; k++)
                    out[m++] = lv4[0]->blind[pp][s][c][k];
    for (s = 0; s < NSTRATA; s++)
        for (c = 0; c < 2; c++)
            for (r = 0; r < NROUTES; r++)
                out[m++] = lv4[0]->route[s][c][r];
    out[m++] = lv4[0]->covered_clear;
    for (lv = 0; lv < NLEVELS; lv++)
        for (k = 0; k < 2; k++)
            for (i = 0; i < 2; i++)
                for (r = 0; r < 6; r++)
                    out[m++] = lv4[lv]->arcs[k][i][r];
    for (s = 0; s <= NSTRATA; s++)
        for (k = 0; k < MAXW + 2; k++)
            out[m++] = lv4[0]->width[s][k];
    for (s = 0; s <= NSTRATA; s++)
        for (k = 0; k < MAXW + 2; k++)
            out[m++] = lv4[0]->depth[s][k];
    for (k = 0; k < 3; k++) out[m++] = lv4[0]->utype[k];
    out[m++] = lv4[0]->back;
    return m;
}

/* the registered rules' inputs back from a pinned vector: L0 cells, the
 * arc labels at every level */
static void
unflatten (
    const s64 *v,
    int        nb,
    Acc       *lv4)
{
    int m = 0, lv, pp, s, c, i, k, r;

    for (lv = 0; lv < NLEVELS; lv++) memset(&lv4[lv], 0, sizeof(Acc));
    for (lv = 0; lv < nb; lv++)
        for (pp = 0; pp < NPOP; pp++)
            for (s = 0; s < NSTRATA; s++)
                for (c = 0; c < 2; c++)
                    for (i = 0; i < 2; i++)
                        for (k = 0; k < 2; k++)
                            lv4[lv].cell[pp][s][c][i][k] = v[m++];
    m += 2 * NSTRATA * 8 + 2 * NSTRATA * 4 + NSTRATA * 2 * NROUTES + 1;
    for (lv = 0; lv < NLEVELS; lv++)
        for (k = 0; k < 2; k++)
            for (i = 0; i < 2; i++)
                for (r = 0; r < 6; r++)
                    lv4[lv].arcs[k][i][r] = v[m++];
}

/* ---------------- the registered rules ---------------- */

/* a group is testable with >= 1,000 pairs and >= 2% of the set it is
 * compared within */
static int
testable (
    s64 n,
    s64 within)
{
    return n >= 1000 && 50 * n >= within;
}

static Fractio
share (
    s64      num,
    s64      den,
    Piscina *p)
{
    Fractio f = fractio_ex_s64(0);

    if (den > 0) (void)fractio_ex_s64_s64(num, den, p, &f);
    return f;
}

/* sign of (an/ad - bn/bd) - k/100, exactly; absolute gap if ab */
static int
gap_cmp (
    s64      an,
    s64      ad,
    s64      bn,
    s64      bd,
    s64      k,
    int      ab,
    Piscina *p)
{
    Fractio g = fractio_subtrahe(share(an, ad, p), share(bn, bd, p), p);
    Fractio t = fractio_ex_s64(0);

    if (ab) g = fractio_absolutum(g, p);
    (void)fractio_ex_s64_s64(k, 100, p, &t);
    return (int)fractio_compara(g, t, p);
}

/* U2a: per stratum g = { misread competed, competed, misread clear,
 * clear, stratum }; in every stratum where both groups are testable
 * B(competed) - B(clear) >= 10 points; three such strata or UNDECIDED */
static int
u2a_rule (
    s64 const (*g)[5],
    int        ns,
    int       *ntest,
    Piscina   *p)
{
    int s, fail = 0;

    *ntest = 0;
    for (s = 0; s < ns; s++) {
        if (!testable(g[s][1], g[s][4]) || !testable(g[s][3], g[s][4]))
            continue;
        (*ntest)++;
        if (gap_cmp(g[s][0], g[s][1], g[s][2], g[s][3], 10, 0, p) < 0)
            fail = 1;
    }
    if (*ntest < 3) return V_UNDECIDED;
    return fail ? V_FAILS : V_HOLDS;
}

/* U2b: per cell g = { misread interfered, interfered, misread retrieved,
 * retrieved, cell }; a cell COUNTS if both groups are testable and
 * B(retrieved) is in [10%, 90%]; every counting cell |B(interfered) -
 * B(retrieved)| <= 5 points; two counting cells or UNDECIDED */
static int
counting_cell (
    const s64 *g)
{
    return testable(g[1], g[4]) && testable(g[3], g[4])
        && 10 * g[2] >= g[3] && 10 * g[2] <= 9 * g[3];
}

static int
u2b_rule (
    s64 const (*g)[5],
    int        ncells,
    int       *ncount,
    Piscina   *p)
{
    int c, fail = 0;

    *ncount = 0;
    for (c = 0; c < ncells; c++) {
        if (!counting_cell(g[c])) continue;
        (*ncount)++;
        if (gap_cmp(g[c][0], g[c][1], g[c][2], g[c][3], 5, 1, p) > 0)
            fail = 1;
    }
    if (*ncount < 2) return V_UNDECIDED;
    return fail ? V_FAILS : V_HOLDS;
}

/* U1 over N displaced adjective arcs, r_L of them RETRIEVED at L (one
 * denominator): I(L0) - I(L2) >= 2 (I(L1) - I(L0)) and I(L0) - I(L2) >=
 * 5 points */
static int
u1_rule (
    s64 r0,
    s64 r1,
    s64 r2,
    s64 n)
{
    if (n == 0) return V_UNDECIDED;
    return r0 - r2 >= 2 * (r1 - r0) && 20 * (r0 - r2) >= n ? V_HOLDS
        : V_FAILS;
}

/* the rules' inputs from per-level totals (registered population: L0,
 * |F| >= 2) */
static void
rule_inputs (
    const Acc *const *lv4,
    s64             (*ga)[5],
    s64             (*gb)[5],
    s64              *u1)
{
    const Acc *A = lv4[0];
    int        s, c, i, kd, lv;

    for (s = 0; s < NSTRATA; s++) {
        for (i = 0; i < 5; i++) ga[s][i] = 0;
        for (i = 0; i < 2; i++) {
            ga[s][0] += A->cell[POP_F2][s][1][i][1];
            ga[s][1] += A->cell[POP_F2][s][1][i][0];
            ga[s][2] += A->cell[POP_F2][s][0][i][1];
            ga[s][3] += A->cell[POP_F2][s][0][i][0];
        }
        ga[s][4] = ga[s][1] + ga[s][3];
    }
    /* U2b: dependent-first strata (dir 0), COMPETED then CLEAR */
    for (kd = 0; kd < 3; kd++)
        for (c = 0; c < 2; c++) {
            s64 *g = gb[kd * 2 + c];
            int  cc = 1 - c;

            s = kd * 3;
            g[0] = A->cell[POP_F2][s][cc][1][1];
            g[1] = A->cell[POP_F2][s][cc][1][0];
            g[2] = A->cell[POP_F2][s][cc][0][1];
            g[3] = A->cell[POP_F2][s][cc][0][0];
            g[4] = g[1] + g[3];
        }
    /* U1: adjective arcs, both directions, RETRIEVED at L0, L1, L2 */
    u1[3] = lv4[0]->arcs[0][0][0] + lv4[0]->arcs[0][1][0];
    for (lv = 0; lv < 3; lv++)
        u1[lv] = lv4[lv]->arcs[0][0][1] + lv4[lv]->arcs[0][1][1];
}

static void
decide (
    const Acc *const *lv4,
    int              *v,
    Piscina          *p)
{
    s64 ga[NSTRATA][5], gb[6][5], u1[4];
    int nt, ncount;

    rule_inputs(lv4, ga, gb, u1);
    v[0] = u2a_rule((s64 const (*)[5])ga, NSTRATA, &nt, p);
    v[1] = u2b_rule((s64 const (*)[5])gb, 6, &ncount, p);
    v[2] = u1_rule(u1[0], u1[1], u1[2], u1[3]);
}

static void
rules_check (void)
{
    Piscina  *p = piscina_generare_dynamicum("d134_regulae", 1 << 16);
    s64       a[3][5], b[3][5];
    int       nt, k;

    printf("\n--- the registered rules on constructed totals ---\n");
    check("testable: 1,000 of 50,000 (exactly 2%) yes; 999 of 10,000 no; "
        "1,000 of 50,001 no", testable(1000, 50000) && !testable(999, 10000)
        && !testable(1000, 50001));
    for (k = 0; k < 3; k++) {
        a[k][0] = 300; a[k][1] = 1000; a[k][2] = 200; a[k][3] = 1000;
        a[k][4] = 2000;
    }
    check("U2a: three testable strata, each gap exactly 10 points, HOLDS",
        u2a_rule((s64 const (*)[5])a, 3, &nt, p) == V_HOLDS && nt == 3);
    a[2][2] = 201;
    check("U2a: one stratum at 9.9 points FAILS", u2a_rule((s64 const (*)[5])a,
        3, &nt, p) == V_FAILS);
    a[2][2] = 200;
    a[1][1] = 999;
    check("U2a: a group of 999 pairs is not testable: two strata, UNDECIDED "
        "(even with a failing one)", u2a_rule((s64 const (*)[5])a, 3, &nt, p)
        == V_UNDECIDED && nt == 2);
    a[1][1] = 1000;
    a[0][0] = 1100; a[0][1] = 2000; a[0][2] = 450; a[0][3] = 1000;
    a[0][4] = 3000;
    check("U2a: 55% of 2,000 against 45% of 1,000 is exactly 10 points, "
        "HOLDS; 1,099 of 2,000 FAILS", u2a_rule((s64 const (*)[5])a, 3, &nt, p)
        == V_HOLDS && (a[0][0] = 1099, u2a_rule((s64 const (*)[5])a, 3, &nt, p)
        == V_FAILS));
    for (k = 0; k < 3; k++) {
        b[k][0] = 150; b[k][1] = 1000; b[k][2] = 100; b[k][3] = 1000;
        b[k][4] = 2000;
    }
    check("U2b: B(retrieved) exactly 10%, gap exactly 5 points, three "
        "counting cells, HOLDS", u2b_rule((s64 const (*)[5])b, 3, &nt, p)
        == V_HOLDS && nt == 3);
    b[1][0] = 151;
    check("U2b: one counting cell at 5.1 points FAILS",
        u2b_rule((s64 const (*)[5])b, 3, &nt, p) == V_FAILS);
    b[1][0] = 49;
    check("U2b: the gap is absolute (4.9% against 10%: 5.1 points) FAILS",
        u2b_rule((s64 const (*)[5])b, 3, &nt, p) == V_FAILS);
    b[1][0] = 150;
    b[0][2] = 99; b[1][2] = 99;
    check("U2b: B(retrieved) 9.9% does not count: one counting cell, "
        "UNDECIDED", u2b_rule((s64 const (*)[5])b, 3, &nt, p) == V_UNDECIDED
        && nt == 1);
    b[0][2] = 900; b[0][0] = 950; b[1][2] = 901; b[1][0] = 951;
    check("U2b: B(retrieved) exactly 90% counts, 90.1% does not",
        counting_cell(b[0]) && !counting_cell(b[1]));
    check("U1: 550, 600, 450 of 1,000 (both clauses at equality but the "
        "floor) HOLDS; 451 at L2 FAILS", u1_rule(550, 600, 450, 1000)
        == V_HOLDS && u1_rule(550, 600, 451, 1000) == V_FAILS);
    check("U1: a gap of exactly 5 points (100 of 2,000) HOLDS, 99 FAILS; no "
        "arcs UNDECIDED", u1_rule(550, 550, 450, 2000) == V_HOLDS
        && u1_rule(550, 550, 451, 2000) == V_FAILS
        && u1_rule(0, 0, 0, 0) == V_UNDECIDED);
    {
        static Acc t[NLEVELS];
        const Acc *t4[NLEVELS];
        s64        ga[NSTRATA][5], gb[6][5], u1[4];
        int        s2, c2, i2, k2, lv, kd, ok = 1;

        memset(t, 0, sizeof t);
        for (s2 = 0; s2 < NSTRATA; s2++)
            for (c2 = 0; c2 < 2; c2++)
                for (i2 = 0; i2 < 2; i2++)
                    for (k2 = 0; k2 < 2; k2++) {
                        t[0].cell[POP_F2][s2][c2][i2][k2] = 1000 * s2
                            + 100 * c2 + 10 * i2 + k2 + 1;
                        t[0].cell[POP_ALL][s2][c2][i2][k2] = 7;
                    }
        for (lv = 0; lv < NLEVELS; lv++) {
            t[lv].arcs[0][0][0] = 50;
            t[lv].arcs[0][1][0] = 30;
            t[lv].arcs[0][0][1] = 10 + lv;
            t[lv].arcs[0][1][1] = 20 + lv;
            t[lv].arcs[1][0][0] = t[lv].arcs[1][1][0] = 999;
            t[lv].arcs[1][0][1] = t[lv].arcs[1][1][1] = 777;
            t4[lv] = &t[lv];
        }
        rule_inputs(t4, ga, gb, u1);
        for (s2 = 0; s2 < NSTRATA; s2++) {
            const s64 (*x)[2][2] = t[0].cell[POP_F2][s2];

            if (ga[s2][0] != x[1][0][1] + x[1][1][1]
                || ga[s2][1] != x[1][0][0] + x[1][1][0]
                || ga[s2][2] != x[0][0][1] + x[0][1][1]
                || ga[s2][3] != x[0][0][0] + x[0][1][0]
                || ga[s2][4] != ga[s2][1] + ga[s2][3])
                ok = 0;
        }
        for (kd = 0; kd < 3; kd++)
            for (c2 = 0; c2 < 2; c2++) {
                const s64 (*x)[2] = t[0].cell[POP_F2][kd * 3][1 - c2];
                const s64 *g = gb[kd * 2 + c2];

                if (g[0] != x[1][1] || g[1] != x[1][0] || g[2] != x[0][1]
                    || g[3] != x[0][0] || g[4] != g[1] + g[3])
                    ok = 0;
            }
        if (u1[3] != 80 || u1[0] != 30 || u1[1] != 32 || u1[2] != 34)
            ok = 0;
        check("rule inputs from constructed totals: U2a sums each stratum's "
            "competed and clear cells (|F| >= 2), U2b reads the "
            "dependent-first strata (competed, then clear), U1 the "
            "adjective arcs at L0, L1, L2", ok);
    }
    piscina_destruere(p);
}

/* oracle_d134.py's totals in flatten's order (generated by the oracle,
 * pasted): [up to five words, up to four words (quick mode)], and the
 * six-word subset (B at L0 and L1) */
static const s64 oracle_full[2][NV_FULL] = {
    { /* python3 -I oracle_d134.py 5 */
      278558, 114, 8272, 115, 38430, 19447, 2932, 1574, 274428, 224, 12402, 5,
      9632, 5844, 31730, 15177, 5695, 0, 201, 0, 681, 386, 591, 202, 88960,
      768, 0, 0, 1191936, 985408, 93312, 69632, 0, 0, 88960, 768, 74624,
      67648, 1210624, 987392, 0, 0, 0, 0, 14336, 384, 346112, 218624, 11812,
      0, 208, 52, 34636, 5042, 14784, 7391, 0, 0, 12020, 52, 6280, 572, 43140,
      11861, 5728, 0, 6024, 0, 58732, 13506, 52396, 8164, 85152, 114, 2138,
      115, 33196, 19447, 2294, 1574, 76568, 224, 10722, 5, 7964, 5844, 27526,
      15177, 136, 0, 20, 0, 646, 386, 574, 202, 88960, 768, 0, 0, 1191936,
      985408, 93312, 69632, 0, 0, 88960, 768, 74624, 67648, 1210624, 987392,
      0, 0, 0, 0, 14336, 384, 346112, 218624, 11812, 0, 208, 52, 34636, 5042,
      14784, 7391, 0, 0, 12020, 52, 6280, 572, 43140, 11861, 5728, 0, 6024, 0,
      58732, 13506, 52396, 8164, 328192, 0, 0, 0, 0, 0, 0, 0, 328192, 0, 0, 0,
      0, 0, 0, 0, 7168, 0, 0, 0, 0, 0, 0, 0, 103424, 0, 0, 0, 1221632,
      1003520, 49152, 24576, 0, 0, 103424, 0, 87040, 78848, 1183744, 949248,
      0, 0, 0, 0, 16384, 0, 344064, 212992, 16384, 0, 0, 0, 32768, 0, 12288,
      6144, 0, 0, 16384, 0, 8192, 0, 36864, 6144, 8192, 0, 8192, 0, 69632,
      12288, 36864, 0, 86016, 0, 0, 0, 0, 0, 0, 0, 86016, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 103424, 0, 0, 0, 1221632, 1003520, 49152, 24576,
      0, 0, 103424, 0, 87040, 78848, 1183744, 949248, 0, 0, 0, 0, 16384, 0,
      344064, 212992, 16384, 0, 0, 0, 32768, 0, 12288, 6144, 0, 0, 16384, 0,
      8192, 0, 36864, 6144, 8192, 0, 8192, 0, 69632, 12288, 36864, 0, 167648,
      16044, 45596, 17186, 88952, 60784, 25996, 23778, 183736, 28788, 29508,
      4442, 24520, 21424, 90428, 63138, 3580, 0, 1444, 0, 860, 660, 1284, 620,
      0, 0, 0, 0, 1033216, 1033216, 340992, 340992, 0, 0, 0, 0, 0, 0, 1374208,
      1374208, 0, 0, 0, 0, 0, 0, 360448, 360448, 1124, 0, 1124, 1002, 37116,
      15896, 22076, 14766, 0, 0, 2248, 1002, 0, 0, 59192, 30662, 0, 0, 0, 0,
      33928, 13832, 88952, 47984, 159220, 16044, 44008, 17186, 88952, 60784,
      25996, 23778, 173720, 28788, 29508, 4442, 24520, 21424, 90428, 63138,
      1140, 0, 468, 0, 860, 660, 1284, 620, 0, 0, 0, 0, 1033216, 1033216,
      340992, 340992, 0, 0, 0, 0, 0, 0, 1374208, 1374208, 0, 0, 0, 0, 0, 0,
      360448, 360448, 1124, 0, 1124, 1002, 37116, 15896, 22076, 14766, 0, 0,
      2248, 1002, 0, 0, 59192, 30662, 0, 0, 0, 0, 33928, 13832, 88952, 47984,
      23296, 0, 0, 0, 231424, 187136, 73472, 71936, 0, 0, 23296, 0, 19200,
      19200, 285696, 239872, 0, 0, 0, 0, 2560, 2560, 4608, 2560, 0, 0, 0, 0,
      1033216, 1033216, 340992, 340992, 0, 0, 0, 0, 0, 0, 1374208, 1374208, 0,
      0, 0, 0, 0, 0, 360448, 360448, 0, 0, 0, 0, 30720, 26624, 30720, 28672,
      0, 0, 0, 0, 0, 0, 61440, 55296, 0, 0, 0, 0, 6144, 6144, 116736, 104448,
      23296, 0, 0, 0, 231424, 187136, 73472, 71936, 0, 0, 23296, 0, 19200,
      19200, 285696, 239872, 0, 0, 0, 0, 2560, 2560, 4608, 2560, 0, 0, 0, 0,
      1033216, 1033216, 340992, 340992, 0, 0, 0, 0, 0, 0, 1374208, 1374208, 0,
      0, 0, 0, 0, 0, 360448, 360448, 0, 0, 0, 0, 30720, 26624, 30720, 28672,
      0, 0, 0, 0, 0, 0, 61440, 55296, 0, 0, 0, 0, 6144, 6144, 116736, 104448,
      286830, 229, 0, 0, 40930, 20823, 432, 198, 274428, 224, 12402, 5, 9632,
      5844, 31730, 15177, 5876, 0, 20, 0, 716, 404, 556, 184, 103424, 8528, 0,
      0, 1177472, 977648, 93312, 69632, 0, 0, 103424, 8528, 87040, 79296,
      1183744, 967984, 0, 0, 0, 0, 16384, 512, 344064, 218496, 12924, 156, 0,
      0, 47748, 11925, 768, 404, 0, 0, 12924, 156, 6982, 669, 41534, 11660,
      6294, 52, 6706, 52, 59958, 14370, 49922, 7196, 87290, 229, 0, 0, 35070,
      20823, 420, 198, 76568, 224, 10722, 5, 7964, 5844, 27526, 15177, 136, 0,
      20, 0, 680, 404, 540, 184, 103424, 8528, 0, 0, 1177472, 977648, 93312,
      69632, 0, 0, 103424, 8528, 87040, 79296, 1183744, 967984, 0, 0, 0, 0,
      16384, 512, 344064, 218496, 12924, 156, 0, 0, 47748, 11925, 768, 404, 0,
      0, 12924, 156, 6982, 669, 41534, 11660, 6294, 52, 6706, 52, 59958,
      14370, 49922, 7196, 23296, 0, 304896, 21250, 23296, 0, 304896, 21250, 0,
      0, 7168, 588, 103424, 8528, 1270784, 1047280, 103424, 8528, 1270784,
      1047280, 0, 0, 360448, 219008, 0, 0, 61440, 12485, 0, 0, 61440, 12485,
      0, 0, 122880, 21670, 10548, 0, 112232, 21250, 10548, 0, 112232, 21250,
      0, 0, 1376, 588, 103424, 8528, 1270784, 1047280, 103424, 8528, 1270784,
      1047280, 0, 0, 360448, 219008, 0, 0, 61440, 12485, 0, 0, 61440, 12485,
      0, 0, 122880, 21670, 0, 0, 0, 229, 21018, 0, 0, 3, 0, 0, 0, 229, 21018,
      0, 0, 3, 0, 0, 0, 0, 588, 0, 0, 0, 0, 768, 0, 0, 1054080, 768, 192, 0,
      0, 768, 0, 0, 1054080, 768, 192, 0, 0, 0, 0, 0, 219008, 0, 0, 0, 0, 0,
      52, 0, 12381, 0, 52, 0, 0, 0, 52, 0, 12381, 0, 52, 0, 0, 0, 0, 0, 21534,
      0, 136, 0, 0, 465408, 438512, 398630, 464976, 398630, 1536, 465408,
      398630, 398630, 398630, 398630, 0, 1923072, 1814208, 139648, 1815936,
      160768, 6144, 1923072, 118144, 139648, 136192, 160768, 21504, 465408,
      453120, 465408, 465408, 465408, 0, 465408, 465408, 465408, 465408,
      465408, 0, 1923072, 1873920, 160768, 1873920, 160768, 0, 1923072,
      136192, 160768, 136192, 160768, 24576, 465408, 358352, 301128, 464592,
      301128, 4096, 465408, 301128, 301128, 301128, 301128, 0, 1923072,
      1484608, 4096, 1487872, 160768, 16384, 1923072, 4096, 4096, 136192,
      160768, 0, 465408, 356096, 31488, 462336, 31488, 4096, 465408, 31488,
      31488, 31488, 31488, 0, 1923072, 1475584, 4096, 1487872, 160768, 16384,
      1923072, 4096, 4096, 136192, 160768, 0, 0, 0, 12288, 190976, 124928, 0,
      0, 0, 0, 0, 115968, 138496, 73728, 0, 0, 0, 0, 0, 1024, 6144, 0, 0, 0,
      0, 0, 0, 49152, 833536, 491520, 0, 0, 0, 0, 0, 496640, 572416, 305152,
      0, 0, 0, 0, 0, 32768, 262144, 65536, 0, 0, 0, 0, 0, 0, 12288, 49152, 0,
      0, 0, 0, 0, 26624, 34816, 0, 0, 0, 0, 0, 0, 12288, 94208, 16384, 0, 0,
      0, 0, 0, 903680, 1110912, 572416, 147456, 0, 0, 1039872, 448512, 30208,
      0, 0, 0, 0, 0, 1216000, 0, 0, 0, 0, 0, 0, 0, 20992, 7680, 0, 0, 0, 0, 0,
      0, 4321280, 1908736, 139264, 0, 0, 0, 0, 0, 5060608, 0, 0, 0, 0, 0, 0,
      0, 950272, 483328, 8192, 0, 0, 0, 0, 0, 184320, 75776, 47104, 0, 0, 0,
      0, 0, 184320, 0, 0, 0, 0, 0, 0, 0, 331776, 157696, 2048, 0, 0, 0, 0, 0,
      11538048, 0, 0, 0, 0, 0, 0, 0, 13477248, 7926144, 6752640, 5551104, },
    { /* python3 -I oracle_d134.py 4 */
      1330, 0, 26, 0, 174, 130, 6, 6, 1320, 0, 36, 0, 36, 34, 144, 102, 0, 0,
      0, 0, 0, 0, 0, 0, 896, 0, 0, 0, 5120, 4096, 128, 128, 0, 0, 896, 0, 896,
      896, 4352, 3328, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 0, 0, 0, 164, 130, 6, 6,
      36, 0, 34, 0, 34, 34, 136, 102, 0, 0, 0, 0, 0, 0, 0, 0, 896, 0, 0, 0,
      5120, 4096, 128, 128, 0, 0, 896, 0, 896, 896, 4352, 3328, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 1536, 0, 0, 0, 0, 0, 0, 0, 1536, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1024, 0, 0, 0, 5120, 4096, 0, 0, 0, 0, 1024, 0, 1024,
      1024, 4096, 3072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1024, 0, 0, 0, 5120, 4096, 0,
      0, 0, 0, 1024, 0, 1024, 1024, 4096, 3072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1008,
      0, 188, 0, 272, 204, 68, 68, 1128, 0, 68, 0, 68, 68, 272, 204, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 5120, 5120, 1024, 1024, 0, 0, 0, 0, 0, 0,
      6144, 6144, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 388, 0, 64, 0, 272, 204, 68, 68,
      384, 0, 68, 0, 68, 68, 272, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      5120, 5120, 1024, 1024, 0, 0, 0, 0, 0, 0, 6144, 6144, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 256, 0, 0, 0, 1024, 768, 256, 256, 0, 0, 256, 0, 256, 256, 1024,
      768, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5120, 5120, 1024, 1024, 0, 0,
      0, 0, 0, 0, 6144, 6144, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 256, 0, 0, 0, 1024, 768,
      256, 256, 0, 0, 256, 0, 256, 256, 1024, 768, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 5120, 5120, 1024, 1024, 0, 0, 0, 0, 0, 0, 6144, 6144, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1356, 0, 0, 0, 180, 136, 0, 0, 1320, 0, 36, 0, 36, 34,
      144, 102, 0, 0, 0, 0, 0, 0, 0, 0, 1024, 64, 0, 0, 4992, 4032, 128, 128,
      0, 0, 1024, 64, 1024, 1024, 4096, 3136, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 0, 0,
      0, 170, 136, 0, 0, 36, 0, 34, 0, 34, 34, 136, 102, 0, 0, 0, 0, 0, 0, 0,
      0, 1024, 64, 0, 0, 4992, 4032, 128, 128, 0, 0, 1024, 64, 1024, 1024,
      4096, 3136, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 256, 0, 1280, 136, 256, 0, 1280,
      136, 0, 0, 0, 0, 1024, 64, 5120, 4160, 1024, 64, 5120, 4160, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 200, 136, 40, 0, 200, 136, 0,
      0, 0, 0, 1024, 64, 5120, 4160, 1024, 64, 5120, 4160, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 136, 0, 0, 0, 0, 0, 0, 0, 136,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4224, 0, 0, 0, 0, 0, 0, 0,
      4224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1536, 1504, 1356, 1536, 1356,
      0, 1536, 1356, 1356, 1356, 1356, 0, 6144, 6016, 896, 6016, 1024, 0,
      6144, 896, 896, 1024, 1024, 0, 1536, 1536, 1536, 1536, 1536, 0, 1536,
      1536, 1536, 1536, 1536, 0, 6144, 6144, 1024, 6144, 1024, 0, 6144, 1024,
      1024, 1024, 1024, 0, 1536, 1280, 1196, 1536, 1196, 0, 1536, 1196, 1196,
      1196, 1196, 0, 6144, 5120, 0, 5120, 1024, 0, 6144, 0, 0, 1024, 1024, 0,
      1536, 1280, 256, 1536, 256, 0, 1536, 256, 256, 256, 256, 0, 6144, 5120,
      0, 5120, 1024, 0, 6144, 0, 0, 1024, 1024, 0, 0, 0, 0, 1536, 0, 0, 0, 0,
      0, 0, 768, 768, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6144, 0, 0,
      0, 0, 0, 0, 3072, 3072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18432,
      15744, 6144, 0, 0, 0, 4096, 2048, 0, 0, 0, 0, 0, 0, 4608, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16384, 8192, 0, 0, 0, 0, 0, 0, 18432, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147072, 0, 0, 0, 0, 0, 0, 0,
      83328, 61824, 55680, 21504, },
};
static const s64 oracle_sub[NV_SUB] = {
    /* python3 -I oracle_d134.py 6 sub */
    211161, 1361, 10732, 1419, 56316, 23514, 6647, 2961, 201369, 2646, 20524,
    134, 14151, 7300, 48812, 19175, 12771, 0, 1529, 0, 3467, 402, 3437, 360,
    48659, 2773, 44, 27, 1120567, 941710, 240782, 179060, 1773, 1773, 46930,
    1027, 33013, 30155, 1328336, 1090615, 0, 0, 0, 0, 18970, 3747, 1167054,
    779641, 17646, 8, 557, 74, 110346, 11883, 65971, 21635, 172, 0, 18031, 82,
    6157, 331, 170160, 33187, 8471, 6, 9391, 6, 158455, 23032, 324131, 56772,
    165127, 1361, 8932, 1419, 52582, 23514, 5767, 2961, 155063, 2646, 18996,
    134, 12877, 7300, 45472, 19175, 4442, 0, 762, 0, 2039, 402, 2313, 360,
    48659, 2773, 44, 27, 1120567, 941710, 240782, 179060, 1773, 1773, 46930,
    1027, 33013, 30155, 1328336, 1090615, 0, 0, 0, 0, 18970, 3747, 1167054,
    779641, 17646, 8, 557, 74, 110346, 11883, 65971, 21635, 172, 0, 18031, 82,
    6157, 331, 170160, 33187, 8471, 6, 9391, 6, 158455, 23032, 324131, 56772,
    280204, 768, 1536, 768, 2804, 1088, 312, 156, 280124, 1536, 1616, 0, 612,
    356, 2504, 888, 21204, 0, 0, 0, 0, 0, 0, 0, 58370, 2106, 0, 0, 1166430,
    963562, 185252, 115554, 2106, 2106, 56264, 0, 39722, 36184, 1311960,
    1042932, 0, 0, 0, 0, 22532, 3186, 1163492, 737970, 25722, 0, 128, 0,
    112802, 780, 55868, 15208, 272, 0, 25578, 0, 8662, 0, 160008, 15988,
    13314, 0, 13338, 0, 201978, 20238, 271818, 21026, 169692, 768, 1536, 768,
    2804, 1088, 312, 156, 169612, 1536, 1616, 0, 612, 356, 2504, 888, 6368, 0,
    0, 0, 0, 0, 0, 0, 58370, 2106, 0, 0, 1166430, 963562, 185252, 115554,
    2106, 2106, 56264, 0, 39722, 36184, 1311960, 1042932, 0, 0, 0, 0, 22532,
    3186, 1163492, 737970, 25722, 0, 128, 0, 112802, 780, 55868, 15208, 272,
    0, 25578, 0, 8662, 0, 160008, 15988, 13314, 0, 13338, 0, 201978, 20238,
    271818, 21026, 221857, 2774, 36, 6, 61987, 26267, 976, 208, 201369, 2646,
    20524, 134, 14151, 7300, 48812, 19175, 13451, 0, 849, 0, 3898, 438, 3006,
    324, 58370, 8899, 0, 0, 1111010, 935704, 240672, 178967, 2106, 2106,
    56264, 6793, 39722, 36481, 1311960, 1078190, 0, 0, 0, 0, 22532, 4623,
    1163492, 778765, 20043, 203, 0, 0, 162424, 30092, 12053, 3305, 189, 0,
    19854, 203, 7105, 431, 167372, 32966, 9620, 81, 10804, 85, 163468, 24329,
    316556, 55321, 174023, 2774, 36, 6, 57613, 26267, 736, 208, 155063, 2646,
    18996, 134, 12877, 7300, 45472, 19175, 4683, 0, 521, 0, 2292, 438, 2060,
    324, 58370, 8899, 0, 0, 1111010, 935704, 240672, 178967, 2106, 2106,
    56264, 6793, 39722, 36481, 1311960, 1078190, 0, 0, 0, 0, 22532, 4623,
    1163492, 778765, 20043, 203, 0, 0, 162424, 30092, 12053, 3305, 189, 0,
    19854, 203, 7105, 431, 167372, 32966, 9620, 81, 10804, 85, 163468, 24329,
    316556, 55321, 11303, 10, 273553, 29245, 11303, 10, 273553, 29245, 0, 0,
    21204, 762, 58370, 8899, 1351682, 1114671, 58370, 8899, 1351682, 1114671,
    0, 0, 1186024, 783388, 0, 0, 194520, 33600, 0, 0, 194520, 33600, 0, 0,
    500448, 79816, 9593, 10, 222815, 29245, 9593, 10, 222815, 29245, 0, 0,
    9556, 762, 58370, 8899, 1351682, 1114671, 58370, 8899, 1351682, 1114671,
    0, 0, 1186024, 783388, 0, 0, 194520, 33600, 0, 0, 194520, 33600, 0, 0,
    500448, 79816, 0, 0, 119, 2661, 26314, 0, 10, 151, 0, 0, 119, 2661, 26314,
    0, 10, 151, 0, 0, 0, 0, 762, 0, 0, 0, 0, 2566, 230, 4, 1118101, 2062, 605,
    2, 0, 2566, 230, 4, 1118101, 2062, 605, 2, 0, 0, 0, 0, 783388, 0, 0, 0, 0,
    16, 66, 0, 32839, 429, 250, 0, 0, 16, 66, 0, 32839, 429, 250, 0, 0, 12, 0,
    0, 78376, 886, 490, 52, 0, 786636, 682660, 614575, 785017, 614575, 10376,
    786636, 610256, 614575, 610256, 614575, 0, 3463992, 3035359, 148803,
    3042124, 175198, 44592, 3463992, 109927, 148803, 129032, 175198, 36152,
    786636, 718092, 772470, 786636, 772470, 6160, 786636, 771960, 772470,
    771960, 772470, 0, 3463992, 3175808, 175198, 3175808, 175198, 25040,
    3463992, 129032, 175198, 129032, 175198, 42974, 786636, 552938, 416194,
    782686, 416194, 13334, 786636, 409194, 416194, 409194, 416194, 0, 3463992,
    2445389, 11056, 2462176, 175198, 56286, 3463992, 10360, 11056, 129032,
    175198, 696, 786636, 544885, 27392, 771929, 27392, 13190, 786636, 27351,
    27392, 27351, 27392, 0, 3463992, 2408732, 11056, 2462176, 175198, 55638,
    3463992, 10360, 11056, 129032, 175198, 696, 0, 0, 10838, 117990, 123172,
    32856, 0, 0, 0, 0, 74271, 115060, 74855, 20670, 0, 0, 0, 0, 1376, 11507,
    8321, 0, 0, 0, 0, 0, 48258, 594644, 626474, 140676, 0, 0, 0, 0, 379402,
    550842, 377204, 102604, 0, 0, 0, 0, 78404, 574616, 475348, 57656, 0, 0, 0,
    0, 0, 27928, 122940, 43652, 0, 0, 0, 0, 59832, 82190, 52498, 0, 0, 0, 0,
    0, 32776, 249566, 199374, 18732, 0, 0, 0, 0, 291103, 483051, 276202,
    116512, 17040, 0, 1038018, 392363, 44257, 4348, 0, 0, 0, 0, 1222550, 0, 0,
    0, 0, 0, 0, 0, 72472, 25689, 1649, 0, 0, 0, 0, 0, 5021268, 2064398,
    280714, 32204, 0, 0, 0, 0, 5967976, 0, 0, 0, 0, 0, 0, 0, 3685792, 1671566,
    150218, 7624, 0, 0, 0, 0, 635904, 288438, 161462, 11590, 0, 0, 0, 0,
    719238, 0, 0, 0, 0, 0, 0, 0, 1565796, 662940, 85480, 1736, 0, 0, 0, 0,
    5652450, 0, 0, 0, 0, 0, 0, 0, 16676100, 8121600, 6670440, 8554500,
};

/* ---------------- printing ---------------- */

static void
bfmt (
    char *buf,
    int   size,
    s64   mis,
    s64   n)
{
    char t[32];

    pct(t, 32, mis, n);
    snprintf(buf, (size_t)size, "%s of %lld", t, (long long)n);
}

/* an exact difference of two shares in points, rounded to hundredths */
static void
gap_fmt (
    char    *buf,
    int      size,
    s64      an,
    s64      ad,
    s64      bn,
    s64      bd,
    Piscina *p)
{
    Fractio g = fractio_subtrahe(share(an, ad, p), share(bn, bd, p), p);
    s64     h = 0, m;

    g = fractio_multiplica(g, fractio_ex_s64(10000), p);
    (void)magnus_ad_s64(fractio_rotunda(g, p), &h);
    m = h < 0 ? -h : h;
    snprintf(buf, (size_t)size, "%s%lld.%02lld", h < 0 ? "-" : "+",
        (long long)(m / 100), (long long)(m % 100));
}

static void
stratum_label (
    char *buf,
    int   size,
    int   s)
{
    if (s == NSTRATA) snprintf(buf, (size_t)size, "projective");
    else snprintf(buf, (size_t)size, "%s, %s", kind_name[s / 3],
        dir_name[s % 3]);
}

static void
print_dist (
    const Acc  *A,
    const char *title)
{
    int  s, k;
    char lab[64];

    printf("\n  %s: pairs by WIDTH (umbrae open at once)\n  %-33s", title,
        "");
    for (k = 1; k <= MAXW; k++) printf(" %11d", k);
    printf("\n");
    for (s = NSTRATA; s >= 0; s--) {
        stratum_label(lab, 64, s);
        printf("  %-33s", lab);
        for (k = 1; k <= MAXW; k++)
            printf(" %11lld", (long long)A->width[s][k]);
        printf("\n");
    }
    printf("  %s: umbrae by DEPTH (later-opened umbrae that outlive it)\n"
        "  %-33s", title, "");
    for (k = 0; k < MAXW; k++) printf(" %11d", k);
    printf("\n");
    for (s = NSTRATA; s >= 0; s--) {
        stratum_label(lab, 64, s);
        printf("  %-33s", lab);
        for (k = 0; k < MAXW; k++)
            printf(" %11lld", (long long)A->depth[s][k]);
        printf("\n");
    }
    printf("  umbrae: %lld dangling, %lld slot, %lld root; %lld backward "
        "attachments\n", (long long)A->utype[U_DANGLING],
        (long long)A->utype[U_SLOT], (long long)A->utype[U_ROOT],
        (long long)A->back);
}

/* the 2 x 2 per stratum: COMPETED / CLEAR x INTERFERED / RETRIEVED */
static void
print_cells (
    const Acc  *A,
    int         rel,
    const char *title)
{
    int  s, c, i;
    char lab[64], b[48];

    printf("\n  %s (crossed pairs, |F| >= 2): B in each cell\n  %-33s %22s "
        "%22s %22s %22s\n", title, "", "competed+interfered",
        "competed+retrieved", "clear+interfered", "clear+retrieved");
    for (s = 0; s < NSTRATA; s++) {
        stratum_label(lab, 64, s);
        printf("  %-33s", lab);
        for (c = 1; c >= 0; c--)
            for (i = 1; i >= 0; i--) {
                const s64 *x = rel ? A->cell_rel[POP_F2][s][c][i]
                    : A->cell[POP_F2][s][c][i];

                bfmt(b, 48, x[1], x[0]);
                printf(" %22s", b);
            }
        printf("\n");
    }
}

static void
print_l0_extra (
    const Acc *A)
{
    int  s, c, r;
    char lab[64], b1[48], b2[48], b3[48], b4[48];
    s64  mis = 0, rt[NROUTES], rtc[NROUTES];

    printf("\n  competition, real against MORPHOLOGY-BLIND (any noun as near "
        "as the head), L0, |F| >= 2\n  %-33s %22s %22s %22s %22s\n", "",
        "competed", "clear", "blind: competed", "blind: clear");
    for (s = 0; s < NSTRATA; s++) {
        s64 cm = 0, cn = 0, lm = 0, ln = 0;
        int i;

        for (i = 0; i < 2; i++) {
            cm += A->cell[POP_F2][s][1][i][1];
            cn += A->cell[POP_F2][s][1][i][0];
            lm += A->cell[POP_F2][s][0][i][1];
            ln += A->cell[POP_F2][s][0][i][0];
        }
        stratum_label(lab, 64, s);
        bfmt(b1, 48, cm, cn);
        bfmt(b2, 48, lm, ln);
        bfmt(b3, 48, A->blind[POP_F2][s][1][1], A->blind[POP_F2][s][1][0]);
        bfmt(b4, 48, A->blind[POP_F2][s][0][1], A->blind[POP_F2][s][0][0]);
        printf("  %-33s %22s %22s %22s %22s\n", lab, b1, b2, b3, b4);
    }
    for (r = 0; r < NROUTES; r++) rt[r] = rtc[r] = 0;
    for (s = 0; s < NSTRATA; s++)
        for (c = 0; c < 2; c++)
            for (r = 0; r < NROUTES; r++) {
                rt[r] += A->route[s][c][r];
                if (!c) rtc[r] += A->route[s][c][r];
                mis += A->route[s][c][r];
            }
    pct(b1, 48, rt[RT_NEAR] + rt[RT_FLIP], mis);
    pct(b2, 48, rt[RT_FLIP], mis);
    printf("\n  MECHANISM COVERAGE (L0): of %lld misread pairs, %s have an "
        "analysis in P(s) that puts a displaced word on a word at least as "
        "near as its head (%s through a reversed dependency)\n",
        (long long)mis, b1, b2);
    printf("  misread pairs by route (all; CLEAR only):");
    for (r = 0; r < NROUTES; r++)
        printf("  %s %lld; %lld", route_name[r], (long long)rt[r],
            (long long)rtc[r]);
    printf("\n  covered from outside d's subtree but CLEAR: %lld\n",
        (long long)A->covered_clear);
}

static void
print_arcs (
    const Acc *const *lv4,
    const char       *title)
{
    int  lv, k, i, r;
    char b[5][32];
    static const char *const an[2] = { "adjective", "genitive" };

    printf("\n  %s: displaced arcs, share RETRIEVED, share CLEAR, the same by "
        "own relation, INTERFERED only through a cycle\n", title);
    for (lv = 0; lv < NLEVELS; lv++)
        for (k = 0; k <= 2; k++)
            for (i = 0; i <= 2; i++) {
                s64 x[6];

                if ((k == 2) != (i == 2)) continue;
                for (r = 0; r < 6; r++) {
                    int kk, ii;

                    x[r] = 0;
                    for (kk = 0; kk < 2; kk++)
                        for (ii = 0; ii < 2; ii++)
                            if ((k == 2 || kk == k) && (i == 2 || ii == i))
                                x[r] += lv4[lv]->arcs[kk][ii][r];
                }
                for (r = 1; r <= 4; r++) pct(b[r], 32, x[r], x[0]);
                printf("  %-17s %-10s %-16s %10lld arcs: %8s %8s; own "
                    "relation %8s %8s; cycle-only %lld\n", level_name[lv],
                    k == 2 ? "all" : an[k], i == 2 ? "" : dir_name[i],
                    (long long)x[0], b[1], b[2], b[3], b[4], (long long)x[5]);
            }
}

/* the registered tests with their tables; verdicts into v[3] */
static void
print_registered (
    const Acc *const *lv4,
    int              *v,
    Piscina          *p)
{
    s64  ga[NSTRATA][5], gb[6][5], u1[4];
    int  s, c, nt, ncount;
    char lab[64], b1[48], b2[48], g[32], i0[32], i1[32], i2[32];

    rule_inputs(lv4, ga, gb, u1);
    printf("\n  U2a (EXPECTED; a positive control, partly implied by the "
        "definitions): B(competed) - B(clear) >= 10 points\n");
    for (s = 0; s < NSTRATA; s++) {
        int t = testable(ga[s][1], ga[s][4]) && testable(ga[s][3], ga[s][4]);

        stratum_label(lab, 64, s);
        bfmt(b1, 48, ga[s][0], ga[s][1]);
        bfmt(b2, 48, ga[s][2], ga[s][3]);
        gap_fmt(g, 32, ga[s][0], ga[s][1], ga[s][2], ga[s][3], p);
        printf("  %-33s competed %24s  clear %24s  gap %8s  %s\n", lab, b1,
            b2, g, t ? "testable" : "not testable");
    }
    v[0] = u2a_rule((s64 const (*)[5])ga, NSTRATA, &nt, p);
    printf("  U2a %s (%d testable strata)\n", verdict_name[v[0]], nt);
    printf("\n  U2b (retrieval adds nothing; dependent-first strata): "
        "|B(interfered) - B(retrieved)| <= 5 points where B(retrieved) is "
        "in [10%%, 90%%]\n");
    for (c = 0; c < 6; c++) {
        const s64 *x = gb[c];

        bfmt(b1, 48, x[0], x[1]);
        bfmt(b2, 48, x[2], x[3]);
        gap_fmt(g, 32, x[0], x[1], x[2], x[3], p);
        printf("  %-16s %-9s interfered %24s  retrieved %24s  gap %8s  %s\n",
            kind_name[c / 2], c % 2 ? "CLEAR" : "COMPETED", b1, b2, g,
            counting_cell(x) ? "COUNTS" : !testable(x[1], x[4])
            || !testable(x[3], x[4]) ? "not testable"
            : "outside [10%, 90%]");
    }
    v[1] = u2b_rule((s64 const (*)[5])gb, 6, &ncount, p);
    printf("  U2b %s (%d counting cells)\n", verdict_name[v[1]], ncount);
    pct(i0, 32, u1[0], u1[3]);
    pct(i1, 32, u1[1], u1[3]);
    pct(i2, 32, u1[2], u1[3]);
    gap_fmt(g, 32, u1[0], u1[3], u1[2], u1[3], p);
    v[2] = u1_rule(u1[0], u1[1], u1[2], u1[3]);
    printf("\n  U1 (Latin keeps most of the ideal's retrieval; %lld displaced "
        "adjective arcs): I(L1) %s, I(L0) %s, I(L2) %s; I(L0) - I(L2) %s "
        "points against 2 (I(L1) - I(L0)) and the 5-point floor\n",
        (long long)u1[3], i1, i0, i2, g);
    printf("  U1 %s\n", verdict_name[v[2]]);
}

/* ---------------- the parts ---------------- */

static int
check_vector (
    const char *title,
    const s64  *got,
    const s64  *want,
    int         n)
{
    int k, ok = 1, first = -1;

    for (k = 0; k < n; k++)
        if (got[k] != want[k]) {
            if (first < 0) first = k;
            ok = 0;
        }
    if (getenv("D134_VECTOR") != NULL)
        for (k = 0; k < n; k++)
            printf("VECTOR %s %d %lld\n", title, k, (long long)got[k]);
    if (!ok)
        printf("  (%s: first difference at %d: %lld, the oracle %lld)\n",
            title, first, (long long)got[first], (long long)want[first]);
    return ok;
}

/* populations, labels against task 1's pass, width and depth at every
 * level, covered => competed; lengths lo..hi */
static void
measure_checks (
    const char    *where,
    int            lo,
    int            hi,
    const UChecks *ck,
    const s64     *cross_by_len)
{
    int  lv, n, s, c, i, k;
    int  pop_ok = 1, f2_ok = 1, lab_ok = 1, wd_ok = 1, cov_ok = 1;
    char msg[400];

    for (lv = 0; lv < NLEVELS; lv++)
        for (n = lo; n <= hi; n++) {
            const Acc *A = &acc[lv][n];
            s64        pairs = 0, mis = 0, mis2 = 0, x[4] = { 0, 0, 0, 0 };

            for (s = 0; s < NSTRATA; s++)
                for (c = 0; c < 2; c++)
                    for (i = 0; i < 2; i++) {
                        pairs += A->cell[POP_ALL][s][c][i][0];
                        mis += A->cell[POP_ALL][s][c][i][1];
                        mis2 += A->cell[POP_F2][s][c][i][1];
                    }
            if (pairs != cross_by_len[n]) pop_ok = 0;
            if (n <= 5 && mis != meas[lv].len_b_num[n]) pop_ok = 0;
            if (mis2 != mis) f2_ok = 0;
            for (k = 0; k < 2; k++)
                for (i = 0; i < 2; i++) {
                    x[0] += A->arcs[k][i][0];
                    x[1] += A->arcs[k][i][1];
                    x[2] += A->arcs[k][i][2];
                    x[3] += A->arcs[k][i][5];
                }
            for (k = 0; k < 4; k++)
                if (x[k] != ck->lab[lv][n][k]) lab_ok = 0;
            if (memcmp(A->width, acc[0][n].width, sizeof A->width) != 0
                || memcmp(A->depth, acc[0][n].depth, sizeof A->depth) != 0
                || memcmp(A->utype, acc[0][n].utype, sizeof A->utype) != 0
                || A->back != acc[0][n].back)
                wd_ok = 0;
            if (A->covered_clear != 0) cov_ok = 0;
        }
    snprintf(msg, sizeof msg, "%s: the 2 x 2 cells hold every crossed pair at "
        "every level%s", where, hi <= 5 ? ", and their misreads are D131's B "
        "by length" : "");
    check(msg, pop_ok);
    snprintf(msg, sizeof msg, "%s: every misread pair has |F| >= 2 (the "
        "registered population loses none)", where);
    check(msg, f2_ok);
    snprintf(msg, sizeof msg, "%s: arc labels by the per-level pass (each "
        "level's own bags) == task 1's pass (by L1 bags), level by level, "
        "length by length", where);
    check(msg, lab_ok);
    snprintf(msg, sizeof msg, "%s: width, depth, umbra types and backward "
        "attachments the same at every level, length by length", where);
    check(msg, wd_ok);
    snprintf(msg, sizeof msg, "%s: covered => competed, restated (re-check "
        "M1 meets L1): no misread pair whose P(s) moves a displaced word to a "
        "word as near as its head, OUTSIDE its own subtree, is CLEAR", where);
    check(msg, cov_ok);
}

/* The T4 review's own oracle (recensor-d134, rec.c: trees first, F(s)
 * and P(s) by grouping pairs by their string; pasted from its output).
 * The genitive cell (genitives only, dependent first, COMPETED, L0,
 * |F| >= 2), one row per nonzero key: { interfered, the interferers'
 * true relations (bit r - 1), some P analysis hangs d on an interferer,
 * some hangs an interferer on h, pairs, misread }. Up to four words, up
 * to five, six-word pairs. */
static const s64 review_gdiag_4[3][6] = {
    { 0, 0, 0, 0, 5120, 4096 },
    { 1, 1, 1, 0, 96, 96 },
    { 1, 1, 1, 1, 32, 32 },
};
static const s64 review_gdiag_5[16][6] = {
    { 0, 0, 0, 0, 1191936, 985408 },
    { 1, 1, 0, 0, 5280, 4128 },
    { 1, 1, 0, 1, 2016, 2016 },
    { 1, 1, 1, 0, 24052, 24052 },
    { 1, 1, 1, 1, 6668, 6668 },
    { 1, 4, 0, 0, 776, 776 },
    { 1, 4, 0, 1, 120, 120 },
    { 1, 4, 1, 0, 4328, 4328 },
    { 1, 4, 1, 1, 408, 408 },
    { 1, 5, 1, 0, 400, 400 },
    { 1, 5, 1, 1, 112, 112 },
    { 1, 16, 0, 0, 23296, 768 },
    { 1, 16, 0, 1, 256, 256 },
    { 1, 16, 1, 0, 1024, 1024 },
    { 1, 16, 1, 1, 23552, 23552 },
    { 1, 17, 1, 1, 1024, 1024 },
};
static const s64 review_gdiag_6[28][6] = {
    { 0, 0, 0, 0, 136687296, 114999482 },
    { 1, 1, 0, 0, 1003462, 827238 },
    { 1, 1, 0, 1, 361514, 361514 },
    { 1, 1, 1, 0, 2739976, 2739976 },
    { 1, 1, 1, 1, 796232, 796232 },
    { 1, 4, 0, 0, 188272, 170704 },
    { 1, 4, 0, 1, 93968, 93968 },
    { 1, 4, 1, 0, 600994, 600994 },
    { 1, 4, 1, 1, 173534, 173534 },
    { 1, 5, 0, 0, 3256, 3256 },
    { 1, 5, 0, 1, 4424, 4424 },
    { 1, 5, 1, 0, 64367, 64367 },
    { 1, 5, 1, 1, 50833, 50833 },
    { 1, 16, 0, 0, 7559840, 450624 },
    { 1, 16, 0, 1, 2249440, 2249440 },
    { 1, 16, 1, 0, 4447104, 4447104 },
    { 1, 16, 1, 1, 6815488, 6815488 },
    { 1, 17, 0, 0, 18432, 0 },
    { 1, 17, 0, 1, 3072, 3072 },
    { 1, 17, 1, 0, 118656, 118656 },
    { 1, 17, 1, 1, 545920, 545920 },
    { 1, 20, 1, 0, 33664, 33664 },
    { 1, 20, 1, 1, 87168, 87168 },
    { 1, 21, 1, 0, 2304, 2304 },
    { 1, 21, 1, 1, 7936, 7936 },
    { 1, 32, 0, 0, 5624, 2800 },
    { 1, 32, 0, 1, 10568, 10568 },
    { 1, 33, 1, 1, 192, 192 },
};

/* the same oracle: displaced arcs of crossed pairs with |F| >= 2,
 * [four, five, six][level][adjective, genitive][dependent first, head
 * first] = { arcs, RETRIEVED } */
static const s64 review_arcs_f2[3][NLEVELS][2][2][2] = {
    { /* up to four words */
      { { { 240, 234 }, { 240, 70 } }, { { 6144, 6016 }, { 6144, 896 } } },
      { { { 240, 240 }, { 240, 240 } }, { { 6144, 6144 }, { 6144, 1024 } } },
      { { { 240, 200 }, { 240, 90 } }, { { 6144, 5120 }, { 6144, 0 } } },
      { { { 240, 200 }, { 240, 40 } }, { { 6144, 5120 }, { 6144, 0 } } },
    },
    { /* up to five words */
      { { { 248424, 228530 }, { 248424, 187558 } }, { { 1923072, 1814208 }, { 1923072, 118144 } } },
      { { { 248424, 236136 }, { 248424, 248424 } }, { { 1923072, 1873920 }, { 1923072, 136192 } } },
      { { { 248424, 188408 }, { 248424, 149596 } }, { { 1923072, 1484608 }, { 1923072, 4096 } } },
      { { { 248424, 188228 }, { 248424, 15432 } }, { { 1923072, 1475584 }, { 1923072, 4096 } } },
    },
    { /* six-word pairs */
      { { { 84991776, 73815074 }, { 84991776, 64920246 } }, { { 409337856, 361743680 }, { 409337856, 13158400 } } },
      { { { 84991776, 76914464 }, { 84991776, 83130144 } }, { { 409337856, 376438784 }, { 409337856, 15179776 } } },
      { { { 84991776, 59294646 }, { 84991776, 45503825 } }, { { 409337856, 289893152 }, { 409337856, 1220608 } } },
      { { { 84991776, 58699264 }, { 84991776, 2699775 } }, { { 409337856, 285265920 }, { 409337856, 1220608 } } },
    },
};

/* the demo's genitive cell == the review's rows, and no other key
 * nonzero */
static int
gdiag_matches (
    const Acc *A,
    const s64 (*rows)[6],
    int        nrows)
{
    int r, it, ro, pd, px, nonzero = 0;

    for (r = 0; r < nrows; r++) {
        const s64 *x = rows[r];
        const s64 *y = A->gdiag[x[0]][x[1]][x[2]][x[3]];

        if (y[0] != x[4] || y[1] != x[5]) return 0;
    }
    for (it = 0; it < 2; it++)
        for (ro = 0; ro < 64; ro++)
            for (pd = 0; pd < 2; pd++)
                for (px = 0; px < 2; px++)
                    nonzero += A->gdiag[it][ro][pd][px][0] != 0;
    return nonzero == nrows;
}

static int
arcs_f2_match (
    const Acc *const *lv4,
    int               set)
{
    int lv, k, i, j;

    for (lv = 0; lv < NLEVELS; lv++)
        for (k = 0; k < 2; k++)
            for (i = 0; i < 2; i++)
                for (j = 0; j < 2; j++)
                    if (lv4[0]->arcs_f2[lv][k][i][j]
                        != review_arcs_f2[set][lv][k][i][j])
                        return 0;
    return 1;
}

/* T4 (review M1): the genitive cell decomposed */
static void
print_genitive_cell (
    const Acc *A)
{
    s64  rn = 0, rm = 0, gn = 0, gm = 0, on = 0, om = 0, pn = 0, pm = 0;
    int  ro, pd, px;
    char b[48];

    for (ro = 0; ro < 64; ro++)
        for (pd = 0; pd < 2; pd++)
            for (px = 0; px < 2; px++) {
                const s64 *r = A->gdiag[0][ro][pd][px];
                const s64 *x = A->gdiag[1][ro][pd][px];

                rn += r[0];
                rm += r[1];
                if ((ro & (1 << (R_NMOD - 1))) && !pd && !px) {
                    gn += x[0];
                    gm += x[1];
                } else {
                    on += x[0];
                    om += x[1];
                }
                if (pd) {
                    pn += x[0];
                    pm += x[1];
                }
            }
    printf("\n  the genitive cell (genitives only, dependent first, COMPETED; "
        "L0, |F| >= 2) by its interferers (T4 review M1):\n");
    bfmt(b, 48, rm, rn);
    printf("  %-64s %26s\n", "RETRIEVED", b);
    bfmt(b, 48, gm, gn);
    printf("  %-64s %26s\n", "INTERFERED by a genitive no projective "
        "alternative uses", b);
    bfmt(b, 48, om, on);
    printf("  %-64s %26s\n", "INTERFERED otherwise", b);
    bfmt(b, 48, pm, pn);
    printf("  %-64s %26s\n", "  of which a projective alternative hangs d "
        "on an interferer", b);
}

/* U1 in exact terms, by direction, and on the |F| >= 2 population (T4
 * review M2, L6) */
static void
print_u1_more (
    const Acc *const *lv4,
    Piscina          *p)
{
    s64  r[3], n = 0, rf[3], nf = 0;
    int  lv, i;
    char g1[32], g2[32], k[32];

    for (lv = 0; lv < 3; lv++) {
        r[lv] = lv4[lv]->arcs[0][0][1] + lv4[lv]->arcs[0][1][1];
        rf[lv] = lv4[0]->arcs_f2[lv][0][0][1] + lv4[0]->arcs_f2[lv][0][1][1];
    }
    n = lv4[0]->arcs[0][0][0] + lv4[0]->arcs[0][1][0];
    nf = lv4[0]->arcs_f2[0][0][0][0] + lv4[0]->arcs_f2[0][0][1][0];
    gap_fmt(g1, 32, r[0], n, r[2], n, p);
    gap_fmt(g2, 32, 2 * r[1], n, 2 * r[0], n, p);
    pct(k, 32, r[0] - r[2], r[1] - r[2]);
    printf("  U1 exactly: I(L0) - I(L2) %s points, 2 (I(L1) - I(L0)) %s; "
        "Latin keeps %s of the gap between L1 and L2\n", g1, g2, k);
    for (i = 0; i < 2; i++) {
        s64 x0 = lv4[0]->arcs[0][i][1], x1 = lv4[1]->arcs[0][i][1];
        s64 x2 = lv4[2]->arcs[0][i][1];

        pct(k, 32, x0 - x2, x1 - x2);
        printf("    %s adjective arcs: Latin keeps %s\n", dir_name[i], k);
    }
    gap_fmt(g1, 32, rf[0], nf, rf[2], nf, p);
    gap_fmt(g2, 32, 2 * rf[1], nf, 2 * rf[0], nf, p);
    pct(k, 32, rf[0], nf);
    printf("  U1 read on the |F| >= 2 population (%lld adjective arcs; "
        "descriptive): I(L0) %s, I(L0) - I(L2) %s against 2 (I(L1) - I(L0)) "
        "%s - the rule %s\n", (long long)nf, k, g1, g2,
        verdict_name[u1_rule(rf[0], rf[1], rf[2], nf)]);
}

static void
part_measures_five (void)
{
    static Acc pool[NLEVELS];
    const Acc *lv4[NLEVELS];
    s64        v[NV_FULL], cross_by_len[MAXW + 1];
    Acc        orc[NLEVELS];
    const Acc *olv4[NLEVELS];
    Piscina   *p = piscina_generare_dynamicum("d134_quinque", 1 << 20);
    int        lv, n, nv, vc[3], vo[3];
    clock_t    t0 = clock();
    char       title[64];

    printf("\n--- measures (task 2): up to %d words, DESCRIPTIVE ---\n",
        gmaxw);
    for (lv = 0; lv < NLEVELS; lv++) {
        memset(acc[lv], 0, sizeof acc[lv]);
        measure_umbrae(lv, 0, gmaxw);
    }
    tempus("pass", t0);
    for (n = 0; n <= MAXW; n++) cross_by_len[n] = meas[0].len_b_den[n];
    measure_checks(quick ? "up to four words" : "up to five words", 0,
        gmaxw, &ck_five, cross_by_len);
    for (lv = 0; lv < NLEVELS; lv++) {
        memset(&pool[lv], 0, sizeof pool[lv]);
        for (n = 0; n <= gmaxw; n++) acc_add(&pool[lv], &acc[lv][n]);
        lv4[lv] = &pool[lv];
    }
    nv = flatten(lv4, NLEVELS, v);
    if (nv != NV_FULL) fatal("vector size");
    check(quick ? "every total up to four words == oracle_d134.py's "
        "(independent; a pin, 1,125 numbers)" : "every total up to five words "
        "== oracle_d134.py's (independent, exhaustive; a pin, 1,125 numbers)",
        check_vector(quick ? "four" : "five", v, oracle_full[quick ? 1 : 0],
            NV_FULL));
    unflatten(oracle_full[quick ? 1 : 0], NLEVELS, orc);
    for (lv = 0; lv < NLEVELS; lv++) olv4[lv] = &orc[lv];
    decide(lv4, vc, p);
    decide(olv4, vo, p);
    check("the rules applied to the oracle's totals give the same three "
        "answers (wiring)", vc[0] == vo[0] && vc[1] == vo[1]
        && vc[2] == vo[2]);
    check(quick ? "up to four words: the genitive cell by its interferers, "
        "and the |F| >= 2 adjective and genitive arc labels, == the T4 "
        "review's own oracle (rec.c; a pin)" : "up to five words: the "
        "genitive cell by its interferers, and the |F| >= 2 adjective and "
        "genitive arc labels, == the T4 review's own oracle (rec.c; a pin)",
        quick ? gdiag_matches(lv4[0], review_gdiag_4, 3)
            && arcs_f2_match(lv4, 0)
        : gdiag_matches(lv4[0], review_gdiag_5, 16) && arcs_f2_match(lv4, 1));
    snprintf(title, sizeof title, "up to %d words", gmaxw);
    print_dist(lv4[0], title);
    for (lv = 0; lv < NLEVELS; lv++) {
        char t2[96];

        snprintf(t2, sizeof t2, "%s, %s", title, level_name[lv]);
        print_cells(lv4[lv], 0, t2);
    }
    print_cells(lv4[0], 1, "L0, labels by OWN RELATION");
    print_l0_extra(lv4[0]);
    print_arcs(lv4, title);
    print_genitive_cell(lv4[0]);
    printf("\n  the registered rules applied at %s - DESCRIPTIVE (registered "
        "at six words):\n", title);
    print_registered(lv4, vc, p);
    print_u1_more(lv4, p);
    piscina_destruere(p);
}

static void
part_measures_six (
    s64 cross6)
{
    const Acc *lv4[NLEVELS], *sub4[NLEVELS], *olv4[NLEVELS];
    Acc        orc[NLEVELS];
    s64        v[NV_SUB], cross_by_len[MAXW + 1];
    Piscina   *p = piscina_generare_dynamicum("d134_sex", 1 << 20);
    int        lv, n, nv, vc[3], vs[3], vo[3];
    clock_t    t0 = clock();

    printf("\n--- measures (task 2): six-word pairs - the REGISTERED "
        "verdicts ---\n");
    for (lv = 0; lv < NLEVELS; lv++) {
        memset(&acc[lv][MAXW], 0, sizeof acc[lv][MAXW]);
        memset(&acc_sub[lv], 0, sizeof acc_sub[lv]);
        measure_umbrae(lv, MAXW, MAXW);
        lv4[lv] = &acc[lv][MAXW];
        sub4[lv] = &acc_sub[lv];
    }
    tempus("pass", t0);
    for (n = 0; n <= MAXW; n++) cross_by_len[n] = 0;
    cross_by_len[MAXW] = cross6;
    measure_checks("six words", MAXW, MAXW, &ck_six, cross_by_len);
    nv = flatten(sub4, 2, v);
    if (nv != NV_SUB) fatal("vector size");
    check("the oracle's subset (2,250 L1 bags): every total == "
        "oracle_d134.py's (independent; B at L0 and L1, labels at every "
        "level; a pin, 837 numbers - PARTIAL)", check_vector("six-subset", v,
            oracle_sub, NV_SUB));
    unflatten(oracle_sub, 2, orc);
    for (lv = 0; lv < NLEVELS; lv++) olv4[lv] = &orc[lv];
    decide(sub4, vs, p);
    decide(olv4, vo, p);
    check("the rules applied to the oracle's subset totals give the same "
        "three answers as to the demo's subset totals (wiring)", vs[0] == vo[0]
        && vs[1] == vo[1] && vs[2] == vo[2]);
    print_dist(lv4[0], "six words");
    for (lv = 0; lv < NLEVELS; lv++) {
        char t2[96];

        snprintf(t2, sizeof t2, "six words, %s", level_name[lv]);
        print_cells(lv4[lv], 0, t2);
    }
    print_cells(lv4[0], 1, "six words, L0, labels by OWN RELATION");
    print_l0_extra(lv4[0]);
    print_arcs(lv4, "six words");
    print_genitive_cell(lv4[0]);
    check("six words: the genitive cell by its interferers, and the "
        "|F| >= 2 adjective and genitive arc labels, == the T4 review's own "
        "exhaustive oracle (rec.c; a pin)", gdiag_matches(lv4[0],
            review_gdiag_6, 28) && arcs_f2_match(lv4, 2));
    printf("\n--- verdicts (registered, six-word pairs, L0, |F| >= 2) ---\n");
    print_registered(lv4, vc, p);
    print_u1_more(lv4, p);
    printf("\n  (the oracle's subset gives U2a %s, U2b %s, U1 %s - partial, "
        "for wiring only)\n", verdict_name[vs[0]], verdict_name[vs[1]],
        verdict_name[vs[2]]);
    piscina_destruere(p);
}

static void
part_umbrae_five (void)
{
    clock_t t0 = clock();
    s64     pairs = 0, crossed = 0;
    int     n;

    printf("\n--- umbrae, width, depth and labels: every pair up to %d words "
        "(checks only) ---\n", gmaxw);
    for (n = 0; n <= gmaxw; n++) pairs += level_pairs[0][n];
    crossed = meas[0].b_den;
    umbra_pass(gmaxw, &ck_five);
    tempus("pass", t0);
    umbra_checks(quick ? "up to four words" : "up to five words", &ck_five,
        pairs, crossed);
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
    s64     proj[NLEVELS], cross[NLEVELS];
    char    msg[400];

    printf("\n--- six words: feasibility (task 0; counts only) ---\n");
    free_entries();
    gmaxw = MAXW;
    t0 = clock();
    generate();
    tempus("generator", t0);
    for (lv = 0; lv < NLEVELS; lv++) {
        raw += gen_entries[lv].n;
        gcap += gen_entries[lv].cap;
    }
    printf("  generator: %ld abstract analyses up to %d words; %ld "
        "placements before sorting, four levels (%.0f MB)\n", gen_emitted,
        gmaxw, raw, megabytes(gcap));
    for (lv = 0; lv < NLEVELS; lv++) {
        long gd, ad, i;
        int  same;

        t0 = clock();
        gd = sort_unique(&gen_entries[lv]);
        tempus("generator sort", t0);
        tried = 0;
        t0 = clock();
        analyse_all_bags(lv, &tried);
        ad = sort_unique(&an_entries[lv]);
        tempus("analyser", t0);
        same = gen_entries[lv].n == an_entries[lv].n;
        for (i = 0; same && i < gen_entries[lv].n; i++)
            if (entry_cmp(&gen_entries[lv].e[i], &an_entries[lv].e[i]) != 0)
                same = 0;
        printf("  %-17s generator: %ld repeats; analyser over %ld bags, %ld "
            "analyses (%.0f MB)\n", level_name[lv], gd, tried,
            an_entries[lv].n, megabytes(an_entries[lv].cap));
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
        t0 = clock();
        sweep_length(lv, MAXW, &proj[lv], &cross[lv]);
        printf("  %-17s projective %10lld  crossed %10lld\n",
            level_name[lv], (long long)proj[lv], (long long)cross[lv]);
        tempus("sweep", t0);
        if (proj[lv] != proj[0] || cross[lv] != cross[0]
            || proj[lv] + cross[lv] != (s64)level_pairs[lv][MAXW])
            same_split = 0;
    }
    check("six words: projective + crossed == the six-word pairs, and the "
        "split is the same at every level", same_split);
    {
        s64 pairs = 0;

        printf("\n  umbrae, width, depth and labels: every pair up to six "
            "words (checks only)\n");
        for (n = 0; n <= MAXW; n++) pairs += level_pairs[0][n];
        t0 = clock();
        umbra_pass(MAXW, &ck_six);
        tempus("pass", t0);
        umbra_checks("up to six words", &ck_six, pairs, meas[0].b_den
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
                if (rank % SUBSET_K == 0) {
                    if (n_sel == MAXSEL) fatal("subset too large");
                    sel_codes[n_sel++] = an_entries[1].e[i].bag;
                }
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
    part_measures_six(cross[0]);
}

int
main (void)
{
    quick = getenv("D134_CELER") != NULL;
    show_times = getenv("D134_TEMPORA") != NULL;
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
    part_measures_five();
    rules_check();
    if (!quick && getenv("D134_QUINQUE") == NULL) part_six();
    printf("\n=== %d pass, %d fail ===\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
