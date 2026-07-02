/* probatio_silva_parsare.c - Gubernator: fistula tota + recuperatio
 * (Phase 4 Chunk D)
 *
 * silva_parsare = praeprocessor -> circuitus secans -> GLR ->
 * commissio. OMNIS input arborem completam reddit (vectis
 * tree-sitter): segmenta fracta nodi ERROR fiunt, parsura pergit.
 * Corpus syntaxis v1 (125 lineae harvestae) per recuperationem
 * funditur - vectis robustatis, non correctitudinis (consumptor verus
 * = grammatica C89, post M1).
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
#include "silva_expandere.h"
#include "silva_commissio.h"
#include "silva_parsare.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA 1024
#define LINEA_MAXIMA 4096

hic_manens constans SilvaGrammatica GRAMMATICA_SCELETI = {
    &SILVA_SCELETUM_TABULA,
    &SILVA_SCELETUM_REGISTRUM,
    silva_sceletum_construere,
    silva_sceletum_ambiguum_fabricare
};

interior SilvaParsura*
_parsare (Piscina* piscina, constans character* fons)
{
    redde silva_parsare(piscina, "probatio.c", fons,
        (i32)strlen(fons), &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
}

interior SilvaNodus*
_elementum (SilvaValor lista, i32 index)
{
    SilvaValor* elem;

    elem = silva_valor_lista_obtinere(lista, index);
    si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde elem->datum.nodus;
}

/* Resolutor sceleti (ut in probatio_silva_commissio) */
interior vacuum
_resolutor_sceleti (
    constans SilvaNodus*     ambiguum,
    constans SilvaOraculum*  oraculum,
    vacuum*                  contextus,
    SilvaResolutioResponsum* responsum)
{
    SilvaValor interps =
        silva_sceletum_ambiguus_interpretationes(ambiguum);
    i32 i;

    (vacuum)contextus;
    per (i = ZEPHYRUM; i < silva_valor_lista_numerus(interps); i++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(interps, i);
        SilvaNodus* nodus;
        SilvaValor  typus;

        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS) perge;
        nodus = elem->datum.nodus;
        si (nodus == NIHIL
            || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATIO)
        {
            perge;
        }
        typus = silva_sceletum_declaratio_typus(nodus);
        si (typus.genus != SILVA_VALOR_NODUS
            || typus.datum.nodus->genus
                != (s32)SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS)
        {
            perge;
        }
        {
            SilvaValor titulus =
                silva_sceletum_typus_nominatus_tok_titulus(
                    typus.datum.nodus);

            si (titulus.genus == SILVA_VALOR_TOKEN
                && silva_oraculum_typum_novit(oraculum,
                       titulus.datum.token->valor))
            {
                responsum->victor = (s32)i;
                responsum->discriminans = titulus.datum.token;
                redde;
            }
        }
    }
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silva_parsare",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: fistula tota - macro per praeprocessorem
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus* declaratio;
        SilvaValor  typus;

        imprimere("\n--- Probans fistulam totam ---\n");

        parsura = _parsare(piscina, "#define T int\nT x; y + 2;");
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (parsura->numerus_segmentorum, II);
        CREDO_NON_NIHIL (parsura->expansio);
        CREDO_NON_NIHIL (parsura->lexemata);
        CREDO_NON_NIHIL (parsura->lexema_finis);

        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            parsura->commissio->radix), II);

        /* Macro T -> int: typus PRIMITIVUS, non nominatus - expansio
         * vere cucurrit */
        declaratio = _elementum(parsura->commissio->radix, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (declaratio->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO);
        typus = silva_sceletum_declaratio_typus(declaratio);
        CREDO_AEQUALIS_S32 (typus.datum.nodus->genus,
            (s32)SILVA_SCELETUM_GENUS_TYPUS_PRIMITIVUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_sceletum_typus_primitivus_tok_verbum(
                typus.datum.nodus).datum.token->valor, "int");

        /* Pater fixus per gubernatorem (commissio inclusa) */
        CREDO_AEQUALIS_PTR (typus.datum.nodus->pater, declaratio);
    }


    /* ========================================================
     * PROBARE: recuperatio media - vicini intacti
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus* erroris;

        imprimere("\n--- Probans recuperationem mediam ---\n");

        parsura = _parsare(piscina, "int x; @ + ); y + 2;");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, I);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            parsura->commissio->radix), III);

        CREDO_AEQUALIS_S32 (
            _elementum(parsura->commissio->radix, ZEPHYRUM)->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO);
        erroris = _elementum(parsura->commissio->radix, I);
        CREDO_AEQUALIS_S32 (erroris->genus,
            (s32)SILVA_SCELETUM_GENUS_ERROR);
        CREDO_AEQUALIS_S32 (
            _elementum(parsura->commissio->radix, II)->genus,
            (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS);

        /* Lexemata praetermissa in nodo ERROR servata (nihil umquam
         * tacite abicitur) */
        {
            SilvaValor lexemata_erroris =
                silva_sceletum_error_tokens(erroris);

            CREDO_AEQUALIS_S32 ((s32)lexemata_erroris.genus,
                (s32)SILVA_VALOR_LISTA);
            CREDO_MAIOR_I32 (silva_valor_lista_numerus(
                lexemata_erroris), ZEPHYRUM);
        }
    }


    /* ========================================================
     * PROBARE: cauda sine terminatore; purgamentum totale;
     * fons vacuus
     * ======================================================== */

    {
        SilvaParsura* parsura;

        imprimere("\n--- Probans oras ---\n");

        /* Cauda sine ; -> segmentum ultimum ERROR */
        parsura = _parsare(piscina, "int x; +");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, I);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            parsura->commissio->radix), II);

        /* Purgamentum totale -> arbor unius nodi ERROR */
        parsura = _parsare(piscina, "@ $ ^");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, I);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            parsura->commissio->radix), I);

        /* Fons vacuus -> lista vacua, successus */
        parsura = _parsare(piscina, "");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_segmentorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            parsura->commissio->radix), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: oraculum per gubernatorem - collapsus in fistula
     * ======================================================== */

    {
        SilvaOraculum* oraculum;
        SilvaParsura*  parsura;

        imprimere("\n--- Probans oraculum per gubernatorem ---\n");

        oraculum = silva_oraculum_creare(piscina);
        CREDO_VERUM (silva_oraculum_typum_addere_literis(oraculum,
            "foo"));

        parsura = silva_parsare(piscina, "probatio.c", "foo * bar;",
            X, &GRAMMATICA_SCELETI, oraculum, _resolutor_sceleti,
            NIHIL);
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura->commissio->resolutiones), I);
        CREDO_AEQUALIS_S32 (
            _elementum(parsura->commissio->radix, ZEPHYRUM)->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO);

        /* Sine oraculo: superstes */
        parsura = _parsare(piscina, "foo * bar;");
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            I);
        CREDO_MAIOR_I32 (parsura->fusiones, ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: ingressus lexematum (fluxus praelexatus)
     * ======================================================== */

    {
        Xar* lexemata;
        SilvaParsura* parsura;

        imprimere("\n--- Probans ingressum lexematum ---\n");

        lexemata = silva_lexare(piscina, "int q;", VI, ZEPHYRUM);
        parsura = silva_lexemata_parsare(piscina, lexemata,
            &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
        CREDO_VERUM (parsura->successus);
        CREDO_NIHIL (parsura->expansio);  /* fistula praetermissa */
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            parsura->commissio->radix), I);
    }


    /* ========================================================
     * PROBARE: corpus syntaxis v1 per recuperationem (vectis
     * robustatis: omnis linea arborem reddit, numquam fragor)
     * ======================================================== */

    {
        constans character* radix_env;
        character via[VIA_MAXIMA];
        character linea[LINEA_MAXIMA];
        FILE* corpus;
        i32 lineae = ZEPHYRUM;
        i32 arbores = ZEPHYRUM;
        i32 cum_erroribus = ZEPHYRUM;

        imprimere("\n--- Probans corpus syntaxis v1 (recuperatio) ---\n");

        radix_env = getenv("RHUBARB_RADIX");
        si (radix_env == NIHIL)
        {
            radix_env = "..";
        }
        sprintf(via, "%s/silva/probationes/fixa/syntaxis_v1_corpus.txt",
            radix_env);
        corpus = fopen(via, "rb");
        CREDO_NON_NIHIL (corpus);

        si (corpus != NIHIL)
        {
            dum (fgets(linea, LINEA_MAXIMA, corpus) != NIHIL)
            {
                Piscina* piscina_lineae;
                SilvaParsura* parsura;
                memoriae_index m = strlen(linea);

                si (m == ZEPHYRUM || linea[ZEPHYRUM] == '#'
                    || linea[ZEPHYRUM] == '\n')
                {
                    perge;
                }
                lineae++;

                piscina_lineae = piscina_generare_dynamicum(
                    "corpus_lineae", 4194304);
                si (piscina_lineae == NIHIL)
                {
                    perge;
                }
                parsura = silva_parsare(piscina_lineae, "corpus.c",
                    linea, (i32)m, &GRAMMATICA_SCELETI, NIHIL, NIHIL,
                    NIHIL);
                si (parsura != NIHIL && parsura->successus)
                {
                    arbores++;
                    si (parsura->numerus_errorum > ZEPHYRUM)
                    {
                        cum_erroribus++;
                    }
                }
                piscina_destruere(piscina_lineae);
            }
            fclose(corpus);
        }

        imprimere("  lineae: %d, arbores: %d, cum erroribus: %d\n",
            (int)lineae, (int)arbores, (int)cum_erroribus);
        CREDO_AEQUALIS_I32 (lineae, CXXV);
        CREDO_AEQUALIS_I32 (arbores, lineae);  /* OMNIS input arborem */
        CREDO_MAIOR_I32 (cum_erroribus, ZEPHYRUM);  /* honestas: C89
            verum sceleto maiore parte ERROR est */
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
