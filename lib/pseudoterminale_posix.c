/* pseudoterminale_posix.c - pons posix: openpty + furca + exec
 * (aemulator-plan B3, decisiones XI-XVI)
 *
 * Forma: openpty (magnitudine posita) -> fistula CLOEXEC erroris ->
 * fork. INFANS: setsid (sessio et grex novi), TIOCSCTTY (servus fit
 * terminale regens), servus in fossas 0/1/2, signa ad ordinarium
 * (ignorata per exec manent - hospes SIGPIPE ignorans infanti non
 * tradit), directorium, ambitus, execvp; defectus (gradus + errno)
 * per fistulam, _exit. PARENS: fistulam legit donec exec aut
 * defectus - "binarium abest" a "exitus 127" DISTINGUITUR (mos
 * processus, decisio XII: codex iteratus, non communis).
 *
 * Magister: CLOEXEC (alii infantes eum non hereditant) et
 * NON-OBSTANS: scribere reddit acceptos (decisio XVI), legere per
 * poll moram observat. Finis: read 0 aut EIO (latus servum
 * clausum) = -1.
 *
 * Messis per waitpid WNOHANG in finitus (decisio XIII) - nulli
 * tractatores signorum, nullus status communis. claudere: SIGHUP
 * gregi, magistrum claudere, moram brevem exspectare, deinde SIGKILL
 * et messis obstans - post id nullus zombi.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "pseudoterminale.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <util.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

/* mora claudendi: SIGHUP -> SIGKILL (X gradus V ms) */
#define CLAUDERE_GRADUS  XX
#define CLAUDERE_MORA_MS V

/* gradus defectus infantis (per fistulam) */
#define GRADUS_DIRECTORIUM I
#define GRADUS_EXEC        II

nomen structura {
          Pseudoterminale pons;      /* pons.datum = haec structura */
                  integer magister;  /* -1 = clausus */
                    pid_t infans;
                      b32 messus;
    PseudoterminaleExitus exitus;
} PseudoterminalePosix;

nomen structura {
    integer gradus;
    integer numerus_erroris;
} DefectusInfantis;

interior b32
magnitudinem_implere (
    struct winsize* fenestra,
               i32  latitudo,
               i32  altitudo,
               i32  px_latitudo,
               i32  px_altitudo)
{
    si (   latitudo < I || altitudo < I
        || latitudo > 0xFFFF || altitudo > 0xFFFF
        || px_latitudo > 0xFFFF || px_altitudo > 0xFFFF)
    {
        redde FALSUM;
    }
    memset(fenestra, ZEPHYRUM, magnitudo(*fenestra));
    fenestra->ws_col     = (unsigned short)latitudo;
    fenestra->ws_row     = (unsigned short)altitudo;
    fenestra->ws_xpixel  = (unsigned short)px_latitudo;
    fenestra->ws_ypixel  = (unsigned short)px_altitudo;
    redde VERUM;
}

