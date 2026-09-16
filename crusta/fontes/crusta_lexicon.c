/* crusta_lexicon.c - Descriptor lexicalis crustae et dialectus bash
 *
 * Vide crusta_lexicon.h. Ordo ordinem CrustaLexGenus sequitur;
 * probatio_crusta_registrum id asserit per titulos. Tabulae dialecti
 * (CRUSTA_BASH) hic quoque: plagula una manu scripta tabularum.
 */

#include "crusta_lexicon.h"
#include "crusta_registrum.h"

hic_manens constans MateriaLexGenus GENERA_CRUSTA[] = {
    /* titulus                  orthographia species  munus */
    { "FINIS",                  "",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },

    /* structura */
    { "RESERVATUM",             NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ASSIGNATIO_TITULUS",     NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ASSIGNATIO_SUBSCRIPTUM", NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ASSIGNATIO_OPERATOR",    NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "REDIRECTIO_FD",          NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "REDIRECTIO_OPERATOR",    NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PIPA",                   "|",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PIPA_ERRORIS",           "|&",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ET",                     "&&",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "AUT",                    "||",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SEPARATOR",              NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    /* linea nova quae sententiam terminat: SUBSTANTIVA (C6) */
    { "SEPARATOR_LINEAE",       "\n",  MATERIA_LEX_TERMINATOR,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TERMINATOR_OPTIONIS",    NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PARENTHESIS",            "(",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PARENTHESIS_CLAUSURA",   ")",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ARITHMETICA_APERTURA",   "((",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ARITHMETICA_CLAUSURA",   "))",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "IUDICIUM_APERTURA",      "[[",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "IUDICIUM_CLAUSURA",      "]]",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* partes verbi */
    { "LITTERALIS",             NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "EFFUGIUM",               NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CONTINUATIO",            NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SIMPLEX",                NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "GEMINA_APERTURA",        "\"",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "GEMINA_CLAUSURA",        "\"",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "EFFUGIA",                NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "VERSA_APERTURA",         "$\"", MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PARAMETRUM_SIGILLUM",    NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PARAMETRUM_TITULUS",     NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "EXPANSIO_APERTURA",      "${",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "EXPANSIO_PRAEFIXUM",     NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "EXPANSIO_OPERATOR",      NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "EXPANSIO_CLAUSURA",      "}",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SUBSTITUTIO_APERTURA",   "$(",  MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    /* backtick: apertura et clausura; intra regionem profunditatis d
     * octeti '\'xd + '`' sunt - ergo VERBATIM */
    { "GRAVIS",                 NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ARITHMETICA_PARTIS_APERTURA", "$((", MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PROCESSUS_APERTURA",     NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "DOMUS",                  NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* heredoc */
    { "HEREDOC_DELIMITATOR",    NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* arithmetica */
    { "NUMERUS",                NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "VARIABILIS",             NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ARITHMETICA_OPERATOR",   NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "ARITHMETICA_SEPARATOR",  ";",   MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* iudicium */
    { "IUDICIUM_OPERATOR",      NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "REGULA",                 NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },

    /* trivia (C7: retro ligata per lineam novam ultimam) */
    { "SPATIUM",                NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SPATIUM },
    { "COMMENTUM",              NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_COMMENTUM },
    /* linea nova ut trivium (C6) */
    { "LINEA",                  "\n",  MATERIA_LEX_TERMINATOR,
        MATERIA_MUNUS_LINEA },
    /* '\' + linea nova inter verba: terminator physicus solus */
    { "LAMINA",                 NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_LAMINA }
};

constans MateriaLexiconCoctum CRUSTA_LEXICON = {
    GENERA_CRUSTA,
    (i32)(magnitudo(GENERA_CRUSTA) / magnitudo(GENERA_CRUSTA[0])),
    "crusta-",
    (s32)-I   /* SPATIUM VERBATIM: compressio nulla (exemplar css) */
};


/* ==================================================
 * Dialectus bash 5.2.15 (optiones ordinariae; spec par. V)
 * ================================================== */

hic_manens constans character* constans RESERVATA_BASH[] = {
    "if", "then", "else", "elif", "fi", "case", "esac", "for", "select",
    "while", "until", "do", "done", "in", "function", "time", "coproc",
    "{", "}", "[[", "]]", "!", NIHIL
};

/* aedificatores assignationum: post eos 'x=(...)' assignatio manet
 * (bash PST_COMPASSIGN) */
hic_manens constans character* constans AEDIFICATORES_BASH[] = {
    "alias", "declare", "export", "local", "readonly", "typeset", NIHIL
};

hic_manens constans character* constans REDIRECTIONES_BASH[] = {
    "<", ">", ">>", "<&", ">&", "<>", ">|", "&>", "&>>", "<<", "<<-",
    "<<<", NIHIL
};

hic_manens constans character* constans OPTIONUM_TERMINI_BASH[] = {
    ";;", ";&", ";;&", NIHIL
};

/* impletur in P4 (crusta_arithmetica): tabula vacua interim */
hic_manens constans CrustaOperator ARITHMETICI_BASH[] = {
    { NIHIL, ZEPHYRUM, FALSUM, CRUSTA_OPERATOR_BINARIUS }
};

hic_manens constans character* constans IUDICII_PRAEPOSITA_BASH[] = {
    "-a", "-b", "-c", "-d", "-e", "-f", "-g", "-h", "-k", "-p", "-r",
    "-s", "-t", "-u", "-w", "-x", "-G", "-L", "-N", "-O", "-S", "-z",
    "-n", "-o", "-v", "-R", NIHIL
};

hic_manens constans character* constans IUDICII_BINARIA_BASH[] = {
    "-nt", "-ot", "-ef", "-eq", "-ne", "-lt", "-le", "-gt", "-ge",
    "==", "=", "!=", "=~", "<", ">", NIHIL
};

constans CrustaDialectus CRUSTA_BASH = {
    RESERVATA_BASH,
    AEDIFICATORES_BASH,
    REDIRECTIONES_BASH,
    OPTIONUM_TERMINI_BASH,
    ARITHMETICI_BASH,
    IUDICII_PRAEPOSITA_BASH,
    IUDICII_BINARIA_BASH
};
