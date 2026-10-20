/* sonda flatura r3: distantia ante initium output non reicitur */
#include "latina.h"
#include "piscina.h"
#include "flatura.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina*       piscina;
    FlaturaFructus fr;
    hic_manens constans i8 rivus[3] = { 0x03, 0x02, 0x00 };
    i32 k;

    piscina = piscina_generare_dynamicum("sonda", 65536);
    /* fixed block: primum symbolum longitudo 3 distantia 1, deinde EOB */
    fr = flatura_inflare(rivus, 3, piscina);
    imprimere("status=%d (%s) mensura=%d\n", (int)fr.status,
        flatura_status_nuntium(fr.status), (int)fr.mensura);
    per (k = 0; fr.datum != NIHIL && k < fr.mensura; k++)
    {
        imprimere("  octetus[%d]=0x%02x\n", (int)k, (unsigned)(fr.datum[k] & 0xFF));
    }
    piscina_destruere(piscina);
    redde 0;
}
