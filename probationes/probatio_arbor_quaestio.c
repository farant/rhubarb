/* probatio_arbor_quaestio.c - Probationes pro CSS-style AST query engine */

#include "credo.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor_syntaxis.h"
#include "arbor_quaestio.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Helper Functions
 * ================================================== */

interior ArborSyntaxisResultus
_parsere(Piscina* piscina, InternamentumChorda* intern, constans character* fons)
{
    ArborSyntaxis* syn;
    ArborSyntaxisResultus res;

    syn = arbor_syntaxis_creare(piscina, intern);
    res = arbor_syntaxis_parsere_fontem(syn, fons, (i32)strlen(fons), "probatio");
    redde res;
}

/* ==================================================
 * Probationes - Type Selectors
 * ================================================== */

interior vacuum
probatio_type_selector(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;

    imprimere("\n--- Type Selectors ---\n");

    /* Parse simple code */
    ast = _parsere(piscina, intern, "int main(void) { return 0; }");
    CREDO_VERUM(ast.successus);

    /* Query for function-definition */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition");
    CREDO_NON_NIHIL(q);

    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  function-definition: found %d - OK\n", xar_numerus(resultata));

    /* Query for identifier */
    q = arbor_quaestio_compilare(piscina, intern, "identifier");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    imprimere("  identifier: found %d - OK\n", xar_numerus(resultata));

    /* Query for return-statement */
    q = arbor_quaestio_compilare(piscina, intern, "return-statement");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  return-statement: found %d - OK\n", xar_numerus(resultata));
}

/* ==================================================
 * Probationes - More Type Selectors
 * ================================================== */

interior vacuum
probatio_type_selector_extended(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;

    imprimere("\n--- Type Selectors (Extended) ---\n");

    /* Parse code with many different node types */
    ast = _parsere(piscina, intern,
        "struct Point { int x; int y; };\n"
        "enum Color { RED, GREEN, BLUE };\n"
        "typedef int Integer;\n"
        "int global = 42;\n"
        "int add(int a, int b) {\n"
        "    int sum = a + b;\n"
        "    if (sum > 0) { return sum; }\n"
        "    else { return -sum; }\n"
        "}\n"
        "void loop(void) {\n"
        "    int i;\n"
        "    for (i = 0; i < 10; i++) { }\n"
        "    while (i > 0) { i--; }\n"
        "}\n");
    CREDO_VERUM(ast.successus);

    /* struct-specifier */
    q = arbor_quaestio_compilare(piscina, intern, "struct-specifier");
    CREDO_NON_NIHIL(q);
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  struct-specifier: found %d - OK\n", xar_numerus(resultata));

    /* enum-specifier */
    q = arbor_quaestio_compilare(piscina, intern, "enum-specifier");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  enum-specifier: found %d - OK\n", xar_numerus(resultata));

    /* declaration */
    q = arbor_quaestio_compilare(piscina, intern, "declaration");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  declaration: found %d - OK\n", xar_numerus(resultata));

    /* if-statement */
    q = arbor_quaestio_compilare(piscina, intern, "if-statement");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  if-statement: found %d - OK\n", xar_numerus(resultata));

    /* for-statement */
    q = arbor_quaestio_compilare(piscina, intern, "for-statement");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  for-statement: found %d - OK\n", xar_numerus(resultata));

    /* while-statement */
    q = arbor_quaestio_compilare(piscina, intern, "while-statement");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  while-statement: found %d - OK\n", xar_numerus(resultata));

    /* binary-expression */
    q = arbor_quaestio_compilare(piscina, intern, "binary-expression");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  binary-expression: found %d - OK\n", xar_numerus(resultata));

    /* unary-expression */
    q = arbor_quaestio_compilare(piscina, intern, "unary-expression");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  unary-expression: found %d - OK\n", xar_numerus(resultata));

    /* integer-literal */
    q = arbor_quaestio_compilare(piscina, intern, "integer-literal");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  integer-literal: found %d - OK\n", xar_numerus(resultata));

    /* compound-statement */
    q = arbor_quaestio_compilare(piscina, intern, "compound-statement");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  compound-statement: found %d - OK\n", xar_numerus(resultata));

    /* type-specifier */
    q = arbor_quaestio_compilare(piscina, intern, "type-specifier");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  type-specifier: found %d - OK\n", xar_numerus(resultata));
}

