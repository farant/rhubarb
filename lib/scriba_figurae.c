/* scriba_figurae.c - figurae scribae */

#include "scriba_figurae.h"
#include "thema.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior ColorMandati
color_thematis (
    ColorThema c)
{
    ColorMandati cm;

    cm.genus = COLOR_MANDATI_THEMA;
    cm.valor = (i32)c;
    redde cm;
}

interior Fines
fines (
    s32 x,
    s32 y,
    s32 w,
    s32 h)
{
    Fines f;

    f.x         = x;
    f.y         = y;
    f.latitudo  = w;
    f.altitudo  = h;
    redde f;
}

/* cellula visibilis: tab, continuatio, '\0', regimen -> spatium */
interior character
visibilis (
    character c)
{
    i8 o;

    o = (i8)c;
    redde (o >= XXXII && o < CXXVII) ? c : ' ';
}


/* ==================================================
 * Figurae
 * ================================================== */

/* <purus/> mensa: fines toti colore COLOR_SUPERFICIES */
vacuum
scriba_figura_mensae (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    (vacuum)thema;
    (vacuum)ctx;
    mandata_rectangulum(m, fines(ZEPHYRUM, ZEPHYRUM, c->fines.latitudo,
        c->fines.altitudo), color_thematis(COLOR_SUPERFICIES), VERUM);
}

/* <purus/> folium: charta, margo, selectio, cursor, textus */
vacuum
scriba_figura_folii (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
                 ScribaFigurae* sf;
    constans TabulaCharacterum* t;
                           s32  cw;
                           s32  ch;
                           s32  l;
                           s32  col;
                           s32  finis;
                           s32  sel_a;
                           s32  sel_b;
                    ColorThema  color_cursoris;
                        chorda  linea;
                     character  sub;
                       Punctum  cursor;

    (vacuum)thema;
    sf = (ScribaFigurae*)ctx;
    si (!sf || !sf->sa)
    {
        redde;
    }
    t = &sf->sa->laboris;
    si (t->latitudo == ZEPHYRUM || t->altitudo == ZEPHYRUM)
    {
        redde;
    }
    cw = c->fines.latitudo / (s32)t->latitudo;
    ch = c->fines.altitudo / (s32)t->altitudo;
    mandata_rectangulum(m, fines(ZEPHYRUM, ZEPHYRUM, c->fines.latitudo,
        c->fines.altitudo), color_thematis(COLOR_BACKGROUND), VERUM);
    /* margo in cellulis MARGINIS (cellula tota extra folium):
     * tessellatio rectangulum ad oras cellularum PROXIMAS rotundat et
     * marginem in anulo interiore pingit - margo I pixeli extra
     * chartam in cellulas folii cadebat (Franus: scribendo
     * delebatur) */
    mandata_rectangulum(m, fines(-cw, -ch,
        c->fines.latitudo + II * cw, c->fines.altitudo + II * ch),
        color_thematis(COLOR_BORDER), FALSUM);

    /* selectio (visualis): lineae inter ancoram et cursorem */
    sel_a = -I;
    sel_b = -II;
    si (c->numerus_punctorum >= II)
    {
        sel_a = c->puncta[I].y < c->puncta[ZEPHYRUM].y
              ? c->puncta[I].y : c->puncta[ZEPHYRUM].y;
        sel_b = c->puncta[I].y < c->puncta[ZEPHYRUM].y
              ? c->puncta[ZEPHYRUM].y : c->puncta[I].y;
        mandata_rectangulum(m, fines(ZEPHYRUM, sel_a * ch,
            c->fines.latitudo, (sel_b - sel_a + I) * ch),
            color_thematis(COLOR_SELECTION), VERUM);
    }

    /* cursor: quadratum plenum sub textu, colore STATUS modi sui (ut
     * verbum in linea status) - COLOR_CURSOR idem aurum est ac
     * COLOR_STATUS_INSERT in themate: modi aliter pares essent */
    color_cursoris = chorda_aequalis_literis(c->titulus, "inserere")
                   ? COLOR_STATUS_INSERT
                   : chorda_aequalis_literis(c->titulus, "visualis")
                   ? COLOR_STATUS_VISUAL : COLOR_STATUS_NORMAL;
    cursor.x = -I;
    cursor.y = -I;
    si (c->numerus_punctorum >= I)
    {
        cursor = c->puncta[ZEPHYRUM];
        mandata_rectangulum(m, fines(cursor.x * cw, cursor.y * ch, cw,
            ch),
            color_thematis(color_cursoris),
            VERUM);
    }

    /* textus: linea una per mandatum, spatiis finalibus omissis */
    per (l = ZEPHYRUM; l < (s32)t->altitudo; l++)
    {
        finis = (s32)t->latitudo;
        dum (   finis > ZEPHYRUM
             && visibilis(tabula_cellula(t, (i32)l, (i32)(finis - I)))
                == ' ')
        {
            finis--;
        }
        si (finis == ZEPHYRUM)
        {
            perge;
        }
        linea.datum = (i8*)piscina_allocare(m->piscina,
            (memoriae_index)finis);
        linea.mensura = (i32)finis;
        per (col = ZEPHYRUM; col < finis; col++)
        {
            linea.datum[col] = (i8)visibilis(tabula_cellula(t, (i32)l,
                (i32)col));
        }
        mandata_textus(m, ZEPHYRUM, l * ch, linea, ZEPHYRUM,
            color_thematis((l >= sel_a && l <= sel_b) ? COLOR_BACKGROUND
                                                      : COLOR_TEXT));
    }

    /* character sub cursore colore chartae */
    si (   cursor.x >= ZEPHYRUM && cursor.y >= ZEPHYRUM
        && cursor.x < (s32)t->latitudo && cursor.y < (s32)t->altitudo)
    {
        sub = visibilis(tabula_cellula(t, (i32)cursor.y,
            (i32)cursor.x));
        si (sub != ' ')
        {
            linea.datum = (i8*)piscina_allocare(m->piscina, I);
            linea.datum[ZEPHYRUM] = (i8)sub;
            linea.mensura = I;
            mandata_textus(m, cursor.x * cw, cursor.y * ch, linea,
                           ZEPHYRUM, color_thematis(COLOR_BACKGROUND));
        }
    }
}

