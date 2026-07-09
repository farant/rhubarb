/* officina.h - Medulla: IR registrorum linearis (interfacies publica)
 *
 * MANU SCRIPTUM, C89 vanilla - definitiones latinae numquam trans
 * limitem publicum transeunt. Structurae PELLUCIDAE: campi ordine
 * EXACTO fontium (definitiones hae solae in TU amalgamato - deriva
 * compilationem frangit). Vocabularium SIGNATUM:
 * officina/vocabularium-medullae.md.
 *
 * Regulae typorum domus: i32 = unsigned int EXACTE, s32 = int,
 * s64 = long long, f64 = double, b32 = int, chorda = OfficinaChorda
 * (mensura + datum, NON NUL-terminata).
 *
 * Provenientia: instructiones origo (SilvaNodus*) ferunt; hospites
 * qui silva.h quoque includunt eundem tag vident.
 */

#ifndef OFFICINA_H
#define OFFICINA_H

#include <stddef.h>

struct SilvaNodus;   /* provenientia; silva.h eundem tag possidet */

/* ==================================================
 * Piscina (arena vendicata) - creare et destruere solum
 * ================================================== */

typedef struct OfficinaPiscina OfficinaPiscina;

OfficinaPiscina* officina_piscina_generare_dynamicum(
    const char* titulus, size_t mensura_alvei_initia);
void officina_piscina_destruere(OfficinaPiscina* piscina);

/* ==================================================
 * Chorda (visus octetorum, NON NUL-terminatus) + Xar (tabula)
 * ================================================== */

typedef struct OfficinaChorda {
    unsigned int   mensura;
    unsigned char* datum;
} OfficinaChorda;

/* PELLUCIDA - campi ordine EXACTO include/xar.h (definitio haec
 * sola in TU amalgamato); 64 = XAR_MAXIMUS_SEGMENTORUM, 32 =
 * mensura tituli */
typedef struct OfficinaXar {
    unsigned int     numerus_elementorum;
    unsigned int     magnitudo_elementi;
    unsigned int     magnitudo_primi;
    unsigned int     numerus_segmentorum;
    unsigned int     capacitas_totalis;
    unsigned int     vexilla;
    OfficinaPiscina* piscina;
    void*            segmenta[64];
    char             titulus[32];
} OfficinaXar;

unsigned int officina_xar_numerus(const OfficinaXar* xar);
void* officina_xar_obtinere(const OfficinaXar* xar,
    unsigned int index);
void* officina_xar_obtinere_s(const OfficinaXar* xar, int index);

/* ==================================================
 * Typi operationum (suffixa: conventio domus i/s/f)
 * ================================================== */

typedef enum {
    MEDULLA_TYPUS_I8 = 0,
    MEDULLA_TYPUS_I16,
    MEDULLA_TYPUS_I32,
    MEDULLA_TYPUS_I64,
    MEDULLA_TYPUS_S8,
    MEDULLA_TYPUS_S16,
    MEDULLA_TYPUS_S32,
    MEDULLA_TYPUS_S64,
    MEDULLA_TYPUS_F32,
    MEDULLA_TYPUS_F64,
    MEDULLA_TYPUS_NUMERUS
} MedullaTypus;

#define MEDULLA_TYPUS_NIHIL (-1)

/* ==================================================
 * Operationes (vocabularium-medullae.md - SIGNATUM)
 * ================================================== */