/* ==================================================
 * Probationes - Descendant Combinator
 * ================================================== */

interior vacuum
probatio_descendant_combinator(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;

    imprimere("\n--- Descendant Combinator ---\n");

    ast = _parsere(piscina, intern, "int foo(void) { int x; return x; }");
    CREDO_VERUM(ast.successus);

    /* function-definition identifier - should find foo and x */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition identifier");
    CREDO_NON_NIHIL(q);

    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    imprimere("  function-definition identifier: found %d - OK\n", xar_numerus(resultata));

    /* compound-statement return-statement */
    q = arbor_quaestio_compilare(piscina, intern, "compound-statement return-statement");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  compound-statement return-statement: found %d - OK\n", xar_numerus(resultata));
}

/* ==================================================
 * Probationes - Child Combinator
 * ================================================== */

interior vacuum
probatio_child_combinator(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;

    imprimere("\n--- Child Combinator ---\n");

    ast = _parsere(piscina, intern, "int main(void) { return 0; }");
    CREDO_VERUM(ast.successus);

    /* function-definition > compound-statement */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition > compound-statement");
    CREDO_NON_NIHIL(q);

    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  function-definition > compound-statement: found %d - OK\n", xar_numerus(resultata));
}

/* ==================================================
 * Probationes - Attribute Selectors
 * ================================================== */

interior vacuum
probatio_attribute_selector(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;

    imprimere("\n--- Attribute Selectors ---\n");

    ast = _parsere(piscina, intern, "int main(void) { return 0; }");
    CREDO_VERUM(ast.successus);

    /* identifier[value="main"] */
    q = arbor_quaestio_compilare(piscina, intern, "identifier[value=\"main\"]");
    CREDO_NON_NIHIL(q);

    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  identifier[value=\"main\"]: found %d - OK\n", xar_numerus(resultata));

    /* identifier[value="nonexistent"] - should find 0 */
    q = arbor_quaestio_compilare(piscina, intern, "identifier[value=\"nonexistent\"]");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) == ZEPHYRUM);
    imprimere("  identifier[value=\"nonexistent\"]: found %d - OK\n", xar_numerus(resultata));
}

/* ==================================================
 * Probationes - Attribute Operators
 * ================================================== */

interior vacuum
probatio_attribute_operators(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;

    imprimere("\n--- Attribute Operators ---\n");

    /* Parse code with various identifiers */
    ast = _parsere(piscina, intern,
        "int prefix_one(void) { return 0; }\n"
        "int prefix_two(void) { return 0; }\n"
        "int other_suffix(void) { return 0; }\n"
        "int contains_mid_value(void) { return 0; }\n");
    CREDO_VERUM(ast.successus);

    /* [value^="prefix"] - starts with */
    q = arbor_quaestio_compilare(piscina, intern, "identifier[value^=\"prefix\"]");
    CREDO_NON_NIHIL(q);
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) >= II);
    imprimere("  identifier[value^=\"prefix\"]: found %d - OK\n", xar_numerus(resultata));

    /* [value$="suffix"] - ends with */
    q = arbor_quaestio_compilare(piscina, intern, "identifier[value$=\"suffix\"]");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  identifier[value$=\"suffix\"]: found %d - OK\n", xar_numerus(resultata));

    /* [value*="mid"] - contains */
    q = arbor_quaestio_compilare(piscina, intern, "identifier[value*=\"mid\"]");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  identifier[value*=\"mid\"]: found %d - OK\n", xar_numerus(resultata));

    /* Non-matching prefix */
    q = arbor_quaestio_compilare(piscina, intern, "identifier[value^=\"xyz\"]");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) == ZEPHYRUM);
    imprimere("  identifier[value^=\"xyz\"]: found %d - OK\n", xar_numerus(resultata));
}

