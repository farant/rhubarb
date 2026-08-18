#ifndef HTML_LEXEMA_H
#define HTML_LEXEMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* ==================================================
 * HTML LEXEMA - lexator totalis pro HTML
 *
 * Bibliotheca secunda trium formarum interretis (vide
 * project-specs/css-html-js-spec.md). Dimidium tractabile
 * lexatoris HTML5, contractu silvae:
 *
 *  - TOTALIS: quaelibet series octetorum lexatur; nihil
 *    umquam perditur, nihil frangit. Trunca genera
 *    IMPERFECTA nominata accipiunt; anomala genus PRAVUM.
 *  - FIDELITAS OCTETORUM: emittere(lexare(x)) == x pro
 *    QUOLIBET x. Valores in fontem spectant (fons non
 *    copiatur - vivat quamdiu lexemata vivunt); emissio
 *    est concatenatio.
 *  - POSITIONES IN OCTETIS: offset + linea/columna
 *    1-basatae (decretum columnarum 01M0ATF1E1).
 *  - EXTENSIONES VERBATIM: casus litterarum titulorum
 *    notatur, non normalizatur; referentiae characterum
 *    non decoquuntur; valores cocti = cura gradus arboris
 *    (html_arbor, futurus).
 *
 * HTML statum fert quod CSS non fert: post '<div' modus
 * intra-tag est (attributa), post '<script...>' modus
 * crudus (textus ad '</script' solum). Lexator modum
 * trans lexemata servat; elementa cruda: script, style
 * (RAWTEXT), title, textarea (RCDATA - hic idem lexatum,
 * distinctio decoctionis = arbor).
 *
 * Deviationes a spec, omnes pro fidelitate aut clamore:
 *  - referentiae characterum lexemata propria fiunt SOLUM
 *    forma stricta '&...;' in modo datorum (in valoribus
 *    attributorum et RCDATA verbatim manent);
 *  - '=' intra tag semper AEQUALE (spec nonnumquam titulo
 *    attributi iungit);
 *  - '<script/>' modum crudum intrat (mos navigatorum:
 *    signum se-claudens pro script ignoratur);
 *  - doctype = lexema unum verbatim ad '>' primum (quod
 *    spec etiam intra identificatores citatos facit);
 *  - CDATA ubique agnoscitur (spec: solum in contentu
 *    alieno; lexator arborem nescit - genus nominatum
 *    clamans praestat commentario pravo mutilanti);
 *  - valor citatus non terminatus ad EOF -> IMPERFECTUS
 *    (lineae novae intra valores citatos LICITAE sunt -
 *    solus EOF truncat);
 *  - tag ad EOF apertum ('<div cl' + EOF) lexemata singula
 *    integra dat sine TAG_FINIS - truncatio structurae, non
 *    lexematis (arbor eam videt).
 * ================================================== */

nomen enumeratio {
    HTML_LEX_FINIS = 0,

    /* flumen datorum */
    HTML_LEX_TEXTUS,                 /* cursus datorum inter tags */
    HTML_LEX_REFERENTIA,             /* &amp;  &#65;  &#x41; */
    HTML_LEX_TEXTUS_CRUDUS,          /* contentum script/style/
                                        title/textarea */
    HTML_LEX_TEXTUS_CRUDUS_IMPERFECTUS, /* ad EOF sine claudente */

    /* tags */
    HTML_LEX_TAG_APERTURA,           /* <div  (angulus + titulus) */
    HTML_LEX_TAG_CLAUSURA,           /* </div */
    HTML_LEX_TAG_FINIS,              /* > */
    HTML_LEX_TAG_FINIS_SOLUS,        /* /> */

    /* attributa (intra tag) */
    HTML_LEX_ATTRIBUTUM_NOMEN,
    HTML_LEX_AEQUALE,                /* = */
    HTML_LEX_ATTRIBUTUM_VALOR,       /* "x" 'x' x (citatio in
                                        extensione visibilis) */
    HTML_LEX_ATTRIBUTUM_VALOR_IMPERFECTUS, /* citatus ad EOF */

    /* structurae speciales */
    HTML_LEX_COMMENTARIUM,           /* <!-- ... --> */
    HTML_LEX_COMMENTARIUM_IMPERFECTUM,
    HTML_LEX_COMMENTARIUM_PRAVUM,    /* <!x <?x </3 ... ad '>' */
    HTML_LEX_DOCTYPE,                /* <!doctype ... > */
    HTML_LEX_DOCTYPE_IMPERFECTUM,
    HTML_LEX_CDATA,                  /* <![CDATA[ ... ]]> */
    HTML_LEX_CDATA_IMPERFECTA,

    /* trivia intra tag */
    HTML_LEX_SPATIA,
    HTML_LEX_DELIM,                  /* '/' solivagus intra tag */

    HTML_LEX_NUMERUS_GENERUM         /* comes, semper ultimus */
} HtmlLexemaGenus;

nomen structura {
    HtmlLexemaGenus genus;
    chorda          valor;    /* in fontem spectat - verbatim */
    i32             offset;   /* octeti ab initio fontis */
    i32             linea;    /* 1-basata */
    i32             columna;  /* 1-basata, octeti */
} HtmlLexema;

/* Lexare fontem: Xar de HtmlLexema (per valorem), lexema FINIS
 * ultimum (valor vacuus). Numquam NIHIL nisi memoria deficit.
 * Fons non copiatur. */
Xar*
html_lexare (
    Piscina*            piscina,
    constans character* fons,
    i32                 mensura);

/* Oraculum fidelitatis: valores concatenati == fons */
chorda
html_lexemata_emittere (
    Piscina* piscina,
    Xar*     lexemata);

/* Titulus generis (pro debugging et probationibus) */
constans character*
html_lexema_genus_nomen (
    HtmlLexemaGenus genus);

#endif /* HTML_LEXEMA_H */
