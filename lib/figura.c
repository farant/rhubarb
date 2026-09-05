/* figura.c - registrum figurarum et pingere */

#include "figura.h"
#include "thema.h"


/* ==================================================
 * Registrum
 * ================================================== */

interior FiguraIntroitus*
introitus_invenire (
    constans FiguraRegistrum* reg,
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
        si (f->partes == partes && f->thema == thema)
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
    si (introitus_invenire(reg, partes, thema))
    {
        redde FALSUM;
    }
    f          = (FiguraIntroitus*)xar_addere(reg->introitus);
    f->partes  = partes;
    f->thema   = thema;
    f->fn      = fn;
    f->ctx     = ctx;
    redde VERUM;
}

b32
figura_invenire (
    constans FiguraRegistrum*  reg,
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
    f = introitus_invenire(reg, partes, thema);
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
 * pingitur = ordo z). */
interior vacuum
pingere_nodum (
          constans Componens* c,
    constans FiguraRegistrum* reg,
                         i32  thema,
                     Mandata* m)
{
    FiguraFn  fn;
      vacuum* ctx;
         i32  coetus;
         i32  i;
         i32  n;

    coetus = mandata_coetus_incipere(m, c->fines, c->sectio,
                                     c->translatio.x, c->translatio.y,
                                     c->scala, c->id);
    si (figura_invenire(reg, c->partes, thema, &fn, &ctx))
    {
        fn(c, m, thema, ctx);
    }
    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++)
    {
        pingere_nodum(componens_liberum(c, i), reg, thema, m);
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
    pingere_nodum(radix, reg, thema, m);
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
