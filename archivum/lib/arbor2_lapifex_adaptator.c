/* arbor2_lapifex_adaptator.c - Pons inter Arbor2 Lexer et Lapifex Parser
 *
 * Convertit Arbor2Lexema* flumen in LapifexSignum[] cum indicibus
 * terminalium ex grammatica STML.
 */

#include "arbor2_lapifex_adaptator.h"
#include <stdio.h>
#include <string.h> /* strlen, memcmp */

/* ================================================
 * Adiutores Interni
 * ================================================ */

/* Quaerere terminalem in grammatica per titulum (C string) */
hic_manens s32
terminalem_per_titulum_quaerere(
    LapifexGrammatica*   grammatica,
    constans character*  titulis)
{
    i32 i;
    i32 num;
    i32 longitudo;

    si (!grammatica || !titulis) redde -I;

    num = (i32)xar_numerus(grammatica->symbola);
    longitudo = (i32)strlen(titulis);

    per (i = ZEPHYRUM; i < num; i++)
    {
        LapifexSymbolum* sym =
            (LapifexSymbolum*)xar_obtinere(grammatica->symbola, i);

        si (sym && sym->est_terminale && sym->titulus)
        {
            si ((i32)sym->titulus->mensura == longitudo &&
                memcmp(sym->titulus->datum, titulis,
                    (memoriae_index)longitudo) == ZEPHYRUM)
            {
                redde sym->index;
            }
        }
    }
    redde -I;
}

/* ================================================
 * arbor2_lapifex_est_trivia
 * ================================================ */

