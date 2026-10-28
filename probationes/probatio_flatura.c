/* probatio_flatura.c - Probationes pro DEFLATE/gzip bibliotheca
 */

#include "flatura.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * PROBATIONES - CRC-32
 * ======================================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
            i32  crc;
    constans i8* testum;

    (vacuum)c;

    /* Notum vector: "123456789" -> 0xCBF43926 */
    testum  = (constans i8*)"123456789";
    crc     = flatura_crc32(testum, IX);

    printf("  CRC-32 de \"123456789\": 0x%08X\n", crc);
    CREDO_AEQUALIS_I32(crc, (i32)0xCBF43926);

    printf("\n");
}


interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    i32 crc;

    (vacuum)c;

    crc = flatura_crc32((constans i8*)"", 0);
    printf("  CRC-32 de \"\": 0x%08X\n", crc);
    CREDO_AEQUALIS_I32(crc, 0);

    printf("\n");
}


interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    i32 crc;
    i32 crc_totalis;

    (vacuum)c;

    /* Calculare in partibus */
    crc = 0xFFFFFFFF;
    crc = flatura_crc32_continuare(crc, (constans i8*)"1234", IV);
    crc = flatura_crc32_continuare(crc, (constans i8*)"56789", V);
    crc = crc ^ 0xFFFFFFFF;

    /* Comparare cum totali */
    crc_totalis = flatura_crc32((constans i8*)"123456789", IX);

    printf("  Incrementalis: 0x%08X\n", crc);
    printf("  Totalis:       0x%08X\n", crc_totalis);
    CREDO_AEQUALIS_I32(crc, crc_totalis);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - DEFLATE INFLATIO
 * ======================================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
           Piscina* piscina;
    FlaturaFructus  compressa;
    FlaturaFructus  decompressa;
       constans i8* originalis;
               i32  mensura;

    piscina = c->piscina;

    originalis  = (constans i8*)"Hello, World!";
    mensura     = XIII;

    compressa = flatura_deflare(originalis, mensura,
                                FLATURA_COMPRESSIO_ORDINARIA, piscina);

    printf("  Originalis: %d bytes\n", mensura);
    printf("  Compressa:  %d bytes (status=%d)\n",
           compressa.mensura, compressa.status);

    CREDO_AEQUALIS_I32((i32)compressa.status, (i32)FLATURA_STATUS_OK);
    CREDO_NON_NIHIL(compressa.datum);

    decompressa = flatura_inflare(compressa.datum, compressa.mensura,
        piscina);

    printf("  Decompressa: %d bytes (status=%d)\n",
           decompressa.mensura, decompressa.status);

    CREDO_AEQUALIS_I32((i32)decompressa.status, (i32)FLATURA_STATUS_OK);
    CREDO_AEQUALIS_I32(decompressa.mensura, mensura);
    CREDO_VERUM(memcmp(decompressa.datum, originalis, (size_t)mensura)
        == 0);

    printf("\n");
}


interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
           Piscina* piscina;
    FlaturaFructus  compressa;
    FlaturaFructus  decompressa;
                i8* originalis;
               i32  mensura;
               i32  i;

    piscina = c->piscina;

    /* Generare datum repetitivum (bonus pro compressione) */
    mensura = IV * MXXIV;  /* 4096 */
    originalis = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura);

    per (i = 0; i < mensura; i++)
    {
        originalis[i] = (i8)('A' + (i % XXVI));
    }

    compressa = flatura_deflare(originalis, mensura,
                                FLATURA_COMPRESSIO_ORDINARIA, piscina);

    printf("  Originalis: %d bytes\n", mensura);
    printf("  Compressa:  %d bytes (ratio: %.1f%%)\n",
           compressa.mensura,
           (double)compressa.mensura * 100.0 / (double)mensura);

    CREDO_AEQUALIS_I32((i32)compressa.status, (i32)FLATURA_STATUS_OK);
    CREDO_NON_NIHIL(compressa.datum);
    CREDO_VERUM(compressa.mensura < mensura);  /* Debet esse minor */

    decompressa = flatura_inflare(compressa.datum, compressa.mensura,
        piscina);

    printf("  Decompressa: %d bytes\n", decompressa.mensura);

    CREDO_AEQUALIS_I32((i32)decompressa.status, (i32)FLATURA_STATUS_OK);
    CREDO_AEQUALIS_I32(decompressa.mensura, mensura);
    CREDO_VERUM(memcmp(decompressa.datum, originalis, (size_t)mensura)
        == 0);

    printf("\n");
}


interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
           Piscina* piscina;
    FlaturaFructus  compressa;
    FlaturaFructus  decompressa;
       constans i8* originalis;
               i32  mensura;

    piscina = c->piscina;

    originalis  = (constans i8*)"Testum";
    mensura     = VI;

    compressa = flatura_deflare(originalis, mensura,
                                FLATURA_COMPRESSIO_NULLA, piscina);

    printf("  Originalis: %d bytes\n", mensura);
    printf("  Compressa:  %d bytes\n", compressa.mensura);

    CREDO_AEQUALIS_I32((i32)compressa.status, (i32)FLATURA_STATUS_OK);

    decompressa = flatura_inflare(compressa.datum, compressa.mensura,
        piscina);

    CREDO_AEQUALIS_I32((i32)decompressa.status, (i32)FLATURA_STATUS_OK);
    CREDO_AEQUALIS_I32(decompressa.mensura, mensura);
    CREDO_VERUM(memcmp(decompressa.datum, originalis, (size_t)mensura)
        == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - GZIP
 * ======================================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
           Piscina* piscina;
    FlaturaFructus  compressa;
    FlaturaFructus  decompressa;
       constans i8* originalis;
               i32  mensura;

    piscina = c->piscina;

    originalis =
        (constans i8*)"Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
    mensura = LVII;

    compressa = flatura_gzip_deflare(originalis, mensura,
                                     FLATURA_COMPRESSIO_ORDINARIA,
                                     piscina);

    printf("  Originalis: %d bytes\n", mensura);
    printf("  Gzip:       %d bytes (status=%d)\n",
           compressa.mensura, compressa.status);

    CREDO_AEQUALIS_I32((i32)compressa.status, (i32)FLATURA_STATUS_OK);
    CREDO_NON_NIHIL(compressa.datum);

    /* Verificare magic number */
    CREDO_AEQUALIS_I32((i32)compressa.datum[0], 0x1F);
    CREDO_AEQUALIS_I32((i32)compressa.datum[I], 0x8B);

    decompressa = flatura_gzip_inflare(compressa.datum,
        compressa.mensura, piscina);

    printf("  Decompressa: %d bytes (status=%d)\n",
           decompressa.mensura, decompressa.status);

    CREDO_AEQUALIS_I32((i32)decompressa.status, (i32)FLATURA_STATUS_OK);
    CREDO_AEQUALIS_I32(decompressa.mensura, mensura);
    CREDO_VERUM(memcmp(decompressa.datum, originalis, (size_t)mensura)
        == 0);

    printf("\n");
}


interior vacuum
_sectio_nona (
    CredoContextus* c)
{
           Piscina* piscina;
    FlaturaFructus  compressa;
    FlaturaFructus  decompressa;
       constans i8* originalis;
               i32  mensura;

    piscina = c->piscina;

    originalis  = (constans i8*)"Test CRC";
    mensura     = VIII;

    compressa = flatura_gzip_deflare(originalis, mensura,
                                     FLATURA_COMPRESSIO_ORDINARIA,
                                     piscina);

    CREDO_AEQUALIS_I32((i32)compressa.status, (i32)FLATURA_STATUS_OK);

    /* Corrumpere CRC in trailer */
    compressa.datum[compressa.mensura - V] ^= 0xFF;

    decompressa = flatura_gzip_inflare(compressa.datum,
        compressa.mensura, piscina);

    printf("  Status post CRC corruptum: %d\n", decompressa.status);
    CREDO_AEQUALIS_I32((i32)decompressa.status,
        (i32)FLATURA_STATUS_GZIP_FRACTA);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - LIMITES
 * ======================================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
           Piscina* piscina;
    FlaturaFructus  compressa;
    FlaturaFructus  decompressa;

    piscina = c->piscina;

    compressa = flatura_deflare((constans i8*)"", 0,
                                FLATURA_COMPRESSIO_ORDINARIA, piscina);

    printf("  Compressa mensura: %d\n", compressa.mensura);
    CREDO_AEQUALIS_I32((i32)compressa.status, (i32)FLATURA_STATUS_OK);

    decompressa = flatura_inflare(compressa.datum, compressa.mensura,
        piscina);

    CREDO_AEQUALIS_I32((i32)decompressa.status, (i32)FLATURA_STATUS_OK);
    CREDO_AEQUALIS_I32(decompressa.mensura, 0);

    printf("\n");
}


