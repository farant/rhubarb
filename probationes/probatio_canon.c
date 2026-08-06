/* probatio_canon.c - Probationes strati schematis STML (gradus II)
 *
 * Octo sectiones: lectio canonis, iudicium sanum, classes vitiorum,
 * intra= (formae contextu dependentes - casus aedilis),
 * CANON INFIXUS (liberum primum radicis = contractus, non contentum),
 * CITATIO + SCOPI PER INSTANTIAM (clavis-relatio, mos xs:keyref),
 * registrum (clavis extensionis et clavis radicis), et CORPUS VERUM:
 * canon.canon SE IPSUM iudicat, deinde natura.canon et aedilis.canon,
 * deinde culpa plantata (attributum falso scriptum quod lector tacite
 * voraret) vitium parere debet. Lex domus: porta sine culpa plantata
 * in ortu non creditur.
 */
#include "latina.h"
#include "canon.h"
#include "credo.h"
#include "filum.h"
#include <stdio.h>

/* ==================================================
 * Fixturae - dialectus "hortus" genera valorum omnia exercet
 * ================================================== */

interior constans character* CANON_HORTUS =
    "<canon dialectus=\"hortus\" versio=\"1\">\n"
    "  <elementum nomen=\"hortus\" radix=\"verum\">\n"
    "    <attributum nomen=\"titulus\" genus=\"nomen\"\n"
    "      necessarium=\"verum\"/>\n"
    "    <liberum nomen=\"planta\" minimum=\"1\" maximum=\"2\"/>\n"
    "    <liberum nomen=\"adnotatio\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"planta\">\n"
    "    <attributum nomen=\"nomen\" genus=\"nomen\"\n"
    "      necessarium=\"verum\"/>\n"
    "    <attributum nomen=\"aetas\" genus=\"numerus\"/>\n"
    "    <attributum nomen=\"viva\"  genus=\"veritas\"/>\n"
    "    <attributum nomen=\"sata\"  genus=\"dies\"/>\n"
    "    <attributum nomen=\"color\" genus=\"electio\"\n"
    "      ordinarius=\"ruber\">\n"
    "      <optio>ruber</optio>\n"
    "      <optio>albus</optio>\n"
    "    </attributum>\n"
    "    <attributum nomen=\"cultivar\" genus=\"compositum\"/>\n"
    "    <liberum nomen=\"adnotatio\"/>\n"
    "    <liberum nomen=\"mensura\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"adnotatio\" textus=\"verum\"/>\n"
    "  <elementum nomen=\"mensura\" textus=\"numerus\"\n"
    "    nota=\"textus typatus - non vacuus generi congruere\n"
    "          debet, vacuus praeteritur\"/>\n"
    "  <unicitas nomen=\"plantae\" attributum=\"nomen\"\n"
    "    super=\"planta\"/>\n"
    "</canon>\n";

/* aetas negativa (numerus signatus), sata forma yyyy sola,
 * cultivar kebab (compositum lineolam admittit ubi nomen vetat) */
interior constans character* HORTUS_SANUS =
    "<hortus titulus=\"meus\">\n"
    "  <planta nomen=\"rosa\" aetas=\"3\" viva=\"verum\"\n"
    "    sata=\"2020-05-01\" color=\"ruber\"\n"
    "    cultivar=\"alba-plena\">\n"
    "    <adnotatio>pulchra</adnotatio>\n"
    "    <mensura>-42</mensura>\n"
    "    <mensura></mensura>\n"
    "  </planta>\n"
    "  <planta nomen=\"lilium\" aetas=\"-1\" viva=\"falsum\"\n"
    "    sata=\"2019\"/>\n"
    "</hortus>\n";

