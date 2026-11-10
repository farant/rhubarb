/* probatio_dithering_ordinatum.c - dithering sine diffusione
 * (dithering_ordinatum.h): dithering_proximus_colorum et
 * dithering_bayer_colorum
 *
 * proximus: color palettae se ipsum reddit; inter nigrum et album
 * propior vincit; sine coloribus activis deficit. bayer: color
 * palettae purus manet; medium inter duos colores dimidium exacte
 * dat, quarta pars quartam; gradatio crescit; craticula ad originem
 * imaginis ancorata (motus VIII pixelorum exitum movet, motus I
 * non); color activus unus solus redditur; digesta in fixo (imago
 * DC lata, colorum distinctorum plus quam tabula consiliorum capit:
 * via sine tabula quoque probatur).
 */
#include "latina.h"
#include "piscina.h"
#include "imago.h"
#include "sigillum.h"
#include "dithering.h"
#include "dithering_ordinatum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define NIGER  ZEPHYRUM
#define ALBUS  V

#define VIA_FIXI "probationes/fixa/exemplaria/lapis_600.png"

/* digesta (SHA-256 indicum) lapidis DC, omnibus coloribus activis -
 * ex hac implementatione fixa 2026-11-09 */
#define DIGESTUM_BAYER \
    "b99d5056cefe0299c619542b2ce435c459cb90651a43cc946284e3d10ef95d98"
#define DIGESTUM_PROXIMI \
    "b8f8e08ae2226b53d61b33be8d3beced41a415fb37f7c9d0c97e625f2c7685d6"

/* imago plana coloris unius */
interior i8*
_planum (
    CredoContextus* c,
               i32  latitudo,
               i32  altitudo,
               i32  r,
               i32  g,
               i32  b)
{
    i8* p = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)latitudo * altitudo * IV);
    i32 k;

    per (k = ZEPHYRUM; k < latitudo * altitudo; k++)
    {
        p[k * IV]        = (i8)r;
        p[k * IV + I]    = (i8)g;
        p[k * IV + II]   = (i8)b;
        p[k * IV + III]  = 255;
    }
    redde p;
}

interior vacuum
_duo_activi (
    b32 activi[XVI])
{
    memset(activi, ZEPHYRUM, XVI * magnitudo(b32));
    activi[NIGER] = VERUM;
    activi[ALBUS] = VERUM;
}

/* quot pixela indicem datum habent in rectangulo */
interior i32
_numerare (
    constans DitheringFructus* f,
                          i32  x0,
                          i32  y0,
                          i32  latitudo,
                          i32  altitudo,
                          i32  index)
{
    i32 n = ZEPHYRUM;
    i32 x;
    i32 y;

    per (y = y0; y < y0 + altitudo; y++)
    {
        per (x = x0; x < x0 + latitudo; x++)
        {
            n += f->indices[y * f->latitudo + x] == index ? I
                                                          : ZEPHYRUM;
        }
    }
    redde n;
}

interior vacuum
_sectio_proximus (
    CredoContextus* c)
{
    DitheringFructus f;
                 b32 activi[XVI];
                  i8 palette[XVI * IV];
                 i32 k;

    dithering_praeparare_omnes(activi);
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        palette[k * IV]        = AQUINAS_PALETTE[k][ZEPHYRUM];
        palette[k * IV + I]    = AQUINAS_PALETTE[k][I];
        palette[k * IV + II]   = AQUINAS_PALETTE[k][II];
        palette[k * IV + III]  = 255;
    }
    f = dithering_proximus_colorum(palette, XVI, I, activi, c->piscina);
    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.latitudo, XVI);
    CREDO_AEQUALIS_I32 (f.altitudo, I);
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        CREDO_AEQUALIS_I32 (f.indices[k], k);
    }
    /* niger et albus soli: propior vincit */
    _duo_activi(activi);
    f = dithering_proximus_colorum(_planum(c, IV, IV, 100, 100, 100),
        IV, IV, activi, c->piscina);
    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (_numerare(&f, ZEPHYRUM, ZEPHYRUM, IV, IV,
        NIGER), XVI);
    f = dithering_proximus_colorum(_planum(c, IV, IV, 200, 200, 200),
        IV, IV, activi, c->piscina);
    CREDO_AEQUALIS_I32 (_numerare(&f, ZEPHYRUM, ZEPHYRUM, IV, IV,
        ALBUS), XVI);
    /* sine coloribus activis, sine imagine, mensura 0: deficit */
    memset(activi, ZEPHYRUM, magnitudo(activi));
    f = dithering_proximus_colorum(palette, XVI, I, activi, c->piscina);
    CREDO_FALSUM (f.successus);
    f = dithering_bayer_colorum(palette, XVI, I, activi, c->piscina);
    CREDO_FALSUM (f.successus);
    dithering_praeparare_omnes(activi);
    f = dithering_proximus_colorum(NIHIL, XVI, I, activi, c->piscina);
    CREDO_FALSUM (f.successus);
    f = dithering_bayer_colorum(NIHIL, XVI, I, activi, c->piscina);
    CREDO_FALSUM (f.successus);
    f = dithering_bayer_colorum(palette, ZEPHYRUM, I, activi,
        c->piscina);
    CREDO_FALSUM (f.successus);
    f = dithering_proximus_colorum(palette, XVI, I, activi, NIHIL);
    CREDO_FALSUM (f.successus);
}

