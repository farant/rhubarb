/* processus_posix.c - Implementatio generationis processuum pro POSIX
 *
 * Forma: pipe x III + fork + execvp + ansa select() super duas
 * fistulas cum termino temporis. Fistula TERTIA (FD_CLOEXEC) fert
 * errorem exec: si exec succedit fistula clauditur vacua; si fallit
 * infans errno scribit. Ita "binarium abest" a "processus cucurrit
 * et codicem 127 reddidit" DISTINGUITUR - quod ssh momenti facit,
 * quia ssh ipse 127 reddit cum imperium REMOTUM deest.
 *
 * DUAE SEMITAE, ANSA UNA (2026-07-24): omnis status ansae in
 * structura Processus habitat, et _ansam_pulsare gradum UNUM agit.
 * processus_exsequi eam OBSTANTER in ansa vocat; processus_pulsare
 * eam SEMEL non-obstanter. Nullum exemplar alterum ansae select()
 * exsistit quod dissidere possit.
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

/* Frusta maxima quae pulsus UNUS haurit. Sine termino infans
 * effundens pulsum in aeternum teneret (id est, faciem congelaret -
 * quod tota ratio semitae incrementalis est vitare). Cum eo pulsus
 * ad summum CCLVI KiB agit et redit. */
#define PROCESSUS_FRUSTA_PER_PULSUM LXIV

/* ========================================================================
 * STATUS
 * ======================================================================== */

/* sacculus crescens in arena (duplicatio + copia - piscina
 * liberationem non habet, ergo capacitas sola crescit) */
nomen structura {
    i8* datum;
    i32 mensura;
    i32 capacitas;
} Sacculus;

structura Processus {
    Piscina* piscina;
    pid_t    pid;

    /* fines LEGENDI; -I = clausum (custos contra clausuram duplam) */
    integer  fd_ef;
    integer  fd_er;
    integer  fd_exec;

    Sacculus sac_ef;
    Sacculus sac_er;

    i64      initium;
    i32      mora_maxima_ms;
    i32      mora_ms;

    b32      ef_apertus;
    b32      er_apertus;
    b32      memoria_fracta;
    b32      tempus_excessum;
    b32      abruptus;
    b32      occisus;
    b32      messus;
    b32      perfectus;

    integer  status;
    integer  exec_errno;

    /* error ANTE generationem (argumenta prava, furca fracta).
     * Manubrium validum manet et statim PARATUS est, ut vocator
     * semitam UNAM tractet - vide caput. Nuntius litterae STATICAE
     * sunt, ergo nulla allocatio in semita erroris. */
    ProcessusError      error_initialis;
    constans character* nuntius_initialis;
};

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

