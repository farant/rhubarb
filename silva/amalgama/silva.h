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

/* Cella praelata (<praelatio>): conflictus in tabulis DECLARATE
 * resolutus - categoria census (M2c) */
typedef struct SilvaTabPraelata {
    int status;             /* status cellae */
    int terminalis;         /* index symboli terminalis */
    int actio_retenta;      /* SilvaTabActioGenus retentum */
    int productio_remota;   /* index productionis remotae */
} SilvaTabPraelata;

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

/* Oraculum POSITIONALE (M2b): typus a SITU declarationis visibilis
 * (byte_offset lexematis nominis; 0 = praeoneratus, ubique
 * visibilis - addere sine situ = situs 0). typedef POST usum
 * ambiguitatem priorem non solvit. Situs primus servatur. */
int silva_oraculum_typum_addere_situ(SilvaOraculum* oraculum,
    SilvaChorda titulus, int situs);
int silva_oraculum_situs_typi(const SilvaOraculum* oraculum,
    SilvaChorda titulus, int* situs_out);

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

/* Uncus praecommissionis (M2b): gubernator eum (si non-NULL)
 * ante commissionem vocat, radice nondum commissa (pater absens -
 * lectio sola); datum = datum_resolutoris. Sedes registrationis
 * typorum c89. */
typedef void (*SilvaPraecommissio)(SilvaValor radix, void* datum);

