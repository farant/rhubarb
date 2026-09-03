/* md_lexicon.h - Descriptor lexicalis markdown (md-arbor-spec par. IV)
 *
 * Lexema markdown NUMQUAM trivium est: modellum linearum sine triviis
 * (spec par. III) omnem octetum in loco nodi ponit. Ergo INDENTATIO et
 * SPATIA_FINALIA munus SUBSTANTIVUM ferunt, non SPATIUM, et
 * genus_spatii -I est - lingua spatium canonicum NON habet, ne
 * compressio triviorum materiae quicquam inveniat.
 *
 * LINEA munus LINEA fert (species TERMINATOR: "\n" aut "\r\n" - CRLF
 * ut vexillum in STML), quod CSS carebat: lector materiae lineas ex
 * cursore reficit quem solum munus LINEA movet, ergo comparator
 * FIDELIS hic currere potest. LINEA_CR ('\r' solum, Mac vetus) rarus
 * est et VERBATIM munere LINEA: TERMINATOR in '\n' desinere debet.
 *
 * ORDO ENUMERATIONIS ordinem tabulae md_lexicon.c sequitur; probatio
 * id per TITULOS asserit (permutatio capitur, non absorbetur). Genera
 * APPENDUNTUR, numquam interponuntur.
 */

#ifndef MD_LEXICON_H
#define MD_LEXICON_H

#include "latina.h"
#include "materia_lexicon.h"

nomen enumeratio {
    MD_LEX_FINIS = 0,
    MD_LEX_LINEA,             /* "\n" | "\r\n" (TERMINATOR, munus LINEA) */
    MD_LEX_LINEA_CR,          /* '\r' solum (VERBATIM, munus LINEA) */

    /* praefixa linearum - continentia et marcae blocorum */
    MD_LEX_INDENTATIO,        /* spatia/tabulae ducentia */
    MD_LEX_MARCA_CITATIONIS,  /* '>' + spatium optionale */
    MD_LEX_MARCA_PUNCTI,      /* '-' '+' '*' (+ spatia sequentia) */
    MD_LEX_MARCA_NUMERI,      /* '1.' '1)' (+ spatia sequentia) */
    MD_LEX_MARCA_OFFICII,     /* '[ ]' '[x]' (GFM) */
    MD_LEX_MARCA_ATX,         /* '#'{1,6} + spatia */
    MD_LEX_CLAUSURA_ATX,      /* spatia + '#'+ + spatia in fine */
    MD_LEX_SUBDUCTIO,         /* '==='|'---' setext */
    MD_LEX_SAEPTUM,           /* '```' | '~~~' (limes saepti) */
    MD_LEX_INFO,              /* chorda info post saeptum apertum */
    MD_LEX_DIVISIO,           /* '***' '---' '___' */
    MD_LEX_PIPA,              /* '|' + spatia (tabulae GFM) */

    /* contentum */
    MD_LEX_TEXTUS,            /* cursus textus crudi */
    MD_LEX_DELIMITATOR,       /* cursus '*' '_' '~' */
    MD_LEX_GRAVIS,            /* cursus '`' */
    MD_LEX_EFFUGIUM,          /* '\' + character ASCII interpunctionis */
    MD_LEX_ENS,               /* '&nomen;' '&#123;' '&#x1F;' */
    MD_LEX_NEXUS_APERTUM,     /* '[' '![' '<' (autonexus/html) */
    MD_LEX_NEXUS_CAUDA,       /* '](...)' '][ref]' ']' */
    MD_LEX_AUTONEXUS,         /* URL nudus (GFM) */
    MD_LEX_HTML,              /* html crudum inlineum aut linea blocci */
    MD_LEX_SPATIA_FINALIA,    /* spatia in fine lineae (fractura dura) */

    /* fons I - valores semantici (spec par. III): emissor ea omittit */
    MD_LEX_DERIVATUM,

    MD_LEX_NUMERUS_GENERUM
} MdLexGenus;

/* Index fontis lexematum DERIVATORUM in tabula fontium proiectionis:
 * fons ZEPHYRUM = plagula, fons I = "derivata". */
#define MD_FONS_PLAGULAE  ((s32)ZEPHYRUM)
#define MD_FONS_DERIVATUS ((s32)I)

externus constans MateriaLexiconCoctum MD_LEXICON;

#endif /* MD_LEXICON_H */
