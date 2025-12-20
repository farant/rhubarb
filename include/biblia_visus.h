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

/* Maximum pages in a chapter (176 verses in Psalm 119) */
#define BIBLIA_PAGINAE_MAXIMUS LXIV

/* BibliaVisus - Widget state */
nomen structura {
    Piscina* piscina;
    constans Biblia* biblia;

    /* View state */
    b32 in_toc;              /* VERUM = TOC, FALSUM = lectio */
    i32 liber_currens;       /* Index libri currentis */
    i32 capitulum_currens;   /* Capitulum currens (1-indexed) */
    i32 versus_initium;      /* Primus versus in pagina currenti */
    i32 versus_in_pagina;    /* Versus redditi in pagina currenti */

    /* Pre-calculated pagination for current chapter */
    i32 paginae_limites[BIBLIA_PAGINAE_MAXIMUS];  /* First verse of each page */
    i32 paginae_numerus;     /* Total pages in chapter */
    i32 index_paginae;       /* Current page index (0-based) */

    /* Cache invalidation */
    i32 cache_liber;         /* Cached liber for pagination */
    i32 cache_capitulum;     /* Cached capitulum for pagination */
    i32 cache_latitudo;      /* Cached latitudo for pagination */
    i32 cache_altitudo;      /* Cached altitudo for pagination */

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


/* ==================================================
 * Navigatio
 * ================================================== */

/* Navigate ad referentiam biblicam
 *
 * visus: Bible viewer
 * referentia: "Book", "Book N", vel "Book N:V"
 *             e.g., "Genesis", "Genesis 3", "Genesis 3:16"
 *
 * Redde: VERUM si navigatio successus, FALSUM si liber non inventus
 */
b32
biblia_visus_navigare_ad(
    BibliaVisus* visus,
    constans character* referentia);


#endif /* BIBLIA_VISUS_H */
