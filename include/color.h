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
Color color_ex_rgb(i8 r, i8 g, i8 b);

/* Creare colorem ex componentibus RGBA */
Color color_ex_rgba(i8 r, i8 g, i8 b, i8 a);

/* Creare colorem ex palette index (uses thema_palette_aquinas) */
Color color_ex_palette(i32 palette_index);

/* Creare colorem ex integro (platform-specific pixel format) */
Color color_ex_pixelum(i32 pixel);


/* ==================================================
 * Color Conversion
 * ================================================== */

/* Convertere ad pixel format (ABGR for macOS) */
i32 color_ad_pixelum(Color color);

/* Extrahere componentes individuales */
i8 color_obtinere_r(Color color);
i8 color_obtinere_g(Color color);
i8 color_obtinere_b(Color color);
i8 color_obtinere_a(Color color);


/* ==================================================
 * Color Manipulation
 * ================================================== */

/* Interpolate inter duos colores (t = 0-256, ubi 0=a, 256=b) */
Color color_interpolate(Color a, Color b, i32 t);

/* Convertere ad cinereum (grayscale) */
i8 color_ad_cinereum(Color color);

/* Comparare duo colores */
b32 color_aequalis(Color a, Color b);


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
