#ifndef THEMA_VISUS_H
#define THEMA_VISUS_H

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "widget.h"
#include "entitas_repositorium.h"

/* ==================================================
 * THEMA VISUS - Theme & Pattern Viewer Widget
 *
 * Widget paginatus pro visualizare palette colorum,
 * mappas semanticas, et exemplaria (patterns).
 *
 * PHILOSOPHIA:
 * - Pagina 0: Contrast grid, palette, semantic mappings
 * - Pagina 1: Pattern viewer cum fg/bg color picker
 * - h/l: navigare paginas
 * - f/b: selectare modus fg/bg (in pagina exemplaria)
 * - j/k: cyclare colores (in pagina exemplaria)
 *
 * EXEMPLUM:
 *   ThemaVisus* visus = thema_visus_creare(piscina);
 *   thema_visus_reddere(visus, tabula, x, y, lat, alt, scala, focused);
 *   thema_visus_tractare_eventum(visus, &eventus);
 *
 * ================================================== */


/* ==================================================
 * Typi
 * ================================================== */

/* ThemaVisus - Widget state */
nomen structura {
    Piscina* piscina;
    i32 pagina;           /* 0 = colores, 1 = exemplaria */
    i32 exemplar_fg;      /* foreground color index 0-15 */
    i32 exemplar_bg;      /* background color index 0-15 */
    b32 modus_fg;         /* VERUM = editing fg, FALSUM = editing bg */
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
 * h/l: navigare paginas
 * f/b: selectare modus fg/bg (pagina exemplaria)
 * j/k: cyclare colores (pagina exemplaria)
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
thema_visus_init(
    ContextusWidget* ctx);

/* Salvare status visus ad entitas
 *
 * Salvat: pagina, exemplar_fg, exemplar_bg, modus_fg
 *
 * visus: theme viewer
 * repo: repositorium entitatum
 * entitas_id: identificator entitas (e.g., "ThemaStatus::0")
 */
vacuum
thema_visus_salvare_status(
    ThemaVisus*          visus,
    EntitasRepositorium* repo,
    constans character*  entitas_id);

/* Carcare status visus ex entitas
 *
 * Carcat: pagina, exemplar_fg, exemplar_bg, modus_fg
 *
 * visus: theme viewer
 * repo: repositorium entitatum
 * entitas_id: identificator entitas (e.g., "ThemaStatus::0")
 */
vacuum
thema_visus_carcare_status(
    ThemaVisus*          visus,
    EntitasRepositorium* repo,
    constans character*  entitas_id);


#endif /* THEMA_VISUS_H */
