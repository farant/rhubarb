#include <math.h>
#include "color.h"
#include "thema.h"


/* ==================================================
 * Color Creation
 * ================================================== */

Color
color_ex_rgb (
    i8 r,
    i8 g,
    i8 b)
{
    Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = (i8)CCLV;  /* 255 - fully opaque */
    redde color;
}

Color
color_ex_rgba (
    i8 r,
    i8 g,
    i8 b,
    i8 a)
{
    Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    redde color;
}

Color
color_ex_palette (
    i32 palette_index)
{
    constans i8* pal;
            i32  offset;
          Color  color;
    i32 r, g, b;

    pal     = thema_palette_aquinas();
    offset  = palette_index * III;

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
color_ex_pixelum (
    i32 pixel)
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
color_ad_pixelum (
    Color color)
{
    /* Pack as ABGR for macOS pixel buffer */
    redde ((i32)color.a << XXIV)
        | ((i32)color.b << XVI)
        | ((i32)color.g << VIII)
        | ((i32)color.r);
}

i8
color_obtinere_r (
    Color color)
{
    redde color.r;
}

i8
color_obtinere_g (
    Color color)
{
    redde color.g;
}

i8
color_obtinere_b (
    Color color)
{
    redde color.b;
}

i8
color_obtinere_a (
    Color color)
{
    redde color.a;
}


/* ==================================================
 * Color Manipulation
 * ================================================== */

Color
color_interpolate (
    Color a,
    Color b,
      i32 t)
{
    Color result;
    /* Arithmetica SIGNATA ex decreto (inventum gradus severi
     * 2026-07-17): t insignatum differentias negativas (b < a,
     * obscuratio) per UAC volvebat - divisio insignata versus
     * infinitum rotundat, signata versus zephyrum: fructus uno
     * differebat in obscurationibus non-exactis. */
    s32 ts = (s32)t;

    /* Interpolate each component (t = 0-256) */
    result.r = (i8)((s32)a.r + (((s32)b.r - (s32)a.r) * ts) / CCLVI);
    result.g = (i8)((s32)a.g + (((s32)b.g - (s32)a.g) * ts) / CCLVI);
    result.b = (i8)((s32)a.b + (((s32)b.b - (s32)a.b) * ts) / CCLVI);
    result.a = (i8)((s32)a.a + (((s32)b.a - (s32)a.a) * ts) / CCLVI);

    redde result;
}

i8
color_ad_cinereum (
    Color color)
{
    /* ITU-R BT.601 luminance weights:
     * Y = 0.299*R + 0.587*G + 0.114*B
     * Approximated as: (30*R + 59*G + 11*B) / 100 */
    i32 gray = ((i32)color.r * XXX + (i32)color.g * LIX
        + (i32)color.b * XI) / C;
    redde (i8)gray;
}

b32
color_aequalis (
    Color a,
    Color b)
{
    redde (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.a == b.a);
}


/* ==================================================
 * Spatia Colorum (lux linearis, OKLab)
 * ================================================== */

f64
color_srgb_ad_linearem (
    f64 c)
{
    si (c <= 0.04045)
    {
        redde c / 12.92;
    }
    redde pow((c + 0.055) / 1.055, 2.4);
}

f64
color_linearis_ad_srgb (
    f64 l)
{
    si (l <= 0.0031308)
    {
        redde l * 12.92;
    }
    redde 1.055 * pow(l, 1.0 / 2.4) - 0.055;
}

f64
color_octetus_ad_linearem (
    i8 octetus)
{
    hic_manens f64 tabula[CCLVI];
    hic_manens b32 parata = FALSUM;

    si (!parata)
    {
        s32 i;
        per (i = 0; i < CCLVI; i++)
        {
            tabula[i] = color_srgb_ad_linearem((f64)i / 255.0);
        }
        parata = VERUM;
    }
    redde tabula[(s32)octetus];
}

/* radix cubica pro x >= 0 (C89 cbrt non habet) */
interior f64
_radix_cubica (
    f64 x)
{
    redde x <= 0.0 ? 0.0 : pow(x, 1.0 / 3.0);
}

vacuum
color_linearis_ad_oklab (
    constans f64 rgb[III],
             f64 lab[III])
{
    f64 r = rgb[0] < 0.0 ? 0.0 : rgb[0];
    f64 g = rgb[1] < 0.0 ? 0.0 : rgb[1];
    f64 b = rgb[2] < 0.0 ? 0.0 : rgb[2];
    f64 l = _radix_cubica(0.4122214708 * r + 0.5363325363 * g
        + 0.0514459929 * b);
    f64 m = _radix_cubica(0.2119034982 * r + 0.6806995451 * g
        + 0.1073969566 * b);
    f64 s = _radix_cubica(0.0883024619 * r + 0.2817188376 * g
        + 0.6299787005 * b);

    lab[0] = 0.2104542553 * l + 0.7936177850 * m - 0.0040720468 * s;
    lab[1] = 1.9779984951 * l - 2.4285922050 * m + 0.4505937099 * s;
    lab[2] = 0.0259040371 * l + 0.7827717662 * m - 0.8086757660 * s;
}

vacuum
color_oklab_ad_linearem (
    constans f64 lab[III],
             f64 rgb[III])
{
    f64 l = lab[0] + 0.3963377774 * lab[1] + 0.2158037573 * lab[2];
    f64 m = lab[0] - 0.1055613458 * lab[1] - 0.0638541728 * lab[2];
    f64 s = lab[0] - 0.0894841775 * lab[1] - 1.2914855480 * lab[2];

    l       = l * l * l;
    m       = m * m * m;
    s       = s * s * s;
    rgb[0]  =  4.0767416621 * l - 3.3077115913 * m + 0.2309699292 * s;
    rgb[1]  = -1.2684380046 * l + 2.6097574011 * m - 0.3413193965 * s;
    rgb[2]  = -0.0041960863 * l - 0.7034186147 * m + 1.7076147010 * s;
}

vacuum
color_ad_oklab (
    Color color,
      f64 lab[III])
{
    f64 lin[III];

    lin[0] = color_octetus_ad_linearem(color.r);
    lin[1] = color_octetus_ad_linearem(color.g);
    lin[2] = color_octetus_ad_linearem(color.b);
    color_linearis_ad_oklab(lin, lab);
}

f64
color_oklab_distantia (
    constans f64 a[III],
    constans f64 b[III])
{
    f64 dl = a[0] - b[0];
    f64 da = a[1] - b[1];
    f64 db = a[2] - b[2];

    redde sqrt(dl * dl + da * da + db * db);
}


/* ==================================================
 * Predefined Colors
 * ================================================== */

constans Color COLOR_NIGER      = {0x00, 0x00, 0x00, (i8)0xFF};  /* Black */
constans Color COLOR_ALBUS      = {(i8)0xFF, (i8)0xFF, (i8)0xFF,
    (i8)0xFF};  /* White */
constans Color COLOR_RUBER      = {(i8)0xFF, 0x00, 0x00, (i8)0xFF};  /* Red */
constans Color COLOR_VIRIDIS    = {0x00, (i8)0xFF, 0x00, (i8)0xFF};  /* Green */
constans Color COLOR_CAERULEUS  = {0x00, 0x00, (i8)0xFF, (i8)0xFF};  /* Blue */
constans Color COLOR_FLAVUS     = {(i8)0xFF, (i8)0xFF, 0x00, (i8)0xFF};  /* Yellow */
constans Color COLOR_CYANEUS    = {0x00, (i8)0xFF, (i8)0xFF, (i8)0xFF};  /* Cyan */
constans Color COLOR_PURPUREUS  = {(i8)0xFF, 0x00, (i8)0xFF, (i8)0xFF};  /* Magenta */