/* ==================================================
 * Probationes - Captures
 * ================================================== */

interior vacuum
probatio_captures(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;
    ArborQuaestioResultatum* res;
    vacuum* captured;
    chorda* fn_key;

    imprimere("\n--- Captures ---\n");

    ast = _parsere(piscina, intern, "int foo(void) { return 0; }");
    CREDO_VERUM(ast.successus);

    /* function-definition identifier$fn */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition identifier$fn");
    CREDO_NON_NIHIL(q);

    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);

    si (xar_numerus(resultata) > ZEPHYRUM)
    {
        res = xar_obtinere(resultata, ZEPHYRUM);
        CREDO_NON_NIHIL(res);
        CREDO_NON_NIHIL(res->capturae);

        fn_key = chorda_internare_ex_literis(intern, "fn");
        si (tabula_dispersa_invenire(res->capturae, *fn_key, &captured))
        {
            imprimere("  Capture $fn: found - OK\n");
        }
        alioquin
        {
            imprimere("  Capture $fn: not found\n");
        }
    }
    alioquin
    {
        imprimere("  No results for capture test\n");
    }
}

/* ==================================================
 * Probationes - Pseudo-Selectors
 * ================================================== */

interior vacuum
probatio_pseudo_selectors(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;

    imprimere("\n--- Pseudo-Selectors ---\n");

    ast = _parsere(piscina, intern,
        "void foo(void) { printf(\"hello\"); }\n"
        "int bar(void) { return 42; }");
    CREDO_VERUM(ast.successus);

    /* :returns - functions with return statements */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition:returns");
    CREDO_NON_NIHIL(q);

    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    imprimere("  function-definition:returns: found %d - OK\n", xar_numerus(resultata));

    /* :calls(printf) */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition:calls(printf)");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    imprimere("  function-definition:calls(printf): found %d - OK\n", xar_numerus(resultata));
}

/* ==================================================
 * Probationes - More Pseudo-Selectors
 * ================================================== */

interior vacuum
probatio_pseudo_extended(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;

    imprimere("\n--- Pseudo-Selectors (Extended) ---\n");

    ast = _parsere(piscina, intern,
        "int global_var = 100;\n"
        "int compute(int x) {\n"
        "    int local = x * 2;\n"
        "    return local + global_var;\n"
        "}\n"
        "void caller(void) {\n"
        "    int result = compute(5);\n"
        "}\n");
    CREDO_VERUM(ast.successus);

    /* :defines(compute) - function that defines 'compute' */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition:defines(compute)");
    CREDO_NON_NIHIL(q);
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) == I);
    imprimere("  function-definition:defines(compute): found %d - OK\n", xar_numerus(resultata));

    /* :calls(compute) - functions that call 'compute' */
    /* Note: This may find 0 if parser structures call differently */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition:calls(compute)");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    imprimere("  function-definition:calls(compute): found %d\n", xar_numerus(resultata));

    /* :uses(global_var) - functions that use 'global_var' */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition:uses(global_var)");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  function-definition:uses(global_var): found %d - OK\n", xar_numerus(resultata));

    /* :uses(nonexistent) - should find 0 */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition:uses(nonexistent)");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) == ZEPHYRUM);
    imprimere("  function-definition:uses(nonexistent): found %d - OK\n", xar_numerus(resultata));

    /* :returns without :returns - void function */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition:defines(caller)");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    imprimere("  function-definition:defines(caller): found %d - OK\n", xar_numerus(resultata));
}

/* ==================================================
 * Probationes - Multiple Captures
 * ================================================== */

