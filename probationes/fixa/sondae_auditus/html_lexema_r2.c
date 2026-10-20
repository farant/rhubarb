/* lib/html_lexema.c mutatus: '/>' ante valorem post '=' (ordo permutatus) */
#define html_lexare html_lexare_mutatum
#define html_lexemata_emittere html_lexemata_emittere_mutatum
#define html_lexema_genus_nomen html_lexema_genus_nomen_mutatum
#include "html_lexema_r2_mutatum.inc"
#include <stdio.h>
#include <string.h>

interior vacuum
monstrare (Piscina* p, constans character* fons)
{
    Xar* lx;
    i32 k;
    i32 numerus;
    lx = html_lexare(p, fons, (i32)strlen(fons));
    numerus = xar_numerus(lx);
    imprimere("%s (ordo permutatus)\n", fons);
    per (k = 0; k < numerus; k++)
    {
        HtmlLexema* l = (HtmlLexema*)xar_obtinere(lx, k);
        imprimere("  %s '%.*s'\n", html_lexema_genus_nomen(l->genus),
            (integer)l->valor.mensura, (constans character*)l->valor.datum);
    }
}

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    monstrare(p, "<a href=/>");
    monstrare(p, "<a href=x/>");
    redde 0;
}
