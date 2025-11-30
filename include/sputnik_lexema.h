#ifndef SPUTNIK_LEXEMA_H
#define SPUTNIK_LEXEMA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"

/* ==================================================
 * SPUTNIK LEXEMA - Lexer pro Sputnik
 *
 * Tokenizator pro lingua Sputnik.
 * Producit lexemata ex fonte textu.
 *
 * PHILOSOPHIA:
 * - Omnia identificatores kebab-case permittuntur
 * - x-1 est identificator, non subtractio
 * - Commenta praservantur ut lexemata
 * - Template strings differuntur
 *
 * EXEMPLUM:
 *   SputnikLexator* lex = sputnik_lexator_creare(fons, piscina, intern);
 *   SputnikLexema lexema = sputnik_lexator_legere(lex);
 *   dum (lexema.genus != SPUTNIK_LEXEMA_FINIS) {
 *       sputnik_lexema_imprimere(&lexema);
 *       lexema = sputnik_lexator_legere(lex);
 *   }
 *
 * ================================================== */


/* ==================================================
 * SputnikLexemaGenus - Token Types
 * ================================================== */

nomen enumeratio {
    /* Finis et Error */
    SPUTNIK_LEXEMA_FINIS           = ZEPHYRUM,
    SPUTNIK_LEXEMA_ERROR           = I,

    /* Literalia */
    SPUTNIK_LEXEMA_NUMERUS         = II,        /* 42, 1_000, 3.14 */
    SPUTNIK_LEXEMA_CHORDA          = III,       /* "string" vel 'string' */
    SPUTNIK_LEXEMA_VERUM           = IV,        /* true */
    SPUTNIK_LEXEMA_FALSUM          = V,         /* false */
    SPUTNIK_LEXEMA_NIHIL           = VI,        /* null */

    /* Identificatores et Signa */
    SPUTNIK_LEXEMA_IDENTIFICATOR   = VII,       /* foo, foo-bar, x-1 */
    SPUTNIK_LEXEMA_SIGNUM          = VIII,      /* #tag, #Namespace::tag */

    /* Verba Clausa (Keywords) */
    SPUTNIK_LEXEMA_LET             = IX,
    SPUTNIK_LEXEMA_CONST           = X,
    SPUTNIK_LEXEMA_SI              = XI,        /* if */
    SPUTNIK_LEXEMA_ALIOQUIN        = XII,       /* else */
    SPUTNIK_LEXEMA_DUM             = XIII,      /* while */
    SPUTNIK_LEXEMA_PER             = XIV,       /* for */
    SPUTNIK_LEXEMA_REDDE           = XV,        /* return */
    SPUTNIK_LEXEMA_FUNCTIO         = XVI,       /* function */

    /* Operatores Arithmetici */
    SPUTNIK_LEXEMA_PLUS            = XVII,      /* + */
    SPUTNIK_LEXEMA_MINUS           = XVIII,     /* - */
    SPUTNIK_LEXEMA_ASTERISCUS      = XIX,       /* * */
    SPUTNIK_LEXEMA_DIVISIO         = XX,        /* / */
    SPUTNIK_LEXEMA_MODULUS         = XXI,       /* % */

    /* Operatores Comparationis */
    SPUTNIK_LEXEMA_AEQUALIS        = XXII,      /* = */
    SPUTNIK_LEXEMA_AEQUALIS_DUO    = XXIII,     /* == */
    SPUTNIK_LEXEMA_AEQUALIS_TRIA   = XXIV,      /* === */
    SPUTNIK_LEXEMA_NON_AEQUALIS    = XXV,       /* != */
    SPUTNIK_LEXEMA_STRICTE_NON_AEQ = XXVI,      /* !== */
    SPUTNIK_LEXEMA_MINOR           = XXVII,     /* < */
    SPUTNIK_LEXEMA_MAIOR           = XXVIII,    /* > */
    SPUTNIK_LEXEMA_MINOR_AUT       = XXIX,      /* <= */
    SPUTNIK_LEXEMA_MAIOR_AUT       = XXX,       /* >= */

    /* Operatores Logici */
    SPUTNIK_LEXEMA_ET              = XXXI,      /* && */
    SPUTNIK_LEXEMA_VEL             = XXXII,     /* || */
    SPUTNIK_LEXEMA_NON             = XXXIII,    /* ! */

    /* Operatores Speciales */
    SPUTNIK_LEXEMA_SAGITTA         = XXXIV,     /* => */
    SPUTNIK_LEXEMA_PUNCTUM_TRIPLEX = XXXV,      /* ... */

    /* Punctuatio */
    SPUTNIK_LEXEMA_PARENTHESIS_A   = XXXVI,     /* ( */
    SPUTNIK_LEXEMA_PARENTHESIS_C   = XXXVII,    /* ) */
    SPUTNIK_LEXEMA_BRACCHIUM_A     = XXXVIII,   /* { */
    SPUTNIK_LEXEMA_BRACCHIUM_C     = XXXIX,     /* } */
    SPUTNIK_LEXEMA_QUADRATUM_A     = XL,        /* [ */
    SPUTNIK_LEXEMA_QUADRATUM_C     = XLI,       /* ] */
    SPUTNIK_LEXEMA_SEMICOLON       = XLII,      /* ; */
    SPUTNIK_LEXEMA_COMMA           = XLIII,     /* , */
    SPUTNIK_LEXEMA_PUNCTUM         = XLIV,      /* . */
    SPUTNIK_LEXEMA_COLON           = XLV,       /* : */

    SPUTNIK_LEXEMA_INTERROGATIO    = XLVI,      /* ? */

    /* Commenta */
    SPUTNIK_LEXEMA_COMMENTUM       = XLVII      /* // vel block */
} SputnikLexemaGenus;


