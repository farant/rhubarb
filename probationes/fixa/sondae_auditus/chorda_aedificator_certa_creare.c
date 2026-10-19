#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>

int main(void)
{
    Piscina* pp = piscina_generare_certae_magnitudinis("sonda_certa", 256);
    ChordaAedificator* a;
    printf("ANTE creare(p, 4096) in piscina certa 256\n"); fflush(stdout);
    a = chorda_aedificator_creare(pp, 4096);
    printf("POST creare: %s (expected NIHIL)\n", a ? "non-NIHIL" : "NIHIL");
    return 0;
}
