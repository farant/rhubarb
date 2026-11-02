/* veredarius_domesticus.h - veredarius super http_vocatio in reactore
 * (communio-plan-5 T1): macOS, Linux, instrumenta. Reactor a vocante
 * agitur (reactor_poll). Loci LXIV simul. */
#ifndef VEREDARIUS_DOMESTICUS_H
#define VEREDARIUS_DOMESTICUS_H
/* <aedilis corpus="lib/veredarius_domesticus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "reactor.h"
#include "veredarius.h"

/* in piscina (vivit cum ea); NIHIL si argumenta mala aut memoria */
Veredarius*
veredarium_domesticum_creare (
    Piscina* piscina,
    Reactor* reactor);

#endif /* VEREDARIUS_DOMESTICUS_H */
