/* sonda: '-1 si error' ad litteram sub vexillis domus */
#include "latina.h"
#include "piscina.h"
#include "reactor.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Reactor* r = reactor_creare(p);
    ReactorTimerId id = reactor_timer_adicere(r, X, FALSUM, NIHIL, NIHIL);
    si (id < 0) { imprimere("error deprehensus per < 0\n"); }
    alioquin { imprimere("error NON deprehensus per < 0 (id=%lu)\n", (insignatus longus)id); }
    redde 0;
}
