#include "thema_visus.h"
#include "thema.h"
#include "color.h"
#include "delineare.h"
#include "chorda.h"
#include "registrum_commandi.h"
#include "internamentum.h"
#include "entitas.h"
#include <string.h>
#include <stdio.h>

/* ==================================================
 * Constantae
 * ================================================== */

#define CHAR_LATITUDO   VI      /* 6 pixels per character */
#define CHAR_ALTITUDO   VIII    /* 8 pixels per character */
#define SWATCH_LATITUDO VIII    /* 8 characters wide */
#define PADDING         II      /* 2 characters padding */

/* Paginatio */
#define THEMA_PAGINA_COLORES     ZEPHYRUM
#define THEMA_PAGINA_EXEMPLARIA  I
#define THEMA_PAGINA_MAXIMA      I

/* Exemplaria layout */
#define EXEMPLAR_SWATCH_LAT      XLVIII /* 48 pixels (2x wider) */
#define EXEMPLAR_SWATCH_ALT      XXXII  /* 32 pixels (2x taller) */
#define EXEMPLARIA_PER_ROW       V      /* 5 patterns per row */
#define EXEMPLAR_NUMERUS         XXXVIII /* 38 patterns */


/* ==================================================
 * Nomina Colorum
 * ================================================== */

hic_manens constans character* nomina_palette[XVI] = {
    "Black",
    "Dark Gray",
    "Med Dark Gray",
    "Medium Gray",
    "Light Gray",
    "White",
    "Dark Red",
    "Blue",
    "Bright Pink",
    "Dark Gold",
    "Medium Gold",
    "Bright Gold",
    "Dark Leaf",
    "Medium Leaf",
    "Bright Leaf",
    "Warm Gray"
};

hic_manens constans character* nomina_semantica[COLOR_COUNT] = {
    "Background",
    "Text",
    "Text Dim",
    "Cursor",
    "Selection",
    "Border",
    "Border Active",
    "Status Normal",
    "Status Insert",
    "Status Visual",
    "Accent Primary",
    "Accent Secondary",
    "Error",
    "Warning",
    "Success"
};



/* ==================================================
 * Functiones Auxiliares
 * ================================================== */

/* Creare chorda ex C string (view, non allocatio)
 * NOTA: Hic facit view in stringam staticam - non modificare!
 */
hic_manens chorda
_chorda_ex_cstr(
    constans character* cstr)
{
    chorda fructus;
    /* Caute: haec est view, non copia. Chorda non debet modificari. */
    union { constans character* c; i8* m; } u;
    u.c = cstr;

    fructus.datum = u.m;
    fructus.mensura = (i32)strlen(cstr);

    redde fructus;
}


/* ==================================================
 * Creatio
 * ================================================== */

ThemaVisus*
thema_visus_creare(
    Piscina* piscina)
{
    ThemaVisus* visus;

    si (!piscina)
    {
        redde NIHIL;
    }

    visus = (ThemaVisus*)piscina_allocare(piscina, magnitudo(ThemaVisus));
    si (!visus)
    {
        redde NIHIL;
    }

    visus->piscina = piscina;
    visus->pagina = THEMA_PAGINA_COLORES;
    visus->exemplar_fg = V;        /* White (good default foreground) */
    visus->exemplar_bg = ZEPHYRUM; /* Black (good default background) */
    visus->modus_fg = VERUM;       /* Start in foreground edit mode */

    redde visus;
}


/* ==================================================
 * Reddere - Pagina Colores
 * ================================================== */

