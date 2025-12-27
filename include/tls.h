/* tls.h - Bibliotheca TLS (Transport Layer Security)
 *
 * API platform-agnostica pro connexionibus securis.
 * Implementatio: tls_macos.m (macOS Security.framework)
 *
 * USUS:
 *   TlsResultus res = tls_connectere("httpbin.org", 443, piscina);
 *   si (res.successus) {
 *       tls_mittere(res.connexio, data, mensura);
 *       i32 n = tls_recipere(res.connexio, buffer, capacitas);
 *       tls_claudere(res.connexio);
 *   }
 */

#ifndef TLS_H
#define TLS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Opaque handle - implementatio in tls_macos.m */
nomen structura TlsConnexio TlsConnexio;

/* Error codes */
nomen enumeratio {
    TLS_OK = 0,
    TLS_ERROR_TCP,           /* Connexio TCP fallita */
    TLS_ERROR_HANDSHAKE,     /* TLS handshake fallita */
    TLS_ERROR_CERTIFICATUM,  /* Certificatum invalidum */
    TLS_ERROR_IO,            /* I/O error */
    TLS_ERROR_CLAUSUM        /* Connexio clausa */
} TlsError;

/* Resultus connexionis */
nomen structura {
    b32          successus;
    TlsConnexio* connexio;
    TlsError     error;
    chorda       error_descriptio;
} TlsResultus;

/* Optiones */
nomen structura {
    i32 timeout_ms;              /* Timeout in milliseconds (0 = infinitus) */
    b32 verificare_certificatum; /* Verificare certificatum hospitis? */
} TlsOptiones;


/* ========================================================================
 * FUNCTIONES - CONNEXIO
 * ======================================================================== */

/* Connectere ad hospes:portus cum TLS
 *
 * hospes:  Nomen DNS (requiritur pro SNI)
 * portus:  Numerus portus (typice 443)
 * piscina: Arena pro allocationibus
 *
 * Redde: Resultus cum connexio vel error
 */
TlsResultus
tls_connectere(
    constans character* hospes,
    i32                 portus,
    Piscina*            piscina);

/* Connectere cum optionibus */
TlsResultus
tls_connectere_cum_optionibus(
    constans character*  hospes,
    i32                  portus,
    constans TlsOptiones* optiones,
    Piscina*             piscina);

/* Optiones default */
TlsOptiones tls_optiones_default(vacuum);


/* ========================================================================
 * FUNCTIONES - I/O
 * ======================================================================== */

/* Mittere data (encrypted)
 *
 * Redde: Numerus bytes missi, vel -1 si error
 */
i32
tls_mittere(
    TlsConnexio*   connexio,
    constans i8*   data,
    i32            mensura);

/* Mittere omnia data (blocking usque omnia missa)
 *
 * Redde: VERUM si omnia missa, FALSUM si error
 */
b32
tls_mittere_omnia(
    TlsConnexio*   connexio,
    constans i8*   data,
    i32            mensura);

/* Recipere data (decrypted)
 *
 * Redde: Numerus bytes recepti, 0 si connexio clausa, -1 si error
 */
i32
tls_recipere(
    TlsConnexio* connexio,
    i8*          buffer,
    i32          capacitas);

/* Claudere connexionem */
vacuum
tls_claudere(TlsConnexio* connexio);


/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */

/* Verificare si connexio valida */
b32 tls_est_valida(TlsConnexio* connexio);

/* Obtinere error descriptio */
constans character* tls_error_descriptio(TlsError error);


#endif /* TLS_H */
