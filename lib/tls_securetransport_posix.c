/* tls_securetransport_posix.c - vectura SecureTransport sub POSIX:
 * NON EXSTAT.
 *
 * Variantes 'posix' tls_securetransport_macos.m: SecureTransport est
 * Apple solum. Dispensator tls.c eam solum vocat cum
 * RHUBARB_TLS=securetransport petitur; hic omnis vocatio deficit cum
 * causa nominata, ut sub Linux vecturae domus (tls13, bearssl)
 * solae maneant (hospitium v3 T9). */
#include "tls_securetransport.h"

interior TlsSecuretransportResultus
_deest (
    Piscina* piscina)
{
    TlsSecuretransportResultus r;

    r.successus  = FALSUM;
    r.connexio   = NIHIL;
    r.error      = TLS_ERROR_HANDSHAKE;
    r.error_descriptio = chorda_ex_literis(
        "SecureTransport solum sub macOS (RHUBARB_TLS=securetransport)",
        piscina);
    redde r;
}

TlsSecuretransportResultus
tls_securetransport_connectere (
      constans character* hospes,
                     i32  portus,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
    (vacuum)hospes;
    (vacuum)portus;
    (vacuum)optiones;
    redde _deest(piscina);
}

TlsSecuretransportResultus
tls_securetransport_incipere (
             TcpConnexio* tcp,
      constans character* hospes,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
    (vacuum)tcp;
    (vacuum)hospes;
    (vacuum)optiones;
    redde _deest(piscina);
}

TlsGradus
tls_securetransport_gradus (
    TlsSecuretransport* connexio)
{
    (vacuum)connexio;
    redde TLS_GRADUS_ERROR;
}

chorda
tls_securetransport_gradus_error (
    TlsSecuretransport* connexio)
{
    chorda c;

    (vacuum)connexio;
    c.datum    = NIHIL;
    c.mensura  = 0;
    redde c;
}

s32
tls_securetransport_mittere (
    TlsSecuretransport* connexio,
           constans i8* data,
                   i32  mensura)
{
    (vacuum)connexio;
    (vacuum)data;
    (vacuum)mensura;
    redde -I;
}

s32
tls_securetransport_recipere (
    TlsSecuretransport* connexio,
                    i8* buffer,
                   i32  capacitas)
{
    (vacuum)connexio;
    (vacuum)buffer;
    (vacuum)capacitas;
    redde -I;
}

vacuum
tls_securetransport_claudere (
    TlsSecuretransport* connexio)
{
    (vacuum)connexio;
}

b32
tls_securetransport_est_valida (
    TlsSecuretransport* connexio)
{
    (vacuum)connexio;
    redde FALSUM;
}

i32
tls_securetransport_versio (
    TlsSecuretransport* connexio)
{
    (vacuum)connexio;
    redde 0;
}

b32
tls_securetransport_certificatum (
    TlsSecuretransport* connexio,
       TlsCertificatum* exitus)
{
    (vacuum)connexio;
    (vacuum)exitus;
    redde FALSUM;
}
