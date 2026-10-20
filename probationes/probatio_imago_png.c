/*
 * probatio_imago_png.c - Probationes codificationis PNG
 *
 * ORACULUM I: stb_image. Decodificator est quem NOS non scripsimus,
 * ergo si imago nostra per eum redit incolumis, duo implementa
 * independentia de PIXELIS ET STRUCTURA consentiunt.
 *
 * SED stb_image SUMMAS NON PROBAT (mensuratum: Adler corruptus
 * omnes probationes circuitus virides reliquit). Ideo ORACULUM II
 * adest - valores ex Python zlib computati et infixi (sectio IX).
 * Circuitus solus probaret "stb legere potest", quod minus est
 * quam "plagula recta est".
 *
 * Praeterea imago_conferre adhibetur - bibliotheca hesterna
 * bibliothecam hodiernam iudicat.
 */

#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "imago.h"
#include "imago_png.h"
#include "imago_collatio.h"
#include <stdio.h>
#include <string.h>


/* ============================================================
 * Auxilia
 * ============================================================ */

interior Imago
_imago_ficta (
    Piscina* piscina,
        i32  lat,
        i32  alt,
        b32  alpha_varia)
{
    Imago im;
    i32   x, y;

    im.latitudo = lat;
    im.altitudo = alt;
    im.pixela   = (i8*)piscina_allocare(
                      piscina, (memoriae_index)(lat * alt * (i32)IV));

    si (im.pixela != NIHIL)
    {
        per (y = ZEPHYRUM; y < alt; y++)
        {
            per (x = ZEPHYRUM; x < lat; x++)
            {
                i32 sedes;

                sedes = (y * lat + x) * (i32)IV;
                im.pixela[sedes + 0] = (i8)((x * (i32)VII
                    + y * (i32)XI) & CCLV);
                im.pixela[sedes + I] = (i8)((x * (i32)V
                    + y * (i32)III) & CCLV);
                im.pixela[sedes + II] = (i8)((x * y) & CCLV);

                si (alpha_varia)
                {
                    im.pixela[sedes + III] = (i8)((x * (i32)XVI
                        + y) & CCLV);
                }
                alioquin
                {
                    im.pixela[sedes + III] = (i8)CCLV;
                }
            }
        }
    }

    redde im;
}

interior i32
_be32_legere (
    constans i8* d)
{
    redde ((i32)d[0] << XXIV) | ((i32)d[I] << XVI)
         | ((i32)d[II] << VIII) | (i32)d[III];
}

/* Imaginem codificare, per stb_image reducere, et cum fonte conferre */
interior CollatioFructus
_circuitus (
    constans Imago* fons,
           Piscina* piscina,
        PngFructus* png_exitus)
{
         PngFructus png;
       ImagoFructus reductus;
     CollatioRegula regula;
    CollatioFructus nihil_fructus;

    /* Fructum invalidum ex bibliotheca ipsa sumimus - ita omnis
     * campus impletur, etiam si campus novus additur */
    nihil_fructus = imago_conferre(NIHIL, NIHIL,
                                   collatio_regula_solita(), NIHIL);

    png = imago_png_codificare(fons, piscina);
    si (png_exitus != NIHIL)
    {
        *png_exitus = png;
    }
    si (!png.successus)
    {
        redde nihil_fructus;
    }

    reductus = imago_caricare_ex_memoria(png.datum, png.mensura,
        piscina);
    si (!reductus.successus)
    {
        redde nihil_fructus;
    }

    regula                    = collatio_regula_solita();
    regula.tolerantia         = ZEPHYRUM;
    regula.sperne_lenimentum  = FALSUM;

    redde imago_conferre(fons, &reductus.imago, regula, NIHIL);
}


