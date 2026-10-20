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
    Piscina* pp = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(pp);
    const char* s = "<r>\r\n  <b>x</b>\r\n  <c/>\r\n</r>";
    chorda in; Xar* lx; i32 i, n; StmlResultus r; StmlNodus* b; StmlNodus* c;
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    lx = stml_lexemata_colligere(in, pp, it);
    n = xar_numerus(lx);
    per (i = 0; i < n; i++)
    {
        StmlLexema* l = (StmlLexema*)xar_obtinere(lx, i);
        printf("LEX\t%u\tgenus=%d\t[%u,%u)\tlinea=%u col=%u\tcrudum_in_input=[", i, (int)l->genus, l->initium, l->finis, l->linea, l->columna);
        fwrite(s + l->initium, 1, l->finis <= in.mensura ? l->finis - l->initium : 0, stdout);
        printf("]\tvalor=[%.*s]\n", (int)l->valor.mensura, (char*)l->valor.datum);
    }
    r = stml_legere(in, pp, it);
    b = stml_invenire_liberum(r.elementum_radix, "b");
    c = stml_invenire_liberum(r.elementum_radix, "c");
    printf("NODUS_b\t[%u,%u)\tin_input=[%.*s]\texpect=[<b>x</b>]\n", b->positus_initium, b->positus_finis, (int)(b->positus_finis - b->positus_initium), s + b->positus_initium);
    printf("NODUS_c\t[%u,%u)\tin_input=[%.*s]\texpect=[<c/>]\tcrlf=%d\n", c->positus_initium, c->positus_finis, (int)(c->positus_finis - c->positus_initium), s + c->positus_initium, (int)r.crlf_canonicalizatum);
    /* error position on CRLF input */
    {
        const char* e = "<r>\r\n\r\n\r\n  <x>\r\n</r>";
        chorda ei; StmlResultus re;
        ei.datum = (i8*)(size_t)e; ei.mensura = (i32)strlen(e);
        re = stml_legere(ei, pp, it);
        printf("ERROR_POS\tstatus=%d\tlinea=%u\tcolumna=%u\n", (int)re.status, re.linea_erroris, re.columna_erroris);
    }
    return 0;
}
