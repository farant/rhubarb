/* sonda r2: responsum_aptare buffer (mensura*II + w*IV + LXIV) a
 * dispositione pendente superatur */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "vicus_applicatio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

integer
principale (integer argc, character** argv)
{
    memoriae_index alveus = argc > I ? (memoriae_index)atoi(argv[I]) : 65536;
    Piscina*  piscina = piscina_generare_dynamicum("sonda", alveus);
    character textus[CXL];
    chorda    c;
    chorda    r;
    s32       j;

    per (j = 0; j < XXXIV; j++) textus[j] = ' ';
    per (j = XXXIV; j < CXXXIV; j++) textus[j] = 'x';
    textus[CXXXIV] = '\0';
    c = chorda_ex_literis(textus, piscina);
    imprimere("alveus %lu, input %u, buffer allocatum %d\n",
        (unsigned long)alveus, (insignatus integer)c.mensura,
        (integer)c.mensura * II + XL * IV + LXIV);
    fflush(stdout);
    r = vicus_quaestio_disponere(c, XL, C, piscina);
    imprimere("exitus mensura %u (incl. '\\n' praefixum)\n",
        (insignatus integer)r.mensura);
    imprimere("[%.*s]\n", (integer)(r.mensura > 200 ? 200 : r.mensura),
        (constans character*)r.datum);
    piscina_destruere(piscina);
    redde 0;
}
