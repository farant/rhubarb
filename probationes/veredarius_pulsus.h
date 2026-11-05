/* veredarius_pulsus.h - probationes pulsus in pictore (pulsus-plan T9):
 * veredarius qui petitiones /communio/pulsus/ notat (methodus, via,
 * corpus) et ipse respondet - PUT 204, POST 202, post I ms per
 * reactorem - cetera ad fundum (veredarius domesticus, servus verus).
 * Servus probationis pulsum non montat; sic eventus et inscriptiones
 * legi possunt. Usus: probatio_pictor_communio, probatio_pictor_
 * inscriptio. */
#ifndef VEREDARIUS_PULSUS_H
#define VEREDARIUS_PULSUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"
#include "veredarius.h"
#include <string.h>

#define VP_NOTAE    XVI
#define VP_PENDENS  VIII
#define VP_SIGNUM   0x7ffe0000u

nomen structura {
    VeredariusFinis  finis;
             vacuum* ctx;
                i32  status;
     ReactorTimerId  hora;
                b32  vivus;
} VeredariusPulsusMora;

nomen structura {
              Veredarius  v;
              Veredarius* fundus;
                 Reactor* reactor;
                     i32  numerus;
               character  methodi[VP_NOTAE];
               character  viae[VP_NOTAE][CXXVIII];
               character  corpora[VP_NOTAE][MXXIV];
    VeredariusPulsusMora  morae[VP_PENDENS];
} VeredariusPulsus;

interior vacuum
_vp_hora (
    vacuum* datum)
{
    VeredariusPulsusMora* m = (VeredariusPulsusMora*)datum;
     VeredariusResponsum  r;

    m->vivus = FALSUM;
    memset(&r, ZEPHYRUM, magnitudo(r));
    r.successus  = VERUM;
    r.status     = m->status;
    m->finis(m->ctx, &r);
}

interior b32
_vp_petere (
                        vacuum* sui,
    constans VeredariusPetitio* p,
                       Piscina* pi,
               VeredariusFinis  finis,
                        vacuum* ctx,
                           i32* signum)
{
    VeredariusPulsus* vp = (VeredariusPulsus*)sui;
                 i32  i;
                 i32  n;
                 i32  k;

    si (!strstr(p->url, "/communio/pulsus/"))
    {
        redde vp->fundus->petere(vp->fundus->sui, p, pi, finis, ctx,
            signum);
    }
    per (k = ZEPHYRUM; k < VP_PENDENS && vp->morae[k].vivus; k++)
    {
    }
    si (k == VP_PENDENS)
    {
        redde FALSUM;
    }
    i = vp->numerus;
    si (i < VP_NOTAE)
    {
        vp->methodi[i] = p->methodus == VEREDARIUS_PUT ? 'U' : 'P';
        strncpy(vp->viae[i], strstr(p->url, "/communio/"), CXXVII);
        vp->viae[i][CXXVII] = '\0';
        n = p->corpus.mensura < MXXIII ? (i32)p->corpus.mensura
            : MXXIII;
        memcpy(vp->corpora[i], p->corpus.datum, (size_t)n);
        vp->corpora[i][n] = '\0';
    }
    vp->numerus++;
    vp->morae[k].finis   = finis;
    vp->morae[k].ctx     = ctx;
    vp->morae[k].status  = p->methodus == VEREDARIUS_PUT ? CCIV : CCII;
    vp->morae[k].hora    = reactor_timer_adicere(vp->reactor, I, FALSUM,
        _vp_hora, &vp->morae[k]);
    vp->morae[k].vivus  = VERUM;
    *signum             = (i32)(VP_SIGNUM + (insignatus integer)k);
    redde VERUM;
}

interior vacuum
_vp_abrumpere (
    vacuum* sui,
       i32  signum)
{
    VeredariusPulsus* vp = (VeredariusPulsus*)sui;
                 i32  k;

    si (   signum >= (i32)VP_SIGNUM
        && signum < (i32)VP_SIGNUM + VP_PENDENS)
    {
        k = signum - (i32)VP_SIGNUM;
        si (vp->morae[k].vivus)
        {
            (vacuum)reactor_timer_cancellare(vp->reactor,
                vp->morae[k].hora);
            vp->morae[k].vivus = FALSUM;
        }
        redde;
    }
    vp->fundus->abrumpere(vp->fundus->sui, signum);
}

interior vacuum
veredarium_pulsus_parare (
    VeredariusPulsus* vp,
          Veredarius* fundus,
             Reactor* reactor)
{
    memset(vp, ZEPHYRUM, magnitudo(*vp));
    vp->v.petere     = _vp_petere;
    vp->v.abrumpere  = _vp_abrumpere;
    vp->v.sui        = vp;
    vp->fundus       = fundus;
    vp->reactor      = reactor;
}

/* corpus notae k-tae cuius methodus 'methodus' ('U' PUT, 'P' POST) et
 * via in 'finis' desinit; NIHIL si non tot */
interior constans character*
veredarius_pulsus_corpus (
    constans VeredariusPulsus* vp,
                    character  methodus,
           constans character* finis,
                          i32  k)
{
    i32 i;
    i32 n;

    n = vp->numerus < VP_NOTAE ? vp->numerus : VP_NOTAE;
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (   vp->methodi[i]      == methodus
            && strlen(vp->viae[i]) >= strlen(finis)
            && strcmp(vp->viae[i] + strlen(vp->viae[i]) - strlen(finis),
                   finis) == ZEPHYRUM
            && k--                 == ZEPHYRUM)
        {
            redde vp->corpora[i];
        }
    }
    redde NIHIL;
}

#endif /* VEREDARIUS_PULSUS_H */
