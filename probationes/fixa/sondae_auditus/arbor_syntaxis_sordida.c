#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "arbor_syntaxis.h"
#include <stdio.h>
#include <string.h>

s32
principale (vacuum)
{
    Piscina*              p = piscina_generare_dynamicum("sonda", 1048576);
    InternamentumChorda*  intern = internamentum_creare(p);
    ArborSyntaxis*        syn;
    ArborSyntaxisResultus res;
    vacuum*               sordes;
    PiscinaNotatio        nota;
    constans character*   fons = "int f(void) { return 1 + 2 * 3; }";

    /* memoria sordida: allocare, 0x11 implere (0xFF in ++ ad nihil involveretur), reficere - proxima
     * allocatio eosdem octetos accipit (sine sanitatoribus quoque) */
    nota = piscina_notare(p);
    sordes = piscina_allocare(p, 4096);
    memset(sordes, 0x11, 4096);
    piscina_reficere(p, nota);
    syn = arbor_syntaxis_creare(p, intern);
    res = arbor_syntaxis_parsere_fontem(syn, fons, (i32)strlen(fons), "sordida.c");
    printf("successus=%d radix=%s\n", (int)res.successus, res.radix ? "adest" : "NIHIL");
    redde res.successus ? ZEPHYRUM : I;
}
