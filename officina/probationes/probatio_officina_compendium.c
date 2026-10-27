/* probatio_officina_compendium.c - compendium capitis (communis
 * legato et briar): extractio declarationum gradus supremi ex
 * parsura silvae, ordo plagulae, contractio in lineam unam */

#include "compendium.h"
#include "credo.h"

#include <stdio.h>
#include <string.h>

interior constans character* FIXURA =
    "/* x.h - fixura */\n"                          /* 1 */
    "#ifndef X_H\n"                                 /* 2 */
    "#define X_H\n"                                 /* 3 */
    "\n"                                            /* 4 */
    "/* Genus coloris */\n"                         /* 5 */
    "typedef enum { RUBER, VIRIDIS } Color;\n"      /* 6 */
    "\n"                                            /* 7 */
    "/* Punctum in plano */\n"                      /* 8 */
    "typedef struct {\n"                            /* 9 */
    "    int x;\n"                                  /* 10 */
    "    int y;\n"                                  /* 11 */
    "} Punctum;\n"                                  /* 12 */
    "\n"                                            /* 13 */
    "/* Punctum creare\n"                           /* 14 */
    " * cum coordinatis */\n"                       /* 15 */
    "Punctum\n"                                     /* 16 */
    "punctum_creare (\n"                            /* 17 */
    "    int x,\n"                                  /* 18 */
    "       int y);\n"                              /* 19 */
    "\n"                                            /* 20 */
    "int punctum_distantia (const Punctum* a, const Punctum* b);\n"
    "\n"                                            /* 22 */
    "extern int numerus_punctorum;\n"               /* 23 */
    "\n"                                            /* 24 */
    "static int\n"                                  /* 25 */
    "adiutor (int a)\n"                             /* 26 */
    "{\n"                                           /* 27 */
    "    return a + 1;\n"                           /* 28 */
    "}\n"                                           /* 29 */
    "\n"                                            /* 30 */
    "/* Arbor opaca */\n"                           /* 31 */
    "typedef struct Arbor Arbor;\n"                 /* 32 */
    "\n"                                            /* 33 */
    "/* Arbor definita */\n"                        /* 34 */
    "struct Arbor {\n"                              /* 35 */
    "    int radix;\n"                              /* 36 */
    "};\n"                                          /* 37 */
    "\n"
    "#endif\n";

/* FIXURA SUITAE: FIXURA semel parsa (silva + semantica), asserta in
 * parare_suitae ut olim ante blocos; sectiones parsuram et semanticam
 * per monstratorem constantem LEGUNT (compendium_declarationes
 * constans accipit) - declarationes quaeque sectio suas extrahit. */
nomen structura {
      SilvaPiscina* sp;
      SilvaParsura* parsura;
    SilvaSemantica* sem;
} FixuraParsurae;

/* declaratio tituli (prima registratio) aut NIHIL */
interior constans CompendiumDeclaratio*
_invenire (
                   Xar* declarationes,
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(declarationes); i++)
    {
        constans CompendiumDeclaratio* d =
            (constans CompendiumDeclaratio*)xar_obtinere(declarationes,
                i);

        si (chorda_aequalis_literis(d->titulus, titulus))
        {
            redde d;
        }
    }
    redde NIHIL;
}

/* extentum fixurae ut chorda */
interior chorda
_extentum (
    s32 initium,
    s32 finis)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l        = FIXURA;
    c.datum    = u.m + initium;
    c.mensura  = (i32)(finis - initium);
    redde c;
}

interior chorda
_contractum (
     constans CompendiumDeclaratio* d,
                               i32  tectum,
                           Piscina* pn)
{
    redde compendium_contrahere(_extentum(d->corpus_initium,
        d->corpus_finis), tectum, pn);
}

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    FixuraParsurae* f;
     SilvaExpansio* exp;

    f = (FixuraParsurae*)piscina_allocare(c->piscina,
        magnitudo(FixuraParsurae));
    si (f == NIHIL)
    {
        redde;
    }
    f->sp = silva_piscina_generare_dynamicum("probatio_compendium",
        (size_t)8388608);
    si (f->sp == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde;
    }
    c->datum  = f;
    exp       = silva_expansio_creare(f->sp);
    f->parsura  = silva_parsare_cum_expansione(f->sp, exp, "x.h",
        FIXURA, (insignatus integer)strlen(FIXURA),
        &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
    CREDO_VERUM (f->parsura != NIHIL && f->parsura->numerus_errorum
        == ZEPHYRUM);
    f->sem = silva_c89_semantica_analysare(f->sp, f->parsura);
    CREDO_VERUM (f->sem != NIHIL);
}

interior vacuum
_suitam_purgare (
    CredoContextus* c)
{
    FixuraParsurae* f = (FixuraParsurae*)c->datum;

    si (f != NIHIL && f->sp != NIHIL)
    {
        silva_piscina_destruere(f->sp);
    }
}


/* ==================================================
 * PROBARE: declarationes gradus supremi + contractio
 * ================================================== */

