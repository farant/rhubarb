/* tls_bearssl.h - VECTURA BearSSL, INTERNA: solum lib/tls.c includit
 * (tls-bearssl-spec II.6, regula R8). Machina TLS 1.2 BearSSL
 * (vendor/bearssl_amalgama.c), fiducia per fiducia.h. Contractus
 * quisque = functio tls.h eiusdem nominis sine 'bearssl'. */
#ifndef TLS_BEARSSL_H
#define TLS_BEARSSL_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tcp.h"
#include "tls.h"

nomen structura TlsBearssl TlsBearssl;

nomen structura {
           b32  successus;
    TlsBearssl* connexio;
      TlsError  error;
        chorda  error_descriptio;
} TlsBearsslResultus;

/* obstruens: tcp cum moris (timeout_ms), pactio usque ad finem (T5) */
TlsBearsslResultus
tls_bearssl_connectere (
      constans character* hospes,
                     i32  portus,
    constans TlsOptiones* optiones,
                 Piscina* piscina);

TlsBearsslResultus
tls_bearssl_incipere (
             TcpConnexio* tcp,
      constans character* hospes,
    constans TlsOptiones* optiones,
                 Piscina* piscina);

TlsGradus
tls_bearssl_gradus (
    TlsBearssl* connexio);

chorda
tls_bearssl_gradus_error (
    TlsBearssl* connexio);

s32
tls_bearssl_mittere (
     TlsBearssl* connexio,
    constans i8* data,
            i32  mensura);

s32
tls_bearssl_recipere (
    TlsBearssl* connexio,
            i8* buffer,
           i32  capacitas);

vacuum
tls_bearssl_claudere (
    TlsBearssl* connexio);

b32
tls_bearssl_est_valida (
    TlsBearssl* connexio);

/* folium catenae collectae -> fiducia_folium_describere (T5) */
b32
tls_bearssl_certificatum (
         TlsBearssl* connexio,
    TlsCertificatum* exitus);

#endif /* TLS_BEARSSL_H */
