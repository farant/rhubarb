/* tls_securetransport.h - VECTURA SecureTransport, INTERNA: solum
 * lib/tls.c includit (tls-bearssl-spec II.5, regula R8). Implementatio:
 * tls_securetransport_macos.m. Contractus quisque = functio tls.h
 * eiusdem nominis sine 'securetransport'. */
#ifndef TLS_SECURETRANSPORT_H
#define TLS_SECURETRANSPORT_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tcp.h"
#include "tls.h"

nomen structura TlsSecuretransport TlsSecuretransport;

nomen structura {
                   b32  successus;
    TlsSecuretransport* connexio;
              TlsError  error;
                chorda  error_descriptio;
} TlsSecuretransportResultus;

TlsSecuretransportResultus
tls_securetransport_connectere (
      constans character* hospes,
                     i32  portus,
    constans TlsOptiones* optiones,
                 Piscina* piscina);

TlsSecuretransportResultus
tls_securetransport_incipere (
             TcpConnexio* tcp,
      constans character* hospes,
    constans TlsOptiones* optiones,
                 Piscina* piscina);

TlsGradus
tls_securetransport_gradus (
    TlsSecuretransport* connexio);

chorda
tls_securetransport_gradus_error (
    TlsSecuretransport* connexio);

s32
tls_securetransport_mittere (
    TlsSecuretransport* connexio,
           constans i8* data,
                   i32  mensura);

s32
tls_securetransport_recipere (
    TlsSecuretransport* connexio,
                    i8* buffer,
                   i32  capacitas);

vacuum
tls_securetransport_claudere (
    TlsSecuretransport* connexio);

b32
tls_securetransport_est_valida (
    TlsSecuretransport* connexio);

b32
tls_securetransport_certificatum (
    TlsSecuretransport* connexio,
       TlsCertificatum* exitus);

#endif /* TLS_SECURETRANSPORT_H */
