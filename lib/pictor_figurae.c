/* pictor_figurae.c - figurae pictoris */

#include "pictor_figurae.h"
#include "thema.h"
#include "color.h"
#include "exemplaria.h"
#include "xar.h"

#include <stdio.h>
#include <string.h>

/* indicium foci: exemplar marginis lateris focati (vicus-latera) */
#define EXEMPLAR_FOCI EXEMPLAR_PUNCTA_DUPLICIA_DISPERSA

interior ColorMandati
color_thematis (
    ColorThema c)
{
    ColorMandati cm;

    cm.genus = COLOR_MANDATI_THEMA;
    cm.valor = (i32)c;
    redde cm;
}

/* index palettae COLORATIONIS (colores ictuum), non semanticus */
/* color palettae Aquinas (ut ictus pinguntur): RGBA ex
 * color_ex_palette - non COLOR_MANDATI_INDEX, quod rasterizatores per
 * indices colorationis (munera syntaxeos) resolvunt */
interior ColorMandati
color_thematis_index (
    s32 index)
{
    ColorMandati cm;

    cm.genus = COLOR_MANDATI_RGBA;
    cm.valor = color_ad_pixelum(color_ex_palette((i32)index));
    redde cm;
}

/* <purus/> Prospectus (013): mensa - fines toti colore
 * COLOR_SUPERFICIES; pagina (liber) supra pingitur */
vacuum
figura_prospectus (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    Fines f;

    (vacuum)thema;
    (vacuum)ctx;
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    mandata_rectangulum(m, f, color_thematis(COLOR_SUPERFICIES), VERUM);
    /* indicium foci (Franus): puncta levia super superficiem */
    si (chorda_aequalis_literis(c->titulus, "focatum"))
    {
        mandata_rectangulum_exemplar(m, f, color_thematis(COLOR_BORDER),
            EXEMPLAR_FOCI);
    }
}

/* <purus/> */
vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    PictorFigurae* pf;
            Fines  f;
              i32  i;

    (vacuum)thema;
    pf = (PictorFigurae*)ctx;
    si (!pf || !pf->doc)
    {
        redde;
    }
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    mandata_imago(m, pictor_documentum_sigillum_hex(pf->doc,
        m->piscina), f);
    /* margo paginae (013): in cellulis MARGINIS (cellula tota extra
     * documentum) - tessellatio rectangulum ad oras cellularum
     * proximas rotundat et marginem in anulo interiore pingit: margo I
     * pixeli extra documentum in cellulas documenti cadebat (scriba,
     * Franus 2026-10-05). Sine cellula (0): I pixelum, ut olim. */
    {
        Fines margo;
          s32 mx;
          s32 my;

        mx = pf->cellula_latitudo > ZEPHYRUM ? (s32)pf->cellula_latitudo
                                             : I;
        my = pf->cellula_altitudo > ZEPHYRUM ? (s32)pf->cellula_altitudo
                                             : I;
        margo.x         = -mx;
        margo.y         = -my;
        margo.latitudo  = f.latitudo + II * mx;
        margo.altitudo  = f.altitudo + II * my;
        mandata_rectangulum(m, margo, color_thematis(COLOR_BORDER),
                            FALSUM);
    }
    /* aspergillum (actio): guttae puncti cuiusque ex semine, radio et
     * colore in titulo - eaedem ac in actu (pictor_gutta) et colore
     * vero, sine morae guttis (eae solutione apparent) */
    si (   c->numerus_punctorum > ZEPHYRUM
        && chorda_aequalis_literis(c->actio, "aspergillum.ictus"))
    {
      longus semen;
     integer radius;
     integer color;
         s32 dx;
         s32 dy;
         i32 k;
       Fines g;

        semen   = ZEPHYRUM;
        radius  = ZEPHYRUM;
        color   = ZEPHYRUM;
        (vacuum)sscanf(chorda_ut_cstr(c->titulus, m->piscina),
            "%ld %d %d", &semen, &radius, &color);
        g.latitudo = I;
        g.altitudo = I;
        /* color primus nullus (-1): ictus nihil pingit, praevisio
         * quoque nihil */
        per (i = ZEPHYRUM; color >= ZEPHYRUM && color < XVI
                           && i < c->numerus_punctorum; i++)
        {
            per (k = ZEPHYRUM; k < PICTOR_GUTTAE_PUNCTO; k++)
            {
                pictor_gutta((s64)semen, i, k, (s32)radius, &dx, &dy);
                g.x = c->puncta[i].x + dx;
                g.y = c->puncta[i].y + dy;
                mandata_rectangulum(m, g,
                    color_thematis_index((s32)color), VERUM);
            }
        }
    }
    alioquin
    {
        per (i = I; i < c->numerus_punctorum; i++)
        {
            mandata_linea(m, c->puncta[i - I], c->puncta[i], I,
                          color_thematis(COLOR_ACCENT_PRIMARY));
        }
    }
    si (c->numerus_punctorum > ZEPHYRUM)
    {
        f.x         = c->puncta[c->numerus_punctorum - I].x - I;
        f.y         = c->puncta[c->numerus_punctorum - I].y - I;
        f.latitudo  = III;
        f.altitudo  = III;
        mandata_rectangulum(m, f, color_thematis(COLOR_CURSOR), FALSUM);
    }
}

