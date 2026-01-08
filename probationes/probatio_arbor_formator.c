#include "credo.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor_syntaxis.h"
#include "arbor_formator.h"
#include <stdio.h>
#include <string.h>

/* ===========================================================
 * PROBATIO ARBOR FORMATOR
 * =========================================================== */

/* ===========================================================
 * ADIUTORES
 * =========================================================== */

interior ArborNodus*
_parsere_fontem (
               Piscina* piscina,
    InternamentumChorda* intern,
       constans character* fons)
{
    ArborSyntaxis*        syn;
    ArborSyntaxisResultus res;

    syn = arbor_syntaxis_creare(piscina, intern);
    res = arbor_syntaxis_parsere_fontem(
        syn,
        fons,
        (i32)strlen(fons),
        "test.c");

    si (res.successus)
    {
        redde res.radix;
    }

    redde NIHIL;
}

/* ===========================================================
 * PROBATIO - OPTIONES DEFAULT
 * =========================================================== */

interior vacuum
probatio_optiones_default (
    vacuum)
{
    ArborFormatorOptiones opt;

    imprimere("  probatio_optiones_default...\n");

    opt = arbor_formator_optiones_default();

    CREDO_VERUM(opt.latinizare_keywords == VERUM);
    CREDO_VERUM(opt.romanizare_numeros == VERUM);
    CREDO_VERUM(opt.max_linea_longitudo == LXXX);
    CREDO_VERUM(opt.indentatio_spatia == II);
    CREDO_VERUM(opt.latina_via == NIHIL);

    imprimere("    [OK]\n");
}

/* ===========================================================
 * PROBATIO - KEYWORD CONVERSION
 * =========================================================== */

interior vacuum
probatio_keyword_conversion (
    vacuum)
{
    constans character* lat;
    constans character* ang;

    imprimere("  probatio_keyword_conversion...\n");

    /* Anglicum -> Latinum */
    lat = arbor_formator_keyword_ad_latinum("if");
    CREDO_VERUM(lat != NIHIL);
    CREDO_VERUM(strcmp(lat, "si") == ZEPHYRUM);

    lat = arbor_formator_keyword_ad_latinum("return");
    CREDO_VERUM(lat != NIHIL);
    CREDO_VERUM(strcmp(lat, "redde") == ZEPHYRUM);

    lat = arbor_formator_keyword_ad_latinum("while");
    CREDO_VERUM(lat != NIHIL);
    CREDO_VERUM(strcmp(lat, "dum") == ZEPHYRUM);

    lat = arbor_formator_keyword_ad_latinum("struct");
    CREDO_VERUM(lat != NIHIL);
    CREDO_VERUM(strcmp(lat, "structura") == ZEPHYRUM);

    /* Unknown keyword */
    lat = arbor_formator_keyword_ad_latinum("foobar");
    CREDO_VERUM(lat == NIHIL);

    /* Latinum -> Anglicum */
    ang = arbor_formator_keyword_ad_anglicum("si");
    CREDO_VERUM(ang != NIHIL);
    CREDO_VERUM(strcmp(ang, "if") == ZEPHYRUM);

    ang = arbor_formator_keyword_ad_anglicum("redde");
    CREDO_VERUM(ang != NIHIL);
    CREDO_VERUM(strcmp(ang, "return") == ZEPHYRUM);

    ang = arbor_formator_keyword_ad_anglicum("structura");
    CREDO_VERUM(ang != NIHIL);
    CREDO_VERUM(strcmp(ang, "struct") == ZEPHYRUM);

    imprimere("    [OK]\n");
}

/* ===========================================================
 * PROBATIO - ROMAN NUMERALS
 * =========================================================== */

