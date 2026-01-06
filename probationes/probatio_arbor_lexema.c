/* ==================================================
 * PROBATIO ARBOR LEXEMA - Unit Tests pro C89 Lexer
 * ================================================== */

#include "credo.h"
#include "arbor_lexema.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Helper Macros
 * ================================================== */

#define TEST_LEXEMA(fons, expected_genus, expected_valor) \
    fac { \
        ArborLexator* lex_; \
        ArborLexema* tok_; \
        lex_ = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons)); \
        tok_ = arbor_lexema_proximum(lex_); \
        CREDO_AEQUALIS_I32(tok_->genus, expected_genus); \
        si (expected_valor != NIHIL) { \
            CREDO_CHORDA_AEQUALIS_LITERIS(tok_->valor, expected_valor); \
        } \
    } dum (FALSUM)

/* ==================================================
 * Test: C89 Keywords
 * ================================================== */

interior vacuum
test_keywords(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- C89 Keywords ---\n");

    /* Probare omnia 32 verba clausa */
    TEST_LEXEMA("auto", ARBOR_LEXEMA_AUTO, "auto");
    TEST_LEXEMA("break", ARBOR_LEXEMA_BREAK, "break");
    TEST_LEXEMA("case", ARBOR_LEXEMA_CASE, "case");
    TEST_LEXEMA("char", ARBOR_LEXEMA_CHAR, "char");
    TEST_LEXEMA("const", ARBOR_LEXEMA_CONST, "const");
    TEST_LEXEMA("continue", ARBOR_LEXEMA_CONTINUE, "continue");
    TEST_LEXEMA("default", ARBOR_LEXEMA_DEFAULT, "default");
    TEST_LEXEMA("do", ARBOR_LEXEMA_DO, "do");
    TEST_LEXEMA("double", ARBOR_LEXEMA_DOUBLE, "double");
    TEST_LEXEMA("else", ARBOR_LEXEMA_ELSE, "else");
    TEST_LEXEMA("enum", ARBOR_LEXEMA_ENUM, "enum");
    TEST_LEXEMA("extern", ARBOR_LEXEMA_EXTERN, "extern");
    TEST_LEXEMA("float", ARBOR_LEXEMA_FLOAT, "float");
    TEST_LEXEMA("for", ARBOR_LEXEMA_FOR, "for");
    TEST_LEXEMA("goto", ARBOR_LEXEMA_GOTO, "goto");
    TEST_LEXEMA("if", ARBOR_LEXEMA_IF, "if");
    TEST_LEXEMA("int", ARBOR_LEXEMA_INT, "int");
    TEST_LEXEMA("long", ARBOR_LEXEMA_LONG, "long");
    TEST_LEXEMA("register", ARBOR_LEXEMA_REGISTER, "register");
    TEST_LEXEMA("return", ARBOR_LEXEMA_RETURN, "return");
    TEST_LEXEMA("short", ARBOR_LEXEMA_SHORT, "short");
    TEST_LEXEMA("signed", ARBOR_LEXEMA_SIGNED, "signed");
    TEST_LEXEMA("sizeof", ARBOR_LEXEMA_SIZEOF, "sizeof");
    TEST_LEXEMA("static", ARBOR_LEXEMA_STATIC, "static");
    TEST_LEXEMA("struct", ARBOR_LEXEMA_STRUCT, "struct");
    TEST_LEXEMA("switch", ARBOR_LEXEMA_SWITCH, "switch");
    TEST_LEXEMA("typedef", ARBOR_LEXEMA_TYPEDEF, "typedef");
    TEST_LEXEMA("union", ARBOR_LEXEMA_UNION, "union");
    TEST_LEXEMA("unsigned", ARBOR_LEXEMA_UNSIGNED, "unsigned");
    TEST_LEXEMA("void", ARBOR_LEXEMA_VOID, "void");
    TEST_LEXEMA("volatile", ARBOR_LEXEMA_VOLATILE, "volatile");
    TEST_LEXEMA("while", ARBOR_LEXEMA_WHILE, "while");

    imprimere("  Keywords: OK\n");
}

/* ==================================================
 * Test: Identifiers
 * ================================================== */

