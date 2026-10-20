#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include <stdio.h>

hic_manens vacuum
_proba (constans character* t, Piscina* p)
{
    chorda s = chorda_ex_literis(t, p);
    i32 valor = ZEPHYRUM;
    b32 r = numerus_romanus_legere(s, &valor);
    chorda canon;
    si (r)
    {
        canon = numerus_romanus_scribere(valor, p);
        imprimere("%-8s -> VERUM valor=%u canon=\"%.*s\"%s\n", t,
            (insignatus integer)valor, (integer)canon.mensura,
            canon.datum ? (constans character*)canon.datum : "",
            (canon.mensura == s.mensura) ? "" : "  <- NON CANONICUM");
    }
    alioquin
    {
        imprimere("%-8s -> FALSUM\n", t);
    }
}

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    _proba("XIV", p);
    _proba("VIX", p);
    _proba("VIV", p);
    _proba("LXC", p);
    _proba("DCD", p);
    _proba("MMMDCM", p);
    _proba("vix", p);
    _proba("IXX", p);
    _proba("MIX", p);
    redde ZEPHYRUM;
}
