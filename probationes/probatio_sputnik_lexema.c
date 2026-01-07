/* probatio_sputnik_lexema.c - Probationes pro Sputnik Lexer */

#include "../include/sputnik_lexema.h"
#include "../include/credo.h"
#include <stdio.h>


/* ==================================================
 * Adiutores
 * ================================================== */

interior SputnikLexema
_legere_primum(constans character* fons, Piscina* piscina, InternamentumChorda* intern)
{
    SputnikLexator* lex;
    lex = sputnik_lexator_creare_ex_literis(fons, piscina, intern);
    redde sputnik_lexator_legere(lex);
}

interior vacuum
_probare_lexema_simplex(
    constans character*  fons,
    SputnikLexemaGenus   expectatus,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    SputnikLexema lexema;
    lexema = _legere_primum(fons, piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, expectatus);
}


/* ==================================================
 * Probationes: Punctuatio
 * ================================================== */

interior vacuum
_probare_punctuatio(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Punctuatio ---\n");

    _probare_lexema_simplex("(", SPUTNIK_LEXEMA_PARENTHESIS_A, piscina, intern);
    _probare_lexema_simplex(")", SPUTNIK_LEXEMA_PARENTHESIS_C, piscina, intern);
    _probare_lexema_simplex("{", SPUTNIK_LEXEMA_BRACCHIUM_A, piscina, intern);
    _probare_lexema_simplex("}", SPUTNIK_LEXEMA_BRACCHIUM_C, piscina, intern);
    _probare_lexema_simplex("[", SPUTNIK_LEXEMA_QUADRATUM_A, piscina, intern);
    _probare_lexema_simplex("]", SPUTNIK_LEXEMA_QUADRATUM_C, piscina, intern);
    _probare_lexema_simplex(";", SPUTNIK_LEXEMA_SEMICOLON, piscina, intern);
    _probare_lexema_simplex(",", SPUTNIK_LEXEMA_COMMA, piscina, intern);
    _probare_lexema_simplex(".", SPUTNIK_LEXEMA_PUNCTUM, piscina, intern);
    _probare_lexema_simplex(":", SPUTNIK_LEXEMA_COLON, piscina, intern);

    imprimere("  Punctuatio: VERUM\n");
}


/* ==================================================
 * Probationes: Operatores
 * ================================================== */

interior vacuum
_probare_operatores(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Operatores ---\n");

    /* Arithmetici */
    _probare_lexema_simplex("+", SPUTNIK_LEXEMA_PLUS, piscina, intern);
    _probare_lexema_simplex("-", SPUTNIK_LEXEMA_MINUS, piscina, intern);
    _probare_lexema_simplex("*", SPUTNIK_LEXEMA_ASTERISCUS, piscina, intern);
    _probare_lexema_simplex("/", SPUTNIK_LEXEMA_DIVISIO, piscina, intern);
    _probare_lexema_simplex("%", SPUTNIK_LEXEMA_MODULUS, piscina, intern);

    /* Comparatio */
    _probare_lexema_simplex("=", SPUTNIK_LEXEMA_AEQUALIS, piscina, intern);
    _probare_lexema_simplex("==", SPUTNIK_LEXEMA_AEQUALIS_DUO, piscina, intern);
    _probare_lexema_simplex("===", SPUTNIK_LEXEMA_AEQUALIS_TRIA, piscina, intern);
    _probare_lexema_simplex("!=", SPUTNIK_LEXEMA_NON_AEQUALIS, piscina, intern);
    _probare_lexema_simplex("!==", SPUTNIK_LEXEMA_STRICTE_NON_AEQ, piscina, intern);
    _probare_lexema_simplex("<", SPUTNIK_LEXEMA_MINOR, piscina, intern);
    _probare_lexema_simplex(">", SPUTNIK_LEXEMA_MAIOR, piscina, intern);
    _probare_lexema_simplex("<=", SPUTNIK_LEXEMA_MINOR_AUT, piscina, intern);
    _probare_lexema_simplex(">=", SPUTNIK_LEXEMA_MAIOR_AUT, piscina, intern);

    /* Logici */
    _probare_lexema_simplex("&&", SPUTNIK_LEXEMA_ET, piscina, intern);
    _probare_lexema_simplex("||", SPUTNIK_LEXEMA_VEL, piscina, intern);
    _probare_lexema_simplex("!", SPUTNIK_LEXEMA_NON, piscina, intern);

    /* Speciales */
    _probare_lexema_simplex("=>", SPUTNIK_LEXEMA_SAGITTA, piscina, intern);
    _probare_lexema_simplex("...", SPUTNIK_LEXEMA_PUNCTUM_TRIPLEX, piscina, intern);

    imprimere("  Operatores: VERUM\n");
}


