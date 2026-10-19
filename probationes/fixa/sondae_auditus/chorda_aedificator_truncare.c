#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>

static void show(const char* tag, ChordaAedificator* a)
{
    chorda s = chorda_aedificator_spectare(a);
    printf("%s\tlen=%u\t[%.*s]\n", tag, s.mensura, (int)s.mensura, (char*)s.datum);
}
int main(void)
{
    Piscina* pp = piscina_generare_dynamicum("sonda", 65536);
    ChordaAedificator* a = chorda_aedificator_creare(pp, 32);
    memoriae_index signum;
    i8* ante;
    chorda_aedificator_appendere_literis(a, "abcdef");
    chorda_aedificator_truncare(a, 100); show("TRUNC_GT", a);
    chorda_aedificator_truncare(a, 6); show("TRUNC_EQ", a);
    chorda_aedificator_truncare(a, (memoriae_index)-1); show("TRUNC_MAX", a);
    chorda_aedificator_truncare(a, 3); show("TRUNC_3", a);
    chorda_aedificator_truncare(a, 5); show("TRUNC_UP_AGAIN", a);
    signum = chorda_aedificator_longitudo(a);
    chorda_aedificator_appendere_literis(a, "XYZ");
    chorda_aedificator_appendere_s32(a, -42);
    chorda_aedificator_truncare(a, signum); show("ROLLBACK", a);
    ante = chorda_aedificator_spectare(a).datum;
    chorda_aedificator_truncare(a, 0); show("TRUNC_0", a);
    chorda_aedificator_appendere_literis(a, "0123456789012345678901234567890");
    printf("CAP_KEPT_TRUNC\tsame_buffer=%d\n", chorda_aedificator_spectare(a).datum == ante);
    chorda_aedificator_reset(a);
    chorda_aedificator_appendere_literis(a, "q");
    printf("CAP_KEPT_RESET\tsame_buffer=%d\n", chorda_aedificator_spectare(a).datum == ante);
    show("POST_RESET", a);
    chorda_aedificator_truncare(NIHIL, 3);
    printf("TRUNC_NIHIL ok\n");
    return 0;
}
