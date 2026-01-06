#ifndef ARBOR_LEXEMA_H
#define ARBOR_LEXEMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"

/* ==================================================
 * ARBOR LEXEMA - Lexer pro C89
 *
 * Tokenizat fontem C in fluxum lexematum.
 * Conservat trivia (spatia, commenta) pro roundtrip.
 *
 * PHILOSOPHIA:
 * - Omnia ex piscina allocata
 * - Trivia ante et post conservata pro roundtrip
 * - Standard tracking (C89/C99/extension)
 * - 1-based linea/columna (ut editores)
 *
 * EXEMPLUM:
 *   ArborLexator* lex = arbor_lexator_creare(piscina, intern, fons, mensura);
 *   ArborLexema* tok;
 *   dum ((tok = arbor_lexema_proximum(lex))->genus != ARBOR_LEXEMA_EOF) {
 *       // processare tok
 *   }
 * ================================================== */

/* ==================================================
 * Standard Values
 * ================================================== */

#define ARBOR_STANDARD_C89       0
#define ARBOR_STANDARD_C99       1
#define ARBOR_STANDARD_EXTENSION 2

/* ==================================================
 * Token Types
 * ================================================== */

nomen enumeratio {
    /* C89 Keywords (32) */
    ARBOR_LEXEMA_AUTO,
    ARBOR_LEXEMA_BREAK,
    ARBOR_LEXEMA_CASE,
    ARBOR_LEXEMA_CHAR,
    ARBOR_LEXEMA_CONST,
    ARBOR_LEXEMA_CONTINUE,
    ARBOR_LEXEMA_DEFAULT,
    ARBOR_LEXEMA_DO,
    ARBOR_LEXEMA_DOUBLE,
    ARBOR_LEXEMA_ELSE,
    ARBOR_LEXEMA_ENUM,
    ARBOR_LEXEMA_EXTERN,
    ARBOR_LEXEMA_FLOAT,
    ARBOR_LEXEMA_FOR,
    ARBOR_LEXEMA_GOTO,
    ARBOR_LEXEMA_IF,
    ARBOR_LEXEMA_INT,
    ARBOR_LEXEMA_LONG,
    ARBOR_LEXEMA_REGISTER,
    ARBOR_LEXEMA_RETURN,
    ARBOR_LEXEMA_SHORT,
    ARBOR_LEXEMA_SIGNED,
    ARBOR_LEXEMA_SIZEOF,
    ARBOR_LEXEMA_STATIC,
    ARBOR_LEXEMA_STRUCT,
    ARBOR_LEXEMA_SWITCH,
    ARBOR_LEXEMA_TYPEDEF,
    ARBOR_LEXEMA_UNION,
    ARBOR_LEXEMA_UNSIGNED,
    ARBOR_LEXEMA_VOID,
    ARBOR_LEXEMA_VOLATILE,
    ARBOR_LEXEMA_WHILE,

    /* Identifiers and Literals */
    ARBOR_LEXEMA_IDENTIFICATOR,
    ARBOR_LEXEMA_INTEGER,
    ARBOR_LEXEMA_FLOAT_LIT,
    ARBOR_LEXEMA_CHAR_LIT,
    ARBOR_LEXEMA_STRING_LIT,

    /* Arithmetic Operators */
    ARBOR_LEXEMA_PLUS,           /* + */
    ARBOR_LEXEMA_MINUS,          /* - */
    ARBOR_LEXEMA_ASTERISCUS,     /* * */
    ARBOR_LEXEMA_SOLIDUS,        /* / */
    ARBOR_LEXEMA_PERCENTUM,      /* % */

    /* Bitwise Operators */
    ARBOR_LEXEMA_AMPERSAND,      /* & */
    ARBOR_LEXEMA_PIPA,           /* | */
    ARBOR_LEXEMA_CARET,          /* ^ */
    ARBOR_LEXEMA_TILDE,          /* ~ */

    /* Logical Operators */
    ARBOR_LEXEMA_EXCLAMATIO,     /* ! */
    ARBOR_LEXEMA_DUAMPERSAND,    /* && */
    ARBOR_LEXEMA_DUPIPA,         /* || */

    /* Comparison Operators */
    ARBOR_LEXEMA_AEQUALIS,       /* == */
    ARBOR_LEXEMA_NON_AEQUALIS,   /* != */
    ARBOR_LEXEMA_MINOR,          /* < */
    ARBOR_LEXEMA_MAIOR,          /* > */
    ARBOR_LEXEMA_MINOR_AEQ,      /* <= */
    ARBOR_LEXEMA_MAIOR_AEQ,      /* >= */

    /* Shift Operators */
    ARBOR_LEXEMA_SINISTRUM,      /* << */
    ARBOR_LEXEMA_DEXTRUM,        /* >> */

    /* Assignment Operators */
    ARBOR_LEXEMA_ASSIGNATIO,     /* = */
    ARBOR_LEXEMA_PLUS_ASSIGN,    /* += */
    ARBOR_LEXEMA_MINUS_ASSIGN,   /* -= */
    ARBOR_LEXEMA_MULT_ASSIGN,    /* *= */
    ARBOR_LEXEMA_DIV_ASSIGN,     /* /= */
    ARBOR_LEXEMA_MOD_ASSIGN,     /* %= */
    ARBOR_LEXEMA_AND_ASSIGN,     /* &= */
    ARBOR_LEXEMA_OR_ASSIGN,      /* |= */
    ARBOR_LEXEMA_XOR_ASSIGN,     /* ^= */
    ARBOR_LEXEMA_SHL_ASSIGN,     /* <<= */
    ARBOR_LEXEMA_SHR_ASSIGN,     /* >>= */

    /* Increment/Decrement */
    ARBOR_LEXEMA_DUPLUS,         /* ++ */
    ARBOR_LEXEMA_DUMINUS,        /* -- */

    /* Member Access */
    ARBOR_LEXEMA_PUNCTUM,        /* . */
    ARBOR_LEXEMA_SAGITTA,        /* -> */

    /* Punctuation */
    ARBOR_LEXEMA_COMMA,          /* , */
    ARBOR_LEXEMA_COLON,          /* : */
    ARBOR_LEXEMA_SEMICOLON,      /* ; */
    ARBOR_LEXEMA_QUAESTIO,       /* ? */

    /* Delimiters */
    ARBOR_LEXEMA_PAREN_APERTA,   /* ( */
    ARBOR_LEXEMA_PAREN_CLAUSA,   /* ) */
    ARBOR_LEXEMA_BRACKET_APERTA, /* [ */
    ARBOR_LEXEMA_BRACKET_CLAUSA, /* ] */
    ARBOR_LEXEMA_BRACE_APERTA,   /* { */
    ARBOR_LEXEMA_BRACE_CLAUSA,   /* } */

    /* Preprocessor */
    ARBOR_LEXEMA_HASH,           /* # */
    ARBOR_LEXEMA_HASH_HASH,      /* ## */

    /* Ellipsis */
    ARBOR_LEXEMA_ELLIPSIS,       /* ... */

    /* Special */
    ARBOR_LEXEMA_EOF,
    ARBOR_LEXEMA_ERROR
} ArborLexemaGenus;

