#include "latina.h"
#include "chorda.h"
#include "friatio.h"
#include <stdio.h>

static unsigned long sem = 3UL;
static unsigned int gen(void) { sem = (sem * 1103515245UL + 12345UL) & 0xFFFFFFFFUL; return (unsigned int)((sem >> 16) & 0xFFU); }

int main(void)
{
    static unsigned char buf[1024];
    unsigned int n, q, cut, mal = 0, cas = 0;
    chorda ch;
    unsigned char nul3[3] = {'a', 0, 'b'};
    for (q = 0; q < sizeof buf; q++) buf[q] = (unsigned char)gen();
    for (q = 600; q < 700; q++) buf[q] = 0xFF;
    printf("crc_check %08x\n", crc32_calculare((const i8*)"123456789", 9));
    printf("crc_empty %08x\n", crc32_calculare((const i8*)"", 0));
    printf("crc_finire_initiare %08x\n", crc32_finire(crc32_initiare()));
    for (n = 0; n <= 1024; n += 37) printf("crc %u %08x\n", n, crc32_calculare(buf, n));
    printf("crc_ff100 %08x\n", crc32_calculare(buf + 600, 100));
    ch.mensura = 3; ch.datum = nul3;
    printf("crc_chorda_a0b %08x direct %08x\n", crc32_calculare_chorda(ch), crc32_calculare(nul3, 3));
    /* compositio, cum partibus vacuis */
    for (n = 0; n <= 300; n += 13) for (cut = 0; cut <= n; cut += 7) {
        unsigned int c = crc32_initiare();
        c = crc32_addere(c, buf, 0);
        c = crc32_addere(c, buf, cut);
        c = crc32_addere(c, buf + cut, 0);
        c = crc32_addere(c, buf + cut, n - cut);
        c = crc32_finire(c);
        cas++; if (c != crc32_calculare(buf, n)) mal++;
    }
    printf("compositio casus %u mal %u\n", cas, mal);
    /* stilus zlib: crc finitum transmissum ad addere */
    {
        unsigned int a = crc32_calculare(buf, 10);
        unsigned int z = crc32_addere(a, buf + 10, 10);  /* sine re-inversione */
        unsigned int r = crc32_finire(crc32_addere(crc32_finire(a), buf + 10, 10)); /* re-inversio */
        printf("zlib_stilus_naive %08x re_inversum %08x verum %08x\n", z, r, crc32_calculare(buf, 20));
        printf("zlib_initium_0 %08x\n", crc32_addere(0, buf, 10));
    }
    return 0;
}
