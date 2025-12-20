#ifndef BIBLIA_VISUS_H
#define BIBLIA_VISUS_H

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "biblia.h"

/* ==================================================
 * BIBLIA VISUS - Douay-Rheims Bible Viewer Widget
 *
 * Widget pro legere Sacram Scripturam cum navigatione
 * et paginatione.
 *
 * USUS:
 * - TOC: Tabula contentorum per testamentum
 * - Lectio: Versus paginati, integri (non fracti)
 * - Navigatio: j/k paginae, h/l capitula, Escape ad TOC
 *
 * EXEMPLUM:
 *   BibliaVisus* visus = biblia_visus_creare(piscina);
 *   biblia_visus_reddere(visus, tabula, x, y, lat, alt, scala, focused);
 *
 * ================================================== */


/* ==================================================
 * Typi
 * ================================================== */

/* BibliaVisus - Widget state */
nomen structura {
    Piscina* piscina;
    constans Biblia* biblia;

    /* View state */
    b32 in_toc;              /* VERUM = TOC, FALSUM = lectio */
    i32 liber_currens;       /* Index libri currentis */
    i32 capitulum_currens;   /* Capitulum currens (1-indexed) */
    i32 versus_initium;      /* Primus versus in pagina currenti */

    /* Layout dimensions (set during render) */
    i32 latitudo_characterum; /* Characters per line */
    i32 altitudo_linearum;    /* Lines available */
    i32 scala;                /* Scale factor (for click detection) */
    i32 widget_x;             /* Widget position in characters */
    i32 widget_y;
} BibliaVisus;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare Bible viewer
 *
 * piscina: piscina memoriae
 *
 * Redde: BibliaVisus* si successus, NIHIL si error
 */
BibliaVisus*
biblia_visus_creare(
    Piscina* piscina);


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere Bible viewer
 *
 * visus: Bible viewer
 * tabula: tabula pixelorum
 * x, y: positio in characteribus
 * latitudo, altitudo: dimensiones in characteribus
 * scala: factor scalae fontis
 * focused: an widget habet focus
 */
vacuum
biblia_visus_reddere(
    BibliaVisus*     visus,
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
 * visus: Bible viewer
 * eventus: eventus tractandus
 *
 * Redde: VERUM si eventus tractatus, FALSUM alioquin
 */
b32
biblia_visus_tractare_eventum(
    BibliaVisus*      visus,
    constans Eventus* eventus);


#endif /* BIBLIA_VISUS_H */
