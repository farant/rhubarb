/* cantor.c - Cantor (vide include/cantor.h)
 *
 * QUADRUM PULSUS in transitu currente: F(p) = basis_quadrum + Q(p) -
 * Q(basis_pulsus), Q = cantus_quadrum_pulsus (mappa carminis). Iteratio
 * basis ad F(finis) promovet et basis_pulsus ad initium regionis
 * reponit - quadra transitus secundi ex eadem mappa, sine aberratione.
 * 'iussus' = pulsus usque ad quem notae iussae sunt (exclusivus).
 */
#include "cantor.h"
#include "cantus.h"
#include <string.h>

/* prioritas vocum musicae: supra cantilenas */
#define CANTOR_PRIORITAS II

nomen structura {
    MixturaAnsa ansa;
            s64 initium;   /* quadrum */
            s64 finis;
            b32 activa;
} CantorVox;

structura Cantor {
              Concentus* concentus;
        CantorResolutor  resolutor;
        constans Carmen* carmen;
                    b32  sonat;          /* notae adhuc iubendae */
                    b32  iterare;
                    s64  regio_initium;
                    s64  regio_finis;    /* 0 = carmen totum */
                    s64  basis_quadrum;
                    s64  basis_pulsus;
                    s64  prior_quadrum;  /* transitus prior (lusor) */
                    s64  prior_pulsus;
                    b32  prior;
                    s64  iussus;
              CantorVox  voces[CANTOR_VOCES];
};

constans TibiaParametra*
cantor_tibia_ordinaria (
     vacuum* ctx,
     chorda  instrumentum)
{
    character titulus[LXIV];

    (vacuum)ctx;
    si (   instrumentum.mensura <= ZEPHYRUM
        || instrumentum.mensura >= (i32)magnitudo(titulus))
    {
        redde NIHIL;
    }
    memcpy(titulus, instrumentum.datum, (size_t)instrumentum.mensura);
    titulus[instrumentum.mensura] = '\0';
    redde tibia_praeparatum(titulus);
}

constans SonitusParametra*
cantor_sonitus_ordinarius (
     vacuum* ctx,
     chorda  praeparatum)
{
    (vacuum)ctx;
    redde cantus_praeparatum_sonitus(praeparatum, NIHIL);
}

Cantor*
cantor_creare (
                     Piscina* piscina,
                   Concentus* concentus,
    constans CantorResolutor* resolutor)
{
    Cantor* c;

    si (concentus == NIHIL)
    {
        redde NIHIL;
    }
    c = (Cantor*)piscina_allocare(piscina, magnitudo(Cantor));
    si (c == NIHIL)
    {
        redde NIHIL;
    }
    memset(c, ZEPHYRUM, magnitudo(Cantor));
    c->concentus = concentus;
    si (resolutor != NIHIL)
    {
        c->resolutor = *resolutor;
    }
    si (c->resolutor.tibia == NIHIL)
    {
        c->resolutor.tibia = cantor_tibia_ordinaria;
    }
    si (c->resolutor.sonitus == NIHIL)
    {
        c->resolutor.sonitus = cantor_sonitus_ordinarius;
    }
    redde c;
}


/* ==================================================
 * Tempus
 * ================================================== */

interior s64
_q (
    constans Cantor* c,
                s64  pulsus)
{
    redde cantus_quadrum_pulsus(c->carmen->tempora,
        c->carmen->numerus_temporum, pulsus);
}

/* quadrum absolutum pulsus in transitu currente */
interior s64
_quadrum (
    constans Cantor* c,
                s64  pulsus)
{
    redde c->basis_quadrum + _q(c, pulsus) - _q(c, c->basis_pulsus);
}

interior s64
_finis (
    constans Cantor* c)
{
    si (c->regio_finis > ZEPHYRUM)
    {
        redde c->regio_finis;
    }
    redde c->carmen->pulsus;
}


/* ==================================================
 * Voces
 * ================================================== */

interior CantorVox*
_vox_libera (
    Cantor* c)
{
    i32 i;

    per (i = ZEPHYRUM; i < CANTOR_VOCES; i++)
    {
        si (!c->voces[i].activa)
        {
            redde &c->voces[i];
        }
    }
    redde NIHIL;
}

/* voces omnes tacent: sonans evanescit ab nunc, nondum incepta ad
 * initium suum sine exemplo */
