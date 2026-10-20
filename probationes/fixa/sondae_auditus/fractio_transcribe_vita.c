#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include <stdio.h>

int main(void)
{
    Piscina* src = piscina_generare_dynamicum("fons", 4096);
    Piscina* dst = piscina_generare_dynamicum("meta", 4096);
    Fractio big, small, zero, copy_big, copy_small, copy_zero, same, nb;
    chorda t;
    (void)fractio_ex_chorda(chorda_ex_literis("-123456789012345678901234567890123/98765432109876543210987654321", src), src, &big);
    (void)fractio_ex_s64_s64(-3, 7, src, &small);
    zero = fractio_ex_s64(0);
    copy_big = fractio_transcribe(big, dst);
    copy_small = fractio_transcribe(small, dst);
    copy_zero = fractio_transcribe(zero, dst);
    same = fractio_transcribe(big, src);
    nb = fractio_nega(copy_big, dst);
    printf("limbs differ: num %d den %d\n",
        fractio_numerator(copy_big).membra != fractio_numerator(big).membra,
        fractio_denominator(copy_big).membra != fractio_denominator(big).membra);
    printf("same-piscina copy equal=%d limbs differ=%d\n", fractio_aequalis(same, big), fractio_numerator(same).membra != fractio_numerator(big).membra);
    piscina_destruere(src);
    t = fractio_ad_chordam(copy_big, dst); printf("big after destroy = %.*s\n", (int)t.mensura, (const char*)t.datum);
    t = fractio_ad_chordam(copy_small, dst); printf("small after destroy = %.*s\n", (int)t.mensura, (const char*)t.datum);
    t = fractio_ad_chordam(copy_zero, dst); printf("zero after destroy = %.*s\n", (int)t.mensura, (const char*)t.datum);
    t = fractio_ad_chordam(nb, dst); printf("nega(copy) = %.*s\n", (int)t.mensura, (const char*)t.datum);
    piscina_destruere(dst);
    return 0;
}
