/* ==================================================
 * PROBATIO ARBOR SYNTAXIS - Unit Tests pro C89 Parser
 * ================================================== */

#include "credo.h"
#include "arbor_syntaxis.h"
#include "arbor_praeparator.h"
#include "arbor_lexema.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Adiutor: Parsere fontem et obtinere AST
 * ================================================== */

interior ArborSyntaxisResultus
_parsere_fontem(Piscina* piscina, InternamentumChorda* intern,
                constans character* fons)
{
    ArborSyntaxis* syn;
    syn = arbor_syntaxis_creare(piscina, intern);
    redde arbor_syntaxis_parsere_fontem(syn, fons, (i32)strlen(fons), "test.c");
}

/* ==================================================
 * Test: Creatio Parser
 * ================================================== */

interior vacuum
test_creare(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxis* syn;

    imprimere("\n--- Creatio Parser ---\n");

    syn = arbor_syntaxis_creare(piscina, intern);
    CREDO_NON_NIHIL(syn);

    imprimere("  Creare: OK\n");
}

/* Test functions block 1 */
#if 1  /* Enable full tests */

/* ==================================================
 * Test: Integer Literals
 * ================================================== */

interior vacuum
test_integer_literal(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* init;

    imprimere("\n--- Integer Literal ---\n");

    /* Simplex integer */
    res = _parsere_fontem(piscina, intern, "int x = 42;");
    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.radix);

    radix = res.radix;
    CREDO_AEQUALIS_I32(radix->genus, ARBOR_NODUS_TRANSLATION_UNIT);
    CREDO_NON_NIHIL(radix->datum.genericum.liberi);
    CREDO_AEQUALIS_I32(xar_numerus(radix->datum.genericum.liberi), I);

    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    CREDO_AEQUALIS_I32(decl->genus, ARBOR_NODUS_DECLARATION);

    /* Obtinere init-declarator */
    CREDO_NON_NIHIL(decl->datum.declaratio.declaratores);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    CREDO_AEQUALIS_I32(init->genus, ARBOR_NODUS_INIT_DECLARATOR);
    CREDO_NON_NIHIL(init->datum.init_decl.initializer);
    CREDO_AEQUALIS_I32(init->datum.init_decl.initializer->genus, ARBOR_NODUS_INTEGER_LITERAL);

    imprimere("  Integer literal: OK\n");

    /* Hexadecimalis */
    res = _parsere_fontem(piscina, intern, "int y = 0xFF;");
    CREDO_VERUM(res.successus);

    imprimere("  Hexadecimal: OK\n");

    /* Octalis */
    res = _parsere_fontem(piscina, intern, "int z = 077;");
    CREDO_VERUM(res.successus);

    imprimere("  Octal: OK\n");
}

/* ==================================================
 * Test: Float Literals
 * ================================================== */

interior vacuum
test_float_literal(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;

    imprimere("\n--- Float Literal ---\n");

    res = _parsere_fontem(piscina, intern, "double x = 3.14;");
    CREDO_VERUM(res.successus);
    imprimere("  Float simplex: OK\n");

    res = _parsere_fontem(piscina, intern, "double y = 1.0e10;");
    CREDO_VERUM(res.successus);
    imprimere("  Float exponent: OK\n");

    res = _parsere_fontem(piscina, intern, "float z = 2.5f;");
    CREDO_VERUM(res.successus);
    imprimere("  Float suffix: OK\n");
}

/* ==================================================
 * Test: Character Literals
 * ================================================== */

interior vacuum
test_char_literal(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;

    imprimere("\n--- Character Literal ---\n");

    res = _parsere_fontem(piscina, intern, "char c = 'a';");
    CREDO_VERUM(res.successus);
    imprimere("  Char simplex: OK\n");

    res = _parsere_fontem(piscina, intern, "char n = '\\n';");
    CREDO_VERUM(res.successus);
    imprimere("  Char escape: OK\n");
}

/* ==================================================
 * Test: String Literals
 * ================================================== */

interior vacuum
test_string_literal(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;

    imprimere("\n--- String Literal ---\n");

    res = _parsere_fontem(piscina, intern, "char* s = \"hello\";");
    CREDO_VERUM(res.successus);
    imprimere("  String simplex: OK\n");

    res = _parsere_fontem(piscina, intern, "char* t = \"line\\n\";");
    CREDO_VERUM(res.successus);
    imprimere("  String escape: OK\n");
}

/* ==================================================
 * Test: Identifiers
 * ================================================== */

interior vacuum
test_identifier(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* init;

    imprimere("\n--- Identifier ---\n");

    res = _parsere_fontem(piscina, intern, "int a = b;");
    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.radix);

    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    CREDO_NON_NIHIL(init->datum.init_decl.initializer);
    CREDO_AEQUALIS_I32(init->datum.init_decl.initializer->genus, ARBOR_NODUS_IDENTIFIER);

    imprimere("  Identifier: OK\n");
}

/* ==================================================
 * Test: Unary Expressions
 * ================================================== */

interior vacuum
test_unary(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;

    imprimere("\n--- Unary Expressions ---\n");

    /* Negatio */
    res = _parsere_fontem(piscina, intern, "int x = -5;");
    CREDO_VERUM(res.successus);
    imprimere("  Negatio: OK\n");

    /* Logicus non */
    res = _parsere_fontem(piscina, intern, "int y = !flag;");
    CREDO_VERUM(res.successus);
    imprimere("  Logicus non: OK\n");

    /* Bitwise non */
    res = _parsere_fontem(piscina, intern, "int z = ~bits;");
    CREDO_VERUM(res.successus);
    imprimere("  Bitwise non: OK\n");

    /* Address-of */
    res = _parsere_fontem(piscina, intern, "int* p = &var;");
    CREDO_VERUM(res.successus);
    imprimere("  Address-of: OK\n");

    /* Dereference */
    res = _parsere_fontem(piscina, intern, "int v = *ptr;");
    CREDO_VERUM(res.successus);
    imprimere("  Dereference: OK\n");

    /* Prefix increment */
    res = _parsere_fontem(piscina, intern, "int a = ++i;");
    CREDO_VERUM(res.successus);
    imprimere("  Prefix increment: OK\n");

    /* Prefix decrement */
    res = _parsere_fontem(piscina, intern, "int b = --j;");
    CREDO_VERUM(res.successus);
    imprimere("  Prefix decrement: OK\n");
}

