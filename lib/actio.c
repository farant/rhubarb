/* actio.c - registrum actionum nominatarum */

#include "actio.h"


/* ==================================================
 * Auxilia
 * ================================================== */

interior s32
index_nominis (
    constans ActioRegistrum* reg,
                     chorda  titulus)
{
    i32 i;
    i32 n;

    n = xar_numerus(reg->nomina);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(reg->nomina, i),
                            titulus))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

interior b32
continet_chordam (
    constans Xar* tabula,
          chorda  t)
{
    i32 i;
    i32 n;

    n = xar_numerus(tabula);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(tabula, i), t))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Actiones relatae in arbore, semel quaeque */
interior vacuum
colligere_actiones (
    Componens* c,
          Xar* tabula)
{
    chorda* sedes;
       i32  i;
       i32  n;

    si (!chorda_vacua(c->actio) && !continet_chordam(tabula, c->actio))
    {
        sedes   = (chorda*)xar_addere(tabula);
        *sedes  = c->actio;
    }
    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++)
    {
        colligere_actiones(componens_liberum(c, i), tabula);
    }
}


/* ==================================================
 * Registrum
 * ================================================== */

ActioRegistrum*
actio_registrum_creare (
                Piscina* piscina,
    InternamentumChorda* intern)
{
    ActioRegistrum* reg;

    si (!piscina || !intern)
    {
        redde NIHIL;
    }
    reg = (ActioRegistrum*)piscina_allocare(piscina,
                                            magnitudo(ActioRegistrum));
    si (!reg)
    {
        redde NIHIL;
    }
    reg->nomina      = xar_creare(piscina, (i32)magnitudo(chorda));
    reg->functiones  = xar_creare(piscina, (i32)magnitudo(ActioFn));
    reg->contextus   = xar_creare(piscina, (i32)magnitudo(vacuum*));
    reg->piscina     = piscina;
    reg->intern      = intern;
    redde reg;
}

b32
actio_registrare (
        ActioRegistrum* reg,
    constans character* titulus,
               ActioFn  fn,
                vacuum* ctx)
{
     chorda*  internata;
     chorda*  n;
    ActioFn*  f;
     vacuum** c;

    si (!reg || !titulus || !fn)
    {
        redde FALSUM;
    }
    /* internamentum vacuum = NIHIL: titulus vacuus recusatur */
    internata = chorda_internare_ex_literis(reg->intern, titulus);
    si (!internata)
    {
        redde FALSUM;
    }
    si (index_nominis(reg, *internata) >= ZEPHYRUM)
    {
        redde FALSUM;
    }
    n   = (chorda*)xar_addere(reg->nomina);
    *n  = *internata;
    f   = (ActioFn*)xar_addere(reg->functiones);
    *f  = fn;
    c   = (vacuum**)xar_addere(reg->contextus);
    *c  = ctx;
    redde VERUM;
}

vacuum
actio_registrum_vacare (
    ActioRegistrum* reg)
{
    si (!reg)
    {
        redde;
    }
    xar_vacare(reg->nomina);
    xar_vacare(reg->functiones);
    xar_vacare(reg->contextus);
}

b32
actio_registrum_miscere (
             ActioRegistrum* reg,
    constans ActioRegistrum* fons)
{
      chorda*  n;
      chorda*  nn;
     ActioFn*  f;
      vacuum** c;
         i32   i;
         i32   k;

    si (!reg || !fons)
    {
        redde FALSUM;
    }
    k = xar_numerus(fons->nomina);
    /* collisio ulla: nihil additur */
    per (i = ZEPHYRUM; i < k; i++)
    {
        n = (chorda*)xar_obtinere(fons->nomina, i);
        si (index_nominis(reg, *n) >= ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < k; i++)
    {
        nn   = (chorda*)xar_addere(reg->nomina);
        *nn  = *(chorda*)xar_obtinere(fons->nomina, i);
        f    = (ActioFn*)xar_addere(reg->functiones);
        *f   = *(ActioFn*)xar_obtinere(fons->functiones, i);
        c    = (vacuum**)xar_addere(reg->contextus);
        *c   = *(vacuum**)xar_obtinere(fons->contextus, i);
    }
    redde VERUM;
}

b32
actio_invenire (
    constans ActioRegistrum*  reg,
                     chorda   titulus,
                    ActioFn*  fn_ex,
                     vacuum** ctx_ex)
{
    s32 k;

    si (!reg || !fn_ex || !ctx_ex)
    {
        redde FALSUM;
    }
    k = index_nominis(reg, titulus);
    si (k < ZEPHYRUM)
    {
        redde FALSUM;
    }
    *fn_ex   = *(ActioFn*)xar_obtinere(reg->functiones, (i32)k);
    *ctx_ex  = *(vacuum**)xar_obtinere(reg->contextus, (i32)k);
    redde VERUM;
}


/* ==================================================
 * Resolutio utrimque (L10)
 * ================================================== */

Xar*
actio_non_registratae (
    constans ActioRegistrum* reg,
                  Componens* arbor,
                    Piscina* piscina)
{
       Xar* relatae;
       Xar* desunt;
    chorda* sedes;
    chorda  t;
       i32  i;
       i32  n;

    si (!reg || !piscina)
    {
        redde NIHIL;
    }
    relatae  = xar_creare(piscina, (i32)magnitudo(chorda));
    desunt   = xar_creare(piscina, (i32)magnitudo(chorda));
    si (arbor)
    {
        colligere_actiones(arbor, relatae);
    }
    n = xar_numerus(relatae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        t = *(chorda*)xar_obtinere(relatae, i);
        si (index_nominis(reg, t) < ZEPHYRUM)
        {
            sedes   = (chorda*)xar_addere(desunt);
            *sedes  = t;
        }
    }
    redde desunt;
}

Xar*
actio_non_relatae (
    constans ActioRegistrum* reg,
                  Componens* arbor,
                    Piscina* piscina)
{
       Xar* relatae;
       Xar* otiosae;
    chorda* sedes;
    chorda  t;
       i32  i;
       i32  n;

    si (!reg || !piscina)
    {
        redde NIHIL;
    }
    relatae = xar_creare(piscina, (i32)magnitudo(chorda));
    otiosae = xar_creare(piscina, (i32)magnitudo(chorda));
    si (arbor)
    {
        colligere_actiones(arbor, relatae);
    }
    n = xar_numerus(reg->nomina);
    per (i = ZEPHYRUM; i < n; i++)
    {
        t = *(chorda*)xar_obtinere(reg->nomina, i);
        si (!continet_chordam(relatae, t))
        {
            sedes   = (chorda*)xar_addere(otiosae);
            *sedes  = t;
        }
    }
    redde otiosae;
}
