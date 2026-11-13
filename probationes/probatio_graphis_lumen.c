/* probatio_graphis_lumen.c - lux picturae (graphis_lumen.h)
 *
 * graphis_illustrare super tabulas: lux plena nihil mutat, umbra
 * angulorum, lumen puncti cum radiis duobus, lumen areae cum
 * margine molli, lucentes, saturatio, summa sine ordine, argumenta
 * mala. In descriptione: attributa radicis, <lumen>, lucens,
 * recusationes cum linea, digestum fixum.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "friatio.h"
#include "graphis.h"
#include "graphis_lumen.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LAT GRAPHIS_LATITUDO
#define ALT GRAPHIS_ALTITUDO
#define UNA GRAPHIS_LUX_UNA

/* fons planus: omnia pixela (r, g, b) */
interior i8*
_fons_planus (
    CredoContextus* c,
                i8  r,
                i8  g,
                i8  b)
{
    i8* fons = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)(LAT * ALT * IV));
    i32 k;

    CREDO_NECESSE (fons != NIHIL);
    per (k = ZEPHYRUM; k < LAT * ALT; k++)
    {
        fons[k * IV]        = r;
        fons[k * IV + I]    = g;
        fons[k * IV + II]   = b;
        fons[k * IV + III]  = 255;
    }
    redde fons;
}

interior vacuum
_lux_plena (
    GraphisLux* lux)
{
    memset(lux, ZEPHYRUM, magnitudo(*lux));
    lux->lux = UNA;
    memset(lux->tinctura, 255, III);
}

interior vacuum
_lumen_puncti (
    GraphisLumen* lumen,
             s32  x,
             s32  y,
             s32  radius_x,
             s32  radius_y,
             i32  vis)
{
    memset(lumen, ZEPHYRUM, magnitudo(*lumen));
    lumen->punctum   = VERUM;
    lumen->x         = x;
    lumen->y         = y;
    lumen->radius_x  = radius_x;
    lumen->radius_y  = radius_y;
    lumen->vis       = vis;
    memset(lumen->color, 255, III);
}

/* canalis pixeli */
interior i32
_canalis (
    constans i8* rgba,
            i32  x,
            i32  y,
            i32  k)
{
    redde (i32)rgba[(y * LAT + x) * IV + k];
}

interior vacuum
_sectio_plena (
    CredoContextus* c)
{
    GraphisLux  lux;
            i8* fons = _fons_planus(c, 10, 20, 30);
            i8* exitus;
           i32  k;

    /* fons varius, non planus */
    per (k = ZEPHYRUM; k < LAT * ALT * IV; k++)
    {
        si (k % IV != III)
        {
            fons[k] = (i8)(k * VII % 256);
        }
    }
    _lux_plena(&lux);
    exitus = graphis_illustrare(fons, NIHIL, LAT, ALT, &lux,
        c->piscina);
    CREDO_NECESSE (exitus != NIHIL);
    CREDO_VERUM (exitus != fons);
    CREDO_VERUM (memcmp(exitus, fons, LAT * ALT * IV) == ZEPHYRUM);
    /* lux dimidia: dimidium, rotundatum */
    lux.lux = UNA / II;
    exitus  = graphis_illustrare(_fons_planus(c, 200, 101, 0), NIHIL,
        LAT, ALT, &lux, c->piscina);
    CREDO_NECESSE (exitus != NIHIL);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 5, 5, ZEPHYRUM), 100);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 5, 5, I), 51);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 5, 5, II), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 5, 5, III), 255);
    /* tinctura canales separatim minuit */
    lux.lux                 = UNA;
    lux.tinctura[ZEPHYRUM]  = 255;
    lux.tinctura[I]         = 128;
    lux.tinctura[II]        = ZEPHYRUM;
    exitus = graphis_illustrare(_fons_planus(c, 200, 200, 200), NIHIL,
        LAT, ALT, &lux, c->piscina);
    CREDO_NECESSE (exitus != NIHIL);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 5, 5, ZEPHYRUM), 200);
    CREDO_VERUM (_canalis(exitus, 5, 5, I) >= 100
        && _canalis(exitus, 5, 5, I) <= 101);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 5, 5, II), ZEPHYRUM);
}

