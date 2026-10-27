/* reactor.c - Implementatio Reactor (Event Loop)
 *
 * Utitur poll() pro I/O multiplexing. Labores cooperativi post fd et
 * timers currunt (gradus unus per cyclum; labor in cyclo natus cyclo
 * proximo primum). Signa per fistulam sui: tractator systematis octetum
 * signi scribit, fd internum (non numeratum) in ansa legit et
 * tractatores vocat - status staticus UNUS (fistula + possessor),
 * quia signa processui toti pertinent (hospitium-v3-plan P4).
 */

#include "postulata_posix.h"
#include "reactor.h"

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

/* Superficies poll.h olim hic ut blocus externa stabat (iudici
 * soli); 2026-07-27 in silva/fontes/systema_posix.h promota et
 * per auspex_posix certificata (01KYB2Z7XA) - derivatio ex
 * inclusionibus eam nunc sponte praebet. */

/* Sentinel pro "non invenit" */
#define REACTOR_NON_INVENIT 0xFFFFFFFFu


/* ========================================================================
 * STRUCTURAE INTERNAE
 * ======================================================================== */

/* Entry pro FD in reactor */
nomen structura {
            integer  fd;
                i32  eventus;
    ReactorCallback  callback;
             vacuum* data;
                b32  activus;
    /* generatio auget in omni inscriptione et remotione: dispatchus
     * registrationem eandem agnoscit (http-vocatio II.0) */
                i32 generatio;
    /* fistula signorum: in reactor_numerus_fd non numeratur */
                b32 internus;
} ReactorFdEntry;

/* Labor cooperativus */
nomen structura {
    ReactorLabor  labor;
          vacuum* datum;
             b32  activus;
             i32  cyclus_natalis;   /* cyclus additionis */
} ReactorLaborEntry;

/* Signum registratum */
nomen structura {
                  integer  signum;
    ReactorSigniTractator  tractator;
                   vacuum* datum;
} ReactorSignumEntry;

/* Entry pro timer */
nomen structura {
          ReactorTimerId  id;
                     i64  proximus_ms;  /* Next fire time in ms */
                     i32  interval_ms;
                     b32  repetere;
    ReactorTimerCallback  callback;
                  vacuum* data;
                     b32  activus;
} ReactorTimerEntry;

/* Reactor structura */
structura Reactor {
    Piscina* piscina;

    /* FD tracking */
      ReactorFdEntry fd_entries[REACTOR_FD_MAXIMA];
    structura pollfd poll_fds[REACTOR_FD_MAXIMA];
                 i32 fd_numerus;

    /* imago transitus: poll_fds[k] pertinet ad
     * fd_entries[poll_index[k]] registrationis poll_generatio[k] -
     * callbacks tabulam mutare possunt sine dispatcho turbato */
                 i32 poll_index[REACTOR_FD_MAXIMA];
                 i32 poll_generatio[REACTOR_FD_MAXIMA];
                 i32 poll_numerus;

    /* vicus quies T4: crescit omni adiectione, modificatione,
     * remotione - hospes cum ansa sua fossas solum tunc renovat */
                 i32 fossarum_versio;

    /* Timer tracking */
    ReactorTimerEntry timers[REACTOR_TIMER_MAXIMA];
                  i32 timer_numerus;
       ReactorTimerId proximus_timer_id;

    /* Labores (labor_numerus = summa aquae, ut timer_numerus) */
    ReactorLaborEntry labores[REACTOR_LABOR_MAXIMA];
                  i32 labor_numerus;
                  i32 cyclus;          /* auget in omni reactor_poll */

    /* Signa (solum si possessor) */
    ReactorSignumEntry signa[REACTOR_SIGNA_MAXIMA];
                   i32 signa_numerus;

    /* State */
    b32 currens;
    b32 sistere_petitum;
};

/* Signa processui toti pertinent: fistula una, possessor unus */
hic_manens integer   _fistula_signorum[II] = { -I, -I };
hic_manens Reactor*  _signorum_possessor   = NIHIL;


/* ========================================================================
 * FUNCTIONES INTERNAE - TEMPUS
 * ======================================================================== */

