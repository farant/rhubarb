/* flatura.h - DEFLATE/gzip Compressio et Decompressio
 *
 * Bibliotheca pro DEFLATE (RFC 1951) et gzip (RFC 1952).
 * Nomen ex Latina "flare" (to blow) - inflare/deflare.
 *
 * USUS:
 *   FlaturaFructus res = flatura_gzip_inflare(compressa, mensura, piscina);
 *   FlaturaFructus res = flatura_gzip_deflare(originalis, mensura, nivellus, piscina);
 */

#ifndef FLATURA_H
#define FLATURA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"


/* ========================================================================
 * CONSTANTIAE
 * ======================================================================== */

#define FLATURA_FENESTRA_MAGNITUDO  32768   /* 32K sliding window */
#define FLATURA_MAXIMA_LONGITUDO    CCLVIII /* 258 max match length */


/* ========================================================================
 * TYPI - STATUS
 * ======================================================================== */

nomen enumeratio {
    FLATURA_STATUS_OK = 0,
    FLATURA_STATUS_FRACTA_DATUM,        /* Datum invalidum */
    FLATURA_STATUS_FRACTA_HUFFMAN,      /* Arbor Huffman invalida */
    FLATURA_STATUS_FRACTA_DISTANTIA,    /* Distantia invalida */
    FLATURA_STATUS_FRACTA_ALLOCATIO,    /* Allocatio fallita */
    FLATURA_STATUS_INCOMPLETUS,         /* Plus datum necessarium */
    FLATURA_STATUS_GZIP_FRACTA          /* Gzip caput/cauda invalida */
} FlaturaStatus;


/* ========================================================================
 * TYPI - COMPRESSIO NIVELLUS
 * ======================================================================== */

nomen enumeratio {
    FLATURA_COMPRESSIO_NULLA = 0,       /* Sine compressione (stored) */
    FLATURA_COMPRESSIO_RAPIDA = 1,      /* Velociter, minus compressio */
    FLATURA_COMPRESSIO_ORDINARIA = 6,   /* Default */
    FLATURA_COMPRESSIO_OPTIMA = 9       /* Optima compressio, lentior */
} FlaturaCompressioNivellus;


/* ========================================================================
 * TYPI - FRUCTUS
 * ======================================================================== */

/* Fructus operationis */
nomen structura {
    FlaturaStatus status;
    i8*           datum;      /* Output (allocatum ex piscina) */
    i32           mensura;    /* Longitudo output */
} FlaturaFructus;


/* ========================================================================
 * FUNCTIONES - INFLATIO (Decompressio)
 * ======================================================================== */

/* Inflare DEFLATE datum (raw, sine wrapper)
 *
 * datum:    Datum compressum
 * mensura:  Longitudo datorum
 * piscina:  Arena pro allocatio
 *
 * Redde: FlaturaFructus cum data decompressa
 */
FlaturaFructus
flatura_inflare(
    constans i8* datum,
    i32          mensura,
    Piscina*     piscina);


/* Inflare gzip datum (cum capite et cauda)
 *
 * datum:    Datum gzip
 * mensura:  Longitudo datorum
 * piscina:  Arena pro allocatio
 *
 * Redde: FlaturaFructus cum data decompressa
 */
FlaturaFructus
flatura_gzip_inflare(
    constans i8* datum,
    i32          mensura,
    Piscina*     piscina);


/* ========================================================================
 * FUNCTIONES - DEFLATIO (Compressio)
 * ======================================================================== */

/* Deflare datum (raw DEFLATE)
 *
 * datum:    Datum originale
 * mensura:  Longitudo datorum
 * nivellus: Nivellus compressionis
 * piscina:  Arena pro allocatio
 *
 * Redde: FlaturaFructus cum data compressa
 */
FlaturaFructus
flatura_deflare(
    constans i8*              datum,
    i32                       mensura,
    FlaturaCompressioNivellus nivellus,
    Piscina*                  piscina);


/* Deflare datum cum gzip wrapper
 *
 * datum:    Datum originale
 * mensura:  Longitudo datorum
 * nivellus: Nivellus compressionis
 * piscina:  Arena pro allocatio
 *
 * Redde: FlaturaFructus cum data gzip compressa
 */
FlaturaFructus
flatura_gzip_deflare(
    constans i8*              datum,
    i32                       mensura,
    FlaturaCompressioNivellus nivellus,
    Piscina*                  piscina);


/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */

/* Calculare CRC-32 (IEEE 802.3 polynom)
 *
 * datum:   Data pro CRC
 * mensura: Longitudo datorum
 *
 * Redde: CRC-32 valor
 */
i32
flatura_crc32(
    constans i8* datum,
    i32          mensura);


/* Calculare CRC-32 incrementaliter
 *
 * crc:     CRC praecedens (0xFFFFFFFF pro initio)
 * datum:   Data addenda
 * mensura: Longitudo datorum
 *
 * Redde: CRC-32 valor (XOR cum 0xFFFFFFFF pro finali)
 */
i32
flatura_crc32_continuare(
    i32          crc,
    constans i8* datum,
    i32          mensura);


/* Aestimare maximam magnitudinem compressam
 *
 * mensura_originalis: Longitudo datorum originalium
 *
 * Redde: Maxima magnitudo possibilis post compressionem
 */
i32
flatura_maxima_magnitudo_compressa(i32 mensura_originalis);


/* Obtinere nuntium erroris
 *
 * status: Status erroris
 *
 * Redde: Descriptio erroris (Latine)
 */
constans character*
flatura_status_nuntium(FlaturaStatus status);


#endif /* FLATURA_H */
