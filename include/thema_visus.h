#ifndef THEMA_VISUS_H
#define THEMA_VISUS_H

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"

/* ==================================================
 * THEMA VISUS - Theme Palette Viewer Widget
 *
 * Widget static pro visualizare palette colorum et
 * mappas semanticas themae currentis.
 *
 * PHILOSOPHIA:
 * - Read-only viewer, non interactivus
 * - Monstrat omnes XVI colores palette
 * - Monstrat mappas semanticas (Background, Text, etc.)
 * - Utile pro debugging et design themae
 *
 * EXEMPLUM:
 *   ThemaVisus* visus = thema_visus_creare(piscina);
 *   thema_visus_reddere(visus, tabula, x, y, lat, alt, scala, focused);
 *
 * ================================================== */


/* ==================================================
 * Typi
 * ================================================== */

/* ThemaVisus - Widget state */
nomen structura {
    Piscina* piscina;
} ThemaVisus;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare theme viewer
 *
 * piscina: piscina memoriae
 *
 * Redde: ThemaVisus* si successus, NIHIL si error
 */
ThemaVisus*
thema_visus_creare(
    Piscina* piscina);


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere theme viewer
 *
 * visus: theme viewer
 * tabula: tabula pixelorum
 * x, y: positio in tabula
 * latitudo, altitudo: dimensiones widget
 * scala: factor scalae fontis
 * focused: an widget habet focus
 */
vacuum
thema_visus_reddere(
    ThemaVisus*      visus,
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
 * visus: theme viewer
 * eventus: eventus tractandus
 *
 * Redde: VERUM si eventus tractatus, FALSUM alioquin
 */
b32
thema_visus_tractare_eventum(
    ThemaVisus*       visus,
    constans Eventus* eventus);


#endif /* THEMA_VISUS_H */