/* Obtinere tempus currentem in milliseconds */
interior i64
_obtinere_tempus_ms (vacuum)
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
_invenire_fd_index (
    Reactor* reactor,
    integer  fd)
{
    i32 i;

    per (i = 0; i < reactor->fd_numerus; i++)
    {
        si (   reactor->fd_entries[i].activus
            && reactor->fd_entries[i].fd == fd)
        {
            redde i;
        }
    }

    redde REACTOR_NON_INVENIT;
}

/* Convertere reactor eventus ad poll eventus */
interior brevis
_eventus_ad_poll (
    i32 eventus)
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
_poll_ad_eventus (
    brevis revents)
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
_rebuilere_poll_fds (
    Reactor* reactor)
{
    i32 i;
    i32 poll_idx = 0;

    per (i = 0; i < reactor->fd_numerus
        && poll_idx < REACTOR_FD_MAXIMA; i++)
    {
        si (reactor->fd_entries[i].activus)
        {
            reactor->poll_fds[poll_idx].fd =
                (integer)reactor->fd_entries[i].fd;
            reactor->poll_fds[poll_idx].events =
                _eventus_ad_poll(reactor->fd_entries[i].eventus);
            reactor->poll_fds[poll_idx].revents  = 0;
            reactor->poll_index[poll_idx]        = i;
            reactor->poll_generatio[poll_idx] =
                reactor->fd_entries[i].generatio;
            poll_idx++;
        }
    }
    reactor->poll_numerus = poll_idx;
}


/* ========================================================================
 * FUNCTIONES INTERNAE - TIMER
 * ======================================================================== */

/* Invenire timer index per ID */
interior i32
_invenire_timer_index (
           Reactor* reactor,
    ReactorTimerId  id)
{
    i32 i;

    per (i = 0; i < reactor->timer_numerus; i++)
    {
        si (   reactor->timers[i].activus
            && reactor->timers[i].id == id)
        {
            redde i;
        }
    }

    redde REACTOR_NON_INVENIT;
}

/* Calculare timeout usque ad proximum timer */
interior i32
_calculare_timer_timeout (
    constans Reactor* reactor,
                 i32  max_timeout_ms)
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

    nunc     = _obtinere_tempus_ms();
    minimus  = 0;
    invenit  = FALSUM;

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

/* labores pendentes (constans: reactor_mora_proxima) */
interior i32
_labores_numerare (
    constans Reactor* reactor)
{
    i32 numerus = 0;
    i32 i;

    si (!reactor)
    {
        redde 0;
    }
    per (i = 0; i < reactor->labor_numerus; i++)
    {
        si (reactor->labores[i].activus)
        {
            numerus++;
        }
    }
    redde numerus;
}

/* Processare expired timers */
interior i32
_processare_timers (
    Reactor* reactor)
{
               i64 nunc;
               i32 i;
               i32 numerus;
               i32 processati = 0;
    ReactorTimerId limes;

    nunc     = _obtinere_tempus_ms();
    /* timers in hoc transitu nati (id >= limes) proximum exspectant -
     * aliter callback qui semper addit ansam numquam finiret */
    limes    = reactor->proximus_timer_id;
    numerus  = reactor->timer_numerus;

    per (i = 0; i < numerus; i++)
    {
           ReactorTimerEntry* t = &reactor->timers[i];
        ReactorTimerCallback  functio;
                      vacuum* datum;

        si (!t->activus || t->proximus_ms > nunc || t->id >= limes)
        {
            perge;
        }
        functio  = t->callback;
        datum    = t->data;
        /* status ANTE callback: callback se ipsum cancellare et
         * timerem novum (fortasse in hoc loco) addere potest - olim
         * activus post callback exstinguebatur, novus cum eo */
        si (t->repetere)
        {
            t->proximus_ms = nunc + (i64)t->interval_ms;
        }
        alioquin
        {
            t->activus = FALSUM;
        }
        si (functio)
        {
            functio(datum);
        }
        processati++;
    }

    redde processati;
}


