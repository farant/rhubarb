#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>

int main(void)
{
    Piscina* pp = piscina_generare_certae_magnitudinis("sonda_certa", 256);
    ChordaAedificator* a;
    i32 k; b32 r = VERUM;
    a = chorda_aedificator_creare(pp, 16);
    printf("CREARE\t%s\n", a ? "ok" : "NIHIL"); fflush(stdout);
    for (k = 0; k < 1000 && r; k++)
    {
        r = chorda_aedificator_appendere_character(a, 'x');
        if (k % 16 == 15) { printf("APP\tk=%u\tr=%d\tlen=%lu\n", k, (int)r, (unsigned long)chorda_aedificator_longitudo(a)); fflush(stdout); }
    }
    printf("FINIS\tk=%u\tr=%d (expected: FALSUM returned, process continues)\n", k, (int)r);
    return 0;
}
