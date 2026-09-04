/* oratio_lexicon.c - Descriptor lexicalis orationis
 *
 * Vide oratio_lexicon.h. Ordo ordinem OratioLexGenus sequitur;
 * probatio_oratio_registrum id asserit per titulos.
 */

#include "oratio_lexicon.h"

hic_manens constans MateriaLexGenus GENERA_ORATIONIS[] = {
    /* titulus          orthographia  species                  munus */
    { "FINIS",          "",     MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },
    { "LINEA",          "\n",   MATERIA_LEX_TERMINATOR,
        MATERIA_MUNUS_LINEA },
    { "LINEA_CR",       NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_LINEA },
    { "SPATIUM",        NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    { "LITTERAE",       NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "DIGITI",         NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "HYPHEN",         "-",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "APOSTROPHUS",    NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PUNCTUM",        ".",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "INTERPUNCTIO",   NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SIGNUM",         NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    { "DERIVATUM",      NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM }
};

constans MateriaLexiconCoctum ORATIO_LEXICON = {
    GENERA_ORATIONIS,
    (i32)(magnitudo(GENERA_ORATIONIS) / magnitudo(GENERA_ORATIONIS[0])),
    "or-",
    (s32)-I   /* nullum spatium canonicum - trivia non exsistunt */
};
