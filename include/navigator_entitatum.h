#ifndef NAVIGATOR_ENTITATUM_H
#define NAVIGATOR_ENTITATUM_H

#include "latina.h"
#include "entitas.h"
#include "entitas_repositorium.h"
#include "piscina.h"
#include "fenestra.h"
#include "xar.h"
#include "widget.h"

/* ==================================================
 * Structurae
 * ================================================== */

/* Genus item in columna media
 * "Type of item in center column"
 */
nomen enumeratio {
    ITEM_RELATIO,          /* Relatio ad alias entitates (outbound) */
    ITEM_PROPRIETAS,       /* Proprietas (clavis-valor) */
    ITEM_RELATIO_INVERSA   /* Relatio inversa / backlink (inbound) */
} GenusItem;

/* Item historiae navigationis
 * "History item for navigation path"
 */
nomen structura {
    chorda* entitas_id;             /* ID entitatis parentis */
    chorda* entitas_id_destinatio;  /* ID entitatis quo navigavimus */
    b32     per_backlink;           /* VERUM si navigavimus per backlink */
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
    ContextusWidget*     ctx;

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
 * Si repositorium habet radices, navigat ad primam radicem
 *
 * ctx: contextus widget cum servitiis communicatis
 *
 * Redde: Navigator novus, vel NIHIL si fractura
 */
NavigatorEntitatum*
navigator_entitatum_creare(
    ContextusWidget* ctx);


/* ==================================================
 * Navigatio
 * ================================================== */

/* Navigare ad entitatem per ID
 * Addit entitatem currens ad viam, cargat novam entitatem
 * per_backlink: VERUM si navigatio est per backlink (pro historia)
 *
 * Redde: VERUM si successus, FALSUM si entitas non inventa
 */
b32
navigator_entitatum_navigare_ad(
    NavigatorEntitatum* nav,
    chorda*             entitas_id,
    b32                 per_backlink);

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

/* Reddere navigatorem (duae columnae)
 * Columna sinistra (30%): entitas parens (dimmed)
 * Columna dextra (70%): entitas currens (cum selectione)
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
navigator_entitatum_init(
    ContextusWidget* ctx);

/* Salvare status visus ad entitas
 *
 * nav: navigator entitatum
 * repo: repositorium entitatum
 * entitas_id: identificator entitas (e.g., "NavigatorStatus::0")
 *
 * Salvat: entitas_currens_id, selectio, pagina_currens
 */
vacuum
navigator_entitatum_salvare_status(
    NavigatorEntitatum*  nav,
    EntitasRepositorium* repo,
    constans character*  entitas_id);

/* Carcare status visus ex entitas
 *
 * nav: navigator entitatum
 * repo: repositorium entitatum
 * entitas_id: identificator entitas (e.g., "NavigatorStatus::0")
 *
 * Carcat: navigat ad entitatem, restaurat selectio/pagina
 */
vacuum
navigator_entitatum_carcare_status(
    NavigatorEntitatum*  nav,
    EntitasRepositorium* repo,
    constans character*  entitas_id);


#endif /* NAVIGATOR_ENTITATUM_H */
