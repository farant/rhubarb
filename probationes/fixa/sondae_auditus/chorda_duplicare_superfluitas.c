#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    i8* buf = (i8*)malloc(65536);
    chorda s, r;
    memset(buf, 'a', 65536);
    s.datum = buf; s.mensura = 65536;
    /* 65536 * 65537 = 2^32 + 65536 -> wraps to 65536 */
    printf("case duplicare s.mensura=65536 numerus=65537 (true len 4295032832)\n");
    fflush(stdout);
    r = chorda_duplicare(s, 65537, p);
    printf("mensura=%u\n", r.mensura);
    return 0;
}
