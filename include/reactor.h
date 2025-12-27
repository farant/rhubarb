/* reactor.h - Bibliotheca Reactor (Event Loop)
 *
 * Single-threaded event loop usando poll() pro I/O multiplexing.
 * Permittit handling multarum connexionum in uno thread.
 *
 * USUS:
 *   Reactor* reactor = reactor_creare(piscina);
 *
 *   reactor_adicere(reactor, server_fd, REACTOR_LEGERE, on_accept, data);
 *   reactor_adicere(reactor, client_fd, REACTOR_LEGERE, on_read, data);
 *
 *   reactor_currere(reactor);  // Currit usque reactor_sistere()
 */

#ifndef REACTOR_H
#define REACTOR_H

#include "latina.h"
#include "piscina.h"

/* ========================================================================
 * CONSTANTES
 * ======================================================================== */

/* Event types (bitmask) */
#define REACTOR_LEGERE    0x01u  /* Paratus ad legendum */
#define REACTOR_SCRIBERE  0x02u  /* Paratus ad scribendum */
#define REACTOR_ERROR     0x04u  /* Error conditio */
#define REACTOR_CLAUSUM   0x08u  /* Connexio clausa */

/* Limites */
#define REACTOR_FD_MAXIMA       M        /* 1000 FDs maximum */
#define REACTOR_TIMER_MAXIMA    CXXVIII  /* 128 timers maximum */


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Opaque reactor handle */
nomen structura Reactor Reactor;

/* Timer ID */
nomen i32 ReactorTimerId;

/* Error value pro timer ID */
#define REACTOR_TIMER_ERROR  0xFFFFFFFFu

/* Callback pro FD events
 *
 * fd:      File descriptor
 * eventus: Bitmask of events (REACTOR_LEGERE, etc.)
 * data:    User data
 */
nomen vacuum (*ReactorCallback)(integer fd, i32 eventus, vacuum* data);

/* Callback pro timer events
 *
 * data: User data
 */
nomen vacuum (*ReactorTimerCallback)(vacuum* data);


/* ========================================================================
 * FUNCTIONES - LIFECYCLE
 * ======================================================================== */

/* Creare novum reactor
 *
 * piscina: Arena pro allocationibus
 *
 * Redde: Reactor vel NIHIL si error
 */
Reactor*
reactor_creare(Piscina* piscina);

/* Destruere reactor */
vacuum
reactor_destruere(Reactor* reactor);


/* ========================================================================
 * FUNCTIONES - FD MANAGEMENT
 * ======================================================================== */

/* Adicere file descriptor ad reactor
 *
 * reactor:  Reactor
 * fd:       File descriptor
 * eventus:  Events ad monitorandum (REACTOR_LEGERE | REACTOR_SCRIBERE)
 * callback: Functio vocanda quando eventus occurrit
 * data:     User data pro callback
 *
 * Redde: VERUM si successus
 */
b32
reactor_adicere(
    Reactor*        reactor,
    integer         fd,
    i32             eventus,
    ReactorCallback callback,
    vacuum*         data);

/* Modificare events pro FD
 *
 * Redde: VERUM si successus
 */
b32
reactor_modificare(
    Reactor* reactor,
    integer  fd,
    i32      eventus);

/* Removere FD ex reactor
 *
 * Redde: VERUM si successus
 */
b32
reactor_removere(
    Reactor* reactor,
    integer  fd);


/* ========================================================================
 * FUNCTIONES - TIMER MANAGEMENT
 * ======================================================================== */

/* Adicere timer
 *
 * reactor:    Reactor
 * timeout_ms: Timeout in milliseconds
 * repetere:   VERUM si timer debet repetere
 * callback:   Functio vocanda quando timer expirat
 * data:       User data pro callback
 *
 * Redde: Timer ID (>= 0) vel -1 si error
 */
ReactorTimerId
reactor_timer_adicere(
    Reactor*             reactor,
    i32                  timeout_ms,
    b32                  repetere,
    ReactorTimerCallback callback,
    vacuum*              data);

/* Cancellare timer
 *
 * Redde: VERUM si successus
 */
b32
reactor_timer_cancellare(
    Reactor*       reactor,
    ReactorTimerId id);


/* ========================================================================
 * FUNCTIONES - EVENT LOOP
 * ======================================================================== */

/* Poll una vice
 *
 * timeout_ms: Maximum tempus expectandi (-1 = infinitum, 0 = non-blocking)
 *
 * Redde: Numerus events processatorum
 */
i32
reactor_poll(
    Reactor* reactor,
    i32      timeout_ms);

/* Currere event loop usque reactor_sistere()
 *
 * Vocat reactor_poll() in loop infinito.
 */
vacuum
reactor_currere(Reactor* reactor);

/* Petere ut event loop sistat
 *
 * Event loop finietur post currentem poll() iteration.
 */
vacuum
reactor_sistere(Reactor* reactor);

/* Verificare si reactor currens */
b32
reactor_est_currens(Reactor* reactor);

/* Obtinere numerum FDs registratorum */
i32
reactor_numerus_fd(Reactor* reactor);

/* Obtinere numerum timers activorum */
i32
reactor_numerus_timer(Reactor* reactor);


#endif /* REACTOR_H */
