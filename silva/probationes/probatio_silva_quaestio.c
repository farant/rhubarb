/* probatio_silva_quaestio.c - machina selectorum, QA+QB+QC
 * (tags DECISUS, *, descendens, filius, unio, fractura clara,
 * congruit, descensus canonicus per ambigua, error congruibile;
 * QB attributa/capturae/fratres; QC pseudo-classes + registratio
 * usoris; consilium = project-specs/silva-quaestio-design.md) */
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
_captura (
    constans SilvaQuaestioResultatum* r,
                  constans character* titulus)
{
    i32 i;

    si (r == NIHIL || r->capturae == NIHIL) redde NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(r->capturae); i++)
    {
        SilvaQuaestioCaptura* cap = (SilvaQuaestioCaptura*)
            xar_obtinere(r->capturae, i);

        si (   cap                  != NIHIL
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
_numerus (
                  Piscina* piscina,
    constans SilvaParsura* parsura,
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

/* ut _numerus, sed cum tabula pseudo usoris (QC) */
interior s32
_numerus_cum (
                                  Piscina* piscina,
                    constans SilvaParsura* parsura,
    constans SilvaQuaestioPseudoRegistrum* registro,
                       constans character* selector)
{
    constans character* causa = NIHIL;
         SilvaQuaestio* q = silva_quaestio_compilare_cum_registro(
             piscina, &SILVA_C89_REGISTRUM, registro, selector, &causa);
    Xar* resultata;

    si (q == NIHIL) redde -I;
    resultata = silva_quaestio_exsequi(q,
        parsura->commissio->radix, piscina);
    si (resultata == NIHIL) redde -I;
    redde (s32)xar_numerus(resultata);
}

/* pseudo usoris probandum (QC): congruit si argumentum == "ita";
 * datum = numerator vocationum (filum argumenti + dati probat) */
interior b32
_pseudo_argosum (
    constans SilvaNodus* nodus,
                 chorda  argumentum,
                 vacuum* datum)
{
    i32* numerator = (i32*)datum;

    (vacuum)nodus;
    si (numerator != NIHIL) (*numerator)++;
    redde (argumentum.mensura == III
        && memcmp(argumentum.datum, "ita", III) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

s32 principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

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


    /* ========================================================
     * PROBARE QC: pseudo-classes structurales/logicales/
     * semanticae (:primus/:ultimus, :habet limite confinatum,
     * :non, :vocat/:definit/:utitur/:reddit, capturae compositae)
     * ======================================================== */

    {
        constans character* fons =
            "int a;\n"
            "int quadratum(int x) { int y; y = x * x; return y; }\n"
            "void nihil_facere(void) { return; }\n"
            "int cubus(int x) { return quadratum(x) * x; }\n"
            "void vacua(void) { int z; z = 1; }\n";
        SilvaParsura* parsura;

        imprimere("\n--- Probans QC: pseudo-classes ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis"), IV);

        /* :reddit - CONTINET sententiam redde */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:reddit"), III);
        /* :non(:reddit) - vacua sola */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:non(:reddit)"), I);
        /* :vocat - folium directum vocationis */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:vocat(quadratum)"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:vocat(nusquam)"), ZEPHYRUM);
        /* :habet cum tag nidificato */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:habet(vocatio)"), I);
        /* :definit - declarator-titulus (parametra quoque) */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:definit(y)"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:definit(x)"), II);
        /* :utitur - folium-identificator in corpore */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:utitur(x)"), II);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:utitur(a)"), ZEPHYRUM);

        /* :habet LIMES (paritas CSS): catena nidificata intra
         * subarborem subiecti confinata - definitio SUPRA corpus
         * subiectum numquam ancoram praebet */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "corpus:habet(binarium)"), II);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "corpus:habet(definitio-functionis binarium)"),
            ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:habet(corpus binarium)"), II);

        /* :primus/:ultimus intra corpora */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "corpus > declaratio:primus"), II);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "corpus > redde:ultimus"), III);
        /* nodi summi: pater absens = sine lista continente =
         * filius solus = AMBO congruunt (DECISUS pinnatum -
         * quaestiones utiles sub patre scopandae) */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:primus"), IV);

        /* unio INTRA :non (compilatio nidificata plena) */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:non(:vocat(quadratum),"
            " declaratio)"), III);

        /* captura composita cum pseudo + congruit */
        {
            constans character* causa = NIHIL;
                 SilvaQuaestio* q = silva_quaestio_compilare(piscina,
                     &SILVA_C89_REGISTRUM,
                     "definitio-functionis:vocat(quadratum)$f",
                     &causa);
            SilvaQuaestio* q_reddit = silva_quaestio_compilare(
                piscina, &SILVA_C89_REGISTRUM,
                "definitio-functionis:reddit", &causa);
            SilvaQuaestio* q_definit = silva_quaestio_compilare(
                piscina, &SILVA_C89_REGISTRUM,
                "definitio-functionis:definit(y)", &causa);
            Xar* resultata;

            CREDO_NON_NIHIL (q);
            CREDO_NON_NIHIL (q_reddit);
            CREDO_NON_NIHIL (q_definit);
            resultata = silva_quaestio_exsequi(q,
                parsura->commissio->radix, piscina);
            CREDO_NON_NIHIL (resultata);
            CREDO_AEQUALIS_I32 (xar_numerus(resultata), I);
            {
                SilvaQuaestioResultatum* r =
                    (SilvaQuaestioResultatum*)xar_obtinere(
                        resultata, ZEPHYRUM);

                CREDO_NON_NIHIL (r);
                CREDO_VERUM (_captura(r, "f") == r->nodus);
                /* congruit cum pseudo: cubus reddit, y non
                 * definit */
                CREDO_VERUM (silva_quaestio_congruit(q_reddit,
                    r->nodus));
                CREDO_FALSUM (silva_quaestio_congruit(q_definit,
                    r->nodus));
            }
        }
    }


    /* ========================================================
     * PROBARE QC: filius solus in loco NODO directo (DECISUS:
     * AMBO :primus/:ultimus congruunt)
     * ======================================================== */

    {
        constans character* fons = "int arr[7];\n";
              SilvaParsura* parsura;

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "folium-integer:primus"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "folium-integer:ultimus"), I);
    }


    /* ========================================================
     * PROBARE QC: :ambiguum + :lectiones (lectiones omnes
     * apertae) + :sumptus/:omissus (textura conditionalium)
     * ======================================================== */

    {
        constans character* fons = "Ignotus * x;\n";
              SilvaParsura* parsura;

        imprimere("\n--- Probans QC: lectiones/rami ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);

        /* :ambiguum = involucrum ipsum (aequivalens tag) */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            ":ambiguum"), I);
        /* sine :lectiones - lectio canonica sola (expressionis) */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio"), ZEPHYRUM);
        /* :lectiones - declaratio lectionis alterius INVENITUR */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio:lectiones"), I);
        /* canonica adhuc visibilis cum vexillo */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "binarium:lectiones"), I);
    }
    {
        constans character* fons =
            "#if 1\n"
            "int sumptum_var;\n"
            "#else\n"
            "int omissum_var;\n"
            "#endif\n"
            "int post;\n";
        SilvaParsura* parsura;

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "conditionalis"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "ramus-sumptus"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "ramus-omissus"), I);
        /* bracchium omissum crudum - declaratio eius non parsata */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio"), II);
        /* :sumptus - intra bracchium sumptum sola */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio:sumptus"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio:omissus"), ZEPHYRUM);
        /* nodus bracchii ipse maior-aut-ipse congruit */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "ramus-sumptus:sumptus"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "ramus-omissus:omissus"), I);
    }


    /* ========================================================
     * PROBARE QC: registratio usoris (tabula ANTE compilationem;
     * insectum v1 #4 functionale factum) + fracturae pseudo
     * ======================================================== */

    {
        constans character* fons =
            "int a;\n"
            "int quadratum(int x) { int y; y = x * x; return y; }\n"
            "void vacua(void) { int z; z = 1; }\n";
                        SilvaParsura* parsura;
        SilvaQuaestioPseudoRegistrum* registro;
                                 i32  numerator = ZEPHYRUM;

        imprimere("\n--- Probans QC: registratio/fracturae ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);

        registro = silva_quaestio_registrum_creare(piscina);
        CREDO_NON_NIHIL (registro);
        /* recusationes registrationis */
        CREDO_FALSUM (silva_quaestio_registrare(registro,
            "argosum", NIHIL, NIHIL));
        CREDO_FALSUM (silva_quaestio_registrare(registro, "vocat",
            _pseudo_argosum, NIHIL));   /* nativum obumbrare */
        CREDO_FALSUM (silva_quaestio_registrare(registro, "Malus",
            _pseudo_argosum, NIHIL));   /* littera non-tag */
        CREDO_FALSUM (silva_quaestio_registrare(registro, "",
            _pseudo_argosum, NIHIL));
        /* registratio vera */
        CREDO_VERUM (silva_quaestio_registrare(registro, "argosum",
            _pseudo_argosum, &numerator));

        /* sine tabula: fractura clara */
        {
            constans character* causa = NIHIL;

            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, "declaratio:argosum(ita)",
                &causa));
            CREDO_VERUM (causa != NIHIL
                && strcmp(causa, "pseudo ignotum") == ZEPHYRUM);
        }
        /* cum tabula: functio + argumentum + datum feruntur */
        CREDO_AEQUALIS_S32 (_numerus_cum(piscina, parsura,
            registro, "declaratio:argosum(ita)"), III);
        CREDO_AEQUALIS_I32 (numerator, III);   /* declarationes
            solae probatae (genus prior in composito) */
        CREDO_AEQUALIS_S32 (_numerus_cum(piscina, parsura,
            registro, "declaratio:argosum(minime)"), ZEPHYRUM);
        /* registratio iterata = renovatio (VERUM) */
        CREDO_VERUM (silva_quaestio_registrare(registro, "argosum",
            _pseudo_argosum, NIHIL));

        /* fracturae compilationis pseudo (causae nominatae) */
        {
            constans character* causa = NIHIL;

            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, ":fictus", &causa));
            CREDO_VERUM (causa != NIHIL
                && strcmp(causa, "pseudo ignotum") == ZEPHYRUM);
            causa = NIHIL;
            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, "declaratio:primus(x)",
                &causa));
            CREDO_VERUM (causa != NIHIL
                && strcmp(causa, "pseudo argumentum non accipit")
                    == ZEPHYRUM);
            causa = NIHIL;
            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM,
                "definitio-functionis:vocat", &causa));
            CREDO_VERUM (causa != NIHIL
                && strcmp(causa, "pseudo argumentum requirit")
                    == ZEPHYRUM);
            causa = NIHIL;
            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM,
                "definitio-functionis:vocat()", &causa));
            CREDO_VERUM (causa != NIHIL
                && strcmp(causa, "pseudo argumentum requirit")
                    == ZEPHYRUM);
            causa = NIHIL;
            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM,
                "definitio-functionis:habet(vocatio", &causa));
            CREDO_VERUM (causa != NIHIL
                && strcmp(causa, "argumentum pseudo non clausum")
                    == ZEPHYRUM);
            causa = NIHIL;
            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, "declaratio:", &causa));
            CREDO_VERUM (causa != NIHIL
                && strcmp(causa, "pseudo sine nomine")
                    == ZEPHYRUM);
            /* causa nidificata propagatur */
            causa = NIHIL;
            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM, ":habet(nodus-fictus)",
                &causa));
            CREDO_VERUM (causa != NIHIL
                && strcmp(causa, "tag registro ignotum")
                    == ZEPHYRUM);
            /* nidificatio nimis profunda (custos XVI) */
            causa = NIHIL;
            CREDO_NIHIL (silva_quaestio_compilare(piscina,
                &SILVA_C89_REGISTRUM,
                ":non(:non(:non(:non(:non(:non(:non(:non("
                ":non(:non(:non(:non(:non(:non(:non(:non("
                ":non(declaratio"
                ")))))))))))))))))", &causa));
            CREDO_VERUM (causa != NIHIL
                && strcmp(causa,
                       "pseudo nidificatio nimis profunda")
                    == ZEPHYRUM);
        }
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
