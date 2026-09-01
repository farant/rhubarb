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

    /* Selectores (spec par. V, B8) - arbor ANALYSEOS super lexemata
     * praeludii (css_selector), non emissionis. APPENSA post genera
     * priora: interpositio indices registri TACITE moveret. */
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

    CSS_GENUS_NUMERUS
} CssGenus;

/* ==================================================
 * Indices LOCORUM intra nodum
 *
 * Registrum fenestram nominat (loci_offset + loci_numerus);
 * hi indices INTRA fenestram sunt, quos materia_nodus_ponere et
 * materia_nodus_appendere accipiunt.
 *
 * CUR NOMINATI: sine his parsator numeros nudos scriberet -
 * 'materia_nodus_ponere(decl, II, ...)' - et permutatio locorum
 * TACITE valores in alienum locum poneret. Probatio hos contra
 * TITULOS tabulae retiret, ne tertia tabula manu scripta fiant
 * quae per se labi possit.
 * ================================================== */

nomen enumeratio {
    CSS_PLAGULA_REGULAE = 0,
    CSS_PLAGULA_CAUDA
} CssLocusPlagulae;

nomen enumeratio {
    CSS_REGULA_PRAELUDIUM = 0,
    CSS_REGULA_CORPUS
} CssLocusRegulae;

nomen enumeratio {
    CSS_APUD_TOK_NOMEN = 0,
    CSS_APUD_PRAELUDIUM,
    CSS_APUD_CORPUS,
    CSS_APUD_TOK_TERMINATOR
} CssLocusApud;

nomen enumeratio {
    CSS_DECL_TOK_NOMEN = 0,
    CSS_DECL_TOK_COLON,
    CSS_DECL_VALOR,
    CSS_DECL_PRAEVALENTIA,
    CSS_DECL_TOK_TERMINATOR
} CssLocusDeclarationis;

nomen enumeratio {
    CSS_PRAEVALENTIA_TOK_CLAMOR = 0,
    CSS_PRAEVALENTIA_TOK_VERBUM
} CssLocusPraevalentiae;

nomen enumeratio {
    CSS_SERVATUM_TOK = 0
} CssLocusServati;

nomen enumeratio {
    CSS_FUNCTIO_TOK_NOMEN = 0,
    CSS_FUNCTIO_ARGUMENTA,
    CSS_FUNCTIO_TOK_CLAUSUM
} CssLocusFunctionis;

nomen enumeratio {
    CSS_SAEPTUM_TOK_APERTUM = 0,
    CSS_SAEPTUM_CONTENTUM,
    CSS_SAEPTUM_TOK_CLAUSUM
} CssLocusSaepti;

nomen enumeratio {
    CSS_MALUM_TOKENS = 0
} CssLocusMali;

/* Selectores (B8). selector-malus locum 'tokens' cum malis
 * prioribus communicat (CssLocusMali). */
nomen enumeratio {
    CSS_SEL_LISTA_SELECTORES = 0
} CssLocusSelectorisListae;

nomen enumeratio {
    CSS_COMPLEXUS_SINISTER = 0,
    CSS_COMPLEXUS_COMBINATOR,
    CSS_COMPLEXUS_DEXTER
} CssLocusComplexi;

nomen enumeratio {
    CSS_COMPOSITUS_PARTES = 0
} CssLocusCompositi;

nomen enumeratio {
    CSS_TYPI_TOK = 0
} CssLocusTypi;

nomen enumeratio {
    CSS_UNIVERSALIS_TOK = 0
} CssLocusUniversalis;

nomen enumeratio {
    CSS_CLASSIS_TOK_PUNCTUM = 0,
    CSS_CLASSIS_TOK_NOMEN
} CssLocusClassis;

nomen enumeratio {
    CSS_IDENTITATIS_TOK = 0
} CssLocusIdentitatis;

nomen enumeratio {
    CSS_ATTRIBUTI_TOK_APERTUM = 0,
    CSS_ATTRIBUTI_TOK_NOMEN,
    CSS_ATTRIBUTI_TOK_OPERATOR,
    CSS_ATTRIBUTI_VALOR,
    CSS_ATTRIBUTI_TOK_CLAUSUM
} CssLocusAttributi;

nomen enumeratio {
    CSS_PSEUDO_CLASSIS_TOK_COLON = 0,
    CSS_PSEUDO_CLASSIS_TOK_NOMEN,
    CSS_PSEUDO_CLASSIS_ARGUMENTA
} CssLocusPseudoClassis;

nomen enumeratio {
    CSS_PSEUDO_ELEMENTI_TOK_COLON = 0,
    CSS_PSEUDO_ELEMENTI_TOK_NOMEN
} CssLocusPseudoElementi;


externus constans MateriaRegistrumCoctum CSS_REGISTRUM;

#endif /* CSS_REGISTRUM_H */
