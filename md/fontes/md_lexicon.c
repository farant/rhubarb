/* md_lexicon.c - Descriptor lexicalis markdown
 *
 * Vide md_lexicon.h. Ordo ordinem MdLexGenus sequitur;
 * probatio_md_registrum id asserit per titulos.
 */

#include "md_lexicon.h"

hic_manens constans MateriaLexGenus GENERA_MD[] = {
    /* titulus              orthographia  species                  munus */
    { "FINIS",              "",     MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },
    { "LINEA",              "\n",   MATERIA_LEX_TERMINATOR,
        MATERIA_MUNUS_LINEA },
    { "LINEA_CR",           NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_LINEA },

    /* praefixa linearum - CONTENTUM, non trivia (modellum linearum) */
    { "INDENTATIO",         NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "MARCA_CITATIONIS",   NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "MARCA_PUNCTI",       NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "MARCA_NUMERI",       NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "MARCA_OFFICII",      NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "MARCA_ATX",          NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CLAUSURA_ATX",       NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SUBDUCTIO",          NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SAEPTUM",            NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "INFO",               NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "DIVISIO",            NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PIPA",               NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* contentum */
    { "TEXTUS",             NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "DELIMITATOR",        NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "GRAVIS",             NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "EFFUGIUM",           NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ENS",                NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "NEXUS_APERTUM",      NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "NEXUS_CAUDA",        NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "AUTONEXUS",          NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "HTML",               NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SPATIA_FINALIA",     NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* fons I */
    { "DERIVATUM",          NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM }
};

constans MateriaLexiconCoctum MD_LEXICON = {
    GENERA_MD,
    (i32)(magnitudo(GENERA_MD) / magnitudo(GENERA_MD[0])),
    "md-",
    (s32)-I   /* nullum spatium canonicum - trivia non exsistunt */
};