interior vacuum
_declarationes_probare (
    CredoContextus* c)
{
    constans FixuraParsurae* f   = (constans FixuraParsurae*)c->suita;
                    Piscina* pn  = c->piscina;
                       Xar* declarationes;
    constans CompendiumDeclaratio* d;

    si (f == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    declarationes = compendium_declarationes(f->parsura, f->sem, pn);
    CREDO_VERUM (declarationes != NIHIL);
    CREDO_VERUM (_invenire(declarationes, "Color") != NIHIL);
    CREDO_VERUM (_invenire(declarationes, "RUBER") != NIHIL);
    CREDO_VERUM (_invenire(declarationes, "numerus_punctorum")
        != NIHIL);
    /* parametri et locales non sunt gradus supremi */
    CREDO_VERUM (_invenire(declarationes, "x") == NIHIL);
    CREDO_VERUM (_invenire(declarationes, "a") == NIHIL);

    d = _invenire(declarationes, "punctum_creare");
    CREDO_VERUM (d != NIHIL);
    si (d != NIHIL)
    {
        CREDO_AEQUALIS_S32 (d->genus, (s32)SYMBOLUM_FUNCTIO);
        CREDO_FALSUM (d->est_definitio);
        /* lineae ex RADICE declarationis (typus reditus incluso) */
        CREDO_AEQUALIS_I32 (d->linea_a, XVI);
        CREDO_AEQUALIS_I32 (d->linea_b, XIX);
        CREDO_CHORDA_AEQUALIS_LITERIS (_extentum(
            d->commentarium_initium, d->commentarium_finis),
            "/* Punctum creare\n * cum coordinatis */");
        /* contractio: nullum spatium post '(' nec ante ')' */
        CREDO_CHORDA_AEQUALIS_LITERIS (_contractum(d, CCXL, pn),
            "Punctum punctum_creare (int x, int y);");
        /* symbolum silvae vivit dum parsura (signatura legati) */
        CREDO_VERUM (d->symbolum != NIHIL
            && d->symbolum->genus == (int)SYMBOLUM_FUNCTIO);
    }
    d = _invenire(declarationes, "punctum_distantia");
    CREDO_VERUM (d != NIHIL && d->commentarium_initium == (s32)-I);
    d = _invenire(declarationes, "adiutor");
    CREDO_VERUM (d != NIHIL && d->est_definitio);
    si (d != NIHIL)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS (_contractum(d, CCXL, pn),
            "static int adiutor (int a) {...}");
    }
    /* typedef structurae: corpus = definitio tota, commentarium eius */
    d = _invenire(declarationes, "Punctum");
    CREDO_VERUM (d != NIHIL);
    si (d != NIHIL)
    {
        CREDO_AEQUALIS_I32 (d->linea_a, IX);
        CREDO_CHORDA_AEQUALIS_LITERIS (_contractum(d, CCXL, pn),
            "typedef struct {...} Punctum;");
        CREDO_CHORDA_AEQUALIS_LITERIS (_extentum(
            d->commentarium_initium, d->commentarium_finis),
            "/* Punctum in plano */");
    }
    /* typedef opacum: charta ad DEFINITIONEM structurae repungitur
     * (corpus, lineae, commentarium definitionis) */
    d = _invenire(declarationes, "Arbor");
    CREDO_VERUM (d != NIHIL);
    si (d != NIHIL)
    {
        CREDO_AEQUALIS_I32 (d->linea_a, XXXV);
        CREDO_CHORDA_AEQUALIS_LITERIS (_contractum(d, CCXL, pn),
            "struct Arbor {...} ;");
        CREDO_CHORDA_AEQUALIS_LITERIS (_extentum(
            d->commentarium_initium, d->commentarium_finis),
            "/* Arbor definita */");
    }
    /* praecisio ultra tectum: XII octeti exacte, deinde " ..." */
    d = _invenire(declarationes, "punctum_distantia");
    si (d != NIHIL)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS (_contractum(d, XII, pn),
            "int punctum_ ...");
    }
}


/* ==================================================
 * PROBARE: ordo plagulae
 * ================================================== */

interior vacuum
_ordinem_probare (
    CredoContextus* c)
{
    constans FixuraParsurae* f   = (constans FixuraParsurae*)c->suita;
                    Piscina* pn  = c->piscina;
                       Xar* declarationes;
                       Xar* ordo;

    si (f == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    /* sine assertione: sectio 'declarationes' eas iudicat */
    declarationes = compendium_declarationes(f->parsura, f->sem, pn);
    si (declarationes == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    ordo = compendium_ordo(declarationes, pn);
    CREDO_VERUM (ordo != NIHIL);
    /* Color (RUBER, VIRIDIS eodem corpore - semel, typus primus),
     * Punctum, punctum_creare, punctum_distantia,
     * numerus_punctorum, adiutor, Arbor (ad definitionem) */
    CREDO_AEQUALIS_I32 (xar_numerus(ordo), VII);
    si (xar_numerus(ordo) == VII)
    {
        constans character* sperata[VII];
                       i32  i;

        sperata[ZEPHYRUM]  = "Color";
        sperata[I]         = "Punctum";
        sperata[II]        = "punctum_creare";
        sperata[III]       = "punctum_distantia";
        sperata[IV]        = "numerus_punctorum";
        sperata[V]         = "adiutor";
        sperata[VI]        = "Arbor";
        per (i = ZEPHYRUM; i < VII; i++)
        {
            constans CompendiumDeclaratio* e =
                (constans CompendiumDeclaratio*)xar_obtinere(
                    declarationes, *(i32*)xar_obtinere(ordo, i));

            CREDO_CHORDA_AEQUALIS_LITERIS (e->titulus, sperata[i]);
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "declarationes", _declarationes_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ordinem", _ordinem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("officina_compendium", SECTIONES,
        _suitam_parare, _suitam_purgare);
}
