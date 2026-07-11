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
 * Lineae distillatae (M2a; proto-indicium M3)
 *
 * origo = CLAVIS OPACA (numquam dereferenda post liberationem
 * frontis). Distillatio: demissio_lineas_colligere infra.
 * ================================================== */

typedef struct {
    OfficinaPiscina* piscina;
    OfficinaXar*     viae;    /* OfficinaChorda valore */
    void*            lineae;  /* tabula interna (typus anonymus
                               * vendicatus - void* pellucide,
                               * exemplar Xar layout-exactum) */
} MedullaLineae;

MedullaLineae* medulla_lineas_creare(OfficinaPiscina* piscina);
int medulla_lineam_ponere(MedullaLineae* lineae,
    const struct SilvaNodus* origo, OfficinaChorda via,
    unsigned int linea);
/* via_out/linea_out NULL licent (probatio existentiae) */
int medulla_lineam_quaerere(const MedullaLineae* lineae,
    const struct SilvaNodus* origo, OfficinaChorda* via_out,
    unsigned int* linea_out);

/* ==================================================
 * Regio (M2a): memoria possessa machinulae
 *
 * UNA reservatio mmap magna cum BASI FIXA (determinismus);
 * areae globalia | stiva | acervus. Monstratores veri SUNT
 * monstratores machinulae. UNA regio uno tempore (figura).
 * ================================================== */

typedef struct Regio Regio;

Regio* regio_generare(OfficinaPiscina* piscina);
void regio_destruere(Regio* regio);

void* regio_basis(const Regio* regio);
size_t regio_magnitudo_tota(const Regio* regio);
int regio_continet(const Regio* regio, const void* locus);

void regio_custodiam_ponere(Regio* regio, int custodia);
int regio_custodia(const Regio* regio);

void* regio_globalia_allocare(Regio* regio,
    size_t magnitudo_octetorum, size_t ordinatio);
size_t regio_globalia_usus(const Regio* regio);

void* regio_stiva_initium(const Regio* regio);
size_t regio_stiva_magnitudo_octetorum(const Regio* regio);

void* regio_allocare(Regio* regio, size_t n);
int regio_liberare(Regio* regio, void* locus);
void* regio_reallocare(Regio* regio, void* locus, size_t n);

size_t regio_acervus_usus(const Regio* regio);
size_t regio_acervus_apex(const Regio* regio);
size_t regio_numerus_allocationum(const Regio* regio);
size_t regio_numerus_liberationum(const Regio* regio);

/* ==================================================
 * Conexio (M2a): nexor moduli medullae
 *
 * Tabula symbolorum globalis + translatio per modulum (moduli
 * IMMUTABILES); nectere mundum; decipulae pigrae (ruunt solum
 * si vocatae); descriptores 16-octetorum (inscriptio EST valor
 * monstratoris functionis; signum = commissura nativa M5);
 * relocatio ADDITIVA; cellae externae (ansae 0/1/2 + errno).
 * ================================================== */

typedef enum {
    CONEXIO_SYMBOLUM_IGNOTUM = 0,
    CONEXIO_SYMBOLUM_FUNCTIO,
    CONEXIO_SYMBOLUM_DATUM,
    CONEXIO_SYMBOLUM_CELLA,
    CONEXIO_SYMBOLUM_DECIPULA
} ConexioSymbolumGenus;

#define CONEXIO_SIGNUM_INTERPRETATUM 1
#define CONEXIO_SIGNUM_DECIPULA      2

typedef struct {
    long long signum;
    long long index;
} ConexioDescriptor;

typedef struct {
    OfficinaChorda titulus;
    int            genus;
    int            modulus_index;   /* -1 nullus */
    int            index_localis;   /* -1 */
    void*          sedes;
} ConexioSymbolum;

typedef struct {
    MedullaFunctio* functio;
    int             modulus_index;
} ConexioFunctioNexa;

typedef struct Conexio Conexio;

Conexio* conexio_creare(OfficinaPiscina* piscina, Regio* regio);
int conexio_modulum_addere(Conexio* conexio,
    MedullaModulus* modulus);
int conexio_nectere(Conexio* conexio);
const OfficinaChorda* conexio_querela(const Conexio* conexio);
const OfficinaChorda* conexio_querela_symbolum(
    const Conexio* conexio);

