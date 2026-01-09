#ifndef ARBOR_H
#define ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "arbor_syntaxis.h"
#include "arbor_praeparator.h"

/* ==================================================
 * ARBOR - Convenience API
 *
 * High-level wrapper functions for common parsing tasks.
 * Simplifies the verbose pipeline of lex -> preprocess -> parse.
 *
 * EXEMPLUM:
 *   ArborResultus res = arbor_parsere_filum("lib/foo.c", piscina, intern, NIHIL);
 *   si (res.successus)
 *   {
 *       arbor_nodus_imprimere(res.radix, ZEPHYRUM);
 *   }
 * ================================================== */

/* ==================================================
 * Options
 * ================================================== */

nomen structura {
    ArborPPModus          pp_modus;        /* HYBRID, PROCESSARE, PRESERVARE */
    constans character**  include_viae;    /* Array of include paths */
    i32                   include_numerus; /* Number of include paths */
} ArborOptiones;

/* ==================================================
 * Result
 * ================================================== */

nomen structura {
    b32          successus;
    ArborNodus*  radix;        /* AST root */
    Xar*         errores;      /* Parse errors (Xar of ArborError) */
    Xar*         lexemata;     /* Preprocessed tokens (for formatter) */
} ArborResultus;

/* ==================================================
 * API
 * ================================================== */

/* Optiones default: HYBRID modus, "include" via */
ArborOptiones arbor_optiones_default(vacuum);

/* Parsere filum per viam
 *
 * via:       Via ad filum (e.g., "lib/foo.c")
 * piscina:   Arena pro allocare
 * intern:    String internment
 * optiones:  NIHIL pro default
 *
 * Legit filum, lexet, praeparat, et parset.
 */
ArborResultus
arbor_parsere_filum(
    constans character*   via,
    Piscina*              piscina,
    InternamentumChorda*  intern,
    ArborOptiones*        optiones);

/* Parsere fontem directe
 *
 * fons:      Source code buffer
 * longitudo: Length in bytes
 * piscina:   Arena pro allocare
 * intern:    String internment
 * optiones:  NIHIL pro default
 *
 * Lexet, praeparat, et parset ex memoria.
 */
ArborResultus
arbor_parsere_fontem(
    constans character*   fons,
    i32                   longitudo,
    Piscina*              piscina,
    InternamentumChorda*  intern,
    ArborOptiones*        optiones);

#endif /* ARBOR_H */
