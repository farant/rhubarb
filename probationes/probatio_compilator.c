/* probatio_compilator.c - compilator_compilare_in (parcum …T77EH4):
 * allocationes vocationis (plagulae lectae, argumenta, effusio,
 * erratum) in piscina VOCANTIS; piscina compilatoris per vocationes
 * thesauri non crescit. Fabrica membrum quodque in piscina temporaria
 * agit: sine hoc ~I MB per compilationem usque ad finem cursus.
 *
 * Plagulae fictae et thesaurus in area sectionis (credo_area); clang
 * verus (compilatio prima vera, deinde ex thesauro).
 */
#include "latina.h"
#include "lectiones.h"   /* lectiones_fopen: canalis (examen 96) */
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "compilator.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>

/* via in area sectionis; in piscina contextus */
interior constans character*
_via (
         CredoContextus* c,
     constans character* titulus)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
        (memoriae_index)CXXVIII);

    chorda_aedificator_appendere_literis(a, credo_area(c));
    chorda_aedificator_appendere_character(a, '/');
    chorda_aedificator_appendere_literis(a, titulus);
    redde chorda_ut_cstr(chorda_aedificator_finire(a), c->piscina);
}

/* plagulam in area scribere; via reddita */
interior constans character*
_plagula (
         CredoContextus* c,
     constans character* titulus,
     constans character* contentum)
{
    constans character* via = _via(c, titulus);
                  FILE* f;

    f = lectiones_fopen(via, "w");
    si (f != NIHIL)
    {
        fputs(contentum, f);
        fclose(f);
    }
    redde via;
}

/* argumenta compilatoris SINE 'clang' (ut fabrica et bin/compilator:
 * 'clang' ipse fons alter videretur): -I<area> -c fons -o obiectum */
interior i32
_argumenta (
         CredoContextus*  c,
     constans character*  fons,
     constans character*  obiectum,
     constans character** argv)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
        (memoriae_index)CXXVIII);

    chorda_aedificator_appendere_literis(a, "-I");
    chorda_aedificator_appendere_literis(a, credo_area(c));
    argv[0] = chorda_ut_cstr(chorda_aedificator_finire(a),
        c->piscina);
    argv[I]    = "-c";
    argv[II]   = fons;
    argv[III]  = "-o";
    argv[IV]   = obiectum;
    argv[V]    = NIHIL;
    redde V;
}

/* I. vocationes ex thesauro: piscina compilatoris immota, vocantis
 * usa; compilator_compilare (sine piscina) in propria, ut olim */
interior vacuum
_sectio_piscina_vocantis (
    CredoContextus* c)
{
               Piscina* propria;
               Piscina* vocatio;
            Compilator* compilator;
    CompilatorResultus  r;
    constans character* argv[VII];
    constans character* fons;
        memoriae_index  ante;
                   i32  numerus;
                   i32  k;

    (vacuum)_plagula(c, "h.h", "#define VALOR 7\n");
    fons = _plagula(c, "x.c",
        "#include \"h.h\"\nint valor(void) { return VALOR; }\n");
    numerus = _argumenta(c, fons, _via(c, "x.o"), argv);
    propria = piscina_generare_dynamicum("compilator_proprius", 65536);
    vocatio = piscina_generare_dynamicum("compilator_vocatio", 65536);
    CREDO_NECESSE(propria != NIHIL && vocatio != NIHIL);
    compilator = compilator_aperire(propria, _via(c, "thesaurus"),
        NIHIL);
    CREDO_NECESSE(compilator != NIHIL);

    /* prima: compilatio vera */
    r = compilator_compilare_in(compilator, argv, numerus, vocatio);
    CREDO_VERUM(r.codex == ZEPHYRUM);
    CREDO_FALSUM(r.ex_thesauro);
    piscina_vacare(vocatio);

    /* sequentes ex thesauro: propria non crescit */
    ante = piscina_summa_usus(propria);
    per (k = ZEPHYRUM; k < III; k++)
    {
        r = compilator_compilare_in(compilator, argv, numerus, vocatio);
        CREDO_VERUM(r.codex == ZEPHYRUM);
        CREDO_VERUM(r.ex_thesauro);
        CREDO_VERUM(piscina_summa_usus(vocatio) > ZEPHYRUM);
        piscina_vacare(vocatio);
    }
    CREDO_VERUM(piscina_summa_usus(propria) == ante);

    /* sine piscina vocantis: in propria (ut olim) */
    r = compilator_compilare(compilator, argv, numerus);
    CREDO_VERUM(r.codex == ZEPHYRUM && r.ex_thesauro);
    CREDO_VERUM(piscina_summa_usus(propria) > ante);

    piscina_destruere(vocatio);
    piscina_destruere(propria);
}