interior vacuum
test_identifiers(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Identifiers ---\n");

    TEST_LEXEMA("foo", ARBOR_LEXEMA_IDENTIFICATOR, "foo");
    TEST_LEXEMA("_bar", ARBOR_LEXEMA_IDENTIFICATOR, "_bar");
    TEST_LEXEMA("__baz__", ARBOR_LEXEMA_IDENTIFICATOR, "__baz__");
    TEST_LEXEMA("camelCase", ARBOR_LEXEMA_IDENTIFICATOR, "camelCase");
    TEST_LEXEMA("snake_case", ARBOR_LEXEMA_IDENTIFICATOR, "snake_case");
    TEST_LEXEMA("x123", ARBOR_LEXEMA_IDENTIFICATOR, "x123");
    TEST_LEXEMA("CONSTANT", ARBOR_LEXEMA_IDENTIFICATOR, "CONSTANT");

    imprimere("  Identifiers: OK\n");
}

/* ==================================================
 * Test: Integer Literals
 * ================================================== */

interior vacuum
test_integers(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Integer Literals ---\n");

    /* Decimal */
    TEST_LEXEMA("0", ARBOR_LEXEMA_INTEGER, "0");
    TEST_LEXEMA("123", ARBOR_LEXEMA_INTEGER, "123");
    TEST_LEXEMA("42", ARBOR_LEXEMA_INTEGER, "42");

    /* Hex */
    TEST_LEXEMA("0x0", ARBOR_LEXEMA_INTEGER, "0x0");
    TEST_LEXEMA("0xFF", ARBOR_LEXEMA_INTEGER, "0xFF");
    TEST_LEXEMA("0xDEADBEEF", ARBOR_LEXEMA_INTEGER, "0xDEADBEEF");
    TEST_LEXEMA("0X1A2B", ARBOR_LEXEMA_INTEGER, "0X1A2B");

    /* Octal */
    TEST_LEXEMA("07", ARBOR_LEXEMA_INTEGER, "07");
    TEST_LEXEMA("0777", ARBOR_LEXEMA_INTEGER, "0777");

    /* Suffixes */
    TEST_LEXEMA("123L", ARBOR_LEXEMA_INTEGER, "123L");
    TEST_LEXEMA("123U", ARBOR_LEXEMA_INTEGER, "123U");
    TEST_LEXEMA("123UL", ARBOR_LEXEMA_INTEGER, "123UL");
    TEST_LEXEMA("123LU", ARBOR_LEXEMA_INTEGER, "123LU");

    imprimere("  Integer Literals: OK\n");
}

/* ==================================================
 * Test: Float Literals
 * ================================================== */

interior vacuum
test_floats(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Float Literals ---\n");

    TEST_LEXEMA("3.14", ARBOR_LEXEMA_FLOAT_LIT, "3.14");
    TEST_LEXEMA("0.5", ARBOR_LEXEMA_FLOAT_LIT, "0.5");
    TEST_LEXEMA("1.0e10", ARBOR_LEXEMA_FLOAT_LIT, "1.0e10");
    TEST_LEXEMA("1.0E-5", ARBOR_LEXEMA_FLOAT_LIT, "1.0E-5");
    TEST_LEXEMA("2.5e+3", ARBOR_LEXEMA_FLOAT_LIT, "2.5e+3");
    TEST_LEXEMA("1e10", ARBOR_LEXEMA_FLOAT_LIT, "1e10");
    TEST_LEXEMA("3.14f", ARBOR_LEXEMA_FLOAT_LIT, "3.14f");
    TEST_LEXEMA("3.14F", ARBOR_LEXEMA_FLOAT_LIT, "3.14F");
    TEST_LEXEMA("3.14L", ARBOR_LEXEMA_FLOAT_LIT, "3.14L");

    imprimere("  Float Literals: OK\n");
}

/* ==================================================
 * Test: String Literals
 * ================================================== */

interior vacuum
test_strings(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- String Literals ---\n");

    TEST_LEXEMA("\"hello\"", ARBOR_LEXEMA_STRING_LIT, "\"hello\"");
    TEST_LEXEMA("\"\"", ARBOR_LEXEMA_STRING_LIT, "\"\"");
    TEST_LEXEMA("\"with spaces\"", ARBOR_LEXEMA_STRING_LIT, "\"with spaces\"");
    TEST_LEXEMA("\"escape\\ntest\"", ARBOR_LEXEMA_STRING_LIT, "\"escape\\ntest\"");
    TEST_LEXEMA("\"tab\\there\"", ARBOR_LEXEMA_STRING_LIT, "\"tab\\there\"");
    TEST_LEXEMA("\"quote\\\"inside\"", ARBOR_LEXEMA_STRING_LIT, "\"quote\\\"inside\"");

    imprimere("  String Literals: OK\n");
}

/* ==================================================
 * Test: Character Literals
 * ================================================== */

