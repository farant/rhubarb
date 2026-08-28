/* css_registrum.h - Vocabularium nodorum CSS
 *
 * MANU SCRIPTUM (decretum 01M134M3, faber differtur). Consequentia
 * BONA quam consilium notat: registro manu scripto enumeratio genus
 * TYPEDEF et COMITEM habere potest - quae silvae, a generatore
 * anonyme emissa, carebat (silva_tabulae_c89.h:21). Ergo hic
 * 'CssGenus' typus VERUS est et CSS_GENUS_NUMERUS exsistit.
 *
 * QUINQUE GENERA STRUCTURALIA NON DECLARANTUR. Silva ea poscebat
 * (ambiguus, error, conditionalis, ramus-sumptus, ramus-omissus);
 * materia M6 requisitum generatoris retiravit et 'ambiguus'
 * OPTIONALEM fecit. Descensus recursivus ambiguitatem non parit;
 * praeprocessorem CSS non habet. Robustitas per genera SPEC-DEFINITA
 * agitur - regula-mala, declaratio-mala - non per nodum 'error'
 * generalem.
 *
 * Vocabularium ex css-arbor-spec.md par. IV.
 */

#ifndef CSS_REGISTRUM_H
#define CSS_REGISTRUM_H

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

    CSS_GENUS_NUMERUS
} CssGenus;

externus constans MateriaRegistrumCoctum CSS_REGISTRUM;

#endif /* CSS_REGISTRUM_H */
