#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include <stdio.h>
#include <stdlib.h>

static Piscina* pp;
static void pr(const char* tag, Fractio f)
{
    chorda t = fractio_ad_chordam(f, pp);
    printf("%s = %.*s\n", tag, (int)t.mensura, (const char*)t.datum);
}
static void prm(const char* tag, Magnus m)
{
    chorda t = magnus_ad_chordam(m, pp);
    printf("%s = %.*s\n", tag, (int)t.mensura, (const char*)t.datum);
}

int main(int argc, char** argv)
{
    int k = argc > 1 ? atoi(argv[1]) : 0;
    s64 Smax = (s64)0x7FFFFFFFFFFFFFFFLL, Smin = -Smax - 1;
    Fractio h = fractio_ex_s64(1), t3 = fractio_ex_s64(1), f = fractio_ex_s64(4242);
    pp = piscina_generare_dynamicum("sonda", 65536);
    (void)fractio_ex_s64_s64(1, 2, pp, &h);
    (void)fractio_ex_s64_s64(1, 3, pp, &t3);
    printf("casus %d\n", k); fflush(stdout);
    switch (k) {
    case 1: printf("ret=%d ", fractio_ex_s64_s64(1, 2, NULL, &f)); pr("1/2", f); break;
    case 2: printf("ret=%d ", fractio_ex_s64_s64(-6, -4, NULL, &f)); pr("-6/-4", f); break;
    case 3: printf("ret=%d ", fractio_ex_chorda(chorda_ex_literis("1", pp), NULL, &f)); pr("'1'", f); break;
    case 4: { chorda t = fractio_ad_chordam(h, NULL); printf("ad_chordam(1/2,NIHIL) datum=%p mensura=%u\n", (void*)t.datum, (unsigned)t.mensura);
              t = fractio_ad_chordam(fractio_ex_s64(5), NULL); printf("ad_chordam(5,NIHIL) datum=%p mensura=%u\n", (void*)t.datum, (unsigned)t.mensura); } break;
    case 5: printf("compara(1/2,1/3,NIHIL) = %d\n", fractio_compara(h, t3, NULL)); break;
    case 6: pr("adde(1/2,1/3,NIHIL)", fractio_adde(h, t3, NULL)); break;
    case 7: printf("ret=%d ", fractio_ex_s64_s64(Smin, -1, NULL, &f)); pr("MIN/-1", f); break;
    case 8: prm("rotunda(1/2,NIHIL)", fractio_rotunda(h, NULL)); break;
    case 9: printf("ret=%d ", fractio_potentia(h, 3, NULL, &f)); pr("(1/2)^3", f); break;
    case 10: pr("nega(MIN,NIHIL)", fractio_nega(fractio_ex_s64(Smin), NULL)); break;
    case 11: pr("transcribe(1/2,NIHIL)", fractio_transcribe(h, NULL)); break;
    case 12: pr("multiplica(1/2,1/3,NIHIL)", fractio_multiplica(h, t3, NULL)); break;
    case 13: printf("ret=%d ", fractio_divide(h, t3, NULL, &f)); pr("(1/2)/(1/3)", f); break;
    }
    piscina_destruere(pp);
    return 0;
}