interior vacuum
test_chars(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Character Literals ---\n");

    TEST_LEXEMA("'a'", ARBOR_LEXEMA_CHAR_LIT, "'a'");
    TEST_LEXEMA("'Z'", ARBOR_LEXEMA_CHAR_LIT, "'Z'");
    TEST_LEXEMA("'0'", ARBOR_LEXEMA_CHAR_LIT, "'0'");
    TEST_LEXEMA("'\\n'", ARBOR_LEXEMA_CHAR_LIT, "'\\n'");
    TEST_LEXEMA("'\\t'", ARBOR_LEXEMA_CHAR_LIT, "'\\t'");
    TEST_LEXEMA("'\\''", ARBOR_LEXEMA_CHAR_LIT, "'\\''");
    TEST_LEXEMA("'\\\\'", ARBOR_LEXEMA_CHAR_LIT, "'\\\\'");

    imprimere("  Character Literals: OK\n");
}

/* ==================================================
 * Test: Operators
 * ================================================== */

interior vacuum
test_operators(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Operators ---\n");

    /* Arithmetic */
    TEST_LEXEMA("+", ARBOR_LEXEMA_PLUS, "+");
    TEST_LEXEMA("-", ARBOR_LEXEMA_MINUS, "-");
    TEST_LEXEMA("*", ARBOR_LEXEMA_ASTERISCUS, "*");
    TEST_LEXEMA("/", ARBOR_LEXEMA_SOLIDUS, "/");
    TEST_LEXEMA("%", ARBOR_LEXEMA_PERCENTUM, "%");

    /* Bitwise */
    TEST_LEXEMA("&", ARBOR_LEXEMA_AMPERSAND, "&");
    TEST_LEXEMA("|", ARBOR_LEXEMA_PIPA, "|");
    TEST_LEXEMA("^", ARBOR_LEXEMA_CARET, "^");
    TEST_LEXEMA("~", ARBOR_LEXEMA_TILDE, "~");

    /* Logical */
    TEST_LEXEMA("!", ARBOR_LEXEMA_EXCLAMATIO, "!");
    TEST_LEXEMA("&&", ARBOR_LEXEMA_DUAMPERSAND, "&&");
    TEST_LEXEMA("||", ARBOR_LEXEMA_DUPIPA, "||");

    /* Comparison */
    TEST_LEXEMA("==", ARBOR_LEXEMA_AEQUALIS, "==");
    TEST_LEXEMA("!=", ARBOR_LEXEMA_NON_AEQUALIS, "!=");
    TEST_LEXEMA("<", ARBOR_LEXEMA_MINOR, "<");
    TEST_LEXEMA(">", ARBOR_LEXEMA_MAIOR, ">");
    TEST_LEXEMA("<=", ARBOR_LEXEMA_MINOR_AEQ, "<=");
    TEST_LEXEMA(">=", ARBOR_LEXEMA_MAIOR_AEQ, ">=");

    /* Shift */
    TEST_LEXEMA("<<", ARBOR_LEXEMA_SINISTRUM, "<<");
    TEST_LEXEMA(">>", ARBOR_LEXEMA_DEXTRUM, ">>");

    /* Assignment */
    TEST_LEXEMA("=", ARBOR_LEXEMA_ASSIGNATIO, "=");
    TEST_LEXEMA("+=", ARBOR_LEXEMA_PLUS_ASSIGN, "+=");
    TEST_LEXEMA("-=", ARBOR_LEXEMA_MINUS_ASSIGN, "-=");
    TEST_LEXEMA("*=", ARBOR_LEXEMA_MULT_ASSIGN, "*=");
    TEST_LEXEMA("/=", ARBOR_LEXEMA_DIV_ASSIGN, "/=");
    TEST_LEXEMA("%=", ARBOR_LEXEMA_MOD_ASSIGN, "%=");
    TEST_LEXEMA("&=", ARBOR_LEXEMA_AND_ASSIGN, "&=");
    TEST_LEXEMA("|=", ARBOR_LEXEMA_OR_ASSIGN, "|=");
    TEST_LEXEMA("^=", ARBOR_LEXEMA_XOR_ASSIGN, "^=");
    TEST_LEXEMA("<<=", ARBOR_LEXEMA_SHL_ASSIGN, "<<=");
    TEST_LEXEMA(">>=", ARBOR_LEXEMA_SHR_ASSIGN, ">>=");

    /* Increment/Decrement */
    TEST_LEXEMA("++", ARBOR_LEXEMA_DUPLUS, "++");
    TEST_LEXEMA("--", ARBOR_LEXEMA_DUMINUS, "--");

    /* Member access */
    TEST_LEXEMA(".", ARBOR_LEXEMA_PUNCTUM, ".");
    TEST_LEXEMA("->", ARBOR_LEXEMA_SAGITTA, "->");

    imprimere("  Operators: OK\n");
}

