/* html_lexicon.c - Descriptor lexicalis HTML
 *
 * Vide html_lexicon.h. Ordo ordinem HtmlLexemaGenus
 * (include/html_lexema.h) sequitur; probatio_html_registrum id
 * asserit per titulos, ne permutatio tacite absorbeatur.
 */

#include "html_lexicon.h"

hic_manens constans MateriaLexGenus GENERA_HTML[] = {
    /* titulus                    orthographia  species  munus */
    { "FINIS",                        "",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },

    /* flumen datorum - contentum, nodi (H6) */
    { "TEXTUS",                       NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "REFERENTIA",                   NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TEXTUS_CRUDUS",                NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TEXTUS_CRUDUS_IMPERFECTUS",    NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* tags - orthographia FIXA ubi lexator unum valorem solum dat */
    { "TAG_APERTURA",                 NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TAG_CLAUSURA",                 NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TAG_FINIS",                    ">",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TAG_FINIS_SOLUS",              "/>",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* attributa */
    { "ATTRIBUTUM_NOMEN",             NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "AEQUALE",                      "=",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ATTRIBUTUM_VALOR",             NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ATTRIBUTUM_VALOR_IMPERFECTUS", NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* structurae speciales - nodi omnes; COMMENTARIUM SUBSTANTIVUM
     * consulto (caput, II) */
    { "COMMENTARIUM",                 NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "COMMENTARIUM_IMPERFECTUM",     NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "COMMENTARIUM_PRAVUM",          NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "DOCTYPE",                      NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "DOCTYPE_IMPERFECTUM",          NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CDATA",                        NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CDATA_IMPERFECTA",             NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* trivia INTRA TAG sola (caput, I et III). DELIM semper '/' unus
     * est et FIXUM fieri posset; VERBATIM ut semita probata (css
     * SPATIA) - trivium FIXUM nemo adhuc per scriptorem duxit. */
    { "SPATIA",                       NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SPATIUM },
    { "DELIM",                        NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SPATIUM }
};

constans MateriaLexiconCoctum HTML_LEXICON = {
    GENERA_HTML,
    (i32)(magnitudo(GENERA_HTML) / magnitudo(GENERA_HTML[0])),
    "lex-",
    /* genus_spatii: -I CONSULTO (ut css). SPATIA VERBATIM est;
     * compressio templorum speciem REPETITUM poscit (materia_arbor.c
     * custodit). */
    (s32)-I
};
