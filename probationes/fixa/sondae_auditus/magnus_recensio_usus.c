#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Magnus m;
    chorda t;
    memoriae_index ante, post;
    b32 ok;
    int idx;
    t = chorda_ex_literis("7", p);
    ante = piscina_summa_usus(p);
    ok = magnus_ex_chorda(t, p, &m);
    post = piscina_summa_usus(p);
    printf("ex_chorda(7) ok=%d membra=%p usus_delta=%lu\n", (int)ok, (void*)m.membra, (unsigned long)(post - ante));
    t = chorda_ex_literis("-9223372036854775808", p);
    ante = piscina_summa_usus(p);
    ok = magnus_ex_chorda(t, p, &m);
    post = piscina_summa_usus(p);
    printf("ex_chorda(S64_MIN) ok=%d membra=%p usus_delta=%lu\n", (int)ok, (void*)m.membra, (unsigned long)(post - ante));
    ante = piscina_summa_usus(p);
    for (idx = 0; idx < 1000; idx++) (void)magnus_ex_chorda(chorda_ex_buffer((i8*)"12345", 5), p, &m);
    post = piscina_summa_usus(p);
    printf("1000 x ex_chorda(12345) usus_delta=%lu\n", (unsigned long)(post - ante));
    ante = piscina_summa_usus(p);
    m = magnus_adde(magnus_ex_s64(2), magnus_ex_s64(3), p);
    post = piscina_summa_usus(p);
    printf("adde(2,3) usus_delta=%lu\n", (unsigned long)(post - ante));
    piscina_destruere(p);
    return 0;
}
