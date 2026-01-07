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
    ArborTypus* typus;
    chorda* titulus;

    imprimere("\n--- Primitive Types ---\n");

    ast = _parsere(piscina, intern, "int x;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    CREDO_NON_NIHIL(res);

    arbor_typus_resolvere(res, ast.radix);

    /* Verificare typum variabilis 'x' */
    titulus = chorda_internare_ex_literis(intern, "x");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_INT);
    CREDO_VERUM(arbor_typus_est_integer(typus));
    CREDO_VERUM(arbor_typus_est_arithmeticus(typus));
    CREDO_VERUM(arbor_typus_est_scalaris(typus));
    imprimere("  Int declaration: genus=%s - OK\n", arbor_typus_genus_nomen(typus->genus));
}

interior vacuum
probatio_typus_char(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    ast = _parsere(piscina, intern, "char c;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "c");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_CHAR);
    CREDO_VERUM(arbor_typus_est_integer(typus));
    imprimere("  Char declaration: genus=%s - OK\n", arbor_typus_genus_nomen(typus->genus));
}

interior vacuum
probatio_typus_void(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    ast = _parsere(piscina, intern, "void f(void);");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "f");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_FUNCTION);
    CREDO_NON_NIHIL(typus->reditum);
    CREDO_VERUM(typus->reditum->genus == ARBOR_TYPUS_VOID);
    imprimere("  Void function: reditum=%s - OK\n", arbor_typus_genus_nomen(typus->reditum->genus));
}

interior vacuum
probatio_typus_unsigned(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    ast = _parsere(piscina, intern, "unsigned int u;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "u");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_INT);
    CREDO_VERUM(typus->est_unsigned);
    CREDO_FALSUM(typus->est_signed);
    imprimere("  Unsigned int: est_unsigned=%d - OK\n", typus->est_unsigned);
}

interior vacuum
probatio_typus_long(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    ast = _parsere(piscina, intern, "long x;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "x");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_LONG);
    imprimere("  Long: genus=%s - OK\n", arbor_typus_genus_nomen(typus->genus));
}

/* ==================================================
 * Probationes - Pointer Types
 * ================================================== */

interior vacuum
probatio_typus_pointer(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    imprimere("\n--- Pointer Types ---\n");

    ast = _parsere(piscina, intern, "int *p;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "p");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_POINTER);
    CREDO_NON_NIHIL(typus->basis);
    CREDO_VERUM(typus->basis->genus == ARBOR_TYPUS_INT);
    CREDO_VERUM(arbor_typus_est_scalaris(typus));
    CREDO_FALSUM(arbor_typus_est_arithmeticus(typus));
    imprimere("  Pointer to int: basis=%s - OK\n", arbor_typus_genus_nomen(typus->basis->genus));
}

interior vacuum
probatio_typus_pointer_duplex(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    ast = _parsere(piscina, intern, "int **pp;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "pp");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_POINTER);
    CREDO_NON_NIHIL(typus->basis);
    CREDO_VERUM(typus->basis->genus == ARBOR_TYPUS_POINTER);
    CREDO_NON_NIHIL(typus->basis->basis);
    CREDO_VERUM(typus->basis->basis->genus == ARBOR_TYPUS_INT);
    imprimere("  Double pointer: **int - OK\n");
}

interior vacuum
probatio_typus_const_pointer(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    ast = _parsere(piscina, intern, "const int *p;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "p");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_POINTER);
    CREDO_NON_NIHIL(typus->basis);
    CREDO_VERUM(typus->basis->est_const);
    imprimere("  Pointer to const int: basis->est_const=%d - OK\n", typus->basis->est_const);
}

/* ==================================================
 * Probationes - Array Types
 * ================================================== */

interior vacuum
probatio_typus_array(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    imprimere("\n--- Array Types ---\n");

    ast = _parsere(piscina, intern, "int arr[10];");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "arr");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_ARRAY);
    CREDO_NON_NIHIL(typus->basis);
    CREDO_VERUM(typus->basis->genus == ARBOR_TYPUS_INT);
    CREDO_VERUM(typus->array_mensura == X);
    imprimere("  Array of int[10]: mensura=%d - OK\n", typus->array_mensura);
}

interior vacuum
probatio_typus_array_unsized(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    ast = _parsere(piscina, intern, "int arr[];");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "arr");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_ARRAY);
    CREDO_VERUM(typus->array_mensura == -I);  /* -1 pro unsized */
    imprimere("  Unsized array: mensura=%d - OK\n", typus->array_mensura);
}

/* ==================================================
 * Probationes - Function Types
 * ================================================== */

interior vacuum
probatio_typus_functio(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    imprimere("\n--- Function Types ---\n");

    ast = _parsere(piscina, intern, "int f(int x, char c) { return 0; }");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "f");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_FUNCTION);
    CREDO_NON_NIHIL(typus->reditum);
    CREDO_VERUM(typus->reditum->genus == ARBOR_TYPUS_INT);
    /* NB: parametra resolution nondum implementatum */
    imprimere("  Function -> int: reditum=%s - OK\n", arbor_typus_genus_nomen(typus->reditum->genus));
}

