/* probatio_fenestra_fossae.c - fossae ansam fenestrae expergefaciunt
 * (vicus quies T5). Sine fenestra: fenestra_expectare_eventus(NIHIL)
 * ansam solam exspectat. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "credo.h"
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* ms quos exspectatio duravit */
interior s64
exspectare_mensum (
    Mora ms_maximae)
{
    s64 initium;

    initium = fenestra_tempus_ms();
    fenestra_expectare_eventus(NIHIL, ms_maximae);
    redde fenestra_tempus_ms() - initium;
}

interior vacuum
exhaurire (
    integer fd)
{
    character capsa[XVI];

    (vacuum)read(fd, capsa, magnitudo(capsa));
}

/* filius: post ms scribit octetum unum, exit */
interior pid_t
scriptorem_differre (
    integer fd,
        s64 ms)
{
    pid_t filius;

    filius = fork();
    si (filius == ZEPHYRUM)
    {
        structura timespec t;

        t.tv_sec   = (time_t)(ms / M);
        t.tv_nsec  = (longus)((ms % M) * M * M);
        (vacuum)nanosleep(&t, NIHIL);
        (vacuum)write(fd, "x", I);
        _exit(ZEPHYRUM);
    }
    redde filius;
}

interior vacuum
_sectio_legere (
    CredoContextus* c)
{
          integer p[II];
    FenestraFossa f;
              s64 ms;
            pid_t filius;
          integer status;

    (vacuum)c;
    /* NSApp prima vocatione creatur (~70 ms): initium non mensuratur */
    fenestra_expectare_eventus(NIHIL, ZEPHYRUM);
    CREDO_VERUM(pipe(p) == ZEPHYRUM);
    f.fd       = p[0];
    f.eventus  = FENESTRA_FOSSA_LEGERE;
    CREDO_VERUM(fenestra_fossas_ponere(&f, I));

    CREDO_GRADUS("fossa iam legenda: exspectatio statim redit");
    (vacuum)write(p[I], "x", I);
    ms = exspectare_mensum(V * M);
    CREDO_VERUM(ms < C);

    CREDO_GRADUS("exhausta: mora tota (CC ms)");
    exhaurire(p[0]);
    ms = exspectare_mensum(CC);
    CREDO_VERUM(ms >= CL && ms < M);

    CREDO_GRADUS("filius post CL ms scribit: exspectatio excitatur");
    filius = scriptorem_differre(p[I], CL);
    CREDO_VERUM(filius > ZEPHYRUM);
    ms = exspectare_mensum(V * M);
    CREDO_VERUM(ms >= C && ms < M);
    (vacuum)waitpid(filius, &status, ZEPHYRUM);
    exhaurire(p[0]);

    CREDO_GRADUS("iterum exhausta: mora tota (rearmata, non spuria)");
    ms = exspectare_mensum(CC);
    CREDO_VERUM(ms >= CL && ms < M);

    CREDO_GRADUS("fossae sublatae: legenda non expergefacit");
    CREDO_VERUM(fenestra_fossas_ponere(NIHIL, ZEPHYRUM));
    (vacuum)write(p[I], "x", I);
    ms = exspectare_mensum(CC);
    CREDO_VERUM(ms >= CL && ms < M);

    close(p[0]);
    close(p[I]);
}

interior vacuum
_sectio_scribere (
    CredoContextus* c)
{
          integer p[II];
    FenestraFossa f;
              s64 ms;

    (vacuum)c;
    /* NSApp (ut supra) */
    fenestra_expectare_eventus(NIHIL, ZEPHYRUM);
    CREDO_VERUM(pipe(p) == ZEPHYRUM);

    CREDO_GRADUS("fossa scribenda (fistula vacua): statim redit");
    f.fd       = p[I];
    f.eventus  = FENESTRA_FOSSA_SCRIBERE;
    CREDO_VERUM(fenestra_fossas_ponere(&f, I));
    ms = exspectare_mensum(V * M);
    CREDO_VERUM(ms < C);

    CREDO_GRADUS("fossa legenda (nihil scriptum): mora tota");
    f.eventus  = FENESTRA_FOSSA_LEGERE;
    f.fd       = p[0];
    CREDO_VERUM(fenestra_fossas_ponere(&f, I));
    ms = exspectare_mensum(CC);
    CREDO_VERUM(ms >= CL && ms < M);

    CREDO_VERUM(fenestra_fossas_ponere(NIHIL, ZEPHYRUM));
    close(p[0]);
    close(p[I]);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "legere", _sectio_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "scribere", _sectio_scribere, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("fenestra_fossae", SECTIONES);
}
