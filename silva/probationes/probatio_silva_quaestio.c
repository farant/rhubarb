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

/* capturam nomine invenire (QB); NIHIL = absens */
interior constans SilvaNodus*
_captura (constans SilvaQuaestioResultatum* r,
    constans character* titulus)
{
    i32 i;

    si (r == NIHIL || r->capturae == NIHIL) redde NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(r->capturae); i++)
    {
        SilvaQuaestioCaptura* cap = (SilvaQuaestioCaptura*)
            xar_obtinere(r->capturae, i);

        si (cap != NIHIL
            && cap->titulus.mensura == (i32)strlen(titulus)
            && memcmp(cap->titulus.datum, titulus,
                   strlen(titulus)) == ZEPHYRUM)
        {
            redde cap->nodus;
        }
    }
    redde NIHIL;
}

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

    /* ========================================================
     * PROBARE QB: attributa [locus op "valor"] (quinque
     * operationes; resolutio per genus; validatio compilationis),
     * capturae $nomen (gradus OMNES - insectum v1 #1), fratres
     * +/~, fracturae novae
     * ======================================================== */
    {
        constans character* fons =
            "int a;\n"
            "int quadratum(int x) { int y; y = x * x; return y; }\n"
            "void nihil_facere(void) { return; }\n";
        SilvaParsura* parsura;

        imprimere("\n--- Probans QB: attributa/capturae/fratres"
            " ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        /* attributa: quinque operationes */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus=\"quadratum\"]"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus^=\"quadr\"]"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus$=\"atum\"]"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus*=\"adrat\"]"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus]"), V);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus=\"nusquam\"]"),
            ZEPHYRUM);
        /* attributum in composito descendenti */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis"
            " declarator-titulus[tok_titulus=\"y\"]"), I);
        /* locus ignotus SINE tag: differt ad tempus cursus -
         * compilat, nihil congruit */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "[locus_fictus]"), ZEPHYRUM);

        /* fractura compilationis: locus generi ignotus CUM tag
         * (viae attributorum mortuae v1 exstirpatae) */
        {
            constans character* causa = NIHIL;

            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, "declaratio[locus_fictus]",
                &causa));
            CREDO_VERUM (causa != NIHIL
                && strcmp(causa, "locus generi ignotus")
                    == ZEPHYRUM);
            causa = NIHIL;
            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, "[=\"x\"]", &causa));
            causa = NIHIL;
            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, "[tok_titulus^\"x\"]",
                &causa));
            causa = NIHIL;
            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, "[tok_titulus=\"x]",
                &causa));
            causa = NIHIL;
            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, "declaratio$", &causa));
        }

        /* fratres: corpus quadrati = [declaratio,
         * sententia-expressionis, redde] */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio + sententia-expressionis"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio + redde"), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio ~ redde"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "sententia-expressionis + redde"), I);

        /* capturae: gradus OMNES catenae ligati (insectum v1 #1) */
        {
            constans character* causa = NIHIL;
            SilvaQuaestio* q = silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM,
                "definitio-functionis$f binarium$b", &causa);
            SilvaQuaestio* q_def = silva_quaestio_compilare(
                piscina, &SILVA_C89_REGISTRUM,
                "definitio-functionis", &causa);
            Xar* resultata;

            CREDO_NON_NIHIL (q);
            CREDO_NON_NIHIL (q_def);
            resultata = silva_quaestio_exsequi(q,
                parsura->commissio->radix, piscina);
            CREDO_NON_NIHIL (resultata);
            CREDO_AEQUALIS_I32 (xar_numerus(resultata), I);
            {
                SilvaQuaestioResultatum* r =
                    (SilvaQuaestioResultatum*)xar_obtinere(
                        resultata, ZEPHYRUM);
                constans SilvaNodus* f;
                constans SilvaNodus* b;

                CREDO_NON_NIHIL (r);
                CREDO_NON_NIHIL (r->capturae);
                CREDO_AEQUALIS_I32 (xar_numerus(r->capturae), II);
                b = _captura(r, "b");
                f = _captura(r, "f");
                CREDO_VERUM (b == r->nodus);
                CREDO_NON_NIHIL (f);
                CREDO_VERUM (silva_quaestio_congruit(q_def, f));
                CREDO_VERUM (f != b);
            }
        }
        /* sine capturis: capturae NIHIL (nulla allocatio) */
        {
            constans character* causa = NIHIL;
            SilvaQuaestio* q = silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, "definitio-functionis",
                &causa);
            Xar* resultata = silva_quaestio_exsequi(q,
                parsura->commissio->radix, piscina);
            SilvaQuaestioResultatum* r;

            CREDO_NON_NIHIL (resultata);
            r = (SilvaQuaestioResultatum*)xar_obtinere(resultata,
                ZEPHYRUM);
            CREDO_NON_NIHIL (r);
            CREDO_NIHIL ((vacuum*)r->capturae);
        }
        /* unio: catena congruens PRIMA ligat - subiecta redde
         * capturam "r" ferunt, binarium "b" */
        {
            constans character* causa = NIHIL;
            SilvaQuaestio* q = silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, "redde$r, binarium$b",
                &causa);
            Xar* resultata = silva_quaestio_exsequi(q,
                parsura->commissio->radix, piscina);
            i32 cum_b = ZEPHYRUM;
            i32 cum_r = ZEPHYRUM;
            i32 i;

            CREDO_NON_NIHIL (resultata);
            CREDO_AEQUALIS_I32 (xar_numerus(resultata), III);
            per (i = ZEPHYRUM; i < xar_numerus(resultata); i++)
            {
                SilvaQuaestioResultatum* r =
                    (SilvaQuaestioResultatum*)xar_obtinere(
                        resultata, i);

                si (_captura(r, "b") != NIHIL) cum_b++;
                si (_captura(r, "r") != NIHIL) cum_r++;
            }
            CREDO_AEQUALIS_I32 (cum_b, I);
            CREDO_AEQUALIS_I32 (cum_r, II);
        }
    }

    /* ========================================================
     * PROBARE QB: lexemata interposita in fratribus (virgulae
     * congeriei transilitae) + locus LISTA lexematum
     * (folium-chorda: elementum ULLUM congruit)
     * ======================================================== */
    {
        constans character* fons =
            "int arr[3] = {1, 2, 3};\n"
            "char* s = \"ab\" \"cd\";\n";
        SilvaParsura* parsura;

        imprimere("\n--- Probans QB: congeries/folium-chorda"
            " ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        /* elementa congeriei: [folium, COMMA, folium, COMMA,
         * folium] - virgulae transilitae; folium "3" in loco
         * mensurae NODO directo sedet = sine fratribus */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "folium-integer + folium-integer"), II);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "folium-integer ~ folium-integer"), II);
        /* attributum trans fratrem */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "folium-integer[tok_valor=\"1\"] +"
            " folium-integer[tok_valor=\"2\"]"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "folium-integer[tok_valor=\"2\"] +"
            " folium-integer[tok_valor=\"1\"]"), ZEPHYRUM);

        /* locus LISTA: tok_valor folii-chordae = lexemata duo -
         * elementum ULLUM congruit (CONTINET vitat citationes in
         * valore - limes fugarum nominatus) */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "folium-chorda[tok_valor*=\"ab\"]"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "folium-chorda[tok_valor*=\"cd\"]"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "folium-chorda[tok_valor*=\"zz\"]"), ZEPHYRUM);
        /* tok_operator initiatoris */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-initiatus[tok_operator=\"=\"]"), II);
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
