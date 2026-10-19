#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "url.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_certae_magnitudinis("parva", 64);
    chorda in, r;
    in.datum = (i8*)"                              "; in.mensura = 30;
    printf("ante codificare (90 bytes in piscina 64)\n"); fflush(stdout);
    r = url_codificare(in, p);
    printf("post: datum %s mensura %u\n", r.datum ? "ptr" : "NIHIL", (unsigned)r.mensura);
    return 0;
}
