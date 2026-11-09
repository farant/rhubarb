/* gabc_pictura.c - pagina compositionis gabc in mandata (gabc pars II,
 * P4). Regulae in include/gabc_pictura.h.
 *
 * Larva -> rectangula: copia ex lib/terminale.c larvam_pingere (cum
 * mixtura canalium), non extractio - mandatum thema nescit et nescire
 * debet; debitum in tabulario notatum. */
#include "latina.h"
#include "gabc_pictura.h"
#include "neumae_ductae.h"
#include "thema.h"
#include "color.h"
#include "figura.h"
#include "fons.h"
#include "fons_8x16.h"
#include "utf8.h"
#include <string.h>

#define TEXTUS_MAXIMUS  DXII


/* ==================================================
 * Colores
 * ================================================== */

interior Color
colorem_resolvere (
    ColorMandati c)
{
    commutatio (c.genus)
    {
        casus COLOR_MANDATI_THEMA:
            redde thema_color((ColorThema)c.valor);
        casus COLOR_MANDATI_INDEX:
            redde thema_color_ex_indice_colorationis((i8)c.valor);
        ordinarius:
            redde color_ex_pixelum(c.valor);
    }
}

interior ColorMandati
colorem_rgba (
    Color c)
{
    ColorMandati cm;

    cm.genus = COLOR_MANDATI_RGBA;
    cm.valor = color_ad_pixelum(color_ex_rgba(c.r, c.g, c.b, (i8)0xFF));
    redde cm;
}

/* littera in fundum per opacitatem (0..CCLV), canalis quisque */
interior ColorMandati
miscere (
    ColorMandati littera,
           Color fundus,
             i32 opacitas)
{
    Color l;
    Color r;

    l    = colorem_resolvere(littera);
    r.r  = (i8)(((i32)fundus.r * (CCLV - opacitas)
                 + (i32)l.r * opacitas) / CCLV);
    r.g  = (i8)(((i32)fundus.g * (CCLV - opacitas)
                 + (i32)l.g * opacitas) / CCLV);
    r.b  = (i8)(((i32)fundus.b * (CCLV - opacitas)
                 + (i32)l.b * opacitas) / CCLV);
    r.a  = (i8)0xFF;
    redde colorem_rgba(r);
}

interior ColorMandati
colorem_picti (
           constans GabcPictum* p,
    constans GabcPicturaStatus* status)
{
    b32 illuminatum;

    si (p->tinctura == GABC_TINCTURA_SYSTEMA)
    {
        redde figura_color_thematis(COLOR_TEXT_DIM);
    }
    illuminatum = status != NIHIL
        && ((p->syllaba >= ZEPHYRUM
             && p->syllaba == status->syllaba_illuminata)
            || (p->nota >= ZEPHYRUM
                && p->nota == status->nota_illuminata));
    si (illuminatum)
    {
        redde figura_color_thematis(COLOR_ACCENT_PRIMARY);
    }
    si (p->tinctura == GABC_TINCTURA_RUBRICA)
    {
        redde colorem_rgba(color_ex_palette(PALETTE_DARK_RED));
    }
    redde figura_color_thematis(COLOR_TEXT);
}


/* ==================================================
 * Partes, textus, initialis
 * ================================================== */

/* larva ut rectangula: cursus aequalis opacitatis per ordinem;
 * plenum = color, partialis in fundum mixtus */
interior vacuum
larvam_pingere (
         Mandata* md,
     constans i8* larva,
             s32  latitudo,
             s32  altitudo,
             s32  x0,
             s32  y0,
    ColorMandati  color,
           Color  fundus)
{
    Fines f;
      s32 x;
      s32 y;
      s32 initium;
       i8 valor;

    f.altitudo = I;
    per (y = ZEPHYRUM; y < altitudo; y++)
    {
        x = ZEPHYRUM;
        dum (x < latitudo)
        {
            valor = larva[y * latitudo + x];
            si (valor == ZEPHYRUM)
            {
                x++;
                perge;
            }
            initium = x;
            dum (x < latitudo && larva[y * latitudo + x] == valor)
            {
                x++;
            }
            f.x         = x0 + initium;
            f.y         = y0 + y;
            f.latitudo  = x - initium;
            mandata_rectangulum(md, f, valor == 0xFF ? color
                : miscere(color, fundus, (i32)valor), VERUM);
        }
    }
}

