/* silva_token.h - Contractus lexematis unificati silvae
 *
 * UNUS typus lexematis fert OMNIA: textum, positionem, trivia, originem.
 * Hoc est contractum quod arbor2 scindit (Lexema/Token) et quod scissio
 * permisit ut origo ad limitem parsatoris perderetur (M3). In silva origo
 * per constructionem inseparabilis est.
 *
 * Decisiones (vide project-specs/silva-spec-v2.md §8, simulatio ①):
 *   - origo INSERTA (non allocata separatim): numquam NIHIL, localitas melior
 *   - lexemata immutabilia et COMMUNIA trans strata (layer_index deletum;
 *     generatio = profunditas catenae originis)
 *   - initium_lineae = primum lexema non-trivia LINEAE LOGICAE
 *     (CONTINUATIO-conscium; directivum detectio fit proprietas localis)
 *   - scissurae: laminae \<nl> INTRA lexema (C89 phase 2); NIHIL fere semper
 *   - catenae originis IMMUTABILES post creationem (v1 praeparator pependit
 *     mutando catenam communem)
 *   - caecatio (hideset, Prosser): lista immutabilis cauda-communis
 */

#ifndef SILVA_TOKEN_H
#define SILVA_TOKEN_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* ==================================================
 * Genus lexematis - vocabularium lexicale C89 totum
 * + genera robustitatis (input quodlibet lexatur)
 * ================================================== */

