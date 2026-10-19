#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    memoriae_index u0, u1, n0, n1;
    chorda vac; chorda* r;
    int k;
    i8 buf[4] = {'a','b','c',0};

    u0 = piscina_summa_usus(p); n0 = piscina_numerus_allocationum(p);
    r = chorda_internare_ex_literis(in, "");
    u1 = piscina_summa_usus(p); n1 = piscina_numerus_allocationum(p);
    printf("ex_literis(\"\"): ret=%p usus_delta=%lu alloc_delta=%lu numerus=%u\n",
        (void*)r, (unsigned long)(u1-u0), (unsigned long)(n1-n0), internamentum_numerus(in));

    vac.datum = buf; vac.mensura = 0;
    u0 = piscina_summa_usus(p); n0 = piscina_numerus_allocationum(p);
    r = chorda_internare(in, vac);
    u1 = piscina_summa_usus(p); n1 = piscina_numerus_allocationum(p);
    printf("internare(len0,datum!=NIHIL): ret=%p usus_delta=%lu alloc_delta=%lu numerus=%u\n",
        (void*)r, (unsigned long)(u1-u0), (unsigned long)(n1-n0), internamentum_numerus(in));

    vac.datum = NIHIL; vac.mensura = 0;
    u0 = piscina_summa_usus(p);
    r = chorda_internare(in, vac);
    u1 = piscina_summa_usus(p);
    printf("internare(len0,datum NIHIL): ret=%p usus_delta=%lu\n", (void*)r, (unsigned long)(u1-u0));

    /* repeated refusals */
    u0 = piscina_summa_usus(p);
    for (k = 0; k < 10000; k++) chorda_internare_ex_literis(in, "");
    u1 = piscina_summa_usus(p);
    printf("10000 x ex_literis(\"\"): usus_delta=%lu numerus=%u\n", (unsigned long)(u1-u0), internamentum_numerus(in));

    /* duplicate does not allocate */
    chorda_internare_ex_literis(in, "abc");
    u0 = piscina_summa_usus(p);
    chorda_internare_ex_literis(in, "abc");
    vac.datum = buf; vac.mensura = 3;
    chorda_internare(in, vac);
    u1 = piscina_summa_usus(p);
    printf("duplicate interns: usus_delta=%lu\n", (unsigned long)(u1-u0));
    piscina_destruere(p);
    return 0;
}
