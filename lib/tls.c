/* tls.c - tls.h: DISPATCHER vecturarum (tls-bearssl-spec II.5). Vectura
 * per connexionem eligitur (TlsOptiones.vectura; ORDINARIA ->
 * RHUBARB_TLS; aliter ordinaria domus) et in connexione servatur.
 * Vecturae: tls_securetransport_macos.m; tls_bearssl.c; tls_tls13.c
 * (TLS 1.3 proprius, tls13-spec). Ambitus
 * per lectiones_ambitus legitur (fabrica dependentiam videt). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tcp.h"
#include "tls.h"
#include "tls_securetransport.h"
#include "tls_bearssl.h"
#include "tls_tls13.h"
#include "lectiones.h"
#include <poll.h>
#include <string.h>
#include <unistd.h>

/* ordinaria domus: TLS 1.3 proprius (tls13 T8), hospites 1.2 per
 * descensum BearSSL cum signo DOWNGRD. RHUBARB_TLS=bearssl: solum 1.2
 * (vectura T6 tls-bearssl); RHUBARB_TLS=securetransport: vectura vetus,
 * delenda post tempus sine differentia */
#define TLS_VECTURA_DOMUS TLS_VECTURA_TLS13

structura TlsConnexio {
            TlsVectura  vectura;
    TlsSecuretransport* securetransport;
            TlsBearssl* bearssl;
              TlsTls13* tls13;
    /* descensus gradatim (vectura TLS13, tls13-spec II.3): quae BearSSL
     * in eadem connexione postea requirit */
           TcpConnexio* tcp;
    constans character* hospes;
           TlsOptiones  optiones;      /* copia; ancora in piscina */
               Piscina* piscina;
                   b32  redintegrans;  /* connexio nova pendet */
                chorda  erratum;       /* error tls.c ipsius */
};

TlsOptiones
tls_optiones_default (vacuum)
{
    TlsOptiones opt;

    memset(&opt, ZEPHYRUM, magnitudo(opt));
    opt.timeout_ms               = XXX * M;
    opt.verificare_certificatum  = VERUM;
    opt.ancora_via               = NIHIL;
    opt.vectura                  = TLS_VECTURA_ORDINARIA;
    redde opt;
}

TlsVectura
tls_vecturam_eligere (
    constans TlsOptiones* optiones)
{
    constans character* ambitus;

    si (optiones != NIHIL && optiones->vectura != TLS_VECTURA_ORDINARIA)
    {
        redde optiones->vectura;
    }
    ambitus = lectiones_ambitus("RHUBARB_TLS");
    si (ambitus != NIHIL && strcmp(ambitus, "bearssl") == ZEPHYRUM)
    {
        redde TLS_VECTURA_BEARSSL;
    }
    si (   ambitus                            != NIHIL
        && strcmp(ambitus, "securetransport") == ZEPHYRUM)
    {
        redde TLS_VECTURA_SECURETRANSPORT;
    }
    si (ambitus != NIHIL && strcmp(ambitus, "tls13") == ZEPHYRUM)
    {
        redde TLS_VECTURA_TLS13;
    }
    redde TLS_VECTURA_DOMUS;
}

interior TlsResultus
_error (
              TlsError  error,
    constans character* nuntius,
               Piscina* piscina)
{
    TlsResultus r;

    r.successus         = FALSUM;
    r.connexio          = NIHIL;
    r.error             = error;
    r.error_descriptio  = chorda_ex_literis(nuntius, piscina);
    redde r;
}

/* resultus vecturae SecureTransport -> TlsResultus */
interior TlsResultus
_involvere_securetransport (
    TlsSecuretransportResultus  st,
                       Piscina* piscina)
{
    TlsResultus r;

    r.successus         = st.successus;
    r.error             = st.error;
    r.error_descriptio  = st.error_descriptio;
    r.connexio          = NIHIL;
    si (st.successus)
    {
        r.connexio = (TlsConnexio*)piscina_allocare(piscina,
            (i64)magnitudo(TlsConnexio));
        si (r.connexio == NIHIL)
        {
            tls_securetransport_claudere(st.connexio);
            redde _error(TLS_ERROR_TCP, "Allocatio fallita", piscina);
        }
        memset(r.connexio, ZEPHYRUM, magnitudo(TlsConnexio));
        r.connexio->vectura          = TLS_VECTURA_SECURETRANSPORT;
        r.connexio->securetransport  = st.connexio;
    }
    redde r;
}

