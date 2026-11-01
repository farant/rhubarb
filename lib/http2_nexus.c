/* http2_nexus.c - connexio HTTP/2 diuturna super reactorem. Vide
 * include/http2_nexus.h; status ad modum http_vocatio.c (sed conatus
 * addressuum seriatim, non parallele - connexio una diuturna). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "reactor.h"
#include "nomina.h"
#include "tcp.h"
#include "tls.h"
#include "filum.h"
#include "hpack.h"
#include "http2_forma.h"
#include "http2.h"
#include "http2_nexus.h"
#include <stdio.h>
#include <string.h>

#define NEXUS_CAPSA              (XVI * MXXIV)
#define NEXUS_MORA_CONNEXIONIS   (XXX * M)
#define NEXUS_MORA_OTII          (LX * M)
#define NEXUS_SCHEDA             (LXIV * MXXIV)
#define NEXUS_PORTUS_ORDINARIUS  CDXLIII

nomen enumeratio {
    NEXUS_NOMINA = ZEPHYRUM,
    NEXUS_CONNEXIO,
    NEXUS_PACTIO,
    NEXUS_VIVUS,
    NEXUS_CLAUSUS
} NexusStatus;

nomen structura {
                  b32  occupata;
                  i32  rivus;
      Http2NexusFinis  finis;
               vacuum* ctx;
              Piscina* piscina;
                  i32  status;
                  Xar* capita;
    ChordaAedificator* corpus;
                  b32  differenda;    /* finis per timer 0 */
               chorda  causa;
                  b32  retentabilis;
} NexusPetitio;

structura Http2Nexus {
             Reactor* reactor;
             Piscina* piscina;
             Piscina* scheda;           /* eventus capti, vacatur */
           character  hospes[CCLVI];
           character  auctoritas[CCLXX];
         NexusStatus  status;
      NominaQuaestio* quaestio;
     NominaAddressus  addressus[NOMINA_ADDRESSUS_MAXIMI];
                 i32  addressus_numerus;
                 i32  addressus_index;
              chorda  error_ultimus;
         TcpConnexio* tcp;              /* tls possidet post pactionem */
         TlsConnexio* tls;
             integer  fossa;
       Http2Connexio* h2;
    Http2NexusStatus  status_nuntius;
              vacuum* status_ctx;
                 b32  vivus_nuntiatus;
                 b32  mors_nuntiata;
                 b32  discessus;
                 Xar* petitiones;       /* NexusPetitio */
                  i8* capsa;
      ReactorTimerId  mora;
                 b32  mora_posita;
                 i32  mora_connexionis_ms;
                 i32  mora_otii_ms;
      ReactorTimerId  differentia;
                 b32  differentia_posita;
              chorda  error_pendens;    /* perire per timer 0 */
  constans character* ancora;
       FilumScriptor* tabula_servi;
       FilumScriptor* tabula_clientis;
};

hic_manens constans character* constans PROTOCOLLA_H2[I] = { "h2" };

interior vacuum
_fossa_parata (
    integer  fossa,
        i32  eventus,
     vacuum* datum);

interior vacuum
_mora_expiravit (
    vacuum* datum);

interior vacuum
_differentia_expiravit (
    vacuum* datum);

interior vacuum
_conatum_incipere (
    Http2Nexus* x);

interior vacuum
_scribere (
    Http2Nexus* x);


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_literae (
            Http2Nexus* x,
    constans character* s)
{
    redde chorda_ex_literis(s, x->piscina);
}

