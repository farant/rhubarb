/* probatio_statuae_quadrum.c - quadrum statuae (statuae_quadrum.h)
 *
 * modi: tituli eunt et redeunt, ignotus recusatur. quadrum: quisque
 * modus imaginem dat; pixela tecta opaca et (praeter crudum) colores
 * palettae; charta munda - opaca Albus aut perlucida - exacte ubi
 * pictor non texit. crudum picturam ipsam reddit. tempus et modus
 * quadrum mutant; eadem petitio idem dat. vacua et errores. fines:
 * rectangulum tecti (margo, reticulum, coactio); exemplaria intra
 * fines solum - pixela tecta eadem ac praecisio manu facta.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "dithering.h"
#include "dithering_exemplaria_velox.h"
#include "stereometria.h"
#include "statua.h"
#include "statua_glb.h"
#include "statua_pictura.h"
#include "statuae_quadrum.h"
#include "credo.h"
#include <string.h>

#define VIA_LAPIDIS "probationes/fixa/statuae/lapis.glb"
#define LATITUDO CXX
#define ALTITUDO CL

interior Statua*
_lapis (
    CredoContextus* c)
{
    StatuaFructus f = statua_glb_legere(
        filum_legere_totum(VIA_LAPIDIS, c->piscina), c->piscina);

    redde f.successus ? f.statua : NIHIL;
}

interior StatuaeQuadrumPetitio
_petitio (
    StatuaeModus modus,
             b32 perlucida)
{
    StatuaeQuadrumPetitio p;

    memset(&p, ZEPHYRUM, magnitudo(p));
    p.cyclus             = ZEPHYRUM;
    p.tempus             = 0.35f;
    p.visus.gyrus        = STATUA_GYRUS_ORDINARIUS;
    p.visus.inclinatio   = STATUA_INCLINATIO_ORDINARIA;
    p.visus.illuminatum  = VERUM;
    p.modus              = modus;
    p.latitudo           = LATITUDO;
    p.altitudo           = ALTITUDO;
    p.charta_perlucida   = perlucida;
    redde p;
}

/* estne pixelum color palettae (index redditus) aut -1 */
interior s32
_index_palettae (
    constans i8* pixelum)
{
    s32 k;

    per (k = ZEPHYRUM; k < XVI; k++)
    {
        si (   pixelum[ZEPHYRUM] == AQUINAS_PALETTE[k][ZEPHYRUM]
            && pixelum[I]        == AQUINAS_PALETTE[k][I]
            && pixelum[II]       == AQUINAS_PALETTE[k][II])
        {
            redde k;
        }
    }
    redde -I;
}

interior vacuum
_sectio_modi (
    CredoContextus* c)
{
    StatuaeModus m;
             s32 k;

    per (k = ZEPHYRUM; k < (s32)STATUAE_MODORUM; k++)
    {
        CREDO_NECESSE (statuae_modi_titulus((StatuaeModus)k) != NIHIL);
        m = STATUAE_MODORUM;
        CREDO_VERUM (statuae_modum_legere(chorda_ex_literis(
            statuae_modi_titulus((StatuaeModus)k), c->piscina), &m));
        CREDO_AEQUALIS_S32 ((s32)m, k);
    }
    CREDO_VERUM (strcmp(statuae_modi_titulus(STATUAE_MODUS_ATKINSON),
        "atkinson") == ZEPHYRUM);
    CREDO_VERUM (strcmp(statuae_modi_titulus(STATUAE_MODUS_BAYER),
        "bayer") == ZEPHYRUM);
    CREDO_VERUM (strcmp(statuae_modi_titulus(STATUAE_MODUS_EXEMPLARIA),
        "exemplaria") == ZEPHYRUM);
    CREDO_VERUM (strcmp(statuae_modi_titulus(STATUAE_MODUS_NULLUS),
        "nullum") == ZEPHYRUM);
    CREDO_VERUM (strcmp(statuae_modi_titulus(STATUAE_MODUS_CRUDUS),
        "crudum") == ZEPHYRUM);
    CREDO_VERUM (statuae_modi_titulus(STATUAE_MODORUM) == NIHIL);
    m = STATUAE_MODUS_BAYER;
    CREDO_FALSUM (statuae_modum_legere(chorda_ex_literis("nemo",
        c->piscina), &m));
    CREDO_FALSUM (statuae_modum_legere(chorda_ex_literis("",
        c->piscina), &m));
    CREDO_FALSUM (statuae_modum_legere(chorda_ex_literis("Bayer",
        c->piscina), NIHIL));
    CREDO_AEQUALIS_S32 ((s32)m, (s32)STATUAE_MODUS_BAYER);
}

