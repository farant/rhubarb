/* probatio_salve.c - a briar genitum ex briar/probationes/fixa/thistle/salve.thistle: regio munus="probatio" */
#include "latina.h"
#include "salve_regiones.h"
#line 18 "briar/probationes/fixa/thistle/salve.thistle"
#include "credo.h"

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("probatio", 65536);

    credo_aperire(piscina);
    CREDO_VERUM(I + I == II);
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
