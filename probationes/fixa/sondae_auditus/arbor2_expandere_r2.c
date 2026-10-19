#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "arbor2_expandere.h"
#include <stdio.h>
#include <string.h>

static void currere(const char* s)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    Arbor2Expansion* e = arbor2_expansion_creare(p, in);
    Xar* out; int k, cnt, shown = 0;
    out = arbor2_expansion_processare(e, s, (i32)strlen(s), "t.c");
    cnt = (int)xar_numerus(out);
    printf("INPUT %-40s -> %d tokens, %d layers:", s, cnt, (int)xar_numerus(arbor2_expansion_obtinere_layers(e)));
    for (k = 0; k < cnt && shown < 24; k++, shown++) {
        Arbor2Token* t = *(Arbor2Token**)xar_obtinere(out, (i32)k);
        printf(" %.*s", (int)t->lexema->valor.mensura, (const char*)t->lexema->valor.datum);
    }
    printf("%s\n", cnt > 24 ? " ..." : "");
    piscina_destruere(p);
}

int main(void)
{
    currere("#define F (1+F)\nint x = F;\n");
    currere("#define G(x) (x+G(x))\nG(2)\n");
    currere("#define A B\n#define B A\nA\n");
    currere("#define SELF SELF\nSELF\n");
    return 0;
}
