/* probatio_graphis_specimen.c - scheda ad inspiciendum
 * (graphis_specimen.h)
 *
 * Scheda: magnitudo, pictura bis aucta, quattuor tabulae suo loco
 * (fons, larva cum lucentibus caeruleis, illustratus, regiones),
 * stria. Quadrum quater auctum et fines eius. Numeri regionum.
 * Colores regionum XII in orbem. Fructus recusatus: nihil.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "imago.h"
#include "dithering.h"
#include "graphis.h"
#include "graphis_specimen.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LAT     GRAPHIS_LATITUDO
#define ALT     GRAPHIS_ALTITUDO
#define DEXTER  (II * LAT + GRAPHIS_SPECIMEN_STRIA)

#define DESCRIPTIO \
    "<pictura lux=\"0.5\">\n" \
    "<rectangulum titulus=\"arca\" fines=\"10 20 30 40\"" \
    " colores=\"#c86432\"/>\n" \
    "<rectangulum titulus=\"fenestra\" fines=\"100 20 30 40\"" \
    " colores=\"0 5\" exemplar=\"tessellatum\" vis=\"1\"" \
    " lucens=\"verum\"/>\n" \
    "<rectangulum fines=\"200 20 30 40\" colores=\"0 5\"" \
    " exemplar=\"tessellatum\" vis=\"1\"/>\n" \
    "</pictura>"

interior GraphisFructus
_pictura (
        CredoContextus* c,
    constans character* literae)
{
    GraphisFructus f = graphis_legere(
        chorda_ex_literis(literae, c->piscina), c->piscina);

    si (!f.successus)
    {
        printf("descriptio recusata (linea %u): %.*s\n",
            (insignatus)f.linea, (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    redde f;
}

interior constans i8*
_pixelum (
    constans Imago* imago,
               i32  x,
               i32  y)
{
    redde imago->pixela + (y * imago->latitudo + x) * IV;
}

interior vacuum
_sectio_scheda (
    CredoContextus* c)
{
    GraphisFructus  f      = _pictura(c, DESCRIPTIO);
             Imago  imago  = graphis_specimen_pingere(&f, c->piscina);
       constans i8* p;
               i32  k;
               i32  mala = ZEPHYRUM;
                i8  griseum[III];

    CREDO_NECESSE (imago.pixela != NIHIL);
    CREDO_AEQUALIS_I32 (imago.latitudo, 1448);
    CREDO_AEQUALIS_I32 (imago.altitudo, 480);
    CREDO_AEQUALIS_I32 (GRAPHIS_SPECIMEN_LATITUDO, 1448);
    /* omne pixelum opacum */
    per (k = ZEPHYRUM; k < imago.latitudo * imago.altitudo; k++)
    {
        si (imago.pixela[k * IV + III] != 255)
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
    /* pictura finita bis aucta: pixelum (15, 30) quater */
    p = AQUINAS_PALETTE[f.indices[30 * LAT + 15]];
    CREDO_VERUM (memcmp(_pixelum(&imago, 30, 60), p, III) == ZEPHYRUM);
    CREDO_VERUM (memcmp(_pixelum(&imago, 31, 60), p, III) == ZEPHYRUM);
    CREDO_VERUM (memcmp(_pixelum(&imago, 30, 61), p, III) == ZEPHYRUM);
    CREDO_VERUM (memcmp(_pixelum(&imago, 31, 61), p, III) == ZEPHYRUM);
    /* pixela vicina alterius pixeli picturae sunt */
    p = AQUINAS_PALETTE[f.indices[30 * LAT + 16]];
    CREDO_VERUM (memcmp(_pixelum(&imago, 32, 60), p, III) == ZEPHYRUM);
    /* angulus ultimus picturae */
    p = AQUINAS_PALETTE[f.indices[LAT * ALT - I]];
    CREDO_VERUM (memcmp(_pixelum(&imago, II * LAT - I, II * ALT - I),
        p, III) == ZEPHYRUM);
    /* stria grisea */
    memset(griseum, 90, III);
    CREDO_VERUM (memcmp(_pixelum(&imago, II * LAT, 100), griseum, III)
        == ZEPHYRUM);
    CREDO_VERUM (memcmp(_pixelum(&imago, DEXTER - I, 479), griseum,
        III) == ZEPHYRUM);
    /* fons: color ante lucem */
    p = _pixelum(&imago, DEXTER + 15, 30);
    CREDO_AEQUALIS_I32 (p[ZEPHYRUM], 0xc8);
    CREDO_AEQUALIS_I32 (p[I], 0x64);
    CREDO_AEQUALIS_I32 (p[II], 0x32);
    /* illustratus: dimidium */
    p = _pixelum(&imago, DEXTER + 15, ALT + 30);
    CREDO_AEQUALIS_I32 (p[ZEPHYRUM], 0x64);
    CREDO_AEQUALIS_I32 (p[I], 0x32);
    CREDO_AEQUALIS_I32 (p[II], 0x19);
    /* larva: planum nigrum, exemplar album, lucens caeruleum */
    p = _pixelum(&imago, DEXTER + LAT + 15, 30);
    CREDO_AEQUALIS_I32 (p[ZEPHYRUM], ZEPHYRUM);
    CREDO_AEQUALIS_I32 (p[II], ZEPHYRUM);
    p = _pixelum(&imago, DEXTER + LAT + 215, 30);
    CREDO_AEQUALIS_I32 (p[ZEPHYRUM], 255);
    CREDO_AEQUALIS_I32 (p[I], 255);
    CREDO_AEQUALIS_I32 (p[II], 255);
    p = _pixelum(&imago, DEXTER + LAT + 115, 30);
    CREDO_VERUM (p[II] > p[ZEPHYRUM] + 100);
    /* regiones: color suus; extra regiones pictura pallida */
    CREDO_VERUM (memcmp(_pixelum(&imago, DEXTER + LAT + 15, ALT + 30),
        graphis_specimen_color(I), III) == ZEPHYRUM);
    CREDO_VERUM (memcmp(_pixelum(&imago, DEXTER + LAT + 115,
        ALT + 30), graphis_specimen_color(II), III) == ZEPHYRUM);
    p = _pixelum(&imago, DEXTER + LAT + 215, ALT + 30);
    CREDO_VERUM (p[ZEPHYRUM] >= 190 && p[I] >= 190 && p[II] >= 190);
    /* scheda eadem bis */
    {
        Imago altera = graphis_specimen_pingere(&f, c->piscina);

        CREDO_NECESSE (altera.pixela != NIHIL);
        CREDO_VERUM (memcmp(imago.pixela, altera.pixela,
            imago.latitudo * imago.altitudo * IV) == ZEPHYRUM);
    }
}