/* <purus/> */
vacuum
figura_tituli (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
         Fines  f;
           s32  x;
           i32  i;
     Componens* q;

    (vacuum)thema;
    (vacuum)ctx;
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    mandata_rectangulum(m, f, color_thematis(COLOR_BACKGROUND), VERUM);
    /* P1a: textus post quadratum filium ultimum (cellula inter) */
    x = II;
    per (i = ZEPHYRUM; c->liberi && i < xar_numerus(c->liberi); i++)
    {
        q = *(Componens**)xar_obtinere(c->liberi, i);
        si (q->fines.x + q->fines.latitudo + VI > x)
        {
            x = q->fines.x + q->fines.latitudo + VI;
        }
    }
    /* 013 B3: linea status una cellula (VIII) - textus (fons 6x8)
     * verticaliter centratus, numquam infra fines (olim II fixum in
     * linea XII: in VIII pars inferior praecidebatur) */
    mandata_textus(m, x, (f.altitudo > VIII) ? (f.altitudo - VIII) / II
                                             : ZEPHYRUM,
                   c->titulus, ZEPHYRUM, color_thematis(COLOR_TEXT));
}

constans Imago*
pictor_imago_fons (
    chorda  provenientia,
    vacuum* ctx)
{
    PictorFigurae* pf;

    pf = (PictorFigurae*)ctx;
    si (!pf || !pf->doc)
    {
        redde NIHIL;
    }
    si (chorda_aequalis(provenientia,
            pictor_documentum_sigillum_hex(pf->doc, pf->doc->piscina)))
    {
        redde pictor_documentum_proiectio(pf->doc);
    }
    redde NIHIL;
}

/* P1a: icones 1-bit XVI x XVI ('#' = pixelum colore textus) */
hic_manens constans character* icon_penicilli[XVI] = {
    "................",
    "............##..",
    "...........####.",
    "..........#####.",
    ".........#####..",
    "........#####...",
    ".......#####....",
    "......#####.....",
    ".....####.......",
    "....#.##........",
    "...###..........",
    "..####..........",
    ".####...........",
    ".###............",
    ".#..............",
    "................"
};

hic_manens constans character* icon_aspergilli[XVI] = {
    "................",
    ".#..#...........",
    "..#..#..##......",
    ".#..#..####.....",
    ".......#..#.....",
    "......######....",
    "......#....#....",
    "......#....#....",
    "......#.##.#....",
    "......#.##.#....",
    "......#....#....",
    "......#....#....",
    "......#....#....",
    "......######....",
    "................",
    "................"
};

interior vacuum
iconem_pingere (
                Mandata*  m,
     constans character** icon,
                    s32   x0,
                    s32   y0)
{
    Fines g;
      s32 x;
      s32 y;

    g.latitudo = I;
    g.altitudo = I;
    per (y = ZEPHYRUM; y < XVI; y++)
    {
        per (x = ZEPHYRUM; x < XVI; x++)
        {
            si (icon[y][x] == '#')
            {
                g.x = x0 + x;
                g.y = y0 + y;
                mandata_rectangulum(m, g, color_thematis(COLOR_TEXT),
                    VERUM);
            }
        }
    }
}

