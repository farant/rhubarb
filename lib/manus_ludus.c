/* manus_ludus.c - manus in processu */

#include "manus_ludus.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
chorda_nulla (vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

interior vacuum
causam_ponere (
            ManusLudus* m,
    constans character* litterae)
{
    m->causa = chorda_ex_literis(litterae, m->piscina);
}


/* ==================================================
 * Vita
 * ================================================== */

ManusLudus*
manus_ludus_creare (
        Piscina* piscina,
    Dispensator* d)
{
    ManusLudus* m;

    si (!piscina || !d)
    {
        redde NIHIL;
    }
    m = (ManusLudus*)piscina_allocare(piscina, magnitudo(ManusLudus));
    si (!m)
    {
        redde NIHIL;
    }
    m->d          = d;
    m->tempus     = M;
    m->gradus_ms  = L;
    m->piscina    = piscina;
    m->causa      = chorda_nulla();
    redde m;
}


/* ==================================================
 * Selectores
 * ================================================== */

nomen enumeratio {
    SEL_ID,
    SEL_PARTES,
    SEL_ACTIO,
    SEL_TITULUS,
    SEL_MALUS
} SelGenus;

/* '#id' | '[partes=x]' | '[actio=x]' | '[titulus=x]' */
interior SelGenus
selector_legere (
    constans character* sel,
                chorda* valor,
               Piscina* p)
{
    constans character* aeq;
    constans character* fin;
        memoriae_index  n;

    si (!sel || sel[ZEPHYRUM] == ZEPHYRUM)
    {
        redde SEL_MALUS;
    }
    si (sel[ZEPHYRUM] == '#')
    {
        *valor = chorda_ex_literis(sel + I, p);
        redde SEL_ID;
    }
    si (sel[ZEPHYRUM] != '[')
    {
        redde SEL_MALUS;
    }
    aeq = strchr(sel, '=');
    fin = strchr(sel, ']');
    si (!aeq || !fin || fin < aeq)
    {
        redde SEL_MALUS;
    }
    n               = (memoriae_index)(fin - aeq - I);
    valor->mensura  = (i32)n;
    valor->datum    = (i8*)piscina_allocare(p, n + I);
    si (!valor->datum)
    {
        redde SEL_MALUS;
    }
    memcpy(valor->datum, aeq + I, n);
    si (strncmp(sel + I, "partes=", VII) == ZEPHYRUM)
    {
        redde SEL_PARTES;
    }
    si (strncmp(sel + I, "actio=", VI) == ZEPHYRUM)
    {
        redde SEL_ACTIO;
    }
    si (strncmp(sel + I, "titulus=", VIII) == ZEPHYRUM)
    {
        redde SEL_TITULUS;
    }
    redde SEL_MALUS;
}

interior Componens*
quaerere (
     Componens* c,
      SelGenus  g,
        chorda  v)
{
    constans character* partes_t;
             Componens* r;
                   i32  i;
                   i32  n;
                   b32  congruit;

    commutatio (g)
    {
        casus SEL_ID:
            congruit = chorda_aequalis(c->id, v);
            frange;
        casus SEL_PARTES:
            partes_t = partes_titulus(c->partes);
            congruit = chorda_aequalis_literis(v, partes_t);
            frange;
        casus SEL_ACTIO:
            congruit = chorda_aequalis(c->actio, v);
            frange;
        casus SEL_TITULUS:
            congruit = chorda_aequalis(c->titulus, v);
            frange;
        ordinarius:
            congruit = FALSUM;
            frange;
    }
    si (congruit)
    {
        redde c;
    }
    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++)
    {
        r = quaerere(componens_liberum(c, i), g, v);
        si (r)
        {
            redde r;
        }
    }
    redde NIHIL;
}

Componens*
manus_ludus_invenire (
            ManusLudus* m,
    constans character* selector)
{
    SelGenus g;
      chorda v;

    si (!m)
    {
        redde NIHIL;
    }
    g = selector_legere(selector, &v, m->piscina);
    si (g == SEL_MALUS)
    {
        causam_ponere(m, "selector malus: '#id' | '[partes|actio|"
                         "titulus=x]'");
        redde NIHIL;
    }
    si (!dispensator_arbor(m->d))
    {
        redde NIHIL;
    }
    redde quaerere(dispensator_arbor(m->d), g, v);
}

