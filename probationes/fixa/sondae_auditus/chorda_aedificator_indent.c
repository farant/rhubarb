#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>

int main(void)
{
    Piscina* pp = piscina_generare_dynamicum("sonda", 65536);
    ChordaAedificator* a;
    b32 r; i32 k;

    a = chorda_aedificator_creare(pp, 16);
    r = chorda_aedificator_appendere_indentationem(a, 0x80000000u);
    printf("IND_2^31\tr=%d\tlen=%lu\texpected=4294967296\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    r = chorda_aedificator_appendere_indentationem(a, 0x80000001u);
    printf("IND_2^31+1\tr=%d\tlen=%lu\texpected=4294967298\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    chorda_aedificator_reset(a);
    r = chorda_aedificator_appendere_indentationem(a, 0);
    printf("IND_0\tr=%d\tlen=%lu\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));
    r = chorda_aedificator_appendere_indentationem(a, 3);
    printf("IND_3\tr=%d\tlen=%lu\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a));

    a = chorda_aedificator_creare(pp, 16);
    chorda_aedificator_pop_indentationem(a);
    printf("POP_AT_0\tgradus=%u\n", chorda_aedificator_indentatio_gradus(a));
    for (k = 0; k < 1005; k++) chorda_aedificator_push_indentationem(a);
    printf("PUSH_1005\tgradus=%u\texpected=1005\n", chorda_aedificator_indentatio_gradus(a));
    for (k = 0; k < 1000; k++) chorda_aedificator_pop_indentationem(a);
    printf("PUSH1005_POP1000\tgradus=%u\texpected=5\n", chorda_aedificator_indentatio_gradus(a));
    chorda_aedificator_push_indentationem(a);
    chorda_aedificator_push_indentationem(a);
    chorda_aedificator_reset(a);
    printf("RESET_GRADUS\tgradus=%u\n", chorda_aedificator_indentatio_gradus(a));
    r = chorda_aedificator_appendere_lineam_novam(a);
    printf("LINEA\tr=%d\tlen=%lu\tbyte=%02x\n", (int)r, (unsigned long)chorda_aedificator_longitudo(a), (unsigned)(unsigned char)chorda_aedificator_spectare(a).datum[0]);
    return 0;
}
