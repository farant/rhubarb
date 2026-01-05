#include "schirmata.h"
#include "fons_visus.h"
#include "thema.h"
#include "delineare.h"
#include "tempus.h"
#include "fasti.h"
#include "calendarium_liturgicum.h"
#include "chorda_aedificator.h"
#include "clipboard_platform.h"
#include "dialogus_importatio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Timeout pro praefixum ctrl-a (secundae) */
#define PRAEFIXUM_TIMEOUT  2.0

/* ==================================================
 * Datum Internum pro Widgets
 * ================================================== */

/* Datum pro libro widget wrapper */
nomen structura {
    LibroPaginarum* libro;
    Schirmata*      schirmata;  /* Pro accedere reg_commandi */
} SchirmataLibroDatum;

/* Datum pro navigator widget wrapper */
nomen structura {
    NavigatorEntitatum* navigator;
    Piscina*            piscina;
} SchirmataNavigatorDatum;


/* ==================================================
 * Widget Commutare Callback
 *
 * Haec functio vocatur per widgets (via ctx->commutare_widget)
 * ad commutare inter modi widgetorum.
 * ================================================== */

hic_manens vacuum
_schirmata_commutare_widget_callback(
    vacuum*             schirmata_datum,
    constans character* widget_titulus,
    constans character* argumentum)
{
    Schirmata* schirmata;

    schirmata = (Schirmata*)schirmata_datum;
    si (!schirmata || !widget_titulus)
    {
        redde;
    }

    /* Mappare widget_titulus ad functionem commutandi */
    si (strcmp(widget_titulus, "biblia") == ZEPHYRUM)
    {
        schirmata_commutare_ad_biblia_visus(schirmata);
        si (argumentum)
        {
            biblia_visus_navigare_ad(schirmata->biblia_visus, argumentum);
        }
    }
    alioquin si (strcmp(widget_titulus, "librarium") == ZEPHYRUM)
    {
        schirmata_commutare_ad_librarium(schirmata, argumentum);
    }
    alioquin si (strcmp(widget_titulus, "thema") == ZEPHYRUM)
    {
        schirmata_commutare_ad_thema_visus(schirmata);
    }
    alioquin si (strcmp(widget_titulus, "sputnik") == ZEPHYRUM)
    {
        schirmata_commutare_ad_sputnik_syntaxis(schirmata);
    }
    alioquin si (strcmp(widget_titulus, "arx_caeli") == ZEPHYRUM)
    {
        schirmata_commutare_ad_arx_caeli(schirmata, argumentum);
    }
    alioquin si (strcmp(widget_titulus, "navigator") == ZEPHYRUM)
    {
        schirmata_commutare_ad_navigator(schirmata);
    }
    alioquin si (strcmp(widget_titulus, "fons") == ZEPHYRUM)
    {
        schirmata_commutare_ad_fons_visus(schirmata);
    }
    alioquin si (strcmp(widget_titulus, "calendario") == ZEPHYRUM)
    {
        schirmata_commutare_ad_calendario_visus(schirmata);
    }
}


/* ==================================================
 * Arx Caeli Link Callback
 * ================================================== */

hic_manens vacuum
_arx_caeli_link_callback(
    vacuum*             datum,
    constans character* link)
{
    Schirmata* schirmata;

    schirmata = (Schirmata*)datum;
    si (!schirmata || !schirmata->libro || !link)
    {
        redde;
    }

    /* Navigare ad paginam per nomen */
    libro_navigare_ad_nomen(schirmata->libro, link);
}


/* ==================================================
 * Widget Wrapper Functiones - Libro
 * ================================================== */

