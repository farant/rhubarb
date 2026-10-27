/* probatio_reactor.c - Probationes pro Reactor (Event Loop)
 *
 * Probationes functionum reactor: FD monitoring, timers, event loop.
 */

#include "postulata_posix.h"
#include "reactor.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>


/* ========================================================================
 * VARIABILES PRO PROBATIONIBUS
 * ======================================================================== */

/* Status observatus: in sectione quaque localis, per 'data'
 * traditus (nullus status inter sectiones) */
nomen structura {
         i32  callback_vocatus;
         i32  eventus_receptus;
         i32  fd_receptus;
         i32  timer_vocatus;
     Reactor* reactor_ref;
     integer  fd_subsidiarius;
         i32* counter;
} Observatio;


/* ========================================================================
 * CALLBACKS PRO PROBATIONIBUS
 * ======================================================================== */

interior vacuum
callback_simplex (
    integer  fd,
        i32  eventus,
     vacuum* data)
{
    Observatio* o = (Observatio*)data;

    o->callback_vocatus++;
    o->eventus_receptus  = eventus;
    o->fd_receptus       = (i32)fd;
}

interior vacuum
callback_timer (
    vacuum* data)
{
    Observatio* o = (Observatio*)data;
    si (o->counter)
    {
        (*o->counter)++;
    }
    o->timer_vocatus++;
}

interior vacuum
callback_sistere (
    vacuum* data)
{
    Observatio* o = (Observatio*)data;
    o->timer_vocatus++;
    si (o->reactor_ref)
    {
        reactor_sistere(o->reactor_ref);
    }
}

interior vacuum
callback_fd_clausum (
    integer  fd,
        i32  eventus,
     vacuum* data)
{
    Observatio* o = (Observatio*)data;

    (vacuum)fd;
    o->callback_vocatus++;
    o->eventus_receptus = eventus;
}

/* Intra callback: removere se ipsum, adicere fd subsidiarium */
interior vacuum
callback_reentrans (
    integer  fd,
        i32  eventus,
     vacuum* data)
{
    Observatio* o = (Observatio*)data;

    (vacuum)eventus;
    o->callback_vocatus++;
    si (o->reactor_ref)
    {
        reactor_removere(o->reactor_ref, fd);
        reactor_adicere(o->reactor_ref, o->fd_subsidiarius,
            REACTOR_LEGERE,
                        callback_simplex, o);
    }
}


/* ========================================================================
 * FUNCTIONES AUXILIARES
 * ======================================================================== */

interior vacuum
reset_globals (
    Observatio* o)
{
    o->callback_vocatus  = 0;
    o->eventus_receptus  = 0;
    o->fd_receptus       = 0;
    o->timer_vocatus     = 0;
    o->reactor_ref       = NIHIL;
    o->fd_subsidiarius   = -1;
    o->counter           = NIHIL;
}


/* ========================================================================
 * PROBATIONES - LIFECYCLE
 * ======================================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    Reactor* reactor;

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
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    Reactor* reactor;
    integer  pipe_fds[II];
        b32  res;

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
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    Reactor* reactor;
    integer  pipe_fds[II];
        b32  res;

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
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    Reactor* reactor;
    integer  pipe_fds[II];
        b32  res;

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
_sectio_quinta (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
            Observatio  o;
               Reactor* reactor;
               integer  pipe_fds[II];
    constans character* msg = "Salve";
                   i32  events;

    reset_globals(&o);
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere read end */
    reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                    callback_simplex, &o);

    /* Scribere data ad pipe */
    write(pipe_fds[I], msg, V);

    /* Poll - debet triggerare callback */
    events = reactor_poll(reactor, C);
    CREDO_VERUM(events >= I);
    CREDO_VERUM(o.callback_vocatus >= I);
    CREDO_VERUM((o.eventus_receptus & (i32)REACTOR_LEGERE) != 0);
    printf("  Events processati: %u\n", events);

    close(pipe_fds[0]);
    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
       Piscina* piscina = c->piscina;
    Observatio  o;
       Reactor* reactor;
       integer  pipe_fds[II];
           i32  events;

    reset_globals(&o);
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere write end - pipe semper paratus ad scribendum */
    reactor_adicere(reactor, pipe_fds[I], REACTOR_SCRIBERE,
                    callback_simplex, &o);

    /* Poll */
    events = reactor_poll(reactor, C);
    CREDO_VERUM(events >= I);
    CREDO_VERUM(o.callback_vocatus >= I);
    CREDO_VERUM((o.eventus_receptus & (i32)REACTOR_SCRIBERE) != 0);
    printf("  Events processati: %u\n", events);

    close(pipe_fds[0]);
    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
