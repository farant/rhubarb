#include "color.h"
#include "thema.h"

/* ==================================================
 * Color Creation
 * ================================================== */

Color
color_ex_rgb(i8 r, i8 g, i8 b)
{
    Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = (i8)CCLV;  /* 255 - fully opaque */
    redde color;
}

Color
color_ex_rgba(i8 r, i8 g, i8 b, i8 a)
{
    Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    redde color;
}

Color
color_ex_palette(i32 palette_index)
{
    constans i8* pal;
    i32 offset;
    Color color;
    i32 r, g, b;

    pal = thema_palette_aquinas();
    offset = palette_index * III;

    /* Palette uses 6-bit values (0x00-0x3F), scale to 8-bit (0x00-0xFF) */
    r = ((i32)pal[offset + ZEPHYRUM] * CCLV) / LXIII;
    g = ((i32)pal[offset + I] * CCLV) / LXIII;
    b = ((i32)pal[offset + II] * CCLV) / LXIII;

    color.r = (i8)r;
    color.g = (i8)g;
    color.b = (i8)b;
    color.a = (i8)CCLV;

    redde color;
}

Color
color_ex_pixelum(i32 pixel)
{
    Color color;

    /* Unpack from ABGR format (macOS pixel buffer) */
    color.r = (i8)(pixel & CCLV);
    color.g = (i8)((pixel >> VIII) & CCLV);
    color.b = (i8)((pixel >> XVI) & CCLV);
    color.a = (i8)((pixel >> XXIV) & CCLV);

    redde color;
}


/* ==================================================
 * Color Conversion
 * ================================================== */

i32
color_ad_pixelum(Color color)
{
    /* Pack as ABGR for macOS pixel buffer */
    redde ((i32)color.a << XXIV) |
          ((i32)color.b << XVI) |
          ((i32)color.g << VIII) |
          ((i32)color.r);
}

i8
color_obtinere_r(Color color)
{
    redde color.r;
}

i8
color_obtinere_g(Color color)
{
    redde color.g;
}

i8
color_obtinere_b(Color color)
{
    redde color.b;
}

i8
color_obtinere_a(Color color)
{
    redde color.a;
}


/* ==================================================
 * Color Manipulation
 * ================================================== */

Color
color_interpolate(Color a, Color b, i32 t)
{
    Color result;

    /* Interpolate each component (t = 0-256) */
    result.r = (i8)(a.r + ((b.r - a.r) * t) / CCLVI);
    result.g = (i8)(a.g + ((b.g - a.g) * t) / CCLVI);
    result.b = (i8)(a.b + ((b.b - a.b) * t) / CCLVI);
    result.a = (i8)(a.a + ((b.a - a.a) * t) / CCLVI);

    redde result;
}

i8
color_ad_cinereum(Color color)
{
    /* ITU-R BT.601 luminance weights:
     * Y = 0.299*R + 0.587*G + 0.114*B
     * Approximated as: (30*R + 59*G + 11*B) / 100 */
    i32 gray = ((i32)color.r * XXX + (i32)color.g * LIX + (i32)color.b * XI) / C;
    redde (i8)gray;
}

b32
color_aequalis(Color a, Color b)
{
    redde (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.a == b.a);
}


/* ==================================================
 * Predefined Colors
 * ================================================== */

constans Color COLOR_NIGER      = {0x00, 0x00, 0x00, (i8)0xFF};  /* Black */
constans Color COLOR_ALBUS      = {(i8)0xFF, (i8)0xFF, (i8)0xFF, (i8)0xFF};  /* White */
constans Color COLOR_RUBER      = {(i8)0xFF, 0x00, 0x00, (i8)0xFF};  /* Red */
constans Color COLOR_VIRIDIS    = {0x00, (i8)0xFF, 0x00, (i8)0xFF};  /* Green */
constans Color COLOR_CAERULEUS  = {0x00, 0x00, (i8)0xFF, (i8)0xFF};  /* Blue */
constans Color COLOR_FLAVUS     = {(i8)0xFF, (i8)0xFF, 0x00, (i8)0xFF};  /* Yellow */
constans Color COLOR_CYANEUS    = {0x00, (i8)0xFF, (i8)0xFF, (i8)0xFF};  /* Cyan */
constans Color COLOR_PURPUREUS  = {(i8)0xFF, 0x00, (i8)0xFF, (i8)0xFF};  /* Magenta */