/* <purus/> quadratum: margo, deinde icon aut color aut crux (nullus) */
vacuum
figura_quadrati (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
      Fines f;
      Fines intus;
    Punctum a;
    Punctum b;
     chorda t;
     chorda reliquum;
        s32 index;
        b32 electum;

    (vacuum)thema;
    (vacuum)ctx;
    /* P1b: ':electum' - optio electa, margo accentus */
    t        = c->titulus;
    electum  = t.mensura > VIII
        && memcmp(t.datum + t.mensura - VIII, ":electum", VIII)
           == ZEPHYRUM;
    si (electum)
    {
        t.mensura -= VIII;
    }
    f.x             = ZEPHYRUM;
    f.y             = ZEPHYRUM;
    f.latitudo      = c->fines.latitudo;
    f.altitudo      = c->fines.altitudo;
    intus.x         = II;
    intus.y         = II;
    intus.latitudo  = c->fines.latitudo - IV;
    intus.altitudo  = c->fines.altitudo - IV;
    mandata_rectangulum(m, f, color_thematis(COLOR_BACKGROUND), VERUM);
    mandata_rectangulum(m, f, color_thematis(electum
        ? COLOR_ACCENT_PRIMARY : COLOR_BORDER), FALSUM);
    si (electum)
    {
        Fines g;

        g.x         = I;
        g.y         = I;
        g.latitudo  = f.latitudo - II;
        g.altitudo  = f.altitudo - II;
        mandata_rectangulum(m, g, color_thematis(COLOR_ACCENT_PRIMARY),
            FALSUM);
    }
    si (chorda_aequalis_literis(t, "instrumentum:penicillus"))
    {
        iconem_pingere(m, icon_penicilli, II, II);
        redde;
    }
    si (chorda_aequalis_literis(t, "instrumentum:aspergillum"))
    {
        iconem_pingere(m, icon_aspergilli, II, II);
        redde;
    }
    si (   t.mensura > VI
        && memcmp(t.datum, "color:", VI) == ZEPHYRUM)
    {
        reliquum.datum    = t.datum + VI;
        reliquum.mensura  = t.mensura - VI;
        si (   chorda_ut_s32(reliquum, &index) && index >= ZEPHYRUM
            && index < XVI)
        {
            mandata_rectangulum(m, intus, color_thematis_index(index),
                VERUM);
            redde;
        }
        /* nullus: crux */
        a.x = intus.x;
        a.y = intus.y;
        b.x = intus.x + intus.latitudo - I;
        b.y = intus.y + intus.altitudo - I;
        mandata_linea(m, a, b, I, color_thematis(COLOR_TEXT));
        a.x = b.x;
        b.x = intus.x;
        mandata_linea(m, a, b, I, color_thematis(COLOR_TEXT));
    }
}

/* <purus/> palette: fundus superficiei, margo */
vacuum
figura_palettae (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    Fines f;

    (vacuum)thema;
    (vacuum)ctx;
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    mandata_rectangulum(m, f, color_thematis(COLOR_SUPERFICIES), VERUM);
    mandata_rectangulum(m, f, color_thematis(COLOR_BORDER), FALSUM);
}

vacuum
pictor_figurae_registrare (
    FiguraRegistrum* reg,
                i32  thema,
      PictorFigurae* ctx)
{
    si (!reg || !ctx)
    {
        redde;
    }
    figura_registrare(reg, PARTES_TABULA, thema, figura_tabulae, ctx);
    figura_registrare(reg, PARTES_TITULUS, thema, figura_tituli, ctx);
    figura_registrare(reg, PARTES_BOTTONE, thema, figura_quadrati, ctx);
    figura_registrare(reg, PARTES_DIALOGUS, thema, figura_palettae,
        ctx);
    figura_registrare(reg, PARTES_PROSPECTUS, thema, figura_prospectus,
                      ctx);
}
