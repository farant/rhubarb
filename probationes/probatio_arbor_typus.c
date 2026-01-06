/* probatio_arbor_typus.c - Probationes pro type resolver */

#include "credo.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor_syntaxis.h"
#include "arbor_typus.h"
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
 * Probationes - Primitive Types
 * ================================================== */

interior vacuum
probatio_typus_int(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    imprimere("\n--- Primitive Types ---\n");

    ast = _parsere(piscina, intern, "int x;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    CREDO_NON_NIHIL(res);

    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Int declaration resolved: OK\n");
}

interior vacuum
probatio_typus_char(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    ast = _parsere(piscina, intern, "char c;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Char declaration resolved: OK\n");
}

interior vacuum
probatio_typus_void(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    ast = _parsere(piscina, intern, "void f(void);");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Void function resolved: OK\n");
}

interior vacuum
probatio_typus_unsigned(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    ast = _parsere(piscina, intern, "unsigned int u;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Unsigned int resolved: OK\n");
}

interior vacuum
probatio_typus_long(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    ast = _parsere(piscina, intern, "long x;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Long resolved: OK\n");
}

/* ==================================================
 * Probationes - Pointer Types
 * ================================================== */

interior vacuum
probatio_typus_pointer(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    imprimere("\n--- Pointer Types ---\n");

    ast = _parsere(piscina, intern, "int *p;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Pointer to int: OK\n");
}

interior vacuum
probatio_typus_pointer_duplex(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    ast = _parsere(piscina, intern, "int **pp;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Double pointer: OK\n");
}

interior vacuum
probatio_typus_const_pointer(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    ast = _parsere(piscina, intern, "const int *p;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Pointer to const int: OK\n");
}

/* ==================================================
 * Probationes - Array Types
 * ================================================== */

interior vacuum
probatio_typus_array(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    imprimere("\n--- Array Types ---\n");

    ast = _parsere(piscina, intern, "int arr[10];");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Array of int: OK\n");
}

interior vacuum
probatio_typus_array_unsized(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    ast = _parsere(piscina, intern, "int arr[];");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Unsized array: OK\n");
}

/* ==================================================
 * Probationes - Function Types
 * ================================================== */

interior vacuum
probatio_typus_functio(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    imprimere("\n--- Function Types ---\n");

    ast = _parsere(piscina, intern, "int f(int x, char c) { return 0; }");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Function definition: OK\n");
}

interior vacuum
probatio_typus_functio_pointer(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    ast = _parsere(piscina, intern, "int (*fp)(int);");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Function pointer: OK\n");
}

/* ==================================================
 * Probationes - Struct Types
 * ================================================== */

interior vacuum
probatio_typus_struct(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    imprimere("\n--- Struct Types ---\n");

    ast = _parsere(piscina, intern, "struct Point { int x; int y; };");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Struct definition: OK\n");
}

interior vacuum
probatio_typus_struct_variabilis(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    ast = _parsere(piscina, intern, "struct Point { int x; int y; } p;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Struct with variable: OK\n");
}

/* ==================================================
 * Probationes - Typedef
 * ================================================== */

interior vacuum
probatio_typus_typedef(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    imprimere("\n--- Typedef ---\n");

    ast = _parsere(piscina, intern, "typedef int Int32;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Typedef simplex: OK\n");
}

interior vacuum
probatio_typus_typedef_pointer(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    ast = _parsere(piscina, intern, "typedef int *IntPtr;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Typedef pointer: OK\n");
}

/* ==================================================
 * Probationes - Type Utilities
 * ================================================== */

interior vacuum
probatio_typus_utilities(vacuum)
{
    constans character* genus_str;

    imprimere("\n--- Type Utilities ---\n");

    /* Test genus nomen */
    genus_str = arbor_typus_genus_nomen(ARBOR_TYPUS_INT);
    CREDO_VERUM(strcmp(genus_str, "int") == ZEPHYRUM);
    imprimere("  Genus nomen int: %s - OK\n", genus_str);

    genus_str = arbor_typus_genus_nomen(ARBOR_TYPUS_POINTER);
    CREDO_VERUM(strcmp(genus_str, "pointer") == ZEPHYRUM);
    imprimere("  Genus nomen pointer: %s - OK\n", genus_str);

    genus_str = arbor_typus_genus_nomen(ARBOR_TYPUS_STRUCT);
    CREDO_VERUM(strcmp(genus_str, "struct") == ZEPHYRUM);
    imprimere("  Genus nomen struct: %s - OK\n", genus_str);
}

interior vacuum
probatio_typus_est_integer(vacuum)
{
    constans character* genus_str;

    /* Verify type classification functions exist */
    genus_str = arbor_typus_genus_nomen(ARBOR_TYPUS_CHAR);
    CREDO_VERUM(strcmp(genus_str, "char") == ZEPHYRUM);
    imprimere("  Est integer check: OK\n");
}

/* ==================================================
 * Probationes - Scope
 * ================================================== */

interior vacuum
probatio_typus_scope(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;

    imprimere("\n--- Scope ---\n");

    /* Function with local scope */
    ast = _parsere(piscina, intern,
        "int f(void) { int x; { int y; } return x; }");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);
    imprimere("  Nested scopes: OK\n");
}

/* ==================================================
 * Main
 * ================================================== */

integer principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    imprimere("===========================================\n");
    imprimere("PROBATIO ARBOR TYPUS\n");
    imprimere("===========================================\n");

    piscina = piscina_generare_dynamicum("probatio_arbor_typus", M * LXIV);
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

    /* Primitive types */
    probatio_typus_int(piscina, intern);
    probatio_typus_char(piscina, intern);
    probatio_typus_void(piscina, intern);
    probatio_typus_unsigned(piscina, intern);
    probatio_typus_long(piscina, intern);

    /* Pointer types */
    probatio_typus_pointer(piscina, intern);
    probatio_typus_pointer_duplex(piscina, intern);
    probatio_typus_const_pointer(piscina, intern);

    /* Array types */
    probatio_typus_array(piscina, intern);
    probatio_typus_array_unsized(piscina, intern);

    /* Function types */
    probatio_typus_functio(piscina, intern);
    probatio_typus_functio_pointer(piscina, intern);

    /* Struct types */
    probatio_typus_struct(piscina, intern);
    probatio_typus_struct_variabilis(piscina, intern);

    /* Typedef */
    probatio_typus_typedef(piscina, intern);
    probatio_typus_typedef_pointer(piscina, intern);

    /* Utilities */
    probatio_typus_utilities();
    probatio_typus_est_integer();

    /* Scope */
    probatio_typus_scope(piscina, intern);

    imprimere("\n===========================================\n");
    imprimere("OMNIA PROBATIONES SUCCESSAE!\n");
    imprimere("===========================================\n");

    credo_claudere();
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
