/* briar_lexicon.h - Descriptor lexicalis thistle
 *
 * Lexemata OMNIA verbatim praeter FINIS; lineae novae INTRA valores
 * vivunt (cursor materiae eas numerat - _cursorem_promovere), ergo
 * nullum genus LINEA: trivia non exsistunt, quisque octetus uni
 * lexemati pertinet. DERIVATUM (fons I) nomen tagi fert - octetos
 * nullos, emissor eum omittit.
 */

#ifndef BRIAR_LEXICON_H
#define BRIAR_LEXICON_H

#include "latina.h"
#include "materia_lexicon.h"

nomen enumeratio {
    BRIAR_LEX_FINIS = 0,
    BRIAR_LEX_INTERPRES,     /* linea '#!' prima, cum terminatore */
    BRIAR_LEX_TAG_APERTUM,   /* linea tagi aperti, cum terminatore */
    BRIAR_LEX_TAG_CLAUSUM,   /* linea '</nomen>', cum terminatore */
    BRIAR_LEX_CRUDUM,        /* contentum regionis crudae */
    BRIAR_LEX_PROSA,         /* cursus prosae (markdown) */
    BRIAR_LEX_STML,          /* extensio elementi STML tota */
    BRIAR_LEX_DERIVATUM,     /* fons I: nomen tagi */

    BRIAR_LEX_NUMERUS_GENERUM
} BriarLexGenus;

#define BRIAR_FONS_PLAGULAE   ((s32)0)
#define BRIAR_FONS_DERIVATUS  ((s32)1)

externus constans MateriaLexiconCoctum BRIAR_LEXICON;

#endif /* BRIAR_LEXICON_H */
