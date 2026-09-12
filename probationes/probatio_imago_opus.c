/* probatio_imago_opus.c - scalatio: tres modi, media alpha-ponderata
 *
 * PRIMA porta automata huius bibliothecae: probatio_imago.c fenestram
 * aperit, nullam assertionem CREDO_ fert, et fenestra_destruere vocat -
 * ergo PROXIMUS et BILINEARIS numquam pinnati sunt. Hic ambo pinnantur
 * ANTE modum novum, ne porta modi novi sola stet.
 *
 * Constantes I2 b (XL et CC) et I2 a (CXXVIII, CCLV, ZEPHYRUM) oraculo
 * ALTERIUS GENERIS confirmatae sunt: Python fractionibus exactis, non
 * arithmetica nostra se ipsam probante (disciplina imago_png, quae stb
 * nec Adler-32 nec CRC validare invenit).
 */

#include "latina.h"
#include "imago_opus.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>
#include <string.h>

interior Imago
_fingere (
        s32  latitudo,
        s32  altitudo,
    Piscina* piscina)
{
    Imago im;

    im.latitudo = (i32)latitudo;
    im.altitudo = (i32)altitudo;
    im.pixela    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(latitudo * altitudo * IV));
    redde im;
}

interior vacuum
_ponere (
     Imago* im,
       s32  x,
       s32  y,
       s32  r,
       s32  g,
       s32  b,
       s32  a)
{
    i8* p = im->pixela + (((y * (s32)im->latitudo) + x) * IV);

    p[ZEPHYRUM]  = (i8)r;
    p[I]         = (i8)g;
    p[II]        = (i8)b;
    p[III]       = (i8)a;
}

