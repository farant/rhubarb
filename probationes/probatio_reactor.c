/* probatio_reactor.c - Probationes pro Reactor (Event Loop)
 *
 * Probationes functionum reactor: FD monitoring, timers, event loop.
 */

#include "reactor.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


/* ========================================================================
 * VARIABILES PRO PROBATIONIBUS
 * ======================================================================== */

hic_manens i32 g_callback_vocatus = 0;
hic_manens i32 g_eventus_receptus = 0;
hic_manens i32 g_fd_receptus = 0;
hic_manens i32 g_timer_vocatus = 0;
hic_manens Reactor* g_reactor_ref = NIHIL;


/* ========================================================================
 * CALLBACKS PRO PROBATIONIBUS
 * ======================================================================== */

interior vacuum
callback_simplex(integer fd, i32 eventus, vacuum* data)
{
    (vacuum)data;
    g_callback_vocatus++;
    g_eventus_receptus = eventus;
    g_fd_receptus = (i32)fd;
}

interior vacuum
callback_timer(vacuum* data)
{
    i32* counter = (i32*)data;
    si (counter)
    {
        (*counter)++;
    }
    g_timer_vocatus++;
}

interior vacuum
callback_sistere(vacuum* data)
{
    Reactor* reactor = (Reactor*)data;
    g_timer_vocatus++;
    si (reactor)
    {
        reactor_sistere(reactor);
    }
}

interior vacuum
callback_fd_clausum(integer fd, i32 eventus, vacuum* data)
{
    (vacuum)fd;
    (vacuum)data;
    g_callback_vocatus++;
    g_eventus_receptus = eventus;
}


/* ========================================================================
 * FUNCTIONES AUXILIARES
 * ======================================================================== */

interior vacuum
reset_globals(vacuum)
{
    g_callback_vocatus = 0;
    g_eventus_receptus = 0;
    g_fd_receptus = 0;
    g_timer_vocatus = 0;
    g_reactor_ref = NIHIL;
}


/* ========================================================================
 * PROBATIONES - LIFECYCLE
 * ======================================================================== */

