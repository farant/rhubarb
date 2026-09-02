/* silva_conditio.h - Evaluator expressionum praeprocessoris (#if/#elif)
 *
 * Portatum ex arbor2_conditio_evaluare cum duabus emendationibus:
 *   - s64 pro i64: latina i64 INSIGNATUS est - arbor2 comparationes
 *     signatas insignate computabat (#if -1 < 0 erat FALSUM!)
 *   - expansio plena: operanda 'defined' primum substituuntur
 *     (protecta, numquam expansa), tum expressio TOTA per expansorem
 *     verum expanditur, tum arithmetica evaluatur. arbor2 solum
 *     macros obiecti-similes unius lexematis tractabat.
 *
 * Identificatores post expansionem reliqui = 0 (C89 6.8.1).
 * In errore: 0 redditur et *successus = FALSUM (nullus fragor).
 */

#ifndef SILVA_CONDITIO_H
#define SILVA_CONDITIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_expandere.h"

/* Estne macro definitum in tabula viva? */
b32
silva_conditio_est_definitum (
    SilvaExpansio* exp,
           chorda  titulus);

/* Evaluare expressionem constantem praeprocessoris.
 * lexemata: Xar de SilvaToken* (linea directivae post verbum, sine EOF).
 * Reddit valorem (0 = falsum); *successus FALSUM in errore. */
s64
silva_conditio_evaluare (
    SilvaExpansio* exp,
              Xar* lexemata,
              b32* successus);

#endif /* SILVA_CONDITIO_H */
