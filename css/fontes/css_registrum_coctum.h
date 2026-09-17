/* css_registrum_coctum.h
 *
 * Registrum generum COCTUM grammaticae 'css' - GENERATUM, NE MANU
 * EDITES. Fons: css/grammatica/css.registrum.stml (materia/coquere.sh).
 * Genera XXI, loci XLV.
 */

#ifndef CSS_REGISTRUM_COCTUM_H
#define CSS_REGISTRUM_COCTUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    /* Structura */
    CSS_GENUS_PLAGULA = 0,
    CSS_GENUS_REGULA_QUALIFICATA,
    CSS_GENUS_REGULA_APUD,
    CSS_GENUS_DECLARATIO,
    CSS_GENUS_PRAEVALENTIA,
    /* Valores componentes (CSS Syntax L3) */
    CSS_GENUS_LEXEMA_SERVATUM,
    CSS_GENUS_FUNCTIO,
    CSS_GENUS_SAEPTUM,
    /* Robustitas: recuperatio spec-definita */
    CSS_GENUS_REGULA_MALA,
    CSS_GENUS_DECLARATIO_MALA,
    /* Selectores (spec par. V, B8) - arbor ANALYSEOS super lexemata
     * praeludii (css_selector), non emissionis. APPENSA post genera
     * priora: interpositio indices registri tacite moveret */
    CSS_GENUS_SELECTOR_LISTA,
    CSS_GENUS_SELECTOR_COMPLEXUS,
    CSS_GENUS_SELECTOR_COMPOSITUS,
    CSS_GENUS_SELECTOR_TYPI,
    CSS_GENUS_SELECTOR_UNIVERSALIS,
    CSS_GENUS_SELECTOR_CLASSIS,
    CSS_GENUS_SELECTOR_IDENTITATIS,
    CSS_GENUS_SELECTOR_ATTRIBUTI,
    CSS_GENUS_PSEUDO_CLASSIS,
    CSS_GENUS_PSEUDO_ELEMENTUM,
    CSS_GENUS_SELECTOR_MALUS,

    CSS_GENUS_NUMERUS_GENERUM
} CssGenus;

externus constans MateriaRegistrumCoctum CSS_REGISTRUM;

externus constans MateriaDiagnosticaCocta CSS_DIAGNOSTICA;

#endif /* CSS_REGISTRUM_COCTUM_H */
