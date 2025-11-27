#include "piscina.h"
#include "graphus_entitatum.h"
#include "entitas_repositorium.h"
#include "navigator_entitatum.h"
#include "pagina.h"
#include "fenestra.h"
#include "delineare.h"
#include "thema.h"
#include "internamentum.h"
#include "tempus.h"
#include "widget.h"
#include "registrum_commandi.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#define LATITUDO_FENESTRA  DCCCLIII  /* 853 */
#define ALTITUDO_FENESTRA  CDLXXX    /* 480 */

/* Creare graphum probationis cum aliquot entitates */
interior vacuum
creare_graphum_probationis(
    EntitasRepositorium* repo)
{
    Entitas* radix;
    Entitas* pagina_1;
    Entitas* pagina_2;
    Entitas* pagina_3;
    Entitas* nota;

    /* === Entitas Radix === */
    radix = repo->entitas_scaffoldare(repo->datum, "Root", "system");
    repo->proprietas_ponere(repo->datum, radix, "name", "System Root");
    repo->proprietas_ponere(repo->datum, radix, "description", "Root of the entity graph");
    repo->nota_addere(repo->datum, radix, "#root");

    /* === Pagina 1 === */
    pagina_1 = repo->entitas_scaffoldare(repo->datum, "Page", "introduction");
    repo->proprietas_ponere(repo->datum, pagina_1, "name", "Introduction");
    repo->proprietas_ponere(repo->datum, pagina_1, "content", "Welcome to the entity navigator");
    repo->proprietas_ponere(repo->datum, pagina_1, "author", "Fran");

    /* === Pagina 2 === */
    pagina_2 = repo->entitas_scaffoldare(repo->datum, "Page", "getting-started");
    repo->proprietas_ponere(repo->datum, pagina_2, "name", "Getting Started");
    repo->proprietas_ponere(repo->datum, pagina_2, "content", "Use j/k to navigate, l to enter, h to go back");

    /* === Pagina 3 === */
    pagina_3 = repo->entitas_scaffoldare(repo->datum, "Page", "navigation");
    repo->proprietas_ponere(repo->datum, pagina_3, "name", "Navigation");
    repo->proprietas_ponere(repo->datum, pagina_3, "content", "The graph structure allows bidirectional traversal");

    /* === Nota === */
    nota = repo->entitas_scaffoldare(repo->datum, "Note", "important");
    repo->proprietas_ponere(repo->datum, nota, "name", "Important");
    repo->proprietas_ponere(repo->datum, nota, "text", "Remember to test thoroughly");

    /* === Relationes === */
    /* Radix -> Paginae */
    repo->relatio_addere(repo->datum, radix, "pages", pagina_1->id);
    repo->relatio_addere(repo->datum, radix, "pages", pagina_2->id);
    repo->relatio_addere(repo->datum, radix, "pages", pagina_3->id);

    /* Radix -> Notae */
    repo->relatio_addere(repo->datum, radix, "notes", nota->id);

    /* Pagina 1 -> Pagina 2 */
    repo->relatio_addere(repo->datum, pagina_1, "next", pagina_2->id);

    /* Pagina 2 -> Pagina 1, Pagina 3 */
    repo->relatio_addere(repo->datum, pagina_2, "prev", pagina_1->id);
    repo->relatio_addere(repo->datum, pagina_2, "next", pagina_3->id);

    /* Pagina 3 -> Pagina 2 */
    repo->relatio_addere(repo->datum, pagina_3, "prev", pagina_2->id);

    /* Pagina 1 -> Nota */
    repo->relatio_addere(repo->datum, pagina_1, "references", nota->id);

    imprimere("Graphus probationis creatus\n");
}

/* ==================================================
 * Commands
 * ================================================== */

/* $date command - insert current date */
interior b32
command_date(
    ContextusCommandi* ctx)
{
    character resultado[XXXII];
    time_t tempus_nunc;
    structura tm* tempus_info;

    /* Get current time */
    tempus_nunc = time(NIHIL);
    tempus_info = localtime(&tempus_nunc);

    /* Format: MM/DD/YYYY */
    sprintf(resultado, " %02d/%02d/%04d",
            tempus_info->tm_mon + I,
            tempus_info->tm_mday,
            tempus_info->tm_year + MCMX);  /* 1900 */

    /* Insert result after tag */
    pagina_ponere_cursor(ctx->pagina, ctx->positio);
    pagina_inserere_chordam(ctx->pagina, resultado);

    redde VERUM;
}


/* ==================================================
 * Widget Wrapper Functions
 * ================================================== */

/* Datum pro widget pagina */
nomen structura {
    Pagina*  pagina;
    Piscina* piscina;
    RegistrumCommandi* reg_commandi;
} DatumWidgetPagina;

