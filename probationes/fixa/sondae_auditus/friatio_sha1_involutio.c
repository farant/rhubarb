#include "latina.h"
#include "chorda.h"
#include "friatio.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

/* j = 1 (unus byte in buffer), deinde mensura = 0xFFFFFFFF: (j + mensura) involvitur ad 0,
 * ramus 'alioquin' memcpy(&buffer[1], data, 0xFFFFFFFF) in buffer 64 bytes. */
int main(void)
{
    unsigned long tot = 0xFFFFFFFFUL;
    void* p = mmap(NULL, tot + 1UL, PROT_READ, MAP_ANON | MAP_PRIVATE, -1, 0);
    SHA1Contextus* c2 = (SHA1Contextus*)malloc(sizeof(SHA1Contextus));
    if (p == MAP_FAILED || !c2) { printf("mmap/malloc fallit\n"); return 1; }
    sha1_initiare(c2);
    sha1_addere(c2, (const i8*)"a", 1);
    printf("j=1, nunc addere mensura=0x%08lx (j+mensura mod 2^32 = %u)\n", tot, (unsigned int)(1U + 0xFFFFFFFFU));
    fflush(stdout);
    sha1_addere(c2, (const i8*)p, (i32)tot);
    printf("redditum (non expectatum)\n");
    return 0;
}
