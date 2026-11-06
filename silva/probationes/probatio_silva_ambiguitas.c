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
#include "silva_tabulae_c89.h"
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

/* Aequalitas structuralis (lexemata per IDENTITATEM - fluxus idem
 * utrique parsurae datur) */
interior b32
_valores_pares (
    SilvaValor a,
    SilvaValor b)
{
    i32 i;

    si (a.genus != b.genus)
    {
        redde FALSUM;
    }
    commutatio (a.genus)
    {
    casus SILVA_VALOR_NIHIL:
        redde VERUM;
    casus SILVA_VALOR_TOKEN:
        redde (a.datum.token == b.datum.token) ? VERUM : FALSUM;
    casus SILVA_VALOR_INDEX:
        redde (a.datum.index == b.datum.index) ? VERUM : FALSUM;
    casus SILVA_VALOR_NODUS:
        si (a.datum.nodus == NIHIL || b.datum.nodus == NIHIL)
        {
            redde (a.datum.nodus == b.datum.nodus) ? VERUM : FALSUM;
        }
        si (   a.datum.nodus->genus != b.datum.nodus->genus
            || a.datum.nodus->numerus_locorum
                != b.datum.nodus->numerus_locorum)
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < a.datum.nodus->numerus_locorum; i++)
        {
            si (!_valores_pares(a.datum.nodus->loci[i],
                    b.datum.nodus->loci[i]))
            {
                redde FALSUM;
            }
        }
        redde VERUM;
    casus SILVA_VALOR_LISTA:
        si (silva_valor_lista_numerus(a)
            != silva_valor_lista_numerus(b))
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < silva_valor_lista_numerus(a); i++)
        {
            SilvaValor* ea = silva_valor_lista_obtinere(a, i);
            SilvaValor* eb = silva_valor_lista_obtinere(b, i);

            si (   ea == NIHIL || eb == NIHIL
                || !_valores_pares(*ea, *eb))
            {
                redde FALSUM;
            }
        }
        redde VERUM;
    ordinarius:
        redde FALSUM;
    }
}

/* VIA CELER contra GENERALEM (budget silvae gradus Ib, 2026-11-03):
 * fluxus idem bis parsatur, via celer accensa et exstincta. Generalis
 * est oraculum: arbores structuraliter pares, numeratores omnes pares.
 * Reddit fructum generalem pro assertionibus de copia furcarum: ne
 * probatio vacua sit, inputa cessiones viae celeris accendere
 * debent. */
