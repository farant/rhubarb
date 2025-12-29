/* probatio_flatura_benchmark.c - Benchmark pro flatura compressio
 *
 * Generat datum probationis et metitur MB/s.
 *
 * USUS:
 *   ./bin/probatio_flatura_benchmark [megabytes]
 *   Default: 100 MB
 */

#include "latina.h"
#include "piscina.h"
#include "flatura.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* ========================================================================
 * DATUM GENERATIO
 * ======================================================================== */

/* Simple pseudo-random generator (LCG) */
interior i32
_prng_proximus(i32* semen)
{
    *semen = (*semen * 1103515245 + 12345) & 0x7FFFFFFF;
    redde *semen;
}

/* Generare datum simile texto reali (variabilis patterns) */
interior vacuum
generare_datum_textuale(i8* buffer, i32 mensura)
{
    /* Multa verba variabilia pro simulatione textu reali */
    constans character* verba[] = {
        "the ", "and ", "of ", "to ", "a ", "in ", "that ", "is ", "was ", "he ",
        "for ", "it ", "with ", "as ", "his ", "on ", "be ", "at ", "by ", "I ",
        "this ", "had ", "not ", "are ", "but ", "from ", "or ", "have ", "an ", "they ",
        "which ", "one ", "you ", "were ", "her ", "all ", "she ", "there ", "would ", "their ",
        "we ", "him ", "been ", "has ", "when ", "who ", "will ", "more ", "no ", "if ",
        "out ", "so ", "said ", "what ", "up ", "its ", "about ", "into ", "than ", "them ",
        "can ", "only ", "other ", "new ", "some ", "could ", "time ", "very ", "these ", "may ",
        "just ", "any ", "know ", "see ", "now ", "way ", "made ", "after ", "most ", "also ",
        "did ", "many ", "before ", "must ", "through ", "back ", "years ", "where ", "much ", "your ",
        "well ", "down ", "should ", "because ", "each ", "people ", "Mr ", "how ", "long ", "little ",
        "world ", "like ", "still ", "own ", "never ", "such ", "here ", "last ", "between ", "under "
    };
    constans character* punctuatio[] = { ". ", ", ", "; ", ": ", "? ", "! ", ".\n", ",\n" };

    i32 num_verba;
    i32 num_punct;
    i32 positus;
    i32 semen;
    i32 verbum_count;

    num_verba = (i32)(sizeof(verba) / sizeof(verba[0]));
    num_punct = (i32)(sizeof(punctuatio) / sizeof(punctuatio[0]));
    positus = 0;
    semen = 12345;
    verbum_count = 0;

    dum (positus < mensura)
    {
        constans character* s;
        i32 len;

        /* Selectio verbum vel punctuatio */
        si (verbum_count > 0 && (_prng_proximus(&semen) % XII) == 0)
        {
            /* Punctuatio */
            s = punctuatio[_prng_proximus(&semen) % num_punct];
            verbum_count = 0;
        }
        alioquin
        {
            /* Verbum */
            s = verba[_prng_proximus(&semen) % num_verba];
            verbum_count++;
        }

        len = (i32)strlen(s);
        si (positus + len > mensura)
        {
            len = mensura - positus;
        }

        memcpy(buffer + positus, s, (size_t)len);
        positus += len;
    }
}


/* ========================================================================
 * BENCHMARK
 * ======================================================================== */

interior vacuum
currere_benchmark(i32 megabytes, FlaturaCompressioNivellus nivellus, constans character* titulus)
{
    Piscina*       piscina;
    i8*            datum;
    i32            mensura;
    FlaturaFructus fructus;
    clock_t        initium;
    clock_t        finis;
    f64            secunda;
    f64            mb_per_sec;
    f64            ratio;

    mensura = megabytes * M * M;

    /* Allocare piscina magna */
    piscina = piscina_generare_dynamicum("benchmark", (memoriae_index)(mensura * III));

    /* Generare datum */
    datum = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    generare_datum_textuale(datum, mensura);

    /* Benchmark */
    initium = clock();

    fructus = flatura_gzip_deflare(datum, mensura, nivellus, piscina);

    finis = clock();

    si (fructus.status != FLATURA_STATUS_OK)
    {
        printf("  %s: FALLITA (status=%d)\n", titulus, fructus.status);
        piscina_destruere(piscina);
        redde;
    }

    secunda = (f64)(finis - initium) / (f64)CLOCKS_PER_SEC;
    mb_per_sec = (f64)megabytes / secunda;
    ratio = (f64)fructus.mensura / (f64)mensura * 100.0;

    printf("  %-12s %3d MB -> %7.2f MB (%5.1f%%)  %6.2fs  %6.1f MB/s\n",
           titulus,
           megabytes,
           (f64)fructus.mensura / (f64)(M * M),
           ratio,
           secunda,
           mb_per_sec);

    piscina_destruere(piscina);
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(integer argc, character** argv)
{
    i32 megabytes;

    megabytes = C;  /* Default 100 MB */

    si (argc > I)
    {
        megabytes = (i32)atoi(argv[I]);
        si (megabytes < (i32)I)
        {
            megabytes = I;
        }
        si (megabytes > M)
        {
            megabytes = M;  /* Max 1 GB */
        }
    }

    printf("\n");
    printf("Flatura Benchmark\n");
    printf("=================\n");
    printf("Datum: %d MB (textus simulatus)\n\n", megabytes);

    printf("  %-12s %6s    %10s %8s  %7s  %10s\n",
           "Nivellus", "Input", "Output", "Ratio", "Tempus", "Throughput");
    printf("  %-12s %6s    %10s %8s  %7s  %10s\n",
           "--------", "-----", "------", "-----", "------", "----------");

    currere_benchmark(megabytes, FLATURA_COMPRESSIO_NULLA,     "NULLA");
    currere_benchmark(megabytes, FLATURA_COMPRESSIO_RAPIDA,    "RAPIDA");
    currere_benchmark(megabytes, FLATURA_COMPRESSIO_ORDINARIA, "ORDINARIA");
    currere_benchmark(megabytes, FLATURA_COMPRESSIO_OPTIMA,    "OPTIMA");

    printf("\n");

    redde 0;
}
