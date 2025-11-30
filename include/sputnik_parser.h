#ifndef SPUTNIK_PARSER_H
#define SPUTNIK_PARSER_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "sputnik_lexema.h"

/* ==================================================
 * SPUTNIK PARSER - Parser pro Sputnik
 *
 * Parser pro lingua Sputnik.
 * Producit AST ex lexematis.
 *
 * PHILOSOPHIA:
 * - Pratt parser pro expressionibus (praecedentia operatorum)
 * - Recursive descent pro sententiis
 * - Xar* liberi pro uniformitate infantium
 * - Omnia ex piscina allocata
 *
 * EXEMPLUM:
 *   SputnikParserResultus r = sputnik_parsere_ex_fonte(
 *       chorda_ex_constante("let x = 42;"),
 *       piscina, intern);
 *   si (r.successus) {
 *       sputnik_ast_imprimere(r.radix, ZEPHYRUM);
 *   }
 *
 * ================================================== */


/* ==================================================
 * SputnikAstGenus - AST Node Types
 * ================================================== */

nomen enumeratio {
    /* Expressiones - Literalia */
    SPUTNIK_AST_NUMERUS_LITERALIS    = I,
    SPUTNIK_AST_CHORDA_LITERALIS     = II,
    SPUTNIK_AST_VERUM_LITERALIS      = III,
    SPUTNIK_AST_FALSUM_LITERALIS     = IV,
    SPUTNIK_AST_NIHIL_LITERALIS      = V,
    SPUTNIK_AST_IDENTIFICATOR        = VI,
    SPUTNIK_AST_SIGNUM               = VII,       /* #tag */

    /* Expressiones - Operationes */
    SPUTNIK_AST_OPERATIO_BINARIA     = VIII,      /* a + b */
    SPUTNIK_AST_OPERATIO_UNARIA      = IX,        /* -x, !x */
    SPUTNIK_AST_TERNARIA             = X,         /* a ? b : c */
    SPUTNIK_AST_VOCATIO              = XI,        /* foo(a, b) */
    SPUTNIK_AST_ACCESSUS_MEMBRI      = XII,       /* obj.prop */
    SPUTNIK_AST_ACCESSUS_INDICE      = XIII,      /* arr[i] */
    SPUTNIK_AST_ASSIGNATIO           = XIV,       /* x = y */

    /* Literalia Composita */
    SPUTNIK_AST_XAR_LITERALIS        = XV,        /* [1, 2, 3] */
    SPUTNIK_AST_OBJECTUM_LITERALIS   = XVI,       /* { a: 1, b: 2 } */

    /* Sententiae (Statements) */
    SPUTNIK_AST_DECLARATIO_LET       = XVII,
    SPUTNIK_AST_DECLARATIO_CONST     = XVIII,
    SPUTNIK_AST_SENTENTIA_SI         = XIX,       /* if/else */
    SPUTNIK_AST_SENTENTIA_DUM        = XX,        /* while */
    SPUTNIK_AST_SENTENTIA_PER        = XXI,       /* for */
    SPUTNIK_AST_SENTENTIA_REDDE      = XXII,      /* return */
    SPUTNIK_AST_SENTENTIA_EXPRESSIO  = XXIII,     /* expression; */
    SPUTNIK_AST_SENTENTIA_GREX       = XXIV,      /* { ... } block */

    /* Declarationes */
    SPUTNIK_AST_DECLARATIO_FUNCTIO   = XXV,       /* function foo() {} */
    SPUTNIK_AST_PROGRAMMA            = XXVI,      /* root node */

    /* Control Flow Additi */
    SPUTNIK_AST_SENTENTIA_FRANGE     = XXVII,     /* break */
    SPUTNIK_AST_SENTENTIA_PERGE      = XXVIII,    /* continue */

    /* Operationes Additi */
    SPUTNIK_AST_SENTENTIA_INCREMENT  = XXIX,      /* i++ */
    SPUTNIK_AST_SENTENTIA_DECREMENT  = XXX,       /* i-- */

    /* Functio Sagitta (Arrow Function) */
    SPUTNIK_AST_FUNCTIO_SAGITTA      = XXXI,      /* x => x * 2 */

    /* Declaratio Entitas */
    SPUTNIK_AST_DECLARATIO_ENTITAS   = XXXII      /* entitas x : Genus::Path; */
} SputnikAstGenus;


