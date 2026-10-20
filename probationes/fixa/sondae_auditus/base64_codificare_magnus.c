#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

/* Ingressus virtualis 3 GiB+ (paginae zero, MAP_ANON) - codificare cum
 * n ubi 4*ceil(n/3) > 2^32-1. argv[1] = n decimalis. */
int main(int argc, char** argv)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    unsigned long n = (argc > 1) ? strtoul(argv[1], NULL, 10) : 3221225470ul;
    size_t mapsz = (size_t)n + 16;
    void* m = mmap(NULL, mapsz, PROT_READ, MAP_ANON | MAP_PRIVATE, -1, 0);
    chorda enc;
    if (m == MAP_FAILED) { printf("mmap fracta\n"); return 3; }
    printf("n=%lu longitudo_codificata=%u (verum %lu)\n", n,
        (unsigned)base64_longitudo_codificata((i32)n), 4ul * ((n + 2ul) / 3ul));
    fflush(stdout);
    enc = base64_codificare((const i8*)m, (i32)n, p);
    printf("redit: datum=%p mensura=%u\n", (void*)enc.datum, (unsigned)enc.mensura);
    return 0;
}