interior vacuum
_sectio_bayer_purus (
    CredoContextus* c)
{
    DitheringFructus f;
                 b32 activi[XVI];
                 i32 k;

    dithering_praeparare_omnes(activi);
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        f = dithering_bayer_colorum(_planum(c, XVI, XVI,
            AQUINAS_PALETTE[k][ZEPHYRUM], AQUINAS_PALETTE[k][I],
            AQUINAS_PALETTE[k][II]), XVI, XVI, activi, c->piscina);
        CREDO_NECESSE (f.successus);
        CREDO_AEQUALIS_I32 (_numerare(&f, ZEPHYRUM, ZEPHYRUM, XVI, XVI,
            k), CCLVI);
    }
    /* color activus unus: solus redditur, imagine quavis */
    memset(activi, ZEPHYRUM, magnitudo(activi));
    activi[VII] = VERUM;
    f = dithering_bayer_colorum(_planum(c, XVI, XVI, 10, 200, 90), XVI,
        XVI, activi, c->piscina);
    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (_numerare(&f, ZEPHYRUM, ZEPHYRUM, XVI, XVI,
        VII), CCLVI);
    f = dithering_proximus_colorum(_planum(c, XVI, XVI, 10, 200, 90),
        XVI, XVI, activi, c->piscina);
    CREDO_AEQUALIS_I32 (_numerare(&f, ZEPHYRUM, ZEPHYRUM, XVI, XVI,
        VII), CCLVI);
}

interior vacuum
_sectio_bayer_portiones (
    CredoContextus* c)
{
    DitheringFructus f;
                 b32 activi[XVI];
                 i32 x;
                 i32 y;
                 i32 n;

    _duo_activi(activi);
    /* medium exactum inter nigrum (0,0,0) et album (FC,F0,E8):
     * dimidium cuiusque tegulae VIII x VIII, nullum quadratum II x II
     * uniforme */
    f = dithering_bayer_colorum(_planum(c, XVI, XVI, 0x7E, 0x78, 0x74),
        XVI, XVI, activi, c->piscina);
    CREDO_NECESSE (f.successus);
    per (y = ZEPHYRUM; y < XVI; y += VIII)
    {
        per (x = ZEPHYRUM; x < XVI; x += VIII)
        {
            CREDO_AEQUALIS_I32 (_numerare(&f, x, y, VIII, VIII, ALBUS),
                XXXII);
            CREDO_AEQUALIS_I32 (_numerare(&f, x, y, VIII, VIII, NIGER),
                XXXII);
        }
    }
    per (y = ZEPHYRUM; y + I < XVI; y++)
    {
        per (x = ZEPHYRUM; x + I < XVI; x++)
        {
            n = _numerare(&f, x, y, II, II, ALBUS);
            si (n == ZEPHYRUM || n == IV)
            {
                CREDO_AEQUALIS_I32 (x * 100 + y, 0xFFFF);
                redde;
            }
        }
    }
    /* quarta pars (3F,3C,3A): XVI ex LXIV albi */
    f = dithering_bayer_colorum(_planum(c, XVI, XVI, 0x3F, 0x3C, 0x3A),
        XVI, XVI, activi, c->piscina);
    CREDO_AEQUALIS_I32 (_numerare(&f, ZEPHYRUM, ZEPHYRUM, VIII, VIII,
        ALBUS), XVI);
    CREDO_AEQUALIS_I32 (_numerare(&f, VIII, VIII, VIII, VIII, ALBUS),
        XVI);
    /* tres quartae (BD,B4,AE): XLVIII ex LXIV albi */
    f = dithering_bayer_colorum(_planum(c, XVI, XVI, 0xBD, 0xB4, 0xAE),
        XVI, XVI, activi, c->piscina);
    CREDO_AEQUALIS_I32 (_numerare(&f, ZEPHYRUM, ZEPHYRUM, VIII, VIII,
        ALBUS), XLVIII);
}

