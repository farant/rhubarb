/* oratio_lexicon.h - Descriptor lexicalis orationis (oratio-spec par. III)
 *
 * Lexema orationis NUMQUAM trivium est: spatia et lineae novae
 * CONTENTUM sunt in caudis (lex possessoris), ergo SPATIUM munus
 * SUBSTANTIVUM fert et genus_spatii -I est. LINEA munus LINEA fert
 * (TERMINATOR "\n" | "\r\n") ut comparator FIDELIS currat.
 *
 * LITTERAE = cursus litterarum ASCII ET sequentiarum UTF-8 multi-
 * octetorum (limen unicode: sequentia quaelibet littera est, sine
 * classibus aut plicatione - historia unicode rhubarbi postea).
 * PUNCTUM ('.') genus proprium: abbreviationes, initiales et fines
 * sententiarum eo pendent. APOSTROPHUS ''' aut curvus (multi-octetus).
 * INTERPUNCTIO = cursus interpunctionis ASCII ceterae; SIGNUM = octeti
 * ceteri (imperii, invalidi UTF-8).
 *
 * ORDO ENUMERATIONIS ordinem tabulae oratio_lexicon.c sequitur;
 * probatio id per TITULOS asserit. Genera APPENDUNTUR.
 */

#ifndef ORATIO_LEXICON_H
#define ORATIO_LEXICON_H

#include "latina.h"
#include "materia_lexicon.h"

nomen enumeratio {
    ORATIO_LEX_FINIS = 0,
    ORATIO_LEX_LINEA,          /* "\n" | "\r\n" (TERMINATOR, munus LINEA) */
    ORATIO_LEX_LINEA_CR,       /* '\r' solum (VERBATIM, munus LINEA) */
    ORATIO_LEX_SPATIUM,        /* spatia et tabulae (CONTENTUM in caudis) */

    ORATIO_LEX_LITTERAE,       /* cursus litterarum (ASCII + UTF-8) */
    ORATIO_LEX_DIGITI,         /* cursus digitorum */
    ORATIO_LEX_HYPHEN,         /* '-' inter litteras (pars vocabuli) */
    ORATIO_LEX_APOSTROPHUS,    /* ''' aut curvus inter litteras */
    ORATIO_LEX_PUNCTUM,        /* '.' */
    ORATIO_LEX_INTERPUNCTIO,   /* interpunctio cetera (cursus eiusdem) */
    ORATIO_LEX_SIGNUM,         /* octeti ceteri */

    /* fons I - valores derivati (classes, linguae, lemma, sensus) */
    ORATIO_LEX_DERIVATUM,

    ORATIO_LEX_NUMERUS_GENERUM
} OratioLexGenus;

#define ORATIO_FONS_PLAGULAE  ((s32)ZEPHYRUM)
#define ORATIO_FONS_DERIVATUS ((s32)I)

externus constans MateriaLexiconCoctum ORATIO_LEXICON;

#endif /* ORATIO_LEXICON_H */
