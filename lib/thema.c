#include "thema.h"

/* ==================================================
 * Palette Aquinas (Ordinaria)
 * ================================================== */

hic_manens constans i8 palette_aquinas[XLVIII] = {
    /* Grayscale foundation (6 shades) */
    /*0x0D, 0x11, 0x09, */ /* 0: Black */
    0x00, 0x00, 0x00,  /* 0: Black */
    0x0D, 0x11, 0x09,  /* 1: Dark gray */
    0x20, 0x20, 0x20,  /* 2: Medium dark gray */
    0x32, 0x2E, 0x28,  /* 3: Medium gray (warm) */
    0x3A, 0x36, 0x30,  /* 4: Light gray (warm) */
    0x3F, 0x3C, 0x3A,

    /* Reds (3 shades) */
    0x20, 0x08, 0x08,  /* 6: Dark red */
    0x30, 0x0C, 0x0C,  /* 7: Medium red */
    0x3F, 0x10, 0x10,  /* 8: Bright red */

    /* Yellow-golds (3 shades) */
    0x28, 0x20, 0x08,  /* 9:  Dark gold */
    0x38, 0x30, 0x10,  /* 10: Medium gold */
    0x3F, 0x38, 0x18,  /* 11: Bright yellow-gold */

    /* Cyans (3 shades) */
    0x08, 0x20, 0x28,  /* 12: Dark cyan */
    0x10, 0x30, 0x38,  /* 13: Medium cyan */
    0x18, 0x38, 0x3F,  /* 14: Bright cyan */

    /* Special purpose */
    0x2C, 0x28, 0x20   /* 15: Warm gray (for text backgrounds) */
};


/* ==================================================
 * State Globalis
 * ================================================== */

/* Palette currens (16 colores × 3 bytes RGB) */
hic_manens i8 palette_currens[XLVIII];

/* Mappa semantica: semantic color -> palette index */
hic_manens i32 mappa_semantica[COLOR_COUNT];


/* ==================================================
 * Initiatio
 * ================================================== */

vacuum
thema_initiare(vacuum)
{
    i32 i;

    /* Copiere palette Aquinas ad palette currens */
    per (i = ZEPHYRUM; i < XLVIII; i++) {
        palette_currens[i] = palette_aquinas[i];
    }

    /* Ponere mappas semanticas ordinarias */
    mappa_semantica[COLOR_BACKGROUND]       = PALETTE_WARM_GRAY;
    mappa_semantica[COLOR_TEXT]             = PALETTE_DARK_GRAY;
    mappa_semantica[COLOR_TEXT_DIM]         = PALETTE_MEDIUM_DARK_GRAY;
    mappa_semantica[COLOR_CURSOR]           = PALETTE_BRIGHT_GOLD;
    mappa_semantica[COLOR_SELECTION]        = PALETTE_MEDIUM_DARK_GRAY;

    mappa_semantica[COLOR_BORDER]           = PALETTE_MEDIUM_DARK_GRAY;
    mappa_semantica[COLOR_BORDER_ACTIVE]    = PALETTE_DARK_CYAN;

    mappa_semantica[COLOR_STATUS_NORMAL]    = PALETTE_MEDIUM_GRAY;
    mappa_semantica[COLOR_STATUS_INSERT]    = PALETTE_BRIGHT_GOLD;
    mappa_semantica[COLOR_STATUS_VISUAL]    = PALETTE_BRIGHT_CYAN;

    mappa_semantica[COLOR_ACCENT_PRIMARY]   = PALETTE_BRIGHT_CYAN;
    mappa_semantica[COLOR_ACCENT_SECONDARY] = PALETTE_BRIGHT_GOLD;

    mappa_semantica[COLOR_ERROR]            = PALETTE_BRIGHT_RED;
    mappa_semantica[COLOR_WARNING]          = PALETTE_MEDIUM_GOLD;
    mappa_semantica[COLOR_SUCCESS]          = PALETTE_BRIGHT_CYAN;
}