interior vacuum
_sectio_bayer_gradatio (
    CredoContextus* c)
{
    DitheringFructus  f;
                 b32  activi[XVI];
                  i8* p;
                 i32  x;
                 i32  y;
                 i32  prior = ZEPHYRUM;
                 i32  n;

    /* gradatio horizontalis nigri ad album, LXIV x XVI */
    p = (i8*)piscina_allocare(c->piscina, LXIV * XVI * IV);
    per (y = ZEPHYRUM; y < XVI; y++)
    {
        per (x = ZEPHYRUM; x < LXIV; x++)
        {
            p[(y * LXIV + x) * IV]        = (i8)(0xFC * x / LXIII);
            p[(y * LXIV + x) * IV + I]    = (i8)(0xF0 * x / LXIII);
            p[(y * LXIV + x) * IV + II]   = (i8)(0xE8 * x / LXIII);
            p[(y * LXIV + x) * IV + III]  = 255;
        }
    }
    _duo_activi(activi);
    f = dithering_bayer_colorum(p, LXIV, XVI, activi, c->piscina);
    CREDO_NECESSE (f.successus);
    per (x = ZEPHYRUM; x < LXIV; x += VIII)
    {
        n = _numerare(&f, x, ZEPHYRUM, VIII, XVI, ALBUS);
        CREDO_VERUM (n >= prior);
        prior = n;
    }
    CREDO_AEQUALIS_I32 (_numerare(&f, ZEPHYRUM, ZEPHYRUM, I, XVI,
        ALBUS), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerare(&f, LXIII, ZEPHYRUM, I, XVI, ALBUS),
        XVI);
    /* fascia prima paene nigra, ultima paene alba */
    CREDO_VERUM (_numerare(&f, ZEPHYRUM, ZEPHYRUM, VIII, XVI, ALBUS)
        < XVI);
    CREDO_VERUM (_numerare(&f, LVI, ZEPHYRUM, VIII, XVI, ALBUS)
        > CXII);
}

/* imago tonorum mediorum determinata (generator linearis) cum
 * margine sinistro 'motus' pixelorum albo */
interior i8*
_imago_mota (
    CredoContextus* c,
               i32  latitudo,
               i32  altitudo,
               i32  motus)
{
     i8* p = _planum(c, latitudo + motus, altitudo, 0xFC, 0xF0, 0xE8);
    i32  semen = 2463534242u;
    i32  x;
    i32  y;
    i32  k;

    per (y = ZEPHYRUM; y < altitudo; y++)
    {
        per (x = ZEPHYRUM; x < latitudo; x++)
        {
            per (k = ZEPHYRUM; k < III; k++)
            {
                semen = semen * 1664525u + 1013904223u;
                p[(y * (latitudo + motus) + x + motus) * IV + k] =
                    (i8)(64 + ((semen >> XVI) & 0x7F));
            }
        }
    }
    redde p;
}