interior vacuum
probatio_roman_numerals (
       Piscina* piscina)
{
    chorda s;

    imprimere("  probatio_roman_numerals...\n");

    /* Zero */
    s = arbor_formator_numerus_ad_romanum(piscina, ZEPHYRUM);
    CREDO_VERUM(s.mensura > ZEPHYRUM);
    CREDO_VERUM(strncmp(( constans character* )s.datum, "ZEPHYRUM", ( memoriae_index )s.mensura) == ZEPHYRUM);

    /* Basic numbers */
    s = arbor_formator_numerus_ad_romanum(piscina, I);
    CREDO_VERUM(s.mensura == I);
    CREDO_VERUM(s.datum[ZEPHYRUM] == 'I');

    s = arbor_formator_numerus_ad_romanum(piscina, V);
    CREDO_VERUM(s.mensura == I);
    CREDO_VERUM(s.datum[ZEPHYRUM] == 'V');

    s = arbor_formator_numerus_ad_romanum(piscina, X);
    CREDO_VERUM(s.mensura == I);
    CREDO_VERUM(s.datum[ZEPHYRUM] == 'X');

    /* Compound numbers */
    s = arbor_formator_numerus_ad_romanum(piscina, IV);
    CREDO_VERUM(s.mensura == II);
    CREDO_VERUM(strncmp(( constans character* )s.datum, "IV", II) == ZEPHYRUM);

    s = arbor_formator_numerus_ad_romanum(piscina, IX);
    CREDO_VERUM(s.mensura == II);
    CREDO_VERUM(strncmp(( constans character* )s.datum, "IX", II) == ZEPHYRUM);

    s = arbor_formator_numerus_ad_romanum(piscina, XLII);
    CREDO_VERUM(strncmp(( constans character* )s.datum, "XLII", ( memoriae_index )s.mensura) == ZEPHYRUM);

    /* Larger numbers */
    s = arbor_formator_numerus_ad_romanum(piscina, C);
    CREDO_VERUM(s.mensura == I);
    CREDO_VERUM(s.datum[ZEPHYRUM] == 'C');

    s = arbor_formator_numerus_ad_romanum(piscina, M);
    CREDO_VERUM(s.mensura == I);
    CREDO_VERUM(s.datum[ZEPHYRUM] == 'M');

    /* Negative */
    s = arbor_formator_numerus_ad_romanum(piscina, ( s64 )-V);
    CREDO_VERUM(s.datum[ZEPHYRUM] == '-');
    CREDO_VERUM(s.datum[I] == 'V');

    /* Over 4096 - should return decimal */
    s = arbor_formator_numerus_ad_romanum(piscina, 5000);
    CREDO_VERUM(s.mensura == IV);
    CREDO_VERUM(strncmp(( constans character* )s.datum, "5000", IV) == ZEPHYRUM);

    imprimere("    [OK]\n");
}

/* ===========================================================
 * PROBATIO - FIDELIS SIMPLE
 * =========================================================== */

interior vacuum
probatio_fidelis_simple (
               Piscina* piscina,
    InternamentumChorda* intern)
{
    ArborNodus* radix;
    chorda      fructus;

    imprimere("  probatio_fidelis_simple...\n");

    /* Simple function */
    radix = _parsere_fontem(piscina, intern, "int main() { return 0; }");
    CREDO_VERUM(radix != NIHIL);

    fructus = arbor_formator_emittere_fidelis(piscina, radix);
    CREDO_VERUM(fructus.mensura > ZEPHYRUM);

    /* Verify contains key elements */
    CREDO_VERUM(fructus.datum != NIHIL);

    imprimere("    fructus: ");
    fwrite(fructus.datum, I, ( memoriae_index )fructus.mensura, stdout);
    imprimere("\n");

    imprimere("    [OK]\n");
}

/* ===========================================================
 * PROBATIO - FIDELIS DECLARATION
 * =========================================================== */

interior vacuum
probatio_fidelis_declaration (
               Piscina* piscina,
    InternamentumChorda* intern)
{
    ArborNodus* radix;
    chorda      fructus;

    imprimere("  probatio_fidelis_declaration...\n");

    radix = _parsere_fontem(piscina, intern, "int x = 42;");
    CREDO_VERUM(radix != NIHIL);

    fructus = arbor_formator_emittere_fidelis(piscina, radix);
    CREDO_VERUM(fructus.mensura > ZEPHYRUM);

    imprimere("    fructus: ");
    fwrite(fructus.datum, I, ( memoriae_index )fructus.mensura, stdout);
    imprimere("\n");

    imprimere("    [OK]\n");
}

/* ===========================================================
 * PROBATIO - FIDELIS CONTROL FLOW
 * =========================================================== */