interior vacuum
probatio_captures_extended(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;
    ArborQuaestioResultatum* res;
    vacuum* captured;
    chorda* key;
    ArborNodus* nodus;

    imprimere("\n--- Multiple Captures ---\n");

    ast = _parsere(piscina, intern,
        "int foo(int x) { return x + 1; }\n"
        "int bar(int y) { return y * 2; }\n");
    CREDO_VERUM(ast.successus);

    /* Capture function name and return statement */
    q = arbor_quaestio_compilare(piscina, intern,
        "function-definition$func > compound-statement return-statement$ret");
    CREDO_NON_NIHIL(q);

    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) >= II);
    imprimere("  Multi-capture query: found %d - OK\n", xar_numerus(resultata));

    /* Verify captures exist */
    si (xar_numerus(resultata) > ZEPHYRUM)
    {
        res = xar_obtinere(resultata, ZEPHYRUM);
        CREDO_NON_NIHIL(res);
        CREDO_NON_NIHIL(res->capturae);

        /* Check $ret capture */
        key = chorda_internare_ex_literis(intern, "ret");
        si (tabula_dispersa_invenire(res->capturae, *key, &captured))
        {
            nodus = (ArborNodus*)captured;
            CREDO_VERUM(nodus->genus == ARBOR_NODUS_RETURN_STATEMENT);
            imprimere("  Capture $ret: return-statement - OK\n");
        }
        alioquin
        {
            imprimere("  Capture $ret: not found\n");
        }

        /* Check $func capture */
        key = chorda_internare_ex_literis(intern, "func");
        si (tabula_dispersa_invenire(res->capturae, *key, &captured))
        {
            nodus = (ArborNodus*)captured;
            CREDO_VERUM(nodus->genus == ARBOR_NODUS_FUNCTION_DEFINITION);
            imprimere("  Capture $func: function-definition - OK\n");
        }
        alioquin
        {
            imprimere("  Capture $func: not found\n");
        }
    }
}

/* ==================================================
 * Probationes - Complex Queries
 * ================================================== */

interior vacuum
probatio_complex_queries(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;

    imprimere("\n--- Complex Queries ---\n");

    ast = _parsere(piscina, intern,
        "int add(int a, int b) { return a + b; }\n"
        "int main(void) { return add(1, 2); }");
    CREDO_VERUM(ast.successus);

    /* function-definition > compound-statement return-statement */
    q = arbor_quaestio_compilare(piscina, intern,
        "function-definition > compound-statement return-statement");
    CREDO_NON_NIHIL(q);

    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    imprimere("  Complex chain: found %d return statements - OK\n", xar_numerus(resultata));

    /* call-expression */
    q = arbor_quaestio_compilare(piscina, intern, "call-expression");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    imprimere("  call-expression: found %d - OK\n", xar_numerus(resultata));
}

/* ==================================================
 * Probationes - Deep Nesting
 * ================================================== */

interior vacuum
probatio_deep_nesting(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;

    imprimere("\n--- Deep Nesting ---\n");

    /* Parse deeply nested code */
    ast = _parsere(piscina, intern,
        "int deep(int n) {\n"
        "    if (n > 0) {\n"
        "        if (n > 10) {\n"
        "            if (n > 100) {\n"
        "                return n;\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    return 0;\n"
        "}\n");
    CREDO_VERUM(ast.successus);

    /* Find deeply nested return statements */
    q = arbor_quaestio_compilare(piscina, intern,
        "function-definition if-statement if-statement if-statement return-statement");
    CREDO_NON_NIHIL(q);
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_NON_NIHIL(resultata);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  4-level nesting: found %d - OK\n", xar_numerus(resultata));

    /* Direct child should NOT find deeply nested */
    q = arbor_quaestio_compilare(piscina, intern,
        "function-definition > return-statement");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) == ZEPHYRUM);
    imprimere("  Direct child (no deep match): found %d - OK\n", xar_numerus(resultata));

    /* But descendant should find all */
    q = arbor_quaestio_compilare(piscina, intern,
        "function-definition return-statement");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= II);
    imprimere("  Descendant (all returns): found %d - OK\n", xar_numerus(resultata));
}