interior i32
_pendentes (
    Http2Nexus* x)
{
    i32 i;
    i32 n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(x->petitiones); i++)
    {
        NexusPetitio* p = (NexusPetitio*)xar_obtinere(x->petitiones, i);

        si (p->occupata && !p->differenda)
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
_moram_tollere (
    Http2Nexus* x)
{
    si (x->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(x->reactor, x->mora);
        x->mora_posita = FALSUM;
    }
}

interior vacuum
_moram_ponere (
    Http2Nexus* x,
           i32  ms)
{
    _moram_tollere(x);
    x->mora         = reactor_timer_adicere(x->reactor, ms, FALSUM,
        _mora_expiravit, x);
    x->mora_posita  = x->mora != REACTOR_TIMER_ERROR;
}

/* vivus: mora otii solum dum petitiones pendent */
interior vacuum
_otium_renovare (
    Http2Nexus* x)
{
    si (x->status != NEXUS_VIVUS)
    {
        redde;
    }
    si (_pendentes(x) > ZEPHYRUM)
    {
        _moram_ponere(x, x->mora_otii_ms);
    }
    alioquin
    {
        _moram_tollere(x);
    }
}

interior vacuum
_differentiam_ponere (
    Http2Nexus* x)
{
    si (x->differentia_posita)
    {
        redde;
    }
    x->differentia = reactor_timer_adicere(x->reactor, ZEPHYRUM, FALSUM,
        _differentia_expiravit, x);
    x->differentia_posita = x->differentia != REACTOR_TIMER_ERROR;
}

interior vacuum
_fossam_ponere (
    Http2Nexus* x,
           i32  eventus)
{
    integer fd = (integer)tcp_obtinere_fd(x->tcp);

    si (x->fossa == fd)
    {
        (vacuum)reactor_modificare(x->reactor, fd, eventus);
        redde;
    }
    si (x->fossa >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(x->reactor, x->fossa);
    }
    x->fossa = fd;
    (vacuum)reactor_adicere(x->reactor, fd, eventus, _fossa_parata, x);
}

/* fossa e reactore PRIUS, deinde clausura */
interior vacuum
_connexionem_dimittere (
    Http2Nexus* x)
{
    si (x->fossa >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(x->reactor, x->fossa);
        x->fossa = -I;
    }
    si (x->tls)
    {
        tls_claudere(x->tls);
        x->tls = NIHIL;
        x->tcp = NIHIL;
    }
    alioquin si (x->tcp)
    {
        tcp_claudere(x->tcp);
        x->tcp = NIHIL;
    }
    si (x->tabula_servi)
    {
        (vacuum)filum_scriptor_claudere(x->tabula_servi);
        x->tabula_servi = NIHIL;
    }
    si (x->tabula_clientis)
    {
        (vacuum)filum_scriptor_claudere(x->tabula_clientis);
        x->tabula_clientis = NIHIL;
    }
}

/* finis unius petitionis: locus PRIUS vacatur (finis petere potest) */
interior vacuum
_petitionem_finire (
      Http2Nexus* x,
    NexusPetitio* p,
             b32  successus,
          chorda  causa,
             b32  retentabilis)
{
     Http2Responsum  r;
    Http2NexusFinis  finis;
             vacuum* ctx;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.successus  = successus;
    r.status     = p->status;
    r.capita     = p->capita;
    r.corpus        = p->corpus ? chorda_aedificator_finire(p->corpus)
                                : causa;
    si (!successus)
    {
        r.corpus.datum    = NIHIL;
        r.corpus.mensura  = ZEPHYRUM;
    }
    r.causa         = causa;
    r.retentabilis  = retentabilis;
    finis           = p->finis;
    ctx             = p->ctx;
    memset(p, ZEPHYRUM, magnitudo(NexusPetitio));
    (vacuum)x;
    finis(ctx, &r);
}

interior vacuum
_mortem_nuntiare (
    Http2Nexus* x,
        chorda  causa)
{
    si (x->mors_nuntiata)
    {
        redde;
    }
    x->mors_nuntiata = VERUM;
    si (x->status_nuntius)
    {
        x->status_nuntius(x->status_ctx, FALSUM, causa);
    }
}

/* connexio perit: status FALSUM semel, petitiones omnes cum defectu */
interior vacuum
_perire (
    Http2Nexus* x,
        chorda  causa)
{
    i32 i;

    si (x->status == NEXUS_CLAUSUS)
    {
        redde;
    }
    x->status = NEXUS_CLAUSUS;
    _moram_tollere(x);
    si (x->quaestio)
    {
        nomina_quaerere_abrumpere(x->quaestio);
        x->quaestio = NIHIL;
    }
    _connexionem_dimittere(x);
    _mortem_nuntiare(x, causa);
    per (i = ZEPHYRUM; i < xar_numerus(x->petitiones); i++)
    {
        NexusPetitio* p = (NexusPetitio*)xar_obtinere(x->petitiones, i);

        si (p->occupata)
        {
            /* differenda causam suam servant; exspectans numquam missa
             * (supra MAX_CONCURRENT_STREAMS) tuto iteratur */
            _petitionem_finire(x, p, FALSUM, p->differenda ? p->causa
                : causa, p->differenda ? p->retentabilis
                : http2_rivus_exspectat(x->h2, p->rivus));
        }
    }
}

interior NexusPetitio*
_petitio (
    Http2Nexus* x,
           i32  rivus)
{
    i32 i;

    per (i = ZEPHYRUM; rivus > ZEPHYRUM
        && i < xar_numerus(x->petitiones);
         i++)
    {
        NexusPetitio* p = (NexusPetitio*)xar_obtinere(x->petitiones, i);

        si (p->occupata && p->rivus == rivus)
        {
            redde p;
        }
    }
    redde NIHIL;
}

interior NexusPetitio*
_locum_novum (
    Http2Nexus* x)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(x->petitiones); i++)
    {
        NexusPetitio* p = (NexusPetitio*)xar_obtinere(x->petitiones, i);

        si (!p->occupata)
        {
            redde p;
        }
    }
    redde (NexusPetitio*)xar_addere(x->petitiones);
}

