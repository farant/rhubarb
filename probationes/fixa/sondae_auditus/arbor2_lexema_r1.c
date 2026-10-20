#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "arbor2_lexema.h"
#include <stdio.h>
#include <string.h>

static void currere(const char* s)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    Arbor2Lexator* lex = arbor2_lexator_creare(p, in, s, (i32)strlen(s));
    Arbor2Lexema* t;
    printf("INPUT: \"");
    { const char* q; for (q = s; *q; q++) { if (*q == '\n') printf("\\n"); else putchar(*q); } }
    printf("\"\n");
    while ((t = arbor2_lexema_proximum(lex))->genus != ARBOR2_LEXEMA_EOF) {
        printf("  %-14s '%.*s' linea=%u col=%u\n", arbor2_lexema_genus_nomen(t->genus),
            (int)t->valor.mensura, (const char*)t->valor.datum, (unsigned)t->linea, (unsigned)t->columna);
        if (t->genus == ARBOR2_LEXEMA_NOVA_LINEA) printf("  ** NOVA_LINEA returned by proximum\n");
    }
    piscina_destruere(p);
}

int main(void)
{
    currere("x;\n// c\ny");
    currere("x;\n/* c */\ny");
    currere("x; // c\ny");
    currere("a = .5;");
    currere("a = 1.;");
    currere("a = 1.e5;");
    currere("a = 012.5;");
    currere("a = 09;");
    currere("a = 1.5e3;");
    return 0;
}
