#include "piscina.h"
#include "entitas_repositorium.h"
#include "persistentia.h"
#include "fenestra.h"
#include "delineare.h"
#include "thema.h"
#include "internamentum.h"
#include "tempus.h"
#include "layout.h"
#include "registrum_commandi.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#define LATITUDO_FENESTRA  DCCCLIII  /* 853 */
#define ALTITUDO_FENESTRA  CDLXXX    /* 480 */

/* STML layout definition cum widgets et entitates */
constans character* LAYOUT_STML =
    "<layout>"
    "  <pagina id='editor' x='0' y='0' latitudo='71' altitudo='60'/>"
    "  <navigator id='nav' x='71' y='0' latitudo='71' altitudo='60'/>"
    ""
    "  <entitas genus='Root' slug='system'>"
    "    <proprietas clavis='name' valor='System Root'/>"
    "    <proprietas clavis='description' valor='Root of the entity graph'/>"
    "    <nota>#root</nota>"
    "    <relatio genus='pages' ad='Page::introduction'/>"
    "    <relatio genus='pages' ad='Page::getting-started'/>"
    "    <relatio genus='pages' ad='Page::navigation'/>"
    "    <relatio genus='notes' ad='Note::important'/>"
    "  </entitas>"
    ""
    "  <entitas genus='Page' slug='introduction'>"
    "    <proprietas clavis='name' valor='Introduction'/>"
    "    <proprietas clavis='content' valor='Welcome to the entity navigator'/>"
    "    <proprietas clavis='author' valor='Fran'/>"
    "    <relatio genus='next' ad='Page::getting-started'/>"
    "    <relatio genus='references' ad='Note::important'/>"
    "  </entitas>"
    ""
    "  <entitas genus='Page' slug='getting-started'>"
    "    <proprietas clavis='name' valor='Getting Started'/>"
    "    <proprietas clavis='content' valor='Use j/k to navigate, l to enter, h to go back'/>"
    "    <relatio genus='prev' ad='Page::introduction'/>"
    "    <relatio genus='next' ad='Page::navigation'/>"
    "  </entitas>"
    ""
    "  <entitas genus='Page' slug='navigation'>"
    "    <proprietas clavis='name' valor='Navigation'/>"
    "    <proprietas clavis='content' valor='The graph structure allows bidirectional traversal'/>"
    "    <relatio genus='prev' ad='Page::getting-started'/>"
    "  </entitas>"
    ""
    "  <entitas genus='Note' slug='important'>"
    "    <proprietas clavis='name' valor='Important'/>"
    "    <proprietas clavis='text' valor='Remember to test thoroughly'/>"
    "  </entitas>"
    ""
    "  <!-- Entitates cum genere hierarchico -->"
    "  <entitas genus='Application-State::Widget' slug='editor-widget'>"
    "    <proprietas clavis='name' valor='Editor Widget'/>"
    "    <proprietas clavis='type' valor='text-area' typus='WidgetType'/>"
    "    <proprietas clavis='x' valor='0' typus='Coordinate' literalis='s32'/>"
    "    <proprietas clavis='y' valor='0' typus='Coordinate' literalis='s32'/>"
    "    <proprietas clavis='width' valor='71' typus='Dimension' literalis='s32'/>"
    "    <proprietas clavis='height' valor='60' typus='Dimension' literalis='s32'/>"
    "  </entitas>"
    ""
    "  <entitas genus='Application-State::Widget' slug='nav-widget'>"
    "    <proprietas clavis='name' valor='Navigator Widget'/>"
    "    <proprietas clavis='type' valor='entity-browser' typus='WidgetType'/>"
    "    <proprietas clavis='x' valor='71' typus='Coordinate' literalis='s32'/>"
    "    <proprietas clavis='y' valor='0' typus='Coordinate' literalis='s32'/>"
    "    <proprietas clavis='width' valor='71' typus='Dimension' literalis='s32'/>"
    "    <proprietas clavis='height' valor='60' typus='Dimension' literalis='s32'/>"
    "  </entitas>"
    ""
    "  <entitas genus='Application-State::Settings' slug='theme'>"
    "    <proprietas clavis='name' valor='Theme Settings'/>"
    "    <proprietas clavis='dark-mode' valor='true' typus='Boolean' literalis='b32'/>"
    "  </entitas>"
    ""
    "  <entitas genus='Content::Document::Article' slug='first-post'>"
    "    <proprietas clavis='title' valor='First Article'/>"
    "    <proprietas clavis='published' valor='2025-01-15' typus='Date'/>"
    "  </entitas>"
    ""
    "  <entitas genus='Content::Document::Article' slug='second-post'>"
    "    <proprietas clavis='title' valor='Second Article'/>"
    "    <proprietas clavis='published' valor='2025-02-20' typus='Date'/>"
    "  </entitas>"
    ""
    "  <entitas genus='Content::Document::Note' slug='meeting-notes'>"
    "    <proprietas clavis='title' valor='Meeting Notes'/>"
    "    <proprietas clavis='date' valor='2025-03-01'/>"
    "  </entitas>"
    ""
    "  <entitas genus='Content::Media' slug='logo'>"
    "    <proprietas clavis='name' valor='Application Logo'/>"
    "    <proprietas clavis='format' valor='png'/>"
    "  </entitas>"
    "</layout>";

