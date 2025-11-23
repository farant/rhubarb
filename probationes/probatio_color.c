/* probatio_color.c - Probationes Bibliothecae Coloris */
#include "latina.h"
#include "color.h"
#include "piscina.h"
#include "credo.h"
#include "thema.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina* piscina;
    b32      praeteritus;

    /* Aperire piscinam et credonem */
    piscina = piscina_generare_dynamicum("probatio_color", CDLVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* Initiare thema pro palette tests */
    thema_initiare();

    /* ==================================================
     * Probare color_ex_rgb
     * ================================================== */

    {
        Color color;

        imprimere("\n--- Probans color_ex_rgb ---\n");

        color = color_ex_rgb(CCLV, ZEPHYRUM, ZEPHYRUM);  /* Red */
        CREDO_AEQUALIS_I32(color.r, CCLV);
        CREDO_AEQUALIS_I32(color.g, ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.b, ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.a, CCLV);  /* Alpha should default to 255 */

        color = color_ex_rgb(ZEPHYRUM, CCLV, ZEPHYRUM);  /* Green */
        CREDO_AEQUALIS_I32(color.r, ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.g, CCLV);
        CREDO_AEQUALIS_I32(color.b, ZEPHYRUM);

        color = color_ex_rgb(ZEPHYRUM, ZEPHYRUM, CCLV);  /* Blue */
        CREDO_AEQUALIS_I32(color.r, ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.g, ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.b, CCLV);
    }

    /* ==================================================
     * Probare color_ex_rgba
     * ================================================== */

    {
        Color color;

        imprimere("\n--- Probans color_ex_rgba ---\n");

        color = color_ex_rgba(CCLV, CXXVIII, LXIV, XXXII);
        CREDO_AEQUALIS_I32(color.r, CCLV);
        CREDO_AEQUALIS_I32(color.g, CXXVIII);
        CREDO_AEQUALIS_I32(color.b, LXIV);
        CREDO_AEQUALIS_I32(color.a, XXXII);
    }

    /* ==================================================
     * Probare color_ex_palette
     * ================================================== */

    {
        Color color;

        imprimere("\n--- Probans color_ex_palette ---\n");

        /* Test black (palette index 0) */
        color = color_ex_palette(ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.r, ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.g, ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.b, ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.a, CCLV);

        /* Test white (palette index 5) */
        color = color_ex_palette(V);
        CREDO_AEQUALIS_I32(color.r, CCLV);
        CREDO_AEQUALIS_I32(color.g, CCLV);
        CREDO_AEQUALIS_I32(color.b, CCLV);
    }

    /* ==================================================
     * Probare color_ad_pixelum et color_ex_pixelum (round trip)
     * ================================================== */

    {
        Color color1, color2;
        i32 pixel;

        imprimere("\n--- Probans color_ad_pixelum / color_ex_pixelum ---\n");

        /* Create a color, convert to pixel, convert back */
        color1 = color_ex_rgba(CCLV, CXXVIII, LXIV, CCLV);
        pixel = color_ad_pixelum(color1);
        color2 = color_ex_pixelum(pixel);

        /* Should match after round trip */
        CREDO_AEQUALIS_I32(color2.r, CCLV);
        CREDO_AEQUALIS_I32(color2.g, CXXVIII);
        CREDO_AEQUALIS_I32(color2.b, LXIV);
        CREDO_AEQUALIS_I32(color2.a, CCLV);

        /* Test with black */
        color1 = color_ex_rgb(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
        pixel = color_ad_pixelum(color1);
        color2 = color_ex_pixelum(pixel);
        CREDO_VERUM(color_aequalis(color1, color2));

        /* Test with white */
        color1 = color_ex_rgb(CCLV, CCLV, CCLV);
        pixel = color_ad_pixelum(color1);
        color2 = color_ex_pixelum(pixel);
        CREDO_VERUM(color_aequalis(color1, color2));
    }

    /* ==================================================
     * Probare color_interpolate
     * ================================================== */

    {
        Color black, white, gray;

        imprimere("\n--- Probans color_interpolate ---\n");

        black = color_ex_rgb(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
        white = color_ex_rgb(CCLV, CCLV, CCLV);

        /* 0% = black */
        gray = color_interpolate(black, white, ZEPHYRUM);
        CREDO_AEQUALIS_I32(gray.r, ZEPHYRUM);
        CREDO_AEQUALIS_I32(gray.g, ZEPHYRUM);
        CREDO_AEQUALIS_I32(gray.b, ZEPHYRUM);

        /* 100% = white */
        gray = color_interpolate(black, white, CCLVI);
        CREDO_AEQUALIS_I32(gray.r, CCLV);
        CREDO_AEQUALIS_I32(gray.g, CCLV);
        CREDO_AEQUALIS_I32(gray.b, CCLV);

        /* 50% = medium gray */
        gray = color_interpolate(black, white, CXXVIII);
        CREDO_AEQUALIS_I32(gray.r, CXXVII);  /* 127 (close to 128) */
        CREDO_AEQUALIS_I32(gray.g, CXXVII);
        CREDO_AEQUALIS_I32(gray.b, CXXVII);
    }

    /* ==================================================
     * Probare color_ad_cinereum
     * ================================================== */

    {
        Color color;
        i8 gray;

        imprimere("\n--- Probans color_ad_cinereum ---\n");

        /* Black -> 0 */
        color = color_ex_rgb(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
        gray = color_ad_cinereum(color);
        CREDO_AEQUALIS_I32(gray, ZEPHYRUM);

        /* White -> 255 */
        color = color_ex_rgb(CCLV, CCLV, CCLV);
        gray = color_ad_cinereum(color);
        CREDO_AEQUALIS_I32(gray, CCLV);

        /* Red (uses ITU-R BT.601 weights: 30% red) */
        color = color_ex_rgb(CCLV, ZEPHYRUM, ZEPHYRUM);
        gray = color_ad_cinereum(color);
        CREDO_AEQUALIS_I32(gray, LXXVI);  /* 255 * 0.30 ≈ 76 */

        /* Green (59% green) */
        color = color_ex_rgb(ZEPHYRUM, CCLV, ZEPHYRUM);
        gray = color_ad_cinereum(color);
        CREDO_AEQUALIS_I32(gray, CL);  /* 255 * 0.59 ≈ 150 */

        /* Blue (11% blue) */
        color = color_ex_rgb(ZEPHYRUM, ZEPHYRUM, CCLV);
        gray = color_ad_cinereum(color);
        CREDO_AEQUALIS_I32(gray, XXVIII);  /* 255 * 0.11 ≈ 28 */
    }

    /* ==================================================
     * Probare color_aequalis
     * ================================================== */

    {
        Color color1, color2;

        imprimere("\n--- Probans color_aequalis ---\n");

        color1 = color_ex_rgb(CCLV, CXXVIII, LXIV);
        color2 = color_ex_rgb(CCLV, CXXVIII, LXIV);
        CREDO_VERUM(color_aequalis(color1, color2));

        color2 = color_ex_rgb(CCLV, CXXVIII, LXV);  /* Different blue */
        CREDO_FALSUM(color_aequalis(color1, color2));

        /* Test alpha channel */
        color1 = color_ex_rgba(CCLV, ZEPHYRUM, ZEPHYRUM, CCLV);
        color2 = color_ex_rgba(CCLV, ZEPHYRUM, ZEPHYRUM, CXXVIII);
        CREDO_FALSUM(color_aequalis(color1, color2));
    }

    /* ==================================================
     * Probare colores praedefinitos
     * ================================================== */

    {
        imprimere("\n--- Probans colores praedefinitos ---\n");

        /* Test predefined colors */
        CREDO_AEQUALIS_I32(COLOR_NIGER.r, ZEPHYRUM);
        CREDO_AEQUALIS_I32(COLOR_NIGER.g, ZEPHYRUM);
        CREDO_AEQUALIS_I32(COLOR_NIGER.b, ZEPHYRUM);

        CREDO_AEQUALIS_I32(COLOR_ALBUS.r, CCLV);
        CREDO_AEQUALIS_I32(COLOR_ALBUS.g, CCLV);
        CREDO_AEQUALIS_I32(COLOR_ALBUS.b, CCLV);

        CREDO_AEQUALIS_I32(COLOR_RUBER.r, CCLV);
        CREDO_AEQUALIS_I32(COLOR_RUBER.g, ZEPHYRUM);
        CREDO_AEQUALIS_I32(COLOR_RUBER.b, ZEPHYRUM);

        CREDO_AEQUALIS_I32(COLOR_VIRIDIS.r, ZEPHYRUM);
        CREDO_AEQUALIS_I32(COLOR_VIRIDIS.g, CCLV);
        CREDO_AEQUALIS_I32(COLOR_VIRIDIS.b, ZEPHYRUM);

        CREDO_AEQUALIS_I32(COLOR_CAERULEUS.r, ZEPHYRUM);
        CREDO_AEQUALIS_I32(COLOR_CAERULEUS.g, ZEPHYRUM);
        CREDO_AEQUALIS_I32(COLOR_CAERULEUS.b, CCLV);
    }

    /* ==================================================
     * Probare accessor functions
     * ================================================== */

    {
        Color color;

        imprimere("\n--- Probans accessor functions ---\n");

        color = color_ex_rgba(CCLV, CXXVIII, LXIV, XXXII);
        CREDO_AEQUALIS_I32(color_obtinere_r(color), CCLV);
        CREDO_AEQUALIS_I32(color_obtinere_g(color), CXXVIII);
        CREDO_AEQUALIS_I32(color_obtinere_b(color), LXIV);
        CREDO_AEQUALIS_I32(color_obtinere_a(color), XXXII);
    }

    /* ==================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Destructio */
    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