interior chorda
_copia (
    chorda  s,
   Piscina* piscina)
{
    chorda r;

    r.mensura  = s.mensura;
    r.datum    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(s.mensura + I));
    si (r.datum && s.mensura > ZEPHYRUM)
    {
        memcpy(r.datum, s.datum, (size_t)s.mensura);
    }
    redde r;
}


/* ==================================================
 * Eventus connexionis -> petitiones
 * ================================================== */

interior i32
_status_legere (
    chorda valor)
{
    i32 s;
    i32 i;

    s = ZEPHYRUM;
    per (i = ZEPHYRUM; i < valor.mensura && i < III; i++)
    {
        s = s * X + (i32)(valor.datum[i] - '0');
    }
    redde s;
}

interior vacuum
_capita_addere (
    NexusPetitio* p,
             Xar* capita)
{
    i32 i;

    si (!p->capita)
    {
        p->capita = xar_creare(p->piscina, (i32)magnitudo(HpackCaput));
    }
    per (i = ZEPHYRUM; p->capita && i < xar_numerus(capita); i++)
    {
        HpackCaput* h = (HpackCaput*)xar_obtinere(capita, i);
        HpackCaput* d;

        si (chorda_aequalis_literis(h->titulus, ":status"))
        {
            p->status = _status_legere(h->valor);
            perge;
        }
        d = (HpackCaput*)xar_addere(p->capita);
        si (d)
        {
            d->titulus  = _copia(h->titulus, p->piscina);
            d->valor    = _copia(h->valor, p->piscina);
            d->numquam  = h->numquam;
        }
    }
}

interior vacuum
_eventus_tractare (
    Http2Nexus* x)
{
    Http2Eventus ev;
       character t[XCVI];

    dum (   x->status != NEXUS_CLAUSUS
         && http2_eventum_capere(x->h2, &ev, x->scheda))
    {
        NexusPetitio* p = _petitio(x, ev.rivus);

        commutatio (ev.genus)
        {
            casus HTTP2_EVENTUS_CAPITA:
                si (p && !ev.informativa)
                {
                    _capita_addere(p, ev.capita);
                }
                frange;
            casus HTTP2_EVENTUS_DATA:
                si (p && p->corpus)
                {
                    (vacuum)chorda_aedificator_appendere_chorda(p->corpus,
                        ev.data);
                }
                frange;
            casus HTTP2_EVENTUS_FINIS:
                si (p)
                {
                    _petitionem_finire(x, p, VERUM, _literae(x, ""),
                        FALSUM);
                }
                frange;
            casus HTTP2_EVENTUS_ABROGATUS:
                si (p)
                {
                    sprintf(t, "rivus abrogatus a servo (codex %u)",
                        (insignatus integer)ev.codex);
                    _petitionem_finire(x, p, FALSUM, _literae(x, t),
                        ev.codex == HTTP2_ERROR_RECUSATUM);
                }
                frange;
            casus HTTP2_EVENTUS_DISCESSUS:
                si (ev.rivus > ZEPHYRUM)
                {
                    si (p)
                    {
                        _petitionem_finire(x, p, FALSUM, _literae(x,
                            "GOAWAY: petitio a servo non tractata"),
                            VERUM);
                    }
                }
                alioquin
                {
                    x->discessus = VERUM;
                    sprintf(t, "GOAWAY a servo (codex %u, ultimus %u)",
                        (insignatus integer)ev.codex,
                        (insignatus integer)ev.rivus_ultimus);
                    _mortem_nuntiare(x, _literae(x, t));
                }
                frange;
            casus HTTP2_EVENTUS_ERROR:
                sprintf(t, "HTTP/2: connexio mortua (codex %u)",
                    (insignatus integer)ev.codex);
                _scribere(x);
                _perire(x, _literae(x, t));
                frange;
            ordinarius:
                frange;
        }
    }
    si (x->status == NEXUS_CLAUSUS)
    {
        redde;
    }
    /* scheda vacatur cum omnes capti (http2_eventum_capere FALSUM) */
    piscina_vacare(x->scheda);
    si (x->discessus && _pendentes(x) == ZEPHYRUM)
    {
        x->status = NEXUS_CLAUSUS;
        _moram_tollere(x);
        _connexionem_dimittere(x);
    }
}