interior vacuum
probatio_creare_destruere(Piscina* piscina)
{
    Reactor* reactor;

    printf("--- Probans creare et destruere ---\n");

    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_FALSUM(reactor_est_currens(reactor));
    CREDO_VERUM(reactor_numerus_fd(reactor) == 0);
    CREDO_VERUM(reactor_numerus_timer(reactor) == 0);

    reactor_destruere(reactor);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - FD MANAGEMENT
 * ======================================================================== */

interior vacuum
probatio_adicere_fd(Piscina* piscina)
{
    Reactor* reactor;
    integer pipe_fds[II];
    b32 res;

    printf("--- Probans adicere FD ---\n");

    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    /* Creare pipe pro testing */
    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere read end */
    res = reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                          callback_simplex, NIHIL);
    CREDO_VERUM(res);
    CREDO_VERUM(reactor_numerus_fd(reactor) == I);

    /* Adicere write end */
    res = reactor_adicere(reactor, pipe_fds[I], REACTOR_SCRIBERE,
                          callback_simplex, NIHIL);
    CREDO_VERUM(res);
    CREDO_VERUM(reactor_numerus_fd(reactor) == II);

    close(pipe_fds[0]);
    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
probatio_modificare_fd(Piscina* piscina)
{
    Reactor* reactor;
    integer pipe_fds[II];
    b32 res;

    printf("--- Probans modificare FD ---\n");

    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere cum LEGERE */
    res = reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                          callback_simplex, NIHIL);
    CREDO_VERUM(res);

    /* Modificare ad SCRIBERE */
    res = reactor_modificare(reactor, pipe_fds[0], REACTOR_SCRIBERE);
    CREDO_VERUM(res);

    /* Modificare FD non existentem */
    res = reactor_modificare(reactor, CXXVIII, REACTOR_LEGERE);
    CREDO_FALSUM(res);

    close(pipe_fds[0]);
    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
probatio_removere_fd(Piscina* piscina)
{
    Reactor* reactor;
    integer pipe_fds[II];
    b32 res;

    printf("--- Probans removere FD ---\n");

    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere */
    res = reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                          callback_simplex, NIHIL);
    CREDO_VERUM(res);
    CREDO_VERUM(reactor_numerus_fd(reactor) == I);

    /* Removere */
    res = reactor_removere(reactor, pipe_fds[0]);
    CREDO_VERUM(res);
    CREDO_VERUM(reactor_numerus_fd(reactor) == 0);

    /* Removere FD non existentem */
    res = reactor_removere(reactor, pipe_fds[0]);
    CREDO_FALSUM(res);

    close(pipe_fds[0]);
    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - POLL
 * ======================================================================== */

interior vacuum
probatio_poll_legere(Piscina* piscina)
{
    Reactor* reactor;
    integer pipe_fds[II];
    constans character* msg = "Salve";
    i32 events;

    printf("--- Probans poll legere ---\n");

    reset_globals();
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere read end */
    reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                    callback_simplex, NIHIL);

    /* Scribere data ad pipe */
    write(pipe_fds[I], msg, V);

    /* Poll - debet triggerare callback */
    events = reactor_poll(reactor, C);
    CREDO_VERUM(events >= I);
    CREDO_VERUM(g_callback_vocatus >= I);
    CREDO_VERUM((g_eventus_receptus & (i32)REACTOR_LEGERE) != 0);
    printf("  Events processati: %u\n", events);

    close(pipe_fds[0]);
    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
probatio_poll_scribere(Piscina* piscina)
{
    Reactor* reactor;
    integer pipe_fds[II];
    i32 events;

    printf("--- Probans poll scribere ---\n");

    reset_globals();
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere write end - pipe semper paratus ad scribendum */
    reactor_adicere(reactor, pipe_fds[I], REACTOR_SCRIBERE,
                    callback_simplex, NIHIL);

    /* Poll */
    events = reactor_poll(reactor, C);
    CREDO_VERUM(events >= I);
    CREDO_VERUM(g_callback_vocatus >= I);
    CREDO_VERUM((g_eventus_receptus & (i32)REACTOR_SCRIBERE) != 0);
    printf("  Events processati: %u\n", events);

    close(pipe_fds[0]);
    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
probatio_poll_timeout(Piscina* piscina)
{
    Reactor* reactor;
    integer pipe_fds[II];
    i32 events;

    printf("--- Probans poll timeout ---\n");

    reset_globals();
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere read end (nullus data) */
    reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                    callback_simplex, NIHIL);

    /* Poll cum breve timeout - debet timeout sine events */
    events = reactor_poll(reactor, X);  /* 10ms */
    CREDO_VERUM(events == 0);
    CREDO_VERUM(g_callback_vocatus == 0);
    printf("  Timeout correcte (nulla events)\n");

    close(pipe_fds[0]);
    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - TIMERS
 * ======================================================================== */

interior vacuum
probatio_timer_simplex(Piscina* piscina)
{
    Reactor* reactor;
    ReactorTimerId id;
    i32 counter = 0;
    i32 events;

    printf("--- Probans timer simplex ---\n");

    reset_globals();
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    /* Adicere timer 50ms one-shot */
    id = reactor_timer_adicere(reactor, L, FALSUM, callback_timer, &counter);
    CREDO_VERUM(id != REACTOR_TIMER_ERROR);
    CREDO_VERUM(reactor_numerus_timer(reactor) == I);

    /* Expectare et poll */
    usleep(LX * M);  /* 60ms */
    events = reactor_poll(reactor, C);

    CREDO_VERUM(events >= I);
    CREDO_VERUM(g_timer_vocatus >= I);
    CREDO_VERUM(counter >= I);
    printf("  Timer vocatus: %u\n", g_timer_vocatus);

    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
probatio_timer_repetens(Piscina* piscina)
{
    Reactor* reactor;
    ReactorTimerId id;
    i32 counter = 0;
    i32 i;

    printf("--- Probans timer repetens ---\n");

    reset_globals();
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    /* Adicere timer 20ms repeating */
    id = reactor_timer_adicere(reactor, XX, VERUM, callback_timer, &counter);
    CREDO_VERUM(id != REACTOR_TIMER_ERROR);

    /* Poll multiples - debet fire multiples */
    per (i = 0; i < V; i++)
    {
        usleep(XXV * M);  /* 25ms */
        reactor_poll(reactor, L);
    }

    CREDO_VERUM(counter >= III);  /* Debet fire at least 3 times */
    printf("  Timer vocatus: %u (expectatum >= 3)\n", counter);

    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
probatio_timer_cancellare(Piscina* piscina)
{
    Reactor* reactor;
    ReactorTimerId id;
    i32 counter = 0;
    b32 res;

    printf("--- Probans timer cancellare ---\n");

    reset_globals();
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    /* Adicere timer */
    id = reactor_timer_adicere(reactor, L, FALSUM, callback_timer, &counter);
    CREDO_VERUM(id != REACTOR_TIMER_ERROR);
    CREDO_VERUM(reactor_numerus_timer(reactor) == I);

    /* Cancellare ante firing */
    res = reactor_timer_cancellare(reactor, id);
    CREDO_VERUM(res);
    CREDO_VERUM(reactor_numerus_timer(reactor) == 0);

    /* Expectare et poll - timer non debet fire */
    usleep(LX * M);
    reactor_poll(reactor, C);

    CREDO_VERUM(counter == 0);
    printf("  Timer cancellatus correcte\n");

    reactor_destruere(reactor);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
probatio_nullum_argumenta(Piscina* piscina)
{
    Reactor* reactor;
    b32 res;
    ReactorTimerId id;

    printf("--- Probans nullum argumenta ---\n");

    /* Creare cum NIHIL */
    reactor = reactor_creare(NIHIL);
    CREDO_NIHIL(reactor);

    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    /* Adicere cum NIHIL callback */
    res = reactor_adicere(reactor, 0, REACTOR_LEGERE, NIHIL, NIHIL);
    CREDO_FALSUM(res);

    /* Modificare cum NIHIL reactor */
    res = reactor_modificare(NIHIL, 0, REACTOR_LEGERE);
    CREDO_FALSUM(res);

    /* Removere cum NIHIL reactor */
    res = reactor_removere(NIHIL, 0);
    CREDO_FALSUM(res);

    /* Timer cum NIHIL callback */
    id = reactor_timer_adicere(reactor, C, FALSUM, NIHIL, NIHIL);
    CREDO_VERUM(id == REACTOR_TIMER_ERROR);

    /* Poll cum NIHIL reactor */
    CREDO_VERUM(reactor_poll(NIHIL, C) == 0);

    /* Numerus cum NIHIL */
    CREDO_VERUM(reactor_numerus_fd(NIHIL) == 0);
    CREDO_VERUM(reactor_numerus_timer(NIHIL) == 0);
    CREDO_FALSUM(reactor_est_currens(NIHIL));

    reactor_destruere(reactor);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - ACTUALIZARE FD
 * ======================================================================== */

interior vacuum
probatio_actualizare_fd(Piscina* piscina)
{
    Reactor* reactor;
    integer pipe_fds[II];
    b32 res;

    printf("--- Probans actualizare FD existentem ---\n");

    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere FD */
    res = reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                          callback_simplex, NIHIL);
    CREDO_VERUM(res);
    CREDO_VERUM(reactor_numerus_fd(reactor) == I);

    /* Adicere iterum - debet actualizare, non adicere novum */
    res = reactor_adicere(reactor, pipe_fds[0], REACTOR_SCRIBERE,
                          callback_simplex, NIHIL);
    CREDO_VERUM(res);
    CREDO_VERUM(reactor_numerus_fd(reactor) == I);  /* Still 1 */
    printf("  FD actualizatus correcte\n");

    close(pipe_fds[0]);
    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - EVENT LOOP (currere/sistere)
 * ======================================================================== */

interior vacuum
probatio_currere_sistere(Piscina* piscina)
{
    Reactor* reactor;
    ReactorTimerId id;

    printf("--- Probans currere et sistere ---\n");

    reset_globals();
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    /* Adicere timer quod vocat sistere post 50ms */
    id = reactor_timer_adicere(reactor, L, FALSUM, callback_sistere, reactor);
    CREDO_VERUM(id != REACTOR_TIMER_ERROR);

    /* Ante currere */
    CREDO_FALSUM(reactor_est_currens(reactor));

    /* Currere - debet exire quando timer vocat sistere */
    reactor_currere(reactor);

    /* Post currere */
    CREDO_FALSUM(reactor_est_currens(reactor));
    CREDO_VERUM(g_timer_vocatus >= I);
    printf("  Loop exivit correcte post sistere\n");

    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
probatio_currere_cum_fd_et_timer(Piscina* piscina)
{
    Reactor* reactor;
    integer pipe_fds[II];
    ReactorTimerId id;
    constans character* msg = "Probatio";

    printf("--- Probans currere cum FD et timer ---\n");

    reset_globals();
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere FD pro legere */
    reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                    callback_simplex, NIHIL);

    /* Scribere data */
    write(pipe_fds[I], msg, VIII);

    /* Timer pro sistere post 100ms */
    id = reactor_timer_adicere(reactor, C, FALSUM, callback_sistere, reactor);
    CREDO_VERUM(id != REACTOR_TIMER_ERROR);

    /* Currere */
    reactor_currere(reactor);

    /* Verificare ambo FD et timer processati */
    CREDO_VERUM(g_callback_vocatus >= I);  /* FD callback */
    CREDO_VERUM(g_timer_vocatus >= I);     /* Timer callback */
    printf("  FD callbacks: %u, Timer callbacks: %u\n",
           g_callback_vocatus, g_timer_vocatus);

    close(pipe_fds[0]);
    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
probatio_sistere_nihil(Piscina* piscina)
{
    (vacuum)piscina;

    printf("--- Probans sistere cum NIHIL ---\n");

    /* Non debet crash */
    reactor_sistere(NIHIL);
    reactor_currere(NIHIL);

    printf("  NIHIL handled sine crash\n");
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CAPACITAS
 * ======================================================================== */

interior vacuum
probatio_timer_capacitas(Piscina* piscina)
{
    Reactor* reactor;
    ReactorTimerId id;
    i32 i;
    i32 successus_count = 0;

    printf("--- Probans timer capacitas (CXXVIII max) ---\n");

    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    /* Adicere CXXVIII timers */
    per (i = 0; i < CXXVIII; i++)
    {
        id = reactor_timer_adicere(reactor, M, FALSUM, callback_timer, NIHIL);
        si (id != REACTOR_TIMER_ERROR)
        {
            successus_count++;
        }
    }

    CREDO_VERUM(successus_count == CXXVIII);
    printf("  Timers additi: %u\n", successus_count);

    /* Proximus debet fallere */
    id = reactor_timer_adicere(reactor, M, FALSUM, callback_timer, NIHIL);
    CREDO_VERUM(id == REACTOR_TIMER_ERROR);
    printf("  Timer CXXIX recte recusatus\n");

    reactor_destruere(reactor);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - ERROR EVENTS
 * ======================================================================== */

interior vacuum
probatio_fd_clausum_event(Piscina* piscina)
{
    Reactor* reactor;
    integer pipe_fds[II];
    i32 events;

    printf("--- Probans FD clausum event ---\n");

    reset_globals();
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere read end */
    reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                    callback_fd_clausum, NIHIL);

    /* Claudere write end - debet causare POLLHUP in read end */
    close(pipe_fds[I]);

    /* Poll */
    events = reactor_poll(reactor, C);

    /* Verificare eventum clausum */
    CREDO_VERUM(events >= I);
    CREDO_VERUM(g_callback_vocatus >= I);
    /* POLLHUP mapped to REACTOR_CLAUSUM */
    CREDO_VERUM((g_eventus_receptus & (i32)REACTOR_CLAUSUM) != 0 ||
                (g_eventus_receptus & (i32)REACTOR_LEGERE) != 0);
    printf("  Eventus receptus: 0x%x\n", g_eventus_receptus);

    close(pipe_fds[0]);
    reactor_destruere(reactor);
    printf("\n");
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32 successus;

    printf("\n");
    printf("========================================\n");
    printf("PROBATIONES REACTOR\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_reactor", CXXVIII * M);
    credo_aperire(piscina);

    probatio_creare_destruere(piscina);
    probatio_adicere_fd(piscina);
    probatio_modificare_fd(piscina);
    probatio_removere_fd(piscina);
    probatio_poll_legere(piscina);
    probatio_poll_scribere(piscina);
    probatio_poll_timeout(piscina);
    probatio_timer_simplex(piscina);
    probatio_timer_repetens(piscina);
    probatio_timer_cancellare(piscina);
    probatio_nullum_argumenta(piscina);
    probatio_actualizare_fd(piscina);
    probatio_currere_sistere(piscina);
    probatio_currere_cum_fd_et_timer(piscina);
    probatio_sistere_nihil(piscina);
    probatio_timer_capacitas(piscina);
    probatio_fd_clausum_event(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
