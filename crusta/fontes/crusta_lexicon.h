/* crusta_lexicon.h - Descriptor lexicalis crustae pro materia
 *
 * MANU SCRIPTUS (exemplar html_lexicon.h). FONS VERITATIS generum
 * lexematum: enumeratio CrustaLexGenus INFRA (lector eam emittit,
 * lexicon eam describit); probatio_crusta_registrum ordinem per
 * titulos asserit.
 *
 * TRES RES DE CRUSTA QUAE MATERIAM EXERCENT (spec par. III, X):
 *
 * I. LINEA NOVA BIS (C6): eadem octeta genera duo sunt -
 *    SEPARATOR_LINEAE (SUBSTANTIVUM: sententiam terminat) et LINEA
 *    (munus LINEA: trivium
 *    post operatorem, post verbum reservatum, in positione imperii
 *    vacua). MODUS lectoris decernit, numquam relabellatio post.
 *    Ergo munus LINEA ADEST (responsum md, non html) - quid FIDELITAS
 *    de lingua faciat cuius octetus idem modo substantivus est, porta
 *    stml mensurat.
 *
 * II. TRIVIA RETRO LIGANTUR (C7): SPATIUM, COMMENTUM, LINEA, LAMINA
 *    usque ad lineam novam ultimam 'post' lexematis prioris sunt;
 *    reliqua 'ante' sequentis. Commentum caudale lineae suae est;
 *    corpus heredoc lineam novam sequitur (C5).
 *
 * III. LAMINA = '\' + linea nova INTER verba (terminator physicus
 *    solus, munus substrati); INTRA verbum eadem octeta pars
 *    'continuatio' sunt (verbum unum manet).
 */

#ifndef CRUSTA_LEXICON_H
#define CRUSTA_LEXICON_H

#include "latina.h"
#include "materia_lexicon.h"

nomen enumeratio {
    CRUSTA_LEX_FINIS = 0,

    /* structura */
    CRUSTA_LEX_RESERVATUM,
    CRUSTA_LEX_ASSIGNATIO_TITULUS,
    CRUSTA_LEX_ASSIGNATIO_SUBSCRIPTUM,
    CRUSTA_LEX_ASSIGNATIO_OPERATOR,
    CRUSTA_LEX_REDIRECTIO_FD,
    CRUSTA_LEX_REDIRECTIO_OPERATOR,
    CRUSTA_LEX_PIPA,
    CRUSTA_LEX_PIPA_ERRORIS,
    CRUSTA_LEX_ET,
    CRUSTA_LEX_AUT,
    CRUSTA_LEX_SEPARATOR,
    CRUSTA_LEX_SEPARATOR_LINEAE,
    CRUSTA_LEX_TERMINATOR_OPTIONIS,
    CRUSTA_LEX_PARENTHESIS,
    CRUSTA_LEX_PARENTHESIS_CLAUSURA,
    CRUSTA_LEX_ARITHMETICA_APERTURA,
    CRUSTA_LEX_ARITHMETICA_CLAUSURA,
    CRUSTA_LEX_IUDICIUM_APERTURA,
    CRUSTA_LEX_IUDICIUM_CLAUSURA,

    /* partes verbi */
    CRUSTA_LEX_LITTERALIS,
    CRUSTA_LEX_EFFUGIUM,
    CRUSTA_LEX_CONTINUATIO,
    CRUSTA_LEX_SIMPLEX,
    CRUSTA_LEX_GEMINA_APERTURA,
    CRUSTA_LEX_GEMINA_CLAUSURA,
    CRUSTA_LEX_EFFUGIA,
    CRUSTA_LEX_VERSA_APERTURA,
    CRUSTA_LEX_PARAMETRUM_SIGILLUM,
    CRUSTA_LEX_PARAMETRUM_TITULUS,
    CRUSTA_LEX_EXPANSIO_APERTURA,
    CRUSTA_LEX_EXPANSIO_PRAEFIXUM,
    CRUSTA_LEX_EXPANSIO_OPERATOR,
    CRUSTA_LEX_EXPANSIO_CLAUSURA,
    CRUSTA_LEX_SUBSTITUTIO_APERTURA,
    CRUSTA_LEX_GRAVIS,
    CRUSTA_LEX_ARITHMETICA_PARTIS_APERTURA,
    CRUSTA_LEX_PROCESSUS_APERTURA,
    CRUSTA_LEX_DOMUS,

    /* heredoc */
    CRUSTA_LEX_HEREDOC_DELIMITATOR,

    /* arithmetica */
    CRUSTA_LEX_NUMERUS,
    CRUSTA_LEX_VARIABILIS,
    CRUSTA_LEX_ARITHMETICA_OPERATOR,
    CRUSTA_LEX_ARITHMETICA_SEPARATOR,

    /* iudicium */
    CRUSTA_LEX_IUDICIUM_OPERATOR,
    CRUSTA_LEX_REGULA,

    /* trivia */
    CRUSTA_LEX_SPATIUM,
    CRUSTA_LEX_COMMENTUM,
    CRUSTA_LEX_LINEA,
    CRUSTA_LEX_LAMINA,

    CRUSTA_LEX_NUMERUS_GENERUM
} CrustaLexGenus;

externus constans MateriaLexiconCoctum CRUSTA_LEXICON;

#endif /* CRUSTA_LEXICON_H */