interior vacuum
_sectio_umbra (
    CredoContextus* c)
{
    GraphisLux  lux;
            i8* exitus;
           i32  k;
           i32  prior;

    _lux_plena(&lux);
    lux.umbra = UNA / II;
    exitus = graphis_illustrare(_fons_planus(c, 200, 200, 200), NIHIL,
        LAT, ALT, &lux, c->piscina);
    CREDO_NECESSE (exitus != NIHIL);
    /* centrum immutatum, anguli dimidio obscuriores */
    CREDO_AEQUALIS_I32 (_canalis(exitus, 180, 120, ZEPHYRUM), 200);
    CREDO_VERUM (_canalis(exitus, 0, 0, ZEPHYRUM) >= 100
        && _canalis(exitus, 0, 0, ZEPHYRUM) <= 102);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 0, 0, ZEPHYRUM),
        _canalis(exitus, 359, 239, ZEPHYRUM));
    CREDO_AEQUALIS_I32 (_canalis(exitus, 0, 239, ZEPHYRUM),
        _canalis(exitus, 359, 0, ZEPHYRUM));
    /* medium lateris: quarta parte obscurius */
    CREDO_VERUM (_canalis(exitus, 0, 120, ZEPHYRUM) >= 149
        && _canalis(exitus, 0, 120, ZEPHYRUM) <= 152);
    /* a centro ad angulum numquam clarius */
    prior = 255;
    per (k = ZEPHYRUM; k < 120; k++)
    {
        i32 v = _canalis(exitus, 180 + k * III / II, 120 + k,
            ZEPHYRUM);

        CREDO_VERUM (v <= prior);
        prior = v;
    }
}

interior vacuum
_sectio_punctum (
    CredoContextus* c)
{
      GraphisLux  lux;
    GraphisLumen  lumen;
              i8* exitus;

    _lux_plena(&lux);
    lux.lux      = ZEPHYRUM;
    lux.lumina   = &lumen;
    lux.luminum  = I;
    _lumen_puncti(&lumen, 180, 120, 40, 20, UNA);
    exitus = graphis_illustrare(_fons_planus(c, 100, 100, 100), NIHIL,
        LAT, ALT, &lux, c->piscina);
    CREDO_NECESSE (exitus != NIHIL);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 180, 120, ZEPHYRUM), 100);
    /* ad radium unum: exp(-1/2) = 0.6065 */
    CREDO_VERUM (_canalis(exitus, 220, 120, ZEPHYRUM) >= 60
        && _canalis(exitus, 220, 120, ZEPHYRUM) <= 61);
    CREDO_VERUM (_canalis(exitus, 180, 140, ZEPHYRUM) >= 60
        && _canalis(exitus, 180, 140, ZEPHYRUM) <= 61);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 140, 120, ZEPHYRUM),
        _canalis(exitus, 220, 120, ZEPHYRUM));
    /* ad radios duos: exp(-2) = 0.135 */
    CREDO_VERUM (_canalis(exitus, 260, 120, ZEPHYRUM) >= 13
        && _canalis(exitus, 260, 120, ZEPHYRUM) <= 14);
    /* ad radios tres: exp(-9/2) = 0.011 - parum, sed non nihil */
    CREDO_AEQUALIS_I32 (_canalis(exitus, 300, 120, ZEPHYRUM), I);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 180, 180, ZEPHYRUM), I);
    /* latior secundum x */
    CREDO_VERUM (_canalis(exitus, 200, 120, ZEPHYRUM)
        > _canalis(exitus, 180, 140, ZEPHYRUM));
    /* ultra IV radios nihil */
    CREDO_AEQUALIS_I32 (_canalis(exitus, 341, 120, ZEPHYRUM),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 180, 201, ZEPHYRUM),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 0, 0, ZEPHYRUM), ZEPHYRUM);
    /* color luminis canales separat */
    lumen.color[ZEPHYRUM]  = 255;
    lumen.color[I]         = 128;
    lumen.color[II]        = ZEPHYRUM;
    exitus = graphis_illustrare(_fons_planus(c, 200, 200, 200), NIHIL,
        LAT, ALT, &lux, c->piscina);
    CREDO_NECESSE (exitus != NIHIL);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 180, 120, ZEPHYRUM), 200);
    CREDO_VERUM (_canalis(exitus, 180, 120, I) >= 100
        && _canalis(exitus, 180, 120, I) <= 101);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 180, 120, II), ZEPHYRUM);
    /* lumen extra picturam positum adhuc illustrat */
    _lumen_puncti(&lumen, -20, 120, 40, 40, UNA);
    exitus = graphis_illustrare(_fons_planus(c, 100, 100, 100), NIHIL,
        LAT, ALT, &lux, c->piscina);
    CREDO_NECESSE (exitus != NIHIL);
    CREDO_VERUM (_canalis(exitus, 0, 120, ZEPHYRUM) > 80);
}

