/* toml_lexicon.c - Descriptor lexicalis toml
 *
 * Vide toml_lexicon.h. Ordo ordinem TomlLexGenus sequitur;
 * probatio_toml_registrum id asserit per titulos.
 */

#include "toml_lexicon.h"

hic_manens constans MateriaLexGenus GENERA_TOML[] = {
    /* titulus                  orthographia species  munus */
    { "FINIS",                  "",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },

    /* claves */
    { "CLAVIS_NUDA",            NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CLAVIS_GEMINA",          NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CLAVIS_SIMPLEX",         NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PUNCTUM",                ".",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SIGNUM",                 "=",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* capita */
    { "TABULA_APERTURA",        "[",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TABULA_CLAUSURA",        "]",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SERIES_TABULARUM_APERTURA", "[[", MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SERIES_TABULARUM_CLAUSURA", "]]", MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* valores */
    { "CHORDA_GEMINA",          NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CHORDA_GEMINA_MULTA",    NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CHORDA_SIMPLEX",         NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CHORDA_SIMPLEX_MULTA",   NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "NUMERUS",                NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TEMPUS",                 NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "VERUM",                  "true", MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "FALSUM",                 "false", MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SERIES_APERTURA",        "[",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SERIES_CLAUSURA",        "]",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "COMPACTA_APERTURA",      "{",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "COMPACTA_CLAUSURA",      "}",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "COMMA",                  ",",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* terminator et trivia */
    /* linea nova quae par aut caput terminat (substantivum) */
    { "LINEA_FINIS",            "\n",  MATERIA_LEX_TERMINATOR,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SPATIUM",                NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SPATIUM },
    { "COMMENTUM",              NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_COMMENTUM },
    /* linea nova intra seriem: trivium */
    { "LINEA",                  "\n",  MATERIA_LEX_TERMINATOR,
        MATERIA_MUNUS_LINEA },
    /* octetus quem nulla regula accipit: in malum */
    { "IGNOTUM",                NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM }
};

constans MateriaLexiconCoctum TOML_LEXICON = {
    GENERA_TOML,
    (i32)(magnitudo(GENERA_TOML) / magnitudo(GENERA_TOML[0])),
    "toml-",
    (s32)-I   /* SPATIUM VERBATIM: compressio nulla (exemplar crusta) */
};
