/* exprimere over chosen edges and pseudo-random values in every decade
 * of magnitude up to 2^64; output 'E <n> <compositum> <expr>' for the
 * Python oracle (C int semantics evaluator). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include <stdio.h>

static Piscina* p;
static void unum(i64 n)
{
    b32 c = 7;
    chorda e = numerus_romanus_exprimere(n, &c, p);
    printf("E %llu %d %.*s\n", (unsigned long long)n, (int)c, (int)e.mensura, e.datum ? (char*)e.datum : "");
    piscina_vacare(p);
}

int main(void)
{
    i64 edges[] = {0, 1, 3999, 4000, 4001, 4096, 5280, 1024, 2048, 3072, 128000, 1024000, 4096000,
        4500000, 4000005, 1000000001ULL, 1000000000000000000ULL, 5000000, 1500000, 3999999999ULL,
        4000000, 2147483647ULL, 2147483648ULL, 2147483649ULL, 2147484647ULL, 2147999999ULL, 2148000000ULL,
        2147483647ULL + 1024, 2148000000ULL - 1, 3000000000ULL, 4294967295ULL, 4294967296ULL,
        4097ULL * 1024, 4097ULL * 1024 * 1024, 2097153ULL * 1024, 1ULL << 40, 1ULL << 63,
        (1ULL << 63) - 1, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFC00ULL, 4608000, 4500ULL * 1024,
        4501ULL * 1024, 2147483ULL * 1000 + 648, 18000000000000000000ULL, 18446744073709551000ULL,
        3999ULL * 1024, 4000ULL * 1024 - 1024 * 3, 1ULL<<32, (1ULL<<31)+1, 2147483647ULL*1024,
        2097151ULL*1024, 2097152ULL*1024, 2097151ULL*1024 + 1};
    unsigned k, d;
    i64 x = 88172645463325252ULL;
    p = piscina_generare_dynamicum("sonda", 65536);
    for (k = 0; k < sizeof(edges)/sizeof(edges[0]); k++) unum(edges[k]);
    /* dense window around 2^31 where terms fit int but sums do not */
    for (k = 0; k < 2000; k++) unum(2147483648ULL + (i64)k * 257);
    /* random per magnitude bit 12..63 */
    for (d = 12; d < 64; d++)
        for (k = 0; k < 300; k++)
        {
            i64 n;
            x ^= x << 13; x ^= x >> 7; x ^= x << 17;
            n = x & ((d == 63) ? 0xFFFFFFFFFFFFFFFFULL : ((1ULL << (d + 1)) - 1));
            if (k % 3 == 0) n = (n / 1024) * 1024;        /* binary family */
            if (k % 7 == 0) n = (n / 1000) * 1000;        /* round thousands */
            unum(n);
        }
    piscina_destruere(p);
    return 0;
}