typedef struct SilvaGrammatica {
    const SilvaTabulaCocta*     tabula;
    const SilvaRegistrumCoctum* tabularium;
    SilvaGLRConstructor         constructor;
    SilvaGLRFabricaAmbigui      fabrica;
    SilvaPraecommissio          praecommissio;  /* NULL licet */
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

/* (superficies sceleti retracta M2d Chunk B: grammatica c89 est
 * grammatica UNA amalgamae; sceletum in fontes/ manet ut fixum
 * generatoris) */

/* ==================================================
 * Grammatica C89 (M2, crescens per M2a..M2d) - expressiones
 * integrae cum familia furcarum typedef (localiter AMBIGUUS,
 * spina canonica; oraculum ad commissionem aut per
 * silva_recanonicare vertit). Nomina generum et locorum:
 * silva/grammatica/genera-c89.md (DECISUS) - genera per titulum
 * kebab-case in SILVA_C89_REGISTRUM ("conversio",
 * "folium-chorda", "magnitudo-typi"...).
 * ================================================== */

extern const SilvaTabulaCocta     SILVA_C89_TABULA;
extern const SilvaRegistrumCoctum SILVA_C89_REGISTRUM;

/* Cellae praelatae grammaticae c89 (census: superficies
 * interrogabilis - codex est basis datorum) */
extern const SilvaTabPraelata     SILVA_C89_PRAELATAE[];

/* Regio GENERATA (silva/generare.sh eam scribit - inter signa NE
 * MANU EDITES; cetera capitis manu scripta manent) */
/* >>> GENERATUM (silva_coquere): superficies publica SILVA_C89 >>> */
/* Genera nodorum (GENERATA - constantes compilationis; custos
 * communis: copia interna in amalgamato cadit) */
#ifndef SILVA_C89_GENERA_CUSTOS
#define SILVA_C89_GENERA_CUSTOS
enum {
    SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS = 0,
    SILVA_C89_GENUS_SENTENTIA_VACUA = 1,
    SILVA_C89_GENUS_CORPUS = 2,
    SILVA_C89_GENUS_SI = 3,
    SILVA_C89_GENUS_DUM = 4,
    SILVA_C89_GENUS_FAC_DUM = 5,
    SILVA_C89_GENUS_PER = 6,
    SILVA_C89_GENUS_PER_CLAUSULA = 7,
    SILVA_C89_GENUS_COMMUTATIO = 8,
    SILVA_C89_GENUS_CASUS = 9,
    SILVA_C89_GENUS_ORDINARIUS = 10,
    SILVA_C89_GENUS_TITULATUM = 11,
    SILVA_C89_GENUS_SALTA = 12,
    SILVA_C89_GENUS_FRANGE = 13,
    SILVA_C89_GENUS_PERGE = 14,
    SILVA_C89_GENUS_REDDE = 15,
    SILVA_C89_GENUS_DECLARATIO = 16,
    SILVA_C89_GENUS_TYPUS_NOMINATUS = 17,
    SILVA_C89_GENUS_TYPUS_PRIMITIVUS = 18,
    SILVA_C89_GENUS_DECLARATOR_INITIATUS = 19,
    SILVA_C89_GENUS_CONGERIES = 20,
    SILVA_C89_GENUS_STRUCTURA = 21,
    SILVA_C89_GENUS_UNIO = 22,
    SILVA_C89_GENUS_MEMBRUM = 23,
    SILVA_C89_GENUS_CAMPUS = 24,
    SILVA_C89_GENUS_ENUMERATIO = 25,
    SILVA_C89_GENUS_ENUMERATOR = 26,
    SILVA_C89_GENUS_DECLARATOR_MONSTRATOR = 27,
    SILVA_C89_GENUS_DECLARATOR_TITULUS = 28,
    SILVA_C89_GENUS_PARENTHESIS = 29,
    SILVA_C89_GENUS_DECLARATOR_ACIEI = 30,
    SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS = 31,
    SILVA_C89_GENUS_PARAMETRUM = 32,
    SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS = 33,
    SILVA_C89_GENUS_VIRGULA = 34,
    SILVA_C89_GENUS_ASSIGNATIO = 35,
    SILVA_C89_GENUS_TERNARIUS = 36,
    SILVA_C89_GENUS_BINARIUM = 37,
    SILVA_C89_GENUS_CONVERSIO = 38,
    SILVA_C89_GENUS_UNARIUM = 39,
    SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS = 40,
    SILVA_C89_GENUS_MAGNITUDO_TYPI = 41,
    SILVA_C89_GENUS_SPECIES_TYPI = 42,
    SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS = 43,
    SILVA_C89_GENUS_SUBSCRIPTIO = 44,
    SILVA_C89_GENUS_VOCATIO = 45,
    SILVA_C89_GENUS_ACCESSUS = 46,
    SILVA_C89_GENUS_POSTCREMENTUM = 47,
    SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR = 48,
    SILVA_C89_GENUS_FOLIUM_INTEGER = 49,
    SILVA_C89_GENUS_FOLIUM_FLUITANS = 50,
    SILVA_C89_GENUS_FOLIUM_CHARACTER = 51,
    SILVA_C89_GENUS_FOLIUM_CHORDA = 52,
    SILVA_C89_GENUS_ERROR = 53,
    SILVA_C89_GENUS_AMBIGUUS = 54,
    SILVA_C89_GENUS_CONDITIONALIS = 55,
    SILVA_C89_GENUS_RAMUS_SUMPTUS = 56,
    SILVA_C89_GENUS_RAMUS_OMISSUS = 57
};
#endif /* SILVA_C89_GENERA_CUSTOS */

/* Accessores locorum (GENERATI - unus per locum; nodus NULL aut
 * generis alieni -> valor generis SILVA_VALOR_NIHIL) */
SilvaValor silva_c89_definitio_functionis_specificatores(const SilvaNodus* nodus);
SilvaValor silva_c89_definitio_functionis_declarator(const SilvaNodus* nodus);
SilvaValor silva_c89_definitio_functionis_declarationes_kr(const SilvaNodus* nodus);
SilvaValor silva_c89_definitio_functionis_corpus(const SilvaNodus* nodus);
SilvaValor silva_c89_sententia_vacua_tok_terminator(const SilvaNodus* nodus);
SilvaValor silva_c89_corpus_tok_aperta(const SilvaNodus* nodus);
SilvaValor silva_c89_corpus_elementa(const SilvaNodus* nodus);
SilvaValor silva_c89_corpus_tok_clausa(const SilvaNodus* nodus);
SilvaValor silva_c89_si_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_si_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_si_conditio(const SilvaNodus* nodus);
SilvaValor silva_c89_si_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_si_consequens(const SilvaNodus* nodus);
SilvaValor silva_c89_si_tok_alioquin(const SilvaNodus* nodus);
SilvaValor silva_c89_si_alioquin(const SilvaNodus* nodus);
SilvaValor silva_c89_dum_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_dum_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_dum_conditio(const SilvaNodus* nodus);
SilvaValor silva_c89_dum_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_dum_corpus(const SilvaNodus* nodus);
SilvaValor silva_c89_fac_dum_tok_fac(const SilvaNodus* nodus);
SilvaValor silva_c89_fac_dum_corpus(const SilvaNodus* nodus);
SilvaValor silva_c89_fac_dum_tok_dum(const SilvaNodus* nodus);
SilvaValor silva_c89_fac_dum_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_fac_dum_conditio(const SilvaNodus* nodus);
SilvaValor silva_c89_fac_dum_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_fac_dum_tok_terminator(const SilvaNodus* nodus);
SilvaValor silva_c89_per_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_per_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_per_clausula(const SilvaNodus* nodus);
SilvaValor silva_c89_per_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_per_corpus(const SilvaNodus* nodus);
SilvaValor silva_c89_per_clausula_initium(const SilvaNodus* nodus);
SilvaValor silva_c89_per_clausula_tok_terminator_i(const SilvaNodus* nodus);
SilvaValor silva_c89_per_clausula_conditio(const SilvaNodus* nodus);
SilvaValor silva_c89_per_clausula_tok_terminator_ii(const SilvaNodus* nodus);
SilvaValor silva_c89_per_clausula_passus(const SilvaNodus* nodus);
SilvaValor silva_c89_commutatio_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_commutatio_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_commutatio_discrimen(const SilvaNodus* nodus);
SilvaValor silva_c89_commutatio_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_commutatio_corpus(const SilvaNodus* nodus);
SilvaValor silva_c89_casus_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_casus_valor(const SilvaNodus* nodus);
SilvaValor silva_c89_casus_tok_colon(const SilvaNodus* nodus);
SilvaValor silva_c89_casus_sententiae(const SilvaNodus* nodus);
SilvaValor silva_c89_ordinarius_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_ordinarius_tok_colon(const SilvaNodus* nodus);
SilvaValor silva_c89_ordinarius_sententiae(const SilvaNodus* nodus);
SilvaValor silva_c89_titulatum_tok_titulus(const SilvaNodus* nodus);
SilvaValor silva_c89_titulatum_tok_colon(const SilvaNodus* nodus);
SilvaValor silva_c89_titulatum_sententia(const SilvaNodus* nodus);
SilvaValor silva_c89_salta_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_salta_tok_destinatio(const SilvaNodus* nodus);
SilvaValor silva_c89_salta_tok_terminator(const SilvaNodus* nodus);
SilvaValor silva_c89_frange_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_frange_tok_terminator(const SilvaNodus* nodus);
SilvaValor silva_c89_perge_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_perge_tok_terminator(const SilvaNodus* nodus);
SilvaValor silva_c89_redde_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_redde_valor(const SilvaNodus* nodus);
SilvaValor silva_c89_redde_tok_terminator(const SilvaNodus* nodus);
SilvaValor silva_c89_declaratio_specificatores(const SilvaNodus* nodus);
SilvaValor silva_c89_declaratio_declaratores(const SilvaNodus* nodus);
SilvaValor silva_c89_declaratio_tok_terminator(const SilvaNodus* nodus);
SilvaValor silva_c89_typus_nominatus_tok_titulus(const SilvaNodus* nodus);
SilvaValor silva_c89_typus_primitivus_tok_verba(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_initiatus_declarator(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_initiatus_tok_operator(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_initiatus_initiator(const SilvaNodus* nodus);
SilvaValor silva_c89_congeries_tok_aperta(const SilvaNodus* nodus);
SilvaValor silva_c89_congeries_elementa(const SilvaNodus* nodus);
SilvaValor silva_c89_congeries_tok_clausa(const SilvaNodus* nodus);
SilvaValor silva_c89_structura_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_structura_tok_titulus(const SilvaNodus* nodus);
SilvaValor silva_c89_structura_tok_aperta(const SilvaNodus* nodus);
SilvaValor silva_c89_structura_membra(const SilvaNodus* nodus);
SilvaValor silva_c89_structura_tok_clausa(const SilvaNodus* nodus);
SilvaValor silva_c89_unio_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_unio_tok_titulus(const SilvaNodus* nodus);
SilvaValor silva_c89_unio_tok_aperta(const SilvaNodus* nodus);
SilvaValor silva_c89_unio_membra(const SilvaNodus* nodus);
SilvaValor silva_c89_unio_tok_clausa(const SilvaNodus* nodus);
SilvaValor silva_c89_membrum_specificatores(const SilvaNodus* nodus);
SilvaValor silva_c89_membrum_declaratores(const SilvaNodus* nodus);
SilvaValor silva_c89_membrum_tok_terminator(const SilvaNodus* nodus);
SilvaValor silva_c89_campus_declarator(const SilvaNodus* nodus);
SilvaValor silva_c89_campus_tok_colon(const SilvaNodus* nodus);
SilvaValor silva_c89_campus_latitudo(const SilvaNodus* nodus);
SilvaValor silva_c89_enumeratio_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_enumeratio_tok_titulus(const SilvaNodus* nodus);
SilvaValor silva_c89_enumeratio_tok_aperta(const SilvaNodus* nodus);
SilvaValor silva_c89_enumeratio_enumeratores(const SilvaNodus* nodus);
SilvaValor silva_c89_enumeratio_tok_clausa(const SilvaNodus* nodus);
SilvaValor silva_c89_enumerator_tok_titulus(const SilvaNodus* nodus);
SilvaValor silva_c89_enumerator_tok_operator(const SilvaNodus* nodus);
SilvaValor silva_c89_enumerator_valor(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_monstrator_tok_stella(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_monstrator_qualificatores(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_monstrator_internum(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_titulus_tok_titulus(const SilvaNodus* nodus);
SilvaValor silva_c89_parenthesis_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_parenthesis_internum(const SilvaNodus* nodus);
SilvaValor silva_c89_parenthesis_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_aciei_internum(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_aciei_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_aciei_mensura(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_aciei_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_functionis_internum(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_functionis_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_functionis_parametra(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_functionis_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_parametrum_specificatores(const SilvaNodus* nodus);
SilvaValor silva_c89_parametrum_declarator(const SilvaNodus* nodus);
SilvaValor silva_c89_sententia_expressionis_expressio(const SilvaNodus* nodus);
SilvaValor silva_c89_sententia_expressionis_tok_terminator(const SilvaNodus* nodus);
SilvaValor silva_c89_virgula_sinister(const SilvaNodus* nodus);
SilvaValor silva_c89_virgula_tok_operator(const SilvaNodus* nodus);
SilvaValor silva_c89_virgula_dexter(const SilvaNodus* nodus);
SilvaValor silva_c89_assignatio_sinister(const SilvaNodus* nodus);
SilvaValor silva_c89_assignatio_tok_operator(const SilvaNodus* nodus);
SilvaValor silva_c89_assignatio_dexter(const SilvaNodus* nodus);
SilvaValor silva_c89_ternarius_conditio(const SilvaNodus* nodus);
SilvaValor silva_c89_ternarius_tok_quaestio(const SilvaNodus* nodus);
SilvaValor silva_c89_ternarius_verum(const SilvaNodus* nodus);
SilvaValor silva_c89_ternarius_tok_colon(const SilvaNodus* nodus);
SilvaValor silva_c89_ternarius_falsum(const SilvaNodus* nodus);
SilvaValor silva_c89_binarium_sinister(const SilvaNodus* nodus);
SilvaValor silva_c89_binarium_tok_operator(const SilvaNodus* nodus);
SilvaValor silva_c89_binarium_dexter(const SilvaNodus* nodus);
SilvaValor silva_c89_conversio_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_conversio_typus(const SilvaNodus* nodus);
SilvaValor silva_c89_conversio_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_conversio_internum(const SilvaNodus* nodus);
SilvaValor silva_c89_unarium_tok_operator(const SilvaNodus* nodus);
SilvaValor silva_c89_unarium_internum(const SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_expressionis_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_expressionis_internum(const SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_typi_tok_verbum(const SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_typi_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_typi_typus(const SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_typi_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_species_typi_specificatores(const SilvaNodus* nodus);
SilvaValor silva_c89_species_typi_declarator(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_abstractus_tok_stella(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_abstractus_qualificatores(const SilvaNodus* nodus);
SilvaValor silva_c89_declarator_abstractus_internum(const SilvaNodus* nodus);
SilvaValor silva_c89_subscriptio_basis(const SilvaNodus* nodus);
SilvaValor silva_c89_subscriptio_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_subscriptio_index(const SilvaNodus* nodus);
SilvaValor silva_c89_subscriptio_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_vocatio_functio(const SilvaNodus* nodus);
SilvaValor silva_c89_vocatio_tok_apertum(const SilvaNodus* nodus);
SilvaValor silva_c89_vocatio_argumenta(const SilvaNodus* nodus);
SilvaValor silva_c89_vocatio_tok_clausum(const SilvaNodus* nodus);
SilvaValor silva_c89_accessus_basis(const SilvaNodus* nodus);
SilvaValor silva_c89_accessus_tok_operator(const SilvaNodus* nodus);
SilvaValor silva_c89_accessus_tok_titulus(const SilvaNodus* nodus);
SilvaValor silva_c89_postcrementum_internum(const SilvaNodus* nodus);
SilvaValor silva_c89_postcrementum_tok_operator(const SilvaNodus* nodus);
SilvaValor silva_c89_folium_identificator_tok_valor(const SilvaNodus* nodus);
SilvaValor silva_c89_folium_integer_tok_valor(const SilvaNodus* nodus);
SilvaValor silva_c89_folium_fluitans_tok_valor(const SilvaNodus* nodus);
SilvaValor silva_c89_folium_character_tok_valor(const SilvaNodus* nodus);
SilvaValor silva_c89_folium_chorda_tok_valor(const SilvaNodus* nodus);
SilvaValor silva_c89_error_tokens(const SilvaNodus* nodus);
SilvaValor silva_c89_ambiguus_interpretationes(const SilvaNodus* nodus);
SilvaValor silva_c89_ambiguus_canonica(const SilvaNodus* nodus);
SilvaValor silva_c89_conditionalis_rami(const SilvaNodus* nodus);
SilvaValor silva_c89_conditionalis_finis(const SilvaNodus* nodus);
SilvaValor silva_c89_ramus_sumptus_directiva(const SilvaNodus* nodus);
SilvaValor silva_c89_ramus_sumptus_contentum(const SilvaNodus* nodus);
SilvaValor silva_c89_ramus_sumptus_conditio_id(const SilvaNodus* nodus);
SilvaValor silva_c89_ramus_omissus_directiva(const SilvaNodus* nodus);
SilvaValor silva_c89_ramus_omissus_cruda(const SilvaNodus* nodus);
SilvaValor silva_c89_ramus_omissus_conditio_id(const SilvaNodus* nodus);
/* <<< GENERATUM (silva_coquere): finis superficiei publicae SILVA_C89 <<< */

SilvaValor silva_c89_construere(SilvaPiscina* piscina,
    int productio, const SilvaValor* valores);
SilvaValor silva_c89_ambiguum_fabricare(SilvaPiscina* piscina,
    SilvaValor interpretationes, int canonica);

/* ==================================================
 * Anima semantica c89 (M2b, simulatio X): saltatio oraculi.
 * silva_c89_parsare = vocatio una (registratio typedef per
 * uncum praecommissionis + resolutor verus ad commissionem +
 * politica spinae canonicae post). oraculum NULL licet (vacuum
 * intus); oraculum datum typos plagulae ACCIPIT.
 * ================================================== */

extern const SilvaGrammatica SILVA_C89_GRAMMATICA;

SilvaParsura* silva_c89_parsare(SilvaPiscina* piscina,
    const char* via, const char* fons, unsigned int mensura,
    SilvaOraculum* oraculum);

/* Eadem cum contextu hospitis (lexica latina, includenda
 * praebita) - grammatica c89 expansione pascitur (M2d) */
SilvaParsura* silva_c89_parsare_cum_contextu(
    SilvaPiscina* piscina, const SilvaContextus* contextus,
    const char* via, const char* fons, unsigned int mensura,
    SilvaOraculum* oraculum);

/* Resolutor verus (filtrum combinationis X10 + oraculum
 * positionale X3 + retentio ignotorum) - pro machinatione
 * propria (silva_parsare/silva_recanonicare directis) */
void silva_c89_resolutor(const SilvaNodus* ambiguum,
    const SilvaOraculum* oraculum, void* datum,
    SilvaResolutioResponsum* responsum);

/* Registratio typorum radice (etiam intra ramos sumptos;
 * elementa ambigua praetermissa) */
void silva_c89_typos_registrare(SilvaValor radix,
    SilvaOraculum* oraculum);

/* Lexema tituli declaratoris (catena persecuta; NULL si
 * abstractus) - ambulator vistae declarationum */
SilvaToken* silva_c89_declaratoris_titulus(
    const SilvaNodus* declarator);

/* Politica canonicae super superstites; numerum versorum reddit */
unsigned int silva_c89_politicam_imponere(SilvaCommissio* commissio,
    SilvaOraculum* oraculum);

/* Vista declarationum: ordines TOC (unus per declaratorem; nudae
 * per tag; rami sumpti visitantur; ambigua per spinam canonicam) */
typedef struct SilvaDeclaratioVista {
    const char* genus;    /* titulus generis (kebab, e registro) */
    SilvaChorda titulus;  /* nomen; mensura 0 = anonymum */
    int         linea;
    int         situs;    /* byte_offset */
    int est_typedef;          /* TYPEDEF in specificatoribus */
    int fons_index;           /* plagula lexematis tituli; -1 si
                               * abest (filtrum fons_princeps) */
} SilvaDeclaratioVista;

unsigned int silva_c89_declarationes_numerus(
    const SilvaParsura* parsura);
int silva_c89_declaratio_vista(const SilvaParsura* parsura,
    unsigned int index, SilvaDeclaratioVista* vista);

/* Subscriptio functionis (M2c): specificatores + declarator
 * octetim ex arbore (sine corpore) - textus ordinis TOC.
 * Ordo vistae ad indicem definitio-functionis sit. */
SilvaScriptura silva_c89_functionis_subscriptio(
    SilvaPiscina* piscina, const SilvaParsura* parsura,
    unsigned int index);

/* ==================================================
 * Quaestio (QA): selectores CSS-similes super arbores commissas
 * (consilium: project-specs/silva-quaestio-design.md, DECISUS).
 * Tags = tituli generum registri (kebab); combinatores spatium
 * (descendens) et > (filius); unio virgula; * universalis. Tag
 * ignotum aut selector malformatus = NULL + causa (fractura
 * CLARA). Ambigua: descensus per lectionem canonicam solam;
 * involucrum ipsum congruibile (tag "ambiguus"); nodi ERROR
 * congruibiles (tag "error"). Arbores COMMISSAE exspectantur
 * (pater fixus).
 * ================================================== */

typedef struct SilvaQuaestio SilvaQuaestio;  /* compilata (opaca) */

typedef struct SilvaQuaestioResultatum {
    const SilvaNodus* nodus;   /* subiectum congruens */
} SilvaQuaestioResultatum;

/* Selector -> quaestio compilata (reusabilis trans arbores).
 * NULL + *causa_out (litterae staticae; NULL licet) si malformatus
 * aut tag ignotum. */
SilvaQuaestio* silva_quaestio_compilare(SilvaPiscina* piscina,
    const SilvaRegistrumCoctum* tabularium, const char* selector,
    const char** causa_out);

/* Omnes nodi congruentes sub radice (commissio->radix directa).
 * SilvaXar de SilvaQuaestioResultatum (valore) in piscinam. */
SilvaXar* silva_quaestio_exsequi(const SilvaQuaestio* quaestio,
    SilvaValor radix, SilvaPiscina* piscina);

/* Nodusne unus catenae alicui congruit (maiores per patrem)? */
int silva_quaestio_congruit(const SilvaQuaestio* quaestio,
    const SilvaNodus* nodus);

/* ==================================================
 * Semantica C89 (M0a): typi + scopi + forma + index — tabulae
 * parallelae super arbores commissas (consilium:
 * project-specs/silva-semantica-design.md DECISUS). Ansa typi =
 * monstrator UNUS: derivati internantur (aequalitas monstratorum
 * EST identitas), tags nominales. Forma arm64 LP64 Apple
 * (long double = 8; long long = extensio recepta — s64!),
 * VERIFICATA contra clang per haruspicem (4,801 assertiones).
 * Diagnostica venenata: TYPUS_C89_ERROR absorbet sine cascata.
 * ================================================== */

typedef enum {
    TYPUS_C89_PRIMITIVUS = 0,
    TYPUS_C89_MONSTRATOR,
    TYPUS_C89_ACIES,
    TYPUS_C89_FUNCTIO,
    TYPUS_C89_STRUCTURA,
    TYPUS_C89_UNIO,
    TYPUS_C89_ENUMERATUS,
    TYPUS_C89_QUALIFICATUS,
    TYPUS_C89_ERROR
} TypusC89Genus;

typedef enum {
    PRIMITIVUM_VACUUM = 0,
    PRIMITIVUM_CHARACTER,
    PRIMITIVUM_CHARACTER_SIGNATUM,
    PRIMITIVUM_CHARACTER_INSIGNATUM,
    PRIMITIVUM_BREVIS,
    PRIMITIVUM_BREVIS_INSIGNATUM,
    PRIMITIVUM_INTEGER,
    PRIMITIVUM_INTEGER_INSIGNATUM,
    PRIMITIVUM_LONGUS,
    PRIMITIVUM_LONGUS_INSIGNATUM,
    PRIMITIVUM_LONGUS_LONGUS,
    PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM,
    PRIMITIVUM_FLUITANS,
    PRIMITIVUM_DUPLEX,
    PRIMITIVUM_DUPLEX_LONGUS,
    PRIMITIVUM_NUMERUS
} TypusC89Primitivum;

#define QUALIS_CONSTANS   1
#define QUALIS_VOLATILIS  2

typedef struct TypusC89 TypusC89;

typedef struct {
    SilvaChorda  titulus;      /* copia; mensura 0 = anonymum */
    TypusC89*    typus;
    unsigned int offset;       /* octeti; a forma computatus */
    int          est_campus;   /* bitfield (forma tag parcata) */
} TypusC89Membrum;

struct TypusC89 {
    int genus;                 /* TypusC89Genus */
    int ex_systemate;          /* provenientia (haruspex) */

    int forma_computata;
    int in_computatione;
    unsigned int magnitudo_octetorum;
    unsigned int ordinatio;

    union {
        int primitivum;        /* TypusC89Primitivum */
        struct {
            TypusC89* internum;
        } monstrator;
        struct {
            TypusC89* elementum;
            int       numerus;      /* -1 = incompleta */
        } acies;
        struct {
            TypusC89*    reditus;
            TypusC89**   parametra;
            unsigned int numerus_parametrorum;
            int          est_variadica;
            int          est_prototypata;
        } functio;
        struct {
            SilvaChorda       titulus;
            int               completa;
            TypusC89Membrum*  membra;
            unsigned int      numerus_membrorum;
            const SilvaNodus* declarans;
            int               habet_campos;
        } tag;
        struct {
            SilvaChorda       titulus;
            int               completa;
            const SilvaNodus* declarans;
        } enumeratus;
        struct {
            TypusC89*    internum;
            unsigned int quales;
        } qualificatus;
    } datum;
};

typedef enum {
    SYMBOLUM_VARIABILE = 0,
    SYMBOLUM_FUNCTIO,
    SYMBOLUM_TYPEDEF,
    SYMBOLUM_CONSTANS,
    SYMBOLUM_PARAMETRUM
} SemanticaSymbolumGenus;

#define REPOSITIO_STATICA   1
#define REPOSITIO_EXTERNA   2
#define REPOSITIO_AUTOMATA  4
#define REPOSITIO_REGISTRI  8

typedef struct {
    int               genus;        /* SemanticaSymbolumGenus */
    SilvaChorda       titulus;
    TypusC89*         typus;
    long long         valor;        /* CONSTANS solum */
    unsigned int      repositio;
    unsigned int      profunditas;  /* 0 = scopus fili */
    int               ex_systemate;
    int               est_implicitum; /* extern int implicitum
                                       * synthetizatum (M0b C) */
    const SilvaNodus* declarans;    /* implicitis: sedes vocationis */
    SilvaToken*       lexema;
} SemanticaSymbolum;

typedef struct {
    const SilvaNodus* nodus;
    const char*       causa;
} SemanticaDiagnosticum;

typedef struct SilvaSemantica SilvaSemantica;   /* opaca */

/* Analysis totius fili super arborem commissam (ordine fontis,
 * registra-ante-usum; rami sumpti; canonica ambiguorum; K&R
 * parca). Numquam ruit. */
SilvaSemantica* silva_c89_semantica_analysare(SilvaPiscina* piscina,
    const SilvaParsura* parsura);

/* Ut supra, symbola systematis prius harvestata (systema =
 * parsura systema_c89.h; NULL licet = simplex) */
SilvaSemantica* silva_c89_semantica_analysare_cum_systemate(
    SilvaPiscina* piscina, const SilvaParsura* parsura,
    const SilvaParsura* systema);

/* Symbola in oraculum fundere (typedef -> typi; cetera ->
 * NON-typi, oraculum trivalens) - praeoneratio et clausura */
unsigned int silva_c89_semantica_oraculum_augere(
    const SilvaSemantica* sem, SilvaOraculum* oraculum);

/* Quot ambigua retenta adhuc INDECISA (voca
 * silva_oraculum_responsa_vacare ante pro statu recenti) */
unsigned int silva_c89_ambigua_indecisa_numerare(
    const SilvaCommissio* commissio, const SilvaOraculum* oraculum);

/* Quaestio symboli (a scopo fili post analysem) */
SemanticaSymbolum* silva_c89_symbolum_invenire(SilvaSemantica* sem,
    SilvaChorda titulus);

/* Index: omnia symbola creata (cum profunditate) - saltuarius
 * saltus typorum = consumptor primus nominatus */
unsigned int silva_c89_symbola_numerus(const SilvaSemantica* sem);
const SemanticaSymbolum* silva_c89_symbolum_per_indicem(
    const SilvaSemantica* sem, unsigned int index);

unsigned int silva_c89_diagnostica_numerus(
    const SilvaSemantica* sem);
const SemanticaDiagnosticum* silva_c89_diagnosticum_per_indicem(
    const SilvaSemantica* sem, unsigned int index);

/* Forma pigra: VERUM si magnitudo/ordinatio (et offsets) validae */
int silva_c89_formam_computare(SilvaSemantica* sem,
    TypusC89* typus);

/* Singuli primitivi (aequalitas monstratorum = identitas) */
TypusC89* silva_c89_typus_primitivum(SilvaSemantica* sem,
    int primitivum);

#endif /* SILVA_H */
