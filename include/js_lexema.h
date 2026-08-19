#ifndef JS_LEXEMA_H
#define JS_LEXEMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"


/* ==================================================
 * JS LEXEMA - lexator totalis pro JavaScript
 *
 * Bibliotheca tertia trium formarum interretis (vide
 * project-specs/css-html-js-spec.md). Scopus: DIALECTUS
 * DOMUS - ES5 + quae domus scribit (class, functiones
 * sagittatae, litterae formularum, let/const); syntaxis
 * serior nihilominus LEXATUR (lexemata fere sine versione
 * sunt - incrementum = arbor, non lexator novus).
 * Contractus silvae:
 *
 *  - TOTALIS: quaelibet series octetorum lexatur; nihil
 *    umquam perditur, nihil frangit. Trunca genera
 *    IMPERFECTA nominata accipiunt; octeti intractabiles
 *    IGNOTUM.
 *  - FIDELITAS OCTETORUM: emittere(lexare(x)) == x pro
 *    QUOLIBET x. Valores in fontem spectant (fons non
 *    copiatur); emissio est concatenatio.
 *  - POSITIONES IN OCTETIS: offset + linea/columna
 *    1-basatae (decretum columnarum 01M0ATF1E1).
 *  - EXTENSIONES VERBATIM: effugia non decoquuntur;
 *    valores cocti = cura gradus arboris (js_arbor,
 *    futurus). ASI cura arboris est - lexator lineas novas
 *    in extensionibus spatiorum verbatim servat, arbor eas
 *    inspicit.
 *
 * PERICULA POSSESSA (a die primo):
 *  - REGEX contra DIVISIONEM: '/' post lexema significans
 *    prius iudicatur (heuristice classica). SIMPLIFICATIO
 *    DOMUS: post ')' et ']' divisio semper (etiam post
 *    condicionem 'if (x) /re/' - domus id non scribit);
 *    post '}' regex semper (finis clausulae praesumptus);
 *    post verba this/true/false/null/super divisio, post
 *    verba cetera regex. Arbor serior reiudicare potest.
 *  - FORMULAE (litterae formularum): secantur in TOTA
 *    (`x`), INITIUM (`x${), MEDIA (}x${), FINIS (}x`) -
 *    acervus interpolationum '{' numerat, ut '}' resumens
 *    a '}' interpunctionis distinguatur. Profunditas
 *    tecta LXIV: ultra eam '${' pars formulae manet
 *    (degradatio totalis, numquam fractura). Formulae
 *    trans lineas LICITAE (lexema multilineare).
 *
 * Deviationes et fines minores:
 *  - interpunctio genus UNUM fert (munch maximus, >>>= ad
 *    octetos IV): consumens extensionem inspicit; arbor
 *    tabulam propriam habebit - genera XLV hodie divinare
 *    APIs cras claudicaret;
 *  - verba (VERBUM) = reservata ES5 + let/const/class/
 *    extends/super/yield/static + this/true/false/null
 *    (consilium colorum ea unum tractat; arbor distinguat);
 *  - effugia \u in identificatoribus non intelleguntur
 *    (domus ea non scribit; '\' IGNOTUM fit - clamat);
 *  - '//' ad lineam novam EXCLUSIVE (linea spatiis manet);
 *    '//' ad EOF commentarium PERFECTUM est;
 *  - chorda/regex linea nova cruda -> IMPERFECTA, linea
 *    NON consumpta (mos css_lexema);
 *  - '<!--'/'-->' faex scriptorum interretis veterum non
 *    agnoscitur (interpunctio fit).
 * ================================================== */

nomen enumeratio {
    JS_LEX_FINIS = 0,

    /* nomina */
    JS_LEX_IDENTIFICATOR,
    JS_LEX_VERBUM,                /* reservata + this/true/false/null */

    /* litterae */
    JS_LEX_CHORDA,                /* '...' "..." (continuatio \-linea) */
    JS_LEX_CHORDA_IMPERFECTA,     /* linea nova cruda aut EOF intus */
    JS_LEX_NUMERUS,               /* 12 1.5 .5 1e3 0x1F 0b101 0o17 */
    JS_LEX_REGEX,                 /* /ab[/]c/gi totum cum vexillis */
    JS_LEX_REGEX_IMPERFECTA,      /* linea nova aut EOF intus */

    /* formulae (litterae formularum) */
    JS_LEX_FORMULA_TOTA,          /* `salve` sine interpolatione */
    JS_LEX_FORMULA_INITIUM,       /* `a${ */
    JS_LEX_FORMULA_MEDIA,         /* }b${ */
    JS_LEX_FORMULA_FINIS,         /* }c` */
    JS_LEX_FORMULA_IMPERFECTA,    /* ad EOF intus */

    /* trivia (in flumine - fidelitas) */
    JS_LEX_SPATIA,
    JS_LEX_COMMENTARIUM,          /* '//...' et clausum block */
    JS_LEX_COMMENTARIUM_IMPERFECTUM, /* block apertum ad EOF */

    /* reliquum */
    JS_LEX_INTERPUNCTIO,          /* operatores omnes, munch maximus */
    JS_LEX_IGNOTUM,               /* octetus solus intractabilis */

    JS_LEX_NUMERUS_GENERUM        /* comes, semper ultimus */
} JsLexemaGenus;

nomen structura {
    JsLexemaGenus genus;
           chorda valor;    /* in fontem spectat - verbatim */
              i32 offset;   /* octeti ab initio fontis */
              i32 linea;    /* 1-basata */
              i32 columna;  /* 1-basata, octeti */
} JsLexema;

/* Lexare fontem: Xar de JsLexema (per valorem), lexema FINIS
 * ultimum (valor vacuus). Numquam NIHIL nisi memoria deficit.
 * Fons non copiatur. */
Xar*
js_lexare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

/* Oraculum fidelitatis: valores concatenati == fons */
chorda
js_lexemata_emittere (
    Piscina* piscina,
        Xar* lexemata);

/* Titulus generis (pro debugging et probationibus) */
constans character*
js_lexema_genus_nomen (
    JsLexemaGenus genus);

#endif /* JS_LEXEMA_H */