/* ============================================================ */


    /* ========================================================
     * I. Signum et IHDR
     * ======================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
             Imago fons;
        PngFructus png;

        fons  = _imago_ficta(piscina, VII, V, FALSUM);
        png   = imago_png_codificare(&fons, piscina);

        CREDO_VERUM(png.successus);
        CREDO_NON_NIHIL(png.datum);

        /* \x89 P N G \r \n \x1a \n */
        CREDO_AEQUALIS_I32((i32)png.datum[0],   (i32)CXXXVII);
        CREDO_AEQUALIS_I32((i32)png.datum[I],   (i32)LXXX);
        CREDO_AEQUALIS_I32((i32)png.datum[II],  (i32)LXXVIII);
        CREDO_AEQUALIS_I32((i32)png.datum[III], (i32)LXXI);
        CREDO_AEQUALIS_I32((i32)png.datum[IV],  (i32)XIII);
        CREDO_AEQUALIS_I32((i32)png.datum[V],   (i32)X);
        CREDO_AEQUALIS_I32((i32)png.datum[VI],  (i32)XXVI);
        CREDO_AEQUALIS_I32((i32)png.datum[VII], (i32)X);

        /* Pars prima: longitudo XIII, genus IHDR */
        CREDO_AEQUALIS_I32(_be32_legere(png.datum + VIII), (i32)XIII);
        CREDO_AEQUALIS_I32((i32)png.datum[XII],  (i32)LXXIII);  /* I */
        CREDO_AEQUALIS_I32((i32)png.datum[XIII], (i32)LXXII);   /* H */

        /* Campi IHDR: latitudo, altitudo, profunditas, genus coloris */
        CREDO_AEQUALIS_I32(_be32_legere(png.datum + XVI), (i32)VII);
        CREDO_AEQUALIS_I32(_be32_legere(png.datum + XX),  (i32)V);
        CREDO_AEQUALIS_I32((i32)png.datum[XXIV], (i32)VIII); /* VIII bita */
        CREDO_AEQUALIS_I32((i32)png.datum[XXV],  (i32)VI);   /* RGBA */
        CREDO_AEQUALIS_I32((i32)png.datum[XXVI], ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)png.datum[XXVII], ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)png.datum[XXVIII], ZEPHYRUM);
    }
}


    /* ========================================================
     * II. Circuitus per stb_image (oraculum alienum)
     * ======================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                  Imago fons;
        CollatioFructus collatio;

        fons      = _imago_ficta(piscina, VII, V, FALSUM);
        collatio  = _circuitus(&fons, piscina, NIHIL);

        CREDO_AEQUALIS_S32((s32)collatio.sententia,
            (s32)COLLATIO_CONGRUUNT);
        CREDO_AEQUALIS_I32(collatio.pixela_diversa, ZEPHYRUM);
        CREDO_AEQUALIS_I32(collatio.delta_maximum, ZEPHYRUM);
        CREDO_AEQUALIS_I32(collatio.pixela_tota, (i32)(VII * V));
    }
}


    /* ========================================================
     * III. Imago minima
     * ======================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                  Imago fons;
        CollatioFructus collatio;

        fons      = _imago_ficta(piscina, I, I, FALSUM);
        collatio  = _circuitus(&fons, piscina, NIHIL);

        CREDO_AEQUALIS_S32((s32)collatio.sententia,
            (s32)COLLATIO_CONGRUUNT);
        CREDO_AEQUALIS_I32(collatio.pixela_tota, (i32)I);
    }
}


    /* ========================================================
     * IV. Alpha servatur (PNG non praemultiplicat)
     * ======================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                  Imago fons;
           ImagoFructus reductus;
             PngFructus png;
        CollatioFructus collatio;

        fons      = _imago_ficta(piscina, XVI, XVI, VERUM);
        collatio  = _circuitus(&fons, piscina, &png);

        CREDO_VERUM(png.successus);
        CREDO_AEQUALIS_S32((s32)collatio.sententia,
            (s32)COLLATIO_CONGRUUNT);
        CREDO_AEQUALIS_I32(collatio.delta_maximum, ZEPHYRUM);

        /* Canalem alpha directe inspicere - collatio sola nos
         * fallere posset si utraque pars eodem modo erraret */
        reductus = imago_caricare_ex_memoria(png.datum, png.mensura,
            piscina);
        CREDO_VERUM(reductus.successus);
        CREDO_AEQUALIS_I32(
            (i32)reductus.imago.pixela[III],
            (i32)fons.pixela[III]);
        /* pixelum (0,0): alpha = ZEPHYRUM ex formula */
        CREDO_AEQUALIS_I32((i32)fons.pixela[III], ZEPHYRUM);
    }
}


    /* ========================================================
     * V. Imago maior
     * ======================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                  Imago fons;
        CollatioFructus collatio;

        fons      = _imago_ficta(piscina, LXIV, XLVIII, FALSUM);
        collatio  = _circuitus(&fons, piscina, NIHIL);

        CREDO_AEQUALIS_S32((s32)collatio.sententia,
            (s32)COLLATIO_CONGRUUNT);
        CREDO_AEQUALIS_I32(collatio.pixela_diversa, ZEPHYRUM);
        CREDO_AEQUALIS_I32(collatio.pixela_tota, (i32)(LXIV * XLVIII));
    }
}


    /* ========================================================
     * VI. Emissio DETERMINATA
     * ======================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Imago fons;
        PngFructus prima, altera;

        fons    = _imago_ficta(piscina, XVI, XVI, FALSUM);
        prima   = imago_png_codificare(&fons, piscina);
        altera  = imago_png_codificare(&fons, piscina);

        CREDO_VERUM(prima.successus);
        CREDO_VERUM(altera.successus);
        CREDO_AEQUALIS_I32(prima.mensura, altera.mensura);
        CREDO_AEQUALIS_S32(
            (s32)memcmp(prima.datum, altera.datum,
            (size_t)prima.mensura),
            (s32)ZEPHYRUM);
    }
}


    /* ========================================================
     * VII. Culpae NOMINANTUR
     * ======================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        Imago      fons, vacua;
        PngFructus png;

        fons = _imago_ficta(piscina, IV, IV, FALSUM);

        png = imago_png_codificare(NIHIL, piscina);
        CREDO_FALSUM(png.successus);
        CREDO_CHORDA_NON_VACUA(png.error);

        vacua.pixela    = NIHIL;
        vacua.latitudo  = IV;
        vacua.altitudo  = IV;
        png             = imago_png_codificare(&vacua, piscina);
        CREDO_FALSUM(png.successus);
        CREDO_CHORDA_NON_VACUA(png.error);

        vacua.pixela    = fons.pixela;
        vacua.latitudo  = ZEPHYRUM;
        vacua.altitudo  = IV;
        png             = imago_png_codificare(&vacua, piscina);
        CREDO_FALSUM(png.successus);
        CREDO_CHORDA_NON_VACUA(png.error);

        /* Sine piscina nuntius fingi non potest, sed tacere licet */
        png = imago_png_codificare(&fons, NIHIL);
        CREDO_FALSUM(png.successus);
    }
}


    /* ========================================================
     * VIII. Scriptio in plagulam et lectio
     * ======================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                     Imago  fons;
                PngFructus  png;
              ImagoFructus  reductus;
            CollatioRegula  regula;
           CollatioFructus  collatio;
        constans character* via = "/tmp/probatio_imago_png.png";

        fons = _imago_ficta(piscina, XXXII, XVI, FALSUM);

        png = imago_png_scribere(&fons, via, piscina);
        CREDO_VERUM(png.successus);
        CREDO_MAIOR_I32(png.mensura, ZEPHYRUM);

        reductus = imago_caricare_ex_file(via, piscina);
        CREDO_VERUM(reductus.successus);
        CREDO_AEQUALIS_I32(reductus.imago.latitudo, (i32)XXXII);
        CREDO_AEQUALIS_I32(reductus.imago.altitudo, (i32)XVI);

        regula                    = collatio_regula_solita();
        regula.tolerantia         = ZEPHYRUM;
        regula.sperne_lenimentum  = FALSUM;
        collatio = imago_conferre(&fons, &reductus.imago, regula,
            NIHIL);
        CREDO_AEQUALIS_S32((s32)collatio.sententia,
            (s32)COLLATIO_CONGRUUNT);

        remove(via);

        /* Via impossibilis: culpa NOMINATUR, non tacetur */
        png = imago_png_scribere(&fons, "/nusquam/omnino/x.png",
            piscina);
        CREDO_FALSUM(png.successus);
        CREDO_CHORDA_NON_VACUA(png.error);
    }
}


    /* ==================================================
     * IX. Mensurae SINE decodificatione (lapide feature-requests/
     *     006): imago non quadrata, ut latitudo/altitudo permutata
     *     appareant
     * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                     Imago  fons;
                PngFructus  png;
                       i32  latitudo = I;
                       i32  altitudo = I;
        constans character* via = "/tmp/probatio_imago_mensurae.png";
                        i8  purgamentum[XVI];

        fons  = _imago_ficta(piscina, XXXII, XVI, FALSUM);
        png   = imago_png_scribere(&fons, via, piscina);
        CREDO_VERUM(png.successus);
        CREDO_VERUM(imago_mensuras_ex_file(via, &latitudo, &altitudo));
        CREDO_AEQUALIS_I32(latitudo, (i32)XXXII);
        CREDO_AEQUALIS_I32(altitudo, (i32)XVI);
        remove(via);

        png = imago_png_codificare(&fons, piscina);
        CREDO_VERUM(png.successus);
        CREDO_VERUM(imago_mensuras_ex_memoria(png.datum, png.mensura,
            &latitudo, &altitudo));
        CREDO_AEQUALIS_I32(latitudo, (i32)XXXII);
        CREDO_AEQUALIS_I32(altitudo, (i32)XVI);

        /* file absens, octeti non imago: FALSUM et mensurae nullae */
        CREDO_FALSUM(imago_mensuras_ex_file("/nusquam/omnino/x.png",
            &latitudo, &altitudo));
        CREDO_AEQUALIS_I32(latitudo, ZEPHYRUM);
        memset(purgamentum, 'x', magnitudo(purgamentum));
        latitudo = I;
        CREDO_FALSUM(imago_mensuras_ex_memoria(purgamentum,
            (i32)magnitudo(purgamentum), &latitudo, &altitudo));
        CREDO_AEQUALIS_I32(latitudo, ZEPHYRUM);
    }
}


    /* ========================================================
     * IX. Summae contra ORACULUM EXTERNUM
     *
     * MENSURATUM 2026-08-13: stb_image Adler-32 NON probat. Cum
     * summa consulto corrupta esset (adler + I), omnes LIV
     * probationes circuitus VIRIDES manserunt. Ergo circuitus
     * probat "stb legere potest", non "plagula recta est".
     *
     * Hi numeri ex Python (zlib) veniunt - implementum quod nec
     * nos scripsimus nec stb. Valores infixi algorithmum ipsum
     * ligant, non solum collocationem bytorum.
     * ======================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
             Imago fons;
        PngFructus png;
               i32 idat_mensura;
               i32 adler_scriptus;
               i32 crc_ihdr;
               i32 crc_iend;

        fons  = _imago_ficta(piscina, IV, III, FALSUM);
        png   = imago_png_codificare(&fons, piscina);
        CREDO_VERUM(png.successus);

        /* CRC partis IHDR: pendet a mensuris solis, ergo extra
         * computari potest (zlib.crc32 super 'IHDR' + datum) */
        crc_ihdr = _be32_legere(png.datum + XXIX);
        CREDO_AEQUALIS_I32(crc_ihdr, (i32)0xB4F4AEC6u);

        /* Adler-32 in cauda fluxus zlib, super versus NON
         * compressos (byte filtri in quoque versu inclusum) */
        idat_mensura = _be32_legere(png.datum + XXXIII);
        adler_scriptus = _be32_legere(png.datum + XLI + idat_mensura
            - (i32)IV);
        CREDO_AEQUALIS_I32(adler_scriptus, (i32)0x3D9E0D87u);

        /* CRC partis IEND: constans in OMNI plagula PNG valida.
         * Polynomium falsum, initium falsum, aut ambitus falsus
         * (longitudinem includens) hunc numerum frangit. */
        crc_iend = _be32_legere(png.datum + png.mensura - (i32)IV);
        CREDO_AEQUALIS_I32(crc_iend, (i32)0xAE426082u);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. Signum et IHDR", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "II. Circuitus: VII x V", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "III. Imago I x I", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "IV. Alpha varia", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "V. Imago LXIV x XLVIII", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "VI. Determinatio", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "VII. Culpae", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "VIII. Plagula", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "IX. Mensurae sine decodificatione", _sectio_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "IX. Summae contra oraculum externum", _sectio_decima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("imago_png", SECTIONES);
}
