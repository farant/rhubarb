#ifndef SELECTIO_H
#define SELECTIO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"

/* ==================================================
 * Selectio - Parser et Congruens Selectorum CSS
 *
 * Sustinet:
 * - Selectores tituli: div, span
 * - Selectores ID: #myid
 * - Selectores classis: .myclass (congruit cum attributo labels)
 * - Selectores attributi: [attr], [attr=val], [attr^=val], etc.
 * - Combinatores: descendens (spatium), liberum (>), fratres (+, ~)
 * - Pseudo-classes: :first-child, :last-child, :empty
 * ================================================== */

/* ==================================================
 * Genera Selectionum
 * ================================================== */

/* Genera componentium selectoris */
nomen enumeratio {
    SELECTIO_TITULUS        = I,     /* nomen tituli */
    SELECTIO_ID             = II,    /* #identificator */
    SELECTIO_CLASSIS        = III,   /* .nomen_classis */
    SELECTIO_ATTRIBUTUM     = IV,    /* [attr] */
    SELECTIO_PSEUDO_CLASSIS = V      /* :pseudo */
} SelectioGenus;

/* Genera operatorum attributi */
nomen enumeratio {
    ATTR_OP_EXISTIT   = I,   /* [attr] - existit */
    ATTR_OP_AEQUALIS  = II,  /* [attr=val] - aequalis */
    ATTR_OP_INCIPIT   = III, /* [attr^=val] - incipit cum */
    ATTR_OP_TERMINAT  = IV,  /* [attr$=val] - terminat cum */
    ATTR_OP_CONTINET  = V,   /* [attr*=val] - continet */
    ATTR_OP_VERBUM    = VI   /* [attr~=val] - verbum congruit */
} AttributumOperator;

/* Genera pseudo-classium */
nomen enumeratio {
    PSEUDO_PRIMUS_LIBERUM  = I,   /* :first-child */
    PSEUDO_ULTIMUS_LIBERUM = II,  /* :last-child */
    PSEUDO_VACUUS          = III  /* :empty */
} PseudoClassisGenus;

/* Genera combinatorum */
nomen enumeratio {
    COMBINATOR_DESCENDENS  = I,   /* spatium - quilibet descendens */
    COMBINATOR_LIBERUM     = II,  /* > - liberum directum */
    COMBINATOR_FRATER_ADJ  = III, /* + - frater adjacens */
    COMBINATOR_FRATER_GEN  = IV   /* ~ - frater generalis */
} CombinatorGenus;

/* ==================================================
 * Structurae
 * ================================================== */

/* Componens selectoris simplicis */
nomen structura {
    SelectioGenus       genus;
    chorda*             valor;       /* nomen tituli, id, classis */
    AttributumOperator  attr_op;     /* pro selectoribus attributi */
    chorda*             attr_valor;  /* valor attributi ad congruendum */
    PseudoClassisGenus  pseudo;      /* pro selectoribus pseudo-classis */
} SelectioSimplex;

/* Selector compositus (plures selectores simplices sine combinatore)
 * e.g., div.class#id congruit cum omnibus tribus conditionibus
 */
nomen structura {
    Xar* partes;  /* Xar de SelectioSimplex */
} SelectioComposita;

/* Catena selectoris complexi cum combinatoribus
 * e.g., div > p .class est catena III selectorum compositorum
 */
nomen structura SelectioSequentia {
    SelectioComposita*           composita;
    CombinatorGenus              combinator;  /* combinator ANTE hoc */
    structura SelectioSequentia* sequens;     /* proximum in catena */
} SelectioSequentia;

/* Resultus parsationis */
nomen structura {
    b32                successus;
    SelectioSequentia* selectio;
    i32                error_positus;
    chorda             error;
} SelectioResultus;

/* ==================================================
 * Parsatio - Legere Selectores
 * ================================================== */

/* Legere selectorem ex chorda */
SelectioResultus
selectio_legere(
    chorda               input,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Legere selectorem ex literis C */
SelectioResultus
selectio_legere_ex_literis(
    constans character*  cstr,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* ==================================================
 * Congruentia - Verificare Congruentia
 * ================================================== */

/* Verificare si nodus congruit cum selectore */
b32
selectio_congruit(
    SelectioSequentia* selectio,
    StmlNodus*         nodus);

/* ==================================================
 * Quaestio - Invenire Nodos
 * ================================================== */

/* Invenire primum nodum congruentem (profunditas prima) */
StmlNodus*
selectio_invenire_primum(
    StmlNodus*         radix,
    SelectioSequentia* selectio);

/* Invenire omnes nodos congruentes
 * Redde: Xar de StmlNodus*
 */
Xar*
selectio_invenire_omnes(
    StmlNodus*         radix,
    SelectioSequentia* selectio,
    Piscina*           piscina);

/* ==================================================
 * Commoditas - Functiones Commoditatis
 * ================================================== */

/* Legere et invenire primum congruentem (modus querySelector) */
StmlNodus*
stml_quaerere(
    StmlNodus*           radix,
    constans character*  selector,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Legere et invenire omnes congruentes (modus querySelectorAll)
 * Redde: Xar de StmlNodus*
 */
Xar*
stml_quaerere_omnes(
    StmlNodus*           radix,
    constans character*  selector,
    Piscina*             piscina,
    InternamentumChorda* intern);

#endif /* SELECTIO_H */
