/* probatio_color.c - Probationes Bibliothecae Coloris */
#include "latina.h"
#include "color.h"
#include "piscina.h"
#include "credo.h"
#include "thema.h"
#include <stdio.h>


interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    (vacuum)c;

    /* Initiare thema pro palette tests */
    thema_initiare();
}


/* ==================================================
 * Probare color_ex_rgb
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Color color;

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
}


/* ==================================================
 * Probare color_ex_rgba
 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Color color;

        color = color_ex_rgba(CCLV, CXXVIII, LXIV, XXXII);
        CREDO_AEQUALIS_I32(color.r, CCLV);
        CREDO_AEQUALIS_I32(color.g, CXXVIII);
        CREDO_AEQUALIS_I32(color.b, LXIV);
        CREDO_AEQUALIS_I32(color.a, XXXII);
    }
}


/* ==================================================
 * Probare color_ex_palette
 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Color color;

        /* Test black (palette index 0) */
        color = color_ex_palette(ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.r, ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.g, ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.b, ZEPHYRUM);
        CREDO_AEQUALIS_I32(color.a, CCLV);

        /* Test warm white (palette index 5: 0x3F, 0x3C, 0x3A) */
        color = color_ex_palette(V);
        CREDO_AEQUALIS_I32(color.r, CCLV);  /* 0x3F -> 255 */
        CREDO_AEQUALIS_I32(color.g, CCXLII);   /* 0x3C -> 242 */
        CREDO_AEQUALIS_I32(color.b, CCXXXIV);   /* 0x3A -> 234 */
    }
}


/* ==================================================
 * Probare color_ad_pixelum et color_ex_pixelum (round trip)
 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Color color1, color2;
        i32 pixel;

        /* Create a color, convert to pixel, convert back */
        color1  = color_ex_rgba(CCLV, CXXVIII, LXIV, CCLV);
        pixel   = color_ad_pixelum(color1);
        color2  = color_ex_pixelum(pixel);

        /* Should match after round trip */
        CREDO_AEQUALIS_I32(color2.r, CCLV);
        CREDO_AEQUALIS_I32(color2.g, CXXVIII);
        CREDO_AEQUALIS_I32(color2.b, LXIV);
        CREDO_AEQUALIS_I32(color2.a, CCLV);

        /* Test with black */
        color1  = color_ex_rgb(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
        pixel   = color_ad_pixelum(color1);
        color2  = color_ex_pixelum(pixel);
        CREDO_VERUM(color_aequalis(color1, color2));

        /* Test with white */
        color1  = color_ex_rgb(CCLV, CCLV, CCLV);
        pixel   = color_ad_pixelum(color1);
        color2  = color_ex_pixelum(pixel);
        CREDO_VERUM(color_aequalis(color1, color2));
    }
}


/* ==================================================
 * Probare color_interpolate
 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Color black, white, gray;

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
}


/* ==================================================
 * Probare color_ad_cinereum
 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Color color;
           i8 gray;

        /* Black -> 0 */
        color  = color_ex_rgb(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
        gray   = color_ad_cinereum(color);
        CREDO_AEQUALIS_I32(gray, ZEPHYRUM);

        /* White -> 255 */
        color  = color_ex_rgb(CCLV, CCLV, CCLV);
        gray   = color_ad_cinereum(color);
        CREDO_AEQUALIS_I32(gray, CCLV);

        /* Red (uses ITU-R BT.601 weights: 30% red) */
        color  = color_ex_rgb(CCLV, ZEPHYRUM, ZEPHYRUM);
        gray   = color_ad_cinereum(color);
        CREDO_AEQUALIS_I32(gray, LXXVI);  /* 255 * 0.30 ≈ 76 */

        /* Green (59% green) */
        color  = color_ex_rgb(ZEPHYRUM, CCLV, ZEPHYRUM);
        gray   = color_ad_cinereum(color);
        CREDO_AEQUALIS_I32(gray, CL);  /* 255 * 0.59 ≈ 150 */

        /* Blue (11% blue) */
        color  = color_ex_rgb(ZEPHYRUM, ZEPHYRUM, CCLV);
        gray   = color_ad_cinereum(color);
        CREDO_AEQUALIS_I32(gray, XXVIII);  /* 255 * 0.11 ≈ 28 */
    }
}


