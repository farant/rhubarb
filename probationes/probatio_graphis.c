/* probatio_graphis.c - compilator formarum planarum (graphis.h)
 *
 * Formae (rectangulum, ovum, polygonum, linea, grex), colores,
 * exemplaria cum vi, margo, ordo picturae, tegumentum, tabula
 * dominorum, praecisio ad fines picturae, recusationes cum linea,
 * determinata cum digesto fixo (arithmetica integra: eadem in omni
 * machina).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "friatio.h"
#include "exemplaria.h"
#include "dithering.h"
#include "graphis.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LAT GRAPHIS_LATITUDO
#define ALT GRAPHIS_ALTITUDO

interior GraphisFructus
_lege (
        CredoContextus* c,
    constans character* literae)
{
    redde graphis_legere(chorda_ex_literis(literae, c->piscina),
        c->piscina);
}

/* descriptio accepta; recusatio imprimitur */
interior GraphisFructus
_pictura (
        CredoContextus* c,
    constans character* literae)
{
    GraphisFructus f = _lege(c, literae);

    si (!f.successus)
    {
        printf("descriptio recusata (linea %u): %.*s\n",
            (insignatus)f.linea, (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    CREDO_NECESSE (f.tabula != NIHIL);
    CREDO_NECESSE (f.indices != NIHIL);
    redde f;
}

/* recusatur in linea data, nuntio fragmentum continente */
#define RECUSATUR(literae, linea_sperata, fragmentum) \
    fac { \
        GraphisFructus rf = _lege(c, (literae)); \
        CREDO_FALSUM (rf.successus); \
        CREDO_VERUM (rf.tabula == NIHIL); \
        CREDO_VERUM (rf.indices == NIHIL); \
        si (rf.linea != (linea_sperata) || !chorda_continet(rf.error, \
            chorda_ex_literis((fragmentum), c->piscina))) \
        { \
            printf("linea %u, nuntius: %.*s\n", \
                (insignatus)rf.linea, (integer)rf.error.mensura, \
                (constans character*)rf.error.datum); \
        } \
        CREDO_AEQUALIS_I32 (rf.linea, (linea_sperata)); \
        CREDO_VERUM (chorda_continet(rf.error, \
            chorda_ex_literis((fragmentum), c->piscina))); \
    } dum (FALSUM)

interior i32
_index (
    constans GraphisFructus* f,
                        i32  x,
                        i32  y)
{
    redde (i32)f->indices[y * LAT + x];
}

interior i32
_dominus (
    constans GraphisFructus* f,
                        i32  x,
                        i32  y)
{
    redde (i32)f->tabula->domini[y * LAT + x];
}

/* pixela picturae cum indice dato */
interior i32
_numera (
    constans GraphisFructus* f,
                        i32  index)
{
    i32 k;
    i32 n = ZEPHYRUM;

    per (k = ZEPHYRUM; k < LAT * ALT; k++)
    {
        si ((i32)f->indices[k] == index)
        {
            n++;
        }
    }
    redde n;
}

/* pixelum fontis colorem palettae exacte tenet? */
interior b32
_fons_palettae (
    constans GraphisFructus* f,
                        i32  x,
                        i32  y,
                        i32  index)
{
    constans i8* p = f->tabula->fons + (y * LAT + x) * IV;

    redde memcmp(p, AQUINAS_PALETTE[index], III) == ZEPHYRUM
        && p[III] == 255;
}

interior vacuum
_sectio_quadratum (
    CredoContextus* c)
{
    GraphisFructus f = _pictura(c,
        "<pictura titulus=\"q\">\n"
        "<rectangulum fines=\"10 20 30 40\" colores=\"6\"/>\n"
        "</pictura>");

    CREDO_AEQUALIS_I32 (_numera(&f, VI), 1200);
    CREDO_AEQUALIS_I32 (_numera(&f, GRAPHIS_COLOR_FUNDI),
        LAT * ALT - 1200);
    CREDO_AEQUALIS_I32 (_index(&f, X, XX), VI);
    CREDO_AEQUALIS_I32 (_index(&f, 39, 59), VI);
    CREDO_AEQUALIS_I32 (_index(&f, IX, XX), GRAPHIS_COLOR_FUNDI);
    CREDO_AEQUALIS_I32 (_index(&f, 40, 60), GRAPHIS_COLOR_FUNDI);
    CREDO_VERUM (_fons_palettae(&f, X, XX, VI));
    CREDO_VERUM (_fons_palettae(&f, ZEPHYRUM, ZEPHYRUM,
        GRAPHIS_COLOR_FUNDI));
    CREDO_AEQUALIS_I32 (f.tabula->titulorum, ZEPHYRUM);
    /* color liber: fons eum exacte tenet */
    f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"0 0 8 8\" colores=\"#102030\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (f.tabula->fons[ZEPHYRUM], 0x10);
    CREDO_AEQUALIS_I32 (f.tabula->fons[I], 0x20);
    CREDO_AEQUALIS_I32 (f.tabula->fons[II], 0x30);
    CREDO_AEQUALIS_I32 (f.tabula->fons[III], 255);
    /* magnitudo dicta, si recta, accipitur */
    f = _pictura(c,
        "<pictura latitudo=\"360\" altitudo=\"240\">\n"
        "<rectangulum fines=\"0 0 8 8\" colores=\"6\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (_numera(&f, VI), LXIV);
}

/* palettae colores, vis plena: pictura = exemplar ipsum */
interior vacuum
_sectio_exemplar_purum (
    CredoContextus* c)
{
    GraphisFructus f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"0 0 360 240\" colores=\"0 11\""
        " exemplar=\"latericium\" vis=\"1\"/>\n"
        "</pictura>");
    i32 x;
    i32 y;
    i32 mala = ZEPHYRUM;

    per (y = ZEPHYRUM; y < ALT; y++)
    {
        per (x = ZEPHYRUM; x < LAT; x++)
        {
            i32 speratus = exemplar_punctum(EXEMPLAR_LATERICIUM,
                (s32)x, (s32)y) ? ZEPHYRUM : XI;

            si (   _index(&f, x, y)             != speratus
                || f.tabula->larva[y * LAT + x] != 255)
            {
                mala++;
            }
        }
    }
    CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
}

interior vacuum
_sectio_vis (
    CredoContextus* c)
{
       constans i8* octeti = exemplar_obtinere(EXEMPLAR_TESSELLATUM);
    GraphisFructus  f;
               i32  densitas = ZEPHYRUM;
               i32  k;
               i32  b;

    per (k = ZEPHYRUM; k < VIII; k++)
    {
        per (b = ZEPHYRUM; b < VIII; b++)
        {
            densitas += (i32)(octeti[k] >> b) & I;
        }
    }
    /* vis 0, color unus palettae: index unus ubique */
    f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"0 0 360 240\" colores=\"9\""
        " exemplar=\"tessellatum\" vis=\"0\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (_numera(&f, IX), LAT * ALT);
    /* vis 0, colores duo: planus = mixtura secundum densitatem */
    f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"0 0 360 240\""
        " colores=\"#000000 #404040\""
        " exemplar=\"tessellatum\" vis=\"0\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (f.tabula->fons[ZEPHYRUM],
        (0x40 * (LXIV - densitas) + XXXII) / LXIV);
    CREDO_AEQUALIS_I32 (f.tabula->fons[(LAT + I) * IV],
        f.tabula->fons[ZEPHYRUM]);
    CREDO_AEQUALIS_I32 (f.tabula->larva[ZEPHYRUM], ZEPHYRUM);
    /* vis ordinaria: 0.6 si colores duo et exemplar datum */
    f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"0 0 360 240\" colores=\"0 5\""
        " exemplar=\"tessellatum\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (f.tabula->larva[ZEPHYRUM], 154);
    /* ... aliter 0 */
    f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"0 0 360 240\" colores=\"0 5\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (f.tabula->larva[ZEPHYRUM], ZEPHYRUM);
    /* vis media: fons inter planum et exemplar */
    f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"0 0 360 240\""
        " colores=\"#000000 #c8c8c8\""
        " exemplar=\"tessellatum\" vis=\"0.5\"/>\n"
        "</pictura>");
    {
        i32 planus  = (200 * (LXIV - densitas) + XXXII) / LXIV;
        i32 positus = exemplar_punctum(EXEMPLAR_TESSELLATUM,
            ZEPHYRUM, ZEPHYRUM) ? ZEPHYRUM : 200;

        CREDO_AEQUALIS_I32 (f.tabula->fons[ZEPHYRUM],
            (planus * 128 + positus * 128 + 128) >> VIII);
        CREDO_AEQUALIS_I32 (f.tabula->larva[ZEPHYRUM], 128);
    }
}

interior vacuum
_sectio_ordo (
    CredoContextus* c)
{
    GraphisFructus f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"0 0 100 100\" colores=\"6\"/>\n"
        "<rectangulum fines=\"50 50 100 100\" colores=\"12\"/>\n"
        "</pictura>");
    i32 x;
    i32 y;
    i32 mala = ZEPHYRUM;

    CREDO_AEQUALIS_I32 (_index(&f, X, X), VI);
    CREDO_AEQUALIS_I32 (_index(&f, 60, 60), XII);
    CREDO_AEQUALIS_I32 (_numera(&f, VI), 7500);
    /* tegumentum: biti vacui quod subest relinquunt */
    f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"0 0 360 240\" colores=\"6\"/>\n"
        "<rectangulum fines=\"0 0 360 240\" colores=\"0 -\""
        " exemplar=\"tessellatum\"/>\n"
        "</pictura>");
    per (y = ZEPHYRUM; y < ALT; y++)
    {
        per (x = ZEPHYRUM; x < LAT; x++)
        {
            i32 speratus = exemplar_punctum(EXEMPLAR_TESSELLATUM,
                (s32)x, (s32)y) ? ZEPHYRUM : VI;

            si (_index(&f, x, y) != speratus)
            {
                mala++;
            }
        }
    }
    CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
    /* grex locum addit, etiam nidatus */
    f = _pictura(c,
        "<pictura>\n"
        "<grex locus=\"100 50\">\n"
        "<grex locus=\"-10 5\">\n"
        "<rectangulum fines=\"1 2 4 4\" colores=\"6\"/>\n"
        "</grex>\n"
        "</grex>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (_numera(&f, VI), XVI);
    CREDO_AEQUALIS_I32 (_index(&f, 91, 57), VI);
    CREDO_AEQUALIS_I32 (_index(&f, 94, 60), VI);
    CREDO_AEQUALIS_I32 (_index(&f, 90, 57), GRAPHIS_COLOR_FUNDI);
}

