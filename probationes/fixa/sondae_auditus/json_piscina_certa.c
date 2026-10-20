/* sonda: fixed-size piscina exhausted during parse / write. argv[1]: legere|scribere|creare */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    const char* modus = argc > 1 ? argv[1] : "legere";
    Piscina* dyn = piscina_generare_dynamicum("sonda", 65536);
    Piscina* certa = piscina_generare_certae_magnitudinis("sonda_certa", 2048);
    char* doc; int i; size_t o = 0;
    doc = (char*)malloc(100000);
    doc[o++] = '[';
    for (i = 0; i < 2000; i++) o += (size_t)sprintf(doc + o, "%s\"elementum_%d\"", i ? "," : "", i);
    doc[o++] = ']'; doc[o] = 0;
    setvbuf(stdout, NULL, _IONBF, 0);
    if (strcmp(modus, "legere") == 0) {
        JsonResultus r;
        printf("legere: input %lu bytes into 2048-byte certa piscina\n", (unsigned long)o);
        r = json_legere(chorda_ex_literis(doc, dyn), certa);
        printf("returned: succ=%d err=[%.*s] n=%u\n", (int)r.successus, (int)r.error.mensura,
            r.error.datum ? (const char*)r.error.datum : "", (unsigned)json_tabulatum_numerus(r.radix));
    } else if (strcmp(modus, "scribere") == 0) {
        JsonResultus r = json_legere(chorda_ex_literis(doc, dyn), dyn);
        chorda w;
        printf("scribere: parsed succ=%d, writing into 2048-byte certa piscina\n", (int)r.successus);
        w = json_scribere(r.radix, certa);
        printf("returned: mensura=%u\n", (unsigned)w.mensura);
    } else {
        JsonValor* v; int n = 0;
        printf("creare: json_integer_creare until exhausted\n");
        for (;;) { v = json_integer_creare(certa, 1); if (!v) break; n++; }
        printf("returned NIHIL after %d\n", n);
    }
    return 0;
}
