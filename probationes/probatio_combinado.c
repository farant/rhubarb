#include "piscina.h"
#include "graphus_entitatum.h"
#include "navigator_entitatum.h"
#include "pagina.h"
#include "fenestra.h"
#include "thema.h"
#include "internamentum.h"
#include "tempus.h"
#include "widget.h"
#include <stdio.h>

#define LATITUDO_FENESTRA  DCCCLIII  /* 853 */
#define ALTITUDO_FENESTRA  CDLXXX    /* 480 */

/* Creare graphum probationis cum aliquot entitates */
interior vacuum
creare_graphum_probationis(
    GraphusEntitatum* graphus)
{
    InternamentumChorda* intern;
    Entitas*             radix;
    Entitas*             pagina_1;
    Entitas*             pagina_2;
    Entitas*             pagina_3;
    Entitas*             nota;
    chorda*              id;
    chorda*              genus;
    chorda*              clavis;
    chorda*              valor;
    chorda*              rel_genus;
    chorda*              nota_root;

    intern = graphus_entitatum_internamentum(graphus);

    /* === Entitas Radix === */
    id = chorda_internare_ex_literis(intern, "root");
    genus = chorda_internare_ex_literis(intern, "Root");
    radix = entitas_creare(graphus_entitatum_internamentum(graphus)->piscina, id, genus);

    /* Proprietates radix */
    clavis = chorda_internare_ex_literis(intern, "name");
    valor = chorda_internare_ex_literis(intern, "System Root");
    entitas_proprietas_addere(radix, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "description");
    valor = chorda_internare_ex_literis(intern, "Root of the entity graph");
    entitas_proprietas_addere(radix, clavis, valor);

    /* Nota radix */
    nota_root = chorda_internare_ex_literis(intern, "#root");
    entitas_nota_addere(radix, nota_root);

    graphus_entitatum_addere_entitatem(graphus, radix);

    /* === Pagina 1 === */
    id = chorda_internare_ex_literis(intern, "page:001");
    genus = chorda_internare_ex_literis(intern, "Page");
    pagina_1 = entitas_creare(graphus_entitatum_internamentum(graphus)->piscina, id, genus);

    clavis = chorda_internare_ex_literis(intern, "name");
    valor = chorda_internare_ex_literis(intern, "Introduction");
    entitas_proprietas_addere(pagina_1, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "content");
    valor = chorda_internare_ex_literis(intern, "Welcome to the entity navigator");
    entitas_proprietas_addere(pagina_1, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "author");
    valor = chorda_internare_ex_literis(intern, "Fran");
    entitas_proprietas_addere(pagina_1, clavis, valor);

    graphus_entitatum_addere_entitatem(graphus, pagina_1);

    /* === Pagina 2 === */
    id = chorda_internare_ex_literis(intern, "page:002");
    genus = chorda_internare_ex_literis(intern, "Page");
    pagina_2 = entitas_creare(graphus_entitatum_internamentum(graphus)->piscina, id, genus);

    clavis = chorda_internare_ex_literis(intern, "name");
    valor = chorda_internare_ex_literis(intern, "Getting Started");
    entitas_proprietas_addere(pagina_2, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "content");
    valor = chorda_internare_ex_literis(intern, "Use j/k to navigate, l to enter, h to go back");
    entitas_proprietas_addere(pagina_2, clavis, valor);

    graphus_entitatum_addere_entitatem(graphus, pagina_2);

    /* === Pagina 3 === */
    id = chorda_internare_ex_literis(intern, "page:003");
    genus = chorda_internare_ex_literis(intern, "Page");
    pagina_3 = entitas_creare(graphus_entitatum_internamentum(graphus)->piscina, id, genus);

    clavis = chorda_internare_ex_literis(intern, "name");
    valor = chorda_internare_ex_literis(intern, "Navigation");
    entitas_proprietas_addere(pagina_3, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "content");
    valor = chorda_internare_ex_literis(intern, "The graph structure allows bidirectional traversal");
    entitas_proprietas_addere(pagina_3, clavis, valor);

    graphus_entitatum_addere_entitatem(graphus, pagina_3);

    /* === Nota === */
    id = chorda_internare_ex_literis(intern, "note:001");
    genus = chorda_internare_ex_literis(intern, "Note");
    nota = entitas_creare(graphus_entitatum_internamentum(graphus)->piscina, id, genus);

    clavis = chorda_internare_ex_literis(intern, "name");
    valor = chorda_internare_ex_literis(intern, "Important");
    entitas_proprietas_addere(nota, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "text");
    valor = chorda_internare_ex_literis(intern, "Remember to test thoroughly");
    entitas_proprietas_addere(nota, clavis, valor);

    graphus_entitatum_addere_entitatem(graphus, nota);

    /* === Relationes === */
    /* Radix -> Paginae */
    rel_genus = chorda_internare_ex_literis(intern, "pages");
    entitas_relatio_addere(radix, rel_genus, pagina_1->id);
    entitas_relatio_addere(radix, rel_genus, pagina_2->id);
    entitas_relatio_addere(radix, rel_genus, pagina_3->id);

    /* Radix -> Notae */
    rel_genus = chorda_internare_ex_literis(intern, "notes");
    entitas_relatio_addere(radix, rel_genus, nota->id);

    /* Pagina 1 -> Pagina 2 */
    rel_genus = chorda_internare_ex_literis(intern, "next");
    entitas_relatio_addere(pagina_1, rel_genus, pagina_2->id);

    /* Pagina 2 -> Pagina 1, Pagina 3 */
    rel_genus = chorda_internare_ex_literis(intern, "prev");
    entitas_relatio_addere(pagina_2, rel_genus, pagina_1->id);
    rel_genus = chorda_internare_ex_literis(intern, "next");
    entitas_relatio_addere(pagina_2, rel_genus, pagina_3->id);

    /* Pagina 3 -> Pagina 2 */
    rel_genus = chorda_internare_ex_literis(intern, "prev");
    entitas_relatio_addere(pagina_3, rel_genus, pagina_2->id);

    /* Pagina 1 -> Nota */
    rel_genus = chorda_internare_ex_literis(intern, "references");
    entitas_relatio_addere(pagina_1, rel_genus, nota->id);

    imprimere("Graphus probationis creatus:\n");
    imprimere("  Radix: %d entitates\n", graphus_entitatum_numerus(graphus));
}