interior vacuum
_sectio_quadrum (
    CredoContextus* c)
{
    GraphisFructus  f = _pictura(c, DESCRIPTIO);
             Imago  imago;
       constans i8* p;
               i32  u;
               i32  v;
               i32  mala = ZEPHYRUM;

    /* margo arcae: columna 8 et 9 fundus, 10 arca */
    imago = graphis_quadrum_pingere(&f, 8, 20, 5, 3, c->piscina);
    CREDO_NECESSE (imago.pixela != NIHIL);
    CREDO_AEQUALIS_I32 (imago.latitudo, XX);
    CREDO_AEQUALIS_I32 (imago.altitudo, XII);
    per (v = ZEPHYRUM; v < III; v++)
    {
        per (u = ZEPHYRUM; u < V; u++)
        {
            i32 a;
            i32 b;

            p = AQUINAS_PALETTE[f.indices[(20 + v) * LAT + 8 + u]];
            per (b = ZEPHYRUM; b < IV; b++)
            {
                per (a = ZEPHYRUM; a < IV; a++)
                {
                    si (memcmp(_pixelum(&imago, u * IV + a,
                        v * IV + b), p, III) != ZEPHYRUM)
                    {
                        mala++;
                    }
                }
            }
        }
    }
    CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
    /* pictura tota et angulus ultimus accipiuntur */
    CREDO_VERUM (graphis_quadrum_pingere(&f, 0, 0, 360, 240,
        c->piscina).pixela != NIHIL);
    CREDO_VERUM (graphis_quadrum_pingere(&f, 359, 239, 1, 1,
        c->piscina).pixela != NIHIL);
    /* extra picturam aut sine magnitudine: nihil */
    CREDO_VERUM (graphis_quadrum_pingere(&f, -1, 0, 5, 5,
        c->piscina).pixela == NIHIL);
    CREDO_VERUM (graphis_quadrum_pingere(&f, 0, -1, 5, 5,
        c->piscina).pixela == NIHIL);
    CREDO_VERUM (graphis_quadrum_pingere(&f, 356, 0, 5, 5,
        c->piscina).pixela == NIHIL);
    CREDO_VERUM (graphis_quadrum_pingere(&f, 0, 236, 5, 5,
        c->piscina).pixela == NIHIL);
    CREDO_VERUM (graphis_quadrum_pingere(&f, 0, 0, 0, 5,
        c->piscina).pixela == NIHIL);
    CREDO_VERUM (graphis_quadrum_pingere(&f, 0, 0, 5, 0,
        c->piscina).pixela == NIHIL);
    CREDO_VERUM (graphis_quadrum_pingere(&f, 0, 0, 361, 5,
        c->piscina).pixela == NIHIL);
}

