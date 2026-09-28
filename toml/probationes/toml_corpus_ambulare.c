/* toml_corpus_ambulare.c - vide toml_corpus_ambulare.h */

#include "toml_corpus_ambulare.h"
#include "filum.h"
#include "sigillum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA (IV * MXXIV)

/* lineam proximam ex textu [*k, ...) in 'linea' (NUL); FALSUM in
 * fine */
interior b32
_linea (
         chorda  textus,
            i32* k,
      character* linea,
            i32  capacitas)
{
    i32 n = ZEPHYRUM;

    si (*k >= textus.mensura)
    {
        redde FALSUM;
    }
    dum (*k < textus.mensura && textus.datum[*k] != '\n')
    {
        si (n + I < capacitas)
        {
            linea[n++] = (character)textus.datum[*k];
        }
        (*k)++;
    }
    (*k)++;
    linea[n] = '\0';
    redde VERUM;
}

vacuum
toml_corpus_ambulare (
                Piscina* piscina,
                Piscina* opus,
    constans character* radix,
         TomlCorpusVisor visor,
                 vacuum* datum,
       TomlCorpusNumeri* numeri)
{
    character via[VIA_MAXIMA];
    character linea[VIA_MAXIMA];
       chorda index;
          i32 k;
          b32 toml_test_lectus;

    memset(numeri, ZEPHYRUM, magnitudo(*numeri));

    /* I. toml-test */
    sprintf(via, "%s/toml/probationes/fixa/toml-test/tests/"
        "files-toml-1.0.0", radix);
    index             = filum_legere_totum(via, piscina);
    toml_test_lectus  = index.mensura > ZEPHYRUM;
    k                 = ZEPHYRUM;
    dum (_linea(index, &k, linea, VIA_MAXIMA))
    {
        i32 l = (i32)strlen(linea);

        si (l < V || strcmp(linea + l - V, ".toml") != ZEPHYRUM)
        {
            perge;
        }
        sprintf(via, "%s/toml/probationes/fixa/toml-test/tests/%s",
            radix,
            linea);
        visor(datum, TOML_CORPUS_TOML_TEST, linea,
            filum_legere_totum(via, opus), opus);
        numeri->plagulae[TOML_CORPUS_TOML_TEST]++;
        piscina_vacare(opus);
    }

    /* II. domus */
    sprintf(via, "%s/build/toml_corpus.lst", radix);
    index = filum_legere_totum(via, piscina);
    numeri->indices_lecti = toml_test_lectus
        && index.mensura > ZEPHYRUM;
    k = ZEPHYRUM;
    dum (_linea(index, &k, linea, VIA_MAXIMA))
    {
        si (linea[ZEPHYRUM] == '\0')
        {
            perge;
        }
        sprintf(via, "%s/%s", radix, linea);
        visor(datum, TOML_CORPUS_DOMUS, linea,
            filum_legere_totum(via, opus), opus);
        numeri->plagulae[TOML_CORPUS_DOMUS]++;
        piscina_vacare(opus);
    }

    /* III. silvestria (vivae, sigillo iudicatae) */
    sprintf(via, "%s/toml/probationes/fixa/silvestria.manifestum",
        radix);
    index  = filum_legere_totum(via, piscina);
    k      = ZEPHYRUM;
    dum (_linea(index, &k, linea, VIA_MAXIMA))
    {
                  character* tab        = strchr(linea, '\t');
         constans character* domus_via  = getenv("HOME");
                     chorda  textus;
                   Sigillum  s;
                  character  hex[SIGILLUM_HEX_MENSURA];

        si (linea[ZEPHYRUM] == '#' || tab == NIHIL)
        {
            perge;
        }
        *tab = '\0';
        si (tab[I] == '~' && domus_via != NIHIL)
        {
            sprintf(via, "%s%s", domus_via, tab + II);
        }
        alioquin
        {
            sprintf(via, "%s", tab + I);
        }
        textus = filum_legere_totum(via, opus);
        s = sigillum_computare(textus.datum,
            (memoriae_index)textus.mensura);
        sigillum_hex(&s, hex);
        si (textus.datum == NIHIL || strcmp(hex, linea) != ZEPHYRUM)
        {
            numeri->omissae++;
            piscina_vacare(opus);
            perge;
        }
        visor(datum, TOML_CORPUS_SILVESTRIA, via, textus, opus);
        numeri->plagulae[TOML_CORPUS_SILVESTRIA]++;
        piscina_vacare(opus);
    }
}