_sectio_septima (
    CredoContextus* c)
{
       Piscina* piscina = c->piscina;
    Observatio  o;
       Reactor* reactor;
       integer  pipe_fds[II];
           i32  events;

    reset_globals(&o);
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere read end (nullus data) */
    reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                    callback_simplex, &o);

    /* Poll cum breve timeout - debet timeout sine events */
    events = reactor_poll(reactor, X);  /* 10ms */
    CREDO_VERUM(events == 0);
    CREDO_VERUM(o.callback_vocatus == 0);
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
_sectio_octava (
    CredoContextus* c)
{
           Piscina* piscina = c->piscina;
        Observatio  o;
           Reactor* reactor;
    ReactorTimerId  id;
               i32  counter = 0;
               i32  events;

    reset_globals(&o);
    o.counter  = &counter;
    reactor    = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    /* Adicere timer 50ms one-shot */
    id = reactor_timer_adicere(reactor, L, FALSUM, callback_timer,
        &o);
    CREDO_VERUM(id != REACTOR_TIMER_ERROR);
    CREDO_VERUM(reactor_numerus_timer(reactor) == I);

    /* Expectare et poll */
    usleep(LX * M);  /* 60ms */
    events = reactor_poll(reactor, C);

    CREDO_VERUM(events >= I);
    CREDO_VERUM(o.timer_vocatus >= I);
    CREDO_VERUM(counter >= I);
    printf("  Timer vocatus: %u\n", o.timer_vocatus);

    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
_sectio_nona (
    CredoContextus* c)
{
           Piscina* piscina = c->piscina;
        Observatio  o;
           Reactor* reactor;
    ReactorTimerId  id;
               i32  counter = 0;
               i32  i;

    reset_globals(&o);
    o.counter  = &counter;
    reactor    = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    /* Adicere timer 20ms repeating */
    id = reactor_timer_adicere(reactor, XX, VERUM, callback_timer,
        &o);
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
_sectio_decima (
    CredoContextus* c)
{
           Piscina* piscina = c->piscina;
        Observatio  o;
           Reactor* reactor;
    ReactorTimerId  id;
               i32  counter = 0;
               b32  res;

    reset_globals(&o);
    o.counter  = &counter;
    reactor    = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    /* Adicere timer */
    id = reactor_timer_adicere(reactor, L, FALSUM, callback_timer,
        &o);
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
_sectio_decima_prima (
    CredoContextus* c)
{
           Piscina* piscina = c->piscina;
           Reactor* reactor;
               b32  res;
    ReactorTimerId  id;

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
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    Reactor* reactor;
    integer  pipe_fds[II];
        b32  res;

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
_sectio_decima_tertia (
    CredoContextus* c)
{
           Piscina* piscina = c->piscina;
        Observatio  o;
           Reactor* reactor;
    ReactorTimerId  id;

    reset_globals(&o);
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);
    o.reactor_ref = reactor;

    /* Adicere timer quod vocat sistere post 50ms */
    id = reactor_timer_adicere(reactor, L, FALSUM, callback_sistere,
        &o);
    CREDO_VERUM(id != REACTOR_TIMER_ERROR);

    /* Ante currere */
    CREDO_FALSUM(reactor_est_currens(reactor));

    /* Currere - debet exire quando timer vocat sistere */
    reactor_currere(reactor);

    /* Post currere */
    CREDO_FALSUM(reactor_est_currens(reactor));
    CREDO_VERUM(o.timer_vocatus >= I);
    printf("  Loop exivit correcte post sistere\n");

    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
            Observatio  o;
               Reactor* reactor;
               integer  pipe_fds[II];
        ReactorTimerId  id;
    constans character* msg = "Probatio";

    reset_globals(&o);
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);
    o.reactor_ref = reactor;

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere FD pro legere */
    reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                    callback_simplex, &o);

    /* Scribere data */
    write(pipe_fds[I], msg, VIII);

    /* Timer pro sistere post 100ms */
    id = reactor_timer_adicere(reactor, C, FALSUM, callback_sistere,
        &o);
    CREDO_VERUM(id != REACTOR_TIMER_ERROR);

    /* Currere */
    reactor_currere(reactor);

    /* Verificare ambo FD et timer processati */
    CREDO_VERUM(o.callback_vocatus >= I);  /* FD callback */
    CREDO_VERUM(o.timer_vocatus >= I);     /* Timer callback */
    printf("  FD callbacks: %u, Timer callbacks: %u\n",
           o.callback_vocatus, o.timer_vocatus);

    close(pipe_fds[0]);
    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");

    /* sistere cum NIHIL: nulla assertio - gradus huius sectionis
     * (sectio sine assertione VACUA esset) */
    CREDO_GRADUS("sistere cum NIHIL");

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
_sectio_decima_quinta (
    CredoContextus* c)
{
           Piscina* piscina = c->piscina;
           Reactor* reactor;
    ReactorTimerId  id;
               i32  i;
               i32  successus_count = 0;

    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    /* Adicere CXXVIII timers */
    per (i = 0; i < CXXVIII; i++)
    {
        id = reactor_timer_adicere(reactor, M, FALSUM, callback_timer,
            NIHIL);
        si (id != REACTOR_TIMER_ERROR)
        {
            successus_count++;
        }
    }

    CREDO_VERUM(successus_count == CXXVIII);
    printf("  Timers additi: %u\n", successus_count);

    /* Proximus debet fallere */
    id = reactor_timer_adicere(reactor, M, FALSUM, callback_timer,
        NIHIL);
    CREDO_VERUM(id == REACTOR_TIMER_ERROR);
    printf("  Timer CXXIX recte recusatus\n");

    reactor_destruere(reactor);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - ERROR EVENTS
 * ======================================================================== */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
       Piscina* piscina = c->piscina;
    Observatio  o;
       Reactor* reactor;
       integer  pipe_fds[II];
           i32  events;

    reset_globals(&o);
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Adicere read end */
    reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                    callback_fd_clausum, &o);

    /* Claudere write end - debet causare POLLHUP in read end */
    close(pipe_fds[I]);

    /* Poll */
    events = reactor_poll(reactor, C);

    /* Verificare eventum clausum */
    CREDO_VERUM(events >= I);
    CREDO_VERUM(o.callback_vocatus >= I);
    /* POLLHUP mapped to REACTOR_CLAUSUM */
    CREDO_VERUM((o.eventus_receptus & (i32)REACTOR_CLAUSUM) != 0
        || (o.eventus_receptus & (i32)REACTOR_LEGERE) != 0);
    printf("  Eventus receptus: 0x%x\n", o.eventus_receptus);

    close(pipe_fds[0]);
    reactor_destruere(reactor);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CHURN, REENTRANTIA, POLL ERROR
 * ======================================================================== */

/* Sine reutilizatione slotorum inactivorum tabula fd permanenter
 * exhauritur - MCC cycli adicere/removere id demonstrant */
interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    Reactor* reactor;
    integer  pipe_fds[II];
        i32  cyclus;
        b32  res;

    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    per (cyclus = 0; cyclus < M + CC; cyclus++)
    {
        si (pipe(pipe_fds) != 0)
        {
            frange;
        }

        res = reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE,
                              callback_simplex, NIHIL);
        si (!res)
        {
            frange;
        }

        res = reactor_adicere(reactor, pipe_fds[I], REACTOR_SCRIBERE,
                              callback_simplex, NIHIL);
        si (!res)
        {
            frange;
        }

        reactor_removere(reactor, pipe_fds[0]);
        reactor_removere(reactor, pipe_fds[I]);
        close(pipe_fds[0]);
        close(pipe_fds[I]);
    }

    CREDO_VERUM(cyclus == M + CC);
    CREDO_VERUM(reactor_numerus_fd(reactor) == 0);
    printf("  Cycli completi: %u\n", cyclus);

    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
            Observatio  o;
               Reactor* reactor;
               integer  pipa_a[II];
               integer  pipa_b[II];
    constans character* msg = "X";
                   i32  events;

    reset_globals(&o);
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipa_a) == 0);
    CREDO_VERUM(pipe(pipa_b) == 0);

    o.reactor_ref      = reactor;
    o.fd_subsidiarius  = pipa_b[0];

    reactor_adicere(reactor, pipa_a[0], REACTOR_LEGERE,
                    callback_reentrans, &o);

    /* Excitare: intra callback removere-se + adicere subsidiarium */
    write(pipa_a[I], msg, I);
    events = reactor_poll(reactor, C);
    CREDO_VERUM(events >= I);
    CREDO_VERUM(o.callback_vocatus == I);

    /* Permutatio reflexa: unus fd monitoratus, subsidiarius respondet */
    CREDO_VERUM(reactor_numerus_fd(reactor) == I);

    write(pipa_b[I], msg, I);
    events = reactor_poll(reactor, C);
    CREDO_VERUM(events >= I);
    CREDO_VERUM(o.callback_vocatus == II);
    CREDO_VERUM(o.fd_receptus == (i32)pipa_b[0]);
    printf("  Permutatio intra callback sine fragore\n");

    close(pipa_a[0]);
    close(pipa_a[I]);
    close(pipa_b[0]);
    close(pipa_b[I]);
    reactor_destruere(reactor);
    printf("\n");
}


interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
       Piscina* piscina = c->piscina;
    Observatio  o;
       Reactor* reactor;
       integer  pipe_fds[II];
           i32  events;

    reset_globals(&o);
    reactor = reactor_creare(piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_VERUM(pipe(pipe_fds) == 0);

    /* Extremum lectionis clausum - scriptio in pipe orbam = error */
    close(pipe_fds[0]);

    reactor_adicere(reactor, pipe_fds[I], REACTOR_SCRIBERE,
                    callback_fd_clausum, &o);

    events = reactor_poll(reactor, C);

    CREDO_VERUM(events >= I);
    CREDO_VERUM(o.callback_vocatus >= I);
    /* POLLERR -> REACTOR_ERROR; systemata etiam POLLHUP dare possunt */
    CREDO_VERUM((o.eventus_receptus & (i32)REACTOR_ERROR) != 0
        || (o.eventus_receptus & (i32)REACTOR_CLAUSUM) != 0);
    printf("  Eventus receptus: 0x%x\n", o.eventus_receptus);

    close(pipe_fds[I]);
    reactor_destruere(reactor);
    printf("\n");
}


/* ========================================================================
 * HOSPES CUM ANSA SUA (vicus quies T4)
 * ======================================================================== */

/* fossae a reactor_fossas_visitare visae */
nomen structura {
        i32 numerus;
    integer fossae[VIII];
        i32 eventus[VIII];
} FossaeVisae;