interior vacuum
_voces_tacere (
    Cantor* c)
{
    MixturaIussum i;
              s64 nunc = concentus_momentum(c->concentus);
              i32 k;

    per (k = ZEPHYRUM; k < CANTOR_VOCES; k++)
    {
        CantorVox* v = &c->voces[k];

        si (!v->activa)
        {
            perge;
        }
        v->activa = FALSUM;
        si (!concentus_vox_vivit(c->concentus, v->ansa))
        {
            perge;
        }
        si (v->initium >= nunc)
        {
            mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SISTERE,
                v->initium);
            i.clivus = ZEPHYRUM;
        }
        alioquin
        {
            mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SISTERE, nunc);
            i.clivus = CANTOR_EVANESCENTIA;
        }
        i.ansa = v->ansa;
        (vacuum)concentus_iubere(c->concentus, &i);
    }
}

/* nota una (pulsus absolutus p) iubetur; sine voce aut sede omissa */
interior vacuum
_notam_iubere (
                     Cantor* c,
        constans CarmenPars* pars,
    constans TibiaParametra* tibia,
        constans CarmenNota* n,
                        s64  p)
{
                    CantorVox* v = _vox_libera(c);
                MixturaIussum  i;
                  MixturaAnsa  a;
                          s64  initium  = _quadrum(c, p);
                          s64  finis    = _quadrum(c, p + n->longitudo);
    constans SonitusParametra* sonitus  = NIHIL;

    si (n->praeparatum.mensura > ZEPHYRUM)
    {
        sonitus = c->resolutor.sonitus(c->resolutor.ctx,
            n->praeparatum);
        si (sonitus == NIHIL)
        {
            redde;
        }
    }
    alioquin si (tibia == NIHIL)
    {
        redde;
    }
    si (v == NIHIL)
    {
        redde;
    }
    a = mixtura_rector_capere(concentus_rector(c->concentus),
        CANTOR_PRIORITAS);
    si (mixtura_ansa_nulla(a))
    {
        redde;
    }
    si (sonitus != NIHIL)
    {
        mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SONITUM, initium);
        i.parametra = *sonitus;
        i.parametra.frequentia = sonus_notae_frequentia((f32)n->nota);
        i.semen = (i64)p;
        finis = initium + (s64)sonitus_longitudo(&i.parametra, i.semen);
    }
    alioquin
    {
        mixtura_iussum_parare(&i, MIXTURA_IUSSUM_TIBIAM, initium);
        i.tibia  = *tibia;
        i.nota   = (f32)n->nota;
    }
    i.ansa    = a;
    i.grex    = MIXTURA_GREX_MUSICA;
    i.lucrum  = n->lucrum * pars->lucrum;
    si (!concentus_iubere(c->concentus, &i))
    {
        redde;
    }
    si (sonitus == NIHIL)
    {
        mixtura_iussum_parare(&i, MIXTURA_IUSSUM_PORTAM_CLAUDERE,
            finis);
        i.ansa = a;
        (vacuum)concentus_iubere(c->concentus, &i);
    }
    v->ansa     = a;
    v->initium  = initium;
    v->finis    = finis;
    v->activa   = VERUM;
}

/* notae quarum pulsus in [infimus, summus) iubentur */
interior vacuum
_notas_iubere (
    Cantor* c,
       s64  infimus,
       s64  summus)
{
     constans Carmen* k      = c->carmen;
                 b32  solus  = FALSUM;
                 i32  i;
                 i32  j;
                 i32  m;

    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        solus = solus || k->partes[i].solus;
    }
    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
            constans CarmenPars* pars = &k->partes[i];
        constans TibiaParametra* tibia;

        si (pars->mutus || (solus && !pars->solus))
        {
            perge;
        }
        tibia = c->resolutor.tibia(c->resolutor.ctx,
            pars->instrumentum);
        per (j = ZEPHYRUM; j < pars->numerus_frustorum; j++)
        {
            constans CarmenFrustum* f = &pars->frusta[j];

            si (   f->pulsus                >= summus
                || f->pulsus + f->longitudo <= infimus)
            {
                perge;
            }
            per (m = ZEPHYRUM; m < f->numerus; m++)
            {
                s64 p = f->pulsus + f->notae[m].pulsus;

                si (p >= infimus && p < summus)
                {
                    _notam_iubere(c, pars, tibia, &f->notae[m], p);
                }
            }
        }
    }
}


/* ==================================================
 * API
 * ================================================== */

interior vacuum
_ponere (
    Cantor* c,
       s64  pulsus)
{
    c->basis_pulsus   = pulsus;
    c->iussus         = pulsus;
    c->basis_quadrum  = concentus_initium(c->concentus);
    c->prior          = FALSUM;
    c->sonat          = VERUM;
}