/* ==================================================
 * Test: Punctuation
 * ================================================== */

interior vacuum
test_punctuation(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Punctuation ---\n");

    TEST_LEXEMA(",", ARBOR_LEXEMA_COMMA, ",");
    TEST_LEXEMA(":", ARBOR_LEXEMA_COLON, ":");
    TEST_LEXEMA(";", ARBOR_LEXEMA_SEMICOLON, ";");
    TEST_LEXEMA("?", ARBOR_LEXEMA_QUAESTIO, "?");
    TEST_LEXEMA("(", ARBOR_LEXEMA_PAREN_APERTA, "(");
    TEST_LEXEMA(")", ARBOR_LEXEMA_PAREN_CLAUSA, ")");
    TEST_LEXEMA("[", ARBOR_LEXEMA_BRACKET_APERTA, "[");
    TEST_LEXEMA("]", ARBOR_LEXEMA_BRACKET_CLAUSA, "]");
    TEST_LEXEMA("{", ARBOR_LEXEMA_BRACE_APERTA, "{");
    TEST_LEXEMA("}", ARBOR_LEXEMA_BRACE_CLAUSA, "}");
    TEST_LEXEMA("#", ARBOR_LEXEMA_HASH, "#");
    TEST_LEXEMA("##", ARBOR_LEXEMA_HASH_HASH, "##");
    TEST_LEXEMA("...", ARBOR_LEXEMA_ELLIPSIS, "...");

    imprimere("  Punctuation: OK\n");
}

/* ==================================================
 * Test: Trivia (Whitespace and Comments)
 * ================================================== */

interior vacuum
test_trivia(Piscina* piscina, InternamentumChorda* intern)
{
    ArborLexator* lex;
    ArborLexema* tok;
    ArborTrivia* trivia;

    imprimere("\n--- Trivia ---\n");

    /* Leading whitespace */
    lex = arbor_lexator_creare(piscina, intern, "   int", VI);
    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_INT);
    CREDO_NON_NIHIL(tok->trivia_ante);
    CREDO_AEQUALIS_I32(xar_numerus(tok->trivia_ante), I);
    trivia = *(ArborTrivia**)xar_obtinere(tok->trivia_ante, ZEPHYRUM);
    CREDO_FALSUM(trivia->est_commentum);
    CREDO_AEQUALIS_I32(trivia->valor.mensura, III);

    /* Block comment */
    lex = arbor_lexator_creare(piscina, intern, "/* comment */ int", XVII);
    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_INT);
    CREDO_NON_NIHIL(tok->trivia_ante);
    CREDO_AEQUALIS_I32(xar_numerus(tok->trivia_ante), II);  /* comment + space */
    trivia = *(ArborTrivia**)xar_obtinere(tok->trivia_ante, ZEPHYRUM);
    CREDO_VERUM(trivia->est_commentum);
    CREDO_FALSUM(trivia->est_c99);

    /* Line comment (C99) */
    lex = arbor_lexator_creare(piscina, intern, "// comment\nint", XIV);
    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_INT);
    CREDO_NON_NIHIL(tok->trivia_ante);
    trivia = *(ArborTrivia**)xar_obtinere(tok->trivia_ante, ZEPHYRUM);
    CREDO_VERUM(trivia->est_commentum);
    CREDO_VERUM(trivia->est_c99);

    /* Trailing comment */
    lex = arbor_lexator_creare(piscina, intern, "int /* trailing */", XVIII);
    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_INT);
    CREDO_NON_NIHIL(tok->trivia_post);
    CREDO_AEQUALIS_I32(xar_numerus(tok->trivia_post), II);  /* space + comment */

    imprimere("  Trivia: OK\n");
}

/* ==================================================
 * Test: Line/Column Tracking
 * ================================================== */

