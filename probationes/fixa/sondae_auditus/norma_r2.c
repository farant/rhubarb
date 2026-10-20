/* sonda: electio + aut_nullum - iudex null accipit, exportatio enum sine null */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "norma.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    constans character* licita[3];
    Norma* n;
    JsonResultus r;
    NormaIudicium j;
    chorda s;

    licita[0] = "a";
    licita[1] = "b";
    licita[2] = NIHIL;
    n = norma_aut_nullum(norma_electio(norma_textus(p), licita));
    r = json_legere_literis("null", p);
    j = norma_iudicare(n, r.radix, p);
    s = json_scribere(norma_json_schema(n, p), p);
    imprimere("null: validum=%d vitia=%d\n  schema=%.*s\n",
        (integer)j.validum, (integer)xar_numerus(j.vitia),
        (integer)s.mensura, (character*)s.datum);
    piscina_destruere(p);
    redde 0;
}
