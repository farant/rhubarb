#ifndef ARBOR2_CONDITIO_EVALUARE_H
#define ARBOR2_CONDITIO_EVALUARE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"
#include "arbor2_expandere.h"

/* ==================================================
 * ARBOR2 CONDITIO EVALUARE - Preprocessor Expression Evaluator
 *
 * Evaluates C89 preprocessor constant expressions for
 * #if and #elif directives.
 *
 * Supports:
 *   - Integer constants
 *   - Arithmetic: + - * / % (unary -)
 *   - Comparison: < > <= >= == !=
 *   - Logical: && || !
 *   - Bitwise: & | ^ ~ << >>
 *   - Ternary: ? :
 *   - Grouping: ( )
 *   - defined(MACRO) and defined MACRO
 *   - Macro expansion
 *
 * EXEMPLUM:
 *   i64 valor = arbor2_conditio_evaluare(exp, tokens, &successus);
 * ================================================== */

/* ==================================================
 * API - Expression Evaluation
 * ================================================== */

/* Evaluate preprocessor constant expression
 *
 * exp       - Expansion context for macro lookups
 * lexemata  - Expression tokens (Xar of Arbor2Token*)
 * successus - Output: VERUM if evaluation succeeded
 *
 * Returns evaluated value (0 = false, nonzero = true)
 * On error, returns 0 and sets *successus to FALSUM
 */
i64 arbor2_conditio_evaluare(
    Arbor2Expansion*    exp,
    Xar*                lexemata,
    b32*                successus);

/* ==================================================
 * API - Macro Definition Query
 * ================================================== */

/* Check if macro is defined
 *
 * exp     - Expansion context
 * titulus - Macro name to check
 *
 * Returns VERUM if macro is defined, FALSUM otherwise
 */
b32 arbor2_conditio_est_definitum(
    Arbor2Expansion*    exp,
    chorda              titulus);

#endif /* ARBOR2_CONDITIO_EVALUARE_H */
