/* tcp.h - Bibliotheca TCP (Connexiones Rete)
 *
 * API platform-agnostica pro connexionibus TCP.
 * Implementatio: tcp_posix.c (macOS, Linux)
 *
 * USUS:
 *   TcpResultus res = tcp_connectere("httpbin.org", 80, piscina);
 *   si (res.successus) {
 *       tcp_mittere(res.connexio, data, mensura);
 *       i32 n = tcp_recipere(res.connexio, buffer, capacitas);
 *       tcp_claudere(res.connexio);
 *   }
 */

#ifndef TCP_H
#define TCP_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Opaque handle - implementatio in tcp_posix.c */
nomen structura TcpConnexio TcpConnexio;

/* Error codes */
nomen enumeratio {
    TCP_OK = 0,
    TCP_ERROR_DNS,           /* DNS resolutio fallita */
    TCP_ERROR_CONNEXIO,      /* Connexio fallita */
    TCP_ERROR_TIMEOUT,       /* Operatio timeout */
    TCP_ERROR_CLAUSUM,       /* Connexio clausa ab hospite */
    TCP_ERROR_IO             /* I/O error */
} TcpError;

/* Resultus connexionis */
nomen structura {
    b32          successus;
    TcpConnexio* connexio;
    TcpError     error;
    chorda       error_descriptio;
} TcpResultus;

/* Optiones */
nomen structura {
    i32 timeout_ms;          /* Timeout in milliseconds (0 = infinitus) */
    b32 nodelay;             /* TCP_NODELAY (disable Nagle) */
    b32 keepalive;           /* SO_KEEPALIVE */
} TcpOptiones;


/* ========================================================================
 * FUNCTIONES - CONNEXIO
 * ======================================================================== */

/* Connectere ad hospes:portus
 *
 * hospes:  Nomen DNS vel IP address
 * portus:  Numerus portus (e.g., 80, 443)
 * piscina: Arena pro allocationibus
 *
 * Redde: Resultus cum connexio vel error
 */
TcpResultus
tcp_connectere(
    constans character* hospes,
    i32                 portus,
    Piscina*            piscina);

/* Connectere cum optionibus */
TcpResultus
tcp_connectere_cum_optionibus(
    constans character*   hospes,
    i32                   portus,
    constans TcpOptiones* optiones,
    Piscina*              piscina);

/* Optiones default */
TcpOptiones tcp_optiones_default(vacuum);


/* ========================================================================
 * FUNCTIONES - I/O
 * ======================================================================== */

/* Mittere data
 *
 * Redde: Numerus bytes missi, vel -1 si error
 */
i32
tcp_mittere(
    TcpConnexio*   connexio,
    constans i8*   data,
    i32            mensura);

/* Mittere omnia data (blocking usque omnia missa)
 *
 * Redde: VERUM si omnia missa, FALSUM si error
 */
b32
tcp_mittere_omnia(
    TcpConnexio*   connexio,
    constans i8*   data,
    i32            mensura);

/* Recipere data
 *
 * Redde: Numerus bytes recepti, 0 si connexio clausa, -1 si error
 */
i32
tcp_recipere(
    TcpConnexio* connexio,
    i8*          buffer,
    i32          capacitas);

/* Claudere connexionem */
vacuum
tcp_claudere(TcpConnexio* connexio);


/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */

/* Obtinere file descriptor (pro select/poll) */
i32 tcp_obtinere_fd(TcpConnexio* connexio);

/* Verificare si connexio valida */
b32 tcp_est_valida(TcpConnexio* connexio);

/* Obtinere error descriptio */
constans character* tcp_error_descriptio(TcpError error);


#endif /* TCP_H */