b32
cantor_incipere (
             Cantor* c,
    constans Carmen* carmen,
                s64  pulsus)
{
    si (carmen == NIHIL || !concentus_aperire(c->concentus))
    {
        redde FALSUM;
    }
    _voces_tacere(c);
    c->carmen = carmen;
    _ponere(c, pulsus);
    redde VERUM;
}

vacuum
cantor_regio (
    Cantor* c,
       b32  iterare,
       s64  initium,
       s64  finis)
{
    c->iterare        = iterare;
    c->regio_initium  = initium < ZEPHYRUM ? ZEPHYRUM : initium;
    c->regio_finis    = finis < ZEPHYRUM ? ZEPHYRUM : finis;
}

b32
cantor_agere (
    Cantor* c)
{
    s64 scopus;
    s64 summus;
    s64 finis;
    i32 k;
    i32 iterationes  = ZEPHYRUM;
    b32 vivae        = FALSUM;

    per (k = ZEPHYRUM; k < CANTOR_VOCES; k++)
    {
        CantorVox* v = &c->voces[k];

        si (v->activa && !concentus_vox_vivit(c->concentus, v->ansa))
        {
            v->activa = FALSUM;
        }
        vivae = vivae || v->activa;
    }
    si (!c->sonat || c->carmen == NIHIL)
    {
        redde vivae;
    }
    dum (c->sonat)
    {
        finis = _finis(c);
        /* notae quarum quadrum < horizon: Q(p) < scopus */
        scopus = concentus_initium(c->concentus)
               + (s64)CANTOR_PROSPECTUS * (s64)SONUS_FREQUENTIA / (s64)M
               - c->basis_quadrum + _q(c, c->basis_pulsus);
        summus = scopus <= ZEPHYRUM ? ZEPHYRUM
               : cantus_pulsus_quadri(c->carmen->tempora,
                     c->carmen->numerus_temporum, scopus - I) + I;
        si (summus > finis)
        {
            summus = finis;
        }
        si (summus > c->iussus)
        {
            _notas_iubere(c, c->iussus, summus);
            c->iussus = summus;
        }
        si (c->iussus < finis)
        {
            frange;
        }
        /* finis regionis iussus */
        si (!c->iterare || finis <= c->regio_initium)
        {
            c->sonat = FALSUM;
            frange;
        }
        si (++iterationes > CANTOR_ITERATIONES)
        {
            frange;   /* basis manet ad finem: pulsus proximus pergit */
        }
        c->prior_quadrum  = c->basis_quadrum;
        c->prior_pulsus   = c->basis_pulsus;
        c->prior          = VERUM;
        c->basis_quadrum  = _quadrum(c, finis);
        c->basis_pulsus   = c->regio_initium;
        c->iussus         = c->regio_initium;
    }
    redde VERUM;
}

vacuum
cantor_sistere (
    Cantor* c)
{
    _voces_tacere(c);
    c->sonat = FALSUM;
}

vacuum
cantor_quaerere (
    Cantor* c,
       s64  pulsus)
{
    si (c->carmen == NIHIL)
    {
        redde;
    }
    _voces_tacere(c);
    _ponere(c, pulsus < ZEPHYRUM ? ZEPHYRUM : pulsus);
}

s64
cantor_pulsus (
    Cantor* c)
{
    s64 nunc;
    s64 basis_quadrum;
    s64 basis_pulsus;
    s64 scopus;

    si (c->carmen == NIHIL)
    {
        redde ZEPHYRUM;
    }
    nunc           = concentus_momentum(c->concentus);
    basis_quadrum  = c->basis_quadrum;
    basis_pulsus   = c->basis_pulsus;
    si (nunc < basis_quadrum && c->prior)
    {
        basis_quadrum  = c->prior_quadrum;
        basis_pulsus   = c->prior_pulsus;
    }
    si (nunc < basis_quadrum)
    {
        redde basis_pulsus;
    }
    scopus = nunc - basis_quadrum + _q(c, basis_pulsus);
    redde cantus_pulsus_quadri(c->carmen->tempora,
        c->carmen->numerus_temporum, scopus);
}

b32
cantor_sonat (
    constans Cantor* c)
{
    i32 k;

    si (c->sonat)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < CANTOR_VOCES; k++)
    {
        si (   c->voces[k].activa
            && concentus_vox_vivit(c->concentus, c->voces[k].ansa))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}
