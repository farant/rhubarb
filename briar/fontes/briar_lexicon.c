/* briar_lexicon.c - Vide briar_lexicon.h. Ordo ordinem BriarLexGenus
 * sequitur; probatio_briar_registrum id per titulos asserit. */

#include "briar_lexicon.h"

hic_manens constans MateriaLexGenus GENERA_BRIAR[] = {
    /* titulus        orthographia  species                munus */
    { "FINIS",        "",     MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },
    { "INTERPRES",    NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TAG_APERTUM",  NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TAG_CLAUSUM",  NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CRUDUM",       NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PROSA",        NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "STML",         NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "DERIVATUM",    NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM }
};

constans MateriaLexiconCoctum BRIAR_LEXICON = {
    GENERA_BRIAR,
    (i32)(magnitudo(GENERA_BRIAR) / magnitudo(GENERA_BRIAR[0])),
    "briar-",
    (s32)-I   /* nullum spatium canonicum - trivia non exsistunt */
};