interior vacuum
_sectio_decima (
    CredoContextus* c)
{
           Piscina* piscina;
    FlaturaFructus  fructus;

    piscina = c->piscina;

    fructus = flatura_deflare(NIHIL, X, FLATURA_COMPRESSIO_ORDINARIA,
        piscina);
    CREDO_AEQUALIS_I32((i32)fructus.status,
        (i32)FLATURA_STATUS_FRACTA_DATUM);

    fructus = flatura_deflare((constans i8*)"test", IV,
        FLATURA_COMPRESSIO_ORDINARIA, NIHIL);
    CREDO_AEQUALIS_I32((i32)fructus.status,
        (i32)FLATURA_STATUS_FRACTA_DATUM);

    fructus = flatura_inflare(NIHIL, X, piscina);
    CREDO_AEQUALIS_I32((i32)fructus.status,
        (i32)FLATURA_STATUS_FRACTA_DATUM);

    printf("  NIHIL argumenta recte recusata\n");

    printf("\n");
}


interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    constans character* nuntium;

    (vacuum)c;

    nuntium = flatura_status_nuntium(FLATURA_STATUS_OK);
    CREDO_NON_NIHIL(nuntium);
    printf("  OK: %s\n", nuntium);

    nuntium = flatura_status_nuntium(FLATURA_STATUS_FRACTA_DATUM);
    CREDO_NON_NIHIL(nuntium);
    printf("  FRACTA_DATUM: %s\n", nuntium);

    nuntium = flatura_status_nuntium(FLATURA_STATUS_GZIP_FRACTA);
    CREDO_NON_NIHIL(nuntium);
    printf("  GZIP_FRACTA: %s\n", nuntium);

    printf("\n");
}


interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    i32 max_mag;

    (vacuum)c;

    max_mag = flatura_maxima_magnitudo_compressa(M);
    printf("  Maxima pro 1000 bytes: %d\n", max_mag);
    CREDO_VERUM(max_mag >= M);

    max_mag = flatura_maxima_magnitudo_compressa(0);
    printf("  Maxima pro 0 bytes: %d\n", max_mag);
    CREDO_VERUM(max_mag >= XVIII);  /* Header + trailer */

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - COMPRESSIO EFFICACIA
 * ======================================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
           Piscina* piscina;
    FlaturaFructus  compressa;
                i8* originalis;
               i32  mensura;
               i32  i;

    piscina = c->piscina;

    /* Datum valde repetitivum - debet esse valde compressibile */
    mensura = IV * MXXIV;
    originalis = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura);

    per (i = 0; i < mensura; i++)
    {
        originalis[i] = 'A';  /* Omnes A */
    }

    compressa = flatura_deflare(originalis, mensura,
                                FLATURA_COMPRESSIO_ORDINARIA, piscina);

    printf("  Originalis: %d bytes (omnes 'A')\n", mensura);
    printf("  Compressa:  %d bytes (ratio: %.2f%%)\n",
           compressa.mensura,
           (double)compressa.mensura * 100.0 / (double)mensura);

    CREDO_AEQUALIS_I32((i32)compressa.status, (i32)FLATURA_STATUS_OK);
    /* Datum valde repetitivum debet habere bonam compressionem */
    CREDO_VERUM(compressa.mensura < mensura / X);  /* < 10% */

    printf("\n");
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