nomen enumeratio {
    SILVA_LEX_EOF = 0,

    /* Robustitas: numquam frangere, semper lexare */
    SILVA_LEX_OCTETUS_IGNOTUS,       /* octetus non-C (garbage, NUL, ...) */
    SILVA_LEX_STRING_IMPERFECTUM,    /* littera chordae non terminata */
    SILVA_LEX_CHARACTER_IMPERFECTUM, /* littera characteris non terminata */

    /* Identificator et litterae */
    SILVA_LEX_IDENTIFICATOR,
    SILVA_LEX_INTEGER,
    SILVA_LEX_FLOAT,
    SILVA_LEX_CHARACTER_LIT,
    SILVA_LEX_STRING_LIT,

    /* Verba clavium C89 (nomina Anglica: verba ipsa C sunt) */
    SILVA_LEX_AUTO,
    SILVA_LEX_BREAK,
    SILVA_LEX_CASE,
    SILVA_LEX_CHAR,
    SILVA_LEX_CONST,
    SILVA_LEX_CONTINUE,
    SILVA_LEX_DEFAULT,
    SILVA_LEX_DO,
    SILVA_LEX_DOUBLE,
    SILVA_LEX_ELSE,
    SILVA_LEX_ENUM,
    SILVA_LEX_EXTERN,
    SILVA_LEX_FLOAT_KW,
    SILVA_LEX_FOR,
    SILVA_LEX_GOTO,
    SILVA_LEX_IF,
    SILVA_LEX_INT,
    SILVA_LEX_LONG,
    SILVA_LEX_REGISTER,
    SILVA_LEX_RETURN,
    SILVA_LEX_SHORT,
    SILVA_LEX_SIGNED,
    SILVA_LEX_SIZEOF,
    SILVA_LEX_STATIC,
    SILVA_LEX_STRUCT,
    SILVA_LEX_SWITCH,
    SILVA_LEX_TYPEDEF,
    SILVA_LEX_UNION,
    SILVA_LEX_UNSIGNED,
    SILVA_LEX_VOID,
    SILVA_LEX_VOLATILE,
    SILVA_LEX_WHILE,

    /* Interpunctiones */
    SILVA_LEX_QUADRA_APERTA,         /* [ */
    SILVA_LEX_QUADRA_CLAUSA,         /* ] */
    SILVA_LEX_PAREN_APERTA,          /* ( */
    SILVA_LEX_PAREN_CLAUSA,          /* ) */
    SILVA_LEX_BRACE_APERTA,          /* { */
    SILVA_LEX_BRACE_CLAUSA,          /* } */
    SILVA_LEX_PUNCTUM,               /* . */
    SILVA_LEX_SAGITTA,               /* -> */
    SILVA_LEX_INCREMENTUM,           /* ++ */
    SILVA_LEX_DECREMENTUM,           /* -- */
    SILVA_LEX_AMPERSAND,             /* & */
    SILVA_LEX_STAR,                  /* * */
    SILVA_LEX_PLUS,                  /* + */
    SILVA_LEX_MINUS,                 /* - */
    SILVA_LEX_TILDE,                 /* ~ */
    SILVA_LEX_EXCLAMATIO,            /* ! */
    SILVA_LEX_SOLIDUS,               /* / */
    SILVA_LEX_PERCENTUM,             /* % */
    SILVA_LEX_SINISTRORSUM,          /* << */
    SILVA_LEX_DEXTRORSUM,            /* >> */
    SILVA_LEX_MINOR,                 /* < */
    SILVA_LEX_MAIOR,                 /* > */
    SILVA_LEX_MINOR_AEQUALIS,        /* <= */
    SILVA_LEX_MAIOR_AEQUALIS,        /* >= */
    SILVA_LEX_AEQUALIS_AEQUALIS,     /* == */
    SILVA_LEX_NON_AEQUALIS,          /* != */
    SILVA_LEX_CARET,                 /* ^ */
    SILVA_LEX_BARRA,                 /* | */
    SILVA_LEX_ET_ET,                 /* && */
    SILVA_LEX_VEL_VEL,               /* || */
    SILVA_LEX_QUAESTIO,              /* ? */
    SILVA_LEX_COLON,                 /* : */
    SILVA_LEX_SEMICOLON,             /* ; */
    SILVA_LEX_ELLIPSIS,              /* ... */
    SILVA_LEX_ASSIGNATIO,            /* = */
    SILVA_LEX_STAR_ASSIGNATIO,       /* *= */
    SILVA_LEX_SOLIDUS_ASSIGNATIO,    /* /= */
    SILVA_LEX_PERCENTUM_ASSIGNATIO,  /* %= */
    SILVA_LEX_PLUS_ASSIGNATIO,       /* += */
    SILVA_LEX_MINUS_ASSIGNATIO,      /* -= */
    SILVA_LEX_SINISTRORSUM_ASSIGNATIO, /* <<= */
    SILVA_LEX_DEXTRORSUM_ASSIGNATIO, /* >>= */
    SILVA_LEX_AMPERSAND_ASSIGNATIO,  /* &= */
    SILVA_LEX_CARET_ASSIGNATIO,      /* ^= */
    SILVA_LEX_BARRA_ASSIGNATIO,      /* |= */
    SILVA_LEX_COMMA,                 /* , */
    SILVA_LEX_CANCELLUM,             /* #  (pp) */
    SILVA_LEX_CANCELLUM_CANCELLUM,   /* ## (pp) */

    /* Trivia (lexemata plena, spatia_* NIHIL in ipsis - nulla recursio) */
    SILVA_LEX_SPATIA,                /* n spatia */
    SILVA_LEX_TABULAE,               /* n tabulae */
    SILVA_LEX_NOVA_LINEA,            /* \n vel \r\n (valor verbatim) */
    SILVA_LEX_CONTINUATIO,           /* \<nl> INTER lexemata */
    SILVA_LEX_COMMENTUM_CLAUSUM,     /* comment block */
    SILVA_LEX_COMMENTUM_LINEA,       /* comment lineae (C99 - signatum) */

    SILVA_LEX_NUMERUS_GENERUM        /* comes generum, semper ultimus */
} SilvaLexemaGenus;


/* Signum standardi */
#define SILVA_STANDARD_C89       0
#define SILVA_STANDARD_C99       1
#define SILVA_STANDARD_EXTENSIO  2


/* ==================================================
 * Fons - identitas plagulae
 * Lexemata ferunt fons_index in tabulam contextus;
 * tabula ipsa a SilvaContextus administratur (Phase 2)
 * ================================================== */