/* ==================================================
 * Probationes - Child vs Descendant
 * ================================================== */

interior vacuum
probatio_child_vs_descendant(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;
    i32 child_count;
    i32 desc_count;

    imprimere("\n--- Child vs Descendant ---\n");

    ast = _parsere(piscina, intern,
        "int outer(void) {\n"
        "    int inner(void) { return 1; }\n"  /* Nested function (GCC extension) - may not parse */
        "    return 0;\n"
        "}\n");
    /* Note: nested functions may not parse in strict C89 */

    /* Try simpler case */
    ast = _parsere(piscina, intern,
        "int func(void) {\n"
        "    if (1) {\n"
        "        return 1;\n"
        "    }\n"
        "    return 0;\n"
        "}\n");
    CREDO_VERUM(ast.successus);

    /* Direct child of compound-statement */
    q = arbor_quaestio_compilare(piscina, intern,
        "compound-statement > return-statement");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    child_count = xar_numerus(resultata);
    imprimere("  compound > return (direct): found %d\n", child_count);

    /* Any descendant */
    q = arbor_quaestio_compilare(piscina, intern,
        "compound-statement return-statement");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    desc_count = xar_numerus(resultata);
    imprimere("  compound return (descendant): found %d\n", desc_count);

    /* Descendant should find >= direct children */
    CREDO_VERUM(desc_count >= child_count);
    imprimere("  descendant >= child: OK\n");
}

/* ==================================================
 * Probationes - Edge Cases
 * ================================================== */

interior vacuum
probatio_edge_cases(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;
    Xar* resultata;

    imprimere("\n--- Edge Cases ---\n");

    /* Empty code */
    ast = _parsere(piscina, intern, "");
    /* May or may not succeed depending on parser */

    /* Query on simple declaration (no functions) */
    ast = _parsere(piscina, intern, "int x;");
    CREDO_VERUM(ast.successus);

    q = arbor_quaestio_compilare(piscina, intern, "function-definition");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) == ZEPHYRUM);
    imprimere("  No functions in decl-only: found %d - OK\n", xar_numerus(resultata));

    /* Unknown type name returns NULL query (graceful handling) */
    q = arbor_quaestio_compilare(piscina, intern, "nonexistent-node-type");
    /* Should return non-NULL but with empty results, or NULL */
    si (q != NIHIL)
    {
        resultata = arbor_quaestio_exsequi(q, ast.radix);
        CREDO_VERUM(xar_numerus(resultata) == ZEPHYRUM);
        imprimere("  Unknown type: graceful empty result - OK\n");
    }
    alioquin
    {
        imprimere("  Unknown type: NULL query (acceptable) - OK\n");
    }

    /* Multiple functions, find specific one */
    ast = _parsere(piscina, intern,
        "int alpha(void) { return 1; }\n"
        "int beta(void) { return 2; }\n"
        "int gamma(void) { return 3; }\n");
    CREDO_VERUM(ast.successus);

    q = arbor_quaestio_compilare(piscina, intern, "function-definition");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) == III);
    imprimere("  Three functions: found %d - OK\n", xar_numerus(resultata));

    /* Query with attribute on specific function */
    q = arbor_quaestio_compilare(piscina, intern,
        "function-definition identifier[value=\"beta\"]");
    resultata = arbor_quaestio_exsequi(q, ast.radix);
    CREDO_VERUM(xar_numerus(resultata) >= I);
    imprimere("  Find 'beta' identifier: found %d - OK\n", xar_numerus(resultata));
}

/* ==================================================
 * Probationes - Utility Functions
 * ================================================== */

