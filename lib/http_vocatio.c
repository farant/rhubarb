/* http_vocatio.c - vocatio HTTP(S) non-obstruens super reactorem
 * (http-vocatio-spec II.4; vide caput) */
#include "http_vocatio.h"
#include "nomina.h"
#include "tcp.h"
#include "tls.h"
#include <string.h>

#define HTTP_VOCATIO_CAPSA             (XVI * MXXIV)
#define HTTP_VOCATIO_MORA_CONNEXIONIS  (XXX * M)
#define HTTP_VOCATIO_MORA_OTII         (LX * M)

structura HttpVocatio {
              Reactor* reactor;
              Piscina* piscina;
          HttpPetitio* petitio;
            HttpRivus  rivus;
               vacuum* rivi_datum;
     HttpVocatioFinis  finis;
               vacuum* finis_datum;
    HttpVocatioStatus  status;
       NominaQuaestio* quaestio;
      NominaAddressus  addressus[NOMINA_ADDRESSUS_MAXIMI];
                  i32  addressus_numerus;
                  i32  addressus_index;
               chorda  error_ultimus;     /* connexionis prioris */
          TcpConnexio* tcp;
          TlsConnexio* tls;               /* tcp possidet */
              integer  fossa;             /* in reactore; -1 nulla */
                  b32  est_https;
               chorda  octeti;            /* clavis intus (R2) */
                  i32  missa;
           HttpLector* lector;
                   i8* capsa;
       ReactorTimerId  mora;
                  b32  mora_posita;
                  i32  mora_connexionis_ms;
                  i32  mora_otii_ms;
            HttpError  error_pendens;     /* != HTTP_OK: per timer 0 */
               chorda  descriptio_pendens;
         HttpResultus  resultus;
                  b32  finita;
};

interior vacuum
_mora_expiravit (
    vacuum* datum);
interior vacuum
_fossa_parata (
    integer  fossa,
        i32  eventus,
     vacuum* datum);

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

interior vacuum
_moram_ponere (
    HttpVocatio* v,
            i32  ms)
{
    si (v->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(v->reactor, v->mora);
    }
    v->mora         = reactor_timer_adicere(v->reactor, ms, FALSUM,
        _mora_expiravit, v);
    v->mora_posita  = (v->mora != REACTOR_TIMER_ERROR);
}

interior vacuum
_fossam_ponere (
    HttpVocatio* v,
            i32  eventus)
{
    integer fd = (integer)tcp_obtinere_fd(v->tcp);

    si (v->fossa == fd)
    {
        (vacuum)reactor_modificare(v->reactor, fd, eventus);
        redde;
    }
    si (v->fossa >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(v->reactor, v->fossa);
    }
    v->fossa = fd;
    (vacuum)reactor_adicere(v->reactor, fd, eventus, _fossa_parata, v);
}

/* fossa e reactore PRIUS, deinde clausura (numerus fossae reusus) */
interior vacuum
_connexionem_dimittere (
    HttpVocatio* v)
{
    si (v->fossa >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(v->reactor, v->fossa);
        v->fossa = -I;
    }
    si (v->tls)
    {
        tls_claudere(v->tls);
        v->tls = NIHIL;
        v->tcp = NIHIL;
    }
    alioquin si (v->tcp)
    {
        tcp_claudere(v->tcp);
        v->tcp = NIHIL;
    }
}

interior vacuum
_octetos_obliterare (
    HttpVocatio* v)
{
    si (v->octeti.datum && v->octeti.mensura > ZEPHYRUM)
    {
        memset(v->octeti.datum, ZEPHYRUM, (size_t)v->octeti.mensura);
    }
}