hic_manens vacuum
_thema_visus_reddere_colores(
    ThemaVisus*          visus,
    ContextusDelineandi* ctx,
    TabulaPixelorum*     tabula,
    i32                  x,
    i32                  y,
    i32                  scala,
    i32                  char_lat,
    i32                  char_alt,
    i32                  pixelum_text,
    i32                  pixelum_text_dim,
    Color                color_border)
{
    i32 linea;
    i32 i;
    character numero_buffer[IV];
    chorda numero_chorda;
    chorda nomen_chorda;

    (vacuum)visus;

    linea = y + I;

    /* === Sectio: Contrast Grid === */
    nomen_chorda = _chorda_ex_cstr("=== Contrast Grid ===");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        nomen_chorda, pixelum_text, scala);
    linea += II;

    /* Grid: 16 rows (background) x 16 columns (foreground) */
    {
        i32 row, col;
        i32 cell_lat;
        i32 cell_alt;
        i32 grid_x;
        i32 grid_y;
        chorda aa_chorda;

        aa_chorda = _chorda_ex_cstr("Aa");
        cell_lat = II * char_lat;
        cell_alt = char_alt;
        grid_x = (x + PADDING) * char_lat;

        per (row = ZEPHYRUM; row < XVI; row++)
        {
            Color bg_color;

            bg_color = color_ex_palette(row);
            grid_y = linea * char_alt;

            per (col = ZEPHYRUM; col < XVI; col++)
            {
                Color fg_color;
                i32 cell_x;

                fg_color = color_ex_palette(col);
                cell_x = grid_x + col * cell_lat;

                delineare_rectangulum_plenum(ctx, cell_x, grid_y,
                    cell_lat, cell_alt, bg_color);

                tabula_pixelorum_pingere_chordam_scalatam(tabula,
                    cell_x, grid_y,
                    aa_chorda, color_ad_pixelum(fg_color), scala);
            }

            linea++;
        }
    }

    linea += II;

    /* === Sectio: Palette === */
    {
        i32 palette_x;
        i32 palette_linea;

        palette_x = x + PADDING + XXXIV;
        palette_linea = y + I;

        nomen_chorda = _chorda_ex_cstr("=== Palette ===");
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            palette_x * char_lat, palette_linea * char_alt,
            nomen_chorda, pixelum_text, scala);
        palette_linea += II;

        per (i = ZEPHYRUM; i < XVI; i++)
        {
            Color palette_color;
            i32 swatch_x;
            i32 swatch_y;
            i32 swatch_lat;
            i32 swatch_alt;

            sprintf(numero_buffer, "%2d", i);
            numero_chorda = _chorda_ex_cstr(numero_buffer);
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                palette_x * char_lat, palette_linea * char_alt,
                numero_chorda, pixelum_text_dim, scala);

            nomen_chorda = _chorda_ex_cstr(nomina_palette[i]);
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (palette_x + IV) * char_lat, palette_linea * char_alt,
                nomen_chorda, pixelum_text, scala);

            palette_color = color_ex_palette(i);
            swatch_x = (palette_x + XX) * char_lat;
            swatch_y = palette_linea * char_alt;
            swatch_lat = SWATCH_LATITUDO * char_lat;
            swatch_alt = char_alt;

            delineare_rectangulum_plenum(ctx, swatch_x, swatch_y,
                swatch_lat, swatch_alt, palette_color);
            delineare_rectangulum(ctx, swatch_x, swatch_y,
                swatch_lat, swatch_alt, color_border);

            palette_linea++;
        }
    }

    /* === Sectio: Semantic Mappings === */
    nomen_chorda = _chorda_ex_cstr("=== Semantic Mappings ===");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        nomen_chorda, pixelum_text, scala);
    linea += II;

    per (i = ZEPHYRUM; i < COLOR_COUNT; i++)
    {
        Color semantic_color;
        i32 palette_index;
        i32 swatch_x;
        i32 swatch_y;
        i32 swatch_lat;
        i32 swatch_alt;

        nomen_chorda = _chorda_ex_cstr(nomina_semantica[i]);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            nomen_chorda, pixelum_text, scala);

        palette_index = thema_palette_index((ColorThema)i);
        sprintf(numero_buffer, "%2d", palette_index);

        nomen_chorda = _chorda_ex_cstr("->");
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + XVI) * char_lat, linea * char_alt,
            nomen_chorda, pixelum_text_dim, scala);

        numero_chorda = _chorda_ex_cstr(numero_buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + XIX) * char_lat, linea * char_alt,
            numero_chorda, pixelum_text_dim, scala);

        nomen_chorda = _chorda_ex_cstr(nomina_palette[palette_index]);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + XXII) * char_lat, linea * char_alt,
            nomen_chorda, pixelum_text, scala);

        semantic_color = thema_color((ColorThema)i);
        swatch_x = (x + PADDING + XXXVIII) * char_lat;
        swatch_y = linea * char_alt;
        swatch_lat = IV * char_lat;
        swatch_alt = char_alt;

        delineare_rectangulum_plenum(ctx, swatch_x, swatch_y,
            swatch_lat, swatch_alt, semantic_color);
        delineare_rectangulum(ctx, swatch_x, swatch_y,
            swatch_lat, swatch_alt, color_border);

        linea++;
    }
}


/* ==================================================
 * Reddere - Pagina Exemplaria
 * ================================================== */