interior vacuum
_sectio_quadrum (
    CredoContextus* c)
{
                   Statua* s = _lapis(c);
       StereometriaMatrix* matrices;
            StatuaPictura  pictura;
    StatuaeQuadrumPetitio  p;
                    Imago  opaca;
                    Imago  perlucida;
                      s32  modus;
                      i32  k;
                      i32  mala;
                      i32  tecta;

    CREDO_NECESSE (s != NIHIL);
    /* tectum verum ex pictore ipso */
    p         = _petitio(STATUAE_MODUS_CRUDUS, FALSUM);
    matrices  = (StereometriaMatrix*)piscina_allocare(c->piscina,
        s->nodorum * magnitudo(StereometriaMatrix));
    statua_positura(s, p.cyclus, p.tempus, matrices);
    CREDO_NECESSE (statua_pingere(s, matrices, &p.visus, LATITUDO,
        ALTITUDO, &pictura, c->piscina));
    tecta = ZEPHYRUM;
    per (k = ZEPHYRUM; k < LATITUDO * ALTITUDO; k++)
    {
        tecta += pictura.tectum[k] ? I : ZEPHYRUM;
    }
    CREDO_VERUM (tecta > LATITUDO * ALTITUDO / XX);
    per (modus = ZEPHYRUM; modus < (s32)STATUAE_MODORUM; modus++)
    {
        p = _petitio((StatuaeModus)modus, FALSUM);
        CREDO_NECESSE (statuae_quadrum_pingere(s, &p, &opaca,
            c->piscina));
        p.charta_perlucida = VERUM;
        CREDO_NECESSE (statuae_quadrum_pingere(s, &p, &perlucida,
            c->piscina));
        CREDO_AEQUALIS_I32 (opaca.latitudo, LATITUDO);
        CREDO_AEQUALIS_I32 (opaca.altitudo, ALTITUDO);
        CREDO_AEQUALIS_I32 (perlucida.latitudo, LATITUDO);
        mala = ZEPHYRUM;
        per (k = ZEPHYRUM; k < LATITUDO * ALTITUDO; k++)
        {
            constans i8* o = opaca.pixela + k * IV;
            constans i8* l = perlucida.pixela + k * IV;

            /* opaca: semper alpha plena; charta = Albus */
            si (o[III] != 255)
            {
                mala++;
            }
            si (   !pictura.tectum[k]
                && _index_palettae(o) != STATUAE_CHARTA_INDEX)
            {
                mala++;
            }
            /* perlucida: alpha 0 exacte ubi pictor non texit */
            si ((l[III] == ZEPHYRUM) != (pictura.tectum[k] == ZEPHYRUM))
            {
                mala++;
            }
            si (pictura.tectum[k] && l[III] != 255)
            {
                mala++;
            }
            /* tecta: eadem in ambabus; color palettae nisi crudum */
            si (pictura.tectum[k] && memcmp(o, l, III) != ZEPHYRUM)
            {
                mala++;
            }
            si (   pictura.tectum[k]
                && modus != (s32)STATUAE_MODUS_CRUDUS
                && _index_palettae(o) < ZEPHYRUM)
            {
                mala++;
            }
            /* crudum: pictura ipsa */
            si (   pictura.tectum[k]
                && modus == (s32)STATUAE_MODUS_CRUDUS
                && memcmp(o, pictura.rgba + k * IV, III) != ZEPHYRUM)
            {
                mala++;
            }
        }
        CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
    }
}