interior vacuum
fossam_colligere (
     vacuum* ctx,
    integer  fd,
        i32  eventus)
{
    FossaeVisae* fv = (FossaeVisae*)ctx;

    si (fv->numerus < VIII)
    {
        fv->fossae[fv->numerus]   = fd;
        fv->eventus[fv->numerus]  = eventus;
    }
    fv->numerus++;
}

interior FossaeVisae
fossas_videre (
    constans Reactor* reactor)
{
    FossaeVisae fv;

    memset(&fv, ZEPHYRUM, magnitudo(fv));
    reactor_fossas_visitare(reactor, fossam_colligere, &fv);
    redde fv;
}

/* eventus fossae visae, aut ZEPHYRUM si non visa */
interior i32
fossae_eventus (
    constans FossaeVisae* fv,
                 integer  fd)
{
    i32 k;

    per (k = 0; k < fv->numerus && k < VIII; k++)
    {
        si (fv->fossae[k] == fd)
        {
            redde fv->eventus[k];
        }
    }
    redde ZEPHYRUM;
}

/* labor gradus unius */
interior b32
labor_semel (
    vacuum* datum)
{
    (*(i32*)datum)++;
    redde VERUM;
}

interior vacuum
signum_nullum (
    vacuum* datum)
{
    (vacuum)datum;
}

