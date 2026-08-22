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

/* citatio cum titulis PLURIBUS ante solidum - subsumptio
 * compilata: generator naturae petitum cum posteris omnibus
 * enumerat quia claves per titulum exactum colliguntur. Claves
 * hic ex quercu ET fago colliguntur, ex ave NON. */
interior constans character* CANON_SILVAE =
    "<canon dialectus=\"silvula\" versio=\"1\">\n"
    "  <elementum nomen=\"silvula\" radix=\"verum\">\n"
    "    <liberum nomen=\"quercus\"/>\n"
    "    <liberum nomen=\"fagus\"/>\n"
    "    <liberum nomen=\"avis\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"quercus\">\n"
    "    <attributum nomen=\"nomen\" genus=\"nomen\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"fagus\">\n"
    "    <attributum nomen=\"nomen\" genus=\"nomen\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"avis\">\n"
    "    <attributum nomen=\"nomen\" genus=\"nomen\"/>\n"
    "    <attributum nomen=\"nidificat\" genus=\"nomen\"/>\n"
    "  </elementum>\n"
    "  <citatio nomen=\"nidorum\" attributum=\"nidificat\"\n"
    "    ad=\"quercus fagus/nomen\" super=\"avis\"/>\n"
    "</canon>\n";

/* clavis sub titulo SECUNDO indicis stat - hoc ipsum indicem
 * probat (forma simplici solum quercus clavigera esset) */
interior constans character* SILVULA_SANA =
    "<silvula>\n"
    "  <fagus nomen=\"fagia\"/>\n"
    "  <avis nomen=\"merula\" nidificat=\"fagia\"/>\n"
    "</silvula>\n";

/* valor adest sed sub titulo EXTRA indicem (avis alia): titulus
 * extra indicem clavem non fert, citatio irrita clamare debet */
interior constans character* SILVULA_MALA =
    "<silvula>\n"
    "  <avis nomen=\"merula\"/>\n"
    "  <avis nomen=\"turdus\" nidificat=\"merula\"/>\n"
    "</silvula>\n";

/* signa (spec canon-referentia + decretum 2026-08-10): identitas
 * '&nomen;' poscit, referentia '&nomen;' aut '.genus', nudum in
 * utroque VITIUM - referentia litterale non est. Signum vetus '#'
 * spatio documenti (fragmenta STML) redditum: hic vitium CLAMANS,
 * numquam litterale tacitum (custos migrationis). Terminator ';'
 * obligatorius - prosam tutam facit (AT&T litterale manet). */
interior constans character* CANON_SIGNORUM =
    "<canon dialectus=\"grex\" versio=\"1\">\n"
    "  <elementum nomen=\"grex\" radix=\"verum\">\n"
    "    <liberum nomen=\"ovis\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"ovis\">\n"
    "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
    "    <attributum nomen=\"custos\" genus=\"referentia\"/>\n"
    "  </elementum>\n"
    "</canon>\n";

interior constans character* GREX_SANUS =
    "<grex>\n"
    "  <ovis nomen=\"&agna-prima;\" custos=\".ovis\"/>\n"
    "  <ovis nomen=\"&agna-altera;\" custos=\"&agna-prima;\"/>\n"
    "</grex>\n";

/* septem vitia typorum: nomen nudum, referentia nuda, signum sine
 * corpore (bis - '&;' et '.'), signum identitatis falsum
 * (punctum), SIGNUM VETUS '#' (custos migrationis), terminator
 * ';' deest */
interior constans character* GREX_MALUS =
    "<grex>\n"
    "  <ovis nomen=\"agna-nuda\"/>\n"
    "  <ovis nomen=\"&agna;\" custos=\"ovis-nuda\"/>\n"
    "  <ovis nomen=\"&;\"/>\n"
    "  <ovis nomen=\"&agna-quarta;\" custos=\".\"/>\n"
    "  <ovis nomen=\".agna-puncto\"/>\n"
    "  <ovis nomen=\"#agna-vetus\"/>\n"
    "  <ovis nomen=\"&agna-sine\"/>\n"
    "</grex>\n";

/* dispositio signorum in citatione: '.' vocabularium (sine
 * clavibus), '#' clavis verbatim; forma vetus nuda (natura.canon)
 * clavem verbatim retinet - probatur eo quod fixturae veteres
 * (bibliotheca, silvula) IMMUTATAE virent */
interior constans character* CANON_GREGIS_CITATI =
    "<canon dialectus=\"grex2\" versio=\"1\">\n"
    "  <elementum nomen=\"grex2\" radix=\"verum\">\n"
    "    <liberum nomen=\"ovis\"/>\n"
    "    <liberum nomen=\"canis\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"ovis\">\n"
    "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
    "    <attributum nomen=\"custos\" genus=\"referentia\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"canis\">\n"
    "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
    "  </elementum>\n"
    "  <citatio nomen=\"custodum\" attributum=\"custos\"\n"
    "    ad=\"canis/nomen\" super=\"ovis\"/>\n"
    "</canon>\n";

interior constans character* GREX2_SANUS =
    "<grex2>\n"
    "  <canis nomen=\"&canis-unus;\"/>\n"
    "  <ovis nomen=\"&agna;\" custos=\".canis\"/>\n"
    "  <ovis nomen=\"&agnella;\" custos=\"&canis-unus;\"/>\n"
    "</grex2>\n";

/* '.ovis' extra vocabularium (index = canis solum);
 * '&nemo;' clavem non habet */
interior constans character* GREX2_MALUS =
    "<grex2>\n"
    "  <ovis nomen=\"&agna;\" custos=\".ovis\"/>\n"
    "  <ovis nomen=\"&agnella;\" custos=\"&nemo;\"/>\n"
    "</grex2>\n";

