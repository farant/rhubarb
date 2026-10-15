/* probatio_tls_gradus.c - pactio TLS gradatim (http-vocatio II.3) sine
 * rete: par TCP localis qui servus TLS NON est. Salutatio clientis
 * (recordum 0x16 0x03) mittitur et gradus LEGERE reddit sine mora;
 * responsum pravum -> ERROR nominatus intra tectum, numquam obstat;
 * par clausus -> ERROR; tls_claudere fossam claudit (R5). Pactio vera:
 * probatio_http_vocatio_rete.c (facultas rete). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "reactor.h"
#include "nomina.h"
#include "tcp.h"
#include "tls.h"
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/time.h>

nomen structura {
        TcpServus* servus;
      TcpConnexio* par;       /* servi latus (acceptum) */
      TlsConnexio* tls;
              s32  fossa;     /* clientis */
} Iunctura;

nomen structura {
                i32 vocatus;
    NominaAddressus addressus;
} Responsum;

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

/* acceptio cum exspectatione: in loopback cliens "connexus" esse
 * potest antequam servus connexionem in cauda habet - auscultator
 * non-blocans tunc EAGAIN reddit (porta radix sub onere id vidit) */
interior TcpResultus
_accipere (
    TcpServus* servus,
      Piscina* p)
{
    structura pollfd pf;
         TcpResultus acc;
                 s64 initium = _ms_nunc();

    fac
    {
        pf.fd       = tcp_servus_obtinere_fd(servus);
        pf.events   = POLLIN;
        pf.revents  = 0;
        (vacuum)poll(&pf, I, (integer)L);
        acc = tcp_servus_accipere(servus, p);
    } dum (!acc.successus && acc.error == TCP_ERROR_ITERUM
           && _ms_nunc() - initium < II * M);
    redde acc;
}

interior vacuum
_nomina_finis (
                      vacuum* datum,
    constans NominaAddressus* addressus,
                         i32  numerus,
                      chorda  error)
{
    Responsum* r = (Responsum*)datum;

    (vacuum)error;
    r->vocatus++;
    si (numerus > ZEPHYRUM)
    {
        r->addressus = addressus[ZEPHYRUM];
    }
}

/* fossa parata ad eventus (poll() simplex, tectum ms) */
interior b32
_exspectare (
    s32 fossa,
    b32 scribere,
    i32 tectum_ms)
{
    structura pollfd pf;

    pf.fd       = (integer)fossa;
    pf.events   = scribere ? POLLOUT : POLLIN;
    pf.revents  = 0;
    redde poll(&pf, I, (integer)tectum_ms) == I;
}

/* servus 127.0.0.1, connexio non-blocans, acceptio, tls_incipere */
interior b32
_iungere (
     Iunctura* j,
      Piscina* p)
{
              Reactor* r = reactor_creare(p);
    TcpServusOptiones  o = tcp_servus_optiones_default();
    TcpServusResultus  sr;
            Responsum  x;
               chorda  error;
          TcpResultus  res;
          TcpResultus  acc;
          TlsResultus  tr;
                  s64  initium;

    sr = tcp_servus_creare_cum_optionibus("127.0.0.1", ZEPHYRUM, &o, p);
    si (!sr.successus || tcp_servus_auscultare(sr.servus, IV) != TCP_OK)
    {
        redde FALSUM;
    }
    j->servus = sr.servus;
    memset(&x, ZEPHYRUM, magnitudo(x));
    si (!nomina_quaerere_incipere(r, chorda_ex_literis("127.0.0.1", p),
            tcp_servus_obtinere_portum(sr.servus), _nomina_finis, &x, p,
            &error))
    {
        redde FALSUM;
    }
    initium = _ms_nunc();
    dum (x.vocatus == ZEPHYRUM && _ms_nunc() - initium < M)
    {
        (vacuum)reactor_poll(r, L);
    }
    res = tcp_incipere(x.addressus.octeti, x.addressus.mensura, p);
    si (!res.successus)
    {
        redde FALSUM;
    }
    j->fossa = tcp_obtinere_fd(res.connexio);
    si (   !_exspectare(j->fossa, VERUM, M)
        || tcp_incipere_perficere(res.connexio, &error, p) != TCP_OK)
    {
        redde FALSUM;
    }
    acc = _accipere(sr.servus, p);
    si (!acc.successus)
    {
        redde FALSUM;
    }
    j->par  = acc.connexio;
    tr      = tls_incipere(res.connexio, "localhost", NIHIL, p);
    si (!tr.successus)
    {
        redde FALSUM;
    }
    j->tls = tr.connexio;
    redde VERUM;
}

