#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda s = chorda_ex_literis("a", p);
    chorda d = chorda_ex_literis("abc", p);
    chorda_fissio_fructus f;
    printf("case delim longer than s: s=\"a\" delim=\"abc\"\n");
    fflush(stdout);
    f = chorda_fissio_chorda(s, d, p);
    printf("numerus=%u\n", f.numerus);
    return 0;
}