int conexio_symbolum_quaerere(const Conexio* conexio,
    OfficinaChorda titulus);
const ConexioSymbolum* conexio_symbolum_obtinere(
    const Conexio* conexio, int index);
unsigned int conexio_numerus_symbolorum(const Conexio* conexio);
void* conexio_sedes_quaerere(const Conexio* conexio,
    OfficinaChorda titulus);

int conexio_symbolum_globale(const Conexio* conexio,
    int modulus_index, int index_localis);
const ConexioFunctioNexa* conexio_functionem_obtinere(
    const Conexio* conexio, long long index);
unsigned int conexio_numerus_functionum(const Conexio* conexio);

unsigned int conexio_numerus_modulorum(const Conexio* conexio);
const MedullaModulus* conexio_modulum_obtinere(
    const Conexio* conexio, int index);

unsigned int conexio_numerus_datorum(const Conexio* conexio);
unsigned int conexio_numerus_cellarum(const Conexio* conexio);
unsigned int conexio_numerus_decipularum(const Conexio* conexio);
const OfficinaChorda* conexio_decipulam_obtinere(
    const Conexio* conexio, int index);

/* ==================================================
 * Machinula (M2b): interpres medullae
 *
 * Canonicum-in-scriptura; recordator volatus semper activus
 * (relatio halitus ad stderr); aedificata tabula statica interna
 * (series publica ad M4); exit = vexillum; sistere exsecutum =
 * halitus cum causa.
 * ================================================== */

typedef enum {
    MACHINULA_BENE = 0,
    MACHINULA_SISTERE,
    MACHINULA_DECIPULA,
    MACHINULA_VITIUM,
    MACHINULA_PAUSA   /* punctum tactum - resumabile (pergere) */
} MachinulaExitusGenus;

typedef struct {
    int            genus;
    long long      codex;
    OfficinaChorda nuntius;
} MachinulaExitus;

typedef struct Machinula Machinula;

Machinula* machinula_creare(OfficinaPiscina* piscina,
    Conexio* conexio, Regio* regio);
void machinula_lineas_praebere(Machinula* machinula,
    int modulus_index, const MedullaLineae* lineae);
MachinulaExitus machinula_currere(Machinula* machinula,
    OfficinaChorda titulus_functionis);
unsigned long long machinula_numerus_instructionum(
    const Machinula* machinula);
unsigned long long machinula_numerus_op(const Machinula* machinula,
    int op);
unsigned long long machinula_numerus_vocationum(
    const Machinula* machinula);
unsigned long long machinula_numerus_aedificatorum(
    const Machinula* machinula);
size_t machinula_stiva_apex(const Machinula* machinula);

/* M3 chunk 4: gressus + puncta + inspectio (superficies vindicis).
 * aperire = status + tabulatum primum; gradus = instructio UNA;
 * pergere = usque ad halitum/pausam (genus redditur; resumptio ex
 * pausa punctum transit). Puncta: patching exemplaris congelati -
 * sedes (index functionis conexionis, index planus; indicium
 * easdem coordinatas dat). tabulatum_index: 0 = imum. */
int machinula_aperire(Machinula* machinula,
    OfficinaChorda titulus_functionis);
int machinula_gradus(Machinula* machinula);
int machinula_pergere(Machinula* machinula);
int machinula_punctum_ponere(Machinula* machinula,
    int functio_index, unsigned int instructio);
int machinula_punctum_tollere(Machinula* machinula,
    int functio_index, unsigned int instructio);
long long machinula_halitus_codex(const Machinula* machinula);
unsigned int machinula_tabulata_numerus(const Machinula* machinula);
int machinula_positionem_inspicere(const Machinula* machinula,
    unsigned int tabulatum_index, int* functio_index_out,
    unsigned int* instructio_out);
int machinula_registrum_legere(const Machinula* machinula,
    unsigned int tabulatum_index, unsigned int index_registri,
    unsigned long long* valor_out);

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

/* Distillatio linearum (M2a): DUM frons vivit (expansio
 * necessaria); post hanc arbores liberari possunt. */
MedullaLineae* demissio_lineas_colligere(OfficinaPiscina* piscina,
    const MedullaModulus* modulus,
    const struct SilvaParsura* parsura);