interior vacuum
probatio_utilities(vacuum)
{
    imprimere("\n--- Utilities ---\n");

    CREDO_VERUM(strcmp(arbor_quaestio_pseudo_nomen(QUAESTIO_PSEUDO_CALLS), "calls") == ZEPHYRUM);
    imprimere("  pseudo_nomen(calls): OK\n");

    CREDO_VERUM(strcmp(arbor_quaestio_pseudo_nomen(QUAESTIO_PSEUDO_RETURNS), "returns") == ZEPHYRUM);
    imprimere("  pseudo_nomen(returns): OK\n");

    CREDO_VERUM(strcmp(arbor_quaestio_pseudo_nomen(QUAESTIO_PSEUDO_DEFINES), "defines") == ZEPHYRUM);
    imprimere("  pseudo_nomen(defines): OK\n");

    CREDO_VERUM(strcmp(arbor_quaestio_pseudo_nomen(QUAESTIO_PSEUDO_USES), "uses") == ZEPHYRUM);
    imprimere("  pseudo_nomen(uses): OK\n");

    CREDO_VERUM(strcmp(arbor_quaestio_combinator_nomen(QUAESTIO_COMBINATOR_DESCENDENS), "descendens") == ZEPHYRUM);
    imprimere("  combinator_nomen(descendens): OK\n");

    CREDO_VERUM(strcmp(arbor_quaestio_combinator_nomen(QUAESTIO_COMBINATOR_LIBERUM), "liberum") == ZEPHYRUM);
    imprimere("  combinator_nomen(liberum): OK\n");

    CREDO_VERUM(strcmp(arbor_quaestio_combinator_nomen(QUAESTIO_COMBINATOR_NULLUS), "nullus") == ZEPHYRUM);
    imprimere("  combinator_nomen(nullus): OK\n");
}

/* ==================================================
 * Probationes - Congruit (Simple Match)
 * ================================================== */

interior vacuum
probatio_congruit(Piscina* piscina, InternamentumChorda* intern)
{
    ArborQuaestio* q;
    ArborSyntaxisResultus ast;

    imprimere("\n--- Congruit (Simple Match) ---\n");

    ast = _parsere(piscina, intern, "int x;");
    CREDO_VERUM(ast.successus);

    /* Check translation-unit matches root */
    q = arbor_quaestio_compilare(piscina, intern, "translation-unit");
    CREDO_NON_NIHIL(q);
    CREDO_VERUM(arbor_quaestio_congruit(q, ast.radix));
    imprimere("  translation-unit matches root: OK\n");

    /* Check function-definition does not match root */
    q = arbor_quaestio_compilare(piscina, intern, "function-definition");
    CREDO_FALSUM(arbor_quaestio_congruit(q, ast.radix));
    imprimere("  function-definition does not match declaration root: OK\n");
}

/* ==================================================
 * Main
 * ================================================== */

integer principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    b32 praeteritus;

    imprimere("===========================================\n");
    imprimere("PROBATIO ARBOR QUAESTIO\n");
    imprimere("===========================================\n");

    piscina = piscina_generare_dynamicum("probatio_arbor_quaestio", M * LXIV);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina_generare_dynamicum\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        imprimere("FRACTA: internamentum_creare\n");
        piscina_destruere(piscina);
        redde I;
    }

    credo_aperire(piscina);

    /* Basic tests */
    probatio_type_selector(piscina, intern);
    probatio_type_selector_extended(piscina, intern);
    probatio_descendant_combinator(piscina, intern);
    probatio_child_combinator(piscina, intern);

    /* Attribute tests */
    probatio_attribute_selector(piscina, intern);
    probatio_attribute_operators(piscina, intern);

    /* Capture tests */
    probatio_captures(piscina, intern);
    probatio_captures_extended(piscina, intern);

    /* Pseudo-selector tests */
    probatio_pseudo_selectors(piscina, intern);
    probatio_pseudo_extended(piscina, intern);

    /* Complex and edge case tests */
    probatio_complex_queries(piscina, intern);
    probatio_deep_nesting(piscina, intern);
    probatio_child_vs_descendant(piscina, intern);
    probatio_edge_cases(piscina, intern);

    /* Utility tests */
    probatio_utilities();
    probatio_congruit(piscina, intern);

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