/* formae binae eiusdem nominis per intra= - casus aedilis <regula> */
interior constans character* CANON_ARCA =
    "<canon dialectus=\"arca\" versio=\"2\">\n"
    "  <elementum nomen=\"arca\" radix=\"verum\">\n"
    "    <liberum nomen=\"a\" maximum=\"1\"/>\n"
    "    <liberum nomen=\"b\" maximum=\"1\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"a\"><liberum nomen=\"cella\"/></elementum>\n"
    "  <elementum nomen=\"b\"><liberum nomen=\"cella\"/></elementum>\n"
    "  <elementum nomen=\"cella\" intra=\"a\">\n"
    "    <attributum nomen=\"x\" genus=\"numerus\"\n"
    "      necessarium=\"verum\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"cella\" intra=\"b\">\n"
    "    <attributum nomen=\"y\" genus=\"numerus\"\n"
    "      necessarium=\"verum\"/>\n"
    "  </elementum>\n"
    "</canon>\n";

/* culpa plantata: 'necesarium' (falso scriptum) - ante canon.canon
 * lector hoc tacite vorabat et postulatum evanescebat */
interior constans character* CANON_CULPA_PLANTATA =
    "<canon dialectus=\"probatio\" versio=\"1\">\n"
    "  <elementum nomen=\"res\" necesarium=\"verum\"/>\n"
    "</canon>\n";

/* canon INFIXUS: liberum primum radicis contractum fert */
interior constans character* VIRIDARIUM_INFIXO =
    "<viridarium titulus=\"meum\">\n"
    "  <canon dialectus=\"viridarium\" versio=\"1\">\n"
    "    <elementum nomen=\"viridarium\" radix=\"verum\">\n"
    "      <attributum nomen=\"titulus\" genus=\"nomen\"\n"
    "        necessarium=\"verum\"/>\n"
    "      <liberum nomen=\"arbor\" minimum=\"1\"/>\n"
    "    </elementum>\n"
    "    <elementum nomen=\"arbor\">\n"
    "      <attributum nomen=\"nomen\" genus=\"nomen\"\n"
    "        necessarium=\"verum\"/>\n"
    "    </elementum>\n"
    "  </canon>\n"
    "  <arbor nomen=\"quercus\"/>\n"
    "</viridarium>\n";

interior constans character* VIRIDARIUM_INFIXO_PECCANS =
    "<viridarium>\n"
    "  <canon dialectus=\"viridarium\" versio=\"1\">\n"
    "    <elementum nomen=\"viridarium\" radix=\"verum\">\n"
    "      <liberum nomen=\"arbor\"/>\n"
    "    </elementum>\n"
    "    <elementum nomen=\"arbor\">\n"
    "      <attributum nomen=\"nomen\" genus=\"nomen\"\n"
    "        necessarium=\"verum\"/>\n"
    "    </elementum>\n"
    "  </canon>\n"
    "  <arbor/>\n"
    "</viridarium>\n";

/* infixus vitiosus ('necesarium') - contra canonem canonum clamat */
interior constans character* VIRIDARIUM_INFIXUS_VITIOSUS =
    "<viridarium>\n"
    "  <canon dialectus=\"viridarium\" versio=\"1\">\n"
    "    <elementum nomen=\"arbor\" necesarium=\"verum\"/>\n"
    "  </canon>\n"
    "  <arbor/>\n"
    "</viridarium>\n";

/* praeterlapsus unicitatis: canon dialecti unicitatem super
 * 'elementum' fert; infixus definitiones binas 'res' legitime
 * continet (intra= diversa) - subarbor infixi praetermittenda est
 * ne NOMEN_BIS falso flagret */
interior constans character* CANON_CAPSAE =
    "<canon dialectus=\"capsa\" versio=\"1\">\n"
    "  <elementum nomen=\"capsa\" radix=\"verum\">\n"
    "    <liberum nomen=\"res\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"res\">\n"
    "    <attributum nomen=\"nomen\" genus=\"nomen\"/>\n"
    "  </elementum>\n"
    "  <unicitas nomen=\"resnomina\" attributum=\"nomen\"\n"
    "    super=\"res elementum\"/>\n"
    "</canon>\n";