typedef enum {
    MEDULLA_OP_MOVERE = 0,
    MEDULLA_OP_ADDERE,
    MEDULLA_OP_SUBTRAHERE,
    MEDULLA_OP_MULTIPLICARE,
    MEDULLA_OP_DIVIDERE,
    MEDULLA_OP_RESIDUUM,
    MEDULLA_OP_NEGARE,
    MEDULLA_OP_ET,
    MEDULLA_OP_VEL,
    MEDULLA_OP_AUT,
    MEDULLA_OP_COMPLEMENTUM,
    MEDULLA_OP_SINISTRORSUM,
    MEDULLA_OP_DEXTRORSUM,
    MEDULLA_OP_AEQUALIS,
    MEDULLA_OP_INAEQUALIS,
    MEDULLA_OP_MINOR,
    MEDULLA_OP_MINOR_AEQUALIS,
    MEDULLA_OP_MAIOR,
    MEDULLA_OP_MAIOR_AEQUALIS,
    MEDULLA_OP_EXTENDERE,
    MEDULLA_OP_AMPUTARE,
    MEDULLA_OP_FLUITARE,
    MEDULLA_OP_INTEGRARE,
    MEDULLA_OP_DILATARE,
    MEDULLA_OP_ARTARE,
    MEDULLA_OP_LEGERE,
    MEDULLA_OP_SCRIBERE,
    MEDULLA_OP_ARCA,
    MEDULLA_OP_LOCUS,
    MEDULLA_OP_COPIA,
    MEDULLA_OP_SALIRE,
    MEDULLA_OP_RAMUS,
    MEDULLA_OP_REDDE,
    MEDULLA_OP_VOCARE,
    MEDULLA_OP_SISTERE,
    MEDULLA_OP_NUMERUS
} MedullaOp;

/* ==================================================
 * Operandum (signatum, 16 octeti)
 * ================================================== */

typedef enum {
    MEDULLA_OPERANDUM_NIHIL = 0,
    MEDULLA_OPERANDUM_REGISTRUM,
    MEDULLA_OPERANDUM_IMMEDIATUM,
    MEDULLA_OPERANDUM_IMMEDIATUM_F,
    MEDULLA_OPERANDUM_SYMBOLUM,
    MEDULLA_OPERANDUM_BLOCCUS,
    MEDULLA_OPERANDUM_CAUSA
} MedullaOperandumGenus;

typedef struct {
    int genus;
    union {
        int       index;
        long long immediatum;
        double    immediatum_f;
    } datum;
} MedullaOperandum;

/* ==================================================
 * Instructio (origo semper fertur - provenientia est merx)
 * ================================================== */

typedef struct {
    int op;
    int typus;                    /* MEDULLA_TYPUS_NIHIL licet */
    int typus_secundus;           /* conversiones: typus=FONS,
                                   * typus_secundus=FINIS */
    int destinatio;               /* index registri; -1 nulla */
    int extra_index;              /* vocare: argumenta in stiva */
    int extra_numerus;
    MedullaOperandum a;
    MedullaOperandum b;
    MedullaOperandum c;
    const struct SilvaNodus* origo;
} MedullaInstructio;

/* ==================================================
 * Bloccus / functio / datum / modulus (pellucida)
 * ================================================== */

typedef struct {
    OfficinaChorda titulus;
    OfficinaXar*   instructiones;   /* MedullaInstructio valore */
    int            terminatus;
} MedullaBloccus;

typedef struct {
    int index;                    /* registri */
    int typus;
} MedullaParametrum;

typedef struct {
    OfficinaChorda titulus;
    int            typus_reditus; /* MEDULLA_TYPUS_NIHIL = vacuum */
    int            est_variadica;
    OfficinaXar*   parametra;     /* MedullaParametrum valore */
    OfficinaXar*   blocci;        /* MedullaBloccus valore */
    OfficinaXar*   registra;      /* OfficinaChorda valore */
    OfficinaXar*   operanda;      /* MedullaOperandum valore */
    const struct SilvaNodus* origo;
} MedullaFunctio;

typedef enum {
    MEDULLA_SYMBOLUM_EXTERNUM = 0,
    MEDULLA_SYMBOLUM_FUNCTIO,
    MEDULLA_SYMBOLUM_DATUM
} MedullaSymbolumGenus;

typedef struct {
    OfficinaChorda titulus;
    int            genus;
    int            index;         /* -1 externum */
} MedullaSymbolum;

typedef struct {
    unsigned int offset;          /* locellus 8 octetorum */
    int          symbolum;
} MedullaRelocatio;

typedef struct {
    OfficinaChorda titulus;
    unsigned int   magnitudo_octetorum;
    unsigned int   ordinatio;
    unsigned char* imago;         /* zephyrum-pleni initio */
    OfficinaXar*   relocationes;  /* MedullaRelocatio valore */
    const struct SilvaNodus* origo;
} MedullaDatum;

