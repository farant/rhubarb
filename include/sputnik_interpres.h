#ifndef SPUTNIK_INTERPRES_H
#define SPUTNIK_INTERPRES_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "sputnik_parser.h"
#include "entitas.h"
#include "entitas_repositorium.h"

/* ==================================================
 * SPUTNIK INTERPRES - Interpres pro Sputnik
 *
 * Tree-walking evaluator pro AST.
 * Tertius modulus in pipeline: Lexer -> Parser -> Interpres
 *
 * PHILOSOPHIA:
 * - Halt-immediate error handling
 * - Per-execution piscina (arena)
 * - Functiones non first-class (nulla closure escape)
 * - Tagged union pro valoribus
 * - Scope chain cum TabulaDispersa
 *
 * EXEMPLUM:
 *   SputnikInterpresResultus r = sputnik_evaluare(
 *       "let x = 2 + 3; x * 2",
 *       piscina, intern);
 *   si (r.successus) {
 *       -- r.valor.ut.numerus == 10.0
 *   }
 *
 * ================================================== */


/* ==================================================
 * SputnikValorGenus - Runtime Value Types
 * ================================================== */

nomen enumeratio {
    SPUTNIK_VALOR_NIHIL                 = I,
    SPUTNIK_VALOR_NUMERUS               = II,      /* f64 */
    SPUTNIK_VALOR_CHORDA                = III,     /* chorda */
    SPUTNIK_VALOR_VERUM                 = IV,      /* boolean true */
    SPUTNIK_VALOR_FALSUM                = V,       /* boolean false */
    SPUTNIK_VALOR_XAR                   = VI,      /* Xar de SputnikValor* */
    SPUTNIK_VALOR_OBJECTUM              = VII,     /* TabulaDispersa */
    SPUTNIK_VALOR_FUNCTIO               = VIII,    /* function reference */
    SPUTNIK_VALOR_METHODUS_XAR          = IX,      /* array method reference */
    SPUTNIK_VALOR_ENTITAS               = X,       /* Entitas* - entity reference */
    SPUTNIK_VALOR_METHODUS_ENTITAS      = XI,      /* entity method reference */
    SPUTNIK_VALOR_REPOSITORIUM          = XII,     /* EntitasRepositorium* */
    SPUTNIK_VALOR_METHODUS_REPOSITORIUM = XIII     /* repository method reference */
} SputnikValorGenus;


/* ==================================================
 * SputnikMethodusXar - Array Method Reference
 *
 * Continet referentiam ad array et nomen methodi.
 * ================================================== */

nomen structura {
    Xar*   xar;       /* Array objectum */
    chorda titulus;   /* Nomen methodi (push, pop, etc.) */
} SputnikMethodusXar;


/* ==================================================
 * SputnikMethodusEntitas - Entity Method Reference
 *
 * Continet referentiam ad entitas et nomen methodi.
 * ================================================== */

nomen structura {
    Entitas* entitas;   /* Entity objectum */
    chorda   titulus;   /* Nomen methodi (add_relation, related, etc.) */
} SputnikMethodusEntitas;


/* ==================================================
 * SputnikMethodusRepositorium - Repository Method Reference
 *
 * Continet referentiam ad repositorium et nomen methodi.
 * ================================================== */

nomen structura {
    EntitasRepositorium* repositorium;  /* Repository objectum */
    chorda               titulus;       /* Nomen methodi (get, query, etc.) */
} SputnikMethodusRepositorium;


/* ==================================================
 * SputnikValor - Runtime Value
 *
 * Tagged union pro omnibus valoribus runtime.
 * ================================================== */

nomen structura SputnikValor SputnikValor;

structura SputnikValor {
    SputnikValorGenus genus;
    unio {
        f64                          numerus;
        chorda                       chorda_valor;
        Xar*                         xar;                  /* Xar de SputnikValor* */
        TabulaDispersa*              objectum;             /* chorda -> SputnikValor* */
        SputnikAstNodus*             functio;              /* DECLARATIO_FUNCTIO node */
        SputnikMethodusXar           methodus_xar;         /* array method reference */
        Entitas*                     entitas;              /* entity reference */
        SputnikMethodusEntitas       methodus_entitas;     /* entity method reference */
        EntitasRepositorium*         repositorium;         /* repository reference */
        SputnikMethodusRepositorium  methodus_repositorium; /* repo method reference */
    } ut;
};


