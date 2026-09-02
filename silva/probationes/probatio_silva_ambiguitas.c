/* probatio_silva_ambiguitas.c - Furca, compactio, transmutatio (Chunk B)
 *
 * Cellula furcae in statu 7 cocta (conflictus reduce/reduce in STAR)
 * demum ACCENDITUR: "foo * bar;" dupliciter legitur et in nodum
 * AMBIGUUM LOCALIZATUM colligitur.
 *
 * Sceletum: brachia profunditate PARI reconveniunt (2/2 undae) ->
 * compactio eiusdem-undae (fusiones), transmutationes == 0.
 * Imparilis: gradus unitatis "mandatum" brachium expressionis
 * profundius facit (2/3 undae) -> compactio DEERRAT, transmutatio
 * (religatio identitatis) accenditur: monstrator elementi listae iam
 * repositus AMBIGUUM genus MONSTRAT (probatio identitatis ipsa).
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

/* Quot interpretationes generis dati? (ordo brachiorum res interna
 * exhaustionis est - assertiones per copiam, non per positionem) */
interior i32
_interpretationes_generis (
    SilvaValor interpretationes,
           s32 genus)
{
    i32 numerus = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM;
         i < silva_valor_lista_numerus(interpretationes); i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(interpretationes, i);

        si (   v != NIHIL && v->genus == SILVA_VALOR_NODUS
            && v->datum.nodus != NIHIL
            && v->datum.nodus->genus == genus)
        {
            numerus++;
        }
    }
    redde numerus;
}

/* Interpretatio prima generis dati */
interior SilvaNodus*
_interpretatio_generis (
    SilvaValor interpretationes,
           s32 genus)
{
    i32 i;

    per (i = ZEPHYRUM;
         i < silva_valor_lista_numerus(interpretationes); i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(interpretationes, i);

        si (   v != NIHIL && v->genus == SILVA_VALOR_NODUS
            && v->datum.nodus != NIHIL
            && v->datum.nodus->genus == genus)
        {
            redde v->datum.nodus;
        }
    }
    redde NIHIL;
}