interior vacuum
probatio_fidelis_control_flow (
               Piscina* piscina,
    InternamentumChorda* intern)
{
    ArborNodus* radix;
    chorda      fructus;

    imprimere("  probatio_fidelis_control_flow...\n");

    radix = _parsere_fontem(piscina, intern,
        "void test() { if (x) { y = 1; } else { y = 2; } }");
    CREDO_VERUM(radix != NIHIL);

    fructus = arbor_formator_emittere_fidelis(piscina, radix);
    CREDO_VERUM(fructus.mensura > ZEPHYRUM);

    imprimere("    fructus: ");
    fwrite(fructus.datum, I, ( memoriae_index )fructus.mensura, stdout);
    imprimere("\n");

    imprimere("    [OK]\n");
}

/* ===========================================================
 * PROBATIO - FIDELIS STRUCT
 * =========================================================== */

interior vacuum
probatio_fidelis_struct (
               Piscina* piscina,
    InternamentumChorda* intern)
{
    ArborNodus* radix;
    chorda      fructus;

    imprimere("  probatio_fidelis_struct...\n");

    radix = _parsere_fontem(piscina, intern,
        "struct Point { int x; int y; };");
    CREDO_VERUM(radix != NIHIL);

    fructus = arbor_formator_emittere_fidelis(piscina, radix);
    CREDO_VERUM(fructus.mensura > ZEPHYRUM);

    imprimere("    fructus: ");
    fwrite(fructus.datum, I, ( memoriae_index )fructus.mensura, stdout);
    imprimere("\n");

    imprimere("    [OK]\n");
}

/* ===========================================================
 * PROBATIO - FIDELIS EXPRESSIONS
 * =========================================================== */

interior vacuum
probatio_fidelis_expressions (
               Piscina* piscina,
    InternamentumChorda* intern)
{
    ArborNodus* radix;
    chorda      fructus;

    imprimere("  probatio_fidelis_expressions...\n");

    radix = _parsere_fontem(piscina, intern,
        "int f() { return a + b * c - d / e; }");
    CREDO_VERUM(radix != NIHIL);

    fructus = arbor_formator_emittere_fidelis(piscina, radix);
    CREDO_VERUM(fructus.mensura > ZEPHYRUM);

    imprimere("    fructus: ");
    fwrite(fructus.datum, I, ( memoriae_index )fructus.mensura, stdout);
    imprimere("\n");

    imprimere("    [OK]\n");
}

/* ===========================================================
 * PROBATIO - FIDELIS ROUNDTRIP (Exact Match)
 *
 * These tests verify byte-for-byte roundtrip preservation.
 * They should FAIL until parser properly captures punctuation.
 * =========================================================== */

interior vacuum
_credo_roundtrip (
               Piscina* piscina,
    InternamentumChorda* intern,
       constans character* input,
       constans character* titulus)
{
    ArborNodus* radix;
    chorda      fructus;
    memoriae_index input_len;

    imprimere("    %s: ", titulus);

    radix = _parsere_fontem(piscina, intern, input);
    si (radix == NIHIL)
    {
        imprimere("[FAIL - parse error]\n");
        CREDO_VERUM(radix != NIHIL);
        redde;
    }

    fructus = arbor_formator_emittere_fidelis(piscina, radix);
    input_len = (memoriae_index)strlen(input);

    /* Print what we got for debugging */
    imprimere("'");
    fwrite(fructus.datum, I, (memoriae_index)fructus.mensura, stdout);
    imprimere("' ");

    /* Check exact length match */
    si (fructus.mensura != (i32)input_len)
    {
        imprimere("[FAIL - length %d != %d]\n",
            (int)fructus.mensura, (int)input_len);
        CREDO_VERUM(fructus.mensura == (i32)input_len);
        redde;
    }

    /* Check exact content match */
    si (strncmp((constans character*)fructus.datum, input, input_len) != ZEPHYRUM)
    {
        imprimere("[FAIL - content mismatch]\n");
        imprimere("      expected: '%s'\n", input);
        CREDO_VERUM(strncmp((constans character*)fructus.datum, input, input_len) == ZEPHYRUM);
        redde;
    }

    imprimere("[OK]\n");
}

