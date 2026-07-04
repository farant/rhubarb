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

#endif /* SILVA_H */
