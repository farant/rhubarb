#ifndef ARBOR2_SCRIBERE_H
#define ARBOR2_SCRIBERE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "arbor2_glr.h"

/* ==================================================
 * ARBOR2 SCRIBERE - AST Serializer for Roundtrip
 *
 * Serializes an AST back to source code, preserving
 * all trivia (whitespace, comments) for exact roundtrip.
 *
 * EXEMPLUM:
 *   chorda* fons = arbor2_scribere(piscina, radix);
 *   // fons now contains the reconstructed source
 * ================================================== */

/* ==================================================
 * API
 * ================================================== */

/* Scribere AST ad chordam
 *
 * Walks the AST and emits all tokens with their trivia.
 * Returns a chorda containing the reconstructed source.
 *
 * piscina - Arena for output allocation
 * radix   - Root AST node (typically TRANSLATION_UNIT)
 *
 * Returns: chorda with reconstructed source
 */
chorda* arbor2_scribere(Piscina* piscina, Arbor2Nodus* radix);

/* Scribere singulum lexema ad Xar
 *
 * Emits a single token with its trivia to the output buffer.
 * Useful for debugging or custom serialization.
 *
 * output - Xar of i8 (character buffer)
 * token  - Token to emit
 */
vacuum arbor2_scribere_lexema(Xar* output, Arbor2Token* token);

#endif /* ARBOR2_SCRIBERE_H */