hic_manens vacuum
_thema_visus_reddere_exemplaria(
    ThemaVisus*          visus,
    ContextusDelineandi* ctx,
    TabulaPixelorum*     tabula,
    i32                  x,
    i32                  y,
    i32                  scala,
    i32                  char_lat,
    i32                  char_alt,
    i32                  pixelum_text,
    i32                  pixelum_text_dim,
    Color                color_border)
{
    i32 linea;
    i32 i;
    i32 row;
    i32 col;
    character buffer[XXXII];
    chorda text_chorda;
    Color fg_color;
    Color bg_color;
    i32 pixelum_accent;

    linea = y + I;

    /* Titulus */
    text_chorda = _chorda_ex_cstr("=== Pattern Viewer (h/l: page, f/b: mode, j/k: color) ===");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        text_chorda, pixelum_text, scala);
    linea += II;

    /* Color picker status */
    fg_color = color_ex_palette(visus->exemplar_fg);
    bg_color = color_ex_palette(visus->exemplar_bg);
    pixelum_accent = color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY));

    /* FG display */
    sprintf(buffer, "FG: [%2d] %-14s", visus->exemplar_fg, nomina_palette[visus->exemplar_fg]);
    text_chorda = _chorda_ex_cstr(buffer);
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        text_chorda, visus->modus_fg ? pixelum_accent : pixelum_text, scala);

    /* FG swatch */
    delineare_rectangulum_plenum(ctx,
        (x + PADDING + XXII) * char_lat, linea * char_alt,
        IV * char_lat, char_alt, fg_color);
    delineare_rectangulum(ctx,
        (x + PADDING + XXII) * char_lat, linea * char_alt,
        IV * char_lat, char_alt, color_border);

    /* BG display */
    sprintf(buffer, "BG: [%2d] %-14s", visus->exemplar_bg, nomina_palette[visus->exemplar_bg]);
    text_chorda = _chorda_ex_cstr(buffer);
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING + XXVIII) * char_lat, linea * char_alt,
        text_chorda, visus->modus_fg ? pixelum_text : pixelum_accent, scala);

    /* BG swatch */
    delineare_rectangulum_plenum(ctx,
        (x + PADDING + L) * char_lat, linea * char_alt,
        IV * char_lat, char_alt, bg_color);
    delineare_rectangulum(ctx,
        (x + PADDING + L) * char_lat, linea * char_alt,
        IV * char_lat, char_alt, color_border);

    /* Mode indicator */
    sprintf(buffer, "[editing: %s]", visus->modus_fg ? "FG" : "BG");
    text_chorda = _chorda_ex_cstr(buffer);
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING + LVI) * char_lat, linea * char_alt,
        text_chorda, pixelum_text_dim, scala);

    linea += II;

    /* Pattern grid */
    per (i = ZEPHYRUM; i < EXEMPLAR_NUMERUS; i++)
    {
        i32 cell_x;
        i32 cell_y;
        i32 swatch_x;
        i32 swatch_y;

        row = i / EXEMPLARIA_PER_ROW;
        col = i % EXEMPLARIA_PER_ROW;

        cell_x = (x + PADDING) + col * XIII;   /* 13 chars per cell */
        cell_y = linea + row * VI;             /* 6 lines per cell (1 label + 4 swatch + 1 spacing) */

        /* Pattern index only */
        sprintf(buffer, "%02d", i);
        text_chorda = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            cell_x * char_lat, cell_y * char_alt,
            text_chorda, pixelum_text_dim, scala);

        /* Pattern swatch */
        swatch_x = cell_x * char_lat;
        swatch_y = (cell_y + I) * char_alt;

        /* Draw background fill */
        delineare_rectangulum_plenum(ctx, swatch_x, swatch_y,
            EXEMPLAR_SWATCH_LAT, EXEMPLAR_SWATCH_ALT, bg_color);

        /* Set pattern mode and draw foreground pattern */
        delineare_ponere_exemplar_internum(ctx, i);
        delineare_ponere_modum(ctx, MODUS_EXEMPLAR);
        delineare_rectangulum_plenum(ctx, swatch_x, swatch_y,
            EXEMPLAR_SWATCH_LAT, EXEMPLAR_SWATCH_ALT, fg_color);

        /* Reset to solid mode */
        delineare_ponere_modum(ctx, MODUS_SOLIDUS);

        /* Border */
        delineare_rectangulum(ctx, swatch_x, swatch_y,
            EXEMPLAR_SWATCH_LAT, EXEMPLAR_SWATCH_ALT, color_border);
    }
}


