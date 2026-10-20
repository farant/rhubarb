/* numerus_pairs = count('&')+1 = 2^28+1; magnitudo(chorda)=16; 16*(2^28+1) wraps i32 to 16 */
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
    size_t n = (size_t)1 << 28;
    i8* m = (i8*)malloc(n);
    chorda in;
    UrlParams r;
    printf("magnitudo(chorda)=%lu, n_amp=%lu, producta i32=%u\n",
        (unsigned long)sizeof(chorda), (unsigned long)n,
        (unsigned)((i32)sizeof(chorda) * (i32)(n + 1)));
    fflush(stdout);
    memset(m, '&', n);
    in.datum = m; in.mensura = (i32)n;
    r = url_params_parse(in, p);
    printf("numerus %u (non perventum si ASan)\n", (unsigned)r.numerus);
    return 0;
}
