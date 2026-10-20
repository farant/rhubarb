/* mensura = 1431655766 bytes of ' ' (reserved): longitudo_output = 3*n
 * = 4294967298 wraps i32 to 2 (url.c:87), allocation of 2 bytes, then 3*n writes. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "url.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    i32 n = 1431655766U;
    i8* m = (i8*)malloc((size_t)n);
    chorda in, r;
    if (!m) { printf("malloc fractum\n"); return 3; }
    memset(m, ' ', (size_t)n);
    printf("n=%u, 3*n i32=%u\n", (unsigned)n, (unsigned)(n * 3U));
    fflush(stdout);
    in.datum = m; in.mensura = n;
    r = url_codificare(in, p);
    printf("mensura %u (non perventum si ASan)\n", (unsigned)r.mensura);
    return 0;
}
