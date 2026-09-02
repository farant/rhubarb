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

s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_imago_png",
        M * M * IV);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * I. Signum et IHDR
     * ======================================================== */

    imprimere("\n--- I. Signum et IHDR ---\n");
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


    /* ========================================================
     * II. Circuitus per stb_image (oraculum alienum)
     * ======================================================== */

    imprimere("\n--- II. Circuitus: VII x V ---\n");
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


    /* ========================================================
     * III. Imago minima
     * ======================================================== */

    imprimere("\n--- III. Imago I x I ---\n");
    {
                  Imago fons;
        CollatioFructus collatio;

        fons      = _imago_ficta(piscina, I, I, FALSUM);
        collatio  = _circuitus(&fons, piscina, NIHIL);

        CREDO_AEQUALIS_S32((s32)collatio.sententia,
            (s32)COLLATIO_CONGRUUNT);
        CREDO_AEQUALIS_I32(collatio.pixela_tota, (i32)I);
    }


    /* ========================================================
     * IV. Alpha servatur (PNG non praemultiplicat)
     * ======================================================== */

    imprimere("\n--- IV. Alpha varia ---\n");
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


    /* ========================================================
     * V. Imago maior
     * ======================================================== */

    imprimere("\n--- V. Imago LXIV x XLVIII ---\n");
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


    /* ========================================================
     * VI. Emissio DETERMINATA
     * ======================================================== */

    imprimere("\n--- VI. Determinatio ---\n");
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


    /* ========================================================
     * VII. Culpae NOMINANTUR
     * ======================================================== */

    imprimere("\n--- VII. Culpae ---\n");
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


    /* ========================================================
     * VIII. Scriptio in plagulam et lectio
     * ======================================================== */

    imprimere("\n--- VIII. Plagula ---\n");
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

    imprimere("\n--- IX. Summae contra oraculum externum ---\n");
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


    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
