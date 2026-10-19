#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
#include <sys/mman.h>
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    vacuum* m;
    chorda a, b, r;
    m = mmap(NIHIL, (size_t)0x80000010UL, PROT_READ, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (m == MAP_FAILED) { printf("mmap failed\n"); return 3; }
    a.datum = (i8*)m; a.mensura = 0x80000000U;
    b.datum = (i8*)m; b.mensura = 0x80000010U;
    printf("case concatenare a.mensura=0x80000000 b.mensura=0x80000010 (sum wraps to 16)\n");
    fflush(stdout);
    r = chorda_concatenare(a, b, p);
    printf("mensura=%u\n", r.mensura);
    return 0;
}
