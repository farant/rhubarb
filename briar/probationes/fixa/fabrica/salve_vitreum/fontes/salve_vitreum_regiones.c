/* salve_vitreum_regiones.c - a briar genitum ex briar/probationes/fixa/thistle/salve_vitreum.thistle: obiecta et definitiones regionum */
#include "latina.h"
#include "salve_vitreum_regiones.h"
#line 24 "briar/probationes/fixa/thistle/salve_vitreum.thistle"
JsonValor*
salve (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    JsonValor* fructus = json_objectum_creare(piscina);

    (vacuum)argumenta; (vacuum)datum; (vacuum)culpa;
    json_objectum_ponere(fructus, "nuntius",
        json_chorda_creare_literis(piscina, "salve, munde"));
    redde fructus;
}