/* Datum pro widget navigator */
nomen structura {
    NavigatorEntitatum* navigator;
    Piscina*            piscina;
} DatumWidgetNavigator;

/* Wrapper pro reddere paginam (REVISED) */
interior vacuum
widget_pagina_reddere_revised(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    DatumWidgetPagina* datum;

    datum = (DatumWidgetPagina*)widget->datum;

    pagina_reddere_cum_margine(
        datum->piscina,
        tabula,
        datum->pagina,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

/* Wrapper pro tractare eventum paginae */
interior b32
widget_pagina_tractare_eventum(
    Widget*          widget,
    constans Eventus* eventus)
{
    DatumWidgetPagina* datum;
    b32 consumptus;

    datum = (DatumWidgetPagina*)widget->datum;

    /* Handle mouse clicks for tag detection */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS)
    {
        RegioClicca regio;
        i32 click_x;
        i32 click_y;
        i32 character_latitudo;
        i32 character_altitudo;

        character_latitudo = VI;  /* 6 pixels per character */
        character_altitudo = VIII;

        /* Convert pixel to character coordinates */
        /* Account for widget position and border */
        click_x = (eventus->datum.mus.x / character_latitudo) - widget->x - I;
        click_y = (eventus->datum.mus.y / character_altitudo) - widget->y - I;

        /* Try to detect tag at click position */
        si (pagina_obtinere_regio_ad_punctum(datum->pagina, click_x, click_y, &regio))
        {
            /* Tag detected! */
            si (strcmp(regio.genus, "command") == ZEPHYRUM)
            {
                ContextusCommandi ctx;

                ctx.pagina = datum->pagina;
                ctx.positio = regio.finis;
                ctx.piscina = datum->piscina;
                ctx.datum_custom = NIHIL;

                /* Execute command through registry */
                registrum_commandi_executare(datum->reg_commandi, regio.datum, &ctx);

                redde VERUM;  /* Consumed click */
            }
        }
    }

    consumptus = pagina_tractare_eventum(datum->pagina, eventus);

    /* Si pagina reddidit FALSUM (ESC in normal mode), non consumere */
    /* Hoc permittet application tractare quit */
    redde consumptus;
}

/* Wrapper pro reddere navigator */
interior vacuum
widget_navigator_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    DatumWidgetNavigator* datum;

    datum = (DatumWidgetNavigator*)widget->datum;

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

/* Wrapper pro tractare eventum navigator */
interior b32
widget_navigator_tractare_eventum(
    Widget*          widget,
    constans Eventus* eventus)
{
    DatumWidgetNavigator* datum;
    b32 consumptus;

    datum = (DatumWidgetNavigator*)widget->datum;

    consumptus = navigator_entitatum_tractare_eventum(datum->navigator, eventus);

    /* Passare consumptionem ad manager */
    redde consumptus;
}

int
main(void)
{
    Piscina*             piscina;
    GraphusEntitatum*    graphus;
    EntitasRepositorium* repositorium;
    NavigatorEntitatum*  navigator;
    Pagina               pagina;
    Fenestra*            fenestra;
    TabulaPixelorum*     tabula;
    ContextusDelineandi* ctx;
    FenestraConfiguratio configuratio;
    Eventus              eventus;
    b32                  currens;
    ManagerWidget*       manager;
    DatumWidgetPagina*   datum_pagina;
    DatumWidgetNavigator* datum_navigator;
    RegistrumCommandi*   registrum_commandi;

    /* Initiare thema */
    thema_initiare();

    /* Initiare tempus */
    tempus_initiare();

    /* Creare piscinam */
    piscina = piscina_generare_dynamicum("combinado", M * M * II);  /* 2MB */
    si (!piscina)
    {
        imprimere("Fractura: non potest creare piscinam\n");
        redde I;
    }

    /* Creare graphum */
    graphus = graphus_entitatum_creare(piscina);
    si (!graphus)
    {
        imprimere("Fractura: non potest creare graphum\n");
        redde I;
    }

    /* Creare repositorium */
    repositorium = graphus_entitatum_repositorium_creare(graphus);
    si (!repositorium)
    {
        imprimere("Fractura: non potest creare repositorium\n");
        redde I;
    }

    /* Creare entitates probationis */
    creare_graphum_probationis(repositorium);

    /* Creare navigator */
    navigator = navigator_entitatum_creare(piscina, repositorium);
    si (!navigator)
    {
        imprimere("Fractura: non potest creare navigator\n");
        redde I;
    }

    /* Initiare paginam */
    pagina_initiare(&pagina, chorda_internare_ex_literis(internamentum_globale(), "page:demo"));
    pagina_inserere_chordam(&pagina, "TAB = switch focus (not in insert mode)\n\n");
    pagina_inserere_chordam(&pagina, "Click widgets to focus them\n\n");
    pagina_inserere_chordam(&pagina, "Navigator on right ->\n\n");
    pagina_inserere_chordam(&pagina, "Press 'i' to enter insert mode\n");
    pagina_inserere_chordam(&pagina, "Press ESC to return to normal mode\n");
    pagina_inserere_chordam(&pagina, "Use hjkl to navigate\n\n");
    pagina_inserere_chordam(&pagina, "Try clicking on: $date\n");

    /* Create command registry */
    registrum_commandi = registrum_commandi_creare(piscina);
    si (!registrum_commandi)
    {
        imprimere("Fractura: non potest creare registrum commandi\n");
        redde I;
    }

    /* Register commands */
    registrum_commandi_registrare(registrum_commandi, "date", command_date, NIHIL);

    /* Creare widget manager */
    manager = manager_widget_creare(piscina);
    si (!manager)
    {
        imprimere("Fractura: non potest creare widget manager\n");
        redde I;
    }

    /* Creare datum structuras pro widgets */
    datum_pagina = piscina_allocare(piscina, magnitudo(DatumWidgetPagina));
    datum_pagina->pagina = &pagina;
    datum_pagina->piscina = piscina;
    datum_pagina->reg_commandi = registrum_commandi;

    datum_navigator = piscina_allocare(piscina, magnitudo(DatumWidgetNavigator));
    datum_navigator->navigator = navigator;
    datum_navigator->piscina = piscina;

    /* Registrare widgets */
    manager_widget_registrare(
        manager,
        datum_pagina,
        widget_pagina_reddere_revised,
        widget_pagina_tractare_eventum,
        ZEPHYRUM,  /* x */
        ZEPHYRUM,  /* y */
        LXXI,      /* latitudo (71 chars) */
        LX);       /* altitudo (60 lines) */

    manager_widget_registrare(
        manager,
        datum_navigator,
        widget_navigator_reddere,
        widget_navigator_tractare_eventum,
        LXXI,      /* x (71 chars offset) */
        ZEPHYRUM,  /* y */
        LXXI,      /* latitudo (71 chars) */
        LX);       /* altitudo (60 lines) */

    /* Configurare fenestram */
    configuratio.titulus = "Pagina + Navigator - Widget Manager Demo";
    configuratio.x = C;
    configuratio.y = C;
    configuratio.latitudo = LATITUDO_FENESTRA;
    configuratio.altitudo = ALTITUDO_FENESTRA;
    configuratio.vexilla = FENESTRA_ORDINARIA;

    /* Creare fenestram */
    fenestra = fenestra_creare(piscina, &configuratio);
    si (!fenestra)
    {
        imprimere("Fractura: non potest creare fenestram\n");
        redde I;
    }

    /* Creare tabulam pixelorum */
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, ALTITUDO_FENESTRA);
    si (!tabula)
    {
        imprimere("Fractura: non potest creare tabulam\n");
        fenestra_destruere(fenestra);
        redde I;
    }

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina, tabula);
    si (!ctx)
    {
        imprimere("Fractura: non potest creare contextum\n");
        fenestra_destruere(fenestra);
        redde I;
    }

    /* Monstrare fenestram */
    fenestra_monstrare(fenestra);

    /* Cyclus principalis */
    currens = VERUM;

    dum (currens && !fenestra_debet_claudere(fenestra))
    {
        /* Actualizare tempus */
        tempus_quadrum();

        /* Perscrutari eventus */
        fenestra_perscrutari_eventus(fenestra);

        /* Tractare eventus */
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            si (eventus.genus == EVENTUS_CLAUDERE)
            {
                currens = FALSUM;
            }
            alioquin
            {
                b32 tractatus;

                /* Manager tractat omnes eventus (focus, routing, etc) */
                tractatus = manager_widget_tractare_eventum(manager, &eventus);

                /* Si pagina reddidit FALSUM (ESC in normal mode), exire */
                si (!tractatus && eventus.genus == EVENTUS_CLAVIS_DEPRESSUS)
                {
                    si (eventus.datum.clavis.clavis == CLAVIS_EFFUGIUM)
                    {
                        currens = FALSUM;
                    }
                }
            }
        }

        /* Vacare fondum cum colore calido griseo */
        tabula_pixelorum_vacare(tabula,
            color_ad_pixelum(thema_color(COLOR_BACKGROUND)));

        /* Manager reddit omnes widgets */
        manager_widget_reddere(manager, tabula, I);

        /* Praesentare pixela */
        fenestra_praesentare_pixela(fenestra, tabula);
    }

    /* Purgare */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
