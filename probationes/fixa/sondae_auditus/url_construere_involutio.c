/* numerus = 2^28+1: (i32)magnitudo(chorda)*numerus wraps to 16 at url.c:211-212.
 * claves/valores = lazily mapped zero pages (chorda {NIHIL,0}), so only
 * touched pages cost memory. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "url.h"
#include <stdio.h>
#include <sys/mman.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    i32 numerus_v = ((i32)1 << 28) + 1;
    size_t bytes = (size_t)numerus_v * sizeof(chorda);
    chorda* cl = (chorda*)mmap(NULL, bytes, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    chorda* va = (chorda*)mmap(NULL, bytes, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    chorda q;
    if (cl == MAP_FAILED || va == MAP_FAILED) { printf("mmap fractum\n"); return 3; }
    printf("numerus=%u allocatio i32=%u (pro %lu)\n", (unsigned)numerus_v,
        (unsigned)((i32)sizeof(chorda) * numerus_v), (unsigned long)bytes);
    fflush(stdout);
    q = url_params_construere(cl, va, numerus_v, p);
    printf("mensura %u (non perventum si ASan)\n", (unsigned)q.mensura);
    return 0;
}