interior vacuum
_sectio_area (
    CredoContextus* c)
{
      GraphisLux  lux;
    GraphisLumen  lumen;
             s32  area_x[IV];
             s32  area_y[IV];
              i8* exitus;

    area_x[ZEPHYRUM]  = 100;
    area_y[ZEPHYRUM]  = 50;
    area_x[I]         = 200;
    area_y[I]         = 50;
    area_x[II]        = 200;
    area_y[II]        = 150;
    area_x[III]       = 100;
    area_y[III]       = 150;
    _lux_plena(&lux);
    lux.lux      = ZEPHYRUM;
    lux.lumina   = &lumen;
    lux.luminum  = I;
    memset(&lumen, ZEPHYRUM, magnitudo(lumen));
    lumen.area_x     = area_x;
    lumen.area_y     = area_y;
    lumen.punctorum  = IV;
    lumen.vis        = UNA;
    memset(lumen.color, 255, III);
    /* margo durus */
    exitus = graphis_illustrare(_fons_planus(c, 100, 100, 100), NIHIL,
        LAT, ALT, &lux, c->piscina);
    CREDO_NECESSE (exitus != NIHIL);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 150, 100, ZEPHYRUM), 100);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 100, 100, ZEPHYRUM), 100);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 99, 100, ZEPHYRUM),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 199, 149, ZEPHYRUM), 100);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 200, 100, ZEPHYRUM),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 50, 100, ZEPHYRUM),
        ZEPHYRUM);
    /* margo mollis: pixelum bis, vicini quattuor semel, per VI */
    lumen.mollities = IV;
    exitus = graphis_illustrare(_fons_planus(c, 100, 100, 100), NIHIL,
        LAT, ALT, &lux, c->piscina);
    CREDO_NECESSE (exitus != NIHIL);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 150, 100, ZEPHYRUM), 100);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 100, 100, ZEPHYRUM), 83);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 98, 100, ZEPHYRUM), 17);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 95, 100, ZEPHYRUM),
        ZEPHYRUM);
    /* angulus intus: duo vicini extra */
    CREDO_AEQUALIS_I32 (_canalis(exitus, 100, 50, ZEPHYRUM), 67);
}

interior vacuum
_sectio_lucens (
    CredoContextus* c)
{
    GraphisLux  lux;
            i8* fons     = _fons_planus(c, 200, 150, 100);
            i8* lucentes = (i8*)piscina_allocare(c->piscina,
                (memoriae_index)(LAT * ALT));
            i8* exitus;

    CREDO_NECESSE (lucentes != NIHIL);
    memset(lucentes, ZEPHYRUM, LAT * ALT);
    lucentes[20 * LAT + 10] = I;
    lucentes[20 * LAT + 11] = 255;
    _lux_plena(&lux);
    lux.lux = UNA / IV;
    exitus  = graphis_illustrare(fons, lucentes, LAT, ALT, &lux,
        c->piscina);
    CREDO_NECESSE (exitus != NIHIL);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 10, 20, ZEPHYRUM), 200);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 10, 20, I), 150);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 10, 20, II), 100);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 11, 20, ZEPHYRUM), 200);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 12, 20, ZEPHYRUM), 50);
    CREDO_AEQUALIS_I32 (_canalis(exitus, 9, 20, ZEPHYRUM), 50);
}