/* ==================================================
 * Obtinere Colores
 * ================================================== */

Color
thema_color(ColorThema color)
{
    i32 palette_index;

    /* Lookup semantic -> palette index */
    palette_index = mappa_semantica[color];

    /* Use color library to convert palette index to Color */
    redde color_ex_palette(palette_index);
}

i32
thema_palette_index(ColorThema color)
{
    redde mappa_semantica[color];
}

Color
thema_color_ex_indice_colorationis(i8 index)
{
    /* Mappa ColoratioIndex ad indices palette
     *
     * COLORATIO_DEFALTA     = 0  -> COLOR_TEXT
     * COLORATIO_COMMANDUM   = 1  -> PALETTE_BRIGHT_GOLD
     * COLORATIO_TAGUM       = 2  -> PALETTE_WHITE (grayscale)
     * COLORATIO_CLAVIS      = 3  -> PALETTE_MEDIUM_RED
     * COLORATIO_CHORDA      = 4  -> PALETTE_DARK_GOLD
     * COLORATIO_NUMERUS     = 5  -> PALETTE_MEDIUM_GOLD
     * COLORATIO_COMMENTUM   = 6  -> PALETTE_MEDIUM_GRAY
     * COLORATIO_OPERANS     = 7  -> PALETTE_LIGHT_GRAY
     * COLORATIO_PROPRIETAS  = 8  -> PALETTE_BRIGHT_RED
     * COLORATIO_ATTRIBUTUM  = 9  -> PALETTE_LIGHT_GRAY (grayscale)
     * COLORATIO_VALOR_ATTR  = 10 -> PALETTE_MEDIUM_DARK_GRAY (grayscale)
     * COLORATIO_LINK        = 11 -> PALETTE_DARK_CYAN
     */

    commutatio (index)
    {
        casus I:   /* COLORATIO_COMMANDUM */
            redde color_ex_palette(PALETTE_BRIGHT_GOLD);
        casus II:  /* COLORATIO_TAGUM */
            redde color_ex_palette(PALETTE_WHITE);
        casus III: /* COLORATIO_CLAVIS */
            redde color_ex_palette(PALETTE_MEDIUM_RED);
        casus IV:  /* COLORATIO_CHORDA */
            redde color_ex_palette(PALETTE_DARK_GOLD);
        casus V:   /* COLORATIO_NUMERUS */
            redde color_ex_palette(PALETTE_MEDIUM_GOLD);
        casus VI:  /* COLORATIO_COMMENTUM */
            redde color_ex_palette(PALETTE_MEDIUM_GRAY);
        casus VII: /* COLORATIO_OPERANS */
            redde color_ex_palette(PALETTE_LIGHT_GRAY);
        casus VIII: /* COLORATIO_PROPRIETAS */
            redde color_ex_palette(PALETTE_BRIGHT_RED);
        casus IX:  /* COLORATIO_ATTRIBUTUM */
            redde color_ex_palette(PALETTE_LIGHT_GRAY);
        casus X:   /* COLORATIO_VALOR_ATTR */
            redde color_ex_palette(PALETTE_MEDIUM_DARK_GRAY);
        casus XI:  /* COLORATIO_LINK */
            redde color_ex_palette(PALETTE_DARK_CYAN);
        ordinarius: /* COLORATIO_DEFALTA vel ignotus */
            redde thema_color(COLOR_TEXT);
    }
}


/* ==================================================
 * Ponere Colores
 * ================================================== */

vacuum
thema_ponere_mappam(
    ColorThema color,
    i32 palette_index)
{
    mappa_semantica[color] = palette_index;
}

vacuum
thema_ponere_palette(
    constans i8* palette)
{
    i32 i;

    per (i = ZEPHYRUM; i < XLVIII; i++) {
        palette_currens[i] = palette[i];
    }
}


/* ==================================================
 * Palette Praeselecta
 * ================================================== */

constans i8*
thema_palette_aquinas(vacuum)
{
    redde palette_aquinas;
}