interior SilvaGLRFructus
_differentialis (
               Piscina* piscina,
              SilvaGLR* glr,
    constans character* fons)
{
                Xar* lexemata;
    SilvaGLRFructus  celer;
    SilvaGLRFructus  generalis;
           SilvaGLR  glr_celer;   /* numeratores post viam celerem */

    lexemata = silva_lexare(piscina, fons, (i32)strlen(fons), ZEPHYRUM);

    glr->via_celer  = VERUM;
    celer           = silva_glr_parsare(glr, lexemata, piscina);
    glr_celer       = *glr;

    glr->via_celer  = FALSUM;
    generalis       = silva_glr_parsare(glr, lexemata, piscina);
    glr->via_celer  = VERUM;

    CREDO_VERUM (generalis.successus);
    CREDO_AEQUALIS_S32 (celer.successus, generalis.successus);
    CREDO_VERUM (_valores_pares(celer.valor, generalis.valor));
    CREDO_AEQUALIS_I32 (celer.numerus_acceptorum,
        generalis.numerus_acceptorum);
    CREDO_AEQUALIS_I32 (celer.fusiones, generalis.fusiones);
    CREDO_AEQUALIS_I32 (celer.transmutationes,
        generalis.transmutationes);
    CREDO_AEQUALIS_I32 (celer.transmutationes_negatae,
        generalis.transmutationes_negatae);
    CREDO_AEQUALIS_I32 (celer.eventa_marginis_novi,
        generalis.eventa_marginis_novi);
    CREDO_AEQUALIS_I32 (celer.frons_maxima, generalis.frons_maxima);
    CREDO_AEQUALIS_I32 (glr_celer.nodi_creati, glr->nodi_creati);
    CREDO_AEQUALIS_I32 (glr_celer.reductiones_factae,
        glr->reductiones_factae);
    imprimere("  differentialis: fus %d trm %d marg %d frons %d\n",
        (int)generalis.fusiones, (int)generalis.transmutationes,
        (int)generalis.eventa_marginis_novi,
        (int)generalis.frons_maxima);
    redde generalis;
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
        SilvaGLRFructus fructus;
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
        SilvaGLRFructus fructus;
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
        SilvaGLRFructus fructus;
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
        SilvaGLRFructus fructus;
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


    /* ========================================================
     * PROBARE: VIA CELER = VIA GENERALIS (differentialis, gradus Ib)
     *
     * Via celer passus determinatos sine indice currit et in furcis
     * generali CEDIT, catenam suam in indicem transferens. Inputa
     * furcas, compactiones et transmutationes accendunt, ut cessiones
     * (catena translata, anuli exhausti, nodus pendens) vere
     * probentur - non solum passus simplices.
     * ======================================================== */

    {
               SilvaGLR* c89;
        SilvaGLRFructus  fructus;

        imprimere("\n--- Probans viam celerem contra generalem ---\n");

        fructus = _differentialis(piscina, sceletum, "foo * bar;");
        CREDO_MAIOR_I32 (fructus.fusiones, ZEPHYRUM);
        (vacuum)_differentialis(piscina, sceletum,
            "int a; foo * bar; b + 1; baz * qux;");
        fructus = _differentialis(piscina, imparilis, "foo * bar;");
        CREDO_AEQUALIS_I32 (fructus.transmutationes, I);
        (vacuum)_differentialis(piscina, imparilis,
            "int a; foo * bar;");

        c89 = silva_glr_creare(piscina, &SILVA_C89_TABULA,
            silva_c89_construere, silva_c89_ambiguum_fabricare);
        CREDO_NON_NIHIL (c89);
        si (c89 != NIHIL)
        {
            /* Frons fusa (praedecessores plures) + margines novi */
            fructus = _differentialis(piscina, c89,
                "void f(T bytes[16]);");
            CREDO_MAIOR_I32 (fructus.fusiones, ZEPHYRUM);
            CREDO_MAIOR_I32 (fructus.eventa_marginis_novi, ZEPHYRUM);

            /* Transmutationes: catena translata cum anulis exhaustis */
            fructus = _differentialis(piscina, c89,
                "int g(void) { T * p; a * b; x = y + 1 * (z - 2);"
                " return f(p); }");
            CREDO_MAIOR_I32 (fructus.transmutationes, ZEPHYRUM);

            /* Assignatio catenata: status idem basi alia INTRA
             * catenam celerem (margo novus sine furca) */
            fructus = _differentialis(piscina, c89,
                "int k(void) { a = b = c = d; return a; }");
            CREDO_MAIOR_I32 (fructus.eventa_marginis_novi, ZEPHYRUM);

            /* Status visus post cessionem: reductio generalis statum
             * anuli catenae (basi alia) attingit - catena in indicem
             * NON translata margines amitteret (planta B; minimum ex
             * lib/carmen.c reductum: arbores eaedem, numeratores
             * non) */
            fructus = _differentialis(piscina, c89,
                "void g(void) { a[*n] = y; }");
            CREDO_MAIOR_I32 (fructus.eventa_marginis_novi, ZEPHYRUM);

            /* Omnia simul, frons ultra duos */
            fructus = _differentialis(piscina, c89,
                "int h(int n) { int i; for (i = 0; i < n; i++)"
                " { s = (T)(u) + v[i]; (q)(r); } return s; }");
            CREDO_MAIOR_I32 (fructus.transmutationes, ZEPHYRUM);
            CREDO_MAIOR_I32 (fructus.eventa_marginis_novi, ZEPHYRUM);
            CREDO_MAIOR_I32 (fructus.frons_maxima, II);
        }
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
