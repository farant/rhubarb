#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include <stdio.h>
#include <sys/mman.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    unsigned long sz = 2147483648UL + 16UL; /* 2^31 + 16 */
    i8* m = (i8*)mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    chorda c; chorda r;
    if (m == (i8*)MAP_FAILED) { printf("mmap failed\n"); return 3; }
    /* "/dir/" + zeros ... + "name.txt" at the very end */
    m[0] = '/'; m[1] = 'd'; m[2] = 'i'; m[3] = 'r'; m[4] = '/';
    m[sz - 9] = '/'; m[sz - 8] = 'n'; m[sz - 7] = 'a'; m[sz - 6] = 'm'; m[sz - 5] = 'e';
    m[sz - 4] = '.'; m[sz - 3] = 't'; m[sz - 2] = 'x'; m[sz - 1] = 't';
    c.datum = m; c.mensura = (i32)sz;
    r = via_nomen(c, p);
    printf("nomen: mensura=%u (expected 8 'name.txt') first='%.*s'\n", (unsigned)r.mensura, r.mensura < 16 ? (int)r.mensura : 8, (const char*)r.datum);
    r = via_extensio(c, p);
    printf("extensio: mensura=%u (expected 4 '.txt')\n", (unsigned)r.mensura);
    r = via_directorium(c, p);
    printf("directorium: mensura=%u (expected %lu)\n", (unsigned)r.mensura, sz - 9);
    munmap(m, sz);
    piscina_destruere(p);
    return 0;
}
