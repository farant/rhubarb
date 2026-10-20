#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* pp; InternamentumChorda* it; Xar* x; i32 i; chorda in;
    constans character* s = "<a>x&amp;y</a>";
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    x = stml_lexemata_colligere(in, pp, it);
    per (i = 0; i < xar_numerus(x); i++)
    {
        StmlLexema* l = (StmlLexema*)xar_obtinere(x, i);
        printf("lexema genus=%d valor=[%.*s] [%d,%d)\n", (int)l->genus,
            (int)l->valor.mensura, l->valor.datum ? (char*)l->valor.datum : "", (int)l->initium, (int)l->finis);
    }
    printf("STML_TOKEN_TEXTUS=%d\n", (int)STML_TOKEN_TEXTUS);
    redde 0;
}
