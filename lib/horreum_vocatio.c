/* horreum_vocatio.c - stratum reactoris horrei (vide horreum_vocatio.h;
 * horreum-plan T5)
 *
 * Ansa horreum_exsequi (horreum_cliens.c) ut machina status: conatus =
 * signare + http_vocare; finis http -> legere -> (a) horologium semel
 * correctum: conatus novus statim; (b) 412 condicionale / (d)
 * NoSuchUpload Complete post iterationem: HEAD per vocationem FILIAM
 * (eadem machina, iterationes suas habet); (c) horreum_iterandum ->
 * timer, deinde conatus novus; ceterum finis. Observator (si quis) post
 * quemque finem http, ante lectionem; filia optiones (observatorem
 * quoque) hereditat.
 *
 * Unum ex his pendet dum vocatio vivit: http (conatus), filia (HEAD),
 * timer (mora aut finis dilatus). Abrumpere omnia tollit; finis
 * http/filiae intra abrumpere vocatus a vexillo 'abrumpens' tacetur. */
#include "latina.h"
#include "horreum_vocatio.h"
#include "sors.h"

#include <string.h>
#include <time.h>

structura HorreumVocatio {
                   Reactor* reactor;
                   Horreum* horreum;
               HorreumOpus  opus;
               HorreumOpus  inspectio;
       HorreumVocatioFinis  finis;
                    vacuum* datum;
    HorreumVocatioOptiones  optiones;
       HttpVocatioOptiones  http_optiones;
                       b32  http_data;
                   Piscina* piscina;
                      Sors  sors;
                       b32  correctum;
               HttpVocatio* http;
               HttpPetitio* petitio;        /* conatus currentis */
            HorreumVocatio* filia;
            ReactorTimerId  timer;
                       b32  timer_positum;
               MomentumSec  tempus;
              HorreumError  error_servatum;
    HorreumVocatioResultus  resultus;
                       b32  finita;
                       b32  abrumpens;
};

interior vacuum
_finire (
    HorreumVocatio* v)
{
    si (v->finita)
    {
        redde;
    }
    v->finita = VERUM;
    v->finis(v->datum, v, &v->resultus);
}

interior vacuum
_errorem_ponere (
       HorreumVocatio* v,
    HorreumErrorGenus  genus,
   constans character* nuntius)
{
    memset(&v->resultus.error, ZEPHYRUM, magnitudo(v->resultus.error));
    v->resultus.error.genus    = genus;
    v->resultus.error.nuntius  = chorda_ex_literis(nuntius, v->piscina);
}

/* finis dilatus (defectus intra horreum_vocare) */
interior vacuum
_finis_dilatus (
    vacuum* datum)
{
    HorreumVocatio* v = (HorreumVocatio*)datum;

    v->timer_positum = FALSUM;
    _finire(v);
}

interior vacuum
_http_finis (
         vacuum* datum,
    HttpVocatio* hv);

/* signat (tempus = nunc + deviatio) et mittit; FALSUM = error in
 * resultus */