interior vacuum
_sectio_domini (
    CredoContextus* c)
{
    GraphisFructus f = _pictura(c,
        "<pictura>\n"
        "<rectangulum titulus=\"arca\" fines=\"10 10 40 40\""
        " colores=\"6\" stilus=\"2 0\"/>\n"
        "<rectangulum fines=\"30 30 40 40\" colores=\"12\"/>\n"
        "<ovum titulus=\"luna\" fines=\"200 100 40 40\""
        " colores=\"11\"/>\n"
        "</pictura>");

    CREDO_AEQUALIS_I32 (f.tabula->titulorum, II);
    CREDO_VERUM (chorda_aequalis_literis(f.tabula->tituli[ZEPHYRUM],
        "arca"));
    CREDO_VERUM (chorda_aequalis_literis(f.tabula->tituli[I],
        "luna"));
    CREDO_AEQUALIS_I32 (_dominus(&f, XV, XV), I);
    /* margo quoque formae est */
    CREDO_AEQUALIS_I32 (_index(&f, X, X), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_dominus(&f, X, X), I);
    /* pars tecta a forma sine titulo: nullius */
    CREDO_AEQUALIS_I32 (_dominus(&f, 35, 35), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_dominus(&f, 60, 60), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_dominus(&f, 100, 200), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_dominus(&f, 220, 120), II);
    /* angulus quadrati circa ovum extra ovum est */
    CREDO_AEQUALIS_I32 (_dominus(&f, 200, 100), ZEPHYRUM);
    /* tegumentum sine titulo regionem non rumpit; cum titulo
     * bitos positos suos possidet */
    f = _pictura(c,
        "<pictura>\n"
        "<rectangulum titulus=\"arca\" fines=\"0 0 40 40\""
        " colores=\"6\"/>\n"
        "<rectangulum fines=\"0 0 20 40\" colores=\"0 -\""
        " exemplar=\"tessellatum\"/>\n"
        "<rectangulum titulus=\"umbra\" fines=\"20 0 20 40\""
        " colores=\"0 -\" exemplar=\"tessellatum\"/>\n"
        "</pictura>");
    {
        i32 x;
        i32 y;
        i32 mala = ZEPHYRUM;

        per (y = ZEPHYRUM; y < 40; y++)
        {
            per (x = ZEPHYRUM; x < 40; x++)
            {
                i32 speratus = (x >= XX && exemplar_punctum(
                    EXEMPLAR_TESSELLATUM, (s32)x, (s32)y)) ? II : I;

                si (_dominus(&f, x, y) != speratus)
                {
                    mala++;
                }
            }
        }
        CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
    }
}

