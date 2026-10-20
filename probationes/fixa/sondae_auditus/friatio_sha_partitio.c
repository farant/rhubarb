#include "latina.h"
#include "chorda.h"
#include "friatio.h"
#include <stdio.h>
#include <string.h>

static unsigned long sem = 7UL;
static unsigned int gen(void) { sem = (sem * 1103515245UL + 12345UL) & 0xFFFFFFFFUL; return (unsigned int)((sem >> 16) & 0xFFU); }

/* partitiones: magnitudines particularum (0 = vocatio vacua) cyclice */
int main(void)
{
    static unsigned char buf[1000];
    static const unsigned int schemata[][6] = {
        {1,0,0,0,0,0}, {63,1,0,0,0,0}, {64,64,0,0,0,0}, {0,5,0,70,0,0},
        {3,200,1,0,0,0}, {130,0,0,0,0,0}, {65,129,0,0,0,0}, {1,63,128,7,0,0},
        {200,0,0,0,0,0}, {55,9,0,0,0,0}, {2,190,0,0,0,0}
    };
    unsigned int lens[] = {0, 1, 55, 56, 63, 64, 65, 127, 128, 129, 191, 192, 193, 500, 1000};
    unsigned int s, li, q, mal = 0, cas = 0;
    for (q = 0; q < sizeof buf; q++) buf[q] = (unsigned char)gen();
    for (s = 0; s < sizeof schemata / sizeof schemata[0]; s++) {
        for (li = 0; li < sizeof lens / sizeof lens[0]; li++) {
            unsigned int tot = lens[li], pos = 0, k = 0, guard = 0;
            SHA1Contextus c1; SHA256Contextus c2;
            unsigned char a1[20], b1[20], a2[32], b2[32];
            sha1_friare(buf, tot, a1); sha256_friare(buf, tot, a2);
            sha1_initiare(&c1); sha256_initiare(&c2);
            while (pos < tot && guard < 100000) {
                unsigned int m = schemata[s][k % 6];
                k++; guard++;
                if (m > tot - pos) m = tot - pos;
                sha1_addere(&c1, buf + pos, m); sha256_addere(&c2, buf + pos, m);
                pos += m;
            }
            sha1_finire(&c1, b1); sha256_finire(&c2, b2);
            cas++;
            if (memcmp(a1, b1, 20) || memcmp(a2, b2, 32)) { mal++; printf("MAL schema %u len %u\n", s, tot); }
        }
    }
    printf("partitio casus %u mal %u\n", cas, mal);
    return 0;
}