/* ==================================================
 * Commands
 * ================================================== */

/* $date command - insert/update current date in output region */
interior b32
command_date(
    ContextusCommandi* ctx)
{
    character expectatum[XIII];  /* " MM/DD/YYYY\0" = 12 chars */
    character contentum[XIII];
    i32 longitudo;
    time_t tempus_nunc;
    structura tm* tempus_info;

    /* Generare datum expectatum */
    tempus_nunc = time(NIHIL);
    tempus_info = localtime(&tempus_nunc);
    sprintf(expectatum, " %02d/%02d/%04d",
            tempus_info->tm_mon + I,
            tempus_info->tm_mday,
            tempus_info->tm_year + MCMX);  /* 1900 */

    longitudo = XI;  /* " MM/DD/YYYY" = 11 characters */

    /* Legere contentum currentem ad positionem */
    tabula_legere_ad_positionem(&ctx->pagina->tabula,
        ctx->linea, ctx->columna, contentum, longitudo);

    /* Si iam correctum, nihil facere */
    si (memcmp(contentum, expectatum, (size_t)longitudo) == ZEPHYRUM)
    {
        redde VERUM;
    }

    /* Aliter: inserere spatium et scribere */
    si (!tabula_inserere_spatium(&ctx->pagina->tabula,
        ctx->linea, ctx->columna, longitudo))
    {
        redde FALSUM;  /* Pagina plena */
    }

    tabula_scribere_ad_positionem(&ctx->pagina->tabula,
        ctx->linea, ctx->columna, expectatum);

    redde VERUM;
}


int
main(void)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    Persistentia*        persistentia;
    EntitasRepositorium* repositorium;
    LayoutDom*           dom;
    Pagina*              pagina;
    Fenestra*            fenestra;
    TabulaPixelorum*     tabula;
    ContextusDelineandi* ctx;
    FenestraConfiguratio configuratio;
    Eventus              eventus;
    b32                  currens;
    RegistrumCommandi*   reg_commandi;

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

    /* Obtinere internamentum */
    intern = internamentum_globale();

    /* Creare persistentia (in memoria) */
    persistentia = persistentia_memoria_creare(piscina);
    si (!persistentia)
    {
        imprimere("Fractura: non potest creare persistentiam\n");
        redde I;
    }

    /* Creare repositorium */
    repositorium = entitas_repositorium_creare(piscina, persistentia);
    si (!repositorium)
    {
        imprimere("Fractura: non potest creare repositorium\n");
        redde I;
    }

    /* Creare layout ex STML (entitates creantur declarative in STML) */
    dom = layout_creare(piscina, intern, LAYOUT_STML, repositorium);
    si (!dom)
    {
        imprimere("Fractura: non potest creare layout\n");
        redde I;
    }

    /* Creare registrum commandi */
    reg_commandi = registrum_commandi_creare(piscina);
    si (!reg_commandi)
    {
        imprimere("Fractura: non potest creare registrum commandi\n");
        redde I;
    }

    /* Registrare commands */
    registrum_commandi_registrare(reg_commandi, "date", command_date, NIHIL);

    /* Ponere registrum in layout */
    layout_ponere_reg_commandi(dom, reg_commandi);

    /* Obtinere pagina ex layout et inserere textum */
    pagina = layout_obtinere_pagina(dom, "editor");
    si (pagina)
    {
        pagina_inserere_textum(pagina, "TAB = switch focus (not in insert mode)\n\n");
        pagina_inserere_textum(pagina, "Click widgets to focus them\n\n");
        pagina_inserere_textum(pagina, "Navigator on right ->\n\n");
        pagina_inserere_textum(pagina, "Press 'i' to enter insert mode\n");
        pagina_inserere_textum(pagina, "Press ESC to return to normal mode\n");
        pagina_inserere_textum(pagina, "Use hjkl to navigate\n\n");
        pagina_inserere_textum(pagina, "Try clicking on: $date\n");
    }

    /* Configurare fenestram */
    configuratio.titulus = "Pagina + Navigator - Layout Demo";
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
                tractatus = manager_widget_tractare_eventum(dom->manager, &eventus);

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
        manager_widget_reddere(dom->manager, tabula, I);

        /* Praesentare pixela */
        fenestra_praesentare_pixela(fenestra, tabula);
    }

    /* Purgare */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