interior vacuum
_sectio_ovum_et_linea (
    CredoContextus* c)
{
    GraphisFructus f = _pictura(c,
        "<pictura>\n"
        "<ovum fines=\"100 100 40 20\" colores=\"6\"/>\n"
        "</pictura>");
    i32 x;
    i32 y;
    i32 n     = _numera(&f, VI);
    i32 mala  = ZEPHYRUM;

    /* area ellipsis: pi x 20 x 10 = 628 */
    CREDO_VERUM (n > 600 && n < 656);
    per (y = 100; y < 120; y++)
    {
        per (x = 100; x < 140; x++)
        {
            si (   _index(&f, x, y) != _index(&f, 239 - x, y)
                || _index(&f, x, y) != _index(&f, x, 219 - y))
            {
                mala++;
            }
        }
    }
    CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_index(&f, 120, 110), VI);
    CREDO_AEQUALIS_I32 (_index(&f, 100, 100), GRAPHIS_COLOR_FUNDI);
    /* margo ovi: anulus, medium vacuum */
    f = _pictura(c,
        "<pictura>\n"
        "<ovum fines=\"100 100 40 20\" stilus=\"2 6\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (_index(&f, 120, 110), GRAPHIS_COLOR_FUNDI);
    CREDO_AEQUALIS_I32 (_index(&f, 120, 100), VI);
    CREDO_AEQUALIS_I32 (_index(&f, 120, 101), VI);
    CREDO_AEQUALIS_I32 (_index(&f, 120, 102), GRAPHIS_COLOR_FUNDI);
    CREDO_AEQUALIS_I32 (_index(&f, 100, 110), VI);
    CREDO_VERUM (_numera(&f, VI) < n / II);
    /* linea: quadrata lateris 'crassitudo' per iter posita */
    f = _pictura(c,
        "<pictura>\n"
        "<linea puncta=\"10,10 50,10\" stilus=\"1 6\"/>\n"
        "<linea puncta=\"10,30 50,30\" stilus=\"3 0\"/>\n"
        "<linea puncta=\"100,100 110,110 120,100\" stilus=\"1 12\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (_numera(&f, VI), 41);
    CREDO_AEQUALIS_I32 (_numera(&f, ZEPHYRUM), 43 * III);
    CREDO_AEQUALIS_I32 (_index(&f, IX, 29), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_index(&f, 51, 31), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numera(&f, XII), XXI);
    CREDO_AEQUALIS_I32 (_index(&f, 105, 105), XII);
    CREDO_AEQUALIS_I32 (_index(&f, 115, 105), XII);
    /* polygonum: triangulum rectum, dimidium quadrati */
    f = _pictura(c,
        "<pictura>\n"
        "<polygonum puncta=\"0,0 100,0 0,100\" colores=\"6\"/>\n"
        "</pictura>");
    n = _numera(&f, VI);
    CREDO_VERUM (n >= 4950 && n <= 5100);
    CREDO_AEQUALIS_I32 (_index(&f, X, X), VI);
    CREDO_AEQUALIS_I32 (_index(&f, 90, 90), GRAPHIS_COLOR_FUNDI);
}

interior vacuum
_sectio_praecisio (
    CredoContextus* c)
{
    GraphisFructus f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"-10 -10 20 20\" colores=\"6\"/>\n"
        "<ovum fines=\"-4000 100 4020 10\" colores=\"0\"/>\n"
        "<linea puncta=\"-4096,120 4096,120\" stilus=\"16 9\"/>\n"
        "<polygonum puncta=\"340,-50 400,120 340,300\""
        " colores=\"11\" stilus=\"1 0\"/>\n"
        "<rectangulum fines=\"350 230 100 100\" colores=\"12\"/>\n"
        "</pictura>");

    CREDO_AEQUALIS_I32 (_numera(&f, VI), 100);
    CREDO_AEQUALIS_I32 (_numera(&f, XII), 100);
    CREDO_VERUM (_numera(&f, IX) > LAT * VIII);
    CREDO_VERUM (_numera(&f, ZEPHYRUM) > ZEPHYRUM);
    CREDO_VERUM (_numera(&f, XI) > ZEPHYRUM);
    /* linea sola per totam latitudinem: XVI ordines, nec plus */
    f = _pictura(c,
        "<pictura>\n"
        "<linea puncta=\"-4096,120 4096,120\" stilus=\"16 9\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (_numera(&f, IX), LAT * XVI);
}

interior vacuum
_sectio_exemplaria (
    CredoContextus* c)
{
    i32 k;
    i32 j;

    (vacuum)c;
    per (k = ZEPHYRUM; k < EXEMPLAR_NUMERUS; k++)
    {
        constans character* t = graphis_exemplar_titulus(k);

        CREDO_NECESSE (t != NIHIL);
        CREDO_VERUM (t[ZEPHYRUM] != '\0');
        per (j = ZEPHYRUM; j < k; j++)
        {
            CREDO_VERUM (strcmp(t, graphis_exemplar_titulus(j))
                != ZEPHYRUM);
        }
    }
    CREDO_VERUM (graphis_exemplar_titulus(EXEMPLAR_NUMERUS) == NIHIL);
    CREDO_VERUM (strcmp(graphis_exemplar_titulus(EXEMPLAR_SOLIDUS),
        "solidus") == ZEPHYRUM);
    CREDO_VERUM (strcmp(graphis_exemplar_titulus(EXEMPLAR_TEXTUM),
        "textum") == ZEPHYRUM);
    CREDO_VERUM (strcmp(graphis_exemplar_titulus(
        EXEMPLAR_HORIZONTALIS_LATUS), "horizontalis_latus")
        == ZEPHYRUM);
    CREDO_VERUM (strcmp(graphis_exemplar_titulus(
        EXEMPLAR_RHOMBUS_LINEATUS), "rhombus_lineatus") == ZEPHYRUM);
}

#define CAPUT "<pictura>\n"
#define CAUDA "\n</pictura>"

interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    character longa[XXXII * M];
          i32 k;
          i32 n;

    RECUSATUR ("<pictura", I, "not valid STML");
    RECUSATUR ("<res titulus=\"a\"/>", I, "<pictura>");
    RECUSATUR (CAPUT "<circulus fines=\"0 0 9 9\" colores=\"6\"/>"
        CAUDA, II, "unknown element");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\" colores=\"6\""
        " color=\"6\"/>" CAUDA, II, "unknown attribute 'color'");
    RECUSATUR ("<pictura semen=\"5\">\n</pictura>", I,
        "unknown attribute 'semen'");
    RECUSATUR (CAPUT "<rectangulum colores=\"6\"/>" CAUDA, II,
        "fines");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9\" colores=\"6\"/>"
        CAUDA, II, "fines");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9.5 9\""
        " colores=\"6\"/>" CAUDA, II, "fines");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 a 9\" colores=\"6\"/>"
        CAUDA, II, "fines");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 5000 9 9\""
        " colores=\"6\"/>" CAUDA, II, "fines");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 0 9\" colores=\"6\"/>"
        CAUDA, II, "size");
    RECUSATUR (CAPUT "<ovum fines=\"0 0 9 -3\" colores=\"6\"/>"
        CAUDA, II, "size");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\" colores=\"16\"/>"
        CAUDA, II, "colores");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\""
        " colores=\"#12345\"/>" CAUDA, II, "colores");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\""
        " colores=\"1 2 3\"/>" CAUDA, II, "colores");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\""
        " colores=\"- 2\"/>" CAUDA, II, "colores");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\" colores=\"1 2\""
        " exemplar=\"marmor\"/>" CAUDA, II, "exemplar");
    RECUSATUR (CAPUT "\n<rectangulum fines=\"0 0 9 9\""
        " colores=\"1 2\" exemplar=\"textum\" vis=\"0.2\"/>" CAUDA,
        III, "vis");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\" colores=\"1 2\""
        " exemplar=\"textum\" vis=\"1.5\"/>" CAUDA, II, "vis");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\" colores=\"1 2\""
        " exemplar=\"textum\" vis=\"0.4567\"/>" CAUDA, II, "vis");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\" colores=\"1 -\""
        " exemplar=\"textum\" vis=\"0.5\"/>" CAUDA, II, "vis");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\"/>" CAUDA, II,
        "colores or stilus");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\""
        " stilus=\"0 6\"/>" CAUDA, II, "stilus");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\""
        " stilus=\"17 6\"/>" CAUDA, II, "stilus");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\""
        " stilus=\"2\"/>" CAUDA, II, "stilus");
    RECUSATUR (CAPUT "<polygonum puncta=\"0,0 9,9\" colores=\"6\"/>"
        CAUDA, II, "puncta");
    RECUSATUR (CAPUT "<polygonum puncta=\"0,0 9,9 4\""
        " colores=\"6\"/>" CAUDA, II, "puncta");
    RECUSATUR (CAPUT "<linea puncta=\"0,0\" stilus=\"1 0\"/>" CAUDA,
        II, "puncta");
    RECUSATUR (CAPUT "<linea puncta=\"0,0 9,9\"/>" CAUDA, II,
        "stilus");
    RECUSATUR (CAPUT "<linea puncta=\"0,0 9,9\" stilus=\"1 0\""
        " colores=\"6\"/>" CAUDA, II, "unknown attribute 'colores'");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\" colores=\"6\"/>"
        "\n<rectangulum fines=\"400 0 9 9\" colores=\"6\"/>" CAUDA,
        III, "outside");
    RECUSATUR (CAPUT "<linea puncta=\"-50,-5 -9,-9\" stilus=\"2 0\"/>"
        CAUDA, II, "outside");
    RECUSATUR (CAPUT "<grex locus=\"5\"/>" CAUDA, II, "locus");
    RECUSATUR (CAPUT "<grex locus=\"0 0\" colores=\"6\"/>" CAUDA, II,
        "unknown attribute 'colores'");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\" colores=\"6\">"
        "<ovum fines=\"0 0 9 9\" colores=\"6\"/></rectangulum>"
        CAUDA, II, "children");
    RECUSATUR (CAPUT "<rectangulum titulus=\"\" fines=\"0 0 9 9\""
        " colores=\"6\"/>" CAUDA, II, "titulus");
    RECUSATUR (CAPUT "<rectangulum titulus=\"a\" fines=\"0 0 9 9\""
        " colores=\"6\"/>\n<rectangulum titulus=\"a\""
        " fines=\"20 0 9 9\" colores=\"6\"/>" CAUDA, III,
        "'a' is used twice");
    /* regio minor LXIV pixelis: 7 x 9 = 63 */
    RECUSATUR (CAPUT "<rectangulum titulus=\"parva\""
        " fines=\"0 0 7 9\" colores=\"6\"/>" CAUDA, II, "'parva'");
    /* ... aut tecta donec minor fit */
    RECUSATUR (CAPUT "<rectangulum titulus=\"tecta\""
        " fines=\"0 0 20 20\" colores=\"6\"/>\n"
        "<rectangulum fines=\"0 0 20 17\" colores=\"12\"/>" CAUDA,
        II, "'tecta'");
    RECUSATUR ("<pictura latitudo=\"320\" altitudo=\"240\">\n"
        "</pictura>", I, "360");
    RECUSATUR ("<pictura altitudo=\"200\">\n</pictura>", I, "240");
    /* tituli plus quam CCLV */
    n = (i32)sprintf(longa, "<pictura>\n");
    per (k = ZEPHYRUM; k < GRAPHIS_TITULI_MAXIMI + I; k++)
    {
        n += (i32)sprintf(longa + n, "<rectangulum titulus=\"t%u\""
            " fines=\"%u %u 8 8\" colores=\"6\"/>\n", (insignatus)k,
            (insignatus)(k % 40 * IX), (insignatus)(k / 40 * IX));
    }
    (vacuum)sprintf(longa + n, "</pictura>");
    RECUSATUR (longa, GRAPHIS_TITULI_MAXIMI + II, "255");
    /* numerus ipse CCLV accipitur */
    n = (i32)sprintf(longa, "<pictura>\n");
    per (k = ZEPHYRUM; k < GRAPHIS_TITULI_MAXIMI; k++)
    {
        n += (i32)sprintf(longa + n, "<rectangulum titulus=\"t%u\""
            " fines=\"%u %u 8 8\" colores=\"6\"/>\n", (insignatus)k,
            (insignatus)(k % 40 * IX), (insignatus)(k / 40 * IX));
    }
    (vacuum)sprintf(longa + n, "</pictura>");
    {
        GraphisFructus f = _pictura(c, longa);

        CREDO_AEQUALIS_I32 (f.tabula->titulorum,
            GRAPHIS_TITULI_MAXIMI);
        CREDO_AEQUALIS_I32 (_dominus(&f, XIV * IX, VI * IX),
            GRAPHIS_TITULI_MAXIMI);
    }
}