/* TRUNCATIO = INCOMPLETUS (2026-10-27): fluxus truncatus INCOMPLETUS
 * reddere debet, non fracturam datorum - lector lib/git fenestram
 * crescentem in sarcinis legit (LXIV K, deinde quadruplicata) et SOLUM
 * INCOMPLETUS eam crescere facit; olim lector in fine vacuus bits
 * nullos addebat et distantia "invalida" reddebatur, ergo obiectum
 * compressum > LXIV K in sarcina numquam legebatur. Fractura vera
 * (genus blocki III reservatum) fractura manet. */
interior vacuum
_sectio_fluxus_brevis (
    CredoContextus* c)
{
                               Piscina* piscina = c->piscina;
                                    i8* originalis;
                                   i32  mensura  = (i32)CC * (i32)MXXIV;
                                   i32  semen    = (i32)12345;
                                   i32  k;
                                   i32  n;
                        FlaturaFructus  compressa;
                        FlaturaFructus  fructus;
                                    i8  reservatum[IV];
    constans FlaturaCompressioNivellus  modi[II] = {
        FLATURA_COMPRESSIO_ORDINARIA, FLATURA_COMPRESSIO_NULLA };

    originalis = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura);
    CREDO_NECESSE_NON_NIHIL(originalis);
    /* verba pseudo-fortuita: compressio partialis, blocki multi */
    per (k = ZEPHYRUM; k < mensura; k++)
    {
        semen = semen * (i32)1103515245 + (i32)12345;
        originalis[k] = (i8)((semen >> XVI) % VII == ZEPHYRUM
            ? ' ' : 'a' + (character)((semen >> XVIII) % XXVI));
    }
    per (n = ZEPHYRUM; n < II; n++)
    {
        compressa = flatura_deflare(originalis, mensura, modi[n],
            piscina);
        CREDO_NECESSE((i32)compressa.status == (i32)FLATURA_STATUS_OK);
        fructus = flatura_inflare(compressa.datum, compressa.mensura,
            piscina);
        CREDO_AEQUALIS_I32((i32)fructus.status, (i32)FLATURA_STATUS_OK);
        CREDO_AEQUALIS_I32(fructus.mensura, mensura);
        per (k = I; k <= VIII; k++)
        {
            i32 longitudo = k < VIII ? compressa.mensura * k / VIII
                : compressa.mensura - I;

            fructus = flatura_inflare(compressa.datum, longitudo,
                piscina);
            si ((i32)fructus.status != (i32)FLATURA_STATUS_INCOMPLETUS)
            {
                printf("  nivellus %d, longitudo %d/%d: status %d\n",
                    (integer)modi[n], (integer)longitudo,
                    (integer)compressa.mensura,
                    (integer)fructus.status);
            }
            CREDO_AEQUALIS_I32((i32)fructus.status,
                (i32)FLATURA_STATUS_INCOMPLETUS);
        }
    }
    /* BFINAL=1, BTYPE=11 (reservatum): fractura, non INCOMPLETUS */
    reservatum[ZEPHYRUM]  = (i8)0x07;
    reservatum[I]         = (i8)0;
    reservatum[II]        = (i8)0;
    reservatum[III]       = (i8)0;
    fructus               = flatura_inflare(reservatum, IV, piscina);
    CREDO_VERUM((i32)fructus.status != (i32)FLATURA_STATUS_OK
        && (i32)fructus.status != (i32)FLATURA_STATUS_INCOMPLETUS);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "CRC-32 vector", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "CRC-32 vacuus", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "CRC-32 incrementalis", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "round-trip parvus", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "round-trip magnus", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stored block (sine compressione)", _sectio_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "datum vacuus", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "gzip round-trip", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "gzip CRC invalidus", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nullum argumenta", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "status nuntia", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "maxima magnitudo", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "compressio repetitiva", _sectio_decima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "truncatio = INCOMPLETUS", _sectio_fluxus_brevis, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("flatura", SECTIONES);
}
