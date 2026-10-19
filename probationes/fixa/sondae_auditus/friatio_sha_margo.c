#include "latina.h"
#include "chorda.h"
#include "friatio.h"
#include <stdio.h>

static unsigned long sem = 7UL;
static unsigned int gen(void) { sem = (sem * 1103515245UL + 12345UL) & 0xFFFFFFFFUL; return (unsigned int)((sem >> 16) & 0xFFU); }
static void hexa(const char* t, unsigned int n, const unsigned char* d, unsigned int k)
{ unsigned int q; printf("%s %u ", t, n); for (q = 0; q < k; q++) printf("%02x", d[q]); printf("\n"); }

int main(void)
{
    static unsigned char buf[400];
    unsigned char d1[20], d2[32];
    unsigned int n, q;
    for (q = 0; q < sizeof buf; q++) buf[q] = (unsigned char)gen();
    for (n = 0; n <= 300; n++) {
        sha1_friare(buf, n, d1);   hexa("sha1", n, d1, 20);
        sha256_friare(buf, n, d2); hexa("sha256", n, d2, 32);
    }
    return 0;
}