hic_manens vacuum
_schirmata_libro_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataLibroDatum* datum;

    datum = (SchirmataLibroDatum*)widget->datum;

    libro_reddere(
        datum->schirmata->ctx->piscina,
        tabula,
        datum->libro,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_libro_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataLibroDatum* datum;
    Pagina* pagina;

    datum = (SchirmataLibroDatum*)widget->datum;

    /* Tractare keyboard navigation */
    si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        /* Ctrl+Shift+Right -> pagina proxima */
        si ((eventus->datum.clavis.modificantes & MOD_IMPERIUM) &&
            (eventus->datum.clavis.modificantes & MOD_SHIFT))
        {
            si (eventus->datum.clavis.clavis == CLAVIS_DEXTER)
            {
                libro_pagina_proxima(datum->libro);
                redde VERUM;
            }
            /* Ctrl+Shift+Left -> pagina prior */
            si (eventus->datum.clavis.clavis == CLAVIS_SINISTER)
            {
                libro_pagina_prior(datum->libro);
                redde VERUM;
            }
        }
    }

    /* Tractare mouse clicks pro tag detection (commands et links) */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS && datum->schirmata->ctx->reg_commandi)
    {
        RegioClicca regio;
        i32 click_x;
        i32 click_y;
        i32 character_latitudo;
        i32 character_altitudo;

        pagina = libro_pagina_currens(datum->libro);
        si (!pagina)
        {
            redde VERUM;
        }

        character_latitudo = VI;   /* 6 pixels per character */
        character_altitudo = VIII; /* 8 pixels per character */

        /* Convertere pixel ad character coordinates */
        click_x = (eventus->datum.mus.x / character_latitudo) - widget->x - I;
        click_y = (eventus->datum.mus.y / character_altitudo) - widget->y - I;

        /* Tentare detegere tag ad click position */
        si (pagina_obtinere_regio_ad_punctum(pagina, click_y, click_x, &regio))
        {
            si (strcmp(regio.genus, "command") == ZEPHYRUM)
            {
                ContextusCommandi ctx;

                ctx.pagina = pagina;
                ctx.linea = regio.finis_linea;
                ctx.columna = regio.finis_columna;
                ctx.piscina = datum->schirmata->ctx->piscina;
                ctx.datum_custom = datum->libro;

                registrum_commandi_executare(datum->schirmata->ctx->reg_commandi, regio.datum, &ctx);

                redde VERUM;
            }
            alioquin si (strcmp(regio.genus, "link") == ZEPHYRUM)
            {
                constans character* link = regio.datum;

                si (link[ZEPHYRUM] == '#')
                {
                    link++;

                    si (strcmp(link, "back") == ZEPHYRUM)
                    {
                        libro_retro(datum->libro);
                    }
                    alioquin si (strcmp(link, "next") == ZEPHYRUM)
                    {
                        libro_pagina_proxima(datum->libro);
                    }
                    alioquin si (strcmp(link, "prev") == ZEPHYRUM)
                    {
                        libro_pagina_prior(datum->libro);
                    }
                    alioquin si (strcmp(link, "last") == ZEPHYRUM)
                    {
                        libro_pagina_ultima(datum->libro);
                    }
                    alioquin si (link[ZEPHYRUM] >= '0' && link[ZEPHYRUM] <= '9')
                    {
                        s32 page_num = atoi(link) - I;
                        libro_navigare_ad(datum->libro, page_num);
                    }
                    alioquin
                    {
                        libro_navigare_ad_nomen(datum->libro, link);
                    }
                    redde VERUM;
                }
            }
        }
    }

    /* Delegare ad libro tractare_eventum */
    redde libro_tractare_eventum(datum->libro, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Navigator
 * ================================================== */

hic_manens vacuum
_schirmata_navigator_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataNavigatorDatum* datum;

    datum = (SchirmataNavigatorDatum*)widget->datum;

    navigator_entitatum_reddere(
        datum->navigator,
        tabula,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_navigator_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataNavigatorDatum* datum;

    datum = (SchirmataNavigatorDatum*)widget->datum;

    redde navigator_entitatum_tractare_eventum(datum->navigator, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Arx Caeli
 * ================================================== */

/* Datum pro arx caeli widget wrapper */
nomen structura {
    ArcCaeli*  arx_caeli;
    Schirmata* schirmata;
} SchirmataArcCaeliDatum;

hic_manens vacuum
_schirmata_arx_caeli_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataArcCaeliDatum* datum;

    datum = (SchirmataArcCaeliDatum*)widget->datum;

    arx_caeli_reddere(
        datum->arx_caeli,
        tabula,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_arx_caeli_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataArcCaeliDatum* datum;

    datum = (SchirmataArcCaeliDatum*)widget->datum;

    redde arx_caeli_tractare_eventum(datum->arx_caeli, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Thema Visus
 * ================================================== */

/* Datum pro thema visus widget wrapper */
nomen structura {
    ThemaVisus* thema_visus;
    Schirmata*  schirmata;
} SchirmataThemaVisusDatum;

hic_manens vacuum
_schirmata_thema_visus_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataThemaVisusDatum* datum;

    datum = (SchirmataThemaVisusDatum*)widget->datum;

    thema_visus_reddere(
        datum->thema_visus,
        tabula,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_thema_visus_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataThemaVisusDatum* datum;

    datum = (SchirmataThemaVisusDatum*)widget->datum;

    redde thema_visus_tractare_eventum(datum->thema_visus, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Sputnik Syntaxis
 * ================================================== */

/* Datum pro sputnik syntaxis widget wrapper */
nomen structura {
    SputnikSyntaxis* sputnik_syntaxis;
    Schirmata*       schirmata;
} SchirmataSputnikSyntaxisDatum;

hic_manens vacuum
_schirmata_sputnik_syntaxis_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataSputnikSyntaxisDatum* datum;

    datum = (SchirmataSputnikSyntaxisDatum*)widget->datum;

    sputnik_syntaxis_reddere(
        datum->sputnik_syntaxis,
        tabula,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_sputnik_syntaxis_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataSputnikSyntaxisDatum* datum;

    datum = (SchirmataSputnikSyntaxisDatum*)widget->datum;

    redde sputnik_syntaxis_tractare_eventum(datum->sputnik_syntaxis, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Biblia Visus
 * ================================================== */

/* Datum pro biblia visus widget wrapper */
nomen structura {
    BibliaVisus* biblia_visus;
    Schirmata*   schirmata;
} SchirmataBibliaVisusDatum;

hic_manens vacuum
_schirmata_biblia_visus_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataBibliaVisusDatum* datum;

    datum = (SchirmataBibliaVisusDatum*)widget->datum;

    biblia_visus_reddere(
        datum->biblia_visus,
        tabula,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_biblia_visus_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataBibliaVisusDatum* datum;

    datum = (SchirmataBibliaVisusDatum*)widget->datum;

    redde biblia_visus_tractare_eventum(datum->biblia_visus, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Librarium Visus
 * ================================================== */

/* Datum pro librarium visus widget wrapper */
nomen structura {
    LibrariumVisus* librarium_visus;
    Schirmata*      schirmata;
} SchirmataLibrariumVisusDatum;

/* Datum pro fons visus widget wrapper */
nomen structura {
    FonsVisus*  fons_visus;
    Schirmata*  schirmata;
} SchirmataFonsVisusDatum;

/* Datum pro calendario visus widget wrapper */
nomen structura {
    CalendarioVisus*  calendario_visus;
    Schirmata*        schirmata;
} SchirmataCalendarioVisusDatum;

/* Datum pro importatio visus widget wrapper */
nomen structura {
    ImportatioVisus*  importatio_visus;
    Schirmata*        schirmata;
} SchirmataImportatioVisusDatum;

hic_manens vacuum
_schirmata_librarium_visus_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataLibrariumVisusDatum* datum;

    datum = (SchirmataLibrariumVisusDatum*)widget->datum;

    librarium_visus_reddere(
        datum->librarium_visus,
        tabula,
        (s32)x,
        (s32)y,
        (s32)latitudo,
        (s32)altitudo,
        (s32)scala,
        focused);
}

hic_manens b32
_schirmata_librarium_visus_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataLibrariumVisusDatum* datum;

    datum = (SchirmataLibrariumVisusDatum*)widget->datum;

    redde librarium_visus_tractare_eventum(datum->librarium_visus, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Fons Visus
 * ================================================== */

hic_manens vacuum
_schirmata_fons_visus_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataFonsVisusDatum* datum;

    datum = (SchirmataFonsVisusDatum*)widget->datum;

    fons_visus_reddere(
        datum->fons_visus,
        tabula,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_fons_visus_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataFonsVisusDatum* datum;

    datum = (SchirmataFonsVisusDatum*)widget->datum;

    redde fons_visus_tractare_eventum(datum->fons_visus, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Calendario Visus
 * ================================================== */

hic_manens vacuum
_schirmata_calendario_visus_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataCalendarioVisusDatum* datum;

    datum = (SchirmataCalendarioVisusDatum*)widget->datum;

    calendario_visus_reddere(
        datum->calendario_visus,
        tabula,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_calendario_visus_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataCalendarioVisusDatum* datum;

    datum = (SchirmataCalendarioVisusDatum*)widget->datum;

    redde calendario_visus_tractare_eventum(datum->calendario_visus, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Importatio Visus
 * ================================================== */

hic_manens vacuum
_schirmata_importatio_visus_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataImportatioVisusDatum* datum;

    datum = (SchirmataImportatioVisusDatum*)widget->datum;

    importatio_visus_reddere(
        datum->importatio_visus,
        tabula,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_importatio_visus_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataImportatioVisusDatum* datum;

    datum = (SchirmataImportatioVisusDatum*)widget->datum;

    redde importatio_visus_tractare_eventum(datum->importatio_visus, eventus);
}


/* ==================================================
 * Status Salvare / Restituere
 * ================================================== */

hic_manens vacuum
_salvare_status(
    Schirma*        schirma,
    LibroPaginarum* libro)
{
    Pagina* pagina;

    /* Salvare index paginae */
    schirma->libro_status.index_paginae = libro->index_currens;

    /* Salvare cursor et modo ex pagina currenti */
    pagina = libro_pagina_currens(libro);
    si (pagina)
    {
        schirma->libro_status.cursor_linea = pagina->vim.cursor_linea;
        schirma->libro_status.cursor_columna = pagina->vim.cursor_columna;
        schirma->libro_status.modo = pagina->vim.modo;
    }
}

hic_manens vacuum
_restituere_status(
    Schirma*        schirma,
    LibroPaginarum* libro)
{
    Pagina* pagina;

    /* Navigare ad pagina salvata */
    libro_navigare_ad(libro, (s32)schirma->libro_status.index_paginae);

    /* Restituere cursor et modo */
    pagina = libro_pagina_currens(libro);
    si (pagina)
    {
        pagina->vim.cursor_linea = schirma->libro_status.cursor_linea;
        pagina->vim.cursor_columna = schirma->libro_status.cursor_columna;
        pagina->vim.modo = schirma->libro_status.modo;
    }
}

/* Salvare widget status pro schirma specifica ad entitas */
hic_manens vacuum
_salvare_widget_status(
    Schirmata* schirmata,
    i32        schirma_index)
{
    Schirma* schirma;
    character entitas_id[XXXII];

    si (!schirmata || !schirmata->ctx || !schirmata->ctx->repo)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirma_index];

    /* Salvare status basatus in modo activo */
    si (schirma->modus_arx_caeli)
    {
        sprintf(entitas_id, "ArcCaeliStatus::%d", schirma_index);
        arx_caeli_salvare_status(schirmata->arx_caeli, schirmata->ctx->repo, entitas_id);
    }
    alioquin si (schirma->modus_biblia_visus)
    {
        sprintf(entitas_id, "BibliaStatus::%d", schirma_index);
        biblia_visus_salvare_status(schirmata->biblia_visus, schirmata->ctx->repo, entitas_id);
    }
    alioquin si (schirma->modus_librarium)
    {
        sprintf(entitas_id, "LibrariumStatus::%d", schirma_index);
        librarium_visus_salvare_status(schirmata->librarium_visus, schirmata->ctx->repo, entitas_id);
    }
    alioquin si (schirma->modus_thema_visus)
    {
        sprintf(entitas_id, "ThemaStatus::%d", schirma_index);
        thema_visus_salvare_status(schirmata->thema_visus, schirmata->ctx->repo, entitas_id);
    }
    /* SputnikSyntaxis est read-only - nihil salvare */
}

/* Carcare widget status pro schirma specifica ex entitas */
hic_manens vacuum
_carcare_widget_status(
    Schirmata* schirmata,
    i32        schirma_index)
{
    Schirma* schirma;
    character entitas_id[XXXII];

    si (!schirmata || !schirmata->ctx || !schirmata->ctx->repo)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirma_index];

    /* Carcare status basatus in modo activo */
    si (schirma->modus_arx_caeli)
    {
        sprintf(entitas_id, "ArcCaeliStatus::%d", schirma_index);
        arx_caeli_carcare_status(schirmata->arx_caeli, schirmata->ctx->repo, entitas_id);
    }
    alioquin si (schirma->modus_biblia_visus)
    {
        sprintf(entitas_id, "BibliaStatus::%d", schirma_index);
        biblia_visus_carcare_status(schirmata->biblia_visus, schirmata->ctx->repo, entitas_id);
    }
    alioquin si (schirma->modus_librarium)
    {
        sprintf(entitas_id, "LibrariumStatus::%d", schirma_index);
        librarium_visus_carcare_status(schirmata->librarium_visus, schirmata->ctx->repo, entitas_id);
    }
    alioquin si (schirma->modus_thema_visus)
    {
        sprintf(entitas_id, "ThemaStatus::%d", schirma_index);
        thema_visus_carcare_status(schirmata->thema_visus, schirmata->ctx->repo, entitas_id);
    }
    /* SputnikSyntaxis est read-only - nihil carcare */
}


/* ==================================================
 * Creare Layout pro Schirma
 * ================================================== */

hic_manens b32
_creare_schirma_layout(
    Schirmata* schirmata,
    i32        index)
{
    Schirma*                 schirma;
    ManagerWidget*           manager;
    SchirmataLibroDatum*     libro_datum;
    SchirmataNavigatorDatum* nav_datum;
    NavigatorEntitatum*      navigator;

    schirma = &schirmata->schirmae[index];

    /* Creare manager pro hac schirma */
    manager = manager_widget_creare(schirmata->ctx->piscina);
    si (!manager)
    {
        redde FALSUM;
    }
    schirma->manager = manager;

    /* Creare libro widget datum */
    libro_datum = piscina_allocare(schirmata->ctx->piscina, magnitudo(SchirmataLibroDatum));
    si (!libro_datum)
    {
        redde FALSUM;
    }
    libro_datum->libro = schirmata->libro;
    libro_datum->schirmata = schirmata;

    /* Registrare libro widget (sinistra medietas) */
    /* altitudo = 59 (reservare unum versum pro tabula schirmarum) */
    manager_widget_registrare(
        manager,
        libro_datum,
        _schirmata_libro_reddere,
        _schirmata_libro_tractare_eventum,
        ZEPHYRUM,   /* x */
        ZEPHYRUM,   /* y */
        LXXI,       /* latitudo = 71 */
        LIX);       /* altitudo = 59 */

    /* Creare navigator si repositorium disponibilis */
    si (schirmata->ctx->repo)
    {
        navigator = navigator_entitatum_creare(schirmata->ctx);
        si (navigator)
        {
            nav_datum = piscina_allocare(schirmata->ctx->piscina, magnitudo(SchirmataNavigatorDatum));
            si (nav_datum)
            {
                nav_datum->navigator = navigator;
                nav_datum->piscina = schirmata->ctx->piscina;

                /* Registrare navigator widget (dextra medietas) */
                /* altitudo = 59 (reservare unum versum pro tabula schirmarum) */
                manager_widget_registrare(
                    manager,
                    nav_datum,
                    _schirmata_navigator_reddere,
                    _schirmata_navigator_tractare_eventum,
                    LXXI,   /* x = 71 */
                    ZEPHYRUM,
                    LXXI,   /* latitudo = 71 */
                    LIX);   /* altitudo = 59 */
            }
        }
    }

    /* Initiare libro status */
    schirma->libro_status.index_paginae = ZEPHYRUM;
    schirma->libro_status.cursor_linea = ZEPHYRUM;
    schirma->libro_status.cursor_columna = ZEPHYRUM;
    schirma->libro_status.modo = MODO_VIM_NORMALIS;

    /* Initiare modi flags - omnes ad modus navigator (default) */
    schirma->modus_arx_caeli = FALSUM;
    schirma->modus_thema_visus = FALSUM;
    schirma->modus_sputnik_syntaxis = FALSUM;
    schirma->modus_biblia_visus = FALSUM;
    schirma->modus_librarium = FALSUM;
    schirma->modus_fons_visus = FALSUM;

    /* Initiare dialogum */
    schirma->dialogus = NIHIL;
    schirma->dialogus_panel = ZEPHYRUM;

    schirma->initiatus = VERUM;

    redde VERUM;
}


/* ==================================================
 * Tab Bar Rendering
 * ================================================== */

hic_manens vacuum
_reddere_tabulam_schirmarum(
    Schirmata*       schirmata,
    TabulaPixelorum* tabula,
    i32              scala)
{
    i32 character_latitudo;
    i32 character_altitudo;
    i32 tab_y;
    i32 tab_x;
    i32 tab_width;
    i32 margin_right;
    i32 i;
    character buffer[IV];
    chorda label;
    Color color_text_normal;
    Color color_text_activum;
    Color color_fondum_bar;
    Color color_fondum_activum;

    character_latitudo = VI * scala;
    character_altitudo = VIII * scala;
    tab_width = III * character_latitudo;  /* "[N]" = 3 characters */
    margin_right = VIII;  /* 8 pixels margin */

    /* Tab bar ad fundum fenestrae (extra pixel altitudo) */
    tab_y = tabula->altitudo - character_altitudo - I;
    tab_x = II;

    color_text_normal = thema_color(COLOR_BACKGROUND);      /* Warm gray text */
    color_text_activum = color_ex_palette(PALETTE_DARK_RED);  /* Bright pink text */
    color_fondum_bar = color_ex_palette(PALETTE_DARK_GRAY); /* Dark gray bar */
    color_fondum_activum = color_ex_palette(PALETTE_BLUE);  /* Blue background */

    /* Pingere fondum tab bar (tota linea, extra pixel altitudo) */
    {
        i32 py, px;
        i32 bar_altitudo;

        bar_altitudo = character_altitudo + I;  /* Extra pixel */

        per (py = ZEPHYRUM; py < bar_altitudo; py++)
        {
            per (px = ZEPHYRUM; px < tabula->latitudo; px++)
            {
                si (tab_y + py >= ZEPHYRUM && tab_y + py < tabula->altitudo)
                {
                    tabula->pixela[(tab_y + py) * tabula->latitudo + px] =
                        color_ad_pixelum(color_fondum_bar);
                }
            }
        }
    }

    /* Pingere tabs */
    per (i = ZEPHYRUM; i < SCHIRMATA_MAXIMUS; i++)
    {
        Color color_text;
        i32 num;
        b32 est_activum;

        est_activum = (i == schirmata->index_currens);

        /* Pingere fondum pro tab activo */
        si (est_activum)
        {
            i32 py, px;
            i32 padding_left;
            i32 padding_right;
            i32 bar_altitudo;
            i32 bg_width;

            padding_left = II;
            padding_right = IV;
            bar_altitudo = character_altitudo + I;
            bg_width = padding_left + tab_width + padding_right;

            per (py = ZEPHYRUM; py < bar_altitudo; py++)
            {
                per (px = ZEPHYRUM; px < bg_width; px++)
                {
                    i32 draw_x;
                    draw_x = tab_x - padding_left + px;

                    si (tab_y + py >= ZEPHYRUM && tab_y + py < tabula->altitudo &&
                        draw_x >= ZEPHYRUM && draw_x < tabula->latitudo)
                    {
                        tabula->pixela[(tab_y + py) * tabula->latitudo + draw_x] =
                            color_ad_pixelum(color_fondum_activum);
                    }
                }
            }
        }

        /* Numerus: 1-9, 0 pro 10 */
        num = (i + I) % X;
        sprintf(buffer, " %d ", num);
        label.datum = (i8*)buffer;
        label.mensura = III;

        color_text = est_activum ? color_text_activum : color_text_normal;

        tabula_pixelorum_pingere_chordam(
            tabula,
            tab_x,
            tab_y + I,  /* 1 pixel padding top */
            label,
            color_ad_pixelum(color_text));

        tab_x += tab_width + margin_right;
    }

    /* Pingere festivitatem et datum hodiernum ad dextram */
    {
        Piscina* piscina_temp;
        ChordaAedificator* aed;
        Dies hodie;
        chorda chorda_display;
        chorda nomina_celebrationum;
        i32 display_x;
        i32 display_y;
        i32 col;
        i32 margin_dextra;

        piscina_temp = piscina_generare_dynamicum("tab_date", M);
        si (piscina_temp != NIHIL) {
            hodie = fasti_dies_hodie();
            aed = chorda_aedificator_creare(piscina_temp, CCLVI);

            si (aed != NIHIL) {
                /* Obtinere nomina omnium celebrationum (mobiles et fixae) */
                nomina_celebrationum = calendarium_nomen_celebrationum(hodie, piscina_temp);

                si (nomina_celebrationum.mensura > ZEPHYRUM) {
                    /* Celebrationes - Date */
                    chorda_aedificator_appendere_chorda(aed, nomina_celebrationum);
                    chorda_aedificator_appendere_literis(aed, " - ");
                }

                /* Addere datum */
                fasti_scribere_diem(aed, hodie, FASTI_FORMA_ANGLICA_LONGA);
                chorda_display = chorda_aedificator_spectare(aed);

                margin_dextra = VIII;
                display_x = tabula->latitudo - (i32)chorda_display.mensura * character_latitudo - margin_dextra;
                display_y = tab_y + I;

                per (col = ZEPHYRUM; col < (i32)chorda_display.mensura; col++) {
                    tabula_pixelorum_pingere_characterem(
                        tabula,
                        display_x + col * character_latitudo,
                        display_y,
                        (character)chorda_display.datum[col],
                        color_ad_pixelum(color_text_normal)
                    );
                }
            }

            piscina_destruere(piscina_temp);
        }
    }
}


/* ==================================================
 * API Publica
 * ================================================== */

Schirmata*
schirmata_creare(
    ContextusWidget* ctx,
    LibroPaginarum*  libro)
{
    Schirmata* schirmata;
    i32 i;

    si (!ctx || !ctx->piscina || !ctx->intern || !libro)
    {
        redde NIHIL;
    }

    /* Allocare schirmata */
    schirmata = piscina_allocare(ctx->piscina, magnitudo(Schirmata));
    si (!schirmata)
    {
        redde NIHIL;
    }

    /* Initiare campos */
    schirmata->ctx = ctx;
    schirmata->libro = libro;
    schirmata->index_currens = ZEPHYRUM;
    schirmata->praefixum_activum = FALSUM;
    schirmata->tempus_praefixum = 0.0;

    /* Initiare omnes schirmas */
    per (i = ZEPHYRUM; i < SCHIRMATA_MAXIMUS; i++)
    {
        schirmata->schirmae[i].manager = NIHIL;
        schirmata->schirmae[i].initiatus = FALSUM;
    }

    /* Creare layout pro omnibus schirmis */
    per (i = ZEPHYRUM; i < SCHIRMATA_MAXIMUS; i++)
    {
        si (!_creare_schirma_layout(schirmata, i))
        {
            redde NIHIL;
        }
    }

    /* Creare singleton widgets (una instancia pro omnes schirmae) */
    schirmata->arx_caeli = arx_caeli_creare(ctx);
    si (schirmata->arx_caeli)
    {
        arx_caeli_ponere_link_callback(schirmata->arx_caeli,
                                        _arx_caeli_link_callback,
                                        schirmata);
    }

    schirmata->thema_visus = thema_visus_creare(ctx->piscina);
    schirmata->sputnik_syntaxis = sputnik_syntaxis_creare(ctx->piscina);
    schirmata->biblia_visus = biblia_visus_creare(ctx->piscina);
    schirmata->librarium_visus = librarium_visus_creare(ctx->piscina, ctx);
    schirmata->fons_visus = fons_visus_creare(ctx->piscina);
    schirmata->calendario_visus = calendario_visus_creare(ctx->piscina);

    /* Configurare callback pro widget switching */
    ctx->commutare_widget = _schirmata_commutare_widget_callback;
    ctx->schirmata_datum = schirmata;

    redde schirmata;
}

vacuum
schirmata_commutare_ad(
    Schirmata* schirmata,
    i32        index)
{
    si (!schirmata)
    {
        redde;
    }

    /* Clamp index */
    si (index < ZEPHYRUM)
    {
        index = ZEPHYRUM;
    }
    si (index >= SCHIRMATA_MAXIMUS)
    {
        index = SCHIRMATA_MAXIMUS - I;
    }

    /* Si iam in hac schirma, nihil facere */
    si (index == schirmata->index_currens)
    {
        redde;
    }

    /* Salvare status schirmae currentis */
    _salvare_status(&schirmata->schirmae[schirmata->index_currens], schirmata->libro);
    _salvare_widget_status(schirmata, schirmata->index_currens);

    /* Commutare */
    schirmata->index_currens = index;

    /* Restituere status novae schirmae */
    _restituere_status(&schirmata->schirmae[index], schirmata->libro);
    _carcare_widget_status(schirmata, index);
}

vacuum
schirmata_proxima(
    Schirmata* schirmata)
{
    i32 nova_index;

    si (!schirmata)
    {
        redde;
    }

    nova_index = schirmata->index_currens + I;
    si (nova_index >= SCHIRMATA_MAXIMUS)
    {
        nova_index = ZEPHYRUM;
    }

    schirmata_commutare_ad(schirmata, nova_index);
}

vacuum
schirmata_prior(
    Schirmata* schirmata)
{
    i32 nova_index;

    si (!schirmata)
    {
        redde;
    }

    nova_index = schirmata->index_currens - I;
    si (nova_index < ZEPHYRUM)
    {
        nova_index = SCHIRMATA_MAXIMUS - I;
    }

    schirmata_commutare_ad(schirmata, nova_index);
}

b32
schirmata_tractare_eventum(
    Schirmata*        schirmata,
    constans Eventus* eventus)
{
    ManagerWidget* manager;
    Schirma*       schirma;

    si (!schirmata || !eventus)
    {
        redde FALSUM;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    /* Tractare dialogum si activus */
    si (schirma->dialogus != NIHIL && schirma->dialogus->tractare_eventum != NIHIL)
    {
        DialogusFructus fructus;
        b32 in_right_panel = FALSUM;

        /* Si in modus importatio, tractare mouse events in right panel separatim */
        si (schirma->modus_importatio_visus && schirma->importatio_visus != NIHIL)
        {
            /* Determinare si mouse est in right panel (secunda dimidia fenestrae) */
            si (eventus->genus == EVENTUS_MUS_DEPRESSUS ||
                eventus->genus == EVENTUS_MUS_LIBERATUS ||
                eventus->genus == EVENTUS_MUS_MOTUS ||
                eventus->genus == EVENTUS_MUS_ROTULA)
            {
                i32 fenestra_latitudo = 640; /* hardcoded ut in probatio_combinado */
                i32 mus_x = eventus->datum.mus.x;

                /* Right panel starts at half width */
                si (mus_x >= fenestra_latitudo / II)
                {
                    in_right_panel = VERUM;
                    /* Tradere eventum ad importatio_visus */
                    importatio_visus_tractare_eventum(schirma->importatio_visus, eventus);
                }
            }
        }

        /* Tradere eventum ad dialogum (nisi erat in right panel) */
        si (!in_right_panel)
        {
            fructus = schirma->dialogus->tractare_eventum(schirma->dialogus, eventus);
        }
        alioquin
        {
            fructus = DIALOGUS_CURRENS;
        }

        si (fructus == DIALOGUS_CONFIRMATUS || fructus == DIALOGUS_ABORTUS)
        {
            /* Si erat importatio, tractare fructum */
            si (schirma->modus_importatio_visus && schirma->importatio_visus != NIHIL)
            {
                si (fructus == DIALOGUS_CONFIRMATUS)
                {
                    /* TODO: Salvare imaginem ad repositorium */
                    constans i8* indices;
                    i32 lat, alt;
                    chorda titulus;

                    si (importatio_visus_obtinere_fructum(
                            schirma->importatio_visus, &indices, &lat, &alt, &titulus))
                    {
                        fprintf(stderr, "Imago importata: %.*s (%d x %d)\n",
                            (integer)titulus.mensura, titulus.datum, lat, alt);
                    }
                }

                /* Terminare sessionem importationis */
                importatio_visus_terminare_sessionem(schirma->importatio_visus);
                schirma->importatio_visus = NIHIL;
                schirma->modus_importatio_visus = FALSUM;

                /* Commutare ad navigator */
                schirmata_commutare_ad_navigator(schirmata);
            }

            /* Claudere dialogum */
            si (schirma->dialogus->destruere != NIHIL)
            {
                schirma->dialogus->destruere(schirma->dialogus);
            }
            schirma->dialogus = NIHIL;
        }

        /* Dialogus tractavit eventum */
        redde VERUM;
    }

    /* Tractare mouse click in tab bar */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS)
    {
        i32 click_y;
        i32 click_x;
        i32 tab_y_min;
        i32 character_altitudo;
        i32 character_latitudo;
        i32 fenestra_altitudo;

        character_altitudo = VIII;
        character_latitudo = VI;

        /* Fenestra altitudo in pixelis (hardcoded ut in probatio_combinado) */
        fenestra_altitudo = CDLXXX;  /* 480 */

        click_y = eventus->datum.mus.y;
        click_x = eventus->datum.mus.x;

        /* Tab bar est ad fundum fenestrae: altitudo - character_altitudo - 2 */
        tab_y_min = fenestra_altitudo - character_altitudo - II;

        si (click_y >= tab_y_min)
        {
            /* Click in tab bar area */
            i32 tab_index;
            i32 tab_width;
            i32 margin_right;

            tab_width = III * character_latitudo;  /* "[N]" = 3 characters */
            margin_right = VIII;  /* 8 pixels margin */

            tab_index = (click_x - II) / (tab_width + margin_right);
            si (tab_index >= ZEPHYRUM && tab_index < SCHIRMATA_MAXIMUS)
            {
                schirmata_commutare_ad(schirmata, tab_index);
                redde VERUM;
            }
        }
    }

    /* Si praefixum activum, tractare commandum schirmae */
    si (schirmata->praefixum_activum)
    {
        f64 tempus_currens;
        f64 tempus_elapsum;

        tempus_currens = tempus_nunc();
        tempus_elapsum = tempus_currens - schirmata->tempus_praefixum;

        /* Verificare timeout */
        si (tempus_elapsum > PRAEFIXUM_TIMEOUT)
        {
            /* Timeout - reset praefixum et passare eventum */
            schirmata->praefixum_activum = FALSUM;
        }
        alioquin
        {
            si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
            {
                s32 clavis;

                clavis = (s32)eventus->datum.clavis.clavis;

                /* n = proxima */
                si (clavis == 'n' || clavis == 'N')
                {
                    schirmata->praefixum_activum = FALSUM;
                    schirmata_proxima(schirmata);
                    redde VERUM;
                }
                /* p = prior */
                si (clavis == 'p' || clavis == 'P')
                {
                    schirmata->praefixum_activum = FALSUM;
                    schirmata_prior(schirmata);
                    redde VERUM;
                }
                /* 1-9 = schirma 0-8 */
                si (clavis >= '1' && clavis <= '9')
                {
                    schirmata->praefixum_activum = FALSUM;
                    schirmata_commutare_ad(schirmata, (i32)(clavis - '1'));
                    redde VERUM;
                }
                /* 0 = schirma 9 (decima) */
                si (clavis == '0')
                {
                    schirmata->praefixum_activum = FALSUM;
                    schirmata_commutare_ad(schirmata, IX);
                    redde VERUM;
                }
                /* ctrl-a iterum = passare ad widget */
                si ((clavis == 'a' || clavis == 'A') && (eventus->datum.clavis.modificantes & MOD_IMPERIUM))
                {
                    schirmata->praefixum_activum = FALSUM;
                    manager = schirmata->schirmae[schirmata->index_currens].manager;
                    redde manager_widget_tractare_eventum(manager, eventus);
                }

                /* Clavis non recognita - reset praefixum */
                schirmata->praefixum_activum = FALSUM;
            }

            /* Non-clavis eventus (e.g. key-up) - ignorare sed manere in praefixum modo */
            redde VERUM;
        }
    }

    /* Detegere ctrl-a (A quia convertere_clavem reddit maiusculas) */
    si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        si ((eventus->datum.clavis.clavis == 'a' || eventus->datum.clavis.clavis == 'A') &&
            (eventus->datum.clavis.modificantes & MOD_IMPERIUM))
        {
            schirmata->praefixum_activum = VERUM;
            schirmata->tempus_praefixum = tempus_nunc();
            redde VERUM;
        }

        /* Detegere Cmd+V (paste) - initiare importationem imaginis */
        si ((eventus->datum.clavis.clavis == 'v' || eventus->datum.clavis.clavis == 'V') &&
            (eventus->datum.clavis.modificantes & MOD_SUPER))
        {
            si (schirmata_initiare_importationem_ex_clipboard(schirmata))
            {
                redde VERUM;
            }
            /* Si non successus, fall through ad manager */
        }
    }

    /* Delegare ad manager schirmae currentis */
    manager = schirmata->schirmae[schirmata->index_currens].manager;
    redde manager_widget_tractare_eventum(manager, eventus);
}

vacuum
schirmata_reddere(
    Schirmata*       schirmata,
    TabulaPixelorum* tabula,
    i32              scala)
{
    ManagerWidget* manager;
    Schirma*       schirma;

    si (!schirmata || !tabula)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    /* Reddere widgets schirmae currentis */
    manager = schirma->manager;
    manager_widget_reddere(manager, tabula, scala);

    /* Reddere dialogum si activus */
    si (schirma->dialogus != NIHIL)
    {
        i32 panel_x, panel_y, panel_w, panel_h;

        /* Computare limites panel basatus in dialogus_panel */
        si (schirma->dialogus_panel == ZEPHYRUM)
        {
            /* Panel sinister */
            panel_x = ZEPHYRUM;
            panel_y = ZEPHYRUM;
            panel_w = LXXI;   /* 71 characteres */
            panel_h = LIX;    /* 59 characteres */
        }
        alioquin
        {
            /* Panel dexter */
            panel_x = LXXI;
            panel_y = ZEPHYRUM;
            panel_w = LXXI;
            panel_h = LIX;
        }

        dialogus_reddere_overlay(
            schirma->dialogus,
            tabula,
            panel_x, panel_y,
            panel_w, panel_h,
            (f32)scala);
    }

    /* Reddere tab bar */
    _reddere_tabulam_schirmarum(schirmata, tabula, scala);
}

i32
schirmata_index_currens(
    Schirmata* schirmata)
{
    si (!schirmata)
    {
        redde ZEPHYRUM;
    }

    redde schirmata->index_currens;
}

ManagerWidget*
schirmata_manager_currens(
    Schirmata* schirmata)
{
    si (!schirmata)
    {
        redde NIHIL;
    }

    redde schirmata->schirmae[schirmata->index_currens].manager;
}


/* ==================================================
 * Mode Switching - Arx Caeli / Navigator
 * ================================================== */

vacuum
schirmata_commutare_ad_arx_caeli(
    Schirmata*          schirmata,
    constans character* slug)
{
    Schirma*                schirma;
    ManagerWidget*          manager;
    SchirmataArcCaeliDatum* arc_datum;

    si (!schirmata)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    si (schirma->modus_arx_caeli)
    {
        /* Iam in modus arx caeli - navigare ad slug */
        arx_caeli_navigare_ad(schirmata->arx_caeli, slug);
        redde;
    }

    /* Commutare ex navigator ad arx caeli */
    manager = schirma->manager;

    /* Creare arx caeli datum */
    arc_datum = piscina_allocare(schirmata->ctx->piscina, magnitudo(SchirmataArcCaeliDatum));
    si (!arc_datum)
    {
        redde;
    }
    arc_datum->arx_caeli = schirmata->arx_caeli;
    arc_datum->schirmata = schirmata;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = arc_datum;
        manager->widgets[I].reddere = _schirmata_arx_caeli_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_arx_caeli_tractare_eventum;
        manager->focus_index = I;
    }

    schirma->modus_arx_caeli = VERUM;
    schirma->modus_thema_visus = FALSUM;
    schirma->modus_sputnik_syntaxis = FALSUM;
    schirma->modus_biblia_visus = FALSUM;
    schirma->modus_librarium = FALSUM;
    schirma->modus_fons_visus = FALSUM;
    schirma->modus_calendario_visus = FALSUM;
    schirma->modus_importatio_visus = FALSUM;

    /* Navigare ad slug */
    arx_caeli_navigare_ad(schirmata->arx_caeli, slug);
}

vacuum
schirmata_commutare_ad_navigator(
    Schirmata* schirmata)
{
    Schirma*                 schirma;
    ManagerWidget*           manager;
    SchirmataNavigatorDatum* nav_datum;
    NavigatorEntitatum*      navigator;

    si (!schirmata)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    si (!schirma->modus_arx_caeli && !schirma->modus_thema_visus &&
        !schirma->modus_sputnik_syntaxis && !schirma->modus_biblia_visus &&
        !schirma->modus_librarium && !schirma->modus_fons_visus)
    {
        /* Iam in modus navigator */
        redde;
    }

    /* Commutare ex arx caeli vel thema visus vel sputnik syntaxis ad navigator */
    manager = schirma->manager;

    /* Creare navigator si non existit */
    navigator = navigator_entitatum_creare(schirmata->ctx);
    si (!navigator)
    {
        redde;
    }

    nav_datum = piscina_allocare(schirmata->ctx->piscina, magnitudo(SchirmataNavigatorDatum));
    si (!nav_datum)
    {
        redde;
    }
    nav_datum->navigator = navigator;
    nav_datum->piscina = schirmata->ctx->piscina;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = nav_datum;
        manager->widgets[I].reddere = _schirmata_navigator_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_navigator_tractare_eventum;
    }

    schirma->modus_arx_caeli = FALSUM;
    schirma->modus_thema_visus = FALSUM;
    schirma->modus_sputnik_syntaxis = FALSUM;
    schirma->modus_biblia_visus = FALSUM;
    schirma->modus_librarium = FALSUM;
    schirma->modus_fons_visus = FALSUM;
    schirma->modus_calendario_visus = FALSUM;
    schirma->modus_importatio_visus = FALSUM;
}


/* ==================================================
 * Mode Switching - Thema Visus
 * ================================================== */

vacuum
schirmata_commutare_ad_thema_visus(
    Schirmata* schirmata)
{
    Schirma*                  schirma;
    ManagerWidget*            manager;
    SchirmataThemaVisusDatum* thema_datum;

    si (!schirmata)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    si (schirma->modus_thema_visus)
    {
        /* Iam in modus thema visus */
        redde;
    }

    /* Commutare ad thema visus */
    manager = schirma->manager;

    /* Creare thema visus datum */
    thema_datum = piscina_allocare(schirmata->ctx->piscina, magnitudo(SchirmataThemaVisusDatum));
    si (!thema_datum)
    {
        redde;
    }
    thema_datum->thema_visus = schirmata->thema_visus;
    thema_datum->schirmata = schirmata;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = thema_datum;
        manager->widgets[I].reddere = _schirmata_thema_visus_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_thema_visus_tractare_eventum;
        manager->focus_index = I;
    }

    schirma->modus_arx_caeli = FALSUM;
    schirma->modus_thema_visus = VERUM;
    schirma->modus_sputnik_syntaxis = FALSUM;
    schirma->modus_biblia_visus = FALSUM;
    schirma->modus_librarium = FALSUM;
    schirma->modus_fons_visus = FALSUM;
    schirma->modus_calendario_visus = FALSUM;
    schirma->modus_importatio_visus = FALSUM;
}


/* ==================================================
 * Mode Switching - Sputnik Syntaxis
 * ================================================== */

vacuum
schirmata_commutare_ad_sputnik_syntaxis(
    Schirmata* schirmata)
{
    Schirma*                       schirma;
    ManagerWidget*                 manager;
    SchirmataSputnikSyntaxisDatum* syntaxis_datum;

    si (!schirmata)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    si (schirma->modus_sputnik_syntaxis)
    {
        /* Iam in modus sputnik syntaxis */
        redde;
    }

    /* Commutare ad sputnik syntaxis */
    manager = schirma->manager;

    /* Creare sputnik syntaxis datum */
    syntaxis_datum = piscina_allocare(schirmata->ctx->piscina, magnitudo(SchirmataSputnikSyntaxisDatum));
    si (!syntaxis_datum)
    {
        redde;
    }
    syntaxis_datum->sputnik_syntaxis = schirmata->sputnik_syntaxis;
    syntaxis_datum->schirmata = schirmata;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = syntaxis_datum;
        manager->widgets[I].reddere = _schirmata_sputnik_syntaxis_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_sputnik_syntaxis_tractare_eventum;
        manager->focus_index = I;
    }

    schirma->modus_arx_caeli = FALSUM;
    schirma->modus_thema_visus = FALSUM;
    schirma->modus_sputnik_syntaxis = VERUM;
    schirma->modus_biblia_visus = FALSUM;
    schirma->modus_librarium = FALSUM;
    schirma->modus_fons_visus = FALSUM;
    schirma->modus_calendario_visus = FALSUM;
    schirma->modus_importatio_visus = FALSUM;
}


/* ==================================================
 * Mode Switching - Biblia Visus
 * ================================================== */

vacuum
schirmata_commutare_ad_biblia_visus(
    Schirmata* schirmata)
{
    Schirma*                   schirma;
    ManagerWidget*             manager;
    SchirmataBibliaVisusDatum* biblia_datum;

    si (!schirmata)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    si (schirma->modus_biblia_visus)
    {
        /* Iam in modus biblia visus */
        redde;
    }

    /* Commutare ad biblia visus */
    manager = schirma->manager;

    /* Creare biblia visus datum */
    biblia_datum = piscina_allocare(schirmata->ctx->piscina, magnitudo(SchirmataBibliaVisusDatum));
    si (!biblia_datum)
    {
        redde;
    }
    biblia_datum->biblia_visus = schirmata->biblia_visus;
    biblia_datum->schirmata = schirmata;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = biblia_datum;
        manager->widgets[I].reddere = _schirmata_biblia_visus_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_biblia_visus_tractare_eventum;
        manager->focus_index = I;  /* Focus ad biblia_visus */
    }

    schirma->modus_arx_caeli = FALSUM;
    schirma->modus_thema_visus = FALSUM;
    schirma->modus_sputnik_syntaxis = FALSUM;
    schirma->modus_biblia_visus = VERUM;
    schirma->modus_librarium = FALSUM;
    schirma->modus_fons_visus = FALSUM;
    schirma->modus_calendario_visus = FALSUM;
    schirma->modus_importatio_visus = FALSUM;
}


/* ==================================================
 * Mode Switching - Librarium Visus
 * ================================================== */

vacuum
schirmata_commutare_ad_librarium(
    Schirmata*          schirmata,
    constans character* quaestio)
{
    Schirma*                       schirma;
    ManagerWidget*                 manager;
    SchirmataLibrariumVisusDatum*  librarium_datum;

    si (!schirmata)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    si (schirma->modus_librarium)
    {
        /* Iam in modus librarium */
        si (quaestio)
        {
            /* Quaerere si quaestio provisa */
            librarium_visus_quaerere(schirmata->librarium_visus, quaestio);
        }
        alioquin
        {
            /* Sine quaestio - reset ad radix */
            librarium_visus_reset_ad_radix(schirmata->librarium_visus);
        }
        redde;
    }

    /* Commutare ad librarium visus */
    manager = schirma->manager;

    /* Creare librarium visus datum */
    librarium_datum = piscina_allocare(schirmata->ctx->piscina, magnitudo(SchirmataLibrariumVisusDatum));
    si (!librarium_datum)
    {
        redde;
    }
    librarium_datum->librarium_visus = schirmata->librarium_visus;
    librarium_datum->schirmata = schirmata;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = librarium_datum;
        manager->widgets[I].reddere = _schirmata_librarium_visus_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_librarium_visus_tractare_eventum;
        manager->focus_index = I;  /* Focus ad librarium_visus */
    }

    schirma->modus_arx_caeli = FALSUM;
    schirma->modus_thema_visus = FALSUM;
    schirma->modus_sputnik_syntaxis = FALSUM;
    schirma->modus_biblia_visus = FALSUM;
    schirma->modus_librarium = VERUM;
    schirma->modus_fons_visus = FALSUM;
    schirma->modus_calendario_visus = FALSUM;
    schirma->modus_importatio_visus = FALSUM;

    /* Quaerere si quaestio */
    si (quaestio)
    {
        librarium_visus_quaerere(schirmata->librarium_visus, quaestio);
    }
}


/* ==================================================
 * Mode Switching - Fons Visus
 * ================================================== */

vacuum
schirmata_commutare_ad_fons_visus(
    Schirmata* schirmata)
{
    Schirma*                 schirma;
    ManagerWidget*           manager;
    SchirmataFonsVisusDatum* fons_datum;

    si (!schirmata)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    si (schirma->modus_fons_visus)
    {
        /* Iam in modus fons visus */
        redde;
    }

    /* Commutare ad fons visus */
    manager = schirma->manager;

    /* Creare fons visus datum */
    fons_datum = piscina_allocare(schirmata->ctx->piscina, magnitudo(SchirmataFonsVisusDatum));
    si (!fons_datum)
    {
        redde;
    }
    fons_datum->fons_visus = schirmata->fons_visus;
    fons_datum->schirmata = schirmata;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = fons_datum;
        manager->widgets[I].reddere = _schirmata_fons_visus_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_fons_visus_tractare_eventum;
    }

    schirma->modus_arx_caeli = FALSUM;
    schirma->modus_thema_visus = FALSUM;
    schirma->modus_sputnik_syntaxis = FALSUM;
    schirma->modus_biblia_visus = FALSUM;
    schirma->modus_librarium = FALSUM;
    schirma->modus_fons_visus = VERUM;
    schirma->modus_calendario_visus = FALSUM;
    schirma->modus_importatio_visus = FALSUM;
}


/* ==================================================
 * Mode Switching - Calendario Visus
 * ================================================== */

vacuum
schirmata_commutare_ad_calendario_visus(
    Schirmata* schirmata)
{
    Schirma*                       schirma;
    ManagerWidget*                 manager;
    SchirmataCalendarioVisusDatum* calendario_datum;

    si (!schirmata)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    si (schirma->modus_calendario_visus)
    {
        /* Iam in modus calendario visus */
        redde;
    }

    /* Commutare ad calendario visus */
    manager = schirma->manager;

    /* Creare calendario visus datum */
    calendario_datum = piscina_allocare(schirmata->ctx->piscina,
        magnitudo(SchirmataCalendarioVisusDatum));
    si (!calendario_datum)
    {
        redde;
    }
    calendario_datum->calendario_visus = schirmata->calendario_visus;
    calendario_datum->schirmata = schirmata;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = calendario_datum;
        manager->widgets[I].reddere = _schirmata_calendario_visus_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_calendario_visus_tractare_eventum;
    }

    schirma->modus_arx_caeli = FALSUM;
    schirma->modus_thema_visus = FALSUM;
    schirma->modus_sputnik_syntaxis = FALSUM;
    schirma->modus_biblia_visus = FALSUM;
    schirma->modus_librarium = FALSUM;
    schirma->modus_fons_visus = FALSUM;
    schirma->modus_calendario_visus = VERUM;
    schirma->modus_importatio_visus = FALSUM;
}


/* ==================================================
 * Import Imaginis
 * ================================================== */

b32
schirmata_initiare_importationem_ex_clipboard(
    Schirmata* schirmata)
{
    Schirma*                       schirma;
    ManagerWidget*                 manager;
    ImagoFructus                   imago_fructus;
    ImportatioVisus*               visus;
    DialogusImportatio*            di;
    Dialogus*                      dlg;
    SchirmataImportatioVisusDatum* import_datum;

    si (!schirmata)
    {
        redde FALSUM;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];
    manager = schirma->manager;

    /* Verificare an dialogus iam activus */
    si (schirma->dialogus != NIHIL)
    {
        redde FALSUM;
    }

    /* Obtinere imaginem ex clipboard */
    imago_fructus = clipboard_capere_imaginem(schirmata->ctx->piscina);
    si (!imago_fructus.successus)
    {
        fprintf(stderr, "Nulla imago in clipboard\n");
        redde FALSUM;
    }

    fprintf(stderr, "Imago obtenta ex clipboard: %u x %u\n",
        imago_fructus.imago.latitudo, imago_fructus.imago.altitudo);

    /* Creare ImportatioVisus */
    visus = importatio_visus_creare(schirmata->ctx->piscina);
    si (visus == NIHIL)
    {
        fprintf(stderr, "Errore: non possum creare ImportatioVisus\n");
        redde FALSUM;
    }

    /* Initiare sessionem */
    importatio_visus_initiare_sessionem(visus, &imago_fructus.imago);
    schirma->importatio_visus = visus;

    /* Creare dialogum importationis */
    di = dialogus_importatio_creare(schirmata->ctx->piscina, visus);
    si (di == NIHIL)
    {
        fprintf(stderr, "Errore: non possum creare DialogusImportatio\n");
        schirma->importatio_visus = NIHIL;
        redde FALSUM;
    }

    dlg = dialogus_importatio_obtinere_dialogum(di);

    /* Creare import_datum pro widget */
    import_datum = piscina_allocare(schirmata->ctx->piscina,
                                    magnitudo(SchirmataImportatioVisusDatum));
    si (!import_datum)
    {
        schirma->importatio_visus = NIHIL;
        redde FALSUM;
    }
    import_datum->importatio_visus = visus;
    import_datum->schirmata = schirmata;

    /* Substituere widget index 1 (right panel) cum importatio_visus */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = import_datum;
        manager->widgets[I].reddere = _schirmata_importatio_visus_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_importatio_visus_tractare_eventum;
    }

    /* Reset omnes modi, activare importatio */
    schirma->modus_arx_caeli = FALSUM;
    schirma->modus_thema_visus = FALSUM;
    schirma->modus_sputnik_syntaxis = FALSUM;
    schirma->modus_biblia_visus = FALSUM;
    schirma->modus_librarium = FALSUM;
    schirma->modus_fons_visus = FALSUM;
    schirma->modus_calendario_visus = FALSUM;
    schirma->modus_importatio_visus = VERUM;

    /* Aperire dialogum in left panel */
    schirma->dialogus = dlg;
    schirma->dialogus_panel = 0;  /* Sinister */

    fprintf(stderr, "Importatio initiata\n");

    redde VERUM;
}


LibroPaginarum*
schirmata_libro(
    Schirmata* schirmata)
{
    si (!schirmata)
    {
        redde NIHIL;
    }

    redde schirmata->libro;
}


/* ==================================================
 * Dialogi
 * ================================================== */

vacuum
schirmata_aperire_dialogum(
    Schirmata* schirmata,
    Dialogus*  dialogus,
    s32        panel)
{
    Schirma* schirma;

    si (!schirmata || !dialogus)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    /* Claudere dialogum priorem si existit */
    si (schirma->dialogus != NIHIL)
    {
        si (schirma->dialogus->destruere != NIHIL)
        {
            schirma->dialogus->destruere(schirma->dialogus);
        }
    }

    schirma->dialogus = dialogus;
    schirma->dialogus_panel = panel;
}

vacuum
schirmata_claudere_dialogum(
    Schirmata* schirmata)
{
    Schirma* schirma;

    si (!schirmata)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    si (schirma->dialogus != NIHIL)
    {
        si (schirma->dialogus->destruere != NIHIL)
        {
            schirma->dialogus->destruere(schirma->dialogus);
        }
        schirma->dialogus = NIHIL;
    }
}

b32
schirmata_habet_dialogum(
    Schirmata* schirmata)
{
    Schirma* schirma;

    si (!schirmata)
    {
        redde FALSUM;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    redde (schirma->dialogus != NIHIL);
}


/* ==================================================
 * Persistentia - Salvare/Carcare Omnes
 * ================================================== */

/* Convertere modus flags ad chorda */
hic_manens constans character*
_modus_ad_chorda(
    Schirma* schirma)
{
    si (schirma->modus_arx_caeli)
    {
        redde "arx_caeli";
    }
    si (schirma->modus_biblia_visus)
    {
        redde "biblia";
    }
    si (schirma->modus_librarium)
    {
        redde "librarium";
    }
    si (schirma->modus_thema_visus)
    {
        redde "thema";
    }
    si (schirma->modus_sputnik_syntaxis)
    {
        redde "sputnik";
    }
    si (schirma->modus_fons_visus)
    {
        redde "fons";
    }
    si (schirma->modus_calendario_visus)
    {
        redde "calendario";
    }
    redde "navigator";
}


vacuum
schirmata_salvare_omnes(
    Schirmata* schirmata)
{
    EntitasRepositorium* repo;
    Entitas* entitas;
    character entitas_id[XXXII];
    character valor[XXXII];
    i32 i;

    si (!schirmata || !schirmata->ctx || !schirmata->ctx->repo)
    {
        redde;
    }

    repo = schirmata->ctx->repo;

    /* Salvare status globalis */
    entitas = repo->entitas_scaffoldare(repo->datum, "SchirmataGlobal", "0");
    si (entitas)
    {
        sprintf(valor, "%d", schirmata->index_currens);
        repo->proprietas_ponere(repo->datum, entitas, "index_currens", valor);
    }

    /* Salvare status per schirma */
    per (i = ZEPHYRUM; i < SCHIRMATA_MAXIMUS; i++)
    {
        Schirma* schirma;

        schirma = &schirmata->schirmae[i];
        sprintf(entitas_id, "%d", i);

        entitas = repo->entitas_scaffoldare(repo->datum, "SchirmaStatus", entitas_id);
        si (!entitas)
        {
            perge;
        }

        /* Salvare modus */
        repo->proprietas_ponere(repo->datum, entitas, "modus", _modus_ad_chorda(schirma));

        /* Salvare libro_status */
        sprintf(valor, "%d", schirma->libro_status.index_paginae);
        repo->proprietas_ponere(repo->datum, entitas, "libro_index_paginae", valor);

        sprintf(valor, "%d", schirma->libro_status.cursor_linea);
        repo->proprietas_ponere(repo->datum, entitas, "libro_cursor_linea", valor);

        sprintf(valor, "%d", schirma->libro_status.cursor_columna);
        repo->proprietas_ponere(repo->datum, entitas, "libro_cursor_columna", valor);

        sprintf(valor, "%d", (i32)schirma->libro_status.modo);
        repo->proprietas_ponere(repo->datum, entitas, "libro_modo", valor);

        /* Salvare widget status */
        _salvare_widget_status(schirmata, i);
    }
}


vacuum
schirmata_carcare_omnes(
    Schirmata* schirmata)
{
    EntitasRepositorium* repo;
    InternamentumChorda* intern;
    Entitas* entitas;
    chorda* valor;
    character entitas_id[XXXII];
    i32 i;

    si (!schirmata || !schirmata->ctx || !schirmata->ctx->repo)
    {
        redde;
    }

    repo = schirmata->ctx->repo;
    intern = schirmata->ctx->intern;

    /* Carcare status globalis */
    entitas = repo->entitas_scaffoldare(repo->datum, "SchirmataGlobal", "0");
    si (entitas)
    {
        valor = entitas_proprietas_capere(entitas,
            chorda_internare_ex_literis(intern, "index_currens"));
        si (valor && valor->mensura > ZEPHYRUM)
        {
            i32 idx;
            idx = ZEPHYRUM;
            {
                i32 j;
                per (j = ZEPHYRUM; j < (i32)valor->mensura; j++)
                {
                    character c;
                    c = (character)valor->datum[j];
                    si (c >= '0' && c <= '9')
                    {
                        idx = idx * X + (i32)(c - '0');
                    }
                }
            }
            si (idx >= ZEPHYRUM && idx < SCHIRMATA_MAXIMUS)
            {
                schirmata->index_currens = idx;
            }
        }
    }

    /* Carcare status per schirma */
    per (i = ZEPHYRUM; i < SCHIRMATA_MAXIMUS; i++)
    {
        Schirma* schirma;
        chorda* modus_valor;

        schirma = &schirmata->schirmae[i];
        sprintf(entitas_id, "%d", i);

        entitas = repo->entitas_scaffoldare(repo->datum, "SchirmaStatus", entitas_id);
        si (!entitas)
        {
            perge;
        }

        /* Carcare libro_status */
        valor = entitas_proprietas_capere(entitas,
            chorda_internare_ex_literis(intern, "libro_index_paginae"));
        si (valor && valor->mensura > ZEPHYRUM)
        {
            i32 v;
            v = ZEPHYRUM;
            {
                i32 j;
                per (j = ZEPHYRUM; j < (i32)valor->mensura; j++)
                {
                    character c;
                    c = (character)valor->datum[j];
                    si (c >= '0' && c <= '9')
                    {
                        v = v * X + (i32)(c - '0');
                    }
                }
            }
            schirma->libro_status.index_paginae = v;
        }

        valor = entitas_proprietas_capere(entitas,
            chorda_internare_ex_literis(intern, "libro_cursor_linea"));
        si (valor && valor->mensura > ZEPHYRUM)
        {
            i32 v;
            v = ZEPHYRUM;
            {
                i32 j;
                per (j = ZEPHYRUM; j < (i32)valor->mensura; j++)
                {
                    character c;
                    c = (character)valor->datum[j];
                    si (c >= '0' && c <= '9')
                    {
                        v = v * X + (i32)(c - '0');
                    }
                }
            }
            schirma->libro_status.cursor_linea = v;
        }

        valor = entitas_proprietas_capere(entitas,
            chorda_internare_ex_literis(intern, "libro_cursor_columna"));
        si (valor && valor->mensura > ZEPHYRUM)
        {
            i32 v;
            v = ZEPHYRUM;
            {
                i32 j;
                per (j = ZEPHYRUM; j < (i32)valor->mensura; j++)
                {
                    character c;
                    c = (character)valor->datum[j];
                    si (c >= '0' && c <= '9')
                    {
                        v = v * X + (i32)(c - '0');
                    }
                }
            }
            schirma->libro_status.cursor_columna = v;
        }

        valor = entitas_proprietas_capere(entitas,
            chorda_internare_ex_literis(intern, "libro_modo"));
        si (valor && valor->mensura > ZEPHYRUM)
        {
            i32 v;
            v = ZEPHYRUM;
            {
                i32 j;
                per (j = ZEPHYRUM; j < (i32)valor->mensura; j++)
                {
                    character c;
                    c = (character)valor->datum[j];
                    si (c >= '0' && c <= '9')
                    {
                        v = v * X + (i32)(c - '0');
                    }
                }
            }
            schirma->libro_status.modo = (ModoVim)v;
        }

        /* Carcare modus et commutare widget */
        modus_valor = entitas_proprietas_capere(entitas,
            chorda_internare_ex_literis(intern, "modus"));
        si (modus_valor && modus_valor->mensura > ZEPHYRUM)
        {
            /* Temporarie commutare ad hac schirma */
            i32 prior_index;
            prior_index = schirmata->index_currens;
            schirmata->index_currens = i;

            si (chorda_aequalis_literis(*modus_valor, "arx_caeli"))
            {
                schirmata_commutare_ad_arx_caeli(schirmata, NIHIL);
            }
            alioquin si (chorda_aequalis_literis(*modus_valor, "biblia"))
            {
                schirmata_commutare_ad_biblia_visus(schirmata);
            }
            alioquin si (chorda_aequalis_literis(*modus_valor, "librarium"))
            {
                schirmata_commutare_ad_librarium(schirmata, NIHIL);
            }
            alioquin si (chorda_aequalis_literis(*modus_valor, "thema"))
            {
                schirmata_commutare_ad_thema_visus(schirmata);
            }
            alioquin si (chorda_aequalis_literis(*modus_valor, "sputnik"))
            {
                schirmata_commutare_ad_sputnik_syntaxis(schirmata);
            }
            alioquin si (chorda_aequalis_literis(*modus_valor, "fons"))
            {
                schirmata_commutare_ad_fons_visus(schirmata);
            }
            alioquin si (chorda_aequalis_literis(*modus_valor, "calendario"))
            {
                schirmata_commutare_ad_calendario_visus(schirmata);
            }
            /* "navigator" = default, nihil facere */

            schirmata->index_currens = prior_index;
        }

        /* Carcare widget status */
        _carcare_widget_status(schirmata, i);
    }

    /* Restituere libro status pro schirma currens */
    _restituere_status(&schirmata->schirmae[schirmata->index_currens], schirmata->libro);

    /* Re-carcare widget status pro schirma currens
     * (necessarium quia loop supra superscribit singleton widgets) */
    _carcare_widget_status(schirmata, schirmata->index_currens);
}
