/* fragmenta.c - a briar genitum ex briar/probationes/fixa/thistle/fragmenta.thistle: principale */
#include "latina.h"
#include "fragmenta_regiones.h"
#line 44 "briar/probationes/fixa/thistle/fragmenta.thistle"
s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("fragmenta", 65536);
    i32 numeri[3];
#line 24 "briar/probationes/fixa/thistle/fragmenta.thistle"
    chorda salutatio = chorda_ex_literis("salve, fragmenta", piscina);
#line 50 "briar/probationes/fixa/thistle/fragmenta.thistle"

    numeri[0] = I;
    numeri[1] = II;
    numeri[2] = III;
    imprimere("%.*s: %d\n", (integer)salutatio.mensura,
        (constans character*)salutatio.datum,
        (integer)summare(numeri, III));
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
