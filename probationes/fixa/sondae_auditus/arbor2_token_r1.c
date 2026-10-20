#include "latina.h"
#include "piscina.h"
#include "arbor2_token.h"
#include <stdio.h>

integer principale(vacuum)
{
    Piscina* piscina;
    Arbor2Lexema* lexema;
    Arbor2Token* tok;
    Arbor2Token* radix;

    piscina = piscina_generare_dynamicum("sonda", 65536);
    lexema = piscina_allocare(piscina, magnitudo(Arbor2Lexema));
    /* exemplum capitis verbatim */
    tok = arbor2_token_ex_lexema(piscina, lexema, "file.c", 10, 5);
    radix = arbor2_token_radix(tok);
    imprimere("radix == tok: %d\n", radix == tok);
    piscina_destruere(piscina);
    redde 0;
}
