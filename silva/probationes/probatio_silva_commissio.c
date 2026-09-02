/* probatio_silva_commissio.c - Passus commissionis (Phase 4 Chunk C)
 *
 * Fabula integra spec-v2 par 12: parsare -> furca -> compactio/
 * transmutatio -> COMMITTERE (pater + normalizatio + collapsus cum
 * diario) -> superstites -> iniectio typi -> RECANONICARE in loco.
 *
 * Resolutores manu scripti (M1): semantica grammaticae in vocatore
 * vivit; ambulatio ipsa grammaticae-ignara formam ambigui ex registro
 * per nomen legit.
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
#include "silva_tabulae_imparilis.h"
#include "silva_glr.h"
#include "silva_commissio.h"
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

/* Resolutor sceleti: declaratio vincit si titulus typi-nominati
 * oraculo notus est; alioquin ignotum */
interior vacuum
_resolutor_sceleti (
        constans SilvaNodus* ambiguum,
     constans SilvaOraculum* oraculum,
                     vacuum* contextus,
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
        si (   nodus        == NIHIL
            || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATIO)
        {
            perge;
        }
        typus = silva_sceletum_declaratio_typus(nodus);
        si (   typus.genus != SILVA_VALOR_NODUS
            || typus.datum.nodus->genus
                != (s32)SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS)
        {
            perge;
        }
        {
            SilvaValor titulus =
                silva_sceletum_typus_nominatus_tok_titulus(
                    typus.datum.nodus);

            si (   titulus.genus == SILVA_VALOR_TOKEN
                && silva_oraculum_typum_novit(oraculum,
                       titulus.datum.token->valor))
            {
                responsum->victor        = (s32)i;
                responsum->discriminans  = titulus.datum.token;
                redde;
            }
        }
    }
}

/* Resolutor imparilis: idem, accessoribus imparilis */
interior vacuum
_resolutor_imparilis (
        constans SilvaNodus* ambiguum,
     constans SilvaOraculum* oraculum,
                     vacuum* contextus,
    SilvaResolutioResponsum* responsum)
{
    SilvaValor interps =
        silva_imparilis_ambiguus_interpretationes(ambiguum);
    i32 i;

    (vacuum)contextus;
    per (i = ZEPHYRUM; i < silva_valor_lista_numerus(interps); i++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(interps, i);
        SilvaNodus* nodus;
        SilvaValor  typus;

        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS) perge;
        nodus = elem->datum.nodus;
        si (   nodus        == NIHIL
            || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_DECLARATIO)
        {
            perge;
        }
        typus = silva_imparilis_declaratio_typus(nodus);
        si (   typus.genus != SILVA_VALOR_NODUS
            || typus.datum.nodus->genus
                != (s32)SILVA_IMPARILIS_GENUS_TYPUS_NOMINATUS)
        {
            perge;
        }
        {
            SilvaValor titulus =
                silva_imparilis_typus_nominatus_tok_titulus(
                    typus.datum.nodus);

            si (   titulus.genus == SILVA_VALOR_TOKEN
                && silva_oraculum_typum_novit(oraculum,
                       titulus.datum.token->valor))
            {
                responsum->victor        = (s32)i;
                responsum->discriminans  = titulus.datum.token;
                redde;
            }
        }
    }
}

/* Resolutor probationis: expressionem SEMPER praefert (pro probatione
 * versionis canonicae - semantica ficta) */
