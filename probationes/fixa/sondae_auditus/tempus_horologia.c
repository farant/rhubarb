#include "postulata_posix.h"
#include "latina.h"
#include "tempus.h"
#include "piscina.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Tempus* t; Tempus* g; StatisticaeQuadri* s; int i; pid_t pid; int st;

    /* before initiare */
    printf("ante initiare: nunc=%g delta=%g\n", tempus_nunc(), tempus_delta());
    s = tempus_obtinere_statisticas();
    printf("ante quadrum: n=%lu min=%g max=%g media=%g currentia=%g destinata=%g\n",
        (unsigned long)s->numerus_quadrorum, s->tempus_minimum_quadri, s->tempus_maximum_quadri,
        s->quadra_media_per_secundum, s->quadra_currentia_per_secundum, s->quadra_destinata_per_secundum);

    /* reinit resets target */
    tempus_ponere_quadra_destinata(30.0);
    tempus_initiare();
    printf("post ponere(30)+initiare: destinata=%g\n", tempus_obtinere_statisticas()->quadra_destinata_per_secundum);

    /* NIHIL */
    printf("creare(NIHIL)=%p elapsum(NIHIL)=%g\n", (void*)tempus_creare(NIHIL), tempus_elapsum(NIHIL));
    tempus_restituere(NIHIL); tempus_actualizare(NIHIL);

    /* fresh Tempus */
    t = tempus_creare(p);
    printf("creatum: delta=%g totale=%g accum=%g freq=%g\n", t->delta_tempus, t->tempus_totale, t->accumulator, t->frequentia);
    {
        f64 summa = 0.0, el;
        for (i = 0; i < 1000; i++) { tempus_actualizare(t); summa += t->delta_tempus; }
        el = t->tempus_totale;
        printf("summa delta=%.12g totale=%.12g diff=%.3g accumulator=%g\n", summa, el, summa - el, t->accumulator);
    }

    /* global aliasing via restituere */
    tempus_quadrum(); usleep(2000); tempus_quadrum();
    g = tempus_obtinere_horologium();
    printf("global antequam restituere: totale=%g delta=%g\n", g->tempus_totale, g->delta_tempus);
    tempus_restituere(g);
    printf("global post restituere: totale=%g nunc=%g\n", g->tempus_totale, tempus_nunc());

    /* exhausted fixed piscina */
    fflush(stdout);
    pid = fork();
    if (pid == 0) {
        Piscina* f = piscina_generare_certae_magnitudinis("certa", 16);
        Tempus* r = tempus_creare(f);
        printf("  creare(piscina plena) = %p\n", (void*)r);
        fflush(stdout);
        _exit(0);
    }
    waitpid(pid, &st, 0);
    printf("  filius: exitus=%d (0 = NIHIL redditum; 1 = processus exiit)\n", WIFEXITED(st) ? WEXITSTATUS(st) : -1);

    /* non-monotonic: global state edited via pointer */
    g->pulsus_ultimi_quadri += 1000000; tempus_quadrum();
    printf("ultimi_quadri in futuro: delta=%g\n", tempus_delta());
    return 0;
}