interior constans character* CAPSA_INFIXO =
    "<capsa>\n"
    "  <canon dialectus=\"capsa\" versio=\"1\">\n"
    "    <elementum nomen=\"capsa\" radix=\"verum\">\n"
    "      <liberum nomen=\"res\"/>\n"
    "    </elementum>\n"
    "    <elementum nomen=\"res\" intra=\"capsa\">\n"
    "      <attributum nomen=\"nomen\" genus=\"nomen\"/>\n"
    "    </elementum>\n"
    "    <elementum nomen=\"res\" intra=\"alia\">\n"
    "      <attributum nomen=\"nomen\" genus=\"nomen\"/>\n"
    "    </elementum>\n"
    "  </canon>\n"
    "  <res nomen=\"unica\"/>\n"
    "</capsa>\n";

/* citatio (clavis-relatio) + scopi per instantiam: fons-similis
 * (documentum totum) et machina-similis (intra=) uno canone */
interior constans character* CANON_BIBLIOTHECAE =
    "<canon dialectus=\"bibliotheca\" versio=\"1\">\n"
    "  <elementum nomen=\"bibliotheca\" radix=\"verum\">\n"
    "    <liberum nomen=\"fons\"/>\n"
    "    <liberum nomen=\"liber\"/>\n"
    "    <liberum nomen=\"machina\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"fons\">\n"
    "    <attributum nomen=\"clavis\" genus=\"nomen\"\n"
    "      necessarium=\"verum\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"liber\">\n"
    "    <attributum nomen=\"fons\" genus=\"nomen\"/>\n"
    "    <attributum nomen=\"nomen\" genus=\"nomen\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"machina\">\n"
    "    <liberum nomen=\"status\"/>\n"
    "    <liberum nomen=\"transitus\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"status\">\n"
    "    <attributum nomen=\"nomen\" genus=\"nomen\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"transitus\">\n"
    "    <attributum nomen=\"ad\" genus=\"nomen\"/>\n"
    "  </elementum>\n"
    "  <citatio nomen=\"fontium\" attributum=\"fons\"\n"
    "    ad=\"fons/clavis\"/>\n"
    "  <citatio nomen=\"transituum\" attributum=\"ad\"\n"
    "    ad=\"status/nomen\" super=\"transitus\" intra=\"machina\"/>\n"
    "  <unicitas nomen=\"statuum\" attributum=\"nomen\"\n"
    "    super=\"status\" intra=\"machina\"/>\n"
    "</canon>\n";

interior constans character* BIBLIOTHECA_SANA =
    "<bibliotheca>\n"
    "  <fons clavis=\"plinius\"/>\n"
    "  <liber fons=\"plinius\" nomen=\"historia\"/>\n"
    "  <machina>\n"
    "    <status nomen=\"vigens\"/>\n"
    "    <transitus ad=\"vigens\"/>\n"
    "  </machina>\n"
    "  <machina>\n"
    "    <status nomen=\"vigens\"/>\n"
    "    <transitus ad=\"vigens\"/>\n"
    "  </machina>\n"
    "</bibliotheca>\n";

interior constans character* BIBLIOTHECA_MALA =
    "<bibliotheca>\n"
    "  <liber fons=\"ignotus\" nomen=\"x\"/>\n"
    "  <machina>\n"
    "    <status nomen=\"unus\"/>\n"
    "    <status nomen=\"unus\"/>\n"
    "    <transitus ad=\"alienus\"/>\n"
    "  </machina>\n"
    "  <machina>\n"
    "    <status nomen=\"alienus\"/>\n"
    "  </machina>\n"
    "</bibliotheca>\n";

interior constans character* CATALOGUS_FIXTURA =
    "# commentarium praetermittendum\n"
    ".genera\tnatura/natura.canon\n"
    "<aedilis>\taedilis.canon\n"
    "<canon>\tcanon.canon\n";

/* ==================================================
 * Auxilia
 * ================================================== */

interior Canon*
canon_ex_literis(
    constans character*   literae,
    Piscina*              piscina,
    InternamentumChorda*  intern)
{
    chorda causa;

    redde canon_legere(chorda_ex_literis(literae, piscina),
                       piscina, intern, &causa);
}