/* ==================================================
 * Reddere - Page Indicator
 * ================================================== */

hic_manens vacuum
_thema_visus_reddere_pagina_indicator(
    ThemaVisus*      visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y_bottom,
    i32              scala,
    i32              char_lat,
    i32              char_alt,
    i32              pixelum_text_dim)
{
    character buffer[XXXII];
    chorda indicator;

    sprintf(buffer, "Page %d/%d  (h/l to navigate)",
            visus->pagina + I, THEMA_PAGINA_MAXIMA + I);
    indicator = _chorda_ex_cstr(buffer);

    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, y_bottom * char_alt,
        indicator, pixelum_text_dim, scala);
}


/* ==================================================
 * Reddere - Main Entry Point
 * ================================================== */

vacuum
thema_visus_reddere(
    ThemaVisus*      visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    ContextusDelineandi* ctx;
    i32 char_lat;
    i32 char_alt;
    i32 px_x;
    i32 px_y;
    Color color_background;
    Color color_text;
    Color color_text_dim;
    Color color_border;
    i32 pixelum_text;
    i32 pixelum_text_dim;

    (vacuum)focused;

    si (!visus || !tabula)
    {
        redde;
    }

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(visus->piscina, tabula);
    si (!ctx)
    {
        redde;
    }

    char_lat = CHAR_LATITUDO * scala;
    char_alt = CHAR_ALTITUDO * scala;

    /* Colores */
    color_background = thema_color(COLOR_BACKGROUND);
    color_text = thema_color(COLOR_TEXT);
    color_text_dim = thema_color(COLOR_TEXT_DIM);
    color_border = thema_color(COLOR_BORDER);

    pixelum_text = color_ad_pixelum(color_text);
    pixelum_text_dim = color_ad_pixelum(color_text_dim);

    /* Pingere fondum */
    px_x = x * char_lat;
    px_y = y * char_alt;
    delineare_rectangulum_plenum(ctx, px_x, px_y,
        latitudo * char_lat, altitudo * char_alt, color_background);

    /* Dispatch ad paginam currentem */
    commutatio (visus->pagina)
    {
        casus THEMA_PAGINA_COLORES:
            _thema_visus_reddere_colores(visus, ctx, tabula, x, y, scala,
                char_lat, char_alt, pixelum_text, pixelum_text_dim, color_border);
            frange;

        casus THEMA_PAGINA_EXEMPLARIA:
            _thema_visus_reddere_exemplaria(visus, ctx, tabula, x, y, scala,
                char_lat, char_alt, pixelum_text, pixelum_text_dim, color_border);
            frange;

        ordinarius:
            frange;
    }

    /* Page indicator at bottom */
    _thema_visus_reddere_pagina_indicator(visus, tabula, x, y + altitudo - II,
        scala, char_lat, char_alt, pixelum_text_dim);
}


/* ==================================================
 * Eventus
 * ================================================== */

b32
thema_visus_tractare_eventum(
    ThemaVisus*       visus,
    constans Eventus* eventus)
{
    si (!visus || !eventus)
    {
        redde FALSUM;
    }

    si (eventus->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }

    /* Page navigation */
    si (eventus->datum.clavis.typus == 'h')
    {
        si (visus->pagina > ZEPHYRUM)
        {
            visus->pagina--;
        }
        redde VERUM;
    }

    si (eventus->datum.clavis.typus == 'l')
    {
        si (visus->pagina < THEMA_PAGINA_MAXIMA)
        {
            visus->pagina++;
        }
        redde VERUM;
    }

    /* Mode selection (only on pattern page) */
    si (eventus->datum.clavis.typus == 'f')
    {
        si (visus->pagina == THEMA_PAGINA_EXEMPLARIA)
        {
            visus->modus_fg = VERUM;
        }
        redde VERUM;
    }

    si (eventus->datum.clavis.typus == 'b')
    {
        si (visus->pagina == THEMA_PAGINA_EXEMPLARIA)
        {
            visus->modus_fg = FALSUM;
        }
        redde VERUM;
    }

    /* Color cycling (only on pattern page) */
    si (eventus->datum.clavis.typus == 'j')
    {
        si (visus->pagina == THEMA_PAGINA_EXEMPLARIA)
        {
            si (visus->modus_fg)
            {
                visus->exemplar_fg = (visus->exemplar_fg + I) & XV;
            }
            alioquin
            {
                visus->exemplar_bg = (visus->exemplar_bg + I) & XV;
            }
        }
        redde VERUM;
    }

    si (eventus->datum.clavis.typus == 'k')
    {
        si (visus->pagina == THEMA_PAGINA_EXEMPLARIA)
        {
            si (visus->modus_fg)
            {
                visus->exemplar_fg = (visus->exemplar_fg - I + XVI) & XV;
            }
            alioquin
            {
                visus->exemplar_bg = (visus->exemplar_bg - I + XVI) & XV;
            }
        }
        redde VERUM;
    }

    redde FALSUM;
}