Punctum
manus_ludus_ad_schirmum (
     ManusLudus* m,
      Componens* c,
        Punctum  locale)
{
       Punctum  p;
     Componens* a;
           s32  x;
           s32  y;

    (vacuum)m;
    p = locale;
    si (!c)
    {
        redde p;
    }
    x = (s32)c->fines.x + (s32)locale.x;
    y = (s32)c->fines.y + (s32)locale.y;
    per (a = c->parens; a; a = a->parens)
    {
        x = x * (s32)a->scala + (s32)a->fines.x + (s32)a->translatio.x;
        y = y * (s32)a->scala + (s32)a->fines.y + (s32)a->translatio.y;
    }
    p.x = (i32)x;
    p.y = (i32)y;
    redde p;
}


/* ==================================================
 * Actus
 * ================================================== */

interior vacuum
mus (
         ManusLudus* m,
    eventus_genus_t  genus,
                i32  x,
                i32  y)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = genus;
    e.tempus       = m->tempus;
    e.datum.mus.x  = x;
    e.datum.mus.y  = y;
    dispensator_tractare(m->d, &e);
    m->tempus += m->gradus_ms;
}

b32
manus_ludus_premere_ad (
     ManusLudus* m,
            i32  x,
            i32  y)
{
    si (!m)
    {
        redde FALSUM;
    }
    mus(m, EVENTUS_MUS_DEPRESSUS, x, y);
    mus(m, EVENTUS_MUS_LIBERATUS, x, y);
    redde VERUM;
}

b32
manus_ludus_premere (
            ManusLudus* m,
    constans character* selector)
{
     Componens* c;
       Punctum  centrum;

    c = manus_ludus_invenire(m, selector);
    si (!c)
    {
        si (m && chorda_vacua(m->causa))
        {
            causam_ponere(m, "premere: selector nihil invenit");
        }
        redde FALSUM;
    }
    centrum.x  = c->fines.latitudo / II;
    centrum.y  = c->fines.altitudo / II;
    centrum    = manus_ludus_ad_schirmum(m, c, centrum);
    redde manus_ludus_premere_ad(m, centrum.x, centrum.y);
}

b32
manus_ludus_movere (
     ManusLudus* m,
            i32  x,
            i32  y)
{
    si (!m)
    {
        redde FALSUM;
    }
    mus(m, EVENTUS_MUS_MOTUS, x, y);
    redde VERUM;
}

b32
manus_ludus_trahere (
            ManusLudus* m,
    constans character* selector,
      constans Punctum* puncta,
                   i32  n)
{
     Componens* c;
       Punctum  p;
           i32  i;

    c = manus_ludus_invenire(m, selector);
    si (!c || !puncta || n < I)
    {
        si (m)
        {
            causam_ponere(m, "trahere: selector aut puncta");
        }
        redde FALSUM;
    }
    p = manus_ludus_ad_schirmum(m, c, puncta[ZEPHYRUM]);
    mus(m, EVENTUS_MUS_DEPRESSUS, p.x, p.y);
    per (i = I; i < n; i++)
    {
        p = manus_ludus_ad_schirmum(m, c, puncta[i]);
        mus(m, EVENTUS_MUS_MOTUS, p.x, p.y);
    }
    mus(m, EVENTUS_MUS_LIBERATUS, p.x, p.y);
    redde VERUM;
}

b32
manus_ludus_clavem (
     ManusLudus* m,
      character  typus,
            i32  modificantes)
{
    Eventus e;

    si (!m)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus                     = m->tempus;
    e.datum.clavis.typus         = typus;
    e.datum.clavis.modificantes  = modificantes;
    dispensator_tractare(m->d, &e);
    m->tempus  += m->gradus_ms;
    e.genus    = EVENTUS_CLAVIS_LIBERATUS;
    e.tempus   = m->tempus;
    dispensator_tractare(m->d, &e);
    m->tempus += m->gradus_ms;
    redde VERUM;
}

vacuum
manus_ludus_exspectare (
     ManusLudus* m,
            s64  ms)
{
    si (!m)
    {
        redde;
    }
    m->tempus += ms;
    dispensator_pulsare(m->d, m->tempus);
}


/* ==================================================
 * Lectio
 * ================================================== */

b32
manus_ludus_existit (
            ManusLudus* m,
    constans character* selector)
{
    redde manus_ludus_invenire(m, selector) != NIHIL;
}

chorda
manus_ludus_focus (
    ManusLudus* m)
{
    si (!m)
    {
        redde chorda_nulla();
    }
    redde dispensator_focus(m->d);
}

chorda
manus_ludus_causa (
    constans ManusLudus* m)
{
    si (!m)
    {
        redde chorda_nulla();
    }
    redde m->causa;
}
