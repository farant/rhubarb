/* processus_posix.c - Implementatio generationis processuum pro POSIX
 *
 * Forma: pipe x III + fork + execvp + ansa select() super duas
 * fistulas cum termino temporis. Fistula TERTIA (FD_CLOEXEC) fert
 * errorem exec: si exec succedit fistula clauditur vacua; si fallit
 * infans errno scribit. Ita "binarium abest" a "processus cucurrit
 * et codicem 127 reddidit" DISTINGUITUR - quod ssh momenti facit,
 * quia ssh ipse 127 reddit cum imperium REMOTUM deest.
 */

#include "processus.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

/* magnitudo lectionis unius; sacculus ab hoc incipit et duplicat */
#define PROCESSUS_FRUSTUM 4096

/* ========================================================================
 * FUNCTIONES INTERNAE
 * ======================================================================== */

interior i64
_tempus_ms (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (i64)tv.tv_sec * (i64)M + (i64)tv.tv_usec / (i64)M;
}

/* sacculus crescens in arena (duplicatio + copia - piscina
 * liberationem non habet, ergo capacitas sola crescit) */
nomen structura {
    i8* datum;
    i32 mensura;
    i32 capacitas;
} Sacculus;

interior vacuum
_sacculum_incipere (Sacculus* s)
{
    s->datum = NIHIL;
    s->mensura = ZEPHYRUM;
    s->capacitas = ZEPHYRUM;
}

interior b32
_sacculum_addere (Sacculus* s, constans i8* fons, i32 numerus,
    Piscina* piscina)
{
    si (numerus <= ZEPHYRUM)
    {
        redde VERUM;
    }
    si (s->mensura + numerus > s->capacitas)
    {
        i32 nova_capacitas = (s->capacitas > ZEPHYRUM)
            ? s->capacitas : (i32)PROCESSUS_FRUSTUM;
        i8* nova_datum;

        dum (nova_capacitas < s->mensura + numerus)
        {
            nova_capacitas *= II;
        }
        nova_datum = (i8*)piscina_allocare(piscina,
            (memoriae_index)nova_capacitas);
        si (nova_datum == NIHIL)
        {
            redde FALSUM;
        }
        si (s->mensura > ZEPHYRUM)
        {
            memcpy(nova_datum, s->datum, (memoriae_index)s->mensura);
        }
        s->datum = nova_datum;
        s->capacitas = nova_capacitas;
    }
    memcpy(s->datum + s->mensura, fons, (memoriae_index)numerus);
    s->mensura += numerus;
    redde VERUM;
}

interior chorda
_sacculum_finire (constans Sacculus* s)
{
    chorda c;

    c.datum = s->datum;
    c.mensura = s->mensura;
    redde c;
}

interior vacuum
_non_blocantem_ponere (integer fd)
{
    integer vexilla = fcntl(fd, F_GETFL, 0);

    si (vexilla >= 0)
    {
        (vacuum)fcntl(fd, F_SETFL, vexilla | O_NONBLOCK);
    }
}

interior ProcessusResultus
_resultus_vacuus (vacuum)
{
    ProcessusResultus r;

    r.successus = FALSUM;
    r.codex_exitus = ZEPHYRUM;
    r.signum = ZEPHYRUM;
    r.effusio.datum = NIHIL;
    r.effusio.mensura = ZEPHYRUM;
    r.erratum.datum = NIHIL;
    r.erratum.mensura = ZEPHYRUM;
    r.mora_ms = ZEPHYRUM;
    r.error = PROCESSUS_OK;
    r.error_descriptio.datum = NIHIL;
    r.error_descriptio.mensura = ZEPHYRUM;
    redde r;
}

interior ProcessusResultus
_error_reddere (ProcessusError error, constans character* nuntius,
    Piscina* piscina)
{
    ProcessusResultus r = _resultus_vacuus();

    r.error = error;
    si (nuntius != NIHIL && piscina != NIHIL)
    {
        r.error_descriptio = chorda_ex_literis(nuntius, piscina);
    }
    redde r;
}

/* ========================================================================
 * FUNCTIONES PUBLICAE
 * ======================================================================== */