/* resultus vecturae BearSSL -> TlsResultus */
interior TlsResultus
_involvere_bearssl (
    TlsBearsslResultus  br,
               Piscina* piscina)
{
    TlsResultus r;

    r.successus         = br.successus;
    r.error             = br.error;
    r.error_descriptio  = br.error_descriptio;
    r.connexio          = NIHIL;
    si (br.successus)
    {
        r.connexio = (TlsConnexio*)piscina_allocare(piscina,
            (i64)magnitudo(TlsConnexio));
        si (r.connexio == NIHIL)
        {
            tls_bearssl_claudere(br.connexio);
            redde _error(TLS_ERROR_TCP, "Allocatio fallita", piscina);
        }
        memset(r.connexio, ZEPHYRUM, magnitudo(TlsConnexio));
        r.connexio->vectura = TLS_VECTURA_BEARSSL;
        r.connexio->bearssl = br.connexio;
    }
    redde r;
}

/* resultus vecturae TLS 1.3 -> TlsResultus */
interior TlsResultus
_involvere_tls13 (
    TlsTls13Resultus  t3,
             Piscina* piscina)
{
    TlsResultus r;

    r.successus         = t3.successus;
    r.error             = t3.error;
    r.error_descriptio  = t3.error_descriptio;
    r.connexio          = NIHIL;
    si (t3.successus)
    {
        r.connexio = (TlsConnexio*)piscina_allocare(piscina,
            (i64)magnitudo(TlsConnexio));
        si (r.connexio == NIHIL)
        {
            tls_tls13_claudere(t3.connexio);
            redde _error(TLS_ERROR_TCP, "Allocatio fallita", piscina);
        }
        memset(r.connexio, ZEPHYRUM, magnitudo(TlsConnexio));
        r.connexio->vectura  = TLS_VECTURA_TLS13;
        r.connexio->tls13    = t3.connexio;
    }
    redde r;
}

/* optiones (NIHIL licet) in copiam: ancora in piscinam */
interior b32
_optiones_copiare (
     constans TlsOptiones* optiones,
              TlsOptiones* exitus,
                  Piscina* piscina)
{
    *exitus = optiones ? *optiones : tls_optiones_default();
    si (exitus->ancora_via != NIHIL)
    {
        memoriae_index  n = strlen(exitus->ancora_via) + I;
             character* d = (character*)piscina_allocare(piscina,
                 (i64)n);

        si (d == NIHIL)
        {
            redde FALSUM;
        }
        memcpy(d, exitus->ancora_via, n);
        exitus->ancora_via = d;
    }
    redde VERUM;
}

/* fossa paratane ad scribendum (connexio redintegrata perfecta)? */
interior b32
_scribibilis (
    TcpConnexio* tcp)
{
    structura pollfd pf;

    pf.fd       = (integer)tcp_obtinere_fd(tcp);
    pf.events   = POLLOUT;
    pf.revents  = ZEPHYRUM;
    redde poll(&pf, I, ZEPHYRUM) > ZEPHYRUM;
}

interior chorda
_causa (
     constans character* praefixum,
                 chorda  causa,
                Piscina* piscina)
{
    redde chorda_concatenare(chorda_ex_literis(praefixum, piscina),
        causa, piscina);
}

/* gradatim: hospes 1.2 -> fossa redintegratur (non-blocans), deinde
 * _connexionem_novam_perficere (SCRIBERE donec connexa) */
interior TlsGradus
_descendere_gradatim (
    TlsConnexio* c)
{
    TcpResultus tr;

    si (c->optiones.sine_descensu)
    {
        c->erratum = chorda_ex_literis("hospes solum TLS 1.2 "
            "(sine_descensu)", c->piscina);
        redde TLS_GRADUS_ERROR;
    }
    c->tcp  = tls_tls13_tcp_relinquere(c->tls13);
    tr      = tcp_redintegrare(c->tcp, NIHIL, c->piscina);
    si (!tr.successus)
    {
        c->erratum = _causa("descensus: redintegratio fallit: ",
            tr.error_descriptio, c->piscina);
        tcp_claudere(c->tcp);
        redde TLS_GRADUS_ERROR;
    }
    c->redintegrans = VERUM;
    redde TLS_GRADUS_SCRIBERE;
}