/* ==================================================
 * Probationes: Numeri
 * ================================================== */

interior vacuum
_probare_numeros(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikLexema lexema;

    imprimere("\n--- Probatio Numeri ---\n");

    /* Integer simplex */
    lexema = _legere_primum("42", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_NUMERUS);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, II);

    /* Integer cum underscore */
    lexema = _legere_primum("1_000_000", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_NUMERUS);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, IX);

    /* Float */
    lexema = _legere_primum("3.14", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_NUMERUS);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, IV);

    /* Float cum underscore */
    lexema = _legere_primum("1_000.50", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_NUMERUS);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, VIII);

    /* Zero */
    lexema = _legere_primum("0", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_NUMERUS);

    imprimere("  Numeri: VERUM\n");
}


/* ==================================================
 * Probationes: Chordae (Strings)
 * ================================================== */

interior vacuum
_probare_chordas(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikLexema lexema;

    imprimere("\n--- Probatio Chordae ---\n");

    /* Chorda cum quotis duplicibus - nunc sine quotis, cum effugiis processatis */
    lexema = _legere_primum("\"hello\"", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_CHORDA);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, V);  /* "hello" sine quotis = 5 */

    /* Chorda cum quotis singularibus */
    lexema = _legere_primum("'world'", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_CHORDA);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, V);  /* 'world' sine quotis = 5 */

    /* Chorda vacua */
    lexema = _legere_primum("\"\"", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_CHORDA);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, ZEPHYRUM);  /* "" vacua = 0 */

    /* Chorda cum spatiis */
    lexema = _legere_primum("\"hello world\"", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_CHORDA);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, XI);  /* "hello world" = 11 */

    /* Chorda cum effugiis */
    lexema = _legere_primum("\"a\\nb\"", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_CHORDA);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, III);  /* a + newline + b = 3 */

    imprimere("  Chordae: VERUM\n");
}


/* ==================================================
 * Probationes: Identificatores
 * ================================================== */

interior vacuum
_probare_identificatores(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikLexema lexema;

    imprimere("\n--- Probatio Identificatores ---\n");

    /* Simplex */
    lexema = _legere_primum("foo", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_IDENTIFICATOR);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, III);

    /* Cum underscore */
    lexema = _legere_primum("foo_bar", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_IDENTIFICATOR);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, VII);

    /* Kebab-case */
    lexema = _legere_primum("foo-bar", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_IDENTIFICATOR);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, VII);

    /* Kebab-case cum numero */
    lexema = _legere_primum("x-1", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_IDENTIFICATOR);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, III);

    /* Kebab-case longum */
    lexema = _legere_primum("get-user-id", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_IDENTIFICATOR);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, XI);

    /* Cum cifris */
    lexema = _legere_primum("var123", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_IDENTIFICATOR);

    /* Incipit cum underscore */
    lexema = _legere_primum("_private", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_IDENTIFICATOR);

    imprimere("  Identificatores: VERUM\n");
}


/* ==================================================
 * Probationes: Verba Clausa (Keywords)
 * ================================================== */