/* ========================================================================
 * FUNCTIONES INTERNAE - LABORES ET SIGNA
 * ======================================================================== */

/* Gradus unus cuiusque laboris non in hoc cyclo nati (labor in gradu
 * alterius additus - fortasse in loco iam percurso - cyclo proximo) */
interior i32
_processare_labores (
    Reactor* reactor)
{
    i32 i;
    i32 numerus     = reactor->labor_numerus;
    i32 processati  = 0;

    per (i = 0; i < numerus; i++)
    {
        ReactorLaborEntry* l = &reactor->labores[i];

        si (!l->activus || l->cyclus_natalis == reactor->cyclus)
        {
            perge;
        }
        si (l->labor(l->datum))
        {
            l->activus = FALSUM;
        }
        processati++;
    }

    redde processati;
}

/* Tractator systematis: octetum signi in fistulam, nihil aliud
 * (async-signal-safe: write et errno solum) */
interior vacuum
_signum_excipere (
    integer signum)
{
    integer servatum  = errno;
         i8 octetus   = (i8)signum;

    si (write(_fistula_signorum[I], &octetus, I) < 0)
    {
        /* fistula plena: signum iam pendet - confluit */
    }
    errno = servatum;
}

/* fd internum: octeti signorum lecti, tractatores in ansa vocati */
interior vacuum
_signa_legere (
    integer  fd,
        i32  eventus,
     vacuum* data)
{
     Reactor* reactor = (Reactor*)data;
          i8  capsa[XVI];
     ssize_t  n;

    (vacuum)eventus;
    dum ((n = read(fd, capsa, magnitudo(capsa))) > 0)
    {
        i32 k;

        per (k = 0; k < (i32)n; k++)
        {
            i32 j;

            per (j = 0; j < reactor->signa_numerus; j++)
            {
                si (reactor->signa[j].signum == (integer)capsa[k])
                {
                    ReactorSignumEntry* e = &reactor->signa[j];

                    e->tractator(e->datum);
                }
            }
        }
    }
}