interior Xar*
iudicare_literis(
    Canon*                canon,
    constans character*   literae,
    Piscina*              piscina,
    InternamentumChorda*  intern)
{
    StmlResultus r;

    r = stml_legere(chorda_ex_literis(literae, piscina),
                    piscina, intern);
    si (!r.successus)
    {
        redde NIHIL;
    }

    redde canon_iudicare(canon, r.radix, piscina);
}

interior i32
quot_generis(
    Xar*              vitia,
    CanonVitiumGenus  genus)
{
    i32 summa;
    i32 i;

    summa = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(vitia); i++)
    {
        CanonVitium* v;

        v = (CanonVitium*)xar_obtinere(vitia, i);
        si (v->genus == genus)
        {
            summa++;
        }
    }

    redde summa;
}

/* ==================================================
 * Principale
 * ================================================== */

s32 principale (vacuum)
{
                     b32  praeteritus;
                Piscina*  piscina;
    InternamentumChorda*  intern;
                  Canon*  hortus;

    piscina = piscina_generare_dynamicum("probatio_canon", 262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: lectio canonis
     * ======================================================== */

    {
        Canon* c;
        chorda causa;

        imprimere("\n--- Probans lectionem canonis ---\n");

        c = canon_ex_literis(CANON_HORTUS, piscina, intern);
        CREDO_NON_NIHIL (c);
        CREDO_NON_NIHIL (c->dialectus);
        CREDO_CHORDA_AEQUALIS_LITERIS (*c->dialectus, "hortus");
        CREDO_NON_NIHIL (c->radix);
        CREDO_CHORDA_AEQUALIS_LITERIS (*c->radix, "hortus");
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(c->unicitates), I);
        hortus = c;

        /* fons fractus */
        c = canon_legere(chorda_ex_literis("<<<", piscina),
                         piscina, intern, &causa);
        CREDO_NIHIL (c);
        CREDO_CHORDA_NON_VACUA (causa);

        /* radix aliena */
        c = canon_legere(chorda_ex_literis("<res/>", piscina),
                         piscina, intern, &causa);
        CREDO_NIHIL (c);
        CREDO_CHORDA_NON_VACUA (causa);

        /* ordinarius mendax (praestitutum electionis extra
         * optiones) canonem CLAMANS frangit - documentatio falsa
         * tacita peior est quam canon fractus. Validum in
         * CANON_HORTUS supra iam probatur (color=ruber). */
        c = canon_legere(chorda_ex_literis(
            "<canon dialectus=\"x\" versio=\"1\">"
            "<elementum nomen=\"r\" radix=\"verum\">"
            "<attributum nomen=\"g\" genus=\"electio\""
            " ordinarius=\"viridis\">"
            "<optio>ruber</optio>"
            "</attributum>"
            "</elementum>"
            "</canon>", piscina), piscina, intern, &causa);
        CREDO_NIHIL (c);
        CREDO_CHORDA_NON_VACUA (causa);
    }


    /* ========================================================
     * PROBARE: documentum sanum - nulla vitia
     * ======================================================== */

    {
        Xar* vitia;

        imprimere("\n--- Probans documentum sanum ---\n");

        vitia = iudicare_literis(hortus, HORTUS_SANUS,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: classes vitiorum omnes decem
     * ======================================================== */

    {
        Xar* vitia;

        imprimere("\n--- Probans classes vitiorum ---\n");

        /* elementum ignotum + liberum illicitum (idem nodus bis) */
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"a\"><planta nomen=\"x\"/>"
            "<bestia/></hortus>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), II);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_ELEMENTUM_IGNOTUM), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_LIBERUM_ILLICITUM), I);

        /* attributum ignotum */
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"b\">"
            "<planta nomen=\"x\" cauda=\"verum\"/></hortus>",
            piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_ATTRIBUTUM_IGNOTUM), I);

        /* suggestio: nomen male scriptum 'an X?' accipit
         * (similitudo_optima_decurtata - errores caudae) */
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"s\"><plant nomen=\"x\"/>"
            "<planta nomen=\"y\"/></hortus>", piscina, intern);
        {
            i32 iv;
            b32 inventa;

            inventa = FALSUM;
            per (iv = ZEPHYRUM; iv < xar_numerus(vitia); iv++)
            {
                CanonVitium* v;

                v = (CanonVitium*)xar_obtinere(vitia, iv);
                si (v->genus == CANON_ELEMENTUM_IGNOTUM)
                {
                    inventa = VERUM;
                    CREDO_NON_NIHIL (v->detail);
                    CREDO_CHORDA_CONTINET (*v->detail,
                        chorda_ex_literis("planta", piscina));
                }
            }
            CREDO_VERUM (inventa);
        }

        /* attributum necessarium deest */
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"c\"><planta/></hortus>",
            piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_ATTRIBUTUM_DEEST), I);

        /* valores mali - genera sex uno documento: nomen cum '-'
         * (quod compositum admitteret - discrimen generum!),
         * numerus verbalis, veritas tertia, mensis XIII, electio
         * extra optiones, compositum cum spatio */
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"d\">"
            "<planta nomen=\"mala-forma\" aetas=\"tres\""
            " viva=\"fortasse\" sata=\"2020-13-01\""
            " color=\"viridis\" cultivar=\"mala forma\"/>"
            "</hortus>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), VI);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_MALUS), VI);

        /* liberi pauciores quam minimum */
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"e\"></hortus>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_LIBERI_PAUCI), I);

        /* liberi plures quam maximum */
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"f\"><planta nomen=\"p\"/>"
            "<planta nomen=\"q\"/><planta nomen=\"r\"/></hortus>",
            piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_LIBERI_MULTI), I);

        /* textus TYPATUS malus (vacuus et -42 in fixtura sana
         * iam probantur) */
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"j\"><planta nomen=\"x\">"
            "<mensura>tres</mensura></planta></hortus>",
            piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_TEXTUS_MALUS), I);

        /* textus ubi non licet (spatium album semper licet) */
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"g\">textus vagus"
            "<planta nomen=\"x\"/></hortus>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_TEXTUS_ILLICITUS), I);

        /* nomen bis in spatio unico */
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"h\"><planta nomen=\"rosa\"/>"
            "<planta nomen=\"rosa\"/></hortus>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_NOMEN_BIS), I);

        /* radix aliena (et ignota, ergo vitia duo) */
        vitia = iudicare_literis(hortus,
            "<ager titulus=\"i\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), II);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_RADIX_MALA), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_ELEMENTUM_IGNOTUM), I);
    }


    /* ========================================================
     * PROBARE: intra= - formae contextu dependentes
     * ======================================================== */

    {
        Canon* arca;
        Xar*   vitia;

        imprimere("\n--- Probans intra= (formae binae) ---\n");

        arca = canon_ex_literis(CANON_ARCA, piscina, intern);
        CREDO_NON_NIHIL (arca);

        /* forma recta in contextu utroque */
        vitia = iudicare_literis(arca,
            "<arca><a><cella x=\"1\"/></a>"
            "<b><cella y=\"2\"/></b></arca>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* forma commutata: y in a - ignotum + deest */
        vitia = iudicare_literis(arca,
            "<arca><a><cella y=\"1\"/></a></arca>",
            piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), II);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_ATTRIBUTUM_IGNOTUM), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_ATTRIBUTUM_DEEST), I);
    }


    /* ========================================================
     * PROBARE: canon infixus - contractus, non contentum
     * ======================================================== */

    {
        StmlResultus  r;
        StmlNodus*    infixus;
        Canon*        c_inf;
        Canon*        capsae;
        Xar*          vitia;
        chorda        causa;

        imprimere("\n--- Probans canonem infixum ---\n");

        r = stml_legere(chorda_ex_literis(VIRIDARIUM_INFIXO,
                                          piscina),
                        piscina, intern);
        CREDO_VERUM (r.successus);

        /* inventio: liberum primum elementare */
        infixus = canon_infixum_invenire(r.elementum_radix);
        CREDO_NON_NIHIL (infixus);

        /* absens ubi non est */
        {
            StmlResultus r2;

            r2 = stml_legere(chorda_ex_literis(HORTUS_SANUS,
                                               piscina),
                             piscina, intern);
            CREDO_NIHIL (canon_infixum_invenire(r2.elementum_radix));

            /* positio ipsa regula: <canon> SECUNDUM = contentum */
            r2 = stml_legere(chorda_ex_literis(
                "<v><res/><canon/></v>", piscina), piscina, intern);
            CREDO_NIHIL (canon_infixum_invenire(r2.elementum_radix));
        }

        /* canon_ex_nodo: struere ex subarbore parsata */
        c_inf = canon_ex_nodo(infixus, piscina, intern, &causa);
        CREDO_NON_NIHIL (c_inf);
        CREDO_CHORDA_AEQUALIS_LITERIS (*c_inf->dialectus,
                                       "viridarium");

        /* ex nodo non-canonico: NIHIL et causa */
        CREDO_NIHIL (canon_ex_nodo(r.elementum_radix, piscina,
                                   intern, &causa));
        CREDO_CHORDA_NON_VACUA (causa);

        /* contentum contra infixum sanum: liberum <canon> pro
         * liberis/licentia INVISIBILE esse debet (minimum arboris
         * nihilominus impletur, nullum LIBERUM_ILLICITUM) */
        vitia = canon_iudicare(c_inf, r.radix, piscina);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* contentum peccans contra infixum */
        {
            StmlResultus r3;
            Canon*       c3;

            r3 = stml_legere(chorda_ex_literis(
                VIRIDARIUM_INFIXO_PECCANS, piscina),
                piscina, intern);
            CREDO_VERUM (r3.successus);
            c3 = canon_ex_nodo(
                canon_infixum_invenire(r3.elementum_radix),
                piscina, intern, &causa);
            CREDO_NON_NIHIL (c3);
            vitia = canon_iudicare(c3, r3.radix, piscina);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
            CREDO_AEQUALIS_I32 (quot_generis(vitia,
                CANON_ATTRIBUTUM_DEEST), I);
        }

        /* unicitas: subarbor infixi praetermittenda - definitiones
         * binae 'res' (intra= diversa) NOMEN_BIS non sunt */
        capsae = canon_ex_literis(CANON_CAPSAE, piscina, intern);
        CREDO_NON_NIHIL (capsae);
        vitia = iudicare_literis(capsae, CAPSA_INFIXO,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: citatio et scopi per instantiam
     * ======================================================== */

    {
        Canon* bib_canon;
        Xar*   vitia;

        imprimere("\n--- Probans citationem et scopos ---\n");

        bib_canon = canon_ex_literis(CANON_BIBLIOTHECAE,
                                     piscina, intern);
        CREDO_NON_NIHIL (bib_canon);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(bib_canon->citationes),
                            II);

        /* sanum - ET nomen status trans machinas iteratum LICITUM
         * (scopus per instantiam: postulatio secunda soluta) */
        vitia = iudicare_literis(bib_canon, BIBLIOTHECA_SANA,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* mala: fons ignotus (citatio documenti), transitus
         * statum machinae ALTERIUS citans (citatio intra), status
         * geminus INTRA machinam (unicitas intra) */
        vitia = iudicare_literis(bib_canon, BIBLIOTHECA_MALA,
                                 piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), III);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_CITATIO_IRRITA), II);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_NOMEN_BIS), I);

        /* citatio 'ad' sine solido: canon totus clamans frangitur
         * (citatio muta quae nihil custodit custodire visa peior
         * est quam canon fractus) */
        CREDO_NIHIL (canon_ex_literis(
            "<canon dialectus=\"x\" versio=\"1\">"
            "<elementum nomen=\"r\" radix=\"verum\"/>"
            "<citatio nomen=\"c\" attributum=\"a\""
            " ad=\"sine_solido\"/>"
            "</canon>", piscina, intern));
    }


    /* ========================================================
     * PROBARE: registrum - clavis extensionis et clavis radicis
     * ======================================================== */

    {
        chorda catalogus;
        chorda via;
        chorda radix;

        imprimere("\n--- Probans registrum ---\n");

        catalogus = chorda_ex_literis(CATALOGUS_FIXTURA, piscina);

        via = canon_registrum_quaerere(catalogus,
            "natura/vas.genera", piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS (via, "natura/natura.canon");

        via = canon_registrum_quaerere(catalogus,
            "res.txt", piscina);
        CREDO_CHORDA_VACUA (via);

        radix = chorda_ex_literis("aedilis", piscina);
        via = canon_registrum_quaerere_radice(catalogus, &radix,
                                              piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS (via, "aedilis.canon");

        radix = chorda_ex_literis("ignotus", piscina);
        via = canon_registrum_quaerere_radice(catalogus, &radix,
                                              piscina);
        CREDO_CHORDA_VACUA (via);
    }


    /* ========================================================
     * PROBARE: corpus verum - canon.canon se ipsum iudicat
     * ======================================================== */

    {
        chorda        fons;
        Canon*        canon_ipse;
        StmlResultus  r;
        Xar*          vitia;
        chorda        causa;

        imprimere("\n--- Probans canonem se hospitantem ---\n");

        fons = filum_legere_totum("canon.canon", piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            imprimere("FRACTA: canon.canon legi nequit "
                      "(currendum e radice arboris)\n");
            CREDO_VERUM (fons.mensura > ZEPHYRUM);
        }
        alioquin
        {
            canon_ipse = canon_legere(fons, piscina, intern, &causa);
            CREDO_NON_NIHIL (canon_ipse);

            /* SE IPSUM iudicat */
            r = stml_legere(fons, piscina, intern);
            CREDO_VERUM (r.successus);
            vitia = canon_iudicare(canon_ipse, r.radix, piscina);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

            /* canones ceteros veros iudicat */
            fons = filum_legere_totum("natura/natura.canon",
                                      piscina);
            CREDO_VERUM (fons.mensura > ZEPHYRUM);
            r = stml_legere(fons, piscina, intern);
            CREDO_VERUM (r.successus);
            vitia = canon_iudicare(canon_ipse, r.radix, piscina);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

            fons = filum_legere_totum("aedilis.canon", piscina);
            CREDO_VERUM (fons.mensura > ZEPHYRUM);
            r = stml_legere(fons, piscina, intern);
            CREDO_VERUM (r.successus);
            vitia = canon_iudicare(canon_ipse, r.radix, piscina);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

            fons = filum_legere_totum(
                "silva/grammatica/grammatica.canon", piscina);
            CREDO_VERUM (fons.mensura > ZEPHYRUM);
            r = stml_legere(fons, piscina, intern);
            CREDO_VERUM (r.successus);
            vitia = canon_iudicare(canon_ipse, r.radix, piscina);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

            fons = filum_legere_totum("silva/quaestiones.canon",
                                      piscina);
            CREDO_VERUM (fons.mensura > ZEPHYRUM);
            r = stml_legere(fons, piscina, intern);
            CREDO_VERUM (r.successus);
            vitia = canon_iudicare(canon_ipse, r.radix, piscina);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

            /* culpa plantata vitium parit - porta viva probatur */
            vitia = iudicare_literis(canon_ipse,
                CANON_CULPA_PLANTATA, piscina, intern);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
            CREDO_AEQUALIS_I32 (quot_generis(vitia,
                CANON_ATTRIBUTUM_IGNOTUM), I);

            /* infixus vitiosus contra canonem canonum VERUM:
             * debilitatio contractus proprii CLAMAT (ambo
             * iudicant - via 'schemaLocation' XSD clausa) */
            {
                StmlResultus r4;
                StmlNodus*   inf4;

                r4 = stml_legere(chorda_ex_literis(
                    VIRIDARIUM_INFIXUS_VITIOSUS, piscina),
                    piscina, intern);
                CREDO_VERUM (r4.successus);
                inf4 = canon_infixum_invenire(r4.elementum_radix);
                CREDO_NON_NIHIL (inf4);
                vitia = canon_iudicare(canon_ipse, inf4, piscina);
                CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
                CREDO_AEQUALIS_I32 (quot_generis(vitia,
                    CANON_ATTRIBUTUM_IGNOTUM), I);
            }
        }
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