interior TlsGradus
_connexionem_novam_perficere (
    TlsConnexio* c)
{
    TlsBearsslResultus br;
                chorda descriptio;

    si (!_scribibilis(c->tcp))
    {
        redde TLS_GRADUS_SCRIBERE;
    }
    c->redintegrans     = FALSUM;
    descriptio.datum    = NIHIL;
    descriptio.mensura  = ZEPHYRUM;
    si (tcp_incipere_perficere(c->tcp, &descriptio, c->piscina)
        != TCP_OK)
    {
        c->erratum = _causa("descensus: connexio nova fallit: ",
            descriptio, c->piscina);
        tcp_claudere(c->tcp);
        redde TLS_GRADUS_ERROR;
    }
    br = tls_bearssl_incipere_signo(c->tcp, c->hospes, &c->optiones,
        FALSUM, c->piscina);
    si (!br.successus)
    {
        c->erratum = br.error_descriptio;   /* tcp a BearSSL clausa */
        redde TLS_GRADUS_ERROR;
    }
    c->bearssl = br.connexio;
    c->vectura = TLS_VECTURA_BEARSSL;
    redde tls_bearssl_gradus(c->bearssl);
}

/* obstruens: hospes 1.2 -> fossa redintegratur cum moris, BearSSL cum
 * signo usque ad finem */
interior TlsResultus
_descendere_obstruens (
           TlsTls13Resultus  t3,
         constans character* hospes,
       constans TlsOptiones* o,
                    Piscina* piscina)
{
    TcpConnexio* tcp  = tls_tls13_tcp_relinquere(t3.connexio);
    TcpOptiones  to   = tcp_optiones_default();
    TcpResultus  tr;

    si (o->sine_descensu)
    {
        tcp_claudere(tcp);
        redde _error(TLS_ERROR_HANDSHAKE,
            "hospes solum TLS 1.2 (sine_descensu)", piscina);
    }
    si (o->timeout_ms > ZEPHYRUM)
    {
        to.timeout_ms = o->timeout_ms;
    }
    tr = tcp_redintegrare(tcp, &to, piscina);
    si (!tr.successus)
    {
        TlsResultus r = _error(TLS_ERROR_TCP, "", piscina);

        r.error_descriptio = _causa("descensus: redintegratio fallit: ",
            tr.error_descriptio, piscina);
        tcp_claudere(tcp);
        redde r;
    }
    redde _involvere_bearssl(tls_bearssl_incipere_signo(tcp, hospes, o,
        VERUM, piscina), piscina);
}

TlsResultus
tls_connectere (
    constans character* hospes,
                   i32  portus,
               Piscina* piscina)
{
    TlsOptiones opt = tls_optiones_default();

    redde tls_connectere_cum_optionibus(hospes, portus, &opt, piscina);
}

TlsResultus
tls_connectere_cum_optionibus (
      constans character* hospes,
                     i32  portus,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
    TlsVectura v = tls_vecturam_eligere(optiones);

    si (v == TLS_VECTURA_SECURETRANSPORT)
    {
        redde _involvere_securetransport(tls_securetransport_connectere(
            hospes, portus, optiones, piscina), piscina);
    }
    si (v == TLS_VECTURA_TLS13)
    {
             TlsOptiones o;
        TlsTls13Resultus t3;

        o = optiones ? *optiones : tls_optiones_default();
        si (o.descensus_coactus)
        {
            /* probationes (P5): statim BearSSL cum signo */
            TcpOptiones to = tcp_optiones_default();
            TcpResultus tr;

            si (o.timeout_ms > ZEPHYRUM)
            {
                to.timeout_ms = o.timeout_ms;
            }
            tr = tcp_connectere_cum_optionibus(hospes, portus, &to,
                piscina);
            si (!tr.successus)
            {
                TlsResultus r = _error(TLS_ERROR_TCP, "", piscina);

                r.error_descriptio = tr.error_descriptio;
                redde r;
            }
            redde _involvere_bearssl(tls_bearssl_incipere_signo(
                tr.connexio, hospes, &o, VERUM, piscina), piscina);
        }
        t3 = tls_tls13_connectere(hospes, portus, optiones, piscina);
        si (!t3.successus && t3.solum_1_2)
        {
            redde _descendere_obstruens(t3, hospes, &o, piscina);
        }
        redde _involvere_tls13(t3, piscina);
    }
    redde _involvere_bearssl(tls_bearssl_connectere(hospes, portus,
        optiones, piscina), piscina);
}

