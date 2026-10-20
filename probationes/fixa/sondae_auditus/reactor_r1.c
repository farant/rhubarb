/* sonda: reactor_currere sine fd/timer - nectit an dormit? */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "reactor.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

hic_manens integer fossa_lectionis = -1;

/* callback: legit EOF, fossam removet (exemplar commune), sistere non vocat */
interior vacuum
_ad_legendum (integer fd, i32 eventus, vacuum* datum)
{
    character alveus[XVI];
    Reactor* r = (Reactor*)datum;
    (vacuum)eventus;
    (vacuum)read(fd, alveus, magnitudo(alveus));
    reactor_removere(r, fd);
}

interior vacuum
_experiri (integer modus)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Reactor* r = reactor_creare(p);
    pid_t filius;
    integer status;
    structura rusage usus;
    integer fossae[II];

    si (modus == I)
    {
        si (pipe(fossae) != 0) { imprimere("pipe fracta\n"); redde; }
        close(fossae[I]); /* EOF statim */
        fossa_lectionis = fossae[0];
        reactor_adicere(r, fossae[0], REACTOR_LEGERE, _ad_legendum, r);
    }

    filius = fork();
    si (filius == 0)
    {
        reactor_currere(r);
        _exit(0);
    }
    usleep(1500000);
    kill(filius, SIGKILL);
    wait4(filius, &status, 0, &usus);
    imprimere("modus %d: filius CPU usor %ld.%06ld s + systema %ld.%06ld s in ~1.5 s parietis\n",
        modus,
        (longus)usus.ru_utime.tv_sec, (longus)usus.ru_utime.tv_usec,
        (longus)usus.ru_stime.tv_sec, (longus)usus.ru_stime.tv_usec);
    si (modus == I) close(fossae[0]);
}

integer
principale (vacuum)
{
    _experiri(0);   /* reactor vacuus */
    _experiri(I);   /* fossa ultima in callback remota */
    redde 0;
}