/* salutatio clientis mittitur, gradus LEGERE sine mora */
interior vacuum
_sectio_salutatio (
    CredoContextus* c)
{
    Iunctura j;
          i8 capsa[DXII];
         s32 n;
         s64 initium;

    memset(&j, ZEPHYRUM, magnitudo(j));
    CREDO_NECESSE(_iungere(&j, c->piscina));
    initium = _ms_nunc();
    CREDO_AEQUALIS_I32((i32)tls_gradus(j.tls), (i32)TLS_GRADUS_LEGERE);
    CREDO_VERUM(_ms_nunc() - initium < C);
    /* iterum sine responso: LEGERE, numquam 'tempus excessum' */
    CREDO_AEQUALIS_I32((i32)tls_gradus(j.tls), (i32)TLS_GRADUS_LEGERE);
    CREDO_NECESSE(_exspectare(tcp_obtinere_fd(j.par), FALSUM, M));
    n = tcp_recipere(j.par, capsa, DXII);
    CREDO_VERUM(n > V);
    CREDO_AEQUALIS_I32((i32)(insignatus character)capsa[ZEPHYRUM],
        XXII);
    CREDO_AEQUALIS_I32((i32)(insignatus character)capsa[I], III);
    tls_claudere(j.tls);
    tcp_claudere(j.par);
    tcp_servus_claudere(j.servus);
}

/* responsum non TLS: ERROR nominatus intra tectum */
interior vacuum
_sectio_pravum (
    CredoContextus* c)
{
     Iunctura j;
    TlsGradus g;
          s64 initium;
       chorda causa;

    memset(&j, ZEPHYRUM, magnitudo(j));
    CREDO_NECESSE(_iungere(&j, c->piscina));
    g = tls_gradus(j.tls);
    CREDO_NECESSE(g == TLS_GRADUS_LEGERE);
    CREDO_NECESSE(tcp_mittere_omnia(j.par,
        (constans i8*)"HTTP/1.1 400 Bad Request\r\n\r\n", XXVIII));
    initium = _ms_nunc();
    dum (g == TLS_GRADUS_LEGERE && _ms_nunc() - initium < II * M)
    {
        (vacuum)_exspectare(j.fossa, FALSUM, C);
        g = tls_gradus(j.tls);
    }
    CREDO_AEQUALIS_I32((i32)g, (i32)TLS_GRADUS_ERROR);
    causa = tls_gradus_error(j.tls);
    CREDO_VERUM(causa.mensura > ZEPHYRUM);
    tls_claudere(j.tls);
    tcp_claudere(j.par);
    tcp_servus_claudere(j.servus);
}

/* par claudit: ERROR; tls_claudere fossam clientis VERE claudit */
interior vacuum
_sectio_clausus (
    CredoContextus* c)
{
     Iunctura j;
    TlsGradus g;
          s64 initium;

    memset(&j, ZEPHYRUM, magnitudo(j));
    CREDO_NECESSE(_iungere(&j, c->piscina));
    g = tls_gradus(j.tls);
    CREDO_NECESSE(g == TLS_GRADUS_LEGERE);
    tcp_claudere(j.par);
    initium = _ms_nunc();
    dum (g == TLS_GRADUS_LEGERE && _ms_nunc() - initium < II * M)
    {
        (vacuum)_exspectare(j.fossa, FALSUM, C);
        g = tls_gradus(j.tls);
    }
    CREDO_AEQUALIS_I32((i32)g, (i32)TLS_GRADUS_ERROR);
    tls_claudere(j.tls);
    CREDO_VERUM(fcntl((integer)j.fossa, F_GETFD) == -I);
    tls_claudere(j.tls);
    tcp_servus_claudere(j.servus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "salutatio", _sectio_salutatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pravum", _sectio_pravum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausus", _sectio_clausus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tls_gradus", SECTIONES);
}