interior vacuum
_resolutor_praeferens_expressionem (
        constans SilvaNodus* ambiguum,
     constans SilvaOraculum* oraculum,
                     vacuum* contextus,
    SilvaResolutioResponsum* responsum)
{
    SilvaValor interps =
        silva_sceletum_ambiguus_interpretationes(ambiguum);
    i32 i;

    (vacuum)oraculum;
    (vacuum)contextus;
    per (i = ZEPHYRUM; i < silva_valor_lista_numerus(interps); i++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(interps, i);

        si (   elem != NIHIL && elem->genus == SILVA_VALOR_NODUS
            && elem->datum.nodus != NIHIL
            && elem->datum.nodus->genus
                == (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS)
        {
            responsum->victor = (s32)i;
            redde;
        }
    }
}

s32 principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;
    SilvaGLR* sceletum;
    SilvaGLR* imparilis;

    piscina = piscina_generare_dynamicum("probatio_silva_commissio",
        8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    sceletum = silva_glr_creare(piscina, &SILVA_SCELETUM_TABULA,
        silva_sceletum_construere, silva_sceletum_ambiguum_fabricare);
    imparilis = silva_glr_creare(piscina, &SILVA_IMPARILIS_TABULA,
        silva_imparilis_construere, silva_imparilis_ambiguum_fabricare);


    /* ========================================================
     * PROBARE: pater figitur (S27 solvitur)
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
         SilvaCommissio* commissio;
             SilvaNodus* declaratio;
             SilvaNodus* sententia;

        imprimere("\n--- Probans patrem fixum ---\n");

        fructus = _parsare(piscina, sceletum, "int x; y + 2;");
        CREDO_VERUM (fructus.successus);

        commissio = silva_committere(piscina, fructus.valor,
            &SILVA_SCELETUM_REGISTRUM, NIHIL, NIHIL, NIHIL);
        CREDO_NON_NIHIL (commissio);
        CREDO_AEQUALIS_I32 (xar_numerus(commissio->ambigui), ZEPHYRUM);

        declaratio  = _elementum(commissio->radix, ZEPHYRUM);
        sententia   = _elementum(commissio->radix, I);

        /* Radices listae radicalis: pater NIHIL (lista nodum non
         * habet) */
        CREDO_NIHIL (declaratio->pater);
        CREDO_NIHIL (sententia->pater);

        /* Interna: filii patrem accipiunt */
        {
            SilvaValor typus =
                silva_sceletum_declaratio_typus(declaratio);
            SilvaValor declarator =
                silva_sceletum_declaratio_declarator(declaratio);

            CREDO_AEQUALIS_PTR (typus.datum.nodus->pater, declaratio);
            CREDO_AEQUALIS_PTR (declarator.datum.nodus->pater,
                declaratio);
        }
        {
            SilvaValor expressio =
                silva_sceletum_sententia_expressionis_expressio(sententia);
            SilvaValor sinister =
                silva_sceletum_binarium_sinister(expressio.datum.nodus);

            CREDO_AEQUALIS_PTR (expressio.datum.nodus->pater,
                sententia);
            CREDO_AEQUALIS_PTR (sinister.datum.nodus->pater,
                expressio.datum.nodus);
        }
    }


    /* ========================================================
     * PROBARE: superstes AMBIGUUS - spina canonica
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
         SilvaCommissio* commissio;
             SilvaNodus* ambiguum;
             SilvaValor  interps;
             SilvaNodus* declaratio;
             SilvaNodus* sententia;

        imprimere("\n--- Probans spinam canonicam ---\n");

        fructus = _parsare(piscina, sceletum, "foo * bar;");
        CREDO_VERUM (fructus.successus);

        /* Sine oraculo: involucrum superstes */
        commissio = silva_committere(piscina, fructus.valor,
            &SILVA_SCELETUM_REGISTRUM, NIHIL, NIHIL, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(commissio->ambigui), I);
        CREDO_AEQUALIS_I32 (xar_numerus(commissio->resolutiones),
            ZEPHYRUM);

        ambiguum = _elementum(commissio->radix, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (ambiguum->genus,
            (s32)SILVA_SCELETUM_GENUS_AMBIGUUS);
        CREDO_NIHIL (ambiguum->pater);  /* radix listae */

        interps = silva_sceletum_ambiguus_interpretationes(ambiguum);
        declaratio = _elementum(interps, ZEPHYRUM);
        sententia = _elementum(interps, I);
        CREDO_AEQUALIS_S32 (declaratio->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO);
        CREDO_AEQUALIS_S32 (sententia->genus,
            (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS);

        /* Spina: canonica (0 = declaratio) pater habet, altera NIHIL */
        CREDO_AEQUALIS_PTR (declaratio->pater, ambiguum);
        CREDO_NIHIL (sententia->pater);

        /* Interna AMBARUM interpretationum texta (quaestiones in
         * alternativas descendunt) */
        CREDO_AEQUALIS_PTR (
            silva_sceletum_declaratio_typus(declaratio)
                .datum.nodus->pater, declaratio);
        {
            SilvaValor expressio =
                silva_sceletum_sententia_expressionis_expressio(sententia);

            CREDO_AEQUALIS_PTR (expressio.datum.nodus->pater,
                sententia);
        }
    }


    /* ========================================================
     * PROBARE: COLLAPSUS + DIARIUM (oraculum iam novit)
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
          SilvaOraculum* oraculum;
         SilvaCommissio* commissio;
             SilvaNodus* sedes_ante;
             SilvaNodus* sedes_post;

        imprimere("\n--- Probans collapsum cum diario ---\n");

        oraculum = silva_oraculum_creare(piscina);
        CREDO_VERUM (silva_oraculum_typum_addere_literis(oraculum,
            "foo"));

        fructus     = _parsare(piscina, sceletum, "foo * bar;");
        sedes_ante  = _elementum(fructus.valor, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (sedes_ante->genus,
            (s32)SILVA_SCELETUM_GENUS_AMBIGUUS);

        commissio = silva_committere(piscina, fructus.valor,
            &SILVA_SCELETUM_REGISTRUM, oraculum, _resolutor_sceleti,
            NIHIL);

        /* IDENTITAS: eadem allocatio nunc DECLARATIONEM monstrat */
        sedes_post = _elementum(commissio->radix, ZEPHYRUM);
        CREDO_AEQUALIS_PTR (sedes_ante, sedes_post);
        CREDO_AEQUALIS_S32 (sedes_post->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO);

        /* Involucrum abiit; diarium unum eventum fert */
        CREDO_AEQUALIS_I32 (xar_numerus(commissio->ambigui), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(commissio->resolutiones), I);
        {
            SilvaResolutioEventum* eventum = (SilvaResolutioEventum*)
                xar_obtinere(commissio->resolutiones, ZEPHYRUM);

            CREDO_AEQUALIS_S32 ((s32)eventum->genus,
                (s32)SILVA_RESOLUTIO_COLLAPSUS);
            CREDO_AEQUALIS_PTR (eventum->sedes, sedes_post);
            CREDO_AEQUALIS_S32 (eventum->victor, ZEPHYRUM);
            CREDO_AEQUALIS_S32 (eventum->genus_victoris,
                (s32)SILVA_SCELETUM_GENUS_DECLARATIO);
            CREDO_NON_NIHIL (eventum->discriminans);
            CREDO_CHORDA_AEQUALIS_LITERIS (
                eventum->discriminans->valor, "foo");
        }

        /* Arbor collapsata plene texta et usabilis */
        {
            SilvaValor typus =
                silva_sceletum_declaratio_typus(sedes_post);

            CREDO_AEQUALIS_S32 (typus.datum.nodus->genus,
                (s32)SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS);
            CREDO_AEQUALIS_PTR (typus.datum.nodus->pater, sedes_post);
        }
    }


    /* ========================================================
     * PROBARE: superstes -> iniectio -> RECANONICARE in loco
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
          SilvaOraculum* oraculum;
         SilvaCommissio* commissio;
             SilvaNodus* ambiguum;
             SilvaValor  interps;
             SilvaNodus* declaratio;
             SilvaNodus* sententia;

        imprimere("\n--- Probans recanonicationem ---\n");

        oraculum = silva_oraculum_creare(piscina);

        fructus = _parsare(piscina, sceletum, "foo * bar;");
        commissio = silva_committere(piscina, fructus.valor,
            &SILVA_SCELETUM_REGISTRUM, oraculum, _resolutor_sceleti,
            NIHIL);

        /* Oraculum vacuum: superstes */
        CREDO_AEQUALIS_I32 (xar_numerus(commissio->ambigui), I);
        ambiguum = _elementum(commissio->radix, ZEPHYRUM);
        interps = silva_sceletum_ambiguus_interpretationes(ambiguum);
        declaratio = _elementum(interps, ZEPHYRUM);
        sententia = _elementum(interps, I);

        /* Iniectio POST commissionem: resolutor verus victorem 0 =
         * canonicam iam datam invenit -> NULLA versio (iter vacuum) */
        CREDO_VERUM (silva_oraculum_typum_addere_literis(oraculum,
            "foo"));
        CREDO_AEQUALIS_I32 (silva_recanonicare(commissio, oraculum,
            _resolutor_sceleti, NIHIL), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (
            silva_sceletum_ambiguus_canonica(ambiguum).datum.index,
            ZEPHYRUM);

        /* Resolutor fictus expressionem praefert -> versio 0 -> 1:
         * canonica in loco vertitur, spina retexta AMBOBUS modis */
        CREDO_AEQUALIS_I32 (silva_recanonicare(commissio, oraculum,
            _resolutor_praeferens_expressionem, NIHIL), I);
        CREDO_AEQUALIS_S32 (
            silva_sceletum_ambiguus_canonica(ambiguum).datum.index, I);
        CREDO_AEQUALIS_PTR (sententia->pater, ambiguum);
        CREDO_NIHIL (declaratio->pater);

        /* Involucrum manet (immutabilitas post commissionem praeter
         * hanc op sanctionatam); iteratio eadem -> nulla versio */
        CREDO_AEQUALIS_S32 (ambiguum->genus,
            (s32)SILVA_SCELETUM_GENUS_AMBIGUUS);
        CREDO_AEQUALIS_I32 (silva_recanonicare(commissio, oraculum,
            _resolutor_praeferens_expressionem, NIHIL), ZEPHYRUM);

        /* Diarium: eventum RECANONICATA adscriptum */
        CREDO_AEQUALIS_I32 (xar_numerus(commissio->resolutiones), I);
        {
            SilvaResolutioEventum* eventum = (SilvaResolutioEventum*)
                xar_obtinere(commissio->resolutiones, ZEPHYRUM);

            CREDO_AEQUALIS_S32 ((s32)eventum->genus,
                (s32)SILVA_RESOLUTIO_RECANONICATA);
            CREDO_AEQUALIS_S32 (eventum->victor, I);
        }
    }


    /* ========================================================
     * PROBARE: normalizatio prospectus divergentis (A½ debitum)
     * ======================================================== */

    {
                   Xar* lexemata;
            SilvaToken* tok_a;
            SilvaToken* tok_b;
            SilvaValor  basis;
            SilvaValor  ramus_a;
            SilvaValor  ramus_b;
            SilvaNodus* nodus;
        SilvaCommissio* commissio;

        imprimere("\n--- Probans normalizationem ---\n");

        lexemata  = silva_lexare(piscina, "a b", III, ZEPHYRUM);
        tok_a     = *(SilvaToken**)xar_obtinere(lexemata, ZEPHYRUM);
        tok_b     = *(SilvaToken**)xar_obtinere(lexemata, I);

        /* Divergentia manu: basis {xar,1}; ramus_a scribit ultra;
         * ramus_b ex basi copiam facit */
        basis = silva_valor_lista_nova(piscina);
        basis = silva_valor_lista_appendere(piscina, basis,
            silva_valor_token(tok_a));
        ramus_a = silva_valor_lista_appendere(piscina, basis,
            silva_valor_token(tok_b));
        ramus_b = silva_valor_lista_appendere(piscina, basis,
            silva_valor_token(tok_b));
        (vacuum)ramus_a;
        (vacuum)ramus_b;

        /* basis: mensura 1, repositorium 2 - divergens */
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(basis), I);
        CREDO_INAEQUALITAS_I32 (
            (i32)xar_numerus(basis.datum.lista.xar), I);

        /* Nodus error (locus tokens: lista-token) prospectum
         * divergentem fert; commissio eum normalizat */
        nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_ERROR, I);
        CREDO_VERUM (silva_nodus_ponere(nodus, ZEPHYRUM, basis,
            SILVA_LOCUS_LISTA_TOKEN));

        commissio = silva_committere(piscina, silva_valor_nodus(nodus),
            &SILVA_SCELETUM_REGISTRUM, NIHIL, NIHIL, NIHIL);
        CREDO_NON_NIHIL (commissio);

        /* Post: mensura exacta - xar_numerus tuto legitur */
        CREDO_AEQUALIS_I32 (
            silva_valor_lista_numerus(nodus->loci[ZEPHYRUM]), I);
        CREDO_AEQUALIS_I32 (
            (i32)xar_numerus(nodus->loci[ZEPHYRUM].datum.lista.xar), I);
    }


    /* ========================================================
     * PROBARE: compositio imparilis - transmutatio deinde
     * collapsus in EADEM allocatione
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
          SilvaOraculum* oraculum;
         SilvaCommissio* commissio;
             SilvaNodus* sedes;

        imprimere("\n--- Probans transmutatum collapsum (imparilis) ---\n");

        oraculum = silva_oraculum_creare(piscina);
        CREDO_VERUM (silva_oraculum_typum_addere_literis(oraculum,
            "foo"));

        fructus = _parsare(piscina, imparilis, "foo * bar;");
        CREDO_AEQUALIS_I32 (fructus.transmutationes, I);
        sedes = _elementum(fructus.valor, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (sedes->genus,
            (s32)SILVA_IMPARILIS_GENUS_AMBIGUUS);

        commissio = silva_committere(piscina, fructus.valor,
            &SILVA_IMPARILIS_REGISTRUM, oraculum, _resolutor_imparilis,
            NIHIL);

        /* Allocatio bis religata: nodus -> ambiguum (transmutatio in
         * exhaustione) -> declaratio (collapsus in commissione) -
         * identitas per totam fabulam una */
        CREDO_AEQUALIS_PTR (_elementum(commissio->radix, ZEPHYRUM),
            sedes);
        CREDO_AEQUALIS_S32 (sedes->genus,
            (s32)SILVA_IMPARILIS_GENUS_DECLARATIO);
        CREDO_AEQUALIS_I32 (xar_numerus(commissio->ambigui), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(commissio->resolutiones), I);
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
