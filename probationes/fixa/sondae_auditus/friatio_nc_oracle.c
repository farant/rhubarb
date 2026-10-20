#include "latina.h"
#include "chorda.h"
#include "friatio.h"
#include <stdio.h>

/* LCG deterministicus: s = s*1103515245 + 12345 mod 2^32, byte = s>>16 & 0xFF */
static unsigned long sem = 1UL;
static unsigned int gen(void) { sem = (sem * 1103515245UL + 12345UL) & 0xFFFFFFFFUL; return (unsigned int)((sem >> 16) & 0xFFU); }

static void caso(const char* tit, const unsigned char* p, unsigned int n)
{
    chorda ch;
    unsigned int f1, f2, d1, d2;
    ch.mensura = n; ch.datum = (i8*)p;
    f1 = friatio_fnv1a(ch); f2 = friatio_fnv1a_literis((const char*)p, n);
    d1 = friatio_djb2(ch);  d2 = friatio_djb2_literis((const char*)p, n);
    printf("%s n=%u fnv=%08x fnvlit=%08x djb=%08x djblit=%08x\n", tit, n, f1, f2, d1, d2);
}

int main(void)
{
    static unsigned char buf[300];
    unsigned char e9[2] = {0xC3, 0xA9};
    unsigned char ff[1] = {0xFF};
    unsigned char anb[3] = {'a', 0, 'b'};
    unsigned int i, n;
    chorda nul;
    caso("e_acutum", e9, 2);
    caso("ff", ff, 1);
    caso("a0b", anb, 3);
    caso("vacua", e9, 0);
    for (n = 1; n <= 257; n += 32) {
        for (i = 0; i < n; i++) buf[i] = (unsigned char)gen();
        caso("rand", buf, n);
    }
    for (i = 0; i < 64; i++) buf[i] = 0xFF;
    caso("ff64", buf, 64);
    /* NIHIL, mensura 0 */
    nul.mensura = 0; nul.datum = NIHIL;
    printf("nihil fnv=%08x djb=%08x fnvlit=%08x djblit=%08x\n",
        friatio_fnv1a(nul), friatio_djb2(nul),
        friatio_fnv1a_literis(NIHIL, 0), friatio_djb2_literis(NIHIL, 0));
    /* mensura 0 cum literis non vacuis: 'strlen' non sumitur? */
    printf("zero_cum_literis fnvlit=%08x djblit=%08x\n",
        friatio_fnv1a_literis("abc", 0), friatio_djb2_literis("abc", 0));
    return 0;
}