/* ==================================================
 * Trivia (whitespace and comments)
 * ================================================== */

nomen structura {
    chorda      valor;           /* Actual text content */
    b32         est_commentum;   /* true if comment, false if whitespace */
    b32         est_c99;         /* true if // comment (C99) */
    i32         linea;           /* Starting line (1-based) */
    i32         columna;         /* Starting column (1-based) */
} ArborTrivia;

/* ==================================================
 * Token
 * ================================================== */

nomen structura {
    ArborLexemaGenus  genus;         /* Token type */
    chorda            valor;         /* Text of token (identifier/literal text) */
    i32               byte_offset;   /* 0-based offset in source */
    i32               longitudo;     /* Length in bytes */
    i32               linea;         /* Starting line (1-based) */
    i32               columna;       /* Starting column (1-based) */
    Xar*              trivia_ante;   /* Leading trivia (Xar of ArborTrivia) */
    Xar*              trivia_post;   /* Trailing trivia (Xar of ArborTrivia) */
    i8                standard;      /* ARBOR_STANDARD_* */
} ArborLexema;

/* ==================================================
 * Lexer State (opaque)
 * ================================================== */

nomen structura ArborLexator ArborLexator;

/* ==================================================
 * API
 * ================================================== */

/* Creare lexator ex fonte
 *
 * piscina: Arena pro allocations
 * intern: String interning table
 * fons: Source text (not necessarily null-terminated)
 * mensura: Length of source in bytes
 */
ArborLexator* arbor_lexator_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura);

/* Obtinere proximum lexema
 *
 * Returns pointer to token (allocated from piscina).
 * Returns token with genus=ARBOR_LEXEMA_EOF at end.
 * Returns token with genus=ARBOR_LEXEMA_ERROR on error.
 */
ArborLexema* arbor_lexema_proximum(ArborLexator* lex);

/* Prospicere sine consumendo
 *
 * ahead: Number of tokens to look ahead (0 = next token)
 * Returns NULL if beyond end of input.
 */
ArborLexema* arbor_lexema_prospicere(ArborLexator* lex, i32 ahead);

/* Obtinere omnia lexemata
 *
 * Tokenizes entire source into Xar of ArborLexema.
 * Includes EOF token at end.
 * Returns NULL on allocation failure.
 */
Xar* arbor_lexema_omnia(ArborLexator* lex);

/* Obtinere nomen genus (pro debugging)
 *
 * Returns human-readable name for token type.
 */
constans character* arbor_lexema_genus_nomen(ArborLexemaGenus genus);

#endif /* ARBOR_LEXEMA_H */
