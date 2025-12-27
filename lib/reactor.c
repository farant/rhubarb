/* reactor.c - Implementatio Reactor (Event Loop)
 *
 * Utitur poll() pro I/O multiplexing.
 */

#include "reactor.h"

#include <poll.h>
#include <sys/time.h>
#include <string.h>

/* Sentinel pro "non invenit" */
#define REACTOR_NON_INVENIT 0xFFFFFFFFu


/* ========================================================================
 * STRUCTURAE INTERNAE
 * ======================================================================== */

/* Entry pro FD in reactor */
nomen structura {
    integer         fd;
    i32             eventus;
    ReactorCallback callback;
    vacuum*         data;
    b32             activus;
} ReactorFdEntry;

/* Entry pro timer */
nomen structura {
    ReactorTimerId       id;
    i64                  proximus_ms;  /* Next fire time in ms */
    i32                  interval_ms;
    b32                  repetere;
    ReactorTimerCallback callback;
    vacuum*              data;
    b32                  activus;
} ReactorTimerEntry;

/* Reactor structura */
structura Reactor {
    Piscina* piscina;

    /* FD tracking */
    ReactorFdEntry     fd_entries[REACTOR_FD_MAXIMA];
    structura pollfd   poll_fds[REACTOR_FD_MAXIMA];
    i32                fd_numerus;

    /* Timer tracking */
    ReactorTimerEntry  timers[REACTOR_TIMER_MAXIMA];
    i32                timer_numerus;
    ReactorTimerId     proximus_timer_id;

    /* State */
    b32 currens;
    b32 sistere_petitum;
};


/* ========================================================================
 * FUNCTIONES INTERNAE - TEMPUS
 * ======================================================================== */

/* Obtinere tempus currentem in milliseconds */
interior i64
_obtinere_tempus_ms(vacuum)
{
    structura timeval tv;
    gettimeofday(&tv, NIHIL);
    redde (i64)tv.tv_sec * M + (i64)tv.tv_usec / M;
}


/* ========================================================================
 * FUNCTIONES INTERNAE - FD
 * ======================================================================== */

/* Invenire FD entry index */
interior i32
_invenire_fd_index(Reactor* reactor, integer fd)
{
    i32 i;

    per (i = 0; i < reactor->fd_numerus; i++)
    {
        si (reactor->fd_entries[i].activus &&
            reactor->fd_entries[i].fd == fd)
        {
            redde i;
        }
    }

    redde REACTOR_NON_INVENIT;
}

/* Convertere reactor eventus ad poll eventus */
interior brevis
_eventus_ad_poll(i32 eventus)
{
    brevis poll_events = 0;

    si (eventus & REACTOR_LEGERE)
    {
        poll_events = (brevis)(poll_events | POLLIN);
    }
    si (eventus & REACTOR_SCRIBERE)
    {
        poll_events = (brevis)(poll_events | POLLOUT);
    }

    redde poll_events;
}

/* Convertere poll eventus ad reactor eventus */
interior i32
_poll_ad_eventus(brevis revents)
{
    i32 eventus = 0;

    si (revents & POLLIN)
    {
        eventus |= (i32)REACTOR_LEGERE;
    }
    si (revents & POLLOUT)
    {
        eventus |= (i32)REACTOR_SCRIBERE;
    }
    si (revents & POLLERR)
    {
        eventus |= (i32)REACTOR_ERROR;
    }
    si (revents & POLLHUP)
    {
        eventus |= (i32)REACTOR_CLAUSUM;
    }

    redde eventus;
}

/* Rebuilere poll_fds array ex fd_entries */
interior vacuum
_rebuilere_poll_fds(Reactor* reactor)
{
    i32 i;
    i32 poll_idx = 0;

    per (i = 0; i < reactor->fd_numerus && poll_idx < REACTOR_FD_MAXIMA; i++)
    {
        si (reactor->fd_entries[i].activus)
        {
            reactor->poll_fds[poll_idx].fd = (integer)reactor->fd_entries[i].fd;
            reactor->poll_fds[poll_idx].events = _eventus_ad_poll(reactor->fd_entries[i].eventus);
            reactor->poll_fds[poll_idx].revents = 0;
            poll_idx++;
        }
    }
}


/* ========================================================================
 * FUNCTIONES INTERNAE - TIMER
 * ======================================================================== */

/* Invenire timer index per ID */
interior i32
_invenire_timer_index(Reactor* reactor, ReactorTimerId id)
{
    i32 i;

    per (i = 0; i < reactor->timer_numerus; i++)
    {
        si (reactor->timers[i].activus &&
            reactor->timers[i].id == id)
        {
            redde i;
        }
    }

    redde REACTOR_NON_INVENIT;
}

