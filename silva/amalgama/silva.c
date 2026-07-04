/* silva.c - GENERATUM (amalgamator) - NE MANU EDITES
 *
 * Substratum silvae in plagula una (SQLite modo).
 * Fons veritatis: silva/fontes/ + bibliothecae
 * vendicatae in lib/. Regenerare: silva/amalgamare.sh
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* ================= silva.h (verbatim) ================= */
/* silva.h - Silva: C89 parsing substrate (public interface)
 *
 * MANU SCRIPTUM, vanilla C89 (spec-v2 par 11.1 / S43): nullum
 * vestigium latina.h hic - hospes qui variabiles "si", "per",
 * "character" declarat sine damno compilat (probatio pollutionis
 * hospitis id perpetuo imponit).
 *
 * Hoc caput possidet OMNES typos publicos: amalgamator unitates
 * typedef eiusdem tituli ex capitibus internis demittit (CADENDA),
 * ergo definitio quaeque in silva.c SEMEL existit - discrepantia
 * inter hoc caput et corpora compilationem ipsam frangit (deriva
 * se-corrigens). Latin type spellings map as: i8=unsigned char,
 * i32=unsigned int, s32=int, s64=long long, b32=int,
 * memoriae_index=size_t, chorda=SilvaChorda (NOT NUL-terminated!).
 *
 * API surface (M1, walking skeleton): parse pipeline (praeprocessor
 * incluso), oraculum + resolutio, byte-exact emission, sceletum
 * grammar tables. Generated per-genus accessors and deeper query
 * surfaces arrive with the C89 grammar (M2).
 */

#ifndef SILVA_H
#define SILVA_H

#include <stddef.h>  /* size_t */

/* ==================================================
 * Substratum vendicatum (renominatum): piscina, chorda, xar
 * ================================================== */

typedef struct SilvaPiscina SilvaPiscina;  /* arena memoriae (opaca) */

/* Chorda: mensura + datum, NON NUL-terminata */
typedef struct SilvaChorda {
    unsigned int   mensura;
    unsigned char* datum;
} SilvaChorda;

/* Xar: tabula crescens sine reallocatione (monstratores elementorum
 * stabiles trans additiones). Transparens: hospes per
 * silva_xar_numerus/obtinere legit. */
#define SILVA_XAR_MAXIMUS_SEGMENTORUM 64
typedef struct SilvaXar {
    unsigned int  numerus_elementorum;
    unsigned int  magnitudo_elementi;
    unsigned int  magnitudo_primi;
    unsigned int  numerus_segmentorum;
    unsigned int  capacitas_totalis;
    unsigned int  vexilla;
    SilvaPiscina* piscina;
    void*         segmenta[SILVA_XAR_MAXIMUS_SEGMENTORUM];
    char          titulus[32];
} SilvaXar;

SilvaPiscina* silva_piscina_generare_dynamicum(const char* titulus,
    size_t mensura_alvei_initia);
void silva_piscina_destruere(SilvaPiscina* piscina);

/* Telemetria arenae (additiones I): octeti in usu + apex usus -
 * hospites longaevi (saltuarius LRU) mensurare possunt */
size_t silva_piscina_summa_usus(const SilvaPiscina* piscina);
size_t silva_piscina_summa_apex_usus(const SilvaPiscina* piscina);

unsigned int silva_xar_numerus(const SilvaXar* xar);
void* silva_xar_obtinere(const SilvaXar* xar, unsigned int index);

/* ==================================================
 * Lexema (token) - genus, origo, contractus unificatus
 * ================================================== */

typedef enum {
    SILVA_LEX_EOF = 0,
    SILVA_LEX_OCTETUS_IGNOTUS,
    SILVA_LEX_STRING_IMPERFECTUM,
    SILVA_LEX_CHARACTER_IMPERFECTUM,
    SILVA_LEX_IDENTIFICATOR,
    SILVA_LEX_INTEGER,
    SILVA_LEX_FLOAT,
    SILVA_LEX_CHARACTER_LIT,
    SILVA_LEX_STRING_LIT,
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
    SILVA_LEX_QUADRA_APERTA,
    SILVA_LEX_QUADRA_CLAUSA,
    SILVA_LEX_PAREN_APERTA,
    SILVA_LEX_PAREN_CLAUSA,
    SILVA_LEX_BRACE_APERTA,
    SILVA_LEX_BRACE_CLAUSA,
    SILVA_LEX_PUNCTUM,
    SILVA_LEX_SAGITTA,
    SILVA_LEX_INCREMENTUM,
    SILVA_LEX_DECREMENTUM,
    SILVA_LEX_AMPERSAND,
    SILVA_LEX_STAR,
    SILVA_LEX_PLUS,
    SILVA_LEX_MINUS,
    SILVA_LEX_TILDE,
    SILVA_LEX_EXCLAMATIO,
    SILVA_LEX_SOLIDUS,
    SILVA_LEX_PERCENTUM,
    SILVA_LEX_SINISTRORSUM,
    SILVA_LEX_DEXTRORSUM,
    SILVA_LEX_MINOR,
    SILVA_LEX_MAIOR,
    SILVA_LEX_MINOR_AEQUALIS,
    SILVA_LEX_MAIOR_AEQUALIS,
    SILVA_LEX_AEQUALIS_AEQUALIS,
    SILVA_LEX_NON_AEQUALIS,
    SILVA_LEX_CARET,
    SILVA_LEX_BARRA,
    SILVA_LEX_ET_ET,
    SILVA_LEX_VEL_VEL,
    SILVA_LEX_QUAESTIO,
    SILVA_LEX_COLON,
    SILVA_LEX_SEMICOLON,
    SILVA_LEX_ELLIPSIS,
    SILVA_LEX_ASSIGNATIO,
    SILVA_LEX_STAR_ASSIGNATIO,
    SILVA_LEX_SOLIDUS_ASSIGNATIO,
    SILVA_LEX_PERCENTUM_ASSIGNATIO,
    SILVA_LEX_PLUS_ASSIGNATIO,
    SILVA_LEX_MINUS_ASSIGNATIO,
    SILVA_LEX_SINISTRORSUM_ASSIGNATIO,
    SILVA_LEX_DEXTRORSUM_ASSIGNATIO,
    SILVA_LEX_AMPERSAND_ASSIGNATIO,
    SILVA_LEX_CARET_ASSIGNATIO,
    SILVA_LEX_BARRA_ASSIGNATIO,
    SILVA_LEX_COMMA,
    SILVA_LEX_CANCELLUM,
    SILVA_LEX_CANCELLUM_CANCELLUM,
    SILVA_LEX_SPATIA,
    SILVA_LEX_TABULAE,
    SILVA_LEX_NOVA_LINEA,
    SILVA_LEX_CONTINUATIO,
    SILVA_LEX_COMMENTUM_CLAUSUM,
    SILVA_LEX_COMMENTUM_LINEA,
    SILVA_LEX_NUMERUS_GENERUM
} SilvaLexemaGenus;

#define SILVA_STANDARD_C89       0
#define SILVA_STANDARD_C99       1
#define SILVA_STANDARD_EXTENSIO  2

/* Caecatio (hideset, Prosser) - lista immutabilis cauda-communis */
typedef struct SilvaCaecatio SilvaCaecatio;
struct SilvaCaecatio {
    SilvaChorda*   titulus;
    SilvaCaecatio* cauda;
};

/* Origo - provenientia ut unio signata; FONS nihil fert */
typedef enum {
    SILVA_ORIGO_FONS = 0,
    SILVA_ORIGO_EXPANSIO,
    SILVA_ORIGO_PASTA,
    SILVA_ORIGO_CHORDA,
    SILVA_ORIGO_API
} SilvaOrigoGenus;

typedef struct SilvaToken SilvaToken;

typedef struct SilvaOrigo {
    SilvaOrigoGenus genus;
    union {
        struct {
            SilvaToken*    corpus;
            SilvaToken*    invocatio;
            SilvaChorda*   nomen_macro;
            SilvaCaecatio* caecatio;
        } expansio;
        struct {
            SilvaToken*    sinister;
            SilvaToken*    dexter;
            SilvaChorda*   nomen_macro;
            SilvaCaecatio* caecatio;
        } pasta;
        struct {
            SilvaToken*    primus;
            SilvaChorda*   nomen_macro;
        } stringificatio;
        struct {
            SilvaChorda*   nomen_macro;
        } api;
    } datum;
} SilvaOrigo;

struct SilvaToken {
    SilvaLexemaGenus genus;
    SilvaChorda      valor;          /* visus in textum fontis */
    int              byte_offset;    /* -1 si syntheticum */
    unsigned int     longitudo;
    unsigned int     linea;          /* 1-basata */
    unsigned int     columna;        /* 1-basata */
    int              fons_index;
    unsigned char    standard;       /* SILVA_STANDARD_* */
    int              initium_lineae;
    SilvaXar*        spatia_ante;    /* SilvaToken* (trivia); NULL */
    SilvaXar*        spatia_post;    /* SilvaToken* (trivia); NULL */
    SilvaXar*        scissurae;      /* laminae intra lexema; NULL */
    SilvaOrigo       origo;          /* inserta - numquam absens */
};

/* ==================================================
 * Lexatio + auxilia lexematum (additiones I)
 * ================================================== */

/* Lexare fontem: SilvaXar de SilvaToken* (EOF ultimo), trivia
 * (commenta, spatia) tokenis attachata in spatia_ante/spatia_post.
 * CONTRACTUS VITAE: FONS NON COPIATUR - valores lexematum in
 * textum fontis spectant, ergo textus vivere debet quamdiu
 * lexemata vivunt (arena eadem consulto). fons_index tokenis
 * inscribitur; 0 licet pro usu solitario. */
SilvaXar* silva_lexare(SilvaPiscina* piscina, const char* fons,
    unsigned int mensura, int fons_index);

/* Fluxus crudus: lexemata vera ET trivia INTERPOSITA, ordine
 * fontis - pro pictoribus (omnis octetus classificatus in
 * ordine). Idem contractus vitae. */
SilvaXar* silva_lexare_cruda(SilvaPiscina* piscina,
    const char* fons, unsigned int mensura, int fons_index);

/* Catena originis: radix (maiorum FONS/API), profunditas
 * (generatio expansionis; 0 = fons), estne stratum 0 */
SilvaToken* silva_token_radix(SilvaToken* token);
unsigned int silva_token_profunditas(SilvaToken* token);
int silva_token_est_fons(SilvaToken* token);

/* Tituli generum (tabellae originis, debugging) */
const char* silva_lexema_genus_nomen(SilvaLexemaGenus genus);
const char* silva_origo_genus_nomen(SilvaOrigoGenus genus);

/* ==================================================
 * Nodus + valor signatus (codex ut basis datorum)
 * ================================================== */

typedef enum {
    SILVA_LOCUS_NODUS = 0,
    SILVA_LOCUS_TOKEN,
    SILVA_LOCUS_LISTA_NODUS,
    SILVA_LOCUS_LISTA_TOKEN,
    SILVA_LOCUS_LISTA_MIXTA,
    SILVA_LOCUS_INDEX
} SilvaLocusSpecies;

typedef enum {
    SILVA_VALOR_NIHIL = 0,
    SILVA_VALOR_NODUS,
    SILVA_VALOR_TOKEN,
    SILVA_VALOR_LISTA,
    SILVA_VALOR_INDEX
} SilvaValorGenus;

typedef struct SilvaNodus SilvaNodus;
typedef struct SilvaValor SilvaValor;

/* Prospectus listae: {repositorium, mensura} - repositorium commune,
 * mensura PROSPECTUS est veritas (numquam xar numerus directus) */
typedef struct SilvaListaProspectus {
    SilvaXar*    xar;
    unsigned int mensura;
} SilvaListaProspectus;

struct SilvaValor {
    SilvaValorGenus genus;
    union {
        SilvaNodus*          nodus;
        SilvaToken*          token;
        SilvaListaProspectus lista;
        int                  index;
    } datum;
};

struct SilvaNodus {
    int          genus;            /* index in registro generum */
    unsigned int numerus_locorum;
    SilvaValor*  loci;             /* ordine layout */
    SilvaNodus*  pater;            /* post commissionem */
};

unsigned int silva_valor_lista_numerus(SilvaValor lista);
SilvaValor* silva_valor_lista_obtinere(SilvaValor lista,
    unsigned int index);

/* Liberi nodales (NODUS loci + elementa NODUS listarum, ordine
 * locorum): SilvaXar de SilvaNodus* (additiones I) */
SilvaXar* silva_nodus_liberi(SilvaPiscina* piscina,
    const SilvaNodus* nodus);

/* ==================================================
 * Tabulae coctae (generatae - contractus stabilis)
 * ================================================== */

typedef enum {
    SILVA_TAB_ACTIO_ERRARE = 0,
    SILVA_TAB_ACTIO_TRANSPONERE,
    SILVA_TAB_ACTIO_REDUCERE,
    SILVA_TAB_ACTIO_ACCIPERE,
    SILVA_TAB_ACTIO_RECUPERARE
} SilvaTabActioGenus;

typedef struct SilvaTabActio {
    int          terminalis;   /* -1 = $ */
    int          actio;        /* SilvaTabActioGenus */
    int          valor;
    unsigned int pretium;
} SilvaTabActio;

typedef struct SilvaTabGoto {
    int non_terminalis;
    int status_novus;
} SilvaTabGoto;

typedef struct SilvaTabStatus {
    unsigned int actiones_offset;
    unsigned int actiones_numerus;
    unsigned int goto_offset;
    unsigned int goto_numerus;
} SilvaTabStatus;

typedef struct SilvaTabSymbolum {
    const char*      titulus;
    SilvaLexemaGenus genus_lexematis;
    int              est_terminale;
} SilvaTabSymbolum;

typedef struct SilvaTabProductio {
    int          sinistrum;
    unsigned int longitudo;
    const char*  id;
} SilvaTabProductio;

typedef struct SilvaTabLocus {
    const char* titulus;
    int         species;       /* SilvaLocusSpecies */
} SilvaTabLocus;

typedef struct SilvaTabGenus {
    const char*  titulus;
    unsigned int loci_offset;
    unsigned int loci_numerus;
} SilvaTabGenus;

typedef struct SilvaRegistrumCoctum {
    const SilvaTabGenus* genera;
    unsigned int         numerus_generum;
    const SilvaTabLocus* loci;
    unsigned int         numerus_locorum;
} SilvaRegistrumCoctum;

typedef struct SilvaTabulaCocta {
    const SilvaTabSymbolum*  symbola;
    unsigned int             numerus_symbolorum;
    unsigned int             numerus_terminalium;
    const SilvaTabProductio* productiones;
    unsigned int             numerus_productionum;
    const SilvaTabStatus*    status;
    unsigned int             numerus_statuum;
    const SilvaTabActio*     actiones;
    unsigned int             numerus_actionum;
    const SilvaTabGoto*      goto_introitus;
    unsigned int             numerus_goto;
    int                      initium_index;
    int                      productio_augmentata;
    int                      numerus_conflictuum;
} SilvaTabulaCocta;

/* Subscriptiones generatae (constructor + fabrica ambigui) */
typedef SilvaValor (*SilvaGLRConstructor)(SilvaPiscina* piscina,
    int productio, const SilvaValor* valores);
typedef SilvaValor (*SilvaGLRFabricaAmbigui)(SilvaPiscina* piscina,
    SilvaValor interpretationes, int canonica);

/* ==================================================
 * Oraculum + resolutio
 * ================================================== */

typedef struct SilvaOraculum SilvaOraculum;

SilvaOraculum* silva_oraculum_creare(SilvaPiscina* piscina);
int silva_oraculum_typum_addere(SilvaOraculum* oraculum,
    SilvaChorda titulus);
int silva_oraculum_typum_addere_literis(SilvaOraculum* oraculum,
    const char* titulus);
int silva_oraculum_typum_novit(const SilvaOraculum* oraculum,
    SilvaChorda titulus);

typedef struct SilvaResolutioResponsum {
    int         victor;        /* index interpretationis; -1 ignotum */
    SilvaToken* discriminans;  /* NULL licet */
} SilvaResolutioResponsum;

typedef void (*SilvaResolutor)(const SilvaNodus* ambiguum,
    const SilvaOraculum* oraculum, void* datum_resolutoris,
    SilvaResolutioResponsum* responsum);

typedef enum {
    SILVA_RESOLUTIO_COLLAPSUS = 0,
    SILVA_RESOLUTIO_RECANONICATA
} SilvaResolutioGenus;

typedef struct SilvaResolutioEventum {
    SilvaResolutioGenus genus;
    SilvaNodus*         sedes;
    int                 victor;
    int                 genus_victoris;
    SilvaToken*         discriminans;
} SilvaResolutioEventum;

typedef struct SilvaCommissio {
    SilvaValor   radix;
    SilvaXar*    ambigui;        /* SilvaNodus* superstites */
    SilvaXar*    resolutiones;   /* SilvaResolutioEventum */
    int          genus_ambigui;
    unsigned int locus_interpretationum;
    unsigned int locus_canonicae;
} SilvaCommissio;

unsigned int silva_recanonicare(SilvaCommissio* commissio,
    const SilvaOraculum* oraculum, SilvaResolutor resolutor,
    void* datum_resolutoris);

/* ==================================================
 * Praeprocessor (expansio) - contextus praeparabilis
 * ================================================== */

typedef struct SilvaExpansio SilvaExpansio;

SilvaExpansio* silva_expansio_creare(SilvaPiscina* piscina);
int silva_fons_addere(SilvaExpansio* exp, const char* via,
    int est_syntheticus);
int silva_includendum_praebere(SilvaExpansio* exp, const char* via,
    const char* textus, unsigned int mensura);
int silva_macro_addere(SilvaExpansio* exp, const char* titulus,
    const char* corpus);
int silva_macro_functio_addere(SilvaExpansio* exp,
    const char* titulus, const char** parametra, const char* corpus);

/* ==================================================
 * Contextus hospitis (Phase 7) - receptum diu vivens:
 * fines (limina dura; 0 = infinitum; sub finibus arbor
 * completa manet et octetim exacte reconstruitur),
 * intermissio (pergere - monotona sit: semel falsum,
 * semper falsum), lexica (definitiones ante fontem -
 * latina compilata via silva_contextus_latinam_addere),
 * praebenda (includenda)
 * ================================================== */

typedef struct SilvaFines {
    unsigned int lexemata;                /* fluxus expansus max */
    unsigned int generationes;
    unsigned int profunditas_includendi;
    unsigned int profunditas_regionum;
    unsigned int frons;                   /* frons GSS max */
} SilvaFines;

typedef int (*SilvaPergereFunctio)(void* datum);

typedef struct SilvaContextusPlagula {
    const char*  via;
    const char*  textus;
    unsigned int mensura;
} SilvaContextusPlagula;

typedef struct SilvaContextus SilvaContextus;

struct SilvaContextus {
    SilvaPiscina*       piscina;
    SilvaFines          fines;
    SilvaPergereFunctio pergere;          /* NULL = numquam rogare */
    void*               pergere_datum;
    unsigned int        passus_pergendi;  /* intervallum lexematum */
    SilvaXar*           lexica;           /* SilvaContextusPlagula */
    SilvaXar*           praebenda;        /* SilvaContextusPlagula */
};

SilvaContextus* silva_contextus_creare(SilvaPiscina* piscina);
int silva_contextus_lexicon_addere(SilvaContextus* contextus,
    const char* via, const char* textus, unsigned int mensura);
int silva_contextus_latinam_addere(SilvaContextus* contextus);
int silva_contextus_praebere(SilvaContextus* contextus,
    const char* via, const char* textus, unsigned int mensura);
void silva_contextus_pergere_ponere(SilvaContextus* contextus,
    SilvaPergereFunctio pergere, void* datum, unsigned int passus);

/* Textus latina.h compilatus (GENERATUM ex include/latina.h) */
extern const char         silva_latina_textus[];
extern const unsigned int silva_latina_mensura;

/* ==================================================
 * Gubernator (fistula tota) + fructus
 * ================================================== */

typedef struct SilvaGrammatica {
    const SilvaTabulaCocta*     tabula;
    const SilvaRegistrumCoctum* tabularium;
    SilvaGLRConstructor         constructor;
    SilvaGLRFabricaAmbigui      fabrica;
} SilvaGrammatica;

typedef struct SilvaParsura {
    int             successus;
    SilvaCommissio* commissio;
    SilvaToken*     lexema_finis;   /* EOF - trivia caudae */
    SilvaXar*       lexemata;       /* fluxus expansus */
    SilvaExpansio*  expansio;       /* NULL in ingressu lexematum */
    SilvaXar*       directivae;     /* lineae consumptae; NULL */
    int             fons_princeps;  /* NB non 0 post praebere! */
    unsigned int    numerus_errorum;
    unsigned int    numerus_segmentorum;
    unsigned int    fusiones;
    unsigned int    transmutationes;
    unsigned int    transmutationes_negatae;
    unsigned int    eventa_marginis_novi;
    unsigned int    frons_maxima;
    int             est_intermissa;   /* pergere falsum; arbor tamen
                                       * completa (cauda = ERROR) */
    int             expansio_decisa;  /* expansio trunca (limen) */
    int             fines_tactae;     /* quilibet limen tactus */
    unsigned int    segmenta_ultra_limen;
    unsigned int    regiones_textae;  /* regiones in arborem textae */
    unsigned int    regiones_omissae; /* degradatae (limes transgressus) */
    SilvaXar*       strata;           /* per-generation streams (Xar of
                                       * SilvaXar* of SilvaToken*), one per
                                       * MUTATING expansion generation;
                                       * NULL if nothing ever expanded.
                                       * strata[last] ALIASES lexemata.
                                       * Layer view = [as-written] +
                                       * strata[] (additiones II) */
} SilvaParsura;

SilvaParsura* silva_parsare(SilvaPiscina* piscina,
    const char* titulus_fontis, const char* fons,
    unsigned int mensura, const SilvaGrammatica* grammatica,
    const SilvaOraculum* oraculum, SilvaResolutor resolutor,
    void* datum_resolutoris);

SilvaParsura* silva_parsare_cum_expansione(SilvaPiscina* piscina,
    SilvaExpansio* expansio, const char* titulus_fontis,
    const char* fons, unsigned int mensura,
    const SilvaGrammatica* grammatica,
    const SilvaOraculum* oraculum, SilvaResolutor resolutor,
    void* datum_resolutoris);

SilvaParsura* silva_parsare_cum_contextu(SilvaPiscina* piscina,
    const SilvaContextus* contextus, const char* titulus_fontis,
    const char* fons, unsigned int mensura,
    const SilvaGrammatica* grammatica,
    const SilvaOraculum* oraculum, SilvaResolutor resolutor,
    void* datum_resolutoris);

SilvaParsura* silva_lexemata_parsare(SilvaPiscina* piscina,
    const SilvaXar* lexemata, const SilvaGrammatica* grammatica,
    const SilvaOraculum* oraculum, SilvaResolutor resolutor,
    void* datum_resolutoris);

/* ==================================================
 * Expansio: reading windows (additiones II) - SilvaExpansio stays
 * OPAQUE; these slim vistas are the cathedral-time contract
 * (internal shapes remain free). Ramus index = conditio_id - 1
 * (flat table). Extents are BYTE offsets into the fons text.
 * ================================================== */

typedef enum {
    SILVA_RAMUS_IF = 0,
    SILVA_RAMUS_IFDEF,
    SILVA_RAMUS_IFNDEF,
    SILVA_RAMUS_ELIF,
    SILVA_RAMUS_ELSE
} SilvaRamusGenus;

typedef struct {
    const SilvaChorda* via;
    int                fons_ex;
    int                fons_ad;           /* -1 = unresolved */
    int                est_praetermissa;
} SilvaInclusioVista;

typedef struct {
    SilvaRamusGenus    genus;
    int                est_sumptum;
    int                est_numquam;       /* #if 0 idiom */
    int                corpus_initium;    /* BYTES in fons; -1 empty */
    int                corpus_finis;      /* exclusive; -1 empty */
    int                fons_index;
    unsigned int       linea;
} SilvaRamusVista;

typedef struct {
    const SilvaChorda* titulus;
    int                est_functio;
    int                fons_index;
    unsigned int       linea;
} SilvaMacroVista;

unsigned int silva_fontes_numerus(const SilvaExpansio* exp);
/* NULL if out of range; synthetic fontes return their title */
const SilvaChorda* silva_fons_via(const SilvaExpansio* exp,
    int fons_index);
unsigned int silva_inclusiones_numerus(const SilvaExpansio* exp);
int silva_inclusio_vista(const SilvaExpansio* exp,
    unsigned int index, SilvaInclusioVista* vista_out);
unsigned int silva_rami_numerus(const SilvaExpansio* exp);
int silva_ramus_vista(const SilvaExpansio* exp,
    unsigned int index, SilvaRamusVista* vista_out);
/* Definitions AS RECORDED (the journal is the product: #undef does
 * not erase history); O(acta) per call */
unsigned int silva_macros_numerus(const SilvaExpansio* exp);
int silva_macro_vista(const SilvaExpansio* exp,
    unsigned int index, SilvaMacroVista* vista_out);


/* ==================================================
 * Scriptura (emissio octetim exacta)
 * ================================================== */

typedef struct SilvaScriptura {
    int               successus;
    SilvaChorda       textus;
    const char*       causa;    /* diagnostica statica; NULL si bene */
    const SilvaNodus* sedes;    /* nodus fracturae; NULL licet */
} SilvaScriptura;

SilvaScriptura silva_scribere_valorem(SilvaPiscina* piscina,
    SilvaValor valor, const SilvaRegistrumCoctum* tabularium,
    const SilvaExpansio* expansio);
SilvaScriptura silva_scribere_nodum(SilvaPiscina* piscina,
    const SilvaNodus* nodus, const SilvaRegistrumCoctum* tabularium,
    const SilvaExpansio* expansio);
SilvaScriptura silva_scribere_fontem(SilvaPiscina* piscina,
    const SilvaParsura* parsura,
    const SilvaRegistrumCoctum* tabularium, int fons_index);

/* ==================================================
 * Grammatica sceleti (M1) - superficies generata stabilis
 * ================================================== */

extern const SilvaTabulaCocta     SILVA_SCELETUM_TABULA;
extern const SilvaRegistrumCoctum SILVA_SCELETUM_REGISTRUM;

SilvaValor silva_sceletum_construere(SilvaPiscina* piscina,
    int productio, const SilvaValor* valores);
SilvaValor silva_sceletum_ambiguum_fabricare(SilvaPiscina* piscina,
    SilvaValor interpretationes, int canonica);

#endif /* SILVA_H */

/* ================= ex include/latina.h ================= */
#ifndef LATINA_H
#define LATINA_H

#include <stddef.h>

#define character 	char
#define brevis 			short
#define integer 		int
#define longus			long
#define fluitans		float
#define duplex			double

#define vacuum			void
#define signatus 		signed
#define insignatus  unsigned
#define constans		const
#define volatilis		volatile
#define sponte			auto
#define registrum 	register
#define staticus 		static
#define	externus 		extern

#define si					if
#define alioquin		else
#define commutatio	switch
#define casus				case
#define ordinarius	default
#define per					for
#define dum 				while
#define fac 				do
#define frange 			break
#define perge 			continue
#define salta				goto
#define redde				return

#define structura		struct
#define unio 				union	
#define enumeratio 	enum
#define nomen 			typedef

#define magnitudo 	sizeof

#define principale 	main

#define NIHIL				NULL
#define VERUM 			1
#define FALSUM 			0

#define ZEPHYRUM    0
#define I 					1
#define II					2
#define III					3
#define IV					4
#define V						5
#define VI					6
#define VII					7
#define VIII				8
#define IX					9
#define X						10
#define XI					11
#define XII					12
#define XIII				13
#define XIV					14
#define XV					15
#define XVI					16
#define XVII				17
#define XVIII				18
#define XIX					19
#define XX					20
#define XXI					21
#define XXII				22
#define XXIII				23
#define XXIV				24
#define XXV					25
#define XXVI				26
#define XXVII				27
#define XXVIII			28
#define XXIX				29
#define XXX					30
#define XXXI				31
#define XXXII				32
#define XXXIII			33
#define XXXIV				34
#define XXXV				35
#define XXXVI				36
#define XXXVII			37
#define XXXVIII			38
#define XXXIX				39
#define XL					40
#define XLI					41
#define XLII				42
#define XLIII				43
#define XLIV				44
#define XLV					45
#define XLVI				46
#define XLVII				47
#define XLVIII			48
#define XLIX				49
#define L						50
#define LI					51
#define LII					52
#define LIII				53
#define LIV					54
#define LV					55
#define LVI					56
#define LVII				57
#define LVIII				58
#define LIX					59
#define LX					60
#define LXI					61
#define LXII				62
#define LXIII				63
#define LXIV				64
#define LXV				  65
#define LXVI				66
#define LXVII				67
#define LXVIII			68
#define LXIX				69
#define LXX					70
#define LXXI				71
#define LXXII				72
#define LXXIII			73
#define LXXIV				74
#define LXXV				75
#define LXXVI			  76
#define LXXVII			77
#define LXXVIII			78
#define LXXIX				79
#define LXXX				80
#define LXXXI				81
#define LXXXII			82
#define LXXXIII			83
#define LXXXIV			84
#define LXXXV				85
#define LXXXVI			86
#define LXXXVII			87
#define LXXXVIII		88
#define LXXXIX			89
#define XC					90
#define XCI					91
#define XCII				92
#define XCIII				93
#define XCIV				94
#define XCV					95
#define XCVI				96
#define XCVII				97
#define XCVIII			98
#define XCIX				99
#define C						100
#define CI 					101
#define CII 				102
#define CIII				103
#define CIV					104
#define CV					105
#define CVI					106
#define CVII				107
#define CVIII				108
#define CIX					109
#define CX					110
#define CXI					111
#define CXII				112
#define CXIII				113
#define CXIV				114
#define CXV					115
#define CXVI				116
#define CXVII				117
#define CXVIII			118
#define CXIX				119
#define CXX 				120
#define CXXI				121
#define CXXII 			122
#define CXXIII			123
#define CXXIV 			124
#define CXXV 				125
#define CXXVI 			126
#define CXXVII 			127
#define CXXVIII 		128
#define CXXIX 			129
#define CXXX 				130
#define CXXXI 			131
#define CXXXII 			132
#define CXXXIII			133
#define CXXXIV 			134
#define CXXXV 			135
#define CXXXVI 			136
#define CXXXVII 		137
#define CXXXVIII 		138
#define CXXXIX 			139
#define CXL 				140
#define CXLI 				141
#define CXLII 			142
#define CXLIII			143
#define CXLIV 			144
#define CXLV 				145
#define CXLVI 			146
#define CXLVII 			147
#define CXLVIII 		148
#define CXLIX 			149
#define CL					150
#define CLI 				151
#define CLII 				152
#define CLIII 			153
#define CLIV 				154
#define CLV 				155
#define CLVI 				156
#define CLVII 			157
#define CLVIII 			158
#define CLIX 				159
#define CLX					160
#define CLXI				161
#define CLXII				162
#define CLXIII			163
#define CLXIV				164
#define CLXV				165
#define CLXVI				166
#define CLXVII			167
#define CLXVIII			168
#define CLXIX				169
#define CLXX				170
#define CLXXI				171
#define CLXXII			172
#define CLXXIII			173
#define CLXXIV			174
#define CLXXV				175
#define CLXXVI			176
#define CLXXVII			177
#define CLXXVIII		178
#define CLXXIX			179
#define CLXXX				180
#define CLXXXI			181
#define CLXXXII			182
#define CLXXXIII		183
#define CLXXXIV			184
#define CLXXXV			185
#define CLXXXVI			186
#define CLXXXVII		187
#define CLXXXVIII		188
#define CLXXXIX			189
#define CXC					190
#define CXCI 				191
#define CXCII 			192
#define CXCIII			193
#define CXCIV				194
#define CXCV				195
#define CXCVI				196
#define CXCVII			197
#define CXCVIII			198
#define CXCIX				199
#define CC					200
#define CCI					201
#define CCII				202
#define CCIII				203
#define CCIV				204
#define CCV					205
#define CCVI				206
#define CCVII				207
#define CCVIII			208
#define CCIX				209
#define CCX					210
#define CCXI				211
#define CCXII				212
#define CCXIII			213
#define CCXIV			  214
#define CCXX				220
#define CCXXX				230
#define CCXXXI			231
#define CCXXXII			232
#define CCXXXIII		233
#define CCXXXIV			234
#define CCXXXV			235
#define CCXXXVI			236
#define CCXXXVII		237
#define CCXXXVIII		238
#define CCXXXIX			239
#define CCXL				240
#define CCXLI				241
#define CCXLII			242
#define CCL					250
#define CCLI 				251
#define CCLII 			252
#define CCLIII 			253
#define CCLIV 			254
#define CCLV 				255
#define CCLVI 			256
#define CCLX				260
#define CCLXX				270
#define CCLXXX			280
#define CCLXXXIV		284
#define CCLXXXV		  285
#define CCXC				290
#define CCC				  300
#define CCCI				301
#define CCCII				302
#define CCCIII			303
#define CCCIV				304
#define CCCV				305
#define CCCVI				306
#define CCCVII			307
#define CCCVIII			308
#define CCCX				310
#define CCCXX				320
#define CCCXXX			330
#define CCCXL				340
#define CCCLX 			360
#define CD					400
#define CDI					401
#define CDII				402
#define CDIII				403
#define CDIV				404
#define CDV					405
#define CDVI				406
#define CDVII				407
#define CDVIII			408
#define CDIX				409
#define CDX					410
#define CDXI				411
#define CDXII				412
#define CDXIII			413
#define CDXIV				414
#define CDXV				415
#define CDXVI				416
#define CDXVII			417
#define CDXVIII			418
#define CDXIX				419
#define CDXX 				420
#define CDXXI				421
#define CDXXII			422
#define CDXXIII			423
#define CDXXIV			424
#define CDXXV				425
#define CDXXVI 			426
#define CDXXVII			427
#define CDXXVIII		428
#define CDXXIX			429
#define CDXXX 			430
#define CDXL 				440
#define CDL 				450
#define CDLI 				451
#define CDLII 			452
#define CDLIII 			453
#define CDLIV 			454
#define CDLV 				455
#define CDLVI 			456
#define CDLVII			457
#define CDLVIII			458
#define CDLIX				459
#define CDLX 				460
#define CDLXX				470
#define CDLXXX			480
#define D						500
#define DI					501
#define DII					502
#define DIII				503
#define DIV					504
#define DV					505
#define DVI					506
#define DVII				507
#define DVIII				508
#define DIX					509
#define DX 					510
#define DXI 				511
#define DXII 				512
#define DLXVIII			568
#define DLXXV				575
#define DC					600
#define DCXXXIX			639
#define DCC					700
#define DCCL				750
#define DCCC				800
#define DCCCLIII		853
#define CM					900
#define CMLXXIX		  979
#define M						1000
#define MDCCLXXVI		1776
#define MCM					1900
#define MCMX 			  1910
#define MCMXCIX			1999
#define MM					2000
#define MMIV				2004
#define MMXX				2020
#define MMXXI				2021
#define MMXXII			2022
#define MMXXIII			2023
#define MMXXIV			2024
#define MMXXV				2025
#define MMC					2100
#define MMM					3000
#define MMMM				4000
#define MMMMXCVI		4096
#define CCCLXV			365
#define CCCLXVI			366

#define imprimere 	printf
#define liberare 		free
#define memoriae_allocare	malloc
#define exire				exit

#define interior 		static 
#define hic_manens 	static
#define universalis static

#define FILE FILUM

nomen insignatus character	i8;
nomen insignatus brevis 		i16;
nomen insignatus integer   	i32;
nomen insignatus longus longus	i64;

nomen signatus character    s8;
nomen signatus brevis   	 	s16;
nomen signatus integer     	s32;
nomen signatus longus longus	s64;

nomen fluitans          		f32;
nomen duplex         				f64;

nomen integer            		b32;

nomen size_t 								memoriae_index;

#endif /* LATINA_H */


/* ================= ex include/piscina.h ================= */
#ifndef PISCINA_H
#define PISCINA_H

/* PiscinaNotatio - nota pro mark/reset pattern
 * Captat statum piscinam ut postea reficere possit
 */
nomen structura SilvaPiscinaNotatio {
    vacuum*        alveus_nunc;   /* Index ad alveum currentem */
    memoriae_index positus;       /* Offset in alveo */
} SilvaPiscinaNotatio;

SilvaPiscina*
silva_piscina_generare_dynamicum (
		constans character* piscinae_titulum,
			  memoriae_index  mensura_alvei_initia);


/* ===============================================
 * Destructio
 * =============================================== */

vacuum
silva_piscina_destruere (
		SilvaPiscina* piscina);


/* ===============================================
 * Allocatio - fatalis si fallit
 * =============================================== */

static vacuum*
silva_piscina_allocare (
						 SilvaPiscina* piscina,
		memoriae_index  mensura);

static vacuum*
silva_piscina_allocare_ordinatum (
						 SilvaPiscina* piscina,
		memoriae_index  mensura,
		memoriae_index  ordinatio);


/* ===============================================
 * Quaestio
 * =============================================== */

memoriae_index
silva_piscina_summa_usus (
		constans SilvaPiscina* piscina);

memoriae_index
silva_piscina_summa_apex_usus (
		constans SilvaPiscina* piscina);

#endif

/* ================= ex include/chorda.h ================= */
#ifndef CHORDA_H
#define CHORDA_H


/* ==================================================
 * Constructores
 * ================================================== */

static SilvaChorda
silva_chorda_ex_literis (
		constans character* cstr,
							 SilvaPiscina* piscina);

static SilvaChorda
silva_chorda_transcribere (
		 SilvaChorda  s,
		SilvaPiscina* piscina);


/* ==================================================
 * Divisio
 * ================================================= */

nomen structura {
	SilvaChorda* elementa;
	i32     numerus;
} silva_chorda_fissio_fructus;

static s32
silva_chorda_comparare (
		SilvaChorda a,
		SilvaChorda b);

#endif /* CHORDA_H */

/* ================= ex include/chorda_aedificator.h ================= */
#ifndef CHORDA_AEDIFICATOR_H
#define CHORDA_AEDIFICATOR_H

/* ==================================================
 * ChordaAedificator - Accumulatio efficax chordarum
 *
 * Ad chordas aedificandas quando magnitudo finalis ignota.
 * Destinatus ad generationem texti structurati:
 * - INI/JSON/XML aedificatores (casus primarii)
 * - Formatatio diariorum
 * - Generatio texti ex structuris datorum
 *
 * EXEMPLUM:
 *   				   Piscina* p = piscina_generare_dynamicum("probatio", 4096);
 *   ChordaAedificator* a = chorda_aedificator_creare(p, 256);
 *
 *   chorda_aedificator_appendere_literis(a, "nomen");
 *   chorda_aedificator_appendere_character(a, ':');
 *   chorda_aedificator_appendere_integer(a, 42);
 *   chorda_aedificator_appendere_lineam_novam(a);
 *
 *   chorda fructus = chorda_aedificator_finire(a);
 *   piscina_destruere(p);
 *
 * PROPRIETATES:
 * - Crescentia automatica capacitatis (duplicatur quando plenus)
 * - Indentationis observatio (pro impressione pulchra)
 * - Effugium specificum formati (JSON effugium inclusum)
 * - Reutilizabilis per reset()
 * - Omnes allocationes ex Piscina data
 *
 * ================================================== */

nomen structura SilvaChordaAedificator SilvaChordaAedificator;

/* ==================================================
 * Creatio / Destructio
 * ================================================== */

static SilvaChordaAedificator*
silva_chorda_aedificator_creare (
	       SilvaPiscina* piscina,
	memoriae_index  capacitas_initialis);


/* ==================================================
 * Appendere - Singularis Character
 * ================================================== */

static b32
silva_chorda_aedificator_appendere_character (
	SilvaChordaAedificator* aedificator,
	        character  c);


/* ==================================================
 * Appendere - Chordae (Chordae et C-chordae)
 * ================================================== */

static b32
silva_chorda_aedificator_appendere_literis (
	 SilvaChordaAedificator* aedificator,
	constans character* cstr);

static b32
silva_chorda_aedificator_appendere_chorda (
	SilvaChordaAedificator* aedificator,
	           SilvaChorda  s);

/* finire: converte aedificatorem ad chordam
 * Transfert dominium chordae accumulatae ad vocantem.
 * Aedificator destruitur post finire.
 * Vocans debet finaliter deallocare per piscinam. */
static SilvaChorda
silva_chorda_aedificator_finire (
	SilvaChordaAedificator* aedificator);


/* ==================================================
 * Constantae Configurationis
 * ================================================== */

/* CHORDA_AEDIFICATOR_INDENTATIO_SPATIA
 * Numerus spatiorum per gradum indentationis (typice 2 vel 4) */
#define CHORDA_AEDIFICATOR_INDENTATIO_SPATIA II


#endif /* CHORDA_AEDIFICATOR_H */

/* ================= ex include/friatio.h ================= */
#ifndef FRIATIO_H
#define FRIATIO_H

/* ==================================================
 * Friatio - Functiones Friationis (Hash Functions)
 *
 * "Friare" = to crumble, break into pieces
 * "Friatio" = the act of crumbling
 *
 * Continet:
 * - Functiones non-cryptographicae (pro tabulis dispersis)
 * - Functiones cryptographicae (pro UUIDs, checksums, etc.)
 * ================================================== */

/* ==================================================
 * Functiones Non-Cryptographicae (32-bit)
 *
 * Pro tabulis dispersis et distributione rapida.
 * NON securae pro cryptographia!
 * ================================================== */

/* FNV-1a (Fowler-Noll-Vo)
 * Rapida, bona distributio, simplex
 * Offset basis: 2166136261
 * Prime: 16777619
 */
static i32
silva_friatio_fnv1a(
    SilvaChorda clavis);

/* ==================================================
 * SHA-1 (Secure Hash Algorithm 1)
 *
 * Cryptographica (sed OBSOLETA pro securitate!)
 * Producit 160 bits (20 bytes) digest
 * Usus: UUIDv5, checksums legacy, non pro securitate nova
 * ================================================== */

#define SHA1_DIGEST_MENSURA   XX    /* 20 bytes */
#define SHA1_BLOCK_MENSURA    LXIV  /* 64 bytes */

/* ==================================================
 * SHA-256 (Secure Hash Algorithm 256)
 *
 * Cryptographica moderna et secura
 * Producit 256 bits (32 bytes) digest
 * Usus: Securitas, integritas, signaturas digitales
 * ================================================== */

#define SHA256_DIGEST_MENSURA   XXXII  /* 32 bytes */
#define SHA256_BLOCK_MENSURA    LXIV   /* 64 bytes */

#endif /* FRIATIO_H */

/* ================= ex include/tabula_dispersa.h ================= */
#ifndef TABULA_DISPERSA_H
#define TABULA_DISPERSA_H

/* ===================================================
 * Tabula Dispersa - Tabula Friationis per Addressing Apertum
 * "Tabula: series plana, per friationes indexata"
 * ================================================== */

/* Status cuiusque sloti in tabula
 * "Status sloti" */
nomen enumeratio {
    SLOT_VACUUM    = 0, /* Slotus vacuus */
    SLOT_OCCUPATUM = 1, /* Clavis-valor par continens */
    SLOT_DELETUM   = 2  /* Deletum (tumulus) */
} SilvaSlotusStatus;


/* Genus functionis friationis
 * Clavis chordae capit, friationes i32 reddit */
nomen i32 (*TabulaFriatio)(SilvaChorda clavis);


/* Genus functionis comparationis
 * 0 reddit si aequalis, <0 si a<b, >0 si a>b */
nomen s32 (*TabulaComparatio)(SilvaChorda a, SilvaChorda b);

nomen structura {
    SilvaSlotusStatus  status;
          SilvaChorda  clavis;
          vacuum* valor;
             i32  hash_cachatum;
} SilvaSlotus;

/* Ipsa tabula friationis */
nomen structura {
              SilvaSlotus* sloti;
                 i32  capacitas;
                 i32  capacitas_mask;
                 i32  numerus;
                 i32  numerus_deletorum;
             SilvaPiscina* piscina;
       TabulaFriatio  friatio;
    TabulaComparatio  comparatio;
                 f32  factor_maximus;
                 f32  factor_deletorum_maximus;
                 i32  collisiones_totales;
                 i32  distantia_maxima;
} SilvaTabulaDispersa;

/* Iterator ad omnes introitus ambulandos */
nomen structura {
    SilvaTabulaDispersa* tabula;
               i32  positus_currens;
} SilvaTabulaIterator;


/* ==================================================
 * Creatio
 * ================================================== */

static SilvaTabulaDispersa*
silva_tabula_dispersa_creare (
             SilvaPiscina* piscina,
                 i32  capacitas_initialis,
       TabulaFriatio  friatio,
    TabulaComparatio  comparatio);

/* Commoditas: crea cum comparatione chordae (signum) */
static SilvaTabulaDispersa*
silva_tabula_dispersa_creare_chorda (
    SilvaPiscina* piscina,
        i32  capacitas_initialis);

/* ==================================================
 * Insertio / Quaestio / Deletio
 * ================================================== */

/* Clavis-valor par inserere
 * VERUM reddit in successu, FALSUM in allocationis fractura
 * Valor existens renovat si clavis iam existit */

static b32
silva_tabula_dispersa_inserere (
    SilvaTabulaDispersa* tabula,
            SilvaChorda  clavis,
            vacuum* valor);


/* Clavis per quaestio
 * VERUM reddit et *valor_exitus statuit si inventum
 * FALSUM reddit si non inventum */
static b32
silva_tabula_dispersa_invenire (
    SilvaTabulaDispersa*  tabula,
            SilvaChorda   clavis,
            vacuum** valor_out);

/* Celeris existentiae verificatio */
static b32
silva_tabula_dispersa_continet (
    SilvaTabulaDispersa* tabula,
            SilvaChorda  clavis);

/* Introitum delere per clavis
 * VERUM reddit si inventum et deletum, FALSUM alioquin */
static b32
silva_tabula_dispersa_delere (
    SilvaTabulaDispersa* tabula,
            SilvaChorda  clavis);

/* =================================================
 * Statisticae / Diagnostica
 * ================================================== */

static f32
silva_tabula_dispersa_factor_oneris (
    SilvaTabulaDispersa* tabula);

static f32
silva_tabula_dispersa_factor_deletorum (
    SilvaTabulaDispersa* tabula);


/* =================================================
 * Functiones Friationis (in bibliotheca separata ideale)
 * ================================================== */

static i32
tabula_friare_fnv1a (
    SilvaChorda clavis);

#endif /* TABULA_DISPERSA_H */


/* ================= ex include/xar.h ================= */
/* xar.h - "Codex Tabulae Exponentialis"
 *
 * Tabula crescens sine reallocatio
 *
 * PHILOSOPHIA:
 * - Segmenta duplicant magnitudinem: primi, primi, 2×primi, 4×primi, 8×primi...
 * - Duo prima segmenta eiusdem magnitudinis (optimizatio pro parvis tabulis)
 * - Elementa numquam movent (indices stabiles permanent)
 * - Omnia ex piscina allocata (destructio simplex)
 * - Algorithmus O(1) pro locatio indicis
 *
 * USUS:
 *   Xar* xar = xar_creare(piscina, sizeof(MeusTypus));
 *   MeusTypus* elem = xar_addere(xar);
 *   MeusTypus* get = xar_obtinere(xar, index);
 */

#ifndef XAR_H
#define XAR_H

/* Constantae */
#define XAR_MAXIMUS_SEGMENTORUM     LXIV    /* 64 segmenta maxima */
#define XAR_PRIMUS_SEGMENTUM        XVI     /* 16 elementa in primo segmento */

/* Vexilla */
#define XAR_VEXILLUM_ORDINARIUS     ZEPHYRUM    /* Ordinarius: memoria ad zephyrum */
#define XAR_VEXILLUM_SINE_ZEPHYRUM  I           /* Non zephyrum facere memoriam in allocatio */

/* XarIterator - Iterator pro ambulatione omnium elementorum
 */
nomen structura SilvaXarIterator {
	constans structura SilvaXar* xar;     /* Tabula iterata */
			               i32  index_currens;         /* Index currens */
				             i32  index_segmenti;        /* Segmentum currens */
			               i32  offset_in_segmento;    /* Offset in segmento */
		            vacuum* basis_segmenti;        /* Basis segmenti currentis */
			               i32  finis_segmenti;        /* Finis segmenti currentis */
} SilvaXarIterator;

/* XarComparator - Index functionis pro comparationibus
 * Redde: -1 si a < b, 0 si a == b, +1 si a > b
 */
nomen s32 (*SilvaXarComparator)(constans vacuum* a, constans vacuum* b);

/* ========================================================================
 * FUNCTIONES PRINCIPALES
 * ======================================================================== */

/* Xar Creare - Creare novum xar
 * "Genesis tabulae exponentialis"
 */
static SilvaXar*
silva_xar_creare(
		SilvaPiscina* piscina,
		 		i32  magnitudo_elementi);

/* Xar Creare Cum Vexillis
 * "Creare cum omnibus optionibus"
 */
static SilvaXar*
silva_xar_creare_cum_vexillis(
		SilvaPiscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi,
        i32  vexilla);

/* ========================================================================
 * LOCATIO ET ACCESSUS
 * ======================================================================== */

/* Xar Obtinere - Obtinere elementum ad indicem
 * "Obtinere elementum ad indicem"
 *
 * Redde: Index ad elementum, vel NIHIL si invalidus
 */
vacuum*
silva_xar_obtinere(
		constans SilvaXar* xar,
		         i32  index);

/* ========================================================================
 * ADDITIO
 * ======================================================================== */

/* Xar Addere - Addere elementum ad finem
 * "Addere elementum ad finem"
 *
 * Redde: Index ad elementum novum, vel NIHIL si allocatio fracta
 */
static vacuum*
silva_xar_addere(
		SilvaXar* xar);

/* ========================================================================
 * UTILITAS
 * ======================================================================== */

/* Xar Numerus - Obtinere numerum elementorum
 * "Obtinere numerum elementorum"
 */
i32
silva_xar_numerus(
		constans SilvaXar* xar);

/* Xar Vacare - Vacuare omnia elementa
 * "Vacuare omnia elementa (segmenta manent allocata)"
 */
static vacuum
silva_xar_vacare(
		SilvaXar* xar);

/* Xar Truncare - Truncare ad magnitudinem minorem
 * "Truncare ad magnitudinem minorem"
 */
static vacuum
silva_xar_truncare(
		SilvaXar* xar,
		i32  numerus_novus);


/* ========================================================================
 * ORDINATIO ET MANIPULATIO
 * ======================================================================== */

/* Xar Ordinare - Ordinare in loco
 * "Ordinare tabulam in loco usans comparatorem"
 *
 * Usans selection sort (simplex, O(n²), sed stabilis pro parvis tabulis)
 */
static vacuum
silva_xar_ordinare(
		          SilvaXar* xar,
		SilvaXarComparator  comparator);

/* Xar Mutare - Mutare duo elementa
 * "Mutare elementa ad indices"
 *
 * Redde: VERUM si successus, FALSUM si indices invalidi
 */
static b32
silva_xar_mutare(
		SilvaXar* xar,
		i32  index_a,
		i32  index_b);

#endif /* XAR_H */

/* assertio derivae: silva.h SilvaXar.segmenta[64] ==
 * XAR_MAXIMUS_SEGMENTORUM internum */
typedef char silva_assertio_xar_segmentorum[
    (XAR_MAXIMUS_SEGMENTORUM == 64) ? 1 : -1];

/* ================= ex silva/fontes/silva_token.h ================= */
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
    SilvaChorda* via;              /* via plagulae interned; titulus syntheticus pro API */
    b32     est_syntheticus;  /* VERUM: textus ex API, non plagula vera */
    b32     est_custos;       /* plagula custodita (#ifndef X / #define X / #endif) */
    SilvaChorda* custos_titulus;   /* titulus macro custodis; NIHIL si non custodita */
} SilvaFons;


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
 * Constructores (unus per genus originis)
 * ================================================== */

/* Lexema fontis (stratum 0) */
SilvaToken*
silva_token_ex_fonte (
    SilvaPiscina*         piscina,
    SilvaLexemaGenus genus,
    SilvaChorda           valor,
    s32              byte_offset,
    i32              linea,
    i32              columna,
    s32              fons_index);

/* Lexema ex expansione macro: campos lexicales a corpore capit (def-site),
 * invocationem et caecationem in origine servat */
SilvaToken*
silva_token_ex_expansione (
    SilvaPiscina*       piscina,
    SilvaToken*    corpus,
    SilvaToken*    invocatio,
    SilvaChorda*        nomen_macro,
    SilvaCaecatio* caecatio);

/* Lexema ex ## - textus novus, parentes ambo servati */
SilvaToken*
silva_token_ex_pasta (
    SilvaPiscina*         piscina,
    SilvaLexemaGenus genus,
    SilvaChorda           valor,
    SilvaToken*      sinister,
    SilvaToken*      dexter,
    SilvaChorda*          nomen_macro,
    SilvaCaecatio*   caecatio);

/* Lexema ex # - littera chordae synthetica */
SilvaToken*
silva_token_ex_stringificatione (
    SilvaPiscina*    piscina,
    SilvaChorda      valor,
    SilvaToken* primus,
    SilvaChorda*     nomen_macro);

/* Lexema ex definitione API iniecta (fons syntheticus) */
SilvaToken*
silva_token_ex_api (
    SilvaPiscina*         piscina,
    SilvaLexemaGenus genus,
    SilvaChorda           valor,
    SilvaChorda*          nomen_macro,
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
    SilvaPiscina*       piscina,
    SilvaCaecatio* parens,
    SilvaChorda*        titulus);

/* Continetne caecatio titulum? (comparatio contentus) */
b32
silva_caecatio_continet (
    SilvaCaecatio* caecatio,
    SilvaChorda         titulus);


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

/* ================= ex silva/fontes/silva_lexema.h ================= */
/* silva_lexema.h - Lexator totalis silvae
 *
 * Proprietates (vide silva-spec-v2.md §8.2, simulatio ①):
 *   - TOTALIS: quaelibet series octetorum lexatur (OCTETUS_IGNOTUS,
 *     genera IMPERFECTUM) - numquam frangit, numquam octetos perdit
 *   - Scanner triviae UNUS + transitus attachmenti regulis actus
 *     (trailing = trivia eiusdem lineae usque ad primam NOVA_LINEA
 *     inclusive; leading = reliqua)
 *   - initium_lineae = lineae LOGICAE (CONTINUATIO-conscium)
 *   - Laminae \<nl> INTRA lexemata: valor mundus + scissurae
 *   - Fidelitas octetorum: emittere(lexare(x)) == x pro QUOLIBET x
 *   - Verba clausa ANGLICA solum: latina.h per expansorem fluit
 *     (definitiones compilatae), non per lexatorem
 */

#ifndef SILVA_LEXEMA_H
#define SILVA_LEXEMA_H

/* Lexare fontem: reddit Xar de SilvaToken* - lexemata vera (cum EOF ultimo)
 * cum triviis attachatis in spatia_ante/spatia_post.
 * Fons NON copiatur: valores lexematum in textum fontis spectant,
 * ergo textus vivere debet quamdiu lexemata vivunt. */
SilvaXar*
silva_lexare (
    SilvaPiscina*            piscina,
    constans character* fons,
    i32                 mensura,
    s32                 fons_index);

/* Fluxus crudus: lexemata vera ET trivia interposita, ordine fontis.
 * Pro probationibus et instrumentis; silva_lexare hoc + attachmentum est. */
SilvaXar*
silva_lexare_cruda (
    SilvaPiscina*            piscina,
    constans character* fons,
    i32                 mensura,
    s32                 fons_index);

/* Transitus attachmenti: fluxus crudus -> lexemata vera cum triviis.
 * Functio pura super regulas possessionis - separatim probabilis. */
SilvaXar*
silva_spatia_attachere (
    SilvaPiscina* piscina,
    SilvaXar*     cruda);

/* Emittere fluxum attachatum ad octetos - oraculum fidelitatis.
 * Scissurae reinseruntur; trivia verbatim. */
SilvaChorda
silva_lexemata_emittere (
    SilvaPiscina* piscina,
    SilvaXar*     lexemata);

/* Primitivum emissionis lexematis UNIUS in aedificatorem datum:
 * trivia ante + valor (scissurae reinsertae) + trivia post.
 * Commune fluxui supra et emissori arborum (silva_scribere,
 * Phase 5) - una implementatio fidelitatis, non duae. */
vacuum
silva_lexema_emittere_in (
    SilvaChordaAedificator* aed,
    SilvaToken*        token);

#endif /* SILVA_LEXEMA_H */

/* ================= ex silva/fontes/silva_expandere.h ================= */
/* silva_expandere.h - Expansor silvae (Phase 2)
 *
 * Chunk A: acta (journal) + directivae + definitiones macro.
 * Chunk B: generationes expansionis (caecationes, S13).
 * Chunk C: # et ## in substitutione.
 * Chunk D: includenda + regiones conditionales + custodes.
 *
 * Decisiones (silva-spec-v2.md §8.1/S10 + §10.2, simulationes ①②):
 *   - ACTA pro segmentis: historia status macro = index appendix-solus
 *     eventorum (fons, linea, ±definitio, conditio_id). Tabula viva =
 *     via defalta (default track). macros_ad_lineam = replay actorum
 *     in piscinam vocantis. (TabulaDispersa exemplar non habet - R2.)
 *   - Directiva = lexema CANCELLUM cum initium_lineae VERO: proprietas
 *     LOCALIS (classis defectus arbor2 hic irrepraesentabilis).
 *   - Corpus macro = LAMINA fluxus lexematum fontis: def-site
 *     provenientia gratis (lexemata immutabilia et communia).
 *   - Functio-similis si '(' IMMEDIATE post titulum (adiacentia cruda:
 *     byte_offset paren == byte_offset tituli + longitudo tituli).
 *   - Duae viae (§10.2): via defalta evaluata pellit statum vivum et
 *     acta; OMNES rami retinentur in arbore regionum (rami non sumpti
 *     ut laminae crudae partitionis - quodque lexema uni ramo).
 *   - Custodes (est_custos): plagula custodita nullam regionem gignit -
 *     interior incondicionaliter processatur, custos in SilvaFons.
 *   - Includenda per API praebentur (silva plagulas numquam aperit);
 *     includendum ignotum = "discens" (via memorata, processio pergit).
 */

#ifndef SILVA_EXPANDERE_H
#define SILVA_EXPANDERE_H

/* ==================================================
 * Definitio macro
 * ================================================== */

nomen structura {
    SilvaChorda*  titulus;       /* nomen macro */
    b32      est_functio;   /* '(' adiacens post titulum */
    b32      est_variadica; /* ... in parametris (C99 - signatum) */
    SilvaXar*     parametra;     /* Xar de chorda* (tituli parametrorum); NIHIL si nulla */
    SilvaXar*     corpus;        /* Xar de SilvaToken* - lamina fluxus fontis */
    s32      fons_index;    /* -1 si ex API */
    i32      linea_def;     /* 0 si ignota */
    b32      ex_api;
} SilvaMacroDef;


/* ==================================================
 * Acta (journal) - historia status macro
 * ================================================== */

nomen enumeratio {
    SILVA_EVENTUM_DEFINITIO = 0,
    SILVA_EVENTUM_DELETIO
} SilvaEventumGenus;

nomen structura {
    SilvaEventumGenus genus;
    s32               fons_index;
    i32               linea;
    SilvaChorda*           titulus;
    SilvaMacroDef*    def;          /* NIHIL pro DELETIO */
    i32               conditio_id;  /* 0 = incondicionale */
    s32               positus;      /* index in fluxu reliquorum a quo
                                     * eventum valet (status ad punctum -
                                     * semantica cpp pro #undef medio) */
} SilvaEventum;


/* ==================================================
 * Chunk D - Regiones conditionales (duae viae, §10.2)
 *
 * Regio = #if/#ifdef/#ifndef ... (#elif/#else ...)* #endif.
 * Via defalta: rami ordine evaluantur; primus verus sumitur -
 * lexemata eius normaliter processantur (definitiones cum
 * conditio_id ramī). Rami NON sumpti laminas crudas retinent
 * (partitio: quodque lexema uni ramo pertinet).
 * ================================================== */

nomen structura SilvaRegio SilvaRegio;

nomen structura {
    SilvaRamusGenus genus;
    i32         conditio_id;    /* index+1 in exp->rami; 0 numquam */
    SilvaXar*        directiva;      /* lexemata lineae directivae */
    SilvaXar*        expressio;      /* lexemata conditionis; NIHIL pro else */
    b32         est_evaluatum;  /* evaluatio temptata et successit */
    s64         valor;          /* fructus evaluationis */
    b32         est_sumptum;    /* sumptus in via defalta */
    b32         est_numquam;    /* #if 0 idioma (litteralis falsa) */
    SilvaXar*        lexemata_cruda; /* ramus non sumptus: lamina cruda; NIHIL si sumptus */
    SilvaRegio* regio;          /* regio continens */
    s32         corpus_initium; /* offset primi lexematis corporis
                                 * (post lineam directivae); -1 si
                                 * corpus vacuum (sim ⑦ C1) */
    s32         corpus_finis;   /* offset post ultimum lexema corporis;
                                 * -1 si vacuum */
} SilvaRamus;

structura SilvaRegio {
    s32         fons_index;
    i32         linea;
    SilvaXar*        rami;           /* Xar de SilvaRamus* */
    SilvaRegio* pater;          /* regio amplectens; NIHIL si suprema */
    SilvaXar*        filiae;         /* Xar de SilvaRegio* (in ramis sumptis) */
    b32         est_imperfecta; /* EOF ante #endif */
    b32         est_ultra_modum; /* profunditas ultra limen (Phase 7):
                                  * NULLUS ramus evaluatur - omnes crudi
                                  * (degradatio determinata; recursio
                                  * cessat, octeti supersunt) */
    SilvaXar*        directiva_finis; /* lexemata lineae #endif; NIHIL si
                                  * imperfecta (sim ⑦ C1). REGIO lineas
                                  * structurales suas POSSIDET (β, sim ⑦
                                  * C2): #if/#elif/#else/#endif numquam
                                  * in directivae_out intrant */
    b32         est_texta;       /* in arborem texta (Phase 7 Chunk B):
                                  * lineae + cruda ex ARBORE emittuntur,
                                  * non ex reinserendis (dominus unus) */
};


/* ==================================================
 * Chunk D - Includenda
 * ================================================== */

nomen structura {
    s32  fons_index;
    SilvaXar* lexemata;      /* fluxus attachatus contenti */
} SilvaIncludendum;

/* Inclusio observata (graphum dependentiarum - "discens") */
nomen structura {
    s32     fons_ex;            /* fons in quo #include stat */
    SilvaChorda* via;                /* via petita */
    s32     fons_ad;            /* fons resolutus; -1 = ignotus */
    b32     est_praetermissa;   /* praetermissa (custos vel profunditas) */
} SilvaInclusio;


/* Extentum invocationis functio-similis STRATI 0 (Phase 5 Chunk B,
 * simulatio ⑥ C5): lamina [nomen .. ')'] fluxus quo invocatio
 * collecta est - octeti invocationis cum triviis argumentorum.
 * Memoratur solum cum lexema nominis origo FONS fert (stratum 0):
 * reconstructio fontis hos octetos emittit ubi arbor lexemata
 * expansa fert. */
nomen structura {
    SilvaToken* invocatio;   /* lexema nominis (origo FONS) */
    SilvaXar*        lamina;      /* Xar de SilvaToken* - [nomen, post ')') */
} SilvaExtentumInvocationis;

structura SilvaExpansio {
    SilvaPiscina*        piscina;
    SilvaXar*            fontes;     /* Xar de SilvaFons (per valorem) */
    SilvaTabulaDispersa* macros;     /* status vivus - via defalta */
    SilvaXar*            acta;       /* Xar de SilvaEventum (per valorem) */
    SilvaXar*            rami;       /* Xar de SilvaRamus* - tabula conditio_id */
    SilvaXar*            regiones;   /* Xar de SilvaRegio* - regiones supremae */
    SilvaTabulaDispersa* includenda; /* via (chorda) -> SilvaIncludendum* */
    SilvaXar*            inclusiones; /* Xar de SilvaInclusio (per valorem) */
    SilvaXar*            extenta;    /* Xar de SilvaExtentumInvocationis
                                 * (per valorem) - invocationes
                                 * functio-similes strati 0 */
    i32             profunditas_includendi;
    s32             fons_api;   /* fons syntheticus "<api>"; -1 = nondum */
    SilvaTabulaDispersa* tabula_activa; /* tabula temporalis expansionis
                                    * positionalis; NIHIL = tabula viva */

    /* ==== Fines (Phase 7 Chunk A - par 8.2). 0 = infinitum. ====
     * Fines expansionem DEGRADANT, numquam totalitatem: limine tacto
     * lexemata reliqua INEXPANSA fluunt (FONS - se ipsa emittunt),
     * ergo arbor completa et reconstructio byte-exacta etiam sub
     * finibus tenent. Defaltae generosae in creare positae. */
    i32 limen_lexematum;     /* fluxus expansus maximus (per generationem) */
    i32 limen_generationum;  /* generationes expansionis maximae */
    i32 limen_includendi;    /* profunditas includendi maxima */
    i32 limen_regionum;      /* profunditas regionum maxima */

    /* Intermissio (SilvaContextus eam ponit; forma cruda ne stratum
     * inferius contextum noscat): FALSUM redditum = intermitte. */
    b32     (*pergere)(vacuum* datum);   /* NIHIL = numquam rogare */
    vacuum*   pergere_datum;

    /* Status finium (productum, non depuratio) */
    b32 expansio_decisa;      /* expansio trunca (limen lexematum aut
                               * generationum tactum) */
    b32 est_intermissa;       /* pergere FALSUM reddidit */
    b32 fines_tactae;         /* quilibet limen tactus */
    i32 profunditas_regionum; /* numerator vivus (transiens) */
};

/* Defaltae finium (tree-sitter habitus: fines generosae SEMPER
 * activae; hospes eas tollere potest - 0 = infinitum) */
#define SILVA_LIMEN_LEXEMATUM_DEFALTUM    1048576
#define SILVA_LIMEN_GENERATIONUM_DEFALTUM C
#define SILVA_LIMEN_INCLUDENDI_DEFALTUM   XXXII
#define SILVA_LIMEN_REGIONUM_DEFALTUM     LXIV

SilvaExpansio*
silva_expansio_creare (
    SilvaPiscina* piscina);

/* Addere fontem; reddit indicem eius */
s32
silva_fons_addere (
    SilvaExpansio*      exp,
    constans character* via,
    b32                 est_syntheticus);


/* Praebere contentum includendum: via -> textus (lexatur statim).
 * Silva plagulas numquam ipsa aperit - vocans contentum praebet.
 * Reddit indicem fontis novi. */
s32
silva_includendum_praebere (
    SilvaExpansio*      exp,
    constans character* via,
    constans character* textus,
    i32                 mensura);


/* ==================================================
 * Processio directivarum (Chunk A + D)
 *
 * Ambulat fluxum attachatum (ex silva_lexare); directivas
 * define/undef/include/if/ifdef/ifndef/elif/else/endif tractat:
 * tabulam vivam et acta renovat, regiones aedificat, includenda
 * resoluta in loco inserit. Custodes ante ambulationem deteguntur
 * (plagula custodita nullam regionem gignit). Reddit fluxum
 * RELIQUUM (lexemata non in directivis consumptis); directivae
 * ipsae in *directivae_out (Xar de Xar de SilvaToken* - una linea
 * logica per introitum) si non NIHIL.
 * ================================================== */

SilvaXar*
silva_expansio_directivas_processare (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    SilvaXar**          directivae_out);

/* Quaerere definitionem in tabula viva; NIHIL si absens */
SilvaMacroDef*
silva_expansio_quaerere (
    SilvaExpansio* exp,
    SilvaChorda         titulus);

/* ==================================================
 * Iniectio macro per API (fons syntheticus "<api>")
 *
 * Via definitionum compilatarum (latina.h!) et definitionum
 * vocantis. Corpus textus lexatur in piscinam - lexemata
 * corporis sunt lexemata FONTIS synthetici (provenientia
 * plena per catenas normales).
 * ================================================== */

b32
silva_macro_addere (
    SilvaExpansio*      exp,
    constans character* titulus,
    constans character* corpus);

/* parametra: series NIHIL-terminata; "..." = variadica */
b32
silva_macro_functio_addere (
    SilvaExpansio*       exp,
    constans character*  titulus,
    constans character** parametra,
    constans character*  corpus);


/* ==================================================
 * Prospectus macro - oraculum GLR (brainstorm v2:
 * "macro lookahead" pro prioritate furcarum)
 * ================================================== */

nomen structura {
    SilvaLexemaGenus genus;          /* primi lexematis corporis; EOF si vacuum */
    b32              est_vacuum;     /* corpus vacuum */
    b32              est_recursivum; /* primum lexema ipsum nomen macro est */
} SilvaProspectus;

/* Reddit FALSUM si titulus non est macro */
b32
silva_expansio_prospectare (
    SilvaExpansio*   exp,
    SilvaChorda           titulus,
    SilvaProspectus* prospectus_out);

/* Status macro ad (fons, linea): replay actorum in piscinam datam.
 * Tabula chorda->SilvaMacroDef*. Eventa ordine appendicis replicantur
 * usque ad primum eventum eiusdem fontis cuius linea >= linea data. */
SilvaTabulaDispersa*
silva_expansio_macros_ad_lineam (
    SilvaExpansio* exp,
    SilvaPiscina*       piscina,
    s32            fons_index,
    i32            linea);


/* ==================================================
 * Chunk B - Generationes expansionis
 *
 * Generatio una = gradus expansionis C89 plenus (S13):
 * argumenta PLENE prae-expansa intra gradum, substitutio,
 * caecatio extensa (Prosser); RESCAN = generatio proxima.
 * Terminatio per caecationes (finis semanticus, non cap).
 *
 * SIMPLIFICATIO PROSSER PERMANENS (disposita Phase 7 Chunk C):
 * caecatio argumentorum HS_call fertur, non HS_call∩HS_rparen
 * exacta. Classis divergentiae parenthesibus IMPARIBUS in
 * corporibus macrorum functio-similium eget; lustrum solarii
 * (2.3MB C89 veri, 2026-07-03) NULLAM invenit - super-caecatio
 * classis exoticae accepta, evidentia memorata (phase-log).
 * ================================================== */

/* Una generatio: fluxus novus; *mutatum_out VERUM si quid expansum */
SilvaXar*
silva_expansio_generatio (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    b32*           mutatum_out);

/* Expansio ad fixum CONTRA STATUM VIVUM: generationes iteratae dum
 * mutationes. Pro seriebus sine positione (argumenta, expressiones
 * conditionum) - status macro unus per totam seriem.
 * strata_out (si non NIHIL): Xar de Xar* - exitus CUIUSQUE
 * generationis mutantis, ordine (strata modeli stratorum). */
SilvaXar*
silva_expansio_expandere (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    SilvaXar**          strata_out);

/* Expansio POSITIONALIS fluxus reliquorum (semantica cpp fidelis):
 * eventa actorum per positus applicantur dum fluxus ambulatur, ergo
 * quodque lexema statum macro SUI PUNCTI videt (#undef/redefinitio
 * media honorantur). reliqua DEBET esse fluxus proxime redditus a
 * silva_expansio_directivas_processare huius exp. Haec est via
 * plagularum integrarum; expandere supra pro seriebus solutis. */
SilvaXar*
silva_expansio_expandere_reliqua (
    SilvaExpansio* exp,
    SilvaXar*           reliqua,
    SilvaXar**          strata_out);

i32
silva_fontes_numerus (constans SilvaExpansio* exp);

/* Via fontis; NIHIL si extra fines (synthetici titulos reddunt) */
constans SilvaChorda*
silva_fons_via (constans SilvaExpansio* exp, s32 fons_index);

i32
silva_inclusiones_numerus (constans SilvaExpansio* exp);

b32
silva_inclusio_vista (constans SilvaExpansio* exp, i32 index,
    SilvaInclusioVista* vista_out);

i32
silva_rami_numerus (constans SilvaExpansio* exp);

b32
silva_ramus_vista (constans SilvaExpansio* exp, i32 index,
    SilvaRamusVista* vista_out);

/* Definitiones UT ACTAE (acta = fructus: #undef historiam non
 * delet); O(acta) per vocatum - satis ad mensuram v1 */
i32
silva_macros_numerus (constans SilvaExpansio* exp);

b32
silva_macro_vista (constans SilvaExpansio* exp, i32 index,
    SilvaMacroVista* vista_out);

#endif /* SILVA_EXPANDERE_H */

/* ================= ex silva/fontes/silva_conditio.h ================= */
/* silva_conditio.h - Evaluator expressionum praeprocessoris (#if/#elif)
 *
 * Portatum ex arbor2_conditio_evaluare cum duabus emendationibus:
 *   - s64 pro i64: latina i64 INSIGNATUS est - arbor2 comparationes
 *     signatas insignate computabat (#if -1 < 0 erat FALSUM!)
 *   - expansio plena: operanda 'defined' primum substituuntur
 *     (protecta, numquam expansa), tum expressio TOTA per expansorem
 *     verum expanditur, tum arithmetica evaluatur. arbor2 solum
 *     macros obiecti-similes unius lexematis tractabat.
 *
 * Identificatores post expansionem reliqui = 0 (C89 6.8.1).
 * In errore: 0 redditur et *successus = FALSUM (nullus fragor).
 */

#ifndef SILVA_CONDITIO_H
#define SILVA_CONDITIO_H

/* Estne macro definitum in tabula viva? */
b32
silva_conditio_est_definitum (
    SilvaExpansio* exp,
    SilvaChorda         titulus);

/* Evaluare expressionem constantem praeprocessoris.
 * lexemata: Xar de SilvaToken* (linea directivae post verbum, sine EOF).
 * Reddit valorem (0 = falsum); *successus FALSUM in errore. */
s64
silva_conditio_evaluare (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    b32*           successus);

#endif /* SILVA_CONDITIO_H */

/* ================= ex silva/fontes/silva_nodus.h ================= */
/* silva_nodus.h - Nodus uniformis + valor signatus (spec-v2 §9.1, S21)
 *
 * Nodus = caput fixum + series locorum signatorum per genus. NULLA
 * unio per genus (arbor2 cuique nodo pretium bracchi maximi imputabat
 * - R5). Accessores generati + children() tabulis ducti. Haec est
 * repraesentatio codex-ut-datorum quam solarium petit.
 *
 * PURITAS (S26/S27): constructiones GLR speculativae sunt - pater
 * NUMQUAM in constructione assignatur (furcae vivae communem subarborem
 * habere possunt); passus post-acceptum eum figit. Campi spatii/fontis
 * veniunt cum fluxu vero (Phase 4).
 *
 * S32: silva_nodus_ponere signum valoris contra speciem loci probat et
 * semel-tantum-scribere imponit - errores annotationum in prima parsura
 * apparent, non ut arbores corruptae.
 */

#ifndef SILVA_NODUS_H
#define SILVA_NODUS_H


/* ==================================================
 * Constructores valorum
 * ================================================== */

SilvaValor silva_valor_nihil (vacuum);
SilvaValor silva_valor_nodus (SilvaNodus* nodus);
SilvaValor silva_valor_token (SilvaToken* token);
SilvaValor silva_valor_index (s32 index);

/* Prospectus super Xar existentem (mensura = numerus currens) */
SilvaValor silva_valor_lista (SilvaXar* lista);

/* Lista nova vacua (Xar de SilvaValor, prospectus 0) */
SilvaValor silva_valor_lista_nova (SilvaPiscina* piscina);

/* Appendere PURUM (A½): prospectus alieni numquam laeduntur. Si
 * prospectus ad finem vivum repositorii stat, in loco appendit (O(1),
 * casus communis); alioquin furca divergens praefixum suum in
 * repositorium recens copiat. Prospectum NOVUM reddit (mensura + 1);
 * nihil-valorem in errore aut inputo non-lista. */
SilvaValor
silva_valor_lista_appendere (
    SilvaPiscina*   piscina,
    SilvaValor lista,
    SilvaValor elementum);

/* Numerus elementorum PROSPECTUS (0 si non lista) */
i32 silva_valor_lista_numerus (SilvaValor lista);

/* Elementum intra prospectum (NIHIL extra mensuram prospectus) */
SilvaValor* silva_valor_lista_obtinere (SilvaValor lista, i32 index);


/* ==================================================
 * Nodus
 * ================================================== */

/* Creare nodum: loci omnes SILVA_VALOR_NIHIL */
SilvaNodus*
silva_nodus_creare (
    SilvaPiscina* piscina,
    s32      genus,
    i32      numerus_locorum);

/* Ponere valorem in locum - S32: signum contra speciem probatur,
 * semel-tantum scribere imponitur. Reddit FALSUM in violatione
 * (et nihil scribit). */
b32
silva_nodus_ponere (
    SilvaNodus*       nodus,
    i32               locus,
    SilvaValor        valor,
    SilvaLocusSpecies species);

/* Appendere in locum listae (creat listam si NIHIL) */
b32
silva_nodus_appendere (
    SilvaPiscina*          piscina,
    SilvaNodus*       nodus,
    i32               locus,
    SilvaValor        valor,
    SilvaLocusSpecies species);

/* Congruitne signum valoris cum specie loci? */
b32
silva_valor_congruit (
    SilvaValor        valor,
    SilvaLocusSpecies species);

/* Liberi nodales: NODUS loci + elementa NODUS listarum, ordine locorum.
 * Tabulis non eget - valores signati sunt. Xar de SilvaNodus*. */
SilvaXar*
silva_nodus_liberi (
    SilvaPiscina*           piscina,
    constans SilvaNodus* nodus);

#endif /* SILVA_NODUS_H */

/* ================= ex silva/fontes/silva_tabulae.h ================= */
/* silva_tabulae.h - Contractus tabularum coctarum (runtime)
 *
 * Typi quos plagulae GENERATAE (silva_tabulae_*.c, per silva_coquere)
 * implent et quos motor GLR (Phase 4) legit. Series planae statica -
 * nulla constructio in tempore currendi (lapifex tabulas ~7s in
 * tempore currendi construebat; silva eas in fontem coquit).
 *
 * §8.2: introitus actionum campum `pretium` ferunt et genus
 * SILVA_TAB_ACTIO_RECUPERARE existit - forma parata pro recuperatione
 * GSS pretio-ducta (tree-sitter modo). Introitus RECUPERARE emittuntur
 * cum opus recuperationis veniat; pretium = 0 pro actionibus normalibus.
 *
 * NB: prospectus -1 = $ (finis); terminale EOF etiam ut symbolum
 * explicitum existit (productio augmentata id transponit). Ambo
 * verbatim cocta; compositio EOF<->$ est res adaptatoris Phase 4.
 */

#ifndef SILVA_TABULAE_H
#define SILVA_TABULAE_H

#endif /* SILVA_TABULAE_H */

/* ================= ex silva/fontes/silva_tabulae_sceleti.h ================= */
/* silva_tabulae_sceleti.h - GENERATUM per silva_coquere ex grammatica/sceletum.stml
 * NOLI MANU MUTARE - regenera per silva/generare.sh.
 */

#ifndef SILVA_TABULAE_SCELETI_H
#define SILVA_TABULAE_SCELETI_H

externus constans SilvaTabulaCocta SILVA_SCELETUM_TABULA;
externus constans SilvaRegistrumCoctum SILVA_SCELETUM_REGISTRUM;

/* Genera nodorum (registrum unum: grammatica + extra) */
enumeratio {
    SILVA_SCELETUM_GENUS_DECLARATIO = 0,
    SILVA_SCELETUM_GENUS_TYPUS_PRIMITIVUS = 1,
    SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS = 2,
    SILVA_SCELETUM_GENUS_DECLARATOR_TITULUS = 3,
    SILVA_SCELETUM_GENUS_DECLARATOR_MONSTRATOR = 4,
    SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS = 5,
    SILVA_SCELETUM_GENUS_BINARIUM = 6,
    SILVA_SCELETUM_GENUS_FOLIUM_IDENTIFICATOR = 7,
    SILVA_SCELETUM_GENUS_FOLIUM_INTEGER = 8,
    SILVA_SCELETUM_GENUS_PARENTHESIS = 9,
    SILVA_SCELETUM_GENUS_ERROR = 10,
    SILVA_SCELETUM_GENUS_AMBIGUUS = 11,
    SILVA_SCELETUM_GENUS_CONDITIONALIS = 12,
    SILVA_SCELETUM_GENUS_RAMUS_SUMPTUS = 13,
    SILVA_SCELETUM_GENUS_RAMUS_OMISSUS = 14
};

SilvaValor silva_sceletum_declaratio_typus (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_declaratio_declarator (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_declaratio_tok_terminator (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_typus_primitivus_tok_verbum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_typus_nominatus_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_declarator_titulus_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_declarator_monstrator_tok_stella (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_declarator_monstrator_internum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_sententia_expressionis_expressio (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_sententia_expressionis_tok_terminator (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_binarium_sinister (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_binarium_tok_operator (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_binarium_dexter (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_folium_identificator_tok_valor (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_folium_integer_tok_valor (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_parenthesis_tok_apertum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_parenthesis_internum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_parenthesis_tok_clausum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_error_tokens (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ambiguus_interpretationes (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ambiguus_canonica (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_conditionalis_rami (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_conditionalis_finis (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_sumptus_directiva (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_sumptus_contentum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_sumptus_conditio_id (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_omissus_directiva (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_omissus_cruda (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_omissus_conditio_id (constans SilvaNodus* nodus);

/* Constructio ex reductione GLR: PURA (S26) - allocat
 * et implet solum; pater post-acceptum (S27) */
SilvaValor silva_sceletum_construere (
    SilvaPiscina*             piscina,
    s32                  productio,
    constans SilvaValor* valores);

/* Fabrica ambigui: involucrum AMBIGUUS pro motore
 * GLR (interpretationes = lista valorum, canonica = index
 * spinae canonicae) */
SilvaValor silva_sceletum_ambiguum_fabricare (
    SilvaPiscina*   piscina,
    SilvaValor interpretationes,
    s32        canonica);

#endif /* SILVA_TABULAE_SCELETI_H */

/* ================= ex silva/fontes/silva_glr.h ================= */
/* silva_glr.h - Motor GLR super tabulas coctas (Phase 4)
 *
 * Compositio (spec-v2 par. 3.1): sceletum lapifex_glr (GSS + enumeratio
 * viarum + worklist) + refinamenta arbor2 (duo passus per lexema,
 * exhaustio reducendorum, fusio in fronte translationis) + SilvaValor
 * signatus + constructio generata per callback.
 *
 * Motor tabulae-ignarus est: tabulam coctam (SilvaTabulaCocta) et
 * constructorem (subscriptio silva_*_construere generata) accipit -
 * grammaticam numquam novit. Dispatch unus ex ambobus passibus (par 9.4).
 *
 * REDUCTIONES: nodus quisque reductione creatus RECENS est (nulla fusio
 * intra passum reducendi - vide phase-log Phase 4 INTENTIO: fusio
 * intra-passum lapifex cum dedup (nodus, productio) vias per marginem
 * novum amittere potest). Exhaustio FIFO = undae latitudinis-primae;
 * fusio semel per lexema in fronte translationis (Chunk B).
 *
 * MEMORIA (duae piscinae, par 3.1): nodi GSS et apparatus in piscina
 * motoris (effimera per parsuram); arbores per constructorem in
 * piscina_arborum (a vocatore possessa) allocantur.
 *
 * PURITAS (S26/S27): constructor purus; pater numquam in constructione;
 * passus post-acceptum (Chunk C) eum figit.
 */

#ifndef SILVA_GLR_H
#define SILVA_GLR_H

/* Longitudo maxima dextri productionis (in validatione imposita) */
#define SILVA_GLR_DEXTRUM_MAXIMUM 32

/* Terminalia sentinela prospectus */
#define SILVA_GLR_PROSPECTUS_FINIS   (-1)  /* $ - ultra fluxum */
#define SILVA_GLR_TERMINALE_IGNOTUM  (-2)  /* genus sine terminale in tabula */


/* ==================================================
 * Nodus GSS (acervus grapho-structuratus)
 * ================================================== */

nomen structura SilvaGSSNodus SilvaGSSNodus;

structura SilvaGSSNodus {
    s32         status;
    SilvaValor  valor;
    SilvaXar*        praedecessores;   /* SilvaGSSNodus* (dedup in additione) */
    SilvaToken* lexema;           /* lexema quo creatus (diagnostica/fusio) */
};


/* ==================================================
 * Motor
 * ================================================== */

nomen structura {
    constans SilvaTabulaCocta* tabula;
    SilvaGLRConstructor        constructor;
    SilvaGLRFabricaAmbigui     fabrica;
    SilvaPiscina*                   piscina;    /* GSS + apparatus */

    /* Fines (Phase 7 Chunk A - par 8.2). 0 = infinitum. Frons ultra
     * limen = fractura munda segmenti (gubernator nodum ERROR facit -
     * totalitas tenet). Intermissio: pergere FALSUM = desiste;
     * passus_pergendi = intervallum lexematum inter interrogationes. */
    i32       limen_frontis;
    b32     (*pergere)(vacuum* datum);   /* NIHIL = numquam rogare */
    vacuum*   pergere_datum;
    i32       passus_pergendi;

    /* Statisticae parsurae novissimae (etiam in fructum copiatae -
     * numeratores sunt PRODUCTUM, non depuratio: spec-v2 par 12.2) */
    i32 frons_maxima;
    i32 nodi_creati;
    i32 reductiones_factae;
    i32 fusiones;                /* compactiones (aequales + involutae) */
    i32 transmutationes;         /* religationes identitatis post-exhaustum */
    i32 transmutationes_negatae; /* exhaustum non-NODUS - brachium separatum */
    i32 eventa_marginis_novi;    /* idem status, basis alia (classis Farshi) */
} SilvaGLR;

/* Defaltae finium motoris (tree-sitter habitus - semper activae) */
#define SILVA_GLR_LIMEN_FRONTIS_DEFALTUM  4096
#define SILVA_GLR_PASSUS_PERGENDI_DEFALTUM 1024


/* ==================================================
 * Fructus parsurae
 * ================================================== */

nomen structura {
    b32          successus;
    SilvaValor   valor;              /* radix arboris */
    SilvaToken*  lexema_finis;       /* lexema EOF translatum (trivia!) */
    i32          numerus_acceptorum;

    /* Numeratores compactionis (productum queribile) */
    i32          frons_maxima;
    i32          fusiones;
    i32          transmutationes;
    i32          transmutationes_negatae;
    i32          eventa_marginis_novi;

    /* Error (successus == FALSUM) */
    b32          est_error;
    s32          positio;            /* index lexematis ubi fractum */
    s32          terminalis;         /* prospectus (aut IGNOTUM) */
    s32          status;             /* status primus frontis tunc */
    b32          est_ultra_limen;    /* frons limen excessit (Phase 7) */
    b32          est_intermissus;    /* pergere FALSUM reddidit */
} SilvaGLRFructus;


/* ==================================================
 * API
 * ================================================== */

/* Validare tabulam coctam: fines omnium introituum, ordinatio laminarum,
 * longitudines productionum, goto pro quoque LHS reducibili (goto absens
 * = circuitus infinitus, non defectus mundus - arbor2 worklog), et
 * UNICITAS SYMBOLI ACCESSUS (quisque status uno solo symbolo intratur -
 * proprietas LR quam index reducendorum praesumit; hic assertio, non
 * assumptio - spec-v2 par 12.2). Piscina pro spatio effimero.
 * Errores in stderr; reddit FALSUM in tabula invalida. */
b32
silva_glr_tabulam_validare (
    SilvaPiscina*                   piscina,
    constans SilvaTabulaCocta* tabula);

/* Creare motorem. Tabula validatur - NIHIL in invalida.
 * Fabrica REQUISITA (NIHIL = error): genera structuralia generator
 * imponit, ergo quaevis tabula generata fabricam habet. */
SilvaGLR*
silva_glr_creare (
    SilvaPiscina*                   piscina,
    constans SilvaTabulaCocta* tabula,
    SilvaGLRConstructor        constructor,
    SilvaGLRFabricaAmbigui     fabrica);

/* Parsare fluxum lexematum (Xar de SilvaToken*, EOF ultimo - forma
 * silva_lexare). Arbores in piscina_arborum allocantur. Reentrans:
 * status frontis per parsuram localis est. */
SilvaGLRFructus
silva_glr_parsare (
    SilvaGLR*     glr,
    constans SilvaXar* lexemata,
    SilvaPiscina*      silva_piscina_arborum);

/* Terminale coctum pro genere lexematis (SILVA_GLR_TERMINALE_IGNOTUM
 * si genus in tabula non est). Publicum pro probationibus/adaptatoribus. */
s32
silva_glr_terminale_ex_genere (
    constans SilvaTabulaCocta* tabula,
    SilvaLexemaGenus           genus);

#endif /* SILVA_GLR_H */

/* ================= ex silva/fontes/silva_commissio.h ================= */
/* silva_commissio.h - Passus commissionis + oraculum (Phase 4 Chunk C)
 *
 * OMNIS semantica hic consolidatur (spec-v2 par 12, simulatio ⑤):
 * exhaustiones motoris GLR semanticae-vacuae manent; post acceptum
 * arbor UNA ambulatione committitur:
 *   (a) pater figitur (S27) - spina canonica per nodos AMBIGUOS,
 *   (b) prospectus listarum normalizantur (A½ - mensura exacta),
 *   (c) ambigua quae oraculum iam novit COLLABUNTUR (victor super
 *       allocationem copiatur - identitas manet) et eventum in
 *       diarium scribitur (COLLAPSE+JOURNAL, interview ⑤ Q5).
 *
 * POST commissionem arbor immutabilis est PRAETER
 * silva_recanonicare: scientia sera indicem canonicum in loco
 * vertit (spina retexta localiter) - involucrum manet, monstratores
 * observatorum valent, NULLA reparsura (par 8.2).
 *
 * Ambulatio grammaticae-ignara est: formam ambigui (genus +
 * interpretationes/canonica) ex registro cocto PER NOMEN invenit;
 * semantica sola in resolutore vocatoris vivit (manu scripto per
 * grammaticam - M1).
 */

#ifndef SILVA_COMMISSIO_H
#define SILVA_COMMISSIO_H

structura SilvaOraculum {
    SilvaPiscina*        piscina;
    SilvaTabulaDispersa* typi;      /* clavis chorda (copiata) -> VERUM */
};

SilvaOraculum* silva_oraculum_creare (SilvaPiscina* piscina);

/* Titulus in piscinam oraculi COPIATUR (valores lexematum prospectus
 * in fontem sunt - lectio vitae fons->via) */
b32 silva_oraculum_typum_addere (SilvaOraculum* oraculum,
    SilvaChorda titulus);
b32 silva_oraculum_typum_addere_literis (SilvaOraculum* oraculum,
    constans character* titulus);

b32 silva_oraculum_typum_novit (constans SilvaOraculum* oraculum,
    SilvaChorda titulus);

/* Ambulatio commissionis. Oraculum/resolutor NIHIL licent (tunc
 * nulla resolutio - pater + normalizatio solae). NIHIL reddit si
 * tabularium formam ambigui non fert (generator eam imponit).
 * NB "registrum" ut titulus vetitum - macro latina (register)! */
SilvaCommissio*
silva_committere (
    SilvaPiscina*                       piscina,
    SilvaValor                     radix,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaOraculum*        oraculum,
    SilvaResolutor                 resolutor,
    vacuum*                        datum_resolutoris);

/* Scientia sera: ambigua superstitia iterum resolvere. Canonica in
 * loco vertitur + spina localiter retexta; involucrum MANET (arbor
 * post commissionem immutabilis praeter hoc). Numerum versorum
 * reddit. */
i32
silva_recanonicare (
    SilvaCommissio*         commissio,
    constans SilvaOraculum* oraculum,
    SilvaResolutor          resolutor,
    vacuum*                 datum_resolutoris);

#endif /* SILVA_COMMISSIO_H */

/* ================= ex silva/fontes/silva_contextus.h ================= */
/* silva_contextus.h - Contextus hospitis (Phase 7 Chunk A)
 *
 * Obiectum diu vivens et reusabile hospitis (spec-v2 par 8.2): fines
 * (limina dura configurabilia), intermissio (pergere - interrogatio
 * determinata, nulla horologia), lexica (plagulae definitionum ante
 * fontem principalem processatae - latina.h hac via "compiled-in"),
 * praebenda (contentum includendum). RECEPTUM est, non status:
 * quaeque parsura expansionem recentem creat et receptum applicat
 * (silva_parsare_cum_contextu).
 *
 * Praeposita configurationum nominata ("macos", "c89-nudum" -
 * par 10.2.5) = petra miliaria config-query, NOMINATA.
 *
 * Strata inferiora (expansor, motor GLR) contextum numquam noscunt -
 * gubernator campos crudos deorsum copiat (velamen: silva speculum
 * nescit, expansor contextum nescit).
 */

#ifndef SILVA_CONTEXTUS_H
#define SILVA_CONTEXTUS_H

SilvaContextus*
silva_contextus_creare (
    SilvaPiscina* piscina);

/* Lexicon: plagula definitionum (e.g. latina.h) ante fontem
 * principalem processata. Definitiones eius a positione 0 fontis
 * valent; provenientia ad plagulam lexici ducit (fons verus). */
b32
silva_contextus_lexicon_addere (
    SilvaContextus*     contextus,
    constans character* via,
    constans character* textus,
    i32                 mensura);

/* Lexicon latinum compilatum (silva_latina_datum - GENERATUM ex
 * include/latina.h per amalgatorem; fons veritatis unus) */
b32
silva_contextus_latinam_addere (
    SilvaContextus* contextus);

/* Contentum includendum (via #include resolvendum) */
b32
silva_contextus_praebere (
    SilvaContextus*     contextus,
    constans character* via,
    constans character* textus,
    i32                 mensura);

/* passus <= 1 = quovis lexemate (GLR); expansio per generationem
 * interrogat, gubernator per segmentum - passus ibi non attinet */
vacuum
silva_contextus_pergere_ponere (
    SilvaContextus*     contextus,
    SilvaPergereFunctio pergere,
    vacuum*             datum,
    i32                 passus);

#endif /* SILVA_CONTEXTUS_H */

/* ================= ex silva/fontes/silva_latina_datum.h ================= */
/* silva_latina_datum.h - Textus latina.h ut datum (Phase 7 Chunk A)
 *
 * GENERATUM ex include/latina.h per amalgamatorem - NE MANU MUTES
 * (regeneratur per silva/amalgamare.sh). Copia compilata definitionum
 * latinarum: silva_contextus_latinam_addere eam praebet - "compiled-in
 * defaults" interview ad litteram, sine fonte veritatis secundo (datum
 * IPSA plagula vendicata est).
 */

#ifndef SILVA_LATINA_DATUM_H
#define SILVA_LATINA_DATUM_H

externus constans character silva_latina_textus[];
externus constans i32       silva_latina_mensura;

#endif /* SILVA_LATINA_DATUM_H */

/* ================= ex silva/fontes/silva_parsare.h ================= */
/* silva_parsare.h - Gubernator parsurae (Phase 4 Chunk D)
 *
 * Compositio totius fistulae: expansio -> lexemata -> GLR -> commissio.
 * GUBERNATOR EST CIRCUITUS SECANS (spec-v2 par 3.1, plan of record):
 * fluxus in segmenta per limites secatur (sceletum: SEMICOLON in
 * profunditate parenthesium 0), quodque segmentum + lexema EOF
 * independenter parsatur:
 *   - successus -> sententiae eius radici coniunguntur;
 *   - fractura  -> nodus ERROR lexemata segmenti fert (recuperatio
 *     per-rem gratuita; GSS per sententiam terminatus).
 * OMNIS input arborem completam commissam reddit (par 8.2 - vectis
 * tree-sitter). Inventor limitum C89 (laminae, definitiones
 * functionum) cum grammatica C89 veniet - nominatum.
 */

#ifndef SILVA_PARSARE_H
#define SILVA_PARSARE_H


/* ==================================================
 * API
 * ================================================== */

/* Fistula tota ex fonte crudo: praeprocessor incluso. Oraculum/
 * resolutor NIHIL licent (nulla resolutio - ambigua supersunt). */
SilvaParsura*
silva_parsare (
    SilvaPiscina*                  piscina,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris);

/* Fistula tota cum expansione PRAEPARATA a vocatore: includenda iam
 * praebita (silva_includendum_praebere), macros iam iniecta
 * (silva_macro_addere). Haec est sutura contextus (SilvaContextus
 * Phase 7 eam equitabit); silva_parsare = involucrum tenue huius. */
SilvaParsura*
silva_parsare_cum_expansione (
    SilvaPiscina*                  piscina,
    SilvaExpansio*            expansio,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris);

/* Fistula tota cum CONTEXTU hospitis (Phase 7): receptum contextus
 * expansioni recenti applicatur (fines, pergere, lexica ante fontem
 * processata - latina hac via, praebenda), deinde fistula normalis.
 * Contextus diu vivit et inter parsuras reusabilis est. */
SilvaParsura*
silva_parsare_cum_contextu (
    SilvaPiscina*                  piscina,
    constans SilvaContextus*  contextus,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris);

/* Ingressus lexematum (forma silva_lexare: Xar de SilvaToken*, EOF
 * ultimo) - pro fluxibus iam expansis aut probationibus. */
SilvaParsura*
silva_lexemata_parsare (
    SilvaPiscina*                  piscina,
    constans SilvaXar*             lexemata,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris);

#endif /* SILVA_PARSARE_H */

/* ================= ex silva/fontes/silva_scribere.h ================= */
/* silva_scribere.h - Emissio arborum ad octetos (Phase 5)
 *
 * Claudit catenam lexema -> arbor -> octeti. Duo ingressus, duo
 * contractus (INTENTIO Phase 5 + simulatio ⑥):
 *
 *   - silva_scribere_nodum / _valorem: AMBULATIO PURE STRUCTURALIS
 *     subarborum - loci ordine layout, numquam positione dependens
 *     (nulla ordinatio per byte_offset, nulla lectio vicinorum).
 *     Haec puritas est quae subarbores intactas post transformationes
 *     octetim exactas servat: subarbor mota octetos suos ubicumque
 *     emittit. NB directivae consumptae subarboribus NON pertinent
 *     (res plagulae sunt, non nodorum) - eas fontem infra reinserit.
 *
 *   - silva_scribere_fontem: reconstructio STRATI 0 unius plagulae ex
 *     parsura. Tres fontes octetorum (partitio exacta - invariatum
 *     triviorum eam gratis dat, sim ⑥ C3): lexemata arboris (strati 0
 *     per limitem expansionis), lineae directivae consumptae
 *     (parsura->directivae, per fontem filtratae), laminae ramorum
 *     non sumptorum (arbor regionum). Ordinatio per offset - licita
 *     HIC SOLUM (res strati fluxus, non arboris).
 *
 * LIMES EXPANSIONIS (sim ⑥ C4): lexema cuius origo non FONS est non
 * se emittit - invocatio strati 0 emittitur (radix catenae), semel
 * per instantiam (dedup per intervalla octetorum). Invocationes
 * functio-similes strati 0 laminas extentorum in expansione memoratas
 * emittunt (C5) - argumenta cum triviis suis redeunt. PASTA/CHORDA/
 * API in catena = fractura clara (deferral nominatum). Sine contextu
 * expansionis (expansio NIHIL) lexema expansum = fractura clara.
 *
 * Ambulatio grammaticae-ignara est (exemplar commissionis): forma
 * ambigui ex registro cocto PER NOMEN invenitur; AMBIGUUS solam
 * interpretationem CANONICAM emittit. Nodi ERROR casu speciali NON
 * egent (locus unicus lista-token ambulatione generica emittitur).
 * Genus ignotum aut forma corrupta = fractura clara (successus
 * FALSUM + causa + sedes), numquam praetermissio tacita.
 */

#ifndef SILVA_SCRIBERE_H
#define SILVA_SCRIBERE_H


/* ==================================================
 * API
 * ================================================== */

/* Valor quilibet (radix commissionis est LISTA plerumque).
 * expansio: contextus limitis expansionis (extenta) - NIHIL licet,
 * tunc lexema expansum quodlibet = fractura clara. */
SilvaScriptura
silva_scribere_valorem (
    SilvaPiscina*                       piscina,
    SilvaValor                     valor,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaExpansio*        expansio);

/* Subarbor una - ingressus transformationum */
SilvaScriptura
silva_scribere_nodum (
    SilvaPiscina*                       piscina,
    constans SilvaNodus*           nodus,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaExpansio*        expansio);

/* Fons integer ex parsura: arbor + lineae directivae + rami non
 * sumpti + trivia caudae (EOF - pro plagula inclusa ex includenda
 * accersitur, sim ⑥ C6). fons_index: cuius plagulae octeti petuntur;
 * -1 = quaelibet (fluxus praelexati sine includendo). */
SilvaScriptura
silva_scribere_fontem (
    SilvaPiscina*                       piscina,
    constans SilvaParsura*         parsura,
    constans SilvaRegistrumCoctum* tabularium,
    s32                            fons_index);

#endif /* SILVA_SCRIBERE_H */

/* ================= ex lib/piscina.c ================= */

#define PISCINA_DEBUG FALSUM /* Muta ad VERUM pro imprimere debugging */

/* ===========================================================
 * Structura Alvei - allocatio singularis
 * =========================================================== */

nomen structura Alveus {
              vacuum* buffer;
      memoriae_index  capacitas;
      memoriae_index  offset;
    structura Alveus* sequens;
} Alveus;


/* ===========================================================
 * Structura Piscinae - regit alveos multiples
 * =========================================================== */

structura SilvaPiscina {
            Alveus* primus;
            Alveus* nunc;
    memoriae_index  mensura_alvei_initia;
         character* titulus;
               b32  est_dynamicum;
    memoriae_index  maximus_usus;
};


/* ===========================================================
 * ADIUTORES INTERNI
 * =========================================================== */

interior memoriae_index
_proxima_ordinatio (
        memoriae_index ptr, 
        memoriae_index ordinatio)
{
    memoriae_index ordinatus = ptr + (ordinatio - I);
    redde ordinatus - (ordinatus % ordinatio);
}

interior vacuum
_debug_imprimere (
    constans character* piscinae_titulum,
    constans character* operatio,
        memoriae_index  mensura)
{
    si (PISCINA_DEBUG) 
    {
        imprimere("[PISCINA %s] %s: %zu bytes\n", piscinae_titulum, operatio, mensura);
    }
}


/* ===========================================================
 * REGIO ALVEI
 * =========================================================== */

interior Alveus*
_alveus_nova (
    memoriae_index capacitas)
{
    Alveus* alveus = (Alveus*)memoriae_allocare(magnitudo(Alveus));
    si (!alveus) redde NIHIL;

    alveus->buffer = memoriae_allocare(capacitas);
    si (!alveus->buffer)
    {
        liberare(alveus);
        redde NIHIL;
    }

    alveus->capacitas = capacitas;
    alveus->offset    = ZEPHYRUM;
    alveus->sequens   = NIHIL;

    redde alveus;
}

interior vacuum
_alveus_destruere (
        Alveus* alveus)
{
    si (!alveus) redde;

    si (alveus->buffer) liberare(alveus->buffer);
    liberare(alveus);
}

interior vacuum
_catena_alveus_destruere (
        Alveus* alveus)
{
    dum (alveus)
    {
        Alveus* sequens_temporalis = alveus->sequens;
        _alveus_destruere(alveus);
        alveus = sequens_temporalis;
    }
}

/* ===========================================================
 * ALLOCATIO FUNDAMENTALIS LOGICA
 * =========================================================== */

interior vacuum*
_allocare_interna (
               SilvaPiscina* piscina,
        memoriae_index  mensura,
        memoriae_index  ordinatio,
                   b32  fatalis)
{
    memoriae_index  ordinatus_offset;
    memoriae_index  necessaria;
    memoriae_index  summa_nunc; 
            Alveus* b;
            vacuum* ptr;

    si (!piscina || mensura == ZEPHYRUM) redde NIHIL;

    ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset, ordinatio);
    necessaria       = ordinatus_offset + mensura;

    /* Si allocatio in alveum nunc non capit, invenire vel generare alveum novum */
    dum (necessaria > piscina->nunc->capacitas)
    {
        si (piscina->nunc->sequens)
        {
            /* Transire ad alveum sequentem */
            piscina->nunc = piscina->nunc->sequens;
            ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset, ordinatio);
            necessaria = ordinatus_offset + mensura;
        }
        alioquin si (piscina->est_dynamicum)
        {
            Alveus* alveus_novum;

            /* Generare alveum novum */
            memoriae_index capacitas_nova = piscina->mensura_alvei_initia * II;

            /* Si petitio magnitudinem duplicatam superat, allocare 
             * petitionem + sequentem, et mensuram */
            si (necessaria > capacitas_nova)
            {
                capacitas_nova = necessaria + piscina->mensura_alvei_initia;
                piscina->mensura_alvei_initia = capacitas_nova;
            }

            alveus_novum = _alveus_nova(capacitas_nova);
            si (!alveus_novum)
            {
                si (fatalis)
                {
                    imprimere("CREATIO ALVEI FRACTA: %s\n",
                              piscina->titulus ? piscina->titulus : "nemo");
                    exire(I);
                }
                redde NIHIL;
            }

            piscina->nunc->sequens = alveus_novum;
            piscina->nunc = alveus_novum;

            ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset, ordinatio);
            necessaria = ordinatus_offset + mensura;

            _debug_imprimere(
                    piscina->titulus ? piscina->titulus : "nemo",
                    "alveus_novum",
                    capacitas_nova);
        }
        alioquin
        {
            /* Non dynamicum et nulli alvei reliqui */
            si (fatalis)
            {
                imprimere("ALLOCATIO PISCINAE FRACTA: %s (indigentia %zu)\n",
                          piscina->titulus ? piscina->titulus : "nemo",
                          necessaria);
                exire(I);
            }
            redde NIHIL;
        }
    }


    /* Allocare ex alveo nunc */
    ptr = (character*)(piscina->nunc->buffer) + ordinatus_offset;
    piscina->nunc->offset = necessaria;

    /* Sequi apex usus per omnes alvei */
    summa_nunc = ZEPHYRUM;
    per (b = piscina->primus; b; b = b->sequens)
    {
        summa_nunc += b->offset;
    }
    si (summa_nunc > piscina->maximus_usus)
    {
        piscina->maximus_usus = summa_nunc;
    }

    _debug_imprimere(piscina->titulus ? piscina->titulus : "nemo", "allocare", mensura);

    redde ptr;
}

/* ===========================================================
 * GENERATIO
 * =========================================================== */

SilvaPiscina*
silva_piscina_generare_dynamicum (
    constans character* piscinae_titulum,
        memoriae_index  mensura_alvei_initia)
{
    Alveus* alveus_primus;

    SilvaPiscina* piscina = (SilvaPiscina*)memoriae_allocare(magnitudo(SilvaPiscina));
    si (!piscina) redde NIHIL;

    alveus_primus = _alveus_nova(mensura_alvei_initia);
    si (!alveus_primus) 
    {
        liberare(piscina);
        redde NIHIL;
    }

    piscina->primus                = alveus_primus;
    piscina->nunc                  = alveus_primus;
    piscina->mensura_alvei_initia  = mensura_alvei_initia;
    piscina->est_dynamicum         = VERUM;
    piscina->maximus_usus          = ZEPHYRUM;

    si (piscinae_titulum)
    {
        memoriae_index mensura_tituli = strlen(piscinae_titulum);
        piscina->titulus = (character*)memoriae_allocare(mensura_tituli + I);

        si (piscina->titulus)
        {
            strcpy(piscina->titulus, piscinae_titulum);
        }
        alioquin
        {
            piscina->titulus = NIHIL;
        }
    }
    alioquin
    {
        piscina->titulus = NIHIL;
    }

    redde piscina;
}


/* ===========================================================
 * DESTRUCTIO
 * =========================================================== */

vacuum 
silva_piscina_destruere (
        SilvaPiscina* piscina)
{
    si (!piscina) redde;

    si (piscina->primus) _catena_alveus_destruere(piscina->primus);
    si (piscina->titulus) liberare(piscina->titulus);

    liberare(piscina);
}


/* ===========================================================
 * ALLOCATIO - EXITIUM SI DEFECIT
 * =========================================================== */


static vacuum*
silva_piscina_allocare (
           SilvaPiscina* piscina,
    memoriae_index  mensura)
{
    redde _allocare_interna(piscina, mensura, I, VERUM);
}

static vacuum*
silva_piscina_allocare_ordinatum (
           SilvaPiscina* piscina,
    memoriae_index  mensura,
    memoriae_index  ordinatio)
{
    redde _allocare_interna(piscina, mensura, ordinatio, VERUM);
}

/* ===========================================================
 * QUAESTIO
 * =========================================================== */

memoriae_index
silva_piscina_summa_usus (
        constans SilvaPiscina* piscina)
{
    constans Alveus* b;
     memoriae_index  summa;

    si (!piscina) redde ZEPHYRUM;

    summa = ZEPHYRUM;
    per (b = piscina->primus; b; b = b->sequens)
    {
        summa += b->offset;
    }
    redde summa;
}

memoriae_index
silva_piscina_summa_apex_usus (
        constans SilvaPiscina* piscina)
{
    redde piscina ? piscina->maximus_usus : ZEPHYRUM;
}


/* ================= ex lib/chorda.c ================= */

#define CHORDA_FRIATUM_OFFSET  2166136261U
#define CHORDA_FRIATUM_PRIMUS    16777619U


/* ==================================================
 * Constructores
 * ================================================== */

static SilvaChorda
silva_chorda_ex_literis (
    constans character* cstr,
               SilvaPiscina* piscina)
{
    SilvaChorda  fructus;
       i32  mensura;
        i8* allocatus;

    si (!cstr || !piscina)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

      mensura = (i32)strlen(cstr);
    allocatus = (i8*)silva_piscina_allocare(piscina, mensura);

    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    memcpy(allocatus, cstr, mensura);
    fructus.mensura = mensura;
    fructus.datum   = allocatus;
    redde fructus;
}

static SilvaChorda
silva_chorda_transcribere (
     SilvaChorda  s,
    SilvaPiscina* piscina)
{
    SilvaChorda  fructus;
        i8* allocatus;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)silva_piscina_allocare(piscina, s.mensura);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    memcpy(allocatus, s.datum, s.mensura);

    fructus.mensura = s.mensura;
    fructus.datum   = allocatus;

    redde fructus;
}

static s32
silva_chorda_comparare (
    SilvaChorda a,
    SilvaChorda b)
{
    memoriae_index minima_mensura; 
               s32 cmp_result;

    minima_mensura  = a.mensura < b.mensura ? a.mensura : b.mensura;
    cmp_result      = memcmp(a.datum, b.datum, minima_mensura);

    si (cmp_result != ZEPHYRUM)
    {
        redde cmp_result;
    }

    si (a.mensura < b.mensura) redde -I;
    si (a.mensura > b.mensura) redde I;

    redde ZEPHYRUM;
}

/* ================= ex lib/chorda_aedificator.c ================= */

/* ==================================================
 * Structura ChordaAedificator - Interna
 * ================================================== */

structura SilvaChordaAedificator {
	           i8*  buffer;
	memoriae_index  capacitas;
	memoriae_index  offset;
	       SilvaPiscina* piscina;
	           i32  indentatio_gradus;
};


/* ==================================================
 * ADIUTORES INTERNI
 * ================================================== */

interior memoriae_index
_proxima_capacitas(memoriae_index nunc)
{
	/* Duplica capacitatem donec satis habeamus */
	redde nunc > ZEPHYRUM ? nunc * II : XVI;
}

interior b32
_crescere(
    SilvaChordaAedificator* aedificator, 
       memoriae_index  necessaria)
{
	memoriae_index  capacitas_nova;
	            i8* buffer_novum;

	capacitas_nova = aedificator->capacitas;
	dum (capacitas_nova < necessaria)
	{
		capacitas_nova = _proxima_capacitas(capacitas_nova);
	}

	buffer_novum = (i8*)silva_piscina_allocare(aedificator->piscina, capacitas_nova);
	si (!buffer_novum)
		redde FALSUM;

	si (aedificator->buffer && aedificator->offset > ZEPHYRUM)
	{
		memcpy(buffer_novum, aedificator->buffer, aedificator->offset);
	}

	aedificator->buffer    = buffer_novum;
	aedificator->capacitas = capacitas_nova;

	redde VERUM;
}

interior b32
_appendere_interna(
    SilvaChordaAedificator* aedificator, 
          constans i8* datum, 
       memoriae_index  mensura)
{
	memoriae_index necessaria;

	si (!aedificator || !datum || mensura == ZEPHYRUM)
		redde mensura == ZEPHYRUM; /* Appendix vacua bona est */

	necessaria = aedificator->offset + mensura;

	si (necessaria > aedificator->capacitas)
	{
		si (!_crescere(aedificator, necessaria))
			redde FALSUM;
	}

	memcpy(aedificator->buffer + aedificator->offset, datum, mensura);
	aedificator->offset += mensura;

	redde VERUM;
}


/* ==================================================
 * Creatio
 * ================================================== */

static SilvaChordaAedificator*
silva_chorda_aedificator_creare(
           SilvaPiscina* piscina, 
    memoriae_index capacitas_initialis)
{
	SilvaChordaAedificator* aedificator;
	               i8* buffer;

	si (!piscina || capacitas_initialis == ZEPHYRUM)
		redde NIHIL;

	aedificator = (SilvaChordaAedificator*)silva_piscina_allocare(
                                        piscina, 
                                        magnitudo(SilvaChordaAedificator));
	si (!aedificator)
		redde NIHIL;

	buffer = (i8*)silva_piscina_allocare(piscina, capacitas_initialis);
	si (!buffer)
		redde NIHIL;

	aedificator->buffer            = buffer;
	aedificator->capacitas         = capacitas_initialis;
	aedificator->offset            = ZEPHYRUM;
	aedificator->piscina           = piscina;
	aedificator->indentatio_gradus = ZEPHYRUM;

	redde aedificator;
}


/* ==================================================
 * Appendere - Character
 * ================================================== */

static b32
silva_chorda_aedificator_appendere_character(
    SilvaChordaAedificator* aedificator, 
            character  c)
{
	i8 ch = (i8)c;
	redde _appendere_interna(aedificator, &ch, I);
}


/* ==================================================
 * Appendere - Chordae
 * ================================================== */

static b32
silva_chorda_aedificator_appendere_literis(
     SilvaChordaAedificator* aedificator, 
    constans character* cstr)
{
	memoriae_index mensura;

	si (!aedificator || !cstr)
		redde FALSUM;

	mensura = strlen(cstr);
	redde _appendere_interna(aedificator, (constans i8*)cstr, mensura);
}

static b32
silva_chorda_aedificator_appendere_chorda(
    SilvaChordaAedificator* aedificator, 
               SilvaChorda s)
{
	si (!aedificator || !s.datum)
		redde FALSUM;

	redde _appendere_interna(aedificator, s.datum, s.mensura);
}

static SilvaChorda
silva_chorda_aedificator_finire(
    SilvaChordaAedificator* aedificator)
{
	SilvaChorda result;

	si (!aedificator)
	{
		result.mensura = ZEPHYRUM;
		result.datum   = NIHIL;
		redde result;
	}

	result.mensura = (i32)aedificator->offset;
	result.datum   = aedificator->buffer;

	redde result;
}

/* ================= ex lib/friatio.c ================= */

/* ==================================================
 * FNV-1a (Fowler-Noll-Vo)
 * ================================================== */

/* FNV-1a constants (32-bit) */
#define FNV_OFFSET_BASIS 2166136261U
#define FNV_PRIME        16777619U

static i32
silva_friatio_fnv1a(
    SilvaChorda clavis)
{
    i32 friatum;
    i32 i;

    friatum = (i32)FNV_OFFSET_BASIS;

    per (i = ZEPHYRUM; i < clavis.mensura; i++)
    {
        friatum ^= (i8)clavis.datum[i];
        friatum *= (i32)FNV_PRIME;
    }

    redde friatum;
}

/* ==================================================
 * DJB2 (Dan Bernstein)
 * ================================================== */

#define DJB2_INIT 5381U

/* ==================================================
 * SHA-1 Implementation
 *
 * Based on RFC 3174
 * Produces 160-bit (20-byte) message digest
 * ================================================== */

/* Left rotate 32-bit value */
#define SHA1_ROL(value, bits) (((value) << (bits)) | ((value) >> (XXXII - (bits))))

/* SHA-1 round functions */
#define SHA1_F0(b,c,d) (((b) & (c)) | ((~(b)) & (d)))
#define SHA1_F1(b,c,d) ((b) ^ (c) ^ (d))
#define SHA1_F2(b,c,d) (((b) & (c)) | ((b) & (d)) | ((c) & (d)))
#define SHA1_F3(b,c,d) ((b) ^ (c) ^ (d))

/* SHA-1 constants */
#define SHA1_K0 0x5A827999UL
#define SHA1_K1 0x6ED9EBA1UL
#define SHA1_K2 0x8F1BBCDCUL
#define SHA1_K3 0xCA62C1D6UL

/* SHA-1 initial hash values */
#define SHA1_H0 0x67452301UL
#define SHA1_H1 0xEFCDAB89UL
#define SHA1_H2 0x98BADCFEUL
#define SHA1_H3 0x10325476UL
#define SHA1_H4 0xC3D2E1F0UL

/* ==================================================
 * SHA-256 Implementation
 *
 * Based on FIPS 180-4
 * Produces 256-bit (32-byte) message digest
 * ================================================== */

/* Right rotate 32-bit value */
#define SHA256_ROR(value, bits) (((value) >> (bits)) | ((value) << (XXXII - (bits))))

/* SHA-256 logical functions */
#define SHA256_CH(x,y,z)  (((x) & (y)) ^ ((~(x)) & (z)))
#define SHA256_MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SHA256_EP0(x)     (SHA256_ROR(x, II) ^ SHA256_ROR(x, XIII) ^ SHA256_ROR(x, XXII))
#define SHA256_EP1(x)     (SHA256_ROR(x, VI) ^ SHA256_ROR(x, XI) ^ SHA256_ROR(x, XXV))
#define SHA256_SIG0(x)    (SHA256_ROR(x, VII) ^ SHA256_ROR(x, XVIII) ^ ((x) >> III))
#define SHA256_SIG1(x)    (SHA256_ROR(x, XVII) ^ SHA256_ROR(x, XIX) ^ ((x) >> X))

/* SHA-256 initial hash values (first 32 bits of fractional parts of square roots of first 8 primes) */
#define SHA256_H0 0x6A09E667UL
#define SHA256_H1 0xBB67AE85UL
#define SHA256_H2 0x3C6EF372UL
#define SHA256_H3 0xA54FF53AUL
#define SHA256_H4 0x510E527FUL
#define SHA256_H5 0x9B05688CUL
#define SHA256_H6 0x1F83D9ABUL
#define SHA256_H7 0x5BE0CD19UL

/* ================= ex lib/tabula_dispersa.c ================= */

/* ==================================================
 * Functiones Friationis (wrappers ad friatio.h)
 * ================================================== */

static i32
tabula_friare_fnv1a(SilvaChorda clavis)
{
    redde silva_friatio_fnv1a(clavis);
}

/* ==================================================
 * Adiutor: Proxima Potentia Binaria
 * ================================================== */

interior i32
_proxima_potentia_2(i32 n)
{
    n--;
    n |= n >> I;
    n |= n >> II;
    n |= n >> IV;
    n |= n >> VIII;
    n |= n >> XVI;
    n++;
    redde n;
}



/* ==================================================
 * Adiutor: Invenire Slotum
 * Redde positus sloti, statuit vexillum *inventum
 * ================================================== */

interior i32
_invenire_slotum (
    SilvaTabulaDispersa* tabula,
            SilvaChorda  clavis,
               i32  friatum,
               b32* inventum)
{
    i32 positus;
    i32 distantia;
    i32 positus_primus_deletum;

    *inventum              = FALSUM;
    positus                = friatum & tabula->capacitas_mask;
    distantia              = ZEPHYRUM;
    positus_primus_deletum = (i32)-I;

    dum (tabula->sloti[positus].status != SLOT_VACUUM)
    {
        si (tabula->sloti[positus].status == SLOT_OCCUPATUM)
        {
            si (tabula->sloti[positus].hash_cachatum == friatum &&
                tabula->comparatio(
                    tabula->sloti[positus].clavis, clavis) == ZEPHYRUM)
            {
                *inventum = VERUM;
                redde positus;
            }
            tabula->collisiones_totales++;
        }
        alioquin si (tabula->sloti[positus].status == SLOT_DELETUM)
        {
            si (positus_primus_deletum == (i32)-I)
            {
                positus_primus_deletum = positus;
            }
        }

        positus = (positus + I) & tabula->capacitas_mask;
        distantia++;

        si (distantia > tabula->distantia_maxima)
        {
            tabula->distantia_maxima = distantia;
        }

        si (distantia > tabula->capacitas)
        {
            frange;
        }
    }

    si (positus_primus_deletum != (i32)-I)
    {
        redde positus_primus_deletum;
    }

    redde positus;
}


/* ==================================================
 * Crescentia
 * ================================================== */

interior b32
_tabula_dispersa_crescere(SilvaTabulaDispersa* tabula)
{
    SilvaSlotus* sloti_veteres;
       i32  capacitas_vetus;
       i32  i;

    sloti_veteres   = tabula->sloti;
    capacitas_vetus = tabula->capacitas;

    tabula->capacitas      = capacitas_vetus * II;
    tabula->capacitas_mask = tabula->capacitas - I;

    tabula->sloti = (SilvaSlotus*)silva_piscina_allocare(
        tabula->piscina,
        tabula->capacitas * magnitudo(SilvaSlotus));

    si (!tabula->sloti)
    {
        tabula->sloti          = sloti_veteres;
        tabula->capacitas      = capacitas_vetus;
        tabula->capacitas_mask = tabula->capacitas - I;
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < tabula->capacitas; i++)
    {
        tabula->sloti[i].status = SLOT_VACUUM;
    }

    tabula->numerus = ZEPHYRUM;
    tabula->numerus_deletorum = ZEPHYRUM;
    tabula->collisiones_totales = ZEPHYRUM;
    tabula->distantia_maxima = ZEPHYRUM;

    per (i = ZEPHYRUM; i < capacitas_vetus; i++)
    {
        si (sloti_veteres[i].status == SLOT_OCCUPATUM)
        {
            silva_tabula_dispersa_inserere(
                tabula,
                sloti_veteres[i].clavis,
                sloti_veteres[i].valor);
        }
    }

    redde VERUM;
}

/* ==================================================
 * Creatio
 * ================================================== */

static SilvaTabulaDispersa*
silva_tabula_dispersa_creare(
             SilvaPiscina* piscina,
                 i32  capacitas_initialis,
       TabulaFriatio  friatio,
    TabulaComparatio  comparatio)
{
    SilvaTabulaDispersa* tabula;
               i32  capacitas;
               i32  i;

    si (!piscina || !friatio || !comparatio)
    {
        redde NIHIL;
    }

    capacitas = _proxima_potentia_2(capacitas_initialis);
    si (capacitas < XVI) capacitas = XVI;

    tabula = (SilvaTabulaDispersa*)silva_piscina_allocare(
                                piscina, 
                                magnitudo(SilvaTabulaDispersa));
    si (!tabula) redde NIHIL;

    tabula->sloti = (SilvaSlotus*)silva_piscina_allocare(
                                piscina,
                                capacitas * magnitudo(SilvaSlotus));
    si (!tabula->sloti) redde NIHIL;

    per (i = ZEPHYRUM; i < capacitas; i++)
    {
        tabula->sloti[i].status         = SLOT_VACUUM;
        tabula->sloti[i].clavis.mensura = ZEPHYRUM;
        tabula->sloti[i].clavis.datum   = NIHIL;
        tabula->sloti[i].valor          = NIHIL;
        tabula->sloti[i].hash_cachatum  = ZEPHYRUM;
    }

    tabula->capacitas                = capacitas;
    tabula->capacitas_mask           = capacitas - I;
    tabula->numerus                  = ZEPHYRUM;
    tabula->numerus_deletorum        = ZEPHYRUM;
    tabula->piscina                  = piscina;
    tabula->comparatio               = comparatio;
    tabula->friatio                  = friatio;
    tabula->factor_maximus           = 0.75f;
    tabula->factor_deletorum_maximus = 0.25f;
    tabula->collisiones_totales      = ZEPHYRUM;
    tabula->distantia_maxima         = ZEPHYRUM;

    redde tabula;
}


static SilvaTabulaDispersa*
silva_tabula_dispersa_creare_chorda(
    SilvaPiscina* piscina,
        i32  capacitas_initialis)
{
    redde silva_tabula_dispersa_creare(
        piscina,
        capacitas_initialis,
        tabula_friare_fnv1a,
        (TabulaComparatio)silva_chorda_comparare);
}


/* ==================================================
 * Insertio
 * ================================================== */

static b32
silva_tabula_dispersa_inserere(
    SilvaTabulaDispersa* tabula,
            SilvaChorda  clavis,
            vacuum* valor)
{
    i32 friatum;
    i32 positus;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Confer factores oneris */
    si (silva_tabula_dispersa_factor_oneris(tabula) > tabula->factor_maximus ||
        silva_tabula_dispersa_factor_deletorum(tabula) > tabula->factor_deletorum_maximus)
    {
        si (!_tabula_dispersa_crescere(tabula))
        {
            redde FALSUM;
        }
    }

    friatum = tabula->friatio(clavis);
    positus = _invenire_slotum(tabula, clavis, friatum, &inventum);

    si (inventum)
    {
        /* Renovatio existentis */
        tabula->sloti[positus].valor = valor;
        redde VERUM;
    }

    /* Insertio nova */
    si (tabula->sloti[positus].status == SLOT_DELETUM)
    {
        tabula->numerus_deletorum--;
    }

    tabula->sloti[positus].clavis          = clavis;
    tabula->sloti[positus].valor           = valor;
    tabula->sloti[positus].hash_cachatum   = friatum;
    tabula->sloti[positus].status          = SLOT_OCCUPATUM;
    tabula->numerus++;

    redde VERUM;
}


/* ==================================================
 * Quaestio
 * ================================================== */

static b32
silva_tabula_dispersa_invenire(
    SilvaTabulaDispersa*  tabula,
            SilvaChorda   clavis,
            vacuum** valor_out)
{
    i32 friatum;
    i32 positus;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    friatum = tabula->friatio(clavis);
    positus = _invenire_slotum(tabula, clavis, friatum, &inventum);

    si (inventum)
    {
        si (valor_out)
        {
            *valor_out = tabula->sloti[positus].valor;
        }
        redde VERUM;
    }

    redde FALSUM;
}

static b32
silva_tabula_dispersa_continet(
    SilvaTabulaDispersa* tabula,
            SilvaChorda  clavis)
{
    redde silva_tabula_dispersa_invenire(tabula, clavis, NIHIL);
}

/* ==================================================
 * Deletio
 * ================================================== */

static b32
silva_tabula_dispersa_delere(
    SilvaTabulaDispersa* tabula,
            SilvaChorda  clavis)
{
    i32 friatum;
    i32 positus;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    friatum = tabula->friatio(clavis);
    positus = _invenire_slotum(tabula, clavis, friatum, &inventum);

    si (inventum)
    {
        tabula->sloti[positus].status = SLOT_DELETUM;
        tabula->numerus--;
        tabula->numerus_deletorum++;
        redde VERUM;
    }

    redde FALSUM;
}


/* ==================================================
 * Statisticae
 * ================================================== */

static f32
silva_tabula_dispersa_factor_oneris(SilvaTabulaDispersa* tabula)
{
    si (!tabula || tabula->capacitas == ZEPHYRUM)
    {
        redde 0.0f;
    }
    redde (f32)tabula->numerus / (f32)tabula->capacitas;
}

static f32
silva_tabula_dispersa_factor_deletorum(SilvaTabulaDispersa* tabula)
{
    si (!tabula || tabula->capacitas == ZEPHYRUM)
    {
        redde 0.0f;
    }
    redde (f32)tabula->numerus_deletorum / (f32)tabula->capacitas;
}

/* ================= ex lib/xar.c ================= */

/* ========================================================================
 * STRUCTURAE INTERNAE
 * ======================================================================== */

/* XarLocatio - Locatio elementi in structura segmentata (interior)
 */
nomen structura XarLocatio {
       i32  index_segmenti;         /* Quis segmentum? */
       i32  offset_in_segmento;     /* Ubi in segmento? */
       i32  magnitudo_segmenti;     /* Quam magnus? */
    vacuum* basis_segmenti;         /* Initium segmenti */
} XarLocatio;

/* ========================================================================
 * CONSTANTAE INTERNAE
 * ======================================================================== */

/* Factor duplicandi pro crescentia segmentorum */
#define XAR_FACTOR_DUPLICANDI  II

/* ========================================================================
 * FUNCTIONES AUXILIARES
 * ======================================================================== */

/* Computare Magnitudinem Segmenti
 * "Computare magnitudinem segmenti usans formam conservatam"
 *
 * Notitia clavis: duo prima segmenta eadem magnitudo, deinde duplicatio
 * "Regula duplicandi"
 */
interior i32
computare_magnitudinem_segmenti(
    constans SilvaXar* xar,
             i32  index_segmenti)
{
	i32 shift_amount;

	si (index_segmenti <= I)
    {
		/* Duo prima segmenta: eadem magnitudo */
		redde xar->magnitudo_primi;
	}

	shift_amount = index_segmenti - I;

	/* Impossibilis cum indices i32 validi
	 * Cum magnitudo_primi = XVI et indices i32 (max ~2^31),
	 * numquam opus est plus quam ~27-30 segmenta.
	 * Si hic pervenis, corruptio structurae vel error gravis.
	 */
	si (shift_amount >= XXX)
    {
		imprimere("FRACTA: xar segmentum nimis altum: %d\n", index_segmenti);
		imprimere("        (impossibilis cum indices i32 - corruptio?)\n");
		exire(I);
	}

	/* Segmenta sequentia: duplicant */
	redde xar->magnitudo_primi << shift_amount;
}

/* Allocare Segmentum
 * "Partem allocare"
 *
 * Redde: VERUM si successus, FALSUM si error
 */
interior b32
allocare_segmentum(
    SilvaXar* xar,
    i32  index_segmenti)
{
	           i32  magnitudo_segmenti;
	memoriae_index  magnitudo_memoriae;
	        vacuum* memoria;

	si (index_segmenti >= XAR_MAXIMUS_SEGMENTORUM)
    {
		redde FALSUM;  /* Nimis multa segmenta! */
	}

	si (xar->segmenta[index_segmenti])
    {
		redde VERUM;  /* Iam allocatus */
	}

	/* Computare magnitudinem segmenti */
	magnitudo_segmenti = computare_magnitudinem_segmenti(xar, index_segmenti);
	magnitudo_memoriae = (memoriae_index)magnitudo_segmenti * xar->magnitudo_elementi;

	/* Allocare ex piscina */
	memoria = silva_piscina_allocare(xar->piscina, magnitudo_memoriae);
	si (!memoria)
    {
		redde FALSUM;  /* Allocatio fracta */
	}

	/* Zephyrum memoriam si vexillum non ponit */
	si (!(xar->vexilla & XAR_VEXILLUM_SINE_ZEPHYRUM))
    {
		memset(memoria, ZEPHYRUM, magnitudo_memoriae);
	}

	/* Renovare statum xar */
	xar->segmenta[index_segmenti] = memoria;
	si (index_segmenti >= xar->numerus_segmentorum)
    {
		xar->numerus_segmentorum = index_segmenti + I;
	}
	/* Incrementaliter renovare capacitatem - O(1) non O(n) */
	xar->capacitas_totalis += magnitudo_segmenti;

	redde VERUM;
}

/* ========================================================================
 * FUNCTIONES CREATIONIS
 * ======================================================================== */

/* Xar Creare */
static SilvaXar*
silva_xar_creare(
    SilvaPiscina* piscina,
        i32  magnitudo_elementi)
{
	redde silva_xar_creare_cum_vexillis(piscina,
	                               magnitudo_elementi,
	                               XAR_PRIMUS_SEGMENTUM,
	                               XAR_VEXILLUM_ORDINARIUS);
}

/* Xar Creare Cum Vexillis
 * "Genesis tabulae exponentialis - cum omnibus optionibus"
 */
static SilvaXar*
silva_xar_creare_cum_vexillis(
    SilvaPiscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi,
        i32  vexilla)
{
	SilvaXar* xar;
	i32  i;

	si (!piscina || magnitudo_elementi == ZEPHYRUM)
    {
		redde NIHIL;
	}

	/* Allocare structuram xar ex piscina */
	xar = (SilvaXar*)silva_piscina_allocare_ordinatum(piscina, magnitudo(SilvaXar), magnitudo(SilvaXar));
	si (!xar)
    {
		redde NIHIL;
	}

	/* Initializare metadatum
	 * "Initium notitiae"
	 */
	xar->numerus_elementorum = ZEPHYRUM;
	xar->magnitudo_elementi  = magnitudo_elementi;
	xar->magnitudo_primi     = magnitudo_primi ? magnitudo_primi : XAR_PRIMUS_SEGMENTUM;
	xar->numerus_segmentorum = ZEPHYRUM;
	xar->capacitas_totalis   = ZEPHYRUM;
	xar->vexilla             = vexilla;
	xar->piscina             = piscina;

	/* Vacuare segmenta
	 * "Vacuare omnes indices segmentorum"
	 */
	per (i = ZEPHYRUM; i < XAR_MAXIMUS_SEGMENTORUM; i++)
    {
		xar->segmenta[i] = NIHIL;
	}

	/* Ponere titulus */
	strcpy(xar->titulus, "Xar Anonymus");

	redde xar;
}

/* ========================================================================
 * LOCATIO ET ACCESSUS
 * ======================================================================== */

/* Xar Locare - Algorithmus O(1)! (interior)
 * "Invenire locum indicis"
 *
 * ALGORITHMUS:
 * 1. Via rapida pro primis duobus segmentis (computatio directa)
 * 2. Quaestio exponentia pro ceteris (sine circulatio per omnia segmenta)
 */
interior b32
silva_xar_locare(
    constans     SilvaXar* xar,
                 i32  index,
          XarLocatio* locatio)
{
	i32 index_segmenti;
	i32 magnitudo_segmenti;
	i32 index_adiustus;

	si (!xar || !locatio)
    {
		redde FALSUM;
	}

	/* Via rapida pro primis duobus segmentis (eadem magnitudo)
	 */
	si (index < xar->magnitudo_primi * II)
    {
		si (index < xar->magnitudo_primi)
        {
			/* Segmentum primum */
			locatio->index_segmenti     = ZEPHYRUM;
			locatio->offset_in_segmento = index;
			locatio->magnitudo_segmenti = xar->magnitudo_primi;
		}
        alioquin
        {
			/* Segmentum secundum */
			locatio->index_segmenti     = I;
			locatio->offset_in_segmento = index - xar->magnitudo_primi;
			locatio->magnitudo_segmenti = xar->magnitudo_primi;
		}
		locatio->basis_segmenti = xar->segmenta[locatio->index_segmenti];
		redde VERUM;
	}

	/* Computatio directa pro segmentis exponentialibus
	 *
	 * Usans manipulationem bitorum pro quaestio exponentia
	 */
	index_adiustus     = index - (xar->magnitudo_primi * II);
	index_segmenti     = II;
	magnitudo_segmenti = xar->magnitudo_primi * II;  /* Magnitudo segmenti 2 */

	/* Invenire segmentum usans formam crescentiae exponentialem
	 */
	dum (index_adiustus >= magnitudo_segmenti &&
         index_segmenti < XAR_MAXIMUS_SEGMENTORUM)
    {
		index_adiustus -= magnitudo_segmenti;

		/* Verificare overflow ante duplicatio
		 * Si magnitudo_segmenti > 2^30, duplicatio overflow facit
		 */
		si (magnitudo_segmenti > (0x7FFFFFFF >> I))
        {
			imprimere("FRACTA: xar magnitudo segmenti overflow: %d\n",
			          magnitudo_segmenti);
			imprimere("        (impossibilis cum indices i32)\n");
			exire(I);
		}

		magnitudo_segmenti <<= I;  /* Duplicare pro segmento proximo */
		index_segmenti++;
	}

	si (index_segmenti >= XAR_MAXIMUS_SEGMENTORUM)
    {
		redde FALSUM;  /* Index nimis magnus */
	}

	locatio->index_segmenti     = index_segmenti;
	locatio->offset_in_segmento = index_adiustus;
	locatio->magnitudo_segmenti = computare_magnitudinem_segmenti(xar, index_segmenti);
	locatio->basis_segmenti     = xar->segmenta[index_segmenti];

	redde VERUM;
}

/* Xar Obtinere
 * "Obtinere elementum"
 */
vacuum*
silva_xar_obtinere(
    constans SilvaXar* xar,
             i32  index)
{
	XarLocatio  locatio;
	        i8* basis;

	si (!xar || index >= xar->numerus_elementorum)
    {
		redde NIHIL;
	}

	si (!silva_xar_locare(xar, index, &locatio))
    {
		redde NIHIL;
	}

	si (!locatio.basis_segmenti)
    {
		redde NIHIL;  /* Segmentum non allocatus */
	}

	/* Computare locum elementi
	 */
	basis = (i8*)locatio.basis_segmenti;
	redde basis + (locatio.offset_in_segmento * xar->magnitudo_elementi);
}

/* ========================================================================
 * ADDITIO
 * ======================================================================== */

/* Xar Addere
 * "Addere ad finem"
 */
static vacuum*
silva_xar_addere(
    SilvaXar* xar)
{
	XarLocatio  locatio;
	       i32  index_novus;
	        i8* basis;

	si (!xar)
    {
		redde NIHIL;
	}

	index_novus = xar->numerus_elementorum;

	/* Invenire ubi elementum novum ibit */
	si (!silva_xar_locare(xar, index_novus, &locatio))
    {
		redde NIHIL;
	}

	/* Assecurare segmentum existit */
	si (!xar->segmenta[locatio.index_segmenti])
    {
		si (!allocare_segmentum(xar, locatio.index_segmenti))
        {
			redde NIHIL;  /* Allocatio fracta */
		}
		/* Renovare locationem cum segmento novo */
		locatio.basis_segmenti = xar->segmenta[locatio.index_segmenti];
	}

	/* Incrementare numerum */
	xar->numerus_elementorum++;

	/* Reddere indicem ad elementum novum */
	basis = (i8*)locatio.basis_segmenti;
	redde basis + (locatio.offset_in_segmento * xar->magnitudo_elementi);
}

/* ========================================================================
 * UTILITAS
 * ======================================================================== */

/* Xar Numerus */
i32
silva_xar_numerus(
    constans SilvaXar* xar)
{
	redde xar ? xar->numerus_elementorum : ZEPHYRUM;
}

/* Xar Vacare */
static vacuum
silva_xar_vacare(
    SilvaXar* xar)
{
	si (xar)
    {
		xar->numerus_elementorum = ZEPHYRUM;
		/* Nota: segmenta manent allocata cum piscina */
	}
}

/* Xar Truncare */
static vacuum
silva_xar_truncare(SilvaXar* xar, i32 numerus_novus)
{
	si (xar && numerus_novus < xar->numerus_elementorum)
    {
		xar->numerus_elementorum = numerus_novus;
	}
}


/* ========================================================================
 * ORDINATIO ET MANIPULATIO
 * ======================================================================== */

/* Xar Mutare
 * "Mutare duo elementa"
 */
static b32
silva_xar_mutare(
    SilvaXar* xar,
    i32  index_a,
    i32  index_b)
{
	vacuum* elem_a;
	vacuum* elem_b;
	    i8  temporalis[CCLVI];  /* Buffer temporalis pro swap */
	    i8* temp_heap;

	si (!xar)
	{
		redde FALSUM;
	}

	si (index_a >= xar->numerus_elementorum ||
	    index_b >= xar->numerus_elementorum)
	{
		redde FALSUM;
	}

	/* Si idem index, nihil agendum */
	si (index_a == index_b)
	{
		redde VERUM;
	}

	elem_a = silva_xar_obtinere(xar, index_a);
	elem_b = silva_xar_obtinere(xar, index_b);

	si (!elem_a || !elem_b)
	{
		redde FALSUM;
	}

	/* Mutare usans buffer temporalem */
	si (xar->magnitudo_elementi <= CCLVI)
	{
		/* Usare buffer in stack */
		memcpy(temporalis, elem_a, xar->magnitudo_elementi);
		memcpy(elem_a, elem_b, xar->magnitudo_elementi);
		memcpy(elem_b, temporalis, xar->magnitudo_elementi);
	}
	alioquin
	{
		/* Allocare in heap pro elementis magnis */
		temp_heap = (i8*)silva_piscina_allocare(xar->piscina, xar->magnitudo_elementi);
		si (!temp_heap)
		{
			redde FALSUM;
		}
		memcpy(temp_heap, elem_a, xar->magnitudo_elementi);
		memcpy(elem_a, elem_b, xar->magnitudo_elementi);
		memcpy(elem_b, temp_heap, xar->magnitudo_elementi);
		/* Nota: temp_heap liberabitur cum piscina */
	}

	redde VERUM;
}

/* Xar Ordinare
 * "Ordinare in loco usans selection sort"
 *
 * Selection sort: O(n²) sed simplex et stabilis
 */
static vacuum
silva_xar_ordinare(
              SilvaXar* xar,
    SilvaXarComparator  comparator)
{
	   i32  i;
	   i32  j;
	   i32  min_index;
	vacuum* elem_j;
	vacuum* elem_min;

	si (!xar || !comparator || xar->numerus_elementorum <= I)
	{
		redde;
	}

	per (i = ZEPHYRUM; i < xar->numerus_elementorum - I; i++)
	{
		min_index = i;
		elem_min  = silva_xar_obtinere(xar, i);

		per (j = i + I; j < xar->numerus_elementorum; j++)
		{
			elem_j = silva_xar_obtinere(xar, j);
			si (elem_j && elem_min && comparator(elem_j, elem_min) < ZEPHYRUM)
			{
				min_index = j;
				elem_min  = elem_j;
			}
		}

		si (min_index != i)
		{
			silva_xar_mutare(xar, i, min_index);
		}
	}
}

/* ================= ex silva/fontes/silva_token.c ================= */

/* ==================================================
 * Auxiliares interni
 * ================================================== */

interior SilvaToken*
_token_novum (SilvaPiscina* piscina)
{
    SilvaToken* token;

    token = (SilvaToken*)silva_piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaToken));
    si (token != NIHIL)
    {
        memset(token, ZEPHYRUM, magnitudo(SilvaToken));
        token->byte_offset = -I;
        token->fons_index  = -I;
    }
    redde token;
}

interior b32
_chordae_pares (SilvaChorda a, SilvaChorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    si (a.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (memcmp(a.datum, b.datum, (memoriae_index)a.mensura) == ZEPHYRUM)
        ? VERUM : FALSUM;
}


/* ==================================================
 * Constructores
 * ================================================== */

SilvaToken*
silva_token_ex_fonte (
    SilvaPiscina*         piscina,
    SilvaLexemaGenus genus,
    SilvaChorda           valor,
    s32              byte_offset,
    i32              linea,
    i32              columna,
    s32              fons_index)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus       = genus;
    token->valor       = valor;
    token->byte_offset = byte_offset;
    token->longitudo   = valor.mensura;
    token->linea       = linea;
    token->columna     = columna;
    token->fons_index  = fons_index;
    token->standard    = SILVA_STANDARD_C89;
    token->origo.genus = SILVA_ORIGO_FONS;

    redde token;
}

SilvaToken*
silva_token_ex_expansione (
    SilvaPiscina*       piscina,
    SilvaToken*    corpus,
    SilvaToken*    invocatio,
    SilvaChorda*        nomen_macro,
    SilvaCaecatio* caecatio)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    /* Campi lexicales a corpore (def-site identitas lexicalis) */
    token->genus       = corpus->genus;
    token->valor       = corpus->valor;
    token->byte_offset = corpus->byte_offset;
    token->longitudo   = corpus->longitudo;
    token->linea       = corpus->linea;
    token->columna     = corpus->columna;
    token->fons_index  = corpus->fons_index;
    token->standard    = corpus->standard;

    token->origo.genus                   = SILVA_ORIGO_EXPANSIO;
    token->origo.datum.expansio.corpus     = corpus;
    token->origo.datum.expansio.invocatio  = invocatio;
    token->origo.datum.expansio.nomen_macro = nomen_macro;
    token->origo.datum.expansio.caecatio    = caecatio;

    redde token;
}

SilvaToken*
silva_token_ex_pasta (
    SilvaPiscina*         piscina,
    SilvaLexemaGenus genus,
    SilvaChorda           valor,
    SilvaToken*      sinister,
    SilvaToken*      dexter,
    SilvaChorda*          nomen_macro,
    SilvaCaecatio*   caecatio)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus     = genus;
    token->valor     = valor;
    token->longitudo = valor.mensura;
    si (sinister != NIHIL)
    {
        token->linea      = sinister->linea;
        token->columna    = sinister->columna;
        token->fons_index = sinister->fons_index;
    }

    token->origo.genus                  = SILVA_ORIGO_PASTA;
    token->origo.datum.pasta.sinister    = sinister;
    token->origo.datum.pasta.dexter      = dexter;
    token->origo.datum.pasta.nomen_macro = nomen_macro;
    token->origo.datum.pasta.caecatio    = caecatio;

    redde token;
}

SilvaToken*
silva_token_ex_stringificatione (
    SilvaPiscina*    piscina,
    SilvaChorda      valor,
    SilvaToken* primus,
    SilvaChorda*     nomen_macro)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus     = SILVA_LEX_STRING_LIT;
    token->valor     = valor;
    token->longitudo = valor.mensura;
    si (primus != NIHIL)
    {
        token->linea      = primus->linea;
        token->columna    = primus->columna;
        token->fons_index = primus->fons_index;
    }

    token->origo.genus                           = SILVA_ORIGO_CHORDA;
    token->origo.datum.stringificatio.primus      = primus;
    token->origo.datum.stringificatio.nomen_macro = nomen_macro;

    redde token;
}

SilvaToken*
silva_token_ex_api (
    SilvaPiscina*         piscina,
    SilvaLexemaGenus genus,
    SilvaChorda           valor,
    SilvaChorda*          nomen_macro,
    s32              fons_index)
{
    SilvaToken* token;

    token = _token_novum(piscina);
    si (token == NIHIL)
    {
        redde NIHIL;
    }

    token->genus      = genus;
    token->valor      = valor;
    token->longitudo  = valor.mensura;
    token->fons_index = fons_index;

    token->origo.genus                = SILVA_ORIGO_API;
    token->origo.datum.api.nomen_macro = nomen_macro;

    redde token;
}


/* ==================================================
 * Quaestiones catenae
 * ================================================== */

interior SilvaToken*
_praedecessor_use_site (SilvaToken* token)
{
    commutatio (token->origo.genus)
    {
        casus SILVA_ORIGO_EXPANSIO:
            redde token->origo.datum.expansio.invocatio;
        casus SILVA_ORIGO_PASTA:
            redde token->origo.datum.pasta.sinister;
        casus SILVA_ORIGO_CHORDA:
            redde token->origo.datum.stringificatio.primus;
        casus SILVA_ORIGO_FONS:
        casus SILVA_ORIGO_API:
        ordinarius:
            redde NIHIL;
    }
}

SilvaToken*
silva_token_radix (SilvaToken* token)
{
    SilvaToken* praecedens;

    dum (token != NIHIL)
    {
        praecedens = _praedecessor_use_site(token);
        si (praecedens == NIHIL)
        {
            redde token;
        }
        token = praecedens;
    }
    redde NIHIL;
}

i32
silva_token_profunditas (SilvaToken* token)
{
    i32 profunditas;

    profunditas = ZEPHYRUM;
    dum (token != NIHIL)
    {
        SilvaToken* praecedens;

        praecedens = _praedecessor_use_site(token);
        si (praecedens == NIHIL)
        {
            frange;
        }
        profunditas = profunditas + I;
        token = praecedens;
    }
    redde profunditas;
}

b32
silva_token_est_fons (SilvaToken* token)
{
    redde (token->origo.genus == SILVA_ORIGO_FONS) ? VERUM : FALSUM;
}


/* ==================================================
 * Caecatio
 * ================================================== */

SilvaCaecatio*
silva_caecatio_extendere (
    SilvaPiscina*       piscina,
    SilvaCaecatio* parens,
    SilvaChorda*        titulus)
{
    SilvaCaecatio* cella;

    cella = (SilvaCaecatio*)silva_piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaCaecatio));
    si (cella == NIHIL)
    {
        redde parens;
    }
    cella->titulus = titulus;
    cella->cauda   = parens;
    redde cella;
}

b32
silva_caecatio_continet (
    SilvaCaecatio* caecatio,
    SilvaChorda         titulus)
{
    dum (caecatio != NIHIL)
    {
        si (caecatio->titulus != NIHIL
            && _chordae_pares(*caecatio->titulus, titulus))
        {
            redde VERUM;
        }
        caecatio = caecatio->cauda;
    }
    redde FALSUM;
}


/* ==================================================
 * Auxiliares
 * ================================================== */

hic_manens constans character* NOMINA_GENERUM[SILVA_LEX_NUMERUS_GENERUM] = {
    "EOF",
    "OCTETUS_IGNOTUS",
    "STRING_IMPERFECTUM",
    "CHARACTER_IMPERFECTUM",
    "IDENTIFICATOR",
    "INTEGER",
    "FLOAT",
    "CHARACTER_LIT",
    "STRING_LIT",
    "AUTO", "BREAK", "CASE", "CHAR", "CONST", "CONTINUE", "DEFAULT",
    "DO", "DOUBLE", "ELSE", "ENUM", "EXTERN", "FLOAT_KW", "FOR",
    "GOTO", "IF", "INT", "LONG", "REGISTER", "RETURN", "SHORT",
    "SIGNED", "SIZEOF", "STATIC", "STRUCT", "SWITCH", "TYPEDEF",
    "UNION", "UNSIGNED", "VOID", "VOLATILE", "WHILE",
    "QUADRA_APERTA", "QUADRA_CLAUSA",
    "PAREN_APERTA", "PAREN_CLAUSA",
    "BRACE_APERTA", "BRACE_CLAUSA",
    "PUNCTUM", "SAGITTA",
    "INCREMENTUM", "DECREMENTUM",
    "AMPERSAND", "STAR", "PLUS", "MINUS", "TILDE", "EXCLAMATIO",
    "SOLIDUS", "PERCENTUM",
    "SINISTRORSUM", "DEXTRORSUM",
    "MINOR", "MAIOR", "MINOR_AEQUALIS", "MAIOR_AEQUALIS",
    "AEQUALIS_AEQUALIS", "NON_AEQUALIS",
    "CARET", "BARRA", "ET_ET", "VEL_VEL",
    "QUAESTIO", "COLON", "SEMICOLON", "ELLIPSIS",
    "ASSIGNATIO",
    "STAR_ASSIGNATIO", "SOLIDUS_ASSIGNATIO", "PERCENTUM_ASSIGNATIO",
    "PLUS_ASSIGNATIO", "MINUS_ASSIGNATIO",
    "SINISTRORSUM_ASSIGNATIO", "DEXTRORSUM_ASSIGNATIO",
    "AMPERSAND_ASSIGNATIO", "CARET_ASSIGNATIO", "BARRA_ASSIGNATIO",
    "COMMA", "CANCELLUM", "CANCELLUM_CANCELLUM",
    "SPATIA", "TABULAE", "NOVA_LINEA", "CONTINUATIO",
    "COMMENTUM_CLAUSUM", "COMMENTUM_LINEA"
};

constans character*
silva_lexema_genus_nomen (SilvaLexemaGenus genus)
{
    si ((integer)genus >= ZEPHYRUM
        && (integer)genus < (integer)SILVA_LEX_NUMERUS_GENERUM)
    {
        redde NOMINA_GENERUM[genus];
    }
    redde "IGNOTUM";
}

constans character*
silva_origo_genus_nomen (SilvaOrigoGenus genus)
{
    commutatio (genus)
    {
        casus SILVA_ORIGO_FONS:     redde "FONS";
        casus SILVA_ORIGO_EXPANSIO: redde "EXPANSIO";
        casus SILVA_ORIGO_PASTA:    redde "PASTA";
        casus SILVA_ORIGO_CHORDA:   redde "CHORDA";
        casus SILVA_ORIGO_API:      redde "API";
        ordinarius:                 redde "IGNOTUM";
    }
}

/* ================= ex silva/fontes/silva_lexema.c ================= */

/* ==================================================
 * Status lexatoris (internus)
 * ================================================== */

nomen structura {
    constans character* fons;
    i32                 mensura;
    i32                 positus;
    i32                 linea;
    i32                 columna;
    SilvaPiscina*            piscina;
    s32                 fons_index;
} SilvaLexator;

/* Contextus scansionis unius lexematis (laminas colligit) */
nomen structura {
    i32  initium;        /* byte offset initii */
    i32  linea_initium;
    i32  columna_initium;
    i32  effectivi;      /* numerus characterum effectivorum sumptorum */
    SilvaXar* scissurae;      /* Xar de SilvaScissura; NIHIL si nullae */
} Scansio;


/* ==================================================
 * Verba clausa C89 (Anglica solum - vide caput)
 * ================================================== */

hic_manens constans structura {
    constans character* verbum;
    SilvaLexemaGenus    genus;
} VERBA_CLAUSA[] = {
    {"auto",     SILVA_LEX_AUTO},     {"break",    SILVA_LEX_BREAK},
    {"case",     SILVA_LEX_CASE},     {"char",     SILVA_LEX_CHAR},
    {"const",    SILVA_LEX_CONST},    {"continue", SILVA_LEX_CONTINUE},
    {"default",  SILVA_LEX_DEFAULT},  {"do",       SILVA_LEX_DO},
    {"double",   SILVA_LEX_DOUBLE},   {"else",     SILVA_LEX_ELSE},
    {"enum",     SILVA_LEX_ENUM},     {"extern",   SILVA_LEX_EXTERN},
    {"float",    SILVA_LEX_FLOAT_KW}, {"for",      SILVA_LEX_FOR},
    {"goto",     SILVA_LEX_GOTO},     {"if",       SILVA_LEX_IF},
    {"int",      SILVA_LEX_INT},      {"long",     SILVA_LEX_LONG},
    {"register", SILVA_LEX_REGISTER}, {"return",   SILVA_LEX_RETURN},
    {"short",    SILVA_LEX_SHORT},    {"signed",   SILVA_LEX_SIGNED},
    {"sizeof",   SILVA_LEX_SIZEOF},   {"static",   SILVA_LEX_STATIC},
    {"struct",   SILVA_LEX_STRUCT},   {"switch",   SILVA_LEX_SWITCH},
    {"typedef",  SILVA_LEX_TYPEDEF},  {"union",    SILVA_LEX_UNION},
    {"unsigned", SILVA_LEX_UNSIGNED}, {"void",     SILVA_LEX_VOID},
    {"volatile", SILVA_LEX_VOLATILE}, {"while",    SILVA_LEX_WHILE},
    {NIHIL,      SILVA_LEX_EOF}
};


/* ==================================================
 * Auxiliares - classificatio characterum
 * ================================================== */

interior b32
_est_cifra (character c)
{
    redde (c >= '0' && c <= '9') ? VERUM : FALSUM;
}

interior b32
_est_cifra_hex (character c)
{
    redde ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F')) ? VERUM : FALSUM;
}

interior b32
_est_littera (character c)
{
    redde ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        ? VERUM : FALSUM;
}

interior b32
_est_corpus_identificatoris (character c)
{
    redde (_est_littera(c) || _est_cifra(c)) ? VERUM : FALSUM;
}


/* ==================================================
 * Auxiliares - chorda ex fonte (visus, nulla copia)
 * chorda.datum est i8* sed fons constans est: unio castum
 * celat; nemo per visum scribit (contractus)
 * ================================================== */

interior SilvaChorda
_chorda_ex_fonte (constans character* fons, i32 mensura)
{
    SilvaChorda c;
    unio { constans character* c; i8* m; } u;

    u.c = fons;
    c.datum = u.m;
    c.mensura = mensura;
    redde c;
}

interior SilvaChorda
_subchorda (SilvaChorda c, i32 a, i32 b)
{
    SilvaChorda pars;

    pars.datum = c.datum + a;
    pars.mensura = b - a;
    redde pars;
}


/* ==================================================
 * Auxiliares - positio et lectio cruda
 * ================================================== */

interior b32
_finis (SilvaLexator* lex)
{
    redde (lex->positus >= lex->mensura) ? VERUM : FALSUM;
}

interior character
_aspicere_crudum (SilvaLexator* lex, i32 offset)
{
    i32 pos;

    pos = lex->positus + offset;
    si (pos < ZEPHYRUM || pos >= lex->mensura)
    {
        redde '\0';
    }
    redde lex->fons[pos];
}

interior vacuum
_progredi_crudum (SilvaLexator* lex, i32 n)
{
    i32 i;

    per (i = ZEPHYRUM; i < n && !_finis(lex); i++)
    {
        si (lex->fons[lex->positus] == '\n')
        {
            lex->linea++;
            lex->columna = I;
        }
        alioquin
        {
            lex->columna++;
        }
        lex->positus++;
    }
}

/* Longitudo laminae ad positionem datam: 0 (nulla), 2 (\ \n), 3 (\ \r\n) */
interior i32
_lamina_hic (SilvaLexator* lex, i32 offset)
{
    character c;
    character c2;

    c = _aspicere_crudum(lex, offset);
    si (c != '\\')
    {
        redde ZEPHYRUM;
    }
    c2 = _aspicere_crudum(lex, offset + I);
    si (c2 == '\n')
    {
        redde II;
    }
    si (c2 == '\r' && _aspicere_crudum(lex, offset + II) == '\n')
    {
        redde III;
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * Lectio EFFECTIVA - laminas transilit
 * ================================================== */

/* Character effectivus n-us ab positione currenti (laminis transilitis) */
interior character
_aspicere_eff (SilvaLexator* lex, i32 n)
{
    i32 pos;
    i32 restant;
    i32 lam;
    character c;

    pos = ZEPHYRUM;
    restant = n;
    dum (VERUM)
    {
        lam = _lamina_hic(lex, pos);
        dum (lam > ZEPHYRUM)
        {
            pos = pos + lam;
            lam = _lamina_hic(lex, pos);
        }
        si (lex->positus + pos >= lex->mensura)
        {
            redde '\0';
        }
        c = lex->fons[lex->positus + pos];
        si (restant == ZEPHYRUM)
        {
            redde c;
        }
        restant--;
        pos++;
    }
}

/* Estne finis effectivus? (solae laminae usque ad finem restant) */
interior b32
_finis_eff (SilvaLexator* lex)
{
    i32 pos;
    i32 lam;

    pos = ZEPHYRUM;
    lam = _lamina_hic(lex, pos);
    dum (lam > ZEPHYRUM)
    {
        pos = pos + lam;
        lam = _lamina_hic(lex, pos);
    }
    redde (lex->positus + pos >= lex->mensura) ? VERUM : FALSUM;
}

/* Sumere unum characterem effectivum: laminas ante eum consumit
 * et in scansione memorat */
interior vacuum
_sumere (SilvaLexator* lex, Scansio* s)
{
    i32 lam;
    SilvaScissura* locus;

    lam = _lamina_hic(lex, ZEPHYRUM);
    dum (lam > ZEPHYRUM)
    {
        si (s->scissurae == NIHIL)
        {
            s->scissurae = silva_xar_creare(lex->piscina, magnitudo(SilvaScissura));
        }
        locus = (SilvaScissura*)silva_xar_addere(s->scissurae);
        si (locus != NIHIL)
        {
            locus->offset = (s32)s->effectivi;
            locus->crlf = (lam == III) ? VERUM : FALSUM;
        }
        _progredi_crudum(lex, lam);
        lam = _lamina_hic(lex, ZEPHYRUM);
    }
    si (!_finis(lex))
    {
        _progredi_crudum(lex, I);
        s->effectivi++;
    }
}

interior vacuum
_scansio_incipere (SilvaLexator* lex, Scansio* s)
{
    s->initium = lex->positus;
    s->linea_initium = lex->linea;
    s->columna_initium = lex->columna;
    s->effectivi = ZEPHYRUM;
    s->scissurae = NIHIL;
}

/* Valor lexematis: visus directus si nullae laminae, aliter textus
 * mundus synthesitur (laminae exclusae) */
interior SilvaChorda
_valor_finire (SilvaLexator* lex, Scansio* s)
{
    SilvaChorda mundus;
    i8* datum;
    i32 i;
    i32 scriptum;

    si (s->scissurae == NIHIL)
    {
        redde _chorda_ex_fonte(lex->fons + s->initium,
            lex->positus - s->initium);
    }

    datum = (i8*)silva_piscina_allocare(lex->piscina,
        (memoriae_index)s->effectivi);
    scriptum = ZEPHYRUM;
    i = s->initium;
    dum (i < lex->positus && scriptum < s->effectivi)
    {
        /* transilire laminas in textu crudo */
        si (lex->fons[i] == '\\' && i + I < lex->positus
            && lex->fons[i + I] == '\n')
        {
            i = i + II;
            perge;
        }
        si (lex->fons[i] == '\\' && i + II < lex->positus
            && lex->fons[i + I] == '\r' && lex->fons[i + II] == '\n')
        {
            i = i + III;
            perge;
        }
        datum[scriptum] = (i8)lex->fons[i];
        scriptum++;
        i++;
    }
    mundus.datum = datum;
    mundus.mensura = scriptum;
    redde mundus;
}

interior SilvaToken*
_lexema_finire (SilvaLexator* lex, Scansio* s, SilvaLexemaGenus genus)
{
    SilvaToken* token;

    token = silva_token_ex_fonte(lex->piscina, genus, _valor_finire(lex, s),
        (s32)s->initium, s->linea_initium, s->columna_initium, lex->fons_index);
    si (token != NIHIL)
    {
        token->longitudo = lex->positus - s->initium; /* longitudo CRUDA */
        token->scissurae = s->scissurae;
    }
    redde token;
}


/* ==================================================
 * Verba clausa
 * ================================================== */

interior SilvaLexemaGenus
_quaerere_verbum_clausum (SilvaChorda valor)
{
    i32 i;

    per (i = ZEPHYRUM; VERBA_CLAUSA[i].verbum != NIHIL; i++)
    {
        si ((i32)strlen(VERBA_CLAUSA[i].verbum) == valor.mensura
            && memcmp(VERBA_CLAUSA[i].verbum, valor.datum,
                   (memoriae_index)valor.mensura) == ZEPHYRUM)
        {
            redde VERBA_CLAUSA[i].genus;
        }
    }
    redde SILVA_LEX_IDENTIFICATOR;
}


/* ==================================================
 * Scanner triviae UNUS
 * Reddit lexema triviae proximum, vel NIHIL si non-trivia sequitur.
 * Valores triviae semper CRUDI (verbatim - fidelitas octetorum).
 * Commentum lineae novam lineam NUMQUAM consumit (fons divergentiae
 * arbor2 mortuus).
 * ================================================== */

interior SilvaToken*
_trivia_creare (SilvaLexator* lex, SilvaLexemaGenus genus,
                i32 initium, i32 linea_i, i32 columna_i)
{
    redde silva_token_ex_fonte(lex->piscina, genus,
        _chorda_ex_fonte(lex->fons + initium, lex->positus - initium),
        (s32)initium, linea_i, columna_i, lex->fons_index);
}

interior SilvaToken*
_trivia_proxima (SilvaLexator* lex)
{
    character c;
    character c2;
    i32 initium;
    i32 linea_i;
    i32 columna_i;
    i32 lam;
    SilvaToken* trivia;

    si (_finis(lex))
    {
        redde NIHIL;
    }

    c = _aspicere_crudum(lex, ZEPHYRUM);
    c2 = _aspicere_crudum(lex, I);
    initium = lex->positus;
    linea_i = lex->linea;
    columna_i = lex->columna;

    /* Continuatio INTER lexemata (lamina nudae positionis) */
    lam = _lamina_hic(lex, ZEPHYRUM);
    si (lam > ZEPHYRUM)
    {
        _progredi_crudum(lex, lam);
        redde _trivia_creare(lex, SILVA_LEX_CONTINUATIO,
            initium, linea_i, columna_i);
    }

    /* Spatia */
    si (c == ' ')
    {
        dum (!_finis(lex) && _aspicere_crudum(lex, ZEPHYRUM) == ' ')
        {
            _progredi_crudum(lex, I);
        }
        redde _trivia_creare(lex, SILVA_LEX_SPATIA,
            initium, linea_i, columna_i);
    }

    /* Tabulae */
    si (c == '\t')
    {
        dum (!_finis(lex) && _aspicere_crudum(lex, ZEPHYRUM) == '\t')
        {
            _progredi_crudum(lex, I);
        }
        redde _trivia_creare(lex, SILVA_LEX_TABULAE,
            initium, linea_i, columna_i);
    }

    /* Commentum clausum - lectio effectiva (apertura et clausura
     * laminari possunt); valor crudus. Non terminatum -> ad finem
     * (totalitas: numquam frangit) */
    si (_aspicere_eff(lex, ZEPHYRUM) == '/' && _aspicere_eff(lex, I) == '*')
    {
        Scansio s;

        _scansio_incipere(lex, &s);
        _sumere(lex, &s);  /* / */
        _sumere(lex, &s);  /* * */
        dum (!_finis_eff(lex))
        {
            si (_aspicere_eff(lex, ZEPHYRUM) == '*'
                && _aspicere_eff(lex, I) == '/')
            {
                _sumere(lex, &s);
                _sumere(lex, &s);
                frange;
            }
            _sumere(lex, &s);
        }
        /* laminae in triviis crudae manent - scissurae abiciuntur */
        redde _trivia_creare(lex, SILVA_LEX_COMMENTUM_CLAUSUM,
            initium, linea_i, columna_i);
    }

    /* Commentum lineae (C99 - signatum); novam lineam NON consumit */
    si (_aspicere_eff(lex, ZEPHYRUM) == '/' && _aspicere_eff(lex, I) == '/')
    {
        Scansio s;
        character ce;

        _scansio_incipere(lex, &s);
        _sumere(lex, &s);
        _sumere(lex, &s);
        dum (!_finis_eff(lex))
        {
            ce = _aspicere_eff(lex, ZEPHYRUM);
            si (ce == '\n' || ce == '\r')
            {
                frange;
            }
            _sumere(lex, &s);
        }
        trivia = _trivia_creare(lex, SILVA_LEX_COMMENTUM_LINEA,
            initium, linea_i, columna_i);
        si (trivia != NIHIL)
        {
            trivia->standard = SILVA_STANDARD_C99;
        }
        redde trivia;
    }

    /* Nova linea: \n, \r\n, \r nuda - valor verbatim */
    si (c == '\n')
    {
        _progredi_crudum(lex, I);
        redde _trivia_creare(lex, SILVA_LEX_NOVA_LINEA,
            initium, linea_i, columna_i);
    }
    si (c == '\r')
    {
        _progredi_crudum(lex, (c2 == '\n') ? II : I);
        redde _trivia_creare(lex, SILVA_LEX_NOVA_LINEA,
            initium, linea_i, columna_i);
    }

    redde NIHIL;
}


/* ==================================================
 * Lectores lexematum verorum (omnes effectivi)
 * ================================================== */

interior SilvaToken*
_legere_identificatorem (SilvaLexator* lex)
{
    Scansio s;
    SilvaToken* token;
    SilvaChorda valor;
    SilvaLexemaGenus genus;

    _scansio_incipere(lex, &s);
    dum (!_finis_eff(lex)
        && _est_corpus_identificatoris(_aspicere_eff(lex, ZEPHYRUM)))
    {
        _sumere(lex, &s);
    }

    valor = _valor_finire(lex, &s);
    genus = _quaerere_verbum_clausum(valor);

    token = silva_token_ex_fonte(lex->piscina, genus, valor,
        (s32)s.initium, s.linea_initium, s.columna_initium, lex->fons_index);
    si (token != NIHIL)
    {
        token->longitudo = lex->positus - s.initium;
        token->scissurae = s.scissurae;
    }
    redde token;
}

interior SilvaToken*
_legere_numerum (SilvaLexator* lex)
{
    Scansio s;
    character c;
    character c2;
    b32 est_hex;
    b32 est_fluitans;

    _scansio_incipere(lex, &s);
    est_hex = FALSUM;
    est_fluitans = FALSUM;

    c = _aspicere_eff(lex, ZEPHYRUM);

    /* Punctum initiale: .5 */
    si (c == '.')
    {
        est_fluitans = VERUM;
        _sumere(lex, &s);  /* . */
        dum (!_finis_eff(lex) && _est_cifra(_aspicere_eff(lex, ZEPHYRUM)))
        {
            _sumere(lex, &s);
        }
    }
    alioquin si (c == '0'
        && (_aspicere_eff(lex, I) == 'x' || _aspicere_eff(lex, I) == 'X'))
    {
        est_hex = VERUM;
        _sumere(lex, &s);
        _sumere(lex, &s);
        dum (!_finis_eff(lex) && _est_cifra_hex(_aspicere_eff(lex, ZEPHYRUM)))
        {
            _sumere(lex, &s);
        }
    }
    alioquin
    {
        /* decimales et octales: cifras omnes sumere (0-9; semantica
         * octalis posterioris curae est, lexema idem manet) */
        dum (!_finis_eff(lex) && _est_cifra(_aspicere_eff(lex, ZEPHYRUM)))
        {
            _sumere(lex, &s);
        }

        /* pars fractionalis */
        si (!_finis_eff(lex) && _aspicere_eff(lex, ZEPHYRUM) == '.'
            && _est_cifra(_aspicere_eff(lex, I)))
        {
            est_fluitans = VERUM;
            _sumere(lex, &s);
            dum (!_finis_eff(lex) && _est_cifra(_aspicere_eff(lex, ZEPHYRUM)))
            {
                _sumere(lex, &s);
            }
        }
        alioquin si (!_finis_eff(lex) && _aspicere_eff(lex, ZEPHYRUM) == '.'
            && !_est_cifra(_aspicere_eff(lex, I))
            && _aspicere_eff(lex, I) != '.')
        {
            /* 3. -> fluitans (C89: cifrae punctum sequens optionales),
             * sed 3.. relinquitur (punctum punctum) */
            est_fluitans = VERUM;
            _sumere(lex, &s);
        }
    }

    /* exponens */
    si (!est_hex)
    {
        c = _aspicere_eff(lex, ZEPHYRUM);
        si (c == 'e' || c == 'E')
        {
            c2 = _aspicere_eff(lex, I);
            si (_est_cifra(c2)
                || ((c2 == '+' || c2 == '-') && _est_cifra(_aspicere_eff(lex, II))))
            {
                est_fluitans = VERUM;
                _sumere(lex, &s);  /* e */
                c = _aspicere_eff(lex, ZEPHYRUM);
                si (c == '+' || c == '-')
                {
                    _sumere(lex, &s);
                }
                dum (!_finis_eff(lex) && _est_cifra(_aspicere_eff(lex, ZEPHYRUM)))
                {
                    _sumere(lex, &s);
                }
            }
        }
    }

    /* suffixus */
    c = _aspicere_eff(lex, ZEPHYRUM);
    si (est_fluitans)
    {
        si (c == 'f' || c == 'F' || c == 'l' || c == 'L')
        {
            _sumere(lex, &s);
        }
    }
    alioquin
    {
        si (c == 'u' || c == 'U')
        {
            _sumere(lex, &s);
            c = _aspicere_eff(lex, ZEPHYRUM);
            si (c == 'l' || c == 'L')
            {
                _sumere(lex, &s);
            }
        }
        alioquin si (c == 'l' || c == 'L')
        {
            _sumere(lex, &s);
            c = _aspicere_eff(lex, ZEPHYRUM);
            si (c == 'u' || c == 'U')
            {
                _sumere(lex, &s);
            }
        }
    }

    redde _lexema_finire(lex, &s,
        est_fluitans ? SILVA_LEX_FLOAT : SILVA_LEX_INTEGER);
}

/* Littera chordae vel characteris. terminator = '"' vel '\''.
 * Nova linea effectiva vel finis -> IMPERFECTUM (non consumitur). */
interior SilvaToken*
_legere_litteram (SilvaLexator* lex, character terminator,
                  SilvaLexemaGenus genus_perfectum,
                  SilvaLexemaGenus genus_imperfectum)
{
    Scansio s;
    character c;
    b32 perfectum;

    _scansio_incipere(lex, &s);
    perfectum = FALSUM;

    _sumere(lex, &s);  /* terminator aperiens */
    dum (!_finis_eff(lex))
    {
        c = _aspicere_eff(lex, ZEPHYRUM);
        si (c == terminator)
        {
            _sumere(lex, &s);
            perfectum = VERUM;
            frange;
        }
        si (c == '\n' || c == '\r')
        {
            frange;  /* imperfectum - novam lineam non consumere */
        }
        si (c == '\\')
        {
            _sumere(lex, &s);  /* \ */
            si (!_finis_eff(lex))
            {
                _sumere(lex, &s);  /* character effugii */
            }
            perge;
        }
        _sumere(lex, &s);
    }

    redde _lexema_finire(lex, &s,
        perfectum ? genus_perfectum : genus_imperfectum);
}

/* Interpunctio - munch maximalis, lectio effectiva */
interior SilvaToken*
_legere_interpunctionem (SilvaLexator* lex)
{
    Scansio s;
    character c;
    character c2;
    character c3;
    SilvaLexemaGenus genus;
    i32 n;

    _scansio_incipere(lex, &s);
    c  = _aspicere_eff(lex, ZEPHYRUM);
    c2 = _aspicere_eff(lex, I);
    c3 = _aspicere_eff(lex, II);
    n = I;
    genus = SILVA_LEX_OCTETUS_IGNOTUS;

    /* tres characteres */
    si (c == '.' && c2 == '.' && c3 == '.')      { genus = SILVA_LEX_ELLIPSIS; n = III; }
    alioquin si (c == '<' && c2 == '<' && c3 == '=') { genus = SILVA_LEX_SINISTRORSUM_ASSIGNATIO; n = III; }
    alioquin si (c == '>' && c2 == '>' && c3 == '=') { genus = SILVA_LEX_DEXTRORSUM_ASSIGNATIO; n = III; }
    /* duo characteres */
    alioquin si (c == '+' && c2 == '+') { genus = SILVA_LEX_INCREMENTUM; n = II; }
    alioquin si (c == '-' && c2 == '-') { genus = SILVA_LEX_DECREMENTUM; n = II; }
    alioquin si (c == '-' && c2 == '>') { genus = SILVA_LEX_SAGITTA; n = II; }
    alioquin si (c == '&' && c2 == '&') { genus = SILVA_LEX_ET_ET; n = II; }
    alioquin si (c == '|' && c2 == '|') { genus = SILVA_LEX_VEL_VEL; n = II; }
    alioquin si (c == '=' && c2 == '=') { genus = SILVA_LEX_AEQUALIS_AEQUALIS; n = II; }
    alioquin si (c == '!' && c2 == '=') { genus = SILVA_LEX_NON_AEQUALIS; n = II; }
    alioquin si (c == '<' && c2 == '=') { genus = SILVA_LEX_MINOR_AEQUALIS; n = II; }
    alioquin si (c == '>' && c2 == '=') { genus = SILVA_LEX_MAIOR_AEQUALIS; n = II; }
    alioquin si (c == '<' && c2 == '<') { genus = SILVA_LEX_SINISTRORSUM; n = II; }
    alioquin si (c == '>' && c2 == '>') { genus = SILVA_LEX_DEXTRORSUM; n = II; }
    alioquin si (c == '+' && c2 == '=') { genus = SILVA_LEX_PLUS_ASSIGNATIO; n = II; }
    alioquin si (c == '-' && c2 == '=') { genus = SILVA_LEX_MINUS_ASSIGNATIO; n = II; }
    alioquin si (c == '*' && c2 == '=') { genus = SILVA_LEX_STAR_ASSIGNATIO; n = II; }
    alioquin si (c == '/' && c2 == '=') { genus = SILVA_LEX_SOLIDUS_ASSIGNATIO; n = II; }
    alioquin si (c == '%' && c2 == '=') { genus = SILVA_LEX_PERCENTUM_ASSIGNATIO; n = II; }
    alioquin si (c == '&' && c2 == '=') { genus = SILVA_LEX_AMPERSAND_ASSIGNATIO; n = II; }
    alioquin si (c == '|' && c2 == '=') { genus = SILVA_LEX_BARRA_ASSIGNATIO; n = II; }
    alioquin si (c == '^' && c2 == '=') { genus = SILVA_LEX_CARET_ASSIGNATIO; n = II; }
    alioquin si (c == '#' && c2 == '#') { genus = SILVA_LEX_CANCELLUM_CANCELLUM; n = II; }
    /* unus character */
    alioquin si (c == '+') { genus = SILVA_LEX_PLUS; }
    alioquin si (c == '-') { genus = SILVA_LEX_MINUS; }
    alioquin si (c == '*') { genus = SILVA_LEX_STAR; }
    alioquin si (c == '/') { genus = SILVA_LEX_SOLIDUS; }
    alioquin si (c == '%') { genus = SILVA_LEX_PERCENTUM; }
    alioquin si (c == '&') { genus = SILVA_LEX_AMPERSAND; }
    alioquin si (c == '|') { genus = SILVA_LEX_BARRA; }
    alioquin si (c == '^') { genus = SILVA_LEX_CARET; }
    alioquin si (c == '~') { genus = SILVA_LEX_TILDE; }
    alioquin si (c == '!') { genus = SILVA_LEX_EXCLAMATIO; }
    alioquin si (c == '<') { genus = SILVA_LEX_MINOR; }
    alioquin si (c == '>') { genus = SILVA_LEX_MAIOR; }
    alioquin si (c == '=') { genus = SILVA_LEX_ASSIGNATIO; }
    alioquin si (c == '.') { genus = SILVA_LEX_PUNCTUM; }
    alioquin si (c == ',') { genus = SILVA_LEX_COMMA; }
    alioquin si (c == ':') { genus = SILVA_LEX_COLON; }
    alioquin si (c == ';') { genus = SILVA_LEX_SEMICOLON; }
    alioquin si (c == '?') { genus = SILVA_LEX_QUAESTIO; }
    alioquin si (c == '(') { genus = SILVA_LEX_PAREN_APERTA; }
    alioquin si (c == ')') { genus = SILVA_LEX_PAREN_CLAUSA; }
    alioquin si (c == '[') { genus = SILVA_LEX_QUADRA_APERTA; }
    alioquin si (c == ']') { genus = SILVA_LEX_QUADRA_CLAUSA; }
    alioquin si (c == '{') { genus = SILVA_LEX_BRACE_APERTA; }
    alioquin si (c == '}') { genus = SILVA_LEX_BRACE_CLAUSA; }
    alioquin si (c == '#') { genus = SILVA_LEX_CANCELLUM; }
    /* aliter: octetus ignotus (garbage, NUL, ...) - UNUS octetus */

    per (; n > ZEPHYRUM; n--)
    {
        _sumere(lex, &s);
    }

    redde _lexema_finire(lex, &s, genus);
}

/* Lexema verum proximum (numquam trivia; EOF ad finem) */
interior SilvaToken*
_lexema_proximum (SilvaLexator* lex)
{
    character c;
    character c2;

    si (_finis(lex))
    {
        SilvaChorda vacua;

        vacua.datum = NIHIL;
        vacua.mensura = ZEPHYRUM;
        redde silva_token_ex_fonte(lex->piscina, SILVA_LEX_EOF, vacua,
            (s32)lex->positus, lex->linea, lex->columna, lex->fons_index);
    }

    c = _aspicere_eff(lex, ZEPHYRUM);
    c2 = _aspicere_eff(lex, I);

    si (_est_littera(c))
    {
        redde _legere_identificatorem(lex);
    }
    si (_est_cifra(c) || (c == '.' && _est_cifra(c2)))
    {
        redde _legere_numerum(lex);
    }
    si (c == '"')
    {
        redde _legere_litteram(lex, '"',
            SILVA_LEX_STRING_LIT, SILVA_LEX_STRING_IMPERFECTUM);
    }
    si (c == '\'')
    {
        redde _legere_litteram(lex, '\'',
            SILVA_LEX_CHARACTER_LIT, SILVA_LEX_CHARACTER_IMPERFECTUM);
    }
    redde _legere_interpunctionem(lex);
}


/* ==================================================
 * Fluxus crudus + initium_lineae
 * ================================================== */

SilvaXar*
silva_lexare_cruda (
    SilvaPiscina*            piscina,
    constans character* fons,
    i32                 mensura,
    s32                 fons_index)
{
    SilvaLexator lex;
    SilvaXar* cruda;
    SilvaToken* token;
    SilvaToken** locus;
    b32 in_initio_lineae;

    lex.fons = fons;
    lex.mensura = mensura;
    lex.positus = ZEPHYRUM;
    lex.linea = I;
    lex.columna = I;
    lex.piscina = piscina;
    lex.fons_index = fons_index;

    cruda = silva_xar_creare(piscina, magnitudo(SilvaToken*));
    in_initio_lineae = VERUM;

    dum (VERUM)
    {
        token = _trivia_proxima(&lex);
        si (token != NIHIL)
        {
            /* NOVA_LINEA incipit lineam logicam novam;
             * CONTINUATIO lineam logicam CONTINUAT (vexillum immutatum) */
            si (token->genus == SILVA_LEX_NOVA_LINEA)
            {
                in_initio_lineae = VERUM;
            }
            locus = (SilvaToken**)silva_xar_addere(cruda);
            si (locus != NIHIL)
            {
                *locus = token;
            }
            perge;
        }

        token = _lexema_proximum(&lex);
        si (token == NIHIL)
        {
            frange;  /* allocatio fracta - fluxum partialem reddere */
        }
        token->initium_lineae = in_initio_lineae;
        in_initio_lineae = FALSUM;
        locus = (SilvaToken**)silva_xar_addere(cruda);
        si (locus != NIHIL)
        {
            *locus = token;
        }
        si (token->genus == SILVA_LEX_EOF)
        {
            frange;
        }
    }

    redde cruda;
}


/* ==================================================
 * Transitus attachmenti
 * Regula: trailing lexematis praecedentis = trivia usque ad primam
 * NOVA_LINEA inclusive; reliqua = leading lexematis sequentis.
 * Trivia ante lexema primum -> leading eius.
 * ================================================== */

SilvaXar*
silva_spatia_attachere (
    SilvaPiscina* piscina,
    SilvaXar*     cruda)
{
    SilvaXar* vera;
    SilvaXar* pendentia;
    SilvaToken* token;
    SilvaToken* prior;
    SilvaToken** locus;
    i32 i;
    i32 j;
    i32 n;
    i32 divisio;

    vera = silva_xar_creare(piscina, magnitudo(SilvaToken*));
    pendentia = silva_xar_creare(piscina, magnitudo(SilvaToken*));
    prior = NIHIL;

    n = silva_xar_numerus(cruda);
    per (i = ZEPHYRUM; i < n; i++)
    {
        token = *(SilvaToken**)silva_xar_obtinere(cruda, i);

        si (token->genus == SILVA_LEX_SPATIA
            || token->genus == SILVA_LEX_TABULAE
            || token->genus == SILVA_LEX_NOVA_LINEA
            || token->genus == SILVA_LEX_CONTINUATIO
            || token->genus == SILVA_LEX_COMMENTUM_CLAUSUM
            || token->genus == SILVA_LEX_COMMENTUM_LINEA)
        {
            locus = (SilvaToken**)silva_xar_addere(pendentia);
            si (locus != NIHIL)
            {
                *locus = token;
            }
            perge;
        }

        /* Lexema verum: pendentia dividere */
        si (silva_xar_numerus(pendentia) > ZEPHYRUM)
        {
            /* divisio = index post primam NOVA_LINEA (si prior est);
             * aliter omnia ad leading (nihil trailing sine linea nova...
             * immo: si prior NIHIL, omnia leading) */
            divisio = ZEPHYRUM;
            si (prior != NIHIL)
            {
                divisio = silva_xar_numerus(pendentia);  /* omnia trailing si nulla nova linea */
                per (j = ZEPHYRUM; j < silva_xar_numerus(pendentia); j++)
                {
                    SilvaToken* t;

                    t = *(SilvaToken**)silva_xar_obtinere(pendentia, j);
                    si (t->genus == SILVA_LEX_NOVA_LINEA)
                    {
                        divisio = j + I;
                        frange;
                    }
                }
            }

            si (divisio > ZEPHYRUM && prior != NIHIL)
            {
                prior->spatia_post = silva_xar_creare(piscina, magnitudo(SilvaToken*));
                per (j = ZEPHYRUM; j < divisio; j++)
                {
                    locus = (SilvaToken**)silva_xar_addere(prior->spatia_post);
                    si (locus != NIHIL)
                    {
                        *locus = *(SilvaToken**)silva_xar_obtinere(pendentia, j);
                    }
                }
            }
            si (divisio < silva_xar_numerus(pendentia))
            {
                token->spatia_ante = silva_xar_creare(piscina, magnitudo(SilvaToken*));
                per (j = divisio; j < silva_xar_numerus(pendentia); j++)
                {
                    locus = (SilvaToken**)silva_xar_addere(token->spatia_ante);
                    si (locus != NIHIL)
                    {
                        *locus = *(SilvaToken**)silva_xar_obtinere(pendentia, j);
                    }
                }
            }
            pendentia = silva_xar_creare(piscina, magnitudo(SilvaToken*));
        }

        locus = (SilvaToken**)silva_xar_addere(vera);
        si (locus != NIHIL)
        {
            *locus = token;
        }
        prior = token;
    }

    redde vera;
}


/* ==================================================
 * API principalis
 * ================================================== */

SilvaXar*
silva_lexare (
    SilvaPiscina*            piscina,
    constans character* fons,
    i32                 mensura,
    s32                 fons_index)
{
    redde silva_spatia_attachere(piscina,
        silva_lexare_cruda(piscina, fons, mensura, fons_index));
}


/* ==================================================
 * Emissio - oraculum fidelitatis octetorum
 * ================================================== */

interior vacuum
_emittere_trivia (SilvaChordaAedificator* aed, SilvaXar* spatia)
{
    i32 i;
    SilvaToken* t;

    si (spatia == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < silva_xar_numerus(spatia); i++)
    {
        t = *(SilvaToken**)silva_xar_obtinere(spatia, i);
        silva_chorda_aedificator_appendere_chorda(aed, t->valor);
    }
}

interior vacuum
_emittere_valorem (SilvaChordaAedificator* aed, SilvaToken* token)
{
    i32 i;
    i32 prius;
    SilvaScissura* sc;

    si (token->scissurae == NIHIL)
    {
        silva_chorda_aedificator_appendere_chorda(aed, token->valor);
        redde;
    }

    prius = ZEPHYRUM;
    per (i = ZEPHYRUM; i < silva_xar_numerus(token->scissurae); i++)
    {
        sc = (SilvaScissura*)silva_xar_obtinere(token->scissurae, i);
        silva_chorda_aedificator_appendere_chorda(aed,
            _subchorda(token->valor, prius, (i32)sc->offset));
        silva_chorda_aedificator_appendere_literis(aed, sc->crlf ? "\\\r\n" : "\\\n");
        prius = (i32)sc->offset;
    }
    silva_chorda_aedificator_appendere_chorda(aed,
        _subchorda(token->valor, prius, token->valor.mensura));
}

vacuum
silva_lexema_emittere_in (
    SilvaChordaAedificator* aed,
    SilvaToken*        token)
{
    si (aed == NIHIL || token == NIHIL)
    {
        redde;
    }
    _emittere_trivia(aed, token->spatia_ante);
    _emittere_valorem(aed, token);
    _emittere_trivia(aed, token->spatia_post);
}

SilvaChorda
silva_lexemata_emittere (
    SilvaPiscina* piscina,
    SilvaXar*     lexemata)
{
    SilvaChordaAedificator* aed;
    SilvaToken* token;
    i32 i;

    aed = silva_chorda_aedificator_creare(piscina, 1024);
    per (i = ZEPHYRUM; i < silva_xar_numerus(lexemata); i++)
    {
        token = *(SilvaToken**)silva_xar_obtinere(lexemata, i);
        silva_lexema_emittere_in(aed, token);
    }
    redde silva_chorda_aedificator_finire(aed);
}

/* ================= ex silva/fontes/silva_expandere.c ================= */

/* ==================================================
 * Auxiliares
 * ================================================== */

interior b32
_chorda_est_literis (SilvaChorda c, constans character* literis)
{
    i32 mensura;

    mensura = (i32)strlen(literis);
    si (c.mensura != mensura)
    {
        redde FALSUM;
    }
    si (mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (memcmp(c.datum, literis, (memoriae_index)mensura) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

interior SilvaChorda*
_chorda_figere (SilvaPiscina* piscina, SilvaChorda c)
{
    SilvaChorda* fixa;

    fixa = (SilvaChorda*)silva_piscina_allocare(piscina, (memoriae_index)magnitudo(SilvaChorda));
    si (fixa != NIHIL)
    {
        *fixa = c;
    }
    redde fixa;
}


/* ==================================================
 * Contextus
 * ================================================== */

SilvaExpansio*
silva_expansio_creare (SilvaPiscina* piscina)
{
    SilvaExpansio* exp;

    exp = (SilvaExpansio*)silva_piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaExpansio));
    si (exp == NIHIL)
    {
        redde NIHIL;
    }
    exp->piscina = piscina;
    exp->fontes = silva_xar_creare(piscina, magnitudo(SilvaFons));
    exp->macros = silva_tabula_dispersa_creare_chorda(piscina, LXIV);
    exp->acta = silva_xar_creare(piscina, magnitudo(SilvaEventum));
    exp->rami = silva_xar_creare(piscina, magnitudo(SilvaRamus*));
    exp->regiones = silva_xar_creare(piscina, magnitudo(SilvaRegio*));
    exp->includenda = silva_tabula_dispersa_creare_chorda(piscina, XVI);
    exp->inclusiones = silva_xar_creare(piscina, magnitudo(SilvaInclusio));
    exp->extenta = silva_xar_creare(piscina,
        magnitudo(SilvaExtentumInvocationis));
    exp->profunditas_includendi = ZEPHYRUM;
    exp->fons_api = -I;
    exp->tabula_activa = NIHIL;
    exp->limen_lexematum = SILVA_LIMEN_LEXEMATUM_DEFALTUM;
    exp->limen_generationum = SILVA_LIMEN_GENERATIONUM_DEFALTUM;
    exp->limen_includendi = SILVA_LIMEN_INCLUDENDI_DEFALTUM;
    exp->limen_regionum = SILVA_LIMEN_REGIONUM_DEFALTUM;
    exp->pergere = NIHIL;
    exp->pergere_datum = NIHIL;
    exp->expansio_decisa = FALSUM;
    exp->est_intermissa = FALSUM;
    exp->fines_tactae = FALSUM;
    exp->profunditas_regionum = ZEPHYRUM;
    redde exp;
}

s32
silva_fons_addere (
    SilvaExpansio*      exp,
    constans character* via,
    b32                 est_syntheticus)
{
    SilvaFons* locus;
    SilvaChorda* via_fixa;
    SilvaChorda temp;
    i8* datum_novum;
    i32 mensura;

    /* via in piscinam duplicatur - vocans chordam suam liberare potest */
    mensura = (i32)strlen(via);
    datum_novum = (i8*)silva_piscina_allocare(exp->piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (datum_novum == NIHIL)
    {
        redde -I;
    }
    si (mensura > ZEPHYRUM)
    {
        memcpy(datum_novum, via, (memoriae_index)mensura);
    }
    temp.datum = datum_novum;
    temp.mensura = mensura;
    via_fixa = _chorda_figere(exp->piscina, temp);

    locus = (SilvaFons*)silva_xar_addere(exp->fontes);
    si (locus == NIHIL)
    {
        redde -I;
    }
    locus->via = via_fixa;
    locus->est_syntheticus = est_syntheticus;
    locus->est_custos = FALSUM;
    locus->custos_titulus = NIHIL;
    redde (s32)(silva_xar_numerus(exp->fontes) - I);
}


/* ==================================================
 * Acta
 * ================================================== */

interior vacuum
_eventum_scribere (
    SilvaExpansio*    exp,
    SilvaEventumGenus genus,
    s32               fons_index,
    i32               linea,
    SilvaChorda*           titulus,
    SilvaMacroDef*    def,
    i32               conditio_id,
    s32               positus)
{
    SilvaEventum* locus;

    locus = (SilvaEventum*)silva_xar_addere(exp->acta);
    si (locus == NIHIL)
    {
        redde;
    }
    locus->genus = genus;
    locus->fons_index = fons_index;
    locus->linea = linea;
    locus->titulus = titulus;
    locus->def = def;
    locus->conditio_id = conditio_id;
    locus->positus = positus;
}

SilvaTabulaDispersa*
silva_expansio_macros_ad_lineam (
    SilvaExpansio* exp,
    SilvaPiscina*       piscina,
    s32            fons_index,
    i32            linea)
{
    SilvaTabulaDispersa* tabula;
    SilvaEventum* eventum;
    i32 i;
    i32 n;

    tabula = silva_tabula_dispersa_creare_chorda(piscina, LXIV);
    n = silva_xar_numerus(exp->acta);
    per (i = ZEPHYRUM; i < n; i++)
    {
        eventum = (SilvaEventum*)silva_xar_obtinere(exp->acta, i);
        si (eventum->fons_index == fons_index && eventum->linea >= linea)
        {
            frange;
        }
        si (eventum->genus == SILVA_EVENTUM_DEFINITIO)
        {
            silva_tabula_dispersa_inserere(tabula, *eventum->titulus,
                (vacuum*)eventum->def);
        }
        alioquin
        {
            silva_tabula_dispersa_delere(tabula, *eventum->titulus);
        }
    }
    redde tabula;
}


/* ==================================================
 * Quaestio tabulae vivae
 * ================================================== */

SilvaMacroDef*
silva_expansio_quaerere (
    SilvaExpansio* exp,
    SilvaChorda         titulus)
{
    vacuum* valor;
    SilvaTabulaDispersa* tabula;

    /* expansio positionalis tabulam temporalem substituit */
    tabula = (exp->tabula_activa != NIHIL) ? exp->tabula_activa : exp->macros;
    si (silva_tabula_dispersa_invenire(tabula, titulus, &valor))
    {
        redde (SilvaMacroDef*)valor;
    }
    redde NIHIL;
}


/* ==================================================
 * Processio directivarum (Chunk A + D)
 * (Limen includendi: exp->limen_includendi, Phase 7)
 * ================================================== */

/* Genus directivae (internum) */
nomen enumeratio {
    SILVA_DIR_NULLA = 0,   /* # solum (directiva vacua) */
    SILVA_DIR_DEFINE,
    SILVA_DIR_UNDEF,
    SILVA_DIR_INCLUDE,
    SILVA_DIR_IF,
    SILVA_DIR_IFDEF,
    SILVA_DIR_IFNDEF,
    SILVA_DIR_ELIF,
    SILVA_DIR_ELSE,
    SILVA_DIR_ENDIF,
    SILVA_DIR_IGNOTA       /* alia (line/pragma/error/...) */
} SilvaDirectivaGenus;

/* Estne lexema verum initium directivae? (# ad initium lineae LOGICAE) */
interior b32
_est_initium_directivae (SilvaToken* token)
{
    redde (token->genus == SILVA_LEX_CANCELLUM && token->initium_lineae)
        ? VERUM : FALSUM;
}

/* Finis lineae logicae incipientis ad i (exclusive) */
interior i32
_lineam_finire (SilvaXar* lexemata, i32 i, i32 n)
{
    i32 i_finis;
    SilvaToken* t;

    i_finis = i + I;
    dum (i_finis < n)
    {
        t = *(SilvaToken**)silva_xar_obtinere(lexemata, i_finis);
        si (t->initium_lineae || t->genus == SILVA_LEX_EOF)
        {
            frange;
        }
        i_finis++;
    }
    redde i_finis;
}

interior vacuum
_lexema_addere (SilvaXar* xar, SilvaToken* token)
{
    SilvaToken** locus;

    locus = (SilvaToken**)silva_xar_addere(xar);
    si (locus != NIHIL)
    {
        *locus = token;
    }
}

/* Lamina: exemplar lexematum [a, b) */
interior SilvaXar*
_lamina_capere (SilvaExpansio* exp, SilvaXar* lexemata, i32 a, i32 b)
{
    SilvaXar* lamina;
    i32 j;

    lamina = silva_xar_creare(exp->piscina, magnitudo(SilvaToken*));
    per (j = a; j < b; j++)
    {
        _lexema_addere(lamina, *(SilvaToken**)silva_xar_obtinere(lexemata, j));
    }
    redde lamina;
}

/* Lineam directivae in directivae_out capere (si petitum) */
interior vacuum
_directivam_capere (
    SilvaExpansio* exp,
    SilvaXar*           directivae,
    SilvaXar*           lexemata,
    i32            a,
    i32            b)
{
    SilvaXar** locus;

    si (directivae == NIHIL)
    {
        redde;
    }
    locus = (SilvaXar**)silva_xar_addere(directivae);
    si (locus != NIHIL)
    {
        *locus = _lamina_capere(exp, lexemata, a, b);
    }
}

interior b32
_chordae_aequales (SilvaChorda a, SilvaChorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    si (a.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (memcmp(a.datum, b.datum, (memoriae_index)a.mensura) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

/* Potestne lexema nomen esse in directiva? (verba clausa quoque) */
interior b32
_est_nomen_directivae (SilvaToken* token)
{
    si (token->genus == SILVA_LEX_IDENTIFICATOR)
    {
        redde VERUM;
    }
    si (token->genus >= SILVA_LEX_AUTO && token->genus <= SILVA_LEX_WHILE)
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* Classificare directivam ad i_cancellum. NB: #if et #else verba
 * clausa lexantur (genus IF/ELSE), cetera identificatores. */
interior SilvaDirectivaGenus
_directivae_genus (SilvaXar* lexemata, i32 i_cancellum, i32 i_finis)
{
    SilvaToken* verbum;

    si (i_cancellum + I >= i_finis)
    {
        redde SILVA_DIR_NULLA;
    }
    verbum = *(SilvaToken**)silva_xar_obtinere(lexemata, i_cancellum + I);
    si (verbum->genus == SILVA_LEX_IF)
    {
        redde SILVA_DIR_IF;
    }
    si (verbum->genus == SILVA_LEX_ELSE)
    {
        redde SILVA_DIR_ELSE;
    }
    si (verbum->genus != SILVA_LEX_IDENTIFICATOR)
    {
        redde SILVA_DIR_IGNOTA;
    }
    si (_chorda_est_literis(verbum->valor, "define"))
    {
        redde SILVA_DIR_DEFINE;
    }
    si (_chorda_est_literis(verbum->valor, "undef"))
    {
        redde SILVA_DIR_UNDEF;
    }
    si (_chorda_est_literis(verbum->valor, "include"))
    {
        redde SILVA_DIR_INCLUDE;
    }
    si (_chorda_est_literis(verbum->valor, "ifdef"))
    {
        redde SILVA_DIR_IFDEF;
    }
    si (_chorda_est_literis(verbum->valor, "ifndef"))
    {
        redde SILVA_DIR_IFNDEF;
    }
    si (_chorda_est_literis(verbum->valor, "elif"))
    {
        redde SILVA_DIR_ELIF;
    }
    si (_chorda_est_literis(verbum->valor, "endif"))
    {
        redde SILVA_DIR_ENDIF;
    }
    redde SILVA_DIR_IGNOTA;
}

/* Processare define/undef: lexemata[i_cancellum] est CANCELLUM.
 * Reddit VERUM si processata. conditio_id = ramus continens (0 = nullus).
 * positus = longitudo reliquorum in consumptione (status ad punctum). */
interior b32
_definitionem_processare (
    SilvaExpansio*      exp,
    SilvaXar*                lexemata,
    i32                 i_cancellum,
    i32                 i_finis,
    SilvaDirectivaGenus genus_dir,
    i32                 conditio_id,
    s32                 positus)
{
    SilvaToken* titulus_tok;
    SilvaMacroDef* def;
    SilvaChorda* titulus;
    i32 i;

    /* # verbum titulus ... - minimum # + verbum + titulus */
    si (i_cancellum + II >= i_finis)
    {
        redde FALSUM;
    }
    titulus_tok = *(SilvaToken**)silva_xar_obtinere(lexemata, i_cancellum + II);
    si (!_est_nomen_directivae(titulus_tok))
    {
        redde FALSUM;
    }

    si (genus_dir == SILVA_DIR_UNDEF)
    {
        titulus = _chorda_figere(exp->piscina, titulus_tok->valor);
        silva_tabula_dispersa_delere(exp->macros, titulus_tok->valor);
        _eventum_scribere(exp, SILVA_EVENTUM_DELETIO,
            titulus_tok->fons_index, titulus_tok->linea, titulus, NIHIL,
            conditio_id, positus);
        redde VERUM;
    }

    def = (SilvaMacroDef*)silva_piscina_allocare(exp->piscina,
        (memoriae_index)magnitudo(SilvaMacroDef));
    si (def == NIHIL)
    {
        redde FALSUM;
    }
    memset(def, ZEPHYRUM, magnitudo(SilvaMacroDef));

    titulus = _chorda_figere(exp->piscina, titulus_tok->valor);
    def->titulus = titulus;
    def->fons_index = titulus_tok->fons_index;
    def->linea_def = titulus_tok->linea;
    def->corpus = silva_xar_creare(exp->piscina, magnitudo(SilvaToken*));

    i = i_cancellum + III;

    /* Functio-similis: '(' IMMEDIATE adiacens (adiacentia cruda) */
    si (i < i_finis)
    {
        SilvaToken* paren;

        paren = *(SilvaToken**)silva_xar_obtinere(lexemata, i);
        si (paren->genus == SILVA_LEX_PAREN_APERTA
            && paren->byte_offset == titulus_tok->byte_offset
                + (s32)titulus_tok->longitudo)
        {
            def->est_functio = VERUM;
            def->parametra = silva_xar_creare(exp->piscina, magnitudo(SilvaChorda*));
            i++;  /* trans ( */
            dum (i < i_finis)
            {
                SilvaToken* t;

                t = *(SilvaToken**)silva_xar_obtinere(lexemata, i);
                si (t->genus == SILVA_LEX_PAREN_CLAUSA)
                {
                    i++;
                    frange;
                }
                si (t->genus == SILVA_LEX_IDENTIFICATOR)
                {
                    SilvaChorda** locus;

                    locus = (SilvaChorda**)silva_xar_addere(def->parametra);
                    si (locus != NIHIL)
                    {
                        *locus = _chorda_figere(exp->piscina, t->valor);
                    }
                }
                alioquin si (t->genus == SILVA_LEX_ELLIPSIS)
                {
                    /* variadica: parametrum "__VA_ARGS__" appenditur ut
                     * ultimum - cauda argumentorum ei ligatur */
                    SilvaChorda** locus;

                    def->est_variadica = VERUM;
                    locus = (SilvaChorda**)silva_xar_addere(def->parametra);
                    si (locus != NIHIL)
                    {
                        *locus = _chorda_figere(exp->piscina,
                            silva_chorda_ex_literis("__VA_ARGS__", exp->piscina));
                    }
                }
                /* COMMA transitur */
                i++;
            }
        }
    }

    /* Corpus: reliqua lineae logicae */
    per (; i < i_finis; i++)
    {
        _lexema_addere(def->corpus, *(SilvaToken**)silva_xar_obtinere(lexemata, i));
    }

    silva_tabula_dispersa_inserere(exp->macros, *titulus, (vacuum*)def);
    _eventum_scribere(exp, SILVA_EVENTUM_DEFINITIO,
        def->fons_index, def->linea_def, titulus, def, conditio_id, positus);
    redde VERUM;
}

/* Praedeclarationes (recursio mutua: fluxus <-> regiones <-> includenda) */
interior vacuum
_fluxum_processare (SilvaExpansio* exp, SilvaXar* lexemata, i32 i_initium,
    i32 i_finis, i32 conditio_id, SilvaRegio* pater, b32 servare_eof,
    SilvaXar* reliqua, SilvaXar* directivae);
interior vacuum
_plagulam_processare (SilvaExpansio* exp, SilvaXar* lexemata, b32 servare_eof,
    SilvaXar* reliqua, SilvaXar* directivae);

/* ==================================================
 * Chunk D - Regiones conditionales
 * ================================================== */

interior SilvaRegio*
_regionem_creare (SilvaExpansio* exp, SilvaToken* cancellum, SilvaRegio* pater)
{
    SilvaRegio* regio;

    regio = (SilvaRegio*)silva_piscina_allocare(exp->piscina,
        (memoriae_index)magnitudo(SilvaRegio));
    si (regio == NIHIL)
    {
        redde NIHIL;
    }
    regio->fons_index = cancellum->fons_index;
    regio->linea = cancellum->linea;
    regio->rami = silva_xar_creare(exp->piscina, magnitudo(SilvaRamus*));
    regio->pater = pater;
    regio->filiae = silva_xar_creare(exp->piscina, magnitudo(SilvaRegio*));
    regio->est_imperfecta = FALSUM;
    regio->est_ultra_modum = FALSUM;
    regio->directiva_finis = NIHIL;
    regio->est_texta = FALSUM;

    si (pater != NIHIL)
    {
        SilvaRegio** locus;

        locus = (SilvaRegio**)silva_xar_addere(pater->filiae);
        si (locus != NIHIL)
        {
            *locus = regio;
        }
    }
    alioquin
    {
        SilvaRegio** locus;

        locus = (SilvaRegio**)silva_xar_addere(exp->regiones);
        si (locus != NIHIL)
        {
            *locus = regio;
        }
    }
    redde regio;
}

/* Processare regionem: lexemata[i] est CANCELLUM #if/#ifdef/#ifndef.
 * Consumit usque ad #endif parem (vel finem fluxus). Reddit indicem
 * post regionem. Via defalta: primus ramus verus sumitur - lexemata
 * eius normaliter processantur; ceteri laminas crudas retinent.
 * Profunditas ultra limen (Phase 7): regio est_ultra_modum - NULLUS
 * ramus evaluatur, omnes crudi, recursio cessat (fluxus infestus
 * acervum numquam perfodit); octeti in crudis supersunt, ergo
 * reconstructio tenet. */
interior i32
_regionem_processare_interna (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    i32            i,
    i32            i_finis,
    SilvaRegio*    pater,
    SilvaXar*           reliqua,
    SilvaXar*           directivae)
{
    SilvaRegio* regio;
    b32 sumptum_iam;
    i32 i_currens;

    regio = _regionem_creare(exp,
        *(SilvaToken**)silva_xar_obtinere(lexemata, i), pater);
    si (regio == NIHIL)
    {
        redde i_finis;
    }
    si (exp->limen_regionum > ZEPHYRUM
        && exp->profunditas_regionum > exp->limen_regionum)
    {
        regio->est_ultra_modum = VERUM;
        exp->fines_tactae = VERUM;
    }
    sumptum_iam = FALSUM;
    i_currens = i;

    dum (i_currens < i_finis)
    {
        SilvaRamus* ramus;
        SilvaRamus** locus_rami;
        SilvaDirectivaGenus genus_dir;
        i32 i_linea_finis;
        i32 i_corpus;
        i32 i_scan;
        i32 profunditas;

        i_linea_finis = _lineam_finire(lexemata, i_currens, i_finis);
        genus_dir = _directivae_genus(lexemata, i_currens, i_linea_finis);
        /* β (sim ⑦ C2): linea structuralis NON capitur - regio eam
         * possidet (ramus->directiva infra); scriptura lineas regionum
         * non textarum ex arbore regionum colligit */

        /* Ramum creare */
        ramus = (SilvaRamus*)silva_piscina_allocare(exp->piscina,
            (memoriae_index)magnitudo(SilvaRamus));
        si (ramus == NIHIL)
        {
            redde i_finis;
        }
        memset(ramus, ZEPHYRUM, magnitudo(SilvaRamus));
        commutatio (genus_dir)
        {
            casus SILVA_DIR_IF:     ramus->genus = SILVA_RAMUS_IF;     frange;
            casus SILVA_DIR_IFDEF:  ramus->genus = SILVA_RAMUS_IFDEF;  frange;
            casus SILVA_DIR_IFNDEF: ramus->genus = SILVA_RAMUS_IFNDEF; frange;
            casus SILVA_DIR_ELIF:   ramus->genus = SILVA_RAMUS_ELIF;   frange;
            ordinarius:             ramus->genus = SILVA_RAMUS_ELSE;   frange;
        }
        ramus->conditio_id = (i32)(silva_xar_numerus(exp->rami) + I);
        locus_rami = (SilvaRamus**)silva_xar_addere(exp->rami);
        si (locus_rami != NIHIL)
        {
            *locus_rami = ramus;
        }
        ramus->directiva = _lamina_capere(exp, lexemata, i_currens,
            i_linea_finis);
        ramus->regio = regio;
        si (ramus->genus != SILVA_RAMUS_ELSE)
        {
            ramus->expressio = _lamina_capere(exp, lexemata,
                i_currens + II, i_linea_finis);
        }

        /* #if 0 idioma: litteralis falsa */
        si ((ramus->genus == SILVA_RAMUS_IF
                || ramus->genus == SILVA_RAMUS_ELIF)
            && ramus->expressio != NIHIL
            && silva_xar_numerus(ramus->expressio) == I)
        {
            SilvaToken* solum;

            solum = *(SilvaToken**)silva_xar_obtinere(ramus->expressio, ZEPHYRUM);
            si (solum->genus == SILVA_LEX_INTEGER
                && _chorda_est_literis(solum->valor, "0"))
            {
                ramus->est_numquam = VERUM;
            }
        }

        /* Evaluatio (via defalta) - rami post sumptum NON evaluantur;
         * regio ultra modum: nullus umquam (omnes crudi) */
        si (!sumptum_iam && !regio->est_ultra_modum)
        {
            si (ramus->genus == SILVA_RAMUS_ELSE)
            {
                ramus->valor = I;
                ramus->est_evaluatum = VERUM;
            }
            alioquin si (ramus->genus == SILVA_RAMUS_IFDEF
                || ramus->genus == SILVA_RAMUS_IFNDEF)
            {
                si (ramus->expressio != NIHIL
                    && silva_xar_numerus(ramus->expressio) > ZEPHYRUM)
                {
                    SilvaToken* operandum;

                    operandum = *(SilvaToken**)silva_xar_obtinere(
                        ramus->expressio, ZEPHYRUM);
                    si (_est_nomen_directivae(operandum))
                    {
                        b32 definitum;

                        definitum = silva_conditio_est_definitum(exp,
                            operandum->valor);
                        si (ramus->genus == SILVA_RAMUS_IFNDEF)
                        {
                            definitum = definitum ? FALSUM : VERUM;
                        }
                        ramus->valor = definitum ? I : ZEPHYRUM;
                        ramus->est_evaluatum = VERUM;
                    }
                }
                /* operandum absens/malformatum: falsum, non evaluatum */
            }
            alioquin
            {
                b32 successus;

                successus = FALSUM;
                ramus->valor = silva_conditio_evaluare(exp,
                    ramus->expressio, &successus);
                ramus->est_evaluatum = successus;
                si (!successus)
                {
                    ramus->valor = ZEPHYRUM;  /* robustitas: falsum */
                }
            }
            ramus->est_sumptum = (ramus->valor != ZEPHYRUM) ? VERUM : FALSUM;
        }

        locus_rami = (SilvaRamus**)silva_xar_addere(regio->rami);
        si (locus_rami != NIHIL)
        {
            *locus_rami = ramus;
        }

        /* Corpus rami: usque ad ELIF/ELSE/ENDIF parem (profunditas 0) */
        i_corpus = i_linea_finis;
        i_scan = i_corpus;
        profunditas = ZEPHYRUM;
        dum (i_scan < i_finis)
        {
            SilvaToken* t;

            t = *(SilvaToken**)silva_xar_obtinere(lexemata, i_scan);
            si (_est_initium_directivae(t))
            {
                SilvaDirectivaGenus g;
                i32 lf;

                lf = _lineam_finire(lexemata, i_scan, i_finis);
                g = _directivae_genus(lexemata, i_scan, lf);
                si (g == SILVA_DIR_IF || g == SILVA_DIR_IFDEF
                    || g == SILVA_DIR_IFNDEF)
                {
                    profunditas++;
                }
                alioquin si (g == SILVA_DIR_ENDIF)
                {
                    si (profunditas == ZEPHYRUM)
                    {
                        frange;
                    }
                    profunditas--;
                }
                alioquin si ((g == SILVA_DIR_ELIF || g == SILVA_DIR_ELSE)
                    && profunditas == ZEPHYRUM)
                {
                    frange;
                }
                i_scan = lf;
            }
            alioquin
            {
                i_scan++;
            }
        }

        /* Fines corporis in offsetibus (sim ⑦ C1 - textura eos contra
         * extenta sententiarum comparat). Finis EXCLUSIVUS = offset
         * directivae proximae; imperfecta = apertum */
        si (i_corpus < i_scan)
        {
            SilvaToken* primum_corporis =
                *(SilvaToken**)silva_xar_obtinere(lexemata, i_corpus);

            ramus->corpus_initium = primum_corporis->byte_offset;
        }
        alioquin
        {
            ramus->corpus_initium = -I;
        }
        si (i_scan < i_finis)
        {
            SilvaToken* post_corpus =
                *(SilvaToken**)silva_xar_obtinere(lexemata, i_scan);

            ramus->corpus_finis = post_corpus->byte_offset;
        }
        alioquin
        {
            ramus->corpus_finis = 0x7FFFFFFF;  /* imperfecta: apertum */
        }

        si (ramus->est_sumptum)
        {
            sumptum_iam = VERUM;
            _fluxum_processare(exp, lexemata, i_corpus, i_scan,
                ramus->conditio_id, regio, VERUM, reliqua, directivae);
        }
        alioquin
        {
            ramus->lexemata_cruda = _lamina_capere(exp, lexemata,
                i_corpus, i_scan);
        }

        si (i_scan >= i_finis)
        {
            regio->est_imperfecta = VERUM;  /* EOF ante #endif */
            redde i_finis;
        }

        /* i_scan stat ad ELIF/ELSE/ENDIF */
        {
            SilvaDirectivaGenus g;
            i32 lf;

            lf = _lineam_finire(lexemata, i_scan, i_finis);
            g = _directivae_genus(lexemata, i_scan, lf);
            si (g == SILVA_DIR_ENDIF)
            {
                /* β: linea #endif regioni ipsa (sim ⑦ C1/C2) */
                regio->directiva_finis = _lamina_capere(exp, lexemata,
                    i_scan, lf);
                redde lf;
            }
            i_currens = i_scan;  /* ramus proximus (elif/else) */
        }
    }

    regio->est_imperfecta = VERUM;
    redde i_finis;
}

/* Involucrum numeratoris profunditatis (omnes viae reditus tectae) */
interior i32
_regionem_processare (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    i32            i,
    i32            i_finis,
    SilvaRegio*    pater,
    SilvaXar*           reliqua,
    SilvaXar*           directivae)
{
    i32 fructus;

    exp->profunditas_regionum++;
    fructus = _regionem_processare_interna(exp, lexemata, i, i_finis,
        pater, reliqua, directivae);
    exp->profunditas_regionum--;
    redde fructus;
}

/* ==================================================
 * Chunk D - Includenda
 * ================================================== */

/* Processare #include: viam extrahere, inclusionem memorare,
 * contentum praebitum recursive processare (reliqua hic inserta) */
interior vacuum
_includendum_processare (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    i32            i_cancellum,
    i32            i_finis,
    SilvaXar*           reliqua,
    SilvaXar*           directivae)
{
    SilvaToken* cancellum;
    SilvaInclusio* inclusio;
    SilvaIncludendum* incl;
    SilvaFons* fons;
    SilvaChorda via;
    b32 habet_viam;
    vacuum* valor;
    i32 i_op;

    cancellum = *(SilvaToken**)silva_xar_obtinere(lexemata, i_cancellum);
    habet_viam = FALSUM;
    via.datum = NIHIL;
    via.mensura = ZEPHYRUM;
    i_op = i_cancellum + II;

    si (i_op < i_finis)
    {
        SilvaToken* t;

        t = *(SilvaToken**)silva_xar_obtinere(lexemata, i_op);
        si (t->genus == SILVA_LEX_STRING_LIT && t->valor.mensura >= II)
        {
            /* "via" - termini remoti */
            via.datum = t->valor.datum + I;
            via.mensura = t->valor.mensura - II;
            habet_viam = VERUM;
        }
        alioquin si (t->genus == SILVA_LEX_MINOR)
        {
            /* <via> - valores concatenati usque ad '>' */
            SilvaChordaAedificator* aed;
            b32 clausa;
            i32 j;

            aed = silva_chorda_aedificator_creare(exp->piscina, XXXII);
            clausa = FALSUM;
            per (j = i_op + I; j < i_finis; j++)
            {
                SilvaToken* u;

                u = *(SilvaToken**)silva_xar_obtinere(lexemata, j);
                si (u->genus == SILVA_LEX_MAIOR)
                {
                    clausa = VERUM;
                    frange;
                }
                silva_chorda_aedificator_appendere_chorda(aed, u->valor);
            }
            si (clausa)
            {
                via = silva_chorda_aedificator_finire(aed);
                habet_viam = VERUM;
            }
        }
        /* forma per macro expansa: differtur (vide phase-log Chunk D) */
    }

    si (!habet_viam)
    {
        redde;  /* malformata: linea capta, nihil insertum */
    }

    /* Inclusionem memorare (graphum dependentiarum - "discens").
     * NB: monstratores elementorum Xar stabiles sunt trans additiones
     * (segmenta fixa, sine reallocatio - vide xar.c); ordo
     * scribendi-ante-recursionem est claritas, non necessitas. */
    inclusio = (SilvaInclusio*)silva_xar_addere(exp->inclusiones);
    si (inclusio == NIHIL)
    {
        redde;
    }
    inclusio->fons_ex = cancellum->fons_index;
    inclusio->via = _chorda_figere(exp->piscina, via);
    inclusio->fons_ad = -I;
    inclusio->est_praetermissa = FALSUM;

    si (!silva_tabula_dispersa_invenire(exp->includenda, via, &valor))
    {
        redde;  /* ignotum: via memorata, processio pergit */
    }
    incl = (SilvaIncludendum*)valor;
    inclusio->fons_ad = incl->fons_index;

    /* Custos definitus -> plagula praetermittitur (interior iam nota) */
    fons = (SilvaFons*)silva_xar_obtinere(exp->fontes, (i32)incl->fons_index);
    si (fons->est_custos && fons->custos_titulus != NIHIL
        && silva_conditio_est_definitum(exp, *fons->custos_titulus))
    {
        inclusio->est_praetermissa = VERUM;
        redde;
    }

    si (exp->limen_includendi > ZEPHYRUM
        && exp->profunditas_includendi >= exp->limen_includendi)
    {
        inclusio->est_praetermissa = VERUM;  /* profunditas nimia */
        exp->fines_tactae = VERUM;
        redde;
    }

    exp->profunditas_includendi++;
    _plagulam_processare(exp, incl->lexemata, FALSUM, reliqua, directivae);
    exp->profunditas_includendi--;
}

s32
silva_includendum_praebere (
    SilvaExpansio*      exp,
    constans character* via,
    constans character* textus,
    i32                 mensura)
{
    SilvaIncludendum* incl;
    SilvaFons* fons;
    s32 fons_index;

    fons_index = silva_fons_addere(exp, via, FALSUM);
    si (fons_index < ZEPHYRUM)
    {
        redde -I;
    }
    incl = (SilvaIncludendum*)silva_piscina_allocare(exp->piscina,
        (memoriae_index)magnitudo(SilvaIncludendum));
    si (incl == NIHIL)
    {
        redde -I;
    }
    incl->fons_index = fons_index;
    incl->lexemata = silva_lexare(exp->piscina, textus, mensura, fons_index);

    fons = (SilvaFons*)silva_xar_obtinere(exp->fontes, (i32)fons_index);
    silva_tabula_dispersa_inserere(exp->includenda, *fons->via, (vacuum*)incl);
    redde fons_index;
}

/* ==================================================
 * Chunk D - Custodes (est_custos)
 *
 * Forma stricta: primum lexema plagulae est '#' de '#ifndef X';
 * linea logica proxima est '#define X'; #endif par nihil nisi
 * EOF sequitur. Detecta: NULLA regio - interior incondicionaliter
 * processatur (a linea #define), custos in SilvaFons memoratur.
 * ================================================== */

interior b32
_custodem_detegere (
    SilvaXar*         lexemata,
    i32*         i_corpus_out,   /* initium lineae #define */
    i32*         i_endif_out,    /* initium lineae #endif */
    i32*         i_post_out,     /* post lineam #endif */
    SilvaToken** operandum_out)
{
    SilvaToken* tok;
    SilvaToken* operandum;
    SilvaToken* def_operandum;
    i32 n;
    i32 lf0;
    i32 lf1;
    i32 i;
    i32 profunditas;

    n = silva_xar_numerus(lexemata);
    si (n == ZEPHYRUM)
    {
        redde FALSUM;
    }
    tok = *(SilvaToken**)silva_xar_obtinere(lexemata, ZEPHYRUM);
    si (!_est_initium_directivae(tok))
    {
        redde FALSUM;
    }
    lf0 = _lineam_finire(lexemata, ZEPHYRUM, n);
    si (_directivae_genus(lexemata, ZEPHYRUM, lf0) != SILVA_DIR_IFNDEF)
    {
        redde FALSUM;
    }
    si (II >= lf0)
    {
        redde FALSUM;
    }
    operandum = *(SilvaToken**)silva_xar_obtinere(lexemata, II);
    si (!_est_nomen_directivae(operandum))
    {
        redde FALSUM;
    }

    /* linea proxima: #define X (idem X) */
    si (lf0 >= n)
    {
        redde FALSUM;
    }
    tok = *(SilvaToken**)silva_xar_obtinere(lexemata, lf0);
    si (!_est_initium_directivae(tok))
    {
        redde FALSUM;
    }
    lf1 = _lineam_finire(lexemata, lf0, n);
    si (_directivae_genus(lexemata, lf0, lf1) != SILVA_DIR_DEFINE)
    {
        redde FALSUM;
    }
    si (lf0 + II >= lf1)
    {
        redde FALSUM;
    }
    def_operandum = *(SilvaToken**)silva_xar_obtinere(lexemata, lf0 + II);
    si (!_chordae_aequales(def_operandum->valor, operandum->valor))
    {
        redde FALSUM;
    }

    /* #endif par quaerere; post eum nihil nisi EOF */
    i = lf1;
    profunditas = ZEPHYRUM;
    dum (i < n)
    {
        tok = *(SilvaToken**)silva_xar_obtinere(lexemata, i);
        si (_est_initium_directivae(tok))
        {
            SilvaDirectivaGenus g;
            i32 lf;

            lf = _lineam_finire(lexemata, i, n);
            g = _directivae_genus(lexemata, i, lf);
            si (g == SILVA_DIR_IF || g == SILVA_DIR_IFDEF
                || g == SILVA_DIR_IFNDEF)
            {
                profunditas++;
            }
            alioquin si (g == SILVA_DIR_ENDIF)
            {
                si (profunditas == ZEPHYRUM)
                {
                    i32 j;

                    per (j = lf; j < n; j++)
                    {
                        SilvaToken* u;

                        u = *(SilvaToken**)silva_xar_obtinere(lexemata, j);
                        si (u->genus != SILVA_LEX_EOF)
                        {
                            redde FALSUM;  /* contentum post #endif */
                        }
                    }
                    *i_corpus_out = lf0;
                    *i_endif_out = i;
                    *i_post_out = lf;
                    *operandum_out = operandum;
                    redde VERUM;
                }
                profunditas--;
            }
            i = lf;
        }
        alioquin
        {
            i++;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Ambulator fluxus + introitus publicus
 * ================================================== */

interior vacuum
_fluxum_processare (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    i32            i_initium,
    i32            i_finis,
    i32            conditio_id,
    SilvaRegio*    pater,
    b32            servare_eof,
    SilvaXar*           reliqua,
    SilvaXar*           directivae)
{
    SilvaToken* token;
    i32 i;

    i = i_initium;
    dum (i < i_finis)
    {
        token = *(SilvaToken**)silva_xar_obtinere(lexemata, i);

        si (_est_initium_directivae(token))
        {
            SilvaDirectivaGenus genus_dir;
            i32 i_linea_finis;

            i_linea_finis = _lineam_finire(lexemata, i, i_finis);
            genus_dir = _directivae_genus(lexemata, i, i_linea_finis);

            si (genus_dir == SILVA_DIR_DEFINE || genus_dir == SILVA_DIR_UNDEF)
            {
                si (_definitionem_processare(exp, lexemata, i, i_linea_finis,
                        genus_dir, conditio_id,
                        (s32)silva_xar_numerus(reliqua)))
                {
                    _directivam_capere(exp, directivae, lexemata, i,
                        i_linea_finis);
                    i = i_linea_finis;
                    perge;
                }
                /* malformata: cadit ad reliqua */
            }
            alioquin si (genus_dir == SILVA_DIR_INCLUDE)
            {
                _directivam_capere(exp, directivae, lexemata, i,
                    i_linea_finis);
                _includendum_processare(exp, lexemata, i, i_linea_finis,
                    reliqua, directivae);
                i = i_linea_finis;
                perge;
            }
            alioquin si (genus_dir == SILVA_DIR_IF
                || genus_dir == SILVA_DIR_IFDEF
                || genus_dir == SILVA_DIR_IFNDEF)
            {
                i = _regionem_processare(exp, lexemata, i, i_finis,
                    pater, reliqua, directivae);
                perge;
            }
            /* ELIF/ELSE/ENDIF sine regione, NULLA, IGNOTA:
             * transeunt ad reliqua (byte-conservativum) */
        }

        si (token->genus != SILVA_LEX_EOF || servare_eof)
        {
            _lexema_addere(reliqua, token);
        }
        i++;
    }
}

interior vacuum
_plagulam_processare (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    b32            servare_eof,
    SilvaXar*           reliqua,
    SilvaXar*           directivae)
{
    SilvaToken* operandum;
    i32 i_corpus;
    i32 i_endif;
    i32 i_post;
    i32 n;

    n = silva_xar_numerus(lexemata);
    operandum = NIHIL;

    si (_custodem_detegere(lexemata, &i_corpus, &i_endif, &i_post, &operandum))
    {
        s32 fi;

        fi = operandum->fons_index;
        si (fi >= ZEPHYRUM)
        {
            SilvaFons* fons;

            fons = (SilvaFons*)silva_xar_obtinere(exp->fontes, (i32)fi);
            fons->est_custos = VERUM;
            fons->custos_titulus = _chorda_figere(exp->piscina,
                operandum->valor);
        }

        /* custos iam definitus (rarum: iniectio ante processionem).
         * NON transparentia: per ambulatorem NORMALEM - #ifndef falsum
         * evaluat, interior ramus NON sumptus fit (lexemata_cruda -
         * "extra rationem sed numquam perditum", sim ⑥ C7), lineae
         * directivae normaliter captae. Reconstructio fontis viam
         * ordinariam reinserendorum equitat. */
        si (silva_conditio_est_definitum(exp, operandum->valor))
        {
            _fluxum_processare(exp, lexemata, ZEPHYRUM, n, ZEPHYRUM, NIHIL,
                servare_eof, reliqua, directivae);
            redde;
        }

        /* transparentia custodis: nulla regio; tres directivae captae;
         * interior (a linea #define) incondicionaliter processatur */
        _directivam_capere(exp, directivae, lexemata, ZEPHYRUM, i_corpus);
        _fluxum_processare(exp, lexemata, i_corpus, i_endif, ZEPHYRUM, NIHIL,
            servare_eof, reliqua, directivae);
        _directivam_capere(exp, directivae, lexemata, i_endif, i_post);
        _fluxum_processare(exp, lexemata, i_post, n, ZEPHYRUM, NIHIL,
            servare_eof, reliqua, directivae);
        redde;
    }

    _fluxum_processare(exp, lexemata, ZEPHYRUM, n, ZEPHYRUM, NIHIL,
        servare_eof, reliqua, directivae);
}

SilvaXar*
silva_expansio_directivas_processare (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    SilvaXar**          directivae_out)
{
    SilvaXar* reliqua;
    SilvaXar* directivae;

    reliqua = silva_xar_creare(exp->piscina, magnitudo(SilvaToken*));
    directivae = NIHIL;
    si (directivae_out != NIHIL)
    {
        directivae = silva_xar_creare(exp->piscina, magnitudo(SilvaXar*));
        *directivae_out = directivae;
    }
    _plagulam_processare(exp, lexemata, VERUM, reliqua, directivae);
    redde reliqua;
}


/* ==================================================
 * Iniectio macro per API
 * ================================================== */

interior s32
_fons_api_obtinere (SilvaExpansio* exp)
{
    si (exp->fons_api < ZEPHYRUM)
    {
        exp->fons_api = silva_fons_addere(exp, "<api>", VERUM);
    }
    redde exp->fons_api;
}

/* Lexare corpus in piscinam (textus duplicatur - lexemata visus in
 * eum tenent); EOF remotum. Lexemata sunt FONTIS synthetici:
 * provenientia per catenas normales fluit. */
interior SilvaXar*
_corpus_api_lexare (SilvaExpansio* exp, constans character* textus)
{
    SilvaXar* lexemata;
    SilvaXar* corpus;
    i8* fixum;
    i32 mensura;
    i32 i;
    i32 n;

    mensura = (i32)strlen(textus);
    fixum = (i8*)silva_piscina_allocare(exp->piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (fixum == NIHIL)
    {
        redde NIHIL;
    }
    si (mensura > ZEPHYRUM)
    {
        memcpy(fixum, textus, (memoriae_index)mensura);
    }

    lexemata = silva_lexare(exp->piscina, (constans character*)fixum,
        mensura, _fons_api_obtinere(exp));
    corpus = silva_xar_creare(exp->piscina, magnitudo(SilvaToken*));
    n = silva_xar_numerus(lexemata);
    per (i = ZEPHYRUM; i < n; i++)
    {
        SilvaToken* t;

        t = *(SilvaToken**)silva_xar_obtinere(lexemata, i);
        si (t->genus != SILVA_LEX_EOF)
        {
            _lexema_addere(corpus, t);
        }
    }
    redde corpus;
}

interior SilvaMacroDef*
_def_api_creare (
    SilvaExpansio*      exp,
    constans character* titulus,
    constans character* corpus)
{
    SilvaMacroDef* def;

    def = (SilvaMacroDef*)silva_piscina_allocare(exp->piscina,
        (memoriae_index)magnitudo(SilvaMacroDef));
    si (def == NIHIL)
    {
        redde NIHIL;
    }
    memset(def, ZEPHYRUM, magnitudo(SilvaMacroDef));
    def->titulus = _chorda_figere(exp->piscina,
        silva_chorda_ex_literis(titulus, exp->piscina));
    def->corpus = _corpus_api_lexare(exp, corpus);
    def->fons_index = _fons_api_obtinere(exp);
    def->linea_def = ZEPHYRUM;
    def->ex_api = VERUM;
    si (def->titulus == NIHIL || def->corpus == NIHIL)
    {
        redde NIHIL;
    }
    redde def;
}

interior vacuum
_def_api_registrare (SilvaExpansio* exp, SilvaMacroDef* def)
{
    silva_tabula_dispersa_inserere(exp->macros, *def->titulus, (vacuum*)def);
    _eventum_scribere(exp, SILVA_EVENTUM_DEFINITIO, def->fons_index,
        ZEPHYRUM, def->titulus, def, ZEPHYRUM, ZEPHYRUM);
}

b32
silva_macro_addere (
    SilvaExpansio*      exp,
    constans character* titulus,
    constans character* corpus)
{
    SilvaMacroDef* def;

    def = _def_api_creare(exp, titulus, corpus);
    si (def == NIHIL)
    {
        redde FALSUM;
    }
    _def_api_registrare(exp, def);
    redde VERUM;
}

b32
silva_macro_functio_addere (
    SilvaExpansio*       exp,
    constans character*  titulus,
    constans character** parametra,
    constans character*  corpus)
{
    SilvaMacroDef* def;
    i32 i;

    def = _def_api_creare(exp, titulus, corpus);
    si (def == NIHIL)
    {
        redde FALSUM;
    }
    def->est_functio = VERUM;
    def->parametra = silva_xar_creare(exp->piscina, magnitudo(SilvaChorda*));

    per (i = ZEPHYRUM; parametra != NIHIL && parametra[i] != NIHIL; i++)
    {
        SilvaChorda** locus;
        constans character* titulus_parametri;

        titulus_parametri = parametra[i];
        si (strcmp(titulus_parametri, "...") == ZEPHYRUM)
        {
            def->est_variadica = VERUM;
            titulus_parametri = "__VA_ARGS__";
        }
        locus = (SilvaChorda**)silva_xar_addere(def->parametra);
        si (locus != NIHIL)
        {
            *locus = _chorda_figere(exp->piscina,
                silva_chorda_ex_literis(titulus_parametri, exp->piscina));
        }
    }

    _def_api_registrare(exp, def);
    redde VERUM;
}


/* ==================================================
 * Prospectus macro - oraculum GLR
 * ================================================== */

b32
silva_expansio_prospectare (
    SilvaExpansio*   exp,
    SilvaChorda           titulus,
    SilvaProspectus* prospectus_out)
{
    SilvaMacroDef* def;

    def = silva_expansio_quaerere(exp, titulus);
    si (def == NIHIL)
    {
        redde FALSUM;
    }

    prospectus_out->genus = SILVA_LEX_EOF;
    prospectus_out->est_vacuum = VERUM;
    prospectus_out->est_recursivum = FALSUM;

    si (def->corpus != NIHIL && silva_xar_numerus(def->corpus) > ZEPHYRUM)
    {
        SilvaToken* primum;

        primum = *(SilvaToken**)silva_xar_obtinere(def->corpus, ZEPHYRUM);
        prospectus_out->genus = primum->genus;
        prospectus_out->est_vacuum = FALSUM;
        si (_est_nomen_directivae(primum)
            && silva_expansio_quaerere(exp, primum->valor) != NIHIL)
        {
            prospectus_out->est_recursivum = VERUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Chunk B - Generationes expansionis
 * ================================================== */

/* Potestne lexema nomen macro esse? (identificator vel verbum clausum -
 * praeprocessor verba clausa non novit) */
interior b32
_est_nomen_potentiale (SilvaToken* token)
{
    si (token->genus == SILVA_LEX_IDENTIFICATOR)
    {
        redde VERUM;
    }
    si (token->genus >= SILVA_LEX_AUTO && token->genus <= SILVA_LEX_WHILE)
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* Caecatio lexematis (NIHIL = vacua) */
interior SilvaCaecatio*
_caecatio_lexematis (SilvaToken* token)
{
    commutatio (token->origo.genus)
    {
        casus SILVA_ORIGO_EXPANSIO:
            redde token->origo.datum.expansio.caecatio;
        casus SILVA_ORIGO_PASTA:
            redde token->origo.datum.pasta.caecatio;
        casus SILVA_ORIGO_FONS:
        casus SILVA_ORIGO_CHORDA:
        casus SILVA_ORIGO_API:
        ordinarius:
            redde NIHIL;
    }
}

/* Unio caecationum: elementa ex a super b consuntur (si absentia) */
interior SilvaCaecatio*
_caecatio_unire (SilvaPiscina* piscina, SilvaCaecatio* a, SilvaCaecatio* b)
{
    dum (a != NIHIL)
    {
        si (a->titulus != NIHIL
            && !silva_caecatio_continet(b, *a->titulus))
        {
            b = silva_caecatio_extendere(piscina, b, a->titulus);
        }
        a = a->cauda;
    }
    redde b;
}

/* Congruitne corpus-lexema parametro? Reddit indicem vel -1 */
interior s32
_parametrum_quaerere (SilvaMacroDef* def, SilvaToken* token)
{
    i32 i;
    i32 n;
    SilvaChorda* param;

    si (def->parametra == NIHIL || !_est_nomen_potentiale(token))
    {
        redde -I;
    }
    n = silva_xar_numerus(def->parametra);
    per (i = ZEPHYRUM; i < n; i++)
    {
        param = *(SilvaChorda**)silva_xar_obtinere(def->parametra, i);
        si (param->mensura == token->valor.mensura
            && param->mensura > ZEPHYRUM
            && memcmp(param->datum, token->valor.datum,
                   (memoriae_index)param->mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* Prae-declaratio (recursio: argumenta plene expanduntur) */
interior SilvaXar*
_expandere_plene (SilvaExpansio* exp, SilvaXar* lexemata);

/* Colligere argumenta invocationis. lexemata[i_paren] est '('.
 * Reddit Xar de Xar* (unum per argumentum) vel NIHIL si non
 * terminata (robustitas: invocatio abicitur, nomen manet).
 * *i_post_out = index post ')' clausam.
 * scissiones_maximae: numerus scissionum comma permissarum (-1 =
 * sine fine). Pro variadicis: cauda post parametra nominata manet
 * UNUM argumentum cum lexematibus comma VERIS suis. */
interior SilvaXar*
_argumenta_colligere (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    i32            i_paren,
    i32*           i_post_out,
    s32            scissiones_maximae)
{
    SilvaXar* argumenta;
    SilvaXar* currens;
    SilvaToken* t;
    i32 i;
    i32 n;
    i32 profunditas;
    s32 scissiones_factae;

    argumenta = silva_xar_creare(exp->piscina, magnitudo(SilvaXar*));
    currens = silva_xar_creare(exp->piscina, magnitudo(SilvaToken*));
    profunditas = I;
    scissiones_factae = ZEPHYRUM;
    n = silva_xar_numerus(lexemata);

    per (i = i_paren + I; i < n; i++)
    {
        t = *(SilvaToken**)silva_xar_obtinere(lexemata, i);
        si (t->genus == SILVA_LEX_EOF)
        {
            frange;  /* non terminata */
        }
        si (t->genus == SILVA_LEX_PAREN_APERTA)
        {
            profunditas++;
        }
        alioquin si (t->genus == SILVA_LEX_PAREN_CLAUSA)
        {
            profunditas--;
            si (profunditas == ZEPHYRUM)
            {
                SilvaXar** locus;

                locus = (SilvaXar**)silva_xar_addere(argumenta);
                si (locus != NIHIL)
                {
                    *locus = currens;
                }
                *i_post_out = i + I;
                redde argumenta;
            }
        }
        alioquin si (t->genus == SILVA_LEX_COMMA && profunditas == I
            && (scissiones_maximae < ZEPHYRUM
                || scissiones_factae < scissiones_maximae))
        {
            SilvaXar** locus;

            locus = (SilvaXar**)silva_xar_addere(argumenta);
            si (locus != NIHIL)
            {
                *locus = currens;
            }
            currens = silva_xar_creare(exp->piscina, magnitudo(SilvaToken*));
            scissiones_factae++;
            perge;
        }
        _lexema_addere(currens, t);
    }

    redde NIHIL;  /* EOF ante ')' - invocatio imperfecta */
}

/* ==================================================
 * Chunk C - # (stringificatio) et ## (pasta)
 * Operanda parametrorum CRUDA sumuntur (C89); ceterae
 * substitutiones argumenta prae-expansa accipiunt.
 * ================================================== */

/* Suntne trivia inter lexemata consecutiva? */
interior b32
_trivia_inter (SilvaToken* prius, SilvaToken* posterius)
{
    si (prius != NIHIL && prius->spatia_post != NIHIL
        && silva_xar_numerus(prius->spatia_post) > ZEPHYRUM)
    {
        redde VERUM;
    }
    si (posterius != NIHIL && posterius->spatia_ante != NIHIL
        && silva_xar_numerus(posterius->spatia_ante) > ZEPHYRUM)
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* Stringificatio (#): argumentum CRUDUM -> littera chordae.
 * Cursus albi unum spatium fiunt; " et \\ effugiuntur;
 * termini exteriores nulli (per constructionem). */
interior SilvaToken*
_stringificare (
    SilvaExpansio* exp,
    SilvaXar*           argumentum_crudum,
    SilvaToken*    invocatio,
    SilvaChorda*        nomen_macro)
{
    SilvaChordaAedificator* aed;
    SilvaToken* t;
    SilvaToken* prius;
    SilvaToken* primus;
    SilvaChorda valor;
    i32 i;
    i32 j;
    i32 n;

    aed = silva_chorda_aedificator_creare(exp->piscina, LXIV);
    silva_chorda_aedificator_appendere_character(aed, '"');

    prius = NIHIL;
    primus = NIHIL;
    n = (argumentum_crudum == NIHIL) ? ZEPHYRUM : silva_xar_numerus(argumentum_crudum);
    per (i = ZEPHYRUM; i < n; i++)
    {
        t = *(SilvaToken**)silva_xar_obtinere(argumentum_crudum, i);
        si (primus == NIHIL)
        {
            primus = t;
        }
        si (prius != NIHIL && _trivia_inter(prius, t))
        {
            silva_chorda_aedificator_appendere_character(aed, ' ');
        }
        per (j = ZEPHYRUM; j < t->valor.mensura; j++)
        {
            character c;

            c = (character)t->valor.datum[j];
            si (c == '"' || c == '\\')
            {
                silva_chorda_aedificator_appendere_character(aed, '\\');
            }
            silva_chorda_aedificator_appendere_character(aed, c);
        }
        prius = t;
    }

    silva_chorda_aedificator_appendere_character(aed, '"');
    valor = silva_chorda_aedificator_finire(aed);

    /* argumentum vacuum: catena originis in invocatione radicatur */
    si (primus == NIHIL)
    {
        primus = invocatio;
    }

    redde silva_token_ex_stringificatione(exp->piscina, valor, primus,
        nomen_macro);
}

/* Pasta (##): duo lexemata conglutinantur et RE-LEXANTUR.
 * Fructus unicus -> lexema pasta mundum; plures -> best-effort
 * (primum pasta, reliqua ut sunt). Emittit in exitus. */
interior vacuum
_conglutinare (
    SilvaExpansio* exp,
    SilvaToken*    sinister,
    SilvaToken*    dexter,
    SilvaChorda*        nomen_macro,
    SilvaCaecatio* hs,
    SilvaXar*           exitus)
{
    SilvaChordaAedificator* aed;
    SilvaChorda glutinum;
    SilvaXar* relexata;
    SilvaToken* t;
    SilvaToken* pasta;
    i32 i;
    i32 n_reales;

    aed = silva_chorda_aedificator_creare(exp->piscina, XXXII);
    silva_chorda_aedificator_appendere_chorda(aed, sinister->valor);
    silva_chorda_aedificator_appendere_chorda(aed, dexter->valor);
    glutinum = silva_chorda_aedificator_finire(aed);

    relexata = silva_lexare(exp->piscina, (constans character*)glutinum.datum,
        glutinum.mensura, -I);

    /* numerare lexemata realia (sine EOF) */
    n_reales = silva_xar_numerus(relexata) - I;
    si (n_reales <= ZEPHYRUM)
    {
        redde;  /* glutinum vacuum */
    }

    t = *(SilvaToken**)silva_xar_obtinere(relexata, ZEPHYRUM);
    pasta = silva_token_ex_pasta(exp->piscina, t->genus, t->valor,
        sinister, dexter, nomen_macro, hs);
    _lexema_addere(exitus, pasta);

    /* best-effort: reliqua emittuntur ut sunt */
    per (i = I; i < n_reales; i++)
    {
        _lexema_addere(exitus,
            *(SilvaToken**)silva_xar_obtinere(relexata, i));
    }
}

/* Item substitutionis: series lexematum unius positionis corporis */
interior SilvaXar*
_item_capere (
    SilvaExpansio* exp,
    SilvaMacroDef* def,
    SilvaToken*    corpus_tok,
    SilvaXar*           argumenta,   /* cruda vel expansa */
    b32*           est_param_out)
{
    s32 p;
    SilvaXar* unum;

    *est_param_out = FALSUM;
    si (argumenta != NIHIL)
    {
        p = _parametrum_quaerere(def, corpus_tok);
        si (p >= ZEPHYRUM && p < (s32)silva_xar_numerus(argumenta))
        {
            *est_param_out = VERUM;
            redde *(SilvaXar**)silva_xar_obtinere(argumenta, (i32)p);
        }
    }
    unum = silva_xar_creare(exp->piscina, magnitudo(SilvaToken*));
    _lexema_addere(unum, corpus_tok);
    redde unum;
}

/* Emittere seriem, quodque lexema involutum ex_expansione */
interior vacuum
_seriem_emittere (
    SilvaExpansio* exp,
    SilvaXar*           series,
    i32            a,
    i32            b,
    SilvaToken*    invocatio,
    SilvaChorda*        nomen_macro,
    SilvaCaecatio* hs,
    SilvaXar*           exitus)
{
    i32 i;
    SilvaToken* t;
    SilvaToken* novum;

    per (i = a; i < b; i++)
    {
        t = *(SilvaToken**)silva_xar_obtinere(series, i);
        novum = silva_token_ex_expansione(exp->piscina, t, invocatio,
            nomen_macro,
            _caecatio_unire(exp->piscina, _caecatio_lexematis(t), hs));
        _lexema_addere(exitus, novum);
    }
}

/* Substituere: corpus definitionis ambulatur cum # et ## tractatis.
 * argumenta_expansa: pro positionibus normalibus (NIHIL = obiectum-simile);
 * argumenta_cruda: pro operandis # et ## (semantica C89). */
interior vacuum
_substituere (
    SilvaExpansio* exp,
    SilvaMacroDef* def,
    SilvaToken*    invocatio,
    SilvaXar*           argumenta_expansa,
    SilvaXar*           argumenta_cruda,
    SilvaXar*           exitus)
{
    SilvaCaecatio* hs_nova;
    SilvaToken* corpus_tok;
    SilvaToken* sequens;
    i32 i;
    i32 n;

    hs_nova = silva_caecatio_extendere(exp->piscina,
        _caecatio_lexematis(invocatio), def->titulus);

    n = silva_xar_numerus(def->corpus);
    i = ZEPHYRUM;
    dum (i < n)
    {
        corpus_tok = *(SilvaToken**)silva_xar_obtinere(def->corpus, i);

        /* Stringificatio: # param (functio-similis solum) */
        si (corpus_tok->genus == SILVA_LEX_CANCELLUM
            && def->est_functio && i + I < n)
        {
            SilvaToken* post;
            s32 p;

            post = *(SilvaToken**)silva_xar_obtinere(def->corpus, i + I);
            p = _parametrum_quaerere(def, post);
            si (p >= ZEPHYRUM && argumenta_cruda != NIHIL
                && p < (s32)silva_xar_numerus(argumenta_cruda))
            {
                _lexema_addere(exitus, _stringificare(exp,
                    *(SilvaXar**)silva_xar_obtinere(argumenta_cruda, (i32)p),
                    invocatio, def->titulus));
                i = i + II;
                perge;
            }
        }

        /* Pasta: item ## item (## item)* - operanda cruda */
        sequens = (i + I < n)
            ? *(SilvaToken**)silva_xar_obtinere(def->corpus, i + I) : NIHIL;
        si (sequens != NIHIL
            && sequens->genus == SILVA_LEX_CANCELLUM_CANCELLUM
            && i + II < n)
        {
            SilvaXar* sinistra;
            b32 est_param;

            sinistra = _item_capere(exp, def, corpus_tok,
                argumenta_cruda, &est_param);
            i = i + I;  /* super primum operandum */

            dum (i < n
                && (*(SilvaToken**)silva_xar_obtinere(def->corpus, i))->genus
                    == SILVA_LEX_CANCELLUM_CANCELLUM
                && i + I < n)
            {
                SilvaXar* dextra;
                SilvaXar* nova_sinistra;
                SilvaToken* op_sin;
                SilvaToken* op_dex;
                i32 j;

                dextra = _item_capere(exp, def,
                    *(SilvaToken**)silva_xar_obtinere(def->corpus, i + I),
                    argumenta_cruda, &est_param);
                i = i + II;

                /* latus vacuum: alterum solum manet */
                si (silva_xar_numerus(sinistra) == ZEPHYRUM)
                {
                    sinistra = dextra;
                    perge;
                }
                si (silva_xar_numerus(dextra) == ZEPHYRUM)
                {
                    perge;
                }

                /* praecedentia sinistrae + pasta + sequentia dextrae */
                nova_sinistra = silva_xar_creare(exp->piscina,
                    magnitudo(SilvaToken*));
                per (j = ZEPHYRUM; j < silva_xar_numerus(sinistra) - I; j++)
                {
                    _lexema_addere(nova_sinistra,
                        *(SilvaToken**)silva_xar_obtinere(sinistra, j));
                }
                op_sin = *(SilvaToken**)silva_xar_obtinere(sinistra,
                    silva_xar_numerus(sinistra) - I);
                op_dex = *(SilvaToken**)silva_xar_obtinere(dextra, ZEPHYRUM);

                _conglutinare(exp, op_sin, op_dex, def->titulus, hs_nova,
                    nova_sinistra);

                per (j = I; j < silva_xar_numerus(dextra); j++)
                {
                    _lexema_addere(nova_sinistra,
                        *(SilvaToken**)silva_xar_obtinere(dextra, j));
                }
                sinistra = nova_sinistra;
            }

            /* emittere seriem confectam. Lexemata pasta iam involuta;
             * cetera involvenda. Simplex: pasta lexemata (origo PASTA)
             * transmittuntur, cetera ex_expansione involvuntur. */
            {
                i32 j;
                SilvaToken* t;

                per (j = ZEPHYRUM; j < silva_xar_numerus(sinistra); j++)
                {
                    t = *(SilvaToken**)silva_xar_obtinere(sinistra, j);
                    si (t->origo.genus == SILVA_ORIGO_PASTA)
                    {
                        _lexema_addere(exitus, t);
                    }
                    alioquin
                    {
                        _seriem_emittere(exp, sinistra, j, j + I,
                            invocatio, def->titulus, hs_nova, exitus);
                    }
                }
            }
            perge;
        }

        /* Positio normalis: param -> argumentum expansum; aliter corpus */
        {
            SilvaXar* series;
            b32 est_param;

            series = _item_capere(exp, def, corpus_tok,
                argumenta_expansa, &est_param);
            _seriem_emittere(exp, series, ZEPHYRUM, silva_xar_numerus(series),
                invocatio, def->titulus, hs_nova, exitus);
        }
        i++;
    }
}

/* Nucleus generationis. tabula (si non NIHIL) = tabula operans
 * positionalis, activata per ambulationem (quaerere eam videt, etiam
 * in expansione argumentorum nidificata); positus_localis = positiones
 * eventorum in FLUXU HOC, remappatae in situ ad positiones exitus
 * (generatio proxima eas legit). Ambo NIHIL = semantica tabulae vivae. */
interior SilvaXar*
_generatio_interna (
    SilvaExpansio*  exp,
    SilvaXar*            lexemata,
    b32*            mutatum_out,
    SilvaTabulaDispersa* tabula,
    s32*            positus_localis)
{
    SilvaXar* exitus;
    SilvaToken* token;
    SilvaMacroDef* def;
    SilvaTabulaDispersa* tabula_prior;
    i32 i;
    i32 n;
    i32 cursor;
    i32 n_acta;
    b32 mutatum;

    exitus = silva_xar_creare(exp->piscina, magnitudo(SilvaToken*));
    mutatum = FALSUM;
    n = silva_xar_numerus(lexemata);
    i = ZEPHYRUM;

    tabula_prior = exp->tabula_activa;
    si (tabula != NIHIL)
    {
        exp->tabula_activa = tabula;
    }
    cursor = ZEPHYRUM;
    n_acta = silva_xar_numerus(exp->acta);

    dum (i < n)
    {
        /* eventa ante lexema i applicare (status ad punctum) */
        si (positus_localis != NIHIL && tabula != NIHIL)
        {
            dum (cursor < n_acta && positus_localis[cursor] <= (s32)i)
            {
                SilvaEventum* eventum;

                eventum = (SilvaEventum*)silva_xar_obtinere(exp->acta, cursor);
                si (eventum->genus == SILVA_EVENTUM_DEFINITIO)
                {
                    silva_tabula_dispersa_inserere(tabula, *eventum->titulus,
                        (vacuum*)eventum->def);
                }
                alioquin
                {
                    silva_tabula_dispersa_delere(tabula, *eventum->titulus);
                }
                positus_localis[cursor] = (s32)silva_xar_numerus(exitus);
                cursor++;
            }
        }

        token = *(SilvaToken**)silva_xar_obtinere(lexemata, i);

        si (_est_nomen_potentiale(token))
        {
            def = silva_expansio_quaerere(exp, token->valor);
            si (def != NIHIL
                && !silva_caecatio_continet(_caecatio_lexematis(token),
                       *def->titulus))
            {
                si (!def->est_functio)
                {
                    _substituere(exp, def, token, NIHIL, NIHIL, exitus);
                    mutatum = VERUM;
                    i++;
                    perge;
                }
                alioquin si (i + I < n)
                {
                    SilvaToken* proximum;

                    proximum = *(SilvaToken**)silva_xar_obtinere(lexemata, i + I);
                    si (proximum->genus == SILVA_LEX_PAREN_APERTA)
                    {
                        SilvaXar* argumenta;
                        i32 i_post;
                        s32 scissiones;

                        /* variadica: scissiones = parametra nominata
                         * (cauda manet unum argumentum __VA_ARGS__) */
                        scissiones = -I;
                        si (def->est_variadica)
                        {
                            scissiones =
                                (s32)silva_xar_numerus(def->parametra) - I;
                        }

                        i_post = i;
                        argumenta = _argumenta_colligere(exp, lexemata,
                            i + I, &i_post, scissiones);
                        si (argumenta != NIHIL)
                        {
                            SilvaXar* expansa;
                            SilvaXar* arg;
                            SilvaXar** locus;
                            i32 j;
                            i32 m;

                            /* Extentum strati 0 memorare (Phase 5,
                             * sim ⑥ C5): reconstructio fontis octetos
                             * invocationis [nomen..')'] petit - solum
                             * cum nomen ipsum lexema FONTIS est */
                            si (token->origo.genus == SILVA_ORIGO_FONS)
                            {
                                SilvaExtentumInvocationis* ext;

                                ext = (SilvaExtentumInvocationis*)
                                    silva_xar_addere(exp->extenta);
                                si (ext != NIHIL)
                                {
                                    ext->invocatio = token;
                                    ext->lamina = _lamina_capere(exp,
                                        lexemata, i, i_post);
                                }
                            }

                            /* argumenta PLENE prae-expandere (S13) */
                            expansa = silva_xar_creare(exp->piscina, magnitudo(SilvaXar*));
                            m = silva_xar_numerus(argumenta);

                            /* macro sine parametris + unum argumentum
                             * vacuum = invocatio vacua */
                            si (!(m == I && silva_xar_numerus(def->parametra) == ZEPHYRUM
                                && silva_xar_numerus(*(SilvaXar**)silva_xar_obtinere(argumenta, 0))
                                    == ZEPHYRUM))
                            {
                                per (j = ZEPHYRUM; j < m; j++)
                                {
                                    arg = *(SilvaXar**)silva_xar_obtinere(argumenta, j);
                                    locus = (SilvaXar**)silva_xar_addere(expansa);
                                    si (locus != NIHIL)
                                    {
                                        *locus = _expandere_plene(exp, arg);
                                    }
                                }
                            }

                            _substituere(exp, def, token, expansa, argumenta, exitus);
                            mutatum = VERUM;
                            i = i_post;
                            perge;
                        }
                        /* imperfecta: nomen manet, cadit infra */
                    }
                }
            }
        }

        _lexema_addere(exitus, token);
        i++;
    }

    /* eventa post finem fluxus: remappare ad finem exitus */
    si (positus_localis != NIHIL)
    {
        dum (cursor < n_acta)
        {
            positus_localis[cursor] = (s32)silva_xar_numerus(exitus);
            cursor++;
        }
    }
    exp->tabula_activa = tabula_prior;

    si (mutatum_out != NIHIL)
    {
        *mutatum_out = mutatum;
    }
    redde exitus;
}

SilvaXar*
silva_expansio_generatio (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    b32*           mutatum_out)
{
    redde _generatio_interna(exp, lexemata, mutatum_out, NIHIL, NIHIL);
}

/* Limina circuituum fixorum (Phase 7): ante quamque generationem
 * inspiciuntur - volumen (limen_lexematum), numerus generationum
 * (limen_generationum, olim assertio), intermissio (pergere).
 * Limine tacto expansio CESSAT sed fluxus manet - lexemata reliqua
 * inexpansa fluunt (degradatio, non amputatio). Reddit VERUM si
 * pergendum. */
interior b32
_generationem_licere (
    SilvaExpansio* exp,
    SilvaXar*           currens,
    i32            generationes)
{
    si (exp->pergere != NIHIL && !exp->pergere(exp->pergere_datum))
    {
        exp->est_intermissa = VERUM;
        redde FALSUM;
    }
    si (exp->limen_generationum > ZEPHYRUM
        && generationes >= exp->limen_generationum)
    {
        exp->expansio_decisa = VERUM;
        exp->fines_tactae = VERUM;
        redde FALSUM;
    }
    si (exp->limen_lexematum > ZEPHYRUM
        && (i32)silva_xar_numerus(currens) > exp->limen_lexematum)
    {
        exp->expansio_decisa = VERUM;
        exp->fines_tactae = VERUM;
        redde FALSUM;
    }
    redde VERUM;
}

SilvaXar*
silva_expansio_expandere (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    SilvaXar**          strata_out)
{
    SilvaXar* currens;
    SilvaXar* strata;
    b32 mutatum;
    i32 generationes;

    strata = NIHIL;
    si (strata_out != NIHIL)
    {
        strata = silva_xar_creare(exp->piscina, magnitudo(SilvaXar*));
        *strata_out = strata;
    }

    currens = lexemata;
    generationes = ZEPHYRUM;
    dum (_generationem_licere(exp, currens, generationes))
    {
        SilvaXar* exitus;

        mutatum = FALSUM;
        exitus = silva_expansio_generatio(exp, currens, &mutatum);
        si (!mutatum)
        {
            frange;
        }
        currens = exitus;
        si (strata != NIHIL)
        {
            SilvaXar** locus;

            locus = (SilvaXar**)silva_xar_addere(strata);
            si (locus != NIHIL)
            {
                *locus = currens;
            }
        }
        generationes++;
    }

    redde currens;
}

interior SilvaXar*
_expandere_plene (SilvaExpansio* exp, SilvaXar* lexemata)
{
    redde silva_expansio_expandere(exp, lexemata, NIHIL);
}

SilvaXar*
silva_expansio_expandere_reliqua (
    SilvaExpansio* exp,
    SilvaXar*           reliqua,
    SilvaXar**          strata_out)
{
    SilvaXar* currens;
    SilvaXar* strata;
    s32* positus_localis;
    b32 mutatum;
    i32 generationes;
    i32 n_acta;
    i32 k;

    strata = NIHIL;
    si (strata_out != NIHIL)
    {
        strata = silva_xar_creare(exp->piscina, magnitudo(SilvaXar*));
        *strata_out = strata;
    }

    /* positiones locales eventorum: remappantur per generationem */
    n_acta = silva_xar_numerus(exp->acta);
    positus_localis = (s32*)silva_piscina_allocare(exp->piscina,
        (memoriae_index)(magnitudo(s32)
            * (n_acta > ZEPHYRUM ? (memoriae_index)n_acta : I)));
    si (positus_localis == NIHIL)
    {
        redde reliqua;
    }
    per (k = ZEPHYRUM; k < n_acta; k++)
    {
        positus_localis[k] =
            ((SilvaEventum*)silva_xar_obtinere(exp->acta, k))->positus;
    }

    currens = reliqua;
    generationes = ZEPHYRUM;
    dum (_generationem_licere(exp, currens, generationes))
    {
        SilvaTabulaDispersa* tabula;
        SilvaXar* exitus;

        mutatum = FALSUM;
        tabula = silva_tabula_dispersa_creare_chorda(exp->piscina, LXIV);
        exitus = _generatio_interna(exp, currens, &mutatum, tabula,
            positus_localis);
        si (!mutatum)
        {
            frange;
        }
        currens = exitus;
        si (strata != NIHIL)
        {
            SilvaXar** locus;

            locus = (SilvaXar**)silva_xar_addere(strata);
            si (locus != NIHIL)
            {
                *locus = currens;
            }
        }
        generationes++;
    }

    redde currens;
}

/* ==================================================
 * Fenestrae lectionis (additiones II)
 * ================================================== */

i32
silva_fontes_numerus (constans SilvaExpansio* exp)
{
    redde silva_xar_numerus(exp->fontes);
}

constans SilvaChorda*
silva_fons_via (constans SilvaExpansio* exp, s32 fons_index)
{
    constans SilvaFons* fons;

    si (fons_index < ZEPHYRUM
        || fons_index >= (s32)silva_xar_numerus(exp->fontes))
    {
        redde NIHIL;
    }
    fons = (constans SilvaFons*)silva_xar_obtinere(exp->fontes,
        (i32)fons_index);
    redde fons->via;
}

i32
silva_inclusiones_numerus (constans SilvaExpansio* exp)
{
    redde silva_xar_numerus(exp->inclusiones);
}

b32
silva_inclusio_vista (constans SilvaExpansio* exp, i32 index,
    SilvaInclusioVista* vista_out)
{
    constans SilvaInclusio* inclusio;

    si (index >= silva_xar_numerus(exp->inclusiones))
    {
        redde FALSUM;
    }
    inclusio = (constans SilvaInclusio*)silva_xar_obtinere(
        exp->inclusiones, index);
    vista_out->via = inclusio->via;
    vista_out->fons_ex = inclusio->fons_ex;
    vista_out->fons_ad = inclusio->fons_ad;
    vista_out->est_praetermissa = inclusio->est_praetermissa;
    redde VERUM;
}

i32
silva_rami_numerus (constans SilvaExpansio* exp)
{
    redde silva_xar_numerus(exp->rami);
}

b32
silva_ramus_vista (constans SilvaExpansio* exp, i32 index,
    SilvaRamusVista* vista_out)
{
    constans SilvaRamus* ramus;

    si (index >= silva_xar_numerus(exp->rami))
    {
        redde FALSUM;
    }
    ramus = *(SilvaRamus* constans*)silva_xar_obtinere(exp->rami, index);
    vista_out->genus = ramus->genus;
    vista_out->est_sumptum = ramus->est_sumptum;
    vista_out->est_numquam = ramus->est_numquam;
    vista_out->corpus_initium = ramus->corpus_initium;
    vista_out->corpus_finis = ramus->corpus_finis;
    si (ramus->regio != NIHIL)
    {
        vista_out->fons_index = ramus->regio->fons_index;
        vista_out->linea = ramus->regio->linea;
    }
    alioquin
    {
        vista_out->fons_index = -I;
        vista_out->linea = ZEPHYRUM;
    }
    redde VERUM;
}

i32
silva_macros_numerus (constans SilvaExpansio* exp)
{
    i32 n = silva_xar_numerus(exp->acta);
    i32 numerus = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        constans SilvaEventum* eventum =
            (constans SilvaEventum*)silva_xar_obtinere(exp->acta, k);

        si (eventum->genus == SILVA_EVENTUM_DEFINITIO)
        {
            numerus++;
        }
    }
    redde numerus;
}

b32
silva_macro_vista (constans SilvaExpansio* exp, i32 index,
    SilvaMacroVista* vista_out)
{
    i32 n = silva_xar_numerus(exp->acta);
    i32 visae = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        constans SilvaEventum* eventum =
            (constans SilvaEventum*)silva_xar_obtinere(exp->acta, k);

        si (eventum->genus != SILVA_EVENTUM_DEFINITIO)
        {
            perge;
        }
        si (visae == index)
        {
            vista_out->titulus = eventum->titulus;
            vista_out->est_functio = (eventum->def != NIHIL)
                ? eventum->def->est_functio : FALSUM;
            vista_out->fons_index = eventum->fons_index;
            vista_out->linea = eventum->linea;
            redde VERUM;
        }
        visae++;
    }
    redde FALSUM;
}

/* ================= ex silva/fontes/silva_conditio.c ================= */

/* ==================================================
 * Contextus evaluationis
 * ================================================== */

nomen structura {
    SilvaXar* lexemata;   /* SilvaToken* - post substitutionem et expansionem */
    i32  positus;
    i32  numerus;
    b32  error;
} SilvaConditioEval;

/* ==================================================
 * Praedeclarationes (descensus recursivus)
 * ================================================== */

interior s64 _evaluare_expressio (SilvaConditioEval* eval);
interior s64 _evaluare_ternarius (SilvaConditioEval* eval);
interior s64 _evaluare_disiunctio (SilvaConditioEval* eval);
interior s64 _evaluare_coniunctio (SilvaConditioEval* eval);
interior s64 _evaluare_vel_bitalis (SilvaConditioEval* eval);
interior s64 _evaluare_xor_bitalis (SilvaConditioEval* eval);
interior s64 _evaluare_et_bitalis (SilvaConditioEval* eval);
interior s64 _evaluare_aequalitas (SilvaConditioEval* eval);
interior s64 _evaluare_comparatio (SilvaConditioEval* eval);
interior s64 _evaluare_translatio (SilvaConditioEval* eval);
interior s64 _evaluare_additio (SilvaConditioEval* eval);
interior s64 _evaluare_multiplicatio (SilvaConditioEval* eval);
interior s64 _evaluare_unarium (SilvaConditioEval* eval);
interior s64 _evaluare_primarium (SilvaConditioEval* eval);

/* ==================================================
 * Auxiliares
 * ================================================== */

interior SilvaToken*
_lexema_currens (SilvaConditioEval* eval)
{
    si (eval->positus >= eval->numerus)
    {
        redde NIHIL;
    }
    redde *(SilvaToken**)silva_xar_obtinere(eval->lexemata, eval->positus);
}

interior SilvaLexemaGenus
_genus_currens (SilvaConditioEval* eval)
{
    SilvaToken* tok;

    tok = _lexema_currens(eval);
    si (tok == NIHIL)
    {
        redde SILVA_LEX_EOF;
    }
    redde tok->genus;
}

interior vacuum
_progredi (SilvaConditioEval* eval)
{
    si (eval->positus < eval->numerus)
    {
        eval->positus++;
    }
}

/* Potestne lexema nomen macro esse? (praeprocessor verba clausa non novit) */
interior b32
_est_nomen_conditionis (SilvaToken* token)
{
    si (token->genus == SILVA_LEX_IDENTIFICATOR)
    {
        redde VERUM;
    }
    si (token->genus >= SILVA_LEX_AUTO && token->genus <= SILVA_LEX_WHILE)
    {
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
_est_defined (SilvaToken* tok)
{
    si (tok == NIHIL || tok->genus != SILVA_LEX_IDENTIFICATOR)
    {
        redde FALSUM;
    }
    si (tok->valor.mensura != VII)
    {
        redde FALSUM;
    }
    redde (memcmp(tok->valor.datum, "defined", VII) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

/* Parsere litteram integram (dec/hex/oct, suffixa L/U praetermissa) */
interior s64
_parsere_integer (SilvaChorda valor)
{
    s64 fructus;
    s64 basis;
    i32 i;
    constans i8* p;

    fructus = ZEPHYRUM;
    basis = X;
    i = ZEPHYRUM;
    p = valor.datum;

    si (valor.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    si (i + I < valor.mensura && p[i] == '0')
    {
        si (p[i + I] == 'x' || p[i + I] == 'X')
        {
            basis = XVI;
            i += II;
        }
        alioquin si (p[i + I] >= '0' && p[i + I] <= '7')
        {
            basis = VIII;
            i++;
        }
    }

    dum (i < valor.mensura)
    {
        character c;
        s64 cifra;

        c = (character)p[i];

        si (c == 'L' || c == 'l' || c == 'U' || c == 'u')
        {
            frange;  /* suffixum */
        }

        si (c >= '0' && c <= '9')
        {
            cifra = (s64)(c - '0');
        }
        alioquin si (basis == XVI && c >= 'a' && c <= 'f')
        {
            cifra = X + (s64)(c - 'a');
        }
        alioquin si (basis == XVI && c >= 'A' && c <= 'F')
        {
            cifra = X + (s64)(c - 'A');
        }
        alioquin
        {
            frange;
        }

        si (cifra >= basis)
        {
            frange;
        }

        fructus = fructus * basis + cifra;
        i++;
    }

    redde fructus;
}

/* Littera characteris: 'a' vel '\n' etc. */
interior s64
_parsere_characterem (SilvaChorda valor)
{
    si (valor.mensura >= II && valor.datum[ZEPHYRUM] == '\'')
    {
        si (valor.mensura >= III && valor.datum[I] == '\\')
        {
            commutatio ((character)valor.datum[II])
            {
                casus 'n':  redde '\n';
                casus 't':  redde '\t';
                casus 'r':  redde '\r';
                casus '0':  redde ZEPHYRUM;
                casus '\\': redde '\\';
                casus '\'': redde '\'';
                ordinarius: redde (s64)valor.datum[II];
            }
        }
        redde (s64)valor.datum[I];
    }
    redde ZEPHYRUM;
}

/* ==================================================
 * Descensus recursivus
 *
 * Praecedentia (infima ad summam):
 *   ternarius ?:, || , &&, |, ^, &, == !=, < > <= >=,
 *   << >>, + -, * / %, unaria ! ~ - +, primaria
 * ================================================== */

interior s64
_evaluare_expressio (SilvaConditioEval* eval)
{
    redde _evaluare_ternarius(eval);
}

interior s64
_evaluare_ternarius (SilvaConditioEval* eval)
{
    s64 conditio;
    s64 valor_verus;
    s64 valor_falsus;

    conditio = _evaluare_disiunctio(eval);

    si (_genus_currens(eval) == SILVA_LEX_QUAESTIO)
    {
        _progredi(eval);
        valor_verus = _evaluare_expressio(eval);
        si (_genus_currens(eval) != SILVA_LEX_COLON)
        {
            eval->error = VERUM;
            redde ZEPHYRUM;
        }
        _progredi(eval);
        valor_falsus = _evaluare_ternarius(eval);
        redde conditio ? valor_verus : valor_falsus;
    }

    redde conditio;
}

interior s64
_evaluare_disiunctio (SilvaConditioEval* eval)
{
    s64 sinister;

    sinister = _evaluare_coniunctio(eval);
    dum (_genus_currens(eval) == SILVA_LEX_VEL_VEL)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_coniunctio(eval);
        sinister = (sinister || dexter) ? I : ZEPHYRUM;
    }
    redde sinister;
}

interior s64
_evaluare_coniunctio (SilvaConditioEval* eval)
{
    s64 sinister;

    sinister = _evaluare_vel_bitalis(eval);
    dum (_genus_currens(eval) == SILVA_LEX_ET_ET)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_vel_bitalis(eval);
        sinister = (sinister && dexter) ? I : ZEPHYRUM;
    }
    redde sinister;
}

interior s64
_evaluare_vel_bitalis (SilvaConditioEval* eval)
{
    s64 sinister;

    sinister = _evaluare_xor_bitalis(eval);
    dum (_genus_currens(eval) == SILVA_LEX_BARRA)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_xor_bitalis(eval);
        sinister = sinister | dexter;
    }
    redde sinister;
}

interior s64
_evaluare_xor_bitalis (SilvaConditioEval* eval)
{
    s64 sinister;

    sinister = _evaluare_et_bitalis(eval);
    dum (_genus_currens(eval) == SILVA_LEX_CARET)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_et_bitalis(eval);
        sinister = sinister ^ dexter;
    }
    redde sinister;
}

interior s64
_evaluare_et_bitalis (SilvaConditioEval* eval)
{
    s64 sinister;

    sinister = _evaluare_aequalitas(eval);
    dum (_genus_currens(eval) == SILVA_LEX_AMPERSAND)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_aequalitas(eval);
        sinister = sinister & dexter;
    }
    redde sinister;
}

interior s64
_evaluare_aequalitas (SilvaConditioEval* eval)
{
    s64 sinister;
    SilvaLexemaGenus op;

    sinister = _evaluare_comparatio(eval);
    dum ((op = _genus_currens(eval)) == SILVA_LEX_AEQUALIS_AEQUALIS
        || op == SILVA_LEX_NON_AEQUALIS)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_comparatio(eval);
        si (op == SILVA_LEX_AEQUALIS_AEQUALIS)
        {
            sinister = (sinister == dexter) ? I : ZEPHYRUM;
        }
        alioquin
        {
            sinister = (sinister != dexter) ? I : ZEPHYRUM;
        }
    }
    redde sinister;
}

interior s64
_evaluare_comparatio (SilvaConditioEval* eval)
{
    s64 sinister;
    SilvaLexemaGenus op;

    sinister = _evaluare_translatio(eval);
    dum ((op = _genus_currens(eval)) == SILVA_LEX_MINOR
        || op == SILVA_LEX_MAIOR
        || op == SILVA_LEX_MINOR_AEQUALIS
        || op == SILVA_LEX_MAIOR_AEQUALIS)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_translatio(eval);
        commutatio (op)
        {
            casus SILVA_LEX_MINOR:
                sinister = (sinister < dexter) ? I : ZEPHYRUM;
                frange;
            casus SILVA_LEX_MAIOR:
                sinister = (sinister > dexter) ? I : ZEPHYRUM;
                frange;
            casus SILVA_LEX_MINOR_AEQUALIS:
                sinister = (sinister <= dexter) ? I : ZEPHYRUM;
                frange;
            casus SILVA_LEX_MAIOR_AEQUALIS:
                sinister = (sinister >= dexter) ? I : ZEPHYRUM;
                frange;
            ordinarius:
                frange;
        }
    }
    redde sinister;
}

interior s64
_evaluare_translatio (SilvaConditioEval* eval)
{
    s64 sinister;
    SilvaLexemaGenus op;

    sinister = _evaluare_additio(eval);
    dum ((op = _genus_currens(eval)) == SILVA_LEX_SINISTRORSUM
        || op == SILVA_LEX_DEXTRORSUM)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_additio(eval);
        si (dexter < ZEPHYRUM || dexter >= LXIV)
        {
            eval->error = VERUM;  /* translatio extra fines: UB vitatum */
            redde ZEPHYRUM;
        }
        si (op == SILVA_LEX_SINISTRORSUM)
        {
            sinister = sinister << dexter;
        }
        alioquin
        {
            sinister = sinister >> dexter;
        }
    }
    redde sinister;
}

interior s64
_evaluare_additio (SilvaConditioEval* eval)
{
    s64 sinister;
    SilvaLexemaGenus op;

    sinister = _evaluare_multiplicatio(eval);
    dum ((op = _genus_currens(eval)) == SILVA_LEX_PLUS
        || op == SILVA_LEX_MINUS)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_multiplicatio(eval);
        si (op == SILVA_LEX_PLUS)
        {
            sinister = sinister + dexter;
        }
        alioquin
        {
            sinister = sinister - dexter;
        }
    }
    redde sinister;
}

interior s64
_evaluare_multiplicatio (SilvaConditioEval* eval)
{
    s64 sinister;
    SilvaLexemaGenus op;

    sinister = _evaluare_unarium(eval);
    dum ((op = _genus_currens(eval)) == SILVA_LEX_STAR
        || op == SILVA_LEX_SOLIDUS
        || op == SILVA_LEX_PERCENTUM)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_unarium(eval);
        si (op == SILVA_LEX_STAR)
        {
            sinister = sinister * dexter;
        }
        alioquin
        {
            si (dexter == ZEPHYRUM)
            {
                eval->error = VERUM;  /* divisio per zephyrum */
                redde ZEPHYRUM;
            }
            si (op == SILVA_LEX_SOLIDUS)
            {
                sinister = sinister / dexter;
            }
            alioquin
            {
                sinister = sinister % dexter;
            }
        }
    }
    redde sinister;
}

interior s64
_evaluare_unarium (SilvaConditioEval* eval)
{
    SilvaLexemaGenus op;

    op = _genus_currens(eval);

    si (op == SILVA_LEX_EXCLAMATIO)
    {
        s64 operandum;

        _progredi(eval);
        operandum = _evaluare_unarium(eval);
        redde operandum ? ZEPHYRUM : I;
    }
    si (op == SILVA_LEX_TILDE)
    {
        s64 operandum;

        _progredi(eval);
        operandum = _evaluare_unarium(eval);
        redde ~operandum;
    }
    si (op == SILVA_LEX_MINUS)
    {
        s64 operandum;

        _progredi(eval);
        operandum = _evaluare_unarium(eval);
        redde -operandum;
    }
    si (op == SILVA_LEX_PLUS)
    {
        _progredi(eval);
        redde _evaluare_unarium(eval);
    }

    redde _evaluare_primarium(eval);
}

interior s64
_evaluare_primarium (SilvaConditioEval* eval)
{
    SilvaToken* tok;

    tok = _lexema_currens(eval);
    si (tok == NIHIL)
    {
        eval->error = VERUM;
        redde ZEPHYRUM;
    }

    si (tok->genus == SILVA_LEX_INTEGER)
    {
        s64 valor;

        valor = _parsere_integer(tok->valor);
        _progredi(eval);
        redde valor;
    }

    si (tok->genus == SILVA_LEX_PAREN_APERTA)
    {
        s64 valor;

        _progredi(eval);
        valor = _evaluare_expressio(eval);
        si (_genus_currens(eval) != SILVA_LEX_PAREN_CLAUSA)
        {
            eval->error = VERUM;
            redde ZEPHYRUM;
        }
        _progredi(eval);
        redde valor;
    }

    si (tok->genus == SILVA_LEX_CHARACTER_LIT)
    {
        s64 valor;

        valor = _parsere_characterem(tok->valor);
        _progredi(eval);
        redde valor;
    }

    /* Identificator reliquus post expansionem = 0 (C89) */
    si (_est_nomen_conditionis(tok))
    {
        _progredi(eval);
        redde ZEPHYRUM;
    }

    eval->error = VERUM;
    redde ZEPHYRUM;
}

/* ==================================================
 * Substitutio 'defined' (protecta - ANTE expansionem)
 *
 * defined X / defined(X) -> lexema INTEGER syntheticum "1"/"0".
 * Operanda numquam expanduntur (C89 6.8.1). Reddit NIHIL in
 * forma malformata (evaluatio deficit).
 * ================================================== */

interior SilvaToken*
_integer_syntheticum (SilvaExpansio* exp, b32 valor, SilvaChorda* nomen_macro)
{
    SilvaChorda textus;
    unio { constans character* c; i8* m; } u;

    u.c = valor ? "1" : "0";
    textus.datum = u.m;
    textus.mensura = I;
    redde silva_token_ex_api(exp->piscina, SILVA_LEX_INTEGER, textus,
        nomen_macro, -I);
}

interior SilvaChorda*
_chordam_figere_conditionis (SilvaPiscina* piscina, SilvaChorda c)
{
    SilvaChorda* fixa;

    fixa = (SilvaChorda*)silva_piscina_allocare(piscina, (memoriae_index)magnitudo(SilvaChorda));
    si (fixa != NIHIL)
    {
        *fixa = c;
    }
    redde fixa;
}

interior SilvaXar*
_defined_substituere (SilvaExpansio* exp, SilvaXar* lexemata)
{
    SilvaXar* exitus;
    SilvaToken* tok;
    i32 i;
    i32 n;

    exitus = silva_xar_creare(exp->piscina, magnitudo(SilvaToken*));
    n = silva_xar_numerus(lexemata);
    i = ZEPHYRUM;

    dum (i < n)
    {
        tok = *(SilvaToken**)silva_xar_obtinere(lexemata, i);

        si (_est_defined(tok))
        {
            SilvaToken* operandum;
            SilvaToken** locus;
            b32 habet_paren;
            b32 est_def;

            i++;
            habet_paren = FALSUM;
            si (i < n && (*(SilvaToken**)silva_xar_obtinere(lexemata, i))->genus
                == SILVA_LEX_PAREN_APERTA)
            {
                habet_paren = VERUM;
                i++;
            }
            si (i >= n)
            {
                redde NIHIL;  /* malformata */
            }
            operandum = *(SilvaToken**)silva_xar_obtinere(lexemata, i);
            si (!_est_nomen_conditionis(operandum))
            {
                redde NIHIL;
            }
            i++;
            si (habet_paren)
            {
                si (i >= n || (*(SilvaToken**)silva_xar_obtinere(lexemata, i))->genus
                    != SILVA_LEX_PAREN_CLAUSA)
                {
                    redde NIHIL;
                }
                i++;
            }

            est_def = silva_conditio_est_definitum(exp, operandum->valor);
            locus = (SilvaToken**)silva_xar_addere(exitus);
            si (locus != NIHIL)
            {
                *locus = _integer_syntheticum(exp, est_def,
                    _chordam_figere_conditionis(exp->piscina, operandum->valor));
            }
            perge;
        }

        {
            SilvaToken** locus;

            locus = (SilvaToken**)silva_xar_addere(exitus);
            si (locus != NIHIL)
            {
                *locus = tok;
            }
        }
        i++;
    }

    redde exitus;
}

/* ==================================================
 * API publica
 * ================================================== */

b32
silva_conditio_est_definitum (
    SilvaExpansio* exp,
    SilvaChorda         titulus)
{
    si (exp == NIHIL)
    {
        redde FALSUM;
    }
    redde (silva_expansio_quaerere(exp, titulus) != NIHIL) ? VERUM : FALSUM;
}

s64
silva_conditio_evaluare (
    SilvaExpansio* exp,
    SilvaXar*           lexemata,
    b32*           successus)
{
    SilvaConditioEval eval;
    SilvaXar* substituta;
    SilvaXar* expansa;
    s64 fructus;

    si (successus != NIHIL)
    {
        *successus = FALSUM;
    }
    si (exp == NIHIL || lexemata == NIHIL || silva_xar_numerus(lexemata) == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    /* 1. defined-operanda protecte substituere */
    substituta = _defined_substituere(exp, lexemata);
    si (substituta == NIHIL)
    {
        redde ZEPHYRUM;
    }

    /* 2. expansio macro plena (expansor verus, caecationes) */
    expansa = silva_expansio_expandere(exp, substituta, NIHIL);

    /* 3. evaluatio arithmetica */
    eval.lexemata = expansa;
    eval.positus = ZEPHYRUM;
    eval.numerus = silva_xar_numerus(expansa);
    eval.error = FALSUM;

    si (eval.numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    fructus = _evaluare_expressio(&eval);

    si (eval.error || eval.positus < eval.numerus)
    {
        redde ZEPHYRUM;  /* error vel lexemata non consumpta */
    }

    si (successus != NIHIL)
    {
        *successus = VERUM;
    }
    redde fructus;
}

/* ================= ex silva/fontes/silva_nodus.c ================= */

/* ==================================================
 * Constructores valorum
 * ================================================== */

SilvaValor
silva_valor_nihil (vacuum)
{
    SilvaValor v;

    v.genus = SILVA_VALOR_NIHIL;
    v.datum.nodus = NIHIL;
    redde v;
}

SilvaValor
silva_valor_nodus (SilvaNodus* nodus)
{
    SilvaValor v;

    v.genus = SILVA_VALOR_NODUS;
    v.datum.nodus = nodus;
    redde v;
}

SilvaValor
silva_valor_token (SilvaToken* token)
{
    SilvaValor v;

    v.genus = SILVA_VALOR_TOKEN;
    v.datum.token = token;
    redde v;
}

SilvaValor
silva_valor_lista (SilvaXar* lista)
{
    SilvaValor v;

    v.genus = SILVA_VALOR_LISTA;
    v.datum.lista.xar = lista;
    v.datum.lista.mensura = silva_xar_numerus(lista);
    redde v;
}

SilvaValor
silva_valor_index (s32 index)
{
    SilvaValor v;

    v.genus = SILVA_VALOR_INDEX;
    v.datum.index = index;
    redde v;
}

SilvaValor
silva_valor_lista_nova (SilvaPiscina* piscina)
{
    redde silva_valor_lista(
        silva_xar_creare(piscina, magnitudo(SilvaValor)));
}

SilvaValor
silva_valor_lista_appendere (
    SilvaPiscina*   piscina,
    SilvaValor lista,
    SilvaValor elementum)
{
    SilvaXar*        xar;
    SilvaValor* slot;

    si (lista.genus != SILVA_VALOR_LISTA || lista.datum.lista.xar == NIHIL)
    {
        redde silva_valor_nihil();
    }
    xar = lista.datum.lista.xar;

    si (silva_xar_numerus(xar) != lista.datum.lista.mensura)
    {
        /* Divergentia: furca alia ultra prospectum meum scripsit.
         * Praefixum meum in repositorium recens copiatur - prospectus
         * eius intactus manet (numerus repositorii = numerator
         * versionum gratuitus). */
        SilvaXar* recens = silva_xar_creare(piscina, (i32)magnitudo(SilvaValor));
        i32  i;

        si (recens == NIHIL)
        {
            redde silva_valor_nihil();
        }
        per (i = ZEPHYRUM; i < lista.datum.lista.mensura; i++)
        {
            SilvaValor* fons = (SilvaValor*)silva_xar_obtinere(xar, i);
            SilvaValor* dest = (SilvaValor*)silva_xar_addere(recens);

            si (fons == NIHIL || dest == NIHIL)
            {
                redde silva_valor_nihil();
            }
            *dest = *fons;
        }
        xar = recens;
    }

    slot = (SilvaValor*)silva_xar_addere(xar);
    si (slot == NIHIL)
    {
        redde silva_valor_nihil();
    }
    *slot = elementum;

    {
        SilvaValor novus;

        novus.genus = SILVA_VALOR_LISTA;
        novus.datum.lista.xar = xar;
        novus.datum.lista.mensura = lista.datum.lista.mensura + I;
        redde novus;
    }
}

i32
silva_valor_lista_numerus (SilvaValor lista)
{
    si (lista.genus != SILVA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    redde lista.datum.lista.mensura;
}

SilvaValor*
silva_valor_lista_obtinere (SilvaValor lista, i32 index)
{
    si (lista.genus != SILVA_VALOR_LISTA || lista.datum.lista.xar == NIHIL
        || index >= lista.datum.lista.mensura)
    {
        redde NIHIL;
    }
    redde (SilvaValor*)silva_xar_obtinere(lista.datum.lista.xar, index);
}


/* ==================================================
 * Nodus
 * ================================================== */

SilvaNodus*
silva_nodus_creare (
    SilvaPiscina* piscina,
    s32      genus,
    i32      numerus_locorum)
{
    SilvaNodus* nodus;
    i32 i;

    nodus = (SilvaNodus*)silva_piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaNodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->genus = genus;
    nodus->numerus_locorum = numerus_locorum;
    nodus->pater = NIHIL;
    nodus->loci = NIHIL;

    si (numerus_locorum > ZEPHYRUM)
    {
        nodus->loci = (SilvaValor*)silva_piscina_allocare(piscina,
            (memoriae_index)(magnitudo(SilvaValor)
                * (memoriae_index)numerus_locorum));
        si (nodus->loci == NIHIL)
        {
            redde NIHIL;
        }
        per (i = ZEPHYRUM; i < numerus_locorum; i++)
        {
            nodus->loci[i] = silva_valor_nihil();
        }
    }
    redde nodus;
}

b32
silva_valor_congruit (
    SilvaValor        valor,
    SilvaLocusSpecies species)
{
    commutatio (species)
    {
        casus SILVA_LOCUS_NODUS:
            redde (valor.genus == SILVA_VALOR_NODUS) ? VERUM : FALSUM;
        casus SILVA_LOCUS_TOKEN:
            redde (valor.genus == SILVA_VALOR_TOKEN) ? VERUM : FALSUM;
        casus SILVA_LOCUS_LISTA_NODUS:
        casus SILVA_LOCUS_LISTA_TOKEN:
        casus SILVA_LOCUS_LISTA_MIXTA:
            redde (valor.genus == SILVA_VALOR_LISTA) ? VERUM : FALSUM;
        casus SILVA_LOCUS_INDEX:
            redde (valor.genus == SILVA_VALOR_INDEX) ? VERUM : FALSUM;
        ordinarius:
            redde FALSUM;
    }
}

b32
silva_nodus_ponere (
    SilvaNodus*       nodus,
    i32               locus,
    SilvaValor        valor,
    SilvaLocusSpecies species)
{
    si (nodus == NIHIL || locus >= nodus->numerus_locorum)
    {
        fprintf(stderr, "silva_nodus: locus %d extra fines\n", (int)locus);
        redde FALSUM;
    }
    /* S32: signum contra speciem */
    si (!silva_valor_congruit(valor, species))
    {
        fprintf(stderr,
            "silva_nodus: signum valoris (%d) speciei loci (%d) non congruit\n",
            (int)valor.genus, (int)species);
        redde FALSUM;
    }
    /* S32: semel tantum scribere (dominus unicus) */
    si (nodus->loci[locus].genus != SILVA_VALOR_NIHIL)
    {
        fprintf(stderr,
            "silva_nodus: locus %d iam scriptus (dominus duplex)\n",
            (int)locus);
        redde FALSUM;
    }
    nodus->loci[locus] = valor;
    redde VERUM;
}

b32
silva_nodus_appendere (
    SilvaPiscina*          piscina,
    SilvaNodus*       nodus,
    i32               locus,
    SilvaValor        valor,
    SilvaLocusSpecies species)
{
    SilvaValor novus;

    si (nodus == NIHIL || locus >= nodus->numerus_locorum)
    {
        redde FALSUM;
    }
    si (species != SILVA_LOCUS_LISTA_NODUS
        && species != SILVA_LOCUS_LISTA_TOKEN
        && species != SILVA_LOCUS_LISTA_MIXTA)
    {
        fprintf(stderr,
            "silva_nodus: appendere in locum non-listam (species %d)\n",
            (int)species);
        redde FALSUM;
    }
    si (nodus->loci[locus].genus == SILVA_VALOR_NIHIL)
    {
        nodus->loci[locus] = silva_valor_lista_nova(piscina);
    }
    si (nodus->loci[locus].genus != SILVA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    /* Appendere purum + religatio loci (nodus sub constructione
     * dominus unicus locorum suorum est) */
    novus = silva_valor_lista_appendere(piscina, nodus->loci[locus],
        valor);
    si (novus.genus != SILVA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    nodus->loci[locus] = novus;
    redde VERUM;
}

SilvaXar*
silva_nodus_liberi (
    SilvaPiscina*             piscina,
    constans SilvaNodus* nodus)
{
    SilvaXar* liberi;
    i32 i;

    liberi = silva_xar_creare(piscina, magnitudo(SilvaNodus*));
    si (nodus == NIHIL)
    {
        redde liberi;
    }

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans SilvaValor* v = &nodus->loci[i];

        si (v->genus == SILVA_VALOR_NODUS && v->datum.nodus != NIHIL)
        {
            SilvaNodus** locus = (SilvaNodus**)silva_xar_addere(liberi);

            si (locus != NIHIL)
            {
                *locus = v->datum.nodus;
            }
        }
        alioquin si (v->genus == SILVA_VALOR_LISTA
            && v->datum.lista.xar != NIHIL)
        {
            i32 j;

            /* Mensura prospectus, non numerus repositorii (A½) */
            per (j = ZEPHYRUM; j < v->datum.lista.mensura; j++)
            {
                SilvaValor* e = (SilvaValor*)silva_xar_obtinere(
                    v->datum.lista.xar, j);

                si (e != NIHIL && e->genus == SILVA_VALOR_NODUS
                    && e->datum.nodus != NIHIL)
                {
                    SilvaNodus** locus = (SilvaNodus**)silva_xar_addere(liberi);

                    si (locus != NIHIL)
                    {
                        *locus = e->datum.nodus;
                    }
                }
            }
        }
    }
    redde liberi;
}

/* ================= ex silva/fontes/silva_tabulae_sceleti.c ================= */

/* ==================================================
 * Symbola
 * ================================================== */

hic_manens constans SilvaTabSymbolum SILVA_SCELETUM_SYMBOLA[19] = {
    /* [ 0] */ { "IDENTIFICATOR", SILVA_LEX_IDENTIFICATOR, VERUM },
    /* [ 1] */ { "INTEGER", SILVA_LEX_INTEGER, VERUM },
    /* [ 2] */ { "INT", SILVA_LEX_INT, VERUM },
    /* [ 3] */ { "STAR", SILVA_LEX_STAR, VERUM },
    /* [ 4] */ { "PLUS", SILVA_LEX_PLUS, VERUM },
    /* [ 5] */ { "SEMICOLON", SILVA_LEX_SEMICOLON, VERUM },
    /* [ 6] */ { "PAREN_APERTA", SILVA_LEX_PAREN_APERTA, VERUM },
    /* [ 7] */ { "PAREN_CLAUSA", SILVA_LEX_PAREN_CLAUSA, VERUM },
    /* [ 8] */ { "EOF", SILVA_LEX_EOF, VERUM },
    /* [ 9] */ { "sententiae", SILVA_LEX_EOF, FALSUM },
    /* [10] */ { "sententia", SILVA_LEX_EOF, FALSUM },
    /* [11] */ { "declaratio", SILVA_LEX_EOF, FALSUM },
    /* [12] */ { "typus", SILVA_LEX_EOF, FALSUM },
    /* [13] */ { "declarator", SILVA_LEX_EOF, FALSUM },
    /* [14] */ { "sententia-expressionis", SILVA_LEX_EOF, FALSUM },
    /* [15] */ { "expressio", SILVA_LEX_EOF, FALSUM },
    /* [16] */ { "terminus", SILVA_LEX_EOF, FALSUM },
    /* [17] */ { "factor", SILVA_LEX_EOF, FALSUM },
    /* [18] */ { "__initium__", SILVA_LEX_EOF, FALSUM }
};

/* ==================================================
 * Productiones
 * ================================================== */

hic_manens constans SilvaTabProductio SILVA_SCELETUM_PRODUCTIONES[18] = {
    /* P0  sententiae (0 symbola) */ { 9, 0, NIHIL },
    /* P1  sententiae (2 symbola) */ { 9, 2, "sententiae-appendere" },
    /* P2  sententia (1 symbola) */ { 10, 1, NIHIL },
    /* P3  sententia (1 symbola) */ { 10, 1, NIHIL },
    /* P4  declaratio (3 symbola) */ { 11, 3, "declaratio-simplex" },
    /* P5  typus (1 symbola) */ { 12, 1, "typus-int" },
    /* P6  typus (1 symbola) */ { 12, 1, "typus-nomen" },
    /* P7  declarator (1 symbola) */ { 13, 1, "declarator-nomen" },
    /* P8  declarator (2 symbola) */ { 13, 2, "declarator-stella" },
    /* P9  sententia-expressionis (2 symbola) */ { 14, 2, "sententia-expr" },
    /* P10 expressio (3 symbola) */ { 15, 3, "expressio-plus" },
    /* P11 expressio (1 symbola) */ { 15, 1, NIHIL },
    /* P12 terminus (3 symbola) */ { 16, 3, "terminus-star" },
    /* P13 terminus (1 symbola) */ { 16, 1, NIHIL },
    /* P14 factor (1 symbola) */ { 17, 1, "factor-identificator" },
    /* P15 factor (1 symbola) */ { 17, 1, "factor-integer" },
    /* P16 factor (3 symbola) */ { 17, 3, "factor-parenthesis" },
    /* P17 __initium__ (2 symbola) */ { 18, 2, NIHIL }
};

/* ==================================================
 * Actiones (laminae per statum in SILVA_SCELETUM_STATUS)
 * ================================================== */

hic_manens constans SilvaTabActio SILVA_SCELETUM_ACTIONES[] = {
    /* --- status 0 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* IDENTIFICATOR: reducere P0 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* INTEGER: reducere P0 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* INT: reducere P0 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* PAREN_APERTA: reducere P0 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* EOF: reducere P0 */
    /* --- status 1 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,   7, 0 },  /* IDENTIFICATOR: transponere -> 7 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* INTEGER: transponere -> 11 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,   6, 0 },  /* INT: transponere -> 6 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* PAREN_APERTA: transponere -> 12 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* EOF: transponere -> 13 */
    /* --- status 2 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* IDENTIFICATOR: reducere P1 sententiae-appendere */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* INTEGER: reducere P1 sententiae-appendere */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* INT: reducere P1 sententiae-appendere */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* PAREN_APERTA: reducere P1 sententiae-appendere */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* EOF: reducere P1 sententiae-appendere */
    /* --- status 3 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* IDENTIFICATOR: reducere P2 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* INTEGER: reducere P2 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* INT: reducere P2 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* PAREN_APERTA: reducere P2 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* EOF: reducere P2 */
    /* --- status 4 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* IDENTIFICATOR: reducere P3 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* INTEGER: reducere P3 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* INT: reducere P3 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* PAREN_APERTA: reducere P3 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* EOF: reducere P3 */
    /* --- status 5 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* IDENTIFICATOR: transponere -> 15 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  16, 0 },  /* STAR: transponere -> 16 */
    /* --- status 6 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* IDENTIFICATOR: reducere P5 typus-int */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* STAR: reducere P5 typus-int */
    /* --- status 7 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* IDENTIFICATOR: reducere P6 typus-nomen */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STAR: reducere P6 typus-nomen */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* STAR: reducere P14 factor-identificator */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* PLUS: reducere P14 factor-identificator */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* SEMICOLON: reducere P14 factor-identificator */
    /* --- status 8 --- */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  18, 0 },  /* PLUS: transponere -> 18 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  17, 0 },  /* SEMICOLON: transponere -> 17 */
    /* --- status 9 --- */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* STAR: transponere -> 19 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* PLUS: reducere P11 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* SEMICOLON: reducere P11 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* PAREN_CLAUSA: reducere P11 */
    /* --- status 10 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* STAR: reducere P13 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* PLUS: reducere P13 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* SEMICOLON: reducere P13 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* PAREN_CLAUSA: reducere P13 */
    /* --- status 11 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* STAR: reducere P15 factor-integer */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* PLUS: reducere P15 factor-integer */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* SEMICOLON: reducere P15 factor-integer */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* PAREN_CLAUSA: reducere P15 factor-integer */
    /* --- status 12 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* IDENTIFICATOR: transponere -> 21 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* INTEGER: transponere -> 11 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* PAREN_APERTA: transponere -> 12 */
    /* --- status 13 --- */
    {  -1, SILVA_TAB_ACTIO_ACCIPERE,   0, 0 },  /* $: accipere */
    /* --- status 14 --- */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* SEMICOLON: transponere -> 22 */
    /* --- status 15 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* SEMICOLON: reducere P7 declarator-nomen */
    /* --- status 16 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* IDENTIFICATOR: transponere -> 15 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  16, 0 },  /* STAR: transponere -> 16 */
    /* --- status 17 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* IDENTIFICATOR: reducere P9 sententia-expr */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* INTEGER: reducere P9 sententia-expr */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* INT: reducere P9 sententia-expr */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* PAREN_APERTA: reducere P9 sententia-expr */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* EOF: reducere P9 sententia-expr */
    /* --- status 18 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* IDENTIFICATOR: transponere -> 21 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* INTEGER: transponere -> 11 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* PAREN_APERTA: transponere -> 12 */
    /* --- status 19 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* IDENTIFICATOR: transponere -> 21 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* INTEGER: transponere -> 11 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* PAREN_APERTA: transponere -> 12 */
    /* --- status 20 --- */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  18, 0 },  /* PLUS: transponere -> 18 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* PAREN_CLAUSA: transponere -> 26 */
    /* --- status 21 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* STAR: reducere P14 factor-identificator */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* PLUS: reducere P14 factor-identificator */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* SEMICOLON: reducere P14 factor-identificator */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* PAREN_CLAUSA: reducere P14 factor-identificator */
    /* --- status 22 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* IDENTIFICATOR: reducere P4 declaratio-simplex */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* INTEGER: reducere P4 declaratio-simplex */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* INT: reducere P4 declaratio-simplex */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* PAREN_APERTA: reducere P4 declaratio-simplex */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* EOF: reducere P4 declaratio-simplex */
    /* --- status 23 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* SEMICOLON: reducere P8 declarator-stella */
    /* --- status 24 --- */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* STAR: transponere -> 19 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* PLUS: reducere P10 expressio-plus */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* SEMICOLON: reducere P10 expressio-plus */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* PAREN_CLAUSA: reducere P10 expressio-plus */
    /* --- status 25 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* STAR: reducere P12 terminus-star */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* PLUS: reducere P12 terminus-star */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* SEMICOLON: reducere P12 terminus-star */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* PAREN_CLAUSA: reducere P12 terminus-star */
    /* --- status 26 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* STAR: reducere P16 factor-parenthesis */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* PLUS: reducere P16 factor-parenthesis */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* SEMICOLON: reducere P16 factor-parenthesis */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* PAREN_CLAUSA: reducere P16 factor-parenthesis */
};

/* ==================================================
 * Goto
 * ================================================== */

hic_manens constans SilvaTabGoto SILVA_SCELETUM_GOTO[] = {
    /* --- status 0 --- */
    {   9,   1 },  /* sententiae -> 1 */
    /* --- status 1 --- */
    {  10,   2 },  /* sententia -> 2 */
    {  11,   3 },  /* declaratio -> 3 */
    {  12,   5 },  /* typus -> 5 */
    {  14,   4 },  /* sententia-expressionis -> 4 */
    {  15,   8 },  /* expressio -> 8 */
    {  16,   9 },  /* terminus -> 9 */
    {  17,  10 },  /* factor -> 10 */
    /* --- status 5 --- */
    {  13,  14 },  /* declarator -> 14 */
    /* --- status 12 --- */
    {  15,  20 },  /* expressio -> 20 */
    {  16,   9 },  /* terminus -> 9 */
    {  17,  10 },  /* factor -> 10 */
    /* --- status 16 --- */
    {  13,  23 },  /* declarator -> 23 */
    /* --- status 18 --- */
    {  16,  24 },  /* terminus -> 24 */
    {  17,  10 },  /* factor -> 10 */
    /* --- status 19 --- */
    {  17,  25 },  /* factor -> 25 */
    { -1, -1 }   /* terminator */
};

/* ==================================================
 * Status (offset + numerus in series planas)
 * ================================================== */

hic_manens constans SilvaTabStatus SILVA_SCELETUM_STATUS[27] = {
    /* status  0 */ {    0,  5,    0,  1 },
    /* status  1 */ {    5,  5,    1,  7 },
    /* status  2 */ {   10,  5,    8,  0 },
    /* status  3 */ {   15,  5,    8,  0 },
    /* status  4 */ {   20,  5,    8,  0 },
    /* status  5 */ {   25,  2,    8,  1 },
    /* status  6 */ {   27,  2,    9,  0 },
    /* status  7 */ {   29,  5,    9,  0 },
    /* status  8 */ {   34,  2,    9,  0 },
    /* status  9 */ {   36,  4,    9,  0 },
    /* status 10 */ {   40,  4,    9,  0 },
    /* status 11 */ {   44,  4,    9,  0 },
    /* status 12 */ {   48,  3,    9,  3 },
    /* status 13 */ {   51,  1,   12,  0 },
    /* status 14 */ {   52,  1,   12,  0 },
    /* status 15 */ {   53,  1,   12,  0 },
    /* status 16 */ {   54,  2,   12,  1 },
    /* status 17 */ {   56,  5,   13,  0 },
    /* status 18 */ {   61,  3,   13,  2 },
    /* status 19 */ {   64,  3,   15,  1 },
    /* status 20 */ {   67,  2,   16,  0 },
    /* status 21 */ {   69,  4,   16,  0 },
    /* status 22 */ {   73,  5,   16,  0 },
    /* status 23 */ {   78,  1,   16,  0 },
    /* status 24 */ {   79,  4,   16,  0 },
    /* status 25 */ {   83,  4,   16,  0 },
    /* status 26 */ {   87,  4,   16,  0 }
};

/* ==================================================
 * Tabula cocta integra
 * ================================================== */

constans SilvaTabulaCocta SILVA_SCELETUM_TABULA = {
    SILVA_SCELETUM_SYMBOLA, 19,
    9,  /* terminalia */
    SILVA_SCELETUM_PRODUCTIONES, 18,
    SILVA_SCELETUM_STATUS, 27,
    SILVA_SCELETUM_ACTIONES, 91,
    SILVA_SCELETUM_GOTO, 16,
    18,  /* initium (augmentatum) */
    17,  /* productio augmentata */
    1   /* conflictus servati */
};

/* ==================================================
 * Registrum generum (layouts locorum)
 * ================================================== */

hic_manens constans SilvaTabLocus SILVA_SCELETUM_LOCI[] = {
    /* declaratio */
    { "typus", SILVA_LOCUS_NODUS },
    { "declarator", SILVA_LOCUS_NODUS },
    { "tok_terminator", SILVA_LOCUS_TOKEN },
    /* typus-primitivus */
    { "tok_verbum", SILVA_LOCUS_TOKEN },
    /* typus-nominatus */
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    /* declarator-titulus */
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    /* declarator-monstrator */
    { "tok_stella", SILVA_LOCUS_TOKEN },
    { "internum", SILVA_LOCUS_NODUS },
    /* sententia-expressionis */
    { "expressio", SILVA_LOCUS_NODUS },
    { "tok_terminator", SILVA_LOCUS_TOKEN },
    /* binarium */
    { "sinister", SILVA_LOCUS_NODUS },
    { "tok_operator", SILVA_LOCUS_TOKEN },
    { "dexter", SILVA_LOCUS_NODUS },
    /* folium-identificator */
    { "tok_valor", SILVA_LOCUS_TOKEN },
    /* folium-integer */
    { "tok_valor", SILVA_LOCUS_TOKEN },
    /* parenthesis */
    { "tok_apertum", SILVA_LOCUS_TOKEN },
    { "internum", SILVA_LOCUS_NODUS },
    { "tok_clausum", SILVA_LOCUS_TOKEN },
    /* error */
    { "tokens", SILVA_LOCUS_LISTA_TOKEN },
    /* ambiguus */
    { "interpretationes", SILVA_LOCUS_LISTA_NODUS },
    { "canonica", SILVA_LOCUS_INDEX },
    /* conditionalis */
    { "rami", SILVA_LOCUS_LISTA_NODUS },
    { "finis", SILVA_LOCUS_LISTA_TOKEN },
    /* ramus-sumptus */
    { "directiva", SILVA_LOCUS_LISTA_TOKEN },
    { "contentum", SILVA_LOCUS_LISTA_NODUS },
    { "conditio_id", SILVA_LOCUS_INDEX },
    /* ramus-omissus */
    { "directiva", SILVA_LOCUS_LISTA_TOKEN },
    { "cruda", SILVA_LOCUS_LISTA_TOKEN },
    { "conditio_id", SILVA_LOCUS_INDEX },
    { NIHIL, -1 }   /* terminator */
};

hic_manens constans SilvaTabGenus SILVA_SCELETUM_GENERA[] = {
    /* [ 0] */ { "declaratio", 0, 3 },
    /* [ 1] */ { "typus-primitivus", 3, 1 },
    /* [ 2] */ { "typus-nominatus", 4, 1 },
    /* [ 3] */ { "declarator-titulus", 5, 1 },
    /* [ 4] */ { "declarator-monstrator", 6, 2 },
    /* [ 5] */ { "sententia-expressionis", 8, 2 },
    /* [ 6] */ { "binarium", 10, 3 },
    /* [ 7] */ { "folium-identificator", 13, 1 },
    /* [ 8] */ { "folium-integer", 14, 1 },
    /* [ 9] */ { "parenthesis", 15, 3 },
    /* [10] */ { "error", 18, 1 },
    /* [11] */ { "ambiguus", 19, 2 },
    /* [12] */ { "conditionalis", 21, 2 },
    /* [13] */ { "ramus-sumptus", 23, 3 },
    /* [14] */ { "ramus-omissus", 26, 3 },
    { NIHIL, 0, 0 }   /* terminator */
};

constans SilvaRegistrumCoctum SILVA_SCELETUM_REGISTRUM = {
    SILVA_SCELETUM_GENERA, 15,
    SILVA_SCELETUM_LOCI, 29
};

/* ==================================================
 * Accessores (probati: genus + fines)
 * ================================================== */

SilvaValor
silva_sceletum_declaratio_typus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATIO
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_declaratio_declarator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATIO
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_declaratio_tok_terminator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATIO
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_sceletum_typus_primitivus_tok_verbum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_TYPUS_PRIMITIVUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_typus_nominatus_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_declarator_titulus_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATOR_TITULUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_declarator_monstrator_tok_stella (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATOR_MONSTRATOR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_declarator_monstrator_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATOR_MONSTRATOR
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_sententia_expressionis_expressio (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_sententia_expressionis_tok_terminator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_binarium_sinister (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_BINARIUM
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_binarium_tok_operator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_BINARIUM
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_binarium_dexter (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_BINARIUM
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_sceletum_folium_identificator_tok_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_FOLIUM_IDENTIFICATOR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_folium_integer_tok_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_FOLIUM_INTEGER
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_parenthesis_tok_apertum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_PARENTHESIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_parenthesis_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_PARENTHESIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_parenthesis_tok_clausum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_PARENTHESIS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_sceletum_error_tokens (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_ERROR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_ambiguus_interpretationes (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_AMBIGUUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_ambiguus_canonica (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_AMBIGUUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_conditionalis_rami (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_CONDITIONALIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_conditionalis_finis (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_CONDITIONALIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_ramus_sumptus_directiva (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_SUMPTUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_ramus_sumptus_contentum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_SUMPTUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_ramus_sumptus_conditio_id (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_SUMPTUS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_sceletum_ramus_omissus_directiva (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_OMISSUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_ramus_omissus_cruda (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_OMISSUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_ramus_omissus_conditio_id (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_OMISSUS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

/* ==================================================
 * Constructio ex reductione (PURA - S26; pater numquam - S27)
 * ================================================== */

SilvaValor
silva_sceletum_construere (
    SilvaPiscina*             piscina,
    s32                  productio,
    constans SilvaValor* valores)
{
    (vacuum)piscina;
    (vacuum)valores;
    commutatio (productio)
    {
    casus 0:  /* transitus */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        redde lista;
    }
    casus 1:  /* sententiae-appendere */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 2:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 3:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 4:  /* declaratio-simplex */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 5:  /* typus-int */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_TYPUS_PRIMITIVUS, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 6:  /* typus-nomen */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 7:  /* declarator-nomen */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_DECLARATOR_TITULUS, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 8:  /* declarator-stella */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_DECLARATOR_MONSTRATOR, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 9:  /* sententia-expr */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 10:  /* expressio-plus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 11:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 12:  /* terminus-star */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 13:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 14:  /* factor-identificator */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_FOLIUM_IDENTIFICATOR, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 15:  /* factor-integer */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_FOLIUM_INTEGER, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 16:  /* factor-parenthesis */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_PARENTHESIS, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 17:  /* augmentata */
        redde valores[ZEPHYRUM];
    ordinarius:
        redde silva_valor_nihil();
    }
}

/* ==================================================
 * Fabrica ambigui (spec-v2 par 12)
 * ================================================== */

SilvaValor
silva_sceletum_ambiguum_fabricare (
    SilvaPiscina*   piscina,
    SilvaValor interpretationes,
    s32        canonica)
{
    SilvaNodus* nodus = silva_nodus_creare(piscina,
        (s32)SILVA_SCELETUM_GENUS_AMBIGUUS, 2);
    si (nodus == NIHIL)
    {
        redde silva_valor_nihil();
    }
    si (!silva_nodus_ponere(nodus, 0, interpretationes, SILVA_LOCUS_LISTA_NODUS))
    {
        redde silva_valor_nihil();
    }
    si (!silva_nodus_ponere(nodus, 1, silva_valor_index(canonica),
            SILVA_LOCUS_INDEX))
    {
        redde silva_valor_nihil();
    }
    redde silva_valor_nodus(nodus);
}

/* ================= ex silva/fontes/silva_glr.c ================= */

/* ==================================================
 * Via per GSS
 * ================================================== */

nomen structura {
    SilvaValor     valores[SILVA_GLR_DEXTRUM_MAXIMUM];
    SilvaGSSNodus* basis;     /* nodus sub via - fons goto */
} SilvaGSSVia;

/* Introitus indicis reducendorum (spec-v2 par 12): quisque nodus
 * reductione creatus sub clavi (status, basis) registratur. Fenestra
 * transmutationis = UNA exhaustio (index per lexema recens - regula
 * ordinis quae periculum retroactivitatis claudit). */
nomen structura {
    s32            status;
    SilvaGSSNodus* basis;
    SilvaGSSNodus* nodus;
    b32            exhaustus;         /* reductiones eius iam cucurrerunt */
    b32            est_ambiguum;      /* valor iam involucrum AMBIGUUS */
    SilvaValor     interpretationes;  /* prospectus - ab motore tentus, ne
                                       * nodum umquam legere debeat */
} SilvaIndexIntroitus;

/* Contextus passus unius lexematis */
nomen structura {
    s32         terminale;
    SilvaToken* lexema;           /* NIHIL in prospectu $ */
    SilvaXar*        index;            /* SilvaIndexIntroitus (per valorem) */
    SilvaXar*        reducenda;        /* SilvaIndexIntroitus* (FIFO) */
    SilvaXar*        frons_nova;       /* SilvaGSSNodus* */
    SilvaXar*        acceptati;        /* SilvaGSSNodus* */
    SilvaPiscina*    silva_piscina_arborum;
} SilvaGLRPassus;


/* ==================================================
 * Aequalitas structuralis (recursio generica - modellum
 * nodorum uniforme nullos casus per-genus poscit)
 * ================================================== */

interior b32 _valores_aequales (SilvaValor a, SilvaValor b);

interior b32
_nodi_aequales (constans SilvaNodus* a, constans SilvaNodus* b)
{
    i32 i;

    si (a == b)
    {
        redde VERUM;
    }
    si (a == NIHIL || b == NIHIL || a->genus != b->genus
        || a->numerus_locorum != b->numerus_locorum)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < a->numerus_locorum; i++)
    {
        si (!_valores_aequales(a->loci[i], b->loci[i]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_valores_aequales (SilvaValor a, SilvaValor b)
{
    si (a.genus != b.genus)
    {
        redde FALSUM;
    }
    commutatio (a.genus)
    {
    casus SILVA_VALOR_NIHIL:
        redde VERUM;
    casus SILVA_VALOR_NODUS:
        redde _nodi_aequales(a.datum.nodus, b.datum.nodus);
    casus SILVA_VALOR_TOKEN:
        /* IDENTITAS monstratoris: parsurae eiusdem fluxus eadem
         * lexemata communicant; aequalitas textus positiones fontis
         * diversas false funderet */
        redde (a.datum.token == b.datum.token) ? VERUM : FALSUM;
    casus SILVA_VALOR_LISTA:
    {
        i32 n = silva_valor_lista_numerus(a);
        i32 i;

        si (n != silva_valor_lista_numerus(b))
        {
            redde FALSUM;
        }
        si (a.datum.lista.xar == b.datum.lista.xar)
        {
            redde VERUM;  /* idem repositorium + eadem mensura */
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            SilvaValor* ea = silva_valor_lista_obtinere(a, i);
            SilvaValor* eb = silva_valor_lista_obtinere(b, i);

            si (ea == NIHIL || eb == NIHIL
                || !_valores_aequales(*ea, *eb))
            {
                redde FALSUM;
            }
        }
        redde VERUM;
    }
    casus SILVA_VALOR_INDEX:
        redde (a.datum.index == b.datum.index) ? VERUM : FALSUM;
    ordinarius:
        redde FALSUM;
    }
}


/* ==================================================
 * Index reducendorum + compactio + transmutatio
 * ================================================== */

/* Quaerere clavem exactam; status_visus_out = VERUM si status idem
 * cum basi ALIA visus (classis Farshi - numeratur, non compingi
 * potest) */
interior SilvaIndexIntroitus*
_index_quaerere (
    SilvaXar*            index,
    s32             status,
    SilvaGSSNodus*  basis,
    b32*            status_visus_out)
{
    i32 i;
    i32 numerus = silva_xar_numerus(index);

    *status_visus_out = FALSUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaIndexIntroitus* introitus =
            (SilvaIndexIntroitus*)silva_xar_obtinere(index, i);

        si (introitus == NIHIL) perge;
        si (introitus->status == status)
        {
            si (introitus->basis == basis)
            {
                redde introitus;
            }
            *status_visus_out = VERUM;
        }
    }
    redde NIHIL;
}

/* Compactio ANTE exhaustionem: nihil adhuc valorem consumpsit, ergo
 * valor GSS simpliciter religatur - identitas non requiritur */
interior vacuum
_compingere (
    SilvaGLR*            glr,
    SilvaGLRPassus*      passus,
    SilvaIndexIntroitus* introitus,
    SilvaValor           valor_novus)
{
    SilvaValor interps;
    SilvaValor involucrum;

    si (!introitus->est_ambiguum)
    {
        interps = silva_valor_lista_nova(passus->silva_piscina_arborum);
        interps = silva_valor_lista_appendere(passus->silva_piscina_arborum,
            interps, introitus->nodus->valor);
    }
    alioquin
    {
        interps = introitus->interpretationes;
    }
    interps = silva_valor_lista_appendere(passus->silva_piscina_arborum,
        interps, valor_novus);
    si (interps.genus != SILVA_VALOR_LISTA)
    {
        fprintf(stderr, "silva_glr: compactio fracta (lista)\n");
        redde;
    }
    involucrum = glr->fabrica(passus->silva_piscina_arborum, interps,
        ZEPHYRUM);
    si (involucrum.genus != SILVA_VALOR_NODUS)
    {
        fprintf(stderr, "silva_glr: fabrica ambigui fracta\n");
        redde;
    }
    introitus->nodus->valor = involucrum;
    introitus->est_ambiguum = VERUM;
    introitus->interpretationes = interps;
    glr->fusiones++;
}

/* TRANSMUTATIO (spec-v2 par 12, simulatio ⑤): brachium serum ad
 * clavem iam exhaustam - valor eius iam per COPIAM VALORIS in
 * constructiones fluxit, sed monstratorem communicant. Contenta per
 * vias probatas aedificantur (creare/ponere/fabrica), deinde UNA
 * copia structurae super allocationem stabilem - identitas manet,
 * nullum praesidium S32 praeteritur. */
interior vacuum
_transmutare (
    SilvaGLR*            glr,
    SilvaGLRPassus*      passus,
    SilvaIndexIntroitus* introitus,
    SilvaValor           valor_novus)
{
    SilvaNodus* sedes = introitus->nodus->valor.datum.nodus;
    SilvaValor  interps;
    SilvaValor  involucrum;

    si (sedes == NIHIL)
    {
        redde;
    }
    si (!introitus->est_ambiguum)
    {
        /* Clon contenta vetera servat: copia structurae seriem loci
         * VETEREM ad clonem transfert (sedes seriem novam ambigui
         * accipiet - series vetus clonis sola fit) */
        SilvaNodus* clon = (SilvaNodus*)silva_piscina_allocare(
            passus->silva_piscina_arborum,
            (memoriae_index)magnitudo(SilvaNodus));

        si (clon == NIHIL)
        {
            redde;
        }
        *clon = *sedes;
        interps = silva_valor_lista_nova(passus->silva_piscina_arborum);
        interps = silva_valor_lista_appendere(passus->silva_piscina_arborum,
            interps, silva_valor_nodus(clon));
    }
    alioquin
    {
        interps = introitus->interpretationes;
    }
    interps = silva_valor_lista_appendere(passus->silva_piscina_arborum,
        interps, valor_novus);
    si (interps.genus != SILVA_VALOR_LISTA)
    {
        fprintf(stderr, "silva_glr: transmutatio fracta (lista)\n");
        redde;
    }
    involucrum = glr->fabrica(passus->silva_piscina_arborum, interps,
        ZEPHYRUM);
    si (involucrum.genus != SILVA_VALOR_NODUS
        || involucrum.datum.nodus == NIHIL)
    {
        fprintf(stderr, "silva_glr: fabrica ambigui fracta\n");
        redde;
    }
    *sedes = *involucrum.datum.nodus;  /* identitas religata */
    introitus->est_ambiguum = VERUM;
    introitus->interpretationes = interps;
    glr->transmutationes++;
}


/* ==================================================
 * Nodi GSS
 * ================================================== */

interior SilvaGSSNodus*
_nodus_creare (
    SilvaGLR*   glr,
    s32         status,
    SilvaValor  valor,
    SilvaToken* lexema)
{
    SilvaGSSNodus* nodus;

    nodus = (SilvaGSSNodus*)silva_piscina_allocare(glr->piscina,
        (memoriae_index)magnitudo(SilvaGSSNodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->status = status;
    nodus->valor = valor;
    nodus->praedecessores = silva_xar_creare(glr->piscina,
        (i32)magnitudo(SilvaGSSNodus*));
    nodus->lexema = lexema;
    glr->nodi_creati++;
    redde nodus;
}

interior vacuum
_praedecessorem_addere (
    SilvaGSSNodus* nodus,
    SilvaGSSNodus* praedecessor)
{
    SilvaGSSNodus** slot;
    i32 i;
    i32 numerus;

    si (nodus == NIHIL || praedecessor == NIHIL)
    {
        redde;
    }
    numerus = silva_xar_numerus(nodus->praedecessores);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGSSNodus** existens =
            (SilvaGSSNodus**)silva_xar_obtinere(nodus->praedecessores, i);

        si (existens != NIHIL && *existens == praedecessor)
        {
            redde;  /* iam adest */
        }
    }
    slot = (SilvaGSSNodus**)silva_xar_addere(nodus->praedecessores);
    si (slot != NIHIL)
    {
        *slot = praedecessor;
    }
}


/* ==================================================
 * Enumeratio viarum
 * ================================================== */

interior vacuum
_vias_recursio (
    SilvaGSSNodus* nodus,
    i32            profunditas,
    i32            gradus,
    SilvaValor*    partiales,
    SilvaXar*           viae)
{
    si (profunditas == gradus)
    {
        SilvaGSSVia* via = (SilvaGSSVia*)silva_xar_addere(viae);

        si (via != NIHIL)
        {
            i32 i;

            per (i = ZEPHYRUM; i < gradus; i++)
            {
                via->valores[i] = partiales[i];
            }
            via->basis = nodus;
        }
        redde;
    }

    si (nodus == NIHIL)
    {
        redde;
    }

    {
        i32 idx = gradus - I - profunditas;
        i32 numerus_praed;
        i32 pi;

        si (idx >= (i32)SILVA_GLR_DEXTRUM_MAXIMUM)
        {
            redde;
        }
        partiales[idx] = nodus->valor;

        numerus_praed = silva_xar_numerus(nodus->praedecessores);
        per (pi = ZEPHYRUM; pi < numerus_praed; pi++)
        {
            SilvaGSSNodus** praed =
                (SilvaGSSNodus**)silva_xar_obtinere(nodus->praedecessores, pi);

            si (praed != NIHIL && *praed != NIHIL)
            {
                _vias_recursio(*praed, profunditas + I, gradus,
                    partiales, viae);
            }
        }
    }
}

interior SilvaXar*
_vias_enumerare (
    SilvaGLR*      glr,
    SilvaGSSNodus* culmen,
    i32            gradus)
{
    SilvaXar*       viae;
    SilvaValor partiales[SILVA_GLR_DEXTRUM_MAXIMUM];

    viae = silva_xar_creare(glr->piscina, (i32)magnitudo(SilvaGSSVia));
    si (viae == NIHIL)
    {
        redde NIHIL;
    }

    si (gradus == ZEPHYRUM)
    {
        SilvaGSSVia* via = (SilvaGSSVia*)silva_xar_addere(viae);

        si (via != NIHIL)
        {
            via->basis = culmen;
        }
        redde viae;
    }

    _vias_recursio(culmen, ZEPHYRUM, gradus, partiales, viae);
    redde viae;
}


/* ==================================================
 * Quaestio tabularum (laminae per statum)
 * ================================================== */

/* Omnes actiones pro (status, terminale): monstratorem ad primam et
 * numerum reddit (ordinatae adiacentes - in validatione impositum). */
interior i32
_actiones_invenire (
    constans SilvaTabulaCocta* tabula,
    s32                        status,
    s32                        terminale,
    constans SilvaTabActio**   prima_out)
{
    constans SilvaTabStatus* lamina;
    i32 i;
    i32 numerus = ZEPHYRUM;

    *prima_out = NIHIL;
    si (status < ZEPHYRUM || status >= (s32)tabula->numerus_statuum)
    {
        redde ZEPHYRUM;
    }
    lamina = &tabula->status[status];

    per (i = ZEPHYRUM; i < lamina->actiones_numerus; i++)
    {
        constans SilvaTabActio* actio =
            &tabula->actiones[lamina->actiones_offset + i];

        si (actio->terminalis == terminale)
        {
            si (*prima_out == NIHIL)
            {
                *prima_out = actio;
            }
            numerus++;
        }
        alioquin si (*prima_out != NIHIL)
        {
            frange;  /* ordinatae: praeter cursum */
        }
    }
    redde numerus;
}

interior s32
_goto_quaerere (
    constans SilvaTabulaCocta* tabula,
    s32                        status,
    s32                        non_terminalis)
{
    constans SilvaTabStatus* lamina;
    i32 i;

    si (status < ZEPHYRUM || status >= (s32)tabula->numerus_statuum)
    {
        redde -I;
    }
    lamina = &tabula->status[status];

    per (i = ZEPHYRUM; i < lamina->goto_numerus; i++)
    {
        constans SilvaTabGoto* g =
            &tabula->goto_introitus[lamina->goto_offset + i];

        si (g->non_terminalis == non_terminalis)
        {
            redde g->status_novus;
        }
    }
    redde -I;
}

s32
silva_glr_terminale_ex_genere (
    constans SilvaTabulaCocta* tabula,
    SilvaLexemaGenus           genus)
{
    i32 i;

    per (i = ZEPHYRUM; i < tabula->numerus_terminalium; i++)
    {
        si (tabula->symbola[i].genus_lexematis == genus)
        {
            redde (s32)i;
        }
    }
    redde SILVA_GLR_TERMINALE_IGNOTUM;
}


/* ==================================================
 * Validatio tabulae
 * ================================================== */

b32
silva_glr_tabulam_validare (
    SilvaPiscina*                   piscina,
    constans SilvaTabulaCocta* tabula)
{
    i32 st;
    i32 i;

    si (piscina == NIHIL || tabula == NIHIL
        || tabula->numerus_statuum == ZEPHYRUM
        || tabula->symbola == NIHIL || tabula->productiones == NIHIL
        || tabula->status == NIHIL || tabula->actiones == NIHIL
        || tabula->goto_introitus == NIHIL)
    {
        fprintf(stderr, "silva_glr: tabula vacua aut manca\n");
        redde FALSUM;
    }

    /* Productiones: longitudo intra terminum, sinistrum non-terminale */
    per (i = ZEPHYRUM; i < tabula->numerus_productionum; i++)
    {
        constans SilvaTabProductio* p = &tabula->productiones[i];

        si (p->longitudo > (i32)SILVA_GLR_DEXTRUM_MAXIMUM)
        {
            fprintf(stderr, "silva_glr: productio %d longior quam %d\n",
                (int)i, (int)SILVA_GLR_DEXTRUM_MAXIMUM);
            redde FALSUM;
        }
        si (p->sinistrum < (s32)tabula->numerus_terminalium
            || p->sinistrum >= (s32)tabula->numerus_symbolorum)
        {
            fprintf(stderr,
                "silva_glr: productio %d sinistrum non non-terminale\n",
                (int)i);
            redde FALSUM;
        }
    }

    /* Laminae statuum: fines, introitus, ordinatio, goto pro LHS */
    per (st = ZEPHYRUM; st < tabula->numerus_statuum; st++)
    {
        constans SilvaTabStatus* lamina = &tabula->status[st];
        s32 terminale_prius = -II;

        si (lamina->actiones_offset + lamina->actiones_numerus
                > tabula->numerus_actionum
            || lamina->goto_offset + lamina->goto_numerus
                > tabula->numerus_goto)
        {
            fprintf(stderr, "silva_glr: lamina status %d extra fines\n",
                (int)st);
            redde FALSUM;
        }

        per (i = ZEPHYRUM; i < lamina->actiones_numerus; i++)
        {
            constans SilvaTabActio* actio =
                &tabula->actiones[lamina->actiones_offset + i];

            si (actio->terminalis < -I
                || actio->terminalis >= (s32)tabula->numerus_terminalium)
            {
                fprintf(stderr,
                    "silva_glr: status %d terminale %d extra fines\n",
                    (int)st, (int)actio->terminalis);
                redde FALSUM;
            }
            si (actio->terminalis < terminale_prius)
            {
                fprintf(stderr,
                    "silva_glr: status %d actiones non ordinatae\n",
                    (int)st);
                redde FALSUM;
            }
            terminale_prius = actio->terminalis;

            commutatio (actio->actio)
            {
            casus SILVA_TAB_ACTIO_TRANSPONERE:
                si (actio->valor < ZEPHYRUM
                    || actio->valor >= (s32)tabula->numerus_statuum)
                {
                    fprintf(stderr,
                        "silva_glr: status %d translatio extra fines\n",
                        (int)st);
                    redde FALSUM;
                }
                frange;
            casus SILVA_TAB_ACTIO_REDUCERE:
                si (actio->valor < ZEPHYRUM
                    || actio->valor >= (s32)tabula->numerus_productionum)
                {
                    fprintf(stderr,
                        "silva_glr: status %d reductio extra fines\n",
                        (int)st);
                    redde FALSUM;
                }
                /* Goto absens = circuitus infinitus, non defectus
                 * mundus (arbor2 worklog) - LHS goto alicubi habere
                 * debet. */
                {
                    s32 lhs = tabula->productiones[actio->valor].sinistrum;
                    b32 inventum = FALSUM;
                    i32 gi;

                    per (gi = ZEPHYRUM; gi < tabula->numerus_goto; gi++)
                    {
                        si (tabula->goto_introitus[gi].non_terminalis
                            == lhs)
                        {
                            inventum = VERUM;
                            frange;
                        }
                    }
                    si (!inventum)
                    {
                        fprintf(stderr,
                            "silva_glr: symbolum %d sine goto ullo\n",
                            (int)lhs);
                        redde FALSUM;
                    }
                }
                frange;
            casus SILVA_TAB_ACTIO_ACCIPERE:
            casus SILVA_TAB_ACTIO_RECUPERARE:
            casus SILVA_TAB_ACTIO_ERRARE:
                frange;
            ordinarius:
                fprintf(stderr,
                    "silva_glr: status %d actio ignota %d\n",
                    (int)st, (int)actio->actio);
                redde FALSUM;
            }
        }

        per (i = ZEPHYRUM; i < lamina->goto_numerus; i++)
        {
            constans SilvaTabGoto* g =
                &tabula->goto_introitus[lamina->goto_offset + i];

            si (g->non_terminalis < (s32)tabula->numerus_terminalium
                || g->non_terminalis >= (s32)tabula->numerus_symbolorum
                || g->status_novus < ZEPHYRUM
                || g->status_novus >= (s32)tabula->numerus_statuum)
            {
                fprintf(stderr,
                    "silva_glr: status %d goto extra fines\n", (int)st);
                redde FALSUM;
            }
        }
    }

    /* Unicitas symboli accessus (spec-v2 par 12.2, simulatio ⑤ C3):
     * quisque status uno solo symbolo intratur - proprietas LR quam
     * index reducendorum praesumit (nodi translatione creati cum
     * nodis reductione creatis in statu collidere non possunt).
     * Assertio ex tabulis, non assumptio ex theoria. */
    {
        s32* accessus;
        i32  st2;

        accessus = (s32*)silva_piscina_allocare(piscina,
            (memoriae_index)((memoriae_index)magnitudo(s32)
                * (memoriae_index)tabula->numerus_statuum));
        si (accessus == NIHIL)
        {
            redde FALSUM;
        }
        per (st2 = ZEPHYRUM; st2 < tabula->numerus_statuum; st2++)
        {
            accessus[st2] = -I;  /* nondum intratus */
        }
        per (st2 = ZEPHYRUM; st2 < tabula->numerus_statuum; st2++)
        {
            constans SilvaTabStatus* lamina = &tabula->status[st2];

            per (i = ZEPHYRUM; i < lamina->actiones_numerus; i++)
            {
                constans SilvaTabActio* actio =
                    &tabula->actiones[lamina->actiones_offset + i];

                si (actio->actio == (s32)SILVA_TAB_ACTIO_TRANSPONERE)
                {
                    si (accessus[actio->valor] != -I
                        && accessus[actio->valor] != actio->terminalis)
                    {
                        fprintf(stderr, "silva_glr: status %d symbolis "
                            "pluribus intratur (%d, %d)\n",
                            (int)actio->valor,
                            (int)accessus[actio->valor],
                            (int)actio->terminalis);
                        redde FALSUM;
                    }
                    accessus[actio->valor] = actio->terminalis;
                }
            }
            per (i = ZEPHYRUM; i < lamina->goto_numerus; i++)
            {
                constans SilvaTabGoto* g =
                    &tabula->goto_introitus[lamina->goto_offset + i];

                si (accessus[g->status_novus] != -I
                    && accessus[g->status_novus] != g->non_terminalis)
                {
                    fprintf(stderr, "silva_glr: status %d symbolis "
                        "pluribus intratur (%d, %d)\n",
                        (int)g->status_novus,
                        (int)accessus[g->status_novus],
                        (int)g->non_terminalis);
                    redde FALSUM;
                }
                accessus[g->status_novus] = g->non_terminalis;
            }
        }
    }

    redde VERUM;
}


/* ==================================================
 * Processio nodi unius (dispatch actionum)
 * ================================================== */

interior vacuum
_nodum_processare (
    SilvaGLR*       glr,
    SilvaGSSNodus*  nodus,
    SilvaGLRPassus* passus)
{
    constans SilvaTabActio* actio;
    i32 numerus_actionum;
    i32 ai;

    numerus_actionum = _actiones_invenire(glr->tabula, nodus->status,
        passus->terminale, &actio);

    per (ai = ZEPHYRUM; ai < numerus_actionum; ai++, actio++)
    {
        commutatio (actio->actio)
        {
        casus SILVA_TAB_ACTIO_TRANSPONERE:
        {
            SilvaGSSNodus*  novus;
            SilvaGSSNodus** slot;

            si (passus->lexema == NIHIL)
            {
                frange;  /* translatio sine lexemate impossibilis */
            }
            novus = _nodus_creare(glr, actio->valor,
                silva_valor_token(passus->lexema), passus->lexema);
            si (novus == NIHIL)
            {
                frange;
            }
            _praedecessorem_addere(novus, nodus);
            slot = (SilvaGSSNodus**)silva_xar_addere(passus->frons_nova);
            si (slot != NIHIL)
            {
                *slot = novus;
            }
            frange;
        }

        casus SILVA_TAB_ACTIO_REDUCERE:
        {
            constans SilvaTabProductio* prod;
            SilvaXar* viae;
            i32  numerus_viarum;
            i32  vi;

            prod = &glr->tabula->productiones[actio->valor];
            viae = _vias_enumerare(glr, nodus, prod->longitudo);
            numerus_viarum = silva_xar_numerus(viae);

            per (vi = ZEPHYRUM; vi < numerus_viarum; vi++)
            {
                SilvaGSSVia*         via;
                SilvaValor           valor_novus;
                s32                  goto_status;
                SilvaIndexIntroitus* introitus;
                b32                  status_visus;

                via = (SilvaGSSVia*)silva_xar_obtinere(viae, vi);
                si (via == NIHIL || via->basis == NIHIL)
                {
                    perge;
                }

                valor_novus = glr->constructor(passus->silva_piscina_arborum,
                    actio->valor, via->valores);

                goto_status = _goto_quaerere(glr->tabula,
                    via->basis->status, prod->sinistrum);
                si (goto_status < ZEPHYRUM)
                {
                    /* Validatio praeterita - corruptio tabulae */
                    fprintf(stderr,
                        "silva_glr: goto absens (status %d, symbolum %d)"
                        " - via moritur\n",
                        (int)via->basis->status, (int)prod->sinistrum);
                    perge;
                }
                glr->reductiones_factae++;

                /* Dispatch compactionis (spec-v2 par 12): clavis
                 * (status, basis) reconventiones classificat */
                introitus = _index_quaerere(passus->index, goto_status,
                    via->basis, &status_visus);

                si (introitus == NIHIL)
                {
                    /* Clavis nova - nodus recens registratur */
                    SilvaGSSNodus*        novus;
                    SilvaIndexIntroitus*  recens;
                    SilvaIndexIntroitus** slot;

                    si (status_visus)
                    {
                        /* margo novus ad statum visum (classis
                         * Farshi): brachium separatum, numeratum */
                        glr->eventa_marginis_novi++;
                    }
                    novus = _nodus_creare(glr, goto_status, valor_novus,
                        nodus->lexema);
                    si (novus == NIHIL)
                    {
                        perge;
                    }
                    _praedecessorem_addere(novus, via->basis);

                    recens = (SilvaIndexIntroitus*)silva_xar_addere(
                        passus->index);
                    si (recens == NIHIL)
                    {
                        perge;
                    }
                    recens->status = goto_status;
                    recens->basis = via->basis;
                    recens->nodus = novus;
                    recens->exhaustus = FALSUM;
                    recens->est_ambiguum = FALSUM;
                    recens->interpretationes = silva_valor_nihil();

                    slot = (SilvaIndexIntroitus**)silva_xar_addere(
                        passus->reducenda);
                    si (slot != NIHIL)
                    {
                        *slot = recens;
                    }
                }
                alioquin si (_valores_aequales(introitus->nodus->valor,
                    valor_novus))
                {
                    /* Derivatio duplex - abicitur (superstes solus
                     * consistens manet) */
                    glr->fusiones++;
                }
                alioquin si (!introitus->exhaustus)
                {
                    _compingere(glr, passus, introitus, valor_novus);
                }
                alioquin si (introitus->nodus->valor.genus
                    == SILVA_VALOR_NODUS)
                {
                    _transmutare(glr, passus, introitus, valor_novus);
                }
                alioquin
                {
                    /* Exhaustum, valor non-NODUS (lista/lexema):
                     * identitas religanda non existit - brachium
                     * separatum, scala gratiosa. Introitus additus
                     * (exhauriendus) sed a clavi priore obumbratus. */
                    SilvaGSSNodus*        novus;
                    SilvaIndexIntroitus*  recens;
                    SilvaIndexIntroitus** slot;

                    glr->transmutationes_negatae++;
                    novus = _nodus_creare(glr, goto_status, valor_novus,
                        nodus->lexema);
                    si (novus == NIHIL)
                    {
                        perge;
                    }
                    _praedecessorem_addere(novus, via->basis);
                    recens = (SilvaIndexIntroitus*)silva_xar_addere(
                        passus->index);
                    si (recens == NIHIL)
                    {
                        perge;
                    }
                    recens->status = goto_status;
                    recens->basis = via->basis;
                    recens->nodus = novus;
                    recens->exhaustus = FALSUM;
                    recens->est_ambiguum = FALSUM;
                    recens->interpretationes = silva_valor_nihil();
                    slot = (SilvaIndexIntroitus**)silva_xar_addere(
                        passus->reducenda);
                    si (slot != NIHIL)
                    {
                        *slot = recens;
                    }
                }
            }
            frange;
        }

        casus SILVA_TAB_ACTIO_ACCIPERE:
        {
            SilvaGSSNodus** slot =
                (SilvaGSSNodus**)silva_xar_addere(passus->acceptati);

            si (slot != NIHIL)
            {
                *slot = nodus;
            }
            frange;
        }

        casus SILVA_TAB_ACTIO_RECUPERARE:
            /* Recuperatio pretio-ducta post-substratum (par 8.2) -
             * clare deficere donec impleta */
            fprintf(stderr,
                "silva_glr: actio RECUPERARE nondum impleta\n");
            frange;

        ordinarius:
            frange;
        }
    }
}


/* ==================================================
 * API
 * ================================================== */

/* Fusio frontis translationis: nodi eiusdem status uniuntur per
 * unionem praedecessorum. Valores comparandi NON sunt: nodi frontis
 * translatione creantur et IDEM lexema portant (unicitas symboli
 * accessus in validatione probata). Ordo servatur (compactio in
 * loco). */
interior vacuum
_frontem_compingere (SilvaGLR* glr, SilvaXar* frons)
{
    i32 i;
    i32 j;
    i32 numerus = silva_xar_numerus(frons);

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGSSNodus** slot_a =
            (SilvaGSSNodus**)silva_xar_obtinere(frons, i);

        si (slot_a == NIHIL || *slot_a == NIHIL) perge;
        per (j = i + I; j < numerus; j++)
        {
            SilvaGSSNodus** slot_b =
                (SilvaGSSNodus**)silva_xar_obtinere(frons, j);

            si (slot_b == NIHIL || *slot_b == NIHIL) perge;
            si ((*slot_a)->status == (*slot_b)->status)
            {
                i32 pi;

                per (pi = ZEPHYRUM;
                     pi < silva_xar_numerus((*slot_b)->praedecessores); pi++)
                {
                    SilvaGSSNodus** praed = (SilvaGSSNodus**)silva_xar_obtinere(
                        (*slot_b)->praedecessores, pi);

                    si (praed != NIHIL && *praed != NIHIL)
                    {
                        _praedecessorem_addere(*slot_a, *praed);
                    }
                }
                *slot_b = NIHIL;
                glr->fusiones++;
            }
        }
    }

    /* Compactio in loco - ordo servatur */
    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaGSSNodus** slot =
            (SilvaGSSNodus**)silva_xar_obtinere(frons, i);

        si (slot != NIHIL && *slot != NIHIL)
        {
            si (i != j)
            {
                SilvaGSSNodus** dest =
                    (SilvaGSSNodus**)silva_xar_obtinere(frons, j);

                si (dest != NIHIL)
                {
                    *dest = *slot;
                }
            }
            j++;
        }
    }
    silva_xar_truncare(frons, j);
}

/* Numeratores in zephyrum */
interior vacuum
_statisticas_purgare (SilvaGLR* glr)
{
    glr->frons_maxima = ZEPHYRUM;
    glr->nodi_creati = ZEPHYRUM;
    glr->reductiones_factae = ZEPHYRUM;
    glr->fusiones = ZEPHYRUM;
    glr->transmutationes = ZEPHYRUM;
    glr->transmutationes_negatae = ZEPHYRUM;
    glr->eventa_marginis_novi = ZEPHYRUM;
}

/* Numeratores in fructum (productum queribile, par 12.2) */
interior vacuum
_statisticas_copiare (constans SilvaGLR* glr, SilvaGLRFructus* fructus)
{
    fructus->frons_maxima = glr->frons_maxima;
    fructus->fusiones = glr->fusiones;
    fructus->transmutationes = glr->transmutationes;
    fructus->transmutationes_negatae = glr->transmutationes_negatae;
    fructus->eventa_marginis_novi = glr->eventa_marginis_novi;
}

SilvaGLR*
silva_glr_creare (
    SilvaPiscina*                   piscina,
    constans SilvaTabulaCocta* tabula,
    SilvaGLRConstructor        constructor,
    SilvaGLRFabricaAmbigui     fabrica)
{
    SilvaGLR* glr;

    si (piscina == NIHIL || constructor == NIHIL || fabrica == NIHIL)
    {
        redde NIHIL;
    }
    si (!silva_glr_tabulam_validare(piscina, tabula))
    {
        redde NIHIL;
    }

    glr = (SilvaGLR*)silva_piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaGLR));
    si (glr == NIHIL)
    {
        redde NIHIL;
    }
    glr->tabula = tabula;
    glr->constructor = constructor;
    glr->fabrica = fabrica;
    glr->piscina = piscina;
    glr->limen_frontis = SILVA_GLR_LIMEN_FRONTIS_DEFALTUM;
    glr->pergere = NIHIL;
    glr->pergere_datum = NIHIL;
    glr->passus_pergendi = SILVA_GLR_PASSUS_PERGENDI_DEFALTUM;
    _statisticas_purgare(glr);
    redde glr;
}

SilvaGLRFructus
silva_glr_parsare (
    SilvaGLR*     glr,
    constans SilvaXar* lexemata,
    SilvaPiscina*      silva_piscina_arborum)
{
    SilvaGLRFructus fructus;
    SilvaXar* frons;
    SilvaXar* acceptati;
    i32  numerus_lexematum;
    i32  positio;

    fructus.successus = FALSUM;
    fructus.valor = silva_valor_nihil();
    fructus.lexema_finis = NIHIL;
    fructus.numerus_acceptorum = ZEPHYRUM;
    fructus.frons_maxima = ZEPHYRUM;
    fructus.fusiones = ZEPHYRUM;
    fructus.transmutationes = ZEPHYRUM;
    fructus.transmutationes_negatae = ZEPHYRUM;
    fructus.eventa_marginis_novi = ZEPHYRUM;
    fructus.est_error = FALSUM;
    fructus.positio = ZEPHYRUM;
    fructus.terminalis = ZEPHYRUM;
    fructus.status = ZEPHYRUM;
    fructus.est_ultra_limen = FALSUM;
    fructus.est_intermissus = FALSUM;

    si (glr == NIHIL || lexemata == NIHIL || silva_piscina_arborum == NIHIL)
    {
        fructus.est_error = VERUM;
        redde fructus;
    }

    _statisticas_purgare(glr);

    numerus_lexematum = silva_xar_numerus(lexemata);
    positio = ZEPHYRUM;

    /* Frons initialis: nodus unus in statu 0 */
    frons = silva_xar_creare(glr->piscina, (i32)magnitudo(SilvaGSSNodus*));
    {
        SilvaGSSNodus*  initialis;
        SilvaGSSNodus** slot;

        initialis = _nodus_creare(glr, ZEPHYRUM, silva_valor_nihil(),
            NIHIL);
        slot = (SilvaGSSNodus**)silva_xar_addere(frons);
        si (slot != NIHIL)
        {
            *slot = initialis;
        }
    }
    acceptati = silva_xar_creare(glr->piscina, (i32)magnitudo(SilvaGSSNodus*));

    dum (VERUM)
    {
        SilvaGLRPassus passus;
        i32 fi;
        i32 cursor;

        /* Intermissio (Phase 7): interrogatio determinata per passum
         * lexematum; FALSUM = fractura munda (gubernator recuperat) */
        si (glr->pergere != NIHIL
            && (glr->passus_pergendi <= I
                || (positio % glr->passus_pergendi) == ZEPHYRUM)
            && !glr->pergere(glr->pergere_datum))
        {
            fructus.est_error = VERUM;
            fructus.est_intermissus = VERUM;
            fructus.positio = (s32)positio;
            _statisticas_copiare(glr, &fructus);
            redde fructus;
        }

        /* Prospectus: lexema currens aut $ ultra fluxum */
        passus.terminale = SILVA_GLR_PROSPECTUS_FINIS;
        passus.lexema = NIHIL;
        si (positio < numerus_lexematum)
        {
            SilvaToken** ref =
                (SilvaToken**)silva_xar_obtinere(lexemata, positio);

            passus.lexema = (ref != NIHIL) ? *ref : NIHIL;
            si (passus.lexema != NIHIL)
            {
                passus.terminale = silva_glr_terminale_ex_genere(
                    glr->tabula, passus.lexema->genus);
                si (passus.terminale == SILVA_GLR_TERMINALE_IGNOTUM)
                {
                    fructus.est_error = VERUM;
                    fructus.positio = (s32)positio;
                    fructus.terminalis = SILVA_GLR_TERMINALE_IGNOTUM;
                    _statisticas_copiare(glr, &fructus);
                    redde fructus;
                }
            }
        }

        passus.reducenda = silva_xar_creare(glr->piscina,
            (i32)magnitudo(SilvaGSSNodus*));
        passus.frons_nova = silva_xar_creare(glr->piscina,
            (i32)magnitudo(SilvaGSSNodus*));
        passus.index = silva_xar_creare(glr->piscina,
            (i32)magnitudo(SilvaIndexIntroitus));
        passus.acceptati = acceptati;
        passus.silva_piscina_arborum = silva_piscina_arborum;
        silva_xar_vacare(acceptati);

        /* Passus 1: frons; passus 2: exhaustio reducendorum (FIFO -
         * crescit dum exhauritur; introitus indicis per valorem in
         * Xar sedent, monstratores stabiles trans additiones).
         * exhaustus POST processionem signatur - fenestra
         * transmutationis his signis definitur. */
        per (fi = ZEPHYRUM; fi < silva_xar_numerus(frons); fi++)
        {
            SilvaGSSNodus** ref =
                (SilvaGSSNodus**)silva_xar_obtinere(frons, fi);

            si (ref != NIHIL && *ref != NIHIL)
            {
                _nodum_processare(glr, *ref, &passus);
            }
        }
        cursor = ZEPHYRUM;
        dum (cursor < silva_xar_numerus(passus.reducenda))
        {
            SilvaIndexIntroitus** ref = (SilvaIndexIntroitus**)
                silva_xar_obtinere(passus.reducenda, cursor);

            cursor++;
            si (ref != NIHIL && *ref != NIHIL && (*ref)->nodus != NIHIL)
            {
                _nodum_processare(glr, (*ref)->nodus, &passus);
                (*ref)->exhaustus = VERUM;
            }
        }

        /* Acceptatio: scala reconciliationis (spec-v2 par 12) -
         * valores praedecessorum omnium colliguntur; aequales
         * deduplicantur; radices vere diversae in AMBIGUUS
         * involvuntur (ambiguitas radicalis - scala gratiosa) */
        si (silva_xar_numerus(acceptati) > ZEPHYRUM)
        {
            SilvaXar* radices = silva_xar_creare(glr->piscina,
                (i32)magnitudo(SilvaValor));
            i32  vias_acceptae = ZEPHYRUM;
            i32  ai;

            per (ai = ZEPHYRUM; ai < silva_xar_numerus(acceptati); ai++)
            {
                SilvaGSSNodus* acceptus =
                    *(SilvaGSSNodus**)silva_xar_obtinere(acceptati, ai);
                i32 pi;

                si (acceptus == NIHIL) perge;
                si (ai == ZEPHYRUM
                    && acceptus->valor.genus == SILVA_VALOR_TOKEN)
                {
                    /* Lexema EOF translatum - trivia Phase 5 emittet */
                    fructus.lexema_finis = acceptus->valor.datum.token;
                }
                per (pi = ZEPHYRUM;
                     pi < silva_xar_numerus(acceptus->praedecessores); pi++)
                {
                    SilvaGSSNodus** praed = (SilvaGSSNodus**)silva_xar_obtinere(
                        acceptus->praedecessores, pi);
                    b32 novum = VERUM;
                    i32 ri;

                    si (praed == NIHIL || *praed == NIHIL) perge;
                    vias_acceptae++;
                    per (ri = ZEPHYRUM; ri < silva_xar_numerus(radices); ri++)
                    {
                        SilvaValor* r =
                            (SilvaValor*)silva_xar_obtinere(radices, ri);

                        si (r != NIHIL
                            && _valores_aequales(*r, (*praed)->valor))
                        {
                            novum = FALSUM;
                            frange;
                        }
                    }
                    si (novum)
                    {
                        SilvaValor* slot =
                            (SilvaValor*)silva_xar_addere(radices);

                        si (slot != NIHIL)
                        {
                            *slot = (*praed)->valor;
                        }
                    }
                }
            }

            fructus.successus = VERUM;
            fructus.numerus_acceptorum = vias_acceptae;
            si (silva_xar_numerus(radices) == I)
            {
                fructus.valor = *(SilvaValor*)silva_xar_obtinere(radices,
                    ZEPHYRUM);
            }
            alioquin si (silva_xar_numerus(radices) > I)
            {
                /* Radices diversae: involucrum AMBIGUUS radicale */
                SilvaValor interps =
                    silva_valor_lista_nova(silva_piscina_arborum);
                i32 ri;

                per (ri = ZEPHYRUM; ri < silva_xar_numerus(radices); ri++)
                {
                    SilvaValor* r =
                        (SilvaValor*)silva_xar_obtinere(radices, ri);

                    si (r != NIHIL)
                    {
                        interps = silva_valor_lista_appendere(
                            silva_piscina_arborum, interps, *r);
                    }
                }
                fructus.valor = glr->fabrica(silva_piscina_arborum, interps,
                    ZEPHYRUM);
            }
            _statisticas_copiare(glr, &fructus);
            redde fructus;
        }

        /* Fusio frontis: nodi eiusdem status uniuntur (valores
         * translationis = IDEM lexema, ergo unio praedecessorum
         * sufficit - fusio valorum in exhaustione sola accidit) */
        _frontem_compingere(glr, passus.frons_nova);

        /* Frons vacua sine acceptatione = error parsurae */
        si (silva_xar_numerus(passus.frons_nova) == ZEPHYRUM)
        {
            fructus.est_error = VERUM;
            fructus.positio = (s32)positio;
            fructus.terminalis = passus.terminale;
            si (silva_xar_numerus(frons) > ZEPHYRUM)
            {
                SilvaGSSNodus** ref =
                    (SilvaGSSNodus**)silva_xar_obtinere(frons, ZEPHYRUM);

                si (ref != NIHIL && *ref != NIHIL)
                {
                    fructus.status = (*ref)->status;
                }
            }
            _statisticas_copiare(glr, &fructus);
            redde fructus;
        }

        si (silva_xar_numerus(passus.frons_nova) > glr->frons_maxima)
        {
            glr->frons_maxima = silva_xar_numerus(passus.frons_nova);
        }

        /* Limen frontis (Phase 7): fractura munda pro fluxu infesto -
         * gubernator segmentum in nodum ERROR vertit (totalitas) */
        si (glr->limen_frontis > ZEPHYRUM
            && silva_xar_numerus(passus.frons_nova) > glr->limen_frontis)
        {
            fructus.est_error = VERUM;
            fructus.est_ultra_limen = VERUM;
            fructus.positio = (s32)positio;
            fructus.terminalis = passus.terminale;
            _statisticas_copiare(glr, &fructus);
            redde fructus;
        }

        frons = passus.frons_nova;
        positio++;
    }
}

/* ================= ex silva/fontes/silva_commissio.c ================= */

/* ==================================================
 * Oraculum
 * ================================================== */

SilvaOraculum*
silva_oraculum_creare (SilvaPiscina* piscina)
{
    SilvaOraculum* oraculum;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    oraculum = (SilvaOraculum*)silva_piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaOraculum));
    si (oraculum == NIHIL)
    {
        redde NIHIL;
    }
    oraculum->piscina = piscina;
    oraculum->typi = silva_tabula_dispersa_creare_chorda(piscina, XVI);
    si (oraculum->typi == NIHIL)
    {
        redde NIHIL;
    }
    redde oraculum;
}

b32
silva_oraculum_typum_addere (SilvaOraculum* oraculum, SilvaChorda titulus)
{
    SilvaChorda copia;

    si (oraculum == NIHIL)
    {
        redde FALSUM;
    }
    /* Copia in piscinam oraculi: valores lexematum prospectus in
     * fontem sunt - fons post oraculum mori potest */
    copia = silva_chorda_transcribere(titulus, oraculum->piscina);
    redde silva_tabula_dispersa_inserere(oraculum->typi, copia,
        (vacuum*)oraculum);
}

b32
silva_oraculum_typum_addere_literis (
    SilvaOraculum*      oraculum,
    constans character* titulus)
{
    si (oraculum == NIHIL || titulus == NIHIL)
    {
        redde FALSUM;
    }
    redde silva_oraculum_typum_addere(oraculum,
        silva_chorda_ex_literis(titulus, oraculum->piscina));
}

b32
silva_oraculum_typum_novit (
    constans SilvaOraculum* oraculum,
    SilvaChorda                  titulus)
{
    si (oraculum == NIHIL)
    {
        redde FALSUM;
    }
    redde silva_tabula_dispersa_continet(oraculum->typi, titulus);
}


/* ==================================================
 * Ambulatio
 * ================================================== */

nomen structura {
    SilvaPiscina*                piscina;
    SilvaCommissio*         commissio;
    constans SilvaOraculum* oraculum;
    SilvaResolutor          resolutor;
    vacuum*                 datum_resolutoris;
} SilvaAmbulatio;

interior SilvaValor _valorem_committere (SilvaAmbulatio* ambulatio,
    SilvaValor valor, SilvaNodus* pater);

/* Normalizatio prospectus (A½): mensura exacta post commissionem -
 * consumptores xar_numerus tuto legere possunt */
interior SilvaValor
_listam_normalizare (SilvaAmbulatio* ambulatio, SilvaValor lista)
{
    SilvaXar* exactum;
    i32  i;

    si (lista.genus != SILVA_VALOR_LISTA || lista.datum.lista.xar == NIHIL
        || silva_xar_numerus(lista.datum.lista.xar) == lista.datum.lista.mensura)
    {
        redde lista;
    }
    exactum = silva_xar_creare(ambulatio->piscina,
        (i32)magnitudo(SilvaValor));
    si (exactum == NIHIL)
    {
        redde lista;
    }
    per (i = ZEPHYRUM; i < lista.datum.lista.mensura; i++)
    {
        SilvaValor* fons = silva_valor_lista_obtinere(lista, i);
        SilvaValor* dest = (SilvaValor*)silva_xar_addere(exactum);

        si (fons != NIHIL && dest != NIHIL)
        {
            *dest = *fons;
        }
    }
    redde silva_valor_lista(exactum);
}

/* Nodus ordinarius: loci per ambulationem, filii pater accipiunt */
interior vacuum
_nodum_committere (SilvaAmbulatio* ambulatio, SilvaNodus* nodus)
{
    i32 i;

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        nodus->loci[i] = _valorem_committere(ambulatio, nodus->loci[i],
            nodus);
    }
}

/* Nodus AMBIGUUS: collapsus si oraculum novit, alioquin superstes
 * cum spina canonica */
interior vacuum
_ambiguum_committere (SilvaAmbulatio* ambulatio, SilvaNodus* nodus)
{
    SilvaCommissio* commissio = ambulatio->commissio;
    SilvaValor      interps;
    s32             canonica;

    /* Resolutio: oraculum iam novit? -> COLLAPSUS (victor super
     * allocationem copiatur - identitas manet, involucrum abiit) */
    si (ambulatio->oraculum != NIHIL && ambulatio->resolutor != NIHIL)
    {
        SilvaResolutioResponsum responsum;

        responsum.victor = -I;
        responsum.discriminans = NIHIL;
        ambulatio->resolutor(nodus, ambulatio->oraculum,
            ambulatio->datum_resolutoris, &responsum);
        si (responsum.victor >= ZEPHYRUM)
        {
            SilvaValor* victor_valor = silva_valor_lista_obtinere(
                nodus->loci[commissio->locus_interpretationum],
                (i32)responsum.victor);

            si (victor_valor != NIHIL
                && victor_valor->genus == SILVA_VALOR_NODUS
                && victor_valor->datum.nodus != NIHIL)
            {
                SilvaNodus* pater_servatus = nodus->pater;
                SilvaResolutioEventum* eventum;

                *nodus = *victor_valor->datum.nodus;  /* collapsus */
                nodus->pater = pater_servatus;

                eventum = (SilvaResolutioEventum*)silva_xar_addere(
                    commissio->resolutiones);
                si (eventum != NIHIL)
                {
                    eventum->genus = SILVA_RESOLUTIO_COLLAPSUS;
                    eventum->sedes = nodus;
                    eventum->victor = responsum.victor;
                    eventum->genus_victoris = nodus->genus;
                    eventum->discriminans = responsum.discriminans;
                }
                _nodum_committere(ambulatio, nodus);  /* nunc ordinarius */
                redde;
            }
            fprintf(stderr, "silva_commissio: victor non-NODUS - "
                "involucrum manet\n");
        }
    }

    /* Superstes: registratur; interpretationes normalizatae et
     * ambulatae - internis omnium pater intra subarborem, radicibus
     * spina sola: canonica pater=ambiguum, aliae NIHIL. Canonica
     * ULTIMA ambulatur - in subarbore communi parens spinae vincit. */
    {
        SilvaNodus** slot = (SilvaNodus**)silva_xar_addere(commissio->ambigui);

        si (slot != NIHIL)
        {
            *slot = nodus;
        }
    }

    nodus->loci[commissio->locus_interpretationum] = _listam_normalizare(
        ambulatio, nodus->loci[commissio->locus_interpretationum]);
    interps = nodus->loci[commissio->locus_interpretationum];
    canonica = nodus->loci[commissio->locus_canonicae].datum.index;

    {
        i32 numerus = silva_valor_lista_numerus(interps);
        i32 i;

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaValor* elem;

            si (i == (i32)canonica) perge;  /* canonica ultima */
            elem = silva_valor_lista_obtinere(interps, i);
            si (elem != NIHIL)
            {
                *elem = _valorem_committere(ambulatio, *elem, NIHIL);
            }
        }
        si (canonica >= ZEPHYRUM && (i32)canonica < numerus)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(interps,
                (i32)canonica);

            si (elem != NIHIL)
            {
                *elem = _valorem_committere(ambulatio, *elem, nodus);
            }
        }
    }
}

interior SilvaValor
_valorem_committere (
    SilvaAmbulatio* ambulatio,
    SilvaValor      valor,
    SilvaNodus*     pater)
{
    commutatio (valor.genus)
    {
    casus SILVA_VALOR_NODUS:
        si (valor.datum.nodus == NIHIL)
        {
            redde valor;
        }
        valor.datum.nodus->pater = pater;
        si (valor.datum.nodus->genus
            == ambulatio->commissio->genus_ambigui)
        {
            _ambiguum_committere(ambulatio, valor.datum.nodus);
        }
        alioquin
        {
            _nodum_committere(ambulatio, valor.datum.nodus);
        }
        redde valor;

    casus SILVA_VALOR_LISTA:
    {
        i32 numerus;
        i32 i;

        valor = _listam_normalizare(ambulatio, valor);
        numerus = silva_valor_lista_numerus(valor);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            si (elem != NIHIL)
            {
                *elem = _valorem_committere(ambulatio, *elem, pater);
            }
        }
        redde valor;
    }

    ordinarius:
        redde valor;  /* TOKEN / INDEX / NIHIL - intacta */
    }
}


/* ==================================================
 * API
 * ================================================== */

SilvaCommissio*
silva_committere (
    SilvaPiscina*                       piscina,
    SilvaValor                     radix,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaOraculum*        oraculum,
    SilvaResolutor                 resolutor,
    vacuum*                        datum_resolutoris)
{
    SilvaCommissio* commissio;
    SilvaAmbulatio  ambulatio;
    s32 genus_ambigui = -I;
    s32 locus_interp = -I;
    s32 locus_canonicae = -I;
    i32 i;

    si (piscina == NIHIL || tabularium == NIHIL)
    {
        redde NIHIL;
    }

    /* Forma ambigui ex registro PER NOMEN (generator eam imposuit) */
    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans SilvaTabGenus* genus = &tabularium->genera[i];

        si (genus->titulus != NIHIL
            && strcmp(genus->titulus, "ambiguus") == ZEPHYRUM)
        {
            i32 k;

            genus_ambigui = (s32)i;
            per (k = ZEPHYRUM; k < genus->loci_numerus; k++)
            {
                constans SilvaTabLocus* locus =
                    &tabularium->loci[genus->loci_offset + k];

                si (locus->titulus == NIHIL) perge;
                si (strcmp(locus->titulus, "interpretationes")
                    == ZEPHYRUM)
                {
                    locus_interp = (s32)k;
                }
                si (strcmp(locus->titulus, "canonica") == ZEPHYRUM)
                {
                    locus_canonicae = (s32)k;
                }
            }
            frange;
        }
    }
    si (genus_ambigui < ZEPHYRUM || locus_interp < ZEPHYRUM
        || locus_canonicae < ZEPHYRUM)
    {
        fprintf(stderr,
            "silva_commissio: forma ambigui in registro abest\n");
        redde NIHIL;
    }

    commissio = (SilvaCommissio*)silva_piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaCommissio));
    si (commissio == NIHIL)
    {
        redde NIHIL;
    }
    commissio->ambigui = silva_xar_creare(piscina,
        (i32)magnitudo(SilvaNodus*));
    commissio->resolutiones = silva_xar_creare(piscina,
        (i32)magnitudo(SilvaResolutioEventum));
    commissio->genus_ambigui = genus_ambigui;
    commissio->locus_interpretationum = (i32)locus_interp;
    commissio->locus_canonicae = (i32)locus_canonicae;

    ambulatio.piscina = piscina;
    ambulatio.commissio = commissio;
    ambulatio.oraculum = oraculum;
    ambulatio.resolutor = resolutor;
    ambulatio.datum_resolutoris = datum_resolutoris;

    commissio->radix = _valorem_committere(&ambulatio, radix, NIHIL);
    redde commissio;
}

i32
silva_recanonicare (
    SilvaCommissio*         commissio,
    constans SilvaOraculum* oraculum,
    SilvaResolutor          resolutor,
    vacuum*                 datum_resolutoris)
{
    i32 versi = ZEPHYRUM;
    i32 i;

    si (commissio == NIHIL || oraculum == NIHIL || resolutor == NIHIL)
    {
        redde ZEPHYRUM;
    }

    per (i = ZEPHYRUM; i < silva_xar_numerus(commissio->ambigui); i++)
    {
        SilvaNodus** slot =
            (SilvaNodus**)silva_xar_obtinere(commissio->ambigui, i);
        SilvaNodus* nodus;
        SilvaResolutioResponsum responsum;
        SilvaValor interps;
        s32 canonica;

        si (slot == NIHIL || *slot == NIHIL) perge;
        nodus = *slot;
        si (nodus->genus != commissio->genus_ambigui)
        {
            perge;  /* iam collapsus alio modo? - praetermittitur */
        }

        responsum.victor = -I;
        responsum.discriminans = NIHIL;
        resolutor(nodus, oraculum, datum_resolutoris, &responsum);
        si (responsum.victor < ZEPHYRUM)
        {
            perge;  /* adhuc ignotum */
        }

        interps = nodus->loci[commissio->locus_interpretationum];
        canonica = nodus->loci[commissio->locus_canonicae].datum.index;
        si (responsum.victor == canonica
            || (i32)responsum.victor
                >= silva_valor_lista_numerus(interps))
        {
            perge;  /* iam canonica aut extra fines */
        }

        /* Versio in loco (op sanctionata post commissionem) + spina
         * localiter retexta */
        {
            SilvaValor* vetus = silva_valor_lista_obtinere(interps,
                (i32)canonica);
            SilvaValor* novus = silva_valor_lista_obtinere(interps,
                (i32)responsum.victor);
            SilvaResolutioEventum* eventum;

            si (vetus != NIHIL && vetus->genus == SILVA_VALOR_NODUS
                && vetus->datum.nodus != NIHIL)
            {
                vetus->datum.nodus->pater = NIHIL;
            }
            si (novus != NIHIL && novus->genus == SILVA_VALOR_NODUS
                && novus->datum.nodus != NIHIL)
            {
                novus->datum.nodus->pater = nodus;
            }
            nodus->loci[commissio->locus_canonicae] =
                silva_valor_index(responsum.victor);

            eventum = (SilvaResolutioEventum*)silva_xar_addere(
                commissio->resolutiones);
            si (eventum != NIHIL)
            {
                eventum->genus = SILVA_RESOLUTIO_RECANONICATA;
                eventum->sedes = nodus;
                eventum->victor = responsum.victor;
                eventum->genus_victoris =
                    (novus != NIHIL && novus->genus == SILVA_VALOR_NODUS
                     && novus->datum.nodus != NIHIL)
                    ? novus->datum.nodus->genus : -I;
                eventum->discriminans = responsum.discriminans;
            }
            versi++;
        }
    }
    redde versi;
}

/* ================= ex silva/fontes/silva_contextus.c ================= */

/* Copia cstring in piscinam (contextus vocatorem supervivit -
 * lectio vitae fons->via) */
interior constans character*
_literis_figere (SilvaPiscina* piscina, constans character* fons)
{
    memoriae_index mensura;
    character* novum;

    si (fons == NIHIL)
    {
        redde NIHIL;
    }
    mensura = strlen(fons) + I;
    novum = (character*)silva_piscina_allocare(piscina, mensura);
    si (novum == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(novum, fons, mensura);
    redde novum;
}

interior b32
_plagulam_addere (
    SilvaContextus*     contextus,
    SilvaXar*                quo,
    constans character* via,
    constans character* textus,
    i32                 mensura)
{
    SilvaContextusPlagula* locus;
    constans character* via_fixa;
    character* textus_fixus;

    si (contextus == NIHIL || quo == NIHIL || via == NIHIL
        || textus == NIHIL)
    {
        redde FALSUM;
    }
    via_fixa = _literis_figere(contextus->piscina, via);
    si (via_fixa == NIHIL)
    {
        redde FALSUM;
    }
    textus_fixus = (character*)silva_piscina_allocare(contextus->piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (textus_fixus == NIHIL)
    {
        redde FALSUM;
    }
    si (mensura > ZEPHYRUM)
    {
        memcpy(textus_fixus, textus, (memoriae_index)mensura);
    }

    locus = (SilvaContextusPlagula*)silva_xar_addere(quo);
    si (locus == NIHIL)
    {
        redde FALSUM;
    }
    locus->via = via_fixa;
    locus->textus = textus_fixus;
    locus->mensura = mensura;
    redde VERUM;
}

SilvaContextus*
silva_contextus_creare (SilvaPiscina* piscina)
{
    SilvaContextus* contextus;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    contextus = (SilvaContextus*)silva_piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaContextus));
    si (contextus == NIHIL)
    {
        redde NIHIL;
    }
    contextus->piscina = piscina;
    contextus->fines.lexemata = SILVA_LIMEN_LEXEMATUM_DEFALTUM;
    contextus->fines.generationes = SILVA_LIMEN_GENERATIONUM_DEFALTUM;
    contextus->fines.profunditas_includendi =
        SILVA_LIMEN_INCLUDENDI_DEFALTUM;
    contextus->fines.profunditas_regionum =
        SILVA_LIMEN_REGIONUM_DEFALTUM;
    contextus->fines.frons = SILVA_GLR_LIMEN_FRONTIS_DEFALTUM;
    contextus->pergere = NIHIL;
    contextus->pergere_datum = NIHIL;
    contextus->passus_pergendi = SILVA_GLR_PASSUS_PERGENDI_DEFALTUM;
    contextus->lexica = silva_xar_creare(piscina,
        magnitudo(SilvaContextusPlagula));
    contextus->praebenda = silva_xar_creare(piscina,
        magnitudo(SilvaContextusPlagula));
    si (contextus->lexica == NIHIL || contextus->praebenda == NIHIL)
    {
        redde NIHIL;
    }
    redde contextus;
}

b32
silva_contextus_lexicon_addere (
    SilvaContextus*     contextus,
    constans character* via,
    constans character* textus,
    i32                 mensura)
{
    si (contextus == NIHIL)
    {
        redde FALSUM;
    }
    redde _plagulam_addere(contextus, contextus->lexica, via, textus,
        mensura);
}

b32
silva_contextus_latinam_addere (SilvaContextus* contextus)
{
    redde silva_contextus_lexicon_addere(contextus, "latina.h",
        silva_latina_textus, silva_latina_mensura);
}

b32
silva_contextus_praebere (
    SilvaContextus*     contextus,
    constans character* via,
    constans character* textus,
    i32                 mensura)
{
    si (contextus == NIHIL)
    {
        redde FALSUM;
    }
    redde _plagulam_addere(contextus, contextus->praebenda, via,
        textus, mensura);
}

vacuum
silva_contextus_pergere_ponere (
    SilvaContextus*     contextus,
    SilvaPergereFunctio pergere,
    vacuum*             datum,
    i32                 passus)
{
    si (contextus == NIHIL)
    {
        redde;
    }
    contextus->pergere = pergere;
    contextus->pergere_datum = datum;
    contextus->passus_pergendi = passus;
}

/* ================= ex silva/fontes/silva_latina_datum.c ================= */

constans character silva_latina_textus[] = {
    35, 105, 102, 110, 100, 101, 102, 32, 76, 65, 84, 73,
    78, 65, 95, 72, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 76, 65, 84, 73, 78, 65, 95, 72, 10, 10, 35,
    105, 110, 99, 108, 117, 100, 101, 32, 60, 115, 116, 100,
    100, 101, 102, 46, 104, 62, 10, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 99, 104, 97, 114, 97, 99, 116, 101,
    114, 32, 9, 99, 104, 97, 114, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 98, 114, 101, 118, 105, 115, 32, 9,
    9, 9, 115, 104, 111, 114, 116, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 105, 110, 116, 101, 103, 101, 114, 32,
    9, 9, 105, 110, 116, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 108, 111, 110, 103, 117, 115, 9, 9, 9, 108,
    111, 110, 103, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    102, 108, 117, 105, 116, 97, 110, 115, 9, 9, 102, 108,
    111, 97, 116, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    100, 117, 112, 108, 101, 120, 9, 9, 9, 100, 111, 117,
    98, 108, 101, 10, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 118, 97, 99, 117, 117, 109, 9, 9, 9, 118, 111,
    105, 100, 10, 35, 100, 101, 102, 105, 110, 101, 32, 115,
    105, 103, 110, 97, 116, 117, 115, 32, 9, 9, 115, 105,
    103, 110, 101, 100, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 105, 110, 115, 105, 103, 110, 97, 116, 117, 115, 32,
    32, 117, 110, 115, 105, 103, 110, 101, 100, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 99, 111, 110, 115, 116, 97,
    110, 115, 9, 9, 99, 111, 110, 115, 116, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 118, 111, 108, 97, 116, 105,
    108, 105, 115, 9, 9, 118, 111, 108, 97, 116, 105, 108,
    101, 10, 35, 100, 101, 102, 105, 110, 101, 32, 115, 112,
    111, 110, 116, 101, 9, 9, 9, 97, 117, 116, 111, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 114, 101, 103, 105,
    115, 116, 114, 117, 109, 32, 9, 114, 101, 103, 105, 115,
    116, 101, 114, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    115, 116, 97, 116, 105, 99, 117, 115, 32, 9, 9, 115,
    116, 97, 116, 105, 99, 10, 35, 100, 101, 102, 105, 110,
    101, 9, 101, 120, 116, 101, 114, 110, 117, 115, 32, 9,
    9, 101, 120, 116, 101, 114, 110, 10, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 115, 105, 9, 9, 9, 9, 9,
    105, 102, 10, 35, 100, 101, 102, 105, 110, 101, 32, 97,
    108, 105, 111, 113, 117, 105, 110, 9, 9, 101, 108, 115,
    101, 10, 35, 100, 101, 102, 105, 110, 101, 32, 99, 111,
    109, 109, 117, 116, 97, 116, 105, 111, 9, 115, 119, 105,
    116, 99, 104, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    99, 97, 115, 117, 115, 9, 9, 9, 9, 99, 97, 115,
    101, 10, 35, 100, 101, 102, 105, 110, 101, 32, 111, 114,
    100, 105, 110, 97, 114, 105, 117, 115, 9, 100, 101, 102,
    97, 117, 108, 116, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 112, 101, 114, 9, 9, 9, 9, 9, 102, 111, 114,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 100, 117, 109,
    32, 9, 9, 9, 9, 119, 104, 105, 108, 101, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 102, 97, 99, 32, 9,
    9, 9, 9, 100, 111, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 102, 114, 97, 110, 103, 101, 32, 9, 9, 9,
    98, 114, 101, 97, 107, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 112, 101, 114, 103, 101, 32, 9, 9, 9, 99,
    111, 110, 116, 105, 110, 117, 101, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 115, 97, 108, 116, 97, 9, 9, 9,
    9, 103, 111, 116, 111, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 114, 101, 100, 100, 101, 9, 9, 9, 9, 114,
    101, 116, 117, 114, 110, 10, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 115, 116, 114, 117, 99, 116, 117, 114, 97,
    9, 9, 115, 116, 114, 117, 99, 116, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 117, 110, 105, 111, 32, 9, 9,
    9, 9, 117, 110, 105, 111, 110, 9, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 101, 110, 117, 109, 101, 114, 97,
    116, 105, 111, 32, 9, 101, 110, 117, 109, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 110, 111, 109, 101, 110, 32,
    9, 9, 9, 116, 121, 112, 101, 100, 101, 102, 10, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 109, 97, 103, 110,
    105, 116, 117, 100, 111, 32, 9, 115, 105, 122, 101, 111,
    102, 10, 10, 35, 100, 101, 102, 105, 110, 101, 32, 112,
    114, 105, 110, 99, 105, 112, 97, 108, 101, 32, 9, 109,
    97, 105, 110, 10, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 78, 73, 72, 73, 76, 9, 9, 9, 9, 78, 85,
    76, 76, 10, 35, 100, 101, 102, 105, 110, 101, 32, 86,
    69, 82, 85, 77, 32, 9, 9, 9, 49, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 70, 65, 76, 83, 85, 77,
    32, 9, 9, 9, 48, 10, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 90, 69, 80, 72, 89, 82, 85, 77, 32,
    32, 32, 32, 48, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 73, 32, 9, 9, 9, 9, 9, 49, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 73, 73, 9, 9, 9, 9,
    9, 50, 10, 35, 100, 101, 102, 105, 110, 101, 32, 73,
    73, 73, 9, 9, 9, 9, 9, 51, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 73, 86, 9, 9, 9, 9, 9,
    52, 10, 35, 100, 101, 102, 105, 110, 101, 32, 86, 9,
    9, 9, 9, 9, 9, 53, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 86, 73, 9, 9, 9, 9, 9, 54, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 86, 73, 73, 9,
    9, 9, 9, 9, 55, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 86, 73, 73, 73, 9, 9, 9, 9, 56, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 73, 88, 9, 9,
    9, 9, 9, 57, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 88, 9, 9, 9, 9, 9, 9, 49, 48, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 88, 73, 9, 9, 9,
    9, 9, 49, 49, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 88, 73, 73, 9, 9, 9, 9, 9, 49, 50, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 88, 73, 73, 73,
    9, 9, 9, 9, 49, 51, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 88, 73, 86, 9, 9, 9, 9, 9, 49,
    52, 10, 35, 100, 101, 102, 105, 110, 101, 32, 88, 86,
    9, 9, 9, 9, 9, 49, 53, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 88, 86, 73, 9, 9, 9, 9, 9,
    49, 54, 10, 35, 100, 101, 102, 105, 110, 101, 32, 88,
    86, 73, 73, 9, 9, 9, 9, 49, 55, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 88, 86, 73, 73, 73, 9,
    9, 9, 9, 49, 56, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 88, 73, 88, 9, 9, 9, 9, 9, 49, 57,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 88, 88, 9,
    9, 9, 9, 9, 50, 48, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 88, 88, 73, 9, 9, 9, 9, 9, 50,
    49, 10, 35, 100, 101, 102, 105, 110, 101, 32, 88, 88,
    73, 73, 9, 9, 9, 9, 50, 50, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 88, 88, 73, 73, 73, 9, 9,
    9, 9, 50, 51, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 88, 88, 73, 86, 9, 9, 9, 9, 50, 52, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 88, 88, 86, 9,
    9, 9, 9, 9, 50, 53, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 88, 88, 86, 73, 9, 9, 9, 9, 50,
    54, 10, 35, 100, 101, 102, 105, 110, 101, 32, 88, 88,
    86, 73, 73, 9, 9, 9, 9, 50, 55, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 88, 88, 86, 73, 73, 73,
    9, 9, 9, 50, 56, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 88, 88, 73, 88, 9, 9, 9, 9, 50, 57,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 88, 88, 88,
    9, 9, 9, 9, 9, 51, 48, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 88, 88, 88, 73, 9, 9, 9, 9,
    51, 49, 10, 35, 100, 101, 102, 105, 110, 101, 32, 88,
    88, 88, 73, 73, 9, 9, 9, 9, 51, 50, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 88, 88, 88, 73, 73,
    73, 9, 9, 9, 51, 51, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 88, 88, 88, 73, 86, 9, 9, 9, 9,
    51, 52, 10, 35, 100, 101, 102, 105, 110, 101, 32, 88,
    88, 88, 86, 9, 9, 9, 9, 51, 53, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 88, 88, 88, 86, 73, 9,
    9, 9, 9, 51, 54, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 88, 88, 88, 86, 73, 73, 9, 9, 9, 51,
    55, 10, 35, 100, 101, 102, 105, 110, 101, 32, 88, 88,
    88, 86, 73, 73, 73, 9, 9, 9, 51, 56, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 88, 88, 88, 73, 88,
    9, 9, 9, 9, 51, 57, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 88, 76, 9, 9, 9, 9, 9, 52, 48,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 88, 76, 73,
    9, 9, 9, 9, 9, 52, 49, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 88, 76, 73, 73, 9, 9, 9, 9,
    52, 50, 10, 35, 100, 101, 102, 105, 110, 101, 32, 88,
    76, 73, 73, 73, 9, 9, 9, 9, 52, 51, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 88, 76, 73, 86, 9,
    9, 9, 9, 52, 52, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 88, 76, 86, 9, 9, 9, 9, 9, 52, 53,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 88, 76, 86,
    73, 9, 9, 9, 9, 52, 54, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 88, 76, 86, 73, 73, 9, 9, 9,
    9, 52, 55, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    88, 76, 86, 73, 73, 73, 9, 9, 9, 52, 56, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 88, 76, 73, 88,
    9, 9, 9, 9, 52, 57, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 76, 9, 9, 9, 9, 9, 9, 53, 48,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 76, 73, 9,
    9, 9, 9, 9, 53, 49, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 76, 73, 73, 9, 9, 9, 9, 9, 53,
    50, 10, 35, 100, 101, 102, 105, 110, 101, 32, 76, 73,
    73, 73, 9, 9, 9, 9, 53, 51, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 76, 73, 86, 9, 9, 9, 9,
    9, 53, 52, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    76, 86, 9, 9, 9, 9, 9, 53, 53, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 76, 86, 73, 9, 9, 9,
    9, 9, 53, 54, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 76, 86, 73, 73, 9, 9, 9, 9, 53, 55, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 76, 86, 73, 73,
    73, 9, 9, 9, 9, 53, 56, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 76, 73, 88, 9, 9, 9, 9, 9,
    53, 57, 10, 35, 100, 101, 102, 105, 110, 101, 32, 76,
    88, 9, 9, 9, 9, 9, 54, 48, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 76, 88, 73, 9, 9, 9, 9,
    9, 54, 49, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    76, 88, 73, 73, 9, 9, 9, 9, 54, 50, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 76, 88, 73, 73, 73,
    9, 9, 9, 9, 54, 51, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 76, 88, 73, 86, 9, 9, 9, 9, 54,
    52, 10, 35, 100, 101, 102, 105, 110, 101, 32, 76, 88,
    86, 9, 9, 9, 9, 32, 32, 54, 53, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 76, 88, 86, 73, 9, 9,
    9, 9, 54, 54, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 76, 88, 86, 73, 73, 9, 9, 9, 9, 54, 55,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 76, 88, 86,
    73, 73, 73, 9, 9, 9, 54, 56, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 76, 88, 73, 88, 9, 9, 9,
    9, 54, 57, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    76, 88, 88, 9, 9, 9, 9, 9, 55, 48, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 76, 88, 88, 73, 9,
    9, 9, 9, 55, 49, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 76, 88, 88, 73, 73, 9, 9, 9, 9, 55,
    50, 10, 35, 100, 101, 102, 105, 110, 101, 32, 76, 88,
    88, 73, 73, 73, 9, 9, 9, 55, 51, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 76, 88, 88, 73, 86, 9,
    9, 9, 9, 55, 52, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 76, 88, 88, 86, 9, 9, 9, 9, 55, 53,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 76, 88, 88,
    86, 73, 9, 9, 9, 32, 32, 55, 54, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 76, 88, 88, 86, 73, 73,
    9, 9, 9, 55, 55, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 76, 88, 88, 86, 73, 73, 73, 9, 9, 9,
    55, 56, 10, 35, 100, 101, 102, 105, 110, 101, 32, 76,
    88, 88, 73, 88, 9, 9, 9, 9, 55, 57, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 76, 88, 88, 88, 9,
    9, 9, 9, 56, 48, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 76, 88, 88, 88, 73, 9, 9, 9, 9, 56,
    49, 10, 35, 100, 101, 102, 105, 110, 101, 32, 76, 88,
    88, 88, 73, 73, 9, 9, 9, 56, 50, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 76, 88, 88, 88, 73, 73,
    73, 9, 9, 9, 56, 51, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 76, 88, 88, 88, 73, 86, 9, 9, 9,
    56, 52, 10, 35, 100, 101, 102, 105, 110, 101, 32, 76,
    88, 88, 88, 86, 9, 9, 9, 9, 56, 53, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 76, 88, 88, 88, 86,
    73, 9, 9, 9, 56, 54, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 76, 88, 88, 88, 86, 73, 73, 9, 9,
    9, 56, 55, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    76, 88, 88, 88, 86, 73, 73, 73, 9, 9, 56, 56,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 76, 88, 88,
    88, 73, 88, 9, 9, 9, 56, 57, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 88, 67, 9, 9, 9, 9, 9,
    57, 48, 10, 35, 100, 101, 102, 105, 110, 101, 32, 88,
    67, 73, 9, 9, 9, 9, 9, 57, 49, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 88, 67, 73, 73, 9, 9,
    9, 9, 57, 50, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 88, 67, 73, 73, 73, 9, 9, 9, 9, 57, 51,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 88, 67, 73,
    86, 9, 9, 9, 9, 57, 52, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 88, 67, 86, 9, 9, 9, 9, 9,
    57, 53, 10, 35, 100, 101, 102, 105, 110, 101, 32, 88,
    67, 86, 73, 9, 9, 9, 9, 57, 54, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 88, 67, 86, 73, 73, 9,
    9, 9, 9, 57, 55, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 88, 67, 86, 73, 73, 73, 9, 9, 9, 57,
    56, 10, 35, 100, 101, 102, 105, 110, 101, 32, 88, 67,
    73, 88, 9, 9, 9, 9, 57, 57, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 9, 9, 9, 9, 9, 9,
    49, 48, 48, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 73, 32, 9, 9, 9, 9, 9, 49, 48, 49, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 73, 73, 32,
    9, 9, 9, 9, 49, 48, 50, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 73, 73, 73, 9, 9, 9, 9,
    49, 48, 51, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 73, 86, 9, 9, 9, 9, 9, 49, 48, 52, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 86, 9, 9,
    9, 9, 9, 49, 48, 53, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 86, 73, 9, 9, 9, 9, 9, 49,
    48, 54, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    86, 73, 73, 9, 9, 9, 9, 49, 48, 55, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 86, 73, 73, 73,
    9, 9, 9, 9, 49, 48, 56, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 73, 88, 9, 9, 9, 9, 9,
    49, 48, 57, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 88, 9, 9, 9, 9, 9, 49, 49, 48, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 88, 73, 9, 9,
    9, 9, 9, 49, 49, 49, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 88, 73, 73, 9, 9, 9, 9, 49,
    49, 50, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    88, 73, 73, 73, 9, 9, 9, 9, 49, 49, 51, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 88, 73, 86,
    9, 9, 9, 9, 49, 49, 52, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 88, 86, 9, 9, 9, 9, 9,
    49, 49, 53, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 88, 86, 73, 9, 9, 9, 9, 49, 49, 54, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 88, 86, 73,
    73, 9, 9, 9, 9, 49, 49, 55, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 88, 86, 73, 73, 73, 9,
    9, 9, 49, 49, 56, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 88, 73, 88, 9, 9, 9, 9, 49, 49,
    57, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 88,
    88, 32, 9, 9, 9, 9, 49, 50, 48, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 88, 88, 73, 9, 9,
    9, 9, 49, 50, 49, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 88, 88, 73, 73, 32, 9, 9, 9, 49,
    50, 50, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    88, 88, 73, 73, 73, 9, 9, 9, 49, 50, 51, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 88, 88, 73,
    86, 32, 9, 9, 9, 49, 50, 52, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 88, 88, 86, 32, 9, 9,
    9, 9, 49, 50, 53, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 88, 88, 86, 73, 32, 9, 9, 9, 49,
    50, 54, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    88, 88, 86, 73, 73, 32, 9, 9, 9, 49, 50, 55,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 88, 88,
    86, 73, 73, 73, 32, 9, 9, 49, 50, 56, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 88, 88, 73, 88,
    32, 9, 9, 9, 49, 50, 57, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 88, 88, 88, 32, 9, 9, 9,
    9, 49, 51, 48, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 88, 88, 88, 73, 32, 9, 9, 9, 49, 51,
    49, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 88,
    88, 88, 73, 73, 32, 9, 9, 9, 49, 51, 50, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 88, 88, 88,
    73, 73, 73, 9, 9, 9, 49, 51, 51, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 88, 88, 88, 73, 86,
    32, 9, 9, 9, 49, 51, 52, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 88, 88, 88, 86, 32, 9, 9,
    9, 49, 51, 53, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 88, 88, 88, 86, 73, 32, 9, 9, 9, 49,
    51, 54, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    88, 88, 88, 86, 73, 73, 32, 9, 9, 49, 51, 55,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 88, 88,
    88, 86, 73, 73, 73, 32, 9, 9, 49, 51, 56, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 88, 88, 88,
    73, 88, 32, 9, 9, 9, 49, 51, 57, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 88, 76, 32, 9, 9,
    9, 9, 49, 52, 48, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 88, 76, 73, 32, 9, 9, 9, 9, 49,
    52, 49, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    88, 76, 73, 73, 32, 9, 9, 9, 49, 52, 50, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 88, 76, 73,
    73, 73, 9, 9, 9, 49, 52, 51, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 88, 76, 73, 86, 32, 9,
    9, 9, 49, 52, 52, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 88, 76, 86, 32, 9, 9, 9, 9, 49,
    52, 53, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    88, 76, 86, 73, 32, 9, 9, 9, 49, 52, 54, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 88, 76, 86,
    73, 73, 32, 9, 9, 9, 49, 52, 55, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 88, 76, 86, 73, 73,
    73, 32, 9, 9, 49, 52, 56, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 88, 76, 73, 88, 32, 9, 9,
    9, 49, 52, 57, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 76, 9, 9, 9, 9, 9, 49, 53, 48, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 76, 73, 32,
    9, 9, 9, 9, 49, 53, 49, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 76, 73, 73, 32, 9, 9, 9,
    9, 49, 53, 50, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 76, 73, 73, 73, 32, 9, 9, 9, 49, 53,
    51, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 76,
    73, 86, 32, 9, 9, 9, 9, 49, 53, 52, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 76, 86, 32, 9,
    9, 9, 9, 49, 53, 53, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 76, 86, 73, 32, 9, 9, 9, 9,
    49, 53, 54, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 76, 86, 73, 73, 32, 9, 9, 9, 49, 53, 55,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 76, 86,
    73, 73, 73, 32, 9, 9, 9, 49, 53, 56, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 76, 73, 88, 32,
    9, 9, 9, 9, 49, 53, 57, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 76, 88, 9, 9, 9, 9, 9,
    49, 54, 48, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 76, 88, 73, 9, 9, 9, 9, 49, 54, 49, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 76, 88, 73,
    73, 9, 9, 9, 9, 49, 54, 50, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 76, 88, 73, 73, 73, 9,
    9, 9, 49, 54, 51, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 76, 88, 73, 86, 9, 9, 9, 9, 49,
    54, 52, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    76, 88, 86, 9, 9, 9, 9, 49, 54, 53, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 76, 88, 86, 73,
    9, 9, 9, 9, 49, 54, 54, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 76, 88, 86, 73, 73, 9, 9,
    9, 49, 54, 55, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 76, 88, 86, 73, 73, 73, 9, 9, 9, 49,
    54, 56, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    76, 88, 73, 88, 9, 9, 9, 9, 49, 54, 57, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 76, 88, 88,
    9, 9, 9, 9, 49, 55, 48, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 76, 88, 88, 73, 9, 9, 9,
    9, 49, 55, 49, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 76, 88, 88, 73, 73, 9, 9, 9, 49, 55,
    50, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 76,
    88, 88, 73, 73, 73, 9, 9, 9, 49, 55, 51, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 76, 88, 88,
    73, 86, 9, 9, 9, 49, 55, 52, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 76, 88, 88, 86, 9, 9,
    9, 9, 49, 55, 53, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 76, 88, 88, 86, 73, 9, 9, 9, 49,
    55, 54, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    76, 88, 88, 86, 73, 73, 9, 9, 9, 49, 55, 55,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 76, 88,
    88, 86, 73, 73, 73, 9, 9, 49, 55, 56, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 76, 88, 88, 73,
    88, 9, 9, 9, 49, 55, 57, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 76, 88, 88, 88, 9, 9, 9,
    9, 49, 56, 48, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 76, 88, 88, 88, 73, 9, 9, 9, 49, 56,
    49, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 76,
    88, 88, 88, 73, 73, 9, 9, 9, 49, 56, 50, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 76, 88, 88,
    88, 73, 73, 73, 9, 9, 49, 56, 51, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 76, 88, 88, 88, 73,
    86, 9, 9, 9, 49, 56, 52, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 76, 88, 88, 88, 86, 9, 9,
    9, 49, 56, 53, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 76, 88, 88, 88, 86, 73, 9, 9, 9, 49,
    56, 54, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    76, 88, 88, 88, 86, 73, 73, 9, 9, 49, 56, 55,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 76, 88,
    88, 88, 86, 73, 73, 73, 9, 9, 49, 56, 56, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 76, 88, 88,
    88, 73, 88, 9, 9, 9, 49, 56, 57, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 88, 67, 9, 9, 9,
    9, 9, 49, 57, 48, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 88, 67, 73, 32, 9, 9, 9, 9, 49,
    57, 49, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    88, 67, 73, 73, 32, 9, 9, 9, 49, 57, 50, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 88, 67, 73,
    73, 73, 9, 9, 9, 49, 57, 51, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 88, 67, 73, 86, 9, 9,
    9, 9, 49, 57, 52, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 88, 67, 86, 9, 9, 9, 9, 49, 57,
    53, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 88,
    67, 86, 73, 9, 9, 9, 9, 49, 57, 54, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 88, 67, 86, 73,
    73, 9, 9, 9, 49, 57, 55, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 88, 67, 86, 73, 73, 73, 9,
    9, 9, 49, 57, 56, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 88, 67, 73, 88, 9, 9, 9, 9, 49,
    57, 57, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    67, 9, 9, 9, 9, 9, 50, 48, 48, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 67, 73, 9, 9, 9,
    9, 9, 50, 48, 49, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 67, 73, 73, 9, 9, 9, 9, 50, 48,
    50, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 67,
    73, 73, 73, 9, 9, 9, 9, 50, 48, 51, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 67, 73, 86, 9,
    9, 9, 9, 50, 48, 52, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 67, 86, 9, 9, 9, 9, 9, 50,
    48, 53, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    67, 86, 73, 9, 9, 9, 9, 50, 48, 54, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 67, 86, 73, 73,
    9, 9, 9, 9, 50, 48, 55, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 67, 86, 73, 73, 73, 9, 9,
    9, 50, 48, 56, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 67, 73, 88, 9, 9, 9, 9, 50, 48, 57,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 67, 88,
    9, 9, 9, 9, 9, 50, 49, 48, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 67, 88, 73, 9, 9, 9,
    9, 50, 49, 49, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 67, 88, 73, 73, 9, 9, 9, 9, 50, 49,
    50, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 67,
    88, 73, 73, 73, 9, 9, 9, 50, 49, 51, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 67, 88, 73, 86,
    9, 9, 9, 32, 32, 50, 49, 52, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 67, 88, 88, 9, 9, 9,
    9, 50, 50, 48, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 67, 88, 88, 88, 9, 9, 9, 9, 50, 51,
    48, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 67,
    88, 88, 88, 73, 9, 9, 9, 50, 51, 49, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 67, 88, 88, 88,
    73, 73, 9, 9, 9, 50, 51, 50, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 67, 88, 88, 88, 73, 73,
    73, 9, 9, 50, 51, 51, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 67, 88, 88, 88, 73, 86, 9, 9,
    9, 50, 51, 52, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 67, 88, 88, 88, 86, 9, 9, 9, 50, 51,
    53, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 67,
    88, 88, 88, 86, 73, 9, 9, 9, 50, 51, 54, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 67, 88, 88,
    88, 86, 73, 73, 9, 9, 50, 51, 55, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 67, 88, 88, 88, 86,
    73, 73, 73, 9, 9, 50, 51, 56, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 67, 88, 88, 88, 73, 88,
    9, 9, 9, 50, 51, 57, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 67, 88, 76, 9, 9, 9, 9, 50,
    52, 48, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    67, 88, 76, 73, 9, 9, 9, 9, 50, 52, 49, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 67, 88, 76,
    73, 73, 9, 9, 9, 50, 52, 50, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 67, 76, 9, 9, 9, 9,
    9, 50, 53, 48, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 67, 76, 73, 32, 9, 9, 9, 9, 50, 53,
    49, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 67,
    76, 73, 73, 32, 9, 9, 9, 50, 53, 50, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 67, 76, 73, 73,
    73, 32, 9, 9, 9, 50, 53, 51, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 67, 76, 73, 86, 32, 9,
    9, 9, 50, 53, 52, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 67, 76, 86, 32, 9, 9, 9, 9, 50,
    53, 53, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    67, 76, 86, 73, 32, 9, 9, 9, 50, 53, 54, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 67, 76, 88,
    9, 9, 9, 9, 50, 54, 48, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 67, 76, 88, 88, 9, 9, 9,
    9, 50, 55, 48, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 67, 76, 88, 88, 88, 9, 9, 9, 50, 56,
    48, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 67,
    76, 88, 88, 88, 73, 86, 9, 9, 50, 56, 52, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 67, 76, 88,
    88, 88, 86, 9, 9, 32, 32, 50, 56, 53, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 67, 88, 67, 9,
    9, 9, 9, 50, 57, 48, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 67, 67, 9, 9, 9, 9, 32, 32,
    51, 48, 48, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 67, 67, 73, 9, 9, 9, 9, 51, 48, 49, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 67, 67, 73,
    73, 9, 9, 9, 9, 51, 48, 50, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 67, 67, 73, 73, 73, 9,
    9, 9, 51, 48, 51, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 67, 67, 73, 86, 9, 9, 9, 9, 51,
    48, 52, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    67, 67, 86, 9, 9, 9, 9, 51, 48, 53, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 67, 67, 86, 73,
    9, 9, 9, 9, 51, 48, 54, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 67, 67, 86, 73, 73, 9, 9,
    9, 51, 48, 55, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 67, 67, 86, 73, 73, 73, 9, 9, 9, 51,
    48, 56, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    67, 67, 88, 9, 9, 9, 9, 51, 49, 48, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 67, 67, 88, 88,
    9, 9, 9, 9, 51, 50, 48, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 67, 67, 88, 88, 88, 9, 9,
    9, 51, 51, 48, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 67, 67, 88, 76, 9, 9, 9, 9, 51, 52,
    48, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 67,
    67, 76, 88, 32, 9, 9, 9, 51, 54, 48, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 68, 9, 9, 9,
    9, 9, 52, 48, 48, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 68, 73, 9, 9, 9, 9, 9, 52, 48,
    49, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 68,
    73, 73, 9, 9, 9, 9, 52, 48, 50, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 68, 73, 73, 73, 9,
    9, 9, 9, 52, 48, 51, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 68, 73, 86, 9, 9, 9, 9, 52,
    48, 52, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    68, 86, 9, 9, 9, 9, 9, 52, 48, 53, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 68, 86, 73, 9,
    9, 9, 9, 52, 48, 54, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 68, 86, 73, 73, 9, 9, 9, 9,
    52, 48, 55, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 68, 86, 73, 73, 73, 9, 9, 9, 52, 48, 56,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 68, 73,
    88, 9, 9, 9, 9, 52, 48, 57, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 68, 88, 9, 9, 9, 9,
    9, 52, 49, 48, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 68, 88, 73, 9, 9, 9, 9, 52, 49, 49,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 68, 88,
    73, 73, 9, 9, 9, 9, 52, 49, 50, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 68, 88, 73, 73, 73,
    9, 9, 9, 52, 49, 51, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 68, 88, 73, 86, 9, 9, 9, 9,
    52, 49, 52, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 68, 88, 86, 9, 9, 9, 9, 52, 49, 53, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 68, 88, 86,
    73, 9, 9, 9, 9, 52, 49, 54, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 68, 88, 86, 73, 73, 9,
    9, 9, 52, 49, 55, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 68, 88, 86, 73, 73, 73, 9, 9, 9,
    52, 49, 56, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 68, 88, 73, 88, 9, 9, 9, 9, 52, 49, 57,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 68, 88,
    88, 32, 9, 9, 9, 9, 52, 50, 48, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 68, 88, 88, 73, 9,
    9, 9, 9, 52, 50, 49, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 68, 88, 88, 73, 73, 9, 9, 9,
    52, 50, 50, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 68, 88, 88, 73, 73, 73, 9, 9, 9, 52, 50,
    51, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 68,
    88, 88, 73, 86, 9, 9, 9, 52, 50, 52, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 68, 88, 88, 86,
    9, 9, 9, 9, 52, 50, 53, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 68, 88, 88, 86, 73, 32, 9,
    9, 9, 52, 50, 54, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 68, 88, 88, 86, 73, 73, 9, 9, 9,
    52, 50, 55, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    67, 68, 88, 88, 86, 73, 73, 73, 9, 9, 52, 50,
    56, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 68,
    88, 88, 73, 88, 9, 9, 9, 52, 50, 57, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 68, 88, 88, 88,
    32, 9, 9, 9, 52, 51, 48, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 68, 88, 76, 32, 9, 9, 9,
    9, 52, 52, 48, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 68, 76, 32, 9, 9, 9, 9, 52, 53, 48,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 68, 76,
    73, 32, 9, 9, 9, 9, 52, 53, 49, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 67, 68, 76, 73, 73, 32,
    9, 9, 9, 52, 53, 50, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 67, 68, 76, 73, 73, 73, 32, 9, 9,
    9, 52, 53, 51, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 68, 76, 73, 86, 32, 9, 9, 9, 52, 53,
    52, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67, 68,
    76, 86, 32, 9, 9, 9, 9, 52, 53, 53, 10, 35,
    100, 101, 102, 105, 110, 101, 32, 67, 68, 76, 86, 73,
    32, 9, 9, 9, 52, 53, 54, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 68, 76, 86, 73, 73, 9, 9,
    9, 52, 53, 55, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 67, 68, 76, 86, 73, 73, 73, 9, 9, 9, 52,
    53, 56, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    68, 76, 73, 88, 9, 9, 9, 9, 52, 53, 57, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 68, 76, 88,
    32, 9, 9, 9, 9, 52, 54, 48, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 68, 76, 88, 88, 9, 9,
    9, 9, 52, 55, 48, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 67, 68, 76, 88, 88, 88, 9, 9, 9, 52,
    56, 48, 10, 35, 100, 101, 102, 105, 110, 101, 32, 68,
    9, 9, 9, 9, 9, 9, 53, 48, 48, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 68, 73, 9, 9, 9, 9,
    9, 53, 48, 49, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 68, 73, 73, 9, 9, 9, 9, 9, 53, 48, 50,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 68, 73, 73,
    73, 9, 9, 9, 9, 53, 48, 51, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 68, 73, 86, 9, 9, 9, 9,
    9, 53, 48, 52, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 68, 86, 9, 9, 9, 9, 9, 53, 48, 53, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 68, 86, 73, 9,
    9, 9, 9, 9, 53, 48, 54, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 68, 86, 73, 73, 9, 9, 9, 9,
    53, 48, 55, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    68, 86, 73, 73, 73, 9, 9, 9, 9, 53, 48, 56,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 68, 73, 88,
    9, 9, 9, 9, 9, 53, 48, 57, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 68, 88, 32, 9, 9, 9, 9,
    9, 53, 49, 48, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 68, 88, 73, 32, 9, 9, 9, 9, 53, 49, 49,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 68, 88, 73,
    73, 32, 9, 9, 9, 9, 53, 49, 50, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 68, 76, 88, 86, 73, 73,
    73, 9, 9, 9, 53, 54, 56, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 68, 76, 88, 88, 86, 9, 9, 9,
    9, 53, 55, 53, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 68, 67, 9, 9, 9, 9, 9, 54, 48, 48, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 68, 67, 88, 88,
    88, 73, 88, 9, 9, 9, 54, 51, 57, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 68, 67, 67, 9, 9, 9,
    9, 9, 55, 48, 48, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 68, 67, 67, 76, 9, 9, 9, 9, 55, 53,
    48, 10, 35, 100, 101, 102, 105, 110, 101, 32, 68, 67,
    67, 67, 9, 9, 9, 9, 56, 48, 48, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 68, 67, 67, 67, 76, 73,
    73, 73, 9, 9, 56, 53, 51, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 67, 77, 9, 9, 9, 9, 9, 57,
    48, 48, 10, 35, 100, 101, 102, 105, 110, 101, 32, 67,
    77, 76, 88, 88, 73, 88, 9, 9, 32, 32, 57, 55,
    57, 10, 35, 100, 101, 102, 105, 110, 101, 32, 77, 9,
    9, 9, 9, 9, 9, 49, 48, 48, 48, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 77, 68, 67, 67, 76, 88,
    88, 86, 73, 9, 9, 49, 55, 55, 54, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 77, 67, 77, 9, 9, 9,
    9, 9, 49, 57, 48, 48, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 77, 67, 77, 88, 32, 9, 9, 9, 32,
    32, 49, 57, 49, 48, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 77, 67, 77, 88, 67, 73, 88, 9, 9, 9,
    49, 57, 57, 57, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 77, 77, 9, 9, 9, 9, 9, 50, 48, 48, 48,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 77, 77, 73,
    86, 9, 9, 9, 9, 50, 48, 48, 52, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 77, 77, 88, 88, 9, 9,
    9, 9, 50, 48, 50, 48, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 77, 77, 88, 88, 73, 9, 9, 9, 9,
    50, 48, 50, 49, 10, 35, 100, 101, 102, 105, 110, 101,
    32, 77, 77, 88, 88, 73, 73, 9, 9, 9, 50, 48,
    50, 50, 10, 35, 100, 101, 102, 105, 110, 101, 32, 77,
    77, 88, 88, 73, 73, 73, 9, 9, 9, 50, 48, 50,
    51, 10, 35, 100, 101, 102, 105, 110, 101, 32, 77, 77,
    88, 88, 73, 86, 9, 9, 9, 50, 48, 50, 52, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 77, 77, 88, 88,
    86, 9, 9, 9, 9, 50, 48, 50, 53, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 77, 77, 67, 9, 9, 9,
    9, 9, 50, 49, 48, 48, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 77, 77, 77, 9, 9, 9, 9, 9, 51,
    48, 48, 48, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    77, 77, 77, 77, 9, 9, 9, 9, 52, 48, 48, 48,
    10, 35, 100, 101, 102, 105, 110, 101, 32, 77, 77, 77,
    77, 88, 67, 86, 73, 9, 9, 52, 48, 57, 54, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 67, 67, 67, 76,
    88, 86, 9, 9, 9, 51, 54, 53, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 67, 67, 67, 76, 88, 86, 73,
    9, 9, 9, 51, 54, 54, 10, 10, 35, 100, 101, 102,
    105, 110, 101, 32, 105, 109, 112, 114, 105, 109, 101, 114,
    101, 32, 9, 112, 114, 105, 110, 116, 102, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 108, 105, 98, 101, 114, 97,
    114, 101, 32, 9, 9, 102, 114, 101, 101, 10, 35, 100,
    101, 102, 105, 110, 101, 32, 109, 101, 109, 111, 114, 105,
    97, 101, 95, 97, 108, 108, 111, 99, 97, 114, 101, 9,
    109, 97, 108, 108, 111, 99, 10, 35, 100, 101, 102, 105,
    110, 101, 32, 101, 120, 105, 114, 101, 9, 9, 9, 9,
    101, 120, 105, 116, 10, 10, 35, 100, 101, 102, 105, 110,
    101, 32, 105, 110, 116, 101, 114, 105, 111, 114, 32, 9,
    9, 115, 116, 97, 116, 105, 99, 32, 10, 35, 100, 101,
    102, 105, 110, 101, 32, 104, 105, 99, 95, 109, 97, 110,
    101, 110, 115, 32, 9, 115, 116, 97, 116, 105, 99, 10,
    35, 100, 101, 102, 105, 110, 101, 32, 117, 110, 105, 118,
    101, 114, 115, 97, 108, 105, 115, 32, 115, 116, 97, 116,
    105, 99, 10, 10, 35, 100, 101, 102, 105, 110, 101, 32,
    70, 73, 76, 69, 32, 70, 73, 76, 85, 77, 10, 10,
    110, 111, 109, 101, 110, 32, 105, 110, 115, 105, 103, 110,
    97, 116, 117, 115, 32, 99, 104, 97, 114, 97, 99, 116,
    101, 114, 9, 105, 56, 59, 10, 110, 111, 109, 101, 110,
    32, 105, 110, 115, 105, 103, 110, 97, 116, 117, 115, 32,
    98, 114, 101, 118, 105, 115, 32, 9, 9, 105, 49, 54,
    59, 10, 110, 111, 109, 101, 110, 32, 105, 110, 115, 105,
    103, 110, 97, 116, 117, 115, 32, 105, 110, 116, 101, 103,
    101, 114, 32, 32, 32, 9, 105, 51, 50, 59, 10, 110,
    111, 109, 101, 110, 32, 105, 110, 115, 105, 103, 110, 97,
    116, 117, 115, 32, 108, 111, 110, 103, 117, 115, 32, 108,
    111, 110, 103, 117, 115, 9, 105, 54, 52, 59, 10, 10,
    110, 111, 109, 101, 110, 32, 115, 105, 103, 110, 97, 116,
    117, 115, 32, 99, 104, 97, 114, 97, 99, 116, 101, 114,
    32, 32, 32, 32, 115, 56, 59, 10, 110, 111, 109, 101,
    110, 32, 115, 105, 103, 110, 97, 116, 117, 115, 32, 98,
    114, 101, 118, 105, 115, 32, 32, 32, 9, 32, 9, 115,
    49, 54, 59, 10, 110, 111, 109, 101, 110, 32, 115, 105,
    103, 110, 97, 116, 117, 115, 32, 105, 110, 116, 101, 103,
    101, 114, 32, 32, 32, 32, 32, 9, 115, 51, 50, 59,
    10, 110, 111, 109, 101, 110, 32, 115, 105, 103, 110, 97,
    116, 117, 115, 32, 108, 111, 110, 103, 117, 115, 32, 108,
    111, 110, 103, 117, 115, 9, 115, 54, 52, 59, 10, 10,
    110, 111, 109, 101, 110, 32, 102, 108, 117, 105, 116, 97,
    110, 115, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    9, 9, 102, 51, 50, 59, 10, 110, 111, 109, 101, 110,
    32, 100, 117, 112, 108, 101, 120, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 9, 9, 9, 9, 102, 54, 52, 59,
    10, 10, 110, 111, 109, 101, 110, 32, 105, 110, 116, 101,
    103, 101, 114, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 9, 9, 98, 51, 50, 59, 10, 10, 110,
    111, 109, 101, 110, 32, 115, 105, 122, 101, 95, 116, 32,
    9, 9, 9, 9, 9, 9, 9, 9, 109, 101, 109, 111,
    114, 105, 97, 101, 95, 105, 110, 100, 101, 120, 59, 10,
    10, 35, 101, 110, 100, 105, 102, 32, 47, 42, 32, 76,
    65, 84, 73, 78, 65, 95, 72, 32, 42, 47, 10, 10
};

constans i32 silva_latina_mensura = 8556;

/* ================= ex silva/fontes/silva_parsare.c ================= */

/* Genus ERROR + locus lexematum + numerus locorum ex registro per
 * nomen */
interior b32
_formam_erroris_invenire (
    constans SilvaRegistrumCoctum* tabularium,
    s32*                           genus_out,
    i32*                           locus_out,
    i32*                           numerus_locorum_out)
{
    i32 i;

    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans SilvaTabGenus* genus = &tabularium->genera[i];

        si (genus->titulus != NIHIL
            && strcmp(genus->titulus, "error") == ZEPHYRUM)
        {
            i32 k;

            per (k = ZEPHYRUM; k < genus->loci_numerus; k++)
            {
                constans SilvaTabLocus* locus =
                    &tabularium->loci[genus->loci_offset + k];

                si (locus->titulus != NIHIL
                    && strcmp(locus->titulus, "tokens") == ZEPHYRUM)
                {
                    *genus_out = (s32)i;
                    *locus_out = k;
                    *numerus_locorum_out = genus->loci_numerus;
                    redde VERUM;
                }
            }
        }
    }
    fprintf(stderr,
        "silva_parsare: forma erroris in registro abest\n");
    redde FALSUM;
}

/* Nodus ERROR ex lexematibus segmenti [initium, finis) */
interior SilvaValor
_nodum_erroris_facere (
    SilvaPiscina*      piscina,
    constans SilvaXar* lexemata,
    i32           initium,
    i32           finis,
    s32           genus_erroris,
    i32           locus_lexematum,
    i32           numerus_locorum)
{
    SilvaNodus* nodus;
    i32 i;

    nodus = silva_nodus_creare(piscina, genus_erroris, numerus_locorum);
    si (nodus == NIHIL)
    {
        redde silva_valor_nihil();
    }
    per (i = initium; i < finis; i++)
    {
        SilvaToken** ref = (SilvaToken**)silva_xar_obtinere(lexemata, i);

        si (ref != NIHIL && *ref != NIHIL)
        {
            silva_nodus_appendere(piscina, nodus, locus_lexematum,
                silva_valor_token(*ref), SILVA_LOCUS_LISTA_TOKEN);
        }
    }
    redde silva_valor_nodus(nodus);
}

/* Textura conditionalium (infra definita - ante commissionem) */
interior SilvaValor
_texere (
    SilvaPiscina*                       piscina,
    SilvaValor                     radix,
    SilvaExpansio*                 expansio,
    constans SilvaRegistrumCoctum* tabularium,
    SilvaParsura*                  parsura);

/* Nucleus circuitus secantis; contextus NIHIL licet (fines motoris
 * defaltae tunc solae valent); expansio (si praesens) texturam
 * conditionalium ante commissionem permittit */
interior SilvaParsura*
_lexemata_parsare_interna (
    SilvaPiscina*                  piscina,
    constans SilvaXar*             lexemata,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris,
    SilvaExpansio*            expansio,
    constans SilvaContextus*  contextus)
{
    SilvaParsura* parsura;
    SilvaGLR*     glr;
    SilvaToken*   lexema_eof = NIHIL;
    SilvaValor    radix;
    s32           genus_erroris = -I;
    i32           locus_lexematum = ZEPHYRUM;
    i32           numerus_locorum_erroris = ZEPHYRUM;
    i32           numerus;
    i32           initium;
    i32           i;
    i32           profunditas;

    si (piscina == NIHIL || lexemata == NIHIL || grammatica == NIHIL)
    {
        redde NIHIL;
    }
    si (!_formam_erroris_invenire(grammatica->tabularium,
            &genus_erroris, &locus_lexematum,
            &numerus_locorum_erroris))
    {
        redde NIHIL;
    }
    glr = silva_glr_creare(piscina, grammatica->tabula,
        grammatica->constructor, grammatica->fabrica);
    si (glr == NIHIL)
    {
        redde NIHIL;
    }
    si (contextus != NIHIL)
    {
        glr->limen_frontis = contextus->fines.frons;
        glr->pergere = contextus->pergere;
        glr->pergere_datum = contextus->pergere_datum;
        glr->passus_pergendi = contextus->passus_pergendi;
    }

    parsura = (SilvaParsura*)silva_piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaParsura));
    si (parsura == NIHIL)
    {
        redde NIHIL;
    }
    parsura->successus = FALSUM;
    parsura->commissio = NIHIL;
    parsura->lexema_finis = NIHIL;
    parsura->lexemata = NIHIL;
    parsura->expansio = NIHIL;
    parsura->directivae = NIHIL;
    parsura->fons_princeps = -I;
    parsura->numerus_errorum = ZEPHYRUM;
    parsura->numerus_segmentorum = ZEPHYRUM;
    parsura->fusiones = ZEPHYRUM;
    parsura->transmutationes = ZEPHYRUM;
    parsura->transmutationes_negatae = ZEPHYRUM;
    parsura->eventa_marginis_novi = ZEPHYRUM;
    parsura->frons_maxima = ZEPHYRUM;
    parsura->est_intermissa = FALSUM;
    parsura->expansio_decisa = FALSUM;
    parsura->fines_tactae = FALSUM;
    parsura->segmenta_ultra_limen = ZEPHYRUM;
    parsura->regiones_textae = ZEPHYRUM;
    parsura->regiones_omissae = ZEPHYRUM;
    parsura->strata = NIHIL;

    numerus = silva_xar_numerus(lexemata);

    /* Lexema EOF (ultimum - forma silva_lexare) */
    si (numerus > ZEPHYRUM)
    {
        SilvaToken** ref = (SilvaToken**)silva_xar_obtinere(lexemata,
            numerus - I);

        si (ref != NIHIL && *ref != NIHIL
            && (*ref)->genus == SILVA_LEX_EOF)
        {
            lexema_eof = *ref;
        }
    }
    parsura->lexema_finis = lexema_eof;

    radix = silva_valor_lista_nova(piscina);

    /* CIRCUITUS SECANS: segmentum = lexemata usque ad SEMICOLON in
     * profunditate 0 (inclusum); quodque + EOF independenter
     * parsatur. Fractura -> nodus ERROR (recuperatio per-rem). */
    initium = ZEPHYRUM;
    profunditas = ZEPHYRUM;
    i = ZEPHYRUM;
    dum (i < numerus)
    {
        SilvaToken** ref = (SilvaToken**)silva_xar_obtinere(lexemata, i);
        SilvaToken*  lexema = (ref != NIHIL) ? *ref : NIHIL;
        b32 est_finis_segmenti = FALSUM;
        b32 est_eof = FALSUM;

        si (lexema == NIHIL)
        {
            i++;
            perge;
        }
        si (lexema->genus == SILVA_LEX_EOF)
        {
            est_eof = VERUM;
            est_finis_segmenti = (i > initium) ? VERUM : FALSUM;
        }
        alioquin si (lexema->genus == SILVA_LEX_PAREN_APERTA)
        {
            profunditas++;
        }
        alioquin si (lexema->genus == SILVA_LEX_PAREN_CLAUSA)
        {
            si (profunditas > ZEPHYRUM)
            {
                profunditas--;
            }
        }
        alioquin si (lexema->genus == SILVA_LEX_SEMICOLON
            && profunditas == ZEPHYRUM)
        {
            est_finis_segmenti = VERUM;
        }

        si (est_finis_segmenti)
        {
            i32 finis = est_eof ? i : (i + I);
            SilvaXar* segmentum;
            i32 k;
            SilvaGLRFructus fructus;

            /* Intermissio (Phase 7): ante segmentum interrogamus;
             * cauda TOTA [initium..EOF) nodus ERROR unus fit - arbor
             * completa manet, reconstructio byte-exacta tenet */
            si (!parsura->est_intermissa
                && contextus != NIHIL && contextus->pergere != NIHIL
                && !contextus->pergere(contextus->pergere_datum))
            {
                parsura->est_intermissa = VERUM;
            }
            si (parsura->est_intermissa)
            {
                i32 finis_caudae = (lexema_eof != NIHIL)
                    ? (numerus - I) : numerus;

                si (finis_caudae > initium)
                {
                    SilvaValor nodus_caudae = _nodum_erroris_facere(
                        piscina, lexemata, initium, finis_caudae,
                        genus_erroris, locus_lexematum,
                        numerus_locorum_erroris);

                    si (nodus_caudae.genus == SILVA_VALOR_NODUS)
                    {
                        radix = silva_valor_lista_appendere(piscina,
                            radix, nodus_caudae);
                        parsura->numerus_errorum++;
                    }
                }
                frange;
            }

            segmentum = silva_xar_creare(piscina,
                (i32)magnitudo(SilvaToken*));

            per (k = initium; k < finis; k++)
            {
                SilvaToken** fons_ref =
                    (SilvaToken**)silva_xar_obtinere(lexemata, k);
                SilvaToken** slot =
                    (SilvaToken**)silva_xar_addere(segmentum);

                si (slot != NIHIL && fons_ref != NIHIL)
                {
                    *slot = *fons_ref;
                }
            }
            si (lexema_eof != NIHIL)
            {
                SilvaToken** slot =
                    (SilvaToken**)silva_xar_addere(segmentum);

                si (slot != NIHIL)
                {
                    *slot = lexema_eof;
                }
            }

            fructus = silva_glr_parsare(glr, segmentum, piscina);
            parsura->numerus_segmentorum++;
            parsura->fusiones += fructus.fusiones;
            parsura->transmutationes += fructus.transmutationes;
            parsura->transmutationes_negatae +=
                fructus.transmutationes_negatae;
            parsura->eventa_marginis_novi +=
                fructus.eventa_marginis_novi;
            si (fructus.frons_maxima > parsura->frons_maxima)
            {
                parsura->frons_maxima = fructus.frons_maxima;
            }
            si (fructus.est_ultra_limen)
            {
                parsura->segmenta_ultra_limen++;
                parsura->fines_tactae = VERUM;
            }
            si (fructus.est_intermissus)
            {
                /* Segmentum hoc nodus ERROR fit (via ordinaria infra);
                 * cauda in limite proximo (unus nodus) */
                parsura->est_intermissa = VERUM;
            }

            si (fructus.successus
                && fructus.valor.genus == SILVA_VALOR_LISTA)
            {
                i32 n = silva_valor_lista_numerus(fructus.valor);
                i32 e;

                per (e = ZEPHYRUM; e < n; e++)
                {
                    SilvaValor* elem = silva_valor_lista_obtinere(
                        fructus.valor, e);

                    si (elem != NIHIL)
                    {
                        radix = silva_valor_lista_appendere(piscina,
                            radix, *elem);
                    }
                }
            }
            alioquin
            {
                /* Recuperatio: nodus ERROR lexemata segmenti fert */
                SilvaValor nodus_erroris = _nodum_erroris_facere(
                    piscina, lexemata, initium, finis,
                    genus_erroris, locus_lexematum,
                    numerus_locorum_erroris);

                si (nodus_erroris.genus == SILVA_VALOR_NODUS)
                {
                    radix = silva_valor_lista_appendere(piscina,
                        radix, nodus_erroris);
                    parsura->numerus_errorum++;
                }
            }
            initium = est_eof ? i : (i + I);
        }

        si (est_eof)
        {
            frange;
        }
        i++;
    }

    /* Textura conditionalium (Phase 7 Chunk B) - ANTE commissionem
     * (pater fixup nodos novos ambulatione generica tegit) */
    si (expansio != NIHIL)
    {
        radix = _texere(piscina, radix, expansio,
            grammatica->tabularium, parsura);
    }

    /* Commissio: pater + normalizatio + resolutio (collapse+diarium) */
    parsura->commissio = silva_committere(piscina, radix,
        grammatica->tabularium, oraculum, resolutor, datum_resolutoris);
    parsura->successus = (parsura->commissio != NIHIL) ? VERUM : FALSUM;
    redde parsura;
}

SilvaParsura*
silva_lexemata_parsare (
    SilvaPiscina*                  piscina,
    constans SilvaXar*             lexemata,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris)
{
    redde _lexemata_parsare_interna(piscina, lexemata, grammatica,
        oraculum, resolutor, datum_resolutoris, NIHIL, NIHIL);
}

/* ==================================================
 * Textura conditionalium (Phase 7 Chunk B, simulatio ⑦)
 *
 * Passus gubernatoris ANTE commissionem: regiones conditionales in
 * arborem texuntur ubi limites regionis cum limitibus sententiarum
 * congruunt (detectio per extenta strati 0); aliter regio degradat
 * (laminis reinserendis possessa manet - mos hodiernus IPSE est via
 * regressus). Textura est opt-in per regionem; numeratores
 * regiones_textae/omissae rationem produnt.
 *
 * Formae nodorum (genera-extra, generator eas IMPONIT):
 *   conditionalis {rami:lista-nodus, finis:lista-token}
 *   ramus-sumptus {directiva, contentum:lista-nodus, conditio_id}
 *   ramus-omissus {directiva, cruda:lista-token, conditio_id}
 * Ordo locorum == ordo octetorum - emissor genericus nihil novi
 * requirit.
 * ================================================== */

/* Extentum strati 0 valoris: fons + [initium, finis) super lexemata
 * radicum. fons -1 = nondum visum; -2 = opacum (catena pasta/chorda
 * aut fontes mixti) - degradatio conservativa. */
nomen structura {
    s32 fons;
    s32 initium;
    s32 finis;
} TexExtentum;

interior vacuum
_extentum_valoris (SilvaValor valor, TexExtentum* extentum)
{
    commutatio (valor.genus)
    {
        casus SILVA_VALOR_TOKEN:
        {
            SilvaToken* t = valor.datum.token;

            dum (t != NIHIL
                && (t->origo.genus == SILVA_ORIGO_EXPANSIO
                    || t->origo.genus == SILVA_ORIGO_CHORDA))
            {
                t = (t->origo.genus == SILVA_ORIGO_EXPANSIO)
                    ? t->origo.datum.expansio.invocatio
                    : t->origo.datum.stringificatio.primus;
            }
            si (t == NIHIL || t->origo.genus != SILVA_ORIGO_FONS)
            {
                extentum->fons = -II;  /* opacum */
                redde;
            }
            si (extentum->fons == -II)
            {
                redde;
            }
            si (extentum->fons == -I)
            {
                extentum->fons = t->fons_index;
                extentum->initium = t->byte_offset;
                extentum->finis = t->byte_offset + (s32)t->longitudo;
            }
            alioquin si (extentum->fons != t->fons_index)
            {
                extentum->fons = -II;  /* fontes mixti */
            }
            alioquin
            {
                si (t->byte_offset < extentum->initium)
                {
                    extentum->initium = t->byte_offset;
                }
                si (t->byte_offset + (s32)t->longitudo > extentum->finis)
                {
                    extentum->finis = t->byte_offset + (s32)t->longitudo;
                }
            }
            frange;
        }
        casus SILVA_VALOR_NODUS:
        {
            constans SilvaNodus* nodus = valor.datum.nodus;
            i32 k;

            per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
            {
                _extentum_valoris(nodus->loci[k], extentum);
                si (extentum->fons == -II)
                {
                    redde;
                }
            }
            frange;
        }
        casus SILVA_VALOR_LISTA:
        {
            i32 n = silva_valor_lista_numerus(valor);
            i32 k;

            per (k = ZEPHYRUM; k < n; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(valor, k);

                si (elem != NIHIL)
                {
                    _extentum_valoris(*elem, extentum);
                    si (extentum->fons == -II)
                    {
                        redde;
                    }
                }
            }
            frange;
        }
        ordinarius:
            frange;  /* INDEX/NIHIL: sine positione */
    }
}

/* Formae generum texturae ex registro PER NOMEN (mos commissionis) */
nomen structura {
    s32 genus;
    i32 numerus_locorum;
    s32 locus_a;
    s32 locus_b;
    s32 locus_c;
} TexForma;

interior b32
_formam_texturae_invenire (
    constans SilvaRegistrumCoctum* tabularium,
    constans character*            titulus_generis,
    constans character*            titulus_a,
    constans character*            titulus_b,
    constans character*            titulus_c,
    TexForma*                      forma)
{
    i32 i;

    forma->genus = -I;
    forma->locus_a = -I;
    forma->locus_b = -I;
    forma->locus_c = -I;
    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans SilvaTabGenus* genus = &tabularium->genera[i];
        i32 k;

        si (genus->titulus == NIHIL
            || strcmp(genus->titulus, titulus_generis) != ZEPHYRUM)
        {
            perge;
        }
        forma->genus = (s32)i;
        forma->numerus_locorum = genus->loci_numerus;
        per (k = ZEPHYRUM; k < genus->loci_numerus; k++)
        {
            constans SilvaTabLocus* locus =
                &tabularium->loci[genus->loci_offset + k];

            si (locus->titulus == NIHIL) perge;
            si (strcmp(locus->titulus, titulus_a) == ZEPHYRUM)
            {
                forma->locus_a = (s32)k;
            }
            alioquin si (titulus_b != NIHIL
                && strcmp(locus->titulus, titulus_b) == ZEPHYRUM)
            {
                forma->locus_b = (s32)k;
            }
            alioquin si (titulus_c != NIHIL
                && strcmp(locus->titulus, titulus_c) == ZEPHYRUM)
            {
                forma->locus_c = (s32)k;
            }
        }
        redde (forma->locus_a >= ZEPHYRUM
            && (titulus_b == NIHIL || forma->locus_b >= ZEPHYRUM)
            && (titulus_c == NIHIL || forma->locus_c >= ZEPHYRUM))
            ? VERUM : FALSUM;
    }
    redde FALSUM;
}

/* Status passus texturae */
nomen structura {
    SilvaPiscina*                       piscina;
    constans SilvaRegistrumCoctum* tabularium;
    SilvaParsura*                  parsura;
    SilvaValor                     radix;    /* lista currens */
    TexForma                       conditionalis; /* a=rami b=finis */
    TexForma                       sumptus;  /* a=directiva b=contentum c=conditio_id */
    TexForma                       omissus;  /* a=directiva b=cruda c=conditio_id */
} TexturaStatus;

/* Lexemata laminae in locum listae-token appendere */
interior vacuum
_laminam_in_locum (SilvaPiscina* piscina, SilvaNodus* nodus, i32 locus,
    SilvaXar* lamina)
{
    i32 k;

    si (lamina == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < silva_xar_numerus(lamina); k++)
    {
        SilvaToken** ref = (SilvaToken**)silva_xar_obtinere(
            lamina, k);

        si (ref != NIHIL && *ref != NIHIL)
        {
            silva_nodus_appendere(piscina, nodus, locus,
                silva_valor_token(*ref), SILVA_LOCUS_LISTA_TOKEN);
        }
    }
}

/* Regionem unam texere temptare (filiae IAM visitatae - profundum
 * primum). Detectio: quodque elementum radicis regioni intersecans
 * PLENE intra corpus rami sumpti iacere debet; elementum opacum
 * intersectionis incertae = degradatio conservativa. */
interior vacuum
_regionem_texere (TexturaStatus* st, SilvaRegio* regio)
{
    SilvaRamus* ramus_sumptus = NIHIL;
    s32 regio_initium;
    s32 regio_finis;
    i32 numerus_elementorum;
    s32 primum_congregatum = -I;
    s32 primum_post = -I;
    b32 degradata = FALSUM;
    b32* congreganda;
    i32 e;

    si (regio->rami == NIHIL || silva_xar_numerus(regio->rami) == ZEPHYRUM)
    {
        st->parsura->regiones_omissae++;
        redde;
    }

    /* Fines regionis in offsetibus */
    {
        SilvaRamus* primus = *(SilvaRamus**)silva_xar_obtinere(regio->rami,
            ZEPHYRUM);
        SilvaToken* t;

        si (primus->directiva == NIHIL
            || silva_xar_numerus(primus->directiva) == ZEPHYRUM)
        {
            st->parsura->regiones_omissae++;
            redde;
        }
        t = *(SilvaToken**)silva_xar_obtinere(primus->directiva, ZEPHYRUM);
        regio_initium = t->byte_offset;
    }
    si (regio->directiva_finis != NIHIL
        && silva_xar_numerus(regio->directiva_finis) > ZEPHYRUM)
    {
        SilvaToken* ultimum = *(SilvaToken**)silva_xar_obtinere(
            regio->directiva_finis,
            (i32)(silva_xar_numerus(regio->directiva_finis) - I));

        regio_finis = ultimum->byte_offset + (s32)ultimum->longitudo;
    }
    alioquin
    {
        regio_finis = 0x7FFFFFFF;  /* imperfecta: apertum ad EOF */
    }

    /* Ramus sumptus (si quis) */
    {
        i32 r;

        per (r = ZEPHYRUM; r < silva_xar_numerus(regio->rami); r++)
        {
            SilvaRamus* ramus = *(SilvaRamus**)silva_xar_obtinere(
                regio->rami, r);

            si (ramus != NIHIL && ramus->est_sumptum)
            {
                ramus_sumptus = ramus;
                frange;
            }
        }
    }

    /* Elementa classificare */
    numerus_elementorum = silva_valor_lista_numerus(st->radix);
    congreganda = (b32*)silva_piscina_allocare(st->piscina,
        (memoriae_index)(magnitudo(b32)
            * (numerus_elementorum > ZEPHYRUM
                ? (memoriae_index)numerus_elementorum : I)));
    si (congreganda == NIHIL)
    {
        st->parsura->regiones_omissae++;
        redde;
    }
    per (e = ZEPHYRUM; e < numerus_elementorum; e++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(st->radix, e);
        TexExtentum extentum;

        congreganda[e] = FALSUM;
        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        extentum.fons = -I;
        extentum.initium = ZEPHYRUM;
        extentum.finis = ZEPHYRUM;
        _extentum_valoris(*elem, &extentum);
        si (extentum.fons == -II)
        {
            /* opacum: positio incerta - degradatio conservativa */
            degradata = VERUM;
            frange;
        }
        si (extentum.fons == -I || extentum.fons != regio->fons_index)
        {
            perge;  /* plagula alia aut sine positione */
        }
        si (extentum.finis <= regio_initium)
        {
            perge;  /* ante regionem */
        }
        si (extentum.initium >= regio_finis)
        {
            si (primum_post == -I)
            {
                primum_post = (s32)e;
            }
            perge;  /* post regionem */
        }
        /* intersecat: plene intra corpus rami sumpti aut degradatio */
        si (ramus_sumptus != NIHIL
            && ramus_sumptus->corpus_initium >= ZEPHYRUM
            && extentum.initium >= ramus_sumptus->corpus_initium
            && extentum.finis <= ramus_sumptus->corpus_finis)
        {
            congreganda[e] = VERUM;
            si (primum_congregatum == -I)
            {
                primum_congregatum = (s32)e;
            }
        }
        alioquin
        {
            degradata = VERUM;  /* limes transgressus */
            frange;
        }
    }
    si (degradata)
    {
        st->parsura->regiones_omissae++;
        redde;
    }

    /* Nodum conditionalis aedificare */
    {
        SilvaNodus* nodus_conditionalis;
        i32 r;

        nodus_conditionalis = silva_nodus_creare(st->piscina,
            st->conditionalis.genus,
            st->conditionalis.numerus_locorum);
        si (nodus_conditionalis == NIHIL)
        {
            st->parsura->regiones_omissae++;
            redde;
        }
        per (r = ZEPHYRUM; r < silva_xar_numerus(regio->rami); r++)
        {
            SilvaRamus* ramus = *(SilvaRamus**)silva_xar_obtinere(
                regio->rami, r);
            constans TexForma* forma;
            SilvaNodus* nodus_rami;

            si (ramus == NIHIL) perge;
            forma = ramus->est_sumptum ? &st->sumptus : &st->omissus;
            nodus_rami = silva_nodus_creare(st->piscina, forma->genus,
                forma->numerus_locorum);
            si (nodus_rami == NIHIL)
            {
                st->parsura->regiones_omissae++;
                redde;
            }
            _laminam_in_locum(st->piscina, nodus_rami, (i32)forma->locus_a,
                ramus->directiva);
            si (ramus->est_sumptum)
            {
                per (e = ZEPHYRUM; e < numerus_elementorum; e++)
                {
                    si (congreganda[e])
                    {
                        SilvaValor* elem = silva_valor_lista_obtinere(
                            st->radix, e);

                        si (elem != NIHIL)
                        {
                            silva_nodus_appendere(st->piscina,
                                nodus_rami, (i32)forma->locus_b, *elem,
                                SILVA_LOCUS_LISTA_NODUS);
                        }
                    }
                }
            }
            alioquin
            {
                _laminam_in_locum(st->piscina, nodus_rami,
                    (i32)forma->locus_b, ramus->lexemata_cruda);
            }
            silva_nodus_ponere(nodus_rami, (i32)forma->locus_c,
                silva_valor_index((s32)ramus->conditio_id),
                SILVA_LOCUS_INDEX);
            silva_nodus_appendere(st->piscina, nodus_conditionalis,
                (i32)st->conditionalis.locus_a,
                silva_valor_nodus(nodus_rami),
                SILVA_LOCUS_LISTA_NODUS);
        }
        _laminam_in_locum(st->piscina, nodus_conditionalis,
            (i32)st->conditionalis.locus_b, regio->directiva_finis);

        /* Listam radicis retexere: congregata sublata, conditionalis
         * in positione ordinali primi congregati (aut ante primum
         * elementum eiusdem fontis post regionem; aut in fine) */
        {
            SilvaValor lista_nova = silva_valor_lista_nova(st->piscina);
            i32 positio_insertionis;
            b32 insertum = FALSUM;

            si (primum_congregatum >= ZEPHYRUM)
            {
                positio_insertionis = (i32)primum_congregatum;
            }
            alioquin si (primum_post >= ZEPHYRUM)
            {
                positio_insertionis = (i32)primum_post;
            }
            alioquin
            {
                positio_insertionis = numerus_elementorum;
            }
            per (e = ZEPHYRUM; e < numerus_elementorum; e++)
            {
                SilvaValor* elem;

                si (e == positio_insertionis)
                {
                    lista_nova = silva_valor_lista_appendere(
                        st->piscina, lista_nova,
                        silva_valor_nodus(nodus_conditionalis));
                    insertum = VERUM;
                }
                si (congreganda[e])
                {
                    perge;  /* in contentum migravit */
                }
                elem = silva_valor_lista_obtinere(st->radix, e);
                si (elem != NIHIL)
                {
                    lista_nova = silva_valor_lista_appendere(
                        st->piscina, lista_nova, *elem);
                }
            }
            si (!insertum)
            {
                lista_nova = silva_valor_lista_appendere(st->piscina,
                    lista_nova,
                    silva_valor_nodus(nodus_conditionalis));
            }
            st->radix = lista_nova;
        }
    }

    regio->est_texta = VERUM;
    st->parsura->regiones_textae++;
}

/* Arbor regionum profundum primum (filiae ante patrem - conditionalis
 * interior elementum extenti gerens fit quod textura exterior
 * congregat) */
interior vacuum
_regiones_texere (TexturaStatus* st, SilvaXar* regiones)
{
    i32 i;

    si (regiones == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < silva_xar_numerus(regiones); i++)
    {
        SilvaRegio* regio = *(SilvaRegio**)silva_xar_obtinere(regiones, i);

        si (regio == NIHIL) perge;
        _regiones_texere(st, regio->filiae);
        _regionem_texere(st, regio);
    }
}

/* Ingressus passus: radix nova (aut eadem) redditur */
interior SilvaValor
_texere (
    SilvaPiscina*                       piscina,
    SilvaValor                     radix,
    SilvaExpansio*                 expansio,
    constans SilvaRegistrumCoctum* tabularium,
    SilvaParsura*                  parsura)
{
    TexturaStatus st;

    si (expansio == NIHIL || expansio->regiones == NIHIL
        || silva_xar_numerus(expansio->regiones) == ZEPHYRUM)
    {
        redde radix;
    }
    si (!_formam_texturae_invenire(tabularium, "conditionalis",
            "rami", "finis", NIHIL, &st.conditionalis)
        || !_formam_texturae_invenire(tabularium, "ramus-sumptus",
            "directiva", "contentum", "conditio_id", &st.sumptus)
        || !_formam_texturae_invenire(tabularium, "ramus-omissus",
            "directiva", "cruda", "conditio_id", &st.omissus))
    {
        /* generator formas imponit - hic defensivum solum */
        fprintf(stderr,
            "silva_parsare: formae texturae in registro absunt\n");
        redde radix;
    }
    st.piscina = piscina;
    st.tabularium = tabularium;
    st.parsura = parsura;
    st.radix = radix;
    _regiones_texere(&st, expansio->regiones);
    redde st.radix;
}

/* Fistula tota: praeprocessor + circuitus secans; contextus NIHIL
 * licet (via cum_expansione) */
interior SilvaParsura*
_fistula_interna (
    SilvaPiscina*                  piscina,
    SilvaExpansio*            expansio,
    constans SilvaContextus*  contextus,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris)
{
    s32            fons_index;
    SilvaXar*           lexemata;
    SilvaXar*           reliqua;
    SilvaXar*           expansa;
    SilvaXar*           strata;
    SilvaXar*           directivae;
    SilvaParsura*  parsura;

    si (piscina == NIHIL || expansio == NIHIL || fons == NIHIL
        || grammatica == NIHIL)
    {
        redde NIHIL;
    }

    /* Fistula praeprocessoris: lexare -> directivae (lineae captae -
     * reconstructio fontis, Phase 5) -> expansio positionalis
     * (point-in-time - Phase 2.5) */
    fons_index = silva_fons_addere(expansio,
        (titulus_fontis != NIHIL) ? titulus_fontis : "<fons>", FALSUM);
    lexemata = silva_lexare(piscina, fons, mensura, fons_index);
    directivae = NIHIL;
    reliqua = silva_expansio_directivas_processare(expansio, lexemata,
        &directivae);
    strata = NIHIL;
    expansa = silva_expansio_expandere_reliqua(expansio, reliqua,
        &strata);

    parsura = _lexemata_parsare_interna(piscina, expansa, grammatica,
        oraculum, resolutor, datum_resolutoris, expansio, contextus);
    si (parsura != NIHIL)
    {
        parsura->lexemata = expansa;
        parsura->strata = strata;
        parsura->expansio = expansio;
        parsura->directivae = directivae;
        parsura->fons_princeps = fons_index;

        /* Vexilla expansionis in fructum (fines/intermissio in
         * praeprocessore tactae) */
        si (expansio->expansio_decisa)
        {
            parsura->expansio_decisa = VERUM;
            parsura->fines_tactae = VERUM;
        }
        si (expansio->fines_tactae)
        {
            parsura->fines_tactae = VERUM;
        }
        si (expansio->est_intermissa)
        {
            parsura->est_intermissa = VERUM;
        }
    }
    redde parsura;
}

SilvaParsura*
silva_parsare_cum_expansione (
    SilvaPiscina*                  piscina,
    SilvaExpansio*            expansio,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris)
{
    redde _fistula_interna(piscina, expansio, NIHIL, titulus_fontis,
        fons, mensura, grammatica, oraculum, resolutor,
        datum_resolutoris);
}

/* Receptum contextus expansioni recenti applicare: fines + pergere
 * deorsum copiantur (strata inferiora contextum nesciunt), praebenda
 * praebentur, lexica processantur (definitiones eorum a positione 0
 * fontis principalis valent - positus eventorum novorum in zephyrum
 * fingitur). */
interior b32
_contextum_applicare (
    SilvaPiscina*                 piscina,
    SilvaExpansio*           expansio,
    constans SilvaContextus* contextus)
{
    i32 k;

    expansio->limen_lexematum = contextus->fines.lexemata;
    expansio->limen_generationum = contextus->fines.generationes;
    expansio->limen_includendi =
        contextus->fines.profunditas_includendi;
    expansio->limen_regionum = contextus->fines.profunditas_regionum;
    expansio->pergere = contextus->pergere;
    expansio->pergere_datum = contextus->pergere_datum;

    per (k = ZEPHYRUM; k < silva_xar_numerus(contextus->praebenda); k++)
    {
        constans SilvaContextusPlagula* plagula =
            (constans SilvaContextusPlagula*)silva_xar_obtinere(
                contextus->praebenda, k);

        si (plagula == NIHIL
            || silva_includendum_praebere(expansio, plagula->via,
                   plagula->textus, plagula->mensura) < ZEPHYRUM)
        {
            redde FALSUM;
        }
    }

    per (k = ZEPHYRUM; k < silva_xar_numerus(contextus->lexica); k++)
    {
        constans SilvaContextusPlagula* plagula =
            (constans SilvaContextusPlagula*)silva_xar_obtinere(
                contextus->lexica, k);
        s32  fons_index;
        SilvaXar* lexemata;
        i32  acta_ante;
        i32  e;

        si (plagula == NIHIL)
        {
            redde FALSUM;
        }
        acta_ante = silva_xar_numerus(expansio->acta);
        fons_index = silva_fons_addere(expansio, plagula->via, FALSUM);
        si (fons_index < ZEPHYRUM)
        {
            redde FALSUM;
        }
        lexemata = silva_lexare(piscina, plagula->textus,
            plagula->mensura, fons_index);
        si (lexemata == NIHIL)
        {
            redde FALSUM;
        }
        /* Reliqua lexici abiciuntur (lexicon = directivae; contentum
         * praeter eas nusquam fluit - documentatum) */
        silva_expansio_directivas_processare(expansio, lexemata,
            NIHIL);

        /* Positus in zephyrum: eventa lexici a positione 0 fluxus
         * PRINCIPALIS valent (positus eorum ad fluxum lexici
         * pertinebat) */
        per (e = acta_ante; e < silva_xar_numerus(expansio->acta); e++)
        {
            SilvaEventum* eventum =
                (SilvaEventum*)silva_xar_obtinere(expansio->acta, e);

            si (eventum != NIHIL)
            {
                eventum->positus = ZEPHYRUM;
            }
        }
    }
    redde VERUM;
}

SilvaParsura*
silva_parsare_cum_contextu (
    SilvaPiscina*                  piscina,
    constans SilvaContextus*  contextus,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris)
{
    SilvaExpansio* expansio;

    si (piscina == NIHIL || contextus == NIHIL)
    {
        redde NIHIL;
    }
    expansio = silva_expansio_creare(piscina);
    si (expansio == NIHIL)
    {
        redde NIHIL;
    }
    si (!_contextum_applicare(piscina, expansio, contextus))
    {
        redde NIHIL;
    }
    redde _fistula_interna(piscina, expansio, contextus,
        titulus_fontis, fons, mensura, grammatica, oraculum, resolutor,
        datum_resolutoris);
}

SilvaParsura*
silva_parsare (
    SilvaPiscina*                  piscina,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris)
{
    SilvaExpansio* expansio;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    expansio = silva_expansio_creare(piscina);
    si (expansio == NIHIL)
    {
        redde NIHIL;
    }
    redde silva_parsare_cum_expansione(piscina, expansio,
        titulus_fontis, fons, mensura, grammatica, oraculum, resolutor,
        datum_resolutoris);
}

/* ================= ex silva/fontes/silva_scribere.c ================= */

#define SILVA_SCRIBERE_OMNIA 0x7FFFFFFF

/* ==================================================
 * Reinserendum - lamina per offset reinserenda
 * ================================================== */

nomen structura {
    s32  offset;
    SilvaXar* lamina;    /* Xar de SilvaToken* */
} SilvaReinserendum;

/* ==================================================
 * Status scriptoris
 * ================================================== */

nomen structura {
    SilvaChordaAedificator*             aed;
    constans SilvaRegistrumCoctum* tabularium;
    constans SilvaExpansio*        expansio;   /* NIHIL licet */
    s32                            genus_ambigui;
    i32                            locus_interpretationum;
    i32                            locus_canonicae;
    s32                            fons_index;   /* -1 = quaelibet */
    SilvaXar*                           reinserenda;  /* ordinata; NIHIL */
    i32                            reinserenda_index;
    s32                            fons_ultimus; /* -2 = nulla adhuc */
    s32                            emissum_usque;
    b32                            successus;
    constans character*            causa;
    constans SilvaNodus*           sedes;
} SilvaScriptor;

interior vacuum _valorem_scribere (SilvaScriptor* st, SilvaValor valor);
interior vacuum _nodum_scribere (SilvaScriptor* st,
    constans SilvaNodus* nodus);

interior vacuum
_fractura (SilvaScriptor* st, constans character* causa,
    constans SilvaNodus* sedes)
{
    si (st->successus)
    {
        st->successus = FALSUM;
        st->causa = causa;
        st->sedes = sedes;
    }
}

/* Forma ambigui ex registro PER NOMEN (exemplar commissionis).
 * Reddit FALSUM si registrum formam non fert (generator eam imponit -
 * absentia = tabulae corruptae aut manu confectae). */
interior b32
_formam_ambigui_invenire (SilvaScriptor* st)
{
    i32 g;

    st->genus_ambigui = -I;
    st->locus_interpretationum = ZEPHYRUM;
    st->locus_canonicae = ZEPHYRUM;

    per (g = ZEPHYRUM; g < st->tabularium->numerus_generum; g++)
    {
        constans SilvaTabGenus* def = &st->tabularium->genera[g];

        si (def->titulus != NIHIL
            && strcmp(def->titulus, "ambiguus") == ZEPHYRUM)
        {
            i32 k;
            s32 interp = -I;
            s32 canon = -I;

            per (k = ZEPHYRUM; k < def->loci_numerus; k++)
            {
                constans SilvaTabLocus* locus =
                    &st->tabularium->loci[def->loci_offset + k];

                si (locus->titulus == NIHIL) perge;
                si (strcmp(locus->titulus, "interpretationes") == ZEPHYRUM)
                {
                    interp = (s32)k;
                }
                si (strcmp(locus->titulus, "canonica") == ZEPHYRUM)
                {
                    canon = (s32)k;
                }
            }
            si (interp < ZEPHYRUM || canon < ZEPHYRUM)
            {
                redde FALSUM;
            }
            st->genus_ambigui = (s32)g;
            st->locus_interpretationum = (i32)interp;
            st->locus_canonicae = (i32)canon;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Limes expansionis (Chunk B, sim ⑥ C4/C5)
 * ================================================== */

/* Radix strati 0 cum probatione puritatis: catenam invocationum
 * sequitur. PASTA/CHORDA (radix trans plagulam definitionis errare
 * potest) et API (fons syntheticus) = stratum 0 non recuperabile -
 * deferral nominatum (INTENTIO Phase 5). */
interior SilvaToken*
_radix_probata (SilvaToken* token, b32* impurum_out)
{
    dum (token != NIHIL)
    {
        commutatio (token->origo.genus)
        {
            casus SILVA_ORIGO_FONS:
                redde token;
            casus SILVA_ORIGO_EXPANSIO:
                token = token->origo.datum.expansio.invocatio;
                frange;
            casus SILVA_ORIGO_CHORDA:
                /* stringificatio (#x): primus = lexema primum
                 * argumenti CRUDI (use-site) - intra extentum
                 * invocationis iacet; quaestio continentiae infra
                 * extentum invenit (Chunk C - corpus solarii
                 * deferral coegit, vectis maximalista) */
                token = token->origo.datum.stringificatio.primus;
                frange;
            ordinarius:
                *impurum_out = VERUM;
                redde token;
        }
    }
    *impurum_out = VERUM;
    redde NIHIL;
}

/* Extentum cuius lamina offset radicis CONTINET (pro lexematibus
 * CHORDA: radix = lexema argumenti INTRA invocationem, non lexema
 * nominis - quaestio per identitatem fallit, continentia invenit).
 * Scansio linearis - numeri parvi. */
interior SilvaXar*
_extentum_continens (constans SilvaExpansio* expansio,
    constans SilvaToken* radix)
{
    i32 k;

    si (expansio == NIHIL || expansio->extenta == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < silva_xar_numerus(expansio->extenta); k++)
    {
        SilvaExtentumInvocationis* extentum =
            (SilvaExtentumInvocationis*)silva_xar_obtinere(
                expansio->extenta, k);
        SilvaToken* primum;
        SilvaToken* ultimum;

        si (extentum == NIHIL || extentum->lamina == NIHIL
            || silva_xar_numerus(extentum->lamina) == ZEPHYRUM)
        {
            perge;
        }
        primum = *(SilvaToken**)silva_xar_obtinere(extentum->lamina,
            ZEPHYRUM);
        ultimum = *(SilvaToken**)silva_xar_obtinere(extentum->lamina,
            (i32)(silva_xar_numerus(extentum->lamina) - I));
        si (primum->fons_index == radix->fons_index
            && radix->byte_offset >= primum->byte_offset
            && radix->byte_offset
                < ultimum->byte_offset + (s32)ultimum->longitudo)
        {
            redde extentum->lamina;
        }
    }
    redde NIHIL;
}

/* Extentum invocationis functio-similis strati 0 (scansio linearis -
 * numeri parvi; radix comparatur per identitatem monstratoris) */
interior SilvaXar*
_extentum_quaerere (constans SilvaExpansio* expansio,
    constans SilvaToken* radix)
{
    i32 k;

    si (expansio == NIHIL || expansio->extenta == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < silva_xar_numerus(expansio->extenta); k++)
    {
        SilvaExtentumInvocationis* ext = (SilvaExtentumInvocationis*)
            silva_xar_obtinere(expansio->extenta, k);

        si (ext != NIHIL && ext->invocatio == radix)
        {
            redde ext->lamina;
        }
    }
    redde NIHIL;
}

/* Lamina verbatim (sine fusione reinserendorum) */
interior vacuum
_laminam_emittere (SilvaScriptor* st, SilvaXar* lamina)
{
    i32 k;

    per (k = ZEPHYRUM; k < silva_xar_numerus(lamina); k++)
    {
        silva_lexema_emittere_in(st->aed,
            *(SilvaToken**)silva_xar_obtinere(lamina, k));
    }
}

/* Fundere reinserenda quorum offset < ante */
interior vacuum
_reinserenda_fundere (SilvaScriptor* st, s32 ante)
{
    si (st->reinserenda == NIHIL)
    {
        redde;
    }
    dum (st->reinserenda_index < silva_xar_numerus(st->reinserenda))
    {
        SilvaReinserendum* r = (SilvaReinserendum*)silva_xar_obtinere(
            st->reinserenda, st->reinserenda_index);

        si (r == NIHIL || r->offset >= ante)
        {
            frange;
        }
        _laminam_emittere(st, r->lamina);
        st->reinserenda_index++;
    }
}

/* ==================================================
 * Emissio lexematis
 * ================================================== */

interior vacuum
_lexema_scribere (SilvaScriptor* st, SilvaToken* token)
{
    SilvaToken* radix;
    b32 impurum;

    si (!st->successus || token == NIHIL)
    {
        redde;
    }

    si (token->origo.genus == SILVA_ORIGO_FONS)
    {
        si (st->fons_index >= ZEPHYRUM
            && token->fons_index != st->fons_index)
        {
            /* plagulae alienae (limes includendi): octeti eius
             * reconstructioni SUAE pertinent; linea #include hic
             * reinseritur */
            redde;
        }
        _reinserenda_fundere(st, token->byte_offset);
        silva_lexema_emittere_in(st->aed, token);
        st->fons_ultimus = token->fons_index;
        st->emissum_usque = token->byte_offset + (s32)token->longitudo;
        redde;
    }

    /* Limes expansionis: stratum 0 emittitur, non lexema expansum */
    si (st->expansio == NIHIL)
    {
        _fractura(st, "lexema expansum sine contextu expansionis",
            NIHIL);
        redde;
    }
    impurum = FALSUM;
    radix = _radix_probata(token, &impurum);
    si (impurum || radix == NIHIL)
    {
        _fractura(st, "origo pasta/chorda/api - stratum 0 non "
            "recuperabile (deferral nominatum)", NIHIL);
        redde;
    }
    si (st->fons_index >= ZEPHYRUM
        && radix->fons_index != st->fons_index)
    {
        redde;  /* expansio in plagula aliena */
    }

    /* intra unitatem iam emissam? (corpus multi-lexematis eiusdem
     * invocationis; lexemata ex argumentis intra extentum) */
    si (radix->fons_index == st->fons_ultimus
        && radix->byte_offset < st->emissum_usque)
    {
        redde;
    }

    {
        SilvaXar* extentum = _extentum_quaerere(st->expansio, radix);

        si (extentum == NIHIL)
        {
            /* radix intra invocationem (lexema argumenti - via
             * CHORDA/stringificatio): extentum per continentiam */
            extentum = _extentum_continens(st->expansio, radix);
        }
        si (extentum != NIHIL && silva_xar_numerus(extentum) > ZEPHYRUM)
        {
            /* invocatio functio-similis: [nomen..')'] lexematim -
             * fusio reinserendorum INTRA extentum manet possibilis
             * (directiva intra argumenta) */
            SilvaToken* t = NIHIL;
            i32 k;

            per (k = ZEPHYRUM; k < silva_xar_numerus(extentum); k++)
            {
                t = *(SilvaToken**)silva_xar_obtinere(extentum, k);
                _reinserenda_fundere(st, t->byte_offset);
                silva_lexema_emittere_in(st->aed, t);
            }
            st->fons_ultimus = radix->fons_index;
            st->emissum_usque = t->byte_offset + (s32)t->longitudo;
        }
        alioquin
        {
            /* obiectum-simile: lexema invocationis solum */
            _reinserenda_fundere(st, radix->byte_offset);
            silva_lexema_emittere_in(st->aed, radix);
            st->fons_ultimus = radix->fons_index;
            st->emissum_usque = radix->byte_offset
                + (s32)radix->longitudo;
        }
    }
}

/* ==================================================
 * Ambulatio
 * ================================================== */

interior vacuum
_ambiguum_scribere (SilvaScriptor* st, constans SilvaNodus* nodus)
{
    SilvaValor interp;
    SilvaValor canonica;
    SilvaValor* electa;

    si ((i32)st->locus_interpretationum >= nodus->numerus_locorum
        || (i32)st->locus_canonicae >= nodus->numerus_locorum)
    {
        _fractura(st, "forma ambigui extra loci nodi", nodus);
        redde;
    }
    interp = nodus->loci[st->locus_interpretationum];
    canonica = nodus->loci[st->locus_canonicae];

    si (interp.genus != SILVA_VALOR_LISTA
        || canonica.genus != SILVA_VALOR_INDEX)
    {
        _fractura(st, "forma ambigui corrupta", nodus);
        redde;
    }
    si (canonica.datum.index < ZEPHYRUM
        || canonica.datum.index
            >= (s32)silva_valor_lista_numerus(interp))
    {
        _fractura(st, "canonica extra fines interpretationum", nodus);
        redde;
    }
    electa = silva_valor_lista_obtinere(interp,
        (i32)canonica.datum.index);
    si (electa == NIHIL)
    {
        _fractura(st, "interpretatio canonica deest", nodus);
        redde;
    }
    _valorem_scribere(st, *electa);
}

interior vacuum
_nodum_scribere (SilvaScriptor* st, constans SilvaNodus* nodus)
{
    i32 k;

    si (!st->successus || nodus == NIHIL)
    {
        redde;
    }
    si (nodus->genus < ZEPHYRUM
        || nodus->genus >= (s32)st->tabularium->numerus_generum)
    {
        _fractura(st, "genus ignotum", nodus);
        redde;
    }

    /* AMBIGUUS: sola interpretatio canonica - emissio omnium
     * interpretationum octetos duplicaret (lexemata communia) */
    si (nodus->genus == st->genus_ambigui)
    {
        _ambiguum_scribere(st, nodus);
        redde;
    }

    /* Genericus: loci ordine layout. ERROR gratis (locus unicus
     * lista-token); CONDITIONALIS cum Phase 7 veniet. */
    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        _valorem_scribere(st, nodus->loci[k]);
    }
}

interior vacuum
_valorem_scribere (SilvaScriptor* st, SilvaValor valor)
{
    si (!st->successus)
    {
        redde;
    }
    commutatio (valor.genus)
    {
        casus SILVA_VALOR_NIHIL:
        casus SILVA_VALOR_INDEX:
            frange;
        casus SILVA_VALOR_TOKEN:
            _lexema_scribere(st, valor.datum.token);
            frange;
        casus SILVA_VALOR_NODUS:
            _nodum_scribere(st, valor.datum.nodus);
            frange;
        casus SILVA_VALOR_LISTA:
        {
            i32 i;
            i32 n = silva_valor_lista_numerus(valor);

            per (i = ZEPHYRUM; i < n; i++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

                si (elem != NIHIL)
                {
                    _valorem_scribere(st, *elem);
                }
            }
            frange;
        }
        ordinarius:
            _fractura(st, "signum valoris ignotum", NIHIL);
            frange;
    }
}

/* ==================================================
 * Reinserenda colligere (fontem)
 * ================================================== */

interior vacuum
_reinserendum_addere (SilvaScriptor* st, SilvaPiscina* piscina, SilvaXar* lamina)
{
    SilvaToken* primum;
    SilvaReinserendum* r;

    si (lamina == NIHIL || silva_xar_numerus(lamina) == ZEPHYRUM)
    {
        redde;
    }
    primum = *(SilvaToken**)silva_xar_obtinere(lamina, ZEPHYRUM);
    si (st->fons_index >= ZEPHYRUM
        && primum->fons_index != st->fons_index)
    {
        redde;
    }
    si (st->reinserenda == NIHIL)
    {
        st->reinserenda = silva_xar_creare(piscina,
            magnitudo(SilvaReinserendum));
    }
    r = (SilvaReinserendum*)silva_xar_addere(st->reinserenda);
    si (r != NIHIL)
    {
        r->offset = primum->byte_offset;
        r->lamina = lamina;
    }
}

/* Arbor regionum recursive: regiones NON textae lineas structurales
 * suas (rami directiva + directiva_finis - β, sim ⑦ C2) et laminas
 * crudas reinserendis dant; regiones TEXTAE omnia ex ARBORE emittunt
 * (dominus unus) - sed filiae semper visitantur (regio degradata
 * intra textam sua adhuc possidet reinserendis). */
interior vacuum
_regiones_colligere (SilvaScriptor* st, SilvaPiscina* piscina, SilvaXar* regiones)
{
    i32 i;

    si (regiones == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < silva_xar_numerus(regiones); i++)
    {
        SilvaRegio* regio = *(SilvaRegio**)silva_xar_obtinere(regiones, i);
        i32 j;

        si (regio == NIHIL) perge;
        si (!regio->est_texta && regio->rami != NIHIL)
        {
            per (j = ZEPHYRUM; j < silva_xar_numerus(regio->rami); j++)
            {
                SilvaRamus* ramus = *(SilvaRamus**)silva_xar_obtinere(
                    regio->rami, j);

                si (ramus == NIHIL) perge;
                si (ramus->directiva != NIHIL)
                {
                    _reinserendum_addere(st, piscina, ramus->directiva);
                }
                si (ramus->lexemata_cruda != NIHIL)
                {
                    _reinserendum_addere(st, piscina,
                        ramus->lexemata_cruda);
                }
            }
        }
        si (!regio->est_texta && regio->directiva_finis != NIHIL)
        {
            _reinserendum_addere(st, piscina, regio->directiva_finis);
        }
        _regiones_colligere(st, piscina, regio->filiae);
    }
}

interior s32
_reinserenda_comparare (constans vacuum* a, constans vacuum* b)
{
    constans SilvaReinserendum* ra = (constans SilvaReinserendum*)a;
    constans SilvaReinserendum* rb = (constans SilvaReinserendum*)b;

    si (ra->offset < rb->offset) redde -I;
    si (ra->offset > rb->offset) redde I;
    redde ZEPHYRUM;
}

/* ==================================================
 * Compositio fructus
 * ================================================== */

interior SilvaScriptura
_scriptura_fracta (constans character* causa)
{
    SilvaScriptura s;

    s.successus = FALSUM;
    s.textus.datum = NIHIL;
    s.textus.mensura = ZEPHYRUM;
    s.causa = causa;
    s.sedes = NIHIL;
    redde s;
}

interior b32
_scriptor_parare (SilvaScriptor* st, SilvaPiscina* piscina,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaExpansio* expansio, s32 fons_index)
{
    st->aed = silva_chorda_aedificator_creare(piscina, 1024);
    st->tabularium = tabularium;
    st->expansio = expansio;
    st->fons_index = fons_index;
    st->reinserenda = NIHIL;
    st->reinserenda_index = ZEPHYRUM;
    st->fons_ultimus = -II;
    st->emissum_usque = ZEPHYRUM;
    st->successus = VERUM;
    st->causa = NIHIL;
    st->sedes = NIHIL;
    si (st->aed == NIHIL)
    {
        redde FALSUM;
    }
    redde _formam_ambigui_invenire(st);
}

interior SilvaScriptura
_scriptura_finire (SilvaScriptor* st)
{
    SilvaScriptura s;

    s.successus = st->successus;
    s.causa = st->causa;
    s.sedes = st->sedes;
    si (st->successus)
    {
        s.textus = silva_chorda_aedificator_finire(st->aed);
    }
    alioquin
    {
        s.textus.datum = NIHIL;
        s.textus.mensura = ZEPHYRUM;
    }
    redde s;
}

/* ==================================================
 * API
 * ================================================== */

SilvaScriptura
silva_scribere_valorem (
    SilvaPiscina*                       piscina,
    SilvaValor                     valor,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaExpansio*        expansio)
{
    SilvaScriptor st;

    si (piscina == NIHIL || tabularium == NIHIL)
    {
        redde _scriptura_fracta("argumenta nulla");
    }
    si (!_scriptor_parare(&st, piscina, tabularium, expansio, -I))
    {
        redde _scriptura_fracta("tabularium sine forma ambigui");
    }
    _valorem_scribere(&st, valor);
    redde _scriptura_finire(&st);
}

SilvaScriptura
silva_scribere_nodum (
    SilvaPiscina*                       piscina,
    constans SilvaNodus*           nodus,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaExpansio*        expansio)
{
    SilvaScriptor st;

    si (piscina == NIHIL || nodus == NIHIL || tabularium == NIHIL)
    {
        redde _scriptura_fracta("argumenta nulla");
    }
    si (!_scriptor_parare(&st, piscina, tabularium, expansio, -I))
    {
        redde _scriptura_fracta("tabularium sine forma ambigui");
    }
    _nodum_scribere(&st, nodus);
    redde _scriptura_finire(&st);
}

SilvaScriptura
silva_scribere_fontem (
    SilvaPiscina*                       piscina,
    constans SilvaParsura*         parsura,
    constans SilvaRegistrumCoctum* tabularium,
    s32                            fons_index)
{
    SilvaScriptor st;

    si (piscina == NIHIL || parsura == NIHIL || tabularium == NIHIL
        || parsura->commissio == NIHIL)
    {
        redde _scriptura_fracta("argumenta nulla");
    }
    si (!_scriptor_parare(&st, piscina, tabularium, parsura->expansio,
            fons_index))
    {
        redde _scriptura_fracta("tabularium sine forma ambigui");
    }

    /* Reinserenda: lineae directivae consumptae + rami non sumpti
     * (per fontem filtrata, per offset ordinata) */
    si (parsura->directivae != NIHIL)
    {
        i32 i;

        per (i = ZEPHYRUM; i < silva_xar_numerus(parsura->directivae); i++)
        {
            _reinserendum_addere(&st, piscina,
                *(SilvaXar**)silva_xar_obtinere(parsura->directivae, i));
        }
    }
    si (parsura->expansio != NIHIL)
    {
        _regiones_colligere(&st, piscina, parsura->expansio->regiones);
    }
    si (st.reinserenda != NIHIL)
    {
        silva_xar_ordinare(st.reinserenda, _reinserenda_comparare);
    }

    _valorem_scribere(&st, parsura->commissio->radix);
    _reinserenda_fundere(&st, SILVA_SCRIBERE_OMNIA);

    /* Trivia caudae plagulae: lexema EOF. Plagulae inclusae EOF suum
     * in includenda retinent (fluxus reliquorum id abicit - sim ⑥
     * C6). */
    si (parsura->lexema_finis != NIHIL
        && (fons_index < ZEPHYRUM
            || parsura->lexema_finis->fons_index == fons_index))
    {
        _lexema_scribere(&st, parsura->lexema_finis);
    }
    alioquin si (fons_index >= ZEPHYRUM && parsura->expansio != NIHIL)
    {
        SilvaFons* fons = (SilvaFons*)silva_xar_obtinere(
            parsura->expansio->fontes, (i32)fons_index);
        vacuum* valor;

        si (fons != NIHIL && fons->via != NIHIL
            && silva_tabula_dispersa_invenire(parsura->expansio->includenda,
                   *fons->via, &valor))
        {
            SilvaIncludendum* incl = (SilvaIncludendum*)valor;

            si (incl != NIHIL && incl->lexemata != NIHIL
                && silva_xar_numerus(incl->lexemata) > ZEPHYRUM)
            {
                SilvaToken* ultimum = *(SilvaToken**)silva_xar_obtinere(
                    incl->lexemata,
                    (i32)(silva_xar_numerus(incl->lexemata) - I));

                si (ultimum != NIHIL
                    && ultimum->genus == SILVA_LEX_EOF)
                {
                    _lexema_scribere(&st, ultimum);
                }
            }
        }
    }

    redde _scriptura_finire(&st);
}
