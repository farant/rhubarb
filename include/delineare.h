#ifndef DELINEARE_H
#define DELINEARE_H

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "color.h"

/* ==================================================
 * DELINEARE - Functiones Graphicae
 *
 * Bibliotheca pro delineando formas graphicas in
 * tabula pixelorum. Sustinet lineas, circulos,
 * rectangulos, triangulos, ellipses, et polygonos.
 *
 * PHILOSOPHIA:
 * - API simplex pro formis graphicis
 * - Sustinet praecisionem (clipping)
 * - Modi pingendi (solidus, exemplar, XOR)
 * - Exemplaria classica Mac
 *
 * EXEMPLUM:
 *   Piscina* piscina = piscina_generare_dynamicum("app", M * M);
 *   Fenestra* fenestra = fenestra_creare(piscina, &config);
 *   TabulaPixelorum* tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CDLXXX);
 *   ContextusDelineandi* ctx = delineare_creare_contextum(piscina, tabula);
 *
 *   dum (!fenestra_debet_claudere(fenestra)) {
 *       delineare_vacare(ctx, RGB(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM));
 *       delineare_circulum(ctx, C, C, L, RGB(CCLV, ZEPHYRUM, ZEPHYRUM));
 *       delineare_rectangulum_plenum(ctx, CC, CC, C, L, RGB(ZEPHYRUM, CCLV, ZEPHYRUM));
 *       fenestra_praesentare_pixela(fenestra, tabula);
 *   }
 *
 *   fenestra_destruere(fenestra);
 *   piscina_destruere(piscina);
 *
 * ================================================== */


/* ==================================================
 * Constantae - Modi Delineandi
 * ================================================== */

/* Modi delineandi */
nomen enumeratio {
    MODUS_SOLIDUS = ZEPHYRUM,  /* Color solidus */
    MODUS_EXEMPLAR,             /* Exemplar (pattern fill) */
    MODUS_XOR                   /* Modus XOR */
} modus_delineandi_t;


/* ==================================================
 * Typi - Contextus Delineandi
 * ================================================== */

/* Contextus delineandi pro gestione status */
nomen structura {
    TabulaPixelorum* tabula;
    Piscina* piscina;

    /* Rectangulum praecisionis */
    i32 praecisio_x;
    i32 praecisio_y;
    i32 praecisio_latitudo;
    i32 praecisio_altitudo;

    /* Exemplar currens (bitmap 8x8) */
    i8 exemplar[VIII];

    /* Modus delineandi currens */
    modus_delineandi_t modus;

    /* Offset exemplaris pro tegendo */
    i32 exemplar_offset_x;
    i32 exemplar_offset_y;
} ContextusDelineandi;


/* ==================================================
 * Gestio Contextus
 * ================================================== */

/* Creare contextum delineandi
 *
 * piscina: piscina pro allocando memoriam
 * tabula: tabula pixelorum
 *
 * Reddit: contextum novum vel NIHIL si error
 */
ContextusDelineandi*
delineare_creare_contextum (
    Piscina*         piscina,
    TabulaPixelorum* tabula);

/* Restituere contextum ad statum initialem
 *
 * ctx: contextus delineandi
 */
vacuum
delineare_restituere_contextum (
    ContextusDelineandi* ctx);


/* ==================================================
 * Modus Delineandi
 * ================================================== */

/* Ponere modum delineandi
 *
 * ctx: contextus delineandi
 * modus: modus novus
 */
vacuum
delineare_ponere_modum (
    ContextusDelineandi* ctx,
    modus_delineandi_t   modus);


/* ==================================================
 * Gestio Exemplaris
 * ================================================== */

/* Ponere exemplar
 *
 * ctx: contextus delineandi
 * exemplar: exemplar novum (8 bytes)
 */
vacuum
delineare_ponere_exemplar (
    ContextusDelineandi* ctx,
    constans i8          exemplar[VIII]);

/* Ponere exemplar internum
 *
 * ctx: contextus delineandi
 * exemplar_id: id exemplaris interni
 */
vacuum
delineare_ponere_exemplar_internum (
    ContextusDelineandi* ctx,
    i32                  exemplar_id);

/* Ponere offset exemplaris
 *
 * ctx: contextus delineandi
 * x: offset x
 * y: offset y
 */
vacuum
delineare_ponere_offset_exemplaris (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y);


/* ==================================================
 * Praecisio (Clipping)
 * ================================================== */

