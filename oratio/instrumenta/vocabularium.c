/* vocabularium.c - Instrumentum tabulae vocabularii Latini (T7)
 *
 * Usus: vocabularium [-coquere] [-scribere] [-census]
 *   (nihil)    tabulam commissam oratio/vocabularium/la.bin recensere:
 *              numeri, sigillum, octeti
 *   -coquere   fontes oratio/vocabularium/la/ coquere -> oratio/build/
 *              vocabularium_la.bin; numeri, sigillum, ms
 *   -scribere  (cum -coquere) tabulam commissam quoque rescribere -
 *              AUDI causam: porta sigillum pinnat
 * Radix per RHUBARB_RADIX (cursor eam ponit). Exitus: 0 sanum | 1 vitium
 * | 2 usus/plagula absens
 */

#include "latina.h"
#include "oratio_vocabularium.h"
#include "chorda.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
            i8* memoria;

    si (f == NIHIL)
    {
        redde FALSUM;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    memoria = (i8*)piscina_allocare(piscina, (memoriae_index)longitudo
        + I);
    si (   memoria                                 == NIHIL
        || fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);
    exitus->datum    = memoria;
    exitus->mensura  = (i32)longitudo;
    redde VERUM;
}

interior b32
_plagulam_scribere (
    constans character* via,
                chorda  datum)
{
    FILE* f = fopen(via, "wb");
     b32  sanum;

    si (f == NIHIL)
    {
        redde FALSUM;
    }
    sanum = (b32)(fwrite(datum.datum, I, (size_t)datum.mensura, f)
        == (size_t)datum.mensura);
    fclose(f);
    redde sanum;
}

interior vacuum
_censum_imprimere (
    constans OratioVocabulariumCensus* c,
                               chorda  sigillum,
                                  i32  octeti)
{
    imprimere("  stirpes %d  flexiones %d  additamenta %d  unica %d  versio %d  fons %.*s\n",
        (integer)c->stirpes, (integer)c->flexiones,
        (integer)c->additamenta,
        (integer)c->unica, (integer)c->versio, (integer)c->fons.mensura,
        (constans character*)c->fons.datum);
    imprimere("  octeti %d  sigillum %.*s\n", (integer)octeti,
        (integer)sigillum.mensura, (constans character*)sigillum.datum);
}

integer
principale (
      integer   argc,
    character** argv)
{
                 Piscina* piscina;
      constans character* radix;
                     b32  coquere   = FALSUM;
                     b32  scribere  = FALSUM;
                 integer  i;
               character  via[1024];
OratioVocabulariumCensus  census;
OratioVocabulariumVitium  vitium;
                  chorda  tabula;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-coquere") == ZEPHYRUM)
        {
            coquere = VERUM;
        }
        alioquin si (strcmp(argv[i], "-scribere") == ZEPHYRUM)
        {
            scribere = VERUM;
        }
        alioquin si (strcmp(argv[i], "-census") != ZEPHYRUM)
        {
            fprintf(stderr,
                "usus: vocabularium [-coquere] [-scribere] [-census]\n");
            redde II;
        }
    }
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    piscina =
        piscina_generare_dynamicum("oratio_vocabularium_instrumentum",
        134217728);

    si (coquere)
    {
        OratioVocabulariumFontes  fontes;
              constans character* tituli[IV];
                          chorda* campi[IV];
                         clock_t  ante;
                         clock_t  post;
                             i32  k;

        tituli[ZEPHYRUM] = "DICTLINE.GEN"; campi[ZEPHYRUM] =
                                               &fontes.dictline;
        tituli[I]        = "INFLECTS.LAT"; campi[I]        =
                                               &fontes.inflects;
        tituli[II]       = "ADDONS.LAT";   campi[II]       =
                                               &fontes.addons;
        tituli[III]      = "UNIQUES.LAT";  campi[III]      =
                                               &fontes.uniques;
        per (k = ZEPHYRUM; k < (i32)IV; k++)
        {
            sprintf(via, "%s/oratio/vocabularium/la/%s", radix,
                tituli[k]);
            si (!_plagulam_legere(piscina, via, campi[k]))
            {
                fprintf(stderr, "vocabularium: fons absens: %s\n", via);
                redde II;
            }
        }
        ante = clock();
        si (!oratio_vocabularium_coquere(piscina, &fontes,
                "mk270/whitakers-words@1f2f0fb0", &tabula, &census,
                &vitium))
        {
            fprintf(stderr, "vocabularium: coctio sistit: %s:%d %s\n",
                vitium.plagula, (integer)vitium.linea, vitium.causa);
            redde I;
        }
        post = clock();
        imprimere("--- coctio fontium (oratio/vocabularium/la) ---\n");
        _censum_imprimere(&census, oratio_vocabularium_sigillum(piscina,
            tabula), tabula.mensura);
        imprimere("  ms %.1f\n", 1000.0 * (duplex)(post - ante)
            / (duplex)CLOCKS_PER_SEC);
        sprintf(via, "%s/oratio/build/vocabularium_la.bin", radix);
        si (!_plagulam_scribere(via, tabula))
        {
            fprintf(stderr, "vocabularium: scriptio fracta: %s\n", via);
            redde I;
        }
        imprimere("  scripta %s\n", via);
        si (scribere)
        {
            sprintf(via, "%s/oratio/vocabularium/la.bin", radix);
            si (!_plagulam_scribere(via, tabula))
            {
                fprintf(stderr, "vocabularium: scriptio fracta: %s\n",
                    via);
                redde I;
            }
            imprimere("--- TABULA COMMISSA RESCRIPTA (%s) - AUDI causam: porta sigillum pinnat ---\n",
                via);
        }
        redde ZEPHYRUM;
    }

    sprintf(via, "%s/oratio/vocabularium/la.bin", radix);
    si (!_plagulam_legere(piscina, via, &tabula))
    {
        fprintf(stderr,
            "vocabularium: tabula absens: %s (-coquere -scribere)\n",
            via);
        redde II;
    }
    {
        clock_t ante = clock();
        clock_t post;
            b32 sana = oratio_vocabularium_recensere(piscina, tabula,
                &census,
                &vitium);

        post = clock();
        imprimere("--- recensio tabulae commissae (%s) ---\n", via);
        si (!sana)
        {
            imprimere("  VITIUM @%d: %s\n", (integer)vitium.offset,
                vitium.causa);
            redde I;
        }
        _censum_imprimere(&census, oratio_vocabularium_sigillum(piscina,
            tabula), tabula.mensura);
        imprimere("  ms %.1f\n", 1000.0 * (duplex)(post - ante)
            / (duplex)CLOCKS_PER_SEC);
    }
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