interior vacuum
partem_pingere (
              Mandata* md,
    constans GabcPictum* p,
                  i32  spatium,
                  s32  x_origo,
                  s32  y_origo,
         ColorMandati  color,
                Color  fundus)
{
               i8 larva[NEUMAE_LARVA_MAXIMA];
    NeumaeMensura m;

    si (   !neumae_ductae_metiri(p->pars, spatium, p->parametrum, &m)
        || m.latitudo * m.altitudo > NEUMAE_LARVA_MAXIMA
        || !neumae_ductae_pingere(p->pars, spatium, p->parametrum,
               m.latitudo, m.altitudo, larva))
    {
        redde;
    }
    larvam_pingere(md, larva, (s32)m.latitudo, (s32)m.altitudo,
        x_origo + p->x - m.origo_x, y_origo + p->y - m.origo_y, color,
        fundus);
}

/* textus sine notis compositis (U+0300-036F) */
interior vacuum
textum_pingere (
                Mandata* md,
    constans GabcPictum* p,
                    s32  x_origo,
                    s32  y_origo,
           ColorMandati  color)
{
             i8  receptaculum[TEXTUS_MAXIMUS];
    constans i8* q;
    constans i8* ante;
    constans i8* finis;
         chorda  purum;
            s32  runa;
            i32  n;

    q      = p->textus.datum;
    finis  = q + p->textus.mensura;
    n      = ZEPHYRUM;
    dum (q < finis)
    {
        ante = q;
        runa = utf8_decodere(&q, finis);
        si (runa >= 0x300 && runa <= 0x36F)
        {
            perge;
        }
        si (n + (i32)(q - ante) > TEXTUS_MAXIMUS)
        {
            frange;
        }
        memcpy(receptaculum + n, ante, (size_t)(q - ante));
        n += (i32)(q - ante);
    }
    purum.datum    = receptaculum;
    purum.mensura  = n;
    mandata_textus(md, x_origo + p->x, y_origo + p->y, purum, p->fons,
        color);
}

/* littera VIII x XVI pixelo quoque ut quadrato scalae */
interior vacuum
initialem_pingere (
                Mandata* md,
    constans GabcPictum* p,
                    s32  x_origo,
                    s32  y_origo,
           ColorMandati  color)
{
    constans i8* q;
          Fines  f;
            s32  runa;
            s32  scala;
            s32  x;
            s32  y;
            s32  initium;
             i8  glypha;
             i8  ordo;

    q = p->textus.datum;
    runa = utf8_decodere(&q, p->textus.datum + p->textus.mensura);
    glypha = fons_8x16_codepoint_ad_glypham(runa);
    scala = p->fons > ZEPHYRUM ? (s32)p->fons : I;
    f.altitudo = scala;
    per (y = ZEPHYRUM; y < FONS_8X16_ALTITUDO; y++)
    {
        ordo  = fons_8x16[glypha][y];
        x     = ZEPHYRUM;
        dum (x < FONS_8X16_LATITUDO)
        {
            si (!((ordo >> (VII - x)) & I))
            {
                x++;
                perge;
            }
            initium = x;
            dum (x < FONS_8X16_LATITUDO && ((ordo >> (VII - x)) & I))
            {
                x++;
            }
            f.x         = x_origo + p->x + initium * scala;
            f.y         = y_origo + p->y + y * scala;
            f.latitudo  = (x - initium) * scala;
            mandata_rectangulum(md, f, color, VERUM);
        }
    }
}


/* ==================================================
 * Functio
 * ================================================== */

b32
gabc_paginam_pingere (
       constans GabcCompositio* compositio,
                           i32  pagina,
                           s32  x,
                           s32  y,
    constans GabcPicturaStatus* status,
                       Mandata* mandata)
{
    constans GabcPictum* p;
           ColorMandati  color;
                  Color  fundus;
                  Fines  f;
                    i32  i;

    si (   compositio == NIHIL || mandata == NIHIL
        || pagina     >= compositio->numerus_paginarum)
    {
        redde FALSUM;
    }
    fundus = thema_color(COLOR_BACKGROUND);
    per (i = compositio->paginae[pagina];
         i < compositio->paginae[pagina + I]; i++)
    {
        p      = &compositio->picta[i];
        color  = colorem_picti(p, status);
        commutatio (p->genus)
        {
            casus GABC_PICTUM_PARS:
                partem_pingere(mandata, p, compositio->forma.spatium, x,
                    y,
                    color, fundus);
                frange;
            casus GABC_PICTUM_RECTANGULUM:
                f.x         = x + p->x;
                f.y         = y + p->y;
                f.latitudo  = (s32)p->latitudo;
                f.altitudo  = (s32)p->altitudo;
                mandata_rectangulum(mandata, f, color, VERUM);
                frange;
            casus GABC_PICTUM_TEXTUS:
                textum_pingere(mandata, p, x, y, color);
                frange;
            casus GABC_PICTUM_INITIALIS:
                initialem_pingere(mandata, p, x, y, color);
                frange;
            ordinarius:
                frange;
        }
    }
    redde VERUM;
}