/* ==================================================
 * Probare color_aequalis
 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Color color1, color2;

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
}


/* ==================================================
 * Probare colores praedefinitos
 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    (vacuum)c;
    {
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
}


/* ==================================================
 * Probare accessor functions
 * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Color color;

        color = color_ex_rgba(CCLV, CXXVIII, LXIV, XXXII);
        CREDO_AEQUALIS_I32(color_obtinere_r(color), CCLV);
        CREDO_AEQUALIS_I32(color_obtinere_g(color), CXXVIII);
        CREDO_AEQUALIS_I32(color_obtinere_b(color), LXIV);
        CREDO_AEQUALIS_I32(color_obtinere_a(color), XXXII);
    }
}


/* ==================================================
 * Probare spatia colorum (lux linearis, OKLab)
 * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        f64 lin[III], lab[III], retro[III];
        f64 nigrum[III]  = {0.0, 0.0, 0.0};
        f64 album[III]   = {1.0, 1.0, 1.0};
        s32 i;

        /* curva sRGB: extrema, medium, et reditus */
        CREDO_F64_PROXIMUS(color_srgb_ad_linearem(0.0), 0.0, 1e-12);
        CREDO_F64_PROXIMUS(color_srgb_ad_linearem(1.0), 1.0, 1e-12);
        CREDO_F64_PROXIMUS(color_srgb_ad_linearem(0.5), 0.214041, 1e-6);
        per (i = 0; i <= C; i++)
        {
            f64 c = (f64)i / 100.0;
            CREDO_F64_PROXIMUS(color_linearis_ad_srgb(color_srgb_ad_linearem(c)),
                c, 1e-9);
        }

        /* tabula nigro-alba alternans: media lineare 0.5 -> sRGB ~188, non 128 */
        CREDO_F64_PROXIMUS(color_linearis_ad_srgb(0.5) * 255.0, 187.5,
            0.5);

        /* octeti per tabulam = formula */
        CREDO_F64_PROXIMUS(color_octetus_ad_linearem(0), 0.0, 1e-12);
        CREDO_F64_PROXIMUS(color_octetus_ad_linearem(CCLV), 1.0, 1e-12);
        per (i = 0; i < CCLVI; i++)
        {
            CREDO_F64_PROXIMUS(color_octetus_ad_linearem((i8)i),
                               color_srgb_ad_linearem((f64)i / 255.0),
                               1e-12);
        }

        /* OKLab: niger, albus, ruber purus (Ottosson: 0.627955, 0.224863, 0.125846) */
        color_linearis_ad_oklab(nigrum, lab);
        CREDO_F64_PROXIMUS(lab[0], 0.0, 1e-9);
        CREDO_F64_PROXIMUS(lab[1], 0.0, 1e-9);
        CREDO_F64_PROXIMUS(lab[2], 0.0, 1e-9);
        color_linearis_ad_oklab(album, lab);
        CREDO_F64_PROXIMUS(lab[0], 1.0, 1e-4);
        CREDO_F64_PROXIMUS(lab[1], 0.0, 1e-4);
        CREDO_F64_PROXIMUS(lab[2], 0.0, 1e-4);
        lin[0] = 1.0; lin[1] = 0.0; lin[2] = 0.0;
        color_linearis_ad_oklab(lin, lab);
        CREDO_F64_PROXIMUS(lab[0], 0.627955, 1e-4);
        CREDO_F64_PROXIMUS(lab[1], 0.224863, 1e-4);
        CREDO_F64_PROXIMUS(lab[2], 0.125846, 1e-4);

        /* reditus lineare -> OKLab -> lineare. Matrices Ottosson X
         * digitis editae non sunt inversae exactae: error maximus
         * 2.6e-7 (mensuratum 2026-10-13), longe infra octetum (~2e-3). */
        per (i = 0; i < XXVII; i++)
        {
            lin[0] = (f64)(i % III) / 2.0;
            lin[1] = (f64)((i / III) % III) / 2.0;
            lin[2] = (f64)(i / IX) / 2.0;
            color_linearis_ad_oklab(lin, lab);
            color_oklab_ad_linearem(lab, retro);
            CREDO_F64_PROXIMUS(retro[0], lin[0], 1e-6);
            CREDO_F64_PROXIMUS(retro[1], lin[1], 1e-6);
            CREDO_F64_PROXIMUS(retro[2], lin[2], 1e-6);
        }

        /* negativa ad 0 tonduntur (error diffusus extra gamutum) */
        lin[0] = -0.2; lin[1] = 0.0; lin[2] = 0.0;
        color_linearis_ad_oklab(lin, lab);
        CREDO_F64_PROXIMUS(lab[0], 0.0, 1e-9);

        /* commoditas Color et distantia */
        color_ad_oklab(COLOR_ALBUS, lab);
        CREDO_F64_PROXIMUS(lab[0], 1.0, 1e-4);
        color_ad_oklab(COLOR_NIGER, retro);
        CREDO_F64_PROXIMUS(color_oklab_distantia(lab, retro), 1.0,
            1e-4);
        CREDO_F64_PROXIMUS(color_oklab_distantia(lab, lab), 0.0, 1e-12);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "color_ex_rgb", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "color_ex_rgba", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "color_ex_palette", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "color_ad_pixelum / color_ex_pixelum", _sectio_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "color_interpolate", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "color_ad_cinereum", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "color_aequalis", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "colores praedefinitos", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "accessor functions", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "spatia colorum", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("color", SECTIONES,
        _suitam_parare, NIHIL);
}