#define DESCRIPTIO_FIXA \
    "<pictura titulus=\"fixa\">\n" \
    "<polygonum puncta=\"0,0 360,0 296,34 64,34\"" \
    " colores=\"1 9\" exemplar=\"lineae_verticales\" vis=\"0.5\"/>\n" \
    "<rectangulum fines=\"64 34 232 116\" colores=\"9 15\"" \
    " exemplar=\"horizontalis_latus\" vis=\"0.45\"" \
    " stilus=\"1 0\"/>\n" \
    "<polygonum puncta=\"0,204 64,150 296,150 360,204 360,240" \
    " 0,240\" colores=\"#a46800 #c8b8a0\" exemplar=\"margo_sinister\"" \
    " stilus=\"1 0\"/>\n" \
    "<ovum titulus=\"stragulum\" fines=\"96 186 150 40\"" \
    " colores=\"6 15\" exemplar=\"rhombus_lineatus\" vis=\"1\"" \
    " stilus=\"2 6\"/>\n" \
    "<grex locus=\"196 150\">\n" \
    "<linea puncta=\"14,26 10,62\" stilus=\"4 1\"/>\n" \
    "<polygonum titulus=\"mensa\" puncta=\"22,8 98,8 118,26 2,26\"" \
    " colores=\"10\" stilus=\"1 0\"/>\n" \
    "</grex>\n" \
    "<rectangulum fines=\"214 156 80 40\" colores=\"11 -\"" \
    " exemplar=\"puncta_duplicia_dispersa\"/>\n" \
    "</pictura>"