interior vacuum
_sectio_mora_proxima (
    CredoContextus* c)
{
     Observatio  o;
        Reactor* reactor;
            s32  mora;
            i32  gradus;
 ReactorTimerId  id;

    reset_globals(&o);
    reactor = reactor_creare(c->piscina);
    CREDO_NON_NIHIL(reactor);

    CREDO_GRADUS("nihil pendet: -1");
    CREDO_AEQUALIS_S64((s64)reactor_mora_proxima(reactor), (s64)-I);
    CREDO_AEQUALIS_S64((s64)reactor_mora_proxima(NIHIL), (s64)-I);

    CREDO_GRADUS("timer in CCL ms: mora ~CCL");
    (vacuum)reactor_timer_adicere(reactor, CCL, FALSUM,
        callback_timer, &o);
    mora = reactor_mora_proxima(reactor);
    CREDO_VERUM(mora > CC && mora <= CCL);

    CREDO_GRADUS("labor pendens: 0; labore finito iterum ~CCL");
    gradus = ZEPHYRUM;
    CREDO_VERUM(reactor_laborem_adicere(reactor, labor_semel, &gradus));
    CREDO_AEQUALIS_S64((s64)reactor_mora_proxima(reactor), ZEPHYRUM);
    (vacuum)reactor_poll(reactor, ZEPHYRUM);
    CREDO_AEQUALIS_I32(gradus, I);
    mora = reactor_mora_proxima(reactor);
    CREDO_VERUM(mora > CC && mora <= CCL);

    CREDO_GRADUS("timer debitus: 0");
    id = reactor_timer_adicere(reactor, ZEPHYRUM, FALSUM,
        callback_timer, &o);
    CREDO_AEQUALIS_S64((s64)reactor_mora_proxima(reactor), ZEPHYRUM);
    CREDO_VERUM(reactor_timer_cancellare(reactor, id));
    mora = reactor_mora_proxima(reactor);
    CREDO_VERUM(mora > CC && mora <= CCL);

    reactor_destruere(reactor);
}