interior vacuum
_sectio_summa (
    CredoContextus* c)
{
      GraphisLux  lux;
    GraphisLumen  lumina[II];
    GraphisLumen  inversa[II];
              i8* a;
              i8* b;

    _lux_plena(&lux);
    lux.lux      = UNA / IV;
    lux.lumina   = lumina;
    lux.luminum  = II;
    _lumen_puncti(&lumina[ZEPHYRUM], 100, 100, 50, 50, UNA / II);
    _lumen_puncti(&lumina[I], 150, 100, 30, 60, UNA / IV);
    a = graphis_illustrare(_fons_planus(c, 200, 200, 200), NIHIL,
        LAT, ALT, &lux, c->piscina);
    CREDO_NECESSE (a != NIHIL);
    /* 1/4 + 1/2 ad centrum primi, plus aliquid secundi */
    CREDO_VERUM (_canalis(a, 100, 100, ZEPHYRUM) > 150);
    /* ordo luminum nihil mutat */
    inversa[ZEPHYRUM]  = lumina[I];
    inversa[I]         = lumina[ZEPHYRUM];
    lux.lumina         = inversa;
    b = graphis_illustrare(_fons_planus(c, 200, 200, 200), NIHIL,
        LAT, ALT, &lux, c->piscina);
    CREDO_NECESSE (b != NIHIL);
    CREDO_VERUM (memcmp(a, b, LAT * ALT * IV) == ZEPHYRUM);
    /* saturatio: duo lumina fortia ad CCLV sistunt */
    lux.lumina = lumina;
    _lumen_puncti(&lumina[ZEPHYRUM], 100, 100, 50, 50, II * UNA);
    _lumen_puncti(&lumina[I], 100, 100, 50, 50, II * UNA);
    a = graphis_illustrare(_fons_planus(c, 200, 90, 0), NIHIL, LAT,
        ALT, &lux, c->piscina);
    CREDO_NECESSE (a != NIHIL);
    CREDO_AEQUALIS_I32 (_canalis(a, 100, 100, ZEPHYRUM), 255);
    CREDO_AEQUALIS_I32 (_canalis(a, 100, 100, I), 255);
    CREDO_AEQUALIS_I32 (_canalis(a, 100, 100, II), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_canalis(a, 100, 100, III), 255);
}

interior vacuum
_sectio_argumenta (
    CredoContextus* c)
{
      GraphisLux  lux;
    GraphisLumen  lumen;
             s32  area_x[III];
             s32  area_y[III];
              i8* fons = _fons_planus(c, 100, 100, 100);

#define NEGATUR() \
    CREDO_VERUM (graphis_illustrare(fons, NIHIL, LAT, ALT, &lux, \
        c->piscina) == NIHIL)

    _lux_plena(&lux);
    CREDO_VERUM (graphis_illustrare(NIHIL, NIHIL, LAT, ALT, &lux,
        c->piscina) == NIHIL);
    CREDO_VERUM (graphis_illustrare(fons, NIHIL, LAT, ALT, NIHIL,
        c->piscina) == NIHIL);
    CREDO_VERUM (graphis_illustrare(fons, NIHIL, LAT, ALT, &lux,
        NIHIL) == NIHIL);
    CREDO_VERUM (graphis_illustrare(fons, NIHIL, ZEPHYRUM, ALT, &lux,
        c->piscina) == NIHIL);
    CREDO_VERUM (graphis_illustrare(fons, NIHIL, LAT, ZEPHYRUM, &lux,
        c->piscina) == NIHIL);
    lux.lux = UNA + I;
    NEGATUR ();
    _lux_plena(&lux);
    lux.umbra = UNA + I;
    NEGATUR ();
    _lux_plena(&lux);
    lux.luminum = I;                 /* lumina NIHIL */
    NEGATUR ();
    lux.lumina   = &lumen;
    lux.luminum  = GRAPHIS_LUMINA_MAXIMA + I;
    NEGATUR ();
    lux.luminum = I;
    _lumen_puncti(&lumen, 10, 10, ZEPHYRUM, 10, UNA);
    NEGATUR ();
    _lumen_puncti(&lumen, 10, 10, 10, -I, UNA);
    NEGATUR ();
    _lumen_puncti(&lumen, 10, 10, 4097, 10, UNA);
    NEGATUR ();
    _lumen_puncti(&lumen, 10, 10, 10, 10, II * UNA + I);
    NEGATUR ();
    memset(&lumen, ZEPHYRUM, magnitudo(lumen));
    area_x[ZEPHYRUM]  = ZEPHYRUM;
    area_y[ZEPHYRUM]  = ZEPHYRUM;
    area_x[I]         = 50;
    area_y[I]         = ZEPHYRUM;
    area_x[II]        = ZEPHYRUM;
    area_y[II]        = 50;
    lumen.area_x      = area_x;
    lumen.area_y      = area_y;
    lumen.punctorum   = II;
    lumen.vis         = UNA;
    NEGATUR ();
    lumen.punctorum = GRAPHIS_AREA_PUNCTA + I;
    NEGATUR ();
    lumen.punctorum = III;
    lumen.mollities = GRAPHIS_MOLLITIES_MAXIMA + I;
    NEGATUR ();
    lumen.mollities  = ZEPHYRUM;
    lumen.area_x     = NIHIL;
    NEGATUR ();
    /* ... et bona accipiuntur */
    lumen.area_x = area_x;
    CREDO_VERUM (graphis_illustrare(fons, NIHIL, LAT, ALT, &lux,
        c->piscina) != NIHIL);
#undef NEGATUR
}

