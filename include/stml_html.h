#ifndef STML_HTML_H
#define STML_HTML_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "stml.h"


/* ==================================================
 * STML_HTML - emissio STML -> HTML
 *
 * Praemissa (Fran, spec instrumenti par. 5): "tags in syntaxi
 * STML scribis et in syntaxin HTML vertuntur." NULLUM
 * vocabularium tagorum, nulla tabula translationis - tagi
 * auctoris sunt. Emissor solum discrepantias syntacticas
 * tractat:
 *
 *  - '<!DOCTYPE html>' sponte praefigitur (fons STML sine
 *    doctype - decretum 2026-08-29)
 *  - clausura per leges HTML: elementa VACUA ('br', 'img',
 *    'meta', ...) nuda emittuntur; cetera SEMPER paria
 *    ('<div></div>', numquam '<div/>' - navigator id ut
 *    apertum legit)
 *  - effugium DUPLEX (par. 5.4): textus decoctus -> evade;
 *    attributa inscripta CRUDA -> octeti ut sunt; elementa
 *    attributorum decocta -> evade
 *  - singularia HTML ('<body>' geminum etc.) EMITTUNTUR UT
 *    SCRIPTA (decretum Franis 2026-09-01): emissor validitatem
 *    HTML numquam iudicat - nusquam
 *  - '<crudum!>octeti</crudum>' = SPLEX octetorum SINE tags (decretum
 *    2026-09-03, B3.1 md: html crudum markdown transit ut est) -
 *    nomen unum decretum, semper cum '!' (sine eo CRUDUS_DEEST) -
 *    splex tag-less per nomen unum visibilis manet, numquam tacitus
 *
 * Recusationes (omnes CLAMOSAE, primus vincit): processio,
 * transclusio non resoluta, augmentatio, titulus punctatus,
 * titulus NIHIL, elementum vacuum cum liberis, script/style
 * sine '!', ens ambiguum in textu ('&verbum;' - character
 * ipse scribendus), attributum '="true"' non-booleanum,
 * elementum attributi extra praefixum.
 *
 * Arbor POST catenam (legere -> expandere -> distribuere)
 * pascenda est; radix DOCUMENTI. Trivia (spatia_ante/post/
 * clausurae) leguntur - sine iis 'x y' fit 'xy' (par. 5.9).
 * ================================================== */

nomen enumeratio {
    STML_HTML_BENE               = ZEPHYRUM,
    STML_HTML_PROCESSIO          = I,     /* <?...?> - HTML non habet */
    STML_HTML_TRANSCLUSIO        = II,    /* <<sel>> non resoluta */
    STML_HTML_AUGMENTATIO        = III,   /* <% &k;> */
    STML_HTML_TITULUS_PUNCTATUS  = IV,    /* <.x> non est nomen HTML */
    STML_HTML_TITULUS_NIHIL      = V,     /* < > */
    STML_HTML_VACUUM_CUM_LIBERIS = VI,    /* <br>x</br> illicitum */
    STML_HTML_CRUDUS_DEEST       = VII,   /* script/style sine '!' */
    STML_HTML_ENS_AMBIGUUM       = VIII,  /* '&verbum;' in textu */
    STML_HTML_ATTRIBUTUM_VERUM   = IX,    /* ="true" non-booleanum */
    STML_HTML_ATTRIBUTUM_SOLUTUM = X,     /* attr-elementum extra
                                           * praefixum (anomalum
                                           * post expansionem) */
    STML_HTML_MEMORIA            = XI
} StmlHtmlVitium;

nomen structura {
               b32 successus;
            chorda html;       /* documentum totum, doctype incluso */
    StmlHtmlVitium vitium;
            chorda detail;     /* titulus/attributum/ens peccans
                                * (aut vacua) */
} StmlHtmlResultus;

/* Vertere documentum STML (radix DOCUMENTI, post catenam) in
 * HTML. Nihil partiale: in vitio html vacua est. */
StmlHtmlResultus
stml_html_vertere (
    StmlNodus* radix,
      Piscina* piscina);

/* Vertere FRAGMENTUM: liberos elementi (arbor post catenam) in HTML
 * SINE doctype - pro transformationibus quae fragmentum reddunt
 * (md -> html, B2.2: CommonMark exitum sine involucro documenti
 * comparat). Leges eaedem ac stml_html_vertere. */
/* litteralis (B3.3): arbor a PARSATORE genita, non ex fonte STML -
 * octetus quisque litteralis: '&' in textu semper evaditur (nulla
 * recusatio entis ambigui - ambiguitas lectoris STML est, non arboris
 * genitae), attributa inscripta EVADUNTUR ('&' '"') ut decocta.
 * FALSUM = leges eaedem ac stml_html_vertere. */
StmlHtmlResultus
stml_html_vertere_liberos (
     StmlNodus* parens,
       Piscina* piscina,
           b32  litteralis);

/* Titulus vitii (pro nuntiis et probationibus) */
constans character*
stml_html_vitium_titulus (
    StmlHtmlVitium vitium);

#endif /* STML_HTML_H */