#define DIGESTUM_INDICUM  0x17fa4398u
#define DIGESTUM_FONTIS   0x71449233u
#define DIGESTUM_DOMINORUM 0xa14b367eu

interior vacuum
_sectio_determinata (
    CredoContextus* c)
{
    GraphisFructus a = _pictura(c, DESCRIPTIO_FIXA);
    GraphisFructus b = _pictura(c, DESCRIPTIO_FIXA);
    i32 indicum, fontis, dominorum;

    CREDO_VERUM (memcmp(a.indices, b.indices, LAT * ALT) == ZEPHYRUM);
    CREDO_VERUM (memcmp(a.tabula->fons, b.tabula->fons,
        LAT * ALT * IV) == ZEPHYRUM);
    CREDO_VERUM (memcmp(a.tabula->larva, b.tabula->larva, LAT * ALT)
        == ZEPHYRUM);
    CREDO_VERUM (memcmp(a.tabula->domini, b.tabula->domini,
        LAT * ALT) == ZEPHYRUM);
    indicum  = friatio_fnv1a_literis(
        (constans character*)a.indices, LAT * ALT);
    fontis   = friatio_fnv1a_literis(
        (constans character*)a.tabula->fons, LAT * ALT * IV);
    dominorum = friatio_fnv1a_literis(
        (constans character*)a.tabula->domini, LAT * ALT);
    si (   indicum   != DIGESTUM_INDICUM || fontis != DIGESTUM_FONTIS
        || dominorum != DIGESTUM_DOMINORUM)
    {
        printf("digesta: indicum 0x%08xu fontis 0x%08xu"
            " dominorum 0x%08xu\n", (insignatus)indicum,
            (insignatus)fontis, (insignatus)dominorum);
    }
    CREDO_AEQUALIS_I32 (indicum, DIGESTUM_INDICUM);
    CREDO_AEQUALIS_I32 (fontis, DIGESTUM_FONTIS);
    CREDO_AEQUALIS_I32 (dominorum, DIGESTUM_DOMINORUM);
    CREDO_AEQUALIS_I32 (a.tabula->titulorum, II);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "quadratum", _sectio_quadratum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exemplar_purum", _sectio_exemplar_purum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "vis", _sectio_vis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ordo", _sectio_ordo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "domini", _sectio_domini, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ovum_et_linea", _sectio_ovum_et_linea, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "praecisio", _sectio_praecisio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exemplaria", _sectio_exemplaria, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "determinata", _sectio_determinata, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("graphis", SECTIONES);
}
