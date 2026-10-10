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
/* praevisio verrendi (penicillus, spongia): sigillum in puncto primo
 * et per lineam (pictor_lineam_ambulare, regula documenti); gradus >
 * I (penicillus magnus): sigillum quoque gradu et in fine segmenti
 * solum - fasciae minus, ora vix differt */
nomen structura {
         Mandata* m;
             s32  n;
             b32  quadratum;
    ColorMandati  color;
             s32  gradus;
             s32  numerus;
} Praevidendum;

/* sigillum: quadratum n x n aut discus per fascias (lineas I altas)
 * centratum in (cx, cy) - [c - n/2, c - n/2 + n) */
interior vacuum
sigillum_praevidere (
    Praevidendum* pv,
             s32  cx,
             s32  cy)
{
    Fines g;
      s32 i;
      s32 j;
      s32 a;

    si (pv->quadratum)
    {
        g.x         = cx - pv->n / II;
        g.y         = cy - pv->n / II;
        g.latitudo  = pv->n;
        g.altitudo  = pv->n;
        mandata_rectangulum(pv->m, g, pv->color, VERUM);
        redde;
    }
    per (j = ZEPHYRUM; j < pv->n; j++)
    {
        a = -I;
        per (i = ZEPHYRUM; i <= pv->n; i++)
        {
            si (i < pv->n && pictor_disci_pixelum(pv->n, i, j))
            {
                si (a < ZEPHYRUM)
                {
                    a = i;
                }
            }
            alioquin si (a >= ZEPHYRUM)
            {
                g.x         = cx - pv->n / II + a;
                g.y         = cy - pv->n / II + j;
                g.latitudo  = i - a;
                g.altitudo  = I;
                mandata_rectangulum(pv->m, g, pv->color, VERUM);
                a = -I;
            }
        }
    }
}

interior vacuum
vestigium_praevidendi (
       s32  x,
       s32  y,
    vacuum* ctx)
{
    Praevidendum* pv;

    pv = (Praevidendum*)ctx;
    pv->numerus++;
    si (pv->numerus % pv->gradus == ZEPHYRUM)
    {
        sigillum_praevidere(pv, x, y);
    }
}

interior vacuum
verrere_praevidere (
       constans Componens* c,
             Praevidendum* pv)
{
    i32 i;

    sigillum_praevidere(pv, c->puncta[ZEPHYRUM].x,
        c->puncta[ZEPHYRUM].y);
    per (i = I; i < c->numerus_punctorum; i++)
    {
        pv->numerus = ZEPHYRUM;
        pictor_lineam_ambulare(c->puncta[i - I].x, c->puncta[i - I].y,
            c->puncta[i].x, c->puncta[i].y, vestigium_praevidendi, pv);
        si (pv->numerus % pv->gradus != ZEPHYRUM)
        {
            sigillum_praevidere(pv, c->puncta[i].x, c->puncta[i].y);
        }
    }
}

/* spongia (praevisio): quadrata colore fundi; margo cursoris in fine */
interior vacuum
spongiam_praevidere (
    constans Componens* c,
               Mandata* m)
{
         integer latus;
    Praevidendum pv;
           Fines g;

    latus = ZEPHYRUM;
    (vacuum)sscanf(chorda_ut_cstr(c->titulus, m->piscina), "%d",
        &latus);
    si (latus < I)
    {
        redde;
    }
    pv.m          = m;
    pv.n          = (s32)latus;
    pv.quadratum  = VERUM;
    pv.color      = color_thematis(COLOR_BACKGROUND);
    pv.gradus     = I;
    pv.numerus    = ZEPHYRUM;
    verrere_praevidere(c, &pv);
    g.x = c->puncta[c->numerus_punctorum - I].x - (s32)latus
        / II;
    g.y = c->puncta[c->numerus_punctorum - I].y - (s32)latus
        / II;
    g.latitudo = (s32)latus;
    g.altitudo = (s32)latus;
    mandata_rectangulum(m, g, color_thematis(COLOR_BORDER), FALSUM);
}

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