interior vacuum
probatio_fidelis_roundtrip (
               Piscina* piscina,
    InternamentumChorda* intern)
{
    imprimere("  probatio_fidelis_roundtrip...\n");

    /* Test declaration with semicolon */
    _credo_roundtrip(piscina, intern,
        "int x = 42;",
        "declaration");

    /* Test function with parentheses */
    _credo_roundtrip(piscina, intern,
        "int main() { return 0; }",
        "function");

    /* Test struct with member semicolons */
    _credo_roundtrip(piscina, intern,
        "struct Point { int x; int y; };",
        "struct");

    /* Test control flow */
    _credo_roundtrip(piscina, intern,
        "void f() { if (x) { y = 1; } }",
        "control");

    /* Test expressions with binary operators */
    _credo_roundtrip(piscina, intern,
        "int f() { return a + b * c - d / e; }",
        "expressions");

    /* Test function call with arguments */
    _credo_roundtrip(piscina, intern,
        "int f() { return foo(a, b, c); }",
        "call");

    /* Test array subscript */
    _credo_roundtrip(piscina, intern,
        "int f() { return arr[i]; }",
        "subscript");

    /* Test pointer declaration */
    _credo_roundtrip(piscina, intern,
        "int *p = &x;",
        "pointer");

    /* Test ternary expression */
    _credo_roundtrip(piscina, intern,
        "int f() { return x ? a : b; }",
        "ternary");

    /* Test assignment expression */
    _credo_roundtrip(piscina, intern,
        "void f() { x = y + z; }",
        "assign");

    /* Test unary operators */
    _credo_roundtrip(piscina, intern,
        "int f() { return -x + !y; }",
        "unary");

    /* Test parenthesized expression */
    _credo_roundtrip(piscina, intern,
        "int f() { return (a + b) * c; }",
        "parens");

    /* Test for loop */
    _credo_roundtrip(piscina, intern,
        "void f() { for (i = 0; i < n; i++) { x++; } }",
        "for");

    /* Test while loop */
    _credo_roundtrip(piscina, intern,
        "void f() { while (x > 0) { x--; } }",
        "while");

    /* Test nested if-else */
    _credo_roundtrip(piscina, intern,
        "void f() { if (a) { if (b) { x = 1; } } else { x = 2; } }",
        "nested-if");

    /* Test complex expression */
    _credo_roundtrip(piscina, intern,
        "int f() { return (a + b) * (c - d) / e; }",
        "complex");

    /* Test member access */
    _credo_roundtrip(piscina, intern,
        "int f() { return p->x + s.y; }",
        "member");

    /* Test comparison operators */
    _credo_roundtrip(piscina, intern,
        "int f() { return a < b && c >= d; }",
        "compare");

    /* Test bitwise operators */
    _credo_roundtrip(piscina, intern,
        "int f() { return a | b & c ^ d; }",
        "bitwise");

    /* Test multiple declarations */
    _credo_roundtrip(piscina, intern,
        "int a, b, c;",
        "multi-decl");

    /* Test function with multiple parameters */
    _credo_roundtrip(piscina, intern,
        "int add(int a, int b) { return a + b; }",
        "func-params");

    imprimere("    [OK]\n");
}

interior vacuum
probatio_fidelis_irregular_whitespace (
               Piscina* piscina,
    InternamentumChorda* intern)
{
    imprimere("  probatio_fidelis_irregular_whitespace...\n");

    /* Double spaces around = */
    _credo_roundtrip(piscina, intern,
        "int x  =  42;",
        "double-space-assign");

    /* No spaces around = */
    _credo_roundtrip(piscina, intern,
        "int x=42;",
        "no-space-assign");

    /* Triple spaces */
    _credo_roundtrip(piscina, intern,
        "int   x   =   42;",
        "triple-space");

    /* Mixed spacing in expression */
    _credo_roundtrip(piscina, intern,
        "int f() { return a+b * c-d; }",
        "mixed-spacing-expr");

    /* No spaces in expression */
    _credo_roundtrip(piscina, intern,
        "int f() { return a+b+c; }",
        "no-space-expr");

    /* Double space after comma */
    _credo_roundtrip(piscina, intern,
        "int f(int a,  int b,  int c) { return a; }",
        "double-space-comma");

    /* No space after comma */
    _credo_roundtrip(piscina, intern,
        "int f(int a,int b,int c) { return a; }",
        "no-space-comma");

    /* Extra space inside parens */
    _credo_roundtrip(piscina, intern,
        "int f( int x ) { return x; }",
        "space-inside-parens");

    /* Extra space inside braces */
    _credo_roundtrip(piscina, intern,
        "void f() {  x = 1;  }",
        "space-inside-braces");

    /* No space before brace */
    _credo_roundtrip(piscina, intern,
        "void f(){ return; }",
        "no-space-before-brace");

    /* Double space before else */
    _credo_roundtrip(piscina, intern,
        "void f() { if (x) { a = 1; }  else { b = 2; } }",
        "double-space-else");

    /* No space around semicolons in for */
    _credo_roundtrip(piscina, intern,
        "void f() { for (i=0;i<n;i++) { x++; } }",
        "compact-for");

    /* Extra spaces in for */
    _credo_roundtrip(piscina, intern,
        "void f() { for (i = 0;  i < n;  i++) { x++; } }",
        "extra-space-for");

    /* Asymmetric spacing */
    _credo_roundtrip(piscina, intern,
        "int x= 42;",
        "asymmetric-left");

    _credo_roundtrip(piscina, intern,
        "int x =42;",
        "asymmetric-right");

    /* Multiple declarations with irregular spacing */
    _credo_roundtrip(piscina, intern,
        "int a,b,  c,   d;",
        "irregular-multi-decl");

    imprimere("    [OK]\n");
}