/* Ponere rectangulum praecisionis
 *
 * ctx: contextus delineandi
 * x: x rectanguli
 * y: y rectanguli
 * latitudo: latitudo rectanguli
 * altitudo: altitudo rectanguli
 */
vacuum
delineare_ponere_praecisionem (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo);

/* Tollere praecisionem
 *
 * ctx: contextus delineandi
 */
vacuum
delineare_tollere_praecisionem (
    ContextusDelineandi* ctx);


/* ==================================================
 * Functiones Primitivae - Pixelum
 * ================================================== */

/* Delineare pixelum singulare
 *
 * ctx: contextus delineandi
 * x: coordinata x
 * y: coordinata y
 * color: color
 */
vacuum
delineare_pixelum (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    Color                color);


/* ==================================================
 * Functiones Primitivae - Lineae
 * ================================================== */

/* Delineare lineam
 *
 * ctx: contextus delineandi
 * x0: coordinata x initialis
 * y0: coordinata y initialis
 * x1: coordinata x finalis
 * y1: coordinata y finalis
 * color: color
 */
vacuum
delineare_lineam (
    ContextusDelineandi* ctx,
    i32                  x0,
    i32                  y0,
    i32                  x1,
    i32                  y1,
    Color                color);

/* Delineare lineam horizontalem
 *
 * ctx: contextus delineandi
 * x0: coordinata x initialis
 * x1: coordinata x finalis
 * y: coordinata y
 * color: color
 */
vacuum
delineare_lineam_horizontalem (
    ContextusDelineandi* ctx,
    i32                  x0,
    i32                  x1,
    i32                  y,
    Color                color);

/* Delineare lineam verticalem
 *
 * ctx: contextus delineandi
 * x: coordinata x
 * y0: coordinata y initialis
 * y1: coordinata y finalis
 * color: color
 */
vacuum
delineare_lineam_verticalem (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y0,
    i32                  y1,
    Color                color);


/* ==================================================
 * Functiones Primitivae - Rectangula
 * ================================================== */

/* Delineare rectangulum
 *
 * ctx: contextus delineandi
 * x: coordinata x
 * y: coordinata y
 * latitudo: latitudo rectanguli
 * altitudo: altitudo rectanguli
 * color: color
 */
vacuum
delineare_rectangulum (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo,
    Color                color);

/* Delineare rectangulum plenum
 *
 * ctx: contextus delineandi
 * x: coordinata x
 * y: coordinata y
 * latitudo: latitudo rectanguli
 * altitudo: altitudo rectanguli
 * color: color
 */
vacuum
delineare_rectangulum_plenum (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo,
    Color                color);


/* ==================================================
 * Functiones Primitivae - Circuli
 * ================================================== */

/* Delineare circulum
 *
 * ctx: contextus delineandi
 * centrum_x: coordinata x centri
 * centrum_y: coordinata y centri
 * radius: radius circuli
 * color: color
 */
vacuum
delineare_circulum (
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius,
    Color                color);

/* Delineare circulum plenum
 *
 * ctx: contextus delineandi
 * centrum_x: coordinata x centri
 * centrum_y: coordinata y centri
 * radius: radius circuli
 * color: color
 */
vacuum
delineare_circulum_plenum (
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius,
    Color                color);


/* ==================================================
 * Functiones Primitivae - Triangula
 * ================================================== */

/* Delineare triangulum
 *
 * ctx: contextus delineandi
 * x0: coordinata x vertice 0
 * y0: coordinata y vertice 0
 * x1: coordinata x vertice 1
 * y1: coordinata y vertice 1
 * x2: coordinata x vertice 2
 * y2: coordinata y vertice 2
 * color: color
 */
vacuum
delineare_triangulum (
    ContextusDelineandi* ctx,
    i32                  x0,
    i32                  y0,
    i32                  x1,
    i32                  y1,
    i32                  x2,
    i32                  y2,
    Color                color);

/* Delineare triangulum plenum
 *
 * ctx: contextus delineandi
 * x0: coordinata x vertice 0
 * y0: coordinata y vertice 0
 * x1: coordinata x vertice 1
 * y1: coordinata y vertice 1
 * x2: coordinata x vertice 2
 * y2: coordinata y vertice 2
 * color: color
 */
vacuum
delineare_triangulum_plenum (
    ContextusDelineandi* ctx,
    i32                  x0,
    i32                  y0,
    i32                  x1,
    i32                  y1,
    i32                  x2,
    i32                  y2,
    Color                color);


/* ==================================================
 * Functiones Primitivae - Ellipses
 * ================================================== */

