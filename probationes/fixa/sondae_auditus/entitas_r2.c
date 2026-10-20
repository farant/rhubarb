/* sonda: _parsare_s32/_parsare_f64 truncant valorem ad 63/127 bytes */
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
    character  textus[300];
    b32        r;
    i32        k;

    piscina = piscina_generare_dynamicum("sonda", 65536);

    /* 63 '0' + "abc" -> debet FALSUM */
    per (k = 0; k < 63; k++) textus[k] = '0';
    strcpy(textus + 63, "abc");
    r = _proba(textus, TYPUS_S32, &p, &c);
    printf("s32 63x0+abc: r=%d val=%d\n", (int)r, (int)p.parsitus.ut_s32);

    /* 63 '0' + "7" -> debet VERUM 7 */
    strcpy(textus + 63, "7");
    r = _proba(textus, TYPUS_S32, &p, &c);
    printf("s32 63x0+7: r=%d val=%d\n", (int)r, (int)p.parsitus.ut_s32);

    /* s64 eadem (parser manualis, comparatio) */
    r = _proba(textus, TYPUS_S64, &p, &c);
    printf("s64 63x0+7: r=%d val=%lld\n", (int)r, (long long)p.parsitus.ut_s64);

    /* 127 '0' + "xyz" -> f64 debet FALSUM */
    per (k = 0; k < 127; k++) textus[k] = '0';
    strcpy(textus + 127, "xyz");
    r = _proba(textus, TYPUS_F64, &p, &c);
    printf("f64 127x0+xyz: r=%d val=%g\n", (int)r, p.parsitus.ut_f64);
    strcpy(textus + 127, "5");
    r = _proba(textus, TYPUS_F64, &p, &c);
    printf("f64 127x0+5: r=%d val=%g\n", (int)r, p.parsitus.ut_f64);

    /* casus naturalior: "1" + 62 spatia? strtol non accipit trailing spatia;
       "123" + 61 '0' -> s32 overflow debet FALSUM */
    textus[0] = '1';
    per (k = 1; k < 64; k++) textus[k] = '0';
    textus[64] = '\0';
    r = _proba(textus, TYPUS_S32, &p, &c);
    printf("s32 1+63x0: r=%d\n", (int)r);

    piscina_destruere(piscina);
    redde 0;
}