/* ==================================================
 * IO
 * ================================================== */

interior vacuum
_scribere (
    Http2Nexus* x)
{
    si (x->status != NEXUS_VIVUS || !x->tls)
    {
        redde;
    }
    dum (VERUM)
    {
        constans i8* d;
                i32  n;
                s32  k;

        n = http2_mittenda(x->h2, &d);
        si (n == ZEPHYRUM)
        {
            frange;
        }
        k = tls_mittere(x->tls, d, n);
        si (k > ZEPHYRUM)
        {
            si (x->tabula_clientis)
            {
                (vacuum)filum_scriptor_octetos_scribere(x->tabula_clientis,
                    d, (i32)k);
            }
            http2_mittenda_consumere(x->h2, (i32)k);
            perge;
        }
        si (k == TLS_ITERUM || k == ZEPHYRUM)
        {
            _fossam_ponere(x, REACTOR_LEGERE | REACTOR_SCRIBERE);
            redde;
        }
        _perire(x, _literae(x, "mittere fallita"));
        redde;
    }
    _fossam_ponere(x, REACTOR_LEGERE);
}

/* legere usque ad ITERUM (SecureTransport octetos tenere potest) */
interior vacuum
_legere (
    Http2Nexus* x)
{
    dum (x->status == NEXUS_VIVUS)
    {
        s32 n = tls_recipere(x->tls, x->capsa, NEXUS_CAPSA);

        si (n > ZEPHYRUM)
        {
            b32 viva;

            si (x->tabula_servi)
            {
                (vacuum)filum_scriptor_octetos_scribere(x->tabula_servi,
                    x->capsa, (i32)n);
            }
            viva = http2_recipere(x->h2, x->capsa, (i32)n);
            _eventus_tractare(x);
            si (x->status != NEXUS_VIVUS)
            {
                redde;
            }
            si (!viva)
            {
                _scribere(x);
                _perire(x, _literae(x, "HTTP/2: connexio mortua"));
                redde;
            }
            _otium_renovare(x);
            perge;
        }
        si (n == TCP_ITERUM)
        {
            frange;
        }
        _perire(x, _literae(x, n == ZEPHYRUM
            ? "connexio a servo clausa" : "error retis inter receptionem"));
        redde;
    }
    _scribere(x);
}

interior vacuum
_vivere (
    Http2Nexus* x)
{
    chorda electum = tls_protocollum(x->tls);

    si (!chorda_aequalis_literis(electum, "h2"))
    {
        _perire(x, _literae(x, "hospes h2 non elegit (ALPN)"));
        redde;
    }
    x->status = NEXUS_VIVUS;
    _moram_tollere(x);
    si (!x->vivus_nuntiatus && x->status_nuntius)
    {
        x->vivus_nuntiatus = VERUM;
        x->status_nuntius(x->status_ctx, VERUM, _literae(x, ""));
        si (x->status != NEXUS_VIVUS)
        {
            redde;
        }
    }
    _otium_renovare(x);
    _scribere(x);
}

interior vacuum
_pactionem_promovere (
    Http2Nexus* x)
{
    commutatio (tls_gradus(x->tls))
    {
        casus TLS_GRADUS_PERFECTUS:
            _vivere(x);
            frange;
        casus TLS_GRADUS_LEGERE:
            _fossam_ponere(x, REACTOR_LEGERE);
            frange;
        casus TLS_GRADUS_SCRIBERE:
            _fossam_ponere(x, REACTOR_SCRIBERE);
            frange;
        ordinarius:
            _perire(x, chorda_concatenare(_literae(x, "TLS: "),
                tls_gradus_error(x->tls), x->piscina));
            frange;
    }
}

