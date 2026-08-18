#ifndef CSS_LEXEMA_H
#define CSS_LEXEMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* ==================================================
 * CSS LEXEMA - lexator totalis pro CSS
 *
 * Bibliotheca prima trium formarum interretis (vide
 * project-specs/css-html-js-spec.md). Vocabularium
 * CSS Syntax Module Level 3, contractu silvae:
 *
 *  - TOTALIS: quaelibet series octetorum lexatur; nihil
 *    umquam perditur, nihil frangit. Trunca genera
 *    IMPERFECTA nominata accipiunt.
 *  - FIDELITAS OCTETORUM: emittere(lexare(x)) == x pro
 *    QUOLIBET x. Valores in fontem spectant (fons non
 *    copiatur - vivat quamdiu lexemata vivunt); emissio
 *    est concatenatio.
 *  - POSITIONES IN OCTETIS: offset + linea/columna
 *    1-basatae (decretum columnarum 01M0ATF1E1).
 *  - TRIVIA SUNT LEXEMATA: spatia et commentaria in
 *    flumine emittuntur (spec commentaria devorat - nos
 *    fidelitati et stratui quaestionum commentariorum
 *    servimus; consumens spec-conformis ea cribrat).
 *  - EXTENSIONES VERBATIM: effugia non decoquuntur -
 *    satis intelleguntur ut fines lexematum inveniantur;
 *    valores cocti = cura gradus arboris (css_arbor,
 *    futurus).
 *
 * Deviationes a spec, omnes pro fidelitate aut clamore:
 *  - praeprocessio nulla (\r\n, \f, NUL verbatim manent;
 *    \r et \f spatia directe sunt, NUL delim fit);
 *  - chorda/url non terminatae ad EOF -> IMPERFECTA
 *    (spec lexema validum cum errore reddit - malumus
 *    clamorem);
 *  - octeti >= 0x80 characteres nominis sunt (UTF-8:
 *    omnes octeti continuationis >= 0x80).
 * ================================================== */

nomen enumeratio {
    CSS_LEX_FINIS = 0,

    /* nomina */
    CSS_LEX_IDENTIFICATOR,           /* color, -webkit-x, --varia */
    CSS_LEX_FUNCTIO,                 /* var(  rgba(  (cum parenthesi) */
    CSS_LEX_APUD_VERBUM,             /* @media @import */
    CSS_LEX_CANCELLUM,               /* #fff #titulus */

    /* litterae */
    CSS_LEX_CHORDA,                  /* "..." '...' */
    CSS_LEX_CHORDA_IMPERFECTA,       /* linea nova aut EOF intus */
    CSS_LEX_URL,                     /* url(imago.png) totum */
    CSS_LEX_URL_IMPERFECTA,          /* url mala aut EOF intus */
    CSS_LEX_NUMERUS,                 /* 12  1.5  -3e2 */
    CSS_LEX_CENTESIMA,               /* 50% */
    CSS_LEX_DIMENSIO,                /* 12px  1.5em */

    /* trivia (in flumine - fidelitas) */
    CSS_LEX_SPATIA,                  /* spatia, tabulae, lineae novae */
    CSS_LEX_COMMENTARIUM,            /* comment clausum */
    CSS_LEX_COMMENTARIUM_IMPERFECTUM,/* comment apertum ad EOF */

    /* interpunctio */
    CSS_LEX_CDO,                     /* <!-- */
    CSS_LEX_CDC,                     /* --> */
    CSS_LEX_COLON,
    CSS_LEX_SEMICOLON,
    CSS_LEX_COMMA,
    CSS_LEX_QUADRA_APERTA,           /* [ */
    CSS_LEX_QUADRA_CLAUSA,           /* ] */
    CSS_LEX_PAREN_APERTA,            /* ( */
    CSS_LEX_PAREN_CLAUSA,            /* ) */
    CSS_LEX_BRACE_APERTA,            /* { */
    CSS_LEX_BRACE_CLAUSA,            /* } */

    /* reliquum - octetus unus quilibet (NUL inclusus) */
    CSS_LEX_DELIM,

    CSS_LEX_NUMERUS_GENERUM          /* comes, semper ultimus */
} CssLexemaGenus;

nomen structura {
    CssLexemaGenus genus;
    chorda         valor;    /* in fontem spectat - verbatim */
    i32            offset;   /* octeti ab initio fontis */
    i32            linea;    /* 1-basata */
    i32            columna;  /* 1-basata, octeti */
} CssLexema;

/* Lexare fontem: Xar de CssLexema (per valorem), lexema FINIS
 * ultimum (valor vacuus). Numquam NIHIL nisi memoria deficit.
 * Fons non copiatur. */
Xar*
css_lexare (
    Piscina*            piscina,
    constans character* fons,
    i32                 mensura);

/* Oraculum fidelitatis: valores concatenati == fons */
chorda
css_lexemata_emittere (
    Piscina* piscina,
    Xar*     lexemata);

/* Titulus generis (pro debugging et probationibus) */
constans character*
css_lexema_genus_nomen (
    CssLexemaGenus genus);

#endif /* CSS_LEXEMA_H */
