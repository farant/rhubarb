#include "latina.h"
#include "sigillum.h"
#include <stdio.h>
#include <sys/mman.h>
/* one-shot of 2^32 + 70 bytes (zeros, tail "TAILmarker..." in last 16 bytes): any i32 truncation would hash 70 bytes */
int main(void)
{
    memoriae_index n_len = ((memoriae_index)1 << 32) + 70;
    unsigned char* p = (unsigned char*)mmap(NULL, n_len, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    char hex[SIGILLUM_HEX_MENSURA];
    Sigillum s; SigillumContextus c;
    int idx;
    if (p == MAP_FAILED) { printf("mmap failed\n"); return 3; }
    for (idx = 0; idx < 16; idx++) p[n_len - 16 + (memoriae_index)idx] = (unsigned char)("TAILmarker012345"[idx]);
    s = sigillum_computare(p, n_len);
    sigillum_hex(&s, hex);
    printf("magnus %s\n", hex);
    /* incremental: one add of whole length after a 3-byte prefix offset trick */
    sigillum_incipere(&c);
    sigillum_addere(&c, p, 3);
    sigillum_addere(&c, p + 3, n_len - 3);
    printf("totalis=%llu massa=%u\n", (unsigned long long)c.totalis, (unsigned)c.massa_mensura);
    s = sigillum_finire(&c);
    sigillum_hex(&s, hex);
    printf("magnus_inc %s\n", hex);
    return 0;
}