interior vacuum
_finire (
          HttpVocatio* v,
    HttpVocatioStatus  status,
            HttpError  error,
               chorda  descriptio)
{
    si (v->finita)
    {
        redde;
    }
    v->finita = VERUM;
    si (v->quaestio)
    {
        nomina_quaerere_abrumpere(v->quaestio);
        v->quaestio = NIHIL;
    }
    si (v->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(v->reactor, v->mora);
        v->mora_posita = FALSUM;
    }
    _connexionem_dimittere(v);
    _octetos_obliterare(v);
    v->status              = status;
    v->resultus.successus  = (status == HTTP_VOCATIO_FINITA);
    v->resultus.responsum         = v->resultus.successus
        ? http_lector_responsum(v->lector) : NIHIL;
    v->resultus.error =
        v->resultus.successus ? HTTP_OK : error;
    v->resultus.error_descriptio = descriptio;
    v->finis(v->finis_datum, v);
}

interior vacuum
_errorem_differre (
    HttpVocatio* v,
      HttpError  error,
         chorda  descriptio)
{
    v->error_pendens       = error;
    v->descriptio_pendens  = descriptio;
    _moram_ponere(v, ZEPHYRUM);
}

interior vacuum
_mora_expiravit (
    vacuum* datum)
{
    HttpVocatio* v = (HttpVocatio*)datum;

    v->mora_posita = FALSUM;
    si (v->finita)
    {
        redde;
    }
    si (v->error_pendens != HTTP_OK)
    {
        _finire(v, HTTP_VOCATIO_IRRITA, v->error_pendens,
            v->descriptio_pendens);
        redde;
    }
    _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_TIMEOUT,
        chorda_ex_literis(
        v->status <= HTTP_VOCATIO_PACTIO ? "tempus connexionis excessum"
                                         : "tempus otii excessum",
        v->piscina));
}

/* rivus vocantis custoditus: post finem (abruptum intra rivum) nihil */
interior vacuum
_rivus_custos (
         vacuum* datum,
    constans i8* octeti,
            i32  n)
{
    HttpVocatio* v = (HttpVocatio*)datum;

    si (!v->finita && v->rivus)
    {
        v->rivus(v->rivi_datum, octeti, n);
    }
}

interior vacuum
_connectere_proximum (
    HttpVocatio* v)
{
    dum (v->addressus_index < v->addressus_numerus)
    {
         NominaAddressus* a = &v->addressus[v->addressus_index];
             TcpResultus  res;

        v->addressus_index++;
        res = tcp_incipere(a->octeti, a->mensura, v->piscina);
        si (res.successus)
        {
            v->tcp     = res.connexio;
            v->status  = HTTP_VOCATIO_CONNEXIO;
            _fossam_ponere(v, REACTOR_SCRIBERE);
            redde;
        }
        v->error_ultimus = res.error_descriptio;
    }
    _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_CONNEXIO,
        v->error_ultimus.mensura > ZEPHYRUM ? v->error_ultimus
            : chorda_ex_literis("connexio recusata", v->piscina));
}

interior vacuum
_nomina_finis (
                      vacuum* datum,
    constans NominaAddressus* addressus,
                         i32  numerus,
                      chorda  error)
{
    HttpVocatio* v = (HttpVocatio*)datum;
            i32  k;

    v->quaestio = NIHIL;
    si (v->finita)
    {
        redde;
    }
    si (numerus == ZEPHYRUM)
    {
        _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_CONNEXIO,
            chorda_concatenare(chorda_ex_literis("nomen: ", v->piscina),
            error, v->piscina));
        redde;
    }
    per (k = ZEPHYRUM; k < numerus && k < NOMINA_ADDRESSUS_MAXIMI; k++)
    {
        v->addressus[k] = addressus[k];
    }
    v->addressus_numerus  = k;
    v->addressus_index    = ZEPHYRUM;
    _connectere_proximum(v);
}

interior vacuum
_mittere (
    HttpVocatio* v)
{
    dum (v->missa < v->octeti.mensura)
    {
        constans i8* initium = (constans i8*)v->octeti.datum + v->missa;
                i32  restans = v->octeti.mensura - v->missa;
                s32  n       = v->tls
                    ? tls_mittere(v->tls, initium, restans)
                    : tcp_mittere(v->tcp, initium, restans);

        si (n > ZEPHYRUM)
        {
            v->missa += (i32)n;
            _moram_ponere(v, v->mora_otii_ms);
            perge;
        }
        si (n == TLS_ITERUM || n == ZEPHYRUM)
        {
            _fossam_ponere(v, REACTOR_SCRIBERE);
            redde;
        }
        _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_IO,
            chorda_ex_literis(
            "mittere fallita", v->piscina));
        redde;
    }
    _octetos_obliterare(v);
    v->status = HTTP_VOCATIO_LEGENS;
    _fossam_ponere(v, REACTOR_LEGERE);
}

