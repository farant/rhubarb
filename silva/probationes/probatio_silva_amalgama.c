/* probatio_silva_amalgama.c - Artefacta amalgamatis (Phase 6 Chunk C)
 *
 * Probationes gradus compilationis in amalgamare.sh vivunt
 * (standalone plena severitate, hospes = pollutio S43 +
 * aequivalentia, intersectio nm = 0). HAEC probatio proprietates
 * ARTEFACTORUM COMMISSORUM examinat, in suite ordinaria:
 *
 *   - silva.h: NULLUS identificator est nomen macro latina.h -
 *     probatio vera pollutionis: definitiones latina.h ipsius
 *     lectae (# define NOMEN), identificatores silva.h lexati
 *     lexatore silvae, intersectio = 0. Commenta (trivia) exempta -
 *     prosa Latina ibi licet.
 *   - silva.c: assertio derivae XAR praesens; nullum #include
 *     proiecti superstes; static praesens in scala.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA 1024

interior i8*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_out)
{
    FILE* pl;
    i8* buffer;
    signatus longus mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);

    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (   mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    buffer[mensura]  = '\0';
    *mensura_out     = (i32)mensura;
    redde buffer;
}

s32 principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
    constans character* radix;
             character  via[VIA_MAXIMA];
                    i8* latina_textus;
                    i8* caput_textus;
                    i8* corpus_textus;
                   i32  m_latinae;
                   i32  m_capitis;
                   i32  m_corporis;

    piscina = piscina_generare_dynamicum("probatio_silva_amalgama",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = "..";
    }

    sprintf(via, "%s/include/latina.h", radix);
    latina_textus = _plagulam_legere(piscina, via, &m_latinae);
    sprintf(via, "%s/silva/amalgama/silva.h", radix);
    caput_textus = _plagulam_legere(piscina, via, &m_capitis);
    sprintf(via, "%s/silva/amalgama/silva.c", radix);
    corpus_textus = _plagulam_legere(piscina, via, &m_corporis);

    imprimere("\n--- Probans artefacta amalgamatis ---\n");
    CREDO_NON_NIHIL (latina_textus);
    CREDO_NON_NIHIL (caput_textus);
    CREDO_NON_NIHIL (corpus_textus);

    si (   latina_textus != NIHIL && caput_textus != NIHIL
        && corpus_textus != NIHIL)
    {


        /* ========================================================
         * PROBARE: pollutio silva.h - identificatores vs
         * definitiones latina.h (probatio VERA, non index manualis)
         * ======================================================== */

        TabulaDispersa* definitiones =
            tabula_dispersa_creare_chorda(piscina, DXII);
        i32 numerus_definitionum  = ZEPHYRUM;
        i32 violationes           = ZEPHYRUM;

        /* definitiones latina.h colligere: lineae "# define NOMEN" */
        {
            Xar* lexemata = silva_lexare(piscina,
                (constans character*)latina_textus, m_latinae,
                ZEPHYRUM);
            i32 n = xar_numerus(lexemata);
            i32 i;

            per (i = ZEPHYRUM; i + II < n; i++)
            {
                SilvaToken* t = *(SilvaToken**)xar_obtinere(
                    lexemata, i);
                SilvaToken* d = *(SilvaToken**)xar_obtinere(
                    lexemata, i + I);
                SilvaToken* titulus = *(SilvaToken**)xar_obtinere(
                    lexemata, i + II);

                si (   t->genus         == SILVA_LEX_CANCELLUM
                    && t->initium_lineae
                    && d->genus         == SILVA_LEX_IDENTIFICATOR
                    && d->valor.mensura == VI
                    && memcmp(d->valor.datum, "define", VI)
                        == ZEPHYRUM
                    && titulus->genus   == SILVA_LEX_IDENTIFICATOR)
                {
                    tabula_dispersa_inserere(definitiones,
                        titulus->valor, (vacuum*)(s64)I);
                    numerus_definitionum++;
                }
            }
        }
        imprimere("  definitiones latina.h: %d\n",
            (int)numerus_definitionum);
        CREDO_MAIOR_I32 ((i32)numerus_definitionum, CC);

        /* identificatores silva.h contra eas */
        {
            Xar* lexemata = silva_lexare(piscina,
                (constans character*)caput_textus, m_capitis,
                ZEPHYRUM);
            i32 n = xar_numerus(lexemata);
            i32 i;

            per (i = ZEPHYRUM; i < n; i++)
            {
                SilvaToken* t = *(SilvaToken**)xar_obtinere(
                    lexemata, i);

                si (   t->genus == SILVA_LEX_IDENTIFICATOR
                    && tabula_dispersa_continet(definitiones,
                           t->valor))
                {
                    si (violationes < V)
                    {
                        imprimere("  POLLUTIO: '%.*s' (linea %d)\n",
                            (int)t->valor.mensura,
                            (constans character*)t->valor.datum,
                            (int)t->linea);
                    }
                    violationes++;
                }
            }
        }
        CREDO_AEQUALIS_I32 ((i32)violationes, ZEPHYRUM);


        /* ========================================================
         * PROBARE: proprietates silva.c
         * ======================================================== */

        CREDO_VERUM (strstr((constans character*)corpus_textus,
            "silva_assertio_xar_segmentorum") != NIHIL);
        CREDO_VERUM (strstr((constans character*)corpus_textus,
            "#include \"") == NIHIL);
        CREDO_VERUM (strstr((constans character*)corpus_textus,
            "static ") != NIHIL);
        CREDO_MAIOR_I32 (m_corporis, 300000);
        CREDO_MAIOR_I32 (m_capitis, 10000);

        /* silva.h ipsum quoque sine #include proiecti */
        CREDO_VERUM (strstr((constans character*)caput_textus,
            "#include \"") == NIHIL);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