TlsResultus
tls_incipere (
             TcpConnexio* tcp,
      constans character* hospes,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
    TlsVectura v = tls_vecturam_eligere(optiones);

    si (v == TLS_VECTURA_SECURETRANSPORT)
    {
        redde _involvere_securetransport(tls_securetransport_incipere(
            tcp, hospes, optiones, piscina), piscina);
    }
    si (v == TLS_VECTURA_TLS13)
    {
        TlsResultus r;
        TlsOptiones o;

        si (!_optiones_copiare(optiones, &o, piscina))
        {
            tcp_claudere(tcp);
            redde _error(TLS_ERROR_TCP, "Allocatio fallita", piscina);
        }
        si (o.descensus_coactus)
        {
            redde _involvere_bearssl(tls_bearssl_incipere_signo(tcp,
                hospes, &o, FALSUM, piscina), piscina);
        }
        r = _involvere_tls13(tls_tls13_incipere(tcp, hospes, &o,
            piscina), piscina);
        si (r.successus)
        {
            memoriae_index  n = strlen(hospes) + I;
                 character* h = (character*)piscina_allocare(piscina,
                     (i64)n);

            si (h == NIHIL)
            {
                tls_claudere(r.connexio);
                redde _error(TLS_ERROR_TCP, "Allocatio fallita",
                    piscina);
            }
            memcpy(h, hospes, n);
            r.connexio->tcp       = tcp;
            r.connexio->hospes    = h;
            r.connexio->optiones  = o;
            r.connexio->piscina   = piscina;
        }
        redde r;
    }
    redde _involvere_bearssl(tls_bearssl_incipere(tcp, hospes, optiones,
        piscina), piscina);
}

TlsVectura
tls_vectura (
    TlsConnexio* connexio)
{
    redde connexio ? connexio->vectura : TLS_VECTURA_ORDINARIA;
}