/* ==================================================
 * SputnikAmbitus - Scope/Environment
 *
 * Linked list scoporum. Quisque ambitus habet
 * TabulaDispersa pro variabilibus.
 * ================================================== */

nomen structura SputnikAmbitus SputnikAmbitus;

structura SputnikAmbitus {
    TabulaDispersa*  variabiles;    /* chorda -> SputnikValor* */
    TabulaDispersa*  constantes;    /* chorda -> VERUM si constans */
    SputnikAmbitus*  parens;        /* parent scope (NIHIL pro globali) */
};


/* ==================================================
 * SputnikInterpres - Interpreter State
 * ================================================== */

nomen structura {
    Piscina*              piscina;
    InternamentumChorda*  intern;
    EntitasRepositorium*  repositorium;       /* entity storage */
    SputnikAmbitus*       ambitus_globalis;
    SputnikAmbitus*       ambitus_currens;
    b32                   error_accidit;
    chorda                error_nuntius;
    i32                   error_linea;
    i32                   error_columna;
    b32                   redde_activa;       /* return statement hit */
    SputnikValor          redde_valor;        /* return value */
    b32                   frange_activa;      /* break statement hit */
    b32                   perge_activa;       /* continue statement hit */
} SputnikInterpres;


/* ==================================================
 * SputnikInterpresResultus - Evaluation Result
 * ================================================== */

nomen structura {
    b32           successus;
    SputnikValor  valor;
    chorda        error_nuntius;
    i32           error_linea;
    i32           error_columna;
} SputnikInterpresResultus;


/* ==================================================
 * Functiones Principales
 * ================================================== */

/* Evaluare programma ex C string - primary API
 *
 * fons: C string cum fonte Sputnik
 * piscina: Piscina pro allocationibus
 * intern: Internamentum pro chordis
 *
 * Redde: SputnikInterpresResultus
 */
SputnikInterpresResultus
sputnik_evaluare(
    constans character*  fons,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Evaluare ex chorda */
SputnikInterpresResultus
sputnik_evaluare_ex_chorda(
    chorda               fons,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Interpretare pre-parsed AST */
SputnikInterpresResultus
sputnik_interpretare(
    SputnikAstNodus*     radix,
    Piscina*             piscina,
    InternamentumChorda* intern);


/* ==================================================
 * API cum Repositorio (Entity-Aware)
 * ================================================== */

/* Evaluare programma cum repositorio entitatum
 *
 * fons: C string cum fonte Sputnik
 * piscina: Piscina pro allocationibus
 * intern: Internamentum pro chordis
 * repositorium: Repositorium entitatum (potest esse NIHIL)
 *
 * Redde: SputnikInterpresResultus
 */
SputnikInterpresResultus
sputnik_evaluare_cum_repositorio(
    constans character*   fons,
    Piscina*              piscina,
    InternamentumChorda*  intern,
    EntitasRepositorium*  repositorium);

/* Evaluare ex chorda cum repositorio */
SputnikInterpresResultus
sputnik_evaluare_ex_chorda_cum_repositorio(
    chorda                fons,
    Piscina*              piscina,
    InternamentumChorda*  intern,
    EntitasRepositorium*  repositorium);

/* Interpretare pre-parsed AST cum repositorio */
SputnikInterpresResultus
sputnik_interpretare_cum_repositorio(
    SputnikAstNodus*      radix,
    Piscina*              piscina,
    InternamentumChorda*  intern,
    EntitasRepositorium*  repositorium);


/* ==================================================
 * Utilitas
 * ================================================== */

/* Convertere valor ad chorda (pro print, debug, etc.) */
chorda
sputnik_valor_ad_chordam(
    SputnikValor*        valor,
    Piscina*             piscina);

/* Obtinere nomen generis valoris */
constans character*
sputnik_valor_genus_nomen(
    SputnikValorGenus genus);

#endif /* SPUTNIK_INTERPRES_H */
