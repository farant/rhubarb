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

    CREDO_VERUM(strcmp(arbor_quaestio_combinator_nomen(QUAESTIO_COMBINATOR_DESCENDENS), "descendens") == ZEPHYRUM);
    imprimere("  combinator_nomen(descendens): OK\n");

    CREDO_VERUM(strcmp(arbor_quaestio_combinator_nomen(QUAESTIO_COMBINATOR_LIBERUM), "liberum") == ZEPHYRUM);
    imprimere("  combinator_nomen(liberum): OK\n");
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

    probatio_type_selector(piscina, intern);
    probatio_descendant_combinator(piscina, intern);
    probatio_child_combinator(piscina, intern);
    probatio_attribute_selector(piscina, intern);
    probatio_captures(piscina, intern);
    probatio_pseudo_selectors(piscina, intern);
    probatio_complex_queries(piscina, intern);
    probatio_utilities();
    probatio_congruit(piscina, intern);

    imprimere("\n===========================================\n");
    imprimere("OMNIA PROBATIONES SUCCESSAE!\n");
    imprimere("===========================================\n");

    credo_claudere();
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
