#include "latina.h"
#include "chorda.h"
#include "friatio.h"
#include <stdio.h>
#include <stdlib.h>

static void hexa(const char* t, const unsigned char* d, unsigned int k)
{ unsigned int q; printf("%s ", t); for (q = 0; q < k; q++) printf("%02x", d[q]); printf("\n"); }

int main(void)
{
    unsigned long tot = (1UL << 29) + 3UL, tot2 = (1UL << 29) + 5UL, q, pos;
    unsigned char* buf = (unsigned char*)malloc(tot2);
    unsigned char d1[20], d2[32];
    SHA1Contextus c1; SHA256Contextus c2;
    if (!buf) { printf("malloc fallit\n"); return 1; }
    for (q = 0; q < tot2; q++) buf[q] = (unsigned char)(q % 251UL);
    /* per frusta 1 MiB + 7 (non alineata) */
    sha1_initiare(&c1); sha256_initiare(&c2);
    for (pos = 0; pos < tot; ) {
        unsigned long m = (1UL << 20) + 7UL;
        if (m > tot - pos) m = tot - pos;
        sha1_addere(&c1, buf + pos, (i32)m); sha256_addere(&c2, buf + pos, (i32)m);
        pos += m;
    }
    printf("numerus post frusta: lo=%08x hi=%08x\n", c2.numerus[0], c2.numerus[1]);
    sha1_finire(&c1, d1); sha256_finire(&c2, d2);
    hexa("frusta_sha1", d1, 20); hexa("frusta_sha256", d2, 32);
    /* vocatio una, mensura >= 2^29 */
    sha1_friare(buf, (i32)tot2, d1); sha256_friare(buf, (i32)tot2, d2);
    hexa("una_sha1", d1, 20); hexa("una_sha256", d2, 32);
    free(buf);
    return 0;
}
