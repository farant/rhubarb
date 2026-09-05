/* probatio_fragmenta.c - a briar genitum ex briar/probationes/fixa/thistle/fragmenta.thistle: regio munus="probatio" */
#include "latina.h"
#include "fragmenta_regiones.h"
#line 63 "briar/probationes/fixa/thistle/fragmenta.thistle"
#include "credo.h"

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("probatio", 65536);
    i32 numeri[3];
#line 24 "briar/probationes/fixa/thistle/fragmenta.thistle"
    chorda salutatio = chorda_ex_literis("salve, fragmenta", piscina);
#line 71 "briar/probationes/fixa/thistle/fragmenta.thistle"

    numeri[0] = I;
    numeri[1] = II;
    numeri[2] = III;
    credo_aperire(piscina);
    CREDO_AEQUALIS_I32(summare(numeri, III), VI);
    CREDO_AEQUALIS_I32(salutatio.mensura, XVI);
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
