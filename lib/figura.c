/* figura.c - registrum figurarum et pingere */

#include "figura.h"
#include "thema.h"


/* ==================================================
 * Registrum
 * ================================================== */

interior chorda
chorda_vacua_figurae (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

/* spatia vacua aequalia sine memcmp (datum NIHIL) */
interior b32
spatia_aequalia (
    chorda a,
    chorda b)
{
    si (a.mensura == ZEPHYRUM && b.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde chorda_aequalis(a, b);
}

interior FiguraIntroitus*
introitus_invenire (
    constans FiguraRegistrum* reg,
                      chorda  spatium,
                      Partes  partes,
                         i32  thema)
{
    FiguraIntroitus* f;
                i32  i;
                i32  n;

    n = xar_numerus(reg->introitus);
    per (i = ZEPHYRUM; i < n; i++)
    {
        f = (FiguraIntroitus*)xar_obtinere(reg->introitus, i);
        si (   f->partes == partes && f->thema == thema
            && spatia_aequalia(f->spatium, spatium))
        {
            redde f;
        }
    }
    redde NIHIL;
}

FiguraRegistrum*
figura_registrum_creare (
    Piscina* piscina)
{
    FiguraRegistrum* reg;

    si (!piscina)
    {
        redde NIHIL;
    }
    reg = (FiguraRegistrum*)piscina_allocare(piscina,
                                            magnitudo(*reg));
    si (!reg)
    {
        redde NIHIL;
    }
    reg->introitus = xar_creare(piscina,
                                (i32)magnitudo(FiguraIntroitus));
    reg->piscina = piscina;
    redde reg;
}

b32
figura_registrare (
    FiguraRegistrum* reg,
             Partes  partes,
                i32  thema,
           FiguraFn  fn,
             vacuum* ctx)
{
    FiguraIntroitus* f;

    si (!reg || !fn)
    {
        redde FALSUM;
    }
    si (introitus_invenire(reg, chorda_vacua_figurae(), partes, thema))
    {
        redde FALSUM;
    }
    f           = (FiguraIntroitus*)xar_addere(reg->introitus);
    f->partes   = partes;
    f->thema    = thema;
    f->fn       = fn;
    f->ctx      = ctx;
    f->spatium  = chorda_vacua_figurae();
    redde VERUM;
}

vacuum
figura_registrum_vacare (
    FiguraRegistrum* reg)
{
    si (!reg)
    {
        redde;
    }
    xar_vacare(reg->introitus);
}

/* spatium NIHIL = spatium cuiusque introitus servatur */
interior b32
miscere (
             FiguraRegistrum* reg,
    constans FiguraRegistrum* fons,
             constans chorda* spatium)
{
     FiguraIntroitus* f;
     FiguraIntroitus* novus;
              chorda  s;
                 i32  i;
                 i32  k;

    si (!reg || !fons)
    {
        redde FALSUM;
    }
    k = xar_numerus(fons->introitus);
    per (i = ZEPHYRUM; i < k; i++)
    {
        f = (FiguraIntroitus*)xar_obtinere(fons->introitus, i);
        s = spatium && chorda_vacua(f->spatium) ? *spatium : f->spatium;
        si (introitus_invenire(reg, s, f->partes, f->thema))
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < k; i++)
    {
        f       = (FiguraIntroitus*)xar_obtinere(fons->introitus, i);
        novus   = (FiguraIntroitus*)xar_addere(reg->introitus);
        *novus  = *f;
        /* S8c: spatium datum solum introitibus sine spatio */
        si (spatium && chorda_vacua(f->spatium))
        {
            novus->spatium = *spatium;
        }
    }
    redde VERUM;
}

b32
figura_registrum_miscere (
             FiguraRegistrum* reg,
    constans FiguraRegistrum* fons)
{
    redde miscere(reg, fons, NIHIL);
}

b32
figura_registrum_miscere_in_spatio (
             FiguraRegistrum* reg,
    constans FiguraRegistrum* fons,
                      chorda  spatium)
{
    redde miscere(reg, fons, &spatium);
}

b32
figura_invenire (
    constans FiguraRegistrum*  reg,
                      Partes   partes,
                         i32   thema,
                    FiguraFn*  fn_ex,
                      vacuum** ctx_ex)
{
    redde figura_invenire_in_spatio(reg, chorda_vacua_figurae(), partes,
        thema, fn_ex, ctx_ex);
}

b32
figura_invenire_in_spatio (
    constans FiguraRegistrum*  reg,
                      chorda   spatium,
                      Partes   partes,
                         i32   thema,
                    FiguraFn*  fn_ex,
                      vacuum** ctx_ex)
{
    FiguraIntroitus* f;

    si (!reg || !fn_ex || !ctx_ex)
    {
        redde FALSUM;
    }
    f = introitus_invenire(reg, spatium, partes, thema);
    si (!f)
    {
        redde FALSUM;
    }
    *fn_ex   = f->fn;
    *ctx_ex  = f->ctx;
    redde VERUM;
}


/* ==================================================
 * Pingere
 * ================================================== */

/* Coetus per componens; figura ante liberos (parens sub liberis
 * pingitur = ordo z). Spatium parentis descendit nisi componens suum
 * aperit (S2a). */
interior vacuum
pingere_nodum (
          constans Componens* c,
    constans FiguraRegistrum* reg,
                         i32  thema,
                     Mandata* m,
                      chorda  spatium)
{
    FiguraFn  fn;
      vacuum* ctx;
         i32  coetus;
         i32  i;
         i32  n;

    si (!chorda_vacua(c->spatium))
    {
        spatium = c->spatium;
    }
    coetus = mandata_coetus_incipere(m, c->fines, c->sectio,
                                     c->translatio.x, c->translatio.y,
                                     c->scala, c->id);
    si (figura_invenire_in_spatio(reg, spatium, c->partes, thema, &fn,
            &ctx))
    {
        fn(c, m, thema, ctx);
    }
    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++)
    {
        pingere_nodum(componens_liberum(c, i), reg, thema, m, spatium);
    }
    mandata_coetus_finire(m, coetus);
}

/* <purus/> */
vacuum
pingere (
          constans Componens* radix,
    constans FiguraRegistrum* reg,
                         i32  thema,
                     Mandata* m)
{
    si (!radix || !reg || !m)
    {
        redde;
    }
    pingere_nodum(radix, reg, thema, m, componens_spatium(radix));
}

/* <purus/> */
vacuum
figura_finium (
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
    mandata_rectangulum(m, f, figura_color_thematis(COLOR_BORDER),
        FALSUM);
}

vacuum
figura_mensam_pingere (
    Mandata* md,
      Fines  fines,
        b32  focatum)
{
    mandata_rectangulum(md, fines, figura_color_thematis(
        COLOR_SUPERFICIES), VERUM);
    si (focatum)
    {
        mandata_rectangulum_exemplar(md, fines, figura_color_thematis(
            COLOR_BORDER), FIGURA_EXEMPLAR_FOCI);
    }
}

ColorMandati
figura_color_thematis (
    ColorThema color)
{
    ColorMandati cm;

    cm.genus = COLOR_MANDATI_THEMA;
    cm.valor = (i32)color;
    redde cm;
}


/* ==================================================
 * Instrumenta visus (V1)
 * ================================================== */

#define FIGURA_LITTERA  VI     /* latitudo glyphi 6x8 */
#define FIGURA_LINEA    VIII   /* altitudo glyphi */

interior Fines
fines_figurae (
    s32 x,
    s32 y,
    s32 latitudo,
    s32 altitudo)
{
    Fines f;

    f.x         = x;
    f.y         = y;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    redde f;
}

interior vacuum
lineam_ducere (
       Mandata* md,
           s32  x0,
           s32  y0,
           s32  x1,
           s32  y1,
           i32  crassitudo,
    ColorThema  color)
{
    Punctum a;
    Punctum b;

    a.x = x0;
    a.y = y0;
    b.x = x1;
    b.y = y1;
    mandata_linea(md, a, b, crassitudo, figura_color_thematis(color));
}

/* textus centratus in fines, in piscinam mandatorum transcriptus */
interior vacuum
textum_centrare (
       Mandata* md,
         Fines  f,
        chorda  textus,
    ColorThema  color)
{
    mandata_textus(md, f.x + (f.latitudo - (s32)textus.mensura
        * FIGURA_LITTERA) / II, f.y + (f.altitudo - FIGURA_LINEA) / II,
        chorda_transcribere(textus, md->piscina), ZEPHYRUM,
        figura_color_thematis(color));
}

/* corpus (cum umbra II minus, umbra prius) */
interior Fines
corpus_cum_umbra (
    Mandata* md,
      Fines  f,
        i32  status)
{
    si (status & FIGURA_UMBRA)
    {
        f.latitudo -= II;
        f.altitudo -= II;
        mandata_rectangulum(md, fines_figurae(f.x + II, f.y + II,
            f.latitudo, f.altitudo), figura_color_thematis(COLOR_TEXT),
            VERUM);
    }
    redde f;
}

vacuum
figura_globulum_pingere (
    Mandata* md,
      Fines  f,
     chorda  textus,
        i32  status)
{
    f = corpus_cum_umbra(md, f, status);
    mandata_rectangulum(md, f, figura_color_thematis((status
        & FIGURA_ELECTUM) ? COLOR_SELECTION : COLOR_BACKGROUND), VERUM);
    mandata_rectangulum(md, f, figura_color_thematis(COLOR_TEXT),
        FALSUM);
    textum_centrare(md, f, textus, (status & FIGURA_OBSCURUM)
        ? COLOR_TEXT_DIM : COLOR_TEXT);
}

vacuum
figura_sagittam_pingere (
    Mandata* md,
      Fines  f,
        s32  directio,
        i32  status)
{
    Punctum t[III];
        s32 cx;
        s32 cy;

    f = corpus_cum_umbra(md, f, status);
    mandata_rectangulum(md, f, figura_color_thematis((status
        & FIGURA_ELECTUM) ? COLOR_SELECTION : COLOR_BACKGROUND), VERUM);
    mandata_rectangulum(md, f, figura_color_thematis(COLOR_TEXT),
        FALSUM);
    cx             = f.x + f.latitudo / II;
    cy             = f.y + f.altitudo / II;
    t[ZEPHYRUM].x  = cx - II * directio;
    t[ZEPHYRUM].y  = cy - IV;
    t[I].x         = cx - II * directio;
    t[I].y         = cy + IV;
    t[II].x        = cx + II * directio;
    t[II].y        = cy;
    mandata_polygonum(md, t, III, figura_color_thematis((status
        & FIGURA_OBSCURUM) ? COLOR_TEXT_DIM : COLOR_TEXT), VERUM);
}

vacuum
figura_segmentum_pingere (
    Mandata* md,
      Fines  f,
     chorda  textus,
        i32  positio,
        i32  status)
{
    b32 ultimum;

    ultimum = positio == FIGURA_SEGMENTUM_ULTIMUM;
    /* umbra: fascia infra (et dextra ultimi) - continua per segmenta */
    si (status & FIGURA_UMBRA)
    {
        f.altitudo -= II;
        si (ultimum)
        {
            f.latitudo -= II;
        }
        mandata_rectangulum(md, fines_figurae(f.x + II, f.y + II,
            f.latitudo, f.altitudo), figura_color_thematis(COLOR_TEXT),
            VERUM);
    }
    mandata_rectangulum(md, f, figura_color_thematis((status
        & FIGURA_ELECTUM) ? COLOR_SELECTION : COLOR_BACKGROUND), VERUM);
    lineam_ducere(md, f.x, f.y, f.x + f.latitudo - I, f.y, I,
        COLOR_TEXT);
    lineam_ducere(md, f.x, f.y + f.altitudo - I, f.x + f.latitudo - I,
        f.y + f.altitudo - I, I, COLOR_TEXT);
    lineam_ducere(md, f.x, f.y, f.x, f.y + f.altitudo - I, I,
        COLOR_TEXT);
    si (ultimum)
    {
        lineam_ducere(md, f.x + f.latitudo - I, f.y, f.x + f.latitudo
            - I, f.y + f.altitudo - I, I, COLOR_TEXT);
    }
    textum_centrare(md, f, textus, (status & FIGURA_OBSCURUM)
        ? COLOR_TEXT_DIM : COLOR_TEXT);
}

vacuum
figura_capsam_pingere (
    Mandata* md,
        s32  x,
        s32  y,
        b32  factum)
{
    mandata_rectangulum(md, fines_figurae(x, y, IX, IX),
        figura_color_thematis(COLOR_BACKGROUND), VERUM);
    mandata_rectangulum(md, fines_figurae(x, y, IX, IX),
        figura_color_thematis(COLOR_TEXT), FALSUM);
    si (factum)
    {
        lineam_ducere(md, x + II, y + IV, x + IV, y + VI, II,
            COLOR_TEXT);
        lineam_ducere(md, x + IV, y + VI, x + VII, y + II, II,
            COLOR_TEXT);
    }
}

vacuum
figura_ordinem_electum_pingere (
    Mandata* md,
      Fines  f)
{
    mandata_rectangulum(md, f, figura_color_thematis(COLOR_SELECTION),
        VERUM);
    mandata_rectangulum(md, fines_figurae(f.x, f.y, III, f.altitudo),
        figura_color_thematis(COLOR_ACCENT_PRIMARY), VERUM);
}

vacuum
figura_fasciam_pingere (
    Mandata* md,
      Fines  f)
{
    mandata_rectangulum(md, f, figura_color_thematis(COLOR_BACKGROUND),
        VERUM);
    mandata_rectangulum_exemplar(md, f, figura_color_thematis(
        COLOR_SUPERFICIES), EXEMPLAR_CINEREUM_LEVE);
}

vacuum
figura_umbram_pingere (
    Mandata* md,
      Fines  f)
{
    mandata_rectangulum(md, fines_figurae(f.x + II, f.y + II,
        f.latitudo, f.altitudo), figura_color_thematis(COLOR_TEXT),
        VERUM);
}

vacuum
figura_campum_pingere (
    Mandata* md,
      Fines  f)
{
    mandata_rectangulum(md, f, figura_color_thematis(COLOR_SUPERFICIES),
        VERUM);
    mandata_rectangulum(md, f, figura_color_thematis(COLOR_TEXT),
        FALSUM);
}

vacuum
figura_textum_audacem (
         Mandata* md,
             s32  x,
             s32  y,
          chorda  textus,
    ColorMandati  color)
{
    chorda copia;

    copia = chorda_transcribere(textus, md->piscina);
    mandata_textus(md, x, y, copia, ZEPHYRUM, color);
    mandata_textus(md, x + I, y, copia, ZEPHYRUM, color);
}