interior GraphisFructus
_lege (
        CredoContextus* c,
    constans character* literae)
{
    redde graphis_legere(chorda_ex_literis(literae, c->piscina),
        c->piscina);
}

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
    CREDO_NECESSE (f.illustratus != NIHIL);
    redde f;
}

#define RECUSATUR(literae, linea_sperata, fragmentum) \
    fac { \
        GraphisFructus rf = _lege(c, (literae)); \
        CREDO_FALSUM (rf.successus); \
        CREDO_VERUM (rf.tabula == NIHIL); \
        CREDO_VERUM (rf.illustratus == NIHIL); \
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

interior vacuum
_sectio_descriptio (
    CredoContextus* c)
{
            GraphisFructus  f;
     constans GraphisLumen* l;

    /* sine luce descripta: illustratus = fons */
    f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"0 0 100 100\" colores=\"#c8c8c8\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (f.tabula->lux.lux, UNA);
    CREDO_AEQUALIS_I32 (f.tabula->lux.umbra, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (f.tabula->lux.luminum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (f.tabula->lux.tinctura[I], 255);
    CREDO_VERUM (memcmp(f.illustratus, f.tabula->fons,
        LAT * ALT * IV) == ZEPHYRUM);
    /* lux dimidia; forma lucens immutata; forma posterior non
     * lucens lucentem tegit */
    f = _pictura(c,
        "<pictura lux=\"0.5\" umbra=\"0\" tinctura=\"#ffffff\">\n"
        "<rectangulum fines=\"0 0 100 100\" colores=\"#c8c8c8\"/>\n"
        "<rectangulum fines=\"200 0 100 100\" colores=\"#c8c8c8\""
        " lucens=\"verum\"/>\n"
        "<rectangulum fines=\"250 0 50 100\" colores=\"#c8c8c8\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (f.tabula->lux.lux, UNA / II);
    CREDO_AEQUALIS_I32 (_canalis(f.tabula->fons, 50, 50, ZEPHYRUM),
        200);
    CREDO_AEQUALIS_I32 (_canalis(f.illustratus, 50, 50, ZEPHYRUM),
        100);
    CREDO_AEQUALIS_I32 (_canalis(f.illustratus, 220, 50, ZEPHYRUM),
        200);
    CREDO_AEQUALIS_I32 (_canalis(f.illustratus, 270, 50, ZEPHYRUM),
        100);
    CREDO_VERUM (f.tabula->lucentes[50 * LAT + 220] != ZEPHYRUM);
    CREDO_VERUM (f.tabula->lucentes[50 * LAT + 270] == ZEPHYRUM);
    CREDO_VERUM (f.tabula->lucentes[50 * LAT + 50] == ZEPHYRUM);
    /* lumina leguntur ut scripta */
    f = _pictura(c,
        "<pictura lux=\"0.25\" umbra=\"0.35\" tinctura=\"#d1dcff\">\n"
        "<lumen locus=\"170 140\" radii=\"75 58\" color=\"#ffb873\""
        " vis=\"0.85\"/>\n"
        "<rectangulum fines=\"0 0 360 240\" colores=\"#c8c8c8\"/>\n"
        "<lumen area=\"214,156 286,156 322,198 226,198\""
        " mollities=\"3\" color=\"#fff0d8\" vis=\"1.5\"/>\n"
        "<lumen locus=\"-5 300\" radii=\"10 10\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (f.tabula->lux.lux, UNA / IV);
    CREDO_AEQUALIS_I32 (f.tabula->lux.umbra, 1434);
    CREDO_AEQUALIS_I32 (f.tabula->lux.tinctura[ZEPHYRUM], 0xd1);
    CREDO_AEQUALIS_I32 (f.tabula->lux.tinctura[II], 0xff);
    CREDO_NECESSE (f.tabula->lux.luminum == III);
    l = &f.tabula->lux.lumina[ZEPHYRUM];
    CREDO_VERUM (l->punctum);
    CREDO_AEQUALIS_S32 (l->x, 170);
    CREDO_AEQUALIS_S32 (l->y, 140);
    CREDO_AEQUALIS_S32 (l->radius_x, 75);
    CREDO_AEQUALIS_S32 (l->radius_y, 58);
    CREDO_AEQUALIS_I32 (l->vis, 3482);
    CREDO_AEQUALIS_I32 (l->color[I], 0xb8);
    l = &f.tabula->lux.lumina[I];
    CREDO_FALSUM (l->punctum);
    CREDO_AEQUALIS_I32 (l->punctorum, IV);
    CREDO_AEQUALIS_S32 (l->area_x[II], 322);
    CREDO_AEQUALIS_S32 (l->area_y[III], 198);
    CREDO_AEQUALIS_I32 (l->mollities, III);
    CREDO_AEQUALIS_I32 (l->vis, UNA + UNA / II);
    l = &f.tabula->lux.lumina[II];
    CREDO_AEQUALIS_I32 (l->vis, UNA);
    CREDO_AEQUALIS_I32 (l->color[ZEPHYRUM], 255);
    CREDO_AEQUALIS_S32 (l->x, -5);
    /* lumen pixela non possidet nec pingit */
    CREDO_AEQUALIS_I32 (f.tabula->titulorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_canalis(f.tabula->fons, 170, 140, ZEPHYRUM),
        200);
    /* prope lumen clarius quam in angulo */
    CREDO_VERUM (_canalis(f.illustratus, 170, 140, ZEPHYRUM)
        > _canalis(f.illustratus, 5, 5, ZEPHYRUM) + 100);
}

#define CAPUT "<pictura>\n"
#define CAUDA "\n</pictura>"
#define FORMA "<rectangulum fines=\"0 0 9 9\" colores=\"6\"/>\n"

interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    character longa[IV * M];
          i32 k;
          i32 n;

    RECUSATUR ("<pictura lux=\"1.5\">\n" FORMA "</pictura>", I,
        "lux");
    RECUSATUR ("<pictura lux=\"0.1234\">\n" FORMA "</pictura>", I,
        "lux");
    RECUSATUR ("<pictura lux=\"x\">\n" FORMA "</pictura>", I, "lux");
    RECUSATUR ("<pictura umbra=\"2\">\n" FORMA "</pictura>", I,
        "umbra");
    RECUSATUR ("<pictura tinctura=\"#12\">\n" FORMA "</pictura>", I,
        "tinctura");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5 5\" radii=\"0 5\"/>"
        CAUDA, III, "radii");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5 5\" radii=\"5 -5\"/>"
        CAUDA, III, "radii");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5 5\" radii=\"5\"/>" CAUDA,
        III, "radii");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5 5\"/>" CAUDA, III,
        "radii");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5\" radii=\"5 5\"/>" CAUDA,
        III, "locus");
    RECUSATUR (CAPUT FORMA "<lumen radii=\"5 5\"/>" CAUDA, III,
        "locus or area");
    RECUSATUR (CAPUT FORMA "<lumen/>" CAUDA, III, "locus or area");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5 5\" radii=\"5 5\""
        " area=\"0,0 9,0 0,9\"/>" CAUDA, III, "locus or area");
    RECUSATUR (CAPUT FORMA "<lumen area=\"0,0 9,0 0,9\""
        " radii=\"5 5\"/>" CAUDA, III, "radii");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5 5\" radii=\"5 5\""
        " mollities=\"2\"/>" CAUDA, III, "mollities");
    RECUSATUR (CAPUT FORMA "<lumen area=\"0,0 9,0\"/>" CAUDA, III,
        "area");
    RECUSATUR (CAPUT FORMA "<lumen area=\"0,0 9,0 0,9\""
        " mollities=\"17\"/>" CAUDA, III, "mollities");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5 5\" radii=\"5 5\""
        " vis=\"2.5\"/>" CAUDA, III, "vis");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5 5\" radii=\"5 5\""
        " vis=\"0.5555\"/>" CAUDA, III, "vis");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5 5\" radii=\"5 5\""
        " color=\"16\"/>" CAUDA, III, "color");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5 5\" radii=\"5 5\""
        " titulus=\"a\"/>" CAUDA, III, "unknown attribute 'titulus'");
    RECUSATUR (CAPUT FORMA "<lumen locus=\"5 5\" radii=\"5 5\">"
        FORMA "</lumen>" CAUDA, III, "children");
    RECUSATUR (CAPUT FORMA "<grex>\n<lumen locus=\"5 5\""
        " radii=\"5 5\"/>\n</grex>" CAUDA, IV, "<pictura>");
    RECUSATUR (CAPUT "<rectangulum fines=\"0 0 9 9\" colores=\"6\""
        " lucens=\"falsum\"/>" CAUDA, II, "lucens");
    /* lumina plus quam XVI */
    n = (i32)sprintf(longa, "<pictura>\n" FORMA);
    per (k = ZEPHYRUM; k < GRAPHIS_LUMINA_MAXIMA + I; k++)
    {
        n += (i32)sprintf(longa + n,
            "<lumen locus=\"5 5\" radii=\"5 5\"/>\n");
    }
    (vacuum)sprintf(longa + n, "</pictura>");
    RECUSATUR (longa, GRAPHIS_LUMINA_MAXIMA + III, "16");
}

