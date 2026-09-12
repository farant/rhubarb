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
_scalare_proximus (
    constans i8* fons_pixela,
            s32  fons_lat,
            s32  fons_alt,
             i8* dest_pixela,
            s32  dest_lat,
            s32  dest_alt)
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
            src_idx   = (src_y * fons_lat + src_x) * 4;
            dest_idx  = (y * dest_lat + x) * 4;

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
_scalare_bilinearis (
    constans i8* fons_pixela,
            s32  fons_lat,
            s32  fons_alt,
             i8* dest_pixela,
            s32  dest_lat,
            s32  dest_alt)
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
        s32 src_y_fp  = y * scale_y;
        s32 src_y0    = src_y_fp >> FP_SHIFT;
        s32 src_y1    = src_y0 + 1;
        s32 frac_y    = src_y_fp & (FP_ONE - 1);

        si (src_y0 >= fons_alt) src_y0 = fons_alt - 1;
        si (src_y1 >= fons_alt) src_y1 = fons_alt - 1;

        per (x = 0; x < dest_lat; x++)
        {
            /* Source X in fixed point: x * scale_x */
            s32 src_x_fp  = x * scale_x;
            s32 src_x0    = src_x_fp >> FP_SHIFT;
            s32 src_x1    = src_x0 + 1;
            s32 frac_x    = src_x_fp & (FP_ONE - 1);

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
                    s32 v00 =
                        (s32)(insignatus character)fons_pixela[idx00
                            + c];
                    s32 v01 =
                        (s32)(insignatus character)fons_pixela[idx01
                            + c];
                    s32 v10 =
                        (s32)(insignatus character)fons_pixela[idx10
                            + c];
                    s32 v11 =
                        (s32)(insignatus character)fons_pixela[idx11
                            + c];

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

#define AREA_FIX_SHIFT XVI
#define AREA_FIX_ONE   (((s64)I) << AREA_FIX_SHIFT)

/* Media areae ALPHA-PONDERATA. Pixela NON praemultiplicata sunt
 * (imago_typus.h), ergo RGB per alpha ponderandum et per summam alpha
 * dividendum est: aliter pixelum pellucidum colorem suum in vicinos
 * trahit (halo classicus), et icones marginibus pellucidis constant.
 *
 * Accumulatio in s64: area LXIV x LXIV ad ~266M summat (intra s32), sed
 * reductio CCLVI x duo miliarda transit. Offsetum quoque in s64
 * computatur - semitae veteres s32 utuntur et circa 23170^2 fontis
 * circumvolvuntur.
 *
 * Ad 1:1 nulla custodia opus est: extentum unius pixeli pondus plenum
 * accipit et rotundatio dimidii sursum valorem non movet. */
interior vacuum
_scalare_area (
    constans i8* fons_pixela,
            s32  fons_lat,
            s32  fons_alt,
             i8* dest_pixela,
            s32  dest_lat,
            s32  dest_alt)
{
    s32 dx, dy;

    per (dy = ZEPHYRUM; dy < dest_alt; dy++)
    {
        s64 sy0 = ((s64)dy * fons_alt * AREA_FIX_ONE) / dest_alt;
        s64 sy1 = ((s64)(dy + I) * fons_alt * AREA_FIX_ONE) / dest_alt;
        s32 y_primus = (s32)(sy0 >> AREA_FIX_SHIFT);
        s32 y_ultimus = (s32)((sy1 - I) >> AREA_FIX_SHIFT);

        si (y_ultimus >= fons_alt)
        {
            y_ultimus = fons_alt - I;
        }

        per (dx = ZEPHYRUM; dx < dest_lat; dx++)
        {
            s64 sx0 = ((s64)dx * fons_lat * AREA_FIX_ONE) / dest_lat;
            s64 sx1 = ((s64)(dx + I) * fons_lat * AREA_FIX_ONE)
                / dest_lat;
            s32 x_primus      = (s32)(sx0 >> AREA_FIX_SHIFT);
            s32 x_ultimus     = (s32)((sx1 - I) >> AREA_FIX_SHIFT);
            s64 pondus_totum  = ZEPHYRUM;
            s64 summa_a       = ZEPHYRUM;
            s64 summa_r       = ZEPHYRUM;
            s64 summa_g       = ZEPHYRUM;
            s64 summa_b       = ZEPHYRUM;
            s32 sy, sx;
            i8* dest_ptr;

            si (x_ultimus >= fons_lat)
            {
                x_ultimus = fons_lat - I;
            }

            per (sy = y_primus; sy <= y_ultimus; sy++)
            {
                s64 y_ab = (sy == y_primus)
                    ? sy0 : ((s64)sy * AREA_FIX_ONE);
                s64 y_ad = (sy == y_ultimus)
                    ? sy1 : ((s64)(sy + I) * AREA_FIX_ONE);
                s64 pondus_y = y_ad - y_ab;

                per (sx = x_primus; sx <= x_ultimus; sx++)
                {
                    s64 x_ab = (sx == x_primus)
                        ? sx0 : ((s64)sx * AREA_FIX_ONE);
                    s64 x_ad = (sx == x_ultimus)
                        ? sx1 : ((s64)(sx + I) * AREA_FIX_ONE);
                    s64 pondus = (pondus_y * (x_ad - x_ab))
                        >> AREA_FIX_SHIFT;
                    s64 offsetum  = (((s64)sy * fons_lat) + sx) * IV;
                    s64 a         = (s64)fons_pixela[offsetum + III];

                    pondus_totum  += pondus;
                    summa_a       += a * pondus;
                    summa_r       += (s64)fons_pixela[offsetum] * a
                        * pondus;
                    summa_g       += (s64)fons_pixela[offsetum + I] * a
                        * pondus;
                    summa_b       += (s64)fons_pixela[offsetum + II] * a
                        * pondus;
                }
            }

            dest_ptr = dest_pixela
                + ((((s64)dy * dest_lat) + dx) * IV);

            si (pondus_totum <= ZEPHYRUM)
            {
                dest_ptr[ZEPHYRUM]  = ZEPHYRUM;
                dest_ptr[I]         = ZEPHYRUM;
                dest_ptr[II]        = ZEPHYRUM;
                dest_ptr[III]       = ZEPHYRUM;
                perge;
            }

            /* alpha = media simplex; RGB = media per alpha ponderata */
            dest_ptr[III] = (i8)((summa_a + (pondus_totum / II))
                / pondus_totum);
            si (summa_a > ZEPHYRUM)
            {
                dest_ptr[ZEPHYRUM] = (i8)((summa_r + (summa_a / II))
                    / summa_a);
                dest_ptr[I] = (i8)((summa_g + (summa_a / II))
                    / summa_a);
                dest_ptr[II] = (i8)((summa_b + (summa_a / II))
                    / summa_a);
            }
            alioquin
            {
                dest_ptr[ZEPHYRUM]  = ZEPHYRUM;
                dest_ptr[I]         = ZEPHYRUM;
                dest_ptr[II]        = ZEPHYRUM;
            }
        }
    }
}

Imago
imago_scalare (
     constans Imago* fons,
                i32  nova_latitudo,
                i32  nova_altitudo,
    ImagoScalaModus  modus,
            Piscina* piscina)
{
    Imago dest;
      i32 pixela_size;

    /* Initiare dest cum valoribus nullis */
    dest.pixela    = NIHIL;
    dest.latitudo  = 0;
    dest.altitudo  = 0;

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
    dest.pixela = (i8*)piscina_allocare(piscina,
        (memoriae_index)pixela_size);
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
                                (s32)fons->latitudo,
                                (s32)fons->altitudo,
                                dest.pixela,
                                (s32)dest.latitudo, (s32)dest.altitudo);
            frange;
        casus IMAGO_SCALA_AREA:
            _scalare_area(fons->pixela,
                                (s32)fons->latitudo,
                                (s32)fons->altitudo,
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
imago_scalare_ad_limites (
     constans Imago* fons,
                i32  max_latitudo,
                i32  max_altitudo,
    ImagoScalaModus  modus,
            Piscina* piscina)
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
    dest.pixela    = NIHIL;
    dest.latitudo  = 0;
    dest.altitudo  = 0;

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
    fons_lat  = (s32)fons->latitudo;
    fons_alt  = (s32)fons->altitudo;
    max_lat   = (s32)max_latitudo;
    max_alt   = (s32)max_altitudo;

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
    redde imago_scalare(fons, (i32)nova_lat, (i32)nova_alt, modus,
        piscina);
}

Imago
imago_extrahere_et_scalare (
     constans Imago* fons,
                i32  crop_x,
                i32  crop_y,
                i32  crop_lat,
                i32  crop_alt,
                i32  max_latitudo,
                i32  max_altitudo,
    ImagoScalaModus  modus,
            Piscina* piscina)
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
    dest.pixela    = NIHIL;
    dest.latitudo  = 0;
    dest.altitudo  = 0;

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
    dest.pixela = (i8*)piscina_allocare(piscina,
        (memoriae_index)(dest_lat * dest_alt * IV));
    si (dest.pixela == NIHIL)
    {
        redde dest;
    }
    dest.latitudo = (i32)dest_lat;
    dest.altitudo = (i32)dest_alt;

    /* Transitus unus: regio + scala. AREA PRIMUM: olim modus nullus
     * nisi PROXIMUS hic agnoscebatur, et AREA TACITE in ramum
     * bilinearem cadebat - icones sic omnes icones bilineares scripsit
     * (2026-09-12, oraculum sips congelatum invenit). Regio copiatur et
     * _scalare_area ipsum vocatur, quod I2 iam pinnat: nulla
     * arithmetica nova ubi falsa fieri posset. */
    si (modus == IMAGO_SCALA_AREA)
    {
         i8* regio;
        s32  ry;

        regio = (i8*)piscina_allocare(piscina,
            (memoriae_index)((s64)cw * ch * IV));
        si (regio == NIHIL)
        {
            dest.pixela    = NIHIL;
            dest.latitudo  = 0;
            dest.altitudo  = 0;
            redde dest;
        }
        per (ry = 0; ry < ch; ry++)
        {
            memcpy(regio + ((s64)ry * cw * IV),
                   fons->pixela + ((((s64)cy + ry) * fons_lat
                       + cx) * IV),
                   (size_t)(cw * IV));
        }
        _scalare_area(regio, cw, ch, dest.pixela, dest_lat, dest_alt);
    }
    alioquin si (modus == IMAGO_SCALA_PROXIMUS)
    {
        /* Nearest neighbor - optimized with pointer arithmetic */
         i8* dest_row        = dest.pixela;
        s32  dest_row_bytes  = dest_lat * IV;

        per (y = 0; y < dest_alt; y++)
        {
            /* Compute source row once per scanline */
                    s32  src_y = cy + (y * ch) / dest_alt;
            constans i8* src_row;
                     i8* dest_ptr = dest_row;

            si (src_y >= fons_alt) src_y = fons_alt - 1;
            src_row = fons->pixela + (src_y * fons_lat * IV);

            per (x = 0; x < dest_lat; x++)
            {
                        s32  src_x = cx + (x * cw) / dest_lat;
                constans i8* src_ptr;

                si (src_x >= fons_lat) src_x = fons_lat - 1;
                src_ptr = src_row + (src_x * IV);

                /* Copy 4 bytes as 32-bit value */
                *(insignatus integer*)dest_ptr =
                    *(constans insignatus integer*)src_ptr;
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

        s32 scale_x_bl = (cw > 1) ? ((cw - 1) * FP_ONE_BL) / (dest_lat
            > 1 ? dest_lat - 1 : 1) : 0;
        s32 scale_y_bl = (ch > 1) ? ((ch - 1) * FP_ONE_BL) / (dest_alt
            > 1 ? dest_alt - 1 : 1) : 0;

        per (y = 0; y < dest_alt; y++)
        {
            s32 src_y_fp  = y * scale_y_bl;
            s32 src_y0    = cy + (src_y_fp >> FP_SHIFT);
            s32 src_y1    = src_y0 + 1;
            s32 frac_y    = src_y_fp & (FP_ONE_BL - 1);

            si (src_y0 >= fons_alt) src_y0 = fons_alt - 1;
            si (src_y1 >= fons_alt) src_y1 = fons_alt - 1;

            per (x = 0; x < dest_lat; x++)
            {
                s32 src_x_fp  = x * scale_x_bl;
                s32 src_x0    = cx + (src_x_fp >> FP_SHIFT);
                s32 src_x1    = src_x0 + 1;
                s32 frac_x    = src_x_fp & (FP_ONE_BL - 1);
                s32 idx00, idx01, idx10, idx11;
                s32 dst_idx;

                si (src_x0 >= fons_lat) src_x0 = fons_lat - 1;
                si (src_x1 >= fons_lat) src_x1 = fons_lat - 1;

                idx00    = (src_y0 * fons_lat + src_x0) * IV;
                idx01    = (src_y0 * fons_lat + src_x1) * IV;
                idx10    = (src_y1 * fons_lat + src_x0) * IV;
                idx11    = (src_y1 * fons_lat + src_x1) * IV;
                dst_idx  = (y * dest_lat + x) * IV;

                /* Unrolled RGBA interpolation */
                {
                    s32 v00, v01, v10, v11, v0, v1, v;

                    /* R */
                    v00 = (s32)(insignatus character)fons->pixela[idx00
                        + 0];
                    v01 = (s32)(insignatus character)fons->pixela[idx01
                        + 0];
                    v10 = (s32)(insignatus character)fons->pixela[idx10
                        + 0];
                    v11 = (s32)(insignatus character)fons->pixela[idx11
                        + 0];
                    v0 = v00 + ((v01 - v00) * frac_x >> FP_SHIFT);
                    v1 = v10 + ((v11 - v10) * frac_x >> FP_SHIFT);
                    v = v0 + ((v1 - v0) * frac_y >> FP_SHIFT);
                    dest.pixela[dst_idx + 0] = (i8)v;

                    /* G */
                    v00 = (s32)(insignatus character)fons->pixela[idx00
                        + 1];
                    v01 = (s32)(insignatus character)fons->pixela[idx01
                        + 1];
                    v10 = (s32)(insignatus character)fons->pixela[idx10
                        + 1];
                    v11 = (s32)(insignatus character)fons->pixela[idx11
                        + 1];
                    v0 = v00 + ((v01 - v00) * frac_x >> FP_SHIFT);
                    v1 = v10 + ((v11 - v10) * frac_x >> FP_SHIFT);
                    v = v0 + ((v1 - v0) * frac_y >> FP_SHIFT);
                    dest.pixela[dst_idx + 1] = (i8)v;

                    /* B */
                    v00 = (s32)(insignatus character)fons->pixela[idx00
                        + 2];
                    v01 = (s32)(insignatus character)fons->pixela[idx01
                        + 2];
                    v10 = (s32)(insignatus character)fons->pixela[idx10
                        + 2];
                    v11 = (s32)(insignatus character)fons->pixela[idx11
                        + 2];
                    v0 = v00 + ((v01 - v00) * frac_x >> FP_SHIFT);
                    v1 = v10 + ((v11 - v10) * frac_x >> FP_SHIFT);
                    v = v0 + ((v1 - v0) * frac_y >> FP_SHIFT);
                    dest.pixela[dst_idx + 2] = (i8)v;

                    /* A */
                    v00 = (s32)(insignatus character)fons->pixela[idx00
                        + 3];
                    v01 = (s32)(insignatus character)fons->pixela[idx01
                        + 3];
                    v10 = (s32)(insignatus character)fons->pixela[idx10
                        + 3];
                    v11 = (s32)(insignatus character)fons->pixela[idx11
                        + 3];
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