/* fistula non-obstruens, clausa in exec */
interior b32
_fistulam_parare (
    integer fd)
{
    integer vexilla = fcntl(fd, F_GETFL, 0);

    si (vexilla < 0 || fcntl(fd, F_SETFL, vexilla | O_NONBLOCK) < 0)
    {
        redde FALSUM;
    }
    redde fcntl(fd, F_SETFD, FD_CLOEXEC) >= 0;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - LIFECYCLE
 * ======================================================================== */

Reactor*
reactor_creare (
    Piscina* piscina)
{
    Reactor* reactor;

    si (!piscina)
    {
        redde NIHIL;
    }

    reactor = (Reactor*)piscina_allocare(piscina,
        (i64)magnitudo(Reactor));
    si (!reactor)
    {
        redde NIHIL;
    }

    reactor->piscina            = piscina;
    reactor->fd_numerus         = 0;
    reactor->timer_numerus      = 0;
    reactor->proximus_timer_id  = 0;
    reactor->currens            = FALSUM;
    reactor->sistere_petitum    = FALSUM;
    reactor->labor_numerus      = 0;
    reactor->cyclus             = 0;
    reactor->signa_numerus      = 0;
    reactor->fossarum_versio    = 0;

    /* Initiare arrays */
    memset(reactor->fd_entries, 0, magnitudo(reactor->fd_entries));
    memset(reactor->poll_fds, 0, magnitudo(reactor->poll_fds));
    memset(reactor->timers, 0, magnitudo(reactor->timers));
    memset(reactor->labores, 0, magnitudo(reactor->labores));

    redde reactor;
}

vacuum
reactor_destruere (
    Reactor* reactor)
{
    si (!reactor)
    {
        redde;
    }

    /* possessor signorum: tractatores ordinarii restituuntur, fistula
     * clauditur - reactor alter postea signa capere potest */
    si (_signorum_possessor == reactor)
    {
        i32 k;

        per (k = 0; k < reactor->signa_numerus; k++)
        {
            structura sigaction sa;

            memset(&sa, 0, magnitudo(sa));
            sa.sa_handler = SIG_DFL;
            (vacuum)sigemptyset(&sa.sa_mask);
            (vacuum)sigaction(reactor->signa[k].signum, &sa, NIHIL);
        }
        (vacuum)close(_fistula_signorum[0]);
        (vacuum)close(_fistula_signorum[I]);
        _fistula_signorum[0]    = -I;
        _fistula_signorum[I]    = -I;
        _signorum_possessor     = NIHIL;
        reactor->signa_numerus  = 0;
    }

    reactor->currens        = FALSUM;
    reactor->fd_numerus     = 0;
    reactor->timer_numerus  = 0;
    reactor->labor_numerus  = 0;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - FD MANAGEMENT
 * ======================================================================== */

b32
reactor_adicere (
            Reactor* reactor,
            integer  fd,
                i32  eventus,
    ReactorCallback  callback,
             vacuum* data)
{
    i32 index;
    i32 i;

    si (!reactor || fd < 0 || !callback)
    {
        redde FALSUM;
    }

    /* Verificare si iam existit */
    index = _invenire_fd_index(reactor, fd);
    si (index != REACTOR_NON_INVENIT)
    {
        /* Actualizare existentem */
        reactor->fd_entries[index].eventus   = eventus;
        reactor->fd_entries[index].callback  = callback;
        reactor->fd_entries[index].data      = data;
        reactor->fossarum_versio++;
        redde VERUM;
    }

    /* Reuti slot inactivum primum (speculum exemplaris timer) - sine
     * hoc churn adicere/removere tabulam permanenter exhaurit */
    index = REACTOR_NON_INVENIT;
    per (i = 0; i < reactor->fd_numerus; i++)
    {
        si (!reactor->fd_entries[i].activus)
        {
            index = i;
            frange;
        }
    }

    /* Nullum slot liberum - extendere si capacitas sinit */
    si (index == REACTOR_NON_INVENIT)
    {
        si (reactor->fd_numerus >= REACTOR_FD_MAXIMA)
        {
            redde FALSUM;
        }
        index = reactor->fd_numerus;
        reactor->fd_numerus++;
    }

    reactor->fd_entries[index].fd        = fd;
    reactor->fd_entries[index].eventus   = eventus;
    reactor->fd_entries[index].callback  = callback;
    reactor->fd_entries[index].data      = data;
    reactor->fd_entries[index].activus   = VERUM;
    reactor->fd_entries[index].internus  = FALSUM;
    reactor->fd_entries[index].generatio++;
    reactor->fossarum_versio++;

    /* nulla rebuilatio hic: reactor_poll in initio omnis transitus
     * rebuilat, et rebuilatio intra dispatchum revents posteriorum
     * purgabat (eventus transitus deperditi, http-vocatio II.0) */
    redde VERUM;
}

b32
reactor_modificare (
    Reactor* reactor,
    integer  fd,
        i32  eventus)
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
    reactor->fossarum_versio++;

    redde VERUM;
}

b32
reactor_removere (
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
    reactor->fd_entries[index].generatio++;
    reactor->fossarum_versio++;

    redde VERUM;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - TIMER MANAGEMENT
 * ======================================================================== */

ReactorTimerId
reactor_timer_adicere (
                 Reactor* reactor,
                     i32  timeout_ms,
                     b32  repetere,
    ReactorTimerCallback  callback,
                  vacuum* data)
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
    nunc  = _obtinere_tempus_ms();
    id    = reactor->proximus_timer_id++;

    reactor->timers[index].id           = id;
    reactor->timers[index].proximus_ms  = nunc + (i64)timeout_ms;
    reactor->timers[index].interval_ms  = timeout_ms;
    reactor->timers[index].repetere     = repetere;
    reactor->timers[index].callback     = callback;
    reactor->timers[index].data         = data;
    reactor->timers[index].activus      = VERUM;

    si (index >= reactor->timer_numerus)
    {
        reactor->timer_numerus = index + I;
    }

    redde id;
}

b32
reactor_timer_cancellare (
           Reactor* reactor,
    ReactorTimerId  id)
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
 * FUNCTIONES PUBLICAE - LABORES ET SIGNA
 * ======================================================================== */

b32
reactor_laborem_adicere (
         Reactor* reactor,
    ReactorLabor  labor,
          vacuum* datum)
{
    i32 index = REACTOR_NON_INVENIT;
    i32 i;

    si (!reactor || !labor)
    {
        redde FALSUM;
    }
    per (i = 0; i < reactor->labor_numerus; i++)
    {
        si (!reactor->labores[i].activus)
        {
            index = i;
            frange;
        }
    }
    si (index == REACTOR_NON_INVENIT)
    {
        si (reactor->labor_numerus >= REACTOR_LABOR_MAXIMA)
        {
            redde FALSUM;
        }
        index = reactor->labor_numerus;
        reactor->labor_numerus++;
    }
    reactor->labores[index].labor           = labor;
    reactor->labores[index].datum           = datum;
    reactor->labores[index].activus         = VERUM;
    reactor->labores[index].cyclus_natalis  = reactor->cyclus;
    redde VERUM;
}

i32
reactor_numerus_laborum (
    Reactor* reactor)
{
    redde _labores_numerare(reactor);
}

b32
reactor_signum_adicere (
                  Reactor* reactor,
                  integer  signum,
    ReactorSigniTractator  tractator,
                   vacuum* datum)
{
    structura sigaction sa;

    si (!reactor || !tractator || signum <= 0)
    {
        redde FALSUM;
    }
    si (   _signorum_possessor != NIHIL
        && _signorum_possessor != reactor)
    {
        redde FALSUM;
    }
    si (reactor->signa_numerus >= REACTOR_SIGNA_MAXIMA)
    {
        redde FALSUM;
    }
    si (_signorum_possessor == NIHIL)
    {
        i32 index;

        si (pipe(_fistula_signorum) != 0)
        {
            _fistula_signorum[0] = -I;
            _fistula_signorum[I] = -I;
            redde FALSUM;
        }
        si (   !_fistulam_parare(_fistula_signorum[0])
            || !_fistulam_parare(_fistula_signorum[I])
            || !reactor_adicere(reactor, _fistula_signorum[0],
                   (i32)REACTOR_LEGERE, _signa_legere, reactor))
        {
            (vacuum)close(_fistula_signorum[0]);
            (vacuum)close(_fistula_signorum[I]);
            _fistula_signorum[0] = -I;
            _fistula_signorum[I] = -I;
            redde FALSUM;
        }
        index = _invenire_fd_index(reactor, _fistula_signorum[0]);
        reactor->fd_entries[index].internus = VERUM;
        _signorum_possessor = reactor;
    }

    memset(&sa, 0, magnitudo(sa));
    sa.sa_handler  = _signum_excipere;
    sa.sa_flags    = SA_RESTART;
    (vacuum)sigemptyset(&sa.sa_mask);
    si (sigaction(signum, &sa, NIHIL) != 0)
    {
        redde FALSUM;
    }
    reactor->signa[reactor->signa_numerus].signum     = signum;
    reactor->signa[reactor->signa_numerus].tractator  = tractator;
    reactor->signa[reactor->signa_numerus].datum      = datum;
    reactor->signa_numerus++;
    redde VERUM;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - EVENT LOOP
 * ======================================================================== */

i32
reactor_poll (
    Reactor* reactor,
        i32  timeout_ms)
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

    reactor->cyclus++;

    /* timeout ex timers; labor pendens = nulla exspectatio */
    timeout = _calculare_timer_timeout(reactor, timeout_ms);
    si (reactor_numerus_laborum(reactor) > 0)
    {
        timeout = 0;
    }

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

        /* timers ACTIVI, non timer_numerus (summa aquae, numquam
         * decrescit): olim post timerem primum poll() sine fossis
         * timeout totum dormiebat */
        si (   active_count == 0 && reactor_numerus_timer(reactor) == 0
            && reactor_numerus_laborum(reactor) == 0)
        {
            redde 0;
        }

        /* Vocare poll() */
        poll_result = poll(reactor->poll_fds,
            (nfds_t)reactor->poll_numerus, (integer)timeout);
    }

    /* Processare FD events per imaginem (vide _rebuilere_poll_fds) */
    si (poll_result > 0)
    {
        i32 numerus = reactor->poll_numerus;

        per (poll_idx = 0; poll_idx < numerus; poll_idx++)
        {
            ReactorFdEntry* e;

            si (reactor->poll_fds[poll_idx].revents == 0)
            {
                perge;
            }
            e = &reactor->fd_entries[reactor->poll_index[poll_idx]];
            /* remota aut locus reusus intra hunc transitum */
            si (   !e->activus
                || e->generatio != reactor->poll_generatio[poll_idx])
            {
                perge;
            }
            si (e->callback)
            {
                e->callback(e->fd, _poll_ad_eventus(
                    reactor->poll_fds[poll_idx].revents), e->data);
            }
            events_processati++;
        }
    }

    /* poll() a signo interruptus (EINTR - SA_RESTART poll() NON
     * restituit, nec Darwin nec Linux): fistula signorum hic hauritur,
     * aliter signum cyclum integrum exspectaret */
    si (   poll_result < 0 && errno == EINTR
        && _signorum_possessor == reactor)
    {
        _signa_legere(_fistula_signorum[0], 0, reactor);
        events_processati++;
    }

    /* Processare timers */
    events_processati += _processare_timers(reactor);

    /* Processare labores (gradus unus quisque) */
    events_processati += _processare_labores(reactor);

    redde events_processati;
}

