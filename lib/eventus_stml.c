/* eventus_stml.c - Eventus[] <-> STML */

#include "eventus_stml.h"
#include "stml.h"

#include <string.h>

/* Ordo = ordo enumerationis eventus_genus_t (fenestra.h). Extende
 * UNA cum enumeratione. Quinque ultimi = genera derivata a
 * dispensatore (T10); usque tum inertes, TITULI_NUMERUS custodit. */
hic_manens constans character* tituli[] = {
    "nihil", "claudere", "mutare_magnitudinem", "focus", "defocus",
    "exponere", "clavis_depressus", "clavis_liberatus", "mus_depressus",
    "mus_liberatus", "mus_motus", "mus_rotula", "mus_duplex",
    "mus_intravit", "mus_exiit", "focus_captus", "focus_amissus",
    "focus_petitus"
};
#define TITULI_NUMERUS ((i32)(magnitudo(tituli) / magnitudo(tituli[0])))


/* ==================================================
 * TITULI GENERUM
 * ================================================== */

constans character*
eventus_genus_titulus (
    eventus_genus_t genus)
{
    si ((i32)genus >= TITULI_NUMERUS)
    { redde "ignotum";
    }
    redde tituli[genus];
}

eventus_genus_t
eventus_genus_ex_titulo (
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < TITULI_NUMERUS; i++)
    {
        si (strcmp(titulus, tituli[i]) == ZEPHYRUM)
        { redde (eventus_genus_t)i;
        }
    }
    redde EVENTUS_NIHIL;
}


/* ==================================================
 * SCRIBERE
 * ================================================== */

interior vacuum
attr_s (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
     constans character* t,
                    s32  v)
{
    stml_attributum_addere(n, p, in, t, chorda_ut_cstr(chorda_ex_s32(v,
        p), p));
}

interior vacuum
attr_longus (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
     constans character* t,
                    s64  v)
{
    stml_attributum_addere(n, p, in, t,
        chorda_ut_cstr(chorda_ex_f64((f64)v, ZEPHYRUM, p), p));
}

interior vacuum
attr_f (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
     constans character* t,
                    f32  v)
{
    stml_attributum_addere(n, p, in, t,
        chorda_ut_cstr(chorda_ex_f64((f64)v, III, p), p));
}

chorda
eventus_scribere_stml (
           constans Xar* eventus,
                Piscina* piscina,
    InternamentumChorda* intern,
                    b32  pulchrum)
{
    StmlNodus* radix;
    StmlNodus* n;
      Eventus* e;
          s32  codex_typi;
          i32  i;
          i32  num;

    radix  = stml_elementum_creare(piscina, intern, "eventus_index");
    num    = xar_numerus(eventus);
    per (i = ZEPHYRUM; i < num; i++)
    {
        e = (Eventus*)xar_obtinere(eventus, i);
        n = stml_elementum_creare(piscina, intern, "eventus");
        stml_attributum_addere(n, piscina, intern, "genus",
                               eventus_genus_titulus(e->genus));
        attr_longus(n, piscina, intern, "tempus", e->tempus);
        commutatio (e->genus)
        {
            casus EVENTUS_MUS_DEPRESSUS:
            casus EVENTUS_MUS_LIBERATUS:
            casus EVENTUS_MUS_MOTUS:
            casus EVENTUS_MUS_DUPLEX:
                attr_s(n, piscina, intern, "x", (s32)e->datum.mus.x);
                attr_s(n, piscina, intern, "y", (s32)e->datum.mus.y);
                attr_s(n, piscina, intern, "botton",
                    (s32)e->datum.mus.botton);
                attr_s(n, piscina, intern, "modificantes",
                       (s32)e->datum.mus.modificantes);
                frange;
            casus EVENTUS_CLAVIS_DEPRESSUS:
            casus EVENTUS_CLAVIS_LIBERATUS:
                attr_s(n, piscina, intern, "clavis",
                    (s32)e->datum.clavis.clavis);
                codex_typi =
                    (s32)(insignatus character)e->datum.clavis.typus;
                attr_s(n, piscina, intern, "typus", codex_typi);
                attr_s(n, piscina, intern, "modificantes",
                       (s32)e->datum.clavis.modificantes);
                frange;
            casus EVENTUS_MUS_ROTULA:
                attr_f(n, piscina, intern, "delta_x",
                    e->datum.rotula.delta_x);
                attr_f(n, piscina, intern, "delta_y",
                    e->datum.rotula.delta_y);
                frange;
            casus EVENTUS_MUTARE_MAGNITUDINEM:
                attr_s(n, piscina, intern, "latitudo",
                       (s32)e->datum.mutare_magnitudinem.latitudo);
                attr_s(n, piscina, intern, "altitudo",
                       (s32)e->datum.mutare_magnitudinem.altitudo);
                frange;
            ordinarius:
                frange;
        }
        stml_liberum_addere(radix, n);
    }
    redde stml_scribere(radix, piscina, pulchrum);
}


