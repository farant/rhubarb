/* compilator.c - bin/compilator: 'clang -c' per thesaurum contentorum
 * (fabrica plan 2 T4). Argumenta eadem ac 'clang -c ... -o <obj>
 * <fons>': runner verbum unum mutat. Logica in lib/compilator.c
 * (bibliotheca, fabrica-6 T4); hic argv, provenientia, thesaurus
 * ordinarius, et regressus ad clang ipsum (argumenta non cacheabilia:
 * sine -c/-o, fontes plures, -E/-S/-M*; aut identitas ignota).
 *
 * PLURES (2026-10-23): 'bin/compilator --plures <lista>' - linea
 * quaeque listae vocatio integra (argumenta spatiis separata, ut
 * compile_tests.sh bibliothecae.vocationes scribit). Thesaurus et
 * identitas clang SEMEL per processum: vocatio singula ~XXII ms
 * (processus, identitas), CCCLII in porta radicis. Lineae omnes
 * curruntur etiam post fracturam (ut xargs); exitus I si qua fracta.
 * Capita communia semel lecta et sigillata
 * (compilator_memoriam_incipere); piscina non reficitur per lineam -
 * thesaurus eadem utitur. */
/* <aedilis obiectum="build/fabrica/provenientia/compilator.c"/> */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "compilator.h"
#include "processus.h"
#include "provenientia.h"
#include "filum.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THESAURUS_ORDINARIUS "build/aedilis/obiecta"
#define ARGUMENTA_MAXIMA     CCLVI

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

/* clang ut filius (lineae --plures non cacheabiles): effusio
 * transmissa, codex exitus redditus */
interior s32
_clang_filius (
    constans character* constans* argumenta,
                              i32 numerus,
                          Piscina* piscina)
{
    constans character* vector[ARGUMENTA_MAXIMA + I];
    constans character* clang;
     ProcessusResultus  resultus;
                   i32  i;

    clang = getenv("FABRICA_CLANG");
    si (clang == NIHIL || clang[0] == '\0')
    {
        clang = "clang";
    }
    vector[0] = clang;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        vector[i + I] = argumenta[i];
    }
    vector[numerus + I] = NIHIL;

    resultus = processus_exsequi(vector, ZEPHYRUM, piscina);
    (vacuum)fwrite(resultus.effusio.datum, I,
        (memoriae_index)resultus.effusio.mensura, stdout);
    (vacuum)fwrite(resultus.erratum.datum, I,
        (memoriae_index)resultus.erratum.mensura, stderr);
    si (!resultus.successus)
    {
        fprintf(stderr, "compilator: %s exsequi nequit\n", clang);
        redde I;
    }
    redde (s32)resultus.codex_exitus;
}

/* lineae listae per thesaurum unum; exitus I si qua fracta */
interior s32
_plures (
    constans character* via_listae,
            Compilator* compilator,
               Piscina* piscina)
{
    chorda lista;
       i32 initium;
       i32 i;
       s32 exitus = ZEPHYRUM;

    lista = filum_legere_totum(via_listae, piscina);
    si (lista.datum == NIHIL)
    {
        fprintf(stderr, "compilator: --plures: %s legi nequit\n",
            via_listae);
        redde I;
    }
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= lista.mensura; i++)
    {
        constans character* argumenta[ARGUMENTA_MAXIMA];
                 character* linea;
                       i32  numerus;
                       i32  k;
                       s32  codex;

        si (i < lista.mensura && lista.datum[i] != '\n')
        {
            perge;
        }
        linea = chorda_ut_cstr(chorda_sectio(lista, initium, i),
            piscina);
        initium = i + I;
        numerus = ZEPHYRUM;
        per (k = ZEPHYRUM; linea[k] != '\0'; k++)
        {
            si (linea[k] == ' ' || linea[k] == '\t' || linea[k] == '\r')
            {
                linea[k] = '\0';
            }
            alioquin si (k == ZEPHYRUM || linea[k - I] == '\0')
            {
                si (numerus >= ARGUMENTA_MAXIMA)
                {
                    fprintf(stderr, "compilator: argumenta nimis "
                        "multa\n");
                    redde I;
                }
                argumenta[numerus++] = linea + k;
            }
        }
        si (numerus == ZEPHYRUM)
        {
            perge;
        }
        codex = -(s32)I;
        si (   compilator != NIHIL
            && compilator_cacheabile(argumenta, numerus))
        {
            CompilatorResultus resultus;

            resultus = compilator_compilare(compilator, argumenta,
                numerus);
            codex = resultus.codex;
            si (codex >= ZEPHYRUM)
            {
                (vacuum)fwrite(resultus.effusio.datum, I,
                    (memoriae_index)resultus.effusio.mensura, stdout);
                (vacuum)fwrite(resultus.erratum.datum, I,
                    (memoriae_index)resultus.erratum.mensura, stderr);
            }
        }
        si (codex < ZEPHYRUM)
        {
            codex = _clang_filius(argumenta, numerus, piscina);
        }
        si (codex != ZEPHYRUM)
        {
            exitus = I;
        }
    }
    redde exitus;
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
    si (argc == III && strcmp(argv[I], "--plures") == ZEPHYRUM)
    {
        s32 exitus;

        si (piscina == NIHIL)
        {
            redde I;
        }
        compilator_memoriam_incipere(compilator);
        exitus = _plures(argv[II], compilator, piscina);
        piscina_destruere(piscina);
        redde exitus;
    }
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
