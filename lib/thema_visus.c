#include "thema_visus.h"
#include "thema.h"
#include "color.h"
#include "delineare.h"
#include "chorda.h"
#include <string.h>
#include <stdio.h>

/* ==================================================
 * Constantae
 * ================================================== */

#define CHAR_LATITUDO   VI      /* 6 pixels per character */
#define CHAR_ALTITUDO   VIII    /* 8 pixels per character */
#define SWATCH_LATITUDO VIII    /* 8 characters wide */
#define PADDING         II      /* 2 characters padding */


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

    redde visus;
}


/* ==================================================
 * Reddere
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
    i32 linea;
    i32 i;
    Color color_background;
    Color color_text;
    Color color_text_dim;
    Color color_border;
    i32 pixelum_text;
    i32 pixelum_text_dim;
    character numero_buffer[IV];  /* "XX\0" */
    chorda numero_chorda;
    chorda nomen_chorda;

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
    color_border = thema_color(COLOR_BORDER);

    pixelum_text = color_ad_pixelum(color_text);
    pixelum_text_dim = color_ad_pixelum(color_text_dim);

    /* Pingere fondum */
    px_x = x * char_lat;
    px_y = y * char_alt;
    delineare_rectangulum_plenum(ctx, px_x, px_y,
        latitudo * char_lat, altitudo * char_alt, color_background);

    /* === Sectio: Contrast Grid === */
    linea = y + I;

    /* Titulus */
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
        cell_lat = II * char_lat;   /* 2 chars wide per cell */
        cell_alt = char_alt;        /* 1 char tall */
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

                /* Pingere fondum */
                delineare_rectangulum_plenum(ctx, cell_x, grid_y,
                    cell_lat, cell_alt, bg_color);

                /* Pingere "Aa" */
                tabula_pixelorum_pingere_chordam_scalatam(tabula,
                    cell_x, grid_y,
                    aa_chorda, color_ad_pixelum(fg_color), scala);
            }

            linea++;
        }
    }

    linea += II;

    /* === Sectio: Palette === */
    /* Palette ad dextram grid */
    {
        i32 palette_x;
        i32 palette_linea;

        palette_x = x + PADDING + XXXIV;  /* To the right of the grid */
        palette_linea = y + I;

        /* Titulus */
        nomen_chorda = _chorda_ex_cstr("=== Palette ===");
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            palette_x * char_lat, palette_linea * char_alt,
            nomen_chorda, pixelum_text, scala);
        palette_linea += II;

        /* Omnes XVI colores */
        per (i = ZEPHYRUM; i < XVI; i++)
        {
            Color palette_color;
            i32 swatch_x;
            i32 swatch_y;
            i32 swatch_lat;
            i32 swatch_alt;

            /* Numero (0-15) */
            sprintf(numero_buffer, "%2d", i);
            numero_chorda = _chorda_ex_cstr(numero_buffer);
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                palette_x * char_lat, palette_linea * char_alt,
                numero_chorda, pixelum_text_dim, scala);

            /* Nomen */
            nomen_chorda = _chorda_ex_cstr(nomina_palette[i]);
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (palette_x + IV) * char_lat, palette_linea * char_alt,
                nomen_chorda, pixelum_text, scala);

            /* Swatch - rectangulum plenum cum colore */
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

    /* Omnes mappae semanticae */
    per (i = ZEPHYRUM; i < COLOR_COUNT; i++)
    {
        Color semantic_color;
        i32 palette_index;
        i32 swatch_x;
        i32 swatch_y;
        i32 swatch_lat;
        i32 swatch_alt;

        /* Nomen semanticum */
        nomen_chorda = _chorda_ex_cstr(nomina_semantica[i]);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            nomen_chorda, pixelum_text, scala);

        /* Arrow et index */
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

        /* Nomen palette */
        nomen_chorda = _chorda_ex_cstr(nomina_palette[palette_index]);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + XXII) * char_lat, linea * char_alt,
            nomen_chorda, pixelum_text, scala);

        /* Swatch */
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
 * Eventus
 * ================================================== */

b32
thema_visus_tractare_eventum(
    ThemaVisus*       visus,
    constans Eventus* eventus)
{
    (vacuum)visus;
    (vacuum)eventus;

    /* Widget est read-only - non tractat eventus */
    redde FALSUM;
}
