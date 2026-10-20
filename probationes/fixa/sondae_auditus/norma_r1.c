/* sonda: discrimen APERTUM/NOTANDUM - iudex tag ignotum accipit,
 * exportatio oneOf cum const solum */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "norma.h"
#include <stdio.h>

interior vacuum
_ostendere (constans character* titulus, NormaModus modus, Piscina* p)
{
    Norma* d = norma_discrimen(p, "type");
    Norma* o = norma_objectum(p);
    JsonResultus r;
    NormaIudicium j;
    chorda s;

    norma_campus(o, "text", norma_textus(p), VERUM);
    norma_variatio(d, "text", o);
    norma_modus(d, modus);
    r = json_legere_literis("{\"type\":\"image\",\"text\":\"x\"}", p);
    j = norma_iudicare(d, r.radix, p);
    s = json_scribere(norma_json_schema(d, p), p);
    imprimere("%s: validum=%d vitia=%d notae=%d\n  schema=%.*s\n", titulus,
        (integer)j.validum, (integer)xar_numerus(j.vitia),
        (integer)xar_numerus(j.notae), (integer)s.mensura, (character*)s.datum);
}

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    _ostendere("CLAUSUM", NORMA_CLAUSUM, p);
    _ostendere("APERTUM", NORMA_APERTUM, p);
    _ostendere("NOTANDUM", NORMA_NOTANDUM, p);
    piscina_destruere(p);
    redde 0;
}