/* TCP connexa: TLS cum ALPN h2 */
interior vacuum
_connexum (
    Http2Nexus* x)
{
    TlsOptiones opt = tls_optiones_default();
    TlsResultus tr;

    (vacuum)reactor_removere(x->reactor, x->fossa);
    x->fossa = -I;
    opt.ancora_via = x->ancora;
    opt.protocolla = PROTOCOLLA_H2;
    opt.protocolla_numerus = I;
    tr = tls_incipere(x->tcp, x->hospes, &opt, x->piscina);
    si (!tr.successus)
    {
        x->tcp = NIHIL;                  /* tls_incipere clausit */
        _perire(x, tr.error_descriptio);
        redde;
    }
    x->tls     = tr.connexio;
    x->status  = NEXUS_PACTIO;
    _pactionem_promovere(x);
}

interior vacuum
_fossa_parata (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
     Http2Nexus* x = (Http2Nexus*)datum;
         chorda  descriptio;

    (vacuum)fossa;
    commutatio (x->status)
    {
        casus NEXUS_CONNEXIO:
            descriptio.datum    = NIHIL;
            descriptio.mensura  = ZEPHYRUM;
            si (tcp_incipere_perficere(x->tcp, &descriptio, x->piscina)
                != TCP_OK)
            {
                x->error_ultimus = descriptio;
                (vacuum)reactor_removere(x->reactor, x->fossa);
                x->fossa = -I;
                tcp_claudere(x->tcp);
                x->tcp = NIHIL;
                _conatum_incipere(x);
                frange;
            }
            _connexum(x);
            frange;
        casus NEXUS_PACTIO:
            _pactionem_promovere(x);
            frange;
        casus NEXUS_VIVUS:
            si (eventus & REACTOR_SCRIBERE)
            {
                _scribere(x);
            }
            si (   x->status == NEXUS_VIVUS
                && (eventus & (REACTOR_LEGERE | REACTOR_ERROR
                               | REACTOR_CLAUSUM)))
            {
                _legere(x);
            }
            frange;
        ordinarius:
            frange;
    }
}

/* addressus proximus seriatim */
interior vacuum
_conatum_incipere (
    Http2Nexus* x)
{
    dum (x->addressus_index < x->addressus_numerus)
    {
        NominaAddressus* a = &x->addressus[x->addressus_index++];
            TcpResultus  res;

        res = tcp_incipere(a->octeti, a->mensura, x->piscina);
        si (!res.successus)
        {
            x->error_ultimus = res.error_descriptio;
            perge;
        }
        x->tcp     = res.connexio;
        x->status  = NEXUS_CONNEXIO;
        _fossam_ponere(x, REACTOR_SCRIBERE);
        redde;
    }
    _perire(x, x->error_ultimus.mensura > ZEPHYRUM ? x->error_ultimus
        : _literae(x, "connexio recusata"));
}

interior vacuum
_nomina_finis (
                      vacuum* datum,
    constans NominaAddressus* addressus,
                         i32  numerus,
                      chorda  error)
{
    Http2Nexus* x = (Http2Nexus*)datum;
           i32  k;

    x->quaestio = NIHIL;
    si (x->status == NEXUS_CLAUSUS)
    {
        redde;
    }
    si (numerus == ZEPHYRUM)
    {
        _perire(x, chorda_concatenare(_literae(x, "nomen: "), error,
            x->piscina));
        redde;
    }
    per (k = ZEPHYRUM; k < numerus && k < NOMINA_ADDRESSUS_MAXIMI; k++)
    {
        x->addressus[k] = addressus[k];
    }
    x->addressus_numerus  = k;
    x->addressus_index    = ZEPHYRUM;
    _conatum_incipere(x);
}

interior vacuum
_mora_expiravit (
    vacuum* datum)
{
    Http2Nexus* x = (Http2Nexus*)datum;

    x->mora_posita = FALSUM;
    si (x->status == NEXUS_CLAUSUS)
    {
        redde;
    }
    si (x->error_pendens.mensura > ZEPHYRUM)
    {
        _perire(x, x->error_pendens);
        redde;
    }
    si (x->status < NEXUS_VIVUS)
    {
        _perire(x, _literae(x, "tempus connexionis excessum"));
        redde;
    }
    si (_pendentes(x) > ZEPHYRUM)
    {
        _perire(x, _literae(x, "tempus otii excessum"));
    }
}

