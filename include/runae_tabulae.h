/* runae_tabulae.h - Tabulae GENERATAE runarum (runae)
 *
 * lib/runae_tabulae.c a tools/runae_generare.sh scribitur ex datis
 * Unicode fixis; hoc caput manu scriptum et stabile est. Tabula duorum
 * graduum: GRADUS_PRIMUS[runa >> VIII] = index truncii; truncus
 * CCLVI octetorum in GRADUS_SECUNDUS (trunci identici semel servati).
 * Valor octeti: bits 0-1 latitudo (0-II); ceteri reservati.
 */

#ifndef RUNAE_TABULAE_H
#define RUNAE_TABULAE_H

#include "latina.h"

#define RUNAE_TRUNCUS 256
#define RUNAE_TRUNCI_PRIMI 4352          /* 0x110000 / RUNAE_TRUNCUS */
#define RUNAE_LATITUDO_MASCULA 0x03

externus constans i16 RUNAE_GRADUS_PRIMUS[RUNAE_TRUNCI_PRIMI];
externus constans i8  RUNAE_GRADUS_SECUNDUS[];

#endif /* RUNAE_TABULAE_H */
