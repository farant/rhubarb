/* sonus_wav.h - Exempla soni in plagulam WAV (RIFF, PCM XVI bitorum)
 *
 * Forma imago_png: codificare = computatio PURA in octetos (sine
 * plagula probabilis), scribere = commoditas (codificare + filum).
 * Separatum a sonus.c, ut primitiva sine I/O maneant.
 *
 * Exitus: PCM integer XVI bitorum signatus, 'little-endian', mono aut
 * stereo intertextus (L R L R ...), caput XLIV octetorum. Exemplum f32
 * x -> rotundatum(x * 32767), extra [-1, 1] COERCETUR (numquam
 * circumvolvitur - crepitus maximus). Symmetricum: -1 -> -32767.
 *
 * Usus: auditio (tools/auditio.c), aurea probationum, exportatio
 * replay. Lectio WAV (et conversio frequentiae) postea, cum soni scripti
 * trahuntur (spec §IX 'later').
 *
 * USUS:
 *   chorda octeti = sonus_wav_codificare(x, n, I, SONUS_FREQUENTIA,
 *                                        piscina);
 *   si (!sonus_wav_scribere("/tmp/radius.wav", x, n, I,
 *                           SONUS_FREQUENTIA, piscina)) ...
 */
/* <aedilis corpus="lib/sonus_wav.c"/> */
#ifndef SONUS_WAV_H
#define SONUS_WAV_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

#define SONUS_WAV_CAPUT XLIV   /* octeti capitis RIFF/fmt/data */

/* quadra = exempla per canalem (stereo: quadrum = L + R); exempla
 * habet quadra x canales valores. canales I aut II, frequentia > 0;
 * aliter chorda mensurae 0. Mensura successus = XLIV + quadra x
 * canales x II */
chorda
sonus_wav_codificare (
    constans f32* exempla,
             i32  quadra,
             i32  canales,
             i32  frequentia,
         Piscina* piscina);

/* codificare + filum_scribere (via filum - examen 96); FALSUM si
 * codificatio aut scriptio deficit */
b32
sonus_wav_scribere (
    constans character* via,
          constans f32* exempla,
                   i32  quadra,
                   i32  canales,
                   i32  frequentia,
               Piscina* piscina);

#endif /* SONUS_WAV_H */
