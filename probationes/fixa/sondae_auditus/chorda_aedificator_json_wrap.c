#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    Piscina* pp = piscina_generare_dynamicum("sonda", 65536);
    ChordaAedificator* a;
    chorda in;
    b32 r;
    unsigned long nn[2];
    int q;
    nn[0] = 715827883UL; nn[1] = 715827983UL;
    for (q = 0; q < 2; q++)
    {
        i8* big = (i8*)malloc(nn[q]);
        if (!big) { printf("malloc fracta\n"); return 3; }
        memset(big, 'a', nn[q]);
        a = chorda_aedificator_creare(pp, 16);
        in.datum = big; in.mensura = (i32)nn[q];
        printf("necessaria_i32=%u\n", (unsigned)(in.mensura * VI));
        r = chorda_aedificator_appendere_evasus_json(a, in);
        printf("JSON_WRAP\tinput_len=%lu\tr=%d\toutput_len=%lu\n", nn[q], (int)r, (unsigned long)chorda_aedificator_longitudo(a));
        free(big);
    }
    return 0;
}
