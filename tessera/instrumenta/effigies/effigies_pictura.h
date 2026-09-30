/* effigies_pictura.h - Pars PURA instrumenti effigies (quadrans Q5)
 *
 * Cellulas quadrantum in TesseraOpus ponere et imaginem ad paletam
 * reducere. Decodificatio (imago) et
 * scalatio (imago_opus) ad vocantem pertinent; hic mensurae et
 * adaptator soli. Extra fontes/ et amalgama tesserae consulto (ut
 * folium): tessera = cratis + initus.
 *
 * Aptatio (sine distortione) in quadrans_aptare (lib/quadrans).
 */

#ifndef EFFIGIES_PICTURA_H
#define EFFIGIES_PICTURA_H

#include "latina.h"
#include "quadrans.h"
#include "tessera_opus.h"
#include "piscina.h"

/* Cellulas lat x alt (ordine linearum) ab (x, y) in opus ponere:
 * runa -> signum compactum, colores -> stilus. */
vacuum
effigies_pingere (
                 TesseraOpus* opus,
                         s32  x,
                         s32  y,
    constans QuadransCellula* cellulae,
                         i32  lat,
                         i32  alt);

/* Imaginem ad paletam Aquinas (XVI colores, dithering.h) per
 * diffusionem Atkinson reducere (planum D4, modus PALETTAE, quadrans
 * Q6): imago NOVA in piscina, pixela opaca, colores palettae soli. Cum
 * QUADRANS_EXTREMA cellulae in paletta manent (colores = pixela ipsa);
 * MEDIA eos miscet. Imago vacua (pixela NIHIL) si allocatio fracta. */
Imago
effigies_palettam_applicare (
    constans Imago* imago,
           Piscina* piscina);

#endif /* EFFIGIES_PICTURA_H */