/* Calculare timeout usque ad proximum timer */
interior i32
_calculare_timer_timeout(Reactor* reactor, i32 max_timeout_ms)
{
    i64 nunc;
    i64 minimus;
    b32 invenit;
    i32 i;
    i32 timeout;
    i64 diff;

    si (reactor->timer_numerus == 0)
    {
        redde max_timeout_ms;
    }

    nunc = _obtinere_tempus_ms();
    minimus = 0;
    invenit = FALSUM;

    per (i = 0; i < reactor->timer_numerus; i++)
    {
        si (reactor->timers[i].activus)
        {
            si (!invenit || reactor->timers[i].proximus_ms < minimus)
            {
                minimus = reactor->timers[i].proximus_ms;
                invenit = VERUM;
            }
        }
    }

    si (!invenit)
    {
        redde max_timeout_ms;
    }

    /* Calculare differentia - si negativa, timer iam expiravit */
    si (minimus <= nunc)
    {
        timeout = 0;
    }
    alioquin
    {
        diff = minimus - nunc;
        si (diff > (i64)0x7FFFFFFFu)
        {
            timeout = (i32)0x7FFFFFFFu;
        }
        alioquin
        {
            timeout = (i32)diff;
        }
    }

    si (max_timeout_ms >= 0 && timeout > max_timeout_ms)
    {
        timeout = max_timeout_ms;
    }

    redde timeout;
}