/* addressus dati (sine DNS) aut conatus primus: per timer 0 */
interior vacuum
_differentia_expiravit (
    vacuum* datum)
{
    Http2Nexus* x = (Http2Nexus*)datum;
           i32  i;

    x->differentia_posita = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(x->petitiones); i++)
    {
        NexusPetitio* p = (NexusPetitio*)xar_obtinere(x->petitiones, i);

        si (p->occupata && p->differenda)
        {
            _petitionem_finire(x, p, FALSUM, p->causa, p->retentabilis);
        }
    }
    si (   x->status == NEXUS_NOMINA && x->addressus_numerus > ZEPHYRUM
        && x->addressus_index == ZEPHYRUM && x->quaestio == NIHIL)
    {
        _conatum_incipere(x);
    }
}


/* ==================================================
 * Publica
 * ================================================== */

Http2Nexus*
http2_nexum_aperire (
                    Reactor* reactor,
         constans character* hospes,
                        i32  portus,
    constans Http2NexusOptiones* optiones,
           Http2NexusStatus  status,
                     vacuum* ctx,
                    Piscina* piscina)
{
     Http2Nexus* x;
         chorda  error;

    si (!reactor || !hospes || !piscina || strlen(hospes) >= CCLVI)
    {
        redde NIHIL;
    }
    x = (Http2Nexus*)piscina_allocare(piscina, magnitudo(Http2Nexus));
    si (!x)
    {
        redde NIHIL;
    }
    memset(x, ZEPHYRUM, magnitudo(Http2Nexus));
    x->reactor         = reactor;
    x->piscina         = piscina;
    x->status_nuntius  = status;
    x->status_ctx      = ctx;
    x->fossa           = -I;
    x->status          = NEXUS_NOMINA;
    strcpy(x->hospes, hospes);
    si (portus == NEXUS_PORTUS_ORDINARIUS)
    {
        strcpy(x->auctoritas, hospes);
    }
    alioquin
    {
        sprintf(x->auctoritas, "%s:%u", hospes,
            (insignatus integer)portus);
    }
    x->mora_connexionis_ms = (optiones && optiones->mora_connexionis_ms)
        ? optiones->mora_connexionis_ms : NEXUS_MORA_CONNEXIONIS;
    x->mora_otii_ms = (optiones && optiones->mora_otii_ms)
        ? optiones->mora_otii_ms : NEXUS_MORA_OTII;
    x->ancora  = optiones ? optiones->tls_ancora_via : NIHIL;
    x->h2      = http2_connexionem_creare(HTTP2_CLIENS, NIHIL,
        piscina);
    x->scheda      = piscina_generare_dynamicum("http2_nexus",
        NEXUS_SCHEDA);
    x->petitiones  = xar_creare(piscina, (i32)magnitudo(NexusPetitio));
    x->capsa       = (i8*)piscina_allocare(piscina, NEXUS_CAPSA);
    si (!x->h2 || !x->scheda || !x->petitiones || !x->capsa)
    {
        http2_connexionem_destruere(x->h2);
        si (x->scheda)
        {
            piscina_destruere(x->scheda);
        }
        redde NIHIL;
    }
    si (optiones && optiones->tabula_via)
    {
        character via[MXXIV];

        sprintf(via, "%.1000s.servus.bin", optiones->tabula_via);
        x->tabula_servi = filum_scriptor_aperire(via,
            FILUM_MODUS_CREARE,
            piscina);
        sprintf(via, "%.1000s.cliens.bin", optiones->tabula_via);
        x->tabula_clientis = filum_scriptor_aperire(via,
            FILUM_MODUS_CREARE, piscina);
    }
    _moram_ponere(x, x->mora_connexionis_ms);
    si (optiones && optiones->addressus && optiones->addressus_numerus)
    {
        i32 k;

        per (k = ZEPHYRUM; k < optiones->addressus_numerus
             && k < NOMINA_ADDRESSUS_MAXIMI; k++)
        {
            x->addressus[k] = optiones->addressus[k];
        }
        x->addressus_numerus = k;
        _differentiam_ponere(x);
        redde x;
    }
    error.datum    = NIHIL;
    error.mensura  = ZEPHYRUM;
    x->quaestio = nomina_quaerere_incipere(reactor, _literae(x, hospes),
        portus, _nomina_finis, x, piscina, &error);
    si (!x->quaestio)
    {
        x->error_pendens = error.mensura > ZEPHYRUM ? error
            : _literae(x, "nomen quaeri non potest");
        _moram_ponere(x, ZEPHYRUM);
    }
    redde x;
}