interior vacuum
_fd_claudere (integer* fd)
{
    si (*fd >= 0)
    {
        close(*fd);
        *fd = -I;
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

/* fistulam unam haurire donec EEXHAURIATUR aut terminus frustorum
 * attingatur. Fistulae iam non-blocantes sunt, ergo lectio repetita
 * tuta est - et pulsum UNUM multa frusta haurire sinit potius quam
 * IV KiB per tictum (quod effusionem magnam per multos tictus
 * traheret). */
interior vacuum
_fistulam_haurire (Processus* p, integer fd, b32* apertus,
    Sacculus* sac)
{
    i32 frusta = ZEPHYRUM;

    dum (*apertus && frusta < (i32)PROCESSUS_FRUSTA_PER_PULSUM)
    {
        i8      frustum[PROCESSUS_FRUSTUM];
        ssize_t n = read(fd, frustum, (memoriae_index)PROCESSUS_FRUSTUM);

        si (n > 0)
        {
            si (!_sacculum_addere(sac, frustum, (i32)n, p->piscina))
            {
                p->memoria_fracta = VERUM;
                *apertus = FALSUM;
                redde;
            }
            frusta++;
            perge;
        }
        si (n == 0)
        {
            *apertus = FALSUM;   /* EOF verus */
            redde;
        }
        si (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            redde;               /* exhausta in praesenti - normale */
        }
        si (errno == EINTR)
        {
            perge;
        }
        *apertus = FALSUM;       /* error verus */
        redde;
    }
}

/* GRADUS UNUS ansae - corpus quod AMBAE semitae communicant.
 *
 * obstans = VERUM  : select obstat usque ad terminum totalem (aut
 *                    in aeternum si terminus abest) - mos
 *                    processus_exsequi, immutatus.
 * obstans = FALSUM : select cum mora ZEPHYRI - pulsus qui numquam
 *                    obstat.
 *
 * CAUTIO: 'select redidit ZEPHYRUM' significat TERMINUM EXCESSUM
 * solum in modo obstante. In modo pulsante idem valorem significat
 * 'nihil paratum nunc' - quod normalissimum est. Confundere ea
 * omnem probationem post pulsum primum tempore excedi faceret.
 */
interior vacuum
_ansam_pulsare (Processus* p, b32 obstans)
{
    fd_set             legendi;
    structura timeval  mora;
    structura timeval* mora_ptr = NIHIL;
    integer            maximus = -I;
    integer            paratus;

    si (!p->ef_apertus && !p->er_apertus)
    {
        redde;
    }

    FD_ZERO(&legendi);
    si (p->ef_apertus)
    {
        FD_SET(p->fd_ef, &legendi);
        si (p->fd_ef > maximus)
        {
            maximus = p->fd_ef;
        }
    }
    si (p->er_apertus)
    {
        FD_SET(p->fd_er, &legendi);
        si (p->fd_er > maximus)
        {
            maximus = p->fd_er;
        }
    }

    si (obstans)
    {
        si (p->mora_maxima_ms > ZEPHYRUM)
        {
            i64 reliquum = (i64)p->mora_maxima_ms
                - (_tempus_ms() - p->initium);

            si (reliquum <= (i64)ZEPHYRUM)
            {
                p->tempus_excessum = VERUM;
                redde;
            }
            /* tv_usec est int in Darwin, longus alibi - conversio
             * per typum campi ipsius, non per typum coniectum */
            mora.tv_sec = (time_t)(reliquum / (i64)M);
            mora.tv_usec = (integer)((reliquum % (i64)M) * (i64)M);
            mora_ptr = &mora;
        }
        /* alioquin mora_ptr = NIHIL: in aeternum, ut ante */
    }
    alioquin
    {
        /* terminus custoditur ETIAM in modo pulsante - alioquin
         * vocator qui tarde pulsat processum aeternum pareret */
        si (p->mora_maxima_ms > ZEPHYRUM
            && (_tempus_ms() - p->initium)
                >= (i64)p->mora_maxima_ms)
        {
            p->tempus_excessum = VERUM;
            redde;
        }
        mora.tv_sec = (time_t)ZEPHYRUM;
        mora.tv_usec = ZEPHYRUM;
        mora_ptr = &mora;
    }

    paratus = select(maximus + I, &legendi, NIHIL, NIHIL, mora_ptr);
    si (paratus < 0)
    {
        si (errno == EINTR)
        {
            redde;   /* gradus proximus retentat */
        }
        p->ef_apertus = FALSUM;
        p->er_apertus = FALSUM;
        redde;
    }
    si (paratus == ZEPHYRUM)
    {
        si (obstans)
        {
            p->tempus_excessum = VERUM;
        }
        redde;
    }

    si (p->ef_apertus && FD_ISSET(p->fd_ef, &legendi))
    {
        _fistulam_haurire(p, p->fd_ef, &p->ef_apertus, &p->sac_ef);
    }
    si (p->er_apertus && FD_ISSET(p->fd_er, &legendi))
    {
        _fistulam_haurire(p, p->fd_er, &p->er_apertus, &p->sac_er);
    }
    si (p->memoria_fracta)
    {
        p->ef_apertus = FALSUM;
        p->er_apertus = FALSUM;
    }
}

/* infantem metere. obstans=FALSUM adhibet WNOHANG.
 * Redde: VERUM si messus (aut nihil metendum). */
interior b32
_reficere (Processus* p, b32 obstans)
{
    si (p->messus)
    {
        redde VERUM;
    }
    si (p->pid <= 0)
    {
        p->messus = VERUM;
        redde VERUM;
    }
    dum (VERUM)
    {
        integer status = ZEPHYRUM;
        pid_t   r = waitpid(p->pid, &status,
            obstans ? 0 : WNOHANG);

        si (r == p->pid)
        {
            p->status = status;
            p->messus = VERUM;
            redde VERUM;
        }
        si (r == (pid_t)ZEPHYRUM)
        {
            redde FALSUM;   /* WNOHANG: adhuc currit */
        }
        si (errno == EINTR)
        {
            perge;
        }
        p->status = ZEPHYRUM;
        p->messus = VERUM;
        redde VERUM;
    }
}

/* errorem exec legere. VOCANDA POST MESSEM SOLUM: fistula exec
 * non-blocans NON est, sed infans messus scriptorem suum iam
 * clausit, ergo lectio statim redit (datum aut EOF). Ante messem
 * eadem lectio OBSTARET - quod totam semitam incrementalem
 * everteret. */
interior vacuum
_exec_errno_legere (Processus* p)
{
    ssize_t n;

    si (p->fd_exec < 0)
    {
        redde;
    }
    n = read(p->fd_exec, &p->exec_errno, magnitudo(p->exec_errno));
    si (n != (ssize_t)magnitudo(p->exec_errno))
    {
        p->exec_errno = ZEPHYRUM;
    }
    _fd_claudere(&p->fd_exec);
}

/* processum ad finem ducere. obstans=FALSUM redit FALSUM si nondum
 * paratus est. */
interior b32
_perficere (Processus* p, b32 obstans)
{
    si (p->perfectus)
    {
        redde VERUM;
    }
    /* CONDICIO TERMINALIS iam nota => NOLI haurire.
     * 'abruptus' hic esse DEBET, non solum tempus/memoria: sine eo
     * abrumpere in haustum obstantem cadit qui EOF exspectat, et
     * EOF nonnisi cum infans sponte finit venit - id est, abrumpere
     * moram integram infantis exspectaret, quod contrarium est eius
     * quod pollicetur (probatio XII id cepit: 'sleep 10' abruptum
     * decem secunda tenuit). */
    si ((p->ef_apertus || p->er_apertus)
        && !p->tempus_excessum && !p->memoria_fracta && !p->abruptus)
    {
        si (!obstans)
        {
            redde FALSUM;
        }
        dum ((p->ef_apertus || p->er_apertus)
            && !p->tempus_excessum && !p->memoria_fracta)
        {
            _ansam_pulsare(p, VERUM);
        }
    }
    si ((p->tempus_excessum || p->memoria_fracta || p->abruptus)
        && !p->occisus)
    {
        (vacuum)kill(p->pid, SIGKILL);
        p->occisus = VERUM;
    }
    /* fistulas claudere ANTE messem: infans in scriptione haerens
     * (fistula plena, nemo legens) aliter waitpid in aeternum
     * teneret */
    _fd_claudere(&p->fd_ef);
    _fd_claudere(&p->fd_er);
    p->ef_apertus = FALSUM;
    p->er_apertus = FALSUM;

    si (!_reficere(p, obstans))
    {
        redde FALSUM;
    }
    _exec_errno_legere(p);
    p->mora_ms = (i32)(_tempus_ms() - p->initium);
    p->perfectus = VERUM;
    redde VERUM;
}

interior ProcessusResultus
_resultus_aedificare (Processus* p)
{
    ProcessusResultus r = _resultus_vacuus();

    si (p->error_initialis != PROCESSUS_OK)
    {
        r.error = p->error_initialis;
        si (p->nuntius_initialis != NIHIL)
        {
            r.error_descriptio = chorda_ex_literis(
                p->nuntius_initialis, p->piscina);
        }
        redde r;
    }

    r.effusio = _sacculum_finire(&p->sac_ef);
    r.erratum = _sacculum_finire(&p->sac_er);
    r.mora_ms = p->mora_ms;

    si (p->exec_errno != ZEPHYRUM)
    {
        r.error = PROCESSUS_ERROR_EXEC;
        r.error_descriptio = chorda_ex_literis(
            strerror(p->exec_errno), p->piscina);
        redde r;
    }
    si (p->memoria_fracta)
    {
        r.error = PROCESSUS_ERROR_IO;
        r.error_descriptio = chorda_ex_literis("piscina exhausta",
            p->piscina);
        redde r;
    }
    si (p->abruptus)
    {
        r.error = PROCESSUS_ERROR_TEMPUS;
        r.error_descriptio = chorda_ex_literis(
            "abruptus a vocatore - processus occisus", p->piscina);
        redde r;
    }
    si (p->tempus_excessum)
    {
        r.error = PROCESSUS_ERROR_TEMPUS;
        r.error_descriptio = chorda_ex_literis(
            "mora maxima excessa - processus occisus", p->piscina);
        redde r;
    }

    r.successus = VERUM;
    si (WIFEXITED(p->status))
    {
        r.codex_exitus = (i32)WEXITSTATUS(p->status);
    }
    alioquin si (WIFSIGNALED(p->status))
    {
        r.signum = (i32)WTERMSIG(p->status);
    }
    redde r;
}

interior Processus*
_manubrium_novum (Piscina* piscina)
{
    Processus* p = (Processus*)piscina_allocare(piscina,
        magnitudo(Processus));

    si (p == NIHIL)
    {
        redde NIHIL;
    }
    memset(p, 0, magnitudo(Processus));
    p->piscina = piscina;
    p->pid = (pid_t)-I;
    p->fd_ef = -I;
    p->fd_er = -I;
    p->fd_exec = -I;
    _sacculum_incipere(&p->sac_ef);
    _sacculum_incipere(&p->sac_er);
    p->error_initialis = PROCESSUS_OK;
    redde p;
}

interior Processus*
_manubrium_fractum (Piscina* piscina, ProcessusError error,
    constans character* nuntius)
{
    Processus* p = _manubrium_novum(piscina);

    si (p == NIHIL)
    {
        redde NIHIL;
    }
    p->error_initialis = error;
    p->nuntius_initialis = nuntius;
    p->perfectus = VERUM;
    p->messus = VERUM;
    redde p;
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

Processus*
processus_incipere (constans character* constans* argumenta,
    i32 mora_maxima_ms, Piscina* piscina)
{
    Processus* p;
    integer    fistula_ef[II];
    integer    fistula_er[II];
    integer    fistula_exec[II];

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    si (argumenta == NIHIL || argumenta[0] == NIHIL)
    {
        redde _manubrium_fractum(piscina, PROCESSUS_ERROR_ARGUMENTA,
            "vector argumentorum vacuus");
    }
    p = _manubrium_novum(piscina);
    si (p == NIHIL)
    {
        redde NIHIL;
    }
    p->mora_maxima_ms = mora_maxima_ms;

    si (pipe(fistula_ef) != 0)
    {
        redde _manubrium_fractum(piscina, PROCESSUS_ERROR_GENERARE,
            "fistula effusionis fracta");
    }
    si (pipe(fistula_er) != 0)
    {
        close(fistula_ef[0]);
        close(fistula_ef[I]);
        redde _manubrium_fractum(piscina, PROCESSUS_ERROR_GENERARE,
            "fistula errati fracta");
    }
    si (pipe(fistula_exec) != 0)
    {
        close(fistula_ef[0]);
        close(fistula_ef[I]);
        close(fistula_er[0]);
        close(fistula_er[I]);
        redde _manubrium_fractum(piscina, PROCESSUS_ERROR_GENERARE,
            "fistula exec fracta");
    }
    /* CLOEXEC: exec felix fistulam tacite claudit = signum */
    (vacuum)fcntl(fistula_exec[I], F_SETFD, FD_CLOEXEC);

    p->initium = _tempus_ms();
    p->pid = fork();
    si (p->pid < 0)
    {
        close(fistula_ef[0]);   close(fistula_ef[I]);
        close(fistula_er[0]);   close(fistula_er[I]);
        close(fistula_exec[0]); close(fistula_exec[I]);
        redde _manubrium_fractum(piscina, PROCESSUS_ERROR_GENERARE,
            "furca fracta");
    }

    si (p->pid == (pid_t)ZEPHYRUM)
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
    p->fd_ef = fistula_ef[0];
    p->fd_er = fistula_er[0];
    p->fd_exec = fistula_exec[0];
    _non_blocantem_ponere(p->fd_ef);
    _non_blocantem_ponere(p->fd_er);
    p->ef_apertus = VERUM;
    p->er_apertus = VERUM;
    redde p;
}

ProcessusStatus
processus_pulsare (Processus* processus)
{
    si (processus == NIHIL || processus->perfectus)
    {
        redde PROCESSUS_PARATUS;
    }
    _ansam_pulsare(processus, FALSUM);
    si (_perficere(processus, FALSUM))
    {
        redde PROCESSUS_PARATUS;
    }
    redde PROCESSUS_CURRIT;
}

ProcessusResultus
processus_metere (Processus* processus)
{
    si (processus == NIHIL)
    {
        ProcessusResultus r = _resultus_vacuus();

        r.error = PROCESSUS_ERROR_ARGUMENTA;
        redde r;
    }
    si (processus->error_initialis == PROCESSUS_OK)
    {
        (vacuum)_perficere(processus, VERUM);
    }
    redde _resultus_aedificare(processus);
}

vacuum
processus_abrumpere (Processus* processus)
{
    si (processus == NIHIL || processus->perfectus)
    {
        redde;
    }
    processus->abruptus = VERUM;
    (vacuum)_perficere(processus, VERUM);
}

ProcessusResultus
processus_exsequi (constans character* constans* argumenta,
    i32 mora_maxima_ms, Piscina* piscina)
{
    Processus* p;

    si (piscina == NIHIL)
    {
        redde _error_reddere(PROCESSUS_ERROR_ARGUMENTA,
            "piscina requiritur", NIHIL);
    }
    p = processus_incipere(argumenta, mora_maxima_ms, piscina);
    si (p == NIHIL)
    {
        redde _error_reddere(PROCESSUS_ERROR_GENERARE,
            "manubrium allocari non potuit", piscina);
    }
    /* metere OBSTANTER: ansam eandem ad finem ducit */
    redde processus_metere(p);
}
