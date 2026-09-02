/* css_lexicon.c - Descriptor lexicalis CSS
 *
 * Vide css_lexicon.h. Ordo ordinem CssLexemaGenus
 * (include/css_lexema.h) sequitur; probatio_css_lexicon id asserit
 * per titulos, ne permutatio tacite absorbeatur.
 */

#include "css_lexicon.h"

hic_manens constans MateriaLexGenus GENERA_CSS[] = {
    /* titulus                     orthographia  species              munus */
    { "FINIS",                     "",     MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },

    /* nomina - contentum arbitrarium */
    { "IDENTIFICATOR",             NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "FUNCTIO",                   NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "APUD_VERBUM",               NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CANCELLUM",                 NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* litterae */
    { "CHORDA",                    NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CHORDA_IMPERFECTA",         NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "URL",                       NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "URL_IMPERFECTA",            NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "NUMERUS",                   NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CENTESIMA",                 NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "DIMENSIO",                  NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* trivia. SPATIA VERBATIM est, non REPETITUM - vide caput. */
    { "SPATIA",                    NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SPATIUM },
    { "COMMENTARIUM",              NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_COMMENTUM },
    { "COMMENTARIUM_IMPERFECTUM",  NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_COMMENTUM },

    /* interpunctio - orthographia in genere ipso vivit */
    { "CDO",                       "<!--", MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CDC",                       "-->",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "COLON",                     ":",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SEMICOLON",                 ";",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "COMMA",                     ",",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "QUADRA_APERTA",             "[",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "QUADRA_CLAUSA",             "]",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PAREN_APERTA",              "(",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PAREN_CLAUSA",              ")",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "BRACE_APERTA",              "{",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "BRACE_CLAUSA",              "}",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* octetus unus quilibet - NUL inclusus */
    { "DELIM",                     NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM }
};

constans MateriaLexiconCoctum CSS_LEXICON = {
    GENERA_CSS,
    (i32)(magnitudo(GENERA_CSS) / magnitudo(GENERA_CSS[0])),
    "lex-",
    /* genus_spatii: -I CONSULTO. CSS_LEX_SPATIA munus SPATIUM fert
     * et hic poni POSSET, sed compressio templorum speciem
     * REPETITUM poscit (materia_arbor.c custodit). -I honestius
     * est quam campus qui numquam ardet. */
    (s32)-I
};