nomen structura {
    chorda* via;              /* via plagulae interned; titulus syntheticus pro API */
    b32     est_syntheticus;  /* VERUM: textus ex API, non plagula vera */
    b32     est_lexicon;      /* plagula lexici (definitiones ante fontem
                               * principalem processatae - systema/latina);
                               * macros eius NON domestica pro examine
                               * alienorum (ISO iure ubique adhibentur) */
    b32     est_custos;       /* plagula custodita (#ifndef X / #define X / #endif) */
    chorda* custos_titulus;   /* titulus macro custodis; NIHIL si non custodita */
} SilvaFons;


/* ==================================================
 * Caecatio - hideset (Prosser), lista immutabilis
 * cauda-communis: extensio O(1), cauda communicata
 * ================================================== */

nomen structura SilvaCaecatio SilvaCaecatio;

structura SilvaCaecatio {
    chorda*        titulus;   /* titulus macro (interned praeferatur) */
    SilvaCaecatio* cauda;     /* lista parentis - NUMQUAM mutanda */
};


/* ==================================================
 * Scissura - lamina \<nl> INTRA lexema (C89 phase 2).
 * offset: positio in valore MUNDO ubi lamina reinserenda;
 * crlf: terminator erat \r\n (aliter \n)
 * ================================================== */

nomen structura {
    s32 offset;
    b32 crlf;
} SilvaScissura;


/* ==================================================
 * Origo - provenientia ut unio signata explicita.
 * Catena originis VIVIT in variantibus (praedecessores);
 * FONS nihil fert. Numquam mutanda post creationem.
 * ================================================== */

nomen enumeratio {
    SILVA_ORIGO_FONS = 0,     /* lexema fontis (stratum 0) */
    SILVA_ORIGO_EXPANSIO,     /* ex expansione macro */
    SILVA_ORIGO_PASTA,        /* ex ## */
    SILVA_ORIGO_CHORDA,       /* ex # (stringificatio) */
    SILVA_ORIGO_API           /* ex definitione per API iniecta */
} SilvaOrigoGenus;

nomen structura SilvaToken SilvaToken;

nomen structura {
    SilvaOrigoGenus genus;
    unio {
        structura {
            SilvaToken*    corpus;      /* lexema corporis macro (def-site) */
            SilvaToken*    invocatio;   /* lexema invocationis (use-site) */
            chorda*        nomen_macro;
            SilvaCaecatio* caecatio;    /* hideset lexematis expansi */
        } expansio;
        structura {
            SilvaToken*    sinister;    /* parens sinister ## */
            SilvaToken*    dexter;      /* parens dexter ## */
            SilvaToken*    invocatio;   /* lexema invocationis (use-site) */
            chorda*        nomen_macro;
            SilvaCaecatio* caecatio;
        } pasta;
        structura {
            SilvaToken*    primus;      /* primum lexema argumenti stringificati */
            chorda*        nomen_macro;
        } stringificatio;
        structura {
            chorda*        nomen_macro; /* definitio per API */
        } api;
    } datum;                            /* pro FONS: unio non usurpatur */
} SilvaOrigo;


/* ==================================================
 * SilvaToken - lexema unificatum
 * ================================================== */

structura SilvaToken {
    SilvaLexemaGenus genus;
    chorda           valor;          /* visus in textum fontis (vel synthesin) */
    s32              byte_offset;    /* intra fontem; -1 si synthesitum */
    i32              longitudo;      /* mensura in octetis */
    i32              linea;          /* 1-basata */
    i32              columna;        /* 1-basata */
    s32              fons_index;     /* in tabulam fontium contextus */
    i8               standard;       /* SILVA_STANDARD_* */
    b32              initium_lineae; /* primum non-trivia lineae LOGICAE */
    Xar*             spatia_ante;    /* Xar de SilvaToken* (trivia); NIHIL */
    Xar*             spatia_post;    /* Xar de SilvaToken* (trivia); NIHIL */
    Xar*             scissurae;      /* Xar de SilvaScissura; NIHIL fere semper */
    SilvaOrigo       origo;          /* INSERTA - numquam NIHIL */
};


