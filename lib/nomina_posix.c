/* nomina_posix.c - resolutio nominum sub POSIX (vide caput nomina.h).
 *
 * POSIX resolutionem non-obstruentem non habet (getaddrinfo obstat;
 * getaddrinfo_a solius glibc est et fila adhibet - domus fila non
 * habet). Ergo processus FILIUS getaddrinfo vocat - per resolutorem
 * systematis (nsswitch, /etc/hosts, systemd-resolved), responsa eadem
 * ac ceterae applicationes - et responsum UNUM per fistulam scribit
 * (scriptura una < PIPE_BUF, ergo atomica), deinde _exit. Pars
 * legens fistulae in reactorem VOCANTIS intrat.
 *
 * Portus ut "servitium" numericum datur (AI_NUMERICSERV): getaddrinfo
 * sockaddr portu posito reddit, ergo haec plagula sockaddr_in6 numquam
 * tangit. IP litteralis (AI_NUMERICHOST, numquam obstat) in processu
 * ipso solvitur, sine filio. Finis semper per timer reactoris (0 ms),
 * numquam intra callback aut intra nomina_quaerere_incipere.
 *
 * Abrumpere: fossa e reactore, timer cancellatus, filius SIGKILL et
 * waitpid (nullus zombie). Tempus proprium nullum (ut in macOS): mora
 * connexionis vocantis id tegit. */
#include "postulata_posix.h"
#include "nomina.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

/* responsum filii (scriptura una) */
nomen structura {
                s32 erratum;      /* 0 = bene; aliter codex EAI_* */
                i32 numerus;
    NominaAddressus addressus[NOMINA_ADDRESSUS_MAXIMI];
          character nuntius[LXIV];  /* gai_strerror, NUL-terminatum */
} NominaResponsum;

structura NominaQuaestio {
             Reactor* reactor;
               pid_t  filius;       /* 0: nullus */
             integer  fossa;        /* -1: nulla in reactore */
         NominaFinis  finis;
              vacuum* datum;
             Piscina* piscina;
     NominaAddressus  addressus[NOMINA_ADDRESSUS_MAXIMI];
                 i32  numerus;
     NominaResponsum  responsum;    /* octeti ex fistula */
                 i32  lecti;
      ReactorTimerId  mora;
                 b32  mora_posita;
                 b32  finita;
              chorda  error;
};

interior vacuum
_moram_tollere (
    NominaQuaestio* q)
{
    si (q->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(q->reactor, q->mora);
        q->mora_posita = FALSUM;
    }
}

interior vacuum
_fossam_claudere (
    NominaQuaestio* q)
{
    si (q->fossa >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(q->reactor, q->fossa);
        (vacuum)close(q->fossa);
        q->fossa = -I;
    }
}

/* filius metitur; si adhuc vivit (abrumpere), prius occiditur */
interior vacuum
_filium_metere (
    NominaQuaestio* q,
               b32  occidere)
{
    si (q->filius > ZEPHYRUM)
    {
        si (occidere)
        {
            (vacuum)kill(q->filius, SIGKILL);
        }
        dum (   waitpid(q->filius, NIHIL, ZEPHYRUM) < ZEPHYRUM
             && errno == EINTR)
        {
        }
        q->filius = ZEPHYRUM;
    }
}

interior vacuum
_finire (
    vacuum* datum)
{
    NominaQuaestio* q = (NominaQuaestio*)datum;

    q->mora_posita = FALSUM;   /* incensus */
    si (q->finita)
    {
        redde;
    }
    q->finita = VERUM;
    _fossam_claudere(q);
    _filium_metere(q, VERUM);
    si (q->numerus == ZEPHYRUM && q->error.mensura == ZEPHYRUM)
    {
        q->error = chorda_ex_literis("nomen ignotum", q->piscina);
    }
    nomina_familias_alternare(q->addressus, q->numerus);
    q->finis(q->datum, q->addressus, q->error.mensura > ZEPHYRUM
        ? ZEPHYRUM : q->numerus, q->error);
}

