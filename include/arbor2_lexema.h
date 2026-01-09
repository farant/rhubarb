#ifndef ARBOR2_LEXEMA_H
#define ARBOR2_LEXEMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"

/* ==================================================
 * ARBOR2 LEXEMA - Lexer pro C89 (v2)
 *
 * Forked from arbor_lexema for arbor-v2.
 * Key differences:
 * - NOVA_LINEA token type for explicit newlines
 * - Line continuation tracking (est_continuatio)
 * - Newlines not consumed as trivia
 *
 * EXEMPLUM:
 *   Arbor2Lexator* lex = arbor2_lexator_creare(piscina, intern, fons, mensura);
 *   Arbor2Lexema* tok;
 *   dum ((tok = arbor2_lexema_proximum(lex))->genus != ARBOR2_LEXEMA_EOF) {
 *       // processare tok
 *   }
 * ================================================== */

/* ==================================================
 * Standard Values
 * ================================================== */

#define ARBOR2_STANDARD_C89       0
#define ARBOR2_STANDARD_C99       1
#define ARBOR2_STANDARD_EXTENSION 2

/* ==================================================
 * Token Types
 * ================================================== */

nomen enumeratio {
    /* C89 Keywords (32) */
    ARBOR2_LEXEMA_AUTO,
    ARBOR2_LEXEMA_BREAK,
    ARBOR2_LEXEMA_CASE,
    ARBOR2_LEXEMA_CHAR,
    ARBOR2_LEXEMA_CONST,
    ARBOR2_LEXEMA_CONTINUE,
    ARBOR2_LEXEMA_DEFAULT,
    ARBOR2_LEXEMA_DO,
    ARBOR2_LEXEMA_DOUBLE,
    ARBOR2_LEXEMA_ELSE,
    ARBOR2_LEXEMA_ENUM,
    ARBOR2_LEXEMA_EXTERN,
    ARBOR2_LEXEMA_FLOAT,
    ARBOR2_LEXEMA_FOR,
    ARBOR2_LEXEMA_GOTO,
    ARBOR2_LEXEMA_IF,
    ARBOR2_LEXEMA_INT,
    ARBOR2_LEXEMA_LONG,
    ARBOR2_LEXEMA_REGISTER,
    ARBOR2_LEXEMA_RETURN,
    ARBOR2_LEXEMA_SHORT,
    ARBOR2_LEXEMA_SIGNED,
    ARBOR2_LEXEMA_SIZEOF,
    ARBOR2_LEXEMA_STATIC,
    ARBOR2_LEXEMA_STRUCT,
    ARBOR2_LEXEMA_SWITCH,
    ARBOR2_LEXEMA_TYPEDEF,
    ARBOR2_LEXEMA_UNION,
    ARBOR2_LEXEMA_UNSIGNED,
    ARBOR2_LEXEMA_VOID,
    ARBOR2_LEXEMA_VOLATILE,
    ARBOR2_LEXEMA_WHILE,

    /* Identifiers and Literals */
    ARBOR2_LEXEMA_IDENTIFICATOR,
    ARBOR2_LEXEMA_INTEGER,
    ARBOR2_LEXEMA_FLOAT_LIT,
    ARBOR2_LEXEMA_CHAR_LIT,
    ARBOR2_LEXEMA_STRING_LIT,

    /* Arithmetic Operators */
    ARBOR2_LEXEMA_PLUS,           /* + */
    ARBOR2_LEXEMA_MINUS,          /* - */
    ARBOR2_LEXEMA_ASTERISCUS,     /* * */
    ARBOR2_LEXEMA_SOLIDUS,        /* / */
    ARBOR2_LEXEMA_PERCENTUM,      /* % */

    /* Bitwise Operators */
    ARBOR2_LEXEMA_AMPERSAND,      /* & */
    ARBOR2_LEXEMA_PIPA,           /* | */
    ARBOR2_LEXEMA_CARET,          /* ^ */
    ARBOR2_LEXEMA_TILDE,          /* ~ */

    /* Logical Operators */
    ARBOR2_LEXEMA_EXCLAMATIO,     /* ! */
    ARBOR2_LEXEMA_DUAMPERSAND,    /* && */
    ARBOR2_LEXEMA_DUPIPA,         /* || */

    /* Comparison Operators */
    ARBOR2_LEXEMA_AEQUALIS,       /* == */
    ARBOR2_LEXEMA_NON_AEQUALIS,   /* != */
    ARBOR2_LEXEMA_MINOR,          /* < */
    ARBOR2_LEXEMA_MAIOR,          /* > */
    ARBOR2_LEXEMA_MINOR_AEQ,      /* <= */
    ARBOR2_LEXEMA_MAIOR_AEQ,      /* >= */

    /* Shift Operators */
    ARBOR2_LEXEMA_SINISTRUM,      /* << */
    ARBOR2_LEXEMA_DEXTRUM,        /* >> */

    /* Assignment Operators */
    ARBOR2_LEXEMA_ASSIGNATIO,     /* = */
    ARBOR2_LEXEMA_PLUS_ASSIGN,    /* += */
    ARBOR2_LEXEMA_MINUS_ASSIGN,   /* -= */
    ARBOR2_LEXEMA_MULT_ASSIGN,    /* *= */
    ARBOR2_LEXEMA_DIV_ASSIGN,     /* /= */
    ARBOR2_LEXEMA_MOD_ASSIGN,     /* %= */
    ARBOR2_LEXEMA_AND_ASSIGN,     /* &= */
    ARBOR2_LEXEMA_OR_ASSIGN,      /* |= */
    ARBOR2_LEXEMA_XOR_ASSIGN,     /* ^= */
    ARBOR2_LEXEMA_SHL_ASSIGN,     /* <<= */
    ARBOR2_LEXEMA_SHR_ASSIGN,     /* >>= */

    /* Increment/Decrement */
    ARBOR2_LEXEMA_DUPLUS,         /* ++ */
    ARBOR2_LEXEMA_DUMINUS,        /* -- */

    /* Member Access */
    ARBOR2_LEXEMA_PUNCTUM,        /* . */
    ARBOR2_LEXEMA_SAGITTA,        /* -> */

    /* Punctuation */
    ARBOR2_LEXEMA_COMMA,          /* , */
    ARBOR2_LEXEMA_COLON,          /* : */
    ARBOR2_LEXEMA_SEMICOLON,      /* ; */
    ARBOR2_LEXEMA_QUAESTIO,       /* ? */

    /* Delimiters */
    ARBOR2_LEXEMA_PAREN_APERTA,   /* ( */
    ARBOR2_LEXEMA_PAREN_CLAUSA,   /* ) */
    ARBOR2_LEXEMA_BRACKET_APERTA, /* [ */
    ARBOR2_LEXEMA_BRACKET_CLAUSA, /* ] */
    ARBOR2_LEXEMA_BRACE_APERTA,   /* { */
    ARBOR2_LEXEMA_BRACE_CLAUSA,   /* } */

    /* Preprocessor */
    ARBOR2_LEXEMA_HASH,           /* # */
    ARBOR2_LEXEMA_HASH_HASH,      /* ## */

    /* Ellipsis */
    ARBOR2_LEXEMA_ELLIPSIS,       /* ... */

    /* NEW: Explicit newline token */
    ARBOR2_LEXEMA_NOVA_LINEA,     /* \n or \r\n */

    /* Special */
    ARBOR2_LEXEMA_EOF,
    ARBOR2_LEXEMA_ERROR,

    /* Pseudo-type for keyword macro table */
    ARBOR2_LEXEMA_TYPEDEF_MACRO    /* Macro expands to a type */
} Arbor2LexemaGenus;

