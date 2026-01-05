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

    /* Computare scale factors (fixed point, 16 bits fractio)
     * Semper scalare ad limites, etiam si imago iam capit (pro zoom) */
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

Imago
imago_extrahere_et_scalare(
    constans Imago* fons,
    i32             crop_x,
    i32             crop_y,
    i32             crop_lat,
    i32             crop_alt,
    i32             max_latitudo,
    i32             max_altitudo,
    ImagoScalaModus modus,
    Piscina*        piscina)
{
    Imago dest;
    s32 fons_lat, fons_alt;
    s32 cx, cy, cw, ch;
    s32 dest_lat, dest_alt;
    s32 scale_fp;
    s32 x, y;

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
    si (crop_lat == 0 || crop_alt == 0)
    {
        redde dest;
    }
    si (max_latitudo == 0 || max_altitudo == 0)
    {
        redde dest;
    }

    fons_lat = (s32)fons->latitudo;
    fons_alt = (s32)fons->altitudo;

    /* Copiare parametros crop ad variabiles locales */
    cx = (s32)crop_x;
    cy = (s32)crop_y;
    cw = (s32)crop_lat;
    ch = (s32)crop_alt;

    /* Clamp crop region ad limites fontis */
    si (cx + cw > fons_lat) cw = fons_lat - cx;
    si (cy + ch > fons_alt) ch = fons_alt - cy;

    si (cw == 0 || ch == 0)
    {
        redde dest;
    }

    /* Computare output dimensions (fit within max, preserve aspect) */
    {
        s32 scale_x_fp = ((s32)max_latitudo * SCALE_FP_ONE) / cw;
        s32 scale_y_fp = ((s32)max_altitudo * SCALE_FP_ONE) / ch;
        scale_fp = (scale_x_fp < scale_y_fp) ? scale_x_fp : scale_y_fp;
        dest_lat = (cw * scale_fp) >> SCALE_FP_SHIFT;
        dest_alt = (ch * scale_fp) >> SCALE_FP_SHIFT;
        si (dest_lat < 1) dest_lat = 1;
        si (dest_alt < 1) dest_alt = 1;
    }

    /* Allocare output buffer */
    dest.pixela = (i8*)piscina_allocare(piscina, (memoriae_index)(dest_lat * dest_alt * IV));
    si (dest.pixela == NIHIL)
    {
        redde dest;
    }
    dest.latitudo = (i32)dest_lat;
    dest.altitudo = (i32)dest_alt;

    /* Single-pass crop + scale (nearest neighbor or bilinear) */
    si (modus == IMAGO_SCALA_PROXIMUS)
    {
        /* Nearest neighbor - optimized with pointer arithmetic */
        i8* dest_row = dest.pixela;
        s32 dest_row_bytes = dest_lat * IV;

        per (y = 0; y < dest_alt; y++)
        {
            /* Compute source row once per scanline */
            s32 src_y = cy + (y * ch) / dest_alt;
            constans i8* src_row;
            i8* dest_ptr = dest_row;

            si (src_y >= fons_alt) src_y = fons_alt - 1;
            src_row = fons->pixela + (src_y * fons_lat * IV);

            per (x = 0; x < dest_lat; x++)
            {
                s32 src_x = cx + (x * cw) / dest_lat;
                constans i8* src_ptr;

                si (src_x >= fons_lat) src_x = fons_lat - 1;
                src_ptr = src_row + (src_x * IV);

                /* Copy 4 bytes as 32-bit value */
                *(insignatus integer*)dest_ptr = *(constans insignatus integer*)src_ptr;
                dest_ptr += IV;
            }
            dest_row += dest_row_bytes;
        }
    }
    alioquin
    {
        /* Bilinear - from source crop region */
        #define FP_SHIFT 12
        #define FP_ONE_BL (1 << FP_SHIFT)

        s32 scale_x_bl = (cw > 1) ? ((cw - 1) * FP_ONE_BL) / (dest_lat > 1 ? dest_lat - 1 : 1) : 0;
        s32 scale_y_bl = (ch > 1) ? ((ch - 1) * FP_ONE_BL) / (dest_alt > 1 ? dest_alt - 1 : 1) : 0;

        per (y = 0; y < dest_alt; y++)
        {
            s32 src_y_fp = y * scale_y_bl;
            s32 src_y0 = cy + (src_y_fp >> FP_SHIFT);
            s32 src_y1 = src_y0 + 1;
            s32 frac_y = src_y_fp & (FP_ONE_BL - 1);

            si (src_y0 >= fons_alt) src_y0 = fons_alt - 1;
            si (src_y1 >= fons_alt) src_y1 = fons_alt - 1;

            per (x = 0; x < dest_lat; x++)
            {
                s32 src_x_fp = x * scale_x_bl;
                s32 src_x0 = cx + (src_x_fp >> FP_SHIFT);
                s32 src_x1 = src_x0 + 1;
                s32 frac_x = src_x_fp & (FP_ONE_BL - 1);
                s32 idx00, idx01, idx10, idx11;
                s32 dst_idx;

                si (src_x0 >= fons_lat) src_x0 = fons_lat - 1;
                si (src_x1 >= fons_lat) src_x1 = fons_lat - 1;

                idx00 = (src_y0 * fons_lat + src_x0) * IV;
                idx01 = (src_y0 * fons_lat + src_x1) * IV;
                idx10 = (src_y1 * fons_lat + src_x0) * IV;
                idx11 = (src_y1 * fons_lat + src_x1) * IV;
                dst_idx = (y * dest_lat + x) * IV;

                /* Unrolled RGBA interpolation */
                {
                    s32 v00, v01, v10, v11, v0, v1, v;

                    /* R */
                    v00 = (s32)(insignatus character)fons->pixela[idx00 + 0];
                    v01 = (s32)(insignatus character)fons->pixela[idx01 + 0];
                    v10 = (s32)(insignatus character)fons->pixela[idx10 + 0];
                    v11 = (s32)(insignatus character)fons->pixela[idx11 + 0];
                    v0 = v00 + ((v01 - v00) * frac_x >> FP_SHIFT);
                    v1 = v10 + ((v11 - v10) * frac_x >> FP_SHIFT);
                    v = v0 + ((v1 - v0) * frac_y >> FP_SHIFT);
                    dest.pixela[dst_idx + 0] = (i8)v;

                    /* G */
                    v00 = (s32)(insignatus character)fons->pixela[idx00 + 1];
                    v01 = (s32)(insignatus character)fons->pixela[idx01 + 1];
                    v10 = (s32)(insignatus character)fons->pixela[idx10 + 1];
                    v11 = (s32)(insignatus character)fons->pixela[idx11 + 1];
                    v0 = v00 + ((v01 - v00) * frac_x >> FP_SHIFT);
                    v1 = v10 + ((v11 - v10) * frac_x >> FP_SHIFT);
                    v = v0 + ((v1 - v0) * frac_y >> FP_SHIFT);
                    dest.pixela[dst_idx + 1] = (i8)v;

                    /* B */
                    v00 = (s32)(insignatus character)fons->pixela[idx00 + 2];
                    v01 = (s32)(insignatus character)fons->pixela[idx01 + 2];
                    v10 = (s32)(insignatus character)fons->pixela[idx10 + 2];
                    v11 = (s32)(insignatus character)fons->pixela[idx11 + 2];
                    v0 = v00 + ((v01 - v00) * frac_x >> FP_SHIFT);
                    v1 = v10 + ((v11 - v10) * frac_x >> FP_SHIFT);
                    v = v0 + ((v1 - v0) * frac_y >> FP_SHIFT);
                    dest.pixela[dst_idx + 2] = (i8)v;

                    /* A */
                    v00 = (s32)(insignatus character)fons->pixela[idx00 + 3];
                    v01 = (s32)(insignatus character)fons->pixela[idx01 + 3];
                    v10 = (s32)(insignatus character)fons->pixela[idx10 + 3];
                    v11 = (s32)(insignatus character)fons->pixela[idx11 + 3];
                    v0 = v00 + ((v01 - v00) * frac_x >> FP_SHIFT);
                    v1 = v10 + ((v11 - v10) * frac_x >> FP_SHIFT);
                    v = v0 + ((v1 - v0) * frac_y >> FP_SHIFT);
                    dest.pixela[dst_idx + 3] = (i8)v;
                }
            }
        }

        #undef FP_SHIFT
        #undef FP_ONE_BL
    }

    #undef SCALE_FP_SHIFT
    #undef SCALE_FP_ONE

    redde dest;
}