interior vacuum
_sectio_mutationes (
    CredoContextus* c)
{
                   Statua* s = _lapis(c);
    StatuaeQuadrumPetitio  p;
                    Imago  a;
                    Imago  b;
           memoriae_index  octeti = LATITUDO * ALTITUDO * IV;

    CREDO_NECESSE (s != NIHIL);
    p = _petitio(STATUAE_MODUS_ATKINSON, FALSUM);
    CREDO_NECESSE (statuae_quadrum_pingere(s, &p, &a, c->piscina));
    /* determinatum */
    CREDO_NECESSE (statuae_quadrum_pingere(s, &p, &b, c->piscina));
    CREDO_VERUM (memcmp(a.pixela, b.pixela, octeti) == ZEPHYRUM);
    /* tempus aliud */
    p.tempus = 0.7f;
    CREDO_NECESSE (statuae_quadrum_pingere(s, &p, &b, c->piscina));
    CREDO_VERUM (memcmp(a.pixela, b.pixela, octeti) != ZEPHYRUM);
    /* modus alius */
    p        = _petitio(STATUAE_MODUS_BAYER, FALSUM);
    CREDO_NECESSE (statuae_quadrum_pingere(s, &p, &b, c->piscina));
    CREDO_VERUM (memcmp(a.pixela, b.pixela, octeti) != ZEPHYRUM);
    /* visus alius: planum */
    p                    = _petitio(STATUAE_MODUS_ATKINSON, FALSUM);
    p.visus.illuminatum  = FALSUM;
    CREDO_NECESSE (statuae_quadrum_pingere(s, &p, &b, c->piscina));
    CREDO_VERUM (memcmp(a.pixela, b.pixela, octeti) != ZEPHYRUM);
    /* cyclus extra fines et quies: pinguntur (quies) */
    p         = _petitio(STATUAE_MODUS_NULLUS, FALSUM);
    p.cyclus  = -I;
    CREDO_NECESSE (statuae_quadrum_pingere(s, &p, &a, c->piscina));
    p.cyclus  = XCIX;
    CREDO_NECESSE (statuae_quadrum_pingere(s, &p, &b, c->piscina));
    CREDO_VERUM (memcmp(a.pixela, b.pixela, octeti) == ZEPHYRUM);
}

interior vacuum
_omnes_parvae (
     Statua* s,
    Piscina* piscina)
{
    hic_manens constans i32 MENSURAE[XII] = {
        1, 1, 1, 9, 9, 1, 2, 3, 7, 5, 16, 16
    };
    StatuaeQuadrumPetitio p;
                    Imago im;
                      s32 modus;
                      i32 k;

    per (modus = ZEPHYRUM; modus < (s32)STATUAE_MODORUM; modus++)
    {
        per (k = ZEPHYRUM; k < VI; k++)
        {
            p           = _petitio((StatuaeModus)modus, k & I);
            p.latitudo  = MENSURAE[k * II];
            p.altitudo  = MENSURAE[k * II + I];
            (vacuum)statuae_quadrum_pingere(s, &p, &im, piscina);
        }
    }
}

interior vacuum
_sectio_vacua (
    CredoContextus* c)
{
                   Statua* s = _lapis(c);
    StatuaeQuadrumPetitio  p;
                    Imago  im;

    CREDO_NECESSE (s != NIHIL);
    p           = _petitio(STATUAE_MODUS_ATKINSON, FALSUM);
    p.latitudo  = ZEPHYRUM;
    CREDO_VERUM (statuae_quadrum_pingere(s, &p, &im, c->piscina));
    CREDO_VERUM (im.pixela == NIHIL);
    CREDO_AEQUALIS_I32 (im.latitudo, ZEPHYRUM);
    p           = _petitio(STATUAE_MODUS_EXEMPLARIA, VERUM);
    p.altitudo  = ZEPHYRUM;
    CREDO_VERUM (statuae_quadrum_pingere(s, &p, &im, c->piscina));
    CREDO_VERUM (im.pixela == NIHIL);
    /* errores */
    p = _petitio(STATUAE_MODUS_ATKINSON, FALSUM);
    CREDO_FALSUM (statuae_quadrum_pingere(NIHIL, &p, &im, c->piscina));
    CREDO_FALSUM (statuae_quadrum_pingere(s, NIHIL, &im, c->piscina));
    CREDO_FALSUM (statuae_quadrum_pingere(s, &p, NIHIL, c->piscina));
    CREDO_FALSUM (statuae_quadrum_pingere(s, &p, &im, NIHIL));
    p.modus = STATUAE_MODORUM;
    CREDO_FALSUM (statuae_quadrum_pingere(s, &p, &im, c->piscina));
    /* imagines minimae, omni modo: nihil ruit */
    CREDO_NON_RUIT (_omnes_parvae(s, c->piscina));
}