interior vacuum
_sectio_bayer_ancora (
    CredoContextus* c)
{
    DitheringFructus f0;
    DitheringFructus f8;
    DitheringFructus f1;
                 b32 activi[XVI];
                 i32 x;
                 i32 y;
                 i32 diversa_viii  = ZEPHYRUM;
                 i32 diversa_i     = ZEPHYRUM;

    dithering_praeparare_omnes(activi);
    f0 = dithering_bayer_colorum(_imago_mota(c, XXXII, XXIV, ZEPHYRUM),
        XXXII, XXIV, activi, c->piscina);
    f8 = dithering_bayer_colorum(_imago_mota(c, XXXII, XXIV, VIII),
        XL, XXIV, activi, c->piscina);
    f1 = dithering_bayer_colorum(_imago_mota(c, XXXII, XXIV, I),
        XXXIII, XXIV, activi, c->piscina);
    CREDO_NECESSE (f0.successus && f8.successus && f1.successus);
    per (y = ZEPHYRUM; y < XXIV; y++)
    {
        per (x = ZEPHYRUM; x < XXXII; x++)
        {
            si (f8.indices[y * XL + x + VIII]
                != f0.indices[y * XXXII + x])
            {
                diversa_viii++;
            }
            si (f1.indices[y * XXXIII + x + I]
                != f0.indices[y * XXXII + x])
            {
                diversa_i++;
            }
        }
    }
    /* VIII pixelis mota: exitus idem motus; I pixelo: alius */
    CREDO_AEQUALIS_I32 (diversa_viii, ZEPHYRUM);
    CREDO_VERUM (diversa_i > XXXII);
    /* imago vere mixta est (non unus color) */
    CREDO_VERUM (_numerare(&f0, ZEPHYRUM, ZEPHYRUM, XXXII, XXIV,
        f0.indices[ZEPHYRUM]) < XXXII * XXIV / II);
    /* determinatum */
    f1 = dithering_bayer_colorum(_imago_mota(c, XXXII, XXIV, ZEPHYRUM),
        XXXII, XXIV, activi, c->piscina);
    CREDO_VERUM (memcmp(f0.indices, f1.indices, XXXII * XXIV)
        == ZEPHYRUM);
}

interior vacuum
_sectio_fixa (
    CredoContextus* c)
{
        ImagoFructus im;
    DitheringFructus f;
            Sigillum s;
                 b32 activi[XVI];
           character hex[SIGILLUM_HEX_MENSURA];
                 i32 k;
                 i32 usi = ZEPHYRUM;
                 b32 visus[XVI];

    im = imago_caricare_ex_file(VIA_FIXI, c->piscina);
    CREDO_NECESSE (im.successus);
    dithering_praeparare_omnes(activi);
    f = dithering_bayer_colorum(im.imago.pixela, im.imago.latitudo,
        im.imago.altitudo, activi, c->piscina);
    CREDO_NECESSE (f.successus);
    s = sigillum_computare(f.indices,
        (memoriae_index)f.latitudo * f.altitudo);
    sigillum_hex(&s, hex);
    si (strcmp(hex, DIGESTUM_BAYER) != ZEPHYRUM)
    {
        printf("  digestum bayer: %s\n", hex);
    }
    CREDO_VERUM (strcmp(hex, DIGESTUM_BAYER) == ZEPHYRUM);
    /* imago vera plures colores adhibet */
    memset(visus, ZEPHYRUM, magnitudo(visus));
    per (k = ZEPHYRUM; k < f.latitudo * f.altitudo; k++)
    {
        visus[f.indices[k] & XV]  = VERUM;
        usi                       += f.indices[k] < XVI ? ZEPHYRUM : I;
    }
    CREDO_AEQUALIS_I32 (usi, ZEPHYRUM);
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        usi += visus[k] ? I : ZEPHYRUM;
    }
    CREDO_VERUM (usi >= VI);
    f = dithering_proximus_colorum(im.imago.pixela, im.imago.latitudo,
        im.imago.altitudo, activi, c->piscina);
    CREDO_NECESSE (f.successus);
    s = sigillum_computare(f.indices,
        (memoriae_index)f.latitudo * f.altitudo);
    sigillum_hex(&s, hex);
    si (strcmp(hex, DIGESTUM_PROXIMI) != ZEPHYRUM)
    {
        printf("  digestum proximi: %s\n", hex);
    }
    CREDO_VERUM (strcmp(hex, DIGESTUM_PROXIMI) == ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "proximus", _sectio_proximus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bayer_purus", _sectio_bayer_purus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bayer_portiones", _sectio_bayer_portiones, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "bayer_gradatio", _sectio_bayer_gradatio, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "bayer_ancora", _sectio_bayer_ancora, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fixa", _sectio_fixa, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("dithering_ordinatum", SECTIONES);
}
