#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "css_lexema.h"
#include <stdio.h>
#include <string.h>

interior vacuum
monstrare (Piscina* piscina, constans character* fons)
{
    Xar* lexemata;
    i32 k;
    i32 numerus;

    lexemata = css_lexare(piscina, fons, (i32)strlen(fons));
    numerus = (i32)xar_numerus(lexemata);
    imprimere("fons: %s\n", fons);
    per (k = 0; k < numerus; k++)
    {
        CssLexema* lex = (CssLexema*)xar_obtinere(lexemata, k);
        imprimere("  %s [%.*s]\n", css_lexema_genus_nomen(lex->genus),
                  (integer)lex->valor.mensura, (constans character*)lex->valor.datum);
    }
}

integer
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("sonda", 65536);
    monstrare(piscina, "a{b:url(x)}");
    monstrare(piscina, "a{b:u\\72l(x)}");
    monstrare(piscina, "a{b:u\\72l(x y)}");
    redde 0;
}
