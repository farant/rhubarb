/* tls.c - tls.h: DISPATCHER vecturarum (tls-bearssl-spec II.5). Vectura
 * per connexionem eligitur (TlsOptiones.vectura; ORDINARIA ->
 * RHUBARB_TLS; aliter ordinaria domus) et in connexione servatur.
 * Vecturae: tls_securetransport_macos.m; tls_bearssl.c (T4). Ambitus
 * per lectiones_ambitus legitur (fabrica dependentiam videt). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tcp.h"
#include "tls.h"
#include "tls_securetransport.h"
#include "lectiones.h"
#include <string.h>
#include <unistd.h>

/* ordinaria domus (T6: TLS_VECTURA_BEARSSL) */
#define TLS_VECTURA_DOMUS TLS_VECTURA_SECURETRANSPORT

structura TlsConnexio {
            TlsVectura  vectura;
    TlsSecuretransport* securetransport;
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
    si (tls_vecturam_eligere(optiones) == TLS_VECTURA_SECURETRANSPORT)
    {
        redde _involvere_securetransport(tls_securetransport_connectere(
            hospes, portus, optiones, piscina), piscina);
    }
    redde _error(TLS_ERROR_HANDSHAKE, "vectura bearssl nondum",
        piscina);
}

TlsResultus
tls_incipere (
             TcpConnexio* tcp,
      constans character* hospes,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
    si (tls_vecturam_eligere(optiones) == TLS_VECTURA_SECURETRANSPORT)
    {
        redde _involvere_securetransport(tls_securetransport_incipere(
            tcp, hospes, optiones, piscina), piscina);
    }
    si (tcp)
    {
        tcp_claudere(tcp);
    }
    redde _error(TLS_ERROR_HANDSHAKE, "vectura bearssl nondum",
        piscina);
}

TlsVectura
tls_vectura (
    TlsConnexio* connexio)
{
    redde connexio ? connexio->vectura : TLS_VECTURA_ORDINARIA;
}

TlsGradus
tls_gradus (
    TlsConnexio* connexio)
{
    si (connexio == NIHIL)
    {
        redde TLS_GRADUS_ERROR;
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
    tls_securetransport_claudere(connexio->securetransport);
}

b32
tls_est_valida (
    TlsConnexio* connexio)
{
    redde connexio != NIHIL
        && tls_securetransport_est_valida(connexio->securetransport);
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