typedef struct {
    OfficinaPiscina* piscina;
    OfficinaChorda   titulus;
    OfficinaXar*     functiones;  /* MedullaFunctio* */
    OfficinaXar*     data;        /* MedullaDatum* */
    OfficinaXar*     symbola;     /* MedullaSymbolum valore */
    OfficinaXar*     causae;      /* OfficinaChorda valore */
} MedullaModulus;

/* ==================================================
 * Fabrica
 * ================================================== */

MedullaModulus* medulla_modulum_creare(OfficinaPiscina* piscina,
    OfficinaChorda titulus);

int medulla_symbolum_internare(MedullaModulus* modulus,
    OfficinaChorda titulus);
const MedullaSymbolum* medulla_symbolum_obtinere(
    const MedullaModulus* modulus, int index);

int medulla_causam_internare(MedullaModulus* modulus,
    OfficinaChorda causa);
const OfficinaChorda* medulla_causam_obtinere(
    const MedullaModulus* modulus, int index);

MedullaFunctio* medulla_functionem_creare(MedullaModulus* modulus,
    OfficinaChorda titulus, int typus_reditus, int est_variadica);

int medulla_parametrum_addere(MedullaFunctio* functio,
    OfficinaChorda titulus, int typus);
int medulla_registrum_novum(MedullaFunctio* functio,
    OfficinaChorda titulus);
const OfficinaChorda* medulla_registrum_titulus(
    const MedullaFunctio* functio, int index);

int medulla_bloccum_creare(MedullaFunctio* functio,
    OfficinaChorda titulus);
MedullaBloccus* medulla_bloccum_obtinere(
    const MedullaFunctio* functio, int index);

int medulla_operanda_addere(MedullaFunctio* functio,
    const MedullaOperandum* operanda, int numerus);

MedullaOperandum medulla_op_nihil(void);
MedullaOperandum medulla_op_registrum(int index);
MedullaOperandum medulla_op_immediatum(long long valor);
MedullaOperandum medulla_op_immediatum_f(double valor);
MedullaOperandum medulla_op_symbolum(int index);
MedullaOperandum medulla_op_bloccum(int index);
MedullaOperandum medulla_op_causa(int index);

int medulla_emittere(MedullaFunctio* functio, int bloccus,
    const MedullaInstructio* instructio);

int medulla_op_terminator(int op);
int medulla_functio_terminata(const MedullaFunctio* functio);

MedullaDatum* medulla_datum_creare(MedullaModulus* modulus,
    OfficinaChorda titulus, unsigned int magnitudo_octetorum,
    unsigned int ordinatio);
int medulla_datum_scribere(MedullaDatum* datum, unsigned int offset,
    const unsigned char* octeti, unsigned int numerus);
int medulla_relocationem_addere(MedullaDatum* datum,
    unsigned int offset, int symbolum);

/* ==================================================
 * Forma textualis (quod scriptor evomit, canon est)
 * ================================================== */

const char* medulla_op_titulus(int op);
const char* medulla_typum_titulus(int typus);

OfficinaChorda medulla_textum_scribere(OfficinaPiscina* piscina,
    const MedullaModulus* modulus);

/* NIHIL + linea_erroris (1-basis; NULL licet) in errore */
MedullaModulus* medulla_textum_legere(OfficinaPiscina* piscina,
    OfficinaChorda textus, unsigned int* linea_erroris);

/* ==================================================
 * Demissio (M1b): arbor typata -> medulla
 *
 * Typi silvae TAG-SOLUM hic (officina.h sine silva.h manet);
 * vocatores silva.h includunt (idem tags). Praesumit sem
 * POST-FLIP (typatio canonicae-relativa; fistula bis-analysans
 * vocantis est). sem non-const: forma pigra + aestimator
 * tabulas per quaestionem mutant. Inlowerabilia -> sistere
 * causa classificata; numquam ruit.
 * ================================================== */

struct SilvaParsura;
struct SilvaSemantica;

MedullaModulus* demissio_currere(OfficinaPiscina* piscina,
    const struct SilvaParsura* parsura,
    struct SilvaSemantica* sem, OfficinaChorda titulus_moduli);

#endif /* OFFICINA_H */
