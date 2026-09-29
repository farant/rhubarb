/* runae.c - Nucleus Unicode (vide runae.h)
 *
 * Proprietates ex tabulis generatis (lib/runae_tabulae.c, tools/
 * runae_generare.sh). Quaestio: duo lectiones ordinum, nulla ramificatio
 * praeter limitem runae validae.
 */

#include "runae.h"
#include "runae_tabulae.h"

#define RUNA_MAXIMA 0x10FFFF

i32
runae_latitudo (
    s32 runa)
{
    i32 truncus;

    si (runa < ZEPHYRUM || runa > RUNA_MAXIMA)
    {
        redde I;   /* invalida: U+FFFD pingitur */
    }
    truncus = (i32)RUNAE_GRADUS_PRIMUS[(i32)runa >> VIII];
    redde (i32)(RUNAE_GRADUS_SECUNDUS[truncus * RUNAE_TRUNCUS
        + ((i32)runa & 0xFF)] & RUNAE_LATITUDO_MASCULA);
}
