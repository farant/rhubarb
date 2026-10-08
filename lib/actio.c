/* actio.c - registrum actionum nominatarum */

#include "actio.h"


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
chorda_vacua_actionis (vacuum)
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

interior s32
index_nominis (
    constans ActioRegistrum* reg,
                     chorda  spatium,
                     chorda  titulus)
{
    i32 i;
    i32 n;

    n = xar_numerus(reg->nomina);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (   chorda_aequalis(*(chorda*)xar_obtinere(reg->nomina, i),
                   titulus)
            && spatia_aequalia(*(chorda*)xar_obtinere(reg->spatia, i),
                   spatium))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* actio relata: nomen in spatio componentis */
nomen structura {
    chorda spatium;
    chorda titulus;
} Relatio;

interior b32
continet_relationem (
    constans Xar* tabula,
          chorda  spatium,
          chorda  titulus)
{
    Relatio* r;
        i32  i;
        i32  n;

    n = xar_numerus(tabula);
    per (i = ZEPHYRUM; i < n; i++)
    {
        r = (Relatio*)xar_obtinere(tabula, i);
        si (   chorda_aequalis(r->titulus, titulus)
            && spatia_aequalia(r->spatium, spatium))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Actiones relatae in arbore, semel quaeque per spatium */
interior vacuum
colligere_actiones (
    Componens* c,
       chorda  spatium,
          Xar* tabula)
{
    Relatio* r;
        i32  i;
        i32  n;

    si (!chorda_vacua(c->spatium))
    {
        spatium = c->spatium;
    }
    si (   !chorda_vacua(c->actio)
        && !continet_relationem(tabula, spatium, c->actio))
    {
        r           = (Relatio*)xar_addere(tabula);
        r->spatium  = spatium;
        r->titulus  = c->actio;
    }
    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++)
    {
        colligere_actiones(componens_liberum(c, i), spatium, tabula);
    }
}

/* introitus addere (collisio iam probata) */
interior vacuum
introitum_addere (
    ActioRegistrum* reg,
            chorda  spatium,
            chorda  titulus,
           ActioFn  fn,
            vacuum* ctx)
{
    *(chorda*)xar_addere(reg->nomina)       = titulus;
    *(ActioFn*)xar_addere(reg->functiones)  = fn;
    *(vacuum**)xar_addere(reg->contextus)   = ctx;
    *(chorda*)xar_addere(reg->spatia)       = spatium;
}

/* spatium NIHIL = spatium cuiusque introitus servatur */
interior b32
miscere (
             ActioRegistrum* reg,
    constans ActioRegistrum* fons,
            constans chorda* spatium)
{
    chorda s;
       i32 i;
       i32 k;

    si (!reg || !fons)
    {
        redde FALSUM;
    }
    k = xar_numerus(fons->nomina);
    /* collisio ulla: nihil additur */
    per (i = ZEPHYRUM; i < k; i++)
    {
        s = spatium ? *spatium : *(chorda*)xar_obtinere(fons->spatia,
            i);
        si (index_nominis(reg, s, *(chorda*)xar_obtinere(fons->nomina,
                i)) >= ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < k; i++)
    {
        s = spatium ? *spatium : *(chorda*)xar_obtinere(fons->spatia,
            i);
        introitum_addere(reg, s, *(chorda*)xar_obtinere(fons->nomina,
            i),
            *(ActioFn*)xar_obtinere(fons->functiones, i),
            *(vacuum**)xar_obtinere(fons->contextus, i));
    }
    redde VERUM;
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
    reg->spatia      = xar_creare(piscina, (i32)magnitudo(chorda));
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
    chorda* internata;

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
    si (index_nominis(reg, chorda_vacua_actionis(), *internata)
        >= ZEPHYRUM)
    {
        redde FALSUM;
    }
    introitum_addere(reg, chorda_vacua_actionis(), *internata, fn, ctx);
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
    xar_vacare(reg->spatia);
}

b32
actio_registrum_miscere (
             ActioRegistrum* reg,
    constans ActioRegistrum* fons)
{
    redde miscere(reg, fons, NIHIL);
}

b32
actio_registrum_miscere_in_spatio (
             ActioRegistrum* reg,
    constans ActioRegistrum* fons,
                     chorda  spatium)
{
    redde miscere(reg, fons, &spatium);
}

b32
actio_invenire (
    constans ActioRegistrum*  reg,
                     chorda   titulus,
                    ActioFn*  fn_ex,
                     vacuum** ctx_ex)
{
    redde actio_invenire_in_spatio(reg, chorda_vacua_actionis(),
        titulus,
        fn_ex, ctx_ex);
}

b32
actio_invenire_in_spatio (
    constans ActioRegistrum*  reg,
                     chorda   spatium,
                     chorda   titulus,
                    ActioFn*  fn_ex,
                     vacuum** ctx_ex)
{
    s32 k;

    si (!reg || !fn_ex || !ctx_ex)
    {
        redde FALSUM;
    }
    k = index_nominis(reg, spatium, titulus);
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
     Relatio* r;
      chorda* sedes;
         i32  i;
         i32  n;

    si (!reg || !piscina)
    {
        redde NIHIL;
    }
    relatae  = xar_creare(piscina, (i32)magnitudo(Relatio));
    desunt   = xar_creare(piscina, (i32)magnitudo(chorda));
    si (arbor)
    {
        colligere_actiones(arbor, chorda_vacua_actionis(), relatae);
    }
    n = xar_numerus(relatae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        r = (Relatio*)xar_obtinere(relatae, i);
        si (index_nominis(reg, r->spatium, r->titulus) < ZEPHYRUM)
        {
            sedes   = (chorda*)xar_addere(desunt);
            *sedes  = r->titulus;
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
    relatae = xar_creare(piscina, (i32)magnitudo(Relatio));
    otiosae = xar_creare(piscina, (i32)magnitudo(chorda));
    si (arbor)
    {
        colligere_actiones(arbor, chorda_vacua_actionis(), relatae);
    }
    n = xar_numerus(reg->nomina);
    per (i = ZEPHYRUM; i < n; i++)
    {
        t = *(chorda*)xar_obtinere(reg->nomina, i);
        si (!continet_relationem(relatae,
                *(chorda*)xar_obtinere(reg->spatia, i), t))
        {
            sedes   = (chorda*)xar_addere(otiosae);
            *sedes  = t;
        }
    }
    redde otiosae;
}
