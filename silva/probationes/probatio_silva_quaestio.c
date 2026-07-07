/* probatio_silva_quaestio.c - machina selectorum, QA
 * (tags DECISUS, *, descendens, filius, unio, fractura clara,
 * congruit, descensus canonicus per ambigua, error congruibile;
 * consilium = project-specs/silva-quaestio-design.md) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_quaestio.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior s32
_numerus (Piscina* piscina, constans SilvaParsura* parsura,
    constans character* selector)
{
    constans character* causa = NIHIL;
    SilvaQuaestio* q = silva_quaestio_compilare(piscina,
        &SILVA_C89_REGISTRUM, selector, &causa);
    Xar* resultata;

    si (q == NIHIL) redde -I;
    resultata = silva_quaestio_exsequi(q,
        parsura->commissio->radix, piscina);
    si (resultata == NIHIL) redde -I;
    redde (s32)xar_numerus(resultata);
}

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_quaestio",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: selectores fundamentales super programma parvum
     * ======================================================== */
    {
        constans character* fons =
            "int a;\n"
            "int quadratum(int x) { int y; y = x * x; return y; }\n"
            "void nihil_facere(void) { return; }\n";
        SilvaParsura* parsura;

        imprimere("\n--- Probans selectores fundamentales ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        /* tag generis: declarationes (una summa, una localis) */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio"), II);
        /* definitiones */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis"), II);
        /* tag latinum P1: redde-sententiae */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "redde"), II);
        /* descendens: declaratio intra definitionem (int y sola) */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis declaratio"), I);
        /* filius directus: corpus sub definitione */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis > corpus"), II);
        /* filius directus: declaratio sub corpore */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "corpus > declaratio"), I);
        /* declaratio summa NON est filius definitionis */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis > declaratio"), ZEPHYRUM);
        /* universalis: multa */
        CREDO_VERUM (_numerus(piscina, parsura, "*") > (s32)X);
        /* unio */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio, definitio-functionis"), IV);
        /* catena trium: binarium intra corpus intra definitionem */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis corpus binarium"), I);

        /* congruit: subiectum inventum eandem quaestionem congruit,
         * aliam non */
        {
            constans character* causa = NIHIL;
            SilvaQuaestio* q_def = silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, "definitio-functionis",
                &causa);
            SilvaQuaestio* q_decl = silva_quaestio_compilare(
                piscina, &SILVA_C89_REGISTRUM, "declaratio",
                &causa);
            Xar* resultata;

            CREDO_NON_NIHIL (q_def);
            CREDO_NON_NIHIL (q_decl);
            resultata = silva_quaestio_exsequi(q_def,
                parsura->commissio->radix, piscina);
            CREDO_NON_NIHIL (resultata);
            CREDO_AEQUALIS_I32 (xar_numerus(resultata), II);
            {
                SilvaQuaestioResultatum* r =
                    (SilvaQuaestioResultatum*)xar_obtinere(
                        resultata, ZEPHYRUM);

                CREDO_NON_NIHIL (r);
                CREDO_VERUM (silva_quaestio_congruit(q_def,
                    r->nodus));
                CREDO_FALSUM (silva_quaestio_congruit(q_decl,
                    r->nodus));
            }
        }
    }

    /* ========================================================
     * PROBARE: fractura clara (tag ignotum = mutatio deliberata
     * a v1, DECISUS; selectores malformati)
     * ======================================================== */
    {
        constans character* causa = NIHIL;

        imprimere("\n--- Probans fracturas claras ---\n");

        CREDO_NIHIL (silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "nodus-inexistens", &causa));
        CREDO_VERUM (causa != NIHIL);
        causa = NIHIL;
        CREDO_NIHIL (silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "", &causa));
        CREDO_VERUM (causa != NIHIL);
        causa = NIHIL;
        CREDO_NIHIL (silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "> declaratio", &causa));
        causa = NIHIL;
        CREDO_NIHIL (silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "declaratio >", &causa));
        causa = NIHIL;
        CREDO_NIHIL (silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "declaratio,", &causa));
        causa = NIHIL;
        CREDO_NIHIL (silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "declaratio [x", &causa));
    }

    /* ========================================================
     * PROBARE: ambigua - descensus canonicus solus (DECISUS);
     * involucrum ipsum congruibile (tag "ambiguus")
     * ======================================================== */
    {
        constans character* fons = "Ignotus * x;\n";
        SilvaParsura* parsura;

        imprimere("\n--- Probans descensum canonicum ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura->commissio->ambigui), I);

        /* involucrum congruibile */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "ambiguus"), I);
        /* canonica = lectio expressionis (politica) - declaratio
         * intra lectionem alteram NON invenitur */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio"), ZEPHYRUM);
        /* sed binarium lectionis canonicae invenitur */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "sententia-expressionis"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "binarium"), I);
    }

    /* ========================================================
     * PROBARE: nodi ERROR congruibiles (tag "error")
     * ======================================================== */
    {
        constans character* fons = "int bonus;\n@@@;\n";
        SilvaParsura* parsura;

        imprimere("\n--- Probans errorem congruibilem ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->numerus_errorum > ZEPHYRUM);
        CREDO_VERUM (_numerus(piscina, parsura, "error")
            >= (s32)I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio"), I);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
