#ifndef LIBRARIUM_VISUS_H
#define LIBRARIUM_VISUS_H

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "xar.h"
#include "chorda.h"
#include "widget.h"

/* ==================================================
 * LIBRARIUM VISUS - Library Viewer Widget
 *
 * Widget pro inspicere et legere libros e librarium.stml
 * cum paginatione (sine scrolling).
 *
 * USUS:
 * - CATEGORIAE: Menu principale (Omnes, Per Titulum, Per Tag)
 * - TAGS: Lista tagorum unicorum
 * - CATALOGO: Lista librorum (compacta vel plena)
 * - LIBRO: Singulus liber cum metadatis
 * - LECTIO: Textus libri paginatus
 *
 * NAVIGATIO:
 * - j/k: sursum/deorsum vel paginae
 * - h/l: paginae in catalogo compacto
 * - Enter: selectare / aperire
 * - Escape: retro
 *
 * EXEMPLUM:
 *   LibrariumVisus* visus = librarium_visus_creare(piscina, ctx);
 *   librarium_visus_reddere(visus, tabula, x, y, lat, alt, scala, focused);
 *
 * ================================================== */


/* ==================================================
 * Constantae
 * ================================================== */

#define LIBRARIUM_PAGINAE_MAX    CCLVI     /* 256 - Maximus paginarum in libro */
#define LIBRARIUM_VIA_STML       "librarium.stml"


/* ==================================================
 * Enumerationes
 * ================================================== */

/* Modus visus librarii */
nomen enumeratio {
    LIBRARIUM_MODUS_CATEGORIAE,   /* Menu principale */
    LIBRARIUM_MODUS_TAGS,         /* Lista tagorum */
    LIBRARIUM_MODUS_CATALOGO,     /* Lista librorum */
    LIBRARIUM_MODUS_LIBRO,        /* Singulus liber */
    LIBRARIUM_MODUS_LECTIO        /* Lectio textus */
} LibrariumModus;

/* Ordo sortationis */
nomen enumeratio {
    LIBRARIUM_ORDO_CHRONOLOGICUS, /* Per annum */
    LIBRARIUM_ORDO_ALPHABETICUS   /* Per titulum */
} LibrariumOrdo;

/* Selectio categoriae */
nomen enumeratio {
    LIBRARIUM_CAT_OMNES,          /* Omnes libri */
    LIBRARIUM_CAT_PER_TITULUM,    /* Alphabetice */
    LIBRARIUM_CAT_PER_TAG         /* Per tag */
} LibrariumCategoria;


/* ==================================================
 * Typi
 * ================================================== */

/* Informationes de libro */
nomen structura {
    chorda numerus;      /* Gutenberg ID */
    chorda titulus;
    chorda auctor;
    chorda annus;
    chorda summarium;
    chorda notae;
    chorda via;          /* Via ad filum textus */
    Xar* tags;           /* Xar de chorda */
} LibrumInfo;

/* Status visus librarii */
nomen structura {
    Piscina* piscina;
    ContextusWidget* ctx;

    /* Omnes libri ex librarium.stml */
    Xar* libri;              /* Xar de LibrumInfo* */
    Xar* tags_unici;         /* Xar de chorda - tags unici */

    /* Status navigationis */
    LibrariumModus modus;
    LibrariumOrdo ordo;
    LibrariumCategoria categoria;
    chorda filtrum_tag;      /* Tag currens pro filtro (mensura=0 = nullum) */
    Xar* libri_filtrati;     /* Vista filtrata/sortata in libri */
    chorda filtrum_quaestio; /* Quaestio quaerendi */

    /* Paginatio catalogi */
    s32 index_paginae;       /* Pagina currens in catalogo */
    s32 index_selecta;       /* Item selectum */

    /* Status libri / lectionis */
    s32 liber_currens;       /* Index in libri_filtrati */
    chorda textus_libri;     /* Textus carcatus */
    s32 pagina_lectio;       /* Pagina currens lectionis */
    s32 paginae_totales;     /* Totales paginae in libro */

    /* Cache paginationis pro lectione */
    s32 paginae_limites[LIBRARIUM_PAGINAE_MAX];
    s32 cache_liber;
    s32 cache_latitudo;
    s32 cache_altitudo;

    /* Dimensiones widget */
    s32 latitudo_characterum;
    s32 altitudo_linearum;
    s32 scala;
    s32 widget_x;
    s32 widget_y;
} LibrariumVisus;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare library viewer
 *
 * piscina: piscina memoriae
 * ctx: contextus widget (pro entity repository)
 *
 * Redde: LibrariumVisus* si successus, NIHIL si error
 */
LibrariumVisus*
librarium_visus_creare(
    Piscina*        piscina,
    ContextusWidget* ctx);


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere library viewer
 *
 * visus: library viewer
 * tabula: tabula pixelorum
 * x, y: positio in characteribus
 * latitudo, altitudo: dimensiones in characteribus
 * scala: factor scalae fontis
 * focused: an widget habet focus
 */
vacuum
librarium_visus_reddere(
    LibrariumVisus*  visus,
    TabulaPixelorum* tabula,
    s32              x,
    s32              y,
    s32              latitudo,
    s32              altitudo,
    s32              scala,
    b32              focused);


/* ==================================================
 * Eventus
 * ================================================== */

/* Tractare eventum
 *
 * visus: library viewer
 * eventus: eventus tractandus
 *
 * Redde: VERUM si eventus tractatus, FALSUM alioquin
 */
b32
librarium_visus_tractare_eventum(
    LibrariumVisus*   visus,
    constans Eventus* eventus);


/* ==================================================
 * Quaerere
 * ================================================== */

/* Quaerere librum per titulum
 *
 * visus: library viewer
 * quaestio: textus quaerendi
 *
 * Si 1 inventum: salta ad MODUS_LIBRO
 * Si plures: monstra MODUS_CATALOGO cum filtro
 * Si 0: nihil mutare
 */
vacuum
librarium_visus_quaerere(
    LibrariumVisus*     visus,
    constans character* quaestio);


#endif /* LIBRARIUM_VISUS_H */
