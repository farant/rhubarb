#ifndef FONS_VISUS_H
#define FONS_VISUS_H

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "widget.h"
#include "entitas_repositorium.h"

/* ==================================================
 * FONS VISUS - Font Glyph Viewer Widget
 *
 * Widget staticum pro visualizare omnes 256 glyphas
 * fontis fons_6x8.
 *
 * PHILOSOPHIA:
 * - Read-only viewer, non interactivus
 * - Monstrat grid 16x16 omnium glypharum
 * - Monstrat codices hex pro singulis glyphiis
 * - Utile pro debugging et inspectione fontis
 *
 * EXEMPLUM:
 *   FonsVisus* visus = fons_visus_creare(piscina);
 *   fons_visus_reddere(visus, tabula, x, y, lat, alt, scala, focused);
 *
 * ================================================== */


/* ==================================================
 * Typi
 * ================================================== */

/* FonsVisus - Widget state */
nomen structura {
    Piscina* piscina;
    i32      scroll_y;     /* Scroll offset si necessarium */
} FonsVisus;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare font viewer
 *
 * piscina: piscina memoriae
 *
 * Redde: FonsVisus* si successus, NIHIL si error
 */
FonsVisus*
fons_visus_creare(
    Piscina* piscina);


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere font viewer
 *
 * visus: font viewer
 * tabula: tabula pixelorum
 * x, y: positio in characteribus
 * latitudo, altitudo: dimensiones widget in characteribus
 * scala: factor scalae fontis
 * focused: an widget habet focus
 */
vacuum
fons_visus_reddere(
    FonsVisus*       visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused);


/* ==================================================
 * Eventus
 * ================================================== */

/* Tractare eventum
 *
 * Widget est read-only, ergo plerumque redde FALSUM.
 *
 * visus: font viewer
 * eventus: eventus tractandus
 *
 * Redde: VERUM si eventus tractatus, FALSUM alioquin
 */
b32
fons_visus_tractare_eventum(
    FonsVisus*        visus,
    constans Eventus* eventus);


/* ==================================================
 * Lifecycle (Init / Status)
 * ================================================== */

/* Initiare widget - registrare commandi
 *
 * ctx: contextus widget cum reg_commandi
 *
 * Vocatur per registrum_widget_initiare_omnes()
 */
vacuum
fons_visus_init(
    ContextusWidget* ctx);

/* Salvare status visus ad entitas
 *
 * visus: font viewer
 * repo: repositorium entitatum
 * entitas_id: identificator entitas
 *
 * NOTA: FonsVisus est read-only, ergo nihil salvare
 */
vacuum
fons_visus_salvare_status(
    FonsVisus*           visus,
    EntitasRepositorium* repo,
    constans character*  entitas_id);

/* Carcare status visus ex entitas
 *
 * visus: font viewer
 * repo: repositorium entitatum
 * entitas_id: identificator entitas
 *
 * NOTA: FonsVisus est read-only, ergo nihil carcare
 */
vacuum
fons_visus_carcare_status(
    FonsVisus*           visus,
    EntitasRepositorium* repo,
    constans character*  entitas_id);


#endif /* FONS_VISUS_H */
