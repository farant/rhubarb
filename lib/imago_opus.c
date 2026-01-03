/*
 * imago_opus.c - Image transformation operations
 *
 * Implementatio operationum pro transformare imagines.
 */

#include "imago_opus.h"
#include <string.h>

/* ============================================================
 * Functiones Auxiliares
 * ============================================================ */

/*
 * Nearest neighbor scaling - velox et simplex
 */
hic_manens vacuum
_scalare_proximus(
    constans i8* fons_pixela,
    s32          fons_lat,
    s32          fons_alt,
    i8*          dest_pixela,
    s32          dest_lat,
    s32          dest_alt)
{
    s32 x, y;
    s32 src_x, src_y;
    s32 src_idx, dest_idx;

    per (y = 0; y < dest_alt; y++)
    {
        /* Computare source Y coordinate */
        src_y = (y * fons_alt) / dest_alt;
        si (src_y >= fons_alt) src_y = fons_alt - 1;

        per (x = 0; x < dest_lat; x++)
        {
            /* Computare source X coordinate */
            src_x = (x * fons_lat) / dest_lat;
            si (src_x >= fons_lat) src_x = fons_lat - 1;

            /* Copiare pixel (4 bytes RGBA) */
            src_idx = (src_y * fons_lat + src_x) * 4;
            dest_idx = (y * dest_lat + x) * 4;

            dest_pixela[dest_idx + 0] = fons_pixela[src_idx + 0];
            dest_pixela[dest_idx + 1] = fons_pixela[src_idx + 1];
            dest_pixela[dest_idx + 2] = fons_pixela[src_idx + 2];
            dest_pixela[dest_idx + 3] = fons_pixela[src_idx + 3];
        }
    }
}

/*
 * Bilinear interpolation scaling - lenis sed tardior
 * Utitur fixed-point arithmetica (16 bits fractio)
 */
hic_manens vacuum
_scalare_bilinearis(
    constans i8* fons_pixela,
    s32          fons_lat,
    s32          fons_alt,
    i8*          dest_pixela,
    s32          dest_lat,
    s32          dest_alt)
{
    s32 x, y;
    s32 dest_idx;
    s32 div_x, div_y;
    s32 scale_x, scale_y;

    /* Fixed point: 12 bits pro fractione (evitare overflow) */
    #define FP_SHIFT 12
    #define FP_ONE (1 << FP_SHIFT)

    /* Precomputare scale factors extra ansa */
    div_x = dest_lat > 1 ? dest_lat - 1 : 1;
    div_y = dest_alt > 1 ? dest_alt - 1 : 1;

    /* scale = (fons - 1) * FP_ONE / div */
    scale_x = ((fons_lat - 1) * FP_ONE) / div_x;
    scale_y = ((fons_alt - 1) * FP_ONE) / div_y;

    per (y = 0; y < dest_alt; y++)
    {
        /* Source Y in fixed point: y * scale_y */
        s32 src_y_fp = y * scale_y;
        s32 src_y0 = src_y_fp >> FP_SHIFT;
        s32 src_y1 = src_y0 + 1;
        s32 frac_y = src_y_fp & (FP_ONE - 1);

        si (src_y0 >= fons_alt) src_y0 = fons_alt - 1;
        si (src_y1 >= fons_alt) src_y1 = fons_alt - 1;

        per (x = 0; x < dest_lat; x++)
        {
            /* Source X in fixed point: x * scale_x */
            s32 src_x_fp = x * scale_x;
            s32 src_x0 = src_x_fp >> FP_SHIFT;
            s32 src_x1 = src_x0 + 1;
            s32 frac_x = src_x_fp & (FP_ONE - 1);

            si (src_x0 >= fons_lat) src_x0 = fons_lat - 1;
            si (src_x1 >= fons_lat) src_x1 = fons_lat - 1;

            /* Indices quattuor pixelorum vicinorum */
            {
                s32 idx00 = (src_y0 * fons_lat + src_x0) * 4;
                s32 idx01 = (src_y0 * fons_lat + src_x1) * 4;
                s32 idx10 = (src_y1 * fons_lat + src_x0) * 4;
                s32 idx11 = (src_y1 * fons_lat + src_x1) * 4;

                /* Interpolatio pro singulis canalibus (R, G, B, A) */
                s32 c;
                dest_idx = (y * dest_lat + x) * 4;

                per (c = 0; c < 4; c++)
                {
                    /* Obtinere valores quattuor vicinorum */
                    s32 v00 = (s32)(insignatus character)fons_pixela[idx00 + c];
                    s32 v01 = (s32)(insignatus character)fons_pixela[idx01 + c];
                    s32 v10 = (s32)(insignatus character)fons_pixela[idx10 + c];
                    s32 v11 = (s32)(insignatus character)fons_pixela[idx11 + c];

                    /* Interpolatio horizontalis */
                    s32 v0 = v00 + ((v01 - v00) * frac_x >> FP_SHIFT);
                    s32 v1 = v10 + ((v11 - v10) * frac_x >> FP_SHIFT);

                    /* Interpolatio verticalis */
                    s32 v = v0 + ((v1 - v0) * frac_y >> FP_SHIFT);

                    /* Clamp ad 0-255 */
                    si (v < 0) v = 0;
                    si (v > 255) v = 255;

                    dest_pixela[dest_idx + c] = (i8)v;
                }
            }
        }
    }

    #undef FP_SHIFT
    #undef FP_ONE
}

