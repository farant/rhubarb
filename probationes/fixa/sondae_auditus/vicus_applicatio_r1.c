/* sonda r1: responsum_disponere pendet si indentatio >= latitudo interior */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "vicus_applicatio.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

interior vacuum
alarma (integer s)
{
    (vacuum)s;
    imprimere("PENDET: alarma post II s - dum (a < b) numquam finit\n");
    fflush(stdout);
    _exit(VII);
}

integer
principale (vacuum)
{
    Piscina*  piscina = piscina_generare_dynamicum("sonda", 65536);
    character textus[LXIV];
    chorda    c;
    chorda    r;
    s32       j;

    /* casus sanus primum: indentatio parva */
    r = vicus_quaestio_disponere(chorda_ex_literis("  - verbum unum", piscina),
        XL, XX, piscina);
    imprimere("sanus: mensura %u [%.*s]\n", (insignatus integer)r.mensura,
        (integer)r.mensura, (constans character*)r.datum);

    per (j = 0; j < XL; j++) textus[j] = ' ';
    memcpy(textus + XL, "word", IV);
    textus[XLIV] = '\0';
    c = chorda_ex_literis(textus, piscina);
    signal(SIGALRM, alarma);
    alarm(II);
    imprimere("XL spatia + 'word', w=XL, l=XX ...\n");
    fflush(stdout);
    r = vicus_quaestio_disponere(c, XL, XX, piscina);
    alarm(0);
    imprimere("rediit: mensura %u\n", (insignatus integer)r.mensura);
    piscina_destruere(piscina);
    redde 0;
}