/* ==================================================
 * SputnikLexema - Token
 * ================================================== */

nomen structura {
    SputnikLexemaGenus  genus;
    chorda              valor;          /* Vista in fontem */
    i32                 linea;          /* 1-based */
    i32                 columna;        /* 1-based */
    i32                 offset_initium;
    i32                 offset_finis;
} SputnikLexema;


/* ==================================================
 * SputnikLexator - Lexer State
 * ================================================== */

nomen structura {
    chorda               fons;
    i32                  positus;
    i32                  linea;
    i32                  columna;
    Piscina*             piscina;
    InternamentumChorda* intern;
} SputnikLexator;


/* ==================================================
 * SputnikLexatorResultus - Full Tokenization Result
 * ================================================== */

nomen structura {
    b32                  successus;
    Xar*                 lexemata;      /* Xar de SputnikLexema */
    chorda               error_nuntius;
    i32                  error_linea;
    i32                  error_columna;
} SputnikLexatorResultus;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare lexatorem ex chorda
 *
 * fons: Fons textus
 * piscina: Piscina pro allocationibus
 * intern: Internamentum pro chordis
 *
 * Redde: SputnikLexator* si successus, NIHIL si fractura
 */
SputnikLexator*
sputnik_lexator_creare(
    chorda               fons,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Creare lexatorem ex C string */
SputnikLexator*
sputnik_lexator_creare_ex_literis(
    constans character*  fons,
    Piscina*             piscina,
    InternamentumChorda* intern);


/* ==================================================
 * Lexicatio Singularis
 * ================================================== */

/* Legere proximum lexema
 *
 * Redde: SputnikLexema
 * Nota: genus == SPUTNIK_LEXEMA_FINIS indicat finem
 *       genus == SPUTNIK_LEXEMA_ERROR indicat errorem
 */
SputnikLexema
sputnik_lexator_legere(
    SputnikLexator* lexator);

/* Aspicere proximum lexema sine consumptione */
SputnikLexema
sputnik_lexator_aspicere(
    SputnikLexator* lexator);

/* Verificare si ad finem */
b32
sputnik_lexator_finis(
    SputnikLexator* lexator);


/* ==================================================
 * Lexicatio Totalis
 * ================================================== */

/* Lexicare totum fontem
 *
 * Redde: SputnikLexatorResultus cum omnibus lexematis
 */
SputnikLexatorResultus
sputnik_lexicare(
    chorda               fons,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Lexicare ex C string */
SputnikLexatorResultus
sputnik_lexicare_ex_literis(
    constans character*  fons,
    Piscina*             piscina,
    InternamentumChorda* intern);


/* ==================================================
 * Utilitas
 * ================================================== */

/* Obtinere nomen generis pro depuratione */
constans character*
sputnik_lexema_genus_nomen(
    SputnikLexemaGenus genus);

/* Imprimere lexema pro depuratione */
vacuum
sputnik_lexema_imprimere(
    SputnikLexema* lexema);

/* Imprimere omnia lexemata */
vacuum
sputnik_lexemata_imprimere(
    Xar* lexemata);

#endif /* SPUTNIK_LEXEMA_H */
