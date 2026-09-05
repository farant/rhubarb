/* probatio_derivatum.c - a briar genitum ex briar/probationes/fixa/thistle/derivatum.thistle: regio munus="probatio" */
#include "latina.h"
#include "derivatum_regiones.h"
#include "credo.h"
#include "piscina.h"
#line 28 "briar/probationes/fixa/thistle/derivatum.thistle"
s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("probatio", 65536);

    credo_aperire(piscina);
    CREDO_AEQUALIS_I32(longitudo_salutationis(piscina), XII);
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