interior s32
_legere (
     constans Imago* im,
                s32  x,
                s32  y,
                s32  canalis)
{
    redde (s32)im->pixela[(((y * (s32)im->latitudo) + x) * IV)
        + canalis];
}

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_imago_opus",
                                         16777216);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ---- modi EXSISTENTES pinnati (numquam antea asserti) ---- */
    {
        Imago fons;
        Imago par;
        Imago dimidia;

        imprimere("\n--- modi exsistentes: PROXIMUS, BILINEARIS ---\n");

        fons = _fingere(II, II, piscina);
        CREDO_NON_NIHIL(fons.pixela);
        _ponere(&fons, ZEPHYRUM, ZEPHYRUM, 255, ZEPHYRUM, ZEPHYRUM,
            255);
        _ponere(&fons, I, ZEPHYRUM, ZEPHYRUM, 255, ZEPHYRUM, 255);
        _ponere(&fons, ZEPHYRUM, I, ZEPHYRUM, ZEPHYRUM, 255, 255);
        _ponere(&fons, I, I, 255, 255, 255, 255);

        /* 1:1 per PROXIMUM = transitus exactus */
        par = imago_scalare(&fons, II, II, IMAGO_SCALA_PROXIMUS,
                            piscina);
        CREDO_NON_NIHIL(par.pixela);
        CREDO_AEQUALIS_I32((i32)_legere(&par, ZEPHYRUM, ZEPHYRUM,
                                       ZEPHYRUM), 255);
        CREDO_AEQUALIS_I32((i32)_legere(&par, I, I, I), 255);

        /* 1:1 per BILINEAREM: fractio ZEPHYRUM est, ergo item exactus.
         * MENSURATUM, non creditum: scala = ((fons-1)*FP)/(dest-1)
         * exacte FP_ONE fit cum dimensiones congruunt. */
        par = imago_scalare(&fons, II, II, IMAGO_SCALA_BILINEARIS,
                            piscina);
        CREDO_NON_NIHIL(par.pixela);
        CREDO_AEQUALIS_I32((i32)_legere(&par, ZEPHYRUM, ZEPHYRUM,
                                       ZEPHYRUM), 255);
        CREDO_AEQUALIS_I32((i32)_legere(&par, I, ZEPHYRUM, I), 255);

        /* PROXIMUS 2:1 unum pixelum ELIGIT, non miscet */
        dimidia = imago_scalare(&fons, I, I, IMAGO_SCALA_PROXIMUS,
                                piscina);
        CREDO_NON_NIHIL(dimidia.pixela);
        CREDO_AEQUALIS_I32((i32)_legere(&dimidia, ZEPHYRUM, ZEPHYRUM,
                                       ZEPHYRUM), 255);
        CREDO_AEQUALIS_I32((i32)_legere(&dimidia, ZEPHYRUM, ZEPHYRUM,
            I),
                           ZEPHYRUM);
    }

    /* ---- I2 a: AREA alpha-PONDERATA ---- */
    {
        Imago fons;
        Imago parva;

        imprimere("\n--- I2 a: area alpha-ponderata ---\n");

        /* rubrum OPACUM iuxta ALBUM PELLUCIDUM. Album, non viride:
         * pixelum pellucidum in OMNI canali colorem ferre debet, aliter
         * accumulatores rubri et caerulei contra hoc fixum
         * infalsificabiles sunt - planta prima ('* a' -> '* 255' in
         * canali rubro) omnino INVISIBILIS fuit, quia solum pixelum
         * rubrum ferens alpha CCLV iam habebat. */
        fons = _fingere(II, I, piscina);
        CREDO_NON_NIHIL(fons.pixela);
        _ponere(&fons, ZEPHYRUM, ZEPHYRUM, 255, ZEPHYRUM, ZEPHYRUM,
            255);
        _ponere(&fons, I, ZEPHYRUM, 255, 255, 255, ZEPHYRUM);

        parva = imago_scalare(&fons, I, I, IMAGO_SCALA_AREA, piscina);
        CREDO_NON_NIHIL(parva.pixela);

        /* RECTUM: rubrum plenum, alpha dimidium - album pellucidum
         * nihil confert. Media ingenua (255,127,127,127) daret, ergo
         * canales VIRIDIS et CAERULEUS uterque plantam capiunt, et
         * canalis ruber per fixum album quoque falsificabilis est. */
        CREDO_AEQUALIS_I32((i32)_legere(&parva, ZEPHYRUM, ZEPHYRUM,
                                       ZEPHYRUM), 255);
        CREDO_AEQUALIS_I32((i32)_legere(&parva, ZEPHYRUM, ZEPHYRUM, I),
                           ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)_legere(&parva, ZEPHYRUM, ZEPHYRUM, II),
                           ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)_legere(&parva, ZEPHYRUM, ZEPHYRUM,
            III),
                           128);
    }

    /* ---- I2 b: COPERTURA FRACTIONARIA, constantes pinnatae ---- */
    {
        Imago fons;
        Imago duo;

        imprimere("\n--- I2 b: copertura fractionaria III -> II ---\n");

        /* Ex fonte MXXIV omnis magnitudo ratio potestatis binariae
         * exacta est, ergo tabula integra probationes nostras naturales
         * omnes transiret. Haec ratio III -> II fractionaria est. */
        fons = _fingere(III, I, piscina);
        CREDO_NON_NIHIL(fons.pixela);
        _ponere(&fons, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, 255);
        _ponere(&fons, I, ZEPHYRUM, 120, 120, 120, 255);
        _ponere(&fons, II, ZEPHYRUM, 240, 240, 240, 255);

        duo = imago_scalare(&fons, II, I, IMAGO_SCALA_AREA, piscina);
        CREDO_NON_NIHIL(duo.pixela);

        /* Python fractionibus exactis: pixelum 0 extentum [0, 3/2)
         * tegit -> (0*1.0 + 120*0.5)/1.5 = XL; pixelum 1 [3/2, 3) ->
         * (120*0.5 + 240*1.0)/1.5 = CC. */
        CREDO_AEQUALIS_I32((i32)_legere(&duo, ZEPHYRUM, ZEPHYRUM,
                                       ZEPHYRUM), 40);
        CREDO_AEQUALIS_I32((i32)_legere(&duo, I, ZEPHYRUM, ZEPHYRUM),
                           200);
        CREDO_AEQUALIS_I32((i32)_legere(&duo, ZEPHYRUM, ZEPHYRUM, III),
                           255);
    }

    /* ---- I2 c: AREA ad 1:1 TRANSIT, sine custodia propria ---- */
    {
        Imago fons;
        Imago par;

        imprimere("\n--- I2 c: area 1:1 exacta ---\n");

        /* PRAEDICTIO: nulla custodia 1:1 in implementatione est, sed
         * arithmetica eadem exactum dat - extentum unius pixeli
         * pondus plenum accipit et rotundatio dimidii sursum valorem
         * non movet. Si haec probatio rubra fit, ratio mea falsa est
         * et id scire volo, non custodia tegere. */
        fons = _fingere(III, I, piscina);
        CREDO_NON_NIHIL(fons.pixela);
        _ponere(&fons, ZEPHYRUM, ZEPHYRUM, 13, 21, 34, 255);
        _ponere(&fons, I, ZEPHYRUM, 55, 89, 144, 200);
        _ponere(&fons, II, ZEPHYRUM, 233, 255, 1, 100);

        par = imago_scalare(&fons, III, I, IMAGO_SCALA_AREA, piscina);
        CREDO_NON_NIHIL(par.pixela);
        CREDO_AEQUALIS_I32((i32)_legere(&par, I, ZEPHYRUM, ZEPHYRUM),
            55);
        CREDO_AEQUALIS_I32((i32)_legere(&par, I, ZEPHYRUM, I), 89);
        CREDO_AEQUALIS_I32((i32)_legere(&par, II, ZEPHYRUM, III), 100);
    }

    /* ---- I2 d: AREA per REGIONEM (imago_extrahere_et_scalare) ---- */
    {
        Imago fons;
        Imago regio;
        Imago per_regionem;
        Imago area_regionis;
        Imago bilinearis_regionis;
          s32 x;
          s32 y;
          s32 valor;

        imprimere("\n--- I2 d: area per regionem ---\n");

        /* Olim imago_extrahere_et_scalare modum NULLUM nisi PROXIMUS
         * agnoscebat, et AREA TACITE bilinearis fiebat: icones sic
         * omnes icones bilineares scripsit, et nulla porta id vidit
         * quia I2 solum imago_scalare probabat. Porta EQUIVALENTIAE:
         * regio per extrahere == eadem regio manu copiata per
         * imago_scalare.
         *
         * Fons VI x II; regio [I, V) x [0, II) = IV x II; exitus
         * II x I. Rationes IV -> II et II -> I EXACTAE sunt, ergo
         * truncatio 'intra limites' functionis dimensiones non
         * movet. */
        fons = _fingere(VI, II, piscina);
        CREDO_NON_NIHIL(fons.pixela);
        per (x = ZEPHYRUM; x < VI; x++)
        {
            valor = (x + I) * X;
            _ponere(&fons, x, ZEPHYRUM, valor, 255 - valor, 100, 255);
            _ponere(&fons, x, I, 200 - (x * XX), x * XX, 100, 255);
        }

        regio = _fingere(IV, II, piscina);
        CREDO_NON_NIHIL(regio.pixela);
        per (y = ZEPHYRUM; y < II; y++)
        {
            per (x = ZEPHYRUM; x < IV; x++)
            {
                _ponere(&regio, x, y,
                        _legere(&fons, x + I, y, ZEPHYRUM),
                        _legere(&fons, x + I, y, I),
                        _legere(&fons, x + I, y, II),
                        _legere(&fons, x + I, y, III));
            }
        }

        per_regionem = imago_extrahere_et_scalare(&fons, I, ZEPHYRUM,
            IV, II, II, I, IMAGO_SCALA_AREA, piscina);
        area_regionis = imago_scalare(&regio, II, I, IMAGO_SCALA_AREA,
                                      piscina);
        bilinearis_regionis = imago_scalare(&regio, II, I,
                                            IMAGO_SCALA_BILINEARIS,
                                            piscina);
        CREDO_NON_NIHIL(per_regionem.pixela);
        CREDO_NON_NIHIL(area_regionis.pixela);
        CREDO_NON_NIHIL(bilinearis_regionis.pixela);
        CREDO_AEQUALIS_I32(per_regionem.latitudo, II);
        CREDO_AEQUALIS_I32(per_regionem.altitudo, I);

        /* PINNAE MANU COMPUTATAE: (20 + 30 + 180 + 160) / 4 = 97.5 ->
         * XCVIII; (40 + 50 + 140 + 120) / 4 = 87.5 -> LXXXVIII
         * (rotundatio dimidii sursum, pondera omnia plena). */
        CREDO_AEQUALIS_I32((i32)_legere(&per_regionem, ZEPHYRUM,
                                       ZEPHYRUM, ZEPHYRUM), 98);
        CREDO_AEQUALIS_I32((i32)_legere(&per_regionem, I, ZEPHYRUM,
                                       ZEPHYRUM), 88);

        /* EQUIVALENTIA octetim cum imago_scalare super regionem */
        CREDO_VERUM(memcmp(per_regionem.pixela, area_regionis.pixela,
                           (size_t)VIII) == ZEPHYRUM);

        /* FIXUM FALSIFICABILE: bilinearis HIC aliud dat, ergo
         * aequalitas supra non per casum stat. Si area et bilinearis
         * in hoc fixo congruerent, porta modum discernere non
         * posset. */
        CREDO_VERUM(memcmp(bilinearis_regionis.pixela,
                           area_regionis.pixela, (size_t)VIII)
                    != ZEPHYRUM);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
