/* probatio_salve_vitreum.c - a briar genitum ex briar/probationes/fixa/thistle/salve_vitreum.thistle: regio munus="probatio" */
#include "latina.h"
#include "salve_vitreum_regiones.h"
#line 38 "briar/probationes/fixa/thistle/salve_vitreum.thistle"
#include "credo.h"
#include "json.h"

s32
principale (vacuum)
{
    Piscina*   piscina = piscina_generare_dynamicum("probatio", 65536);
    JsonValor* r;
    chorda     culpa;

    credo_aperire(piscina);
    r = salve(NIHIL, piscina, NIHIL, &culpa);
    CREDO_NON_NIHIL(r);
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