/* ==================================================
 * Widget Wrapper Functions
 * ================================================== */

/* Datum pro widget pagina */
nomen structura {
    Pagina*  pagina;
    Piscina* piscina;
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
    EntitasProvidor*     providor;
    NavigatorEntitatum*  navigator;
    Pagina               pagina;
    Fenestra*            fenestra;
    TabulaPixelorum*     tabula;
    FenestraConfiguratio configuratio;
    Eventus              eventus;
    b32                  currens;
    ManagerWidget*       manager;
    DatumWidgetPagina*   datum_pagina;
    DatumWidgetNavigator* datum_navigator;

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

    /* Creare entitates probationis */
    creare_graphum_probationis(graphus);

    /* Creare providor */
    providor = graphus_entitatum_providor_creare(graphus);
    si (!providor)
    {
        imprimere("Fractura: non potest creare providor\n");
        redde I;
    }

    /* Creare navigator */
    navigator = navigator_entitatum_creare(piscina, providor);
    si (!navigator)
    {
        imprimere("Fractura: non potest creare navigator\n");
        redde I;
    }

    /* Initiare paginam */
    pagina_initiare(&pagina, "page:demo");
    pagina_inserere_chordam(&pagina, "TAB = switch focus (not in insert mode)\n\n");
    pagina_inserere_chordam(&pagina, "Click widgets to focus them\n\n");
    pagina_inserere_chordam(&pagina, "Navigator on right ->\n\n");
    pagina_inserere_chordam(&pagina, "Press 'i' to enter insert mode\n");
    pagina_inserere_chordam(&pagina, "Press ESC to return to normal mode\n");
    pagina_inserere_chordam(&pagina, "Use hjkl to navigate\n");

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

        /* Vacare fondum */
        tabula_pixelorum_vacare(tabula, thema_color(COLOR_BACKGROUND));

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