#define DESCRIPTIO_FIXA \
    "<pictura titulus=\"lux\" lux=\"0.55\" tinctura=\"#d1dcff\"" \
    " umbra=\"0.35\">\n" \
    "<rectangulum fines=\"0 0 360 150\" colores=\"9 15\"" \
    " exemplar=\"horizontalis_latus\" vis=\"0.45\"/>\n" \
    "<rectangulum fines=\"0 150 360 90\" colores=\"9 3\"" \
    " exemplar=\"margo_sinister\" vis=\"0.4\"/>\n" \
    "<rectangulum titulus=\"fenestra\" fines=\"222 54 58 50\"" \
    " colores=\"5 7\" exemplar=\"cinereum_leve\" vis=\"1\"" \
    " stilus=\"3 1\" lucens=\"verum\"/>\n" \
    "<lumen locus=\"170 140\" radii=\"75 58\" color=\"#ffb873\"" \
    " vis=\"0.85\"/>\n" \
    "<lumen area=\"214,156 286,156 322,198 226,198\"" \
    " mollities=\"3\" color=\"#fff0d8\" vis=\"0.55\"/>\n" \
    "</pictura>"

#define DIGESTUM_ILLUSTRATI  0x0a9cdc6eu
#define DIGESTUM_INDICUM     0x6e4c708au