interior vacuum
_probare_verba_clausa(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Verba Clausa ---\n");

    _probare_lexema_simplex("sit", SPUTNIK_LEXEMA_LET, piscina, intern);
    _probare_lexema_simplex("constans", SPUTNIK_LEXEMA_CONST, piscina, intern);
    _probare_lexema_simplex("si", SPUTNIK_LEXEMA_SI, piscina, intern);
    _probare_lexema_simplex("alioquin", SPUTNIK_LEXEMA_ALIOQUIN, piscina, intern);
    _probare_lexema_simplex("dum", SPUTNIK_LEXEMA_DUM, piscina, intern);
    _probare_lexema_simplex("per", SPUTNIK_LEXEMA_PER, piscina, intern);
    _probare_lexema_simplex("redde", SPUTNIK_LEXEMA_REDDE, piscina, intern);
    _probare_lexema_simplex("functio", SPUTNIK_LEXEMA_FUNCTIO, piscina, intern);
    _probare_lexema_simplex("verum", SPUTNIK_LEXEMA_VERUM, piscina, intern);
    _probare_lexema_simplex("falsum", SPUTNIK_LEXEMA_FALSUM, piscina, intern);
    _probare_lexema_simplex("nihil", SPUTNIK_LEXEMA_NIHIL, piscina, intern);

    /* Non-keyword quod simile */
    _probare_lexema_simplex("situ", SPUTNIK_LEXEMA_IDENTIFICATOR, piscina, intern);
    _probare_lexema_simplex("constantia", SPUTNIK_LEXEMA_IDENTIFICATOR, piscina, intern);
    _probare_lexema_simplex("sibi", SPUTNIK_LEXEMA_IDENTIFICATOR, piscina, intern);

    imprimere("  Verba Clausa: VERUM\n");
}


/* ==================================================
 * Probationes: Signa (Tags)
 * ================================================== */

interior vacuum
_probare_signa(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikLexema lexema;

    imprimere("\n--- Probatio Signa ---\n");

    /* Signum simplex */
    lexema = _legere_primum("#tag", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_SIGNUM);
    CREDO_AEQUALIS_I32(lexema.valor.mensura, IV);

    /* Signum cum namespace */
    lexema = _legere_primum("#Number::inches", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_SIGNUM);

    /* Signum kebab-case */
    lexema = _legere_primum("#client-request", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_SIGNUM);

    /* Signum malum (# solum) */
    lexema = _legere_primum("# ", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_ERROR);

    imprimere("  Signa: VERUM\n");
}


/* ==================================================
 * Probationes: Commenta
 * ================================================== */

interior vacuum
_probare_commenta(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikLexema lexema;
    SputnikLexator* lex;

    imprimere("\n--- Probatio Commenta ---\n");

    /* Commentum linea */
    lexema = _legere_primum("// this is a comment", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_COMMENTUM);

    /* Commentum bloc */
    lexema = _legere_primum("/* block comment */", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_COMMENTUM);

    /* Commentum ante codicem */
    lex = sputnik_lexator_creare_ex_literis("// comment\nx", piscina, intern);
    lexema = sputnik_lexator_legere(lex);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_COMMENTUM);
    lexema = sputnik_lexator_legere(lex);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_IDENTIFICATOR);

    imprimere("  Commenta: VERUM\n");
}


/* ==================================================
 * Probationes: Positiones
 * ================================================== */

interior vacuum
_probare_positiones(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikLexema lexema;
    SputnikLexator* lex;

    imprimere("\n--- Probatio Positiones ---\n");

    /* Linea et columna */
    lex = sputnik_lexator_creare_ex_literis("a\nb\nc", piscina, intern);

    lexema = sputnik_lexator_legere(lex);
    CREDO_AEQUALIS_I32(lexema.linea, I);
    CREDO_AEQUALIS_I32(lexema.columna, I);

    lexema = sputnik_lexator_legere(lex);
    CREDO_AEQUALIS_I32(lexema.linea, II);
    CREDO_AEQUALIS_I32(lexema.columna, I);

    lexema = sputnik_lexator_legere(lex);
    CREDO_AEQUALIS_I32(lexema.linea, III);
    CREDO_AEQUALIS_I32(lexema.columna, I);

    /* Columna post spatia */
    lex = sputnik_lexator_creare_ex_literis("   x", piscina, intern);
    lexema = sputnik_lexator_legere(lex);
    CREDO_AEQUALIS_I32(lexema.columna, IV);

    imprimere("  Positiones: VERUM\n");
}


/* ==================================================
 * Probationes: Expressiones Complexae
 * ================================================== */