b32
arbor2_lapifex_est_trivia(
    Arbor2LexemaGenus  genus)
{
    commutatio (genus)
    {
        casus ARBOR2_LEXEMA_SPATIA:
        casus ARBOR2_LEXEMA_TABULAE:
        casus ARBOR2_LEXEMA_CONTINUATIO:
        casus ARBOR2_LEXEMA_COMMENTUM_CLAUSUM:
        casus ARBOR2_LEXEMA_COMMENTUM_LINEA:
        casus ARBOR2_LEXEMA_NOVA_LINEA:
        casus ARBOR2_LEXEMA_ERROR:
        casus ARBOR2_LEXEMA_TYPEDEF_MACRO:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* ================================================
 * arbor2_lapifex_adaptator_creare
 * ================================================ */

Arbor2LapifexAdaptator*
arbor2_lapifex_adaptator_creare(
    Piscina*            piscina,
    LapifexGrammatica*  grammatica)
{
    Arbor2LapifexAdaptator* adapt;
    s32 i;

    si (!piscina || !grammatica) redde NIHIL;

    adapt = (Arbor2LapifexAdaptator*)piscina_allocare(
        piscina, (memoriae_index)magnitudo(Arbor2LapifexAdaptator));
    si (!adapt) redde NIHIL;

    adapt->piscina = piscina;
    adapt->grammatica = grammatica;

    /* Initializare omnes mappas ad -1 (non mappatum) */
    per (i = ZEPHYRUM; i < ARBOR2_LAPIFEX_MAX_GENERA; i++)
    {
        adapt->mappa[i] = -I;
    }

    /* Pro quoque genere non-trivia, quaerere terminalem in grammatica */
    per (i = ZEPHYRUM; i < ARBOR2_LAPIFEX_MAX_GENERA; i++)
    {
        constans character* genus_titulis;

        /* Saltare trivia */
        si (arbor2_lapifex_est_trivia((Arbor2LexemaGenus)i))
        {
            perge;
        }

        /* EOF tractatur separatim */
        si (i == (s32)ARBOR2_LEXEMA_EOF)
        {
            adapt->mappa[i] = terminalem_per_titulum_quaerere(
                grammatica, "EOF");
            perge;
        }

        /* Obtinere canonicum titulum generis */
        genus_titulis = arbor2_lexema_genus_nomen((Arbor2LexemaGenus)i);
        si (!genus_titulis) perge;

        /* Quaerere in grammatica */
        adapt->mappa[i] = terminalem_per_titulum_quaerere(
            grammatica, genus_titulis);
    }

    redde adapt;
}

/* ================================================
 * arbor2_lapifex_convertere
 * ================================================ */

Arbor2LapifexFructus
arbor2_lapifex_convertere(
    Arbor2LapifexAdaptator*  adaptator,
    Xar*                     lexemata)
{
    Arbor2LapifexFructus fructus;
    Xar*                 signa_xar;
    s32                  i;
    s32                  num_lex;
    s32                  numerus_ignoratorum;
    s32                  eof_term;

    fructus.signa = NIHIL;
    fructus.numerus = ZEPHYRUM;
    fructus.successus = FALSUM;
    fructus.numerus_ignoratorum = ZEPHYRUM;

    si (!adaptator || !lexemata) redde fructus;

    num_lex = (s32)xar_numerus(lexemata);
    numerus_ignoratorum = ZEPHYRUM;

    /* Creare xar pro signis */
    signa_xar = xar_creare(adaptator->piscina, (i32)magnitudo(LapifexSignum));
    si (!signa_xar) redde fructus;

    per (i = ZEPHYRUM; i < num_lex; i++)
    {
        Arbor2Lexema** lex_ptr =
            (Arbor2Lexema**)xar_obtinere(lexemata, (i32)i);
        Arbor2Lexema*  lex;
        s32            genus_val;
        s32            term_index;
        LapifexSignum* signum;

        si (!lex_ptr) perge;
        lex = *lex_ptr;
        si (!lex) perge;

        genus_val = (s32)lex->genus;

        /* Saltare EOF — addemus in fine */
        si (lex->genus == ARBOR2_LEXEMA_EOF)
        {
            perge;
        }

        /* Saltare trivia */
        si (arbor2_lapifex_est_trivia(lex->genus))
        {
            numerus_ignoratorum++;
            perge;
        }

        /* Obtinere index terminalis */
        si (genus_val < ZEPHYRUM || genus_val >= ARBOR2_LAPIFEX_MAX_GENERA)
        {
            imprimere("arbor2_lapifex: genus %d extra limites\n", genus_val);
            redde fructus;
        }

        term_index = adaptator->mappa[genus_val];

        si (term_index < ZEPHYRUM)
        {
            /* Non mappatum — error */
            imprimere("arbor2_lapifex: genus %s (%d) non mappatum in grammatica\n",
                arbor2_lexema_genus_nomen(lex->genus), genus_val);
            redde fructus;
        }

        /* Addere signum */
        signum = (LapifexSignum*)xar_addere(signa_xar);
        signum->terminalis = term_index;
        signum->valor = (s64)(longus)(vacuum*)lex;
    }

    /* Addere EOF signum in fine */
    eof_term = adaptator->mappa[(s32)ARBOR2_LEXEMA_EOF];
    {
        LapifexSignum* signum_eof = (LapifexSignum*)xar_addere(signa_xar);
        si (eof_term >= ZEPHYRUM)
        {
            signum_eof->terminalis = eof_term;
        }
        alioquin
        {
            /* EOF debet semper esse mappatum — sed -1 est conventio */
            signum_eof->terminalis = -I;
        }
        signum_eof->valor = 0;
    }

    /* Transcribere ex xar in tabulam continuam */
    {
        s32 num_signa = (s32)xar_numerus(signa_xar);
        LapifexSignum* tabula = (LapifexSignum*)piscina_allocare(
            adaptator->piscina,
            (memoriae_index)num_signa * (memoriae_index)magnitudo(LapifexSignum));
        s32 j;

        si (!tabula) redde fructus;

        per (j = ZEPHYRUM; j < num_signa; j++)
        {
            LapifexSignum* src = (LapifexSignum*)xar_obtinere(signa_xar, (i32)j);
            tabula[j] = *src;
        }

        fructus.signa = tabula;
        fructus.numerus = num_signa;
        fructus.successus = VERUM;
        fructus.numerus_ignoratorum = numerus_ignoratorum;
    }

    redde fructus;
}

/* ================================================
 * arbor2_lapifex_mappam_imprimere
 * ================================================ */

vacuum
arbor2_lapifex_mappam_imprimere(
    Arbor2LapifexAdaptator*  adaptator)
{
    s32 i;

    si (!adaptator)
    {
        imprimere("(adaptator NIHIL)\n");
        redde;
    }

    imprimere("=== Mappa Arbor2 -> Lapifex ===\n");
    per (i = ZEPHYRUM; i < ARBOR2_LAPIFEX_MAX_GENERA; i++)
    {
        constans character* genus_titulis = arbor2_lexema_genus_nomen(
            (Arbor2LexemaGenus)i);

        si (adaptator->mappa[i] >= ZEPHYRUM)
        {
            imprimere("  %s (%d) -> terminalis %d\n",
                genus_titulis, i, adaptator->mappa[i]);
        }
        alioquin si (!arbor2_lapifex_est_trivia((Arbor2LexemaGenus)i) &&
                     i != (s32)ARBOR2_LEXEMA_EOF)
        {
            imprimere("  %s (%d) -> [NON MAPPATUM]\n",
                genus_titulis, i);
        }
    }
    imprimere("================================\n");
}
