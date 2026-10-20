#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include <stdio.h>

hic_manens vacuum
_proba (i64 n, Piscina* p)
{
    b32 comp = VERUM;
    chorda c = numerus_romanus_exprimere(n, &comp, p);
    imprimere("n=%lld -> \"%.*s\" mensura=%u datum=%s compositum=%d\n",
        (longus longus)(s64)n, (integer)c.mensura,
        c.datum ? (constans character*)c.datum : "",
        (insignatus integer)c.mensura, c.datum ? "non-NIHIL" : "NIHIL",
        (integer)comp);
}

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    _proba((i64)CVI, p);
    _proba((i64)IV * (i64)M, p);
    _proba((i64)(s64)-1, p);
    _proba((i64)(s64)-4096, p);
    _proba((i64)((s64)-4294967296LL + (s64)V), p);
    redde ZEPHYRUM;
}
