/* sonda: reactor_timer_adicere error - 'id >= 0' et '-1' ut caput dicit */
#include "latina.h"
#include "piscina.h"
#include "reactor.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Reactor* r = reactor_creare(p);
    ReactorTimerId id;
    integer valor_minus = -1;

    /* callback NIHIL: error per codicem */
    id = reactor_timer_adicere(r, X, FALSUM, NIHIL, NIHIL);
    imprimere("id = %lu (0x%lx)\n", (insignatus longus)id, (insignatus longus)id);
    imprimere("id >= 0 (successus secundum caput): %d\n", (integer)((s64)id >= 0));
    imprimere("id == (integer)-1 ut s64: %d\n", (integer)((s64)id == (s64)valor_minus));
    imprimere("id == REACTOR_TIMER_ERROR: %d\n", (integer)(id == REACTOR_TIMER_ERROR));
    redde 0;
}
