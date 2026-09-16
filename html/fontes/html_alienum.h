/* html_alienum.h - Spatia nominum aliena: SVG et MathML
 *
 * Tabulae COMMUNES aedificatoris (eruptio, tag se-claudens) et
 * visionis coctae (O2b-6, 2026-09-15). Aedificator spatia nominum
 * non fert: tituli et positio sufficiunt. Spatium PROPRIUM elementi
 * ex parente derivatur (svg/math in HTML aperiunt; in puncto
 * integrationis MathML mglyph/malignmark MathML manent), spatium
 * LIBERORUM ex proprio (puncta integrationis HTML: foreignObject/
 * desc/title in SVG, mi/mo/mn/ms/mtext et annotation-xml cum
 * encoding text/html | application/xhtml+xml in MathML). Tituli svg
 * camelCase (WHATWG 'adjust SVG tag names'), attributa svg
 * accommodata (LVIII), definitionURL MathML, praefixa xlink/xml/xmlns
 * in spatia ('xlink href').
 */

#ifndef HTML_ALIENUM_H
#define HTML_ALIENUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "materia_nodus.h"

nomen enumeratio {
    HTML_ALIENUM_NULLUM = 0,
    HTML_ALIENUM_SVG,
    HTML_ALIENUM_MATHEMATICA
} HtmlAlienum;

/* Spatium proprium elementi 'titulus' (minusculus) cuius parens
 * spatium 'parentis' habet et liberis 'liberorum_parentis' dat. */
HtmlAlienum
html_alienum_proprium (
    HtmlAlienum parentis,
    HtmlAlienum liberorum_parentis,
         chorda titulus);

/* Spatium liberorum elementi cuius spatium proprium 'proprium' est
 * (titulus minusculus, svg accommodatus); elementum NIHIL = sine
 * attributis (annotation-xml tum MathML). */
HtmlAlienum
html_alienum_liberorum (
              HtmlAlienum  proprium,
                   chorda  titulus,
    constans MateriaNodus* elementum);

/* Titulus minusculus -> accommodatus svg camelCase si proprium SVG
 * (copia in piscina), aliter idem. */
chorda
html_alienum_titulus (
        Piscina* piscina,
         chorda  minusculus,
    HtmlAlienum  proprium);

/* Titulus attributi minusculus -> coctus in spatio proprio: svg
 * camelCase, MathML definitionURL, 'xlink:x' -> 'xlink x', 'xml:x',
 * 'xmlns:x', 'xmlns' -> 'xmlns xmlns' (alieni soli). Copia in
 * piscina ubi mutatur, aliter idem. */
chorda
html_alienum_attributum (
        Piscina* piscina,
         chorda  minusculus,
    HtmlAlienum  proprium);

/* Tag apertionis HTML quod contentum alienum RUMPIT (WHATWG 'in
 * foreign content': b big blockquote body br center code dd div dl
 * dt em embed h1-h6 head hr i img li listing menu meta nobr ol p pre
 * ruby s small span strong strike sub sup table tt u ul var; font
 * cum color/face/size hic NON - attributa post tag veniunt). */
b32
html_alienum_rumpit (
    chorda titulus);

#endif /* HTML_ALIENUM_H */