interior b32
_conatum_incipere (
    HorreumVocatio* v)
{
    MomentumSec  nunc    = (MomentumSec)time(NIHIL);
    MomentumSec  tempus  = nunc + (MoraSec)v->resultus.deviatio_s;
    HttpPetitio* pet;

    pet = horreum_petitionem_signare(v->horreum, &v->opus, tempus,
        &v->resultus.error, v->piscina);
    si (!pet)
    {
        redde FALSUM;
    }
    v->tempus   = tempus;
    v->petitio  = pet;
    v->http     = http_vocare(v->reactor, pet, NIHIL, NIHIL,
        _http_finis,
        v, v->http_data ? &v->http_optiones : NIHIL, v->piscina);
    si (!v->http)
    {
        _errorem_ponere(v, HORREUM_MALUM,
            "horreum: http_vocare recusavit");
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_iterare (
    vacuum* datum)
{
    HorreumVocatio* v = (HorreumVocatio*)datum;

    v->timer_positum = FALSUM;
    si (!_conatum_incipere(v))
    {
        _finire(v);
    }
}

/* obiectum fert x-amz-meta-horreum-sha256 == sigillum nostrum? */
interior b32
_sigillum_nostrum (
    constans HorreumObjectum* ob,
                      chorda  sigillum)
{
    i32 i;

    per (i = ZEPHYRUM; i < ob->meta_numerus; i++)
    {
        si (   chorda_aequalis_literis(ob->meta[i].titulus,
                   HORREUM_META_SIGILLUM)
            && chorda_aequalis(ob->meta[i].valor, sigillum))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_inspectio_finis (
                             vacuum* datum,
                     HorreumVocatio* filia,
    constans HorreumVocatioResultus* r)
{
    HorreumVocatio* v = (HorreumVocatio*)datum;
               b32  nostrum;

    (vacuum)filia;
    v->filia = NIHIL;
    si (v->abrumpens)
    {
        redde;
    }
    v->resultus.deviatio_s = r->deviatio_s;
    si (v->opus.genus == HORREUM_OPUS_MP_PERFICERE)
    {
        nostrum = r->successus && chorda_aequalis(r->obiectum.etag,
            v->opus.perficere_etag_expectatum);
    }
    alioquin
    {
        nostrum = r->successus && _sigillum_nostrum(&r->obiectum,
            v->opus.corpus_sha256_hex);
    }
    si (nostrum)
    {
        memset(&v->resultus.error, ZEPHYRUM,
            magnitudo(v->resultus.error));
        v->resultus.obiectum              = r->obiectum;
        v->resultus.obiectum.iam_nostrum  = VERUM;
        v->resultus.successus             = VERUM;
        v->resultus.iam_nostrum           = VERUM;
    }
    alioquin
    {
        v->resultus.error = v->error_servatum;
    }
    _finire(v);
}

/* II.5: HEAD clavis per vocationem filiam; error servatur si non
 * nostrum */
interior vacuum
_inspicere (
    HorreumVocatio* v)
{
    HorreumVocatioOptiones o = v->optiones;
              HorreumError e;

    v->error_servatum = v->resultus.error;
    si (!horreum_opus_inspicere(v->horreum, v->opus.situla,
        v->opus.clavis,
            &v->inspectio, &e, v->piscina))
    {
        _finire(v);
        redde;
    }
    o.deviatio_s  = v->resultus.deviatio_s;
    o.semen       = (i64)sors_proximum(&v->sors) + I;
    o.http        = v->http_data ? &v->http_optiones : NIHIL;
    v->filia      = horreum_vocare(v->reactor, v->horreum,
        &v->inspectio,
        _inspectio_finis, v, &o, v->piscina);
    si (!v->filia)
    {
        _finire(v);
    }
}

/* conatus observatori (si quis) ante lectionem */
interior vacuum
_observare (
    constans HorreumVocatio* v,
               HttpResultus  res)
{
    HorreumObservatio ob;

    si (!v->optiones.observator)
    {
        redde;
    }
    ob.opus       = &v->opus;
    ob.petitio    = v->petitio;
    ob.responsum  = res.responsum;
    ob.error_transportus  = res.responsum
                            ? chorda_ex_buffer(NIHIL, ZEPHYRUM)
                            : res.error_descriptio;
    ob.conatus            = v->resultus.conatus;
    v->optiones.observator(v->optiones.observatoris_datum, &ob);
}

interior vacuum
_http_finis (
         vacuum* datum,
    HttpVocatio* hv)
{
    HorreumVocatio* v           = (HorreumVocatio*)datum;
      HttpResultus  res         = http_vocatio_resultus(hv);
      HorreumError* e           = &v->resultus.error;
            chorda  mora_servi  = chorda_ex_buffer(NIHIL, ZEPHYRUM);
               i32  mora;

    v->http = NIHIL;
    si (v->abrumpens)
    {
        redde;
    }
    v->resultus.conatus++;
    _observare(v, res);
    si (v->abrumpens)
    {
        /* observator abrupit: finis iam vocatus, nihil ultra
         * (recensio I5) */
        redde;
    }
    v->resultus.responsum = res.responsum;
    memset(&v->resultus.obiectum, ZEPHYRUM,
        magnitudo(v->resultus.obiectum));
    memset(e, ZEPHYRUM, magnitudo(*e));
    si (horreum_responsum_legere(&v->opus, res, v->tempus,
            &v->resultus.obiectum, e, v->piscina))
    {
        v->resultus.successus = VERUM;
        _finire(v);
        redde;
    }
    si (   e->genus      == HORREUM_TRANSITORIUM
        && e->deviatio_s != ZEPHYRUM
        && !v->correctum)
    {
        v->resultus.deviatio_s  += e->deviatio_s;
        v->correctum            = VERUM;
        si (!_conatum_incipere(v))
        {
            _finire(v);
        }
        redde;
    }
    si (   e->genus == HORREUM_CONDITIO && v->opus.condicionale
        && v->resultus.conatus > I)
    {
        _inspicere(v);
        redde;
    }
    si (   e->genus      == HORREUM_ABEST
        && v->opus.genus == HORREUM_OPUS_MP_PERFICERE
        && v->resultus.conatus > I
        && v->opus.perficere_etag_expectatum.mensura > ZEPHYRUM)
    {
        _inspicere(v);
        redde;
    }
    si (res.responsum)
    {
        mora_servi = http_responsum_caput(res.responsum, "Retry-After");
    }
    si (!horreum_iterandum(&v->opus, e, mora_servi, v->resultus.conatus,
            v->optiones.conatus_maximi, v->optiones.mora_basis_ms,
            sors_proximum(&v->sors), &mora))
    {
        _finire(v);
        redde;
    }
    v->timer = reactor_timer_adicere(v->reactor, mora, FALSUM, _iterare,
        v);
    si (v->timer == REACTOR_TIMER_ERROR)
    {
        _finire(v);
        redde;
    }
    v->timer_positum = VERUM;
}

HorreumVocatio*
horreum_vocare (
                            Reactor* reactor,
                            Horreum* h,
               constans HorreumOpus* opus,
                HorreumVocatioFinis  finis,
                             vacuum* datum,
    constans HorreumVocatioOptiones* optiones,
                            Piscina* piscina)
{
    HorreumVocatio* v;

    si (!reactor || !opus || !finis || !piscina)
    {
        redde NIHIL;
    }
    v = (HorreumVocatio*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(HorreumVocatio));
    memset(v, ZEPHYRUM, magnitudo(*v));
    v->reactor  = reactor;
    v->horreum  = h;
    v->opus     = *opus;
    v->finis    = finis;
    v->datum    = datum;
    v->piscina  = piscina;
    si (optiones)
    {
        v->optiones = *optiones;
    }
    si (v->optiones.http)
    {
        v->http_optiones  = *v->optiones.http;
        v->http_data      = VERUM;
    }
    v->optiones.http = NIHIL;
    si (v->optiones.conatus_maximi == ZEPHYRUM)
    {
        v->optiones.conatus_maximi = III;
    }
    si (v->optiones.mora_basis_ms == ZEPHYRUM)
    {
        v->optiones.mora_basis_ms = CC;
    }
    sors_seminare(&v->sors, v->optiones.semen != ZEPHYRUM
        ? v->optiones.semen : (i64)time(NIHIL), ZEPHYRUM);
    v->resultus.deviatio_s = v->optiones.deviatio_s;
    si (!_conatum_incipere(v))
    {
        /* finis numquam intra horreum_vocare: pulsu proximo */
        v->timer = reactor_timer_adicere(reactor, ZEPHYRUM, FALSUM,
            _finis_dilatus, v);
        si (v->timer == REACTOR_TIMER_ERROR)
        {
            redde NIHIL;
        }
        v->timer_positum = VERUM;
    }
    redde v;
}

vacuum
horreum_vocationem_abrumpere (
    HorreumVocatio* v)
{
       HttpVocatio* hv;
    HorreumVocatio* filia;

    si (!v || v->finita || v->abrumpens)
    {
        redde;
    }
    v->abrumpens = VERUM;
    si (v->timer_positum)
    {
        (vacuum)reactor_timer_cancellare(v->reactor, v->timer);
        v->timer_positum = FALSUM;
    }
    hv = v->http;
    si (hv)
    {
        v->http = NIHIL;
        http_vocationem_abrumpere(hv);
    }
    filia = v->filia;
    si (filia)
    {
        v->filia = NIHIL;
        horreum_vocationem_abrumpere(filia);
    }
    _errorem_ponere(v, HORREUM_MALUM, "horreum: vocatio abrupta");
    v->resultus.successus  = FALSUM;
    v->resultus.abrupta    = VERUM;
    _finire(v);
}

b32
horreum_vocatio_finita (
    constans HorreumVocatio* v)
{
    redde v ? v->finita : VERUM;
}

i32
horreum_vocatio_conatus (
    constans HorreumVocatio* v)
{
    redde v ? v->resultus.conatus : ZEPHYRUM;
}
