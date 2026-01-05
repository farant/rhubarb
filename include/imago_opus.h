/*
 * imago_opus.h - Image transformation operations
 *
 * Operationes pro transformare imagines: scalare, etc.
 * Separatum ab imago.h (caricare) pro separatione concernuum.
 */

#ifndef IMAGO_OPUS_H
#define IMAGO_OPUS_H

#include "latina.h"
#include "imago.h"
#include "piscina.h"

/* ============================================================
 * Typi
 * ============================================================ */

/*
 * ImagoScalaModus - Algorithmus pro scalare
 */
nomen enumeratio {
    IMAGO_SCALA_PROXIMUS,   /* Nearest neighbor - velox, pixelatum */
    IMAGO_SCALA_BILINEARIS  /* Bilinear interpolatio - lenis */
} ImagoScalaModus;

/* ============================================================
 * Functiones
 * ============================================================ */

/*
 * imago_scalare - Scalare imaginem ad dimensiones exactas
 *
 * @fons:          Imago originalis
 * @nova_latitudo: Nova latitudo in pixelis
 * @nova_altitudo: Nova altitudo in pixelis
 * @modus:         Algorithmus scalandi
 * @piscina:       Piscina pro allocare novam imaginem
 *
 * Redde: Nova imago scalata (allocata in piscina)
 */
Imago
imago_scalare(
    constans Imago* fons,
    i32             nova_latitudo,
    i32             nova_altitudo,
    ImagoScalaModus modus,
    Piscina*        piscina);

/*
 * imago_scalare_ad_limites - Scalare ad limites, servare proportionem
 *
 * Scalat imaginem ut capiat intra limites datos, sed servat
 * proportionem aspectus (aspect ratio). Imago finalis potest
 * esse minor quam limites in una dimensione.
 *
 * @fons:          Imago originalis
 * @max_latitudo:  Latitudo maxima
 * @max_altitudo:  Altitudo maxima
 * @modus:         Algorithmus scalandi
 * @piscina:       Piscina pro allocare novam imaginem
 *
 * Redde: Nova imago scalata (allocata in piscina)
 */
Imago
imago_scalare_ad_limites(
    constans Imago* fons,
    i32             max_latitudo,
    i32             max_altitudo,
    ImagoScalaModus modus,
    Piscina*        piscina);

/*
 * imago_extrahere_et_scalare - Extrahere regionem et scalare
 *
 * Extrahit regionem rectangularem ab imagine fonte et scalat
 * ad limites datos, servando proportionem aspectus.
 *
 * @fons:          Imago originalis
 * @crop_x:        X originis regionis in fonte
 * @crop_y:        Y originis regionis in fonte
 * @crop_lat:      Latitudo regionis
 * @crop_alt:      Altitudo regionis
 * @max_latitudo:  Latitudo maxima output
 * @max_altitudo:  Altitudo maxima output
 * @modus:         Algorithmus scalandi
 * @piscina:       Piscina pro allocare novam imaginem
 *
 * Redde: Nova imago extracta et scalata
 */
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
    Piscina*        piscina);

#endif /* IMAGO_OPUS_H */