/* <purus/> status: "MODUS l:c" - modus colore suo, positio textu */
vacuum
scriba_figura_status (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    chorda modus;
    chorda reliquum;
       s32 y;
       i32 i;

    (vacuum)thema;
    (vacuum)ctx;
    mandata_rectangulum(m, fines(ZEPHYRUM, ZEPHYRUM, c->fines.latitudo,
        c->fines.altitudo), color_thematis(COLOR_BACKGROUND), VERUM);
    y = (c->fines.altitudo > VIII) ? (c->fines.altitudo - VIII) / II
                                   : ZEPHYRUM;
    modus = c->titulus;
    per (i = ZEPHYRUM; i < c->titulus.mensura; i++)
    {
        si (c->titulus.datum[i] == ' ')
        {
            frange;
        }
    }
    modus.mensura     = i;
    reliquum.datum    = c->titulus.datum + i;
    reliquum.mensura  = c->titulus.mensura - i;
    mandata_textus(m, II, y, modus, ZEPHYRUM,
        color_thematis(chorda_aequalis_literis(modus, "INSERERE")
                       ? COLOR_STATUS_INSERT
                       : chorda_aequalis_literis(modus, "VISUALIS")
                       ? COLOR_STATUS_VISUAL : COLOR_STATUS_NORMAL));
    si (reliquum.mensura > ZEPHYRUM)
    {
        mandata_textus(m, II + (s32)modus.mensura * VI, y, reliquum,
                       ZEPHYRUM, color_thematis(COLOR_TEXT));
    }
}

vacuum
scriba_figurae_registrare (
    FiguraRegistrum* reg,
                i32  thema,
      ScribaFigurae* ctx)
{
    si (!reg || !ctx)
    {
        redde;
    }
    figura_registrare(reg, PARTES_PROSPECTUS, thema,
        scriba_figura_mensae,
                      ctx);
    figura_registrare(reg, PARTES_CAMPUS, thema, scriba_figura_folii,
        ctx);
    figura_registrare(reg, PARTES_TITULUS, thema, scriba_figura_status,
                      ctx);
}