interior vacuum
probatio_fidelis_complex_structures (
               Piscina* piscina,
    InternamentumChorda* intern)
{
    imprimere("  probatio_fidelis_complex_structures...\n");

    /* Deeply nested control flow */
    _credo_roundtrip(piscina, intern,
        "void f() { if (a) { if (b) { if (c) { x = 1; } } } }",
        "deep-nested-if");

    /* Nested loops */
    _credo_roundtrip(piscina, intern,
        "void f() { for (i = 0; i < n; i++) { for (j = 0; j < m; j++) { x++; } } }",
        "nested-for");

    /* While inside for */
    _credo_roundtrip(piscina, intern,
        "void f() { for (i = 0; i < n; i++) { while (x > 0) { x--; } } }",
        "for-while");

    /* Pointer parameters */
    _credo_roundtrip(piscina, intern,
        "int f(int *a, int **b, int ***c) { return *a; }",
        "pointer-params");

    /* Deeply nested parens */
    _credo_roundtrip(piscina, intern,
        "int f() { return ((((a + b)))); }",
        "deep-parens");

    /* Chained member access */
    _credo_roundtrip(piscina, intern,
        "int f() { return a->b->c->d; }",
        "chained-arrow");

    /* Mixed member access */
    _credo_roundtrip(piscina, intern,
        "int f() { return a.b->c.d->e; }",
        "mixed-member");

    /* Multiple array subscripts */
    _credo_roundtrip(piscina, intern,
        "int f() { return a[i][j][k]; }",
        "multi-subscript");

    /* Complex expression mix */
    _credo_roundtrip(piscina, intern,
        "int f() { return a->b[i].c + d->e[j].f; }",
        "complex-member-subscript");

    /* Cast expressions */
    _credo_roundtrip(piscina, intern,
        "int f() { return (int)x; }",
        "cast-simple");

    _credo_roundtrip(piscina, intern,
        "int f() { return (long)y + (char)z; }",
        "cast-multi");

    _credo_roundtrip(piscina, intern,
        "int f() { return (int)(x + y); }",
        "cast-expr");

    /* Sizeof expression */
    _credo_roundtrip(piscina, intern,
        "int f() { return sizeof(int) + sizeof(x); }",
        "sizeof");

    _credo_roundtrip(piscina, intern,
        "int f() { return sizeof (int); }",
        "sizeof-space");

    /* Pre/post increment/decrement */
    _credo_roundtrip(piscina, intern,
        "void f() { ++a; --b; c++; d--; }",
        "inc-dec");

    /* Address-of and dereference */
    _credo_roundtrip(piscina, intern,
        "void f() { int *p = &x; int y = *p; }",
        "addr-deref");

    /* Complex conditional */
    _credo_roundtrip(piscina, intern,
        "int f() { return a ? b ? c : d : e ? f : g; }",
        "nested-ternary");

    /* Logical operators */
    _credo_roundtrip(piscina, intern,
        "int f() { return a && b || c && d || e; }",
        "logical-chain");

    /* Compound assignment */
    _credo_roundtrip(piscina, intern,
        "void f() { a += b; c -= d; e *= f; g /= h; }",
        "compound-assign");

    /* Comma expression in for */
    _credo_roundtrip(piscina, intern,
        "void f() { for (i = 0, j = 0; i < n; i++, j++) { x++; } }",
        "comma-in-for");

    /* Do-while */
    _credo_roundtrip(piscina, intern,
        "void f() { do { x++; } while (x < 10); }",
        "do-while");

    /* Break and continue */
    _credo_roundtrip(piscina, intern,
        "void f() { while (1) { if (a) break; if (b) continue; } }",
        "break-continue");

    /* Return with complex expression */
    _credo_roundtrip(piscina, intern,
        "int f() { return (a + b) * (c - d) / (e + f) % (g - h); }",
        "complex-return");

    /* Multiple statements */
    _credo_roundtrip(piscina, intern,
        "void f() { int a; int b; a = 1; b = 2; }",
        "multi-statement");

    imprimere("    [OK]\n");
}

