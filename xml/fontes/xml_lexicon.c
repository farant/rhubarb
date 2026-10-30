/* xml_lexicon.c - Descriptor lexicalis xml
 *
 * Vide xml_lexicon.h. Ordo ordinem XmlLexGenus sequitur;
 * probatio_xml_registrum id asserit per titulos.
 */

#include "xml_lexicon.h"

hic_manens constans MateriaLexGenus GENERA_XML[] = {
    /* titulus                  orthographia species  munus */
    { "FINIS",                  "",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },
    /* BOM UTF-8 */
    { "SIGNUM_ORDINIS",         "\357\273\277", MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* contentum */
    { "TEXTUS",                 NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "REFERENTIA",             NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CDATA",                  NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    /* commentarium contentum est (ut in html), non trivium */
    { "COMMENTARIUM",           NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "INSTRUCTIO",             NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* declaratio XML */
    { "DECLARATIO_XML",         "<?xml", MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "INSTRUCTIO_FINIS",       "?>",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* tags */
    { "TAG_APERTURA",           NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TAG_CLAUSURA",           NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TAG_FINIS",              ">",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TAG_FINIS_SOLUS",        "/>",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TITULUS",                NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "AEQUALE",                "=",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "VALOR",                  NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* doctype et pars interna */
    { "DOCTYPE",                "<!DOCTYPE", MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PARS_INTERNA_APERTURA",  "[",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PARS_INTERNA_CLAUSURA",  "]",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "DECLARATIO",             NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "REFERENTIA_PARAMETRI",   NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "VERBUM_PRAEDEFINITUM",   NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "DELIM",                  NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* trivia et reliqua */
    { "SPATIA",                 NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SPATIUM },
    /* octetus quem nulla regula accipit: in malum */
    { "IGNOTUM",                NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM }
};

constans MateriaLexiconCoctum XML_LEXICON = {
    GENERA_XML,
    (i32)(magnitudo(GENERA_XML) / magnitudo(GENERA_XML[0])),
    "xml-",
    (s32)-I   /* SPATIA VERBATIM: compressio nulla (exemplar toml) */
};