interior vacuum
_sectio_fines (
    CredoContextus* c)
{
                   Statua* s = _lapis(c);
       StereometriaMatrix* matrices;
            StatuaPictura  pictura;
    StatuaeQuadrumPetitio  p;
         DitheringFructus  f;
                    Imago  im;
                      b32  activi[XVI];
                       i8  tectum[XL * XL];
                       i8* praecisa;
                      i32  x;
                      i32  y;
                      i32  lat;
                      i32  alt;
                      i32  px;
                      i32  py;
                      i32  mala;
                      i32  tecta;

    /* pixelum unum (13, 21) in 40 x 40: margo 8, reticulum 8 */
    memset(tectum, ZEPHYRUM, magnitudo(tectum));
    x = y = lat = alt = XCIX;
    CREDO_FALSUM (statuae_fines_tecti(tectum, XL, XL, &x, &y, &lat,
        &alt));
    CREDO_AEQUALIS_I32 (x, XCIX);
    CREDO_FALSUM (statuae_fines_tecti(NIHIL, XL, XL, &x, &y, &lat,
        &alt));
    CREDO_FALSUM (statuae_fines_tecti(tectum, XL, XL, NIHIL, &y, &lat,
        &alt));
    tectum[XXI * XL + XIII] = I;
    CREDO_NECESSE (statuae_fines_tecti(tectum, XL, XL, &x, &y, &lat,
        &alt));
    CREDO_AEQUALIS_I32 (x, ZEPHYRUM);     /* 13 - 8 = 5 -> 0 */
    CREDO_AEQUALIS_I32 (y, VIII);         /* 21 - 8 = 13 -> 8 */
    CREDO_AEQUALIS_I32 (lat, XXIV);       /* 13 + 9 = 22 -> 24 */
    CREDO_AEQUALIS_I32 (alt, XXIV);       /* 21 + 9 = 30 -> 32 */
    /* angulus imus dexter: ad picturam cogitur */
    tectum[XXXIX * XL + XXXIX] = I;
    CREDO_NECESSE (statuae_fines_tecti(tectum, XL, XL, &x, &y, &lat,
        &alt));
    CREDO_AEQUALIS_I32 (x, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (y, VIII);
    CREDO_AEQUALIS_I32 (lat, XL);
    CREDO_AEQUALIS_I32 (alt, XXXII);

    /* exemplaria: pixela tecta = praecisio manu dithering accepta */
    CREDO_NECESSE (s != NIHIL);
    p         = _petitio(STATUAE_MODUS_EXEMPLARIA, FALSUM);
    matrices  = (StereometriaMatrix*)piscina_allocare(c->piscina,
        s->nodorum * magnitudo(StereometriaMatrix));
    statua_positura(s, p.cyclus, p.tempus, matrices);
    CREDO_NECESSE (statua_pingere(s, matrices, &p.visus, LATITUDO,
        ALTITUDO, &pictura, c->piscina));
    CREDO_NECESSE (statuae_fines_tecti(pictura.tectum, LATITUDO,
        ALTITUDO, &x, &y, &lat, &alt));
    /* fines vere minores quadro: aliter probatio nihil probat */
    CREDO_VERUM (lat * alt < LATITUDO * ALTITUDO * III / IV);
    CREDO_VERUM (x + lat <= LATITUDO);
    CREDO_VERUM (y + alt <= ALTITUDO);
    praecisa = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)lat * alt * IV);
    per (py = ZEPHYRUM; py < alt; py++)
    {
        memcpy(praecisa + py * lat * IV,
            pictura.rgba + ((y + py) * LATITUDO + x) * IV, lat * IV);
    }
    dithering_praeparare_omnes(activi);
    f = dithering_exemplaria_velox_colorum(praecisa, lat, alt, activi,
        NIHIL, NIHIL, c->piscina);
    CREDO_NECESSE (f.successus && f.indices != NIHIL);
    CREDO_NECESSE (statuae_quadrum_pingere(s, &p, &im, c->piscina));
    mala   = ZEPHYRUM;
    tecta  = ZEPHYRUM;
    per (py = ZEPHYRUM; py < ALTITUDO; py++)
    {
        per (px = ZEPHYRUM; px < LATITUDO; px++)
        {
            constans i8* o = im.pixela + (py * LATITUDO + px) * IV;

            si (!pictura.tectum[py * LATITUDO + px])
            {
                mala += _index_palettae(o) != STATUAE_CHARTA_INDEX
                    ? I : ZEPHYRUM;
                perge;
            }
            tecta++;
            si (   px < x || px >= x + lat || py < y || py >= y + alt
                || _index_palettae(o)
                    != (s32)f.indices[(py - y) * lat + (px - x)])
            {
                mala++;
            }
        }
    }
    CREDO_VERUM (tecta > ZEPHYRUM);
    CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "modi", _sectio_modi, NIHIL, NIHIL, NIHIL, NIHIL },
    { "quadrum", _sectio_quadrum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mutationes", _sectio_mutationes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacua", _sectio_vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fines", _sectio_fines, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("statuae_quadrum", SECTIONES);
}