/* P4a penicillus (praevisio): disci per fascias colore vero (titulus
 * "amplitudo color"; color -1 nihil); gradus n/IV - exemplar in
 * solutione solum apparet */
interior vacuum
penicillum_praevidere (
    constans Componens* c,
               Mandata* m)
{
         integer amplitudo;
         integer color;
    Praevidendum pv;

    amplitudo  = I;
    color      = -I;
    (vacuum)sscanf(chorda_ut_cstr(c->titulus, m->piscina), "%d %d",
        &amplitudo, &color);
    si (amplitudo < I || color < ZEPHYRUM || color >= XVI)
    {
        redde;
    }
    pv.m          = m;
    pv.n          = (s32)amplitudo;
    pv.quadratum  = FALSUM;
    pv.color      = color_thematis_index((s32)color);
    pv.gradus     = pv.n / IV > I ? pv.n / IV : I;
    pv.numerus    = ZEPHYRUM;
    verrere_praevidere(c, &pv);
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
     integer primus;
     integer secundus;
     integer exemplar;
     integer color;
         s32 dx;
         s32 dy;
         i32 k;
       Fines g;

        semen     = ZEPHYRUM;
        radius    = ZEPHYRUM;
        primus    = ZEPHYRUM;
        secundus  = -I;
        exemplar  = ZEPHYRUM;
        (vacuum)sscanf(chorda_ut_cstr(c->titulus, m->piscina),
            "%ld %d %d %d %d", &semen, &radius, &primus, &secundus,
            &exemplar);
        g.latitudo = I;
        g.altitudo = I;
        /* P3: gutta quaeque colorem atramenti ad locum suum (ut
         * documentum); color nullus (-1): nihil, ut ictus */
        per (i = ZEPHYRUM; i < c->numerus_punctorum; i++)
        {
            /* P4b: VI x m guttae, m = radius / ASPERGILLI_RADIUS */
            per (k = ZEPHYRUM; k < (i32)(PICTOR_GUTTAE_PUNCTO
                     * (radius / PICTOR_ASPERGILLI_RADIUS > I
                        ? radius / PICTOR_ASPERGILLI_RADIUS : I)); k++)
            {
                pictor_gutta((s64)semen, i, k, (s32)radius, &dx, &dy);
                g.x = c->puncta[i].x + dx;
                g.y = c->puncta[i].y + dy;
                color = exemplar_punctum((i32)exemplar, g.x, g.y)
                    ? primus : secundus;
                si (color >= ZEPHYRUM && color < XVI)
                {
                    mandata_rectangulum(m, g,
                        color_thematis_index((s32)color), VERUM);
                }
            }
        }
    }
    alioquin si (   c->numerus_punctorum > ZEPHYRUM
                 && chorda_aequalis_literis(c->actio, "spongia.ictus"))
    {
        spongiam_praevidere(c, m);
    }
    alioquin si (   c->numerus_punctorum > ZEPHYRUM
                 && (   chorda_aequalis_literis(c->actio,
                            "penicillus.ictus")
                     || chorda_aequalis_literis(c->actio,
                     "linea.ictus")))
    {
        penicillum_praevidere(c, m);
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

/* linea: tractus obliquus */
hic_manens constans character* icon_lineae[XVI] = {
    "................",
    "..............#.",
    ".............##.",
    "............##..",
    "...........##...",
    "..........##....",
    ".........##.....",
    "........##......",
    ".......##.......",
    "......##........",
    ".....##.........",
    "....##..........",
    "...##...........",
    "..##............",
    ".##.............",
    "................"
};

/* P2: spongia - truncus obliquus */
hic_manens constans character* icon_spongiae[XVI] = {
    "................",
    "................",
    "................",
    "......#########.",
    ".....#.......##.",
    "....#.......#.#.",
    "...#.......#..#.",
    "..#.......#...#.",
    ".#########....#.",
    ".#.......#...#..",
    ".#.......#..#...",
    ".#.......#.#....",
    ".#.......##.....",
    ".#########......",
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
    si (chorda_aequalis_literis(t, "instrumentum:spongia"))
    {
        iconem_pingere(m, icon_spongiae, II, II);
        redde;
    }
    si (chorda_aequalis_literis(t, "instrumentum:linea"))
    {
        iconem_pingere(m, icon_lineae, II, II);
        redde;
    }
    /* L3 strata: '+' / '-' (strata:novum / strata:deletum), nomen
     * strati currentis ("strata:<nomen>"), ordo ("stratum:<id>"),
     * oculus ("oculus:<0|1>": plenum visibile, vacuum occultum) */
    si (chorda_aequalis_literis(t, "strata:novum"))
    {
        a.x = c->fines.latitudo / II;
        a.y = V;
        b.x = a.x;
        b.y = c->fines.altitudo - VI;
        mandata_linea(m, a, b, I, color_thematis(COLOR_TEXT));
        a.x = V;
        a.y = c->fines.altitudo / II;
        b.x = c->fines.latitudo - VI;
        b.y = a.y;
        mandata_linea(m, a, b, I, color_thematis(COLOR_TEXT));
        redde;
    }
    si (chorda_aequalis_literis(t, "strata:deletum"))
    {
        a.x = V;
        a.y = c->fines.altitudo / II;
        b.x = c->fines.latitudo - VI;
        b.y = a.y;
        mandata_linea(m, a, b, I, color_thematis(COLOR_TEXT));
        redde;
    }
    si (t.mensura > VII && memcmp(t.datum, "strata:", VII) == ZEPHYRUM)
    {
        reliquum.datum    = t.datum + VII;
        reliquum.mensura  = t.mensura - VII;
        mandata_textus(m, IV, (c->fines.altitudo - VIII) / II, reliquum,
            ZEPHYRUM, color_thematis(COLOR_TEXT));
        redde;
    }
    si (   t.mensura > VIII
        && memcmp(t.datum, "stratum:", VIII) == ZEPHYRUM)
    {
        reliquum.datum    = t.datum + VIII;
        reliquum.mensura  = t.mensura - VIII;
        mandata_textus(m, IV, (c->fines.altitudo - VIII) / II,
            chorda_concatenare(chorda_ex_literis("stratum ",
            m->piscina),
            reliquum, m->piscina), ZEPHYRUM,
            color_thematis(COLOR_TEXT));
        redde;
    }
    si (chorda_aequalis_literis(t, "oculus:1"))
    {
        Fines g;

        g.x         = V;
        g.y         = V;
        g.latitudo  = c->fines.latitudo - X;
        g.altitudo  = c->fines.altitudo - X;
        mandata_rectangulum(m, g, color_thematis(COLOR_TEXT), VERUM);
        redde;
    }
    si (chorda_aequalis_literis(t, "oculus:0"))
    {
        redde;
    }
    /* P4a: magnitudo - discus diametri n centratus (n > XVI:
     * numerus) */
    si (   t.mensura > X
        && memcmp(t.datum, "magnitudo:", X) == ZEPHYRUM)
    {
        Praevidendum pv;
             integer n;
               Fines g;
                 s32 r;
                 s32 dx;
                 s32 dy;
                 s32 k;
                 s32 gradus;

        /* P4b aspergillum: guttae in disco radii II + III x log2(m) /
         * II (II..VIII) - semen fixum */
        si (   t.mensura > XXII
            && memcmp(t.datum, "magnitudo:aspergillum:", XXII)
               == ZEPHYRUM)
        {
            n = I;
            (vacuum)sscanf(chorda_ut_cstr(t, m->piscina) + XXII, "%d",
                &n);
            gradus = ZEPHYRUM;
            dum ((I << gradus) < (s32)n && gradus < IV)
            {
                gradus++;
            }
            r           = II + (III * gradus) / II;
            g.latitudo  = I;
            g.altitudo  = I;
            per (k = ZEPHYRUM; k < IV + III * gradus; k++)
            {
                pictor_gutta((s64)CCCXXI, ZEPHYRUM, (i32)k, r, &dx,
                    &dy);
                g.x = c->fines.latitudo / II + dx;
                g.y = c->fines.altitudo / II + dy;
                mandata_rectangulum(m, g, color_thematis(COLOR_TEXT),
                    VERUM);
            }
            redde;
        }
        /* linea: tractus latitudinis n (XVI ad summum), colore
         * textus */
        si (   t.mensura > XVI
            && memcmp(t.datum, "magnitudo:linea:", XVI) == ZEPHYRUM)
        {
            n = I;
            (vacuum)sscanf(chorda_ut_cstr(t, m->piscina) + XVI, "%d",
                &n);
            n           = n > XVI ? XVI : n < I ? I : n;
            g.x         = III;
            g.y         = (c->fines.altitudo - (s32)n) / II;
            g.latitudo  = c->fines.latitudo - VI;
            g.altitudo  = (s32)n;
            mandata_rectangulum(m, g, color_thematis(COLOR_TEXT),
                VERUM);
            redde;
        }
        /* P4b spongia: magnitudo fixa - quadratum hebes */
        si (   t.mensura > XVIII
            && memcmp(t.datum, "magnitudo:spongia:", XVIII) == ZEPHYRUM)
        {
            g.x         = IV;
            g.y         = IV;
            g.latitudo  = c->fines.latitudo - VIII;
            g.altitudo  = c->fines.altitudo - VIII;
            mandata_rectangulum(m, g, color_thematis(COLOR_BORDER),
                FALSUM);
            redde;
        }
        n = ZEPHYRUM;
        (vacuum)sscanf(chorda_ut_cstr(t, m->piscina) + X, "%d", &n);
        si (n > XVI)
        {
            reliquum.datum    = t.datum + X;
            reliquum.mensura  = t.mensura - X;
            mandata_textus(m, (c->fines.latitudo
                - (s32)reliquum.mensura * VI) / II,
                (c->fines.altitudo - VIII) / II, reliquum, ZEPHYRUM,
                color_thematis(COLOR_TEXT));
            redde;
        }
        si (n >= I)
        {
            pv.m          = m;
            pv.n          = (s32)n;
            pv.quadratum  = FALSUM;
            pv.color      = color_thematis(COLOR_TEXT);
            pv.gradus     = I;
            pv.numerus    = ZEPHYRUM;
            sigillum_praevidere(&pv, c->fines.latitudo / II,
                c->fines.altitudo / II);
        }
        redde;
    }
    /* P3: exemplar - "exemplar:<n>" 1-bit colore textus (optio),
     * "exemplar:<n>:<primus>:<secundus>" coloribus veris (secundus
     * plenus prius, primus per exemplar; nullus = fundus) */
    si (   t.mensura > IX
        && memcmp(t.datum, "exemplar:", IX) == ZEPHYRUM)
    {
        integer exemplar;
        integer primus;
        integer secundus;
        integer numerus;

        exemplar  = ZEPHYRUM;
        primus    = -I;
        secundus  = -I;
        numerus   = sscanf(chorda_ut_cstr(t, m->piscina) + IX,
            "%d:%d:%d", &exemplar, &primus, &secundus);
        /* sine coloribus (aut ambo nulli): 1-bit, ut exemplaria
         * legibilia maneant */
        si (   numerus < III
            || (   (primus < ZEPHYRUM || primus >= XVI)
                && (secundus < ZEPHYRUM || secundus >= XVI)))
        {
            mandata_rectangulum_exemplar(m, intus,
                color_thematis(COLOR_TEXT), (i32)exemplar);
            redde;
        }
        /* solidus (0): secundus invisibilis */
        si (   exemplar != ZEPHYRUM && secundus >= ZEPHYRUM
            && secundus < XVI)
        {
            mandata_rectangulum(m, intus,
                color_thematis_index((s32)secundus), VERUM);
        }
        si (primus >= ZEPHYRUM && primus < XVI)
        {
            mandata_rectangulum_exemplar(m, intus,
                color_thematis_index((s32)primus), (i32)exemplar);
        }
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
