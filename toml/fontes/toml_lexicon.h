/* toml_lexicon.h - Descriptor lexicalis toml pro materia
 *
 * MANU SCRIPTUS (exemplar crusta_lexicon.h). FONS VERITATIS generum
 * lexematum: enumeratio TomlLexGenus INFRA (lector eam emittit,
 * lexicon eam describit); probatio_toml_registrum ordinem per titulos
 * asserit.
 *
 * DUO MODI (spec par. III): CLAVIS (initium lineae, intra capita, ante
 * '=' in tabula compacta) et VALOR (post '=', intra seriem). Idem
 * octetus modo decernente genera diversa fit ('[' caput vel series;
 * '1979-05-27' clavis nuda vel tempus) - numquam relabellatio post.
 *
 * LINEA NOVA BIS: LINEA_FINIS (SUBSTANTIVUM: par aut caput terminat)
 * et LINEA (munus LINEA: trivium intra seriem). Profunditas uncorum
 * decernit. CRLF: species TERMINATOR vexillum 'cr' fert (materia B6).
 *
 * CHORDAE: GEMINA = '"...' (effugia), SIMPLEX = '\'...' (litteralis),
 * utraque MULTA (triplex) - nomina ut in crusta.
 *
 * IGNOTUM: octetus (aut sequentia UTF-8 invalida) quem nulla regula
 * accipit; aedificator in 'malum' ponit (totalitas, T4).
 */

#ifndef TOML_LEXICON_H
#define TOML_LEXICON_H

#include "latina.h"
#include "materia_lexicon.h"

nomen enumeratio {
    TOML_LEX_FINIS = 0,

    /* claves */
    TOML_LEX_CLAVIS_NUDA,
    TOML_LEX_CLAVIS_GEMINA,
    TOML_LEX_CLAVIS_SIMPLEX,
    TOML_LEX_PUNCTUM,
    TOML_LEX_SIGNUM,

    /* capita */
    TOML_LEX_TABULA_APERTURA,
    TOML_LEX_TABULA_CLAUSURA,
    TOML_LEX_SERIES_TABULARUM_APERTURA,
    TOML_LEX_SERIES_TABULARUM_CLAUSURA,

    /* valores */
    TOML_LEX_CHORDA_GEMINA,
    TOML_LEX_CHORDA_GEMINA_MULTA,
    TOML_LEX_CHORDA_SIMPLEX,
    TOML_LEX_CHORDA_SIMPLEX_MULTA,
    TOML_LEX_NUMERUS,
    TOML_LEX_TEMPUS,
    TOML_LEX_VERUM,
    TOML_LEX_FALSUM,
    TOML_LEX_SERIES_APERTURA,
    TOML_LEX_SERIES_CLAUSURA,
    TOML_LEX_COMPACTA_APERTURA,
    TOML_LEX_COMPACTA_CLAUSURA,
    TOML_LEX_COMMA,

    /* terminator et trivia */
    TOML_LEX_LINEA_FINIS,
    TOML_LEX_SPATIUM,
    TOML_LEX_COMMENTUM,
    TOML_LEX_LINEA,
    TOML_LEX_IGNOTUM,

    TOML_LEX_NUMERUS_GENERUM
} TomlLexGenus;

externus constans MateriaLexiconCoctum TOML_LEXICON;

#endif /* TOML_LEXICON_H */