interior constans character* CATALOGUS_FIXTURA =
    "# commentarium praetermittendum\n"
    ".genera\tnatura/natura.canon\n"
    "<aedilis>\taedilis.canon\n"
    "<canon>\tcanon.canon\n";


/* ==================================================
 * Auxilia
 * ================================================== */

interior Canon*
canon_ex_literis (
     constans character* literae,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    chorda causa;

    redde canon_legere(chorda_ex_literis(literae, piscina),
                       piscina, intern, &causa);
}

interior Xar*
iudicare_literis (
                  Canon* canon,
     constans character* literae,
                Piscina* piscina,
    InternamentumChorda* intern)
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
quot_generis (
                 Xar* vitia,
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
 * Fixturae - fines (spec fines): constrictiones valorum
 * structuratae - fractio (punctum decimale), minimum/maximum
 * INCLUSIVA. Forma mala = VALOR_MALUS; forma sana extra fines =
 * VALOR_EXTRA_FINES. Canones mendaces (ordinarius extra, fines
 * inversi, fines in genere non numerico) in ORTU reiciuntur.
 * ================================================== */

interior constans character* CANON_FINIUM =
    "<canon dialectus=\"probatio\" versio=\"1\">\n"
    "  <elementum nomen=\"radix\" radix=\"verum\">\n"
    "    <attributum nomen=\"gradus\" genus=\"numerus\"\n"
    "      fractio=\"verum\" minimum=\"0\" maximum=\"1\"/>\n"
    "    <attributum nomen=\"aetas\" genus=\"numerus\"/>\n"
    "    <attributum nomen=\"calor\" genus=\"numerus\"\n"
    "      fractio=\"verum\" minimum=\"-40\" maximum=\"0.5\"/>\n"
    "  </elementum>\n"
    "</canon>\n";

interior constans character* CANON_ORDINARIUS_EXTRA =
    "<canon dialectus=\"probatio\" versio=\"1\">\n"
    "  <elementum nomen=\"radix\" radix=\"verum\">\n"
    "    <attributum nomen=\"gradus\" genus=\"numerus\"\n"
    "      minimum=\"0\" maximum=\"1\" ordinarius=\"2\"/>\n"
    "  </elementum>\n"
    "</canon>\n";

interior constans character* CANON_FINES_INVERSI =
    "<canon dialectus=\"probatio\" versio=\"1\">\n"
    "  <elementum nomen=\"radix\" radix=\"verum\">\n"
    "    <attributum nomen=\"gradus\" genus=\"numerus\"\n"
    "      minimum=\"2\" maximum=\"1\"/>\n"
    "  </elementum>\n"
    "</canon>\n";

interior constans character* CANON_FINES_TEXTUS =
    "<canon dialectus=\"probatio\" versio=\"1\">\n"
    "  <elementum nomen=\"radix\" radix=\"verum\">\n"
    "    <attributum nomen=\"titulus\" genus=\"textus\"\n"
    "      minimum=\"0\"/>\n"
    "  </elementum>\n"
    "</canon>\n";


/* ==================================================
 * Fixturae - super adstrictum (spec super-adstrictum): liberi
 * multiplicati nomen trans parentes communicant, clausurae
 * differunt - 'parens/titulus' in super= parentem nominat.
 * Fixtura: auctor sub libro homines citat, sub nave portus.
 * ================================================== */

interior constans character* CANON_SUPER_ADSTRICTI =
    "<canon dialectus=\"probatio\" versio=\"1\">\n"
    "  <elementum nomen=\"radix\" radix=\"verum\">\n"
    "    <liberum nomen=\"homo\"/>\n"
    "    <liberum nomen=\"portus\"/>\n"
    "    <liberum nomen=\"liber\"/>\n"
    "    <liberum nomen=\"navis\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"homo\">\n"
    "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"portus\">\n"
    "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"liber\">\n"
    "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
    "    <liberum nomen=\"auctor\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"navis\">\n"
    "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
    "    <liberum nomen=\"auctor\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"auctor\" intra=\"liber\">\n"
    "    <attributum nomen=\"ad\" genus=\"referentia\"/>\n"
    "  </elementum>\n"
    "  <elementum nomen=\"auctor\" intra=\"navis\">\n"
    "    <attributum nomen=\"ad\" genus=\"referentia\"/>\n"
    "  </elementum>\n"
    "  <citatio nomen=\"auctores-librorum\" attributum=\"ad\"\n"
    "    super=\"liber/auctor\" ad=\"homo/nomen\"/>\n"
    "  <citatio nomen=\"auctores-navium\" attributum=\"ad\"\n"
    "    super=\"navis/auctor\" ad=\"portus/nomen\"/>\n"
    "</canon>\n";

interior constans character* SUPER_SANUM =
    "<radix>\n"
    "  <homo nomen=\"&plinius;\"/>\n"
    "  <portus nomen=\"&ostia;\"/>\n"
    "  <liber nomen=\"&historia-naturalis;\">\n"
    "    <auctor ad=\"&plinius;\"/>\n"
    "  </liber>\n"
    "  <navis nomen=\"&classis-prima;\">\n"
    "    <auctor ad=\"&ostia;\"/>\n"
    "  </navis>\n"
    "</radix>\n";

/* EADEM structura, clausurae PERMUTATAE: auctor libri portum
 * citat, auctor navis hominem - utraque citatio suam viam solam
 * videt, ergo vitium BIS */
interior constans character* SUPER_PERMUTATUM =
    "<radix>\n"
    "  <homo nomen=\"&plinius;\"/>\n"
    "  <portus nomen=\"&ostia;\"/>\n"
    "  <liber nomen=\"&historia-naturalis;\">\n"
    "    <auctor ad=\"&ostia;\"/>\n"
    "  </liber>\n"
    "  <navis nomen=\"&classis-prima;\">\n"
    "    <auctor ad=\"&plinius;\"/>\n"
    "  </navis>\n"
    "</radix>\n";


/* ==================================================
 * Fixturae - glossae (spec glossae): documentatio vernacula.
 * Forma sola canonis est (lingua necessaria per machinam genericam
 * necessarium=verum); copertura lintri (natura_glossae) est.
 * ================================================== */

interior constans character* GLOSSA_SUB_ELEMENTO =
    "<canon dialectus=\"probatio\" versio=\"1\">\n"
    "  <elementum nomen=\"radix\" radix=\"verum\">\n"
    "    <glossa lingua=\"en\">A test root element.</glossa>\n"
    "    <glossa lingua=\"fr\">Un element racine d'essai.</glossa>\n"
    "    <attributum nomen=\"nomen\" genus=\"nomen\"/>\n"
    "  </elementum>\n"
    "</canon>\n";

interior constans character* GLOSSA_SUB_GENERE =
    "<natura modulus=\"probatio\" versio=\"1\" lingua=\"latina\">\n"
    "  <.genus nomen=\"probandum\">\n"
    "    <definitio>Res probationis.</definitio>\n"
    "    <glossa lingua=\"en\">A thing under test.</glossa>\n"
    "    <glossa lingua=\"fr\">Une chose a l'essai.</glossa>\n"
    "  </.genus>\n"
    "</natura>\n";

interior constans character* GLOSSA_SINE_LINGUA =
    "<natura modulus=\"probatio\" versio=\"1\" lingua=\"latina\">\n"
    "  <.genus nomen=\"probandum\">\n"
    "    <glossa>Sine lingua.</glossa>\n"
    "  </.genus>\n"
    "</natura>\n";

/* fines in proprietate (spec fines): grammatica generum eadem
 * tria attributa fert quae canon - catena .genera -> canon coctus */
interior constans character* PROPRIETAS_CUM_FINIBUS =
    "<natura modulus=\"probatio\" versio=\"1\" lingua=\"latina\">\n"
    "  <.genus nomen=\"probandum\">\n"
    "    <definitio>Res probationis.</definitio>\n"
    "    <proprietates>\n"
    "      <proprietas nomen=\"probabilitas\" genus=\"numerus\"\n"
    "        fractio=\"verum\" minimum=\"0\" maximum=\"1\"/>\n"
    "    </proprietates>\n"
    "  </.genus>\n"
    "</natura>\n";


/* ==================================================
 * Principale
 * ================================================== */

s32
principale (vacuum)
{
                     b32  praeteritus;
                 Piscina* piscina;
     InternamentumChorda* intern;
                   Canon* hortus;

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
        chorda  causa;

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
     * PROBARE: regula quae attributum IGNOTUM nominat
     *
     * VITIUM IV capitis canon.canon in facie altera: auctores
     * casum ABSENTIS ('attributum=' deest) clauserunt, casum
     * FALSI NOMINIS non. Attributum ignotum nominare portam
     * generat quae nihil custodit et se custodire simulat -
     * omnis inquisitio nihil reddit, ergo duplicata nulla
     * inveniuntur, ergo regula 'successum' nuntiat.
     *
     * MENSURATUM ante emendationem: unicitas cum
     * attributum="a-b" (compositum LEGALE, attributum tamen
     * nullum) documentum cum pari duplicato VITIIS 0 transire
     * sinebat, et canon.canon ipse nihil dicebat.
     *
     * TRIA nomina probanda sunt, non unum: citatio DUO latera
     * fert (attributum= citans, ad= clavis).
     * ======================================================== */

    {
         Canon* c;
        chorda  causa;

        imprimere("\n--- Probans regulas attributa ignota "
                  "nominantes ---\n");

        /* --- culpae plantatae: quaeque recusari DEBET --- */

        /* I. unicitas/attributum ignotum */
        c = canon_legere(chorda_ex_literis(
            "<canon dialectus=\"x\" versio=\"1\">"
            "<elementum nomen=\"r\" radix=\"verum\">"
            "<liberum nomen=\"item\"/></elementum>"
            "<elementum nomen=\"item\">"
            "<attributum nomen=\"a\" genus=\"nomen\"/></elementum>"
            "<unicitas nomen=\"u\" attributum=\"non-tale\""
            " super=\"item\"/>"
            "</canon>", piscina), piscina, intern, &causa);
        CREDO_NIHIL (c);
        CREDO_CHORDA_NON_VACUA (causa);

        /* II. citatio/attributum (latus citans) ignotum */
        c = canon_legere(chorda_ex_literis(
            "<canon dialectus=\"x\" versio=\"1\">"
            "<elementum nomen=\"r\" radix=\"verum\">"
            "<liberum nomen=\"item\"/></elementum>"
            "<elementum nomen=\"item\">"
            "<attributum nomen=\"a\" genus=\"nomen\"/></elementum>"
            "<citatio nomen=\"c\" attributum=\"non-tale\""
            " super=\"item\" ad=\"item/a\"/>"
            "</canon>", piscina), piscina, intern, &causa);
        CREDO_NIHIL (c);
        CREDO_CHORDA_NON_VACUA (causa);

        /* III. citatio/ad (latus clavis) ignotum - hoc latus
         * prototypum primum OMISIT */
        c = canon_legere(chorda_ex_literis(
            "<canon dialectus=\"x\" versio=\"1\">"
            "<elementum nomen=\"r\" radix=\"verum\">"
            "<liberum nomen=\"item\"/></elementum>"
            "<elementum nomen=\"item\">"
            "<attributum nomen=\"a\" genus=\"nomen\"/></elementum>"
            "<citatio nomen=\"c\" attributum=\"a\""
            " super=\"item\" ad=\"item/non-tale\"/>"
            "</canon>", piscina), piscina, intern, &causa);
        CREDO_NIHIL (c);
        CREDO_CHORDA_NON_VACUA (causa);

        /* IV. STELLA non est exemptio universalis: 'quodlibet
         * elementum' exsistentiam adhuc poscit. Sine hac proba
         * emendatio stellae portam alteram mutam pareret. */
        c = canon_legere(chorda_ex_literis(
            "<canon dialectus=\"x\" versio=\"1\">"
            "<elementum nomen=\"r\" radix=\"verum\">"
            "<liberum nomen=\"item\"/></elementum>"
            "<elementum nomen=\"item\">"
            "<attributum nomen=\"a\" genus=\"nomen\"/></elementum>"
            "<citatio nomen=\"c\" attributum=\"a\""
            " ad=\"*/non-tale\"/>"
            "</canon>", piscina), piscina, intern, &causa);
        CREDO_NIHIL (c);
        CREDO_CHORDA_NON_VACUA (causa);

        /* --- CUSTODES: quaeque legi DEBET.
         * Sine his quattuor probae supra nihil probant: canon
         * omnis recusatus easdem lineas viridis faceret. --- */

        /* I'. unicitas cum attributo vero */
        c = canon_legere(chorda_ex_literis(
            "<canon dialectus=\"x\" versio=\"1\">"
            "<elementum nomen=\"r\" radix=\"verum\">"
            "<liberum nomen=\"item\"/></elementum>"
            "<elementum nomen=\"item\">"
            "<attributum nomen=\"a\" genus=\"nomen\"/></elementum>"
            "<unicitas nomen=\"u\" attributum=\"a\""
            " super=\"item\"/>"
            "</canon>", piscina), piscina, intern, &causa);
        CREDO_NON_NIHIL (c);

        /* II'. citatio utroque latere vero */
        c = canon_legere(chorda_ex_literis(
            "<canon dialectus=\"x\" versio=\"1\">"
            "<elementum nomen=\"r\" radix=\"verum\">"
            "<liberum nomen=\"item\"/></elementum>"
            "<elementum nomen=\"item\">"
            "<attributum nomen=\"a\" genus=\"nomen\"/>"
            "<attributum nomen=\"b\" genus=\"nomen\"/></elementum>"
            "<citatio nomen=\"c\" attributum=\"a\""
            " super=\"item\" ad=\"item/b\"/>"
            "</canon>", piscina), piscina, intern, &causa);
        CREDO_NON_NIHIL (c);

        /* III'. STELLA cum attributo vero - forma quam
         * natura/cocta/individua.canon CDLXX vicibus fert
         * (ad stellam, deinde solidus, deinde 'nomen'). Regula
         * quae stellam ut titulum elementi legit canonem
         * generatum maximum totius repositorii recusaret. */
        c = canon_legere(chorda_ex_literis(
            "<canon dialectus=\"x\" versio=\"1\">"
            "<elementum nomen=\"r\" radix=\"verum\">"
            "<liberum nomen=\"item\"/></elementum>"
            "<elementum nomen=\"item\">"
            "<attributum nomen=\"a\" genus=\"nomen\"/></elementum>"
            "<citatio nomen=\"c\" attributum=\"a\""
            " ad=\"*/a\"/>"
            "</canon>", piscina), piscina, intern, &causa);
        CREDO_NON_NIHIL (c);

        /* IV'. super= ABSENS in citatione = elementum quodlibet
         * (mos vetus) - idem quod stella dicit */
        c = canon_legere(chorda_ex_literis(
            "<canon dialectus=\"x\" versio=\"1\">"
            "<elementum nomen=\"r\" radix=\"verum\">"
            "<liberum nomen=\"item\"/></elementum>"
            "<elementum nomen=\"item\">"
            "<attributum nomen=\"a\" genus=\"nomen\"/></elementum>"
            "<citatio nomen=\"c\" attributum=\"a\""
            " ad=\"item/a\"/>"
            "</canon>", piscina), piscina, intern, &causa);
        CREDO_NON_NIHIL (c);
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
     * PROBARE: classes vitiorum primigenae (X priores; recentiores
     * - TEXTUS_MALUS, CITATIO_IRRITA, VOCABULUM_IGNOTUM,
     * VALOR_EXTRA_FINES, AUGMENTUM_PUGNANS - sectionibus suis
     * probantur)
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
          Xar* vitia;

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
           StmlNodus* infixus;
               Canon* c_inf;
               Canon* capsae;
                 Xar* vitia;
              chorda  causa;

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
            StmlResultus  r3;
                   Canon* c3;

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
          Xar* vitia;

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
     * PROBARE: citatio cum titulis pluribus (index ad=)
     *
     * PAR obligatorium: sana clavem sub titulo SECUNDO fert
     * (indicem probat - forma simplici rueret), mala valorem
     * sub titulo extra indicem (clamorem probat - canon sine
     * citatione utramque acciperet).
     * ======================================================== */

    {
        Canon* silvula;
          Xar* vitia;

        imprimere("\n--- Probans citationem indicis ---\n");

        silvula = canon_ex_literis(CANON_SILVAE, piscina, intern);
        CREDO_NON_NIHIL (silvula);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(silvula->citationes),
                            I);

        vitia = iudicare_literis(silvula, SILVULA_SANA,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        vitia = iudicare_literis(silvula, SILVULA_MALA,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_CITATIO_IRRITA), I);

        /* album solum ante solidum CLAMAT, non tacet - citatio
         * sine clavigero (ad=' /n') porta muta esset */
        CREDO_NIHIL (canon_ex_literis(
            "<canon dialectus=\"x\" versio=\"1\">"
            "<elementum nomen=\"r\" radix=\"verum\"/>"
            "<citatio nomen=\"c\" attributum=\"a\""
            " ad=\" /n\"/>"
            "</canon>", piscina, intern));
    }


    /* ========================================================
     * PROBARE: claves externae (librarium W1, 2026-08-10)
     *
     * Scopus citationum = claves documenti + claves bibliothecae
     * in canone INFIXAE. Culpa plantata viva: ante W1 citatio
     * '&quercus_prima;' sine clave locali 'clavis absens' erat.
     * ======================================================== */

    {
        Canon* c;
          Xar* vitia;

        imprimere("\n--- Probans claves externas ---\n");

        c = canon_ex_literis(
            "<canon dialectus=\"nemus\" versio=\"1\">\n"
            "  <elementum nomen=\"silva\" radix=\"verum\">\n"
            "    <liberum nomen=\"avis\"/>\n"
            "    <liberum nomen=\"quercus\"/>\n"
            "  </elementum>\n"
            "  <elementum nomen=\"quercus\">\n"
            "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
            "  </elementum>\n"
            "  <elementum nomen=\"avis\">\n"
            "    <attributum nomen=\"nidificat\""
            " genus=\"referentia\"/>\n"
            "  </elementum>\n"
            "  <citatio nomen=\"nidorum\" attributum=\"nidificat\"\n"
            "    ad=\"quercus/nomen\" super=\"avis\"/>\n"
            "  <claves-externae fons=\"probatum\">\n"
            "    <clavis genus=\"quercus\">&quercus_prima;</clavis>\n"
            "  </claves-externae>\n"
            "</canon>\n", piscina, intern);
        CREDO_NON_NIHIL (c);
        CREDO_NON_NIHIL (c->claves_externae);

        /* citatio per clavem externam SOLVIT sine clave locali */
        vitia = iudicare_literis(c,
            "<silva><avis nidificat=\"&quercus_prima;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* clavis ignota vitium MANET (pinna negativa) */
        vitia = iudicare_literis(c,
            "<silva><avis nidificat=\"&nemo;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_CITATIO_IRRITA), I);

        /* clavis localis et externa simul - utraque solvit */
        vitia = iudicare_literis(c,
            "<silva><quercus nomen=\"&quercus_domi;\"/>"
            "<avis nidificat=\"&quercus_domi;\"/>"
            "<avis nidificat=\"&quercus_prima;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: augmentatio <% &clavis;> (librarium W3, 2026-08-10)
     *
     * Contentum additivum individuo bibliothecae appositum.
     * Additivum MECHANICE = fissio LIBERUM/ATTRIBUTUM quam
     * generator iam computat: liberum sine maximo = multiplex =
     * augmentabile; attributum aut liberum maximo adstrictum =
     * singulare = pugna (bibliothecam ipsam muta). Membrum
     * ignotum = vitium ordinarium. Liberi legales pleno more
     * recursive iudicantur (modellum contenti eorum vivit).
     * ======================================================== */

    {
        Canon* c;
          Xar* vitia;

        imprimere("\n--- Probans augmentationem ---\n");

        c = canon_ex_literis(
            "<canon dialectus=\"nemus\" versio=\"1\">\n"
            "  <elementum nomen=\"silva\" radix=\"verum\">\n"
            "    <liberum nomen=\"quercus\"/>\n"
            "  </elementum>\n"
            "  <elementum nomen=\"quercus\">\n"
            "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
            "    <attributum nomen=\"aetas\" genus=\"numerus\"/>\n"
            "    <liberum nomen=\"ramus\"/>\n"
            "    <liberum nomen=\"folium\"/>\n"
            "    <liberum nomen=\"historia\" maximum=\"1\"/>\n"
            "    <liberum nomen=\"nota\"/>\n"
            "  </elementum>\n"
            "  <elementum nomen=\"ramus\">\n"
            "    <attributum nomen=\"longitudo\""
            " genus=\"numerus\"/>\n"
            "  </elementum>\n"
            "  <elementum nomen=\"folium\" intra=\"quercus\">\n"
            "    <attributum nomen=\"color\" genus=\"nomen\"/>\n"
            "  </elementum>\n"
            "  <elementum nomen=\"nota\" textus=\"verum\"/>\n"
            "  <elementum nomen=\"historia\" textus=\"verum\"/>\n"
            "  <claves-externae fons=\"probatum\">\n"
            "    <clavis genus=\"quercus\">&quercus_prima;</clavis>\n"
            "  </claves-externae>\n"
            "</canon>\n", piscina, intern);
        CREDO_NON_NIHIL (c);

        /* legalis: clavis externa, membra multiplicia + nota;
         * augmentatio vocabulario parentis INVISIBILIS (silva
         * liberum '%' non fert - nullum vitium tamen) */
        vitia = iudicare_literis(c,
            "<silva><% &quercus_prima;>"
            "<ramus longitudo=\"3\"/><nota>bene</nota>"
            "</%></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* membrum intra= adstrictum: parens semanticus = genus
         * destinatum, non '%' (vitium corpore VERO inventum -
         * 'adhibet' intra='instrumentum-aedificationis' in censu
         * ELEMENTUM_IGNOTUM falso clamabat) */
        vitia = iudicare_literis(c,
            "<silva><% &quercus_prima;>"
            "<folium color=\"viridis\"/>"
            "</%></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* recursio viva: modellum liberi legalis iudicatur */
        vitia = iudicare_literis(c,
            "<silva><% &quercus_prima;>"
            "<ramus longitudo=\"non_numerus\"/>"
            "</%></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_MALUS), I);

        /* clavis ignota: augmentatio inexsistentis */
        vitia = iudicare_literis(c,
            "<silva><% &nemo_est;><nota>x</nota></%></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_CITATIO_IRRITA), I);

        /* pugna: attributum (singulare per naturam) */
        vitia = iudicare_literis(c,
            "<silva><% &quercus_prima;>"
            "<aetas>5</aetas></%></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_AUGMENTUM_PUGNANS), I);

        /* pugna: liberum maximo adstrictum (historia maximum=1 -
         * alterum per augmentationem canonem basis frangeret) */
        vitia = iudicare_literis(c,
            "<silva><% &quercus_prima;>"
            "<historia>olim</historia></%></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_AUGMENTUM_PUGNANS), I);

        /* membrum ignotum: vitium ordinarium */
        vitia = iudicare_literis(c,
            "<silva><% &quercus_prima;><rota/></%></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_LIBERUM_ILLICITUM), I);

        /* textus nudus in augmentatione */
        vitia = iudicare_literis(c,
            "<silva><% &quercus_prima;>textus nudus</%></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_TEXTUS_ILLICITUS), I);

        /* clavis domi cusa: augmentatio ANTE cusionem in ordine
         * documenti - passus cusas prius totas colligit */
        vitia = iudicare_literis(c,
            "<silva>"
            "<% &quercus_domi;><ramus longitudo=\"4\"/></%>"
            "<quercus nomen=\"&quercus_domi;\"/>"
            "</silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: collisio clavium (librarium W2, 2026-08-10)
     *
     * Documentum clavem identitatis CUDENS quae in clavibus
     * externis iam vivit ambiguitatem committit - suppositum
     * aequivocum esse nequit. EXEMPTIO seminum: documentum cuius
     * radix fons= fontem clavium externarum aequat IPSA bibliotheca
     * est (claves eius claves externae SUNT) - collisio secum
     * praetermittitur. Fons alienus exemptionem NON dat.
     * ======================================================== */

    {
        Canon* c;
          Xar* vitia;

        imprimere("\n--- Probans collisionem clavium ---\n");

        c = canon_ex_literis(
            "<canon dialectus=\"nemus\" versio=\"1\">\n"
            "  <elementum nomen=\"silva\" radix=\"verum\">\n"
            "    <attributum nomen=\"fons\" genus=\"nomen\"/>\n"
            "    <liberum nomen=\"quercus\"/>\n"
            "  </elementum>\n"
            "  <elementum nomen=\"quercus\">\n"
            "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
            "  </elementum>\n"
            "  <claves-externae fons=\"probatum\">\n"
            "    <clavis genus=\"quercus\">&quercus_prima;</clavis>\n"
            "  </claves-externae>\n"
            "</canon>\n", piscina, intern);
        CREDO_NON_NIHIL (c);

        /* cusio clavis bibliothecae = collisio */
        vitia = iudicare_literis(c,
            "<silva><quercus nomen=\"&quercus_prima;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_CLAVIS_COLLISA), I);

        /* cusio clavis novae libera manet */
        vitia = iudicare_literis(c,
            "<silva><quercus nomen=\"&quercus_domi;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* exemptio: radix fons= fontem clavium aequat - documentum
         * IPSA bibliotheca est, claves eius claves externae SUNT */
        vitia = iudicare_literis(c,
            "<silva fons=\"probatum\">"
            "<quercus nomen=\"&quercus_prima;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* fons alienus exemptionem NON dat */
        vitia = iudicare_literis(c,
            "<silva fons=\"alienum\">"
            "<quercus nomen=\"&quercus_prima;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_CLAVIS_COLLISA), I);
    }


    /* ========================================================
     * PROBARE: genus clavis externae + citatio stellata
     * (librarium, 2026-08-10)
     *
     * Via domestica genus per collectionem clavigerorum iam
     * custodit (claves solum ex elementis indicis ad=
     * colliguntur); via externa solam exsistentiam probabat -
     * clavis generis alieni tacite solvebat. Nunc genus= clavis
     * contra indicem ad= comparatur (ALIENA XVII). Citatio
     * STELLATA (stella ante solidum) exsistentiam solam poscit:
     * claviger
     * domesticus = elementum quodvis cuius definitio attributum
     * identitatis eiusdem nominis fert; externa genere quovis.
     * Ante hoc ad="*" NIHIL emittebat (artefactum aetatis ante
     * claves externas: spatium clavium computari non poterat).
     * ======================================================== */

    {
        Canon* c;
          Xar* vitia;

        imprimere("\n--- Probans genus externum et stellam ---\n");

        c = canon_ex_literis(
            "<canon dialectus=\"nemus\" versio=\"1\">\n"
            "  <elementum nomen=\"silva\" radix=\"verum\">\n"
            "    <liberum nomen=\"avis\"/>\n"
            "    <liberum nomen=\"quercus\"/>\n"
            "  </elementum>\n"
            "  <elementum nomen=\"quercus\">\n"
            "    <attributum nomen=\"nomen\" genus=\"identitas\"/>\n"
            "  </elementum>\n"
            "  <elementum nomen=\"avis\">\n"
            "    <attributum nomen=\"nidificat\""
            " genus=\"referentia\"/>\n"
            "    <attributum nomen=\"spectat\""
            " genus=\"referentia\"/>\n"
            "  </elementum>\n"
            "  <citatio nomen=\"nidorum\" attributum=\"nidificat\"\n"
            "    ad=\"quercus/nomen\" super=\"avis\"/>\n"
            "  <citatio nomen=\"spectaculorum\""
            " attributum=\"spectat\"\n"
            "    ad=\"*/nomen\" super=\"avis\"/>\n"
            "  <claves-externae fons=\"probatum\">\n"
            "    <clavis genus=\"quercus\">&quercus_prima;</clavis>\n"
            "    <clavis genus=\"avis\">&avis_peregrina;</clavis>\n"
            "  </claves-externae>\n"
            "</canon>\n", piscina, intern);
        CREDO_NON_NIHIL (c);

        /* genus rectum externum solvit (mos W1 servatus) */
        vitia = iudicare_literis(c,
            "<silva><avis nidificat=\"&quercus_prima;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* genus ALIENUM: clavis exsistit, genus non congruit -
         * avis ubi quercus petita */
        vitia = iudicare_literis(c,
            "<silva><avis nidificat=\"&avis_peregrina;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_CITATIO_ALIENA), I);

        /* stella + externa generis cuiusvis: solvit */
        vitia = iudicare_literis(c,
            "<silva><avis spectat=\"&avis_peregrina;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* stella + cusa domestica: claviger stellatus elementum
         * est cuius definitio attributum identitatis fert */
        vitia = iudicare_literis(c,
            "<silva><quercus nomen=\"&quercus_domi;\"/>"
            "<avis spectat=\"&quercus_domi;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* stella exsistentiam tamen POSCIT - clavis absens clamat */
        vitia = iudicare_literis(c,
            "<silva><avis spectat=\"&nemo_omnino;\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_CITATIO_IRRITA), I);

        /* stella + signum generis ('.'): genus quodvis NOTUM -
         * vocabularium elementorum totum, non index citationis
         * (corpore VERO inventum: semina geritur_a='.processus') */
        vitia = iudicare_literis(c,
            "<silva><avis spectat=\".quercus\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
        vitia = iudicare_literis(c,
            "<silva><avis spectat=\".draco\"/></silva>",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VOCABULUM_IGNOTUM), I);
    }


    /* ========================================================
     * PROBARE: genera valorum identitas/referentia (signa)
     * ======================================================== */

    {
        Canon* grex;
          Xar* vitia;

        imprimere("\n--- Probans signa valorum ---\n");

        grex = canon_ex_literis(CANON_SIGNORUM, piscina, intern);
        CREDO_NON_NIHIL (grex);

        vitia = iudicare_literis(grex, GREX_SANUS, piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        vitia = iudicare_literis(grex, GREX_MALUS, piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), VII);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_MALUS), VII);
    }


    /* ========================================================
     * PROBARE: dispositio signorum in citatione
     * ======================================================== */

    {
        Canon* grex2;
          Xar* vitia;

        imprimere("\n--- Probans dispositionem signorum ---\n");

        grex2 = canon_ex_literis(CANON_GREGIS_CITATI,
                                 piscina, intern);
        CREDO_NON_NIHIL (grex2);

        vitia = iudicare_literis(grex2, GREX2_SANUS,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        vitia = iudicare_literis(grex2, GREX2_MALUS,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), II);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VOCABULUM_IGNOTUM), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_CITATIO_IRRITA), I);
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
              chorda  fons;
               Canon* canon_ipse;
        StmlResultus  r;
                 Xar* vitia;
              chorda  causa;

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
                StmlResultus  r4;
                   StmlNodus* inf4;

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


    /* ========================================================
     * PROBARE: super adstrictum (spec super-adstrictum)
     * ======================================================== */

    {
        Canon* adstrictus;
          Xar* vitia;

        imprimere("\n--- Probans super adstrictum ---\n");

        adstrictus = canon_ex_literis(CANON_SUPER_ADSTRICTI,
                                      piscina, intern);
        CREDO_NON_NIHIL (adstrictus);

        /* clausurae rectae per viam: nulla vitia */
        vitia = iudicare_literis(adstrictus, SUPER_SANUM,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* clausurae permutatae: utraque via suam clausuram solam
         * videt - CITATIO_IRRITA bis. Ante mechanismum entries
         * cum solido nihil congruebant et NIHIL iudicabatur -
         * foramen mensuratum descriptoris. */
        vitia = iudicare_literis(adstrictus, SUPER_PERMUTATUM,
                                 piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), II);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_CITATIO_IRRITA), II);
    }


    /* ========================================================
     * PROBARE: fines valorum (spec fines)
     * ======================================================== */

    {
        Canon* finium;
          Xar* vitia;

        imprimere("\n--- Probans fines valorum ---\n");

        finium = canon_ex_literis(CANON_FINIUM, piscina, intern);
        CREDO_NON_NIHIL (finium);

        /* fractio: forma decimalis intra fines */
        vitia = iudicare_literis(finium,
            "<radix gradus=\"0.7\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* sine fractio punctum reicitur (forma, non fines) */
        vitia = iudicare_literis(finium,
            "<radix aetas=\"0.5\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_MALUS), I);

        /* extra fines - vitium proprium, non forma */
        vitia = iudicare_literis(finium,
            "<radix gradus=\"1.5\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_EXTRA_FINES), I);
        vitia = iudicare_literis(finium,
            "<radix gradus=\"-1\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_EXTRA_FINES), I);

        /* fines INCLUSIVI */
        vitia = iudicare_literis(finium,
            "<radix gradus=\"0\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
        vitia = iudicare_literis(finium,
            "<radix gradus=\"1\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* zephyra caudalia: 0.50 == 0.5 (finis maximus caloris) */
        vitia = iudicare_literis(finium,
            "<radix calor=\"0.50\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* finis minimus negativus */
        vitia = iudicare_literis(finium,
            "<radix calor=\"-39.5\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
        vitia = iudicare_literis(finium,
            "<radix calor=\"-40.5\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_EXTRA_FINES), I);

        /* forma prava: punctum sine digito utrimque */
        vitia = iudicare_literis(finium,
            "<radix gradus=\".5\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_MALUS), I);
        vitia = iudicare_literis(finium,
            "<radix gradus=\"5.\"/>", piscina, intern);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_MALUS), I);

        /* ----------------------------------------------------
         * dies ante Christum natum (decretum Franis 2026-08-08,
         * res 01KZC7F388): '-' praefixum lectione HISTORICA -
         * '-312' = annus 312 a.C.n., annus zephyrus NON est.
         * Annus solus digitis I..IV sine suppletione; formae
         * cum mense quattuor digitos servant.
         * ---------------------------------------------------- */
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"b\">"
            "<planta nomen=\"rosa\" sata=\"-312\"/></hortus>",
            piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"b\">"
            "<planta nomen=\"rosa\" sata=\"-4\"/></hortus>",
            piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"b\">"
            "<planta nomen=\"rosa\" sata=\"-0312-04-01\"/></hortus>",
            piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"b\">"
            "<planta nomen=\"rosa\" sata=\"--312\"/></hortus>",
            piscina, intern);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_MALUS), I);
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"b\">"
            "<planta nomen=\"rosa\" sata=\"-\"/></hortus>",
            piscina, intern);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_MALUS), I);
        vitia = iudicare_literis(hortus,
            "<hortus titulus=\"b\">"
            "<planta nomen=\"rosa\" sata=\"-31234\"/></hortus>",
            piscina, intern);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_MALUS), I);

        /* canones mendaces in ortu reiciuntur */
        CREDO_NIHIL (canon_ex_literis(CANON_ORDINARIUS_EXTRA,
                                      piscina, intern));
        CREDO_NIHIL (canon_ex_literis(CANON_FINES_INVERSI,
                                      piscina, intern));
        CREDO_NIHIL (canon_ex_literis(CANON_FINES_TEXTUS,
                                      piscina, intern));
    }


    /* ========================================================
     * PROBARE: glossae in grammaticis (spec glossae)
     * ======================================================== */

    {
         Canon* canon_canonum;
         Canon* canon_naturae;
        chorda  fons;
        chorda  causa;
           Xar* vitia;

        imprimere("\n--- Probans glossas in grammaticis ---\n");

        fons = filum_legere_totum("canon.canon", piscina);
        CREDO_MAIOR_I32 (fons.mensura, (i32)ZEPHYRUM);
        canon_canonum = canon_legere(fons, piscina, intern, &causa);
        CREDO_NON_NIHIL (canon_canonum);

        vitia = iudicare_literis(canon_canonum, GLOSSA_SUB_ELEMENTO,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        fons = filum_legere_totum("natura/natura.canon", piscina);
        CREDO_MAIOR_I32 (fons.mensura, (i32)ZEPHYRUM);
        canon_naturae = canon_legere(fons, piscina, intern, &causa);
        CREDO_NON_NIHIL (canon_naturae);

        vitia = iudicare_literis(canon_naturae, GLOSSA_SUB_GENERE,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* lingua necessaria: machina generica (necessarium=verum)
         * eam cogit - nullus codex novus in lib/canon.c */
        vitia = iudicare_literis(canon_naturae, GLOSSA_SINE_LINGUA,
                                 piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_ATTRIBUTUM_DEEST), I);

        /* fines in proprietate admissi (spec fines) */
        vitia = iudicare_literis(canon_naturae,
                                 PROPRIETAS_CUM_FINIBUS,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: genus 'titulus' - nomina elementorum punctata
     * (arbor porphyriana 2026-08-10: '<.species>' etc.)
     * ======================================================== */

    {
        chorda  fons;
        chorda  causa;
         Canon* canon_ipse2;
           Xar* vitia;

        imprimere("\n--- Probans genus titulus (nomina punctata) ---\n");

        fons = filum_legere_totum("canon.canon", piscina);
        CREDO_MAIOR_I32 (fons.mensura, (i32)ZEPHYRUM);
        canon_ipse2 = canon_legere(fons, piscina, intern, &causa);
        CREDO_NON_NIHIL (canon_ipse2);

        /* elementum/liberum/intra nomina punctata ACCIPIUNT */
        vitia = iudicare_literis(canon_ipse2,
            "<canon dialectus=\"probandus\" versio=\"1\">\n"
            "  <elementum nomen=\".probandum\" radix=\"verum\">\n"
            "    <attributum nomen=\"nomen\" genus=\"nomen\"/>\n"
            "    <liberum nomen=\".filius\"/>\n"
            "  </elementum>\n"
            "  <elementum nomen=\".filius\" intra=\".probandum\"/>\n"
            "</canon>\n",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* attributum punctatum NUMQUAM (compositum manet) */
        vitia = iudicare_literis(canon_ipse2,
            "<canon dialectus=\"probandus\" versio=\"1\">\n"
            "  <elementum nomen=\"probandum\" radix=\"verum\">\n"
            "    <attributum nomen=\".malus\" genus=\"nomen\"/>\n"
            "  </elementum>\n"
            "</canon>\n",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_VALOR_MALUS), I);

        /* punctum solum / geminatum in nomine elementi = vitium */
        vitia = iudicare_literis(canon_ipse2,
            "<canon dialectus=\"probandus\" versio=\"1\">\n"
            "  <elementum nomen=\"..x\" radix=\"verum\"/>\n"
            "</canon>\n",
            piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_MAIOR_I32 (quot_generis(vitia, CANON_VALOR_MALUS),
                         (i32)ZEPHYRUM);
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