s32 principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;
    SilvaGLR* sceletum;
    SilvaGLR* imparilis;

    piscina = piscina_generare_dynamicum("probatio_silva_ambiguitas",
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

    CREDO_NON_NIHIL (sceletum);
    CREDO_NON_NIHIL (imparilis);


    /* ========================================================
     * PROBARE: furca compacta eiusdem-undae (sceletum)
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
             SilvaNodus* ambiguum;
             SilvaValor  interps;
             SilvaValor  canonica;

        imprimere("\n--- Probans furcam compactam (sceletum) ---\n");

        fructus = _parsare(piscina, sceletum, "foo * bar;");
        CREDO_VERUM (fructus.successus);

        /* LOCALIZATA: radix est lista ordinaria unius elementi */
        CREDO_AEQUALIS_S32 ((s32)fructus.valor.genus,
            (s32)SILVA_VALOR_LISTA);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(fructus.valor),
            I);

        ambiguum = _elementum(fructus.valor, ZEPHYRUM);
        CREDO_NON_NIHIL (ambiguum);
        CREDO_AEQUALIS_S32 (ambiguum->genus,
            (s32)SILVA_SCELETUM_GENUS_AMBIGUUS);

        /* Interpretationes: declaratio UNA + sententia-expressionis
         * UNA (ordo brachiorum non asseritur) */
        interps = silva_sceletum_ambiguus_interpretationes(ambiguum);
        CREDO_AEQUALIS_S32 ((s32)interps.genus, (s32)SILVA_VALOR_LISTA);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(interps), II);
        CREDO_AEQUALIS_I32 (_interpretationes_generis(interps,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO), I);
        CREDO_AEQUALIS_I32 (_interpretationes_generis(interps,
            (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS), I);

        canonica = silva_sceletum_ambiguus_canonica(ambiguum);
        CREDO_AEQUALIS_S32 ((s32)canonica.genus,
            (s32)SILVA_VALOR_INDEX);
        CREDO_AEQUALIS_S32 (canonica.datum.index, ZEPHYRUM);

        /* Lectio declarationis integra */
        {
            SilvaNodus* declaratio = _interpretatio_generis(interps,
                (s32)SILVA_SCELETUM_GENUS_DECLARATIO);
            SilvaValor typus =
                silva_sceletum_declaratio_typus(declaratio);
            SilvaValor declarator =
                silva_sceletum_declaratio_declarator(declaratio);

            CREDO_AEQUALIS_S32 (typus.datum.nodus->genus,
                (s32)SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS);
            CREDO_CHORDA_AEQUALIS_LITERIS (
                silva_sceletum_typus_nominatus_tok_titulus(
                    typus.datum.nodus).datum.token->valor, "foo");
            CREDO_AEQUALIS_S32 (declarator.datum.nodus->genus,
                (s32)SILVA_SCELETUM_GENUS_DECLARATOR_MONSTRATOR);
        }

        /* Lectio expressionis integra */
        {
            SilvaNodus* sententia = _interpretatio_generis(interps,
                (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS);
            SilvaValor expressio =
                silva_sceletum_sententia_expressionis_expressio(sententia);

            CREDO_AEQUALIS_S32 (expressio.datum.nodus->genus,
                (s32)SILVA_SCELETUM_GENUS_BINARIUM);
            CREDO_CHORDA_AEQUALIS_LITERIS (
                silva_sceletum_binarium_tok_operator(
                    expressio.datum.nodus).datum.token->valor, "*");
        }

        /* Numeratores: compactio ante exhaustionem - NULLA
         * transmutatio (profunditas par) */
        CREDO_MAIOR_I32 (fructus.fusiones, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (fructus.transmutationes, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (fructus.transmutationes_negatae, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (fructus.numerus_acceptorum, I);
    }


    /* ========================================================
     * PROBARE: localizatio inter sententias ordinarias
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
             SilvaNodus* primus;
             SilvaNodus* medius;
             SilvaNodus* ultimus;

        imprimere("\n--- Probans localizationem ---\n");

        fructus = _parsare(piscina, sceletum,
            "int a; foo * bar; b + 1;");
        CREDO_VERUM (fructus.successus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(fructus.valor),
            III);

        primus   = _elementum(fructus.valor, ZEPHYRUM);
        medius   = _elementum(fructus.valor, I);
        ultimus  = _elementum(fructus.valor, II);
        CREDO_AEQUALIS_S32 (primus->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO);
        CREDO_AEQUALIS_S32 (medius->genus,
            (s32)SILVA_SCELETUM_GENUS_AMBIGUUS);
        CREDO_AEQUALIS_S32 (ultimus->genus,
            (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS);
    }


    /* ========================================================
     * PROBARE: furcae plures independentes
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
             SilvaNodus* a;
             SilvaNodus* b;

        imprimere("\n--- Probans furcas plures ---\n");

        fructus = _parsare(piscina, sceletum, "foo * bar; baz * qux;");
        CREDO_VERUM (fructus.successus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(fructus.valor),
            II);

        a = _elementum(fructus.valor, ZEPHYRUM);
        b = _elementum(fructus.valor, I);
        CREDO_AEQUALIS_S32 (a->genus,
            (s32)SILVA_SCELETUM_GENUS_AMBIGUUS);
        CREDO_AEQUALIS_S32 (b->genus,
            (s32)SILVA_SCELETUM_GENUS_AMBIGUUS);
        CREDO_INAEQUALITAS_PTR (a, b);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            silva_sceletum_ambiguus_interpretationes(a)), II);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            silva_sceletum_ambiguus_interpretationes(b)), II);
    }


    /* ========================================================
     * PROBARE: inputa inambigua - numeratores taciti
     * ======================================================== */

    {
        SilvaGLRFructus fructus;

        imprimere("\n--- Probans inambigua ---\n");

        fructus = _parsare(piscina, sceletum, "int x; y + 2;");
        CREDO_VERUM (fructus.successus);
        CREDO_AEQUALIS_I32 (fructus.fusiones, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (fructus.transmutationes, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (fructus.transmutationes_negatae, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (fructus.eventa_marginis_novi, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_elementum(fructus.valor, ZEPHYRUM)->genus,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO);
    }


    /* ========================================================
     * PROBARE: TRANSMUTATIO (imparilis - profunditas impar 2/3)
     * ======================================================== */

    {
        SilvaGLRFructus  fructus;
             SilvaNodus* ambiguum;
             SilvaValor  interps;

        imprimere("\n--- Probans transmutationem (imparilis) ---\n");

        fructus = _parsare(piscina, imparilis, "foo * bar;");
        CREDO_VERUM (fructus.successus);

        /* TRANSMUTATIO accensa: brachium expressionis serum venit,
         * sententia declarationis iam exhausta et in listam reposita
         * erat - monstrator repositus AMBIGUUM genus nunc monstrat
         * (identitas allocationis religata, probatio ipsa) */
        CREDO_AEQUALIS_I32 (fructus.transmutationes, I);
        CREDO_AEQUALIS_I32 (fructus.transmutationes_negatae, ZEPHYRUM);

        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(fructus.valor),
            I);
        ambiguum = _elementum(fructus.valor, ZEPHYRUM);
        CREDO_NON_NIHIL (ambiguum);
        CREDO_AEQUALIS_S32 (ambiguum->genus,
            (s32)SILVA_IMPARILIS_GENUS_AMBIGUUS);

        interps = silva_imparilis_ambiguus_interpretationes(ambiguum);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(interps), II);
        CREDO_AEQUALIS_I32 (_interpretationes_generis(interps,
            (s32)SILVA_IMPARILIS_GENUS_DECLARATIO), I);
        CREDO_AEQUALIS_I32 (_interpretationes_generis(interps,
            (s32)SILVA_IMPARILIS_GENUS_SENTENTIA_EXPRESSIONIS), I);
        CREDO_AEQUALIS_S32 (
            silva_imparilis_ambiguus_canonica(ambiguum).datum.index,
            ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: imparilis inambigua + localizatio post
     * transmutationem
     * ======================================================== */

    {
        SilvaGLRFructus fructus;

        imprimere("\n--- Probans imparilem ordinariam ---\n");

        fructus = _parsare(piscina, imparilis, "x + 1;");
        CREDO_VERUM (fructus.successus);
        CREDO_AEQUALIS_I32 (fructus.transmutationes, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_elementum(fructus.valor, ZEPHYRUM)->genus,
            (s32)SILVA_IMPARILIS_GENUS_SENTENTIA_EXPRESSIONIS);

        fructus = _parsare(piscina, imparilis, "int a; foo * bar;");
        CREDO_VERUM (fructus.successus);
        CREDO_AEQUALIS_I32 (fructus.transmutationes, I);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(fructus.valor),
            II);
        CREDO_AEQUALIS_S32 (_elementum(fructus.valor, ZEPHYRUM)->genus,
            (s32)SILVA_IMPARILIS_GENUS_DECLARATIO);
        CREDO_AEQUALIS_S32 (_elementum(fructus.valor, I)->genus,
            (s32)SILVA_IMPARILIS_GENUS_AMBIGUUS);
    }


    /* ========================================================
     * PROBARE: validatio imparilis (unicitas symboli accessus
     * in AMBABUS tabulis)
     * ======================================================== */

    {
        imprimere("\n--- Probans validationem ambarum ---\n");

        CREDO_VERUM (silva_glr_tabulam_validare(piscina,
            &SILVA_SCELETUM_TABULA));
        CREDO_VERUM (silva_glr_tabulam_validare(piscina,
            &SILVA_IMPARILIS_TABULA));
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
