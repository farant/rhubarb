/* html_coctum.h - Visio COCTA arboris html in forma html5lib
 *
 * Serializator arboris html_arbor in formam sectionis '#document'
 * plagularum .dat html5lib (tree-construction): lineae '| ' + II
 * spatia per gradum; '<titulus>' minusculis ASCII; alieni '<svg
 * titulus>' / '<math titulus>' (tituli svg camelCase accommodati;
 * puncta integrationis HTML foreignObject/desc/title et mi/mo/mn/ms/
 * mtext); attributa ordine octetorum lineis suis 'titulus="valor"'
 * (titulus minusculus, valor citationibus exutus et decoctus, titulus
 * bis datus semel servatus - primus); textus '"..."' (referentiae
 * decoctae, fratres contigui in unum coniuncti, CR LF -> LF; linea
 * nova prima post pre/listing/textarea dempta); commentaria '<!--
 * datum -->' (prava '<!x' '<?x' quoque); '<!DOCTYPE titulus>' aut cum
 * identificatoribus '"publicus" "systema"'; template: pseudo-nodus
 * 'content'; CDATA intra alienos textus, extra commentarium;
 * elementa-mala OMISSA (DOM nihil eorum retinet).
 *
 * VISIO est, non documentum: octeti arboris ipsius crudi manent (spec
 * par. XI: paritas = forma, valores normalizati DERIVATI). Ambulatio
 * RECURSIVA - fixurae oraculi, non paginae M gradus profundae.
 * Consumens primus: probatio_html_oraculum (O2).
 */

#ifndef HTML_COCTUM_H
#define HTML_COCTUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "materia_nodus.h"

/* Contentum alienum (HTML5 'foreign content') in quo elementum vivit */
nomen enumeratio {
    HTML_ALIENUM_NULLUM = 0,
    HTML_ALIENUM_SVG,
    HTML_ALIENUM_MATHEMATICA
} HtmlAlienum;

/* Contentum liberorum elementi 'titulus' (minusculus, svg accommodatus)
 * in contento 'parentis': svg/math aperiunt, puncta integrationis
 * HTML claudunt (annotation-xml cum encoding text/html aut
 * application/xhtml+xml; elementum NIHIL = sine attributis). */
HtmlAlienum
html_coctum_alienum_liberorum (
              HtmlAlienum  parentis,
                   chorda  titulus,
    constans MateriaNodus* elementum);

/* Titulus tagi minusculus (svg camelCase accommodatus si alienum
 * SVG); copia in piscina. */
chorda
html_coctum_titulus (
        Piscina* piscina,
         chorda  crudus,
    HtmlAlienum  alienum);

/* Liberos radicis (documentum aut elementum) gradu ZEPHYRUM scribere;
 * alienum = contentum contextus (fragmentum in 'svg path' etc.).
 * Chorda lineis '\n' separatis, sine '\n' ultimo; vacua si nulli
 * liberi. datum NIHIL = memoria deficit. */
chorda
html_coctum_scribere (
                  Piscina* piscina,
    constans MateriaNodus* radix,
              HtmlAlienum  alienum);

#endif /* HTML_COCTUM_H */