/* ==================================================
 * Indicium (M3): forma debug plana - sectiones typatae, indices
 * i32, chorda-in-disco; coquitur tempore compilationis (dum
 * parsura vivit), functio_idx == index functionis conexionis.
 * Tabula linearum prorsum: acies macronum = introitus consecutivi
 * eodem indice instructionis, radix (invocatio) prima. Endian
 * hospitis, catena instrumentorum haec sola; versio dure reiecta.
 * ================================================== */

#define INDICIUM_MAGICA        "INDICIUM"
#define INDICIUM_VERSIO        1
#define INDICIUM_TYPUS_IGNOTUS (-2)

typedef enum {
    INDICIUM_SECTIO_CHORDAE_DATA = 0,
    INDICIUM_SECTIO_CHORDAE_OFFSETA,
    INDICIUM_SECTIO_VIAE,
    INDICIUM_SECTIO_MODULI,
    INDICIUM_SECTIO_FUNCTIONES,
    INDICIUM_SECTIO_LINEAE_INDICES,
    INDICIUM_SECTIO_LINEAE_RES,
    INDICIUM_SECTIO_RETRO_LINEAE,
    INDICIUM_SECTIO_RETRO_INTERVALLA,
    INDICIUM_SECTIO_RETRO_SITUS,
    INDICIUM_SECTIO_VARIABILIA,
    INDICIUM_SECTIO_NUMERUS
} IndiciumSectioGenus;

typedef struct {
    unsigned char magica[8];
    unsigned int  versio;
    unsigned int  numerus_sectionum;
} IndiciumCaput;

typedef struct {
    unsigned int offset;
    unsigned int magnitudo_octetorum;
} IndiciumSectio;

typedef struct {
    unsigned int titulus;
    unsigned int retro_lineae_primus;
    unsigned int retro_intervalla_primus;
    unsigned int retro_numerus;
} IndiciumVia;

typedef struct {
    unsigned int titulus;
} IndiciumModulus;

typedef struct {
    unsigned int titulus;
    unsigned int modulus;
    unsigned int instructiones_numerus;
    unsigned int lineae_indices_primus;
    unsigned int lineae_res_primus;
    unsigned int lineae_numerus;
    unsigned int variabilia_prima;
    unsigned int variabilia_numerus;
} IndiciumFunctio;

typedef struct {
    unsigned int via;
    unsigned int linea;
    unsigned int profunditas;
    unsigned int nomen_macro;
} IndiciumLinea;

typedef struct {
    unsigned int functio;
    unsigned int instructio;
} IndiciumSitus;

typedef struct {
    unsigned int titulus;
    unsigned int index_registri;
    int          typus_medulla;
    unsigned int typus_scriptus;
} IndiciumVariabile;

typedef struct IndiciumScriptor IndiciumScriptor;
typedef struct IndiciumLector IndiciumLector;

IndiciumScriptor* indicium_scriptor_creare(OfficinaPiscina* piscina);
int indicium_modulum_colligere(IndiciumScriptor* scriptor,
    const MedullaModulus* modulus,
    const struct SilvaParsura* parsura,
    struct SilvaSemantica* sem);
int indicium_scribere(IndiciumScriptor* scriptor,
    const Conexio* conexio, const char* via);

IndiciumLector* indicium_aperire(OfficinaPiscina* piscina,
    const char* via);
OfficinaChorda indicium_chorda(const IndiciumLector* lector,
    unsigned int index);
unsigned int indicium_functiones_numerus(
    const IndiciumLector* lector);
const IndiciumFunctio* indicium_functio(
    const IndiciumLector* lector, unsigned int index);
int indicium_functionem_quaerere(const IndiciumLector* lector,
    OfficinaChorda titulus);
unsigned int indicium_lineas_de_instructione(
    const IndiciumLector* lector, unsigned int functio_index,
    unsigned int instructio, const IndiciumLinea** acies_out);
unsigned int indicium_situs_de_linea(const IndiciumLector* lector,
    OfficinaChorda via, unsigned int linea,
    const IndiciumSitus** acies_out);
unsigned int indicium_variabilia_functionis(
    const IndiciumLector* lector, unsigned int functio_index,
    const IndiciumVariabile** acies_out);

/* numerus instructionum exemplaris congelati (pactum ambulationis
 * canonicae: == instructiones_numerus indicii) */
unsigned int machinula_numerus_instructionum_planarum(
    const Machinula* machinula, int functio_index);

#endif /* OFFICINA_H */
