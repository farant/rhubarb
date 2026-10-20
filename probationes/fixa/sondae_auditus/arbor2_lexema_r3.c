#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "arbor2_lexema.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char* s = "x\n\ny\r\nz";
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    Arbor2Lexator* lex = arbor2_lexator_creare(p, in, s, (i32)strlen(s));
    Arbor2Lexema* t;
    i32 k;
    for (;;) {
        t = arbor2_lexema_proximum(lex);
        printf("TOKEN %s\n", arbor2_lexema_genus_nomen(t->genus));
        if (t->spatia_ante) for (k = 0; k < xar_numerus(t->spatia_ante); k++)
            printf("   ante: %s\n", arbor2_lexema_genus_nomen((*(Arbor2Lexema**)xar_obtinere(t->spatia_ante, k))->genus));
        if (t->spatia_post) for (k = 0; k < xar_numerus(t->spatia_post); k++)
            printf("   post: %s\n", arbor2_lexema_genus_nomen((*(Arbor2Lexema**)xar_obtinere(t->spatia_post, k))->genus));
        if (t->genus == ARBOR2_LEXEMA_EOF) break;
    }
    piscina_destruere(p);
    return 0;
}