interior s32
posix_legere (
    vacuum* datum,
        i8* buffer,
       i32  capacitas,
       s32  mora_ms)
{
    PseudoterminalePosix* p;
           struct pollfd  pf;
                 integer r;
                 ssize_t n;

    p = (PseudoterminalePosix*)datum;
    si (p->magister < ZEPHYRUM)
    {
        redde -I;
    }
    si (capacitas == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    pf.fd = p->magister;
    pf.events = POLLIN;
    pf.revents = ZEPHYRUM;
    r = poll(&pf, I, mora_ms < ZEPHYRUM ? -I : (integer)mora_ms);
    si (r == ZEPHYRUM || (r < ZEPHYRUM && errno == EINTR))
    {
        redde ZEPHYRUM;
    }
    n = read(p->magister, buffer, (size_t)capacitas);
    si (n > ZEPHYRUM)
    {
        redde (s32)n;
    }
    si (n < ZEPHYRUM && (errno == EAGAIN || errno == EINTR))
    {
        redde ZEPHYRUM;
    }
    /* 0 aut EIO: latus servum clausum - infans finivit */
    redde -I;
}

interior s32
posix_scribere (
          vacuum* datum,
     constans i8* octeti,
             i32  n)
{
    PseudoterminalePosix* p;
                 ssize_t  r;

    p = (PseudoterminalePosix*)datum;
    si (p->magister < ZEPHYRUM)
    {
        redde -I;
    }
    si (n == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    r = write(p->magister, octeti, (size_t)n);
    si (r >= ZEPHYRUM)
    {
        redde (s32)r;
    }
    si (errno == EAGAIN || errno == EINTR)
    {
        redde ZEPHYRUM;
    }
    redde -I;
}

interior b32
posix_amplitudo (
    vacuum* datum,
       i32  latitudo,
       i32  altitudo,
       i32  px_latitudo,
       i32  px_altitudo)
{
    PseudoterminalePosix* p;
          struct winsize  fenestra;

    p = (PseudoterminalePosix*)datum;
    si (   p->magister < ZEPHYRUM
        || !magnitudinem_implere(&fenestra, latitudo, altitudo,
               px_latitudo, px_altitudo))
    {
        redde FALSUM;
    }
    redde ioctl(p->magister, TIOCSWINSZ, &fenestra) == ZEPHYRUM;
}

interior vacuum
statum_notare (
    PseudoterminalePosix* p,
                 integer  status)
{
    p->messus = VERUM;
    si (WIFSIGNALED(status))
    {
        p->exitus.codex   = ZEPHYRUM;
        p->exitus.signum  = (i32)WTERMSIG(status);
    }
    alioquin
    {
        p->exitus.codex   = (i32)WEXITSTATUS(status);
        p->exitus.signum  = ZEPHYRUM;
    }
}

interior b32
posix_finitus (
                   vacuum* datum,
    PseudoterminaleExitus* exitus)
{
    PseudoterminalePosix* p;
                 integer  status;
                   pid_t  r;

    p = (PseudoterminalePosix*)datum;
    si (!p->messus)
    {
        status  = ZEPHYRUM;
        r       = waitpid(p->infans, &status, WNOHANG);
        si (r == p->infans)
        {
            statum_notare(p, status);
        }
        alioquin si (r < ZEPHYRUM && errno != EINTR)
        {
            /* ECHILD: alius messuit - exitus ignotus */
            p->messus = VERUM;
        }
    }
    si (p->messus && exitus)
    {
        *exitus = p->exitus;
    }
    redde p->messus;
}

interior s32
posix_fossa (
    vacuum* datum)
{
    redde (s32)((PseudoterminalePosix*)datum)->magister;
}

interior vacuum
posix_claudere (
    vacuum* datum)
{
    PseudoterminalePosix* p;
                 integer  status;
                     i32  i;

    p = (PseudoterminalePosix*)datum;
    /* signa solum ante messem: pid non messus non reutitur */
    si (!p->messus)
    {
        (vacuum)kill(-p->infans, SIGHUP);
        (vacuum)kill(p->infans, SIGHUP);
    }
    si (p->magister >= ZEPHYRUM)
    {
        (vacuum)close(p->magister);
        p->magister = -I;
    }
    per (i = ZEPHYRUM; i < CLAUDERE_GRADUS && !p->messus; i++)
    {
        si (posix_finitus(p, NIHIL))
        {
            frange;
        }
        (vacuum)poll(NIHIL, ZEPHYRUM, CLAUDERE_MORA_MS);
    }
    si (!p->messus)
    {
        (vacuum)kill(-p->infans, SIGKILL);
        (vacuum)kill(p->infans, SIGKILL);
        dum (   waitpid(p->infans, &status, ZEPHYRUM) < ZEPHYRUM
             && errno == EINTR)
        {
        }
        p->messus = VERUM;
    }
}

/* INFANS: numquam redit. Defectum per fistulam nuntiat. */
interior vacuum
infantem_agere (
    constans PseudoterminaleConfiguratio* cfg,
                                 integer  servus,
                                 integer  fistula)
{
    hic_manens constans integer signa[] = {
        SIGHUP, SIGINT, SIGQUIT, SIGPIPE, SIGALRM, SIGTERM, SIGCHLD,
        SIGTSTP, SIGTTIN, SIGTTOU, SIGWINCH
    };
    DefectusInfantis d;
           character titulus[CCLVI];
                 i32 i;
                 i32 k;
    unio {
        constans character* constans* c;
        character* constans*          m;
    } u;

    (vacuum)setsid();
    (vacuum)ioctl(servus, TIOCSCTTY, ZEPHYRUM);
    si (   dup2(servus, STDIN_FILENO) < ZEPHYRUM
        || dup2(servus, STDOUT_FILENO) < ZEPHYRUM
        || dup2(servus, STDERR_FILENO) < ZEPHYRUM)
    {
        _exit(CXXVII);
    }
    si (servus > STDERR_FILENO)
    {
        (vacuum)close(servus);
    }
    per (i = ZEPHYRUM;
         i < (i32)(magnitudo(signa) / magnitudo(signa[ZEPHYRUM]));
         i++)
    {
        (vacuum)signal(signa[i], SIG_DFL);
    }
    si (cfg->directorium && chdir(cfg->directorium) != ZEPHYRUM)
    {
        d.gradus           = GRADUS_DIRECTORIUM;
        d.numerus_erroris  = errno;
        (vacuum)write(fistula, &d, magnitudo(d));
        _exit(CXXVII);
    }
    /* "NOMEN=valor" ponit, "NOMEN" tollit; titulus longior omittitur */
    per (i = ZEPHYRUM; cfg->ambitus && cfg->ambitus[i]; i++)
    {
        per (k = ZEPHYRUM; cfg->ambitus[i][k]
            && cfg->ambitus[i][k] != '='
                 && k < CCLV; k++)
        {
            titulus[k] = cfg->ambitus[i][k];
        }
        si (k >= CCLV)
        {
            perge;
        }
        titulus[k] = '\0';
        si (cfg->ambitus[i][k] == '=')
        {
            (vacuum)setenv(titulus, cfg->ambitus[i] + k + I, I);
        }
        alioquin
        {
            (vacuum)unsetenv(titulus);
        }
    }
    u.c = cfg->argumenta;
    (vacuum)execvp(cfg->argumenta[ZEPHYRUM], u.m);
    d.gradus           = GRADUS_EXEC;
    d.numerus_erroris  = errno;
    (vacuum)write(fistula, &d, magnitudo(d));
    _exit(CXXVII);
}

interior vacuum
errorem_ponere (
       PseudoterminaleError* error,
                     chorda* descriptio,
                    Piscina* piscina,
       PseudoterminaleError  genus,
         constans character* quid,
                    integer  numerus_erroris)
{
    character b[CCLVI];

    si (error)
    {
        *error = genus;
    }
    si (descriptio)
    {
        si (numerus_erroris != ZEPHYRUM)
        {
            (vacuum)snprintf(b, magnitudo(b), "%s: %s", quid,
                strerror(numerus_erroris));
        }
        alioquin
        {
            (vacuum)snprintf(b, magnitudo(b), "%s", quid);
        }
        *descriptio = chorda_ex_literis(b, piscina);
    }
}

Pseudoterminale*
pseudoterminale_posix_creare (
                                 Piscina* piscina,
    constans PseudoterminaleConfiguratio* cfg,
                    PseudoterminaleError* error,
                                  chorda* descriptio)
{
    PseudoterminalePosix* p;
          struct winsize  fenestra;
                 integer magister;
                 integer servus;
                 integer fistula[II];
                 integer vexilla;
                 integer status;
        DefectusInfantis d;
                 ssize_t n;
                   pid_t infans;

    errorem_ponere(error, NIHIL, piscina, PSEUDOTERMINALE_OK, "",
        ZEPHYRUM);
    si (   !piscina || !cfg || !cfg->argumenta
        || !cfg->argumenta[ZEPHYRUM]
        || !magnitudinem_implere(&fenestra, cfg->latitudo,
        cfg->altitudo,
               cfg->px_latitudo, cfg->px_altitudo))
    {
        errorem_ponere(error, descriptio, piscina,
            PSEUDOTERMINALE_ERROR_ARGUMENTA, "argumenta mala",
            ZEPHYRUM);
        redde NIHIL;
    }
    p = (PseudoterminalePosix*)piscina_conari_allocare(piscina,
        magnitudo(PseudoterminalePosix));
    si (!p)
    {
        errorem_ponere(error, descriptio, piscina,
            PSEUDOTERMINALE_ERROR_GENERARE, "piscina deficit",
            ZEPHYRUM);
        redde NIHIL;
    }
    si (openpty(&magister, &servus, NIHIL, NIHIL, &fenestra)
        != ZEPHYRUM)
    {
        errorem_ponere(error, descriptio, piscina,
            PSEUDOTERMINALE_ERROR_APERIRE, "openpty", errno);
        redde NIHIL;
    }
    (vacuum)fcntl(magister, F_SETFD, FD_CLOEXEC);
    (vacuum)fcntl(servus, F_SETFD, FD_CLOEXEC);
    si (pipe(fistula) != ZEPHYRUM)
    {
        errorem_ponere(error, descriptio, piscina,
            PSEUDOTERMINALE_ERROR_GENERARE, "pipe", errno);
        (vacuum)close(magister);
        (vacuum)close(servus);
        redde NIHIL;
    }
    /* CLOEXEC: exec felix fistulam tacite claudit = signum */
    (vacuum)fcntl(fistula[ZEPHYRUM], F_SETFD, FD_CLOEXEC);
    (vacuum)fcntl(fistula[I], F_SETFD, FD_CLOEXEC);

    infans = fork();
    si (infans < ZEPHYRUM)
    {
        errorem_ponere(error, descriptio, piscina,
            PSEUDOTERMINALE_ERROR_GENERARE, "fork", errno);
        (vacuum)close(magister);
        (vacuum)close(servus);
        (vacuum)close(fistula[ZEPHYRUM]);
        (vacuum)close(fistula[I]);
        redde NIHIL;
    }
    si (infans == ZEPHYRUM)
    {
        (vacuum)close(magister);
        (vacuum)close(fistula[ZEPHYRUM]);
        infantem_agere(cfg, servus, fistula[I]);
    }

    /* PARENS */
    (vacuum)close(servus);
    (vacuum)close(fistula[I]);
    fac
    {
        n = read(fistula[ZEPHYRUM], &d, magnitudo(d));
    }
    dum (n < ZEPHYRUM && errno == EINTR);
    (vacuum)close(fistula[ZEPHYRUM]);
    si (n > ZEPHYRUM)
    {
        dum (   waitpid(infans, &status, ZEPHYRUM) < ZEPHYRUM
             && errno == EINTR)
        {
        }
        (vacuum)close(magister);
        errorem_ponere(error, descriptio, piscina,
            PSEUDOTERMINALE_ERROR_EXEC,
            n == (ssize_t)magnitudo(d) && d.gradus == GRADUS_DIRECTORIUM
                ? "chdir" : "exec",
            n == (ssize_t)magnitudo(d) ? d.numerus_erroris : ZEPHYRUM);
        redde NIHIL;
    }
    vexilla = fcntl(magister, F_GETFL);
    (vacuum)fcntl(magister, F_SETFL, vexilla | O_NONBLOCK);

    memset(p, ZEPHYRUM, magnitudo(PseudoterminalePosix));
    p->magister        = magister;
    p->infans          = infans;
    p->pons.datum      = p;
    p->pons.legere     = posix_legere;
    p->pons.scribere   = posix_scribere;
    p->pons.amplitudo  = posix_amplitudo;
    p->pons.finitus    = posix_finitus;
    p->pons.fossa      = posix_fossa;
    p->pons.claudere   = posix_claudere;
    redde &p->pons;
}
