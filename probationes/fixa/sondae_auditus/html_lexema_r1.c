#include "latina.h"
#include "piscina.h"
#include "html_lexema.h"
#include "xar.h"
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
    imprimere("%s\n", fons);
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
    monstrare(p, "<xmp><b>x</b></xmp>");
    monstrare(p, "<iframe><b></iframe>");
    monstrare(p, "<noembed><b></noembed>");
    monstrare(p, "<noframes><b></noframes>");
    monstrare(p, "<plaintext><b></plaintext>");
    monstrare(p, "<a href=/>");
    monstrare(p, "<a href=x/>");
    redde 0;
}
