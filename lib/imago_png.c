/*
 * imago_png.c - Codificatio PNG
 *
 * Structura plagulae:
 *   signum (VIII bytes)
 *   pars IHDR (XIII bytes dati)
 *   pars IDAT (fluxus zlib)
 *   pars IEND (sine dato)
 *
 * Pars quaeque: longitudo (IV, BE) + genus (IV) + datum +
 *               CRC-32 (IV, BE super GENUS ET DATUM).
 *
 * Fluxus zlib (RFC 1950): caput duorum bytorum + DEFLATE crudum +
 *                         Adler-32 (IV, BE super datum NON compressum).
 */

#include "imago_png.h"
#include "flatura.h"
#include <stdio.h>
#include <string.h>

/* Adler-32: modulus, et quot bytes ante reductionem tuto addi
 * possunt sine excessu XXXII bitorum (valor canonicus) */
#define ADLER_BASIS   65521
#define ADLER_MAXIMUM 5552

/* Caput zlib: CM=VIII (deflate), CINFO=VII (fenestra 32K); byte
 * alterum ita electum ut (primus * CCLVI + alter) per XXXI dividi
 * possit, ut RFC postulat */
#define ZLIB_CAPUT_PRIMUS CXX    /* 0x78 */
#define ZLIB_CAPUT_ALTER  CLVI   /* 0x9C */

/* \x89 P N G \r \n \x1a \n */
interior constans i8 SIGNUM_PNG[VIII] = {
    (i8)CXXXVII, (i8)LXXX, (i8)LXXVIII, (i8)LXXI,
    (i8)XIII,    (i8)X,    (i8)XXVI,    (i8)X
};


/* ============================================================
 * Auxilia interna
 * ============================================================ */

interior vacuum
_be32_scribere (
     i8* destinatio,
    i32  valor)
{
    destinatio[0]    = (i8)((valor >> XXIV) & CCLV);
    destinatio[I]    = (i8)((valor >> XVI) & CCLV);
    destinatio[II]   = (i8)((valor >> VIII) & CCLV);
    destinatio[III]  = (i8)( valor & CCLV);
}

/*
 * _adler32 - Summa Adler-32 (RFC 1950)
 *
 * Reductio omni ADLER_MAXIMO bytis fit: valor canonicus, ita
 * electus ut summae intra XXXII bita maneant.
 */
interior i32
_adler32 (
    constans i8* datum,
            i32  mensura)
{
    i32 a, b;
    i32 i;
    i32 numerata;

    a         = (i32)I;
    b         = ZEPHYRUM;
    numerata  = ZEPHYRUM;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        a += (i32)datum[i];
        b += a;

        numerata++;
        si (numerata == (i32)ADLER_MAXIMUM)
        {
            a         %= (i32)ADLER_BASIS;
            b         %= (i32)ADLER_BASIS;
            numerata  = ZEPHYRUM;
        }
    }

    a %= (i32)ADLER_BASIS;
    b %= (i32)ADLER_BASIS;

    redde (b << XVI) | a;
}

/*
 * _pars_scribere - Partem PNG in telam ponere
 *
 * Redde: sedes proxima post partem scriptam.
 */
interior i32
_pars_scribere (
                                i8* tela,
                               i32  sedes,
                constans character* genus,
                       constans i8* datum,
                               i32  mensura)
{
    i32 crc;

    _be32_scribere(tela + sedes, mensura);

    tela[sedes + IV]   = (i8)genus[0];
    tela[sedes + V]    = (i8)genus[I];
    tela[sedes + VI]   = (i8)genus[II];
    tela[sedes + VII]  = (i8)genus[III];

    si (mensura > ZEPHYRUM && datum != NIHIL)
    {
        memcpy(tela + sedes + VIII, datum, (size_t)mensura);
    }

    /* CRC super GENUS ET DATUM, non super longitudinem */
    crc = flatura_crc32(tela + sedes + IV, mensura + (i32)IV);
    _be32_scribere(tela + sedes + VIII + mensura, crc);

    redde sedes + (i32)XII + mensura;
}


/* ============================================================
 * Functiones Publicae
 * ============================================================ */