i32
tls_versio (
    TlsConnexio* connexio)
{
    si (connexio == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (connexio->vectura == TLS_VECTURA_BEARSSL)
    {
        redde tls_bearssl_versio(connexio->bearssl);
    }
    si (connexio->vectura == TLS_VECTURA_TLS13)
    {
        redde tls_tls13_versio(connexio->tls13);
    }
    redde tls_securetransport_versio(connexio->securetransport);
}

TlsGradus
tls_gradus (
    TlsConnexio* connexio)
{
    si (connexio == NIHIL)
    {
        redde TLS_GRADUS_ERROR;
    }
    si (connexio->vectura == TLS_VECTURA_BEARSSL)
    {
        redde tls_bearssl_gradus(connexio->bearssl);
    }
    si (connexio->vectura == TLS_VECTURA_TLS13)
    {
        TlsGradus g;

        si (connexio->erratum.mensura > ZEPHYRUM)
        {
            redde TLS_GRADUS_ERROR;
        }
        si (connexio->redintegrans)
        {
            redde _connexionem_novam_perficere(connexio);
        }
        g = tls_tls13_gradus(connexio->tls13);
        si (   g == TLS_GRADUS_ERROR
            && tls_tls13_solum_1_2(connexio->tls13))
        {
            redde _descendere_gradatim(connexio);
        }
        redde g;
    }
    redde tls_securetransport_gradus(connexio->securetransport);
}

chorda
tls_gradus_error (
    TlsConnexio* connexio)
{
    chorda vacua;

    si (connexio == NIHIL)
    {
        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde vacua;
    }
    si (connexio->erratum.mensura > ZEPHYRUM)
    {
        redde connexio->erratum;
    }
    si (connexio->vectura == TLS_VECTURA_BEARSSL)
    {
        redde tls_bearssl_gradus_error(connexio->bearssl);
    }
    si (connexio->vectura == TLS_VECTURA_TLS13)
    {
        redde tls_tls13_gradus_error(connexio->tls13);
    }
    redde tls_securetransport_gradus_error(connexio->securetransport);
}

s32
tls_mittere (
    TlsConnexio* connexio,
    constans i8* data,
            i32  mensura)
{
    si (connexio == NIHIL)
    {
        redde -I;
    }
    si (connexio->vectura == TLS_VECTURA_BEARSSL)
    {
        redde tls_bearssl_mittere(connexio->bearssl, data, mensura);
    }
    si (connexio->vectura == TLS_VECTURA_TLS13)
    {
        redde tls_tls13_mittere(connexio->tls13, data, mensura);
    }
    redde tls_securetransport_mittere(connexio->securetransport, data,
        mensura);
}

/* obstruens usque omnia missa: 0 aut TLS_ITERUM -> mora 1 ms et iterum
 * (ut olim in tls_macos.m); error -> FALSUM */
b32
tls_mittere_omnia (
    TlsConnexio* connexio,
    constans i8* data,
            i32  mensura)
{
    i32 totalis = ZEPHYRUM;

    si (connexio == NIHIL || data == NIHIL || mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    dum (totalis < mensura)
    {
        s32 n = tls_mittere(connexio, data + totalis, mensura
            - totalis);

        si (n < ZEPHYRUM && n != TLS_ITERUM)
        {
            redde FALSUM;
        }
        si (n <= ZEPHYRUM)
        {
            (vacuum)usleep(M);
            perge;
        }
        totalis += (i32)n;
    }
    redde VERUM;
}

s32
tls_recipere (
    TlsConnexio* connexio,
             i8* buffer,
            i32  capacitas)
{
    si (connexio == NIHIL)
    {
        redde -I;
    }
    si (connexio->vectura == TLS_VECTURA_BEARSSL)
    {
        redde tls_bearssl_recipere(connexio->bearssl, buffer,
            capacitas);
    }
    si (connexio->vectura == TLS_VECTURA_TLS13)
    {
        redde tls_tls13_recipere(connexio->tls13, buffer, capacitas);
    }
    redde tls_securetransport_recipere(connexio->securetransport,
        buffer,
        capacitas);
}

vacuum
tls_claudere (
    TlsConnexio* connexio)
{
    si (connexio == NIHIL)
    {
        redde;
    }
    si (connexio->vectura == TLS_VECTURA_BEARSSL)
    {
        tls_bearssl_claudere(connexio->bearssl);
        redde;
    }
    si (connexio->vectura == TLS_VECTURA_TLS13)
    {
        /* redintegrans: fossa iam a tls.c possessa */
        si (connexio->redintegrans)
        {
            tcp_claudere(connexio->tcp);
            connexio->redintegrans = FALSUM;
        }
        tls_tls13_claudere(connexio->tls13);
        redde;
    }
    tls_securetransport_claudere(connexio->securetransport);
}

b32
tls_est_valida (
    TlsConnexio* connexio)
{
    si (connexio == NIHIL)
    {
        redde FALSUM;
    }
    si (connexio->vectura == TLS_VECTURA_BEARSSL)
    {
        redde tls_bearssl_est_valida(connexio->bearssl);
    }
    si (connexio->vectura == TLS_VECTURA_TLS13)
    {
        redde tls_tls13_est_valida(connexio->tls13);
    }
    redde tls_securetransport_est_valida(connexio->securetransport);
}

b32
tls_certificatum_obtinere (
        TlsConnexio* connexio,
    TlsCertificatum* exitus)
{
    si (connexio == NIHIL)
    {
        si (exitus)
        {
            memset(exitus, ZEPHYRUM, magnitudo(*exitus));
        }
        redde FALSUM;
    }
    si (connexio->vectura == TLS_VECTURA_BEARSSL)
    {
        redde tls_bearssl_certificatum(connexio->bearssl, exitus);
    }
    si (connexio->vectura == TLS_VECTURA_TLS13)
    {
        redde tls_tls13_certificatum(connexio->tls13, exitus);
    }
    redde tls_securetransport_certificatum(connexio->securetransport,
        exitus);
}

constans character*
tls_error_descriptio (
    TlsError error)
{
    commutatio (error)
    {
        casus TLS_OK:                  redde "OK";
        casus TLS_ERROR_TCP:           redde "Connexio TCP fallita";
        casus TLS_ERROR_HANDSHAKE:     redde "TLS handshake fallita";
        casus TLS_ERROR_CERTIFICATUM:  redde "Certificatum invalidum";
        casus TLS_ERROR_IO:            redde "I/O error";
        casus TLS_ERROR_CLAUSUM:       redde "Connexio clausa";
        ordinarius:                    redde "Error ignotus";
    }
}
