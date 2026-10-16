/* tls_tls13.h - VECTURA TLS 1.3 propria, INTERNA: solum lib/tls.c
 * includit (tls13-spec II.1; forma tls_bearssl.h ad litteram).
 * Machina tls13.c (sine fossis) per antliam super TcpConnexio; fiducia
 * per fiducia.h. Contractus quisque = functio tls.h eiusdem nominis
 * sine 'tls13'. */
#ifndef TLS_TLS13_H
#define TLS_TLS13_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tcp.h"
#include "tls.h"

nomen structura TlsTls13 TlsTls13;

nomen structura {
         b32  successus;
    TlsTls13* connexio;
    TlsError  error;
      chorda  error_descriptio;
} TlsTls13Resultus;

/* obstruens: tcp cum moris (timeout_ms), pactio usque ad finem */
TlsTls13Resultus
tls_tls13_connectere (
      constans character* hospes,
                     i32  portus,
    constans TlsOptiones* optiones,
                 Piscina* piscina);

TlsTls13Resultus
tls_tls13_incipere (
             TcpConnexio* tcp,
      constans character* hospes,
    constans TlsOptiones* optiones,
                 Piscina* piscina);

TlsGradus
tls_tls13_gradus (
    TlsTls13* connexio);

/* causa erroris pactionis AUT lectionis/scriptionis post pactionem
 * (e.g. alerta a pare nominata) */
chorda
tls_tls13_gradus_error (
    TlsTls13* connexio);

s32
tls_tls13_mittere (
       TlsTls13* connexio,
    constans i8* data,
            i32  mensura);

s32
tls_tls13_recipere (
    TlsTls13* connexio,
          i8* buffer,
         i32  capacitas);

vacuum
tls_tls13_claudere (
    TlsTls13* connexio);

b32
tls_tls13_est_valida (
    TlsTls13* connexio);

/* folium catenae machinae -> fiducia_folium_describere */
b32
tls_tls13_certificatum (
           TlsTls13* connexio,
    TlsCertificatum* exitus);

#endif /* TLS_TLS13_H */
