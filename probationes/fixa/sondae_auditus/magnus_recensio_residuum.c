#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Magnus big, nbig, smin, u, v, g;
    smin = magnus_ex_s64((s64)(-0x7FFFFFFFFFFFFFFFLL - 1));
    (void)magnus_ex_chorda(chorda_ex_literis("340282366920938463463374607431768211455", p), p, &big);
    nbig = magnus_nega(big, p);
    printf("-7 mod 3 = %u\n", (unsigned)magnus_residuum_parvum(magnus_ex_s64(-7), 3));
    printf("-9 mod 3 = %u\n", (unsigned)magnus_residuum_parvum(magnus_ex_s64(-9), 3));
    printf("-1 mod 4294967295 = %u\n", (unsigned)magnus_residuum_parvum(magnus_ex_s64(-1), 4294967295U));
    printf("smin mod 3 = %u\n", (unsigned)magnus_residuum_parvum(smin, 3));
    printf("smin mod 2147483648 = %u\n", (unsigned)magnus_residuum_parvum(smin, 2147483648U));
    printf("(2^128-1) mod 1000000007 = %u\n", (unsigned)magnus_residuum_parvum(big, 1000000007U));
    printf("-(2^128-1) mod 1000000007 = %u\n", (unsigned)magnus_residuum_parvum(nbig, 1000000007U));
    printf("5 mod 0 = %u\n", (unsigned)magnus_residuum_parvum(magnus_ex_s64(5), 0));
    g = magnus_divisor_communis_testatus(big, magnus_ex_s64(0), p, &u, &v);
    printf("testatus(big,0) apex=%lu g==big %d\n", (unsigned long)magnus_apex_alternarum(), (int)magnus_aequalis(g, big));
    {
        Magnus q;
        b32 ok = magnus_divide(smin, magnus_ex_s64(-1), p, &q, &q);
        printf("divide(smin,-1,&q,&q) ok=%d q=%s\n", (int)ok, (char*)magnus_ad_chordam(q,p).datum);
    }
    piscina_destruere(p);
    return 0;
}