/* II. erratum clang in piscina vocantis: legibile, propria immota */
interior vacuum
_sectio_erratum (
    CredoContextus* c)
{
               Piscina* propria;
               Piscina* vocatio;
            Compilator* compilator;
    CompilatorResultus  r;
    constans character* argv[VII];
        memoriae_index  ante;
                   i32  numerus;

    numerus = _argumenta(c, _plagula(c, "y.c",
        "int y(void) { return 1 }\n"), _via(c, "y.o"), argv);
    propria = piscina_generare_dynamicum("compilator_proprius", 65536);
    vocatio = piscina_generare_dynamicum("compilator_vocatio", 65536);
    CREDO_NECESSE(propria != NIHIL && vocatio != NIHIL);
    compilator = compilator_aperire(propria, _via(c, "thesaurus"),
        NIHIL);
    CREDO_NECESSE(compilator != NIHIL);
    ante  = piscina_summa_usus(propria);
    r     = compilator_compilare_in(compilator, argv, numerus, vocatio);
    CREDO_VERUM(r.codex != ZEPHYRUM);
    CREDO_VERUM(r.erratum.mensura > ZEPHYRUM);
    CREDO_VERUM(piscina_summa_usus(propria) == ante);
    piscina_destruere(vocatio);
    piscina_destruere(propria);
}

/* III. SINE COLLOCATIONE (nexus ex thesauro): miss compilat in exitum
 * (via = exitus); hit exitum NON scribit, via = blobus thesauri;
 * compilare (cum collocatione) exitum ut olim scribit */
interior vacuum
_sectio_sine_collocatione (
    CredoContextus* c)
{
               Piscina* propria;
               Piscina* vocatio;
            Compilator* compilator;
    CompilatorResultus  r;
    constans character* argv[VII];
    constans character* exitus;
                   i32  numerus;

    exitus  = _via(c, "z.o");
    numerus = _argumenta(c, _plagula(c, "z.c",
        "int z(void) { return 3; }\n"), exitus, argv);
    propria = piscina_generare_dynamicum("compilator_proprius", 65536);
    vocatio = piscina_generare_dynamicum("compilator_vocatio", 65536);
    CREDO_NECESSE(propria != NIHIL && vocatio != NIHIL);
    compilator = compilator_aperire(propria, _via(c, "thesaurus"),
        NIHIL);
    CREDO_NECESSE(compilator != NIHIL);

    /* miss: compilatur in exitum */
    r = compilator_compilare_sine_collocatione(compilator, argv,
        numerus, vocatio);
    CREDO_VERUM(r.codex == ZEPHYRUM && !r.ex_thesauro);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.via, exitus);
    CREDO_VERUM(filum_existit(exitus));

    /* hit: exitus deletus NON rescribitur, via = blobus */
    (vacuum)filum_delere(exitus);
    r = compilator_compilare_sine_collocatione(compilator, argv,
        numerus, vocatio);
    CREDO_VERUM(r.codex == ZEPHYRUM && r.ex_thesauro);
    CREDO_FALSUM(filum_existit(exitus));
    CREDO_VERUM(strstr(chorda_ut_cstr(r.via, vocatio), "/blobi/")
        != NIHIL);
    CREDO_VERUM(filum_existit(chorda_ut_cstr(r.via, vocatio)));

    /* cum collocatione (ut olim): exitus scribitur, via = exitus */
    r = compilator_compilare_in(compilator, argv, numerus, vocatio);
    CREDO_VERUM(r.codex == ZEPHYRUM && r.ex_thesauro);
    CREDO_VERUM(filum_existit(exitus));
    CREDO_CHORDA_AEQUALIS_LITERIS(r.via, exitus);

    piscina_destruere(vocatio);
    piscina_destruere(propria);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "piscina vocantis",
      _sectio_piscina_vocantis,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "erratum",
      _sectio_erratum,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine collocatione",
      _sectio_sine_collocatione,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("compilator", SECTIONES);
}