/* Delineare ellipsin
 *
 * ctx: contextus delineandi
 * centrum_x: coordinata x centri
 * centrum_y: coordinata y centri
 * radius_x: radius horizontalis
 * radius_y: radius verticalis
 * color: color
 */
vacuum
delineare_ellipsin (
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius_x,
    i32                  radius_y,
    Color                color);

/* Delineare ellipsin plenam
 *
 * ctx: contextus delineandi
 * centrum_x: coordinata x centri
 * centrum_y: coordinata y centri
 * radius_x: radius horizontalis
 * radius_y: radius verticalis
 * color: color
 */
vacuum
delineare_ellipsin_plenam (
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius_x,
    i32                  radius_y,
    Color                color);


/* ==================================================
 * Functiones Primitivae - Polygona
 * ================================================== */

/* Delineare polygonum
 *
 * ctx: contextus delineandi
 * puncta: tabula punctorum (x,y paria)
 * numerus_punctorum: numerus punctorum
 * color: color
 */
vacuum
delineare_polygonum (
    ContextusDelineandi* ctx,
    constans i32*        puncta,
    i32                  numerus_punctorum,
    Color                color);

/* Delineare polygonum plenum
 *
 * ctx: contextus delineandi
 * puncta: tabula punctorum (x,y paria)
 * numerus_punctorum: numerus punctorum
 * color: color
 */
vacuum
delineare_polygonum_plenum (
    ContextusDelineandi* ctx,
    constans i32*        puncta,
    i32                  numerus_punctorum,
    Color                color);


/* ==================================================
 * Functiones Utilitatis
 * ================================================== */

/* Vacare tabulam (implere cum colore)
 *
 * ctx: contextus delineandi
 * color: color
 */
vacuum
delineare_vacare (
    ContextusDelineandi* ctx,
    Color                color);

/* Delineare cratem (grid)
 *
 * ctx: contextus delineandi
 * spatium: spatium inter lineas
 * color: color
 */
vacuum
delineare_cratem (
    ContextusDelineandi* ctx,
    i32                  spatium,
    Color                color);


/* ==================================================
 * Functiones Primitivae - Gradientia Dithered
 * ================================================== */

/* Algorithmi dithering */
nomen enumeratio {
    DITHERING_FLOYD_STEINBERG = ZEPHYRUM,
    DITHERING_ATKINSON,
    DITHERING_BAYER_4X4,
    DITHERING_BAYER_8X8
} AlgorithusDithering;

/* Delineare gradientum linearem cum Floyd-Steinberg dithering (backwards compatibility)
 *
 * ctx: contextus delineandi
 * x: coordinata x originis
 * y: coordinata y originis
 * latitudo: latitudo rectanguli
 * altitudo: altitudo rectanguli
 * color_initium: color initialis
 * color_finis: color finalis
 * horizontalis: VERUM pro gradiente horizontali, FALSUM pro verticali
 */
vacuum
delineare_gradientum_linearem_dithered(
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo,
    Color                color_initium,
    Color                color_finis,
    b32                  horizontalis);

/* Delineare gradientum radialem cum Floyd-Steinberg dithering (backwards compatibility)
 *
 * ctx: contextus delineandi
 * centrum_x: coordinata x centri
 * centrum_y: coordinata y centri
 * radius: radius gradienti
 * color_centrum: color centri
 * color_peripheria: color peripheriae
 */
vacuum
delineare_gradientum_radialem_dithered(
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius,
    Color                color_centrum,
    Color                color_peripheria);

/* Delineare gradientum linearem cum dithering algorithmum et palette
 *
 * ctx: contextus delineandi
 * x: coordinata x originis
 * y: coordinata y originis
 * latitudo: latitudo rectanguli
 * altitudo: altitudo rectanguli
 * color_initium: color initialis
 * color_finis: color finalis
 * horizontalis: VERUM pro gradiente horizontali, FALSUM pro verticali
 * algorithmus: algorithmus dithering (Floyd-Steinberg, Bayer, etc)
 * palette: array colorum palette, vel NIHIL pro dithering binario
 * numerus_colorum: numerus colorum in palette (ignoratur si palette == NIHIL)
 */
vacuum
delineare_gradientum_linearem_dithered_cum_palette(
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo,
    Color                color_initium,
    Color                color_finis,
    b32                  horizontalis,
    AlgorithusDithering  algorithmus,
    constans Color*      palette,
    i32                  numerus_colorum);