/* legere USQUE AD ITERUM: SecureTransport octetos decryptos tenere
 * potest quos fossa iam non nuntiat */
interior vacuum
_legere (
    HttpVocatio* v)
{
    dum (!v->finita)
    {
        s32 n = v->tls
            ? tls_recipere(v->tls, v->capsa, HTTP_VOCATIO_CAPSA)
            : tcp_recipere(v->tcp, v->capsa, HTTP_VOCATIO_CAPSA);
        HttpLectorStatus st;

        si (n > ZEPHYRUM)
        {
            _moram_ponere(v, v->mora_otii_ms);
            st = http_lector_adicere(v->lector, v->capsa, (i32)n);
            si (v->finita)
            {
                redde;                   /* abruptum intra rivum */
            }
            si (st == HTTP_LECTOR_FINITUS)
            {
                _finire(v, HTTP_VOCATIO_FINITA, HTTP_OK, _vacua());
                redde;
            }
            si (st == HTTP_LECTOR_ERROR)
            {
                _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_PARSE,
                    http_lector_error(v->lector));
                redde;
            }
            perge;
        }
        si (n == TCP_ITERUM)
        {
            redde;                       /* = TLS_ITERUM: exspecta */
        }
        si (n == ZEPHYRUM)
        {
            st = http_lector_finire(v->lector);
            si (st == HTTP_LECTOR_FINITUS)
            {
                _finire(v, HTTP_VOCATIO_FINITA, HTTP_OK, _vacua());
            }
            alioquin
            {
                _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_IO,
                    http_lector_error(v->lector).mensura > ZEPHYRUM
                    ? http_lector_error(v->lector)
                    : chorda_ex_literis(
                        "connexio clausa ante responsum", v->piscina));
            }
            redde;
        }
        _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_IO,
            chorda_ex_literis(
            "error retis inter receptionem", v->piscina));
        redde;
    }
}

interior vacuum
_pactionem_promovere (
    HttpVocatio* v)
{
    commutatio (tls_gradus(v->tls))
    {
        casus TLS_GRADUS_PERFECTUS:
            v->status = HTTP_VOCATIO_MITTENS;
            _moram_ponere(v, v->mora_otii_ms);
            _mittere(v);
            frange;
        casus TLS_GRADUS_LEGERE:
            _fossam_ponere(v, REACTOR_LEGERE);
            frange;
        casus TLS_GRADUS_SCRIBERE:
            _fossam_ponere(v, REACTOR_SCRIBERE);
            frange;
        ordinarius:
            _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_CONNEXIO,
                chorda_concatenare(chorda_ex_literis("TLS: ",
                v->piscina),
                tls_gradus_error(v->tls), v->piscina));
            frange;
    }
}

interior vacuum
_connexionem_perficere (
    HttpVocatio* v)
{
         chorda descriptio = _vacua();
    TlsResultus tr;

    si (tcp_incipere_perficere(v->tcp, &descriptio, v->piscina)
        != TCP_OK)
    {
        v->error_ultimus = descriptio;
        _connexionem_dimittere(v);
        _connectere_proximum(v);
        redde;
    }
    si (v->est_https)
    {
        tr = tls_incipere(v->tcp, http_petitio_hospes(v->petitio),
            NIHIL,
            v->piscina);
        si (!tr.successus)
        {
            v->tcp = NIHIL;              /* tls_incipere clausit */
            _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_CONNEXIO,
                tr.error_descriptio);
            redde;
        }
        v->tls     = tr.connexio;
        v->status  = HTTP_VOCATIO_PACTIO;
        _pactionem_promovere(v);
        redde;
    }
    v->status = HTTP_VOCATIO_MITTENS;
    _moram_ponere(v, v->mora_otii_ms);
    _mittere(v);
}