interior vacuum
_probare_expressiones(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikLexatorResultus resultus;
    SputnikLexema* lexema;

    imprimere("\n--- Probatio Expressiones ---\n");

    /* sit x = 42; */
    resultus = sputnik_lexicare_ex_literis("sit x = 42;", piscina, intern);
    CREDO_VERUM(resultus.successus);
    CREDO_AEQUALIS_I32(xar_numerus(resultus.lexemata), VI);  /* sit x = 42 ; FINIS */

    lexema = xar_obtinere(resultus.lexemata, ZEPHYRUM);
    CREDO_AEQUALIS_I32(lexema->genus, SPUTNIK_LEXEMA_LET);

    lexema = xar_obtinere(resultus.lexemata, I);
    CREDO_AEQUALIS_I32(lexema->genus, SPUTNIK_LEXEMA_IDENTIFICATOR);

    lexema = xar_obtinere(resultus.lexemata, II);
    CREDO_AEQUALIS_I32(lexema->genus, SPUTNIK_LEXEMA_AEQUALIS);

    lexema = xar_obtinere(resultus.lexemata, III);
    CREDO_AEQUALIS_I32(lexema->genus, SPUTNIK_LEXEMA_NUMERUS);

    lexema = xar_obtinere(resultus.lexemata, IV);
    CREDO_AEQUALIS_I32(lexema->genus, SPUTNIK_LEXEMA_SEMICOLON);

    lexema = xar_obtinere(resultus.lexemata, V);
    CREDO_AEQUALIS_I32(lexema->genus, SPUTNIK_LEXEMA_FINIS);

    /* Arrow function: (x) => x + 1 */
    resultus = sputnik_lexicare_ex_literis("(x) => x + 1", piscina, intern);
    CREDO_VERUM(resultus.successus);
    CREDO_AEQUALIS_I32(xar_numerus(resultus.lexemata), VIII);

    /* Kebab-case: foo - bar vs foo-bar */
    resultus = sputnik_lexicare_ex_literis("foo - bar", piscina, intern);
    CREDO_VERUM(resultus.successus);
    CREDO_AEQUALIS_I32(xar_numerus(resultus.lexemata), IV);  /* foo - bar FINIS */

    lexema = xar_obtinere(resultus.lexemata, ZEPHYRUM);
    CREDO_AEQUALIS_I32(lexema->genus, SPUTNIK_LEXEMA_IDENTIFICATOR);
    CREDO_AEQUALIS_I32(lexema->valor.mensura, III);  /* "foo" non "foo-bar" */

    resultus = sputnik_lexicare_ex_literis("foo-bar", piscina, intern);
    CREDO_VERUM(resultus.successus);
    CREDO_AEQUALIS_I32(xar_numerus(resultus.lexemata), II);  /* foo-bar FINIS */

    lexema = xar_obtinere(resultus.lexemata, ZEPHYRUM);
    CREDO_AEQUALIS_I32(lexema->genus, SPUTNIK_LEXEMA_IDENTIFICATOR);
    CREDO_AEQUALIS_I32(lexema->valor.mensura, VII);  /* "foo-bar" */

    imprimere("  Expressiones: VERUM\n");
}


/* ==================================================
 * Probationes: Errores
 * ================================================== */

interior vacuum
_probare_errores(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikLexema lexema;
    SputnikLexatorResultus resultus;

    imprimere("\n--- Probatio Errores ---\n");

    /* Chorda non clausa */
    lexema = _legere_primum("\"unterminated", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_ERROR);

    /* Character ignotus */
    lexema = _legere_primum("@", piscina, intern);
    CREDO_AEQUALIS_I32(lexema.genus, SPUTNIK_LEXEMA_ERROR);

    /* Full tokenization cum errore */
    resultus = sputnik_lexicare_ex_literis("sit x = @", piscina, intern);
    CREDO_FALSUM(resultus.successus);

    imprimere("  Errores: VERUM\n");
}


/* ==================================================
 * Principale
 * ================================================== */

s32 principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_sputnik_lexema", MMMMXCVI * IV);
    intern = internamentum_creare(piscina);

    credo_aperire(piscina);

    imprimere("=== Probationes Sputnik Lexema ===\n");

    _probare_punctuatio(piscina, intern);
    _probare_operatores(piscina, intern);
    _probare_numeros(piscina, intern);
    _probare_chordas(piscina, intern);
    _probare_identificatores(piscina, intern);
    _probare_verba_clausa(piscina, intern);
    _probare_signa(piscina, intern);
    _probare_commenta(piscina, intern);
    _probare_positiones(piscina, intern);
    _probare_expressiones(piscina, intern);
    _probare_errores(piscina, intern);

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
