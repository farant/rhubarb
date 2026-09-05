/* probatio_punctum.c - a briar genitum ex briar/probationes/fixa/thistle/punctum.thistle: regio munus="probatio" */
#include "latina.h"
#include "punctum_regiones.h"
#line 38 "briar/probationes/fixa/thistle/punctum.thistle"
#include "credo.h"

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("probatio", 65536);
    Punctum  p;

    p.x = II;
    p.y = II;
    credo_aperire(piscina);
    CREDO_AEQUALIS_I32(punctum_summa(&p), IV);
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
