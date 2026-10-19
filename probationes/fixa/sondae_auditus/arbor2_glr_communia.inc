/* communia sondarum arbor2_glr */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"
#include "arbor2_expandere.h"
#include "arbor2_glr.h"
#include "arbor2_scribere.h"
#include <stdio.h>
#include <string.h>

hic_manens Xar*
_lexare(Piscina* p, InternamentumChorda* intern, constans character* input)
{
    Xar* tokens;
    Arbor2Lexator* lex;
    chorda* via;
    unio { constans character* c; i8* m; } u;

    tokens = xar_creare(p, magnitudo(Arbor2Token*));
    lex = arbor2_lexator_creare(p, intern, input, (i32)strlen(input));
    u.c = "<test>";
    via = piscina_allocare(p, magnitudo(chorda));
    via->datum = u.m;
    via->mensura = VI;
    dum (VERUM)
    {
        Arbor2Lexema* l = arbor2_lexema_proximum(lex);
        Arbor2Token* t;
        Arbor2Token** slot;
        si (l == NIHIL) frange;
        t = arbor2_token_ex_lexema(p, l, via, l->linea, l->columna);
        slot = xar_addere(tokens);
        *slot = t;
        si (l->genus == ARBOR2_LEXEMA_EOF) frange;
    }
    redde tokens;
}