interior vacuum
_finire_post (
    NominaQuaestio* q)
{
    _moram_tollere(q);
    q->mora         = reactor_timer_adicere(q->reactor, ZEPHYRUM,
        FALSUM, _finire, q);
    q->mora_posita  = (q->mora != REACTOR_TIMER_ERROR);
}

/* addrinfo -> responsum (filius et litteralis communiter) */
interior vacuum
_responsum_implere (
       NominaResponsum* r,
                   s32  erratum,
    structura addrinfo* ai)
{
    structura addrinfo* p;

    memset(r, ZEPHYRUM, magnitudo(*r));
    r->erratum = erratum;
    si (erratum != ZEPHYRUM)
    {
        strncpy(r->nuntius, gai_strerror((integer)erratum),
            magnitudo(r->nuntius) - I);
        redde;
    }
    per (p = ai; p && r->numerus < NOMINA_ADDRESSUS_MAXIMI;
         p = p->ai_next)
    {
        NominaAddressus* a = &r->addressus[r->numerus];

        si (   (p->ai_family != AF_INET && p->ai_family != AF_INET6)
            || p->ai_addr == NIHIL || p->ai_addrlen == ZEPHYRUM
            || (i32)p->ai_addrlen > NOMINA_ADDRESSUS_OCTETI)
        {
            perge;
        }
        memcpy(a->octeti, p->ai_addr, (size_t)p->ai_addrlen);
        a->mensura = (i32)p->ai_addrlen;
        a->familia = (s32)p->ai_family;
        r->numerus++;
    }
}

/* responsum -> quaestio (addressus aut error nominatus) */
interior vacuum
_responsum_accipere (
                   NominaQuaestio* q,
         constans NominaResponsum* r)
{
    i32 k;

    si (r->erratum == EAI_NONAME
        #ifdef EAI_NODATA
        || r->erratum == EAI_NODATA
        #endif
        )
    {
        q->error = chorda_ex_literis("nomen ignotum", q->piscina);
        redde;
    }
    si (r->erratum != ZEPHYRUM)
    {
        character nuntius[CXXVIII];

        sprintf(nuntius, "DNS erratum: %.*s", (integer)(LXIV - I),
            r->nuntius);
        q->error = chorda_ex_literis(nuntius, q->piscina);
        redde;
    }
    per (k = ZEPHYRUM; k < r->numerus && k < NOMINA_ADDRESSUS_MAXIMI;
         k++)
    {
        q->addressus[k] = r->addressus[k];
    }
    q->numerus = k;
}

interior vacuum
_legibilis (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
    NominaQuaestio* q = (NominaQuaestio*)datum;
           ssize_t  n;

    (vacuum)eventus;
    si (q->finita)
    {
        redde;
    }
    n = read(fossa, (character*)&q->responsum + q->lecti,
        magnitudo(q->responsum) - (size_t)q->lecti);
    si (n < ZEPHYRUM && (errno == EAGAIN || errno == EINTR))
    {
        redde;
    }
    si (n > ZEPHYRUM)
    {
        q->lecti += (i32)n;
        si (q->lecti < (i32)magnitudo(q->responsum))
        {
            redde;
        }
        _responsum_accipere(q, &q->responsum);
    }
    alioquin
    {
        /* EOF ante responsum plenum: filius periit */
        q->error = chorda_ex_literis(
            "resolutio fallita (processus filius sine responso)",
            q->piscina);
    }
    /* filius post scripturam statim exit: messis brevis */
    _fossam_claudere(q);
    _filium_metere(q, FALSUM);
    _finire_post(q);
}

/* in filio: numquam redit */
interior vacuum
_filius_quaerere (
                integer  fossa,
     constans character* hospes,
     constans character* servitium)
{
     structura addrinfo  hints;
     structura addrinfo* ai = NIHIL;
        NominaResponsum  r;
                integer  erratum;
            constans i8* p;
                 size_t  reliquum;

    memset(&hints, ZEPHYRUM, magnitudo(hints));
    hints.ai_family    = AF_UNSPEC;
    hints.ai_socktype  = SOCK_STREAM;
    hints.ai_flags     = AI_NUMERICSERV;
    erratum            = getaddrinfo(hospes, servitium, &hints, &ai);
    _responsum_implere(&r, (s32)erratum, ai);
    si (ai)
    {
        freeaddrinfo(ai);
    }
    p         = (constans i8*)&r;
    reliquum  = magnitudo(r);
    dum (reliquum > ZEPHYRUM)
    {
        ssize_t n = write(fossa, p, reliquum);

        si (n < ZEPHYRUM && errno == EINTR)
        {
            perge;
        }
        si (n <= ZEPHYRUM)
        {
            frange;
        }
        p         += n;
        reliquum  -= (size_t)n;
    }
    _exit(ZEPHYRUM);
}

