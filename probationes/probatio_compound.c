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
    test(p, intern, "struct S { volatile const int x; };");
    
    imprimere("\n-- Combined qualifiers with compound types --\n");
    test(p, intern, "struct S { const unsigned int x; };");
    test(p, intern, "struct S { volatile long int x; };");

    imprimere("\n-- const + unsigned/signed variants --\n");
    test(p, intern, "struct S { const unsigned int x; };");
    test(p, intern, "struct S { const unsigned char c; };");
    test(p, intern, "struct S { const unsigned long x; };");
    test(p, intern, "struct S { const unsigned short x; };");
    test(p, intern, "struct S { const unsigned long int x; };");
    test(p, intern, "struct S { const unsigned short int x; };");
    test(p, intern, "struct S { const unsigned long long x; };");
    test(p, intern, "struct S { const signed int x; };");
    test(p, intern, "struct S { const signed char c; };");
    test(p, intern, "struct S { const signed long x; };");
    test(p, intern, "struct S { const signed short x; };");
    test(p, intern, "struct S { const signed long int x; };");
    test(p, intern, "struct S { const signed short int x; };");
    test(p, intern, "struct S { const signed long long x; };");
    test(p, intern, "struct S { const long int x; };");
    test(p, intern, "struct S { const long long x; };");
    test(p, intern, "struct S { const short int x; };");

    imprimere("\n-- volatile + unsigned/signed variants --\n");
    test(p, intern, "struct S { volatile unsigned int x; };");
    test(p, intern, "struct S { volatile unsigned char c; };");
    test(p, intern, "struct S { volatile unsigned long x; };");
    test(p, intern, "struct S { volatile unsigned short x; };");
    test(p, intern, "struct S { volatile unsigned long int x; };");
    test(p, intern, "struct S { volatile unsigned short int x; };");
    test(p, intern, "struct S { volatile unsigned long long x; };");
    test(p, intern, "struct S { volatile signed int x; };");
    test(p, intern, "struct S { volatile signed char c; };");
    test(p, intern, "struct S { volatile signed long x; };");
    test(p, intern, "struct S { volatile signed short x; };");
    test(p, intern, "struct S { volatile signed long int x; };");
    test(p, intern, "struct S { volatile signed short int x; };");
    test(p, intern, "struct S { volatile signed long long x; };");
    test(p, intern, "struct S { volatile long int x; };");

    imprimere("\n-- Top-level declarations (for comparison) --\n");
    test(p, intern, "unsigned int x;");
    test(p, intern, "const int x;");
    test(p, intern, "long long x;");

    imprimere("\n-- Top-level const + compound types (items 7a/7b) --\n");
    test(p, intern, "const unsigned int x;");
    test(p, intern, "const signed int x;");
    test(p, intern, "const unsigned char c;");
    test(p, intern, "const signed char c;");
    test(p, intern, "const long int x;");
    test(p, intern, "const short int x;");
    test(p, intern, "const unsigned long x;");
    test(p, intern, "const unsigned short x;");
    test(p, intern, "const signed long x;");
    test(p, intern, "const signed short x;");
    test(p, intern, "const unsigned long int x;");
    test(p, intern, "const unsigned short int x;");
    test(p, intern, "const signed long int x;");
    test(p, intern, "const signed short int x;");
    test(p, intern, "const long long x;");
    test(p, intern, "const unsigned long long x;");
    test(p, intern, "const signed long long x;");

    imprimere("\n-- Top-level volatile + compound types --\n");
    test(p, intern, "volatile unsigned int x;");
    test(p, intern, "volatile signed int x;");
    test(p, intern, "volatile unsigned char c;");
    test(p, intern, "volatile signed char c;");
    test(p, intern, "volatile long int x;");
    test(p, intern, "volatile short int x;");
    test(p, intern, "volatile unsigned long x;");
    test(p, intern, "volatile unsigned short x;");
    test(p, intern, "volatile signed long x;");
    test(p, intern, "volatile signed short x;");
    test(p, intern, "volatile unsigned long int x;");
    test(p, intern, "volatile unsigned short int x;");
    test(p, intern, "volatile signed long int x;");
    test(p, intern, "volatile signed short int x;");
    test(p, intern, "volatile long long x;");
    test(p, intern, "volatile unsigned long long x;");
    test(p, intern, "volatile signed long long x;");

    imprimere("\n=== Qualifier order preservation tests ===\n\n");

    imprimere("-- Top-level const volatile --\n");
    test(p, intern, "const volatile int x;");
    test(p, intern, "volatile const int x;");
    test(p, intern, "const volatile char c;");
    test(p, intern, "volatile const char c;");

    imprimere("\n-- Struct member const volatile (both orders) --\n");
    test(p, intern, "struct S { const volatile int x; };");
    test(p, intern, "struct S { volatile const int x; };");
    test(p, intern, "struct S { const volatile char c; };");
    test(p, intern, "struct S { volatile const char c; };");
    test(p, intern, "struct S { const volatile float f; };");
    test(p, intern, "struct S { volatile const double d; };");

    imprimere("\n-- Basic pointers --\n");
    test(p, intern, "int *p;");
    test(p, intern, "int *const p;");
    test(p, intern, "int *volatile p;");

    imprimere("\n-- Pointers with const volatile --\n");
    test(p, intern, "int *const volatile p;");
    test(p, intern, "int *volatile const p;");
    test(p, intern, "const int *const volatile p;");
    test(p, intern, "volatile int *volatile const p;");

    imprimere("\n-- Double pointer with qualifiers --\n");
    test(p, intern, "int **const volatile p;");
    test(p, intern, "int **volatile const p;");
    test(p, intern, "int *const *volatile p;");
    test(p, intern, "int *volatile *const p;");

    imprimere("\n-- Struct member pointers with qualifiers --\n");
    test(p, intern, "struct S { int *const volatile p; };");
    test(p, intern, "struct S { int *volatile const p; };");
    test(p, intern, "struct S { const volatile int *p; };");
    test(p, intern, "struct S { volatile const int *p; };");

    imprimere("\n-- Mixed qualifiers and type modifiers --\n");
    test(p, intern, "const volatile unsigned int x;");
    test(p, intern, "volatile const unsigned int x;");
    test(p, intern, "const volatile long int x;");
    test(p, intern, "volatile const long int x;");

    imprimere("\n=== Trailing semicolon tests ===\n\n");

    imprimere("-- Struct definitions with trailing ; --\n");
    test(p, intern, "struct S { int x; };");
    test(p, intern, "struct T { int a; int b; };");
    test(p, intern, "struct { int x; };");

    imprimere("\n-- Struct with variable declaration --\n");
    test(p, intern, "struct S { int x; } s;");
    test(p, intern, "struct { int x; } anon;");

    imprimere("\n-- Enum definitions with trailing ; --\n");
    test(p, intern, "enum E { A, B, C };");
    test(p, intern, "enum E { A = 1, B = 2 };");
    test(p, intern, "enum { X, Y, Z };");

    imprimere("\n-- Enum with variable declaration --\n");
    test(p, intern, "enum E { A, B } e;");
    test(p, intern, "enum { X, Y } anon;");

    imprimere("\n-- Forward declarations --\n");
    test(p, intern, "struct S;");
    test(p, intern, "enum E;");

    piscina_destruere(p);
    redde 0;
}
