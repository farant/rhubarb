/* html_registrum_coctum.h
 *
 * Registrum generum COCTUM grammaticae 'html' - GENERATUM, NE MANU
 * EDITES. Fons: html/grammatica/html.registrum.stml
 * (materia/coquere.sh). Genera X, loci XVIII.
 */

#ifndef HTML_REGISTRUM_COCTUM_H
#define HTML_REGISTRUM_COCTUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    /* Structura documenti: series nodorum summorum (fragmentum
     * nullum aut plura elementa radicis habere potest - synthesis
     * vetita, H4) + lexema FINIS */
    HTML_GENUS_DOCUMENTUM = 0,
    HTML_GENUS_DOCTYPE,
    /* Sex loci lexematum OMNES optionales (H4): vacuum, clausura
     * implicita, apertum ad EOF, se-claudens = loci absentes,
     * numquam lexemata ficta */
    HTML_GENUS_ELEMENTUM,
    HTML_GENUS_ATTRIBUTUM,
    /* Contentum: nodi, non trivia (H6). Spatia extra tags textus
     * sunt */
    HTML_GENUS_TEXTUS,
    HTML_GENUS_REFERENTIA,
    HTML_GENUS_TEXTUS_CRUDUS,
    HTML_GENUS_COMMENTARIUM,
    HTML_GENUS_CDATA,
    /* Robustitas: tag clausurae sine pari aperto (etiam
     * '&lt;/br&gt;', quod HTML5 in '&lt;br&gt;' vertit - deviatio
     * nominata) */
    HTML_GENUS_ELEMENTUM_MALUM,

    HTML_GENUS_NUMERUS_GENERUM
} HtmlGenus;

externus constans MateriaRegistrumCoctum HTML_REGISTRUM;

#endif /* HTML_REGISTRUM_COCTUM_H */
