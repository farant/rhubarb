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
#include <math.h>
#include <string.h>

/* prioritas vocum musicae: supra cantilenas */
#define CANTOR_PRIORITAS II

nomen structura {
    MixturaAnsa ansa;
            s64 initium;   /* quadrum */
            s64 finis;
            b32 activa;
            /* dynamica sequenda (tibia sola): pars, lucrum sine
             * dynamica, spatium notae in pulsibus, transitus (basis
             * quadrum quo iussa est - iteratio aliam habet) */
            b32 sequitur;
            s32 pars;
            f32 basis;
            s64 pulsus_initium;
            s64 pulsus_finis;
            s64 transitus;
} CantorVox;

structura Cantor {
              Concentus* concentus;
        CantorResolutor  resolutor;
        constans Carmen* carmen;
                    b32  sonat;          /* notae adhuc iubendae */
                    b32  tenet;          /* concentus_tenere datum */
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

/* punctum dynamicae proximum post pulsum x pro parte: octava
 * proxima (CANTOR_GRADUS_INTENSIONIS) aut initium / finis clivi
 * partis aut carminis */
interior s64
_proximum_intensionis (
    constans Carmen* k,
                s32  pars,
                s64  x)
{
    s64 p = (x / CANTOR_GRADUS_INTENSIONIS + I)
          * CANTOR_GRADUS_INTENSIONIS;
    i32 i;

    per (i = ZEPHYRUM; i < k->numerus_intensionum; i++)
    {
        constans CarmenIntensio* e = &k->intensiones[i];

        si (e->pars != pars && e->pars != -I)
        {
            perge;
        }
        si (e->pulsus > x && e->pulsus < p)
        {
            p = e->pulsus;
        }
        si (e->usque > x && e->usque < p)
        {
            p = e->usque;
        }
    }
    redde p;
}

/* gradus dynamicae (sine 'usque') partis aut carminis incipit ad s:
 * saltus ibi (clivus continuus est, initium eius quoque) */
interior b32
_gradus_ad (
    constans Carmen* k,
                s32  pars,
                s64  s)
{
    i32 i;

    per (i = ZEPHYRUM; i < k->numerus_intensionum; i++)
    {
        constans CarmenIntensio* e = &k->intensiones[i];

        si (   (e->pars == pars || e->pars == -I) && e->pulsus == s
            && e->usque == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* lucrum vocis ad 'lucrum' cum clivo (positio et missio partis
 * manent) */
interior vacuum
_vocem_ponere (
                Cantor* c,
    constans CantorVox* v,
                   s64  momentum,
                   s64  clivus,
                   f32  lucrum)
{
    constans CarmenPars* pars = &c->carmen->partes[v->pars];
          MixturaIussum  i;

    mixtura_iussum_parare(&i, MIXTURA_IUSSUM_VOCEM_PONERE, momentum);
    i.ansa     = v->ansa;
    i.lucrum   = lucrum;
    i.positio  = pars->positio;
    i.missio   = pars->reverberatio;
    i.clivus   = (i32)clivus;
    (vacuum)concentus_iubere(c->concentus, &i);
}

/* segmentum dynamicae vocis a pulsu s ad punctum proximum: gradus ad s
 * (evanescentia brevis) si <dynamica> sine clivo ibi incipit, deinde
 * clivus ad valorem ante punctum proximum si differt */
interior vacuum
_segmentum_iubere (
                Cantor* c,
    constans CantorVox* v,
                   s64  s)
{
    constans Carmen* k = c->carmen;
                s64  e = _proximum_intensionis(k, v->pars, s);
                f32  g = carmen_intensio(k, v->pars, s);
                f32  h;

    si (e > v->pulsus_finis)
    {
        e = v->pulsus_finis;
    }
    h = carmen_intensio(k, v->pars, e - I);
    si (s > v->pulsus_initium && _gradus_ad(k, v->pars, s))
    {
        _vocem_ponere(c, v, _quadrum(c, s), CANTOR_EVANESCENTIA,
                      v->basis * g);
    }
    si (fabs((f64)(h - g)) > 1e-6)
    {
        _vocem_ponere(c, v, _quadrum(c, s),
                      _quadrum(c, e) - _quadrum(c, s), v->basis * h);
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
    i.ansa = a;
    i.grex = MIXTURA_GREX_MUSICA;
    i.lucrum   = n->lucrum * pars->lucrum
               * carmen_intensio(c->carmen,
                     (s32)(pars - c->carmen->partes), p);
    i.positio  = pars->positio;
    i.missio   = pars->reverberatio;
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
    v->ansa            = a;
    v->initium         = initium;
    v->finis           = finis;
    v->activa          = VERUM;
    v->sequitur        = sonitus == NIHIL;
    v->pars            = (s32)(pars - c->carmen->partes);
    v->basis           = n->lucrum * pars->lucrum;
    v->pulsus_initium  = p;
    v->pulsus_finis    = p + n->longitudo;
    v->transitus       = c->basis_quadrum;
    si (v->sequitur)
    {
        _segmentum_iubere(c, v, p);
    }
}

/* voces sonantes huius transitus: puncta dynamicae in [infimus,
 * summus) intra notam suam - iussa per prospectum, non praeiussa */
interior vacuum
_intensiones_iubere (
    Cantor* c,
       s64  infimus,
       s64  summus)
{
    i32 i;

    si (c->carmen->numerus_intensionum == ZEPHYRUM)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < CANTOR_VOCES; i++)
    {
        constans CantorVox* v = &c->voces[i];
                       s64  s;

        si (   !v->activa || !v->sequitur
            || v->transitus != c->basis_quadrum)
        {
            perge;
        }
        s = infimus > v->pulsus_initium ? infimus
            - I : v->pulsus_initium;
        s = _proximum_intensionis(c->carmen, v->pars, s);
        dum (s < summus && s < v->pulsus_finis)
        {
            _segmentum_iubere(c, v, s);
            s = _proximum_intensionis(c->carmen, v->pars, s);
        }
    }
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
        /* instrumentum carminis (libri) primum, deinde resolutor */
        tibia = carmen_instrumentum(k, pars->instrumentum) != NIHIL
              ? &carmen_instrumentum(k, pars->instrumentum)->parametra
              : c->resolutor.tibia(c->resolutor.ctx,
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

/* exitum tenet dum notae iubendae; semel (non cumulatur) */
interior vacuum
_tenere (
    Cantor* c,
       b32  tenere)
{
    si (c->tenet != tenere)
    {
        concentus_tenere(c->concentus, tenere);
        c->tenet = tenere;
    }
}

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
    _tenere(c, VERUM);
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
    /* aula carminis (sine aula reditus 0: prior tacet) */
    {
        MixturaIussum i;

        mixtura_iussum_parare(&i, MIXTURA_IUSSUM_REVERBERATIONEM_PONERE,
                              concentus_initium(c->concentus));
        i.reverberatio  = carmen->reverberatio;
        i.lucrum        = carmen->lucrum_reverberationis;
        (vacuum)concentus_iubere(c->concentus, &i);
    }
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
            /* notae primum: voces novae puncta sua in hoc prospectu
             * accipiunt (initium ipsum _notam_iubere tractat) */
            _notas_iubere(c, c->iussus, summus);
            _intensiones_iubere(c, c->iussus, summus);
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
            _tenere(c, FALSUM);
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
    _tenere(c, FALSUM);
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
