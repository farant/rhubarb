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
        s = spatium ? *spatium : f->spatium;
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
        si (spatium)
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
    ColorMandati color;

    (vacuum)thema;
    (vacuum)ctx;
    f.x          = ZEPHYRUM;
    f.y          = ZEPHYRUM;
    f.latitudo   = c->fines.latitudo;
    f.altitudo   = c->fines.altitudo;
    color.genus  = COLOR_MANDATI_THEMA;
    color.valor  = (i32)COLOR_BORDER;
    mandata_rectangulum(m, f, color, FALSUM);
}