constans character*
processus_error_nomen (ProcessusError error)
{
    commutatio (error)
    {
        casus PROCESSUS_OK:                redde "nullus";
        casus PROCESSUS_ERROR_ARGUMENTA:   redde "argumenta invalida";
        casus PROCESSUS_ERROR_GENERARE:    redde "generatio fallita";
        casus PROCESSUS_ERROR_EXEC:        redde "binarium non exsecutabile";
        casus PROCESSUS_ERROR_TEMPUS:      redde "mora excessa";
        casus PROCESSUS_ERROR_IO:          redde "defectus I/O";
        ordinarius:                        redde "ignotus";
    }
}

ProcessusResultus
processus_exsequi (constans character* constans* argumenta,
    i32 mora_maxima_ms, Piscina* piscina)
{
    integer fistula_ef[II];
    integer fistula_er[II];
    integer fistula_exec[II];
    pid_t pid;
    Sacculus sac_ef;
    Sacculus sac_er;
    ProcessusResultus r;
    i64 initium;
    integer exec_errno = ZEPHYRUM;
    b32 memoria_fracta = FALSUM;
    b32 tempus_excessum = FALSUM;
    integer status = ZEPHYRUM;

    si (piscina == NIHIL)
    {
        redde _error_reddere(PROCESSUS_ERROR_ARGUMENTA,
            "piscina requiritur", NIHIL);
    }
    si (argumenta == NIHIL || argumenta[0] == NIHIL)
    {
        redde _error_reddere(PROCESSUS_ERROR_ARGUMENTA,
            "vector argumentorum vacuus", piscina);
    }

    si (pipe(fistula_ef) != 0)
    {
        redde _error_reddere(PROCESSUS_ERROR_GENERARE,
            "fistula effusionis fracta", piscina);
    }
    si (pipe(fistula_er) != 0)
    {
        close(fistula_ef[0]);
        close(fistula_ef[I]);
        redde _error_reddere(PROCESSUS_ERROR_GENERARE,
            "fistula errati fracta", piscina);
    }
    si (pipe(fistula_exec) != 0)
    {
        close(fistula_ef[0]);
        close(fistula_ef[I]);
        close(fistula_er[0]);
        close(fistula_er[I]);
        redde _error_reddere(PROCESSUS_ERROR_GENERARE,
            "fistula exec fracta", piscina);
    }
    /* CLOEXEC: exec felix fistulam tacite claudit = signum */
    (vacuum)fcntl(fistula_exec[I], F_SETFD, FD_CLOEXEC);

    initium = _tempus_ms();
    pid = fork();
    si (pid < 0)
    {
        close(fistula_ef[0]);   close(fistula_ef[I]);
        close(fistula_er[0]);   close(fistula_er[I]);
        close(fistula_exec[0]); close(fistula_exec[I]);
        redde _error_reddere(PROCESSUS_ERROR_GENERARE,
            "furca fracta", piscina);
    }

    si (pid == ZEPHYRUM)
    {
        /* INFANS - numquam ad vocantem redit */
        unio {
            constans character* constans* c;
            character* constans*          m;
        } u;
        integer e;

        close(fistula_ef[0]);
        close(fistula_er[0]);
        close(fistula_exec[0]);
        si (dup2(fistula_ef[I], STDOUT_FILENO) < 0
            || dup2(fistula_er[I], STDERR_FILENO) < 0)
        {
            _exit(CXXVII);
        }
        close(fistula_ef[I]);
        close(fistula_er[I]);

        u.c = argumenta;
        (vacuum)execvp(argumenta[0], u.m);

        /* exec fefellit: errno per fistulam tertiam */
        e = errno;
        (vacuum)write(fistula_exec[I], &e, magnitudo(e));
        _exit(CXXVII);
    }

    /* PARENS */
    close(fistula_ef[I]);
    close(fistula_er[I]);
    close(fistula_exec[I]);
    _non_blocantem_ponere(fistula_ef[0]);
    _non_blocantem_ponere(fistula_er[0]);
    _sacculum_incipere(&sac_ef);
    _sacculum_incipere(&sac_er);

    {
        b32 ef_apertus = VERUM;
        b32 er_apertus = VERUM;

        dum (ef_apertus || er_apertus)
        {
            fd_set legendi;
            structura timeval mora;
            structura timeval* mora_ptr = NIHIL;
            integer maximus = -I;
            integer paratus;

            FD_ZERO(&legendi);
            si (ef_apertus)
            {
                FD_SET(fistula_ef[0], &legendi);
                si (fistula_ef[0] > maximus) maximus = fistula_ef[0];
            }
            si (er_apertus)
            {
                FD_SET(fistula_er[0], &legendi);
                si (fistula_er[0] > maximus) maximus = fistula_er[0];
            }
            si (mora_maxima_ms > ZEPHYRUM)
            {
                i64 reliquum = (i64)mora_maxima_ms
                    - (_tempus_ms() - initium);

                si (reliquum <= (i64)ZEPHYRUM)
                {
                    tempus_excessum = VERUM;
                    frange;
                }
                /* tv_usec est int in Darwin, longus alibi - conversio
                 * per typum campi ipsius, non per typum coniectum */
                mora.tv_sec = (time_t)(reliquum / (i64)M);
                mora.tv_usec = (integer)((reliquum % (i64)M) * (i64)M);
                mora_ptr = &mora;
            }

            paratus = select(maximus + I, &legendi, NIHIL, NIHIL,
                mora_ptr);
            si (paratus < 0)
            {
                si (errno == EINTR)
                {
                    perge;
                }
                frange;
            }
            si (paratus == ZEPHYRUM)
            {
                tempus_excessum = VERUM;
                frange;
            }
            si (ef_apertus && FD_ISSET(fistula_ef[0], &legendi))
            {
                i8 frustum[PROCESSUS_FRUSTUM];
                ssize_t n = read(fistula_ef[0], frustum,
                    (memoriae_index)PROCESSUS_FRUSTUM);

                si (n > 0)
                {
                    si (!_sacculum_addere(&sac_ef, frustum, (i32)n,
                            piscina))
                    {
                        memoria_fracta = VERUM;
                        frange;
                    }
                }
                alioquin si (n == 0)
                {
                    ef_apertus = FALSUM;
                }
                alioquin si (errno != EAGAIN && errno != EINTR)
                {
                    ef_apertus = FALSUM;
                }
            }
            si (er_apertus && FD_ISSET(fistula_er[0], &legendi))
            {
                i8 frustum[PROCESSUS_FRUSTUM];
                ssize_t n = read(fistula_er[0], frustum,
                    (memoriae_index)PROCESSUS_FRUSTUM);

                si (n > 0)
                {
                    si (!_sacculum_addere(&sac_er, frustum, (i32)n,
                            piscina))
                    {
                        memoria_fracta = VERUM;
                        frange;
                    }
                }
                alioquin si (n == 0)
                {
                    er_apertus = FALSUM;
                }
                alioquin si (errno != EAGAIN && errno != EINTR)
                {
                    er_apertus = FALSUM;
                }
            }
        }
    }

    si (tempus_excessum || memoria_fracta)
    {
        (vacuum)kill(pid, SIGKILL);
    }
    close(fistula_ef[0]);
    close(fistula_er[0]);

    /* errorem exec legere (fistula iam clausa si exec successit) */
    {
        ssize_t n = read(fistula_exec[0], &exec_errno,
            magnitudo(exec_errno));

        si (n != (ssize_t)magnitudo(exec_errno))
        {
            exec_errno = ZEPHYRUM;
        }
    }
    close(fistula_exec[0]);

    dum (waitpid(pid, &status, 0) < 0)
    {
        si (errno != EINTR)
        {
            status = ZEPHYRUM;
            frange;
        }
    }

    r = _resultus_vacuus();
    r.effusio = _sacculum_finire(&sac_ef);
    r.erratum = _sacculum_finire(&sac_er);
    r.mora_ms = (i32)(_tempus_ms() - initium);

    si (exec_errno != ZEPHYRUM)
    {
        r.error = PROCESSUS_ERROR_EXEC;
        r.error_descriptio = chorda_ex_literis(strerror(exec_errno),
            piscina);
        redde r;
    }
    si (memoria_fracta)
    {
        r.error = PROCESSUS_ERROR_IO;
        r.error_descriptio = chorda_ex_literis("piscina exhausta",
            piscina);
        redde r;
    }
    si (tempus_excessum)
    {
        r.error = PROCESSUS_ERROR_TEMPUS;
        r.error_descriptio = chorda_ex_literis(
            "mora maxima excessa - processus occisus", piscina);
        redde r;
    }

    r.successus = VERUM;
    si (WIFEXITED(status))
    {
        r.codex_exitus = (i32)WEXITSTATUS(status);
    }
    alioquin si (WIFSIGNALED(status))
    {
        r.signum = (i32)WTERMSIG(status);
    }
    redde r;
}
