#ifndef THEMA_H
#define THEMA_H

#include "latina.h"
#include "color.h"

/* ==================================================
 * THEMA - 16-Color Palette Theme System
 *
 * Fixed 16-color palette with semantic color mapping.
 * All UI colors are indices into this palette.
 *
 * PHILOSOPHIA:
 * - Constraint-based design (only 16 colors)
 * - Semantic names map to palette indices
 * - Can swap entire palette for different themes
 * - Consistent visual language
 *
 * EXEMPLUM:
 *   thema_initiare();
 *   i32 bg = thema_color(COLOR_BACKGROUND);  // Returns RGB from palette
 *   thema_ponere_palette(aquinas_palette);
 *
 * ================================================== */


/* ==================================================
 * Palette Indices (0-15)
 * ================================================== */

#define PALETTE_BLACK           ZEPHYRUM
#define PALETTE_DARK_GRAY       I
#define PALETTE_MEDIUM_DARK_GRAY II
#define PALETTE_MEDIUM_GRAY     III
#define PALETTE_LIGHT_GRAY      IV
#define PALETTE_WHITE           V

#define PALETTE_DARK_RED        VI
#define PALETTE_BLUE            VII
#define PALETTE_BRIGHT_PINK     VIII

#define PALETTE_DARK_GOLD       IX
#define PALETTE_MEDIUM_GOLD     X
#define PALETTE_BRIGHT_GOLD     XI

#define PALETTE_DARK_LEAF       XII
#define PALETTE_MEDIUM_LEAF     XIII
#define PALETTE_BRIGHT_LEAF     XIV

#define PALETTE_WARM_GRAY       XV


/* ==================================================
 * Semantic Color Names
 * ================================================== */

nomen enumeratio {
    COLOR_BACKGROUND = ZEPHYRUM,    /* Window/screen background */
    COLOR_TEXT,                      /* Primary text color */
    COLOR_TEXT_DIM,                  /* Dimmed/secondary text */
    COLOR_CURSOR,                    /* Text cursor */
    COLOR_SELECTION,                 /* Selected text background */

    COLOR_BORDER,                    /* Widget borders */
    COLOR_BORDER_ACTIVE,             /* Active/focused border */

    COLOR_STATUS_NORMAL,             /* Status: normal mode */
    COLOR_STATUS_INSERT,             /* Status: insert mode */
    COLOR_STATUS_VISUAL,             /* Status: visual mode */

    COLOR_ACCENT_PRIMARY,            /* Primary accent color */
    COLOR_ACCENT_SECONDARY,          /* Secondary accent */

    COLOR_ERROR,                     /* Error messages */
    COLOR_WARNING,                   /* Warnings */
    COLOR_SUCCESS,                   /* Success messages */

    COLOR_COUNT                      /* Total number of semantic colors */
} ColorThema;


/* ==================================================
 * Initiatio
 * ================================================== */

/* Initiare thema cum palette et mappis ordinariis
 *
 * Debet vocari ante usum systematis themae.
 */
vacuum
thema_initiare(vacuum);


/* ==================================================
 * Obtinere Colores
 * ================================================== */

/* Obtinere colorem RGB ex themate currente
 *
 * Lookup: semantic color -> palette index -> Color
 *
 * color: color semanticus ex enumeratione ColorThema
 *
 * Reddit: Color structura
 */
Color
thema_color(ColorThema color);

/* Obtinere indicem palette pro colore semantico
 *
 * color: color semanticus
 *
 * Reddit: index in palette (0-15)
 */
i32
thema_palette_index(ColorThema color);

/* Obtinere colorem ex indice colorationis (syntax highlighting)
 *
 * Maps ColoratioIndex values (from coloratio.h) to actual colors.
 *
 * index: ColoratioIndex value (0 = default, 1 = command, 2 = tag, etc.)
 *
 * Reddit: Color structura
 */
Color
thema_color_ex_indice_colorationis(i8 index);


/* ==================================================
 * Ponere Colores
 * ================================================== */

/* Ponere colorem semanticum ad indicem palette
 *
 * color: color semanticus
 * palette_index: index in palette (0-15)
 */
vacuum
thema_ponere_mappam(
    ColorThema color,
    i32 palette_index);

/* Ponere palette completam (16 colores)
 *
 * palette: array de 48 bytes (16 colores × 3 bytes RGB)
 *          Format: [R0, G0, B0, R1, G1, B1, ..., R15, G15, B15]
 */
vacuum
thema_ponere_palette(
    constans i8* palette);


/* ==================================================
 * Palette Praeselecta
 * ================================================== */

/* Obtinere palette Aquinas (ordinaria)
 *
 * Reddit: pointer ad palette Aquinas (48 bytes)
 */
constans i8*
thema_palette_aquinas(vacuum);

#endif /* THEMA_H */
