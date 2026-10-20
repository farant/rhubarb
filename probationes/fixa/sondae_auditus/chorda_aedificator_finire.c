#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>

int main(void)
{
    Piscina* pp = piscina_generare_dynamicum("sonda", 65536);
    ChordaAedificator* a;
    chorda f1, f2;
    b32 r;

    a = chorda_aedificator_creare(pp, 64);
    chorda_aedificator_appendere_literis(a, "hello");
    f1 = chorda_aedificator_finire(a);
    printf("FINIRE1\t[%.*s]\n", (int)f1.mensura, (char*)f1.datum);
    r = chorda_aedificator_appendere_literis(a, "XX");
    printf("APP_POST_FINIRE\tr=%d\tlongitudo=%lu (builder 'destruitur' yet still live)\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    f2 = chorda_aedificator_finire(a);
    printf("FINIRE2\t[%.*s]\tsame_datum=%d\n", (int)f2.mensura, (char*)f2.datum, f1.datum == f2.datum);
    chorda_aedificator_reset(a);
    chorda_aedificator_appendere_literis(a, "WORLD");
    printf("CALLER_CHORDA_AFTER_RESET\t[%.*s]\t(expected [hello], caller owns it)\n", (int)f1.mensura, (char*)f1.datum);
    chorda_aedificator_destruere(a);
    chorda_aedificator_destruere(a);
    r = chorda_aedificator_appendere_literis(a, "!");
    printf("APP_POST_DESTRUERE_x2\tr=%d\tlen=%lu\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    return 0;
}