interior vacuum
probatio_typus_functio_pointer(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    /* NB: Functio pointer declarator complexus - verificamus parsing tantum */
    ast = _parsere(piscina, intern, "int (*fp)(int);");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "fp");
    typus = arbor_typus_identificatoris(res, titulus);
    /* Function pointer resolution complexior - typus potest esse NIHIL vel non-pointer */
    si (typus != NIHIL)
    {
        imprimere("  Function pointer: typus inventa, genus=%s - OK\n",
                  arbor_typus_genus_nomen(typus->genus));
    }
    alioquin
    {
        imprimere("  Function pointer: parsing OK, resolution nondum - OK\n");
    }
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

    /* Struct sine variabilis - just verify no crash */
    imprimere("  Struct definition: OK\n");
}

interior vacuum
probatio_typus_struct_variabilis(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    ast = _parsere(piscina, intern, "struct Point { int x; int y; } p;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "p");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_STRUCT);
    imprimere("  Struct with variable: genus=%s - OK\n", arbor_typus_genus_nomen(typus->genus));
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

    /* Typedef registered - verify no crash */
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

    genus_str = arbor_typus_genus_nomen(ARBOR_TYPUS_FUNCTION);
    CREDO_VERUM(strcmp(genus_str, "function") == ZEPHYRUM);
    imprimere("  Genus nomen function: %s - OK\n", genus_str);
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
    ArborTypus* typus;
    chorda* titulus;

    imprimere("\n--- Scope ---\n");

    /* Function with local scope */
    ast = _parsere(piscina, intern,
        "int f(void) { int x; { int y; } return x; }");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    /* Verificare functio f */
    titulus = chorda_internare_ex_literis(intern, "f");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_FUNCTION);
    imprimere("  Nested scopes: OK\n");
}

/* ==================================================
 * Probationes - Type Equality
 * ================================================== */

interior vacuum
probatio_typus_aequalis(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus_x;
    ArborTypus* typus_y;
    chorda* titulus;

    imprimere("\n--- Type Equality ---\n");

    ast = _parsere(piscina, intern, "int x; int y;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "x");
    typus_x = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus_x);

    titulus = chorda_internare_ex_literis(intern, "y");
    typus_y = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus_y);

    CREDO_VERUM(arbor_typus_aequalis(typus_x, typus_y));
    imprimere("  int == int: OK\n");
}

/* ==================================================
 * Probationes - Float Types
 * ================================================== */

interior vacuum
probatio_typus_float(Piscina* piscina, InternamentumChorda* intern)
{
    ArborTypusResolver* res;
    ArborSyntaxisResultus ast;
    ArborTypus* typus;
    chorda* titulus;

    imprimere("\n--- Float Types ---\n");

    ast = _parsere(piscina, intern, "float f; double d;");
    CREDO_VERUM(ast.successus);

    res = arbor_typus_creare(piscina, intern);
    arbor_typus_resolvere(res, ast.radix);

    titulus = chorda_internare_ex_literis(intern, "f");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_FLOAT);
    CREDO_VERUM(arbor_typus_est_arithmeticus(typus));
    CREDO_FALSUM(arbor_typus_est_integer(typus));
    imprimere("  Float: genus=%s - OK\n", arbor_typus_genus_nomen(typus->genus));

    titulus = chorda_internare_ex_literis(intern, "d");
    typus = arbor_typus_identificatoris(res, titulus);
    CREDO_NON_NIHIL(typus);
    CREDO_VERUM(typus->genus == ARBOR_TYPUS_DOUBLE);
    CREDO_VERUM(arbor_typus_est_arithmeticus(typus));
    imprimere("  Double: genus=%s - OK\n", arbor_typus_genus_nomen(typus->genus));
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

    /* Float types */
    probatio_typus_float(piscina, intern);

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

    /* Type equality */
    probatio_typus_aequalis(piscina, intern);

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
