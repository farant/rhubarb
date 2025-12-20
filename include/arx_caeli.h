#ifndef ARX_CAELI_H
#define ARX_CAELI_H

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "fenestra.h"
#include "entitas_repositorium.h"
#include "widget.h"

/* ==================================================
 * ARX CAELI - Castellum Nubium (Cloud Castle)
 *
 * Spatialis editor cartarum. Cartae possunt creari,
 * moveri, et in folders organizari.
 *
 * INTERACTIONES:
 * - Double-click vacuum -> creare cartam
 * - Single-click carta -> selectare (x ad delere)
 * - Double-click carta -> modus inserere (editare textum)
 * - Trahere carta -> movere (sine collisione)
 * - Folder cartae (> praefixum) -> navigare
 *
 * EXEMPLUM:
 *   ArcCaeli* arc = arx_caeli_creare(piscina, intern, repo);
 *   arx_caeli_navigare_ad(arc, NIHIL);  // ad radicem
 *   arx_caeli_tractare_eventum(arc, &eventus);
 *   arx_caeli_reddere(arc, tabula, x, y, lat, alt, scala, focused);
 *
 * ================================================== */


/* ==================================================
 * Constantae
 *
 * NOTA DE ARCHITECTURA:
 * Cartae in memoria (array infra) sunt solum visibiles
 * in schirma currenti. Repositorium entitatum continet
 * omnes cartas sine limite - milia cartarum per multas
 * schirmas possibiles sunt. Haec array est "working set"
 * pro UI, non pro persistentia.
 * ================================================== */

#define CARTAE_MAXIMUS      LXIV    /* 64 cartae per schirma visibiles */
#define HISTORIA_MAXIMUS    XVI     /* 16 gradus historiae navigationis */


/* ==================================================
 * Typi
 * ================================================== */

/* Modus interactionis */
nomen enumeratio {
    ARC_MODUS_NORMALIS,     /* Nihil selectum */
    ARC_MODUS_SELECTA,      /* Carta selecta, x ad delere */
    ARC_MODUS_INSERERE      /* Editare textum cartae */
} ArcModus;

/* Callback pro link navigation (#link-name) */
nomen vacuum (*FunctioLinkCallback)(vacuum* datum, constans character* link);

/* Singula carta */
nomen structura {
    chorda id;              /* Entitas ID */
    i32 x;                  /* Positio in pixelis (relativum ad widget) */
    i32 y;                  /* Positio in pixelis */
    i32 latitudo;           /* Latitudo calculata ex textu (in characteribus) */
    i32 altitudo;           /* Altitudo calculata ex textu (in characteribus) */
    i32 z_index;            /* Ordo reddendi (higher = on top) */
    chorda textus;          /* Contentum cartae (multilinea) */
    b32 est_folder;         /* Habet portal_ad relationem */
} Carta;

/* Controller Arcis Caeli */
nomen structura {
    ContextusWidget*     ctx;

    /* Schirma currens */
    chorda schirma_currens_id;      /* Entitas ID schirmae */
    chorda schirma_currens_slug;    /* Slug pro navigatione */

    /* Cartae in schirma currenti */
    Carta cartae[CARTAE_MAXIMUS];
    chorda* entitas_ids[CARTAE_MAXIMUS];  /* Entity ID per carta, NIHIL si nova */
    i32 numerus_cartarum;

    /* Historia navigationis (pro < Retro) */
    chorda historia[HISTORIA_MAXIMUS];
    i32 historia_index;

    /* Status selectionis/editionis */
    ArcModus modus;
    i32 index_selecta;              /* -1 si nihil */
    i32 cursor_linea;               /* Cursor in carta (inserere) */
    i32 cursor_columna;

    /* Buffer editionis (pro inserere modo) */
    character edit_buffer[DXII];    /* 512 characters max */
    i32 edit_longitudo;

    /* Status trahendi */
    b32 trahens;
    i32 trahere_origin_x;
    i32 trahere_origin_y;
    i32 trahere_offset_px_x;        /* Offset in pixelis */
    i32 trahere_offset_px_y;
    i32 trahere_px_x;               /* Positio currens mus in pixelis */
    i32 trahere_px_y;
    i32 trahere_grid_x;             /* Positio snapped ad gridum */
    i32 trahere_grid_y;
    b32 trahere_validum;            /* An positio valida? */
    i32 trahere_hover_folder;       /* Index folderis sub puncto, NIHIL_SELECTA si nihil */

    /* Status immunditiae pro debounced save */
    b32 immundum;
    f64 tempus_immundum;

    /* Widget positio et dimensiones (ponitur in reddere) */
    i32 widget_x;
    i32 widget_y;
    i32 widget_latitudo;
    i32 widget_altitudo;
    i32 scala;                      /* Factor scalae fontis */

    /* Link callback */
    FunctioLinkCallback link_callback;
    vacuum*             link_callback_datum;
} ArcCaeli;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare controller Arcis Caeli
 *
 * ctx: contextus widget cum servitiis communicatis
 *
 * Redde: ArcCaeli* si successus, NIHIL si error
 */
