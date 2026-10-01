/* musivum_pictura.h - Pars PURA instrumenti musivum (tessellatio T5):
 * cellulas tessellationis in TesseraOpus ponere
 *
 * Positio TENUIS et separata (tessellatio D2): nucleus (lib/tessellatio)
 * tesseram nescit; hic unitates per tessera_graphema_ponere (latae,
 * graphemata, regimen -> '?'), juncturae per runam delineandi, cellulae
 * vacuae per spatium cum fundo. Continuatio (latitudo 0) omittitur -
 * unitas lata eam ipsa ponit. Extra fontes/ et amalgama tesserae
 * consulto (ut folium, effigies): tessera = cratis + initus.
 */

#ifndef MUSIVUM_PICTURA_H
#define MUSIVUM_PICTURA_H

#include "latina.h"
#include "tessellatio.h"
#include "tessera_opus.h"

/* Cellulas columnae x lineae (ordine linearum) ab (x, y) in opus
 * ponere: colores -> stilus (ornamenta nulla). */
vacuum
musivum_pingere (
                    TesseraOpus* opus,
                            s32  x,
                            s32  y,
    constans TessellatioCellula* cellulae,
                            s32  columnae,
                            s32  lineae);

#endif /* MUSIVUM_PICTURA_H */
