/* imago_opus.h - imagines scalare (image resize, transforms)
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
    IMAGO_SCALA_BILINEARIS, /* Bilinear interpolatio - lenis */
    IMAGO_SCALA_AREA        /* Media areae, alpha-ponderata: pro
                             * DEMINUTIONE. Bilinearis vicinitatem
                             * II x II legit quantavis scala sit, ergo
                             * 1024 -> 16 pixela IV ex 4096 legit. Ad
                             * AUCTIONEM bilinearem adhibe. */
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
imago_scalare (
     constans Imago* fons,
                i32  nova_latitudo,
                i32  nova_altitudo,
    ImagoScalaModus  modus,
            Piscina* piscina);

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
imago_scalare_ad_limites (
     constans Imago* fons,
                i32  max_latitudo,
                i32  max_altitudo,
    ImagoScalaModus  modus,
            Piscina* piscina);

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
imago_extrahere_et_scalare (
     constans Imago* fons,
                i32  crop_x,
                i32  crop_y,
                i32  crop_lat,
                i32  crop_alt,
                i32  max_latitudo,
                i32  max_altitudo,
    ImagoScalaModus  modus,
            Piscina* piscina);


/* ==================================================
 * Creare, excidere, transcribere, rectangulum (lapide FR-021)
 *
 * SINE scala et SINE mixtura alpha: pixela RGBA substituuntur.
 * Omnia ad margines praecisa (regio extra = nihil, non error).
 * Imago vacua = pixela NIHIL, dimensiones 0. (TabulaPixelorum
 * fenestrae est tabula FENESTRAE - i32 RGBA8888 cum scala; delineare
 * in eam pingit, non in Imago. Imago hic manet sine fenestra.)
 * ================================================== */

/*
 * imago_creare - Imago nova colore uno plena (RGBA)
 *
 * Dimensio nulla -> imago vacua.
 */
Imago
imago_creare (
         i32  latitudo,
         i32  altitudo,
          i8  r,
          i8  g,
          i8  b,
          i8  a,
     Piscina* piscina);

/*
 * imago_excidere - Regionem exscindere SINE scala
 *
 * Regio ad fontem praecisa (intersectio): (III, III, V, V) in IV x IV
 * -> I x I. Regio tota extra aut vacua -> imago vacua.
 */
Imago
imago_excidere (
    constans Imago* fons,
               i32  x,
               i32  y,
               i32  latitudo,
               i32  altitudo,
           Piscina* piscina);

/*
 * imago_transcribere - Fontem in dest ad (dx, dy) transcribere
 *
 * Pixela dest SUBSTITUUNTUR (etiam alpha fontis nulla) - transcriptio,
 * non compositio. dx, dy negativi licent; praecisio in omnibus
 * marginibus; tota extra = nihil mutatum.
 */
vacuum
imago_transcribere (
             Imago* dest,
    constans Imago* fons,
               s32  dx,
               s32  dy);

/*
 * imago_rectangulum - Ambitus rectanguli, crassitudine INTRORSUM
 *
 * [x, x + latitudo) x [y, y + altitudo); crassitudo pixela intra
 * marginem. Crassitudo >= dimidium lateris brevioris -> plenum;
 * crassitudo 0 -> nihil. Praecisum ad imaginem (x, y negativi licent).
 */
vacuum
imago_rectangulum (
    Imago* dest,
      s32  x,
      s32  y,
      i32  latitudo,
      i32  altitudo,
      i32  crassitudo,
       i8  r,
       i8  g,
       i8  b,
       i8  a);

#endif /* IMAGO_OPUS_H */