ArcCaeli*
arx_caeli_creare(
    ContextusWidget* ctx);


/* ==================================================
 * Navigatio
 * ================================================== */

/* Navigare ad schirmam per slug
 *
 * arc: controller
 * slug: slug schirmae (NIHIL = radix)
 */
vacuum
arx_caeli_navigare_ad(
    ArcCaeli*         arc,
    constans character* slug);

/* Navigare retro in historia
 *
 * arc: controller
 */
vacuum
arx_caeli_retro(
    ArcCaeli* arc);

/* Ponere callback pro link navigation
 *
 * arc: controller
 * callback: functio vocanda cum link clicca
 * datum: datum transmittendum ad callback
 */
vacuum
arx_caeli_ponere_link_callback(
    ArcCaeli*           arc,
    FunctioLinkCallback callback,
    vacuum*             datum);


/* ==================================================
 * Operationes Cartarum
 * ================================================== */

/* Creare novam cartam ad positionem
 *
 * arc: controller
 * x: positio x in characteribus
 * y: positio y in characteribus
 */
vacuum
arx_caeli_carta_creare(
    ArcCaeli* arc,
    i32       x,
    i32       y);

/* Delere cartam per index
 *
 * arc: controller
 * index: index cartae in array
 */
vacuum
arx_caeli_carta_delere(
    ArcCaeli* arc,
    i32       index);

/* Movere cartam ad novam positionem
 *
 * arc: controller
 * index: index cartae
 * x: nova positio x
 * y: nova positio y
 */
vacuum
arx_caeli_carta_movere(
    ArcCaeli* arc,
    i32       index,
    i32       x,
    i32       y);

/* Verificare collisionem cum aliis cartis
 *
 * arc: controller
 * x, y, lat, alt: rectangulum verificandum
 * ignorare_index: index cartae ignorandae (-1 = nihil)
 *
 * Redde: VERUM si collisio, FALSUM si vacuum
 */
b32
arx_caeli_verificare_collisio(
    ArcCaeli* arc,
    i32       x,
    i32       y,
    i32       latitudo,
    i32       altitudo,
    i32       ignorare_index);


/* ==================================================
 * Widget Interface
 * ================================================== */

/* Reddere Arcem Caeli
 *
 * arc: controller
 * tabula: tabula pixelorum
 * x, y: positio widget in characteribus
 * latitudo, altitudo: dimensiones widget
 * scala: factor scalae
 * focused: an widget habet focus
 */
vacuum
arx_caeli_reddere(
    ArcCaeli*        arc,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused);

/* Tractare eventum
 *
 * arc: controller
 * eventus: eventus tractandus
 *
 * Redde: VERUM si eventus tractatus
 */
b32
arx_caeli_tractare_eventum(
    ArcCaeli*         arc,
    constans Eventus* eventus);


/* ==================================================
 * Persistentia
 * ================================================== */

/* Carcare cartas ex repositorio
 *
 * arc: controller
 */
vacuum
arx_caeli_carcare(
    ArcCaeli* arc);

/* Salvare cartas ad repositorium
 *
 * arc: controller
 */
vacuum
arx_caeli_salvare(
    ArcCaeli* arc);

/* Salvare si immundum et tempus elapsum
 *
 * arc: controller
 */
vacuum
arx_caeli_salvare_si_immundum(
    ArcCaeli* arc);

/* Marcare immundum (triggerat debounced save)
 *
 * arc: controller
 */
vacuum
arx_caeli_marcare_immundum(
    ArcCaeli* arc);


#endif /* ARX_CAELI_H */
