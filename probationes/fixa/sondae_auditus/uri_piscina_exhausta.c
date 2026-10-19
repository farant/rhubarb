#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "uri.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_certae_magnitudinis("sonda_parva", 48);
    Uri u; b32 ok;
    printf("ante uri_parse in piscina 48 bytes\n"); fflush(stdout);
    ok = uri_parse("https://user:pass@example.com:8080/a/very/long/path/that/does/not/fit?q=1#frag", &u, p);
    printf("post uri_parse ok=%u (exp FALSUM, non exitus)\n", (unsigned)ok);
    piscina_destruere(p);
    return 0;
}
