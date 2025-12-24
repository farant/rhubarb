#include "fons_visus.h"
#include "fons_6x8.h"
#include "thema.h"
#include "color.h"
#include "delineare.h"
#include "chorda.h"
#include "registrum_commandi.h"
#include <string.h>
#include <stdio.h>

/* ==================================================
 * Constantae
 * ================================================== */

#define CHAR_LATITUDO   VI      /* 6 pixels per character */
#define CHAR_ALTITUDO   VIII    /* 8 pixels per character */
#define PADDING         II      /* 2 characters padding */
#define CELL_LATITUDO   IV      /* 4 chars wide per cell (glyph + hex) */
#define CELL_ALTITUDO   II      /* 2 rows per cell */


/* ==================================================
 * Functiones Auxiliares
 * ================================================== */

hic_manens chorda
_chorda_ex_cstr(
    constans character* cstr)
{
    chorda fructus;
    union { constans character* c; i8* m; } u;
    u.c = cstr;

    fructus.datum = u.m;
    fructus.mensura = (i32)strlen(cstr);

    redde fructus;
}


/* ==================================================
 * Creatio
 * ================================================== */

FonsVisus*
fons_visus_creare(
    Piscina* piscina)
{
    FonsVisus* visus;

    si (!piscina)
    {
        redde NIHIL;
    }

    visus = (FonsVisus*)piscina_allocare(piscina, magnitudo(FonsVisus));
    si (!visus)
    {
        redde NIHIL;
    }

    visus->piscina = piscina;
    visus->scroll_y = 0;

    redde visus;
}


/* ==================================================
 * Reddere
 * ================================================== */

vacuum
fons_visus_reddere(
    FonsVisus*       visus,
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
    i32 linea;
    i32 row, col;
    Color color_background;
    Color color_text;
    Color color_text_dim;
    Color color_accent;
    i32 pixelum_text;
    i32 pixelum_text_dim;
    i32 pixelum_accent;
    chorda hex_chorda;
    chorda titulo_chorda;

    (vacuum)visus;
    (vacuum)latitudo;
    (vacuum)altitudo;
    (vacuum)focused;

    si (!tabula)
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
    color_accent = thema_color(COLOR_ACCENT_PRIMARY);

    pixelum_text = color_ad_pixelum(color_text);
    pixelum_text_dim = color_ad_pixelum(color_text_dim);
    pixelum_accent = color_ad_pixelum(color_accent);

    /* Pingere fondum */
    px_x = x * char_lat;
    px_y = y * char_alt;
    delineare_rectangulum_plenum(ctx, px_x, px_y,
        latitudo * char_lat, altitudo * char_alt, color_background);

    linea = y + I;

    /* === Titulus === */
    titulo_chorda = _chorda_ex_cstr("=== Font Glyphs (fons_6x8) ===");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulo_chorda, pixelum_text, scala);
    linea += II;

    /* === Header row (0-F) === */
    {
        i32 header_x = x + PADDING + IV;  /* Offset pro row labels */
        character col_label[II];

        /* Spatium pro row labels */
        titulo_chorda = _chorda_ex_cstr("    ");
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulo_chorda, pixelum_text_dim, scala);

        per (col = ZEPHYRUM; col < XVI; col++)
        {
            sprintf(col_label, "%X", col);
            hex_chorda = _chorda_ex_cstr(col_label);
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (header_x + col * CELL_LATITUDO + I) * char_lat,
                linea * char_alt,
                hex_chorda, pixelum_accent, scala);
        }
        linea += II;
    }

    /* === Grid 16x16 === */
    per (row = ZEPHYRUM; row < XVI; row++)
    {
        i32 grid_x;
        i32 grid_y;
        character row_label[IV];

        grid_y = linea * char_alt;

        /* Row label (0x00, 0x10, ..., 0xF0) */
        sprintf(row_label, "%X_", row);
        hex_chorda = _chorda_ex_cstr(row_label);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, grid_y,
            hex_chorda, pixelum_accent, scala);

        grid_x = x + PADDING + IV;

        per (col = ZEPHYRUM; col < XVI; col++)
        {
            i32 glyph_index;
            i32 cell_x;
            i32 cell_y;
            character glyph_char;

            glyph_index = row * XVI + col;
            cell_x = (grid_x + col * CELL_LATITUDO) * char_lat;
            cell_y = grid_y;

            /* Pingere rectangulum plenum cum medium gray */
            delineare_rectangulum_plenum(ctx,
                cell_x, cell_y,
                CELL_LATITUDO * char_lat - I,
                char_alt,
                color_ex_palette(PALETTE_MEDIUM_GRAY));

            /* Pingere border */
            delineare_rectangulum(ctx,
                cell_x, cell_y,
                CELL_LATITUDO * char_lat - I,
                char_alt,
                color_ex_palette(PALETTE_MEDIUM_GRAY));

            /* Pingere glypham directe (sine UTF-8 decodere) */
            glyph_char = (character)glyph_index;

            /* Centrare glypham in cella */
            tabula_pixelorum_pingere_characterem_scalatum(tabula,
                cell_x + char_lat,  /* Offset I character */
                cell_y,
                glyph_char, pixelum_text, scala);

            /* Hex code sub glypha (smaller, dimmer) - skip pro simplicitate */
        }

        linea += II;
    }

    linea += I;

    /* === Legenda === */
    titulo_chorda = _chorda_ex_cstr("Row labels: high nibble (X_)");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulo_chorda, pixelum_text_dim, scala);
    linea++;

    titulo_chorda = _chorda_ex_cstr("Col labels: low nibble (_X)");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulo_chorda, pixelum_text_dim, scala);
    linea++;

    titulo_chorda = _chorda_ex_cstr("Example: row 4, col 1 = 0x41 = 'A'");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulo_chorda, pixelum_text_dim, scala);
    linea += II;

    /* === Ranges === */
    titulo_chorda = _chorda_ex_cstr("=== Ranges ===");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulo_chorda, pixelum_text, scala);
    linea += II;

    titulo_chorda = _chorda_ex_cstr("00-1F: Control symbols (Alcuin)");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulo_chorda, pixelum_text_dim, scala);
    linea++;

    titulo_chorda = _chorda_ex_cstr("20-7F: ASCII (HP 100LX)");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulo_chorda, pixelum_text_dim, scala);
    linea++;

    titulo_chorda = _chorda_ex_cstr("80-9F: Extended (Alcuin)");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulo_chorda, pixelum_text_dim, scala);
    linea++;

    titulo_chorda = _chorda_ex_cstr("A0-FF: Latin-1 accents (HP 100LX)");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulo_chorda, pixelum_text_dim, scala);
}


