/* xml_lexicon.h - Descriptor lexicalis xml pro materia
 *
 * MANU SCRIPTUS (exemplar html_lexicon.h, toml_lexicon.h). FONS
 * VERITATIS generum lexematum: enumeratio XmlLexGenus INFRA (lector
 * eam emittit, lexicon eam describit); probatio_xml_registrum ordinem
 * per titulos asserit.
 *
 * MODI LECTORIS (spec par. III, X5): contentum, tag, declaratio XML,
 * doctype, pars interna, declaratio DTD - lector modum ipse eligit
 * ('<' et '&' in contento, citationes intra tags), numquam
 * relabellatio post.
 *
 * FORMA EX HTML: '<titulus' (TAG_APERTURA) et '</titulus'
 * (TAG_CLAUSURA) lexemata singula - XML spatium inter '<' et titulum
 * vetat. Commentaria, instructiones, CDATA lexemata tota (coctio
 * interiora iudicat: '--' in commentario, scopus 'xml' reservatus).
 *
 * TRIVIA: SPATIA solum, intra tags et declarationes (S productionis
 * XML: spatium, TAB, LF, CR). Spatia contenti TEXTUS sunt. Munus
 * LINEA nullum: XML lineas logicas non habet (lector lineam et
 * columnam ipse numerat).
 *
 * TITULUS: Name aut Nmtoken - tituli attributorum, doctype,
 * declarationum, verba clavis (SYSTEM, PUBLIC, EMPTY, CDATA, NDATA
 * ...); xml_dtd significationem legit. VERBUM_PRAEDEFINITUM: '#PCDATA',
 * '#REQUIRED', '#IMPLIED', '#FIXED'. DELIM: signum unum declarationis
 * ( ) | , ? * + %.
 *
 * IGNOTUM: octetus (aut sequentia UTF-8 invalida, aut runa extra
 * Char) quem nulla regula accipit; aedificator in 'malum' ponit
 * (totalitas, X6).
 */

#ifndef XML_LEXICON_H
#define XML_LEXICON_H

#include "latina.h"
#include "materia_lexicon.h"

nomen enumeratio {
    XML_LEX_FINIS = 0,
    XML_LEX_SIGNUM_ORDINIS,

    /* contentum */
    XML_LEX_TEXTUS,
    XML_LEX_REFERENTIA,
    XML_LEX_CDATA,
    XML_LEX_COMMENTARIUM,
    XML_LEX_INSTRUCTIO,

    /* declaratio XML */
    XML_LEX_DECLARATIO_XML,
    XML_LEX_INSTRUCTIO_FINIS,

    /* tags */
    XML_LEX_TAG_APERTURA,
    XML_LEX_TAG_CLAUSURA,
    XML_LEX_TAG_FINIS,
    XML_LEX_TAG_FINIS_SOLUS,
    XML_LEX_TITULUS,
    XML_LEX_AEQUALE,
    XML_LEX_VALOR,

    /* doctype et pars interna */
    XML_LEX_DOCTYPE,
    XML_LEX_PARS_INTERNA_APERTURA,
    XML_LEX_PARS_INTERNA_CLAUSURA,
    XML_LEX_DECLARATIO,
    XML_LEX_REFERENTIA_PARAMETRI,
    XML_LEX_VERBUM_PRAEDEFINITUM,
    XML_LEX_DELIM,

    /* trivia et reliqua */
    XML_LEX_SPATIA,
    XML_LEX_IGNOTUM,

    XML_LEX_NUMERUS_GENERUM
} XmlLexGenus;

externus constans MateriaLexiconCoctum XML_LEXICON;

#endif /* XML_LEXICON_H */
