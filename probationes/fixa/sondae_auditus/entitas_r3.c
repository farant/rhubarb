/* sonda: _parsare_s64 recusat minimum s64 */
#include "latina.h"
#include "entitas.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

interior b32
_proba (constans character* s, TypusLiteralis t, Proprietas* p, chorda* c)
{
    c->datum   = (i8*)(constans vacuum*)s;
    c->mensura = (i32)strlen(s);
    memset(p, 0, magnitudo(*p));
    p->valor = c;
    redde proprietas_parsare_ut_typum(p, t);
}

integer
principale (vacuum)
{
    Piscina*   piscina;
    Proprietas p;
    chorda     c;
    b32        r;

    piscina = piscina_generare_dynamicum("sonda", 65536);

    r = _proba("-9223372036854775808", TYPUS_S64, &p, &c);
    printf("s64 min: r=%d val=%lld\n", (int)r, (long long)p.parsitus.ut_s64);
    r = _proba("-9223372036854775807", TYPUS_S64, &p, &c);
    printf("s64 min+1: r=%d val=%lld\n", (int)r, (long long)p.parsitus.ut_s64);
    r = _proba("9223372036854775807", TYPUS_S64, &p, &c);
    printf("s64 max: r=%d val=%lld\n", (int)r, (long long)p.parsitus.ut_s64);
    r = _proba("-9223372036854775808", TYPUS_TEMPUS, &p, &c);
    printf("tempus min: r=%d\n", (int)r);
    r = _proba("-2147483648", TYPUS_S32, &p, &c);
    printf("s32 min (comparatio): r=%d val=%d\n", (int)r, (int)p.parsitus.ut_s32);

    piscina_destruere(piscina);
    redde 0;
}