interior vacuum
_sectio_digestum (
    CredoContextus* c)
{
    GraphisFructus a = _pictura(c, DESCRIPTIO_FIXA);
    GraphisFructus b = _pictura(c, DESCRIPTIO_FIXA);
               i32 illustrati;
               i32 indicum;

    CREDO_VERUM (memcmp(a.illustratus, b.illustratus, LAT * ALT * IV)
        == ZEPHYRUM);
    CREDO_VERUM (memcmp(a.indices, b.indices, LAT * ALT) == ZEPHYRUM);
    illustrati = friatio_fnv1a_literis(
        (constans character*)a.illustratus, LAT * ALT * IV);
    indicum    = friatio_fnv1a_literis(
        (constans character*)a.indices, LAT * ALT);
    si (   illustrati != DIGESTUM_ILLUSTRATI
        || indicum    != DIGESTUM_INDICUM)
    {
        printf("digesta: illustrati 0x%08xu indicum 0x%08xu\n",
            (insignatus)illustrati, (insignatus)indicum);
    }
    CREDO_AEQUALIS_I32 (illustrati, DIGESTUM_ILLUSTRATI);
    CREDO_AEQUALIS_I32 (indicum, DIGESTUM_INDICUM);
    /* fenestra lucens: fons eius immutatus manet */
    CREDO_AEQUALIS_I32 (_canalis(a.illustratus, 240, 70, ZEPHYRUM),
        _canalis(a.tabula->fons, 240, 70, ZEPHYRUM));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "plena", _sectio_plena, NIHIL, NIHIL, NIHIL, NIHIL },
    { "umbra", _sectio_umbra, NIHIL, NIHIL, NIHIL, NIHIL },
    { "punctum", _sectio_punctum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "area", _sectio_area, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lucens", _sectio_lucens, NIHIL, NIHIL, NIHIL, NIHIL },
    { "summa", _sectio_summa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "argumenta", _sectio_argumenta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "descriptio", _sectio_descriptio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "digestum", _sectio_digestum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("graphis_lumen", SECTIONES);
}