/* ==================================================
 * Test: Sizeof
 * ================================================== */

interior vacuum
test_sizeof(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* init;
    ArborNodus* sizeof_node;

    imprimere("\n--- Sizeof ---\n");

    /* sizeof expression */
    res = _parsere_fontem(piscina, intern, "int s = sizeof x;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    sizeof_node = init->datum.init_decl.initializer;
    CREDO_AEQUALIS_I32(sizeof_node->genus, ARBOR_NODUS_SIZEOF_EXPRESSION);
    imprimere("  Sizeof expr: OK\n");

    /* sizeof(type) */
    res = _parsere_fontem(piscina, intern, "int t = sizeof(int);");
    CREDO_VERUM(res.successus);
    imprimere("  Sizeof type: OK\n");
}

/* ==================================================
 * Test: Binary Expressions
 * ================================================== */

interior vacuum
test_binary(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* init;
    ArborNodus* expr;

    imprimere("\n--- Binary Expressions ---\n");

    /* Additio */
    res = _parsere_fontem(piscina, intern, "int a = 1 + 2;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    expr = init->datum.init_decl.initializer;
    CREDO_AEQUALIS_I32(expr->genus, ARBOR_NODUS_BINARY_EXPRESSION);
    CREDO_AEQUALIS_I32(expr->datum.binarium.operator, ARBOR_LEXEMA_PLUS);
    imprimere("  Additio: OK\n");

    /* Subtractio */
    res = _parsere_fontem(piscina, intern, "int b = 5 - 3;");
    CREDO_VERUM(res.successus);
    imprimere("  Subtractio: OK\n");

    /* Multiplicatio */
    res = _parsere_fontem(piscina, intern, "int c = 2 * 3;");
    CREDO_VERUM(res.successus);
    imprimere("  Multiplicatio: OK\n");

    /* Divisio */
    res = _parsere_fontem(piscina, intern, "int d = 10 / 2;");
    CREDO_VERUM(res.successus);
    imprimere("  Divisio: OK\n");

    /* Modulus */
    res = _parsere_fontem(piscina, intern, "int e = 10 % 3;");
    CREDO_VERUM(res.successus);
    imprimere("  Modulus: OK\n");

    /* Comparationes */
    res = _parsere_fontem(piscina, intern, "int f = a < b;");
    CREDO_VERUM(res.successus);
    imprimere("  Minor quam: OK\n");

    res = _parsere_fontem(piscina, intern, "int g = a > b;");
    CREDO_VERUM(res.successus);
    imprimere("  Maior quam: OK\n");

    res = _parsere_fontem(piscina, intern, "int h = a == b;");
    CREDO_VERUM(res.successus);
    imprimere("  Aequalis: OK\n");

    res = _parsere_fontem(piscina, intern, "int i = a != b;");
    CREDO_VERUM(res.successus);
    imprimere("  Non aequalis: OK\n");

    /* Logici */
    res = _parsere_fontem(piscina, intern, "int j = a && b;");
    CREDO_VERUM(res.successus);
    imprimere("  Logicus et: OK\n");

    res = _parsere_fontem(piscina, intern, "int k = a || b;");
    CREDO_VERUM(res.successus);
    imprimere("  Logicus vel: OK\n");

    /* Bitwise */
    res = _parsere_fontem(piscina, intern, "int l = a & b;");
    CREDO_VERUM(res.successus);
    imprimere("  Bitwise et: OK\n");

    res = _parsere_fontem(piscina, intern, "int m = a | b;");
    CREDO_VERUM(res.successus);
    imprimere("  Bitwise vel: OK\n");

    res = _parsere_fontem(piscina, intern, "int n = a ^ b;");
    CREDO_VERUM(res.successus);
    imprimere("  Bitwise xor: OK\n");

    /* Shift */
    res = _parsere_fontem(piscina, intern, "int o = a << 2;");
    CREDO_VERUM(res.successus);
    imprimere("  Shift sinister: OK\n");

    res = _parsere_fontem(piscina, intern, "int p = a >> 2;");
    CREDO_VERUM(res.successus);
    imprimere("  Shift dexter: OK\n");
}

/* ==================================================
 * Test: Operator Precedence
 * ================================================== */

interior vacuum
test_praecedentia(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* init;
    ArborNodus* expr;

    imprimere("\n--- Praecedentia ---\n");

    /* a + b * c => a + (b * c) */
    res = _parsere_fontem(piscina, intern, "int x = a + b * c;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    expr = init->datum.init_decl.initializer;

    /* Radix debet esse + */
    CREDO_AEQUALIS_I32(expr->genus, ARBOR_NODUS_BINARY_EXPRESSION);
    CREDO_AEQUALIS_I32(expr->datum.binarium.operator, ARBOR_LEXEMA_PLUS);

    /* Dexter filius debet esse * */
    CREDO_AEQUALIS_I32(expr->datum.binarium.dexter->genus, ARBOR_NODUS_BINARY_EXPRESSION);
    CREDO_AEQUALIS_I32(expr->datum.binarium.dexter->datum.binarium.operator, ARBOR_LEXEMA_ASTERISCUS);

    imprimere("  a + b * c: OK\n");

    /* a * b + c => (a * b) + c */
    res = _parsere_fontem(piscina, intern, "int y = a * b + c;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    expr = init->datum.init_decl.initializer;

    CREDO_AEQUALIS_I32(expr->datum.binarium.operator, ARBOR_LEXEMA_PLUS);
    CREDO_AEQUALIS_I32(expr->datum.binarium.sinister->genus, ARBOR_NODUS_BINARY_EXPRESSION);
    CREDO_AEQUALIS_I32(expr->datum.binarium.sinister->datum.binarium.operator, ARBOR_LEXEMA_ASTERISCUS);

    imprimere("  a * b + c: OK\n");

    /* a && b || c => (a && b) || c */
    res = _parsere_fontem(piscina, intern, "int z = a && b || c;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    expr = init->datum.init_decl.initializer;

    CREDO_AEQUALIS_I32(expr->datum.binarium.operator, ARBOR_LEXEMA_DUPIPA);
    CREDO_AEQUALIS_I32(expr->datum.binarium.sinister->datum.binarium.operator, ARBOR_LEXEMA_DUAMPERSAND);

    imprimere("  a && b || c: OK\n");
}

/* ==================================================
 * Test: Postfix Expressions
 * ================================================== */

interior vacuum
test_postfix(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* init;
    ArborNodus* expr;

    imprimere("\n--- Postfix ---\n");

    /* Vocatio functionis */
    res = _parsere_fontem(piscina, intern, "int a = foo();");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    expr = init->datum.init_decl.initializer;
    CREDO_AEQUALIS_I32(expr->genus, ARBOR_NODUS_CALL_EXPRESSION);
    imprimere("  Vocatio vacua: OK\n");

    /* Vocatio cum argumentis */
    res = _parsere_fontem(piscina, intern, "int b = bar(1, 2, 3);");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    expr = init->datum.init_decl.initializer;
    CREDO_AEQUALIS_I32(expr->genus, ARBOR_NODUS_CALL_EXPRESSION);
    CREDO_AEQUALIS_I32(xar_numerus(expr->datum.vocatio.argumenta), III);
    imprimere("  Vocatio cum args: OK\n");

    /* Subscriptum */
    res = _parsere_fontem(piscina, intern, "int c = arr[5];");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    expr = init->datum.init_decl.initializer;
    CREDO_AEQUALIS_I32(expr->genus, ARBOR_NODUS_SUBSCRIPT_EXPRESSION);
    imprimere("  Subscriptum: OK\n");

    /* Membrum (.) */
    res = _parsere_fontem(piscina, intern, "int d = obj.field;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    expr = init->datum.init_decl.initializer;
    CREDO_AEQUALIS_I32(expr->genus, ARBOR_NODUS_MEMBER_EXPRESSION);
    CREDO_AEQUALIS_I32(expr->datum.membrum.operator, ARBOR_LEXEMA_PUNCTUM);
    imprimere("  Membrum dot: OK\n");

    /* Membrum (->) */
    res = _parsere_fontem(piscina, intern, "int e = ptr->field;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    expr = init->datum.init_decl.initializer;
    CREDO_AEQUALIS_I32(expr->genus, ARBOR_NODUS_MEMBER_EXPRESSION);
    CREDO_AEQUALIS_I32(expr->datum.membrum.operator, ARBOR_LEXEMA_SAGITTA);
    imprimere("  Membrum arrow: OK\n");

    /* Postfix increment */
    res = _parsere_fontem(piscina, intern, "int f = i++;");
    CREDO_VERUM(res.successus);
    imprimere("  Postfix increment: OK\n");

    /* Postfix decrement */
    res = _parsere_fontem(piscina, intern, "int g = j--;");
    CREDO_VERUM(res.successus);
    imprimere("  Postfix decrement: OK\n");
}

/* ==================================================
 * Test: Ternary Expression
 * ================================================== */

interior vacuum
test_ternary(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* init;
    ArborNodus* expr;

    imprimere("\n--- Ternary ---\n");

    res = _parsere_fontem(piscina, intern, "int x = a ? b : c;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    expr = init->datum.init_decl.initializer;
    CREDO_AEQUALIS_I32(expr->genus, ARBOR_NODUS_CONDITIONAL_EXPRESSION);
    CREDO_NON_NIHIL(expr->datum.ternarium.conditio);
    CREDO_NON_NIHIL(expr->datum.ternarium.verum);
    CREDO_NON_NIHIL(expr->datum.ternarium.falsum);
    imprimere("  Ternary simplex: OK\n");

    /* Nested ternary (right associative) */
    res = _parsere_fontem(piscina, intern, "int y = a ? b : c ? d : e;");
    CREDO_VERUM(res.successus);
    imprimere("  Ternary nested: OK\n");
}

/* ==================================================
 * Test: Assignment Expression
 * ================================================== */

interior vacuum
test_assignment(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;

    imprimere("\n--- Assignment ---\n");

    res = _parsere_fontem(piscina, intern, "void f(void) { x = 5; }");
    CREDO_VERUM(res.successus);
    imprimere("  Simplex: OK\n");

    res = _parsere_fontem(piscina, intern, "void f(void) { x += 5; }");
    CREDO_VERUM(res.successus);
    imprimere("  Plus-assign: OK\n");

    res = _parsere_fontem(piscina, intern, "void f(void) { x -= 5; }");
    CREDO_VERUM(res.successus);
    imprimere("  Minus-assign: OK\n");

    res = _parsere_fontem(piscina, intern, "void f(void) { x *= 5; }");
    CREDO_VERUM(res.successus);
    imprimere("  Mult-assign: OK\n");

    res = _parsere_fontem(piscina, intern, "void f(void) { x /= 5; }");
    CREDO_VERUM(res.successus);
    imprimere("  Div-assign: OK\n");
}

/* ==================================================
 * Test: If Statement
 * ================================================== */

interior vacuum
test_if_statement(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* func;
    ArborNodus* body;
    ArborNodus* stmt;

    imprimere("\n--- If Statement ---\n");

    /* If simplex */
    res = _parsere_fontem(piscina, intern, "void f(void) { if (x) y = 1; }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    func = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    CREDO_AEQUALIS_I32(func->genus, ARBOR_NODUS_FUNCTION_DEFINITION);
    body = func->datum.functio.corpus;
    CREDO_AEQUALIS_I32(body->genus, ARBOR_NODUS_COMPOUND_STATEMENT);
    stmt = *(ArborNodus**)xar_obtinere(body->datum.compositum.sententiae, ZEPHYRUM);
    CREDO_AEQUALIS_I32(stmt->genus, ARBOR_NODUS_IF_STATEMENT);
    CREDO_NON_NIHIL(stmt->datum.conditionale.conditio);
    CREDO_NON_NIHIL(stmt->datum.conditionale.consequens);
    CREDO_AEQUALIS_PTR(stmt->datum.conditionale.alternans, NIHIL);
    imprimere("  If simplex: OK\n");

    /* If-else */
    res = _parsere_fontem(piscina, intern, "void f(void) { if (x) y = 1; else y = 2; }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    func = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    body = func->datum.functio.corpus;
    stmt = *(ArborNodus**)xar_obtinere(body->datum.compositum.sententiae, ZEPHYRUM);
    CREDO_NON_NIHIL(stmt->datum.conditionale.alternans);
    imprimere("  If-else: OK\n");

    /* If-else-if chain */
    res = _parsere_fontem(piscina, intern, "void f(void) { if (a) x=1; else if (b) x=2; else x=3; }");
    CREDO_VERUM(res.successus);
    imprimere("  If-else-if: OK\n");
}

/* ==================================================
 * Test: While Statement
 * ================================================== */

interior vacuum
test_while_statement(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* func;
    ArborNodus* body;
    ArborNodus* stmt;

    imprimere("\n--- While Statement ---\n");

    res = _parsere_fontem(piscina, intern, "void f(void) { while (x) x--; }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    func = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    body = func->datum.functio.corpus;
    stmt = *(ArborNodus**)xar_obtinere(body->datum.compositum.sententiae, ZEPHYRUM);
    CREDO_AEQUALIS_I32(stmt->genus, ARBOR_NODUS_WHILE_STATEMENT);
    CREDO_NON_NIHIL(stmt->datum.iteratio.conditio);
    CREDO_NON_NIHIL(stmt->datum.iteratio.corpus);
    imprimere("  While: OK\n");

    /* While cum block */
    res = _parsere_fontem(piscina, intern, "void f(void) { while (x) { x--; y++; } }");
    CREDO_VERUM(res.successus);
    imprimere("  While block: OK\n");
}

/* ==================================================
 * Test: Do-While Statement
 * ================================================== */

interior vacuum
test_do_statement(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* func;
    ArborNodus* body;
    ArborNodus* stmt;

    imprimere("\n--- Do-While Statement ---\n");

    res = _parsere_fontem(piscina, intern, "void f(void) { do x++; while (x < 10); }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    func = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    body = func->datum.functio.corpus;
    stmt = *(ArborNodus**)xar_obtinere(body->datum.compositum.sententiae, ZEPHYRUM);
    CREDO_AEQUALIS_I32(stmt->genus, ARBOR_NODUS_DO_STATEMENT);
    imprimere("  Do-while: OK\n");

    /* Do-while cum block */
    res = _parsere_fontem(piscina, intern, "void f(void) { do { x++; } while (x < 10); }");
    CREDO_VERUM(res.successus);
    imprimere("  Do-while block: OK\n");
}

/* ==================================================
 * Test: For Statement
 * ================================================== */

interior vacuum
test_for_statement(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* func;
    ArborNodus* body;
    ArborNodus* stmt;

    imprimere("\n--- For Statement ---\n");

    /* For complet */
    res = _parsere_fontem(piscina, intern, "void f(void) { for (i=0; i<10; i++) x++; }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    func = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    body = func->datum.functio.corpus;
    stmt = *(ArborNodus**)xar_obtinere(body->datum.compositum.sententiae, ZEPHYRUM);
    CREDO_AEQUALIS_I32(stmt->genus, ARBOR_NODUS_FOR_STATEMENT);
    CREDO_NON_NIHIL(stmt->datum.circuitus.init);
    CREDO_NON_NIHIL(stmt->datum.circuitus.conditio);
    CREDO_NON_NIHIL(stmt->datum.circuitus.post);
    CREDO_NON_NIHIL(stmt->datum.circuitus.corpus);
    imprimere("  For complet: OK\n");

    /* For sine init */
    res = _parsere_fontem(piscina, intern, "void f(void) { for (; i<10; i++) x++; }");
    CREDO_VERUM(res.successus);
    imprimere("  For sine init: OK\n");

    /* For infinitus */
    res = _parsere_fontem(piscina, intern, "void f(void) { for (;;) break; }");
    CREDO_VERUM(res.successus);
    imprimere("  For infinitus: OK\n");
}

/* ==================================================
 * Test: Switch Statement
 * ================================================== */

interior vacuum
test_switch_statement(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* func;
    ArborNodus* body;
    ArborNodus* stmt;

    imprimere("\n--- Switch Statement ---\n");

    res = _parsere_fontem(piscina, intern,
        "void f(void) { switch (x) { case 1: y=1; break; case 2: y=2; break; default: y=0; } }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    func = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    body = func->datum.functio.corpus;
    stmt = *(ArborNodus**)xar_obtinere(body->datum.compositum.sententiae, ZEPHYRUM);
    CREDO_AEQUALIS_I32(stmt->genus, ARBOR_NODUS_SWITCH_STATEMENT);
    CREDO_NON_NIHIL(stmt->datum.selectio.conditio);
    CREDO_NON_NIHIL(stmt->datum.selectio.corpus);
    imprimere("  Switch: OK\n");
}

/* ==================================================
 * Test: Return Statement
 * ================================================== */

interior vacuum
test_return_statement(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* func;
    ArborNodus* body;
    ArborNodus* stmt;

    imprimere("\n--- Return Statement ---\n");

    /* Return cum valore */
    res = _parsere_fontem(piscina, intern, "int f(void) { return 42; }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    func = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    body = func->datum.functio.corpus;
    stmt = *(ArborNodus**)xar_obtinere(body->datum.compositum.sententiae, ZEPHYRUM);
    CREDO_AEQUALIS_I32(stmt->genus, ARBOR_NODUS_RETURN_STATEMENT);
    CREDO_NON_NIHIL(stmt->datum.reditio.valor);
    imprimere("  Return valor: OK\n");

    /* Return vacuum */
    res = _parsere_fontem(piscina, intern, "void f(void) { return; }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    func = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    body = func->datum.functio.corpus;
    stmt = *(ArborNodus**)xar_obtinere(body->datum.compositum.sententiae, ZEPHYRUM);
    CREDO_AEQUALIS_PTR(stmt->datum.reditio.valor, NIHIL);
    imprimere("  Return vacuum: OK\n");
}

/* ==================================================
 * Test: Break/Continue
 * ================================================== */

interior vacuum
test_break_continue(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;

    imprimere("\n--- Break/Continue ---\n");

    res = _parsere_fontem(piscina, intern, "void f(void) { while(1) break; }");
    CREDO_VERUM(res.successus);
    imprimere("  Break: OK\n");

    res = _parsere_fontem(piscina, intern, "void f(void) { while(1) continue; }");
    CREDO_VERUM(res.successus);
    imprimere("  Continue: OK\n");
}

/* ==================================================
 * Test: Goto/Label
 * ================================================== */

interior vacuum
test_goto_label(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* func;
    ArborNodus* body;
    ArborNodus* stmt;

    imprimere("\n--- Goto/Label ---\n");

    res = _parsere_fontem(piscina, intern, "void f(void) { goto end; end: return; }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    func = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    body = func->datum.functio.corpus;

    /* Primum: goto */
    stmt = *(ArborNodus**)xar_obtinere(body->datum.compositum.sententiae, ZEPHYRUM);
    CREDO_AEQUALIS_I32(stmt->genus, ARBOR_NODUS_GOTO_STATEMENT);
    imprimere("  Goto: OK\n");

    /* Secundum: label */
    stmt = *(ArborNodus**)xar_obtinere(body->datum.compositum.sententiae, I);
    CREDO_AEQUALIS_I32(stmt->genus, ARBOR_NODUS_LABELED_STATEMENT);
    imprimere("  Label: OK\n");
}

/* ==================================================
 * Test: Declarations
 * ================================================== */

interior vacuum
test_declarations(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;

    imprimere("\n--- Declarations ---\n");

    /* Simplex */
    res = _parsere_fontem(piscina, intern, "int x;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    CREDO_AEQUALIS_I32(decl->genus, ARBOR_NODUS_DECLARATION);
    imprimere("  Simplex: OK\n");

    /* Cum initializatione */
    res = _parsere_fontem(piscina, intern, "int y = 42;");
    CREDO_VERUM(res.successus);
    imprimere("  Cum init: OK\n");

    /* Multiple declaratores */
    res = _parsere_fontem(piscina, intern, "int a, b, c;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(decl->datum.declaratio.declaratores), III);
    imprimere("  Multiple: OK\n");

    /* Pointer */
    res = _parsere_fontem(piscina, intern, "int *p;");
    CREDO_VERUM(res.successus);
    imprimere("  Pointer: OK\n");

    /* Double pointer */
    res = _parsere_fontem(piscina, intern, "int **pp;");
    CREDO_VERUM(res.successus);
    imprimere("  Double pointer: OK\n");

    /* Const */
    res = _parsere_fontem(piscina, intern, "const int x = 5;");
    CREDO_VERUM(res.successus);
    imprimere("  Const: OK\n");

    /* Static */
    res = _parsere_fontem(piscina, intern, "static int counter;");
    CREDO_VERUM(res.successus);
    imprimere("  Static: OK\n");

    /* Extern */
    res = _parsere_fontem(piscina, intern, "extern int global;");
    CREDO_VERUM(res.successus);
    imprimere("  Extern: OK\n");

    /* Unsigned */
    res = _parsere_fontem(piscina, intern, "unsigned int u;");
    CREDO_VERUM(res.successus);
    imprimere("  Unsigned: OK\n");

    /* Long */
    res = _parsere_fontem(piscina, intern, "long int l;");
    CREDO_VERUM(res.successus);
    imprimere("  Long: OK\n");
}

/* ==================================================
 * Test: Function Definition
 * ================================================== */

interior vacuum
test_function_definition(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* func;

    imprimere("\n--- Function Definition ---\n");

    /* Simplex void */
    res = _parsere_fontem(piscina, intern, "void f(void) { }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    func = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    CREDO_AEQUALIS_I32(func->genus, ARBOR_NODUS_FUNCTION_DEFINITION);
    CREDO_NON_NIHIL(func->datum.functio.corpus);
    imprimere("  Void simplex: OK\n");

    /* Cum valore reditus */
    res = _parsere_fontem(piscina, intern, "int square(int x) { return x * x; }");
    CREDO_VERUM(res.successus);
    imprimere("  Cum parametro: OK\n");

    /* Multiple parameters */
    res = _parsere_fontem(piscina, intern, "int add(int a, int b) { return a + b; }");
    CREDO_VERUM(res.successus);
    imprimere("  Multiple params: OK\n");

    /* Pointer parameter */
    res = _parsere_fontem(piscina, intern, "void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }");
    CREDO_VERUM(res.successus);
    imprimere("  Pointer param: OK\n");

    /* Static function */
    res = _parsere_fontem(piscina, intern, "static int helper(void) { return 0; }");
    CREDO_VERUM(res.successus);
    imprimere("  Static func: OK\n");
}

/* ==================================================
 * Test: Multiple Functions
 * ================================================== */

interior vacuum
test_multiple_functions(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;

    imprimere("\n--- Multiple Functions ---\n");

    res = _parsere_fontem(piscina, intern,
        "int foo(void) { return 1; }\n"
        "int bar(void) { return 2; }\n"
        "int main(void) { return foo() + bar(); }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    CREDO_AEQUALIS_I32(xar_numerus(radix->datum.genericum.liberi), III);
    imprimere("  Multiple functions: OK\n");
}

/* ==================================================
 * Test: Mixed Declarations and Functions
 * ================================================== */

interior vacuum
test_mixed_toplevel(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;

    imprimere("\n--- Mixed Toplevel ---\n");

    res = _parsere_fontem(piscina, intern,
        "int global = 0;\n"
        "void inc(void) { global++; }\n"
        "int get(void) { return global; }");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    CREDO_AEQUALIS_I32(xar_numerus(radix->datum.genericum.liberi), III);
    imprimere("  Mixed toplevel: OK\n");
}

#endif /* Full tests block 1 */

/* ==================================================
 * Test: Nodus Genus Nomen
 * ================================================== */

interior vacuum
test_nodus_genus_nomen(Piscina* piscina, InternamentumChorda* intern)
{
    constans character* titulus;

    (vacuum)piscina;
    (vacuum)intern;

    imprimere("\n--- Nodus Genus Nomen ---\n");

    titulus = arbor_nodus_genus_nomen(ARBOR_NODUS_TRANSLATION_UNIT);
    CREDO_NON_NIHIL(titulus);
    imprimere("  TRANSLATION_UNIT: %s\n", titulus);

    titulus = arbor_nodus_genus_nomen(ARBOR_NODUS_FUNCTION_DEFINITION);
    CREDO_NON_NIHIL(titulus);
    imprimere("  FUNCTION_DEFINITION: %s\n", titulus);

    titulus = arbor_nodus_genus_nomen(ARBOR_NODUS_BINARY_EXPRESSION);
    CREDO_NON_NIHIL(titulus);
    imprimere("  BINARY_EXPRESSION: %s\n", titulus);

    imprimere("  Genus nomen: OK\n");
}

#if 1 /* Full tests block 2 - enabled */

/* ==================================================
 * Test: AST Imprimere (visual check)
 * ================================================== */

interior vacuum
test_imprimere(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;

    imprimere("\n--- AST Imprimere ---\n");

    res = _parsere_fontem(piscina, intern, "int main(void) { return 0; }");
    CREDO_VERUM(res.successus);

    imprimere("  AST pro 'int main(void) { return 0; }':\n");
    arbor_nodus_imprimere(res.radix, II);

    imprimere("  Imprimere: OK\n");
}

/* ==================================================
 * Test: Compound Expressions
 * ================================================== */

interior vacuum
test_compound_expressions(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;

    imprimere("\n--- Compound Expressions ---\n");

    /* Chained member access */
    res = _parsere_fontem(piscina, intern, "int x = a.b.c;");
    CREDO_VERUM(res.successus);
    imprimere("  Chained member: OK\n");

    /* Array of array */
    res = _parsere_fontem(piscina, intern, "int x = a[i][j];");
    CREDO_VERUM(res.successus);
    imprimere("  Multi subscript: OK\n");

    /* Call result member */
    res = _parsere_fontem(piscina, intern, "int x = foo().field;");
    CREDO_VERUM(res.successus);
    imprimere("  Call member: OK\n");

    /* Complex expression */
    res = _parsere_fontem(piscina, intern, "int x = a->b[i].c(d, e);");
    CREDO_VERUM(res.successus);
    imprimere("  Complex: OK\n");
}

/* ==================================================
 * Test: Parenthesized Expressions
 * ================================================== */

interior vacuum
test_parentheses(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* init;
    ArborNodus* expr;

    imprimere("\n--- Parentheses ---\n");

    /* (a + b) * c - parentheses override precedence */
    res = _parsere_fontem(piscina, intern, "int x = (a + b) * c;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    expr = init->datum.init_decl.initializer;

    /* Radix debet esse * */
    CREDO_AEQUALIS_I32(expr->datum.binarium.operator, ARBOR_LEXEMA_ASTERISCUS);
    /* Sinister debet esse + */
    CREDO_AEQUALIS_I32(expr->datum.binarium.sinister->datum.binarium.operator, ARBOR_LEXEMA_PLUS);
    imprimere("  Override precedence: OK\n");

    /* Nested parentheses */
    res = _parsere_fontem(piscina, intern, "int y = ((a));");
    CREDO_VERUM(res.successus);
    imprimere("  Nested parens: OK\n");
}

/* ==================================================
 * Test: Struct Definition
 * ================================================== */

interior vacuum
test_struct_definition(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* spec;

    imprimere("\n--- Struct Definition ---\n");

    /* Simplex struct declaration */
    res = _parsere_fontem(piscina, intern, "struct Point { int x; int y; };");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    CREDO_AEQUALIS_I32(decl->genus, ARBOR_NODUS_DECLARATION);
    spec = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.specifiers, ZEPHYRUM);
    CREDO_AEQUALIS_I32(spec->genus, ARBOR_NODUS_STRUCT_SPECIFIER);
    CREDO_NON_NIHIL(spec->datum.aggregatum.titulus);
    CREDO_NON_NIHIL(spec->datum.aggregatum.membra);
    CREDO_AEQUALIS_I32(xar_numerus(spec->datum.aggregatum.membra), II);
    imprimere("  Struct simplex: OK\n");

    /* Struct variable declaration */
    res = _parsere_fontem(piscina, intern, "struct Point p;");
    CREDO_VERUM(res.successus);
    imprimere("  Struct variabilis: OK\n");

    /* Forward declaration */
    res = _parsere_fontem(piscina, intern, "struct Node;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    spec = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.specifiers, ZEPHYRUM);
    CREDO_AEQUALIS_I32(spec->genus, ARBOR_NODUS_STRUCT_SPECIFIER);
    CREDO_NIHIL(spec->datum.aggregatum.membra);
    imprimere("  Forward decl: OK\n");

    /* Anonymous struct */
    res = _parsere_fontem(piscina, intern, "struct { int a; } anon;");
    CREDO_VERUM(res.successus);
    imprimere("  Anonymous struct: OK\n");

    /* Nested struct */
    res = _parsere_fontem(piscina, intern,
        "struct Outer { struct Inner { int x; } inner; int y; };");
    CREDO_VERUM(res.successus);
    imprimere("  Nested struct: OK\n");

    /* Struct with pointer member */
    res = _parsere_fontem(piscina, intern,
        "struct Node { int val; struct Node *next; };");
    CREDO_VERUM(res.successus);
    imprimere("  Self-referential: OK\n");
}

/* ==================================================
 * Test: Union Definition
 * ================================================== */

interior vacuum
test_union_definition(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* spec;

    imprimere("\n--- Union Definition ---\n");

    /* Simplex union */
    res = _parsere_fontem(piscina, intern, "union Value { int i; float f; };");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    spec = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.specifiers, ZEPHYRUM);
    CREDO_AEQUALIS_I32(spec->genus, ARBOR_NODUS_UNION_SPECIFIER);
    CREDO_NON_NIHIL(spec->datum.aggregatum.membra);
    CREDO_AEQUALIS_I32(xar_numerus(spec->datum.aggregatum.membra), II);
    imprimere("  Union simplex: OK\n");

    /* Union variable */
    res = _parsere_fontem(piscina, intern, "union Value v;");
    CREDO_VERUM(res.successus);
    imprimere("  Union variabilis: OK\n");

    /* Anonymous union */
    res = _parsere_fontem(piscina, intern, "union { int a; char b; } u;");
    CREDO_VERUM(res.successus);
    imprimere("  Anonymous union: OK\n");
}

/* ==================================================
 * Test: Enum Definition
 * ================================================== */

interior vacuum
test_enum_definition(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* spec;

    imprimere("\n--- Enum Definition ---\n");

    /* Simplex enum */
    res = _parsere_fontem(piscina, intern, "enum Color { RED, GREEN, BLUE };");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    spec = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.specifiers, ZEPHYRUM);
    CREDO_AEQUALIS_I32(spec->genus, ARBOR_NODUS_ENUM_SPECIFIER);
    CREDO_NON_NIHIL(spec->datum.enum_spec.enumeratores);
    CREDO_AEQUALIS_I32(xar_numerus(spec->datum.enum_spec.enumeratores), III);
    imprimere("  Enum simplex: OK\n");

    /* Enum with values */
    res = _parsere_fontem(piscina, intern, "enum Status { OK = 0, ERROR = 1, PENDING = 2 };");
    CREDO_VERUM(res.successus);
    imprimere("  Enum cum valoribus: OK\n");

    /* Enum variable */
    res = _parsere_fontem(piscina, intern, "enum Color c;");
    CREDO_VERUM(res.successus);
    imprimere("  Enum variabilis: OK\n");

    /* Anonymous enum */
    res = _parsere_fontem(piscina, intern, "enum { A, B, C } e;");
    CREDO_VERUM(res.successus);
    imprimere("  Anonymous enum: OK\n");

    /* Enum with trailing comma (C89 extension, common) */
    res = _parsere_fontem(piscina, intern, "enum Flags { F1 = 1, F2 = 2, };");
    CREDO_VERUM(res.successus);
    imprimere("  Trailing comma: OK\n");
}

/* ==================================================
 * Test: Typedef Declarations
 * ================================================== */

interior vacuum
test_typedef(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;

    imprimere("\n--- Typedef ---\n");

    /* Simple typedef */
    res = _parsere_fontem(piscina, intern, "typedef int Int32;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    CREDO_AEQUALIS_I32(decl->genus, ARBOR_NODUS_DECLARATION);
    imprimere("  Typedef simplex: OK\n");

    /* Typedef pointer */
    res = _parsere_fontem(piscina, intern, "typedef char *String;");
    CREDO_VERUM(res.successus);
    imprimere("  Typedef pointer: OK\n");

    /* Typedef struct */
    res = _parsere_fontem(piscina, intern, "typedef struct Point Point;");
    CREDO_VERUM(res.successus);
    imprimere("  Typedef struct: OK\n");

    /* Typedef struct definition */
    res = _parsere_fontem(piscina, intern, "typedef struct { int x; int y; } Point;");
    CREDO_VERUM(res.successus);
    imprimere("  Typedef struct def: OK\n");

    /* Using typedef'd name in declaration */
    res = _parsere_fontem(piscina, intern,
        "typedef int Int32;\n"
        "Int32 x;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    CREDO_AEQUALIS_I32(xar_numerus(radix->datum.genericum.liberi), II);
    imprimere("  Typedef usus: OK\n");

    /* Typedef function pointer */
    res = _parsere_fontem(piscina, intern, "typedef int (*Comparator)(int, int);");
    CREDO_VERUM(res.successus);
    imprimere("  Typedef func ptr: OK\n");
}

/* ==================================================
 * Test: Bitfields
 * ================================================== */

interior vacuum
test_bitfields(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;

    imprimere("\n--- Bitfields ---\n");

    /* Simple bitfield */
    res = _parsere_fontem(piscina, intern, "struct Flags { int a : 4; int b : 4; };");
    CREDO_VERUM(res.successus);
    imprimere("  Bitfield simplex: OK\n");

    /* Unsigned bitfield */
    res = _parsere_fontem(piscina, intern, "struct Bits { unsigned flags : 8; };");
    CREDO_VERUM(res.successus);
    imprimere("  Unsigned bitfield: OK\n");

    /* Mixed members */
    res = _parsere_fontem(piscina, intern, "struct Mixed { int x; int b : 3; int y; };");
    CREDO_VERUM(res.successus);
    imprimere("  Mixed members: OK\n");
}

/* ==================================================
 * Test: Extensions (__attribute__, etc.)
 * ================================================== */

interior vacuum
test_extensions(Piscina* piscina, InternamentumChorda* intern)
{
    ArborSyntaxisResultus res;
    ArborNodus* radix;
    ArborNodus* decl;
    ArborNodus* spec;

    imprimere("\n--- Extensions ---\n");

    /* __extension__ int x */
    res = _parsere_fontem(piscina, intern, "__extension__ int x;");
    CREDO_VERUM(res.successus);
    radix = res.radix;
    decl = *(ArborNodus**)xar_obtinere(radix->datum.genericum.liberi, ZEPHYRUM);
    CREDO_AEQUALIS_I32(decl->genus, ARBOR_NODUS_DECLARATION);
    spec = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.specifiers, ZEPHYRUM);
    CREDO_AEQUALIS_I32(spec->genus, ARBOR_NODUS_EXTENSION);
    imprimere("  __extension__: OK\n");

    /* __attribute__((packed)) before type */
    res = _parsere_fontem(piscina, intern, "__attribute__((packed)) int x;");
    CREDO_VERUM(res.successus);
    imprimere("  __attribute__: OK\n");

    /* __inline__ function */
    res = _parsere_fontem(piscina, intern, "__inline__ int foo(void) { return 0; }");
    CREDO_VERUM(res.successus);
    imprimere("  __inline__: OK\n");

    /* __volatile__ qualifier */
    res = _parsere_fontem(piscina, intern, "__volatile__ int v;");
    CREDO_VERUM(res.successus);
    imprimere("  __volatile__: OK\n");

    /* Multiple extensions */
    res = _parsere_fontem(piscina, intern, "__extension__ __attribute__((unused)) int y;");
    CREDO_VERUM(res.successus);
    imprimere("  Multiple ext: OK\n");
}

#endif /* Full tests block 2 */

/* ==================================================
 * Principale
 * ================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    b32 praeteritus;

    imprimere("===========================================\n");
    imprimere("PROBATIO ARBOR SYNTAXIS\n");
    imprimere("===========================================\n");

    piscina = piscina_generare_dynamicum("test_arbor_syntaxis", M);
    si (piscina == NIHIL)
    {
        imprimere("FATALE: Piscina non creata\n");
        redde I;
    }

    credo_aperire(piscina);

    intern = internamentum_creare(piscina);
    CREDO_NON_NIHIL(intern);

    /* Creatio */
    test_creare(piscina, intern);

    /* Simple test first */
    test_nodus_genus_nomen(piscina, intern);
    imprimere("  Simplex probatio: OK\n");

    /* Primary expressions */
    test_integer_literal(piscina, intern);
    test_float_literal(piscina, intern);
    test_char_literal(piscina, intern);
    test_string_literal(piscina, intern);
    test_identifier(piscina, intern);

    /* Unary */
    test_unary(piscina, intern);
    test_sizeof(piscina, intern);

    /* Binary */
    test_binary(piscina, intern);
    test_praecedentia(piscina, intern);

    /* Postfix */
    test_postfix(piscina, intern);

    /* Other expressions */
    test_ternary(piscina, intern);
    test_assignment(piscina, intern);
    test_parentheses(piscina, intern);
    test_compound_expressions(piscina, intern);

    /* Statements */
    test_if_statement(piscina, intern);
    test_while_statement(piscina, intern);
    test_do_statement(piscina, intern);
    test_for_statement(piscina, intern);
    test_switch_statement(piscina, intern);
    test_return_statement(piscina, intern);
    test_break_continue(piscina, intern);
    test_goto_label(piscina, intern);

    /* Declarations */
    test_declarations(piscina, intern);

    /* Functions */
    test_function_definition(piscina, intern);
    test_multiple_functions(piscina, intern);
    test_mixed_toplevel(piscina, intern);

    /* Phase 4: Struct/Union/Enum/Typedef */
    test_struct_definition(piscina, intern);
    test_union_definition(piscina, intern);
    test_enum_definition(piscina, intern);
    test_typedef(piscina, intern);
    test_bitfields(piscina, intern);
    test_extensions(piscina, intern);

    /* Utilities */
    test_imprimere(piscina, intern);

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
