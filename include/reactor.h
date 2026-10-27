/* reactor.h - Bibliotheca Reactor (Event Loop)
 *
 * Single-threaded event loop usando poll() pro I/O multiplexing.
 * Permittit handling multarum connexionum in uno thread. Praeterea
 * (hospitium-v3-spec II.1): LABORES cooperativi (opus CPU in gradus
 * secto - gradus unus per cyclum, poll() non exspectat dum labor
 * pendet) et SIGNA ut eventus ansae (fistula sui) - processus unus,
 * ansa una, filum unum.
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
#define REACTOR_LABOR_MAXIMA    LXIV     /* labores simul pendentes */
#define REACTOR_SIGNA_MAXIMA    VIII     /* signa registrata */


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

/* Labor cooperativus: gradus UNUS (~1-2 ms operis) per vocationem.
 * Redde VERUM cum finitum est (labor tollitur), FALSUM ut pergat. */
nomen b32 (*ReactorLabor)(vacuum* datum);

/* Tractator signi - in ansa vocatur (reactor_poll), numquam in
 * contextu signi: omnis functio licet */
nomen vacuum (*ReactorSigniTractator)(vacuum* datum);


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
reactor_creare (
    Piscina* piscina);

/* Destruere reactor */
vacuum
reactor_destruere (
    Reactor* reactor);


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
reactor_adicere (
            Reactor* reactor,
            integer  fd,
                i32  eventus,
    ReactorCallback  callback,
             vacuum* data);

/* Modificare events pro FD
 *
 * Redde: VERUM si successus
 */
b32
reactor_modificare (
    Reactor* reactor,
    integer  fd,
        i32  eventus);

/* Removere FD ex reactor
 *
 * Redde: VERUM si successus
 */
b32
reactor_removere (
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
reactor_timer_adicere (
                 Reactor* reactor,
                     i32  timeout_ms,
                     b32  repetere,
    ReactorTimerCallback  callback,
                  vacuum* data);

/* Cancellare timer
 *
 * Redde: VERUM si successus
 */
b32
reactor_timer_cancellare (
           Reactor* reactor,
    ReactorTimerId  id);


/* ========================================================================
 * FUNCTIONES - LABORES ET SIGNA
 * ======================================================================== */

/* Laborem addere: omni cyclo reactor_poll gradum unum laboris quisque
 * pendentis vocat (post fd et timers); dum labor pendet, poll() cum
 * mora ZEPHYRI vocatur (I/O et labores alternant). Labor in gradu
 * alterius additus cyclo PROXIMO primum currit.
 *
 * Redde: FALSUM si tabula plena (REACTOR_LABOR_MAXIMA) */
b32
reactor_laborem_adicere (
         Reactor* reactor,
    ReactorLabor  labor,
          vacuum* datum);

/* Numerus laborum pendentium */
i32
reactor_numerus_laborum (
    Reactor* reactor);

/* Signum ut eventum ansae: tractator in reactor_poll vocatur postquam
 * signum venit (fistula sui - tractator systematis octetum solum
 * scribit). Plura signa ante poll in vocationes pauciores confluere
 * possunt, numquam perire. Fistula interna in reactor_numerus_fd non
 * numeratur, sed poll() eam exspectat (reactor solum signum habens
 * obstat donec signum venit).
 *
 * Signa processui toti pertinent: reactor UNUS ea possidet - alter
 * FALSUM accipit donec possessor destruitur (reactor_destruere
 * tractatores systematis ordinarios restituit).
 *
 * Redde: FALSUM si reactor alter possidet, tabula plena, aut fistula
 * creari nequit */
b32
reactor_signum_adicere (
                  Reactor* reactor,
                  integer  signum,
    ReactorSigniTractator  tractator,
                   vacuum* datum);


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
reactor_poll (
    Reactor* reactor,
        i32  timeout_ms);

/* Currere event loop usque reactor_sistere()
 *
 * Vocat reactor_poll() in loop infinito.
 */
vacuum
reactor_currere (
    Reactor* reactor);

/* Petere ut event loop sistat
 *
 * Event loop finietur post currentem poll() iteration.
 */
vacuum
reactor_sistere (
    Reactor* reactor);

/* Verificare si reactor currens */
b32
reactor_est_currens (
    Reactor* reactor);

/* Obtinere numerum FDs registratorum */
i32
reactor_numerus_fd (
    Reactor* reactor);

/* Obtinere numerum timers activorum */
i32
reactor_numerus_timer (
    Reactor* reactor);


/* ========================================================================
 * FUNCTIONES - HOSPES CUM ANSA SUA (vicus quies T4)
 *
 * Hospes qui ansam suam habet (fenestra) reactorem non currit sed
 * poscit (reactor_poll cum mora ZEPHYRI). Ut otiosus dormiat, scire
 * debet QUANDO reactor iterum poscendus sit et QUAE fossae eum
 * expergefacere debeant.
 * ======================================================================== */

/* Mora usque ad opus proximum (ms):
 *   0  si labor pendet aut timer iam debitus;
 *   n  ms usque ad timer proximum;
 *  -1  si nec timer nec labor (solae fossae expergefaciunt).
 * Hospes exspectat minimum huius et morarum suarum. */
s32
reactor_mora_proxima (
    constans Reactor* reactor);

/* Visitator fossae: fossa activa cum eventibus petitis
 * (REACTOR_LEGERE | REACTOR_SCRIBERE) */
nomen vacuum (*ReactorFossaVisitor)(
     vacuum* ctx,
    integer  fd,
        i32  eventus);

/* Fossas activas omnes visitare - etiam fistulam signorum internam
 * (signum venit -> fistula legenda -> hospes expergiscitur), quamquam
 * reactor_numerus_fd eam non numerat. Hospes eas ansae suae addit
 * (macOS: CFFileDescriptor). */
vacuum
reactor_fossas_visitare (
       constans Reactor* reactor,
    ReactorFossaVisitor  visitor,
                 vacuum* ctx);

/* Versio fossarum: crescit omni adiectione, modificatione, remotione
 * successa (non poll, non timers, non labores). Versio eadem = fossae
 * eaedem cum eisdem eventibus - hospes solum tunc renovat cum
 * mutatur. */
i32
reactor_versio_fossarum (
    constans Reactor* reactor);


#endif /* REACTOR_H */
