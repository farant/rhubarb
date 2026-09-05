/* derivatum.c - a briar genitum ex briar/probationes/fixa/thistle/derivatum.thistle: principale */
#include "latina.h"
#include "derivatum_regiones.h"
#line 16 "briar/probationes/fixa/thistle/derivatum.thistle"
s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("derivatum", 65536);

    imprimere("longitudo %d\n", (integer)longitudo_salutationis(piscina));
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