/* ==================================================
 * Lifecycle (Init / Status)
 * ================================================== */

/* Command handler pro $thema */
hic_manens b32
_thema_command_handler(
    ContextusCommandi* ctx)
{
    ContextusWidget* widget_ctx;

    widget_ctx = (ContextusWidget*)ctx->datum_registratus;

    /* ThemaVisus non habet argumenta */
    widget_ctx->commutare_widget(
        widget_ctx->schirmata_datum,
        "thema",
        NIHIL);

    redde VERUM;
}

vacuum
thema_visus_init(
    ContextusWidget* ctx)
{
    si (!ctx || !ctx->reg_commandi)
    {
        redde;
    }

    registrum_commandi_registrare(
        ctx->reg_commandi,
        "thema",
        _thema_command_handler,
        ctx);
}

vacuum
thema_visus_salvare_status(
    ThemaVisus*          visus,
    EntitasRepositorium* repo,
    constans character*  entitas_id)
{
    Entitas* entitas;
    character valor[XVI];

    si (!visus || !repo || !entitas_id)
    {
        redde;
    }

    entitas = repo->entitas_scaffoldare(repo->datum, "ThemaVisusStatus", entitas_id);
    si (!entitas)
    {
        redde;
    }

    sprintf(valor, "%d", visus->pagina);
    repo->proprietas_ponere(repo->datum, entitas, "pagina", valor);

    sprintf(valor, "%d", visus->exemplar_fg);
    repo->proprietas_ponere(repo->datum, entitas, "exemplar_fg", valor);

    sprintf(valor, "%d", visus->exemplar_bg);
    repo->proprietas_ponere(repo->datum, entitas, "exemplar_bg", valor);

    sprintf(valor, "%d", visus->modus_fg ? I : ZEPHYRUM);
    repo->proprietas_ponere(repo->datum, entitas, "modus_fg", valor);
}

vacuum
thema_visus_carcare_status(
    ThemaVisus*          visus,
    EntitasRepositorium* repo,
    constans character*  entitas_id)
{
    Entitas* entitas;
    chorda* clavis;
    chorda* valor;
    InternamentumChorda* intern;
    s32 temp;

    si (!visus || !repo || !entitas_id)
    {
        redde;
    }

    intern = internamentum_globale();
    entitas = repo->entitas_scaffoldare(repo->datum, "ThemaVisusStatus", entitas_id);
    si (!entitas)
    {
        redde;
    }

    /* Carcare pagina */
    clavis = chorda_internare_ex_literis(intern, "pagina");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor && valor->mensura > ZEPHYRUM)
    {
        si (chorda_ut_s32(*valor, &temp))
        {
            visus->pagina = (i32)temp;
            si (visus->pagina < ZEPHYRUM) visus->pagina = ZEPHYRUM;
            si (visus->pagina > THEMA_PAGINA_MAXIMA) visus->pagina = THEMA_PAGINA_MAXIMA;
        }
    }

    /* Carcare exemplar_fg */
    clavis = chorda_internare_ex_literis(intern, "exemplar_fg");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor && valor->mensura > ZEPHYRUM)
    {
        si (chorda_ut_s32(*valor, &temp))
        {
            visus->exemplar_fg = (i32)temp & XV;
        }
    }

    /* Carcare exemplar_bg */
    clavis = chorda_internare_ex_literis(intern, "exemplar_bg");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor && valor->mensura > ZEPHYRUM)
    {
        si (chorda_ut_s32(*valor, &temp))
        {
            visus->exemplar_bg = (i32)temp & XV;
        }
    }

    /* Carcare modus_fg */
    clavis = chorda_internare_ex_literis(intern, "modus_fg");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor && valor->mensura > ZEPHYRUM)
    {
        si (chorda_ut_s32(*valor, &temp))
        {
            visus->modus_fg = (temp != ZEPHYRUM) ? VERUM : FALSUM;
        }
    }
}