/* Processare expired timers */
interior i32
_processare_timers(Reactor* reactor)
{
    i64 nunc;
    i32 i;
    i32 processati = 0;

    nunc = _obtinere_tempus_ms();

    per (i = 0; i < reactor->timer_numerus; i++)
    {
        si (reactor->timers[i].activus &&
            reactor->timers[i].proximus_ms <= nunc)
        {
            /* Vocare callback */
            si (reactor->timers[i].callback)
            {
                reactor->timers[i].callback(reactor->timers[i].data);
            }

            processati++;

            /* Repetere vel removere */
            si (reactor->timers[i].repetere)
            {
                reactor->timers[i].proximus_ms = nunc + (i64)reactor->timers[i].interval_ms;
            }
            alioquin
            {
                reactor->timers[i].activus = FALSUM;
            }
        }
    }

    redde processati;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - LIFECYCLE
 * ======================================================================== */

Reactor*
reactor_creare(Piscina* piscina)
{
    Reactor* reactor;

    si (!piscina)
    {
        redde NIHIL;
    }

    reactor = (Reactor*)piscina_allocare(piscina, (i64)magnitudo(Reactor));
    si (!reactor)
    {
        redde NIHIL;
    }

    reactor->piscina = piscina;
    reactor->fd_numerus = 0;
    reactor->timer_numerus = 0;
    reactor->proximus_timer_id = 0;
    reactor->currens = FALSUM;
    reactor->sistere_petitum = FALSUM;

    /* Initiare arrays */
    memset(reactor->fd_entries, 0, magnitudo(reactor->fd_entries));
    memset(reactor->poll_fds, 0, magnitudo(reactor->poll_fds));
    memset(reactor->timers, 0, magnitudo(reactor->timers));

    redde reactor;
}

vacuum
reactor_destruere(Reactor* reactor)
{
    si (!reactor)
    {
        redde;
    }

    reactor->currens = FALSUM;
    reactor->fd_numerus = 0;
    reactor->timer_numerus = 0;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - FD MANAGEMENT
 * ======================================================================== */

b32
reactor_adicere(
    Reactor*        reactor,
    integer         fd,
    i32             eventus,
    ReactorCallback callback,
    vacuum*         data)
{
    i32 index;

    si (!reactor || fd < 0 || !callback)
    {
        redde FALSUM;
    }

    /* Verificare si iam existit */
    index = _invenire_fd_index(reactor, fd);
    si (index != REACTOR_NON_INVENIT)
    {
        /* Actualizare existentem */
        reactor->fd_entries[index].eventus = eventus;
        reactor->fd_entries[index].callback = callback;
        reactor->fd_entries[index].data = data;
        _rebuilere_poll_fds(reactor);
        redde VERUM;
    }

    /* Verificare capacitatem */
    si (reactor->fd_numerus >= REACTOR_FD_MAXIMA)
    {
        redde FALSUM;
    }

    /* Adicere novum */
    index = reactor->fd_numerus;
    reactor->fd_entries[index].fd = fd;
    reactor->fd_entries[index].eventus = eventus;
    reactor->fd_entries[index].callback = callback;
    reactor->fd_entries[index].data = data;
    reactor->fd_entries[index].activus = VERUM;
    reactor->fd_numerus++;

    _rebuilere_poll_fds(reactor);

    redde VERUM;
}

b32
reactor_modificare(
    Reactor* reactor,
    integer  fd,
    i32      eventus)
{
    i32 index;

    si (!reactor || fd < 0)
    {
        redde FALSUM;
    }

    index = _invenire_fd_index(reactor, fd);
    si (index == REACTOR_NON_INVENIT)
    {
        redde FALSUM;
    }

    reactor->fd_entries[index].eventus = eventus;
    _rebuilere_poll_fds(reactor);

    redde VERUM;
}

b32
reactor_removere(
    Reactor* reactor,
    integer  fd)
{
    i32 index;

    si (!reactor || fd < 0)
    {
        redde FALSUM;
    }

    index = _invenire_fd_index(reactor, fd);
    si (index == REACTOR_NON_INVENIT)
    {
        redde FALSUM;
    }

    reactor->fd_entries[index].activus = FALSUM;
    _rebuilere_poll_fds(reactor);

    redde VERUM;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - TIMER MANAGEMENT
 * ======================================================================== */

ReactorTimerId
reactor_timer_adicere(
    Reactor*             reactor,
    i32                  timeout_ms,
    b32                  repetere,
    ReactorTimerCallback callback,
    vacuum*              data)
{
    i32 index;
    i32 i;
    ReactorTimerId id;
    i64 nunc;

    si (!reactor || !callback)
    {
        redde REACTOR_NON_INVENIT;
    }

    /* Invenire slot liberum */
    index = REACTOR_NON_INVENIT;
    per (i = 0; i < REACTOR_TIMER_MAXIMA; i++)
    {
        si (!reactor->timers[i].activus)
        {
            index = i;
            frange;
        }
    }

    si (index == REACTOR_NON_INVENIT)
    {
        redde REACTOR_NON_INVENIT;  /* Nullum slot liberum */
    }

    /* Creare timer */
    nunc = _obtinere_tempus_ms();
    id = reactor->proximus_timer_id++;

    reactor->timers[index].id = id;
    reactor->timers[index].proximus_ms = nunc + (i64)timeout_ms;
    reactor->timers[index].interval_ms = timeout_ms;
    reactor->timers[index].repetere = repetere;
    reactor->timers[index].callback = callback;
    reactor->timers[index].data = data;
    reactor->timers[index].activus = VERUM;

    si (index >= reactor->timer_numerus)
    {
        reactor->timer_numerus = index + I;
    }

    redde id;
}

b32
reactor_timer_cancellare(
    Reactor*       reactor,
    ReactorTimerId id)
{
    i32 index;

    si (!reactor || id == REACTOR_NON_INVENIT)
    {
        redde FALSUM;
    }

    index = _invenire_timer_index(reactor, id);
    si (index == REACTOR_NON_INVENIT)
    {
        redde FALSUM;
    }

    reactor->timers[index].activus = FALSUM;

    redde VERUM;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - EVENT LOOP
 * ======================================================================== */

i32
reactor_poll(
    Reactor* reactor,
    i32      timeout_ms)
{
    i32 timeout;
    integer poll_result;
    i32 events_processati = 0;
    i32 i;
    i32 poll_idx;

    si (!reactor)
    {
        redde 0;
    }

    /* Calculare timeout cum timers */
    timeout = _calculare_timer_timeout(reactor, timeout_ms);

    /* Rebuilere poll_fds (in casu modificatus) */
    _rebuilere_poll_fds(reactor);

    /* Numerare FDs activos */
    {
        i32 active_count = 0;
        per (i = 0; i < reactor->fd_numerus; i++)
        {
            si (reactor->fd_entries[i].activus)
            {
                active_count++;
            }
        }

        si (active_count == 0 && reactor->timer_numerus == 0)
        {
            redde 0;
        }

        /* Vocare poll() */
        poll_result = poll(reactor->poll_fds, (nfds_t)active_count, (integer)timeout);
    }

    /* Processare FD events */
    si (poll_result > 0)
    {
        poll_idx = 0;
        per (i = 0; i < reactor->fd_numerus; i++)
        {
            si (reactor->fd_entries[i].activus)
            {
                si (reactor->poll_fds[poll_idx].revents != 0)
                {
                    i32 eventus = _poll_ad_eventus(reactor->poll_fds[poll_idx].revents);

                    si (reactor->fd_entries[i].callback)
                    {
                        reactor->fd_entries[i].callback(
                            reactor->fd_entries[i].fd,
                            eventus,
                            reactor->fd_entries[i].data);
                    }

                    events_processati++;
                }
                poll_idx++;
            }
        }
    }

    /* Processare timers */
    events_processati += _processare_timers(reactor);

    redde events_processati;
}

vacuum
reactor_currere(Reactor* reactor)
{
    si (!reactor)
    {
        redde;
    }

    reactor->currens = VERUM;
    reactor->sistere_petitum = FALSUM;

    dum (!reactor->sistere_petitum)
    {
        reactor_poll(reactor, M);  /* 1000ms timeout */
    }

    reactor->currens = FALSUM;
}

vacuum
reactor_sistere(Reactor* reactor)
{
    si (!reactor)
    {
        redde;
    }

    reactor->sistere_petitum = VERUM;
}

b32
reactor_est_currens(Reactor* reactor)
{
    si (!reactor)
    {
        redde FALSUM;
    }

    redde reactor->currens;
}

i32
reactor_numerus_fd(Reactor* reactor)
{
    i32 count = 0;
    i32 i;

    si (!reactor)
    {
        redde 0;
    }

    per (i = 0; i < reactor->fd_numerus; i++)
    {
        si (reactor->fd_entries[i].activus)
        {
            count++;
        }
    }

    redde count;
}

i32
reactor_numerus_timer(Reactor* reactor)
{
    i32 count = 0;
    i32 i;

    si (!reactor)
    {
        redde 0;
    }

    per (i = 0; i < REACTOR_TIMER_MAXIMA; i++)
    {
        si (reactor->timers[i].activus)
        {
            count++;
        }
    }

    redde count;
}
