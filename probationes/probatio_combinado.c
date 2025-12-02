#include "piscina.h"
#include "entitas_repositorium.h"
#include "persistentia.h"
#include "fenestra.h"
#include "delineare.h"
#include "thema.h"
#include "internamentum.h"
#include "tempus.h"
#include "layout.h"
#include "libro_paginarum.h"
#include "registrum_commandi.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#define LATITUDO_FENESTRA  DCCCLIII  /* 853 */
#define ALTITUDO_FENESTRA  CDLXXX    /* 480 */

/* STML layout definition cum widgets et entitates */
constans character* LAYOUT_STML =
    "<layout>"
    "  <libro! id='editor' x='0' y='0' latitudo='71' altitudo='60'>"
    "LibroPaginarum Demo - Multi-Page Editor\n"
    "\n"
    "Navigation:\n"
    "  Ctrl+Shift+Right = next page\n"
    "  Ctrl+Shift+Left  = previous page\n"
    "  Click #next or #prev links\n"
    "  Click #back to go back in history\n"
    "\n"
    "Click $date to insert date\n"
    "\n"
    "Press 'i' to enter insert mode\n"
    "Press ESC to return to normal mode\n"
    "Use hjkl to navigate\n"
    "\n"
    "--- Sputnik Example ---\n"
    "<sputnik>\n"
    "sit x = 42;\n"
    "constans PI = 3.14;\n"
    "\n"
    "si (x > 10) {\n"
    "    print(\"magnum!\");\n"
    "}\n"
    "\n"
    "functio salve(nomen) {\n"
    "    redde `Hello ${nomen}`;\n"
    "}\n"
    "</sputnik>\n"
    "</libro>"
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

/* Legere argumentum ex textu post command (usque ad finem lineae vel spatium)
 * Redde: longitudo argumenti
 */
interior i32
_legere_argumentum(
    ContextusCommandi* ctx,
    character*         buffer,
    i32                max_long)
{
    i32 i;
    character c;

    /* Skip leading space */
    si (ctx->columna < ctx->pagina->tabula.latitudo)
    {
        c = tabula_cellula(&ctx->pagina->tabula, ctx->linea, ctx->columna);
        si (c == ' ')
        {
            ctx->columna++;
        }
    }

    /* Read until space or end of line */
    per (i = ZEPHYRUM; i < max_long - I && (s32)(ctx->columna + i) < (s32)ctx->pagina->tabula.latitudo; i++)
    {
        c = tabula_cellula(&ctx->pagina->tabula, ctx->linea, ctx->columna + i);
        si (c == '\0' || c == ' ' || c == '\n')
        {
            frange;
        }
        buffer[i] = c;
    }
    buffer[i] = '\0';

    redde i;
}


/* $rename <name> - rename current page */
interior b32
command_rename(
    ContextusCommandi* ctx)
{
    LibroPaginarum* libro;
    character titulus[LXIV];
    i32 longitudo;
    i32 index;

    libro = (LibroPaginarum*)ctx->datum_custom;
    si (!libro)
    {
        redde FALSUM;
    }

    longitudo = _legere_argumentum(ctx, titulus, LXIV);
    si (longitudo == ZEPHYRUM)
    {
        redde FALSUM;  /* No name provided */
    }

    index = libro_index_currens(libro);
    libro_pagina_nominare(libro, index, titulus);

    redde VERUM;
}


/* $goto <N> - go to page N */
interior b32
command_goto(
    ContextusCommandi* ctx)
{
    LibroPaginarum* libro;
    character num_buffer[XVI];
    i32 longitudo;
    s32 page_num;
    i32 i;

    libro = (LibroPaginarum*)ctx->datum_custom;
    si (!libro)
    {
        redde FALSUM;
    }

    longitudo = _legere_argumentum(ctx, num_buffer, XVI);
    si (longitudo == ZEPHYRUM)
    {
        redde FALSUM;  /* No number provided */
    }

    /* Parse integer */
    page_num = ZEPHYRUM;
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        si (num_buffer[i] >= '0' && num_buffer[i] <= '9')
        {
            page_num = page_num * X + (s32)(num_buffer[i] - '0');
        }
    }

    libro_navigare_ad(libro, page_num);

    redde VERUM;
}


/* $new - create new page and navigate to it */
interior b32
command_new(
    ContextusCommandi* ctx)
{
    LibroPaginarum* libro;
    s32 new_index;

    libro = (LibroPaginarum*)ctx->datum_custom;
    si (!libro)
    {
        redde FALSUM;
    }

    new_index = libro_pagina_nova(libro, NIHIL);
    si (new_index >= ZEPHYRUM)
    {
        libro_navigare_ad(libro, new_index);
    }

    redde VERUM;
}


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

    /* Obtinere libro ex layout pro commands */
    {
        LibroPaginarum* libro;
        libro = layout_obtinere_libro(dom, "editor");

        /* Registrare commands */
        registrum_commandi_registrare(reg_commandi, "date", command_date, NIHIL);
        registrum_commandi_registrare(reg_commandi, "rename", command_rename, libro);
        registrum_commandi_registrare(reg_commandi, "goto", command_goto, libro);
        registrum_commandi_registrare(reg_commandi, "new", command_new, libro);
    }

    /* Ponere registrum in layout */
    layout_ponere_reg_commandi(dom, reg_commandi);

    /* Configurare fenestram */
    configuratio.titulus = "LibroPaginarum + Navigator Demo";
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
