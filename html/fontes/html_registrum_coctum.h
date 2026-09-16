/* html_registrum_coctum.h
 *
 * Registrum generum COCTUM grammaticae 'html' - GENERATUM, NE MANU
 * EDITES. Fons: html/grammatica/html.registrum.stml
 * (materia/coquere.sh). Genera X, loci XXV.
 */

#ifndef HTML_REGISTRUM_COCTUM_H
#define HTML_REGISTRUM_COCTUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    /* Structura documenti: series nodorum summorum (fragmentum
     * nullum aut plura elementa radicis habere potest; involucrum
     * html FICTUM sub documento, locus synthesis - O7a) + lexema
     * FINIS */
    HTML_GENUS_DOCUMENTUM = 0,
    HTML_GENUS_DOCTYPE,
    /* Sex loci lexematum OMNES optionales (H4): vacuum, clausura
     * implicita, apertum ad EOF, se-claudens = loci absentes,
     * numquam lexemata ficta. synthesis (O7a, 2026-09-15): elementum
     * a spec HTML5 FICTUM - lexemata nulla, index in tabulam
     * html_arbor (html head body tbody tr colgroup p br; ZEPHYRUM
     * numquam scriptus, locus absens = elementum verum); visio cocta
     * id imprimit, emissor nihil. exemplar (O7c, 2026-09-16):
     * elementum formans iterum apertum (adoption agency,
     * reconstructio) - lexemata nulla, titulus et attributa elementi
     * originalis referentia lecti. praecedens (O7c): frater DOM
     * prior elementi ab adoption agency moti (post elementum formans
     * clausum) */
    HTML_GENUS_ELEMENTUM,
    HTML_GENUS_ATTRIBUTUM,
    /* Contentum: nodi, non trivia (H6). Spatia extra tags textus
     * sunt. sedes (O7b): parens DOM textus alibi positi (foster
     * parenting, post body) */
    HTML_GENUS_TEXTUS,
    /* Referentia characteris (entitas). sedes (O7b) ut in textu */
    HTML_GENUS_REFERENTIA,
    HTML_GENUS_TEXTUS_CRUDUS,
    /* sedes (O7b): commentarium post '</body>' cum contentum iam in
     * body rediit (spec 'after body' -> 'in body') */
    HTML_GENUS_COMMENTARIUM,
    HTML_GENUS_CDATA,
    /* Robustitas: tag clausurae sine pari aperto ('&lt;/br&gt;' et
     * '&lt;/p&gt;' sine p aperto elementa FICTA dant, O7c - deviatio
     * '&lt;/br&gt;' malum' H4 retirata) */
    HTML_GENUS_ELEMENTUM_MALUM,

    HTML_GENUS_NUMERUS_GENERUM
} HtmlGenus;

externus constans MateriaRegistrumCoctum HTML_REGISTRUM;

#endif /* HTML_REGISTRUM_COCTUM_H */
