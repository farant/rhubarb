#ifndef NAVIGATOR_ENTITATUM_H
#define NAVIGATOR_ENTITATUM_H

#include "latina.h"
#include "entitas.h"
#include "entitas_providor.h"
#include "piscina.h"
#include "fenestra.h"
#include "xar.h"

/* ==================================================
 * Structurae
 * ================================================== */

/* Genus item in columna media
 * "Type of item in center column"
 */
nomen enumeratio {
    ITEM_RELATIO,      /* Relatio ad alias entitates */
    ITEM_PROPRIETAS    /* Proprietas (clavis-valor) */
} GenusItem;

/* Item historiae navigationis
 * "History item for navigation path"
 */
nomen structura {
    chorda* entitas_id;        /* ID entitatis parentis */
    chorda* entitas_id_destinatio;  /* ID entitatis quo navigavimus */
} ItemHistoriae;

/* Item ad reddendum in columna media
 * "Item to render in center column"
 */
nomen structura {
    GenusItem genus;
    i32       altitudo;  /* Lineae quas occupat */
    vacuum*   datum;     /* Relatio* vel Proprietas* */
} ItemNavigatoris;

/* Navigator entitatum - Widget pro exploratione graphi
 * "Entity navigator - Widget for graph exploration"
 */
nomen structura {
    EntitasProvidor* providor;
    Piscina*         piscina;

    /* Via navigationis (historia) */
    Xar* via;  /* Xar de ItemHistoriae - dynamic path stack */

    /* Entitas currens */
    Entitas* entitas_currens;

    /* Items pro columna media */
    ItemNavigatoris items[CXXVIII];  /* Capacitas maxima */
    i32             numerus_itemorum;
    i32             selectio;        /* Index item selectus */

    /* Paginatio */
    i32 pagina_currens;
    i32 items_per_pagina;  /* Calculatus ex altitudine columnae */
} NavigatorEntitatum;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare navigatorem novum
 * Si providor habet radices, navigat ad primam radicem
 *
 * Redde: Navigator novus, vel NIHIL si fractura
 */
NavigatorEntitatum*
navigator_entitatum_creare(
    Piscina*         piscina,
    EntitasProvidor* providor);


/* ==================================================
 * Navigatio
 * ================================================== */

/* Navigare ad entitatem per ID
 * Addit entitatem currens ad viam, cargat novam entitatem
 *
 * Redde: VERUM si successus, FALSUM si entitas non inventa
 */
b32
navigator_entitatum_navigare_ad(
    NavigatorEntitatum* nav,
    chorda*             entitas_id);

/* Retro in via (pop historia)
 * Navigat ad entitatem praecendentem in via
 *
 * Redde: VERUM si successus, FALSUM si via vacua
 */
b32
navigator_entitatum_retro(
    NavigatorEntitatum* nav);


/* ==================================================
 * Tractatio Eventuum
 * ================================================== */

/* Tractare eventum (clavis, etc)
 * j/k - movere selectionem
 * l   - intrare relationem / aperire proprietatem
 * h   - retro in via
 *
 * Redde: VERUM si eventum tractatum, FALSUM si non
 */
b32
navigator_entitatum_tractare_eventum(
    NavigatorEntitatum*  nav,
    constans Eventus*    eventus);


/* ==================================================
 * Redditionis
 * ================================================== */

/* Reddere navigatorem (tres columnae)
 * Columna sinistra: entitas parens
 * Columna media: entitas currens (cum selectione)
 * Columna dextra: praeviso item selecti
 * focused: VERUM si widget habet focus (mutat colorem border)
 */
vacuum
navigator_entitatum_reddere(
    NavigatorEntitatum*  nav,
    TabulaPixelorum*     tabula,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo,
    i32                  scala,
    b32                  focused);

#endif /* NAVIGATOR_ENTITATUM_H */