vacuum
reactor_currere (
    Reactor* reactor)
{
    si (!reactor)
    {
        redde;
    }

    reactor->currens          = VERUM;
    reactor->sistere_petitum  = FALSUM;

    dum (!reactor->sistere_petitum)
    {
        reactor_poll(reactor, M);  /* 1000ms timeout */
    }

    reactor->currens = FALSUM;
}

vacuum
reactor_sistere (
    Reactor* reactor)
{
    si (!reactor)
    {
        redde;
    }

    reactor->sistere_petitum = VERUM;
}

b32
reactor_est_currens (
    Reactor* reactor)
{
    si (!reactor)
    {
        redde FALSUM;
    }

    redde reactor->currens;
}

i32
reactor_numerus_fd (
    Reactor* reactor)
{
    i32 count = 0;
    i32 i;

    si (!reactor)
    {
        redde 0;
    }

    per (i = 0; i < reactor->fd_numerus; i++)
    {
        si (   reactor->fd_entries[i].activus
            && !reactor->fd_entries[i].internus)
        {
            count++;
        }
    }

    redde count;
}

i32
reactor_numerus_timer (
    Reactor* reactor)
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


/* ========================================================================
 * FUNCTIONES PUBLICAE - HOSPES CUM ANSA SUA (vicus quies T4)
 * ======================================================================== */

s32
reactor_mora_proxima (
    constans Reactor* reactor)
{
    i32 mora;

    si (!reactor)
    {
        redde -I;
    }
    si (_labores_numerare(reactor) > 0)
    {
        redde 0;
    }
    /* maximum 0xFFFFFFFF redditur solum si nullus timer activus
     * (mora timeris ad 0x7FFFFFFF coercetur) */
    mora = _calculare_timer_timeout(reactor, REACTOR_NON_INVENIT);
    si (mora == REACTOR_NON_INVENIT)
    {
        redde -I;
    }
    redde (s32)mora;
}

vacuum
reactor_fossas_visitare (
       constans Reactor* reactor,
    ReactorFossaVisitor  visitor,
                 vacuum* ctx)
{
    i32 i;

    si (!reactor || !visitor)
    {
        redde;
    }
    per (i = 0; i < reactor->fd_numerus; i++)
    {
        si (reactor->fd_entries[i].activus)
        {
            visitor(ctx, reactor->fd_entries[i].fd,
                reactor->fd_entries[i].eventus);
        }
    }
}

i32
reactor_versio_fossarum (
    constans Reactor* reactor)
{
    si (!reactor)
    {
        redde 0;
    }
    redde reactor->fossarum_versio;
}