/* ==================================================
 * Trivia (whitespace and comments)
 * ================================================== */

nomen structura {
    chorda      valor;           /* Actual text content */
    b32         est_commentum;   /* true if comment, false if whitespace */
    b32         est_c99;         /* true if // comment (C99) */
    b32         est_continuatio; /* NEW: true if contains \\\n continuation */
    i32         linea;           /* Starting line (1-based) */
    i32         columna;         /* Starting column (1-based) */
} Arbor2Trivia;

/* ==================================================
 * Token
 * ================================================== */

nomen structura {
    Arbor2LexemaGenus genus;         /* Token type */
    chorda            valor;         /* Text of token */
    i32               byte_offset;   /* 0-based offset in source */
    i32               longitudo;     /* Length in bytes */
    i32               linea;         /* Starting line (1-based) */
    i32               columna;       /* Starting column (1-based) */
    Xar*              trivia_ante;   /* Leading trivia (Xar of Arbor2Trivia) */
    Xar*              trivia_post;   /* Trailing trivia (Xar of Arbor2Trivia) */
    i8                standard;      /* ARBOR2_STANDARD_* */
} Arbor2Lexema;

/* ==================================================
 * Lexer State (opaque)
 * ================================================== */

nomen structura Arbor2Lexator Arbor2Lexator;

/* ==================================================
 * API
 * ================================================== */

/* Creare lexator ex fonte */
Arbor2Lexator* arbor2_lexator_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura);

/* Obtinere proximum lexema */
Arbor2Lexema* arbor2_lexema_proximum(Arbor2Lexator* lex);

/* Obtinere omnia lexemata */
Xar* arbor2_lexema_omnia(Arbor2Lexator* lex);

/* Obtinere nomen genus (pro debugging) */
constans character* arbor2_lexema_genus_nomen(Arbor2LexemaGenus genus);

#endif /* ARBOR2_LEXEMA_H */