/* ==================================================
 * Constructores (unus per genus originis)
 * ================================================== */

/* Lexema fontis (stratum 0) */
SilvaToken*
silva_token_ex_fonte (
    Piscina*         piscina,
    SilvaLexemaGenus genus,
    chorda           valor,
    s32              byte_offset,
    i32              linea,
    i32              columna,
    s32              fons_index);

/* Lexema ex expansione macro: campos lexicales a corpore capit (def-site),
 * invocationem et caecationem in origine servat */
SilvaToken*
silva_token_ex_expansione (
    Piscina*       piscina,
    SilvaToken*    corpus,
    SilvaToken*    invocatio,
    chorda*        nomen_macro,
    SilvaCaecatio* caecatio);

/* Lexema ex ## - textus novus, parentes ambo servati.
 *
 * 'invocatio' ANCORA EMISSIONIS est, non provenientia: sinister et
 * dexter dicunt QUID glutinatum sit (visio ea solario ostendit),
 * invocatio dicit UBI in octetis id factum sit. Ambo necessaria
 * sunt quia parens uterque ex CORPORE macri venire potest
 * ('#define C(a) pre##a' - 'pre' def-site est), ergo a parentibus
 * ad usum semita non semper ducit. */
SilvaToken*
silva_token_ex_pasta (
    Piscina*         piscina,
    SilvaLexemaGenus genus,
    chorda           valor,
    SilvaToken*      sinister,
    SilvaToken*      dexter,
    SilvaToken*      invocatio,
    chorda*          nomen_macro,
    SilvaCaecatio*   caecatio);

/* Lexema ex # - littera chordae synthetica */
SilvaToken*
silva_token_ex_stringificatione (
    Piscina*    piscina,
    chorda      valor,
    SilvaToken* primus,
    chorda*     nomen_macro);

/* Lexema praedefinitum (__FILE__/__LINE__/__STDC__) - littera
 * synthetica generis dati; origo CHORDA (primus = lexema
 * invocationis: radix/profunditas per eam ambulant, ergo linea
 * radicis = linea invocationis - semantica C89) */
SilvaToken*
silva_token_ex_praedefinito (
    Piscina*         piscina,
    SilvaLexemaGenus genus,
    chorda           valor,
    SilvaToken*      invocatio,
    chorda*          nomen_macro);

/* Lexema ex definitione API iniecta (fons syntheticus) */
SilvaToken*
silva_token_ex_api (
    Piscina*         piscina,
    SilvaLexemaGenus genus,
    chorda           valor,
    chorda*          nomen_macro,
    s32              fons_index);


/* ==================================================
 * Quaestiones catenae
 * ================================================== */

/* Radix use-site: sequitur invocatio/sinister/primus usque ad FONS/API */
SilvaToken*
silva_token_radix (
    SilvaToken* token);

/* Profunditas catenae = generatio expansionis (0 pro fonte) */
i32
silva_token_profunditas (
    SilvaToken* token);

/* Estne lexema fontis (stratum 0)? */
b32
silva_token_est_fons (
    SilvaToken* token);


/* ==================================================
 * Caecatio (hideset)
 * ================================================== */

/* Extendere: cella nova quae caudam parentis communicat - O(1) */
SilvaCaecatio*
silva_caecatio_extendere (
    Piscina*       piscina,
    SilvaCaecatio* parens,
    chorda*        titulus);

/* Continetne caecatio titulum? (comparatio contentus) */
b32
silva_caecatio_continet (
    SilvaCaecatio* caecatio,
    chorda         titulus);


/* ==================================================
 * Auxiliares
 * ================================================== */

/* Titulus generis pro debugging et grammatica (NOMINA_GENERUM) */
constans character*
silva_lexema_genus_nomen (
    SilvaLexemaGenus genus);

/* Titulus generis originis */
constans character*
silva_origo_genus_nomen (
    SilvaOrigoGenus genus);

#endif /* SILVA_TOKEN_H */