/* ==================================================
 * Eventus
 * ================================================== */

b32
fons_visus_tractare_eventum(
    FonsVisus*        visus,
    constans Eventus* eventus)
{
    (vacuum)visus;
    (vacuum)eventus;

    /* Widget est read-only - non tractat eventus */
    redde FALSUM;
}


/* ==================================================
 * Lifecycle (Init / Status)
 * ================================================== */

/* Command handler pro $fons */
hic_manens b32
_fons_command_handler(
    ContextusCommandi* ctx)
{
    ContextusWidget* widget_ctx;

    widget_ctx = (ContextusWidget*)ctx->datum_registratus;

    widget_ctx->commutare_widget(
        widget_ctx->schirmata_datum,
        "fons",
        NIHIL);

    redde VERUM;
}

vacuum
fons_visus_init(
    ContextusWidget* ctx)
{
    si (!ctx || !ctx->reg_commandi)
    {
        redde;
    }

    registrum_commandi_registrare(
        ctx->reg_commandi,
        "fons",
        _fons_command_handler,
        ctx);
}

vacuum
fons_visus_salvare_status(
    FonsVisus*           visus,
    EntitasRepositorium* repo,
    constans character*  entitas_id)
{
    /* FonsVisus est read-only - nihil status ad salvare */
    (vacuum)visus;
    (vacuum)repo;
    (vacuum)entitas_id;
}

vacuum
fons_visus_carcare_status(
    FonsVisus*           visus,
    EntitasRepositorium* repo,
    constans character*  entitas_id)
{
    /* FonsVisus est read-only - nihil status ad carcare */
    (vacuum)visus;
    (vacuum)repo;
    (vacuum)entitas_id;
}
