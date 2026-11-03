/* veredarius_domesticus.c - veredarius super http_vocatio
 * (communio-plan-5 T1). Locus per petitionem; abrumpere locum prius
 * notat, ergo finis quem http_vocationem_abrumpere synchronice vocat
 * tacetur (contractus veredarii: post abrumpere numquam). */

#include "veredarius_domesticus.h"
#include "http.h"
#include "http_vocatio.h"

#include <stdio.h>
#include <string.h>

#define LOCI          LXIV
#define TEMPUS_ORDO   ((i32)XXX * (i32)M)
#define CAUSA_MAXIMA  XCVI

structura VeredariusDomesticus;

nomen structura {
                       b32  in_usu;
                       b32  abrupta;
                       i32  generatio;
               HttpVocatio* vocatio;
           VeredariusFinis  finis;
                    vacuum* ctx;
                   Piscina* piscina;
    structura VeredariusDomesticus* dominus;
} LocusVeredarii;

nomen structura VeredariusDomesticus {
        Veredarius  v;
           Reactor* reactor;
    LocusVeredarii  loci[LOCI];
} VeredariusDomesticus;

interior HttpMethodus
_methodus (
    VeredariusMethodus m)
{
    commutatio (m)
    {
        casus VEREDARIUS_POST:
            redde HTTP_POST;
        casus VEREDARIUS_PUT:
            redde HTTP_PUT;
        ordinarius:
            redde HTTP_GET;
    }
}

interior vacuum
_finis_http (
         vacuum* datum,
    HttpVocatio* vocatio)
{
         LocusVeredarii* l;
           HttpResultus  res;
    VeredariusResponsum  r;
        VeredariusFinis  finis;
                 vacuum* ctx;
              character  causa[CAUSA_MAXIMA];

    l = (LocusVeredarii*)datum;
    si (!l->in_usu || l->abrupta || l->vocatio != vocatio)
    {
        redde;
    }
    res = http_vocatio_resultus(vocatio);
    memset(&r, ZEPHYRUM, magnitudo(r));
    si (res.successus && res.responsum)
    {
        r.successus  = VERUM;
        r.status     = res.responsum->status;
        r.corpus     = res.responsum->corpus;
    }
    alioquin
    {
        /* descriptio erroris viam non nominat; ad CAUSA_MAXIMA */
        sprintf(causa, "rete: %.80s", res.error_descriptio.mensura
            > ZEPHYRUM ? chorda_ut_cstr(res.error_descriptio,
            l->piscina)
            : "defectus");
        r.causa = chorda_ex_literis(causa, l->piscina);
    }
    finis       = l->finis;
    ctx         = l->ctx;
    l->in_usu   = FALSUM;
    l->vocatio  = NIHIL;
    finis(ctx, &r);
}

interior b32
_petere (
                        vacuum* sui,
    constans VeredariusPetitio* p,
                       Piscina* piscina,
               VeredariusFinis  finis,
                        vacuum* ctx,
                           i32* signum)
{
    VeredariusDomesticus* d;
          LocusVeredarii* l;
             HttpPetitio* pet;
     HttpVocatioOptiones  optiones;
                     i32  tempus;
                     i32  i;

    d = (VeredariusDomesticus*)sui;
    si (!d || !p || !p->url || !piscina || !finis)
    {
        redde FALSUM;
    }
    l = NIHIL;
    per (i = ZEPHYRUM; i < LOCI; i++)
    {
        si (!d->loci[i].in_usu)
        {
            l = &d->loci[i];
            frange;
        }
    }
    si (!l)
    {
        redde FALSUM;
    }
    pet = http_petitio_creare(piscina, _methodus(p->methodus), p->url);
    si (!pet)
    {
        redde FALSUM;
    }
    si (p->auctoritas)
    {
        http_petitio_caput_addere(pet, "Authorization", p->auctoritas);
    }
    si (p->corpus.mensura > ZEPHYRUM)
    {
        http_petitio_caput_addere(pet, "Content-Type", p->mimen
            ? p->mimen : "application/octet-stream");
        http_petitio_corpus_ponere_chorda(pet, p->corpus);
    }
    /* tempus et connexionis et otii (servus tacens): http_vocatio
     * aliter LX s otii exspectat */
    tempus = p->tempus_ms > ZEPHYRUM ? p->tempus_ms : TEMPUS_ORDO;
    http_petitio_tempus_ponere(pet, (MoraAngusta)tempus);
    memset(&optiones, ZEPHYRUM, magnitudo(optiones));
    optiones.mora_otii_ms  = tempus;
    l->in_usu              = VERUM;
    l->abrupta             = FALSUM;
    l->generatio++;
    l->finis    = finis;
    l->ctx      = ctx;
    l->piscina  = piscina;
    l->dominus  = d;
    l->vocatio   = http_vocare(d->reactor, pet, NIHIL, NIHIL,
        _finis_http,
        l, &optiones, piscina);
    si (!l->vocatio)
    {
        l->in_usu = FALSUM;
        redde FALSUM;
    }
    si (signum)
    {
        *signum = (i32)(l - d->loci) + l->generatio * (i32)LOCI;
    }
    redde VERUM;
}

interior vacuum
_abrumpere (
    vacuum* sui,
       i32  signum)
{
    VeredariusDomesticus* d;
          LocusVeredarii* l;
             HttpVocatio* v;

    d = (VeredariusDomesticus*)sui;
    si (!d)
    {
        redde;
    }
    l = &d->loci[signum % (i32)LOCI];
    si (!l->in_usu || l->generatio != signum / (i32)LOCI)
    {
        redde;
    }
    l->abrupta  = VERUM;
    v           = l->vocatio;
    si (v)
    {
        http_vocationem_abrumpere(v);
    }
    l->vocatio  = NIHIL;
    l->in_usu   = FALSUM;
}

Veredarius*
veredarium_domesticum_creare (
    Piscina* piscina,
    Reactor* reactor)
{
    VeredariusDomesticus* d;

    si (!piscina || !reactor)
    {
        redde NIHIL;
    }
    d = (VeredariusDomesticus*)piscina_allocare(piscina,
        magnitudo(VeredariusDomesticus));
    si (!d)
    {
        redde NIHIL;
    }
    memset(d, ZEPHYRUM, magnitudo(VeredariusDomesticus));
    d->reactor      = reactor;
    d->v.petere     = _petere;
    d->v.abrumpere  = _abrumpere;
    d->v.sui        = d;
    redde &d->v;
}
