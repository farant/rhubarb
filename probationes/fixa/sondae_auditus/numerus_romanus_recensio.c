#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include <stdio.h>
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    i64 qs[6]; int k;
    qs[0] = 4296015872ULL; qs[1] = 2147483648ULL; qs[2]=2147483647ULL; qs[3]=2148000000ULL;
    qs[4] = 2147483649ULL; qs[5] = 4097ULL*1024ULL;
    for (k = 0; k < 6; k++) {
        b32 c = 7;
        chorda e = numerus_romanus_exprimere(qs[k], &c, p);
        printf("%llu c=%d %.*s\n", (unsigned long long)qs[k], (int)c, (int)e.mensura, (char*)e.datum);
    }
    piscina_destruere(p);
    return 0;
}
