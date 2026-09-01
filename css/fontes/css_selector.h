/* css_selector.h - Arbor selectorum (spec par. V, B9)
 *
 * ARBOR ANALYSEOS, non emissionis (spec D3). Praeludium regulae
 * (lista valorum componentium quam css_arbor parit) in arborem
 * selectorum vertitur quae LEXEMATA cum arbore plagulae communicat -
 * numquam NODOS (pater unus): folia nova super lexemata eadem.
 * Ergo porta octetorum NULLA consulto: spatia iuxta combinatorem
 * punctuatum, colon alterum '::', '=' post '^' non referuntur.
 * Praeludium octetos omnes possidet. NE PORTAM ADDAS.
 *
 * COMBINATOR super folia servata (B2): '>' '+' '~' aut lexema
 * spatiorum IPSUM ubi combinatori punctuato non adiacet. 'a>b' et
 * 'a > b' arbores EASDEM dant; 'a b' et 'ab' DIVERSAS.
 *
 * TOTALIS: quod parsari nequit 'selector-malus' fit. NIHIL solum
 * si memoria deficit.
 */

#ifndef CSS_SELECTOR_H
#define CSS_SELECTOR_H

#include "latina.h"
#include "piscina.h"
#include "materia_nodus.h"

/* Praeludium (MATERIA_VALOR_LISTA valorum componentium) in arborem
 * selectorum vertere. Radix: selector-lista si commata suprema,
 * aliter selector unus (complexus / compositus / malus). Patres
 * positi (materia_arbor_patres_figere). */
MateriaNodus*
css_selector_parsare (
         Piscina* piscina,
    MateriaValor  praeludium);

#endif /* CSS_SELECTOR_H */
