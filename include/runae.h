/* runae.h - Nucleus Unicode (acervus textus, stratum primum)
 *
 * Proprietates runarum (codepoints) ex TABULIS GENERATIS e datis Unicode
 * fixis (probationes/fixa/unicode/<versio>/, tools/runae_generare.sh).
 * Lapis primus: LATITUDO in cellulis terminalis. Postea hic (cum
 * trahuntur): rupturae graphematum (UAX #29), normalizatio, casus,
 * rupturae linearum, bidi. Locale (collatio, formae) NUMQUAM hic.
 *
 * Purum: nulla allocatio, nullus status mutabilis, totale.
 * Unitates in nominibus: octeti (bytes), runa (codepoint), graphema
 * (cluster), latitudo (cellulae) - numquam 'n' nudum.
 */

#ifndef RUNAE_H
#define RUNAE_H

#include "latina.h"

/* Versio datorum Unicode tabularum - contractus, non ornamentum */
#define RUNAE_VERSIO "15.1.0"

/* Latitudo runae in cellulis: 0, 1 aut 2. Regula Ghostty (uucode
 * wcwidth_standalone + wcwidth_zero_in_grapheme): 0 regimina (Cc), Cs,
 * Zl, Zp, Default_Ignorable (praeter U+00AD = 1), Mn, Me, iamo Hangul
 * V/T; 2 East_Asian_Width W/F et Regional_Indicator; Mc = 1;
 * modificatores emoji et Prepend latitudinem suam servant; ceterae 1.
 * TOTALIS: runa invalida (< 0 aut > U+10FFFF) = 1 (U+FFFD pingitur). */
i32
runae_latitudo (
    s32 runa);

#endif /* RUNAE_H */
