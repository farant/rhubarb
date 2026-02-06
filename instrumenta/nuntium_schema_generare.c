/* nuntium_schema_generare - Instrumentum CLI pro generatione codicis C
 *
 * Usus: nuntium_schema_generare <schema.nuntium> <output.h> <output.c>
 *
 * Legit schema nuntii, generat .h et .c fontes.
 */

#include "latina.h"
#include "nuntium_schema.h"
#include "filum.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>

s32 principale(s32 argc, character** argv)
{
    Piscina*            piscina;
    chorda              fons;
    NuntiumSchemaNodus* radix;
    chorda              caput;
    chorda              corpus;

    si (argc != IV)
    {
        fprintf(stderr,
            "Usus: %s <schema.nuntium> <output.h> <output.c>\n",
            argv[ZEPHYRUM]);
        redde I;
    }

    piscina = piscina_generare_dynamicum("nuntium_gen", MMMMXCVI);
    si (!piscina)
    {
        fprintf(stderr, "Error: non potuit creare piscinam\n");
        redde I;
    }

    /* Legere schema filum */
    fons = filum_legere_totum(argv[I], piscina);
    si (fons.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "Error: non potuit legere filum '%s'\n", argv[I]);
        piscina_destruere(piscina);
        redde I;
    }

    /* Parsare schema */
    radix = nuntium_schema_legere(piscina, (constans character*)fons.datum,
        (s32)fons.mensura);
    si (!radix)
    {
        fprintf(stderr, "Error: schema parsatio fracta pro '%s'\n", argv[I]);
        piscina_destruere(piscina);
        redde I;
    }

    /* Extrahere nomen capitis ex via output */
    {
        constans character* caput_titulis;
        constans character* p;

        /* Invenire ultimum '/' in via */
        caput_titulis = argv[II];
        per (p = argv[II]; *p; p++)
        {
            si (*p == '/')
                caput_titulis = p + I;
        }

        /* Generare caput */
        caput = nuntium_schema_generare_caput(piscina, radix, caput_titulis);
        si (caput.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "Error: generatio capitis fracta\n");
            piscina_destruere(piscina);
            redde I;
        }

        /* Generare corpus */
        corpus = nuntium_schema_generare_corpus(piscina, radix, caput_titulis);
        si (corpus.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "Error: generatio corporis fracta\n");
            piscina_destruere(piscina);
            redde I;
        }
    }

    /* Scribere fila output */
    si (!filum_scribere(argv[II], caput))
    {
        fprintf(stderr, "Error: non potuit scribere '%s'\n", argv[II]);
        piscina_destruere(piscina);
        redde I;
    }

    si (!filum_scribere(argv[III], corpus))
    {
        fprintf(stderr, "Error: non potuit scribere '%s'\n", argv[III]);
        piscina_destruere(piscina);
        redde I;
    }

    imprimere("Generatum: %s, %s\n", argv[II], argv[III]);

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