b32
http2_nexus_petere (
               Http2Nexus* x,
       constans character* methodus,
       constans character* via,
      constans HpackCaput* capita,
                      i32  numerus,
                   chorda  corpus,
          Http2NexusFinis  finis,
                   vacuum* ctx,
                  Piscina* piscina)
{
    NexusPetitio* p;
      HpackCaput* h;
             i32  i;

    si (   !x || !methodus || !via || !finis || !piscina
        || (numerus > ZEPHYRUM && !capita))
    {
        redde FALSUM;
    }
    p = _locum_novum(x);
    si (!p)
    {
        redde FALSUM;
    }
    memset(p, ZEPHYRUM, magnitudo(NexusPetitio));
    p->occupata  = VERUM;
    p->finis     = finis;
    p->ctx       = ctx;
    p->piscina   = piscina;
    p->corpus    = chorda_aedificator_creare(piscina, MXXIV);
    si (x->status == NEXUS_CLAUSUS || x->discessus)
    {
        p->differenda    = VERUM;
        p->causa         = _literae(x, x->discessus
            ? "GOAWAY: connexio nova petitiones non accipit"
            : "connexio clausa");
        p->retentabilis  = VERUM;
        _differentiam_ponere(x);
        redde VERUM;
    }
    h = (HpackCaput*)piscina_allocare(piscina,
        (memoriae_index)(numerus + IV) * magnitudo(HpackCaput));
    si (!h)
    {
        memset(p, ZEPHYRUM, magnitudo(NexusPetitio));
        redde FALSUM;
    }
    h[ZEPHYRUM].titulus  = chorda_ex_literis(":method", piscina);
    h[ZEPHYRUM].valor    = chorda_ex_literis(methodus, piscina);
    h[I].titulus         = chorda_ex_literis(":scheme", piscina);
    h[I].valor           = chorda_ex_literis("https", piscina);
    h[II].titulus        = chorda_ex_literis(":authority", piscina);
    h[II].valor          = chorda_ex_literis(x->auctoritas, piscina);
    h[III].titulus       = chorda_ex_literis(":path", piscina);
    h[III].valor         = chorda_ex_literis(via, piscina);
    h[ZEPHYRUM].numquam  = h[I].numquam = h[II].numquam = h[III].numquam
                         = FALSUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        h[IV + i] = capita[i];
    }
    p->rivus = http2_petere(x->h2, h, numerus + IV, corpus);
    si (p->rivus == ZEPHYRUM)
    {
        p->differenda    = VERUM;
        p->causa         = _literae(x, "petitio a connexione recusata");
        p->retentabilis  = VERUM;
        _differentiam_ponere(x);
        redde VERUM;
    }
    si (x->status == NEXUS_VIVUS)
    {
        _otium_renovare(x);
        _scribere(x);
    }
    redde VERUM;
}

vacuum
http2_nexum_claudere (
    Http2Nexus* x)
{
    i32 i;

    si (!x)
    {
        redde;
    }
    si (x->status == NEXUS_VIVUS)
    {
        http2_discedere(x->h2, HTTP2_ERROR_NULLUS);
        _scribere(x);
    }
    x->status = NEXUS_CLAUSUS;
    _moram_tollere(x);
    si (x->differentia_posita)
    {
        (vacuum)reactor_timer_cancellare(x->reactor, x->differentia);
        x->differentia_posita = FALSUM;
    }
    si (x->quaestio)
    {
        nomina_quaerere_abrumpere(x->quaestio);
        x->quaestio = NIHIL;
    }
    _connexionem_dimittere(x);
    per (i = ZEPHYRUM; i < xar_numerus(x->petitiones); i++)
    {
        NexusPetitio* p = (NexusPetitio*)xar_obtinere(x->petitiones, i);

        si (p->occupata)
        {
            _petitionem_finire(x, p, FALSUM, p->differenda ? p->causa
                : _literae(x, "connexio clausa"), p->retentabilis);
        }
    }
    http2_connexionem_destruere(x->h2);
    si (x->scheda)
    {
        piscina_destruere(x->scheda);
        x->scheda = NIHIL;
    }
}