interior vacuum
probatio_fidelis_tabs_newlines (
               Piscina* piscina,
    InternamentumChorda* intern)
{
    imprimere("  probatio_fidelis_tabs_newlines...\n");

    /* Tab instead of space - basic */
    _credo_roundtrip(piscina, intern,
        "int\tx\t=\t42;",
        "tabs-basic");

    /* Mixed tabs and spaces */
    _credo_roundtrip(piscina, intern,
        "int x\t= 42;",
        "mixed-tab-space");

    /* Tabs in expressions */
    _credo_roundtrip(piscina, intern,
        "int f() { return a\t+\tb; }",
        "tabs-expr");

    /* Tabs around braces */
    _credo_roundtrip(piscina, intern,
        "void f()\t{\tx = 1;\t}",
        "tabs-braces");

    /* Tabs in for loop */
    _credo_roundtrip(piscina, intern,
        "void f() { for (i\t=\t0;\ti\t<\tn;\ti++) { x++; } }",
        "tabs-for");

    /* Tabs in if */
    _credo_roundtrip(piscina, intern,
        "void f() { if\t(x)\t{\ty = 1;\t} }",
        "tabs-if");

    /* Return keyword trivia now preserved */
    _credo_roundtrip(piscina, intern,
        "int\tf()\t{\treturn\t0;\t}",
        "tabs-function");

    /* Newline tests */
    _credo_roundtrip(piscina, intern,
        "void f() {\nx = 1;\n}",
        "newline-basic");

    /* Newline with indentation */
    _credo_roundtrip(piscina, intern,
        "void f() {\n    x = 1;\n}",
        "newline-indent");

    /* Multiple lines */
    _credo_roundtrip(piscina, intern,
        "void f() {\n    int a;\n    int b;\n}",
        "newline-multi");

    /* Newline in if-else */
    _credo_roundtrip(piscina, intern,
        "void f() {\n    if (x) {\n        y = 1;\n    }\n}",
        "newline-if");

    /* Newline before opening brace */
    _credo_roundtrip(piscina, intern,
        "void f()\n{\n    x = 1;\n}",
        "newline-before-brace");

    /* Declaration on separate line */
    _credo_roundtrip(piscina, intern,
        "int x;\nint y;",
        "newline-decls");

    imprimere("    [OK]\n");
}

/* ===========================================================
 * PRINCIPALE
 * =========================================================== */

s32
main (
    vacuum)
{
               Piscina* piscina;
    InternamentumChorda* intern;
                    b32  praeteritus;

    imprimere("=== PROBATIO ARBOR FORMATOR ===\n\n");

    piscina = piscina_generare_dynamicum("probatio_formator", LXIV * M);
    credo_aperire(piscina);
    CREDO_VERUM(piscina != NIHIL);

    intern = internamentum_creare(piscina);
    CREDO_VERUM(intern != NIHIL);

    /* Run tests */
    probatio_optiones_default();
    probatio_keyword_conversion();
    probatio_roman_numerals(piscina);
    probatio_fidelis_simple(piscina, intern);
    probatio_fidelis_declaration(piscina, intern);
    probatio_fidelis_control_flow(piscina, intern);
    probatio_fidelis_struct(piscina, intern);
    probatio_fidelis_expressions(piscina, intern);
    probatio_fidelis_roundtrip(piscina, intern);
    probatio_fidelis_irregular_whitespace(piscina, intern);
    probatio_fidelis_complex_structures(piscina, intern);
    probatio_fidelis_tabs_newlines(piscina, intern);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