NominaQuaestio*
nomina_quaerere_incipere (
        Reactor* reactor,
         chorda  hospes,
            i32  portus,
    NominaFinis  finis,
         vacuum* datum,
        Piscina* piscina,
         chorda* error)
{
        NominaQuaestio* q;
             character* hospes_c;
             character  servitium[XVI];
    structura addrinfo  hints;
    structura addrinfo* ai = NIHIL;
               integer fistula[II];
               integer erratum;

    si (!reactor || !finis || !piscina || hospes.mensura <= ZEPHYRUM)
    {
        si (error && piscina)
        {
            *error = chorda_ex_literis("hospes vacuus", piscina);
        }
        redde NIHIL;
    }
    q = (NominaQuaestio*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(NominaQuaestio));
    memset(q, ZEPHYRUM, magnitudo(NominaQuaestio));
    q->reactor  = reactor;
    q->fossa    = -I;
    q->finis    = finis;
    q->datum    = datum;
    q->piscina  = piscina;
    hospes_c    = chorda_ut_cstr(hospes, piscina);
    sprintf(servitium, "%u", (insignatus integer)portus);

    /* IP litteralis: sine filio, finis per timer 0 ms */
    memset(&hints, ZEPHYRUM, magnitudo(hints));
    hints.ai_family    = AF_UNSPEC;
    hints.ai_socktype  = SOCK_STREAM;
    hints.ai_flags     = AI_NUMERICHOST | AI_NUMERICSERV;
    erratum            = getaddrinfo(hospes_c, servitium, &hints, &ai);
    si (erratum == ZEPHYRUM)
    {
        _responsum_implere(&q->responsum, ZEPHYRUM, ai);
        freeaddrinfo(ai);
        _responsum_accipere(q, &q->responsum);
        _finire_post(q);
        redde q;
    }

    si (pipe(fistula) != ZEPHYRUM)
    {
        si (error)
        {
            *error = chorda_ex_literis("fistula DNS creari non potest",
                piscina);
        }
        redde NIHIL;
    }
    q->filius = fork();
    si (q->filius < ZEPHYRUM)
    {
        (vacuum)close(fistula[ZEPHYRUM]);
        (vacuum)close(fistula[I]);
        q->filius = ZEPHYRUM;
        si (error)
        {
            *error =
                chorda_ex_literis("processus DNS creari non potest",
                piscina);
        }
        redde NIHIL;
    }
    si (q->filius == ZEPHYRUM)
    {
        (vacuum)close(fistula[ZEPHYRUM]);
        _filius_quaerere(fistula[I], hospes_c, servitium);
    }
    (vacuum)close(fistula[I]);
    q->fossa = fistula[ZEPHYRUM];
    (vacuum)fcntl(q->fossa, F_SETFL, fcntl(q->fossa, F_GETFL)
        | O_NONBLOCK);
    si (!reactor_adicere(reactor, q->fossa, REACTOR_LEGERE, _legibilis,
            q))
    {
        (vacuum)close(q->fossa);
        q->fossa = -I;
        _filium_metere(q, VERUM);
        si (error)
        {
            *error =
                chorda_ex_literis("fossa DNS in reactorem non intrat",
                piscina);
        }
        redde NIHIL;
    }
    redde q;
}

vacuum
nomina_quaerere_abrumpere (
    NominaQuaestio* q)
{
    si (!q || q->finita)
    {
        redde;
    }
    q->finita = VERUM;
    _moram_tollere(q);
    _fossam_claudere(q);
    _filium_metere(q, VERUM);
}
