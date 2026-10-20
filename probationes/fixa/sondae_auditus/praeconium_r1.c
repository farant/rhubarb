/* sonda: praeconium_via cum RHUBARB_COLLOQUIA="" et HOME longo */
#include "latina.h"
#include "piscina.h"
#include "praeconium.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

integer
principale (
    vacuum)
{
             Piscina* magna;
             Piscina* parva;
    constans character* via;
    hic_manens character domus_longa[] =
        "/Users/aliquis_cum_titulo_longissimo_valde/domus/profunda";

    setenv("HOME", domus_longa, I);
    setenv("RHUBARB_COLLOQUIA", "", I);
    imprimere("strlen(HOME) = %lu\n", (unsigned long)strlen(domus_longa));

    magna = piscina_generare_dynamicum("sonda", 65536);
    via = praeconium_via(magna);
    imprimere("via = %s\n", via);
    imprimere("octeti scripti = %lu, allocati = %lu (0 + 64)\n",
        (unsigned long)(strlen(via) + I), (unsigned long)LXIV);
    fflush(stdout);

    /* piscina parva: alveus 64 octetorum, ut ASan transgressionem videat */
    parva = piscina_generare_dynamicum("sonda_parva", LXIV);
    via = praeconium_via(parva);
    imprimere("parva via = %s\n", via);
    piscina_destruere(magna);
    piscina_destruere(parva);
    redde ZEPHYRUM;
}