interior vacuum
test_positions(Piscina* piscina, InternamentumChorda* intern)
{
    ArborLexator* lex;
    ArborLexema* tok;

    imprimere("\n--- Position Tracking ---\n");

    /* Simple positioning */
    lex = arbor_lexator_creare(piscina, intern, "int x", V);
    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->linea, I);
    CREDO_AEQUALIS_I32(tok->columna, I);
    CREDO_AEQUALIS_I32(tok->byte_offset, ZEPHYRUM);

    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->linea, I);
    CREDO_AEQUALIS_I32(tok->columna, V);
    CREDO_AEQUALIS_I32(tok->byte_offset, IV);

    /* Multi-line */
    lex = arbor_lexator_creare(piscina, intern, "int\nx", V);
    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->linea, I);

    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->linea, II);
    CREDO_AEQUALIS_I32(tok->columna, I);

    imprimere("  Position Tracking: OK\n");
}

/* ==================================================
 * Test: Token Sequence
 * ================================================== */

interior vacuum
test_sequence(Piscina* piscina, InternamentumChorda* intern)
{
    ArborLexator* lex;
    ArborLexema* tok;

    imprimere("\n--- Token Sequence ---\n");

    /* Function declaration */
    lex = arbor_lexator_creare(piscina, intern, "int main(void)", XIV);

    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_INT);

    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_IDENTIFICATOR);
    CREDO_CHORDA_AEQUALIS_LITERIS(tok->valor, "main");

    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_PAREN_APERTA);

    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_VOID);

    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_PAREN_CLAUSA);

    tok = arbor_lexema_proximum(lex);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_EOF);

    imprimere("  Token Sequence: OK\n");
}

/* ==================================================
 * Test: arbor_lexema_omnia
 * ================================================== */

interior vacuum
test_omnia(Piscina* piscina, InternamentumChorda* intern)
{
    ArborLexator* lex;
    Xar* tokens;
    ArborLexema* tok;

    imprimere("\n--- arbor_lexema_omnia ---\n");

    lex = arbor_lexator_creare(piscina, intern, "int x = 42;", XI);
    tokens = arbor_lexema_omnia(lex);

    CREDO_NON_NIHIL(tokens);
    CREDO_AEQUALIS_I32(xar_numerus(tokens), VI);  /* int, x, =, 42, ;, EOF */

    tok = *(ArborLexema**)xar_obtinere(tokens, ZEPHYRUM);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_INT);

    tok = *(ArborLexema**)xar_obtinere(tokens, I);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_IDENTIFICATOR);

    tok = *(ArborLexema**)xar_obtinere(tokens, II);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_ASSIGNATIO);

    tok = *(ArborLexema**)xar_obtinere(tokens, III);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_INTEGER);

    tok = *(ArborLexema**)xar_obtinere(tokens, IV);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_SEMICOLON);

    tok = *(ArborLexema**)xar_obtinere(tokens, V);
    CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_EOF);

    imprimere("  arbor_lexema_omnia: OK\n");
}

/* ==================================================
 * Test: genus_nomen
 * ================================================== */

interior vacuum
test_genus_nomen(vacuum)
{
    imprimere("\n--- arbor_lexema_genus_nomen ---\n");

    CREDO_AEQUALIS_S32(strcmp(arbor_lexema_genus_nomen(ARBOR_LEXEMA_INT), "INT"), ZEPHYRUM);
    CREDO_AEQUALIS_S32(strcmp(arbor_lexema_genus_nomen(ARBOR_LEXEMA_IDENTIFICATOR), "IDENTIFICATOR"), ZEPHYRUM);
    CREDO_AEQUALIS_S32(strcmp(arbor_lexema_genus_nomen(ARBOR_LEXEMA_PLUS), "PLUS"), ZEPHYRUM);
    CREDO_AEQUALIS_S32(strcmp(arbor_lexema_genus_nomen(ARBOR_LEXEMA_EOF), "EOF"), ZEPHYRUM);

    imprimere("  arbor_lexema_genus_nomen: OK\n");
}

/* ==================================================
 * Main
 * ================================================== */

s32 principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("test_arbor_lexema", M);
    intern = internamentum_creare(piscina);

    credo_aperire(piscina);

    imprimere("\n========================================\n");
    imprimere("PROBATIO ARBOR LEXEMA\n");
    imprimere("========================================\n");

    test_keywords(piscina, intern);
    test_identifiers(piscina, intern);
    test_integers(piscina, intern);
    test_floats(piscina, intern);
    test_strings(piscina, intern);
    test_chars(piscina, intern);
    test_operators(piscina, intern);
    test_punctuation(piscina, intern);
    test_trivia(piscina, intern);
    test_positions(piscina, intern);
    test_sequence(piscina, intern);
    test_omnia(piscina, intern);
    test_genus_nomen();

    imprimere("\n========================================\n");
    credo_imprimere_compendium();
    imprimere("========================================\n");

    praeteritus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
