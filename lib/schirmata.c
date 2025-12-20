#include "schirmata.h"
#include "thema.h"
#include "delineare.h"
#include "tempus.h"
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

    /* Creare Arx Caeli */
    schirma->arx_caeli = arx_caeli_creare(schirmata->ctx);
    si (schirma->arx_caeli)
    {
        arx_caeli_ponere_link_callback(schirma->arx_caeli,
                                        _arx_caeli_link_callback,
                                        schirmata);
    }
    schirma->modus_arx_caeli = FALSUM;  /* Initare in modus navigator */

    /* Creare Thema Visus */
    schirma->thema_visus = thema_visus_creare(schirmata->ctx->piscina);
    schirma->modus_thema_visus = FALSUM;

    /* Creare Sputnik Syntaxis */
    schirma->sputnik_syntaxis = sputnik_syntaxis_creare(schirmata->ctx->piscina);
    schirma->modus_sputnik_syntaxis = FALSUM;

    /* Creare Biblia Visus */
    schirma->biblia_visus = biblia_visus_creare(schirmata->ctx->piscina);
    schirma->modus_biblia_visus = FALSUM;

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

    /* Commutare */
    schirmata->index_currens = index;

    /* Restituere status novae schirmae */
    _restituere_status(&schirmata->schirmae[index], schirmata->libro);
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

    si (!schirmata || !eventus)
    {
        redde FALSUM;
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

    si (!schirmata || !tabula)
    {
        redde;
    }

    /* Reddere widgets schirmae currentis */
    manager = schirmata->schirmae[schirmata->index_currens].manager;
    manager_widget_reddere(manager, tabula, scala);

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
        arx_caeli_navigare_ad(schirma->arx_caeli, slug);
        redde;
    }

    /* Commutare ex navigator ad arx caeli */
    manager = schirma->manager;

    /* Removere widget index 1 (navigator) et addere arx caeli */
    /* Nota: ManagerWidget non habet functionem removere, ergo recreare */

    /* Creare arx caeli datum */
    arc_datum = piscina_allocare(schirmata->ctx->piscina, magnitudo(SchirmataArcCaeliDatum));
    si (!arc_datum)
    {
        redde;
    }
    arc_datum->arx_caeli = schirma->arx_caeli;
    arc_datum->schirmata = schirmata;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = arc_datum;
        manager->widgets[I].reddere = _schirmata_arx_caeli_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_arx_caeli_tractare_eventum;
    }

    schirma->modus_arx_caeli = VERUM;
    schirma->modus_thema_visus = FALSUM;
    schirma->modus_sputnik_syntaxis = FALSUM;
    schirma->modus_biblia_visus = FALSUM;

    /* Navigare ad slug */
    arx_caeli_navigare_ad(schirma->arx_caeli, slug);
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
        !schirma->modus_sputnik_syntaxis && !schirma->modus_biblia_visus)
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
    thema_datum->thema_visus = schirma->thema_visus;
    thema_datum->schirmata = schirmata;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = thema_datum;
        manager->widgets[I].reddere = _schirmata_thema_visus_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_thema_visus_tractare_eventum;
    }

    schirma->modus_arx_caeli = FALSUM;
    schirma->modus_thema_visus = VERUM;
    schirma->modus_sputnik_syntaxis = FALSUM;
    schirma->modus_biblia_visus = FALSUM;
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
    syntaxis_datum->sputnik_syntaxis = schirma->sputnik_syntaxis;
    syntaxis_datum->schirmata = schirmata;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = syntaxis_datum;
        manager->widgets[I].reddere = _schirmata_sputnik_syntaxis_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_sputnik_syntaxis_tractare_eventum;
    }

    schirma->modus_arx_caeli = FALSUM;
    schirma->modus_thema_visus = FALSUM;
    schirma->modus_sputnik_syntaxis = VERUM;
    schirma->modus_biblia_visus = FALSUM;
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
    biblia_datum->biblia_visus = schirma->biblia_visus;
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