/* Delineare gradientum radialem cum dithering algorithmum et palette
 *
 * ctx: contextus delineandi
 * centrum_x: coordinata x centri
 * centrum_y: coordinata y centri
 * radius: radius gradienti
 * color_centrum: color centri
 * color_peripheria: color peripheriae
 * algorithmus: algorithmus dithering (Floyd-Steinberg, Bayer, etc)
 * palette: array colorum palette, vel NIHIL pro dithering binario
 * numerus_colorum: numerus colorum in palette (ignoratur si palette == NIHIL)
 */
vacuum
delineare_gradientum_radialem_dithered_cum_palette(
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius,
    Color                color_centrum,
    Color                color_peripheria,
    AlgorithusDithering  algorithmus,
    constans Color*      palette,
    i32                  numerus_colorum);


/* ==================================================
 * Constantae - Exemplaria Interna
 * ================================================== */

/* Identificationes exemplarium internorum (exemplaria classica Mac) */
nomen enumeratio {
    EXEMPLAR_SOLIDUS = ZEPHYRUM,      /* Exemplar 00 */
    EXEMPLAR_PUNCTA_DISPERSA,          /* Exemplar 01 */
    EXEMPLAR_PUNCTA_REGULARIA,         /* Exemplar 02 */
    EXEMPLAR_TESSELLATUM,              /* Exemplar 03 - checkerboard */
    EXEMPLAR_LINEOLAE_HORIZONTALES,    /* Exemplar 04 */
    EXEMPLAR_LINEAE_VERTICALES,        /* Exemplar 05 */
    EXEMPLAR_DIAGONALIS,               /* Exemplar 06 */
    EXEMPLAR_VERTICALIS_DISPERSUS,     /* Exemplar 07 */
    EXEMPLAR_DISPERSUS,                /* Exemplar 08 */
    EXEMPLAR_PUNCTA_DIAGONALIA,        /* Exemplar 09 */
    EXEMPLAR_LATERICIUM,               /* Exemplar 10 - brick */
    EXEMPLAR_LATERICIUM_OFFSET,        /* Exemplar 11 */
    EXEMPLAR_CINEREUM_LEVE,            /* Exemplar 12 */
    EXEMPLAR_DIAGONALIS_DISPERSUS,     /* Exemplar 13 */
    EXEMPLAR_RHOMBUS_DISPERSUS,        /* Exemplar 14 */
    EXEMPLAR_TEXTUM,                   /* Exemplar 15 - weave */
    EXEMPLAR_MIXTUS,                   /* Exemplar 16 */
    EXEMPLAR_ANGULUS,                  /* Exemplar 17 */
    EXEMPLAR_LATERICIUM_COMPLEXUS,     /* Exemplar 18 */
    EXEMPLAR_ALBUS,                    /* Exemplar 19 */
    EXEMPLAR_RARISSIMUS,               /* Exemplar 20 */
    EXEMPLAR_PUNCTA_DUPLICIA_DISPERSA, /* Exemplar 21 */
    EXEMPLAR_PUNCTA_DUPLICIA,          /* Exemplar 22 */
    EXEMPLAR_HORIZONTALIS_DISPERSUS,   /* Exemplar 23 */
    EXEMPLAR_LINEAE_HORIZONTALES,      /* Exemplar 24 */
    EXEMPLAR_DIAGONALIS_ANTERIOR,      /* Exemplar 25 */
    EXEMPLAR_HORIZONTALIS_LATUS,       /* Exemplar 26 */
    EXEMPLAR_DIAGONALIS_POSTERIOR,     /* Exemplar 27 */
    EXEMPLAR_DISPERSUS_MIXTUS,         /* Exemplar 28 */
    EXEMPLAR_MARGO_SINISTER,           /* Exemplar 29 */
    EXEMPLAR_ARCUS,                    /* Exemplar 30 */
    EXEMPLAR_LINEA_RHOMBUS,            /* Exemplar 31 */
    EXEMPLAR_ANGULI_PARVI,             /* Exemplar 32 */
    EXEMPLAR_DIAGONALIS_UNDULATUS,     /* Exemplar 33 */
    EXEMPLAR_CRUX,                     /* Exemplar 34 */
    EXEMPLAR_MIXTUS_GRAVIS,            /* Exemplar 35 */
    EXEMPLAR_CAPSAE_COMPLEXAE,         /* Exemplar 36 */
    EXEMPLAR_RHOMBUS_LINEATUS,         /* Exemplar 37 */
    EXEMPLAR_NUMERUS
} exemplar_internum_t;

#endif /* DELINEARE_H */