interior vacuum
_fossa_parata (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
    HttpVocatio* v = (HttpVocatio*)datum;

    (vacuum)fossa;
    (vacuum)eventus;
    si (v->finita)
    {
        redde;
    }
    commutatio (v->status)
    {
        casus HTTP_VOCATIO_CONNEXIO:
            _connexionem_perficere(v);
            frange;
        casus HTTP_VOCATIO_PACTIO:
            _pactionem_promovere(v);
            frange;
        casus HTTP_VOCATIO_MITTENS:
            _mittere(v);
            frange;
        casus HTTP_VOCATIO_LEGENS:
            _legere(v);
            frange;
        ordinarius:
            frange;
    }
}

HttpVocatio*
http_vocare (
                    Reactor* reactor,
                HttpPetitio* petitio,
                  HttpRivus  rivus,
                     vacuum* rivi_datum,
           HttpVocatioFinis  finis,
                     vacuum* finis_datum,
    constans HttpVocatioOptiones* optiones,
                    Piscina* piscina)
{
    HttpVocatio* v;
         chorda  error = _vacua();
    MoraAngusta  tempus;

    si (!reactor || !petitio || !finis || !piscina)
    {
        redde NIHIL;
    }
    v = (HttpVocatio*)piscina_allocare(piscina, magnitudo(HttpVocatio));
    memset(v, ZEPHYRUM, magnitudo(HttpVocatio));
    v->reactor        = reactor;
    v->piscina        = piscina;
    v->petitio        = petitio;
    v->rivus          = rivus;
    v->rivi_datum     = rivi_datum;
    v->finis          = finis;
    v->finis_datum    = finis_datum;
    v->status         = HTTP_VOCATIO_NOMINA;
    v->fossa          = -I;
    v->error_pendens  = HTTP_OK;
    v->est_https      = http_petitio_est_https(petitio);
    tempus            = http_petitio_tempus(petitio);
    v->mora_connexionis_ms = (optiones && optiones->mora_connexionis_ms
        > ZEPHYRUM) ? optiones->mora_connexionis_ms
        : (tempus
            > ZEPHYRUM ? (i32)tempus : HTTP_VOCATIO_MORA_CONNEXIONIS);
    v->mora_otii_ms   = (optiones && optiones->mora_otii_ms > ZEPHYRUM)
        ? optiones->mora_otii_ms : HTTP_VOCATIO_MORA_OTII;
    v->octeti         = http_petitio_octeti(petitio);
    v->lector         = http_lector_creare(piscina,
        http_petitio_methodus(petitio), _rivus_custos, v);
    v->capsa          = (i8*)piscina_allocare(piscina,
        (i64)HTTP_VOCATIO_CAPSA);
    _moram_ponere(v, v->mora_connexionis_ms);
    v->quaestio = nomina_quaerere_incipere(reactor, chorda_ex_literis(
        http_petitio_hospes(petitio), piscina),
        http_petitio_portus(petitio),
        _nomina_finis, v, piscina, &error);
    si (!v->quaestio)
    {
        _errorem_differre(v, HTTP_ERROR_CONNEXIO, error);
    }
    redde v;
}

HttpVocatioStatus
http_vocatio_status (
    constans HttpVocatio* v)
{
    redde v ? v->status : HTTP_VOCATIO_IRRITA;
}

b32
http_vocatio_finita (
    constans HttpVocatio* v)
{
    redde !v || v->finita;
}

HttpResultus
http_vocatio_resultus (
    constans HttpVocatio* v)
{
    HttpResultus r;

    si (!v || !v->finita)
    {
        memset(&r, ZEPHYRUM, magnitudo(r));
        r.error = HTTP_ERROR_IO;
        redde r;
    }
    redde v->resultus;
}

vacuum
http_vocationem_abrumpere (
    HttpVocatio* v)
{
    si (!v || v->finita)
    {
        redde;
    }
    _finire(v, HTTP_VOCATIO_ABRUPTA, HTTP_ERROR_IO, chorda_ex_literis(
        "abruptum", v->piscina));
}
