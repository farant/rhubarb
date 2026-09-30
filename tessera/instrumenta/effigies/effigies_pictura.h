/* effigies_pictura.h - Pars PURA instrumenti effigies (quadrans Q5)
 *
 * Imaginem ad cellulas terminalis aptare (sine distortione) et
 * cellulas quadrantum in TesseraOpus ponere. Decodificatio (imago) et
 * scalatio (imago_opus) ad vocantem pertinent; hic mensurae et
 * adaptator soli. Extra fontes/ et amalgama tesserae consulto (ut
 * folium): tessera = cratis + initus.
 *
 * ASPECTUS (planum D5): nulla quaestio magnitudinis cellulae - aspectus
 * cellulae (latitudo / altitudo) PARAMETRUS est, in centesimis
 * (ordinarie L = 1:2). Sub-pixelum QUADRANTUM dimidia cellula latum
 * et dimidia altum est (cum 1:2 altum, 1:2); DIMIDII cellula tota
 * latum et dimidia altum (cum 1:2 quadratum).
 */

#ifndef EFFIGIES_PICTURA_H
#define EFFIGIES_PICTURA_H

#include "latina.h"
#include "quadrans.h"
#include "tessera_opus.h"

/* cellula 1:2 (latitudo / altitudo), in centesimis */
#define EFFIGIES_ASPECTUS_ORDINARIUS L

/* Sub-pixela ad imaginem img_lat x img_alt pixelorum intra cellulas
 * max_lat x max_alt aptandam (scalatio sursum aut deorsum), ut in
 * scrinio sine distortione appareat: *sub_lat x *sub_alt (>= I si
 * imago non vacua; 0 aliter). */
vacuum
effigies_mensurare (
              i32  img_lat,
              i32  img_alt,
              i32  max_lat,
              i32  max_alt,
    QuadransModus  modus,
              i32  aspectus,
              i32* sub_lat,
              i32* sub_alt);

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

#endif /* EFFIGIES_PICTURA_H */
