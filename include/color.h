#ifndef COLOR_H
#define COLOR_H

#include "latina.h"


/* ==================================================
 * Color Library
 *
 * Centralized color representation and conversion
 * for consistent handling across all libraries.
 *
 * Handles platform-specific pixel format (ABGR for macOS)
 * ================================================== */

/* Color representation (platform-agnostic) */
nomen structura {
    i8 r;
    i8 g;
    i8 b;
    i8 a;
} Color;


/* ==================================================
 * Color Creation
 * ================================================== */

/* Creare colorem ex componentibus RGB */
Color
color_ex_rgb (
    i8 r,
    i8 g,
    i8 b);

/* Creare colorem ex componentibus RGBA */
Color
color_ex_rgba (
    i8 r,
    i8 g,
    i8 b,
    i8 a);

/* Creare colorem ex palette index (uses thema_palette_aquinas) */
Color
color_ex_palette (
    i32 palette_index);

/* Creare colorem ex integro (platform-specific pixel format) */
Color
color_ex_pixelum (
    i32 pixel);


/* ==================================================
 * Color Conversion
 * ================================================== */

/* Convertere ad pixel format (ABGR for macOS) */
i32
color_ad_pixelum (
    Color color);

/* Extrahere componentes individuales */
i8
color_obtinere_r (
    Color color);
i8
color_obtinere_g (
    Color color);
i8
color_obtinere_b (
    Color color);
i8
color_obtinere_a (
    Color color);


/* ==================================================
 * Color Manipulation
 * ================================================== */

/* Interpolate inter duos colores (t = 0-256, ubi 0=a, 256=b) */
Color
color_interpolate (
    Color a,
    Color b,
      i32 t);

/* Convertere ad cinereum (grayscale) */
i8
color_ad_cinereum (
    Color color);

/* Comparare duo colores */
b32
color_aequalis (
    Color a,
    Color b);


/* ==================================================
 * Spatia Colorum (lux linearis, OKLab)
 *
 * Mixtio colorum (dithering, gradientes) in luce LINEARI fit: tabula
 * nigro-alba alternans oculo ~188 sRGB videtur, non 128. Comparatio in
 * OKLab (Ottosson 2020): distantia Euclidea ~ differentia percepta.
 * Canales lineares et sRGB in [0, 1]; L OKLab in [0, 1].
 * ================================================== */

/* canalis unus */
f64
color_srgb_ad_linearem (
    f64 c);

f64
color_linearis_ad_srgb (
    f64 l);

/* octetus 0..255 -> linearis (tabula interna, celer) */
f64
color_octetus_ad_linearem (
    i8 octetus);

/* lux linearis <-> OKLab (rgb negativa ad 0 tonduntur) */
vacuum
color_linearis_ad_oklab (
    constans f64 rgb[III],
             f64 lab[III]);

vacuum
color_oklab_ad_linearem (
    constans f64 lab[III],
             f64 rgb[III]);

/* commoditas: Color (octeti sRGB) -> OKLab */
vacuum
color_ad_oklab (
    Color color,
      f64 lab[III]);

/* distantia Euclidea in OKLab */
f64
color_oklab_distantia (
    constans f64 a[III],
    constans f64 b[III]);


/* ==================================================
 * Predefined Colors
 * ================================================== */

externus constans Color COLOR_NIGER;      /* Black */
externus constans Color COLOR_ALBUS;      /* White */
externus constans Color COLOR_RUBER;      /* Red */
externus constans Color COLOR_VIRIDIS;    /* Green */
externus constans Color COLOR_CAERULEUS;  /* Blue */
externus constans Color COLOR_FLAVUS;     /* Yellow */
externus constans Color COLOR_CYANEUS;    /* Cyan */
externus constans Color COLOR_PURPUREUS;  /* Magenta */

#endif /* COLOR_H */