interior vacuum
_sectio_regiones (
    CredoContextus* c)
{
    GraphisFructus f = _pictura(c,
        "<pictura>\n"
        "<rectangulum titulus=\"arca\" fines=\"10 20 30 40\""
        " colores=\"6\"/>\n"
        "<ovum titulus=\"luna\" fines=\"200 100 40 20\""
        " colores=\"11\"/>\n"
        "<rectangulum fines=\"30 20 100 100\" colores=\"12\"/>\n"
        "</pictura>");
    GraphisRegio r[II];

    CREDO_AEQUALIS_I32 (graphis_regiones(&f, r), II);
    CREDO_VERUM (chorda_aequalis_literis(r[ZEPHYRUM].titulus,
        "arca"));
    /* pars tecta non numeratur: 20 x 40 manent */
    CREDO_AEQUALIS_I32 (r[ZEPHYRUM].pixela, 800);
    CREDO_AEQUALIS_S32 (r[ZEPHYRUM].x_primus, 10);
    CREDO_AEQUALIS_S32 (r[ZEPHYRUM].y_primus, 20);
    CREDO_AEQUALIS_S32 (r[ZEPHYRUM].x_ultimus, 29);
    CREDO_AEQUALIS_S32 (r[ZEPHYRUM].y_ultimus, 59);
    CREDO_VERUM (chorda_aequalis_literis(r[I].titulus, "luna"));
    CREDO_VERUM (r[I].pixela > 600 && r[I].pixela < 656);
    CREDO_AEQUALIS_S32 (r[I].x_primus, 200);
    CREDO_AEQUALIS_S32 (r[I].x_ultimus, 239);
    CREDO_AEQUALIS_S32 (r[I].y_primus, 100);
    CREDO_AEQUALIS_S32 (r[I].y_ultimus, 119);
    CREDO_AEQUALIS_I32 (graphis_regiones(&f, NIHIL), ZEPHYRUM);
}

interior vacuum
_sectio_colores (
    CredoContextus* c)
{
    i32 k;
    i32 j;

    (vacuum)c;
    CREDO_VERUM (graphis_specimen_color(ZEPHYRUM) == NIHIL);
    CREDO_VERUM (graphis_specimen_color_titulus(ZEPHYRUM) == NIHIL);
    per (k = I; k <= GRAPHIS_SPECIMEN_COLORES; k++)
    {
        CREDO_NECESSE (graphis_specimen_color(k) != NIHIL);
        CREDO_NECESSE (graphis_specimen_color_titulus(k) != NIHIL);
        per (j = I; j < k; j++)
        {
            CREDO_VERUM (memcmp(graphis_specimen_color(k),
                graphis_specimen_color(j), III) != ZEPHYRUM);
            CREDO_VERUM (strcmp(graphis_specimen_color_titulus(k),
                graphis_specimen_color_titulus(j)) != ZEPHYRUM);
        }
        /* in orbem */
        CREDO_VERUM (graphis_specimen_color(k)
            == graphis_specimen_color(k + GRAPHIS_SPECIMEN_COLORES));
    }
    CREDO_VERUM (strcmp(graphis_specimen_color_titulus(I), "red")
        == ZEPHYRUM);
    CREDO_VERUM (strcmp(graphis_specimen_color_titulus(XIII), "red")
        == ZEPHYRUM);
}

interior vacuum
_sectio_recusatus (
    CredoContextus* c)
{
    GraphisFructus f = graphis_legere(chorda_ex_literis(
        "<pictura><circulus/></pictura>", c->piscina), c->piscina);
      GraphisRegio r[I];

    CREDO_FALSUM (f.successus);
    CREDO_VERUM (graphis_specimen_pingere(&f, c->piscina).pixela
        == NIHIL);
    CREDO_VERUM (graphis_specimen_pingere(NIHIL, c->piscina).pixela
        == NIHIL);
    CREDO_VERUM (graphis_quadrum_pingere(&f, 0, 0, 5, 5,
        c->piscina).pixela == NIHIL);
    CREDO_AEQUALIS_I32 (graphis_regiones(&f, r), ZEPHYRUM);
    f = _pictura(c, DESCRIPTIO);
    CREDO_VERUM (graphis_specimen_pingere(&f, NIHIL).pixela == NIHIL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "scheda", _sectio_scheda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "quadrum", _sectio_quadrum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "regiones", _sectio_regiones, NIHIL, NIHIL, NIHIL, NIHIL },
    { "colores", _sectio_colores, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusatus", _sectio_recusatus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("graphis_specimen", SECTIONES);
}
