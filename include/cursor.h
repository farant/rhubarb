#ifndef CURSOR_H
#define CURSOR_H

#include "latina.h"
#include "fenestra.h"

/* ==================================================
 * CURSOR - Cursor Muris Proprius
 *
 * Cursor simplex sagitta pro modo plena visio.
 * Usus colores ex thema: PALETTE_BLACK (0) et
 * PALETTE_MEDIUM_GRAY (3).
 *
 * EXEMPLUM:
 *   si (fenestra_est_plena_visio(fenestra))
 *   {
 *       cursor_reddere(tabula, mus_x, mus_y);
 *   }
 *
 * ================================================== */


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere cursorem muris ad positum (x, y) in tabula pixelorum
 *
 * Cursor est sagitta simplex cum colore nigro (PALETTE_BLACK)
 * et margine griseo (PALETTE_MEDIUM_GRAY).
 *
 * Hotspot est ad (0, 0) - angulus superior sinister.
 *
 * tabula: tabula pixelorum in qua reddere
 * x: positio horizontalis in pixelis
 * y: positio verticalis in pixelis
 */
vacuum
cursor_reddere(
    TabulaPixelorum* tabula,
    i32 x,
    i32 y);

#endif /* CURSOR_H */
