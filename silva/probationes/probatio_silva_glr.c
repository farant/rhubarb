/* probatio_silva_glr.c - Motor GLR, via inambigua (Phase 4 Chunk A)
 *
 * Arbores quas probatio_silva_constructio MANU aedificavit nunc per
 * tabulas coctas MECHANICE surgunt: lexare -> silva_glr_parsare ->
 * accessores generati. Validatio tabularum, adaptator terminalium,
 * listae, praecedentia (+ vs *), parentheses, errores mundi, S27
 * (pater NIHIL ante passum post-acceptum).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_sceleti.h"
#include "silva_glr.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior SilvaGLRFructus
_parsare (
               Piscina* piscina,
              SilvaGLR* glr,
    constans character* fons)
{
    Xar* lexemata;

    lexemata = silva_lexare(piscina, fons, (i32)strlen(fons), ZEPHYRUM);
    redde silva_glr_parsare(glr, lexemata, piscina);
}

/* Nodus ex elemento listae (NIHIL si non nodus) */
interior SilvaNodus*
_elementum (
    SilvaValor lista,
           i32 index)
{
    SilvaValor* elem;

    elem = silva_valor_lista_obtinere(lista, index);
    si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde elem->datum.nodus;
}

s32 principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;
    SilvaGLR* glr;

    piscina = piscina_generare_dynamicum("probatio_silva_glr", 4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    glr = silva_glr_creare(piscina, &SILVA_SCELETUM_TABULA,
        silva_sceletum_construere, silva_sceletum_ambiguum_fabricare);


    /* ========================================================
     * PROBARE: validatio tabulae + adaptator terminalium
     * ======================================================== */

    {
        imprimere("\n--- Probans validationem et adaptatorem ---\n");

        CREDO_VERUM (silva_glr_tabulam_validare(piscina,
            &SILVA_SCELETUM_TABULA));
        CREDO_NON_NIHIL (glr);

        CREDO_AEQUALIS_S32 (silva_glr_terminale_ex_genere(
            &SILVA_SCELETUM_TABULA, SILVA_LEX_INT), II);
        CREDO_AEQUALIS_S32 (silva_glr_terminale_ex_genere(
            &SILVA_SCELETUM_TABULA, SILVA_LEX_SEMICOLON), V);
        CREDO_AEQUALIS_S32 (silva_glr_terminale_ex_genere(
            &SILVA_SCELETUM_TABULA, SILVA_LEX_EOF), VIII);
        CREDO_AEQUALIS_S32 (silva_glr_terminale_ex_genere(
            &SILVA_SCELETUM_TABULA, SILVA_LEX_OCTETUS_IGNOTUS),
            SILVA_GLR_TERMINALE_IGNOTUM);
    }


    /* ========================================================
     * PROBARE: fluxus vacuus (epsilon + EOF)
     * ======================================================== */

    {
        SilvaGLRFructus fructus;

        imprimere("\n--- Probans fluxum vacuum ---\n");

        fructus = _parsare(piscina, glr, "");
        CREDO_VERUM (fructus.successus);
        CREDO_AEQUALIS_I32 (fructus.numerus_acceptorum, I);
        CREDO_AEQUALIS_S32 ((s32)fructus.valor.genus,
            (s32)SILVA_VALOR_LISTA);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(fructus.valor),
            ZEPHYRUM);
        CREDO_NON_NIHIL (fructus.lexema_finis);
        CREDO_AEQUALIS_S32 ((s32)fructus.lexema_finis->genus,
            (s32)SILVA_LEX_EOF);
    }


    /* ========================================================
     * PROBARE: declaratio simplex "int x;"
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
             SilvaNodus* declaratio;
             SilvaValor  typus;
             SilvaValor  declarator;
             SilvaValor  terminator;

        imprimere("\n--- Probans declarationem ---\n");

        fructus = _parsare(piscina, glr, "int x;");
        CREDO_VERUM (fructus.successus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(fructus.valor),
            I);

        declaratio = _elementum(fructus.valor, ZEPHYRUM);
        CREDO_NON_NIHIL (declaratio);
        CREDO_AEQUALIS_S32 (declaratio->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO);

        typus = silva_sceletum_declaratio_typus(declaratio);
        CREDO_AEQUALIS_S32 ((s32)typus.genus, (s32)SILVA_VALOR_NODUS);
        CREDO_AEQUALIS_S32 (typus.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_TYPUS_PRIMITIVUS);
        {
            SilvaValor verbum =
                silva_sceletum_typus_primitivus_tok_verbum(
                typus.datum.nodus);

            CREDO_AEQUALIS_S32 ((s32)verbum.genus,
                (s32)SILVA_VALOR_TOKEN);
            CREDO_CHORDA_AEQUALIS_LITERIS (verbum.datum.token->valor,
                "int");
        }

        declarator = silva_sceletum_declaratio_declarator(declaratio);
        CREDO_AEQUALIS_S32 ((s32)declarator.genus,
            (s32)SILVA_VALOR_NODUS);
        CREDO_AEQUALIS_S32 (declarator.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATOR_TITULUS);
        {
            SilvaValor titulus =
                silva_sceletum_declarator_titulus_tok_titulus(
                    declarator.datum.nodus);

            CREDO_CHORDA_AEQUALIS_LITERIS (titulus.datum.token->valor,
                "x");
        }

        terminator =
            silva_sceletum_declaratio_tok_terminator(declaratio);
        CREDO_AEQUALIS_S32 ((s32)terminator.genus,
            (s32)SILVA_VALOR_TOKEN);
        CREDO_CHORDA_AEQUALIS_LITERIS (terminator.datum.token->valor,
            ";");

        /* S27: pater NIHIL ubique ante passum post-acceptum (Chunk C) */
        CREDO_NIHIL (declaratio->pater);
        CREDO_NIHIL (typus.datum.nodus->pater);
        CREDO_NIHIL (declarator.datum.nodus->pater);
    }


    /* ========================================================
     * PROBARE: declarator monstratoris "int *x;" (inambiguum -
     * typus est verbum INT, non identificator)
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
             SilvaNodus* declaratio;
             SilvaValor  declarator;
             SilvaValor  internum;

        imprimere("\n--- Probans monstratorem ---\n");

        fructus = _parsare(piscina, glr, "int *x;");
        CREDO_VERUM (fructus.successus);

        declaratio = _elementum(fructus.valor, ZEPHYRUM);
        CREDO_NON_NIHIL (declaratio);

        declarator = silva_sceletum_declaratio_declarator(declaratio);
        CREDO_AEQUALIS_S32 (declarator.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATOR_MONSTRATOR);
        {
            SilvaValor stella =
                silva_sceletum_declarator_monstrator_tok_stella(
                    declarator.datum.nodus);

            CREDO_CHORDA_AEQUALIS_LITERIS (stella.datum.token->valor,
                "*");
        }
        internum = silva_sceletum_declarator_monstrator_internum(
            declarator.datum.nodus);
        CREDO_AEQUALIS_S32 (internum.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATOR_TITULUS);
    }


    /* ========================================================
     * PROBARE: praecedentia "x + 1 * 2;" - structura per tabulas
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
             SilvaNodus* sententia;
             SilvaValor  expressio;
             SilvaValor  sinister;
             SilvaValor  dexter;

        imprimere("\n--- Probans praecedentiam ---\n");

        fructus = _parsare(piscina, glr, "x + 1 * 2;");
        CREDO_VERUM (fructus.successus);

        sententia = _elementum(fructus.valor, ZEPHYRUM);
        CREDO_NON_NIHIL (sententia);
        CREDO_AEQUALIS_S32 (sententia->genus,
            (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS);

        expressio = silva_sceletum_sententia_expressionis_expressio(
            sententia);
        CREDO_AEQUALIS_S32 (expressio.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_BINARIUM);
        {
            SilvaValor op = silva_sceletum_binarium_tok_operator(
                expressio.datum.nodus);

            CREDO_CHORDA_AEQUALIS_LITERIS (op.datum.token->valor, "+");
        }

        sinister =
            silva_sceletum_binarium_sinister(expressio.datum.nodus);
        CREDO_AEQUALIS_S32 (sinister.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_FOLIUM_IDENTIFICATOR);

        /* dexter = binarium(1 * 2): STAR arctius ligat quam PLUS */
        dexter = silva_sceletum_binarium_dexter(expressio.datum.nodus);
        CREDO_AEQUALIS_S32 (dexter.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_BINARIUM);
        {
            SilvaValor op = silva_sceletum_binarium_tok_operator(
                dexter.datum.nodus);
            SilvaValor unum = silva_sceletum_binarium_sinister(
                dexter.datum.nodus);
            SilvaValor duo = silva_sceletum_binarium_dexter(
                dexter.datum.nodus);

            CREDO_CHORDA_AEQUALIS_LITERIS (op.datum.token->valor, "*");
            CREDO_AEQUALIS_S32 (unum.datum.nodus->genus,
                (s32)SILVA_SCELETUM_GENUS_FOLIUM_INTEGER);
            CREDO_CHORDA_AEQUALIS_LITERIS (
                silva_sceletum_folium_integer_tok_valor(
                    unum.datum.nodus).datum.token->valor, "1");
            CREDO_AEQUALIS_S32 (duo.datum.nodus->genus,
                (s32)SILVA_SCELETUM_GENUS_FOLIUM_INTEGER);
            CREDO_CHORDA_AEQUALIS_LITERIS (
                silva_sceletum_folium_integer_tok_valor(
                    duo.datum.nodus).datum.token->valor, "2");
        }
    }


    /* ========================================================
     * PROBARE: parentheses "(x + 1) * 2;"
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
             SilvaNodus* sententia;
             SilvaValor  expressio;
             SilvaValor  sinister;
             SilvaValor  internum;

        imprimere("\n--- Probans parentheses ---\n");

        fructus = _parsare(piscina, glr, "(x + 1) * 2;");
        CREDO_VERUM (fructus.successus);

        sententia = _elementum(fructus.valor, ZEPHYRUM);
        expressio = silva_sceletum_sententia_expressionis_expressio(
            sententia);
        CREDO_AEQUALIS_S32 (expressio.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_BINARIUM);
        {
            SilvaValor op = silva_sceletum_binarium_tok_operator(
                expressio.datum.nodus);

            CREDO_CHORDA_AEQUALIS_LITERIS (op.datum.token->valor, "*");
        }

        sinister =
            silva_sceletum_binarium_sinister(expressio.datum.nodus);
        CREDO_AEQUALIS_S32 (sinister.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_PARENTHESIS);

        internum = silva_sceletum_parenthesis_internum(
            sinister.datum.nodus);
        CREDO_AEQUALIS_S32 (internum.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_BINARIUM);
        {
            SilvaValor op = silva_sceletum_binarium_tok_operator(
                internum.datum.nodus);

            CREDO_CHORDA_AEQUALIS_LITERIS (op.datum.token->valor, "+");
        }
    }


    /* ========================================================
     * PROBARE: sententiae plures - lista in grammatica
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
             SilvaNodus* primus;
             SilvaNodus* secundus;

        imprimere("\n--- Probans sententias plures ---\n");

        fructus = _parsare(piscina, glr, "int x; y + 2;");
        CREDO_VERUM (fructus.successus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(fructus.valor),
            II);

        primus = _elementum(fructus.valor, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (primus->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO);
        secundus = _elementum(fructus.valor, I);
        CREDO_AEQUALIS_S32 (secundus->genus,
            (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS);

        /* Statisticae vivunt */
        CREDO_MAIOR_AUT_AEQUALIS_I32 (glr->frons_maxima, I);
        CREDO_MAIOR_I32 (glr->reductiones_factae, ZEPHYRUM);
        CREDO_MAIOR_I32 (glr->nodi_creati, ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: errores mundi (positio + terminale)
     * ======================================================== */

    {
        SilvaGLRFructus fructus;

        imprimere("\n--- Probans errores ---\n");

        /* "int;" - declarator deest; fractum ad SEMICOLON (term 5) */
        fructus = _parsare(piscina, glr, "int;");
        CREDO_FALSUM (fructus.successus);
        CREDO_VERUM (fructus.est_error);
        CREDO_AEQUALIS_S32 (fructus.positio, I);
        CREDO_AEQUALIS_S32 (fructus.terminalis, V);

        /* "x +" - fluxus finitur medio; fractum ad EOF (term 8) */
        fructus = _parsare(piscina, glr, "x +");
        CREDO_FALSUM (fructus.successus);
        CREDO_VERUM (fructus.est_error);
        CREDO_AEQUALIS_S32 (fructus.positio, II);
        CREDO_AEQUALIS_S32 (fructus.terminalis, VIII);

        /* "@;" - octetus ignotus: genus sine terminale */
        fructus = _parsare(piscina, glr, "@;");
        CREDO_FALSUM (fructus.successus);
        CREDO_VERUM (fructus.est_error);
        CREDO_AEQUALIS_S32 (fructus.positio, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (fructus.terminalis,
            SILVA_GLR_TERMINALE_IGNOTUM);

        /* Post errores motor reentrans manet */
        fructus = _parsare(piscina, glr, "int x;");
        CREDO_VERUM (fructus.successus);
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
