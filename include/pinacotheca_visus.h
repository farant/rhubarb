#ifndef PINACOTHECA_VISUS_H
#define PINACOTHECA_VISUS_H

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "xar.h"
#include "chorda.h"
#include "widget.h"
#include "entitas_repositorium.h"

/* ==================================================
 * PINACOTHECA VISUS - Gallery Widget
 *
 * Widget pro inspicere imagines dithered (Vultus::Imago).
 * Duo modi: browser (lista + preview) et visus (plena imago).
 *
 * NAVIGATIO:
 * - j/k: sursum/deorsum in lista
 * - Enter: aperire in modo visus
 * - Escape: retro (visus→browser, browser→exire)
 * - d: delere cum confirmatione
 * - r: renominare
 *
 * EXEMPLUM:
 *   PinacothecaVisus* visus = pinacotheca_visus_creare(ctx);
 *   pinacotheca_visus_reddere(visus, tabula, x, y, lat, alt, scala, focused);
 *
 * ================================================== */


/* ==================================================
 * Constantae
 * ================================================== */

#define PINACOTHECA_LATITUDO_LISTAE   XX    /* Latitudo listae in characteribus */


/* ==================================================
 * Enumerationes
 * ================================================== */

/* Modus visus */
nomen enumeratio {
    PINACOTHECA_MODUS_BROWSER,    /* Lista + preview (ranger-style) */
    PINACOTHECA_MODUS_VISUS       /* Plena imago centrata */
} PinacothecaModus;


/* ==================================================
 * Typi
 * ================================================== */

/* Informationes de imagine (cached ex entitas) */
nomen structura {
    chorda  id;           /* Entitas UUID */
    chorda  titulus;
    i32     latitudo;
    i32     altitudo;
} ImagoInfo;

/* Status visus pinacothecae */
nomen structura {
    Piscina*         piscina;
    ContextusWidget* ctx;

    /* Lista imaginum */
    Xar* imagines;            /* Xar de ImagoInfo */
    s32  index_selecta;       /* Index in lista */
    s32  index_paginae;       /* Prima imago visibilis in lista */

    /* Modus */
    PinacothecaModus modus;

    /* Cache pro imagine selecta */
    i8*  datum_cache;         /* Palette indices (decompressed) */
    i32  cache_latitudo;
    i32  cache_altitudo;
    chorda cache_id;          /* Id imaginis in cache */

    /* Dimensiones widget (salvati pro eventus tractatio) */
    s32 latitudo_characterum;
    s32 altitudo_linearum;
    s32 scala;
    s32 widget_x;
    s32 widget_y;

    /* Dialog state (pro delete/rename) */
    b32 confirmare_delere;    /* Monstrare confirmationem */
    b32 renominare_activum;   /* Text input activum */
    chorda novum_nomen;       /* Buffer pro renominare */
    s32 novum_nomen_cursor;
} PinacothecaVisus;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare pinacotheca viewer
 *
 * ctx: contextus widget (pro entity repository)
 *
 * Redde: PinacothecaVisus* si successus, NIHIL si error
 */
PinacothecaVisus*
pinacotheca_visus_creare(
    ContextusWidget* ctx);


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere pinacotheca viewer
 *
 * visus: pinacotheca viewer
 * tabula: tabula pixelorum
 * x, y: positio in characteribus
 * latitudo, altitudo: dimensiones in characteribus
 * scala: factor scalae fontis
 * focused: an widget habet focus
 */
vacuum
pinacotheca_visus_reddere(
    PinacothecaVisus* visus,
    TabulaPixelorum*  tabula,
    i32               x,
    i32               y,
    i32               latitudo,
    i32               altitudo,
    i32               scala,
    b32               focused);


/* ==================================================
 * Eventus
 * ================================================== */

/* Tractare eventum
 *
 * visus: pinacotheca viewer
 * eventus: eventus tractandus
 *
 * Redde: VERUM si eventus tractatus, FALSUM alioquin
 */
b32
pinacotheca_visus_tractare_eventum(
    PinacothecaVisus* visus,
    constans Eventus* eventus);


/* ==================================================
 * Navigatio
 * ================================================== */

/* Reficere listam imaginum ex repositorium
 *
 * visus: pinacotheca viewer
 *
 * Quaerit omnes Vultus::Imago et sortat alphabetice.
 */
vacuum
pinacotheca_visus_reficere(
    PinacothecaVisus* visus);

/* Navigare ad imaginem per titulum
 *
 * visus: pinacotheca viewer
 * titulus: titulus imaginis
 *
 * Si inventum: selectat et monstra in preview/visus
 */
vacuum
pinacotheca_visus_navigare_ad(
    PinacothecaVisus*   visus,
    constans character* titulus);


/* ==================================================
 * Lifecycle
 * ================================================== */

/* Initiare widget - registrare commandi
 *
 * ctx: contextus widget cum reg_commandi
 *
 * Registrat: $pinacotheca, $image
 */
vacuum
pinacotheca_visus_init(
    ContextusWidget* ctx);


#endif /* PINACOTHECA_VISUS_H */
