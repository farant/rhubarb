/* destinatio.c - strategia geometrica ordinaria */

#include "destinatio.h"


/* ==================================================
 * Auxilia
 * ================================================== */

/* Eventus muris CUM puncto. Rotula punctum in Eventu non fert
 * (unio: datum.rotula sola) - ad focum it. */
interior b32
est_muris_positus (
    eventus_genus_t g)
{
    redde g == EVENTUS_MUS_DEPRESSUS || g == EVENTUS_MUS_LIBERATUS
        || g == EVENTUS_MUS_MOTUS || g == EVENTUS_MUS_DUPLEX;
}

interior b32
est_focalis (
    eventus_genus_t g)
{
    redde g == EVENTUS_CLAVIS_DEPRESSUS || g == EVENTUS_CLAVIS_LIBERATUS
        || g == EVENTUS_MUS_ROTULA;
}

interior chorda
chorda_nulla (vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

/* p in spatio PARENTIS c. Redde destinatum intimum, punctum locale
 * scriptum in *locale; NIHIL si nihil tactum. Liberi ultimo-primi
 * (summus pictus = summus tactus). */
/* p in spatio PARENTIS c. Redde destinatum intimum, punctum locale
 * scriptum in *locale; NIHIL si nihil tactum. Liberi ultimo-primi
 * (summus pictus = summus tactus). Spatium signatum: liberi cum
 * finibus negativis legitimi (P3 T1). */
interior Componens*
invenire (
     Componens* c,
       Punctum  p,
       Punctum* locale)
{
     Componens* tactum;
       Punctum  q;
           s32  scala;
           i32  i;
           b32  intra;

    intra = fines_continet(c->fines, p);
    si (c->sectio && !intra)
    {
        redde NIHIL;
    }
    scala = (s32)c->scala;
    si (scala <= ZEPHYRUM)
    {
        scala = I;
    }
    /* in spatium liberorum: origo c, translatio, scala */
    q.x  = (p.x - c->fines.x - c->translatio.x) / scala;
    q.y  = (p.y - c->fines.y - c->translatio.y) / scala;
    i    = componens_numerus_liberorum(c);
    dum (i > ZEPHYRUM)
    {
        i--;
        tactum = invenire(componens_liberum(c, i), q, locale);
        si (tactum)
        {
            redde tactum;
        }
    }
    si (intra)
    {
        locale->x = p.x - c->fines.x;
        locale->y = p.y - c->fines.y;
        redde c;
    }
    redde NIHIL;
}

interior Xar*
ascensus_ex (
    Componens* c,
      Piscina* piscina)
{
          Xar*  a;
    Componens** sedes;

    a = xar_creare(piscina, (i32)magnitudo(Componens*));
    dum (c)
    {
        sedes   = (Componens**)xar_addere(a);
        *sedes  = c;
        c       = c->parens;
    }
    redde a;
}


/* ==================================================
 * Strategia ordinaria
 * ================================================== */

Destinatio
destinatio_geometrica (
           Componens* arbor,
      constans Motus* motus,
              chorda  focus,
    constans Eventus* ev,
             Piscina* piscina)
{
    Destinatio  d;
     Componens* geo;
     Componens* destinatum;
       Punctum  p;

    d.id_captum         = chorda_nulla();
    d.id_geometricum    = chorda_nulla();
    d.punctum_locale.x  = ZEPHYRUM;
    d.punctum_locale.y  = ZEPHYRUM;
    d.ascensus          = NIHIL;
    si (!arbor || !ev || !piscina)
    {
        redde d;
    }
    destinatum = arbor;

    si (est_muris_positus(ev->genus))
    {
                p.x  = (s32)ev->datum.mus.x;
        p.y          = (s32)ev->datum.mus.y;
        geo          = invenire(arbor, p, &d.punctum_locale);
        si (geo)
        {
            d.id_geometricum  = geo->id;
            destinatum        = geo;
        }
        si (motus && !chorda_vacua(motus->captura))
        {
            d.id_captum = motus->captura;
            destinatum  = componens_invenire_per_id(arbor,
                                                    motus->captura);
            si (!destinatum)
            {
                destinatum = geo ? geo : arbor;
            }
        }
    }
    alioquin si (est_focalis(ev->genus) && !chorda_vacua(focus))
    {
        destinatum = componens_invenire_per_id(arbor, focus);
        si (!destinatum)
        {
            destinatum = arbor;
        }
    }
    d.ascensus = ascensus_ex(destinatum, piscina);
    redde d;
}

Componens*
destinatio_componens (
    constans Destinatio* d)
{
    si (!d || !d->ascensus || xar_numerus(d->ascensus) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde *(Componens**)xar_obtinere(d->ascensus, ZEPHYRUM);
}

Destinatio
destinatio_ex_componente (
    Componens* c,
      Piscina* piscina)
{
    Destinatio d;

    d.id_captum = chorda_nulla();
    d.id_geometricum = c ? c->id : chorda_nulla();
    d.punctum_locale.x = ZEPHYRUM;
    d.punctum_locale.y = ZEPHYRUM;
    d.ascensus = c && piscina ? ascensus_ex(c, piscina) : NIHIL;
    redde d;
}


/* ==================================================
 * Ordo foci
 * ================================================== */

interior vacuum
colligere_focusabiles (
    Componens* c,
          Xar* tabula)
{
     Componens** sedes;
           i32   i;
           i32   n;

    si (c->focusabilis)
    {
        sedes   = (Componens**)xar_addere(tabula);
        *sedes  = c;
    }
    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++)
    {
        colligere_focusabiles(componens_liberum(c, i), tabula);
    }
}

interior chorda
focus_gradu (
     Componens* arbor,
        chorda  focus,
           s32  gradus,
       Piscina* piscina)
{
    Xar* tabula;
    i32  n;
    i32  i;
    s32  k;

    si (!arbor || !piscina)
    {
        redde chorda_nulla();
    }
    tabula = xar_creare(piscina, (i32)magnitudo(Componens*));
    colligere_focusabiles(arbor, tabula);
    n = xar_numerus(tabula);
    si (n == ZEPHYRUM)
    {
        redde chorda_nulla();
    }
    k = -I;
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (chorda_aequalis(
                (*(Componens**)xar_obtinere(tabula, i))->id, focus))
        {
            k = (s32)i;
            frange;
        }
    }
    si (k < ZEPHYRUM)
    {
        /* ignotus aut vacuus: proximum = primus,
         * praecedens = ultimus */
        k = gradus > ZEPHYRUM ? -I : ZEPHYRUM;
    }
    k = (k + gradus + (s32)n) % (s32)n;
    redde (*(Componens**)xar_obtinere(tabula, (i32)k))->id;
}

chorda
destinatio_focus_proximum (
     Componens* arbor,
        chorda  focus,
       Piscina* piscina)
{
    redde focus_gradu(arbor, focus, I, piscina);
}

chorda
destinatio_focus_praecedens (
     Componens* arbor,
        chorda  focus,
       Piscina* piscina)
{
    redde focus_gradu(arbor, focus, -I, piscina);
}
