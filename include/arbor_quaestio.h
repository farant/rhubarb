#ifndef ARBOR_QUAESTIO_H
#define ARBOR_QUAESTIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "arbor_syntaxis.h"

/* ==================================================
 * ARBOR QUAESTIO - CSS-Style AST Query Engine
 *
 * Quaerit nodos AST cum selectoribus CSS-similis.
 * Supportat captures ($name), pseudo-selectors.
 *
 * SYNTAXIS:
 *   function-definition                  # Omnes functiones
 *   identifier[value="main"]             # Identificator nominatus main
 *   function-definition > compound-statement
 *   :calls(printf)                       # Vocationes ad printf
 *   function-definition > identifier$fn  # Capture fn nomen
 *
 * EXEMPLUM:
 *   ArborQuaestio* q = arbor_quaestio_compilare(piscina, intern,
 *       "function-definition > identifier$fn");
 *   Xar* resultata = arbor_quaestio_exsequi(q, radix);
 *   per (i32 i = ZEPHYRUM; i < xar_numerus(resultata); i++) {
 *       ArborQuaestioResultatum* r = xar_obtinere(resultata, i);
 *       // r->nodus est identifier, r->capturae habet "fn" -> nodus
 *   }
 * ================================================== */

/* ==================================================
 * Selector Part Types
 * ================================================== */

nomen enumeratio {
    QUAESTIO_SIMPLEX_GENUS,       /* Type selector: identifier */
    QUAESTIO_SIMPLEX_ATTRIBUTUM,  /* [value="main"] */
    QUAESTIO_SIMPLEX_PSEUDO,      /* :calls(printf) */
    QUAESTIO_SIMPLEX_CAPTURA      /* $name capture marker */
} QuaestioSimplexGenus;

/* ==================================================
 * Attribute Operators
 * ================================================== */

nomen enumeratio {
    QUAESTIO_ATTR_EXISTIT,        /* [attr] - attributum existit */
    QUAESTIO_ATTR_AEQUALIS,       /* [attr="val"] */
    QUAESTIO_ATTR_INCIPIT,        /* [attr^="val"] */
    QUAESTIO_ATTR_TERMINAT,       /* [attr$="val"] */
    QUAESTIO_ATTR_CONTINET        /* [attr*="val"] */
} QuaestioAttrOp;

/* ==================================================
 * Combinator Types
 * ================================================== */

nomen enumeratio {
    QUAESTIO_COMBINATOR_NULLUS,
    QUAESTIO_COMBINATOR_DESCENDENS,   /* Space: any descendant */
    QUAESTIO_COMBINATOR_LIBERUM       /* >: direct child */
} QuaestioCombinator;

/* ==================================================
 * Built-in Pseudo-Selector Types
 * ================================================== */

nomen enumeratio {
    QUAESTIO_PSEUDO_CALLS,        /* :calls(name) */
    QUAESTIO_PSEUDO_DEFINES,      /* :defines(name) */
    QUAESTIO_PSEUDO_USES,         /* :uses(name) */
    QUAESTIO_PSEUDO_RETURNS,      /* :returns */
    QUAESTIO_PSEUDO_C89,          /* :c89 */
    QUAESTIO_PSEUDO_C99,          /* :c99 */
    QUAESTIO_PSEUDO_EXTENSION,    /* :extension */
    QUAESTIO_PSEUDO_CUSTOM        /* User-registered */
} QuaestioPseudoGenus;

/* ==================================================
 * Custom Pseudo-Selector Function
 * ================================================== */

nomen b32 (*ArborPseudoFunctio)(ArborNodus* n, vacuum* datum);

/* ==================================================
 * Selector Simplex - Singulum criterion
 * ================================================== */

nomen structura {
    QuaestioSimplexGenus  genus;

    /* Pro QUAESTIO_SIMPLEX_GENUS */
    ArborNodusGenus       nodus_genus;

    /* Pro QUAESTIO_SIMPLEX_ATTRIBUTUM */
    chorda*               attr_nomen;
    QuaestioAttrOp        attr_op;
    chorda*               attr_valor;

    /* Pro QUAESTIO_SIMPLEX_PSEUDO */
    QuaestioPseudoGenus   pseudo_genus;
    chorda*               pseudo_arg;
    ArborPseudoFunctio    pseudo_functio;
    vacuum*               pseudo_datum;

    /* Pro QUAESTIO_SIMPLEX_CAPTURA */
    chorda*               captura_nomen;
} QuaestioSimplex;

/* ==================================================
 * Selector Composita - Partes in uno nodo
 *
 * e.g., "identifier[value="main"]$id" habet tres partes
 * ================================================== */

nomen structura {
    Xar*                  partes;   /* Xar of QuaestioSimplex* */
} QuaestioComposita;

/* ==================================================
 * Selector Sequentia - Catena cum combinatoribus
 *
 * e.g., "function-definition > compound-statement"
 * ================================================== */

nomen structura QuaestioSequentia QuaestioSequentia;

structura QuaestioSequentia {
    QuaestioComposita*    composita;
    QuaestioCombinator    combinator;
    QuaestioSequentia*    sequens;
};

/* ==================================================
 * Query Result
 * ================================================== */

nomen structura {
    ArborNodus*           nodus;
    TabulaDispersa*       capturae;   /* nomen -> ArborNodus* */
} ArborQuaestioResultatum;

/* ==================================================
 * Compiled Query (opaque)
 * ================================================== */

nomen structura ArborQuaestio ArborQuaestio;

/* ==================================================
 * API - Compilation
 * ================================================== */

/* Compilare selector string to query */
ArborQuaestio* arbor_quaestio_compilare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   selector);

/* ==================================================
 * API - Execution
 * ================================================== */

/* Exsequi query on AST - returns Xar of ArborQuaestioResultatum */
Xar* arbor_quaestio_exsequi(
    ArborQuaestio*        q,
    ArborNodus*           radix);

/* Check if single node matches (sine captures) */
b32 arbor_quaestio_congruit(
    ArborQuaestio*        q,
    ArborNodus*           nodus);

/* ==================================================
 * API - Custom Pseudo-Selectors
 * ================================================== */

/* Registrare custom pseudo-selector */
vacuum arbor_quaestio_registrare_pseudo(
    ArborQuaestio*        q,
    constans character*   titulus,
    ArborPseudoFunctio    functio,
    vacuum*               datum);

/* ==================================================
 * API - Utilities
 * ================================================== */

/* Obtinere nomen pseudo-selector genus */
constans character* arbor_quaestio_pseudo_nomen(QuaestioPseudoGenus genus);

/* Obtinere nomen combinator */
constans character* arbor_quaestio_combinator_nomen(QuaestioCombinator comb);

#endif /* ARBOR_QUAESTIO_H */