/* ============================================================
 * Functiones Publicae
 * ============================================================ */

Imago
imago_scalare(
    constans Imago* fons,
    i32             nova_latitudo,
    i32             nova_altitudo,
    ImagoScalaModus modus,
    Piscina*        piscina)
{
    Imago dest;
    i32 pixela_size;

    /* Initiare dest cum valoribus nullis */
    dest.pixela = NIHIL;
    dest.latitudo = 0;
    dest.altitudo = 0;

    /* Validare argumenta */
    si (fons == NIHIL || fons->pixela == NIHIL || piscina == NIHIL)
    {
        redde dest;
    }
    si (nova_latitudo == 0 || nova_altitudo == 0)
    {
        redde dest;
    }

    /* Allocare buffer pro nova imagine */
    pixela_size = nova_latitudo * nova_altitudo * 4;
    dest.pixela = (i8*)piscina_allocare(piscina, (memoriae_index)pixela_size);
    si (dest.pixela == NIHIL)
    {
        redde dest;
    }

    dest.latitudo = nova_latitudo;
    dest.altitudo = nova_altitudo;

    /* Scalare secundum modum */
    commutatio (modus)
    {
        casus IMAGO_SCALA_PROXIMUS:
            _scalare_proximus(fons->pixela,
                              (s32)fons->latitudo, (s32)fons->altitudo,
                              dest.pixela,
                              (s32)dest.latitudo, (s32)dest.altitudo);
            frange;

        casus IMAGO_SCALA_BILINEARIS:
            _scalare_bilinearis(fons->pixela,
                                (s32)fons->latitudo, (s32)fons->altitudo,
                                dest.pixela,
                                (s32)dest.latitudo, (s32)dest.altitudo);
            frange;

        ordinarius:
            /* Default ad nearest neighbor */
            _scalare_proximus(fons->pixela,
                              (s32)fons->latitudo, (s32)fons->altitudo,
                              dest.pixela,
                              (s32)dest.latitudo, (s32)dest.altitudo);
            frange;
    }

    redde dest;
}

Imago
imago_scalare_ad_limites(
    constans Imago* fons,
    i32             max_latitudo,
    i32             max_altitudo,
    ImagoScalaModus modus,
    Piscina*        piscina)
{
    Imago dest;
    s32 nova_lat, nova_alt;
    s32 scale_x_fp, scale_y_fp, scale_fp;
    s32 fons_lat, fons_alt;
    s32 max_lat, max_alt;

    /* Fixed point: 16 bits pro fractione */
    #define SCALE_FP_SHIFT 16
    #define SCALE_FP_ONE (1 << SCALE_FP_SHIFT)

    /* Initiare dest cum valoribus nullis */
    dest.pixela = NIHIL;
    dest.latitudo = 0;
    dest.altitudo = 0;

    /* Validare argumenta */
    si (fons == NIHIL || fons->pixela == NIHIL || piscina == NIHIL)
    {
        redde dest;
    }
    si (max_latitudo == 0 || max_altitudo == 0)
    {
        redde dest;
    }
    si (fons->latitudo == 0 || fons->altitudo == 0)
    {
        redde dest;
    }

    /* Convertere ad signed */
    fons_lat = (s32)fons->latitudo;
    fons_alt = (s32)fons->altitudo;
    max_lat = (s32)max_latitudo;
    max_alt = (s32)max_altitudo;

    /* Si imago iam capit, reddere copiam */
    si (fons_lat <= max_lat && fons_alt <= max_alt)
    {
        redde imago_scalare(fons, fons->latitudo, fons->altitudo, modus, piscina);
    }

    /* Computare scale factors (fixed point, 16 bits fractio) */
    scale_x_fp = (max_lat * SCALE_FP_ONE) / fons_lat;
    scale_y_fp = (max_alt * SCALE_FP_ONE) / fons_alt;

    /* Uti minore scale ut capiat in utrisque dimensionibus */
    scale_fp = (scale_x_fp < scale_y_fp) ? scale_x_fp : scale_y_fp;

    /* Computare novas dimensiones */
    nova_lat = (fons_lat * scale_fp) >> SCALE_FP_SHIFT;
    nova_alt = (fons_alt * scale_fp) >> SCALE_FP_SHIFT;

    /* Assicurare minimum 1x1 */
    si (nova_lat < 1) nova_lat = 1;
    si (nova_alt < 1) nova_alt = 1;

    #undef SCALE_FP_SHIFT
    #undef SCALE_FP_ONE

    /* Scalare ad novas dimensiones */
    redde imago_scalare(fons, (i32)nova_lat, (i32)nova_alt, modus, piscina);
}