PngFructus
imago_png_codificare (
    constans Imago* imago,
           Piscina* piscina)
{
        PngFructus  fructus;
    FlaturaFructus  compressa;
               i32  versus_mensura;
               i32  crudum_mensura;
                i8* crudum;
               i32  y;
               i32  adler;
               i32  zlib_mensura;
                i8* zlib;
               i32  png_mensura;
                i8* tela;
               i32  sedes;
                i8  ihdr[XIII];

    fructus.successus      = FALSUM;
    fructus.datum          = NIHIL;
    fructus.mensura        = ZEPHYRUM;
    fructus.error.datum    = NIHIL;
    fructus.error.mensura  = ZEPHYRUM;

    /* Sine piscina ne nuntium quidem erroris fingi potest */
    si (piscina == NIHIL)
    {
        redde fructus;
    }

    si (imago == NIHIL || imago->pixela == NIHIL)
    {
        fructus.error = chorda_ex_literis("Imago abest", piscina);
        redde fructus;
    }

    si (imago->latitudo == ZEPHYRUM || imago->altitudo == ZEPHYRUM)
    {
        fructus.error = chorda_ex_literis("Imago vacua", piscina);
        redde fructus;
    }

    /* Versus quisque byte filtri praefixum habet */
    versus_mensura = (i32)I + imago->latitudo * (i32)IV;
    crudum_mensura = versus_mensura * imago->altitudo;

    crudum = (i8*)piscina_allocare(piscina, (memoriae_index)crudum_mensura);
    si (crudum == NIHIL)
    {
        fructus.error = chorda_ex_literis("Allocatio fracta (crudum)", piscina);
        redde fructus;
    }

    per (y = ZEPHYRUM; y < imago->altitudo; y++)
    {
        i8* versus;

        versus     = crudum + y * versus_mensura;
        versus[0]  = (i8)ZEPHYRUM;   /* filtrum NULLUM */

        memcpy(versus + I,
               imago->pixela + y * imago->latitudo * (i32)IV,
               (size_t)(imago->latitudo * (i32)IV));
    }

    /* Adler super datum NON compressum */
    adler = _adler32(crudum, crudum_mensura);

    compressa = flatura_deflare(crudum, crudum_mensura,
                                FLATURA_COMPRESSIO_ORDINARIA, piscina);
    si (compressa.status != FLATURA_STATUS_OK)
    {
        fructus.error = chorda_ex_literis(
                            flatura_status_nuntium(compressa.status), piscina);
        redde fructus;
    }

    zlib_mensura = (i32)II + compressa.mensura + (i32)IV;
    zlib = (i8*)piscina_allocare(piscina, (memoriae_index)zlib_mensura);
    si (zlib == NIHIL)
    {
        fructus.error = chorda_ex_literis("Allocatio fracta (zlib)", piscina);
        redde fructus;
    }

    zlib[0] = (i8)ZLIB_CAPUT_PRIMUS;
    zlib[I] = (i8)ZLIB_CAPUT_ALTER;
    memcpy(zlib + II, compressa.datum, (size_t)compressa.mensura);
    _be32_scribere(zlib + II + compressa.mensura, adler);

    _be32_scribere(ihdr,      imago->latitudo);
    _be32_scribere(ihdr + IV, imago->altitudo);
    ihdr[VIII]  = (i8)VIII;       /* profunditas bitorum */
    ihdr[IX]    = (i8)VI;         /* genus coloris: RGBA */
    ihdr[X]     = (i8)ZEPHYRUM;   /* modus compressionis */
    ihdr[XI]    = (i8)ZEPHYRUM;   /* modus filtrandi */
    ihdr[XII]   = (i8)ZEPHYRUM;   /* sine intertextura */

    png_mensura = (i32)VIII                      /* signum */
                + (i32)XII + (i32)XIII           /* IHDR */
                + (i32)XII + zlib_mensura        /* IDAT */
                + (i32)XII;                      /* IEND */

    tela = (i8*)piscina_allocare(piscina, (memoriae_index)png_mensura);
    si (tela == NIHIL)
    {
        fructus.error = chorda_ex_literis("Allocatio fracta (tela)", piscina);
        redde fructus;
    }

    memcpy(tela, SIGNUM_PNG, (size_t)VIII);
    sedes = (i32)VIII;

    sedes = _pars_scribere(tela, sedes, "IHDR", ihdr, (i32)XIII);
    sedes = _pars_scribere(tela, sedes, "IDAT", zlib, zlib_mensura);
    sedes = _pars_scribere(tela, sedes, "IEND", NIHIL, ZEPHYRUM);

    si (sedes != png_mensura)
    {
        fructus.error = chorda_ex_literis(
                            "Mensura computata cum scripta non congruit",
                            piscina);
        redde fructus;
    }

    fructus.successus  = VERUM;
    fructus.datum      = tela;
    fructus.mensura    = png_mensura;

    redde fructus;
}

PngFructus
imago_png_scribere (
                        constans Imago* imago,
                    constans character* via,
                               Piscina* piscina)
{
    PngFructus fructus;
    FILE*      plagula;
    size_t     scripta;

    fructus = imago_png_codificare(imago, piscina);
    si (!fructus.successus)
    {
        redde fructus;
    }

    si (via == NIHIL)
    {
        fructus.successus  = FALSUM;
        fructus.error      = chorda_ex_literis("Via abest", piscina);
        redde fructus;
    }

    plagula = fopen(via, "wb");
    si (plagula == NIHIL)
    {
        fructus.successus = FALSUM;
        fructus.error = chorda_ex_literis("Plagula aperiri non potest", piscina);
        redde fructus;
    }

    scripta = fwrite(fructus.datum, (size_t)I, (size_t)fructus.mensura, plagula);
    fclose(plagula);

    si (scripta != (size_t)fructus.mensura)
    {
        fructus.successus = FALSUM;
        fructus.error = chorda_ex_literis("Scriptio incompleta", piscina);
    }

    redde fructus;
}
