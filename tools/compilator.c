/* compilator.c - bin/compilator: 'clang -c' per thesaurum contentorum
 * (fabrica plan 2 T4). Argumenta eadem ac 'clang -c ... -o <obj>
 * <fons>': runner verbum unum mutat. Logica in lib/compilator.c
 * (bibliotheca, fabrica-6 T4); hic argv, provenientia, thesaurus
 * ordinarius, et regressus ad clang ipsum (argumenta non cacheabilia:
 * sine -c/-o, fontes plures, -E/-S/-M*; aut identitas ignota). */
/* <aedilis obiectum="build/fabrica/provenientia/compilator.c"/> */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "compilator.h"
#include "processus.h"
#include "provenientia.h"

#include <stdio.h>
#include <stdlib.h>

#define THESAURUS_ORDINARIUS "build/aedilis/obiecta"

externus constans ProvenientiaRelatio provenientia_compilator;

/* clang ipse fit (processus_transformare): numquam redit si successit */
interior s32
_clang_ipse (
      integer   argc,
    character** argv)
{
    constans character* transformanda[CCLVI];
    constans character* clang;
               integer  i;

    clang = getenv("FABRICA_CLANG");
    si (clang == NIHIL || clang[0] == '\0')
    {
        clang = "clang";
    }
    si (argc >= CCLV)
    {
        fprintf(stderr, "compilator: argumenta nimis multa\n");
        redde I;
    }
    transformanda[0] = clang;
    per (i = I; i < argc; i++)
    {
        transformanda[i] = argv[i];
    }
    transformanda[argc] = NIHIL;
    (vacuum)processus_transformare(transformanda);
    fprintf(stderr, "compilator: %s exsequi nequit\n", clang);
    redde I;
}

s32
principale (
      integer   argc,
    character** argv)
{
               Piscina* piscina;
            Compilator* compilator;
    CompilatorResultus  resultus;
    constans character* radix;
    constans character* constans* argumenta;

    si (provenientia_respondere(argc, argv, &provenientia_compilator))
    {
        redde ZEPHYRUM;
    }
    argumenta  = (constans character* constans*)(argv + I);
    piscina    = piscina_generare_dynamicum("compilator", 16777216);
    radix      = getenv("FABRICA_THESAURUS");
    si (radix == NIHIL || radix[0] == '\0')
    {
        radix = THESAURUS_ORDINARIUS;
    }
    compilator = (piscina != NIHIL)
        ? compilator_aperire(piscina, radix, NIHIL) : NIHIL;
    si (   compilator == NIHIL
        || !compilator_cacheabile(argumenta, (i32)(argc - I)))
    {
        redde _clang_ipse(argc, argv);
    }
    resultus = compilator_compilare(compilator, argumenta,
        (i32)(argc - I));
    si (resultus.codex < ZEPHYRUM)
    {
        /* identitas ignota: clang ipse (ut olim) */
        redde _clang_ipse(argc, argv);
    }
    (vacuum)fwrite(resultus.effusio.datum, I,
        (memoriae_index)resultus.effusio.mensura, stdout);
    (vacuum)fwrite(resultus.erratum.datum, I,
        (memoriae_index)resultus.erratum.mensura, stderr);
    piscina_destruere(piscina);
    redde resultus.codex;
}