/* ==================================================
 * LEGERE
 * ================================================== */

interior s32
capere_s (
             StmlNodus* n,
    constans character* t)
{
    chorda* c;
       s32  v;

    c = stml_attributum_capere(n, t);
    v = ZEPHYRUM;
    si (c)
    { chorda_ut_s32(*c, &v);
    }
    redde v;
}

interior s64
capere_longus (
             StmlNodus* n,
    constans character* t)
{
    chorda* c;
       f64  v;

    c = stml_attributum_capere(n, t);
    v = 0.0;
    si (c)
    { chorda_ut_f64(*c, &v);
    }
    redde (s64)v;
}

interior f32
capere_f (
             StmlNodus* n,
    constans character* t)
{
    chorda* c;
       f64  v;

    c = stml_attributum_capere(n, t);
    v = 0.0;
    si (c)
    { chorda_ut_f64(*c, &v);
    }
    redde (f32)v;
}

Xar*
eventus_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlResultus  res;
             Xar* index;
       StmlNodus* n;
         Eventus* e;
          chorda* g;

    res = stml_legere_ex_literis(cstr, piscina, intern);
    si (!res.successus || !res.elementum_radix)
    { redde NIHIL;
    }
    index  = xar_creare(piscina, (i32)magnitudo(Eventus));
    n      = stml_primus_liberum(res.elementum_radix);
    dum (n)
    {
        si (n->genus == STML_NODUS_ELEMENTUM)
        {
            e = (Eventus*)xar_addere(index);
            memset(e, ZEPHYRUM, magnitudo(Eventus));
            g = stml_attributum_capere(n, "genus");
            e->genus  = g ? eventus_genus_ex_titulo(chorda_ut_cstr(*g,
                piscina))
                          : EVENTUS_NIHIL;
            e->tempus = capere_longus(n, "tempus");
            si (   e->genus == EVENTUS_CLAVIS_DEPRESSUS
                || e->genus == EVENTUS_CLAVIS_LIBERATUS)
            {
                e->datum.clavis.clavis = (clavis_t)capere_s(n,
                    "clavis");
                e->datum.clavis.typus = (character)capere_s(n,
                    "typus");
                e->datum.clavis.modificantes = (i32)capere_s(n,
                    "modificantes");
            }
            alioquin si (e->genus == EVENTUS_MUS_ROTULA)
            {
                e->datum.rotula.delta_x = capere_f(n, "delta_x");
                e->datum.rotula.delta_y = capere_f(n, "delta_y");
            }
            alioquin si (e->genus == EVENTUS_MUTARE_MAGNITUDINEM)
            {
                e->datum.mutare_magnitudinem.latitudo = (i32)capere_s(n,
                    "latitudo");
                e->datum.mutare_magnitudinem.altitudo = (i32)capere_s(n,
                    "altitudo");
            }
            alioquin
            {
                e->datum.mus.x = (i32)capere_s(n, "x");
                e->datum.mus.y = (i32)capere_s(n, "y");
                e->datum.mus.botton = (mus_botton_t)capere_s(n,
                    "botton");
                e->datum.mus.modificantes = (i32)capere_s(n,
                    "modificantes");
            }
        }
        n = stml_frater_proximus(n);
    }
    redde index;
}