interior vacuum
_sectio_fossae_versio (
    CredoContextus* c)
{
     Observatio  o;
        Reactor* reactor;
        integer  a[II];
        integer  b[II];
            i32  v;
            i32  gradus;
    FossaeVisae  fv;

    reset_globals(&o);
    reactor = reactor_creare(c->piscina);
    CREDO_NON_NIHIL(reactor);
    CREDO_VERUM(pipe(a) == 0);
    CREDO_VERUM(pipe(b) == 0);

    CREDO_GRADUS("adicere: versio crescit, fossae visae cum eventibus");
    fv = fossas_videre(reactor);
    CREDO_AEQUALIS_I32(fv.numerus, ZEPHYRUM);
    v = reactor_versio_fossarum(reactor);
    CREDO_VERUM(reactor_adicere(reactor, a[0], REACTOR_LEGERE,
        callback_simplex, &o));
    CREDO_VERUM(reactor_versio_fossarum(reactor) != v);
    v = reactor_versio_fossarum(reactor);
    CREDO_VERUM(reactor_adicere(reactor, b[I], REACTOR_SCRIBERE,
        callback_simplex, &o));
    CREDO_VERUM(reactor_versio_fossarum(reactor) != v);
    fv = fossas_videre(reactor);
    CREDO_AEQUALIS_I32(fv.numerus, II);
    CREDO_AEQUALIS_I32(fossae_eventus(&fv, a[0]), REACTOR_LEGERE);
    CREDO_AEQUALIS_I32(fossae_eventus(&fv, b[I]), REACTOR_SCRIBERE);

    CREDO_GRADUS("poll, timer, labor: versio eadem");
    v = reactor_versio_fossarum(reactor);
    (vacuum)reactor_poll(reactor, ZEPHYRUM);
    (vacuum)reactor_timer_adicere(reactor, M, FALSUM, callback_timer,
        &o);
    gradus = ZEPHYRUM;
    (vacuum)reactor_laborem_adicere(reactor, labor_semel, &gradus);
    (vacuum)reactor_poll(reactor, ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_versio_fossarum(reactor), v);

    CREDO_GRADUS("modificare: versio crescit, eventus novi visi");
    CREDO_VERUM(reactor_modificare(reactor, a[0],
        REACTOR_LEGERE | REACTOR_SCRIBERE));
    CREDO_VERUM(reactor_versio_fossarum(reactor) != v);
    fv = fossas_videre(reactor);
    CREDO_AEQUALIS_I32(fossae_eventus(&fv, a[0]),
        REACTOR_LEGERE | REACTOR_SCRIBERE);

    CREDO_GRADUS("removere: versio crescit, fossa abest");
    v = reactor_versio_fossarum(reactor);
    CREDO_VERUM(reactor_removere(reactor, b[I]));
    CREDO_VERUM(reactor_versio_fossarum(reactor) != v);
    fv = fossas_videre(reactor);
    CREDO_AEQUALIS_I32(fv.numerus, I);
    CREDO_AEQUALIS_I32(fossae_eventus(&fv, b[I]), ZEPHYRUM);

    CREDO_GRADUS("removere frustra: versio eadem");
    v = reactor_versio_fossarum(reactor);
    CREDO_FALSUM(reactor_removere(reactor, b[I]));
    CREDO_AEQUALIS_I32(reactor_versio_fossarum(reactor), v);

    CREDO_GRADUS("signum: fistula interna visa (legenda)");
    CREDO_VERUM(reactor_signum_adicere(reactor, SIGUSR2, signum_nullum,
        NIHIL));
    CREDO_VERUM(reactor_versio_fossarum(reactor) != v);
    fv = fossas_videre(reactor);
    CREDO_AEQUALIS_I32(fv.numerus, II);
    {
        /* fossa altera = fistula signorum (non a[0], legenda) */
        integer fistula = fv.fossae[0] == a[0] ? fv.fossae[I]
                                               : fv.fossae[0];

        CREDO_VERUM(fistula >= 0 && fistula != a[0]);
        CREDO_AEQUALIS_I32(fossae_eventus(&fv, fistula),
            REACTOR_LEGERE);
    }

    reactor_destruere(reactor);
    close(a[0]);
    close(a[I]);
    close(b[0]);
    close(b[I]);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "creare et destruere", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "adicere FD", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "modificare FD", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "removere FD", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "poll legere", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "poll scribere", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "poll timeout", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "timer simplex", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "timer repetens", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "timer cancellare", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nullum argumenta", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "actualizare FD existentem", _sectio_decima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "currere et sistere", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "currere cum FD et timer", _sectio_decima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "timer capacitas (CXXVIII max)", _sectio_decima_quinta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "FD clausum event", _sectio_decima_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "churn adicere/removere (MCC cycli)", _sectio_decima_septima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "callback reentrans", _sectio_decima_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "poll error", _sectio_decima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "hospes: mora proxima", _sectio_mora_proxima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "hospes: fossae et versio", _sectio_fossae_versio, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale (vacuum)
{
    redde credo_suitam_currere("reactor", SECTIONES);
}
