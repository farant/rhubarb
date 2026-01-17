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

hic_manens Xar* _lexare(Piscina* p, InternamentumChorda* intern, constans character* input)
{
    Xar* tokens = xar_creare(p, magnitudo(Arbor2Token*));
    Arbor2Lexator* lex = arbor2_lexator_creare(p, intern, input, (i32)strlen(input));
    chorda* via;
    unio { constans character* c; i8* m; } u;
    u.c = "<test>";
    via = piscina_allocare(p, magnitudo(chorda));
    via->datum = u.m;
    via->mensura = VI;
    dum (VERUM) {
        Arbor2Lexema* l = arbor2_lexema_proximum(lex);
        Arbor2Token* t;
        si (l == NIHIL) frange;
        t = arbor2_token_ex_lexema(p, l, via, l->linea, l->columna);
        *(Arbor2Token**)xar_addere(tokens) = t;
        si (l->genus == ARBOR2_LEXEMA_EOF) frange;
    }
    redde tokens;
}

hic_manens vacuum test(Piscina* p, InternamentumChorda* intern, constans character* input)
{
    Arbor2Expansion* exp = arbor2_expansion_creare(p, intern);
    Arbor2GLR* glr = arbor2_glr_creare(p, intern, exp);
    Xar* tokens = _lexare(p, intern, input);
    Arbor2GLRResultus res = arbor2_glr_parsere_translation_unit(glr, tokens);
    chorda* output;
    imprimere("'%s'\n  -> ", input);
    si (!res.successus) { imprimere("PARSE FAILED\n"); redde; }
    output = arbor2_scribere(p, res.radix);
    si (output->mensura == (i32)strlen(input) && memcmp(output->datum, input, (size_t)output->mensura) == 0)
        imprimere("OK\n");
    alioquin
        imprimere("MISMATCH: '%.*s'\n", (integer)output->mensura, output->datum);
}

s32 principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", 1048576);
    InternamentumChorda* intern = internamentum_creare(p);
    
    imprimere("=== Compound type specifiers in struct members ===\n\n");
    
    imprimere("-- unsigned/signed variants --\n");
    test(p, intern, "struct S { unsigned int x; };");
    test(p, intern, "struct S { signed int x; };");
    test(p, intern, "struct S { unsigned char c; };");
    test(p, intern, "struct S { signed char c; };");
    
    imprimere("\n-- long/short variants --\n");
    test(p, intern, "struct S { long int x; };");
    test(p, intern, "struct S { short int x; };");
    test(p, intern, "struct S { unsigned long x; };");
    test(p, intern, "struct S { unsigned short x; };");
    
    imprimere("\n-- long long (C99, but common) --\n");
    test(p, intern, "struct S { long long x; };");
    test(p, intern, "struct S { unsigned long long x; };");
    
    imprimere("\n-- const/volatile qualifiers --\n");
    test(p, intern, "struct S { const int x; };");
    test(p, intern, "struct S { volatile int x; };");
    test(p, intern, "struct S { const volatile int x; };");
    
    imprimere("\n-- Combined qualifiers with compound types --\n");
    test(p, intern, "struct S { const unsigned int x; };");
    test(p, intern, "struct S { volatile long int x; };");
    
    imprimere("\n-- Top-level declarations (for comparison) --\n");
    test(p, intern, "unsigned int x;");
    test(p, intern, "const int x;");
    test(p, intern, "long long x;");
    
    piscina_destruere(p);
    redde 0;
}