/* ==================================================
 * SputnikAstNodus - AST Node
 *
 * Structura uniformis pro omnibus nodis.
 * liberi continet infantes secundum genus:
 *
 * BINARIA:        [sinister, dexter]
 * UNARIA:         [operandus]
 * TERNARIA:       [conditio, verum, falsum]
 * VOCATIO:        [callee, arg0, arg1, ...]
 * ACCESSUS_MEMBRI:[objectum] (valor = nomen membri)
 * ACCESSUS_INDICE:[objectum, index]
 * ASSIGNATIO:     [target, valor]
 * XAR_LITERALIS:  [elem0, elem1, ...]
 * OBJECTUM:       [key0, val0, key1, val1, ...]
 * LET/CONST:      [valor] vel [] (valor = nomen)
 * SI:             [conditio, tunc, alioquin?]
 * DUM:            [conditio, corpus]
 * PER:            [init, conditio, incrementum, corpus]
 * REDDE:          [valor?]
 * GREX:           [sent0, sent1, ...]
 * FUNCTIO:        [param0, param1, ..., corpus] (valor = nomen)
 * PROGRAMMA:      [sent0, sent1, ...]
 * ================================================== */

nomen structura SputnikAstNodus SputnikAstNodus;

structura SputnikAstNodus {
    SputnikAstGenus      genus;
    chorda               valor;           /* Identificator, chorda literalis, etc. */
    SputnikLexemaGenus   operator;        /* Pro operationibus binariis/unariis */
    f64                  numerus;         /* Pro numeris literalibus */
    Xar*                 liberi;          /* Xar de SputnikAstNodus* */
    i32                  linea;
    i32                  columna;
};


/* ==================================================
 * SputnikParser - Parser State
 * ================================================== */

nomen structura {
    Xar*                 lexemata;        /* Lexemata input */
    i32                  positus;         /* Positio currens */
    Piscina*             piscina;
    InternamentumChorda* intern;
    b32                  error_accidit;
    chorda               error_nuntius;
    i32                  error_linea;
    i32                  error_columna;
} SputnikParser;


/* ==================================================
 * SputnikParserResultus - Parse Result
 * ================================================== */

nomen structura {
    b32              successus;
    SputnikAstNodus* radix;               /* Radix PROGRAMMA nodus */
    chorda           error_nuntius;
    i32              error_linea;
    i32              error_columna;
} SputnikParserResultus;


/* ==================================================
 * Functiones Principales
 * ================================================== */

/* Parsere programma ex lexematis
 *
 * lexemata: Xar de SputnikLexema (ex sputnik_lexicare)
 * piscina: Piscina pro allocationibus
 * intern: Internamentum pro chordis
 *
 * Redde: SputnikParserResultus
 */
SputnikParserResultus
sputnik_parsere(
    Xar*                 lexemata,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Parsere programma ex fonte chorda (commoditas)
 *
 * Lexicat et parset fontem in uno passu.
 */
SputnikParserResultus
sputnik_parsere_ex_fonte(
    chorda               fons,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Parsere ex C string */
SputnikParserResultus
sputnik_parsere_ex_literis(
    constans character*  fons,
    Piscina*             piscina,
    InternamentumChorda* intern);


/* ==================================================
 * Utilitas
 * ================================================== */

/* Obtinere nomen generis AST pro depuratione */
constans character*
sputnik_ast_genus_nomen(
    SputnikAstGenus genus);

/* Imprimere AST pro depuratione */
vacuum
sputnik_ast_imprimere(
    SputnikAstNodus* nodus,
    i32              indentatio);

#endif /* SPUTNIK_PARSER_H */
